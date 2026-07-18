#include "motion/mot_ship.h"

extern f32 float_0_80420eb8;

u32 hitGetAttr(void* hit);
void* marioGetPtr(void);
s32 sysMsec2Frame(s32 msec);

s32 marioChkShipPlace(void* hit) {
    u32 attr = hitGetAttr(hit) & 0x3C000;
    return (u32)(-attr | attr) >> 31;
}

s32 marioChkSwirlPower(void) {
    void* mario;
    s32 value;

    marioGetPtr();
    mario = marioGetPtr();
    value = *(s32*)(*(s32*)((s32)mario + 0x294) + 0x30);
    return (u32)(-value | value) >> 31;
}

s32 N_marioShipGetDir(void) {
    marioGetPtr();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        return (*(u32*)*(s32*)((s32)marioGetPtr() + 0x294) >> 1) & 1;
    }
    return 0;
}

void marioShipSetDispDir(void) {
    f32 zero2;
    f32 zero1;
    void* mario;

    marioGetPtr();
    zero1 = float_0_80420eb8;
    mario = marioGetPtr();
    zero2 = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)mario + 0x294) + 0x14) = zero1;
    mario = marioGetPtr();
    *(f32*)(*(s32*)((s32)mario + 0x294) + 0x18) = zero2;
}

void marioSetSwirlPower(s32 msec, f32 a, f32 b) {
    marioGetPtr();
    *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30) = sysMsec2Frame(msec);
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x2C) = a;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x28) = b;
}


