#include "battle/battle_acrobat.h"


s32 BattleAcrobatStart(void* battleWork, s32 unitId, s32 a, s32 b, s32 c, s32 d) {
    *(s32*)((s32)battleWork + 0x271C) = 0;
    *(s32*)((s32)battleWork + 0x2720) = a;
    *(s32*)((s32)battleWork + 0x2724) = b;
    *(s32*)((s32)battleWork + 0x2728) = c;
    *(s32*)((s32)battleWork + 0x272C) = unitId;
    *(s32*)((s32)battleWork + 0x2730) = 0;
    *(s32*)((s32)battleWork + 0x2734) = d;
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
s32 BattleAcrobatMain(void* battleWork) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern f32 dispCalcZ(Vec3* pos);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void _accrobat_timing_icon_disp(s32, void*);
    extern s32 BattlePadCheckRecordTrigger(s32, s32);
    extern s32 BattlePadCheckTrigger(s32);

    u8* unit;
    s32 hit;
    s32 i;
    Vec3 pos;

    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x272C));
    hit = 0;
    (*(s32*)((s32)battleWork + 0x271C))++;
    if (unit == NULL) {
        *(s32*)((s32)battleWork + 0x2730) = 1;
        return 1;
    }
    if (*(u32*)(unit + 0x27C) & 0x10) {
        *(s32*)((s32)battleWork + 0x2730) = 1;
        return 1;
    }
    if ((*(u32*)((s32)battleWork + 0x163F4) & 4) &&
        *(s32*)((s32)battleWork + 0x271C) > *(s32*)((s32)battleWork + 0x2720) &&
        *(s32*)((s32)battleWork + 0x271C) <= *(s32*)((s32)battleWork + 0x2724)) {
        pos.x = *(f32*)(unit + 0x3C);
        pos.y = *(f32*)(unit + 0x40);
        pos.z = *(f32*)(unit + 0x44) + 15.0f;
        dispEntry(4, 1, _accrobat_timing_icon_disp, unit, dispCalcZ(&pos));
    }
    if (unit[0x307] != 0 &&
        *(s32*)((s32)battleWork + 0x271C) > *(s32*)((s32)battleWork + 0x2720) &&
        *(s32*)((s32)battleWork + 0x271C) <= *(s32*)((s32)battleWork + 0x2724)) {
        hit = 1;
    }
    if (*(s32*)((s32)battleWork + 0x271C) <= 1 && *(s32*)((s32)battleWork + 0x2734) > 0) {
        for (i = 0; i < *(s32*)((s32)battleWork + 0x2734); i++) {
            if (BattlePadCheckRecordTrigger(i, 0x100) != 0) {
                *(s32*)((s32)battleWork + 0x2730) = -1;
                return 1;
            }
        }
    }
    if (BattlePadCheckTrigger(0x100) != 0 || hit != 0) {
        if (*(s32*)((s32)battleWork + 0x271C) <= *(s32*)((s32)battleWork + 0x2720)) {
            *(s32*)((s32)battleWork + 0x2730) = -1;
            return 1;
        }
        if (*(s32*)((s32)battleWork + 0x271C) <= *(s32*)((s32)battleWork + 0x2724)) {
            *(s32*)((s32)battleWork + 0x2730) = 2;
            return 1;
        }
        *(s32*)((s32)battleWork + 0x2730) = -2;
        return 1;
    }
    if (*(s32*)((s32)battleWork + 0x271C) >= *(s32*)((s32)battleWork + 0x2728)) {
        *(s32*)((s32)battleWork + 0x2730) = 1;
        return 1;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void _accrobat_timing_icon_disp(s32 param_1, void* unit) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern void iconDispGx(f32* pos, s32 param_2, s32 icon, f32 scale);
    extern f32 float_15_804245d8;
    extern f32 float_0p67_804245dc;

    f32 z;
    f32 y;
    f32 x;

    BtlUnit_GetPos(unit, &x, &y, &z);
    y += BtlUnit_GetHeight(unit) + 5;
    x += float_15_804245d8;
    iconDispGx(&x, 0, 0xA2, float_0p67_804245dc);
}


s32 BattleAcrobatGetResult(void* battleWork, s32* result1, s32* result2) {
    *result1 = *(s32*)((s32)battleWork + 0x2730);
    *result2 = *(s32*)((s32)battleWork + 0x271C);
    return 2;
}
