#include "battle/battle_stage.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"




#define STG ((BattleStage*)BattleStageGetPtr()) // FAKE: no-op cast; without it US BattleStageEnd swaps the registers of s and i (r28/r29) around the batSpotEnd call

typedef struct ScreenWork {
    void* tex;      // 0x0
    s32 captured;   // 0x4
    f32 field_8;    // 0x8
    f32 field_C;    // 0xC
    f32 field_10;   // 0x10
    f32 field_14;   // 0x14
    f32 field_18;   // 0x18
    f32 field_1C;   // 0x1C
    s32 field_20;   // 0x20
    s32 field_24;   // 0x24
    f32 field_28;   // 0x28
} ScreenWork;

//.sbss
static BattleSpotWork work;
static ScreenWork* screen_wp;

//.sdata
static BattleSpotWork* wp = &work;

// US: battle stage memory goes through BattleAlloc/BattleFree
#ifdef VERSION_G8ME01
#define STG_ALLOC(size) BattleAlloc(size)
#define STG_FREE(ptr) BattleFree(ptr)
void batSpotEnd(void);
#else
#define STG_ALLOC(size) __memAlloc(HEAP_BATTLE, size)
#define STG_FREE(ptr) __memFree(HEAP_BATTLE, ptr)
#endif

static u8 inta_tex[32] ATTRIBUTE_ALIGN(32) = {0xFF,0xFF,0xFF,0xFF,0,0,0,0,0xFF,0xFF,0xFF,0xFF,0,0,0,0,0xFF,0xFF,0xFF,0xFF,0,0,0,0,0xFF,0xFF,0xFF,0xFF,0,0,0,0};

USER_FUNC(screen_evt_func);

EVT_BEGIN(screen_evt)
    BEGIN_INLINE_EVENT
        CALL(screen_evt_func, LW(0))
    END_INLINE_EVENT
    RETURN
EVT_END()


// forward declarations
void batSpotDisp(CameraId camId, void* param);
BattleSpotEntry* batSpotEntry(void);
void BattleStageDispDark(CameraId camId, void* param);
void BattleStageDispLight(CameraId camId, void* param);
void DarkByPoint(u8 alpha, f32 x, f32 y, f32 z);
void DarkBySquare(f32 x0, f32 y0, f32 x1, f32 y1, f32 z, u8 alpha);
void BattleStageSetDark(StageDark* d, u8 v, s32 time, u8 mode);
void BattleStageSetDarkBase(StageDark* d, u8 v, s32 time, u8 mode);
void batSpotInit(void);
f32 getSpline(f32* a, f32* b, f32* c, f32* d, s32* t, s32 n, s32 x);
void batSpotMain();
void drawCone(CameraId camId, BattleSpotEntry* e);

BattleStage* BattleStageGetPtr(void) {
    return &_battleWorkPointer->stage;
}

void BattleStageInit(void) {
    BattleStage* s = STG;
    s32 i;

    memset(s, 0, sizeof(BattleStage));
    s->reserved_00 = 0;
    for (i = 0; i < 12; i++) {
        StageLight* l = &s->lights[i];
        l->flags = 0;
        l->data = NULL;
    }
    s->stgDark.flags = 1;
    s->stgDark.cur = s->stgDark.base = 0;
    s->stgDark.start = 0;
    s->stgDark.target = 0;
    s->stgDark.time = 0;
    s->stgDark.max = 0;
    s->audDark.flags = 1;
    s->audDark.cur = s->audDark.base = 0x8C;
    s->audDark.start = 0;
    s->audDark.target = 0;
    s->audDark.time = 0;
    s->audDark.max = 0;
    s->stageSizeX = 290.0f;
    s->stageSizeY = 200.0f;
    s->stageSizeZ = 90.0f;
    s->audienceSizeX = s->stageSizeX;
    s->audienceSizeY = -50.0f;
    s->audienceSizeZ = 200.0f;
    BattleStageObjectInit();
    batSpotInit();
}

typedef struct LightSpline {
    s32 n;          // 0x0
    s32 t[8];       // 0x4
    f32 x[4][8];    // 0x24
    f32 y[4][8];    // 0xA4
    f32 z[4][8];    // 0x124
} LightSpline;

