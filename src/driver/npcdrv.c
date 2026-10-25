#include "driver/npcdrv.h"
#include "data/npc_data.h"
#include "libc_decls.h"
#include "pmario_sound.h"
#include <math.h>
#include <dolphin/pad.h>
#include "dolphin/gx/GXStruct.h"
#include "driver/animdrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "mario/mario_party.h"
#include "party.h"
#include "driver/swdrv.h"
#include "data/mapdata.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/itemdrv.h"
#include "eff/eff_majinai.h"
#include "evt/evt_cmd.h"
#include "evt/evt_snd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "memory.h"
#include "addr_word.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "driver/offscreendrv.h"
#include "driver/seqdrv.h"
#include "battle/battle_information.h"
#include <string.h>
#include "kpa_act.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_msg.h"
#include "koopa.h"
#include "koopa_hit.h"
#include "mario/mario_hit.h"
#include "poly_trig.h"
#include "eff/eff_small_star.h"

f32 cloudGetBreathPower(Vec pos, f32 width);
void kpaPowDown(void);

void FontDrawStringShake(f32 x, f32 y, const char* message);
ItemEntry* itemEntry(const char* name, s32 itemId, s32 type, s32 collectFlag, s32 evtCode, f32 x, f32 y, f32 z);

#define NPC_RAD2DEG(r) ((r) * 57.29579f)
USER_FUNC(evt_mario_get_pos);

typedef struct NpcWork2 {
    NpcWork field;  // 0x0
    NpcWork battle; // 0x14
} NpcWork2;

//.data
char* luigi_pose[14][2] = { { "Z_1", "Z_1R" }, { "S_1", "S_1R" },   { "W_1", "W_1R" },   { "R_1", "R_1R" },
                            { "T_1", "T_1R" }, { "J_1A", "J_1RR" }, { "J_1B", "J_1BR" }, { "Z_2", "Z_2R" },
                            { "S_2", "S_2R" }, { "T_2", "T_2R" },   { "L_1", "L_1R" },   { "L_2", "L_2R" },
                            { "L_3", "L_3R" }, { "I_1", "I_1R" } };

//.sdata
char MarioStayPose[32] = "KPA_S_1";
char MarioTalkPose[32] = "KPA_T_1";

//.sbss
BOOL dbg_battleskip = 0;
u32 npcMainCount = 0;
NpcFieldEntry* release_wp = NULL;
static NpcWork2 work;

// local prototypes
void mtx_setup(NpcEntry* entry, Mtx mtx, s32 history);
void npcDisp_xlu(CameraId cameraId, void* param);
void npcDisp(CameraId camId, void* param);
void npcDisp_offscreen_xlu(CameraId camId, void* param);
void npcDisp_offscreen(CameraId camId, void* param);
void npcDisp_blur(CameraId camId, void* param);
void _npcDeleteGroup(NpcEntry* entry);

#define npcGetWorkPtr2(inBattle) ((inBattle) ? &work.battle : &work.field)
// TODO: inlining?
#define npcGetWorkPtrInline() (gp->inBattle ? &work.battle : &work.field)

// TODO: inlining?
NpcWork* npcGetWorkPtr(void) {
    return gp->inBattle ? &work.battle : &work.field;
}

void npcReleaseFieldNpc(void) {
    NpcWork* wp;
    NpcFieldEntry* field;
    NpcEntry* entry;
    AnimationPose* pose;
    int i;

    wp = &work.field;
    field = release_wp;
    if (wp->flags & 2) {
        entry = wp->entries;
        for (i = 0; i < wp->total; i++, entry++) {
            if (entry->flags & 1 && entry->flags & 2 && animPoseGetAnimPosePtr(entry->poseId)->paperId == -1) {
                if (entry->poseId >= 0) {
                    pose = animPoseGetAnimPosePtr(entry->poseId);
                    field->mFlags = pose->flags;
                    field->peraRotY = pose->peraRotY;
                    field->rotationY = pose->rotationY;
                    field->scaleZFlip = pose->scaleZFlip;
                    field->peraTargetRotY = pose->peraTargetRotY;
                    strcpy(field->mModelName, animPoseGetAnimBaseDataPtr(entry->poseId)->fileName);
                    strcpy(field->mAnimName, animPoseGetCurrentAnim(entry->poseId)->name);
                    animPoseRelease(entry->poseId);
                    field++; // next release_wp entry
                    entry->poseId = -1;
                }
            }
        }
    }
}

void npcRecoveryFieldNpc(void) {
    NpcWork* wp;
    NpcFieldEntry* field;
    NpcEntry* entry;
    AnimationPose* pose;
    int i;

    wp = &work.field;
    field = release_wp;
    if (wp->flags & 2) {
        entry = wp->entries;
        for (i = 0; i < wp->total; i++, entry++) {
            if (entry->flags & 1 && entry->flags & 2 && entry->poseId == -1) {
                entry->poseId = animPoseEntry(field->mModelName, 0);
                animPoseSetAnim(entry->poseId, field->mAnimName, TRUE);
                pose = animPoseGetAnimPosePtr(entry->poseId);
                pose->flags = field->mFlags;
                pose->peraRotY = field->peraRotY;
                pose->rotationY = field->rotationY;
                pose->scaleZFlip = field->scaleZFlip;
                pose->peraTargetRotY = field->peraTargetRotY;

                if (pose->flags & 0x40) {
                    pose->flags &= ~0x40;
                    animPoseWorldPositionEvalOn(entry->poseId);
                }
                if (pose->flags & 0x80) {
                    pose->flags &= ~0x80;
                    animPoseWorldMatrixEvalOn(entry->poseId);
                }
                animPoseMain(entry->poseId);
                field++; // next release_wp entry
            }
        }
        wp->flags &= ~2;
    }
}

void mtx_setup(NpcEntry* entry, Mtx mtx, s32 history) {
    f32 rotationAngle;
    f32 scaleSign;
    CameraEntry* camera;
    f32 tmpi2[2];
    f32 v15;
    Mtx mtxX, mtxY, mtxZ;

    rotationAngle = 0.0f;
    scaleSign = 1.0f;
    if (!(entry->flags & 0x2000000) && !(entry->flags & 0x8000000)) {
        camera = camGetCurPtr();
        tmpi2[1] = angleABf(camera->cameraPos.x, camera->cameraPos.z, camera->cameraAt.x, camera->cameraAt.z);
        tmpi2[0] = angleABf(camera->cameraPos.x, camera->cameraPos.z, entry->position.x, entry->position.z);
        rotationAngle = reviseAngle(tmpi2[1] - tmpi2[0]);
    }
    v15 = reviseAngle(reviseAngle(entry->rotation.y));
    if (v15 >= 90.0f && v15 <= 270.0f) {
        scaleSign *= -1.0f;
    }
    MTXIdentity(mtx);
    if (entry->scale.x != 1.0f || entry->scale.y != 1.0f || entry->scale.z != 1.0f || scaleSign != 1.0f) {
        MTXScaleApply(mtx, mtx, entry->scale.x, entry->scale.y, entry->scale.z * scaleSign);
    }
    if (entry->rotationOffset.x || entry->rotationOffset.y || entry->rotationOffset.z) {
        MTXTransApply(mtx, mtx, -entry->rotationOffset.x, -entry->rotationOffset.y, -entry->rotationOffset.z);
    }
    if (entry->rotation.z) {
        MTXRotRad(mtxZ, 'z', entry->rotation.z * 0.017453292f);
        MTXConcat(mtxZ, mtx, mtx);
    }
    if (entry->rotation.x) {
        MTXRotRad(mtxX, 'x', entry->rotation.x * 0.017453292f);
        MTXConcat(mtxX, mtx, mtx);
    }
    if (entry->rotation.y || rotationAngle) {
        MTXRotRad(mtxY, 'y', (entry->rotation.y + rotationAngle) * 0.017453292f);
        MTXConcat(mtxY, mtx, mtx);
    }
    if (entry->rotationOffset.x || entry->rotationOffset.y || entry->rotationOffset.z) {
        MTXTransApply(mtx, mtx, entry->rotationOffset.x, entry->rotationOffset.y, entry->rotationOffset.z);
    }
    if (history == -1) {
        MTXTransApply(mtx, mtx, entry->position.x, entry->position.y, entry->position.z);
    } else {
        MTXTransApply(mtx, mtx, entry->positionHistory[history].x, entry->positionHistory[history].y,
                      entry->positionHistory[history].z);
    }
}

static inline void npcLuigiPoseFlip(NpcEntry* entry) {
    if (entry->flags & 0x800000) {
        AnimationPose* animPose;
        AnimTableEntry* anim;
        CameraEntry* camera;
        f32 dx, dz;
        f32 angle;

        animPoseGetCurrentAnim(entry->poseId);
        animPose = animPoseGetAnimPosePtr(entry->poseId);
        anim = animPoseGetCurrentAnim(entry->poseId);

        camera = camGetPtr(CAMERA_3D);
        dx = camera->cameraAt.x - camera->cameraPos.x;
        dz = camera->cameraAt.z - camera->cameraPos.z;
        angle = reviseAngle(NPC_RAD2DEG((f32)atan2(-dx, -dz)));
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        angle = angle - animPose->rotationY;
        angle = reviseAngle(angle);

        if ((angle > 90.0f && angle <= 270.0f) || (angle < -90.0f && angle >= -270.0f)) {
            u32 i;
            for (i = 0; i < 14; i++) {
                if (!strcmp(luigi_pose[i][0], anim->name)) {
                    break;
                }
            }
            if (i < 14) {
                animPoseSetAnim(entry->poseId, luigi_pose[i][1], FALSE);
            }
        } else {
            u32 i;
            for (i = 0; i < 14; i++) {
                if (!strcmp(luigi_pose[i][1], anim->name)) {
                    break;
                }
            }
            if (i < 14) {
                animPoseSetAnim(entry->poseId, luigi_pose[i][0], FALSE);
            }
        }
    }
}

static inline void npcDimColor(NpcEntry* entry) {
    GXColor color = entry->color;
    if (entry->jumpFlags & 0x400000) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
        animPoseSetMaterialEvtColor(entry->poseId, color);
        animPoseSetMaterialFlagOn(entry->poseId, 0x40);
    }
}

void npcDisp_xlu(CameraId cameraId, void* param) {
    NpcEntry* entry = param; // cast to proper type
    Mtx mtx;
    GXColor color;
    f32 rotY;

    rotY = reviseAngle(-entry->direction);
    mtx_setup(entry, mtx, -1);

    color = entry->color;
    if (entry->jumpFlags & 0x400000) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
        animPoseSetMaterialEvtColor(entry->poseId, color);
        animPoseSetMaterialFlagOn(entry->poseId, 0x40);
    }

    npcLuigiPoseFlip(entry);
    animPoseMain(entry->poseId);
    animPoseDrawMtx(entry->poseId, mtx, 1, rotY, 2.0f);
    animPoseDrawMtx(entry->poseId, mtx, 2, rotY, 2.0f);
    animPoseDrawMtx(entry->poseId, mtx, 3, rotY, 2.0f);
}

void npcDisp(CameraId camId, void* param) {
    NpcEntry* entry = param; // cast to proper type
    Mtx mtx;
    GXColor color;
    f32 rotY;
    AnimTableEntry* anim;
    const char* nm;

    rotY = reviseAngle(-entry->direction);
    mtx_setup(entry, mtx, -1);

#ifdef VERSION_G8ME01
    // US: the dimmed colour is applied for every render mode
    color = entry->color;
    if (entry->jumpFlags & 0x400000) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
        animPoseSetMaterialEvtColor(entry->poseId, color);
        animPoseSetMaterialFlagOn(entry->poseId, 0x40);
    }
    switch (dispGetCurWork()->renderMode) {
        case 0:
#else
    switch (dispGetCurWork()->renderMode) {
        case 0:
            color = entry->color;
            if (entry->jumpFlags & 0x400000) {
                color.r /= 2;
                color.g /= 2;
                if (rotY && rotY && rotY) {
                }
                color.b /= 2;
                animPoseSetMaterialEvtColor(entry->poseId, color);
                animPoseSetMaterialFlagOn(entry->poseId, 0x40);
            }
#endif

            npcLuigiPoseFlip(entry);
            animPoseMain(entry->poseId);
            animPoseDrawMtx(entry->poseId, mtx, 1, rotY, 2.0f);
            break;
        case 1:
            animPoseDrawMtx(entry->poseId, mtx, 2, rotY, 2.0f);
            break;
        case 2:
            animPoseDrawMtx(entry->poseId, mtx, 3, rotY, 2.0f);
            break;
    }
}

void npcDisp_offscreen_xlu(CameraId camId, void* param) {
    NpcEntry* entry = param; // cast to proper type
    u16 left, top, right, bottom;
    Mtx mtx;
    GXColor color;
    f32 rotY;

    sysWaitDrawSync();
    GXClearBoundingBox();
    rotY = reviseAngle(-entry->direction);
    mtx_setup(entry, mtx, -1);

    npcDimColor(entry);

    npcLuigiPoseFlip(entry);
    if (!bottom) {
    }
    animPoseMain(entry->poseId);
    animPoseDrawMtx(entry->poseId, mtx, 1, rotY, 2.0f);
    animPoseDrawMtx(entry->poseId, mtx, 2, rotY, 2.0f);
    animPoseDrawMtx(entry->poseId, mtx, 3, rotY, 2.0f);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
}

