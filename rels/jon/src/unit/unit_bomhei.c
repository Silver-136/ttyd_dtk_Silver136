#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"


USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_CheckActStatus);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_GetDamageCode);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetRunSound);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SetWalkSound);
USER_FUNC(btlevtcmd_StartWaitEvent);

f32 fd_bomhei_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_bomhei_neg_one_382;
f32 fd_bomhei___local_PI_383;
f32 fd_bomhei___local_3PIO2_385;
f32 fd_bomhei_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_bomhei___local_PIO2_384;
f32 fd_bomhei_one_381;
f32 fd_bomhei_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_bomhei_neg_one_365;
f32 fd_bomhei___local_PI_366;
f32 fd_bomhei___local_3PIO2_368;
f32 fd_bomhei_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_bomhei___local_PIO2_367;
f32 fd_bomhei_one_364;
BattleUnitKind unit_bomhei;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_spark[18];
static BattleDataEntry data_table[2];
static BattleDataEntry data_table_spark[2];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
static BattleWeapon weapon_bomhei_attack;
static BattleWeapon weapon_bomhei_bomb;
static EvtWord bomb_event[106];
static EvtWord init_event[42];
static EvtWord damage_event[58];
static EvtWord change_spark_event[70];
static EvtWord attack_event[440];
static EvtWord wait_event[6];
static EvtWord damage_event_spark[25];
static EvtWord attack_event_spark[288];
static EvtWord wait_event_spark[27];

f32 fd_bomhei_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_bomhei_neg_one_382 = -1.0f;

f32 fd_bomhei___local_PI_383 = 3.1415927f;

f32 fd_bomhei___local_3PIO2_385 = 4.712389f;

f32 fd_bomhei_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_bomhei___local_PIO2_384 = 1.5707964f;

f32 fd_bomhei_one_381 = 1.0f;

f32 fd_bomhei_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_bomhei_neg_one_365 = -1.0f;

f32 fd_bomhei___local_PI_366 = 3.1415927f;

f32 fd_bomhei___local_3PIO2_368 = 4.712389f;

f32 fd_bomhei_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_bomhei___local_PIO2_367 = 1.5707964f;

f32 fd_bomhei_one_364 = 1.0f;

BattleUnitKind unit_bomhei = {
    43,
    "btl_un_bomhei",
    4,
    0,
    1,
    1,
    12,
    0,
    1,
    70,
    1,
    50,
    9999,
    32,
    30,
    4,
    30,
    0.0f,
    15.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    0.0f,
    11.0f,
    16.0f,
    0.0f,
    2.0f,
    15.0f,
    0.0f,
    32.0f,
    30.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BOMB_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static PoseTableEntry pose_table[] = {
    {1, "BOM_N_1"},
    {2, "BOM_Y_1"},
    {9, "BOM_Y_1"},
    {5, "BOM_K_1"},
    {4, "BOM_X_1"},
    {3, "BOM_X_1"},
    {28, "BOM_S_1"},
    {29, "BOM_Q_1"},
    {30, "BOM_Q_1"},
    {31, "BOM_S_1"},
    {39, "BOM_D_1"},
    {50, "BOM_A_1"},
    {42, "BOM_R_1"},
    {40, "BOM_W_1"},
    {56, "BOM_X_1"},
    {57, "BOM_X_1"},
    {65, "BOM_T_1"},
    {69, "BOM_S_1"},
};

static PoseTableEntry pose_table_spark[] = {
    {1, "BOM_N_2"},
    {2, "BOM_Y_2"},
    {9, "BOM_Y_2"},
    {5, "BOM_K_2"},
    {4, "BOM_X_2"},
    {3, "BOM_X_2"},
    {28, "BOM_S_2"},
    {29, "BOM_Q_2"},
    {30, "BOM_Q_2"},
    {31, "BOM_S_2"},
    {39, "BOM_D_2"},
    {50, "BOM_A_2"},
    {42, "BOM_R_2"},
    {40, "BOM_W_2"},
    {56, "BOM_X_2"},
    {57, "BOM_X_2"},
    {65, "BOM_T_2"},
    {69, "BOM_S_2"},
};

static BattleDataEntry data_table[] = {
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleDataEntry data_table_spark[] = {
    {48, btldefaultevt_Dummy},
    {0},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_bomhei", "c_bomhey", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0x1000, pose_table},
};

static StatusVulnerability regist = {70, 100, 70, 0, 70, 150, 100, 100, 100, 90, 100, 90, 100, 95, 80, 90, 80, 100, 80, 100, 100, 95};

static BattleWeapon weapon_bomhei_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x21004004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0, // counterResistFlags
    0x2004, // targetWeightFlags
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

static BattleWeapon weapon_bomhei_bomb = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x21004004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x2087, // special
    0x7FF, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 25, 25, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(bomb_event)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 15, 0)
    CALL(btlevtcmd_OnAttribute, -2, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 33554432)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_snd_sfxoff, LW(0))
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENEMY_DIE1_2"), -250000000, 0, -250000000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENM_BOMB_ATTACK2"), -250000000, 0, -250000000)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("sandars"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_StageDispellFog)
    CALL(btlevtcmd_GetCoin, -2, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, -2, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_MS(1000)
    IF_INT_EQUAL(LW(15), 1)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, -1)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_ENM_BOMB_MOVE2"), STRING("SFX_ENM_BOMB_MOVE2"), 0, 4, 4)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_ENM_BOMB_MOVE2"), STRING("SFX_ENM_BOMB_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_OR_EQUAL(24)
            CASE_OR_EQUAL(27)
            CASE_OR_EQUAL(25)
            SET_VALUE(LW(15), 1)
            CALL_CHILD(PTR(bomb_event))
            RETURN
            CASE_END
        END_SWITCH
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckStatus, -2, 27, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, -2, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(change_spark_event))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(change_spark_event)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 39)
    CALL(btlevtcmd_CheckDamageCode, -2, 256, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetHpDamage, -2, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_SPARK1"), -250000000, 0, -250000000)
        CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event_spark))
        CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event_spark))
        CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event_spark))
        CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event_spark))
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_spark))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 39)
        CALL(btlevtcmd_OnPartsCounterAttribute, -2, 1, 8192)
        CALL(btlevtcmd_ChangeDataTable, -2, PTR(data_table_spark))
    END_IF
    LABEL(99)
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
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_bomhei_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_bomhei_attack), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 60)
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 90)
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("BOM_N_1"))
    WAIT_MS(500)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_bomhei_attack), 256, LW(5))
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(2.0f), FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 75)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE3"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(1.0f), FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 100)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE3"), -250000000, 0, -250000000)
    GOTO(98)
    LABEL(98)
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_ENM_BOMB_MOVE4"), STRING("SFX_ENM_BOMB_MOVE4"), 0, 10, 10, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -2, 1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_spark)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
        CALL_CHILD(PTR(bomb_event))
        ELSE
        CALL_CHILD(PTR(btldefaultevt_Damage))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_spark)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_bomhei_bomb), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_MOVE1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_bomhei_bomb), 256, LW(5))
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
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 500)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    SET_VALUE(LW(15), 0)
    CALL_CHILD(bomb_event)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(1000)
    RETURN
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_bomhei_bomb), 256, LW(5))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(bomb_event)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(1000)
    RETURN
    LABEL(98)
    WAIT_MS(1000)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_spark)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(15))
    IF_INT_EQUAL(LW(15), -1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BOMB_WAIT1"), -250000000, 0, LW(15))
        CALL(btlevtcmd_SetUnitWork, -2, 0, LW(15))
    END_IF
    RETURN
EVT_END()

