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

void extInit(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    *(s32*)wp = 0;
    *(void**)(wp + 8) = 0;
}

void extEntry(int param_1, int* param_2, void* initFunc, int param_4, int param_5) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dest, int ch, u32 count);
    extern void DCInvalidateRange(void* ptr, u32 size);
    extern void PSMTXIdentity(void* mtx);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* user, f32 priority);
    extern void extMakeTexture(void);
    extern const f32 float_0_80421888;
    extern ExtWork work[2];
    extern ExtGp* gp;
    static f32 localNormals[6] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, -1.0f,
    };
    static u32 localColors[1] = {0xFFFFFFFF};
    static f32 localUVs[9] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f,
    };
    s32 wp;
    s32 texCount;
    s32 i;
    int* scan;
    void (*init)(void);

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }

    texCount = 0;
    scan = param_2;
    while (*scan != 0) {
        scan += 3;
        texCount++;
    }

    *(void**)(wp + 8) = __memAlloc(2, param_1 << 5);
    *(s32*)wp = param_1;
    *(void**)(wp + 0x30) = (void*)param_4;
    *(void**)(wp + 0x34) = (void*)param_5;
    *(void**)(wp + 0xC) = __memAlloc(2, texCount * 0x34);
    *(s32*)(wp + 4) = texCount;
    *(void**)(wp + 0x10) = __memAlloc(2, param_1 * 0x30);
    *(void**)(wp + 0x14) = __memAlloc(2, texCount * 0x30);
    *(void**)(wp + 0x18) = localNormals;
    *(void**)(wp + 0x1C) = localColors;
    *(void**)(wp + 0x20) = localUVs;
    *(void**)(wp + 0x24) = __memAlloc(2, param_1 << 3);
    *(void**)(wp + 0x28) = __memAlloc(2, texCount << 7);
    DCInvalidateRange(*(void**)(wp + 0x28), texCount << 7);
    *(void**)(wp + 0x2C) = __memAlloc(2, texCount << 2);

    memset(*(void**)(wp + 8), 0, *(s32*)wp << 5);

    for (i = 0; i < param_1; i++) {
        *(s32*)(*(s32*)(wp + 0x24) + i * 8) = i;
        *(void**)(*(s32*)(wp + 8) + i * 0x20 + 0xC) = (void*)(*(s32*)(wp + 0x10) + i * 0x30);
    }

    PSMTXIdentity((void*)(wp + 0x3C));
    dispEntry(0, 1, extMakeTexture, param_2, float_0_80421888);

    init = initFunc;
    if (init != 0) {
        init();
    }
}

