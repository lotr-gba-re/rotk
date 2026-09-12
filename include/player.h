#pragma once

#include "actor.h"
#include "fx.h"
#include "item.h"
#include "passiveSkill.h"
#include "stats.h"
#include "types.h"
#include "vector2.h"

/** Player.combatFlags bits with known consumers. */
enum PlayerCombatFlag
{
    PLAYER_COMBAT_FLAG_TWO_HANDED = 1 << 0,
    PLAYER_COMBAT_FLAG_1 = 1 << 1,
    PLAYER_COMBAT_FLAG_2 = 1 << 2,

    // recompute Actor.moveSpeed from STAT_SPEED_PERCENT on the next player tick; set at spawn
    // and by STAT_SPEED_PERCENT changes
    PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY = 1 << 6,
    PLAYER_COMBAT_FLAG_DAMAGE_TYPE = 1 << 8, // 1 = slash, 0 = impale

    // re-attach the hand item sprites and glow FX on the next player tick (equipment changed)
    PLAYER_COMBAT_FLAG_HAND_SPRITES_DIRTY = 1 << 11,

    // forces every melee hit to critical (provisional; source/meaning unknown)
    PLAYER_COMBAT_FLAG_FORCE_CRIT_UNKNOWN = 1 << 9,

    // Warrior's Pool well-buff: while set, combat_resolvePveAttack / combat_resolvePvpAttack
    // force every melee hit to critical. Set for 600 frames by the Warrior's Pool well.
    PLAYER_COMBAT_FLAG_WARRIORS_POOL = 1 << 12,

    // set by the instakill proc in combat_resolvePveAttack; the next player_awardXp
    // doubles this player's share and clears it
    // TODO: This is probably for Aragorn's 'Rage of the North' passive skill. Verify.
    PLAYER_COMBAT_FLAG_XP_DOUBLE = 1 << 13,
    PLAYER_COMBAT_FLAG_TRIPLE_DAMAGE = 1 << 14,

    // the STAT_MEDITATIVE heal SFX has played this idle stretch; cleared on leaving idle
    PLAYER_COMBAT_FLAG_MEDITATIVE_SFX = 1 << 15,
};

/**
 * Player.combatFlags; matched code tests the PLAYER_COMBAT_FLAG_* masks on the word view
 * instead. Packed:
 * agbcc would otherwise pad the union to 4-byte size/alignment and shift the Player layout.
 */
typedef union PlayerCombatFlags {
    u16 p;

    struct
    {
        u16 twoHanded : 1;        // 1 << 0
        u16 field_bit_1 : 1;      // 1 << 1
        u16 field_bit_2 : 1;      // 1 << 2
        u16 field_bit_3 : 1;      // 1 << 3
        u16 field_bit_4 : 1;      // 1 << 4
        u16 field_bit_5 : 1;      // 1 << 5
        u16 moveSpeedDirty : 1;   // 1 << 6: recompute Actor.moveSpeed
        u16 field_bit_7 : 1;      // 1 << 7
        u16 damageType : 1;       // 1 << 8: 1 = slash, 0 = impale
        u16 forceCritUnknown : 1; // 1 << 9: forces a melee hit to crit
        u16 field_bit_10 : 1;     // 1 << 10
        u16 handSpritesDirty : 1; // 1 << 11: re-attach hand item sprites and glow FX
        u16 warriorsPool : 1;     // 1 << 12: Warrior's Pool well-buff active (forces melee crit)
        u16 xpDouble : 1;         // 1 << 13: next player_awardXp share is doubled
        u16 tripleDamage : 1;     // 1 << 14
        u16 meditativeSfx : 1;    // 1 << 15: meditative heal SFX played this idle stretch
    } __attribute__((packed)) d;
} __attribute__((packed)) PlayerCombatFlags;
typedef struct
{
    u8 fresh : 4;
    u8 dried : 4;
} __attribute__((packed)) KingsfoilHerbs;

/** Player.statusFlags bits with known consumers. */
enum PlayerStatusFlag
{
    // Ent Water owned (Hollow purchase or loot pickup); the death handler revives the
    // player at full HP instead of applying the 10% XP/gem death penalty. Never cleared.
    PLAYER_STATUS_FLAG_ENT_WATER = 1 << 0,

