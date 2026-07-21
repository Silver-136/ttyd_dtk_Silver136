#include "action/ac_power_gauge_lv.h"

s32 battleAcMain_PowerGaugeLv(void* wp) {
    extern void* memset(void* dest, s32 value, u32 size);
    extern s32 irand(s32 max);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern s32 BattlePadGetNow(void);
    extern f32 float_neg300_80427b48;
    extern f32 float_0_80427b50;
    extern f32 float_100_80427b38;
    extern f32 float_101_80427b54;
    extern u32 dat_80427b28;

    u8* battleWork;
    u8* disp;
    u8* extra;
    s32 state;
    s32 value;
    s32 maxLevel;
    f32 gauge;

    battleWork = (u8*)wp;
    extra = battleWork + 0x1F4C;
    disp = battleWork + 0x1F20;
    state = *(s32*)(battleWork + 0x1C9C);

    switch (state) {
        case 0:
            memset(disp, 0, 0x2C);
            *(f32*)(disp + 0x14) = float_neg300_80427b48;
            *(f32*)(disp + 0x18) = float_0_80427b50;
            *(s32*)(disp + 0x20) = 0x14;
            *(f32*)(disp + 0x28) = float_0_80427b50;
            *(u32*)(disp + 0x0C) = dat_80427b28;
            *(s32*)(extra + 0x00) = 0;
            *(s32*)(battleWork + 0x1CE8) = 0;
            *(s32*)(battleWork + 0x1C9C) = 99;
            *(s32*)(battleWork + 0x1CB8) = 1;
            *(s32*)(extra + 0x0C) = 100;
            *(f32*)(extra + 0x04) = float_0_80427b50;
            return 1;

        case 99:
            return 1;

        case 100:
            if ((*(u32*)(battleWork + 0x1C94) & 1) != 0 &&
                ((*(u32*)(*(u8**)(battleWork + 0x1C90) + 0x27C) & 0x10) != 0)) {
                if (irand(100) < 0) {
                    *(s32*)(battleWork + 0x1CE8) = irand(*(s32*)(battleWork + 0x1CCC) - 1) + 1;
                    *(s32*)(battleWork + 0x1C98) = 0x1E;
                } else {
                    *(s32*)(battleWork + 0x1C98) = 0x1E;
                    *(s32*)(battleWork + 0x1CE8) = 0;
                }
                *(s32*)(battleWork + 0x1C9C) = 1001;
                return 1;
            }
            *(s32*)(battleWork + 0x1C9C) = 1000;
            /* fallthrough */

        case 1000:
            *(s32*)(extra + 0x00) = *(s32*)(extra + 0x00) + 1;
            value = *(s32*)(extra + 0x00);
            if (value > *(s32*)(extra + 0x0C)) {
                *(s32*)(battleWork + 0x1CE8) = 0;
                *(s32*)(battleWork + 0x1CB8) = 0;
                *(f32*)(extra + 0x04) = float_101_80427b54;
                return 0;
            }
            *(f32*)(extra + 0x04) = intplGetValue(2, value, *(s32*)(extra + 0x0C), float_0_80427b50, float_100_80427b38);
            if (BattlePadGetNow() == 0x60) {
                gauge = *(f32*)(extra + 0x04);
                if (gauge < (f32)*(s32*)(battleWork + 0x1CD0)) {
                    *(s32*)(battleWork + 0x1CB8) = 0;
                    *(s32*)(battleWork + 0x1CB8) = *(s32*)(battleWork + 0x1CB8) & ~2;
                } else {
                    *(s32*)(battleWork + 0x1CB8) = 2;
                    *(s32*)(battleWork + 0x1CB4) = *(s32*)(battleWork + 0x1CB4) + 1;
                    maxLevel = *(s32*)(battleWork + 0x1CCC);
                    if (maxLevel <= 1) {
                        *(s32*)(battleWork + 0x1CE8) = 1;
                    } else if (gauge < (f32)*(s32*)(battleWork + 0x1CD4)) {
                        *(s32*)(battleWork + 0x1CE8) = 1;
                    } else if (maxLevel <= 2) {
                        *(s32*)(battleWork + 0x1CE8) = 2;
                    } else if (gauge < (f32)*(s32*)(battleWork + 0x1CD8)) {
                        *(s32*)(battleWork + 0x1CE8) = 2;
                    } else if (maxLevel <= 3) {
                        *(s32*)(battleWork + 0x1CE8) = 3;
                    } else if (gauge < (f32)*(s32*)(battleWork + 0x1CDC)) {
                        *(s32*)(battleWork + 0x1CE8) = 3;
                    } else if (maxLevel <= 4) {
                        *(s32*)(battleWork + 0x1CE8) = 4;
                    } else {
                        *(s32*)(battleWork + 0x1CE8) = 5;
                    }
                }
                *(s32*)(battleWork + 0x1C9C) = 1002;
            }
            return 1;

        case 1001:
            *(s32*)(battleWork + 0x1C98) = *(s32*)(battleWork + 0x1C98) - 1;
            if (*(s32*)(battleWork + 0x1C98) >= 0) {
                return 1;
            }
            if (*(s32*)(battleWork + 0x1CE8) > 0) {
                *(s32*)(battleWork + 0x1CB8) = 2;
                *(s32*)(battleWork + 0x1CB4) = *(s32*)(battleWork + 0x1CB4) + 1;
            } else {
                *(s32*)(battleWork + 0x1CB8) = 0;
            }
            return 0;

        case 1002:
            *(s32*)(battleWork + 0x1C9C) = 1003;
            /* fallthrough */

        case 1003:
            *(u32*)(battleWork + 0x1CC0) = *(u32*)(battleWork + 0x1CC0) | 1;
            *(s32*)(extra + 0x10) = 0x3C;
            *(s32*)(battleWork + 0x1C9C) = 1004;
            /* fallthrough */

        case 1004:
            *(s32*)(extra + 0x10) = *(s32*)(extra + 0x10) - 1;
            if (*(s32*)(extra + 0x10) > 0) {
                return 1;
            }
            *(s32*)(battleWork + 0x1C9C) = 1005;
            return 1;

        case 1005:
            *(s32*)(battleWork + 0x1CA0) = 0;
            *(s32*)(battleWork + 0x1CA8) = 0;
            *(s32*)(battleWork + 0x1CA4) = 0;
            *(s32*)(battleWork + 0x1CAC) = 0;
            return 0;

        default:
            return 1;
    }
}

