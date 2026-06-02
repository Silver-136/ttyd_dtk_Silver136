#include "bowser/koopa.h"

void* fireefp;
f32 baseScaleTbl[4];
f32 kpaWidthDat[4];
extern u32 kpaPauseSeTable[];

void* marioGetPtr(void);
void kpa2DMain(void* mario);
void kpa3DMain(void* mario);
s32 irand(s32 range);
void psndSFXOn(u32 id);
s32 sysMsec2Frame(s32 msec);
void animPoseSetMaterialFlagOff(void* pose, u32 flag);
void animPoseSetMaterialFlagOn(void* pose, u32 flag);
void marioChgPose(void* pose);
void kpaSetSpec(void* mario);
extern f32 float_1_80426718;
extern f32 float_neg0p05_80426750;
extern f32 float_neg0p08_80426754;
extern f32 float_neg0p006_80426758;
extern f32 float_neg0p002_8042675c;
extern f32 float_neg0p1_80426760;
extern f32 float_neg0p2_80426764;
extern f32 float_neg0p024_80426768;
extern f32 float_neg0p01_8042676c;

void* getKoopaFireEfp(void) {
    return fireefp;
}

f32 kpaGetBaseScale(s32 level) {
    return baseScaleTbl[level];
}

f32 kpaGetWidth(s32 level) {
    return kpaWidthDat[level];
}

s32 kpaJumpChk(void) {
    s32 value = *(s16*)((s32)marioGetPtr() + 0x50);
    return ((u32)(-value) | (u32)value) >> 31;
}

s32 kpaGetStageViewType(void) {
    return ((*(u32*)((s32)marioGetPtr() + 0x14) & 1) == 0);
}

s32 kpaGetStageType(void) {
    return (*(u32*)((s32)marioGetPtr() + 0x14) >> 1) & 1;
}

s32 kpaMutekiCheck(void) {
    void* mario = marioGetPtr();
    return (*(u32*)*(void**)((s32)mario + 0x298) >> 0x10) & 1;
}

s32 kpaNoHitEnemyCheck(void) {
    return *(s16*)((s32)marioGetPtr() + 0x78);
}

s32 kpaGetAttackMode(void) {
    s32 value = *(s32*)((s32)*(void**)((s32)marioGetPtr() + 0x298) + 0x13C);
    return ((u32)(-value) | (u32)value) >> 31;
}

void kpaAttackStart2(void) {
    void* work = *(void**)((s32)marioGetPtr() + 0x298);
    if (*(s32*)((s32)work + 0x13C) == 0) {
        *(s32*)((s32)work + 0x13C) = 30;
    }
}

void kpaMain(void) {
    void* mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0x14) & 1) != 0) {
        kpa2DMain(mario);
    } else {
        kpa3DMain(mario);
    }
}

void kpaPauseSE2(void) {
    psndSFXOn(kpaPauseSeTable[irand(0x1C)] | 0x20000);
}

s32 kpaHipAttackCheck(void) {
    register s32 ret = 0;
    void* mario = marioGetPtr();

    if ((s32)*(u8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x11) {
        ret = 1;
    }
    return ret;
}

s32 kpaGetLevel(void) {
    register s32 level = 0;
    void* mario = marioGetPtr();

    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        level = *(s32*)((s32)*(void**)((s32)mario + 0x298) + 4);
    }
    return level;
}

void kpaSetLevel(s32 level) {
    void* mario = marioGetPtr();
    void* work;
    f32 one;
    f32 scale;

    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        work = *(void**)((s32)mario + 0x298);
        if (work != 0) {
            *(s32*)((s32)work + 4) = level;
            one = float_1_80426718;
            scale = baseScaleTbl[level];
            *(f32*)((s32)mario + 0xCC) = scale;
            *(f32*)((s32)mario + 0xC8) = scale;
            *(f32*)((s32)mario + 0xD0) = one;
            kpaSetSpec(mario);
        }
    }
}

void kpaMSetMutekiTimer(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) = sysMsec2Frame(12000);
    *(s16*)((s32)mario + 0x78) = 0;
}

void kpaMutekiOff(void) {
    void* mario = marioGetPtr();
    void* work;

    animPoseSetMaterialFlagOff(*(void**)((s32)mario + 0x22C), 0x80000000);
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work &= ~0x10000;
}

void kpaMutekiOn(void) {
    void* mario = marioGetPtr();
    void* work;

    animPoseSetMaterialFlagOn(*(void**)((s32)mario + 0x22C), 0x80000000);
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work |= 0x10000;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpaChgPose(void* pose3d, void* pose2d) {
    void* p2d = pose2d;
    void* p3d = pose3d;
    void* pose;

    if ((s32)((*(u32*)((s32)marioGetPtr() + 0x14) & 1) ^ 1) == 1) {
        pose = p3d;
    } else {
        pose = p2d;
    }
    marioChgPose(pose);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 kpaFireAttackCheck(void) {
    void* mario = marioGetPtr();
    s32 mode;

    if ((*(u32*)mario & 1) == 0) {
        return 0;
    }
    mode = *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x13C);
    if ((mode >= 0x14 && mode <= 0x17) || (mode >= 0x28 && mode <= 0x2B)) {
        return 1;
    }
    return 0;
}

void kpaSetFallPara(void) {
    void* mario = marioGetPtr();
    f32 valueB;
    f32 valueA;

    if ((*(u16*)((s32)mario + 0x24A) & 0x100) != 0) {
        valueB = float_neg0p08_80426754;
        valueA = float_neg0p05_80426750;
        *(f32*)((s32)mario + 0x7C) = valueA;
        valueA = float_neg0p006_80426758;
        *(f32*)((s32)mario + 0x80) = valueB;
        valueB = float_neg0p002_8042675c;
        *(f32*)((s32)mario + 0x84) = valueA;
        *(f32*)((s32)mario + 0x88) = valueB;
    } else {
        valueB = float_neg0p2_80426764;
        valueA = float_neg0p1_80426760;
        *(f32*)((s32)mario + 0x7C) = valueA;
        valueA = float_neg0p024_80426768;
        *(f32*)((s32)mario + 0x80) = valueB;
        valueB = float_neg0p01_8042676c;
        *(f32*)((s32)mario + 0x84) = valueA;
        *(f32*)((s32)mario + 0x88) = valueB;
    }
}


u8 kpaFireAttack(void) {
    return 0;
}


u8 kpaFireMain(void) {
    return 0;
}


void kpa2DMain(void* mario) {
}


u8 kpaFall(void) {
    return 0;
}


void kpa3DMain(void* mario) {
}


u8 kpaPreInit(void) {
    return 0;
}


u8 kpa_jumpNpc(void) {
    return 0;
}


double kpaSetJumpPara(void) {
    return 0.0;
}


s32 kpaGetBodyStatus(void) {
    return 0;
}


s32 kpaCamFollow(void) {
    return 0;
}


u8 kpaCancelAttackMode(void) {
    return 0;
}


u8 kpaAttackStart(void) {
    return 0;
}
