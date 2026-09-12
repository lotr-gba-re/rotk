// Turn the bonus map selection menu into a chapter select that allows picking other hero's paths
// as bonus maps. Paths get unlocked once the game has been beaten with the respective hero.
// The maps are still launched in bonus map mode, meaning that the campaign progress is not
// overwritten when saving.

#include "actor.h"
#include "font.h"
#include "game.h"
#include "gba_io.h"
#include "gfx.h"
#include "input.h"
#include "lib/lr_button_indicators.h"
#include "menu.h"
#include "mission.h"
#include "save.h"
#include "scene.h"
#include "scene/bonusMapMenu.h"
#include "scene/gameplay.h"
#include "sfx.h"
#include "sprite.h"
#include "text.h"
#include "variables.h"

/**
 * Chapter-list scroll offset: scene args slot unused by the stock scene (zeroed on scene
 * entry by game_requestSceneChange/game_requestSceneChangeWithArgs).
 */
#define SCENE_ARG_CHAPTER_SELECT_SCROLL g_SceneCurrent.args[4]

// SCENE_ARG_BONUS_MAP_CURSOR packs the grid entry (low nibble, <= 7) and the current
// page (bits 8+). Scene-entry zeroing of args[1] lands on page 0, entry 0.
#define ENTRY_INDEX (SCENE_ARG_BONUS_MAP_CURSOR & 0xf)
#define PAGE_INDEX ((SCENE_ARG_BONUS_MAP_CURSOR >> 8) & 0xff)

#define GRID_COLS 4
#define PAGE_COUNT 5
#define VISIBLE_ROWS 5

// Panel rows cleared on redraw: label row 3 through the last list row (y 0x50).
// Difficulty rows (y 0x68+) are below the cleared span. The title (y 4, 11px font)
// spans rows 0-1.
#define PANEL_CLEAR_ROW 3
#define PANEL_CLEAR_HEIGHT 8

// Sub-entry list span: the ^ indicator row, VISIBLE_ROWS chapter rows, the v indicator
// row (rows 4..10).
#define PANEL_LIST_ROW 4
#define PANEL_LIST_HEIGHT 7

// Chapter list geometry: list row 0 is panel row 5; each row owns 0x14 charbase tiles.
// The ^ indicator sits on row -1, the v indicator one row past the last visible chapter.
#define LIST_ROW_Y(row) (0x28 + (row) * 8)
#define LIST_ROW_TILE(row) (0x92 + (row) * 0x14)

// Grid entry names: a region's own name from the region-name text block, and the bonus
// page's map names from the bonus block.
#define REGION_NAME(region) (TEXT_ID_REGION_NAME_BASE + (region))
#define BONUS_MAP_NAME(bonusMap) (TEXT_ID_BONUS_MISSION_NAME_BASE + (bonusMap))

// OBJ palette bank of grid slot 0; the stock select scenes number their icons from here.
#define FIRST_PALETTE 3

// Region unlock conditions: the credits scene sets the finished-as bit of the played hero
// (scene_credits_enter; Sam and Smeagol count as Frodo). Aragorn's path opens for any
// of his fellowship branch's three heroes.
#define UNLOCK_ARAGORNS_PATH                                                                       \
    (UNLOCK_FLAG_FINISHED_AS_ARAGORN | UNLOCK_FLAG_FINISHED_AS_LEGOLAS |                           \
     UNLOCK_FLAG_FINISHED_AS_GIMLI)
#define UNLOCK_FRODOS_PATH UNLOCK_FLAG_FINISHED_AS_FRODO
#define UNLOCK_GANDALFS_PATH UNLOCK_FLAG_FINISHED_AS_GANDALF
#define UNLOCK_EOWYNS_PATH UNLOCK_FLAG_FINISHED_AS_EOWYN

/**
 * One chapter-list row: the mission to launch, the variant (g_MissionVariant) to load it at,
 * and the spawn point (g_MissionSpawnPoint) to enter it at. Mirrors a legit exit-warp
 * transition: the warp writes only the spawn point (both player slots) while the variant byte
 * carries over from the previous mission's scripts.
 */
struct ChapterSelectMission
{
    u16 missionId;
    u8 variant;
    u8 spawnPoint;
};

// Grid icons: the animation record each MP level select entry embeds, named after what it
// depicts. setupSprites gives every slot its own OBJ palette bank, so a page may repeat an icon.
#define ICON_SKULLS (&MultiplayerLevelSelectEntries[0].animation)       // Paths of the Dead
#define ICON_SHIP (&MultiplayerLevelSelectEntries[1].animation)         // Pelennor Fields
#define ICON_MINAS_TIRITH (&MultiplayerLevelSelectEntries[2].animation) // Minas Tirith
#define ICON_MT_DOOM (&MultiplayerLevelSelectEntries[3].animation)      // Mt Doom
#define ICON_MORIA (&MultiplayerLevelSelectEntries[4].animation)        // Moria
#define ICON_HILL (&MultiplayerLevelSelectEntries[5].animation)         // Weathertop
#define ICON_HELMS_DEEP (&MultiplayerLevelSelectEntries[6].animation)   // Helm's Deep
#define ICON_TREE (&MultiplayerLevelSelectEntries[7].animation)         // Fangorn Forest

/**
 * One grid entry of a page: a chapter list under the name of the region it plays in, or of
 * the bonus map it belongs to. unlockMask locks the entry until those bits are set
 * (0 = always unlocked).
 */
struct ChapterSelectRegion
{
    const struct ChapterSelectMission *missions;
    u8 missionCount;
    u16 nameTextId;              // REGION_NAME, or a bonus map's own name
    const SpriteAnimation *icon; // grid icon
    u32 unlockMask;              // tested against SaveGlobals.unlockFlags; 0 = always unlocked
};

/** One L/R-switchable screen of the menu. */
struct ChapterSelectPage
{
    const struct ChapterSelectRegion *regions;
    u8 entryCount;
    u16 nameTextId; // page heading: a TEXT_ID_*_S_PATH name, or TEXT_ID_BONUS_MAPS
};

