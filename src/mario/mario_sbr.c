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


s32 marioGetRub(s32 buttonFlags, s32* outDir, s32* outCount, f32* outMove) {
    extern void* marioGetPtr(void);
    extern void* memset(void*, int, u32);
    extern u32 kpaGetStageViewType(void);
    extern s32 yoshiGetStatus(void);
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern s32 chkRevolveDir(s32*, s32*);
    extern s32 checkOneRevolution(s32);
    extern f32 float_0_80420a60;
    extern f32 float_50_80420a6c;
    extern f32 float_70_80420a64;
    extern f32 float_360_80420a68;
    extern u8 rubwork[0x68];

    void* player;
    f32 dir;
    f32 move;
    s32 dirInt;
    s16 dirShort;
    s32 ret;
    s32 state;
    s32 next;
    s32 revolved;
    s32 prevDir;
    s32 i;

    ret = 0;
    player = marioGetPtr();
    if (buttonFlags != 0 && ((*(u16*)((s32)player + 0x24A) & buttonFlags) == 0)) {
        memset(rubwork, 0, 0x68);
        *(s32*)(rubwork + 0x20) = -1;
        *outDir = -1;
        *outCount = 0;
        *outMove = float_0_80420a60;
        return -1;
    }

    player = marioGetPtr();
    if ((*(u32*)((s32)player + 0x0) & 0x20) != 0) {
        dir = *(f32*)((s32)player + 0x1A4);
        move = *(f32*)((s32)player + 0x180);
    } else {
        s32 stickX = *(s8*)((s32)player + 0x252);
        s32 stickY;
        f32 dist;
        f32 angle;

        if ((*(u32*)((s32)player + 0x0) & 0x02000000) != 0 || kpaGetStageViewType() != 0) {
            stickY = 0;
        } else {
            stickY = -*(s8*)((s32)player + 0x253);
        }
        if (yoshiGetStatus() == 2) {
            stickY = 0;
        }

        dist = (f32)distABf(float_0_80420a60, float_0_80420a60, (f32)stickX, (f32)-stickY);
        if (dist >= float_70_80420a64) {
            dist = float_70_80420a64;
        }
        angle = (f32)angleABf(float_0_80420a60, float_0_80420a60, (f32)stickX, (f32)-stickY);
        if (dist == float_0_80420a60) {
            angle = *(f32*)((s32)player + 0x1A0);
        }
        dir = angle + *(f32*)((s32)player + 0x19C);
        while (dir < float_0_80420a60) {
            dir += float_360_80420a68;
        }
        while (dir >= float_360_80420a68) {
            dir -= float_360_80420a68;
        }
        move = dist;
    }

    dirInt = (s32)dir;
    dirShort = (s16)dirInt;
    if (move < float_50_80420a6c) {
        *(s32*)(rubwork + 0x24) += 1;
        if (*(s32*)(rubwork + 0x24) > 10) {
            *(s32*)(rubwork + 0x8) = 0;
            for (i = 0; i <= 0x14; i++) {
                *(s16*)(rubwork + 0x3C + i * 2) = dirShort;
            }
        }
        return -1;
    }

    *(s32*)(rubwork + 0x24) = 0;
    *(s16*)(rubwork + 0x3C + (*(s32*)(rubwork + 0xC) * 2)) = dirShort;
    next = *(s32*)(rubwork + 0xC) + 1;
    if (next > 0x13) {
        next = *(s32*)(rubwork + 0xC) - 0x13;
    }
    *(s16*)(rubwork + 0x3C + next * 2) = -1;

    state = chkRevolveDir(&prevDir, &revolved);
    if (state == 0) {
        if (*(s32*)(rubwork + 0x10) == 2) {
            *(s32*)(rubwork + 0x8) = 0;
            *(s32*)(rubwork + 0x4) = 0;
        }
        if (*(s32*)(rubwork + 0x8) == 0) {
            *(s32*)(rubwork + 0x14) = dirInt;
            for (i = 0; i <= 0x14; i++) {
                *(s16*)(rubwork + 0x3C + i * 2) = dirShort;
            }
        }
        *(s32*)(rubwork + 0x1C) = 0;
        *(s32*)(rubwork + 0x8) += 1;
        ret = checkOneRevolution(0);
        if (ret == 1) {
            *(s32*)(rubwork + 0x4) += 1;
            for (i = 0; i <= 0x14; i++) {
                *(s16*)(rubwork + 0x3C + i * 2) = *(s16*)(rubwork + 0x14);
            }
        }
        ret = (ret == 1);
        *(s32*)(rubwork + 0x10) = 1;
    } else if (state == 1) {
        if (*(s32*)(rubwork + 0x10) == 1) {
            *(s32*)(rubwork + 0x8) = 0;
            *(s32*)(rubwork + 0x4) = 0;
        }
        if (*(s32*)(rubwork + 0x8) == 0) {
            *(s32*)(rubwork + 0x14) = dirInt;
            for (i = 0; i <= 0x14; i++) {
                *(s16*)(rubwork + 0x3C + i * 2) = dirShort;
            }
        }
        *(s32*)(rubwork + 0x1C) = 0;
        *(s32*)(rubwork + 0x8) += 1;
        ret = checkOneRevolution(1);
        if (ret == 1) {
            *(s32*)(rubwork + 0x4) += 1;
            for (i = 0; i <= 0x14; i++) {
                *(s16*)(rubwork + 0x3C + i * 2) = *(s16*)(rubwork + 0x14);
            }
        }
        ret = (ret == 1);
        *(s32*)(rubwork + 0x10) = 2;
    } else {
        *(s32*)(rubwork + 0xC) = next;
        return -1;
    }

    *outDir = *(s32*)(rubwork + 0x10) - 1;
    *outCount = *(s32*)(rubwork + 0x4);
    *(s32*)(rubwork + 0xC) = next;
    return ret;
}

