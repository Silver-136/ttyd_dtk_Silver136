#pragma once

// Work slots of Yoshi's battle unit, shared by the partner (unit_party_yoshi.c) and its tst copy (unit_yoshi.c).

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SWALLOWED 1        // 1 once partyYoshiAttack_Nomikomi swallowed the target (it clears it first); only then does the run home offer the stylish window (ACRStart)
#define UNIT_WORK_GROUND_POUND 2     // 1 while partyYoshiAttack_NormalAttack (Ground Pound) runs, 0 from init_event and at its end; never read
#define UNIT_WORK_STEP_PITCH 4       // base pitch of the run-up step sounds in partyYoshiAttack_NormalAttack (0, then ramps 95..2387 over 25 frames)
#define UNIT_WORK_STEP_PITCH_SCALE 5 // percent applied to UNIT_WORK_STEP_PITCH by _yoshi_slide_move_sound (100 at the start of partyYoshiAttack_NormalAttack; btl_yoshi_yoroyoro_jump_move swings it between 50 and 100)
// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_EGG_KIND 0       // random 0-2 on egg parts 2-7: pose table (0 pose_table_egg_g, 1 pose_table_egg_y, 2 pose_table_egg_p) and Yoshi's throw poses (PYS_A2_*, PYS_A3_*, PYS_A4_*)
#define PARTS_WORK_WEAPON 2         // weapon the egg hits with (partyWeapon_YoshiEggAttack_Minimini); _egg_attack_event passes it to CheckDamage
#define PARTS_WORK_TARGET 3         // target unit of this egg (parts 2-7: each a ChoiceSamplingEnemy pick)
#define PARTS_WORK_TARGET_PART 4    // target part of this egg (pairs with PARTS_WORK_TARGET)
#define PARTS_WORK_PRECHECK 5       // CommandPreCheckDamage result for this egg (1 hit; 2 miss, 3 lucky, 6 dodgy start the matching StartAvoid)
#define PARTS_WORK_LAST_AT_TARGET 6 // -1 while sorting; 1 the last egg aimed at its target (CheckDamage with the final-hit flag), 0 an earlier egg at the same target (no final-hit flag), 2 the last egg after an earlier one already hit (pre-checked without evasion)
#define PARTS_WORK_EVENT 7          // id of the egg's _egg_attack_event sibling event (0 none); the end of the attack waits for it and clears it
