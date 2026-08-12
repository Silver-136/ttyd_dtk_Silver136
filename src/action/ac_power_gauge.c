#include "action/ac_power_gauge.h"

#include "system.h"

void actionCommandDisp(f32 x, f32 y);
s32 battleAcMain_PowerGauge(void* battleWork) {
    extern void BattleAcGaugeSeInit(void);
    extern s32 BattlePadGetNow(void);
    extern s32 BattlePadCheckTrigger(u32 buttons);
    extern s32 BattlePadCheckNow(u32 buttons);
    extern s32 BattlePadCheckUp(u32 buttons);
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
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    typedef struct PowerGaugeDispWorkLocal {
        u8 pad_00[0x28];
        f32 ratioGaugeFilled;
    } PowerGaugeDispWorkLocal;

    typedef struct PowerGaugeExtraWorkLocal {
        s32 field_00;
        f32 gaugeValue;
        u8 pad_08[0xA];
        s8 inputMode;
    } PowerGaugeExtraWorkLocal;

    extern void* _battleWorkPointer;
    extern void* camGetPtr(s32 cameraId);
    extern void BattleAcDrawGauge(
        s32 x, s32 y, s32 icon, s32 mode, s32 value,
        s32 bar1, s32 bar2, s32 flags);
    extern u16 BattleACGetButtonIcon(u32 button, s32 pressed);
    extern void iconDispGx(f64 scale, VecLocal* pos, u16 flags, u16 iconId);

    u8* battleWork = (u8*)_battleWorkPointer;
    PowerGaugeDispWorkLocal* dispWork =
        (PowerGaugeDispWorkLocal*)(battleWork + 0x1F20);
    PowerGaugeExtraWorkLocal* extraWork =
        (PowerGaugeExtraWorkLocal*)(battleWork + 0x1F4C);
    s32* params = (s32*)(battleWork + 0x1CC8);
    u32 acFlags;
    s32 state;
    u16 pressedIcon = 0;
    u16 releasedIcon = 0;
    u16 pressedSecondIcon = 0;
    u16 releasedSecondIcon = 0;
    f32 ratio;

    camGetPtr(8);

    ratio = extraWork->gaugeValue / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    dispWork->ratioGaugeFilled = ratio;

    if ((*(u32*)(battleWork + 0x1CC4) & 2) != 0) {
        BattleAcDrawGauge(
            (s32)x, (s32)y, 0xB2, 1, 100, 100, 100, 0);
    } else {
        BattleAcDrawGauge(
            (s32)x, (s32)y, 0xB2, 2, params[4], 100, 100, 0);
    }

    acFlags = *(u32*)(battleWork + 0x1CC4);

    if ((acFlags & 4) == 0) {
        VecLocal gaugeMarker = {0.0f, 0.0f, 0.0f};

        gaugeMarker.x =
            (x + -288.0f) + (f32)(params[4] * 176) / 100.0f;
        gaugeMarker.y = y + 45.0f;
        iconDispGx(1.0, &gaugeMarker, 0x10, 0x9E);
    } else {
        s32 markerIcon;

        if (extraWork->gaugeValue < (f32)params[4] ||
            extraWork->gaugeValue > 100.0f) {
            markerIcon = 0x99;
        } else {
            markerIcon = 0x9D;
        }

        if ((acFlags & 8) == 0) {
            VecLocal marker = {0.0f, 0.0f, 0.0f};

            marker.x = 120.0f + (x + -200.0f);
            marker.y = y + 18.0f;
            iconDispGx(1.0, &marker, 0x10, markerIcon);
        } else {
            VecLocal marker = {0.0f, 0.0f, 0.0f};

            marker.x = (120.0f + (x + -200.0f)) - 12.0f;
            marker.y = y + 18.0f;
            iconDispGx(1.0, &marker, 0x10, markerIcon);
        }
    }

    if (params[0] == 2) {
        pressedIcon = BattleACGetButtonIcon(0x100, 1);
        releasedIcon = BattleACGetButtonIcon(0x100, 0);
        pressedSecondIcon = 0;
        releasedSecondIcon = 0;
    } else if (params[0] < 2) {
        if (params[0] == 0) {
            pressedIcon = 0x87;
            releasedIcon = 0x86;
            pressedSecondIcon = 0x89;
            releasedSecondIcon = 0x88;
        } else if (params[0] > -1) {
            pressedIcon = BattleACGetButtonIcon(0x100, 1);
            releasedIcon = BattleACGetButtonIcon(0x100, 0);
            pressedSecondIcon = 0;
            releasedSecondIcon = 0;
        }
    } else if (params[0] < 4) {
        pressedIcon = BattleACGetButtonIcon(0x20, 1);
        releasedIcon = BattleACGetButtonIcon(0x20, 0);
        pressedSecondIcon = 0;
        releasedSecondIcon = 0;
    }

    state = *(s32*)(battleWork + 0x1C9C);

    if (state == 1002) {
        return;
    }

    if (state < 1002) {
        if (state > 100) {
            if (state < 1000) {
                return;
            }

            if (extraWork->inputMode == 0) {
                if (((extraWork->gaugeValue < (f32)params[4]) ||
                     (extraWork->gaugeValue > 100.0f)) &&
                    (*(s32*)(battleWork + 0x1CE8) != 0)) {
                    if (pressedSecondIcon != 0) {
                        VecLocal first = {0.0f, 0.0f, 0.0f};
                        VecLocal second = {0.0f, 0.0f, 0.0f};

                        first.x = x + -225.0f;
                        first.y = y + 80.0f;
                        iconDispGx(
                            1.0, &first, 0x10, pressedIcon);

                        second.x = x + -225.0f + 56.0f;
                        second.y = y + 80.0f;
                        iconDispGx(
                            1.0, &second, 0x10, pressedSecondIcon);
                        return;
                    } else {
                        VecLocal single = {0.0f, 0.0f, 0.0f};

                        single.x = x + -225.0f + 28.0f;
                        single.y = y + 80.0f;
                        iconDispGx(
                            1.0, &single, 0x10, pressedIcon);
                        return;
                    }
                }

                if (pressedSecondIcon != 0) {
                    VecLocal first = {0.0f, 0.0f, 0.0f};
                    VecLocal second = {0.0f, 0.0f, 0.0f};

                    first.x = x + -225.0f;
                    first.y = y + 80.0f;
                    iconDispGx(
                        1.0, &first, 0x10, releasedIcon);

                    second.x = x + -225.0f + 56.0f;
                    second.y = y + 80.0f;
                    iconDispGx(
                        1.0, &second, 0x10, releasedSecondIcon);
                    return;
                } else {
                    VecLocal single = {0.0f, 0.0f, 0.0f};

                    single.x = x + -225.0f + 28.0f;
                    single.y = y + 80.0f;
                    iconDispGx(
                        1.0, &single, 0x10, releasedIcon);
                    return;
                }
            } else {
                if ((f32)params[4] <= extraWork->gaugeValue &&
                    extraWork->gaugeValue <= 100.0f) {
                    if (pressedSecondIcon != 0) {
                        VecLocal first = {0.0f, 0.0f, 0.0f};
                        VecLocal second = {0.0f, 0.0f, 0.0f};

                        first.x = x + -225.0f;
                        first.y = y + 80.0f;
                        iconDispGx(
                            1.0, &first, 0x10, pressedIcon);

                        second.x = x + -225.0f + 56.0f;
                        second.y = y + 80.0f;
                        iconDispGx(
                            1.0, &second, 0x10, pressedSecondIcon);
                        return;
                    } else {
                        VecLocal single = {0.0f, 0.0f, 0.0f};

                        single.x = x + -225.0f + 28.0f;
                        single.y = y + 80.0f;
                        iconDispGx(
                            1.0, &single, 0x10, pressedIcon);
                        return;
                    }
                }

                if (pressedSecondIcon != 0) {
                    VecLocal first = {0.0f, 0.0f, 0.0f};
                    VecLocal second = {0.0f, 0.0f, 0.0f};

                    first.x = x + -225.0f;
                    first.y = y + 80.0f;
                    iconDispGx(
                        1.0, &first, 0x10, releasedIcon);

                    second.x = x + -225.0f + 56.0f;
                    second.y = y + 80.0f;
                    iconDispGx(
                        1.0, &second, 0x10, releasedSecondIcon);
                    return;
                } else {
                    VecLocal single = {0.0f, 0.0f, 0.0f};

                    single.x = x + -225.0f + 28.0f;
                    single.y = y + 80.0f;
                    iconDispGx(
                        1.0, &single, 0x10, releasedIcon);
                    return;
                }
            }
        }

        if (state < 99) {
            return;
        }
    } else if (state > 1005) {
        return;
    }

    if (pressedSecondIcon == 0) {
        VecLocal single = {0.0f, 0.0f, 0.0f};

        single.x = x + -225.0f + 28.0f;
        single.y = y + 80.0f;
        iconDispGx(1.0, &single, 0x10, releasedIcon);
    } else {
        VecLocal first = {0.0f, 0.0f, 0.0f};
        VecLocal second = {0.0f, 0.0f, 0.0f};

        first.x = x + -225.0f;
        first.y = y + 80.0f;
        iconDispGx(1.0, &first, 0x10, releasedIcon);

        second.x = x + -225.0f + 56.0f;
        second.y = y + 80.0f;
        iconDispGx(
            1.0, &second, 0x10, releasedSecondIcon);
    }
}

