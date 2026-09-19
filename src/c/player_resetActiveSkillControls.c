#include "player.h"
#include "skill.h"
#include "variables.h"

/**
 * Restore both players' active skill controls: bind quick select to Herbal Healing and skill
 * rows 0 and 1, and enable every skill in the cycle. Leave the current selection unchanged.
 *
 * @romaddress 0x0804189c
 */
void player_resetActiveSkillControls(void)
{
    u32 i;

    for (i = 0; i < 2; i++)
    {
        Player *player = &PLAYER(i);

        player->quickSelectActiveSkills[0] = ACTIVE_SKILL_HERBAL_HEALING;
        player->quickSelectActiveSkills[1] = 0;
        player->quickSelectActiveSkills[2] = 1;
        player->activeSkillCycleEnabled[0] = 1;
        player->activeSkillCycleEnabled[1] = 1;
        player->activeSkillCycleEnabled[2] = 1;
        player->activeSkillCycleEnabled[3] = 1;
        player->activeSkillCycleEnabled[4] = 1;
        player->activeSkillCycleEnabled[5] = 1;
    }
}
