#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"

#pragma force_active on

USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_CheckPhase);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetMaxMoveCount);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(btlevtcmd_SetStayPose);
USER_FUNC(btlevtcmd_SetTalkPose);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_StopWaitEvent);
USER_FUNC(btlevtcmd_WaitEventEnd);
USER_FUNC(btlevtcmd_onoff_battleflag);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveto);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);

f32 fd_lecture_frankli_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_neg_one_382;
f32 fd_lecture_frankli___local_PI_383;
f32 fd_lecture_frankli___local_3PIO2_385;
f32 fd_lecture_frankli_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli___local_PIO2_384;
f32 fd_lecture_frankli_one_381;
f32 fd_lecture_frankli_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_neg_one_365;
f32 fd_lecture_frankli___local_PI_366;
f32 fd_lecture_frankli___local_3PIO2_368;
f32 fd_lecture_frankli_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli___local_PIO2_367;
f32 fd_lecture_frankli_one_364;
BattleUnitKind unit_lecture_frankli;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
BattleWeapon _lec_weapon_jump;
BattleWeapon _lec_weapon_hammer;
static BattleDataEntry data_table[4];
static BattleUnitKindPart parts[1];
static EvtWord init_event[38];
static EvtWord damage_event[10];
static EvtWord phase_event[681];
static EvtWord attack_event[5];
static EvtWord wait_event[6];
EvtWord lecAttackEvent_Jump[735];
EvtWord lecAttackEvent_Hammer[651];
EvtWord lecture_end_event[51];

f32 fd_lecture_frankli_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_lecture_frankli_neg_one_382 = -1.0f;

f32 fd_lecture_frankli___local_PI_383 = 3.1415927f;

f32 fd_lecture_frankli___local_3PIO2_385 = 4.712389f;

f32 fd_lecture_frankli_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_lecture_frankli___local_PIO2_384 = 1.5707964f;

f32 fd_lecture_frankli_one_381 = 1.0f;

f32 fd_lecture_frankli_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_lecture_frankli_neg_one_365 = -1.0f;

f32 fd_lecture_frankli___local_PI_366 = 3.1415927f;

f32 fd_lecture_frankli___local_3PIO2_368 = 4.712389f;

f32 fd_lecture_frankli_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_lecture_frankli___local_PIO2_367 = 1.5707964f;

f32 fd_lecture_frankli_one_364 = 1.0f;

BattleUnitKind unit_lecture_frankli = {
    178,
    "btl_un_frankli",
    10,
    0,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    28,
    24,
    5,
    25,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    30.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_KURI_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x12000000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {27, "D_1"},
    {28, "S_1"},
    {31, "D_2"},
    {39, "D_2"},
    {69, "S_1"},
};

