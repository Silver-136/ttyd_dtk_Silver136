#include "effect/n64/eff_opuku_jetw_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffOpukuJetwWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 field_18;
    s32 field_1c;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2c;
    s32 field_30;
    f32 scale;
    f32 field_38;
    f32 field_3c;
    f32 field_40;
} EffOpukuJetwWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
void effOpukuJetwMain(void* effEntry);
void effOpukuJetwDisp(s32 cameraId, void* effect);

extern const char str_OpukuJetwN64_802fbb60[];
extern const Vec3 vec3_802fbb48;
extern f32 float_0_80425b48;
extern f32 float_1_80425b4c;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effOpukuJetwN64Entry(s32 type, f32 x, f32 y, f32 z, s32 timer, f32 scale) {
    void* entry;
    EffOpukuJetwWork* work;
    s32 zero;
    s32 white;
    f32 oneFloat;
    f32 zeroFloat;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_OpukuJetwN64_802fbb60;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x44);
    *(EffOpukuJetwWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuJetwMain;

    zero = 0;
    *(u32*)entry |= 2;
    work->type = type;
    work->frame = zero;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }

    white = 0xFF;
    work->field_24 = white;
    zeroFloat = float_0_80425b48;
    work->x = x;
    oneFloat = float_1_80425b4c;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->field_18 = 0x46;
    work->field_1c = 0xB4;
    work->field_20 = white;
    work->field_28 = white;
    work->field_2c = white;
    work->field_30 = white;
    work->field_38 = zeroFloat;
    work->field_3c = oneFloat;
    work->field_40 = oneFloat;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effOpukuJetwMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    EffOpukuJetwWork* work;
    s32 flags;
    s32 timer;
    s32 frame;

    work = *(EffOpukuJetwWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fbb48;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if ((flags & 4) != 0) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 0x10;
    }

    timer = work->timer;
    if (timer < 1000) {
        work->timer = timer - 1;
    }

    frame = work->frame + 1;
    work->frame = frame;
    if (work->timer < 0) {
        effDelete(effEntry);
    } else {
        if (work->frame < 8) {
            work->field_24 = (work->frame << 5) + 0x1F;
        }
        dispEntry(4, 2, effOpukuJetwDisp, effEntry, dispCalcZ(&dispPos));
    }
}


void effOpukuJetwDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32); extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32); extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32); extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32); extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32); extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32); extern void GXSetTevColor(s32, void*); extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425b40, float_0p03125_80425b44, float_0_80425b48, float_1_80425b4c;
    extern u8 size32x16_tex64x32_vtx[];
    f32 trans[3][4], rot[3][4], scale[3][4];
    u8 texObj[0x20];
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    u32 color;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80425b40 * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXScale(scale, *(f32*)(work + 0x34), *(f32*)(work + 0x34), *(f32*)(work + 0x34));
    PSMTXConcat(trans, rot, trans); PSMTXConcat(trans, scale, trans);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, trans);
    GXSetNumChans(0); GXSetNumTevStages(1); GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0); GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 3, 2, 8, 0); GXSetTevAlphaIn(0, 0, 1, 7, 7);
    effGetTexObjN64(0x34, texObj); GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80425b44, float_0p03125_80425b44, float_0_80425b48);
    GXLoadTexMtxImm(scale, 0x1E, 1); GXSetCullMode(0);
    PSMTXRotRad(rot, 0x7A, float_deg2rad_80425b40 * *(f32*)(work + 0x38));
    PSMTXScale(scale, *(f32*)(work + 0x3C), *(f32*)(work + 0x40), float_1_80425b4c);
    PSMTXConcat(rot, scale, scale); PSMTXConcat(trans, scale, trans);
    color = *(u32*)(work + 0x24); GXSetTevColor(2, &color); GXSetTevColor(1, &color);
    GXLoadPosMtxImm(trans, 0); GXSetCurrentMtx(0); effSetVtxDescN64(size32x16_tex64x32_vtx);
    GXBegin(0x90, 0, 6); tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
