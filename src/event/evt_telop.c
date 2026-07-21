#include "event/evt_telop.h"

s32 evt_telop_entry(void* param_1, int param_2) {
    extern void* memset(void* dest, s32 val, u32 size);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 swGet(s32 id);
    extern void swSet(s32 id);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, u32 time, u32 duration);
    extern s32 dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, s32 param);
    extern void* wp;
    extern u8 telop(void);
    extern f32 float_neg256_80424b58;
    extern f32 float_256_80424b5c;
    extern f32 float_300_80424b60;
    s32* args;
    u32 elapsed;
    u32 ticksPerMs;

    args = *(s32**)((s32)param_1 + 0x18);
    if (param_2 != 0) {
        *(s32*)((s32)param_1 + 0x78) = 0;
        memset(wp, 0, 0x10);
        *(s32*)wp = evtGetValue(param_1, args[0]);
        if (swGet(*(s32*)wp + 0x28) != 0) {
            return 2;
        }
        swSet(*(s32*)wp + 0x28);
    }

    switch (*(s32*)((s32)param_1 + 0x78)) {
        case 0:
            *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
            *(s32*)((s32)wp + 8) = *(s32*)param_1;
            *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            break;
        case 1:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed < 1000) {
                *(s32*)((s32)wp + 4) =
                    (s32)intplGetValue(0xB, float_neg256_80424b58, float_256_80424b5c, elapsed, 1000);
            } else {
                *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
                *(s32*)((s32)wp + 8) = *(s32*)param_1;
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 2:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed > 5000) {
                *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
                *(s32*)((s32)wp + 8) = *(s32*)param_1;
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 3:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed < 1000) {
                *(s32*)((s32)wp + 4) =
                    (s32)intplGetValue(0xB, float_256_80424b5c, float_neg256_80424b58, elapsed, 1000);
            } else {
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 4:
            return 2;
    }

    return 0;
}


u8 telop(s32 cameraId) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void effGetTexObj(s32,void*);
    extern s32 GXGetTexObjWidth(void*); extern s32 GXGetTexObjHeight(void*);
    extern void GXSetZCompLoc(s32); extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetBlendMode(s32,s32,s32,s32); extern void GXSetZMode(s32,s32,s32);
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern s32 rand(void); extern void PSMTXTrans(Mtx,f32,f32,f32); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*); extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32); extern void GXLoadTexObj(void*,s32);
    extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32); extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32); extern void GXBegin(s32,s32,s32);
    u8* cam;
    u8 tex[0x20];
    Mtx trans, scale, model;
    f32 width, height;
    s32 i;

    cam = camGetPtr(cameraId);
    effGetTexObj(0x65, tex);
    width = (f32)GXGetTexObjWidth(tex);
    height = (f32)GXGetTexObjHeight(tex);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7,0,0,7,0);
    GXSetBlendMode(1,4,5,7);
    GXSetZMode(0,7,0);
    GXSetNumChans(0);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x1E,0,0x7D);
    PSMTXTrans(trans,(f32)(rand() & 0xFF) / width,(f32)(rand() & 0x3F) / height,0.0f);
    PSMTXScale(scale,0.2f,0.2f,1.0f);
    PSMTXConcat(trans,scale,trans);
    GXLoadTexMtxImm(trans,0x1E,1);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,1,1,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,15,15,15,15); GXSetTevAlphaIn(0,4,7,7,1);
    GXSetTevOrder(1,0,0,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,15,15,15,8); GXSetTevAlphaIn(1,7,1,4,7);
    GXSetTevOrder(2,0xFF,0xFF,0xFF); GXSetTevColorOp(2,0,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0);
    GXSetTevColorIn(2,15,0,2,15); GXSetTevAlphaIn(2,7,1,2,7);
    effGetTexObj(0x65,tex); GXLoadTexObj(tex,0);
    effGetTexObj(0x64,tex); GXInitTexObjLOD(tex,0,0,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex,1);
    GXSetCullMode(0); GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,13,1,4,0);
    for (i = 0; i < 2; i++) {
        PSMTXTrans(model,-15.0f + (f32)(i * 5),-105.0f - (f32)(i * 5),0.0f);
        PSMTXConcat(cam + 0x118,model,model);
        GXLoadPosMtxImm(model,0); GXSetCurrentMtx(0); GXBegin(0x80,0,4);
        *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f;
        *(volatile f32*)0xCC008000=width; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=0.0f;
        *(volatile f32*)0xCC008000=width; *(volatile f32*)0xCC008000=-height; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=1.0f; *(volatile f32*)0xCC008000=1.0f;
        *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=-height; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=0.0f; *(volatile f32*)0xCC008000=1.0f;
    }
    return 0;
}

