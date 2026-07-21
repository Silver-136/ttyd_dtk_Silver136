#include "effect/n64/eff_pokopi_pcharge_n64.h"


void* effPokopiPchargeN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 lifetime) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effPokopiPchargeMain(void*);
    extern s32 rand(void);
    extern char str_PokopiPchargeN64_802fbcd8[];
    extern u8 n_data[];
    extern u8 vx_data[];
    void* entry = effEntry();
    u8* base;
    u8* part;
    s32 count = n_data[type];
    s32 i;
    s32 r;
    s32 baseAngle;
    f32 direction;

    *(char**)((s32)entry + 0x14) = str_PokopiPchargeN64_802fbcd8;
    *(s32*)((s32)entry + 8) = count;
    base = __memAlloc(3, count * 0x48);
    *(u8**)((s32)entry + 0xC) = base;
    *(void**)((s32)entry + 0x10) = effPokopiPchargeMain;
    *(s32*)entry |= 2;
    *(s32*)base = type;
    *(f32*)(base + 4) = x;
    *(f32*)(base + 8) = y;
    *(f32*)(base + 0xC) = z;
    *(s32*)(base + 0x10) = lifetime < 1 ? 1000 : lifetime;
    *(s32*)(base + 0x14) = 0;
    *(s32*)(base + 0x18) = 0xFF;
    *(s32*)(base + 0x1C) = 0xFF;
    *(s32*)(base + 0x20) = 0xFF;
    *(s32*)(base + 0x24) = 0;
    *(s32*)(base + 0x28) = 0xFF;
    *(s32*)(base + 0x2C) = 0xF5;
    *(s32*)(base + 0x30) = 0;
    *(f32*)(base + 0x34) = scale;
    *(f32*)(base + 0x38) = (f32)vx_data[type];
    *(f32*)(base + 0x44) = 1.0f;

    baseAngle = rand() % 360;
    part = base + 0x48;
    for (i = 1; i < count; i++, part += 0x48) {
        r = rand() % 10;
        *(s32*)part = type % 3;
        *(s32*)(part + 0x14) = 0;
        *(f32*)(part + 0x3C) = (f32)(rand() % 360);
        *(f32*)(part + 0x40) = 0.0f;
        direction = 1.0f - 0.3f * (f32)(i % 3);
        *(f32*)(part + 0x34) = direction;
        switch (type) {
        case 0:
        case 1:
        case 2:
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) = 1.0f - 0.1f * (f32)(i % 3);
            *(f32*)(part + 0x38) = -(0.2f * (f32)r + 0.2f);
            *(f32*)(part + 0x40) = 0.0f;
            break;
        case 3:
        case 5:
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) = 1.0f - 0.1f * (f32)(i % 3);
            *(f32*)(part + 0x38) = 0.2f * (f32)r + 0.2f;
            *(f32*)(part + 0x40) = 0.0f;
            break;
        case 4:
        case 12:
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) = 1.0f - 0.3f * (f32)(i % 3);
            *(f32*)(part + 0x38) = 0.2f * (f32)r + 0.6f;
            *(f32*)(part + 0x40) = -2.0f;
            break;
        case 6:
        case 7:
            *(s32*)(part + 0x10) = 10;
            *(f32*)(part + 0x34) = 1.0f - 0.3f * (f32)(i % 3);
            *(f32*)(part + 0x38) = -(f32)(r + 4);
            *(f32*)(part + 0x40) = 0.0f;
            break;
        case 8:
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) = 1.0f - 0.3f * (f32)(i % 3);
            *(f32*)(part + 0x38) = 0.2f * (f32)r + 0.6f;
            *(f32*)(part + 0x40) = -1.0f;
            break;
        case 9:
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) = 1.0f - 0.3f * (f32)(i % 3);
            *(f32*)(part + 0x38) = -(0.2f * (f32)r + 0.6f);
            *(f32*)(part + 0x40) = -2.0f;
            *(s32*)(base + 0x18) = 0xFF;
            *(s32*)(base + 0x1C) = 0xF5;
            *(s32*)(base + 0x20) = 0xD7;
            *(s32*)(base + 0x28) = 0xDC;
            *(s32*)(base + 0x2C) = 0x3C;
            *(s32*)(base + 0x30) = 10;
            break;
        case 10:
            *(s32*)(part + 0x10) = 10;
            *(f32*)(part + 0x34) = -0.5f * (0.3f * (f32)(i % 3) - 1.0f);
            *(f32*)(part + 0x38) = 0.1f * (f32)r + 2.0f;
            *(f32*)(part + 0x40) = 0.0f;
            break;
        case 11:
            *(s32*)(part + 0x10) = 30;
            *(f32*)(part + 0x34) = 1.0f - 0.3f * (f32)(i % 3);
            *(f32*)(part + 0x38) = 0.05f * (f32)r + 0.2f;
            *(f32*)(part + 0x40) = -1.0f;
            break;
        }

        r = rand();
        r = r - (r / *(s32*)(part + 0x10)) * *(s32*)(part + 0x10);
        *(s32*)(part + 0x14) = -r;
        if (*(f32*)(part + 0x38) >= 1.0f) {
            *(f32*)(part + 4) = (f32)-*(s32*)(part + 0x14) * *(f32*)(part + 0x38);
        } else {
            *(f32*)(part + 4) = -*(f32*)(part + 0x38) *
                (f32)(*(s32*)(part + 0x10) - *(s32*)(part + 0x14));
        }
        *(f32*)(part + 8) = 1.0f;
        *(f32*)(part + 0xC) = 1.0f;
        *(f32*)(part + 0x3C) = (f32)baseAngle +
            (360.0f * (f32)(i - 1)) / (f32)(count - 1);
    }
    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiPchargeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiPchargeDisp(void);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern const Vec3 vec3_802fbcc0;
    extern f64 double_to_int_802fbcd0;
    extern f32 float_0_80425d2c;
    extern f32 float_180_80425d34;
    extern f32 float_6p2832_80425d38;
    extern f32 float_360_80425d3c;
    extern f32 float_255_80425d40;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 frame;
    s32 limit;
    f32 angle;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbcc0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 0x3E8) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x10) << 4;
    }
    if (*(s32*)(work + 0x14) < 0x10) {
        *(s32*)(work + 0x24) = (*(s32*)(work + 0x14) << 4) + 0xF;
    }

    zero = float_0_80425d2c;
    p = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, p += 0x48) {
        *(s32*)(p + 0x14) += 1;
        frame = *(s32*)(p + 0x14);
        limit = *(s32*)(p + 0x10);
        if (frame >= limit) {
            *(s32*)(p + 0x14) = 0;
            if (*(f32*)(p + 0x38) < zero) {
                *(f32*)(p + 4) = (f32)(-limit) * *(f32*)(p + 0x38);
            } else {
                *(f32*)(p + 4) = zero;
            }
            *(f32*)(p + 0x3C) = (f32)(rand() % 0x168);
        }

        *(f32*)(p + 4) += *(f32*)(p + 0x38);
        *(f32*)(p + 0x3C) += *(f32*)(p + 0x40);
        angle = (f32)((*(s32*)(p + 0x14) * 0xB4) / *(s32*)(p + 0x10));
        if (angle < zero || angle > float_180_80425d34) {
            *(s32*)(p + 0x24) = 0;
        } else {
            *(s32*)(p + 0x24) = (s32)(float_255_80425d40 * (f32)sin((float_6p2832_80425d38 * angle) / float_360_80425d3c));
        }
    }

    dispEntry(4, 2, effPokopiPchargeDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effPokopiPchargeDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXSetTevColor(s32,void*);extern void GXSetNumChans(s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetCullMode(s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_deg2rad_80425d20,float_0p03125_80425d24,float_0p0078125_80425d28,float_0_80425d2c,float_1_80425d30;f32 trans[3][4],rot[3][4],scale[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);void* camera=camGetPtr(cameraId);u32 color=*(u32*)(work+0x18);s32 i;
    PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x79,float_deg2rad_80425d20*-*(f32*)((s32)camGetPtr(4)+0x114));PSMTXScale(scale,*(f32*)(work+0x34)*(*(f32*)(work+0x44)),*(f32*)(work+0x34)*(*(f32*)(work+0x44)),*(f32*)(work+0x34)*(*(f32*)(work+0x44)));PSMTXConcat(trans,rot,trans);PSMTXConcat(trans,scale,trans);PSMTXConcat((void*)((s32)camera+0x11C),trans,trans);GXSetTevColor(1,&color);GXSetNumChans(0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,3,2,8,0);GXSetTevAlphaIn(0,0,1,7,7);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80425d24,float_0p0078125_80425d28,float_0_80425d2c);GXLoadTexMtxImm(scale,0x1E,1);effGetTexObjN64(0x86,texObj);GXLoadTexObj(texObj,0);GXSetCullMode(0);
    for(i=1;i<*(s32*)((s32)effect+8);i++,work+=0x48){PSMTXRotRad(rot,0x7A,float_deg2rad_80425d20**(f32*)(work+0x84));PSMTXTrans(scale,*(f32*)(work+0x4C)+*(f32*)(work+0x38),float_0_80425d2c,float_0_80425d2c);PSMTXConcat(rot,scale,scale);PSMTXConcat(trans,scale,scale);GXLoadPosMtxImm(scale,0);GXSetCurrentMtx(0);effSetVtxDescN64((void*)0x803A7100);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}
