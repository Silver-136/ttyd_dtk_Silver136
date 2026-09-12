#include "action/ac_power_gauge_lv2.h"

void actionCommandDisp(f32 x, f32 y);

const f32 vec3_80300b60[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300b6c[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300b78[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300b84[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300b90[3] = { 0.0f, 0.0f, 0.0f };

s32 battleAcMain_PowerGaugeLv2(s32 battle) {
    extern u32 BattleActionCommandGetDifficulty(void*);
    extern s32 sysMsec2Frame(s32);
    extern s32 irand(s32);
    extern void BattleAcGaugeSeInit(void);
    extern void BattleAcGaugeSeUpdate(f64);
    extern void BattleAcGaugeSeDelete(void);
    extern u32 BattlePadCheckNow(u32);
    extern s32 _power_table[];
    extern s32 _power_table2[];
    extern u32 dat_804281b8;

    s32 bw;
    s32 work;
    s32 disp;
    s32 unit;
    u32 difficulty;
    s32 autoCommand;
    s32 state;

    bw = battle;
    work = bw + 0x1F4C;
    disp = bw + 0x1F20;
    difficulty = BattleActionCommandGetDifficulty((void*)bw);
    unit = *(s32*)(bw + 0x1C90);
    autoCommand = 0;
    if (*(u8*)(unit + 0x307) != 0) {
        autoCommand = 1;
    }

    state = *(s32*)(bw + 0x1C9C);

    if (state == 1000) {
        goto state_1000;
    }
    if (state >= 1000) {
        goto high_states;
    }
    if (state == 99) {
        goto return_one;
    }
    if (state >= 99) {
        goto low_99_plus;
    }
    if (state == 0) {
        goto state_0;
    }
    goto return_one;

low_99_plus:
    if (state >= 101) {
        goto return_one;
    }
    goto state_100;

high_states:
    if (state == 1004) {
        goto state_1004;
    }
    if (state >= 1004) {
        goto high_1004_plus;
    }
    if (state == 1002) {
        goto state_1002;
    }
    if (state >= 1002) {
        goto state_1003;
    }
    goto state_1001;

high_1004_plus:
    if (state >= 1006) {
        goto return_one;
    }
    goto state_1005;

state_0:
    {
        s32 i;
        s32 offset;
        s32 seconds;
        s32 random;

        memset((void*)disp, 0, 0x2C);
        *(f32*)(disp + 0x14) = -300.0f;
        *(f32*)(disp + 0x18) = 0.0f;
        *(s32*)(disp + 0x20) = 0x14;
        *(f32*)(disp + 0x28) = 0.0f;
        *(u32*)(disp + 0xC) = dat_804281b8;
        *(s8*)(disp + 0x10) = 0;
        *(s8*)(disp + 0x11) = -0x10;
        *(s8*)(disp + 0x12) = -0x10;
        *(s8*)(disp + 0x13) = 0;

        *(s32*)(work + 0x0) = 0;
        *(s32*)(bw + 0x1CE8) = 0;
        *(s32*)(bw + 0x1C9C) = 99;
        *(s32*)(bw + 0x1CB8) = 1;
        *(s32*)(work + 0x4) = sysMsec2Frame(7000);
        *(f32*)(work + 0x8) = 0.0f;
        *(f32*)(work + 0x10) =
            0.01f * (f32)*(s32*)(bw + 0x1CCC);
        *(f32*)(work + 0x14) =
            0.01f * (f32)*(s32*)(bw + 0x1CD0);
        *(s32*)(work + 0x18) = 0;
        *(s32*)(work + 0x1C) = 0;
        *(s32*)(work + 0xC) = 0x100;

        sysMsec2Frame(1000);
        i = 0;
        offset = 0;
        do {
            seconds = sysMsec2Frame(1000);
            seconds = (offset + 1) * seconds;
            random = irand(sysMsec2Frame(250));
            i++;
            *(s16*)(work + 0x20 + offset) = random + seconds;
            offset += 2;
        } while (i < 3);

        goto return_one;
    }

state_100:
    if (((*(u32*)(bw + 0x1C94) & 1) != 0) &&
        ((*(u32*)(unit + 0x27C) & 0x10) != 0)) {
        if (irand(100) >= 0) {
            *(s32*)(bw + 0x1C98) = 0x1E;
            *(s32*)(bw + 0x1CE8) = 0;
        } else {
            *(s32*)(bw + 0x1CE8) =
                irand(*(s32*)(bw + 0x1CD4) - 1) + 1;
            *(s32*)(bw + 0x1C98) = 0x1E;
        }
        *(s32*)(bw + 0x1C9C) = 1001;
        goto return_one;
    }

    BattleAcGaugeSeInit();
    *(s32*)(bw + 0x1C9C) = 1000;

state_1000:
    *(s32*)(work + 0x0) = *(s32*)(work + 0x0) + 1;
    if (*(s32*)(work + 0x0) > *(s32*)(work + 0x4)) {
        if (*(s32*)(bw + 0x1CE8) < *(s32*)(bw + 0x1CD8)) {
            *(s32*)(bw + 0x1CB8) = 0;
            *(u32*)(bw + 0x1CB8) =
                *(u32*)(bw + 0x1CB8) & ~2;
        } else {
            *(s32*)(bw + 0x1CB8) = 2;
            *(s32*)(bw + 0x1CB4) =
                *(s32*)(bw + 0x1CB4) + 1;
        }
        *(s32*)(bw + 0x1C9C) = 1002;
        goto return_one;
    } else {
        s32 i;
        s32 offset;
        s32 window;
        s32 pressed;
        s32 start;

        pressed = BattlePadCheckNow(*(u32*)(work + 0xC)) != 0;
        window = sysMsec2Frame(750);

        *(s32*)(work + 0x1C) = 1;
        offset = 0;
        for (i = 0; i < 3; i++) {
            start = *(s16*)(work + 0x20 + offset);
            if (start <= *(s32*)(work + 0x0) &&
                *(s32*)(work + 0x0) < start + window) {
                *(s32*)(work + 0x1C) = 0;
                break;
            }
            offset += 2;
        }

        if (autoCommand != 0) {
            pressed = *(s32*)(work + 0x1C) != 0;
        }

        if (*(s32*)(work + 0x18) == 0) {
            if (pressed != 0) {
                if (*(s32*)(work + 0x1C) != 0) {
                    *(f32*)(work + 0x8) +=
                        0.01f * *(f32*)(work + 0x10) *
                        (f32)_power_table[difficulty];
                } else {
                    *(f32*)(work + 0x8) -=
                        0.01f * *(f32*)(work + 0x10) *
                        (f32)_power_table2[difficulty];
                }
            }

            if (*(f32*)(work + 0x8) >= 100.0f) {
                *(f32*)(work + 0x8) = 100.0f;
                *(s32*)(work + 0x18) = 1;
            } else {
                *(f32*)(work + 0x8) -= *(f32*)(work + 0x14);
            }

            if (*(f32*)(work + 0x8) <= 0.0f) {
                *(f32*)(work + 0x8) = 0.0f;
            }
        }

        BattleAcGaugeSeUpdate(*(f32*)(work + 0x8));

        *(s32*)(bw + 0x1CE8) =
            (s32)(*(f32*)(work + 0x8) *
                  (f32)*(s32*)(bw + 0x1CD4) / 100.0f);

        if (*(s32*)(bw + 0x1CE8) >= *(s32*)(bw + 0x1CD4)) {
            *(s32*)(bw + 0x1CE8) =
                *(s32*)(bw + 0x1CD4) - 1;
        }

        if ((*(u32*)(bw + 0x1CC4) & 1) == 0) {
            if (*(f32*)(work + 0x8) >= 100.0f) {
                *(s32*)(bw + 0x1CE8) =
                    *(s32*)(bw + 0x1CE8) + 1;
            }
        }
    }
    goto return_one;

state_1001:
    *(s32*)(bw + 0x1C98) =
        *(s32*)(bw + 0x1C98) - 1;
    if (*(s32*)(bw + 0x1C98) >= 0) {
        goto return_one;
    }

    if (*(s32*)(bw + 0x1CE8) >= *(s32*)(bw + 0x1CD8)) {
        *(s32*)(bw + 0x1CB8) = 2;
        *(s32*)(bw + 0x1CB4) =
            *(s32*)(bw + 0x1CB4) + 1;
    } else {
        *(s32*)(bw + 0x1CB8) = 0;
    }
    return 0;

state_1002:
    *(s32*)(bw + 0x1C9C) = 1003;

state_1003:
    *(u32*)(bw + 0x1CC0) =
        *(u32*)(bw + 0x1CC0) | 1;
    BattleAcGaugeSeDelete();
    *(s32*)(work + 0x28) = 0x3C;
    *(s32*)(bw + 0x1C9C) = 1004;

state_1004:
    *(s32*)(work + 0x28) =
        *(s32*)(work + 0x28) - 1;
    if (*(s32*)(work + 0x28) > 0) {
        goto return_one;
    }

    *(s32*)(bw + 0x1C9C) = 1005;
    goto return_one;

state_1005:
    *(s32*)(bw + 0x1CA0) = 0;
    *(s32*)(bw + 0x1CA8) = 0;
    *(s32*)(bw + 0x1CA4) = 0;
    *(s32*)(bw + 0x1CAC) = 0;
    return 0;

return_one:
    return 1;
}

s32 battleAcResult_PowerGaugeLv2(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_PowerGaugeLv2(void* camera, void* wp) {
    typedef struct PowerGaugeLv2DispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } PowerGaugeLv2DispWork;
    extern f32 float_neg300_804281d4;
    extern f32 float_30_804281d8;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    PowerGaugeLv2DispWork* disp;
    s32 state;
    s32 timer;

    disp = (PowerGaugeLv2DispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);
    switch (state) {
        case 99:
            disp->x = intplGetValue(4, 0x14 - disp->timer, 0x14, float_neg300_804281d4, float_30_804281d8);
        case 100:
        case 1000:
        case 1002:
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1003:
        case 1004:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(4, timer - 0x28, 0x14, float_30_804281d8, float_neg300_804281d4);
            } else {
                disp->x = float_30_804281d8;
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


void battleAcDelete_PowerGaugeLv2(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* camGetPtr(s32);
    extern void iconDispGx(Vec*, s32, s32, f32);
    extern void BattleAcDrawGauge(s32, s32, s32, s32, s32, s32, s32, s32, f32);
    extern s32 BattleACGetButtonIcon(s32, s32);
    extern void* _battleWorkPointer;

    u8* work = (u8*)_battleWorkPointer + 0x1C90;
    const Vec* vec = (const Vec*)vec3_80300b60;
    Vec pos0;
    Vec pos1;
    Vec pos2;
    Vec pos3;
    Vec pos4;
    s32 gauge[5];
    f32 ratio;
    f32 iconX;
    f32 iconY;
    s32 count;
    s32 i;
    s32 icon0;
    s32 icon1;
    s32 state;
    s32 highlight;
    s32 finalIcon;

    camGetPtr(8);
    iconX = x - 200.0f;
    iconY = y + 25.0f;
    pos0 = vec[0];
    pos0.x = iconX;
    pos0.y = iconY;
    iconDispGx(&pos0, 0x10, 0x94, 1.0f);

    ratio = *(f32*)(work + 0x2C4) / 100.0f;
    highlight = 0;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    *(f32*)(work + 0x2B8) = ratio;
    if (*(s32*)(work + 0x2D4) != 0) {
        *(u8*)(work + 0x29D) += *(s8*)(work + 0x2A1);
        *(u8*)(work + 0x29E) += *(s8*)(work + 0x2A2);
        if (*(f32*)(work + 0x2B8) >= 1.0f) {
            highlight |= 1;
        }
        if (*(u8*)(work + 0x29D) <= 0x80 && *(s8*)(work + 0x2A1) < 0) {
            *(s8*)(work + 0x2A2) = 0x10;
            *(s8*)(work + 0x2A1) = 0x10;
        }
        if (*(u8*)(work + 0x29D) >= 0xF5 && *(s8*)(work + 0x2A1) > 0) {
            *(s8*)(work + 0x2A2) = -0x10;
            *(s8*)(work + 0x2A1) = -0x10;
        }
    }

    i = 0;
    do {
        gauge[i] = 100;
        i++;
    } while (i < 4);
    count = 0;
    for (i = 0; i < 4; i++) {
        gauge[i] = *(s32*)(work + 0x78 + i * 4);
        if (i != 0 && (gauge[i] - gauge[i - 1]) <= 0) {
            break;
        }
        count++;
    }
    BattleAcDrawGauge((s32)x, (s32)y, 0xB2, count,
                      gauge[0], gauge[1], gauge[2], highlight,
                      *(f32*)(work + 0x2B8));

    icon1 = BattleACGetButtonIcon(*(s32*)(work + 0x2C8), 1);
    icon0 = BattleACGetButtonIcon(*(s32*)(work + 0x2C8), 0);
    state = *(s32*)(work + 0xC);
    switch (state) {
        case 99:
        case 100:
        case 1002:
        case 1003:
        case 1004:
            pos1 = vec[1];
            pos1.x = iconX;
            pos1.y = y + 70.0f;
            iconDispGx(&pos1, 0x10, icon0, 1.0f);
            break;
        case 1000:
            if (*(s32*)(work + 0x2D8) != 0) {
                pos2 = vec[2];
                pos2.x = iconX;
                pos2.y = y + 70.0f;
                iconDispGx(&pos2, 0x10, icon1, 1.0f);
            } else {
                pos3 = vec[3];
                pos3.x = iconX;
                pos3.y = y + 70.0f;
                iconDispGx(&pos3, 0x10, icon0, 1.0f);
            }
            break;
    }

    finalIcon = 0x99;
    if (*(s32*)(work + 0x2D8) != 0) {
        finalIcon = 0x9D;
    }
    pos4 = vec[4];
    pos4.x = iconX + 120.0f;
    pos4.y = iconY;
    iconDispGx(&pos4, 0x10, finalIcon, 1.0f);
}

