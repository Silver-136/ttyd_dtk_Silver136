#include "driver/animdrv.h"

extern s32 wp;
f32 __fabsf(f32 value);

f32 animPoseGetLoopTimes(s32 poseId) {
    return *(f32*)(*(s32*)((s32)wp + 0x10) + poseId * 0x170 + 0x84);
}

void* animGetPtr(void) {
    return (void*)wp;
}

void* animPoseGetAnimPosePtr(s32 poseId) {
    return (void*)(*(s32*)(wp + 0x10) + poseId * 0x170);
}

s32 animPoseGetMaterialEvtColor(s32 poseId) {
    return *(s32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xF0);
}

s32 animPoseGetMaterialFlag(s32 poseId) {
    return *(s32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xE8);
}

s32 animPoseGetMaterialLightFlag(s32 poseId) {
    return *(s32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xEC);
}

s32 animPoseGetVivianType(s32 poseId) {
    return *(s32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0x11C);
}

void animPoseSetLocalTimeRate(s32 poseId, f32 rate) {
    *(f32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0x44) = rate;
}

void animPosePeraOff(s32 poseId) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170) &= ~8;
}

void animPosePeraOn(s32 poseId) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170) |= 8;
}

void animPoseSetDispCallBack(s32 poseId, void* callback, void* user) {
    void* pose = (void*)(*(s32*)(wp + 0x10) + poseId * 0x170);
    *(void**)((s32)pose + 0x154) = callback;
    *(void**)((s32)pose + 0x158) = user;
}

void animPoseSetGXFunc(s32 poseId, void* callback, void* user) {
    void* pose = (void*)(*(s32*)(wp + 0x10) + poseId * 0x170);
    *(void**)((s32)pose + 0xE0) = callback;
    *(void**)((s32)pose + 0xE4) = user;
}

void animPoseSetMaterialFlagOff(s32 poseId, u32 flag) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xE8) &= ~flag;
}

void animPoseSetMaterialFlagOn(s32 poseId, u32 flag) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xE8) |= flag;
}

void animPoseSetMaterialLightFlagOff(s32 poseId, u32 flag) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xEC) &= ~flag;
}

void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flag) {
    *(u32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xEC) |= flag;
}

void animPoseBattleInit(void) {
    extern void animPoseRefresh(void);
    *(s32*)(wp + 0x10C) = 1;
    animPoseRefresh();
}

void animPoseVivianMain(s32 poseId, f32* values) {
    s32 pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    *(u32*)pose |= 0x20;
    *(f32*)(pose + 0x130) = values[0];
    *(f32*)(pose + 0x140) = values[1];
    *(f32*)(pose + 0x150) = values[2];
}

void initTestHeap(void) {
    extern void* __memAlloc(s32, u32);
    if (*(u32*)(wp + 0x104) == 0) {
        *(void**)(wp + 0x104) = __memAlloc(0, 0x180000);
    }
    *(s32*)(wp + 0x108) = *(s32*)(wp + 0x104);
}

void L_animPoseWorldPositionEvalOff(s32 poseId) {
    extern void smartFree(void*);
    s32 pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    u32 flags = *(u32*)pose;
    if (flags & 0x40) {
        *(u32*)pose = flags & ~0x40;
        smartFree(*(void**)(pose + 0x15C));
        *(s32*)(pose + 0x15C) = 0;
    }
}


u8 animPoseMain(s32 poseIdx) {
    return 0;
}


void renderProc(int shapeIdx, int animFrame0) {
    ;
}


u8 animPaperPoseDispSub(s32 cameraId, void* pose) {
    return 0;
}


void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, double a, double b) {
    ;
}


