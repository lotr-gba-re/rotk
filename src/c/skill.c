#include "skill.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "stats.h"
#include "variables.h"

static inline void revealSkillPanel(u8 playerIndex)
{
    g_PlayerHuds[playerIndex].skillPanelTimer = 0;
    g_PlayerHuds[playerIndex].skillPanelFrame = 0;
    g_PlayerHuds[playerIndex].skillPanelState = PLAYERHUD_SKILL_PANEL_REVEALING;
}

// HACK: The inline boundary keeps count-up loops from becoming down-counters.
static inline void addTemporaryActiveLevel(u8 playerIndex, s32 row)
{
    PLAYER(playerIndex).activeSkillLevels[row]++;
}

// Search the player's nine passive-skill slots for an id.
static inline u8 findSlot(u8 playerIndex, u8 passiveSkillId)
{
    s32 slot;

    for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT; slot++)
    {
        if (PLAYER(playerIndex).passiveSkillIds[slot] == passiveSkillId)
        {
            return slot;
        }
    }

    return PASSIVE_SKILL_SLOT_NONE;
}

/**
 * Buy count levels of passiveSkillId for the player: raises Player.passiveSkillLevels and applies
 * the stat records of each new level.
 *
 * BUG: count can raise a one-level skill past its maxLevel, up to PASSIVE_SKILL_MAX_LEVEL.
 *
 * @return FALSE when the hero's tree lacks the skill or it is already maxed
 *
 * @romaddress 0x08042bc4
 */
bool skill_passive_addLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE ||
        PLAYER(playerIndex).passiveSkillLevels[slot] >= PassiveSkills[passiveSkillId].maxLevel)
    {
        return FALSE;
    }

    PLAYER(playerIndex).passiveSkillLevels[slot] += count;
    if (PLAYER(playerIndex).passiveSkillLevels[slot] > PASSIVE_SKILL_MAX_LEVEL)
    {
        PLAYER(playerIndex).passiveSkillLevels[slot] = PASSIVE_SKILL_MAX_LEVEL;
    }

    skill_passive_applyLevels(playerIndex, passiveSkillId, count);

    if (passiveSkillId == PASSIVE_SKILL_FLEET_OF_FOOT)
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY;
    }
    return TRUE;
}

/**
 * Apply count new levels of passiveSkillId: raise Player.passiveSkillLevelsApplied (capped at
 * PASSIVE_SKILL_MAX_LEVEL) and apply the stat records of each level taken.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042c6c
 */
bool skill_passive_applyLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    s32 levels;
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    for (levels = 0; levels < count; levels++)
    {
        if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] < PASSIVE_SKILL_MAX_LEVEL)
        {
            PLAYER(playerIndex).passiveSkillLevelsApplied[slot]++;
            skill_passive_applyLevelStats(playerIndex, slot, 1);
        }
    }

    return TRUE;
}

/**
 * Take count levels back off passiveSkillId: subtract count from Player.passiveSkillLevels,
 * clamping at 0, and drop count applied levels too unless the subtraction underflowed.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042ce8
 */
bool skill_passive_removeLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    PLAYER(playerIndex).passiveSkillLevels[slot] -= count;
    if (PLAYER(playerIndex).passiveSkillLevels[slot] < 0)
    {
        // BUG: purchased levels become zero, but applied levels and their stat bonuses remain.
        PLAYER(playerIndex).passiveSkillLevels[slot] = 0;
    }
    else
    {
        skill_passive_removeAppliedLevels(playerIndex, passiveSkillId, count);
    }

    return TRUE;
}

/**
 * Drop up to count applied levels of passiveSkillId that sit above the bought level, removing the
 * stat records of each level taken back.
 *
 * @return FALSE when the hero's tree lacks the skill
 *
 * @romaddress 0x08042d60
 */
bool skill_passive_removeAppliedLevels(u8 playerIndex, u8 passiveSkillId, u8 count)
{
    u8 slot = findSlot(playerIndex, passiveSkillId);
    s32 i;

    if (slot == PASSIVE_SKILL_SLOT_NONE)
    {
        return FALSE;
    }

    for (i = 0; i < count; i++)
    {
        if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] >
            PLAYER(playerIndex).passiveSkillLevels[slot])
        {
            skill_passive_applyLevelStats(playerIndex, slot, -1);
            PLAYER(playerIndex).passiveSkillLevelsApplied[slot] -= 1;
        }
    }

    return TRUE;
}

