#pragma once

#include "gfx.h"
#include "spriteAnimation.h"
#include "types.h"
#include "unique_item_ids.h"

/** Item type nibble (packed item bits 0-3; index into ItemTypeInfos @0x08068f50). */
typedef enum ItemType
{
    ITEM_TYPE_KNIFE = 0x0,
    ITEM_TYPE_SWORD_1H = 0x1,
    ITEM_TYPE_SWORD_2H = 0x2,
    ITEM_TYPE_AXE = 0x3,
    ITEM_TYPE_MACE = 0x4,
    ITEM_TYPE_STAFF = 0x5,
    ITEM_TYPE_BOW = 0x6,
    ITEM_TYPE_ARROW = 0x7,
    ITEM_TYPE_ARMOR = 0x8,
    ITEM_TYPE_SHIELD = 0x9,
    ITEM_TYPE_ACCESSORY = 0xa,
    ITEM_TYPE_CLOAK = 0xb,
    ITEM_TYPE_GLOVES = 0xc,
    ITEM_TYPE_SHOES = 0xd,
    ITEM_TYPE_UNIQUE = 0xe,
    ITEM_TYPE_EMPTY = 0xf, // empty slot (all handle bytes 0xff)
} ItemType;
/**
 * Runes tracked in Player.runes: RuneId 0..RUNE_COUNT-1 (the 15 collectible
 * runes, RUNE_DWARF_1_DWARFMETAL..RUNE_MORGUL_3_BLACKNESS; RUNE_NONE = uncarved). The
 * count bounds the rune drop roll and the mask's bit count.
 */
#define RUNE_COUNT 15

/**
 * Carved-rune index (Item.d.runeIndex; indexes Runes). A rune is stamped into an item by
 * item_carveRune; its stats apply while the item is equipped. The name/description texts are
 * TEXT_ID_DWARF_RUNE_I_DWARFMETAL + runeIndex (descriptions + 0x11 further).
 */
typedef enum RuneId
{
    RUNE_DWARF_1_DWARFMETAL = 0x0,     // Melee Armor +1
    RUNE_DWARF_2_SHARPNESS = 0x1,      // Damage +1
    RUNE_DWARF_3_THE_FORGE = 0x2,      // Crit Damage +5
    RUNE_DWARF_4_MOUNTAIN_HEART = 0x3, // HP per Kill +5
    RUNE_ELF_1_ORC_FOE = 0x4,          // dmg to Orcs +2
    RUNE_ELF_2_NIMBLEFOOT = 0x5,       // % Dodge +10
    RUNE_ELF_3_SACRED_HEART = 0x6,     // Spirit Regen +2
    RUNE_ELF_4_MEDITATIVE = 0x7,       // Meditative +1
    RUNE_MAN_1_WHITE_CITY = 0x8,       // Missile Armor +2
    RUNE_MAN_2_NATURE = 0x9,           // % Extra HP from Food +100, HP from Herbs +20
    RUNE_MAN_3_MIGHT_OF_MAN = 0xa,     // Damage +1, Max HP +15
    RUNE_MAN_4_RUNE_OF_FIRE = 0xb,     // Fire Protection +3, Fireburst +1
    RUNE_MORGUL_1_ORC_DOOM_KING = 0xc, // dmg to Orcs +4, % Corruption Resistance -50
    RUNE_MORGUL_2_DIRE_FEAR = 0xd,     // Push Foe Back +30, % Corruption Resistance -60
    RUNE_MORGUL_3_BLACKNESS = 0xe,     // All Primary Stats +5, % Corruption Resistance -70
    RUNE_NONE = 0xf,                   // no rune carved
} RuneId;
/**
 * Collected-rune word (Player.runes; saved as SaveSlot.d.runes): bit = enum RuneId
 * (bit 0xf unused; RUNE_NONE is an item's uncarved slot, not a collectible).
 */
