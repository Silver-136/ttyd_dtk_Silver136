#include "effect/eff_batten.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBattenEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effBattenMain(void);
    extern void PSMTXIdentity(f32 mtx[3][4]);
    extern const char str_Batten_80300398[];
    extern f32 float_0_80427eec;
    extern f32 float_3_80427f14;

    void* effect;
    void* work;
    void* child;
    s32 count;
    s32 i;
    s32 zero;
    s32 wait;
    f32 childScale;
    f32 zeroF;

    effect = effEntry();
    count = 1;
    if (type == 0) {
        count = 4;
    }
    *(const char**)((s32)effect + 0x14) = str_Batten_80300398;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x5C);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effBattenMain;
    *(u32*)effect |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(u8*)((s32)work + 0x14) = 0xFF;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(s32*)((s32)work + 0x24) = -1;
    PSMTXIdentity((f32(*)[4])((s32)work + 0x28));

    zeroF = float_0_80427eec;
    child = (void*)((s32)work + 0x5C);
    childScale = float_3_80427f14;
    i = 1;
    zero = 0;
    while (i < *(s32*)((s32)effect + 8)) {
        *(f32*)((s32)child + 4) = zeroF;
        wait = (i - 1) * 10;
        i++;
        *(f32*)((s32)child + 8) = zeroF;
        *(f32*)((s32)child + 0xC) = zeroF;
        *(u8*)((s32)child + 0x14) = zero;
        *(f32*)((s32)child + 0x10) = childScale;
        *(s32*)((s32)child + 0x18) = zero;
        *(s32*)((s32)child + 0x1C) = wait;
        child = (void*)((s32)child + 0x5C);
    }
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* stub-fill: effBattenMain | prototype_only | source_prototype */
void effBattenMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* gp;
    extern u32 animGroupBaseAsync(char*, s32, s32);
    extern s32 animPoseEntry(char*, u32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern void animPoseRelease(s32);
    extern void effDelete(void*);
    extern f64 dispCalcZ(Vec*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effBattenDisp(s32, void*);
    extern void effBattenDisp2(s32, void*);
    extern char str_EFF_batten_x_80300364[];
    extern char str_EFF_batten_rocket_80300374[];
    extern char str_A_1_80427f00[];
    extern char str_A_2_80427f04[];

    u8* entry = effect;
    u8* work = *(u8**)(entry + 0x0C);
    s32 type = *(s32*)work;
    s32 inBattle = *(s32*)((u8*)gp + 0x14) != 0;
    Vec pos;
    s32 finished = 0;
    s32 i;

    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0x0C);

    if (type == 1 || type == 2) {
        char* anim = type == 2 ? str_A_2_80427f04 : str_A_1_80427f00;
        if (animGroupBaseAsync(str_EFF_batten_x_80300364, inBattle, 0) == 0) {
            return;
        }
        if (*(s32*)(work + 0x24) == -1) {
            *(s32*)(work + 0x24) = animPoseEntry(str_EFF_batten_x_80300364, inBattle);
            animPoseSetAnim(*(s32*)(work + 0x24), anim, 1);
        }
    } else if (type == 3) {
        if (animGroupBaseAsync(str_EFF_batten_rocket_80300374, inBattle, 0) == 0) {
            return;
        }
        if (*(s32*)(work + 0x24) == -1) {
            *(s32*)(work + 0x24) = animPoseEntry(str_EFF_batten_rocket_80300374, inBattle);
            animPoseSetAnim(*(s32*)(work + 0x24), str_A_1_80427f00, 1);
        }
    }

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        *(s32*)(work + 0x1C) = 32;
    }
    if (*(s32*)(work + 0x1C) < 1000) {
        (*(s32*)(work + 0x1C))--;
    }

    if (*(s32*)(work + 0x1C) < 0) {
        if (*(s32*)(work + 0x24) != -1) {
            animPoseRelease(*(s32*)(work + 0x24));
        }
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 32) {
        *(u8*)(work + 0x14) = (u8)(*(s32*)(work + 0x1C) << 3);
    }

    if (type != 0) {
        dispEntry(3, 2, effBattenDisp2, effect, (f32)dispCalcZ(&pos));
        return;
    }

    for (i = 1; i < *(s32*)(entry + 8); i++) {
        u8* child = work + i * 0x5C;
        s32 state = *(s32*)(child + 0x18);
        s32 timer = *(s32*)(child + 0x1C);

        switch (state) {
            case 0:
                timer--;
                *(s32*)(child + 0x1C) = timer;
                if (timer < 0) {
                    *(s32*)(child + 0x1C) = 0;
                    (*(s32*)(child + 0x18))++;
                }
                break;

            case 1:
                if (timer < 30) {
                    *(f32*)(child + 0x10) += (1.0f - *(f32*)(child + 0x10)) * 0.125f;
                    *(u8*)(child + 0x14) =
                        (u8)((128.0f - (f32)*(u8*)(child + 0x14)) * 0.125f +
                             (f32)*(u8*)(child + 0x14));
                    (*(s32*)(child + 0x1C))++;
                } else {
                    *(s32*)(child + 0x1C) = (3 - i) * 10;
                    (*(s32*)(child + 0x18))++;
                }
                break;

            case 2:
                timer--;
                *(s32*)(child + 0x1C) = timer;
                if (timer < 0) {
                    *(s32*)(child + 0x1C) = 0;
                    (*(s32*)(child + 0x18))++;
                }
                break;

            case 3:
                if (timer < 30) {
                    *(f32*)(child + 0x10) += (10.0f - *(f32*)(child + 0x10)) * 0.125f;
                    *(u8*)(child + 0x14) =
                        (u8)((0.0f - (f32)*(u8*)(child + 0x14)) * 0.125f +
                             (f32)*(u8*)(child + 0x14));
                    (*(s32*)(child + 0x1C))++;
                } else {
                    finished++;
                }
                break;
        }
    }

    if (finished < *(s32*)(entry + 8) - 1) {
        dispEntry(3, 2, effBattenDisp, effect, (f32)dispCalcZ(&pos));
    } else {
        effDelete(effect);
    }
}

