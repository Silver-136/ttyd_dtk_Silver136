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
    typedef struct Vec3Local {
        f32 x;
        f32 y;
        f32 z;
    } Vec3Local;

    extern s32 wp;
    extern s32 gp;
    extern void memcpy_as4(void* dst, void* src, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void* camGetPtr(s32 cameraId);
    extern s64 __div2i(s64 dividend, s64 divisor);
    extern double atan2(double y, double x);
    extern f32 tanfTbl[];
    extern f32 float_0_8041fb28;
    extern f32 float_1_8041fb24;
    extern f32 float_2_8041fb64;
    extern f32 float_3_8041fbb0;
    extern f32 float_60_8041fba4;
    extern f32 float_1000_8041fba8;
    extern f32 float_0p0625_8041fbac;
    extern f32 float_rad2deg_8041fb74;
    extern f32 float_360_8041fb78;
    extern f32 float_neg360_8041fb7c;
    extern f32 float_90_8041fb80;
    extern f32 float_270_8041fb84;
    extern f32 float_neg90_8041fb70;
    extern f32 float_neg270_8041fb88;
    extern f32 float_neg1_8041fb6c;
    extern f32 float_neg2_8041fb98;

    s32 work;
    s32 poses;
    s32 pose;
    s32 file;
    s32 poseData;
    s32 animEntry;
    s32 animData;
    s32 loopData;
    s32 frameData;
    s32 frame;
    s32 frameEnd;
    s32 frameCount;
    s32 loopFlag;
    s32 anim0;
    s32 anim1;
    s32 oldAnim0;
    s32 posCount;
    s32 nrmCount;
    s32 visCount;
    s32 nodeCount;
    s32 texCount;
    s32 i;
    s32 j;
    s32 count;
    s32 idx;
    u8* upd;
    u8* updEnd;
    Vec3Local* vtx;
    f32* node;
    s32 tex;
    s32 outPos;
    s32 outNrm;
    u64 now;
    u64 oldTime;
    u32 ticks;
    f32 cur;
    f32 start;
    f32 end;
    f32 duration;
    f32 blend;
    f32 scale;
    f32 ftmp;
    f32 angle;
    f32 delta;
    void* cam;

    work = wp;
    poses = *(s32*)(work + 0x10);
    pose = poses + poseIdx * 0x170;
    file = *(s32*)(*(s32*)work + (*(s32*)(pose + 0x10) << 4) + 8);
    poseData = **(s32**)(file + 0xA0);
    animEntry = *(s32*)(poseData + 0x1AC) + (*(s32*)(pose + 0x14) << 6);
    animData = *(s32*)(animEntry + 0x3C);

    if (*(s32*)(work + 0xE8) == 0) {
        outPos = *(s32*)(pose + 0x4C);
        outNrm = *(s32*)(pose + 0x54);
    } else {
        outPos = *(s32*)(work + 0xF8);
        outNrm = *(s32*)(work + 0xFC);
    }

    if ((*(s32*)(pose + 0x90) != -1) && ((*(u32*)(poses + *(s32*)(pose + 0x90) * 0x170 + 4) & 2U) == 0)) {
        animPoseMain(*(s32*)(pose + 0x90));
    }

    if (animData == 0) {
        return 0;
    }

    loopData = *(s32*)(animData + 0x24);
    loopFlag = *(s32*)loopData;

    ticks = (*(u32*)0x800000F8 >> 2) / 1000;
    if (*(s32*)(pose + 0xC) == 0) {
        now = (u64)__div2i(*(s64*)(gp + 0x40), (s64)(s32)ticks);
    } else {
        now = (u64)__div2i(*(s64*)(gp + 0x38), (s64)(s32)ticks);
    }

    oldTime = *(u64*)(pose + 0x18);
    cur = (float)(now - oldTime);
    cur = (float_60_8041fba4 * cur) / float_1000_8041fba8;
    cur *= *(f32*)(pose + 0x44);
    *(f32*)(pose + 0x20) = cur;

    start = *(f32*)(loopData + 4);
    end = *(f32*)(loopData + 8);
    duration = end - start;

    if (duration == float_0_8041fb28) {
        *(f32*)(pose + 0x84) = cur - start;
        cur = start;
    } else if (loopFlag == 0) {
        if (cur < start) {
            *(f32*)(pose + 0x84) = float_0_8041fb28;
            cur = start;
        } else if (cur > end) {
            *(f32*)(pose + 0x84) = (cur - start) / duration;
            cur = end;
        } else {
            *(f32*)(pose + 0x84) = (cur - start) / duration;
        }
    } else {
        if ((cur - start) >= float_0_8041fb28) {
            f32 loops = (cur - start) / duration;
            s32 whole = (s32)loops;
            *(f32*)(pose + 0x84) = loops;
            cur -= (f32)whole * duration;
        } else {
            *(f32*)(pose + 0x84) = float_0_8041fb28;
            cur = start;
        }
    }

    *(f32*)(pose + 0x24) = cur;

    frameData = *(s32*)(animData + 0x28);
    frameCount = *(s32*)(animData + 8);
    anim0 = 0;
    for (i = 0; i < frameCount; i++) {
        if (cur < *(f32*)(frameData + i * 0x2C)) {
            break;
        }
        anim0++;
    }
    *(s32*)(pose + 0x28) = anim0 - 1;
    *(s32*)(pose + 0x2C) = *(s32*)(pose + 0x28) + 1;

    count = *(s32*)(animData + 0x20);
    anim1 = 0;
    for (i = 0; i < count; i++) {
        if (cur < *(f32*)(*(s32*)(animData + 0x40) + i * 8)) {
            break;
        }
        anim1++;
    }
    *(s32*)(pose + 0x34) = anim1 - 1;
    *(s32*)(pose + 0x38) = *(s32*)(pose + 0x34) + 1;

    if (loopFlag == 0) {
        if (*(s32*)(pose + 0x2C) >= frameCount) {
            *(s32*)(pose + 0x2C) = *(s32*)(pose + 0x28);
        }
        if (*(s32*)(pose + 0x38) >= count) {
            *(s32*)(pose + 0x38) = *(s32*)(pose + 0x34);
        }
    }

    *(s32*)(pose + 0x80) = *(s32*)(*(s32*)(animData + 0x40) + *(s32*)(pose + 0x34) * 8 + 4);

    anim0 = *(s32*)(pose + 0x28);
    anim1 = *(s32*)(pose + 0x2C);
    oldAnim0 = *(s32*)(pose + 0x3C);
    blend = *(f32*)(pose + 0x30);

    start = *(f32*)(frameData + anim0 * 0x2C);
    if (anim0 == anim1) {
        duration = float_0_8041fb28;
    } else if ((loopFlag != 0) && (anim1 >= frameCount)) {
        anim1 = 0;
        *(s32*)(pose + 0x2C) = 0;
        duration = *(f32*)(loopData + 8) - *(f32*)(frameData + anim0 * 0x2C);
    } else {
        duration = *(f32*)(frameData + anim1 * 0x2C) - start;
    }

    if (duration == float_0_8041fb28) {
        blend = float_0_8041fb28;
    } else {
        blend = (cur - start) / duration;
    }
    *(f32*)(pose + 0x30) = blend;

    posCount = *(s32*)(poseData + 0xF0);
    nrmCount = *(s32*)(poseData + 0xF8);
    visCount = *(s32*)(poseData + 0x13C);
    nodeCount = *(s32*)(poseData + 0x140);
    texCount = *(s32*)(poseData + 0x12C);

    if (oldAnim0 == anim0) {
        if (blend < *(f32*)(pose + 0x40)) {
            *(f32*)(pose + 0x40) = float_0_8041fb28;
        }
    } else if ((oldAnim0 == -1) || (anim0 < oldAnim0)) {
        if (posCount != 0) memcpy_as4(*(void**)(pose + 0x48), *(void**)(poseData + 0x154), posCount * 0xC);
        if (nrmCount != 0) memcpy_as4(*(void**)(pose + 0x50), *(void**)(poseData + 0x15C), nrmCount * 0xC);
        if (visCount != 0) memcpy(*(void**)(pose + 0x58), *(void**)(poseData + 0x1A0), visCount);
        if (nodeCount != 0) memcpy_as4(*(void**)(pose + 0x60), *(void**)(poseData + 0x1A4), nodeCount << 2);
        if (texCount != 0) memcpy_as4(*(void**)(pose + 0x68), *(void**)(poseData + 0x190), texCount * 0x18);
        *(s32*)(pose + 0x3C) = 0;
        oldAnim0 = -1;
    }

    frameEnd = anim0;
    if (oldAnim0 >= 0 && oldAnim0 < anim0) {
        i = oldAnim0 + 1;
    } else {
        i = 0;
    }

    for (; i <= frameEnd; i++) {
        frame = frameData + i * 0x2C;
        scale = float_0p0625_8041fbac;

        upd = (u8*)(*(s32*)(animData + 0x2C) + *(s32*)(frame + 4) * 4);
        for (j = 0; j < *(s32*)(frame + 8); j++, upd += 4) {
            vtx = (Vec3Local*)*(s32*)(pose + 0x48);
            vtx += upd[0];
            vtx->x += scale * (f32)(s8)upd[1];
            vtx->y += scale * (f32)(s8)upd[2];
            vtx->z += scale * (f32)(s8)upd[3];
        }

        upd = (u8*)(*(s32*)(animData + 0x30) + *(s32*)(frame + 0xC) * 4);
        for (j = 0; j < *(s32*)(frame + 0x10); j++, upd += 4) {
            vtx = (Vec3Local*)*(s32*)(pose + 0x50);
            vtx += upd[0];
            vtx->x += scale * (f32)(s8)upd[1];
            vtx->y += scale * (f32)(s8)upd[2];
            vtx->z += scale * (f32)(s8)upd[3];
        }

        upd = (u8*)(*(s32*)(animData + 0x38) + *(s32*)(frame + 0x1C) * 2);
        updEnd = upd + *(s32*)(frame + 0x20) * 2;
        idx = 0;
        while (upd < updEnd) {
            idx += upd[0];
            *(u8*)(*(s32*)(pose + 0x58) + idx) += upd[1];
            upd += 2;
        }

        upd = (u8*)(*(s32*)(animData + 0x3C) + *(s32*)(frame + 0x24) * 4);
        for (j = 0; j < *(s32*)(frame + 0x28); j++, upd += 4) {
            node = (f32*)*(s32*)(pose + 0x60);
            node += upd[0];
            *node += scale * (f32)(s8)upd[1];
        }

        upd = (u8*)(*(s32*)(animData + 0x34) + *(s32*)(frame + 0x14) * 0xC);
        for (j = 0; j < *(s32*)(frame + 0x18); j++, upd += 0xC) {
            tex = *(s32*)(pose + 0x68) + upd[0] * 0x18;
            *(u8*)tex += upd[1];
            *(f32*)(tex + 4) += *(f32*)(upd + 4);
            *(f32*)(tex + 8) += *(f32*)(upd + 8);
        }
    }

    if (anim0 == anim1 || anim1 < anim0) {
        if (posCount != 0) memcpy_as4((void*)outPos, *(void**)(pose + 0x48), posCount * 0xC);
        if (nrmCount != 0) memcpy_as4((void*)outNrm, *(void**)(pose + 0x50), nrmCount * 0xC);
        if (nodeCount != 0) memcpy_as4(*(void**)(pose + 0x64), *(void**)(pose + 0x60), nodeCount << 2);
        if (visCount != 0) memcpy(*(void**)(pose + 0x5C), *(void**)(pose + 0x58), visCount);
        if (texCount != 0) memcpy_as4(*(void**)(pose + 0x6C), *(void**)(pose + 0x68), texCount * 0x18);
    } else {
        frame = frameData + anim1 * 0x2C;
        scale = float_0p0625_8041fbac * blend;

        if (posCount != 0) memcpy_as4((void*)outPos, *(void**)(pose + 0x48), posCount * 0xC);
        upd = (u8*)(*(s32*)(animData + 0x2C) + *(s32*)(frame + 4) * 4);
        for (j = 0; j < *(s32*)(frame + 8); j++, upd += 4) {
            vtx = (Vec3Local*)outPos;
            vtx += upd[0];
            vtx->x += scale * (f32)(s8)upd[1];
            vtx->y += scale * (f32)(s8)upd[2];
            vtx->z += scale * (f32)(s8)upd[3];
        }

        if (nrmCount != 0) memcpy_as4((void*)outNrm, *(void**)(pose + 0x50), nrmCount * 0xC);
        upd = (u8*)(*(s32*)(animData + 0x30) + *(s32*)(frame + 0xC) * 4);
        for (j = 0; j < *(s32*)(frame + 0x10); j++, upd += 4) {
            vtx = (Vec3Local*)outNrm;
            vtx += upd[0];
            vtx->x += scale * (f32)(s8)upd[1];
            vtx->y += scale * (f32)(s8)upd[2];
            vtx->z += scale * (f32)(s8)upd[3];
        }

        if (nodeCount != 0) memcpy_as4(*(void**)(pose + 0x64), *(void**)(pose + 0x60), nodeCount << 2);
        upd = (u8*)(*(s32*)(animData + 0x3C) + *(s32*)(frame + 0x24) * 4);
        duration = *(f32*)(frameData + anim1 * 0x2C) - *(f32*)(frameData + anim0 * 0x2C);
        ftmp = blend * blend;
        for (j = 0; j < *(s32*)(frame + 0x28); j++, upd += 4) {
            s32 a = (s8)upd[3];
            node = (f32*)*(s32*)(pose + 0x64);
            node += upd[0];
            if (a < 0x59 && a > -0x59) {
                f32 t3 = blend * ftmp;
                *node += (t3 - ftmp) * duration * tanfTbl[a + 0x59]
                      + (float_2_8041fb64 * t3 + float_3_8041fbb0 * ftmp) * float_0p0625_8041fbac * (f32)(s8)upd[1]
                      + (blend - (float_3_8041fbb0 * ftmp - float_2_8041fb64 * t3)) * duration * tanfTbl[(s8)upd[2] + 0x59];
            } else {
                *node += scale * (f32)(s8)upd[1];
            }
        }

        if (visCount != 0) memcpy(*(void**)(pose + 0x5C), *(void**)(pose + 0x58), visCount);
        if (texCount != 0) memcpy_as4(*(void**)(pose + 0x6C), *(void**)(pose + 0x68), texCount * 0x18);
    }

    *(s32*)(pose + 0x3C) = anim0;
    *(f32*)(pose + 0x40) = blend;

    if ((*(u32*)pose & 8U) != 0) {
        cam = camGetPtr(4);
        angle = float_rad2deg_8041fb74 * (f32)atan2((double)-(*(f32*)((s32)cam + 0x18) - *(f32*)((s32)cam + 0xC)),
                                                   (double)-(*(f32*)((s32)cam + 0x20) - *(f32*)((s32)cam + 0x14)));
        while (angle < float_360_8041fb78) angle += float_60_8041fba4;
        while (float_60_8041fba4 <= angle) angle -= float_60_8041fba4;
        if (angle < float_0_8041fb28) angle += float_60_8041fba4;
        delta = angle - *(f32*)(pose + 0x74);
        while (delta < float_360_8041fb78) delta += float_60_8041fba4;
        while (float_60_8041fba4 <= delta) delta -= float_60_8041fba4;
        if (((float_0_8041fb28 < delta) && (delta <= float_90_8041fb80)) ||
            ((delta < float_90_8041fb80) && (float_270_8041fb84 <= delta))) {
            *(f32*)(pose + 0x78) = float_1_8041fb24;
        } else {
            *(f32*)(pose + 0x78) = float_0_8041fb28;
        }
    }

    return 0;
}