typedef union {
    u16 p;

    struct
    {
        u16 dwarf1Dwarfmetal : 1;    // RUNE_DWARF_1_DWARFMETAL
        u16 dwarf2Sharpness : 1;     // RUNE_DWARF_2_SHARPNESS
        u16 dwarf3TheForge : 1;      // RUNE_DWARF_3_THE_FORGE
        u16 dwarf4MountainHeart : 1; // RUNE_DWARF_4_MOUNTAIN_HEART
        u16 elf1OrcFoe : 1;          // RUNE_ELF_1_ORC_FOE
        u16 elf2Nimblefoot : 1;      // RUNE_ELF_2_NIMBLEFOOT
        u16 elf3SacredHeart : 1;     // RUNE_ELF_3_SACRED_HEART
        u16 elf4Meditative : 1;      // RUNE_ELF_4_MEDITATIVE
        u16 man1WhiteCity : 1;       // RUNE_MAN_1_WHITE_CITY
        u16 man2Nature : 1;          // RUNE_MAN_2_NATURE
        u16 man3MightOfMan : 1;      // RUNE_MAN_3_MIGHT_OF_MAN
        u16 man4RuneOfFire : 1;      // RUNE_MAN_4_RUNE_OF_FIRE
        u16 morgul1OrcDoomKing : 1;  // RUNE_MORGUL_1_ORC_DOOM_KING
        u16 morgul2DireFear : 1;     // RUNE_MORGUL_2_DIRE_FEAR
        u16 morgul3Blackness : 1;    // RUNE_MORGUL_3_BLACKNESS
        u16 field_bit_15 : 1;        // 1 << 15
    } __attribute__((packed)) d;
} __attribute__((packed)) RuneFlags;

/**
 * Gem-stack payload view of an item handle: byte 0 stays 0xff from item_clearSlot (low nibble
 * reads ITEM_TYPE_EMPTY), and bytes 1..3 carry the stack's gold value as three chunks filled
 * in order, each saturating at 0xff (gold value = value0 + value1 + value2, capped at 755 so
 * the word never aliases EMPTY_ITEM). A drop payload only, never an equippable item; the
 * loot_generateDrop loot type byte (LOOT_TYPE_GEM_STACK_MIN / LOOT_TYPE_GEM_STACK) is what marks
 * it.
 */
typedef struct GemStack
{
    u8 itemTypeMaterial; // 0xff; written by item_clearSlot, never by the gem code
    u8 value0;           // min(gold value, 0xff); 1 only for a minimal (value 1) stack
    u8 value1;           // min(remaining gold value, 0xff)
    u8 value2;           // remaining gold value
} GemStack;
/**
 * An item instance / item handle: the packed 4-byte item word (itemType nibble | runeIndex<<4 |
 * baseIndex<<8 | prefixIndex<<16 | suffixIndex<<24; 0xff bytes = absent affix). Stored in a
 * Player's Inventory slots and passed around in a register as a u32 handle. The runeIndex nibble
 * is the carved rune (enum RuneId; RUNE_NONE when never carved, e.g. fresh loot).
 *
 * Views: `word` = the packed u32; `halfwords` = the low/affixes u16 pair; `d` = unpacked
 * per-field access; `gem` = the gem-stack payload a drop carries in the same word (see
 * GemStack).
 */
typedef union Item {
    u32 word;

    // byte view (combat_resolvePlayerAttackOnActor's nibble tests read the type byte)
    u8 bytes[4];

    struct
    {
        u16 low;
        u16 affixes; // prefixIndex | suffixIndex<<8; 0xffff = neither affix
    } halfwords;

    struct
    {
        u8 itemType : 4;
        u8 runeIndex : 4; // carved rune (enum RuneId); RUNE_NONE = uncarved
        u8 baseIndex;     // index into the item type's base-item table
        u8 prefixIndex;
        u8 suffixIndex;
    } d;

    struct GemStack gem;
} Item;
/** The empty item slot value: all four bytes 0xff (low nibble ITEM_TYPE_EMPTY). */
#define EMPTY_ITEM                                                                                 \
    ((Item){.d = {.itemType = ITEM_TYPE_EMPTY,                                                     \
                  .runeIndex = RUNE_NONE,                                                          \
                  .baseIndex = 0xff,                                                               \
                  .prefixIndex = 0xff,                                                             \
                  .suffixIndex = 0xff}})

