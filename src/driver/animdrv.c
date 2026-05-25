#include "driver/animdrv.h"

extern s32 wp;

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
