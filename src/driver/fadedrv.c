#include "driver/fadedrv.h"
#define FIFO_S16_FADE (*(volatile s16*)0xCC008000)
#define FIFO_F32_FADE (*(volatile f32*)0xCC008000)
#define READ_F32_FADE(addr) (*(volatile const f32*)&(addr))
typedef struct FadeVecRaw {
    u32 x;
    u32 y;
    u32 z;
} FadeVecRaw;

extern void UnpackTexPalette(void* palette);
extern void DVDMgrClose(s32 entry);
extern const FadeVecRaw vec3_802bf668;
extern void* wp;
extern void animPoseRelease(s32 poseId);
extern void animPaperPoseRelease(s32 poseId);
extern void imgRelease(s32 imageId, void* heap);
extern void* memset(void* dst, int value, unsigned long size);
extern void* __memAlloc(s32 heap, u32 size);
extern void* arcOpen(const char* name, s32 unused0, s32 unused1);
extern char* getMarioStDvdRoot(void);
extern int sprintf(char* str, const char* format, ...);
extern s32 DVDMgrOpen(const char* path, s32 a, s32 b);
extern s32 DVDMgrGetLength(s32 entry);
extern void DVDMgrReadAsync(s32 entry, void* dst, s32 size, s32 offset, void* callback);
extern const char str_fade_tpl_802bf79c[];
extern const char str_PCTs_fade_tpl_802bf7a8[];

extern const f32 float_0_8041f7a8;
extern const f32 float_1_8041f7ac;

extern void* camGetPtr(s32 cameraId);
extern void GXGetProjectionv(void* proj);
extern void GXSetProjection(void* projMtx, s32 projType);
extern void GXSetProjectionv(void* proj);
extern void GXSetZCompLoc(s32 beforeTex);
extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
extern void GXSetNumChans(s32 count);
extern void GXSetChanCtrl(
    s32 chan,
    s32 enable,
    s32 ambSrc,
    s32 matSrc,
    s32 lightMask,
    s32 diffFn,
    s32 attnFn
);
extern void GXSetChanMatColor(s32 chan, void* color);
extern void GXSetNumTevStages(s32 stages);
extern void GXSetNumTexGens(s32 count);
extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevOp(s32 stage, s32 mode);
extern void GXLoadPosMtxImm(void* mtx, s32 id);
extern void GXSetCurrentMtx(s32 id);
extern void GXSetCullMode(s32 mode);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);

extern u32 dat_8041f7a0;
extern u32 dat_8041f7a4;

extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 index);
extern void GXLoadTexObj(void* texObj, s32 mapId);
extern void GXSetTexCoordGen2(
    s32 texCoord,
    s32 func,
    s32 srcParam,
    s32 mtx,
    s32 normalize,
    s32 postMtx
);
extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void GXSetTevColor(s32 reg, void* color);

extern u32 dat_8041f798;
extern u32 dat_8041f79c;
extern u32 unk_80429518;

extern const f32 float_0p04_8041f7b0;
extern const f32 float_255_8041f7b4;
extern const f32 float_2_8041f7b8;

typedef struct FadeVec {
    f32 x;
    f32 y;
    f32 z;
} FadeVec;

typedef union FadeDoubleConv {
    f64 value;
    struct {
        u32 hi;
        u32 lo;
    } words;
} FadeDoubleConv;

extern s32 gp;

extern f64 tan(f64 x);
extern void PSVECSubtract(void* a, void* b, void* out);
extern void PSVECNormalize(void* src, void* dst);
extern f32 PSVECMag(void* vec);

extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXConcat(void* a, void* b, void* ab);
extern void PSMTXMultVec(void* mtx, void* src, void* dst);

extern void animPoseSetGXFunc(s32 poseId, void* callback, void* param);
extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 a, f32 b);
extern void sysWaitDrawSync(void);
extern void GXSetZScaleOffset(f32 scale, f32 offset);
extern f32 z(void);
extern void zFill(void);

