#pragma once

// Work slots of unit_fire_bros, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_FIRE_THROWN 0 // fireballs thrown so far by nagemakuri_attack_event (+1 per throw); with 6 or more it waits until slot 1 > slot 0 - 6
#define UNIT_WORK_FIRE_DONE 1   // fireballs finished (fire_move_event adds 1 at its end); the attacks wait for 1 (normal), 2 (renzoku), 6 (nagemakuri)
// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_TARGET 0      // on fireball parts 2-7, slots 0-4: target units drawn by fire_bros_nagemakuri_set_target (only slot 0 is thrown at) (slots 0-4)
#define PARTS_WORK_TARGET_PART 5 // slots 5-9: target parts for slots 0-4 (slots 5-9)
#define PARTS_WORK_HIT 10        // slots 10-14: 1 once a non-final fireball hit the target in slot n (PreCheckDamage result 1); the last fireball at that target then hits with the final-hit flag without a new check; part 2 slot 10 does the same for the renzoku attack (slots 10-14)
#define PARTS_WORK_DONE 15       // 0 while this fireball flies; 1 once it hit or left the stage (x beyond +-250), which stops its bounce loop
