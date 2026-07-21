#include "cam_shift.h"


void camShiftInit(void) {
    extern void* wp;
    extern f32 float_0_80421fe8;
    extern f32 float_300_80421fec;
    extern f32 float_3_80421ff0;
    extern f32 float_0p002_80421ff4;
    f32 zero = float_0_80421fe8;
    f32 dist = float_300_80421fec;
    f32 three = float_3_80421ff0;
    f32 small = float_0p002_80421ff4;

    *(f32*)((s32)wp + 0x0) = zero;
    *(f32*)((s32)wp + 0x4) = zero;
    *(f32*)((s32)wp + 0x8) = zero;
    *(f32*)((s32)wp + 0xC) = zero;
    *(f32*)((s32)wp + 0x10) = zero;
    *(f32*)((s32)wp + 0x20) = dist;
    *(f32*)((s32)wp + 0x24) = three;
    *(f32*)((s32)wp + 0x28) = small;
    *(f32*)((s32)wp + 0x2C) = zero;
    *(f32*)((s32)wp + 0x30) = zero;
}


void camShiftMain(void* cam, void* mario, f32* out) {
    extern void* gp;
    extern void* wp;
    extern f32 camRoadGetCurrentShiftXRate(void);
    extern f32 getV60FPS(f32 rate, u32 startHi, u32 startLo, u32 endHi, u32 endLo);
    extern f32 float_0_80421fe8;
    extern f32 float_1_80421ff8;
    extern f32 float_25_80421ffc;
    extern f32 float_neg25_80422000;
    extern f32 float_0p0001_80422004;
    extern f32 float_neg0p0001_80422008;
    f32 rate = camRoadGetCurrentShiftXRate();
    f32 dir;
    s32 forced = 0;
    void* work;

    if ((*(u16*)cam & 0x20) != 0) {
        forced = 1;
    }

    if (forced) {
        work = wp;
        *(f32*)((s32)work + 8) = float_1_80421ff8;
        *(f32*)((s32)work + 4) = float_0_80421fe8;
        *(f32*)((s32)work + 0x10) = float_1_80421ff8;
        dir = float_0_80421fe8;
    } else {
        s32 stick = *(s8*)((s32)mario + 0x252);
        work = wp;
        dir = (f32)stick;
        if (dir > float_0_80421fe8) {
            dir = float_25_80421ffc;
        }
        if (dir < float_0_80421fe8) {
            dir = float_neg25_80422000;
        }
        if (*(f32*)((s32)mario + 0x8C) == *(f32*)((s32)work + 0x2C) &&
            *(f32*)((s32)mario + 0x94) == *(f32*)((s32)work + 0x30)) {
            dir = float_0_80421fe8;
        }
        *(f32*)((s32)work + 0x2C) = *(f32*)((s32)mario + 0x8C);
        *(f32*)((s32)wp + 0x30) = *(f32*)((s32)mario + 0x94);
    }

    if (dir != float_0_80421fe8) {
        work = wp;
        if (dir < float_0_80421fe8) {
            if (*(f32*)((s32)work + 0xC) > float_0_80421fe8) {
                *(f32*)((s32)work + 0xC) = getV60FPS(dir,
                    *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
                    *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
            } else {
                *(f32*)((s32)work + 0xC) += getV60FPS(dir,
                    *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
                    *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
            }
            work = wp;
            if (*(f32*)((s32)work + 0xC) <= -*(f32*)((s32)work + 0x20)) {
                *(f32*)((s32)work + 0x10) = float_1_80421ff8;
                if (*(f32*)((s32)work + 4) > float_0_80421fe8) {
                    *(f32*)((s32)work + 8) = float_0_80421fe8;
                }
                *(f32*)((s32)wp + 4) = -rate;
                *(f32*)((s32)wp + 0xC) = -*(f32*)((s32)wp + 0x20);
            }
        } else {
            if (*(f32*)((s32)work + 0xC) < float_0_80421fe8) {
                *(f32*)((s32)work + 0xC) = getV60FPS(dir,
                    *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
                    *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
            } else {
                *(f32*)((s32)work + 0xC) += getV60FPS(dir,
                    *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
                    *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
            }
            work = wp;
            if (*(f32*)((s32)work + 0xC) >= *(f32*)((s32)work + 0x20)) {
                *(f32*)((s32)work + 0x10) = float_1_80421ff8;
                if (*(f32*)((s32)work + 4) < float_0_80421fe8) {
                    *(f32*)((s32)work + 8) = float_0_80421fe8;
                }
                *(f32*)((s32)wp + 4) = rate;
                *(f32*)((s32)wp + 0xC) = *(f32*)((s32)wp + 0x20);
            }
        }
    }

    work = wp;
    if (*(f32*)((s32)work + 0x10) != float_0_80421fe8) {
        *(f32*)((s32)work + 8) += getV60FPS(*(f32*)((s32)work + 0x28),
            *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
            *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
        work = wp;
        if (*(f32*)((s32)work + 8) > float_1_80421ff8) {
            *(f32*)((s32)work + 8) = float_1_80421ff8;
        }
    }

    work = wp;
    if (float_0_80421fe8 == *(f32*)((s32)work + 4) - *(f32*)((s32)work + 0)) {
        *(f32*)((s32)work + 8) = float_0_80421fe8;
        *(f32*)((s32)work + 0x10) = float_0_80421fe8;
    }

    work = wp;
    {
        f32 delta = *(f32*)((s32)work + 4) - *(f32*)((s32)work + 0);
        f32 move = *(f32*)((s32)work + 8) * delta;
        if (delta > float_0_80421fe8) {
            if (delta < float_0p0001_80422004) {
                move = delta;
            }
            if (move > *(f32*)((s32)work + 0x24)) {
                move = *(f32*)((s32)work + 0x24);
            }
        } else {
            if (delta > float_neg0p0001_80422008) {
                move = delta;
            }
            if (move < -*(f32*)((s32)work + 0x24)) {
                move = -*(f32*)((s32)work + 0x24);
            }
        }
        if (dir != float_0_80421fe8 || forced) {
            *(f32*)((s32)work + 0) += getV60FPS(move,
                *(u32*)((s32)work + 0x18), *(u32*)((s32)work + 0x1C),
                *(u32*)((s32)gp + 0x38), *(u32*)((s32)gp + 0x3C));
            work = wp;
            if (*(f32*)((s32)work + 4) >= float_0_80421fe8) {
                if (*(f32*)((s32)work + 0) > *(f32*)((s32)work + 4)) {
                    *(f32*)((s32)work + 0) = *(f32*)((s32)work + 4);
                }
            } else if (*(f32*)((s32)work + 0) < *(f32*)((s32)work + 4)) {
                *(f32*)((s32)work + 0) = *(f32*)((s32)work + 4);
            }
        } else {
            *(f32*)((s32)work + 8) = float_0_80421fe8;
        }
    }
    *out = *(f32*)((s32)wp + 0);
}


void camShiftPostMain(void) {
    extern void* gp;
    extern void* wp;
    void* g;
    void* w;
    s32 temp;

    g = gp;
    w = wp;
    temp = *(s32*)((s32)g + 0x38);
    *(s32*)((s32)w + 0x1C) = *(s32*)((s32)g + 0x3C);
    *(s32*)((s32)w + 0x18) = temp;
}

void camShiftReset(void) {
    extern void* wp;
    extern f32 float_0_80421fe8;
    extern f32 float_300_80421fec;
    extern f32 float_3_80421ff0;
    extern f32 float_0p002_80421ff4;
    f32 zero = float_0_80421fe8;
    f32 dist = float_300_80421fec;
    f32 three = float_3_80421ff0;
    f32 small = float_0p002_80421ff4;

    *(f32*)((s32)wp + 0x0) = zero;
    *(f32*)((s32)wp + 0x4) = zero;
    *(f32*)((s32)wp + 0x8) = zero;
    *(f32*)((s32)wp + 0xC) = zero;
    *(f32*)((s32)wp + 0x10) = zero;
    *(f32*)((s32)wp + 0x20) = dist;
    *(f32*)((s32)wp + 0x24) = three;
    *(f32*)((s32)wp + 0x28) = small;
    *(f32*)((s32)wp + 0x2C) = zero;
    *(f32*)((s32)wp + 0x30) = zero;
}