/**
 * Drop up to count temporary levels (applied above bought) from every passive skill slot.
 *
 * @romaddress 0x08042df4
 */
void skill_passive_removeTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 n;
    s32 slot;

    for (n = 0; n < count; n++)
    {
        for (slot = 0; slot < HERO_PASSIVE_SKILL_COUNT; slot++)
        {
            if (PLAYER(playerIndex).passiveSkillLevelsApplied[slot] >
                PLAYER(playerIndex).passiveSkillLevels[slot])
            {
                // BUG: passes the skill id where skill_passive_applyLevelStats expects a tree
                // slot, so it reads the skill id and applied level of passiveSkillIds[skillId]
                // (past the 9-entry array for most ids) and removes the wrong skill's records.
                skill_passive_applyLevelStats(playerIndex,
                                              PLAYER(playerIndex).passiveSkillIds[slot], -1);
                PLAYER(playerIndex).passiveSkillLevelsApplied[slot] -= 1;
            }
        }
    }
}

/**
 * Add count temporary levels to learned passive skills (capped at five) and active skill rows
 * with callbacks, including unlearned active skills.
 *
 * BUG: the active loop walks 7 rows instead of HERO_ACTIVE_SKILL_COUNT (see
 * skill_active_addTemporaryLevelsToAll).
 *
 * @romaddress 0x08042e84
 */
void skill_addTemporaryLevels(u8 playerIndex, u8 count)
{
    s32 activeLevel;
    s32 activeRow;
    s32 passiveSlot;
    s32 passiveLevel;

    for (passiveLevel = 0; passiveLevel < count; passiveLevel++)
    {
        u8 level;

        for (passiveSlot = 0; passiveSlot < HERO_PASSIVE_SKILL_COUNT; passiveSlot++)
        {
            level = PLAYER(playerIndex).passiveSkillLevelsApplied[passiveSlot];
            if (level > 0 && level < PASSIVE_SKILL_MAX_LEVEL)
            {
                PLAYER(playerIndex).passiveSkillLevelsApplied[passiveSlot] = level + 1;
                // BUG: passes the skill id where skill_passive_applyLevelStats expects a tree slot
                // (see skill_passive_removeTemporaryLevels), so the wrong skill's records apply.
                skill_passive_applyLevelStats(playerIndex,
                                              PLAYER(playerIndex).passiveSkillIds[passiveSlot], 1);
            }
        }
    }

    for (activeLevel = 0; activeLevel < count; activeLevel++)
    {
        for (activeRow = 0; activeRow < HERO_ACTIVE_SKILL_COUNT + 1; activeRow++)
        {
            if (PLAYER_ACTIVE_SKILL(playerIndex, activeRow).callback != NULL)
            {
                addTemporaryActiveLevel(playerIndex, activeRow);
            }
        }
    }
}

/**
 * Add one PassiveSkill stat record value to stats[]: a primary stat or STAT_ALL_PRIMARY_STATS also
 * recomputes max HP / spirit, STAT_SPEED_PERCENT sets PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY.
 *
 * @romaddress 0x08042f48
 */
void skill_passive_addStat(u8 playerIndex, u8 statIndex, s16 delta)
{
    HeroBaseStats baseStats;
    s16 hpBonus;
    s16 spiritBonus;

    switch (statIndex)
    {
    case STAT_STRENGTH:
    case STAT_HEALTH:
    case STAT_COURAGE:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);
        PLAYER_STAT(playerIndex, statIndex) += delta;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);
        player_clampCurrentHpSpirit(playerIndex);
        return;

    case STAT_ALL_PRIMARY_STATS:
        spiritBonus = player_getMaxSpiritBonus(playerIndex);
        hpBonus = player_getMaxHpBonus(playerIndex);
        PLAYER_STAT(playerIndex, STAT_STRENGTH) += delta;
        PLAYER_STAT(playerIndex, STAT_ACCURACY) += delta;
        PLAYER_STAT(playerIndex, STAT_HEALTH) += delta;
        PLAYER_STAT(playerIndex, STAT_DEFENSE) += delta;
        PLAYER_STAT(playerIndex, STAT_COURAGE) += delta;
        PLAYER_RECOMPUTE_BASE_STATS(playerIndex, baseStats, hpBonus, spiritBonus);
        player_clampCurrentHpSpirit(playerIndex);
        return;

    default:
        break;
    }

    PLAYER_STAT(playerIndex, statIndex) += delta;
    if (statIndex == STAT_SPEED_PERCENT)
    {
        PLAYER(playerIndex).combatFlags.p |= PLAYER_COMBAT_FLAG_MOVE_SPEED_DIRTY;
    }
}