/**
 * Whether the handle carries a gem-stack payload (see GemStack): byte 0 is 0xff (the
 * ITEM_TYPE_EMPTY nibble a gem stack is tagged with) but the word is not the empty slot.
 */
#define ITEM_IS_GEM_STACK(item) (((item).word & 0xff) == 0xff && (item).word != 0xffffffff)

/** Sentinel value stored in Item.d.prefixIndex / suffixIndex when that affix is absent. */
#define ITEM_AFFIX_NONE 0xff
/** Item.halfwords.affixes value when neither prefix nor suffix is present (both bytes 0xff). */
#define ITEM_AFFIXES_NONE 0xffff

/** The item's type nibble; equivalent to item.d.itemType (mask form). */
#define ITEM_TYPE(item) ((item).bytes[0] & 0xf)

/**
 * The item's type nibble, shift form; byte-matching requires it over ITEM_TYPE's mask
 * form at some call sites.
 */
#define ITEM_TYPE_VIA_SHIFT(item) ((u8)((item).bytes[0] << 4) >> 4)

// Whether the item carries the given affix (index != ITEM_AFFIX_NONE).
#define ITEM_HAS_PREFIX(item) ((item).d.prefixIndex != ITEM_AFFIX_NONE)
#define ITEM_HAS_SUFFIX(item) ((item).d.suffixIndex != ITEM_AFFIX_NONE)

/** Whether the item has a rune carved into it (runeIndex != RUNE_NONE). */
#define ITEM_HAS_RUNE(item) ((item).d.runeIndex != RUNE_NONE)

/**
 * One carved rune's effect: up to 2 (stat index, value) modifier pairs. The Runes table is
 * indexed by an item's runeIndex.
 */
typedef struct RuneStats
{
    u8 stat0;
    s8 val0;

    u8 stat1; // STAT_NONE when the rune has a single stat
    s8 val1;
} RuneStats;
/**
 * One prefix/suffix affix: nameId + up to 4 (stat index, value) modifier pairs. Affix tables are
 * arrays of these (carved/data/affix_tables/).
 */
typedef struct ItemAffix
{
    u16 nameId;
    // item_getMinLevel adds a prefix's tier halved (>> 1), a suffix's tier whole
    u8 tier;

    u8 stat0;
    s8 val0;

    u8 stat1;
    s8 val1;

    u8 stat2;
    s8 val2;

    u8 stat3;
    s8 val3;
} ItemAffix;
/**
 * One base item (the weapon/armor/etc. itself, before affixes): nameId + gold-value tier +
 * up to 4 (stat index, value) modifier pairs. Same binary layout as ItemAffix. The per-type
 * base-item tables (carved/data/item_tables/) hang off ItemTypeInfos[].baseItems, indexed by an
 * equipped item's baseIndex.
 */
