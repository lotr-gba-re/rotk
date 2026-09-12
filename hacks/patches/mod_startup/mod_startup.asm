.thumb

; game_initBoot entry: replaced wholesale, so a boot patch adds its case to
; mod_startup's requestFirstScene rather than `.org`ing this function.
hookReplace game_initBoot, modStartup_initBoot

; Language-select confirm: replaced wholesale so the boot-time branch also matches an
; entry from the migration scene (stock tests previous == 0 only and would bounce back
; into that scene without persisting the language).
hookReplace scene_languageSelect_onConfirm, modStartup_languageSelectOnConfirm