    // unspent attribute/skill points: blinks the HUD portrait frame; set by
    // player_awardXp point awards, cleared on entering the character screen
    PLAYER_STATUS_FLAG_UNSPENT_POINTS = 1 << 1,

    // set at character creation for Sam; the save/load confirm blocks story mode when set
    PLAYER_STATUS_FLAG_SAM = 1 << 2,

    // Rune Forge slot A/B purchased; cleared on entering a different mission
    PLAYER_STATUS_FLAG_RUNE_FORGE_A_USED = 1 << 4,
    PLAYER_STATUS_FLAG_RUNE_FORGE_B_USED = 1 << 5,
};

/**
 * Player.statusFlags; matched code tests the PLAYER_STATUS_FLAG_* masks on the packed view
 * instead
 */
typedef union {
    u8 p;

    struct
    {
        u8 entWater : 1;       // 1 << 0
        u8 unspentPoints : 1;  // 1 << 1
        u8 sam : 1;            // 1 << 2
        u8 field_bit_3 : 1;    // 1 << 3
        u8 runeForgeAUsed : 1; // 1 << 4
        u8 runeForgeBUsed : 1; // 1 << 5
        u8 field_bit_6 : 1;    // 1 << 6
        u8 field_bit_7 : 1;    // 1 << 7
    } __attribute__((packed)) d;
} __attribute__((packed)) PlayerStatusFlags;

/** Player.optionFlags bits, copied from SaveGlobals.optionsFlags by player_applyOptionFlags. */
enum PlayerOptionFlag
{
    // below a third of max HP, an idle player casts Herbal Healing when it can
    PLAYER_OPTION_FLAG_AUTO_HEALING = 1 << 0,

    // L+A / L+B / L+R cast quickSelectActiveSkills[] instead of L cycling selectedActiveSkill
    PLAYER_OPTION_FLAG_QUICK_SKILL_SELECT = 1 << 1,
};

/** Player.optionFlags; matched code tests the PLAYER_OPTION_FLAG_* masks on the packed view. */
typedef union {
    u8 p;

    struct
    {
        u8 autoHealing : 1;      // 1 << 0
        u8 quickSkillSelect : 1; // 1 << 1
        u8 field_bit_2 : 1;      // 1 << 2
        u8 field_bit_3 : 1;      // 1 << 3
        u8 field_bit_4 : 1;      // 1 << 4
        u8 field_bit_5 : 1;      // 1 << 5
        u8 field_bit_6 : 1;      // 1 << 6
        u8 field_bit_7 : 1;      // 1 << 7
    } __attribute__((packed)) d;
} __attribute__((packed)) PlayerOptionFlags;

/**
 * A hero's base primary stats, the saved source stats[] is rebuilt from (SaveSlot low bytes;
 * attribute points raise these). Passed by value to the max HP / spirit formulas.
 */
typedef struct HeroBaseStats
{
    u16 maxHp; // player_computeMaxHp of the five below; shown on the character screen
    u16 courage;
    u16 defense;
    u16 health;
    u16 accuracy;
    u16 strength;
} HeroBaseStats;

/** Player inventory slot indices. */
enum InventorySlot
{
    INVENTORY_SLOT_ARMOR = 0,
    INVENTORY_SLOT_WEAPON = 1,
    INVENTORY_SLOT_OFFHAND = 2,
    INVENTORY_SLOT_HELMET = 3,
    INVENTORY_SLOT_CLOAK = 4,
    INVENTORY_SLOT_GLOVES = 5,
    INVENTORY_SLOT_BOOTS = 6,
    INVENTORY_SLOT_JEWELRY = 7,
    INVENTORY_SLOT_BACKPACK_0 = 8,  // Row 1 column 1
    INVENTORY_SLOT_BACKPACK_1 = 9,  // Row 1 column 2
    INVENTORY_SLOT_BACKPACK_2 = 10, // Row 1 column 3
    INVENTORY_SLOT_BACKPACK_3 = 11, // Row 1 column 4
    INVENTORY_SLOT_BACKPACK_4 = 12, // Row 2 column 1
    INVENTORY_SLOT_BACKPACK_5 = 13, // Row 2 column 2
    INVENTORY_SLOT_BACKPACK_6 = 14, // Row 2 column 3
    INVENTORY_SLOT_BACKPACK_7 = 15, // Row 2 column 4
};

