#include "action/ac_timing_a.h"

s32 BattleActionCommandGetDifficulty(void);
s32* _ac_timing_a_timing_tbl[4];

void battleAcDelete_TimingA(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3E8;
}

s32 battleAcTimingA_GetSuccessFrame(void* obj) {
    s32 difficulty;
    s32* entry;
    s32 frame;
    s32 result;

    difficulty = BattleActionCommandGetDifficulty();

    frame = *(s32*)((s32)obj + 0x1CB4);
    entry = *(s32**)((s32)_ac_timing_a_timing_tbl + difficulty * 4);
    result = entry[1];

    if ((*(s32*)((s32)obj + 0x1F50) & 2) != 0) {
        if (frame >= 100000) {
            return 0;
        }

loop:
        if (frame <= entry[0]) {
            goto done;
        }

        entry += 2;
        goto loop;

done:
        return entry[1];
    }

    return result;
}


s32 battleAcResult_TimingA(void* battleWork) {
    return 0;
}


void actionCommandDisp(f32 x, f32 y) {
}


s32 battleAcMain_TimingA(int param_1) {
    return 0;
}


void battleAcDisp_TimingA(s32 param_1, void* battleWork) {
}
