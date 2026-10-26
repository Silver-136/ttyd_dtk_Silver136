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
#include "battle/battle_damage.h"


#ifdef VERSION_G8ME01
#else
USER_FUNC(_test_rtn);
#endif
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(_ikasumi_effect);
extern BattleUnitKind unit_boss_gesso_left_arm;
extern BattleUnitKind unit_boss_gesso_right_arm;

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

BattleUnitKind unit_boss_gesso = {
    UNIT_BLOOPER, // mUnitType
    "btl_un_gesso", // name
    12, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    41, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    120, // width
    120, // height
    {46, -75}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {5, -175}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {60.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {60.0f, 0.0f, 0.0f}, // bintaHitOffset
    {60.0f, 78.0f, 0.0f}, // kissHitOffset
    {0.0f, 120.0f, 0.0f}, // cutBaseOffset
    120.0f, // cutWidth
    110.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_GESSO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    100, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

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
    {POSE_TYPE_SLEEP, "GSO_N2_1"},
    {POSE_TYPE_STOP, "GSO_D2_2"},
    {POSE_TYPE_FREEZE, "GSO_D2_2"},
    {POSE_TYPE_CONFUSE, "GSO_K2_1"},
    {POSE_TYPE_POISON, "GSO_X2_1"},
    {POSE_TYPE_DIZZY, "GSO_K2_1"},
    {POSE_TYPE_NORMAL, "GSO_S2_1"},
    {POSE_TYPE_DAMAGE_FIRE, "GSO_Q2_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "GSO_S2_1"},
    {POSE_TYPE_DAMAGE, "GSO_D2_1"},
    {POSE_TYPE_ACTION, "GSO_S2_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "GSO_N1_1"},
    {POSE_TYPE_STOP, "GSO_D1_3"},
    {POSE_TYPE_FREEZE, "GSO_D1_3"},
    {POSE_TYPE_CONFUSE, "GSO_K1_1"},
    {POSE_TYPE_POISON, "GSO_X1_1"},
    {POSE_TYPE_DIZZY, "GSO_K1_1"},
    {POSE_TYPE_NORMAL, "GSO_S1_1"},
    {POSE_TYPE_DAMAGE_FIRE, "GSO_Q1_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "GSO_S1_1"},
    {POSE_TYPE_DAMAGE, "GSO_D1_2"},
    {POSE_TYPE_TALK, "GSO_K1_1"},
    {POSE_TYPE_ACTION, "GSO_S1_1"},
};

PoseTableEntry gesso_pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "GSO_N3_1"},
    {POSE_TYPE_STOP, "GSO_D3_2"},
    {POSE_TYPE_FREEZE, "GSO_D3_2"},
    {POSE_TYPE_CONFUSE, "GSO_K3_1"},
    {POSE_TYPE_POISON, "GSO_K3_1"},
    {POSE_TYPE_DIZZY, "GSO_K3_1"},
    {POSE_TYPE_NORMAL, "GSO_V_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "GSO_D3_1"},
    {POSE_TYPE_DAMAGE, "GSO_D3_1"},
    {POSE_TYPE_ACTION, "GSO_V_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_PRE_DEAD_EVENT, dead_subevent},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gesso", "c_gesso", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 103, -175, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

#ifndef VERSION_G8ME01
EVT_BEGIN(gesso_arm_wait_event)
    CALL(_test_rtn)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_F_4"))
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GSO_X1_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GSO_X1_2"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 84, 177, 558, 84, 140, 10, 30, 11)
    WAIT_FRAMES(30)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_01"), 0, -2)
    CALL(evt_snd_sfxon_3d, STRING("SFX_CONDITION_FIND1"), 95, 65, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, 95, 65, 0, 0, 135, 60)
    WAIT_FRAMES(60)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_02"), 0, -2)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 143, 646, 63, 100, 0, 30, 11)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_left_arm_entry_data), 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
    CALL(btlevtcmd_SetPos, LW(3), 320, 6, 53)
    CALL(btlevtcmd_SetHomePos, LW(3), 320, 6, 53)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        WAIT_FRAMES(35)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitHidden)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_INLINE_EVENT
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_right_arm_entry_data), 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
    CALL(btlevtcmd_SetPos, LW(3), 59, 166, -18)
    CALL(btlevtcmd_SetHomePos, LW(3), 59, 166, -18)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitHidden)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
#else
    SPAWN_EVENT(gesso_arm_wait_event)
#endif
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 84, 177, 558, 84, 140, 10, 30, 11)
    WAIT_FRAMES(30)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_03"), 0, -2)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -117, 62, 284, -115, 43, 0, 30, 11)
    WAIT_MS(500)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_PARTNER, STRING("PKR_T_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_PARTNER, STRING("PKR_S_1"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_03_01"), 0, -4)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 220)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitOnCeiling)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_ceil))
    CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, 5, -175)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 46, -75)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 9, -122, 30)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 9, -122, 30)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_ceil))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_ceil))
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_ceil))
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 46, -75)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