void renderProc(int shapeIdx, int animFrame0) {
    typedef f32 Mtx[3][4];
    extern s32 wp;
    extern s32 g_modeling_mtx;
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXInvXpose(void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXLoadNrmMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32, void*, s32);
    extern void GXBegin(s32, s32, s32);
    s32 work;
    s32 pose;
    s32 poseData;
    s32 shape;
    s32 draw;
    s32 drawCount;
    s32 firstVtxPos;
    s32 firstVtxNrm;
    s32 firstVtxClr;
    s32 texArrays[8];
    s32 drawIdx;
    Mtx posMtx;
    Mtx nrmMtx;
    volatile u16* fifo = (volatile u16*)0xCC008000;

    (void)animFrame0;
    if (shapeIdx == -1) {
        return;
    }

    PSMTXConcat((u8*)camGetCurPtr() + 0x11C, (void*)g_modeling_mtx, posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    PSMTXInvXpose(posMtx, nrmMtx);
    GXLoadNrmMtxImm(nrmMtx, 0);
    GXSetCurrentMtx(0);

    work = wp;
    pose = *(s32*)(work + 0x18);
    poseData = *(s32*)(work + 0x20);
    shape = *(s32*)(poseData + 0x14C) + shapeIdx * 0xA8;
    drawCount = *(s32*)(shape + 0x9C);

    if (!((*(s32*)(shape + 0xA0) == 0 && *(s32*)(work + 0xE0) == 2) ||
          (*(s32*)(shape + 0xA0) == *(s32*)(work + 0xE0)) ||
          (*(u8*)(pose + 0xF3) != 0xFF) ||
          (*(u8*)(pose + 0xF7) != 0xFF))) {
        return;
    }

    GXSetCullMode(*(s32*)(shape + 0xA4));
    firstVtxPos = *(s32*)(shape + 0x40);
    firstVtxNrm = *(s32*)(shape + 0x48);
    firstVtxClr = *(s32*)(shape + 0x50);
    for (drawIdx = 0; drawIdx < 8; drawIdx++) {
        texArrays[drawIdx] = *(s32*)(poseData + 0x18C) +
                             *(s32*)(shape + 0x58 + drawIdx * 8) * 8;
    }

    draw = *(s32*)(poseData + 0x19C) + *(s32*)(shape + 0x98) * 0x6C;
    for (drawIdx = 0; drawIdx < drawCount; drawIdx++, draw += 0x6C) {
        s32 texCount = *(s32*)draw;
        s32 firstDrawCall = *(s32*)(draw + 0x38);
        s32 callCount = *(s32*)(draw + 0x3C);
        s32 firstIdxPos = *(s32*)(draw + 0x40);
        s32 firstIdxNrm = *(s32*)(draw + 0x44);
        s32 firstIdxClr = *(s32*)(draw + 0x48);
        s32 drawCall = *(s32*)(poseData + 0x164) + firstDrawCall * 8;
        s32 i;

        GXClearVtxDesc();
        GXSetVtxDesc(9, 3);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetArray(9, (void*)(*(s32*)(pose + 0x4C) + firstVtxPos * 0xC), 0xC);
        GXSetVtxDesc(10, 3);
        GXSetVtxAttrFmt(0, 10, 0, 4, 0);
        GXSetArray(10, (void*)(*(s32*)(pose + 0x54) + firstVtxNrm * 0xC), 0xC);
        GXSetVtxDesc(11, 3);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetArray(11, (void*)(*(s32*)(poseData + 0x150) + firstVtxClr * 4), 4);

        for (i = 0; i < texCount; i++) {
            s32 texId = *(s8*)(draw + 0x30 + (texCount - i - 1));
            GXSetVtxDesc(13 + i, 3);
            GXSetVtxAttrFmt(0, 13 + i, 1, 4, 0);
            GXSetArray(13 + i, (void*)texArrays[texId], 8);
        }

        for (i = 0; i < callCount; i++, drawCall += 8) {
            s32 firstIdx = *(s32*)drawCall;
            s32 count = *(u16*)(drawCall + 6);
            s32 v;
            GXBegin(0xA0, 0, count);
            for (v = 0; v < count; v++) {
                s32 index = firstIdx + v;
                s32 t;
                *fifo = *(u16*)(*(s32*)(poseData + 0x158) +
                                 (index + firstIdxPos) * 4 + 2);
                *fifo = *(u16*)(*(s32*)(poseData + 0x160) +
                                 (index + firstIdxNrm) * 4 + 2);
                *fifo = *(u16*)(*(s32*)(poseData + 0x168) +
                                 (index + firstIdxClr) * 4 + 2);
                for (t = 0; t < texCount; t++) {
                    s32 texId = *(s8*)(draw + 0x30 + (texCount - t - 1));
                    s32 firstTex = *(s32*)(draw + 0x4C + texId * 4);
                    *fifo = *(u16*)(*(s32*)(poseData + 0x16C + texId * 4) +
                                     (index + firstTex) * 4 + 2);
                }
            }
        }
    }
}

void animPaperPoseDispSub(s32 cameraId, void* posePtr) {
    extern s32 wp;
    extern s32 gp;
    extern void* DEMOGetRenderModeObj(void);
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void PSMTXTrans(void* mtx, double x, double y, double z);
    extern void PSMTXCopy(void* src, void* dst);
    extern void _animPoseDrawMtx(void* pPose, void* pMtx, s32 dispMode, double rot, double scale);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern void GXSetTexCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, u32 format, u32 mipmap);
    extern u32 GXGetTexBufferSize(u32 width, u32 height, s32 format, u8 mipmap, u32 max_lod);
    extern void* smartAlloc(u32 size, s32 heap);
    extern void GXCopyTex(void* dest, u8 clear);
    extern void GXPixModeSync(void);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, u32 format, u32 wrapS, u32 wrapT, u8 mipmap);
    extern f32 float_0_8041fb28;
    extern f32 float_0p25_8041fb4c;
    extern f32 float_0p5_8041fb44;
    extern f32 float_0p75_8041fb48;
    extern f32 float_8_8041fb50;
    extern f32 float_25p74_8041fb54;
    extern f32 float_0p14881_8041fb58;
    extern f32 float_150_8041fb5c;
    extern f32 float_215_8041fb60;
    extern u32 dat_8041fb14;
    extern u32 dat_8041fb18;

    s32 pose = (s32)posePtr;
    s32 work;
    s32 file;
    s32 data;
    s32 animTable;
    s32 animData;
    void* renderMode;
    f32 scale;
    f32 screen75;
    f32 mtx[3][4];
    f32 oldMtx[3][4];
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    u32 oldFlags;
    s32 oldMatFlag;
    s32 oldLightFlag;
    volatile u32 oldColor0;
    volatile u32 oldColor1;
    s32 half;
    s32 v0;
    s32 v1;
    s32 hpad;
    s32 vpad;
    u32 width32;
    u32 height32;
    u16 width;
    u16 height;
    u32 size;
    void* alloc;
    f32 invScale;
    (void)cameraId;

    if ((*(u32*)pose & 1) == 0) {
        return;
    }

    work = wp;
    file = *(s32*)(*(s32*)work + (*(s32*)(pose + 0x10) << 4) + 8);
    data = *(s32*)(file + 0xA0);
    data = *(s32*)data;
    animTable = *(s32*)(data + 0x1AC);
    animData = *(s32*)(animTable + (*(s32*)(pose + 0x14) << 6) + 0x3C);

    scale = float_8_8041fb50 * (float_25p74_8041fb54 / (*(f32*)(animData + 0x54) - *(f32*)(animData + 0x48)));
    renderMode = DEMOGetRenderModeObj();
    screen75 = float_0p75_8041fb48 * (f32)*(u16*)((s32)renderMode + 6);

    sysWaitDrawSync();
    GXClearBoundingBox();

    renderMode = DEMOGetRenderModeObj();
    PSMTXTrans(mtx, (double)float_0_8041fb28,
               (double)(float_0p25_8041fb4c * -((f32)*(u16*)((s32)renderMode + 6)) - (*(f32*)(animData + 0x48) * scale)),
               (double)float_0_8041fb28);

    oldFlags = *(u32*)pose;
    oldColor0 = *(u32*)(pose + 0xF0);
    oldColor1 = *(u32*)(pose + 0xF4);
    oldMatFlag = *(s32*)(pose + 0xE8);
    oldLightFlag = *(s32*)(pose + 0xEC);
    *(u32*)pose = oldFlags & ~8U;
    *(u32*)(pose + 0xF0) = dat_8041fb14;
    *(u32*)(pose + 0xF4) = dat_8041fb18;
    *(s32*)(pose + 0xEC) = 0;

    if (*(s32*)(pose + 0x11C) != 0) {
        PSMTXCopy((void*)(pose + 0x124), oldMtx);
        PSMTXCopy(mtx, (void*)(pose + 0x124));
    }

    _animPoseDrawMtx((void*)pose, mtx, 2, (double)float_0_8041fb28, (double)scale);

    *(u32*)pose = oldFlags;
    *(s32*)(pose + 0xE8) = oldMatFlag;
    *(u32*)(pose + 0xF0) = oldColor0;
    *(u32*)(pose + 0xF4) = oldColor1;
    *(s32*)(pose + 0xEC) = oldLightFlag;

    if (*(s32*)(pose + 0x11C) != 0) {
        PSMTXCopy(oldMtx, (void*)(pose + 0x124));
    }

    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);

    if (right < left) {
        left = 0;
        top = 0;
        right = 0x10;
        bottom = 0x10;
    }

    if (((*(u32*)pose & 2U) != 0) && ((*(u32*)pose & 4U) != 0) && (*(s32*)(gp + 0x14) != 0)) {
        renderMode = DEMOGetRenderModeObj();
        v0 = (s32)(*(f32*)(animData + 0x44) * scale + (f32)(*(u16*)((s32)renderMode + 4) >> 1));
        renderMode = DEMOGetRenderModeObj();
        v1 = (s32)(*(f32*)(animData + 0x50) * scale + (f32)(*(u16*)((s32)renderMode + 4) >> 1));
        hpad = (s32)-(scale * (*(f32*)(animData + 0x54) - *(f32*)(animData + 0x48)) - screen75);
        vpad = (s32)screen75;
        if ((v0 <= (s32)(u32)left) && ((s32)(u32)right <= v1) &&
            (hpad <= (s32)(u32)top) && ((s32)(u32)bottom <= vpad)) {
            left = (u16)v0;
            right = (u16)v1;
            top = (u16)hpad;
            bottom = (u16)vpad;
        }
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
    if ((s32)(*(u16*)((s32)renderMode + 6) - 2) < (s32)(u32)bottom) {
        renderMode = DEMOGetRenderModeObj();
        bottom = *(u16*)((s32)renderMode + 6) - 2;
    }

    if ((*(u32*)pose & 2U) == 0) {
        renderMode = DEMOGetRenderModeObj();
        hpad = (s32)((*(u16*)((s32)renderMode + 4) >> 1) - (u32)left);
        renderMode = DEMOGetRenderModeObj();
        v0 = (s32)((u32)right - (*(u16*)((s32)renderMode + 4) >> 1));
        if (hpad < v0) {
            hpad = v0;
        }
        v1 = (s32)((u32)bottom - (u32)top);
        v0 = (s32)(float_0p5_8041fb44 * (f32)hpad);
        vpad = (s32)(float_0p14881_8041fb58 * (f32)v1);
        renderMode = DEMOGetRenderModeObj();
        *(u16*)(pose + 0x114) = (u16)(((*(u16*)((s32)renderMode + 4) >> 1) - hpad) - v0);
        renderMode = DEMOGetRenderModeObj();
        *(u16*)(pose + 0x116) = (u16)(hpad + (*(u16*)((s32)renderMode + 4) >> 1) + v0);
        *(u16*)(pose + 0x118) = (u16)(top - vpad);
        *(u16*)(pose + 0x11A) = (u16)(bottom + vpad);
        invScale = float_8_8041fb50 / scale;
        *(f32*)(pose + 0xF8) = invScale * ((f32)((s16)*(u16*)(pose + 0x116) - (s16)*(u16*)(pose + 0x114)) / float_150_8041fb5c);
        *(f32*)(pose + 0xFC) = invScale * ((f32)((s16)*(u16*)(pose + 0x11A) - (s16)*(u16*)(pose + 0x118)) / float_215_8041fb60);
        *(f32*)(pose + 0x100) = *(f32*)(animData + 0x48) + ((screen75 - (f32)(u32)bottom) / scale);
    } else if ((*(u32*)pose & 4U) != 0) {
        renderMode = DEMOGetRenderModeObj();
        hpad = (s32)((*(u16*)((s32)renderMode + 4) >> 1) - (u32)left);
        renderMode = DEMOGetRenderModeObj();
        v0 = (s32)((u32)right - (*(u16*)((s32)renderMode + 4) >> 1));
        if (hpad < v0) {
            hpad = v0;
        }
        v1 = (s32)((u32)bottom - (u32)top);
        v0 = (s32)(float_0p5_8041fb44 * (f32)hpad);
        vpad = (s32)(float_0p14881_8041fb58 * (f32)v1);
        renderMode = DEMOGetRenderModeObj();
        *(u16*)(pose + 0x114) = (u16)(((*(u16*)((s32)renderMode + 4) >> 1) - hpad) - v0);
        renderMode = DEMOGetRenderModeObj();
        *(u16*)(pose + 0x116) = (u16)(hpad + (*(u16*)((s32)renderMode + 4) >> 1) + v0);
        *(u16*)(pose + 0x118) = (u16)(top - vpad);
        *(u16*)(pose + 0x11A) = (u16)(bottom + vpad);
        invScale = float_8_8041fb50 / scale;
        *(f32*)(pose + 0xF8) = invScale * ((f32)((s16)*(u16*)(pose + 0x116) - (s16)*(u16*)(pose + 0x114)) / float_150_8041fb5c);
        *(f32*)(pose + 0xFC) = invScale * ((f32)((s16)*(u16*)(pose + 0x11A) - (s16)*(u16*)(pose + 0x118)) / float_215_8041fb60);
        *(f32*)(pose + 0x100) = *(f32*)(animData + 0x48) + ((screen75 - (f32)(u32)bottom) / scale);
        *(u32*)pose &= ~4U;
    }

    left = (left - 2) - ((left - 2) & 1);
    top = (top - 2) - ((top - 2) & 1);
    right = (right + 2) + ((right + 2) & 1);
    bottom = (bottom + 2) + ((bottom + 2) & 1);

    width32 = ((u32)right & 0xFFFF) - (u32)left;
    height32 = ((u32)bottom & 0xFFFF) - (u32)top;
    width = (u16)width32;
    height = (u16)height32;

    *(f32*)(pose + 0x104) = -(((f32)(u32)left - (f32)(s16)*(u16*)(pose + 0x114)) /
                              (f32)((s16)*(u16*)(pose + 0x116) - (s16)*(u16*)(pose + 0x114)));
    *(f32*)(pose + 0x108) = -(((f32)(u32)top - (f32)(s16)*(u16*)(pose + 0x118)) /
                              (f32)((s16)*(u16*)(pose + 0x11A) - (s16)*(u16*)(pose + 0x118)));
    *(f32*)(pose + 0x10C) = (f32)((s16)*(u16*)(pose + 0x116) - (s16)*(u16*)(pose + 0x114)) / (f32)width32;
    *(f32*)(pose + 0x110) = (f32)((s16)*(u16*)(pose + 0x11A) - (s16)*(u16*)(pose + 0x118)) / (f32)height32;

    GXSetTexCopySrc((u32)left, (u32)top, width32 & 0xFFFF, height32 & 0xFFFF);
    GXSetTexCopyDst(width, height, 5, 0);
    size = GXGetTexBufferSize(width32 & 0xFFFF, height32 & 0xFFFF, 5, 0, 0);
    alloc = smartAlloc(size, 3);
    *(void**)(pose + 0x98) = alloc;
    GXCopyTex(*(void**)alloc, 1);
    GXPixModeSync();
    GXInitTexObj((void*)(pose + 0xA0), *(void**)*(void***)(pose + 0x98), width, height, 5, 0, 0, 0);
    *(s32*)(pose + 0x9C) = 0;
}