s32 battleAcResult_PowerGaugeLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_PowerGaugeLv(void* camera, void* wp) {
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

    extern f32 float_neg300_80427b48;
    extern f32 float_30_80427b4c;
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
                float_neg300_80427b48,
                float_30_80427b4c);
            /* fallthrough */
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
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427b4c,
                    float_neg300_80427b48);
            } else {
                disp->x = float_30_80427b4c;
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


void battleAcDelete_PowerGaugeLv(void* wp) {
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

    extern void* _battleWorkPointer;
    extern Vec vec3_802ff340[];
    extern f32 float_neg200_80427b2c;
    extern f32 float_25_80427b30;
    extern f32 float_1_80427b34;
    extern f32 float_100_80427b38;
    extern f32 float_neg220_80427b3c;
    extern f32 float_70_80427b40;
    extern f32 float_56_80427b44;
    extern void* camGetPtr(s32 cameraId);
    extern void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);
    extern void BattleAcDrawGauge(s32 x, s32 y, s32 icon, s32 a, s32 b, s32 c, s32 d, s32 zero, f32 value);

    u8* battleWork;
    Vec* vec;
    u8* disp;
    u8* extra;
    s32 state;
    f32 ratio;
    f32 baseX;
    f32 baseY;
    Vec pos0;
    Vec pos1;
    Vec pos2;
    Vec pos3;
    Vec pos4;
    Vec pos5;
    Vec pos6;

    battleWork = (u8*)_battleWorkPointer;
    vec = vec3_802ff340;
    disp = battleWork + 0x1F20;
    camGetPtr(8);
    extra = battleWork + 0x1F4C;

    pos0 = vec[0];
    pos0.x = float_neg200_80427b2c + x;
    pos0.y = float_25_80427b30 + y;
    iconDispGx(&pos0, 0x10, 0x94, float_1_80427b34);

    ratio = *(f32*)(extra + 0x4) / float_100_80427b38;
    if (ratio > float_1_80427b34) {
        ratio = float_1_80427b34;
    }
    *(f32*)(disp + 0x28) = ratio;

    BattleAcDrawGauge((s32)x, (s32)y, 0xB2,
        *(s32*)(battleWork + 0x1CCC),
        *(s32*)(battleWork + 0x1CD0),
        *(s32*)(battleWork + 0x1CD4),
        *(s32*)(battleWork + 0x1CD8),
        0,
        *(f32*)(disp + 0x28));

    switch (*(s32*)(battleWork + 0x1C9C)) {
        case 99:
        case 100:
        case 1002:
        case 1003:
        case 1004:
            baseX = float_neg220_80427b3c + x;
            baseY = float_70_80427b40 + y;
            pos1 = vec[1];
            pos1.x = baseX;
            pos1.y = baseY;
            iconDispGx(&pos1, 0x10, 0x86, float_1_80427b34);
            pos2 = vec[2];
            pos2.x = float_56_80427b44 + baseX;
            pos2.y = baseY;
            iconDispGx(&pos2, 0x10, 0x88, float_1_80427b34);
            break;

        case 1000:
            if (*(f32*)(extra + 0x4) >= (f32)*(s32*)(battleWork + 0x1CD0) &&
                *(f32*)(extra + 0x4) <= float_100_80427b38) {
                baseX = float_neg220_80427b3c + x;
                baseY = float_70_80427b40 + y;
                pos3 = vec[3];
                pos3.x = baseX;
                pos3.y = baseY;
                iconDispGx(&pos3, 0x10, 0x87, float_1_80427b34);
                pos4 = vec[4];
                pos4.x = float_56_80427b44 + baseX;
                pos4.y = baseY;
                iconDispGx(&pos4, 0x10, 0x89, float_1_80427b34);
            } else {
                baseX = float_neg220_80427b3c + x;
                baseY = float_70_80427b40 + y;
                pos5 = vec[5];
                pos5.x = baseX;
                pos5.y = baseY;
                iconDispGx(&pos5, 0x10, 0x86, float_1_80427b34);
                pos6 = vec[6];
                pos6.x = float_56_80427b44 + baseX;
                pos6.y = baseY;
                iconDispGx(&pos6, 0x10, 0x88, float_1_80427b34);
            }
            break;
    }
}
