#include "driver/envdrv.h"

extern void* gp;
extern void* mapalloc_base_ptr;
static u8 work[0x1E0];
s32 tpl_ok;
void* env_tpl;

extern f32 float_10_80424894;
extern f32 float_0_804248ac;
extern f32 float_20_804248f8;
extern f32 float_5000_8042493c;
extern u8 lbl_8039B860[];

void* __memAlloc(s32 heap, u32 size);
void* _mapAlloc(void* heap, u32 size);
void PSMTXCopy(void* src, void* dst);
void UnpackTexPalette(void* tpl);
void DVDMgrClose(void* entry);
void* DVDMgrOpen(char* path, s32 mode, s32 unk);
u32 DVDMgrGetLength(void* entry);
void DVDMgrReadAsync(void* entry, void* dst, u32 size, s32 offset, void* callback);
void _mapFree(void* heap, void* ptr);
void GXGetViewportv(f32* vp);
void GXGetProjectionv(f32* proj);
void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
void GXSetProjectionv(f32* proj);
void GXSetTexCopySrc(s32 left, s32 top);
void GXSetTexCopyDst(u16 width, u16 height, s32 format, s32 mipmap);
void GXCopyTex(void* dst, s32 clear);
void GXPixModeSync(void);
void GXInitTexObj(void* obj, void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, f32 minLOD, f32 maxLOD, f32 lodBias, s32 biasClamp, s32 edgeLOD, s32 maxAniso);
u32 GXGetTexBufferSize(u16 width, u16 height, s32 format, s32 mipmap, s32 maxLOD);
void* arcOpen(const char* path, s32 mode, s32 heap);
char* getMarioStDvdRoot(void);
s32 sprintf(char* str, const char* format, ...);
void dispEntry(s32 cameraId, s32 mode, void* callback, f32 z, s32 param);
void envDisp_DepthOfField(void);
void envDisp_Blur(void);
void envDisp_FF(void);
void envCapture(void);
void envCapture2(void);
void envDisp2(void);
void envGlare(void* data);

const char str_env_tpl_802f9c78[] = "env.tpl";
const char str_PCTs_env_tpl_802f9c80[] = "%s/env.tpl";

static void* env_current_work(void) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    return ptr;
}

static void _callback(void* unused, void* entry) {
    void* closeEntry = *(void**)((s32)entry + 0x2C);
    UnpackTexPalette(env_tpl);
    DVDMgrClose(closeEntry);
    tpl_ok = 1;
}

void envInit(void) {
    memset(work, 0, 0xF0);
    memset(work + 0xF0, 0, 0xF0);
    env_tpl = 0;
    tpl_ok = 0;
}

void envReInit(void) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    memset(ptr, 0, 0xF0);
}


void envTexSetup(void) {
    void* entry;
    u32 length;
    char path[0x80];

    env_tpl = 0;
    entry = arcOpen(str_env_tpl_802f9c78, 0, 0);
    env_tpl = entry;
    if (entry == 0) {
        sprintf(path, str_PCTs_env_tpl_802f9c80, getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        length = DVDMgrGetLength(entry);
        env_tpl = __memAlloc(0, (length + 0x1F) & ~0x1F);
        *(void**)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, env_tpl, length, 0, _callback);
    } else {
        UnpackTexPalette(entry);
        tpl_ok = 1;
    }
}

void envCapture(void) {
    extern void* smartAlloc(u32 size, s32 mode);
    typedef void (*TexCopySrc4Fn)(s32 left, s32 top, u16 width, u16 height);
    TexCopySrc4Fn setTexCopySrc;
    void* entry;
    void* gpPtr;
    u16 width;
    u16 height;
    u16 halfWidth;
    u16 halfHeight;
    u32 size;

    entry = work;
    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    width = *(u16*)((s32)gpPtr + 0x170);
    height = *(u16*)((s32)gpPtr + 0x172);
    halfWidth = width >> 1;
    halfHeight = height >> 1;
    setTexCopySrc = (TexCopySrc4Fn)GXSetTexCopySrc;

    if ((*(u32*)entry & 1) != 0) {
        setTexCopySrc(0, 0, width, height);
        GXSetTexCopyDst(halfWidth, halfHeight, 6, 1);
        size = GXGetTexBufferSize(halfWidth, halfHeight, 6, 0, 0);
        *(void**)((s32)entry + 4) = smartAlloc(size, 3);
        GXCopyTex(**(void***)((s32)entry + 4), 0);
        GXPixModeSync();
        GXInitTexObj((void*)((s32)entry + 0xC), **(void***)((s32)entry + 4), halfWidth, halfHeight, 6, 0, 0, 0);
        GXInitTexObjLOD((void*)((s32)entry + 0xC), 1, 1, float_0_804248ac, float_0_804248ac, float_0_804248ac, 0, 0, 0);
    }

    if ((*(u32*)entry & 2) != 0) {
        setTexCopySrc(0, 0, width, height);
        GXSetTexCopyDst(halfWidth, halfHeight, 0x39, 1);
        size = GXGetTexBufferSize(halfWidth, halfHeight, 0x39, 0, 0);
        *(void**)((s32)entry + 8) = smartAlloc(size, 3);
        GXCopyTex(**(void***)((s32)entry + 8), 0);
        GXPixModeSync();
        GXInitTexObj((void*)((s32)entry + 0x2C), **(void***)((s32)entry + 8), halfWidth, halfHeight, 1, 0, 0, 0);
        GXInitTexObjLOD((void*)((s32)entry + 0x2C), 1, 1, float_0_804248ac, float_0_804248ac, float_0_804248ac, 0, 0, 0);
    }
}

void envCapture2(void) {
    void* entry = work;
    void* gpPtr;
    u16 width;
    u16 height;

    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    width = *(u16*)((s32)gpPtr + 0x170) >> 1;
    height = *(u16*)((s32)gpPtr + 0x172) >> 1;
    if ((*(u32*)entry & 4) != 0) {
        GXSetTexCopySrc(0, 0);
        GXSetTexCopyDst(width, height, 4, 1);
        GXCopyTex(*(void**)((s32)entry + 0x4C), 0);
        GXPixModeSync();
        GXInitTexObj((void*)((s32)entry + 0x50), *(void**)((s32)entry + 0x4C), width, height, 4, 0, 0, 0);
        GXInitTexObjLOD((void*)((s32)entry + 0x50), 1, 1, float_0_804248ac, float_0_804248ac, float_0_804248ac, 0, 0, 0);
    }
    if ((*(u32*)entry & 0x40000000) != 0) {
        if ((*(u16*)((s32)entry + 0x98) & 1) != 0 && (*(u32*)entry & 8) != 0) {
            *(u32*)entry &= ~4;
        }
        *(u32*)entry |= 8;
    }
}

