#include "gfx/items/staff/backpack.inc"
#include "item.h"

// clang-format off

/**
 * Staff backpack icons: per row of StaffGfx its tiles, frame set and
 * palette, in ROM order (see ITEM_BACKPACK_ICON).
 *
 * @romaddress 0x0878e09c
 */
// [0] "Short Staff"
ITEM_BACKPACK_ICON(StaffShortStaffBackpackIcon);
// [1] "Staff"
ITEM_BACKPACK_ICON(StaffStaffBackpackIcon);
// [2] "Quarterstaff"
ITEM_BACKPACK_ICON(StaffQuarterstaffBackpackIcon);
// [3] "Magestaff"
ITEM_BACKPACK_ICON(StaffMagestaffBackpackIcon);
// clang-format on
