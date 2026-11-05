#pragma once

// Work slots of unit_hammer_bros, shared by its copies in the tik, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_THROW_TARGET 0      // slots 0-4: five randomly selected target units; only the first four are thrown at
#define UNIT_WORK_THROW_TARGET_PART 5 // slots 5-9: target part for slots 0-4 (slots 5-9)
#define UNIT_WORK_THROW_HIT 10        // slots 10-14: 1 once a non-final hammer hit the target of throw n (PreCheckDamage result 1); the last hammer at that target then hits with the final-hit flag without a new check; slot 10 does the same for the renzoku attack; attack_event clears them (slots 10-14)