typedef struct ItemBase
{
    u16 nameId;
    u8 tier;

    u8 stat0;
    s8 val0;

    u8 stat1;
    s8 val1;

    u8 stat2;
    s8 val2;

    u8 stat3;
    s8 val3;
} ItemBase;
/** Per-affix flag bits (ItemTypeInfo.prefixFlags / suffixFlags); one u32 per affix. */
enum AffixFlag
{
    AFFIX_FLAG_MIN_LEVEL_PLUS_5_A = 1 << 0, // +5 min equip level
    AFFIX_FLAG_MIN_LEVEL_PLUS_5_B = 1 << 1, // +5 min equip level
    AFFIX_FLAG_14 = 1 << 14,
    AFFIX_FLAG_15 = 1 << 15,
    AFFIX_FLAG_16 = 1 << 16,
    AFFIX_FLAG_17 = 1 << 17,
    AFFIX_FLAG_18 = 1 << 18,
    AFFIX_FLAG_19 = 1 << 19,
    AFFIX_FLAG_20 = 1 << 20,
    AFFIX_FLAG_21 = 1 << 21,
    AFFIX_FLAG_22 = 1 << 22,
    AFFIX_FLAG_23 = 1 << 23,
    AFFIX_FLAG_24 = 1 << 24,
    AFFIX_FLAG_25 = 1 << 25,
    AFFIX_FLAG_26 = 1 << 26,
    AFFIX_FLAG_27 = 1 << 27,
    AFFIX_FLAG_28 = 1 << 28,
    AFFIX_FLAG_29 = 1 << 29,
    AFFIX_FLAG_30 = 1 << 30,
    AFFIX_FLAG_31 = 1u << 31, // 1u: plain 1 << 31 overflows signed int
};

/**
 * Named bit view of a prefixFlags/suffixFlags table entry; code and tables use the AFFIX_FLAG_*
 * mask view (a bitfield read lowers differently than the ROM's mask tests).
 */
typedef union AffixFlags {
    u32 p;

    struct
    {
        u32 minLevelPlus5A : 1; // 1 << 0: +5 min equip level
        u32 minLevelPlus5B : 1; // 1 << 1: +5 min equip level
        u32 field_bit_2 : 1;    // 1 << 2
        u32 field_bit_3 : 1;    // 1 << 3
        u32 field_bit_4 : 1;    // 1 << 4
        u32 field_bit_5 : 1;    // 1 << 5
        u32 field_bit_6 : 1;    // 1 << 6
        u32 field_bit_7 : 1;    // 1 << 7
        u32 field_bit_8 : 1;    // 1 << 8
        u32 field_bit_9 : 1;    // 1 << 9
        u32 field_bit_10 : 1;   // 1 << 10
        u32 field_bit_11 : 1;   // 1 << 11
        u32 field_bit_12 : 1;   // 1 << 12
        u32 field_bit_13 : 1;   // 1 << 13
        u32 field_bit_14 : 1;   // 1 << 14
        u32 field_bit_15 : 1;   // 1 << 15
        u32 field_bit_16 : 1;   // 1 << 16
        u32 field_bit_17 : 1;   // 1 << 17
        u32 field_bit_18 : 1;   // 1 << 18
        u32 field_bit_19 : 1;   // 1 << 19
        u32 field_bit_20 : 1;   // 1 << 20
        u32 field_bit_21 : 1;   // 1 << 21
        u32 field_bit_22 : 1;   // 1 << 22
        u32 field_bit_23 : 1;   // 1 << 23
        u32 field_bit_24 : 1;   // 1 << 24
        u32 field_bit_25 : 1;   // 1 << 25
        u32 field_bit_26 : 1;   // 1 << 26
        u32 field_bit_27 : 1;   // 1 << 27
        u32 field_bit_28 : 1;   // 1 << 28
        u32 field_bit_29 : 1;   // 1 << 29
        u32 field_bit_30 : 1;   // 1 << 30
        u32 field_bit_31 : 1;   // 1 << 31
    } d;
} AffixFlags;
/** union ItemFlags bits (the mask view the code tests). */
enum ItemFlag
{
    // weapon family the base item counts as (bits 0-5)
    ITEM_FAMILY_SWORD = 1 << 0, // knives count as swords
    ITEM_FAMILY_AXE = 1 << 1,
    ITEM_FAMILY_MACE = 1 << 2,
    ITEM_FAMILY_STAFF = 1 << 3,
    ITEM_FAMILY_BOW = 1 << 4,
    ITEM_FAMILY_SHIELD = 1 << 5,

    // ancient item: drops only once unlockFlags.ancientItems is set;
    // distinct HUD name color
    ITEM_FLAG_ANCIENT = 1 << 6,

