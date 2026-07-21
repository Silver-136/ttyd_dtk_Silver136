#include "effect/n64/eff_stardust_n64.h"

void effStardustDisp(s32 cameraId, void* effect);

void* effStardustN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effStardustMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_StardustN64_802fc0b8[];
    void* entry = effEntry();
    u8* work;
    u8* part;
    s32 count;
    s32 i;
    s32 life = 0x20;
    f32 radiusScale = scale;

    if (type == 0) {
        count = 0x24;
    } else if (type == 1 || type == 3) {
        count = 4;
    } else {
        count = 8;
    }
    if (type >= 3) {
        radiusScale = 8.0f;
    }

    *(char**)((s32)entry + 0x14) = str_StardustN64_802fc0b8;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x40);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStardustMain;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x20) = 0;
    *(s32*)work = type;
    *(s32*)(work + 0x24) = 0x1E;
    *(f32*)(work + 0x28) = 1.0f;
    *(f32*)(work + 0x2C) = 1.0f;
    *(f32*)(work + 0x30) = 1.0f;
    *(s32*)(work + 0x3C) = 4;
    *(f32*)(work + 0x1C) = 1.0f;

    for (i = 1; i <= count; i++, life += 2) {
        f32 speedX = (f32)(rand() % (s32)(1000.0f * scale)) / 1000.0f;
        f32 speedY = (f32)(rand() % (s32)(1000.0f * radiusScale)) / 1000.0f;
        f32 angle;
        f32 angleRad;
        part = work + i * 0x40;

        if (type == 0) {
            f32 ringAngle = (6.2832f * ((1800.0f / (f32)count) * (f32)i - 90.0f)) / 360.0f;
            f32 circleAngle = (6.2832f * (360.0f / (f32)count) * (f32)i) / 360.0f;
            f32 ring = scale *
                (85.0f - 15.0f * (f32)sin(ringAngle) - (f32)((i & 1) * 5)) / 100.0f;
            *(f32*)(part + 0x18) = 0.0f;
            *(f32*)(part + 4) = ring * (f32)sin(circleAngle);
            *(f32*)(part + 8) = ring * (f32)cos(circleAngle);
            *(f32*)(part + 0xC) = 0.0f;
            *(s32*)(part + 0x38) = 0;
            continue;
        }

        angle = (360.0f / (f32)count) * (f32)i + (f32)(rand() % 360);
        angleRad = (6.2832f * angle) / 360.0f;
        *(f32*)(part + 0x18) =
            (type >= 3 || (i & 1)) ? -(0.05f + (f32)(rand() % 50) / 1000.0f) : 0.0f;
        *(f32*)(part + 4) = 0.0f;
        *(f32*)(part + 8) = 0.0f;
        *(f32*)(part + 0xC) = 0.0f;
        *(f32*)(part + 0x10) = speedX * (f32)sin(angleRad);
        *(f32*)(part + 0x14) = speedY * (f32)cos(angleRad);
        *(f32*)(part + 0x1C) = 1.0f;
        *(s32*)(part + 0x20) = 0;
        *(s32*)(part + 0x24) = life;
        *(s32*)(part + 0x38) = -1;
        *(s32*)(part + 0x3C) = i % 3;
    }
    return entry;
}

u8 effStardustMain(void* effect) {
    extern void effDelete(void*);
    extern s32 effTblRandN64(s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    u8* work = *(u8**)((s32)effect + 0xC);
    s32 type = *(s32*)work;
    s32 frame = *(s32*)(work + 0x20);
    s32 i;
    s32 expired = 0;
    s32 cameraId = *(s32*)(work + 0x3C);
    f32 pos[3];

    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    *(s32*)(work + 0x24) -= 1;
    *(s32*)(work + 0x20) += 1;
    if ((type == 0 || type == 10) && *(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return 0;
    }
    for (i = 0; i < *(s32*)((s32)effect + 8) - 1; i++) {
        u8* part = work + 0x40 + i * 0x40;
        *(s32*)(part + 0x38) = (frame + i) & 7;
        if (type == 10) {
            *(f32*)(part + 0x10) *= 0.96f;
            *(f32*)(part + 0x14) *= 0.96f;
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x1C) += 0.1f * (0.1f - *(f32*)(part + 0x1C));
        } else if (type == 0) {
            *(f32*)(part + 0x18) -= 0.02f;
            if (frame >= 8) {
                f32 angle = 6.2832f * (f32)effTblRandN64(0x168, i + 0xB1) / 360.0f;
                f32 radius = 1.0f + 0.5f * (f32)(i & 3);
                *(s32*)part = 10;
                *(f32*)(part + 0x10) = radius * (f32)sin(angle);
                *(f32*)(part + 0x14) = radius * (f32)cos(angle);
                *(f32*)(part + 0x18) = -0.05f;
            }
        } else {
            *(f32*)(part + 0x10) *= 0.9f;
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
            if (*(s32*)(part + 0x24) < 0) expired++;
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(s32*)(part + 0x24) -= 1;
        }
    }
    if (type == 0 || type == 10 || expired < i) {
        dispEntry(cameraId, 2, effStardustDisp, effect, (f32)dispCalcZ(pos));
    } else {
        effDelete(effect);
    }
    return 0;
}

void effStardustDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void GXSetTevColor(s32,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_deg2rad_80426274,float_0p0056818_80426278,float_0p045455_8042627c,float_0_80426280;
    f32 base[3][4],rot[3][4],scale[3][4],mtx[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);u8* p=work+0x40;s32 i;u32 color;
    PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x79,float_deg2rad_80426274*-*(f32*)((s32)camGetPtr(cameraId)+0x114));PSMTXScale(scale,*(f32*)(work+0x1C),*(f32*)(work+0x1C),*(f32*)(work+0x1C));PSMTXConcat(base,rot,base);PSMTXConcat(base,scale,base);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),base,base);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p0056818_80426278,float_0p045455_8042627c,float_0_80426280);GXLoadTexMtxImm(scale,0x1E,1);effGetTexObjN64(0x20,texObj);GXLoadTexObj(texObj,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,2,1,8,0);GXSetTevAlphaIn(0,0,1,7,7);GXSetCullMode(0);
    for(i=0;i<*(s32*)((s32)effect+8)-1;i++,p+=0x40){if(*(s32*)(p+0x38)<0)continue;PSMTXTrans(mtx,*(f32*)(p+4),*(f32*)(p+8),*(f32*)(p+0xC));PSMTXScale(scale,*(f32*)(p+0x1C),*(f32*)(p+0x1C),*(f32*)(p+0x1C));PSMTXConcat(mtx,scale,mtx);PSMTXConcat(base,mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);color=*(u32*)(p+0x28);GXSetTevColor(1,&color);effSetVtxDescN64((void*)0x803A9000);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}

void effStardustN64SetColor(void* effect, f32 r, f32 g, f32 b) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x28) = r;
    *(f32*)((s32)work + 0x2C) = g;
    *(f32*)((s32)work + 0x30) = b;
}

void effStardustN64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x3C) = camId;
}

void effStardustN64SetScale(void* effect, f32 scale) {
    *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = scale;
}
