#include "effect/n64/eff_opuku_jetwrain_n64.h"


void effOpukuJetwrainMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effOpukuJetwrainDisp(void);
    extern f64 sin(f64);
    extern Vec3 vec3_802fbb70;
    u8* work = *(u8**)((s32)effect + 0xC);
    Vec3 pos;
    s32 row, col, index;
    s32 timer;
    s16* heights = (s16*)(work + 0x11A);
    s16 next;

    pos = vec3_802fbb70;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x20;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    *(u8*)(work + 0x1F) = 0xFF;
    if (timer < 0x20) *(u8*)(work + 0x1F) = (u8)(timer << 3);
    if (*(s32*)(work + 0x14) < 0x10)
        *(u8*)(work + 0x1F) = (u8)((*(s32*)(work + 0x14) << 4) + 0xF);

    for (row = 1; row < 18; row++) {
        index = row * 13;
        *(u8*)(work + 0x24 + index + 6) =
            (u8)(32.0f * (f32)sin((6.2832f * (f32)(*(s32*)(work + 0x14) * 8 + row * 12)) / 360.0f));
    }
    for (row = 1; row < 18; row++) {
        for (col = 1; col < 12; col++) {
            index = row * 13 + col;
            next = (s16)(((heights[index - 13] + heights[index + 13] +
                           heights[index - 1] + heights[index + 1]) >> 1) - heights[index]);
            next = (s16)((f32)next * 0.98f);
            if (timer < 0x20) {
                next = (s16)((f32)next * 0.7f);
            }
            heights[index] = next;
            *(u8*)(work + 0x24 + index) = (u8)(next >> 4);
        }
    }
    for (row = 1; row < 18; row++) {
        for (col = 1; col < 12; col++) {
            index = row * 13 + col;
            *(s8*)(work + 0x24 + index) =
                (s8)(0.02f * (f32)heights[index] +
                     (f32)*(s8*)(work + 0x24 + index));
        }
    }
    dispEntry(4, 2, effOpukuJetwrainDisp, effect, dispCalcZ(&pos));
}

void effOpukuJetwrainDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; typedef f32 Mtx44[4][4]; typedef struct Vec { f32 x,y,z; } Vec;
    extern void* camGetPtr(s32); extern void GXGetProjectionv(f32*); extern void C_MTXOrtho(Mtx44,f32,f32,f32,f32,f32,f32);
    extern void GXSetProjection(Mtx44,s32); extern void GXSetProjectionv(f32*); extern void GXSetZMode(s32,s32,s32);
    extern u32 GXGetTexBufferSize(u32,u32,s32,u8,u32); extern void* smartAlloc(u32,s32);
    extern void GXSetTexCopySrc(s32,s32,s32,s32); extern void GXSetTexCopyDst(s32,s32,s32,s32);
    extern void GXCopyTex(void*,s32); extern void GXPixModeSync(void); extern void GXInitTexObj(void*,void*,s32,s32,s32,s32,s32,s32);
    extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32); extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*);
    extern void C_MTXLookAt(Mtx,Vec*,Vec*,Vec*); extern void GXLoadPosMtxImm(Mtx,s32); extern void GXSetCurrentMtx(s32);
    extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXTrans(Mtx,f32,f32,f32); extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* work=*(u8**)((u8*)effect+0xC); f32 oldProjection[7]; Mtx44 ortho; Mtx view,scale,trans; u8 tex[0x20];
    Vec eye={0.0f,0.0f,1.0f},up={0.0f,1.0f,0.0f},target={0.0f,0.0f,0.0f}; u32 color; void* buffer; u32 size; s32 row,col,index;
    camGetPtr(cameraId); GXGetProjectionv(oldProjection); C_MTXOrtho(ortho,0.0f,480.0f,0.0f,608.0f,1.0f,32768.0f);
    GXSetProjection(ortho,1); GXSetZMode(0,7,0); size=GXGetTexBufferSize(0x130,0xF0,4,0,0); buffer=smartAlloc(size,3);
    GXSetTexCopySrc(0,0,0x260,0x1E0); GXSetTexCopyDst(0x130,0xF0,4,1); GXCopyTex(buffer,0); GXPixModeSync();
    GXInitTexObj(tex,buffer,0x130,0xF0,4,0,0,0); GXInitTexObjLOD(tex,1,1,0.0f,0.0f,0.0f,0,0,0); GXLoadTexObj(tex,0);
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,4);
    GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,15,2,7,8); GXSetTevAlphaIn(0,7,7,7,5);
    color=(*(u32*)(work+0x1C)&0xFFFFFF00)|(*(u8*)(work+0x1F)>>1); GXSetTevColor(1,&color); GXSetNumTexGens(1);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetCullMode(0); effSetVtxDescN64((void*)0x803A67D0);
    C_MTXLookAt(view,&eye,&target,&up); GXLoadPosMtxImm(view,0); GXSetCurrentMtx(0);
    for(row=0;row<12;row++) for(col=0;col<18;col++) {
        index=(row==0? -4:0)+0x18+row*0x10; PSMTXScale(scale,0.00390625f,0.00390625f,1.0f);
        PSMTXTrans(trans,(f32)((col==0?-4:0)+0x10+col*0x10),(f32)index,0.0f); PSMTXConcat(scale,trans,scale); GXLoadTexMtxImm(scale,0x1E,1);
        GXBegin(0x90,0,6); tri2(0,3,1,0,0,2,3,0);
    }
    GXSetProjectionv(oldProjection);
}

void* effOpukuJetwrainN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOpukuJetwrainMain(void*);
    extern char str_OpukuJetwrainN64_802fbba8[];
    void* entry;
    u8* work;
    s32 row;
    s32 col;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OpukuJetwrainN64_802fbba8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x308);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuJetwrainMain;
    *(s32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x10) = 0x3E8;
    } else {
        *(s32*)(work + 0x10) = time;
    }
    *(u8*)(work + 0x1F) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x18) = scale;
    *(u8*)(work + 0x1C) = 0x0A;
    *(u8*)(work + 0x1D) = 0x6E;
    *(u8*)(work + 0x1E) = 0xFF;
    *(u8*)(work + 0x20) = 0xBE;
    *(u8*)(work + 0x21) = 0xDC;
    *(u8*)(work + 0x22) = 0xFF;
    *(u8*)(work + 0x23) = 0;

    for (row = 0; row < 0x13; row++) {
        for (col = 0; col < 0xD; col++) {
            *(u8*)(work + 0x24 + row * 0xD + col) = 0;
            *(s16*)(work + 0x11A + row * 0x1A + col * 2) = 0;
        }
    }
    return entry;
}

