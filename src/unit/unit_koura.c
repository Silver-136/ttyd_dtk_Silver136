#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_eff.h"
#include "battle/battle_sub.h"
#include "battle/unit_work.h"

USER_FUNC(btlevtcmd_SetEventPhase);

BattleUnitKind unit_koura;
static BattleDataEntry data_table[7];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
PoseTableEntry pose_table_crack_lv1[5];
PoseTableEntry pose_table_crack_lv2[5];
static BattleUnitKindPart parts[2];
static EvtWord init_event[46];
static EvtWord phase_event[38];
EvtWord koura_open[72];
EvtWord koura_close[79];
EvtWord damage_core[92];
EvtWord pose_tbl_reset[58];
static EvtWord dead_event[243];
EvtWord destroy_event[47];
static EvtWord damage_event[10];
static EvtWord wait_event[6];
static EvtWord attack_event[2];

BattleUnitKind unit_koura = {
    UNIT_SHELL_SHIELD, // mUnitType
    "btl_un_koura", // name
    8, // maxHp
    0, // maxFp
    5, // mDangerHP
    1, // mPerilHP
    1, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    24, // width
    24, // height
    {24, 24}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 0.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {25.0f, 0.0f, 0.0f}, // bintaHitOffset
    {25.0f, 15.6f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BTL_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoncombatant, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_KOURA_OPEN_EVENT, koura_open},
    {UNIT_DATA_KOURA_CLOSE_EVENT, koura_close},
    {UNIT_DATA_DAMAGE_SUB_EVENT, damage_core},
    {UNIT_DATA_POSE_TABLE_RESET_EVENT, pose_tbl_reset},
    {UNIT_DATA_DESTROY_EVENT, destroy_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    0, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "PKO_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PKO_S_1"},
    {POSE_TYPE_DAMAGE, "PKO_D_1"},
    {POSE_TYPE_SHELL_SHIELD_OPEN, "PKO_Z_1"},
    {POSE_TYPE_ACTION, "PKO_S_1"},
};

PoseTableEntry pose_table_crack_lv1[] = {
    {POSE_TYPE_NORMAL, "PKO_S_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PKO_S_2"},
    {POSE_TYPE_DAMAGE, "PKO_D_2"},
    {POSE_TYPE_SHELL_SHIELD_OPEN, "PKO_Z_2"},
    {POSE_TYPE_ACTION, "PKO_S_2"},
};

PoseTableEntry pose_table_crack_lv2[] = {
    {POSE_TYPE_NORMAL, "PKO_S_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "PKO_S_3"},
    {POSE_TYPE_DAMAGE, "PKO_D_3"},
    {POSE_TYPE_SHELL_SHIELD_OPEN, "PKO_Z_3"},
    {POSE_TYPE_ACTION, "PKO_S_3"},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_koura", "c_koura", 0.0f, 0.0f, 0.0f, 20.0f, 40.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kUntattleable, 0, pose_table},
    {2, "btl_un_koura", "c_koura", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.5f))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_OPEN, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_FOLLOW, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL_CHILD(PTR(koura_open))
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL_CHILD(PTR(koura_close))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(koura_open)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_OPEN, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, -23, 0, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_TARGET, LW(3))
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, kHidden)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_OPEN, 1)
    CALL(btlevtcmd_OffAttribute, LW(3), kUnitNeverTargetable)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_FOLLOW, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_SHELL_SHIELD_OPEN)
    RETURN
EVT_END()

EVT_BEGIN(koura_close)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_OPEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, -23, 0, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 60)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_OPEN, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_TARGET, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), kUnitNeverTargetable)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kHidden)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_FOLLOW, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(damage_core)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, LW(1))
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_crack_lv2))
        IF_INT_NOT_EQUAL(LW(1), 2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 2)
        CASE_LESS_OR_EQUAL(6)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_crack_lv1))
        IF_INT_NOT_EQUAL(LW(1), 1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 1)
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    WAIT_MS(500)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(pose_tbl_reset)
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_crack_lv2))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 2)
        CASE_LESS_OR_EQUAL(6)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_crack_lv1))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 1)
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_CRACK, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_TARGET, LW(3))
    CALL(btlevtcmd_OffAttribute, LW(3), (kShellShielded | kUnitNeverTargetable))
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, kHidden)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_GUARD4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKO_H_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("PKO_H_2"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 1, 23, 0, 0)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, -23, 0, 0)
    BEGIN_SIBLING_EVENT
        LOOP(5)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
        LOOP(6)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 2, 0, 1, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(2)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 2, 1, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(5)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsDispOffset, UNIT_ID_SELF, 2, 1, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddPartsRotateOffset, UNIT_ID_SELF, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsRotateOffset, UNIT_ID_SELF, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(14)
            CALL(btlevtcmd_AddPartsRotateOffset, UNIT_ID_SELF, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsRotateOffset, UNIT_ID_SELF, 2, 1, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LOOP(20)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 0, 2)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 2, 0, 0, -2)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(8)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

EVT_BEGIN(destroy_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHELL_SHIELD_WORK_TARGET, LW(3))
    CALL(btlevtcmd_OffAttribute, LW(3), (kShellShielded | kUnitNeverTargetable))
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

