#include "driver/windowdrv.h"

void* winTpl;
void* wakuTexObj;
void* WinObjects;
s32 unk_8041ea1c;

void* __memAlloc(s32 heap, u32 size);

void* getWakuTexObj(s32 id) {
    return (void*)((s32)wakuTexObj + id * 0x20);
}

void* windowGetPointer(s32 id) {
    return (void*)((s32)WinObjects + id * 0x48);
}

s32 windowCheckID(s32 id) {
    return *(u16*)((s32)WinObjects + id * 0x48 + 2) & 2;
}

s32 windowDelete(void* win) {
    void (*cb)(void*) = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

s32 windowDeleteID(s32 id) {
    void* win = (void*)((s32)WinObjects + id * 0x48);
    void (*cb)(void*);
    if ((*(u16*)((s32)win + 2) & 1) == 0) {
        return 0;
    }
    cb = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

void windowReInit(void) {
    void* win = WinObjects;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
}

void windowInit(void) {
    void* win = __memAlloc(0, 0x1F8);
    WinObjects = win;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
    winTpl = 0;
    unk_8041ea1c = 0;
}


u8 _windowDispGX_Message(f32 x, f32 y, f32 width, f32 height, f32 tailX, f32 tailY, s32 kind, u32 flags, u8 alpha, s32 dark) {
    extern void* camGetCurPtr(void);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void PSMTXTransApply(void* src, void* dst, f32 x, f32 y, f32 z);
    extern f32 __fabsf(f32);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 mtx);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetTexCoordGen2(s32 texcoord, s32 tgenType, s32 tgenSrc, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern u16 GXGetTexObjHeight(void* obj);
    extern u16 GXGetTexObjWidth(void* obj);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, u16 nverts);
    extern void* wakuTexObj;
    extern u32 dat_80420534;
    extern f32 float_0_80420538;
    extern f32 float_1_80420548;
    extern f32 float_560_8042054c;
    extern f32 float_176_80420550;
    extern f32 float_72_80420554;
    extern f32 float_416_8042055c;
    extern f32 float_136_80420560;
    extern f32 float_488_80420564;
    extern f32 float_neg136_80420568;
    extern f32 float_40_8042056c;
    extern f32 float_neg176_80420558;
    extern f32 float_neg1_80420570;
    extern f32 float_536_80420574;
    extern f32 float_neg24_80420578;
    extern f32 float_0p7_8042057c;
    extern f32 float_0p38_80420580;
    extern f32 float_0p175_80420584;
    extern f32 float_0p225_80420588;
    extern f32 float_0p8_8042058c;
    extern f32 float_1p2_80420590;
    extern f32 float_0p05_80420594;
    void* cam;
    u32 colorRaw;
    u32 color;
    s32 texId;
    u16 texW;
    u16 texH;
    f32 sx;
    f32 sy;
    f32 baseMtx[3][4];
    f32 scaleMtx[3][4];
    f32 texMtx0[3][4];
    f32 texMtx1[3][4];
    f32 texMtx2[3][4];
    f32 texMtx3[3][4];
    f32 texMtx4[3][4];
    f32 texMtx5[3][4];
    f32 texMtx6[3][4];
    f32 texMtx7[3][4];
    f32 texMtx8[3][4];
    f32 texMtx9[3][4];
    f32 texMtx10[3][4];
    f32 texMtx11[3][4];
    f32 texMtx12[3][4];
    f32 texMtx13[3][4];
    f32 d0;
    f32 d1;
    f32 d2;
    f32 d3;
    f32 tailHalf;

    cam = camGetCurPtr();
    colorRaw = dat_80420534;
    ((u8*)&colorRaw)[3] = alpha;
    color = colorRaw;
    GXSetTevColor(1, &color);

    PSMTXTrans(baseMtx, x, y, float_0_80420538);
    PSMTXScale(scaleMtx, width / float_560_8042054c, height / float_176_80420550, float_1_80420548);
    PSMTXConcat(baseMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11c), baseMtx, baseMtx);
    GXLoadPosMtxImm(baseMtx, 0);
    GXSetCurrentMtx(0);

    switch (kind) {
    case 0:
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 13 : 9;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx0, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx0, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 15 : 11;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_136_80420560) / (f32)texH;
    PSMTXScale(texMtx1, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx1, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 14 : 10;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx2, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx2, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 16 : 12;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_40_8042056c) / (f32)texH;
    PSMTXScale(texMtx3, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx3, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    break;
    case 1:
    texId = dark ? 4 : 3;
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    PSMTXScale(baseMtx, width, height, float_1_80420548);
    PSMTXTransApply(baseMtx, baseMtx, x, y, float_0_80420538);
    PSMTXConcat((void*)((s32)cam + 0x11c), baseMtx, baseMtx);
    GXLoadPosMtxImm(baseMtx, 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_neg1_80420570;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_neg1_80420570;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    break;
    case 6:
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 21 : 17;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx4, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx4, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 23 : 19;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_136_80420560) / (f32)texH;
    PSMTXScale(texMtx5, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx5, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 22 : 18;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx6, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx6, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 24 : 20;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_40_8042056c) / (f32)texH;
    PSMTXScale(texMtx7, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx7, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    break;
    case 8:
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 41 : 41;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_536_80420574) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx8, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx8, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_536_80420574;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_536_80420574;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 41 : 41;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_neg24_80420578) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx9, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx9, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_536_80420574;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_536_80420574;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    break;
    case 9:
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 37 : 33;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx10, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx10, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 39 : 35;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_136_80420560) / (f32)texH;
    PSMTXScale(texMtx11, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx11, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 38 : 34;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_72_80420554) / (f32)texW;
    sy = __fabsf(float_176_80420550) / (f32)texH;
    PSMTXScale(texMtx12, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx12, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_560_8042054c;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    texId = dark ? 40 : 36;
    texH = (u16)GXGetTexObjHeight((void*)((s32)wakuTexObj + texId * 0x20));
    texW = (u16)GXGetTexObjWidth((void*)((s32)wakuTexObj + texId * 0x20));
    sx = __fabsf(float_416_8042055c) / (f32)texW;
    sy = __fabsf(float_40_8042056c) / (f32)texH;
    PSMTXScale(texMtx13, sx, sy, float_1_80420548);
    GXLoadTexMtxImm(texMtx13, 0x1e, 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_72_80420554;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg176_80420558;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_488_80420564;
    *(volatile f32*)0xCC008000 = float_neg136_80420568;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    break;
    }

    GXSetTexCoordGen2(0, 1, 4, 0x3c, 0, 0x7d);
    if ((flags & 1) != 0) {
        sx = ((flags & 4) != 0) ? float_0p7_8042057c : float_0p38_80420580;
        d0 = (f32)(sx * width + x);
        if ((flags & 8) == 0) {
            d1 = -(float_0p8_8042058c * height - y);
            d2 = -(float_1p2_80420590 * height - y);
            if (d1 <= tailY) {
                return 0;
            }
        } else {
            d1 = -(float_0p175_80420584 * height - y);
            d2 = float_0p225_80420588 * height + y;
            if (tailY <= d1) {
                return 0;
            }
        }
        d3 = d0 - (((d0 - tailX) * (d1 - d2)) / (d1 - tailY));
        if (tailX < d0) {
            if (d3 < tailX) {
                d3 = tailX;
            }
        } else if (tailX < d3) {
            d3 = tailX;
        }
        if (kind == 9) {
            texId = dark ? 8 : 7;
            GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
        } else if ((kind == 6) || (kind == 0) || (kind == 1)) {
            texId = dark ? 2 : 1;
            GXLoadTexObj((void*)((s32)wakuTexObj + texId * 0x20), 0);
        }
        GXLoadPosMtxImm((void*)((s32)cam + 0x11c), 0);
        GXBegin(0x80, 0, 4);
        tailHalf = float_0p05_80420594 * width;
        *(volatile f32*)0xCC008000 = (f32)(d0 - tailHalf);
        *(volatile f32*)0xCC008000 = (f32)d1;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = (f32)(d0 + tailHalf);
        *(volatile f32*)0xCC008000 = (f32)d1;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_1_80420548;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = (f32)(d3 + tailHalf);
        *(volatile f32*)0xCC008000 = (f32)d2;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_1_80420548;
        *(volatile f32*)0xCC008000 = float_1_80420548;
        *(volatile f32*)0xCC008000 = (f32)(d3 - tailHalf);
        *(volatile f32*)0xCC008000 = (f32)d2;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_0_80420538;
        *(volatile f32*)0xCC008000 = float_1_80420548;
    }
    return 0;
}