void animPoseDisp_MakeExtTexture(char* name, void* anim, void* texObj, void** outAlloc,
                                 s32* outLeft, s32* outTop, s32* outRight, s32* outBottom,
                                 f32 frame, f32 scale) {
    extern s32 wp;
    extern s32 gp;
    extern s32 strcmp(const char*, const char*);
    extern void* DEMOGetRenderModeObj(void);
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void PSMTXTrans(void* mtx, double x, double y, double z);
    extern void _animPoseDrawMtx(void* pPose, void* pMtx, int dispMode, double rot, double scale);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern void GXSetTexCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, u32 format, u32 mipmap);
    extern u32 GXGetTexBufferSize(u32 width, u32 height, u32 format, u8 mipmap, u32 max_lod);
    extern void* smartAlloc(u32 size, s32 mode);
    extern void GXCopyTex(void* dest, u8 clear);
    extern void GXPixModeSync(void);
    extern void GXInitTexObj(void* obj, void* data, u16 width, u16 height, u32 format, u32 wrapS, u32 wrapT, u8 mipmap);
    extern f32 float_0_8041fb28;
    extern f32 float_16p667_8041fb40;
    extern f32 float_0p5_8041fb44;
    extern f32 float_0p75_8041fb48;
    extern f32 float_0p25_8041fb4c;
    extern u32 dat_8041fb1c;
    extern u32 dat_8041fb20;
    extern u32 anim_ext_tex_size;

    s32 work;
    s32 poses;
    s32 pose;
    s32 poseIdx;
    s32 offset;
    s32 battleFlag;
    s32 fileIdx;
    s32 file;
    s32 data;
    s32 animCount;
    s32 animIdx;
    s32 nameOffset;
    s32 effectIdx;
    s64 now;
    s64 local;
    u32 ticks;
    s32 backup[0x5C];
    f32 mtx[3][4];
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    u32 flags;
    u32 materialColor;
    u32 materialColor2;
    s32 materialFlag;
    void* renderMode;
    f32 centerX;
    f32 centerY;
    u32 copyWidth32;
    u32 copyHeight32;
    u16 copyWidth;
    u16 copyHeight;
    u32 texSize;
    void* alloc;
    s32 i;

    work = wp;
    poses = *(s32*)(work + 0x10);
    battleFlag = (*(s32*)(gp + 0x14) != 0);

    poseIdx = 0;
    offset = 0;
    while (poseIdx < *(s32*)(work + 0x14)) {
        pose = poses + offset;
        if ((*(u32*)pose & 1) != 0 && *(s32*)(pose + 0xC) == battleFlag) {
            fileIdx = *(s32*)(pose + 0x10);
            file = *(s32*)(*(s32*)work + (fileIdx << 4) + 8);
            data = *(s32*)(file + 0xA0);
            data = *(s32*)data;
            if (strcmp((const char*)(data + 4), name) == 0) {
                break;
            }
        }
        offset += 0x170;
        poseIdx++;
    }

    if (poseIdx == *(s32*)(work + 0x14)) {
        poseIdx = -1;
    }
    if (poseIdx < 0) {
        return;
    }

    pose = poses + poseIdx * 0x170;
    {
        s32* srcWords = (s32*)pose;
        s32* dstWords = backup;
        for (i = 0x2E; i > 0; i--) {
            dstWords[0] = srcWords[0];
            dstWords[1] = srcWords[1];
            srcWords += 2;
            dstWords += 2;
        }
    }

    *(s32*)(pose + 0x90) = -1;

    work = wp;
    poses = *(s32*)(work + 0x10);
    pose = poses + poseIdx * 0x170;
    fileIdx = *(s32*)(pose + 0x10);
    file = *(s32*)(*(s32*)work + (fileIdx << 4) + 8);
    data = *(s32*)(file + 0xA0);
    data = *(s32*)data;
    animCount = *(s32*)(data + 0x148);

    if ((s32)anim < 0) {
        animIdx = 0;
        nameOffset = 0;
        while (animIdx < animCount) {
            if (strcmp((const char*)(*(s32*)(data + 0x1AC) + nameOffset), (const char*)anim) == 0) {
                break;
            }
            nameOffset += 0x40;
            animIdx++;
        }
        if (animIdx >= animCount) {
            animIdx = 0;
        }
    } else {
        animIdx = (s32)anim;
        if (animIdx >= animCount) {
            animIdx = 0;
        }
    }

    if (animIdx != *(s32*)(pose + 0x14)) {
        *(s32*)(pose + 0x14) = animIdx;
        if (*(s32*)(pose + 0x90) != -1 && (*(u32*)pose & 2) != 0) {
            *(u32*)pose |= 4;
        }
        ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        if (*(s32*)(pose + 0xC) == 0) {
            now = *(s64*)(gp + 0x40) / ticks;
        } else {
            now = *(s64*)(gp + 0x38) / ticks;
        }
        *(s64*)(pose + 0x18) = now;
        *(s32*)(pose + 0x3C) = -1;
        *(f32*)(pose + 0x40) = float_0_8041fb28;
        *(s32*)(pose + 0x80) = 0;
        *(f32*)(pose + 0x84) = float_0_8041fb28;
    }

    ticks = (*(u32*)0x800000F8 >> 2) / 1000;
    if (*(s32*)(pose + 0xC) == 0) {
        now = *(s64*)(gp + 0x40) / ticks;
    } else {
        now = *(s64*)(gp + 0x38) / ticks;
    }
    *(s64*)(pose + 0x18) = now;
    *(s32*)(pose + 0x3C) = -1;
    *(f32*)(pose + 0x40) = float_0_8041fb28;
    *(s32*)(pose + 0x80) = 0;
    *(f32*)(pose + 0x84) = float_0_8041fb28;

    local = (u64)(float_16p667_8041fb40 * frame);
    ticks = (*(u32*)0x800000F8 >> 2) / 1000;
    if (*(s32*)(pose + 0xC) == 0) {
        now = *(s64*)(gp + 0x40) / ticks;
    } else {
        now = *(s64*)(gp + 0x38) / ticks;
    }
    now = now - local;
    *(s64*)(pose + 0x18) = now;
    *(s64*)(pose + 0x88) = now;
    effectIdx = *(s32*)(pose + 0x90);
    if (effectIdx != -1) {
        s32 effectPose = *(s32*)(wp + 0x10) + effectIdx * 0x170;
        if ((*(u32*)(effectPose + 4) & 2) == 0) {
            *(s64*)(effectPose + 0x18) = now;
        }
    }

    animPoseMain(poseIdx);

    renderMode = DEMOGetRenderModeObj();
    centerX = float_0p5_8041fb44 * (f32)*(u16*)((s32)renderMode + 4);
    renderMode = DEMOGetRenderModeObj();
    centerY = float_0p75_8041fb48 * (f32)*(u16*)((s32)renderMode + 6);

    sysWaitDrawSync();
    GXClearBoundingBox();

    renderMode = DEMOGetRenderModeObj();
    PSMTXTrans(mtx, (double)float_0_8041fb28,
               (double)(float_0p25_8041fb4c * -(f32)*(u16*)((s32)renderMode + 6)),
               (double)float_0_8041fb28);

    flags = *(u32*)pose;
    materialColor = *(u32*)(pose + 0xF0);
    materialColor2 = *(u32*)(pose + 0xF4);
    materialFlag = *(s32*)(pose + 0xE8);
    *(u32*)pose = flags & ~8;
    *(s32*)(pose + 0xE8) = 0;
    *(u32*)(pose + 0xF0) = dat_8041fb1c;
    *(u32*)(pose + 0xF4) = dat_8041fb20;

    _animPoseDrawMtx((void*)pose, mtx, 1, (double)float_0_8041fb28, (double)scale);
    _animPoseDrawMtx((void*)pose, mtx, 2, (double)float_0_8041fb28, (double)scale);
    _animPoseDrawMtx((void*)pose, mtx, 3, (double)float_0_8041fb28, (double)scale);

    *(u32*)(pose + 0xF0) = materialColor;
    *(u32*)(pose + 0xF4) = materialColor2;
    *(u32*)pose = flags;
    *(s32*)(pose + 0xE8) = materialFlag;

    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);

    if (right < left) {
        left = 0;
        top = 0;
        right = 0x10;
        bottom = 0x10;
    }
    if (left < 2) {
        left = 2;
    }
    if (top < 2) {
        top = 2;
    }

    renderMode = DEMOGetRenderModeObj();
    if ((s32)(*(u16*)((s32)renderMode + 4) - 2) < (s32)(u32)right) {
        renderMode = DEMOGetRenderModeObj();
        right = *(u16*)((s32)renderMode + 4) - 2;
    }
    renderMode = DEMOGetRenderModeObj();
    if ((s32)(*(u16*)((s32)renderMode + 6) - 2) < (s32)(u32)top) {
        renderMode = DEMOGetRenderModeObj();
        top = *(u16*)((s32)renderMode + 6) - 2;
    }

    left = left - (left & 1);
    right = right + (right & 1);
    top = top - (top & 1);
    bottom = bottom + (bottom & 1);

    copyWidth32 = ((u32)right & 0xFFFF) - (u32)left;
    copyHeight32 = ((u32)bottom & 0xFFFF) - (u32)top;
    copyWidth = (u16)copyWidth32;
    copyHeight = (u16)copyHeight32;

    GXSetTexCopySrc((u32)left, (u32)top, copyWidth32 & 0xFFFF, copyHeight32 & 0xFFFF);
    GXSetTexCopyDst(copyWidth, copyHeight, 5, 0);
    texSize = GXGetTexBufferSize(copyWidth32 & 0xFFFF, copyHeight32 & 0xFFFF, 5, 0, 0);
    anim_ext_tex_size += texSize;
    texSize = GXGetTexBufferSize(copyWidth32 & 0xFFFF, copyHeight32 & 0xFFFF, 5, 0, 0);
    alloc = smartAlloc(texSize, 1);
    *outAlloc = alloc;
    GXCopyTex(*(void**)alloc, 1);
    GXPixModeSync();
    GXInitTexObj(texObj, *(void**)*outAlloc, copyWidth, copyHeight, 5, 0, 0, 0);

    *outLeft = (u32)left - (s32)centerX;
    *outTop = (u32)top - (s32)centerY;
    *outRight = (u32)right - (s32)centerX;
    *outBottom = (u32)bottom - (s32)centerY;

    {
        s32* srcWords = backup;
        s32* dstWords = (s32*)pose;
        for (i = 0x2E; i > 0; i--) {
            dstWords[0] = srcWords[0];
            dstWords[1] = srcWords[1];
            srcWords += 2;
            dstWords += 2;
        }
    }
}

u8 materialProc(int shapeIdx) {
    return 0;
}


s32 animPoseEntry(void* name, s32 mode) {
    return 0;
}


void dispProc(int parentIdx, int groupIdx, int animFrame0, int animFrame1, float blend) {
    ;
}


int testAlloc(u32 param_1) {
    return 0;
}