extern const f64 double_to_int_mask_802bf678;

extern const f32 float_0_8041f7a8;
extern const f32 float_0p5_8041f7c0;
extern const f32 float_180_8041f7c4;
extern const f32 float_3p1416_8041f7bc;
extern const f32 float_24_8041f7c8;
extern const f32 float_64_8041f7cc;
extern const f32 float_48_8041f7d0;
extern const f32 float_neg32_8041f7d4;
extern const f32 float_608_8041f7d8;
extern const f32 float_480_8041f7dc;
extern const f32 float_32_8041f7e0;
extern const f32 float_neg24_8041f7e4;

void fadeSetNarrowFast(void) {
    *(u16*)((s32)wp + 0xB0) |= 8;
}

void* fadeGetTpl(void) {
    return *(void**)((s32)wp + 0x358);
}

void fadeReset(s32 type) {
    void* entry;

    type *= 0xA8;
    entry = (void*)((s32)wp + type + 0x8);

    if (*(s32*)((s32)entry + 0x6C) >= 0) {
        animPoseRelease(*(s32*)((s32)entry + 0x6C));
    }

    if (*(s32*)((s32)entry + 0x70) >= 0) {
        animPaperPoseRelease(*(s32*)((s32)entry + 0x70));
    }

    if (*(void**)((s32)entry + 0x8C) != NULL) {
        imgRelease(
            *(s32*)((s32)entry + 0x8C),
            *(void**)((s32)entry + 0x68)
        );
    }

    memset(entry, 0, 0xA8);

    *(s32*)((s32)entry + 0x6C) = -1;
    *(s32*)((s32)entry + 0x70) = -1;

    *(f32*)((s32)entry + 0x1C) = float_0_8041f7a8;
    *(f32*)((s32)entry + 0x20) = float_0_8041f7a8;
    *(f32*)((s32)entry + 0x78) = float_0_8041f7a8;
    *(f32*)((s32)entry + 0x7C) = float_0_8041f7a8;
    *(f32*)((s32)entry + 0x90) = float_1_8041f7ac;
}

s32 fadeIsFinish(void) {
    s32 type;

    type = *(s32*)((s32)wp + 0x350);

    if (type == -1) {
        return 1;
    }

    type *= 0xA8;

    return (*(u16*)((s32)wp + type + 0x8) >> 1) & 1;
}

void disp_dummy_black(void) {
    void* cam;
    u32 colorA;
    u32 colorB;
    f32 proj[8];

    cam = camGetPtr(9);

    GXGetProjectionv(proj);
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));

    GXSetZCompLoc(0);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(0, 1, 0, 7);
    GXSetZMode(0, 7, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);

    if ((*(u16*)wp & 0x8000) != 0) {
        colorA = dat_8041f7a0;
        GXSetChanMatColor(4, &colorA);
    } else {
        colorB = dat_8041f7a4;
        GXSetChanMatColor(4, &colorB);
    }

    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 4);

    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXSetCullMode(2);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);

    GXBegin(0x80, 0, 4);

    FIFO_S16_FADE = -0x20;
    FIFO_S16_FADE = 0x18;
    FIFO_S16_FADE = 0;

    FIFO_S16_FADE = 0x20;
    FIFO_S16_FADE = 0x18;
    FIFO_S16_FADE = 0;

    FIFO_S16_FADE = 0x20;
    FIFO_S16_FADE = -0x18;
    FIFO_S16_FADE = 0;

    FIFO_S16_FADE = -0x20;
    FIFO_S16_FADE = -0x18;
    FIFO_S16_FADE = 0;

    GXSetProjectionv(proj);
}

