#include "effect/eff_indirect.h"

typedef struct EffIndirectVecRaw {
    u32 x;
    u32 y;
    u32 z;
} EffIndirectVecRaw;

void* effEntry(void);
void effDelete(void* entry);
void* __memAlloc(s32 heap, u32 size);
void* smartAlloc(u32 size, s32 flags);
void smartFree(void* ptr);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u32 mipmap, u32 maxLod);
f32 dispCalcZ(void* pos);
void dispEntry(s32 prio, s32 type, void* callback, void* entry, f32 z);
void effIndirectDisp(s32 cameraId, void* effect);
void effIndirectMain(void* entry);

extern void* gp;
extern u32 vec3_80302990[];
extern char str_Indirect_803029a8[];
extern f32 float_0_8042888c;
extern f32 float_neg300_80428890;
extern f32 float_60_80428894;
extern f32 float_neg0p05_80428898;

void effIndirectMain(void* entry) {
    EffIndirectVecRaw dispPos;
    EffIndirectVecRaw pos;
    void* work = *(void**)((s32)entry + 0xC);
    s32 type;
    s32 flags;
    s32 timer;
    f32 fps;

    pos = *(EffIndirectVecRaw*)vec3_80302990;
    *(f32*)&pos.x = *(f32*)((s32)work + 4);
    *(f32*)&pos.y = *(f32*)((s32)work + 8);
    *(f32*)&pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;

    type = *(s32*)((s32)work + 0);
    if (type >= 2) {
        goto display;
    }
    if (type >= 0) {
        goto update;
    }
    goto display;

update:
    flags = *(s32*)entry;
    if (flags & 4) {
        *(s32*)entry = flags & ~4;
        *(s32*)((s32)work + 0x28) = 0x10;
    }

    timer = *(s32*)((s32)work + 0x28);
    if (timer < 1000) {
        *(s32*)((s32)work + 0x28) = timer - 1;
    }

    timer = *(s32*)((s32)work + 0x28);
    if (timer < 0) {
        smartFree(*(void**)((s32)work + 0x34));
        effDelete(entry);
        return;
    }

    if (timer < 0x20) {
        *(u8*)((s32)work + 0x20) = (u8)((timer & 0x1F) << 3);
    }

    fps = (f32)*(s32*)((s32)gp + 4);
    *(f32*)((s32)work + 0x18) += float_neg300_80428890 / fps;
    fps = (f32)*(s32*)((s32)gp + 4);
    *(f32*)((s32)work + 0x1C) += float_60_80428894 / fps;

display:
    dispEntry(7, 2, effIndirectDisp, entry, dispCalcZ(&dispPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effIndirectEntry(s32 type, s32 timer, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;
    void* buffer;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_Indirect_803029a8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effIndirectMain;
    *(u32*)entry |= 2;

    zero = float_0_8042888c;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = zero;
    *(u8*)((s32)work + 0x20) = 0xFF;

    buffer = smartAlloc(GXGetTexBufferSize(0x40, 0x40, 3, 0, 0), 0);
    *(void**)((s32)work + 0x34) = buffer;

    zero = float_0_8042888c;
    *(f32*)((s32)work + 0x10) = zero;
    *(f32*)((s32)work + 0x14) = zero;
    *(f32*)((s32)work + 0x18) = zero;
    *(s32*)((s32)work + 0x28) = timer;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x30) = float_neg0p05_80428898;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_indirect 20260624_184929 */

/* stub-fill: effIndirectDisp | prototype_only | source_prototype */
void effIndirectDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetTexCopySrc(s32, s32, s32, s32);
    extern void GXSetTexCopyDst(s32, s32, s32, s32);
    extern u32 GXGetTexBufferSize(s32, s32, s32, s32, s32);
    extern void GXCopyTex(void*, s32);
    extern void GXPixModeSync(void);
    extern void GXInitTexObj(void*, void*, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);

    u8 texObj[0x20];
    f32 trans[3][4];
    f32 scaleMtx[3][4];
    f32 rot[3][4];
    f32 model[3][4];
    u8* work = *(u8**)((u8*)effect + 0xC);
    void* camera = camGetPtr(3);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    s32 type = *(s32*)work;
    u32 color = 0xFFFFFF00 | *(u8*)(work + 0x20);
    u32 size;
    void* buffer;
    f32 scale;
    f32 halfWidth = -32.0f * 0.5f;
    f32 uMax = type == 0 ? 1.0f : 2.0f;
    f32 vMax = type == 1 ? 2.0f : 1.0f;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    if (type == 1) {
        effGetTexObj(0x45, texObj);
        GXLoadTexObj(texObj, 0);
    } else if (type == 0) {
        effGetTexObj(0x6A, texObj);
        GXLoadTexObj(texObj, 0);
    }
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x130, 0xF0, 4, 1);
    size = GXGetTexBufferSize(0x130, 0xF0, 4, 0, 0);
    buffer = smartAlloc(size, 3);
    GXCopyTex(buffer, 0);
    GXPixModeSync();
    GXInitTexObj(texObj, buffer, 0x130, 0xF0, 4, 0, 0, 0);
    GXLoadTexObj(texObj, 1);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    scale = *(f32*)(work + 0x1C);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXRotRad(rot, 'y', -0.017453292f * *(f32*)((u8*)camera + 0x114));
    PSMTXConcat(trans, scaleMtx, scaleMtx);
    PSMTXConcat(scaleMtx, rot, model);
    PSMTXRotRad(rot, 'z', 0.017453292f * *(f32*)(work + 0x18));
    PSMTXConcat(model, rot, model);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 0, 0, 0x1E, 0, 0x7D);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 8);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);
    *fifo = halfWidth; *fifo = 16.0f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    *fifo = 16.0f; *fifo = 16.0f; *fifo = 0.0f; *fifo = uMax; *fifo = 0.0f;
    *fifo = 16.0f; *fifo = halfWidth; *fifo = 0.0f; *fifo = uMax; *fifo = vMax;
    *fifo = halfWidth; *fifo = halfWidth; *fifo = 0.0f; *fifo = 0.0f; *fifo = vMax;
}

