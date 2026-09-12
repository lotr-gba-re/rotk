.thumb

; game_dispatchSceneEnter entry: same-signature replacement (NULL check + the
; previous-scene-0x11 special case live in the C body).
hookReplace game_dispatchSceneEnter, modScenes_dispatchEnter

; game_dispatchSceneTick entry: same replacement.
hookReplace game_dispatchSceneTick, modScenes_dispatchTick

; game_dispatchSceneExit entry: same replacement (NULL check + GAME_FLAG_IN_GAME_DIALOG
; check live in the C body).
hookReplace game_dispatchSceneExit, modScenes_dispatchExit
