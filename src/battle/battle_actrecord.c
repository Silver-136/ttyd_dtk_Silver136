#include "battle/battle_actrecord.h"

extern void* _battleWorkPointer;

void BtlActRec_JudgeRuleKeep(void) {
    extern void* _battleWorkPointer;
    extern s32 _check_no_use(s32);
    extern s32 _check_use(u32);
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartyPtr(void*);
    char* battle = _battleWorkPointer;
    char* info = *(char**)(battle + 0x2738);
    u8* rec = (u8*)(battle + 0x16F38);
    char* mario;
    s32 value = 0;
    s32 rule;

    mario = BattleGetMarioPtr(battle);
    BattleGetPartyPtr(battle);
    rule = info[0x18];
    switch (rule) {
        case 0: info[0x19] = 6; return;
        case 1: value = rec[0]; break;
        case 2: value = rec[0]; break;
        case 3: value = rec[1]; break;
        case 4: value = rec[1]; break;
        case 5: value = rec[2] + rec[3]; break;
        case 6: value = rec[2] + rec[3]; break;
        case 7: value = rec[4]; break;
        case 8: value = rec[4]; break;
        case 9: value = rec[5]; break;
        case 10: value = rec[5]; break;
        case 11: value = rec[4] + rec[5]; break;
        case 12: value = rec[4] + rec[5]; break;
        case 13: value = rec[6]; break;
        case 14: value = rec[6]; break;
        case 15: value = rec[7]; break;
        case 16: value = rec[7]; break;
        case 17: value = rec[6] + rec[7]; break;
        case 18: value = rec[6] + rec[7]; break;
        case 19:
        case 20:
            if (mario == 0) { info[0x19] = 1; return; }
            value = *(s16*)(mario + 0x10);
            break;
        case 21: value = rec[8]; break;
        case 22: value = rec[9] + rec[10]; break;
        case 23: value = rec[9] + rec[10]; break;
        case 24: value = rec[11] + rec[12]; break;
        case 25: value = rec[11] + rec[12]; break;
        case 26: value = rec[9] + rec[10] + rec[11] + rec[12]; break;
        case 27: value = rec[9] + rec[10] + rec[11] + rec[12]; break;
        case 28: value = rec[13]; break;
        case 29: value = rec[13]; break;
        case 30: value = rec[14]; break;
        case 31: value = rec[14]; break;
        case 32: value = rec[13] + rec[14]; break;
        case 33: value = rec[13] + rec[14]; break;
        case 34: value = rec[15]; break;
        case 35: value = rec[15]; break;
        case 36: value = rec[16]; break;
        case 37: value = rec[16]; break;
        case 38: value = rec[17]; break;
        case 39: value = rec[17]; break;
        case 40: value = rec[18]; break;
        case 41: value = rec[18]; break;
        case 42: value = rec[17] + rec[18]; break;
        case 43: value = rec[17] + rec[18]; break;
        case 44: value = rec[19]; break;
        case 45: value = rec[19]; break;
        case 46: value = rec[20]; break;
        case 47: value = rec[20]; break;
        case 48: value = rec[19] + rec[20]; break;
        case 49: value = rec[19] + rec[20]; break;
        case 50: value = rec[21]; break;
        case 51: value = rec[21]; break;
        case 52: value = rec[22]; break;
        case 53: value = rec[22]; break;
        case 54: value = rec[19] + rec[20]; break;
        case 55: value = rec[19] + rec[20]; break;
        case 56: value = rec[21] + rec[22]; break;
        case 57: value = rec[21] + rec[22]; break;
        case 58:
            value = rec[23];
            if (info[0x1A] < value) info[0x19] = 6;
            else if (info[0x1B] < value) info[0x19] = 2;
            else info[0x19] = 1;
            return;
        case 59:
            value = rec[23];
            if (info[0x1A] < value) info[0x19] = info[0x1B] < value ? 1 : 2;
            else info[0x19] = 6;
            return;
        default:
            info[0x19] = 6;
            return;
    }
    if ((rule & 1) != 0 || rule == 20 || rule == 22 || rule == 24 || rule == 26 || rule == 28 || rule == 30 || rule == 32 || rule == 34 || rule == 36 || rule == 38 || rule == 40 || rule == 42 || rule == 44 || rule == 46 || rule == 48 || rule == 50 || rule == 52 || rule == 54 || rule == 56) {
        info[0x19] = _check_no_use(value);
    } else {
        info[0x19] = _check_use(value);
    }
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