void disp_tec(s32 cameraId) {
    void* cam;
    u8 texObj[0x20];
    u32 colorA;
    u32 colorB;
    u32 color;
    u32 colorTemp;
    s32 alpha;
    f32 tecValue;
f32 tecStep;
f32 tecClamp;

    cam = camGetPtr(cameraId);

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    if ((*(u16*)wp & 0x1000) != 0) {
        TEXGetGXTexObjFromPalette(*(void**)((s32)wp + 0x358), texObj, 2);
        GXLoadTexObj(texObj, 0);
    } else {
        TEXGetGXTexObjFromPalette(*(void**)((s32)wp + 0x358), texObj, 3);
        GXLoadTexObj(texObj, 0);
    }

    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 1, 7, 4, 7);

    if ((*(u16*)wp & 0x1000) != 0) {
        colorA = dat_8041f798;
        GXSetTevColor(1, &colorA);
    } else {
        colorB = dat_8041f79c;
        GXSetTevColor(1, &colorB);
    }

    if ((*(u16*)wp & 0x30) != 0) {
        if ((*(u16*)wp & 0x10) != 0) {
            tecValue = *(f32*)((s32)wp + 0x354);
            tecStep = float_0p04_8041f7b0;
            tecClamp = float_1_8041f7ac;

            tecValue = tecValue + tecStep;
        *(f32*)((s32)wp + 0x354) = tecValue;

            if (*(f32*)((s32)wp + 0x354) > tecClamp) {
           *(f32*)((s32)wp + 0x354) = tecClamp;
         *(u16*)wp &= ~0x10;
        }
        } else {
            tecValue = *(f32*)((s32)wp + 0x354);
            tecStep = float_0p04_8041f7b0;
            tecClamp = float_0_8041f7a8;

            tecValue = tecValue - tecStep;
        *(f32*)((s32)wp + 0x354) = tecValue;

    if (*(f32*)((s32)wp + 0x354) < tecClamp) {
        *(f32*)((s32)wp + 0x354) = tecClamp;
        *(u16*)wp &= ~0x20;
     *(u16*)wp &= ~0x3000;
    }
        }

        colorTemp = unk_80429518;
        alpha = (s32)(float_255_8041f7b4 * *(f32*)((s32)wp + 0x354));
        *((u8*)&colorTemp + 3) = alpha;
        color = colorTemp;

        GXSetTevColor(2, &color);

        GXSetNumTevStages(2);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
        GXSetTevAlphaIn(1, 7, 0, 2, 7);
    }

    GXSetCullMode(2);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXBegin(0x80, 0, 4);

    FIFO_S16_FADE = -0x134;
    FIFO_S16_FADE = 0xF4;
    FIFO_S16_FADE = 0;
    FIFO_F32_FADE = READ_F32_FADE(float_0_8041f7a8);
    FIFO_F32_FADE = READ_F32_FADE(float_0_8041f7a8);

    FIFO_S16_FADE = 0x134;
    FIFO_S16_FADE = 0xF4;
    FIFO_S16_FADE = 0;
    FIFO_F32_FADE = READ_F32_FADE(float_2_8041f7b8);
    FIFO_F32_FADE = READ_F32_FADE(float_0_8041f7a8);

    FIFO_S16_FADE = 0x134;
    FIFO_S16_FADE = -0xF4;
    FIFO_S16_FADE = 0;
    FIFO_F32_FADE = READ_F32_FADE(float_2_8041f7b8);
    FIFO_F32_FADE = READ_F32_FADE(float_2_8041f7b8);

    FIFO_S16_FADE = -0x134;
    FIFO_S16_FADE = -0xF4;
    FIFO_S16_FADE = 0;
    FIFO_F32_FADE = READ_F32_FADE(float_0_8041f7a8);
    FIFO_F32_FADE = READ_F32_FADE(float_2_8041f7b8);
}

