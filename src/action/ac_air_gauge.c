#include "action/ac_air_gauge.h"

void battleAcDelete_AirGauge(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

s32 battleAcResult_AirGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_AirGauge(void* camera, void* wp) {
    s32 disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80426fc8;
    extern f32 float_30_80426fcc;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (s32)wp + 0x1F20;
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
            *(f32*)(disp + 0x14) = intplGetValue(
                4,
                0x14 - *(s32*)(disp + 0x20),
                0x14,
                float_neg300_80426fc8,
                float_30_80426fcc);
            /* fallthrough */
        case 100:
        case 1000:
            actionCommandDisp(*(f32*)(disp + 0x14), *(f32*)(disp + 0x18));
            timer = *(s32*)(disp + 0x20);
            if (timer > 0) {
                *(s32*)(disp + 0x20) = timer - 1;
            }
            break;
        case 1002:
        case 1003:
            timer = *(s32*)(disp + 0x20);
            if (timer >= 0x28) {
                *(f32*)(disp + 0x14) = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80426fcc,
                    float_neg300_80426fc8);
            } else {
                *(f32*)(disp + 0x14) = float_30_80426fcc;
            }
            actionCommandDisp(*(f32*)(disp + 0x14), *(f32*)(disp + 0x18));
            timer = *(s32*)(disp + 0x20);
            if (timer < 0x3C) {
                *(s32*)(disp + 0x20) = timer + 1;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 _GetGaugeDecValue(void) {
    extern void* _battleWorkPointer;
    extern s32 lbl_803AEDB8[];
    extern f32 float_0p3_80426fd0;
    extern f32 float_0p33_80426fd4;
    extern f32 float_0p36_80426fd8;
    extern f32 float_0p4_80426fdc;
    extern f32 float_0p44_80426fe0;
    extern f32 float_0p47_80426fe4;
    extern f32 float_0p5_80426fe8;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    s32 battleWork;
    s32 tableBase;
    s32 gaugeWork;
    s32 entry;
    f32 value;
    s32 count;

    battleWork = (s32)_battleWorkPointer;
    tableBase = (s32)lbl_803AEDB8;
    gaugeWork = battleWork + 0x1F4C;

    switch (*(u32*)(battleWork + 0x1CE4)) {
        case 0:
            value = float_0p3_80426fd0;
            entry = tableBase + 0x40;
            break;
        case 1:
            value = float_0p33_80426fd4;
            entry = tableBase + 0x7C;
            break;
        case 2:
            value = float_0p36_80426fd8;
            entry = tableBase + 0xB8;
            break;
        case 3:
            value = float_0p4_80426fdc;
            entry = tableBase + 0xF4;
            break;
        case 4:
            value = float_0p44_80426fe0;
            entry = tableBase + 0x130;
            break;
        case 5:
            value = float_0p47_80426fe4;
            entry = tableBase + 0x16C;
            break;
        case 6:
        default:
            entry = tableBase + 0x1A8;
            value = float_0p5_80426fe8;
            break;
    }

    count = *(s32*)(gaugeWork + 0xC);
    while (count > *(u32*)entry) {
        entry += 0xC;
        if (*(s32*)entry < 0) {
            break;
        }
    }

    if (*(s32*)entry <= 0) {
        *(s32*)(gaugeWork + 0x10) = 1;
        return value;
    }

    *(s32*)(gaugeWork + 0x10) = *(s32*)(entry + 8);
    value -= intplGetValue(
        0,
        count - *(s32*)(entry - 0xC),
        *(s32*)entry - *(s32*)(entry - 0xC),
        *(f32*)(entry - 8),
        *(f32*)(entry + 4));
    return value;
}

s32 battleAcMain_AirGauge(void* battleWork) {
    extern void* memset(void* dest, int ch, u32 count);
    extern s32 irand(s32 range);
    extern void BattleAcGaugeSeInit(void);
    extern s32 BattlePadCheckNow(u32 buttons);
    extern s32 BattlePadCheckTrigger(u32 buttons);
    extern f32 _GetGaugeDecValue(void);
    extern u8 BattleAcGaugeSeUpdate(double value);
    extern void BattleAcGaugeSeDelete(void);
    extern f32 float_neg300_80426fc8;
    extern f32 float_0_80426fec;
    extern f32 float_100_80426fb4;
    extern u32 dat_80426fb0;

    s32 bw;
    s32 work;
    s32 disp;
    s32 state;
    s32 unit;
    s32 value;
    s32 rank;
    f32 fill;
    f32 dec;

    bw = (s32)battleWork;
    work = bw + 0x1F4C;
    disp = bw + 0x1F20;
    state = *(s32*)(bw + 0x1C9C);

    if (state == 1000) {
        goto state_1000;
    }

    if (state >= 1000) {
        if (state == 1003) {
            goto state_1003;
        }
        if (state >= 1003) {
            if (state >= 1005) {
                return 1;
            }
            goto state_1004;
        }
        if (state >= 1002) {
            goto state_1002;
        }
        goto state_1001;
    }

    if (state == 99) {
        return 1;
    }

    if (state < 99) {
        if (state != 0) {
            return 1;
        }

        memset((void*)disp, 0, 0x2C);
        *(f32*)(disp + 0x14) = float_neg300_80426fc8;
        *(f32*)(disp + 0x18) = float_0_80426fec;
        *(s32*)(disp + 0x20) = 0x14;
        *(f32*)(disp + 0x28) = float_0_80426fec;
        *(u32*)(disp + 0xC) = dat_80426fb0;

        *(s32*)work = 0;
        *(s32*)(bw + 0x1CE8) = 0;
        *(s32*)(bw + 0x1CF0) = 0;
        *(s32*)(bw + 0x1CEC) = 0;
        *(s32*)(bw + 0x1C9C) = 99;
        *(s32*)(bw + 0x1CB8) = 1;
        *(f32*)(work + 4) = float_0_80426fec;
        *(s32*)(work + 8) = 0;
        *(s32*)(work + 0xC) = 0;
        *(s32*)(work + 0x10) = 0;
        return 1;
    }

    if (state > 100) {
        return 1;
    }

    if ((*(u32*)(bw + 0x1C94) & 1) != 0) {
        unit = *(s32*)(bw + 0x1C90);
        if ((*(u32*)(unit + 0x27C) & 0x10) != 0) {
            value = irand(100);
            if (value >= 0) {
                value = *(s32*)(bw + 0x1CD4);
                *(s32*)(bw + 0x1C98) = value / 2;
                *(s32*)(bw + 0x1CE8) = 0;
                *(s32*)(bw + 0x1CF0) = 0;
            } else {
                value = irand(*(s32*)(bw + 0x1CD0) - 1);
                rank = value + 1;
                *(s32*)(bw + 0x1CE8) = rank;

                if (rank == 3) {
                    *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1CDC) - 1;
                } else if (rank < 3) {
                    if (rank == 1) {
                        *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1CD4) - 1;
                    } else if (rank >= 1) {
                        *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1CD8) - 1;
                    } else {
                        *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1CDC) + 1;
                    }
                } else {
                    *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1CDC) + 1;
                }

                *(s32*)(bw + 0x1CF0) = 100;
            }

            *(s32*)(bw + 0x1C9C) = 1001;
            return 1;
        }
    }

    BattleAcGaugeSeInit();
    *(s32*)(bw + 0x1C9C) = 1000;