void animPoseDrawShape(int poseIdx, int param_2) {
    s32 work;
    s32 pose;
    s32 animFiles;
    s32 fileIdx;
    s32 animIdx;
    s32 animFile;
    s32 poseData;
    s32 animTable;
    s32 oldTexObj0;
    s32 oldTexObj1;
    s32 oldCurTexObj;
    s32 oldTexData0;
    s32 oldTexData1;
    s32 oldCurTexData;
    s32 oldPaperMtx1;
    s32 oldPaperMtx2;
    s32 oldPaperMtx3;
    s32 oldCurPose;
    s32 oldCurAnimData;
    s32 oldCurPoseData;
    s32 oldD8;
    s32 oldDC;
    s32 zero;
    u32 curPoseIdx;
    s32 posNum;
    s32 nrmNum;
    s32 total;

    work = wp;
    pose = *(s32*)(work + 0x10) + poseIdx * 0x170;
    oldCurPose = *(s32*)(work + 0x18);
    oldCurAnimData = *(s32*)(work + 0x1C);
    oldCurPoseData = *(s32*)(work + 0x20);
    oldTexObj0 = *(s32*)(work + 0x30);
    oldTexObj1 = *(s32*)(work + 0x34);
    oldCurTexObj = *(s32*)(work + 0x38);
    oldTexData0 = *(s32*)(work + 0x24);
    oldTexData1 = *(s32*)(work + 0x28);
    oldCurTexData = *(s32*)(work + 0x2C);
    oldPaperMtx1 = *(s32*)(work + 0xCC);
    oldPaperMtx2 = *(s32*)(work + 0xD0);
    oldPaperMtx3 = *(s32*)(work + 0xD4);
    oldD8 = *(s32*)(work + 0xD8);
    oldDC = *(s32*)(work + 0xDC);
    zero = 0;

    *(s32*)(work + 0x18) = pose;
    animFiles = *(s32*)wp;
    fileIdx = *(s32*)(pose + 0x10);
    animIdx = *(s32*)(pose + 0x14);
    animFile = animFiles + (fileIdx << 4);
    animFile = *(s32*)(animFile + 8);
    poseData = *(s32*)(animFile + 0xA0);
    poseData = *(s32*)poseData;
    animTable = *(s32*)(poseData + 0x1AC);
    animTable += animIdx << 6;
    *(s32*)(wp + 0x1C) = *(s32*)(animTable + 0x3C);
    animFiles = *(s32*)wp;
    fileIdx = *(s32*)(pose + 0x10);
    animFile = animFiles + (fileIdx << 4);
    animFile = *(s32*)(animFile + 8);
    poseData = *(s32*)(animFile + 0xA0);
    poseData = *(s32*)poseData;
    *(s32*)(wp + 0x20) = poseData;
    *(s32*)(wp + 0x30) = zero;
    *(s32*)(wp + 0x34) = zero;
    *(s32*)(wp + 0x38) = zero;
    *(s32*)(wp + 0x24) = zero;
    *(s32*)(wp + 0x28) = zero;
    *(s32*)(wp + 0x2C) = zero;
    *(s32*)(wp + 0xCC) = zero;
    *(s32*)(wp + 0xD0) = zero;
    *(s32*)(wp + 0xD4) = zero;
    *(s32*)(wp + 0xD8) = zero;
    *(s32*)(wp + 0xDC) = zero;

    if (*(s32*)(pose + 0x3C) == -1) {
        work = wp;
        curPoseIdx = (u32)(pose - *(s32*)(work + 0x10)) / 0x170;
        if ((*(u32*)(pose + 4) & 2) != 0) {
            fileIdx = *(s32*)(pose + 0x10);
            animFiles = *(s32*)work;
            animFile = animFiles + (fileIdx << 4);
            animFile = *(s32*)(animFile + 8);
            poseData = *(s32*)(animFile + 0xA0);
            poseData = *(s32*)poseData;
            posNum = *(s32*)(poseData + 0xF0);
            nrmNum = *(s32*)(poseData + 0xF8);
            total = posNum + nrmNum;
            if (total <= *(s32*)(work + 0xF0)) {
                if (*(s32*)(work + 0xF0) < *(s32*)(work + 0xF4) + total) {
                    *(s32*)(work + 0xF4) = zero;
                }
                *(s32*)(wp + 0xE8) = 1;
                *(s32*)(wp + 0xF8) = *(s32*)(wp + 0xEC) + *(s32*)(wp + 0xF4) * 0xC;
                *(s32*)(wp + 0xF4) = *(s32*)(wp + 0xF4) + posNum;
                *(s32*)(wp + 0xFC) = *(s32*)(wp + 0xEC) + *(s32*)(wp + 0xF4) * 0xC;
                *(s32*)(wp + 0xF4) = *(s32*)(wp + 0xF4) + nrmNum;
            }
            animPoseMain(curPoseIdx);
            *(s32*)(wp + 0xE8) = zero;
        } else {
            animPoseMain(curPoseIdx);
        }
    }

    renderProc(param_2, *(s32*)(pose + 0x28));
    *(s32*)(wp + 0x18) = oldCurPose;
    *(s32*)(wp + 0x1C) = oldCurAnimData;
    *(s32*)(wp + 0x20) = oldCurPoseData;
    *(s32*)(wp + 0x30) = oldTexObj0;
    *(s32*)(wp + 0x34) = oldTexObj1;
    *(s32*)(wp + 0x38) = oldCurTexObj;
    *(s32*)(wp + 0x24) = oldTexData0;
    *(s32*)(wp + 0x28) = oldTexData1;
    *(s32*)(wp + 0x2C) = oldCurTexData;
    *(s32*)(wp + 0xCC) = oldPaperMtx1;
    *(s32*)(wp + 0xD0) = oldPaperMtx2;
    *(s32*)(wp + 0xD4) = oldPaperMtx3;
    *(s32*)(wp + 0xD8) = oldD8;
    *(s32*)(wp + 0xDC) = oldDC;
}

void animPoseSetPaperAnimGroup(s32 poseId, void* name, s32 flag) {
    extern s32 strcmp(const char*, const char*);
    extern s32 gp;
    s32 work;
    s32 parent;
    s32 index;
    s32 heapType;
    s32 step;
    s32 pose;
    s64 time;
    s32 offset;
    void* nameLocal;

    offset = poseId * 0x170;
    nameLocal = name;
    work = wp;
    parent = *(s32*)(work + 0x10) + offset;
    if (nameLocal == 0) {
        *(s32*)(parent + 0x90) = -1;
        work = wp;
        *(u32*)(*(s32*)(work + 0x10) + offset + 0xEC) &= ~1;
    } else {
        index = 0;
        step = index;
        heapType = *(s32*)(parent + 0xC);
        while (index < *(s32*)(wp + 0x14)) {
            work = wp;
            pose = *(s32*)(work + 0x10) + step;
            if ((*(u32*)pose & 1) != 0 && (*(u32*)(pose + 4) & 1) != 0 &&
                (*(u32*)(pose + 4) & 2) != 0 && *(s32*)(pose + 0xC) == heapType) {
                s32 file;
                s32 data;
                file = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
                file = *(s32*)(file + 8);
                data = *(s32*)(file + 0xA0);
                data = *(s32*)data;
                if (strcmp((char*)(data + 4), (char*)nameLocal) == 0) {
                    break;
                }
            }
            step += 0x170;
            index++;
        }
        if (index == *(s32*)(wp + 0x14)) {
            index = -1;
        }
        *(s32*)(parent + 0x90) = index;
        if (*(s32*)(parent + 0x90) < 0) {
            index = 0;
            step = index;
            while (index < *(s32*)(wp + 0x14)) {
                work = wp;
                pose = *(s32*)(work + 0x10) + step;
                if ((*(u32*)pose & 1) != 0 && (*(u32*)(pose + 4) & 1) != 0 &&
                    (*(u32*)(pose + 4) & 2) != 0 && *(s32*)(pose + 0xC) == 2) {
                    s32 file;
                    s32 data;
                    file = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
                    file = *(s32*)(file + 8);
                    data = *(s32*)(file + 0xA0);
                    data = *(s32*)data;
                    if (strcmp((char*)(data + 4), (char*)nameLocal) == 0) {
                        break;
                    }
                }
                step += 0x170;
                index++;
            }
            if (index == *(s32*)(wp + 0x14)) {
                index = -1;
            }
            *(s32*)(parent + 0x90) = index;
        }
        *(s32*)(parent + 0x94) = -1;
        if (*(s32*)(parent + 0xC) == 0) {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x40) / ticks;
        } else {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x38) / ticks;
        }
        *(s64*)(parent + 0x88) = time;
        if (flag != 0) {
            *(u32*)parent |= 2;
            *(u32*)parent |= 4;
        } else {
            *(u32*)parent &= ~2;
            *(u32*)parent &= ~4;
        }
        work = wp;
        *(u32*)(*(s32*)(work + 0x10) + offset + 0xEC) |= 1;
    }
}

