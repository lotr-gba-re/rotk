#include "types.h"

// clang-format off

/**
 * Hero level required to buy each active skill level. Rows index
 * Player.activeSkills and columns index activeSkillLevelsPurchased. A value of -1
 * means no further level can be purchased at this count.
 * Hero blocks follow ROM order, not HeroId order.
 *
 * @romaddress 0x0828758e
 */
const s8 ActiveSkillRequiredLevels[8][6][6] = {
    // [0] Frodo (HERO_ID_FRODO)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Knife Toss
        {  2,  4,  6,  8, 10, -1 }, // [1] Snare
        {  2,  4,  6,  8, 10, -1 }, // [2] Galadriel's Cloak
        {  1,  3,  7, 11, 15, -1 }, // [3] The One Ring
        { 12, 14, 16, 18, 20, -1 }, // [4] Ring's Persuasion
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [1] Sam (HERO_ID_SAM)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Knife Toss
        {  2,  4,  6,  8, 10, -1 }, // [1] Snare
        {  2,  4,  6,  8, 10, -1 }, // [2] Galadriel's Cloak
        {  2,  4,  6,  8, 10, -1 }, // [3] Cookpot Smash
        { 12, 14, 16, 18, 20, -1 }, // [4] Samwise the Strong
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [2] Smeagol (HERO_ID_SMEAGOL)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Berserk Attack
        {  2,  4,  6,  8, 10, -1 }, // [1] Rock Throw
        {  2,  4,  6,  8, 10, -1 }, // [2] Cower
        {  4,  6,  8, 10, 12, -1 }, // [3] Gollum!
        { 12, 14, 16, 18, 20, -1 }, // [4] Pitiful Wail
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [3] Legolas (HERO_ID_LEGOLAS)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] White Knives
        {  4,  7, 10, 13, 16, -1 }, // [1] Spread Fire
        {  2,  4,  6,  8, 10, -1 }, // [2] Friend of Mirkwood
        {  4,  6,  8, 10, 12, -1 }, // [3] Foraging
        { 12, 14, 16, 18, 20, -1 }, // [4] Silent Stride
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [4] Aragorn (HERO_ID_ARAGORN)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Sweep
        {  2,  4,  6,  8, 10, -1 }, // [1] King's Command
        {  2,  4,  6,  8, 10, -1 }, // [2] Sword Throw
        {  5,  7,  9, 11, 13, -1 }, // [3] Numenorean Will
        { 12, 14, 16, 18, 20, -1 }, // [4] Call of the Dead
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [5] Gandalf (HERO_ID_GANDALF)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Sword of Power
        {  2,  5,  8, 11, 14, -1 }, // [1] Lightstrike
        {  2,  4,  6,  8, 10, -1 }, // [2] Shield
        {  5,  7,  9, 11, 13, -1 }, // [3] Blinding Aura
        { 12, 14, 16, 18, 20, -1 }, // [4] Summon Gwaihir
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [6] Eowyn (HERO_ID_EOWYN)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Double Strike
        {  2,  4,  6,  8, 10, -1 }, // [1] Shieldmaiden of Rohan
        {  5,  7,  9, 11, 13, -1 }, // [2] Rohan Sprint
        {  2,  4,  6,  8, 10, -1 }, // [3] Shield Bash
        { 12, 14, 16, 18, 20, -1 }, // [4] Forth Eorlingas!
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
    // [7] Gimli (HERO_ID_GIMLI)
    {
        {  2,  4,  6,  8, 10, -1 }, // [0] Axe Throw
        {  2,  4,  6,  8, 10, -1 }, // [1] Whirling Attack
        {  2,  4,  6,  8, 10, -1 }, // [2] Dwarven Rage
        {  5,  8, 11, 14, 17, -1 }, // [3] Stoicism
        { 12, 14, 16, 18, 20, -1 }, // [4] Earth Shatter
        { -1, -1, -1, -1, -1, -1 }, // [5] Herbal Healing
    },
};
// clang-format on