void mot_ship(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, u32);
    extern void __memFree(s32, void*);
    extern void* memset(void*, s32, u32);
    extern void* camGetPtr(s32);
    extern u32 hitGetAttr(void*);
    extern void* hitGetName(void*);
    extern void hitObjGetPos(void*, f32*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void psndSFXOff(s32);
    extern void marioCamZoomUp(void);
    extern void marioCamZoomOffReq2(s32);
    extern void camFollowYOff(void);
    extern void camFollowYOn(void);
    extern void marioPreJump(void);
    extern void marioMakeJumpPara(void);
    extern void marioClearJumpPara(void);
    extern void marioSetFallPara(void);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMot(s32);
    extern void marioChgPose(char*);
    extern void marioPaperOn(char*);
    extern void marioPaperOff(void);
    extern void marioChgPaper(char*);
    extern void marioSetPaperAnimeLocalTime(s32);
    extern s32 marioGetColor(void);
    extern f64 revise360(f64);
    extern f64 toMovedir(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void movePos(f64, f64, f32*, f32*);
    extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f64, f64, f64);
    extern void N_marioReloadMapOnBottomlessOff(void);
    extern void allPartyRideShip(void);
    extern void allPartyRideOff(void);
    extern void* evtEntry(void*, s32, u32);
    extern void* marioChkLandon(f32, f32*);
    extern void* effFunemizuEntry(s32, f64, f64, f64, f64, f64);
    extern void effDelete(void*);
    extern void shipMove(void);
    extern void marioCheckWallShip(f64, f64);
    extern s32 checkHarbor(f32*);
    extern s32 strcmp(char*, char*);
    extern char* paper_ship[];
    extern char str_M_J_1B_802c42c8[];
    extern char str_PM_H_1A_802c42d0[];
    extern char str_M_Z_1_80420f14[];
    extern char str_dou_03_802c42d8[];
    extern s32 sound_evt[];
    extern s32 sound_evt2[];
    extern void* gp;

    void* player;
    void* work;
    void* hit;
    void* evt;
    void* eff;
    void* cam;
    f32 pos[3];
    f32 groundY;
    f32 dir;
    f32 target;
    f32 turn;
    f32 speed;
    f32 dx;
    f32 dz;
    f32 d;
    f32 x;
    f32 z;
    f32 y;
    f32 f;
    s32 state;
    s32 timer;
    s32 side;
    s32 n;
    u32 attr;
    u32 flags;

#define P8(o) (*(u8*)((s32)player + (o)))
#define PH16(o) (*(s16*)((s32)player + (o)))
#define PU16(o) (*(u16*)((s32)player + (o)))
#define P32(o) (*(s32*)((s32)player + (o)))
#define PU32(o) (*(u32*)((s32)player + (o)))
#define PF(o) (*(f32*)((s32)player + (o)))
#define WP(o) (*(void**)((s32)work + (o)))
#define W32(o) (*(s32*)((s32)work + (o)))
#define WU32(o) (*(u32*)((s32)work + (o)))
#define WF(o) (*(f32*)((s32)work + (o)))
#define EFF(o) (*(f32*)((s32)*(void**)((s32)eff + 0x0C) + (o)))

    player = marioGetPtr();

    if ((PU32(0x0C) & 1U) != 0) {
        PU32(0x0C) &= ~1U;
        PU32(0x00) &= ~0x00078000U;
        work = __memAlloc(0, 0x48);
        *(void**)((s32)player + 0x294) = work;
        memset(work, 0, 0x48);
        W32(0x38) = -1;
        WF(0x20) = 0.0f;
        WF(0x08) = 0.0f;
        WF(0x0C) = 4.25f;
        P32(0x44) = 0;
        if ((PU32(0x0C) & 4U) == 0) {
            cam = camGetPtr(8);
            *(u16*)cam |= 0x200;
        }
    }

    work = *(void**)((s32)player + 0x294);
    if (work == 0) {
        return;
    }

    state = P32(0x44);
    switch (state) {
    case 0:
        psndSFXOn_3D(0x18D, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0x0AF, (void*)((s32)player + 0x8C));
        marioCamZoomUp();
        marioPreJump();
        marioChgPose(str_M_J_1B_802c42c8);
        camFollowYOff();
        PU32(0x04) |= 0x100;
        P32(0x48) = 0;
        P32(0x44) = 1;
        PH16(0x50) = 0;
        PH16(0x52) = 0;
        PF(0x180) = 0.0f;
        WF(0x1C) = 0.0f;
        N_marioSetBottomlessRespawnPosOnBeroEntry((f64)PF(0x8C), (f64)PF(0x90), (f64)PF(0x94));

        hit = *(void**)((s32)player + 0x1EC);
        attr = hitGetAttr(hit);
        side = 0;
        if ((attr & 0x4000) != 0) {
            hitObjGetPos(hitGetName(hit), pos);
            dir = (f32)revise360((f32)angleABf((f64)PF(0x8C), (f64)PF(0x94), (f64)pos[0], (f64)pos[2]) - PF(0x19C));
            side = (dir < 180.0f) ? 1 : 2;
        } else if ((attr & 0x8000) != 0) {
            hitObjGetPos(hitGetName(hit), pos);
            dir = (f32)revise360((f32)angleABf((f64)PF(0x8C), (f64)PF(0x94), (f64)pos[0], (f64)pos[2]) - PF(0x19C));
            side = (dir < 180.0f) ? 1 : 2;
        } else if ((attr & 0x10000) != 0) {
            side = 2;
        } else if ((attr & 0x20000) != 0) {
            side = 1;
        }
        W32(0x00) = side;
        /* fallthrough */

    case 1:
        marioMakeJumpPara();
        target = (WU32(0x00) & 1U) ? 30.0f : -30.0f;
        turn = WF(0x1C);
        turn += 0.125f * (target - turn);
        WF(0x1C) = turn;
        if ((WU32(0x00) & 1U) != 0) {
            if (WF(0x1C) >= 30.0f) {
                WF(0x1C) = 30.0f;
            }
        } else if (WF(0x1C) <= -30.0f) {
            WF(0x1C) = -30.0f;
        }
        PF(0x1AC) += WF(0x1C);
        n = 0;
        if ((WU32(0x00) & 1U) != 0) {
            if (PF(0x1AC) >= 270.0f) {
                n = 1;
            }
        } else if (PF(0x1AC) <= -270.0f) {
            n = 1;
        }
        if (n != 0) {
            P32(0x44) = 2;
            P32(0x48) = 0x14;
            marioPaperOn(paper_ship[marioGetColor()]);
            marioChgPaper(str_PM_H_1A_802c42d0);
            marioChgPose(str_M_Z_1_80420f14);
            PU32(0x04) |= 8;
            WF(0x20) = 15.0f;
            evt = evtEntry(sound_evt, 0, 0);
            *(s32*)((s32)evt + 0x9C) = (s32)(1024.0f * PF(0x8C)) - 230000000;
            *(s32*)((s32)evt + 0xA0) = (s32)(1024.0f * PF(0x90)) - 230000000;
            *(s32*)((s32)evt + 0xA4) = (s32)(1024.0f * PF(0x94)) - 230000000;
        }
        if (PF(0x7C) > 0.0f) {
            PF(0x90) += PF(0x7C);
            PU32(0x00) &= ~0x00078000U;
        }
        break;

    case 2:
        target = ((WU32(0x00) & 1U) ? 1.0f : -1.0f) * WF(0x20);
        turn = WF(0x1C);
        turn += 0.125f * (target - turn);
        WF(0x1C) = turn;
        PF(0x1AC) += turn;
        n = 0;
        if ((WU32(0x00) & 1U) != 0) {
            if (PF(0x1AC) >= 1260.0f) {
                n = 1;
            }
        } else if (PF(0x1AC) <= -1080.0f) {
            n = 1;
        }
        if (n != 0) {
            P32(0x48) = 0x28;
            P32(0x44) = 3;
        }
        break;

    case 3:
        if (P32(0x48) == 0x0E) {
            allPartyRideShip();
        }
        if (P32(0x48) == 0x14) {
            marioCamZoomOffReq2(900);
            camFollowYOn();
        }
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            P32(0x44) = 4;
        }
        if (PF(0x9C) > -15.0f) {
            PF(0x9C) -= 1.0f;
            if (PF(0x9C) <= -15.0f) {
                PF(0x9C) = -15.0f;
            }
        }
        /* fallthrough */

    case 4:
        target = ((WU32(0x00) & 1U) ? 1.0f : -1.0f) * WF(0x20);
        turn = WF(0x1C);
        turn += 0.125f * (target - turn);
        WF(0x1C) = turn;
        PF(0x1AC) += turn;
        WF(0x20) -= 0.1f;
        if (WF(0x20) <= 1.0f) {
            WF(0x20) = 1.0f;
        }
        n = 0;
        if ((WU32(0x00) & 1U) != 0) {
            if (PF(0x1AC) >= 1620.0f) {
                PF(0x1AC) = 1620.0f;
                n = 1;
            }
        } else if (PF(0x1AC) <= -1440.0f) {
            PF(0x1AC) = -1440.0f;
            n = 1;
        }
        if (n != 0 && P32(0x44) == 4) {
            P32(0x44) = 10;
        }
        break;

    case 10:
        PU32(0x04) &= ~0x100U;
        PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
        PF(0x1B8) = 40.0f;
        PF(0x1BC) = 40.0f;
        hit = *(void**)((s32)player + 0x1EC);
        hitObjGetPos(hitGetName(hit), pos);
        x = (f32)((s32)(100.0f * pos[0] + 0.5f)) / 100.0f;
        z = (f32)((s32)(100.0f * pos[2] + 0.5f)) / 100.0f;
        WF(0x40) = PF(0x90) - 15.0f;
        attr = hitGetAttr(hit);
        side = 0;
        if ((attr & 0x4000) != 0) {
            side = 1;
        } else if ((attr & 0x8000) != 0) {
            side = 2;
        } else if ((attr & 0x10000) != 0) {
            side = 3;
        } else if ((attr & 0x20000) != 0) {
            side = 4;
        }
        if (side == 1) {
            WF(0x3C) = x;
            WF(0x44) = (z - 50.0f) - 30.303f;
        } else if (side == 2) {
            WF(0x3C) = x;
            WF(0x44) = 30.303f + 50.0f + z;
        } else if (side == 3) {
            WF(0x3C) = (x - 50.0f) - 30.303f;
            WF(0x44) = z;
        } else if (side == 4) {
            WF(0x3C) = 30.303f + 50.0f + x;
            WF(0x44) = z;
        } else {
            WF(0x3C) = x;
            WF(0x44) = z;
        }
        d = (f32)distABf((f64)WF(0x3C), (f64)WF(0x44), (f64)PF(0x8C), (f64)PF(0x94));
        f = 0.1f * d;
        if (side == 1) {
            WF(0x44) -= f;
        } else if (side == 2) {
            WF(0x44) += f;
        } else if (side == 3) {
            WF(0x3C) -= f;
        } else if (side == 4) {
            WF(0x3C) += f;
        }
        PF(0x180) = (f32)distABf((f64)PF(0x8C), (f64)PF(0x94), (f64)WF(0x3C), (f64)WF(0x44)) / 36.0f;
        PF(0x1A4) = (f32)angleABf((f64)PF(0x8C), (f64)PF(0x94), (f64)WF(0x3C), (f64)WF(0x44));
        PF(0x1A0) = PF(0x1A4);
        WF(0x04) = WF(0x40);
        PF(0x7C) = 0.0f;
        PF(0x80) = ((WF(0x40) - PF(0x90)) * 2.0f) / 306.0f;
        PF(0x84) = 0.0f;
        PF(0x88) = 0.0f;
        P32(0x48) = 0x24;
        P32(0x44) = 0x0B;
        break;

    case 0x0B:
        movePos((f64)PF(0x180), (f64)PF(0x1A4), (f32*)((s32)player + 0x8C), (f32*)((s32)player + 0x94));
        PF(0x90) += PF(0x7C);
        marioMakeJumpPara();
        speed = PF(0x7C);
        PF(0x1B8) = 20.0f;
        hit = marioChkLandon(speed, &groundY);
        PF(0x1B8) = 40.0f;
        if (hit != 0) {
            y = PF(0x90) - groundY;
            if (y <= 0.0f || y + speed <= 0.0f) {
                *(void**)((s32)player + 0x1E8) = hit;
                PF(0x90) = groundY;
                PU32(0x00) &= ~0x00078000U;
                d = (f32)distABf((f64)PF(0x8C), (f64)PF(0x94), (f64)PF(0x11C), (f64)PF(0x124));
                if (d < 0.0f) {
                    d = -d;
                }
                PF(0x128) = d;
                PU32(0x00) &= ~0x40U;
                PH16(0x50) = 0;
                PH16(0x52) = 0;
                camFollowYOn();
                PF(0x7C) = 0.0f;
                PF(0x80) = 0.0f;
                PF(0x84) = 0.0f;
                PF(0x88) = 0.0f;
                PF(0x180) = 0.0f;
                WF(0x1C) = 0.0f;
                PU32(0x00) &= ~0x00078000U;
                PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
                PF(0x1B0) = PF(0x1AC);
                PF(0x1A4) = (f32)toMovedir((f64)PF(0x1AC));
                PF(0x1A0) = PF(0x1A4);
                PU32(0x04) &= ~0x100U;
                PU32(0x04) |= 0x1000;
                psndSFXOn_3D(0x18F, (void*)((s32)player + 0x8C));
                eff = effFunemizuEntry(0, 0.0, 0.0, 0.0, 0.0, 0.0);
                WP(0x24) = eff;
                if (eff != 0) {
                    *(s32*)((s32)*(void**)((s32)eff + 0x0C) + 0x48) = P32(0x134);
                }
                WF(0x08) = 0.0f;
                WF(0x0C) = 4.25f;
                P32(0x44) = 0x32;
                if ((PU32(0x0C) & 4U) == 0) {
                    cam = camGetPtr(8);
                    *(u16*)cam &= ~0x200;
                }
            }
        }
        break;

    case 0x1E:
        if ((PU32(0x0C) & 4U) == 0) {
            cam = camGetPtr(8);
            *(u16*)cam |= 0x200;
        }
        P32(0x44) = 0x1F;
        /* fallthrough */

    case 0x1F:
        PF(0x9C) -= 0.4f;
        if (PF(0x9C) <= -18.0f) {
            P32(0x48) = 4;
            P32(0x44) = 0x20;
        }
        eff = WP(0x24);
        if (eff != 0) {
            EFF(0x04) = PF(0x8C);
            EFF(0x08) = PF(0x90);
            EFF(0x0C) = PF(0x94);
            EFF(0x10) = 1.6f;
            EFF(0x14) = PF(0x1AC);
        }
        break;

    case 0x20:
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer > 0) {
            break;
        }
        P32(0x44) = 0x28;
        /* fallthrough */

    case 0x28:
        if (work != 0 && ((u32)(W32(0x38) + 0x10000) != 0xFFFFU)) {
            psndSFXOff(W32(0x38));
            W32(0x38) = -1;
        }
        camFollowYOff();
        PU32(0x04) &= ~0x1000U;
        WF(0x04) = WF(0x40);
        PF(0x180) = 2.8f;
        d = (f32)distABf((f64)PF(0x8C), (f64)PF(0x94), (f64)WF(0x3C), (f64)WF(0x44));
        n = (s32)((10.0f * ((2.0f * d) / PF(0x180)) + 0.5f) / 10.0f);
        PF(0x1A4) = (f32)angleABf((f64)PF(0x8C), (f64)PF(0x94), (f64)WF(0x3C), (f64)WF(0x44));
        PF(0x1A0) = PF(0x1A4);
        W32(0x00) = ((PU32(0x0C) >> 9) & 1) + 1;
        f = 0.5f * (f32)n;
        y = 80.0f / ((f * f) - f);
        PF(0x7C) = y * f;
        PF(0x80) = -y;
        PF(0x84) = 0.0f;
        PF(0x88) = 0.0f;
        P32(0x48) = (s16)n;
        P32(0x44) = 0x29;
        PF(0x2C8) = (PF(0x9C) < 0.0f ? -PF(0x9C) : PF(0x9C)) / (f32)(P32(0x48) + 0x58);
        if (WP(0x24) != 0) {
            effDelete(WP(0x24));
            WP(0x24) = 0;
        }
        break;

    case 0x29:
        movePos((f64)PF(0x180), (f64)PF(0x1A4), (f32*)((s32)player + 0x8C), (f32*)((s32)player + 0x94));
        PF(0x90) += PF(0x7C);
        marioMakeJumpPara();
        PF(0x9C) += PF(0x2C8);
        if (PF(0x9C) > 0.0f) {
            PF(0x9C) = 0.0f;
        }
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer < 1) {
            marioChgPaper(str_PM_H_1A_802c42d0);
            marioChgPose(str_M_Z_1_80420f14);
            P32(0x2BC) = 0x58;
            marioSetPaperAnimeLocalTime(P32(0x2BC));
            WF(0x1C) = 0.0f;
            PU32(0x04) |= 0x100;
            marioAdjustMoveDir();
            P32(0x44) = 0x2B;
            psndSFXOn_3D(0x192, (void*)((s32)player + 0x8C));
            evt = evtEntry(sound_evt2, 0, 0);
            *(s32*)((s32)evt + 0x9C) = (s32)(1024.0f * PF(0x8C)) - 230000000;
            *(s32*)((s32)evt + 0xA0) = (s32)(1024.0f * PF(0x90)) - 230000000;
            *(s32*)((s32)evt + 0xA4) = (s32)(1024.0f * PF(0x94)) - 230000000;
        }
        break;

    case 0x2B:
        PF(0x9C) += PF(0x2C8);
        if (PF(0x9C) > 0.0f) {
            PF(0x9C) = 0.0f;
        }
        marioSetPaperAnimeLocalTime(P32(0x2BC));
        P32(0x2BC)--;
        if (P32(0x2BC) > 0x2B) {
            target = (WU32(0x00) & 1U) ? 20.0f : -20.0f;
            turn = WF(0x1C);
            turn += 0.125f * (target - turn);
            WF(0x1C) = turn;
            if ((WU32(0x00) & 1U) != 0) {
                if (WF(0x1C) >= 20.0f) {
                    WF(0x1C) = 20.0f;
                }
            } else if (WF(0x1C) <= -20.0f) {
                WF(0x1C) = -20.0f;
            }
            PF(0x1AC) += WF(0x1C);
            if (P32(0x2BC) == 0x2C) {
                P32(0x44) = 0x2C;
            }
        }
        break;

    case 0x2C:
        PF(0x9C) += PF(0x2C8);
        if (PF(0x9C) > 0.0f) {
            PF(0x9C) = 0.0f;
        }
        marioSetPaperAnimeLocalTime(P32(0x2BC));
        timer = P32(0x2BC) - 1;
        P32(0x2BC) = timer;
        if (timer < 1) {
            P32(0x2BC) = 0;
            P32(0x44) = 0x2D;
            P32(0x48) = 6;
            PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
            if ((WU32(0x00) & 2U) != 0) {
                PF(0x1AC) -= 360.0f;
            }
        }
        PF(0x1AC) += WF(0x1C);
        break;

    case 0x2D:
        marioSetPaperAnimeLocalTime(P32(0x2BC));
        target = ((WU32(0x00) & 1U) ? 1.0f : -1.0f) * WF(0x20);
        turn = WF(0x1C);
        turn += 0.05f * (target - turn);
        WF(0x1C) = turn;
        PF(0x1AC) += turn;
        WF(0x20) -= 0.005f;
        if (WF(0x20) <= 10.0f) {
            WF(0x20) = 10.0f;
        }
        timer = P32(0x48) - 1;
        P32(0x48) = timer;
        if (timer > 0) {
            break;
        }
        /* fallthrough */

    case 0x2E:
        marioSetPaperAnimeLocalTime(P32(0x2BC));
        target = ((WU32(0x00) & 1U) ? 1.0f : -1.0f) * WF(0x20);
        turn = WF(0x1C);
        turn += 0.05f * (target - turn);
        WF(0x1C) = turn;
        PF(0x1AC) += turn;
        WF(0x20) -= 0.005f;
        if (WF(0x20) <= 10.0f) {
            WF(0x20) = 10.0f;
        }
        n = 0;
        if ((WU32(0x00) & 1U) != 0) {
            if (PF(0x1AC) >= 360.0f) {
                PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
                n = 1;
            }
        } else if (PF(0x1AC) <= -360.0f) {
            PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
            n = 1;
        }
        if (n != 0) {
            PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
            if ((WU32(0x00) & 2U) != 0) {
                PF(0x1AC) -= 360.0f;
            }
            marioPaperOff();
            PU32(0x04) &= ~8U;
            marioChgPose(str_M_J_1B_802c42c8);
            P32(0x44) = 0x2F;
            WF(0x20) = 10.0f;
        }
        break;

    case 0x2F:
        target = (WU32(0x00) & 1U) ? 4.0f : -4.0f;
        turn = WF(0x1C);
        turn += 0.05f * (target - turn);
        WF(0x1C) = turn;
        PF(0x1AC) += turn;
        n = 0;
        if ((WU32(0x00) & 1U) != 0) {
            if (PF(0x1AC) >= 540.0f) {
                PF(0x1AC) = 540.0f;
                n = 1;
            }
        } else if (PF(0x1AC) <= -360.0f) {
            PF(0x1AC) = -360.0f;
            n = 1;
        }
        if (P32(0x44) == 0x2F && n != 0) {
            PF(0x1AC) = (f32)revise360((f64)PF(0x1AC));
            PF(0x1B0) = PF(0x1AC);
            PF(0x1A0) = (f32)toMovedir((f64)PF(0x1AC));
            PF(0x1A4) = PF(0x1A0);
            PU32(0x04) &= ~0x100U;
            PF(0x1B8) = 20.0f;
            PF(0x1BC) = 37.0f;
            PF(0x7C) = 0.0f;
            PF(0x80) = 0.0f;
            PF(0x84) = 0.0f;
            PF(0x88) = 0.0f;
            PF(0x180) = 0.0f;
            PU32(0x00) &= ~0x00078000U;
            PF(0x9C) = 0.0f;
            marioChgMot(10);
            marioSetFallPara();
            allPartyRideOff();
            N_marioSetBottomlessRespawnPosOnBeroEntry((f64)PF(0x8C), (f64)PF(0x90), (f64)PF(0x94));
            N_marioReloadMapOnBottomlessOff();
            if (work != 0) {
                __memFree(0, work);
                *(void**)((s32)player + 0x294) = 0;
            }
            if ((PU32(0x0C) & 4U) == 0) {
                cam = camGetPtr(8);
                *(u16*)cam &= ~0x200;
            }
        }
        break;

    case 0x32:
        P32(0x44) = 0x34;
        /* fallthrough */

    case 0x34:
        if (*(void**)((s32)player + 0x1E8) != 0) {
            WF(0x08) = (f32)revise360((f64)(WF(0x08) - WF(0x0C)));
            PF(0x9C) = 3.0f * (f32)sin((3.1416f * WF(0x08)) / 180.0f) - 13.0f;
        }
        if ((PU32(0x00) & 0x20U) == 0) {
            if (*(void**)((s32)player + 0x1E8) == 0) {
                P8(0x252) = 0;
                P8(0x253) = 0;
            }
            shipMove();
            if ((PU32(0x00) & 0x20000U) == 0) {
                marioClearJumpPara();
            } else {
                marioMakeJumpPara();
                if (PF(0x7C) <= -7.0f) {
                    PF(0x7C) = -7.0f;
                }
                speed = PF(0x7C);
                PF(0x1B8) = 20.0f;
                hit = marioChkLandon(speed, &groundY);
                PF(0x1B8) = 40.0f;
                if (hit != 0) {
                    y = PF(0x90) - groundY;
                    if (y <= 0.0f || y + speed <= 0.0f) {
                        *(void**)((s32)player + 0x1E8) = hit;
                        PF(0x90) = groundY;
                        PU32(0x00) &= ~0x00078000U;
                        d = (f32)distABf((f64)PF(0x8C), (f64)PF(0x94), (f64)PF(0x11C), (f64)PF(0x124));
                        PF(0x128) = d < 0.0f ? -d : d;
                        PU32(0x00) &= ~0x40U;
                        PH16(0x50) = 0;
                        PH16(0x52) = 0;
                        camFollowYOn();
                    } else {
                        PF(0x90) += PF(0x7C);
                    }
                } else {
                    PF(0x90) += PF(0x7C);
                }
            }
            if (gp != 0 && strcmp((char*)((s32)gp + 0x12C), str_dou_03_802c42d8) == 0) {
                if (PF(0x90) <= 260.0f && PF(0x90) >= 50.0f) {
                    if ((f32)((s32)(10.0f * PF(0x218) + 0.5f)) / 10.0f != 0.0f) {
                        PF(0x90) += 2.0f * (f32)sin((3.1416f * PF(0x218)) / 180.0f);
                        PF(0x94) += 2.0f * -(f32)cos((3.1416f * PF(0x218)) / 180.0f);
                    }
                }
            }
            *(void**)((s32)player + 0x1E4) = 0;
            *(void**)((s32)player + 0x1E0) = 0;
            *(void**)((s32)player + 0x1F0) = 0;
            *(void**)((s32)player + 0x1F4) = 0;
            marioCheckWallShip((f64)PF(0x180), (f64)PF(0x1A4));
            if (PF(0x180) < 0.5f) {
                PF(0x180) = 0.0f;
                P32(0x44) = 0x32;
            }
            f = PF(0x180);
            if (f == 0.0f) {
                target = (WF(0x08) > 180.0f) ? 0.6f : 0.3f;
                WF(0x10) += 0.1f * (target - WF(0x10));
                f = WF(0x10);
            }
            eff = WP(0x24);
            if (eff != 0) {
                EFF(0x04) = PF(0x8C);
                EFF(0x08) = PF(0x90);
                EFF(0x0C) = PF(0x94);
                EFF(0x10) = f;
                EFF(0x14) = PF(0x1AC);
            }
            if ((PU16(0x24C) & 0x0A00) != 0) {
                if (checkHarbor((f32*)((s32)work + 0x3C)) != 0) {
                    P32(0x44) = 0x1E;
                }
            }
        } else {
            f = PF(0x180);
            if (f == 0.0f) {
                target = (WF(0x08) > 180.0f) ? 0.6f : 0.3f;
                WF(0x10) += 0.1f * (target - WF(0x10));
                f = WF(0x10);
            }
            eff = WP(0x24);
            if (eff != 0) {
                EFF(0x04) = PF(0x8C);
                EFF(0x08) = PF(0x90);
                EFF(0x0C) = PF(0x94);
                EFF(0x10) = f;
                EFF(0x14) = PF(0x1AC);
            }
        }
        break;
    }

