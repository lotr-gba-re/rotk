#include "actor.h"
#include "combat.h"
#include "item.h"
#include "save.h"
#include "variables.h"

/**
 * Resolve a player's melee attack landing on an actor. Processes the hit if the swing animation is
 * on the hit frame.
 *
 * For Legolas using his knife attack skill, temporarily swaps the weapon/offhand affix stats for
 * the knife's around the hit processing.
 *
 * @romaddress 0x08015220
 */
void combat_resolvePlayerAttackOnActor(Actor *attacker, Actor *victim)
{
    s32 attackerPlayerIndex = attacker->as.combat.playerIndex;
    Player *players = g_PlayerGlobals.players;
    Player *attackerPlayer = &players[attackerPlayerIndex];
    u8 attackerHeroId = attackerPlayer->heroId;

    // Separate declaration and assignment required for matching
    Item legolasKnife;
    legolasKnife.d.itemType = ITEM_TYPE_EMPTY;

    if (attacker->animationFramesLeft == attacker->animationFrameDuration)
    {
        // Legolas: swap in the backpack knife's affixes as the weapon for the hit
        if (attackerHeroId == HERO_ID_LEGOLAS)
        {
            attacker->actionState = ACTOR_STATE_ATK_UNKNOWN_35;
            if (ITEM_TYPE_VIA_SHIFT(attackerPlayer->inventory.slots.backpack_0) ==
                    ITEM_TYPE_KNIFE &&
                !(attacker->field_0x100 & 0x20))
            {
                if (ITEM_TYPE_VIA_SHIFT(attackerPlayer->inventory.slots.weapon) != ITEM_TYPE_EMPTY)
                {
                    item_applyAffixStats(PLAYER(attackerPlayerIndex).inventory.slots.weapon,
                                         INVENTORY_SLOT_WEAPON, attackerPlayerIndex, TRUE);
                }
                if (ITEM_TYPE_VIA_SHIFT(attackerPlayer->inventory.slots.offhand) != ITEM_TYPE_EMPTY)
                {
                    item_applyAffixStats(PLAYER(attackerPlayerIndex).inventory.slots.offhand,
                                         INVENTORY_SLOT_OFFHAND, attackerPlayerIndex, TRUE);
                }
                legolasKnife = PLAYER(attackerPlayerIndex).inventory.slots.backpack_0;
                item_applyAffixStats(legolasKnife, INVENTORY_SLOT_WEAPON, attackerPlayerIndex,
                                     FALSE);
            }
        }

        if (victim->recordType == 0 && victim != attacker)
        {
            if (attackerHeroId == HERO_ID_SMEAGOL ||
                PLAYER(victim->as.combat.playerIndex).heroId == HERO_ID_SMEAGOL ||
                (g_SaveGlobals.d.optionsFlags.p & OPTION_FLAG_DIFFICULTY_GRUELING))
            {
                combat_resolvePvpAttack(attacker, victim);
            }
        }
        else if ((victim->flags.p & ACTOR_FLAG_NO_FLINCH) &&
                 victim->actionState != ACTOR_STATE_HURT)
        {
            combat_resolvePveAttack(attacker, victim);
        }
        else if (victim->flags.p & (ACTOR_FLAG_10 | ACTOR_FLAG_18 | ACTOR_FLAG_28))
        {
            combat_resolvePveAttack(attacker, victim);
        }

        // Legolas: swap the weapon/offhand affixes back in, knife back out
        if (attackerHeroId == HERO_ID_LEGOLAS)
        {
            attacker->actionState = ACTOR_STATE_ATK_UNKNOWN_35;
            if (legolasKnife.d.itemType == ITEM_TYPE_KNIFE && !(attacker->field_0x100 & 0x20))
            {
                if (ITEM_TYPE_VIA_SHIFT(PLAYER(attackerPlayerIndex).inventory.slots.weapon) !=
                    ITEM_TYPE_EMPTY)
                {
                    item_applyAffixStats(PLAYER(attackerPlayerIndex).inventory.slots.weapon,
                                         INVENTORY_SLOT_WEAPON, attackerPlayerIndex, FALSE);
                }
                if (ITEM_TYPE_VIA_SHIFT(PLAYER(attackerPlayerIndex).inventory.slots.offhand) !=
                    ITEM_TYPE_EMPTY)
                {
                    item_applyAffixStats(PLAYER(attackerPlayerIndex).inventory.slots.offhand,
                                         INVENTORY_SLOT_OFFHAND, attackerPlayerIndex, FALSE);
                }
                if (ITEM_TYPE_VIA_SHIFT(PLAYER(attackerPlayerIndex).inventory.slots.backpack_0) !=
                    ITEM_TYPE_EMPTY)
                {
                    item_applyAffixStats(PLAYER(attackerPlayerIndex).inventory.slots.backpack_0,
                                         INVENTORY_SLOT_WEAPON, attackerPlayerIndex, TRUE);
                }
            }
        }
    }
}
