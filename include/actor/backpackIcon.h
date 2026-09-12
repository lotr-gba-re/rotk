#pragma once

#include "types.h"

/** backpack-scene item icon sprite (g_BackpackSlotSprites) */
typedef struct ActorBackpackIconState
{
    u8 field_0x84[6];

    /**
     * the active player can equip/use the item; FALSE picks the cannot-equip cursor
     * animation
     */
    bool equippable;
} ActorBackpackIconState;