void windowDispGX_System(f32 x, f32 y, f32 width, f32 height, s32 unused, u8 alpha) {
    extern void* camGetCurPtr(void);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetTexCoordGen2(s32 texcoord, s32 tgenType, s32 tgenSrc, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 tevStage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 tevStage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 tevStage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 tevStage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 tevStage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 tevStage, s32 rasSel, s32 texSel);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern u16 GXGetTexObjHeight(void* obj);
    extern u16 GXGetTexObjWidth(void* obj);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXBegin(s32 primitive, s32 vtxfmt, u16 nverts);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern f32 __fabsf(f32);

    extern void* wakuTexObj;
    extern u32 dat_80420528;
    extern u32 dat_80420530;
    extern f32 float_0_80420538;
    extern f32 float_1_80420548;
    extern f32 float_0p005_80420598;
    extern f32 float_10_8042059c;
    extern f32 dat_ptr_804205a0;
    extern f32 float_32_804205a4;

    static f32 scrl;

    void* cam;
    u32 tevColor;
    u32 tevColorBase;
    u32 fogColor;
    f32 baseMtx[3][4];
    f32 texMtxA[3][4];
    f32 texMtxB[3][4];
    f32 scaleMtx[3][4];
    f32 scroll;
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    f32 innerW;
    f32 innerH;
    f32 unitW;
    f32 unitH;
    f32 invW;
    f32 invH;

    (void)unused;

    cam = camGetCurPtr();

    scrl = scrl + float_0p005_80420598;
    if (scrl > float_10_8042059c) {
        scrl = scrl - float_10_8042059c;
    }

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);

    fogColor = dat_80420528;
    GXSetFog(0, float_0_80420538, float_0_80420538, float_0_80420538, float_0_80420538, &fogColor);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetCurrentMtx(0);

    tevColorBase = dat_80420530;
    ((u8*)&tevColorBase)[3] = alpha;
    tevColor = tevColorBase;
    GXSetTevColor(1, &tevColor);

    GXSetNumTevStages(4);

    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevSwapMode(0, 0, 0);

    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevSwapMode(1, 0, 0);

    GXSetTevOrder(2, 1, 2, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0, 8, 9, 0xF);
    GXSetTevAlphaIn(2, 7, 7, 7, 0);
    GXSetTevSwapMode(2, 0, 0);

    GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevColorIn(3, 0xF, 0, 2, 0xF);
    GXSetTevAlphaIn(3, 7, 0, 1, 7);
    GXSetTevSwapMode(3, 0, 0);

    PSMTXTrans(baseMtx, x, y, float_0_80420538);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);
    GXLoadPosMtxImm(baseMtx, 0);
    GXSetCurrentMtx(0);

    innerW = width - float_32_804205a4;
    innerH = height - float_32_804205a4;
    x0 = float_0_80420538;
    y0 = float_0_80420538;
    x1 = innerW;
    y1 = float_0_80420538 - innerH;

    /* upper-left/body strip */
    scroll = scrl;
    GXLoadTexObj((void*)((s32)wakuTexObj + 0xA0), 0);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x540), 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x5C0), 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    invH = dat_ptr_804205a0 / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0));
    invW = float_0_80420538 / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXTrans(texMtxA, scroll + invW, invH - scroll, float_0_80420538);
    unitH = __fabsf(height - float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0));
    unitW = __fabsf(width - float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXScale(scaleMtx, unitW, unitH, float_1_80420548);
    PSMTXConcat(texMtxA, scaleMtx, texMtxA);
    GXLoadTexMtxImm(texMtxA, 0x1E, 1);

    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    unitH = __fabsf(height - float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0x540));
    unitW = __fabsf(width - float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0x540));
    PSMTXScale(texMtxB, unitW, unitH, float_1_80420548);
    GXLoadTexMtxImm(texMtxB, 0x21, 1);

    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;

    /* lower strip */
    scroll = scrl;
    GXLoadTexObj((void*)((s32)wakuTexObj + 0xA0), 0);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x560), 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x5E0), 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    invH = innerH / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0)) - scroll;
    invW = float_0_80420538 / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0)) + scroll;
    PSMTXTrans(texMtxA, invW, invH, float_0_80420538);
    unitH = __fabsf(float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0));
    unitW = __fabsf(width - float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXScale(scaleMtx, unitW, unitH, float_1_80420548);
    PSMTXConcat(texMtxA, scaleMtx, texMtxA);
    GXLoadTexMtxImm(texMtxA, 0x1E, 1);

    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    unitH = __fabsf(float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0x560));
    unitW = __fabsf(width - float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0x560));
    PSMTXScale(texMtxB, unitW, unitH, float_1_80420548);
    GXLoadTexMtxImm(texMtxB, 0x21, 1);

    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = -innerH;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = -innerH;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = -(innerH + float_32_804205a4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = -(innerH + float_32_804205a4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;

    /* right strip */
    scroll = scrl;
    GXLoadTexObj((void*)((s32)wakuTexObj + 0xA0), 0);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x580), 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x600), 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    invH = dat_ptr_804205a0 / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0)) - scroll;
    invW = scroll + innerW / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXTrans(texMtxA, invW, invH, float_0_80420538);
    unitH = __fabsf(height - float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0));
    unitW = __fabsf(float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXScale(scaleMtx, unitW, unitH, float_1_80420548);
    PSMTXConcat(texMtxA, scaleMtx, texMtxA);
    GXLoadTexMtxImm(texMtxA, 0x1E, 1);

    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    unitH = __fabsf(height - float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0x580));
    unitW = __fabsf(float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0x580));
    PSMTXScale(texMtxB, unitW, unitH, float_1_80420548);
    GXLoadTexMtxImm(texMtxB, 0x21, 1);

    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = innerW;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = innerW + float_32_804205a4;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = innerW + float_32_804205a4;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = innerW;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;

    /* lower-right */
    scroll = scrl;
    GXLoadTexObj((void*)((s32)wakuTexObj + 0xA0), 0);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x5A0), 1);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x620), 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    invH = innerH / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0)) - scroll;
    invW = scroll + innerW / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXTrans(texMtxA, invW, invH, float_0_80420538);
    unitH = __fabsf(float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0xA0));
    unitW = __fabsf(float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0xA0));
    PSMTXScale(scaleMtx, unitW, unitH, float_1_80420548);
    PSMTXConcat(texMtxA, scaleMtx, texMtxA);
    GXLoadTexMtxImm(texMtxA, 0x1E, 1);

    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    unitH = __fabsf(float_32_804205a4) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + 0x5A0));
    unitW = __fabsf(float_32_804205a4) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + 0x5A0));
    PSMTXScale(texMtxB, unitW, unitH, float_1_80420548);
    GXLoadTexMtxImm(texMtxB, 0x21, 1);

    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = innerW;
    *(volatile f32*)0xCC008000 = -innerH;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = innerW + float_32_804205a4;
    *(volatile f32*)0xCC008000 = -innerH;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = innerW + float_32_804205a4;
    *(volatile f32*)0xCC008000 = -(innerH + float_32_804205a4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = innerW;
    *(volatile f32*)0xCC008000 = -(innerH + float_32_804205a4);
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
}