/** Equipment slots (armor..jewelry) come before the backpack cells. */
#define INVENTORY_SLOT_IS_EQUIPMENT(slot) ((slot) < INVENTORY_SLOT_BACKPACK_0)

typedef union Inventory {
    Item array[16]; // indexed using InventorySlot

    struct
    {
        Item armor;
        Item weapon;
        Item offhand;
        Item helmet;
        Item cloak;
        Item gloves;
        Item boots;
        Item jewelry;

        Item backpack_0; // Row 1 column 1
        Item backpack_1; // Row 1 column 2
        Item backpack_2; // Row 1 column 3
        Item backpack_3; // Row 1 column 4
        Item backpack_4; // Row 2 column 1
        Item backpack_5; // Row 2 column 2
        Item backpack_6; // Row 2 column 3
        Item backpack_7; // Row 2 column 4
    } slots;
} Inventory;

/** IDs for the playable heroes. */
typedef enum HeroId
{
    HERO_ID_FRODO = 0,
    HERO_ID_LEGOLAS = 1,
    HERO_ID_ARAGORN = 2,
    HERO_ID_GANDALF = 3,
    HERO_ID_EOWYN = 4,
    HERO_ID_GIMLI = 5,
    HERO_ID_SAM = 6,
    HERO_ID_SMEAGOL = 7,
} __attribute__((packed)) HeroId;
/** One player's character, including HP, spirit, and stats. */
typedef struct Player
{
    Actor *ownerActor;

    /** Shield bubble for Gandalf and Gimli skills */
    Actor *shieldBubble;

    /** The One Ring effect actor while the ring is worn; corruption stops decaying while set */
    Actor *ringActor;

    /** Legolas: aim FX actor of the held-B arrow volley (action state 0x60) */
    Actor *aimFxActor;

    /** affix glow emitters of the weapon (0) and offhand (1) hands, chosen by the item's affix
     * flags */
    FxEmitter *handFxEmitters[2];

    /** the hero's skill tree: HERO_PASSIVE_SKILL_COUNT PassiveSkillId entries
     * (HeroPassiveSkillIds*), indexes passiveSkillLevels
     */
    const u8 *passiveSkillIds;

    /** this player's active skill data table (per heroId; see active skill.h) */
    void *activeSkillTable;

    /** the hero's 6 LZ77 active skill icon assets, one per activeSkillTable row */
    const void *const *activeSkillIcons;

    /** emitter following the actor; shown while walking on terrain type 3 (wading) */
    FxEmitter *walkFxEmitter;

    HeroBaseStats baseStats;

    /** actor position at spawn; the death timeout revives the actor here */
    Vector2Fp16 respawnCoords;

    Inventory inventory;

    /**
     * the 9 Ranger Hollow items (shared stash; serialized to the GLOBAL
     * save header at g_SaveGlobals.rangerHollowItems, not the per-slot buffer).
     */
    Item rangerHollowItems[9];

    /**
     * distance to the nearest mission warp this frame; scene_gameplay_tick resets it, each
     * actor_missionWarp_tick lowers it and treats <= 0x9f as in range
     */
    s32 nearestWarpDistance;

    /**
     * use StatIndex enum to index stats. Note: stats are NOT saved directly; baseStats, level,
     * passiveSkillLevels, and the equipment affixes are saved, and stats[] is rebuilt from them on
     * load.
     */
    s16 stats[90];

    /**
     * bought level (0-5) of each passiveSkillIds slot; saved 3-bit-packed into
     * SaveSlot.skillLevelData
     */
    u8 passiveSkillLevels[HERO_PASSIVE_SKILL_COUNT];

    /**
     * levels whose PassiveSkill stat records are applied to stats[]; copied from passiveSkillLevels
     * and re-applied by player_initHeroLoadout. BUG: passiveSkill_applyAllLevels walks 10 slots, so
     * passiveSkillIds[9] (a pad byte, PASSIVE_SKILL_FEARLESS) is also applied
     * activeSkillLevelsPurchased[0] times.
     */
    u8 passiveSkillLevelsApplied[HERO_PASSIVE_SKILL_COUNT];

    /**
     * bought level of each active skill row, mirrored into activeSkillLevels; [0..4] saved
     * 3-bit-packed into SaveSlot.skillLevelData, [ACTIVE_SKILL_HERBAL_HEALING] fixed at 1
     */
    u8 activeSkillLevelsPurchased[6];

    /** current level of each active skill row (copied from activeSkillLevelsPurchased on load) */
    u8 activeSkillLevels[8];

    /** total experience; accumulated by player_awardXp (saved as SaveSlot.experience). */
    u32 experience;

    s16 currentHp;
    s16 currentSpirit;

    /**
     * Legolas: frames the attack button has been held; at 15 with a bow equipped the actor
     * enters the arrow-volley state 0x60. Not saved.
     */
    u16 bowHoldFrames;

    /** STAT_MEDITATIVE tick: 30-frame countdown while idle to the next +2 HP. Not saved. */
    u16 meditativeTimer;

    /**
     * gem balance, the currency; added by selling items at a shop (0x0801f7d4),
     * clamped 0..0xFFFF. Saved as SaveSlot.gems.
     */
    u16 gems;

    /**
     * kill counter; 500 -> reward, 2000 -> unlock. Saved as
     * SaveSlot.killCount.
     */
    u16 killCount;

    /**
     * unique weapons this player has picked up; bit = lootType -
     * LOOT_TYPE_UNIQUE_WEAPON_MIN, set by actor_lootPile_tick (ITEM_FLAG_UNIQUE_WEAPON),
     * never cleared. Dedup only: blocks the drop reroll and re-pickup of owned uniques,
     * so a discarded/traded-away unique weapon is gone for good for this character.
     * BUG: actor_lootPile_trigger also probes this mask for carried passives (inert; see
     * its BUG comment). Saved as SaveSlot.uniqueWeaponsCollected.
     */
    u16 uniqueWeaponsCollected;

    /**
     * unique carried passives this player has picked up; bit = lootType -
     * LOOT_TYPE_UNIQUE_PASSIVE_MIN, set by actor_lootPile_tick (ITEM_FLAG_CARRIED_PASSIVE,
     * Orc Drum / Orc Head excluded). BUG: loot_rollBaseItem's owned check is inverted
     * (non-Orc passives unobtainable) and item_deleteFromInventory's clear is a no-op;
     * see the BUG comments there. Saved as SaveSlot.uniquePassivesCollected.
     */
    u16 uniquePassivesCollected;

    /** PlayerCombatFlags (mask view: .p & PLAYER_COMBAT_FLAG_*) */
    PlayerCombatFlags combatFlags;

    /**
     * collected-rune word; bit = rune index (enum RuneId, 0..0xe; the drop rolls
     * rng_rollRange(0, RUNE_NONE - 1)). A dwarf-rune drop's pickup sets its bit;
     * loot_rollBaseItem only rolls runes whose bit is clear; a Rune Forge purchase
     * unlocks the offered rune. Saved as SaveSlot.d.runes.
     */
    RuneFlags runes;

    /** active skill row on the cast button (ACTIVE_SKILL_NONE = none); L cycles it. Not saved. */
    u8 selectedActiveSkill;

    /** active skill rows cast by L+A / L+B / L+R with PLAYER_OPTION_FLAG_QUICK_SKILL_SELECT; saved
     */
    u8 quickSelectActiveSkills[3];

    /** 1 = the active skill row takes part in the cast-button cycle; saved as a 6-bit roll-up */
    u8 activeSkillCycleEnabled[6];

    /** PlayerOptionFlags (mask view: .p & PLAYER_OPTION_FLAG_*) */
    PlayerOptionFlags optionFlags;

    /**
     * corruption meter, 0-100. Its 51 / 81 / 91 thresholds drive the global
     * GAME_FLAG_CORRUPTION_TIER_* bits. Saved as SaveSlot.corruption.
     */
    u8 corruption;

    /** item level requirements pass when level+1 >= required */
    u8 level;

    /** unspent attribute points */
    u8 attributePoints;

    /** unspent skill points */
    u8 skillPoints;

    /** Which hero the player is playing. */
    HeroId heroId;

    u8 field_0x1a0[3];

    /** activeSkillTable row being cast (set by activeSkill_cast, read by the cast states) */
    u8 castingActiveSkill;

    u8 field_0x1a4;

    /** bit 0: an active skill cast is in progress; blocks active skill input until the actor idles
     * again */
    u8 activeSkillStateFlags;

    /**
     * number of attribute points purchased
     * TODO: Determine max range
     */
    u8 attributePointsPurchased;

    /**
     * number of skill points purchased
     * TODO: Determine range
     */
    u8 skillPointsPurchased;

    /**
     * level of whetstone purchased
     * TODO: Determine range
     */
    u8 whetstoneLevel;

    /**
     * misc per-character status (mask view: .p & PLAYER_STATUS_FLAG_*); saved as
     * SaveSlot.statusFlags
     */
    PlayerStatusFlags statusFlags;

    KingsfoilHerbs kingsfoilHerbs;

    u8 field_0x1ab;
} Player;

