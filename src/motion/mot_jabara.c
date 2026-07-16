#include "motion/mot_jabara.h"

void mot_grasp(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)mario &= ~0xF0000;
    }
}

s32 marioPriCheckJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    s32 item;

    if ((flags & 0x80000000) || (flags & 0x02100000)) {
        *(u8*)((s32)mario + 0x40) = 0;
        return 0;
    }
    item = pouchCheckItem(8);
    return ((u32)(-item) | (u32)item) >> 31;
}


void mot_jabara(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void __memFree(s32 heap, void* ptr);
    extern void mot_slit_post(void);
    extern void marioChgPose(char* pose);
    extern void marioPaperOn(char* pose);
    extern void marioPaperOff(void);
    extern void marioChgPaper(char* pose);
    extern void marioChkGnd(void);
    extern void L_allPartyRideOn(void);
    extern void allPartyRideOff(void);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern s32 marioGetRub(s32 type, void* dir, void* count, void* scratch);
    extern f64 marioChkOverheadJabara(s32* side, f32* height);
    extern s32 marioSearchHead(f64 height, void* position, f32* outHeight);
    extern void camFollowYOn(void);
    extern void camFollowYOff(void);
    extern void marioMakeJumpPara(void);
    extern void marioSetFallPara(void);
    extern f64 marioGetFallSpd(void);
    extern void* marioChkLandon(f32 speed, f32* y);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMotSub(s32 motion, s32 param);
    extern void swingMain(void);
    extern char str_M_J_1A_802c4000;
    extern char str_M_J_5A_802c402c;
    extern char str_M_J_1C_802c403c;
    extern char str_PM_J_1B_802c4014;
    extern char str_PM_J_2B_802c4024;
    extern char str_PM_J_3B_802c401c;
    extern char str_PM_J_1C_802c4034;
    extern char str_PM_J_1D_802c4044;
    extern f32 float_0_80420bd4;
    extern f32 float_4p3_80420c54;
    extern f32 float_8_80420c44;
    extern f32 float_10p45_80420c38;
    extern f32 float_neg0p84_80420c48;
    extern f32 float_0p278_80420c3c;
    extern f32 float_0p08_80420c4c;
    extern f32 float_neg0p031_80420c40;
    extern f32 float_neg0p015_80420c50;
    extern f32 float_neg6_80420c68;

    void* player = marioGetPtr();
    void* work;
    void* hit;
    f32 headY;
    f32 landY;
    f32 fall;
    s32 side;
    u8 rubDir[4];
    u8 rubCount[4];
    u8 rubScratch[4];
    s32 rub;

#define FLAGS (*(u32*)((s32)player + 0x0))
#define DISP_FLAGS (*(u32*)((s32)player + 0x4))
#define TRIG_FLAGS (*(u32*)((s32)player + 0xC))
#define BUTTONS (*(u32*)((s32)player + 0x14))
#define RELEASED (*(u32*)((s32)player + 0x18))
#define SUBMOTION (*(s32*)((s32)player + 0x44))
#define TIMER (*(s32*)((s32)player + 0x48))
#define MOTION_COUNT (*(s8*)((s32)player + 0x41))
#define CHARGE (*(s8*)((s32)player + 0x42))
#define AIR_TIMER (*(s16*)((s32)player + 0x50))
#define AIR_TIMER2 (*(s16*)((s32)player + 0x52))
#define POS ((f32*)((s32)player + 0x8C))
#define JUMP_SPEED (*(f32*)((s32)player + 0x7C))
#define JUMP_ACCEL (*(f32*)((s32)player + 0x80))
#define JUMP_JERK (*(f32*)((s32)player + 0x84))
#define JUMP_SNAP (*(f32*)((s32)player + 0x88))
#define MOT_WORK (*(void**)((s32)player + 0x294))

    if ((TRIG_FLAGS & 1) != 0) {
        TRIG_FLAGS &= ~1;
        *(u32*)((s32)player + 0x2B8) = 0;
        *(u32*)((s32)player + 0x2BC) = 0;
        *(u32*)((s32)player + 0x2C0) = 0;
        *(u32*)((s32)player + 0x2C4) = 0;
        *(u32*)((s32)player + 0x2C8) = 0;
        *(u32*)((s32)player + 0x2CC) = 0;
        *(u32*)((s32)player + 0x2D0) = 0;
        if ((DISP_FLAGS & 0x10000000) != 0) mot_slit_post();
        FLAGS &= ~0xF0000;
        SUBMOTION = 0;
        MOT_WORK = __memAlloc(0, 0x20);
        memset(MOT_WORK, 0, 0x20);
    }
    *(void**)((s32)player + 0x1F8) = 0;
    work = MOT_WORK;

    switch (SUBMOTION) {
        case 0:
            DISP_FLAGS |= 0x100;
            MOTION_COUNT = 1;
            CHARGE = 0;
            AIR_TIMER = 0;
            AIR_TIMER2 = 0;
            *(f32*)((s32)player + 0x180) = float_0_80420bd4;
            SUBMOTION = 10;
            marioChgPose(&str_M_J_1A_802c4000);
            marioPaperOn(&str_PM_J_1B_802c4014);
            marioChgPaper(&str_PM_J_1B_802c4014);
            *(f32*)((s32)player + 0x11C) = POS[0];
            *(f32*)((s32)player + 0x120) = POS[1];
            *(f32*)((s32)player + 0x124) = POS[2];
            marioChkGnd();
            if (*(u16*)((s32)player + 0x2E) != 0x14) {
                SUBMOTION = 100;
            }
            break;
        case 10:
            if (*(u16*)((s32)player + 0x2E) != 0x14) {
                SUBMOTION = 100;
                break;
            }
            L_allPartyRideOn();
            psndSFXOn_3D(0x182, POS);
            TIMER = 8;
            SUBMOTION = 11;
        case 11:
            if (--TIMER <= 0) {
                SUBMOTION = 12;
                *(u32*)work |= 0x10000;
            }
        case 12:
            headY = (f32)marioChkOverheadJabara(&side, &landY);
            rub = marioGetRub(0x100, rubDir, rubCount, rubScratch);
            if (rub == 1 && ++MOTION_COUNT > 20) MOTION_COUNT = 20;
            if (MOTION_COUNT == 2) {
                if ((*(u32*)work & 0x20000) == 0) {
                    *(u32*)work |= 0x20000;
                    psndSFXOn_3D(0x183, POS);
                }
                marioChgPaper(&str_PM_J_2B_802c4024);
                CHARGE = 2;
            } else if (MOTION_COUNT >= 3) {
                if ((*(u32*)work & 0x40000) == 0) {
                    *(u32*)work |= 0x40000;
                    psndSFXOn_3D(0x184, POS);
                }
                marioChgPaper(&str_PM_J_3B_802c401c);
                CHARGE = 3;
            }
            marioChkGnd();
            if (*(u16*)((s32)player + 0x2E) != 0x14) {
                SUBMOTION = 100;
                break;
            }
            if ((BUTTONS & 0x100) == 0 || (RELEASED & 0x100) != 0 ||
                marioSearchHead(200.0, POS, &landY) == 0) {
                camFollowYOff();
                marioChgPose(&str_M_J_5A_802c402c);
                marioChgPaper(&str_PM_J_1C_802c4034);
                psndSFXOn_3D(0x20185, POS);
                if (CHARGE == 3) {
                    JUMP_SPEED = float_10p45_80420c38;
                    JUMP_ACCEL = float_0p278_80420c3c;
                    JUMP_JERK = float_neg0p031_80420c40;
                    JUMP_SNAP = float_0_80420bd4;
                } else if (CHARGE == 2) {
                    JUMP_SPEED = float_8_80420c44;
                    JUMP_ACCEL = float_neg0p84_80420c48;
                    JUMP_JERK = float_0p08_80420c4c;
                    JUMP_SNAP = float_neg0p015_80420c50;
                } else {
                    JUMP_SPEED = float_4p3_80420c54;
                    JUMP_ACCEL = float_neg0p84_80420c48;
                    JUMP_JERK = float_0p08_80420c4c;
                    JUMP_SNAP = float_neg0p015_80420c50;
                }
                *(void**)((s32)player + 0x1F8) = *(void**)((s32)player + 0x1E8);
                *(void**)((s32)player + 0x1E8) = 0;
                SUBMOTION = 20;
            }
            break;
        case 20:
            marioMakeJumpPara();
            headY = (f32)marioChkOverheadJabara(&side, &landY);
            if (*(void**)((s32)player + 0x1F8) == 0) {
                POS[1] += JUMP_SPEED;
                if (JUMP_SPEED <= float_0_80420bd4) {
                    SUBMOTION = 30;
                }
            } else {
                POS[1] = headY;
                SUBMOTION = CHARGE == 3 ? 40 : 30;
            }
            break;
        case 30:
            marioChgPose(&str_M_J_1C_802c403c);
            marioChgPaper(&str_PM_J_1D_802c4044);
            marioSetFallPara();
            SUBMOTION = 31;
        case 31:
            fall = (f32)marioGetFallSpd();
            if (fall < float_neg6_80420c68) fall = float_neg6_80420c68;
            hit = marioChkLandon(fall, &landY);
            if (hit != 0) {
                marioPaperOff();
                *(void**)((s32)player + 0x1E8) = hit;
                POS[1] = landY;
                camFollowYOn();
                allPartyRideOff();
                DISP_FLAGS &= ~0x30000000;
                marioAdjustMoveDir();
                marioChgMotSub(0, 0);
                if (MOT_WORK != 0) {
                    __memFree(0, MOT_WORK);
                    MOT_WORK = 0;
                }
                return;
            }
            POS[1] += fall;
            break;
        case 100:
            marioPaperOff();
            if (MOT_WORK != 0) {
                __memFree(0, MOT_WORK);
                MOT_WORK = 0;
            }
            break;
    }
    if (SUBMOTION > 69) swingMain();

#undef MOT_WORK
#undef AIR_TIMER2
#undef AIR_TIMER
#undef JUMP_SNAP
#undef JUMP_JERK
#undef JUMP_ACCEL
#undef JUMP_SPEED
#undef POS
#undef CHARGE
#undef MOTION_COUNT
#undef TIMER
#undef SUBMOTION
#undef RELEASED
#undef BUTTONS
#undef TRIG_FLAGS
#undef DISP_FLAGS
#undef FLAGS
}