// Aragorn's path
static const struct ChapterSelectMission missionsAragornIsengard[] = {
    {MISSION_ISENGARD_FLOODED_FIELD, 0, 0},  //
    {MISSION_ISENGARD_TOWER_BASE, 0, 0},     //
    {MISSION_ISENGARD_ORTHANC_TUNNEL, 0, 0}, //
    {MISSION_ISENGARD_URUK_HATCHERY, 0, 0},  //
    {MISSION_ISENGARD_TOWER_DOOR, 0, 0},     //
    // {MISSION_ISENGARD_TOWER_LEVELS, 0, 0},   // very short
    // {MISSION_ISENGARD_TOWER_BASE, 1, 1},   // Saruman fall
    {MISSION_ISENGARD_CAMPSITE, 0, 0}, //
};
static const struct ChapterSelectMission missionsAragornRohan[] = {
    {MISSION_ROHAN_PLAINS, 0, 0},           //
    {MISSION_ROHAN_SNOWBOURNE_TRAIL, 0, 0}, //
    {MISSION_ROHAN_HILLS, 0, 0},            //
    {MISSION_ROHAN_CAMPSITE, 0, 0},         //
};

static const struct ChapterSelectMission missionsAragornDunharrow[] = {
    {MISSION_DUNHARROW_HILLS, 0, 0},    //
    {MISSION_DUNHARROW_PLATEAU, 0, 0},  //
    {MISSION_DUNHARROW_CAMPS, 0, 0},    //
    {MISSION_DUNHARROW_CAMPSITE, 0, 0}, //
};

static const struct ChapterSelectMission missionsAragornDimholtPathsOfTheDead[] = {
    {MISSION_DIMHOLT_ROAD, 0, 0},                  //
    {MISSION_DIMHOLT_DWIMERBERG_BASE, 0, 0},       //
    {MISSION_DIMHOLT_DWIMERBERG_MT, 0, 0},         //
    {MISSION_DIMHOLT_CAMPSITE, 0, 0},              //
    {MISSION_DIMHOLT_GLEN, 0, 0},                  //
    {MISSION_PATHS_OF_THE_DEAD_GREY_VAPORS, 0, 0}, //
    {MISSION_PATHS_OF_THE_DEAD_DARK_PATHS, 0, 0},  //
    {MISSION_PATHS_OF_THE_DEAD_BLACK_CHASM, 0, 0}, //
    {MISSION_PATHS_OF_THE_DEAD_CAMPSITE, 0, 0},    //
};
static const struct ChapterSelectMission missionsAragornWhiteMountains[] = {
    {MISSION_WHITE_MOUNTAINS_FOOTHILLS, 0, 0},   //
    {MISSION_WHITE_MOUNTAINS_EDGE, 0, 0},        //
    {MISSION_WHITE_MOUNTAINS_SE_MTS_EDGE, 0, 0}, //
    {MISSION_WHITE_MOUNTAINS_CAMPSITE, 0, 0},    //
};
static const struct ChapterSelectMission missionsAragornAnduinRiver[] = {
    {MISSION_ANDUIN_RIVER_CORSAIR_CAMP, 0, 0},  //
    {MISSION_ANDUIN_RIVER_CORSAIR_CHASE, 0, 0}, //
    {MISSION_ANDUIN_BANKS, 0, 0},               //
    {MISSION_ANDUIN_RIVER_CAMPSITE, 0, 0},      //
    {MISSION_ANDUIN_DOCKS, 0, 0},               //
    {MISSION_CORSAIR_FLAGSHIP_BELOW, 0, 0},     //
    {MISSION_CORSAIR_FLAGSHIP_ABOVE, 0, 0},     //
};
static const struct ChapterSelectMission missionsAragornPelennor[] = {
    {MISSION_MINAS_TIRITH_DOCKS, 0, 0},             //
    {MISSION_PELENNOR_FIELDS_OUTPOST, 0, 0},        //
    {MISSION_PELENNOR_FIELDS_FOOTPATH, 0, 0},       //
    {MISSION_PELENNOR_BEACON_MINAS_ENTRANCE, 0, 0}, //
};
static const struct ChapterSelectMission missionsAragornIthilienMordor[] = {
    {MISSION_ITHILIEN_NORTH, 0, 0}, //
    // {MISSION_ITHILIEN_WATERFALL, 0, 0}, // side area
    {MISSION_ITHILIEN_BLACK_ROAD, 0, 0}, //
    {MISSION_ITHILIEN_BLACK_GATE, 0, 0}, // Start of mission
    {MISSION_ITHILIEN_BLACK_GATE, 1, 1}, // Mouth of Sauron fight
};
static const struct ChapterSelectRegion regionsAragorn[] = {
    {missionsAragornIsengard, ARRAY_COUNT(missionsAragornIsengard), REGION_NAME(REGION_ISENGARD),
     ICON_MORIA, UNLOCK_ARAGORNS_PATH},
    {missionsAragornRohan, ARRAY_COUNT(missionsAragornRohan), REGION_NAME(REGION_ROHAN),
     ICON_HELMS_DEEP, UNLOCK_ARAGORNS_PATH},
    {missionsAragornDunharrow, ARRAY_COUNT(missionsAragornDunharrow), REGION_NAME(REGION_DUNHARROW),
     ICON_TREE, UNLOCK_ARAGORNS_PATH},
    {missionsAragornDimholtPathsOfTheDead, ARRAY_COUNT(missionsAragornDimholtPathsOfTheDead),
     REGION_NAME(REGION_DIMHOLT), ICON_SKULLS, UNLOCK_ARAGORNS_PATH},
    {missionsAragornWhiteMountains, ARRAY_COUNT(missionsAragornWhiteMountains),
     REGION_NAME(REGION_WHITE_MOUNTAINS), ICON_HILL, UNLOCK_ARAGORNS_PATH},
    {missionsAragornAnduinRiver, ARRAY_COUNT(missionsAragornAnduinRiver),
     REGION_NAME(REGION_ANDUIN_RIVER), ICON_SHIP, UNLOCK_ARAGORNS_PATH},
    {missionsAragornPelennor, ARRAY_COUNT(missionsAragornPelennor), REGION_NAME(REGION_DOCKS),
     ICON_MINAS_TIRITH, UNLOCK_ARAGORNS_PATH},
    {missionsAragornIthilienMordor, ARRAY_COUNT(missionsAragornIthilienMordor),
     REGION_NAME(REGION_ITHILIEN), ICON_MT_DOOM, UNLOCK_ARAGORNS_PATH},
};

