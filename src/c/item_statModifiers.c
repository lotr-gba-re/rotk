#include "item.h"
#include "player.h"
#include "variables.h"

#define MAINHAND (PLAYER(playerIndex).inventory.slots.mainhand)
#define OFFHAND (PLAYER(playerIndex).inventory.slots.offhand)
#define SET_COMBAT_FLAG_IF(flag, condition)                                                        \
    {                                                                                              \
        if (condition)                                                                             \
        {                                                                                          \
            PLAYER(playerIndex).combatFlags.p |= (flag);                                           \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            PLAYER(playerIndex).combatFlags.p &= ~(flag);                                          \
        }                                                                                          \
    }

/**
 * Refresh equipment-derived combat flags and mark the hand sprites dirty.
 *
 * @romaddress 0x0802d8e0
 */
void player_updateEquipFlags(u8 playerIndex)
{
    // Recognize main-hand weapons. For existing items, a clearer equivalent is:
    // MAINHAND.d.itemType != ITEM_TYPE_EMPTY &&
    //     (ITEM_BASE_METADATA(MAINHAND).flags.p & ITEM_FLAG_SLOT_MAINHAND)
    SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_MAIN_HAND_WEAPON,
                       MAINHAND.d.itemType <= ITEM_TYPE_MACE ||
                           MAINHAND.d.itemType == ITEM_TYPE_BOW ||
                           (MAINHAND.d.itemType == ITEM_TYPE_UNIQUE &&
                            (MAINHAND.d.baseIndex <= UNIQUE_ID_SILVERAXE ||
                             (MAINHAND.d.baseIndex >= UNIQUE_ID_STARFALL_BOW &&
                              MAINHAND.d.baseIndex <= UNIQUE_ID_NIGHTFELL_BOW) ||
                             (MAINHAND.d.baseIndex >= UNIQUE_ID_KING_ALDAZARS_LONGSWORD &&
                              MAINHAND.d.baseIndex <= UNIQUE_ID_GREEN_MACE_OF_THE_OUTLANDER) ||
                             (MAINHAND.d.baseIndex >= UNIQUE_ID_VELLA_OF_LORIENS_GOLDBOW &&
                              MAINHAND.d.baseIndex <= UNIQUE_ID_ORC_LUMPS_BIG_BOW))));

    // Recognize offhand weapons and staves, excluding arrows and shields.
    // For normally equipped items, a clearer spelling is:
    // OFFHAND.d.itemType != ITEM_TYPE_EMPTY &&
    //     (ITEM_BASE_METADATA(OFFHAND).flags.p & ITEM_FLAG_SLOT_OFFHAND) &&
    //     OFFHAND.d.itemType != ITEM_TYPE_ARROW && OFFHAND.d.itemType != ITEM_TYPE_SHIELD
    // The test below also recognizes T-Sword, The Witch King's Daughter, and
    // Brakash's Dwarf Axe of Hacking in the offhand, but they are 2H weapons and can't be in the
    // offhand anyway.
    SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_OFFHAND_WEAPON,
                       OFFHAND.d.itemType <= ITEM_TYPE_SWORD_1H ||
                           OFFHAND.d.itemType == ITEM_TYPE_STAFF ||
                           (OFFHAND.d.itemType == ITEM_TYPE_SWORD_2H && OFFHAND.d.baseIndex < 4) ||
                           (OFFHAND.d.itemType == ITEM_TYPE_AXE && OFFHAND.d.baseIndex < 8) ||
                           (OFFHAND.d.itemType == ITEM_TYPE_MACE && OFFHAND.d.baseIndex < 7) ||
                           (OFFHAND.d.itemType == ITEM_TYPE_UNIQUE &&
                            (OFFHAND.d.baseIndex <= UNIQUE_ID_STAFF_OF_FIVE_MAGES ||
                             (OFFHAND.d.baseIndex >= UNIQUE_ID_KING_ALDAZARS_LONGSWORD &&
                              OFFHAND.d.baseIndex <= UNIQUE_ID_OAKSTAFF_OF_OLD_THALCOS))));

