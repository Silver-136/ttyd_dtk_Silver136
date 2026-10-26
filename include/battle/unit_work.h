#pragma once

// BattleUnit.work / BattleUnitPart.work slots that C code reads as well as the unit's own scripts. Slots only a
// unit's own script uses are named in that unit's file (UNIT_WORK_* / PARTS_WORK_* defines).

// Any unit: added to the defence of its parts with kPartUnitWorkDefense (BattlePreCheckDamage)
#define UNIT_WORK_PART_DEFENSE 1

// Mario (unit_mario, battle_mario, battle_seq)
#define MARIO_WORK_SPIN_ON 2            // marioAttackEvent_TatsumakiJump: 1 keeps the spin loop running, 0 ends it
#define MARIO_WORK_SPIN_SPEED_MAX 3     // ... the spin speed the loop ramps up to (30, then 45 / 60 by the action command)
#define MARIO_WORK_SPIN_SPEED 4         // ... the current spin speed (degrees per frame)
#define MARIO_WORK_SPIN_PITCH_TARGET 5  // ... the pitch the spin sound moves to (0, 1023, 2046, 3069 by the action command)
#define MARIO_WORK_SPIN_PITCH 6         // ... the spin sound's current pitch
#define MARIO_WORK_SPIN_PITCH_STEP 7    // ... its per-frame step toward the target ((target - current) / 10)
#define MARIO_WORK_PINCH_VOICE 8        // counts the pinch-relief voice lines (odd: RELIEF2); reset once no one is in danger
#define MARIO_PARTS_WORK_FIRE_BALL 0    // part 3 (the Fire Drive fireball): 0 ends _mario_fire_ball_controll

// Shell Shield (unit_koura; set up by Koops' move in unit_party_nokotarou and battle_event_subset)
#define SHELL_SHIELD_WORK_TARGET 0      // the unit under the shell
#define SHELL_SHIELD_WORK_OPEN 1        // 1 while the shell is open (koura_open / koura_close)
#define SHELL_SHIELD_WORK_FOLLOW 2      // how BattleUnitDisplay keeps it on the target: 0 not, 1 on it, 2 within 10 in x,
                                        // 3 sliding 1 per frame (then 1)
#define SHELL_SHIELD_WORK_CRACK 3       // pose table: 0 whole, 1 cracked (HP <= 6), 2 badly cracked (HP <= 3)

// Vivian and the other Shadow Sirens: where the shadow tail is anchored on the floor (BattleUnitDisplay passes it to
// animPoseVivianMain for these unit kinds)
#define SHADOW_TAIL_WORK_X 0
#define SHADOW_TAIL_WORK_Y 1
#define SHADOW_TAIL_WORK_Z 2
#define SHADOW_TAIL_WORK_HOLD 3         // 1 keeps the anchor in place (during their attacks)

// The three sisters' line in their boss battles (unit_boss_majolyne / _marilyn / _vivian in win and las, and the las
// Doopliss when he stands in for one): majo_unison_phase_event orders them by HP each turn
#define MAJO_WORK_LINE_POS 4            // place in the line it stands at (0, 1, 2 = home x 55, 105, 155)
#define MAJO_WORK_NEXT_LINE_POS 5       // place majo_unison_phase_event gives it (majo3_sort: 0 = most HP); unison_phase_event
                                        // moves it when this differs from MAJO_WORK_LINE_POS
#define MAJO_WORK_LOW_HP_TALK_DONE 6    // 1 once the low-HP lines ran (the leader's phase_event: below 66 % for her, 50 % for
                                        // the others)
#define MAJO_WORK_LOW_HP_ATTACK 7       // set to 1 by those lines; the unit's attack choice then weights in its strong attack

// Barriern satellites (unit_barriern*_satellite in aji, mri and moo), set up by the Barriern that spawns them
#define BARRIERN_SATELLITE_WORK_OWNER 0       // unit id of that Barriern
#define BARRIERN_SATELLITE_WORK_IDLE_POSE 1   // 1 lets its wait event play the S_4 pose at random times
#define BARRIERN_SATELLITE_PARTS_WORK_SPLINE 2 // part 1: the spline table of its last move (the Barriern's
                                               // barriernSatelliteTo_Init stores it; barriernSatelliteToInside reads it)

// Bobbery's bomb (unit_bomzou; spawned by unit_party_sanders)
#define BOMZOU_WORK_TURNS_LEFT 0        // turns until it explodes (2 at start; phase_event subtracts 1, at 0 explosion_event)
#define BOMZOU_WORK_FUSE_SFX 1          // id of the SFX_BTL_THUNDERS_BOMB1 sound Bobbery's _shot_bomb_event starts (0 none);
                                        // explosion_event and end_battle_evt stop it

// Tutorial ("lecture") units, reached as UNIT_ID_LECTURE (BattleWork.lectureUnitId) by the other units in the battle
// unit_lecture_frankli (gor), spawned by unit_lecture_christine, which keeps its id in her own slot 0
#define LECTURE_FRANKLI_WORK_CHRISTINE 0     // unit id of the unit_lecture_christine unit that spawned it
#define LECTURE_FRANKLI_WORK_STEP 1          // lecture step: 0 start; odd steps 1 jump lesson, 3 christine's turn, 5 hammer
                                             // lesson, 7 christine again, 9 free battle (phase_event)
#define LECTURE_FRANKLI_WORK_ATTACK_RESULT 2 // Mario's action-command result in the free battle (0 none, 1-3)
#define LECTURE_FRANKLI_WORK_GUARD_RESULT 3  // Mario's guard result against christine's attack in the free battle
                                             // (4/5/6 by _get_defence_ac_result, 7 when it was countered)
// unit_lecture_frankli_kuriboo (tik)
#define LECTURE_KURIBOO_WORK_TOGEKURI_TALK_DONE 0 // 1 once a togekuri unit showed its mac_0_092_06/07 lines

// Vivian (unit_party_vivian, battle_unit)
#define VIVIAN_WORK_HIDE_TURNS 4        // turns left hidden in the shadow (Veil); BtlUnit_CheckShadowGuard