s32 marioSetDirEventMain(void) {
    extern void* marioGetPtr(void);
    extern s32 sysMsec2Frame(s32);
    extern f32 float_0_80420a60;
    extern f32 float_0p5_80420a88;
    extern f32 float_neg0p5_80420a8c;
    extern f32 float_270_80420a90;
    extern f32 float_360_80420a68;
    extern f32 float_1000_80420a94;

    void* mario;
    f32 target;
    f32 cur;
    f32 view;
    f32 bias;
    s32 frames;
    s32 timer;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 4) & 0x8000) == 0) {
        return 1;
    }

    frames = sysMsec2Frame(*(s32*)((s32)mario + 0x2A4));

    target = float_270_80420a90 - *(f32*)((s32)mario + 0x2A0);
    while (target < float_0_80420a60) {
        target += float_360_80420a68;
    }
    while (target >= float_360_80420a68) {
        target -= float_360_80420a68;
    }

    if (frames < 1) {
        cur = target;
    } else {
        cur = *(f32*)((s32)mario + 0x29C) +
              (target - *(f32*)((s32)mario + 0x29C)) *
              ((f32)*(s32*)((s32)mario + 0x2A8) / (f32)frames);
        while (cur < float_0_80420a60) {
            cur += float_360_80420a68;
        }
        while (cur >= float_360_80420a68) {
            cur -= float_360_80420a68;
        }
    }

    *(f32*)((s32)mario + 0x1AC) = cur;
    *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);

    view = (float_270_80420a90 - *(f32*)((s32)mario + 0x1AC)) +
           *(f32*)((s32)marioGetPtr() + 0x19C);
    bias = float_neg0p5_80420a8c;
    if (view >= float_0_80420a60) {
        bias = float_0p5_80420a88;
    }
    view = (f32)(s32)(view * float_1000_80420a94 + bias) / float_1000_80420a94;
    while (view < float_0_80420a60) {
        view += float_360_80420a68;
    }
    while (view >= float_360_80420a68) {
        view -= float_360_80420a68;
    }
    *(f32*)((s32)mario + 0x1A4) = view;
    *(f32*)((s32)mario + 0x1A0) = *(f32*)((s32)mario + 0x1A4);

    if (frames < 1) {
        *(u32*)((s32)mario + 4) &= ~0x8000;
        return 1;
    }

    timer = *(s32*)((s32)mario + 0x2A8) + 1;
    *(s32*)((s32)mario + 0x2A8) = timer;
    if (timer > frames) {
        *(u32*)((s32)mario + 4) &= ~0x8000;
        return 1;
    }
    return 0;
}

