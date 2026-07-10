#include "action/ac_monosiri_target.h"

extern void* _battleWorkPointer;
extern s32 _frame_table[4];
extern f32 float_0_80428604;

s32 BattleActionCommandGetDifficulty(void* battleWork);
void actionCommandDisp(f32 x, f32 y);

s32 battleAcResult_MonosiriTarget(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_MonosiriTarget(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

void battleAcDisp_MonosiriTarget(void) {
    switch (*(s32*)((s32)_battleWorkPointer + 0x1C9C)) {
        case 1000:
            actionCommandDisp(float_0_80428604, float_0_80428604);
            break;
    }
}

s32 _getmaxcount(void) {
    return _frame_table[BattleActionCommandGetDifficulty(_battleWorkPointer)];
}


/* CHATGPT STUB FILL: main/action/ac_monosiri_target 20260624_184929 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    return;
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

