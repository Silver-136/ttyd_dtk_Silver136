#include "action/ac_monosiri_target.h"

extern void* _battleWorkPointer;
extern s32 _frame_table[4];
extern f32 float_0_80428604;

s32 BattleActionCommandGetDifficulty(void* battleWork);
void actionCommandDisp(f32 x, f32 y);

s32 _getmaxcount(void) {
    return _frame_table[BattleActionCommandGetDifficulty(_battleWorkPointer)];
}

/* stub-fill: battleAcMain_MonosiriTarget | missing_definition | ghidra_signature */
s32 battleAcMain_MonosiriTarget(s32 battleWork) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void* memset(void* dest, s32 value, u32 size);
    extern s32 _getmaxcount(void);
    extern void BtlUnit_GetHitPos(void* unit, void* part, f32* x, f32* y, f32* z);
    extern s32 psndSFXOn(const char* name);
    extern void psndSFXOff(s32 sound);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 current, s32 total);
    extern s32 BattlePadCheckTrigger(u32 buttons);
    extern f32 float_0_80428604;
    extern f32 float_neg300_80428624;
    extern f32 float_80_80428628;
    extern f32 float_160_8042862c;
    extern const char str_SFX_AC_CURSOR_MOVE1_803012b0[];

    void* unit;
    void* part;
    s32* work;
    s32 autoSuccess;
    f32 value;
    f32 delta;

    work = (s32*)(battleWork + 0x1F4C);
    autoSuccess = (*(u8*)(*(s32*)(battleWork + 0x1C90) + 0x307) != 0);
    unit = BattleGetUnitPtr((void*)battleWork, *(s32*)(battleWork + 0x1CC8));
    part = BtlUnit_GetPartsPtr(unit, *(s32*)(battleWork + 0x1CCC));

    switch (*(s32*)(battleWork + 0x1C9C)) {
        case 0:
            memset((void*)(battleWork + 0x1F20), 0, 0x2C);
            *(f32*)(battleWork + 0x1F34) = float_neg300_80428624;
            *(f32*)(battleWork + 0x1F38) = float_0_80428604;
            *(s32*)(battleWork + 0x1F40) = 0x14;
            work[0] = 0;
            work[1] = _getmaxcount();
            BtlUnit_GetHitPos(unit, part, (f32*)&work[2], (f32*)&work[3], (f32*)&work[4]);
            work[5] = work[2];
            work[6] = work[3];
            work[7] = work[4];
            work[8] = work[2];
            work[9] = work[3];
            work[10] = work[4];
            *(f32*)&work[11] = float_0_80428604;
            work[12] = 0;
            *(s32*)(battleWork + 0x1C9C) = 99;
            *(s32*)(battleWork + 0x1CB8) = 1;
            work[14] = -1;
            break;

        case 100:
            work[0] = 0;
            work[14] = psndSFXOn(str_SFX_AC_CURSOR_MOVE1_803012b0);
            *(s32*)(battleWork + 0x1C9C) = 1000;
            /* fallthrough */

        case 1000:
            work[0]++;
            value = intplGetValue(float_0_80428604, float_160_8042862c, 0, work[0], work[1]);
            *(f32*)&work[5] = (*(f32*)&work[2] - float_80_80428628) + value;
            delta = *(f32*)&work[5] - *(f32*)&work[8];
            if (delta < 0.0f) {
                delta = -delta;
            }
            if (delta < 28.0) {
                work[12] = 1;
            } else {
                work[12] = 0;
            }
            if (work[12] == 0) {
                work[13] = 0;
            } else {
                work[13]++;
            }
            if (work[0] < work[1]) {
                if (BattlePadCheckTrigger(0x100) != 0 || autoSuccess != 0) {
                    if (work[12] == 0) {
                        if (autoSuccess != 0) {
                            return 1;
                        }
                        *(s32*)(battleWork + 0x1CB8) = 0;
                    } else {
                        *(s32*)(battleWork + 0x1CB8) = 2;
                        *(s32*)(battleWork + 0x1CB4) += 1;
                    }
                    *(s32*)(battleWork + 0x1C9C) = 1002;
                }
            } else {
                *(s32*)(battleWork + 0x1CB8) = 0;
                *(s32*)(battleWork + 0x1C9C) = 1002;
            }
            break;

        case 1002:
            work[15] = 0x3C;
            *(s32*)(battleWork + 0x1C9C) = 1003;
            if (work[14] != -1) {
                psndSFXOff(work[14]);
            }
            /* fallthrough */

        case 1003:
            work[15]--;
            if (work[15] <= 0) {
                *(s32*)(battleWork + 0x1C9C) = 1004;
            }
            break;

        case 1004:
            *(s32*)(battleWork + 0x1CA0) = 0;
            *(s32*)(battleWork + 0x1CA8) = 0;
            *(s32*)(battleWork + 0x1CA4) = 0;
            *(s32*)(battleWork + 0x1CAC) = 0;
            return 0;

        default:
            break;
    }

    return 1;
}

