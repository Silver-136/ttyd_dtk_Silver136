#include "effect/n64/eff_thunderflare_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderflareMain(void*);
    extern char str_ThunderflareN64_802fc1d0[];
    extern f32 float_0_80426408;
    void* entry;
    u8* work;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderflareN64_802fc1d0;
    *(s32*)((s32)entry + 8) = 0x1F;
    work = __memAlloc(3, 0x8B8);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderflareMain;
    *(s32*)(work + 4) = type;
    *(s32*)(work + 0x1C) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x18) = 0x64;
    } else {
        *(s32*)(work + 0x18) = lifetime;
    }
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (type == 1) {
        goto color_other;
    }
    if (type >= 1) {
        goto color_other;
    }
    if (type >= 0) {
        goto color_zero;
    }
    goto color_other;
color_zero:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x38) = 0xFF;
    *(s32*)(work + 0x3C) = 0xFF;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    goto color_done;
color_other:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0x14;
    *(s32*)(work + 0x38) = 0x21;
    *(s32*)(work + 0x3C) = 0x32;
    *(s32*)(work + 0x40) = 0x14;
    *(s32*)(work + 0x44) = 0xA;
color_done:
    part = work + 0x48;
    for (i = 1; i < 0x1F; i += 2, part += 0x90) {
        *(f32*)(part + 0x20) = (f32)(((i - 1) * 0xCA8) / 0xF);
        *(f32*)(part + 0x24) = float_0_80426408;
        *(f32*)(part + 0x2C) = (f32)(((i - 1) * 0x64) / 0xF);
        *(s32*)part = 0;
        *(f32*)(part + 0x68) = (f32)((i * 0xCA8) / 0xF);
        *(f32*)(part + 0x6C) = float_0_80426408;
        *(f32*)(part + 0x74) = (f32)((i * 0x64) / 0xF);
        *(s32*)(part + 0x48) = 0;
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset



void effThunderflareMain(void* effect) {
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effThunderflareDisp(void);
    extern f32 float_0_80426408;
    extern f32 float_0p5_8042640c;
    extern f32 float_6p2832_80426410;
    extern f32 float_360_80426414;
    extern f32 float_2_80426418;
    extern f32 float_255_8042641c;
    extern f32 float_100_80426420;
    extern f32 float_0p2_80426424;
    extern f32 float_80_80426428;
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* particle = work;
    f32 position[3];
    s32 frame;
    s32 odd;
    s32 i;

    position[0] = *(f32*)(work + 8);
    position[1] = *(f32*)(work + 0xC);
    position[2] = *(f32*)(work + 0x10);
    if (*(s32*)(work + 0x18) < 100) {
        *(s32*)(work + 0x18) -= 1;
    }
    *(s32*)(work + 0x1C) += 1;
    if (*(s32*)(work + 0x18) < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x1C);
    if (frame < 0x15) {
        *(s32*)(work + 0x14) = frame * 12;
    } else {
        *(s32*)(work + 0x14) = 0xFF;
    }
    if (frame & 1) {
        *(s32*)(work + 0x14) =
            (s32)(float_0p5_8042640c * (f32)*(s32*)(work + 0x14));
    }
    odd = frame & 1;

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, particle += 0x48) {
        f32 radius = *(f32*)(particle + 0x74);
        f32 angleA = float_6p2832_80426410 * *(f32*)(particle + 0x68) /
                     float_360_80426414;
        f32 angleB = float_6p2832_80426410 * *(f32*)(particle + 0x6C) /
                     float_360_80426414;
        f32 sinA = (f32)sin(angleA);
        f32 cosA = (f32)cos(angleA);
        f32 sinB = (f32)sin(angleB);
        f32 cosB = (f32)cos(angleB);
        s32 stepA;
        s32 stepB;

        *(f32*)(particle + 0x50) = cosB * radius * sinA;
        *(f32*)(particle + 0x54) = cosB * radius * cosA;
        *(f32*)(particle + 0x58) = radius * sinB;
        *(s32*)(particle + 0x5C) =
            (s32)((float_2_80426418 * float_255_8042641c *
                   (float_100_80426420 - radius)) / float_100_80426420);
        if (*(s32*)(particle + 0x5C) > 0xFF) {
            *(s32*)(particle + 0x5C) = 0xFF;
        }
        *(f32*)(particle + 0x70) =
            float_0p2_80426424 + radius / float_80_80426428;
        radius -= (f32)(((i & 3) * 2) + 2);
        *(f32*)(particle + 0x74) = radius;
        if (radius < float_0_80426408) {
            *(f32*)(particle + 0x74) = float_0_80426408;
            *(f32*)(particle + 0x70) = float_0_80426408;
        }
        stepA = odd ? 5 : 2;
        stepB = odd ? 2 : 5;
        *(f32*)(particle + 0x68) += (f32)stepA;
        *(f32*)(particle + 0x6C) += (f32)stepB;
    }
    dispEntry(4, 2, effThunderflareDisp, effect, dispCalcZ(position));
}

void effThunderflareDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetCullMode(s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32);
    extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* work=*(u8**)((s32)effect+0xC); f32 base[3][4],rot[3][4],mtx[3][4],scale[3][4]; u8 tex[0x20]; s32 i;
    PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10));
    PSMTXRotRad(rot,0x79,-0.017453292f**(f32*)((s32)camGetPtr(4)+0x114)); PSMTXConcat(base,rot,base); PSMTXConcat((u8*)camGetPtr(cameraId)+0x11C,base,base);
    GXSetNumChans(0); GXSetNumTevStages(2); GXSetTevOrder(0,0,0,0xFF); GXSetTevOrder(1,0xFF,0xFF,0xFF); GXSetCullMode(0); effGetTexObjN64(0x89,tex); GXLoadTexObj(tex,0);
    for(i=1;i<*(s32*)((s32)effect+8);i++){u8* part=work+0x48+(i-1)*0x90; PSMTXTrans(mtx,*(f32*)(part+0x40),*(f32*)(part+0x44),*(f32*)(part+0x48)); PSMTXScale(scale,*(f32*)(part+0x70),*(f32*)(part+0x70),*(f32*)(part+0x70)); PSMTXConcat(mtx,scale,mtx); PSMTXConcat(base,mtx,mtx); GXLoadPosMtxImm(mtx,0); GXSetCurrentMtx(0); effSetVtxDescN64((void*)0x803AB8E0); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);}
}
