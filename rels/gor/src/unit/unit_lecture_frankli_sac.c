#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "statuswindow.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_msg.h"

#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_unit.h"
#include "battle/battle_event_default.h"
#include "battle/battle_sub.h"

u8 dark;
s32 darkCount;
f32 point[4];
#include "battle/battle_break_slot.h"

static USER_FUNC(_battle_set_lecture_unit_id);
USER_FUNC(_set_actend_event);
USER_FUNC(aud_attack_end);
USER_FUNC(aud_attack_init);
USER_FUNC(aud_dec);
USER_FUNC(aud_inc);
USER_FUNC(aud_joy);
USER_FUNC(blinkAP);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(get_ac_result);
USER_FUNC(get_actkind);
USER_FUNC(hammer_on);
static USER_FUNC(lecture_init);
void spot_square_disp(CameraId cameraId, void* param);
void Quads2D(f32 x0, f32 y0, f32 x1, f32 y1);

BattleUnitKind unit_lecture_frankli_sac;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
EvtWord _test_event[20];
EvtWord stop_wait_event[20];
static EvtWord init_event[61];
static EvtWord phase_event[105];
EvtWord actend_event[538];
static EvtWord damage_event[75];
static EvtWord wait_event[6];

BattleUnitKind unit_lecture_frankli_sac = {
    UNIT_TUTORIAL_FRANKLY_B3, // mUnitType
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
    "SFX_ENM_KURI_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoHpGauge, // attributeFlags
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
    {1, "btl_un_frankli", "c_kuri_h", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kPartNoTattle | kNoHpLoss, 0, pose_table},
};

EVT_BEGIN(_test_event)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_LECTURE, STRING("T_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_LECTURE, STRING("S_1"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_000"), 0, -2)
    CALL(_set_actend_event, UNIT_ID_MARIO, 0)
    RETURN
EVT_END()

USER_FUNC(_set_actend_event) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, args[0]);
    EvtWord* evt = (EvtWord*)evtGetValue(event, args[1]);
    BattleUnit* unit;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    unit->actEndEvent = evt;
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

