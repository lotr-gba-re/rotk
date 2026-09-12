#include "actor/lootPile.h"
#include "actor.h"
#include "combat.h"
#include "font.h"
#include "gba_io.h"
#include "input.h"
#include "item.h"
#include "libc.h"
#include "loot.h"
#include "match_hacks.h"
#include "math.h"
#include "mission.h"
#include "player.h"
#include "rng.h"
#include "save.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"
#include "vector2.h"

/** Animation pulse period step in frames; the period is rng_rollRange(1, 3) times this. */
#define LOOT_PULSE_PERIOD_STEP 30

/**
 * Create a loot pile actor from its spawn-group params: a scripted pickup (artifacts
 * 0..7, plain pickups 8..0xc, a rune) or a ground drop rolled by
 * loot_generateDrop, the latter on a despawn timer. Looted with the R button. An
 * artifact already collected in g_SaveGlobals.d.artifactMask is not created.
 *
 * @param groupIndex spawn group
 * @param actorIndex slot within the group
 * @return the new actor (NULL for a collected artifact or on pool exhaustion)
 *
 * @romaddress 0x0802ee0c
 */
Actor *actor_lootPile_createFromMission(u8 groupIndex, u8 actorIndex)
{
    Actor *actor = NULL;
    ActorLootPileMissionParams *params =
        (ActorLootPileMissionParams *)mission_script_getActorParams(groupIndex, actorIndex);
    u8 lootType;
    u32 roll;
    Item *item;
    u8 *unitType;

    if (((g_SaveGlobals.d.artifactMask.p >> params->lootType) & 1) == 0 &&
        (actor = actor_allocMain()) != NULL)
    {
        g_LootPileCount++;
        item = &actor->as.lootPile.item;
        item_clearSlot(&item->word);
        sprite_setPosition(actor, (s32)params->head.x << 16, (s32)params->head.y << 16);
        actor->recordType = params->head.type;
        actor->flags.p = ACTOR_FLAG_RENDER | ACTOR_FLAG_COLLISION_CHECK |
                         ACTOR_FLAG_ANIMATION_PLAYING | ACTOR_FLAG_COLLISION_PASSIVE |
                         ACTOR_FLAG_19;
        unitType = &actor->unitType;
        *unitType = LOOT_UNIT_TYPE_CONTAINER;
        actor->lastHitByPlayerIndex = KILL_CREDIT_NONE;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.bgPriority = 3;
        actor->tickCallback = actor_lootPile_tick;
        if (params->lootType < 8)
        {
            // artifact: collecting sets its artifactMask bit
            lootType = params->lootType;
            actor->triggerCallbacks[0] = actor_lootPile_triggerArtifact;
            actor->as.lootPile.despawnTimer = 0;
        }
        else if (params->lootType < LOOT_TYPE_GEM_STACK_MIN)
        {
            lootType = params->lootType;
            actor->triggerCallbacks[0] = actor_lootPile_trigger;
            actor->as.lootPile.despawnTimer = 0;
        }
        else if (params->lootType == LOOT_TYPE_RUNE)
        {
            roll = rng_rollRange(0, RUNE_NONE - 1);
            actor->as.lootPile.item.d.baseIndex = roll;
            lootType = params->lootType;
            actor->triggerCallbacks[0] = actor_lootPile_trigger;
            actor->as.lootPile.despawnTimer = 0;
        }
        else
        {
            // generated drop: contents rolled up front, despawns like a scattered drop
            Player *player;
            u32 dropPlayerIndex;
            s16 *extraTreasureStat;
            u32 bonus;
            if (g_PlayerGlobals.activePlayerCount < 2)
            {
                dropPlayerIndex = ACTIVE_PLAYER_INDEX;
                player = &PLAYER(dropPlayerIndex);
            }
            else
            {
                MATCH_PIN(u32, ext, "r1");
                roll = rng_rollRange(0, 1);
                ext = roll << 24;
                dropPlayerIndex = ext >> 24;
                player = &PLAYER(roll);
            }
            extraTreasureStat = &player->stats[STAT_EXTRA_TREASURE_PERCENT];
            bonus = params->extraTreasureBonus;
            lootType =
                loot_generateDrop(item, dropPlayerIndex, bonus + *extraTreasureStat, *unitType);
            actor->triggerCallbacks[0] = actor_lootPile_trigger;
            actor->as.lootPile.despawnTimer = DROP_DESPAWN_TIMER;
            actor->actionStateTimer = DROP_DESPAWN_TIMER;
        }
        actor->variant = lootType;
        if (actor->as.lootPile.despawnTimer != 0)
        {
            actor->behaviorState = LOOT_DROP_DESPAWNING;
        }
        else
        {
            actor->behaviorState = LOOT_DROP_IDLE;
        }
        actor->interactionRadius = LOOT_PICKUP_DISTANCE;
        actor->field_0x74 = 0;
        sprite_setAnimation(actor, &LootPileAnimations[lootType]);
        if (lootType == LOOT_TYPE_SPOON || lootType >= LOOT_TYPE_UNIQUE_WEAPON_MIN)
        {
            roll = rng_rollRange(1, 3);
            actor->field_0x56 = roll * LOOT_PULSE_PERIOD_STEP;
            actor->flags.p &= ~ACTOR_FLAG_ANIMATION_PLAYING;
            actor->as.lootPile.pulseEnabled = TRUE;
        }
        else
        {
            actor->as.lootPile.pulseEnabled = FALSE;
        }
        actor_applyTypeDefaults(actor);
    }
    return actor;
}

