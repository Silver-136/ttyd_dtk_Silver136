#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "memory.h"
#include "system.h"
#include "string.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "battle/battle.h"
#include "driver/dispdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_sub.h"

double tan(double x);
typedef struct WanaWork { s32 ready; s32 poseId; Mtx mtx; GXTexObj obj; BattleUnitPart* part; smartEntry* tex; } WanaWork;
void test_kururing_mapdisp(CameraId camera);
static f32 tanf(f32 x);
void replace_callback(BattleUnitPart* part, s32 type);
void replace_disp(CameraId camera, void* param);

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(replace_evt);
void replace_callback(BattleUnitPart* part, s32 type);
void replace_disp(CameraId camera, void* param);
static f32 tanf(f32 x);

BattleUnitKind unit_wanawana;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[29];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[19];
static EvtWord wait_event[6];

static f32 tanf(f32 x) {
    return tan(x);
}

BattleUnitKind unit_wanawana = {
    199,
    "btl_un_wanawana",
    2,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    100,
    9,
    24,
    24,
    24,
    24,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
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

static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

static PoseTableEntry pose_table[] = {
    {28, "S_1"},
    {31, "S_1"},
    {39, "S_1"},
    {69, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_wanawana", "nakayama_test4", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

void replace_disp(CameraId camera, void* param) {
    WanaWork* wp = param;
    CameraEntry saved;
    AnimationPoseData* data;
    CameraEntry* cam;
    Vec min;
    Vec max;
    Vec pos;
    s32 w;
    s32 h;

    memcpy(&saved, camGetPtr(camera), sizeof(CameraEntry));
    data = animPoseGetAnimBaseDataPtr(wp->poseId);
    min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    w = 2.0f * (max.x - min.x);
    h = 2.0f * (max.z - min.z);
    cam = camGetPtr(camera);
    w += w & 1;
    h += h & 1;
    BtlUnit_GetPartsWorldPos(wp->part, &pos.x, &pos.y, &pos.z);
    cam->cameraUp = (Vec){0.0f, 0.0f, -1.0f};
    cam->cameraAt = pos;
    cam->cameraPos = pos;
    cam->cameraPos.y += (h / 2.0f) / tanf(0.21816616f);
    C_MTXPerspective(cam->projection, 25.0f, (f32)w / (f32)h, 1.0f, 32767.0f);
    cam->projType = GX_PERSPECTIVE;
    C_MTXLookAt(cam->view, &cam->cameraPos, &cam->cameraUp, &cam->cameraAt);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(0.0f, 0.0f, w, h, 0.0f, 1.0f);
    GXSetScissor(0, 0, w, h);
    test_kururing_mapdisp(camera);
    sysWaitDrawSync();
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w / 2, h / 2, GX_TF_RGB5A3, GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXCopyTex(wp->tex->address, GX_TRUE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    memcpy(camGetPtr(camera), &saved, sizeof(CameraEntry));
    wp->ready = 1;
}

void replace_callback(BattleUnitPart* part, s32 type) {
    WanaWork* wp = (WanaWork*)part->mOwner->countersetWeapon;

    if (wp->ready) {
        Mtx mtx;

        PSMTXIdentity(mtx);
        if (type == 0) {
            animSetPaperTexMtx(mtx, NULL, NULL);
            animSetPaperTexObj(&wp->obj, NULL, NULL, wp->tex, NULL, NULL, 1, 1);
        } else {
            animSetPaperTexMtx(NULL, NULL, NULL);
            animSetPaperTexObj(NULL, NULL, NULL, NULL, NULL, NULL, 1, 1);
        }
    }
}

USER_FUNC(replace_evt) {
    BattleUnitPart* part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, -2));
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(part->poseId);
    WanaWork* wp;
    s32 w;
    s32 h;

    {
        Vec min;
        Vec max;

        min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
        max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};

        w = 2.0f * (max.x - min.x);
        h = 2.0f * (max.z - min.z);
    }
    w += w & 1;
    h += h & 1;
    wp = BTL_ALLOC(sizeof(WanaWork));
    wp->tex = smartAlloc(GXGetTexBufferSize(w / 2, h / 2, GX_TF_RGB5A3, GX_FALSE, 0), 1);
    wp->poseId = part->poseId;
    wp->part = part;
    GXInitTexObj(&wp->obj, wp->tex->address, w / 2, h / 2, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&wp->obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    PSMTXIdentity(wp->mtx);
    animGetPtr()->poses[part->poseId].flags |= 0x10;
    part->dispCallback = replace_callback;
    unit->countersetWeapon = (struct BattleWeapon*)wp; // reused as the WanaWork pointer
    unit->screenTexBuf = wp->tex;
    wp->ready = 0;
    dispEntry(CAMERA_SHADOW, 0.0f, 0, replace_disp, wp);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka"), 4194304)
    CALL(replace_evt)
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
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

