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
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void marioAdjustMoveDir(void);
    extern void marioGetRubInit(void);
    extern void marioPaperOn(char*);
    extern void marioChgPose(char*);
    extern void marioChgPaper(char*);
    extern void allPartyRideShip(void);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void marioChgMotSub(s32, s32);
    extern void psndSFXOff(s32);
    extern void marioPaperLightOff(void);
    extern void marioSetSpec(void);
    extern void marioOfsRotReset(void);
    extern void marioPaperOff(void);
    extern void __memFree(s32, void*);
    extern void allPartyRideOff(void);
    extern char str_p_roll_802c4248[];
    extern char str_PM_R_1A_802c4250[];
    extern char str_PM_R_1B_802c4258[];
    extern char str_PM_R_1C_802c4260[];
    void* player = marioGetPtr();
    void* work;
    s32 state;

    if ((*(u32*)((s32)player + 4) & 0x1000) != 0) {
        *(u32*)((s32)player + 4) &= ~0x1000;
        *(u32*)player &= ~0x78000;
        *(f32*)((s32)player + 0x178) = 0.0f;
        *(f32*)((s32)player + 0x17C) = 0.0f;
        work = __memAlloc(0, 0x18);
        *(void**)((s32)player + 0x234) = work;
        memset(work, 0, 0x18);
        *(s32*)((s32)work + 0x14) = -1;
        *(s32*)((s32)player + 0x44) = 0;
    }
    work = *(void**)((s32)player + 0x234);
    state = *(s32*)((s32)player + 0x44);
    if (state == 0) {
        marioAdjustMoveDir();
        if (*(f32*)((s32)player + 0x1AC) < 90.0f || *(f32*)((s32)player + 0x1AC) > 270.0f) {
            *(u8*)((s32)player + 0x43) = 1;
            *(f32*)((s32)player + 0x1A4) = 270.0f;
        } else {
            *(u8*)((s32)player + 0x43) = 0;
            *(f32*)((s32)player + 0x1A4) = 90.0f;
        }
        marioGetRubInit();
        *(u32*)player |= 0x1000000;
        *(u32*)((s32)player + 8) |= 0x1400;
        marioPaperOn(str_p_roll_802c4248);
        marioChgPose((char*)0x80420E1C);
        marioChgPaper(str_PM_R_1A_802c4250);
        *(s32*)((s32)player + 0x170) = 65;
        *(f32*)((s32)player + 0xE0) = 14.8f;
        *(f32*)((s32)player + 0xE4) = 14.8f;
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(s32*)((s32)work + 8) = 0;
        *(s32*)((s32)work + 0xC) = 0;
        psndSFXOn_3D(0x179, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0x9F, (void*)((s32)player + 0x8C));
        *(s32*)((s32)player + 0x44) = 1;
    } else if (state == 1) {
        if (*(s32*)((s32)player + 0x170) == 54) allPartyRideShip();
        *(f32*)((s32)player + 0x120) += 0.1f;
        if (*(f32*)((s32)player + 0x120) > 1.0f) *(f32*)((s32)player + 0x120) = 1.0f;
        if (--*(s32*)((s32)player + 0x170) < 1) {
            *(s32*)((s32)player + 0x44) = 30;
            marioChgPaper(str_PM_R_1B_802c4258);
        }
    } else if (state == 20) {
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(f32*)((s32)player + 0x10C) = 0.0f;
        *(s32*)((s32)player + 0x170) = 65;
        *(s32*)((s32)player + 0x44) = 21;
        marioChgPaper(str_PM_R_1C_802c4260);
        if (*(s32*)((s32)work + 0x14) != -1) {
            psndSFXOff(*(s32*)((s32)work + 0x14));
            *(s32*)((s32)work + 0x14) = -1;
        }
        *(f32*)((s32)player + 0x148) = 0.0f;
        *(f32*)((s32)player + 0x158) = 0.004f;
        marioPaperLightOff();
        psndSFXOn_3D(0x17A, (void*)((s32)player + 0x8C));
    } else if (state == 21) {
        if (--*(s32*)((s32)player + 0x170) < 1) {
            marioSetSpec();
            *(u32*)((s32)player + 8) = 0;
            marioOfsRotReset();
            *(u32*)((s32)player + 4) &= ~0x108;
            *(u32*)player &= ~0x1000000;
            marioPaperOff();
            marioAdjustMoveDir();
            if (*(void**)((s32)player + 0x234) != 0) {
                __memFree(0, *(void**)((s32)player + 0x234));
                *(void**)((s32)player + 0x234) = 0;
            }
            allPartyRideOff();
            *(s32*)((s32)player + 0x44) = 22;
        }
    } else if (state == 22) {
        marioChgMotSub(0, 0);
    }
    return 0;
}

