#pragma once

// Work slots of unit_piders, shared by its copies in the jon, mri, tou2 and tst RELs, and by unit_churantalar
// (the same piders_* code).

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOWERED 0      // 1 while hanging low (y 10, hammer and shell toss can reach it; move_kuutei_event), 0 while high up (y 70; move_kuuchu_event)
#define UNIT_WORK_THREAD_ALPHA 1 // alpha of the thread piders_draw_yarn draws: 255 normally; battle_end_event sets 254, then yarn_event fades it by 2 per frame
#define UNIT_WORK_THREAD_END_Y 2 // y of the thread's lower end (piders_draw_yarn; the upper end is at y 400); entry_event lowers it from 400 to 0 over 40 frames
#define UNIT_WORK_FIRST_LOWER 3  // 1 until the first move_kuutei_event, which then plays SFX_ENM_PAIDA_MOVE1 instead of MOVE2 and clears it
#define UNIT_WORK_WINDUP_ANGLE 4 // part 2 rotation (degrees) reached at the end of the wind-up (piders_attack_common_event1); piders_attack_common_event1_2 starts from it
#define UNIT_WORK_TARGET_UNIT 5  // first of three slots (5-7): target unit of each shot of renzoku_attack_event (slots 5-7)
#define UNIT_WORK_TARGET_PART 8  // first of three slots (8-10): target part of each shot of renzoku_attack_event (slots 8-10)
#define UNIT_WORK_TARGET_HIT 11  // first of three slots (11-13): 1 once a shot hit that target; the last shot at a target that was hit skips the precheck and applies final-hit damage (renzoku_attack_event_sub_last) (slots 11-13)
