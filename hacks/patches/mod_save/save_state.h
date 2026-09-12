// The save manager: whether the chip holds a save this build can use, and mirroring it into
// RAM. The boot path needs nothing else; migration lives in save_migrate.h.
#pragma once

#if !HACK_mod_save
#error "requires the mod_save patch for the save manager"
#endif

#include "types.h"

/**
 * TRUE when a save chip is present and holds a perfect current magic + version match with a
 * valid checksum. A vanilla or newer save returns FALSE without being damaged in any way,
 * it just is not this format. Header-only, and touches no RAM state.
 */
bool modSave_isUpToDate(void);

/**
 * Mirror the save state from the chip into g_SaveGlobals and everything derived from it
 * (language, slot mirror, last slot, slot page). Replaces the stock save_init. The caller
 * has established the save is up to date.
 */
void modSave_loadState(void);
