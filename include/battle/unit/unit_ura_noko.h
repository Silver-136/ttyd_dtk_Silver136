#pragma once

// Work slots of unit_ura_noko, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0   // 1 ends the sibling loop that spins it during its shell attacks
#define UNIT_WORK_SPIN_SPEED 1  // degrees per frame of that spin (5, 10, 20, 40)
#define UNIT_WORK_AURA_EFFECT 2 // effect entry from effUranokoEntry; eff_aura keeps it on the unit during ura_attack_event
