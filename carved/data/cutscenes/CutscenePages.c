#include "scene/cutscene.h"
#include "scene/gameplay.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

/**
 * Cutscene pages, chained by nextPage; scene_cutscene shows args[1]'s. Page
 * 58 is the ending's first page for Aragorn; the other heroes' start at 60, 62,
 * 64, 66, 68 by HeroId order (Frodo and Sam 62).
 *
 * @romaddress 0x0806f68c
 */
const CutscenePage CutscenePages[71] = {
    // [0] "Orthanc. It was obvious the Ents had preceded us."
    {.bg0Asset = CutscenePage00BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_ORTHANC_IT_WAS_OBVIOUS_THE_ENTS_HAD_PREC,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 1, .bg0PaletteNumber = 0},
    // [1] "Show yourself, Saruman! Receive the judgment of those you..."
    {.bg0Asset = CutscenePage01BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_SHOW_YOURSELF_SARUMAN_RECEIVE_THE_JUDGME,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 2, .bg0PaletteNumber = 1},
    // [2] "Can we not have peace, you and I?"
    {.bg0Asset = CutscenePage02BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_CAN_WE_NOT_HAVE_PEACE_YOU_AND_I,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 3, .bg0PaletteNumber = 2},
    // [3] "We will have peace when you hang from your window for the..."
    {.bg0Asset = CutscenePage03BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_WE_WILL_HAVE_PEACE_WHEN_YOU_HANG_FROM_YO,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 4, .bg0PaletteNumber = 3},
    // [4] "We must find a way into Saruman's tower."
    {.bg0Asset = CutscenePage04BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_MUST_FIND_A_WAY_INTO_SARUMANS_TOWER,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 4},
    // [5] "Saruman's power is no more. His staff is broken."
    {.bg0Asset = CutscenePage05BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_SARUMANS_POWER_IS_NO_MORE_HIS_STAFF_IS_B,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 6, .bg0PaletteNumber = 0},
    // [6] "Grima! You need not follow him! You were once a man of Ro..."
    {.bg0Asset = CutscenePage06BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_GRIMA_YOU_NEED_NOT_FOLLOW_HIM_YOU_WERE_O,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 7, .bg0PaletteNumber = 1},
    // [7] "You have paid dearly for his devotion, Theoden. Did you n..."
    {.bg0Asset = CutscenePage07BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_YOU_HAVE_PAID_DEARLY_FOR_HIS_DEVOTION_TH,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 8, .bg0PaletteNumber = 2},
    // [8] "You made me do it!"
    {.bg0Asset = CutscenePage08BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_YOU_MADE_ME_DO_IT,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 9, .bg0PaletteNumber = 3},
    // [9] "And you do what I say - always - don't you worm?"
    {.bg0Asset = CutscenePage09BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_AND_YOU_DO_WHAT_I_SAY_ALWAYS_DONT_YOU_WO,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 10, .bg0PaletteNumber = 4},
    // [10] "Nooooo!"
    {.bg0Asset = CutscenePage10BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_NOOOOO,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 5},
    // [11] "Now that Isengard has fallen, Edoras is safe once more."
    {.bg0Asset = CutscenePage11BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_NOW_THAT_ISENGARD_HAS_FALLEN_EDORAS_IS_S,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 12, .bg0PaletteNumber = 0},
    // [12] "The same cannot be said of Gondor."
    {.bg0Asset = CutscenePage12BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_THE_SAME_CANNOT_BE_SAID_OF_GONDOR,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 13, .bg0PaletteNumber = 1},
    // [13] "We must ride for the stronghold of Minas Tirith. No army ..."
    {.bg0Asset = CutscenePage13BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_MUST_RIDE_FOR_THE_STRONGHOLD_OF_MINAS,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 2},
    // [14] "Aragorn... why do you not come with us to Minas Tirith?"
    {.bg0Asset = CutscenePage14BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_ARAGORN_WHY_DO_YOU_NOT_COME_WITH_US_TO_M,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 15, .bg0PaletteNumber = 0},
    // [15] "My path does not lie that way."
    {.bg0Asset = CutscenePage15BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_MY_PATH_DOES_NOT_LIE_THAT_WAY,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 16, .bg0PaletteNumber = 1},
    // [16] "And where do you think you're going?"
    {.bg0Asset = CutscenePage16BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_AND_WHERE_DO_YOU_THINK_YOURE_GOING,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 17, .bg0PaletteNumber = 2},
    // [17] "This time you stay, Gimli my friend."
    {.bg0Asset = CutscenePage15BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_THIS_TIME_YOU_STAY_GIMLI_MY_FRIEND,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 18, .bg0PaletteNumber = 3},
    // [18] "Have you learned nothing of the stubbornness of Dwarves?"
    {.bg0Asset = CutscenePage18BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_HAVE_YOU_LEARNED_NOTHING_OF_THE_STUBBORN,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 19, .bg0PaletteNumber = 4},
    // [19] "The three companions set out together down the dark road ..."
    {.bg0Asset = CutscenePage19BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_THE_THREE_COMPANIONS_SET_OUT_TOGETHER_DO,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 5},
    // [20] "Sauron's Corsairs were defeated, and now our ghostly army..."
    {.bg0Asset = CutscenePage20BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_SAURONS_CORSAIRS_WERE_DEFEATED_AND_NOW_O,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 0},
    // [21] "The battle is won, yet the darkness is deepening."
    {.bg0Asset = CutscenePage21BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_BATTLE_IS_WON_YET_THE_DARKNESS_IS_DE,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 22, .bg0PaletteNumber = 0},
    // [22] "If Sauron had the Ring, we would know it."
    {.bg0Asset = CutscenePage22BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_IF_SAURON_HAD_THE_RING_WE_WOULD_KNOW_IT,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 23, .bg0PaletteNumber = 1},
    // [23] "Would we? Even if Frodo survived Cirith Ungol, thousands ..."
    {.bg0Asset = CutscenePage23BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WOULD_WE_EVEN_IF_FRODO_SURVIVED_CIRITH_U,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 24, .bg0PaletteNumber = 2},
    // [24] "Then we empty Sauron's lands... march on Mordor... and so..."
    {.bg0Asset = CutscenePage24BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_THEN_WE_EMPTY_SAURONS_LANDS_MARCH_ON_MOR,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 3},
    // [25] "We raced from the black tunnels... with a new ally that t..."
    {.bg0Asset = CutscenePage25BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_RACED_FROM_THE_BLACK_TUNNELS_WITH_A_N,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 0},
    // [26] "The battle was won in a glorious charge, my uncle shoutin..."
    {.bg0Asset = CutscenePage26BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_BATTLE_WAS_WON_IN_A_GLORIOUS_CHARGE,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 27, .bg0PaletteNumber = 0},
    // [27] "Eomer and Gandalf may have helped save Rohan, but there i..."
    {.bg0Asset = CutscenePage27BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_EOMER_AND_GANDALF_MAY_HAVE_HELPED_SAVE_R,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 28, .bg0PaletteNumber = 1},
    // [28] "We must ride to Gondor."
    {.bg0Asset = CutscenePage28BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_MUST_RIDE_TO_GONDOR,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 29, .bg0PaletteNumber = 2},
    // [29] "First we deal with a traitor."
    {.bg0Asset = CutscenePage27BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_FIRST_WE_DEAL_WITH_A_TRAITOR,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 30, .bg0PaletteNumber = 3},
    // [30] "Saruman will pay dearly for this night."
    {.bg0Asset = CutscenePage30BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_SARUMAN_WILL_PAY_DEARLY_FOR_THIS_NIGHT,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 31, .bg0PaletteNumber = 4},
    // [31] "Return to Edoras, niece."
    {.bg0Asset = CutscenePage27BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_RETURN_TO_EDORAS_NIECE,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 32, .bg0PaletteNumber = 5},
    // [32] "But I can fight!"
    {.bg0Asset = CutscenePage30BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_BUT_I_CAN_FIGHT,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 33, .bg0PaletteNumber = 6},
    // [33] "Not today. Helm's Deep is in ruin. Bring the wounded to E..."
    {.bg0Asset = CutscenePage27BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_NOT_TODAY_HELMS_DEEP_IS_IN_RUIN_BRING_TH,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 7},
    // [34] "Saruman is dead."
    {.bg0Asset = CutscenePage34BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_SARUMAN_IS_DEAD,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 35, .bg0PaletteNumber = 0},
    // [35] "Aragorn... you startled me. What time is it?"
    {.bg0Asset = CutscenePage35BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_ARAGORN_YOU_STARTLED_ME_WHAT_TIME_IS_IT,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 36, .bg0PaletteNumber = 1},
    // [36] "Not yet dawn. But we have seen the beacons of Minas Tirit..."
    {.bg0Asset = CutscenePage34BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_NOT_YET_DAWN_BUT_WE_HAVE_SEEN_THE_BEACON,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 2},
    // [37] "Eowyn... why did you come?"
    {.bg0Asset = CutscenePage37BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_EOWYN_WHY_DID_YOU_COME,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 38, .bg0PaletteNumber = 0},
    // [38] "Do you not know?"
    {.bg0Asset = CutscenePage38BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_DO_YOU_NOT_KNOW,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 39, .bg0PaletteNumber = 1},
    // [39] "It is only a shadow and a thought that you love, Eowyn. I..."
    {.bg0Asset = CutscenePage39BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_IT_IS_ONLY_A_SHADOW_AND_A_THOUGHT_THAT_Y,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 2},
    // [40] "I'm glad you've come, Gandalf! Hoom! Wood and water, stoc..."
    {.bg0Asset = CutscenePage40BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_IM_GLAD_YOUVE_COME_GANDALF_HOOM_WOOD_AND,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 41, .bg0PaletteNumber = 0},
    // [41] "Too long have you escaped the vengeance of Sauron! What d..."
    {.bg0Asset = CutscenePage41BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_TOO_LONG_HAVE_YOU_ESCAPED_THE_VENGEANCE_1632,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 42, .bg0PaletteNumber = 1},
    // [42] "I have never coveted power, Saruman."
    {.bg0Asset = CutscenePage42BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_I_HAVE_NEVER_COVETED_POWER_SARUMAN,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 43, .bg0PaletteNumber = 2},
    // [43] "Shall I kill him?"
    {.bg0Asset = CutscenePage43BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_SHALL_I_KILL_HIM,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 44, .bg0PaletteNumber = 3},
    // [44] "No... I will deal with him."
    {.bg0Asset = CutscenePage44BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_NO_I_WILL_DEAL_WITH_HIM,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 4},
    // [45] "I see Him! I can see him in my head!"
    {.bg0Asset = CutscenePage45BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_I_SEE_HIM_I_CAN_SEE_HIM_IN_MY_HEAD,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 46, .bg0PaletteNumber = 0},
    // [46] "Pippin! Fool of a Took! Understand, things are now in mot..."
    {.bg0Asset = CutscenePage46BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_PIPPIN_FOOL_OF_A_TOOK_UNDERSTAND_THINGS,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 47, .bg0PaletteNumber = 1},
    // [47] "There is no time to linger. There is a secret route throu..."
    {.bg0Asset = CutscenePage47BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THERE_IS_NO_TIME_TO_LINGER_THERE_IS_A_SE,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 2},
    // [48] "We shall both go up in a great fire. Ash, ash, and smoke...."
    {.bg0Asset = CutscenePage48BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_SHALL_BOTH_GO_UP_IN_A_GREAT_FIRE_ASH,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 49, .bg0PaletteNumber = 0},
    // [49] "Stay this madness, Denethor! I shall not let you slay you..."
    {.bg0Asset = CutscenePage49BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_STAY_THIS_MADNESS_DENETHOR_I_SHALL_NOT_L,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [50] "Wake up! Wake up! Wake up, sleepies! We must go, yes, we ..."
    {.bg0Asset = CutscenePage50BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WAKE_UP_WAKE_UP_WAKE_UP_SLEEPIES_WE_MUST,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 51, .bg0PaletteNumber = 0},
    // [51] "Haven't you had any sleep, Mr. Frodo? It must be getting ..."
    {.bg0Asset = CutscenePage51BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_HAVENT_YOU_HAD_ANY_SLEEP_MR_FRODO_IT_MUS,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 52, .bg0PaletteNumber = 1},
    // [52] "We should save what food we have, Sam. There's little hop..."
    {.bg0Asset = CutscenePage52BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_WE_SHOULD_SAVE_WHAT_FOOD_WE_HAVE_SAM_THE,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_MIDDLE,
     .nextPage = 53, .bg0PaletteNumber = 2},
    // [53] "Hey stinker, go find us some more food! Can't you see Mr...."
    {.bg0Asset = CutscenePage51BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_HEY_STINKER_GO_FIND_US_SOME_MORE_FOOD_CA,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 3},
    // [54] "I'm gonna bleed you like a stuck pig!"
    {.bg0Asset = CutscenePage54BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_IM_GONNA_BLEED_YOU_LIKE_A_STUCK_PIG,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 55, .bg0PaletteNumber = 0},
    // [55] "Not if I stick you first!"
    {.bg0Asset = CutscenePage55BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_NOT_IF_I_STICK_YOU_FIRST,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [56] "Frodo! Destroy it, now, do it! Throw it in the fire!"
    {.bg0Asset = CutscenePage56BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_FRODO_DESTROY_IT_NOW_DO_IT_THROW_IT_IN_T,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 57, .bg0PaletteNumber = 0},
    // [57] "I have made my choice. I will not destroy it... the Ring ..."
    {.bg0Asset = CutscenePage57BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_I_HAVE_MADE_MY_CHOICE_I_WILL_NOT_DESTROY,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [58] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 59, .bg0PaletteNumber = 0},
    // [59] "...and Aragorn was received with great cheers in the wall..."
    {.bg0Asset = CutscenePage59BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_ARAGORN_WAS_RECEIVED_WITH_GREAT_CHEE,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [60] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 61, .bg0PaletteNumber = 0},
    // [61] "...and Eowyn was honored as a Shieldmaiden of Rohan. She ..."
    {.bg0Asset = CutscenePage61BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_EOWYN_WAS_HONORED_AS_A_SHIELDMAIDEN,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [62] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 63, .bg0PaletteNumber = 0},
    // [63] "...and Frodo was reunited with his friends thirteen month..."
    {.bg0Asset = CutscenePage63BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_FRODO_WAS_REUNITED_WITH_HIS_FRIENDS,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [64] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 65, .bg0PaletteNumber = 0},
    // [65] "...and Gandalf met the Hobbits with great hugs and smiles..."
    {.bg0Asset = CutscenePage65BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_GANDALF_MET_THE_HOBBITS_WITH_GREAT_H,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [66] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 67, .bg0PaletteNumber = 0},
    // [67] "...and Gimli was received as a great Dwarf Lord, but more..."
    {.bg0Asset = CutscenePage67BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_GIMLI_WAS_RECEIVED_AS_A_GREAT_DWARF,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [68] "The power of the Ring is destroyed, and all that was done..."
    {.bg0Asset = CutscenePage58BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A,
     .nextMode = GAMEPLAY_MODE_3, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = 69, .bg0PaletteNumber = 0},
    // [69] "...and Legolas was celebrated as a great hero, and though..."
    {.bg0Asset = CutscenePage69BgAsset, .bg0Cnt = &CutsceneBgCnt1,
     .textId = TEXT_ID_AND_LEGOLAS_WAS_CELEBRATED_AS_A_GREAT_HE,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_LAST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 1},
    // [70] "And, in a strange twist of fate, Smeagol threw Frodo into..."
    {.bg0Asset = CutscenePage50BgAsset, .bg0Cnt = &CutsceneBgCnt0,
     .textId = TEXT_ID_AND_IN_A_STRANGE_TWIST_OF_FATE_SMEAGOL_T,
     .nextMode = GAMEPLAY_MODE_4, .mode = CUTSCENE_PAGE_FIRST,
     .nextPage = CUTSCENE_PAGE_NONE, .bg0PaletteNumber = 0},
};
// clang-format on
