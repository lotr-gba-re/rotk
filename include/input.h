#pragma once

#include "player.h" // ACTIVE_PLAYER_INDEX
#include "types.h"

// Per-slot shorthands for the two key-state array sets: the gameplay set (link-synced in
// co-op) and the menu set.
#define PLAYER_KEYS_CURRENT(playerIndex) g_PlayerKeysCurrent[playerIndex]
#define PLAYER_KEYS_PREV(playerIndex) g_PlayerKeysPrev[playerIndex]
#define PLAYER_KEYS_PRESSED(playerIndex) g_PlayerKeysPressed[playerIndex]
#define PLAYER_KEYS_RELEASED(playerIndex) g_PlayerKeysReleased[playerIndex]
#define MENU_KEYS_CURRENT(playerIndex) g_MenuKeysCurrent[playerIndex]
#define MENU_KEYS_PREV(playerIndex) g_MenuKeysPrev[playerIndex]
#define MENU_KEYS_PRESSED(playerIndex) g_MenuKeysPressed[playerIndex]
#define MENU_KEYS_RELEASED(playerIndex) g_MenuKeysReleased[playerIndex]

// The multiplayer menu key set updates only the active player's slot.
#define ACTIVE_MENU_KEYS_CURRENT MENU_KEYS_CURRENT(ACTIVE_PLAYER_INDEX)
#define ACTIVE_MENU_KEYS_PREV MENU_KEYS_PREV(ACTIVE_PLAYER_INDEX)
#define ACTIVE_MENU_KEYS_PRESSED MENU_KEYS_PRESSED(ACTIVE_PLAYER_INDEX)
#define ACTIVE_MENU_KEYS_RELEASED MENU_KEYS_RELEASED(ACTIVE_PLAYER_INDEX)

void input_clear(void);
void input_clearPlayer(u8 playerIndex);
void input_clearPlayerDpad(u8 playerIndex);
void input_update(void);
void input_init(void);
void input_enable(void);
void input_disable(void);
u8 input_getPlayerDpadDirection8(u8 playerIndex);
