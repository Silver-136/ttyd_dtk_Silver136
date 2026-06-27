#include "motion/mot_roll.h"

void* marioGetPtr(void);
void camFollowYOff(void);
s32 marioSlitChkWallAround(void);

void clrRollEvtFlag(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x2BC) &= ~0x100;
}

void setRollEvtFlag(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x2BC) |= 0x100;
}

s32 getRollEvtFlag(void) {
    s32 ret = 0;
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x1000000) && (*(u32*)((s32)mario + 0x2BC) & 0x100)) {
        ret = 1;
    }
    return ret;
}

void rollEvtJumpSetup(void) {
    void* mario = marioGetPtr();

    camFollowYOff();
    *(u32*)mario |= 0x10000;
    *(u16*)((s32)mario + 0x50) = 1;
}

s32 marioRollKeyDisable(void) {
    void* mario = marioGetPtr();
    s32 value;

    if (*(u16*)((s32)mario + 0x2E) != 0x16) {
        return 1;
    }
    value = *(s32*)((s32)mario + 0x44);
    if (value >= 0 && value <= 1) {
        return 0;
    }
    if (value >= 0x14 && value <= 0x16) {
        return 0;
    }
    return 1;
}

void marioRollFixOn(void) {
    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0x16) {
        *(u32*)((s32)mario + 0x2BC) |= 0x200;
    }
}

s32 unk_800a1748(void) {
    void* mario = marioGetPtr();

    if (*(u32*)((s32)mario + 0x2BC) & 0x200) {
        return 1;
    }
    if (*(u32*)mario & 0x08000000) {
        return 1;
    }
    if (*(void**)((s32)mario + 0x1E8) == 0) {
        return 1;
    }
    {
        s32 check = marioSlitChkWallAround();
        return ((u32)(-check) | (u32)check) >> 31;
    }
}

u8 mot_roll(void) {
    return 0;
}


u8 marioSetRollDispDir(void) {
    return 0;
}


void marioRollForceCancel(void) {
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32);
    extern f32 revise360(f32);
    extern void marioSetSpec(void);
    extern void marioPaperOff(void);
    extern void marioAdjustMoveDir(void);
    extern void marioChgPose(char*);
    extern void __memFree(s32, void*);
    extern void allPartyRideOff(void);
    extern void marioChgMotSub(s16, s32);
    extern f32 float_0_80420dd4;
    extern f32 float_180_80420de0;
    extern f32 float_90_80420df0;
    extern f32 float_270_80420df4;
    extern f32 float_0p004_80420e48;
    extern f32 vec3_802c41bc;
    extern f32 DAT_802c41c0;
    extern f32 DAT_802c41c4;
    extern f32 vec3_802c41c8;
    extern f32 DAT_802c41cc;
    extern f32 DAT_802c41d0;
    extern f32 vec3_802c41d4;
    extern f32 DAT_802c41d8;
    extern f32 DAT_802c41dc;
    extern char str_M_S_1_80420e4c;

    void* mario = marioGetPtr();
    void* tmp;
    f32 angle;

    if (*(u16*)((s32)mario + 0x2E) != 0x16) {
        return;
    }
    marioGetPtr();
    tmp = marioGetPtr();
    if (*(void**)((s32)tmp + 0x294) != 0) {
        tmp = marioGetPtr();
        if (*(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14) != -1) {
            tmp = marioGetPtr();
            psndSFXOff(*(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14));
            tmp = marioGetPtr();
            *(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14) = -1;
        }
    }
    tmp = marioGetPtr();
    *(f32*)((s32)tmp + 0x148) = float_0_80420dd4;
    *(f32*)((s32)tmp + 0x158) = float_0p004_80420e48;

    angle = revise360(*(f32*)((s32)mario + 0x1A4) - *(f32*)((s32)mario + 0x19C));
    if (angle < float_180_80420de0) {
        *(f32*)((s32)mario + 0x1AC) = float_180_80420de0;
        *(f32*)((s32)mario + 0x1B0) = float_180_80420de0;
        *(f32*)((s32)mario + 0x1A0) = float_90_80420df0;
        *(f32*)((s32)mario + 0x1A4) = float_90_80420df0;
    } else {
        *(f32*)((s32)mario + 0x1AC) = float_0_80420dd4;
        *(f32*)((s32)mario + 0x1B0) = float_0_80420dd4;
        *(f32*)((s32)mario + 0x1A0) = float_270_80420df4;
        *(f32*)((s32)mario + 0x1A4) = float_270_80420df4;
    }
    marioSetSpec();
    *(u32*)((s32)mario + 0x8) = 0;
    *(f32*)((s32)mario + 0xB0) = vec3_802c41bc;
    *(f32*)((s32)mario + 0xB4) = DAT_802c41c0;
    *(f32*)((s32)mario + 0xB8) = DAT_802c41c4;
    *(f32*)((s32)mario + 0x98) = vec3_802c41c8;
    *(f32*)((s32)mario + 0x9C) = DAT_802c41cc;
    *(f32*)((s32)mario + 0xA0) = DAT_802c41d0;
    *(f32*)((s32)mario + 0xBC) = vec3_802c41d4;
    *(f32*)((s32)mario + 0xC0) = DAT_802c41d8;
    *(f32*)((s32)mario + 0xC4) = DAT_802c41dc;
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    *(u32*)mario &= ~0x1000000;
    marioPaperOff();
    marioAdjustMoveDir();
    marioChgPose(&str_M_S_1_80420e4c);
    tmp = marioGetPtr();
    if (*(void**)((s32)tmp + 0x294) != 0) {
        tmp = marioGetPtr();
        __memFree(0, *(void**)((s32)tmp + 0x294));
        tmp = marioGetPtr();
        *(void**)((s32)tmp + 0x294) = 0;
    }
    allPartyRideOff();
    marioChgMotSub(0, 0);
}