// Frodo's/Sam's path
static const struct ChapterSelectMission missionsFrodoMorgul[] = {
    {MISSION_VALE_OF_MORGUL, 6, 0},    //
    {MISSION_MORGUL_CROSSROADS, 6, 0}, //
    {MISSION_MINAS_MORGUL, 6, 0},      //
    {MISSION_MORGUL_GATE, 6, 0},       //
    // {MISSION_MORGUL_SECRET_CAVE, 6, 0}, // side area, acces by walking perfectly on train tracks
    // {MISSION_MORGUL_BRIDGE, 7, 1},   // return from side area
    {MISSION_MORGUL_BRIDGE, 6, 1},   //
    {MISSION_MORGUL_VALLEY, 6, 0},   //
    {MISSION_MORGUL_CAMPSITE, 6, 0}, //
};
static const struct ChapterSelectMission missionsFrodoCirithUngol[] = {
    {MISSION_CIRITH_UNGOL_STAIRS, 6, 0}, //
    {MISSION_CIRITH_UNGOL_LEDGE, 6, 0},  //
    // {MISSION_CIRITH_UNGOL_SUMMIT, 6, 1}, // side area with loot
    // {MISSION_CIRITH_UNGOL_LEDGE, 7, 1},  // return from side area
    // {MISSION_CIRITH_UNGOL_SUMMIT, 7, 1}, // side area without loot
    {MISSION_CIRITH_UNGOL_PASS, 6, 0}, // sam leaves
};
static const struct ChapterSelectMission missionsFrodoShelobsLair[] = {
    {MISSION_SHELOBS_LAIR_TUNNEL_MOUTH, 6, 0}, //
    {MISSION_SHELOBS_LAIR_CAVES, 6, 0},        //
    {MISSION_SHELOBS_LAIR, 6, 0},              //
    {MISSION_SHELOBS_LAIR_CAVES, 7, 1},        // running back
    {MISSION_SHELOBS_LAIR_TUNNEL_MOUTH, 7, 1}, // running back
};
static const struct ChapterSelectMission missionsFrodoCirithUngol2[] = {
    {MISSION_CIRITH_UNGOL_PASS, 7, 1},   //
    {MISSION_CIRITH_UNGOL_SUMMIT, 7, 0}, //
    // {MISSION_SHELOBS_LAIR_TUNNEL_MOUTH, 7, 2}, // side area
    // {MISSION_CIRITH_UNGOL_SUMMIT, 7, 2},       // exit from side area
    {MISSION_CIRITH_UNGOL_AMBUSH, 6, 0}, //
};
static const struct ChapterSelectMission missionsFrodoOrcTowerSam[] = {
    {MISSION_CIRITH_UNGOL_AMBUSH, 7, 1},   // Shelob fight as Sam
    {MISSION_CIRITH_UNGOL_PARAPET, 7, 0},  //
    {MISSION_ORC_TOWER, 7, 0},             //
    {MISSION_ORC_TOWER_TOP_ROOM, 7, 0},    //
    {MISSION_CIRITH_UNGOL_CAMPSITE, 7, 0}, // back to Frodo
};
static const struct ChapterSelectMission missionsFrodoGorgoroth[] = {
    {MISSION_GORGOROTH_ORC_ROAD, 7, 0}, //
    {MISSION_GORGOROTH_PLAINS, 6, 0},   //
    {MISSION_GORGOROTH_HILLS, 6, 0},    //
    {MISSION_GORGOROTH_CAMPSITE, 6, 0}, //
};
static const struct ChapterSelectMission missionsFrodoMtDoom[] = {
    {MISSION_FOOT_OF_MT_DOOM, 6, 0},               //
    {MISSION_SLOPES_OF_MT_DOOM, 6, 0},             //
    {MISSION_MT_DOOM_SAMMATH_NAUR, 6, 0},          //
    {MISSION_MT_DOOM_CAMPSITE, 6, 0},              //
    {MISSION_CRACK_OF_DOOM_TUNNEL, 6, 0},          //
    {MISSION_CRACK_OF_DOOM, 6, 0},                 //
    {MISSION_CRACK_OF_DOOM_EDGE_OF_VOLCANO, 6, 0}, //
    {MISSION_CRACK_OF_DOOM_EDGE_OF_VOLCANO, 7, 1}, // Gollum fight
};
static const struct ChapterSelectRegion regionsFrodo[] = {
    {missionsFrodoMorgul, ARRAY_COUNT(missionsFrodoMorgul), REGION_NAME(REGION_MORGUL),
     ICON_MINAS_TIRITH, UNLOCK_FRODOS_PATH}, // Minas Morgul
    {missionsFrodoCirithUngol, ARRAY_COUNT(missionsFrodoCirithUngol),
     REGION_NAME(REGION_CIRITH_UNGOL), ICON_MORIA, UNLOCK_FRODOS_PATH}, // the pass
    {missionsFrodoShelobsLair, ARRAY_COUNT(missionsFrodoShelobsLair),
     REGION_NAME(REGION_SHELOBS_LAIR), ICON_SKULLS, UNLOCK_FRODOS_PATH}, // dark tunnels
    {missionsFrodoCirithUngol2, ARRAY_COUNT(missionsFrodoCirithUngol2),
     REGION_NAME(REGION_CIRITH_UNGOL), ICON_HILL, UNLOCK_FRODOS_PATH},
    {missionsFrodoOrcTowerSam, ARRAY_COUNT(missionsFrodoOrcTowerSam), REGION_NAME(REGION_ORC_TOWER),
     ICON_HELMS_DEEP, UNLOCK_FRODOS_PATH}, // the tower
    {missionsFrodoGorgoroth, ARRAY_COUNT(missionsFrodoGorgoroth), REGION_NAME(REGION_GORGOROTH),
     ICON_SHIP, UNLOCK_FRODOS_PATH}, // the plains
    {missionsFrodoMtDoom, ARRAY_COUNT(missionsFrodoMtDoom), REGION_NAME(REGION_MT_DOOM),
     ICON_MT_DOOM, UNLOCK_FRODOS_PATH},
};