#ifdef BUGFIX
    SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_MAIN_HAND_STAFF,
                       MAINHAND.d.itemType != ITEM_TYPE_EMPTY &&
                           (ITEM_BASE_METADATA(MAINHAND).flags.p & ITEM_FAMILY_STAFF));
#else
    // BUG: Unique staves never set the main-hand staff flag. The condition requires
    // their (MAINHAND.d.baseIndex == 7) AND (32 <= MAINHAND.d.baseIndex <= 34) which is impossible.
    // A corrected and clearer way of checking is shown in the BUGFIX block above.
    SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_MAIN_HAND_STAFF,
                       MAINHAND.d.itemType == ITEM_TYPE_STAFF ||
                           (MAINHAND.d.itemType == ITEM_TYPE_UNIQUE &&
                            MAINHAND.d.baseIndex == UNIQUE_ID_STAFF_OF_FIVE_MAGES &&
                            (MAINHAND.d.baseIndex >= UNIQUE_ID_CRYSTAL_CROOK_OF_JAS_MYNN &&
                             MAINHAND.d.baseIndex <= UNIQUE_ID_OAKSTAFF_OF_OLD_THALCOS)));
#endif

    // Enable the 2H damage bonus when one hand holds a weapon and the other is empty.
    SET_COMBAT_FLAG_IF(
        PLAYER_COMBAT_FLAG_TWO_HANDED,
        (MAINHAND.d.itemType == ITEM_TYPE_EMPTY &&
         (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_OFFHAND_WEAPON)) ||
            (OFFHAND.d.itemType == ITEM_TYPE_EMPTY &&
             (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_MAIN_HAND_WEAPON)));

    SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_SHIELD_EQUIPPED, OFFHAND.d.itemType == ITEM_TYPE_SHIELD);

    if (OFFHAND.d.itemType == ITEM_TYPE_ARROW)
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_ARROWS_EQUIPPED;
        SET_COMBAT_FLAG_IF(PLAYER_COMBAT_FLAG_FIRE_ARROWS_EQUIPPED,
                           OFFHAND.d.baseIndex == ARROW_INDEX_FIRE);
    }
    else
    {
        PLAYER(playerIndex).combatFlags.p &= ~PLAYER_COMBAT_FLAG_ARROWS_EQUIPPED;
        PLAYER(playerIndex).combatFlags.p &= ~PLAYER_COMBAT_FLAG_FIRE_ARROWS_EQUIPPED;
    }

    PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_HAND_SPRITES_DIRTY;
}

/**
 * Apply or remove an item's stat modifiers for an inventory slot.
 *
 * @romaddress 0x0802db90
 */
void item_applyAffixStats(Item item, u8 inventorySlot, u8 playerIndex, bool remove)
{
    s8 sign = 1;

    if (remove)
    {
        sign = -1;
    }

    if (ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CARRIED_PASSIVE)
    {
        u32 heroId = PLAYER(playerIndex).heroId;
        u32 allowed;
        u32 classFlags;
        u8 minimumLevel;

        if (heroId != HERO_ID_SMEAGOL)
        {
            if (heroId == HERO_ID_SAM)
            {
                heroId = HERO_ID_FRODO;
            }

            classFlags = ITEM_BASE_METADATA(item).flags.p & ITEM_FLAG_CLASS_ALL;
            allowed = ITEM_FLAG_CLASS_BIT(heroId) & classFlags;
            minimumLevel = item_getMinLevel(item.word);
            if (minimumLevel > PLAYER(playerIndex).level + 1)
            {
                allowed = 0;
            }

            if (allowed != 0)
            {
                item_affix_applyToPlayer(ITEM_TYPE_INFO(item).baseItems, inventorySlot, playerIndex,
                                         item.d.baseIndex, sign);
            }
        }
    }
    else
    {
        item_affix_applyToPlayer(ITEM_TYPE_INFO(item).baseItems, inventorySlot, playerIndex,
                                 item.d.baseIndex, sign);
        if ((item.bytes[0] & 0xf0) != 0xf0)
        {
            item_applyRuneStats(inventorySlot, playerIndex, item.d.runeIndex, sign);
        }
        if (ITEM_HAS_PREFIX(item) && ITEM_TYPE(item) != ITEM_TYPE_ARROW)
        {
            item_affix_applyToPlayer(ITEM_TYPE_INFO(item).prefixes, inventorySlot, playerIndex,
                                     item.d.prefixIndex, sign);
        }
        if (ITEM_HAS_SUFFIX(item))
        {
            item_affix_applyToPlayer(ITEM_TYPE_INFO(item).suffixes, inventorySlot, playerIndex,
                                     item.d.suffixIndex, sign);
        }
    }
}

