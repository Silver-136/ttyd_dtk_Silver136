#include "motion/mot_plane.h"

s32 marioPlaneGetFlyingDir(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    if (*(u32*)*(s32*)((s32)mario + 0x294) & 1) {
        return 1;
    }
    return -1;
}

s32 marioGetPlaneStatus(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    s32 status;

    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    status = *(s32*)((s32)mario + 0x44);
    if (status >= 0xC && status < 0x14) {
        return 2;
    }
    return 1;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void mot_plane(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void __memFree(s32, void*);
    extern void marioPreJump(void);
    extern void marioChgPose(char*);
    extern void marioMakeJumpPara(void);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(char*);
    extern void marioPaperOff(void);
    extern void marioChgPaper(char*);
    extern void allPartyRidePlane(void);
    extern void allPartyRideOff2(void);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void psndSFXOff(s32);
    extern u32 hitGetAttr(void*);
    extern void flyMain(void);
    extern void camFollowYOff(void);
    extern void camFollowYOn(void);
    extern s32 sysMsec2Frame(s32);
    extern void marioAdjustMoveDir(void);
    extern void marioSetFallPara(void);
    extern void marioClearJumpPara(void);
    extern void marioChgMot(s32);
    extern s32 marioKeyOffChk(void);
    extern void marioChkOverhead(void);
    extern void* marioSearchGround(f64, f64, f32*, void*, void*);
    extern void marioChkWallAround(f64, f64, f64, f64, void*, s32);
    extern void marioUpdateCamCollision(void*);
    extern void set_damage_root_ypos(f64);
    extern void marioSetSplash(s32, void*);
    extern f64 revise360(f64);
    extern void animPoseSetLocalTime(f64, s32);
    extern s32 marioAnimeId(void);
    extern void* evtEntry(void*, s32, s32);
    extern s32 sound_evt[];
    extern char* paper_plane[];
    extern char str_M_J_1B_802c4170[];
    extern char str_PM_P_1A_802c4178[];
    extern char str_PM_P_1B_802c4180[];
    extern char str_M_J_1C_802c41a8[];
    extern char str_M_Z_1_80420d88[];
    extern u8 str_SFX_MARIO_AIRPLANE1_802c40d8[];
    extern f32 float_0_80420d1c;
    extern f32 float_1_80420d2c;
    extern f32 float_2_80420d28;
    extern f32 float_20_80420d34;
    extern f32 float_37_80420d38;

    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;

    void* player = marioGetPtr();
    void* work;
    s32 state;
    f32 direction;
    f32 target;
    f32 zero;
    f32 one;
    u8* data;
    f32 groundY;
    u32 trig;
    void* hit;
    u32 attr;
    s32 timer;
    void* evt;

#define P32(o) (*(s32*)((s32)player + (o)))
#define PU32(o) (*(u32*)((s32)player + (o)))
#define PH16(o) (*(s16*)((s32)player + (o)))
#define PF(o) (*(f32*)((s32)player + (o)))
#define PVEC(o) (*(VecBits*)((s32)player + (o)))
#define W32(o) (*(s32*)((s32)work + (o)))
#define WU32(o) (*(u32*)((s32)work + (o)))
#define WF(o) (*(f32*)((s32)work + (o)))

    data = str_SFX_MARIO_AIRPLANE1_802c40d8;
    trig = PU32(0x0C);
    if ((trig & 1) != 0) {
        PU32(0x0C) = trig & ~1U;
        work = __memAlloc(0, 0x48);
        *(void**)((s32)player + 0x294) = work;
        memset(work, 0, 0x48);
        W32(0x28) = -1;
        P32(0x44) = 0;
    }

    work = *(void**)((s32)player + 0x294);
    state = P32(0x44);
    if (state == 0) {
        void* p2 = marioGetPtr();
        *(f32*)((s32)p2 + 0x148) = 0.4f;
        *(f32*)((s32)p2 + 0x158) = 0.01f;
        *(f32*)((s32)p2 + 0x68) = 2.0f;
        *(VecBits*)((s32)p2 + 0x6C) = *(VecBits*)(data + 0x14);
        marioPreJump();
        marioChgPose(str_M_J_1B_802c4170);
        PU32(0x04) |= 0x100;
        P32(0x44) = 1;
        PH16(0x50) = 0;
        PF(0x180) = float_0_80420d1c;
        PF(0x2BC) = float_0_80420d1c;
        PF(0x2C0) = PF(0x90);
        PF(0x2C4) = float_0_80420d1c;
        work = *(void**)((s32)player + 0x294);
        WU32(0x00) = (hitGetAttr(*(void**)((s32)player + 0x1EC)) >> 4) & 1;
        psndSFXOn_3D(0x17D, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0xAF, (void*)((s32)player + 0x8C));
        state = 1;
    }

    if (state == 1) {
        marioMakeJumpPara();
        work = *(void**)((s32)player + 0x294);
        target = (WU32(0x00) & 1) ? 30.0f : -30.0f;
        direction = PF(0x2BC);
        direction += 0.125f * (target - direction);
        PF(0x2BC) = direction;
        PF(0x1AC) += direction;
        if (((WU32(0x00) & 1) == 0 && PF(0x1AC) <= -630.0f) ||
            ((WU32(0x00) & 1) != 0 && PF(0x1AC) >= 630.0f)) {
            P32(0x44) = 2;
            P32(0x48) = 0x14;
            marioPaperOn(paper_plane[marioGetColor()]);
            marioChgPaper(str_PM_P_1A_802c4178);
            marioChgPose(str_M_Z_1_80420d88);
            PU32(0x04) |= 0x8;
            animPoseSetLocalTime((f64)float_0_80420d1c, P32(0x240));
            animPoseSetLocalTime((f64)float_0_80420d1c, *(s32*)((s32)player + 0x22C + marioAnimeId() * 4));
            evt = evtEntry(&sound_evt, 0, 0);
            *(s32*)((s32)evt + 0x9C) = (s32)(1024.0f * PF(0x8C)) - 230000000;
            *(s32*)((s32)evt + 0xA0) = (s32)(1024.0f * PF(0x90)) - 230000000;
            *(s32*)((s32)evt + 0xA4) = (s32)(1024.0f * PF(0x94)) - 230000000;
        }
        if (PF(0x160) > float_0_80420d1c) {
            PF(0x90) += PF(0x160);
        }
    } else if (state == 2) {
        animPoseSetLocalTime((f64)float_0_80420d1c, P32(0x240));
        animPoseSetLocalTime((f64)float_0_80420d1c, *(s32*)((s32)player + 0x22C + marioAnimeId() * 4));
        work = *(void**)((s32)player + 0x294);
        target = (WU32(0x00) & 1) ? 30.0f : -30.0f;
        direction = PF(0x2BC);
        direction += 0.125f * (target - direction);
        PF(0x2BC) = direction;
        PF(0x1AC) += direction;
        if (((WU32(0x00) & 1) == 0 && PF(0x1AC) <= -1080.0f) ||
            ((WU32(0x00) & 1) != 0 && PF(0x1AC) >= 1260.0f)) {
            if ((WU32(0x00) & 1) == 0) {
                PF(0x1AC) = -1080.0f;
            } else {
                PF(0x1AC) = 1260.0f;
            }
            P32(0x48) = 0x3C;
            P32(0x44) = 3;
        }
    } else if (state == 3) {
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            marioChgPaper(str_PM_P_1B_802c4180);
            work = *(void**)((s32)player + 0x294);
            WF(0x08) = (WU32(0x00) & 1) ? 90.0f : 270.0f;
            PF(0x2BC) = float_0_80420d1c;
            WF(0x04) = float_0_80420d1c;
            WF(0x0C) = float_0_80420d1c;
            WF(0x14) = float_0_80420d1c;
            WF(0x10) = float_0_80420d1c;
            if ((WU32(0x00) & 1) == 0) {
                PF(0xB0) = *(f32*)(data + 0x3C);
                PF(0xBC) = *(f32*)(data + 0x40);
            } else {
                PF(0xB0) = *(f32*)(data + 0x34);
                PF(0xBC) = *(f32*)(data + 0x38);
            }
            PF(0x1B8) = 20.0f;
            PF(0x1BC) = 12.333333f;
            PF(0x180) = 1.0f;
            WF(0x10) = 2.0f;
            P32(0x44) = 0x0C;
            ((void)marioGetPtr());
            PF(0x148) = float_0_80420d1c;
            PF(0x158) = 0.004f;
            PF(0x138) = 70.0f * ((WU32(0x00) & 1) ? -1.0f : 1.0f);
            PF(0x140) = -70.0f;
        } else if (timer == 0x28) {
            allPartyRidePlane();
        }
    }

    flyMain();

    state = P32(0x44);
    if (state == 0x14 || state == 0x1E) {
        camFollowYOff();
        PF(0x140) = float_0_80420d1c;
        PF(0x138) = float_0_80420d1c;
        marioClearJumpPara();
        PF(0x180) = float_0_80420d1c;
        work = *(void**)((s32)marioGetPtr() + 0x294);
        if (work != 0 && W32(0x28) != -1) {
            psndSFXOff(W32(0x28));
            W32(0x28) = -1;
        }
        player = marioGetPtr();
        PF(0x1B8) = float_20_80420d34;
        PF(0x1BC) = float_37_80420d38;        PVEC(0xB0) = *(VecBits*)(data + 0x50);
        PVEC(0xBC) = *(VecBits*)(data + 0x5C);
        PVEC(0x98) = *(VecBits*)(data + 0x68);
        marioPaperOff();
        marioChgPaper(0);
        PU32(0x04) &= ~0x8U;
        PU32(0x00) |= 0x80;
        marioAdjustMoveDir();
        PU32(0x04) &= ~0x100U;
        allPartyRideOff2();
        if (state == 0x14) {
            marioChgMot(4);
        } else {
            marioChgMot(0);
        }
    } else if (state == 0x28) {
        camFollowYOff();
        work = *(void**)((s32)marioGetPtr() + 0x294);
        if (work != 0 && W32(0x28) != -1) {
            psndSFXOff(W32(0x28));
            W32(0x28) = -1;
        }
        P32(0x48) = sysMsec2Frame(0x208);
        P32(0x44) = 0x29;
    } else if (state == 0x29) {
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            player = marioGetPtr();
            PF(0x1B8) = float_20_80420d34;
            PF(0x1BC) = float_37_80420d38;
            PVEC(0xB0) = *(VecBits*)(data + 0x50);
            PVEC(0xBC) = *(VecBits*)(data + 0x5C);
            PVEC(0x98) = *(VecBits*)(data + 0x68);
            marioPaperOff();
            marioChgPaper(0);
            PU32(0x04) &= ~0x8U;
            PU32(0x00) |= 0x80;
            marioAdjustMoveDir();
            PU32(0x04) &= ~0x100U;
            marioChgPose(str_M_J_1C_802c41a8);
            PF(0x90) = float_1_80420d2c + (PF(0x2C4) - float_37_80420d38);
            PF(0x180) = float_0_80420d1c;
            marioSetFallPara();
            PF(0x160) = float_0_80420d1c;
            PF(0x164) *= 0.5f;
            PF(0x168) *= 0.5f;
            PF(0x16C) *= 0.5f;
            P32(0x44) = 0x5A;
            PU32(0x04) |= 0x100U;
            allPartyRideOff2();
        }
    } else if (state == 0x32 || state == 0x33) {
        camFollowYOff();
        P32(0x48) = sysMsec2Frame((state == 0x32) ? 0x208 : 100);
        P32(0x44) = 0x34;
    } else if (state == 0x34) {
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            P32(0x44) = 0x35;
        }
    } else if (state == 0x35) {
        work = *(void**)((s32)marioGetPtr() + 0x294);
        if (work != 0 && W32(0x28) != -1) {
            psndSFXOff(W32(0x28));
            W32(0x28) = -1;
        }
        player = marioGetPtr();
        PF(0x1B8) = float_20_80420d34;
        PF(0x1BC) = float_37_80420d38;
        PVEC(0xB0) = *(VecBits*)(data + 0x50);
        PVEC(0xBC) = *(VecBits*)(data + 0x5C);
        PVEC(0x98) = *(VecBits*)(data + 0x68);
        marioPaperOff();
        marioChgPaper(0);
        PU32(0x04) &= ~0x8U;
        PU32(0x00) |= 0x80;
        marioAdjustMoveDir();
        PU32(0x04) &= ~0x100U;
        PF(0x180) = float_0_80420d1c;
        PF(0x2BC) = float_0_80420d1c;
        PF(0x90) = float_1_80420d2c + PF(0x2C4);
        marioPreJump();
        PF(0x160) *= 0.5f;
        marioChgPose(str_M_J_1B_802c4170);
        P32(0x44) = 0x50;
        psndSFXOn_3D(0x180, (void*)((s32)player + 0x8C));
        allPartyRideOff2();
    } else if (state == 0x50) {
        work = *(void**)((s32)player + 0x294);
        target = (WU32(0x00) & 1) ? 28.0f : -28.0f;
        direction = PF(0x2BC);
        direction += 0.3f * (target - direction);
        PF(0x2BC) = direction;
        PF(0x1AC) = (f32)revise360((f64)(PF(0x1AC) + direction));
        marioMakeJumpPara();
        PF(0x90) += PF(0x160);
        marioChkOverhead();
        if (PF(0x160) < float_0_80420d1c || *(void**)((s32)player + 0x1F0) != 0) {
            marioSetFallPara();
            PF(0x160) *= 0.5f;
            PF(0x164) *= 0.5f;
            PF(0x168) *= 0.5f;
            PF(0x16C) *= 0.5f;
            P32(0x44) = 0x5A;
            PU32(0x04) |= 0x100U;
            P32(0x48) = 0x14;
        }
    } else if (state == 0x3C) {
        camFollowYOff();
        P32(0x48) = sysMsec2Frame(0x208);
        P32(0x44) = 0x3D;
    } else if (state == 0x3D) {
        if (PF(0x180) <= float_1_80420d2c) {
            timer = P32(0x48) - 1;
            P32(0x48) = timer;
            if (timer < 1) {
                P32(0x44) = 0x34;
            }
        } else {
            P32(0x44) = 0x0E;
        }
    } else if (state == 0x46) {
        set_damage_root_ypos((f64)PF(0x90));
        marioSetSplash(0, (void*)((s32)player + 0x8C));
        P32(0x48) = 0x10;
        P32(0x44) = 0x47;
        PF(0x160) = -2.0f;
        PF(0x164) = float_0_80420d1c;
        PF(0x168) = float_0_80420d1c;
        PF(0x16C) = float_0_80420d1c;
        camFollowYOff();
    } else if (state == 0x47) {
        marioMakeJumpPara();
        PF(0x90) += PF(0x160);
        if (PF(0x90) < PF(0x2C4) - 100.0f) {
            P32(0x44) = 0x48;
        }
    } else if (state == 0x48) {
        PF(0x140) = float_0_80420d1c;
        PF(0x138) = float_0_80420d1c;
        marioClearJumpPara();
        work = *(void**)((s32)marioGetPtr() + 0x294);
        if (work != 0 && W32(0x28) != -1) {
            psndSFXOff(W32(0x28));
            W32(0x28) = -1;
        }
        PF(0x180) = float_0_80420d1c;
        player = marioGetPtr();
        PF(0x1B8) = float_20_80420d34;
        PF(0x1BC) = float_37_80420d38;
        PVEC(0xB0) = *(VecBits*)(data + 0x50);
        PVEC(0xBC) = *(VecBits*)(data + 0x5C);
        PVEC(0x98) = *(VecBits*)(data + 0x68);
        marioPaperOff();
        marioChgPaper(0);
        PU32(0x04) &= ~0x8U;
        PU32(0x00) |= 0x80;
        marioAdjustMoveDir();
        PU32(0x04) &= ~0x100U;
        allPartyRideOff2();
        marioSetFallPara();
        camFollowYOn();
        marioChgMot(0x0D);
    } else if (state == 0x5A) {
        if (P32(0x48) > 0) {
            work = *(void**)((s32)player + 0x294);
            target = (WU32(0x00) & 1) ? 28.0f : -28.0f;
            direction = PF(0x2BC);
            direction += 0.3f * (target - direction);
            PF(0x2BC) = direction;
            PF(0x1AC) = (f32)revise360((f64)(PF(0x1AC) + direction));
            timer = P32(0x48) - 1;
            P32(0x48) = timer;
            if (timer < 1) {
                PU32(0x04) &= ~0x100U;
            }
        }
        marioMakeJumpPara();
        if (PF(0x160) <= -20.0f) {
            PF(0x160) = -20.0f;
        }
        *(void**)((s32)player + 0x1E8) = 0;
        hit = marioSearchGround((f64)PF(0x160), (f64)PF(0x160), &groundY, (void*)((s32)player + 0x2B8), (void*)((s32)player + 0x2B4));
        if (hit == 0) {
            PF(0x90) += PF(0x160);
        } else {
            PF(0x90) = groundY;
            *(void**)((s32)player + 0x1E8) = hit;
        }
        if (*(void**)((s32)player + 0x1E8) != 0) {
            camFollowYOn();
            allPartyRideOff2();
            PH16(0x50) = 0;
            PH16(0x52) = 0;
            PU32(0x00) &= ~0x80U;
            marioAdjustMoveDir();
            player = marioGetPtr();
            PF(0x1B8) = float_20_80420d34;
            PF(0x1BC) = float_37_80420d38;
            PVEC(0xB0) = *(VecBits*)(data + 0x50);
            PVEC(0xBC) = *(VecBits*)(data + 0x5C);
            PVEC(0x98) = *(VecBits*)(data + 0x68);
            marioPaperOff();
            marioChgPaper(0);
            PU32(0x04) &= ~0x8U;
            PU32(0x00) |= 0x80;
            marioAdjustMoveDir();
            PU32(0x04) &= ~0x100U;
            work = *(void**)((s32)player + 0x294);
            if (work != 0) {
                __memFree(0, work);
                *(void**)((s32)player + 0x294) = 0;
            }
            PF(0x138) = float_0_80420d1c;
            PF(0x140) = float_0_80420d1c;
            marioChgMot(0);
        }
        marioChkWallAround((f64)float_0_80420d1c, (f64)float_0_80420d1c, (f64)float_0_80420d1c, (f64)float_0_80420d1c, (void*)((s32)player + 0x8C), 0);
    }

    {
        VecBits pos;
        pos = PVEC(0x8C);
        marioUpdateCamCollision(&pos);
    }
    PF(0x18C) = PF(0x180);

