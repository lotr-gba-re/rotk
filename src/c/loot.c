#include "loot.h"
#include "actor.h"
#include "game.h"
#include "item.h"
#include "math.h"
#include "mission.h"
#include "player.h"
#include "rng.h"
#include "save.h"
#include "scene.h"
#include "variables.h"

/** Inlined twin of loot_rollDropChance, which the ROM also keeps as its own function. */
static inline bool rollDropChance(u8 unitType)
{
    s32 roll = rng_rollRange(1, 100);

    if (unitType < LOOT_UNIT_TYPE_CONTAINER)
    {
        roll += EnemyLootInfos[unitType].dropChanceBonus;
    }
    if (roll > LOOT_DROP_THRESHOLD)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Quality-roll upper bound for a health drop; above it the drop is an equippable item
 * (at or below LOOT_DROP_THRESHOLD it is a gem stack).
 */
#define HEALTH_DROP_QUALITY_MAX 70

/**
 * Base prefix chance for drops without an EnemyLootInfo (containers), also used for the
 * guaranteed-prefix case (extraTreasurePercent 0xff).
 */
#define PREFIX_BASE_CHANCE 20

// 1..100 roll bounds shifting the prefix region to the hero's previous/next campaign
// region (LootHeroRegionProgressions).
#define PREFIX_PREVIOUS_REGION_ROLL_MAX 10
#define PREFIX_NEXT_REGION_ROLL_MIN 95

// Base suffix chance per difficulty.
#define SUFFIX_CHANCE_NORMAL 2
#define SUFFIX_CHANCE_HARD 3
#define SUFFIX_CHANCE_GRUELING 4

// Affix hiIndex penalty per difficulty below Grueling.
#define LOOT_AFFIX_TIER_PENALTY_HARD 2
#define LOOT_AFFIX_TIER_PENALTY_NORMAL 4

// Stack value: base roll 1..13 plus 5 per loot tier, capped at 755.
#define GEM_STACK_BASE_ROLL_MAX 13
#define GEM_STACK_LOOT_TIER_BONUS 5
#define GEM_STACK_VALUE_MAX 755

// The Spoon is the first knife (KnifeItems[0] / KnifeGfx[0]); a 1-in-SPOON_KEEP_ROLL
// roll keeps it as a drop.
#define KNIFE_INDEX_SPOON 0
#define SPOON_KEEP_ROLL 100

// 1-in-N keep rolls for the unique bands (rng_rollRange(1, N) == N keeps the item).
#define UNIQUE_WEAPON_KEEP_ROLL 20
#define UNIQUE_CARRIED_PASSIVE_KEEP_ROLL 10
#define UNIQUE_REPEATABLE_KEEP_ROLL 10
#define ENT_WATER_KEEP_ROLL 5

// Reroll for a unique that was not kept, 1..100: below ORC_HEAD_MAX an Orc Head, below
// ORC_DRUM_MAX an Orc Drum, else a gem stack worth loot_scaleGemDrop + UNIQUE_REROLL_GEM_BONUS.
#define UNIQUE_REROLL_ORC_HEAD_MAX 60
#define UNIQUE_REROLL_ORC_DRUM_MAX 70
#define UNIQUE_REROLL_GEM_BONUS 250

// Dropped arrow stack size range (carried in Item.d.prefixIndex).
#define ARROW_DROP_STACK_MIN 10
#define ARROW_DROP_STACK_MAX 40

/**
 * Roll whether a slain actor drops loot, and if so generate the drop and spawn it on the
 * ground at the actor's position. In co-op (GAME_FLAG_COOP_ACTIVE) the killer's magic-find
 * stats are used; solo always uses player 0.
 *
 * @param actor the dying actor (its unitType indexes EnemyLootInfos)
 *
 * @romaddress 0x0803a6e8
 */
void loot_rollDrop(Actor *actor)
{
    Item item;
    u8 playerIndex;

    playerIndex = actor->lastHitByPlayerIndex;
    if (!(g_GameFlags.p & GAME_FLAG_COOP_ACTIVE))
    {
        playerIndex = 0;
    }
    if (playerIndex < 2)
    {
        if (rollDropChance(actor->unitType))
        {
            u8 kind = loot_generateDrop(&item, playerIndex,
                                        PLAYER_STAT(playerIndex, STAT_EXTRA_TREASURE_PERCENT),
                                        actor->unitType);
            actor_lootPile_create(actor->coords, kind, item, DROP_DESPAWN_TIMER);
        }
    }
}

/**
 * Core drop generator: clears itemOut, makes the per-enemy quality roll, and dispatches to
 * a gem stack, a health drop, or a full equippable item (loot_generateItem).
 *
 * @param itemOut packed item handle to fill in
 * @param playerIndex whose magic-find stats apply
 * @param extraTreasurePercent the player's STAT_EXTRA_TREASURE_PERCENT
 * @param unitType Actor.unitType of the drop source (LOOT_UNIT_TYPE_CONTAINER = container)
 * @return loot type: ItemGfx.lootType for an item, LOOT_TYPE_GEM_STACK* gem stack,
 *         LOOT_TYPE_FRUIT..LOOT_TYPE_LEMBAS_BREAD health drop
 *
 * @romaddress 0x0803a77c
 */
u8 loot_generateDrop(Item *itemOut, u8 playerIndex, s16 extraTreasurePercent, u8 unitType)
{
    u8 kind = 0;

    item_clearSlot((u32 *)itemOut);

    // Scripted missions: certain enemy types drop a health drop outright.
    if (((g_CurrentMissionId == MISSION_ITHILIEN_BLACK_GATE && g_MissionVariant == 1) ||
         (g_CurrentMissionId == MISSION_PELENNOR_BATTLE_WITCH_KING && g_MissionVariant == 4)) &&
        unitType > 5 && unitType < 10)
    {
        kind = loot_rollHealthDrop();
    }
    else
    {
        s32 roll;

        if (unitType < LOOT_UNIT_TYPE_CONTAINER)
        {
            roll = rng_rollRange(EnemyLootInfos[unitType].qualityLo + 1,
                                 EnemyLootInfos[unitType].qualityHi + 100);
        }
        else if (unitType == LOOT_UNIT_TYPE_CONTAINER)
        {
            roll = rng_rollRange(0x47, 0x61);
        }
        else
        {
            roll = rng_rollRange(1, 100);
        }

        if (roll <= LOOT_DROP_THRESHOLD)
        {
            kind = loot_rollGemStack(itemOut, playerIndex);
        }
        else if (roll <= HEALTH_DROP_QUALITY_MAX)
        {
            kind = loot_rollHealthDrop();
        }
        else if (roll <= 100)
        {
            kind = loot_generateItem(itemOut, playerIndex, extraTreasurePercent, unitType);
        }
        // roll > 100: leave kind at 0 (no drop)
    }

    return kind;
}

/**
 * A rolled item with ITEM_FLAG_ANCIENT is locked unless UNLOCK_FLAG_ANCIENT_ITEMS is set in
 * the given unlock-flag word; locked picks downgrade to the previous base item (clamped at 0).
 * The per-caller ITEM_FLAG_ANCIENT test stays at the call site; this shared tail merges.
 */
static inline void downgradeLockedBase(Item *item, UnlockFlags unlockFlags)
{
    if (!(unlockFlags.p & UNLOCK_FLAG_ANCIENT_ITEMS))
    {
        s32 dec = item->d.baseIndex - 1;
        item->d.baseIndex = (u8)dec;
        if ((u8)dec == 0xff)
        {
            item->d.baseIndex = 0;
        }
    }
}

/**
 * Pick the base item (item->d.baseIndex) for an item handle whose itemType is already set,
 * filtered to base-item tiers the current loot tier allows, and apply unlock-flag and
 * special gem-stack (itemType 0xe) handling.
 *
 * @return the picked base item's ItemGfx.lootType
 *
 * @romaddress 0x0803a844
 */
u32 loot_rollBaseItem(Item *item, u8 playerIndex)
{
    u32 lootType;
    s32 max;
    s32 roll;
    bool reroll = TRUE;
    u32 entryType = ITEM_TYPE(*item);

    // uniques skip the tier filter: every UniqueItems entry is eligible
    max = UNIQUE_ID_NICKOHARES_FOOT;
    if (entryType != ITEM_TYPE_UNIQUE)
    {
        // highest base item whose tier the current loot tier allows
        for (max = 0; max < (s32)ITEM_TYPE_INFO(*item).baseItemCount; max++)
        {
            if (ITEM_TYPE_INFO(*item).baseItems[max].tier > g_LootTier)
            {
                if (--max < 0)
                {
                    max = 0;
                }
                break;
            }
        }
        if (max >= (s32)ITEM_TYPE_INFO(*item).baseItemCount)
        {
            max = (s32)ITEM_TYPE_INFO(*item).baseItemCount - 1;
        }
    }

    roll = rng_rollRange(0, max);
    item->d.baseIndex = roll;

    // Locked base items (ITEM_FLAG_ANCIENT) require the ancient-items unlock; the unlock
    // flag word differs between solo (save-global) and co-op.
    if ((g_GameFlags.p & GAME_FLAG_COOP_ACTIVE) == 0)
    {
        if (ITEM_BASE_METADATA(*item).flags.p & ITEM_FLAG_ANCIENT)
        {
            downgradeLockedBase(item, g_SaveGlobals.d.unlockFlags);
        }
    }
    else
    {
        if (ITEM_BASE_METADATA(*item).flags.p & ITEM_FLAG_ANCIENT)
        {
            downgradeLockedBase(item, g_CoopUnlockFlags);
        }
    }

    if (item->d.itemType == ITEM_TYPE_KNIFE && item->d.baseIndex == KNIFE_INDEX_SPOON)
    {
        roll = rng_rollRange(1, SPOON_KEEP_ROLL);
        if (roll != SPOON_KEEP_ROLL)
        {
            item->d.baseIndex += 1;
        }
    }

    lootType = ITEM_TYPE_INFO(*item).baseGfx[item->d.baseIndex].lootType;
    if (item->d.itemType == ITEM_TYPE_UNIQUE)
    {
        // baseIndex selects a band (boundaries carved from the UniqueItemFlags runs), each
        // with its own 1-in-N keep roll. A failed keep roll, or a kept item the player is not
        // allowed to have again, falls through to the reroll at the bottom.
        if (item->d.baseIndex < UNIQUE_WEAPON_BAND_END)
        {
            // Unique weapons: kept only while this player's uniqueWeaponsCollected bit is
            // clear. Nothing ever clears the bit, so each unique weapon drops at most once
            // per character, even if the first copy was discarded.
            roll = rng_rollRange(1, UNIQUE_WEAPON_KEEP_ROLL);
            if (roll == UNIQUE_WEAPON_KEEP_ROLL && !((PLAYER(playerIndex).uniqueWeaponsCollected >>
                                                      (lootType - LOOT_TYPE_UNIQUE_WEAPON_MIN)) &
                                                     1))
            {
                reroll = FALSE;
            }
        }
        else if (item->d.baseIndex < UNIQUE_CARRIED_PASSIVE_BAND_END)
        {
            // Carried passives: Orc Drum and Orc Head are kept on any successful keep roll.
            // Every other passive is kept only when the uniquePassivesCollected bit is SET.
            // BUG: that test is inverted relative to the weapon band, so a non-Orc passive
            // never drops for a player who does not own it yet. Every random item source
            // (enemy drops, chests, scripted piles, the Forge bargain) rolls through here, so
            // those passives are unobtainable in normal play.
            roll = rng_rollRange(1, UNIQUE_CARRIED_PASSIVE_KEEP_ROLL);
            if (roll == UNIQUE_CARRIED_PASSIVE_KEEP_ROLL &&
                (item->d.baseIndex == UNIQUE_ID_ORC_DRUM ||
                 item->d.baseIndex == UNIQUE_ID_ORC_HEAD ||
                 ((PLAYER(playerIndex).uniquePassivesCollected >>
                   (lootType - LOOT_TYPE_UNIQUE_PASSIVE_MIN)) &
                  1)))
            {
                return lootType;
            }
        }
        else if (item->d.baseIndex < UNIQUE_ITEM_COUNT)
        {
            // Repeatable uniques: keep roll only, no ownership check.
            roll = rng_rollRange(1, UNIQUE_REPEATABLE_KEEP_ROLL);
            if (roll == UNIQUE_REPEATABLE_KEEP_ROLL)
            {
                return lootType;
            }
        }
        else
        {
            // Past the UniqueItems table: a 1-in-ENT_WATER_KEEP_ROLL roll picks Ent Water,
            // kept only while the player lacks it. Any other roll picks a random rune, kept
            // only if the player has not collected it yet.
            roll = rng_rollRange(1, ENT_WATER_KEEP_ROLL);
            if (roll == ENT_WATER_KEEP_ROLL)
            {
                if (!(PLAYER(playerIndex).statusFlags.p & PLAYER_STATUS_FLAG_ENT_WATER))
                {
                    reroll = FALSE;
                    lootType = LOOT_TYPE_ENT_WATER;
                }
            }
            else
            {
                roll = rng_rollRange(0, RUNE_COUNT - 1);
                if (!((PLAYER(playerIndex).runes.p >> roll) & 1))
                {
                    item->d.baseIndex = (u8)roll;
                    reroll = FALSE;
                    lootType = LOOT_TYPE_RUNE;
                }
            }
        }

        if (reroll)
        {
            // Not kept: replace with an Orc Head, an Orc Drum, or a gem stack.
            roll = rng_rollRange(1, 100);
            if (roll < UNIQUE_REROLL_ORC_HEAD_MAX)
            {
                item->d.baseIndex = UNIQUE_ID_ORC_HEAD;
                lootType = LOOT_TYPE_UNIQUE_WEAPON_MIN + UNIQUE_ID_ORC_HEAD;
            }
            else if (roll < UNIQUE_REROLL_ORC_DRUM_MAX)
            {
                item->d.baseIndex = UNIQUE_ID_ORC_DRUM;
                lootType = LOOT_TYPE_UNIQUE_WEAPON_MIN + UNIQUE_ID_ORC_DRUM;
            }
            else
            {
                // Fall back to a scaled gem stack: mark the slot empty, pack the value.
                item->d.itemType = ITEM_TYPE_EMPTY;
                roll = loot_scaleGemDrop(item->d.baseIndex, playerIndex);
                roll += UNIQUE_REROLL_GEM_BONUS;
                loot_setGemStackValue(&item->gem, roll);
                lootType = LOOT_TYPE_GEM_STACK;
            }
        }
    }
    else if (item->d.itemType == ITEM_TYPE_ARROW)
    {
        // Arrows carry their quantity in the prefixIndex byte.
        s32 roll = rng_rollRange(ARROW_DROP_STACK_MIN, ARROW_DROP_STACK_MAX);
        item->d.prefixIndex = (u8)roll;
    }

    return lootType;
}

/**
 * Roll whether the item gets a prefix, and which one. Chance = (EnemyLootInfo
 * .prefixTierChance + loot tier) scaled by extraTreasurePercent; the prefix's region comes from
 * the current mission's region, shifted to the hero's previous (<= 10) or next (> 95)
 * campaign region by a second roll; the range's top is clamped down on lower difficulties. No-op
 * for prefix-less item types and ITEM_FLAG_NO_PREFIX base items.
 *
 * @romaddress 0x0803ab4c
 */
void loot_rollPrefix(Item *item, s16 extraTreasurePercent, u8 heroId, u8 unitType)
{
    s32 roll;
    s32 threshold;
    u32 bonus;

    if (ITEM_TYPE_INFO(*item).prefixes != NULL &&
        !(ITEM_BASE_METADATA(*item).flags.p & ITEM_FLAG_NO_PREFIX))
    {
        if (extraTreasurePercent == 0xff)
        {
            roll = 1;
            threshold = PREFIX_BASE_CHANCE;
        }
        else if (unitType < LOOT_UNIT_TYPE_CONTAINER)
        {
            roll = rng_rollRange(1, 100);
            threshold = EnemyLootInfos[unitType].prefixTierChance + g_LootTier;
        }
        else
        {
            roll = rng_rollRange(1, 100);
            threshold = g_LootTier + PREFIX_BASE_CHANCE;
        }
        bonus = math_percentOf(threshold, extraTreasurePercent);
        threshold += bonus;
        if (roll > 100)
        {
            roll = 100;
        }
        if (threshold > 100)
        {
            threshold = 100;
        }

        if (roll <= threshold)
        {
            s32 regionId;
            u8 loIndex;
            u8 hiIndex;

            regionId = mission_getRegion(g_CurrentMissionId);

            roll = rng_rollRange(1, 100);
            if (roll <= PREFIX_PREVIOUS_REGION_ROLL_MAX)
            {
                regionId = LootHeroRegionProgressions[heroId][regionId].previous;
            }
            else if (roll > PREFIX_NEXT_REGION_ROLL_MIN)
            {
                regionId = LootHeroRegionProgressions[heroId][regionId].next;
            }

            loIndex = ITEM_TYPE_INFO(*item).prefixRegionRanges[regionId].loIndex;
            hiIndex = ITEM_TYPE_INFO(*item).prefixRegionRanges[regionId].hiIndex;
            if (!(g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING))
            {
                if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
                {
                    hiIndex -= LOOT_AFFIX_TIER_PENALTY_HARD;
                }
                else
                {
                    hiIndex -= LOOT_AFFIX_TIER_PENALTY_NORMAL;
                }
            }
            roll = rng_rollRange(loIndex, hiIndex);
            item->d.prefixIndex = (u8)roll;
        }
    }
}

/**
 * Roll whether the item gets a suffix, and which one. Base chance 2%/3%/4% on
 * Normal/Hard/Grueling, scaled by extraTreasurePercent; the suffix tier is uniform in
 * 0..lootTier-1, clamped down on lower difficulties. No-op for suffix-less item types and
 * ITEM_FLAG_NO_SUFFIX base items.
 *
 * @romaddress 0x0803acc4
 */
void loot_rollSuffix(Item *item, s16 extraTreasurePercent)
{
    s32 roll;
    s32 chance;
    u32 bonus;

    if (ITEM_TYPE_INFO(*item).suffixes != NULL &&
        !(ITEM_BASE_METADATA(*item).flags.p & ITEM_FLAG_NO_SUFFIX))
    {
        roll = rng_rollRange(1, 100);
        if (roll < 0)
        {
            roll = 1;
        }
        if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING)
        {
            chance = SUFFIX_CHANCE_GRUELING;
        }
        else
        {
            u8 isHard;
            isHard = g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD;
            chance = SUFFIX_CHANCE_NORMAL;
            if (isHard)
            {
                chance = SUFFIX_CHANCE_HARD;
            }
        }

        bonus = math_percentOf(chance, extraTreasurePercent);
        chance += bonus;

        if (roll <= chance)
        {
            u8 loIndex;
            u8 hiIndex;

            roll = rng_rollRange(0, g_LootTier - 1);
            loIndex = ITEM_TYPE_INFO(*item).suffixTierRanges[roll].loIndex;
            hiIndex = ITEM_TYPE_INFO(*item).suffixTierRanges[roll].hiIndex;
            if (!(g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING))
            {
                if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_HARD)
                {
                    hiIndex -= LOOT_AFFIX_TIER_PENALTY_HARD;
                }
                else
                {
                    hiIndex -= LOOT_AFFIX_TIER_PENALTY_NORMAL;
                }
            }
            roll = rng_rollRange(loIndex, hiIndex);
            item->d.suffixIndex = (u8)roll;
        }
    }
}