/**
 * Pay the active skill's spirit cost and start casting it: Herbal Healing runs its callback at
 * once, every other row enters the cast action state with Player.castingActiveSkill set.
 *
 * @romaddress 0x080430e8
 */
void skill_active_cast(u8 playerIndex, u8 activeSkillIndex)
{
    Actor *actor;

    if (PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).callback == NULL)
    {
        return;
    }

    if (PLAYER(playerIndex).currentSpirit <
            PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).spiritCost &&
        !(g_GameFlags.p & GAME_FLAG_2))
    {
        return;
    }

    actor = PLAYER(playerIndex).ownerActor;

    if (!(g_GameFlags.p & GAME_FLAG_2))
    {
        player_subtractSpirit(playerIndex,
                              PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).spiritCost);
    }

    if (activeSkillIndex == ACTIVE_SKILL_HERBAL_HEALING)
    {
        PLAYER_ACTIVE_SKILL(playerIndex, ACTIVE_SKILL_HERBAL_HEALING).callback(actor);
    }
    else
    {
        actor->actionState = ACTOR_STATE_ACTIVE_SKILL_CAST;
        PLAYER(playerIndex).castingActiveSkill = activeSkillIndex;
        PLAYER(playerIndex).activeSkillStateFlags |= ACTIVE_SKILL_STATE_CASTING;
    }
}

static inline u32 findEnabledSkill(u8 playerIndex, u32 selected, u32 maxAttempts)
{
    u32 attempts;
    u32 lastSkill = ACTIVE_SKILL_HERBAL_HEALING;

    for (attempts = 0; attempts < maxAttempts; attempts++)
    {
        if ((s8)PLAYER(playerIndex).activeSkillLevels[selected] != 0 &&
            skill_active_hasLevels(playerIndex, selected) &&
            PLAYER(playerIndex).activeSkillCycleEnabled[selected] != 0)
        {
            break;
        }
        if (selected >= lastSkill)
        {
            selected = 0;
        }
        else
        {
            selected++;
        }
    }
    if (attempts == maxAttempts)
    {
        selected = ACTIVE_SKILL_NONE;
    }
    return selected;
}

/**
 * Cycle the selected active skill with L and cast it with A.
 *
 * @param fromHudInit Suppress casting and permit selection changes while the panel animates.
 *
 * @romaddress 0x0804319c
 */
