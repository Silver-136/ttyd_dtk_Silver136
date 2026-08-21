#include "effect/n64/eff_kumokumo_chip_n64.h"
#include "driver/effdrv.h"
#include "memory.h"

void effKumokumoChipMain(void* effect);
void effKumokumoChipDisp(s32 cameraId, void* effect);


void* effKumokumoChipN64Entry(
    f32 x0, f32 y0, f32 z0,
    f32 x1, f32 y1, f32 z1,
    f32 step, f32 scale, s32 type)
{
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern char str_KumokumoChipN64_802fb384[];

    void* entry;
    u8* work;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;
    f32 lenSq;
    s32 count;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_KumokumoChipN64_802fb384;
    *(s32*)((s32)entry + 8) = 1;

    work = __memAlloc(3, 0x48);

    dy = y1 - y0;
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKumokumoChipMain;

    dx = x1 - x0;
    dz = z1 - z0;

    *(u32*)entry |= 2;

    lenSq = dz * dz + dx * dx + dy * dy;

    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;

    len = lenSq;

    if (lenSq != 0.0f) {
        f64 d;

        d = (f64)lenSq;

        if (d > 0.0) {
            f64 inv;

            inv = __frsqrte(d);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            len = (f32)(
                d * 0.5 * inv *
                -(d * inv * inv - 3.0));
        } else if (d < 0.0) {
            len = __float_nan;
        } else {
            u32 bits;
            u32 kindBits;
            s32 kind;

            bits = *(u32*)&lenSq;
            kindBits = bits & 0x7F800000;

            if (kindBits == 0x7F800000) {
                if ((bits & 0x7FFFFF) == 0) {
                    kind = 2;
                } else {
                    kind = 1;
                }
            } else if ((kindBits < 0x7F800000) &&
                       (kindBits == 0)) {
                if ((bits & 0x7FFFFF) == 0) {
                    kind = 3;
                } else {
                    kind = 5;
                }
            } else {
                kind = 4;
            }

            if (kind == 1) {
                len = __float_nan;
            } else {
                len = lenSq;
            }
        }
    }

    if (step > 0.0f) {
        count = (s32)(len / step);
    } else {
        count = 0;
    }

    if (count != 0) {
        f32 invCount;

        invCount = 1.0f / (f32)count;
        *(f32*)(work + 0x10) = dx * invCount;
        *(f32*)(work + 0x14) = dy * invCount;
        *(f32*)(work + 0x18) = dz * invCount;
    } else {
        *(f32*)(work + 0x10) = 0.0f;
        *(f32*)(work + 0x14) = 0.0f;
        *(f32*)(work + 0x18) = 0.0f;
    }

    if (count < 0) {
        *(s32*)(work + 0x1C) = 1000;
    } else {
        *(s32*)(work + 0x1C) = count;
    }

    *(s32*)(work + 0x30) = 0;
    *(f32*)(work + 4) = x0;
    *(f32*)(work + 8) = y0;
    *(f32*)(work + 0xC) = z0;
    *(f32*)(work + 0x44) = scale;

    switch (type) {
    case 0:
    case 2:
        *(s32*)(work + 0x24) = 0xD7;
        *(s32*)(work + 0x28) = 0xD7;
        *(s32*)(work + 0x2C) = 0xB9;
        *(s32*)(work + 0x34) = 0xD7;
        *(s32*)(work + 0x38) = 200;
        *(s32*)(work + 0x3C) = 100;
        *(s32*)(work + 0x40) = 0xFF;
        break;

    default:
        *(s32*)(work + 0x24) = 0xFF;
        *(s32*)(work + 0x28) = 0xFF;
        *(s32*)(work + 0x2C) = 0xFF;
        *(s32*)(work + 0x34) = 0xDC;
        *(s32*)(work + 0x38) = 0xF5;
        *(s32*)(work + 0x3C) = 0xFF;
        *(s32*)(work + 0x40) = 0xFF;
        break;
    }

    return entry;
}

void effKumokumoChipMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern const Vec3 vec3_802fb378;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    s32 timer;
    s32 frame;
    s32 flags;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb378;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    flags = *(s32*)effect;
    type = *(s32*)work;
    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    timer = *(s32*)(work + 0x1C);
    if (timer < 0x3E8) {
        *(s32*)(work + 0x1C) = timer - 1;
    }
    *(s32*)(work + 0x20) += 1;
    timer = *(s32*)(work + 0x1C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x20);
    if (type == 0 || type == 2) {
        if (frame < 0x10) {
            *(s32*)(work + 0x30) = (frame << 4) + 0xF;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 0x10) {
            *(s32*)(work + 0x30) = timer << 4;
        }
    } else {
        if (frame < 4) {
            *(s32*)(work + 0x30) = (frame << 6) + 0x3F;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 4) {
            *(s32*)(work + 0x30) = timer << 6;
        }
    }

    *(f32*)(work + 4) += *(f32*)(work + 0x10);
    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
    dispEntry(4, 2, effKumokumoChipDisp, effect, dispCalcZ(&dispPos));
}

void effKumokumoChipDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32,void*); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_0p03125_804258e0; extern f32 float_0_804258e4;
    u8* work=*(u8**)((u8*)effect+0xC);u8* camera=(u8*)camGetPtr(cameraId);u8 tex[0x20];Mtx model,scale;u32 c1,c2;
    PSMTXTrans(model,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXScale(scale,*(f32*)(work+0x44),*(f32*)(work+0x44),*(f32*)(work+0x44));PSMTXConcat(model,scale,model);PSMTXConcat((f32(*)[4])(camera+0x11C),model,model);GXLoadPosMtxImm(model,0);GXSetCurrentMtx(0);
    c1=((u32)*(u8*)(work+0x24)<<24)|((u32)*(u8*)(work+0x28)<<16)|((u32)*(u8*)(work+0x2C)<<8)|*(u8*)(work+0x30);c2=((u32)*(u8*)(work+0x34)<<24)|((u32)*(u8*)(work+0x38)<<16)|((u32)*(u8*)(work+0x3C)<<8)|*(u8*)(work+0x40);GXSetTevColor(1,&c1);GXSetTevColor(2,&c2);
    effGetTexObjN64(0x6B,tex);GXLoadTexObj(tex,0);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_804258e0,float_0p03125_804258e0,float_0_804258e4);GXLoadTexMtxImm(scale,0x1E,1);
    GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,4,2,8,15);GXSetTevAlphaIn(0,7,5,4,7);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A51D8);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3);
}

static u8 size32x32_tex32x32_vtx[56] = {
    0xFF, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF,
    0, 0x10, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0xFF,
    0, 0x10, 0, 0x20, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0xFF,
    0xFF, 0xF0, 0, 0x20, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0xFF,
};
