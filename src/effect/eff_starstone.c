#include "effect/eff_starstone.h"

void effStarStoneDraw(void* camera, void* effect) {
    extern void effStarStoneDisp_1(void* camera, void* effect);
    extern void effStarStoneDisp_2(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 6:
            effStarStoneDisp_1(camera, effect);
            break;
        default:
            effStarStoneDisp_2(camera, effect);
            break;
    }
}


/* CHATGPT STUB FILL: main/effect/eff_starstone 20260624_184128 */

/* stub-fill: effStarStoneDisp_2 | prototype_only | source_prototype */
void effStarStoneDisp_2(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void mapSetMaterialFog(void);
    extern void GXSetArray(s32, void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXCallDisplayList(void*, u32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void* diamond_2_2_dl_0_tbl[];
    extern void* diamond_2_2_dl_1_tbl[];
    extern u8 diamond_2_2_dl_0_size_tbl[];
    extern u8 diamond_2_2_dl_1_size_tbl[];
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* camera = camGetPtr(cameraId);
    u8 texObj[0x20];
    Mtx trans, scale, rot;
    s32 color = *(s32*)(work + 0x5C);
    s32 pass;
    s32 i;

    mapSetMaterialFog();
    GXSetArray(11, (void*)(0x803AF400 + *(s32*)work * 4), 4);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetChanMatColor(4, work + 0x30);
    GXSetNumTexGens(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 2, 0, 0);
    GXSetTevAlphaIn(1, 0, 5, 0, 7);
    GXSetTevColor(1, &color);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    GXSetCullMode(2);

    for (pass = 0; pass < 2; pass++) {
        f32 x = *(f32*)(work + 8) + *(f32*)(work + (pass ? 0x50 : 0x48));
        f32 z = *(f32*)(work + 0x10) + *(f32*)(work + (pass ? 0x54 : 0x4C));
        f32 size = 1.1f * *(f32*)(work + 0x14);
        PSMTXTrans(trans, x, *(f32*)(work + 0xC), z);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * (pass ? -*(f32*)(work + 0x20) : *(f32*)(work + 0x20)), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * (pass ? -*(f32*)(work + 0x18) : *(f32*)(work + 0x18)), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 11);
        GXSetArray(9, (void*)0x803AF500, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x803AF5C0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 14);
        GXSetArray(13, (void*)0x803AF660, 4);
        for (i = 0; i < 19; i++) {
            if ((*(u16*)(work + 4) & 1) || i != 0) {
                GXCallDisplayList(pass ? diamond_2_2_dl_1_tbl[i] : diamond_2_2_dl_0_tbl[i],
                    (pass ? diamond_2_2_dl_1_size_tbl[i] : diamond_2_2_dl_0_size_tbl[i]) << 5);
            }
        }
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)(work + 0x24), *(f32*)(work + 0x28), 0.0f);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    effGetTexObj(*(s32*)work == 2 ? 0x2E : 0x29, texObj);
    GXLoadTexObj(texObj, 0);
}

