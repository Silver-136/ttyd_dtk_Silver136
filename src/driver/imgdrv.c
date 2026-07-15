#include "driver/imgdrv.h"

extern void* wp;

void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int value, u32 size);

void imgSetShadow(void* img, s32 shadow) {
    *(s32*)((s32)img + 0x100) = shadow;
}

void imgClearVirtualPoint(void* img) {
    *(u32*)((s32)img + 0xCC) |= 8;
}

void imgSetVirtualPoint(void* img, void* point) {
    u32 flags;
    u32 y;
    u32 x;
    u32 z;

    flags = *(u32*)((s32)img + 0xCC);
    x = *(u32*)((s32)point + 0);
    flags &= ~8;
    y = *(u32*)((s32)point + 4);
    *(u32*)((s32)img + 0xCC) = flags;
    z = *(u32*)((s32)point + 8);
    *(u32*)((s32)img + 0xF0) = x;
    *(u32*)((s32)img + 0xF4) = y;
    *(u32*)((s32)img + 0xF8) = z;
}

void imgInit(void) {
    *(s32*)wp = 10;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x158);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x158);
}


u8 imgDisp(s32 param_1, void* param_2) {
    extern void* wp;
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void animSetPaperTexMtx(void* m0, void* m1, void* m2);
    extern void animSetPaperTexObj(void* t0, void* t1, void* t2, void* d0, void* d1, void* d2, s32 flag0, s32 flag1);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void animPoseSetStartTime(s32 poseId, s32 mode, s32 start, s32 end);
    extern void animPoseMain(s32 poseId);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flag);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void zFill(void);
    extern void* camGetPtr(s32 cameraId);
    extern f64 tan(f64 x);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECNormalize(void* src, void* dst);
    extern f32 PSVECMag(void* v);
    extern void* DEMOGetRenderModeObj(void);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    extern s32 strcmp(const char* a, const char* b);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern void sysWaitDrawSync(void);
    extern const f32 float_0_80420370;
    extern const f32 float_1_80420374;
    extern const f32 float_64_80420378;
    extern const f32 float_48_8042037c;
    extern const f32 float_neg32_80420380;
    extern const f32 float_24_80420384;
    extern const f32 float_32_80420388;
    extern const f32 float_neg24_8042038c;
    extern const f32 float_3p1416_80420390;
    extern const f32 float_0p5_80420394;
    extern const f32 float_180_80420398;

    u32* img;
    void* texObjPtrs[3];
    void* texDataPtrs[3];
    void* texMtxPtrs[3];
    f32 viewVec[3];
    f32 camVec[3];
    f32 texTrans[3][4];
    f32 texScale[3][4];
    f32 tmpMtx[3][4];
    f32 drawMtx[3][4];
    f32 texMtx[3][3][4];
    void** texObjOut;
    void** texDataOut;
    void** texMtxOut;
    f32 (*curTexMtx)[4];
    u32* part;
    s32 firstPart;
    s32 i;
    u32 flags;
    f32 srcX;
    f32 srcY;
    f32 srcW;
    f32 srcH;
    f32 partX;
    f32 partY;
    f32 partW;
    f32 partH;
    s32 poseId;
    void* animName;
    void* camera;
    void* modeObj;
    f32 dist;
    f32 baseDist;
    f32 drawScale;
    f32 drawZ;
    f32 drawX;
    f32 drawY;
    f32 drawW;
    f32 drawH;
    f32 screenX;
    f32 screenY;
    f32 renderW;
    f32 renderH;
    u32* other;
    s32 count;
    void* baseA;
    void* baseB;

    (void)param_1;

    img = (u32*)param_2;
    texObjOut = texObjPtrs;
    texDataOut = texDataPtrs;
    curTexMtx = texMtx[0];
    texMtxOut = texMtxPtrs;
    part = img;
    firstPart = -1;
    i = 0;

    do {
        flags = part[0];
        if ((flags & 4) == 0) {
            *texObjOut = 0;
            *texDataOut = 0;
        } else if ((flags & 0x10) != 0) {
            *texObjOut = 0;
            *texDataOut = 0;
        } else {
            if ((img[0x33] & 4) != 0) {
                srcX = (f32)*(u16*)((s32)part + 8);
                srcY = (f32)*(u16*)((s32)part + 0xA);
                srcW = (f32)*(u16*)((s32)part + 0xC);
                srcH = (f32)*(u16*)((s32)part + 0xE);
            } else {
                srcX = (f32)*(u16*)((s32)part + 4);
                srcY = (f32)*(u16*)((s32)part + 6);
                srcW = (f32)*(u16*)((s32)img + 0xE4);
                srcH = (f32)*(u16*)((s32)img + 0xE6);
            }

            partX = (f32)*(u16*)((s32)part + 8);
            partY = (f32)*(u16*)((s32)part + 0xA);
            partW = (f32)*(u16*)((s32)part + 0xC);
            partH = (f32)*(u16*)((s32)part + 0xE);
            PSMTXTrans(texTrans, -((partX - srcX) / partW), -((partY - srcY) / partH), float_0_80420370);
            PSMTXScale(texScale, srcW / partW, srcH / partH, float_1_80420374);
            PSMTXConcat(texTrans, texScale, curTexMtx);
            *texObjOut = (void*)((s32)part + 0x1C);
            *texDataOut = *(void**)((s32)part + 0x3C);
        }

        if (*texObjOut != 0) {
            *texMtxOut = curTexMtx;
            if (firstPart == -1) {
                firstPart = i;
            }
        } else {
            *texMtxOut = 0;
        }

        i++;
        texObjOut++;
        texDataOut++;
        curTexMtx += 3;
        texMtxOut++;
        part += 0x11;
    } while (i < 3);

    if (firstPart == -1) {
        img[0x33] |= 0x10000000;
    } else {
        img[0x33] &= 0xEFFFFFFF;
        animSetPaperTexMtx(texMtxPtrs[0], texMtxPtrs[1], texMtxPtrs[2]);
        animSetPaperTexObj(texObjPtrs[0], texObjPtrs[1], texObjPtrs[2],
                           texDataPtrs[0], texDataPtrs[1], texDataPtrs[2],
                           ((-((s32)img[4]) - 1) | ((s32)img[4] + 1)) >> 31,
                           ((-((s32)img[0x15]) - 1) | ((s32)img[0x15] + 1)) >> 31);

        poseId = (s32)img[0x41];
        animName = (void*)img[0x42];
        if (animName != 0) {
            animPoseSetAnim(poseId, animName, 0);
        } else {
            animName = 0;
        }
        animPoseSetStartTime(poseId, (s32)animName, img[0x44], img[0x45]);
        animPoseMain(poseId);
        animPoseSetMaterialLightFlagOn(poseId, 1);
        *(f32*)((s32)img + 0x118) = animPoseGetLoopTimes(poseId);

        if ((img[0x33] & 8) != 0) {
            zFill();
            drawScale = float_1_80420374;
            drawZ = float_0_80420370;
        } else {
            camera = camGetPtr(4);
            baseDist = float_24_80420384 / (f32)tan((f64)(float_3p1416_80420390 * ((*(f32*)((s32)camera + 0x38) * float_0p5_80420394) / float_180_80420398)));
            camera = camGetPtr(4);
            PSVECSubtract((void*)((s32)camera + 0x0C), (void*)((s32)img + 0xF0), camVec);
            camera = camGetPtr(4);
            viewVec[0] = *(f32*)((s32)camera + 0x13C);
            viewVec[1] = *(f32*)((s32)camera + 0x140);
            viewVec[2] = *(f32*)((s32)camera + 0x144);
            PSVECNormalize(viewVec, viewVec);
            camVec[0] *= viewVec[0];
            camVec[1] *= viewVec[1];
            camVec[2] *= viewVec[2];
            dist = PSVECMag(camVec);
            drawScale = dist / baseDist;
            drawZ = baseDist - dist;
        }

        part = img + (firstPart * 0x11);
        if ((img[0x33] & 4) != 0) {
            drawX = (f32)*(u16*)((s32)part + 8);
            drawY = (f32)*(u16*)((s32)part + 0xA);
            drawW = (f32)*(u16*)((s32)part + 0xC);
            drawH = (f32)*(u16*)((s32)part + 0xE);
        } else {
            drawX = (f32)*(u16*)((s32)part + 4);
            drawY = (f32)*(u16*)((s32)part + 6);
            drawW = (f32)*(u16*)((s32)img + 0xE4);
            drawH = (f32)*(u16*)((s32)img + 0xE6);
        }

        modeObj = DEMOGetRenderModeObj();
        renderH = (f32)*(u16*)((s32)modeObj + 6);
        screenY = float_48_8042037c * (-(*(f32*)((s32)img + 0xEC) + drawY) / renderH);
        modeObj = DEMOGetRenderModeObj();
        renderW = (f32)*(u16*)((s32)modeObj + 4);
        screenX = float_64_80420378 * ((*(f32*)((s32)img + 0xE8) + drawX) / renderW);

        PSMTXTrans(drawMtx, screenX, screenY, drawZ);
        PSMTXTrans(tmpMtx, float_neg32_80420380, float_24_80420384, float_0_80420370);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);

        modeObj = DEMOGetRenderModeObj();
        renderW = (f32)*(u16*)((s32)modeObj + 4);
        modeObj = DEMOGetRenderModeObj();
        renderH = (f32)*(u16*)((s32)modeObj + 6);
        modeObj = DEMOGetRenderModeObj();
        PSMTXScale(tmpMtx, drawW / (f32)*(u16*)((s32)modeObj + 4), drawH / renderH, drawW / renderW);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);

        PSMTXTrans(tmpMtx, float_32_80420388, float_neg24_8042038c, float_0_80420370);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);
        PSMTXConcat((void*)((s32)img + 0x120), drawMtx, drawMtx);

        animPoseDrawMtx(poseId, drawMtx, 1, float_0_80420370, drawScale);
        animPoseDrawMtx(poseId, drawMtx, 2, float_0_80420370, drawScale);
        animPoseDrawMtx(poseId, drawMtx, 3, float_0_80420370, drawScale);
        animSetPaperTexMtx(0, 0, 0);

        other = *(u32**)((s32)wp + 4);
        i = 0;
        count = *(s32*)wp;
        while (i < count) {
            flags = other[0x33];
            if ((flags & 1) != 0 && (s32)other[0x41] != -1 && (flags & 2) == 0 &&
                strcmp((const char*)((s32)other + 0xD4), (const char*)((s32)img + 0xD4)) != 0) {
                baseA = animPoseGetAnimBaseDataPtr((s32)img[0x41]);
                baseB = animPoseGetAnimBaseDataPtr((s32)other[0x41]);
                if (strcmp((const char*)((s32)baseB + 4), (const char*)((s32)baseA + 4)) == 0) {
                    break;
                }
            }
            i++;
            other += 0x56;
        }
        if (i < *(s32*)wp) {
            sysWaitDrawSync();
        }
    }
}

