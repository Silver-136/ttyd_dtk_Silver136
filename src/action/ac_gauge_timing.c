#include "action/ac_gauge_timing.h"

/* stub-fill: battleAcMain_GaugeTiming | missing_definition | ghidra_signature */
s32 battleAcMain_GaugeTiming(void* battleWork) {
    extern void* memset(void*, s32, u32);
    extern s32 irand(s32);
    extern u32 BattlePadCheckNow(u32);
    extern u32 BattlePadGetNow(void);
    extern u32 BattlePadCheckTrigger(u32);
    extern void BattlePadCheckUp(u32);
    extern f64 reviseAngle(f64);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern void psndSFXOff(s32);
    u8* bw = (u8*)battleWork;
    u8* unit = *(u8**)(bw + 0x1C90);
    u8* extra = bw + 0x1F4C;
    s32* params = (s32*)(bw + 0x1CC8);
    s32 state = *(s32*)(bw + 0x1C9C);
    s8 autoCommand = *(s8*)(unit + 0x307);

    if (state == 0) {
        memset(bw + 0x1F20, 0, 0x2C);
        *(f32*)(bw + 0x1F34) = -300.0f;
        *(f32*)(bw + 0x1F38) = 0.0f;
        *(s32*)(bw + 0x1F40) = 0x14;
        *(f32*)(bw + 0x1F48) = 0.0f;
        *(s32*)(extra + 0x00) = 0;
        *(s32*)(extra + 0x08) = 0;
        *(s32*)(bw + 0x1CE8) = 0;
        *(s32*)(bw + 0x1C9C) = 99;
        *(s32*)(bw + 0x1CB8) = 1;
        *(s32*)(extra + 0x0C) = params[1];
        *(s32*)(extra + 0x10) = 0;
        *(s32*)(extra + 0x1C) = 0;
        *(s32*)(extra + 0x20) = 0;
        *(s32*)(extra + 0x24) = -1;
        if (params[0] == 0) {
            *(u8*)(extra + 0x16) = 1;
            *(u32*)(extra + 0x18) = 0x20;
            *(u8*)(extra + 0x15) = 1;
            *(s32*)(bw + 0x1CE8) = 1;
        } else if (params[0] == 2) {
            *(u8*)(extra + 0x16) = 0;
            *(u32*)(extra + 0x18) = 0x40000;
            *(u8*)(extra + 0x15) = 0;
        } else {
            *(u8*)(extra + 0x16) = 1;
            *(u32*)(extra + 0x18) = 0x100;
            *(u8*)(extra + 0x15) = 1;
            *(s32*)(bw + 0x1CE8) = 1;
        }
        if (params[2] == 1) {
            *(f32*)(extra + 4) = 0.0f;
            params[4] &= ~1;
        } else {
            *(f32*)(extra + 4) = 50.0f;
        }
        return 1;
    }
    if (state == 99) return 1;
    if (state == 100) {
        if ((*(u32*)(bw + 0x1C94) & 1) && (*(u32*)(unit + 0x27C) & 0x10)) {
            *(u8*)(extra + 0x14) = irand(100) < 0;
            *(s32*)(bw + 0x1C98) = 0x1E;
            *(s32*)(bw + 0x1C9C) = 1002;
            return 1;
        }
        *(s32*)(bw + 0x1C9C) = *(u8*)(extra + 0x16) == 0 ? 1000 : 1001;
    }
    state = *(s32*)(bw + 0x1C9C);
    if (state == 1002) {
        if (--*(s32*)(bw + 0x1C98) > -1) return 1;
        if (*(u8*)(extra + 0x14) == 0) *(s32*)(bw + 0x1CB8) = 0;
        else { *(s32*)(bw + 0x1CB8) = 2; (*(s32*)(bw + 0x1CB4))++; }
        *(u32*)(bw + 0x1CC0) |= 1;
        return 0;
    }
    if (state == 1003 || state == 1004) {
        if (state == 1003) {
            *(u32*)(bw + 0x1CC0) |= 1;
            *(s32*)(extra + 0x28) = 0x3C;
            *(s32*)(bw + 0x1C9C) = 1004;
            if (*(s32*)(extra + 0x24) != -1) psndSFXOff(*(s32*)(extra + 0x24));
        }
        if (--*(s32*)(extra + 0x28) > 0) return 1;
        *(s32*)(bw + 0x1C9C) = 1005;
        return 1;
    }
    if (state == 1005) {
        *(s32*)(bw + 0x1CA0) = 0;
        *(s32*)(bw + 0x1CA8) = 0;
        *(s32*)(bw + 0x1CA4) = 0;
        *(s32*)(bw + 0x1CAC) = 0;
        return 0;
    }
    (*(s32*)extra)++;
    if (*(u8*)(extra + 0x15) == 1) {
        if (*(u8*)(extra + 0x16) == 0) {
            if (BattlePadCheckNow(*(u32*)(extra + 0x18)) == 0) {
                BattlePadCheckUp(*(u32*)(extra + 0x18));
                if (*(u32*)(extra + 0x18) != 0) *(u8*)(extra + 0x15) = 2;
            }
        } else if (BattlePadGetNow() == *(u32*)(extra + 0x18) &&
                   BattlePadCheckTrigger(*(u32*)(extra + 0x18)) != 0) {
            *(u8*)(extra + 0x15) = 2;
        }
    } else if (*(u8*)(extra + 0x15) == 0 && *(u8*)(extra + 0x16) == 0 &&
               BattlePadCheckNow(*(u32*)(extra + 0x18)) != 0) {
        *(u8*)(extra + 0x15) = 1;
        *(s32*)(bw + 0x1C9C) = 1001;
        *(s32*)extra = 0;
        *(s32*)(bw + 0x1CE8) = 1;
    }
    if (*(s32*)(bw + 0x1C9C) == 1000) {
        if (*(s32*)extra > 120) {
            *(s32*)(bw + 0x1CB8) = 0;
            *(f32*)(extra + 4) = 0.0f;
            *(s32*)(bw + 0x1C9C) = 1003;
            *(s32*)(bw + 0x1CE8) = 2;
        }
        return 1;
    }
    if (*(s32*)(extra + 0x0C) < *(s32*)extra) {
        *(s32*)(bw + 0x1CB8) = 0;
        *(s32*)(bw + 0x1C9C) = 1003;
        return 1;
    }
    if (*(s32*)(extra + 0x1C) < 1) (*(s32*)(extra + 8))++;
    if (params[2] == 1) {
        f32 angle = (f32)reviseAngle((f64)((*(s32*)(extra + 8) * 360) / params[4]));
        if (angle <= 90.0f) *(f32*)(extra + 4) = (f32)intplGetValue(0.0, 100.0, 5, (s32)angle, 90);
        else if (angle <= 180.0f) *(f32*)(extra + 4) = (f32)intplGetValue(100.0, 0.0, 2, (s32)(angle - 90.0f), 90);
        else if (angle <= 270.0f) *(f32*)(extra + 4) = (f32)intplGetValue(0.0, 100.0, 5, (s32)(angle - 180.0f), 90);
        else *(f32*)(extra + 4) = (f32)intplGetValue(100.0, 0.0, 2, (s32)(angle - 270.0f), 90);
        *(s32*)(extra + 0x10) = *(f32*)(extra + 4) >= (f32)params[3];
    } else {
        *(f32*)(extra + 4) = ((f32)params[4] / 100.0f) * (f32)*(s32*)(extra + 8) + 50.0f;
        if (*(u32*)(bw + 0x1C94) & 1) *(f32*)(extra + 4) = (f32)((s32)*(f32*)(extra + 4) % 100);
        {
            f32 distance = *(f32*)(extra + 4) - 50.0f;
            if (distance < 0.0f) distance = -distance;
            *(s32*)(extra + 0x10) = distance <= 12.0f;
        }
    }
    if (autoCommand != 0 && *(s32*)(extra + 0x10) != 0) *(u8*)(extra + 0x15) = 2;
    if (*(u8*)(extra + 0x15) == 2) {
        if (*(s32*)(extra + 0x10) == 0) *(s32*)(bw + 0x1CB8) = 0;
        else { *(s32*)(bw + 0x1CB8) = 2; (*(s32*)(bw + 0x1CB4))++; }
        *(s32*)(bw + 0x1C9C) = 1003;
    }
    return 1;
}