BattleWeapon _lec_weapon_jump = {
    "btl_wn_mario_normal_jump", // name
#ifdef VERSION_G8ME01
    167, // icon
#else
    159, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_normal_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x35807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    lecAttackEvent_Jump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon _lec_weapon_hammer = {
    "btl_wn_mario_normal_hammer", // name
#ifdef VERSION_G8ME01
    164, // icon
#else
    156, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_normal_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x30887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    lecAttackEvent_Hammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleDataEntry data_table[] = {
    {68, &_lec_weapon_jump},
    {69, &_lec_weapon_hammer},
    {70, lecture_end_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_frankli", "c_kuri_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x90009, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_onoff_battleflag, 1, 4096)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, 0, 58, 256, 0, 41, 0, 30, 11)
            WAIT_MS(500)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_000"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_onoff_battleflag, 1, 16777216)
            CALL(btlevtcmd_StartWaitEvent, -3)
            RETURN
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetUnitWork, -2, 1, 3)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_003"), 0, -2)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 5, 0, 180, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_004"), 0, -2)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
            CALL(btlevtcmd_StopWaitEvent, LW(3))
            CALL(btlevtcmd_SetTalkPose, LW(3), STRING("PKR_T_1"))
            CALL(btlevtcmd_SetStayPose, LW(3), STRING("PKR_S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_005"), 0, LW(3))
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(btlevtcmd_StartWaitEvent, -3)
            BEGIN_INLINE_EVENT
                CALL(evt_sub_intpl_init, 5, 180, 0, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_INLINE_EVENT
            RETURN
        END_IF
        CASE_EQUAL(4)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetUnitWork, -2, 1, 5)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_007"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(btlevtcmd_onoff_battleflag, 0, 16777216)
            CALL(btlevtcmd_onoff_battleflag, 1, 33554432)
            RETURN
        END_IF
        CASE_EQUAL(6)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetUnitWork, -2, 1, 7)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 5, 180, 0, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -6, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -6, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_011"), 0, -2)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 5, 0, 180, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_012"), 0, -2)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
            CALL(btlevtcmd_StopWaitEvent, LW(3))
            CALL(btlevtcmd_SetTalkPose, LW(3), STRING("PKR_T_1"))
            CALL(btlevtcmd_SetStayPose, LW(3), STRING("PKR_S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_013"), 0, LW(3))
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(btlevtcmd_StartWaitEvent, -3)
            BEGIN_INLINE_EVENT
                CALL(evt_sub_intpl_init, 5, 180, 0, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_INLINE_EVENT
            RETURN
        END_IF
        CASE_EQUAL(8)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetUnitWork, -2, 1, 9)
            CALL(btlevtcmd_onoff_battleflag, 0, 33554432)
            CALL(btlevtcmd_onoff_battleflag, 1, 134217728)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_016"), 0, -2)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, 0, 58, 256, 0, 41, 0, 30, 11)
            WAIT_MS(500)
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_017"), 0, -2)
            CALL(evt_msg_select, 0, STRING("mac_0_b_t2_018"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("mac_0_b_t2_019"))
                CALL(evt_btl_camera_set_mode, 0, 0)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
                RETURN
                ELSE
                CALL(evt_msg_print_add, 0, STRING("mac_0_b_t2_020"))
                CALL(evt_btl_camera_set_mode, 0, 0)
                CALL(btlevtcmd_onoff_battleflag, 1, 32)
                RETURN
            END_IF
        END_IF
        CASE_EQUAL(9)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_GetUnitWork, -6, 3, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(4)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_021"), 0, -2)
                CASE_EQUAL(5)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_022"), 0, -2)
                CASE_EQUAL(6)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_023"), 0, -2)
                CASE_EQUAL(7)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_021"), 0, -2)
            END_SWITCH
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_sub_intpl_init, 5, 180, 0, 20)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, -6, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -6, 0, 0, 0)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_GetUnitWork, -6, 2, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(1)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_021"), 0, -2)
                CASE_EQUAL(2)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_022"), 0, -2)
                CASE_EQUAL(3)
                CALL(evt_msg_print, 2, STRING("mac_0_b_t2_023"), 0, -2)
            END_SWITCH
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            RETURN
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