void mot_roll_post(void) {
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32);
    extern f32 revise360(f32);
    extern void marioSetSpec(void);
    extern void marioPaperOff(void);
    extern void marioAdjustMoveDir(void);
    extern void marioChgPose(char*);
    extern void __memFree(s32, void*);
    extern void allPartyRideOff(void);
    extern f32 float_0_80420dd4;
    extern f32 float_180_80420de0;
    extern f32 float_90_80420df0;
    extern f32 float_270_80420df4;
    extern f32 float_0p004_80420e48;
    extern f32 vec3_802c41bc;
    extern f32 DAT_802c41c0;
    extern f32 DAT_802c41c4;
    extern f32 vec3_802c41c8;
    extern f32 DAT_802c41cc;
    extern f32 DAT_802c41d0;
    extern f32 vec3_802c41d4;
    extern f32 DAT_802c41d8;
    extern f32 DAT_802c41dc;
    extern char str_M_S_1_80420e4c;

    void* mario = marioGetPtr();
    void* tmp;
    f32 angle;

    marioGetPtr();
    tmp = marioGetPtr();
    if (*(void**)((s32)tmp + 0x294) != 0) {
        tmp = marioGetPtr();
        if (*(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14) != -1) {
            tmp = marioGetPtr();
            psndSFXOff(*(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14));
            tmp = marioGetPtr();
            *(s32*)(*(s32*)((s32)tmp + 0x294) + 0x14) = -1;
        }
    }
    tmp = marioGetPtr();
    *(f32*)((s32)tmp + 0x148) = float_0_80420dd4;
    *(f32*)((s32)tmp + 0x158) = float_0p004_80420e48;

    angle = revise360(*(f32*)((s32)mario + 0x1A4) - *(f32*)((s32)mario + 0x19C));
    if (angle < float_180_80420de0) {
        *(f32*)((s32)mario + 0x1AC) = float_180_80420de0;
        *(f32*)((s32)mario + 0x1B0) = float_180_80420de0;
        *(f32*)((s32)mario + 0x1A0) = float_90_80420df0;
        *(f32*)((s32)mario + 0x1A4) = float_90_80420df0;
    } else {
        *(f32*)((s32)mario + 0x1AC) = float_0_80420dd4;
        *(f32*)((s32)mario + 0x1B0) = float_0_80420dd4;
        *(f32*)((s32)mario + 0x1A0) = float_270_80420df4;
        *(f32*)((s32)mario + 0x1A4) = float_270_80420df4;
    }
    marioSetSpec();
    *(u32*)((s32)mario + 0x8) = 0;
    *(f32*)((s32)mario + 0xB0) = vec3_802c41bc;
    *(f32*)((s32)mario + 0xB4) = DAT_802c41c0;
    *(f32*)((s32)mario + 0xB8) = DAT_802c41c4;
    *(f32*)((s32)mario + 0x98) = vec3_802c41c8;
    *(f32*)((s32)mario + 0x9C) = DAT_802c41cc;
    *(f32*)((s32)mario + 0xA0) = DAT_802c41d0;
    *(f32*)((s32)mario + 0xBC) = vec3_802c41d4;
    *(f32*)((s32)mario + 0xC0) = DAT_802c41d8;
    *(f32*)((s32)mario + 0xC4) = DAT_802c41dc;
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    *(u32*)mario &= ~0x1000000;
    marioPaperOff();
    marioAdjustMoveDir();
    marioChgPose(&str_M_S_1_80420e4c);
    tmp = marioGetPtr();
    if (*(void**)((s32)tmp + 0x294) != 0) {
        tmp = marioGetPtr();
        __memFree(0, *(void**)((s32)tmp + 0x294));
        tmp = marioGetPtr();
        *(void**)((s32)tmp + 0x294) = 0;
    }
    allPartyRideOff();
}

u8 marioReInit_roll(void) {
    return 0;
}