/**
 * The plain drop-chance roll: 1..100 plus the enemy's EnemyLootInfo.dropChanceBonus.
 *
 * @return TRUE when roll > LOOT_DROP_THRESHOLD (a drop happens)
 *
 * @romaddress 0x0803adc4
 */
bool loot_rollDropChance(u8 unitType)
{
    s32 roll = rng_rollRange(1, 100);

    if (unitType < LOOT_UNIT_TYPE_CONTAINER)
    {
        roll += EnemyLootInfos[unitType].dropChanceBonus;
    }
    if (roll > LOOT_DROP_THRESHOLD)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Generate a gem stack into itemOut: value 1..13 + 5 x loot tier, scaled by the player's
 * gem magic-find (loot_scaleGemDrop), capped at 755, packed into the handle bytes.
 *
 * @return LOOT_TYPE_GEM_STACK_MIN for a minimal (value 1) stack, LOOT_TYPE_GEM_STACK otherwise
 *
 * @romaddress 0x0803adf8
 */
u8 loot_rollGemStack(Item *itemOut, u8 playerIndex)
{
    s32 value;
    s32 scaled;

    value = rng_rollRange(1, GEM_STACK_BASE_ROLL_MAX);
    value += g_LootTier * GEM_STACK_LOOT_TIER_BONUS;
    scaled = loot_scaleGemDrop(value, playerIndex);
    value += scaled;
    if (value > GEM_STACK_VALUE_MAX)
    {
        value = GEM_STACK_VALUE_MAX;
    }
    loot_setGemStackValue(&itemOut->gem, value);
    if (itemOut->gem.value0 == 1)
    {
        return LOOT_TYPE_GEM_STACK_MIN;
    }
    return LOOT_TYPE_GEM_STACK;
}

/**
 * Roll which health drop a drop becomes, from the weighted LootHealthDropChances table.
 * The first row whose maxRoll exceeds the 1..100 roll wins; if none does,
 * the final fallthrough row's lootType is returned.
 *
 * @return loot type LOOT_TYPE_FRUIT..LOOT_TYPE_LEMBAS_BREAD (0xf..0x13)
 *
 * @romaddress 0x0803ae48
 */
u8 loot_rollHealthDrop(void)
{
    s32 roll = rng_rollRange(1, 100);
    s32 i;

    for (i = 0; i < LootHealthDropChances.count - 1; i++)
    {
        if (roll < LootHealthDropChances.entries[i].maxRoll)
        {
            return LootHealthDropChances.entries[i].lootType;
        }
    }
    return LootHealthDropChances.entries[i].lootType;
}

/**
 * Generate a full equippable item: pick the itemType from the killing hero's per-hero
 * thresholds, then roll base item, prefix, and suffix.
 *
 * @return the base item's ItemGfx.lootType
 *
 * @romaddress 0x0803ae8c
 */
u8 loot_generateItem(Item *itemOut, u8 playerIndex, s16 extraTreasurePercent, u8 unitType)
{
    u32 heroId;
    s32 roll;
    u8 i;
    u8 category;

    heroId = PLAYER(playerIndex).heroId;
    roll = rng_rollRange(1, 100);
    i = 0;
    while (i <= ITEM_TYPE_UNIQUE && roll > LootHeroItemTypeRolls[heroId].thresholds[i])
    {
        i++;
    }
    // Containers never drop the unique-tier roll; downgrade to armor.
    if (unitType == LOOT_UNIT_TYPE_CONTAINER && i == ITEM_TYPE_UNIQUE)
    {
        i = ITEM_TYPE_ARMOR;
    }
    itemOut->d.itemType = (ItemType)i;

    category = loot_rollBaseItem(itemOut, playerIndex);
    if (itemOut->d.itemType != ITEM_TYPE_EMPTY)
    {
        loot_rollPrefix(itemOut, extraTreasurePercent, heroId, unitType);
        loot_rollSuffix(itemOut, extraTreasurePercent);
    }
    return category;
}

/**
 * The gem magic-find multiplier applied to a gem stack's base value:
 * base x (STAT_EXTRA_GEMS_PERCENT + STAT_GEMFINDER x 10) / 100.
 *
 * @romaddress 0x0803af44
 */
s32 loot_scaleGemDrop(s32 baseValue, s32 playerIndex)
{
    s32 gems = PLAYER_STAT(playerIndex, STAT_EXTRA_GEMS_PERCENT);
    s32 gemfinder = PLAYER_STAT(playerIndex, STAT_GEMFINDER);
    gems += gemfinder * 10;
    gems = math_percentOf(baseValue, gems);
    return gems;
}

/**
 * Pack a gem stack's gold value into bytes 1..3 of its handle, 0xff per byte.
 *
 * @romaddress 0x0803af80
 */
void loot_setGemStackValue(GemStack *stackOut, s32 value)
{
    if (value > 0xff)
    {
        stackOut->value0 = 0xff;
    }
    else
    {
        stackOut->value0 = value;
    }
    value -= stackOut->value0;

    if (value > 0xff)
    {
        stackOut->value1 = 0xff;
    }
    else
    {
        stackOut->value1 = value;
    }
    value -= stackOut->value1;

    if (value > 0xff)
    {
        stackOut->value2 = 0xff;
    }
    else
    {
        stackOut->value2 = value;
    }
}
