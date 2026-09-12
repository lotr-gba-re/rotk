#pragma once

#include "eeprom.h"
#include "item.h"
#include "player.h"
#include "text.h"
#include "types.h"

/** union OptionFlags bits (the mask view the code tests). */
typedef enum OptionFlag
{
    OPTION_FLAG_FAST_DISCARD = 1 << 0,       // OPTIONS_ROW_FAST_DISCARD
    OPTION_FLAG_MUSIC_ENABLED = 1 << 1,      // music on/off; cleared at volume 0
    OPTION_FLAG_SFX_ENABLED = 1 << 2,        // sfx on/off; cleared at volume 0
    OPTION_FLAG_QUICK_SKILL_SELECT = 1 << 3, // OPTIONS_ROW_QUICK_SKILL_SELECT
    OPTION_FLAG_AUTO_HEALING = 1 << 4,       // OPTIONS_ROW_AUTO_HEALING
    // difficulty selector is a 3-way: none | HARD | GRUELING (mutually exclusive)
    OPTION_FLAG_DIFFICULTY_HARD = 1 << 5,
    OPTION_FLAG_DIFFICULTY_GRUELING = 1 << 6,
} OptionFlag;

/**
 * SaveGlobals.optionsFlags; matched code tests the OPTION_FLAG_* masks on the byte view.
 * Packed (union and bitfield struct): agbcc would otherwise pad the union to 4-byte
 * size/alignment and shift the SaveGlobals layout.
 */
typedef union OptionFlags {
    u8 p;

    struct
    {
        u8 fastDiscard : 1;        // 1 << 0
        u8 musicEnabled : 1;       // 1 << 1
        u8 sfxEnabled : 1;         // 1 << 2
        u8 quickSkillSelect : 1;   // 1 << 3
        u8 autoHealing : 1;        // 1 << 4
        u8 difficultyHard : 1;     // 1 << 5
        u8 difficultyGrueling : 1; // 1 << 6
        u8 field_bit_7 : 1;        // 1 << 7
    } __attribute__((packed)) d;
} __attribute__((packed)) OptionFlags;

/** Slot mission sentinels (SaveSlot.d.mission) */
enum SlotMission
{
    /** the empty/invalid sentinel; a mission >= this is treated as an empty slot */
    SLOT_MISSION_EMPTY = 0x82,
};

/** Packed hero byte of a slot: Player.heroId in the high nibble, whetstoneLevel low. */
typedef union SaveSlotHero {
    u8 p;
    struct
    {
        u8 whetstoneLevel : 4;
        u8 heroId : 4;
    } __attribute__((packed)) d;
} __attribute__((packed)) SaveSlotHero;

/**
 * One full character savegame slot (112 bytes / 14 EEPROM blocks)
 *
 * Exposed as a union for u8/u16 type punning access.
 */