void disp_maku(s32 cameraId, void* fade) {
    s32 type;
    void* cam;
    FadeVec diff;
    FadeVec forward;
    f32 fovy;
    f32 distBase;
    f32 dist;
    f32 scale;
    f32 zOfs;
    f32 xRate;
    f32 yRate;
    f32 width;
    f32 height;
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    FadeDoubleConv widthConv0;
    FadeDoubleConv heightConv0;
    FadeDoubleConv widthConv1;
    FadeDoubleConv heightConv1;

    camGetPtr(cameraId);

    type = *(s32*)((s32)fade + 0x4);

   if (type != 0x1C) {
    if (type < 0x1C) {
        if (type == 0x18) {
            goto after_bit2_check;
        }

        if (type < 0x18) {
            if (type >= 0x17) {
                goto check_bit2;
            }

            goto after_bit2_check;
        } else if (type >= 0x1A) {
            goto after_bit2_check;
        }
    } else if (type != 0x40) {
        goto after_bit2_check;
    }
}

check_bit2:
if ((*(u16*)fade & 2) != 0) {
    return;
}

after_bit2_check:
if (type != 0x33) {
    if (type < 0x33) {
        if (type >= 0x29) {
            return;
        }

        if (type < 0x16) {
            return;
        }
    } else {
        if (type >= 0x41) {
            return;
        }

        if (type < 0x3F) {
            return;
        }
    }
}

    if (*(s32*)((s32)fade + 0x6C) < 0) {
     return;
    }

    if ((*(u16*)fade & 4) != 0) {
        cam = camGetPtr(4);

        fovy = *(f32*)((s32)cam + 0x38);
        distBase = float_24_8041f7c8 /
            (f32)tan(
                float_3p1416_8041f7bc *
                ((fovy * float_0p5_8041f7c0) / float_180_8041f7c4)
            );

        cam = camGetPtr(4);
        PSVECSubtract(
            (void*)((s32)cam + 0x0C),
            (void*)((s32)fade + 0x80),
            &diff
        );

        cam = camGetPtr(4);

        forward.x = *(f32*)((s32)cam + 0x13C);
        forward.y = *(f32*)((s32)cam + 0x140);
        forward.z = *(f32*)((s32)cam + 0x144);

        PSVECNormalize(&forward, &forward);

        diff.x *= forward.x;
        diff.y *= forward.y;
        diff.z *= forward.z;

        dist = PSVECMag(&diff);

        scale = dist / distBase;
        zOfs = distBase - dist;

        animPoseSetGXFunc(*(s32*)((s32)fade + 0x6C), NULL, NULL);
        GXSetZScaleOffset(z(), float_0_8041f7a8);
    } else {
        scale = *(f32*)((s32)fade + 0x90);
        zOfs = float_0_8041f7a8;
        zFill();
    }

    widthConv0.words.hi = 0x43300000;
    widthConv0.words.lo = *(u16*)((s32)gp + 0x170);
    width = (f32)(widthConv0.value - double_to_int_mask_802bf678);

    heightConv0.words.hi = 0x43300000;
    heightConv0.words.lo = *(u16*)((s32)gp + 0x172);
    height = (f32)(heightConv0.value - double_to_int_mask_802bf678);

    xRate = ((*(f32*)((s32)fade + 0x78) * scale) + float_0_8041f7a8) / width;
    yRate = ((*(f32*)((s32)fade + 0x7C) * scale) + float_0_8041f7a8) / height;

    PSMTXTrans(
        mtxA,
        float_64_8041f7cc * xRate,
        float_48_8041f7d0 * yRate,
        zOfs
    );

    PSMTXTrans(
        mtxB,
        float_neg32_8041f7d4,
        float_24_8041f7c8,
        float_0_8041f7a8
    );

    PSMTXConcat(mtxA, mtxB, mtxA);

    widthConv1.words.hi = 0x43300000;
    widthConv1.words.lo = *(u16*)((s32)gp + 0x170);
    width = (f32)(widthConv1.value - double_to_int_mask_802bf678);

    heightConv1.words.hi = 0x43300000;
    heightConv1.words.lo = *(u16*)((s32)gp + 0x172);
    height = (f32)(heightConv1.value - double_to_int_mask_802bf678);

    xRate = float_608_8041f7d8 / width;

    PSMTXScale(
        mtxB,
        xRate,
        float_480_8041f7dc / height,
        xRate
    );

    PSMTXConcat(mtxA, mtxB, mtxA);

    PSMTXTrans(
        mtxB,
        float_32_8041f7e0,
        float_neg24_8041f7e4,
        float_0_8041f7a8
    );

    PSMTXConcat(mtxA, mtxB, mtxA);

    animPoseDrawMtx(*(s32*)((s32)fade + 0x6C), mtxA, 1, float_0_8041f7a8, scale);
    animPoseDrawMtx(*(s32*)((s32)fade + 0x6C), mtxA, 2, float_0_8041f7a8, scale);
    animPoseDrawMtx(*(s32*)((s32)fade + 0x6C), mtxA, 3, float_0_8041f7a8, scale);

    sysWaitDrawSync();
}