void animPoseSetEffect(s32 poseId, void* name, s32 flag) {
    extern s32 strcmp(const char*, const char*);
    extern void fileFree(void* file);
    extern s32 gp;
    s32 work;
    s32 parent;
    s32 effectIdx;
    s32 effectPose;
    s32 animFile;
    s32 texFile;
    s32 ref;
    s32 newPoseId;
    s32 newPose;
    s64 time;
    s32 offset;

    offset = poseId * 0x170;
    work = wp;
    parent = *(s32*)(work + 0x10) + offset;
    if (*(s32*)(parent + 0x90) != -1) {
        if (name != 0) {
            s32 data;
            s32 file;
            effectIdx = *(s32*)(parent + 0x90);
            work = wp;
            file = *(s32*)work + (*(s32*)(*(s32*)(work + 0x10) + effectIdx * 0x170 + 0x10) << 4);
            file = *(s32*)(file + 8);
            data = *(s32*)(file + 0xA0);
            data = *(s32*)data;
            if (strcmp((char*)(data + 4), (char*)name) == 0 && ((*(u32*)parent >> 1) & 1) == flag) {
                return;
            }
        }

        work = wp;
        effectPose = *(s32*)(work + 0x10) + *(s32*)(parent + 0x90) * 0x170;
        ref = *(s32*)(effectPose + 8) - 1;
        *(s32*)(effectPose + 8) = ref;
        if (*(s32*)(effectPose + 8) < 0) {
            work = wp;
            animFile = *(s32*)work + (*(s32*)(effectPose + 0x10) << 4);
            ref = *(s32*)(animFile + 4) - 1;
            *(s32*)(animFile + 4) = ref;
            if (*(s32*)(animFile + 4) <= 0) {
                *(s32*)animFile = 0;
                work = wp;
                texFile = *(s32*)(work + 8) + *(s32*)(animFile + 0xC) * 0xC;
                ref = *(s32*)(texFile + 4) - 1;
                *(s32*)(texFile + 4) = ref;
                if (*(s32*)(texFile + 4) <= 0) {
                    fileFree(*(void**)(texFile + 8));
                    *(s32*)texFile = 0;
                }
                fileFree(*(void**)(animFile + 8));
            }
            *(s32*)effectPose = 0;
        }
    }

    if (name == 0) {
        *(s32*)(parent + 0x90) = -1;
        work = wp;
        *(u32*)(*(s32*)(work + 0x10) + offset + 0xEC) &= ~1;
    } else {
        newPoseId = animPoseEntry(name, *(s32*)(parent + 0xC));
        if (newPoseId != -2) {
            work = wp;
            newPose = *(s32*)(work + 0x10) + newPoseId * 0x170;
            *(s32*)(newPose + 4) = 1;
            *(s32*)(newPose + 8) = 0;
            *(u32*)newPose |= 1;
        }
        *(s32*)(parent + 0x90) = newPoseId;
        *(s32*)(parent + 0x94) = -1;
        if (*(s32*)(parent + 0xC) == 0) {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x40) / ticks;
        } else {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x38) / ticks;
        }
        *(s64*)(parent + 0x88) = time;
        if (flag != 0) {
            *(u32*)parent |= 2;
            *(u32*)parent |= 4;
        } else {
            *(u32*)parent &= ~2;
        }
        work = wp;
        *(u32*)(*(s32*)(work + 0x10) + offset + 0xEC) |= 1;
    }
}

void animInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern f64 tan(f64 x);
    extern void* fileAllocf(s32 size, const char* fmt, ...);
    extern void memcpy_as4(void* dst, void* src, u32 size);
    extern void fileFree(void* file);
    extern void fileSetCurrentArchiveType(s32 type);
    extern const f32 float_deg2rad_8041fbb4;
    extern const char str_a_vivian_bin_802c11d8[];
    extern const char str_a_ag2tg_bin_802c11e8[];
    extern f32 tanfTbl[];
    extern s32 vivihimoData;
    extern s32 vivihimoTailGroupNo;
    s32 work;
    register f32* table;
    register s32 i;
    void* file;

    work = wp;
    *(s32*)(work + 4) = 0x40;
    *(s32*)(wp + 0xC) = 0x40;
    *(s32*)(wp + 0x14) = 0x100;
    *(s32*)(wp + 0xF0) = 0x400;
    *(s32*)(wp + 0xF4) = 0;

    *(void**)wp = __memAlloc(0, *(s32*)(wp + 4) << 4);
    *(void**)(wp + 8) = __memAlloc(0, *(s32*)(wp + 0xC) * 0xC);
    *(void**)(wp + 0x10) = __memAlloc(0, *(s32*)(wp + 0x14) * 0x170);
    *(s32*)(wp + 0xE8) = 0;
    *(void**)(wp + 0xEC) = __memAlloc(0, *(s32*)(wp + 0xF0) * 0xC);

    *(s32*)(wp + 0x30) = 0;
    *(s32*)(wp + 0x34) = 0;
    *(s32*)(wp + 0x38) = 0;
    *(s32*)(wp + 0x24) = 0;
    *(s32*)(wp + 0x28) = 0;
    *(s32*)(wp + 0x2C) = 0;
    *(s32*)(wp + 0xCC) = 0;
    *(s32*)(wp + 0xD0) = 0;
    *(s32*)(wp + 0xD4) = 0;
    *(s32*)(wp + 0xD8) = 1;
    *(s32*)(wp + 0xDC) = 1;
    *(s32*)(wp + 0x104) = 0;
    *(s32*)(wp + 0x10C) = 0;

    if (*(void**)(wp + 0x104) == 0) {
        *(void**)(wp + 0x104) = __memAlloc(0, 0x180000);
    }

    table = tanfTbl;
    i = 0;
    *(s32*)(wp + 0x108) = *(s32*)(wp + 0x104);
    do {
        *table = (f32)tan((f64)(float_deg2rad_8041fbb4 * (f32)(i - 0x59)));
        i++;
        table++;
    } while (i < 0xB2);

    file = fileAllocf(0, str_a_vivian_bin_802c11d8);
    if (file != 0) {
        memcpy_as4((void*)*((s32*)&vivihimoData + 1), **(void***)((s32)file + 0xA0), 0xC00);
        memcpy_as4(&vivihimoTailGroupNo, (void*)((s32)**(void***)((s32)file + 0xA0) + 0xC00), 0x10);
        fileFree(file);
    }
    fileSetCurrentArchiveType(0);
    *(void**)(wp + 0x100) = fileAllocf(0, str_a_ag2tg_bin_802c11e8);
}


void animPoseRefresh(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 testAlloc(u32 size);
    s32 work;
    s32 i;
    s32 offset;
    s32 pose;
    s32 file;
    s32 data;

    work = wp;
    if (*(s32*)(work + 0x104) == 0) {
        *(void**)(wp + 0x104) = __memAlloc(0, 0x180000);
    }
    *(s32*)(wp + 0x108) = *(s32*)(wp + 0x104);

    i = 0;
    offset = 0;
    while (i < *(s32*)(wp + 0x14)) {
        work = wp;
        pose = *(s32*)(work + 0x10) + offset;
        if (*(s32*)pose != 0 && (*(s32*)(wp + 0x10C) == 0 || *(s32*)(pose + 0xC) != 0) &&
            (*(s32*)(wp + 0x10C) != 0 || *(s32*)(pose + 0xC) != 1)) {
            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x48) = testAlloc(*(s32*)(data + 0xF0) * 0xC);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x4C) = testAlloc(*(s32*)(data + 0xF0) * 0xC);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x50) = testAlloc(*(s32*)(data + 0xF8) * 0xC);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x54) = testAlloc(*(s32*)(data + 0xF8) * 0xC);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x58) = testAlloc(*(s32*)(data + 0x13C));
            *(s32*)(pose + 0x5C) = *(s32*)(pose + 0x58);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x60) = testAlloc(*(s32*)(data + 0x140) << 2);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x64) = testAlloc(*(s32*)(data + 0x140) << 2);

            file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
            data = *(s32*)(file + 8);
            data = *(s32*)(data + 0xA0);
            data = *(s32*)data;
            *(s32*)(pose + 0x68) = testAlloc(*(s32*)(data + 0x12C) * 0x18);
            *(s32*)(pose + 0x6C) = *(s32*)(pose + 0x68);
            *(s32*)(pose + 0x3C) = -1;
            *(s32*)(pose + 0x3C) = -1;
        }
        offset += 0x170;
        i++;
    }
}