#undef P32
#undef PU32
#undef PH16
#undef PF
#undef PVEC
#undef W32
#undef WU32
#undef WF
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void flyMain(void) {
    extern void* marioGetPtr(void);
    extern f64 revise360(f64 angle);
    extern f64 sin(f64 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern void* marioSearchUnder(void);
    extern s32 chkCollision(s32* attr);
    extern u32 hitGetAttr(void* hit);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 planeData[];
    extern f32 float_0_80420d1c;
    extern f32 float_0p1_80420d40;
    extern f32 float_0p5_80420d5c;
    extern f32 float_1_80420d2c;
    extern f32 float_2_80420d28;
    extern f32 float_6_80420d60;
    extern f32 float_60_80420d3c;
    extern f32 float_90_80420d14;
    extern f32 float_150_80420d44;
    extern f32 float_180_80420d54;
    extern f32 float_210_80420d4c;
    extern f32 float_270_80420d30;
    extern f32 float_330_80420d48;
    extern f32 float_3p1416_80420d50;
    extern f32 float_neg0p001_80420d58;
    extern f32 float_neg22_80420d68;

    void* player = marioGetPtr();
    void* work = *(void**)((s32)player + 0x294);
    s32 state;
    f32 sinDir;
    f32 cosDir;
    f32 sinPitch;
    f32 cosPitch;
    f32 pitch;
    f32 speed;
    f32 lift;
    f32 input;
    f32 absInput;
    f32 tmp;
    f32 limit;
    s32 frameLimit;
    void* hit;
    s8 stick;

#define P32(o) (*(s32*)((s32)player + (o)))
#define PU32(o) (*(u32*)((s32)player + (o)))
#define PH16(o) (*(u16*)((s32)player + (o)))
#define PS8(o) (*(s8*)((s32)player + (o)))
#define PF(o) (*(f32*)((s32)player + (o)))
#define W32(o) (*(s32*)((s32)work + (o)))
#define WU32(o) (*(u32*)((s32)work + (o)))
#define WF(o) (*(f32*)((s32)work + (o)))

    state = P32(0x44);
    if (state == 0x0D) {
        goto check_flying;
    }
    if (state >= 0x0D) {
        if (state >= 0x0F) {
            goto check_flying;
        }
        goto state_0e;
    }
    if (state >= 0x0C) {
        goto state_0c;
    }
    goto check_flying;

state_0c:
    if (PF(0x2C0) >= PF(0x90)) {
        P32(0x44) = 0x0E;
    }
    player = marioGetPtr();
    work = *(void**)((s32)player + 0x294);
    if ((u32)(W32(0x28) + 0x10000) == 0xFFFFU) {
        W32(0x28) = psndSFXOn_3D(0x17F, (void*)((s32)player + 0x8C));
    }
    goto check_flying;

state_0e:
    if (PF(0x9C) <= planeData[0x26]) {
        P32(0x44) = 0x0F;
    } else {
        PF(0x9C) -= float_2_80420d28;
        PF(0xB4) += float_2_80420d28;
        PF(0x90) += float_2_80420d28;
    }

check_flying:
    if (P32(0x44) != 0x0F) {
        return;
    }

    if ((PH16(0x24C) & 0x100) != 0) {
        P32(0x44) = 0x33;
        PF(0x2C4) = PF(0x90);
        return;
    }
    if ((PH16(0x24C) & 0x200) != 0) {
        P32(0x44) = 0x33;
        PF(0x2C4) = PF(0x90);
        return;
    }

    work = *(void**)((s32)player + 0x294);
    WF(0x08) = (f32)revise360(WF(0x0C) * planeData[0x0E] + WF(0x08));

    if ((WU32(0x00) & 1) != 0) {
        pitch = WF(0x08);
        if (pitch <= float_60_80420d3c) {
            WF(0x08) = pitch + float_0p1_80420d40 * (float_60_80420d3c - pitch);
        }
        pitch = WF(0x08);
        if (pitch >= float_150_80420d44) {
            WF(0x08) = pitch + float_0p1_80420d40 * (float_150_80420d44 - pitch);
        }
    } else {
        pitch = WF(0x08);
        if (pitch >= float_330_80420d48) {
            WF(0x08) = pitch + float_0p1_80420d40 * (float_330_80420d48 - pitch);
        }
        pitch = WF(0x08);
        if (pitch <= float_210_80420d4c) {
            WF(0x08) = pitch + float_0p1_80420d40 * (float_210_80420d4c - pitch);
        }
    }

    pitch = WF(0x08);
    if (pitch >= float_90_80420d14 && pitch <= float_270_80420d30) {
        PF(0x1AC) = (f32)revise360(-pitch - float_90_80420d14);
    } else {
        PF(0x1AC) = (f32)revise360(float_270_80420d30 - pitch);
    }

    stick = PS8(0x252);
    if (stick != 0) {
        if (PF(0x180) != float_0_80420d1c) {
            input = (f32)stick;
            absInput = input;
            if (absInput < float_0_80420d1c) {
                absInput = -absInput;
            }

            if ((WU32(0x00) & 1) != 0) {
                if (stick < 0) {
                    if (WF(0x04) < planeData[0x03]) {
                        WF(0x04) += float_0p1_80420d40 * absInput * planeData[0x05];
                    } else {
                        WF(0x04) += float_0p1_80420d40 * (planeData[0x03] - WF(0x04));
                    }
                }
                if (stick > 0) {
                    if (WF(0x04) > planeData[0x04]) {
                        WF(0x04) -= float_0p1_80420d40 * absInput * planeData[0x06];
                    } else {
                        WF(0x04) = planeData[0x04];
                    }
                }
            } else {
                if (stick > 0) {
                    if (WF(0x04) < planeData[0x03]) {
                        WF(0x04) += float_0p1_80420d40 * absInput * planeData[0x05];
                    } else {
                        WF(0x04) += float_0p1_80420d40 * (planeData[0x03] - WF(0x04));
                    }
                }
                if (stick < 0) {
                    if (WF(0x04) > planeData[0x04]) {
                        WF(0x04) -= float_0p1_80420d40 * absInput * planeData[0x06];
                    } else {
                        WF(0x04) = planeData[0x04];
                    }
                }
            }
        }
    } else {
        if (WF(0x04) <= float_0_80420d1c || WF(0x14) <= float_0_80420d1c) {
            if (WF(0x14) > float_0_80420d1c && WF(0x14) < float_1_80420d2c) {
                WF(0x04) += planeData[0x1E];
            }
            speed = planeData[0] - PF(0x180);
            if (speed < float_0_80420d1c) {
                speed = float_0_80420d1c;
            }
            lift = planeData[0x1B] * (f32)sin((float_3p1416_80420d50 * speed * (float_90_80420d14 / planeData[0])) / float_180_80420d54);
            if (lift <= planeData[0x1C]) {
                lift = planeData[0x1C];
            }
            player = marioGetPtr();
            hit = *(void**)((s32)player + 0x1F4);
            if (hit != 0 && (hitGetAttr(hit) & 0x38) != 0) {
                work = *(void**)((s32)player + 0x294);
                WF(0x04) += float_neg0p001_80420d58;
                PF(0x180) += float_0p5_80420d5c;
                if (PF(0x180) > float_2_80420d28) {
                    PF(0x180) = float_2_80420d28;
                }
            } else {
                work = *(void**)((s32)player + 0x294);
                WF(0x1C) = lift;
                WF(0x04) += WF(0x1C);
            }
        }
        work = *(void**)((s32)player + 0x294);
        if (WF(0x04) <= planeData[0x04]) {
            WF(0x04) = planeData[0x04];
        }
        if (WF(0x04) >= float_6_80420d60) {
            WF(0x14) = float_0_80420d1c;
        }
    }

    PF(0x180) += PF(0x180) * planeData[0x01];
    if (WF(0x04) >= float_0_80420d1c) {
        WU32(0x00) |= 0x10;
        PF(0x180) += PF(0x180) * planeData[0x08] *
                     (f32)sin((float_3p1416_80420d50 * WF(0x04) * planeData[0x07]) / float_180_80420d54);
        if (PS8(0x252) != 0 && WF(0x04) >= planeData[0x03]) {
            PF(0x180) += planeData[0x02];
        }
        if (PF(0x180) < float_0_80420d1c) {
            PF(0x180) = float_0_80420d1c;
        }
    } else {
        WU32(0x00) &= ~0x10U;
        WF(0x10) = planeData[0x0A] *
                   (f32)sin((float_3p1416_80420d50 * planeData[0x09] * -WF(0x04)) / float_180_80420d54);
        PF(0x180) += WF(0x10);
        WF(0x24) = float_neg22_80420d68;
        if (WF(0x04) <= WF(0x24)) {
            WF(0x04) = WF(0x24);
        }
    }

    if (PF(0x180) < float_0_80420d1c) {
        PF(0x180) = float_0_80420d1c;
    }
    if (PF(0x180) > planeData[0]) {
        PF(0x180) = planeData[0];
    }

    PF(0x90) += WF(0x18) - planeData[0x1D];
    PF(0x10C) = (f32)revise360(-WF(0x04));
    PF(0x1A4) = WF(0x08);
    sincosf(PF(0x1A4), &sinDir, &cosDir);
    PF(0x94) += PF(0x180) * cosDir;
    sincosf(float_90_80420d14 + WF(0x04), &sinPitch, &cosPitch);
    tmp = planeData[0x20];
    if (cosPitch >= float_0_80420d1c) {
        tmp = planeData[0x1F];
    }
    PF(0x90) += PF(0x180) * cosPitch * tmp;
    PF(0x8C) += sinPitch * PF(0x180) * sinDir;

    if (PF(0x180) <= planeData[0x10]) {
        frameLimit = sysMsec2Frame((s32)(1000.0f * planeData[0x11]));
        if ((WU32(0x00) & 1) == 0) {
            if (PS8(0x252) > 0) {
                W32(0x20)++;
                if (W32(0x20) >= frameLimit) {
                    PF(0x2C4) = PF(0x90);
                    P32(0x44) = 0x3C;
                    return;
                }
            }
        } else {
            if (PS8(0x252) < 0) {
                W32(0x20)++;
                if (W32(0x20) >= frameLimit) {
                    PF(0x2C4) = PF(0x90);
                    P32(0x44) = 0x3C;
                    return;
                }
            }
        }
    } else {
        W32(0x20) = 0;
    }

    hit = marioSearchUnder();
    *(void**)((s32)player + 0x1F4) = hit;
    if (chkCollision((s32*)((s32)player + 0x2C4)) != 0) {
        if (*(void**)((s32)player + 0x1F0) != 0) {
            P32(0x44) = 0x28;
        } else {
            P32(0x44) = 0x32;
            hit = *(void**)((s32)player + 0x200);
            if (hit == 0) {
                hit = *(void**)((s32)player + 0x1E8);
            }
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                P32(0x44) = 0x46;
            }
        }
    }

#undef P32
#undef PU32
#undef PH16
#undef PS8
#undef PF
#undef W32
#undef WU32
#undef WF
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 chkCollision(s32* outAttr) {
    extern void* marioGetPtr(void);
    extern void sincosf(f32, f32*, f32*);
    extern void* marioHitCheck(f64, f64, f64, f64, f64, f64, void*, s32*, void*, f32*, void*, void*, void*);
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECNormalize(void*, void*);
    extern void* marioHitCheckVec(void*, void*, f32*, void*, f32*);
    extern f32 planeData[];
    extern f32 float_0_80420d1c;
    extern f32 float_1_80420d2c;
    extern f32 float_2_80420d28;
    extern f32 float_4_80420d10;
    extern f32 float_10_80420d18;
    extern f32 float_10p1_80420d20;
    extern f32 float_90_80420d14;
    extern f32 float_neg1_80420d24;

    typedef struct Vec { f32 x, y, z; } Vec;

    void* mario;
    void* plane;
    void* hit;
    Vec start;
    Vec end;
    Vec delta;
    f32 sx1, cx1;
    f32 sx2, cx2;
    f32 sx3, cx3;
    f32 radius;
    f32 scratchA[3];
    f32 scratchB[3];
    f32 scratchC[3];
    s32 attr;
    s32 hitAny;
    f32 x0;
    f32 y0;
    f32 z0;
    f32 px;
    f32 py;
    f32 pz;
    f32 f28;

    mario = marioGetPtr();
    plane = *(void**)((s32)mario + 0x294);
    hitAny = 0;

    sincosf(float_90_80420d14 + *(f32*)((s32)plane + 4), &sx1, &cx1);
    sincosf(*(f32*)((s32)plane + 8), &sx2, &cx2);
    end.x = float_4_80420d10 * sx2 * cx1 + *(f32*)((s32)mario + 0x8C) +
            *(f32*)((s32)mario + 0x98) + *(f32*)((s32)mario + 0xB0);
    end.y = -sx1 * float_4_80420d10 + *(f32*)((s32)mario + 0x90) +
            *(f32*)((s32)mario + 0x9C) + *(f32*)((s32)mario + 0xB4);
    end.z = float_4_80420d10 * cx2 * cx1 + *(f32*)((s32)mario + 0x94) +
            *(f32*)((s32)mario + 0xA0) + *(f32*)((s32)mario + 0xB8);
    *(Vec*)((s32)plane + 0x3C) = end;

    sincosf(*(f32*)((s32)plane + 0xC), &sx3, &cx3);
    sincosf(*(f32*)((s32)plane + 4), &sx1, &cx1);
    *(void**)((s32)mario + 0x1E4) = 0;
    sincosf(*(f32*)((s32)plane + 8), &sx2, &cx2);
    px = *(f32*)((s32)mario + 0x8C) + *(f32*)((s32)mario + 0x98) + *(f32*)((s32)mario + 0xB0);
    py = *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x9C) + *(f32*)((s32)mario + 0xB4);
    pz = *(f32*)((s32)mario + 0x94) + *(f32*)((s32)mario + 0xA0) + *(f32*)((s32)mario + 0xB8);
    f28 = planeData[0xA0 / 4];
    sincosf(*(f32*)((s32)plane + 4) - float_90_80420d14, &sx1, &cx1);
    start.x = f28 * (sx2 * -cx1) + px + (planeData[0x9C / 4] * cx1 * sx2 - planeData[0x9C / 4] * sx3 * cx2);
    start.y = planeData[0x9C / 4] * sx1 * cx3 + sx1 * f28 + py;
    start.z = f28 * (cx2 * -cx1) + pz + (planeData[0x9C / 4] * cx1 * cx2 - planeData[0x9C / 4] * sx3 * sx2);
    *(Vec*)((s32)plane + 0x30) = start;

    radius = float_10_80420d18 + *(f32*)((s32)mario + 0x180);
    hit = marioHitCheck(-(float_10_80420d18 * sx2 - start.x), start.y,
                        -(float_10_80420d18 * cx2 - start.z), sx2,
                        float_0_80420d1c, cx2, scratchA, &attr, scratchB,
                        &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outAttr = attr;
        hitAny = 1;
    }

    radius = *(f32*)((s32)mario + 0x180);
    hit = marioHitCheck(end.x, end.y, end.z, sx2, float_0_80420d1c, cx2,
                        scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outAttr = attr;
        hitAny = 1;
    }

    *(void**)((s32)mario + 0x1E8) = 0;
    radius = float_10p1_80420d20;
    hit = marioHitCheck(start.x, start.y + float_10_80420d18, start.z,
                        float_0_80420d1c, float_neg1_80420d24, float_0_80420d1c,
                        scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E8) = hit;
        *outAttr = attr;
        hitAny = 1;
    } else if (!hitAny) {
        radius = float_10p1_80420d20;
        hit = marioHitCheck(end.x, end.y + float_10_80420d18, end.z,
                            float_0_80420d1c, float_neg1_80420d24, float_0_80420d1c,
                            scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1E8) = hit;
            *outAttr = attr;
            hitAny = 1;
        }
    }

    if (!hitAny) {
        PSVECSubtract(&end, &start, &delta);
        radius = PSVECMag(&delta);
        PSVECNormalize(&delta, &delta);
        hit = marioHitCheckVec(&start, &delta, &x0, 0, &radius);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1E8) = hit;
            *outAttr = *(s32*)((s32)&x0 - 4);
            hitAny = 1;
        }
    }

    *(void**)((s32)mario + 0x1F0) = 0;
    if (*(void**)((s32)mario + 0x1E8) == 0) {
        radius = float_2_80420d28;
        hit = marioHitCheck(start.x, start.y, start.z,
                            float_0_80420d1c, float_1_80420d2c, float_0_80420d1c,
                            scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1F0) = hit;
            *outAttr = attr;
            hitAny = 1;
        }
    }

    return hitAny;
}

