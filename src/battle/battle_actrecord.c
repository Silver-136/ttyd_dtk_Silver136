#include "battle/battle_actrecord.h"

extern void* _battleWorkPointer;

u8 BtlActRec_JudgeRuleKeep(void) {
    return 0;
}


u8 BtlActRec_JudgeTurnRuleKeep(void) {
    extern void* _battleWorkPointer;
    extern u32 _check_turn_count_0_turn(int);

    u8* battleWork = _battleWorkPointer;
    u8* work = *(u8**)(battleWork + 0x2738);
    u8* record = battleWork + 0x16F38;
    s32 rule = work[0x18];
    int value;

    if (rule >= 0x3F) {
        goto high_dispatch;
    }
    if (rule < 0x3C) {
        return 0;
    }

low_dispatch:
    if ((u32)rule == 0x3E) {
        value = record[2] + record[1] + record[3] + record[9] + record[0xA] + record[0xD] +
                record[0x17] + record[0x19] + record[0x1B] + record[0x1D] + record[0x1E] +
                record[0xB] + record[0xC] + record[0xE] + record[0x18] + record[0x1A] +
                record[0x1C] + record[0];
    }
    else if ((u32)rule == 0x3C) {
        value = record[2] + record[1] + record[3] + record[9] + record[0xA] + record[0xD] +
                record[0x17] + record[0x19] + record[0x1B] + record[0];
    }
    else {
        value = record[0xB] + record[0x1E] + record[0xC] + record[0xE] + record[0x18] +
                record[0x1A] + record[0x1C] + record[0x1D];
    }
    return _check_turn_count_0_turn(value);

high_dispatch:
    if (rule >= 0x42) {
        return 0;
    }
    if ((u32)rule == 0x41) {
        value = record[2] + record[1] + record[9] + record[0x1D] + record[0xB] + record[0];
    }
    else if ((u32)rule == 0x3F) {
        value = record[2] + record[1] + record[9] + record[0];
    }
    else {
        value = record[0x1D] + record[0xB];
    }
    return _check_turn_count_0_turn(value);
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
