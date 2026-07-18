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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void mot_roll(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void __memFree(s32, void*);
    extern void* camGetPtr(s32);
    extern void marioAdjustMoveDir(void);
    extern void marioGetRubInit(void);
    extern void marioPaperOn(char*);
    extern void marioPaperOff(void);
    extern void marioPaperLightOff(void);
    extern void marioChgPose(char*);
    extern void marioChgPaper(char*);
    extern void marioSetSpec(void);
    extern void marioOfsRotReset(void);
    extern void marioChgMotSub(s16, s32);
    extern void allPartyRideShip(void);
    extern void allPartyRideOff(void);
    extern void camFollowYOff(void);
    extern void camFollowYOn(void);
    extern void marioUpdateCamCollision(void*);
    extern void mot_fall_roll(void);
    extern u8 roll_upstairs(void);
    extern void marioSetRollDispDir(void);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioChkBero(void*);
    extern u32 hitGetAttr(void*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void psndSFXOff(s32);
    extern f32 revise360(f32);
    extern f32 toMovedir(f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 __fabsf(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);

    extern char str_p_roll_802c4248[];
    extern char str_PM_R_1A_802c4250[];
    extern char str_PM_R_1B_802c4258[];
    extern char str_PM_R_1C_802c4260[];
    extern char str_M_I_U_80420e1c[];
    extern char str_M_S_1_80420e4c[];

    extern f32 rollData;
    extern f32 vec3_802c41b0;
    extern f32 DAT_802c41b4;
    extern f32 DAT_802c41b8;
    extern f32 vec3_802c41bc;
    extern f32 DAT_802c41c0;
    extern f32 DAT_802c41c4;
    extern f32 vec3_802c41c8;
    extern f32 DAT_802c41cc;
    extern f32 DAT_802c41d0;
    extern f32 vec3_802c41d4;
    extern f32 DAT_802c41d8;
    extern f32 DAT_802c41dc;

    extern f32 float_0_80420dd4;
    extern f32 float_0p5_80420dd8;
    extern f32 float_3p1416_80420ddc;
    extern f32 float_180_80420de0;
    extern f32 float_6_80420de4;
    extern f32 float_5_80420de8;
    extern f32 float_neg4_80420dec;
    extern f32 float_90_80420df0;
    extern f32 float_270_80420df4;
    extern f32 float_360_80420df8;
    extern f32 float_10_80420dfc;
    extern f32 float_135_80420e00;
    extern f32 float_1p2_80420e04;
    extern f32 float_14p8_80420e24;
    extern f32 float_neg6_80420e28;
    extern f32 float_1_80420e2c;
    extern f32 float_0p4_80420e30;
    extern f32 float_0p01_80420e34;
    extern f32 float_2_80420e38;
    extern f32 float_0p1_80420e3c;
    extern f32 float_3_80420e40;
    extern f32 float_neg3_80420e44;
    extern f32 float_0p004_80420e48;
    extern f32 float_1p4_80420e54;
    extern f32 float_1p45_80420e58;
    extern f32 float_0p35_80420e5c;
    extern f32 float_1p05_80420e60;
    extern f32 float_0p2_80420e64;
    extern f32 float_2p85_80420e6c;
    extern f32 float_neg0p14_80420e70;
    extern f32 float_0p08_80420e74;
    extern f32 float_neg0p02_80420e78;
    extern f32 float_neg0p03_80420e7c;
    extern f32 float_neg0p002_80420e80;
    extern f32 float_0p05_80420e84;
    extern f32 float_0p6_80420e90;
    extern f32 float_6p4_80420e94;
    extern f32 float_0p025_80420e98;
    extern f32 float_neg1_80420e9c;

    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;

    void* player = marioGetPtr();
    void* work;
    void* cam;
    s32 state;
    s32 timer;
    s32 cancelBlocked;
    f32 angle;
    f32 target;
    f32 vx;
    f32 vz;
    f32 inputX;
    f32 inputZ;
    f32 mag;
    f32 dir;
    f32 speed;
    f32 scale;
    f32 oldScale;
    f32 ftmp;

#define P32(o) (*(s32*)((s32)player + (o)))
#define PU32(o) (*(u32*)((s32)player + (o)))
#define PH16(o) (*(u16*)((s32)player + (o)))
#define PS16(o) (*(s16*)((s32)player + (o)))
#define PS8(o) (*(s8*)((s32)player + (o)))
#define PU8(o) (*(u8*)((s32)player + (o)))
#define PF(o) (*(f32*)((s32)player + (o)))
#define PVOID(o) (*(void**)((s32)player + (o)))
#define W32(o) (*(s32*)((s32)work + (o)))
#define WU32(o) (*(u32*)((s32)work + (o)))
#define WF(o) (*(f32*)((s32)work + (o)))

    if ((PU32(0x0C) & 1) != 0) {
        PU32(0x0C) &= ~1U;
        PU32(0x00) &= ~0x000F0000U;
        P32(0x2BC) = 0;
        P32(0x2C0) = 0;
        P32(0x2C4) = 0;
        P32(0x2C8) = 0;
        P32(0x2CC) = 0;

        work = __memAlloc(0, 0x18);
        *(void**)((s32)marioGetPtr() + 0x294) = work;
        work = *(void**)((s32)marioGetPtr() + 0x294);
        memset(work, 0, 0x18);
        marioGetPtr();
        work = *(void**)((s32)marioGetPtr() + 0x294);
        if (work != 0) {
            *(s32*)((s32)work + 0x14) = -1;
        }
        P32(0x44) = 0;
    }

    work = PVOID(0x294);
    state = P32(0x44);

    /* Match target's physical dispatch order: 0x14 first, then the
       low states, then 0x15/0x16 and rolling update. */
    if (state == 0x14) {
        goto state_14;
    }
    if (state >= 0x14) {
        if (state == 0x16) {
            goto state_16;
        }
        if (state >= 0x16) {
            goto state_transition_done;
        }
        goto state_15;
    }
    if (state == 1) {
        goto state_1;
    }
    if (state >= 1) {
        goto state_transition_done;
    }
    if (state >= 0) {
        goto state_0;
    }
    goto state_transition_done;

state_0:
    {
        cam = camGetPtr(8);
        *(u16*)cam |= 0x200;

        marioAdjustMoveDir();
        if (PF(0x1AC) < float_90_80420df0 || PF(0x1AC) > float_270_80420df4) {
            PU8(0x43) = 1;
            PF(0x1A4) = revise360(float_270_80420df4 - PF(0x19C));
            PF(0x1A0) = float_270_80420df4;
        } else {
            PU8(0x43) = 0;
            PF(0x1A4) = revise360(float_90_80420df0 - PF(0x19C));
            PF(0x1A0) = float_90_80420df0;
        }

        marioGetRubInit();
        PU32(0x00) |= 0x01000000U;
        P32(0x08) = 0x10;
        PU32(0x04) |= 0x108U;

        marioPaperOn(str_p_roll_802c4248);
        marioChgPose(str_M_I_U_80420e1c);
        marioChgPaper(str_PM_R_1A_802c4250);

        P32(0x48) = 0x41;
        PF(0x1B8) = float_14p8_80420e24;
        PF(0x1BC) = float_14p8_80420e24;
        PF(0x188) = rollData;

        work = PVOID(0x294);
        if (work != 0) {
            WF(0x00) = float_0_80420dd4;
            WF(0x04) = float_0_80420dd4;
            W32(0x08) = 0;
            W32(0x0C) = 0;
        }

        PF(0xB0) = float_neg6_80420e28;
        PF(0xB4) = float_6_80420de4;
        PF(0x98) = float_6_80420de4;
        PF(0x9C) = float_0_80420dd4;
        PF(0xC8) = float_1_80420e2c;
        PF(0xCC) = float_1_80420e2c;
        PF(0xD0) = float_1_80420e2c;
        PF(0x130) = float_1_80420e2c;
        PH16(0x50) = 0;
        if (PS16(0x4E) == 0) {
            PF(0x180) = float_0_80420dd4;
        }

        PF(0x148) = float_0p4_80420e30;
        PF(0x158) = float_0p01_80420e34;
        PF(0x68) = float_2_80420e38;
        PF(0x6C) = vec3_802c41b0;
        PF(0x70) = DAT_802c41b4;
        PF(0x74) = DAT_802c41b8;

        psndSFXOn_3D(0x179, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0x9F, (void*)((s32)player + 0x8C));
        P32(0x44) = 1;
    }
    goto state_transition_done;

state_1:
    {
        if (P32(0x48) == 0x36) {
            allPartyRideShip();
        }
        PF(0x9C) += float_0p1_80420e3c;
        if (PF(0x9C) > float_1_80420e2c) {
            PF(0x9C) = float_1_80420e2c;
        }
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            P32(0x44) = 0x1E;
            marioChgPose(str_M_I_U_80420e1c);
            marioChgPaper(str_PM_R_1B_802c4258);
            cam = camGetPtr(8);
            *(u16*)cam &= ~0x200;
        }
    }
    goto state_transition_done;

state_14:
    {
        PF(0x180) = float_0_80420dd4;
        PF(0xBC) = float_0_80420dd4;
        PF(0xC4) = float_0_80420dd4;
        PF(0xB0) = float_0_80420dd4;
        PF(0xB4) = float_0_80420dd4;

        angle = revise360(PF(0x1A4) - PF(0x19C));
        PF(0x98) = (angle < float_180_80420de0) ? float_3_80420e40 : float_neg3_80420e44;
        PF(0x9C) = float_0_80420dd4;
        P32(0x48) = 0x41;
        P32(0x44) = 0x15;
        PF(0x1AC) = float_0_80420dd4;

        work = PVOID(0x294);
        if (work != 0 && (u32)(W32(0x14) + 0x10000) != 0xFFFF) {
            psndSFXOff(W32(0x14));
            W32(0x14) = -1;
        }

        PF(0x148) = float_0_80420dd4;
        PF(0x158) = float_0p004_80420e48;
        marioChgPaper(str_PM_R_1C_802c4260);
        marioPaperLightOff();
        psndSFXOn_3D(0x17A, (void*)((s32)player + 0x8C));
        state = 0x15;
    }

state_15:
    if (P32(0x44) == 0x15) {
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer > 0) {
            angle = revise360(PF(0x1A4) - PF(0x19C));
            if (angle < float_180_80420de0) {
                target = float_90_80420df0;
            } else {
                target = float_270_80420df4;
            }
            PF(0x1AC) += float_0p1_80420e3c * (target - PF(0x1AC));
            if (__fabsf(target - PF(0x1AC)) < float_1_80420e2c) {
                PF(0x1AC) = target;
            }
            PF(0x1B0) = PF(0x1AC);
            goto state_transition_done;
        }

        angle = revise360(PF(0x1A4) - PF(0x19C));
        if (angle < float_180_80420de0) {
            PF(0x1AC) = float_180_80420de0;
            PF(0x1B0) = float_180_80420de0;
            PF(0x1A0) = float_90_80420df0;
            PF(0x1A4) = float_90_80420df0;
        } else {
            PF(0x1AC) = float_0_80420dd4;
            PF(0x1B0) = float_0_80420dd4;
            PF(0x1A0) = float_270_80420df4;
            PF(0x1A4) = float_270_80420df4;
        }

        marioSetSpec();
        P32(0x08) = 0;
        marioOfsRotReset();
        PU32(0x04) &= ~0x100U;
        PU32(0x04) &= ~0x8U;
        PU32(0x00) &= ~0x01000000U;
        marioPaperOff();
        marioAdjustMoveDir();
        marioChgPose(str_M_S_1_80420e4c);

        PF(0xB0) = vec3_802c41bc;
        PF(0xB4) = DAT_802c41c0;
        PF(0xB8) = DAT_802c41c4;
        PF(0x98) = vec3_802c41c8;
        PF(0x9C) = DAT_802c41cc;
        PF(0xA0) = DAT_802c41d0;
        PF(0xBC) = vec3_802c41d4;
        PF(0xC0) = DAT_802c41d8;
        PF(0xC4) = DAT_802c41dc;

        work = PVOID(0x294);
        if (work != 0) {
            __memFree(0, work);
            PVOID(0x294) = 0;
        }
        allPartyRideOff();
        cam = camGetPtr(8);
        *(u16*)cam &= ~0x200;
        P32(0x44) = 0x16;
    }

state_16:
    if (P32(0x44) == 0x16) {
        marioChgMotSub(0, 0);
        goto state_transition_done;
    }

state_transition_done:
    if (P32(0x44) <= 0x1D) {
        return;
    }

    work = PVOID(0x294);
    if (work == 0) {
        return;
    }

    /* Target has a very early rolling-upstairs escape before the normal
       B-cancel / scale / velocity update body.  A1 put this at the tail,
       which left the whole long update section physically misordered. */
    if ((PU32(0x00) & 0x00040000U) != 0) {
        roll_upstairs();
        return;
    }

    if ((PH16(0x24C) & 0x200) != 0) {
        cancelBlocked = 0;
        if ((PU32(0x2BC) & 0x200U) != 0) {
            cancelBlocked = 1;
        } else if ((PU32(0x00) & 0x08000000U) != 0) {
            cancelBlocked = 1;
        } else if (PVOID(0x1E8) == 0) {
            cancelBlocked = 1;
        } else if (marioSlitChkWallAround() != 0) {
            cancelBlocked = 1;
        }

        if (cancelBlocked == 0) {
            P32(0x44) = 0x14;
            cam = camGetPtr(8);
            *(u16*)cam |= 0x200;
            return;
        }
        PH16(0x24C) |= 0x100;
    }

    oldScale = PF(0x130);
    if (W32(0x08) != 0) {
        W32(0x08) = 0;
        W32(0x0C) = 1;
        oldScale = float_1_80420e2c;
    }
    if (W32(0x0C) == 1) {
        if (oldScale < float_1p4_80420e54) {
            oldScale += float_1_80420e2c * (float_1p45_80420e58 - oldScale);
            if (oldScale >= float_1p4_80420e54) {
                oldScale = float_1p4_80420e54;
            }
        }
        if (PH16(0x50) == 0) {
            W32(0x0C) = 2;
        }
    } else if (W32(0x0C) == 2) {
        if (oldScale > float_0p4_80420e30) {
            oldScale += float_0p4_80420e30 * (float_0p35_80420e5c - oldScale);
            if (oldScale <= float_0p4_80420e30) {
                oldScale = float_0p4_80420e30;
                W32(0x0C) = 3;
            }
        }
    } else if (W32(0x0C) == 3) {
        if (oldScale < float_1_80420e2c) {
            oldScale += float_0p5_80420dd8 * (float_1p05_80420e60 - oldScale);
            if (oldScale >= float_1_80420e2c) {
                oldScale = float_1_80420e2c;
                W32(0x0C) = 0;
            }
        }
    }
    PF(0x130) = oldScale;

    if (PVOID(0x1E8) == 0 && PVOID(0x1E4) != 0 && marioChkBero(PVOID(0x1E4)) == 0) {
        vx = WF(0x00);
        vz = WF(0x04);
        vx += float_0p2_80420e64 * -vx;
        vz += float_0p2_80420e64 * -vz;
        if (__fabsf(vx) < float_1_80420e2c) {
            vx = float_0_80420dd4;
        }
        if (__fabsf(vz) < float_1_80420e2c) {
            vz = float_0_80420dd4;
        }
        WF(0x00) = vx;
        WF(0x04) = vz;
    }

    if (PVOID(0x1E8) != 0) {
        if (((hitGetAttr(PVOID(0x1E8)) & 0xA00U) == 0) && (PH16(0x24C) & 0x100) != 0 && PH16(0x50) == 0) {
            camFollowYOff();
            PF(0x148) = float_2p85_80420e6c;
            PF(0x158) = float_neg0p14_80420e70;
            PF(0x160) = float_0p08_80420e74;
            PF(0x168) = float_neg0p02_80420e78;
            PU32(0x00) |= 0x40000U;
            PH16(0x50) = 1;
            psndSFXOn_3D(0x17C, (void*)((s32)player + 0x8C));
            PVOID(0x1F0) = PVOID(0x1E8);
            PVOID(0x1E8) = 0;
            PF(0x204) = PF(0x8C);
            PF(0x208) = PF(0x90);
            PF(0x20C) = PF(0x94);
            PF(0x210) = PF(0x8C);
            PF(0x214) = PF(0x90);
            PF(0x218) = PF(0x94);
        } else {
            PH16(0x50) = 0;
        }
        marioUpdateCamCollision((void*)((s32)player + 0x8C));
        PH16(0x50) = 0;
    }

    if ((PU32(0x00) & 0x00060000U) != 0) {
        if (PVOID(0x1F8) != 0 && PF(0x148) >= float_0_80420dd4) {
            PF(0x148) = float_neg0p03_80420e7c;
            PF(0x158) = float_neg0p02_80420e78;
            PF(0x160) = float_neg0p02_80420e78;
            PF(0x168) = float_neg0p002_80420e80;
            PU32(0x00) |= 0x20000U;
        }
        if (PH16(0x50) == 0 && PF(0x148) < float_0_80420dd4) {
            PF(0x204) = PF(0x8C);
            PF(0x208) = PF(0x90);
            PF(0x20C) = PF(0x94);
            camFollowYOn();
        }
        PH16(0x50)++;
    }

    vx = WF(0x00);
    vz = WF(0x04);
    inputX = (f32)PS8(0x252);
    inputZ = (f32)PS8(0x253);

    if ((PU32(0x00) & 0x00060000U) == 0) {
        if (inputX != float_0_80420dd4 || inputZ != float_0_80420dd4) {
            mag = distABf(float_0_80420dd4, float_0_80420dd4, inputX, inputZ);
            dir = angleABf(float_0_80420dd4, float_0_80420dd4, inputX, inputZ);
            dir = revise360(PF(0x19C) + dir);
            angle = (float_3p1416_80420ddc * dir) / float_180_80420de0;
            vx += (f32)sin(angle) * mag * float_0p025_80420e98;
            vz -= (f32)cos(angle) * mag * float_0p025_80420e98;
        } else {
            vx -= vx * float_0p05_80420e84;
            vz -= vz * float_0p05_80420e84;
            if (__fabsf(vx) < float_0p1_80420e3c) {
                vx = float_0_80420dd4;
            }
            if (__fabsf(vz) < float_0p1_80420e3c) {
                vz = float_0_80420dd4;
            }
        }
    }

    speed = rollData / float_0p05_80420e84;
    if (vx > speed) {
        vx = speed;
    } else if (vx < -speed) {
        vx = -speed;
    }
    if (vz > speed) {
        vz = speed;
    } else if (vz < -speed) {
        vz = -speed;
    }

    WF(0x00) = vx;
    WF(0x04) = vz;
    dir = angleABf(float_0_80420dd4, float_0_80420dd4, vx, -vz);
    speed = distABf(float_0_80420dd4, float_0_80420dd4, vx, -vz) * float_0p05_80420e84;

    if ((PU32(0x2BC) & 0x100) == 0 && (PU32(0x2BC) & 0x10000) == 0) {
        PF(0x180) = speed * PF(0x134);
        if (PF(0x180) < float_0p05_80420e84) {
            PF(0x180) = float_0_80420dd4;
        }
        if (PF(0x180) != float_0_80420dd4) {
            PF(0x1A4) = dir;
        }
    }

    if (PF(0x180) == float_0_80420dd4) {
        if (work != 0 && W32(0x14) != -1) {
            psndSFXOff(W32(0x14));
            W32(0x14) = -1;
        }
    } else if (work != 0 && W32(0x14) == -1) {
        W32(0x14) = psndSFXOn_3D(0x17B, (void*)((s32)player + 0x8C));
        PU32(0x2BC) |= 0x1000;
    }

    PF(0xBC) = revise360(PF(0xBC) + PF(0x180) * ((PF(0x1AC) < float_180_80420de0) ? float_1_80420e2c : float_neg1_80420e9c));
    marioSetRollDispDir();

    if (PS8(0x252) != 0 || PS8(0x253) != 0) {
        PU32(0x2BC) |= 0x4000;
    }

    if (PF(0x180) != float_0_80420dd4) {
        PF(0x148) = float_0_80420dd4;
        PF(0x158) = float_0p004_80420e48;
    }

#undef P32
#undef PU32
#undef PH16
#undef PS16
#undef PS8
#undef PU8
#undef PF
#undef PVOID
#undef W32
#undef WU32
#undef WF
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