void envDisp(void) {
    f32 vp[6];
    f32 proj[7];
    GXGetViewportv(vp);
    GXGetProjectionv(proj);
    envDisp_DepthOfField();
    envDisp_Blur();
    envDisp_FF();
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
}


void envDisp2(void) {
    f32 vp[6];
    f32 proj[7];
    void* entry;
    void* glare;
    u16 type;

    GXGetViewportv(vp);
    GXGetProjectionv(proj);
    entry = env_current_work();
    glare = (void*)((s32)entry + 0xB0);
    if ((*(u32*)entry & 0x20000000) != 0) {
        type = *(u16*)((s32)glare + 2);
        switch (type) {
            case 1:
                envGlare(lbl_8039B860 + 0x40);
                break;
            case 2:
                envGlare(lbl_8039B860 + 0xDC);
                break;
            case 3:
                envGlare(lbl_8039B860 + 0x138);
                break;
            case 4:
                envGlare(lbl_8039B860 + 0x2F4);
                break;
        }
    }
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
}


void envMain(void) {
    dispEntry(4, 7, envCapture, float_20_804248f8, 0);
    dispEntry(4, 7, envDisp, float_10_80424894, 0);
    dispEntry(4, 7, envCapture2, float_0_804248ac, 0);
    dispEntry(4, 8, envDisp2, float_5000_8042493c, 0);
}

