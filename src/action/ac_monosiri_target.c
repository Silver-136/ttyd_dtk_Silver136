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
s32 battleAcMain_MonosiriTarget(int param_1) {
    return 0;
}
