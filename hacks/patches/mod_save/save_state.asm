.thumb

; save_init entry: stubbed to a bare return.
; This work is now done by modSave_loadState after the save migration machinery had a chance to run
hookStub save_init

; The save functions below should now be dead code.
; Trap the functions to make sure any stray callers would hang the game.
hookTrap save_formatHeader
hookTrap save_validateHeader