    // each of these adds +5 to the item's minimum equip level
    ITEM_FLAG_MIN_LEVEL_PLUS_5_A = 1 << 7,
    ITEM_FLAG_MIN_LEVEL_PLUS_5_B = 1 << 8,

    // cannot roll a prefix; set on all unique items and arrows
    ITEM_FLAG_NO_PREFIX = 1 << 13,
    // cannot roll a suffix; set on all unique items and arrows
    ITEM_FLAG_NO_SUFFIX = 1 << 14,
    // set on unique weapons only; pickup records the item in Player.uniqueWeaponsCollected
    // (carried-passive uniques use uniquePassivesCollected)
    ITEM_FLAG_UNIQUE_WEAPON = 1 << 15,

    // not equippable; the base item's stats apply passively while carried in the backpack
    ITEM_FLAG_CARRIED_PASSIVE = 1 << 16,
    // weapon fills both hands; equipping it clears the offhand first
    ITEM_FLAG_TWO_HANDED = 1 << 17,

    // allowed equip slots, bit (18 + InventorySlot). ITEM_FLAG_SLOT_WEAPON | ITEM_FLAG_SLOT_OFFHAND
    // marks a one-hand weapon eligible for dual-wielding; item_getEquipSlot resolves the actual
    // slot.
    ITEM_FLAG_SLOT_ARMOR = 1 << 18,
    ITEM_FLAG_SLOT_WEAPON = 1 << 19,
    ITEM_FLAG_SLOT_OFFHAND = 1 << 20,
    ITEM_FLAG_SLOT_HELMET = 1 << 21,
    ITEM_FLAG_SLOT_CLOAK = 1 << 22,
    ITEM_FLAG_SLOT_GLOVES = 1 << 23,
    ITEM_FLAG_SLOT_BOOTS = 1 << 24,
    ITEM_FLAG_SLOT_JEWELRY = 1 << 25,

    // a one-hand weapon eligible for dual-wielding allows both hand slots
    ITEM_FLAG_DUAL_WIELD_MASK = ITEM_FLAG_SLOT_WEAPON | ITEM_FLAG_SLOT_OFFHAND,
};

/**
 * per-hero equip permission, bit (26 + heroId); Sam shares Frodo's bit and Smeagol equips
 * nothing. The constant-base shift wraps heroes past bit 31 to 0,
 * which the table data relies on.
 */
#define ITEM_FLAG_CLASS_BIT(heroId) ((u32)0x04000000 << (heroId))
#define ITEM_FLAG_CLASS_ALL 0xfc000000

/**
 * Per-base-item equip/permission flags (ItemMetadata.flags); matched code tests the ITEM_FLAG_*
 * masks on the word view instead (a bitfield read lowers differently than the ROM's mask tests).
 */
typedef union ItemFlags {
    u32 p;

    struct
    {
        u32 familySword : 1;    // 1 << 0: knives count as swords
        u32 familyAxe : 1;      // 1 << 1
        u32 familyMace : 1;     // 1 << 2
        u32 familyStaff : 1;    // 1 << 3
        u32 familyBow : 1;      // 1 << 4
        u32 familyShield : 1;   // 1 << 5
        u32 ancient : 1;        // 1 << 6
        u32 minLevelPlus5A : 1; // 1 << 7
        u32 minLevelPlus5B : 1; // 1 << 8
        u32 field_bit_9 : 1;    // 1 << 9
        u32 field_bit_10 : 1;   // 1 << 10
        u32 field_bit_11 : 1;   // 1 << 11
        u32 field_bit_12 : 1;   // 1 << 12
        u32 noPrefix : 1;       // 1 << 13
        u32 noSuffix : 1;       // 1 << 14
        u32 uniqueWeapon : 1;   // 1 << 15
        u32 carriedPassive : 1; // 1 << 16
        u32 twoHanded : 1;      // 1 << 17
        u32 slotArmor : 1;      // 1 << 18
        u32 slotWeapon : 1;     // 1 << 19
        u32 slotOffhand : 1;    // 1 << 20
        u32 slotHelmet : 1;     // 1 << 21
        u32 slotCloak : 1;      // 1 << 22
        u32 slotGloves : 1;     // 1 << 23
        u32 slotBoots : 1;      // 1 << 24
        u32 slotJewelry : 1;    // 1 << 25
        u32 classFrodo : 1;     // 1 << 26
        u32 classLegolas : 1;   // 1 << 27
        u32 classAragorn : 1;   // 1 << 28
        u32 classGandalf : 1;   // 1 << 29
        u32 classEowyn : 1;     // 1 << 30
        u32 classGimli : 1;     // 1 << 31
    } d;
} ItemFlags;
/**
 * Per-base-item metadata: equip/permission flags + equip sfx. One table per item type (the
 * *ItemFlags arrays), hung off ItemTypeInfos[].baseMetadata and indexed by an equipped item's
 * baseIndex.
 */