static USER_FUNC(lecture_init) {
    BreakSlotWork* slot = BattleBreakSlotGetPtr();
    BattleAudience* aud = BattleAudienceBaseGetPtr();
    s32 seats[11] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 48};
    s32 seat;
    s32 i;

    slot->flags |= 1;
    aud->flags |= AUDIENCE_FLAG_EVENT_CTRL;
    for (i = 0; i < 200; i++) {
        BattleAudience_Delete(i);
    }
    for (i = 0; i < 11; i++) {
        if ((seat = seats[i]) != 48) {
            BattleAudience_Entry(seat, AUDIENCE_TOAD, 0);
        } else {
            BattleAudience_Entry(seat, AUDIENCE_HAMMER_BRO, 0);
            BattleAudienceGetPtr(seats[i])->flags |= 0x200;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(get_actkind) {
    evtSetValue(event, event->args[0], _battleWorkPointer->commandMenu.actType);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_ac_result) {
    evtSetValue(event, event->args[0], _battleWorkPointer->lastAcSuccess);
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_joy) {
    BattleAudienceJoySACLecture();
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_inc) {
    BattleAudienceAddAudienceNum(5);
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_dec) {
    BattleAudienceAddAudienceNum(-5);
    return EVT_RETURN_DONE;
}

USER_FUNC(hammer_on) {
    BattleAudience* aud = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member = BattleAudienceGetPtr(48);

    aud->flags &= ~AUDIENCE_FLAG_EVENT_CTRL;
    if (BattleAudienceItemOn(48, 0xEF, 1) == TRUE) {
        member->flags &= ~AUDIENCE_MEMBER_FLAG_NO_ESCAPE;
        aud->flags |= AUDIENCE_FLAG_EVENT_CTRL;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(aud_attack_init) {
    BattleWork* wp = _battleWorkPointer;

    wp->lectureFlags |= 1;
    wp->lectureWindowId = event->printWindowId;
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_attack_end) {
    _battleWorkPointer->lectureFlags |= 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(blinkAP) {
    statusAPBlink(160);
    return EVT_RETURN_DONE;
}

USER_FUNC(spot_square) {
    EvtWord* args = event->args;
    s32 time;
    s32 wait;

    point[0] = evtGetFloat(event, args[0]);
    point[1] = evtGetFloat(event, args[1]);
    point[2] = evtGetFloat(event, args[2]);
    point[3] = evtGetFloat(event, args[3]);
    time = evtGetValue(event, args[4]);
    wait = evtGetValue(event, args[5]);
    if (isFirstCall) {
        dark = 0;
        darkCount = 0;
    }
    darkCount++;
    if (darkCount >= 0 && darkCount <= time) {
        dark = intplGetValue(EASING_LINEAR, 0.0f, 192.0f, darkCount, time);
    }
    if (darkCount >= time + wait && darkCount <= wait + time * 2) {
        dark = intplGetValue(EASING_LINEAR, 192.0f, 0.0f, darkCount - (time + wait), time);
    }
    dispEntry(CAMERA_2D, 500.0f, 2, spot_square_disp, NULL);
    return darkCount >= wait + time * 2 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

void Quads2D(f32 x0, f32 y0, f32 x1, f32 y1) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y0, 0.0f);
    GXPosition3f32(x1, y0, 0.0f);
    GXPosition3f32(x1, y1, 0.0f);
    GXPosition3f32(x0, y1, 0.0f);
}

void spot_square_disp(CameraId cameraId, void* param) {
    CameraEntry* cam;
    Mtx mtx;

    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, dark});
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    cam = camGetPtr(CAMERA_2D);
    PSMTXIdentity(mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    Quads2D(-320.0f, 240.0f, 320.0f, point[1]);
    Quads2D(-320.0f, point[1], point[0], point[3]);
    Quads2D(point[2], point[1], 320.0f, point[3]);
    Quads2D(-320.0f, point[3], 320.0f, -240.0f);
}

EVT_BEGIN(stop_wait_event)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_MARIO, 1)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_PARTNER, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(lecture_init)
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_NO_OBJECT_FALL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(_battle_set_lecture_unit_id, LW(0))
    CALL(_set_actend_event, UNIT_ID_MARIO, PTR(actend_event))
    CALL(_set_actend_event, UNIT_ID_PARTNER, PTR(actend_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, 0)
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(stop_wait_event))
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_000"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            WAIT_FRAMES(60)
            CALL(blinkAP)
            CALL(spot_square, -105, 172, 206, 128, 20, 120)
            WAIT_FRAMES(30)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            WAIT_MS(500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 1, 11)
            WAIT_MS(500)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_001"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_LECTURE_SPECIAL)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
            GOTO(99)
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(actend_event)
    CALL_CHILD(PTR(stop_wait_event))
    CALL(get_actkind, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            WAIT_MS(1000)
            CALL_CHILD(PTR(stop_wait_event))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            WAIT_MS(500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 1, 11)
            WAIT_MS(500)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_003"), 0, -6)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            WAIT_FRAMES(60)
            CALL(blinkAP)
            CALL(spot_square, -105, 172, 206, 128, 20, 120)
            WAIT_FRAMES(30)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            WAIT_MS(500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 1, 11)
            WAIT_MS(500)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_004"), 0, -6)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 0, 40, 590, 0, 15, 60, 30, 11)
            WAIT_FRAMES(30)
            CALL(aud_joy)
            WAIT_FRAMES(60)
            CALL(aud_joy)
            WAIT_FRAMES(30)
            WAIT_MS(500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 1, 11)
            WAIT_MS(30)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_005"), 0, -6)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_onoff_battleflag, 0, BATTLE_FLAG_LECTURE_SPECIAL)
            CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_LECTURE_ATTACK)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, 0, 2)
            GOTO(99)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 9)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 0, LW(0))
        IF_INT_EQUAL(LW(0), 2)
            WAIT_MS(200)
            CALL_CHILD(PTR(stop_wait_event))
            CALL(get_ac_result, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 1, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    WAIT_MS(2000)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_006"), 0, -6)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    WAIT_FRAMES(60)
                    CALL(blinkAP)
                    CALL(spot_square, -105, 172, 206, 128, 20, 120)
                    WAIT_FRAMES(30)
                    CALL(btlevtcmd_AddUnitWork, UNIT_ID_LECTURE, 1, 1)
                    GOTO(99)
                END_IF
                IF_INT_EQUAL(LW(0), 1)
                    CALL(aud_inc)
                    WAIT_MS(2000)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_007"), 0, -6)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    WAIT_FRAMES(60)
                    CALL(spot_square, -295, -145, -175, -215, 20, 120)
                    WAIT_FRAMES(30)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(btlevtcmd_AddUnitWork, UNIT_ID_LECTURE, 1, 1)
                    GOTO(99)
                END_IF
                IF_INT_EQUAL(LW(0), 2)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_013"), 0, -6)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
                    CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 30, 11)
                    WAIT_FRAMES(30)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_014"), 0, -6)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    WAIT_MS(1600)
                    CALL(hammer_on)
                    WAIT_MS(1000)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_014_1"), 0, -6)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
                    CALL(evt_btl_camera_set_moveto, 0, -30, 29, 499, -30, -3, 5, 30, 11)
                    WAIT_FRAMES(120)
                    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_014_2"), 0, -6)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
                    WAIT_FRAMES(30)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_014_3"), 0, -6)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, 0, 3)
                    CALL(aud_attack_init)
                    CALL(btlevtcmd_onoff_battleflag, 0, BATTLE_FLAG_LECTURE_ATTACK)
                    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_LECTURE_NO_COMMAND)
                    GOTO(99)
                END_IF
                ELSE
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 2, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_008"), 0, -6)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    CALL(btlevtcmd_AddUnitWork, UNIT_ID_LECTURE, 2, 1)
                    GOTO(99)
                END_IF
                IF_INT_EQUAL(LW(0), 1)
                    CALL(aud_dec)
                    WAIT_MS(750)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_009"), 0, -6)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    CALL(btlevtcmd_AddUnitWork, UNIT_ID_LECTURE, 2, 1)
                    GOTO(99)
                END_IF
                IF_INT_EQUAL(LW(0), 2)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_010"), 0, -6)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    GOTO(99)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 0, LW(0))
    IF_INT_EQUAL(LW(0), 3)
        WAIT_MS(200)
        CALL(aud_attack_end)
        CALL_CHILD(PTR(stop_wait_event))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_014_4"), 0, -6)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 92, 94, 320, 92, 44, 13, 30, 11)
        WAIT_FRAMES(30)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_015"), 0, -6)
        WAIT_MS(1000)
        CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_INTERRUPT_STOP)
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_EQUAL(LW(0), 2)
            WAIT_MS(1000)
            CALL(get_ac_result, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_011"), 0, -2)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
                WAIT_FRAMES(60)
                CALL(spot_square, 198, 169, 294, 117, 20, 120)
                WAIT_FRAMES(30)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t3_012"), 0, -2)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
                WAIT_MS(500)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

