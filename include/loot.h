#pragma once

#include "item.h"
#include "mission.h"
#include "types.h"

/** Loot type enum used in loot pile visuals and pickup handler. */
typedef enum LootType
{
    LOOT_TYPE_GEM_STACK_MIN = 0xd, // minimal (value 1) stack
    LOOT_TYPE_GEM_STACK = 0xe,
    LOOT_TYPE_FRUIT = 0xf,
    LOOT_TYPE_MUSHROOMS = 0x10,
    LOOT_TYPE_DRIED_KINGSFOIL = 0x11,
    LOOT_TYPE_FRESH_KINGSFOIL = 0x12,
    LOOT_TYPE_LEMBAS_BREAD = 0x13,
    LOOT_TYPE_LEGOLAS_HERB_LVL_1 = 0x14, // shire herb
    LOOT_TYPE_LEGOLAS_HERB_LVL_2 = 0x15, // elven herb
    LOOT_TYPE_LEGOLAS_HERB_LVL_3 = 0x16, // mirkwood herb
    LOOT_TYPE_LEGOLAS_HERB_LVL_4 = 0x17, // rivendell herb
    LOOT_TYPE_LEGOLAS_HERB_LVL_5 = 0x18,
    LOOT_TYPE_ITEM_MIN = 0x19,   // item band starts here (== LOOT_TYPE_BOW_TIER_1)
    LOOT_TYPE_BOW_TIER_1 = 0x19, // bows, low tiers
    LOOT_TYPE_BOW_TIER_2 = 0x1a,
    LOOT_TYPE_ARROW = 0x1b,
    LOOT_TYPE_KNIFE_TIER_2 = 0x1c, // knives, high tiers (KnifeItems[8..17])
    LOOT_TYPE_SWORD_ONE_HAND = 0x1d,
    LOOT_TYPE_SWORD_TWO_HAND_TIER_1 = 0x1e,
    LOOT_TYPE_SWORD_TWO_HAND_TIER_2 = 0x1f,
    LOOT_TYPE_AXE_TIER_1 = 0x20,
    LOOT_TYPE_AXE_TIER_2 = 0x21,
    LOOT_TYPE_STAFF_TIER_1 = 0x22,
    LOOT_TYPE_STAFF_TIER_2 = 0x23,
    LOOT_TYPE_KNIFE_TIER_1 = 0x24, // knives, low tiers (KnifeItems[1..7])
    LOOT_TYPE_SPOON = 0x25,        // the ultra-rare Spoon (KnifeItems[0])
    LOOT_TYPE_ARMOR_TIER_1 = 0x26, // body armor, low tiers
    LOOT_TYPE_ARMOR_TIER_2 = 0x27,
    LOOT_TYPE_ARMOR_TIER_3 = 0x28,
    LOOT_TYPE_CLOAK_TIER_1 = 0x29,
    LOOT_TYPE_CLOAK_TIER_2 = 0x2a,
    LOOT_TYPE_CLOAK_TIER_3 = 0x2b,
    LOOT_TYPE_SHIELD_TIER_1 = 0x2c,
    LOOT_TYPE_SHIELD_TIER_2 = 0x2d,
    LOOT_TYPE_SHOE_TIER_1 = 0x2e,
    LOOT_TYPE_SHOE_TIER_2 = 0x2f,
    LOOT_TYPE_ACCESSORY_TRINKET = 0x30, // Keepsake/Clasp/Charm/Brooch
    LOOT_TYPE_ACCESSORY_AMULET = 0x31,  // Pendant/Necklace/Medallion/Talisman/Amulet/Firinga
    LOOT_TYPE_ACCESSORY_COLLAR = 0x32,  // Collar/Choker
    LOOT_TYPE_HELM_CROWN = 0x33,        // Crown
    LOOT_TYPE_HELM_HOOD = 0x34,         // Hood/Cap/Coif
    LOOT_TYPE_HELM_METAL = 0x35,        // Pothelm/Helm/Imperial Helm/Greathelm
    LOOT_TYPE_UNIQUE_MELEE = 0x36,      // repeatable-unique weapons (UniqueGfx[29..31])
    LOOT_TYPE_UNIQUE_BOW = 0x37,        // (UniqueGfx[35..37])
    LOOT_TYPE_UNIQUE_STAFF = 0x38,      // (UniqueGfx[32..34])
    LOOT_TYPE_UNIQUE_SWORD = 0x39,      // (UniqueGfx[26..28])
    LOOT_TYPE_GLOVE = 0x3a,
    LOOT_TYPE_CLUB = 0x3b, // club family (Club, Heavy Club, Maul)
    LOOT_TYPE_MACE = 0x3c, // mace family (Mace..Warhammer)
    // 0x3d..0x56: named-unique band, see the enum doc
    LOOT_TYPE_ITEM_MAX = 0x56, // item band ends here
    LOOT_TYPE_ENT_WATER = 0x57,
    LOOT_TYPE_RUNE = 0x58,
} LootType;
/** Drop actor states (Actor.behaviorState). */
enum LootDropState
{
    LOOT_DROP_IDLE = 0,
    LOOT_DROP_PICKED_UP = 1,   // looted (or despawn-expired): finalize and despawn
    LOOT_DROP_DESPAWNING = 3,  // despawn timer running; pickup still possible
    LOOT_DROP_FULL_MESSAGE = 6 // "backpack full" message showing (items only)
};