EVT_BEGIN(lecAttackEvent_Jump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetTalkPose, -6, STRING("T_1"))
        CALL(btlevtcmd_SetStayPose, -6, STRING("S_1"))
        CALL(evt_msg_print, 2, STRING("mac_0_b_t2_001"), 0, -6)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 36, 3, -1)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), -250000000, 0, -250000000)
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 36, 0, -1)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        IF_INT_LESS_OR_EQUAL(LW(10), 0)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 5, 0, 180, 20)
                LOOP(21)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -6, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -6, 0, 180, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
        END_IF
        CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
        CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
        DIVIDE(LW(0), 2)
        WAIT_FRAMES(LW(0))
        IF_INT_LESS_OR_EQUAL(LW(10), 0)
            CALL(btlevtcmd_SetTalkPose, -6, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -6, STRING("S_1"))
            CALL(evt_msg_print, 2, STRING("mac_0_b_t2_002"), 0, -6)
            WAIT_FRAMES(1)
            LABEL(11)
            CALL(btlevtcmd_PadCheckTrigger, 0, 256, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                WAIT_FRAMES(1)
                GOTO(11)
            END_IF
            WAIT_MS(150)
            CALL(evt_msg_continue)
        END_IF
    ELSE
        CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
        CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
        DIVIDE(LW(0), 2)
        WAIT_FRAMES(LW(0))
    END_IF
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_SetUnitWork, -6, 2, 0)
        CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
        IF_INT_EQUAL(LW(0), 9)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(3)
                CALL(btlevtcmd_SetUnitWork, -6, 2, 1)
                CASE_EQUAL(1)
                CALL(btlevtcmd_SetUnitWork, -6, 2, 2)
                CASE_EQUAL(2)
                CALL(btlevtcmd_SetUnitWork, -6, 2, 3)
            END_SWITCH
        END_IF
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, -250000000, LW(1), -250000000)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_snd_voice, -2, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS_OR_EQUAL(LW(6), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131072, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        WAIT_FRAMES(2)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
        WAIT_FRAMES(2)
    END_IF
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            CALL(evt_sub_intpl_init, 5, 180, 0, 20)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, -6, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -6, 0, 0, 0)
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, -250000000, LW(1), -250000000)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, -6, 1, 2)
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(lecAttackEvent_Hammer)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        CALL(btlevtcmd_SetTalkPose, -6, STRING("T_1"))
        CALL(btlevtcmd_SetStayPose, -6, STRING("S_1"))
        CALL(evt_msg_print, 2, STRING("mac_0_b_t2_008"), 0, -6)
        LABEL(1)
        CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(1)
        END_IF
        CALL(evt_msg_continue)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    BEGIN_SIBLING_EVENT
        LABEL(2)
        CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(2)
        END_IF
        CALL(evt_sub_intpl_init, 5, 0, 180, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -6, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -6, 0, 180, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
        CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, -250000000, -250000000, -250000000)
        CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
        IF_INT_EQUAL(LW(0), 5)
            CALL(btlevtcmd_AcSetFlag, 2)
        ELSE
            CALL(btlevtcmd_AcSetFlag, 0)
        END_IF
        CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, -250000000, LW(1), -250000000)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 350)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_1A"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), -250000000, 0, -250000000)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_1B"))
        CALL(btlevtcmd_StartAC, 1)
        CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
        IF_INT_EQUAL(LW(0), 5)
            LABEL(5)
            CALL(btlevtcmd_AcGetParam, 4, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                WAIT_FRAMES(1)
                GOTO(5)
            END_IF
        END_IF
    END_SIBLING_EVENT
    SET_VALUE(LW(14), 0)
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            SET_VALUE(LW(5), 5)
            LOOP(LW(5))
                CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    IS_EVENT_RUNNING(LW(15), LW(0))
                    IF_INT_EQUAL(LW(0), 0)
                    ELSE
                        SUSPEND_EVENT(LW(15))
                        CALL(btlevtcmd_AcSetParam, 2, 1)
                        CALL(btlevtcmd_SetTalkPose, -6, STRING("T_1"))
                        CALL(btlevtcmd_SetStayPose, -6, STRING("S_1"))
                        CALL(evt_msg_print, 2, STRING("mac_0_b_t2_009"), 0, -6)
                        LABEL(6)
                        CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            WAIT_FRAMES(1)
                            GOTO(6)
                        END_IF
                        WAIT_MS(200)
                        CALL(evt_msg_continue)
                        CALL(btlevtcmd_AcSetParam, 2, 0)
                        RESUME_EVENT(LW(15))
                    END_IF
                END_IF
                WAIT_FRAMES(1)
                SET_VALUE(LW(5), 5)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    IF_INT_NOT_EQUAL(LW(14), 0)
        DELETE_EVENT(LW(14))
    END_IF
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        CALL(btlevtcmd_SetTalkPose, -6, STRING("T_1"))
        CALL(btlevtcmd_SetStayPose, -6, STRING("S_1"))
        CALL(evt_msg_print, 2, STRING("mac_0_b_t2_010"), 0, -6)
        LABEL(10)
        CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_ResultAC)
        WAIT_MS(200)
        CALL(evt_msg_continue)
    ELSE
        CALL(btlevtcmd_ResultAC)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -6, 2, 0)
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 9)
        CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(3)
            CALL(btlevtcmd_SetUnitWork, -6, 2, 1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_SetUnitWork, -6, 2, 2)
            CASE_EQUAL(2)
            CALL(btlevtcmd_SetUnitWork, -6, 2, 3)
        END_SWITCH
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_1C"))
    WAIT_FRAMES(7)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND2"), 0)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    WAIT_FRAMES(10)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_GetUnitWork, -6, 1, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        CALL(btlevtcmd_SetUnitWork, -6, 1, 6)
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(lecture_end_event)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 0, 58, 256, 0, 41, 0, 30, 11)
    WAIT_MS(500)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
    CALL(evt_msg_print, 2, STRING("mac_0_b_t2_024"), 0, -2)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_onoff_battleflag, 1, 32)
    RETURN
EVT_END()

