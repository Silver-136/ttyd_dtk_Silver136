#include "effect/n64/eff_star_power_n64.h"

typedef struct EffStarPowerWork {
    s16 field_00;
    s16 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 r1;
    s32 g1;
    s32 b1;
    s32 a1;
    f32 scale;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
} EffStarPowerWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StarPowerN64_802fc058[];
extern f32 float_0_804261ac;

u8 effStarPowerDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    s32 pass;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    effGetTexObj(0x6D, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetCullMode(0);
    for (pass = 0; pass < 2; pass++) {
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)((char*)camGetPtr(4) + 0x114), 'y');
        PSMTXScale(scale, *(f32*)(work + 0x38), *(f32*)(work + 0x38), 1.0f);
        PSMTXConcat(trans, rot, model);
        PSMTXConcat(model, scale, model);
        PSMTXRotRad(rot, *(f32*)(work + 0x3C + pass * 0xC) * 0.017453292f, 'x');
        PSMTXConcat(model, rot, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXSetNumChans(1);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXBegin(0x90, 0, 0x3C);
    }
    GXBegin(0x90, 0, 0x3C);
    return 0;
}

u8 effStarPowerMain(u32* effect) {
    extern void effDelete(void*);extern f32 dispCalcZ(void*);extern void dispEntry(s32,s32,void*,void*,f32);extern void effStarPowerDisp(void);u8* work=*(u8**)((s32)effect+0xC);s32 timer,frame;
    if(*effect&4){*effect&=~4;*(s32*)(work+0x10)=0x10;}if(*(s32*)(work+0x10)<1000)*(s32*)(work+0x10)-=1;*(s32*)(work+0x14)+=1;timer=*(s32*)(work+0x10);frame=*(s32*)(work+0x14);if(timer<0){effDelete(effect);return 0;}if(timer<0x10){if((timer<<4)<*(s32*)(work+0x24))*(s32*)(work+0x24)=timer<<4;if((timer<<4)<*(s32*)(work+0x34))*(s32*)(work+0x34)=timer<<4;}if(frame<0x10){if((frame<<4)+0xF>*(s32*)(work+0x24))*(s32*)(work+0x24)=(frame<<4)+0xF;if((frame<<4)+0xF>*(s32*)(work+0x34))*(s32*)(work+0x34)=(frame<<4)+0xF;}*(s32*)(work+0x48)=*(s32*)(work+0x3C);*(s32*)(work+0x4C)=*(s32*)(work+0x40);*(s32*)(work+0x50)=*(s32*)(work+0x44);*(s32*)(work+0x54)=*(s32*)(work+0x38);dispEntry(4,2,effStarPowerDisp,effect,dispCalcZ(work+4));return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effStarPowerN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStarPowerWork* work;
    f32 zero;
    s32 one;
    s32 zeroInt;
    s32 full;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarPowerN64_802fc058;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x58);
    *(EffStarPowerWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStarPowerMain;
    *(u32*)entry |= 2;

    one = 1;
    zeroInt = 0;
    work->field_00 = one;
    work->type = (s16)type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->x = x;
    full = 0xFF;
    half = 0x78;
    zeroInt = 0;
    work->y = y;
    zero = float_0_804261ac;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = half;
    work->r1 = full;
    work->g1 = half;
    work->b1 = zeroInt;
    work->a1 = full;
    work->a0 = full;
    work->field_3C = zero;
    work->field_40 = zero;
    work->field_44 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
