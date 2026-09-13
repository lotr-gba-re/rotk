#pragma once

#include "types.h"

/**
 * Sound-effect ids passed to sfx_play (index into the sound table). Named by the
 * combat event that triggers each; the audible sound itself is not verified.
 */
typedef enum SfxId
{
    SFX_MENU_CURSOR_MOVE = 0x02,      // menu cursor moves (shop menu, language select)
    SFX_MENU_CONFIRM = 0x03,          // menu selection confirmed
    SFX_MENU_BACK = 0x04,             // menu cancelled / back out
    SFX_MENU_ERROR = 0x05,            // action rejected (locked bonus map, empty slot, ...)
    SFX_COMBAT_HIT_SLASH = 0x07,      // melee hit lands, slash-dominant weapon
    SFX_COMBAT_HIT_IMPALE = 0x08,     // melee hit lands, impale-dominant weapon
    SFX_COMBAT_BLOCK = 0x09,          // parry / shield block / generic avoidance
    SFX_COMBAT_EVADE = 0x0a,          // victim evades a melee hit
    SFX_COMBAT_ITEM_BREAK = 0x0b,     // Fragile weapon breaks
    SFX_COMBAT_VENOM_PROC = 0x0c,     // Venomous effect procs on a melee hit
    SFX_COMBAT_FIREBURST_PROC = 0x0d, // Fireburst effect procs on a melee hit
    SFX_COMBAT_SUNBURST = 0x0e,       // Sunburst on-kill AoE fires
    SFX_COMBAT_TRIPLE_DAMAGE =
        0x0f,                    // PLAYER_COMBAT_FLAG_TRIPLE_DAMAGE hit lands; also Nightburst AoE
    SFX_COMBAT_RIPOSTE = 0x10,   // riposte counterattack
    SFX_COMBAT_CRIT_HIT = 0x11,  // player critical hit lands
    SFX_COMBAT_KNOCKDOWN = 0x12, // crit-driven knockdown on a player
    SFX_COMBAT_HIT_MISSILE = 0x13,         // hit lands from the missile-attacking hero (heroId==1)
    SFX_COMBAT_PLAYER_DODGE = 0x1d,        // player dodges an enemy melee attack
    SFX_COMBAT_PLAYER_DODGE_RANGED = 0x1e, // player dodges an enemy ranged attack
    SFX_GEM_PICKUP = 0x29,       // gem pile pickup; also special pickups (Ent Water, Dwarf Rune)
    SFX_PICKUP_DENIED = 0x2a,    // pickup refused (backpack full, duplicate owned)
    SFX_ITEM_PICKUP = 0x2d,      // item or artifact pickup
    SFX_HERB_PICKUP = 0x2e,      // kingsfoil herb pickup
    SFX_EAT_FOOD = 0x2f,         // food drop eaten (actor_lootPile_eatFood, tiered herbs)
    SFX_COMBAT_INSTAKILL = 0x3f, // Aragorn's instakill melee proc fires
    SFX_CHEST_OPEN = 0x10a,      // chest opens
    SFX_UNLOCK = 0x119,          // unlock jingle: all 8 artifacts (Weathertop) / 2000 kills (Moria)
    SFX_LEVEL_UP = 0x11a,        // player level-up
    SFX_CORRUPTION_GAIN = 0x11b, // corruption meter gains more than 5 in one step
    SFX_CORRUPTION_TIER_3 = 0x11c,          // rolled 1 in 6 per corruption update at meter >= 91
    SFX_COMBAT_GANDALF_SHIELD_CAST = 0x5e,  // Gandalf shield bubble spawns (0x0803b940)
    SFX_COMBAT_GANDALF_SHIELD_ABSORB = 0x5f // Gandalf shield absorbs a hit (skill + bubble)
} SfxId;

/**
 * Play a sound effect. Returns the handle of the playing instance (0 if none played);
 * menus store it to stop overlap on repeat.
 */
u32 sfx_play(s32 sfxId);

/** Stop the playing instance behind handle (gated on sfx enabled). */
void sfx_stop(u32 handle);
/** Whether the instance behind handle is still active. */
bool sfx_isPlaying(u32 handle);