void windowDispGX_Kanban(f32 x, f32 y, f32 width, f32 height, s32 type, u8 alpha) {
    extern void* camGetCurPtr(void);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetTexCoordGen2(s32 texcoord, s32 tgenType, s32 tgenSrc, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 tevStage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 tevStage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 tevStage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 tevStage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 tevStage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 tevStage, s32 rasSel, s32 texSel);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern u16 GXGetTexObjHeight(void* obj);
    extern u16 GXGetTexObjWidth(void* obj);
    extern void GXBegin(s32 primitive, s32 vtxfmt, u16 nverts);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);

    extern void* wakuTexObj;
    extern u32 dat_80420528;
    extern u32 dat_8042052c;
    extern f32 float_0_80420538;
    extern f32 float_1_80420548;
    extern f32 float_560_8042054c;
    extern f32 float_176_80420550;
    extern f32 float_72_80420554;
    extern f32 float_neg176_80420558;
    extern f32 float_416_8042055c;
    extern f32 float_136_80420560;
    extern f32 float_488_80420564;
    extern f32 float_neg136_80420568;
    extern f32 float_40_8042056c;

    void* cam;
    u32 fogColor;
    u32 tevColor;
    f32 posMtx[3][4];
    f32 scaleMtx[3][4];
    f32 tileMtx0[3][4];
    f32 tileMtx1[3][4];
    f32 tileMtx2[3][4];
    f32 tileMtx3[3][4];