void envDisp_DepthOfField(void) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* gp;
    extern void* camGetPtr(s32);
    extern void* smartTexObj(void*, void**);
    extern void GXSetProjection(void*, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevKColorSel(s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetTevKColor(s32, void*);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXBegin(s32, s32, s16);
    extern f32 float_6p2832_804248c8;
    extern f32 float_0p25_804248ec;
    extern f32 float_3p1416_804248f0;
    extern f32 float_0p03705_80424914;
    extern f32 float_0p4967_80424918;
    extern f32 float_1p5708_8042491c;
    extern f32 float_4p7124_80424920;
    extern f32 float_0p00761_80424924;
    extern f32 float_0p16605_80424928;
    extern f32 float_100_8042492c;
    extern f32 float_0p5_804248a8;
    u8* env = (u8*)work;
    char* cam;
    Mtx mtx;
    u32 dofFlags;
    s32 sample;
    u32 tevColor;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        env += 0xF0;
    }
    if ((*(u32*)env & 0x80000000) == 0) {
        return;
    }
    cam = camGetPtr(8);
    GXSetProjection(cam + 0x15C, *(s32*)(cam + 0x19C));
    cam = camGetPtr(8);
    GXLoadPosMtxImm(cam + 0x11C, 0);
    GXSetCurrentMtx(0);
    GXLoadTexObj(smartTexObj(env + 0x0C, *(void***)(env + 4)), 0);
    GXLoadTexObj(smartTexObj(env + 0x2C, *(void***)(env + 8)), 1);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetBlendMode(0, 1, 0, 7);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 3, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetCullMode(0);

    dofFlags = *(u32*)(env + 0x88);

#define DRAW_DOF_PASS(flag, amountOffset, colorOffset, secondPass) \
    if ((dofFlags & flag) != 0 && *(f32*)(env + amountOffset) != 0.0f) { \
        f32 amount = *(f32*)(env + amountOffset); \
        for (sample = 0; sample < 4; sample++) { \
            f32 angle = float_6p2832_804248c8 * (f32)sample * float_0p25_804248ec; \
            f32 s; \
            f32 c; \
            f32 t; \
            if (angle > float_3p1416_804248f0) { \
                if (angle >= float_4p7124_80424920) { \
                    t = float_1p5708_8042491c - (angle - float_4p7124_80424920); \
                    s = -(((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t); \
                } else { \
                    t = angle - float_3p1416_804248f0; \
                    s = -(((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t); \
                } \
            } else if (angle > float_1p5708_8042491c) { \
                t = float_1p5708_8042491c - (angle - float_1p5708_8042491c); \
                s = ((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t; \
            } else { \
                s = ((float_0p00761_80424924 * angle * angle - float_0p16605_80424928) * angle * angle) * angle; \
            } \
            if (angle > float_3p1416_804248f0) { \
                if (angle >= float_4p7124_80424920) { \
                    t = float_1p5708_8042491c - (angle - float_4p7124_80424920); \
                    c = (float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t; \
                } else { \
                    t = angle - float_3p1416_804248f0; \
                    c = -((float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t); \
                } \
            } else if (angle > float_1p5708_8042491c) { \
                t = float_1p5708_8042491c - (angle - float_1p5708_8042491c); \
                c = -((float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t); \
            } else { \
                c = (float_0p03705_80424914 * angle * angle - float_0p4967_80424918) * angle * angle; \
            } \
            PSMTXTrans(mtx, amount * s / float_100_8042492c, amount * c / float_100_8042492c, 0.0f); \
            GXLoadTexMtxImm(mtx, 0x1E + sample * 3, 1); \
            GXSetTexCoordGen2(sample, 1, 4, 0x1E + sample * 3, 0, 0x7D); \
            GXSetTevKColorSel(sample, 6); \
            GXSetTevKAlphaSel(sample, 6); \
            GXSetTevOrder(sample, sample, 0, -1); \
            GXSetTevColorOp(sample, 0, 0, 0, 1, sample == 3); \
            GXSetTevAlphaOp(sample, 0, 0, 0, 1, sample == 3); \
            GXSetTevColorIn(sample, 15, 8, 14, sample == 0 ? 15 : 0); \
            GXSetTevAlphaIn(sample, 7, 4, 6, sample == 0 ? 7 : 0); \
            GXSetTevSwapMode(sample, 0, 0); \
        } \
        tevColor = ((u32)env[colorOffset] << 24) | 0xFF; \
        GXSetTevKColor(0, &tevColor); \
        GXSetTexCoordGen2(4, 1, 4, 0x3C, 0, 0x7D); \
        if (secondPass) { u32 white = 0xFFFFFFFF; GXSetTevColor(2, &white); } \
        GXSetTevKColorSel(4, 0xC); \
        GXSetTevKAlphaSel(4, 0x1C); \
        GXSetTevOrder(4, 4, 1, -1); \
        GXSetTevColorOp(4, 8, 0, 0, 1, 0); \
        GXSetTevAlphaOp(4, 8, 0, 0, 1, 0); \
        GXSetTevColorIn(4, 8, 14, secondPass ? 12 : 2, 15); \
        GXSetTevAlphaIn(4, 4, 6, secondPass ? 2 : 1, 7); \
        GXSetTevSwapMode(4, 0, 0); \
        if (secondPass) { \
            GXSetTevOrder(5, 0xFF, 0xFF, -1); \
            GXSetTevColorOp(5, 0, 0, 0, 1, 0); \
            GXSetTevAlphaOp(5, 0, 0, 0, 1, 0); \
            GXSetTevColorIn(5, 2, 15, 9, 15); \
            GXSetTevAlphaIn(5, 1, 7, 9, 7); \
            GXSetTevSwapMode(5, 0, 0); \
        } \
        { \
        f32 halfW = (f32)*(u16*)((u8*)gp + 0x170) * float_0p5_804248a8; \
        f32 halfH = (f32)*(u16*)((u8*)gp + 0x172) * float_0p5_804248a8; \
        GXSetNumTexGens(5); \
        GXSetNumTevStages(secondPass ? 6 : 5); \
        GXBegin(0x80, 0, 4); \
        *(volatile f32*)0xCC008000 = -halfW; *(volatile f32*)0xCC008000 = halfH;  *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; \
        *(volatile f32*)0xCC008000 = halfW;  *(volatile f32*)0xCC008000 = halfH;  *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; \
        *(volatile f32*)0xCC008000 = halfW;  *(volatile f32*)0xCC008000 = -halfH; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; \
        *(volatile f32*)0xCC008000 = -halfW; *(volatile f32*)0xCC008000 = -halfH; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; *(volatile f32*)0xCC008000 = 0.0f; \
        } \
    }

    DRAW_DOF_PASS(1, 0x8C, 0x8A, 0);
    DRAW_DOF_PASS(2, 0x90, 0x8B, 1);
#undef DRAW_DOF_PASS
}

void envDisp_Blur(void) {
    extern void* camGetPtr(s32);
    extern void GXSetProjection(void*, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetTevKColorSel(s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetTevKColor(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern volatile f32 DAT_cc008000;
    extern f32 float_0_804248ac;
    extern f32 float_0p5_804248a8;

    void* envWork;
    void* gpPtr;
    void* cam;
    u32 color;
    f32 half;
    f32 zero;
    f32 width;
    f32 height;

    envWork = work;
    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        envWork = (void*)((s32)envWork + 0xF0);
    }

    if (((*(u32*)envWork & 0x40000000) != 0) && ((*(u32*)envWork & 8) != 0)) {
        cam = camGetPtr(8);
        GXSetProjection((void*)((s32)camGetPtr(8) + 0x15C), *(s32*)((s32)cam + 0x19C));
        cam = camGetPtr(8);
        GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
        GXSetTevAlphaIn(0, 7, 6, 4, 7);
        GXSetTevSwapMode(0, 0, 0);
        GXSetTevKColorSel(0, 0xC);
        GXSetTevKAlphaSel(0, 0x1C);

        if (*(u32*)((s32)envWork + 0xA8) == 0) {
            color = 0xFFFFFFA0;
        } else {
            color = 0xFFFFFF80;
        }
        GXSetTevKColor(0, &color);
        GXLoadTexObj((void*)((s32)envWork + 0x50), 0);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 3, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXBegin(0x80, 0, 4);

        half = float_0p5_804248a8;
        zero = float_0_804248ac;
        width = (f32)*(u16*)((s32)gpPtr + 0x170) * half;
        height = (f32)*(u16*)((s32)gpPtr + 0x172) * half;

        DAT_cc008000 = -width;
        DAT_cc008000 = height;
        DAT_cc008000 = zero;
        DAT_cc008000 = zero;
        DAT_cc008000 = zero;
        DAT_cc008000 = width;
        DAT_cc008000 = height;
        DAT_cc008000 = zero;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = zero;
        DAT_cc008000 = width;
        DAT_cc008000 = -height;
        DAT_cc008000 = zero;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = 1.0f;
        DAT_cc008000 = -width;
        DAT_cc008000 = -height;
        DAT_cc008000 = zero;
        DAT_cc008000 = zero;
        DAT_cc008000 = 1.0f;
    }
}


void envBlurOn(s32 type, u32 time) {
    void* entry = work;
    void* blur;
    void* gpPtr;
    u16 width;
    u16 height;
    u32 size;

    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    *(u32*)entry |= 0x40000000;
    *(u32*)entry |= 4;
    *(u32*)entry &= ~8;
    blur = (void*)((s32)entry + 0x98);
    memset(blur, 0, 0x18);
    gpPtr = gp;
    *(u32*)((s32)blur + 0xC) = *(u32*)((s32)gpPtr + 0x3C);
    *(u32*)((s32)blur + 8) = *(u32*)((s32)gpPtr + 0x38);
    *(u32*)((s32)blur + 0x10) = time;
    if (type != 0) {
        *(u16*)blur |= 1;
    }
    gpPtr = gp;
    width = *(u16*)((s32)gpPtr + 0x170) >> 1;
    height = *(u16*)((s32)gpPtr + 0x172) >> 1;
    size = GXGetTexBufferSize(width, height, 4, 0, 0);
    *(void**)((s32)entry + 0x4C) = _mapAlloc(mapalloc_base_ptr, size);
}

void envBlurOff(void) {
    void* entry = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    *(u32*)entry &= ~4;
    *(u32*)entry &= ~0x40000000;
    _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x4C));
}

void envGlare(void* data_) {
    typedef f32 Mtx[3][4];
    typedef f32 Mtx44[4][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* smartAlloc(u32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, u32, u32, u32, u32, u32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevKColorSel(u32, u32);
    extern void GXSetTevSwapMode(s32, u32, u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetZMode(u32, u32, u32);
    extern void GXSetBlendMode(u32, u32, u32, u32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void C_MTXOrtho(f32, f32, f32, f32, f32, f32, void*);
    extern void GXSetProjection(void*, s32);
    extern void PSMTXIdentity(void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void GXSetCullMode(s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetNumTexGens(u32);
    extern void GXBegin(s32, s32, s16);
    u32* data = (u32*)data_;
    char* env = (char*)work;
    GXTexObj tex;
    Mtx model;
    Mtx44 projection;
    u16 left, top, right, bottom, width, height;
    void* image;
    u32 i;
    u32 stage;

    if (*(s32*)((char*)gp + 0x14) != 0) env += 0xF0;
    top = *(u16*)(env + 0xB4);
    left = *(u16*)(env + 0xB8);
    height = (*(u16*)(env + 0xB6) - top) >> 1;
    width = (*(u16*)(env + 0xBA) - left) >> 1;
    image = smartAlloc(GXGetTexBufferSize(width, height, 4, 0, 0), 3);
    GXSetTexCopyDst(width, height, 4, 1);
    GXCopyTex(image, 0);
    GXPixModeSync();
    GXInitTexObj(&tex, image, width, height, 4, 0, 0, 0);
    GXInitTexObjLOD(&tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(&tex, 0);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 1, 0, 0, 1, 2);
    GXSetTevColorIn(0, 14, 15, 15, 8);
    GXSetTevKColorSel(0, data[3]);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevOrder(1, 0xFF, 0xFF, -1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 2, 14, 15);
    GXSetTevKColorSel(1, data[4]);
    GXSetTevSwapMode(1, 0, 0);
    for (i = 0, stage = 2; i < 7; i++, stage += 2) {
        GXSetTevOrder(stage, i + 1, 0, -1);
        GXSetTevColorOp(stage, 1, 0, 0, 1, 2);
        GXSetTevColorIn(stage, 14, 15, 15, 8);
        GXSetTevKColorSel(stage, data[3]);
        GXSetTevSwapMode(stage, 0, 0);
        GXSetTevOrder(stage + 1, 0xFF, 0xFF, -1);
        GXSetTevColorOp(stage + 1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(stage + 1, 15, 2, 14, 0);
        GXSetTevKColorSel(stage + 1, data[4]);
        GXSetTevSwapMode(stage + 1, 0, 0);
    }
    for (i = 0; i < 8; i++) GXSetTexCoordGen2(i, 1, 4, 0x1E + i * 3, 0, 0x7D);
    GXSetZMode((*(u32*)(env + 0xB0) & 1) != 0, (*(u32*)(env + 0xB0) & 1) != 0 ? 3 : 7, 0);
    GXSetBlendMode(data[0], data[1], data[2], 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetViewport(0.0f, 0.0f, (f32)*(u16*)((char*)gp + 0x170), (f32)*(u16*)((char*)gp + 0x172), 0.0f, 1.0f);
    GXSetScissor(0, 0, *(u16*)((char*)gp + 0x170), *(u16*)((char*)gp + 0x172));
    C_MTXOrtho(0.0f, (f32)*(u16*)((char*)gp + 0x172), 0.0f, (f32)*(u16*)((char*)gp + 0x170), 0.0f, -32767.0f, projection);
    GXSetProjection(projection, 1);
    PSMTXIdentity(model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(7, 9, 1, 3, 0);
    GXSetVtxAttrFmt(7, 13, 1, 2, 0);
    GXSetCullMode(2);
    right = left + width * 2;
    bottom = top + height * 2;
    GXSetNumTevStages(2);
    GXSetNumTexGens(1);
    GXBegin(0x80, 7, 4);
    *(volatile u16*)0xCC008000 = left; *(volatile u16*)0xCC008000 = top; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 0;
    *(volatile u16*)0xCC008000 = right; *(volatile u16*)0xCC008000 = top; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 1; *(volatile u16*)0xCC008000 = 0;
    *(volatile u16*)0xCC008000 = right; *(volatile u16*)0xCC008000 = bottom; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 1; *(volatile u16*)0xCC008000 = 1;
    *(volatile u16*)0xCC008000 = left; *(volatile u16*)0xCC008000 = bottom; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 0; *(volatile u16*)0xCC008000 = 1;
}


void envGlareFilter(s32 type, s32 x0, s32 y0, s32 x1, s32 y1) {
    void* gpPtr;
    void* entry;
    void* glare;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    gpPtr = gp;
    entry = work;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    glare = (void*)((s32)entry + 0xB0);
    if (type == 0) {
        *(u16*)((s32)glare + 2) = 0;
        *(u16*)((s32)glare + 0xA) = 0;
        *(u16*)((s32)glare + 6) = 0;
        *(u16*)((s32)glare + 8) = 0;
        *(u16*)((s32)glare + 4) = 0;
        *(u32*)entry &= ~0x20000000;
        return;
    }

    top = y0 - (y0 & 1);
    left = x0 - (x0 & 1);
    bottom = y1 + (y1 & 1);
    right = x1 + (x1 & 1);
    if (top < 0) {
        top = 0;
    }
    if (left < 0) {
        left = 0;
    }
    if (bottom > *(u16*)((s32)gpPtr + 0x172)) {
        bottom = *(u16*)((s32)gpPtr + 0x172);
    }
    if (right > *(u16*)((s32)gpPtr + 0x170)) {
        right = *(u16*)((s32)gpPtr + 0x170);
    }
    *(u16*)((s32)glare + 2) = type;
    *(u16*)((s32)glare + 4) = left;
    *(u16*)((s32)glare + 8) = top;
    *(u16*)((s32)glare + 6) = bottom;
    *(u16*)((s32)glare + 0xA) = right;
    *(f32*)((s32)glare + 0xC) = float_0_804248ac;
    *(u16*)glare &= ~1;
    *(u32*)entry |= 0x20000000;
}

void envGlareFilterZ(f32 z) {
    void* entry = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    entry = (void*)((s32)entry + 0xB0);
    *(f32*)((s32)entry + 0xC) = z;
    *(u16*)entry |= 1;
}

void envDisp_FF(void) {
    extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetLineWidth(s32, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern s32 rand(void);
    extern f64 __frsqrte(f64);
    extern volatile f32 DAT_cc008000;
    extern f32 float_0_804248ac;
    extern f32 float_0p5_804248a8;
    extern f32 float_1000_804248b0;
    extern f32 float_6p2832_804248c8;
    extern f32 float_3p1416_804248f0;
    extern f32 float_0p03705_80424914;
    extern f32 float_0p4967_80424918;
    extern f32 float_1p5708_8042491c;
    extern f32 float_4p7124_80424920;
    extern f32 float_0p00761_80424924;
    extern f32 float_0p16605_80424928;
    extern f32 float_100_8042492c;
    extern f64 double_0p5_802f9c60;
    extern f64 double_3_802f9c68;

    void* envWork;
    void* gpPtr;
    void* cam;
    s32 i;
    f32 radius;
    f32 angle;
    f32 x;
    f32 y;
    f32 jitter;
    f32 half;
    f32 zero;
    f32 t;
    f64 radiusValue;
    f64 estimate;
    s32 radiusSquare;

    envWork = work;
    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        envWork = (void*)((s32)envWork + 0xF0);
    }

    if ((*(u32*)envWork & 0x10000000) != 0) {
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevOp(0, 3);
        GXSetTevSwapMode(0, 0, 0);
        GXLoadTexObj((void*)((s32)envWork + 0xC), 0);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 3, 0);
        GXSetLineWidth(0x10, 0);

        cam = camGetPtr(8);
        GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

        half = float_0p5_804248a8;
        zero = float_0_804248ac;
        radiusSquare = (((s32)(*(u16*)((s32)gpPtr + 0x170) >> 1) *
                         (s32)*(u16*)((s32)gpPtr + 0x170)) >> 1) +
                       (((s32)(*(u16*)((s32)gpPtr + 0x172) >> 1) *
                         (s32)*(u16*)((s32)gpPtr + 0x172)) >> 1);
        radiusValue = (f64)(f32)radiusSquare;
        estimate = __frsqrte(radiusValue);
        estimate = double_0p5_802f9c60 * estimate *
                   (double_3_802f9c68 - radiusValue * estimate * estimate);
        estimate = double_0p5_802f9c60 * estimate *
                   (double_3_802f9c68 - radiusValue * estimate * estimate);
        estimate = double_0p5_802f9c60 * estimate *
                   (double_3_802f9c68 - radiusValue * estimate * estimate);
        radius = (f32)(radiusValue * estimate);
        GXBegin(0xA8, 0, 2000);
        for (i = 0; i < 1000; i++) {
            angle = (float_6p2832_804248c8 * (f32)i) / float_1000_804248b0;
            if (angle > float_3p1416_804248f0) {
                if (angle >= float_4p7124_80424920) {
                    t = float_1p5708_8042491c - (angle - float_4p7124_80424920);
                    x = -(((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t);
                    y = (float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t;
                } else {
                    t = angle - float_3p1416_804248f0;
                    x = -(((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t);
                    y = -((float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t);
                }
            } else if (angle > float_1p5708_8042491c) {
                t = float_1p5708_8042491c - (angle - float_1p5708_8042491c);
                x = ((float_0p00761_80424924 * t * t - float_0p16605_80424928) * t * t) * t;
                y = -((float_0p03705_80424914 * t * t - float_0p4967_80424918) * t * t);
            } else {
                x = ((float_0p00761_80424924 * angle * angle - float_0p16605_80424928) *
                     angle * angle) * angle;
                y = (float_0p03705_80424914 * angle * angle - float_0p4967_80424918) *
                    angle * angle;
            }
            jitter = (f32)(rand() % 10) / float_100_8042492c;

            DAT_cc008000 = x * radius;
            DAT_cc008000 = y * radius;
            DAT_cc008000 = zero;
            DAT_cc008000 = jitter;
            DAT_cc008000 = jitter;
            DAT_cc008000 = x * 5.0f + 5.0f + jitter;
            DAT_cc008000 = -y * 5.0f + 5.0f + jitter;
            DAT_cc008000 = zero;
            DAT_cc008000 = zero;
            DAT_cc008000 = zero;
            DAT_cc008000 = 0.5f;
            DAT_cc008000 = 0.5f;
        }
    }
}

void envTevLoadTexMtxImm(s32 type, s32 texMtx, s32 texMtx2, s32 mtxType) {
    typedef f32 Mtx[3][4];
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void PSMTXRotRad(void*, f32, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXInvXpose(void*, void*);
    extern void PSVECNormalize(void*, void*);
    extern void PSMTXMultVecSR(void*, void*, void*);
    extern void C_VECHalfAngle(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXIdentity(void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern Vec vec3_802f9c10;
    extern f32 float_3p1416_804248f0;
    extern f32 float_180_804248f4;
    char* env = (char*)work;
    Mtx matrix;
    Mtx temp;
    Vec direction;
    Vec halfAngle;
    f32 angleY;
    f32 angleZ;
    f32 scale;

    if (*(s32*)((char*)gp + 0x14) != 0) env += 0xF0;
    switch (type) {
        case 0:
            PSMTXRotRad(matrix, 0.017453292f * *(f32*)(env + 0x80), 'Y');
            PSMTXRotRad(temp, 0.017453292f * -*(f32*)(env + 0x84), 'Z');
            PSMTXConcat(matrix, temp, matrix);
            PSMTXInvXpose(matrix, matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            PSMTXScale(temp, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            PSMTXTrans(temp, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            GXLoadTexMtxImm(matrix, texMtx, mtxType);
            break;
        case 1:
            PSMTXRotRad(matrix, -1.5708f, 'Y');
            PSMTXConcat(matrix, *(void**)(env + 0x78), matrix);
            PSMTXInvXpose(matrix, matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            scale = -(0.25f * *(f32*)(env + 0x7C) - 0.5f);
            PSMTXScale(temp, scale, 0.5f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            PSMTXTrans(temp, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            GXLoadTexMtxImm(matrix, texMtx, mtxType);
            break;
        case 2:
            angleY = (float_3p1416_804248f0 * *(f32*)(env + 0x80)) / float_180_804248f4;
            angleZ = (float_3p1416_804248f0 * *(f32*)(env + 0x84)) / float_180_804248f4;
            direction.x = -(f32)cos(angleZ) * (f32)sin(angleY);
            direction.y = -(f32)sin(angleZ);
            direction.z = -(f32)cos(angleZ) * (f32)cos(angleY);
            PSMTXMultVecSR(*(void**)(env + 0x70), &direction, &direction);
            C_VECHalfAngle(&direction, &vec3_802f9c10, &halfAngle);
            PSMTXInvXpose(*(void**)(env + 0x78), matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            PSMTXIdentity(temp);
            temp[0][0] = halfAngle.x;
            temp[0][1] = halfAngle.y;
            temp[0][2] = halfAngle.z;
            PSMTXConcat(temp, matrix, matrix);
            scale = float_20_804248f8 * *(f32*)(env + 0x7C);
            PSMTXScale(temp, scale, 0.0f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            PSMTXTrans(temp, -scale, 0.0f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            GXLoadTexMtxImm(matrix, texMtx, mtxType);
            break;
        case 3:
            PSMTXInvXpose(*(void**)(env + 0x78), matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            PSMTXScale(temp, 2.0f * *(f32*)(env + 0x7C),
                       2.0f * *(f32*)(env + 0x7C), 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            PSMTXTrans(temp, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(temp, matrix, matrix);
            GXLoadTexMtxImm(matrix, texMtx, mtxType);
            break;
        case 4:
            PSMTXInvXpose(*(void**)(env + 0x78), matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            GXLoadTexMtxImm(matrix, texMtx, 1);
            PSMTXScale(temp, 0.5f, 0.5f, 0.0f);
            PSMTXTrans(matrix, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(matrix, temp, matrix);
            GXLoadTexMtxImm(matrix, texMtx2, mtxType);
            break;
    }
}


void envAddTev(s32 stage, s32 entryAddress) {
    typedef f32 Mtx[3][4];

    typedef struct GXTexObj {
        u32 data[8];
    } GXTexObj;

    typedef struct GXTlutObj {
        u32 data[3];
    } GXTlutObj;

    typedef struct TPLHeader {
        u16 mHeight;
        u16 mWidth;
        u32 mFormat;
        void* mpData;
        s32 mWrapS;
        s32 mWrapT;
        s32 mMinFilter;
        s32 mMagFilter;
        f32 mLODBias;
        u8 mEdgeLODEnable;
        u8 mMinLOD;
        u8 mMaxLOD;
        u8 mUnpacked;
    } TPLHeader;

    typedef struct TPLClutHeader {
        u16 mNumEntries;
        u8 mUnpacked;
        u8 pad;
        u32 mFormat;
        void* mpData;
    } TPLClutHeader;

    typedef struct TPLDescriptor {
        TPLHeader* mpTex;
        TPLClutHeader* mpTlut;
    } TPLDescriptor;

    extern void* gp;
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, Mtx);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadTlut(void*, s32);
    extern void GXInitTexObjCI(
        void*, u32, u32, u32, u32, u32, u32, u8, u32);
    extern void GXInitTlutObj(void*, u32, u32, u16);
    extern void GXSetTevKColorSel(s32, s32);
    extern void GXSetTevKColor(s32, void*);
    extern TPLDescriptor* TEXGet(void*, s32);
    extern s32 texmtx_tbl[];

    extern f32 float_0p5_804248a8;
    extern f32 float_0p15_804248d4;
    extern f32 float_45_804248d0;
    extern f32 float_90_804248d8;
    extern f32 float_0p3_804248dc;
    extern u32 dat_8042488c;

    u8* entry;
    u8* envWork;
    char* cam;
    u32 tevStage;
    u32 texMap;
    u32 texCoord;
    u32 field18;

    Mtx viewMtx;
    GXTexObj tex1;
    GXTexObj tex2;
    GXTexObj tex3;
    GXTexObj tex4;
    GXTexObj tex5;
    GXTexObj tex6;
    GXTexObj tex7;
    GXTexObj tex8;
    GXTexObj tex10;
    GXTlutObj tlut1;
    GXTlutObj tlut2;
    GXTlutObj tlut3;
    GXTlutObj tlut4;
    GXTlutObj tlut5;
    GXTlutObj tlut6;
    GXTlutObj tlut7;
    GXTlutObj tlut8;
    GXTlutObj tlut10;
    u32 kColor;

#define LOAD_ENV_TEX(texId, texObjVar, tlutObjVar) do { \
    TPLDescriptor* desc; \
    TPLHeader* header; \
    TPLClutHeader* clut; \
    u8 mipmap; \
    desc = TEXGet(env_tpl, (texId)); \
    if (desc->mpTlut == 0) { \
        desc = TEXGet(env_tpl, (texId)); \
        header = desc->mpTex; \
        GXInitTexObj(&(texObjVar), header->mpData, header->mWidth, \
                     header->mHeight, header->mFormat, 0, 0, \
                     header->mMinLOD != header->mMaxLOD); \
        header = desc->mpTex; \
        GXInitTexObjLOD(&(texObjVar), header->mMinFilter, \
                        header->mMagFilter, (f32)header->mMinLOD, \
                        (f32)header->mMaxLOD, header->mLODBias, 0, \
                        header->mEdgeLODEnable, 0); \
        GXLoadTexObj(&(texObjVar), texMap); \
    } else { \
        desc = TEXGet(env_tpl, (texId)); \
        mipmap = desc->mpTex->mMinLOD != desc->mpTex->mMaxLOD; \
        clut = desc->mpTlut; \
        GXInitTlutObj(&(tlutObjVar), (u32)clut->mpData, \
                      clut->mFormat, clut->mNumEntries); \
        header = desc->mpTex; \
        GXInitTexObjCI(&(texObjVar), (u32)header->mpData, \
                       (u32)header->mWidth, (u32)header->mHeight, \
                       header->mFormat, 0, 0, mipmap, 0); \
        header = desc->mpTex; \
        GXInitTexObjLOD(&(texObjVar), header->mMinFilter, \
                        header->mMagFilter, (f32)header->mMinLOD, \
                        (f32)header->mMaxLOD, header->mLODBias, 0, \
                        header->mEdgeLODEnable, 0); \
        GXLoadTlut(&(tlutObjVar), 0); \
        GXLoadTexObj(&(texObjVar), texMap); \
    } \
} while (0)

    entry = (u8*)entryAddress;
    envWork = work;
    if (*(s32*)((u8*)gp + 0x14) != 0) {
        envWork += 0xF0;
    }

    tevStage = *(u32*)(entry + 0x0C);
    texMap = *(u32*)(entry + 0x10);
    texCoord = *(u32*)(entry + 0x14);
    field18 = *(u32*)(entry + 0x18);

    if (stage != 0) {
        cam = camGetCurPtr();
        PSMTXConcat(cam + 0x11C, *(void**)(entry + 4), viewMtx);
        cam = camGetCurPtr();
        *(void**)(envWork + 0x70) = cam + 0x11C;
        *(void**)(envWork + 0x74) = *(void**)(entry + 4);
        *(void**)(envWork + 0x78) = viewMtx;

        GXSetTevOrder(tevStage, texCoord, texMap, 0xFF);
        GXSetTevColorOp(tevStage, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(tevStage, 0, 0, 0, 1, 0);
        GXSetTevSwapMode(tevStage, 0, 0);

        switch (stage) {
            case 1:
                GXSetTevColorIn(tevStage, 15, 0, 8, 15);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x80) = float_10_80424894;
                *(f32*)(envWork + 0x84) =
                    (f32)(*(u32*)((u8*)gp + 0x1C) + 45);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                LOAD_ENV_TEX(6, tex1, tlut1);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    0, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texCoord++;
                texMap++;
                break;

            case 2:
                GXSetTevColorIn(tevStage, 15, 0, 8, 9);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x80) = float_10_80424894;
                *(f32*)(envWork + 0x84) =
                    (f32)(*(u32*)((u8*)gp + 0x1C) + 45);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                LOAD_ENV_TEX(8, tex2, tlut2);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    0, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texCoord++;
                texMap++;
                break;

            case 3:
                GXSetTevColorIn(tevStage, 0, 15, 8, 15);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                LOAD_ENV_TEX(0, tex3, tlut3);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    1, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 4:
                GXSetTevColorIn(tevStage, 0, 15, 15, 8);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                *(f32*)(envWork + 0x80) = float_45_804248d0;
                *(f32*)(envWork + 0x84) = float_45_804248d0;
                LOAD_ENV_TEX(1, tex4, tlut4);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    2, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 5:
                GXSetTevColorIn(tevStage, 0, 15, 15, 8);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                *(f32*)(envWork + 0x80) = float_45_804248d0;
                *(f32*)(envWork + 0x84) = float_45_804248d0;
                LOAD_ENV_TEX(2, tex5, tlut5);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    3, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 6:
                GXSetTevColorIn(tevStage, 0, 15, 15, 8);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p15_804248d4;
                *(f32*)(envWork + 0x80) = float_0_804248ac;
                *(f32*)(envWork + 0x84) = float_90_804248d8;
                LOAD_ENV_TEX(4, tex6, tlut6);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    2, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 7:
                GXSetTevColorIn(tevStage, 0, 15, 15, 8);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p3_804248dc;
                *(f32*)(envWork + 0x80) = float_0_804248ac;
                *(f32*)(envWork + 0x84) = float_90_804248d8;
                LOAD_ENV_TEX(5, tex7, tlut7);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    3, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 8:
                GXSetTevColorIn(tevStage, 15, 0, 8, 15);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                *(f32*)(envWork + 0x7C) = float_0p5_804248a8;
                *(f32*)(envWork + 0x80) = float_10_80424894;
                *(f32*)(envWork + 0x84) = float_45_804248d0;
                LOAD_ENV_TEX(10, tex8, tlut8);
                GXSetTexCoordGen2(
                    texCoord, 1, 1, texmtx_tbl[texMap], 0, 0x7D);
                envTevLoadTexMtxImm(
                    0, texmtx_tbl[texMap], 0x40, 1);
                tevStage++;
                texMap++;
                texCoord++;
                break;

            case 10:
                GXSetTevColorIn(tevStage, 0, 8, 14, 15);
                GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
                GXSetTevKColorSel(tevStage, 0xD);
                kColor = dat_8042488c;
                GXSetTevKColor(1, &kColor);
                LOAD_ENV_TEX(9, tex10, tlut10);
                GXSetTexCoordGen2(
                    texCoord, 0, 1, texmtx_tbl[texMap], 0, 0x40);
                envTevLoadTexMtxImm(
                    4, texmtx_tbl[texMap], 0x40, 0);
                tevStage++;
                texMap++;
                texCoord++;
                break;
        }

        *(u32*)(entry + 0x0C) = tevStage;
        *(u32*)(entry + 0x14) = texCoord;
        *(u32*)(entry + 0x18) = field18;
        *(u32*)(entry + 0x10) = texMap;
    }

#undef LOAD_ENV_TEX
}


void envSetWater(s32* param) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* gp;
    extern void* smartAlloc(u32, s32);
    extern void GXTexModeSync(void);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTexCopySrc(u32, u32, u32, u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetIndTexMtx(s32, f32*, s32);
    extern void GXSetIndTexOrder(s32, s32, s32);
    extern void GXSetIndTexCoordScale(s32, s32, s32);
    extern void GXSetTevIndWarp(s32, s32, s32, s32, s32);
    extern void PSMTXMultVec(void*, void*, Vec*);
    extern void* camGetCurPtr(void);
    extern f32 PSVECDistance(Vec*, Vec*);
    u32* obj = (u32*)param[0];
    u32 flags = *obj;
    u32 left, top, right, bottom;
    u16 width, height;
    u32 size;
    s32 tevStage = param[3];
    s32 texMap = param[4];
    s32 texCoord = param[5];
    s32 indStage = param[6];
    void* image;
    GXTexObj tex;
    Vec worldPos;
    void* camera;
    void* indirectImage;
    f32 distance;
    f32 intensity;
    f32 indMtx[6] = {0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f};

    if ((flags & 0x20) == 0) {
        if ((flags & 0x20000000) == 0) {
            *obj = flags | 0x20000000;
            return;
        }
    } else {
        do {
            obj = (u32*)obj[0x38];
            if (obj == 0) return;
            flags = *obj;
        } while ((flags & 0x10) == 0);
        if ((flags & 0x20000000) == 0) {
            *obj = flags | 0x20000000;
            return;
        }
    }
    left = *(u16*)((char*)obj + 0xEC);
    top = *(u16*)((char*)obj + 0xEE);
    right = *(u16*)((char*)obj + 0xF0);
    bottom = *(u16*)((char*)obj + 0xF2);
    if (left == 0x3FF || top == 0x3FF || right == 0x3FF || bottom == 0x3FF) return;
    if ((left | top | right | bottom) == 0) return;
    left &= ~1;
    top &= ~1;
    right += right & 1;
    bottom += bottom & 1;
    if (right > *(u16*)((char*)gp + 0x170)) right = *(u16*)((char*)gp + 0x170);
    if (bottom > *(u16*)((char*)gp + 0x172)) bottom = *(u16*)((char*)gp + 0x172);
    width = (right - left) >> 1;
    height = (bottom - top) >> 1;
    size = GXGetTexBufferSize(width, height, 4, 0, 0);
    image = smartAlloc(size, 3);
    GXSetTexCopySrc(left, top, right - left, bottom - top);
    GXSetTexCopyDst(width, height, 4, 1);
    GXCopyTex(image, 0);
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj(&tex, image, width, height, 4, 0, 0, 0);
    GXInitTexObjLOD(&tex, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    PSMTXMultVec((void*)param[1], (u8*)obj + 0x10, &worldPos);
    camera = camGetCurPtr();
    distance = PSVECDistance((Vec*)((u8*)camera + 0x0C), &worldPos);
    intensity = 1000.0f / distance;
    if (intensity < 0.0f) intensity = 0.0f;
    if (intensity > 127.0f) intensity = 127.0f;
    indirectImage = smartAlloc(GXGetTexBufferSize(0x40, 0x40, 3, 0, 0), 3);
    GXLoadTexObj(&tex, texMap);
    GXSetTexCoordGen2(texCoord, 0, 0, 0x1E + texMap * 3, 0, 0x7D);
    GXSetTevOrder(tevStage, texCoord, texMap, -1);
    GXSetTevColorOp(tevStage, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tevStage, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tevStage, 8, 0, 1, 15);
    GXSetTevAlphaIn(tevStage, 7, 7, 7, 4);
    GXSetIndTexMtx(1, indMtx, 1);
    GXSetIndTexOrder(indStage, texCoord, texMap + 1);
    GXSetIndTexCoordScale(indStage, 0, 0);
    GXSetTevIndWarp(tevStage, indStage, 1, 0, 1);
}

void envSetYamiView(void* mtx) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    PSMTXCopy(mtx, (void*)((s32)ptr + 0xC0));
}

void envSetYami(int data) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void C_MTXLightOrtho(f32, f32, f32, f32, f32, f32, f32, f32, Mtx);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern f32 u1_759;
    extern f32 u2_760;
    extern u32 v_count_761;
    extern s32 texmtx_tbl[];
    extern s32 DAT_802f9bec[];
    extern f32 float_0p004_80424890;
    extern f32 float_10_80424894;
    extern f32 float_72_80424898;
    extern f32 float_neg72_8042489c;
    extern f32 float_neg96_804248a0;
    extern f32 float_96_804248a4;

    u8* envWork;
    void* gpPtr;
    GXTexObjLocal texObj;
    Mtx mtx;
    s32 tev;
    s32 texMap;
    s32 texCoord;
    s32 color;
    u32 retrace;
    void* modelMtx;
    f32 zero;

    envWork = work;
    gpPtr = gp;
    if (*(s32*)((s32)gpPtr + 0x14) != 0) {
        envWork += 0xF0;
    }
    retrace = *(u32*)((s32)gpPtr + 0x1C);
    tev = *(s32*)(data + 0xC);
    texMap = *(s32*)(data + 0x10);
    texCoord = *(s32*)(data + 0x14);
    color = *(s32*)(data + 0x18);
    modelMtx = *(void**)(data + 4);

    if (v_count_761 != retrace) {
        u1_759 += float_0p004_80424890;
        u2_760 -= float_0p004_80424890;
        v_count_761 = retrace;
        if (u1_759 > float_10_80424894) {
            u1_759 -= float_10_80424894;
        }
        if (u2_760 < float_10_80424894) {
            u2_760 += float_10_80424894;
        }
    }

    GXSetTevColorOp(tev - 1, 0, 0, 0, 1, 1);
    GXSetTevAlphaOp(tev - 1, 0, 0, 0, 1, 1);
    zero = 0.0f;
    C_MTXLightOrtho(float_72_80424898, float_neg72_8042489c, float_neg96_804248a0,
                    float_96_804248a4, zero, zero, u1_759, zero, mtx);
    PSMTXConcat(mtx, (void*)(envWork + 0xC0), mtx);
    PSMTXConcat(mtx, modelMtx, mtx);
    GXLoadTexMtxImm(mtx, texmtx_tbl[texMap], 0);
    GXSetTexCoordGen2(texCoord, 0, 0, texmtx_tbl[texMap], 0, 0x7D);

    C_MTXLightOrtho(float_72_80424898, float_neg72_8042489c, float_neg96_804248a0,
                    float_96_804248a4, zero, zero, u2_760, zero, mtx);
    PSMTXConcat(mtx, (void*)(envWork + 0xC0), mtx);
    PSMTXConcat(mtx, modelMtx, mtx);
    GXLoadTexMtxImm(mtx, DAT_802f9bec[texMap], 0);
    GXSetTexCoordGen2(texCoord + 1, 0, 0, DAT_802f9bec[texMap], 0, 0x7D);

    TEXGetGXTexObjFromPalette(env_tpl, &texObj, 9);
    GXLoadTexObj(&texObj, texMap);
    TEXGetGXTexObjFromPalette(env_tpl, &texObj, 10);
    GXLoadTexObj(&texObj, texMap + 1);

    GXSetTevOrder(tev, texCoord, texMap, 0xFF);
    GXSetTevColorOp(tev, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tev, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tev, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(tev, 7, 7, 7, 4);
    GXSetTevSwapMode(tev, 0, 0);

    GXSetTevOrder(tev + 1, texCoord + 1, texMap + 1, 0xFF);
    GXSetTevColorOp(tev + 1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tev + 1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tev + 1, 0xF, 0, 4, 0xF);
    GXSetTevAlphaIn(tev + 1, 7, 0, 4, 7);
    GXSetTevSwapMode(tev + 1, 0, 0);

    GXSetTevOrder(tev + 2, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(tev + 2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tev + 2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(tev + 2, 2, 0xF, 0, 0xF);
    GXSetTevAlphaIn(tev + 2, 7, 7, 7, 1);
    GXSetTevSwapMode(tev + 2, 0, 0);

    *(s32*)(data + 0xC) = tev + 3;
    *(s32*)(data + 0x14) = texCoord + 2;
    *(s32*)(data + 0x18) = color;
    *(s32*)(data + 0x10) = texMap + 2;
}

/*
 * Target-owned DepthOfField scalar subset.
 *
 * Keep these definitions after the function bodies so earlier consumers retain
 * their existing opaque/local-extern compiler view.  The order below follows
 * the target .sdata2 order for this proven subset.
 */
const f32 float_0p5_804248a8 = 0.5f;
const f32 float_6p2832_804248c8 = 6.2831855f;
const f32 float_0p25_804248ec = 0.25f;
const f32 float_3p1416_804248f0 = 3.1415927f;
const f32 float_0p03705_80424914 = 0.03705f;
const f32 float_0p4967_80424918 = 0.4967f;
const f32 float_1p5708_8042491c = 1.5707964f;
const f32 float_4p7124_80424920 = 4.712389f;
const f32 float_0p00761_80424924 = 0.00761f;
const f32 float_0p16605_80424928 = 0.16605f;
const f32 float_100_8042492c = 100.0f;