void roll_upstairs_on(void) {
    extern void* marioGetPtr(void);
    extern void marioClearJumpPara(void);
    extern f32 float_4_80420e0c;
    extern f32 float_neg0p42_80420e10;
    extern f32 float_0p04_80420e14;
    extern f32 float_neg0p01_80420e18;
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x0) |= 0x40000;
    *(u32*)((s32)mario + 0x0) |= 0x80;
    marioClearJumpPara();
    *(s32*)((s32)mario + 0x48) = 3;
    *(f32*)((s32)mario + 0x7C) = (*(f32*)((s32)mario + 0x1C8) - *(f32*)((s32)mario + 0x90)) / (f32)*(s32*)((s32)mario + 0x48);
    *(f32*)((s32)mario + 0x7C) = float_4_80420e0c;
    *(f32*)((s32)mario + 0x80) = float_neg0p42_80420e10;
    *(f32*)((s32)mario + 0x84) = float_0p04_80420e14;
    *(f32*)((s32)mario + 0x88) = float_neg0p01_80420e18;
}

void mot_fall_roll(void) {
    extern void* marioGetPtr(void);
    extern f32 marioGetFallSpd(void);
    extern void* marioChkLandon(f32* y, f32 speed);
    extern void marioLandOn(void);
    extern f32 float_neg10_80420e08;
    extern s32 roll_flyout_flag2;
    void* mario = marioGetPtr();
    f32 speed = marioGetFallSpd();
    f32 y;
    void* hit;

    if (speed < float_neg10_80420e08) {
        speed = float_neg10_80420e08;
    }
    hit = marioChkLandon(&y, speed);
    if (hit != 0) {
        *(f32*)((s32)mario + 0x90) = y;
        *(void**)((s32)mario + 0x1E8) = hit;
        *(s32*)((s32)mario + 0x1EC) = 0;
    } else {
        *(f32*)((s32)mario + 0x90) += speed;
    }
    if (*(void**)((s32)mario + 0x1E8) != 0) {
        marioLandOn();
        roll_flyout_flag2 = 0;
    }
}

void marioResetRollSpd(void) {
    extern void* marioGetPtr(void);
    extern f32 float_0_80420dd4;
    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x16) {
        if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
            f32 zero = float_0_80420dd4;
            *(f32*)*(s32*)((s32)marioGetPtr() + 0x294) = zero;
            *(f32*)((s32)*(void**)((s32)marioGetPtr() + 0x294) + 4) = float_0_80420dd4;
        }
        *(f32*)((s32)mario + 0x180) = float_0_80420dd4;
    }
}

u8 marioForceRollAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* paper);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern char str_p_roll_802c4248[];
    extern char str_M_I_U_80420e1c[6];
    extern char str_PM_R_1B_802c4258[];
    void* mario = marioGetPtr();

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(str_p_roll_802c4248);
    marioChgPose(str_M_I_U_80420e1c);
    marioChgPaper(str_PM_R_1B_802c4258);
    *(u32*)((s32)mario + 4) |= 8;
    *(u32*)((s32)mario + 4) |= 0x100;
}

s32 marioRollCancel(void) {
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32 camId);
    void* mario = marioGetPtr();

    if (!(*(u32*)mario & 0x1000000)) {
        return 0;
    }
    if (*(u32*)((s32)mario + 0x2BC) & 0x200) {
        return 0;
    }
    *(s32*)((s32)mario + 0x44) = 0x14;
    *(u16*)camGetPtr(8) |= 0x200;
    return 1;
}


u8 roll_upstairs(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();

    *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    if (--*(s32*)((s32)mario + 0x48) <= 0) {
        *(u32*)mario &= ~0x80;
        *(u32*)mario &= ~0x40;
        *(u32*)mario &= ~0x40000;
    }
}

s32 unk_800a1454(void) {
    extern void* marioGetPtr(void);
    extern s32 marioSlitChkWallAround(void);
    extern void* camGetPtr(s32 camId);
    void* mario = marioGetPtr();
    s32 result = 0;
    s32 blocked;

    if (*(u32*)mario & 0x1000000) {
        return 0;
    }
    if (*(u32*)((s32)mario + 0x2BC) & 0x200) {
        return 0;
    }
    mario = marioGetPtr();
    if (*(u32*)((s32)mario + 0x2BC) & 0x200) {
        blocked = 1;
    } else if (*(u32*)mario & 0x20000000) {
        blocked = 1;
    } else if (*(void**)((s32)mario + 0x1E8) == 0) {
        blocked = 1;
    } else if (marioSlitChkWallAround() != 0) {
        blocked = 1;
    } else {
        blocked = 0;
    }
    if (blocked == 0) {
        *(s32*)((s32)mario + 0x44) = 0x14;
        result = 1;
        *(u16*)camGetPtr(8) |= 0x200;
    }
    return result;
}