void npcDisp_offscreen(CameraId camId, void* param) {
    NpcEntry* entry = param; // cast to proper type
    u16 left, top, right, bottom;
    Mtx mtx;
    GXColor color;
    f32 rotY;

    sysWaitDrawSync();
    GXClearBoundingBox();
    rotY = reviseAngle(-entry->direction);
    mtx_setup(entry, mtx, -1);

#ifdef VERSION_G8ME01
    npcDimColor(entry);
    switch (dispGetCurWork()->renderMode) {
        case 0:
#else
    switch (dispGetCurWork()->renderMode) {
        case 0:
            npcDimColor(entry);
#endif

            npcLuigiPoseFlip(entry);
            animPoseMain(entry->poseId);
            animPoseDrawMtx(entry->poseId, mtx, 1, rotY, 2.0f);
            break;
        case 1:
            animPoseDrawMtx(entry->poseId, mtx, 2, rotY, 2.0f);
            break;
        case 2:
            animPoseDrawMtx(entry->poseId, mtx, 3, rotY, 2.0f);
            break;
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
}

#pragma push
#pragma optimization_level 2

void npcDisp_blur(CameraId camId, void* param) {
    s32 alpha;
    NpcEntry* entry = param;
    s32 i;
    s32 flag;
    f32 rotY;
    f32 x;
    f32 z;
    Mtx mtx;

    rotY = reviseAngle(-entry->direction);
    flag = animPoseGetMaterialFlag(entry->poseId);
    x = 2.0f * polySinf((6.2831855f * camGetPtr(camId)->viewYaw) / 360.0f);
    z = 2.0f * -polyCosf((6.2831855f * camGetPtr(camId)->viewYaw) / 360.0f);
    animPoseMain(entry->poseId);
    i = 4;
    alpha = 100;
    do {
        mtx_setup(entry, mtx, i);
        MTXTransApply(mtx, mtx, x, 0.0f, z);
        animPoseSetMaterialFlagOn(entry->poseId, 0x40);
        animPoseSetMaterialEvtColor(entry->poseId, (GXColor){0xFF, 0xFF, 0xFF, (u8)(0x80 - alpha)});
        animPoseDrawMtx(entry->poseId, mtx, 1, rotY, 2.0f);
        animPoseDrawMtx(entry->poseId, mtx, 2, rotY, 2.0f);
        animPoseDrawMtx(entry->poseId, mtx, 3, rotY, 2.0f);
        i--;
        alpha -= 25;
    } while (i > 0);
    animPoseSetMaterialFlagOff(entry->poseId, 0x40);
    animPoseSetMaterialFlagOn(entry->poseId, flag);
}
#pragma pop

void npcInit(void) {
    FieldBattleData* data;
    NpcWork* wp;

    wp = &work.field;
    wp->total = NPC_FIELD_MAX_COUNT;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * wp->total);
    memset(wp->entries, 0, sizeof(NpcEntry) * wp->total);

    release_wp = __memAlloc(HEAP_DEFAULT, sizeof(NpcFieldEntry) * wp->total);

    wp = &work.battle;
    wp->total = NPC_BATTLE_MAX_COUNT;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * wp->total);
    memset(wp->entries, 0, sizeof(NpcEntry) * wp->total);

    data = __memAlloc(HEAP_DEFAULT, sizeof(FieldBattleData));
    gp->fbatData = data;
    memset(gp->fbatData, 0, sizeof(FieldBattleData));
    data->mode = 0;
    npcMainCount = 0;
}

void npcReset(BOOL inBattle) {
    NpcWork* wp = npcGetWorkPtr2(inBattle);
    memset(wp->entries, 0, sizeof(NpcEntry) * wp->total);
    wp->count = 0;
    if (!inBattle) {
        gp->fbatData->mode = 0;
    }
    npcMainCount = 0;
}

s32 npcGetReactionOfLivingBody(BOOL inBattle) {
    return npcGetWorkPtr2(inBattle)->count;
}

// TODO: fix entry->position delayed set-Vec with r6/r30
// TODO: a1 is desc/ription?
s32 npcEntry(const char* a1, const char* animName) {
    NpcWork* wp;
    s32 i;
    NpcEntry* entry;
    s32 total;

    wp = gp->inBattle ? &work.battle : &work.field;
    total = wp->total;
    for (i = 0, entry = wp->entries; i < total; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->description, a1)) {
            break;
        }
    }

    entry = wp->entries;
    for (i = 0; i < total; i++, entry++) {
        if (!(entry->flags & 1)) {
            break;
        }
    }
    memset(entry, 0, sizeof(NpcEntry));
    entry->flags = 3;
    strcpy(entry->description, a1);
    entry->poseId = animPoseEntry(animName, gp->inBattle != 0);
    if (entry->poseId == -2) {
        entry->poseId = animPoseEntry("hoshi", gp->inBattle != 0);
    }
    animPosePeraOn(entry->poseId);
    entry->position = (Vec){0.0f, 0.0f, 0.0f};
    entry->prevPosition = entry->position;
    entry->width = animPoseGetHeight(entry->poseId);
    entry->height = animPoseGetRadius(entry->poseId);
    entry->scale = (Vec){1.0f, 1.0f, 1.0f};
    entry->shadowScale = 1.0f;
    entry->moveAnimState = 0;
    entry->checkPose = "M_I_2";
    entry->lastUpdateTicks = gp->renderTime;
    entry->moveTicks = 0;
    entry->jumpGravity = 1.0f;
    entry->maxFallSpeed = 0.0f;
    entry->jumpYScale = 1.0f;
    entry->offscreenId = -1;
    entry->color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    entry->cameraId = CAMERA_3D;
    entry->attackMode = 0;
    entry->balloonType = 1;
    if (animPoseGetVivianType(entry->poseId)) {
        entry->flags |= 0x4000000;
    }
    if (!strcmp(animName, "c_luigi")) {
        entry->flags |= 0x800000u;
    }
    animPoseSetMaterialLightFlagOn(entry->poseId, 2);
    wp->count++;
    return i;
}

#ifndef VERSION_G8ME01
static const Vec sZeroVec3 = {0.0f, 0.0f, 0.0f};
#endif

NpcTribeInfo* npcGetTribe(const char* tribeName) {
    NpcTribeInfo* tribe;

    for (tribe = npcTribe; tribe->nameJp; tribe++) {
        if (!strcmp(tribe->nameJp, tribeName)) {
            return tribe;
        }
    }
    return NULL;
}

void npcDelete(NpcEntry* entry) {
    int i;
    NpcEntry* slave;

    for (i = 0; i < 4; i++) {
        slave = entry->slaves[i];
        if (slave) {
            slave->master = NULL;
        }
        entry->slaves[i] = NULL;
    }

    if (entry->prev) {
        if (entry->next) {
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
        } else {
            entry->prev->next = NULL;
        }
    } else {
        if (entry->next) {
            entry->next->prev = NULL;
        }
    }
    entry->flags &= ~2;
    if (entry->poseId >= 0) {
        animPoseRelease(entry->poseId);
    }
    entry->poseId = -1;
    entry->flags &= ~1;
    npcGetWorkPtrInline()->count--;
}

void _npcDeleteGroup(NpcEntry* entry) { // recursively inlined, warning
    int i;

    if (entry->next) {
        _npcDeleteGroup(entry->next);
    }

    for (i = 0; i < 4; i++) {
        if (entry->slaves[i]) {
            npcDelete(entry->slaves[i]);
            entry->slaves[i] = NULL;
        }
    }
    npcDelete(entry);
}

void npcDeleteGroup(NpcEntry* entry) {
    if (entry->master) {
        entry = entry->master;
    }
    while (entry->prev) {
        entry = entry->prev;
    }
    _npcDeleteGroup(entry);
}

void _fbatFirstAttackAnnouceDisp(CameraId camId, void* param);
void fbatEncountCheck(void);
void fbatTalkMode(void);
void fbatBattleMode(void);
s32 kpaJumpChk(void);
s32 kpaFireAttackCheck(void);

void npcMoveG(NpcEntry* npc);
BOOL npcCheckHitMarioSide(NpcEntry* npc);
HitEntry* npcHitCheckSide(NpcEntry* npc, f32* outX, f32* outZ, f32* angle, f32 dx, f32 dz);
s32 vivianGetStatus(void);
#define NPC_CHKMARIO(ok)                                                              \
    do {                                                                              \
        m = marioGetPtr();                                                            \
        if (m->flags & (MARIO_FLAG_BG_MODE | MARIO_FLAG_IS_8_BIT)) { ok = 0; break; } \
        if (m->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) { ok = 0; break; } \
        if (gp->mapEventId != 0) { ok = 0; break; }                                   \
        if (seqGetSeq() != 2) { ok = 0; break; }                                      \
        if (m->currMotionId == MARIO_MOTION_SHIP) { ok = 0; break; }                  \
        if (vivianGetStatus() != 0 && vivianGetStatus() != 1) { ok = 0; } else { ok = 1; } } while (0)
void shadowSetCollision(s32 shadow, s32 val);
s32 shadowEntry(f32 x, f32 y, f32 z, f32 size);
void animPoseVivianMain(s32 poseId, Vec* pos);
f32 sinfd(f32 degrees);
void npcKoopaModeMain(void);
BOOL npcCheckBlow(NpcEntry* npc);
#ifdef VERSION_G8ME01
s32 kpaGetLevel(void);

