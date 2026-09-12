; The load-menu functions save_slot_pages replaces with page-aware C bodies.
; The slot addressing these navigate lives in mod_save (save_slots.asm).
;
; Ported from the original, fully assembly-based SavSlotPages ROM-hack by SuperSaiyajinStackZ.

.thumb

hookReplace scene_saveLoadMenu_enter, saveSlotPages_enter
hookReplace scene_saveLoadMenu_moveCursor, saveSlotPages_moveCursor
hookReplace scene_saveLoadMenu_commitSlot, saveSlotPages_commitSlot