void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, double rotValue, double scaleValue) {
    typedef f32 Mtx[3][4];
    extern s32 wp;
    extern void PSMTXRotRad(void* m, double rad, char axis);
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* m, double x, double y, double z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXCopy(void* src, void* dst);
    extern void evalProc(s32 parent, s32 group);
    extern u8 animPoseMain(s32 poseId);
    extern void _animPoseDrawMtx(void* pose, void* mtx, s32 mode, double rot, double scale);
    extern void* g_modeling_mtx;
    extern s32 g_modeling_mtx_lv;
    extern u8 g_modeling_mtx_stack[];
    extern f32 float_0_8041fb28;
    extern f32 float_0p5_8041fb44;
    extern f32 float_1_8041fb24;
    extern f32 float_deg2rad_8041fbb4;
    s32 work;
    s32 poses;
    s32 pose;
    s32 drawPose;
    s32 file;
    s32 data;
    s32 effectId;
    s32 count;
    s32 posCount;
    s32 nrmCount;
    void* src;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx tempMtx;
    Mtx paperMtx;
    Mtx workMtx;

    work = wp;
    poses = *(s32*)(work + 0x10);
    pose = poses + poseId * 0x170;
    effectId = *(s32*)(pose + 0x90);

    file = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
    file = *(s32*)(file + 8);
    data = *(s32*)(*(s32*)(file + 0xA0));

    if (effectId == -1) {
        if (((mode != 2) || ((*(u32*)(data + 0xC4) & 1) == 0)) &&
            ((*(u32*)(data + 0xC4) & (1 << mode)) == 0)) {
            return;
        }
        _animPoseDrawMtx((void*)pose, mtx, mode, rotValue, scaleValue);
        return;
    }

    if ((*(u32*)pose & 0xC0) != 0) {
        src = mtx;
        if (((*(u32*)pose & 8) == 0) && (rotValue != (double)float_0_8041fb28)) {
            PSMTXRotRad(rotMtx, (double)(float_deg2rad_8041fbb4 * (f32)rotValue), 'y');
            PSMTXConcat(mtx, rotMtx, tempMtx);
            src = tempMtx;
        }
        if (scaleValue != (double)float_0_8041fb28) {
            PSMTXScale(scaleMtx, (f32)scaleValue, (f32)scaleValue, (f32)scaleValue);
            PSMTXConcat(src, scaleMtx, tempMtx);
            src = tempMtx;
        }
        g_modeling_mtx_lv = 0;
        g_modeling_mtx = g_modeling_mtx_stack;
        PSMTXCopy(src, g_modeling_mtx_stack);
        *(s32*)(work + 0x18) = pose;
        *(s32*)(work + 0x20) = data;
        evalProc(-1, *(s32*)(data + 0x144) - 1);
    }

    drawPose = poses + effectId * 0x170;
    file = *(s32*)work + (*(s32*)(drawPose + 0x10) << 4);
    file = *(s32*)(file + 8);
    data = *(s32*)(*(s32*)(file + 0xA0));
    if (((mode != 2) || ((*(u32*)(data + 0xC4) & 1) == 0)) &&
        ((*(u32*)(data + 0xC4) & (1 << mode)) == 0)) {
        return;
    }

    if ((*(u32*)(drawPose + 4) & 2) != 0) {
        posCount = *(s32*)(data + 0xF0);
        nrmCount = *(s32*)(data + 0xF8);
        count = posCount + nrmCount;
        if (count <= *(s32*)(work + 0xF0)) {
            if (*(s32*)(work + 0xF0) < *(s32*)(work + 0xF4) + count) {
                *(s32*)(work + 0xF4) = 0;
            }
            *(s32*)(work + 0xE8) = 1;
            *(s32*)(work + 0xF8) = *(s32*)(work + 0xEC) + *(s32*)(work + 0xF4) * 0xC;
            *(s32*)(work + 0xF4) += posCount;
            *(s32*)(work + 0xFC) = *(s32*)(work + 0xEC) + *(s32*)(work + 0xF4) * 0xC;
            *(s32*)(work + 0xF4) += nrmCount;
        }
        *(s32*)(drawPose + 0x18) = *(s32*)(pose + 0x18);
        *(s32*)(drawPose + 0x1C) = *(s32*)(pose + 0x1C);
        animPoseMain(effectId);
    }

    *(s32*)(work + 0x30) = *(s32*)(pose + 0x98) == 0 ? 0 : pose + 0xA0;
    *(s32*)(work + 0x34) = *(s32*)(pose + 0x9C) == 0 ? 0 : pose + 0xC0;
    *(s32*)(work + 0x38) = 0;
    *(s32*)(work + 0x24) = *(s32*)(pose + 0x98);
    *(s32*)(work + 0x28) = *(s32*)(pose + 0x9C);
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0xD8) = 1;
    *(s32*)(work + 0xDC) = 1;
    *(s32*)(drawPose + 0xE8) = *(s32*)(pose + 0xE8);
    *(s32*)(drawPose + 0xEC) = *(s32*)(pose + 0xEC);
    *(s32*)(drawPose + 0xF0) = *(s32*)(pose + 0xF0);
    *(s32*)(drawPose + 0xF4) = *(s32*)(pose + 0xF4);

    PSMTXScale(paperMtx, *(f32*)(pose + 0x10C), *(f32*)(pose + 0x110), float_1_8041fb24);
    PSMTXTrans(workMtx, (double)*(f32*)(pose + 0x104), (double)*(f32*)(pose + 0x108),
               (double)float_0_8041fb28);
    PSMTXConcat(paperMtx, workMtx, paperMtx);
    PSMTXCopy(paperMtx, (void*)(work + 0x3C));
    *(s32*)(work + 0xCC) = work + 0x3C;
    *(s32*)(work + 0xD0) = 0;
    *(s32*)(work + 0xD4) = 0;

    _animPoseDrawMtx((void*)drawPose, mtx, mode, rotValue, scaleValue);
    *(s32*)(work + 0xCC) = 0;
    *(s32*)(work + 0xD0) = 0;
    *(s32*)(work + 0xD4) = 0;
    if ((*(u32*)(drawPose + 4) & 2) != 0) {
        *(s32*)(work + 0xE8) = 0;
    }
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

