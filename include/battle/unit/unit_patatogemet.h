#pragma once

// Work slots of unit_patatogemet, shared by its copies in the eki, jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ON_CEILING 0 // nonzero at battle start puts it on the ceiling (init_event2: ceiling tables, y 130); cleared when it drops
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins the shell during the attack
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin around y (10, 20)