#define FIFO_F32(v) (*(volatile f32*)0xCC008000 = (v))
#define EMIT_VTX(px, py, tu, tv) \
    do { \
        FIFO_F32((px)); \
        FIFO_F32((py)); \
        FIFO_F32(float_0_80420538); \
        FIFO_F32((tu)); \
        FIFO_F32((tv)); \
    } while (0)

#define DRAW_TILE(mtx, texOff, drawW, drawH, x0, y0, x1, y1) \
    do { \
        GXSetNumTexGens(1); \
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D); \
        PSMTXScale((mtx), \
                   (drawW) / (f32)GXGetTexObjWidth((void*)((s32)wakuTexObj + (texOff))), \
                   (drawH) / (f32)GXGetTexObjHeight((void*)((s32)wakuTexObj + (texOff))), \
                   float_1_80420548); \
        GXLoadTexMtxImm((mtx), 0x1E, 1); \
        GXLoadTexObj((void*)((s32)wakuTexObj + (texOff)), 0); \
        GXBegin(0x80, 0, 4); \
        EMIT_VTX((x0), (y0), float_0_80420538, float_0_80420538); \
        EMIT_VTX((x1), (y0), float_1_80420548, float_0_80420538); \
        EMIT_VTX((x1), (y1), float_1_80420548, float_1_80420548); \
        EMIT_VTX((x0), (y1), float_0_80420538, float_1_80420548); \
    } while (0)

    cam = camGetCurPtr();

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);

    fogColor = dat_80420528;
    GXSetFog(0, float_0_80420538, float_0_80420538, float_0_80420538, float_0_80420538, &fogColor);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetCurrentMtx(0);

    tevColor = dat_8042052c;
    ((u8*)&tevColor)[3] = alpha;
    GXSetTevColor(1, &tevColor);

    PSMTXTrans(posMtx, x, y, float_0_80420538);
    PSMTXScale(scaleMtx, width / float_560_8042054c, height / float_176_80420550, float_1_80420548);
    PSMTXConcat(posMtx, scaleMtx, posMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), posMtx, posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);

    switch (type) {
        case 2:
        case 4:
            DRAW_TILE(tileMtx0, 0x3A0, float_72_80420554, float_176_80420550,
                      float_0_80420538, float_0_80420538, float_72_80420554, float_neg176_80420558);
            DRAW_TILE(tileMtx1, 0x3E0, float_416_8042055c, float_136_80420560,
                      float_72_80420554, float_0_80420538, float_488_80420564, float_neg136_80420568);
            DRAW_TILE(tileMtx2, 0x3C0, float_72_80420554, float_176_80420550,
                      float_488_80420564, float_0_80420538, float_560_8042054c, float_neg176_80420558);
            DRAW_TILE(tileMtx3, 0x400, float_416_8042055c, float_40_8042056c,
                      float_72_80420554, float_neg136_80420568, float_488_80420564, float_neg176_80420558);
            break;

        case 10:
            DRAW_TILE(tileMtx0, 0x320, float_72_80420554, float_176_80420550,
                      float_0_80420538, float_0_80420538, float_72_80420554, float_neg176_80420558);
            DRAW_TILE(tileMtx1, 0x360, float_416_8042055c, float_136_80420560,
                      float_72_80420554, float_0_80420538, float_488_80420564, float_neg136_80420568);
            DRAW_TILE(tileMtx2, 0x340, float_72_80420554, float_176_80420550,
                      float_488_80420564, float_0_80420538, float_560_8042054c, float_neg176_80420558);
            DRAW_TILE(tileMtx3, 0x380, float_416_8042055c, float_40_8042056c,
                      float_72_80420554, float_neg136_80420568, float_488_80420564, float_neg176_80420558);
            break;

        case 7:
            DRAW_TILE(tileMtx0, 0x0C0, float_560_8042054c, float_176_80420550,
                      float_0_80420538, float_0_80420538, float_560_8042054c, float_neg176_80420558);
            break;

    }

