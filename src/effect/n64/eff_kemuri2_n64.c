#include "effect/n64/eff_kemuri2_n64.h"
void effKemuri2Main(void* effEntry);
void effKemuri2Disp(s32 cameraId, void* effect);
void main_dl(s32 type);

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri2Work {
    s32 active;
    s16 type;
    u8 pad_06[2];
    f32 x;
    f32 y;
    f32 z;
    u8 pad_14[0x30];
    f32 moveScale;
    f32 yVel;
    f32 yAccel;
    f32 vx;
    f32 vz;
    s32 animId;
    s32 animFrame;
    s32 animValue;
} EffKemuri2Work;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fb1d8;
extern char str_Kemuri2N64_802fb1f0[];
extern s8* anim_data_table[];
extern f32 float_0p33333_80425650;
extern f32 float_1p75_80425654;
extern f32 float_neg0p16_80425658;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effKemuri2N64Entry(s32 type, f32 x, f32 y, f32 z, f32 vx, f32 vz) {
    void* entry = effEntry();
    EffKemuri2Work* work;
    f32 yAccel;
    f32 yVel;
    f32 moveScale;

    *(char**)((s32)entry + 0x14) = str_Kemuri2N64_802fb1f0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x64);
    *(EffKemuri2Work**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri2Main;
    work->animId = type == 2;
    moveScale = float_0p33333_80425650;
    work->animFrame = 0;
    yVel = float_1p75_80425654;
    work->type = type;
    yAccel = float_neg0p16_80425658;
    work->active = 1;
    work->vx = vx;
    work->vz = vz;
    work->x = x;
    work->y = y;
    work->z = z;
    work->moveScale = moveScale;
    work->yVel = yVel;
    work->yAccel = yAccel;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effKemuri2Main(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri2Work* work;
    register s32 i;
    s32 frame;
    s32 value;
    s8* anim;

    work = *(EffKemuri2Work**)((s32)effEntry + 0xC);
    zero = &vec3_802fb1d8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    anim = anim_data_table[work->animId];
    frame = work->animFrame;
    work->animFrame = frame + 1;
    value = anim[frame];
    work->animValue = value;
    if (value < 0) {
        effDelete(effEntry);
    } else {
        i = 0;
        while (i < *(s32*)((s32)effEntry + 0x8)) {
            i++;
            work->x += work->moveScale * work->vx;
            work->z += work->moveScale * work->vz;
            work->yVel += work->yAccel;
            work->y += work->yVel;
            work++;
        }
        dispEntry(4, 2, effKemuri2Disp, effEntry, dispCalcZ(&dispPos));
    }
}


void main_dl(s32 type) {
    typedef struct GXTexObj { u32 words[8]; } GXTexObj;
    extern void effGetTexObjN64(s32, GXTexObj*);
    extern void GXLoadTexObj(GXTexObj*, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, u32, u32, u32, u32, u32);
    extern void GXSetTevAlphaOp(s32, u32, u32, u32, u32, u32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, u32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXBegin(s32, s32, s32);
    GXTexObj tex;

    if (type == 1) effGetTexObjN64(0xD, &tex);
    else if (type == 0) effGetTexObjN64(0xC, &tex);
    else effGetTexObjN64(0xE, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 2);
    GXSetTevAlphaIn(0, 0, 0, 0, 4);
    GXSetTevOrder(1, 1, 0, -1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 0, 0, 0);
    GXSetTevAlphaIn(1, 0, 1, 4, 7);
    GXSetTevOrder(2, -1, -1, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0, 0, 0, 0);
    GXSetTevAlphaIn(2, 0, 1, 2, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXBegin(0x80, 0, 4);
}

void effKemuri2Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetTevColor(s32, void*);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void main_dl(s32);
    extern f32 float_176_80425630;
    extern f32 float_24_80425634;
    extern f32 float_224_80425638;
    extern f32 float_32_8042563c;
    extern f32 float_256_80425640;
    extern f32 float_1_80425644;
    extern f32 float_0_80425648;
    extern f32 float_deg2rad_8042564c;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    Mtx scale;
    Mtx texMtx;
    Mtx trans;
    Mtx rotate;
    Mtx draw;
    u32 color0 = 0xFFFFFFFF;
    u32 color1 = 0x808080FF;
    s32 i;
    s32 frame = *(s32*)(work + 0x60);
    s32 type = *(u16*)(work + 4);
    s32 tile = type == 0 ? 0x18 : 0x20;
    f32 width = type == 0 ? float_176_80425630 : (type == 1 ? float_224_80425638 : float_256_80425640);
    f32 height = type == 0 ? float_24_80425634 : float_32_8042563c;
    f32 sx = (f32)tile / width;
    f32 sy = (f32)tile / height;

    GXSetTevColor(1, &color0);
    GXSetTevColor(2, &color1);
    PSMTXScale(scale, sx, sy, float_1_80425644);
    PSMTXTrans(trans, (f32)(frame * tile) / width, float_0_80425648 / height, float_0_80425648);
    PSMTXConcat(trans, scale, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    PSMTXScale(scale, sx, sy, float_1_80425644);
    PSMTXTrans(trans, (f32)((frame + 1) * tile) / width, float_0_80425648 / height, float_0_80425648);
    PSMTXConcat(trans, scale, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, 1);
    for (i = 0; i < *(s32*)(entry + 8); i++, work += 0x64) {
        u8* camera3d;
        PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
        camera3d = (u8*)camGetPtr(cameraId);
        PSMTXRotRad(rotate, float_deg2rad_8042564c * -*(f32*)(camera3d + 0x114), 'y');
        PSMTXConcat(trans, rotate, draw);
        PSMTXConcat((f32 (*)[4])(camera + 0x11C), draw, draw);
        GXLoadPosMtxImm(draw, 0);
        main_dl(type);
    }
}
