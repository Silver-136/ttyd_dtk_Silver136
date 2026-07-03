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

void envSetYamiView(void* mtx) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    PSMTXCopy(mtx, (void*)((s32)ptr + 0xC0));
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

void envReInit(void) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    memset(ptr, 0, 0xF0);
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


u8 envAddTev(int param_1, int param_2) {
    return 0;
}


void envDisp_DepthOfField(void) {
    ;
}


u8 envTevLoadTexMtxImm(void) {
    return 0;
}


u8 envSetWater(void) {
    return 0;
}


void envGlare(void* data) {
    ;
}


void envDisp_FF(void) {
    ;
}


void envDisp_Blur(void) {
    ;
}


u8 envSetYami(int param_1) {
    return 0;
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


void envMain(void) {
    dispEntry(4, 7, envCapture, float_20_804248f8, 0);
    dispEntry(4, 7, envDisp, float_10_80424894, 0);
    dispEntry(4, 7, envCapture2, float_0_804248ac, 0);
    dispEntry(4, 8, envDisp2, float_5000_8042493c, 0);
}
