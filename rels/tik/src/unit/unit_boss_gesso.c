#include "eff/n64/eff_opuku_jetw_n64.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "battle/battle_pad.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

EffectEntry* effOpukuJetwN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale);

#ifdef VERSION_G8ME01
USER_FUNC(btlevtcmd_CheckActStatus);
#else
USER_FUNC(_test_rtn);
#endif
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeSetPoseTable);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_KillUnit);
USER_FUNC(btlevtcmd_OnAttribute);
USER_FUNC(btlevtcmd_OnPartsAttribute);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetHitCursorOffset);
USER_FUNC(btlevtcmd_SetHitOffset);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_SetHpGaugeOffset);
USER_FUNC(btlevtcmd_SetPartsDefenceAttrTable);
USER_FUNC(btlevtcmd_SetPartsDefenceTable);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetRegistStatus);
USER_FUNC(btlevtcmd_SetStatusIconOffset);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_WaitEventEnd);
USER_FUNC(_ikasumi_effect);
extern BattleUnitKind unit_boss_gesso_left_arm;
extern BattleUnitKind unit_boss_gesso_right_arm;

f32 fd_boss_gesso_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_boss_gesso_neg_one_382;
f32 fd_boss_gesso___local_PI_383;
f32 fd_boss_gesso___local_3PIO2_385;
f32 fd_boss_gesso_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_boss_gesso___local_PIO2_384;
f32 fd_boss_gesso_one_381;
f32 fd_boss_gesso_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_boss_gesso_neg_one_365;
f32 fd_boss_gesso___local_PI_366;
f32 fd_boss_gesso___local_3PIO2_368;
f32 fd_boss_gesso_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_boss_gesso___local_PIO2_367;
f32 fd_boss_gesso_one_364;
BattleUnitKind unit_boss_gesso;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
BattleUnitDefense defence_ceil;
BattleUnitDefenseAttr defence_attr_ceil;
StatusVulnerability regist_ceil;
s32 R_defence_turn[2];
s32 R_defence_attr_turn[2];
s32 R_regist_turn[6];
static PoseTableEntry pose_table[11];
static PoseTableEntry pose_table_ceil[12];
PoseTableEntry gesso_pose_table_turn[10];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
#ifndef VERSION_G8ME01
EvtWord gesso_arm_wait_event[4];
#endif
#ifdef VERSION_G8ME01
static EvtWord battle_entry_event[313];
#else
static EvtWord battle_entry_event[315];
#endif
static EvtWord init_event[121];
BattleUnitSetup entry_gesso_left_arm_entry_data;
BattleUnitSetup entry_gesso_right_arm_entry_data;
static EvtWord damage_event[34];
#ifdef VERSION_G8ME01
static EvtWord phase_event[497];
#else
static EvtWord phase_event[488];
#endif
static EvtWord attack_event[48];
BattleWeapon weapon_gesso_ikasumi_attack;
EvtWord gesso_ikasumi_attack[193];
static EvtWord wait_event[6];
EvtWord dead_subevent[48];

f32 fd_boss_gesso_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_boss_gesso_neg_one_382 = -1.0f;

f32 fd_boss_gesso___local_PI_383 = 3.1415927f;

f32 fd_boss_gesso___local_3PIO2_385 = 4.712389f;

f32 fd_boss_gesso_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_boss_gesso___local_PIO2_384 = 1.5707964f;

f32 fd_boss_gesso_one_381 = 1.0f;

f32 fd_boss_gesso_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_boss_gesso_neg_one_365 = -1.0f;

f32 fd_boss_gesso___local_PI_366 = 3.1415927f;

f32 fd_boss_gesso___local_3PIO2_368 = 4.712389f;

f32 fd_boss_gesso_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_boss_gesso___local_PIO2_367 = 1.5707964f;

f32 fd_boss_gesso_one_364 = 1.0f;

BattleUnitKind unit_boss_gesso = {
    8,
    "btl_un_gesso",
    12,
    0,
    1,
    1,
    41,
    0,
    0,
    100,
    0,
    50,
    9,
    120,
    120,
    46,
    -75,
    0.0f,
    0.0f,
    0.0f,
    5,
    -175,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    0.0f,
    60.0f,
    78.0f,
    0.0f,
    0.0f,
    120.0f,
    0.0f,
    120.0f,
    110.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GESSO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 100, 100, 0, 0, 0};

BattleUnitDefense defence_ceil = {{2, 2, 2, 2, 2}};

BattleUnitDefenseAttr defence_attr_ceil = {0};

StatusVulnerability regist_ceil = {0};

s32 R_defence_turn[] = {
    0, 0,
};

s32 R_defence_attr_turn[] = {
    0, 0,
};