void fadeSetOffscreenCallback(void* callback, void* param) {
    void* work = wp;

    *(void**)((s32)work + 0x29C) = callback;
    *(void**)((s32)work + 0x2A0) = param;
}

void fadeSetAnimOfsPos(f32 x, f32 y) {
    void* work = wp;

    *(f32*)((s32)work + 0x128) = x;
    *(f32*)((s32)work + 0x12C) = y;
}

void fadeSetSpotPos(f32 x, f32 y) {
    void* work = wp;

    *(f32*)((s32)work + 0x24) = x;
    *(f32*)((s32)work + 0x28) = y;
}

void fadeWhiteOutOff(void) {
    *(u16*)wp &= ~0x4000;
}

void fadeWhiteOutOn(void) {
    *(u16*)wp |= 0x4000;
}

void fadeSoftFocusOff(void) {
    *(u16*)wp &= ~0x2000;
}

void fadeSoftFocusOn(void) {
    *(u16*)wp |= 0x2000;
}

void fadeTecSoftOff(void) {
    *(u16*)wp |= 0x1020;
    *(f32*)((s32)wp + 0x354) = float_1_8041f7ac;
}

void fadeTecSoftOn(void) {
    *(u16*)wp |= 0x1010;
    *(f32*)((s32)wp + 0x354) = float_0_8041f7a8;
}

void fadeTecOff(void) {
    *(u16*)wp &= ~0x1000;
}

void fadeTecOn(void) {
    *(u16*)wp |= 0x1000;
}

void fadeSetAnimVirtualPos(f32 x, f32 y, f32 z) {
    FadeVecRaw pos;
    void* work;

    pos = vec3_802bf668;
    work = wp;

    *(f32*)&pos.x = x;
    *(f32*)&pos.y = y;
    *(f32*)&pos.z = z;

    *(u32*)((s32)work + 0x130) = pos.x;
    *(u32*)((s32)work + 0x134) = pos.y;
    *(u32*)((s32)work + 0x138) = pos.z;

    *(u16*)((s32)work + 0xB0) |= 4;
}

void _callback(void* unused, void* fileInfo) {
    s32 entry;

    entry = *(s32*)((s32)fileInfo + 0x2C);

    UnpackTexPalette(*(void**)((s32)wp + 0x358));
    DVDMgrClose(entry);

    *(s32*)((s32)wp + 0x35C) = 1;
}


u8 fadeMain(void) {
    return 0;
}


void fadeEntry(s32 type, s32 time, void* data) {
}


u8 disp_texture(void) {
    return 0;
}