void evalProc(int parentGroupIdx, int groupIdx) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void PSMTXCopy(void* src, void* dst);
    extern void pushGXModelMtx_JointNode__(void* pJoint, double sx, double sy, double sz);
    extern u8 pushGXModelMtx_TransformNode__(void* pNode);
    extern void* g_modeling_mtx;
    extern s32 g_modeling_mtx_lv;
    extern u8 g_modeling_mtx_stack[];
    extern const Vec3 vec3_802c1054;
    s32 work;
    s32 pose;
    s32 poseData;
    s32 groups;
    s32 group;
    s32 nodes;
    s32 node;
    s32 parentGroup;
    s32 parentNode;
    s32 flags;
    Vec3 temp;
    f32 sx;
    f32 sy;
    f32 sz;
    s32 mtxTop;

    if (groupIdx != -1) {
        work = wp;
        poseData = *(s32*)(work + 0x20);
        pose = *(s32*)(work + 0x18);
        groups = *(s32*)(poseData + 0x1A8);
        group = groups + groupIdx * 0x58;
        if (*(s8*)(*(s32*)(pose + 0x5C) + *(s32*)(group + 0x4C)) != 0) {
            nodes = *(s32*)(pose + 0x64);
            node = nodes + (*(s32*)(group + 0x50) << 2);
            if (*(s32*)(group + 0x54) != 0) {
                if (parentGroupIdx == -1) {
                    sz = 1.0f;
                    sy = sz;
                    sx = sz;
                } else {
                    parentGroup = groups + parentGroupIdx * 0x58;
                    if (*(s32*)(parentGroup + 0x54) == 0) {
                        sz = 1.0f;
                        sy = sz;
                        sx = sz;
                    } else {
                        parentNode = nodes + (*(s32*)(parentGroup + 0x50) << 2);
                        sx = *(f32*)(parentNode + 0xC);
                        sy = *(f32*)(parentNode + 0x10);
                        sz = *(f32*)(parentNode + 0x14);
                    }
                }
                pushGXModelMtx_JointNode__((void*)node, sx, sy, sz);
            } else {
                pushGXModelMtx_TransformNode__((void*)node);
            }
            work = wp;
            pose = *(s32*)(work + 0x18);
            flags = *(s32*)pose;
            if ((flags & 0x40) != 0) {
                temp = vec3_802c1054;
                temp.x = *(f32*)((s32)g_modeling_mtx + 0xC);
                temp.y = *(f32*)((s32)g_modeling_mtx + 0x1C);
                temp.z = *(f32*)((s32)g_modeling_mtx + 0x2C);
                *(Vec3*)(*(s32*)(*(s32*)(pose + 0x15C)) + groupIdx * 0xC) = temp;
            }
            work = wp;
            pose = *(s32*)(work + 0x18);
            flags = *(s32*)pose;
            if ((flags & 0x80) != 0) {
                PSMTXCopy(g_modeling_mtx, (void*)(*(s32*)(*(s32*)(pose + 0x164)) + groupIdx * 0x30));
            }
            work = wp;
            pose = *(s32*)(work + 0x18);
            flags = *(s32*)pose;
            if ((flags & 0x20) == 0 || groupIdx != **(s32**)(pose + 0x120)) {
                evalProc(groupIdx, *(s32*)(group + 0x44));
            }
            mtxTop = g_modeling_mtx_lv - 1;
            g_modeling_mtx_lv = mtxTop;
            g_modeling_mtx = g_modeling_mtx_stack + mtxTop * 0x30;
        }
        evalProc(groupIdx, *(s32*)(group + 0x40));
    }
}

void animPoseAutoRelease(s32 value) {
    extern void fileFree(void* file);
    extern void smartFree(void* ptr);
    extern void animPoseRefresh(void);
    s32 offset;
    s32 pose;
    s32 i;
    s32 flags;
    s32 work;
    s32 texFile;
    s32 animFile;
    s32 ref;

    for (i = 0, offset = 0; i < *(s32*)(wp + 0x14); offset += 0x170, i++) {
        pose = *(s32*)(wp + 0x10) + offset;
        flags = *(s32*)pose;
        if ((flags & 1) != 0 && *(s32*)(pose + 0xC) == value) {
            if (*(u32*)(pose + 4) != 0) {
                ref = *(s32*)(pose + 8) - 1;
                *(s32*)(pose + 8) = ref;
                if (*(s32*)(pose + 8) < 0) {
                    work = wp;
                    animFile = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
                    ref = *(s32*)(animFile + 4) - 1;
                    *(s32*)(animFile + 4) = ref;
                    if (*(s32*)(animFile + 4) <= 0) {
                        *(s32*)animFile = 0;
                        work = wp;
                        texFile = *(s32*)(work + 8) + *(s32*)(animFile + 0xC) * 0xC;
                        ref = *(s32*)(texFile + 4) - 1;
                        *(s32*)(texFile + 4) = ref;
                        if (*(s32*)(texFile + 4) <= 0) {
                            fileFree(*(void**)(texFile + 8));
                            *(s32*)texFile = 0;
                        }
                        fileFree(*(void**)(animFile + 8));
                    }
                    *(s32*)pose = 0;
                }
            } else {
                if ((flags & 0x40) != 0) {
                    smartFree(*(void**)(pose + 0x15C));
                }
                if ((*(s32*)pose & 0x80) != 0) {
                    smartFree(*(void**)(pose + 0x164));
                }
                work = wp;
                animFile = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
                ref = *(s32*)(animFile + 4) - 1;
                *(s32*)(animFile + 4) = ref;
                if (*(s32*)(animFile + 4) <= 0) {
                    *(s32*)animFile = 0;
                    work = wp;
                    texFile = *(s32*)(work + 8) + *(s32*)(animFile + 0xC) * 0xC;
                    ref = *(s32*)(texFile + 4) - 1;
                    *(s32*)(texFile + 4) = ref;
                    if (*(s32*)(texFile + 4) <= 0) {
                        fileFree(*(void**)(texFile + 8));
                        *(s32*)texFile = 0;
                    }
                    fileFree(*(void**)(animFile + 8));
                }
                *(s32*)pose = 0;
            }
        }
    }
    if (value == 1) {
        *(s32*)(wp + 0x10C) = 0;
    }
    animPoseRefresh();
}

void animPoseRelease(s32 poseId) {
    extern void fileFree(void* file);
    extern void smartFree(void* ptr);
    s32 pose;
    s32 texFile;
    s32 animFile;
    s32 ref;
    s32 work;

    pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    if (*(u32*)(pose + 4) != 0) {
        ref = *(s32*)(pose + 8) - 1;
        *(s32*)(pose + 8) = ref;
        if (*(s32*)(pose + 8) < 0) {
            work = wp;
            animFile = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
            ref = *(s32*)(animFile + 4) - 1;
            *(s32*)(animFile + 4) = ref;
            if (*(s32*)(animFile + 4) <= 0) {
                *(s32*)animFile = 0;
                work = wp;
                texFile = *(s32*)(work + 8) + *(s32*)(animFile + 0xC) * 0xC;
                ref = *(s32*)(texFile + 4) - 1;
                *(s32*)(texFile + 4) = ref;
                if (*(s32*)(texFile + 4) <= 0) {
                    fileFree(*(void**)(texFile + 8));
                    *(s32*)texFile = 0;
                }
                fileFree(*(void**)(animFile + 8));
            }
            *(s32*)pose = 0;
        }
    } else {
        if ((*(s32*)pose & 0x40) != 0) {
            smartFree(*(void**)(pose + 0x15C));
        }
        if ((*(s32*)pose & 0x80) != 0) {
            smartFree(*(void**)(pose + 0x164));
        }
        work = wp;
        animFile = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
        ref = *(s32*)(animFile + 4) - 1;
        *(s32*)(animFile + 4) = ref;
        if (*(s32*)(animFile + 4) <= 0) {
            *(s32*)animFile = 0;
            work = wp;
            texFile = *(s32*)(work + 8) + *(s32*)(animFile + 0xC) * 0xC;
            ref = *(s32*)(texFile + 4) - 1;
            *(s32*)(texFile + 4) = ref;
            if (*(s32*)(texFile + 4) <= 0) {
                fileFree(*(void**)(texFile + 8));
                *(s32*)texFile = 0;
            }
            fileFree(*(void**)(animFile + 8));
        }
        *(s32*)pose = 0;
    }
}

