#include "scene/credits.h"
#include "scene/inGameDialog.h"
#include "text_ids.h"
#include "variables.h"

// clang-format off

/**
 * Per text id, the speaker picture the in-game dialog draws with the line and
 * the value it formats into slot 0; the credits lines use the latter to tag
 * their kind. Lines without either are left out.
 *
 * @romaddress 0x082833d8
 */
const DialogLineInfo DialogLineInfos[2022] = {
    // [1498] "Orthanc. It was obvious the Ents had preceded us."
    [TEXT_ID_ORTHANC_IT_WAS_OBVIOUS_THE_ENTS_HAD_PREC] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1499] "Show yourself, Saruman! Receive the judgment of those you..."
    [TEXT_ID_SHOW_YOURSELF_SARUMAN_RECEIVE_THE_JUDGME] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1500] "Can we not have peace, you and I?"
    [TEXT_ID_CAN_WE_NOT_HAVE_PEACE_YOU_AND_I] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1501] "We will have peace when you hang from your window for the..."
    [TEXT_ID_WE_WILL_HAVE_PEACE_WHEN_YOU_HANG_FROM_YO] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1502] "We must find a way into Saruman's tower."
    [TEXT_ID_WE_MUST_FIND_A_WAY_INTO_SARUMANS_TOWER] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1506] "Saruman's power is no more. His staff is broken."
    [TEXT_ID_SARUMANS_POWER_IS_NO_MORE_HIS_STAFF_IS_B] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1507] "Grima! You need not follow him! You were once a man of Ro..."
    [TEXT_ID_GRIMA_YOU_NEED_NOT_FOLLOW_HIM_YOU_WERE_O] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1508] "You have paid dearly for his devotion, Theoden. Did you n..."
    [TEXT_ID_YOU_HAVE_PAID_DEARLY_FOR_HIS_DEVOTION_TH] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1509] "You made me do it!"
    [TEXT_ID_YOU_MADE_ME_DO_IT] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1510] "And you do what I say - always - don't you worm?"
    [TEXT_ID_AND_YOU_DO_WHAT_I_SAY_ALWAYS_DONT_YOU_WO] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1511] "Nooooo!"
    [TEXT_ID_NOOOOO] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1512] "Too long have you escaped the vengeance of Sauron... but ..."
    [TEXT_ID_TOO_LONG_HAVE_YOU_ESCAPED_THE_VENGEANCE] =
        {.speakerPicture = DialogPortraitSarumanBgAsset},
    // [1516] "Edoras may be safe, but we should fear roving bands of Ur..."
    [TEXT_ID_EDORAS_MAY_BE_SAFE_BUT_WE_SHOULD_FEAR_RO] =
        {.speakerPicture = DialogPortraitEomerBgAsset},
    // [1517] "We have had a narrow escape... but the battle for Middle-..."
    [TEXT_ID_WE_HAVE_HAD_A_NARROW_ESCAPE_BUT_THE_BATT] =
        {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1519] "Now that Isengard has fallen, Edoras is safe once more."
    [TEXT_ID_NOW_THAT_ISENGARD_HAS_FALLEN_EDORAS_IS_S] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1520] "The same cannot be said of Gondor."
    [TEXT_ID_THE_SAME_CANNOT_BE_SAID_OF_GONDOR] = {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1521] "We must ride for the stronghold of Minas Tirith. No army ..."
    [TEXT_ID_WE_MUST_RIDE_FOR_THE_STRONGHOLD_OF_MINAS] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1522] "They say some old fountains have restorative powers."
    [TEXT_ID_THEY_SAY_SOME_OLD_FOUNTAINS_HAVE_RESTORA] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1523] "Theoden says we have six-thousand spears."
    [TEXT_ID_THEODEN_SAYS_WE_HAVE_SIX_THOUSAND_SPEARS] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1524] "Half what we had hoped."
    [TEXT_ID_HALF_WHAT_WE_HAD_HOPED] = {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1525] "We need more time."
    [TEXT_ID_WE_NEED_MORE_TIME] = {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1526] "It is a seven day ride to Minas Tirith. Gondor cannot wai..."
    [TEXT_ID_IT_IS_A_SEVEN_DAY_RIDE_TO_MINAS_TIRITH_G] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1527] "Then we must do it in three."
    [TEXT_ID_THEN_WE_MUST_DO_IT_IN_THREE] = {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1528] "Aragorn... why do you not come with us to Minas Tirith?"
    [TEXT_ID_ARAGORN_WHY_DO_YOU_NOT_COME_WITH_US_TO_M] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1529] "My path does not lie that way."
    [TEXT_ID_MY_PATH_DOES_NOT_LIE_THAT_WAY] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1530] "And where do you think you're going?"
    [TEXT_ID_AND_WHERE_DO_YOU_THINK_YOURE_GOING] = {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1531] "This time you stay, Gimli my friend."
    [TEXT_ID_THIS_TIME_YOU_STAY_GIMLI_MY_FRIEND] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1532] "Have you learned nothing of the stubbornness of Dwarves?"
    [TEXT_ID_HAVE_YOU_LEARNED_NOTHING_OF_THE_STUBBORN] =
        {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1533] "The three companions set out together down the dark road ..."
    [TEXT_ID_THE_THREE_COMPANIONS_SET_OUT_TOGETHER_DO] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1534] "Help us! Goblins are raiding the camps. Please save the h..."
    [TEXT_ID_HELP_US_GOBLINS_ARE_RAIDING_THE_CAMPS_PL] =
        {.speakerPicture = DialogPortraitRohanSoldierBgAsset},
    // [1535] "Lord Elrond!"
    [TEXT_ID_LORD_ELROND] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1536] "Your path does not lie with the others, Aragorn. The man ..."
    [TEXT_ID_YOUR_PATH_DOES_NOT_LIE_WITH_THE_OTHERS_A] =
        {.speakerPicture = DialogPortraitElrondBgAsset},
    // [1537] "So be it. I will visit these Paths of the Dead."
    [TEXT_ID_SO_BE_IT_I_WILL_VISIT_THESE_PATHS_OF_THE] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1538] "I saw a tunnel up ahead guarded by two Uruk-Hai captains...."
    [TEXT_ID_I_SAW_A_TUNNEL_UP_AHEAD_GUARDED_BY_TWO_U] =
        {.speakerPicture = DialogPortraitRangerBgAsset},
    // [1539] "Malice lies heavy in the air."
    [TEXT_ID_MALICE_LIES_HEAVY_IN_THE_AIR] = {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1540] "The living are not welcome on this road."
    [TEXT_ID_THE_LIVING_ARE_NOT_WELCOME_ON_THIS_ROAD] =
        {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1541] "What kind of an army would linger in such a place?"
    [TEXT_ID_WHAT_KIND_OF_AN_ARMY_WOULD_LINGER_IN_SUC] =
        {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1542] "One that is cursed... I do not fear the dead."
    [TEXT_ID_ONE_THAT_IS_CURSED_I_DO_NOT_FEAR_THE_DEA] =
        {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1543] "Now this is a thing unheard of... an Elf will go undergro..."
    [TEXT_ID_NOW_THIS_IS_A_THING_UNHEARD_OF_AN_ELF_WI] =
        {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1544] "Who enters my domain???"
    [TEXT_ID_WHO_ENTERS_MY_DOMAIN] = {.speakerPicture = DialogPortraitKingOfTheDeadBgAsset},
    // [1545] "One who would hold you to your oath!"
    [TEXT_ID_ONE_WHO_WOULD_HOLD_YOU_TO_YOUR_OATH] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1546] "The dead do not suffer the living to pass!"
    [TEXT_ID_THE_DEAD_DO_NOT_SUFFER_THE_LIVING_TO_PAS] =
        {.speakerPicture = DialogPortraitKingOfTheDeadBgAsset},
    // [1547] "You will suffer me!"
    [TEXT_ID_YOU_WILL_SUFFER_ME] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1548] "Brave fool! Let the abyss take you!"
    [TEXT_ID_BRAVE_FOOL_LET_THE_ABYSS_TAKE_YOU] =
        {.speakerPicture = DialogPortraitKingOfTheDeadBgAsset},
    // [1549] "I summon you to fulfill your oath!"
    [TEXT_ID_I_SUMMON_YOU_TO_FULFILL_YOUR_OATH] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1550] "None but the King of Gondor may command me!"
    [TEXT_ID_NONE_BUT_THE_KING_OF_GONDOR_MAY_COMMAND] =
        {.speakerPicture = DialogPortraitKingOfTheDeadBgAsset},
    // [1551] "Then I command you, for I am Isildur's heir."
    [TEXT_ID_THEN_I_COMMAND_YOU_FOR_I_AM_ISILDURS_HEI] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1552] "We raced from the black tunnels... with a new ally that t..."
    [TEXT_ID_WE_RACED_FROM_THE_BLACK_TUNNELS_WITH_A_N] =
        {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1553] "Be careful. There dwell here creatures that strike unseen..."
    [TEXT_ID_BE_CAREFUL_THERE_DWELL_HERE_CREATURES_TH] =
        {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1554] "I will defeat your warriors then come for you!"
    [TEXT_ID_I_WILL_DEFEAT_YOUR_WARRIORS_THEN_COME_FO] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1555] "Goblins here don't leave their treasures unattended for l..."
    [TEXT_ID_GOBLINS_HERE_DONT_LEAVE_THEIR_TREASURES] =
        {.speakerPicture = DialogPortraitRangerBgAsset},
    // [1556] "Sauron's corsair forces lie below us."
    [TEXT_ID_SAURONS_CORSAIR_FORCES_LIE_BELOW_US] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1557] "If we are to stop them, we must intercept their boats at ..."
    [TEXT_ID_IF_WE_ARE_TO_STOP_THEM_WE_MUST_INTERCEPT] =
        {.speakerPicture = DialogPortraitGimliBgAsset},
    // [1558] "Cut off the head of the snake."
    [TEXT_ID_CUT_OFF_THE_HEAD_OF_THE_SNAKE] = {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1559] "It will not be easy, time is running out."
    [TEXT_ID_IT_WILL_NOT_BE_EASY_TIME_IS_RUNNING_OUT] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1560] "Quickly! We must hurry!"
    [TEXT_ID_QUICKLY_WE_MUST_HURRY] = {.speakerPicture = DialogPortraitRangerBgAsset},
    // [1561] "I see it a little ways off, there might still be time..."
    [TEXT_ID_I_SEE_IT_A_LITTLE_WAYS_OFF_THERE_MIGHT_S] =
        {.speakerPicture = DialogPortraitRangerBgAsset},
    // [1562] "I see you've noticed the butterflies."
    [TEXT_ID_I_SEE_YOUVE_NOTICED_THE_BUTTERFLIES] = {.speakerPicture = DialogPortraitElderBgAsset},
    // [1563] "They don't mind the rain, but they hate disturbances."
    [TEXT_ID_THEY_DONT_MIND_THE_RAIN_BUT_THEY_HATE_DI] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1564] "I have been separated from my brother, please lead him ba..."
    [TEXT_ID_I_HAVE_BEEN_SEPARATED_FROM_MY_BROTHER_PL] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1565] "There are Corsairs everywhere! They have not found this h..."
    [TEXT_ID_THERE_ARE_CORSAIRS_EVERYWHERE_THEY_HAVE] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1566] "Thank you, kind sir. Maybe this can help you on your way."
    [TEXT_ID_THANK_YOU_KIND_SIR_MAYBE_THIS_CAN_HELP_Y] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1567] "You are swift, and well ahead of them still! Make haste."
    [TEXT_ID_YOU_ARE_SWIFT_AND_WELL_AHEAD_OF_THEM_STI] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1568] "All is not lost. I see it a little ways off, there is sti..."
    [TEXT_ID_ALL_IS_NOT_LOST_I_SEE_IT_A_LITTLE_WAYS_O] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1569] "Sauron's Corsairs were defeated, and now our ghostly army..."
    [TEXT_ID_SAURONS_CORSAIRS_WERE_DEFEATED_AND_NOW_O] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1570] "Friends from Umbar! Welcome! ... wait... what?! What tric..."
    [TEXT_ID_FRIENDS_FROM_UMBAR_WELCOME_WAIT_WHAT_WHA] =
        {.speakerPicture = DialogPortraitOrcBgAsset},
    // [1571] "For Middle-earth!"
    [TEXT_ID_FOR_MIDDLE_EARTH] = {.speakerPicture = DialogPortraitKingOfTheDeadBgAsset},
    // [1572] "The battle is won, yet the darkness is deepening."
    [TEXT_ID_THE_BATTLE_IS_WON_YET_THE_DARKNESS_IS_DE] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1573] "If Sauron had the Ring, we would know it."
    [TEXT_ID_IF_SAURON_HAD_THE_RING_WE_WOULD_KNOW_IT] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1574] "Would we? Even if Frodo survived Cirith Ungol, thousands ..."
    [TEXT_ID_WOULD_WE_EVEN_IF_FRODO_SURVIVED_CIRITH_U] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1575] "Then we empty Sauron's lands... march on Mordor... and so..."
    [TEXT_ID_THEN_WE_EMPTY_SAURONS_LANDS_MARCH_ON_MOR] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1576] "I hid my favorite adventure book, too, but I can't rememb..."
    [TEXT_ID_I_HID_MY_FAVORITE_ADVENTURE_BOOK_TOO_BUT] =
        {.speakerPicture = DialogPortraitWomanBgAsset},
    // [1577] "A lone man against my Orc army? Ha! Fight me if you wish,..."
    [TEXT_ID_A_LONE_MAN_AGAINST_MY_ORC_ARMY_HA_FIGHT] =
        {.speakerPicture = DialogPortraitOrcCaptainBgAsset},
    // [1578] "There are Corsairs about, it's not safe!"
    [TEXT_ID_THERE_ARE_CORSAIRS_ABOUT_ITS_NOT_SAFE] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1579] "Welcome to our city, hidden from the Lidless Eye. If you ..."
    [TEXT_ID_WELCOME_TO_OUR_CITY_HIDDEN_FROM_THE_LIDL] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1580] "Let the Lord of the Black Land come forth! Let justice be..."
    [TEXT_ID_LET_THE_LORD_OF_THE_BLACK_LAND_COME_FORT] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1581] "My Master, Sauron the Great, bids you welcome... he gives..."
    [TEXT_ID_MY_MASTER_SAURON_THE_GREAT_BIDS_YOU_WELC] =
        {.speakerPicture = DialogPortraitMouthOfSauronBgAsset},
    // [1582] "We do not come here to treat with Sauron, still less with..."
    [TEXT_ID_WE_DO_NOT_COME_HERE_TO_TREAT_WITH_SAURON] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1583] "You have failed, heir of Isildur. It takes more than a bl..."
    [TEXT_ID_YOU_HAVE_FAILED_HEIR_OF_ISILDUR_IT_TAKES] =
        {.speakerPicture = DialogPortraitMouthOfSauronBgAsset},
    // [1584] "The wall has been breached!"
    [TEXT_ID_THE_WALL_HAS_BEEN_BREACHED] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1585] "Saruman's forces are upon us! Drive them out!"
    [TEXT_ID_SARUMANS_FORCES_ARE_UPON_US_DRIVE_THEM_O] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1586] "The Orc captains must be stopped before they can rally mo..."
    [TEXT_ID_THE_ORC_CAPTAINS_MUST_BE_STOPPED_BEFORE] =
        {.speakerPicture = DialogPortraitGondorSoldierBgAsset},
    // [1587] "Thank you, good soldier. I do not wish for you to go unre..."
    [TEXT_ID_THANK_YOU_GOOD_SOLDIER_I_DO_NOT_WISH_FOR] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1588] "The battle was won in a glorious charge, my uncle shoutin..."
    [TEXT_ID_THE_BATTLE_WAS_WON_IN_A_GLORIOUS_CHARGE] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1589] "Eomer and Gandalf may have helped save Rohan, but there i..."
    [TEXT_ID_EOMER_AND_GANDALF_MAY_HAVE_HELPED_SAVE_R] =
        {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1590] "We must ride to Gondor."
    [TEXT_ID_WE_MUST_RIDE_TO_GONDOR] = {.speakerPicture = DialogPortraitEomerBgAsset},
    // [1591] "First we deal with a traitor."
    [TEXT_ID_FIRST_WE_DEAL_WITH_A_TRAITOR] = {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1592] "Saruman will pay dearly for this night."
    [TEXT_ID_SARUMAN_WILL_PAY_DEARLY_FOR_THIS_NIGHT] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1593] "Return to Edoras, niece."
    [TEXT_ID_RETURN_TO_EDORAS_NIECE] = {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1594] "But I can fight!"
    [TEXT_ID_BUT_I_CAN_FIGHT] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1595] "Not today. Helm's Deep is in ruin. Bring the wounded to E..."
    [TEXT_ID_NOT_TODAY_HELMS_DEEP_IS_IN_RUIN_BRING_TH] =
        {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1596] "Saruman is dead."
    [TEXT_ID_SARUMAN_IS_DEAD] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1597] "Aragorn... you startled me. What time is it?"
    [TEXT_ID_ARAGORN_YOU_STARTLED_ME_WHAT_TIME_IS_IT] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1598] "Not yet dawn. But we have seen the beacons of Minas Tirit..."
    [TEXT_ID_NOT_YET_DAWN_BUT_WE_HAVE_SEEN_THE_BEACON] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1599] "Thank goodness, a Rohirrim warrior! Help us, we are under..."
    [TEXT_ID_THANK_GOODNESS_A_ROHIRRIM_WARRIOR_HELP_U] =
        {.speakerPicture = DialogPortraitWomanBgAsset},
    // [1600] "Please, help us hold off the Orc hordes until allies arrive."
    [TEXT_ID_PLEASE_HELP_US_HOLD_OFF_THE_ORC_HORDES_U] =
        {.speakerPicture = DialogPortraitWomanBgAsset},
    // [1601] "Look, they retreat! Our allies have come!"
    [TEXT_ID_LOOK_THEY_RETREAT_OUR_ALLIES_HAVE_COME] =
        {.speakerPicture = DialogPortraitWomanBgAsset},
    // [1602] "My horses have been stolen! One was laden with all my wea..."
    [TEXT_ID_MY_HORSES_HAVE_BEEN_STOLEN_ONE_WAS_LADEN] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1603] "Please do not leave, they still attack!"
    [TEXT_ID_PLEASE_DO_NOT_LEAVE_THEY_STILL_ATTACK] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1604] "You wish to leave?  The hordes yet advance."
    [TEXT_ID_YOU_WISH_TO_LEAVE_THE_HORDES_YET_ADVANCE] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1605] "Eowyn... why did you come?"
    [TEXT_ID_EOWYN_WHY_DID_YOU_COME] = {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1606] "Do you not know?"
    [TEXT_ID_DO_YOU_NOT_KNOW] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1607] "It is only a shadow and a thought that you love, Eowyn. I..."
    [TEXT_ID_IT_IS_ONLY_A_SHADOW_AND_A_THOUGHT_THAT_Y] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1608] "Eomer will recognize you if spotted. We'll have to find a..."
    [TEXT_ID_EOMER_WILL_RECOGNIZE_YOU_IF_SPOTTED_WELL] =
        {.speakerPicture = DialogPortraitMerryBgAsset},
    // [1609] "You can't go there, Eowyn! Eomer will surely recognize you!"
    [TEXT_ID_YOU_CANT_GO_THERE_EOWYN_EOMER_WILL_SUREL] =
        {.speakerPicture = DialogPortraitMerryBgAsset},
    // [1610] "Let us find the source of this chaos. Careful, Men of Roh..."
    [TEXT_ID_LET_US_FIND_THE_SOURCE_OF_THIS_CHAOS_CAR] =
        {.speakerPicture = DialogPortraitEomerBgAsset},
    // [1611] "I hear that this river winds fast and that to linger too ..."
    [TEXT_ID_I_HEAR_THAT_THIS_RIVER_WINDS_FAST_AND_TH] =
        {.speakerPicture = DialogPortraitMerryBgAsset},
    // [1612] "Our scouts reported Sauron's Haradrim have herded Mumakil..."
    [TEXT_ID_OUR_SCOUTS_REPORTED_SAURONS_HARADRIM_HAV] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1613] "Where are you going, lady Eowyn?"
    [TEXT_ID_WHERE_ARE_YOU_GOING_LADY_EOWYN] = {.speakerPicture = DialogPortraitMerryBgAsset},
    // [1614] "To scout the Mumakil camps myself. Perhaps my eyes can pr..."
    [TEXT_ID_TO_SCOUT_THE_MUMAKIL_CAMPS_MYSELF_PERHAP] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1615] "50,000 Orcs at the base of the city! Courage, Merry. Cour..."
    [TEXT_ID_50_000_ORCS_AT_THE_BASE_OF_THE_CITY_COUR] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1616] "Forth now, fear no darkness! Arise, arise Riders of Theod..."
    [TEXT_ID_FORTH_NOW_FEAR_NO_DARKNESS_ARISE_ARISE_R] =
        {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1617] "Uncle Theoden!"
    [TEXT_ID_UNCLE_THEODEN] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1618] "Feast on his flesh!"
    [TEXT_ID_FEAST_ON_HIS_FLESH] = {.speakerPicture = DialogPortraitWitchKingBgAsset},
    // [1619] "Begone Lord of Carrion!"
    [TEXT_ID_BEGONE_LORD_OF_CARRION] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1620] "Do not come between the Nazgul and his prey!"
    [TEXT_ID_DO_NOT_COME_BETWEEN_THE_NAZGUL_AND_HIS_P] =
        {.speakerPicture = DialogPortraitWitchKingBgAsset},
    // [1621] "I will kill you if you touch him!"
    [TEXT_ID_I_WILL_KILL_YOU_IF_YOU_TOUCH_HIM] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1622] "Kill me? Thou fool. No living man can slay me!"
    [TEXT_ID_KILL_ME_THOU_FOOL_NO_LIVING_MAN_CAN_SLAY] =
        {.speakerPicture = DialogPortraitWitchKingBgAsset},
    // [1623] "I am no man! You look upon a woman!"
    [TEXT_ID_I_AM_NO_MAN_YOU_LOOK_UPON_A_WOMAN] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1624] "Mumakil threaten to flank Lord Theoden!"
    [TEXT_ID_MUMAKIL_THREATEN_TO_FLANK_LORD_THEODEN] =
        {.speakerPicture = DialogPortraitRohanSoldierBgAsset},
    // [1625] "If the Haradrim are not stopped, the Riders of Rohan will..."
    [TEXT_ID_IF_THE_HARADRIM_ARE_NOT_STOPPED_THE_RIDE] =
        {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1626] "Two more Oliphaunts are upon us!"
    [TEXT_ID_TWO_MORE_OLIPHAUNTS_ARE_UPON_US] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1627] "I know your face... Eowyn..."
    [TEXT_ID_I_KNOW_YOUR_FACE_EOWYN] = {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1628] "Do not die, Uncle. Do not leave me!"
    [TEXT_ID_DO_NOT_DIE_UNCLE_DO_NOT_LEAVE_ME] = {.speakerPicture = DialogPortraitEowynBgAsset},
    // [1629] "My body is broken... I go to my fathers. I shall not be a..."
    [TEXT_ID_MY_BODY_IS_BROKEN_I_GO_TO_MY_FATHERS_I_S] =
        {.speakerPicture = DialogPortraitTheodenBgAsset},
    // [1630] "Eowyn! Aragorn has arrived, and he has brought an army wi..."
    [TEXT_ID_EOWYN_ARAGORN_HAS_ARRIVED_AND_HE_HAS_BRO] =
        {.speakerPicture = DialogPortraitMerryBgAsset},
    // [1631] "I'm glad you've come, Gandalf! Hoom! Wood and water, stoc..."
    [TEXT_ID_IM_GLAD_YOUVE_COME_GANDALF_HOOM_WOOD_AND] =
        {.speakerPicture = DialogPortraitAragornBgAsset},
    // [1632] "Too long have you escaped the vengeance of Sauron! What d..."
    [TEXT_ID_TOO_LONG_HAVE_YOU_ESCAPED_THE_VENGEANCE_1632] =
        {.speakerPicture = DialogPortraitSarumanBgAsset},
    // [1633] "I have never coveted power, Saruman."
    [TEXT_ID_I_HAVE_NEVER_COVETED_POWER_SARUMAN] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1634] "Shall I kill him?"
    [TEXT_ID_SHALL_I_KILL_HIM] = {.speakerPicture = DialogPortraitLegolasBgAsset},
    // [1635] "No... I will deal with him."
    [TEXT_ID_NO_I_WILL_DEAL_WITH_HIM] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1637] "Perhaps I can persuade them from this foolishness."
    [TEXT_ID_PERHAPS_I_CAN_PERSUADE_THEM_FROM_THIS_FO] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1638] "Saruman! You are defeated!"
    [TEXT_ID_SARUMAN_YOU_ARE_DEFEATED] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1639] "I have enough strength left for you, Gandalf Greyhame!"
    [TEXT_ID_I_HAVE_ENOUGH_STRENGTH_LEFT_FOR_YOU_GAND] =
        {.speakerPicture = DialogPortraitSarumanBgAsset},
    // [1640] "Your power is broken. Come back, Saruman."
    [TEXT_ID_YOUR_POWER_IS_BROKEN_COME_BACK_SARUMAN] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1641] "Gibbets and crows! Save your pity and your mercy! I hate ..."
    [TEXT_ID_GIBBETS_AND_CROWS_SAVE_YOUR_PITY_AND_YOU] =
        {.speakerPicture = DialogPortraitSarumanBgAsset},
    // [1642] "I see Him! I can see him in my head!"
    [TEXT_ID_I_SEE_HIM_I_CAN_SEE_HIM_IN_MY_HEAD] = {.speakerPicture = DialogPortraitPippinBgAsset},
    // [1643] "Pippin! Fool of a Took! Understand, things are now in mot..."
    [TEXT_ID_PIPPIN_FOOL_OF_A_TOOK_UNDERSTAND_THINGS] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1644] "There is no time to linger. There is a secret route throu..."
    [TEXT_ID_THERE_IS_NO_TIME_TO_LINGER_THERE_IS_A_SE] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1645] "Minas Tirith! The greatest stronghold in Middle-earth."
    [TEXT_ID_MINAS_TIRITH_THE_GREATEST_STRONGHOLD_IN] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1646] "Nazgul!"
    [TEXT_ID_NAZGUL] = {.speakerPicture = DialogPortraitPippinBgAsset},
    // [1647] "Run, Pippin!"
    [TEXT_ID_RUN_PIPPIN] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1648] "The first wall of Minas Tirith has fallen! An iron monste..."
    [TEXT_ID_THE_FIRST_WALL_OF_MINAS_TIRITH_HAS_FALLE] =
        {.speakerPicture = DialogPortraitGondorSoldierBgAsset},
    // [1649] "The second gate shall not fall! To arms!"
    [TEXT_ID_THE_SECOND_GATE_SHALL_NOT_FALL_TO_ARMS] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1650] "Gandalf! Denethor has lost his mind! He's burning Faramir..."
    [TEXT_ID_GANDALF_DENETHOR_HAS_LOST_HIS_MIND_HES_B] =
        {.speakerPicture = DialogPortraitPippinBgAsset},
    // [1651] "Go back! Go back to the abyss! Fall into the nothingness ..."
    [TEXT_ID_GO_BACK_GO_BACK_TO_THE_ABYSS_FALL_INTO_T] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1652] "Do you not know death when you face it, old man? This is ..."
    [TEXT_ID_DO_YOU_NOT_KNOW_DEATH_WHEN_YOU_FACE_IT_O] =
        {.speakerPicture = DialogPortraitWitchKingBgAsset},
    // [1653] "Rohan has come upon the Pelennor Fields! But Denethor mus..."
    [TEXT_ID_ROHAN_HAS_COME_UPON_THE_PELENNOR_FIELDS] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1654] "We shall both go up in a great fire. Ash, ash, and smoke...."
    [TEXT_ID_WE_SHALL_BOTH_GO_UP_IN_A_GREAT_FIRE_ASH] =
        {.speakerPicture = DialogPortraitDenethorBgAsset},
    // [1655] "Stay this madness, Denethor! I shall not let you slay you..."
    [TEXT_ID_STAY_THIS_MADNESS_DENETHOR_I_SHALL_NOT_L] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1656] "10,000 Orcs at the base of my city!"
    [TEXT_ID_10_000_ORCS_AT_THE_BASE_OF_MY_CITY] =
        {.speakerPicture = DialogPortraitDenethorBgAsset},
    // [1657] "Look, Gandalf! The soldiers of Gondor have returned from ..."
    [TEXT_ID_LOOK_GANDALF_THE_SOLDIERS_OF_GONDOR_HAVE] =
        {.speakerPicture = DialogPortraitPippinBgAsset},
    // [1658] "Faramir is wounded! I must go to his aid, or else dark ti..."
    [TEXT_ID_FARAMIR_IS_WOUNDED_I_MUST_GO_TO_HIS_AID] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1659] "Mithrandir! The Orcs have crossed the river. Osgiliath ha..."
    [TEXT_ID_MITHRANDIR_THE_ORCS_HAVE_CROSSED_THE_RIV] =
        {.speakerPicture = DialogPortraitFaramirBgAsset},
    // [1660] "Peregrin Took, my lad, we have another opportunity for on..."
    [TEXT_ID_PEREGRIN_TOOK_MY_LAD_WE_HAVE_ANOTHER_OPP] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1661] "Ah, I see you fear not the fire's heat."
    [TEXT_ID_AH_I_SEE_YOU_FEAR_NOT_THE_FIRES_HEAT] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1662] "Many of Gondor's treasures have been thrown onto these py..."
    [TEXT_ID_MANY_OF_GONDORS_TREASURES_HAVE_BEEN_THRO] =
        {.speakerPicture = DialogPortraitVillagerBgAsset},
    // [1663] "Wake up! Wake up! Wake up, sleepies! We must go, yes, we ..."
    [TEXT_ID_WAKE_UP_WAKE_UP_WAKE_UP_SLEEPIES_WE_MUST] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1664] "Haven't you had any sleep, Mr. Frodo? It must be getting ..."
    [TEXT_ID_HAVENT_YOU_HAD_ANY_SLEEP_MR_FRODO_IT_MUS] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1665] "We should save what food we have, Sam. There's little hop..."
    [TEXT_ID_WE_SHOULD_SAVE_WHAT_FOOD_WE_HAVE_SAM_THE] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1666] "Hey stinker, go find us some more food! Can't you see Mr...."
    [TEXT_ID_HEY_STINKER_GO_FIND_US_SOME_MORE_FOOD_CA] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1667] "Clever hobbits to climb so high, very clever hobbits! Jus..."
    [TEXT_ID_CLEVER_HOBBITS_TO_CLIMB_SO_HIGH_VERY_CLE] =
        {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1668] "What are you up to? Sneaking off, are we?"
    [TEXT_ID_WHAT_ARE_YOU_UP_TO_SNEAKING_OFF_ARE_WE] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1669] "Sneaking? Fat Hobbit is always so polite. Smeagol shows t..."
    [TEXT_ID_SNEAKING_FAT_HOBBIT_IS_ALWAYS_SO_POLITE] =
        {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1670] "Alright, alright - you startled me was all. What were you..."
    [TEXT_ID_ALRIGHT_ALRIGHT_YOU_STARTLED_ME_WAS_ALL] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1671] "Sneaking."
    [TEXT_ID_SNEAKING] = {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1672] "Mr. Frodo?"
    [TEXT_ID_MR_FRODO] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1673] "I'm alright."
    [TEXT_ID_IM_ALRIGHT] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1674] "You are not alright! You're exhausted! It's that Gollum, ..."
    [TEXT_ID_YOU_ARE_NOT_ALRIGHT_YOURE_EXHAUSTED_ITS] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1675] "Get away from me!"
    [TEXT_ID_GET_AWAY_FROM_ME] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1676] "I don't want to keep it ... I just want to help!"
    [TEXT_ID_I_DONT_WANT_TO_KEEP_IT_I_JUST_WANT_TO_HE] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1677] "You can't have it! It's mine!"
    [TEXT_ID_YOU_CANT_HAVE_IT_ITS_MINE] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1678] "See! See! He wants it for himself!"
    [TEXT_ID_SEE_SEE_HE_WANTS_IT_FOR_HIMSELF] = {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1679] "No ... it's you, Sam. You can't help me anymore. I'm sorr..."
    [TEXT_ID_NO_ITS_YOU_SAM_YOU_CANT_HELP_ME_ANYMORE] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1680] "But ... Gollum's a liar ... he's poisoned you against me."
    [TEXT_ID_BUT_GOLLUMS_A_LIAR_HES_POISONED_YOU_AGAI] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1681] "Go home!"
    [TEXT_ID_GO_HOME] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1682] "Come on you filth! You will not touch him again! Come on ..."
    [TEXT_ID_COME_ON_YOU_FILTH_YOU_WILL_NOT_TOUCH_HIM] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1683] "Mr. Frodo! Don't leave me here alone! Wake up!"
    [TEXT_ID_MR_FRODO_DONT_LEAVE_ME_HERE_ALONE_WAKE_U] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1684] "Not asleep, dead. What do I do now. I have no choice. I'm..."
    [TEXT_ID_NOT_ASLEEP_DEAD_WHAT_DO_I_DO_NOW_I_HAVE] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1685] "A spy! Strip him!"
    [TEXT_ID_A_SPY_STRIP_HIM] = {.speakerPicture = DialogPortraitOrcBgAsset},
    // [1686] "Don't touch him, scum! Any trespassers found by the guard..."
    [TEXT_ID_DONT_TOUCH_HIM_SCUM_ANY_TRESPASSERS_FOUN] =
        {.speakerPicture = DialogPortraitShagratBgAsset},
    // [1687] "Why? What for? He's as dead as a doornail?"
    [TEXT_ID_WHY_WHAT_FOR_HES_AS_DEAD_AS_A_DOORNAIL] = {.speakerPicture = DialogPortraitOrcBgAsset},
    // [1688] "Get back, idiot! She doesn't eat dead meat. This fellow a..."
    [TEXT_ID_GET_BACK_IDIOT_SHE_DOESNT_EAT_DEAD_MEAT] =
        {.speakerPicture = DialogPortraitShagratBgAsset},
    // [1689] "Not dead... Samwise, you fool! Don't worry Mr. Frodo, I'l..."
    [TEXT_ID_NOT_DEAD_SAMWISE_YOU_FOOL_DONT_WORRY_MR] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1690] "A halfling!"
    [TEXT_ID_A_HALFLING] = {.speakerPicture = DialogPortraitOrcCaptainBgAsset},
    // [1691] "I'm gonna bleed you like a stuck pig!"
    [TEXT_ID_IM_GONNA_BLEED_YOU_LIKE_A_STUCK_PIG] = {.speakerPicture = DialogPortraitGorbagBgAsset},
    // [1692] "Not if I stick you first!"
    [TEXT_ID_NOT_IF_I_STICK_YOU_FIRST] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1693] "Sam!"
    [TEXT_ID_SAM_1693] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1694] "You didn't really think I'd leave you?"
    [TEXT_ID_YOU_DIDNT_REALLY_THINK_ID_LEAVE_YOU] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1695] "I'm sorry Sam, sorry for everything. It's too late. It's ..."
    [TEXT_ID_IM_SORRY_SAM_SORRY_FOR_EVERYTHING_ITS_TO] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1696] "Begging your pardon, but they haven't. Back on the stairs..."
    [TEXT_ID_BEGGING_YOUR_PARDON_BUT_THEY_HAVENT_BACK] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1697] "Give it to me. Give me the Ring, Sam. The Ring is my burd..."
    [TEXT_ID_GIVE_IT_TO_ME_GIVE_ME_THE_RING_SAM_THE_R] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1698] "And I will keep your wretched stinger as payment for poor..."
    [TEXT_ID_AND_I_WILL_KEEP_YOUR_WRETCHED_STINGER_AS] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1699] "Frodo! Destroy it, now, do it! Throw it in the fire!"
    [TEXT_ID_FRODO_DESTROY_IT_NOW_DO_IT_THROW_IT_IN_T] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1700] "I have made my choice. I will not destroy it... the Ring ..."
    [TEXT_ID_I_HAVE_MADE_MY_CHOICE_I_WILL_NOT_DESTROY] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1701] "Precious! My Precious! Filthy Baggins!"
    [TEXT_ID_PRECIOUS_MY_PRECIOUS_FILTHY_BAGGINS] = {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1702] "Noooo!"
    [TEXT_ID_NOOOO] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1703] "Sam! It's gone... it is done."
    [TEXT_ID_SAM_ITS_GONE_IT_IS_DONE] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1704] "Yes, Mr. Frodo. It's over now."
    [TEXT_ID_YES_MR_FRODO_ITS_OVER_NOW] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1705] "I'm glad to be with you, Samwise Gamgee... here at the en..."
    [TEXT_ID_IM_GLAD_TO_BE_WITH_YOU_SAMWISE_GAMGEE_HE] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1706] "And, in a strange twist of fate, Smeagol threw Frodo into..."
    [TEXT_ID_AND_IN_A_STRANGE_TWIST_OF_FATE_SMEAGOL_T] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1707] "Look Sam, we've arrived."
    [TEXT_ID_LOOK_SAM_WEVE_ARRIVED] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1708] "Mr. Frodo, move quickly before our way is blocked!"
    [TEXT_ID_MR_FRODO_MOVE_QUICKLY_BEFORE_OUR_WAY_IS] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1709] "We missed it. We'll have to find another way, Mr. Frodo."
    [TEXT_ID_WE_MISSED_IT_WELL_HAVE_TO_FIND_ANOTHER_W] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1710] "The bridge is out...We'll march around."
    [TEXT_ID_THE_BRIDGE_IS_OUT_WELL_MARCH_AROUND] =
        {.speakerPicture = DialogPortraitOrcCaptainBgAsset},
    // [1711] "Enough out of you! MARCH!"
    [TEXT_ID_ENOUGH_OUT_OF_YOU_MARCH] = {.speakerPicture = DialogPortraitOrcCaptainBgAsset},
    // [1712] "Good Smeagol catch a rat for Master, yes, yessss..."
    [TEXT_ID_GOOD_SMEAGOL_CATCH_A_RAT_FOR_MASTER_YES] =
        {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1713] "Rats is very tasty, mmmmm."
    [TEXT_ID_RATS_IS_VERY_TASTY_MMMMM] = {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1714] "I don't like the smell of this place one bit."
    [TEXT_ID_I_DONT_LIKE_THE_SMELL_OF_THIS_PLACE_ONE] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1715] "They're surrounding us!"
    [TEXT_ID_THEYRE_SURROUNDING_US] = {.speakerPicture = DialogPortraitSamBgAsset},
    // [1716] "I wonder where that filthy Gollum is off to?"
    [TEXT_ID_I_WONDER_WHERE_THAT_FILTHY_GOLLUM_IS_OFF] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1717] "What have you got there, eh?"
    [TEXT_ID_WHAT_HAVE_YOU_GOT_THERE_EH] = {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1718] "Nothing... just a skull."
    [TEXT_ID_NOTHING_JUST_A_SKULL] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1719] "Stupid skull. Thief! Deagol, my precious."
    [TEXT_ID_STUPID_SKULL_THIEF_DEAGOL_MY_PRECIOUS] =
        {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1720] "Curse them! We hates them! Nassty hobbitses! Wicked, tric..."
    [TEXT_ID_CURSE_THEM_WE_HATES_THEM_NASSTY_HOBBITSE] =
        {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1721] "Let HER deal with them... she must eat. All She gets is f..."
    [TEXT_ID_LET_HER_DEAL_WITH_THEM_SHE_MUST_EAT_ALL] =
        {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1722] "Frodo! I heard it from his own mouth ... He means to murd..."
    [TEXT_ID_FRODO_I_HEARD_IT_FROM_HIS_OWN_MOUTH_HE_M] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1723] "Never! Smeagol wouldn't hurt a fly! He's a fat, horrid Ho..."
    [TEXT_ID_NEVER_SMEAGOL_WOULDNT_HURT_A_FLY_HES_A_F] =
        {.speakerPicture = DialogPortraitSmeagolBgAsset},
    // [1724] "You miserable little maggot! I'm going to stove your head..."
    [TEXT_ID_YOU_MISERABLE_LITTLE_MAGGOT_IM_GOING_TO] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1725] "Leave him alone! If you scare him off, we are lost."
    [TEXT_ID_LEAVE_HIM_ALONE_IF_YOU_SCARE_HIM_OFF_WE] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1726] "You don't see it, he's leading us into a trap! Gandalf wo..."
    [TEXT_ID_YOU_DONT_SEE_IT_HES_LEADING_US_INTO_A_TR] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1727] "Gandalf is gone. Come, Smeagol, show us the way."
    [TEXT_ID_GANDALF_IS_GONE_COME_SMEAGOL_SHOW_US_THE] =
        {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1728] "Captain, looks what we found!"
    [TEXT_ID_CAPTAIN_LOOKS_WHAT_WE_FOUND] = {.speakerPicture = DialogPortraitOrcBgAsset},
    // [1729] "Please, I'm just a miner who's strayed from the rails..."
    [TEXT_ID_PLEASE_IM_JUST_A_MINER_WHOS_STRAYED_FROM] =
        {.speakerPicture = DialogPortraitElderBgAsset},
    // [1730] "Alright, move you slugs!"
    [TEXT_ID_ALRIGHT_MOVE_YOU_SLUGS] = {.speakerPicture = DialogPortraitShagratBgAsset},
    // [1731] "It's too dark Master Frodo. Let's find another way."
    [TEXT_ID_ITS_TOO_DARK_MASTER_FRODO_LETS_FIND_ANOT] =
        {.speakerPicture = DialogPortraitSamBgAsset},
    // [1732] "Is this the only way?"
    [TEXT_ID_IS_THIS_THE_ONLY_WAY] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1733] "Yes, Precious. We must go in. Oh yes, right inside. It's ..."
    [TEXT_ID_YES_PRECIOUS_WE_MUST_GO_IN_OH_YES_RIGHT] =
        {.speakerPicture = DialogPortraitGollumBgAsset},
    // [1734] "Smeagol? What was that? Smeagol?"
    [TEXT_ID_SMEAGOL_WHAT_WAS_THAT_SMEAGOL] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1735] "Sam! Sam!"
    [TEXT_ID_SAM_SAM] = {.speakerPicture = DialogPortraitFrodoBgAsset},
    // [1736] "The power of the Ring is destroyed, and all that was done..."
    [TEXT_ID_THE_POWER_OF_THE_RING_IS_DESTROYED_AND_A] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1737] "Now come the days of the King..."
    [TEXT_ID_NOW_COME_THE_DAYS_OF_THE_KING] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1738] "And the Hobbits arrived back in Hobbiton thirteen months ..."
    [TEXT_ID_AND_THE_HOBBITS_ARRIVED_BACK_IN_HOBBITON] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1739] "To the Havens, Bilbo. We are to take the ship that leaves..."
    [TEXT_ID_TO_THE_HAVENS_BILBO_WE_ARE_TO_TAKE_THE_S] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1740] "..."
    [TEXT_ID_UNNAMED_1740] = {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1741] "...and Eowyn was honored as a Shieldmaiden of Rohan. She ..."
    [TEXT_ID_AND_EOWYN_WAS_HONORED_AS_A_SHIELDMAIDEN] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1742] "...and Legolas was celebrated as a great hero, and though..."
    [TEXT_ID_AND_LEGOLAS_WAS_CELEBRATED_AS_A_GREAT_HE] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1743] "...and Gimli was received as a great Dwarf Lord, but more..."
    [TEXT_ID_AND_GIMLI_WAS_RECEIVED_AS_A_GREAT_DWARF] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1744] "...and Aragorn was received with great cheers in the wall..."
    [TEXT_ID_AND_ARAGORN_WAS_RECEIVED_WITH_GREAT_CHEE] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1745] "...and Frodo was reunited with his friends thirteen month..."
    [TEXT_ID_AND_FRODO_WAS_REUNITED_WITH_HIS_FRIENDS] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1746] "...and Gandalf met the Hobbits with great hugs and smiles..."
    [TEXT_ID_AND_GANDALF_MET_THE_HOBBITS_WITH_GREAT_H] =
        {.speakerPicture = DialogPortraitGandalfBgAsset},
    // [1810] "GRIPTONITE GAMES"
    [TEXT_ID_GRIPTONITE_GAMES] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_HEADER]},
    // [1812] "-Executive Producer-"
    [TEXT_ID_EXECUTIVE_PRODUCER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1815] "-Producer / Lead Designer-"
    [TEXT_ID_PRODUCER_LEAD_DESIGNER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1818] "-Lead Programmers-"
    [TEXT_ID_LEAD_PROGRAMMERS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1822] "-Art Lead-"
    [TEXT_ID_ART_LEAD] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1825] "-Programmers-"
    [TEXT_ID_PROGRAMMERS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1834] "-Art and Animation-"
    [TEXT_ID_ART_AND_ANIMATION] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1840] "-Background Art-"
    [TEXT_ID_BACKGROUND_ART] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1843] "-Composer and Sound Design-"
    [TEXT_ID_COMPOSER_AND_SOUND_DESIGN] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1846] "-Level Design-"
    [TEXT_ID_LEVEL_DESIGN] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1852] "-Additional Art-"
    [TEXT_ID_ADDITIONAL_ART] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1862] "-Lead Tester-"
    [TEXT_ID_LEAD_TESTER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1865] "-Software Testers-"
    [TEXT_ID_SOFTWARE_TESTERS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1874] "-Special Thanks-"
    [TEXT_ID_SPECIAL_THANKS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1897] "ELECTRONIC ARTS"
    [TEXT_ID_ELECTRONIC_ARTS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_HEADER]},
    // [1899] "-Producer-"
    [TEXT_ID_PRODUCER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1902] "-Design-"
    [TEXT_ID_DESIGN] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1905] "-Lead Tester-"
    [TEXT_ID_LEAD_TESTER_1905] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1908] "-Assistant Lead Tester-"
    [TEXT_ID_ASSISTANT_LEAD_TESTER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1911] "-Core Testers-"
    [TEXT_ID_CORE_TESTERS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1919] "-Product Manager-"
    [TEXT_ID_PRODUCT_MANAGER] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1922] "-Package Design-"
    [TEXT_ID_PACKAGE_DESIGN] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1926] "-Package Project Management-"
    [TEXT_ID_PACKAGE_PROJECT_MANAGEMENT] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1929] "-Documentation-"
    [TEXT_ID_DOCUMENTATION] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1932] "-Documentation Layout-"
    [TEXT_ID_DOCUMENTATION_LAYOUT] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1935] "-Customer Quality Control-"
    [TEXT_ID_CUSTOMER_QUALITY_CONTROL] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1945] "-International Project Coordinator-"
    [TEXT_ID_INTERNATIONAL_PROJECT_COORDINATOR] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1948] "-Sr. Manager Localization-"
    [TEXT_ID_SR_MANAGER_LOCALIZATION] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1951] "-Localization-"
    [TEXT_ID_LOCALIZATION] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1961] "-LIT Supervisor-"
    [TEXT_ID_LIT_SUPERVISOR] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1964] "-LIT Test Lead-"
    [TEXT_ID_LIT_TEST_LEAD] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1967] "-LIT Core Testers-"
    [TEXT_ID_LIT_CORE_TESTERS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1973] "-Electronic Arts Special Thanks-"
    [TEXT_ID_ELECTRONIC_ARTS_SPECIAL_THANKS] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1990] "AMAZE ENTERTAINMENT"
    [TEXT_ID_AMAZE_ENTERTAINMENT] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_HEADER]},
    // [1992] "-Executive Management-"
    [TEXT_ID_EXECUTIVE_MANAGEMENT] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1996] "-Director of Development Services-"
    [TEXT_ID_DIRECTOR_OF_DEVELOPMENT_SERVICES] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [1999] "-Creative Director-"
    [TEXT_ID_CREATIVE_DIRECTOR] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2002] "-Director of Design-"
    [TEXT_ID_DIRECTOR_OF_DESIGN] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2005] "-Director of Operations and Finance-"
    [TEXT_ID_DIRECTOR_OF_OPERATIONS_AND_FINANCE] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2008] "-Director of Marketing-"
    [TEXT_ID_DIRECTOR_OF_MARKETING] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2011] "-Executive Studio Director, KnowWonder-"
    [TEXT_ID_EXECUTIVE_STUDIO_DIRECTOR_KNOWWONDER] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2014] "-Executive Studio Director, Adrenium Games-"
    [TEXT_ID_EXECUTIVE_STUDIO_DIRECTOR_ADRENIUM_GAMES] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2017] "-Studio Director, The Fizz Factor-"
    [TEXT_ID_STUDIO_DIRECTOR_THE_FIZZ_FACTOR] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2020] "-Executive Studio Director, BlackShip Games-"
    [TEXT_ID_EXECUTIVE_STUDIO_DIRECTOR_BLACKSHIP_GAME] =
        {.slot0Value = &CreditsLineKinds[CREDITS_LINE_NAME]},
    // [2021] "Scott K. Tsumura"
    [TEXT_ID_SCOTT_K_TSUMURA] = {.slot0Value = &CreditsLineKinds[CREDITS_LINE_LAST]},
};
// clang-format on