s32 R_regist_turn[] = {
    1684300900, 1684300900, 1684300900, 1684300800, 6579200, 0,
};

static PoseTableEntry pose_table[] = {
    {1, "GSO_N2_1"},
    {2, "GSO_D2_2"},
    {9, "GSO_D2_2"},
    {5, "GSO_K2_1"},
    {4, "GSO_X2_1"},
    {3, "GSO_K2_1"},
    {28, "GSO_S2_1"},
    {29, "GSO_Q2_1"},
    {31, "GSO_S2_1"},
    {39, "GSO_D2_1"},
    {69, "GSO_S2_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {1, "GSO_N1_1"},
    {2, "GSO_D1_3"},
    {9, "GSO_D1_3"},
    {5, "GSO_K1_1"},
    {4, "GSO_X1_1"},
    {3, "GSO_K1_1"},
    {28, "GSO_S1_1"},
    {29, "GSO_Q1_2"},
    {31, "GSO_S1_1"},
    {39, "GSO_D1_2"},
    {65, "GSO_K1_1"},
    {69, "GSO_S1_1"},
};

PoseTableEntry gesso_pose_table_turn[] = {
    {1, "GSO_N3_1"},
    {2, "GSO_D3_2"},
    {9, "GSO_D3_2"},
    {5, "GSO_K3_1"},
    {4, "GSO_K3_1"},
    {3, "GSO_K3_1"},
    {28, "GSO_V_1"},
    {31, "GSO_D3_1"},
    {39, "GSO_D3_1"},
    {69, "GSO_V_1"},
};

static BattleDataEntry data_table[] = {
    {50, dead_subevent},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gesso", "c_gesso", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 103, -175, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

#ifndef VERSION_G8ME01
EVT_BEGIN(gesso_arm_wait_event)
    CALL(_test_rtn)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_OnAttribute, -2, 16777216)
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_APPEAR1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_F_4"))
    CALL(btlevtcmd_OffAttribute, -2, 16777216)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GSO_X1_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GSO_X1_2"))
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 84, 177, 558, 84, 140, 10, 30, 11)
    WAIT_FRAMES(30)
    CALL(evt_msg_print, 2, STRING("tik_02_gs_01"), 0, -2)
    CALL(evt_snd_sfxon_3d, STRING("SFX_CONDITION_FIND1"), 95, 65, 0, 0)
    CALL(evt_eff_fukidashi, 2, STRING(""), 0, 0, 0, 95, 65, 0, 0, 135, 60)
    WAIT_FRAMES(60)
    CALL(evt_msg_print, 2, STRING("tik_02_gs_02"), 0, -2)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 63, 143, 646, 63, 100, 0, 30, 11)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetUnitId, -2, LW(10))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_left_arm_entry_data), 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), 16777216)
    CALL(btlevtcmd_SetUnitWork, -2, 1, LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
    CALL(btlevtcmd_SetPos, LW(3), 320, 6, 53)
    CALL(btlevtcmd_SetHomePos, LW(3), 320, 6, 53)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        WAIT_FRAMES(35)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), -250000000, 0, -250000000)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
        CALL(btlevtcmd_OffAttribute, LW(3), 16777216)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_INLINE_EVENT
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_right_arm_entry_data), 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), 16777216)
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
    CALL(btlevtcmd_SetPos, LW(3), 59, 166, -18)
    CALL(btlevtcmd_SetHomePos, LW(3), 59, 166, -18)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), -250000000, 0, -250000000)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
        CALL(btlevtcmd_OffAttribute, LW(3), 16777216)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
#else
    SPAWN_EVENT(gesso_arm_wait_event)
#endif
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 84, 177, 558, 84, 140, 10, 30, 11)
    WAIT_FRAMES(30)
    CALL(evt_msg_print, 2, STRING("tik_02_gs_03"), 0, -2)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, -117, 62, 284, -115, 43, 0, 30, 11)
    WAIT_MS(500)
    CALL(btlevtcmd_SetTalkPose, -4, STRING("PKR_T_1"))
    CALL(btlevtcmd_SetStayPose, -4, STRING("PKR_S_1"))
    CALL(evt_msg_print, 2, STRING("tik_02_gs_03_01"), 0, -4)
    CALL(btlevtcmd_StartWaitEvent, -4)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 5, 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 220)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OnAttribute, -2, 2)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 6291456)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_ceil))
    CALL(btlevtcmd_SetHpGaugeOffset, -2, 5, -175)
    CALL(btlevtcmd_SetStatusIconOffset, -2, 46, -75)
    CALL(btlevtcmd_SetHitOffset, -2, 1, 9, -122, 30)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 9, -122, 30)
    CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&defence_ceil))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, -2, 1, PTR(&defence_attr_ceil))
    CALL(btlevtcmd_SetRegistStatus, -2, PTR(&regist_ceil))
    CALL(btlevtcmd_SetStatusIconOffset, -2, 46, -75)
    CALL(btlevtcmd_OnAttribute, -2, 16777216)
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

