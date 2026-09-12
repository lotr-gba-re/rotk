#include "combat.h"

/**
 * Get the maximum damage bound for the player with the given playerIndex.
 *
 * @romaddress 0x080287dc
 */
u16 combat_getPlayerMaxDamage(s32 playerIndex)
{
    u16 weaponDamage = combat_getWeaponDamage(playerIndex);
    u32 flatDamage = combat_getFlatDamageBonus(playerIndex);

    return weaponDamage + flatDamage;
}

/**
 * Get the minimum damage bound for the player with the given playerIndex.
 *
 * @romaddress 0x08028800
 */
u16 combat_getPlayerMinDamage(s32 playerIndex)
{
    u16 weaponDamage = combat_getWeaponDamage(playerIndex);
    u16 halfWeaponDamage = weaponDamage / 2;
    u32 flatDamage = combat_getFlatDamageBonus(playerIndex);

    return halfWeaponDamage + flatDamage;
}