void BattleStageMain(void) {
    StageLight* l;
    s32 i;
    LightSpline* sp;
    BattleStage* s = STG;

    batSpotMain();
    for (i = 0; i < 12; i++) {
        l = &s->lights[i];
        if (l->flags & 1) {
            if (l->colorTime > 0) {
                l->colorTime--;
                l->color.r = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->colorMode, l->colorStart.r, l->colorTarget.r, l->colorMax - l->colorTime, l->colorMax);
                l->color.g = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->colorMode, l->colorStart.g, l->colorTarget.g, l->colorMax - l->colorTime, l->colorMax);
                l->color.b = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->colorMode, l->colorStart.b, l->colorTarget.b, l->colorMax - l->colorTime, l->colorMax);
                if ((l->flags & 2) && l->color.r == 0 && l->color.g == 0 && l->color.b == 0) {
                    l->flags = 0;
                    continue;
                }
            }
            if (l->widthTime > 0) {
                l->widthTime--;
                l->width = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->widthMode, l->widthStart, l->widthTarget, l->widthMax - l->widthTime, l->widthMax);
            }
            if (l->posTime > 0) {
                l->posTime--;
                l->pos.x = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->posMode, l->posStart.x, l->posTarget.x, l->posMax - l->posTime, l->posMax);
                l->pos.y = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->posMode, l->posStart.y, l->posTarget.y, l->posMax - l->posTime, l->posMax);
                l->pos.z = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->posMode, l->posStart.z, l->posTarget.z, l->posMax - l->posTime, l->posMax);
            }
            if (l->flags & 4) {
                sp = l->data;
                l->target.x = getSpline(sp->x[0], sp->x[1], sp->x[2], sp->x[3], sp->t, sp->n, l->targetTime);
                l->target.y = getSpline(sp->y[0], sp->y[1], sp->y[2], sp->y[3], sp->t, sp->n, l->targetTime);
                l->target.z = getSpline(sp->z[0], sp->z[1], sp->z[2], sp->z[3], sp->t, sp->n, l->targetTime);
                l->targetTime++;
                if (l->targetTime >= sp->t[sp->n - 1]) {
                    if (l->flags & 8) {
                        l->targetTime -= sp->t[sp->n - 1];
                    } else {
                        l->targetTime = sp->t[sp->n - 1];
                    }
                }
            } else {
                if (l->targetTime > 0) {
                    l->targetTime--;
                    l->target.x = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->targetMode, l->targetStart.x, l->targetTarget.x, l->targetMax - l->targetTime, l->targetMax);
                    l->target.y = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->targetMode, l->targetStart.y, l->targetTarget.y, l->targetMax - l->targetTime, l->targetMax);
                    l->target.z = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->targetMode, l->targetStart.z, l->targetTarget.z, l->targetMax - l->targetTime, l->targetMax);
                }
                if (l->data != NULL) {
                    STG_FREE(l->data);
                    l->data = NULL;
                }
            }
            if (l->upTime > 0) {
                l->upTime--;
                l->up.x = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->upMode, l->upStart.x, l->upTarget.x, l->upMax - l->upTime, l->upMax);
                l->up.y = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->upMode, l->upStart.y, l->upTarget.y, l->upMax - l->upTime, l->upMax);
                l->up.z = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(l->upMode, l->upStart.z, l->upTarget.z, l->upMax - l->upTime, l->upMax);
            }
        } else {
            if (l->data != NULL) {
                STG_FREE(l->data);
                l->data = NULL;
            }
        }
    }
    if (s->stgDark.time > 0) {
        s->stgDark.time--;
        s->stgDark.cur = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(s->stgDark.mode, s->stgDark.start, s->stgDark.target, s->stgDark.max - s->stgDark.time, s->stgDark.max);
    }
    if (s->stgDark.baseTime > 0) {
        s->stgDark.baseTime--;
        s->stgDark.base = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(s->stgDark.baseMode, s->stgDark.baseStart, s->stgDark.baseTarget, s->stgDark.baseMax - s->stgDark.baseTime, s->stgDark.baseMax);
    }
    if (s->audDark.time > 0) {
        s->audDark.time--;
        s->audDark.cur = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(s->audDark.mode, s->audDark.start, s->audDark.target, s->audDark.max - s->audDark.time, s->audDark.max);
    }
    if (s->audDark.baseTime > 0) {
        s->audDark.baseTime--;
        s->audDark.base = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(s->audDark.baseMode, s->audDark.baseStart, s->audDark.baseTarget, s->audDark.baseMax - s->audDark.baseTime, s->audDark.baseMax);
    }
}

