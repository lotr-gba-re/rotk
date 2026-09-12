#include "gfx/items/staff/equip.inc"
#include "item.h"

// clang-format off

/**
 * Staff equipment-grid icons: the BG asset each row of StaffGfx points
 * at, in ROM order (see ITEM_BG_ICON).
 *
 * @romaddress 0x0879cc10
 */
// [0] "Short Staff"
ITEM_BG_ICON(StaffShortStaffEquipIcon, 4, 6);
// [1] "Staff"
ITEM_BG_ICON(StaffStaffEquipIcon, 4, 6);
// [2] "Quarterstaff"
ITEM_BG_ICON(StaffQuarterstaffEquipIcon, 4, 6);
// [3] "Magestaff"
ITEM_BG_ICON(StaffMagestaffEquipIcon, 4, 6);
// clang-format on
