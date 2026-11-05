#pragma once

// Work slots of unit_yami_noko, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0  // 1 ends the sibling loop that spins it during its shell attacks
#define UNIT_WORK_SPIN_SPEED 1 // degrees per frame of that spin (10, 20, 40)