/**
 * The player RAM block @0x03003c00: the two player slots plus the active-player
 * count/index bytes trailing the array. The ROM reaches the trailing bytes off the
 * array base with a displacement, hence one struct global.
 */
typedef struct PlayerGlobals
{
    Player players[2];
    u8 activePlayerCount;
    u8 activePlayerIndex;
} PlayerGlobals;
#define PLAYER(playerIndex) g_PlayerGlobals.players[playerIndex]
#define PLAYER_STAT(playerIndex, statIndex) PLAYER(playerIndex).stats[statIndex]
#define ACTIVE_PLAYER_INDEX (g_PlayerGlobals.activePlayerIndex)
#define ACTIVE_PLAYER PLAYER(ACTIVE_PLAYER_INDEX)

/** union PlayerHudFlags bits (the mask view the code tests). */
enum PlayerHudFlag
{
    PLAYERHUD_FLAG_0 = 1 << 0,            // loot-pile HUD text shown
    PLAYERHUD_FLAG_1 = 1 << 1,            // loot-pile HUD text (re)draw
    PLAYERHUD_FLAG_HP_DIRTY = 1 << 3,     // set by player_addHp on any HP change
    PLAYERHUD_FLAG_SPIRIT_DIRTY = 1 << 4, // set by player_addSpirit on any spirit change
    PLAYERHUD_FLAG_POISON = 1 << 10,
    PLAYERHUD_FLAG_FEAR = 1 << 11,
    PLAYERHUD_FLAG_12 = 1 << 12, // set together with POISON/FEAR on status application
};

