#include "battle/battle_detect.h"

void BattleSamplingEnemyUpdate(void) {
    extern void _btlSamplingEnemy(void);

    _btlSamplingEnemy();
}


void _btlSamplingEnemy(void* targetWork) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern void BtlUnit_GetPartsWorldPos(void*, f32*, f32*, f32*);
    s32 unitIdx;
    s32 partIdx;
    s32 count;
    u8* target;
    void* unit;
    void* part;
    void* kindPart;
    u8* entry;
    f32 x;
    f32 y;
    f32 z;
    f32 cursorX;
    f32 cursorY;
    f32 cursorZ;

    count = 0;
    target = (u8*)((s32)targetWork + 4);
    if ((*(u32*)((s32)targetWork + 0xAC0) & 0x80000000) == 0) {
        for (unitIdx = 0; unitIdx < 64; unitIdx++) {
            unit = BattleGetUnitPtr(_battleWorkPointer, unitIdx);
            if (unit == 0 || BtlUnit_CheckStatus(unit, 0x19) != 0 ||
                (*(u32*)((s32)unit + 0x104) & 0x40) != 0) {
                continue;
            }
            part = *(void**)((s32)unit + 0x14);
            for (partIdx = 0; partIdx < *(u8*)(*(s32*)((s32)unit + 0x10) + 0xB4);
                 partIdx++, part = *(void**)part) {
                if (part == 0 || (*(u32*)((s32)part + 0x1AC) & 0x100) != 0) {
                    continue;
                }
                kindPart = *(void**)((s32)part + 4);
                entry = target + count * 0x24;
                BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
                if ((*(u32*)((s32)unit + 0x104) & 4) == 0) {
                    y += *(f32*)((s32)part + 0x28) * *(f32*)((s32)unit + 0x114);
                }
                cursorX = x + (*(f32*)((s32)part + 0x188) + *(f32*)((s32)part + 0x194)) *
                                  *(f32*)((s32)unit + 0x114);
                cursorY = y + (*(f32*)((s32)part + 0x18C) + *(f32*)((s32)part + 0x198)) *
                                  *(f32*)((s32)unit + 0x114);
                cursorZ = z + (*(f32*)((s32)part + 0x190) + *(f32*)((s32)part + 0x19C)) *
                                  *(f32*)((s32)unit + 0x114);
                *(s16*)(entry + 0) = unitIdx;
                *(s16*)(entry + 2) = *(s32*)kindPart;
                *(s16*)(entry + 4) = (s32)cursorX;
                *(s16*)(entry + 6) = (s32)cursorY;
                *(s16*)(entry + 8) = (s32)cursorZ;
                *(s32*)(entry + 0xC) = (s16)x;
                *(s32*)(entry + 0x10) = (s16)y;
                *(s32*)(entry + 0x14) = (s16)(z + 5.0f);
                *(s32*)(entry + 0x18) = *(s16*)((s32)part + 0x1A0);
                *(s8*)(entry + 0x1C) = 0;
                *(u8*)(entry + 0x1D) = z >= -30.0f;
                *(s32*)(entry + 0x20) = -1;
                count++;
            }
        }
    }
    *(u8*)((s32)targetWork + 0xAB7) = 0;
    *(u8*)((s32)targetWork + 0xA6C) = count;
    for (unitIdx = 0; unitIdx < count; unitIdx++) {
        *((s8*)((s32)targetWork + 0xA6D) + unitIdx) = unitIdx;
    }
}

void BattleChoiceSamplingEnemy(void* targetWork, u32 weighting, int* unitIdx, int* partIdx) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    u32 weights[74];
    s8* order;
    u8 count;
    s32 i;
    s32 index;
    s32 bestWeight;
    void* attacker;

    *unitIdx = -1;
    *partIdx = 0;
    count = *(u8*)((s32)targetWork + 0xA6C);
    if (count == 0) {
        return;
    }
    order = (s8*)((s32)targetWork + 0xA6D);
    for (i = 0; i < 74; i++) {
        weights[i] = 0;
    }
    index = order[0];
    attacker = BattleGetUnitPtr(_battleWorkPointer,
                                *(s32*)((s32)targetWork + 0xAB8));
    *unitIdx = *(s16*)((s32)targetWork + 4 + index * 0x24);
    *partIdx = *(s16*)((s32)targetWork + 6 + index * 0x24);
    weights[index] += 10;
    bestWeight = -1;
    for (i = 0; i < count; i++) {
        void* unit;
        index = order[i];
        weights[index] += 100;
        unit = BattleGetUnitPtr(_battleWorkPointer,
                                *(s16*)((s32)targetWork + 4 + index * 0x24));
        if ((weighting & 1) != 0 && *(s32*)((s32)unit + 8) == 0xDE) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((weighting & 2) != 0 && *(s32*)((s32)unit + 8) > 0xDF &&
            *(s32*)((s32)unit + 8) < 0xE7) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((weighting & 4) != 0) {
            weights[index] = (u32)((f32)weights[index] *
                             (0.1f + 0.4f * (f32)(count - i)));
        }
        if ((weighting & 8) != 0) {
            weights[index] = (u32)((0.5f + 0.4f * (f32)i) *
                             (f32)weights[index]);
        }
        if ((weighting & 0x10) != 0 &&
            *(s8*)((s32)attacker + 0xC) == *(s8*)((s32)unit + 0xC)) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((weighting & 0x20) != 0 &&
            *(s8*)((s32)attacker + 0xC) != *(s8*)((s32)unit + 0xC) &&
            *(s8*)((s32)unit + 0xC) != 2) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((weighting & 0x100) != 0) {
            weights[index] = (u32)((2.0f -
                             (f32)*(s16*)((s32)unit + 0x10C) /
                             (f32)*(s16*)((s32)unit + 0x108)) *
                             (f32)weights[index]);
        }
        if ((weighting & 0x200) != 0) {
            weights[index] = (u32)((1.0f +
                             (f32)*(s16*)((s32)unit + 0x10C) /
                             (f32)*(s16*)((s32)unit + 0x108)) *
                             (f32)weights[index]);
        }
        if ((weighting & 0x400) != 0) {
            weights[index] = (u32)((1.5f - 0.05f *
                             (f32)*(s16*)((s32)unit + 0x10C)) *
                             (f32)weights[index]);
        }
        if ((weighting & 0x800) != 0) {
            weights[index] = (u32)((1.0f + 0.05f *
                             (f32)*(s16*)((s32)unit + 0x10C)) *
                             (f32)weights[index]);
        }
        if ((weighting & 0x1000) != 0 &&
            *(s16*)((s32)unit + 0x10C) <=
            *(u8*)(*(s32*)((s32)unit + 0x10) + 0xD)) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((s32)weights[index] > bestWeight) {
            bestWeight = weights[index];
            *unitIdx = *(s16*)((s32)targetWork + 4 + index * 0x24);
            *partIdx = *(s16*)((s32)targetWork + 6 + index * 0x24);
        }
    }
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
