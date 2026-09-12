#include "actor/chest.h"
#include "actor.h"
#include "fx.h"
#include "gba_io.h"
#include "input.h"
#include "libc.h"
#include "loot.h"
#include "match_hacks.h"
#include "mission.h"
#include "player.h"
#include "rng.h"
#include "sfx.h"
#include "sprite.h"
#include "variables.h"
#include "vector2.h"

/**
 * Create a chest actor from its spawn-group params: a container the player opens with
 * the R button, scattering drops around itself.
 *
 * @param groupIndex spawn group
 * @param actorIndex slot within the group
 * @return the new actor (NULL on pool exhaustion)
 *
 * @romaddress 0x08014978
 */
Actor *actor_chest_createFromMission(u8 groupIndex, u8 actorIndex)
{
    ActorChestMissionParams *params =
        (ActorChestMissionParams *)mission_script_getActorParams(groupIndex, actorIndex);
    Actor *actor = actor_allocMain();

    if (actor != NULL)
    {
        sprite_setPosition(actor, (s32)params->head.x << 16, (s32)params->head.y << 16);
        actor->recordType = params->head.type;
        actor->flags.p =
            ACTOR_FLAG_RENDER | ACTOR_FLAG_COLLISION_CHECK | ACTOR_FLAG_COLLISION_PASSIVE;
        actor->unitType = LOOT_UNIT_TYPE_CONTAINER;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.bgPriority = 3;
        actor_applyTypeDefaults(actor);
        sprite_setAnimation(actor, &ChestAnimation);
        actor->tickCallback = actor_chest_tick;
        actor->triggerCallbacks[0] = actor_chest_trigger;
        actor->interactionRadius = 0x10;
        actor->behaviorState = 0;
        actor->as.chest.maxDrops = params->maxDrops;
        actor->as.chest.dropInterval = 10;
        actor->as.chest.guaranteedDrops = params->guaranteedDrops;
        actor->as.chest.opened = FALSE;
        actor->as.chest.spawnOnOpen = params->spawnOnOpen;
        actor->as.chest.spawnVariant = params->spawnVariant;
    }
    return actor;
}

/**
 * Chest state machine: 1 = retire, 3 = just opened (decide the drop count once the
 * opening animation reaches its end (ACTOR_FLAG_ANIMATION_END), fire the params link),
 * 4 = scattering drops (one per dropInterval frames, currentHp remaining).
 *
 * @romaddress 0x08014a3c
 */
void actor_chest_tick(Actor *actor)
{
    Item item;
    ActorChestMissionParams *params;
    s32 roll;
    s32 dx;
    s32 dy;
    Vector2Fp16 spawnPos;
    u8 dropType;
    u16 extraTreasurePercent;

    switch (actor->behaviorState)
    {
    case 1:
        actor->flags.p = (actor->flags.p & ~ACTOR_FLAG_RENDER) | ACTOR_FLAG_PENDING_REMOVE |
                         ACTOR_FLAG_ANIMATION_FRAME_CHANGED;
        break;
    case 3:
        if ((actor->flags.p & ACTOR_FLAG_ANIMATION_END) == 0)
        {
            break;
        }
        actor->flags.p &= ~ACTOR_FLAG_ANIMATION_PLAYING;
        roll = rng_rollRange(0, 100);
        // no drops on a 90+ no-guarantee roll, or when maxDrops == 0: both zero the count
        // so the roll >= 1 test below takes the no-drop branch. The negative condition
        // keeps the rolledZero block at the branch tail (byte-matched).
        if (actor->as.chest.guaranteedDrops != 0 || roll <= 89)
        {
            if (actor->as.chest.maxDrops != 0)
            {
                roll = rng_rollRange(1, actor->as.chest.maxDrops);
            }
            else
            {
                roll = 0;
            }
        }
        else
        {
            roll = 0;
        }
        if (roll >= 1)
        {
            actor->behaviorState = 4;
            actor->currentHp = roll;
            actor->actionStateTimer = actor->as.chest.dropInterval;
        }
        else
        {
            actor->behaviorState = 0;
        }
        if ((s32)actor->flags.p < 0)
        {
            params = (ActorChestMissionParams *)mission_script_getActorParams(actor->recordGroup,
                                                                              actor->recordIndex);
            mission_script_spawnGroupAndStartStream(params->linkGroup, params->linkStream);
        }
        break;
    case 4:
        if (actor->actionStateTimer-- == 0)
        {
            if (rng_rollPercent(0x32))
            {
                dx = -8;
                dy = -0x10;
            }
            else
            {
                dx = 8;
                dy = 0x10;
            }
            spawnPos.x = actor->coords.x + rng_rollRange(dx, dy) * 0x10000;
            spawnPos.y = actor->coords.y + rng_rollRange(8, 0x10) * 0x10000;
            if (actor->as.chest.guaranteedDrops != 0)
            {
                extraTreasurePercent =
                    PLAYER_STAT(actor->lastHitByPlayerIndex, STAT_EXTRA_TREASURE_PERCENT) + 0x19;
            }
            else
            {
                extraTreasurePercent =
                    PLAYER_STAT(actor->lastHitByPlayerIndex, STAT_EXTRA_TREASURE_PERCENT);
            }
            dropType = loot_generateDrop(&item, actor->lastHitByPlayerIndex, extraTreasurePercent,
                                         actor->unitType);
            actor_lootPile_create(spawnPos, dropType, item, DROP_DESPAWN_TIMER);
            actor->currentHp = (u16)actor->currentHp - 1;
            if (actor->currentHp == -1)
            {
                actor->behaviorState = 0;
            }
            else
            {
                actor->actionStateTimer = actor->as.chest.dropInterval;
            }
        }
        break;
    case 0:
        break;
    default:
        actor_runBehaviorState(actor);
        break;
    }
    actor->field_0x64[0] = 0;
}