/** Set the SFX volume (gated on sfx enabled). volume = 10 -> 0xff, else v*0x19. */
void sfx_setVolume(u8 volume);
/** Set the music volume (gated on music enabled). volume = 10 -> 0x7f, else v*0xc. */
void sfx_setMusicVolume(u8 volume);
/** Stop all playing sound effects (gated on sfx enabled). */
void sfx_stopAll(void);
/** Stop all playing sound effects; the scene-exit teardown twin of sfx_stopAll. */
void sfx_stopAllOnExit(void);
// Vblank driver hooks run by system_vBlankIrqHandler: sfx_vBlankTick advances the
// thumb driver state; sfx_libVBlank is the veneer into the IWRAM sound-library vblank
// routine (buffer/FIFO management).
void sfx_vBlankTick(void);
void sfx_libVBlank(void);
/**
 * Song ids: index into Songs (sfx_requestSong's arg); placeholders until the
 * tracks are identified. SONG_ID_NONE is the g_CurrentSongId sentinel written
 * before sfx_requestSong to force a restart of the currently playing song.
 */
typedef enum SongId
{
    SONG_UNKNOWN_0 = 0,
    SONG_UNKNOWN_1 = 1,
    SONG_UNKNOWN_2 = 2,
    SONG_UNKNOWN_3 = 3,
    SONG_UNKNOWN_4 = 4,
    SONG_UNKNOWN_5 = 5,
    SONG_UNKNOWN_6 = 6,
    SONG_UNKNOWN_7 = 7,
    SONG_UNKNOWN_8 = 8,
    SONG_UNKNOWN_9 = 9,
    SONG_UNKNOWN_10 = 10,
    SONG_UNKNOWN_11 = 11,
    SONG_UNKNOWN_12 = 12,
    SONG_UNKNOWN_13 = 13,
    SONG_UNKNOWN_14 = 14,
    SONG_UNKNOWN_15 = 15,
    SONG_UNKNOWN_16 = 16,
    SONG_UNKNOWN_17 = 17,
    SONG_UNKNOWN_18 = 18,
    SONG_UNKNOWN_19 = 19,
    SONG_UNKNOWN_20 = 20,
    SONG_UNKNOWN_21 = 21,
    SONG_UNKNOWN_22 = 22,
    SONG_UNKNOWN_23 = 23,
    SONG_UNKNOWN_24 = 24,
    SONG_UNKNOWN_25 = 25,
    SONG_UNKNOWN_26 = 26,
    SONG_UNKNOWN_27 = 27,
    SONG_UNKNOWN_28 = 28,
    SONG_UNKNOWN_29 = 29,
    SONG_UNKNOWN_30 = 30,
    SONG_UNKNOWN_31 = 31,
    SONG_UNKNOWN_32 = 32,
    SONG_UNKNOWN_33 = 33,
    SONG_ID_NONE = 0xff,
} SongId;

/** Stop the music (gated on music enabled) and clear the playing flag. */
void sfx_stopMusic(void);
/** Request song songId (gated on music enabled); a no-op if it is already current. */
void sfx_requestSong(u8 songId);
/** Resume the paused music (gated on music enabled) and set the playing flag. */
void sfx_resumeMusic(void);
/** Bare wrapper around sfx_resumeMusic. */
void sfx_resumeMusicWrapper(void);
/** Pause the music (gated on music enabled and the playing flag) and clear the flag. */
void sfx_pauseMusic(void);

/** Song table entry (Songs, indexed by the sfx_requestSong song id). */
typedef struct SongEntry
{
    const void *data; // driver song data
    u8 flags;         // driver play flags (sfx_driverPlaySong arg2)
    u8 variant;       // per-song variant index 0-0x3f (sfx_driverPlaySong arg3)
} SongEntry;

/** Validate a sfx handle against the driver slot table (stamp + active flag). */
u32 sfx_isHandleActive(u32 handle);
/** IWRAM-thunk wrapper: stop the driver instance behind handle. */
void sfx_driverStopHandle(u32 handle);
void sfx_driverSetVolume(u8 volume);
void sfx_driverStopAll(void);
void sfx_driverPlaySong(const void *data, u32 flags, u32 variant);
void sfx_driverStopMusic(void);
void sfx_driverSetMusicVolume(u8 volume, u32 arg2);
// Music pause/resume driver state (word 0x02001648): 1 = playing, 2 = paused.
void sfx_driverResumeMusic(u32 arg1);
void sfx_driverPauseMusic(void);
u32 sfx_driverIsMusicPlaying(void);

void sfx_init(void);
void sfx_initDriver(void);

/**
 * Arm the sound engine's mixer timer (Timer 0 source + Timer 1 IRQ) and set
 * the mixer-active flag. Pairs with sfx_disableAudio; the two are called
 * back-to-back on every scene/menu transition to reset the engine.
 */
void sfx_enableAudio(void);
/** Disarm the mixer timer and clear the mixer-active flag. */
void sfx_disableAudio(void);