/** PlayerHud.flags; matched code tests the PLAYERHUD_FLAG_* masks on the word view instead. */
typedef union PlayerHudFlags {
    u32 p;

    struct
    {
        u32 field_bit_0 : 1;  // 1 << 0: loot-pile HUD text shown
        u32 field_bit_1 : 1;  // 1 << 1: loot-pile HUD text (re)draw
        u32 field_bit_2 : 1;  // 1 << 2
        u32 hpDirty : 1;      // 1 << 3: set by player_addHp on any HP change
        u32 spiritDirty : 1;  // 1 << 4: set by player_addSpirit on any spirit change
        u32 field_bit_5 : 1;  // 1 << 5
        u32 field_bit_6 : 1;  // 1 << 6
        u32 field_bit_7 : 1;  // 1 << 7
        u32 field_bit_8 : 1;  // 1 << 8
        u32 field_bit_9 : 1;  // 1 << 9
        u32 poison : 1;       // 1 << 10
        u32 fear : 1;         // 1 << 11
        u32 field_bit_12 : 1; // 1 << 12: set together with poison/fear on status application
        u32 field_bit_13 : 1; // 1 << 13
        u32 field_bit_14 : 1; // 1 << 14
        u32 field_bit_15 : 1; // 1 << 15
        u32 field_bit_16 : 1; // 1 << 16
        u32 field_bit_17 : 1; // 1 << 17
        u32 field_bit_18 : 1; // 1 << 18
        u32 field_bit_19 : 1; // 1 << 19
        u32 field_bit_20 : 1; // 1 << 20
        u32 field_bit_21 : 1; // 1 << 21
        u32 field_bit_22 : 1; // 1 << 22
        u32 field_bit_23 : 1; // 1 << 23
        u32 field_bit_24 : 1; // 1 << 24
        u32 field_bit_25 : 1; // 1 << 25
        u32 field_bit_26 : 1; // 1 << 26
        u32 field_bit_27 : 1; // 1 << 27
        u32 field_bit_28 : 1; // 1 << 28
        u32 field_bit_29 : 1; // 1 << 29
        u32 field_bit_30 : 1; // 1 << 30
        u32 field_bit_31 : 1; // 1 << 31
    } d;
} PlayerHudFlags;
/**
 * Per-player in-game HUD/status block (provisional identification - only the text head
 * and .flags are decoded).
 */
