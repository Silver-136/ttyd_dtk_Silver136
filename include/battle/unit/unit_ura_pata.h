#pragma once

// Work slots of unit_ura_pata, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0   // 1 ends the sibling loop that spins the shell during the attack
#define UNIT_WORK_SPIN_SPEED 1  // degrees per frame of that spin around y (5, 10, 20, 40)
#define UNIT_WORK_AURA_EFFECT 2 // EffectEntry* of the effUranokoEntry aura that eff_aura keeps on the unit