void BattleStageEnd(void) {
    s32 i;
    BattleStage* s = STG;

#ifdef VERSION_G8ME01
    batSpotEnd();
#endif
    for (i = 0; i < 12; i++) {
        StageLight* l = &s->lights[i];
        if (l->data != NULL) {
            STG_FREE(l->data);
            l->data = NULL;
        }
    }
}

void BattleStageDisp(void) {
    BattleGetUnitPtr(_battleWorkPointer, _battleWorkPointer->activeUnitId);
    dispEntry(CAMERA_3D, 5000.0f, 2, BattleStageDispDark, NULL);
    dispEntry(CAMERA_3D, 6000.0f, 2, BattleStageDispLight, NULL);
}

#pragma dont_inline on
void DarkByPoint(u8 alpha, f32 x, f32 y, f32 z) {
    GXPosition3f32(x, y, z);
    GXColor4u8(0, 0, 0, alpha);
}
#pragma dont_inline reset

#pragma dont_inline on
void DarkBySquare(f32 x0, f32 y0, f32 x1, f32 y1, f32 z, u8 alpha) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    DarkByPoint(alpha, x0, y0, z);
    DarkByPoint(alpha, x1, y0, z);
    DarkByPoint(alpha, x1, y1, z);
    DarkByPoint(alpha, x0, y1, z);
}
#pragma dont_inline reset

void BattleStageDispDark(CameraId camId, void* param) {
    BattleStage* s = STG;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    StageDark* aud;

    GXSetCullMode(GX_CULL_BACK);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    aud = &s->audDark;
    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
    DarkBySquare(-640.0f, 640.0f, 640.0f, -640.0f, 105.0f, aud->cur);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    DarkBySquare(640.0f, 640.0f, -640.0f, -640.0f, 105.0f, aud->cur);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    DarkBySquare(-640.0f, 640.0f, 640.0f, -640.0f, 105.0f, s->stgDark.cur);
}

// FAKE: copy propagation off and the 2.0f held in a named local `two`, so the constant numbers below `width`
// (a propagated constant load is an unnamed temp numbered above every named local); `s` is taken directly
// rather than through STG because with propagation off the inline's return temp would take its place.
#pragma push
#pragma opt_propagation off
void BattleStageDispLight(CameraId camId, void* param) {
    s32 i, j;
    f32 width;
    BattleStage* s = &_battleWorkPointer->stage;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 two;

    GXSetCullMode(GX_CULL_BACK);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_ONE, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    for (i = 0; i < 12; i++) {
        if (s->lights[i].flags & 1) {
            Mtx m;
            Vec v;
            Vec axis;

            axis = (Vec){s->lights[i].target.x - s->lights[i].pos.x, s->lights[i].target.y - s->lights[i].pos.y, s->lights[i].target.z - s->lights[i].pos.z};
            width = s->lights[i].width;
            v = s->lights[i].up;
            two = 2.0f;
            for (j = 0; j < 18; j++) {
                f32 px = s->lights[i].pos.x;
                f32 py = s->lights[i].pos.y;
                f32 pz = s->lights[i].pos.z;
                f32 ex = (s->lights[i].target.x - px) * two + px;
                f32 ey = (s->lights[i].target.y - py) * two + py;
                f32 ez = (s->lights[i].target.z - pz) * two + pz;
                f32 ax = v.x * width + ex;
                f32 ay = v.y * width + ey;
                f32 az = v.z * width + ez;
                f32 bx, by, bz;
                u8 r, g, b;

                PSMTXRotAxisRad(m, &axis, 0.34906584f);
                PSMTXMultVec(m, &v, &v);
                PSVECNormalize(&v, &v);
                bx = v.x * width + ex;
                by = v.y * width + ey;
                bz = v.z * width + ez;
                r = s->lights[i].color.r;
                g = s->lights[i].color.g;
                b = s->lights[i].color.b;
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
                GXPosition3f32(px, py, pz);
                GXColor4u8(r, g, b, 0);
                GXPosition3f32(ax, ay, az);
                GXColor4u8(r, g, b, 0);
                GXPosition3f32(bx, by, bz);
                GXColor4u8(r, g, b, 0);
            }
        }
    }
}
#pragma pop

