#include "battle/battle_detect.h"
#include "battle/battle.h"
#include "system.h"

void _btlSamplingEnemy(void* targetWork) {
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetPartsWorldPos(void*, f32*, f32*, f32*);
    extern s32 BtlCompForwardLv(f64, s32);
    extern void* battleGetUnitMonosiriPtr(s32);

    typedef struct TargetRecordRaw {
        u32 words[9];
    } TargetRecordRaw;

    s32 unitIdx;
    s32 partIdx;
    s32 partCount;
    u8* nextEntry;
    s32 filterIdx;
    s32 propertyIdx;
    s32 count;
    s8 enumerationCount;
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
    void* battleWork;

    battleWork = _battleWorkPointer;
    enumerationCount = 0;
    nextEntry = (u8*)((s32)targetWork + 4);
    if ((*(u32*)((s32)targetWork + 0xAC0) & 0x80000000) != 0) {
        *(s8*)((s32)targetWork + 0xA6C) = -1;
        return;
    }
    {
        for (unitIdx = 0; unitIdx < 64; unitIdx++) {
            unit = BattleGetUnitPtr(battleWork, unitIdx);
            if (unit == 0 || BtlUnit_CheckStatus(unit, 0x1B) != 0 ||
                (*(u32*)((s32)unit + 0x104) & 0x40) != 0) {
                continue;
            }
            part = *(void**)((s32)unit + 0x14);
            partCount = *(u8*)(*(s32*)((s32)unit + 0x10) + 0xB4);
            for (partIdx = 0; partIdx < partCount;
                 partIdx++, part = *(void**)part) {
                if ((*(u32*)((s32)part + 0x1AC) & 0x10000) != 0) {
                    continue;
                }
                kindPart = *(void**)((s32)part + 4);
                entry = nextEntry;
                {
                s32 faceDirection = *(s8*)((s32)part + 0xBD);
                s32 finalX;
                f32 worldX;
                f32 worldY;
                f32 worldZ;
                BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
                if ((*(u32*)((s32)unit + 0x104) & 2) == 0) {
                    y += *(f32*)((s32)part + 0x28) * *(f32*)((s32)unit + 0x114);
                }
                worldX = x;
                worldY = y;
                worldZ = z + 5.0f;
                x = (f32)faceDirection * (*(f32*)((s32)unit + 0x114) *
                    (*(f32*)((s32)part + 0x188) + *(f32*)((s32)part + 0x194))) + worldX;
                y = *(f32*)((s32)unit + 0x114) *
                    (*(f32*)((s32)part + 0x18C) + *(f32*)((s32)part + 0x198)) + y;
                z = *(f32*)((s32)unit + 0x114) *
                    (*(f32*)((s32)part + 0x190) + *(f32*)((s32)part + 0x19C)) + z;
                *(s16*)(entry + 0) = unitIdx;
                *(s16*)(entry + 2) = *(s32*)kindPart;
                *(s16*)(entry + 4) = (s32)x;
                *(s16*)(entry + 6) = (s32)y;
                *(s16*)(entry + 8) = (s32)z;
                *(s32*)(entry + 0x18) = 0;
                *(s32*)(entry + 0x20) = -1;
                if ((*(u32*)((s32)unit + 0x104) & 1) != 0 &&
                    (*(u32*)((s32)part + 0x1AC) & 0x20000) == 0) {
                    *(s32*)(entry + 0x18) += *(s8*)((s32)targetWork + 0xAC8) * 300;
                }
                *(s32*)(entry + 0x18) +=
                    *(s16*)((s32)part + 0x1A0) * faceDirection;
                finalX = (s16)(worldX + (f32)*(s32*)(entry + 0x18));
                *(s32*)(entry + 0xC) = finalX;
                *(s32*)(entry + 0x10) = (s16)worldY;
                *(s32*)(entry + 0x14) = (s16)worldZ;
                *(s8*)(entry + 0x1C) = BtlCompForwardLv((f32)finalX, *(s8*)((s32)targetWork + 0xAC8));
                *(u8*)(entry + 0x1D) = *(s32*)(entry + 0x14) >= -30;
                }
                enumerationCount++;
                nextEntry += 0x24;
            }
        }
    }
    target = (u8*)((s32)targetWork + 4);
    *(u8*)((s32)targetWork + 0xAB7) = 0;
    *(s8*)((s32)targetWork + 0xA6C) = enumerationCount;
    count = *(s8*)((s32)targetWork + 0xA6C);
    for (filterIdx = 0; filterIdx < count; filterIdx++) {
        u8* filterEntry = (u8*)((s32)targetWork + 4 + filterIdx * 0x24);
        s32 filterUnitIdx = *(s16*)(filterEntry + 0);
        void* filterUnit = BattleGetUnitPtr(battleWork, filterUnitIdx);
        void* filterPart = BtlUnit_GetPartsPtr(filterUnit, *(s16*)(filterEntry + 2));
        u32 partAttrs = *(u32*)((s32)filterPart + 0x1AC);
        s32 kind = *(s32*)((s32)filterUnit + 8);
        u32 classFlags = *(u32*)((s32)targetWork + 0xAC0);
        s32 attackerIdx = *(s32*)((s32)targetWork + 0xAB8);
        s8 attackerAlliance = *(s8*)((s32)targetWork + 0xABC);
        s32 valid = 1;

        if ((classFlags & 0x80000000) != 0) valid = 0;
        else if ((classFlags & 0x10000) != 0 && kind != 0xDE) {
            valid = 0;
        }
        else if ((classFlags & 0x20000) != 0 && (kind < 0xD8 || kind > 0xD9)) {
            valid = 0;
        }
        else if ((classFlags & 0x1) != 0 && (kind == 0xDE || kind == 0xDF)) {
            valid = 0;
        }
        else if ((classFlags & 0x2) != 0 && kind >= 0xE0 && kind <= 0xE6) {
            valid = 0;
        }
        else if ((classFlags & 0x10) != 0 && kind >= 1 && kind <= 0xD7) {
            valid = 0;
        }
        else if ((classFlags & 0x40) != 0 && kind >= 0xDA && kind <= 0xDD) {
            valid = 0;
        }
        else if ((classFlags & 0x20) != 0 && kind >= 0xD8 && kind <= 0xD9) {
            valid = 0;
        }
        else if ((classFlags & 0x1000) != 0 && attackerIdx == *(s32*)((s32)filterUnit + 4)) {
            valid = 0;
        }
        else if ((classFlags & 0x100) != 0 &&
            attackerAlliance == *(s8*)((s32)filterUnit + 0xC)) {
            valid = 0;
        }
        else if ((classFlags & 0x200) != 0 &&
            attackerAlliance != *(s8*)((s32)filterUnit + 0xC) &&
            *(s8*)((s32)filterUnit + 0xC) != 2) {
            valid = 0;
        }
        else if ((classFlags & 0x2000) != 0 && attackerIdx != *(s32*)((s32)filterUnit + 4) &&
                 *(s32*)((s32)BattleGetUnitPtr(battleWork, attackerIdx) + 8) == kind) {
            valid = 0;
        }
        else if ((classFlags & 0x4000) != 0 && attackerIdx != *(s32*)((s32)filterUnit + 4)) {
            valid = 0;
        }
        else if ((classFlags & 0x100000) != 0 &&
            !(((partAttrs & 1) != 0 || (partAttrs & 2) != 0) &&
              (partAttrs & 0x40000) == 0)) {
            valid = 0;
        }
        else if ((classFlags & 0x200000) != 0 &&
            !(((partAttrs & 1) != 0 || (partAttrs & 2) != 0 ||
               (partAttrs & 4) != 0) && (partAttrs & 0x10) == 0 &&
              (partAttrs & 0x40000) == 0)) {
            valid = 0;
        }
        if (!valid) {
            s32 moveIdx;
            for (moveIdx = filterIdx; moveIdx < count - 1U; moveIdx++) {
                {
                    u32* dst = (u32*)((s32)targetWork + 4 + moveIdx * 0x24);
                    u32* src = (u32*)((s32)targetWork + 4 + (moveIdx + 1) * 0x24);
                    s32 word;
                    for (word = 0; word < 9; word++) *dst++ = *src++;
                }
            }
            count--;
            filterIdx--;
        }
    }
    for (propertyIdx = 0; propertyIdx < count; propertyIdx++) {
        u8* propertyEntry = (u8*)((s32)targetWork + 4 + propertyIdx * 0x24);
        void* propertyUnit = BattleGetUnitPtr(
            battleWork, *(s16*)(propertyEntry + 0));
        void* propertyPart = BtlUnit_GetPartsPtr(
            propertyUnit, *(s16*)(propertyEntry + 2));
        u32 propertyFlags = *(u32*)((s32)targetWork + 0xAC4);
        u32 unitAttrs = *(u32*)((s32)propertyUnit + 0x104);
        u32 partAttrs = *(u32*)((s32)propertyPart + 0x1AC);
        s32 valid = 1;

        if ((propertyFlags & 0x80000000) != 0) {
            if ((partAttrs & 0x80000) != 0 ||
                battleGetUnitMonosiriPtr(*(s32*)((s32)propertyUnit + 8)) == 0 ||
                *(s32*)((s32)battleGetUnitMonosiriPtr(
                    *(s32*)((s32)propertyUnit + 8)) + 4) == 0) {
                valid = 0;
            }
        }

        if (valid) {
            if ((propertyFlags & 0x2) != 0 && (unitAttrs & 4) != 0) {
                valid = 0;
            }
            if ((propertyFlags & 0x4) != 0 && (unitAttrs & 2) != 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x8) != 0 && (unitAttrs & 4) != 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x10) != 0 &&
                (unitAttrs & (2 | 4)) == 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x1000) != 0 &&
                (partAttrs & 0x100000) != 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x2000) != 0 &&
                (partAttrs & 0x200000) != 0 &&
                ((propertyFlags & 0x100000) == 0 ||
                 *(s32*)((s32)targetWork + 0xAB8) != *(s16*)(propertyEntry + 0))) {
                valid = 0;
            }
            else if ((propertyFlags & 0x4000) != 0 &&
                (partAttrs & 0x400000) != 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x8000) != 0 &&
                (partAttrs & 0x400000) == 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x2000000) != 0 &&
                (unitAttrs & 0x20) != 0) {
                valid = 0;
            }
            else if ((propertyFlags & 0x4000000) != 0 &&
                (partAttrs & 0x2000000) != 0) {
                valid = 0;
            }
        }
        if (!valid) {
            s32 moveIdx;
            for (moveIdx = propertyIdx; moveIdx < count - 1U; moveIdx++) {
                {
                    u32* dst = (u32*)((s32)targetWork + 4 + moveIdx * 0x24);
                    u32* src = (u32*)((s32)targetWork + 4 + (moveIdx + 1) * 0x24);
                    s32 word;
                    for (word = 0; word < 9; word++) *dst++ = *src++;
                }
            }
            count--;
            propertyIdx--;
        }
    }
    if ((*(u32*)((s32)targetWork + 0xAC0) & 0x100000) != 0) {
        for (filterIdx = 0; filterIdx < count; filterIdx++) {
            u8* currentEntry =
                (u8*)((s32)targetWork + 4 + filterIdx * 0x24);
            void* currentUnit = BattleGetUnitPtr(
                battleWork, *(s16*)(currentEntry + 0));
            void* currentPart = BtlUnit_GetPartsPtr(
                currentUnit, *(s16*)(currentEntry + 2));
            s32 valid = 1;

            if ((*(u32*)((s32)currentPart + 0x1AC) & 2) != 0) {
                for (propertyIdx = 0; propertyIdx < count; propertyIdx++) {
                    u8* otherEntry =
                        (u8*)((s32)targetWork + 4 + propertyIdx * 0x24);

                    if (currentEntry != otherEntry &&
                        *(s16*)(currentEntry + 0) ==
                            *(s16*)(otherEntry + 0)) {
                        void* otherUnit = BattleGetUnitPtr(
                            battleWork, *(s16*)(otherEntry + 0));
                        void* otherPart = BtlUnit_GetPartsPtr(
                            otherUnit, *(s16*)(otherEntry + 2));

                        if ((*(u32*)((s32)otherPart + 0x1AC) & 1) != 0) {
                            valid = 0;
                            break;
                        }
                    }
                }
            }

            if (!valid) {
                s32 moveIdx;
                for (moveIdx = filterIdx; moveIdx < count - 1U; moveIdx++) {
                    {
                    u32* dst = (u32*)((s32)targetWork + 4 +
                                         moveIdx * 0x24);
                    u32* src = (u32*)((s32)targetWork + 4 +
                                             (moveIdx + 1) * 0x24);
                    s32 word;
                    for (word = 0; word < 9; word++) *dst++ = *src++;
                }
                }
                count--;
                filterIdx--;
            }
        }
    }

    if ((*(u32*)((s32)targetWork + 0xAC4) & 0x1000000) != 0) {
        for (filterIdx = 0; filterIdx < count; filterIdx++) {
            u8* currentEntry = target + filterIdx * 0x24;
            void* currentUnit = BattleGetUnitPtr(
                battleWork, *(s16*)(currentEntry + 0));
            void* currentPart = BtlUnit_GetPartsPtr(
                currentUnit, *(s16*)(currentEntry + 2));
            s32 valid = 1;

            if ((*(u32*)((s32)currentPart + 0x1AC) & 0x40) == 0) {
                for (propertyIdx = 0; propertyIdx < count; propertyIdx++) {
                    u8* otherEntry = target + propertyIdx * 0x24;
                    s32 currentX;
                    s32 otherX;

                    if (currentEntry == otherEntry ||
                        *(u8*)(currentEntry + 0x1D) !=
                            *(u8*)(otherEntry + 0x1D)) {
                        continue;
                    }
                    currentX = *(s32*)(currentEntry + 0xC);
                    otherX = *(s32*)(otherEntry + 0xC);
                    if (*(s8*)((s32)targetWork + 0xAC8) < 0) {
                        currentX = -currentX;
                        otherX = -otherX;
                    }
                    if (otherX < currentX) {
                        void* otherPart = BattleGetUnitPartsPtr(
                            *(s16*)(otherEntry + 0),
                            *(s16*)(otherEntry + 2));
                        if ((*(u32*)((s32)otherPart + 0x1AC) & 0x40) == 0 &&
                            (*(u32*)((s32)otherPart + 0x1AC) & 0x400000) == 0) {
                            void* attacker = BattleGetUnitPtr(
                                battleWork,
                                *(s32*)((s32)targetWork + 0xAB8));
                            void* otherUnit = BattleGetUnitPtr(
                                battleWork, *(s16*)(otherEntry + 0));
                            if ((*(u32*)((s32)attacker + 0x27C) & 0x10) == 0 ||
                                attacker != otherUnit) {
                                valid = 0;
                                break;
                            }
                        }
                    }
                }
            }
            if (!valid) {
                s32 moveIdx;
                for (moveIdx = filterIdx; moveIdx < count - 1U; moveIdx++) {
                    {
                    u32* dst = (u32*)(target + moveIdx * 0x24);
                    u32* src = (u32*)(target + (moveIdx + 1) * 0x24);
                    s32 word;
                    for (word = 0; word < 9; word++) *dst++ = *src++;
                }
                }
                count--;
                filterIdx--;
            }
        }
    }

    *(u8*)((s32)targetWork + 0xAB7) = 0;
    *(u8*)((s32)targetWork + 0xA6C) = count;
    for (unitIdx = 0; unitIdx < count; unitIdx++) {
        *((s8*)((s32)targetWork + 0xA6D) + unitIdx) = unitIdx;
    }

    {
        s8* targetIndices = (s8*)((s32)targetWork + 0xA6D);
        s32 sortIdx;
        s32 compareIdx;

        if (*(s8*)((s32)targetWork + 0xAC8) < 1) {
            for (sortIdx = 0; sortIdx < count - 1U; sortIdx++) {
                for (compareIdx = sortIdx + 1;
                     compareIdx < count;
                     compareIdx++) {
                    s8 leftIndex = targetIndices[sortIdx];
                    s8 rightIndex = targetIndices[compareIdx];
                    u8* leftEntry = target + leftIndex * 0x24;
                    u8* rightEntry = target + rightIndex * 0x24;
                    s32 leftKey =
                        *(s16*)(leftEntry + 4) +
                        *(s32*)(leftEntry + 0x18) * 10;
                    s32 rightKey =
                        *(s16*)(rightEntry + 4) +
                        *(s32*)(rightEntry + 0x18) * 10;

                    if (leftKey <= rightKey) {
                        targetIndices[sortIdx] = rightIndex;
                        targetIndices[compareIdx] = leftIndex;
                    }
                }
            }
        } else {
            for (sortIdx = 0; sortIdx < count - 1U; sortIdx++) {
                for (compareIdx = sortIdx + 1;
                     compareIdx < count;
                     compareIdx++) {
                    s8 leftIndex = targetIndices[sortIdx];
                    s8 rightIndex = targetIndices[compareIdx];
                    u8* leftEntry = target + leftIndex * 0x24;
                    u8* rightEntry = target + rightIndex * 0x24;
                    s32 leftKey =
                        *(s16*)(leftEntry + 4) +
                        *(s32*)(leftEntry + 0x18) * 10;
                    s32 rightKey =
                        *(s16*)(rightEntry + 4) +
                        *(s32*)(rightEntry + 0x18) * 10;

                    if (rightKey < leftKey) {
                        targetIndices[sortIdx] = rightIndex;
                        targetIndices[compareIdx] = leftIndex;
                    }
                }
            }
        }
    }
}