state_1000:
    *(s32*)work = *(s32*)work + 1;

    if (BattlePadCheckNow(0x100) != 0) {
        *(s32*)(bw + 0x1CEC) = 1;
        if (BattlePadCheckTrigger(0x100) != 0) {
            *(s32*)(work + 8) = *(s32*)(work + 8) + 1;
            *(s32*)(work + 0xC) = 0;
        }
        *(s32*)(work + 0xC) = *(s32*)(work + 0xC) + 1;
    } else {
        *(s32*)(bw + 0x1CEC) = 0;
        *(s32*)(work + 0xC) = 0;
    }

    dec = _GetGaugeDecValue();
    *(f32*)(work + 4) = *(f32*)(work + 4) - dec;

    unit = *(s32*)(bw + 0x1C90);
    if (*(u8*)(unit + 0x307) != 0) {
        *(s32*)(work + 8) = 1;
        *(f32*)(work + 4) = float_100_80426fb4;
    }

    if (*(f32*)(work + 4) < float_0_80426fec) {
        *(f32*)(work + 4) = float_0_80426fec;
    }

    if (float_100_80426fb4 < *(f32*)(work + 4)) {
        *(f32*)(work + 4) = float_100_80426fb4;
    }

    BattleAcGaugeSeUpdate(*(f32*)(work + 4));

    if (*(s32*)work < *(s32*)(bw + 0x1CCC)) {
        return 1;
    }

    unit = *(s32*)(bw + 0x1C90);
    if (*(u8*)(unit + 0x307) != 0) {
        *(s32*)(work + 8) = 1;
        *(f32*)(work + 4) = float_100_80426fb4;
    }

    if (*(s32*)(work + 8) == 0) {
        *(s32*)(bw + 0x1CE8) = 0;
        *(s32*)(bw + 0x1CF0) = 0;
        *(s32*)(bw + 0x1CB8) = 0;
        BattleAcGaugeSeDelete();
        return 0;
    }

    *(s32*)(bw + 0x1CB8) = 2;
    *(s32*)(bw + 0x1CB4) = *(s32*)(bw + 0x1CB4) + 1;

    fill = *(f32*)(work + 4);

    if (fill < (f32)*(s32*)(bw + 0x1CD4)) {
        *(s32*)(bw + 0x1CE8) = 1;
    } else {
        rank = *(s32*)(bw + 0x1CD0);
        if (rank < 3) {
            *(s32*)(bw + 0x1CE8) = 2;
        } else if (fill < (f32)*(s32*)(bw + 0x1CD8)) {
            *(s32*)(bw + 0x1CE8) = 2;
        } else if (rank < 4) {
            *(s32*)(bw + 0x1CE8) = 3;
        } else if (fill < (f32)*(s32*)(bw + 0x1CDC)) {
            *(s32*)(bw + 0x1CE8) = 3;
        } else if (rank < 5) {
            *(s32*)(bw + 0x1CE8) = 4;
        } else if (fill < (f32)*(s32*)(bw + 0x1CE0)) {
            *(s32*)(bw + 0x1CE8) = 4;
        } else {
            *(s32*)(bw + 0x1CE8) = 5;
        }
    }

    *(s32*)(bw + 0x1CF0) = (s32)*(f32*)(work + 4);
    *(s32*)(bw + 0x1C9C) = 1002;
    return 1;