/* stub-fill: effStarStoneDisp_1 | prototype_only | source_prototype */
void effStarStoneDisp_1(void* camera, void* effect) {
    extern void mapSetMaterialFog(void);
    extern void effGetTexObj(void*, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXBegin(s32, s32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8 tex[64];
    f32 trans[3][4], scale[3][4], rot[3][4], temp[3][4], model[3][4];
    s32 pass;
    mapSetMaterialFog();
    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0x79, *(f32*)(work + 0x1C));
    PSMTXScale(scale, *(f32*)(work + 0x14) * 1.1f, *(f32*)(work + 0x14) * 1.1f, *(f32*)(work + 0x14) * 1.1f);
    PSMTXConcat(trans, rot, temp);
    PSMTXConcat(temp, scale, model);
    GXLoadPosMtxImm(model, 0);
    for (pass = 0; pass < 2; pass++) {
        s32 texId = pass == 0 ? (*(s32*)work == 0 ? 0x2E : 0x2F) : 0x2D;
        effGetTexObj(tex, texId, 0);
        GXLoadTexObj(tex, 0);
        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000=-1.0f; *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=0.0f;
        *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=0.0f;
        *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=-1.0f; *(volatile f32*)0xCC008000=0.0f;
        *(volatile f32*)0xCC008000=-1.0f; *(volatile f32*)0xCC008000=-1.0f; *(volatile f32*)0xCC008000=0.0f;
    }
}

/* stub-fill: effStarStoneMain | missing_definition | ghidra_signature */
u8 effStarStoneMain(s32 effEntry) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern void effDelete(void* effect);
    extern void effSoftDelete(void* effect);
    extern void* camGetPtr(s32 cameraId);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* effect, f32 order);
    extern void effStarStoneDisp_1(void);
    extern void effStarStoneDisp_2(void);
    extern u32 psndSFXOn_3D(const char* name, VecLocal* pos);
    extern void psndSFXOff(u32 id);
    extern void psndSFX_pos(u32 id, VecLocal* pos);
    extern double sin(double angle);
    extern double cos(double angle);
    extern char str_SFX_EVT_STARSTONE_SH_802fe6a4[];
    extern char str_SFX_EVT_STARSTONE_JU_802fe6c0[];
    u8* effect = (u8*)effEntry;
    u8* work = *(u8**)(effect + 0xC);
    VecLocal pos;
    s32 type = *(s32*)(work + 0x0);
    u16 flags = *(u16*)(work + 0x4);
    s32 phase = *(s32*)(work + 0x3C);
    s32 cameraId = *(s32*)(work + 0x38);
    s32 timer;
    f32 angle;
    void* camera;

    pos.x = *(f32*)(work + 0x8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        if (*(void**)(work + 0x60) != 0) {
            effSoftDelete(*(void**)(work + 0x60));
        }
        if ((flags & 8) != 0) {
            psndSFXOff(*(u32*)(work + 0x58));
        }
        effDelete(effect);
        return 0;
    }

    if ((flags & 4) != 0) {
        if ((flags & 8) == 0) {
            flags |= 8;
            *(u32*)(work + 0x58) = psndSFXOn_3D(str_SFX_EVT_STARSTONE_SH_802fe6a4, &pos);
        }
        psndSFX_pos(*(u32*)(work + 0x58), &pos);
    } else if ((flags & 8) != 0) {
        flags &= ~8;
        psndSFXOff(*(u32*)(work + 0x58));
    }
    *(u16*)(work + 0x4) = flags;

    if (phase == 1) {
        *(u16*)(work + 0x4) |= 1;
        *(f32*)(work + 0x40) = 0.4f;
        *(f32*)(work + 0x44) = 0.0f;
        *(s32*)(work + 0x3C) = phase + 1;
    } else if (phase == 0) {
        f32* scroll = (f32*)(work + 0x24);
        *scroll += (type == 6) ? -0.01f : 0.01f;
        if (*scroll < 0.0f) *scroll += 1.0f;
        if (*scroll >= 1.0f) *scroll -= 1.0f;
    } else if (phase < 6) {
        *(f32*)(work + 0xC) += *(f32*)(work + 0x44);
        *(f32*)(work + 0x44) -= *(f32*)(work + 0x40);
        if (phase > 2) {
            camera = camGetPtr(cameraId);
            angle = 6.2832f * (90.0f + *(f32*)((u8*)camera + 0x114)) / 360.0f;
            *(f32*)(work + 0x48) -= (f32)sin(angle);
            *(f32*)(work + 0x4C) -= (f32)cos(angle);
            *(f32*)(work + 0x50) += (f32)sin(angle);
            *(f32*)(work + 0x54) += (f32)cos(angle);
            *(f32*)(work + 0x20) += (90.0f - *(f32*)(work + 0x20)) / 10.0f;
        }
        if (phase > 3) {
            *(f32*)(work + 0x18) += (-90.0f - *(f32*)(work + 0x18)) / 10.0f;
        }
    }

    timer = *(s32*)(work + 0x2C) + 1;
    *(s32*)(work + 0x2C) = timer % 60;
    pos.x = *(f32*)(work + 0x8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    if ((*(u16*)(work + 0x4) & 0x20) == 0) {
        if (type == 0 || type == 6) {
            dispEntry(cameraId, 2, effStarStoneDisp_1, effect, dispCalcZ(&pos));
        } else {
            dispEntry(cameraId, 2, effStarStoneDisp_2, effect, dispCalcZ(&pos));
        }
    }
    return 0;
}

