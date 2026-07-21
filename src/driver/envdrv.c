#include "driver/envdrv.h"

extern void* gp;
extern void* mapalloc_base_ptr;
u8 work[0x1E0];
void* env_tpl;
s32 tpl_ok;

extern f32 float_10_80424894;
extern f32 float_0_804248ac;
extern f32 float_20_804248f8;
extern f32 float_5000_8042493c;
extern u8 lbl_8039B860[];

void* memset(void* dst, int value, u32 size);
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

void _callback(void* unused, void* entry) {
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
    u8* env = (u8*)work;
    char* cam;
    Mtx mtx;
    u32 dofFlags;
    s32 direction;
    s32 sample;
    s32 vertex;
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
    for (direction = 0; direction < 2; direction++) {
        f32 amount = *(f32*)(env + 0x8C + direction * 4);
        if ((dofFlags & (1 << direction)) == 0 || amount == 0.0f) {
            continue;
        }
        for (sample = 0; sample < 4; sample++) {
            f32 phase = (f32)sample * 1.5708f;
            f32 offset = amount * phase * 0.01f;
            PSMTXTrans(mtx, direction ? offset : 0.0f, direction ? 0.0f : offset, 0.0f);
            GXLoadTexMtxImm(mtx, 0x1E + sample * 3, 1);
            GXSetTexCoordGen2(sample, 1, 4, 0x1E + sample * 3, 0, 0x7D);
            GXSetTevKColorSel(sample, 6);
            GXSetTevKAlphaSel(sample, 6);
            GXSetTevOrder(sample, sample, 0, -1);
            GXSetTevColorOp(sample, 0, 0, 0, 1, sample == 3);
            GXSetTevAlphaOp(sample, 0, 0, 0, 1, sample == 3);
            GXSetTevColorIn(sample, 15, 8, 14, sample == 0 ? 15 : 0);
            GXSetTevAlphaIn(sample, 7, 4, 6, sample == 0 ? 7 : 0);
            GXSetTevSwapMode(sample, 0, 0);
        }
        tevColor = ((u32)env[0x8A + direction] << 24) | 0xFF;
        GXSetTevKColor(0, &tevColor);
        GXSetTexCoordGen2(4, 1, 4, 0x3C, 0, 0x7D);
        if (direction != 0) {
            u32 white = 0xFFFFFFFF;
            GXSetTevColor(2, &white);
        }
        GXSetTevKColorSel(4, 0xC);
        GXSetTevKAlphaSel(4, 0x1C);
        GXSetTevOrder(4, 4, 1, -1);
        GXSetTevColorOp(4, 8, 0, 0, 1, 0);
        GXSetTevAlphaOp(4, 8, 0, 0, 1, 0);
        GXSetTevColorIn(4, 8, 14, 2, 15);
        GXSetTevAlphaIn(4, 4, 6, 1, 7);
        GXSetTevSwapMode(4, 0, 0);
        GXSetNumTexGens(5);
        GXSetNumTevStages(5);
        GXBegin(0x80, 0, 4);
        for (vertex = 0; vertex < 4; vertex++) {
            *(volatile f32*)0xCC008000 = (vertex & 1) ? 320.0f : -320.0f;
            *(volatile f32*)0xCC008000 = (vertex & 2) ? -240.0f : 240.0f;
            *(volatile f32*)0xCC008000 = 0.0f;
            *(volatile f32*)0xCC008000 = (f32)(vertex & 1);
            *(volatile f32*)0xCC008000 = (f32)((vertex >> 1) & 1);
        }
    }
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
    extern volatile f32 DAT_cc008000;
    extern f32 float_0_804248ac;
    extern f32 float_0p5_804248a8;
    extern f32 float_1000_804248b0;
    extern f32 float_6p2832_804248c8;
    extern f32 float_3p1416_804248f0;
    extern f32 float_1p5708_8042491c;
    extern f32 float_4p7124_80424920;
    extern f32 float_100_8042492c;

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
        radius = ((f32)*(u16*)((s32)gpPtr + 0x170) + (f32)*(u16*)((s32)gpPtr + 0x172)) * half;
        GXBegin(0xA8, 0, 2000);
        for (i = 0; i < 1000; i++) {
            angle = (float_6p2832_804248c8 * (f32)i) / float_1000_804248b0;
            if (angle > float_3p1416_804248f0) {
                angle -= float_3p1416_804248f0;
                x = -angle;
            } else {
                x = angle;
            }
            if (angle > float_1p5708_8042491c) {
                y = float_4p7124_80424920 - angle;
            } else {
                y = angle;
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
    extern void PSMTXRotRad(void*, f32, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXInvXpose(void*, void*);
    extern void PSVECNormalize(void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXIdentity(void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    char* env = (char*)work;
    Mtx matrix;
    Mtx temp;
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
            PSMTXInvXpose(*(void**)(env + 0x78), matrix);
            PSVECNormalize(matrix, matrix);
            PSVECNormalize((char*)matrix + 0x10, (char*)matrix + 0x10);
            PSVECNormalize((char*)matrix + 0x20, (char*)matrix + 0x20);
            PSMTXIdentity(temp);
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


u8 envAddTev(s32 stage, s32 entryAddress) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    typedef f32 Mtx[3][4];
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
    extern void* TEXGet(s32);
    u8* entry = (u8*)entryAddress;
    GXTexObj tex;
    void* image;
    s32 type = stage;
    s32 tevStage = *(u32*)(entry + 0xC);
    s32 texMap = *(u32*)(entry + 0x10);
    s32 texCoord = *(u32*)(entry + 0x14);
    s32 texId;
    u8* envWork = (u8*)work;
    char* cam;
    Mtx viewMtx;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        envWork += 0xF0;
    }
    if (stage != 0) {
        cam = camGetCurPtr();
        PSMTXConcat(cam + 0x11C, *(void**)(entry + 4), viewMtx);
        cam = camGetCurPtr();
        *(void**)(envWork + 0x70) = cam + 0x11C;
        *(void**)(envWork + 0x74) = *(void**)(entry + 4);
        *(void**)(envWork + 0x78) = viewMtx;
    }

    GXSetTevOrder(tevStage, texCoord, texMap, -1);
    GXSetTevColorOp(tevStage, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(tevStage, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(tevStage, 0, 0);
    switch (type) {
        case 1:
            GXSetTevColorIn(tevStage, 15, 8, 10, 15);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 6;
            break;
        case 2:
            GXSetTevColorIn(tevStage, 15, 10, 8, 10);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 8;
            break;
        case 3:
            GXSetTevColorIn(tevStage, 8, 15, 10, 15);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 0;
            break;
        case 4:
            GXSetTevColorIn(tevStage, 8, 15, 15, 10);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 1;
            break;
        case 5:
            GXSetTevColorIn(tevStage, 8, 15, 15, 10);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 2;
            break;
        case 6:
            GXSetTevColorIn(tevStage, 8, 15, 15, 10);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 3;
            break;
        case 7:
            GXSetTevColorIn(tevStage, 8, 15, 15, 10);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 4;
            break;
        case 8:
            GXSetTevColorIn(tevStage, 15, 8, 10, 15);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 5;
            break;
        case 10:
            GXSetTevColorIn(tevStage, 0, 8, 14, 15);
            GXSetTevAlphaIn(tevStage, 7, 7, 7, 0);
            texId = 7;
            break;
        default:
            return type;
    }
    image = TEXGet(texId);
    if (image != 0) {
        *(void**)&tex = image;
        GXLoadTexObj(&tex, texMap);
        if ((*(u8*)image & 0x10) != 0) {
            GXLoadTlut((u8*)image + 0x20, texMap);
        }
    }
    GXSetTexCoordGen2(texCoord, 1, 1, 0x1E + texMap * 3, 0, 0x7D);
    *(u32*)(entry + 0x0C) = tevStage + 1;
    *(u32*)(entry + 0x10) = texMap + 1;
    *(u32*)(entry + 0x14) = texCoord + 1;
    return type + 1;
}

void envSetWater(s32* param) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
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
    u32* obj = (u32*)param[0];
    u32 flags = *obj;
    u32 left, top, right, bottom;
    u16 width, height;
    u32 size;
    void* image;
    GXTexObj tex;
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
    GXLoadTexObj(&tex, param[4]);
    GXSetTexCoordGen2(param[2], 0, 0, 0x1E + param[4] * 3, 0, 0x7D);
    GXSetTevOrder(param[3], param[2], param[4], -1);
    GXSetTevColorOp(param[3], 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(param[3], 0, 0, 0, 1, 0);
    GXSetTevColorIn(param[3], 8, 0, 1, 15);
    GXSetTevAlphaIn(param[3], 7, 7, 7, 4);
    GXSetIndTexMtx(1, indMtx, 1);
    GXSetIndTexOrder(param[5], param[2], param[4] + 1);
    GXSetIndTexCoordScale(param[5], 0, 0);
    GXSetTevIndWarp(param[3], param[5], 1, 0, 1);
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