state_1001:
    *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1C98) - 1;
    if (*(s32*)(bw + 0x1C98) >= 0) {
        return 1;
    }

    if (*(s32*)(bw + 0x1CE8) <= 0) {
        *(s32*)(bw + 0x1CB8) = 0;
    } else {
        *(s32*)(bw + 0x1CB8) = 2;
        *(s32*)(bw + 0x1CB4) = *(s32*)(bw + 0x1CB4) + 1;
    }
    return 0;

state_1002:
    BattleAcGaugeSeDelete();
    *(s32*)(work + 0x14) = 0x3C;
    *(s32*)(bw + 0x1C9C) = 1003;

state_1003:
    *(s32*)(work + 0x14) = *(s32*)(work + 0x14) - 1;
    if (*(s32*)(work + 0x14) > 0) {
        return 1;
    }
    *(s32*)(bw + 0x1C9C) = 1004;
    return 1;

state_1004:
    *(s32*)(bw + 0x1CA0) = 0;
    *(s32*)(bw + 0x1CA8) = 0;
    *(s32*)(bw + 0x1CA4) = 0;
    *(s32*)(bw + 0x1CAC) = 0;
    return 0;
}

void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* _battleWorkPointer;
    extern Vec vec3_802fe5b0;
    extern Vec vec3_802fe5bc;
    extern f32 float_100_80426fb4;
    extern f32 float_0p01_80426fb8;
    extern f32 float_neg200_80426fbc;
    extern f32 float_58_80426fc0;
    extern f32 float_1_80426fc4;
    extern void* camGetPtr(s32 cameraId);
    extern void BattleAcDrawGauge(s32 x, s32 y, s32 icon, s32 a, s32 b, s32 c, s32 d, s32 zero, f32 value);
    extern void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);

    void* bw;
    void* disp;
    void* gauge;
    f32 percent;
    f32 target;
    Vec posOn;
    Vec posOff;
    s32 ix;
    s32 iy;

    bw = _battleWorkPointer;
    disp = (void*)((s32)bw + 0x1F20);

    camGetPtr(8);

    gauge = (void*)((s32)bw + 0x1F4C);
    target = *(f32*)((s32)gauge + 4) / float_100_80426fb4;
    percent = *(f32*)((s32)disp + 0x28);

    if (target <= percent) {
        percent += float_0p01_80426fb8;
        *(f32*)((s32)disp + 0x28) = percent;
        if (target < *(f32*)((s32)disp + 0x28)) {
            *(f32*)((s32)disp + 0x28) = target;
        }
    } else {
        percent -= float_0p01_80426fb8;
        *(f32*)((s32)disp + 0x28) = percent;
        if (*(f32*)((s32)disp + 0x28) < target) {
            *(f32*)((s32)disp + 0x28) = target;
        }
    }

    percent = *(f32*)((s32)disp + 0x28);
    ix = (s32)x;
    iy = (s32)y;

    BattleAcDrawGauge(
        ix,
        iy,
        0xB2,
        *(s32*)((s32)bw + 0x1CD0),
        *(s32*)((s32)bw + 0x1CD4),
        *(s32*)((s32)bw + 0x1CD8),
        *(s32*)((s32)bw + 0x1CDC),
        0,
        percent);

    if (*(s32*)((s32)gauge + 0x10) != 0) {
        posOn = vec3_802fe5b0;
        posOn.x = float_neg200_80426fbc + x;
        posOn.y = float_58_80426fc0 + y;
        iconDispGx(&posOn, 0x10, 0x6D, float_1_80426fc4);
    } else {
        posOff = vec3_802fe5bc;
        posOff.x = float_neg200_80426fbc + x;
        posOff.y = float_58_80426fc0 + y;
        iconDispGx(&posOff, 0x10, 0x6C, float_1_80426fc4);
    }
}
