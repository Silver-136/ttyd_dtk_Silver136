#include "action/ac_power_gauge.h"
s32 battleAcMain_PowerGauge(void* battleWork) {
    extern void* memset(void* dest, int ch, u32 count);
    extern s32 irand(s32 range);
    extern void BattleAcGaugeSeInit(void);
    extern s32 BattlePadGetNow(void);
    extern s32 BattlePadCheckTrigger(u32 buttons);
    extern s32 BattlePadCheckNow(u32 buttons);
    extern s32 BattlePadCheckUp(u32 buttons);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void BattleAcGaugeSeUpdate(double value);
    extern void BattleAcGaugeSeDelete(void);

    extern f32 float_neg300_80427b14;
    extern f32 float_0_80427b1c;
    extern f32 float_100_80427ae4;
    extern f32 float_101_80427b20;
    extern u32 dat_80427ae0;

    s32 bw;
    s32 work;
    s32 disp;
    s32 unit;
    s32 autoCommand;
    s32 state;
    s32 value;
    s32 button;
    f32 gauge;
    s32 now;

    bw = (s32)battleWork;
    autoCommand = 0;
    work = bw + 0x1F4C;
    disp = bw + 0x1F20;
    unit = *(s32*)(bw + 0x1C90);

    if (*(u8*)(unit + 0x307) != 0) {
        autoCommand = 1;
    }

    state = *(s32*)(bw + 0x1C9C);

    if (state == 0x3EA) {
        goto state_3ea;
    }

    if (state >= 0x3EA) {
        if (state == 0x3ED) {
            goto state_3ed;
        }
        if (state >= 0x3ED) {
            if (state >= 0x3EF) {
                return 1;
            }
            goto state_3ee;
        }
        if (state >= 0x3EC) {
            goto state_3ec;
        }
        goto state_3eb;
    }

    if (state == 99) {
        return 1;
    }

    if (state < 99) {
        if (state != 0) {
            return 1;
        }

        memset((void*)disp, 0, 0x2C);
        *(f32*)(disp + 0x14) = float_neg300_80427b14;
        *(f32*)(disp + 0x18) = float_0_80427b1c;
        *(s32*)(disp + 0x20) = 0x14;
        *(f32*)(disp + 0x28) = float_0_80427b1c;
        *(u32*)(disp + 0xC) = dat_80427ae0;

        *(s32*)work = 0;
        *(s32*)(work + 8) = 0;
        *(s32*)(bw + 0x1CE8) = 0;
        *(s32*)(bw + 0x1C9C) = 99;
        *(s32*)(bw + 0x1CB8) = 1;
        *(s32*)(work + 0xC) = *(s32*)(bw + 0x1CCC) + *(s32*)(bw + 0x1CD0);
        *(f32*)(work + 4) = float_0_80427b1c;

        value = *(s32*)(bw + 0x1CC8);
        if (value == 2) {
            *(s8*)(work + 0x12) = 0;
            *(s32*)(work + 0x14) = 0x100;
            *(s8*)(work + 0x11) = 0;
        } else if (value < 2) {
            if (value == 0) {
                *(s8*)(work + 0x12) = 1;
                *(s32*)(work + 0x14) = 0x60;
                *(s8*)(work + 0x11) = 1;
                *(s32*)(bw + 0x1CE8) = 1;
                return 1;
            }
            if (value > -1) {
                *(s8*)(work + 0x12) = 1;
                *(s32*)(work + 0x14) = 0x100;
                *(s8*)(work + 0x11) = 1;
                *(s32*)(bw + 0x1CE8) = 1;
                return 1;
            }
            *(s8*)(work + 0x12) = 1;
            *(s32*)(work + 0x14) = 0x100;
            *(s8*)(work + 0x11) = 1;
            *(s32*)(bw + 0x1CE8) = 1;
        } else if (value < 4) {
            *(s8*)(work + 0x12) = 0;
            *(s32*)(work + 0x14) = 0x20;
            *(s8*)(work + 0x11) = 0;
            return 1;
        } else {
            *(s8*)(work + 0x12) = 1;
            *(s32*)(work + 0x14) = 0x100;
            *(s8*)(work + 0x11) = 1;
            *(s32*)(bw + 0x1CE8) = 1;
        }

        return 1;
    }

    if (state > 100) {
        return 1;
    }

    if ((*(u32*)(bw + 0x1C94) & 1) != 0 && (*(u32*)(unit + 0x27C) & 0x10) != 0) {
        value = irand(100);
        if (value < 0) {
            *(s8*)(work + 0x10) = 1;
            *(s32*)(bw + 0x1C98) = 0x1E;
        } else {
            *(s32*)(bw + 0x1C98) = 0x1E;
            *(s8*)(work + 0x10) = 0;
        }
        *(s32*)(bw + 0x1CE8) = 1;
        *(s32*)(bw + 0x1C9C) = 0x3EA;
        return 1;
    }

    if (*(s8*)(work + 0x12) == 0) {
        *(s32*)(bw + 0x1C9C) = 0x3E8;
    } else {
        *(s32*)(bw + 0x1C9C) = 0x3E9;
    }

    if (*(s8*)(work + 0x12) != 0) {
        BattleAcGaugeSeInit();
    }

state_loop:
    *(s32*)work = *(s32*)work + 1;

    if (*(s8*)(work + 0x11) == 1) {
        if (*(s8*)(work + 0x12) == 0) {
            button = *(s32*)(work + 0x14);
            if (BattlePadCheckNow(button) == 0) {
                if (BattlePadCheckUp(button) != 0) {
                    *(s8*)(work + 0x11) = 2;
                }
            }
        } else {
            now = BattlePadGetNow();
            button = *(s32*)(work + 0x14);
            if (button == now) {
                if (BattlePadCheckTrigger(button) != 0) {
                    *(s8*)(work + 0x11) = 2;
                }
            }
        }
    } else if (*(s8*)(work + 0x11) < 1) {
        if (*(s8*)(work + 0x11) > -1) {
            if (*(s8*)(work + 0x12) == 0) {
                now = BattlePadGetNow();
                if (*(s32*)(work + 0x14) == now || autoCommand != 0) {
                    *(s8*)(work + 0x11) = 1;
                    *(s32*)(bw + 0x1C9C) = 0x3E9;
                    *(s32*)work = 0;
                    *(s32*)(bw + 0x1CE8) = 1;
                    BattleAcGaugeSeInit();
                }
            }
        }
    }

    if ((*(u32*)(bw + 0x1CC4) & 1) == 0) {
        *(s32*)(work + 8) = *(s32*)(work + 8) + 1;
    } else if (*(u8*)(work + 0x11) == 1) {
        *(s32*)(work + 8) = *(s32*)(work + 8) + 1;
    }

    if (*(s32*)(bw + 0x1C9C) == 0x3E8) {
        if (*(s32*)work > 0x78) {
            *(s32*)work = 0;
            *(s32*)(bw + 0x1CB8) = 0;
            *(f32*)(work + 4) = float_0_80427b1c;
            *(s32*)(bw + 0x1C9C) = 0x3EB;
            *(s32*)(bw + 0x1CE8) = 2;
            return 1;
        }
    } else {
        if (*(s32*)work > *(s32*)(work + 0xC)) {
            *(s32*)work = 0;
            *(s32*)(bw + 0x1CB8) = 0;
            *(f32*)(work + 4) = float_101_80427b20;
            *(s32*)(bw + 0x1C9C) = 0x3EB;
            return 1;
        }
    }

    if (*(s32*)work > *(s32*)(bw + 0x1CCC)) {
        *(f32*)(work + 4) = float_100_80427ae4;
    } else {
        *(f32*)(work + 4) = intplGetValue(
            *(s32*)(bw + 0x1CD4),
            *(s32*)(work + 8),
            *(s32*)(bw + 0x1CCC),
            float_0_80427b1c,
            float_100_80427ae4);
        if (*(f32*)(work + 4) > float_100_80427ae4) {
            *(f32*)(work + 4) = float_100_80427ae4;
        }
    }

    BattleAcGaugeSeUpdate(*(f32*)(work + 4));

    if (autoCommand != 0) {
        if (*(f32*)(work + 4) >= (f32)*(s32*)(bw + 0x1CD8)) {
            *(s8*)(work + 0x11) = 2;
        } else {
            *(s8*)(work + 0x11) = 1;
        }
    }

    if (*(u8*)(work + 0x11) == 2) {
        if (*(f32*)(work + 4) < (f32)*(s32*)(bw + 0x1CD8)) {
            *(s32*)(bw + 0x1CB8) = 0;
            *(s32*)(bw + 0x1CB8) = *(s32*)(bw + 0x1CB8) & ~2;
        } else {
            *(s32*)(bw + 0x1CB8) = 2;
            *(s32*)(bw + 0x1CB4) = *(s32*)(bw + 0x1CB4) + 1;
        }
        *(s32*)(bw + 0x1C9C) = 0x3EB;
    }

    return 1;

state_3ea:
    *(s32*)(bw + 0x1C98) = *(s32*)(bw + 0x1C98) - 1;
    if (*(s32*)(bw + 0x1C98) >= 0) {
        return 1;
    }

    if (*(s8*)(work + 0x10) == 0) {
        *(s32*)(bw + 0x1CB8) = 0;
    } else {
        *(s32*)(bw + 0x1CB8) = 2;
        *(s32*)(bw + 0x1CB4) = *(s32*)(bw + 0x1CB4) + 1;
    }

    *(u32*)(bw + 0x1CC0) = *(u32*)(bw + 0x1CC0) | 1;
    *(s32*)(work + 0x18) = 0x3C;
    *(s32*)(bw + 0x1C9C) = 0x3ED;
    return 0;

state_3eb:
    *(s32*)(bw + 0x1C9C) = 0x3EC;

state_3ec:
    *(u32*)(bw + 0x1CC0) = *(u32*)(bw + 0x1CC0) | 1;
    BattleAcGaugeSeDelete();
    *(s32*)(work + 0x18) = 0x3C;
    *(s32*)(bw + 0x1C9C) = 0x3ED;

state_3ed:
    *(s32*)(work + 0x18) = *(s32*)(work + 0x18) - 1;
    if (*(s32*)(work + 0x18) > 0) {
        return 1;
    }
    *(s32*)(bw + 0x1C9C) = 0x3EE;
    return 1;

state_3ee:
    *(s32*)(bw + 0x1CA0) = 0;
    *(s32*)(bw + 0x1CA8) = 0;
    *(s32*)(bw + 0x1CA4) = 0;
    *(s32*)(bw + 0x1CAC) = 0;
    return 0;
}

