#include "lib/lr_button_indicators.h"
#include "actor.h"
#include "variables.h"

Actor *hackLib_createLrButtonIndicator(bool mainPool, enum LrButtonIndicatorIndex index, s32 x,
                                       s32 y)
{
    Actor *actor = mainPool ? actor_allocMain() : actor_allocOverlay();

    sprite_setPosition(actor, x << 16, y << 16);
    actor->flags.p = ACTOR_FLAG_RENDER | ACTOR_FLAG_TICK_ALWAYS | ACTOR_FLAG_ANIMATION_PLAYING;
    actor->recordType = index;
    actor->oam.objMode = 0;
    actor->oam.colors256 = 0;
    actor->oam.paletteNumber = 1;
    actor->oam.bgPriority = 0;
    sprite_setAnimation(actor, &MenuLrButtonIndicatorAnimations[index]);
    return actor;
}

void hackLib_createLrButtonIndicatorsInMainPool(void)
{
    u8 i;

    for (i = 0; i < 2; i++)
    {
        hackLib_createLrButtonIndicator(TRUE, i, MenuLrButtonIndicatorPositions[i].x,
                                        MenuLrButtonIndicatorPositions[i].y);
    }
}
