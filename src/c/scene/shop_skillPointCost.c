#include "player.h"
#include "scene/shop.h"
#include "variables.h"

// Base price in gems for one skill point. Each purchased point adds this amount.
#define SKILL_POINT_BASE_COST 750

/**
 * Price in gems of the player's next skill point: 750 per point already bought plus 750.
 *
 * @romaddress 0x0801ff5c
 */
u32 scene_shop_skillPointCost(u32 playerIndex)
{
    u16 price = SKILL_POINT_BASE_COST;

    u16 total = PLAYER(playerIndex).skillPointsPurchased * price + price;
    return total;
}
