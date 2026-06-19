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


u8 renderProc(int shapeIdx) {
    return 0;
}


u8 animPaperPoseDispSub(s32 cameraId, void* pose) {
    return 0;
}


void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, double a, double b) {
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