/**
 * Contact trigger: a player pressing R while overlapping opens the chest (once),
 * spawning its on-open companion FX if the params ask for one. The tail applies the
 * generic touch reaction to the touching actor.
 *
 * @romaddress 0x08014bf8
 */
void actor_chest_trigger(Actor *chest, Actor *touching)
{
    bool *opened;
    u8 *playerIndex;

    if (touching->recordType == 0 && (opened = &chest->as.chest.opened, !*opened) &&
        (PLAYER_KEYS_PRESSED(touching->as.combat.playerIndex) & R_BUTTON) != 0)
    {
        playerIndex = &touching->as.combat.playerIndex;
        *opened = TRUE;
        chest->lastHitByPlayerIndex = *playerIndex;
        chest->behaviorState = 3;
        chest->flags.p |= ACTOR_FLAG_ANIMATION_PLAYING;
        sfx_play(SFX_CHEST_OPEN);
        if (chest->as.chest.spawnOnOpen)
        {
            fx_spawnTriggerFx(chest->coords.x, chest->coords.y, chest->as.chest.spawnVariant);
        }
    }
    if ((touching->flags.p & (ACTOR_FLAG_28 | ACTOR_FLAG_18 | ACTOR_FLAG_10)) != 0)
    {
        actor_staggerOnTouch(chest, touching);
    }
    else
    {
        actor_resolveTouch(chest, touching);
    }
}

/**
 * Proximity trigger: when a player comes within interactionRadius, freeze in place and
 * fire the behavior link once (latching behaviorState 6), then align the player
 * at the edge.
 *
 * @romaddress 0x08014ca0
 */
void actor_proximityLinkTrigger(Actor *self, Actor *player)
{
    u16 savedCooldown;

    if (player->recordType == 0)
    {
        u32 radius = (u32)self->interactionRadius << 16;
        if (vector2_distance(*(Vector2Int *)&self->renderCoords,
                             *(Vector2Int *)&player->renderCoords) <= radius)
        {
            if (self->behaviorState != 6)
            {
                s32 *vx = &self->velocity.x;
                s32 *vy = &self->velocity.y;
                *vy = 0;
                *vx = 0;
                self->renderCoords = self->coords;
                self->field_0x73 = self->behaviorState;
                self->field_0x64[0] &= 8;
                self->behaviorState = 6;
                mission_script_spawnGroupAndStartStream(self->behaviorLinkGroup,
                                                        self->behaviorLinkStream);
            }
            savedCooldown = self->field_0x74;
            self->field_0x76 = savedCooldown;
            actor_alignTouching(self, player);
        }
    }
}
