#include "action/ac_power_gauge_lv2.h"

s32 battleAcResult_PowerGaugeLv2(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PowerGaugeLv2(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
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
    extern void actionCommandDisp(f32 x, f32 y);

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


/* CHATGPT STUB FILL: main/action/ac_power_gauge_lv2 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    extern void* camGetPtr(s32);
    extern void iconDispGx(f32, f32*, s32, s32);
    extern void BattleAcDrawGauge(f32, s32, s32, s32, s32, s32, s32, s32, s32);
    extern s32 BattleACGetButtonIcon(s32, s32);
    extern void* g_BattleWork;

    u8* work = (u8*)g_BattleWork + 0x1C90;
    f32 pos[3];
    s32 gauge[5];
    f32 ratio;
    f32 iconX = x - 200.0f;
    s32 count = 0;
    s32 i;
    s32 icon0;
    s32 icon1;
    s32 state;
    s32 highlight = 0;

    camGetPtr(8);
    pos[0] = iconX;
    pos[1] = y + 25.0f;
    pos[2] = 0.0f;
    iconDispGx(1.0f, pos, 0x10, 0x94);

    ratio = *(f32*)(work + 0x2C4) / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    *(f32*)(work + 0x2B8) = ratio;
    if (*(s32*)(work + 0x2D4) != 0) {
        *(u8*)(work + 0x29D) += *(s8*)(work + 0x2A1);
        *(u8*)(work + 0x29E) += *(s8*)(work + 0x2A2);
        highlight = ratio >= 1.0f;
        if (*(u8*)(work + 0x29D) < 0x81 && *(s8*)(work + 0x2A1) < 0) {
            *(s8*)(work + 0x2A1) = 0x10;
            *(s8*)(work + 0x2A2) = 0x10;
        }
        if (*(u8*)(work + 0x29D) > 0xF4 && *(s8*)(work + 0x2A1) > 0) {
            *(s8*)(work + 0x2A1) = -0x10;
            *(s8*)(work + 0x2A2) = -0x10;
        }
    }

    for (i = 0; i < 4; i++) {
        gauge[i] = 100;
    }
    for (i = 0; i < 4; i++) {
        gauge[i] = *(s32*)(work + 0x78 + i * 4);
        if (i != 0 && gauge[i] - gauge[i - 1] < 1) {
            break;
        }
        count++;
    }
    BattleAcDrawGauge(*(f32*)(work + 0x2B8), (s32)x, (s32)y, 0xB2,
                      count, gauge[0], gauge[1], gauge[2], highlight);

    icon1 = BattleACGetButtonIcon(*(s32*)(work + 0x2C8), 1);
    icon0 = BattleACGetButtonIcon(*(s32*)(work + 0x2C8), 0);
    state = *(s32*)(work + 0xC);
    if (state == 1000) {
        pos[0] = iconX;
        pos[1] = y + 70.0f;
        pos[2] = 0.0f;
        if (*(s32*)(work + 0x2D8) == 0) {
            iconDispGx(1.0f, pos, 0x10, icon0);
        } else {
            iconDispGx(1.0f, pos, 0x10, icon1);
        }
    } else if ((state >= 99 && state <= 100) ||
               (state >= 1002 && state <= 1004)) {
        pos[0] = iconX;
        pos[1] = y + 70.0f;
        pos[2] = 0.0f;
        iconDispGx(1.0f, pos, 0x10, icon0);
    }

    pos[0] = iconX + 120.0f;
    pos[1] = y + 25.0f;
    pos[2] = 0.0f;
    iconDispGx(1.0f, pos, 0x10,
               *(s32*)(work + 0x2D8) != 0 ? 0x9D : 0x99);
}

/* stub-fill: battleAcMain_PowerGaugeLv2 | missing_definition | ghidra_signature */
s32 battleAcMain_PowerGaugeLv2(s32 battle) {
    extern u32 BattleActionCommandGetDifficulty(void*);
    extern void* memset(void*, s32, u32);
    extern s32 sysMsec2Frame(s32);
    extern s32 irand(s32);
    extern void BattleAcGaugeSeInit(void);
    extern void BattleAcGaugeSeUpdate(f32);
    extern void BattleAcGaugeSeDelete(void);
    extern u32 BattlePadCheckNow(u32);
    extern s32 _power_table[];
    extern s32 _power_table2[];
    extern u32 dat_804281b8;

    s32* timer = (s32*)(battle + 0x1F4C);
    u32 difficulty = BattleActionCommandGetDifficulty((void*)battle);
    s8 autoInput = *(s8*)(*(s32*)(battle + 0x1C90) + 0x307);
    s32 state = *(s32*)(battle + 0x1C9C);

    if (state == 0) {
        s32 i;
        memset((void*)(battle + 0x1F20), 0, 0x2C);
        *(f32*)(battle + 0x1F34) = -300.0f;
        *(f32*)(battle + 0x1F38) = 0.0f;
        *(s32*)(battle + 0x1F40) = 0x14;
        *(f32*)(battle + 0x1F48) = 0.0f;
        *(u32*)(battle + 0x1F2C) = dat_804281b8;
        *(u8*)(battle + 0x1F30) = 0;
        *(u8*)(battle + 0x1F31) = 0xF0;
        *(u8*)(battle + 0x1F32) = 0xF0;
        *(u8*)(battle + 0x1F33) = 0;
        *timer = 0;
        *(s32*)(battle + 0x1CE8) = 0;
        *(s32*)(battle + 0x1C9C) = 99;
        *(s32*)(battle + 0x1CB8) = 1;
        *(s32*)(battle + 0x1F50) = sysMsec2Frame(7000);
        *(f32*)(battle + 0x1F54) = 0.0f;
        *(f32*)(battle + 0x1F5C) = 0.01f * (f32)*(s32*)(battle + 0x1CCC);
        *(f32*)(battle + 0x1F60) = 0.01f * (f32)*(u32*)(battle + 0x1CD0);
        *(s32*)(battle + 0x1F64) = 0;
        *(s32*)(battle + 0x1F68) = 0;
        *(s32*)(battle + 0x1F58) = 0x100;
        sysMsec2Frame(1000);
        for (i = 0; i < 3; i++) {
            *(s16*)((s32)timer + 0x20 + i * 2) =
                (s16)irand(sysMsec2Frame(250)) + (s16)(i + 1) * (s16)sysMsec2Frame(1000);
        }
        return 1;
    }
    if (state == 99) {
        return 1;
    }
    if (state == 100) {
        if ((*(u32*)(battle + 0x1C94) & 1) &&
            (*(u32*)(*(s32*)(battle + 0x1C90) + 0x27C) & 0x10)) {
            if (irand(100) < 0) {
                *(s32*)(battle + 0x1CE8) = irand(*(s32*)(battle + 0x1CD4) - 1) + 1;
            } else {
                *(s32*)(battle + 0x1CE8) = 0;
            }
            *(s32*)(battle + 0x1C98) = 0x1E;
            *(s32*)(battle + 0x1C9C) = 1001;
            return 1;
        }
        BattleAcGaugeSeInit();
        *(s32*)(battle + 0x1C9C) = 1000;
        return 1;
    }
    if (state == 1001) {
        *(s32*)(battle + 0x1C98) -= 1;
        if (*(s32*)(battle + 0x1C98) > -1) {
            return 1;
        }
        if (*(s32*)(battle + 0x1CE8) < *(s32*)(battle + 0x1CD8)) {
            *(s32*)(battle + 0x1CB8) = 0;
        } else {
            *(s32*)(battle + 0x1CB8) = 2;
            *(s32*)(battle + 0x1CB4) += 1;
        }
        return 0;
    }
    if (state == 1002) {
        *(s32*)(battle + 0x1C9C) = 1003;
        return 1;
    }
    if (state == 1003) {
        *(u32*)(battle + 0x1CC0) |= 1;
        BattleAcGaugeSeDelete();
        *(s32*)(battle + 0x1F74) = 0x3C;
        *(s32*)(battle + 0x1C9C) = 1004;
    }
    if (*(s32*)(battle + 0x1C9C) == 1004) {
        *(s32*)(battle + 0x1F74) -= 1;
        if (*(s32*)(battle + 0x1F74) > 0) {
            return 1;
        }
        *(s32*)(battle + 0x1C9C) = 1005;
        return 1;
    }
    if (*(s32*)(battle + 0x1C9C) == 1005) {
        *(s32*)(battle + 0x1CA0) = 0;
        *(s32*)(battle + 0x1CA8) = 0;
        *(s32*)(battle + 0x1CA4) = 0;
        *(s32*)(battle + 0x1CAC) = 0;
        return 0;
    }
    if (state != 1000) {
        return 1;
    }

    *timer += 1;
    if (*timer > *(s32*)(battle + 0x1F50)) {
        if (*(s32*)(battle + 0x1CE8) < *(s32*)(battle + 0x1CD8)) {
            *(s32*)(battle + 0x1CB8) = 0;
            *(u32*)(battle + 0x1CB8) &= ~2;
        } else {
            *(s32*)(battle + 0x1CB8) = 2;
            *(s32*)(battle + 0x1CB4) += 1;
        }
        *(s32*)(battle + 0x1C9C) = 1002;
    } else {
        s32 i;
        s32 window = sysMsec2Frame(750);
        s32 valid = 1;
        s32 pressed = BattlePadCheckNow(*(u32*)(battle + 0x1F58)) != 0;
        *(s32*)(battle + 0x1F68) = 1;
        for (i = 0; i < 3; i++) {
            s32 start = *(s16*)((s32)timer + 0x20 + i * 2);
            if (start <= *timer && *timer < start + window) {
                *(s32*)(battle + 0x1F68) = 0;
                break;
            }
        }
        if (autoInput != 0) {
            pressed = *(s32*)(battle + 0x1F68) != 0;
        }
        if (*(s32*)(battle + 0x1F64) == 0) {
            if (pressed) {
                if (*(s32*)(battle + 0x1F68) == 0) {
                    *(f32*)(battle + 0x1F54) -=
                        0.01f * *(f32*)(battle + 0x1F5C) * (f32)_power_table2[difficulty];
                } else {
                    *(f32*)(battle + 0x1F54) +=
                        0.01f * *(f32*)(battle + 0x1F5C) * (f32)_power_table[difficulty];
                }
            }
            if (*(f32*)(battle + 0x1F54) < 100.0f) {
                *(f32*)(battle + 0x1F54) -= *(f32*)(battle + 0x1F60);
            } else {
                *(f32*)(battle + 0x1F54) = 100.0f;
                *(s32*)(battle + 0x1F64) = 1;
            }
            if (*(f32*)(battle + 0x1F54) <= 0.0f) {
                *(f32*)(battle + 0x1F54) = 0.0f;
            }
        }
        BattleAcGaugeSeUpdate(*(f32*)(battle + 0x1F54));
        *(s32*)(battle + 0x1CE8) =
            (s32)(*(f32*)(battle + 0x1F54) * (f32)*(u32*)(battle + 0x1CD4) / 100.0f);
        if (*(s32*)(battle + 0x1CE8) >= *(s32*)(battle + 0x1CD4)) {
            *(s32*)(battle + 0x1CE8) = *(s32*)(battle + 0x1CD4) - 1;
        }
        if ((*(u32*)(battle + 0x1CC4) & 1) == 0 && *(f32*)(battle + 0x1F54) >= 100.0f) {
            *(s32*)(battle + 0x1CE8) += 1;
        }
        (void)valid;
    }
    return 1;
}

