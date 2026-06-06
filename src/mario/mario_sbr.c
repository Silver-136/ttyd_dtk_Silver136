#include "mario/mario_sbr.h"

void* marioGetPtr(void);
void* memset(void* dst, int value, u32 size);
s32 hitGetAttr(void);
f64 cos(f64 value);
f32 __fabsf(f32 value);
extern f32 float_0_80420a60;
extern f32 float_360_80420a68;
extern f32 float_180_80420a70;
extern f32 float_1_80420a74;
extern f32 float_90_80420a98;
extern f32 float_3p1416_80420a9c;
extern f32 float_0p19_80420aa0;
extern u8 rubwork[0x68];

f32 revise360(f32 value) {
    while (value < float_0_80420a60) {
        value += float_360_80420a68;
    }
    while (value >= float_360_80420a68) {
        value -= float_360_80420a68;
    }
    return value;
}

void marioGetRubInit(void) {
    memset(rubwork, 0, 0x68);
    *(s32*)(rubwork + 0x20) = -1;
}

s32 N_marioGetMovementSpeed(void) {
    void* mario = marioGetPtr();
    u8 ux = *(u8*)((s32)mario + 0x252);
    s32 x = ux;
    s32 value;
    if ((s8)x == 0) {
        if (*(s8*)((s32)mario + 0x253) == 0) {
            return 0;
        }
    }
    x = (s8)x;
    value = x * x;
    x = *(s8*)((s32)mario + 0x253);
    value += x * x;
    if (value > 0xBD1) {
        return 2;
    }
    return 1;
}

f32 marioGetDispDir(void) {
    void* mario = marioGetPtr();
    f32 value = *(f32*)((s32)mario + 0x1AC) - float_90_80420a98 + *(f32*)((s32)mario + 0x19C);
    while (value < float_0_80420a60) {
        value += float_360_80420a68;
    }
    while (value >= float_360_80420a68) {
        value -= float_360_80420a68;
    }
    return value;
}

void* marioGetTouchNpcPtr(void) {
    return *(void**)((s32)marioGetPtr() + 0x1D8);
}

void marioSetPeakYpos(void) {
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x12C) = *(f32*)((s32)mario + 0x90);
}

void marioResetGravity(void) {
    f32 gravity = float_1_80420a74;
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x228) = gravity;
}

s32 marioChkPlanePlace(void) {
    s32 value = hitGetAttr() & 0x38;
    return ((u32)(-value) | (u32)value) >> 31;
}

void N_marioReloadMapOnBottomlessOff(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x2E0) = 0;
}

void N_marioReloadMapOnBottomlessOn(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x2E0) = 1;
}

f32 marioGetMoveRate(f32 angle) {
    void* mario = marioGetPtr();
    f32 value = (angle - *(f32*)((s32)mario + 0x19C)) * float_3p1416_80420a9c / float_180_80420a70;
    return __fabsf((f32)cos(value)) * float_0p19_80420aa0 + float_1_80420a74;
}


u8 marioGetRub(s32 buttonFlags, s32 param_2, s32 param_3, s32 param_4) {
    return 0;
}


u8 marioSetDirEventMain(void) {
    return 0;
}


void marioAdjustMoveDir(void) {
}


double toMovedir2(double param_1, double param_2) {
    return 0.0;
}


u8 marioUpdateKeyData(void) {
    return 0;
}


u8 marioGetStick(void) {
    return 0;
}


s32 chkRevolveDir(int* param_1, int* param_2) {
    return 0;
}


f32 toMovedir(f32 angle) {
    extern void* marioGetPtr(void);
    extern f32 float_0_80420a60;
    extern f32 float_360_80420a68;
    extern f32 float_180_80420a70;
    extern f32 float_270_80420a90;
    extern f32 float_0p5_80420a88;
    extern f32 float_neg0p5_80420a8c;
    extern f32 float_1000_80420a94;
    extern f32 float_280_80420a78;
    extern f32 float_460_80420a7c;
    extern f32 float_260_80420a80;
    extern f32 float_440_80420a84;
    void* mario = marioGetPtr();

    if (*(u32*)((s32)mario + 0x0) & 0x100000) {
        f32 dir = *(f32*)((s32)mario + 0x1AC);
        if (dir == float_280_80420a78 || dir == float_460_80420a7c) {
            angle = float_0_80420a60;
        } else if (dir == float_260_80420a80 || dir == float_440_80420a84) {
            angle = float_180_80420a70;
        } else {
            while (dir < float_0_80420a60) {
                dir += float_360_80420a68;
            }
            while (dir >= float_360_80420a68) {
                dir -= float_360_80420a68;
            }
            if (dir >= float_180_80420a70) {
                angle = float_0_80420a60;
            } else {
                angle = float_180_80420a70;
            }
        }
    }

    angle = (float_270_80420a90 - angle) + *(f32*)((s32)mario + 0x19C);
    angle = (f32)((s32)(angle * float_1000_80420a94 + (angle >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c))) / float_1000_80420a94;
    while (angle < float_0_80420a60) {
        angle += float_360_80420a68;
    }
    while (angle >= float_360_80420a68) {
        angle -= float_360_80420a68;
    }
    return angle;
}

