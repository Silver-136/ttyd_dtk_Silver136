#include "effect/eff_splash.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSplashGetCol(void* unused, void* entry) {
    extern void* camGetPtr(void);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void GXGetProjectionv(void* proj);
    extern void GXGetViewportv(void* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* model, void* proj, void* viewport, f32* sx, f32* sy, f32* sz);
    extern void sysWaitDrawSync(void);
    extern void GXPeekARGB(s32 x, s32 y, void* color);
    extern u32 unk_802294a4(void);
    extern f32 float_0_80427868;
    extern f32 float_608_804278cc;
    extern f32 float_480_804278d0;

    void* work = *(void**)((s32)entry + 0xC);
    void* cam = camGetPtr();
    f32 proj[7];
    f32 viewport[6];
    f32 sx;
    f32 sy;
    f32 sz;
    u32 color;
    u32 fallback;

    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject(*(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC),
              (void*)((s32)cam + 0x11C), proj, viewport, &sx, &sy, &sz);
    if (sx > float_0_80427868 && sx < float_608_804278cc &&
        sy > float_0_80427868 && sy < float_480_804278d0) {
        sysWaitDrawSync();
        GXPeekARGB((s32)sx, (s32)sy, &color);
        *(u8*)((s32)work + 0x3C) = ((u8*)&color)[1];
        *(u8*)((s32)work + 0x3D) = ((u8*)&color)[2];
        *(u8*)((s32)work + 0x3E) = ((u8*)&color)[3];
        *(u8*)((s32)work + 0x3F) = 0xFF;
    }
    *(s32*)((s32)work + 0x38) = 1;
    fallback = unk_802294a4();
    *(u8*)((s32)work + 0x3C) = ((u8*)&fallback)[0];
    *(u8*)((s32)work + 0x3D) = ((u8*)&fallback)[1];
    *(u8*)((s32)work + 0x3E) = ((u8*)&fallback)[2];
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void effSplashMain(struct EffectEntry* effect) {
    typedef struct VecSplash {
        f32 x;
        f32 y;
        f32 z;
    } VecSplash;
    typedef struct EffSplashWork {
        s32 type;
        VecSplash pos;
        f32 velX;
        f32 velZ;
        VecSplash scale;
        s32 frame;
        s32 timer;
        f32 velY;
        f32 gravity;
        s32 unused34;
        s32 state;
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } EffSplashWork;

    extern VecSplash vec3_802ff030;
    extern f32 dispCalcZ(VecSplash* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, struct EffectEntry* param, f32 priority);
    extern void effDelete(struct EffectEntry* effect);
    extern void effSplashGetCol(void* unused, void* entry);
    extern void effSplashDisp(void* unused, void* entry);
    extern s8 geso_trans_dt[];
    extern u8 geso_scale_dt[];
    extern f32 float_0_80427868;
    extern f32 float_0p95_804278d4;
    extern f32 float_96_804278d8;
    extern f32 float_0p125_804278b4;
    extern f32 float_10_80427870;
    extern f32 float_0p3_804278dc;
    extern f32 float_0p5_804278c4;
    extern f32 float_0p6_804278e0;
    extern f32 float_0p9_804278e4;
    extern f32 float_1p3_804278e8;
    extern f32 float_neg1_804278ec;

    EffSplashWork* work;
    EffSplashWork* child;
    VecSplash pos;
    VecSplash zpos;
    s32 type;
    s32 lifetime;
    s32 oldFrame;
    s32 i;
    s32 idx;
    f32 z;

    work = *(EffSplashWork**)((s32)effect + 0xC);
    pos = vec3_802ff030;
    pos.x = work->pos.x;
    pos.y = work->pos.y;
    pos.z = work->pos.z;
    zpos = pos;
    type = work->type;

    if (work->state == 0) {
        z = dispCalcZ(&zpos);
        dispEntry(4, 8, effSplashGetCol, effect, z);
        return;
    }

    switch (type) {
        case 0:
        case 1:
            lifetime = 30;
            break;
        case 2:
        case 4:
            lifetime = 60;
            break;
        case 3:
            lifetime = 71;
            break;
        default:
            lifetime = 0;
            break;
    }

    oldFrame = work->frame;
    work->frame = oldFrame + 1;
    if (work->frame > lifetime) {
        effDelete(effect);
        return;
    }

    child = work + 1;
    for (i = 1; i < *(s32*)((s32)effect + 0x8); i++, child++) {
        if (child->timer != 0) {
            child->timer--;
            continue;
        }

        child->pos.x += child->velX;
        child->pos.z += child->velZ;
        child->pos.y += child->velY;
        child->velY -= child->gravity;

        if (type == 1) {
            if (i == 1 && child->velY < float_0_80427868) {
                child->scale.x *= float_0p95_804278d4;
                child->scale.y *= float_0p95_804278d4;
                child->scale.z *= float_0p95_804278d4;
            }
        }
        else if (type == 3 && i < 7) {
            if ((u32)oldFrame < 0x3D) {
                child->scale.y = (f32)geso_scale_dt[oldFrame] / float_96_804278d8;
            }
            if ((u32)oldFrame < 0x47) {
                child->pos.x = float_10_80427870 * (f32)((s32)geso_trans_dt[oldFrame * 2] + 0x28) * float_0p125_804278b4;
                child->pos.y = float_10_80427870 * (f32)((s32)geso_trans_dt[oldFrame * 2 + 1] - 0xC) * float_0p125_804278b4;
            }

            idx = i - 1;
            switch (idx) {
                case 0:
                case 1:
                    child->pos.x *= float_0p3_804278dc;
                    child->pos.y *= float_0p5_804278c4;
                    child->scale.y *= float_0p6_804278e0;
                    break;
                case 2:
                case 3:
                    child->pos.x *= float_0p9_804278e4;
                    break;
                case 4:
                case 5:
                    child->pos.x *= float_1p3_804278e8;
                    child->pos.y *= float_0p3_804278dc;
                    child->scale.y *= float_0p6_804278e0;
                    break;
            }

            switch (idx) {
                case 1:
                case 3:
                case 5:
                    child->scale.x = float_neg1_804278ec;
                    child->pos.x *= float_neg1_804278ec;
                    break;
            }
        }
    }

    z = dispCalcZ(&zpos);
    dispEntry(4, 1, effSplashDisp, effect, z);
}