void skill_active_handleCycleCastModeInput(u8 playerIndex, bool fromHudInit)
{
    u8 skillCount = HERO_ACTIVE_SKILL_COUNT;
    u32 selected = (s8)PLAYER(playerIndex).selectedActiveSkill.cycleSkillIndex;

    if (!fromHudInit && (PLAYER_KEYS_PRESSED(playerIndex) & A_BUTTON) != 0 &&
        (PLAYER(playerIndex).activeSkillStateFlags & ACTIVE_SKILL_STATE_CASTING) == 0)
    {
        if (selected != ACTIVE_SKILL_NONE && skill_active_canCast(playerIndex, selected) == TRUE)
        {
            skill_active_cast(playerIndex, selected);
        }
    }
    else if (((PLAYER_KEYS_PRESSED(playerIndex) & L_BUTTON) != 0 ||
              !skill_active_hasLevels(playerIndex, selected) ||
#ifdef BUGFIX
              selected == ACTIVE_SKILL_NONE ||
#else
    // BUG: A temporary level in slot 6 can leave an empty selection stuck until L
    // is pressed because this reads past the cycle flags.
#endif
              PLAYER(playerIndex).activeSkillCycleEnabled[selected] == 0) &&
             ((g_PlayerHuds[playerIndex].skillPanelState != PLAYERHUD_SKILL_PANEL_REVEALING &&
               g_PlayerHuds[playerIndex].skillPanelState != PLAYERHUD_SKILL_PANEL_RETRACTING) ||
              fromHudInit == TRUE))
    {
        u32 lastSkill = ACTIVE_SKILL_HERBAL_HEALING;

        if (selected == ACTIVE_SKILL_NONE || selected >= lastSkill)
        {
            selected = 0;
        }
        else
        {
            selected++;
        }

        selected = findEnabledSkill(playerIndex, selected, skillCount);
        PLAYER(playerIndex).selectedActiveSkill.cycleSkillIndex = selected;
        if (selected != ACTIVE_SKILL_NONE)
        {
            u8 hudState = g_PlayerHuds[playerIndex].skillPanelState;

            if (hudState == PLAYERHUD_SKILL_PANEL_IDLE)
            {
                revealSkillPanel(playerIndex);
            }
            else if (hudState == PLAYERHUD_SKILL_PANEL_SHOWING)
            {
                g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_SKILL_PANEL_DIRTY;
            }
        }
    }
}

/**
 * Cast the active skill bound to L+A, L+B, or L+R and reveal its HUD panel.
 *
 * @romaddress 0x08043350
 */
void skill_active_handleQuickCastModeInput(u8 playerIndex)
{
    QuickCastSlot slot;
    u8 activeSkillIndex;

    if ((PLAYER_KEYS_CURRENT(playerIndex) & L_BUTTON) == 0 ||
        (PLAYER(playerIndex).activeSkillStateFlags & ACTIVE_SKILL_STATE_CASTING) != 0)
    {
        return;
    }

    if ((PLAYER_KEYS_PRESSED(playerIndex) & A_BUTTON) != 0)
    {
        slot = QUICK_CAST_SLOT_A;
    }
    else if ((PLAYER_KEYS_PRESSED(playerIndex) & B_BUTTON) != 0)
    {
        slot = QUICK_CAST_SLOT_B;
    }
    else
    {
        slot = (PLAYER_KEYS_PRESSED(playerIndex) & R_BUTTON) != 0 ? QUICK_CAST_SLOT_R
                                                                  : QUICK_CAST_SLOT_NONE;
    }
    if (slot == QUICK_CAST_SLOT_NONE)
    {
        return;
    }

    activeSkillIndex = PLAYER(playerIndex).quickSelectActiveSkills[slot];

    if (skill_active_canCast(playerIndex, activeSkillIndex) == TRUE)
    {
        PLAYER(playerIndex).selectedActiveSkill.quickSlot = slot;
        skill_active_cast(playerIndex, activeSkillIndex);
    }
    else
    {
        s8 activeSkillLevel = PLAYER(playerIndex).activeSkillLevels[activeSkillIndex];

        if (activeSkillLevel == 0)
        {
            slot = QUICK_CAST_SLOT_NONE;
        }
    }

    if (slot == QUICK_CAST_SLOT_NONE)
    {
        return;
    }

    // It's probably impossible for slot to be ACTIVE_SKILL_NONE here, but the ROM checks it
    if (slot != ACTIVE_SKILL_NONE &&
        g_PlayerHuds[playerIndex].skillPanelState == PLAYERHUD_SKILL_PANEL_IDLE)
    {
        revealSkillPanel(playerIndex);
        return;
    }
    if (slot != QUICK_CAST_SLOT_NONE && slot != ACTIVE_SKILL_NONE &&
        g_PlayerHuds[playerIndex].skillPanelState == PLAYERHUD_SKILL_PANEL_SHOWING)
    {
        g_PlayerHuds[playerIndex].flags.p |= PLAYERHUD_FLAG_SKILL_PANEL_DIRTY;
    }
}

/**
 * Check whether a player can cast an active skill now.
 *
 * @romaddress 0x08043484
 */