s32 battleAcResult_GaugeTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_GaugeTiming(void* camera, void* wp) {
    typedef struct GaugeTimingDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } GaugeTimingDispWork;
    GaugeTimingDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80427e20;
    extern f32 float_30_80427e24;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (GaugeTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
            disp->x = intplGetValue(4, 0x14 - disp->timer, 0x14, float_neg300_80427e20, float_30_80427e24);
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
                disp->x = intplGetValue(4, timer - 0x28, 0x14, float_30_80427e24, float_neg300_80427e20);
            } else {
                disp->x = float_30_80427e24;
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


void battleAcDelete_GaugeTiming(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

/* CHATGPT STUB FILL: main/action/ac_gauge_timing 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x, y, z;
        u8 targetFramePad[0xD0];
    } Vec;
    extern void* _battleWorkPointer;
    extern void* camGetPtr(s32);
    extern void BattleAcDrawGauge(f32, s32, s32, s32, s32, s32, s32, s32, s32);
    extern s32 BattleACGetButtonIcon(s32, s32);
    extern void iconDispGx(f64, Vec*, u16, u16);
    extern void btlDispGXInit2DRasta(void);
    extern void btlDispGXQuads2DRasta(f64, f64, f64, f64, s32, s32, s32, s32);
    u8* wp = (u8*)_battleWorkPointer;
    s32* params = (s32*)(wp + 0x1CC8);
    u8* extra = wp + 0x1F4C;
    f32 value = *(f32*)(extra + 4);
    f32 ratio = value / 100.0f;
    s32 pressedIcon;
    s32 normalIcon;
    s32 button;
    Vec pos;
    s32 i;

    camGetPtr(8);
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    *(f32*)(wp + 0x1F28) = ratio;
    if (params[2] == 1) {
        f32 gauge = 100.0f * ratio / (f32)params[3];
        if (gauge > 100.0f) {
            gauge = 100.0f;
        }
        if (*(s32*)(extra + 0x1C) < 1) {
            BattleAcDrawGauge(gauge, (s32)x, (s32)y, 0xB2, 100, 100, 100, 100, 0);
        } else {
            BattleAcDrawGauge(-1.0f, (s32)x, (s32)y, 0xB2, 100, 100, 100, 100, 0);
        }
        pos.x = x - 288.0f + 176.0f + 16.0f;
        pos.y = y + 18.0f;
        pos.z = 0.0f;
        iconDispGx(1.0, &pos, 0x10, value < (f32)params[3] ? 0x99 : 0x9D);
    } else {
        BattleAcDrawGauge(0.0f, (s32)x, (s32)y, 0xB2, 100, 100, 100, 100, 0);
        if (*(s32*)(extra + 0x1C) < 1) {
            f32 left = x - 288.0f - 88.0f;
            f32 right = left + 173.0f * value / 100.0f;
            btlDispGXInit2DRasta();
            btlDispGXQuads2DRasta(left, y + 49.0f, right, y + 49.0f, 0, 0xFF, 0, 0xFF);
        }
        for (i = 0; i < params[3]; i++) {
            pos.x = x - 288.0f + 176.0f * ((f32)(i + 1) / (f32)(params[3] + 1));
            pos.y = y + 20.0f;
            pos.z = 0.0f;
            iconDispGx(0.8, &pos, 0x10, *(s32*)(extra + 0x10) != 0 ? 0x9D : 0x99);
        }
    }

    button = params[0] == 0 ? 0x20 : (params[0] == 2 ? 0x40000 : 0x100);
    pressedIcon = BattleACGetButtonIcon(button, 1);
    normalIcon = BattleACGetButtonIcon(button, 0);
    if (*(s32*)(wp + 0x1C9C) == 1002) {
        return;
    }
    if (*(s32*)(wp + 0x1C9C) < 1002) {
        if (*(s32*)(wp + 0x1C9C) > 100) {
            if (*(s32*)(wp + 0x1C9C) < 1000) {
                return;
            }
        } else if (*(s32*)(wp + 0x1C9C) < 99) {
            return;
        }
    } else if (*(s32*)(wp + 0x1C9C) > 1005) {
        return;
    }
    {
        pos.x = x - 228.0f + 28.0f;
        pos.y = y + 55.0f;
        pos.z = 0.0f;
        if (*(s32*)(wp + 0x1C9C) == 1000 || *(s32*)(wp + 0x1C9C) == 1001) {
            if (*(u8*)(extra + 0x16) != 0) {
                normalIcon = *(s32*)(extra + 0x10) != 0 ? pressedIcon : normalIcon;
            } else {
                normalIcon = *(s32*)(extra + 0x10) != 0 ? normalIcon : pressedIcon;
            }
        }
        iconDispGx(1.0, &pos, 0x10, (u16)normalIcon);
    }
}

