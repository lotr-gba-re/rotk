#include "libc.h"
#include "scene.h"
#include "scene/artifacts.h"
#include "scene/backpack.h"
#include "scene/bonusMapMenu.h"
#include "scene/characterStats.h"
#include "scene/copyright.h"
#include "scene/credits.h"
#include "scene/cutscene.h"
#include "scene/gameplay.h"
#include "scene/gcnLinkMenu.h"
#include "scene/griptoniteLogo.h"
#include "scene/inGameDialog.h"
#include "scene/introLogo.h"
#include "scene/languageSelect.h"
#include "scene/mainMenu.h"
#include "scene/multiplayerLevelSelect.h"
#include "scene/newGame.h"
#include "scene/optionsMenu.h"
#include "scene/pauseMenu.h"
#include "scene/saveLoadMenu.h"
#include "scene/shop.h"
#include "scene/skillTree.h"
#include "scene/unknown16.h"
#include "scene/unused0c.h"
#include "scene/victory.h"

// clang-format off

/** @romaddress 0x0806c828 */
const SceneEntry SceneHandlers[26] = {
    // [0x00]
    { .enter = scene_noOp,
      .tick = scene_noOp,
      .exit = scene_noOp },

    // [0x01]
    { .enter = scene_gameplay_enter,
      .tick = scene_gameplay_tick,
      .exit = scene_gameplay_exit },

    // [0x02]
    { .enter = scene_copyright_enter,
      .tick = scene_copyright_tick,
      .exit = scene_copyright_exit },

    // [0x03]
    { .enter = scene_introLogo_enter,
      .tick = scene_introLogo_tick,
      .exit = scene_introLogo_exit },

    // [0x04]
    { .enter = scene_griptoniteLogo_enter,
      .tick = scene_griptoniteLogo_tick,
      .exit = scene_griptoniteLogo_exit },

    // [0x05]
    { .enter = scene_mainMenu_enter,
      .tick = scene_mainMenu_tick,
      .exit = scene_mainMenu_exit },

    // [0x06]
    { .enter = scene_optionsMenu_enter,
      .tick = scene_optionsMenu_tick,
      .exit = scene_optionsMenu_exit },

    // [0x07]
    { .enter = scene_cutscene_enter,
      .tick = scene_cutscene_tick,
      .exit = scene_cutscene_exit },

    // [0x08]
    { .enter = scene_newGame_enter,
      .tick = scene_newGame_tick,
      .exit = scene_newGame_exit },

    // [0x09]
    { .enter = scene_saveLoadMenu_enter,
      .tick = scene_saveLoadMenu_tick,
      .exit = scene_saveLoadMenu_exit },

    // [0x0a]
    { .enter = scene_backpack_enter,
      .tick = scene_backpack_tick,
      .exit = scene_backpack_exit },

    // [0x0b]
    { .enter = scene_multiplayerLevelSelect_enter,
      .tick = scene_multiplayerLevelSelect_tick,
      .exit = scene_multiplayerLevelSelect_exit },

    // [0x0c]
    { .enter = scene_unused0c_enter,
      .tick = scene_unused0c_tick,
      .exit = scene_unused0c_exit },

    // [0x0d]
    { .enter = scene_artifacts_enter,
      .tick = scene_artifacts_tick,
      .exit = scene_artifacts_exit },

    // [0x0e]
    { .enter = scene_characterStats_enter,
      .tick = scene_characterStats_tick,
      .exit = scene_characterStats_exit },

    // [0x0f]
    { .enter = scene_skillTree_enter,
      .tick = scene_skillTree_tick,
      .exit = scene_skillTree_exit },

    // [0x10]
    { .enter = scene_pauseMenu_enter,
      .tick = scene_pauseMenu_tick,
      .exit = scene_pauseMenu_exit },

    // [0x11]
    { .enter = scene_inGameDialog_enter,
      .tick = scene_inGameDialog_tick,
      .exit = scene_inGameDialog_exit },

    // [0x12]
    { .enter = scene_languageSelect_enter,
      .tick = scene_languageSelect_tick,
      .exit = scene_languageSelect_exit },

    // [0x13]
    { .enter = scene_credits_enter,
      .tick = scene_credits_tick,
      .exit = scene_credits_exit },

    // [0x14]
    { .enter = scene_shop_enter,
      .tick = scene_shop_tick,
      .exit = scene_shop_exit },

    // [0x15]
    { .enter = NULL,
      .tick = NULL,
      .exit = NULL },

    // [0x16]
    { .enter = scene_unknown16_enter,
      .tick = scene_unknown16_tick,
      .exit = scene_unknown16_exit },

    // [0x17]
    { .enter = scene_gcnLinkMenu_enter,
      .tick = scene_gcnLinkMenu_tick,
      .exit = scene_gcnLinkMenu_exit },

    // [0x18]
    { .enter = scene_bonusMapMenu_enter,
      .tick = scene_bonusMapMenu_tick,
      .exit = scene_bonusMapMenu_exit },

    // [0x19]
    { .enter = scene_victory_enter,
      .tick = scene_victory_tick,
      .exit = scene_victory_exit },
};
// clang-format on