// US: probe the wall from a copy of the position first, then slide along it
static inline void npcMoveSide(NpcEntry* entry, f32 mx, f32 mz) {
    f32 wx = entry->position.x;
    f32 wz = entry->position.z;
    entry->wallHit = npcHitCheckSide(entry, &wx, &wz, &entry->moveDir, mx, mz);
    if (entry->wallHit) {
        npcHitCheckSide(entry, &entry->position.x, &entry->position.z, &entry->moveDir, wx - entry->position.x,
                        wz - entry->position.z);
    } else {
        entry->position.x = wx;
        entry->position.z = wz;
    }
}
#endif
void npcMain(void) {
    const char* kpaSfx;
    NpcWork* npcwork;
    FieldBattleData* fbat;
    NpcEntry* entry;
    int i;
    int total;
    f32 dir;
    f32 prio;
    f32 dist; Vec dv; Vec dv2;
    s32 fire, jump;

    npcwork = gp->inBattle ? &work.battle : &work.field;
    marioGetPtr()->targetNpc = NULL;
    kpaSfx = "SFX_KUPPA_3RD_WATER_FALL1";

    npcMainCount++;

    total = npcwork->total;
    for (i = 0, entry = npcwork->entries; i < total; i++, entry++) {
        if (entry->flags & 1 && entry->flags & 2) {
            if (entry->flags & 0x8000) {
                animPoseGetAnimPosePtr(entry->poseId)->flags |= 0x100;
            } else {
                animPoseGetAnimPosePtr(entry->poseId)->flags &= ~0x100;
            }

            if (!(entry->flags & 0x100000)) {
                entry->jumpFlags |= 0x10;
            }
            if (entry->jumpFlags & 0x10000) {
                entry->jumpFlags &= ~0x10;
            }

            if (entry->wFbatHitCheckRelated) {
                entry->wFbatHitCheckRelated = entry->wFbatHitCheckRelated - 1;
            }

            if (entry->jumpFlags & 0x10) {
                entry->dtTicks = gp->renderTime - entry->lastUpdateTicks;
                if (OSTicksToMilliseconds(entry->dtTicks) > 500) {
                    entry->dtTicks = OSMillisecondsToTicks(16);
                }
                if (entry->flags & 0x20000) {
                    entry->jumpTicks += entry->dtTicks;
                } else {
                    entry->jumpTicks = 0;
                }
                entry->moveTicks += entry->dtTicks;
                entry->lastUpdateTicks = gp->renderTime;
                entry->moveElapsed = (f32)(OSTicksToMicroseconds(entry->moveTicks)) / 1000000.0f;
                entry->jumpElapsed = (f32)(OSTicksToMicroseconds(entry->jumpTicks)) / 1000000.0f;
                entry->moveDt = (f32)(OSTicksToMicroseconds(entry->dtTicks)) / 1000000.0f;
                entry->lastUpdateTime = (f32)(OSTicksToMicroseconds(entry->lastUpdateTicks)) / 1000000.0f;

            } else {
                entry->lastUpdateTicks = gp->renderTime;
            }

            if (entry->flags & 2) {
                if (entry->jumpFlags & 0x100000) {
                    entry->jumpFlags |= 0x200000;
                } else {
                    entry->jumpFlags &= ~0x200000;
                }
                entry->jumpFlags &= ~0x100000;
                {
                    f32 f30v;
                    if (!strcmp(gp->currentMapName, "kpa_03") && !(entry->position.y > 365.0f)) {
                        entry->jumpFlags |= 0x100000;
                        if (!(entry->jumpFlags & 0x200000) && (entry->jumpFlags & 0x100000)) {
                            psndSFXOn_3D(kpaSfx, &entry->position);
                        }
                    }
                    if (entry->jumpFlags & 0x100000 && !(entry->jumpFlags & 0x10000000)) {
                        entry->maxFallSpeed = 100.0f;
                        f30v = 0.5f;
                    } else {
                        f30v = 1.0f;
                    }
                    if (entry->jumpFlags & 0x100000) {
                        entry->hazardFlags |= 1;
                    }
                    if (entry->jumpFlags & 0x10) {
                        if (entry->moveDt > 0.0f) {
                            f32 mx = entry->moveOffset.x;
                            f32 mz = entry->moveOffset.z;
                            if (entry->jumpFlags & 0x100000 && !(entry->jumpFlags & 0x10000000)) {
                                mx *= 0.5f;
                                mz *= 0.5f;
                            }
                            if (entry->flags & 0x8000000) {
                                mz = 0.0f;
                            }
                            if (entry->flags & 0x40000000) {
                                entry->position.x += mx;
                                entry->position.z += mz;
                            } else {
#ifdef VERSION_G8ME01
                                npcMoveSide(entry, mx, mz);
#else
                                entry->wallHit = npcHitCheckSide(entry, &entry->position.x, &entry->position.z,
                                                                        &entry->moveDir, mx, mz);
#endif
                            }
                            entry->moveOffset.x = 0.0f;
                            entry->moveOffset.z = 0.0f;
                            npcMoveG(entry);
                        }
                        if (!(entry->flags & 8) && !(entry->flags & 0x20) && !(entry->flags & 0x4000)) {
                            npcCheckHitMarioSide(entry);
                        }
                        {

                            PSVECSubtract(&entry->position, &entry->prevPosition, &dv);
                            if (entry->flags & 0x400 && entry->moveDt > 0.0f) {
                                f32 spd = sqrtf(dv.x * dv.x + dv.z * dv.z) / entry->moveDt;
                                if (spd < 0.1f) {
                                    if (entry->moveAnimState != 1) {
                                        animPoseSetAnim(entry->poseId, entry->tribe->stayAnim, TRUE);
                                        entry->moveAnimState = 1;
                                    }
                                } else if (spd < entry->tribe->runStartSpeed * f30v) {
                                    if (entry->moveAnimState != 2) {
                                        animPoseSetAnim(entry->poseId, entry->tribe->walkAnimation, TRUE);
                                        entry->moveAnimState = 2;
                                    }
                                } else {
                                    if (entry->moveAnimState != 3) {
                                        animPoseSetAnim(entry->poseId, entry->tribe->runAnimation, TRUE);
                                        entry->moveAnimState = 3;
                                    }
                                }
                            }
                        }
                    }
                }
                if (entry->regularEvtId) {
                    EventEntry* evt = evtGetPtrID(entry->regularEvtId);
                    if (evt && !(evt->flags & EVENT_STOPPED)) {
                        s32 st = evt->wNpcEventType;
                        if (entry->blowEvent && st != 5 && st != 8) {
                            int trig;
                            trig = npcCheckBlow(entry);
                            if (trig) {
                                entry->interruptFlags |= 8;
                                goto interrupt;
                            }
                        }
                        if (entry->territoryType != NPC_TERRITORY_TYPE_NOTHING && (st == 1 || st == 2)) {
                            int trig;
                            trig = 0;
                            if (entry->territoryType == NPC_TERRITORY_TYPE_CIRCLE) {
                                f32 tz = entry->territoryBase.z - entry->position.z;
                                f32 tx = entry->territoryBase.x - entry->position.x;
                                f32 d = sqrtf(tx * tx + tz * tz);
                                if (d > (st == 2 ? entry->territoryHoming.x : entry->territoryHoming.x)) {
                                    trig = 1;
                                }
                            } else if (entry->territoryType == NPC_TERRITORY_TYPE_SQUARE) {
                                if (st == 2) {
                                    if (entry->position.x < entry->territoryBase.x - entry->territoryHoming.x ||
                                        entry->position.x > entry->territoryBase.x + entry->territoryHoming.x ||
                                        entry->position.z < entry->territoryBase.z - entry->territoryHoming.z ||
                                        entry->position.z > entry->territoryBase.z + entry->territoryHoming.z) {
                                        trig = 1;
                                    }
                                } else {
                                    if (entry->position.x < entry->territoryBase.x - entry->territoryLoiter.x ||
                                        entry->position.x > entry->territoryBase.x + entry->territoryLoiter.x ||
                                        entry->position.z < entry->territoryBase.z - entry->territoryLoiter.z ||
                                        entry->position.z > entry->territoryBase.z + entry->territoryLoiter.z) {
                                        trig = 1;
                                    }
                                }
                            }
                            if (trig) {
                                entry->interruptFlags |= 4;
                                goto interrupt;
                            }
                        }
                        if (st == 2) {
                            MarioWork* m = marioGetPtr();
                            f32 dx = m->position.x - entry->position.x;
                            f32 dy = m->position.y - entry->position.y;
                            f32 dz = m->position.z - entry->position.z;
                            int trig;
                            PSVECSubtract(&entry->prevPosition, &entry->position, &dv2);
                            dir = entry->direction;
                            if (entry->jumpFlags & 2) {
                                if (sqrtf(dv2.x * dv2.x + dv2.z * dv2.z) > 0.05f) {
                                    dir = entry->moveDir;
                                }
                            }
                            trig = 0;
                            { s32 ok_; NPC_CHKMARIO(ok_); if (!ok_) {
                                trig = 1;
                            } else {
                                dist = sqrtf(dx * dx + dz * dz);
                                if (dist > 0.25f * entry->homingRange) {
                                    if (dy > entry->territoryLoiter.y) {
                                        trig = 1;
                                    } else if (-dy > entry->territoryHoming.y) {
                                        trig = 1;
                                    } else if (dist > entry->homingRange) {
                                        trig = 1;
                                    } else if ((dist = angleABf(0.0f, 0.0f, dx, dz), __fabsf(compAngle(dir, dist))) >
                                               0.5f * entry->homingAngle) {
                                        trig = 1;
                                    }
                                }
                            }
                            }
                            if (trig) {
                                entry->interruptFlags |= 2;
                                goto interrupt;
                            }
                        }
                        if (st == 1 || st == 4) {
                            MarioWork* m = marioGetPtr();
                            f32 dx, dy, dz;
                            int trig;
                            Vec dv;
#ifdef VERSION_G8ME01
                            f32 sd;
#endif
                            dx = m->position.x - entry->position.x;
                            dz = m->position.z - entry->position.z;
                            dy = m->position.y - entry->position.y;
#ifdef VERSION_G8ME01
                            // US: in the Bowser levels the search range shrinks with kpaGetLevel()
                            dist = sqrtf(dx * dx + dz * dz);
                            sd = dist;
                            if (entry->flags & 0x8000000) {
                                f32 kpaRange[4] = {0.0f, 0.0f, 30.0f, 100.0f};
                                sd = dist - kpaRange[kpaGetLevel()];
                                if (sd < 0.0f) {
                                    sd = 0.0f;
                                }
                            }
#else
                            dist = dx * dx + dz * dz;
                            sqrtf(dist);
#endif
                            PSVECSubtract(&entry->prevPosition, &entry->position, &dv);
                            dir = reviseAngle(entry->direction) < 180.0f ? 90.0f : 270.0f;
                            trig = 1;
                            { s32 ok_; NPC_CHKMARIO(ok_); if (!ok_) {
                                trig = 0;
                            } else {
                                f32 a14;
#ifdef VERSION_G8ME01
                                if (sd > 0.25f * entry->searchRange) {
                                    if (sd > entry->searchRange) {
#else
                                dist = sqrtf(dist);
                                if (dist > 0.25f * entry->searchRange) {
                                    if (dist > entry->searchRange) {
#endif
                                        trig = 0;
                                        goto done14;
                                    } else if ((a14 = angleABf(0.0f, 0.0f, dx, dz), __fabsf(compAngle(dir, a14))) >
                                               0.5f * entry->searchAngle) {
                                        trig = 0;
                                        goto done14;
                                    }
                                }
                                {
                                    if (dy > entry->territoryLoiter.y) {
                                        trig = 0;
                                    } else if (-dy > entry->territoryHoming.y) {
                                        trig = 0;
                                    } else if (st == 4) {
                                        if (entry->territoryType == NPC_TERRITORY_TYPE_CIRCLE) {
                                            f32 tz = entry->territoryBase.z - entry->position.z;
                                            f32 tx = entry->territoryBase.x - entry->position.x;
                                            f32 d = sqrtf(tx * tx + tz * tz);
                                            if (d > 0.75 * entry->territoryHoming.x) {
                                                trig = 0;
                                            }
                                        } else {
                                            if (entry->position.x < entry->territoryBase.x - 0.75f * entry->territoryHoming.x ||
                                                entry->position.x > entry->territoryBase.x + 0.75f * entry->territoryHoming.x ||
                                                entry->position.z < entry->territoryBase.z - 0.75f * entry->territoryHoming.z ||
                                                entry->position.z > entry->territoryBase.z + 0.75f * entry->territoryHoming.z) {
                                                trig = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            }
                            done14:
                            if (trig) {
                                entry->interruptFlags |= 1;
                            }
                        }
                    }
interrupt:
                    npcCheckInterrupt(entry);
                }
                if (!(entry->flags & 0x800)) {
                    CameraEntry* cam;

                    cam = camGetPtr((CameraId)4);
                    PSMTXMultVec(cam->view, &entry->position, &dv);
                    cam = camGetPtr((CameraId)4);
                    PSMTX44MultVec(cam->projection, &dv, &dv);
                    if ((dv.z > -1.0f && dv.z < 0.0f && dv.x >= -2.0f && dv.x <= 2.0f && dv.y >= -2.0f && dv.y <= 2.0f) ||
                        (entry->flags & 0x40)) {
                        if (entry->color.r != 0xFF || entry->color.g != 0xFF || entry->color.b != 0xFF ||
                            entry->color.a != 0xFF) {
                            animPoseSetMaterialFlagOn(entry->poseId, 0x40);
                            animPoseSetMaterialEvtColor(entry->poseId, entry->color);
                        } else {
                            animPoseSetMaterialFlagOff(entry->poseId, 0x40);
                        }
                        prio = 5000.0f + 5000.0f * dv.z;
                        if (prio < 0.0f) {
                            prio = 0.0f;
                        } else if (prio > 10000.0f) {
                            prio = 10000.0f;
                        }
                        prio *= -1.0f;
                        if (!(entry->flags & 0x80) &&
                            (!(entry->wFbatHitCheckRelated & 2) || (entry->jumpFlags & 0x10000))) {
                            if (entry->color.a != 0xFF) {
                                dispEntry((CameraId)entry->cameraId, prio, 2, npcDisp_xlu, entry);
                            } else {
                                dispEntry((CameraId)entry->cameraId, prio, 0, npcDisp, entry);
                                dispEntry((CameraId)entry->cameraId, prio, 1, npcDisp, entry);
                                dispEntry((CameraId)entry->cameraId, prio, 2, npcDisp, entry);
                            }
                            if (!(entry->flags & 0x4000000)) {
                                if (!(entry->flags & 0x40000)) {
                                    Vec sp = entry->position;
                                    if (entry->tribe) {
                                        sp.x += entry->tribe->shadowOffsetX * entry->scale.x * sinfd(reviseAngle(entry->direction) < 180.0f ? 90.0f : 270.0f);
                                        sp.y += entry->tribe->shadowOffsetY * entry->scale.y;
                                        sp.z += entry->tribe->shadowOffsetZ * entry->scale.z;
                                    }
                                    {
                                        s32 sh = shadowEntry(
                                            sp.x, sp.y, sp.z,
                                            1.33f * (entry->shadowScale * (entry->width / 3.0f * entry->scale.x)));
                                        if (entry->flags & 0x80000) {
                                            shadowSetCollision(sh, 0);
                                        }
                                    }
                                }
                            } else {
                                Vec vp = entry->position;
                                f32 o0, o1, o2, o3, o4, o5, o6;
                                o3 = 100.0f;
                                if (hitCheckFilter(NULL, &o0, &o1, &o2, &o3, &o4, &o5, &o6, vp.x, 1.0f + vp.y, vp.z,
                                                   0.0f, -1.0f, 0.0f)) {
                                    vp.y = o1;
                                }
                                animPoseVivianMain(entry->poseId, &vp);
                                {
                                    s32 sh = shadowEntry(entry->position.x, entry->position.y, entry->position.z,
                                                         entry->shadowScale * (entry->width / 3.0f * entry->scale.x));
                                    if (entry->flags & 0x80000) {
                                        shadowSetCollision(sh, 0);
                                    }
                                }
                            }
                            if (entry->flags & 0x100) {
                                dispEntry((CameraId)entry->cameraId, prio, 2, npcDisp_blur, entry);
                            }
                        }
                        if (entry->offscreenId != -1) {
                            if (entry->color.a != 0xFF) {
                                dispEntry((CameraId)1, prio, 2, npcDisp_offscreen_xlu, entry);
                            } else {
                                dispEntry((CameraId)1, prio, 0, npcDisp_offscreen, entry);
                                dispEntry((CameraId)1, prio, 1, npcDisp_offscreen, entry);
                                dispEntry((CameraId)1, prio, 2, npcDisp_offscreen, entry);
                            }
                        }
                    }
                }
                entry->prevPosition = entry->position;
                if (entry->flags & 0x100 && !(npcMainCount & 1)) {
                    entry->positionHistory[4] = entry->positionHistory[3];
                    entry->positionHistory[3] = entry->positionHistory[2];
                    entry->positionHistory[2] = entry->positionHistory[1];
                    entry->positionHistory[1] = entry->positionHistory[0];
                    entry->positionHistory[0] = entry->position;
                }
            }
            entry->jumpFlags &= ~0x10;
        }
    }

    fbat = gp->fbatData;
    {
        FirstStrikeInfo* ann = &fbat->firstStrike;
        if (ann->enabled) {
            switch (ann->type) {
            case 2:
                (ann->timer)++;
                if (ann->timer < 20) {
                    ann->x = intplGetValue(5, -600.0f, 0.0f, ann->timer, 20);
                } else if (ann->timer < 80) {
                    ann->x = 0.0f;
                } else if (ann->timer < 100) {
                    ann->x = intplGetValue(1, 0.0f, 600.0f, ann->timer - 80, 20);
                } else {
                    ann->hitKind = 0;
                    break;
                }
                ann->y = 0.0f;
                dispEntry((CameraId)9, 0.0f, 2, _fbatFirstAttackAnnouceDisp, ann);
                break;
            case 1:
                (ann->timer)++;
                if (ann->timer > 60) {
                    ann->hitKind = 0;
                    break;
                }
                ann->x = 0.0f;
                ann->y = 0.0f;
                dispEntry((CameraId)9, 0.0f, 2, _fbatFirstAttackAnnouceDisp, ann);
                break;
            }
        }
        switch (fbat->mode) {
        case 1:
            npcKoopaModeMain();
            break;
        case 2:
        case 3:
            fbatTalkMode();
            break;
        case 5:
            fbatBattleMode();
            break;
        case 6:
            break;
        }
        fbat->sandersBombTrigger = 0;
    }
}

#ifdef VERSION_G8ME01
static const Vec sZeroVec3 = {0.0f, 0.0f, 0.0f};
#endif

NpcEntry* npcNameToPtr(const char* name) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;
    for (i = 0; i < total; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->description, name)) {
            break;
        }
    }
    return entry;
}

NpcEntry* npcNameToPtr_NoAssert(const char* name) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;
    for (i = 0; i < total; i++, entry++) {
        if (entry->flags & 1 && !strcmp(entry->description, name)) {
            break;
        }
    }
    if (i >= total) {
        return NULL;
    }
    return entry;
}

void npcSetMarioAutoTalkPose(const char* stayPose, const char* talkPose) {
    strcpy(MarioStayPose, stayPose);
    strcpy(MarioTalkPose, talkPose);
}

#pragma opt_propagation off
#pragma opt_lifetimes off
// name is non-const as in the original: through a const char* the scheduler moves the first load up into the
// register saves (MWCC treats const loads as not aliasing the stores).
void npcSetTalkPose(char* name) {
    PartyEntry* party;
    NpcEntry* npc;

    if (*(s8*)name == 0) return;
    if (!strcmp(name, "(^x^)party")) return;
    if (!strcmp(name, "party")) {
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            partyChgPoseId(party, 7);
        }
    } else if (!strcmp(name, "extparty")) {
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            partyChgPoseId(party, 7);
        }
    } else if (!strcmp(name, "mario")) {
        marioChgPose(MarioTalkPose);
    } else {
        npc = npcNameToPtr(name);
        if (npc->flags & 0x200) {
            animPoseSetAnim(npc->poseId, npc->talkAnim, TRUE);
        }
    }
}

void npcSetStayPose(char* name) {
    PartyEntry* party;
    NpcEntry* npc;

    if (*(s8*)name == 0) return;
    if (!strcmp(name, "(^x^)party")) return;
    if (!strcmp(name, "party")) {
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            partyChgPoseId(party, 1);
        }
    } else if (!strcmp(name, "extparty")) {
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            partyChgPoseId(party, 1);
        }
    } else if (!strcmp(name, "mario")) {
        marioChgPose(MarioStayPose);
    } else {
        npc = npcNameToPtr(name);
        if (npc->flags & 0x200) {
            animPoseSetAnim(npc->poseId, npc->stayAnim, TRUE);
        }
    }
}