typedef struct ItemMetadata
{
    ItemFlags flags; // ITEM_FLAG_* / ITEM_FAMILY_* mask view (what the code tests)
    u32 equipSfx;    // sfx_play id played when the item is equipped
} ItemMetadata;
/**
 * Per-base-item gfx/loot record, hung off ItemTypeInfos[].baseGfx and indexed by
 * an equipped item's baseIndex.
 */
typedef struct ItemGfx
{
    // OBJ icon shown in the backpack cells; passed to sprite_setAnimation
    SpriteAnimation animation;
    // BG icon shown in the backpack's equipment grid; NULL on an unequippable item
    const void *equipIcon;
    // BG icon shown in the Ranger Hollow's stash cells
    const void *hollowIcon;

    // the loot type loot_rollBaseItem returns
    u8 lootType;
} ItemGfx;

/**
 * One ItemGfx row from the base item's icon symbol prefix: its backpack-icon sprite
 * (name##BackpackIcon{Tiles,Frames,Palette}) plus the equip/hollow icons named here.
 * NULL as equipIcon_ marks an item that is never equipped.
 */
#define ITEM_GFX_ICONS(name, equipIcon_, hollowIcon_, lootType_)                                   \
    {.animation = SPRITE_STILL_ANIMATION(                                                          \
         name##BackpackIconTiles, &name##BackpackIconFrames.header, name##BackpackIconPalette),    \
     .equipIcon = equipIcon_,                                                                      \
     .hollowIcon = hollowIcon_,                                                                    \
     .lootType = lootType_}
/** ITEM_GFX_ICONS where all three icons carry the base item's own name. */
#define ITEM_GFX(name, lootType_) ITEM_GFX_ICONS(name, name##EquipIcon, name##HollowIcon, lootType_)
/** ITEM_GFX for a carried-passive item: never equipped, so it has no equip icon. */
#define ITEM_GFX_CARRIED(name, lootType_) ITEM_GFX_ICONS(name, NULL, name##HollowIcon, lootType_)

// Frame set every backpack icon shares: one 32x24 frame drawn as a 32x8 OBJ over a 32x16 OBJ.
#define ITEM_BACKPACK_ICON_FRAME_SET_INIT(boxCount)                                                \
    .header = {.width = 32,                                                                        \
               .height = 24,                                                                       \
               .frameCount = 1,                                                                    \
               .field_0x8 = 0x180,                                                                 \
               .triggerBoxCount = boxCount},                                                       \
    .frameOffsets = {2},                                                                           \
    .frame = {.flags.d = {.oamCount = 2, .lz77 = 1},                                               \
              .width = 32,                                                                         \
              .height = 24,                                                                        \
              .tileBytes = 0x180},                                                                 \
    .oam = {{.d = {.objSize = 1, .objShape = 1}},                                                  \
            {.d = {.y = 8, .objSize = 2, .objShape = 1, .tileOffset = 4}}}
/**
 * Define a backpack icon's ROM objects (name##Tiles, name##Frames, name##Palette) from the
 * name##_TILES / name##_PALETTE byte lists rotkit build gfx generates from its PNG.
 */
#define ITEM_BACKPACK_ICON(name)                                                                   \
    const u8 name##Tiles[] = {name##_TILES};                                                       \
    const SpriteIconFrameSet name##Frames = {ITEM_BACKPACK_ICON_FRAME_SET_INIT(0)};                \
    const u16 name##Palette[16] = {name##_PALETTE}
/** ITEM_BACKPACK_ICON with one trigger box slot in the frame set. */
#define ITEM_BACKPACK_ICON_BOXED(name, xMin_, xMax_, yMin_, yMax_, enable_)                        \
    const u8 name##Tiles[] = {name##_TILES};                                                       \
    const SpriteIconBoxedFrameSet name##Frames = {                                                 \
        ITEM_BACKPACK_ICON_FRAME_SET_INIT(1),                                                      \
        .triggerBoxes = {                                                                          \
            {.xMin = xMin_, .xMax = xMax_, .yMin = yMin_, .yMax = yMax_, .enable = enable_}}};     \
    const u16 name##Palette[16] = {name##_PALETTE}
