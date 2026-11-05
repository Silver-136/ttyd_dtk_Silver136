#pragma once

// Work slots of unit_honenoko, shared by its copies in the gon, jon, las, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0     // 1 ends the sibling loop that spins the thrown bone (part 2, or part 3 in call_friend_event)
#define UNIT_WORK_REWARD_GIVEN 1  // 1 once coins, EXP and star points have been stored (dead_event or fake_dead_event), so it happens only once
#define UNIT_WORK_REBIRTH_TURNS 2 // turns left before it reassembles: fake_dead_event sets 2 + evt_sub_random(2), rebirth_event counts down and revives at 0