u8 imgShadowDisp(s32 param_1, void* param_2) {
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void animSetPaperTexMtx(void* m0, void* m1, void* m2);
    extern void animSetPaperTexObj(void* t0, void* t1, void* t2, void* d0, void* d1, void* d2, s32 flag0, s32 flag1);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void animPoseSetStartTime(s32 poseId, s32 mode, s32 start, s32 end);
    extern void animPoseMain(s32 poseId);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flag);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* DEMOGetRenderModeObj(void);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    extern void shadowProjShadowDrawCountUp(void);
    extern void shadowDepthShadowDrawCountUp(void);
    extern const f32 float_0_80420370;
    extern const f32 float_1_80420374;
    extern const f32 float_48_8042037c;
    extern const f32 float_64_80420378;
    extern const f32 float_neg32_80420380;
    extern const f32 float_24_80420384;
    extern const f32 float_32_80420388;
    extern const f32 float_neg24_8042038c;

    u32* img;
    void* mtxPtrs[3];
    void* texData[3];
    void* texObjs[3];
    f32 drawMtx[3][4];
    f32 tmpMtx[3][4];
    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    f32 texMtx[3][3][4];
    s32 firstPart;
    s32 i;
    u32* part;
    void** texObjOut;
    void** texDataOut;
    void** mtxOut;
    f32 (*curMtx)[4];
    u32 flags;
    f32 partX;
    f32 partY;
    f32 partW;
    f32 partH;
    f32 srcX;
    f32 srcY;
    f32 srcW;
    f32 srcH;
    s32 poseId;
    void* animName;
    void* modeObj;
    f32 one;
    f32 zero;
    f32 screenX;
    f32 screenY;
    f32 drawX;
    f32 drawY;
    f32 drawW;
    f32 drawH;
    f32 renderW;
    f32 renderH;
    s32 shadowType;

    (void)param_1;

    img = (u32*)param_2;
    texObjOut = texObjs;
    texDataOut = texData;
    curMtx = texMtx[0];
    part = img;
    mtxOut = mtxPtrs;
    firstPart = -1;
    i = 0;

    do {
        flags = part[0];
        if ((flags & 4) == 0) {
            *texObjOut = 0;
            *texDataOut = 0;
        } else if ((flags & 0x10) != 0) {
            *texObjOut = 0;
            *texDataOut = 0;
        } else {
            if ((img[0x33] & 4) != 0) {
                srcX = (f32)*(u16*)((s32)part + 8);
                srcY = (f32)*(u16*)((s32)part + 0xA);
                srcW = (f32)*(u16*)((s32)part + 0xC);
                srcH = (f32)*(u16*)((s32)part + 0xE);
            } else {
                srcX = (f32)*(u16*)((s32)part + 4);
                srcY = (f32)*(u16*)((s32)part + 6);
                srcW = (f32)*(u16*)((s32)img + 0xE4);
                srcH = (f32)*(u16*)((s32)img + 0xE6);
            }

            partX = (f32)*(u16*)((s32)part + 8);
            partY = (f32)*(u16*)((s32)part + 0xA);
            partW = (f32)*(u16*)((s32)part + 0xC);
            partH = (f32)*(u16*)((s32)part + 0xE);

            PSMTXTrans(transMtx, -((partX - srcX) / partW), -((partY - srcY) / partH), float_0_80420370);
            PSMTXScale(scaleMtx, srcW / partW, srcH / partH, float_1_80420374);
            PSMTXConcat(transMtx, scaleMtx, curMtx);
            *texObjOut = (void*)((s32)part + 0x1C);
            *texDataOut = *(void**)((s32)part + 0x3C);
        }

        if (*texObjOut != 0) {
            *mtxOut = curMtx;
            if (firstPart == -1) {
                firstPart = i;
            }
        } else {
            *mtxOut = 0;
        }

        i++;
        texObjOut++;
        texDataOut++;
        curMtx += 3;
        mtxOut++;
        part += 0x11;
    } while (i < 3);

    if (firstPart == -1) {
        img[0x33] |= 0x10000000;
    } else {
        img[0x33] &= 0xEFFFFFFF;
        animSetPaperTexMtx(mtxPtrs[0], mtxPtrs[1], mtxPtrs[2]);
        animSetPaperTexObj(texObjs[0], texObjs[1], texObjs[2],
                           texData[0], texData[1], texData[2],
                           ((-((s32)img[4]) - 1) | ((s32)img[4] + 1)) >> 31,
                           ((-((s32)img[0x15]) - 1) | ((s32)img[0x15] + 1)) >> 31);

        poseId = (s32)img[0x41];
        animName = (void*)img[0x42];
        if (animName != 0) {
            animPoseSetAnim(poseId, animName, 0);
        } else {
            animName = 0;
        }
        animPoseSetStartTime(poseId, (s32)animName, img[0x44], img[0x45]);
        animPoseMain(poseId);
        animPoseSetMaterialLightFlagOn(poseId, 1);
        *(f32*)((s32)img + 0x118) = animPoseGetLoopTimes(poseId);

        flags = img[0x33];
        if ((flags & 8) != 0) {
            one = float_1_80420374;
            zero = float_0_80420370;
        } else {
            one = float_1_80420374;
            zero = float_0_80420370;
        }

        part = img + (firstPart * 0x11);
        if ((flags & 4) != 0) {
            drawX = (f32)*(u16*)((s32)part + 8);
            drawY = (f32)*(u16*)((s32)part + 0xA);
            drawW = (f32)*(u16*)((s32)part + 0xC);
            drawH = (f32)*(u16*)((s32)part + 0xE);
        } else {
            drawX = (f32)*(u16*)((s32)part + 4);
            drawY = (f32)*(u16*)((s32)part + 6);
            drawW = (f32)*(u16*)((s32)img + 0xE4);
            drawH = (f32)*(u16*)((s32)img + 0xE6);
        }

        modeObj = DEMOGetRenderModeObj();
        renderH = (f32)*(u16*)((s32)modeObj + 6);
        screenY = float_48_8042037c * (-(*(f32*)((s32)img + 0xEC) + drawY) / renderH);

        modeObj = DEMOGetRenderModeObj();
        renderW = (f32)*(u16*)((s32)modeObj + 4);
        screenX = float_64_80420378 * ((*(f32*)((s32)img + 0xE8) + drawX) / renderW);

        PSMTXTrans(drawMtx, screenX, screenY, zero);
        PSMTXTrans(tmpMtx, float_neg32_80420380, float_24_80420384, float_0_80420370);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);

        modeObj = DEMOGetRenderModeObj();
        renderW = (f32)*(u16*)((s32)modeObj + 4);
        modeObj = DEMOGetRenderModeObj();
        renderH = (f32)*(u16*)((s32)modeObj + 6);
        modeObj = DEMOGetRenderModeObj();
        PSMTXScale(tmpMtx, drawW / (f32)*(u16*)((s32)modeObj + 4), drawH / renderH, drawW / renderW);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);

        PSMTXTrans(tmpMtx, float_32_80420388, float_neg24_8042038c, float_0_80420370);
        PSMTXConcat(drawMtx, tmpMtx, drawMtx);

        animPoseDrawMtx(poseId, drawMtx, 1, float_0_80420370, one);
        animPoseDrawMtx(poseId, drawMtx, 2, float_0_80420370, one);

        shadowType = *(s32*)((s32)img + 0x100);
        switch (shadowType) {
            case 1:
                shadowProjShadowDrawCountUp();
                break;
            case 2:
                shadowDepthShadowDrawCountUp();
                break;
        }
    }
}

