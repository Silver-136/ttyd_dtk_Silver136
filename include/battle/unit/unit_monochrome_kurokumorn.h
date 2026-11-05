#pragma once

// Work slots of unit_monochrome_kurokumorn, shared by its copies in the jon, mri, tou2, tst and win RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOWERED 0 // 1 while it hangs low (y 10, hammer and shell can reach it), 0 while raised (y 40); unison_phase_event flips it at random (50 %)
