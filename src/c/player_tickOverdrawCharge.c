#include "input.h"
#include "player.h"
#include "variables.h"

#define OVERDRAW_CHARGE_FRAMES 15

/**
 * Advance Legolas's bow charge and start Overdraw when charged.
 *
 * @return The new action state, or -1 when unchanged.
 *
 * @romaddress 0x08025cbc
 */
s32 player_tickOverdrawCharge(s32 playerIndex, Actor *actor)
{
    Item weapon = PLAYER(playerIndex).inventory.slots.weapon;
    Player *player = &PLAYER(playerIndex);

    if (player->heroId == HERO_ID_LEGOLAS && (PLAYER_KEYS_CURRENT(playerIndex) & B_BUTTON))
    {
        s16 frames = ++player->bowHoldFrames;
#ifdef BUGFIX
        // Check the family instead of the item type for normal and unique weapons.
        if (frames >= OVERDRAW_CHARGE_FRAMES &&
            actor->actionState != ACTOR_STATE_LEGOLAS_OVERDRAW &&
            weapon.d.itemType != ITEM_TYPE_EMPTY && ITEM_BASE_METADATA(weapon).flags.d.familyBow)
#else
        // BUG: Unique bows have item type UNIQUE, so Legolas cannot start Overdraw with them.
        if (frames >= OVERDRAW_CHARGE_FRAMES &&
            actor->actionState != ACTOR_STATE_LEGOLAS_OVERDRAW &&
            weapon.d.itemType == ITEM_TYPE_BOW)
#endif
        {
            HeroId hero;

            actor->velocity.x = 0;
            actor->velocity.y = 0;
            actor->flags.p |= ACTOR_FLAG_ANIMATION_PLAYING;
            actor->as.combat.requestedPose = 0;
            hero = player->heroId;
            actor->as.combat.head.player.animationBank = (void *)HeroMeleeAnimationBanks[hero];
            player->bowHoldFrames = 0;
            return ACTOR_STATE_LEGOLAS_OVERDRAW;
        }
    }
    return -1;
}
