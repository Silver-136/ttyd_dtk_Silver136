#pragma once

// Work slots of unit_churantalar, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOWERED 0        // 1 while lowered to y 10 and hammerable (move_kuutei_event), 0 while raised to y 70 (move_kuuchu_event)
#define UNIT_WORK_YARN_ALPHA 1     // alpha of its thread (255 full); battle_end_event sets 254 and yarn_event then fades it by 2 per frame
#define UNIT_WORK_YARN_Y 2         // height of the thread's lower end (piders_draw_yarn y1); entry_event eases it from 400 to 0
#define UNIT_WORK_FIRST_LOWER 3    // 1 until the first move_kuutei_event, which plays SFX_ENM_PAIDA_MOVE1 instead of MOVE2 and clears it
#define UNIT_WORK_PART2_ROTATION 4 // z rotation (times face direction) part 2 ended at after a wind-up; piders_attack_common_event1_2 starts the next wind-up from it
#define UNIT_WORK_TARGET1_ID 5     // unit id of the first renzoku shot's target (piders_renzoku_attack_set_target); first of slots 5-7
#define UNIT_WORK_TARGET2_ID 6     // unit id of the second renzoku shot's target
#define UNIT_WORK_TARGET3_ID 7     // unit id of the third renzoku shot's target
#define UNIT_WORK_TARGET1_PART 8   // part id of the first target; first of slots 8-10 (slot 5 + 3)
#define UNIT_WORK_TARGET2_PART 9   // part id of the second target
#define UNIT_WORK_TARGET3_PART 10  // part id of the third target
#define UNIT_WORK_TARGET1_HIT 11   // 1 once the first target has been hit (target slot + 6); the last shot then takes the plain CheckDamage path
#define UNIT_WORK_TARGET2_HIT 12   // 1 once the second target has been hit
#define UNIT_WORK_TARGET3_HIT 13   // 1 once the third target has been hit