BattleUnitSetup entry_gesso_left_arm_entry_data = {
    &unit_boss_gesso_left_arm, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_gesso_right_arm_entry_data = {
    &unit_boss_gesso_right_arm, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 1)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 4, 3)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
#endif
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
            ADD(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 2)
                CALL(btlevtcmd_PhaseEventStartDeclare, -2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_U_1"))
                CALL(btlevtcmd_ftof, 75, LW(0))
                WAIT_FRAMES(LW(0))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_WAKE1"), -250000000, 0, -250000000)
                CALL(btlevtcmd_ftof, 45, LW(0))
                WAIT_FRAMES(LW(0))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_WAKE2"), -250000000, 0, -250000000)
                CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
                CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
                CALL(btlevtcmd_SetHpGaugeOffset, -2, 0, 0)
                CALL(btlevtcmd_SetStatusIconOffset, -2, 46, 118)
                CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&defence))
                CALL(btlevtcmd_SetPartsDefenceAttrTable, -2, 1, PTR(&defence_attr))
                CALL(btlevtcmd_SetRegistStatus, -2, PTR(&regist))
                CALL(btlevtcmd_SetStatusIconOffset, -2, 55, 113)
                CALL(btlevtcmd_SetUnitWork, -2, 0, 2)
                CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
                CALL(btlevtcmd_StartWaitEvent, -2)
                RETURN
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108869)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 2)
            CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
            ADD(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 2)
                CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
                CALL(btlevtcmd_PhaseEventStartDeclare, -2)
                CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
                CALL(btlevtcmd_OnAttribute, -2, 2)
                CALL(btlevtcmd_OnPartsAttribute, -2, 1, 6291456)
                CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_ceil))
                CALL(btlevtcmd_SetHpGaugeOffset, -2, 5, -175)
                CALL(btlevtcmd_SetStatusIconOffset, -2, 46, -75)
                CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
                CALL(btlevtcmd_SetHitOffset, -2, 1, 9, -122, 30)
                CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 9, -122, 30)
                CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&defence_ceil))
                CALL(btlevtcmd_SetPartsDefenceAttrTable, -2, 1, PTR(&defence_attr_ceil))
                CALL(btlevtcmd_SetRegistStatus, -2, PTR(&regist_ceil))
                CALL(btlevtcmd_SetStatusIconOffset, -2, 46, -75)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_J_1"))
                WAIT_FRAMES(48)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_J_2"))
                SET_VALUE(LW(5), 0)
                LOOP(4)
                    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                    SWITCH(LW(5))
                        CASE_EQUAL(0)
                        ADD(LW(1), 50)
                        CASE_EQUAL(1)
                        ADD(LW(1), 75)
                        CASE_DEFAULT
                        ADD(LW(1), 100)
                    END_SWITCH
                    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_UP1"), -250000000, 0, -250000000)
                    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, 0, 5, 0, -1)
                    ADD(LW(5), 1)
                    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                    SUBTRACT(LW(1), 30)
                    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 16, 0, 11, 0, -1)
                END_LOOP
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                SET_VALUE(LW(1), 220)
                CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_APPEAR1"), -250000000, 0, -250000000)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_F_4"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
                WAIT_MS(333)
                CALL(btlevtcmd_GetUnitId, -2, LW(10))
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_left_arm_entry_data), 0)
                CALL(btlevtcmd_StopWaitEvent, LW(3))
                CALL(btlevtcmd_OnAttribute, LW(3), 16777216)
                CALL(btlevtcmd_SetUnitWork, -2, 1, LW(3))
                CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
                CALL(btlevtcmd_SetPos, LW(3), 320, 6, 53)
                CALL(btlevtcmd_SetHomePos, LW(3), 320, 6, 53)
                BEGIN_INLINE_EVENT_GET_ID(LW(14))
                    WAIT_FRAMES(35)
                    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), -250000000, 0, -250000000)
                    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
                    CALL(btlevtcmd_OffAttribute, LW(3), 16777216)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
                    CALL(btlevtcmd_StartWaitEvent, LW(3))
                END_INLINE_EVENT
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_right_arm_entry_data), 0)
                CALL(btlevtcmd_StopWaitEvent, LW(3))
                CALL(btlevtcmd_OnAttribute, LW(3), 16777216)
                CALL(btlevtcmd_SetUnitWork, -2, 2, LW(3))
                CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
                CALL(btlevtcmd_SetPos, LW(3), 59, 166, -18)
                CALL(btlevtcmd_SetHomePos, LW(3), 59, 166, -18)
                BEGIN_INLINE_EVENT_GET_ID(LW(15))
                    WAIT_FRAMES(5)
                    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), -250000000, 0, -250000000)
                    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
                    CALL(btlevtcmd_OffAttribute, LW(3), 16777216)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
                    CALL(btlevtcmd_StartWaitEvent, LW(3))
                END_INLINE_EVENT
                CALL(btlevtcmd_StartWaitEvent, -2)
                CALL(btlevtcmd_WaitEventEnd, LW(14))
                CALL(btlevtcmd_WaitEventEnd, LW(15))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(gesso_ikasumi_attack))
    RETURN
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

