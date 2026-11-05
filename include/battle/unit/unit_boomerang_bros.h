#pragma once

// Work slots of unit_boomerang_bros, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_RETURNED 0        // boomerangs back in hand: boomerang_move_event adds 1 at its end; the attack waits for 1 (normal) or 2 (renzoku)
#define UNIT_WORK_TARGET1 1         // unit id of the first target (-1 = none)
#define UNIT_WORK_TARGET2 2         // unit id of the second target (-1 = none)
#define UNIT_WORK_TARGET1_PART 3    // part id of the first target
#define UNIT_WORK_TARGET2_PART 4    // part id of the second target
#define UNIT_WORK_TARGET1_HIT 5     // 1 if the outbound pass hit the first target; the last boomerang's return pass then damages it without a new check (damage_check_last)
#define UNIT_WORK_TARGET2_HIT 6     // 1 if the outbound pass hit the second target (same use as UNIT_WORK_TARGET1_HIT)
#define UNIT_WORK_AC_DEFENCE_DONE 7 // 1 once btlevtcmd_ResultACDefence ran for the current pass; attack_event clears it and boomerang_move_event clears it while its second-target return-check state is active
