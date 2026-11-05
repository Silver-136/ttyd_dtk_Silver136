#pragma once

// Work slots of unit_red_honenoko, shared by its copies in the gon, las and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0    // 1 ends the sibling loop that spins a thrown bone part (25 degrees per frame); the throw sets 0, the landing 1
#define UNIT_WORK_REWARD_GIVEN 1 // 1 once its coins and EXP were paid out (first collapse or death), so they are paid only once
#define UNIT_WORK_REVIVE_TURNS 2 // turns until it rebuilds itself: fake_dead_event sets 2-4; rebirth_event counts down and revives at 0; attack_event runs rebirth_event while >= 1