// Gandalf's path
static const struct ChapterSelectMission missionsGandalfIsengard[] = {
    {MISSION_ISENGARD_FLOODED_FIELD, 2, 0}, //
    {MISSION_ISENGARD_TOWER_BASE, 2, 0},    //
    {MISSION_ISENGARD_TOWER_LEVELS, 2, 1},  //
    {MISSION_ISENGARD_ORTHANC, 2, 0},       //
    // {MISSION_ISENGARD_TOWER_BASE, 3, 1},    // Saruman fall
    {MISSION_ISENGARD_CAMPSITE, 2, 0}, //
};
static const struct ChapterSelectMission missionsGandalfRohan[] = {
    {MISSION_ROHAN_PLAINS, 2, 0},           //
    {MISSION_ROHAN_SNOWBOURNE_TRAIL, 2, 0}, //
    {MISSION_ROHAN_HILLS, 2, 0},            //
    {MISSION_ROHAN_CAMPSITE, 2, 0},         //
};
static const struct ChapterSelectMission missionsGandalfWhiteMountains[] = {
    {MISSION_WHITE_MOUNTAINS_HIDDEN_TRAIL, 2, 0}, //
    {MISSION_WHITE_MOUNTAINS_NARROW_PASS, 2, 0},  //
    {MISSION_WHITE_MOUNTAINS_APPROACH, 2, 0},     //
    {MISSION_WHITE_MOUNTAINS_CAMPSITE, 2, 0},     //
};
static const struct ChapterSelectMission missionsGandalfPelennorBeacon[] = {
    {MISSION_PELENNOR_BEACON_MINAS_ENTRANCE, 3, 0}, // 1st time
    {MISSION_MINAS_TIRITH_BATTLEMENTS, 3, 0},       //
    {MISSION_PELENNOR_BEACON_MINAS_ENTRANCE, 4, 1}, // 2nd time after leaving
    {MISSION_PELENNOR_BATTLE_WEST_FIELDS, 2, 0},    //
    {MISSION_PELENNOR_FIELDS_MT_MINDOLLUIN, 2, 0},  //
    {MISSION_PELENNOR_FIELDS_FORK, 2, 0},           //
    {MISSION_PELENNOR_BEACON_EAST_BEACON, 2, 0},    //
    {MISSION_PELENNOR_FIELDS_FORK, 6, 2},           // revisit after east beacon
    {MISSION_PELENNOR_BEACON_WEST_BEACON, 6, 0},    //
    {MISSION_PELENNOR_BATTLE_NORTH_FIELDS, 2, 0},   //
    {MISSION_PELENNOR_FIELDS_CAMPSITE, 2, 0},       //
    {MISSION_PELENNOR_BEACON_MINAS_ENTRANCE, 2, 0}, // 3rd time
};
static const struct ChapterSelectMission missionsGandalfMinasTirith[] = {
    {MISSION_MINAS_TIRITH_BATTLEMENTS, 2, 0},                //
    {MISSION_MINAS_TIRITH_TIERS_GATES, 2, 0},                //
    {MISSION_MINAS_TIRITH_TIERS_TIER_THREE, 2, 0},           //
    {MISSION_MINAS_TIRITH_TIERS_TIER_FOUR, 2, 0},            //
    {MISSION_MINAS_TIRITH_TIERS_STREETS, 2, 0},              //
    {MISSION_MINAS_TIRITH_BATTLEMENTS_COURT_OF_KINGS, 3, 1}, //
    {MISSION_MINAS_TIRITH_BATTLEMENTS_STEWARDS_TOMB, 2, 0},  //
    // {MISSION_DENETHOR_FALL_BALCONY, 2, 0},  // Denethor fall
    {MISSION_MINAS_TIRITH_BATTLEMENTS_COURT_OF_KINGS, 2, 0}, // Campsite-like
};
static const struct ChapterSelectMission missionsGandalfIthilien[] = {
    {MISSION_ITHILIEN_NORTH, 2, 0},      //
    {MISSION_ITHILIEN_BLACK_ROAD, 2, 0}, //
    {MISSION_ITHILIEN_BLACK_GATE, 2, 0}, //
};
static const struct ChapterSelectRegion regionsGandalf[] = {
    {missionsGandalfIsengard, ARRAY_COUNT(missionsGandalfIsengard), REGION_NAME(REGION_ISENGARD),
     ICON_MORIA, UNLOCK_GANDALFS_PATH},
    {missionsGandalfRohan, ARRAY_COUNT(missionsGandalfRohan), REGION_NAME(REGION_ROHAN),
     ICON_HELMS_DEEP, UNLOCK_GANDALFS_PATH},
    {missionsGandalfWhiteMountains, ARRAY_COUNT(missionsGandalfWhiteMountains),
     REGION_NAME(REGION_WHITE_MOUNTAINS), ICON_HILL, UNLOCK_GANDALFS_PATH},
    {missionsGandalfPelennorBeacon, ARRAY_COUNT(missionsGandalfPelennorBeacon),
     REGION_NAME(REGION_PELENNOR_BEACON), ICON_SHIP, UNLOCK_GANDALFS_PATH},
    {missionsGandalfMinasTirith, ARRAY_COUNT(missionsGandalfMinasTirith),
     REGION_NAME(REGION_MINAS_TIRITH_TIERS), ICON_MINAS_TIRITH, UNLOCK_GANDALFS_PATH},
    {missionsGandalfIthilien, ARRAY_COUNT(missionsGandalfIthilien), REGION_NAME(REGION_ITHILIEN),
     ICON_MT_DOOM, UNLOCK_GANDALFS_PATH},
};