/* stub-fill: effStarStoneEntry | missing_definition | ghidra_signature */
u8 effStarStoneEntry(s64 xPos, s64 yPos, s64 zPos, s64 param_4, s32 param_5) {
    typedef struct GXColorLocal {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColorLocal;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern u8 effStarStoneMain(s32 effEntry);
    extern void* effSnowDustN64Entry(f64 x, f64 y, f64 z, f64 scaleX, f64 scaleY, f64 scaleZ, s32 unk1, s32 unk2, s32 unk3);
    extern void* gpGlobals;
    extern const char str_StarStone_802fe6e0[];
    extern f32 float_0_80427094;
    extern f32 float_1_8042709c;
    extern f32 float_10_804270a4;
    extern f32 float_20_804270b4;
    extern f32 float_255_804270b8;
    extern f32 float_90_804270c0;
    extern f64 float_1p5_804270d4;
    extern f32 edge_color[8][3];
    extern u8 color_tbl[];
    extern u8 dat_80427088;

    void* entry;
    void* work;
    void* dust;
    s32 kind;
    s32 div;
    f32 zero;
    f32 scale;
    GXColorLocal color;
    u8* tbl;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarStone_802fe6e0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x64);
    *(void**)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = (void (*)(void))effStarStoneMain;
    *(u32*)entry |= 2;

    div = param_5 / 7 + (param_5 >> 0x1F);
    kind = param_5 + (div - (div >> 0x1F)) * -7;
    *(s32*)work = kind;
    *(u16*)((s32)work + 4) = 0;
    *(f32*)((s32)work + 8) = (f32)xPos;
    *(f32*)((s32)work + 0xC) = (f32)yPos;
    *(f32*)((s32)work + 0x10) = (f32)zPos;
    zero = float_0_80427094;
    scale = (f32)(float_1p5_804270d4 * (f64)param_4);
    *(f32*)((s32)work + 0x14) = scale;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x24) = zero;
    *(f32*)((s32)work + 0x28) = zero;
    *(s32*)((s32)work + 0x38) = 4;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x4C) = zero;
    *(f32*)((s32)work + 0x48) = zero;
    *(f32*)((s32)work + 0x54) = zero;
    *(f32*)((s32)work + 0x50) = zero;
    *(f32*)((s32)work + 0x58) = float_90_804270c0;
    *(u8*)((s32)work + 0x5C) = 0xFF;
    *(u8*)((s32)work + 0x5D) = 0xFF;
    *(u8*)((s32)work + 0x5E) = 0xFF;
    *(u8*)((s32)work + 0x5F) = 0xFF;
    *(s32*)((s32)work + 0x60) = 0;

    color.r = (u8)(s32)(float_255_804270b8 * edge_color[kind][0]);
    color.g = (u8)(s32)(float_255_804270b8 * edge_color[kind][1]);
    color.a = dat_80427088;
    color.b = (u8)(s32)(float_255_804270b8 * edge_color[kind][2]);
    *(GXColorLocal*)((s32)work + 0x30) = color;

    if (param_5 > 6) {
        *(u16*)((s32)work + 4) |= 2;
    }
    if ((*(u16*)((s32)work + 4) & 2) == 0) {
        tbl = &color_tbl[kind * 3];
        dust = effSnowDustN64Entry((f64)xPos,
                                   -((f64)(float_10_804270a4 * scale) - (f64)yPos),
                                   (f64)zPos,
                                   (f64)(float_20_804270b4 * scale),
                                   (f64)(float_20_804270b4 * scale),
                                   (f64)float_1_8042709c,
                                   2, 4, 0);
        *(void**)((s32)work + 0x34) = dust;
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x4C) = tbl[0];
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x50) = tbl[1];
        *(u32*)((s32)*(void**)((s32)dust + 0xC) + 0x54) = tbl[2];
    }
    *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) |= 4;
    if (param_5 > 6) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    if (*(s32*)((s32)gpGlobals + 0x14) != 0) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    return (u8)entry;
}

