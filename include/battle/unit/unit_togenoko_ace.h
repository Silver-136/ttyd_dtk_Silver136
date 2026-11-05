#pragma once

// Work slots of unit_togenoko_ace, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/SetUnitWork)
#define UNIT_WORK_GUARD 0      // 1 while in the shell-guard stance (guard event; guard cancel and wake-up clear it)
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during the attack
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (10, 20, 40)
