#pragma once

// Work slots of unit_bllizard, shared by its copies in the bom, jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOW 0            // 1 while low (y 10, hammer and shell toss can reach it), 0 while high (y 40); unison_phase_event switches it
#define UNIT_WORK_BARRIER_EFFECT 1 // EffectEntry* of the ice barrier effect (effKumokumoN64Entry), 0 none; eff_ice_barrier creates and moves it, eff_ice_barrier_end deletes it