typedef union SaveSlot {
    u8 bytes[0x70];

    u16 halfwords[0x38];

    u32 words[0x1c];

    /**
     * Decoded field view, in slot-byte order. Offsets from save_serializePlayer /
     * save_deserializePlayer (serialize indexes by u16, deserialize by u32, but the byte
     * positions below are identical).
     */
    struct
    {
        /** inventory items (equipment and backpack) */
        Inventory inventory;

        u32 experience;

        /**
         * 3-bit-packed Player.passiveSkillLevels[9] and activeSkillLevelsPurchased[0..4], then
         *  Player.quickSelectActiveSkills[3] and a 6-bit roll-up of activeSkillCycleEnabled[];
         *  byte 3 bits 3-6 = Player.skillPointsPurchased.
         *  Packed mid-byte so kept as a blob. Levels are 0-5 (capped; 0x08042bc4 for skills,
         *  0x08045138 for active skills).
         */
        u8 skillLevelData[8];

        s16 currentHp;

        s16 currentSpirit;

        u16 gems;

        u16 killCount;

        u16 uniqueWeaponsCollected;

        u16 uniquePassivesCollected;

        /** Player.runes: collected-rune word (bit = enum RuneId). */
        RuneFlags runes;

        PlayerStatusFlags statusFlags;

        /** Player.corruption. */
        u8 corruption;

        u8 level;

        /** Unspent attribute points. */
        u8 attributePoints;

        /** Unspent skill points. */
        u8 skillPoints;

        u8 strength;
        u8 accuracy;
        u8 health;
        u8 defense;
        u8 courage;

        u8 attributePointsPurchased;

        SaveSlotHero hero;

        KingsfoilHerbs kingsfoilHerbs;

        /** mission variant at save (stamped from g_MissionVariant). */
        u8 variant;

        /** MissionId at save; SLOT_MISSION_EMPTY marks a blank slot. */
        u16 mission;

        /** spawn point at save (g_MissionSpawnPoint[ACTIVE_PLAYER_INDEX]). */
        u8 spawnPoint;

        /** script checkpoint vars at save (g_MissionCheckpointVars). */
        u8 checkpointVars[3];

        /** negated save_checksum16 over all 0x70 bytes. */
        u16 checksum;
    } d;
} SaveSlot;

#define SAVE_SLOT_COUNT 4

/**
 * A slot's story progress parked per player while a co-op or bonus-map session runs
 * (GAME_FLAG_COOP_ACTIVE, GAME_FLAG_COOP_LEVEL_SELECTED, GAME_FLAG_BONUS_MAP_ACTIVE, or a
 * link session): save_deserializePlayer stores it here instead of into the mission state,
 * and save_serializePlayer writes it back to the slot unchanged.
 */
typedef struct SaveParkedProgress
{
    u16 mission;
    u8 variant;
    u8 spawnPoint;
    u8 checkpointVars[3];
} SaveParkedProgress;

/** UnlockFlags.p bits (the mask view the code tests). */
typedef enum UnlockFlag
{
    UNLOCK_FLAG_CHARACTER_SAM = 1 << 0,        // GameCube link reward
    UNLOCK_FLAG_BONUS_MAP_FANGORN = 1 << 1,    // Finish the game with all characters
    UNLOCK_FLAG_BONUS_MAP_WEATHERTOP = 1 << 2, // Collect all artifacts
    UNLOCK_FLAG_BONUS_MAP_HELMS_DEEP = 1 << 3, // Finish the game with any character
    // per-hero completion, set by scene_credits_enter from the played hero
    UNLOCK_FLAG_FINISHED_AS_FRODO = 1 << 8, // also set by Sam and Smeagol
    UNLOCK_FLAG_FINISHED_AS_LEGOLAS = 1 << 9,
    UNLOCK_FLAG_FINISHED_AS_EOWYN = 1 << 10,
    UNLOCK_FLAG_FINISHED_AS_ARAGORN = 1 << 11,
    UNLOCK_FLAG_FINISHED_AS_GANDALF = 1 << 12,
    UNLOCK_FLAG_FINISHED_AS_GIMLI = 1 << 13,
    UNLOCK_FLAG_CHARACTER_SMEAGOL = 1 << 14, // Finish the game with 2+ characters
    UNLOCK_FLAG_ANCIENT_ITEMS = 1 << 15,     // GameCube link reward
    UNLOCK_FLAG_BONUS_MAP_MORIA = 1 << 28,   // Get 2000 kills as any character
} UnlockFlag;

/**
 * Persistent unlock flags; SaveGlobals.d.unlockFlags (serialized in the EEPROM global
 * header). In co-op the session mirror g_CoopUnlockFlags is used instead (transmitted by
 * the link handler, loot_rollBaseItem). Views: `p` = the u32 mask view the code tests;
 * `d` = named bits.
 */
