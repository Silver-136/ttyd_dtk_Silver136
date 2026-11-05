#pragma once

// Work slots of unit_crimson_togemet, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_CEILING 0    // setup value: nonzero = starts on the ceiling (init_event2 sets the ceiling tables and events); the fall/turn events clear it
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during the attack (and resets the rotation)
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (0, 10, 20)