BattleUnitSetup entry_gesso_left_arm_entry_data = {
    &unit_boss_gesso_left_arm, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_gesso_right_arm_entry_data = {
    &unit_boss_gesso_right_arm, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 1)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 3)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
#endif
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(0))
            ADD(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 2)
                CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_U_1"))
                CALL(btlevtcmd_ftof, 75, LW(0))
                WAIT_FRAMES(LW(0))
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_WAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_ftof, 45, LW(0))
                WAIT_FRAMES(LW(0))
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_WAKE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
                CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, 0, 0)
                CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 46, 118)
                CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence))
                CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr))
                CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist))
                CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 55, 113)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 2)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
                CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                RETURN
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108869)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_EQUAL(LW(0), 2)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(0))
            ADD(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 2)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
                CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
                CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitOnCeiling)
                CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_ceil))
                CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, 5, -175)
                CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 46, -75)
                CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
                CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 9, -122, 30)
                CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 9, -122, 30)
                CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_ceil))
                CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_ceil))
                CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_ceil))
                CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 46, -75)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_J_1"))
                WAIT_FRAMES(48)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_J_2"))
                SET_VALUE(LW(5), 0)
                LOOP(4)
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    SWITCH(LW(5))
                        CASE_EQUAL(0)
                        ADD(LW(1), 50)
                        CASE_EQUAL(1)
                        ADD(LW(1), 75)
                        CASE_DEFAULT
                        ADD(LW(1), 100)
                    END_SWITCH
                    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_CUBIC_OUT, 0, -1)
                    ADD(LW(5), 1)
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    SUBTRACT(LW(1), 30)
                    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, 0, EASING_SINE_IN_OUT, 0, -1)
                END_LOOP
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                SET_VALUE(LW(1), 220)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_F_4"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
                WAIT_MS(333)
                CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_left_arm_entry_data), 0)
                CALL(btlevtcmd_StopWaitEvent, LW(3))
                CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(3))
                CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
                CALL(btlevtcmd_SetPos, LW(3), 320, 6, 53)
                CALL(btlevtcmd_SetHomePos, LW(3), 320, 6, 53)
                BEGIN_INLINE_EVENT_GET_ID(LW(14))
                    WAIT_FRAMES(35)
                    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
                    CALL(btlevtcmd_OffAttribute, LW(3), kUnitHidden)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
                    CALL(btlevtcmd_StartWaitEvent, LW(3))
                END_INLINE_EVENT
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_gesso_right_arm_entry_data), 0)
                CALL(btlevtcmd_StopWaitEvent, LW(3))
                CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(3))
                CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
                CALL(btlevtcmd_SetPos, LW(3), 59, 166, -18)
                CALL(btlevtcmd_SetHomePos, LW(3), 59, 166, -18)
                BEGIN_INLINE_EVENT_GET_ID(LW(15))
                    WAIT_FRAMES(5)
                    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_ARM_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F2_1"))
                    CALL(btlevtcmd_OffAttribute, LW(3), kUnitHidden)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
                    CALL(btlevtcmd_StartWaitEvent, LW(3))
                END_INLINE_EVENT
                CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                CALL(btlevtcmd_WaitEventEnd, LW(14))
                CALL(btlevtcmd_WaitEventEnd, LW(15))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(gesso_ikasumi_attack))
    RETURN
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_IKASUMI, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_A2_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 16, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftof, 55, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftof, 120, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ftof, 90, LW(7))
    CALL(_ikasumi_effect, UNIT_ID_SELF, LW(7))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_SUMI1"), EVT_NULLPTR, 0, LW(15))
        WAIT_FRAMES(LW(7))
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftof, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(10), 0)
    LABEL(50)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        SWITCH(LW(5))
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    ADD(LW(10), 1)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
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
    if (BattlePadMultiCheckTrigger(2, PAD_BUTTON_LEFT)) {
        event->userdata[0]++;
        if (event->userdata[0] >= 3) {
            event->userdata[0] = 0;
        }
    }
    if (BattlePadMultiCheckTrigger(2, PAD_BUTTON_RIGHT)) {
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
    BattlePadMultiCheckTrigger(2, PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);
    if (BattlePadMultiCheckNow(2, PAD_TRIGGER_L)) {
        target->mScale.x += 0.02f;
        target->mScale.y += 0.02f;
        target->mScale.z += 0.02f;
    }
    if (BattlePadMultiCheckNow(2, PAD_TRIGGER_R)) {
        target->mScale.x -= 0.02f;
        target->mScale.y -= 0.02f;
        target->mScale.z -= 0.02f;
    }
    target->mPosition.x += 0.1f * keyGetStickX(2);
    target->mPosition.y += 0.1f * keyGetSubStickY(2);
    target->mPosition.z -= 0.1f * keyGetStickY(2);
    BattlePadMultiCheckTrigger(2, PAD_BUTTON_A);
    return EVT_RETURN_BLOCK;
}
#endif

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(dead_subevent)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(3))
    CALL(btlevtcmd_GetUnitId, LW(3), LW(3))
    BEGIN_SIBLING_EVENT_GET_ID(LW(1))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_KillUnit, LW(3), 0)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(3))
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

