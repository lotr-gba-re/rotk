#include "skill.h"
#include "types.h"

// clang-format off

/**
 * Hero level a player needs before buying each level of a skill: row = skill id,
 * column = level bought so far, -1 once maxLevel is reached.
 *
 * @romaddress 0x082874bc
 */
const s8 PassiveSkillRequiredLevels[35][6] = {
    {  2,  4,  6,  8, 10, -1 }, // [ 0] FEARLESS
    {  2,  4,  6,  8, 10, -1 }, // [ 1] ACCURACY
    {  2,  4,  6,  8, 10, -1 }, // [ 2] NIMBLE
    {  2,  4,  6,  8, 10, -1 }, // [ 3] DEATH_STRIKE
    {  2,  4,  6,  8, 10, -1 }, // [ 4] BLADEMASTER
    {  2,  4,  6,  8, 10, -1 }, // [ 5] AXEMASTER
    {  2,  4,  6,  8, 10, -1 }, // [ 6] HERB_LORE
    {  2,  4,  6,  8, 10, -1 }, // [ 7] KEEN_EYES
    {  2,  4,  6,  8, 10, -1 }, // [ 8] RANGEMASTER
    {  2,  4,  6,  8, 10, -1 }, // [ 9] LUCK
    {  2,  5,  8, 11, 14, -1 }, // [10] DIRTY_CLAWS
    {  2,  4,  6,  8, 10, -1 }, // [11] WHY_DOES_IT_HURT_SMEAGOL
    {  8, 10, 12, 14, 16, -1 }, // [12] ORCSLAYER
    {  5,  7,  9, 11, 13, -1 }, // [13] WOODSMAN
    {  8, 10, 12, 14, 16, -1 }, // [14] DWARF_SENSE
    {  8, 10, 12, 14, 16, -1 }, // [15] IRON_WILL
    {  6,  8, 10, 12, 14, -1 }, // [16] SPIRIT_OF_MIDDLE_EARTH
    {  8, 10, 12, 14, 16, -1 }, // [17] FIGHTERS_RESOLVE
    {  8, 10, 12, 14, 16, -1 }, // [18] SHIELD_OFFENSE
    {  8, 10, 12, 14, 16, -1 }, // [19] FLEET_OF_FOOT
    {  4,  6,  8, 10, 12, -1 }, // [20] HARDY
    { 15, 17, 19, 21, 23, -1 }, // [21] WISE
    { 15, 17, 19, 21, 23, -1 }, // [22] WISDOM_OF_THE_AGES
    { 15, 17, 19, 21, 23, -1 }, // [23] BATTLE_SCARRED
    { 15, 17, 19, 21, 23, -1 }, // [24] LAST_STAND
    { 15, 17, 19, 21, 23, -1 }, // [25] ARROW_PARRY
    { 10, 12, 14, 16, 18, -1 }, // [26] WRAITHSLAYER
    { 15, 17, 19, 21, 23, -1 }, // [27] GALADRIELS_BLESSING
    { 15, 17, 19, 21, 23, -1 }, // [28] BERSERKER
    { 20, -1, -1, -1, -1, -1 }, // [29] SERVANT_OF_THE_SECRET_FIRE
    { 20, -1, -1, -1, -1, -1 }, // [30] RAGE_OF_THE_NORTH
    { 20, -1, -1, -1, -1, -1 }, // [31] ARCHER_OF_MIRKWOOD
    { 20, -1, -1, -1, -1, -1 }, // [32] GLOINS_DOUBLE_AXES
    { 20, -1, -1, -1, -1, -1 }, // [33] DEFENDERS_FURY
    { 20, -1, -1, -1, -1, -1 }, // [34] THE_PRECIOUS
};
// clang-format on
