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


u8 mot_ship(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void marioPreJump(void);
    extern void marioMakeJumpPara(void);
    extern void marioChgPose(char*);
    extern void marioPaperOn(char*);
    extern void marioChgPaper(char*);
    extern s32 marioGetColor(void);
    extern u32 hitGetAttr(void*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void marioCamZoomUp(void);
    extern void camFollowYOff(void);
    extern void allPartyRideShip(void);
    extern char* paper_ship[];
    extern char str_M_J_1B_802c42c8[];
    extern char str_PM_H_1A_802c42d0[];
    void* player = marioGetPtr();
    void* work;
    s32 state;
    f32 turn;
    f32 target;

    if ((*(u32*)((s32)player + 4) & 0x1000) != 0) {
        *(u32*)((s32)player + 4) &= ~0x1000;
        *(u32*)player &= ~0x78000;
        work = __memAlloc(0, 0x48);
        *(void**)((s32)player + 0x234) = work;
        memset(work, 0, 0x48);
        *(s32*)((s32)work + 0x38) = -1;
        *(f32*)((s32)work + 0xC) = 4.25f;
        *(s32*)((s32)player + 0x44) = 0;
    }
    work = *(void**)((s32)player + 0x234);
    state = *(s32*)((s32)player + 0x44);
    switch (state) {
    case 0:
        psndSFXOn_3D(0x18D, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0xAF, (void*)((s32)player + 0x8C));
        marioCamZoomUp();
        marioPreJump();
        marioChgPose(str_M_J_1B_802c42c8);
        camFollowYOff();
        *(u32*)((s32)player + 8) |= 0x400;
        *(s32*)((s32)player + 0x170) = 0;
        *(s32*)((s32)player + 0x44) = 1;
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(f32*)((s32)work + 0x1C) = 0.0f;
        *(u32*)work = (hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x20000) ? 1 : 2;
    case 1:
        marioMakeJumpPara();
        target = (*(u32*)work & 1) ? 30.0f : -30.0f;
        turn = *(f32*)((s32)work + 0x1C);
        turn += 0.125f * (target - turn);
        *(f32*)((s32)work + 0x1C) = turn;
        *(f32*)((s32)player + 0x1AC) += turn;
        if (((*(u32*)work & 1) == 0 && *(f32*)((s32)player + 0x1AC) <= -270.0f) ||
            ((*(u32*)work & 1) != 0 && *(f32*)((s32)player + 0x1AC) >= 270.0f)) {
            *(s32*)((s32)player + 0x44) = 2;
            *(s32*)((s32)player + 0x170) = 20;
            marioPaperOn(paper_ship[marioGetColor()]);
            marioChgPaper(str_PM_H_1A_802c42d0);
        }
        break;
    case 2:
        target = (*(u32*)work & 1) ? 15.0f : -15.0f;
        turn = *(f32*)((s32)work + 0x1C);
        turn += 0.125f * (target - turn);
        *(f32*)((s32)work + 0x1C) = turn;
        *(f32*)((s32)player + 0x1AC) += turn;
        if (((*(u32*)work & 1) == 0 && *(f32*)((s32)player + 0x1AC) <= -1080.0f) ||
            ((*(u32*)work & 1) != 0 && *(f32*)((s32)player + 0x1AC) >= 1260.0f)) {
            *(s32*)((s32)player + 0x170) = 40;
            *(s32*)((s32)player + 0x44) = 3;
        }
        break;
    case 3:
        if (*(s32*)((s32)player + 0x170) == 30) allPartyRideShip();
        if (--*(s32*)((s32)player + 0x170) < 1) *(s32*)((s32)player + 0x44) = 10;
        break;
    }
    return 0;
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