void animPoseSetEffectAnim(s32 poseId, void* anim, s32 force) {
    extern s32 strcmp(const char*, const char*);
    extern s32 gp;
    extern f32 float_0_8041fb28;
    s32 work;
    s32 poses;
    s32 pose;
    s32 effectPoseIdx;
    s32 file;
    s32 animCount;
    s32 data;
    s32 nameOffset;
    s32 animIdx;
    s64 time;
    register s32 offset = poseId * 0x170;

    work = wp;
    poses = *(s32*)(work + 0x10);
    pose = poses + offset;
    effectPoseIdx = *(s32*)(pose + 0x90);
    if (effectPoseIdx != -1) {
        animIdx = (s32)anim;
        pose = poses + effectPoseIdx * 0x170;
        file = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
        file = *(s32*)(file + 8);
        data = *(s32*)(file + 0xA0);
        data = *(s32*)data;
        animCount = *(s32*)(data + 0x148);

        if (animIdx < 0) {
            animIdx = 0;
            nameOffset = animIdx;
            while (animIdx < animCount) {
                if (strcmp((char*)(*(s32*)(data + 0x1AC) + nameOffset), (char*)anim) == 0) {
                    break;
                }
                nameOffset += 0x40;
                animIdx++;
            }
        } else if (animIdx >= animCount) {
            animIdx = 0;
        }

        if (force != 0 || animIdx != *(s32*)(pose + 0x14)) {
            *(s32*)(pose + 0x14) = animIdx;
            if (*(s32*)(pose + 0x90) != -1 && (*(u32*)pose & 2) != 0) {
                *(u32*)pose |= 4;
            }
            if (*(s32*)(pose + 0xC) == 0) {
                u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
                time = *(s64*)(gp + 0x40) / ticks;
            } else {
                u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
                time = *(s64*)(gp + 0x38) / ticks;
            }
            *(s64*)(pose + 0x18) = time;
            *(s32*)(pose + 0x3C) = -1;
            *(f32*)(pose + 0x40) = float_0_8041fb28;
            *(s32*)(pose + 0x80) = 0;
            *(f32*)(pose + 0x84) = float_0_8041fb28;
        }
    }
}

void animPoseSetAnim(s32 poseId, void* name, s32 force) {
    extern s32 strcmp(const char*, const char*);
    extern s32 wp;
    extern s32 gp;
    extern f32 float_0_8041fb28;
    s32 pose;
    s32 file;
    s32 data;
    s32 animCount;
    s32 animIdx;
    s32 offset;
    s64 time;

    pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
    data = **(s32**)(*(s32*)(file + 8) + 0xA0);
    animCount = *(s32*)(data + 0x148);
    animIdx = (s32)name;
    if (animIdx < 0) {
        animIdx = 0;
        offset = 0;
        while (animIdx < animCount) {
            if (strcmp((char*)(*(s32*)(data + 0x1AC) + offset), name) == 0) {
                break;
            }
            offset += 0x40;
            animIdx++;
        }
    } else if (animIdx >= animCount) {
        animIdx = 0;
    }

    if (force != 0 || animIdx != *(s32*)(pose + 0x14)) {
        *(s32*)(pose + 0x14) = animIdx;
        if (*(s32*)(pose + 0x90) != -1 && (*(u32*)pose & 2) != 0) {
            *(u32*)pose |= 4;
        }
        if (*(s32*)(pose + 0xC) == 0) {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x40) / ticks;
        } else {
            u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
            time = *(s64*)(gp + 0x38) / ticks;
        }
        *(s64*)(pose + 0x18) = time;
        *(s32*)(pose + 0x3C) = -1;
        *(f32*)(pose + 0x40) = float_0_8041fb28;
        *(s32*)(pose + 0x80) = 0;
        *(f32*)(pose + 0x84) = float_0_8041fb28;
    }
}

void animPoseSetPaperAnim(s32 poseId, void* name, s32 enabled) {
    extern s32 strcmp(const char*, const char*);
    extern s32 wp;
    extern s32 gp;
    extern u8 animPoseMain(s32 poseIdx);
    s32 pose;
    s32 paper;
    s32 file;
    s32 data;
    s32 animCount;
    s32 animIdx;
    s32 offset;
    s64 time;

    pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    paper = *(s32*)(wp + 0x10) + *(s32*)(pose + 0x90) * 0x170;
    file = *(s32*)wp + (*(s32*)(paper + 0x10) << 4);
    data = **(s32**)(*(s32*)(file + 8) + 0xA0);
    animCount = *(s32*)(data + 0x148);
    animIdx = (s32)name;
    if (animIdx < 0) {
        animIdx = 0;
        offset = 0;
        while (animIdx < animCount) {
            if (strcmp((char*)(*(s32*)(data + 0x1AC) + offset), name) == 0) {
                break;
            }
            offset += 0x40;
            animIdx++;
        }
    } else if (animIdx >= animCount) {
        animIdx = 0;
    }

    *(s32*)(pose + 0x94) = animIdx;
    if (*(s32*)(pose + 0xC) == 0) {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        time = *(s64*)(gp + 0x40) / ticks;
    } else {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        time = *(s64*)(gp + 0x38) / ticks;
    }
    *(s64*)(pose + 0x88) = time;
    if (*(s32*)(pose + 0x90) != -1 && (*(u32*)(paper + 4) & 2) == 0) {
        animPoseMain(*(s32*)(pose + 0x90));
    }
}

void animPaperPoseEntry(s32 name, s32 flag) {
    ;
}


s32 animEffectAsync(void* name, s32 mode) {
    extern void fileSetCurrentArchiveType(s32 type);
    extern s32 fileAsyncf(s32 type, s32 flags, char* fmt, char* ext, ...);
    extern s32 strcmp(const char*, const char*);
    extern s32 wp;
    extern char str_PCTs_PCTs_8041fb34[];
    extern char str_PCTs_PCTs_802c1068[];
    extern char str_a_8041fb3c[];
    extern char zero_8041fb30[];
    s32 first;
    s32 second;
    s32 count;
    char* entry;
    char* found;
    s32 offset;

    switch (mode) {
        case 0:
            fileSetCurrentArchiveType(1);
            break;
        case 1:
            fileSetCurrentArchiveType(2);
            break;
        case 2:
            fileSetCurrentArchiveType(0);
            break;
    }

    first = fileAsyncf(5, 0, str_PCTs_PCTs_8041fb34, str_a_8041fb3c, name);
    entry = *(char**)(*(s32*)(*(s32*)(wp + 0x100) + 0xA0));
    count = *(s32*)(*(s32*)(*(s32*)(wp + 0x100) + 0xA0) + 4);
    offset = 0;
    while (offset < count) {
        if (strcmp(entry, name) == 0) {
            found = entry + 0x20;
            goto found_name;
        }
        entry += 0x20;
        offset += 0x20;
    }
    found = zero_8041fb30;
found_name:
    second = fileAsyncf(4, 0, str_PCTs_PCTs_802c1068, str_a_8041fb3c, found);
    if (first == -1) {
        return 1;
    }
    if (first == 0) {
        return 0;
    }
    if (second == -1) {
        return 1;
    }
    if (second == 0) {
        return 0;
    }
    return 1;
}

