#include "bowser/koopa.h"

void* fireefp;
f32 baseScaleTbl[4];
f32 kpaWidthDat[4];
extern u32 kpaPauseSeTable[];

void* marioGetPtr(void);
void kpa2DMain(void* mario);
void kpa3DMain(void* mario);
void kpaFireAttack(void);
u8 kpaFireMain(void);
u8 kpaFall(void);
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

s32 kpaNoHitEnemyCheck(void) {
    return *(s16*)((s32)marioGetPtr() + 0x78);
}

s32 kpaMutekiCheck(void) {
    void* mario = marioGetPtr();
    return (*(u32*)*(void**)((s32)mario + 0x298) >> 0x10) & 1;
}

void kpaMutekiOn(void) {
    void* mario = marioGetPtr();
    void* work;

    animPoseSetMaterialFlagOn(*(void**)((s32)mario + 0x22C), 0x80000000);
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work |= 0x10000;
}

void kpaMutekiOff(void) {
    void* mario = marioGetPtr();
    void* work;

    animPoseSetMaterialFlagOff(*(void**)((s32)mario + 0x22C), 0x80000000);
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work &= ~0x10000;
}

void kpaMSetMutekiTimer(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) = sysMsec2Frame(12000);
    *(s16*)((s32)mario + 0x78) = 0;
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

f32 kpaGetWidth(s32 level) {
    return kpaWidthDat[level];
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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


s32 kpaGetStageType(void) {
    return (*(u32*)((s32)marioGetPtr() + 0x14) >> 1) & 1;
}

s32 kpaGetStageViewType(void) {
    return ((*(u32*)((s32)marioGetPtr() + 0x14) & 1) == 0);
}
s32 kpaCamFollow(void) {
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern f32 float_76_804267b8;
    void* mario = marioGetPtr();

    if (marioBgmodeChk() == 1 || (((*(u32*)((s32)marioGetPtr() + 0x14) & 1) ^ 1) == 1)) {
        return 0;
    }
    if (*(f32*)((s32)mario + 0x90) > float_76_804267b8) {
        camFollowYOn();
    }
    return 1;
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void kpaMain(void) {
    void* mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0x14) & 1) != 0) {
        kpa2DMain(mario);
    } else {
        kpa3DMain(mario);
    }
}

