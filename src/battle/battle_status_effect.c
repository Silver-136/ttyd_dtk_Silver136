#include "battle/battle_unit.h"
#include "battle/battle_status_effect.h"

s8 _st_pri_data[0x19];
extern void* _battleWorkPointer;

void* memset(void* dest, int value, unsigned long size);
void effDelete(void* effect);
void effSoftDelete(void* effect);
void iconDelete(void* icon);

void BattleStatusEffectInit(BattleWorkUnit* unit) {
}

s32 BattleStatusChangeAnnouceMain_Unit(BattleWorkUnit* unit) {
    return 0;
}

void BSE_TurnFirstProcessEffectEntry(BattleWorkUnit* unit, s32 value) {
    *(s16*)((s32)unit + 0x338) = 1;
    *(s16*)((s32)unit + 0x33A) = value;
}

s32 _get_pri(s32 status) {
    s32 target;
    s32 pri;
    s32 value;
    s8* data;

    target = (s8)status;
    pri = 0x18;
    data = _st_pri_data;

    while (1) {
        value = *data;

        if (value == -1) {
            return -1;
        }

        if (value == target) {
            return pri;
        }

        pri--;
        data++;
    }
}

void BSE_BiribiriDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x330);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x330) = 0;
    }
}

void BSE_FireDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x328);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x328) = 0;
    }
}

void BSE_FreezeDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x32C);
    if (effect != 0) {
        effSoftDelete(effect);
        *(void**)((s32)unit + 0x32C) = 0;
    }
}

void BSE_SleepDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x324);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x324) = 0;
    }
}

void BSE_KagegakureDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x334);
    if (effect != 0) {
        iconDelete((void*)((s32)effect + 0x18));
        *(void**)((s32)unit + 0x334) = 0;
    }
}

void BattleStatusChangeInfoWorkInit(BattleWorkUnit* unit) {
    s32 offset;
    s32 count;
    u8* base;
    s8 minusOne;
    u8 zeroA;
    u8 zeroB;
    u8 zeroC;
    s32 zeroWord;
    u8* entry;

    offset = 0;
    count = 6;
    base = (u8*)((s32)unit + 0xAE8);
    minusOne = -1;
    zeroA = 0;
    zeroB = 0;
    zeroC = 0;
    zeroWord = 0;

    while (count != 0) {
        entry = base + offset;
        offset += 0xC;

        *(s8*)(entry + 0x0) = minusOne;
        *(u8*)(entry + 0x1) = zeroA;
        *(u8*)(entry + 0x2) = zeroB;
        *(u8*)(entry + 0x3) = zeroC;
        *(s32*)(entry + 0x4) = zeroWord;

        count--;
    }
}

void BattleStatusEffectDelete(BattleWorkUnit* unit) {
    BSE_SleepDelete(unit);
    BSE_BiribiriDelete(unit);
    BSE_FireDelete(unit);
    BSE_FreezeDelete(unit);
    BSE_KagegakureDelete(unit);
}

void BattleStatusChangeMsgWorkInit(void) {
    u8* work;
    s32 i;

    work = (u8*)((s32)_battleWorkPointer + 0x18DA0);
    memset(work, 0, 0x258);

    for (i = 0; i < 0x1C; i++) {
        *(s8*)(work + i * 0xC) = -1;
    }
}

void BattleStatusChangeMsgSetAnnouce(void* info, s32 index, s32 value) {
    u8* entry;
    s32 kind;

    entry = (u8*)((s32)_battleWorkPointer + 0x18DA0 + index * 0xC);

    *(s32*)(entry + 0x4) = *(s32*)((s32)info + 0x0);
    *(u8*)(entry + 0x0) = index;
    *(u8*)(entry + 0x2) = 0;
    *(s32*)(entry + 0x8) = value;

    kind = *(s32*)((s32)info + 0x8);

    if (kind == 0xDE) {
        *(u8*)(entry + 0x1) |= 1;
        return;
    }

    if (kind >= 0xE0 && kind < 0xE7) {
        if (*(s8*)((s32)info + 0xC) == 0) {
            *(u8*)(entry + 0x1) |= 2;
            return;
        }
    }

    *(u8*)(entry + 0x1) |= 4;
}