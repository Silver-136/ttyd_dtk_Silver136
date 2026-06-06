#include "battle/battle_detect.h"

void BattleSamplingEnemyUpdate(void) {
    extern void _btlSamplingEnemy(void);

    _btlSamplingEnemy();
}


void _btlSamplingEnemy(void) {
}


void BattleChoiceSamplingEnemy(void* targetWork, u32 targetWeightingFlags, int* unitIdx, int* param_4) {
}


s32 BattleSamplingEnemy(void* targetWork, void* weapon, int attackerIdx, int enemyBelong, u32 targetClassFlags, u32 targetPropertyFlags, u8 param_7) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    s32 changed;
    u32 flags;

    changed = 0;
    *(void**)targetWork = weapon;
    *(s32*)((s32)targetWork + 0xAB8) = attackerIdx;
    *(u8*)((s32)targetWork + 0xABC) = enemyBelong;
    *(u32*)((s32)targetWork + 0xAC0) = targetClassFlags;
    *(u32*)((s32)targetWork + 0xAC4) = targetPropertyFlags;
    *(u8*)((s32)targetWork + 0xAC8) = param_7;

    if ((*(u32*)((s32)BattleGetUnitPtr(_battleWorkPointer, attackerIdx) + 0x27C) & 0x10) != 0) {
        flags = *(u32*)((s32)targetWork + 0xAC0);
        if (((flags & 0x100) != 0) && ((flags & 0x200) == 0)) {
            *(u32*)((s32)targetWork + 0xAC0) = (flags & ~0x100);
            *(u32*)((s32)targetWork + 0xAC0) |= 0x200;
            changed = 1;
        } else if (((flags & 0x100) == 0) && ((flags & 0x200) != 0)) {
            *(u32*)((s32)targetWork + 0xAC0) |= 0x100;
            *(u32*)((s32)targetWork + 0xAC0) &= ~0x200;
            changed = 1;
        }
    }

    ((void (*)(void*))_btlSamplingEnemy)(targetWork);
    return changed;
}

void BattleGetFirstAttackUnit(void* battleWork, void** part, void** unit) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartnerPtr(void*, void*);
    void* foundUnit;
    void* otherUnit;
    s32 i;
    s32 mode;

    otherUnit = NULL;
    for (i = 0; i < 64; i++) {
        foundUnit = BattleGetUnitPtr(battleWork, i);
        if (foundUnit != NULL && *(s8*)((s32)foundUnit + 0xC) == 1) {
            break;
        }
    }

    mode = *(s32*)(*(s32*)((s32)battleWork + 0x2738) + 8);
    switch (mode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            otherUnit = foundUnit;
            foundUnit = BattleGetMarioPtr(battleWork);
            break;
        case 7:
        case 8:
            otherUnit = foundUnit;
            foundUnit = BattleGetPartnerPtr(battleWork, BattleGetMarioPtr(battleWork));
            break;
        default:
            otherUnit = BattleGetMarioPtr(battleWork);
            break;
    }
    *part = foundUnit;
    *unit = otherUnit;
}
