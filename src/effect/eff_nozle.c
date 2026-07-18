#include "effect/eff_nozle.h"

void effNozleMain(void* effEntry) {
    typedef struct VecNozle_s {
        f32 x;
        f32 y;
        f32 z;
    } VecNozle;

    typedef struct EffEntryNozle_s {
        u32 flags;
        u32 battle;
        s32 count;
        void* work;
    } EffEntryNozle;

    typedef struct NozleWork_s {
        s32 mode;
        f32 x;
        f32 y;
        f32 z;
        f32 scaleX;
        f32 scaleY;
        f32 rotA;
        f32 rotB;
        f32 rotC;
        f32 rotD;
        f32 baseScale;
        s32 timer;
        s32 frame;
        s32 alpha;
    } NozleWork;

    typedef struct NozleChildWork_s {
        f32 x;
        f32 y;
        f32 z;
        f32 scaleX;
        f32 scaleY;
        f32 rotA;
        f32 rotB;
        f32 rotC;
        f32 rotD;
        f32 baseScale;
        s32 timer;
        s32 frame;
        s32 alpha;
        s32 unk34;
        s32 state;
        s32 unk3C;
        s32 unk40;
    } NozleChildWork;

    extern VecNozle vec3_802fefc0[];
    extern u8 scale_dt[42];
    extern f32 float_100_80427794;
    extern f32 float_neg0p041667_80427798;
    extern f32 float_neg0p083333_8042779c;
    extern f32 float_300_804277a0;
    extern f32 float_0p002_804277b4;
    extern f32 float_0p004_804277b8;
    extern f32 float_1p3_804277a4;
    extern f32 float_500_804277a8;
    extern f32 float_1p2_804277ac;
    extern f32 float_0p8_804277b0;
    extern void effNozleDisp(void);
    extern void effDelete(void* eff);
    extern f32 dispCalcZ(VecNozle* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void (*callback)(void), void* param, f32 order);

    EffEntryNozle* eff;
    NozleWork* work;
    VecNozle zpos;
    VecNozle pos;
    s32 timer;
    s32 i;
    NozleChildWork* child;
    f32 value;

    eff = (EffEntryNozle*)effEntry;
    work = (NozleWork*)eff->work;

    pos = vec3_802fefc0[0];
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    zpos = pos;

    if (eff->flags & 4) {
        eff->flags &= ~4;
        work->timer = 0x20;
    }

    switch (work->mode) {
        case 0:
        case 2:
            if ((u32)work->frame < 0x15U) {
                i = work->frame << 1;
                work->scaleX = (f32)scale_dt[i] / float_100_80427794;
                work->scaleY = (f32)scale_dt[i + 1] / float_100_80427794;
            }

            if ((u32)work->timer < 0x15U) {
                i = work->timer << 1;
                work->scaleX = (f32)scale_dt[i] / float_100_80427794;
                work->scaleY = (f32)scale_dt[i + 1] / float_100_80427794;
            }

            work->rotB += float_neg0p041667_80427798;
            work->rotD += float_neg0p083333_8042779c;
            break;

        case 3: {
            f32 div300;
            f32 addSmall;
            f32 addLarge;

            child = (NozleChildWork*)((s32)work + 0x44);
            div300 = float_300_804277a0;
            addSmall = float_0p002_804277b4;
            addLarge = float_0p004_804277b8;
            for (i = 1; i < eff->count; i++) {
                value = child->y;
                child->y = value + (-value / div300);

                if (child->state == 0) {
                    child->scaleY += (float_1p3_804277a4 - child->scaleY) / float_500_804277a8;
                    if (child->scaleY > float_1p2_804277ac) {
                        child->state = 1;
                    }
                } else {
                    child->scaleY += (float_0p8_804277b0 - child->scaleY) / div300;
                }

                child->rotB += addSmall;
                child->rotC += addSmall;
                child->rotD += addLarge;
                child = (NozleChildWork*)((s32)child + 0x44);
            }
            break;
        }
    }

    timer = work->timer;
    if (timer < 1000) {
        timer--;
        work->timer = timer;
    }

    if (work->timer < 0x10) {
        work->alpha = work->timer << 4;
    }

    work->frame++;

    if (work->timer < 0) {
        effDelete(eff);
    } else if (work->mode == 3) {
        value = dispCalcZ(&zpos);
        dispEntry(4, 8, effNozleDisp, eff, value);
    } else {
        value = dispCalcZ(&zpos);
        dispEntry(4, 2, effNozleDisp, eff, value);
    }
}



/* CHATGPT STUB FILL: main/effect/eff_nozle 20260624_184823 */

