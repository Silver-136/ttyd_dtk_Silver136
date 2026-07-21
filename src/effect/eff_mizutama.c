#include "effect/eff_mizutama.h"

typedef struct EffMizutamaVec {
    f32 x;
    f32 y;
    f32 z;
} EffMizutamaVec;

void* effMizutamaEntry(s32 type, f32 x, f32 y, f32 z, f32 angle) {
    typedef struct EffMizutamaEntryRaw {
        u32 flags;
        u32 inBattle;
        s32 count;
        void* work;
        void (*main)(void*);
        char* name;
        char effectName[16];
    } EffMizutamaEntryRaw;

    typedef struct EffMizutamaWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        f32 vx;
        f32 vy;
        f32 scale;
        s32 unk1C;
        s32 unk20;
        s32 timer;
    } EffMizutamaWork;

    extern EffMizutamaEntryRaw* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 value);
    extern f64 sin(f64 value);
    extern f64 cos(f64 value);
    extern void effMizutamaMain(void*);
    extern char str_mizutama_802fefec[];

    extern f32 float_1_804277dc;
    extern f32 float_0_804277d8;
    extern f32 float_6p2832_804277f0;
    extern f32 float_360_804277cc;
    extern f32 float_2_804277f4;
    extern f32 float_100_804277f8;
    extern f32 float_225_804277fc;
    extern f32 float_135_80427800;

    s32 timer;
    s32 base;
    EffMizutamaEntryRaw* entry;
    EffMizutamaWork* child;
    s32 i;
    s32 count;
    EffMizutamaWork* work;
    f32 radius;
    f32 rad;
    f32 zero;
    f32 dir;

    entry = effEntry();

    switch (type) {
        case 0:
        case 4:
            count = irand(2) + 3;
            break;
        case 1:
        case 5:
            count = irand(5) + 6;
            break;
        case 2:
        case 6:
            count = irand(10) + 11;
            break;
        case 3:
        case 7:
            count = irand(15) + 16;
            break;
    }

    entry->name = str_mizutama_802fefec;
    entry->count = count;
    work = __memAlloc(3, entry->count * sizeof(EffMizutamaWork));
    entry->work = work;
    entry->main = effMizutamaMain;

    base = (type + 1) * 10;
    work->type = type;
    child = work + 1;
    i = 1;
    work->x = x;
    timer = 2;
    work->y = y;
    work->z = z;
    work->scale = float_1_804277dc;
    work->unk1C = 0;
    work->unk20 = 0;

    while (i < entry->count) {
        if (type < 4) {
            radius = (f32)base;

            rad = (float_6p2832_804277f0 * (f32)irand(360)) / float_360_804277cc;
            child->x = radius * (f32)sin(rad);
            child->y = radius * -(f32)cos(rad);

            rad = (float_6p2832_804277f0 * angle) / float_360_804277cc;
            child->z = float_0_804277d8;
            child->vx = float_2_804277f4 * (f32)sin(rad);
            child->vy = float_2_804277f4 * (f32)cos(rad);

            child->scale = (f32)(irand(20) + 80) / float_100_804277f8;
            child->timer = timer;
        } else {
            radius = (f32)base;
            irand(360);
            child->x = (f32)irand(((s32)radius) << 1) - radius;
            zero = float_0_804277d8;
            child->y = zero;
            child->z = zero;

            if (child->x < zero) {
                dir = float_225_804277fc;
            } else {
                dir = float_135_80427800;
            }

            rad = (float_6p2832_804277f0 * dir) / float_360_804277cc;
            child->vx = float_2_804277f4 * (f32)sin(rad);
            child->vy = float_2_804277f4 * (f32)cos(rad);

            child->scale = (f32)(irand(20) + 80) / float_100_804277f8;
            child->timer = timer;
        }

        timer += 2;
        i++;
        child++;
    }

    return entry;
}

