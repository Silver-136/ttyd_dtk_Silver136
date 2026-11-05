#pragma once

// Work slots of unit_patamet, shared by its copies in the dou, jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ON_CEILING 0 // nonzero: starts on the ceiling (init_event2 sets the ceiling attributes, tables and events; setup value, 0 in every dou/jon setup); dropping from the ceiling clears it
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during attack_event
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (10, 20, then 10)
