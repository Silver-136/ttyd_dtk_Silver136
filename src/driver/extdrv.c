#include "driver/extdrv.h"

typedef struct ExtWork {
    s32 poseNum;
    f32 unk4;
    void* posePtr;
    u8 padC[0x30];
    f32 mtx[3][4];
    u32 pad6C;
} ExtWork;

typedef struct ExtGp {
    u8 pad0[0x14];
    s32 useAlt;
} ExtGp;

ExtWork work[2];
extern ExtGp* gp;

void GXTexModeSync(void);
void PSMTXCopy(void* dst, void* src);

void extLoadTextureExit(void) {
    GXTexModeSync();
}

s32 extGetPoseNum(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(s32*)wp;
}

void* extGetPosePtr(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(void**)(wp + 8);
}

void extInit(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    *(s32*)wp = 0;
    *(void**)(wp + 8) = 0;
}

s32 compare(void* a, void* b) {
    if (*(f32*)((s32)a + 4) > *(f32*)((s32)b + 4)) {
        return 1;
    }
    if (*(f32*)((s32)a + 4) < *(f32*)((s32)b + 4)) {
        return -1;
    }
    return 0;
}

void extLoadShadowMtx(void* dst) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    PSMTXCopy(dst, (void*)(wp + 0x3C));
}


u8 extMain(void) {
    return 0;
}


u8 extMakeTexture(void) {
    return 0;
}


u8 extEntry(int param_1, int* param_2, void* initFunc, int param_4, int param_5) {
    return 0;
}


void extDrawShadow(void) {
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void PSMTXTransApply(void* src, void* dst, f32 x, f32 y, f32 z);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXCallDisplayList(void* list, u32 nbytes);
    extern u8 shadow_dl[0x20];
    extern const f32 float_0_80421888;
    f32 mtx[3][4];
    f32 transY;
    s32 wp;
    s32 i;
    s32* entry;
    s32* pose;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }

    i = *(s32*)wp - 1;
    entry = (s32*)(*(s32*)(wp + 0x24) + (i << 3));
    while (i >= 0) {
        pose = (s32*)(*(s32*)(wp + 8) + ((*entry) << 5));
        if ((pose[0] & 4) == 0) {
            PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), (void*)pose[3], mtx);
            PSMTXConcat(mtx, (void*)(wp + 0x3C), mtx);
            if ((pose[0] & 0x10) != 0) {
                transY = -*(f32*)(pose[3] + 0x1C);
                transY += *(f32*)((s32)pose + 8);
                PSMTXTransApply(mtx, mtx, float_0_80421888, transY, float_0_80421888);
            }
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
            GXCallDisplayList(shadow_dl, 0x20);
        }
        i--;
        entry -= 2;
    }
}

void extLoadShadowTev(void) {
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 stages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern u32 dat_80421884;
    u32 color;
    u8 texObj[0x20];

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    color = dat_80421884;
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    effGetTexObjN64(0x2C, texObj);
    GXLoadTexObj(texObj, 0);
}

void extLoadTexture(void) {
    extern void GXInvalidateTexAll(void);
    extern u32 GXGetTexObjFmt(void* texObj);
    extern u32 GXGetTexObjHeight(void* texObj);
    extern u32 GXGetTexObjWidth(void* texObj);
    extern u32 GXGetTexBufferSize(u32 width, u32 height, u32 fmt, s32 mipmap, s32 max_lod);
    extern void GXInitTexPreLoadRegion(void* region, u32 tmem_even, u32 size_even, u32 tmem_odd, u32 size_odd);
    extern void* smartTexObj(void* texObj, void** data);
    extern void GXPreLoadEntireTexture(void* texObj, void* region);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    u32 height;
    u32 fmtSize;
    s32 wp;
    s32 tex;
    s32 i;
    u32 used;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    tex = *(s32*)(wp + 0xC);
    used = 0;
    GXInvalidateTexAll();
    i = 0;
    while (i < *(s32*)(wp + 4)) {
        fmtSize = GXGetTexObjFmt((void*)(tex + 4));
        height = GXGetTexObjHeight((void*)(tex + 4));
        fmtSize = GXGetTexBufferSize(GXGetTexObjWidth((void*)(tex + 4)), height, fmtSize, 0, 0);
        if (used < 0x80000 && used + fmtSize >= 0x80000) {
            used = 0x80000;
        }
        GXInitTexPreLoadRegion((void*)(tex + 0x24), used, fmtSize, 0, 0);
        GXPreLoadEntireTexture(smartTexObj((void*)(tex + 4), *(void***)(tex + 0)), (void*)(tex + 0x24));
        used += fmtSize;
        i++;
        tex += 0x34;
    }
    *(u32*)(wp + 0x6C) = used;
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
}