void zFill(void) {
    extern void* camGetCurPtr(void);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXSetZTexture(s32 op, s32 fmt, u32 bias);
    extern void GXSetColorUpdate(s32 enable);
    extern void GXSetAlphaUpdate(s32 enable);
    extern void PSMTXIdentity(void* mtx);
    extern const f32 float_6p2832_8041f7e8;
    extern const f32 float_360_8041f7ec;
    extern const f32 float_neg10000_8041f7f0;
    extern const u8 depth_image_995[];

    void* cam;
    u8 texObj[0x20];
    f32 mtx[3][4];
    f32 halfDepth;
    f32 width;
    f32 height;
    f32 angle;
    f32 farX;
    f32 negFarX;
    f32 negHalfDepth;

    cam = camGetCurPtr();
    angle = (float_6p2832_8041f7e8 * *(f32*)((s32)cam + 0x38)) / float_360_8041f7ec;
    halfDepth = (*(f32*)((s32)cam + 0x30) + *(f32*)((s32)cam + 0x34)) * float_0p5_8041f7c0;
    halfDepth *= (f32)tan(float_0p5_8041f7c0 * angle);
    farX = halfDepth * *(f32*)((s32)cam + 0x3C);
    negHalfDepth = -halfDepth;
    negFarX = -farX;

    GXInitTexObj(texObj, (void*)depth_image_995, 4, 4, 0x11, 1, 1, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 4);
    GXSetZTexture(2, 0x11, 0);
    GXSetNumChans(0);
    GXSetAlphaCompare(7, 0, 1, 7, 0);
    GXSetZCompLoc(1);
    GXSetBlendMode(2, 1, 0, 3);
    GXSetZMode(1, 7, 1);
    GXSetColorUpdate(0);
    GXSetAlphaUpdate(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 0, 0);
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    FIFO_F32_FADE = negFarX;
    FIFO_F32_FADE = halfDepth;
    FIFO_F32_FADE = READ_F32_FADE(float_neg10000_8041f7f0);
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;

    FIFO_F32_FADE = farX;
    FIFO_F32_FADE = halfDepth;
    FIFO_F32_FADE = READ_F32_FADE(float_neg10000_8041f7f0);
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 0;

    FIFO_F32_FADE = farX;
    FIFO_F32_FADE = negHalfDepth;
    FIFO_F32_FADE = READ_F32_FADE(float_neg10000_8041f7f0);
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 1;

    FIFO_F32_FADE = negFarX;
    FIFO_F32_FADE = negHalfDepth;
    FIFO_F32_FADE = READ_F32_FADE(float_neg10000_8041f7f0);
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 1;

    GXSetZTexture(0, 0x11, 0);
    GXSetColorUpdate(1);
}