/* stub-fill: effNozleDisp | prototype_only | source_prototype */
void effNozleDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXLoadTexObj(void*, s32);
    extern void effGetTexObj(s32, void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);

    s32* work = *(s32**)((u8*)effect + 0xC);
    u8* camera = camGetPtr(cameraId);
    u8 tex[32];
    f32 m0[3][4];
    f32 m1[3][4];
    f32 m2[3][4];
    f32 model[3][4];
    f32 scale = *(f32*)(work + 10);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    PSMTXTrans(m0, *(f32*)(work + 1), *(f32*)(work + 2), *(f32*)(work + 3));
    PSMTXRotRad(m1, 'y', 0.017453292f * *(f32*)(work + 0x10));
    PSMTXConcat(m0, m1, m0);
    PSMTXRotRad(m1, 'x', 0.017453292f * *(f32*)(work + 0xF));
    PSMTXConcat(m0, m1, m0);
    PSMTXScale(m2, scale * *(f32*)(work + 4), scale * *(f32*)(work + 5), scale);
    PSMTXConcat(m0, m2, m0);
    PSMTXConcat(camera + 0x30, m0, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 5, 0x21, 0, 0x7D);
    PSMTXTrans(m0, *(f32*)(work + 6), -*(f32*)(work + 7), 0.0f);
    GXLoadTexMtxImm(m0, 0x1E, 1);
    PSMTXTrans(m0, *(f32*)(work + 8), -*(f32*)(work + 9), 0.0f);
    GXLoadTexMtxImm(m0, 0x21, 1);

    effGetTexObj(0x3C, tex);
    GXLoadTexObj(tex, 0);
    if (*work == 2) {
        effGetTexObj(0x3E, tex);
        GXLoadTexObj(tex, 1);
    } else if (*work == 0 || *work == 3) {
        effGetTexObj(0x3D, tex);
        GXLoadTexObj(tex, 1);
    }

    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 1, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 8);
    GXSetTevAlphaIn(0, 0, 0, 0, 4);
    GXSetTevOrder(1, 1, 0, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 15, 10, 0);
    GXSetTevAlphaIn(1, 0, 7, 4, 7);
    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0, 15, 10, 0);
    GXSetTevAlphaIn(2, 0, 7, 5, 7);
    GXSetCullMode(0);
}

/* stub-fill: effNozleEntry | missing_definition | ghidra_signature */
void* effNozleEntry(s32 mode, f32 x, f32 y, f32 z, s32 timer, f32 arg5, f32 arg6) {
    extern void effKemuTestEntry(s32 kind, f32 x, f32 y, f32 z, f32 scale);
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effNozleMain(void* effect);
    extern const char str_Nozle_804277bc[];
    extern f32 float_1_80427790;
    extern f32 float_0_8042778c;
    extern f32 float_20_804277c4;

    void* effect;
    void* work;
    void* child;
    s32 count;
    s32 i;

    if (mode == 1) {
        effKemuTestEntry(3, x, y, z, float_1_80427790);
        return 0;
    }

    effect = effEntry();
    count = 1;
    if (mode == 3) {
        count = 4;
    }
    *(const char**)((s32)effect + 0x14) = str_Nozle_804277bc;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x44);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effNozleMain;
    *(u32*)effect |= 2;

    *(s32*)work = mode;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = (mode == 3) ? float_1_80427790 : float_0_8042778c;
    *(f32*)((s32)work + 0x14) = (mode == 3) ? float_1_80427790 : float_0_8042778c;
    *(f32*)((s32)work + 0x28) = float_1_80427790;
    *(f32*)((s32)work + 0x1C) = float_0_8042778c;
    *(f32*)((s32)work + 0x18) = float_0_8042778c;
    *(f32*)((s32)work + 0x24) = float_0_8042778c;
    *(f32*)((s32)work + 0x20) = float_0_8042778c;
    *(s32*)((s32)work + 0x2C) = (timer <= 0) ? 1000 : timer;
    *(s32*)((s32)work + 0x30) = 0;
    *(s32*)((s32)work + 0x34) = 0xFF;
    *(f32*)((s32)work + 0x3C) = arg5;
    *(f32*)((s32)work + 0x40) = arg6;

    child = (void*)((s32)work + 0x44);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child = (void*)((s32)child + 0x44)) {
        s32 sign = (i & 1) ? -1 : 1;
        *(f32*)((s32)child + 4) = (f32)(sign * 0x190);
        *(f32*)((s32)child + 8) = float_0_8042778c;
        *(f32*)((s32)child + 0xC) = float_20_804277c4 * (f32)i;
        *(f32*)((s32)child + 0x10) = (f32)-sign;
        *(f32*)((s32)child + 0x14) = float_1_80427790;
        *(f32*)((s32)child + 0x1C) = float_0_8042778c;
        *(f32*)((s32)child + 0x18) = float_0_8042778c;
        *(f32*)((s32)child + 0x24) = float_0_8042778c;
        *(f32*)((s32)child + 0x20) = float_0_8042778c;
        *(s32*)((s32)child + 0x34) = 0xFF;
        *(s32*)((s32)child + 0x38) = 0;
    }

    return effect;
}