s32 animGroupBaseAsync(void* name, s32 mode, s32 flags) {
    extern void fileSetCurrentArchiveType(s32 type);
    extern s32 fileAsyncf(s32 type, s32 flags, char* fmt, char* ext, ...);
    extern s32 strcmp(const char*, const char*);
    extern s32 wp;
    extern char str_PCTs_PCTs_8041fb34[];
    extern char str_PCTs_PCTs_802c1068[];
    extern char str_a_8041fb3c[];
    extern char zero_8041fb30[];
    s32 first;
    s32 second;
    s32 count;
    char* entry;
    char* found;
    s32 offset;

    switch (mode) {
        case 0:
            fileSetCurrentArchiveType(1);
            break;
        case 1:
            fileSetCurrentArchiveType(2);
            break;
        case 2:
            fileSetCurrentArchiveType(0);
            break;
    }

    first = fileAsyncf(5, flags, str_PCTs_PCTs_8041fb34, str_a_8041fb3c, name);
    entry = *(char**)(*(s32*)(*(s32*)(wp + 0x100) + 0xA0));
    count = *(s32*)(*(s32*)(*(s32*)(wp + 0x100) + 0xA0) + 4);
    offset = 0;
    while (offset < count) {
        if (strcmp(entry, name) == 0) {
            found = entry + 0x20;
            goto found_name;
        }
        entry += 0x20;
        offset += 0x20;
    }
    found = zero_8041fb30;
found_name:
    second = fileAsyncf(4, 0, str_PCTs_PCTs_802c1068, str_a_8041fb3c, found);
    if (first == -1) {
        return 1;
    }
    if (first == 0) {
        return 0;
    }
    if (second == -1) {
        return 1;
    }
    return second != 0;
}

void animPoseSetLocalTime(s32 poseId, f32 frame) {
    extern s32 wp;
    extern s32 gp;
    extern f32 float_16p667_8041fb40;
    s32 pose;
    s64 local;
    s64 time;
    s64 result;
    s32 paper;

    pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    local = (u64)(float_16p667_8041fb40 * frame);
    if (*(s32*)(pose + 0xC) == 0) {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        time = *(s64*)(gp + 0x40) / ticks;
    } else {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        time = *(s64*)(gp + 0x38) / ticks;
    }
    result = time - local;
    *(s64*)(pose + 0x18) = result;
    *(s64*)(pose + 0x88) = result;
    if (*(s32*)(pose + 0x90) != -1) {
        paper = *(s32*)(wp + 0x10) + *(s32*)(pose + 0x90) * 0x170;
        if ((*(u32*)(paper + 4) & 2) == 0) {
            *(s64*)(paper + 0x18) = result;
        }
    }
}

s32 animPaperPoseGetId(s32 name, s32 flag) {
    extern s32 wp;
    extern s32 strcmp(const char*, const char*);
    s32 i;
    s32 pose;
    s32 step;

    i = 0;
    step = 0;
    while (i < *(s32*)(wp + 0x14)) {
        pose = *(s32*)(wp + 0x10) + step;
        if ((*(u32*)pose & 1) && (*(u32*)(pose + 4) & 1) && (*(u32*)(pose + 4) & 2) &&
            *(s32*)(pose + 0xC) == flag) {
            s32 data = *(s32*)wp;
            s32 idx = *(s32*)(pose + 0x10);
            char* poseName = (char*)(**(s32**)(*(s32*)(data + (idx << 4) + 8) + 0xA0) + 4);
            if (strcmp(poseName, (char*)name) == 0) {
                break;
            }
        }
        step += 0x170;
        i++;
    }
    if (i == *(s32*)(wp + 0x14)) {
        i = -1;
    }
    return i;
}

void animPaperPoseRelease(s32 poseId) {
    extern s32 wp;
    extern void fileFree(void*);
    s32 pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    s32 file;
    s32 group;

    *(s32*)(pose + 8) = *(s32*)(pose + 8) - 1;
    if (*(s32*)(pose + 8) < 0) {
        group = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
        *(s32*)(group + 4) = *(s32*)(group + 4) - 1;
        if (*(s32*)(group + 4) <= 0) {
            *(s32*)group = 0;
            file = *(s32*)(wp + 8) + *(s32*)(group + 0xC) * 0xC;
            *(s32*)(file + 4) = *(s32*)(file + 4) - 1;
            if (*(s32*)(file + 4) <= 0) {
                fileFree(*(void**)(file + 8));
                *(s32*)file = 0;
            }
            fileFree(*(void**)(group + 8));
        }
        *(s32*)pose = 0;
    }
}

void animSetPaperTexMtx(void* param_1, void* param_2, void* param_3) {
    extern s32 wp;
    extern void PSMTXCopy(void* src, void* dst);

    if (param_1 == 0) {
        *(s32*)(wp + 0xCC) = 0;
    } else {
        PSMTXCopy(param_1, (void*)(wp + 0x3C));
        *(s32*)(wp + 0xCC) = wp + 0x3C;
    }
    if (param_2 == 0) {
        *(s32*)(wp + 0xD0) = 0;
    } else {
        PSMTXCopy(param_2, (void*)(wp + 0x6C));
        *(s32*)(wp + 0xD0) = wp + 0x6C;
    }
    if (param_3 == 0) {
        *(s32*)(wp + 0xD4) = 0;
    } else {
        PSMTXCopy(param_3, (void*)(wp + 0x9C));
        *(s32*)(wp + 0xD4) = wp + 0x9C;
    }
}

void animPoseWorldMatrixEvalOn(int poseIdx, u8 param_2) {
    extern s32 wp;
    extern void* smartAlloc(s32 size, s32 mode);
    s32 offset = poseIdx * 0x170;
    s32 pose;
    s32 data;
    s32 groupEntry;
    s32 group;
    s32 mode;

    pose = *(s32*)(wp + 0x10) + offset;
    data = *(s32*)wp;
    groupEntry = data + (*(s32*)(pose + 0x10) << 4);
    group = **(s32**)(*(s32*)(groupEntry + 8) + 0xA0);
    if ((*(u32*)pose & 0x80) == 0) {
        *(u32*)pose |= 0x80;
        switch (*(s32*)(pose + 0xC)) {
            case 0:
                mode = 1;
                break;
            case 1:
                mode = 2;
                break;
            case 2:
                mode = 0;
                break;
        }
        *(s32*)(pose + 0x168) = *(s32*)(group + 0x144);
        *(void**)(pose + 0x164) = smartAlloc(*(s32*)(group + 0x144) * 0x30, mode);
    }
}

void animPoseWorldPositionEvalOn(int poseIdx, u8 param_2) {
    extern s32 wp;
    extern void* smartAlloc(s32 size, s32 mode);
    s32 offset = poseIdx * 0x170;
    s32 pose;
    s32 data;
    s32 group;
    s32 mode;

    pose = *(s32*)(wp + 0x10) + offset;
    data = *(s32*)wp;
    group = **(s32**)(*(s32*)(data + (*(s32*)(pose + 0x10) << 4) + 8) + 0xA0);
    if ((*(u32*)pose & 0x40) == 0) {
        *(u32*)pose |= 0x40;
        switch (*(s32*)(pose + 0xC)) {
            case 0:
                mode = 1;
                break;
            case 1:
                mode = 2;
                break;
            case 2:
                mode = 0;
                break;
        }
        *(s32*)(pose + 0x160) = *(s32*)(group + 0x144);
        *(void**)(pose + 0x15C) = smartAlloc(*(s32*)(group + 0x144) * 0xC, mode);
    }
}


int animPoseGetGroupIdx(int poseIdx, char* pGroupName) {
    extern s32 wp;
    extern s32 strcmp(const char*, const char*);
    s32 group;
    s32 entry;
    s32 i = 0;
    register s32 offset = poseIdx * 0x170;
    s32 data = *(s32*)wp;
    s32 base = *(s32*)(wp + 0x10);

    base += offset;
    data += *(s32*)(base + 0x10) << 4;
    group = **(s32**)(*(s32*)(data + 8) + 0xA0);
    entry = *(s32*)(group + 0x1A8);
    while (i < *(s32*)(group + 0x144)) {
        if (strcmp((char*)entry, pGroupName) == 0) {
            return i;
        }
        i++;
        entry += 0x58;
    }
    return -1;
}

