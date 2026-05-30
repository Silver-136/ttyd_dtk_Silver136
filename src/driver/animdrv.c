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


u8 animPoseMain(s32 poseIdx) {
    return 0;
}


u8 renderProc(int shapeIdx) {
    return 0;
}


u8 animPaperPoseDispSub(void) {
    return 0;
}


void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 a, f32 b) {
}


u8 animPoseDisp_MakeExtTexture(void) {
    return 0;
}


u8 materialProc(int shapeIdx) {
    return 0;
}


s32 animPoseEntry(void* name, s32 mode) {
    return 0;
}


void dispProc(int parentIdx, int groupIdx, int animFrame0, int animFrame1, float blend) {
}


int testAlloc(u32 param_1) {
    return 0;
}


u8 animPoseDrawShape(int poseIdx, int param_2) {
    return 0;
}


void animPoseSetPaperAnimGroup(void* pose, s32 anim, s32 enabled) {
}


void animPoseSetEffect(void* pose, s32 value, s32 flag) {
}


u8 animInit(void) {
    return 0;
}


void animPoseRefresh(void) {
}


u8 evalProc(int parentGroupIdx, int groupIdx) {
    return 0;
}


void animPoseAutoRelease(s32 value) {
}


void animPoseRelease(s32 poseId) {
}


void animPoseSetEffectAnim(void* pose, s32 value, s32 flag) {
}


void animPoseSetAnim(s32 poseId, void* name, s32 force) {
}


void animPoseSetPaperAnim(void* pose, s32 anim, s32 enabled) {
}


void animPaperPoseEntry(s32 name, s32 flag) {
}


s32 animEffectAsync(void*, s32) {
    return 0;
}


s32 animGroupBaseAsync(void* name, s32 mode, s32 flags) {
    return 0;
}


u8 animPoseSetLocalTime(double param_1, int param_2) {
    return 0;
}


s32 animPaperPoseGetId(s32 name, s32 flag) {
    return 0;
}


void animPaperPoseRelease(s32 poseId) {
}


u8 animSetPaperTexMtx(void* param_1, void* param_2, void* param_3) {
    return 0;
}


u8 animPoseWorldMatrixEvalOn(int poseIdx, u8 param_2) {
    return 0;
}


u8 animPoseWorldPositionEvalOn(int poseIdx, u8 param_2) {
    return 0;
}


int animPoseGetGroupIdx(int poseIdx, char* pGroupName) {
    return 0;
}


int animPoseGetShapeIdx(int param_1, char* param_2) {
    return 0;
}


s64 animTimeGetTime(int param_1) {
    return 0;
}


u8 animPaperPoseDisp(void) {
    return 0;
}


void animPoseDraw(int poseIdx, double x, double y, double z, double rot, double scale, int param_7) {
}


double animPoseGetHeight(int param_1) {
    return 0.0;
}


double animPoseGetRadius(int param_1) {
    return 0.0;
}


u32 animPoseGetPeraEnd(int param_1) {
    return 0;
}


s32 animPoseGetAnimDataPtr(int poseIdx) {
    return 0;
}


void animPosePaperPeraOn(void* pose) {
}


char* animPoseGetGroupName(int poseIdx, int groupIdx) {
    return 0;
}


u32 animPoseTestXLU(int param_1) {
    return 0;
}


u8 animMain(void) {
    return 0;
}


u8 animPosePaperPeraOff(int param_1) {
    return 0;
}


u8 animPoseSetStartTime(int param_1, s32 param_2, s32 param_3, s32 param_4) {
    return 0;
}


u8 animPoseSetMaterialEvtColor(int param_1, void* param_2) {
    return 0;
}


void animPose_AllocBuffer(void* pPose) {
}


void _animPoseDrawMtx(void* pPose, void* pMtx, int dispMode, double rot, double scale) {
}


u8 animSetMaterial_Texture(s32 texCount, int* pTexIdRemap, void* pTexEntries, void* pAnimEntries, s32 texBindsCapacity, void* pTexBinds, void* pTexFile) {
    return 0;
}


void pushGXModelMtx_JointNode__(void* pJoint, double parentScaleX, double parentScaleY, double parentScaleZ) {
}


u8 pushGXModelMtx_TransformNode__(void* pNode) {
    return 0;
}


void animSetPaperTexObj(void* param_1, void* param_2, void* param_3, void* param_4, void* param_5, void* param_6, s32 param_7) {
}


void* animPoseGetCurrentAnim(int poseIdx) {
    return 0;
}


void* animPoseGetAnimBaseDataPtr(int param_1) {
    return 0;
}