BOOL npcCheckHitMarioSide(NpcEntry* npc) {
    MarioWork* mario;
    Vec marioPos;
    f32 dist;
    f32 range;
    f32 angle;
    f32 dx; f32 dz;
    f32 rad;
    f32 len;
    f32 scale;

    mario = marioGetPtr();
    if (npc->position.y > ( mario->position.y + mario->wPlayerCollisionBox.y) ) {
        dz = mario->wPlayerCollisionBox.y;
        return FALSE;
    }
    if (mario->position.y > ( npc->position.y + npc->height) ) {
        return FALSE;
    }
    marioPos.x = mario->position.x;
    marioPos.z = mario->position.z;
    marioPos.y = npc->position.y;
    dist = PSVECDistance(&marioPos, &npc->position);
    range = (5.0f + (mario->wPlayerCollisionBox.x + npc->width)) / 2.0f;
    if (dist > range) {
        return FALSE;
    }
    npc->position.x = npc->prevPosition.x;
    npc->position.z = npc->prevPosition.z;
    dist = PSVECDistance(&marioPos, &npc->position);
    if (dist > range) {
        return FALSE;
    }
    mario->targetNpc = npc;
    angle = angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z);
    if (__fabsf(compAngle(angle, mario->directionView)) < 40.0f) {
        dx = (range - dist) * ( -polySinf((6.2831855f * mario->directionView) / 360.0f) );
        dz = (range - dist) * polyCosf((6.2831855f * mario->directionView) / 360.0f);

    } else {
        rad = (6.2831855f * angle) / 360.0f;
        dx = (npc->position.x - ( range * polySinf(rad) )) - mario->position.x;
        dz = (npc->position.z + ( range * polyCosf(rad) )) - mario->position.z;
    }
    len = sqrtf(( dx * dx ) + ( dz * dz) );
    if (len > ( (mario->wPlayerCollisionBox.z > 4.0f ) ? ( 3.0f ) : ( 1.5f))) { if (1 ) {
        scale = 2.0f / len;
        dx *= scale;
        dz *= scale;
    }
    }
    mario->position.x += dx;
    mario->position.z += dz;
    dz = mario->wPlayerCollisionBox.y;
    marioChkWallAround(&mario->position, 0, 0.0f, 160.0f + angle, 20.0f, dz);
    return TRUE;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

void npcMoveG(NpcEntry* npc) {
    f32 dy;
    f32 vel;
    f32 t;
    f32 dt;
    f32 tp;
    f32 limit;
    f32 hitX, hitY, hitZ, dist, hitNX, hitNY, hitNZ;
    f32 h;

    dy = npc->moveOffset.y;
    npc->moveOffset.y = 0.0f;
    if (npc->jumpFlags & 0x100000 && !(npc->jumpFlags & 0x10000000)) {
        dy *= 0.5f;
    }
    vel = 0.0f;
    if (npc->flags & 0x20000) {
        t = npc->jumpElapsed;
        dt = npc->moveDt;
        tp = t - dt;
        if (!(npc->jumpFlags & 0x100000) || npc->jumpFlags & 0x10000000) {
            vel = npc->jumpVelY * dt + (-980.0f * npc->jumpGravity) * (t * t - tp * tp);
        } else {
            vel = 0.4f * (npc->jumpVelY * dt) + (0.15f * (-980.0f * npc->jumpGravity)) * (t * t - tp * tp);
        }
        vel *= npc->jumpYScale;
        if (npc->maxFallSpeed > 0.0f) {
            if (npc->jumpFlags & 0x100000) {
                limit = 0.5f * (-npc->maxFallSpeed * dt);
            } else {
                limit = -npc->maxFallSpeed * dt;
            }
            if (vel < limit) {
                vel = limit;
            }
        }
    }
    dy += vel;
    if (!(npc->flags & 0x10)) {
        npc->position.y += dy;
        if (npc->position.y < -1000.0f) {
            npc->position.y = -1000.0f;
        }
    } else if (dy != 0.0f) {
        if (dy > 0.0f) {
            npc->jumpFlags |= 1;
            npc->position.y += dy;
        } else {
            f64 ad = __fabs(dy);
            f32 hh = 0.8f * npc->height;
            dist = hh + (f32)ad;
#ifdef VERSION_G8ME01
            // US: also probe the floor at both side edges of the NPC
            if ((npc->groundHit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &dist, &hitNX, &hitNY, &hitNZ,
                                              npc->position.x, npc->position.y + hh, npc->position.z, 0.0f,
                                              -1.0f, 0.0f)) != NULL) {
#else
            if ((npc->groundHit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &dist, &hitNX, &hitNY, &hitNZ,
                                              npc->position.x, npc->position.y + hh, npc->position.z, 0.0f,
                                              -1.0f, 0.0f)) == NULL) {
                npc->jumpFlags |= 1;
                npc->position.y += dy;
            } else {
#endif
                npc->jumpFlags &= ~1;
                npc->position.y = hitY;
                npc->jumpTicks = 0;
#ifdef VERSION_G8ME01
            } else if ((npc->groundHit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &dist, &hitNX, &hitNY, &hitNZ,
                                              npc->position.x + 0.5f * npc->width, npc->position.y + hh,
                                              npc->position.z, 0.0f, -1.0f, 0.0f)) != NULL) {
                npc->jumpFlags &= ~1;
                npc->position.y = hitY;
                npc->jumpTicks = 0;
            } else if ((npc->groundHit = hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &dist, &hitNX, &hitNY, &hitNZ,
                                              npc->position.x - 0.5f * npc->width, npc->position.y + hh,
                                              npc->position.z, 0.0f, -1.0f, 0.0f)) != NULL) {
                npc->jumpFlags &= ~1;
                npc->position.y = hitY;
                npc->jumpTicks = 0;
            } else {
                npc->jumpFlags |= 1;
                npc->position.y += dy;
            }
#else
            }
#endif
        }
    }
}

void npcTuningRy(NpcEntry* npc, f32 angle) {
    npc->direction = reviseAngle(angle);
}

f32 npcTransRytoFaceDir(NpcEntry* npc) {
    if (reviseAngle(npc->direction) < 180.0f) {
        return 90.0f;
    }
    return 270.0f;
}

static inline void npcSideSlide(f32* outX, f32* outZ, f32 dx, f32 dz, f32 nx, f32 nz) {
    f32 d = dx * nx + dz * nz;
    f32 ax = 0.5f * (dx - d * nx);
    f32 az = 0.5f * (dz - d * nz);
    *outX += ax;
    *outZ += az;
}

HitEntry* npcHitCheckSide(NpcEntry* npc, f32* outX, f32* outZ, f32* angle, f32 dx, f32 dz) {
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 lenSq, d, height;
    HitEntry* hit;
    f32 s, c;

    *outX = npc->position.x;
    *outZ = npc->position.z;
    if (dx == 0.0f && dz == 0.0f) {
        return NULL;
    }
    sincosf(&s, &c, angleABf(0.0f, 0.0f, dx, dz));

    dist = sqrtf(dx * dx + dz * dz) + 0.75f * npc->width;
    height = 0.2f * npc->height;
    if (height > 10.0f) {
        height = 10.0f;
    }
    if (npc->flags & 0x2000) {
        hit = hitCheckAttr(4, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    } else {
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    }
    if (hit) {
        npcSideSlide(outX, outZ, dx, dz, nx, nz);
        if (npc->flags & 0x10000) {
            f32 a = reviseAngle(180.0f + *angle);
            f32 b = reviseAngle(angleABf(0.0f, 0.0f, 100.0f * nx, 100.0f * nz));
            *angle = (s32)reviseAngle(b + reviseAngle(b - a));
        }
        return hit;
    }

    dist = sqrtf(dx * dx + dz * dz) + 0.75f * npc->width;
    height = 0.4f * npc->height;
    if (npc->flags & 0x2000) {
        hit = hitCheckAttr(4, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    } else {
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    }
    if (hit && angleABf(0.0f, 0.0f, sqrtf(nx * nx + nz * nz), -ny) >= 45.0f) {
        npcSideSlide(outX, outZ, dx, dz, nx, nz);
        if (npc->flags & 0x10000) {
            f32 a = reviseAngle(180.0f + *angle);
            f32 b = reviseAngle(angleABf(0.0f, 0.0f, 100.0f * nx, 100.0f * nz));
            *angle = (s32)reviseAngle(b + reviseAngle(b - a));
        }
        return hit;
    }

    dist = sqrtf(dx * dx + dz * dz) + 0.75f * npc->width;
    height = 0.75f * npc->height;
    if (npc->flags & 0x2000) {
        hit = hitCheckAttr(4, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    } else {
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, npc->position.y + height, npc->position.z, s, 0.0f, c);
    }
    if (hit) {
        npcSideSlide(outX, outZ, dx, dz, nx, nz);
        if (npc->flags & 0x10000) {
            f32 a = reviseAngle(180.0f + *angle);
            f32 b = reviseAngle(angleABf(0.0f, 0.0f, 100.0f * nx, 100.0f * nz));
            *angle = (s32)reviseAngle(b + reviseAngle(b - a));
        }
        return hit;
    }
    *outX += dx;
    *outZ += dz;
    return NULL;
}

NpcEntry* npcNearDistCheck(f32 x, f32 y, f32 z, f32 range) {
    NpcWork* wp;
    NpcEntry* entry;
    NpcEntry* found;
    s32 i;
    f32 dx, dy, dz;
    f32 dist;
    s32 total;

    wp = npcGetWorkPtrInline();
    found = NULL;
    entry = wp->entries;
    total = wp->total;
    for (i = 0; i < total; i++, entry++) {
        if (entry->flags & 1) {
            dx = x - entry->position.x;
            dz = z - entry->position.z;
            dy = y - entry->position.y;
            dist = sqrtf(dx * dx + dz * dz);
            sqrtf(dx * dx + dy * dy + dz * dz);
            if (dist < range) {
                found = entry;
            }
        }
    }
    return found;
}

FieldBattleData* fbatGetPointer(void) {
    return gp->fbatData;
}

void fbatChangeMode(s16 mode) {
    gp->fbatData->mode = mode;
}

void fbatSetAttackAnnounce(s32 hitKind) {
    FirstStrikeInfo* firstStrike = &gp->fbatData->firstStrike;

    firstStrike->enabled = FALSE;
    firstStrike->hitKind = hitKind;
    firstStrike->timer = 0;
    switch (firstStrike->hitKind) {
        case 0x20000:
        case 0x40000:
        case 0x80000:
        case 0x100000:
        case 0x200000:
        case 0x400000:
        case 0x800000:
        case 0x1000000:
            firstStrike->msg_tag = "fb_sensei_shita"; // You struck first!
            firstStrike->type = 2;
            firstStrike->color = (GXColor){ 0xFF, 0x80, 0x80, 0xFF };
            break;
        case 0x10000000:
            firstStrike->msg_tag = "fb_sensei_sareta"; // Your foe struck first!
            firstStrike->type = 1;
            firstStrike->color = (GXColor){ 0x80, 0x80, 0xFF, 0xFF };
            break;
        default:
            firstStrike->type = 0;
            break;
    }
}

void fbatSetAttackAnnounceEnable(void) {
    gp->fbatData->firstStrike.enabled = TRUE;
}

static inline f32 mulf(f32 a, f32 b) { return a * b; }
void npcStrippedAnnouncePool(f32* f) {
    f[0] = -600.0f;
    f[1] = 600.0f;
}

void _fbatFirstAttackAnnouceDisp(CameraId camId, void* param) {
#ifdef VERSION_G8ME01
    s32 height;
#endif
    FirstStrikeInfo* firstStrike = param;
    const char* message;
    f32 x;
    f32 y;
#ifdef VERSION_G8ME01
    u16 lines;
    u16 width;
    f32 shakeX;
    f32 shakeY;
    s32 i;
    s32 j;
    char buf[4][128];
    s8* p;

    // US: the announcement may span up to four lines
#else
    f32 width;
    f32 shakeX;
    f32 shakeY;

#endif
    message = msgSearch(firstStrike->msg_tag);
    x = firstStrike->x;
    y = firstStrike->y;
#ifdef VERSION_G8ME01
    width = FontGetMessageWidthLine(message, &lines);
    lines++;
    height = lines * 29 + 3;
    shakeX = x + (f32)(irand(10000) % 5);
    shakeY = y + (f32)(irand(10000) % 5);
    if (lines > 4) {
        lines = 4;
    }
    p = (s8*)message;
    for (i = 0; i < lines; i++) {
        for (j = 0; j < 128; j++) {
            if (p[0] == 0 || p[0] == 10) {
                buf[i][j] = '\0';
                p++;
                break;
            }
            buf[i][j] = *p;
            p++;
        }
    }
    windowDispGX_Waku_col(0, firstStrike->color, mulf(0.1f, shakeX - 10.0f - width / 2),
                          mulf(shakeY, 0.1f), mulf(20 + width, 0.1f), mulf(height, 0.1f), 1.0f);
#else
    width = FontGetMessageWidth(message);
    shakeX = x + (f32)(irand(10000) % 5);
    shakeY = y + (f32)(irand(10000) % 5);
    windowDispGX_Waku_col(0, firstStrike->color, mulf(0.1f, shakeX - 10.0f - width / 2.0f),
                          mulf(shakeY, 0.1f), mulf(20.0f + width, 0.1f), 3.2f, 1.0f);
#endif
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor((GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    FontDrawScale(0.1f);
#ifdef VERSION_G8ME01
    for (i = 0; i < lines; i++) {
        FontDrawStringShake(mulf(0.1f, x - width / 2), mulf(0.1f, y + -29 * i - 3.0f), buf[i]);
    }
#else
    FontDrawStringShake(mulf(0.1f, x - width / 2.0f), mulf(y - 3.0f, 0.1f), message);
#endif
}

void partyKeyOn(void);
void partyKeyOff(void);
void marioChgTalkMotion(void);
void marioChgStayMotion(void);
BOOL marioChkTalkable(void);
void* fbatHitCheck(u32 flags, void* result);

s32 _majinai_effect(EventEntry* event, BOOL isFirstCall);
s32 _check(EventEntry* event, BOOL isFirstCall);
s32 marioStGetSystemLevel(void);
BOOL N_itemPickUpFromFieldCheck(void);
void marioChgMotJump2(void);
f32 marioGetDispDir(void);
void npcKoopaModeEncountCheck(void);

static inline f32 fbat_camAngle(void) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 dx = camera->cameraAt.x - camera->cameraPos.x;
    f32 dz = camera->cameraAt.z - camera->cameraPos.z;
    f32 angle = reviseAngle(NPC_RAD2DEG((f32)atan2(-dx, -dz)));
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

static inline BOOL fbat_setHit(NpcEntry* npc) {
    FieldBattleData* d = gp->fbatData;
    if (d->mode != 1) {
        return FALSE;
    }
    d->mode = 5;
    d->subState = 0;
    d->npc = npc;
    d->target = npc;
    return TRUE;
}

void fbatEncountCheck(void)
{
  NpcWork *wp = (gp->inBattle) ? (&work.battle) : (&work.field);
  s32 total = wp->total;
  MarioWork *mario = marioGetPtr();
  FieldBattleData *data;
  NpcEntry *npc;
  u32 *kind;
  s32 i;
  NpcEntry *talk;
  f32 best;
  wp->talkTarget = (void *) 0;
  if (((u32) marioStGetSystemLevel()) != 0)
  {
    return;
  }
  if (!(wp->flags & 1))
  {
    if ((!npc->width) && (!npc->width))
    {
    }
    if (marioKeyOffChk() || marioCtrlOffChk())
    {
      return;
    }
  }
  if (N_itemPickUpFromFieldCheck())
  {
    return;
  }
  data = gp->fbatData;
  kind = (u32*)&data->hitKind;
  npc = fbatHitCheck(0x13f1, kind);
  if (npc && (data->mode == 1))
  {
    AnimationPose *pose;
    f32 rel;
    f32 ang;
    if (!fbat_setHit(npc))
    {
      return;
    }
    switch (*kind)
    {
      case 0x10000:

      default:
        break;

      case 0x20000:

      case 0x40000:

      case 0x80000:
        marioChgMotJump2();
        if (dbg_battleskip != 1)
      {
        animPoseSetAnim(npc->poseId, npc->tribe->damageAnimation, 1);
      }
        break;

      case 0x100000:

      case 0x200000:

      case 0x400000:
        if (dbg_battleskip != 1)
      {
        animPoseSetAnim(npc->poseId, npc->tribe->damageAnimation, 1);
      }
        break;

      case 0x800000:
        if (dbg_battleskip != 1)
      {
        animPoseSetAnim(npc->poseId, npc->tribe->damageAnimation, 1);
      }
        break;

      case 0x1000000:
        if (dbg_battleskip != 1)
      {
        animPoseSetAnim(npc->poseId, npc->tribe->damageAnimation, 1);
      }
        break;

    }

    pose = animPoseGetAnimPosePtr(npc->poseId);
    npc->savedDirection = npc->direction;
    npc->direction = angleABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z);
    rel = reviseAngle(-npc->direction);
    rel += -90.0f;
    ang = reviseAngle(fbat_camAngle() - rel);
    if (((ang > 90.0f) && (ang <= 270.0f)) || ((ang < (-90.0f)) && (ang >= (-270.0f))))
    {
      pose->peraTargetRotY = reviseAngle(180.0f + fbat_camAngle());
    }
    else
    {
      pose->peraTargetRotY = fbat_camAngle();
    }
    while ((pose->peraTargetRotY - pose->rotationY) > 180.0f)
    {
      pose->rotationY += 360.0f;
    }

    while ((pose->peraTargetRotY - pose->rotationY) < (-180.0f))
    {
      pose->rotationY -= 360.0f;
    }

    pose->rotationY += 0.75f * (pose->peraTargetRotY - pose->rotationY);
    switch (*kind)
    {
      case 0x20000:

      case 0x40000:

      case 0x80000:

      case 0x100000:

      case 0x200000:

      case 0x400000:

      case 0x800000:

      case 0x1000000:
        effSmallStarEntry(3, 3, npc->position.x, npc->position.y + npc->height, npc->position.z, 0.0f, -1.0f, 0.0f);
        break;

      case 0x10000000:
        effSmallStarEntry(3, 3, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y, mario->position.z, 0.0f, -1.0f, 0.0f);
        break;

    }

    switch (*kind)
    {
      case 0x20000:

      case 0x40000:

      case 0x80000:

      case 0x100000:

      case 0x200000:

      case 0x400000:

      case 0x800000:

      case 0x1000000:
        if (npc)
      {
        psndSFXOn_3D("SFX_BTL_FLD_ENEMY_DAMAGED1", &npc->position);
      }
        break;

      case 0x10000000:
        psndSFXOn_3D("SFX_BTL_FLD_MARIO_DAMAGED1", &mario->position);
        break;

    }

    return;
  }
  best = 1000.0f;
  if (marioChkTalkable() && (!mario->keyOffCount))
  {
    talk = (void *) 0;
    npc = wp->entries;
    for (i = 0; i < total; i++, npc++)
    {
      BOOL hit;
      if (((!(npc->flags & 2)) || (npc->flags & 8)) || (!npc->talkEvt))
      {
        continue;
      }
      hit = 0;
      if ((!(npc->position.y > (mario->position.y + mario->wPlayerCollisionBox.y))) && (!(mario->position.y > (npc->position.y + npc->height))))
      {
        Vec pos;
        f32 dist;
        BOOL facing;
        s32 j;
        f32 dir;
        f32 a;
        f32 yaw;
        s32 k;
        pos.x = mario->position.x;
        pos.z = mario->position.z;
        pos.y = npc->position.y;
        dist = PSVECDistance(&pos, &npc->position);
        if ((dist <= ((0.5f * (mario->wPlayerCollisionBox.x + npc->width)) + 10.0f)) && (dist < best))
        {
          talk = npc;
        }
        facing = 0;
        yaw = camGetPtr(CAMERA_3D)->viewYaw;
        dir = reviseAngle(marioGetDispDir() - yaw);
        for (j = 0; j < 3; j++)
        {
          // FAKE: dead int-guarded re-definition inside the loop keeps dir a named web (COLOURING M; no code, no preheader split)
          k = 0;
          if (k != 0) dir = 0.0f;
          switch (j)
          {
            case 0:
              yaw = camGetPtr(CAMERA_3D)->viewYaw;
              a = reviseAngle(angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z) - yaw);
              break;

            case 1:
              yaw = camGetPtr(CAMERA_3D)->viewYaw;
              a = reviseAngle(angleABf(mario->position.x, mario->position.z, npc->position.x + (0.5f * npc->width), npc->position.z) - yaw);
              break;

            default:
              yaw = camGetPtr(CAMERA_3D)->viewYaw;
              a = reviseAngle(angleABf(mario->position.x, mario->position.z, npc->position.x - (0.5f * npc->width), npc->position.z) - yaw);
              break;

          }

          if (a < 180.0f)
          {
            if (!(dir > 180.0f))
            {
              facing = 1;
              break;
            }
          }
          else
            if (!(dir < 180.0f))
          {
            facing = 1;
            break;
          }
        }

        if (facing)
        {
          pos.x = mario->position.x;
          pos.z = mario->position.z;
          pos.y = npc->position.y;
          dist = PSVECDistance(&pos, &npc->position);
          if (!(dist > ((0.5f * (mario->wPlayerCollisionBox.x + npc->width)) + 10.0f)))
          {
            f32 dx = npc->position.x - mario->position.x;
            f32 dz = npc->position.z - mario->position.z;
            f32 s;
            f32 c;
            f32 len;
            f32 hx;
            f32 hy;
            f32 hz;
            f32 nx;
            f32 ny;
            f32 nz;
            sincosf(&s, &c, angleABf(0.0f, 0.0f, dx, dz));
            len = sqrtf((dx * dx) + (dz * dz));
            if (!hitCheckFilter((void *) 0, &hx, &hy, &hz, &len, &nx, &ny, &nz, mario->position.x, mario->position.y + (0.75f * mario->wPlayerCollisionBox.y), mario->position.z, s, 0.0f, c))
            {
              hit = 1;
              if (dist < best)
              {
                best = dist;
                wp->talkTarget = npc;
              }
            }
          }
        }
      }
      if (hit)
      {
        if (!(npc->jumpFlags & 0x20000000))
        {
          MarioWork *m = marioGetPtr();
          npc->jumpFlags |= 0x20000000;
          npc->savedDirection = npc->direction;
          evtStopID(npc->regularEvtId);
          if (!(npc->flags & 0x01000000))
          {
            npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z, m->position.x, m->position.z));
          }
        }
      }
      else
        if (npc->jumpFlags & 0x20000000)
      {
        npc->jumpFlags &= ~0x20000000;
        if (npc->flags & 0x400000)
        {
          npc->direction = npc->savedDirection;
          evtStartID(npc->regularEvtId);
        }
        else
        {
          npc->interruptFlags |= 0x10000000;
        }
      }
    }

    mario->touchNpc = talk;
    if (wp->talkTarget && (keyGetButtonTrg(0) & PAD_BUTTON_A))
    {
      npc = wp->talkTarget;
      data = gp->fbatData;
      if (data->mode != 1)
      {
        return;
      }
      data->mode = 2;
      data->subState = 0;
      data->npc = npc;
      npc->fbatHitKind = 0;
      return;
    }
  }
  data = gp->fbatData;
  if (data->mode == 1)
  {
    u32 *kind = (u32*)&data->hitKind;
    npc = fbatHitCheck(0x2361, kind);
    if (npc && (data->mode == 1))
    {
      u32 k = *kind;
      FieldBattleData *d = gp->fbatData;
      if (d->mode == 1)
      {
        d->mode = 3;
        d->subState = 0;
        d->npc = npc;
        npc->fbatHitKind = k;
      }
      switch (*kind)
      {
        case 0x20000:

        case 0x40000:

        case 0x80000:
          marioChgMotJump2();
          break;

      }

      effSmallStarEntry(3, 3, npc->position.x, npc->position.y + npc->height, npc->position.z, 0.0f, -1.0f, 0.0f);
      return;
    }
  }
  npcKoopaModeEncountCheck();
}

