#include "battle/battle_stage_object.h"

extern void* _battleWorkPointer;
void BattleAudience_Case_HaikeiSet(void);
s32 strcmp(const char*, const char*);

void BattleObjectConfig(void) {
}

void _rate_mix(s8* maxValue, s8* total, s8* current, s8* add) {
    s8 value = *current;

    if (value <= 0) {
        return;
    }
    if (value > *maxValue) {
        *maxValue = value;
    }
    *total += *add;
}

s32 _bgset_audience_bgset_access(void) {
    BattleAudience_Case_HaikeiSet();
    return 2;
}

void* BattleGetObjectPtr(s32 id) {
    s32 offset;
    void* battleWork;
    s32 i;
    void* obj;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        s32 addr = offset + 0x10000;
        obj = (void*)((s32)battleWork + addr + 0x715C);
        if (*(s32*)obj == id) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}

void* BattleSearchObjectPtr(const char* name) {
    s32 i;
    s32 offset;
    void* battleWork;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        s32 addr = offset + 0x10000;
        void* obj = (void*)((s32)battleWork + addr + 0x715C);
        if (*(s32*)obj != 0 && strcmp(**(const char***)((s32)obj + 0x64), name) == 0) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}
