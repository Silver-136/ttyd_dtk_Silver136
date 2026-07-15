#include "effect/eff_ultra_hammer.h"
#include <dolphin/mtx.h>

void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effUltraHammerDisp(void* camera, void* entry);

extern const Vec vec3_8030301c;
extern f32 scroll_tbl[];
extern f32 float_3p5_80428ba0;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effUltraHammerMain(void* entry) {
    void* work = *(void**)((s32)entry + 0xC);
    Vec zpos;
    Vec pos = vec3_8030301c;
    s32 timer;
    s32 i;
    f32 scroll;
    f32 hammerScale;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    timer = *(s32*)((s32)work + 0x20);
    if (timer < 1000) {
        *(s32*)((s32)work + 0x20) = timer - 1;
    }
    if (*(s32*)((s32)work + 0x20) < 0x10) {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x20) << 4;
    }
    if (*(s32*)((s32)work + 0x20) < 0) {
        effDelete(entry);
    } else {
        i = 1;
        work = (void*)((s32)work + 0x2C);
        while (i < *(s32*)((s32)entry + 8)) {
            timer = *(s32*)((s32)work + 0x28);
            if (timer != 0) {
                *(s32*)((s32)work + 0x28) = timer - 1;
            } else if (*(s32*)((s32)work + 0x24) < 0x19) {
                hammerScale = float_3p5_80428ba0;
                scroll = scroll_tbl[*(s32*)((s32)work + 0x24)];
                *(f32*)((s32)work + 0x14) = scroll;
                *(f32*)((s32)work + 0x10) = hammerScale;
                *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x24) + 1;
            } else {
                *(s32*)((s32)work + 0x28) = 0xF;
                *(s32*)((s32)work + 0x24) = 0;
            }
            i++;
            work = (void*)((s32)work + 0x2C);
        }
        dispCalcZ(&zpos);
        dispEntry(4, 2, effUltraHammerDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_ultra_hammer 20260624_185035 */

/* stub-fill: effUltraHammerDisp | prototype_only | source_prototype */
void effUltraHammerDisp(void* cameraArg, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 angle);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void effGetTexObj(s32 textureId, void* textureObject);
    extern void GXLoadTexObj(void* textureObject, s32 mapId);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetTevColor(s32 stage, void* color);
    extern void GXLoadPosMtxImm(f32 matrix[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXLoadTexMtxImm(f32 matrix[3][4], s32 id, s32 type);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXCallDisplayList(void* list, u32 size);
    extern f32 float_deg2rad_80428b8c;
    extern u32 dat_80428b88;
    extern f32 float_0p6_80428b90;
    extern f32 float_neg1_80428b94;
    extern f32 float_1_80428b98;
    extern f32 float_0_80428b9c;
    extern u8 wakka_vertex_tbl[];
    extern u8 wakka_texcoord0_tbl[];
    extern void* wakka_dl_0_tbl[];
    extern u8 wakka_dl_0_size_tbl[];

    u8 textureObject[0x20];
    f32 base[3][4];
    f32 transform[3][4];
    f32 scaleMtx[3][4];
    f32 rotation[3][4];
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* child = work + 0x2C;
    s32 cameraId = (s32)cameraArg;
    void* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 i;

    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x10), *(f32*)(work + 0x10),
                *(f32*)(work + 0x10));
    PSMTXRotRad(rotation, 'y', float_deg2rad_80428b8c *
                -*(f32*)((u8*)camGetPtr(cameraId) + 0x114));
    PSMTXConcat(base, rotation, base);
    PSMTXConcat(base, scaleMtx, transform);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 0);
    GXSetTevAlphaIn(1, 7, 4, 6, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    effGetTexObj(0x75, textureObject);
    GXLoadTexObj(textureObject, 0);
    GXSetCullMode(0);
    {
        u32 color = dat_80428b88;
        ((u8*)&color)[3] = (u8)*(s32*)(work + 0x1C);
        GXSetTevColor(1, &color);
    }

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child += 0x2C) {
        s32 j;
        f32 scale;
        if (*(s32*)(child + 0x28) != 0) {
            continue;
        }
        PSMTXTrans(base, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
        scale = float_0p6_80428b90 * *(f32*)(child + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(base, scaleMtx, base);
        if (type != 0) {
            PSMTXScale(scaleMtx, float_neg1_80428b94, float_1_80428b98,
                        float_1_80428b98);
            PSMTXConcat(base, scaleMtx, base);
        }
        PSMTXConcat(transform, base, base);
        PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), base, base);
        GXLoadPosMtxImm(base, 0);
        GXSetCurrentMtx(0);
        PSMTXTrans(base, *(f32*)(child + 0x14), *(f32*)(child + 0x18),
                    float_0_80428b9c);
        GXLoadTexMtxImm(base, 0x1E, 1);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 10);
        GXSetArray(9, wakka_vertex_tbl, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x8041E0E0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetArray(11, (void*)0x8041E100, 4);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 13);
        GXSetArray(13, wakka_texcoord0_tbl, 4);
        for (j = 0; j < 20; j++) {
            GXCallDisplayList(wakka_dl_0_tbl[j], (u32)wakka_dl_0_size_tbl[j] << 5);
        }
    }
}

/* stub-fill: effUltraHammerEntry | missing_definition | ghidra_signature */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUltraHammerEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effUltraHammerMain(void* entry);
    extern char str_UltraHammer_80303028[];
    extern u32 dat_80303010[];
    extern f32 float_0_80428b9c;
    extern f32 float_neg4_80428ba4;
    extern f32 float_1_80428b98;

    void* entry;
    void* work;
    void* child;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_UltraHammer_80303028;
    *(s32*)((s32)entry + 8) = 4;
    work = __memAlloc(3, 0xB0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effUltraHammerMain;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x20) = 1000;
    } else {
        *(s32*)((s32)work + 0x20) = timer;
    }
    *(s32*)((s32)work + 0x1C) = 0xFF;
    *(s32*)((s32)work + 0x24) = 0;

    child = (void*)((s32)work + 0x2C);
    for (i = 1; i < *(s32*)((s32)entry + 8); i++) {
        *(f32*)((s32)child + 4) = float_0_80428b9c;
        *(f32*)((s32)child + 8) = float_neg4_80428ba4 * (f32)(i - 1);
        *(f32*)((s32)child + 0xC) = float_0_80428b9c;
        *(f32*)((s32)child + 0x10) = float_1_80428b98;
        *(u32*)((s32)child + 0x28) = dat_80303010[i - 1];
        *(s32*)((s32)child + 0x24) = 0;
        *(s32*)((s32)child + 0x20) = 0;
        child = (void*)((s32)child + 0x2C);
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