/** Define an equip/hollow icon: a 16-color BG asset of tilesWide x tilesHigh tiles. */
#define ITEM_BG_ICON(name, tilesWide, tilesHigh)                                                   \
    BG_ASSET_MAPPED(name,                                                                          \
                    BG_ASSET_PALETTE_16 | BG_ASSET_MAP | BG_ASSET_TILES | BG_ASSET_CODEC_PUCRUNCH, \
                    16, tilesWide, tilesHigh)
/**
 * An index range into an affix table. The per-type range tables hang off
 * ItemTypeInfos[].prefixRegionRanges (28 rows, one per region) and .suffixTierRanges (10 rows,
 * indexed by a rolled 0..lootTier-1 tier); the loot roll picks a uniform random affix
 * index in [loIndex, hiIndex].
 */
typedef struct AffixIndexRange
{
    u8 loIndex;    // first affix table index of the range
    u8 hiIndex;    // last affix table index of the range
    u8 padding[2]; // padding, unused
} AffixIndexRange;
/**
 * Contains pointers to item-related tables for a given item type (e.g. knife, axe, shoe, ...),
 * suffix and affix flags, and other metadata.
 */
typedef struct ItemTypeInfo
{
    /**
     * this item type's base-item table (one of the *Items arrays), indexed by an
     * equipped item's baseIndex
     */
    const ItemBase *baseItems;

    /**
     * this item type's prefix region ranges (28 rows, one per region),
     * NULL when the type has no prefixes
     */
    const AffixIndexRange *prefixRegionRanges;

    /**
     * this item type's prefix affix table (one of the *Prefixes arrays), indexed by an
     * equipped item's prefixIndex
     */
    const ItemAffix *prefixes;

    /** per-prefix-flag table, indexed by prefixIndex; bit 0/1 = +5 min equip level */
    const AffixFlags *prefixFlags;

    /**
     * this item type's suffix tier ranges (10 rows indexed by a rolled suffix tier),
     * NULL when the type has no suffixes
     */
    const AffixIndexRange *suffixTierRanges;

    /** this item type's suffix affix table, indexed by an equipped item's suffixIndex */
    const ItemAffix *suffixes;

    /** per-suffix-flag table, indexed by suffixIndex; bit 0/1 = +5 min equip level */
    const AffixFlags *suffixFlags;

    /** per-base-item gfx/loot table (one of the *Gfx arrays), indexed by baseIndex */
    const ItemGfx *baseGfx;

    /**
     * this item type's per-base-item metadata (one of the *ItemFlags arrays), indexed by
     * an equipped item's baseIndex
     */
    const ItemMetadata *baseMetadata;

    // row count of .baseItems
    u8 baseItemCount;
} ItemTypeInfo;
#define ITEM_TYPE_INFO(item) (ItemTypeInfos[(item).d.itemType])
#define ITEM_BASE_METADATA(item) ((ITEM_TYPE_INFO(item)).baseMetadata[(item).d.baseIndex])

