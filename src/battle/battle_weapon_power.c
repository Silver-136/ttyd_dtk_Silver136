#include "battle/battle_weapon_power.h"

extern void* _battleWorkPointer;
s32 weaponGetPowerFromMarioHammerLv(void* attack, void* weapon, void* target);
s32 weaponGetPowerFromMarioJumpLv(void* attack, void* weapon, void* target);
void* BattleGetMarioPtr(void* battleWork);
s32 BattleTransPartyId(s32 id);
s32 pouchGetPartyAttackLv(s32 partyId);
s32 pouchEquipCheckBadge(s32 badgeId);
void* pouchGetPtr(void);

typedef struct BattleWeaponPowerWork {
    u8 pad[0x1CE8];
    s32 acOutputParam[8];
} BattleWeaponPowerWork;

s32 weaponGetFPPowerDefault(void* attack, void* weapon, void* target) {
    return *(s32*)((s32)weapon + 0x44);
}

s32 weaponGetPowerDefault(void* attack, void* weapon, void* target) {
    return *(s32*)((s32)weapon + 0x20);
}

s32 weaponGetPowerHPHalf2(void* attack, void* weapon, void* target) {
    return *(s16*)((s32)target + 0x10C) / 2;
}

s32 weaponGetACOutputParam(void* attack, void* weapon, void* target) {
    BattleWeaponPowerWork* work;
    s32 index;
    s32 add;
    s32 result;

    index = *(s32*)((s32)weapon + 0x20);
    work = (BattleWeaponPowerWork*)_battleWorkPointer;
    add = *(s32*)((s32)weapon + 0x24);

    result = work->acOutputParam[index] + add;
    if (result < 0) {
        result = 0;
    }

    return result;
}

s32 weaponGetFPPowerFPHalf(void* attack, void* weapon, void* target) {
    s32 fp;
    s32 half;

    fp = *(s16*)((s32)target + 0x112);

    if ((fp & 1) == 1) {
        half = fp / 2;
        return half + 1;
    }

    return fp / 2;
}

s32 weaponGetPowerHPHalf(void* attack, void* weapon, void* target) {
    s32 hp;
    s32 half;

    hp = *(s16*)((s32)target + 0x10C);

    if ((hp & 1) == 1) {
        half = hp / 2;
        return half + 1;
    }

    return hp / 2;
}

s32 weaponGetPowerFromGulliblePartyAttackLv(void* attack, void* weapon, void* target) {
    s32 lv;

    lv = BattleTransPartyId(*(s32*)((s32)attack + 0x8));
    lv = pouchGetPartyAttackLv(lv);

    if (lv >= 6) {
        lv = 5;
    }

    return *(s32*)((s32)weapon + 0x20 + lv * 4);
}

s32 weaponGetPowerTatsumaki(void* attack, void* weapon, void* target) {
    s32 power;
    s32 count;

    BattleGetMarioPtr(_battleWorkPointer);

    power = *(s32*)((s32)weapon + 0x20);
    count = pouchEquipCheckBadge(0xF3);

    power += *(s32*)((s32)weapon + 0x24) * count;
    return power;
}

s32 weaponGetPowerFromPartyAttackLv(void* attack, void* weapon, void* target) {
    void* battleWork;
    s32 partyId;
    s32 lv;
    s32 acFlag;
    s32 index;

    battleWork = _battleWorkPointer;
    partyId = BattleTransPartyId(*(s32*)((s32)attack + 0x8));

    acFlag = (*(u32*)((s32)battleWork + 0x1CB8) >> 1) & 1;

    lv = pouchGetPartyAttackLv(partyId);

    index = acFlag + lv * 2;
    if (index >= 6) {
        index = 5;
    }

    return *(s32*)((s32)weapon + 0x20 + index * 4);
}

s32 weaponGetPowerOverlapHammer1(void* attack, void* weapon, void* target) {
    s32 power;
    s32 badgeId;
    s32 add;

    power = weaponGetPowerFromMarioHammerLv(attack, weapon, target);
    add = *(s32*)((s32)weapon + 0x3C);
    badgeId = *(s32*)((s32)weapon + 0x38);

    return power + add * pouchEquipCheckBadge(badgeId);
}

s32 weaponGetPowerOverlapJump1(void* attack, void* weapon, void* target) {
    s32 power;
    s32 badgeId;
    s32 add;

    power = weaponGetPowerFromMarioJumpLv(attack, weapon, target);
    add = *(s32*)((s32)weapon + 0x3C);
    badgeId = *(s32*)((s32)weapon + 0x38);

    return power + add * pouchEquipCheckBadge(badgeId);
}

s32 weaponGetPowerFromMarioHammerLv(void* attack, void* weapon, void* target) {
    void* pouch;
    s32 acFlag;
    s32 lv;
    s32 index;
    u32 add;
    s32 power;

    acFlag = (*(u32*)((s32)_battleWorkPointer + 0x1CB8) >> 1) & 1;

    pouch = pouchGetPtr();
    lv = *(s8*)((s32)pouch + 0x99);

    index = acFlag + (lv - 1) * 2;
    if (index >= 6) {
        index = 5;
    }

    add = *(u8*)((s32)attack + 0x300);
    power = *(s32*)((s32)weapon + 0x20 + index * 4);

    if (add != 0) {
        power += add;
    }

    return power;
}

s32 weaponGetPowerFromMarioJumpLv(void* attack, void* weapon, void* target) {
    void* pouch;
    s32 acFlag;
    s32 lv;
    s32 index;
    u32 add;
    s32 power;

    acFlag = (*(u32*)((s32)_battleWorkPointer + 0x1CB8) >> 1) & 1;

    pouch = pouchGetPtr();
    lv = *(s8*)((s32)pouch + 0x98);

    index = acFlag + (lv - 1) * 2;
    if (index >= 6) {
        index = 5;
    }

    add = *(u8*)((s32)attack + 0x2FF);
    power = *(s32*)((s32)weapon + 0x20 + index * 4);

    if (add != 0) {
        power += add;
    }

    return power;
}
