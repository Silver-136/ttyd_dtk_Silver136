#include "battle/battle_enemy_item.h"

extern u8 itemDataTable[];
extern void* _battleWorkPointer;

s32 BtlUnit_GetEnemyBelong(void* unit);
void BattleSamplingEnemy(void* work, void* weapon, s32 unitId, s32 belong, s32 a, s32 b, s32 c);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 BtlUnit_CanActStatus(void* unit);
s32 BtlUnit_GetMaxFp(void* unit);
s32 BtlUnit_GetFp(void* unit);
s32 irand(s32 range);

s32 _check_status_recover_item(void* param_1) {
    return 0;
}


s32 _check_status_attack_item(void* param_1) {
    return 0;
}


s32 _check_status_support_item(void* param_1) {
    return 0;
}

s32 _check_attack_item(void* unit) {
    s32 itemId = *(s32*)((s32)unit + 0x308);
    u8* item;
    void* battleWork;
    void* sampleWork;
    void* weapon;
    s32 i;

    battleWork = _battleWorkPointer;
    sampleWork = (void*)((s32)battleWork + 0x428);
    item = itemDataTable + itemId * 0x28;
    weapon = *(void**)(item += 0x24);

    BattleSamplingEnemy(sampleWork, weapon, *(s32*)unit, (s8)BtlUnit_GetEnemyBelong(unit), *(s32*)((s32)weapon + 0x64),
                        *(s32*)((s32)weapon + 0x68),
                        *(s8*)((s32)battleWork + ((s8)*(u8*)((s32)unit + 0xC)) * 8 + 0xA));

    if (*(s8*)((s32)sampleWork + 0xA6C) <= 0) {
        return 0;
    }

    if (itemId == 0x8B || itemId == 0x85) {
        for (i = 0; i < *(s8*)((s32)sampleWork + 0xA6C); i++) {
            s32 offset = i + 0xA6D;
            void* target =
                BattleGetUnitPtr(battleWork, *(s16*)((s32)sampleWork + (*(s8*)((s32)sampleWork + offset)) * 0x24 + 4));
            if (*(s32*)((s32)target + 8) == 0xE1 && !(*(s32*)((s32)unit + 0x104) & 0x20000) &&
                BtlUnit_CanActStatus(target) == 0) {
                return *(s32*)((s32)*(void**)item + 0xB0);
            }
        }
    }

    if (irand(100) >= *(s16*)battleWork * 20) {
        return 0;
    }
    return *(s32*)((s32)*(void**)item + 0xB0);
}


s32 _check_fp_recover_item(void* unit) {
    s32 itemId = *(s32*)((s32)unit + 0x308);
    u8* item;
    void* battleWork;
    void* sampleWork;
    void* weapon;
    void* group;
    s32 i;

    battleWork = _battleWorkPointer;
    sampleWork = (void*)((s32)battleWork + 0x428);
    item = itemDataTable;
    item += itemId * 0x28;
    weapon = *(void**)(item += 0x24);
    group = (void*)((s32)battleWork + ((s8)*(u8*)((s32)unit + 0xC)) * 8);

    BattleSamplingEnemy(sampleWork, weapon, *(s32*)unit, (s8)BtlUnit_GetEnemyBelong(unit), *(s32*)((s32)weapon + 0x64),
                        *(s32*)((s32)weapon + 0x68), *(s8*)((s32)group + 0xA));

    for (i = 0; i < *(s8*)((s32)sampleWork + 0xA6C); i++) {
        s32 offset = i + 0xA6D;
        void* target =
            BattleGetUnitPtr(battleWork, *(s16*)((s32)sampleWork + (*(s8*)((s32)sampleWork + offset)) * 0x24 + 4));
        if (!(*(s32*)((s32)unit + 0x104) & 0x20000)) {
            s32 max = BtlUnit_GetMaxFp(target);
            if (BtlUnit_GetFp(target) < max) {
                return *(s32*)((s32)*(void**)item + 0xB0);
            }
        }
    }
    return 0;
}


s32 _check_hp_recover_item(void* unit) {
    s32 itemId = *(s32*)((s32)unit + 0x308);
    u8* item;
    void* battleWork;
    void* sampleWork;
    void* weapon;
    s32 i;

    battleWork = _battleWorkPointer;
    sampleWork = (void*)((s32)battleWork + 0x428);
    item = itemDataTable + itemId * 0x28;
    weapon = *(void**)(item += 0x24);

    BattleSamplingEnemy(sampleWork, weapon, *(s32*)unit, (s8)BtlUnit_GetEnemyBelong(unit), *(s32*)((s32)weapon + 0x64),
                        *(s32*)((s32)weapon + 0x68),
                        *(s8*)((s32)battleWork + ((s8)*(u8*)((s32)unit + 0xC)) * 8 + 0xA));

    for (i = 0; i < *(s8*)((s32)sampleWork + 0xA6C); i++) {
        s32 offset = i + 0xA6D;
        void* target =
            BattleGetUnitPtr(battleWork, *(s16*)((s32)sampleWork + (*(s8*)((s32)sampleWork + offset)) * 0x24 + 4));
        if (!(*(s32*)((s32)unit + 0x104) & 0x20000) && *(s16*)((s32)target + 0x10C) < *(s16*)((s32)target + 0x108)) {
            return *(s32*)((s32)*(void**)item + 0xB0);
        }
    }
    return 0;
}


s32 BattleEnemyUseItemCheck(void* unit) {
    s32 itemId = *(s32*)((s32)unit + 0x308);
    s32 itemOffset;
    void* weapon;
    s32 result;

    if (itemId == 0) {
        return 0;
    }

    itemOffset = itemId * 0x28;
    weapon = *(void**)(itemDataTable + itemOffset + 0x24);
    if (weapon == NULL) {
        return 0;
    }

    if ((*(s32*)((s32)unit + 0x27C) & 0x10) && !(*(s32*)((s32)weapon + 0x74) & 0x10000)) {
        return 0;
    }

    switch (itemId) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x8B:
        case 0x90:
            return _check_attack_item(unit);
        case 0x89:
        case 0x8A:
        case 0x8C:
        case 0x8D:
        case 0x92:
        case 0x93:
            return _check_status_attack_item(unit);
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x8E:
        case 0x8F:
            return _check_status_support_item(unit);
        case 0x99:
            return _check_status_recover_item(unit);
        case 0x9D:
            return _check_hp_recover_item(unit);
        case 0x9E:
            return _check_fp_recover_item(unit);
    }

    if (*(u8*)(itemDataTable + itemOffset + 0x1D) != 0) {
        result = _check_hp_recover_item(unit);
        if (result != 0) {
            return result;
        }
    }

    if (*(u8*)(itemDataTable + itemOffset + 0x1E) != 0) {
        result = _check_fp_recover_item(unit);
        if (result != 0) {
            return result;
        }
    }

    return 0;
}
