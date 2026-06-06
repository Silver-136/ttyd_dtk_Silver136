#include "action/ac_repeatedly.h"

void BattleAcGaugeSeDelete(void* wp);

s32 battleAcResult_Repeatedly(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_Repeatedly(void* wp) {
    void* callArg;

    switch (*(s32*)((s32)wp + 0x1CC8)) {
        case 10:
            *(u32*)((s32)wp + 0xEF4) &= ~0x20000000;
            break;
        default:
            break;
    }
    BattleAcGaugeSeDelete(callArg);
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

s32 battleAcMain_Repeatedly(void* battleWork) {
    return 0;
}


void actionCommandDisp(f32 x, f32 y) {
}

typedef struct AcRepeatedlyDispWork {
    u8 pad_00[0x14];
    f32 x;
    f32 y;
    u8 pad_1C[0x4];
    s32 timer;
} AcRepeatedlyDispWork;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_Repeatedly(s32 param_1, void* battleWork) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    extern f32 float_neg300_80422ab8;
    extern f32 float_30_80422abc;
    register AcRepeatedlyDispWork* work = (AcRepeatedlyDispWork*)((s32)battleWork + 0x1F20);
    s32 status = *(s32*)((s32)battleWork + 0x1C9C);
    s32 timer;

    if (status < 1000) {
        if (status >= 101) {
            return;
        }
        if (status >= 99) {
            goto opening;
        }
        return;
    } else {
        if (status >= 1004) {
            return;
        }
        if (status >= 1002) {
            goto closing;
        }
    }

opening:
    work->x = intplGetValue(
        4,
        float_neg300_80422ab8,
        float_30_80422abc,
        20 - work->timer,
        20
    );
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer > 0) {
        work->timer = timer - 1;
    }
    return;

closing:
    timer = work->timer;
    if (timer >= 40) {
        work->x = intplGetValue(
            4,
            float_30_80422abc,
            float_neg300_80422ab8,
            timer - 40,
            20
        );
    } else {
        work->x = float_30_80422abc;
    }
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer < 60) {
        work->timer = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
