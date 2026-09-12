#include "actor.h"
#include "game.h"
#include "player.h"
#include "scene.h"
#include "scene/gameplay.h"
#include "scene/shop.h"
#include "sfx.h"
#include "variables.h"

/**
 * B button on the shop menu: dismiss a pending Yes/No confirm and redraw the info
 * panel, or leave the shop back into the current mission.
 *
 * @romaddress 0x0801fe4c
 */
void scene_shop_onBack(void)
{
    if (SCENE_ARG_SHOP_STATE == SHOP_STATE_NONE)
    {
        game_requestSceneChangeWithArgs(SCENE_ID_GAMEPLAY, GAMEPLAY_MODE_RESUME,
                                        g_CurrentMissionId);
    }
    else
    {
        SCENE_ARG_SHOP_STATE = SHOP_STATE_NONE;
        SCENE_ARG_SHOP_CONFIRM_SEL = SHOP_CONFIRM_YES;
        scene_shop_drawInfoPanel(g_ShopCursor->actionState, ACTIVE_PLAYER_INDEX);
    }
    sfx_play(SFX_MENU_BACK);
}
