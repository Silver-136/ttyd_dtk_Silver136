#pragma once

// Work slots of unit_boss_gonbaba, shared by its copies in the gon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_POWER_DOWN 0     // subtracted from its attack power (weaponGetPower_Gonbaba, min 1); +1 (up to 20) per hit with DAMAGE_RESULT_ATTACK_FX_R
#define UNIT_WORK_MESSAGE_STEP 2   // damage messages shown: 1 stg1_gon_21, 2 stg1_gon_22, 3 stg1_gon_31_04 (after the negotiation)
#define UNIT_WORK_NEGOTIATED 3     // 1 once negotiation_event has run; at 0 dead_event runs the negotiation instead of dying
#define UNIT_WORK_EATEN_AUDIENCE 4 // audience members in its stomach: gon_extra_eaten_event sets 6; each GNB_D_1 hit lets up to two out, the value doubling as their audience index
#define UNIT_WORK_ATTACK_STEP 5    // attack order: 0 firebreath next, 1 fumituke next, 2 = random from then on