void mot_plane_post(void) {
    extern void* marioGetPtr(void);
    extern void camFollowYOn(void);
    extern void psndSFXOff(s32 id);
    extern void marioPaperOff(void);
    extern void marioChgPaper(s32 mode);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void allPartyRideOff2(void);
    extern u8 str_SFX_MARIO_AIRPLANE1_802c40d8[];
    extern f32 float_20_80420d34;
    extern f32 float_37_80420d38;
    extern f32 float_0_80420d1c;
    typedef struct Vec { u32 x, y, z; } Vec;
    u8* data = str_SFX_MARIO_AIRPLANE1_802c40d8;
    void* mario = marioGetPtr();
    void* mario2;
    void* plane;

    camFollowYOn();
    mario2 = marioGetPtr();
    plane = *(void**)((s32)mario2 + 0x294);
    if (plane != 0) {
        s32 sound = *(s32*)((s32)plane + 0x28);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)*(void**)((s32)mario2 + 0x294) + 0x28) = -1;
        }
    }
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x1B8) = float_20_80420d34;
    *(f32*)((s32)mario2 + 0x1BC) = float_37_80420d38;
    *(Vec*)((s32)mario2 + 0xBC) = *(Vec*)(data + 0x50);
    *(Vec*)((s32)mario2 + 0xB0) = *(Vec*)(data + 0x5C);
    *(Vec*)((s32)mario2 + 0x98) = *(Vec*)(data + 0x68);
    marioPaperOff();
    marioChgPaper(0);
    *(u32*)((s32)mario2 + 0x4) &= ~0x8;
    *(u32*)((s32)mario2 + 0x0) |= 0x80;
    marioAdjustMoveDir();
    *(u32*)((s32)mario2 + 0x4) &= ~0x100;
    *(f32*)((s32)mario + 0x138) = float_0_80420d1c;
    *(f32*)((s32)mario + 0x140) = float_0_80420d1c;
    plane = *(void**)((s32)mario + 0x294);
    if (plane != 0) {
        __memFree(0, plane);
        *(void**)((s32)mario + 0x294) = 0;
    }
    allPartyRideOff2();
}

u8 marioForcePlaneAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* paper);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern void* paper_plane[4];
    extern char str_PM_P_1B_802c4180[];
    extern char str_M_Z_1_80420d88[6];
    void* mario = marioGetPtr();

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(paper_plane[marioGetColor()]);
    marioChgPaper(str_PM_P_1B_802c4180);
    marioChgPose(str_M_Z_1_80420d88);
    *(u32*)((s32)mario + 4) |= 8;
}

s32 mario_plane_cancel(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 state;
    s32 result;

    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        result = 0;
    } else {
        state = *(s32*)((s32)mario + 0x44);
        if (state >= 0xC && state < 0x14) {
            *(s32*)((s32)mario + 0x44) = 0x32;
            *(f32*)((s32)mario + 0x2C4) = *(f32*)((s32)mario + 0x90);
            result = 1;
        } else {
            result = 0;
        }
    }
    return result;
}