void extMakeTexture(s32 unused, s32* texInfo) {
    extern void* memcpy(void* dest, const void* src, u32 size);
    extern void animPoseDisp_MakeExtTexture(f64 a, f64 b, s32 c, s32 d, s32* e, s32* f,
                                            u32* g, s32* h, u32* i, s32* j);
    extern void GXBeginDisplayList(void* list, u32 size);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nverts);
    extern u32 GXEndDisplayList(void);
    extern ExtWork work[2];
    extern ExtGp* gp;
    extern s32 anim_ext_tex_size;
    extern f32 queen_uv[4];
    extern const f32 float_0_80421888;
    extern const f32 float_0p5_804218c0;
    extern const f32 float_4_804218bc;
    extern const f32 float_neg1_804218b8;
    extern const f32 float_neg2_804218b4;
    s32 wp;
    s32 tex;
    s32 i;
    s32 vtxBase;
    s32 displayOffset;
    s32 sizeOffset;
    u32 left;
    u32 right;
    s32 top;
    s32 bottom;
    f32 uvSave[4];
    f32* vtx;
    volatile u16* fifo;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }

    anim_ext_tex_size = 0;
    tex = *(s32*)(wp + 0xC);
    memcpy(uvSave, queen_uv, 0x10);
    queen_uv[0] = float_0_80421888;
    queen_uv[2] = float_0_80421888;
    queen_uv[1] = float_neg2_804218b4 / (f32)*(s32*)(wp + 4);
    queen_uv[3] = float_neg1_804218b8 / (f32)*(s32*)(wp + 4);

    vtxBase = 0;
    displayOffset = 0;
    sizeOffset = 0;
    fifo = (volatile u16*)0xCC008000;
    for (i = 0; i < *(s32*)(wp + 4); i++) {
        animPoseDisp_MakeExtTexture((f64)(f32)texInfo[2], (f64)float_4_804218bc,
                                    texInfo[0], texInfo[1], (s32*)(tex + 4),
                                    (s32*)tex, &left, &top, &right, &bottom);
        vtx = (f32*)(*(s32*)(wp + 0x14) + vtxBase);
        vtx[0] = (f32)(s32)left * float_0p5_804218c0;
        vtx[1] = (f32)-top * float_0p5_804218c0;
        vtx[2] = float_0_80421888;
        vtx[3] = (f32)(s32)right * float_0p5_804218c0;
        vtx[4] = (f32)-top * float_0p5_804218c0;
        vtx[5] = float_0_80421888;
        vtx[6] = (f32)(s32)right * float_0p5_804218c0;
        vtx[7] = (f32)-bottom * float_0p5_804218c0;
        vtx[8] = float_0_80421888;
        vtx[9] = (f32)(s32)left * float_0p5_804218c0;
        vtx[10] = (f32)-bottom * float_0p5_804218c0;
        vtx[11] = float_0_80421888;

        GXBeginDisplayList((void*)(*(s32*)(wp + 0x28) + displayOffset), 0x80);
        GXBegin(0x80, 0, 4);
        *fifo = (u16)sizeOffset;
        *fifo = 0;
        *fifo = (u16)(sizeOffset + 1);
        *fifo = 1;
        *fifo = (u16)(sizeOffset + 2);
        *fifo = 2;
        *fifo = (u16)(sizeOffset + 3);
        *fifo = 3;
        *(u32*)(*(s32*)(wp + 0x2C) + sizeOffset) = GXEndDisplayList();

        texInfo += 3;
        tex += 0x34;
        vtxBase += 0x30;
        displayOffset += 0x80;
        sizeOffset += 4;
    }

    memcpy(queen_uv, uvSave, 0x10);
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