void fbatTalkMode(void) {
    FieldBattleData* data = gp->fbatData;
    NpcEntry* npc = data->npc;
    MarioWork* mario = marioGetPtr();
    EventEntry* evt;

    switch (data->subState) {
        case 0:
            evtStopID(data->npc->regularEvtId);
            if (!(npc->flags & 0x01000000)) {
                npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z,
                                                      mario->position.x, mario->position.z));
            }
            marioKeyOff();
            partyKeyOff();
            switch (data->mode) {
                case 3:
                    evt = evtEntry(npc->deadEvent, 0, 0);
                    evt->wNpcEventType = 6;
                    break;
                default:
                    evt = evtEntry(npc->talkEvt, 0, 0);
                    evt->wNpcEventType = 6;
                    marioChgTalkMotion();
                    break;
            }
            evt->ownerNPC = npc;
            data->evtId = evt->eventId;
            data->subState++;
            break;
        case 1:
            if (!evtCheckID(data->evtId)) {
                if (!(npc->jumpFlags & 0x20000000)) {
                    evtStartID(data->npc->regularEvtId);
                    npc->direction = npc->savedDirection;
                }
                if (npc->flags & 0x400) {
                    npc->moveAnimState = 0;
                }
                if (npc->flags & 0x200) {
                    animPoseSetAnim(npc->poseId, npc->currentAnim, 1);
                }
                marioKeyOn();
                partyKeyOn();
                marioChgStayMotion();
                data->mode = 1;
                npcGetWorkPtrInline()->talkTarget = NULL;
            }
            break;
    }
}

#define NPC_ENEMY_LEVEL(npc) ((npc)->battleInfo.groupSetup->enemyData->mUnitKindParams->mLevel)

void BattleMajinaiDone(void);
void marioChgMoveMotion(void);

// FAKE: the recovered-item loop as a static inline helper (counter = inline-local @ temp, ptr r30), with
// the stored zero held in a local z kept alive by a dead int guard (COLOURING M; folded after RA, no code):
// i = 0 / z = 0 share a block so VN makes z `mr z,i`, numbered between the SR pointer and the counter.
// The do-while keeps the deleted arm from leaving branches behind.
static inline void fbatRecoverItems(NpcEntry* npc) {
    NpcBattleInfo* b = &npc->battleInfo;
    s32 z;
    s32 i;
    i = 0;
    z = 0;
    if (i != 0) {
        z = 1;
    }
    do {
        if (b->recoveredItems[i]) {
#ifdef VERSION_G8ME01
            ItemEntry* item = itemEntry(NULL, b->recoveredItems[i], 11, -1, 0, npc->position.x, npc->position.y,
                                          npc->position.z);
            if (item) {
                itemFlagOn(item, 0x100);
            }
#else
            itemFlagOn(itemEntry(NULL, b->recoveredItems[i], 11, -1, 0, npc->position.x, npc->position.y,
                                 npc->position.z),
                       0x100);
#endif
            b->recoveredItems[i] = z;
        }
        i++;
    } while (i < 8);
}