struct EffectEntry* effSplashEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    typedef struct VecSplash {
        f32 x;
        f32 y;
        f32 z;
    } VecSplash;
    typedef struct EffSplashWork {
        s32 type;
        VecSplash pos;
        f32 velX;
        f32 velZ;
        VecSplash scale;
        s32 frame;
        s32 timer;
        f32 velY;
        f32 gravity;
        s32 unused34;
        s32 state;
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } EffSplashWork;

    extern struct EffectEntry* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 max);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void* camGetPtr(s32 cameraId);
    extern void effWaterDamageN64Entry(double x, double y, double z, double scale, s32 type, s32 time);
    extern void effKemuTestEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern char str_splash_802ff050[];
    extern f32 float_0_80427868;
    extern f32 float_1_804278a0;
    extern f32 float_0p5_804278c4;
    extern f32 float_5_804278f0;
    extern f32 float_0p4_804278f4;
    extern f32 float_0p7_804278f8;
    extern f32 float_4_804278c8;
    extern f32 float_7_804278fc;
    extern f32 float_2_80427900;
    extern f32 float_0p1_804278b8;
    extern f32 float_10_80427870;
    extern f32 float_20_80427864;
    extern f32 float_0p48_80427904;
    extern f32 float_3p6_80427908;
    extern f32 float_0p8_8042790c;
    extern f32 float_0p01_80427910;
    extern f32 float_6p2832_804278b0;
    extern f32 float_360_80427914;
    extern f32 float_neg30_80427918;
    extern f32 float_30_8042791c;

    struct EffectEntry* effect;
    EffSplashWork* work;
    EffSplashWork* child;
    s32 fixedCount;
    s32 randomCount;
    s32 total;
    s32 i;
    s32 idx;
    s32 rAngle;
    s32 rSpeed;
    f32 angleScale;
    f32 angleDiv;
    f32 two;
    f32 zero;
    f32 ten;
    f32 one;
    f32 half;
    f32 angle;
    f32 speed;
    f32 s;
    f32 c;
    f32 camAngle;
    f32 camSin;
    f32 camCos;

    effect = effEntry();
    fixedCount = 0;
    randomCount = 0;

    switch (type) {
        case 0:
        case 1:
            fixedCount = 2;
            randomCount = 10;
            break;
        case 2:
            fixedCount = 1;
            randomCount = 10;
            break;
        case 3:
            fixedCount = 6;
            randomCount = 0;
            scale *= float_0p5_804278c4;
            effWaterDamageN64Entry(x, y, z, float_1_804278a0, 2, 60);
            break;
        case 4:
            fixedCount = 2;
            randomCount = 10;
            break;
    }

    *(void**)((s32)effect + 0x14) = str_splash_802ff050;
    total = fixedCount + randomCount + 1;
    *(s32*)((s32)effect + 0x8) = total;
    work = (EffSplashWork*)__memAlloc(3, total << 6);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effSplashMain;

    work->type = type;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->scale.x = scale;
    work->scale.y = scale;
    work->scale.z = scale;
    work->frame = 0;
    work->state = 0;

    child = work + 1;
    for (i = 1; i < fixedCount + 1; i++, child++) {
        child->pos.z = float_0_80427868;
        child->pos.y = float_0_80427868;
        child->pos.x = float_0_80427868;
        child->velX = float_0_80427868;
        child->velZ = float_0_80427868;
        child->timer = 0;
        child->scale.z = float_1_804278a0;
        child->scale.y = float_1_804278a0;
        child->scale.x = float_1_804278a0;

        switch (type) {
            case 0:
                if (i == 1) {
                    child->velY = float_5_804278f0;
                    child->gravity = float_0p4_804278f4;
                    child->scale.x = float_0p7_804278f8;
                    child->scale.y = float_0p7_804278f8;
                    child->scale.z = float_0p7_804278f8;
                }
                else {
                    child->velY = float_4_804278c8;
                    child->gravity = float_0p4_804278f4;
                }
                break;
            case 1:
                if (i == 1) {
                    child->velY = float_7_804278fc;
                    child->gravity = float_0p4_804278f4;
                }
                else {
                    child->velY = float_4_804278c8;
                    child->gravity = float_0p4_804278f4;
                }
                break;
            case 2:
                child->velY = float_2_80427900;
                child->gravity = float_0p1_804278b8;
                break;
            case 3:
                child->velY = float_0_80427868;
                child->gravity = float_0_80427868;
                idx = i - 1;
                if (idx < 2) {
                    child->pos.z = float_0_80427868;
                }
                else if (idx < 4) {
                    child->pos.z = float_10_80427870;
                }
                else if (idx < 6) {
                    child->pos.z = float_20_80427864;
                }
                break;
            case 4:
                if (i == 1) {
                    child->velY = float_4_804278c8;
                    child->gravity = float_0p48_80427904;
                }
                else {
                    child->velY = float_3p6_80427908;
                    child->gravity = float_0p8_8042790c;
                }
                child->scale.z = float_0p01_80427910;
                break;
        }
    }

    angleScale = float_6p2832_804278b0;
    angleDiv = float_360_80427914;
    two = float_2_80427900;
    zero = float_0_80427868;
    ten = float_10_80427870;
    one = float_1_804278a0;
    half = float_0p5_804278c4;

    for (; i < *(s32*)((s32)effect + 0x8); i++, child++) {
        rAngle = irand(0x168);
        rSpeed = irand(5);
        angle = (angleScale * (f32)rAngle) / angleDiv;
        speed = (f32)rSpeed;
        s = (f32)sin(angle);
        c = (f32)cos(angle);
        child->velX = speed * s;
        child->velZ = speed * c;
        child->pos.x = two * child->velX;
        child->pos.y = zero;
        child->pos.z = two * child->velZ;
        child->velY = ten;
        child->gravity = one;
        child->timer = irand(10);
        child->scale.x = half;
        child->scale.y = half;
        child->scale.z = half;
    }

    if (type == 2) {
        void* cam = camGetPtr(4);
        camAngle = (angleScale * *(f32*)((s32)cam + 0x114)) / angleDiv;
        camSin = (f32)sin(camAngle);
        camCos = (f32)cos(camAngle);
        effKemuTestEntry(6,
                         x + scale * (float_neg30_80427918 * camSin),
                         y + float_5_804278f0 * scale,
                         z + scale * (float_30_8042791c * camCos),
                         scale);
    }

    return effect;
}