#undef P8
#undef PH16
#undef PU16
#undef P32
#undef PU32
#undef PF
#undef WP
#undef W32
#undef WU32
#undef WF
#undef EFF
}

void shipMove(void) {
    extern void* marioGetPtr(void);
    extern void movePos(f32, f32, f32*, f32*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern u32 psndSFXOn_3D(void*, void*);
    extern void psndSFXOff(s32);
    extern void psndSFX_pos(s32, void*);
    extern f32 __fabsf(f32);
    extern f32 shipData[];
    extern f32 float_0_80420eb8;
    extern f32 float_180_80420ed8;
    extern f32 float_3p1416_80420edc;
    extern f32 float_1_80420ee0;
    extern f32 float_0p01_80420ee4;
    extern f32 float_0p2_80420ee8;

    void* player;
    void* ship;
    f32 stick;
    f32 x;
    f32 z;
    f32 targetX;
    f32 targetZ;
    f32 len;
    f32 max;

    player = marioGetPtr();
    stick = *(f32*)((s32)player + 0x194);
    x = float_0_80420eb8;

    if (*(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30) != 0) {
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        *(s32*)((s32)ship + 0x30) -= 1;
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        movePos(*(f32*)((s32)ship + 0x2C), *(f32*)((s32)ship + 0x28), &x, &z);
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        *(f32*)((s32)ship + 0x14) += x;
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        *(f32*)((s32)ship + 0x18) += z;
        max = shipData[4];
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        if (*(f32*)((s32)ship + 0x14) > max) {
            *(f32*)((s32)ship + 0x14) = max;
        }
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        if (*(f32*)((s32)ship + 0x14) < -max) {
            *(f32*)((s32)ship + 0x14) = -max;
        }
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        if (*(f32*)((s32)ship + 0x18) > max) {
            *(f32*)((s32)ship + 0x18) = max;
        }
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        if (*(f32*)((s32)ship + 0x18) < -max) {
            *(f32*)((s32)ship + 0x18) = -max;
        }
    }

    ship = *(void**)((s32)marioGetPtr() + 0x294);
    x = *(f32*)((s32)ship + 0x14);
    ship = *(void**)((s32)marioGetPtr() + 0x294);
    z = *(f32*)((s32)ship + 0x18);
    targetX = x;
    targetZ = z;
    if (stick != float_0_80420eb8) {
        f32 rad = (*(f32*)((s32)player + 0x198) * float_3p1416_80420edc) / float_180_80420ed8;
        targetX = stick * (f32)sin(rad);
        targetZ = stick * -(f32)cos(rad);
    }

    max = shipData[4];
    if (stick != float_0_80420eb8 || *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30) != 0) {
        if (targetX <= x) {
            x += ((targetX - float_1_80420ee0) - x) * shipData[3];
            if (x <= -max) {
                x = -max;
            }
        } else {
            x += ((targetX + float_1_80420ee0) - x) * shipData[3];
            if (x >= max) {
                x = max;
            }
        }
        if (targetZ <= z) {
            z += ((targetZ - float_1_80420ee0) - z) * shipData[3];
            if (z <= -max) {
                z = -max;
            }
        } else {
            z += ((targetZ + float_1_80420ee0) - z) * shipData[3];
            if (z >= max) {
                z = max;
            }
        }
    }

    if (stick == float_0_80420eb8) {
        f32 damp = shipData[0];
        if (*(void**)((s32)player + 0x1E0) != 0) {
            damp = shipData[1];
        }
        x = -x * __fabsf(damp) + x;
        z = -z * __fabsf(damp) + z;
        if (__fabsf(x) < float_0p01_80420ee4) {
            x = float_0_80420eb8;
        }
        if (__fabsf(z) < float_0p01_80420ee4) {
            z = float_0_80420eb8;
        }
    }

    ship = *(void**)((s32)marioGetPtr() + 0x294);
    *(f32*)((s32)ship + 0x14) = x;
    ship = *(void**)((s32)marioGetPtr() + 0x294);
    *(f32*)((s32)ship + 0x18) = z;

    len = (f32)distABf(float_0_80420eb8, float_0_80420eb8, x, z);
    if (stick == float_0_80420eb8) {
        if (len < shipData[5]) {
            ship = *(void**)((s32)marioGetPtr() + 0x294);
            *(f32*)((s32)ship + 0x14) = float_0_80420eb8;
            ship = *(void**)((s32)marioGetPtr() + 0x294);
            *(f32*)((s32)ship + 0x18) = float_0_80420eb8;
            len = float_0_80420eb8;
        }
    } else if (len >= max) {
        len = max;
    }
    *(f32*)((s32)player + 0x180) = len;
    if (len != float_0_80420eb8) {
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        *(f32*)((s32)player + 0x1A4) =
            (f32)angleABf(float_0_80420eb8, float_0_80420eb8,
                          *(f32*)((s32)ship + 0x14), *(f32*)((s32)ship + 0x18));
    }
    if (len < float_1_80420ee0) {
        if (len <= float_0p2_80420ee8) {
            ship = *(void**)((s32)marioGetPtr() + 0x294);
            if (ship != 0 && *(s32*)((s32)ship + 0x38) != -1) {
                psndSFXOff(*(s32*)((s32)ship + 0x38));
                *(s32*)((s32)ship + 0x38) = -1;
            }
        }
    } else {
        ship = *(void**)((s32)marioGetPtr() + 0x294);
        if (ship != 0 && *(s32*)((s32)ship + 0x38) == -1) {
            *(u32*)((s32)ship + 0x38) = psndSFXOn_3D((void*)0x190, (void*)((s32)player + 0x8C));
        }
    }
    ship = *(void**)((s32)marioGetPtr() + 0x294);
    if (*(s32*)((s32)ship + 0x38) != -1) {
        psndSFX_pos(*(s32*)((s32)ship + 0x38), (void*)((s32)player + 0x8C));
    }
    *(f32*)((s32)player + 0x18C) = len;
}