void fbatBattleMode(void) {
    FieldBattleData* data;
    NpcWork* wp;
    s32 num;
    EventEntry* evt;
    s32 result;
    s32 mult;
    NpcEntry* cur;
    FieldBattleInfo* info;
    s32 stolen;
    BOOL skip;
    BOOL win;
    s32 res;
    PouchData* pouch;
    MarioWork* mario;
    data = gp->fbatData;
    cur = data->target;
    pouch = pouchGetPtr();
    mario = marioGetPtr();
    cur->rotation.y = 0.0f;
    // FAKE: dead int-guarded re-definition keeps mario a named web (COLOURING M)
    num = 0;
    if (num != 0) {
        mario = NULL;
    }
    switch (data->subState) {
        case 0:
            if (dbg_battleskip == 1) {
                data->mode = 1;
                break;
            }
            if (!cur->battleInfo.groupSetup) {
                data->mode = 1;
                break;
            }
            if (gp->systemLevelFlags & 0x10) {
                data->mode = 1;
                break;
            }
            if (!cur->deadEvent) {
                data->mode = 1;
                break;
            }
            data->target->interruptFlags &= 0xF00;
            evtStopID(data->target->regularEvtId);
            info = &data->info;
            if (!(cur->battleInfo.battleFlags & 0x10000000)) {
                if (pouchEquipCheckBadge(ITEM_BUMP_ATTACK) && NPC_ENEMY_LEVEL(cur) < pouch->level) {
                    skip = TRUE;
                    win = TRUE;
                    goto decided;
                }
                if (pouchEquipCheckBadge(ITEM_FIRST_ATTACK) && ((u32)data->hitKind & 0x01FE0000) &&
                    NPC_ENEMY_LEVEL(cur) < pouch->level) {
                    skip = TRUE;
                    win = TRUE;
                    goto decided;
                }
            }
            if (dbg_battleskip == 2) {
                skip = TRUE;
                win = TRUE;
            } else {
                skip = FALSE;
                win = FALSE;
            }
        decided:
            if (!skip) {
                if (!win) {
                    gp->fbatData->firstStrike.enabled = TRUE;
                }
                marioFBattlePrepare();
                seqSetSeq(SEQ_BATTLE, NULL, NULL);
                if ((u32)data->hitKind == 0x10000000) {
                    marioChgDamagePose();
                }
                data->subState = 1;
            } else {
                if (win) {
                    BattleInformationSetResult(info, 2);
                } else {
                    BattleInformationSetResult(info, 5);
                }
                data->subState = 4;
            }
            break;
        case 1:
            if (gp->inBattle) {
                data->subState++;
            }
            break;
        case 2:
            if (!gp->inBattle) {
                data->merleeCoinBonus = 0;
                result = BattleInformationGetResult(&data->info);
                if (pouch->mMerleeNextCurseType == 4 && result == 1) {
                    data->subState = 3;
                    data->merleeCoinBonus = 1;
                    evt = evtEntry(_evt_majinai_coin_up_effect, 0, 0);
                    data->merleeEvtId = evt->eventId;
                    BattleMajinaiDone();
                    marioKeyOff();
                    wp = npcGetWorkPtrInline();
                    num = wp->total;
                    for (result = 0, cur = wp->entries; result < num; result++, cur++) {
                        if (cur && (cur->flags & 2) && (cur->flags & 0x10000000)) {
                            cur->jumpFlags |= 0x10000;
                            evtStopID(cur->regularEvtId);
                            animPoseSetLocalTimeRate(cur->poseId, 0.0f);
                        }
                    }
                } else {
                    marioFBattlePost();
                    data->subState = 4;
                }
            }
            break;
        case 3:
            if (!evtCheckID(data->merleeEvtId)) {
                marioKeyOn();
                marioFBattlePost();
                data->merleeEvtId = 0;
                data->subState = 4;
                wp = npcGetWorkPtrInline();
                num = wp->total;
                for (result = 0, cur = wp->entries; result < num; result++, cur++) {
                            if (cur && (cur->flags & 2) && (cur->flags & 0x10000000) &&
                        (cur->jumpFlags & 0x10000)) {
                        cur->jumpFlags &= ~0x10000;
                        evtStartID(cur->regularEvtId);
                        animPoseSetLocalTimeRate(cur->poseId, 1.0f);
                    }
                }
            }
            break;
        case 4:
            marioChgMoveMotion();
            if (!gp->inBattle) {
                FieldBattleInfo* info4;
                info4 = &data->info;
                res = BattleInformationGetResult(info4);
                // FAKE: the stolen check as a loop with break over a byte offset (int counter): MWCC unrolls it
                // into the if/else chain with every offset folded into cur, and the 8 loads share one vreg
                // (an indexed `stolenItems[k]` loop is strength-reduced and leaves an addi base)
                {
                    int k;
                    stolen = FALSE;
                    for (k = 0; k < sizeof(cur->battleInfo.stolenItems); k += sizeof(ItemType)) {
                        if (*(ItemType*)((u8*)cur->battleInfo.stolenItems + k)) {
                            stolen = TRUE;
                            break;
                        }
                    }
                }
                // FAKE: dead int guard (COLOURING W; folded after RA, no code). The arm redefines stolen (dead in
                // the arm), so its 6 division temps raise only res/info4 (simplify pass 2), and `res = 0` keeps
                // res a named web numbered between mario and info (declared after win)
                {
                    s32 gz = 0;
                    if (gz != 0) {
                        stolen = (res / (s32)(AddrWord)info4) + ((s32)(AddrWord)info4 / res) + ((res + 1) / (s32)(AddrWord)info4) +
                                 (((s32)(AddrWord)info4 + 1) / res) + ((res + 2) / (s32)(AddrWord)info4) + (((s32)(AddrWord)info4 + 2) / res);
                        res = 0;
                    }
                }
                data->mode = 1;
                if (res == 1 || res == 2) {
                    mult = 1;
                    if (BattleInformationGetResult(info4) == 1) {
                        cur->btlCoinDropCount = info4->coins;
                    } else if (res == 1) {
                        cur->btlCoinDropCount = cur->baseCoinCount;
                    } else {
                        cur->btlCoinDropCount = 0;
                        mult = 0;
                        goto apply;
                    }
                    if (data->merleeCoinBonus) {
                        mult = 3;
                    }
                    if (pouchEquipCheckBadge(ITEM_MONEY_MONEY)) {
                        mult += pouchEquipCheckBadge(ITEM_MONEY_MONEY);
                    }
                    if (cur->battleInfo.battleFlags & 0x100) {
                        mult++;
                    }
                apply:
                    cur->btlCoinDropCount *= mult;
                    if (!stolen) {
                        npcGroupDead(cur, 0);
                        break;
                    }
                    if (cur->flags & 0x400) {
                        cur->moveAnimState = 0;
                    }
                    cur->wFbatHitCheckRelated = 90;
                    cur->interruptFlags &= ~0x100;
                    cur->interruptFlags |= 0x10000000;
                    if (cur->flags & 0x200) {
                        animPoseSetAnim(cur->poseId, cur->currentAnim, 1);
                    }
                    fbatRecoverItems(cur);
                } else if (res == 4) {
                    if (cur->flags & 0x400) {
                        cur->moveAnimState = 0;
                    }
                    cur->interruptFlags |= 0x10000000;
                    marioSetMutekiTime(3000);
                    mario->coinDropTimer = sysMsec2Frame(3000);
                    if (cur->flags & 0x200) {
                        animPoseSetAnim(cur->poseId, cur->currentAnim, 1);
                    }
                }
            }
            break;
    }
}

void _npcDead(NpcEntry* npc, u16 reason) {
    npc->interruptFlags |= 0x10;
    npc->jumpFlags |= 4;
    npc->koopaEncounterType = reason;
    if (npc->flags & 4) {
        fbatGetPointer()->deadInfo[0].mask |= 1 << npc->setupIndex;
    }
}

void _npcGroupDead(NpcEntry* entry, u16 reason) {
    int i;

    if (entry->next) {
        _npcGroupDead(entry->next, reason);
    }
    for (i = 0; i < 4; i++) {
        if (entry->slaves[i]) {
            _npcDead(entry->slaves[i], reason);
        }
    }
    _npcDead(entry, reason);
}

void npcGroupDead(NpcEntry* entry, u16 reason) {
    if (entry->master) {
        entry = entry->master;
    }
    while (entry->prev) {
        entry = entry->prev;
    }
    _npcGroupDead(entry, reason);
}

NpcEntry* fbatNpcTalkCheck(void) {
    NpcWork* wp = npcGetWorkPtrInline();
    if (wp->talkTarget && marioChkTalkable()) {
        return wp->talkTarget;
    }
    return NULL;
}

NpcEntry* fbatHitCheckAll(void) {
    return fbatHitCheck(0x13f1, NULL);
}

// FAKE: fbatHitCheck reads its NpcEntry/MarioWork/GlobalWork/CameraEntry/FieldBattleData fields as (*(T*)&((S*)p)->field): the plain field forms lower the function's inline cost, and the deferred inliner then pulls _npcHitCheckSphere/_npcHitCheckHammerAllMotion into it

typedef struct FbatHitResult {
    u32 kind;   // 0x0
    s32 evtId;  // 0x4
    f32 dist;   // 0x8
    u32 bits; // 0xC
} FbatHitResult;

s32 vivianGetStatus(void);
s32 marioGetJabaraState(void);
s32 nokonokoGetStatus(PartyEntry* party);
f32 marioGetAngleKaitenHammer(void);
NpcEntry* _npcHitCheckSphere(NpcEntry* npc, f32* dist, f32 x, f32 y, f32 z, f32 r);
NpcEntry* _npcHitCheckHammerAllMotion(NpcEntry* npc, f32* dist, f32 r, f32 angle);

static inline BOOL fbat_canHit(NpcEntry* npc) {
    MarioWork* m = marioGetPtr();
    NpcWork* wp = npcGetWorkPtrInline();
    if (m->flags & (MARIO_FLAG_BG_MODE | MARIO_FLAG_IS_8_BIT)) return FALSE;
    if (!(wp->flags & 1) && (m->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF))) return FALSE;
    if ((*(s32*)&((GlobalWork*)gp)->mapEventId)) return FALSE;
    if (seqGetSeq() != 2) return FALSE;
    if ((*(u16*)&((MarioWork*)m)->currMotionId) == MARIO_MOTION_SHIP) return FALSE;
    if (vivianGetStatus()) return FALSE;
    if ((*(u16*)&((MarioWork*)m)->currMotionId) == MARIO_MOTION_DAMAGE || (*(u16*)&((MarioWork*)m)->currMotionId) == MARIO_MOTION_DAMAGE_TOGE) return FALSE;
    if ((*(u16*)&((NpcEntry*)npc)->wFbatHitCheckRelated)) return FALSE;
    return TRUE;
}

#define FH_NPC_FILTER(f1000, bit)                                                    \
    if (npc == NULL) continue;                                                       \
    if (!(npc->flags & 2)) continue;                                                 \
    if (f1000) { if (!(npc->flags & 8)) continue; } else { if (npc->flags & 8) continue; } \
    if (soft && !((*(u32*)&((NpcEntry*)npc)->reactionFlags) & (bit))) continue;                              \
    if (npc->flags & 0x20000000) continue;                                           \
    if (npc->flags & 0x20) continue;                                                 \
    if ((*(u32*)&((NpcEntry*)npc)->jumpFlags) & 4) continue;                                           \
    if (npc->flags & 0x08000000) continue;

#define FH_NPC_FILTER2(f1000, bit)                                                         \
    if (npc == NULL) continue;                                                             \
    if (!(npc->flags & 2)) continue;                                                       \
    if (f1000) { if (!(npc->flags & 8)) continue; } else { if (npc->flags & 8) continue; } \
    if (soft && !((*(u32*)&npc->reactionFlags) & (bit))) continue;            \
    if (npc->flags & 0x20000000) continue;                                                 \
    if (npc->flags & 0x20) continue;                                                       \
    if (npc->flags & 0x08000000) continue;                                                 \
    if ((*(u32*)&((NpcEntry*)npc)->jumpFlags) & 4) continue;

typedef struct { s32 v[11]; } SinTbl;
static const SinTbl sSinTbl = {{0, 0, 0, 0, 45, 60, 90, 100, 101, 101, 101}};