void marioAdjustMoveDir(void) {
    extern void* marioGetPtr(void);
    extern f32 float_0_80420a60;
    extern f32 float_360_80420a68;
    extern f32 float_180_80420a70;
    extern f32 float_270_80420a90;
    extern f32 float_90_80420a98;
    extern f32 float_0p5_80420a88;
    extern f32 float_neg0p5_80420a8c;
    extern f32 float_1000_80420a94;
    extern f32 float_280_80420a78;
    extern f32 float_460_80420a7c;
    extern f32 float_260_80420a80;
    extern f32 float_440_80420a84;

    void* mario = marioGetPtr();
    f32 out;
    void* mario2;
    f32 dir;
    f32 angle;

    if (*(f32*)((s32)mario + 0x1AC) < float_90_80420a98 ||
        *(f32*)((s32)mario + 0x1AC) > float_270_80420a90) {
        out = float_180_80420a70;
        mario2 = marioGetPtr();
        if ((*(u32*)((s32)mario2 + 0x0) & 0x100000) != 0) {
            dir = *(f32*)((s32)mario2 + 0x1AC);
            if (dir == float_280_80420a78 || dir == float_460_80420a7c) {
                out = float_0_80420a60;
            } else if (dir == float_260_80420a80 || dir == float_440_80420a84) {
                out = float_180_80420a70;
            } else {
                while (dir < float_0_80420a60) {
                    dir += float_360_80420a68;
                }
                while (dir >= float_360_80420a68) {
                    dir -= float_360_80420a68;
                }
                out = float_180_80420a70;
                if (dir >= out) {
                    out = float_0_80420a60;
                }
            }
        }
        angle = (float_270_80420a90 - out) + *(f32*)((s32)mario2 + 0x19C);
        angle = (f32)((s32)(angle * float_1000_80420a94 + (angle >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c))) / float_1000_80420a94;
        while (angle < float_0_80420a60) {
            angle += float_360_80420a68;
        }
        while (angle >= float_360_80420a68) {
            angle -= float_360_80420a68;
        }
        *(f32*)((s32)mario + 0x1A0) = angle;
        *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x1A0);
        *(f32*)((s32)mario + 0x1AC) = float_180_80420a70;
        *(f32*)((s32)mario + 0x1B0) = float_180_80420a70;
    } else {
        out = float_0_80420a60;
        mario2 = marioGetPtr();
        if ((*(u32*)((s32)mario2 + 0x0) & 0x100000) != 0) {
            dir = *(f32*)((s32)mario2 + 0x1AC);
            if (dir == float_280_80420a78 || dir == float_460_80420a7c) {
                out = float_0_80420a60;
            } else if (dir == float_260_80420a80 || dir == float_440_80420a84) {
                out = float_180_80420a70;
            } else {
                while (dir < float_0_80420a60) {
                    dir += float_360_80420a68;
                }
                while (dir >= float_360_80420a68) {
                    dir -= float_360_80420a68;
                }
                out = float_180_80420a70;
                if (dir >= out) {
                    out = float_0_80420a60;
                }
            }
        }
        angle = (float_270_80420a90 - out) + *(f32*)((s32)mario2 + 0x19C);
        angle = (f32)((s32)(angle * float_1000_80420a94 + (angle >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c))) / float_1000_80420a94;
        while (angle < float_0_80420a60) {
            angle += float_360_80420a68;
        }
        while (angle >= float_360_80420a68) {
            angle -= float_360_80420a68;
        }
        *(f32*)((s32)mario + 0x1A0) = angle;
        *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x1A0);
        *(f32*)((s32)mario + 0x1AC) = float_0_80420a60;
        *(f32*)((s32)mario + 0x1B0) = float_0_80420a60;
    }
}