void effMizutamaMain(void* entry) {
    extern const EffMizutamaVec vec3_802fefe0;
    extern f32 float_0p97_804277e0;
    extern f32 float_0p3_804277e4;
    extern f32 float_0p99_804277e8;
    extern f32 float_0p01_804277ec;
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void effMizutamaDisp(void);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

    void* work;
    EffMizutamaVec pos;
    EffMizutamaVec base;
    s32 i = 1;
    s32 dead = 0;

    work = *(void**)((s32)entry + 0xC);
    base = vec3_802fefe0;
    base.x = *(f32*)((s32)work + 4);
    base.y = *(f32*)((s32)work + 8);
    base.z = *(f32*)((s32)work + 0xC);
    pos = base;

    work = (void*)((s32)work + 0x28);
    while (i < *(s32*)((s32)entry + 8)) {
        if (*(s32*)((s32)work + 0x24) != 0) {
            *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x24) - 1;
        } else {
            f32 damp = float_0p97_804277e0;
            f32 rise = float_0p3_804277e4;
            f32 fade = float_0p99_804277e8;
            f32 min = float_0p01_804277ec;

            *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
            *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
            *(f32*)((s32)work + 0x10) *= damp;
            *(f32*)((s32)work + 0x14) *= damp;
            *(f32*)((s32)work + 8) += rise;
            *(f32*)((s32)work + 0x18) *= fade;
            if (*(f32*)((s32)work + 0x18) < min) {
                dead++;
            }
        }
        i++;
        work = (void*)((s32)work + 0x28);
    }

    if (dead >= *(s32*)((s32)entry + 8) - 1) {
        effDelete(entry);
    } else {
        dispEntry(4, 2, effMizutamaDisp, entry, dispCalcZ(&pos));
    }
}



/* CHATGPT STUB FILL: main/effect/eff_mizutama 20260624_184823 */

/* stub-fill: effMizutamaDisp | prototype_only | source_prototype */
void effMizutamaDisp(s32 cameraId, void* effect) {
    typedef f32 MtxLocal[3][4];
    typedef struct CameraLocal {
        char pad[0x58];
        MtxLocal viewMtx;
    } CameraLocal;

    extern CameraLocal* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(MtxLocal mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(MtxLocal mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(MtxLocal a, MtxLocal b, MtxLocal out);
    extern void GXSetNumChans(u32 count);
    extern void GXSetNumTexGens(u32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, u32 mtx, u32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(u32 count);
    extern void GXSetTevOrder(u32 stage, u32 texCoord, u32 texMap, u32 color);
    extern void GXSetTevColorOp(s32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(u32 vtxfmt, s32 attr, u32 compCnt, u32 compType, u32 frac);
    extern void GXLoadPosMtxImm(MtxLocal mtx, s32 id);
    extern void GXSetCurrentMtx(u32 id);
    extern void GXBegin(s32 prim, u8 vtxfmt, u16 nverts);
    extern volatile f32 DAT_cc008000;
    extern f32 float_8_804277c8;
    extern f32 float_360_804277cc;
    extern f32 float_0p5_804277d0;
    extern f32 float_4_804277d4;
    extern f32 float_0_804277d8;
    extern f32 float_1_804277dc;

    void* base = *(void**)((s32)effect + 0xC);
    void* work = base;
    CameraLocal* cam = camGetPtr(cameraId);
    f32 y = *(f32*)((s32)base + 8);
    MtxLocal trans;
    MtxLocal scale;
    MtxLocal baseMtx;
    MtxLocal childMtx;
    MtxLocal drawMtx;
    u8 texObj[0x20];
    s32 i;
    f32 size;

    PSMTXTrans(trans, *(f32*)((s32)base + 4), y, *(f32*)((s32)base + 0xC));
    PSMTXScale(scale, *(f32*)((s32)base + 0x18), *(f32*)((s32)base + 0x18), *(f32*)((s32)base + 0x18));
    PSMTXConcat(trans, scale, baseMtx);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 3);
    effGetTexObj(0x3F, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    size = -float_8_804277c8;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        work = (void*)((s32)work + 0x28);
        if (*(s32*)((s32)work + 0x24) == 0) {
            PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
            PSMTXScale(scale, *(f32*)((s32)work + 0x18), *(f32*)((s32)work + 0x18), *(f32*)((s32)work + 0x18));
            PSMTXConcat(trans, scale, childMtx);
            PSMTXConcat(baseMtx, childMtx, drawMtx);
            if (y <= float_360_804277cc) {
                f32 half = size * float_0p5_804277d0;
                PSMTXConcat(cam->viewMtx, drawMtx, drawMtx);
                GXLoadPosMtxImm(drawMtx, 0);
                GXSetCurrentMtx(0);
                GXBegin(0x80, 0, 4);
                DAT_cc008000 = half;
                DAT_cc008000 = float_4_804277d4;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_4_804277d4;
                DAT_cc008000 = float_4_804277d4;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_1_804277dc;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_4_804277d4;
                DAT_cc008000 = half;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_1_804277dc;
                DAT_cc008000 = float_1_804277dc;
                DAT_cc008000 = half;
                DAT_cc008000 = half;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_0_804277d8;
                DAT_cc008000 = float_1_804277dc;
            }
        }
    }
}