typedef union UnlockFlags {
    u32 p;

    struct
    {
        u32 characterSam : 1;       // 1 << 0: GameCube link reward
        u32 bonusMapFangorn : 1;    // 1 << 1: Finish the game with all characters
        u32 bonusMapWeathertop : 1; // 1 << 2: Collect all artifacts
        u32 bonusMapHelmsDeep : 1;  // 1 << 3: Finish the game with any character
        u32 field_bit_4 : 1;        // 1 << 4
        u32 field_bit_5 : 1;        // 1 << 5
        u32 field_bit_6 : 1;        // 1 << 6
        u32 field_bit_7 : 1;        // 1 << 7
        u32 finishedAsFrodo : 1;    // 1 << 8: also set by Sam and Smeagol
        u32 finishedAsLegolas : 1;  // 1 << 9
        u32 finishedAsEowyn : 1;    // 1 << 10
        u32 finishedAsAragorn : 1;  // 1 << 11
        u32 finishedAsGandalf : 1;  // 1 << 12
        u32 finishedAsGimli : 1;    // 1 << 13
        u32 characterSmeagol : 1;   // 1 << 14: Finish the game with 2+ characters
        u32 ancientItems : 1;       // 1 << 15: GameCube link reward
        u32 field_bit_16 : 1;       // 1 << 16
        u32 field_bit_17 : 1;       // 1 << 17
        u32 field_bit_18 : 1;       // 1 << 18
        u32 field_bit_19 : 1;       // 1 << 19
        u32 field_bit_20 : 1;       // 1 << 20
        u32 field_bit_21 : 1;       // 1 << 21
        u32 field_bit_22 : 1;       // 1 << 22
        u32 field_bit_23 : 1;       // 1 << 23
        u32 field_bit_24 : 1;       // 1 << 24
        u32 field_bit_25 : 1;       // 1 << 25
        u32 field_bit_26 : 1;       // 1 << 26
        u32 field_bit_27 : 1;       // 1 << 27
        u32 bonusMapMoria : 1;      // 1 << 28: Get 2000 kills as any character
        u32 field_bit_29 : 1;       // 1 << 29
        u32 field_bit_30 : 1;       // 1 << 30
        u32 field_bit_31 : 1;       // 1 << 31
    } d;
} UnlockFlags;

/**
 * Collected-artifact mask bits (the mask view the code tests). Bit N is the artifact
 * whose name is TEXT_ID_LOOT_NAME_BASE + N: the Artifacts gallery names slot N with
 * text 0x239 + N, and collecting sets bit = ActorLootPileMissionParams.lootType. All 8 bits set
 * unlocks UNLOCK_FLAG_BONUS_MAP_WEATHERTOP.
 */
typedef enum ArtifactFlag
{
    ARTIFACT_FLAG_DEAGOLS_SKULL = 1 << 0,
    ARTIFACT_FLAG_FEATHER_OF_GWAIHIR = 1 << 1,
    ARTIFACT_FLAG_SARUMANS_PALANTIR = 1 << 2,
    ARTIFACT_FLAG_SHELOBS_STINGER = 1 << 3,
    ARTIFACT_FLAG_GALADRIELS_LOCK = 1 << 4,
    ARTIFACT_FLAG_WITCH_KINGS_CROWN = 1 << 5,
    ARTIFACT_FLAG_THERE_AND_BACK_AGAIN = 1 << 6,
    ARTIFACT_FLAG_SIMBELMYNE = 1 << 7,
    // all 8 artifacts collected
    ARTIFACT_MASK_ALL = ARTIFACT_FLAG_DEAGOLS_SKULL | ARTIFACT_FLAG_FEATHER_OF_GWAIHIR |
                        ARTIFACT_FLAG_SARUMANS_PALANTIR | ARTIFACT_FLAG_SHELOBS_STINGER |
                        ARTIFACT_FLAG_GALADRIELS_LOCK | ARTIFACT_FLAG_WITCH_KINGS_CROWN |
                        ARTIFACT_FLAG_THERE_AND_BACK_AGAIN | ARTIFACT_FLAG_SIMBELMYNE,
} ArtifactFlag;