/**
 * Apply or remove one base item's or affix's stat modifiers.
 *
 * @param table A base-item, prefix, or suffix table.
 *
 * @romaddress 0x0802dd10
 */
void item_affix_applyToPlayer(const ItemStatRecord *table, u8 inventorySlot, u8 playerIndex,
                              u8 index, s8 sign)
{
    if (table[index].stat0 != STAT_NONE)
    {
        player_addStat(inventorySlot, table[index].stat0, playerIndex, sign * table[index].val0);
    }
    if (table[index].stat1 != STAT_NONE)
    {
        player_addStat(inventorySlot, table[index].stat1, playerIndex, sign * table[index].val1);
    }
    if (table[index].stat2 != STAT_NONE)
    {
        player_addStat(inventorySlot, table[index].stat2, playerIndex, sign * table[index].val2);
    }
    if (table[index].stat3 != STAT_NONE)
    {
        player_addStat(inventorySlot, table[index].stat3, playerIndex, sign * table[index].val3);
    }
}

static inline void clampCurrentHpSpiritForItem(u8 playerIndex, u8 inventorySlot, Item backpackItem)
{
    // TODO: Determine why applying stats for a knife in backpack slot 0
    //       suppresses HP/spirit clamping. Legolas skill?
    if (inventorySlot != INVENTORY_SLOT_BACKPACK_0 || ITEM_TYPE(backpackItem) != ITEM_TYPE_KNIFE)
    {
        player_clampCurrentHpSpirit(playerIndex);
    }
}

/**
 * Apply an inventory item's stat modifier, preserving independent HP and spirit bonuses.
 * Offhand slash and impale damage modifiers are halved.
 *
 * @romaddress 0x0802ddc4
 */
void player_addStat(u8 inventorySlot, u8 statIndex, u8 playerIndex, s8 value)
{
    HeroBaseStats baseStats;
    s16 hpBonus;
    s16 spiritBonus;
    Item backpackItem = PLAYER(playerIndex).inventory.slots.backpack_0;

    switch (statIndex)
    {
    case STAT_STRENGTH:
    case STAT_HEALTH:
    case STAT_COURAGE:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);

        PLAYER_STAT(playerIndex, statIndex) += value;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);

        clampCurrentHpSpiritForItem(playerIndex, inventorySlot, backpackItem);
        break;

    case STAT_ALL_PRIMARY_STATS:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);

        PLAYER_STAT(playerIndex, STAT_STRENGTH) += value;
        PLAYER_STAT(playerIndex, STAT_ACCURACY) += value;
        PLAYER_STAT(playerIndex, STAT_HEALTH) += value;
        PLAYER_STAT(playerIndex, STAT_DEFENSE) += value;
        PLAYER_STAT(playerIndex, STAT_COURAGE) += value;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);

        clampCurrentHpSpiritForItem(playerIndex, inventorySlot, backpackItem);
        break;

    default:
        if (inventorySlot == INVENTORY_SLOT_OFFHAND && statIndex <= STAT_DAMAGE_IMPALE)
        {
            PLAYER_STAT(playerIndex, statIndex) += value / 2;
            return;
        }

        PLAYER_STAT(playerIndex, statIndex) += value;

        if (statIndex == STAT_SPEED_PERCENT)
        {
            PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY;
        }

        clampCurrentHpSpiritForItem(playerIndex, inventorySlot, backpackItem);
        break;
    }
}