/* CHATGPT STUB FILL: main/effect/eff_batten 20260624_184929 */

/* stub-fill: effBattenDisp | missing_definition | ghidra_signature */
void effBattenDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void PSMTXTrans(f32[3][4], f64, f64, f64);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetChanMatColor(s32, u32*);
    extern void GXBegin(s32, s32, s32);
    extern f32 float_deg2rad_80427ee8;
    extern u32 dat_80427ee0;

    u8 texObj[0x20];
    f32 baseTrans[3][4];
    f32 baseScale[3][4];
    f32 baseRot[3][4];
    f32 baseMtx[3][4];
    u8* work = *(u8**)((u8*)effect + 0x0C);
    void* camera = camGetPtr(cameraId);
    u8 baseAlpha = *(u8*)(work + 0x14);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 halfWidth = -32.0f * 0.5f;
    f32 halfHeight = 16.0f;
    s32 i;

    effGetTexObj(0x56, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    PSMTXTrans(baseTrans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0x0C));
    PSMTXScale(baseScale, *(f32*)(work + 0x10), *(f32*)(work + 0x10), *(f32*)(work + 0x10));
    PSMTXRotRad(baseRot, 'y', float_deg2rad_80427ee8 * -*(f32*)((u8*)camera + 0x114));
    PSMTXConcat(baseTrans, baseScale, baseScale);
    PSMTXConcat(baseScale, baseRot, baseMtx);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), baseMtx, baseMtx);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++) {
        u8* child = work + i * 0x5C;
        f32 localTrans[3][4];
        f32 localScale[3][4];
        u32 color = dat_80427ee0;

        PSMTXTrans(localTrans, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0x0C));
        PSMTXScale(localScale, *(f32*)(child + 0x10), *(f32*)(child + 0x10), *(f32*)(child + 0x10));
        PSMTXConcat(localTrans, localScale, localScale);
        PSMTXConcat(baseMtx, localScale, localScale);
        GXLoadPosMtxImm(localScale, 0);
        GXSetCurrentMtx(0);

        ((u8*)&color)[3] = (u8)((baseAlpha * *(u8*)(child + 0x14)) / 255);
        GXSetChanMatColor(4, &color);
        GXBegin(0x80, 0, 4);

        *fifo = halfWidth;
        *fifo = halfHeight;
        *fifo = 0.0f;
        *fifo = 0.0f;
        *fifo = 0.0f;
        *fifo = halfHeight;
        *fifo = halfHeight;
        *fifo = 0.0f;
        *fifo = 2.0f;
        *fifo = 0.0f;
        *fifo = halfHeight;
        *fifo = halfWidth;
        *fifo = 0.0f;
        *fifo = 2.0f;
        *fifo = 2.0f;
        *fifo = halfWidth;
        *fifo = halfWidth;
        *fifo = 0.0f;
        *fifo = 0.0f;
        *fifo = 2.0f;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBattenDisp2(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_80427ee8;
    extern f32 float_0_80427eec;
    extern f32 float_2_80427ef0;
    extern u32 dat_80427ee4;

    u32 evtColor;
    u32 color;
    f32 mtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    void* work;
    void* cam;
    f32 deg;
    f32 angle;

    work = *(void**)((s32)effect + 0xC);
    if (*(s32*)((s32)work + 0x24) != -1) {
        PSMTXTrans(mtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_80427ee8;
        PSMTXRotRad(rotMtx, 0x79, deg * angle);
        PSMTXConcat(mtx, rotMtx, mtx);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXConcat(mtx, (f32(*)[4])((s32)work + 0x28), mtx);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x24), 0x40);
        color = dat_80427ee4;
        ((u8*)&color)[3] = *(u8*)((s32)work + 0x14);
        evtColor = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x24), &evtColor);
        animPoseMain(*(s32*)((s32)work + 0x24));
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 1, float_0_80427eec, float_2_80427ef0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 2, float_0_80427eec, float_2_80427ef0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 3, float_0_80427eec, float_2_80427ef0);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

