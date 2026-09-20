#include "skill.h"
#include "types.h"

// clang-format off

/** @romaddress 0x0806e94c */
const ActiveSkill ActiveSkillsFrodo[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Knife Toss (skill_active_frodoKnifeToss)
    {
        .callback = skill_active_frodoKnifeToss,
        .spiritCost = 25,
        .field_0x06 = 0,
        .values =
        {
            { .base = 8, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 1, 0, 1, 0, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 5,
        .field_0x24 = 0xff,
    },
    // [1] Snare (skill_active_frodoSnare)
    {
        .callback = skill_active_frodoSnare,
        .spiritCost = 35,
        .field_0x06 = 0,
        .values =
        {
            { .base = 5, .perLevel = { 3, 3, 3, 3, 3 } },
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 2,
        .field_0x24 = 0xff,
    },
    // [2] Galadriel's Cloak (skill_active_frodoGaladrielsCloak)
    {
        .callback = skill_active_frodoGaladrielsCloak,
        .spiritCost = 40,
        .field_0x06 = 0,
        .values =
        {
            { .base = 60, .perLevel = { 60, 60, 60, 60, 90 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] The One Ring (skill_active_frodoTheOneRing)
    {
        .callback = skill_active_frodoTheOneRing,
        .spiritCost = 75,
        .field_0x06 = 0,
        .values =
        {
            { .base = 5, .perLevel = { 2, 2, 2, 1, 1 } },
            { .base = 150, .perLevel = { 0, 60, 60, 60, 60 } },
        },
        .field_0x20 = 1,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Ring's Persuasion (skill_active_frodoRingsPersuasion)
    {
        .callback = skill_active_frodoRingsPersuasion,
        .spiritCost = 150,
        .field_0x06 = 0,
        .values =
        {
            { .base = 10, .perLevel = { 5, 5, 5, 5, 5 } },
            { .base = 300, .perLevel = { 150, 150, 150, 150, 150 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 1,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806ea3c */
const u8 HeroSkillIdsFrodo[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_FEARLESS, PASSIVE_SKILL_NIMBLE, PASSIVE_SKILL_LUCK, PASSIVE_SKILL_WOODSMAN, PASSIVE_SKILL_IRON_WILL,
    PASSIVE_SKILL_HARDY, PASSIVE_SKILL_WISE, PASSIVE_SKILL_BATTLE_SCARRED, PASSIVE_SKILL_THE_PRECIOUS,
};

/** @romaddress 0x0806ea48 */
const ActiveSkill ActiveSkillsSam[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Knife Toss (skill_active_frodoKnifeToss)
    {
        .callback = skill_active_frodoKnifeToss,
        .spiritCost = 25,
        .field_0x06 = 0,
        .values =
        {
            { .base = 8, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 1, 0, 1, 0, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 5,
        .field_0x24 = 0xff,
    },
    // [1] Snare (skill_active_frodoSnare)
    {
        .callback = skill_active_frodoSnare,
        .spiritCost = 35,
        .field_0x06 = 0,
        .values =
        {
            { .base = 5, .perLevel = { 3, 3, 3, 3, 5 } },
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 2,
        .field_0x24 = 0xff,
    },
    // [2] Galadriel's Cloak (skill_active_frodoGaladrielsCloak)
    {
        .callback = skill_active_frodoGaladrielsCloak,
        .spiritCost = 40,
        .field_0x06 = 0,
        .values =
        {
            { .base = 60, .perLevel = { 60, 60, 60, 60, 15 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Cookpot Smash (skill_active_samCookpotSmash)
    {
        .callback = skill_active_samCookpotSmash,
        .spiritCost = 60,
        .field_0x06 = 0,
        .values =
        {
            { .base = 3, .perLevel = { 5, 5, 5, 5, 5 } },
            { .base = 0, .perLevel = { 30, 30, 30, 30, 30 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Samwise the Strong (skill_active_samSamwiseTheStrong)
    {
        .callback = skill_active_samSamwiseTheStrong,
        .spiritCost = 85,
        .field_0x06 = 0,
        .values =
        {
            { .base = 30, .perLevel = { 10, 10, 10, 10, 10 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 1,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806eb38 */
const u8 HeroSkillIdsSam[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_FEARLESS, PASSIVE_SKILL_NIMBLE, PASSIVE_SKILL_LUCK, PASSIVE_SKILL_WOODSMAN, PASSIVE_SKILL_IRON_WILL,
    PASSIVE_SKILL_HARDY, PASSIVE_SKILL_WISE, PASSIVE_SKILL_SPIRIT_OF_MIDDLE_EARTH, PASSIVE_SKILL_DEFENDERS_FURY,
};

/** @romaddress 0x0806eb44 */
const ActiveSkill ActiveSkillsSmeagol[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Berserk Attack (skill_active_smeagolBerserkAttack)
    {
        .callback = skill_active_smeagolBerserkAttack,
        .spiritCost = 35,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 4, 4, 4, 4, 4 } },
            { .base = 0, .perLevel = { 1, 0, 1, 0, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [1] Rock Throw (skill_active_smeagolRockThrow)
    {
        .callback = skill_active_smeagolRockThrow,
        .spiritCost = 15,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 3, 3, 3, 3, 3 } },
            { .base = 0, .perLevel = { 1, 0, 1, 0, 1 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 4,
        .field_0x24 = 0xff,
    },
    // [2] Cower (skill_active_smeagolCower)
    {
        .callback = skill_active_smeagolCower,
        .spiritCost = 25,
        .field_0x06 = 0,
        .values =
        {
            { .base = 45, .perLevel = { 15, 15, 15, 15, 15 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Gollum! (skill_active_smeagolGollum)
    {
        .callback = skill_active_smeagolGollum,
        .spiritCost = 80,
        .field_0x06 = 0,
        .values =
        {
            { .base = 300, .perLevel = { 60, 60, 60, 60, 60 } },
            { .base = 0, .perLevel = { 5, 5, 5, 5, 5 } },
        },
        .field_0x20 = 4,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Pitiful Wail (skill_active_smeagolPitifulWail)
    {
        .callback = skill_active_smeagolPitifulWail,
        .spiritCost = 50,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 10, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 60, 60, 60, 60, 60 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 1,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806ec34 */
const u8 HeroSkillIdsSmeagol[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_ORCSLAYER, PASSIVE_SKILL_NIMBLE, PASSIVE_SKILL_WHY_DOES_IT_HURT_SMEAGOL, PASSIVE_SKILL_DIRTY_CLAWS, PASSIVE_SKILL_LUCK,
    PASSIVE_SKILL_IRON_WILL, PASSIVE_SKILL_FLEET_OF_FOOT, PASSIVE_SKILL_BERSERKER, PASSIVE_SKILL_THE_PRECIOUS,
};

/** @romaddress 0x0806ec40 */
const ActiveSkill ActiveSkillsLegolas[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] White Knives (skill_active_legolasWhiteKnives)
    {
        .callback = skill_active_legolasWhiteKnives,
        .spiritCost = 25,
        .field_0x06 = 0,
        .values =
        {
            { .base = 7, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [1] Spread Fire (skill_active_legolasSpreadFire)
    {
        .callback = skill_active_legolasSpreadFire,
        .spiritCost = 90,
        .field_0x06 = 0,
        .values =
        {
            { .base = 1, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 4,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 9,
        .field_0x24 = 0xff,
    },
    // [2] Friend of Mirkwood (skill_active_legolasFriendOfMirkwood)
    {
        .callback = skill_active_legolasFriendOfMirkwood,
        .spiritCost = 80,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 3, .perLevel = { 3, 3, 3, 3, 3 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Foraging (skill_active_legolasForaging)
    {
        .callback = skill_active_legolasForaging,
        .spiritCost = 95,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 15, 15, 15, 15, 15 } },
            { .base = 0, .perLevel = { 10, 10, 10, 10, 10 } },
        },
        .field_0x20 = 4,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Silent Stride (skill_active_legolasSilentStride)
    {
        .callback = skill_active_legolasSilentStride,
        .spiritCost = 60,
        .field_0x06 = 0,
        .values =
        {
            { .base = 75, .perLevel = { 20, 20, 20, 20, 20 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 0,
        .castFlags = { 0 },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806ed30 */
const u8 HeroSkillIdsLegolas[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_ACCURACY, PASSIVE_SKILL_HERB_LORE, PASSIVE_SKILL_RANGEMASTER, PASSIVE_SKILL_ORCSLAYER, PASSIVE_SKILL_WOODSMAN,
    PASSIVE_SKILL_FLEET_OF_FOOT, PASSIVE_SKILL_ARROW_PARRY, PASSIVE_SKILL_GALADRIELS_BLESSING, PASSIVE_SKILL_ARCHER_OF_MIRKWOOD,
};

/** @romaddress 0x0806ed3c */
const ActiveSkill ActiveSkillsAragorn[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Sweep (skill_active_aragornSweep)
    {
        .callback = skill_active_aragornSweep,
        .spiritCost = 45,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [1] King's Command (skill_active_aragornKingsCommand)
    {
        .callback = skill_active_aragornKingsCommand,
        .spiritCost = 65,
        .field_0x06 = 0,
        .values =
        {
            { .base = 90, .perLevel = { 15, 15, 15, 15, 15 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 8,
        .field_0x24 = 0xff,
    },
    // [2] Sword Throw (skill_active_aragornSwordThrow)
    {
        .callback = skill_active_aragornSwordThrow,
        .spiritCost = 40,
        .field_0x06 = 0,
        .values =
        {
            { .base = 10, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 8,
        .field_0x24 = 0xff,
    },
    // [3] Numenorean Will (skill_active_aragornNumenoreanWill)
    {
        .callback = skill_active_aragornNumenoreanWill,
        .spiritCost = 80,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 3, 3, 3, 3, 3 } },
        },
        .field_0x20 = 5,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Call of the Dead (skill_active_aragornCallOfTheDead)
    {
        .callback = skill_active_aragornCallOfTheDead,
        .spiritCost = 155,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 10, .perLevel = { 5, 5, 5, 5, 5 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 0,
        .castFlags = { 0 },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806ee2c */
const u8 HeroSkillIdsAragorn[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_FEARLESS, PASSIVE_SKILL_DEATH_STRIKE, PASSIVE_SKILL_BLADEMASTER, PASSIVE_SKILL_HERB_LORE, PASSIVE_SKILL_IRON_WILL,
    PASSIVE_SKILL_FIGHTERS_RESOLVE, PASSIVE_SKILL_HARDY, PASSIVE_SKILL_ARROW_PARRY, PASSIVE_SKILL_RAGE_OF_THE_NORTH,
};

/** @romaddress 0x0806ee38 */
const ActiveSkill ActiveSkillsGandalf[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Sword of Power (skill_active_gandalfSwordOfPower)
    {
        .callback = skill_active_gandalfSwordOfPower,
        .spiritCost = 50,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 3, 3, 3, 3, 3 } },
            { .base = 0, .perLevel = { 270, 270, 270, 270, 270 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [1] Lightstrike (skill_active_gandalfLightstrike)
    {
        .callback = skill_active_gandalfLightstrike,
        .spiritCost = 60,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 12, 12, 12, 12, 12 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 8,
        .field_0x24 = 0xff,
    },
    // [2] Shield (skill_active_gandalfShield)
    {
        .callback = skill_active_gandalfShield,
        .spiritCost = 80,
        .field_0x06 = 0,
        .values =
        {
            { .base = 30, .perLevel = { 25, 20, 25, 20, 25 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Blinding Aura (skill_active_gandalfBlindingAura)
    {
        .callback = skill_active_gandalfBlindingAura,
        .spiritCost = 75,
        .field_0x06 = 0,
        .values =
        {
            { .base = 10, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 90, .perLevel = { 60, 60, 60, 60, 60 } },
        },
        .field_0x20 = 5,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Summon Gwaihir (skill_active_gandalfSummonGwaihir)
    {
        .callback = skill_active_gandalfSummonGwaihir,
        .spiritCost = 150,
        .field_0x06 = 0,
        .values =
        {
            { .base = 10, .perLevel = { 6, 6, 6, 6, 6 } },
            { .base = 0, .perLevel = { 3, 1, 1, 1, 1 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 1,
        .castFlags = { 0 },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806ef28 */
const u8 HeroSkillIdsGandalf[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_BLADEMASTER, PASSIVE_SKILL_KEEN_EYES, PASSIVE_SKILL_HERB_LORE, PASSIVE_SKILL_LUCK, PASSIVE_SKILL_SPIRIT_OF_MIDDLE_EARTH,
    PASSIVE_SKILL_WISE, PASSIVE_SKILL_LAST_STAND, PASSIVE_SKILL_WISDOM_OF_THE_AGES, PASSIVE_SKILL_SERVANT_OF_THE_SECRET_FIRE,
};

/** @romaddress 0x0806ef34 */
const ActiveSkill ActiveSkillsEowyn[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Double Strike (skill_active_eowynDoubleStrike)
    {
        .callback = skill_active_eowynDoubleStrike,
        .spiritCost = 30,
        .field_0x06 = 0,
        .values =
        {
            { .base = 2, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [1] Shieldmaiden of Rohan (skill_active_eowynShieldmaidenOfRohan)
    {
        .callback = skill_active_eowynShieldmaidenOfRohan,
        .spiritCost = 60,
        .field_0x06 = 0,
        .values =
        {
            { .base = 300, .perLevel = { 60, 60, 60, 60, 60 } },
            { .base = 15, .perLevel = { 3, 3, 3, 3, 3 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [2] Rohan Sprint (skill_active_eowynRohanSprint)
    {
        .callback = skill_active_eowynRohanSprint,
        .spiritCost = 65,
        .field_0x06 = 0,
        .values =
        {
            { .base = 50, .perLevel = { 0, 10, 10, 10, 10 } },
            { .base = 0, .perLevel = { 1, 1, 1, 1, 1 } },
        },
        .field_0x20 = 5,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Shield Bash (skill_active_eowynShieldBash)
    {
        .callback = skill_active_eowynShieldBash,
        .spiritCost = 50,
        .field_0x06 = 0,
        .values =
        {
            { .base = 12, .perLevel = { 1, 1, 1, 1, 1 } },
            { .base = 0, .perLevel = { 30, 40, 50, 60, 70 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Forth Eorlingas! (skill_active_eowynForthEorlingas)
    {
        .callback = skill_active_eowynForthEorlingas,
        .spiritCost = 85,
        .field_0x06 = 0,
        .values =
        {
            { .base = 30, .perLevel = { 10, 10, 10, 10, 10 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 1,
        .castFlags = { 0 },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806f024 */
const u8 HeroSkillIdsEowyn[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_NIMBLE, PASSIVE_SKILL_HERB_LORE, PASSIVE_SKILL_KEEN_EYES, PASSIVE_SKILL_FIGHTERS_RESOLVE, PASSIVE_SKILL_SHIELD_OFFENSE,
    PASSIVE_SKILL_FLEET_OF_FOOT, PASSIVE_SKILL_WISE, PASSIVE_SKILL_WRAITHSLAYER, PASSIVE_SKILL_DEFENDERS_FURY,
};

/** @romaddress 0x0806f030 */
const ActiveSkill ActiveSkillsGimli[HERO_ACTIVE_SKILL_COUNT] = {
    // [0] Axe Throw (skill_active_gimliAxeThrow)
    {
        .callback = skill_active_gimliAxeThrow,
        .spiritCost = 50,
        .field_0x06 = 0,
        .values =
        {
            { .base = 12, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { 0 },
        .field_0x22 = 5,
        .castTriggerFrame = 8,
        .field_0x24 = 0xff,
    },
    // [1] Whirling Attack (skill_active_gimliWhirlingAttack)
    {
        .callback = skill_active_gimliWhirlingAttack,
        .spiritCost = 75,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 2, 2, 2, 2, 2 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .keepCallbackState = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [2] Dwarven Rage (skill_active_gimliDwarvenRage)
    {
        .callback = skill_active_gimliDwarvenRage,
        .spiritCost = 65,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 2,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [3] Stoicism (skill_active_gimliStoicism)
    {
        .callback = skill_active_gimliStoicism,
        .spiritCost = 85,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 30, 10, 10, 10, 10 } },
            { .base = 90, .perLevel = { 60, 60, 60, 60, 60 } },
        },
        .field_0x20 = 5,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [4] Earth Shatter (skill_active_gimliEarthShatter)
    {
        .callback = skill_active_gimliEarthShatter,
        .spiritCost = 120,
        .field_0x06 = 0,
        .values =
        {
            { .base = 20, .perLevel = { 10, 10, 10, 10, 10 } },
            { .base = 0, .perLevel = { 10, 10, 10, 10, 10 } },
        },
        .field_0x20 = 12,
        .castFlags = { .d = { .forcePose0 = 1 } },
        .field_0x22 = 5,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
    // [5] Herbal Healing (skill_active_herbalHealing)
    {
        .callback = skill_active_herbalHealing,
        .spiritCost = 0,
        .field_0x06 = 0,
        .values =
        {
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
            { .base = 0, .perLevel = { 0, 0, 0, 0, 0 } },
        },
        .field_0x20 = 0,
        .castFlags = { 0 },
        .field_0x22 = 1,
        .castTriggerFrame = 0,
        .field_0x24 = 0xff,
    },
};

/** @romaddress 0x0806f120 */
const u8 HeroSkillIdsGimli[HERO_PASSIVE_SKILL_COUNT] = {
    PASSIVE_SKILL_DEATH_STRIKE, PASSIVE_SKILL_AXEMASTER, PASSIVE_SKILL_KEEN_EYES, PASSIVE_SKILL_ORCSLAYER, PASSIVE_SKILL_DWARF_SENSE,
    PASSIVE_SKILL_HARDY, PASSIVE_SKILL_BERSERKER, PASSIVE_SKILL_BATTLE_SCARRED, PASSIVE_SKILL_GLOINS_DOUBLE_AXES,
};
// clang-format on