void* fbatHitCheck(u32 flags, void* result) {
    u32 f1000a;
    u32 f20;
    u32 noJump;
    u32 soft;
    NpcEntry* hit = NULL;
    NpcEntry* npc = NULL;
    s32 i;
    s32 total;
    f32 dist;
    f32 angle, r2, angle2, r;
    PartyEntry* party;
    FbatHitResult res;
    MarioWork* mario;
    NpcWork* wp = npcGetWorkPtrInline();

    mario = marioGetPtr();
    res.kind = 0;
    res.dist = 1000.0f;
    res.bits &= ~1;
    soft = flags & 0x2000;
    if (soft) {
        res.bits |= 1;
    }

    if (flags & 0x30) {
        noJump = flags & 0x10;
        total = wp->total;
        npc = wp->entries;
        if (!noJump || mario->mutekiTimer <= 0) {
            if (fbat_canHit(npc)) {
                f1000a = flags & 0x1000;
                f20 = flags & 0x20;
                for (i = 0; i < total; i++, npc++) {
                    f32 dx, dz;
                    if (npc == NULL) continue;
                    if (!(npc->flags & 2)) continue;
                    if (f1000a) {
                        if (!(npc->flags & 8)) continue;
                    } else {
                        if (npc->flags & 8) continue;
                    }
                    if (soft && !(npc->reactionFlags & 1)) continue;
                    if (npc->flags & 0x20) continue;
                    if (npc->jumpFlags & 4) continue;
                    if (npc->flags & 0x08000000) continue;
                    if (mario->position.y >= (*(f32*)&((NpcEntry*)npc)->position.y) + (*(f32*)&((NpcEntry*)npc)->height)) continue;
                    if (mario->position.y + mario->wPlayerCollisionBox.y <= (*(f32*)&((NpcEntry*)npc)->position.y)) continue;
                    dz = mario->position.z - npc->position.z;
                    dx = mario->position.x - (*(f32*)&((NpcEntry*)npc)->position.x);
                    dist = sqrtf(dx * dx + dz * dz);
                    if (dist > 0.5f * (mario->wPlayerCollisionBox.x + (*(f32*)&((NpcEntry*)npc)->width))) continue;
                    if (dist >= res.dist) continue;
                    if (noJump && !((*(u32*)&((NpcEntry*)npc)->jumpFlags) & 0x10000) && mario->mutekiTimer <= 0) {
                        if ((*(s32*)&((NpcEntry*)npc)->attackMode) == 0 ||
                            (evtGetPtrID((*(s32*)&((NpcEntry*)npc)->regularEvtId)) ? evtGetPtrID((*(s32*)&((NpcEntry*)npc)->regularEvtId))->wNpcEventType == 5 : 0)) {
                            if (npc->flags & 0x20000000) continue;
                            res.kind = 0x10000;
                            hit = npc;
                            res.dist = dist;
                            if (!soft) fbatSetAttackAnnounce(0x10000);
                        } else if (pouchEquipCheckBadge(ITEM_CHILL_OUT)) {
                            res.kind = 0x10000;
                            hit = npc;
                            res.dist = dist;
                            if (!soft) fbatSetAttackAnnounce(0x10000);
                        } else {
                            res.kind = 0x10000000;
                            hit = npc;
                            res.dist = dist;
                            if (!soft) {
                                fbatSetAttackAnnounce(0x10000000);
                            }
                            res.evtId = (*(s32*)&((NpcEntry*)npc)->attackMode);
                        }
                    }
                    if (!f20) continue;
                    if ((mario->flags & MARIO_FLAG_IS_FALLING) && !(mario->flags & MARIO_FLAG_IS_JUMPING) &&
                        !(npc->flags & 0x20000000)) {
                        hit = npc;
                        res.dist = dist;
                        if (marioGetJabaraState() == 6) {
                            res.kind = 0x20000;
                            if (!soft) fbatSetAttackAnnounce(0x20000);
                        } else if (mario->currMotionId == MARIO_MOTION_HIP) {
                            res.kind = 0x40000;
                            if (!soft) fbatSetAttackAnnounce(0x40000);
                        } else {
                            res.kind = 0x20000;
                            if (!soft) fbatSetAttackAnnounce(0x20000);
                        }
                    } else if (marioGetJabaraState() == 6 && !(npc->flags & 0x20000000)) {
                        res.kind = 0x20000;
                        hit = npc;
                        res.dist = dist;
                        if (!soft) fbatSetAttackAnnounce(0x20000);
                    }
                }
            }
        }
    }

    if (flags & 0x40) {
        while (hit == NULL) {
            if ((flags & 0x10) && mario->mutekiTimer > 0) break;
            if (!fbat_canHit(npc)) break;
            if (flags & 1) {
                MarioWork* m;
                s32 v;
                f32 f;
                f32 c2, s2, c1, s1;
                f32 x, y, z;
                f32 r, angle;
                if (mario->currMotionId != MARIO_MOTION_HAMMER) break;
                r = 1.5f * mario->wPlayerCollisionBox.x;
                angle = reviseAngle(mario->displayDirection - 90.0f + (*(f32*)&((CameraEntry*)camGetPtr(CAMERA_3D))->viewYaw));
                m = marioGetPtr();
                animPoseGetAnimPosePtr(m->paperAnimGroupId[marioAnimeId()]);
                v = (s32)m->multiTimer;
                if (v > 20) v = 10;
                f = v;
                m = marioGetPtr();
                {
                    SinTbl tbl = sSinTbl;
                    if (f < 0.0f) f = 0.0f;
                    if (f >= 11.0f) f = 10.0f;
                    sincosf(&s1, &c1, angle);
                    sincosf(&s2, &c2, tbl.v[(s32)f]);
                }
                x = m->position.x + s2 * (s1 * r);
                z = m->position.z + s2 * (c1 * r);
                y = m->position.y + r * c2;
                npc = wp->entries;
                for (i = 0; i < total; i++, npc++) {
                    FH_NPC_FILTER2(flags & 0x1000, 2)
                    if (!_npcHitCheckSphere(npc, &dist, x, y, z, 12.0f)) continue;
                    if (dist >= res.dist) continue;
                    res.dist = dist;
                    hit = npc;
                    res.kind = 0x100000;
                    if (!soft) fbatSetAttackAnnounce(0x100000);
                    break;
                }
            } else {
                r = 1.5f * mario->wPlayerCollisionBox.x;
                angle = reviseAngle(mario->displayDirection - 90.0f + (*(f32*)&((CameraEntry*)camGetPtr(CAMERA_3D))->viewYaw));
                npc = wp->entries;
                for (i = 0; i < total; i++, npc++) {
                    FH_NPC_FILTER(flags & 0x1000, 2)
                    if (!_npcHitCheckHammerAllMotion(npc, &dist, r, angle)) continue;
                    if (dist >= res.dist) continue;
                    res.dist = dist;
                    hit = npc;
                    res.kind = 0x100000;
                    if (!soft) fbatSetAttackAnnounce(0x100000);
                    break;
                }
            }
            break;
        }
    }

    if (flags & 0x80) {
        while (hit == NULL) {
            if ((flags & 0x10) && mario->mutekiTimer > 0) break;
            if (!fbat_canHit(npc)) break;
            if (flags & 1) {
                f32 angle, r;
                MarioWork* m;
                f32 c, s, x, y, z;
                if (mario->currMotionId != MARIO_MOTION_HAMMER_2) break;
                angle = marioGetAngleKaitenHammer();
                if (angle < 0.0f) break;
                r = 1.5f * mario->wPlayerCollisionBox.x;
                m = marioGetPtr();
                sincosf(&s, &c, angle);
                x = s * r + m->position.x;
                z = c * r + m->position.z;
                y = 15.0f + m->position.y;
                npc = wp->entries;
                for (i = 0; i < total; i++, npc++) {
                    FH_NPC_FILTER(flags & 0x1000, 2)
                    if (!_npcHitCheckSphere(npc, &dist, x, y, z, 12.0f)) continue;
                    if (dist >= res.dist) continue;
                    res.dist = dist;
                    hit = npc;
                    { FbatHitResult* rp = &res; if (pouchGetHammerLv() < 3) { rp->kind = 0x200000; } else { rp->kind = 0x400000; } } // FAKE: store through a pointer to res (keeps the two branch stores, no if-conversion); same field
                    if (!soft) fbatSetAttackAnnounce(res.kind);
                    break;
                }
            } else {
                r2 = 1.5f * mario->wPlayerCollisionBox.x;
                angle2 = reviseAngle(mario->displayDirection - 90.0f + (*(f32*)&((CameraEntry*)camGetPtr(CAMERA_3D))->viewYaw));
                npc = wp->entries;
                for (i = 0; i < total; i++, npc++) {
                    FH_NPC_FILTER(flags & 0x1000, 2)
                    if (!_npcHitCheckHammerAllMotion(npc, &dist, r2, angle2)) continue;
                    if (dist >= res.dist) continue;
                    res.dist = dist;
                    hit = npc;
                    { FbatHitResult* rp = &res; if (pouchGetHammerLv() < 3) { rp->kind = 0x200000; } else { rp->kind = 0x400000; } } // FAKE: store through a pointer to res (keeps the two branch stores, no if-conversion); same field
                    if (!soft) fbatSetAttackAnnounce(res.kind);
                    break;
                }
            }
            break;
        }
    }

    if (flags & 0x100) {
        party = partyGetPtr(marioGetPartyId());
        while (hit == NULL) {
            u32 b;
            f32 x, y, z, r;
            if ((flags & 0x10) && mario->mutekiTimer > 0) break;
            if (!fbat_canHit(npc)) break;
            b = flags & 1;
            if (b) {
                s32 st;
                if (party == NULL) break;
                if (party->memberKind != 2) break;
                if (!(party->flags & PARTY_FLAG_IS_BEING_USED)) break;
                st = nokonokoGetStatus(party);
                if (st < 2 || st > 5) break;
            }
            if (!b) break;
            r = 0.5f * party->width;
            y = 0.5f * party->height + party->position.y;
            x = party->position.x;
            z = party->position.z;
            npc = wp->entries;
            for (i = 0; i < total; i++, npc++) {
                FH_NPC_FILTER(flags & 0x1000, 4)
                if (!_npcHitCheckSphere(npc, &dist, x, y, z, r)) continue;
                if (dist >= res.dist) continue;
                res.dist = dist;
                hit = npc;
                res.kind = 0x800000;
                if (!soft) fbatSetAttackAnnounce(0x800000);
                break;
            }
            break;
        }
    }

    if (flags & 0x200) {
        party = partyGetPtr(marioGetPartyId());
        while (hit == NULL) {
            if ((flags & 0x10) && mario->mutekiTimer > 0) break;
            if (!fbat_canHit(npc)) break;
            if (!(flags & 1)) break;
            if (party == NULL) break;
            if (party->memberKind != 3) break;
            if (!(*(u8*)&((FieldBattleData*)gp->fbatData)->sandersBombTrigger)) break;
            npc = wp->entries;
            flags &= 0x1000;
            for (i = 0; i < total; i++, npc++) {
                FH_NPC_FILTER(flags, 8)
                if (!_npcHitCheckSphere(npc, &dist, party->position.x, 0.5f * party->height + party->position.y,
                                        party->position.z, 30.0f)) continue;
                if (dist >= res.dist) continue;
                res.dist = dist;
                hit = npc;
                res.kind = 0x1000000;
                if (!soft) fbatSetAttackAnnounce(0x1000000);
                break;
            }
            break;
        }
    }

    if (result) {
        *(FbatHitResult*)result = res;
    }
    return hit;
}

void npcStrippedHitPool(f32* f) {
    f[0] = 15.0f;
}

NpcEntry* _npcHitCheckSphere(NpcEntry* npc, f32* dist, f32 x, f32 y, f32 z, f32 r) {
    f32 dx = x - npc->position.x;
    f32 dy = y - npc->position.y;
    f32 dz = z - npc->position.z;

    if (y != npc->position.y) {
        if (y >= npc->position.y && y <= npc->position.y + npc->height) {
            dy = 0.0f;
        } else if (y > npc->position.y + npc->height) {
            dy = y - (npc->position.y + npc->height);
        }
    }
    *dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (*dist < 0.5f * npc->width + r) {
        return npc;
    }
    return NULL;
}

NpcEntry* _npcHitCheckHammerAllMotion(NpcEntry* npc, f32* dist, f32 r, f32 angle) {
    f32 t;
    f32 d;
    float tmpi2;
    NpcEntry* hit;

    for (t = 0.0f; t <= 11.0f; t += 1.0f) {
        f32 f = t;
        MarioWork* mario = marioGetPtr();
        f32 c2, s2, c1, s1;
        f32 x, y, z;
        SinTbl tbl = *(volatile const SinTbl*)&sSinTbl; // FAKE: volatile copy needed for 100% (87.59 with a plain struct copy)

        if (f < 0.0f) {
            f = 0.0f;
        }
        if (f >= 11.0f) {
            f = 10.0f;
        }
        sincosf(&s1, &c1, angle);
        sincosf(&s2, &c2, tbl.v[(s32)f]);
        tmpi2 = mario->position.x + s2 * (s1 * r);
        x = tmpi2;
        z = mario->position.z + s2 * (c1 * r);
        y = mario->position.y + r * c2;
        hit = _npcHitCheckSphere(npc, &d, x, y, z, 12.0f);
        if (hit) {
            break;
        }
    }
    *dist = d;
    return hit;
}