void extMain(void) {
    extern s32 marioStGetSystemLevel(void);
    extern void* camGetPtr(s32 id);
    extern f64 atan2(f64 y, f64 x);
    extern void qqsort(void* base, u32 count, u32 width, void* compareFunc);
    extern s32 compare(void* a, void* b);
    extern f32 sin(f32 x);
    extern f32 cos(f32 x);
    extern ExtWork work[2];
    extern ExtGp* gp;
    extern const f32 float_rad2deg_8042188c;
    extern const f32 float_360_80421890;
    extern const f32 float_180_80421894;
    extern const f32 float_90_80421898;
    extern const f32 float_270_804218a0;
    extern const f32 float_neg90_804218a4;
    extern const f32 float_neg270_804218a8;
    extern const f32 float_neg360_8042189c;
    extern const f32 float_neg180_804218ac;
    extern const f32 float_0p1_804218b0;
    extern const f32 float_0_80421888;
    s32 wp;
    s32 count;
    s32 i;
    u32* pose;
    s32* entry;
    void (*callback)(void);
    void* cam;
    f32 angle;
    f32 diff;
    f32 target;
    f32 s;
    f32 c;

    wp = (s32)work;
    if (gp->useAlt != 0) {
        wp += 0x70;
    }

    if (marioStGetSystemLevel() == 0) {
        callback = *(void (**)(void))(wp + 0x30);
        if (callback != 0) {
            callback();
        }
    }
    callback = *(void (**)(void))(wp + 0x34);
    if (callback != 0) {
        callback();
    }

    pose = *(u32**)(wp + 8);
    entry = *(s32**)(wp + 0x24);
    cam = camGetPtr(4);
    angle = float_rad2deg_8042188c *
            (f32)atan2(-((f32*)((s32)cam + 0x18))[0] + ((f32*)((s32)cam + 0xC))[0],
                       -((f32*)((s32)cam + 0x20))[0] + ((f32*)((s32)cam + 0x14))[0]);
    while (angle >= float_360_80421890) {
        angle -= float_360_80421890;
    }
    if (angle < float_0_80421888) {
        angle += float_360_80421890;
    }
    camGetPtr(4);
    camGetPtr(4);

    count = *(s32*)wp;
    target = float_180_80421894 + angle;
    for (i = count - 1; i >= 0; i--) {
        entry[0] = i;
        if ((pose[0] & 1) != 0) {
            entry[1] = (s32)(f32)pose[4];
        }
        if ((pose[0] & 2) != 0) {
            diff = float_90_80421898 + (angle - (f32)pose[1]);
            while (diff < float_neg360_8042189c) {
                diff += float_360_80421890;
            }
            while (diff >= float_360_80421890) {
                diff -= float_360_80421890;
            }
            if ((float_90_80421898 < diff && diff <= float_270_804218a0) ||
                (diff < float_neg90_804218a4 && float_neg270_804218a8 <= diff)) {
                pose[7] = (u32)target;
            } else {
                cam = camGetPtr(4);
                diff = float_rad2deg_8042188c *
                       (f32)atan2(-((f32*)((s32)cam + 0x18))[0] + ((f32*)((s32)cam + 0xC))[0],
                                  -((f32*)((s32)cam + 0x20))[0] + ((f32*)((s32)cam + 0x14))[0]);
                while (diff >= float_360_80421890) {
                    diff -= float_360_80421890;
                }
                if (diff < float_0_80421888) {
                    diff += float_360_80421890;
                }
                pose[7] = (u32)diff;
            }
            while (float_180_80421894 < (f32)pose[7] - (f32)pose[5]) {
                pose[5] = (u32)((f32)pose[5] + float_360_80421890);
            }
            while ((f32)pose[7] - (f32)pose[5] < float_neg180_804218ac) {
                pose[5] = (u32)((f32)pose[5] - float_360_80421890);
            }
            pose[5] = (u32)((((f32)pose[7] - (f32)pose[5]) * float_0p1_804218b0) + (f32)pose[5]);
            diff = (f32)pose[5];
            while (diff < float_0_80421888) {
                diff += float_360_80421890;
            }
            while (diff >= float_360_80421890) {
                diff -= float_360_80421890;
            }
            s = sin(diff);
            c = cos(diff);
            *(f32*)pose[3] = c;
            *(f32*)(pose[3] + 0x28) = c;
            *(f32*)(pose[3] + 8) = s;
            *(f32*)(pose[3] + 0x20) = -s;
        }
        pose += 8;
        entry += 2;
    }

    if (*(s32*)wp != 0) {
        qqsort(*(void**)(wp + 0x24), *(s32*)wp, 8, compare);
    }
    *(s32*)(wp + 0x38) = -1;
    *(s32*)(wp + 0x6C) = 0;
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

void* extGetPosePtr(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(void**)(wp + 8);
}

s32 extGetPoseNum(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(s32*)wp;
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

void extLoadRenderMode(void) {
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 aop, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(1, 3, 1);
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

void extLoadTextureExit(void) {
    GXTexModeSync();
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
    extern const u32 dat_80421884;
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

void extLoadShadowRenderMode(void) {
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 aop, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
}

void extLoadShadowMtx(void* dst) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    PSMTXCopy(dst, (void*)(wp + 0x3C));
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

const u32 double_to_int_802cc738[2] = {
    0x43300000,
    0x80000000,
};

static u32 sintbl[450] = {
    0x00000000, 0x3C8EF77F, 0x3D0EF241, 0x3D565E46, 0x3D8EDC3C, 0x3DB27ED8,
    0x3DD612C7, 0x3DF99674, 0x3E0E835E, 0x3E20303C, 0x3E31D0C8, 0x3E43636F,
    0x3E54E6E2, 0x3E66598E, 0x3E77BA67, 0x3E8483ED, 0x3E8D204B, 0x3E95B1C8,
    0x3E9E377A, 0x3EA6B0D9, 0x3EAF1D3F, 0x3EB77C03, 0x3EBFCC7D, 0x3EC80DE5,
    0x3ED03FD5, 0x3ED86163, 0x3EE0722A, 0x3EE87161, 0x3EF05E81, 0x3EF83904,
    0x3F000000, 0x3F03D988, 0x3F07A8C6, 0x3F0B6D76, 0x3F0F2746, 0x3F12D5E0,
    0x3F167914, 0x3F1A108C, 0x3F1D9BF6, 0x3F211B1E, 0x3F248DC1, 0x3F27F37C,
    0x3F2B4C2B, 0x3F2E976C, 0x3F31D51B, 0x3F3504F7, 0x3F3826AB, 0x3F3B3A04,
    0x3F3E3EC0, 0x3F41349C, 0x3F441B76, 0x3F46F30A, 0x3F49BB17, 0x3F4C7358,
    0x3F4F1BBD, 0x3F51B3F2, 0x3F543BC5, 0x3F56B314, 0x3F5919AC, 0x3F5B6F4C,
    0x3F5DB3D0, 0x3F5FE719, 0x3F6208D1, 0x3F6418F8, 0x3F66175D, 0x3F6803CD,
    0x3F69DE16, 0x3F6BA637, 0x3F6D5BEE, 0x3F6EFF19, 0x3F708FB8, 0x3F720D78,
    0x3F737868, 0x3F74D068, 0x3F761544, 0x3F7746ED, 0x3F786552, 0x3F797050,
    0x3F7A67E8, 0x3F7B4BE8, 0x3F7C1C61, 0x3F7CD91F, 0x3F7D8234, 0x3F7E177F,
    0x3F7E98FE, 0x3F7F06A3, 0x3F7F605B, 0x3F7FA637, 0x3F7FD817, 0x3F7FF60A,
    0x3F800000, 0x3F7FF60A, 0x3F7FD817, 0x3F7FA637, 0x3F7F605B, 0x3F7F06A3,
    0x3F7E98FE, 0x3F7E177F, 0x3F7D8234, 0x3F7CD91F, 0x3F7C1C61, 0x3F7B4BE8,
    0x3F7A67E8, 0x3F797050, 0x3F786552, 0x3F7746ED, 0x3F761544, 0x3F74D068,
    0x3F737879, 0x3F720D88, 0x3F708FB8, 0x3F6EFF2A, 0x3F6D5BEE, 0x3F6BA637,
    0x3F69DE27, 0x3F6803CD, 0x3F66175D, 0x3F641909, 0x3F6208E1, 0x3F5FE719,
    0x3F5DB3E1, 0x3F5B6F5D, 0x3F5919AC, 0x3F56B325, 0x3F543BD6, 0x3F51B3F2,
    0x3F4F1BBD, 0x3F4C7369, 0x3F49BB17, 0x3F46F30A, 0x3F441B86, 0x3F4134AD,
    0x3F3E3EC0, 0x3F3B3A04, 0x3F3826AB, 0x3F3504F7, 0x3F31D52C, 0x3F2E977D,
    0x3F2B4C2B, 0x3F27F37C, 0x3F248DC1, 0x3F211B2E, 0x3F1D9C06, 0x3F1A108C,
    0x3F167925, 0x3F12D5F1, 0x3F0F2746, 0x3F0B6D76, 0x3F07A8D6, 0x3F03D999,
    0x3F000000, 0x3EF83904, 0x3EF05EA2, 0x3EE87182, 0x3EE0724B, 0x3ED86184,
    0x3ED03FD5, 0x3EC80E06, 0x3EBFCC7D, 0x3EB77C24, 0x3EAF1D60, 0x3EA6B0FB,
    0x3E9E377A, 0x3E95B1C8, 0x3E8D206D, 0x3E84840E, 0x3E77BAAA, 0x3E6659D1,
    0x3E54E6E2, 0x3E43636F, 0x3E31D10B, 0x3E20307F, 0x3E0E83A1, 0x3DF996FB,
    0x3DD6134D, 0x3DB27ED8, 0x3D8EDCC2, 0x3D565F52, 0x3D0EF34D, 0x3C8EF998,
    0x358637BD, 0xBC8EF77F, 0xBD0EF241, 0xBD565D3A, 0xBD8EDC3C, 0xBDB27E52,
    0xBDD612C7, 0xBDF99674, 0xBE0E835E, 0xBE20303C, 0xBE31D0C8, 0xBE43632C,
    0xBE54E69F, 0xBE66594B, 0xBE77BA23, 0xBE8483ED, 0xBE8D204B, 0xBE95B1A7,
    0xBE9E3758, 0xBEA6B0D9, 0xBEAF1D1D, 0xBEB77BE1, 0xBEBFCC5C, 0xBEC80DC3,
    0xBED03FB3, 0xBED86163, 0xBEE0722A, 0xBEE87161, 0xBEF05E81, 0xBEF838E3,
    0xBEFFFFDE, 0xBF03D977, 0xBF07A8C6, 0xBF0B6D76, 0xBF0F2735, 0xBF12D5E0,
    0xBF167914, 0xBF1A107B, 0xBF1D9BF6, 0xBF211B1E, 0xBF248DB0, 0xBF27F36B,
    0xBF2B4C1B, 0xBF2E976C, 0xBF31D51B, 0xBF3504E6, 0xBF38269A, 0xBF3B39F3,
    0xBF3E3EAF, 0xBF41349C, 0xBF441B76, 0xBF46F2FA, 0xBF49BB06, 0xBF4C7358,
    0xBF4F1BAC, 0xBF51B3F2, 0xBF543BC5, 0xBF56B314, 0xBF5919AC, 0xBF5B6F4C,
    0xBF5DB3D0, 0xBF5FE708, 0xBF6208D1, 0xBF6418F8, 0xBF66175D, 0xBF6803BC,
    0xBF69DE16, 0xBF6BA626, 0xBF6D5BDD, 0xBF6EFF19, 0xBF708FA8, 0xBF720D78,
    0xBF737868, 0xBF74D057, 0xBF761533, 0xBF7746ED, 0xBF786541, 0xBF797050,
    0xBF7A67D7, 0xBF7B4BE8, 0xBF7C1C61, 0xBF7CD91F, 0xBF7D8234, 0xBF7E177F,
    0xBF7E98FE, 0xBF7F06A3, 0xBF7F605B, 0xBF7FA626, 0xBF7FD817, 0xBF7FF60A,
    0xBF800000, 0xBF7FF60A, 0xBF7FD817, 0xBF7FA637, 0xBF7F605B, 0xBF7F06A3,
    0xBF7E98FE, 0xBF7E177F, 0xBF7D8234, 0xBF7CD91F, 0xBF7C1C61, 0xBF7B4BE8,
    0xBF7A67E8, 0xBF797050, 0xBF786552, 0xBF7746ED, 0xBF761544, 0xBF74D068,
    0xBF737879, 0xBF720D88, 0xBF708FB8, 0xBF6EFF2A, 0xBF6D5BEE, 0xBF6BA637,
    0xBF69DE27, 0xBF6803CD, 0xBF66176E, 0xBF641909, 0xBF6208E1, 0xBF5FE719,
    0xBF5DB3E1, 0xBF5B6F5D, 0xBF5919BD, 0xBF56B325, 0xBF543BD6, 0xBF51B403,
    0xBF4F1BCE, 0xBF4C7369, 0xBF49BB17, 0xBF46F31B, 0xBF441B86, 0xBF4134AD,
    0xBF3E3ED1, 0xBF3B3A04, 0xBF3826BB, 0xBF350508, 0xBF31D52C, 0xBF2E977D,
    0xBF2B4C3C, 0xBF27F38C, 0xBF248DC1, 0xBF211B2E, 0xBF1D9C06, 0xBF1A109D,
    0xBF167925, 0xBF12D5F1, 0xBF0F2757, 0xBF0B6D87, 0xBF07A8D6, 0xBF03D999,
    0xBF000011, 0xBEF83926, 0xBEF05EC4, 0xBEE87182, 0xBEE0724B, 0xBED86184,
    0xBED03FD5, 0xBEC80E06, 0xBEBFCC9F, 0xBEB77C24, 0xBEAF1D82, 0xBEA6B0FB,
    0xBE9E379B, 0xBE95B1EA, 0xBE8D206D, 0xBE84840E, 0xBE77BAAA, 0xBE6659D1,
    0xBE54E725, 0xBE4363B2, 0xBE31D10B, 0xBE20307F, 0xBE0E83E4, 0xBDF99781,
    0xBDD613D3, 0xBDB27F5F, 0xBD8EDD48, 0xBD565F52, 0xBD0EF45A, 0xBC8EFBB1,
    0xB58637BD, 0x3C8EF566, 0x3D0EF241, 0x3D565D3A, 0x3D8EDBB6, 0x3DB27DCC,
    0x3DD61240, 0x3DF995EE, 0x3E0E831B, 0x3E202FF9, 0x3E31D085, 0x3E43632C,
    0x3E54E65C, 0x3E66594B, 0x3E77BA23, 0x3E8483CB, 0x3E8D2029, 0x3E95B1A7,
    0x3E9E3737, 0x3EA6B0B8, 0x3EAF1D1D, 0x3EB77BE1, 0x3EBFCC3A, 0x3EC80DC3,
    0x3ED03FB3, 0x3ED86141, 0x3EE07208, 0x3EE87161, 0x3EF05E81, 0x3EF838E3,
    0x3EFFFFDE, 0x3F03D977, 0x3F07A8B5, 0x3F0B6D65, 0x3F0F2724, 0x3F12D5D0,
    0x3F167903, 0x3F1A107B, 0x3F1D9BF6, 0x3F211B0D, 0x3F248DB0, 0x3F27F36B,
    0x3F2B4C1B, 0x3F2E975B, 0x3F31D50B, 0x3F3504D5, 0x3F38269A, 0x3F3B39F3,
    0x3F3E3EAF, 0x3F41349C, 0x3F441B76, 0x3F46F2FA, 0x3F49BB06, 0x3F4C7358,
    0x3F4F1BAC, 0x3F51B3E1, 0x3F543BC5, 0x3F56B314, 0x3F59199C, 0x3F5B6F3B,
    0x3F5DB3D0, 0x3F5FE708, 0x3F6208D1, 0x3F6418F8, 0x3F66174C, 0x3F6803BC,
    0x3F69DE16, 0x3F6BA626, 0x3F6D5BDD, 0x3F6EFF19, 0x3F708FA8, 0x3F720D78,
    0x3F737868, 0x3F74D057, 0x3F761533, 0x3F7746DC, 0x3F786541, 0x3F797050,
    0x3F7A67D7, 0x3F7B4BE8, 0x3F7C1C61, 0x3F7CD91F, 0x3F7D8234, 0x3F7E177F,
    0x3F7E98FE, 0x3F7F06A3, 0x3F7F605B, 0x3F7FA626, 0x3F7FD817, 0x3F7FF60A,
};

u8 shadow_dl[0x20] = {
    0x80, 0x00, 0x04, 0x00,
    0x00, 0x01, 0x01, 0x02,
    0x02, 0x03, 0x03, 0x00,
};

f32 shadow_vtx[12] = {
    -1.2f, 0.0f, -1.2f,
    -1.2f, 0.0f, 1.2f,
    1.2f, 0.0f, 1.2f,
    1.2f, 0.0f, -1.2f,
};

u32 gap_07_804164F0_sdata[4] = {0, 0, 0, 0};

f32 shadow_coord[8] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
};

const u32 dat_80421880 = 0xFFFFFFFF;
const u32 dat_80421884 = 0x000000A0;
const f32 float_0_80421888 = 0.0f;
const f32 float_rad2deg_8042188c = 57.29579f;
const f32 float_360_80421890 = 360.0f;
const f32 float_180_80421894 = 180.0f;
const f32 float_90_80421898 = 90.0f;
const f32 float_neg360_8042189c = -360.0f;
const f32 float_270_804218a0 = 270.0f;
const f32 float_neg90_804218a4 = -90.0f;
const f32 float_neg270_804218a8 = -270.0f;
const f32 float_neg180_804218ac = -180.0f;
const f32 float_0p1_804218b0 = 0.1f;
const f32 float_neg2_804218b4 = -2.0f;
const f32 float_neg1_804218b8 = -1.0f;
const f32 float_4_804218bc = 4.0f;
const f32 float_0p5_804218c0 = 0.5f;
const u32 gap_09_804218C4_sdata2 = 0;