void BattleStageOnLightInCommand(void) {
}

void BattleStageOffLightInCommand(s32 a, s32 b) {
}

void BattleStageOnLightFriendInCommand(void) {
}

void BattleStageOffLightFriendInCommand(void) {
}

void BattleStageOnLightEnemyInCommand(void) {
}

void BattleStageOffLightEnemyInCommand(void) {
}

s32 BattleStageGetLightNumberByName(const char* name) {
    BattleStage* s = STG;
    s32 i;

    for (i = 0; i < 12; i++) {
        StageLight* l = &s->lights[i];
        if ((l->flags & 1) && !strcmp(l->name, name)) {
            return i;
        }
    }
    return -1;
}

s32 BattleStageSysLightEntry(const char* name) {
    BattleStage* s = STG;
    s32 i;

    for (i = 0; i <= 6; i++) {
        StageLight* l = &s->lights[i];
        if ((l->flags & 1) && !strcmp(l->name, name)) {
        }
    }
    for (i = 0; i < 7; i++) {
        StageLight* l = &s->lights[i];
        if (!(l->flags & 1)) {
            l->flags = 1;
            strcpy(l->name, name);
            break;
        }
    }
    return i;
}

void BattleStageLightRelease(s32 idx, s32 time, u8 mode) {
    // FAKE: l first holds the stage pointer (two defs), so with propagation on the stage base and idx * 232 + 4 stay separate adds as in the target; this replaces the old opt_propagation off pragma, under which the {0,0,0} template also left a dead stack store
    StageLight* l = (StageLight*)STG;

    l = ((BattleStage*)l)->lights + idx;

    if (l->flags & 1) {
        if (time == 0) {
            l->flags = 0;
        } else {
            l->flags |= 2;
            l->colorStart = (GXColor){l->color.r, l->color.g, l->color.b};
            { GXColor z = {0, 0, 0}; l->colorTarget = z; }
            l->colorTime = time;
            l->colorMax = time;
            l->colorMode = mode;
        }
    }
}

#pragma opt_propagation off
void BattleStageLightSetLightColor(s32 idx, u8 r, u8 g, u8 b, s32 time, u8 mode) {
    StageLight* l = &STG->lights[idx];

    l->colorStart = (GXColor){l->color.r, l->color.g, l->color.b};
    l->colorTarget = (GXColor){r, g, b};
    l->colorTime = time;
    l->colorMax = time;
    l->colorMode = mode;
    if (time == 0) {
        l->color = (GXColor){l->color.r, l->color.g, l->color.b, 0xFF};
    }
}
#pragma opt_propagation reset

#pragma opt_propagation off
void BattleStageLightSetWidth(s32 idx, s32 width, s32 time, u8 mode) {
    StageLight* l = &STG->lights[idx];

    l->widthStart = l->width;
    l->widthTarget = width;
    l->widthTime = time;
    l->widthMax = time;
    l->widthMode = mode;
    if (time == 0) {
        l->width = width;
    }
}
#pragma opt_propagation reset

#pragma opt_propagation off
void BattleStageLightSetPosition(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode) {
    StageLight* l = &STG->lights[idx];

    l->posStart.x = l->pos.x;
    l->posStart.y = l->pos.y;
    l->posStart.z = l->pos.z;
    l->posTarget.x = x;
    l->posTarget.y = y;
    l->posTarget.z = z;
    l->posTime = time;
    l->posMax = time;
    l->posMode = mode;
    if (time == 0) {
        l->pos = (Vec){x, y, z};
    }
}
#pragma opt_propagation reset