#undef DRAW_TILE
#undef EMIT_VTX
#undef FIFO_F32
}

void windowDispGX2_Waku_col(f32 x, f32 y, f32 width, f32 height, f32 curve, void* mtx, u16 gxTexMapID, u32* color) {
    extern void* camGetCurPtr(void);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 enable);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetTexCoordGen2(s32 texcoord, s32 tgenType, s32 tgenSrc, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
    extern void GXSetCurrentMtx(s32 mtx);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, u16 nverts);
    extern f32 float_0_80420538;
    extern f32 float_0p5_8042053c;
    extern f32 float_0p33333_80420540;
    extern f32 float_0p66667_80420544;
    extern f32 float_1_80420548;
    extern u32 dat_80420528;

    void* cam;
    u32 tevColor;
    u32 fogColor;
    f32 work[3][4];
    f32 x0;
    f32 x1;
    f32 x2;
    f32 x3;
    f32 y0;
    f32 y1;
    f32 y2;
    f32 y3;


    cam = camGetCurPtr();
    if (unk_8041ea1c == 0) {
        return;
    }

    if ((u16)gxTexMapID != 0) {
        gxTexMapID = 0;
    }

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);

    fogColor = dat_80420528;
    GXSetFog(0, float_0_80420538, float_0_80420538, float_0_80420538, float_0_80420538, &fogColor);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetCurrentMtx(0);

    tevColor = *color;
    GXSetTevColor(1, &tevColor);

    y0 = height * float_0p5_8042053c;
    x0 = -width * float_0p5_8042053c;
    PSMTXTrans(work, x - x0, y - y0, float_0_80420538);
    PSMTXConcat(work, mtx, work);
    PSMTXConcat((void*)((s32)cam + 0x11C), work, work);
    GXLoadPosMtxImm(work, 0);
    GXSetCurrentMtx(0);

    GXLoadTexObj((void*)((s32)wakuTexObj + (((u16)gxTexMapID) << 5)), 0);
    GXBegin(0x80, 0, 0x24);

    x3 = x0 + width;
    y3 = y0 - height;
    x1 = x0 + curve;
    y1 = y0 - curve;
    x2 = x3 - curve;
    y2 = y3 + curve;

    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;

    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;

    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y0;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;

    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;

    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;

    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y1;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;

    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = x0;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;

    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = x1;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p33333_80420540;
    *(volatile f32*)0xCC008000 = float_1_80420548;

    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y2;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = x3;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = float_1_80420548;
    *(volatile f32*)0xCC008000 = x2;
    *(volatile f32*)0xCC008000 = y3;
    *(volatile f32*)0xCC008000 = float_0_80420538;
    *(volatile f32*)0xCC008000 = float_0p66667_80420544;
    *(volatile f32*)0xCC008000 = float_1_80420548;

}

