#include "action/ac_crane_timing.h"

void battleAcDelete_CraneTiming(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

void battleAcDisp_CraneTiming(void) {
    ;
}

s32 battleAcResult_CraneTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

s32 battleAcMain_CraneTiming(void* battleWork) {
    extern void* memset(void* dest, int ch, u32 count);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern s32 irand(s32 range);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern f32 reviseAngle(f32 angle);
    extern f32 sinfd(f32 angle);
    extern void BtlUnit_SetPos(void* unit, f32 x, f32 y, f32 z);
    extern s32 BattlePadCheckTrigger(u32 buttons);

    extern f32 float_neg300_80427070;
    extern f32 float_0_80427074;
    extern f32 float_1p5_80427078;
    extern f32 float_5_8042707c;
    extern f32 float_neg1_80427080;
    extern f64 double_5_802fe680;
    extern f64 double_20_802fe688;

    s32 bw;
    s32 work;
    s32 disp;
    void* activeUnit;
    void* unit;
    void* part;
    s32 autoCommand;
    s32 state;
    s32 value;
    f32 diff;
    s32 nearAuto;

    bw = (s32)battleWork;
    autoCommand = 0;
    if (*(u8*)(*(s32*)(bw + 0x1C90) + 0x307) != 0) {
        autoCommand = 1;
    }

    work = bw + 0x1F4C;
    disp = bw + 0x1F20;

    unit = BattleGetUnitPtr(battleWork, *(s32*)(bw + 0x1CCC));
    part = BtlUnit_GetPartsPtr(unit, *(s32*)(bw + 0x1CD0));

    state = *(s32*)(bw + 0x1C9C);

    if (state == 1000) {
        goto state_1000;
    }

    if (state > 999) {
        if (state == 1003) {
            goto state_1003;
        }
        if (state > 1002) {
            if (state > 1004) {
                return 1;
            }
            goto state_1004;
        }
        if (state >= 1002) {
            goto state_1002;
        }
        return 1;
    }

    if (state == 100) {
        goto state_100;
    }

    if (state > 99) {
        return 1;
    }

    if (state != 0) {
        return 1;
    }

    memset((void*)disp, 0, 0x2C);
    *(f32*)(disp + 0x14) = float_neg300_80427070;
    *(f32*)(disp + 0x18) = float_0_80427074;
    *(s32*)(disp + 0x20) = 0x14;

    *(s32*)(work + 4) = 0;
    *(s32*)(work + 0x24) = *(s32*)(bw + 0x1CD4);
    *(f32*)(work + 0x28) = (f32)irand(*(s32*)(bw + 0x1CD8));
    *(s32*)work = 0x100;
    *(s32*)(bw + 0x1CE8) = 0;
    *(s32*)(bw + 0x1C9C) = 99;
    *(s32*)(bw + 0x1CB8) = 1;
    return 1;

state_100:
    unit = BattleGetUnitPtr(battleWork, *(s32*)(bw + 0x1CCC));
    part = BtlUnit_GetPartsPtr(unit, *(s32*)(bw + 0x1CD0));

    BtlUnit_GetPos(unit, (f32*)(work + 0x14), (f32*)(work + 0x18), (f32*)(work + 0x1C));

    *(s32*)(work + 8) = *(s32*)(work + 0x14);
    *(s32*)(work + 0xC) = *(s32*)(work + 0x18);
    *(s32*)(work + 0x10) = *(s32*)(work + 0x1C);
    *(s32*)(work + 4) = 0;
    *(f32*)(work + 0x20) = float_1p5_80427078;
    *(s32*)(bw + 0x1C9C) = 1000;

state_1000:
    if (*(s32*)(work + 0x24) != 0) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x24) - 1;
    }

    *(s32*)(work + 4) = *(s32*)(work + 4) + 1;
    *(f32*)(work + 0x14) = *(f32*)(work + 0x14) + *(f32*)(work + 0x20);

    *(f32*)(work + 0x18) =
        *(f32*)(work + 0xC) +
        (float_5_8042707c * sinfd(reviseAngle((f32)(*(s32*)(work + 4) * 5))));

    if (*(f32*)(work + 0x14) >= (*(f32*)(work + 8) + *(f32*)(work + 0x28)) &&
        *(f32*)(work + 0x20) > float_0_80427074) {
        *(f32*)(work + 0x20) = *(f32*)(work + 0x20) * float_neg1_80427080;
        *(f32*)(work + 0x28) = (f32)irand(*(s32*)(bw + 0x1CD8));
    }

    if (*(f32*)(work + 0x14) <= (*(f32*)(work + 8) - *(f32*)(work + 0x28)) &&
        *(f32*)(work + 0x20) < float_0_80427074) {
        *(f32*)(work + 0x20) = *(f32*)(work + 0x20) * float_neg1_80427080;
        *(f32*)(work + 0x28) = (f32)irand(*(s32*)(bw + 0x1CD8));
    }

    if (*(f32*)(work + 0x20) < float_0_80427074) {
        *(s8*)((s32)part + 0x189) = -1;
    } else {
        *(s8*)((s32)part + 0x189) = 1;
    }

    BtlUnit_SetPos(unit, *(f32*)(work + 0x14), *(f32*)(work + 0x18), *(f32*)(work + 0x1C));

    nearAuto = 0;
    if (autoCommand != 0) {
        diff = *(f32*)(work + 8) - *(f32*)(work + 0x14);
        if (diff < 0.0f) {
            diff = -diff;
        }
        if ((f64)diff < double_5_802fe680) {
            nearAuto = 1;
        }
    }

    value = BattlePadCheckTrigger(*(u32*)work);
    if (((value != 0) && (*(s32*)(work + 0x24) < 1)) ||
        ((nearAuto != 0) && (*(s32*)(work + 0x24) < 1))) {
        *(s32*)(bw + 0x1CE8) = 1;

        diff = *(f32*)(work + 8) - *(f32*)(work + 0x14);
        if (diff < 0.0f) {
            diff = -diff;
        }

        if ((f64)diff <= double_20_802fe688) {
            *(s32*)(bw + 0x1CB8) = 2;
            *(s32*)(bw + 0x1CB4) = *(s32*)(bw + 0x1CB4) + 1;
        } else {
            *(s32*)(bw + 0x1CB8) = 0;
        }

        *(s32*)(bw + 0x1C9C) = 1002;
    } else if (*(s32*)(bw + 0x1CE0) <= *(s32*)(work + 4)) {
        *(s32*)(bw + 0x1CB8) = 0;
        *(s32*)(bw + 0x1C9C) = 1002;
    }

    return 1;

state_1002:
    *(s32*)(work + 0x2C) = 0x3C;
    *(s32*)(bw + 0x1C9C) = 1003;

state_1003:
    *(s32*)(work + 0x2C) = *(s32*)(work + 0x2C) - 1;
    if (*(s32*)(work + 0x2C) > 0) {
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
