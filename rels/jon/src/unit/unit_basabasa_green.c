#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"

#pragma force_active on

USER_FUNC(btlevtcmd_AddHomePos);
USER_FUNC(btlevtcmd_AddPos);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnimeSetPoseSoundTable);
USER_FUNC(btlevtcmd_AnimeSetPoseTable);
USER_FUNC(btlevtcmd_CheckAttribute);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_DivePosition);
USER_FUNC(btlevtcmd_GetDamageCode);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetTotalDamage);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_OffAttribute);
USER_FUNC(btlevtcmd_OnAttribute);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventCeilFall);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetHitCursorOffset);
USER_FUNC(btlevtcmd_SetHitOffset);
USER_FUNC(btlevtcmd_SetKissHitOffset);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetPossessionItemOffset);
USER_FUNC(btlevtcmd_SetStatusIconOffset);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_WaitAttackEnd);

f32 fd_basabasa_green_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_basabasa_green_neg_one_382;
f32 fd_basabasa_green___local_PI_383;
f32 fd_basabasa_green___local_3PIO2_385;
f32 fd_basabasa_green_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_basabasa_green___local_PIO2_384;
f32 fd_basabasa_green_one_381;
f32 fd_basabasa_green_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_basabasa_green_neg_one_365;
f32 fd_basabasa_green___local_PI_366;
f32 fd_basabasa_green___local_3PIO2_368;
f32 fd_basabasa_green_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_basabasa_green___local_PIO2_367;
f32 fd_basabasa_green_one_364;
BattleUnitKind unit_basabasa_green;
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
static EvtWord attack_event[683];
static EvtWord wait_event[6];

f32 fd_basabasa_green_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_basabasa_green_neg_one_382 = -1.0f;

f32 fd_basabasa_green___local_PI_383 = 3.1415927f;

f32 fd_basabasa_green___local_3PIO2_385 = 4.712389f;

f32 fd_basabasa_green_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_basabasa_green___local_PIO2_384 = 1.5707964f;

f32 fd_basabasa_green_one_381 = 1.0f;

f32 fd_basabasa_green_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_basabasa_green_neg_one_365 = -1.0f;

f32 fd_basabasa_green___local_PI_366 = 3.1415927f;

f32 fd_basabasa_green___local_3PIO2_368 = 4.712389f;

f32 fd_basabasa_green_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_basabasa_green___local_PIO2_367 = 1.5707964f;

f32 fd_basabasa_green_one_364 = 1.0f;

BattleUnitKind unit_basabasa_green = {
    167,
    "btl_un_basabasa_green",
    20,
    0,
    1,
    1,
    35,
    2,
    2,
    70,
    2,
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
    0x1101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
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
    {1, "btl_un_basabasa_green", "c_basabasa_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table_ceil},
};

static StatusVulnerability regist = {50, 55, 90, 100, 50, 100, 100, 50, 100, 75, 100, 75, 100, 80, 50, 50, 75, 100, 50, 100, 100, 20};

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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x47, // special
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
            CALL_CHILD(subsetevt_confuse_flustered)
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
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 10)
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
    SET_VALUE(LF(0), 0)
    CALL(btlevtcmd_PreCheckCounter, -2, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 17)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), 14)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
        SET_VALUE(LW(9), PTR(&weapon_basabasa_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_SetRotate, -2, 0, 0, -45)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BASA_SUCK1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("BSA_A_2"))
    LOOP(2)
        LOOP(10)
            CALL(btlevtcmd_AddScale, -2, 0, FLOAT(0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(btlevtcmd_AddScale, -2, 0, FLOAT(-0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 15)
    ADD(LW(2), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetDamage, LW(3), LW(10))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("BSA_N_1"))
        WAIT_MS(300)
        CALL(btlevtcmd_RecoverHp, -2, 1, LW(10))
        CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
        ADD(LW(6), 20)
        ADD(LW(7), 10)
        CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, -2, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 10)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, -2, FLOAT(0.099609375f), FLOAT(-0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, -2, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, -2, 1, 1, 1)
        WAIT_MS(1000)
    END_IF
    WAIT_MS(500)
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

