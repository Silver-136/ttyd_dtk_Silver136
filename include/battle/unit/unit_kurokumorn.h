#pragma once

// Work slots of unit_kurokumorn, shared by its copies in the eki and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOW 0 // 1 while low (y 10, hammer and shell toss can reach it), 0 while high (y 40); unison_phase_event switches it
