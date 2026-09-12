#include "combat.h"
#include "variables.h"

/**
 * Calculate the weapon damage of the player identified by the given playerIndex.
 * This handles slash damage, impale damage, and two-handed damage. It also
 * sets PLAYER_COMBAT_FLAG_DAMAGE_TYPE accordingly.
 *
 * @romaddress 0x08027904
 */
s32 combat_getWeaponDamage(s32 playerIndex)
{
    u16 spread =
        PLAYER_STAT(playerIndex, STAT_DAMAGE_SLASH) + PLAYER_STAT(playerIndex, STAT_DAMAGE_IMPALE);

    if (PLAYER_STAT(playerIndex, STAT_DAMAGE_SLASH) > PLAYER_STAT(playerIndex, STAT_DAMAGE_IMPALE))
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_DAMAGE_TYPE;
    }
    else
    {
        PLAYER(playerIndex).combatFlags.p &= ~PLAYER_COMBAT_FLAG_DAMAGE_TYPE;
    }

    if (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_TWO_HANDED)
    {
        spread += PLAYER_STAT(playerIndex, STAT_DAMAGE_TWOHAND);
    }
    return spread;
}

/**
 * Get the sum of all flat damage bonuses applied, including:
 *
 *   * +x damage stat that includes item affixes and skills
 *   * +x damage when using a shield/blades/axes/bows
 *   * +x damage when the player is at low HP
 *   * damage from strength
 *
 * @romaddress 0x08027984
 */
s32 combat_getFlatDamageBonus(s32 playerIndex)
{
    ItemType offHandType = PLAYER(playerIndex).inventory.slots.offhand.d.itemType;
    ItemType mainHandType = PLAYER(playerIndex).inventory.slots.weapon.d.itemType;
    u16 damage = 0;

    damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_FLAT);

    // TODO: Find out what hero-specific case this is
    if (PLAYER(playerIndex).heroId == HERO_ID_ARAGORN &&
        (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_1) &&
        (PLAYER(playerIndex).combatFlags.p & PLAYER_COMBAT_FLAG_2))
    {
        damage += PLAYER_STAT(playerIndex, STAT_UNKNOWN_86);
    }

    // Weapon type bonuses for each hand, e.g. +x damage with a shield/blades/axes/bows
    if (offHandType == ITEM_TYPE_SHIELD)
    {
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_SHIELD);
    }

    switch (offHandType)
    {
    case ITEM_TYPE_KNIFE:
    case ITEM_TYPE_SWORD_1H:
    case ITEM_TYPE_SWORD_2H:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_BLADE);
        break;
    case ITEM_TYPE_AXE:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_AXE);
        break;
    case ITEM_TYPE_BOW:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_BOW);
        break;
    default:
        break;
    }

    switch (mainHandType)
    {
    case ITEM_TYPE_KNIFE:
    case ITEM_TYPE_SWORD_1H:
    case ITEM_TYPE_SWORD_2H:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_BLADE);
        break;
    case ITEM_TYPE_AXE:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_AXE);
        break;
    case ITEM_TYPE_BOW:
        damage += PLAYER_STAT(playerIndex, STAT_DAMAGE_WITH_BOW);
        break;
    default:
        break;
    }

    if (PLAYER_STAT(playerIndex, STAT_LOW_HP_DAMAGE) != 0)
    {
        u32 hpPercent =
            math_percentage(PLAYER(playerIndex).currentHp, PLAYER_STAT(playerIndex, STAT_MAX_HP));
        if (hpPercent <= LOW_HP_BONUS_THRESHOLD_PERCENT)
        {
            damage += PLAYER_STAT(playerIndex, STAT_LOW_HP_DAMAGE);
        }
    }

    if (PLAYER_STAT(playerIndex, STAT_STRENGTH) > STRENGTH_DAMAGE_BASE)
    {
        s16 strength = PLAYER_STAT(playerIndex, STAT_STRENGTH);
        u8 strengthDamage = (strength - STRENGTH_DAMAGE_BASE) / STRENGTH_DAMAGE_STEP;

        damage += strengthDamage;
    }

    return damage;
}
