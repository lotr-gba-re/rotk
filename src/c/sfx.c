#include "sfx.h"
#include "save.h"
#include "variables.h"

/**
 * Set the SFX volume, gated on sfx being enabled.
 *
 * @param volume volume 0-10 (mapped to 0xff at 10, else v*0x19)
 *
 * @romaddress 0x08047140
 */
void sfx_setVolume(u8 volume)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED)
    {
        sfx_driverSetVolume(volume);
    }
}

/**
 * Set the music volume, gated on music being enabled.
 *
 * @param volume volume 0-10 (mapped to 0x7f at 10, else v*0xc)
 *
 * @romaddress 0x08047164
 */
void sfx_setMusicVolume(u8 volume)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED)
    {
        sfx_driverSetMusicVolume(volume, 1);
    }
}

/**
 * Stop the playing instance behind handle, gated on sfx being enabled.
 *
 * @param handle sound instance handle from sfx_play
 *
 * @romaddress 0x08047188
 */
void sfx_stop(u32 handle)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED)
    {
        sfx_driverStopHandle(handle);
    }
}

/**
 * Stop all playing sound effects, gated on sfx being enabled.
 *
 * @romaddress 0x080471a8
 */
void sfx_stopAll(void)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED)
    {
        sfx_driverStopAll();
    }
}

/**
 * Music mode stamped into g_UnknownMusic_0300765c after starting a song.
 * The mission-dependent selection collapsed to a single value: both arms yield 8.
 */
static inline u8 musicModeForMission(u32 missionId)
{
    if (missionId == 0)
    {
        return 8;
    }
    return 8;
}

/**
 * Request song songId, gated on music being enabled; a no-op if it is already current.
 * The current-song id is only updated while the playing flag is clear.
 *
 * @romaddress 0x080471c4
 */
void sfx_requestSong(u8 songId)
{
    u32 songIndex = songId;
    u8 pendingId = songIndex;
    SongEntry entry;

    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED)
    {
        if (songIndex == g_CurrentSongId)
        {
            return;
        }
        entry = Songs[songIndex];
        sfx_driverPlaySong(entry.data, entry.flags, entry.variant);
        g_UnknownMusic_0300765c = musicModeForMission(g_CurrentMissionId);
    }
    if (g_MusicPlaying == 0)
    {
        g_CurrentSongId = pendingId;
    }
}

/**
 * Stop all playing sound effects on scene exit, gated on sfx being enabled.
 * Twin of sfx_stopAll for the teardown path.
 *
 * @romaddress 0x08047230
 */
void sfx_stopAllOnExit(void)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_SFX_ENABLED)
    {
        sfx_driverStopAll();
    }
}

/**
 * Bare wrapper around sfx_resumeMusic.
 *
 * @romaddress 0x0804724c
 */
void sfx_resumeMusicWrapper(void)
{
    sfx_resumeMusic();
}

/**
 * Resume the paused music, gated on music being enabled, and set the playing flag.
 *
 * @romaddress 0x08047258
 */
void sfx_resumeMusic(void)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED)
    {
        g_MusicPlaying = sfx_driverIsMusicPlaying();
        if (g_MusicPlaying == 0)
        {
            sfx_driverResumeMusic(0);
            g_MusicPlaying = 1;
        }
    }
}

/**
 * Pause the music, gated on music being enabled and the playing flag, and clear the flag.
 *
 * @romaddress 0x08047290
 */
void sfx_pauseMusic(void)
{
    if ((g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED) && g_MusicPlaying != 0)
    {
        sfx_driverPauseMusic();
        g_MusicPlaying = 0;
    }
}

/**
 * Stop the music, gated on music being enabled, and clear the playing flag.
 *
 * @romaddress 0x080472c0
 */
void sfx_stopMusic(void)
{
    if (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_MUSIC_ENABLED)
    {
        sfx_driverStopMusic();
        g_MusicPlaying = 0;
    }
}