f32 partyToMovedir(void* pParty) {
    extern f32 float_0_80420a60;
    extern f32 float_360_80420a68;
    extern f32 float_180_80420a70;
    extern f32 float_270_80420a90;
    extern f32 float_0p5_80420a88;
    extern f32 float_neg0p5_80420a8c;
    extern f32 float_1000_80420a94;
    extern f32 float_280_80420a78;
    extern f32 float_460_80420a7c;
    extern f32 float_260_80420a80;
    extern f32 float_440_80420a84;
    f32 angle;
    void* mario = *(void**)((s32)pParty + 0x160);

    if (*(u32*)((s32)pParty + 0x0) & 0x1000) {
        f32 dir = *(f32*)((s32)pParty + 0x10C);
        if (dir == float_280_80420a78 || dir == float_460_80420a7c) {
            angle = float_0_80420a60;
        } else if (dir == float_260_80420a80 || dir == float_440_80420a84) {
            angle = float_180_80420a70;
        } else {
            while (dir < float_0_80420a60) {
                dir += float_360_80420a68;
            }
            while (dir >= float_360_80420a68) {
                dir -= float_360_80420a68;
            }
            angle = float_180_80420a70;
            if (dir >= angle) {
                angle = float_0_80420a60;
            }
        }
    }

    angle = (float_270_80420a90 - angle) + *(f32*)((s32)mario + 0x19C);
    angle = (f32)((s32)(angle * float_1000_80420a94 + (angle >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c))) / float_1000_80420a94;
    while (angle < float_0_80420a60) {
        angle += float_360_80420a68;
    }
    while (angle >= float_360_80420a68) {
        angle -= float_360_80420a68;
    }
    return angle;
}

s32 checkOneRevolution(int param_1) {
    s32 result = 0;
    s32 count = *(s32*)(rubwork + 8);
    s32 index;
    s16 cur;
    s16 prev;
    s16 start;
    s32 i;

    if (count < 2) {
        return -1;
    }

    index = *(s32*)(rubwork + 0xC);
    start = *(s16*)(rubwork + 0x14);
    cur = *(s16*)(rubwork + 0x3C + index * 2);
    for (i = 1; i <= 0x14; i++) {
        s32 prevIndex = index - i;
        while (prevIndex < 0) {
            prevIndex += 0x14;
        }
        prev = *(s16*)(rubwork + 0x3C + prevIndex * 2);
        if (cur != prev) {
            break;
        }
    }
    if (cur == prev || prev == -1) {
        return -1;
    }
    if (param_1 != 0) {
        if (cur > prev) {
            if (start < prev || cur <= start) {
                result = 1;
            }
        } else if (cur > start || start < prev) {
            result = 1;
        }
    } else if (cur < prev) {
        if (start > prev || start <= cur) {
            result = 1;
        }
    } else if (cur < start || start > prev) {
        result = 1;
    }
    return result;
}

void marioBottomless(void) {
    extern void* marioGetPtr(void);
    extern f32 float_neg2000_80420aa8;
    extern f32 float_neg450_80420aac;
    extern void* gp;
    extern s32 marioBgmodeChk(void);
    extern void marioChgMot(s32 motion);
    extern void seqSetSeq(s32 seq, void* mapName, void* beroName);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* mario = marioGetPtr();
    if (*(f32*)((s32)mario + 0x90) > float_neg2000_80420aa8) {
        return;
    }
    if (marioBgmodeChk() == 1) {
            marioChgMot(0x21);
    } else if (*(u32*)((s32)mario + 0x2E0) != 0) {
            void* work = gp;
            seqSetSeq(3, (void*)((s32)work + 0x12C), (void*)((s32)work + 0x11C));
    } else {
            void* party;
            u32 y = *(u32*)((s32)mario + 0x2E8);
            u32 x = *(u32*)((s32)mario + 0x2E4);
            *(u32*)((s32)mario + 0x8C) = x;
            *(u32*)((s32)mario + 0x90) = y;
            *(u32*)((s32)mario + 0x94) = *(u32*)((s32)mario + 0x2EC);
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                y = *(u32*)((s32)mario + 0x90);
                x = *(u32*)((s32)mario + 0x8C);
                *(u32*)((s32)party + 0x58) = x;
                *(u32*)((s32)party + 0x5C) = y;
                *(u32*)((s32)party + 0x60) = *(u32*)((s32)mario + 0x94);
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party != 0) {
                y = *(u32*)((s32)mario + 0x90);
                x = *(u32*)((s32)mario + 0x8C);
                *(u32*)((s32)party + 0x58) = x;
                *(u32*)((s32)party + 0x5C) = y;
                *(u32*)((s32)party + 0x60) = *(u32*)((s32)mario + 0x94);
            }
            if (marioBgmodeChk() == 1) {
                *(f32*)((s32)mario + 0x94) = float_neg450_80420aac;
            }
            marioChgMot(0x22);
    }
}