typedef struct PlayerHud
{
    u8 field_0x0[4];

    /**
     * the HUD text being shown: what the player stands on or is next to ("A Pile of
     * @1 Gems", the item name, ...), set by actor_lootPile_tick
     */
    char *text;

    /** decode/format buffer behind text */
    char textBuffer[100];

    u8 field_0x6c[4];

    /** PlayerHudFlags (mask view: .p & PLAYERHUD_FLAG_*) */
    PlayerHudFlags flags;

    u8 field_0x74[2];
    u8 field_0x76;
    u8 field_0x77;

    /** HUD text color (HUD_TEXT_COLOR_*, also set by item_affix_formatName) */
    u8 textColor;

    u8 field_0x79[3];
} PlayerHud;

/**
 * PlayerHud.textColor values: the gameplay and menu font palettes map the same colors
 * to different indices; menu values inferred from the setItemNameColor pairs.
 */
enum HudTextColorGameplay
{
    HUD_TEXT_COLOR_GAMEPLAY_WHITE = 0,
    HUD_TEXT_COLOR_GAMEPLAY_YELLOW = 1,
    HUD_TEXT_COLOR_GAMEPLAY_RED = 6,
    HUD_TEXT_COLOR_GAMEPLAY_BLUE = 9,
    HUD_TEXT_COLOR_GAMEPLAY_GREEN = 12,
};

enum HudTextColorMenu
{
    HUD_TEXT_COLOR_MENU_WHITE = 3,
    HUD_TEXT_COLOR_MENU_RED = 6,
    HUD_TEXT_COLOR_MENU_GREEN = 9,
    HUD_TEXT_COLOR_MENU_BLUE = 12,
};

/**
 * Add delta to a player's current HP (clamped to STAT_MAX_HP) and mark the HUD HP display dirty;
 * on damage with flinchOnDamage set, knock the player's actor into the hurt action state.
 */
void player_addHp(u8 playerIndex, s16 delta, bool flinchOnDamage);

/** Subtract HP for damage application: negates damage and forwards to player_addHp. */
static inline void player_subtractHp(u8 playerIndex, s32 damage, bool flinchOnDamage)
{
    player_addHp(playerIndex, -damage, flinchOnDamage);
}

/**
 * Add delta to a player's current spirit (clamped to [0, STAT_MAX_SPIRIT]) and mark the HUD spirit
 * display dirty.
 */
void player_addSpirit(u8 playerIndex, s16 delta);

/**
 * Add delta to a player's corruption meter (clamped to [0, 100]) and re-evaluate the global
 * GAME_FLAG_CORRUPTION_TIER_* bits from the new value. A gain is scaled by
 * STAT_CORRUPTION_RESIST_PERCENT, which amplifies it while negative.
 */
void player_addCorruption(u8 playerIndex, s8 delta);

/**
 * Re-clamp currentHp to STAT_MAX_HP and currentSpirit to STAT_MAX_SPIRIT after a stat change; a
 * non-positive max floors HP to 1 and spirit to 0.
 */
void player_clampCurrentHpSpirit(u8 playerIndex);

/**
 * Apply one affix/rune stat modifier (item_applyAffixStats' worker). An offhand item's
 * STAT_DAMAGE_SLASH/STAT_DAMAGE_IMPALE contribution is halved, STAT_ALL_PRIMARY_STATS fans out
 * to the five primaries, and touching a primary stat recomputes STAT_MAX_HP/STAT_MAX_SPIRIT and
 * re-clamps via player_clampCurrentHpSpirit.
 */