s32 battleAcResult_PowerGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_PowerGauge(void* camera, void* wp) {
    typedef struct PowerGaugeDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } PowerGaugeDispWork;
    PowerGaugeDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80427b14;
    extern f32 float_30_80427b18;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (PowerGaugeDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
            disp->x = intplGetValue(
                4,
                0x14 - disp->timer,
                0x14,
                float_neg300_80427b14,
                float_30_80427b18);
            /* fallthrough */
        case 100:
        case 1000:
        case 1001:
        case 1003:
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1004:
        case 1005:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427b18,
                    float_neg300_80427b14);
            } else {
                disp->x = float_30_80427b18;
            }
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer < 0x3C) {
                disp->timer = timer + 1;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void battleAcDelete_PowerGauge(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}


/* CHATGPT STUB FILL: main/action/ac_power_gauge 20260624_184823 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct VecLocal {
        f32 x, y, z;
    } VecLocal;
    extern void* _battleWorkPointer;
    extern void* camGetPtr(s32);
    extern void BattleAcDrawGauge(s32, s32, s32, s32, s32, s32, s32, s32);
    extern s32 BattleACGetButtonIcon(u32, s32);
    extern void iconDispGx(f64, VecLocal*, s32, s32);

    u8* wp = (u8*)_battleWorkPointer;
    u8* extra = wp + 0x1F4C;
    s32* params = (s32*)(wp + 0x1CC8);
    f32 value = *(f32*)(extra + 4);
    f32 ratio = value / 100.0f;
    u32 flags = *(u32*)(wp + 0x1C94);
    s32 mode = *(s32*)(wp + 0x1C9C);
    s32 left = 0;
    s32 right = 0;
    s32 left2 = 0;
    s32 right2 = 0;
    VecLocal pos;

    camGetPtr(1);
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    *(f32*)(wp + 0x1F28) = ratio;

    if ((flags & 2) == 0) {
        BattleAcDrawGauge((s32)x, (s32)y, 0xB2, 2, params[4], 100, 100, 0);
    } else {
        BattleAcDrawGauge((s32)x, (s32)y, 0xB2, 1, 100, 100, 100, 0);
    }

    if ((flags & 4) == 0) {
        pos.x = x - 288.0f + (176.0f * (f32)params[4]) / 100.0f;
        pos.y = y + 45.0f;
        pos.z = 0.0f;
        iconDispGx(1.0, &pos, 0x10, 0x9E);
    } else {
        pos.x = x - 80.0f - ((flags & 8) ? 12.0f : 0.0f);
        pos.y = y + 18.0f;
        pos.z = 0.0f;
        iconDispGx(1.0, &pos, 0x10,
                   (value < (f32)params[4] || value > 100.0f) ? 0x99 : 0x9D);
    }

    if (params[0] == 2) {
        left = BattleACGetButtonIcon(0x100, 1);
        right = BattleACGetButtonIcon(0x100, 0);
    } else if (params[0] < 2) {
        if (params[0] == 0) {
            left = 0x87;
            right = 0x86;
            left2 = 0x89;
            right2 = 0x88;
        } else if (params[0] > -1) {
            left = BattleACGetButtonIcon(0x100, 1);
            right = BattleACGetButtonIcon(0x100, 0);
        }
    } else if (params[0] < 4) {
        left = BattleACGetButtonIcon(0x20, 1);
        right = BattleACGetButtonIcon(0x20, 0);
    }

    if (mode == 1002) {
        return;
    }
    if (mode < 1002) {
        if (mode > 100) {
            if (mode < 1000) {
                return;
            }
        } else if (mode < 99) {
            return;
        }
    } else if (mode > 1005) {
        return;
    }
    {
        s32 primary;
        s32 secondary;
        s32 usePressed = 0;
        if (*(u8*)(extra + 0x12) == 0) {
            if ((value < (f32)params[4] || value > 100.0f) &&
                *(s32*)(wp + 0x1CB8) != 0) {
                usePressed = 1;
            }
        } else if ((f32)params[4] <= value && value <= 100.0f) {
            usePressed = 1;
        }
        primary = usePressed ? left : right;
        secondary = usePressed ? left2 : right2;
        pos.y = y + 80.0f;
        pos.z = 0.0f;
        pos.x = x - 225.0f;
        if (secondary != 0) {
            iconDispGx(1.0, &pos, 0x10, primary);
            pos.x += 56.0f;
            iconDispGx(1.0, &pos, 0x10, secondary);
        } else {
            pos.x += 28.0f;
            iconDispGx(1.0, &pos, 0x10, primary);
        }
    }
}