u8 swingMain(void) {
    extern void* marioGetPtr(void);
    extern f64 revise360(f64 angle);
    extern f64 __fabs(f64 value);
    extern f32 float_0_80420bd4;
    extern f32 float_0p1_80420bd8;
    extern f32 float_0p8_80420be4;
    extern f32 float_1p4_80420be0;
    extern f32 float_5_80420be8;
    extern f32 float_40_80420bd0;
    extern f32 float_180_80420bdc;

    void* player = marioGetPtr();
    void* mot = *(void**)((s32)player + 0x294);
    f32 previous;
    s16 timer;
    s8 state;

#define ROT (*(f32*)((s32)player + 0xBC))
#define SPEED (*(f32*)((s32)player + 0x2C8))
#define ACCEL (*(f32*)((s32)player + 0x2CC))
#define TIMER (*(s16*)((s32)player + 0x2D0))
#define STATE (*(s8*)((s32)player + 0x2D2))

    if (mot == 0) {
        return 0;
    }

    if (*(s32*)((s32)player + 0x44) < 0x5A &&
        (*(u16*)((s32)player + 0x24C) & 0x300) != 0 &&
        ((*(u32*)mot & 1) == 0)) {
        TIMER = 0;
        *(u32*)mot |= 1;
    }

    if (STATE == 1) {
        timer = TIMER;
        if (timer > 0) {
            TIMER = timer - 1;
            SPEED += *(f32*)((s32)mot + 4);
            if (SPEED >= float_40_80420bd0) {
                SPEED = float_40_80420bd0;
            }
        } else if (timer == 0) {
            SPEED += *(f32*)((s32)mot + 8);
            if (SPEED <= float_0_80420bd4) {
                SPEED = float_0_80420bd4;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 4 : 2;
            }
        }

        if ((*(u32*)mot & 1) == 0) {
            ROT = (f32)revise360(ROT + SPEED);
        } else {
            previous = ROT;
            ROT = (f32)revise360(previous + SPEED);
            if (ROT < previous) {
                SPEED = float_1p4_80420be0;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 4 : 2;
            }
        }
    }

    state = STATE;
    if (state == 4) {
        SPEED -= ACCEL;
        ROT = (f32)revise360(ROT + SPEED);
        if (ROT > float_180_80420bdc) {
            STATE = 5;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 2) {
        SPEED += ACCEL;
        ROT = (f32)revise360(ROT + SPEED);
        if (ROT < float_180_80420bdc) {
            STATE = 3;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 3) {
        SPEED -= ACCEL;
        if (SPEED <= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 4;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT + SPEED);
    } else if (state == 5) {
        SPEED += ACCEL;
        if (SPEED >= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 2;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT + SPEED);
    }

    if (STATE == 11) {
        timer = TIMER;
        if (timer > 0) {
            TIMER = timer - 1;
            SPEED += *(f32*)((s32)mot + 4);
            if (SPEED >= float_40_80420bd0) {
                SPEED = float_40_80420bd0;
            }
        } else if (timer == 0) {
            SPEED += *(f32*)((s32)mot + 8);
            if (SPEED <= float_0_80420bd4) {
                SPEED = float_1p4_80420be0;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 14 : 12;
            }
        }

        if ((*(u32*)mot & 1) == 0) {
            ROT = (f32)revise360(ROT - SPEED);
        } else {
            previous = ROT;
            ROT = (f32)revise360(previous - SPEED);
            if (previous < ROT) {
                SPEED = float_5_80420be8;
                ACCEL = float_0p1_80420bd8;
                STATE = ROT < float_180_80420bdc ? 14 : 12;
            }
        }
    }

    state = STATE;
    if (state == 14) {
        SPEED -= ACCEL;
        ROT = (f32)revise360(ROT - SPEED);
        if (ROT < float_180_80420bdc) {
            STATE = 15;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 12) {
        SPEED += ACCEL;
        ROT = (f32)revise360(ROT - SPEED);
        if (ROT > float_180_80420bdc) {
            STATE = 13;
            ACCEL += *(f32*)((s32)mot + 0x10);
        }
    } else if (state == 13) {
        SPEED -= ACCEL;
        if (SPEED <= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 14;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 0;
            }
        }
        ROT = (f32)revise360(ROT - SPEED);
    } else if (state == 15) {
        SPEED += ACCEL;
        if (SPEED >= float_0_80420bd4) {
            SPEED = float_0_80420bd4;
            STATE = 12;
            ACCEL -= *(f32*)((s32)mot + 0x10);
            if (__fabs(ROT) < float_0p8_80420be4) {
                STATE = 10;
            }
        }
        ROT = (f32)revise360(ROT - SPEED);
    }

#undef STATE
#undef TIMER
#undef ACCEL
#undef SPEED
#undef ROT
    return 0;
}

void mot_jabara_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void allPartyRideOff(void);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void marioClearJumpPara(void);
    extern f32 vec3_802c3fd0[];
    extern f32 float_0_80420bd4;
    extern f32 float_0p01_80420c00;
    extern f32 float_20_80420c04;
    extern f32 float_37_80420c08;
    typedef struct Vec { f32 x, y, z; } Vec;
    f32* data = vec3_802c3fd0;
    void* mario = marioGetPtr();
    void* mario2;
    void* mario3;

    *(u32*)((s32)mario + 0x0) &= ~0x800;
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x148) = float_0_80420bd4;
    *(f32*)((s32)mario2 + 0x158) = float_0p01_80420c00;
    if (*(u32*)((s32)mario + 0x4) & 0x40000000) {
        marioPaperOff();
    }
    allPartyRideOff();
    mario3 = marioGetPtr();
    *(f32*)((s32)mario3 + 0x1AC) = *(f32*)((s32)mario3 + 0x2BC);
    *(f32*)((s32)mario3 + 0x1B0) = *(f32*)((s32)mario3 + 0x2BC);
    *(Vec*)((s32)mario3 + 0xB0) = *(Vec*)(data + 3);
    *(Vec*)((s32)mario3 + 0x98) = *(Vec*)(data + 6);
    *(Vec*)((s32)mario3 + 0xBC) = *(Vec*)(data + 9);
    *(f32*)((s32)mario3 + 0x1B8) = float_20_80420c04;
    *(f32*)((s32)mario3 + 0x1BC) = float_37_80420c08;
    marioAdjustMoveDir();
    *(u32*)((s32)mario3 + 0x4) &= ~0x4;
    *(u32*)((s32)mario3 + 0x4) &= ~0x100;
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        void* ptr = *(void**)((s32)marioGetPtr() + 0x294);
        __memFree(0, ptr);
        *(void**)((s32)marioGetPtr() + 0x294) = 0;
    }
    if (*(u16*)((s32)mario + 0x2E) < 3 || (*(u16*)((s32)mario + 0x2E) >= 9 && *(u16*)((s32)mario + 0x2E) < 0xC)) {
        marioClearJumpPara();
    }
}

int marioChkJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);
    extern s32 strcmp(const char* a, const char* b);
    extern void marioChgPose(const char* pose);
    extern void marioVoiceGlareOff(void);
    extern void marioVoiceGlareOn(void);
    extern s32 marioGetRub(s32 buttonFlags, s32* out1, s32* out2, s32* out3);
    extern char str_M_I_Y_80420cbc[];
    extern char str_M_S_1_80420c80[];
    extern f32 float_0_80420bd4;
    void* mario = marioGetPtr();
    s32 v1;
    s32 v2;
    s32 v3;
    s32 ok;

    ok = marioPriCheckJabara();
    if (ok == 0) {
        return 0;
    }
    if (!(*(u16*)((s32)mario + 0x24A) & 0x100)) {
        if (!(*(u16*)((s32)mario + 0x24A) & 0x200)) {
            if (strcmp(*(const char**)((s32)mario + 0x18), str_M_I_Y_80420cbc) == 0 && *(u16*)((s32)mario + 0x2E) == 0) {
                marioChgPose(str_M_S_1_80420c80);
                marioVoiceGlareOff();
            }
            *(u8*)((s32)mario + 0x40) = 0;
        }
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 0x400000) && *(u16*)((s32)mario + 0x2E) == 0 &&
        !(*(u32*)((s32)mario + 0x4) & 0x40000000) && *(f32*)((s32)mario + 0x194) != float_0_80420bd4) {
        marioChgPose(str_M_I_Y_80420cbc);
        marioVoiceGlareOn();
    }
    if (marioGetRub(0x100, &v1, &v2, &v3) == 0) {
        return 0;
    }
    return v2 > 0;
}

s32 marioGetJabaraState(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 pose;
    register s32 state = 7;

    if (*(u16*)((s32)mario + 0x2E) != 0x14) {
        state = 0;
    } else {
        pose = *(s32*)((s32)mario + 0x44);
        if (pose == 0x14) {
            state = 1;
        } else if (pose >= 0x46 && pose <= 0x47) {
            state = 2;
        } else if (pose == 0x50) {
            state = 3;
        } else if (pose >= 0x28 && pose <= 0x2B) {
            state = 4;
        } else if (pose >= 0x32 && pose <= 0x34) {
            state = 5;
        } else if (pose >= 0x1E && pose <= 0x1F) {
            state = 6;
        }
    }
    return state;
}

s32 jabaraNoHitChk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);
    s32 pose;

    if (motion != 0x14) {
        return 0;
    }

    pose = *(s32*)((s32)mario + 0x44);
    if (pose >= 0x28 && pose < 0x32) {
        return 1;
    }

    return 0;
}