s32 checkHarbor(f32* outPos) {
    extern void* marioGetPtr(void);
    extern f32 toMovedir(f32);
    extern void sincosf(f32, f32*, f32*);
    extern void* hitCheckFilter(f64, f64, f64, f64, f64, f64, s32, void*, void*, void*, f32*, void*, void*, void*);
    extern u32 hitGetAttr(void*);
    extern char* hitGetName(void*);
    extern void hitObjGetPos(char*, void*);
    extern f32 float_0_80420eb8;
    extern f32 float_neg1_80420ebc;
    extern f32 float_0p5_80420ec0;
    extern f32 float_20_80420eb4;
    extern f32 float_40_80420eb0;
    extern f32 float_90_80420ec8;
    extern f32 float_100_80420ec4;

    f32 sx;
    f32 sz;
    f32 radius;
    f32 pos[3];
    f32 scratchA[3];
    f32 scratchB[3];
    f32 scratchC[3];
    void* mario;
    void* hit;
    u32 attr;
    s32 kind;
    f32 angle;

    mario = marioGetPtr();

    angle = toMovedir(*(f32*)((s32)mario + 0x1AC));
    sincosf(angle, &sx, &sz);
    radius = float_40_80420eb0;
    hit = hitCheckFilter(float_40_80420eb0 * sx + *(f32*)((s32)mario + 0x8C),
                         float_20_80420eb4 + *(f32*)((s32)mario + 0x90),
                         float_40_80420eb0 * sz + *(f32*)((s32)mario + 0x94),
                         float_0_80420eb8, float_neg1_80420ebc, float_0_80420eb8,
                         0, scratchA, scratchB, scratchC, &radius, scratchA, scratchB, scratchC);
    if (hit != 0) {
        kind = 0;
        attr = hitGetAttr(hit);
        if (attr & 0x4000) {
            kind = 1;
        } else if (attr & 0x8000) {
            kind = 2;
        } else if (attr & 0x10000) {
            kind = 3;
        } else if (attr & 0x20000) {
            kind = 4;
        }
        if (kind != 0) {
            hitObjGetPos(hitGetName(hit), pos);
            outPos[0] = (f32)((s32)((float_100_80420ec4 * pos[0] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[1] = (f32)((s32)((float_100_80420ec4 * pos[1] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[2] = (f32)((s32)((float_100_80420ec4 * pos[2] + float_0p5_80420ec0) / float_100_80420ec4));
            return 1;
        }
    }

    angle = toMovedir(float_90_80420ec8 + *(f32*)((s32)mario + 0x1AC));
    sincosf(angle, &sx, &sz);
    radius = float_40_80420eb0;
    hit = hitCheckFilter(float_40_80420eb0 * sx + *(f32*)((s32)mario + 0x8C),
                         float_20_80420eb4 + *(f32*)((s32)mario + 0x90),
                         float_40_80420eb0 * sz + *(f32*)((s32)mario + 0x94),
                         float_0_80420eb8, float_neg1_80420ebc, float_0_80420eb8,
                         0, scratchA, scratchB, scratchC, &radius, scratchA, scratchB, scratchC);
    if (hit != 0) {
        kind = 0;
        attr = hitGetAttr(hit);
        if (attr & 0x4000) {
            kind = 1;
        } else if (attr & 0x8000) {
            kind = 2;
        } else if (attr & 0x10000) {
            kind = 3;
        } else if (attr & 0x20000) {
            kind = 4;
        }
        if (kind != 0) {
            hitObjGetPos(hitGetName(hit), pos);
            outPos[0] = (f32)((s32)((float_100_80420ec4 * pos[0] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[1] = (f32)((s32)((float_100_80420ec4 * pos[1] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[2] = (f32)((s32)((float_100_80420ec4 * pos[2] + float_0p5_80420ec0) / float_100_80420ec4));
            return 1;
        }
    }

    angle = toMovedir(*(f32*)((s32)mario + 0x1AC) - float_90_80420ec8);
    sincosf(angle, &sx, &sz);
    radius = float_40_80420eb0;
    hit = hitCheckFilter(float_40_80420eb0 * sx + *(f32*)((s32)mario + 0x8C),
                         float_20_80420eb4 + *(f32*)((s32)mario + 0x90),
                         float_40_80420eb0 * sz + *(f32*)((s32)mario + 0x94),
                         float_0_80420eb8, float_neg1_80420ebc, float_0_80420eb8,
                         0, scratchA, scratchB, scratchC, &radius, scratchA, scratchB, scratchC);
    if (hit != 0) {
        kind = 0;
        attr = hitGetAttr(hit);
        if (attr & 0x4000) {
            kind = 1;
        } else if (attr & 0x8000) {
            kind = 2;
        } else if (attr & 0x10000) {
            kind = 3;
        } else if (attr & 0x20000) {
            kind = 4;
        }
        if (kind != 0) {
            hitObjGetPos(hitGetName(hit), pos);
            outPos[0] = (f32)((s32)((float_100_80420ec4 * pos[0] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[1] = (f32)((s32)((float_100_80420ec4 * pos[1] + float_0p5_80420ec0) / float_100_80420ec4));
            outPos[2] = (f32)((s32)((float_100_80420ec4 * pos[2] + float_0p5_80420ec0) / float_100_80420ec4));
            return 1;
        }
    }
    return 0;
}

void marioReInit_ship(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(void*);
    extern void marioChgPaper(void*);
    extern void marioChgPose(void*);
    extern void marioSetPaperAnimeLocalTime(u32);
    extern void* effFunemizuEntry(double, double, double, double, double, s32);
    extern void allPartyForceRideOn(void);
    extern void* paper_ship[];
    extern char str_PM_H_1A_802c42d0[];
    extern char str_M_Z_1_80420f14[];
    extern f32 float_40_80420eb0;
    extern f32 float_0_80420eb8;
    extern f32 float_4p25_80420efc;
    extern f32 float_neg15_80420f28;

    void* mario = marioGetPtr();
    void* ship;
    void* eff;
    s32 camId;

    *(u32*)((s32)mario + 0xC) &= ~1;
    ship = __memAlloc(0, 0x48);
    *(void**)((s32)marioGetPtr() + 0x294) = ship;
    memset(*(void**)((s32)marioGetPtr() + 0x294), 0, 0x48);
    *(u16*)((s32)mario + 0x2E) = 0x19;
    *(f32*)((s32)mario + 0x9C) = float_neg15_80420f28;
    *(s32*)((s32)mario + 0x44) = 0x32;
    *(f32*)((s32)mario + 0x1B4) = float_40_80420eb0;
    *(f32*)((s32)mario + 0x1B8) = float_40_80420eb0;
    marioPaperOn(paper_ship[marioGetColor()]);
    marioChgPaper(str_PM_H_1A_802c42d0);
    marioChgPose(str_M_Z_1_80420f14);
    *(u32*)((s32)mario + 0x4) |= 8;
    *(s32*)((s32)mario + 0x48) = 0x58;
    marioSetPaperAnimeLocalTime(*(u32*)((s32)mario + 0x48));
    *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38) = -1;
    *(s32*)*(s32*)((s32)marioGetPtr() + 0x294) = 0;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 4) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x14) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x18) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x1C) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x20) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 8) = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0xC) = float_4p25_80420efc;
    eff = effFunemizuEntry(float_0_80420eb8, float_0_80420eb8, float_0_80420eb8, float_0_80420eb8, float_0_80420eb8, 0);
    *(void**)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x24) = eff;
    camId = *(s32*)((s32)mario + 0x134);
    mario = marioGetPtr();
    *(s32*)(*(s32*)(*(s32*)(*(s32*)((s32)mario + 0x294) + 0x24) + 0xC) + 0x48) = camId;
    allPartyForceRideOn();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_ship_post(void) {
    extern void* marioGetPtr(void);
    extern void allPartyRideOff(void);
    extern void marioPaperOff(void);
    extern void psndSFXOff(s32 id);
    extern void marioOfsRotReset(void);
    extern void __memFree(s32 heap, void* ptr);
    extern f32 float_20_80420eb4;
    extern f32 float_37_80420f7c;
    void* mario = marioGetPtr();
    allPartyRideOff();
    if (*(u32*)((s32)mario + 0x4) & 0x40000000) {
        marioPaperOff();
    }
    {
        f32 z = float_37_80420f7c;
        f32 y = float_20_80420eb4;
        *(f32*)((s32)mario + 0x1B8) = y;
        *(f32*)((s32)mario + 0x1BC) = z;
    }
    marioGetPtr();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        s32 sound = *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(*(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38));
            *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38) = -1;
        }
    }
    *(u32*)((s32)mario + 0x4) &= ~0x4;
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    marioOfsRotReset();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        __memFree(0, *(void**)((s32)marioGetPtr() + 0x294));
        *(void**)((s32)marioGetPtr() + 0x294) = 0;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioShipChanging(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 state;
    if (*(u16*)((s32)mario + 0x2E) != 0x19) {
        return 0;
    }
    state = *(s32*)((s32)mario + 0x44);
    if (state == 10) {
        return 2;
    }
    if ((state >= 0 && state <= 12) || (state >= 0x1E && state <= 0x30)) {
        return 1;
    }
    if (state == 0x32 || state == 0x34) {
        return 3;
    }
    return 0;
}

u8 marioForceShipAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* paper);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern void* paper_ship[4];
    extern char str_PM_H_1B_802c42e0[];
    extern char str_M_Z_1_80420f14[6];
    void* mario = marioGetPtr();

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(paper_ship[marioGetColor()]);
    marioChgPaper(str_PM_H_1B_802c42e0);
    marioChgPose(str_M_Z_1_80420f14);
    *(u32*)((s32)mario + 4) |= 8;
}

u8 marioShipForceStop(void) {
    extern void* marioGetPtr(void);
    extern f32 float_0_80420eb8;
    f32 zeroB;
    f32 zeroA;
    void* ship;

    marioGetPtr();
    zeroA = float_0_80420eb8;
    ship = *(void**)((s32)marioGetPtr() + 0x294);
    zeroB = float_0_80420eb8;
    *(f32*)((s32)ship + 0x14) = zeroA;
    *(f32*)((s32)*(void**)((s32)marioGetPtr() + 0x294) + 0x18) = zeroB;
}


s32 marioChkShipMoveMode(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 mode;

    if (*(u16*)((s32)mario + 0x2E) != 0x19) {
        return 0;
    }
    mode = *(s32*)((s32)mario + 0x44);
    if (mode >= 0x32 && mode <= 0x34) {
        return 1;
    }
    return 0;
}

u8 marioShipChgFall(void) {
    extern void* marioGetPtr(void);
    extern f32 float_neg0p1_80420eec;
    extern f32 float_neg0p2_80420ef0;
    extern f32 float_0p024_80420ef4;
    extern f32 float_neg0p004_80420ef8;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;

    if (!(flags & 0x20000)) {
        *(u32*)mario = flags | 0x20000;
        *(f32*)((s32)mario + 0x7C) = float_neg0p1_80420eec;
        *(f32*)((s32)mario + 0x80) = float_neg0p2_80420ef0;
        *(f32*)((s32)mario + 0x84) = float_0p024_80420ef4;
        *(f32*)((s32)mario + 0x88) = float_neg0p004_80420ef8;
    }
}