void extPoseDraw(s32 poseNo) {
    extern void* smartTexObj(void* texObj, void** data);
    extern void GXLoadTexObjPreLoaded(void* texObj, void* region, s32 mapId);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void PSMTXInvXpose(void* src, void* dst);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXCallDisplayList(void* list, u32 nbytes);
    f32 mtx[3][4];
    f32 nrm[3][4];
    s32 wp;
    s32* pose;
    s32 texNo;
    s32 tex;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    pose = (s32*)(*(s32*)(wp + 8) + (poseNo << 5));
    if ((pose[0] & 1) != 0) {
        texNo = pose[4];
        if (*(s32*)(wp + 0x38) != texNo) {
            tex = *(s32*)(wp + 0xC) + texNo * 0x34;
            GXLoadTexObjPreLoaded(smartTexObj((void*)(tex + 4), *(void***)(tex + 0)), (void*)(tex + 0x24), 0);
            *(s32*)(wp + 0x38) = texNo;
        }
        PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), (void*)pose[3], mtx);
        GXLoadPosMtxImm(mtx, 0);
        PSMTXInvXpose(mtx, nrm);
        GXLoadNrmMtxImm(nrm, 0);
        GXSetCurrentMtx(0);
        texNo = pose[4];
        GXCallDisplayList((void*)(*(s32*)(wp + 0x28) + (texNo << 7)), *(u32*)(*(s32*)(wp + 0x2C) + (texNo << 2)));
    }
}

void extLoadTev(void) {
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 stages);
    extern u32 dat_80421880;
    extern const f32 float_0_80421888;
    volatile u32 color;
    u32 fogColor;

    color = dat_80421880;
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    fogColor = color;
    GXSetFog(0, float_0_80421888, float_0_80421888, float_0_80421888, float_0_80421888, &fogColor);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

void extDraw(void) {
    extern void* smartTexObj(void* texObj, void** data);
    extern void GXLoadTexObjPreLoaded(void* texObj, void* region, s32 mapId);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXCallDisplayList(void* list, u32 nbytes);
    f32 mtx[3][4];
    s32 wp;
    s32 i;
    s32* entry;
    s32* pose;
    s32 texNo;
    s32 tex;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    i = *(s32*)wp - 1;
    entry = *(s32**)(wp + 0x24);
    while (i >= 0) {
        pose = (s32*)(*(s32*)(wp + 8) + ((*entry) << 5));
        if ((pose[0] & 1) != 0) {
            texNo = pose[4];
            if (texNo != *(s32*)(wp + 0x38)) {
                tex = *(s32*)(wp + 0xC) + texNo * 0x34;
                GXLoadTexObjPreLoaded(smartTexObj((void*)(tex + 4), *(void***)(tex + 0)), (void*)(tex + 0x24), 0);
                *(s32*)(wp + 0x38) = texNo;
            }
            PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), (void*)pose[3], mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
            GXCallDisplayList((void*)(*(s32*)(wp + 0x28) + (texNo << 7)), *(u32*)(*(s32*)(wp + 0x2C) + (texNo << 2)));
        }
        i--;
        entry += 2;
    }
}

void extReset(void) {
    extern void __memFree(s32 heap, void* ptr);
    extern void* memset(void* dst, s32 val, u32 size);
    s32 wp;
    void* ptr;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    ptr = *(void**)(wp + 8);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0xC);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0x10);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0x14);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0x24);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0x28);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    ptr = *(void**)(wp + 0x2C);
    if (ptr != NULL) {
        __memFree(2, ptr);
    }
    memset((void*)wp, 0, 0x70);
}

void extLoadVertex(void) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXSetCullMode(s32 mode);
    ExtWork* wp;

    wp = work;
    if (gp->useAlt != 0) {
        wp++;
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetArray(9, *(void**)((s32)wp + 0x14), 0xC);
    GXSetVtxDesc(0xD, 3);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetArray(0xD, *(void**)((s32)wp + 0x20), 8);
    GXSetCullMode(0);
}

void extLoadShadowVertex(void) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXSetCullMode(s32 mode);
    extern f32 shadow_vtx[12];
    extern f32 shadow_coord[8];

    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxDesc(0xD, 2);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetArray(9, shadow_vtx, 0xC);
    GXSetArray(0xD, shadow_coord, 8);
    GXSetCullMode(0);
}

u8 extLoadShadowRenderMode(void) {
    return 0;
}


u8 extLoadRenderMode(void) {
    return 0;
}