void materialProc(int shapeIdx) {
    extern s32 wp;
    extern s32 g_modeling_mtx;
    extern u32 vec3_802c1030[];
    extern char str_Error_animSetMateria_802c1078[];

    extern void* camGetCurPtr(void);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXInvXpose(void* a, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(u32 id);
    extern void GXSetCullMode(s32 mode);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void mapSetMaterialLight(u32 materialLightFlag, void* pPos);
    extern u8 animSetMaterial_Texture(s32 texCount, int* pTexIdRemap, void* pTexEntries, void* pAnimEntries, s32 texBindsCapacity, void* pTexBinds, void* pTexFile);
    extern void mapSetMaterialLastStageBlend(u32 flags, void* pEvtColor, void* pEvtColor2);
    extern void mapSetMaterialFog(void);
    extern void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx);
    extern int printf(const char* fmt, ...);
    extern void* smartTexObj(void* texObj, void** data);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXLoadTexMtxImm(void* mtx, u32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, u32 mtx, u32 normalize, s32 postMtx);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);

    s32 work;
    s32 pose;
    s32 poseData;
    s32 shapes;
    s32 shape;
    s32 draws;
    s32 draw;
    s32 drawCount;
    s32 drawStart;
    s32 dispMode;
    s32 curDispMode;
    s32 i;
    f32 posMtx[3][4];
    f32 nrmMtx[3][4];
    u32 tmpVec[3];
    u32 lightVec[3];

    if (shapeIdx == -1) {
        return;
    }

    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), (void*)g_modeling_mtx, posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    PSMTXInvXpose(posMtx, nrmMtx);
    GXLoadNrmMtxImm(nrmMtx, 0);
    GXSetCurrentMtx(0);

    work = wp;
    poseData = *(s32*)(work + 0x20);
    shapes = *(s32*)(poseData + 0x14C);
    draws = *(s32*)(poseData + 0x19C);
    shape = shapes + shapeIdx * 0xA8;
    drawStart = *(s32*)(shape + 0x98);
    drawCount = *(s32*)(shape + 0x9C);
    dispMode = *(s32*)(shape + 0xA0);
    curDispMode = *(s32*)(work + 0xE0);
    pose = *(s32*)(work + 0x18);

    if (!((dispMode == 0 && curDispMode == 2) ||
          (dispMode == curDispMode) ||
          (*(u8*)(pose + 0xF3) != 0xFF) ||
          (*(u8*)(pose + 0xF7) != 0xFF))) {
        return;
    }

    {
        static s32 cull_table[4] = { 0, 1, 2, 3 };
        GXSetCullMode(cull_table[*(s32*)(shape + 0xA4)]);
    }

    draw = draws + drawStart * 0x6C;
    for (i = 0; i < drawCount; i++, draw += 0x6C) {
        s32 texCount = *(s32*)(draw + 0x0);
        s32 drawMode = *(s32*)(draw + 0x8);
        s32 foundFirst;
        s32 foundAny;
        s32 j;
        void* texObj;
        void** texData;
        void* texMtx;
        s32 texBindCount;
        s32 texBinds;
        s32 texEntries;
        s32 texAnims;

        work = wp;
        pose = *(s32*)(work + 0x18);

        if (*(s32*)(pose + 0xE4) == 0) {
            u32 color0;
            u32 color1;
            void* cam4;

            tmpVec[0] = vec3_802c1030[0];
            tmpVec[1] = vec3_802c1030[1];
            tmpVec[2] = vec3_802c1030[2];
            PSMTXMultVec((void*)g_modeling_mtx, tmpVec, tmpVec);
            lightVec[0] = tmpVec[0];
            lightVec[1] = tmpVec[1];
            lightVec[2] = tmpVec[2];

            mapSetMaterialLight(*(u32*)(pose + 0xEC), lightVec);

            poseData = *(s32*)(work + 0x20);
            animSetMaterial_Texture(texCount,
                                    (int*)(draw + 0x10),
                                    (void*)*(s32*)(poseData + 0x194),
                                    (void*)*(s32*)(pose + 0x6C),
                                    *(s32*)(poseData + 0x134),
                                    (void*)*(s32*)(poseData + 0x198),
                                    (void*)(*(s32*)(work + 0x8) + (*(s32*)(*(s32*)work + (*(s32*)(pose + 0x10) * 0x10) + 0xC) * 0xC)));

            color1 = *(u32*)(pose + 0xF4);
            color0 = *(u32*)(pose + 0xF0);
            mapSetMaterialLastStageBlend(*(u32*)(pose + 0xE8), &color0, &color1);

            cam4 = camGetPtr(4);
            if (camGetCurPtr() == cam4) {
                mapSetMaterialFog();
            }
            mapSetMaterialTev(texCount, drawMode, *(u32*)(pose + 0xE8), g_modeling_mtx);
        }

        foundFirst = 0;
        foundAny = 0;
        work = wp;
        pose = *(s32*)(work + 0x18);
        if (*(u32*)(pose + 0x4) != 0) {
            poseData = *(s32*)(work + 0x20);
            texMtx = *(void**)(work + 0xCC);
            texData = *(void***)(work + 0x24);
            texObj = *(void**)(work + 0x30);
            texBinds = *(s32*)(poseData + 0x198);
            texBindCount = *(s32*)(poseData + 0x134);
            texEntries = *(s32*)(poseData + 0x194);
            texAnims = *(s32*)(pose + 0x6C);

            for (j = 0; j < texCount; j++) {
                s32 texId = *(s32*)(draw + ((texCount - j - 1) * 4) + 0x10);
                s32 bindIdx = *(s32*)(texEntries + texId * 8) + *(u8*)(texAnims + texId * 0x18);
                if (bindIdx < 0 || bindIdx >= texBindCount) {
                    bindIdx = 0;
                    printf(str_Error_animSetMateria_802c1078);
                }
                if (*(s32*)(texBinds + bindIdx * 0x40 + 8) == 1) {
                    if (texObj != 0) {
                        texObj = smartTexObj(texObj, texData);
                        GXLoadTexObj(texObj, j);
                        if (texMtx != 0) {
                            s32 mtxId = j * 3 + 0x1E;
                            GXLoadTexMtxImm(texMtx, mtxId, 1);
                            GXSetTexCoordGen2(j, 1, j + 4, mtxId, 0, 0x7D);
                        } else {
                            GXSetTexCoordGen2(j, 1, j + 4, 0x3C, 0, 0x7D);
                        }
                    }
                    foundFirst = 1;
                    foundAny = 1;
                    break;
                }
            }

            if (!foundFirst) {
                poseData = *(s32*)(work + 0x20);
                texMtx = *(void**)(work + 0xD0);
                texData = *(void***)(work + 0x28);
                texObj = *(void**)(work + 0x34);
                texBinds = *(s32*)(poseData + 0x198);
                texBindCount = *(s32*)(poseData + 0x134);
                texEntries = *(s32*)(poseData + 0x194);
                texAnims = *(s32*)(pose + 0x6C);

                for (j = 0; j < texCount; j++) {
                    s32 texId = *(s32*)(draw + ((texCount - j - 1) * 4) + 0x10);
                    s32 bindIdx = *(s32*)(texEntries + texId * 8) + *(u8*)(texAnims + texId * 0x18);
                    if (bindIdx < 0 || bindIdx >= texBindCount) {
                        bindIdx = 0;
                        printf(str_Error_animSetMateria_802c1078);
                    }
                    if (*(s32*)(texBinds + bindIdx * 0x40 + 8) == 2) {
                        if (texObj != 0) {
                            texObj = smartTexObj(texObj, texData);
                            GXLoadTexObj(texObj, j);
                            if (texMtx != 0) {
                                s32 mtxId = j * 3 + 0x1E;
                                GXLoadTexMtxImm(texMtx, mtxId, 1);
                                GXSetTexCoordGen2(j, 1, j + 4, mtxId, 0, 0x7D);
                            } else {
                                GXSetTexCoordGen2(j, 1, j + 4, 0x3C, 0, 0x7D);
                            }
                        }
                        foundAny = 1;
                        break;
                    }
                }
            }

            if (foundAny && drawMode == 0) {
                texObj = *(void**)(work + 0x38);
                if (texObj != 0) {
                    smartTexObj(texObj, *(void***)(work + 0x2C));
                    if (*(s32*)(work + 0xE0) != 3) {
                        GXSetZMode(1, 3, 1);
                        GXSetBlendMode(0, 4, 5, 0);
                        GXSetZCompLoc(0);
                        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                    }
                    if (foundFirst) {
                        if (*(s32*)(work + 0x30) != 0) {
                            GXLoadTexObj(*(void**)(work + 0x30), 0);
                            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                            GXLoadTexMtxImm(*(void**)(work + 0xCC), 0x1E, 1);
                        }
                    } else {
                        if (*(s32*)(work + 0x34) != 0) {
                            GXLoadTexObj(*(void**)(work + 0x34), 0);
                            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
                            GXLoadTexMtxImm(*(void**)(work + 0xD0), 0x1E, 1);
                        }
                    }
                    GXLoadTexObj(texObj, 1);
                    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
                    GXLoadTexMtxImm(*(void**)(work + 0xD4), 0x21, 1);
                    pose = *(s32*)(work + 0x18);
                    mapSetMaterialTev(2, 0xC, *(u32*)(pose + 0xE8), g_modeling_mtx);
                }
            }
        }

        work = wp;
        pose = *(s32*)(work + 0x18);
        if ((*(u32*)pose & 0x10) != 0 && *(s32*)(work + 0x30) != 0 && *(s32*)(work + 0xCC) != 0) {
            poseData = *(s32*)(work + 0x20);
            texMtx = *(void**)(work + 0xCC);
            texData = *(void***)(work + 0x24);
            texObj = *(void**)(work + 0x30);
            texBinds = *(s32*)(poseData + 0x198);
            texBindCount = *(s32*)(poseData + 0x134);
            texEntries = *(s32*)(poseData + 0x194);
            texAnims = *(s32*)(pose + 0x6C);
            for (j = 0; j < texCount; j++) {
                s32 texId = *(s32*)(draw + ((texCount - j - 1) * 4) + 0x10);
                s32 bindIdx = *(s32*)(texEntries + texId * 8) + *(u8*)(texAnims + texId * 0x18);
                if (bindIdx < 0 || bindIdx >= texBindCount) {
                    bindIdx = 0;
                    printf(str_Error_animSetMateria_802c1078);
                }
                if (*(s32*)(texBinds + bindIdx * 0x40 + 8) == 1) {
                    if (texObj != 0) {
                        texObj = smartTexObj(texObj, texData);
                        GXLoadTexObj(texObj, j);
                        if (texMtx != 0) {
                            s32 mtxId = j * 3 + 0x1E;
                            GXLoadTexMtxImm(texMtx, mtxId, 1);
                            GXSetTexCoordGen2(j, 1, j + 4, mtxId, 0, 0x7D);
                        } else {
                            GXSetTexCoordGen2(j, 1, j + 4, 0x3C, 0, 0x7D);
                        }
                    }
                    break;
                }
            }
        }
    }
}