/**
 * Spawn a dropped item/gem actor on the ground.
 *
 * @param pos world-space position to drop at
 * @param lootType the drop's LOOT_TYPE_* (as returned by loot_generateDrop)
 * @param item the packed item handle (gem stacks carry their value in it)
 * @param despawnTimer frames until the drop despawns (0 = never)
 * @return the new actor (NULL on pool exhaustion)
 *
 * @romaddress 0x0802efe8
 */
Actor *actor_lootPile_create(Vector2Fp16 pos, u32 lootType, Item item, s32 despawnTimer)
{
    Actor *actor = actor_allocMain();
    u32 frameSize[2];
    MATCH_PIN(Item *, actorItem, "r6");

    if (actor != NULL)
    {
        g_LootPileCount++;
        actorItem = &actor->as.lootPile.item;
        item_clearSlot(&actorItem->word);
        sprite_setPosition(actor, pos.x, pos.y);
        actor->recordType = ACTOR_TYPE_LOOT_PILE;
        actor->flags.p = ACTOR_FLAG_RENDER | ACTOR_FLAG_COLLISION_CHECK | ACTOR_FLAG_TICK_ALWAYS |
                         ACTOR_FLAG_ANIMATION_PLAYING | ACTOR_FLAG_COLLISION_PASSIVE;
        actor->variant = lootType;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.bgPriority = 3;
        sprite_getAnimationFrameSize(&LootPileAnimations[lootType], frameSize, 0);
        actor->gfxSize = frameSize[0] * frameSize[1];
        actor->tickCallback = actor_lootPile_tick;
        actor->triggerCallbacks[0] = actor_lootPile_trigger;
        actor->interactionRadius = LOOT_PICKUP_DISTANCE;
        actor->field_0x74 = 0;
        *actorItem = item;
        actor->as.lootPile.despawnTimer = despawnTimer;
        actor->actionStateTimer = despawnTimer;
        actor->lastHitByPlayerIndex = KILL_CREDIT_NONE;
        if (actor->as.lootPile.despawnTimer != 0)
        {
            actor->behaviorState = LOOT_DROP_DESPAWNING;
        }
        else
        {
            actor->behaviorState = LOOT_DROP_IDLE;
        }
        sprite_setAnimation(actor, &LootPileAnimations[lootType]);
        if (lootType == LOOT_TYPE_SPOON || lootType >= LOOT_TYPE_UNIQUE_WEAPON_MIN)
        {
            actor->field_0x56 = rng_rollRange(1, 3) * 30;
            actor->flags.p &= ~ACTOR_FLAG_ANIMATION_PLAYING;
            actor->as.lootPile.pulseEnabled = TRUE;
        }
        else
        {
            actor->as.lootPile.pulseEnabled = FALSE;
        }
        actor_applyTypeDefaults(actor);
    }
    return actor;
}

/** Decode textId into the player's HUD text buffer. */
static inline void setHudText(s32 textId, u32 playerIndex)
{
    g_PlayerHuds[playerIndex].text =
        text_decodeString(textId, g_PlayerHuds[playerIndex].textBuffer);
}

/**
 * HUD text of a gem or food/herb drop (LOOT_TYPE_GEM_STACK_MIN <= variant <
 * LOOT_TYPE_ITEM_MIN): the band name. A full gem stack first formats its total into
 * "A Pile of @1 Gems".
 */