// Eowyn's path
static const struct ChapterSelectMission missionsEowynRohan[] = {
    {MISSION_ROHAN_EDORAS_FIELDS, 4, 0},     //
    {MISSION_ROHAN_GAP, 4, 0},               //
    {MISSION_ROHAN_HILLS, 4, 0},             //
    {MISSION_ROHAN_PLAINS, 4, 0},            //
    {MISSION_ROHAN_SNOWBOURNE_TRAIL, 4, 0},  //
    {MISSION_ROHAN_DEEPING_OUTSKIRTS, 4, 0}, //
    {MISSION_ROHAN_CAMPSITE, 4, 0},          //
};
static const struct ChapterSelectMission missionsEowynDunharrow[] = {
    {MISSION_DUNHARROW_HILLS, 4, 0},          //
    {MISSION_DUNHARROW_PLATEAU, 4, 0},        //
    {MISSION_DUNHARROW_CAMPS, 4, 0},          //
    {MISSION_DUNHARROW_RIVER_CROSSING, 4, 0}, //
    {MISSION_DUNHARROW_HOLLOW, 4, 0},         //
    {MISSION_DUNHARROW_CAMPSITE, 4, 0},       //
};
static const struct ChapterSelectMission missionsEowynWhiteMountains[] = {
    {MISSION_WHITE_MOUNTAINS_FOOTHILLS, 4, 0},   //
    {MISSION_WHITE_MOUNTAINS_STORM, 4, 0},       //
    {MISSION_WHITE_MOUNTAINS_ICE_CAVES, 4, 0},   //
    {MISSION_WHITE_MOUNTAINS_EDGE, 4, 0},        //
    {MISSION_WHITE_MOUNTAINS_SE_MTS_EDGE, 4, 0}, //
    {MISSION_WHITE_MOUNTAINS_CAMPSITE, 4, 0},    //
};
static const struct ChapterSelectMission missionsEowynPelennor[] = {
    {MISSION_PELENNOR_BATTLE_EDGE, 4, 0},          //
    {MISSION_PELENNOR_FIELDS_HARADRIM_CAMP, 5, 0}, //
    // {MISSION_PELENNOR_BATTLE_HIDDEN_PLAINS, 4, 0},    // side area, enterable from bottom right
    // of Haradrim Camp {MISSION_PELENNOR_FIELDS_MUMAKIL_CAMP, 4, 0},     // when entered from
    // Hidden Plains
    {MISSION_PELENNOR_FIELDS_MUMAKIL_CAMP, 5, 0},     // when entered from Haradrim Camp
    {MISSION_PELENNOR_FIELDS_CAMPSITE, 4, 0},         //
    {MISSION_PELENNOR_FIELDS_THEODENS_CHARGE, 4, 0},  //
    {MISSION_PELENNOR_FIELDS_MUMAKIL_ATTACK, 4, 0},   //
    {MISSION_PELENNOR_BATTLE_FALL_OF_ROHIRRIM, 4, 0}, //
    {MISSION_PELENNOR_BATTLE_WITCH_KING, 4, 0},       //
};
static const struct ChapterSelectRegion regionsEowyn[] = {
    {missionsEowynRohan, ARRAY_COUNT(missionsEowynRohan), REGION_NAME(REGION_ROHAN),
     ICON_HELMS_DEEP, UNLOCK_EOWYNS_PATH},
    {missionsEowynDunharrow, ARRAY_COUNT(missionsEowynDunharrow), REGION_NAME(REGION_DUNHARROW),
     ICON_TREE, UNLOCK_EOWYNS_PATH}, // the hill refuge
    {missionsEowynWhiteMountains, ARRAY_COUNT(missionsEowynWhiteMountains),
     REGION_NAME(REGION_WHITE_MOUNTAINS), ICON_HILL, UNLOCK_EOWYNS_PATH}, // the ice caves
    {missionsEowynPelennor, ARRAY_COUNT(missionsEowynPelennor), REGION_NAME(REGION_PELENNOR_BATTLE),
     ICON_MINAS_TIRITH, UNLOCK_EOWYNS_PATH}, // Pelennor
};

static const struct ChapterSelectMission missionsBonusMoria[] = {
    {MISSION_MORIA_HALL_TOP, 0, 0},        //
    {MISSION_MORIA_LONG_STAIRS, 0, 0},     //
    {MISSION_MORIA_TREASURE_HOARDS, 0, 0}, //
    {MISSION_MORIA_HALL, 0, 0},            //
    {MISSION_MORIA_BALINS_TOMB, 0, 0},     //
};
static const struct ChapterSelectMission missionsBonusWeathertop[] = {
    {MISSION_WEATHERTOP_TOWER, 0, 0}, //
};
static const struct ChapterSelectMission missionsBonusHelmsDeep[] = {
    {MISSION_HELMS_DEEP_FORTRESS, 0, 0}, //
    {MISSION_HELMS_DEEP_VALLEY, 0, 0},   //
};
static const struct ChapterSelectMission missionsBonusFangorn[] = {
    {MISSION_FANGORN_CLEARING, 0, 0}, //
    {MISSION_FANGORN_COPSE, 0, 0},    //
    {MISSION_FANGORN_ENTWASH, 0, 0},  //
};
static const struct ChapterSelectRegion regionsBonusMaps[] = {
    {missionsBonusMoria, ARRAY_COUNT(missionsBonusMoria), BONUS_MAP_NAME(BONUS_MAP_MORIA),
     ICON_MORIA, UNLOCK_FLAG_BONUS_MAP_MORIA},
    {missionsBonusWeathertop, ARRAY_COUNT(missionsBonusWeathertop),
     BONUS_MAP_NAME(BONUS_MAP_WEATHERTOP), ICON_HILL, UNLOCK_FLAG_BONUS_MAP_WEATHERTOP},
    {missionsBonusHelmsDeep, ARRAY_COUNT(missionsBonusHelmsDeep),
     BONUS_MAP_NAME(BONUS_MAP_HELMS_DEEP), ICON_HELMS_DEEP, UNLOCK_FLAG_BONUS_MAP_HELMS_DEEP},
    {missionsBonusFangorn, ARRAY_COUNT(missionsBonusFangorn), BONUS_MAP_NAME(BONUS_MAP_FANGORN),
     ICON_TREE, UNLOCK_FLAG_BONUS_MAP_FANGORN},
};

/** The 5 pages in L/R order. */
static const struct ChapterSelectPage ChapterSelectPages[PAGE_COUNT] = {
    {regionsBonusMaps, ARRAY_COUNT(regionsBonusMaps), TEXT_ID_BONUS_MAPS},
    {regionsAragorn, ARRAY_COUNT(regionsAragorn), TEXT_ID_ARAGORNS_PATH},
    {regionsFrodo, ARRAY_COUNT(regionsFrodo), TEXT_ID_FRODOS_PATH},
    {regionsGandalf, ARRAY_COUNT(regionsGandalf), TEXT_ID_GANDALFS_PATH},
    {regionsEowyn, ARRAY_COUNT(regionsEowyn), TEXT_ID_EOWYNS_PATH},
};

static const struct ChapterSelectPage *currentPage(void)
{
    return &ChapterSelectPages[PAGE_INDEX];
}