s32 animPoseEntry(void* name, s32 mode) {
    typedef f32 Mtx[3][4];
    extern s32 wp;
    extern void fileSetCurrentArchiveType(s32 type);
    extern s32 strcmp(const char*, const char*);
    extern s32 sprintf(char*, const char*, ...);
    extern void* fileAllocf(s32 heap, const char* fmt, ...);
    extern void* fileAlloc(const char* name, s32 heap);
    extern void fileFree(void*);
    extern void animPose_AllocBuffer(void*);
    extern void PSMTXIdentity(Mtx m);
    extern void* gpGlobals;
    extern char str_PCTs_PCTs_8041fb34[];
    extern char str_a_8041fb3c[];
    extern char str_PCTs_PCTs_802c1068[];
    extern char str_tst_vivi_802c11a8[];
    extern char str_c_vivian_802c11b4[];
    extern char str_c_maririn_802c11c0[];
    extern char str_c_majyorin_802c11cc[];
    extern u32 dat_8041fb0c;
    extern u32 dat_8041fb10;
    extern s32 vivihimoTailGroupNo;
    char path[256];
    s32 work;
    s32 poseIdx;
    s32 pose;
    s32 fileIdx;
    s32 animFile;
    s32 texIdx;
    s32 texFile;
    s32 i;
    void* file;
    u64 time;

    if (mode == 1) {
        fileSetCurrentArchiveType(2);
    } else if (mode >= 0 && mode < 1) {
        fileSetCurrentArchiveType(1);
    } else if (mode < 3) {
        fileSetCurrentArchiveType(0);
    }

    work = wp;
    pose = 0;
    poseIdx = -1;
    for (i = 0; i < *(s32*)(work + 0x14); i++) {
        pose = *(s32*)(work + 0x10) + i * 0x170;
        if (*(s32*)pose == 0) {
            poseIdx = i;
            break;
        }
    }
    if (poseIdx < 0) {
        pose = 0;
    }

    *(u32*)pose |= 1;
    *(s32*)(pose + 4) = 0;
    *(s32*)(pose + 8) = 0;

    fileIdx = -1;
    animFile = 0;
    for (i = 0; i < *(s32*)(work + 4); i++) {
        animFile = *(s32*)work + i * 0x10;
        if (*(s32*)animFile != 0 && strcmp((const char*)(**(s32***)(*(s32*)(animFile + 8) + 0xA0) + 4), name) == 0) {
            (*(s32*)(animFile + 4))++;
            fileIdx = i;
            goto file_done;
        }
    }

    fileIdx = -1;
    animFile = 0;
    for (i = 0; i < *(s32*)(work + 4); i++) {
        animFile = *(s32*)work + i * 0x10;
        if (*(s32*)animFile == 0) {
            fileIdx = i;
            break;
        }
    }

    *(s32*)animFile = 1;
    (*(s32*)(animFile + 4))++;
    file = fileAllocf(5, str_PCTs_PCTs_8041fb34, str_a_8041fb3c, name);
    *(void**)(animFile + 8) = file;
    if (file == 0) {
        fileIdx = -2;
        *(s32*)animFile = 0;
        (*(s32*)(animFile + 4))--;
    } else if (**(s32***)((s32)file + 0xA0) == 0) {
        fileFree(file);
        fileIdx = -2;
        *(s32*)animFile = 0;
        (*(s32*)(animFile + 4))--;
    } else {
        sprintf(path, str_PCTs_PCTs_802c1068, str_a_8041fb3c, **(s32***)((s32)file + 0xA0) + 0x44);
        texIdx = -1;
        texFile = 0;
        for (i = 0; i < *(s32*)(work + 0xC); i++) {
            texFile = *(s32*)(work + 8) + i * 0xC;
            if (*(s32*)texFile != 0 && *(s32*)(texFile + 8) != 0 &&
                strcmp((const char*)(*(s32*)(texFile + 8) + 0x20), path) == 0) {
                (*(s32*)(texFile + 4))++;
                texIdx = i;
                goto tex_done;
            }
        }
        texIdx = -1;
        texFile = 0;
        for (i = 0; i < *(s32*)(work + 0xC); i++) {
            texFile = *(s32*)(work + 8) + i * 0xC;
            if (*(s32*)texFile == 0) {
                texIdx = i;
                break;
            }
        }
        *(s32*)texFile = 1;
        (*(s32*)(texFile + 4))++;
        *(void**)(texFile + 8) = fileAlloc(path, 4);
tex_done:
        *(s32*)(animFile + 0xC) = texIdx;
    }

file_done:
    *(s32*)(pose + 0x10) = fileIdx;
    if (*(s32*)(pose + 0x10) == -2) {
        poseIdx = -2;
        *(s32*)pose = 0;
    } else {
        *(s32*)(pose + 0x14) = 0;
        if (mode == 0) {
            time = *(u64*)((s32)gpGlobals + 0x18) / (*(u32*)0x800000F8 / 4000);
        } else {
            time = *(u64*)((s32)gpGlobals + 0x20) / (*(u32*)0x800000F8 / 4000);
        }
        *(u64*)(pose + 0x18) = time;
        *(s32*)(pose + 0x20) = -1;
        *(f32*)(pose + 0x24) = 0.0f;
        *(f32*)(pose + 0x80) = 0.0f;
        *(f32*)(pose + 0x28) = 0.0f;
        *(f32*)(pose + 0x70) = 0.0f;
        *(f32*)(pose + 0x74) = 0.0f;
        *(f32*)(pose + 0x7C) = 0.0f;
        *(s32*)(pose + 0x0C) = mode;
        *(s32*)(pose + 0x90) = -1;
        animPose_AllocBuffer((void*)pose);
        *(s32*)(pose + 0xE4) = 0;
        *(s32*)(pose + 0xE8) = 0;
        *(s32*)(pose + 0xEC) = 0;
        *(s32*)(pose + 0xF4) = 0;
        *(u32*)(pose + 0xF0) = dat_8041fb0c;
        *(u32*)(pose + 0xF8) = dat_8041fb10;
        *(f32*)(pose + 0xFC) = 0.0f;
        *(f32*)(pose + 0x100) = 0.0f;
        *(f32*)(pose + 0x104) = 0.0f;
        *(f32*)(pose + 0x108) = 0.0f;
        PSMTXIdentity((void*)(pose + 0x124));
        if (strcmp(name, str_tst_vivi_802c11a8) == 0) {
            *(s32*)(pose + 0x14C) = 1;
            *(void**)(pose + 0x150) = &vivihimoTailGroupNo;
        } else if (strcmp(name, str_c_vivian_802c11b4) == 0) {
            *(s32*)(pose + 0x14C) = 1;
            *(void**)(pose + 0x150) = ((s32*)&vivihimoTailGroupNo) + 1;
        } else if (strcmp(name, str_c_maririn_802c11c0) == 0) {
            *(s32*)(pose + 0x14C) = 2;
            *(void**)(pose + 0x150) = ((s32*)&vivihimoTailGroupNo) + 2;
        } else if (strcmp(name, str_c_majyorin_802c11cc) == 0) {
            *(s32*)(pose + 0x14C) = 3;
            *(void**)(pose + 0x150) = ((s32*)&vivihimoTailGroupNo) + 3;
        } else {
            *(s32*)(pose + 0x14C) = 0;
            *(s32*)(pose + 0x150) = 0;
        }
        *(s32*)(pose + 0x154) = 0;
        *(s32*)(pose + 0x158) = 0;
    }
    return poseIdx;
}