#pragma dont_inline on
#pragma opt_propagation off
void BattleStageLightSetTarget(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode) {
    StageLight* l = &STG->lights[idx];

    l->targetStart.x = l->target.x;
    l->targetStart.y = l->target.y;
    l->targetStart.z = l->target.z;
    l->targetTarget.x = x;
    l->targetTarget.y = y;
    l->targetTarget.z = z;
    l->targetTime = time;
    l->targetMax = time;
    l->targetMode = mode;
    l->flags &= ~4;
    if (time == 0) {
        l->target = (Vec){x, y, z};
    }
}
#pragma opt_propagation reset
#pragma dont_inline reset

void BattleStageLightSetTargetByUnit(s32 idx, s32 unitId, f32 x, f32 y, f32 z, s32 time, u8 mode) {
    f32 px, py, pz;
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    BtlUnit_GetPos(unit, &px, &py, &pz);
    px += x;
    py += y;
    pz += z;
    BattleStageLightSetTarget(idx, px, py, pz, time, mode);
}

#pragma opt_propagation off
void BattleStageLightSetUpVec(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode) {
    StageLight* l = &STG->lights[idx];

    l->upStart.x = l->up.x;
    l->upStart.y = l->up.y;
    l->upStart.z = l->up.z;
    l->upTarget.x = x;
    l->upTarget.y = y;
    l->upTarget.z = z;
    l->upTime = time;
    l->upMax = time;
    l->upMode = mode;
    if (time == 0) {
        l->up = (Vec){x, y, z};
    }
}
#pragma opt_propagation reset

#pragma dont_inline on
void BattleStageSetDark(StageDark* d, u8 v, s32 time, u8 mode) {
    d->start = d->cur;
    d->target = v;
    d->time = time;
    d->max = time;
    d->mode = mode;
    if (time == 0) {
        d->cur = v;
    }
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleStageSetDarkBase(StageDark* d, u8 v, s32 time, u8 mode) {
    d->baseStart = d->base;
    d->baseTarget = v;
    d->baseTime = time;
    d->baseMax = time;
    d->baseMode = mode;
    if (time == 0) {
        d->base = v;
    }
}
#pragma dont_inline reset

void BattleStageSetStgDark(u8 v, s32 time, u8 mode) {
    BattleStageSetDark(&STG->stgDark, v, time, mode);
}

void BattleStageSetStgDarkBase(u8 v, s32 time, u8 mode) {
    BattleStageSetDarkBase(&STG->stgDark, v, time, mode);
}

void BattleStageReturnStgDarkBase(s32 time, u8 mode) {
    StageDark* d = &STG->stgDark;
    BattleStageSetDark(d, d->base, time, mode);
}

void BattleStageSetAudDark(u8 v, s32 time, u8 mode) {
    BattleStageSetDark(&STG->audDark, v, time, mode);
}

void BattleStageSetAudDarkBase(u8 v, s32 time, u8 mode) {
    BattleStageSetDarkBase(&STG->audDark, v, time, mode);
}

void BattleStageReturnAudDarkBase(s32 time, u8 mode) {
    StageDark* d = &STG->audDark;
    BattleStageSetDark(d, d->base, time, mode);
}

#pragma dont_inline on
f32 getSpline(f32* a, f32* b, f32* c, f32* d, s32* t, s32 n, s32 x) {
    s32 i;
    f32 f;

    for (i = n - 1; i >= 0; i--) {
        if ((f32)(x - t[i]) >= 0.0f) {
            break;
        }
    }
    f = x - t[i];
    return f * (f * (f * d[i] + c[i]) + b[i]) + a[i];
}
#pragma dont_inline reset
BattleSpotWork* BatSpotGetPtr(void) {
    return wp;
}

#pragma dont_inline on
void batSpotInit(void) {
    int i;
    BattleSpotEntry* entry;

    wp->count = 5;
    wp->entries = STG_ALLOC(sizeof(BattleSpotEntry) * wp->count);
    memset(wp->entries, 0, sizeof(BattleSpotEntry) * wp->count);

    for (i = 0; i < 5; i++) {
        entry = batSpotEntry();
        entry->pos = (Vec){50.0f * i + -100.0f, 0.0f, -50.0f};
        entry->rot = (Vec){0.0f, 0.0f, 0.0f};
        entry->scale = (Vec){20.0f, 20.0f, 150.0f};
        entry->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    }
}
#pragma dont_inline reset

#pragma dont_inline on
void batSpotMain() {
    s32 count = wp->count;

    while (count-- > 0) {}
    if (wp->skipRender > 0) {
        dispEntry(CAMERA_3D_IMAGE, -1000.0f, 0, batSpotDisp, NULL);
    }
}
#pragma dont_inline reset

#ifdef VERSION_G8ME01
// US: frees the spot entries at battle end
#pragma dont_inline on
void batSpotEnd(void) {
    BattleFree(wp->entries);
}
#pragma dont_inline reset
#endif

void batSpotDisp(CameraId camId, void* param) {
    int i;
    BattleSpotEntry* e;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    for (i = 0, e = wp->entries; i < wp->count; i++, e++) {
        if ((e->flags & 1) && (e->flags & 2)) {
            GXSetCullMode(GX_CULL_BACK);
            GXSetAlphaUpdate(GX_TRUE);
            GXSetColorUpdate(GX_FALSE);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            drawCone(camId, e);
            GXSetCullMode(GX_CULL_FRONT);
            GXSetAlphaUpdate(GX_TRUE);
            GXSetColorUpdate(GX_FALSE);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0});
            drawCone(camId, e);
            GXSetCullMode(GX_CULL_BACK);
            GXSetAlphaUpdate(GX_FALSE);
            GXSetColorUpdate(GX_TRUE);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_CLEAR);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanMatColor(GX_COLOR0A0, e->color);
            drawCone(camId, e);
        }
    }
}