double toMovedir2(double angle, double add) {
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
    f32 dir;
    f32 diff;

    if ((*(u32*)((s32)mario + 0x0) & 0x100000) != 0) {
        dir = *(f32*)((s32)mario + 0x1AC);
        if ((double)float_280_80420a78 == (double)dir || (double)float_460_80420a7c == (double)dir) {
            angle = (double)float_0_80420a60;
        } else if ((double)float_260_80420a80 == (double)dir || (double)float_440_80420a84 == (double)dir) {
            angle = (double)float_180_80420a70;
        } else {
            while ((double)dir < (double)float_0_80420a60) {
                dir = (f32)((double)dir + (double)float_360_80420a68);
            }
            while ((double)dir >= (double)float_360_80420a68) {
                dir = (f32)((double)dir - (double)float_360_80420a68);
            }
            angle = (double)float_180_80420a70;
            if (angle <= (double)dir) {
                angle = (double)float_0_80420a60;
            }
        }
    }

    dir = (f32)((double)(f32)((double)float_270_80420a90 - angle) + (double)*(f32*)((s32)mario + 0x19C));
    dir = (f32)((s32)(float_1000_80420a94 * dir + (dir >= float_0_80420a60 ? float_0p5_80420a88 : float_neg0p5_80420a8c))) / float_1000_80420a94;
    while ((double)dir < (double)float_0_80420a60) {
        dir = (f32)((double)dir + (double)float_360_80420a68);
    }
    while ((double)dir >= (double)float_360_80420a68) {
        dir = (f32)((double)dir - (double)float_360_80420a68);
    }

    diff = (f32)((double)dir - (double)*(f32*)((s32)mario + 0x19C));
    while (diff < float_0_80420a60) {
        diff += float_360_80420a68;
    }
    while (diff >= float_360_80420a68) {
        diff -= float_360_80420a68;
    }
    if (diff >= float_180_80420a70) {
        add = -add;
    }
    angle = (double)dir + add;
    while ((double)(f32)angle < (double)float_0_80420a60) {
        angle = (double)(f32)angle + (double)float_360_80420a68;
    }
    while ((double)(f32)angle >= (double)float_360_80420a68) {
        angle = (double)(f32)angle - (double)float_360_80420a68;
    }
    return (double)(f32)angle;
}