u32 skill_active_canCast(u8 playerIndex, u8 activeSkillIndex)
{
    Actor *actor = PLAYER(playerIndex).ownerActor;
    u8 actionState = actor->actionState;
    ItemType offhandType = PLAYER(playerIndex).inventory.slots.offhand.d.itemType;
    ItemType mainHandType = PLAYER(playerIndex).inventory.slots.mainhand.d.itemType;
    u16 actionFlag = 1u << activeSkillIndex;
    s8 learnedLevel;

    if (activeSkillIndex == ACTIVE_SKILL_HERBAL_HEALING &&
        PLAYER(playerIndex).kingsfoilHerbs.fresh == 0 &&
        PLAYER(playerIndex).kingsfoilHerbs.dried == 0)
    {
        return FALSE;
    }
    if (activeSkillIndex == ACTIVE_SKILL_HERBAL_HEALING)
    {
        return TRUE;
    }

    learnedLevel = PLAYER(playerIndex).activeSkillLevels[activeSkillIndex];
    if (learnedLevel == 0)
    {
        return FALSE;
    }
    if (PLAYER(playerIndex).currentSpirit <
            PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).spiritCost &&
        !(g_GameFlags.p & GAME_FLAG_2))
    {
        return FALSE;
    }
    if (actionState == ACTOR_STATE_LEGOLAS_OVERDRAW)
    {
        return FALSE;
    }

    switch (actionState)
    {
    case ACTOR_STATE_ACTIVE_SKILL_CAST_RECOVERY:
    case ACTOR_STATE_UNKNOWN_22:
    case ACTOR_STATE_ACTIVE_SKILL_CAST:
    case ACTOR_STATE_UNKNOWN_30:
    case ACTOR_STATE_KNOCKED_DOWN:
    case ACTOR_STATE_ACTIVE_SKILL_CAST_ANIMATION:
        return FALSE;
    default:
        if (actor->as.combat.actionStateFlags & actionFlag)
        {
            return FALSE;
        }
    }

    switch (PLAYER(playerIndex).heroId)
    {
    case HERO_ID_FRODO:
    case HERO_ID_SAM:
        switch (activeSkillIndex)
        {
        case ACTIVE_SKILL_FRODO_KNIFE_TOSS:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_FRODO_SNARE: {
            const Player *player = &PLAYER(playerIndex);
            const u8 *skillLevel = &player->activeSkillLevels[ACTIVE_SKILL_FRODO_SNARE];
            u8 snareCount = actor->as.combat.comboState[0];
            s16 maxSnares =
                skill_active_getLeveledValue(playerIndex, ACTIVE_SKILL_FRODO_SNARE, 1, *skillLevel);
            if (snareCount < maxSnares)
            {
                break;
            }
            return FALSE;
        }
        case ACTIVE_SKILL_FRODO_THE_ONE_RING:
            if (PLAYER(playerIndex).heroId != HERO_ID_FRODO ||
                g_CurrentMissionId != MISSION_CRACK_OF_DOOM_EDGE_OF_VOLCANO ||
                g_MissionVariant != 7)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_FRODO_RINGS_PERSUASION:
            if (PLAYER(playerIndex).heroId != HERO_ID_FRODO ||
                g_CurrentMissionId != MISSION_CRACK_OF_DOOM_EDGE_OF_VOLCANO ||
                g_MissionVariant != 7)
            {
                break;
            }
            return FALSE;
        default:
            return TRUE;
        }
        break;
    case HERO_ID_LEGOLAS:
        switch (activeSkillIndex)
        {
        case ACTIVE_SKILL_LEGOLAS_FRIEND_OF_MIRKWOOD:
            if (actor->as.combat.comboState[0] == 0)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_LEGOLAS_SPREAD_FIRE:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_LEGOLAS_WHITE_KNIVES:
            if (PLAYER(playerIndex).inventory.slots.backpack_0.d.itemType == ITEM_TYPE_KNIFE)
            {
                break;
            }
            return FALSE;
        default:
            return TRUE;
        }
        break;
    case HERO_ID_ARAGORN:
        switch (activeSkillIndex)
        {
        case ACTIVE_SKILL_ARAGORN_SWEEP:
        case ACTIVE_SKILL_ARAGORN_SWORD_THROW:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_ARAGORN_CALL_OF_THE_DEAD:
            if (actor->as.combat.comboState[0] == 0)
            {
                break;
            }
            return FALSE;
        default:
            return TRUE;
        }
        break;
    case HERO_ID_GANDALF:
        if (activeSkillIndex != ACTIVE_SKILL_GANDALF_SWORD_OF_POWER)
        {
            if (activeSkillIndex == ACTIVE_SKILL_GANDALF_LIGHTSTRIKE)
            {
#ifdef BUGFIX
                // Check the family instead of the item type for normal and unique weapons.
                Item offhand = PLAYER(playerIndex).inventory.slots.offhand;
                if (offhand.d.itemType != ITEM_TYPE_EMPTY &&
                    ITEM_BASE_METADATA(offhand).flags.d.familyStaff)
#else
                // BUG: Unique staves have item type UNIQUE, so Gandalf cannot cast
                // Lightstrike with them.
                if (offhandType == ITEM_TYPE_STAFF)
#endif
                {
                    break;
                }
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
#ifdef BUGFIX
            // Check the family instead of the item type for normal and unique weapons.
            Item mainHand = PLAYER(playerIndex).inventory.slots.mainhand;
            if (mainHand.d.itemType != ITEM_TYPE_EMPTY &&
                ITEM_BASE_METADATA(mainHand).flags.d.familySword)
#else
            // BUG: Unique swords have item type UNIQUE, so Gandalf cannot cast Sword
            // of Power with them.
            if (mainHandType <= ITEM_TYPE_SWORD_1H)
#endif
            {
                break;
            }
        }
        return FALSE;
    case HERO_ID_EOWYN:
        switch (activeSkillIndex)
        {
        case ACTIVE_SKILL_EOWYN_DOUBLE_STRIKE:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_EOWYN_SHIELD_BASH:
            if (offhandType == ITEM_TYPE_SHIELD)
            {
                break;
            }
            return FALSE;
        default:
            return TRUE;
        }
        break;
    case HERO_ID_SMEAGOL:
    default:
        return TRUE;
    case HERO_ID_GIMLI:
        switch (activeSkillIndex)
        {
        case ACTIVE_SKILL_GIMLI_AXE_THROW:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        case ACTIVE_SKILL_GIMLI_WHIRLING_ATTACK:
            if (mainHandType != ITEM_TYPE_EMPTY)
            {
                break;
            }
            return FALSE;
        default:
            return TRUE;
        }
        break;
    }
    return TRUE;
}