int animPoseGetShapeIdx(int poseIdx, char* pShapeName) {
    extern s32 wp;
    extern s32 strcmp(const char*, const char*);
    s32 group;
    s32 entry;
    s32 i = 0;
    register s32 offset = poseIdx * 0x170;
    s32 data = *(s32*)wp;
    s32 base = *(s32*)(wp + 0x10);

    base += offset;
    data += *(s32*)(base + 0x10) << 4;
    group = **(s32**)(*(s32*)(data + 8) + 0xA0);
    entry = *(s32*)(group + 0x14C);
    while (i < *(s32*)(group + 0xE8)) {
        if (strcmp((char*)entry, pShapeName) == 0) {
            return i;
        }
        i++;
        entry += 0xA8;
    }
    return -1;
}

s64 animTimeGetTime(int param_1) {
    extern s32 gp;

    if (param_1 == 0) {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        return *(s64*)(gp + 0x40) / ticks;
    } else {
        u32 ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        return *(s64*)(gp + 0x38) / ticks;
    }
}

void animPaperPoseDisp(void) {
    s32 i;
    s32 pose;

    i = 0;
    pose = *(s32*)(wp + 0x10);
    while (i < *(s32*)(wp + 0x14)) {
        if ((*(u32*)pose & 1) != 0 && *(s32*)(pose + 0x90) != -1) {
            animPaperPoseDispSub(0, (void*)pose);
        }
        i++;
        pose += 0x170;
    }
}


extern void PSMTXTrans(void* mtx, double x, double y, double z);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void animPoseDraw(int poseIdx, double x, double y, double z, double rot, double scale, int param_7) {
    f32 mtx[3][4];

    PSMTXTrans(mtx, x, y, z);
    animPoseDrawMtx(poseIdx, mtx, param_7, rot, scale);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


f32 animPoseGetHeight(s32 poseId) {
    s32 base;
    s32 pose;
    s32 group;
    s32 data;
    s32 value;

    pose = *(s32*)(wp + 0x10);
    pose += poseId * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    base += group * 0x10;
    data = *(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0);
    value = *(s32*)(data + 0xCC);

    return 2.0f * (f32)value;
}


f32 animPoseGetRadius(s32 poseId) {
    s32 base;
    s32 pose;
    s32 group;
    s32 data;
    s32 value;

    pose = *(s32*)(wp + 0x10);
    pose += poseId * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    base += group * 0x10;
    data = *(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0);
    value = *(s32*)(data + 0xC8);

    return 2.0f * (f32)value;
}


u32 animPoseGetPeraEnd(int poseId) {
    s32 pose;

    pose = *(s32*)(wp + 0x10) + poseId * 0x170;
    if ((*(u32*)pose & 8) == 0) {
        return 1;
    }
    return __fabsf(*(f32*)(pose + 0x7C) - *(f32*)(pose + 0x74)) < 10.0;
}


s32 animPoseGetAnimDataPtr(s32 poseIdx) {
    s32 base;
    s32 pose;
    s32 group;
    s32 anim;
    s32 data;

    pose = *(s32*)(wp + 0x10);
    pose += poseIdx * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    anim = *(s32*)(pose + 0x14);
    base += group * 0x10;
    data = *(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0);
    data = *(s32*)(data + 0x1AC);
    data += anim * 0x40;
    return *(s32*)(data + 0x3C);
}


void animPosePaperPeraOn(s32 poseId) {
    s32 base;
    void* pose;
    void* paper;

    base = *(s32*)(wp + 0x10);
    pose = (void*)(base + poseId * 0x170);
    paper = (void*)(base + *(s32*)((s32)pose + 0x90) * 0x170);
    *(u32*)paper |= 8;
    *(f32*)((s32)paper + 0x70) = *(f32*)((s32)pose + 0x70);
    *(f32*)((s32)paper + 0x74) = *(f32*)((s32)pose + 0x74);
    *(f32*)((s32)paper + 0x7C) = *(f32*)((s32)pose + 0x7C);
}

char* animPoseGetGroupName(s32 poseIdx, s32 groupIdx) {
    s32 base;
    s32 pose;
    s32 group;
    s32 offset;
    s32 data;

    pose = *(s32*)(wp + 0x10);
    pose += poseIdx * 0x170;
    base = *(s32*)wp;
    offset = groupIdx * 0x58;
    group = *(s32*)(pose + 0x10);
    base += group * 0x10;
    data = *(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0);
    return (char*)(*(s32*)(data + 0x1A8) + offset);
}


u32 animPoseTestXLU(s32 poseId) {
    s32 base;
    s32 pose;
    s32 group;
    s32 flags;

    pose = *(s32*)(wp + 0x10);
    pose += poseId * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    base += group * 0x10;
    flags = *(s32*)(*(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0) + 0xC4);
    return (flags >> 3) & 1;
}


void animMain(void) {
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void animPaperPoseDisp(void);

    dispEntry(0, 1, animPaperPoseDisp, 0, 0.0f);
}


void animPosePaperPeraOff(s32 poseId) {
    s32 work;
    s32 offset;
    s32 base;
    s32 pose;
    s32 paper;

    work = wp;
    offset = poseId * 0x170;
    base = *(s32*)(work + 0x10);
    pose = base;
    pose += offset;
    paper = *(s32*)(pose + 0x90) * 0x170;
    *(u32*)(base + paper) &= ~8;
}

void animPoseSetStartTime(int poseId, s32 unused, s32 start, s32 end) {
    s32 ptr;

    ptr = *(s32*)(wp + 0x10) + poseId * 0x170 + 0x18;
    *(s32*)(ptr + 4) = end;
    *(s32*)ptr = start;
}


void animPoseSetMaterialEvtColor(s32 poseId, void* color) {
    *(s32*)(*(s32*)(wp + 0x10) + poseId * 0x170 + 0xF0) = *(s32*)color;
}


void animPose_AllocBuffer(void* pPose) {
    ;
}


void _animPoseDrawMtx(void* pPose, void* pMtx, int dispMode, double rot, double scale) {
    ;
}


u8 animSetMaterial_Texture(s32 texCount, int* pTexIdRemap, void* pTexEntries, void* pAnimEntries, s32 texBindsCapacity, void* pTexBinds, void* pTexFile) {
    return 0;
}


void pushGXModelMtx_JointNode__(void* pJoint, double parentScaleX, double parentScaleY, double parentScaleZ) {
    ;
}


u8 pushGXModelMtx_TransformNode__(void* pNode) {
    return 0;
}


void animSetPaperTexObj(void* param_1, void* param_2, void* param_3, void* param_4, void* param_5, void* param_6, s32 param_7) {
    *(void**)(wp + 0x30) = param_4 != 0 ? param_1 : 0;
    *(void**)(wp + 0x34) = param_5 != 0 ? param_2 : 0;
    *(void**)(wp + 0x38) = param_6 != 0 ? param_3 : 0;
    *(void**)(wp + 0x24) = param_4;
    *(void**)(wp + 0x28) = param_5;
    *(void**)(wp + 0x2C) = param_6;
    *(s32*)(wp + 0xD8) = param_7;
    *(s32*)(wp + 0xDC) = param_7;
}

void* animPoseGetCurrentAnim(int poseIdx) {
    s32 base;
    s32 pose;
    s32 group;
    s32 anim;

    pose = *(s32*)(wp + 0x10) + poseIdx * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    anim = *(s32*)(pose + 0x14);
    base += group * 0x10;
    return (void*)(*(s32*)(*(s32*)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0) + 0x1AC) + anim * 0x40);
}


void* animPoseGetAnimBaseDataPtr(s32 poseId) {
    s32 base;
    s32 pose;
    s32 group;

    pose = *(s32*)(wp + 0x10);
    pose += poseId * 0x170;
    base = *(s32*)wp;
    group = *(s32*)(pose + 0x10);
    base += group * 0x10;
    return *(void**)(*(s32*)(*(s32*)(base + 8) + 0xA0) + 0);
}