void marioUpdateKeyData(void) {
    extern void* marioGetPtr(void);
    extern s32 marioChkKey(void);
    extern s32 marioGetMotSlitCancel3(void);
    extern void* gp;
    extern u8 DemoPad[];

    void* mario = marioGetPtr();
    u32 filter;
    void* globals = gp;
    u32 buttons;
    u32 dirs;

    *(u8*)((s32)mario + 0x252) = *(u8*)((s32)globals + 0x13B8);
    *(u8*)((s32)mario + 0x253) = -*(u8*)((s32)gp + 0x13BC);
    *(u8*)((s32)mario + 0x254) = *(u8*)((s32)gp + 0x13C0);
    *(u8*)((s32)mario + 0x255) = -*(u8*)((s32)gp + 0x13C4);
    *(u8*)((s32)mario + 0x256) = *(u8*)((s32)gp + 0x13C8);
    *(u8*)((s32)mario + 0x257) = *(u8*)((s32)gp + 0x13CC);
    *(u8*)((s32)mario + 0x258) = 0;
    *(u8*)((s32)mario + 0x259) = 0;

    filter = 0xFFFFFFFF;
    buttons = *(u32*)((s32)mario + 0x290);
    if (buttons != 0) {
        filter = buttons;
    }
    *(u16*)((s32)mario + 0x24A) = *(u32*)((s32)gp + 0x1328) & filter;
    *(u16*)((s32)mario + 0x24C) = *(u32*)((s32)gp + 0x1338) & filter;
    *(u16*)((s32)mario + 0x24E) = *(u32*)((s32)gp + 0x1348) & filter;
    *(u16*)((s32)mario + 0x250) = *(u32*)((s32)gp + 0x1368) & filter;

    if ((*(u32*)mario & 0xE) == 0) {
        if ((*(u32*)((s32)mario + 0xC) & 0x08000000) != 0) {
            *(u8*)((s32)mario + 0x252) = 0;
            *(u8*)((s32)mario + 0x253) = 0;
        }
        if ((*(u32*)((s32)mario + 0xC) & 0x04000000) != 0) {
            *(u8*)((s32)mario + 0x254) = 0;
            *(u8*)((s32)mario + 0x255) = 0;
        }
        if ((*(u32*)((s32)mario + 0xC) & 0x02000000) != 0) {
            *(u8*)((s32)mario + 0x256) = 0;
            *(u8*)((s32)mario + 0x257) = 0;
        }
        if ((*(u32*)((s32)mario + 0xC) & 0x01000000) != 0) {
            *(u8*)((s32)mario + 0x258) = 0;
            *(u8*)((s32)mario + 0x259) = 0;
        }
    }

    if ((*(u32*)mario & 0xE) != 0 || marioChkKey() == 0) {
        *(u8*)((s32)mario + 0x252) = 0;
        *(u8*)((s32)mario + 0x253) = 0;
        *(u8*)((s32)mario + 0x254) = 0;
        *(u8*)((s32)mario + 0x255) = 0;
        *(u8*)((s32)mario + 0x256) = 0;
        *(u8*)((s32)mario + 0x257) = 0;
        *(u8*)((s32)mario + 0x258) = 0;
        *(u8*)((s32)mario + 0x259) = 0;
        if (*(u32*)((s32)mario + 0x290) == 0) {
            *(u16*)((s32)mario + 0x24A) = 0;
            *(u16*)((s32)mario + 0x24C) = 0;
            *(u16*)((s32)mario + 0x24E) = 0;
            *(u16*)((s32)mario + 0x250) = 0;
        }
        if (marioGetMotSlitCancel3() == 0 && (*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
            *(u16*)((s32)mario + 0x24A) = 0x20;
        }
    }
    dirs = *(u16*)(DemoPad + 0x10);
    *(u32*)((s32)mario + 0x260) = dirs & (dirs ^ *(u32*)((s32)mario + 0x25C));
    *(u32*)((s32)mario + 0x25C) = dirs;
}

void marioGetStick(float* param_1, float* param_2) {
    extern s32 kpaGetStageViewType(void);
    extern s32 yoshiGetStatus(void);
    extern f32 distABf(double, double, double, double);
    extern f32 angleABf(double, double, double, double);
    extern f32 float_0_80420a60;
    extern f32 float_70_80420a64;
    extern f32 float_360_80420a68;

    void* mario = marioGetPtr();
    if ((*(u32*)mario & 0x20) != 0) {
        *param_1 = *(f32*)((s32)mario + 0x1A4);
        *param_2 = *(f32*)((s32)mario + 0x180);
    } else {
        s32 stickX = *(s8*)((s32)mario + 0x252);
        s32 stickY;
        f32 dist;
        f32 angle;

        if ((*(u32*)mario & 0x02000000) != 0 || kpaGetStageViewType() == 0) {
            stickY = 0;
        } else {
            stickY = -*(s8*)((s32)mario + 0x253);
        }
        if (yoshiGetStatus() == 2) {
            stickY = 0;
        }
        stickY = -stickY;
        dist = distABf(float_0_80420a60, float_0_80420a60, (f32)stickX, (f32)stickY);
        if (dist > float_70_80420a64) {
            dist = float_70_80420a64;
        }
        angle = angleABf(float_0_80420a60, float_0_80420a60, (f32)stickX, (f32)stickY);
        if (dist == float_0_80420a60) {
            angle = *(f32*)((s32)mario + 0x1A0);
        }
        angle += *(f32*)((s32)mario + 0x19C);
        while (angle < float_0_80420a60) {
            angle += float_360_80420a68;
        }
        while (angle >= float_360_80420a68) {
            angle -= float_360_80420a68;
        }
        *param_1 = angle;
        *param_2 = dist;
    }
}

s32 chkRevolveDir(int* param_1, int* param_2) {
    extern s32 DAT_803dc234;
    extern s16 DAT_803dc264[];
    extern s32 int32_t_803dc248;
    extern f32 float_180_80420a70;
    extern f32 __fabsf(f32);

    s32 left = 0;
    s32 right = 0;
    s32 i;
    s32 offset = 0;

    for (i = 0; i < 10; i++) {
        s32 idx = DAT_803dc234 + offset;
        s32 prev;
        s32 a;
        s32 b;
        s32 diff;

        while (idx < 0) {
            idx += 20;
        }
        idx %= 20;
        prev = DAT_803dc234 + offset - 1;
        while (prev < 0) {
            prev += 20;
        }
        prev %= 20;
        offset--;
        a = DAT_803dc264[idx];
        b = DAT_803dc264[prev];
        if (i != 0 || a != b) {
            diff = a - b;
            if (__fabsf((f32)diff) > float_180_80420a70) {
                if (diff > 0) {
                    a += 0x168;
                } else {
                    a -= 0x168;
                }
                diff = a - b;
            }
            if (diff > -0x5A && diff < 0) {
                left++;
            }
            if (diff > 0 && diff < 0x5A) {
                right++;
            }
        }
    }
    if (left == 0 && right > 3) {
        int32_t_803dc248 = 0;
        *param_1 = 0;
        *param_2 = right;
        return 0;
    }
    if (right == 0 && left > 3) {
        int32_t_803dc248 = 1;
        *param_1 = left;
        *param_2 = 0;
        return 1;
    }
    *param_1 = left;
    *param_2 = right;
    int32_t_803dc248 = -1;
    return -1;
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