/* CHATGPT STUB FILL: main/effect/eff_splash 20260624_184823 */

/* stub-fill: effSplashDisp | prototype_only | source_prototype */
void effSplashDisp(s32 cameraId, void* entry) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct {
        s32 type;
        Vec pos;
        f32 vx, vz;
        Vec scale;
        s32 frame, timer;
        f32 vy, gravity;
        s32 unk, state;
        u8 r, g, b, a;
    } Work;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void PSMTXIdentity(void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);

    u8 tex[64];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scale[3][4];
    f32 baseMtx[3][4];
    f32 model[3][4];
    Work* work = *(Work**)((u8*)entry + 0xC);
    s32 count = *(s32*)((u8*)entry + 8);
    s32 first = work->type == 3 ? 6 : (work->type == 2 ? 1 : 2);
    u8* camera = camGetPtr(cameraId);
    s32 i;

    PSMTXTrans(trans, work->pos.x, work->pos.y, work->pos.z);
    camera = camGetPtr(4);
    PSMTXRotRad(rot, 'y', 0.017453292f * -*(f32*)(camera + 0x114));
    PSMTXScale(scale, work->scale.x, work->scale.y, work->scale.z);
    PSMTXConcat(trans, rot, baseMtx);
    PSMTXConcat(baseMtx, scale, baseMtx);
    PSMTXConcat(((u8*)camGetPtr(cameraId)) + 0x118, baseMtx, baseMtx);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    for (i = first; i < count; i++) {
        Work* child = work + i;
        u32 color;
        if (child->timer != 0) {
            continue;
        }
        PSMTXTrans(trans, child->pos.x, child->pos.y, child->pos.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -*(f32*)(camera + 0x114));
        PSMTXScale(scale, child->scale.x, child->scale.y, child->scale.z);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(baseMtx, trans, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        effGetTexObj(0x40, tex);
        GXLoadTexObj(tex, 0);
        PSMTXIdentity(trans);
        GXLoadTexMtxImm(trans, 0x1E, 1);
        color = *(u32*)&child->r;
        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000 = -1.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile u32*)0xCC008000 = color;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile u32*)0xCC008000 = color;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = -1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile u32*)0xCC008000 = color;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
        *(volatile f32*)0xCC008000 = -1.0f;
        *(volatile f32*)0xCC008000 = -1.0f;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile u32*)0xCC008000 = color;
        *(volatile f32*)0xCC008000 = 0.0f;
        *(volatile f32*)0xCC008000 = 1.0f;
    }
}