static inline void setHudTextGemsOrFood(Actor *drop, Item item, u32 playerIndex)
{
    if (drop->variant == LOOT_TYPE_GEM_STACK)
    {
        char numberBuffer[16];
        // the "@1" of "A Pile of @1 Gems" reads font slot 0
        g_PlayerHuds[playerIndex].text =
            text_formatInt(item.gem.value0 + item.gem.value1 + item.gem.value2, numberBuffer);
        font_copyStringToSlot0(g_PlayerHuds[playerIndex].text);
    }
    setHudText(drop->variant + TEXT_ID_LOOT_NAME_BASE, playerIndex);
}

/** HUD text of an item drop (variant >= LOOT_TYPE_ITEM_MIN): the item's affixed name. */
static inline void setHudTextItem(Item item, u32 playerIndex)
{
    memset(g_PlayerHuds[playerIndex].textBuffer, 0, sizeof(g_PlayerHuds[playerIndex].textBuffer));
    g_PlayerHuds[playerIndex].text =
        item_affix_formatName(g_PlayerHuds[playerIndex].textBuffer, item, 0xbf);
}

/**
 * Per-frame update of a ground drop: sets the per-player standing flags, shows the
 * HUD text ("A Pile of @1 Gems", the item name, ...) for the active player standing
 * on it, and runs the pickup/despawn state machine. LOOT_DROP_PICKED_UP stores the
 * item or flashes "no more room", then retires the drop and fires the params link.
 * LOOT_DROP_DESPAWNING expires into the same retirement. LOOT_DROP_FULL_MESSAGE
 * times the backpack-full message.
 *
 * @romaddress 0x0802f130
 */