s32 BattleSamplingEnemy(void* targetWork, void* weapon, int attackerIdx, int enemyBelong, u32 targetClassFlags, u32 targetPropertyFlags, u8 param_7) {
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

void BattleSamplingEnemyUpdate(void) {
    extern void _btlSamplingEnemy(void);

    _btlSamplingEnemy();
}

void BattleChoiceSamplingEnemy(void* targetWork, u32 weighting, int* unitIdx, int* partIdx) {
    extern s32 BtlUnit_GetMaxFp(void*);
    extern s32 BtlUnit_GetFp(void*);
    extern u8 itemDataTable[];
    typedef struct TargetInfo { s16 unitIndex; s16 partIndex; u8 rest[32]; } TargetInfo;
    typedef struct Targets { void* weapon; TargetInfo info[74]; s8 count; s8 order[74]; u8 flags; s32 attacker; } Targets;
    Targets* tw = (Targets*)targetWork;
    s32 weights[74];

    s32 i;
    s32 index;
    s32 itemId;
    s32 maxWeight;
    s32 selected;
    s32 total;
    void* battleWork;
    void* attacker;
    void* unit;
    void* weapon;

    battleWork = _battleWorkPointer;
    *unitIdx = -1;
    *partIdx = 0;
    if (tw->count > 0) {

    attacker = BattleGetUnitPtr(battleWork, tw->attacker);
    *unitIdx = tw->info[tw->order[0]].unitIndex;
    *partIdx = tw->info[tw->order[0]].partIndex;

    for (i = 0; i < 74; i++) {
        weights[i] = 0;
    }
    weights[tw->order[0]] += 10;

    for (i = 0; i < tw->count; i++) {
        index = tw->order[i];
        unit = BattleGetUnitPtr(battleWork,
                                tw->info[index].unitIndex);
        weights[index] += 100;

        if ((weighting & 1) && *(s32*)((s32)unit + 8) == 0xDE) {
            weights[index] = (s32)(1.5f * (f32)weights[index]);
        }
        if ((weighting & 2) && *(s32*)((s32)unit + 8) >= 0xE0 &&
            *(s32*)((s32)unit + 8) < 0xE7) {
            weights[index] = (s32)(1.5f * (f32)weights[index]);
        }
        if (weighting & 4) {
            weights[index] = (s32)((f32)weights[index] *
                (0.1f + 0.4f * (f32)(tw->count - i)));
        }
        if (weighting & 8) {
            weights[index] = (s32)((0.5f + 0.4f * (f32)i) *
                (f32)weights[index]);
        }
        if ((weighting & 0x10) &&
            *(s8*)((s32)attacker + 0xC) == *(s8*)((s32)unit + 0xC)) {
            weights[index] = (s32)(1.5f * (f32)weights[index]);
        }
        if ((weighting & 0x20) &&
            *(s8*)((s32)attacker + 0xC) != *(s8*)((s32)unit + 0xC) &&
            *(s8*)((s32)unit + 0xC) != 2) {
            weights[index] = (s32)(1.5f * (f32)weights[index]);
        }
        if (weighting & 0x100) {
            weights[index] = (s32)((2.0f -
                (f32)*(s16*)((s32)unit + 0x10C) /
                (f32)*(s16*)((s32)unit + 0x108)) * (f32)weights[index]);
        }
        if (weighting & 0x200) {
            weights[index] = (s32)((1.0f +
                (f32)*(s16*)((s32)unit + 0x10C) /
                (f32)*(s16*)((s32)unit + 0x108)) * (f32)weights[index]);
        }
        if (weighting & 0x400) {
            weights[index] = (s32)((-(0.05f *
                (f32)*(s16*)((s32)unit + 0x10C) - 1.5f)) * (f32)weights[index]);
        }
        if (weighting & 0x800) {
            weights[index] = (s32)((1.0f + 0.05f *
                (f32)*(s16*)((s32)unit + 0x10C)) * (f32)weights[index]);
        }
        if ((weighting & 0x1000) &&
            *(s16*)((s32)unit + 0x10C) <=
            *(u8*)(*(s32*)((s32)unit + 0x10) + 0xD)) {
            weights[index] = (s32)(1.5f * (f32)weights[index]);
        }

        weapon = tw->weapon;
        if (weapon != 0) {
            if (*(u32*)((s32)unit + 0x104) & 0x20000) {
                weights[index] = 1;
            }
            itemId = *(s32*)((s32)weapon + 8);
            switch (itemId) {
            case 0: break;
            case 0x8E: {
                if (BtlUnit_CanActStatus(unit) == 0 ||
                    BtlUnit_CheckStatus(unit, 0xA) != 0) {
                    weights[index] = 1;
                }
            } break;
            case 0x87: {
                if (BtlUnit_CheckStatus(unit, 6)) weights[index] = 1;
            } break;
            case 0x86: {
                if (BtlUnit_CheckStatus(unit, 0x12)) weights[index] = 1;
            } break;
            case 0x89: {
                if (BtlUnit_CheckStatus(unit, 5)) weights[index] = 1;
            } break;
            case 0x88: {
                if (BtlUnit_CheckStatus(unit, 7)) weights[index] = 1;
            } break;
            case 0x8F: {
                if (BtlUnit_CheckStatus(unit, 0xE)) weights[index] = 1;
            } break;
            case 0x92: {
                if (BtlUnit_CheckStatus(unit, 0xB)) weights[index] = 1;
            } break;
            case 0x93: {
                if (BtlUnit_CheckStatus(unit, 0xF)) weights[index] = 1;
            } break;
            case 0x99: {
                if (BtlUnit_CheckStatus(unit, 1) == 0 &&
                    BtlUnit_CheckStatus(unit, 2) == 0 &&
                    BtlUnit_CheckStatus(unit, 3) == 0 &&
                    BtlUnit_CheckStatus(unit, 4) == 0 &&
                    BtlUnit_CheckStatus(unit, 5) == 0 &&
                    BtlUnit_CheckStatus(unit, 8) == 0 &&
                    BtlUnit_CheckStatus(unit, 9) == 0 &&
                    BtlUnit_CheckStatus(unit, 0xB) == 0 &&
                    BtlUnit_CheckStatus(unit, 0xF) == 0 &&
                    BtlUnit_CheckStatus(unit, 0x14) == 0) {
                    weights[index] = 1;
                }
            } break;
            default: if (*(s8*)(itemDataTable + itemId * 0x28 + 0x1E) != 0) {
                s32 maxFp = BtlUnit_GetMaxFp(unit);
                s32 fp = BtlUnit_GetFp(unit);
                if (fp < maxFp) {
                    weights[index] = (s32)((0.1f * (f32)(BtlUnit_GetMaxFp(unit) - BtlUnit_GetFp(unit)) + 1.0f) *
                                           (f32)weights[index]);
                } else {
                    itemId = *(s32*)((s32)tw->weapon + 8);
                    if (*(s8*)(itemDataTable + itemId * 0x28 + 0x1D) == 0 &&
                        *(s8*)(itemDataTable + itemId * 0x28 + 0x1F) == 0) {
                        weights[index] = 1;
                    }
                }
            }
            }
        }
    }

    if ((weighting & 0x2000) == 0) {
        selected = 0;
        maxWeight = 0;
        for (i = 0; i < 74; i++) {
            if (maxWeight < (s32)weights[i]) {
                selected = i;
                maxWeight = weights[i];
            }
        }
    } else {
        total = 0;
        for (i = 0; i < 74; i++) {
            total += weights[i];
        }
        total = irand(total);
        selected = 0;
        for (i = 0; i < 74; i++) {
            total -= weights[i];
            if (total < 1) {
                selected = i;
                break;
            }
        }
    }
    *unitIdx = tw->info[selected].unitIndex;
    *partIdx = tw->info[selected].partIndex;
    }
}

void BattleGetFirstAttackUnit(void* battleWork, void** part, void** unit) {
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartnerPtr(void*, void*);
    void* foundUnit;
    void* otherUnit;
    s32 i;
    s32 mode;
    otherUnit = NULL;
    for (i = 0; i < 64; i++) {
        foundUnit = BattleGetUnitPtr(battleWork, i);
        if (foundUnit != NULL && *(s8*)((s32)foundUnit + 0xC) == 1) break;
    }
    mode = *(s32*)(*(s32*)((s32)battleWork + 0x2738) + 8);
    switch (mode) {
        case 1: case 2: case 3: case 4: case 5: case 6: {
            void* next = BattleGetMarioPtr(battleWork);
            otherUnit = foundUnit;
            foundUnit = next;
            break;
        }
        case 7: case 8: {
            void* next = BattleGetPartnerPtr(battleWork, BattleGetMarioPtr(battleWork));
            otherUnit = foundUnit;
            foundUnit = next;
            break;
        }
        case 9:
        default:
            otherUnit = BattleGetMarioPtr(battleWork);
            break;
    }
    *part = foundUnit;
    *unit = otherUnit;
}