void marioSetForceMove(s16 time, f32 angle, f32 spd) {
    extern void* marioGetPtr(void);
    extern void marioChgMot(s32 motion);
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x0) |= 0x20;
    *(f32*)((s32)mario + 0x1A0) = angle;
    *(f32*)((s32)mario + 0x1A4) = angle;
    *(s16*)((s32)mario + 0x4E) = time;
    *(f32*)((s32)mario + 0x180) = spd;
    switch (*(u16*)((s32)mario + 0x2E)) {
        case 0x15:
        case 0x16:
        case 0x19:
        case 0x1A:
            break;
        default:
            marioChgMot(spd >= *(f32*)((s32)mario + 0x188) ? 2 : 1);
            *(f32*)((s32)mario + 0x1A0) = angle;
            *(f32*)((s32)mario + 0x1A4) = angle;
            *(s16*)((s32)mario + 0x4E) = time;
            *(f32*)((s32)mario + 0x180) = spd;
            break;
    }
}

f32 toMovedirSimple(f32 angle) {
    extern void* marioGetPtr(void);
    extern f32 float_270_80420a90;
    extern f32 float_0_80420a60;
    extern f32 float_0p5_80420a88;
    extern f32 float_neg0p5_80420a8c;
    extern f32 float_1000_80420a94;
    extern f32 float_360_80420a68;
    void* mario = marioGetPtr();
    f32 value = (float_270_80420a90 - angle) + *(f32*)((s32)mario + 0x19C);
    s32 rounded = (s32)(value * float_1000_80420a94 + (value >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c));
    value = (f32)rounded / float_1000_80420a94;
    while (value < float_0_80420a60) {
        value += float_360_80420a68;
    }
    while (value >= float_360_80420a68) {
        value -= float_360_80420a68;
    }
    return value;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z) {
    extern void* marioGetPtr(void);
    extern f32 vec3_802c3ea8[3];
    extern f32 float_37_80420aa4;
    typedef struct Vec { f32 x, y, z; } Vec;
    void* mario = marioGetPtr();
    Vec pos = *(Vec*)vec3_802c3ea8;
    pos.x = x;
    pos.y = y + float_37_80420aa4;
    pos.z = z;
    *(Vec*)((s32)mario + 0x2E4) = pos;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioForceMoveMain(void) {
    extern void* marioGetPtr(void);
    extern f32 float_0_80420a60;
    extern f32 float_1_80420a74;
    extern f32 marioGetDashSpd(void);
    extern f32 marioGetWalkSpd(void);
    void* mario = marioGetPtr();
    if (*(u32*)((s32)mario + 0x0) & 0x20) {
        s32 time = *(s16*)((s32)mario + 0x4E) - 1;
        *(s16*)((s32)mario + 0x4E) = time;
        if ((s16)time <= 0) {
            f32 zero = float_0_80420a60;
            f32 one = float_1_80420a74;
            f32 speed;
            *(s16*)((s32)mario + 0x4E) = 0;
            *(u32*)((s32)mario + 0x0) &= ~0x20;
            *(f32*)((s32)mario + 0x180) = zero;
            speed = *(f32*)((s32)mario + 0x194);
            if (speed > one) {
                *(f32*)((s32)mario + 0x180) = marioGetDashSpd();
            } else if (speed > zero) {
                *(f32*)((s32)mario + 0x180) = marioGetWalkSpd();
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void N_marioSetBottomlessRespawnPos(f32 x, f32 y, f32 z) {
    extern void* marioGetPtr(void);
    extern f32 vec3_802c3eb4[3];
    typedef struct Vec { f32 x, y, z; } Vec;
    void* mario = marioGetPtr();
    Vec pos = *(Vec*)vec3_802c3eb4;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    *(Vec*)((s32)mario + 0x2E4) = pos;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