void fadeInit(void) {
    void* work;
    s32 minusOne;
    s32 zero;
    f32 oneFloat;
    f32 zeroFloat;

    work = __memAlloc(0, 0x360);
    wp = work;
    memset(work, 0, 0x360);

    work = wp;
    minusOne = -1;
    zero = 0;
    oneFloat = float_1_8041f7ac;
    zeroFloat = float_0_8041f7a8;

    *(s32*)((s32)work + 0x74) = minusOne;
    *(s32*)((s32)work + 0x78) = minusOne;
    *(f32*)((s32)work + 0x24) = zeroFloat;
    *(f32*)((s32)work + 0x28) = zeroFloat;
    *(f32*)((s32)work + 0x80) = zeroFloat;
    *(f32*)((s32)work + 0x84) = zeroFloat;
    *(f32*)((s32)work + 0x98) = oneFloat;
    *(s32*)((s32)work + 0xAC) = zero;

    work = wp;
    *(s32*)((s32)work + 0x11C) = minusOne;
    *(s32*)((s32)work + 0x120) = minusOne;
    *(f32*)((s32)work + 0xCC) = zeroFloat;
    *(f32*)((s32)work + 0xD0) = zeroFloat;
    *(f32*)((s32)work + 0x128) = zeroFloat;
    *(f32*)((s32)work + 0x12C) = zeroFloat;
    *(f32*)((s32)work + 0x140) = oneFloat;
    *(s32*)((s32)work + 0x154) = zero;

    work = wp;
    *(s32*)((s32)work + 0x1C4) = minusOne;
    *(s32*)((s32)work + 0x1C8) = minusOne;
    *(f32*)((s32)work + 0x174) = zeroFloat;
    *(f32*)((s32)work + 0x178) = zeroFloat;
    *(f32*)((s32)work + 0x1D0) = zeroFloat;
    *(f32*)((s32)work + 0x1D4) = zeroFloat;
    *(f32*)((s32)work + 0x1E8) = oneFloat;
    *(s32*)((s32)work + 0x1FC) = zero;

    work = wp;
    *(s32*)((s32)work + 0x26C) = minusOne;
    *(s32*)((s32)work + 0x270) = minusOne;
    *(f32*)((s32)work + 0x21C) = zeroFloat;
    *(f32*)((s32)work + 0x220) = zeroFloat;
    *(f32*)((s32)work + 0x278) = zeroFloat;
    *(f32*)((s32)work + 0x27C) = zeroFloat;
    *(f32*)((s32)work + 0x290) = oneFloat;
    *(s32*)((s32)work + 0x2A4) = zero;

    work = wp;
    *(s32*)((s32)work + 0x314) = minusOne;
    *(s32*)((s32)work + 0x318) = minusOne;
    *(f32*)((s32)work + 0x2C4) = zeroFloat;
    *(f32*)((s32)work + 0x2C8) = zeroFloat;
    *(f32*)((s32)work + 0x320) = zeroFloat;
    *(f32*)((s32)work + 0x324) = zeroFloat;
    *(f32*)((s32)work + 0x338) = oneFloat;
    *(s32*)((s32)work + 0x34C) = zero;

    work = wp;
    *(s32*)((s32)work + 0x350) = minusOne;
    work = wp;
    *(s32*)((s32)work + 0x358) = zero;
    work = wp;
    *(s32*)((s32)work + 0x35C) = zero;
}


f32 z(void) {
    void* work;
    void* cam;
    FadeVec pos;
    f32 divisor;
    f32 value;

    work = wp;
    cam = camGetPtr(4);
    if (*(s32*)((s32)cam + 0x19C) == 0) {
        return float_1_8041f7ac;
    }

    cam = camGetPtr(9);
    PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)work + 0x130), &pos);
    value = ((*(f32*)((s32)cam + 0x110) - *(f32*)((s32)cam + 0x10C)) *
        ((float_1_8041f7ac / -pos.z) *
        (*(f32*)((s32)cam + 0x184) * pos.z + *(f32*)((s32)cam + 0x188)))) +
        *(f32*)((s32)cam + 0x110);
    divisor = value;

    cam = camGetPtr(4);
    PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)work + 0x130), &pos);
    value = ((*(f32*)((s32)cam + 0x110) - *(f32*)((s32)cam + 0x10C)) *
        (float_1_8041f7ac * (*(f32*)((s32)cam + 0x184) * pos.z + *(f32*)((s32)cam + 0x188)))) +
        *(f32*)((s32)cam + 0x110);
    return value / divisor;
}


void fadeTexSetup(void) {
    s32 length;
    s32 entry;
    char path[0x80];

    *(void**)((s32)wp + 0x358) = NULL;
    *(void**)((s32)wp + 0x358) = arcOpen(str_fade_tpl_802bf79c, 0, 0);
    if (*(void**)((s32)wp + 0x358) == NULL) {
        sprintf(path, str_PCTs_fade_tpl_802bf7a8, getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        length = DVDMgrGetLength(entry);
        length = (length + 0x1F) & ~0x1F;
        *(void**)((s32)wp + 0x358) = __memAlloc(0, length);
        *(s32*)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, *(void**)((s32)wp + 0x358), length, 0, _callback);
    } else {
        UnpackTexPalette(*(void**)((s32)wp + 0x358));
        *(s32*)((s32)wp + 0x35C) = 1;
    }
}
