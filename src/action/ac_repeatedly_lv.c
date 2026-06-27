#include "action/ac_repeatedly_lv.h"

void BattleAcGaugeSeDelete(void* wp);
f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
extern f32 float_neg300_80424ba4;
extern f32 float_30_80424ba8;

s32 battleAcResult_RepeatedlyLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_RepeatedlyLv(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
    BattleAcGaugeSeDelete(wp);
}


void actionCommandDisp(f32 x, f32 y) {
    ;
}


s32 battleAcMain_RepeatedlyLv(void* battleWork) {
    return 0;
}

typedef struct AcRepeatedlyLvDispWork {
    u8 pad_00[0x14];
    f32 x;
    f32 y;
    u8 pad_1C[0x4];
    s32 timer;
} AcRepeatedlyLvDispWork;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_RepeatedlyLv(s32 unused, void* battleWork) {
    register AcRepeatedlyLvDispWork* disp = (AcRepeatedlyLvDispWork*)((s32)battleWork + 0x1F20);
    s32 state;
    s32 timer;

    state = *(s32*)((s32)battleWork + 0x1C9C);
    if (state < 1000) {
        if (state >= 101) {
            return;
        }
        if (state >= 99) {
            goto opening;
        }
        return;
    } else {
        if (state >= 1004) {
            return;
        }
        if (state >= 1002) {
            goto closing;
        }
    }

opening:
    disp->x = intplGetValue(4, float_neg300_80424ba4, float_30_80424ba8,
                            0x14 - disp->timer, 0x14);
    actionCommandDisp(disp->x, disp->y);
    timer = disp->timer;
    if (timer > 0) {
        disp->timer = timer - 1;
    }
    return;

closing:
    timer = disp->timer;
    if (timer >= 0x28) {
        disp->x = intplGetValue(4, float_30_80424ba8, float_neg300_80424ba4,
                                timer - 0x28, 0x14);
    } else {
        disp->x = float_30_80424ba8;
    }
    actionCommandDisp(disp->x, disp->y);
    timer = disp->timer;
    if (timer < 0x3C) {
        disp->timer = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/action/ac_repeatedly_lv 20260624_184008 */

/* stub-fill: _init_param | missing_definition | ghidra_signature */
u8 _init_param(int param_1) {
    return 0;
}
