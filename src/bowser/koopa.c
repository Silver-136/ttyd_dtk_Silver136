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
void marioResetCamFollowRate(void);
void camFollowYOff(void);
void marioPreJump(void);
double marioMakeJumpPara(void);
void marioSetPeakYpos(void);
void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int value, u32 size);
s32 strcmp(const char* a, const char* b);
extern void* gp;
extern f32 marioJumpData[];
extern f32 float_1_80426718;
extern f32 float_0_80426734;
extern f32 float_7_8042673c;
extern f32 float_neg0p05_80426750;
extern f32 float_neg0p08_80426754;
extern f32 float_neg0p006_80426758;
extern f32 float_neg0p002_8042675c;
extern f32 float_neg0p1_80426760;
extern f32 float_neg0p2_80426764;
extern f32 float_neg0p024_80426768;
extern f32 float_neg0p01_8042676c;
extern f32 float_2p6_80426780;
extern f32 float_neg0p06_80426784;
extern f32 float_neg0p004_80426788;
extern f32 float_neg0p4_8042678c;
extern f32 float_0p08_80426790;
extern f32 float_neg0p02_80426794;
extern f32 float_8p1_80426798;
extern f32 float_neg0p3_8042679c;
extern f32 float_8p4_804267a0;
extern f32 float_neg0p018_804267a4;
extern f32 float_10p5_804267a8;
extern f32 float_neg0p12_804267ac;
extern f32 float_370_804267bc;
extern const char vec3_802fd238[];
extern const char str_KPA2_S_1_802fd2b8[];
extern const char str_KPA_W_1_802fd2c4[];
extern const char str_KPA2_W_1_802fd2cc[];
extern const char str_KPA_R_1_802fd2d8[];
extern const char str_KPA2_R_1_802fd2e0[];
extern const char str_KPA2_Z_2_802fd2ec[];
extern const char str_kpa_03_802fd2f8[];
extern const char str_M_J_1B_802fd300[];
extern const char str_M_J_1C_802fd308[];

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
    ;
}


u8 kpaFall(void) {
    return 0;
}


void kpa3DMain(void* mario) {
    ;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpaPreInit(void) {
    register const char* base;
    register void* mario;

    base = vec3_802fd238;
    mario = marioGetPtr();
    if (*(void**)((s32)mario + 0x298) == 0) {
        *(void**)((s32)mario + 0x298) = __memAlloc(0, 0x154);
    }
    memset(*(void**)((s32)mario + 0x298), 0, 0x154);
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 4) = 1;
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0xD0) = 1;
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0xE0) = 0;
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0xD8) = 1;

    mario = marioGetPtr();
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x14C) = -1;

    mario = marioGetPtr();
    if (strcmp((char*)((s32)gp + 0x12C), base + 0xD8) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), base + 0xC0) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), base + 0xE0) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), base + 0xE8) == 0 ||
        strcmp((char*)((s32)gp + 0x12C), base + 0xF0) == 0) {
        *(u32*)((s32)mario + 0x14) |= 1;
    } else {
        *(u32*)((s32)mario + 0x14) &= ~1;
    }

    mario = marioGetPtr();
    if (strcmp((char*)((s32)gp + 0x12C), base + 0xC0) == 0) {
        *(u32*)((s32)mario + 0x14) |= 2;
    } else {
        *(u32*)((s32)mario + 0x14) &= ~2;
    }
    fireefp = 0;
}

void kpa_jumpNpc(void) {
    void* mario = marioGetPtr();
    f32 valueA;
    f32 valueB;

    if ((*(u32*)((s32)mario + 0x0C) & 1) != 0) {
        *(u32*)((s32)mario + 0x0C) &= ~1;
        marioResetCamFollowRate();
        *(u32*)mario &= ~0x80;
        camFollowYOff();
        marioPreJump();
        valueB = float_neg0p06_80426784;
        valueA = float_2p6_80426780;
        *(f32*)((s32)mario + 0x7C) = valueA;
        valueA = float_neg0p01_8042676c;
        *(f32*)((s32)mario + 0x80) = valueB;
        valueB = float_neg0p002_8042675c;
        *(f32*)((s32)mario + 0x84) = valueA;
        *(f32*)((s32)mario + 0x88) = valueB;
        if ((*(u32*)mario & 0x2000) == 0 && (s8)*(u8*)((s32)mario + 0x3C) == 0) {
            marioChgPose((void*)str_M_J_1B_802fd300);
        }
        *(u32*)mario |= 0x10000;
        *(s32*)((s32)mario + 0x44) = 0;
    }
    if (*(s32*)((s32)mario + 0x44) != 0) {
    } else {
        marioMakeJumpPara();
        if (*(f32*)((s32)mario + 0x7C) <= float_0_80426734) {
            if ((*(u32*)mario & 0x2000) == 0 && (s8)*(u8*)((s32)mario + 0x3C) == 0) {
                marioChgPose((void*)str_M_J_1C_802fd308);
            }
            marioSetPeakYpos();
            valueA = float_neg0p06_80426784;
            valueB = float_neg0p004_80426788;
            *(f32*)((s32)mario + 0x80) = valueA;
            valueA = float_neg0p002_8042675c;
            *(f32*)((s32)mario + 0x84) = valueB;
            *(f32*)((s32)mario + 0x88) = valueA;
            *(u32*)mario &= ~0x10000;
            *(u32*)mario |= 0x20000;
        }
    }
    *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    *(s16*)((s32)mario + 0x50) += 1;
}