void windowDispGX_Waku_col(double x, double y, double width, double height, double curve, u16 gxTexMapID, u32* color) {
    extern void* camGetCurPtr(void);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 enable);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetTexCoordGen2(s32 texcoord, s32 tgenType, s32 tgenSrc, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
    extern void GXSetCurrentMtx(s32 mtx);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void PSMTXIdentity(void* mtx);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, u16 nverts);
    extern f32 float_0_80420538;
    extern f32 float_0p33333_80420540;
    extern f32 float_0p66667_80420544;
    extern f32 float_1_80420548;
    extern u32 dat_80420528;

    void* cam;
    u32 tevColor;
    u32 fogColor;
    f32 work[3][4];
    f32 x0;
    f32 x1;
    f32 x2;
    f32 x3;
    f32 y0;
    f32 y1;
    f32 y2;
    f32 y3;

#define FIFO_F32 (*(volatile f32*)0xCC008000)
#define EMIT_VERTEX(px, py, s, t) \
    do {                            \
        FIFO_F32 = (px);            \
        FIFO_F32 = (py);            \
        FIFO_F32 = float_0_80420538;\
        FIFO_F32 = (s);             \
        FIFO_F32 = (t);             \
    } while (0)
#define EMIT_QUAD(ax, ay, as, at, bx, by, bs, bt, cx, cy, cs, ct, dx, dy, ds, dt) \
    do {                                                                              \
        EMIT_VERTEX((ax), (ay), (as), (at));                                           \
        EMIT_VERTEX((bx), (by), (bs), (bt));                                           \
        EMIT_VERTEX((cx), (cy), (cs), (ct));                                           \
        EMIT_VERTEX((dx), (dy), (ds), (dt));                                           \
    } while (0)

    cam = camGetCurPtr();
    if (unk_8041ea1c == 0) {
        return;
    }

    if (gxTexMapID != 0) {
        gxTexMapID = 0;
    }

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);
    fogColor = dat_80420528;
    GXSetFog(0, float_0_80420538, float_0_80420538, float_0_80420538,
             float_0_80420538, &fogColor);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetCurrentMtx(0);

    tevColor = *color;
    GXSetTevColor(1, &tevColor);
    PSMTXIdentity(work);
    PSMTXConcat((void*)((s32)cam + 0x11C), work, work);
    GXLoadPosMtxImm(work, 0);
    GXSetCurrentMtx(0);

    GXLoadTexObj((void*)((s32)wakuTexObj + (((u16)gxTexMapID) << 5)), 0);
    GXBegin(0x80, 0, 0x24);

    x3 = (f32)(x + width);
    y3 = (f32)(y - height);
    x1 = (f32)(x + curve);
    y1 = (f32)(y - curve);
    x2 = (f32)((double)x3 - curve);
    y2 = (f32)(curve + (double)y3);
    x0 = (f32)x;
    y0 = (f32)y;

    EMIT_QUAD(x0, y0, float_0_80420538, float_0_80420538,
              x1, y0, float_0p33333_80420540, float_0_80420538,
              x1, y1, float_0p33333_80420540, float_0p33333_80420540,
              x0, y1, float_0_80420538, float_0p33333_80420540);
    EMIT_QUAD(x1, y0, float_0p33333_80420540, float_0_80420538,
              x2, y0, float_0p66667_80420544, float_0_80420538,
              x2, y1, float_0p66667_80420544, float_0p33333_80420540,
              x1, y1, float_0p33333_80420540, float_0p33333_80420540);
    EMIT_QUAD(x2, y0, float_0p66667_80420544, float_0_80420538,
              x3, y0, float_1_80420548, float_0_80420538,
              x3, y1, float_1_80420548, float_0p33333_80420540,
              x2, y1, float_0p66667_80420544, float_0p33333_80420540);
    EMIT_QUAD(x0, y1, float_0_80420538, float_0p33333_80420540,
              x1, y1, float_0p33333_80420540, float_0p33333_80420540,
              x1, y2, float_0p33333_80420540, float_0p66667_80420544,
              x0, y2, float_0_80420538, float_0p66667_80420544);
    EMIT_QUAD(x1, y1, float_0p33333_80420540, float_0p33333_80420540,
              x2, y1, float_0p66667_80420544, float_0p33333_80420540,
              x2, y2, float_0p66667_80420544, float_0p66667_80420544,
              x1, y2, float_0p33333_80420540, float_0p66667_80420544);
    EMIT_QUAD(x2, y1, float_0p66667_80420544, float_0p33333_80420540,
              x3, y1, float_1_80420548, float_0p33333_80420540,
              x3, y2, float_1_80420548, float_0p66667_80420544,
              x2, y2, float_0p66667_80420544, float_0p66667_80420544);
    EMIT_QUAD(x0, y2, float_0_80420538, float_0p66667_80420544,
              x1, y2, float_0p33333_80420540, float_0p66667_80420544,
              x1, y3, float_0p33333_80420540, float_1_80420548,
              x0, y3, float_0_80420538, float_1_80420548);
    EMIT_QUAD(x1, y2, float_0p33333_80420540, float_0p66667_80420544,
              x2, y2, float_0p66667_80420544, float_0p66667_80420544,
              x2, y3, float_0p66667_80420544, float_1_80420548,
              x1, y3, float_0p33333_80420540, float_1_80420548);
    EMIT_QUAD(x2, y2, float_0p66667_80420544, float_0p66667_80420544,
              x3, y2, float_1_80420548, float_0p66667_80420544,
              x3, y3, float_1_80420548, float_1_80420548,
              x2, y3, float_0p66667_80420544, float_1_80420548);