u8 imgCapture_Prim(int param_1, int param_2) {
    extern void offscreenGetTexObj(s32, void*, void*);
    extern s32 GXGetTexObjWidth(void*);
    extern s32 GXGetTexObjHeight(void*);
    extern s32 GXGetTexObjFmt(void*);
    extern s32 GXGetTexBufferSize(u32, u32, s32, s32, u32);
    extern void* smartAlloc(u32, s32);
    extern void* memcpy(void*, void*, u32);
    extern void DCFlushRange(void*, u32);
    extern void GXInitTexObj(void*, void*, u16, u16, s32, s32, s32, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void offscreenGetBoundingBox(s32, void*, void*, void*, void*);
    extern void GXSetTexCopySrc(u32, u32, u32, u32);
    extern void GXSetTexCopyDst(u16, u16, u32, u32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXCopyTex(void*, s32);
    extern void GXPixModeSync(void);
    extern const f32 float_0_80420370;
    u32* part;
    u32* texObj;
    u32* imagePtr;
    void* alloc;
    u32 size;
    s32 width;
    s32 height;
    s32 fmt;
    s32 id;
    s32 storageType;

    part = (u32*)(param_1 + param_2 * 0x44);
    if ((*part & 4) != 0) {
        id = part[4];
        *part |= 0x10;
        if (id != -1) {
            storageType = part[0x10];
            if (storageType == 0) {
                offscreenGetTexObj(id, &texObj, &imagePtr);
                if (texObj != 0) {
                    part[7] = texObj[0];
                    part[8] = texObj[1];
                    part[9] = texObj[2];
                    part[10] = texObj[3];
                    part[11] = texObj[4];
                    part[12] = texObj[5];
                    part[13] = texObj[6];
                    part[14] = texObj[7];
                    part[15] = (u32)imagePtr;
                    offscreenGetBoundingBox(id, (void*)((s32)part + 8), (void*)((s32)part + 10),
                                            (void*)((s32)part + 12), (void*)((s32)part + 14));
                    *(u16*)((s32)part + 12) = *(u16*)((s32)part + 12) - *(u16*)((s32)part + 8);
                    *(u16*)((s32)part + 14) = *(u16*)((s32)part + 14) - *(u16*)((s32)part + 10);
                    *part &= ~0x10;
                }
            } else {
                if ((*part & 0x100) != 0) {
                    offscreenGetTexObj(id, &texObj, &imagePtr);
                    if (texObj != 0) {
                        width = GXGetTexObjWidth(texObj);
                        height = GXGetTexObjHeight(texObj);
                        fmt = GXGetTexObjFmt(texObj);
                        size = GXGetTexBufferSize(width, height, fmt, 0, 0);
                        if (part[0x10] == 1) {
                            alloc = smartAlloc(size, 1);
                            part[15] = (u32)alloc;
                        } else if (part[0x10] == 2) {
                            alloc = smartAlloc(size, 0);
                            part[15] = (u32)alloc;
                        }
                        memcpy(*(void**)part[15], (void*)*imagePtr, size);
                        DCFlushRange(*(void**)part[15], size);
                        GXInitTexObj(part + 7, *(void**)part[15], width, height, fmt, 0, 0, 0);
                        GXInitTexObjLOD(part + 7, 0, 0, float_0_80420370, float_0_80420370,
                                        float_0_80420370, 0, 0, 0);
                        offscreenGetBoundingBox(id, (void*)((s32)part + 8), (void*)((s32)part + 10),
                                                (void*)((s32)part + 12), (void*)((s32)part + 14));
                        *(u16*)((s32)part + 12) = *(u16*)((s32)part + 12) - *(u16*)((s32)part + 8);
                        *(u16*)((s32)part + 14) = *(u16*)((s32)part + 14) - *(u16*)((s32)part + 10);
                        *part &= ~0x100;
                    }
                }
                if ((*part & 0x100) == 0) {
                    *part &= ~0x10;
                }
            }
        } else {
            storageType = part[0x10];
            if (storageType == 0 || (*part & 0x100) != 0) {
                *(u16*)((s32)part + 8) = *(u16*)((s32)part + 4);
                *(u16*)((s32)part + 10) = *(u16*)((s32)part + 6);
                *(u16*)((s32)part + 12) = *(u16*)(param_1 + 0xE4);
                *(u16*)((s32)part + 14) = *(u16*)(param_1 + 0xE6);
                height = *(u16*)((s32)part + 14);
                width = *(u16*)((s32)part + 12);
                fmt = part[6];
                size = GXGetTexBufferSize(width, height, fmt, 0, 0);
                if (storageType == 1) {
                    alloc = smartAlloc(size, 1);
                } else if (storageType == 2) {
                    alloc = smartAlloc(size, 0);
                } else {
                    alloc = smartAlloc(size, 3);
                }
                GXSetTexCopySrc(*(u16*)((s32)part + 8), *(u16*)((s32)part + 10), width, height);
                GXSetTexCopyDst(width, height, part[5], 0);
                GXSetZMode(1, 3, 1);
                GXCopyTex(*(void**)alloc, 0);
                GXPixModeSync();
                GXInitTexObj(part + 7, *(void**)alloc, width, height, fmt, 0, 0, 0);
                GXInitTexObjLOD(part + 7, 0, 0, float_0_80420370, float_0_80420370,
                                float_0_80420370, 0, 0, 0);
                part[15] = (u32)alloc;
                if (storageType == 0) {
                    *part &= ~0x10;
                } else {
                    *part &= ~0x100;
                }
            }
            if ((*part & 0x100) == 0) {
                *part &= ~0x10;
            }
        }
    }
    return 0;
}

void zFill(void) {
    extern void* camGetCurPtr(void);
    extern f64 tan(f64 x);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 stages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetZTexture(s32 op, s32 fmt, u32 bias);
    extern void GXSetNumChans(s32 count);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetColorUpdate(s32 enable);
    extern void GXSetAlphaUpdate(s32 enable);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void PSMTXIdentity(void* mtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern const f32 float_0p5_80420394;
    extern const f32 float_6p2832_8042039c;
    extern const f32 float_360_804203a0;
    extern const f32 float_neg1000_804203a4;
    extern const u8 depth_image_444[];
    void* cam;
    u8 texObj[0x20];
    f32 mtx[3][4];
    f32 halfDepth;
    f32 angle;
    f32 z;
    f32 negHalfDepth;
    f32 negFarX;
    f32 farX;

    cam = camGetCurPtr();
    angle = (float_6p2832_8042039c * *(f32*)((s32)cam + 0x38)) / float_360_804203a0;
    halfDepth = (*(f32*)((s32)cam + 0x30) + *(f32*)((s32)cam + 0x34)) * float_0p5_80420394;
    halfDepth *= (f32)tan(float_0p5_80420394 * angle);
    farX = halfDepth * *(f32*)((s32)cam + 0x3C);
    negHalfDepth = -halfDepth;
    negFarX = -farX;

    GXInitTexObj(texObj, (void*)depth_image_444, 4, 4, 0x11, 1, 1, 0);
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

    z = float_neg1000_804203a4;
    *(volatile f32*)0xCC008000 = negFarX;
    *(volatile f32*)0xCC008000 = halfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = farX;
    *(volatile f32*)0xCC008000 = halfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = farX;
    *(volatile f32*)0xCC008000 = negHalfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile f32*)0xCC008000 = negFarX;
    *(volatile f32*)0xCC008000 = negHalfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 1;

    GXSetZTexture(0, 0x11, 0);
    GXSetColorUpdate(1);
}

u8 imgDisp_ProjPlane(s32 param_1, int param_2) {
    extern void PSMTXIdentity(void* mtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void mapSetMaterialLight(s32 flag, void* pos);
    extern void mapSetMaterialLastStageBlend(u32 flags, void* color0, void* color1);
    extern void mapSetMaterialTev(s32 texCount, s32 drawMode, u32 flags, void* mtx);
    extern void* camGetCurPtr(void);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetAlphaUpdate(s32 enable);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern const f32 float_0_80420370;
    extern const f32 float_neg32_80420380;
    extern const f32 float_24_80420384;
    extern const f32 float_32_80420388;
    extern const f32 float_neg24_8042038c;
    extern const u32 vec3_802c2a98[];
    extern u32 dat_80420368;
    extern u32 dat_8042036c;
    u32 mode;
    u32 flags;
    f32 mtx[3][4];
    f32 outMtx[3][4];
    u32 color0;
    u32 color1;
    u32 src[3];
    u32 dst[3];
    void* cam;

    mode = *(s32*)(param_2 + 0x100);
    if (mode == 1) {
        flags = 0x20000;
        PSMTXIdentity(mtx);
        GXSetBlendMode(1, 4, 5, 0);
    } else if ((s32)mode < 1) {
        if ((s32)mode >= 0) {
            return 0;
        }
        flags = 0;
    } else if ((s32)mode < 3) {
        flags = 0x40000;
        PSMTXTrans(mtx, float_0_80420370, float_0_80420370, *(f32*)(param_2 + 0x11C));
        GXSetBlendMode(3, 4, 5, 0);
    } else {
        flags = 0;
    }

    src[0] = vec3_802c2a98[0];
    src[1] = vec3_802c2a98[1];
    src[2] = vec3_802c2a98[2];
    PSMTXMultVec(mtx, src, src);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    mapSetMaterialLight(0, dst);
    color1 = dat_8042036c;
    color0 = dat_80420368;
    mapSetMaterialLastStageBlend(flags, &color0, &color1);
    mapSetMaterialTev(0, 0, flags, mtx);
    cam = camGetCurPtr();
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, outMtx);
    GXLoadPosMtxImm(outMtx, 0);
    GXSetCurrentMtx(0);
    GXSetAlphaUpdate(1);
    GXSetZMode(0, 3, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCullMode(0);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_neg32_80420380;
    *(volatile f32*)0xCC008000 = float_24_80420384;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_32_80420388;
    *(volatile f32*)0xCC008000 = float_24_80420384;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_32_80420388;
    *(volatile f32*)0xCC008000 = float_neg24_8042038c;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_neg32_80420380;
    *(volatile f32*)0xCC008000 = float_neg24_8042038c;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
}

void imgMain(void) {
    extern void* gp;
    extern void* wp;
    extern void dispEntry(s32 layer, s32 order, void* callback, f32 z, void* user);
    extern void animPoseSetMaterialFlagOff(void* pose, u32 flag);
    extern void animPoseSetMaterialFlagOn(void* pose, u32 flag);
    extern u8 imgDisp(void);
    extern u8 imgShadowDisp(void);
    extern u8 imgDisp_ProjPlane(s32 param_1, int param_2);
    extern s32 imgCapture(s32 layer);
    extern const f32 float_0_80420370;
    s32 i;
    s32 count;
    void* entry;
    s32 localFlag;

    localFlag = ((u32)(-*(s32*)((s32)gp + 0x14)) | *(u32*)((s32)gp + 0x14)) >> 31;
    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        u32 flags = *(u32*)((s32)entry + 0xCC);
        if ((flags & 1) != 0 &&
            *(s32*)((s32)entry + 0x104) != -1 &&
            (flags & 2) == 0) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if (entryFlag == 2 || entryFlag == localFlag) {
                s32 part = 0;
                if ((*(u32*)((s32)entry + 0x0) & 4) != 0) {
                    part = 0;
                } else if ((*(u32*)((s32)entry + 0x44) & 4) != 0) {
                    part = 1;
                } else if ((*(u32*)((s32)entry + 0x88) & 4) != 0) {
                    part = 2;
                } else {
                    part = 3;
                }

                if (part != 3) {
                    dispEntry(*(s32*)((s32)entry + 0x150), 1, imgDisp, *(f32*)((s32)entry + 0xFC), entry);
                    switch (*(s32*)((s32)entry + 0x100)) {
                        case 0:
                            animPoseSetMaterialFlagOff(*(void**)((s32)entry + 0x104), 0x10000);
                            break;
                        case 1:
                            dispEntry(2, 1, imgShadowDisp, float_0_80420370, entry);
                            dispEntry(6, 5, imgDisp_ProjPlane, float_0_80420370, entry);
                            break;
                        case 2:
                            animPoseSetMaterialFlagOn(*(void**)((s32)entry + 0x104), 0x10000);
                            dispEntry(2, 6, imgShadowDisp, float_0_80420370, entry);
                            dispEntry(6, 5, imgDisp_ProjPlane, float_0_80420370, entry);
                            break;
                    }
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }

    i = 0;
    while (i < 13) {
        dispEntry(i, 7, imgCapture, float_0_80420370, 0);
        i++;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 imgEntry(s32 name, s32 flag) {
    extern void* wp;
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void PSMTXIdentity(void* mtx);
    extern const f32 float_0_80420370;
    s32 count;
    void* entry;
    s32 i;
    s32 ret;
    s32 zero;
    s32 negOne;
    s32 six;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 &&
            *(s32*)((s32)entry + 0xD0) == flag &&
            strcmp((char*)((s32)entry + 0xD4), (char*)name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }

    ret = 0;
    entry = *(void**)((s32)wp + 4);
    if (count > 0) {
        do {
            if ((*(u32*)((s32)entry + 0xCC) & 1) == 0) {
                break;
            }
            ret++;
            entry = (void*)((s32)entry + 0x158);
            count--;
        } while (count != 0);
    }

    *(u32*)((s32)entry + 0xCC) = 9;
    *(s32*)((s32)entry + 0xD0) = flag;
    strcpy((char*)((s32)entry + 0xD4), (char*)name);

    zero = 0;
    negOne = -1;
    six = 6;
    *(s32*)((s32)entry + 0x104) = negOne;
    *(s32*)((s32)entry + 0x108) = zero;
    *(u16*)((s32)entry + 0xE4) = zero;
    *(u16*)((s32)entry + 0xE6) = zero;
    *(f32*)((s32)entry + 0xE8) = float_0_80420370;
    *(f32*)((s32)entry + 0xEC) = float_0_80420370;
    *(f32*)((s32)entry + 0xFC) = float_0_80420370;
    *(f32*)((s32)entry + 0x118) = float_0_80420370;
    *(s32*)((s32)entry + 0x100) = zero;
    *(f32*)((s32)entry + 0x11C) = float_0_80420370;
    PSMTXIdentity((void*)((s32)entry + 0x120));

    *(s32*)((s32)entry + 0x150) = six;
    *(s32*)((s32)entry + 0x154) = 4;
    *(s32*)((s32)entry + 0x0) = zero;
    *(u16*)((s32)entry + 0x4) = zero;
    *(u16*)((s32)entry + 0x6) = zero;
    *(s32*)((s32)entry + 0x10) = negOne;
    *(s32*)((s32)entry + 0x14) = six;
    *(s32*)((s32)entry + 0x18) = six;
    *(s32*)((s32)entry + 0x40) = zero;
    *(s32*)((s32)entry + 0x44) = zero;
    *(u16*)((s32)entry + 0x48) = zero;
    *(u16*)((s32)entry + 0x4A) = zero;
    *(s32*)((s32)entry + 0x54) = negOne;
    *(s32*)((s32)entry + 0x58) = six;
    *(s32*)((s32)entry + 0x5C) = six;
    *(s32*)((s32)entry + 0x84) = zero;
    *(s32*)((s32)entry + 0x88) = zero;
    *(u16*)((s32)entry + 0x8C) = zero;
    *(u16*)((s32)entry + 0x8E) = zero;
    *(s32*)((s32)entry + 0x98) = negOne;
    *(s32*)((s32)entry + 0x9C) = six;
    *(s32*)((s32)entry + 0xA0) = six;
    *(s32*)((s32)entry + 0xC8) = zero;
    *(s32*)((s32)entry + 0x9C) = 0x27;
    *(s32*)((s32)entry + 0xA0) = 1;
    return ret;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void imgAutoRelease(s32 value) {
    extern void* wp;
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    s32 count;
    void* entry;
    s32 i;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 && *(s32*)((s32)entry + 0xD0) == value) {
            void* part = entry;
            s32 j = 0;
            while (j < 3) {
                u32 flags = *(u32*)part;
                if ((flags & 4) != 0) {
                    *(u32*)part = flags & ~0x10;
                    if ((*(u32*)part & 0x100) != 0) {
                        *(u32*)part = 0;
                    } else if (*(s32*)((s32)part + 0x40) == 0) {
                        *(u32*)part = 0;
                    } else {
                        *(u32*)part = 0;
                        smartFree(*(void**)((s32)part + 0x3C));
                    }
                }
                j++;
                part = (void*)((s32)part + 0x44);
            }
            *(u32*)((s32)entry + 0xCC) = 0;
            memset(entry, 0, 0x158);
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
}

void imgRelease(void* image) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    void* part = image;
    s32 i = 0;

    while (i < 3) {
        u32 flags = *(u32*)part;
        if ((flags & 4) != 0) {
            *(u32*)part = flags & ~0x10;
            if ((*(u32*)part & 0x100) != 0) {
                *(u32*)part = 0;
            } else if (*(s32*)((s32)part + 0x40) == 0) {
                *(u32*)part = 0;
            } else {
                *(u32*)part = 0;
                smartFree(*(void**)((s32)part + 0x3C));
            }
        }
        i++;
        part = (void*)((s32)part + 0x44);
    }
    *(u32*)((s32)image + 0xCC) = 0;
    memset(image, 0, 0x158);
}

s32 imgCapture(s32 layer) {
    extern void* wp;
    extern void* gp;
    extern void imgCapture_Prim(void* img, s32 index);
    s32 i = 0;
    s32 count;
    void* entry;
    s32 flag;
    s32 localFlag;

    flag = *(s32*)((s32)gp + 0x14);
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    localFlag = ((u32)(-flag) | (u32)flag) >> 31;

    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 &&
            *(s32*)((s32)entry + 0x104) != -1) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if ((entryFlag == 2 || entryFlag == localFlag) &&
                *(s32*)((s32)entry + 0x154) == layer) {
                s32 j = 0;
                while (j < 3) {
                    imgCapture_Prim(entry, j);
                    j++;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return 0;
}

void* imgNameToPtr(s32 name, s32 flag) {
    extern void* wp;
    extern s32 strcmp(const char* a, const char* b);
    s32 count;
    s32 i;
    void* entry;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if (entryFlag == 2 || entryFlag == flag) {
                if (strcmp((char*)((s32)entry + 0xD4), (char*)name) == 0) {
                    break;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return entry;
}

void imgFreeCapture(void* img, s32 index) {
    extern void smartFree(void* ptr);
    s32 part = (s32)img + index * 0x44;
    u32 flags = *(u32*)part;

    if ((flags & 4) != 0) {
        *(u32*)part = flags & ~0x10;
        if ((*(u32*)part & 0x100) != 0) {
            *(u32*)part = 0;
        } else if (*(s32*)(part + 0x40) == 0) {
            *(u32*)part = 0;
        } else {
            *(u32*)part = 0;
            smartFree(*(void**)(part + 0x3C));
        }
    }
}