void actor_lootPile_tick(Actor *drop)
{
    Item item = drop->as.lootPile.item;
    u8 playerIndex;
    for (playerIndex = 0; playerIndex < g_PlayerGlobals.activePlayerCount; playerIndex++)
    {
        Actor *playerActor = PLAYER(playerIndex).ownerActor;
        if ((g_PlayerHuds[playerIndex].flags.p & PLAYERHUD_FLAG_0) == 0)
        {
            if (actor_distance(drop, playerActor) < LOOT_PICKUP_DISTANCE)
            {
                if (drop->as.lootPile.standingFlags[playerIndex] == 0)
                {
                    if (playerIndex == ACTIVE_PLAYER_INDEX)
                    {
                        if (drop->variant < LOOT_TYPE_GEM_STACK_MIN)
                        {
                            setHudText(drop->variant + TEXT_ID_LOOT_NAME_BASE, playerIndex);
                            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_RED;
                        }
                        else if (drop->variant < LOOT_TYPE_ITEM_MIN)
                        {
                            setHudTextGemsOrFood(drop, item, playerIndex);
                            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_YELLOW;
                        }
                        else if (drop->variant == LOOT_TYPE_ENT_WATER)
                        {
                            setHudText(TEXT_ID_ENT_WATER, playerIndex);
                            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_GREEN;
                        }
                        else if (drop->variant == LOOT_TYPE_RUNE)
                        {
                            setHudText(item.d.baseIndex + TEXT_ID_RUNE_NAME_BASE, playerIndex);
                            g_PlayerHuds[playerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_RED;
                        }
                        else
                        {
                            setHudTextItem(item, playerIndex);
                        }
                    }
                    g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_1;
                }
                g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_0;
                drop->as.lootPile.standingFlags[playerIndex] = 1;
            }
            else
            {
                drop->as.lootPile.standingFlags[playerIndex] = 0;
            }
        }
        else
        {
            drop->as.lootPile.standingFlags[playerIndex] = 0;
        }
    }

    switch (drop->behaviorState)
    {
        // HACK: this temp variable is required for matching
        u16 lootPileCountTemp;
    case LOOT_DROP_PICKED_UP:
        lootPileCountTemp = g_LootPileCount - 1;
        g_LootPileCount = lootPileCountTemp;

        if (drop->variant > LOOT_TYPE_LEGOLAS_HERB_LVL_5 && drop->lastHitByPlayerIndex < 2 &&
            drop->variant != LOOT_TYPE_ENT_WATER && drop->variant != LOOT_TYPE_RUNE)
        {
            if (player_addItemToInventory(item, drop->lastHitByPlayerIndex))
            {
                sfx_play(SFX_ITEM_PICKUP);
                if ((ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_UNIQUE_WEAPON) != 0)
                {
                    Player *players = g_PlayerGlobals.players;
                    Player *player = &players[drop->lastHitByPlayerIndex];
                    player->uniqueWeaponsCollected |=
                        1 << (drop->variant - LOOT_TYPE_UNIQUE_WEAPON_MIN);
                }
                else if ((ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CARRIED_PASSIVE) != 0 &&
                         item.d.baseIndex != 0x14 && item.d.baseIndex != 0x17)
                {
                    // 0x14/0x17: carried passives without a collected bit
                    Player *players = g_PlayerGlobals.players;
                    Player *player = &players[drop->lastHitByPlayerIndex];
                    player->uniquePassivesCollected |=
                        1 << (drop->variant - LOOT_TYPE_UNIQUE_PASSIVE_MIN);
                }
            }
            else
            {
                // backpack full: flash the message, stay lootable
                sfx_play(SFX_PICKUP_DENIED);
                g_PlayerHuds[drop->lastHitByPlayerIndex].text =
                    text_getString(TEXT_ID_NO_MORE_ROOM_IN_YOUR_BACKPACK);
                g_PlayerHuds[drop->lastHitByPlayerIndex].textColor = HUD_TEXT_COLOR_GAMEPLAY_WHITE;
                g_PlayerHuds[drop->lastHitByPlayerIndex].flags.p |= PLAYERHUD_FLAG_1;
                g_PlayerHuds[drop->lastHitByPlayerIndex].flags.p |= PLAYERHUD_FLAG_0;
                drop->field_0x54 = 0x96;
                drop->behaviorState = LOOT_DROP_FULL_MESSAGE;
                drop->lastHitByPlayerIndex = KILL_CREDIT_NONE;
                return;
            }
        }
        drop->flags.p = (drop->flags.p & ~ACTOR_FLAG_RENDER) | ACTOR_FLAG_PENDING_REMOVE |
                        ACTOR_FLAG_ANIMATION_FRAME_CHANGED;
        MATCH_MEM_BARRIER();
        if ((s32)drop->flags.p < 0)
        {
            ActorLootPileMissionParams *params =
                (ActorLootPileMissionParams *)mission_script_getActorParams(drop->recordGroup,
                                                                            drop->recordIndex);
            mission_script_spawnGroupAndStartStream(params->linkGroup, params->linkStream);
        }
        break;
    case LOOT_DROP_DESPAWNING:
        if (drop->actionStateTimer-- == 0)
        {
            drop->behaviorState = LOOT_DROP_PICKED_UP;
            break;
        }
        // fall through
    case LOOT_DROP_IDLE:
        if (drop->as.lootPile.pulseEnabled)
        {
            actor_lootPile_pulseAnimation(drop);
        }
        break;
    case LOOT_DROP_FULL_MESSAGE:
        drop->field_0x54--;
        if (drop->field_0x54 == -1)
        {
            if (drop->as.lootPile.despawnTimer != 0)
            {
                drop->behaviorState = LOOT_DROP_DESPAWNING;
            }
            else
            {
                drop->behaviorState = LOOT_DROP_IDLE;
            }
            for (playerIndex = 0; playerIndex < g_PlayerGlobals.activePlayerCount; playerIndex++)
            {
                drop->as.lootPile.standingFlags[playerIndex] = 0;
            }
        }
        else if (drop->as.lootPile.pulseEnabled)
        {
            actor_lootPile_pulseAnimation(drop);
        }
        break;
    }
    drop->field_0x64[0] = 0;
}

/**
 * Pickup-input check of a ground drop for one player: on R press while standing on it
 * (as.lootPile.standingFlags), applies consumables immediately (gems, food/herb heals,
 * ent water, runes) and enters the picked-up state. Items are stored when
 * actor_lootPile_tick finalizes that state. An already-owned unique item, ent water, or rune
 * refuses the pickup.
 *
 * @param playerActor the player actor to check (its playerIndex selects the key slot)
 *
 * @romaddress 0x0802f580
 */
void actor_lootPile_trigger(Actor *drop, Actor *playerActor)
{
    Item item = drop->as.lootPile.item;
    u32 playerIndex = playerActor->as.combat.playerIndex;

    if (playerActor->recordType != 0)
    {
        return;
    }
    if ((PLAYER_KEYS_PRESSED(playerIndex) & R_BUTTON) == 0)
    {
        return;
    }
    if (drop->as.lootPile.standingFlags[playerIndex] == 0)
    {
        return;
    }
    if (drop->behaviorState == LOOT_DROP_PICKED_UP)
    {
        return;
    }
    if (item.d.itemType != ITEM_TYPE_EMPTY)
    {
        // an already-collected unique weapon refuses the pickup.
        // BUG: the CARRIED_PASSIVE test also reads uniqueWeaponsCollected with the
        // weapon-band bit index; passive variants probe bits 10..25, which are never
        // set (shifts of 16+ on the u16 read as 0), so passives are never refused.
        if ((ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_UNIQUE_WEAPON) != 0 &&
            ((PLAYER(playerIndex).uniqueWeaponsCollected >>
              (drop->variant - LOOT_TYPE_UNIQUE_WEAPON_MIN)) &
             1) != 0)
        {
            return;
        }
        if ((ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CARRIED_PASSIVE) != 0 &&
            ((PLAYER(playerIndex).uniqueWeaponsCollected >>
              (drop->variant - LOOT_TYPE_UNIQUE_WEAPON_MIN)) &
             1) != 0)
        {
            return;
        }
    }
    switch (drop->variant)
    {
    case LOOT_TYPE_GEM_STACK_MIN:
    case LOOT_TYPE_GEM_STACK: {
        u32 gems = PLAYER(playerIndex).gems + item.gem.value0 + item.gem.value1 + item.gem.value2;
        if (gems > 0xfffe)
        {
            PLAYER(playerIndex).gems = 0xffff;
        }
        else
        {
            PLAYER(playerIndex).gems = gems;
        }
        sfx_play(SFX_GEM_PICKUP);
        break;
    }
    case LOOT_TYPE_FRUIT:
        actor_lootPile_eatFood(playerIndex, 25);
        break;
    case LOOT_TYPE_MUSHROOMS:
        actor_lootPile_eatFood(playerIndex, 40);
        player_addSpirit(playerIndex, 60);
        break;
    case LOOT_TYPE_DRIED_KINGSFOIL:
        if (PLAYER(playerIndex).kingsfoilHerbs.dried < 10)
        {
            PLAYER(playerIndex).kingsfoilHerbs.dried++;
        }
        else
        {
            player_addHp(playerIndex, 25, FALSE);
        }
        sfx_play(SFX_HERB_PICKUP);
        break;
    case LOOT_TYPE_FRESH_KINGSFOIL:
        if (PLAYER(playerIndex).kingsfoilHerbs.fresh < 10)
        {
            PLAYER(playerIndex).kingsfoilHerbs.fresh++;
        }
        else
        {
            player_addHp(playerIndex, 75, FALSE);
        }
        sfx_play(SFX_HERB_PICKUP);
        break;
    case LOOT_TYPE_LEMBAS_BREAD:
        actor_lootPile_eatFood(playerIndex, 75);
        break;
    case LOOT_TYPE_LEGOLAS_HERB_LVL_1:
        player_addHp(playerIndex, 25, FALSE);
        player_addSpirit(playerIndex, 10);
        sfx_play(SFX_EAT_FOOD);
        break;
    case LOOT_TYPE_LEGOLAS_HERB_LVL_2:
        player_addHp(playerIndex, 50, FALSE);
        player_addSpirit(playerIndex, 20);
        sfx_play(SFX_EAT_FOOD);
        break;
    case LOOT_TYPE_LEGOLAS_HERB_LVL_3:
        player_addHp(playerIndex, 75, FALSE);
        player_addSpirit(playerIndex, 30);
        sfx_play(SFX_EAT_FOOD);
        break;
    case LOOT_TYPE_LEGOLAS_HERB_LVL_4:
        player_addHp(playerIndex, 100, FALSE);
        player_addSpirit(playerIndex, 40);
        sfx_play(SFX_EAT_FOOD);
        break;
    case LOOT_TYPE_LEGOLAS_HERB_LVL_5:
        player_addHp(playerIndex, 125, FALSE);
        player_addSpirit(playerIndex, 50);
        sfx_play(SFX_EAT_FOOD);
        break;
    case LOOT_TYPE_ENT_WATER:
        if ((PLAYER(playerIndex).statusFlags.p & PLAYER_STATUS_FLAG_ENT_WATER) == 0)
        {
            PLAYER(playerIndex).statusFlags.d.entWater = 1;
            sfx_play(SFX_GEM_PICKUP);
            break;
        }
        sfx_play(SFX_PICKUP_DENIED);
        return;
    case LOOT_TYPE_RUNE: {
        Player *players = g_PlayerGlobals.players;
        Player *player = &players[playerIndex];
        u16 runes = player->runes.p;
        s32 shiftedRunes;
        u8 runeIndex;
        MATCH_PIN(u32, shifted, "r0");
        MATCH_PIN(u32, one, "r4");
        // the rune index is item.d.baseIndex
        item.word <<= 16;
        runeIndex = item.word >> 24;
        shiftedRunes = runes >> runeIndex;
        one = 1;
        if ((shiftedRunes & one) == 0)
        {
            shifted = one << runeIndex;
            runes |= shifted;
            player->runes.p = runes;
            sfx_play(SFX_GEM_PICKUP);
            break;
        }
        sfx_play(SFX_PICKUP_DENIED);
        return;
    }
    default:
        // scripted pickups (kinds 8..0xc) and item kinds
        sfx_play(SFX_ITEM_PICKUP);
        break;
    }

    drop->behaviorState = LOOT_DROP_PICKED_UP;
    drop->lastHitByPlayerIndex = playerIndex;
}

/**
 * Pickup-input check of an artifact (loot types 0..7): on R press while standing on it, sets
 * its g_SaveGlobals.d.artifactMask bit and enters the picked-up state. Collecting all 8
 * sets UNLOCK_FLAG_BONUS_MAP_WEATHERTOP and rewrites the save header.
 *
 * @romaddress 0x0802f98c
 */
void actor_lootPile_triggerArtifact(Actor *drop, Actor *playerActor)
{
    if (playerActor->recordType != 0)
    {
        return;
    }
    if ((PLAYER_KEYS_PRESSED(playerActor->as.combat.playerIndex) & R_BUTTON) == 0)
    {
        return;
    }
    if (drop->as.lootPile.standingFlags[playerActor->as.combat.playerIndex] == 0)
    {
        return;
    }
    if (drop->behaviorState == LOOT_DROP_PICKED_UP)
    {
        return;
    }
    g_SaveGlobals.d.artifactMask.p |= 1 << drop->variant;
    drop->behaviorState = LOOT_DROP_PICKED_UP;
    sfx_play(SFX_ITEM_PICKUP);
    if (g_SaveGlobals.d.artifactMask.p == ARTIFACT_MASK_ALL)
    {
        g_SaveGlobals.d.unlockFlags.p |= UNLOCK_FLAG_BONUS_MAP_WEATHERTOP;
        sfx_disableAudio();
        save_writeGlobalHeader();
        sfx_enableAudio();
        sfx_play(SFX_UNLOCK);
    }
}

/**
 * Pulse a drop's animation on a random 30..90 frame period: count down the pause, restart
 * the animation on expiry, rewind it on finish. The shipped single-frame animations make
 * the pulse invisible.
 *
 * @romaddress 0x0802fa20
 */
void actor_lootPile_pulseAnimation(Actor *drop)
{
    if ((drop->flags.p & ACTOR_FLAG_ANIMATION_END_PLAYING) == ACTOR_FLAG_ANIMATION_END_PLAYING)
    {
        drop->field_0x56 = rng_rollRange(1, 3) * LOOT_PULSE_PERIOD_STEP;
        drop->flags.p &= ~ACTOR_FLAG_ANIMATION_END_PLAYING;
        sprite_setAnimationFrame(drop, 0);
    }
    else if ((drop->flags.p & ACTOR_FLAG_ANIMATION_PLAYING) == 0 && --drop->field_0x56 == -1)
    {
        drop->flags.p |= ACTOR_FLAG_ANIMATION_PLAYING;
    }
}

/**
 * Eat a food drop: heal healPercent of STAT_MAX_HP, scaled up by
 * STAT_EXTRA_HP_FROM_FOOD_PERCENT.
 *
 * @romaddress 0x0802fa90
 */
void actor_lootPile_eatFood(u32 playerIndex, u16 healPercent)
{
    u32 heal = math_percentOf(PLAYER_STAT(playerIndex, STAT_MAX_HP), healPercent);

    heal += math_percentOf(heal, (u16)PLAYER_STAT(playerIndex, STAT_EXTRA_HP_FROM_FOOD_PERCENT));
    player_addHp(playerIndex, heal, FALSE);
    sfx_play(SFX_EAT_FOOD);
}