#pragma dont_inline on
void drawCone(CameraId camId, BattleSpotEntry* e) {
    Mtx trans, scale, m, rx, ry, rz;
    CameraEntry* cam;
    s32 i;
    f32 a, s, c;

    cam = camGetPtr(camId);
    PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
    PSMTXScale(scale, e->scale.x, e->scale.y, e->scale.z);
    PSMTXRotRad(rx, 'x', 0.017453292f * e->rot.x);
    PSMTXRotRad(ry, 'y', 0.017453292f * e->rot.y);
    PSMTXRotRad(rz, 'z', 0.017453292f * e->rot.z);
    PSMTXConcat(rz, ry, ry);
    PSMTXConcat(ry, rx, m);
    PSMTXConcat(cam->view, trans, trans);
    PSMTXConcat(trans, m, m);
    PSMTXConcat(m, scale, m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 32);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXColor4u8(0, 0, 0, e->color.a);
    for (i = 0; i < 31; i++) {
        if (i == 30) {
            a = 0.0f;
        } else {
            a = 6.2831855f * (f32)-i / 30.0f;
        }
        s = (f32)sin(a);
        c = (f32)cos(a);
        GXPosition3f32(c, s, -1.0f);
        GXColor4u8(0, 0, 0, e->color.a / 3);
    }
}
#pragma dont_inline reset

#pragma dont_inline on
BattleSpotEntry* batSpotEntry(void) {
    int i;
    BattleSpotEntry* entry = wp->entries;

    for (i = 0; i < wp->count; i++, entry++) {
        if (!(entry->flags & 1)) {
            break;
        }
    }

    entry->pos = (Vec){0.0, 0.0, 0.0};
    entry->scale = (Vec){1.0, 1.0, 1.0};
    entry->rot = (Vec){0.0, 0.0, 0.0};
    entry->color = (GXColor){0, 0, 0, 0};
    entry->flags |= 1;

    return entry;
}
#pragma dont_inline reset

#pragma dont_inline on
static f32 randf(f32 min, f32 max) {
    f32 r = (f32)rand() / 32767.0f;
    f32 d = max - min;
    return r * d + min;
}
#pragma dont_inline reset