void kpa2DMain(void* unused) {
    extern void* kpaHitCheck(void);
    extern void marioItemGetChk(void);
    extern void marioBottomless(void);
    extern void marioPreDisp(void);
    extern void marioBalloonMain(void);
    extern void mario_kemuri(void);
    extern void marioCamMain(void);
    extern void marioChkOverhead(void);
    extern void marioChkGnd(void);
    extern void marioChkGnd2(void);
    extern void marioBoots(void);
    extern void marioForceMoveMain(void);
    extern u8 kpaFall(void);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern void marioCheckWall(f32 speed, f32 angle);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECMag(void* v);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern f32 float_0p015_804267b0;
    extern f32 float_0p25_804267b4;
    void* mario = marioGetPtr();
    f32 oldPos[3];
    f32 diff[3];
    f32 sinValue;
    f32 cosValue;
    f32 sideSpeed = float_0_80426734;
    f32 speed;
    f32 angle;
    f32 maxSpeed;
    f32 mag;

    kpaHitCheck();
    if (*(s16*)((s32)mario + 0x78) != 0) {
        *(s16*)((s32)mario + 0x78) -= 1;
    }
    *(void**)((s32)mario + 0x1F8) = 0;
    *(void**)((s32)mario + 0x1E4) = 0;
    *(void**)((s32)mario + 0x1E0) = 0;
    *(void**)((s32)mario + 0x1F0) = 0;
    *(void**)((s32)mario + 0x1F4) = 0;

    marioItemGetChk();
    kpaFireAttack();
    kpaFireMain();
    mario = marioGetPtr();
    if (*(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) > 0) {
        *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) -= 1;
        if (*(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) <= 0) {
            *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) = 0;
            mario = marioGetPtr();
            animPoseSetMaterialFlagOff(*(void**)((s32)mario + 0x22C), 0x80000000);
            *(u32*)*(void**)((s32)mario + 0x298) &= ~0x10000;
        }
    }

    if (*(u16*)((s32)mario + 0x2E) == 8) {
        *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
        *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
        *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
        *(f32*)((s32)mario + 0x174) = float_0_80426734;
        *(f32*)((s32)mario + 0x178) = float_0_80426734;
        *(f32*)((s32)mario + 0x17C) = float_0_80426734;
        marioBottomless();
        marioPreDisp();
        mario_kemuri();
        marioCamMain();
        *(s32*)((s32)mario + 0x34) += 1;
        return;
    }

    oldPos[0] = *(f32*)((s32)mario + 0x8C);
    oldPos[1] = *(f32*)((s32)mario + 0x90);
    oldPos[2] = *(f32*)((s32)mario + 0x94);
    if ((*(u32*)mario & 0x80) == 0 && *(s16*)((s32)mario + 0x50) != 0) {
        sideSpeed = *(f32*)((s32)mario + 0x194) * *(f32*)((s32)mario + 0x228) * float_0p015_804267b0;
        if ((*(u32*)mario & 0x70000) == 0) {
            sideSpeed *= float_0p25_804267b4;
        }
        sincosf(*(f32*)((s32)mario + 0x198), &sinValue, &cosValue);
        sideSpeed *= sinValue;
    }
    sincosf(*(f32*)((s32)mario + 0x1A4), &sinValue, &cosValue);
    speed = *(f32*)((s32)mario + 0x180) * sinValue + sideSpeed;
    angle = angleABf(float_0_80426734, float_0_80426734, speed, float_0_80426734);
    if (speed < float_0_80426734) {
        speed = -speed;
    }
    maxSpeed = *(f32*)((s32)mario + 0x188) * *(f32*)((s32)mario + 0x228);
    if (speed > maxSpeed) {
        speed = maxSpeed;
    }
    marioCheckWall(speed, angle);
    *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x178);
    PSVECSubtract((void*)((s32)mario + 0x8C), oldPos, diff);
    mag = PSVECMag(diff);
    if (mag > speed) {
        PSVECScale(diff, diff, speed / mag);
        PSVECAdd(oldPos, diff, (void*)((s32)mario + 0x8C));
    }

    if (*(u32*)mario & 0x10000) {
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else if (*(u32*)mario & 0x20000) {
        kpaFall();
        marioChkOverhead();
        if (*(u16*)((s32)mario + 0x2E) != 0x10 && *(u16*)((s32)mario + 0x2E) != 0x11 && *(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else {
        marioChkOverhead();
        marioChkGnd();
    }

    marioBoots();
    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    marioCamMain();
    *(s32*)((s32)mario + 0x34) += 1;
    *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
    *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
    *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
    *(f32*)((s32)mario + 0x174) = float_0_80426734;
    *(f32*)((s32)mario + 0x178) = float_0_80426734;
    *(f32*)((s32)mario + 0x17C) = float_0_80426734;
}

void kpa3DMain(void* unused) {
    extern void* kpaHitCheck(void);
    extern void marioPreDisp(void);
    extern void marioBottomless(void);
    extern void partyMain(void);
    extern void marioCamMain(void);
    extern void marioItemGetChk(void);
    extern void marioBalloonMain(void);
    extern void mario_kemuri(void);
    extern void marioMove(void);
    extern void marioChkOverhead(void);
    extern void marioChkGnd(void);
    extern void marioChkGnd2(void);
    extern void marioChgMot(s32 motion);
    extern void marioSetFallPara(void);
    extern void marioBoots(void);
    extern void marioForceMoveMain(void);
    extern void kpaSearchFrontUnder(void);
    void* mario = marioGetPtr();
    void* hit = kpaHitCheck();

    if (hit != 0) {
        *(void**)((s32)mario + 0x1F8) = hit;
    }
    if (*(s16*)((s32)mario + 0x78) != 0) {
        *(s16*)((s32)mario + 0x78) -= 1;
    }

    if (*(u16*)((s32)mario + 0x2E) == 0x18 || *(u16*)((s32)mario + 0x2E) == 0x1F || *(u16*)((s32)mario + 0x2E) == 0x20) {
        marioPreDisp();
        marioBottomless();
        *(s32*)((s32)mario + 0x34) += 1;
        partyMain();
        marioCamMain();
        *(void**)((s32)mario + 0x1F8) = 0;
        *(void**)((s32)mario + 0x1E4) = 0;
        *(void**)((s32)mario + 0x1E0) = 0;
        *(void**)((s32)mario + 0x1F0) = 0;
        *(void**)((s32)mario + 0x1F4) = 0;
        return;
    }

    marioItemGetChk();
    kpaFireAttack();
    kpaFireMain();
    mario = marioGetPtr();
    if (*(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) > 0) {
        *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) -= 1;
        if (*(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) <= 0) {
            *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x138) = 0;
            mario = marioGetPtr();
            animPoseSetMaterialFlagOff(*(void**)((s32)mario + 0x22C), 0x80000000);
            *(u32*)*(void**)((s32)mario + 0x298) &= ~0x10000;
        }
    }

    if (*(u16*)((s32)mario + 0x2E) == 8) {
        *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
        *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
        *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
        *(f32*)((s32)mario + 0x174) = float_0_80426734;
        *(f32*)((s32)mario + 0x178) = float_0_80426734;
        *(f32*)((s32)mario + 0x17C) = float_0_80426734;
        marioBottomless();
        marioPreDisp();
        marioBalloonMain();
        mario_kemuri();
        partyMain();
        marioCamMain();
        *(s32*)((s32)mario + 0x34) += 1;
        return;
    }

    if (*(u32*)mario & 0x40000) {
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 100) {
            marioChgMot(10);
            marioSetFallPara();
        }
    } else if (*(u32*)mario & 0x10000) {
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else if (*(u32*)mario & 0x20000) {
        marioMove();
        kpaFall();
        kpaSearchFrontUnder();
        marioChkOverhead();
        if (*(u16*)((s32)mario + 0x2E) != 0x10 && *(u16*)((s32)mario + 0x2E) != 0x11 && *(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else {
        marioMove();
        kpaSearchFrontUnder();
        marioChkOverhead();
        marioChkGnd();
    }

    marioBoots();
    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    *(s32*)((s32)mario + 0x34) += 1;
    *(u32*)((s32)mario + 0x110) = *(u32*)((s32)mario + 0x8C);
    *(u32*)((s32)mario + 0x114) = *(u32*)((s32)mario + 0x90);
    *(u32*)((s32)mario + 0x118) = *(u32*)((s32)mario + 0x94);
    *(f32*)((s32)mario + 0x174) = float_0_80426734;
    *(f32*)((s32)mario + 0x178) = float_0_80426734;
    *(f32*)((s32)mario + 0x17C) = float_0_80426734;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 kpaFall(void) {
    extern void kpaClearHitobjRide(void);
    extern f32 marioGetFallSpd(void);
    extern void* marioChkLandon(f32 speed, f32* out);
    extern void marioLandOn(void);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern u32 hitGetAttr(void* hit);
    extern s32 kpaChkHitobjRide(void);
    extern f32 float_neg20_80426770;
    extern f32 float_0p01_80426774;
    extern f32 float_neg0p5_80426778;
    extern f32 float_neg0p001_8042677c;
    void* mario = marioGetPtr();
    f32 pos[4];
    f32 speed;
    void* hit;

    if (strcmp((char*)((s32)gp + 0x12C), str_kpa_03_802fd2f8) == 0) {
        *(void**)((s32)mario + 0x1E8) = 0;
        kpaClearHitobjRide();
        speed = marioGetFallSpd();
        if (speed < float_neg20_80426770) {
            speed = float_neg20_80426770;
        }
        if (speed == float_0_80426734) {
            void* mario2 = marioGetPtr();
            if ((*(u16*)((s32)mario2 + 0x24A) & 0x100) != 0) {
                *(f32*)((s32)mario2 + 0x7C) = float_neg0p05_80426750;
                *(f32*)((s32)mario2 + 0x80) = float_neg0p08_80426754;
                *(f32*)((s32)mario2 + 0x84) = float_neg0p006_80426758;
                *(f32*)((s32)mario2 + 0x88) = float_neg0p002_8042675c;
            } else {
                *(f32*)((s32)mario2 + 0x7C) = float_neg0p1_80426760;
                *(f32*)((s32)mario2 + 0x80) = float_neg0p2_80426764;
                *(f32*)((s32)mario2 + 0x84) = float_neg0p024_80426768;
                *(f32*)((s32)mario2 + 0x88) = float_neg0p01_8042676c;
            }
            speed = marioGetFallSpd();
        }
        hit = marioChkLandon(speed, pos);
        if (hit == 0) {
            *(f32*)((s32)mario + 0x90) += speed;
        } else {
            *(void**)((s32)mario + 0x1E8) = hit;
            *(void**)((s32)mario + 0x1EC) = 0;
            *(f32*)((s32)mario + 0x90) = pos[0];
        }
        if (*(void**)((s32)mario + 0x1E8) != 0) {
            marioLandOn();
            psndSFXOn_3D(0x81D, (void*)((s32)mario + 0x8C));
        }
    } else if (*(u16*)((s32)mario + 0x2E) != 0x10 && *(u16*)((s32)mario + 0x2E) != 0x11) {
        if ((marioKeyOffChk() == 0 || marioCtrlOffChk() == 0) && (*(u32*)mario & 0x20) == 0) {
            f32 value = (*(f32*)((s32)mario + 0x180) +
                         float_0p01_80426774 * (float_neg0p5_80426778 - *(f32*)((s32)mario + 0x180)));
            if (value < float_0_80426734) {
                value = float_0_80426734;
            }
            *(f32*)((s32)mario + 0x180) = value;
        }
        if (*(u16*)((s32)mario + 0x2E) != 5) {
            if ((*(u16*)((s32)mario + 0x24A) & 0x100) != 0) {
                *(f32*)((s32)mario + 0x88) = float_neg0p001_8042677c;
            } else {
                *(f32*)((s32)mario + 0x88) = float_neg0p01_8042676c;
            }
        }
        *(void**)((s32)mario + 0x1E8) = 0;
        kpaClearHitobjRide();
        speed = marioGetFallSpd();
        if (speed < float_neg20_80426770) {
            speed = float_neg20_80426770;
        }
        hit = marioChkLandon(speed, pos);
        if (hit == 0) {
            *(f32*)((s32)mario + 0x90) += speed;
        } else {
            *(void**)((s32)mario + 0x1E8) = hit;
            *(void**)((s32)mario + 0x1EC) = 0;
            if ((hitGetAttr(hit) & 0x200) == 0) {
                *(f32*)((s32)mario + 0x90) = pos[0];
            } else {
                *(f32*)((s32)mario + 0x90) += speed;
            }
        }
        if (hit != 0 || kpaChkHitobjRide() != 0) {
            marioLandOn();
        }
    }
}
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

u8 kpaAttackStart(void) {
    extern void marioChgMot(s32 motion);
    void* mario = marioGetPtr();
    void* work = *(void**)((s32)mario + 0x298);

    if (*(s32*)((s32)work + 0x13C) == 0) {
        marioChgMot(0);
        *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x13C) = 1;
    }
}

void kpaAttackStart2(void) {
    void* work = *(void**)((s32)marioGetPtr() + 0x298);
    if (*(s32*)((s32)work + 0x13C) == 0) {
        *(s32*)((s32)work + 0x13C) = 30;
    }
}


void kpaFireAttack(void) {
    extern void* marioGetPtr(void);
    extern void marioChgPose(void*);
    extern u32 psndSFXOn_3D(void*, void*);
    extern void psndSFXOff(u32);
    extern s32 sysMsec2Frame(s32);
    extern void* effGonbabaBreathEntry(f64, f64, f64, f64, s32, s32);
    extern void* fireefp;
    extern f32 baseScaleTbl[4];
    u8* mario = marioGetPtr();
    u8* work = *(u8**)(mario + 0x298);
    s32 state = *(s32*)(work + 0x13C);
    s32 level;
    s32 left;
    f32 dir;
    f32 scale;

    switch (state) {
        case 1:
            *(u32*)mario &= ~0x7800;
            *(s16*)(mario + 0x50) = 0;
            *(s32*)(work + 0xD8) = *(s32*)(work + 4) == 0 ? 12 : 8;
            *(s32*)(work + 0xE0) = 0;
            *(s32*)(work + 0x140) = 0;
            *(s32*)(work + 0x13C) = 20;
            if ((*(u32*)mario & 0x200000) == 0) {
                *(f32*)(mario + 0x88) = 0.0f;
                *(f32*)(mario + 0x2DC) = 0.0f;
            }
        case 20:
            mario = marioGetPtr();
            marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1A" : (void*)"KPA_A_1A");
            *(s32*)(work + 0x140) = 6;
            *(s32*)(work + 0x13C) = 21;
            break;
        case 21:
            if (--*(s32*)(work + 0x140) < 1) {
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1B" : (void*)"KPA_A_1B");
                *(s32*)(work + 0x140) = 8;
                *(s32*)(work + 0x13C) = 22;
                mario = marioGetPtr();
                work = *(u8**)(mario + 0x298);
                if (*(s32*)(work + 0x14C) == -1) {
                    *(u32*)(work + 0x14C) = psndSFXOn_3D((void*)0x821, mario + 0x8C);
                    psndSFXOn_3D((void*)0x131, mario + 0x8C);
                }
            }
            break;
        case 22:
            mario = marioGetPtr();
            work = *(u8**)(mario + 0x298);
            if (fireefp == NULL) {
                left = (*(f32*)(mario + 0x1A4) >= 90.0f && *(f32*)(mario + 0x1A4) <= 270.0f);
                dir = left ? 1.0f : -1.0f;
                level = (*(u8*)(mario + 0x3C) == 2) ? *(s32*)(work + 4) : 0;
                scale = baseScaleTbl[level];
                fireefp = effGonbabaBreathEntry(
                    *(f32*)(mario + 0x8C) + 40.0f * dir * scale,
                    *(f32*)(mario + 0x90) + 20.0f * scale +
                        ((*(u32*)(mario + 0x14) & 1) ? 0.0f : 6.0f),
                    *(f32*)(mario + 0x94),
                    *(s32*)(work + 4) == 0 ? 0.3 : *(s32*)(work + 4) == 1 ? 1.0 :
                    *(s32*)(work + 4) == 2 ? 1.4 : 1.5,
                    *(s32*)(work + 4) == 3 ? 0 : 7, 0);
                left = (*(f32*)(mario + 0xAC) >= 90.0f && *(f32*)(mario + 0xAC) <= 270.0f);
                *(f32*)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x48) = left ? 180.0f : 0.0f;
                if (*(void**)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x4C)) {
                    u8* child = *(u8**)((u8*)*(void**)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x4C) + 0xC);
                    *(f32*)(child + 0x48) = left ? 180.0f : 0.0f;
                }
            }
            if (--*(s32*)(work + 0x140) < 1) {
                *(s32*)(work + 0x140) = sysMsec2Frame(400);
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1C" : (void*)"KPA_A_1C");
                *(s32*)(work + 0x13C) = 23;
            }
            break;
        case 23:
            if (--*(s32*)(work + 0x140) < 1 && *(s32*)(work + 0x144) == 0) {
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1D" : (void*)"KPA_A_1D");
                *(s32*)(work + 0x13C) = 24;
                *(s32*)(work + 0x140) = 6;
                if (*(s32*)(work + 0x14C) != -1) {
                    psndSFXOff(*(u32*)(work + 0x14C));
                    *(s32*)(work + 0x14C) = -1;
                }
            }
            break;
        case 24:
            if (--*(s32*)(work + 0x140) < 1) {
                *(s32*)(work + 0x13C) = 0;
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_S_1" : (void*)"KPA_S_1");
            }
            break;
    }

    state = *(s32*)(work + 0x13C);
    switch (state) {
        case 30:
            *(s32*)(work + 0xD8) = *(s32*)(work + 4) == 0 ? 12 : 8;
            *(s32*)(work + 0xE0) = 0;
            *(s32*)(work + 0x140) = 0;
            *(s32*)(work + 0x13C) = 40;
            if ((*(u32*)mario & 0x200000) == 0) {
                *(f32*)(mario + 0x88) = 0.0f;
                *(f32*)(mario + 0x2DC) = 0.0f;
            }
        case 40:
            mario = marioGetPtr();
            marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1A" : (void*)"KPA_A_1A");
            *(s32*)(work + 0x140) = 6;
            *(s32*)(work + 0x13C) = 41;
            break;
        case 41:
            if (--*(s32*)(work + 0x140) < 1) {
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1B" : (void*)"KPA_A_1B");
                *(s32*)(work + 0x140) = 8;
                *(s32*)(work + 0x13C) = 42;
            }
            break;
        case 42:
            mario = marioGetPtr();
            work = *(u8**)(mario + 0x298);
            if (fireefp == NULL) {
                left = (*(f32*)(mario + 0x1A4) >= 90.0f && *(f32*)(mario + 0x1A4) <= 270.0f);
                dir = left ? 1.0f : -1.0f;
                level = (*(u8*)(mario + 0x3C) == 2) ? *(s32*)(work + 4) : 0;
                scale = baseScaleTbl[level];
                fireefp = effGonbabaBreathEntry(
                    *(f32*)(mario + 0x8C) + 40.0f * dir * scale,
                    *(f32*)(mario + 0x90) + 20.0f * scale +
                        ((*(u32*)(mario + 0x14) & 1) ? 0.0f : 6.0f),
                    *(f32*)(mario + 0x94),
                    *(s32*)(work + 4) == 0 ? 0.3 : *(s32*)(work + 4) == 1 ? 1.0 :
                    *(s32*)(work + 4) == 2 ? 1.4 : 1.5,
                    *(s32*)(work + 4) == 3 ? 0 : 7, 0);
                left = (*(f32*)(mario + 0xAC) >= 90.0f && *(f32*)(mario + 0xAC) <= 270.0f);
                *(f32*)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x48) = left ? 180.0f : 0.0f;
                if (*(void**)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x4C)) {
                    u8* child = *(u8**)((u8*)*(void**)((u8*)*(void**)((u8*)fireefp + 0xC) + 0x4C) + 0xC);
                    *(f32*)(child + 0x48) = left ? 180.0f : 0.0f;
                }
            }
            if (--*(s32*)(work + 0x140) < 1) {
                *(s32*)(work + 0x140) = sysMsec2Frame(400);
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1C" : (void*)"KPA_A_1C");
                *(s32*)(work + 0x13C) = 43;
            }
            break;
        case 43:
            if (--*(s32*)(work + 0x140) < 1 && *(s32*)(work + 0x144) == 0) {
                mario = marioGetPtr();
                marioChgPose((*(u32*)(mario + 0x14) & 1) ? (void*)"KPA2_A_1D" : (void*)"KPA_A_1D");
                *(s32*)(work + 0x13C) = 44;
                *(s32*)(work + 0x140) = 6;
            }
            break;
        case 44:
            if (--*(s32*)(work + 0x140) < 1) {
                *(s32*)(work + 0x13C) = 0;
            }
            break;
    }
}

