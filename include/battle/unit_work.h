#pragma once

// BattleUnit.work / BattleUnitPart.work slots that C code reads as well as the unit's own scripts. Slots only a
// unit's own script uses are named in that unit's file (UNIT_WORK_* / PARTS_WORK_* defines).

// Any unit: added to the defence of its parts with kPartUnitWorkDefense (BattlePreCheckDamage)
#define UNIT_WORK_PART_DEFENSE 1

// Mario (unit_mario, battle_mario, battle_seq)
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

// Vivian (unit_party_vivian, battle_unit)
#define VIVIAN_WORK_HIDE_TURNS 4        // turns left hidden in the shadow (Veil); BtlUnit_CheckShadowGuard
