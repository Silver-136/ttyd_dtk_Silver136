#include "effect/n64/eff_thunder_n64.h"


u8 effThunderDisp(s32 cameraId, s32 effectAddress) {
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
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, model;
    s32 pass;
    s32 counts[6] = { 0x30, 0x120, 0x9C, 0xE4, 0x36, 0x3C };

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)((char*)camGetPtr(4) + 0x114), 'y');
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(camera + 0x118, model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(*(s32*)work < 3 ? 1 : 0);
    GXSetNumTevStages(*(s32*)work < 3 ? 2 : 4);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    for (pass = 0; pass < 4; pass++) {
        GXSetTevOrder(pass, pass == 0 ? 0 : 1, pass == 0 ? 0 : 1, pass == 3 ? 4 : -1);
    }
    effGetTexObj(0x6E, &tex);
    GXLoadTexObj(&tex, 0);
    GXLoadTexObj(&tex, 1);
    GXSetCullMode(0);
    for (pass = 0; pass < 6; pass++) {
        GXBegin(0x90, 0, counts[pass]);
    }
    return 0;
}

u8 effThunderMain(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderMain(void*);
    extern void* effThunderflareN64Entry(s32 type, s32 unk, f32 x, f32 y, f32 z, f32 scale);
    extern void* effAkariChargeN64Entry(s32 type, s32 unk, f32 x, f32 y, f32 z, f32 scale);
    extern void* effShockN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy);
    extern char str_ThunderN64_802fc1a8[];
    extern void* gp;
    extern f32 float_0_804263c0;
    extern f32 float_48_804263d8;
    extern f32 float_32_804263dc;
    extern f32 float_8_804263e0;
    extern f32 float_neg118_804263e4;
    extern f32 float_184_804263e8;
    extern f32 float_79_804263ec;
    extern f32 float_1_804263c4;
    extern f32 float_neg120_804263f0;
    extern f32 float_189_804263f4;
    extern f32 float_72_804263f8;
    void* entry;
    u8* work;
    s32 shockType;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderN64_802fc1a8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x4C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x18) = 0;
    if (type >= 3) {
        *(s32*)(work + 0x14) = 0x1E;
    } else {
        *(s32*)(work + 0x14) = 0x64;
    }
    *(s32*)(work + 0x10) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x20) = float_0_804263c0;
    *(f32*)(work + 0x1C) = float_0_804263c0;
    *(f32*)(work + 0x2C) = float_48_804263d8;
    *(f32*)(work + 0x30) = float_32_804263dc;
    *(f32*)(work + 0x28) = float_0_804263c0;
    *(f32*)(work + 0x24) = float_0_804263c0;
    *(f32*)(work + 0x34) = float_8_804263e0;
    *(f32*)(work + 0x38) = float_32_804263dc;
    *(s32*)(work + 0x3C) = 4;
    *(s32*)(work + 0x40) = 2;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x44) = 0;
    if (type == 1 || type == 2) {
        if (type == 1) {
            *(void**)(work + 0x44) = effThunderflareN64Entry(0, -1, float_neg118_804263e4, float_184_804263e8, float_79_804263ec, float_1_804263c4);
        }
        if (type != 1) {
            *(void**)(work + 0x48) = effAkariChargeN64Entry(0, 0, float_neg120_804263f0, float_189_804263f4, float_72_804263f8, float_1_804263c4);
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x28) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x2C) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x30) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x18) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x1C) = 0;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x20) = 0xFF;
        }
    }
    if (type >= 3) {
        shockType = 1;
        if (*(s32*)((s32)gp + 0x14) == 1) {
            shockType = 3;
        }
        effShockN64Entry(shockType, x, y, z, sx, sy);
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

