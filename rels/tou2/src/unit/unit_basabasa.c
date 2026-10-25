#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"


BattleUnitKind unit_basabasa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[14];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
static BattleWeapon weapon_basabasa_attack;
static PoseSoundTiming pose_sound_timing_table[9];
static EvtWord ceil_sky_event[98];
static EvtWord init_event[78];
static EvtWord dmg_first_attack_pos[12];
static EvtWord damage_event[10];
static EvtWord damage_event_ceil[51];
static EvtWord counter_damage_event[16];
static EvtWord ceil_fall_ready_event[23];
static EvtWord attack_event[448];
static EvtWord wait_event[6];

BattleUnitKind unit_basabasa = {
    74,
    "btl_un_basabasa",
    6,
    0,
    1,
    1,
    17,
    0,
    2,
    70,
    0,
    50,
    9999,
    32,
    32,
    6,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    -15.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    0.0f,
    16.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    32.0f,
    32.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BASA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4040002,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, 0, // acHelpMsg..counterResistFlags
    0x2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table[] = {
    {1, "BSA_N_1"},
    {2, "BSA_Y_1"},
    {9, "BSA_Y_1"},
    {5, "BSA_K_1"},
    {4, "BSA_X_1"},
    {3, "BSA_X_1"},
    {28, "BSA_S_1"},
    {29, "BSA_Q_1"},
    {30, "BSA_Q_1"},
    {31, "BSA_S_1"},
    {39, "BSA_D_1"},
    {50, "BSA_A_1"},
    {42, "BSA_R_1"},
    {40, "BSA_W_1"},
    {56, "BSA_X_1"},
    {57, "BSA_X_1"},
    {65, "BSA_T_1"},
    {69, "BSA_S_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {1, "BSA_N_2"},
    {2, "BSA_Y_2"},
    {9, "BSA_Y_2"},
    {5, "BSA_K_2"},
    {4, "BSA_X_2"},
    {3, "BSA_X_2"},
    {28, "BSA_S_2"},
    {29, "BSA_Q_2"},
    {30, "BSA_Q_2"},
    {31, "BSA_S_2"},
    {39, "BSA_D_2"},
    {50, "BSA_A_1"},
    {42, "BSA_S_1"},
    {40, "BSA_S_1"},
    {56, "BSA_X_2"},
    {57, "BSA_X_2"},
    {65, "BSA_T_2"},
    {69, "BSA_S_2"},
};

static BattleDataEntry data_table[] = {
    {26, counter_damage_event},
    {25, counter_damage_event},
    {27, counter_damage_event},
    {28, counter_damage_event},
    {29, counter_damage_event},
    {30, counter_damage_event},
    {31, counter_damage_event},
    {32, counter_damage_event},
    {33, counter_damage_event},
    {34, counter_damage_event},
    {35, counter_damage_event},
    {36, counter_damage_event},
    {48, dmg_first_attack_pos},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_basabasa", "c_basabasa", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table_ceil},
};

static StatusVulnerability regist = {80, 85, 105, 100, 80, 100, 100, 80, 100, 90, 100, 90, 100, 95, 80, 80, 95, 100, 80, 100, 100, 95};

static BattleWeapon weapon_basabasa_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseSoundTiming pose_sound_timing_table[] = {
    {"BSA_S_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_W_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_R_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_T_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_A_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_N_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_K_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_X_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

static EVT_BEGIN(ceil_sky_event)
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_OffAttribute, -2, 2)
    CALL(btlevtcmd_OnAttribute, -2, 4)
    CALL(btlevtcmd_SetPossessionItemOffset, -2, 0, 0, 0)
    CALL(btlevtcmd_SetEventCeilFall, -2, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddPos, -2, 0, -30, 0)
    CALL(btlevtcmd_AddHomePos, -2, 0, -30, 0)
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 30, 0)
    CALL(btlevtcmd_SetKissHitOffset, -2, 16, 21, 0)
    CALL(btlevtcmd_SetStatusIconOffset, -2, 6, 32)
    CALL(btlevtcmd_AddHomePos, -2, 0, -40, 0)
    LABEL(99)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event_ceil))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventCeilFall, -2, PTR(ceil_fall_ready_event))
    CALL(btlevtcmd_SetPossessionItemOffset, -2, 0, -20, 0)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, -2, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetHitOffset, -2, 1, 0, -50, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, -30, 0)
    CALL(btlevtcmd_SetStatusIconOffset, -2, 6, 10)
    CALL(btlevtcmd_SetKissHitOffset, -2, 16, -18, 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(ceil_sky_event))
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_first_attack_pos)
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_SetPos, -2, LW(0), 10, LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_ceil)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckDamageCode, -2, 512, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -2, 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetTotalDamage, LW(10), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        RETURN
    END_IF
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 0, 0, -1)
    RETURN
EVT_END()

static EVT_BEGIN(counter_damage_event)
    CALL_CHILD(PTR(ceil_sky_event))
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(PTR(subsetevt_counter_damage))
    RETURN
EVT_END()

static EVT_BEGIN(ceil_fall_ready_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 0, 0, -1)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_basabasa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, -2, 0, -30, 0)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.5f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
        SUBTRACT(LW(1), 40)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -16, 0, 0, -1)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), -6)
    LOOP(2)
        ADD(LW(1), 13)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 10, 10, 0, 0, -1)
        ADD(LW(1), -13)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 10, -10, 0, 0, -1)
    END_LOOP
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -16, 0, 0, -1)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 0, 0, -1)
    WAIT_MS(200)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 4, 0, -1)
    WAIT_MS(200)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(7))
    IF_INT_EQUAL(LW(7), 1)
        SUBTRACT(LW(1), 30)
    END_IF
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 0, 0, -1)
    LABEL(99)
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("BSA_S_2"))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, -2, 0, 30, 0)
        ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("BSA_S_1"))
        WAIT_FRAMES(1)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