static void screen_capture(CameraId camId, void* param) {
    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(304, 240, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(screen_wp->tex, GX_FALSE);
    GXPixModeSync();
    screen_wp->captured = 1;
}

USER_FUNC(screen_evt_func) {
    dispEntry(CAMERA_2D, 1000.0f, 0, screen_capture, NULL);
    return 0;
}

USER_FUNC(tou_gamen_screen_tev_init) {
    u32 size = GXGetTexBufferSize(608, 480, GX_TF_I4, GX_FALSE, 0);

    screen_wp = _mapAlloc(sizeof(ScreenWork));
    screen_wp->tex = _mapAlloc(size);
    screen_wp->captured = 0;
    screen_wp->field_8 = screen_wp->field_C = 1.0f;
    screen_wp->field_10 = screen_wp->field_14 = 0.0f;
    screen_wp->field_18 = screen_wp->field_1C = 0.0f;
    screen_wp->field_20 = 0;
    screen_wp->field_28 = -1.0f;
    if (evtGetValue(event, GSW_STORY_PROGRESS) == 163) {
        screen_wp->field_24 = 1;
    }
    evtEntry(screen_evt, 0, 0);
    return 2;
}

void tou_gamen_screen_tev(MapTevCallbackInfo* p) {
    GXTexObj obj;
    Mtx m;
    Mtx m1;
    Mtx m2;
    u8* tex;
    s32 i, j, l, n, b, w, s, k, idx, o;
    u8 c;
    f32 lim;
    f32 x, y;
    f32* camf;

    if (screen_wp->captured == 0) {
        return;
    }
    if (screen_wp->field_24 != 0) {
        tex = screen_wp->tex;
        for (i = 0; i < 64; i++) {
            for (j = 0; j < 64; j++) {
                b = i / 4; k = i % 4; idx = j / 8; o = j % 8; tex[((idx << 5) + (b << 8)) + (o + k * 8)] = (i & 1) ? 0xFF : 0;
            }
            n = rand() % 100 + 100;
            for (l = 0; l < n; l++) {
                w = rand() % 4 + 2;
                s = rand() % (65 - w);
                c = rand() % 256;
                for (j = s; j < s + w; j++) {
                    b = i / 4; k = i % 4; idx = j / 8; o = j % 8; tex[((idx << 5) + (b << 8)) + (o + k * 8)] = c;
                }
            }
        }
        DCFlushRange(tex, 0x1000);
        GXInitTexObj(&obj, tex, 64, 64, GX_TF_I8, GX_REPEAT, GX_REPEAT, GX_FALSE);
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&obj, GX_TEXMAP0);
    } else {
        GXInitTexObj(&obj, screen_wp->tex, 304, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXInitTexObj(&obj, inta_tex, 4, 8, GX_TF_I4, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    if (screen_wp->field_24 == 0) {
        screen_wp->field_20++;
        if (screen_wp->field_20 % 200 == 0) {
            switch (rand() % 3) {
                case 0:
                    screen_wp->field_14 = randf(0.0f, (1.0f - screen_wp->field_8) / screen_wp->field_8);
                    screen_wp->field_1C = randf(0.0f, (1.0f - screen_wp->field_8) / screen_wp->field_8);
                    break;
                case 1:
                    screen_wp->field_C = randf(1.0f, 0.25f);
                    break;
                case 2:
                    break;
            }
        }
        screen_wp->field_8 += (screen_wp->field_C - screen_wp->field_8) / 60.0f;
        screen_wp->field_10 += (screen_wp->field_14 - screen_wp->field_10) / 60.0f;
        screen_wp->field_18 += (screen_wp->field_1C - screen_wp->field_18) / 60.0f;
        PSMTXScale(m1, screen_wp->field_8, screen_wp->field_8, 1.0f);
        lim = (1.0f - screen_wp->field_8) / screen_wp->field_8;
        y = screen_wp->field_18 > lim ? lim : screen_wp->field_18;
        if (screen_wp->field_10 > lim) {
            x = lim;
        } else {
            x = screen_wp->field_10;
        }
        PSMTXTrans(m2, x, y, 0.0f);
        PSMTXConcat(m1, m2, m);
    } else {
        PSMTXScale(m, 4.75f, 3.75f, 0.0f);
    }
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    camf = (f32*)camGetPtr(CAMERA_3D);
    PSMTXScale(m, 1.0f, 16.0f + 6.0f * ((100.0f - camf[5]) / 1000.0f), 1.0f);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(m, GX_TEXMTX1, GX_MTX2x4);
    p->tevStage = 2;
    p->texMap = 2;
    p->texCoord = 2;
}