void dispProc(int parentGroupIdx, int groupIdx, int animFrame0, int animFrame1, float blend) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern s32 wp;
    extern void PSMTXCopy(void* src, void* dst);
    extern void pushGXModelMtx_JointNode__(void* pJoint, double sx, double sy, double sz);
    extern u8 pushGXModelMtx_TransformNode__(void* pNode);
    extern void renderProc(int shapeIdx, int animFrame0);
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
                    sz = 0.0f;
                    sy = sz;
                    sx = sz;
                } else {
                    parentGroup = groups + parentGroupIdx * 0x58;
                    if (*(s32*)(parentGroup + 0x54) == 0) {
                        sz = 0.0f;
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
                *(Vec3*)(*(s32*)(pose + 0x15C) + groupIdx * 0xC) = temp;
            }
            work = wp;
            pose = *(s32*)(work + 0x18);
            flags = *(s32*)pose;
            if ((flags & 0x80) != 0) {
                PSMTXCopy(g_modeling_mtx, (void*)(*(s32*)(pose + 0x164) + groupIdx * 0x30));
            }
            work = wp;
            pose = *(s32*)(work + 0x18);
            flags = *(s32*)pose;
            if ((flags & 0x20) == 0 || groupIdx != **(s32**)(pose + 0x120)) {
                renderProc(*(s32*)(group + 0x44), animFrame0);
                dispProc(groupIdx, *(s32*)(group + 0x48), animFrame0, animFrame1, blend);
            }
            mtxTop = g_modeling_mtx_lv - 1;
            g_modeling_mtx_lv = mtxTop;
            g_modeling_mtx = g_modeling_mtx_stack + mtxTop * 0x30;
        }
        dispProc(groupIdx, *(s32*)(group + 0x40), animFrame0, animFrame1, blend);
    }
}

int testAlloc(u32 size) {
    extern s32 wp;
    extern void* __memAlloc(s32, u32);
    extern void initTestHeap(void);
    extern void animPose_AllocBuffer(void*);
    extern void animPoseRefresh(void);

    void* ret;
    s32 i;
    s32 offset;
    s32 pose;
    s32 file;
    s32 data;

    ret = *(void**)(wp + 0x108);
    if ((size & 0x1F) != 0) {
        size += 0x20 - (size & 0x1F);
    }
    *(s32*)(wp + 0x108) = *(s32*)(wp + 0x108) + size;

    if (*(s32*)(wp + 0x104) + 0x180000 <= *(s32*)(wp + 0x108)) {
        if (*(s32*)(wp + 0x104) == 0) {
            *(void**)(wp + 0x104) = __memAlloc(0, 0x180000);
        }
        *(s32*)(wp + 0x108) = *(s32*)(wp + 0x104);

        i = 0;
        offset = 0;
        while (i < *(s32*)(wp + 0x14)) {
            pose = *(s32*)(wp + 0x10) + offset;
            if (*(s32*)pose != 0 && (*(s32*)(wp + 0x10C) == 0 || *(s32*)(pose + 0xC) != 0) &&
                (*(s32*)(wp + 0x10C) != 0 || *(s32*)(pose + 0xC) != 1)) {
                file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
                data = *(s32*)(file + 8);
                data = *(s32*)(data + 0xA0);
                data = *(s32*)data;
                *(s32*)(pose + 0x48) = testAlloc(*(s32*)(data + 0xF0) * 0xC);
                *(s32*)(pose + 0x4C) = testAlloc(*(s32*)(data + 0xF0) * 0xC);
                *(s32*)(pose + 0x50) = testAlloc(*(s32*)(data + 0xF8) * 0xC);
                *(s32*)(pose + 0x54) = testAlloc(*(s32*)(data + 0xF8) * 0xC);
                *(s32*)(pose + 0x58) = testAlloc(*(s32*)(data + 0x13C));
                *(s32*)(pose + 0x5C) = *(s32*)(pose + 0x58);
                *(s32*)(pose + 0x60) = testAlloc(*(s32*)(data + 0x140) << 2);
                *(s32*)(pose + 0x64) = testAlloc(*(s32*)(data + 0x140) << 2);
                *(s32*)(pose + 0x68) = testAlloc(*(s32*)(data + 0x12C) * 0x18);
                *(s32*)(pose + 0x6C) = *(s32*)(pose + 0x68);
                *(s32*)(pose + 0x3C) = -1;
            }
            offset += 0x170;
            i++;
        }

        ret = *(void**)(wp + 0x108);
        if ((size & 0x1F) != 0) {
            size += 0x20 - (size & 0x1F);
        }
        *(s32*)(wp + 0x108) = *(s32*)(wp + 0x108) + size;
        if (*(s32*)(wp + 0x104) + 0x180000 <= *(s32*)(wp + 0x108)) {
            initTestHeap();
            i = 0;
            offset = 0;
            while (i < *(s32*)(wp + 0x14)) {
                pose = *(s32*)(wp + 0x10) + offset;
                if (*(s32*)pose != 0 && (*(s32*)(wp + 0x10C) == 0 || *(s32*)(pose + 0xC) != 0) &&
                    (*(s32*)(wp + 0x10C) != 0 || *(s32*)(pose + 0xC) != 1)) {
                    animPose_AllocBuffer((void*)pose);
                    *(s32*)(pose + 0x3C) = -1;
                }
                offset += 0x170;
                i++;
            }
            ret = *(void**)(wp + 0x108);
            if ((size & 0x1F) != 0) {
                size += 0x20 - (size & 0x1F);
            }
            *(s32*)(wp + 0x108) = *(s32*)(wp + 0x108) + size;
            if (*(s32*)(wp + 0x104) + 0x180000 <= *(s32*)(wp + 0x108)) {
                animPoseRefresh();
                ret = (void*)testAlloc(size);
            }
        }
    }
    return (s32)ret;
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
    extern s32 strcmp(const char*, const char*);
    extern s32 animPoseEntry(char*, u32);
    extern s32 wp;
    s32 index = 0;
    s32 offset = 0;
    s32 pose;
    s32 file;
    s32 data;

    while (index < *(s32*)(wp + 0x14)) {
        pose = *(s32*)(wp + 0x10) + offset;
        if ((*(u32*)pose & 1) != 0 &&
            (*(u32*)(pose + 4) & 1) != 0 &&
            (*(u32*)(pose + 4) & 2) != 0 &&
            *(s32*)(pose + 0xC) == flag) {
            file = *(s32*)(*(s32*)wp + (*(s32*)(pose + 0x10) << 4) + 8);
            data = **(s32**)(file + 0xA0);
            if (strcmp((char*)(data + 4), (char*)name) == 0) break;
        }
        offset += 0x170;
        index++;
    }
    if (index == *(s32*)(wp + 0x14)) index = -1;
    if (index < 0) {
        index = animPoseEntry((char*)name, flag);
        if (index != -2) {
            pose = *(s32*)(wp + 0x10) + index * 0x170;
            *(u32*)(pose + 4) = 3;
            *(s32*)(pose + 8) = 0;
            *(u32*)pose |= 1;
        }
    } else {
        pose = *(s32*)(wp + 0x10) + index * 0x170;
        *(s32*)(pose + 8) += 1;
    }
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
    extern s32 wp;
    extern s32 testAlloc(u32 size);
    extern void initTestHeap(void);
    extern void animPoseRefresh(void);
    s32 pose = (s32)pPose;
    s32 file = *(s32*)wp + (*(s32*)(pose + 0x10) << 4);
    s32 data = **(s32**)(*(s32*)(file + 8) + 0xA0);
    s32 ret;
    s32 otherPose;
    s32 i;
    u32 size;

#define ALLOC_INLINE(destination, requestedSize) do { \
    size = (requestedSize); \
    ret = *(s32*)(wp + 0x108); \
    if ((size & 0x1F) != 0) size += 0x20 - (size & 0x1F); \
    *(s32*)(wp + 0x108) += size; \
    if (*(s32*)(wp + 0x104) + 0x180000 <= *(s32*)(wp + 0x108)) { \
        initTestHeap(); \
        otherPose = *(s32*)(wp + 0x10); \
        for (i = 0; i < *(s32*)(wp + 0x14); i++, otherPose += 0x170) { \
            if (*(s32*)otherPose != 0 && \
                (*(s32*)(wp + 0x10C) == 0 || *(s32*)(otherPose + 0xC) != 0) && \
                (*(s32*)(wp + 0x10C) != 0 || *(s32*)(otherPose + 0xC) != 1)) { \
                animPose_AllocBuffer((void*)otherPose); \
                *(s32*)(otherPose + 0x3C) = -1; \
            } \
        } \
        ret = *(s32*)(wp + 0x108); \
        if ((size & 0x1F) != 0) size += 0x20 - (size & 0x1F); \
        *(s32*)(wp + 0x108) += size; \
        if (*(s32*)(wp + 0x104) + 0x180000 <= *(s32*)(wp + 0x108)) { \
            animPoseRefresh(); \
            ret = testAlloc(size); \
        } \
    } \
    *(s32*)(pose + (destination)) = ret; \
} while (0)

    ALLOC_INLINE(0x48, *(u32*)(data + 0xF0) * 0xC);
    ALLOC_INLINE(0x4C, *(u32*)(data + 0xF0) * 0xC);
    ALLOC_INLINE(0x50, *(u32*)(data + 0xF8) * 0xC);
#undef ALLOC_INLINE

    *(s32*)(pose + 0x54) = testAlloc(*(s32*)(data + 0xF8) * 0xC);
    *(s32*)(pose + 0x58) = testAlloc(*(s32*)(data + 0x13C));
    *(s32*)(pose + 0x5C) = *(s32*)(pose + 0x58);
    *(s32*)(pose + 0x60) = testAlloc(*(s32*)(data + 0x140) << 2);
    *(s32*)(pose + 0x64) = testAlloc(*(s32*)(data + 0x140) << 2);
    *(s32*)(pose + 0x68) = testAlloc(*(s32*)(data + 0x12C) * 0x18);
    *(s32*)(pose + 0x6C) = *(s32*)(pose + 0x68);
    *(s32*)(pose + 0x3C) = -1;
}