BOOL npcCheckInterrupt(NpcEntry* npc) {
    EventEntry* evt;
    u32 flags;

    evtGetPtrID(npc->regularEvtId);
    flags = npc->interruptFlags;
    if (flags & ~0xF00) {
        evt = NULL;
        if ((flags & 0x10) && npc->deadEvent) {
            evt = evtEntry(npc->deadEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 8;
            evt->ownerNPC = npc;
        } else if ((flags & 0x10000000) && npc->moveEvent) {
            evt = evtEntry(npc->moveEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 1;
            evt->ownerNPC = npc;
        } else if ((flags & 8) && npc->blowEvent) {
            evt = evtEntry(npc->blowEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 5;
            evt->ownerNPC = npc;
        } else if ((flags & 1) && npc->findEvent && !(npc->jumpFlags & 0x1000) &&
                   !(npc->flags & 0x200000) && !(flags & 0x100)) {
            evt = evtEntry(npc->findEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 2;
            evt->ownerNPC = npc;
        } else if ((flags & 2) && npc->lostEvent && !(npc->jumpFlags & 0x1000) &&
                   !(npc->flags & 0x200000)) {
            evt = evtEntry(npc->lostEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 3;
            evt->ownerNPC = npc;
        } else if ((flags & 4) && npc->returnEvent && !(npc->jumpFlags & 0x1000) &&
                   !(npc->flags & 0x200000)) {
            evt = evtEntry(npc->returnEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 4;
            evt->ownerNPC = npc;
        }
        if (evt) {
            evtDeleteID(npc->regularEvtId);
            npc->regularEvtId = evt->eventId;
            npc->interruptFlags &= 0xF00;
            npc->jumpFlags &= ~0x1000;
            npc->moveTicks = 0;
            npc->moveSpeed = 0.0f;
            return TRUE;
        }
    }
    npc->interruptFlags &= 0xF00;
    npc->jumpFlags &= ~0x1000;
    return FALSE;
}

static inline void npcRandField(NpcBattleInfo* info, NpcSetupBattleInfo* setup) {
    s32 i;
    for (i = 0; i < 16; i++) {
        info->random.values[i] = (u8)setup->randomRanges[i][0] + irand(setup->randomRanges[i][1] - setup->randomRanges[i][0] + 1);
    }
}

void npcSetupBattleInfo(NpcEntry* npc, NpcSetupBattleInfo* setup) {
    s32 i;
    s32 num;
    NpcBattleInfo* info;
    NpcBattleSelect* select;
    BattleUnitSetup* unit;
    ItemDropData* table;
    ItemDropData* drop;
    s32 total;

    info = &npc->battleInfo;
    memset(info, 0, sizeof(NpcBattleInfo));
    memset(info->recoveredItems, 0, sizeof(info->recoveredItems));
    if (setup && setup->name) {
        info->random.mode = setup->randomMode;
        switch (info->random.mode) {
            case 0:
            default:
                npcRandField(info, setup);
                break;
            case 1:
            case 2:
                break;
        }
        info->battleFlags = setup->battleFlags;
        info->fixedItem = setup->fixedItem;
        info->itemDropped = ITEM_NULL;
        info->heartsDropped = 0;
        info->flowersDropped = 0;
        info->bgmName = setup->bgmName;
        if (setup->swFlag >= -EVTDAT_GSWF_BASE && setup->swFlag <= EVTDAT_GSWF_MAX) {
            if (swGet(setup->swFlag + EVTDAT_GSWF_BASE)) {
                select = setup->altSelect;
            } else {
                select = setup->select;
            }
        } else {
            select = setup->select;
        }
        info->groupSetup = NULL;
        if (select) {
            NpcBattleSelect* p;
            s32 w;

            total = 0;
            for (p = select; (w = p->weight) > 0; p++) {
                total += w;
            }
            if (total > 0) {
                total = irand(total);
                while (TRUE) {
                    if ((total -= select->weight) < 0) {
                        break;
                    }
                    select++;
                }
                info->groupSetup = select->groupSetup;
                info->stageData = select->stageData;
            }
        }
        if (info->groupSetup) {
            num = info->groupSetup->numEnemies;
            unit = info->groupSetup->enemyData;
            memset(info->heldItems, 0, sizeof(info->heldItems));
            for (i = 0; i < num; unit++, i++) {
                table = unit->itemDropTable;
                if (table) {
                    total = 0;
                    for (drop = table; drop->holdWeight > 0 || drop->type; drop++) {
                        total += drop->holdWeight;
                    }
                    if (total > 0) {
                        total = irand(total);
                        while (TRUE) {
                            if ((total -= table->holdWeight) < 0) {
                                info->heldItems[i] = table->type;
                                info->itemDropLists[i] = unit->itemDropTable;
                                break;
                            }
                            table++;
                        }
                    }
                }
            }
        }
        npc->baseCoinCount = 0;
        if (info->groupSetup) {
            npc->baseCoinCount = info->groupSetup->enemyData->mUnitKindParams->baseCoin;
        }
        npc->flags |= 0x10000000;
    }
}

void npcSetBattleInfo(NpcEntry* npc, s32 battleInfoId) {
    NpcSetupBattleInfo* setup;
    char areaName[4];
    s32 i;

    if (battleInfoId == -1) {
        setup = NULL;
    } else {
        areaName[0] = gp->currentMapName[0];
        areaName[1] = gp->currentMapName[1];
        areaName[2] = gp->currentMapName[2];
        areaName[3] = '\0';
        setup = areaDataPtr(areaName)->btlSetupTbl;
        for (i = battleInfoId; i > 0; i--) {
            setup++;
        }
    }
    npcSetupBattleInfo(npc, setup);
    for (i = 0; i < 4; i++) {
        if (npc->slaves[i]) {
            npc->slaves[i]->battleInfo = npc->battleInfo;
        }
    }
}

void npcStopForEvent(void) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;
    for (i = 0; i < total; i++, entry++) {
        npcStopForOneEvent(entry);
    }
}

void npcStopForOneEvent(NpcEntry* npc) {
    if (npc && npc->flags & 2 && npc->flags & 0x10000000) {
        npc->jumpFlags |= 0x10000;
        evtStopID(npc->regularEvtId);
        animPoseSetLocalTimeRate(npc->poseId, 0.0f);
    }
}

void npcStartForEvent(void) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;
    if (total) {
    }
    for (i = 0; i < total; i++, entry++) {
        npcStartForOneEvent(entry);
    }
}

void npcStartForOneEvent(NpcEntry* npc) {
    if (npc && npc->flags & 2 && npc->flags & 0x10000000 && npc->jumpFlags & 0x10000) {
        npc->jumpFlags &= ~0x10000;
        evtStartID(npc->regularEvtId);
        animPoseSetLocalTimeRate(npc->poseId, 1.0f);
    }
}

void npcSetColor(const char* name, GXColor color) {
    npcNameToPtr(name)->color = color;
}

void npcBlurOn(const char* name) {
    NpcEntry* npc = npcNameToPtr(name);

    npc->flags |= 0x100;
    npc->positionHistory[0] = npc->position;
    npc->positionHistory[1] = npc->position;
    npc->positionHistory[2] = npc->position;
    npc->positionHistory[3] = npc->position;
    npc->positionHistory[4] = npc->position;
}

void npcBlurOff(const char* name) {
    npcNameToPtr(name)->flags &= ~0x100;
}

s32 npcGetBtlSetupWork(NpcEntry* npc, s32 index) {
    return npc->battleInfo.btlSetupWork[index];
}

void npcSetBtlSetupWork(NpcEntry* npc, s32 index, s32 value) {
    npc->battleInfo.btlSetupWork[index] = value;
}

void npcSetSlave(NpcEntry* npc, NpcEntry* slave, s32 id) {
    slave->master = npc;
    npc->slaves[id] = slave;
}

void npcSetLink(NpcEntry* npc, NpcEntry* other) {
    npc->next = other;
    other->prev = npc;
}

NpcTerritoryType dbGetDefData(NpcTerritoryInfo* info, const char* description) {
    NpcTerritoryType type;

    type = NPC_TERRITORY_TYPE_NOTHING;
    while (TRUE) {
        if (!info->description) {
            break;
        }
        if (!strcmp(info->description, description)) {
            type = info->type;
            break;
        }
        info++;
    }
    return type;
}

void npcExecAllInitEvt(void) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;
    EventEntry* evt;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;

    for (i = 0; i < total; i++, entry++) {
        if (entry && entry->flags & 1) {
            if (entry->initEvent) {
                if (entry->initEvtId) {
                    evtDeleteID(entry->initEvtId);
                }
                evt = evtEntry(entry->initEvent, 0, 0);
                evt->wNpcEventType = 0;
                evt->ownerNPC = entry;
                entry->initEvtId = evt->eventId;
            } else {
                entry->initEvtId = 0;
            }
        }
    }
}

BOOL npcWaitAllInitEvtEnd(void) {
    NpcWork* wp;
    BOOL done;
    s32 i;
    s32 total;
    NpcEntry* entry;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    done = TRUE;
    entry = wp->entries;

    for (i = 0; i < total; i++, entry++) {
        if (entry && entry->flags & 1 && !(entry->flags & 2)) {
            if (entry->initEvtId) {
                done = FALSE;
                if (!evtCheckID(entry->initEvtId)) {
                    entry->initEvtId = 0;
                    entry->flags |= 2;
                }
            } else {
                entry->flags |= 2;
            }
        }
    }
    return done;
}

void npcExecAllReglEvt(void) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;
    EventEntry* evt;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    entry = wp->entries;

    for (i = 0; i < total; i++, entry++) {
        if (entry && entry->flags & 1 && entry->moveEvent) {
            if (entry->regularEvtId) {
                evtDeleteID(entry->regularEvtId);
            }
            evt = evtEntry(entry->moveEvent, 0, EVENT_START_IMMEDIATE);
            evt->wNpcEventType = 1;
            evt->ownerNPC = entry;
            entry->regularEvtId = evt->eventId;
        }
    }
}

USER_FUNC(_check) {
    EvtWord* args = event->args;
    s32 index = *args++;

    if (pouchGetPtr()->mMerleeCurseUsesRemaining) {
        evtSetValue(event, index, 0);
    } else {
        evtSetValue(event, index, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_majinai_effect) {
    MarioWork* mario = marioGetPtr();
    EffectEntry* effect;

    if (isFirstCall) {
        event->userdatap[0] = effMajinaiEntry(0, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y + 16.0f, mario->position.z);
        effect = event->userdatap[0];
        ((EffMajinaiWork*)effect->userdata)->scale = 0.8f;
        effSetName(event->userdatap[0], "mjef_coinup");
    }
    if (event->userdata[0] && effNameToPtr("mjef_coinup")) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_majinai_effect_se_fld)
    CALL(btlevtcmd_ftomsec, 29, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FLD_INCANT_CUBE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 200, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FLD_INCANT_CUBE2"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 90, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FLD_INCANT_CUBE3"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 80, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FLD_INCANT_CUBE4"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 5, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FLD_INCANT_CUBE5"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_majinai_effect_se_btl)
    CALL(btlevtcmd_ftomsec, 29, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_BTL_INCANT_CUBE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 200, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_BTL_INCANT_CUBE2"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 90, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_BTL_INCANT_CUBE3"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 80, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_BTL_INCANT_CUBE4"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_ftomsec, 5, LW(10))
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_BTL_INCANT_CUBE5"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(_evt_majinai_coin_up_effect)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SPAWN_EVENT(evt_majinai_effect_se_fld)
    CALL(_majinai_effect)
    CALL(evt_msg_print, 0, PTR("msg_mj_coin_up"), 0, 0)
    CALL(_check, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, PTR("msg_mj_last"), 0, 0)
    END_IF
    RETURN
EVT_END()

void npcGetBackItemEntry(NpcEntry* npc) {
    NpcBattleInfo* info = &npc->battleInfo;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (info->recoveredItems[i]) {
#ifdef VERSION_G8ME01
            ItemEntry* item = itemEntry(NULL, info->recoveredItems[i], 11, -1, 0, npc->position.x,
                                          npc->position.y, npc->position.z);
            if (item) {
                itemFlagOn(item, 0x100);
            }
#else
            itemFlagOn(itemEntry(NULL, info->recoveredItems[i], 11, -1, 0, npc->position.x,
                                 npc->position.y, npc->position.z),
                       0x100);
#endif
            info->recoveredItems[i] = 0;
        }
    }
}

void npcClearDeadInfo(void) {
    memset(gp->fbatData->deadInfo, 0, sizeof(gp->fbatData->deadInfo));
}

void npcStrippedKoopaPool(f32* f, const char** p) {
    f[0] = 60.0f;
    f[1] = 365.0f;
    p[0] = "kpa_03";
    p[1] = "SFX_KUPPA_3RD_WATER_FALL1";
}

void npcKoopaModeMain(void) {
    s32 jump;
    u8* fb2;
    s32 fire;
    MarioWork* mario;
    fbatEncountCheck();
    fb2 = (u8*)gp->fbatData;
    mario = marioGetPtr();
    jump = 0;
    fire = jump;
    if (mario->characterId == CHARACTER_BOWSER) jump = kpaJumpChk();
    if (mario->characterId == CHARACTER_BOWSER) fire = kpaFireAttackCheck();
    if (jump) {
        if (*(s32*)(fb2 + 0x57c) > 0 && *(s32*)(fb2 + 0x578) >= 2) {
            Vec pos = mario->position;
            pos.y += 60.0f;
            kpaAddScorePos(*(s32*)(fb2 + 0x57c), &pos);
        }
        *(s32*)(fb2 + 0x578) = 0;
        *(s32*)(fb2 + 0x57c) = 0;
    }
    if (!fire) {
        *(s32*)(fb2 + 0x574) = 0;
    }
}

void npcKoopaModeEncountCheck(void) {
    NpcWork* wp;
    FieldBattleData* data;
    s32 i;
    s32 total;
    NpcEntry* entry;

    data = gp->fbatData;
    wp = npcGetWorkPtrInline();
    total = wp->total;

    if (data->mode == 1) {
        entry = wp->entries;
        for (i = 0; i < total; i++, entry++) {
            if (entry && entry->flags & 1 && entry->flags & 0x8000000 && !(entry->jumpFlags & 4)) {
                switch (kpaEnemyHitChk(entry->position.x, entry->position.y, entry->position.z, entry->height,
                                       entry->width)) {
                    case 1:
                        if (!(entry->hazardFlags & 1)) {
                            npcGroupDead(entry, 1);
                        }
                        break;
                    case 2:
                        if (!(entry->hazardFlags & 2)) {
                            npcGroupDead(entry, 2);
                        }
                        break;
                    case 3:
                        if (!(entry->hazardFlags & 4)) {
                            npcGroupDead(entry, 4);
                        }
                        break;
                    case 4:
                        kpaPowDown();
                        break;
                    default:
                        if (!(entry->hazardFlags & 0x10) && entry->groundHit &&
                            !strcmp(hitGetName(entry->groundHit), "a_magu")) {
                            npcGroupDead(entry, 0x10);
                        }
                        break;
                }
            }
        }
    }
}

s32 npcKoopaModeMobjBoundDeadCheck(HitEntry* hit) {
    NpcWork* wp;
    s32 i;
    s32 total;
    NpcEntry* entry;
    s32 count;

    wp = npcGetWorkPtrInline();
    total = wp->total;
    count = 0;

    if (hit == NULL) {
        return 0;
    }
    entry = wp->entries;
    for (i = 0; i < total; i++, entry++) {
        if (entry && entry->flags & 1 && entry->flags & 0x8000000 && !(entry->jumpFlags & 4) &&
            !(entry->hazardFlags & 8) && entry->groundHit == hit) {
            npcGroupDead(entry, 8);
            count++;
        }
    }
    return count;
}

void fbatSandersBombTriggerOn(void) {
    gp->fbatData->sandersBombTrigger = TRUE;
}

s32 npcCalcScore(NpcEntry* npc) {
    FieldBattleData* data = gp->fbatData;
    s32 score = 300;
    Vec pos = npc->position;

    pos.y += npc->tribe->height;
    if (npc->koopaEncounterType == 0x10) {
        score = 0;
    } else {
        if (npc->koopaEncounterType == 8) {
            score = 400;
        }
        if (kpaMutekiCheck()) {
            score += 100;
        }
        if (npc->koopaEncounterType == 2) {
            data->kpaComboCount++;
            score *= data->kpaComboCount;
        }
        if (npc->koopaEncounterType == 1) {
            data->kpaJumpKills++;
            data->kpaJumpScore += score;
        }
        if (score > 9999) {
            score = 10000;
        }
    }
    if (score > 0) {
        kpaAddScorePos(score, &pos);
    }
    return score;
}

BOOL npcCheckBlow(NpcEntry* npc) {
    Vec pos;

    if (npc->flags & 0x20000000) {
        return FALSE;
    }
    if (npc->flags & 0x20) {
        return FALSE;
    }
    pos = npc->position;
    if (cloudGetBreathPower(pos, npc->width) > 0.0f) {
        return TRUE;
    }
    pos = npc->position;
    pos.y -= npc->height / 2.0f;
    if (cloudGetBreathPower(pos, npc->width) > 0.0f) {
        return TRUE;
    }
    pos = npc->position;
    pos.y -= npc->height;
    if (cloudGetBreathPower(pos, npc->width) > 0.0f) {
        return TRUE;
    }
    return FALSE;
}

BOOL N_fbatPreventMarioEventChk(void) {
    s32 mode = gp->fbatData->mode;
    if (mode != 0 && mode != 1 && mode != 8) {
        return TRUE;
    }
    return FALSE;
}

// forward declarations: these functions are defined further down
BOOL N_fbatPreventMarioEventChk(void);
BOOL npcCheckBlow(NpcEntry* npc);
s32 npcCalcScore(NpcEntry* npc);
void fbatSandersBombTriggerOn(void);
s32 npcKoopaModeMobjBoundDeadCheck(HitEntry* hit);
void npcKoopaModeEncountCheck(void);
void npcClearDeadInfo(void);
void npcGetBackItemEntry(NpcEntry* npc);
s32 _majinai_effect(EventEntry* event, BOOL isFirstCall);
s32 _check(EventEntry* event, BOOL isFirstCall);
void npcExecAllReglEvt(void);
BOOL npcWaitAllInitEvtEnd(void);
void npcExecAllInitEvt(void);
NpcTerritoryType dbGetDefData(NpcTerritoryInfo* info, const char* description);
void npcSetLink(NpcEntry* npc, NpcEntry* other);
void npcSetSlave(NpcEntry* npc, NpcEntry* slave, s32 id);
void npcSetBtlSetupWork(NpcEntry* npc, s32 index, s32 value);
s32 npcGetBtlSetupWork(NpcEntry* npc, s32 index);
void npcBlurOff(const char* name);
void npcBlurOn(const char* name);
void npcSetColor(const char* name, GXColor color);
void npcStartForOneEvent(NpcEntry* npc);
void npcStartForEvent(void);
void npcStopForOneEvent(NpcEntry* npc);
void npcStopForEvent(void);
void npcSetBattleInfo(NpcEntry* npc, s32 battleInfoId);
void npcSetupBattleInfo(NpcEntry* npc, NpcSetupBattleInfo* setup);
NpcEntry* _npcHitCheckHammerAllMotion(NpcEntry* npc, f32* dist, f32 r, f32 angle);
NpcEntry* _npcHitCheckSphere(NpcEntry* npc, f32* dist, f32 x, f32 y, f32 z, f32 r);
void* fbatHitCheck(u32 flags, void* result);
NpcEntry* fbatHitCheckAll(void);
NpcEntry* fbatNpcTalkCheck(void);
void npcGroupDead(NpcEntry* entry, u16 reason);
void _npcGroupDead(NpcEntry* entry, u16 reason);
void _npcDead(NpcEntry* npc, u16 reason);
void fbatBattleMode(void);
void fbatTalkMode(void);
void fbatEncountCheck(void);
void _fbatFirstAttackAnnouceDisp(CameraId camId, void* param);
void fbatSetAttackAnnounceEnable(void);
void fbatSetAttackAnnounce(s32 hitKind);
void fbatChangeMode(s16 mode);
FieldBattleData* fbatGetPointer(void);
NpcEntry* npcNearDistCheck(f32 x, f32 y, f32 z, f32 range);
HitEntry* npcHitCheckSide(NpcEntry* npc, f32* outX, f32* outZ, f32* angle, f32 dx, f32 dz);
f32 npcTransRytoFaceDir(NpcEntry* npc);
void npcTuningRy(NpcEntry* npc, f32 angle);
void npcMoveG(NpcEntry* npc);
BOOL npcCheckHitMarioSide(NpcEntry* npc);
void npcSetStayPose(char* name);
void npcSetTalkPose(char* name);
void npcSetMarioAutoTalkPose(const char* stayPose, const char* talkPose);
NpcEntry* npcNameToPtr_NoAssert(const char* name);
NpcEntry* npcNameToPtr(const char* name);
void npcMain(void);
void npcDeleteGroup(NpcEntry* entry);
void _npcDeleteGroup(NpcEntry* entry);
void npcDelete(NpcEntry* entry);
NpcTribeInfo* npcGetTribe(const char* tribeName);
s32 npcEntry(const char* a1, const char* animName);
s32 npcGetReactionOfLivingBody(BOOL inBattle);
void npcReset(BOOL inBattle);
void npcInit(void);
void npcDisp_blur(CameraId camId, void* param);
void npcDisp_offscreen(CameraId camId, void* param);
void npcDisp_offscreen_xlu(CameraId camId, void* param);
void npcDisp(CameraId camId, void* param);
void npcDisp_xlu(CameraId cameraId, void* param);
void mtx_setup(NpcEntry* entry, Mtx mtx, s32 history);
void npcRecoveryFieldNpc(void);
void npcReleaseFieldNpc(void);
NpcWork* npcGetWorkPtr(void);
