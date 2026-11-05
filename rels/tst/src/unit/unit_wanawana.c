// Battle unit wanawana (a trap) (`wanawana`): tst.rel's copy of rels/tou2/src/unit/unit_wanawana.c.
#include "battle/battle_unit.h"
#include "tst_battle.h"
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
#include "battle/unit/unit_wanawana.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_map.h"
#include "driver/mapdrv.h"
#include "libc_decls.h"

void replace_callback(BattleUnitPart* part, s32 type);
void replace_disp(CameraId camera, void* param);

BattleUnitKind unit_wanawana;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[29];
static EvtWord damage_event[10];
static EvtWord attack_event[19];
static EvtWord wait_event[6];

/** tan in single precision, as in tou2's unit_wanawana.c. */
static inline f32 tanf(f32 x) {
    return tan(x);
}

/**
 * Unit definition of the trap unit `wanawana`: 2 HP and level 1, with its sizes, damage sounds,
 * status vulnerability, parts and init script.
 */
BattleUnitKind unit_wanawana = {
    UNIT_UNUSED_WANAWANA,
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

/** Defence of the trap unit `wanawana`: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the trap unit `wanawana`: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Status vulnerability of the trap unit `wanawana`: every status lands at the normal rate. */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Pose table of the trap unit `wanawana`: the normal, guarded-hit, damage and action poses all play S_1.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "S_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Data table of the trap unit `wanawana`: it has no special reaction scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Body part of the trap unit `wanawana`: a single part using the nakayama_test4 model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_wanawana", "nakayama_test4", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/**
 * Draw callback on the shadow camera: renders the stage floor from above, framed to the trap's model bounds,
 * and copies it into the trap's texture so the trap looks like the ground under it; sets ready when done.
 */
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

/**
 * Animation callback of the trap's body part: once the floor texture is ready,
 * binds it as the model's paper texture (type 0) or clears the binding (other types).
 */
void replace_callback(BattleUnitPart* part, s32 type) {
    WanaWork* wp = (WanaWork*)TST_BATTLE_UNIT(part->owner)->countersetWeapon;

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

/**
 * Script helper: allocates the trap's floor texture buffer (sized from its model bounds),
 * registers the texture overlay on its body pose and queues replace_disp to fill it.
 */
USER_FUNC(replace_evt) {
    BattleUnitPart* part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, -2));
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(part->poseId);
    WanaWork* wp;
    s32 w;
    s32 h;
    Vec min;
    Vec max;


    min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};

    w = 2.0f * (max.x - min.x);
    h = 2.0f * (max.z - min.z);
    w += w & 1;
    h += h & 1;
    wp = BTL_ALLOC(sizeof(WanaWork));
    wp->tex = smartAlloc(GXGetTexBufferSize(w / 2, h / 2, GX_TF_RGB5A3, GX_FALSE, 0), SMART_GROUP_MAP);
    wp->poseId = part->poseId;
    wp->part = part;
    GXInitTexObj(&wp->obj, wp->tex->address, w / 2, h / 2, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&wp->obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    PSMTXIdentity(wp->mtx);
    animGetPtr()->poses[part->poseId].flags |= ANIM_POSE_FLAG_CHANGE_TEXTURE;
    part->dispCallback = replace_callback;
    TST_BATTLE_UNIT(unit)->countersetWeapon = (struct BattleWeapon*)wp; // reused as the WanaWork pointer
    TST_BATTLE_UNIT(unit)->screenTexBuf = wp->tex;
    wp->ready = 0;
    dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_OPAQUE, replace_disp, wp);
    return EVT_RETURN_DONE;
}

/**
 * Init event of the trap unit: registers its wait, attack and damage events (default confusion),
 * sets MAP_OBJ_FLAG_KURURING on the S_yuka floor object, has replace_evt dress the trap in the floor's texture,
 * and starts waiting.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka"), MAP_OBJ_FLAG_KURURING)
    CALL(replace_evt)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of the trap: runs the default damage reaction (btldefaultevt_Damage) for its body part. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Empty phase event; nothing in this file registers it. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Attack event of the trap: it has no attack of its own; it only uses an item if it decides to, then waits again. */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait event of the trap: plays the idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