void player_addStat(u8 inventorySlot, u8 statIndex, u8 playerIndex, s8 value);

/**
 * Recompute the equipment-derived PlayerCombatFlags bits from the equipped weapon and
 * offhand item types.
 */
void player_updateEquipFlags(u8 playerIndex);

void player_awardXp(s16 initialXp);

/** Spawn the level-up glow FX actor following the player's actor. */
Actor *player_spawnLevelUpFx(Actor *playerActor);

/**
 * Spawn a 90-frame invulnerability bubble on the player (STAT_INVULNERABILITY_PROC_PERCENT proc);
 * sets ACTOR_FLAG_INVULNERABLE on the player and Player.shieldBubble.
 */
Actor *player_spawnInvulnerabilityBubble(Actor *playerActor);

/**
 * Spawn a 300-frame triple-damage buff bubble on the player (STAT_TRIPLE_DAMAGE_PROC_PERCENT
 * proc); sets PLAYER_COMBAT_FLAG_TRIPLE_DAMAGE and Player.shieldBubble.
 */
Actor *player_spawnTripleDamageBubble(Actor *playerActor);

/** Reset a player's inventory (16 slots) to empty. */
void player_clearInventory(u32 playerIndex);

/**
 * Store a picked-up item: merges arrow stacks, auto-equips into the matching empty equip
 * slot (two-handed conflicts checked), else takes the first free backpack slot.
 *
 * @return FALSE when there is no room
 */
bool player_addItemToInventory(Item item, u8 playerIndex);
/** Reset the active skill lists/levels of both players. */
void player_resetActiveSkills(void);
/** Clear the PlayerHud tail bytes of both players. */
void player_resetHuds(void);

/**
 * Advance one player's HUD one frame: redraw whatever its PLAYERHUD_FLAG_* bits marked
 * dirty, then the always-on parts (bars, text, status icons).
 */
void player_tickHud(s32 playerIndex);
/** Set up a freshly created character: base items and hollow state. */
void player_initItemsAndHollow(u32 playerIndex);

/**
 * Swap Frodo and Sam for the Cirith Ungol missions (Frodo becomes Sam on mission 6 variant 7,
 * Sam becomes Frodo on mission 7), adjusting baseStats.health/strength by 5.
 */
void player_applyFrodoSamSwap(u32 playerIndex);

/** Max HP from the base stats: 4 * health + courage + strength. */
s16 player_computeMaxHp(u8 playerIndex, HeroBaseStats baseStats);

/** Max spirit from the base stats: 4 * courage + health. */
s16 player_computeMaxSpirit(u8 playerIndex, HeroBaseStats baseStats);

/** Copy baseStats into the five primary stats and recompute STAT_MAX_HP / STAT_MAX_SPIRIT. */
void player_applyBaseStats(u8 playerIndex);

/** Refresh baseStats.maxHp from the primaries, then player_applyBaseStats. */
void player_recomputeBaseStats(u8 playerIndex);

/** Zero stats[] and the per-mission scratch, apply the base stats, then reload the slot state. */
void player_rebuildStats(u8 playerIndex);

/**
 * Add one PassiveSkill stat record value to stats[]: a primary stat or STAT_ALL_PRIMARY_STATS also
 * recomputes max HP / spirit, STAT_SPEED_PERCENT sets PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY.
 */
void player_addPassiveSkillStat(u8 playerIndex, u8 statIndex, s16 delta);

/** Replace the hand's affix glow emitter (handFxEmitters[hand]) with the one item calls for. */
void player_setHandFx(u8 playerIndex, u8 hand, Item item);

/** Free the hand's affix glow emitter and detach its sprite. */
void player_clearHandFx(u8 playerIndex, u8 hand);
/** Apply the persistent options (Auto Healing, Quick PassiveSkill Select) to a player's flags. */
void player_applyOptionFlags(u32 playerIndex);
/** Write the per-hero base stats for heroId into a player's stat block. */
void player_initHeroStats(u32 heroId, u32 playerIndex);
/** Second half of hero creation: starting gear/skills (copies field_0x158 into activeSkillLevels).
 */
void player_initHeroLoadout(u32 heroId, u32 playerIndex);