/**
 * Actor.unitType of loot containers (chests); at or above it there is no EnemyLootInfo
 * entry to index.
 */
#define LOOT_UNIT_TYPE_CONTAINER 0x53

/**
 * Shared 1..100 roll threshold: the drop-chance roll drops above it; the quality roll
 * yields a gem stack at or below it.
 */
#define LOOT_DROP_THRESHOLD 45

/** Pixel distance under which a player counts as standing on a drop (see actor_lootPile_tick) */
#define LOOT_PICKUP_DISTANCE 16

/** Ground-drop despawn timer in frames, passed to actor_lootPile_create. */
#define DROP_DESPAWN_TIMER 1800

/**
 * One weighted health-drop row; LootHealthDropChances @0x0806db94. A 1..100 roll under
 * maxRoll drops lootType; the last row is the fallthrough.
 */
typedef struct LootHealthDropChance
{
    u16 maxRoll;
    u16 lootType;
} LootHealthDropChance;
/**
 * The weighted health-drop table, headed by its row count (read as the roll loop bound,
 * count - 1); LootHealthDropChances @0x0806db94.
 */
typedef struct LootHealthDropTable
{
    u16 count;
    u16 field_0x2;
    LootHealthDropChance entries[5];
} LootHealthDropTable;
/**
 * One hero's per-itemType drop-roll thresholds; LootHeroItemTypeRolls @0x0806dbac (8 rows,
 * indexed by heroId). loot_generateItem rolls 1..100 and picks the first itemType whose
 * threshold is >= the roll. Columns are the ItemType values 0..0xf. Views: `thresholds` =
 * the indexed view used by the roll walk; `d` = named per-itemType access.
 */
typedef union LootItemTypeRolls {
    u8 thresholds[16];

    struct
    {
        u8 knife;
        u8 sword1H;
        u8 sword2H;
        u8 axe;
        u8 mace;
        u8 staff;
        u8 bow;
        u8 arrow;
        u8 armor;
        u8 shield;
        u8 accessory;
        u8 cloak;
        u8 gloves;
        u8 shoes;
        u8 unique;
        u8 empty; // never read (ITEM_TYPE_EMPTY never drops)
    } d;
} LootItemTypeRolls;
/**
 * Previous/next neighbor regions for prefix theming, indexed by the current region
 * (enum RegionId): mostly the hero's neighboring campaign regions, with exceptions;
 * path starts/ends self-link. LootHeroRegionProgressions @0x08287310 maps
 * heroId 0..7 to one of the three distinct tables (LootRegionProgressionsAragorn /
 * Eowyn / FrodoGandalf). loot_rollPrefix reads it on a 1..100 roll: <= 10 drops a prefix
 * themed for previous, > 95 one for next.
 */