/** The grid entry under the cursor. */
static const struct ChapterSelectRegion *currentEntry(void)
{
    return &currentPage()->regions[ENTRY_INDEX];
}

/** An entry with unlockMask 0 is always unlocked; otherwise the mask decides. */
static bool entryUnlocked(u32 entryIndex)
{
    u32 mask = currentPage()->regions[entryIndex].unlockMask;

    return mask == 0 || (g_SaveGlobals.d.unlockFlags.p & mask) != 0;
}

/** Grid-slot icon position (used <<16, 16.16 fixed point): two rows of four. */
static u32 slotX(u32 slot)
{
    return 0x03 + (slot & (GRID_COLS - 1)) * 0x24;
}

static u32 slotY(u32 slot)
{
    return slot < GRID_COLS ? 0x12 : 0x56;
}

/**
 * Chapter-list row count of the entry under the cursor (also the preview shown on
 * the grid). A locked entry has no rows, so neither chapters nor page indicators draw.
 */
static u32 rowCount(void)
{
    if (!entryUnlocked(ENTRY_INDEX))
    {
        return 0;
    }
    return currentEntry()->missionCount;
}

/**
 * Allocate the page's entry-icon sprites at their grid positions, an unlocked entry drawing
 * its own icon and a locked one the grey locked template. Each slot gets its own OBJ palette
 * bank, which is where sprite_setAnimation uploads the icon's palette.
 * Also (re)creates the L/R page indicators: Enter and SwitchPage both come here, and
 * SwitchPage's main-list flush frees the previous set.
 */
static void setupSprites(void)
{
    s32 i;
    Actor *actor;
    const struct ChapterSelectPage *page = currentPage();

    for (i = page->entryCount - 1; i >= 0; i--)
    {
        actor = actor_allocMain();
        sprite_setPosition(actor, slotX((u32)i) << 16, slotY((u32)i) << 16);
        actor->flags.p = ACTOR_FLAG_RENDER;
        actor->oam.objMode = 0;
        actor->oam.colors256 = 0;
        actor->oam.bgPriority = 1;
        actor->recordType = 0;
        actor->oam.paletteNumber = (u8)(FIRST_PALETTE + i);
        sprite_setAnimation(actor, entryUnlocked((u32)i) ? page->regions[i].icon
                                                         : &MultiplayerLevelSelectLockedAnimation);
    }

    hackLib_createLrButtonIndicatorsInMainPool();
}

/** Allocate and position the cursor sprite at the current entry's grid position. */
static void setupCursor(void)
{
    g_BonusMapCursorObj = actor_allocMain();
    Actor *cursor = g_BonusMapCursorObj;
    sprite_setPosition(cursor, slotX(ENTRY_INDEX) << 16, slotY(ENTRY_INDEX) << 16);
    // TODO: use the decoded flags view once actor flags bits 0/4 are decoded
    cursor->flags.p = 0x11;
    cursor->oam.objMode = 0;
    cursor->oam.colors256 = 0;
    cursor->oam.paletteNumber = 0;
    cursor->recordType = 0;
    sprite_setAnimation(cursor, &BonusMapCursorAnimation);
}

/**
 * Draw one sub-entry row on the side panel: the mission name of one chapter in the
 * selection's chapter list (a path region's, or a bonus map's sub-mission chain),
 * windowed by SCENE_ARG_CHAPTER_SELECT_SCROLL and highlighted while the chapter cursor
 * is active. Only called for rowIndex < rowCount(), so never for a locked entry.
 */
static void drawSubEntry(u8 rowIndex, u8 selectedIndex)
{
    s32 color = BONUS_MAP_TEXT_COLOR_NORMAL;
    s32 visibleRow;
    char *str;
    const struct ChapterSelectMission *missions = currentEntry()->missions;

    if (rowIndex == selectedIndex && SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_SUB_ENTRY_SELECT)
    {
        color = BONUS_MAP_TEXT_COLOR_SELECTED;
    }
    visibleRow = rowIndex - (s32)SCENE_ARG_CHAPTER_SELECT_SCROLL;
    if (visibleRow < 0 || visibleRow >= VISIBLE_ROWS)
    {
        return;
    }
    str = text_getString(TEXT_ID_MISSION_NAME_BASE + missions[rowIndex].missionId);
    font_setStyle(FONT_STYLE_8PX_B, color, 0);
    font_drawString(LIST_ROW_TILE(visibleRow), BONUS_MAP_PANEL_TEXT_X, LIST_ROW_Y(visibleRow), str,
                    1);
}

/**
 * Draw the chapter-list page indicators: "^" above the list when pages above, "v" below
 * the last visible chapter when pages below.
 */
static void drawScrollIndicators(void)
{
    u32 count;
    u32 scroll;
    u32 visibleCount;

    count = rowCount();
    scroll = SCENE_ARG_CHAPTER_SELECT_SCROLL;
    visibleCount = count - scroll;
    if (visibleCount > VISIBLE_ROWS)
    {
        visibleCount = VISIBLE_ROWS;
    }
    font_setStyle(FONT_STYLE_8PX_B, BONUS_MAP_TEXT_COLOR_NORMAL, 0);
    if (scroll > 0)
    {
        font_drawString(LIST_ROW_TILE(-1), BONUS_MAP_PANEL_TEXT_X, LIST_ROW_Y(-1), "^", 1);
    }
    if (scroll + VISIBLE_ROWS < count)
    {
        font_drawString(LIST_ROW_TILE(visibleCount), BONUS_MAP_PANEL_TEXT_X,
                        LIST_ROW_Y(visibleCount), "v", 1);
    }
}

/**
 * Draw the panel heading: the selection under the grid cursor (the entry's map name,
 * or "Locked!"). The page name is the title (drawTitle).
 */
static void drawEntryLabel(void)
{
    u16 textId;
    char *str;

    if (!entryUnlocked(ENTRY_INDEX))
    {
        textId = TEXT_ID_LOCKED_106;
    }
    else
    {
        textId = currentEntry()->nameTextId;
    }
    str = text_getString(textId);
    font_setStyle(FONT_STYLE_8PX_B, BONUS_MAP_TEXT_COLOR_NORMAL, 0);
    font_drawString(0x51, BONUS_MAP_PANEL_TEXT_X, 0x18, str, 1);
}