#undef EMIT_QUAD
#undef EMIT_VERTEX
#undef FIFO_F32
}

void windowDispGX_Message(f32 x, f32 y, f32 width, f32 height, f32 tailX, f32 tailY,
                          s32 kind, u32 flags, u8 alpha) {
    typedef struct GXColorLocal { u8 r, g, b, a; } GXColorLocal;
    extern void GXSetCullMode(s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, GXColorLocal);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetCurrentMtx(s32);
    extern void _windowDispGX_Message(f32, f32, f32, f32, f32, f32, s32, u32, u8, s32);
    extern GXColorLocal dat_80420528;
    extern f32 float_0_80420538;
    GXColorLocal fog = dat_80420528;

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);
    GXSetFog(0, float_0_80420538, float_0_80420538,
             float_0_80420538, float_0_80420538, fog);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 2, 8, 15);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetCurrentMtx(0);
    if (kind == 8) {
        _windowDispGX_Message(x, y, width, height, tailX, tailY, 8, 0, alpha, 1);
    } else {
        _windowDispGX_Message(x, y, width, height, tailX, tailY, kind, flags, alpha, 1);
        _windowDispGX_Message(x, y, width, height, tailX, tailY, kind, flags, alpha, 0);
    }
}

void windowTexSetup(void) {
    extern void* arcOpen(const char* filename, void** address, u32* length);
    extern char* getMarioStDvdRoot(void);
    extern s32 sprintf(char* str, const char* format, ...);
    extern void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* entry);
    extern void DVDMgrReadAsync(void* entry, void* dst, u32 size, u32 offset, void* callback);
    extern void* UnpackTexPalette(void* data);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void unk_8007fcf0(void* param_1, void* param_2);
    extern const char str_font_msgWindow_tpl_802c3080[];
    extern const char str_PCTs_f_msgWindow_tpl_802c3094[];
    char path[128];
    void* entry;
    u32 length;
    u32 i;
    u32 offset;

    winTpl = NULL;
    unk_8041ea1c = 0;
    winTpl = arcOpen(str_font_msgWindow_tpl_802c3080, NULL, NULL);
    if (winTpl == NULL) {
        sprintf(path, str_PCTs_f_msgWindow_tpl_802c3094, getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        length = (DVDMgrGetLength(entry) + 0x1F) & ~0x1F;
        winTpl = __memAlloc(0, length);
        *(void**)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, winTpl, length, 0, unk_8007fcf0);
    } else {
        UnpackTexPalette(winTpl);
        wakuTexObj = __memAlloc(0, *(u32*)((s32)winTpl + 4) << 5);
        for (i = 0, offset = 0; i < *(u32*)((s32)winTpl + 4); offset += 0x20, i++) {
            TEXGetGXTexObjFromPalette(winTpl, (void*)((s32)wakuTexObj + offset), i);
        }
    }
}

