#include "effect/n64/eff_akari_pose_n64.h"


u8 effAkariPoseMain(void) {
    return 0;
}


void* effAkariPoseN64Entry(s32 type, s32 count, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effAkariPoseMain(void);
    extern char str_AkariPoseN64_802faaf8[];
    extern s32 max_seq_num[];
    extern f32 float_0p5_80424c70;
    extern f32 float_neg1_80424c74;
    void* entry;
    void* work;
    u8* item;
    s32 i;
    f32 halfSeq;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_AkariPoseN64_802faaf8;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x38);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariPoseMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)((s32)work + 0x2C) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }
    *(s32*)((s32)work + 0x30) = 0xFF;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x34) = scale;

    halfSeq = (f32)max_seq_num[type] * float_0p5_80424c70;
    item = (u8*)work + 0x38;
    for (i = 1; i < count + 1; i++, item += 0x38) {
        *(s32*)(item + 0x20) = (s32)(float_neg1_80424c74 - (halfSeq * (f32)(i & 1)));
    }

    return entry;
}

void effAkariPoseDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_deg2rad_80424c48;
    extern f32 float_0p03125_80424c4c;
    extern f32 float_0_80424c50;
    extern u8 size16x16_tex32x32_vtx[];
    extern u32 unk_80429640;
    f32 baseMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 viewMtx[3][4];
    u8 texObj[0x20];
    void* cam;
    u8* work;
    u8* item;
    s32 baseAlpha;
    s32 i;
    u32 color;

    cam = camGetPtr(param_1);
    work = *(u8**)((s32)param_2 + 0xC);
    baseAlpha = *(s32*)(work + 0x30);

    PSMTXTrans(baseMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rotMtx, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114) * float_deg2rad_80424c48));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x34), *(f32*)(work + 0x34), *(f32*)(work + 0x34));
    PSMTXConcat(baseMtx, rotMtx, baseMtx);
    PSMTXConcat(baseMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, viewMtx);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80424c4c, float_0p03125_80424c4c, float_0_80424c50);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x76, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);

    item = work + 0x38;
    for (i = 1; i < *(s32*)((s32)param_2 + 8); i++, item += 0x38) {
        if (*(s32*)(item + 0x20) >= 0) {
            PSMTXTrans(baseMtx, *(f32*)(item + 4), *(f32*)(item + 8), float_0_80424c50);
            PSMTXRotRad(rotMtx, 0x7A, *(f32*)(item + 0x24) * float_deg2rad_80424c48);
            PSMTXScale(scaleMtx, *(f32*)(item + 0x34), *(f32*)(item + 0x34), *(f32*)(item + 0x34));
            PSMTXConcat(baseMtx, rotMtx, baseMtx);
            PSMTXConcat(baseMtx, scaleMtx, baseMtx);
            PSMTXConcat(viewMtx, baseMtx, baseMtx);
            GXLoadPosMtxImm(baseMtx, 0);
            GXSetCurrentMtx(0);
            color = unk_80429640;
            ((u8*)&color)[3] = (baseAlpha * *(s32*)(item + 0x30)) >> 8;
            GXSetTevColor(1, &color);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