/**
 * Clear the panel (heading + all rows) and redraw it for the current page and state,
 * highlighting row selectedIndex where the state has a cursor.
 */
static void redrawPanel(u8 selectedIndex)
{
    u32 i;
    u32 count;

    gfx_fillBgRect(2, 0xc, PANEL_CLEAR_ROW, 0x14, PANEL_CLEAR_HEIGHT, 0);
    drawEntryLabel();
    count = rowCount();
    for (i = 0; i < count; i++)
    {
        drawSubEntry((u8)i, selectedIndex);
    }
    drawScrollIndicators();
}

/**
 * Draw the page name (path / "Bonus Maps") where the stock scene draws its
 * "Choose your path" title.
 */
static void drawTitle(void)
{
    char *str = text_getString(currentPage()->nameTextId);

    font_setStyle(FONT_STYLE_11PX_A, 3, 0);
    font_drawString(1, 0x78, 4, str, 1);
}

/**
 * L/R page switch on the grid: free the icon/cursor sprites and rebuild them for the
 * new page (same alloc path as Enter; Tick's actor_tickAll renders them).
 */
static void switchPage(bool forward)
{
    u32 page = PAGE_INDEX;

    if (forward)
    {
        page = page + 1 < PAGE_COUNT ? page + 1 : 0;
    }
    else
    {
        page = page > 0 ? page - 1 : PAGE_COUNT - 1;
    }
    SCENE_ARG_BONUS_MAP_CURSOR = page << 8; // entry reset to 0
    SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
    SCENE_ARG_CHAPTER_SELECT_SCROLL = 0;
    sprite_flushList(&g_ActorListMain);
    setupSprites();
    setupCursor();
    gfx_fillBgRect(2, 0, 0, 0x20, 2, 0); // the title's 11px glyphs span rows 0-1
    drawTitle();
    redrawPanel(0);
    sfx_play(SFX_MENU_CURSOR_MOVE);
}

/**
 * D-pad handler: 2D entry navigation on the grid (left/right wraps within the row,
 * up/down toggles row where a second row exists), the chapter-list cursor with edge
 * scrolling, and the stock difficulty cursor.
 */