s32 battleAcResult_MonosiriTarget(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDisp_MonosiriTarget(void) {
    switch (*(s32*)((s32)_battleWorkPointer + 0x1C9C)) {
        case 1000:
            actionCommandDisp(float_0_80428604, float_0_80428604);
            break;
    }
}

void battleAcDelete_MonosiriTarget(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}


/* CHATGPT STUB FILL: main/action/ac_monosiri_target 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    extern void* _battleWorkPointer;
    extern void btlGetScreenPoint(const f32* world, f32* screen);
    extern void btlDispTex4(s32 texture, const f32* position,
                            const void* params0, const void* params1,
                            const u32* color);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern u32 BattleACGetButtonIcon(s32 buttons, s32 pressed);
    extern void iconDispGx(f32 scale, const f32* position, s32 flags, u16 iconId);
    extern u32 vec3_80301220[];
    extern u32 vec3_8030122c[];
    extern u32 vec3_80301238[];
    extern u32 vec3_80301244[];
    extern u32 vec3_80301250[];
    extern u32 vec3_8030125c[];
    extern u32 vec3_80301268[];
    extern u32 vec3_80301274[];
    extern u32 vec3_80301280[];
    extern u32 vec3_8030128c[];
    extern u32 dat_804285f0;
    extern u32 dat_804285f4;
    extern u32 dat_804285f8;
    extern u32 dat_804285fc;
    extern u32 dat_80428600;
    extern f32 DAT_803012a0;

    u8* battle = _battleWorkPointer;
    u8* work = battle + 0x1F4C;
    f32 screen[3];
    u32 params0[3];
    u32 params1[3];
    u32 color;
    f32 glow = 0.0f;

    btlGetScreenPoint((f32*)(work + 0x14), screen);
    params0[0] = vec3_80301220[0];
    params0[1] = vec3_80301220[1];
    params0[2] = vec3_80301220[2];
    params1[0] = vec3_8030122c[0];
    params1[1] = vec3_8030122c[1];
    params1[2] = vec3_8030122c[2];
    color = dat_804285f0;
    btlDispTex4(0x58, screen, params0, params1, &color);

    screen[0] += 4.0f;
    screen[1] -= 4.0f;
    screen[2] -= 2.0f;
    params0[0] = vec3_80301238[0];
    params0[1] = vec3_80301238[1];
    params0[2] = vec3_80301238[2];
    params1[0] = vec3_80301244[0];
    params1[1] = vec3_80301244[1];
    params1[2] = vec3_80301244[2];
    color = dat_804285f4;
    btlDispTex4(0x58, screen, params0, params1, &color);

    if (*(s32*)(work + 0x30) != 0) {
        s32 step = *(s32*)(work + 0x34) % 0x1E;
        if (step < 0x14) {
            glow = 0.01f * (f32)intplGetValue(100.0, 0.0, 5, step, 0x14);
        }
    }

    btlGetScreenPoint((f32*)(work + 0x20), screen);
    params0[0] = vec3_80301250[0];
    params0[1] = vec3_80301250[1];
    params0[2] = vec3_80301250[2];
    params1[0] = vec3_8030125c[0];
    params1[1] = vec3_8030125c[1];
    params1[2] = *(u32*)(work + 0x2C);
    color = dat_804285f8;
    btlDispTex4(0x57, screen, params0, params1, &color);

    if (glow > 0.0f) {
        f32 glowParams[3];
        u32 glowUv[3];

        glowParams[0] = glow;
        glowParams[1] = glow;
        glowParams[2] = *(f32*)&vec3_80301268[2];
        glowUv[0] = vec3_80301274[0];
        glowUv[1] = vec3_80301274[1];
        glowUv[2] = *(u32*)(work + 0x2C);
        color = dat_804285fc;
        btlDispTex4(0x57, screen, glowParams, glowUv, &color);
    }

    screen[0] += 4.0f;
    screen[1] -= 4.0f;
    screen[2] -= 2.0f;
    params0[0] = vec3_80301280[0];
    params0[1] = vec3_80301280[1];
    params0[2] = vec3_80301280[2];
    params1[0] = vec3_8030128c[0];
    params1[1] = vec3_8030128c[1];
    params1[2] = *(u32*)(work + 0x2C);
    color = dat_80428600;
    btlDispTex4(0x57, screen, params0, params1, &color);

    *(f32*)(work + 0x2C) += 2.0f;
    {
        u16 normalIcon = (u16)BattleACGetButtonIcon(0x100, 0);
        u16 pressedIcon = (u16)BattleACGetButtonIcon(0x100, 1);
        f64 distance = (f64)*(f32*)(work + 0x14) - (f64)*(f32*)(work + 0x20);
        f32 iconPosition[3];

        if (distance < 0.0) {
            distance = -distance;
        }
        if (distance < 15.0) {
            normalIcon = pressedIcon;
        }
        iconPosition[0] = x - 200.0f;
        iconPosition[1] = y + 50.0f;
        iconPosition[2] = DAT_803012a0;
        iconDispGx(1.0f, iconPosition, 0x10, normalIcon);
    }
}