typedef struct LootRegionProgression
{
    RegionId previous;
    RegionId next;
    u8 field_0x2[2];
} LootRegionProgression;
/**
 * Roll whether a slain actor drops loot, and if so generate the drop and spawn it on the
 * ground at the actor's position. In co-op (GAME_FLAG_COOP_ACTIVE) the killer's magic-find
 * stats are used; solo always uses player 0.
 *
 * @param actor the dying actor (its unitType indexes EnemyLootInfos)
 */
void loot_rollDrop(Actor *actor);

/**
 * Core drop generator: clears itemOut, makes the per-enemy quality roll, and dispatches to
 * a gem stack, a health drop, or a full equippable item.
 *
 * @param itemOut packed item handle to fill in
 * @param playerIndex whose magic-find stats apply
 * @param extraTreasurePercent the player's STAT_EXTRA_TREASURE_PERCENT
 * @param unitType Actor.unitType of the drop source (LOOT_UNIT_TYPE_CONTAINER = container)
 * @return loot type: ItemGfx.lootType for an item, LOOT_TYPE_GEM_STACK* gem stack,
 * LOOT_TYPE_FRUIT..LOOT_TYPE_LEMBAS_BREAD health drop
 */
u8 loot_generateDrop(Item *itemOut, u8 playerIndex, s16 extraTreasurePercent, u8 unitType);

/**
 * Pick the base item (item->d.baseIndex) for an item handle whose itemType is already set,
 * filtered to base-item tiers the current loot tier allows, and apply unlock-flag and
 * special gem-stack (itemType 0xe) handling.
 *
 * @return the picked base item's ItemGfx.lootType
 */
u32 loot_rollBaseItem(Item *item, u8 playerIndex);

void loot_rollPrefix(Item *item, s16 extraTreasurePercent, u8 heroId, u8 unitType);

/**
 * Roll whether the item gets a suffix, and which one. Base chance 2%/3%/4% on
 * Normal/Hard/Grueling, scaled by extraTreasurePercent; the suffix tier is uniform in
 * 0..lootTier-1, clamped down on lower difficulties. No-op for suffix-less item types and
 * ITEM_FLAG_NO_SUFFIX base items.
 */
void loot_rollSuffix(Item *item, s16 extraTreasurePercent);

/**
 * The plain drop-chance roll: 1..100 plus the enemy's EnemyLootInfo.dropChanceBonus.
 *
 * @return TRUE when roll > LOOT_DROP_THRESHOLD (a drop happens)
 */
bool loot_rollDropChance(u8 unitType);

/**
 * Generate a gem stack into itemOut: value 1..13 + 5 x loot tier, scaled by the player's
 * gem magic-find, capped at 755, packed into the handle bytes.
 *
 * @return LOOT_TYPE_GEM_STACK_MIN for a minimal (value 1) stack, LOOT_TYPE_GEM_STACK otherwise
 */
u8 loot_rollGemStack(Item *itemOut, u8 playerIndex);

/**
 * Roll which health drop a drop becomes, from the weighted LootHealthDropChances table.
 *
 * @return loot type LOOT_TYPE_FRUIT..LOOT_TYPE_LEMBAS_BREAD (0xf..0x13)
 */
u8 loot_rollHealthDrop(void);

/**
 * Generate a full equippable item: pick the itemType from the killing hero's per-hero
 * thresholds, then roll base item, prefix, and suffix.
 *
 * @return the base item's ItemGfx.lootType
 */
u8 loot_generateItem(Item *itemOut, u8 playerIndex, s16 extraTreasurePercent, u8 unitType);

/**
 * The gem magic-find multiplier applied to a gem stack's base value:
 * base x (STAT_EXTRA_GEMS_PERCENT + STAT_GEMFINDER x 10) / 100.
 */
s32 loot_scaleGemDrop(s32 baseValue, s32 playerIndex);

/** Pack a gem stack's gold value into bytes 1..3 of its handle, 0xff per byte. */
void loot_setGemStackValue(GemStack *stackOut, s32 value);
