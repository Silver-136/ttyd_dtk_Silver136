#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "battle/battle_database_common.h"

#pragma force_active on

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_GetHp);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(evt_sub_random);

f32 fd_pansy_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_pansy_neg_one_382;
f32 fd_pansy___local_PI_383;
f32 fd_pansy___local_3PIO2_385;
f32 fd_pansy_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_pansy___local_PIO2_384;
f32 fd_pansy_one_381;
f32 fd_pansy_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_pansy_neg_one_365;
f32 fd_pansy___local_PI_366;
f32 fd_pansy___local_3PIO2_368;
f32 fd_pansy_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_pansy___local_PIO2_367;
f32 fd_pansy_one_364;
BattleUnitKind unit_pansy;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
BattleWeapon weapon_pansy_attack;
static EvtWord attack_event[58];
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord song_event[300];
static EvtWord escape_event[96];
static EvtWord wait_event[6];

f32 fd_pansy_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pansy_neg_one_382 = -1.0f;

f32 fd_pansy___local_PI_383 = 3.1415927f;

f32 fd_pansy___local_3PIO2_385 = 4.712389f;

f32 fd_pansy_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pansy___local_PIO2_384 = 1.5707964f;

f32 fd_pansy_one_381 = 1.0f;

f32 fd_pansy_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pansy_neg_one_365 = -1.0f;

f32 fd_pansy___local_PI_366 = 3.1415927f;

f32 fd_pansy___local_3PIO2_368 = 4.712389f;

f32 fd_pansy_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pansy___local_PIO2_367 = 1.5707964f;

f32 fd_pansy_one_364 = 1.0f;

BattleUnitKind unit_pansy = {
    69,
    "btl_un_pansy",
    7,
    0,
    1,
    1,
    16,
    0,
    2,
    70,
    1,
    70,
    9999,
    22,
    32,
    5,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    11.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    11.0f,
    0.0f,
    0.0f,
    6.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    22.0f,
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
    "SFX_ENM_PANSY_DAMAGED1",
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {95, 30, 30, 100, 50, 100, 100, 30, 100, 30, 100, 30, 100, 30, 30, 100, 80, 100, 30, 100, 100, 30};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_pansy", "c_pansy", 0.0f, 0.0f, 0.0f, 0.0f, 29.0f, 0.0f, 0.0f, 34.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

BattleWeapon weapon_pansy_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x47, // special
    0x7FF, // counterResistFlags
    0x80000000, // targetWeightFlags
#ifdef VERSION_G8ME01
    50, // sleepChance
#else
    75, // sleepChance
#endif
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetHp, -2, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(10)
    END_IF
    SET_VALUE(LW(0), 70)
    ADD(LW(0), 30)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 70)
        SET_VALUE(LW(9), PTR(&weapon_pansy_attack))
        CALL_CHILD(PTR(song_event))
        GOTO(99)
    END_IF
    LABEL(10)
    CALL_CHILD(PTR(escape_event))
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

static PoseTableEntry pose_table[] = {
    {1, "PAN_N_1"},
    {2, "PAN_Y_1"},
    {9, "PAN_Y_1"},
    {5, "PAN_K_1"},
    {4, "PAN_X_1"},
    {3, "PAN_X_1"},
    {28, "PAN_S_1"},
    {29, "PAN_Q_1"},
    {30, "PAN_Q_1"},
    {31, "PAN_S_1"},
    {39, "PAN_D_1"},
    {50, "PAN_A_1"},
    {42, "PAN_R_1"},
    {40, "PAN_W_1"},
    {56, "PAN_X_1"},
    {57, "PAN_X_1"},
    {65, "PAN_T_1"},
    {69, "PAN_S_1"},
};

static BattleDataEntry data_table[] = {
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(song_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    SET_VALUE(LW(0), 255)
    SET_VALUE(LW(1), 255)
    LOOP(0)
        IF_INT_LESS(LW(0), 192)
            BREAK_LOOP
        END_IF
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(1), 255)
        SUBTRACT(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PANSY_ATTACK1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 50)
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            IF_INT_LESS(LW(15), 0)
                CALL(evt_eff64, STRING(""), STRING("onpu_n64"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            ELSE
                CALL(evt_eff64, STRING(""), STRING("onpu_n64"), 2, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            END_IF
            WAIT_FRAMES(12)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
        GOTO(97)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(10)
    LABEL(98)
    SET_VALUE(LW(0), 192)
    SET_VALUE(LW(1), 255)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(1), 255)
        ADD(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_blend_off, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(500)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(escape_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PANSY_ESCAPE1_1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PAN_E_1"))
    CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 255, 100, 100, 100, 100, 0)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 67108864)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PANSY_ESCAPE1_2"), -250000000, 0, -250000000)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), FLOAT(20.0f), -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PANSY_ESCAPE2"), -250000000, 0, -250000000)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, -2, 300, LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
    WAIT_MS(500)
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

