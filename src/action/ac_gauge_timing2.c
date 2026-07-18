#include "action/ac_gauge_timing2.h"

s32 battleAcResult_GaugeTiming2(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_GaugeTiming2(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_GaugeTiming2(void* camera, void* wp) {
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

    extern f32 float_neg300_804280d0;
    extern f32 float_30_804280d4;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (GaugeTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);
    switch (state) {
        case 99:
            disp->x = intplGetValue(4, 0x14 - disp->timer, 0x14, float_neg300_804280d0, float_30_804280d4);
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
                disp->x = intplGetValue(4, timer - 0x28, 0x14, float_30_804280d4, float_neg300_804280d0);
            } else {
                disp->x = float_30_804280d4;
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


/* CHATGPT STUB FILL: main/action/ac_gauge_timing2 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* _battleWorkPointer;
    extern void* camGetPtr(s32);
    extern void btlDispGXInit2DRasta(void);
    extern void btlDispTexPlane(f64, f64, f64, f64, f64, s32, u32*, s32);
    extern void btlDispGXQuads2DRasta(f64, f64, f64, f64, s32, s32, s32, s32);
    extern s32 BattleACGetButtonIcon(s32, s32);
    extern void iconDispGx(f64, Vec*, u16, u16);
    u8* bw = (u8*)_battleWorkPointer;
    u8* extra = bw + 0x1F4C;
    s32* params = (s32*)(bw + 0x1CC8);
    f32 value = *(f32*)(extra + 4);
    f32 left = x - 288.0f;
    f32 barY = y + 49.0f;
    u32 color = 0xFFFFFFFF;
    u16 width = *(u16*)(extra + 0x68);
    s32 segments = 0;
    s32 remainder = 0;
    s32 pressed = BattleACGetButtonIcon(0x100, 1);
    s32 normal = BattleACGetButtonIcon(0x100, 0);
    Vec pos;
    s32 i;

    camGetPtr(1);
    if ((s32)width - 0x10 > 0) {
        segments = ((s32)width - 0x0D) / 4;
        remainder = (s32)width - 0x10 - segments * 4;
    }
    btlDispGXInit2DRasta();
    btlDispTexPlane(left, y + 41.0f, 0.0, 1.0, 1.0, 0x59, &color, 0);
    for (i = 0; i < segments; i++) {
        f32 partX = left + 10.0f + (f32)(i * 4);
        if (i == segments - 1) partX += (f32)remainder;
        btlDispTexPlane(partX, y + 41.0f, 0.0, 1.0, 1.0, 0x5A, &color, 0);
    }
    btlDispTexPlane(left + 16.0f + (f32)(width - 0x10), y + 41.0f,
                    0.0, -1.0, 1.0, 0x59, &color, 0);
    if (value > 100.0f) value = 100.0f;
    *(f32*)(bw + 0x1F48) = value / 100.0f;
    if (value >= 0.0f) {
        btlDispGXInit2DRasta();
        btlDispGXQuads2DRasta(left, barY, left + (f32)width * value / 100.0f,
                              barY, 0xFF, 0, 0, 0xFF);
    }
    for (i = 0; i < params[2]; i++) {
        f32 threshold = *(f32*)(extra + 0x4C + i * 4);
        f32 distance = value - threshold;
        if (distance < 0.0f) distance = -distance;
        pos.x = left + (f32)width * threshold / 100.0f;
        pos.y = y + 23.0f;
        pos.z = 0.0f;
        iconDispGx(0.75, &pos, 0x10, value >= threshold || distance <= 6.0f ? 0x9D : 0x99);
    }
    if (*(s32*)(bw + 0x1C9C) == 1000) {
        pos.x = x - 225.0f + 28.0f;
        pos.y = y + 55.0f;
        pos.z = 0.0f;
        iconDispGx(1.0, &pos, 0x10, *(s32*)(extra + 0x10) == 0 ? (u16)normal : (u16)pressed);
    } else if ((*(s32*)(bw + 0x1C9C) >= 99 && *(s32*)(bw + 0x1C9C) <= 100) ||
               (*(s32*)(bw + 0x1C9C) >= 1002 && *(s32*)(bw + 0x1C9C) <= 1004)) {
        pos.x = x - 225.0f + 28.0f;
        pos.y = y + 55.0f;
        pos.z = 0.0f;
        iconDispGx(1.0, &pos, 0x10, (u16)normal);
    }
    for (i = 0; i < params[2]; i++) {
        s8 result = *(s8*)(extra + 0x6C + i);
        s32 texture = result == 1 ? 0x54 : (result == -1 ? 0x55 : 0x5B);
        if (texture != 0x5B) {
            f32 threshold = *(f32*)(extra + 0x4C + i * 4);
            btlDispTexPlane(left + (f32)width * threshold / 100.0f, y,
                            0.0, 1.0, 1.0, texture, &color, 0);
        }
    }
}

/* stub-fill: battleAcMain_GaugeTiming2 | missing_definition | ghidra_signature */
s32 battleAcMain_GaugeTiming2(void* battleWork) {
    extern void* memset(void*, s32, u32);
    extern s32 irand(s32);
    extern u32 BattlePadCheckTrigger(u32);
    extern s32 psndSFXOn(char*);
    extern char str_SFX_AC_COMMAND_NG1_80300998[];
    extern char str_SFX_AC_PONE1_803009ac[];
    extern char str_SFX_AC_COMMAND_OK1_803009bc[];
    u8* bw = (u8*)battleWork;
    u8* unit = *(u8**)(bw + 0x1C90);
    u8* extra = bw + 0x1F4C;
    u8* acWork = bw + 0x1F20;
    s32 state = *(s32*)(bw + 0x1C9C);
    s32 count = *(s32*)(bw + 0x1CD0);
    s32 autoInput = 0;
    s32 i;

    if (*(s8*)(unit + 0x307) != 0) autoInput = 1;
    if (state == 1000) goto state_1000;
    if (state > 999) {
        if (state == 1004) goto state_1004;
        if (state > 1003) {
            if (state > 1005) return 1;
            goto state_1005;
        }
        if (state == 1002) goto state_1002;
        if (state < 1002) goto state_1001;
        goto state_1003;
    }
    if (state == 99) return 1;
    if (state < 99) {
        if (state != 0) return 1;
        goto state_0;
    }
    if (state > 100) return 1;
    goto state_100;

state_0:
    {
        memset(acWork, 0, 0x2C);
        *(f32*)(acWork + 0x14) = -300.0f;
        *(f32*)(acWork + 0x18) = 0.0f;
        *(s32*)(acWork + 0x20) = 0x14;
        *(f32*)(acWork + 0x28) = 0.0f;
        *(s32*)(acWork + 0x0C) = -1;
        *(s32*)(extra + 0x00) = 0;
        *(s32*)(extra + 0x08) = 0;
        *(s32*)(bw + 0x1C9C) = 99;
        *(s32*)(bw + 0x1CB8) = 1;
        *(s32*)(extra + 0x0C) = *(s32*)(bw + 0x1CCC);
        *(s32*)(extra + 0x10) = 0;
        *(f32*)(extra + 0x04) = 0.0f;
        *(s32*)(bw + 0x1CE8) = 0;
        *(u16*)(extra + 0x68) = (u16)(count * 60);
        for (i = 0; i < count; i++) {
            *(f32*)(extra + 0x18 + i * 4) = (f32)((100 / count) * (i + 1)) - 4.0f;
            *(s8*)(extra + 0x38 + i) = 0;
            *(s8*)(extra + 0x40 + i) = 0;
            *(s32*)(extra + 0x48 + i * 4) = -1;
        }
        *(u32*)(extra + 0x14) = 0x100;
        *(s32*)(bw + 0x1CEC) = 0;
        return 1;
    }
state_100:
    {
        if ((*(u32*)(bw + 0x1C94) & 1) && (*(u32*)(unit + 0x27C) & 0x10)) {
            *(u8*)(extra + 0x6A) = irand(100) < 0;
            *(s32*)(bw + 0x1C98) = 0x1E;
            *(s32*)(bw + 0x1C9C) = 1001;
            return 1;
        }
        *(s32*)(bw + 0x1C9C) = 1000;
    }
    goto state_1000;
state_1001:
    {
        if (--*(s32*)(bw + 0x1C98) > -1) return 1;
        if (*(u8*)(extra + 0x6A) == 0) {
            *(s32*)(bw + 0x1CB8) = 0;
            *(s32*)(bw + 0x1CE8) = 0;
        } else {
            *(s32*)(bw + 0x1CB8) = 2;
            (*(s32*)(bw + 0x1CB4))++;
            *(s32*)(bw + 0x1CE8) = irand(count) + 1;
        }
        *(u32*)(bw + 0x1CC0) |= 1;
        return 0;
    }
state_1002:
    *(s32*)(bw + 0x1C9C) = 1003;
state_1003:
    {
        *(s32*)(bw + 0x1CEC) = 1;
        *(u32*)(bw + 0x1CC0) |= 1;
        *(s32*)(extra + 0x6C) = 0x3C;
        *(s32*)(bw + 0x1C9C) = 1004;
    }
state_1004:
    {
        if (--*(s32*)(extra + 0x6C) > 0) return 1;
        *(s32*)(bw + 0x1C9C) = 1005;
        return 1;
    }
state_1005:
    {
        *(s32*)(bw + 0x1CA0) = 0;
        *(s32*)(bw + 0x1CA8) = 0;
        *(s32*)(bw + 0x1CA4) = 0;
        *(s32*)(bw + 0x1CAC) = 0;
        return 0;
    }
state_1000:
    (*(s32*)extra)++;
    {
        u32 trigger = BattlePadCheckTrigger(*(u32*)(extra + 0x14));
        s32 allFinished = 1;
        for (i = 0; i < count; i++) {
            if (*(s8*)(extra + 0x38 + i) == 0) { allFinished = 0; break; }
        }
        if (*(s32*)(extra + 0x0C) < *(s32*)extra && allFinished) {
            if (*(s32*)(bw + 0x1CE8) < 1) *(s32*)(bw + 0x1CB8) = 0;
            else { *(s32*)(bw + 0x1CB8) = 2; (*(s32*)(bw + 0x1CB4))++; }
            *(s32*)(bw + 0x1C9C) = 1002;
            return 1;
        }
        (*(s32*)(extra + 8))++;
        *(f32*)(extra + 4) = 100.0f * (f32)*(s32*)(extra + 8) / (f32)*(s32*)(extra + 0x0C);
        for (i = 0; i < count; i++) {
            f32 threshold = *(f32*)(extra + 0x18 + i * 4);
            f32 distance = *(f32*)(extra + 4) - threshold;
            if (distance < 0.0f) distance = -distance;
            if (*(s8*)(extra + 0x38 + i) == 0 && threshold + 6.0f < *(f32*)(extra + 4)) {
                *(s8*)(extra + 0x38 + i) = -1;
                psndSFXOn(str_SFX_AC_COMMAND_NG1_80300998);
            }
            *(s32*)(extra + 0x10) = 0;
            if (*(s8*)(extra + 0x38 + i) == 0 && distance <= 6.0f) {
                *(s32*)(extra + 0x10) = 1;
                if (*(s32*)(extra + 0x48 + i * 4) == -1)
                    *(s32*)(extra + 0x48 + i * 4) = psndSFXOn(str_SFX_AC_PONE1_803009ac);
            }
        }
        if (autoInput && *(s32*)(extra + 0x10) != 0) trigger = 1;
        if (trigger != 0) {
            for (i = 0; i < count; i++) {
                f32 distance = *(f32*)(extra + 4) - *(f32*)(extra + 0x18 + i * 4);
                if (distance < 0.0f) distance = -distance;
                if (*(s8*)(extra + 0x38 + i) == 0) {
                    if (distance <= 6.0f) {
                        *(s8*)(extra + 0x38 + i) = 1;
                        (*(s32*)(bw + 0x1CE8))++;
                        psndSFXOn(str_SFX_AC_COMMAND_OK1_803009bc);
                    } else {
                        *(s8*)(extra + 0x40 + i) = 1;
                        if (i == count - 1) {
                            *(s8*)(extra + 0x38 + i) = -1;
                            psndSFXOn(str_SFX_AC_COMMAND_NG1_80300998);
                        }
                    }
                    break;
                }
            }
        }
    }
    return 1;
}

