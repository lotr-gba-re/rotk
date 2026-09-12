#include "actor.h"
#include "menu.h"
#include "variables.h"

/**
 * Create the animated L/R button indicators at the top corners of the character screens.
 * The actors are allocated in the overlay pool.
 *
 * @romaddress 0x08027af0
 */
void menu_createLrButtonIndicators(void)
{
    u8 i;

    for (i = 0; i < 2; i++)
    {
        Actor *actor = actor_allocOverlay();

        sprite_setPosition(actor, MenuLrButtonIndicatorPositions[i].x << 16,
                           MenuLrButtonIndicatorPositions[i].y << 16);
        actor->flags.p = ACTOR_FLAG_RENDER | ACTOR_FLAG_TICK_ALWAYS | ACTOR_FLAG_ANIMATION_PLAYING;
        actor->recordType = i;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.paletteNumber = 1;
        actor->oam.bgPriority = 0;
        if (i == 0)
        {
            sprite_setAnimation(actor, &MenuLrButtonIndicatorAnimations[0]);
        }
        else
        {
            sprite_setAnimation(actor, &MenuLrButtonIndicatorAnimations[1]);
        }
    }
}
