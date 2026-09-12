// What ranger_hollow_pages exports. The page image format stays private to the .c: nothing
// outside this patch has business reading or writing a hollow page.
#pragma once

#if !HACK_ranger_hollow_pages
#error "requires the ranger_hollow_pages patch for the page format"
#endif

/**
 * Copy the save-header mirror's stash (a pre-migration save's whole stash) into page 1's SRAM
 * home. Called once from mod_save's migration run, after the hollow region is
 * blanked.
 */
void rangerHollowPages_migrateStash(void);