#define ITEM_PREFIX(item) ((ITEM_TYPE_INFO(item)).prefixes[(item).d.prefixIndex])
#define ITEM_SUFFIX(item) ((ITEM_TYPE_INFO(item)).suffixes[(item).d.suffixIndex])
#define ITEM_BASE(item) ((ITEM_TYPE_INFO(item)).baseItems[(item).d.baseIndex])
#define ITEM_RUNE(item) (Runes[(item).d.runeIndex])

bool item_deleteFromInventory(u8 slot, u8 playerIndex);

/** Clear an item handle slot to the empty value (all four bytes 0xff). */
void item_clearSlot(u32 *slot);

/** Nonzero if the player meets the item's class-restriction bits and minimum level. */
u32 item_isUsableByPlayer(u32 itemHandle, u8 playerIndex);

/** The item's minimum equip level (low byte of the return). */
u32 item_getMinLevel(u32 itemHandle);

/**
 * Draw an item's name, min level, and stat/flavor lines into an info panel at x/y.
 * ITEM_TYPE_UNIQUE items with baseIndex < 26 show flavor text
 * (TEXT_ID_UNIQUE_DESC_BASE + baseIndex) instead of the accumulated stat string.
 *
 * @return the description line (unique flavor text or the accumulated stat string)
 */
char *item_drawInfo(char *buffer, u8 playerIndex, u32 itemHandle, u16 *x, u16 *y, u16 *pen);

/**
 * Apply (remove == 0) or remove (remove != 0) an item's base/rune/prefix/suffix stat modifiers
 * for the given inventory slot. Carried-passive items (ITEM_FLAG_CARRIED_PASSIVE) apply only
 * their base stats, and only when the player meets the class/level requirements.
 */
void item_applyAffixStats(Item item, u8 inventorySlot, u8 playerIndex, s8 remove);

/**
 * Apply (sign > 0) or un-apply (sign < 0) one carved rune's stats (Runes[runeIndex]) for the
 * given inventory slot. Called when a rune-bearing item is equipped/unequipped/re-carved; rune
 * stats count only while the item is equipped.
 */
void item_applyRuneStats(u8 inventorySlot, u8 playerIndex, u8 runeIndex, s8 sign);

/**
 * Carve a rune into the item in one of a Player's inventory slots, replacing any rune already
 * carved into it: un-applies the old rune's stats, stamps runeIndex into the item's rune nibble,
 * and applies the new rune's stats. Only equipment-slot items apply their rune.
 */
void item_carveRune(u8 slot, u8 runeIndex, u8 playerIndex);

/**
 * The InventorySlot the item would occupy for the player, or >= INVENTORY_SLOT_BACKPACK_0 when the
 * player cannot equip it (class mask, min level, or ITEM_FLAG_CARRIED_PASSIVE).
 */
u32 item_getEquipSlot(u32 itemHandle, u8 playerIndex);

/**
 * Equip the item in a backpack slot into its item_getEquipSlot slot, swapping out and
 * un-applying whatever was there. Handles two-handed weapons clearing the offhand (and
 * vice versa) and plays the item's equip sfx.
 *
 * @return nonzero on success, 0 when the swap is impossible (e.g. backpack full)
 */
u32 item_equipFromBackpack(u8 backpackSlot, u8 playerIndex);

/**
 * Move the item in an equipment slot to the first free backpack slot. A one-hand weapon in
 * the offhand is promoted into the weapon slot when the weapon slot is unequipped.
 *
 * @return 0 when the backpack is full
 */
u32 item_unequipToBackpack(u8 slot, u8 playerIndex);

/** Swap the contents of two inventory slots (equipment or backpack). */
u32 item_swapSlots(u8 slotA, u8 slotB, u8 playerIndex);

char *item_affix_formatName(char *dst, Item item, u16 maxWidth);
char *item_affix_accumulateStats(char *dst, Item item);
