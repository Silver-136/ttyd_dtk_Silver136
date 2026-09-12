#include "effect/n64/eff_snowfall_n64.h"

extern void* camGetPtr(s32 camera);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void cam_clip_chk(u8* work, s32 xRange, s32 yRange, s32 timer) {
    extern f32 float_0p5_804260bc;
    extern f32 float_0_80426098;
    u8* cam;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    s32 delta;
    s32 count;

    cam = camGetPtr(4);
    centerX = *(f32*)(cam + 0x18);
    centerY = (*(f32*)(cam + 0x1C) + *(f32*)(cam + 0x10)) * float_0p5_804260bc;
    centerZ = (*(f32*)(cam + 0x20) + *(f32*)(cam + 0x118)) * float_0p5_804260bc;

    delta = (s32)(*(f32*)(work + 8) - centerX);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX + (f32)xRange - (f32)delta;
    }

    delta = (s32)(*(f32*)(work + 0xC) - centerY);
    if (delta > yRange) {
        count = delta / yRange;
        delta -= count * yRange;
        *(f32*)(work + 0xC) = centerY - (f32)yRange + (f32)delta;
    } else if (delta < -yRange) {
        if (timer < 100) {
            *(f32*)(work + 0x18) = float_0_80426098;
            *(s32*)(work + 0x28) = 0;
        } else {
            delta = -delta;
            count = delta / yRange;
            delta -= count * yRange;
            *(f32*)(work + 0xC) = centerY + (f32)yRange - (f32)delta;
        }
    }

    delta = (s32)(*(f32*)(work + 0x10) - centerZ);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ + (f32)xRange - (f32)delta;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void* effSnowfallN64Entry(s32 flags, s32 count) {
    extern void* effEntry(void);extern void* __memAlloc(s32,s32);extern void effSnowfallMain(void*);extern char str_SnowfallN64_802fbf9c[];extern void* camGetPtr(s32);extern s32 rand(void);extern f32 float_1000_804260a8,float_0p05_804260ac,float_10_804260b0,float_neg1p2_804260b4,float_0p01_804260b8,float_0p5_804260bc;
    void* entry;u8* work;u8* part;u8* cam;s32 i;
    f32 c1000, c005, c10, cneg12, c001, c05;
    entry=effEntry();*(char**)((s32)entry+0x14)=str_SnowfallN64_802fbf9c;*(s32*)((s32)entry+8)=count+1;work=__memAlloc(3,(count+1)*0x34);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effSnowfallMain;*(s32*)(work+0x24)=0;*(s32*)(work+0x20)=100;*(s32*)(work+4)=flags;
    c1000=float_1000_804260a8;*(s32*)(work+0x2C)=count;c10=float_10_804260b0;*(s32*)(work+0x28)=0xFF;c005=float_0p05_804260ac;*(s32*)work=1;c05=float_0p5_804260bc;c001=float_0p01_804260b8;cneg12=float_neg1p2_804260b4;part=work+0x34;
    for(i=0;i<count;i++,part+=0x34){cam=camGetPtr(4);*(f32*)(part+8)=*(f32*)(cam+0x18)+(f32)(rand()%2000)-c1000;*(f32*)(part+0xC)=*(f32*)(cam+0x1C)+(f32)(rand()%2000)-c1000;*(f32*)(part+0x10)=*(f32*)(cam+0x20)+(f32)(rand()%2000)-c1000;*(f32*)(part+0x14)=c005*((f32)(rand()%20)-c10);*(f32*)(part+0x18)=-(c001*(f32)(rand()%80)-cneg12);*(f32*)(part+0x1C)=c005*((f32)(rand()%20)-c10);*(s32*)(part+0x2C)=0xFF;*(f32*)(part+0x14)*=c05;*(f32*)(part+0x18)*=c05;*(f32*)(part+0x1C)*=c05;*(s32*)(part+0x30)=0;}
    return entry;
}

void effSnowfallMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern s32 cam_clip_chk(void* work, u32 flags, s32 value, s32 timer);
    extern void effSnowfallDisp(void);
    extern const Vec3 vec3_802fbf90;
    extern f32 float_166_804260a4;
    extern void* gp;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;
    s32 count;
    s32 value;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbf90;
    pos.x = *(f32*)(work + 8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    dispPos = pos;
    camGetPtr(4);
    if (*(s32*)(work + 0x20) < 100) {
        *(s32*)(work + 0x20) -= 1;
    }
    if (*(s32*)(work + 0x20) < 0) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x24) += 1;
    timer = *(s32*)(work + 0x20);
    count = *(s32*)(work + 0x2C);
    if (timer < 10) {
        *(s32*)(work + 0x28) = timer * 25;
    }
    value = (s32)float_166_804260a4;
    part = work + 0x34;
    for (i = 0; i < count; i++, part += 0x34) {
        if (*(s32*)(part + 0x30) > 0) {
            *(s32*)(part + 0x30) -= 1;
            if (*(s32*)(part + 0x30) > 0) {
                continue;
            }
        }
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x10) += *(f32*)(part + 0x1C);
        cam_clip_chk(part, 0x226, value, timer);
    }
    if (*(s32*)((s32)gp + 0x14) == 0) {
        dispEntry(4, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(7, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    }
}


void effSnowfallDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void* mapGetWork(void);
    extern void mapGetBlend(void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void* GXGetTexObjData(void*);
    extern u32 GXGetTexBufferSize(u16, u16, s32, BOOL, u8);
    extern void* memcpy(void*, const void*, u32);
    extern void DCFlushRange(void*, u32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, BOOL, BOOL, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p03125_80426094;
    extern f32 float_0_80426098;
    extern f32 float_2_8042609c;
    extern f32 float_deg2rad_804260a0;
    extern u32 dat_80426090;
    extern void* eff_snowfall_v;
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scale[3][4];
    u8 texObj[0x20];
    u8 texObj2[0x20];
    u8 blend[4];
    u8* camera = camGetPtr(cameraId);
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* part;
    void* texDst;
    void* texSrc;
    u32 color;
    u32 texSize;
    s32 i;
    s32 block;
    s16 index;

    if (*(s32*)work == 0) {
        return;
    }
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80426094, float_0p03125_80426094, float_0_80426098);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    effGetTexObjN64(99, texObj);
    effGetTexObjN64(100, texObj2);
    texDst = (void*)((u32)GXGetTexObjData(texObj) - 0x80000000U);
    texSrc = (void*)((u32)GXGetTexObjData(texObj2) - 0x80000000U);
    texSize = GXGetTexBufferSize(0x40, 0x40, 0, 1, 2);
    texDst = (void*)((u8*)texDst + texSize);
    texSize = GXGetTexBufferSize(0x10, 0x10, 0, 0, 0);
    memcpy(texDst, texSrc, texSize);
    DCFlushRange(texDst, GXGetTexBufferSize(0x40, 0x40, 0, 1, 3));
    GXInitTexObjLOD(texObj, 5, 1, float_0_80426098, float_2_8042609c,
                    float_0_80426098, 1, 1, 0);
    GXLoadTexObj(texObj, 0);
    if ((*(u16*)((u8*)mapGetWork() + 4) & 2) == 0) {
        color = (dat_80426090 & 0xFFFFFF00) | (*(u32*)(work + 0x28) & 0xFF);
    } else {
        mapGetBlend(blend);
        color = ((u32)(u8)-blend[0] << 24) | ((u32)(u8)-blend[1] << 16) |
                ((u32)(u8)-blend[2] << 8) | (u8)(blend[3] * *(u8*)(work + 0x28));
    }
    GXSetTevColor(1, &color);
    GXSetCullMode(0);
    effSetVtxDescN64(&eff_snowfall_v);
    PSMTXRotRad(rot, 0x79, float_deg2rad_804260a0 * -*(f32*)((u8*)camGetPtr(4) + 0x114));
    PSMTXConcat(camera + 0x11C, rot, rot);
    part = work + 0x34;
    for (i = 0; i < *(s32*)(work + 0x2C); i++, part += 0x34) {
        if (*(s32*)(part + 0x30) <= 0 && *(s32*)(part + 0x2C) != 0) {
            PSMTXTrans(trans, *(f32*)(part + 8), *(f32*)(part + 0xC), *(f32*)(part + 0x10));
            PSMTXConcat(rot, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 0x20);
            index = 0;
            block = 4;
            do {
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
                *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; *(volatile u16*)0xCC008000 = index; index++;
            } while (--block != 0);
        }
    }
}

typedef struct EffN64Vertex {
    s16 x;
    s16 y;
    s16 z;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} EffN64Vertex;

EffN64Vertex eff_snowfall_v[32] = {
    { -78,   3, -38,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -68,   3, -38, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -68,  12, -38, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -78,  12, -38,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -33, -33, -62,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -23, -33, -62, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -23, -24, -62, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -33, -24, -62,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -30,  39, -14,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -20,  39, -14, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    { -20,  48, -14, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -30,  48, -14,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -13, -66, -38,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  -3, -66, -38, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  -3, -57, -38, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    { -13, -57, -38,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  -5,  -4,   0,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {   5,  -4,   0, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {   5,   5,   0, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  -5,   5,   0,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  76, -13, -46,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  86, -13, -46, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  86,  -4, -46, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  76,  -4, -46,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  45,  37, -13,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  55,  37, -13, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  55,  46, -13, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  45,  46, -13,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  27, -53, -57,    0, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  37, -53, -57, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF },
    {  37, -44, -57, 2048,    0, 0xFF, 0xFF, 0xFF, 0xFF },
    {  27, -44, -57,    0,    0, 0xFF, 0xFF, 0xFF, 0xFF },
};
