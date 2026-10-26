#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_msg.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "manager/evtmgr_cmd.h"

static USER_FUNC(_battle_set_lecture_unit_id);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
static USER_FUNC(lecture_init);

BattleUnitKind unit_lecture_frankli_kuriboo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[53];
static EvtWord entry_event[29];
static EvtWord phase_event[160];
static EvtWord damage_event[10];
static EvtWord wait_event[6];

static USER_FUNC(lecture_init) {
    return EVT_RETURN_DONE;
}

static USER_FUNC(_battle_set_lecture_unit_id) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 value;

    value = evtGetValue(event, *args++);
    wp->lectureUnitId = value;
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_lecture_frankli_kuriboo = {
    UNIT_TUTORIAL_FRANKLY_B4, // mUnitType
    "btl_un_frankli", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    1, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    28, // width
    24, // height
    {5, 25}, // hitOffset
    {0.0f, 14.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 30.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 15.6f, 0.0f}, // kissHitOffset
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
    "SFX_ENM_KURI_DAMAGED11", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoHpGauge | kUnitNoncombatant, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
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
    {POSE_TYPE_KO, "D_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_2"},
    {POSE_TYPE_DAMAGE, "D_2"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_frankli", "c_kuri_h", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kPartUntargetable | kPartNoTattle | kNoHpLoss, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(lecture_init)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(_battle_set_lecture_unit_id, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 0, -1000, 0)
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    WAIT_MS(900)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, -250, 0, -30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, 0, -30, 45, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_StopWaitEvent, UNIT_ID_MARIO)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_S_1"))
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_01"), 0, -2)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 71, 49, 213, 71, 35, 0, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_02"), 0, -2)
            CALL(evt_btl_camera_set_moveto, 0, 111, 85, 213, 111, 71, 0, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_03"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, -1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            WAIT_FRAMES(60)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_04"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
            WAIT_MS(1500)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, -1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, -60, 82, 340, -60, 60, 13, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_05"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            GOTO(99)
        END_IF
    END_IF
    LABEL(99)
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