/**
 * Collected-artifact mask; SaveGlobals.d.artifactMask (bit = ActorLootPileMissionParams.lootType =
 * the artifact, see enum ArtifactFlag). Views: `p` = the u8 mask view the code tests;
 * `d` = named bits.
 */
typedef union ArtifactFlags {
    u8 p;

    struct
    {
        u8 deagolsSkull : 1;      // 1 << 0
        u8 featherOfGwaihir : 1;  // 1 << 1
        u8 sarumansPalantir : 1;  // 1 << 2
        u8 shelobsStinger : 1;    // 1 << 3
        u8 galadrielsLock : 1;    // 1 << 4
        u8 witchKingsCrown : 1;   // 1 << 5
        u8 thereAndBackAgain : 1; // 1 << 6
        u8 simbelmyne : 1;        // 1 << 7
    } __attribute__((packed)) d;
} __attribute__((packed)) ArtifactFlags;

/**
 * Global save area which stores global settings, ranger hollow items, and global unlocks.
 *
 * Exposed as a union for u8/u16 type punning access.
 */
typedef union SaveGlobals {
    /** 16-bit halfword view */
    u16 halfwords[0x20];

    /** 32-bit word view */
    u32 words[0x10];

    /** Decoded field view. */
    struct
    {
        /** header magic/version bytes (template @SaveHeaderTemplate) */
        u8 header[8];

        /** the 9 Ranger Hollow items, serialized from Player.rangerHollowItems */
        Item rangerHollowItems[9];

        /** persistent unlock flags (see union UnlockFlags) */
        UnlockFlags unlockFlags;

        /** options-screen flags (see union OptionFlags) */
        OptionFlags optionsFlags;

        /**
         * SFX volume in options menu.
         * TODO: mapping 0-10 -> u8
         */
        u8 sfxVolume;

        /**
         * Music volume in options menu.
         * TODO: mapping 0-10 -> u8
         */
        u8 musicVolume;

        /**
         * collected artifacts (see union ArtifactFlags); all 8 set
         * (ARTIFACT_MASK_ALL) unlocks UNLOCK_FLAG_BONUS_MAP_WEATHERTOP
         */
        ArtifactFlags artifactMask;

        /** Configured language. */
        Language language;

        /** set to 1 once a language has been chosen */
        u8 languageSet;

        u8 field_0x36[8];

        /** negated save_checksum16 over the full 0x40 bytes */
        u16 checksum;
    } d;
} SaveGlobals;

void save_deserializePlayer(u32 *slotBuffer, u32 playerIndex);
/** Serialize a player into slotBuffer; mission is stored as u16. */
void save_serializePlayer(u16 *slotBuffer, u32 mission);
bool save_writeGlobalHeader(void);
u16 save_checksum16(const u16 *data, u32 numBytes);

/** Load slot-derived state into the player after save_deserializePlayer. */
void save_loadSlotState(u32 playerIndex);
void save_writeSlot(void *src, u8 slotIndex);
void save_eeprom_readAll(void *dst);
u8 save_getSlotMission(SaveSlot *slot);
u8 save_readSlotAndGetMission(SaveSlot *slotBuffer, u8 slotIndex);
bool save_stampLanguage(void);
void save_refreshSlots(void);
bool save_validateHeader(void);
void save_formatHeader(void);

void save_readSlot(void *dst, u8 slotIndex);
void save_initEmptySlot(SaveSlot *slot);

void save_eeprom_readBlocks(u32 startBlock, s32 count, void *dst);
void save_eeprom_writeBlocks(u32 startBlock, s32 count, void *src);

/**
 * Enter an EEPROM access window: snapshot DISPSTAT, silence H-blank IRQs, select the chip type
 * (via eeprom_selectType), and arm the write-timeout timer. Pairs with save_eeprom_readEnd.
 */
void save_eeprom_readBegin(void);
/** Leave the EEPROM access window: restore DISPSTAT and the H-blank IRQ mask. */
void save_eeprom_readEnd(void);
