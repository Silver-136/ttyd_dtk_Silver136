#pragma once

// Work slots of unit_togemet, shared by its copies in the jin and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ON_CEILING 0 // nonzero (battle setup unitWork) = starts on the ceiling (init_event2 sets the ceiling tables and events); cleared when it falls or drops down
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during the shell attack
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (10, 20, then 10)
