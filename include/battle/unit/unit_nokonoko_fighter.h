#pragma once

// Work slots of unit_nokonoko_fighter, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0  // 1 ends the sibling loop that spins the shell during the attack
#define UNIT_WORK_SPIN_SPEED 1 // degrees per frame of that spin around y (20, 40, 10)
