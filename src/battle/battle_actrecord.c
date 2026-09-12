#include "battle/battle_actrecord.h"

extern void* _battleWorkPointer;

u32 _check_turn_count_0_turn(int value);

void BtlActRec_AddCount(u8* value) {
    if (*value < 100) {
        (*value)++;
    }
}


void BtlActRec_AddPoint(u8* value, u8 point) {
    *value += point;
    if (*value > 100) {
        *value = 100;
    }
}

u8 BtlActRec_JudgeTurnRuleKeep(void) {
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

void BtlActRec_JudgeRuleKeep(void) {
    extern s32 _check_no_use(s32);
    extern s32 _check_use(u32);
    extern u32 _check_turn_count_0_end(s32);
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartyPtr(void*);
    char* battle = _battleWorkPointer;
    u8* rec = (u8*)(battle + 0x16F38);
    char* info = *(char**)(battle + 0x2738);
    char* mario;
    s32 value = 0;
    s32 rule;

    mario = BattleGetMarioPtr(battle);
    BattleGetPartyPtr(battle);
    rule = info[0x18];
    switch (rule) {
        case 0: info[0x19] = 6; return;
        case 1: info[0x19] = _check_no_use(rec[0]); break;
        case 2: info[0x19] = _check_use(rec[0]); break;
        case 3: info[0x19] = _check_no_use(rec[1]); break;
        case 4: info[0x19] = _check_use(rec[1]); break;
        case 5: info[0x19] = _check_no_use(rec[2] + rec[3]); break;
        case 6: info[0x19] = _check_use(rec[2] + rec[3]); break;
        case 7: info[0x19] = _check_no_use(rec[4]); break;
        case 8: info[0x19] = _check_use(rec[4]); break;
        case 9: info[0x19] = _check_no_use(rec[5]); break;
        case 10: info[0x19] = _check_use(rec[5]); break;
        case 11: info[0x19] = _check_no_use(rec[4] + rec[5]); break;
        case 12: info[0x19] = _check_use(rec[4] + rec[5]); break;
        case 13: info[0x19] = _check_no_use(rec[6]); break;
        case 14: info[0x19] = _check_use(rec[6]); break;
        case 15: info[0x19] = _check_no_use(rec[7]); break;
        case 16: info[0x19] = _check_use(rec[7]); break;
        case 17: info[0x19] = _check_no_use(rec[6] + rec[7]); break;
        case 18: info[0x19] = _check_use(rec[6] + rec[7]); break;
        case 19:
        case 20:
            if (mario == 0) { info[0x19] = 1; return; }
            value = *(s16*)(mario + 0x10);
            if (rule == 19) {
                info[0x19] = _check_use(value);
            } else {
                info[0x19] = _check_no_use(value);
            }
            break;
        case 21: info[0x19] = _check_use(rec[8]); break;
        case 22: info[0x19] = _check_no_use(rec[9] + rec[10]); break;
        case 23: info[0x19] = _check_use(rec[9] + rec[10]); break;
        case 24: info[0x19] = _check_no_use(rec[11] + rec[12]); break;
        case 25: info[0x19] = _check_use(rec[11] + rec[12]); break;
        case 26: info[0x19] = _check_no_use(rec[9] + rec[10] + rec[11] + rec[12]); break;
        case 27: info[0x19] = _check_use(rec[9] + rec[10] + rec[11] + rec[12]); break;
        case 28: info[0x19] = _check_no_use(rec[13]); break;
        case 29: info[0x19] = _check_use(rec[13]); break;
        case 30: info[0x19] = _check_no_use(rec[14]); break;
        case 31: info[0x19] = _check_use(rec[14]); break;
        case 32: info[0x19] = _check_no_use(rec[13] + rec[14]); break;
        case 33: info[0x19] = _check_use(rec[13] + rec[14]); break;
        case 34: info[0x19] = _check_no_use(rec[15]); break;
        case 35: info[0x19] = _check_use(rec[15]); break;
        case 36: info[0x19] = _check_no_use(rec[16]); break;
        case 37: info[0x19] = _check_use(rec[16]); break;
        case 38: info[0x19] = _check_no_use(rec[17]); break;
        case 39: info[0x19] = _check_use(rec[17]); break;
        case 40: info[0x19] = _check_no_use(rec[18]); break;
        case 41: info[0x19] = _check_use(rec[18]); break;
        case 42: info[0x19] = _check_no_use(rec[17] + rec[18]); break;
        case 43: info[0x19] = _check_use(rec[17] + rec[18]); break;
        case 44: info[0x19] = _check_no_use(rec[19]); break;
        case 45: info[0x19] = _check_use(rec[19]); break;
        case 46: info[0x19] = _check_no_use(rec[21]); break;
        case 47: info[0x19] = _check_use(rec[21]); break;
        case 48: info[0x19] = _check_no_use(rec[19] + rec[21]); break;
        case 49: info[0x19] = _check_use(rec[19] + rec[21]); break;
        case 50: info[0x19] = _check_no_use(rec[20]); break;
        case 51: info[0x19] = _check_use(rec[20]); break;
        case 52: info[0x19] = _check_no_use(rec[22]); break;
        case 53: info[0x19] = _check_use(rec[22]); break;
        case 54: info[0x19] = _check_no_use(rec[19] + rec[21]); break;
        case 55: info[0x19] = _check_use(rec[19] + rec[21]); break;
        case 56: info[0x19] = _check_no_use(rec[20] + rec[22]); break;
        case 57: info[0x19] = _check_use(rec[20] + rec[22]); break;
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
            break;
    }
    if (rule >= 60 && rule <= 62) {
        if (rule == 62) {
            value = rec[0] + rec[1] + rec[2] + rec[3] + rec[9] + rec[10] +
                    rec[13] + rec[23] + rec[25] + rec[27] + rec[29] + rec[30] +
                    rec[11] + rec[12] + rec[14] + rec[24] + rec[26] + rec[28];
        } else if (rule == 60) {
            value = rec[0] + rec[1] + rec[2] + rec[3] + rec[9] + rec[10] +
                    rec[13] + rec[23] + rec[25] + rec[27];
        } else {
            value = rec[29] + rec[30] + rec[11] + rec[12] + rec[14] +
                    rec[24] + rec[26] + rec[28];
        }
        _check_turn_count_0_end(value);
        return;
    }
    if (rule >= 63 && rule <= 65) {
        if (rule == 65) {
            value = rec[0] + rec[1] + rec[2] + rec[9] + rec[29] + rec[11];
        } else if (rule == 63) {
            value = rec[0] + rec[1] + rec[2] + rec[9];
        } else {
            value = rec[29] + rec[11];
        }
        _check_turn_count_0_end(value);
        return;
    }
    if (rule == 66) {
        value = rec[1] + rec[2] + rec[3] + rec[9] + rec[10] + rec[13] +
                rec[23] + rec[25] + rec[17] + rec[27];
        info[0x19] = value == 0 ? 6 : 1;
        return;
    }
    if (rule == 67) {
        value = rec[0] + rec[2] + rec[3] + rec[9] + rec[10] + rec[13] +
                rec[23] + rec[25] + rec[17] + rec[27];
        info[0x19] = value == 0 ? 6 : 1;
        return;
    }
    if (rule >= 60) {
        info[0x19] = 6;
        return;
    }
    return;
}


int _check_use(u32 param_1) {
    void* work = *(void**)((s32)_battleWorkPointer + 0x2738);

    if ((s32)param_1 >= *(u8*)((s32)work + 0x1A)) {
        return 6;
    }
    return ((s32)param_1 >= *(u8*)((s32)work + 0x1B)) + 1;
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