/**
 * An active skill's leveled value: base + the first `levelCount` per-level
 * increments, plus one extra increment capped at ACTIVE_SKILL_MAX_LEVEL while
 * STAT_ACTIVE_SKILL_LEVEL_BONUS is positive. Returns the summed s16 value.
 *
 * @param playerIndex player whose active skill is being queried
 * @param activeSkillIndex active skill row to query
 * @param valueIndex level curve within the active skill row
 * @param levelCount number of purchased or temporary levels
 *
 * @romaddress 0x08043764
 */
s16 skill_active_getLeveledValue(u8 playerIndex, u8 activeSkillIndex, u8 valueIndex, u8 levelCount)
{
    u16 total = PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).values[valueIndex].base;
    s32 level;

#ifdef BUGFIX
    if (levelCount > ACTIVE_SKILL_MAX_LEVEL)
    {
        levelCount = ACTIVE_SKILL_MAX_LEVEL;
    }
#else
    // BUG: a temporary level above the cap reads beyond
    // ActiveSkillValue.perLevel.
#endif

    for (level = 0; level < levelCount; level++)
    {
        total +=
            PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex).values[valueIndex].perLevel[level];
    }

    if (PLAYER(playerIndex).stats[STAT_ACTIVE_SKILL_LEVEL_BONUS] > 0)
    {
        if (level < ACTIVE_SKILL_MAX_LEVEL)
        {
            total += PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex)
                         .values[valueIndex]
                         .perLevel[level];
        }
        else
        {
            total += PLAYER_ACTIVE_SKILL(playerIndex, activeSkillIndex)
                         .values[valueIndex]
                         .perLevel[ACTIVE_SKILL_MAX_LEVEL - 1];
        }
    }
    return total;
}