void marioSetRollDispDir(void) {
    extern void* marioGetPtr(void);
    extern f32 revise360(f32);
    extern f32 toMovedir(f32);
    extern f32 __fabsf(f32);
    extern f32 float_0_80420dd4;
    extern f32 float_10_80420dfc;
    extern f32 float_90_80420df0;
    extern f32 float_135_80420e00;
    extern f32 float_170_80420ea8;
    extern f32 float_180_80420de0;
    extern f32 float_200_80420ea4;
    extern f32 float_270_80420df4;
    extern f32 float_360_80420df8;
    extern f32 float_1p2_80420e04;
    extern f32 float_neg180_80420ea0;

    void* mario;
    void* mario2;
    f32 view;
    f32 world;
    f32 center;
    f32 lo;
    f32 hi;
    f32 diff;
    f32 movedir;
    s32 wrap;
    s32 outside;
    f32 bias;

    mario = marioGetPtr();
    view = *(f32*)((s32)mario + 0x1A4);
    world = *(f32*)((s32)mario + 0x1A0);
    mario2 = marioGetPtr();

    center = (f32)((s32)(world * float_10_80420dfc)) / float_10_80420dfc;
    lo = center - float_135_80420e00;
    wrap = 0;
    if (lo < float_0_80420dd4) {
        lo += float_360_80420df8;
        wrap = 1;
    }
    hi = center + float_135_80420e00;
    if (hi > float_360_80420df8) {
        hi -= float_360_80420df8;
        wrap = 2;
    }
    outside = 0;
    if (wrap == 1) {
        if (hi < view && view < lo) {
            outside = 1;
        }
    } else if (wrap == 2) {
        if (hi < view && view < lo) {
            outside = 1;
        }
    } else if (wrap == 0) {
        if (view < lo || hi < view) {
            outside = 1;
        }
    }
    if (outside) {
        world = revise360(float_180_80420de0 + world);
    }

    diff = revise360(((f32)((s32)(world * float_10_80420dfc)) / float_10_80420dfc) -
                     ((f32)((s32)(*(f32*)((s32)mario2 + 0x19C) * float_10_80420dfc)) / float_10_80420dfc));
    if (diff >= float_180_80420de0) {
        if (__fabsf(world - view) > float_90_80420df0 && view < world) {
            view += float_360_80420df8;
        }
    } else if (__fabsf(world - view) > float_90_80420df0 && world < view) {
        world += float_360_80420df8;
    }

    center = (f32)((s32)(world * float_10_80420dfc)) / float_10_80420dfc;
    lo = center - float_90_80420df0;
    wrap = 0;
    if (lo < float_0_80420dd4) {
        lo += float_360_80420df8;
        wrap = 1;
    }
    hi = center + float_90_80420df0;
    if (hi > float_360_80420df8) {
        hi -= float_360_80420df8;
        wrap = 2;
    }
    outside = 0;
    if (wrap == 1) {
        if (hi < view && view < lo) {
            outside = 1;
        }
    } else if (wrap == 2) {
        if (hi < view && view < lo) {
            outside = 1;
        }
    } else if (wrap == 0) {
        if (view < lo || hi < view) {
            outside = 1;
        }
    }
    if (!outside && __fabsf(world - view) > float_180_80420de0) {
        if (world <= view) {
            view -= float_360_80420df8;
        } else {
            world -= float_360_80420df8;
        }
    }

    world = revise360((view - world) * float_1p2_80420e04 + world);
    movedir = toMovedir(*(f32*)((s32)mario + 0x1AC));
    if (*(s8*)((s32)mario + 0x43) != 0) {
        movedir = revise360(float_180_80420de0 + movedir);
    }

    center = (f32)((s32)(world * float_10_80420dfc)) / float_10_80420dfc;
    lo = center - float_135_80420e00;
    wrap = 0;
    if (lo < float_0_80420dd4) {
        lo += float_360_80420df8;
        wrap = 1;
    }
    hi = center + float_135_80420e00;
    if (hi > float_360_80420df8) {
        hi -= float_360_80420df8;
        wrap = 2;
    }
    outside = 0;
    if (wrap == 1) {
        if (hi < movedir && movedir < lo) {
            outside = 1;
        }
    } else if (wrap == 2) {
        if (hi < movedir && movedir < lo) {
            outside = 1;
        }
    } else if (wrap == 0) {
        if (movedir < lo || hi < movedir) {
            outside = 1;
        }
    }
    if (outside) {
        *(u8*)((s32)mario + 0x43) = (*(u8*)((s32)mario + 0x43) + 1) & 1;
    }

    bias = float_0_80420dd4;
    if (*(s8*)((s32)mario + 0x43) != 0) {
        bias = float_neg180_80420ea0;
    }
    *(f32*)((s32)mario + 0x1AC) = revise360((float_270_80420df4 + *(f32*)((s32)mario + 0x19C)) - world + bias);

    if (*(f32*)((s32)mario + 0x1AC) >= float_200_80420ea4) {
        bias = float_0_80420dd4;
        if (*(s8*)((s32)mario + 0x43) != 0) {
            bias = float_neg180_80420ea0;
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((float_270_80420df4 + *(f32*)((s32)mario + 0x19C)) - world + bias);
    }
    if (*(f32*)((s32)mario + 0x1AC) < float_170_80420ea8) {
        bias = float_0_80420dd4;
        if (*(s8*)((s32)mario + 0x43) != 0) {
            bias = float_neg180_80420ea0;
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((float_270_80420df4 + *(f32*)((s32)mario + 0x19C)) - world + bias);
    }
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
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern void marioPaperOn(char*);
    extern void marioChgPose(char*);
    extern void marioChgPaper(char*);
    extern void allPartyForceRideOn(void);
    extern char str_p_roll_802c4248[];
    extern char str_M_I_U_80420e1c[];
    extern char str_PM_R_1B_802c4258[];
    extern f32 float_14p8_80420e24;
    extern f32 float_0_80420dd4;
    extern f32 float_neg6_80420e28;
    extern f32 float_6_80420de4;
    extern f32 float_1_80420e2c;
    extern f32 rollData;
    void* player = marioGetPtr();
    void* work = __memAlloc(0, 0x18);
    void* current;

    current = marioGetPtr();
    *(void**)((s32)current + 0x294) = work;
    current = marioGetPtr();
    memset(*(void**)((s32)current + 0x294), 0, 0x18);
    marioGetPtr();
    current = marioGetPtr();
    if (*(void**)((s32)current + 0x294) != 0) {
        current = marioGetPtr();
        *(s32*)((s32)*(void**)((s32)current + 0x294) + 0x14) = -1;
    }

    *(u32*)((s32)player + 0xC) &= ~1;
    *(u16*)((s32)player + 0x2E) = 0x16;
    *(u32*)player |= 0x1000000;
    *(u32*)((s32)player + 8) = 0x10;
    marioPaperOn(str_p_roll_802c4248);
    marioChgPose(str_M_I_U_80420e1c);
    marioChgPaper(str_PM_R_1B_802c4258);
    *(u32*)((s32)player + 4) |= 8;
    *(u32*)((s32)player + 4) |= 0x100;
    *(s32*)((s32)player + 0x44) = 0x1E;

    player = marioGetPtr();
    *(f32*)((s32)player + 0x1B8) = float_14p8_80420e24;
    *(f32*)((s32)player + 0x1BC) = float_14p8_80420e24;
    *(f32*)((s32)player + 0x188) = rollData;
    current = marioGetPtr();
    *(f32*)*(void**)((s32)current + 0x294) = float_0_80420dd4;
    current = marioGetPtr();
    *(f32*)((s32)*(void**)((s32)current + 0x294) + 4) = float_0_80420dd4;
    *(f32*)((s32)player + 0xB0) = float_neg6_80420e28;
    *(f32*)((s32)player + 0xB4) = float_6_80420de4;
    *(f32*)((s32)player + 0x98) = float_6_80420de4;
    *(f32*)((s32)player + 0x9C) = float_0_80420dd4;
    *(f32*)((s32)player + 0xC8) = float_1_80420e2c;
    *(f32*)((s32)player + 0xCC) = float_1_80420e2c;
    *(f32*)((s32)player + 0xD0) = float_1_80420e2c;
    *(f32*)((s32)player + 0x130) = float_1_80420e2c;
    allPartyForceRideOn();
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