BattleWeapon weapon_gesso_ikasumi_attack = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, // element
    13, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0x7FF, // counterResistFlags
    4, // targetWeightFlags
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

EVT_BEGIN(gesso_ikasumi_attack)
    SET_VALUE(LW(12), PTR(&weapon_gesso_ikasumi_attack))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_A2_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 16, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_UP1"), -250000000, 0, -250000000)
        CALL(btlevtcmd_ftof, 55, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_UP1"), -250000000, 0, -250000000)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftof, 120, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ftof, 90, LW(7))
    CALL(_ikasumi_effect, -2, LW(7))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_SUMI1"), -250000000, 0, LW(15))
        WAIT_FRAMES(LW(7))
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftof, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(10), 0)
    LABEL(50)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        SWITCH(LW(5))
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    ADD(LW(10), 1)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

USER_FUNC(_ikasumi_effect) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 count = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    Vec pos;
    EffectEntry* eff;

    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x -= 95.0f;
    pos.y += 75.0f;
    eff = effOpukuJetwN64Entry(0, count, pos.x, pos.y, pos.z, 2.0f);
    ((EffOpukuJetwN64Work*)eff->userdata)->rotZ = 135.0f;
    ((EffOpukuJetwN64Work*)eff->userdata)->colorR = 0;
    ((EffOpukuJetwN64Work*)eff->userdata)->colorG = 0;
    ((EffOpukuJetwN64Work*)eff->userdata)->colorB = 0;
    ((EffOpukuJetwN64Work*)eff->userdata)->color2R = 0;
    ((EffOpukuJetwN64Work*)eff->userdata)->color2G = 0;
    ((EffOpukuJetwN64Work*)eff->userdata)->color2B = 0;
    return EVT_RETURN_DONE;
}

#ifndef VERSION_G8ME01
USER_FUNC(_test_rtn) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit;
    BattleUnit* target;
    s32 targetId;

    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    // debug: pick a unit with the d-pad, then scale and move it
    if (BattlePadMultiCheckTrigger(2, 1)) {
        event->userdata[0]++;
        if (event->userdata[0] >= 3) {
            event->userdata[0] = 0;
        }
    }
    if (BattlePadMultiCheckTrigger(2, 2)) {
        event->userdata[0]--;
        if (event->userdata[0] < 0) {
            event->userdata[0] = 2;
        }
    }
    switch (event->userdata[0]) {
        case 0:
            targetId = BattleTransID(event, -2);
            break;
        case 1:
            targetId = unit->work[1];
            break;
        case 2:
            targetId = unit->work[2];
            break;
    }
    target = BattleGetUnitPtr(wp, targetId);
    BattlePadMultiCheckTrigger(2, 3);
    if (BattlePadMultiCheckNow(2, 0x40)) {
        target->mScale.x += 0.02f;
        target->mScale.y += 0.02f;
        target->mScale.z += 0.02f;
    }
    if (BattlePadMultiCheckNow(2, 0x20)) {
        target->mScale.x -= 0.02f;
        target->mScale.y -= 0.02f;
        target->mScale.z -= 0.02f;
    }
    target->mPosition.x += 0.1f * keyGetStickX(2);
    target->mPosition.y += 0.1f * keyGetSubStickY(2);
    target->mPosition.z -= 0.1f * keyGetStickY(2);
    BattlePadMultiCheckTrigger(2, 0x100);
    return EVT_RETURN_BLOCK;
}
#endif

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

EVT_BEGIN(dead_subevent)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(3))
    CALL(btlevtcmd_GetUnitId, LW(3), LW(3))
    BEGIN_SIBLING_EVENT_GET_ID(LW(1))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_KillUnit, LW(3), 0)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(3))
    CALL(btlevtcmd_GetUnitId, LW(3), LW(3))
    BEGIN_SIBLING_EVENT_GET_ID(LW(2))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_KillUnit, LW(3), 0)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(1))
    CALL(btlevtcmd_WaitEventEnd, LW(2))
    RETURN
EVT_END()