int windowEntry(s16 param_1) {
    void* win = WinObjects;
    s32 i;

    for (i = 0; i < 7; i++, win = (void*)((s32)win + 0x48)) {
        if ((*(u16*)((s32)win + 2) & 1) == 0) {
            memset(win, 0, 0x48);
            *(u16*)((s32)win + 2) = 1;
            *(u16*)((s32)win + 4) = param_1;
            return i;
        }
    }
    return -1;
}

void windowMain(void) {
    extern void* WinObjects;
    u8* window = WinObjects;
    s32 i;

    for (i = 0; i < 7; i++, window += 0x48) {
        void (*mainFunc)(void*);
        if ((*(u16*)(window + 2) & 1) == 0) {
            continue;
        }
        mainFunc = *(void (**)(void*))(window + 0x3C);
        if (mainFunc != 0) {
            mainFunc(window);
        }
    }
}

void unk_8007fcf0(void* param_1, void* param_2) {
    extern void* UnpackTexPalette(void* data);
    extern void DVDMgrClose(void* entry);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    void* entry = *(void**)((s32)param_2 + 0x2C);
    u32 offset;
    u32 i;

    UnpackTexPalette(winTpl);
    DVDMgrClose(entry);
    wakuTexObj = __memAlloc(0, *(u32*)((s32)winTpl + 4) << 5);
    i = 0;
    offset = 0;
    while (i < *(u32*)((s32)winTpl + 4)) {
        TEXGetGXTexObjFromPalette(winTpl, (void*)((s32)wakuTexObj + offset), i);
        offset += 0x20;
        i++;
    }
    unk_8041ea1c = 1;
}