static void moveCursor(void)
{
    u8 oldIndex;
    u8 count;
    u8 i;
    bool moved = FALSE;

    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT: {
        u32 entryCount = currentPage()->entryCount;
        u32 cursor = ENTRY_INDEX;
        u32 row = cursor / GRID_COLS;
        u32 col = cursor % GRID_COLS;
        u32 rowEntries;

        oldIndex = (u8)cursor;
        if ((PLAYER_KEYS_PRESSED(0) & (DPAD_UP | DPAD_DOWN)) != 0 && entryCount > GRID_COLS)
        {
            row ^= 1;
        }
        // clamp the column into the (possibly partial) row
        rowEntries = entryCount - row * GRID_COLS;
        if (rowEntries > GRID_COLS)
        {
            rowEntries = GRID_COLS;
        }
        if (col >= rowEntries)
        {
            col = rowEntries - 1;
        }
        col = menu_movePlayerCursorHorizontal(&col, 0, rowEntries - 1, TRUE, 0);
        cursor = row * GRID_COLS + col;
        // page bits stay: an entry switch keeps the page, resets the drill-down
        SCENE_ARG_BONUS_MAP_CURSOR = (PAGE_INDEX << 8) | cursor;
        if (oldIndex != cursor)
        {
            moved = TRUE;
        }
        sprite_setPosition(g_BonusMapCursorObj, slotX(cursor) << 16, slotY(cursor) << 16);
        SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
        SCENE_ARG_CHAPTER_SELECT_SCROLL = 0;
        redrawPanel(0);
        break;
    }
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT: {
        u32 scroll;

        oldIndex = (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY;
        count = (u8)rowCount();
        SCENE_ARG_BONUS_MAP_SUB_ENTRY =
            menu_movePlayerCursorVertical(&SCENE_ARG_BONUS_MAP_SUB_ENTRY, 0, count - 1, TRUE, 0);
        if (oldIndex != SCENE_ARG_BONUS_MAP_SUB_ENTRY)
        {
            moved = TRUE;
        }
        // Snap the window to the cursor's page (5 chapters per page).
        scroll = SCENE_ARG_CHAPTER_SELECT_SCROLL;
        if (SCENE_ARG_BONUS_MAP_SUB_ENTRY < scroll ||
            SCENE_ARG_BONUS_MAP_SUB_ENTRY >= scroll + VISIBLE_ROWS)
        {
            scroll = (SCENE_ARG_BONUS_MAP_SUB_ENTRY / VISIBLE_ROWS) * VISIBLE_ROWS;
        }
        if (scroll != SCENE_ARG_CHAPTER_SELECT_SCROLL)
        {
            SCENE_ARG_CHAPTER_SELECT_SCROLL = scroll;
            gfx_fillBgRect(2, 0xc, PANEL_LIST_ROW, 0x14, PANEL_LIST_HEIGHT, 0);
            for (i = 0; i < count; i++)
            {
                drawSubEntry(i, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
            }
            drawScrollIndicators();
        }
        else
        {
            drawSubEntry(oldIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
            drawSubEntry((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        }
        break;
    }
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        oldIndex = (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY;
        SCENE_ARG_BONUS_MAP_DIFFICULTY = menu_movePlayerCursorVertical(
            &SCENE_ARG_BONUS_MAP_DIFFICULTY, 0, OPTIONS_DIFFICULTY_GRUELING, TRUE, 0);
        if (oldIndex != SCENE_ARG_BONUS_MAP_DIFFICULTY)
        {
            moved = TRUE;
        }
        scene_bonusMapMenu_drawDifficultyRow(oldIndex, (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    }
    if (moved)
    {
        sfx_play(SFX_MENU_CURSOR_MOVE);
    }
}

/**
 * Confirm-button handler. Grid: enter the selection's sub-entry selector (the region's
 * chapter list, or the bonus map's sub-mission chain; error sfx if locked). Chapter
 * list: advance to the difficulty selector. Difficulty: store the chosen difficulty in
 * the save's option flags and arm the launch.
 */
static void onConfirm(void)
{
    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT:
        if (entryUnlocked(ENTRY_INDEX))
        {
            SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_SUB_ENTRY_SELECT;
            SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
            SCENE_ARG_CHAPTER_SELECT_SCROLL = 0;
            redrawPanel((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
        }
        else
        {
            sfx_play(SFX_MENU_ERROR);
        }
        break;
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_DIFFICULTY_SELECT;
        drawEntryLabel();
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        if (SCENE_ARG_BONUS_MAP_DIFFICULTY == OPTIONS_DIFFICULTY_GRUELING)
        {
            g_SaveGlobals.d.optionsFlags.d.difficultyGrueling = TRUE;
            g_SaveGlobals.d.optionsFlags.d.difficultyHard = FALSE;
        }
        else if (SCENE_ARG_BONUS_MAP_DIFFICULTY == OPTIONS_DIFFICULTY_HARD)
        {
            g_SaveGlobals.d.optionsFlags.d.difficultyHard = TRUE;
            g_SaveGlobals.d.optionsFlags.d.difficultyGrueling = FALSE;
        }
        else
        {
            g_SaveGlobals.d.optionsFlags.d.difficultyHard = FALSE;
            g_SaveGlobals.d.optionsFlags.d.difficultyGrueling = FALSE;
        }
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_LAUNCH_MISSION;
        break;
    }
    sfx_play(SFX_MENU_CONFIRM);
}

/**
 * Back-button handler: step the drill-down back one state (chapter list -> grid ->
 * main menu), redrawing the panel where the content changes.
 */
static void onBack(void)
{
    switch (SCENE_ARG_BONUS_MAP_STATE)
    {
    case BONUS_MAP_STATE_MISSION_SELECT:
        game_requestSceneChange(SCENE_ID_MAIN_MENU);
        break;
    case BONUS_MAP_STATE_SUB_ENTRY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_MISSION_SELECT;
        SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
        SCENE_ARG_CHAPTER_SELECT_SCROLL = 0;
        redrawPanel((u8)ENTRY_INDEX);
        break;
    case BONUS_MAP_STATE_DIFFICULTY_SELECT:
        SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_SUB_ENTRY_SELECT;
        drawEntryLabel();
        scene_bonusMapMenu_drawDifficultyRow((u8)SCENE_ARG_BONUS_MAP_DIFFICULTY,
                                             (u8)SCENE_ARG_BONUS_MAP_DIFFICULTY);
        break;
    }
    sfx_play(SFX_MENU_BACK);
}

/**
 * Enter handler: BGs, page-icon/cursor sprites, and the side-panel text (title = page
 * name, heading, sub-entry preview + page indicators, difficulty rows).
 *
 * REPLACES (whole function): scene_bonusMapMenu_enter @0x08012f90.
 */
void chapterSelect_enter(void)
{
    u8 rowIndex;

    gfx_initDisplay(0);
    gfx_enableDisplayFlags(GFX_DISPCNT_OBJ);
    gfx_setupBg(0, BonusMapMenuBgCnt0);
    gfx_setBgOffset(0, 0);
    gfx_loadBgAsset(0, BonusMapMenuBg0Asset, 1, 0, 0, 0);
    setupSprites();
    gfx_setupBg(2, BonusMapMenuBgCnt2);
    gfx_loadPalette(UiPalette, 0);
    gfx_setBgOffset(2, 0);
    SCENE_ARG_BONUS_MAP_STATE = BONUS_MAP_STATE_MISSION_SELECT;
    SCENE_ARG_BONUS_MAP_CURSOR = 0;
    SCENE_ARG_BONUS_MAP_SUB_ENTRY = 0;
    SCENE_ARG_BONUS_MAP_DIFFICULTY = 0;
    setupCursor();
    font_setRenderCtxBg(BonusMapMenuBgCnt2);
    drawTitle();
    redrawPanel((u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
    for (rowIndex = 0; rowIndex < 3; rowIndex++)
    {
        scene_bonusMapMenu_drawDifficultyRow(rowIndex, (u8)SCENE_ARG_BONUS_MAP_SUB_ENTRY);
    }
    gfx_registerPaletteCycles(BonusMapMenuPaletteCycles);
    actor_tickAll();
    sprite_endFrame();
    gfx_fadeInFromBlack(GFX_BLEND_ALL_TARGETS);
}

/**
 * Per-frame tick. Launch-armed state: write the chapter's variant and spawn point and
 * transition to gameplay on its mission. Otherwise: A
 * confirms, d-pad moves the cursor, L/R switches page on the grid, B/select backs out.
 *
 * REPLACES (whole function): scene_bonusMapMenu_tick @0x08013084.
 */
void chapterSelect_tick(void)
{
    if (SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_LAUNCH_MISSION)
    {
        const struct ChapterSelectMission *mission =
            &currentEntry()->missions[SCENE_ARG_BONUS_MAP_SUB_ENTRY];

        g_MissionVariant = mission->variant;
        g_MissionSpawnPoint[0] = mission->spawnPoint;
        g_MissionSpawnPoint[1] = mission->spawnPoint;
        game_requestSceneChangeWithArgs(SCENE_ID_GAMEPLAY, GAMEPLAY_MODE_START_HEALED,
                                        mission->missionId);
        // handed to the gameplay scene as its args[3];
        // the stock BonusMissionEntry.field_0x2 is 0 throughout
        g_SceneNext.args[3] = 0;
    }
    // ROM asymmetry: A/d-pad read player 0's keys, B/select read the active player's.
    else if ((PLAYER_KEYS_PRESSED(0) & A_BUTTON) != 0)
    {
        onConfirm();
    }
    else if ((PLAYER_KEYS_PRESSED(0) & DPAD_ANY) != 0)
    {
        moveCursor();
    }
    else if (SCENE_ARG_BONUS_MAP_STATE == BONUS_MAP_STATE_MISSION_SELECT &&
             (PLAYER_KEYS_PRESSED(0) & (L_BUTTON | R_BUTTON)) != 0)
    {
        switchPage((PLAYER_KEYS_PRESSED(0) & R_BUTTON) != 0);
    }
    else if ((PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & B_BUTTON) != 0 ||
             (PLAYER_KEYS_PRESSED(ACTIVE_PLAYER_INDEX) & SELECT_BUTTON) != 0)
    {
        onBack();
    }
    actor_tickAll();
    sprite_endFrame();
}
