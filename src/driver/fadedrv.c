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

void _callback(void* unused, void* fileInfo) {
    s32 entry;

    entry = *(s32*)((s32)fileInfo + 0x2C);

    UnpackTexPalette(*(void**)((s32)wp + 0x358));
    DVDMgrClose(entry);

    *(s32*)((s32)wp + 0x35C) = 1;
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

void fadeEntry(s32 type, s32 time, void* data) {
    extern u32 dat_8041f778;
    extern u32 dat_8041f77c;
    extern u32 dat_8041f780;
    extern u32 dat_8041f784;
    extern u32 dat_8041f788;
    extern u32 dat_8041f78c;
    extern const f32 float_3_8041f814;
    extern const char str_OFF_d_maku_up_down_802bf6fc[];
    extern const char str_OFF_d_maku_left_righ_802bf710[];
    extern const char str_OFF_d_maku_center_802bf728[];
    extern const char str_OFF_d_dokan_up_802bf6c4[];
    extern const char str_OFF_d_dokan_down_802bf6b0[];
    extern const char str_OFF_d_dokan_left_802bf6d4[];
    extern const char str_OFF_d_dokan_right_802bf6e8[];
    extern const char str_OFF_d_meku_802bf73c[];
    extern const char str_OFF_d_four_meku_5sec_802bf748[];
    extern const char str_A_1_8041f824[];
    extern const char str_A_2_8041f80c[];
    extern const char str_Z_1_8041f828[];
    extern const char str_Z_2_8041f82c[];
    s32 work;
    s32 entry;
    s32 reset;
    s32 slot;
    s32 pose;
    void* image;

    if (type == 0) {
        return;
    }

    if (type == 0x10) {
        type = 9;
        *(u32*)data = dat_8041f778;
    }
    if (type == 0x11) {
        type = 10;
        *(u32*)data = dat_8041f77c;
    }
    if (type == 0x12) {
        type = 11;
        *(u32*)data = dat_8041f780;
    }
    if (type == 0x13) {
        type = 12;
        *(u32*)data = dat_8041f784;
    }
    if (type == 0x14) {
        type = 14;
        *(u32*)data = dat_8041f788;
    }
    if (type == 0x15) {
        type = 15;
        *(u32*)data = dat_8041f78c;
    }

    if (((type == 1) || (type == 2)) && (time < 1000)) {
        time = 1000;
    }
    if (type == 7) {
        type = 3;
    } else if (type == 8) {
        type = 4;
    } else if ((((u32)(type - 3) <= 2) || (type == 6)) && (time < 1000)) {
        time = 1000;
    }

    work = (s32)wp;
    reset = 0;
    if (type < 0x16) {
        slot = 0;
        reset = work + 0x158;
    } else if ((type < 0x29) || (type == 0x33)) {
        slot = 1;
    } else if (type < 0x31) {
        slot = 2;
        reset = work + 8;
    } else if (type < 0x3F) {
        slot = 3;
        if ((u32)(type - 0x32) <= 8) {
            reset = work + 8;
        }
    } else if (type < 0x41) {
        slot = 4;
    } else {
        return;
    }

    entry = work + 8 + slot * 0xA8;
    if (reset != 0) {
        pose = *(s32*)(reset + 0x6C);
        if (pose >= 0) {
            animPoseRelease(pose);
        }
        pose = *(s32*)(reset + 0x70);
        if (pose >= 0) {
            animPaperPoseRelease(pose);
        }
        image = *(void**)(reset + 0x8C);
        if (image != 0) {
            imgRelease((s32)image, (void*)(reset + 0x68));
        }
        memset((void*)reset, 0, 0xA8);
        *(s32*)(reset + 0x6C) = -1;
        *(s32*)(reset + 0x70) = -1;
        *(f32*)(reset + 0x1C) = float_0_8041f7a8;
        *(f32*)(reset + 0x20) = float_0_8041f7a8;
        *(f32*)(reset + 0x78) = float_0_8041f7a8;
        *(f32*)(reset + 0x7C) = float_0_8041f7a8;
        *(f32*)(reset + 0x90) = float_1_8041f7ac;
        if ((slot == 0) && (type <= 14) && (time != 0)) {
            *(s32*)(work + 0xAC) = 2;
        }
    }

    *(u16*)entry = 0;
    *(u16*)entry |= 1;
    *(s32*)(entry + 4) = type;
    *(s64*)(entry + 0x10) = time;
    *(u32*)(entry + 0x18) = *(u32*)data;
    *(f32*)(entry + 0x24) = float_0_8041f7a8;
    *(f32*)(entry + 0x28) = float_0_8041f7a8;
    *(f32*)(entry + 0x5C) = float_0_8041f7a8;
    *(s32*)(entry + 8) = 0;
    *(f32*)(entry + 0x78) = float_0_8041f7a8;
    *(f32*)(entry + 0x7C) = float_0_8041f7a8;
    *(f32*)(entry + 0x90) = float_1_8041f7ac;
    if ((type != 0x32) && (type != 0x35)) {
        *(s32*)(entry + 0x9C) = 0;
    }
    *(s32*)(work + 0x350) = slot;

    if ((type == 0x22) || (type == 0x28)) {
        *(f32*)(entry + 0x90) = float_3_8041f814;
    }

    switch (type) {
        case 0x16:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_up_down_802bf6fc;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x17:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_up_down_802bf6fc;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x18:
        case 0x1A:
        case 0x40:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_left_righ_802bf710;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x19:
        case 0x1B:
        case 0x3F:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_left_righ_802bf710;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x1C:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_left_righ_802bf710;
            *(const char**)(entry + 0x64) = str_Z_1_8041f828;
            break;
        case 0x1D:
        case 0x25:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_center_802bf728;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x1E:
        case 0x26:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_center_802bf728;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x1F:
        case 0x21:
        case 0x27:
        case 0x28:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_center_802bf728;
            *(const char**)(entry + 0x64) = str_Z_2_8041f82c;
            break;
        case 0x20:
        case 0x24:
            *(const char**)(entry + 0x60) = str_OFF_d_maku_center_802bf728;
            *(const char**)(entry + 0x64) = str_Z_1_8041f828;
            break;
        case 0x29:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_up_802bf6c4;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x2A:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_up_802bf6c4;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x2B:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_down_802bf6b0;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x2C:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_down_802bf6b0;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x2D:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_left_802bf6d4;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x2E:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_left_802bf6d4;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x2F:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_right_802bf6e8;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x30:
            *(const char**)(entry + 0x60) = str_OFF_d_dokan_right_802bf6e8;
            *(const char**)(entry + 0x64) = str_A_2_8041f80c;
            break;
        case 0x31:
        case 0x34:
            *(const char**)(entry + 0x60) = str_OFF_d_meku_802bf73c;
            *(const char**)(entry + 0x64) = str_Z_1_8041f828;
            break;
        case 0x32:
        case 0x35:
            *(const char**)(entry + 0x60) = str_OFF_d_meku_802bf73c;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        case 0x33:
            *(const char**)(entry + 0x60) = str_OFF_d_four_meku_5sec_802bf748;
            *(const char**)(entry + 0x64) = str_A_1_8041f824;
            break;
        default:
            return;
    }
    *(s32*)(entry + 0x68) = 2;
}

void fadeTecOn(void) {
    *(u16*)wp |= 0x1000;
}

void fadeTecOff(void) {
    *(u16*)wp &= ~0x1000;
}

void fadeTecSoftOn(void) {
    *(u16*)wp |= 0x1010;
    *(f32*)((s32)wp + 0x354) = float_0_8041f7a8;
}

void fadeTecSoftOff(void) {
    *(u16*)wp |= 0x1020;
    *(f32*)((s32)wp + 0x354) = float_1_8041f7ac;
}

void fadeSoftFocusOn(void) {
    *(u16*)wp |= 0x2000;
}

void fadeSoftFocusOff(void) {
    *(u16*)wp &= ~0x2000;
}

void fadeWhiteOutOn(void) {
    *(u16*)wp |= 0x4000;
}

void fadeWhiteOutOff(void) {
    *(u16*)wp &= ~0x4000;
}

void fadeSetSpotPos(f32 x, f32 y) {
    void* work = wp;

    *(f32*)((s32)work + 0x24) = x;
    *(f32*)((s32)work + 0x28) = y;
}

void fadeSetAnimOfsPos(f32 x, f32 y) {
    void* work = wp;

    *(f32*)((s32)work + 0x128) = x;
    *(f32*)((s32)work + 0x12C) = y;
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

void fadeSetOffscreenCallback(void* callback, void* param) {
    void* work = wp;

    *(void**)((s32)work + 0x29C) = callback;
    *(void**)((s32)work + 0x2A0) = param;
}


void fadeMain(void) {
    extern f64 __cvt_sll_flt(u32 hi, u32 lo);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void disp_texture(void);
    extern void disp_maku(s32 cameraId, void* fade);
    extern void disp_tec(s32 cameraId);
    extern void disp_dummy_black(void);
    extern s32 animGroupBaseAsync(char* name, s32 heapType, s32 unused);
    extern s32 animPoseEntry(char* name, u32 heapType);
    extern void animPoseRelease(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flags);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 mode);
    extern void animPoseMain(s32 poseId);
    extern f64 animPoseGetLoopTimes(s32 poseId);
    extern void psndSFXOff(s32 sfxId);
    extern s32 psndSFXOn(s32 sfxId);
    extern f64 intplGetValue(f64 start, f64 end, s32 mode, s32 current, s32 total);
    extern const f32 float_0_8041f7a8;
    extern const f32 float_1_8041f7ac;
    extern const f32 float_10_8041f808;
    extern const f32 float_1000_8041f804;
    extern const f32 float_999_8041f81c;
    extern const f32 float_neg100_8041f820;
    extern u32 dat_8041f790;
    extern u32 dat_8041f794;
    extern const f32 float_0p5_8041f7c0;
    extern const f32 float_0p98_8041f810;
    extern const f32 float_3_8041f814;
    extern const f32 float_1p5_8041f818;

    u8* work;
    u8* entry;
    s32 i;
    s32 type;
    s32 state;
    s32 poseId;
    f64 total;
    f64 loops;
    f32 value;
    u16 flags;

    work = (u8*)wp;
    entry = work + 8;

    for (i = 0; i < 5; i++, entry += 0xA8) {
        if ((*(u16*)entry & 1) != 0) {
            type = *(s32*)(entry + 4);
            state = *(s32*)(entry + 8);

            switch (type) {
                case 1:
                case 3:
                case 5:
                case 7:
                case 9:
                case 11:
                    if (state == 0) {
                        if ((*(s32*)(entry + 0x10) == 0) && (*(s32*)(entry + 0x14) == 0)) {
                            *(f32*)(entry + 0x24) = float_0_8041f7a8;
                            *(u16*)entry |= 2;
                            *(s32*)(entry + 8) = state + 1;
                        } else {
                            if (*(s32*)(entry + 0xA4) == 0) {
                                *(f32*)(entry + 0x5C) = *(f32*)(entry + 0x5C) +
                                    (float_1000_8041f804 / (f32)*(u32*)((s32)gp + 4));
                            } else {
                                *(s32*)(entry + 0xA4) = *(s32*)(entry + 0xA4) - 1;
                            }

                            total = __cvt_sll_flt(*(u32*)(entry + 0x10), *(u32*)(entry + 0x14));
                            value = (f32)((f64)*(f32*)(entry + 0x5C) / total);
                            *(f32*)(entry + 0x24) = float_1_8041f7ac - value;
                            *(f32*)(entry + 0x28) = *(f32*)(entry + 0x28) + float_10_8041f808;
                            if (*(f32*)(entry + 0x24) < float_0_8041f7a8) {
                                *(f32*)(entry + 0x24) = float_0_8041f7a8;
                                *(u16*)entry |= 2;
                                *(s32*)(entry + 8) = state + 1;
                            }
                        }
                    }
                    break;

                case 0:
                case 2:
                case 4:
                case 6:
                case 8:
                case 10:
                case 12:
                case 13:
                case 14:
                case 15:
                    if (state == 0) {
                        if ((*(s32*)(entry + 0x10) == 0) && (*(s32*)(entry + 0x14) == 0)) {
                            *(f32*)(entry + 0x24) = float_1_8041f7ac;
                            *(u16*)entry |= 2;
                            *(s32*)(entry + 8) = state + 1;
                        } else {
                            *(f32*)(entry + 0x5C) = *(f32*)(entry + 0x5C) +
                                (float_1000_8041f804 / (f32)*(u32*)((s32)gp + 4));
                            total = __cvt_sll_flt(*(u32*)(entry + 0x10), *(u32*)(entry + 0x14));
                            *(f32*)(entry + 0x24) = (f32)((f64)*(f32*)(entry + 0x5C) / total);
                            *(f32*)(entry + 0x28) = *(f32*)(entry + 0x28) - float_10_8041f808;
                            if (float_1_8041f7ac < *(f32*)(entry + 0x24)) {
                                *(f32*)(entry + 0x24) = float_1_8041f7ac;
                                *(u16*)entry |= 2;
                                *(s32*)(entry + 8) = state + 1;
                            }
                        }
                    }
                    break;

                case 0x16:
                    if (state == 0) {
                        if (animGroupBaseAsync(*(char**)(entry + 0x60), *(s32*)(entry + 0x68), 0) != 0) {
                            if (*(s32*)(entry + 0x6C) > -1) animPoseRelease(*(s32*)(entry + 0x6C));
                            poseId = animPoseEntry(*(char**)(entry + 0x60), *(u32*)(entry + 0x68));
                            *(s32*)(entry + 0x6C) = poseId;
                            animPoseSetMaterialFlagOn(poseId, 0x1800);
                            animPoseSetAnim(poseId, *(char**)(entry + 0x64), 1);
                            *(u32*)(entry + 0x74) = dat_8041f790;
                            animPoseSetMaterialFlagOff(poseId, 0x40);
                            *(s32*)(entry + 8) = 1;
                            state = 1;
                        }
                    }
                    if (state == 1) {
                        animPoseMain(*(s32*)(entry + 0x6C));
                        if (animPoseGetLoopTimes(*(s32*)(entry + 0x6C)) >= (f64)float_1_8041f7ac) {
                            *(u16*)entry |= 2;
                            *(s32*)(entry + 8) += 1;
                        }
                    } else if (state == 2) {
                        animPoseMain(*(s32*)(entry + 0x6C));
                    }
                    break;

                case 0x17:
                    if (state == 0) {
                        if (animGroupBaseAsync(*(char**)(entry + 0x60), *(s32*)(entry + 0x68), 0) == 0) break;
                        if (*(s32*)(entry + 0x6C) > -1) animPoseRelease(*(s32*)(entry + 0x6C));
                        poseId = animPoseEntry(*(char**)(entry + 0x60), *(u32*)(entry + 0x68));
                        *(s32*)(entry + 0x6C) = poseId;
                        animPoseSetMaterialFlagOn(poseId, 0x1800);
                        animPoseSetAnim(poseId, *(char**)(entry + 0x64), 1);
                        *(u32*)(entry + 0x74) = dat_8041f794;
                        animPoseSetMaterialFlagOff(poseId, 0x40);
                        *(s32*)(entry + 8) = 1;
                    }
                    loops = animPoseGetLoopTimes(*(s32*)(entry + 0x6C));
                    if (loops >= (f64)float_0p5_8041f7c0) {
                        u32 color = *(u32*)(entry + 0x74);
                        u8* bytes = (u8*)&color;
                        bytes[0] = (u8)((f32)bytes[0] * float_0p98_8041f810);
                        bytes[1] = (u8)((f32)bytes[1] * float_0p98_8041f810);
                        bytes[2] = (u8)((f32)bytes[2] * float_0p98_8041f810);
                        *(u32*)(entry + 0x74) = color;
                        animPoseSetMaterialEvtColor(*(s32*)(entry + 0x6C), &color);
                        animPoseSetMaterialFlagOn(*(s32*)(entry + 0x6C), 0x40);
                    }
                    animPoseMain(*(s32*)(entry + 0x6C));
                    if (animPoseGetLoopTimes(*(s32*)(entry + 0x6C)) >= (f64)float_1_8041f7ac) {
                        *(u16*)entry |= 2;
                        *(s32*)(entry + 8) += 1;
                        animPoseRelease(*(s32*)(entry + 0x6C));
                        *(s32*)(entry + 0x6C) = -1;
                        entry[0x6D] = 0xFF;
                        entry[0x6E] = 0xFF;
                        entry[0x6F] = 0xFF;
                    }
                    break;

                case 0x18:
                case 0x19:
                case 0x1A:
                case 0x1B:
                case 0x1C:
                case 0x1D:
                case 0x1E:
                case 0x1F:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                case 0x25:
                case 0x26:
                case 0x27:
                case 0x28:
                case 0x33:
                case 0x3F:
                case 0x40:
                    if (state == 0) {
                        if (animGroupBaseAsync(*(char**)(entry + 0x60), *(s32*)(entry + 0x68), 0) != 0) {
                            switch (type) {
                                case 0x18:
                                case 0x1B:
                                case 0x28:
                                    *(s32*)(entry + 0x98) = psndSFXOn(0x40325);
                                    break;
                                case 0x19:
                                case 0x1C:
                                case 0x40:
                                    *(s32*)(entry + 0x98) = psndSFXOn(0x40326);
                                    break;
                                case 0x1D:
                                case 0x22:
                                    *(s32*)(entry + 0x98) = psndSFXOn(0x40321);
                                    break;
                                case 0x1E:
                                case 0x23:
                                    *(s32*)(entry + 0x98) = psndSFXOn(0x40322);
                                    break;
                            }
                            if (*(s32*)(entry + 0x6C) > -1) {
                                animPoseRelease(*(s32*)(entry + 0x6C));
                            }
                            poseId = animPoseEntry(*(char**)(entry + 0x60), *(u32*)(entry + 0x68));
                            *(s32*)(entry + 0x6C) = poseId;
                            animPoseSetMaterialFlagOn(poseId, 0x1800);
                            animPoseSetAnim(*(s32*)(entry + 0x6C), *(char**)(entry + 0x64), 1);
                            animPoseSetMaterialFlagOff(*(s32*)(entry + 0x6C), 0x40);
                            *(s32*)(entry + 8) = 1;
                            state = 1;
                            if (type == 0x21 || type == 0x26) {
                                *(f32*)(entry + 0x90) = float_3_8041f814;
                                *(s32*)(entry + 0x94) = 0;
                            }
                        }
                    }
                    if ((state == 1) || (state == 2)) {
                        if (*(s32*)(entry + 0x6C) > -1) {
                            animPoseMain(*(s32*)(entry + 0x6C));
                            if (type == 0x21 || type == 0x26) {
                                s32 frames;
                                s32 counter;
                                if ((*(u16*)entry & 8) == 0) {
                                    frames = (*(s32*)((s32)gp + 4) * 180) / 60;
                                    if (*(s32*)(entry + 0x94) < frames) {
                                        counter = *(s32*)(entry + 0x94) + 1;
                                        *(s32*)(entry + 0x94) = counter;
                                        *(f32*)(entry + 0x90) = (f32)intplGetValue(
                                            (f64)float_3_8041f814, (f64)float_1_8041f7ac,
                                            12, counter, frames);
                                    }
                                } else {
                                    frames = (*(s32*)((s32)gp + 4) * 90) / 60;
                                    if (*(s32*)(entry + 0x94) < frames) {
                                        counter = *(s32*)(entry + 0x94) + 1;
                                        *(s32*)(entry + 0x94) = counter;
                                        *(f32*)(entry + 0x90) = (f32)intplGetValue(
                                            (f64)float_1p5_8041f818, (f64)float_1_8041f7ac,
                                            12, counter, frames);
                                    }
                                }
                            }
                            loops = animPoseGetLoopTimes(*(s32*)(entry + 0x6C));
                            if (loops >= (f64)float_1_8041f7ac) {
                                s32 canFinish = 1;
                                if (type == 0x21 || type == 0x26) {
                                    s32 frames = ((*(u16*)entry & 8) == 0) ?
                                        (*(s32*)((s32)gp + 4) * 180) / 60 :
                                        (*(s32*)((s32)gp + 4) * 90) / 60;
                                    canFinish = (*(s32*)(entry + 0x94) == frames);
                                }
                                if (canFinish) {
                                    *(u16*)entry |= 2;
                                    *(s32*)(entry + 8) = *(s32*)(entry + 8) + 1;
                                    if ((type == 0x19) || (type == 0x1C) || (type == 0x40)) {
                                        psndSFXOff(*(s32*)(entry + 0x98));
                                        animPoseRelease(*(s32*)(entry + 0x6C));
                                        *(s32*)(entry + 0x6C) = -1;
                                    }
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
            }

            if (type < 0x1D) {
                if (type < 0x10) {
                    if (type < 0x0B) {
                        if ((type < 7) && (type > 4)) {
                            dispEntry(4, 7, disp_texture, wp, float_0_8041f7a8);
                        } else {
                            dispEntry(9, 0, disp_texture, wp, float_0_8041f7a8);
                        }
                    } else {
                        if (type > 0x0D) {
                            dispEntry(4, 7, disp_texture, wp, float_0_8041f7a8);
                        } else {
                            dispEntry(8, 0, disp_texture, wp, float_1000_8041f804);
                        }
                    }
                } else if (type < 0x18) {
                    if (type < 0x16) {
                        dispEntry(9, 0, disp_texture, wp, float_0_8041f7a8);
                    } else {
                        dispEntry(9, 0, disp_maku, entry, float_0_8041f7a8);
                    }
                } else {
                    if (type < 0x1B) {
                        dispEntry(6, 0, disp_maku, entry, float_0_8041f7a8);
                    } else {
                        dispEntry(10, 0, disp_maku, entry, float_0_8041f7a8);
                    }
                }
            } else if (type == 0x33) {
                dispEntry(6, 0, disp_maku, entry, float_0_8041f7a8);
            } else {
                if (type < 0x33) {
                    if (type < 0x29) {
                        if (type > 0x22) {
                            dispEntry(9, 0, disp_maku, entry, float_0_8041f7a8);
                        } else {
                            dispEntry(6, 0, disp_maku, entry, float_0_8041f7a8);
                        }
                    } else {
                        if (type < 0x31) {
                            if ((*(s32*)(entry + 0x70) > -1) && (*(void**)(entry + 0x9C) != NULL)) {
                                dispEntry(1, 0, *(void**)(entry + 0x9C), *(void**)(entry + 0xA0), float_0_8041f7a8);
                            }
                        }
                    }
                } else {
                    if (type > 0x40) {
                        dispEntry(9, 0, disp_texture, wp, float_0_8041f7a8);
                    } else if (type > 0x3E) {
                        dispEntry(10, 0, disp_maku, entry, float_0_8041f7a8);
                    } else {
                        if ((*(s32*)(entry + 0x70) > -1) && (*(void**)(entry + 0x9C) != NULL)) {
                            dispEntry(1, 0, *(void**)(entry + 0x9C), *(void**)(entry + 0xA0), float_0_8041f7a8);
                        }
                    }
                }
            }
        }
    }

    flags = *(u16*)work;
    if ((flags & 0x3000) != 0) {
        dispEntry(8, 0, disp_tec, wp, float_999_8041f81c);
    }
    if ((flags & 0x8000) != 0) {
        dispEntry(9, 0, disp_dummy_black, wp, float_neg100_8041f820);
    }
    if ((flags & 0x4000) != 0) {
        dispEntry(4, 7, disp_dummy_black, wp, float_1000_8041f804);
    }
}

void disp_texture(void) {
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXIdentity(void* mtx);
    extern void GXInitTexObj(void* obj, const void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern const f32 float_100_8041f7f4;
    extern const f32 float_neg0p5_8041f7f8;
    extern const f32 float_deg2rad_8041f7fc;
    extern const f32 float_304_8041f800;
    extern const u8 i4tex_949;

    void* fade;
    void* cam;
    void* texPalette;
    u32 tevColor;
    f32 proj[7];
    u8 texObj[0x20];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    f32 baseMtx[3][4];
    f32 value;
    f32 radius;
    f32 negSize;
    s32 alpha;

    cam = camGetPtr(8);
    fade = wp;

    switch (*(u32*)((s32)fade + 0xC)) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            if ((*(u16*)((s32)fade + 0x8) & 2) != 0) {
                return;
            }
            break;
        case 0:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        default:
            break;
    }

    GXGetProjectionv(proj);
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);

    GXSetCullMode(2);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    if (*(s32*)((s32)fade + 0xC) < 9) {
        if (*(s32*)((s32)fade + 0xC) < 3) {
            if (*(s32*)((s32)fade + 0xC) > 0) {
                GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
                GXSetCurrentMtx(0);

                texPalette = *(void**)((s32)wp + 0x358);
                TEXGetGXTexObjFromPalette(texPalette, texObj, 0);
                GXLoadTexObj(texObj, 0);

                if ((*(u16*)((s32)fade + 0x8) & 2) == 0) {
                    value = *(f32*)((s32)fade + 0x2C);
                    radius = float_100_8041f7f4 * value;
                    radius = radius * value;
                    radius = value * radius;
                    radius = value * radius;
                    radius = value * radius;

                    PSMTXTrans(baseMtx, float_0p5_8041f7c0, float_0p5_8041f7c0, float_0_8041f7a8);
                    PSMTXScale(scaleMtx, radius, radius, float_1_8041f7ac);
                    PSMTXTrans(transMtx, float_neg0p5_8041f7f8, float_neg0p5_8041f7f8, float_0_8041f7a8);
                    PSMTXConcat(baseMtx, scaleMtx, scaleMtx);
                    PSMTXConcat(scaleMtx, transMtx, (void*)((s32)fade + 0x34));
                } else if (*(s32*)((s32)fade + 0xC) == 2) {
                    PSMTXTrans((void*)((s32)fade + 0x34), float_1_8041f7ac, float_1_8041f7ac, float_0_8041f7a8);
                }
            }
        } else if (*(s32*)((s32)fade + 0xC) < 7) {
            GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
            GXSetCurrentMtx(0);

            texPalette = *(void**)((s32)wp + 0x358);
            TEXGetGXTexObjFromPalette(texPalette, texObj, 1);
            GXLoadTexObj(texObj, 0);

            if ((*(u16*)((s32)fade + 0x8) & 2) == 0) {
                value = *(f32*)((s32)fade + 0x2C);
                radius = float_100_8041f7f4 * value;
                radius = radius * value;
                radius = value * radius;
                radius = value * radius;
                radius = value * radius;

                PSMTXTrans(baseMtx, float_0p5_8041f7c0, float_0p5_8041f7c0, float_0_8041f7a8);
                PSMTXScale(scaleMtx, radius, radius, float_1_8041f7ac);
                PSMTXRotRad(rotMtx, 'z', float_deg2rad_8041f7fc * *(f32*)((s32)fade + 0x30));

                PSMTXTrans(
                    transMtx,
                    float_neg0p5_8041f7f8 - (*(f32*)((s32)fade + 0x24) / (f32)*(u16*)((s32)gp + 0x170)),
                    float_neg0p5_8041f7f8 + (*(f32*)((s32)fade + 0x28) / (f32)*(u16*)((s32)gp + 0x172)),
                    float_0_8041f7a8
                );

                PSMTXConcat(baseMtx, scaleMtx, scaleMtx);
                PSMTXConcat(scaleMtx, rotMtx, scaleMtx);
                PSMTXConcat(scaleMtx, transMtx, (void*)((s32)fade + 0x34));
            } else if (*(s32*)((s32)fade + 0xC) == 4 || *(s32*)((s32)fade + 0xC) == 6) {
                PSMTXTrans((void*)((s32)fade + 0x34), float_1_8041f7ac, float_1_8041f7ac, float_0_8041f7a8);
            }
        }
    } else if (*(s32*)((s32)fade + 0xC) == 13) {
        GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXInitTexObj(texObj, &i4tex_949, 1, 1, 0, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        PSMTXIdentity((void*)((s32)fade + 0x34));
    } else if (*(s32*)((s32)fade + 0xC) < 16) {
        GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXInitTexObj(texObj, &i4tex_949, 1, 1, 0, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        PSMTXIdentity((void*)((s32)fade + 0x34));

        alpha = (s32)(float_255_8041f7b4 * *(f32*)((s32)fade + 0x2C));
        *(u8*)((s32)fade + 0x23) = (u8)alpha;
    }

    GXLoadTexMtxImm((void*)((s32)fade + 0x34), 0x1E, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    tevColor = *(u32*)((s32)fade + 0x20);
    GXSetTevColor(1, &tevColor);

    GXBegin(0x80, 0, 4);

    negSize = -float_304_8041f800;

    FIFO_F32_FADE = negSize;
    FIFO_F32_FADE = float_304_8041f800;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_0_8041f7a8;

    FIFO_F32_FADE = float_304_8041f800;
    FIFO_F32_FADE = float_304_8041f800;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_1_8041f7ac;
    FIFO_F32_FADE = float_0_8041f7a8;

    FIFO_F32_FADE = float_304_8041f800;
    FIFO_F32_FADE = negSize;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_1_8041f7ac;
    FIFO_F32_FADE = float_1_8041f7ac;

    FIFO_F32_FADE = negSize;
    FIFO_F32_FADE = negSize;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_0_8041f7a8;
    FIFO_F32_FADE = float_1_8041f7ac;

    GXSetProjectionv(proj);
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

s32 fadeIsFinish(void) {
    s32 type;

    type = *(s32*)((s32)wp + 0x350);

    if (type == -1) {
        return 1;
    }

    type *= 0xA8;

    return (*(u16*)((s32)wp + type + 0x8) >> 1) & 1;
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

void* fadeGetTpl(void) {
    return *(void**)((s32)wp + 0x358);
}

void fadeSetNarrowFast(void) {
    *(u16*)((s32)wp + 0xB0) |= 8;
}
