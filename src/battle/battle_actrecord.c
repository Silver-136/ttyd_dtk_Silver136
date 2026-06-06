#include "battle/battle_actrecord.h"

extern void* _battleWorkPointer;

u8 BtlActRec_JudgeRuleKeep(void) {
    return 0;
}


u8 BtlActRec_JudgeTurnRuleKeep(void) {
    return 0;
}


u32 _check_turn_count_0_end(int param_1) {
    void* battleWork = _battleWorkPointer;
    u8* work = *(u8**)((s32)battleWork + 0x2738);
    u8* count = (u8*)((s32)battleWork + 0x16F57);
    u32 value;

    value = *count;
    if (value <= work[0x1B] || work[0x1B] == 0xFF) {
        if (param_1 == 0) {
            work[0x19] = 2;
        }
        else {
            work[0x19] = 1;
        }
    }

    value = *count;
    if (value <= work[0x1A] || work[0x1A] == 0xFF) {
        if (param_1 == 0) {
            work[0x19] = 6;
        }
        else if (!(work[0x19] & 2)) {
            work[0x19] = 1;
        }
    }

    return work[0x19];
}


u32 _check_turn_count_0_turn(int param_1) {
    void* battleWork = _battleWorkPointer;
    u8* work = *(u8**)((s32)battleWork + 0x2738);
    u8* count = (u8*)((s32)battleWork + 0x16F57);
    u32 value;

    value = *count;
    if (value <= work[0x1B] || work[0x1B] == 0xFF) {
        if (param_1 == 0) {
            work[0x19] = 2;
        }
        else {
            work[0x19] = 1;
        }
    }

    value = *count;
    if (value <= work[0x1A] || work[0x1A] == 0xFF) {
        if (param_1 == 0) {
            work[0x19] = 6;
        }
        else if (!(work[0x19] & 2)) {
            work[0x19] = 1;
        }
    }

    return work[0x19];
}


s32 _check_no_use(int param_1) {
    void* work = *(void**)((s32)_battleWorkPointer + 0x2738);
    s32 result;

    if (param_1 >= *(u8*)((s32)work + 0x1B)) {
        return 1;
    }
    result = 6;
    if (param_1 >= *(u8*)((s32)work + 0x1A)) {
        result = 2;
    }
    return result;
}


int _check_use(u32 param_1) {
    void* work = *(void**)((s32)_battleWorkPointer + 0x2738);

    if ((s32)param_1 >= *(u8*)((s32)work + 0x1A)) {
        return 6;
    }
    return ((s32)param_1 >= *(u8*)((s32)work + 0x1B)) + 1;
}


void BtlActRec_AddPoint(u8* value, u8 point) {
    *value += point;
    if (*value > 100) {
        *value = 100;
    }
}


void BtlActRec_AddCount(u8* value) {
    if (*value < 100) {
        (*value)++;
    }
}