u8 kpaCancelAttackMode(void) {
    extern void psndSFXOff(s32 soundId);
    void* mario = marioGetPtr();
    void* work = *(void**)((s32)mario + 0x298);
    s32 soundId;

    *(s32*)((s32)work + 0x13C) = 0;
    mario = marioGetPtr();
    work = *(void**)((s32)mario + 0x298);
    soundId = *(s32*)((s32)work + 0x14C);
    if ((u32)(soundId + 0x10000) != 0xFFFF) {
        psndSFXOff(soundId);
        *(s32*)((s32)*(void**)((s32)mario + 0x298) + 0x14C) = -1;
    }
}

s32 kpaGetAttackMode(void) {
    s32 value = *(s32*)((s32)*(void**)((s32)marioGetPtr() + 0x298) + 0x13C);
    return ((u32)(-value) | (u32)value) >> 31;
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

u8 kpaFireMain(void) {
    typedef f32 Mtx[3][4];
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* marioGetPtr(void);
    extern void effSoftDelete(void*);
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXMultVec(Mtx, Vec3*, Vec3*);
    extern void* kpaFlameHitCheck(f64, f64, f64, f64);
    extern void* fireefp;
    extern f32 baseScaleTbl[4];
    u8* mario = marioGetPtr();
    u8* kpa = *(u8**)(mario + 0x298);
    s32 isLeft = 0;

    *(void**)(kpa + 0x138) = NULL;
    if (fireefp != NULL) {
        u8* effect = fireefp;
        u8* work = *(u8**)(effect + 0xC);
        u8* current;
        u32 level = 0;
        f32 side = -1.0f;
        f32 scale;
        f32 x;
        f32 y;
        f32 z;
        s32 i;

        if (*(f32*)(mario + 0x1A4) >= 90.0f && *(f32*)(mario + 0x1A4) <= 270.0f) {
            isLeft = 1;
            side = 1.0f;
        }
        if (*(u8*)(mario + 0x3C) == 2) {
            level = *(u32*)(kpa + 4);
        }
        scale = baseScaleTbl[level];
        x = *(f32*)(mario + 0x8C) + 40.0f * side * scale;
        y = *(f32*)(mario + 0x90) + 20.0f * scale;
        if ((*(u32*)(mario + 0x14) & 1) == 0) {
            y += 6.0f;
        }
        z = *(f32*)(mario + 0x94);
        *(f32*)(work + 4) = x;
        *(f32*)(work + 8) = y;
        *(f32*)(work + 0xC) = z;
        if (*(void**)(work + 0x4C) != NULL) {
            u8* childWork = *(u8**)((u8*)*(void**)(work + 0x4C) + 0xC);
            *(f32*)(childWork + 4) = x;
            *(f32*)(childWork + 8) = y;
            *(f32*)(childWork + 0xC) = z;
        }

        level = 0;
        mario = marioGetPtr();
        kpa = *(u8**)(mario + 0x298);
        if (*(u8*)(mario + 0x3C) == 2) {
            level = *(u32*)(kpa + 4);
        }
        if (level != 0) {
            Mtx translate, rotate, scaleMtx, rootMtx, particleMtx;
            f32 rootScale = *(f32*)(work + 0x40);
            PSMTXTrans(translate, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
            PSMTXRotRad(rotate,
                        0.017453292f * (*(f32*)(work + 0x48) -
                                        *(f32*)((u8*)camGetPtr(4) + 0x114)), 'y');
            if (*(f32*)(work + 0x48) <= 90.0f || *(f32*)(work + 0x48) >= 270.0f) {
                PSMTXScale(scaleMtx, rootScale, rootScale, rootScale);
            } else {
                PSMTXScale(scaleMtx, rootScale, rootScale, -rootScale);
            }
            PSMTXConcat(translate, rotate, translate);
            PSMTXConcat(translate, scaleMtx, rootMtx);
            current = work;
            for (i = 1; i < *(s32*)(effect + 8); i++, current += 0x50) {
                if (((i - 1) & 3) == 0 && *(s32*)(current + 0x94) == 0 &&
                    *(s32*)(current + 0x6C) > 16) {
                    f32 particleScale = 7.0f * *(f32*)(current + 0x90);
                    if (particleScale * rootScale >= 7.0f) {
                        Vec3 origin = { 0.0f, 0.0f, 0.0f };
                        Vec3 position;
                        PSMTXTrans(translate, *(f32*)(current + 0x54),
                                  *(f32*)(current + 0x58), *(f32*)(current + 0x5C));
                        PSMTXScale(scaleMtx, particleScale, particleScale, particleScale);
                        PSMTXConcat(translate, scaleMtx, particleMtx);
                        PSMTXConcat(rootMtx, particleMtx, particleMtx);
                        PSMTXMultVec(particleMtx, &origin, &position);
                        *(void**)(kpa + 0x138) = kpaFlameHitCheck(position.x, position.y,
                                                                position.x, position.y);
                        if (*(void**)(kpa + 0x138) != NULL) {
                            break;
                        }
                    }
                }
            }
        }
    }

    mario = marioGetPtr();
    kpa = *(u8**)(mario + 0x298);
    if ((*(u32*)mario & 1) == 0 ||
        !((*(s32*)(kpa + 0x13C) >= 0x14 && *(s32*)(kpa + 0x13C) <= 0x17) ||
          (*(s32*)(kpa + 0x13C) >= 0x28 && *(s32*)(kpa + 0x13C) <= 0x2B))) {
        if (fireefp != NULL) {
            effSoftDelete(fireefp);
            fireefp = NULL;
        }
    }
    return 0;
}

f32 kpaGetBaseScale(s32 level) {
    return baseScaleTbl[level];
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

s32 kpaJumpChk(void) {
    s32 value = *(s16*)((s32)marioGetPtr() + 0x50);
    return ((u32)(-value) | (u32)value) >> 31;
}

void kpaPauseSE2(void) {
    psndSFXOn(kpaPauseSeTable[irand(0x1C)] | 0x20000);
}