f32 kpaSetJumpPara(void) {
    register f32* data;
    void* mario;
    f32 valueA;
    f32 valueB;

    data = marioJumpData;
    mario = marioGetPtr();
    *(f32*)((s32)mario + 0x7C) = data[0];
    *(f32*)((s32)mario + 0x80) = data[1];
    *(f32*)((s32)mario + 0x84) = data[2];
    *(f32*)((s32)mario + 0x88) = data[3];

    switch (*(s32*)((s32)*(void**)((s32)mario + 0x298) + 4)) {
        case 0:
            valueB = float_neg0p4_8042678c;
            valueA = float_7_8042673c;
            *(f32*)((s32)mario + 0x7C) = valueA;
            valueA = float_0p08_80426790;
            *(f32*)((s32)mario + 0x80) = valueB;
            valueB = float_neg0p02_80426794;
            *(f32*)((s32)mario + 0x84) = valueA;
            *(f32*)((s32)mario + 0x88) = valueB;
            break;
        case 1:
            valueB = float_neg0p3_8042679c;
            valueA = float_8p1_80426798;
            *(f32*)((s32)mario + 0x7C) = valueA;
            valueA = float_0p08_80426790;
            *(f32*)((s32)mario + 0x80) = valueB;
            valueB = float_neg0p02_80426794;
            *(f32*)((s32)mario + 0x84) = valueA;
            *(f32*)((s32)mario + 0x88) = valueB;
            break;
        case 2:
            valueB = float_neg0p1_80426760;
            valueA = float_8p4_804267a0;
            *(f32*)((s32)mario + 0x7C) = valueA;
            valueA = float_0p08_80426790;
            *(f32*)((s32)mario + 0x80) = valueB;
            valueB = float_neg0p018_804267a4;
            *(f32*)((s32)mario + 0x84) = valueA;
            *(f32*)((s32)mario + 0x88) = valueB;
            break;
        case 3:
            valueB = float_neg0p12_804267ac;
            valueA = float_10p5_804267a8;
            *(f32*)((s32)mario + 0x7C) = valueA;
            valueA = float_0p08_80426790;
            *(f32*)((s32)mario + 0x80) = valueB;
            valueB = float_neg0p018_804267a4;
            *(f32*)((s32)mario + 0x84) = valueA;
            *(f32*)((s32)mario + 0x88) = valueB;
            break;
    }

    return *(f32*)((s32)mario + 0x7C);
}

s32 kpaGetBodyStatus(void) {
    void* mario;
    register s32 status;
    s32 mode;
    f32 y;
    f32 limit;

    mario = marioGetPtr();
    mode = *(u8*)((s32)mario + 0x3C);
    if (mode != 2) {
        return -1;
    }
    status = 0;
    if (strcmp((char*)((s32)gp + 0x12C), str_kpa_03_802fd2f8) == 0) {
        y = *(f32*)((s32)mario + 0x90);
        limit = float_370_804267bc;
        if (y > limit) {
            return 0;
        }
        if (y + *(f32*)((s32)mario + 0x1BC) < limit) {
            status = 2;
        } else {
            status = 1;
        }
    }
    return status;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 kpaCamFollow(void) {
    return 0;
}


u8 kpaCancelAttackMode(void) {
    return 0;
}


u8 kpaAttackStart(void) {
    return 0;
}

void unk_801fd110(void) {
    extern void effSoftDelete(void* effect);
    extern void psndSFXOff(s32 soundId);
    void* mario;
    void* work;
    s32 soundId;

    if (fireefp != NULL) {
        effSoftDelete(fireefp);
        fireefp = NULL;
        mario = marioGetPtr();
        work = *(void**)((s32)mario + 0x298);
        soundId = *(s32*)((s32)work + 0x14C);
        if ((u32)(soundId + 0x10000) != 0xFFFF) {
            psndSFXOff(soundId);
            *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x14C) = -1;
        }
    }
}