void _animPoseDrawMtx(void* pPose, void* pMtx, int dispMode, double rotValue, double scaleValue) {
    typedef f32 Mtx[3][4];
    typedef void (*GXFunc)(s32 mode);
    extern s32 wp;
    extern s32 mapTestXLU(u32 flags, void* color0, void* color1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void PSMTXRotRad(void* m, double rad, char axis);
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXCopy(void* src, void* dst);
    extern u8 animPoseMain(s32 poseId);
    extern void DCFlushRange(void* ptr, u32 size);
    extern void GXInvalidateVtxCache(void);
    extern void dispProc(s32 parent, s32 group, s32 frame0, s32 frame1, f32 blend);
    extern void* g_modeling_mtx;
    extern s32 g_modeling_mtx_lv;
    extern u8 g_modeling_mtx_stack[];
    extern f32 float_0_8041fb28;
    extern f32 float_deg2rad_8041fbb4;
    s32 work;
    s32 pose;
    s32 poses;
    s32 file;
    s32 data;
    s32 animTable;
    s32 animData;
    s32 poseId;
    s32 frame0;
    s32 frame1;
    f32 blend;
    u32 color0;
    u32 color1;
    void* drawMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx tempMtx;

    work = wp;
    pose = (s32)pPose;
    poses = *(s32*)(work + 0x10);
    frame0 = *(s32*)(pose + 0x28);
    frame1 = *(s32*)(pose + 0x2C);
    blend = *(f32*)(pose + 0x30);

    file = *(s32*)work + (*(s32*)(pose + 0x10) << 4);
    file = *(s32*)(file + 8);
    data = *(s32*)(*(s32*)(file + 0xA0));
    animTable = *(s32*)(data + 0x1AC);
    animData = *(s32*)(animTable + (*(s32*)(pose + 0x14) << 6) + 0x3C);
    if (animData == 0) {
        return;
    }
    if ((*(s32*)(work + 0x10C) != 0) && (*(s32*)(pose + 0xC) == 0)) {
        return;
    }
    if ((*(s32*)(work + 0x10C) == 0) && (*(s32*)(pose + 0xC) == 1)) {
        return;
    }
    if (((dispMode != 2) || ((*(u32*)(data + 0xC4) & 1) == 0)) &&
        ((*(u32*)(data + 0xC4) & (1 << dispMode)) == 0)) {
        return;
    }

    color0 = *(u32*)(pose + 0xF0);
    color1 = *(u32*)(pose + 0xF4);
    if (mapTestXLU(*(u32*)(pose + 0xE8), &color0, &color1) != 0) {
        dispMode = 3;
    }

    if (*(GXFunc*)(pose + 0xE0) != 0) {
        (*(GXFunc*)(pose + 0xE0))(dispMode);
    } else {
        switch (dispMode) {
            case 0:
            case 2:
                GXSetZMode(1, 3, 1);
                GXSetBlendMode(0, 4, 5, 0);
                GXSetZCompLoc(0);
                GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                break;
            case 1:
                GXSetZMode(1, 3, 1);
                GXSetBlendMode(0, 4, 5, 0);
                GXSetZCompLoc(1);
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                break;
            case 3:
                GXSetZMode(1, 3, 0);
                GXSetBlendMode(1, 4, 5, 0);
                GXSetZCompLoc(1);
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                break;
        }
        *(s32*)(work + 0xE0) = dispMode;
    }

    drawMtx = pMtx;
    if (((*(u32*)pose & 8) == 0) && (rotValue != (double)float_0_8041fb28)) {
        PSMTXRotRad(rotMtx, (double)(float_deg2rad_8041fbb4 * (f32)rotValue), 'y');
        PSMTXConcat(pMtx, rotMtx, tempMtx);
        drawMtx = tempMtx;
    } else if ((*(u32*)pose & 8) != 0) {
        *(f32*)(pose + 0x70) = (f32)rotValue + 90.0f;
        if (*(f32*)(pose + 0x74) != float_0_8041fb28) {
            PSMTXRotRad(rotMtx, (double)(float_deg2rad_8041fbb4 * *(f32*)(pose + 0x74)), 'y');
            PSMTXConcat(pMtx, rotMtx, tempMtx);
            drawMtx = tempMtx;
        }
    }
    if (scaleValue != (double)float_0_8041fb28) {
        PSMTXScale(scaleMtx, (f32)scaleValue, (f32)scaleValue, (f32)scaleValue);
        PSMTXConcat(drawMtx, scaleMtx, tempMtx);
        drawMtx = tempMtx;
    }

    if (*(s32*)(pose + 0x3C) == -1) {
        poseId = (pose - poses) / 0x170;
        animPoseMain(poseId);
    }

    g_modeling_mtx_lv = 0;
    g_modeling_mtx = g_modeling_mtx_stack;
    PSMTXCopy(drawMtx, g_modeling_mtx_stack);
    *(s32*)(work + 0x18) = pose;
    *(s32*)(work + 0x1C) = animData;
    *(s32*)(work + 0x20) = data;

    if (*(s32*)(work + 0xE8) == 0) {
        DCFlushRange(*(void**)(pose + 0x4C), *(u32*)(data + 0xF0) * 0xC);
        DCFlushRange(*(void**)(pose + 0x54), *(u32*)(data + 0xF8) * 0xC);
    } else {
        DCFlushRange(*(void**)(work + 0xF8), *(u32*)(data + 0xF0) * 0xC);
        DCFlushRange(*(void**)(work + 0xFC), *(u32*)(data + 0xF8) * 0xC);
    }
    GXInvalidateVtxCache();
    dispProc(-1, *(s32*)(data + 0x144) - 1, frame0, frame1, blend);
}

u8 animSetMaterial_Texture(s32 texCount, int* pTexIdRemap, void* pTexEntries, void* pAnimEntries, s32 texBindsCapacity, void* pTexBinds, void* pTexFile) {
    typedef f32 Mtx[3][4];
    extern s32 wp;
    extern s32 printf(const char*, ...);
    extern void* TEXGet(s32 tpl, s32 id);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, u32 format, u32 wrapS, u32 wrapT, u8 mipmap);
    extern void GXInitTexObjCI(void* obj, u32 image, u32 width, u32 height, u32 format, u32 wrapS, u32 wrapT, u8 mipmap, u32 tlut);
    extern void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, double minLod, double maxLod, double bias, u8 biasClamp, u8 edgeLod, u32 maxAniso);
    extern void GXInitTlutObj(void* obj, u32 data, u32 format, u16 entries);
    extern void GXLoadTlut(void* obj, s32 id);
    extern void GXLoadTexObj(void* obj, s32 id);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, u32 mtx, u32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(Mtx m, s32 id, s32 type);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, double rad, char axis);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
    extern f32 float_deg2rad_8041fbb4;
    extern char str_Error_dispSetTevMode_802c10c4[];
    u8 texObj[32];
    u32 tlutObj[4];
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx tmp;
    Mtx final;
    Mtx* pMtx;
    s32 i;
    s32 idx;
    s32 texId;
    s32 bindIdx;
    s32 bind;
    s32 tex;
    s32 texHeader;
    s32 tlut;
    u32 wrapFlags;
    u32 wrapS;
    u32 wrapT;
    u8 minLod;
    u8 maxLod;
    u8 hasMipmap;
    u8 flags;
    f32 rotate;
    f32 transS;
    f32 transT;
    f32 scaleS;
    f32 scaleT;

    for (i = 0; i < texCount; i++) {
        texId = pTexIdRemap[(texCount - i) - 1];
        bindIdx = *(s32*)((s32)pTexEntries + texId * 8) + *(u8*)((s32)pAnimEntries + texId * 0x18);
        if (bindIdx < 0 || bindIdx >= texBindsCapacity) {
            bindIdx = 0;
            printf(str_Error_dispSetTevMode_802c10c4,
                   (const char*)(**(s32***)(*(s32*)(*(s32*)(wp + 0x18) + 0x10) * 0x10 + *(s32*)wp + 8 + 0x98) + 4),
                   *(s32*)(**(s32***)(*(s32*)(*(s32*)(wp + 0x18) + 0x10) * 0x10 + *(s32*)wp + 8 + 0x98) + 0x1AC) + *(s32*)(*(s32*)(wp + 0x18) + 0x14) * 0x40);
        }
        wrapFlags = *(u32*)((s32)pTexEntries + texId * 8 + 4);
        bind = (s32)pTexBinds + bindIdx * 0x40;
        texId = *(s32*)(bind + 4);
        if (*(s32*)(bind + 8) == 0) {
            tex = (s32)TEXGet(**(s32**)(*(s32*)((s32)pTexFile + 8) + 0xA0), texId);
            texHeader = *(s32*)tex;
            tlut = *(s32*)(tex + 4);
            wrapS = *(u32*)(texHeader + 0xC);
            wrapT = *(u32*)(texHeader + 0x10);
            if ((s32)wrapFlags >= 0) {
                wrapS = (wrapFlags & 4) ? 2 : (wrapFlags & 1);
                wrapT = (wrapFlags & 8) ? 2 : ((wrapFlags >> 1) & 1);
            }
            minLod = *(u8*)(texHeader + 0x21);
            maxLod = *(u8*)(texHeader + 0x22);
            hasMipmap = (u8)(minLod != maxLod);
            if (tlut == 0) {
                GXInitTexObj(texObj, *(void**)(texHeader + 8), *(u16*)(texHeader + 2), *(u16*)texHeader,
                             *(u32*)(texHeader + 4), wrapS, wrapT, hasMipmap);
                GXInitTexObjLOD(texObj, *(s32*)(texHeader + 0x14), *(s32*)(texHeader + 0x18),
                                (double)(f32)minLod, (double)(f32)maxLod, (double)*(f32*)(texHeader + 0x1C),
                                0, *(u8*)(texHeader + 0x20), 0);
                GXLoadTexObj(texObj, i);
            } else {
                GXInitTlutObj(tlutObj, *(u32*)(tlut + 8), *(u32*)(tlut + 4), *(u16*)tlut);
                GXInitTexObjCI(texObj, (u32)*(void**)(texHeader + 8), *(u16*)(texHeader + 2), *(u16*)texHeader,
                               *(u32*)(texHeader + 4), wrapS, wrapT, hasMipmap, 0);
                GXInitTexObjLOD(texObj, *(s32*)(texHeader + 0x14), *(s32*)(texHeader + 0x18),
                                (double)(f32)minLod, (double)(f32)maxLod, (double)*(f32*)(texHeader + 0x1C),
                                0, *(u8*)(texHeader + 0x20), 0);
                GXLoadTlut(tlutObj, 0);
                GXLoadTexObj(texObj, i);
            }
        }
    }

    idx = 0;
    for (i = 0; i < texCount; i++, idx += 3) {
        texId = pTexIdRemap[(texCount - i) - 1];
        transS = *(f32*)((s32)pAnimEntries + texId * 0x18 + 4);
        transT = *(f32*)((s32)pAnimEntries + texId * 0x18 + 8);
        scaleS = *(f32*)((s32)pAnimEntries + texId * 0x18 + 0xC);
        scaleT = *(f32*)((s32)pAnimEntries + texId * 0x18 + 0x10);
        rotate = *(f32*)((s32)pAnimEntries + texId * 0x18 + 0x14);
        flags = 0;
        if (rotate != 0.0f) {
            PSMTXTrans(tmp, -0.5, -0.5, 0.0);
            PSMTXRotRad(rot, (double)(float_deg2rad_8041fbb4 * -rotate), 'z');
            PSMTXConcat(tmp, rot, final);
            PSMTXTrans(tmp, 0.5, 0.5, 0.0);
            PSMTXConcat(final, tmp, rot);
            flags |= 1;
        }
        if (transS != 0.0f || transT != 0.0f || scaleT != 1.0f) {
            PSMTXTrans(final, (double)transS, (double)(-transT - scaleT), 0.0);
            flags |= 2;
        }
        if (scaleS != 1.0f || scaleT != 1.0f) {
            PSMTXScale(scale, scaleS, scaleT, 1.0f);
            flags |= 4;
        }
        switch (flags) {
            case 0:
                GXSetTexCoordGen2(i, 1, i + 4, 0x3C, 0, 0x7D);
                continue;
            case 1:
                pMtx = &rot;
                break;
            case 2:
                pMtx = &final;
                break;
            case 3:
                PSMTXConcat(rot, final, tmp);
                pMtx = &tmp;
                break;
            case 4:
                pMtx = &scale;
                break;
            case 5:
                PSMTXConcat(rot, scale, tmp);
                pMtx = &tmp;
                break;
            case 6:
                PSMTXConcat(final, scale, tmp);
                pMtx = &tmp;
                break;
            default:
                PSMTXConcat(rot, final, tmp);
                PSMTXConcat(tmp, scale, final);
                pMtx = &final;
                break;
        }
        GXSetTexCoordGen2(i, 1, i + 4, idx + 0x1E, 0, 0x7D);
        GXLoadTexMtxImm(*pMtx, idx + 0x1E, 1);
    }
    return 0;
}

