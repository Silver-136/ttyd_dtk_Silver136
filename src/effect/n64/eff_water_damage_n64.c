#include "effect/n64/eff_water_damage_n64.h"


void* effWaterDamageN64Entry(f64 x, f64 y, f64 z, f64 scale, s32 type, s32 timer) {
    typedef struct WaterPart {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        f32 vx;
        f32 vy;
        f32 vz;
        s32 timer;
        s32 frame;
        s32 r;
        s32 g;
        s32 b;
        s32 alpha;
        f32 size;
        s32 altDisp;
    } WaterPart;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effWaterDamageMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_WaterDamageN64_802fc280[];
    void* entry;
    WaterPart* work;
    s32 count;
    s32 altDisp;
    s32 i;

    entry = effEntry();
    altDisp = type > 9;
    if (altDisp) {
        type %= 10;
    }
    if (type == 2) {
        count = 0x29;
    } else if (type == 0 || type == 1) {
        count = 0x15;
    } else if (type == 6) {
        count = 9;
    } else {
        count = 0xB;
    }
    *(char**)((s32)entry + 0x14) = str_WaterDamageN64_802fc280;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * sizeof(WaterPart));
    *(WaterPart**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effWaterDamageMain;
    *(u32*)entry |= 2;

    work[0].type = type;
    work[0].frame = 0;
    work[0].timer = timer < 1 ? 1000 : timer;
    work[0].alpha = 0xFF;
    work[0].x = (f32)x;
    work[0].y = (f32)y;
    work[0].z = (f32)z;
    work[0].size = (f32)scale;
    work[0].r = 0x46;
    work[0].g = 0xB4;
    work[0].b = 0xFF;
    work[0].altDisp = altDisp;

    for (i = 1; i < count; i++) {
        WaterPart* part = &work[i];
        part->x = 0.0f;
        part->y = 0.0f;
        part->z = 0.0f;
        part->size = 2.0f;
        switch (type) {
            case 0:
                part->vx = (f32)((rand() % 11) - 2);
                part->vy = (f32)((rand() % 11) - 5);
                part->vz = (f32)((rand() % 11) - 5);
                break;
            case 1:
                part->vx = -(f32)((rand() % 11) - 2);
                part->vy = (f32)((rand() % 11) - 5);
                part->vz = (f32)((rand() % 11) - 5);
                break;
            case 2:
                part->vx = 0.07f * (f32)((rand() % 100) - 50);
                part->vy = 0.2f * (f32)((rand() % 100) + 10);
                part->vz = 0.07f * (f32)((rand() % 100) - 50);
                break;
            case 3:
                part->vx = 0.5f * (f32)((rand() % 10) - 5);
                part->vy = 0.5f * (f32)((rand() % 10) - 5);
                part->vz = 0.5f * (f32)((rand() % 10) - 5);
                part->size = 0.4f;
                part->timer = i;
                break;
            case 6:
            {
                f32 angle = 6.2832f * (f32)i / (f32)count;
                part->x = 10.0f * (f32)sin(angle);
                part->z = 10.0f * (f32)cos(angle);
                part->vx = part->x / 12.0f;
                part->vy = 1.0f + (f32)(rand() % 10) * 0.1f;
                part->vz = part->z / 12.0f;
                part->size = 1.0f;
                break;
            }
            default:
                part->vx = 0.3f * (f32)((rand() % 10) - 5);
                part->vy = 0.3f * (f32)((rand() % 10) + 5);
                part->vz = 0.3f * (f32)((rand() % 10) - 5);
                part->size = 1.0f;
                part->timer = i;
                break;
        }
    }
    return entry;
}

u8 effWaterDamageDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXSetBlendMode(s32,s32,s32,s32);extern void GXSetNumChans(s32);extern void GXSetNumTevStages(s32);extern void GXSetNumTexGens(s32);extern void GXSetCullMode(s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXBegin(s32,s32,s32);
    u8* w=*(u8**)((s32)effect+0xC);f32 a[3][4],b[3][4],m[3][4];PSMTXTrans(a,(f32)*(s32*)(w+4),(f32)*(s32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(b,0x79,-0.017453292f**(f32*)((s32)camGetPtr(4)+0x114));PSMTXConcat(a,b,m);GXSetBlendMode(1,4,5,0);GXSetNumChans(0);GXSetNumTevStages(3);GXSetNumTexGens(2);GXSetCullMode(0);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);GXBegin(0x90,0,6);return 0;
}

u8 effWaterDamageDisp2(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXSetBlendMode(s32,s32,s32,s32);extern void GXSetNumChans(s32);extern void GXSetNumTevStages(s32);extern void GXSetCullMode(s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXBegin(s32,s32,s32);
    u8* w=*(u8**)((s32)effect+0xC);f32 a[3][4],b[3][4],m[3][4];PSMTXTrans(a,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(b,0x79,-0.017453292f**(f32*)((s32)camGetPtr(4)+0x114));PSMTXConcat(a,b,m);GXSetBlendMode(1,4,5,0);GXSetNumChans(0);GXSetNumTevStages(2);GXSetCullMode(0);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);GXBegin(0x90,0,6);return 0;
}

void effWaterDamageMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effWaterDamageDisp(void);
    extern void effWaterDamageDisp2(void);
    extern Vec3 vec3_802fc268[];
    extern f32 float_0p98_804264e8;
    extern f32 float_neg0p8_804264ec;
    extern f32 float_neg0p1_804264f0;
    extern f32 float_1p2_804264f4;
    extern f32 float_0p9_804264f8;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 type;
    s32 altDisp;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc268;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    type = *(s32*)work;
    altDisp = *(s32*)(work + 0x38);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 1000) {
        *(s32*)(work + 0x1C) -= 1;
    }
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 4) {
        *(s32*)(work + 0x30) = *(s32*)(work + 0x1C) << 6;
    }
    part = work + 0x3C;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x3C) {
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(s32*)(part + 0x30) = (s32)((f32)*(s32*)(part + 0x30) * float_0p98_804264e8);
        if (type == 2) {
            *(f32*)(part + 0x14) += float_neg0p8_804264ec;
        }
        *(f32*)(part + 0x14) += float_neg0p1_804264f0;
        *(f32*)(part + 0x10) *= float_0p98_804264e8;
        *(f32*)(part + 0x14) *= float_0p98_804264e8;
        *(f32*)(part + 0x18) *= float_0p98_804264e8;
        if (type == 3) {
            *(f32*)(part + 0x34) *= float_1p2_804264f4;
        } else {
            *(f32*)(part + 0x34) *= float_0p9_804264f8;
        }
    }
    if (altDisp == 0) {
        dispEntry(4, 8, effWaterDamageDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(4, 8, effWaterDamageDisp2, effect, dispCalcZ(&dispPos));
    }
}