void pushGXModelMtx_JointNode__(void* pJoint, double parentScaleX, double parentScaleY, double parentScaleZ) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXIdentity(Mtx m);
    extern void PSMTXRotRad(Mtx m, double rad, char axis);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
    extern void* g_modeling_mtx;
    extern s32 g_modeling_mtx_lv;
    extern u8 g_modeling_mtx_stack[];
    extern f32 float_deg2rad_8041fbb4;
    Mtx trans;
    Mtx parentScale;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    Mtx* child;
    u32 flags;
    f32 tx;
    f32 ty;
    f32 tz;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 rx1;
    f32 ry1;
    f32 rz1;
    f32 rx2;
    f32 ry2;
    f32 rz2;

    tx = *(f32*)((s32)pJoint + 0x00);
    ty = *(f32*)((s32)pJoint + 0x04);
    tz = *(f32*)((s32)pJoint + 0x08);
    sx = *(f32*)((s32)pJoint + 0x0C);
    sy = *(f32*)((s32)pJoint + 0x10);
    sz = *(f32*)((s32)pJoint + 0x14);
    rx1 = *(f32*)((s32)pJoint + 0x18);
    ry1 = *(f32*)((s32)pJoint + 0x1C);
    rz1 = *(f32*)((s32)pJoint + 0x20);
    rx2 = *(f32*)((s32)pJoint + 0x24);
    ry2 = *(f32*)((s32)pJoint + 0x28);
    rz2 = *(f32*)((s32)pJoint + 0x2C);
    flags = 0;

    if (tx != 0.0f || ty != 0.0f || tz != 0.0f) {
        PSMTXTrans(trans, (double)tx, (double)ty, (double)tz);
        flags |= 1;
    }
    if (parentScaleX != 1.0 || parentScaleY != 1.0 || parentScaleZ != 1.0) {
        if (parentScaleX == 0.0 || parentScaleY == 0.0 || parentScaleZ == 0.0) {
            PSMTXScale(parentScale, 0.0f, 0.0f, 0.0f);
        } else {
            PSMTXScale(parentScale, (f32)(1.0 / parentScaleX), (f32)(1.0 / parentScaleY), (f32)(1.0 / parentScaleZ));
        }
        flags |= 2;
    }
    if (sx != 1.0f || sy != 1.0f || sz != 1.0f) {
        PSMTXScale(scale, sx, sy, sz);
        flags |= 4;
    }

    PSMTXIdentity(rot);
    if (rz2 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rz2), 'z');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (ry2 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * ry2), 'y');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (rx2 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rx2), 'x');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (rz1 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rz1), 'z');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (ry1 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * ry1), 'y');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (rx1 != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rx1), 'x');
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }

    g_modeling_mtx_lv++;
    g_modeling_mtx = g_modeling_mtx_stack + g_modeling_mtx_lv * 0x30;
    switch (flags) {
        case 0:
            PSMTXIdentity(tmp);
            child = &tmp;
            break;
        case 1:
            child = &trans;
            break;
        case 2:
            child = &parentScale;
            break;
        case 3:
            PSMTXConcat(trans, parentScale, tmp);
            child = &tmp;
            break;
        case 4:
            child = &scale;
            break;
        case 5:
            PSMTXConcat(trans, scale, tmp);
            child = &tmp;
            break;
        case 6:
            PSMTXConcat(parentScale, scale, tmp);
            child = &tmp;
            break;
        case 7:
            PSMTXConcat(trans, parentScale, tmp);
            PSMTXConcat(tmp, scale, trans);
            child = &trans;
            break;
        case 8:
            child = &rot;
            break;
        case 9:
            PSMTXConcat(trans, rot, tmp);
            child = &tmp;
            break;
        case 10:
            PSMTXConcat(parentScale, rot, tmp);
            child = &tmp;
            break;
        case 11:
            PSMTXConcat(trans, parentScale, tmp);
            PSMTXConcat(tmp, rot, trans);
            child = &trans;
            break;
        case 12:
            PSMTXConcat(rot, scale, tmp);
            child = &tmp;
            break;
        case 13:
            PSMTXConcat(trans, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            child = &trans;
            break;
        case 14:
            PSMTXConcat(parentScale, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            child = &trans;
            break;
        default:
            PSMTXConcat(trans, parentScale, tmp);
            PSMTXConcat(tmp, rot, trans);
            PSMTXConcat(trans, scale, tmp);
            child = &tmp;
            break;
    }
    PSMTXConcat((void*)(g_modeling_mtx_stack + (g_modeling_mtx_lv - 1) * 0x30), *child, g_modeling_mtx);
}

u8 pushGXModelMtx_TransformNode__(void* pNode) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXIdentity(Mtx m);
    extern void PSMTXRotRad(Mtx m, double rad, char axis);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
    extern void* g_modeling_mtx;
    extern s32 g_modeling_mtx_lv;
    extern u8 g_modeling_mtx_stack[];
    extern f32 float_deg2rad_8041fbb4;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    Mtx centerA;
    Mtx centerB;
    Mtx centerC;
    Mtx* child;
    u32 flags;
    f32 tx;
    f32 ty;
    f32 tz;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 rcx;
    f32 rcy;
    f32 rcz;
    f32 scx;
    f32 scy;
    f32 scz;
    f32 rtx;
    f32 rty;
    f32 rtz;
    f32 stx;
    f32 sty;
    f32 stz;

    tx = *(f32*)((s32)pNode + 0x00);
    ty = *(f32*)((s32)pNode + 0x04);
    tz = *(f32*)((s32)pNode + 0x08);
    sx = *(f32*)((s32)pNode + 0x0C);
    sy = *(f32*)((s32)pNode + 0x10);
    sz = *(f32*)((s32)pNode + 0x14);
    rx = *(f32*)((s32)pNode + 0x18);
    ry = *(f32*)((s32)pNode + 0x1C);
    rz = *(f32*)((s32)pNode + 0x20);
    rcx = *(f32*)((s32)pNode + 0x30);
    rcy = *(f32*)((s32)pNode + 0x34);
    rcz = *(f32*)((s32)pNode + 0x38);
    scx = *(f32*)((s32)pNode + 0x3C);
    scy = *(f32*)((s32)pNode + 0x40);
    scz = *(f32*)((s32)pNode + 0x44);
    rtx = *(f32*)((s32)pNode + 0x48);
    rty = *(f32*)((s32)pNode + 0x4C);
    rtz = *(f32*)((s32)pNode + 0x50);
    stx = *(f32*)((s32)pNode + 0x54);
    sty = *(f32*)((s32)pNode + 0x58);
    stz = *(f32*)((s32)pNode + 0x5C);
    flags = 0;

    if (tx != 0.0f || ty != 0.0f || tz != 0.0f) {
        PSMTXTrans(trans, (double)tx, (double)ty, (double)tz);
        flags |= 1;
    }
    if (sx != 1.0f || sy != 1.0f || sz != 1.0f) {
        PSMTXScale(scale, sx, sy, sz);
        flags |= 2;
    }
    if (scx != 0.0f || scy != 0.0f || scz != 0.0f) {
        if ((flags & 2) == 0) {
            if (stx != 0.0f || sty != 0.0f || stz != 0.0f) {
                PSMTXTrans(scale, (double)stx, (double)sty, (double)stz);
                flags |= 2;
            }
        } else {
            PSMTXTrans(centerA, (double)-scx, (double)-scy, (double)-scz);
            PSMTXTrans(centerB, (double)scx, (double)scy, (double)scz);
            PSMTXTrans(centerC, (double)stx, (double)sty, (double)stz);
            PSMTXConcat(centerC, centerB, tmp);
            PSMTXConcat(tmp, scale, tmp);
            PSMTXConcat(tmp, centerA, scale);
        }
    }

    PSMTXIdentity(rot);
    if (rz != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rz), 'z');
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (ry != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * ry), 'y');
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (rx != 0.0f) {
        PSMTXRotRad(tmp, (double)(float_deg2rad_8041fbb4 * rx), 'x');
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (rcx != 0.0f || rcy != 0.0f || rcz != 0.0f) {
        if ((flags & 4) == 0) {
            if (rtx != 0.0f || rty != 0.0f || rtz != 0.0f) {
                PSMTXTrans(rot, (double)rtx, (double)rty, (double)rtz);
                flags |= 4;
            }
        } else {
            PSMTXTrans(centerB, (double)-rcx, (double)-rcy, (double)-rcz);
            PSMTXTrans(centerA, (double)rcx, (double)rcy, (double)rcz);
            PSMTXTrans(centerC, (double)rtx, (double)rty, (double)rtz);
            PSMTXConcat(centerC, centerA, tmp);
            PSMTXConcat(tmp, rot, rot);
            PSMTXConcat(rot, centerB, rot);
        }
    }

    g_modeling_mtx_lv++;
    g_modeling_mtx = g_modeling_mtx_stack + g_modeling_mtx_lv * 0x30;
    switch (flags) {
        case 0:
            PSMTXIdentity(tmp);
            child = &tmp;
            break;
        case 1:
            child = &trans;
            break;
        case 2:
            child = &scale;
            break;
        case 3:
            PSMTXConcat(trans, scale, tmp);
            child = &tmp;
            break;
        case 4:
            child = &rot;
            break;
        case 5:
            PSMTXConcat(trans, rot, tmp);
            child = &tmp;
            break;
        case 6:
            PSMTXConcat(rot, scale, tmp);
            child = &tmp;
            break;
        default:
            PSMTXConcat(trans, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            child = &trans;
            break;
    }
    PSMTXConcat((void*)(g_modeling_mtx_stack + (g_modeling_mtx_lv - 1) * 0x30), *child, g_modeling_mtx);
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
