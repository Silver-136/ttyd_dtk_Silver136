#include "battle/battle_damage.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
extern s32 irand(s32 max);
extern f64 double_to_int_802ee0a8;
extern f32 float_0p5_804221c4;
#define WEAPON_ACCURACY(weapon) (*(u8*)((s32)(weapon) + 0x10))
#define WEAPON_SPECIAL_PROPERTY_FLAGS(weapon) (*(u32*)((s32)(weapon) + 0x74))
#define WEAPON_COUNTER_RESISTANCE_FLAGS(weapon) (*(u32*)((s32)(weapon) + 0x78))

#define UNIT_ATTRIBUTE_FLAGS(unit) (*(u32*)((s32)(unit) + 0x104))
#define UNIT_CURRENT_HP(unit) (*(s16*)((s32)(unit) + 0x10C))
#define UNIT_KIND_PARAMS(unit) (*(void**)((s32)(unit) + 0x10))
#define UNIT_TOKEN_FLAGS(unit) (*(u32*)((s32)(unit) + 0x27C))

#define UNIT_BADGE_CLOSE_CALL(unit) (*(u8*)((s32)(unit) + 0x2E0))
#define UNIT_BADGE_PRETTY_LUCKY(unit) (*(u8*)((s32)(unit) + 0x2E1))
#define UNIT_BADGE_LUCKY_DAY(unit) (*(u8*)((s32)(unit) + 0x2E2))
#define UNIT_BADGE_SPIKE_SHIELD(unit) (*(u8*)((s32)(unit) + 0x302))

#define UNIT_DANGER_HP(unit) (*(u8*)((s32)UNIT_KIND_PARAMS(unit) + 0x0C))

#define PART_ATTRIBUTE_FLAGS(part) (*(u32*)((s32)(part) + 0x1AC))
#define PART_COUNTER_ATTRIBUTE_FLAGS(part) (*(u32*)((s32)(part) + 0x1B0))

#define BATTLE_FOG_ACTIVE(battleWork) (*(u32*)((s32)(battleWork) + 0x182BC))

#define ATTACK_SP_IS_CHARGEABLE 0x4
#define ATTACK_SP_CANNOT_MISS 0x8
#define ATTACK_SP_CONTACTS_CLONES 0x40000

#define UNIT_TOKEN_CHARGE_EXPENDED 0x8

#define UNIT_ATTR_PERMANENTLY_INVISIBLE 0x8
#define UNIT_ATTR_VEILED 0x10

#define PART_ATTR_HOLE_OR_MISS 0x40
#define PART_ATTR_CLONE 0x4000

#define PART_COUNTER_PREEMPTIVE_FRONT_SPIKY 0x2
#define WEAPON_RESIST_PREEMPTIVE_FRONT_SPIKY 0x4

#define STATUS_SLEEP 1
#define STATUS_STOP 2
#define STATUS_DIZZY 3
#define STATUS_DODGY 7
#define STATUS_INVISIBLE 18

typedef struct BattleWeaponPreCheck {
    u8 pad_00[0x10];
    u8 mAccuracy;                    // 0x10
    u8 pad_11[0x63];
    u32 mSpecialPropertyFlags;       // 0x74
    u32 mCounterResistanceFlags;     // 0x78
} BattleWeaponPreCheck;

void BattleInitCounterPreCheckWork(void* work) {
    extern void* memset(void* ptr, s32 value, u32 size);

    memset(work, 0, 0x30);
    *(s32*)work = 1;
}

s32 BattlePreCheckDamage(void* unit1, void* unit2, void* part, void* weapon, u32 flags) {
    void* battleWork;
    void* attacker;
    void* target;
    void* targetPart;
    void* attackWeapon;
    u32 count;
    s32 rand;
    s32 accuracy;
    s32 status;
    u8 prettyLuckyCount;
    battleWork = _battleWorkPointer;
    attacker = unit1;
    target = unit2;
    targetPart = part;
    attackWeapon = weapon;

    if (target == 0 || targetPart == 0) {
        return 1;
    }

    if ((WEAPON_SPECIAL_PROPERTY_FLAGS(attackWeapon) & ATTACK_SP_IS_CHARGEABLE) != 0) {
        UNIT_TOKEN_FLAGS(attacker) |= UNIT_TOKEN_CHARGE_EXPENDED;
    }

    if ((PART_ATTRIBUTE_FLAGS(targetPart) & PART_ATTR_CLONE) != 0 &&
        (WEAPON_SPECIAL_PROPERTY_FLAGS(attackWeapon) & ATTACK_SP_CONTACTS_CLONES) == 0) {
        return 2;
    }

    if ((WEAPON_SPECIAL_PROPERTY_FLAGS(attackWeapon) & ATTACK_SP_CANNOT_MISS) != 0) {
        return 1;
    }

    status = BtlUnit_CheckStatus(target, STATUS_INVISIBLE);
    if (status != 0 || (UNIT_ATTRIBUTE_FLAGS(target) & UNIT_ATTR_PERMANENTLY_INVISIBLE) != 0) {
        return 4;
    }

    if ((UNIT_ATTRIBUTE_FLAGS(target) & UNIT_ATTR_VEILED) != 0) {
        return 4;
    }

    if ((PART_COUNTER_ATTRIBUTE_FLAGS(targetPart) & PART_COUNTER_PREEMPTIVE_FRONT_SPIKY) != 0 &&
        (WEAPON_COUNTER_RESISTANCE_FLAGS(attackWeapon) & WEAPON_RESIST_PREEMPTIVE_FRONT_SPIKY) == 0 &&
        UNIT_BADGE_SPIKE_SHIELD(attacker) == 0) {
        return 5;
    }

    if ((PART_ATTRIBUTE_FLAGS(targetPart) & PART_ATTR_HOLE_OR_MISS) != 0) {
        return 4;
    }

    if ((flags & 0x100000) != 0) {
        return 1;
    }

    prettyLuckyCount = UNIT_BADGE_PRETTY_LUCKY(target);
    if (prettyLuckyCount != 0) {
        count = prettyLuckyCount;
        for (; (s32)count > 0; count--) {
            rand = irand(100);
            if (rand >= 0x5A) {
                return 3;
            }
        }
    }

    count = UNIT_BADGE_LUCKY_DAY(target);
    if (count != 0) {
        for (; (s32)count > 0; count--) {
            rand = irand(100);
            if (rand >= 0x4B) {
                return 3;
            }
        }
    }

    if (UNIT_CURRENT_HP(target) <= UNIT_DANGER_HP(target)) {
        count = UNIT_BADGE_CLOSE_CALL(target);
        if (count != 0) {
            for (; (s32)count > 0; count--) {
                rand = irand(100);
                if (rand >= 0x43) {
                    return 3;
                }
            }
        }
    }

    status = BtlUnit_CheckStatus(target, STATUS_DODGY);
    if (status != 0) {
        if (BtlUnit_CheckStatus(target, STATUS_SLEEP) == 0 &&
            BtlUnit_CheckStatus(target, STATUS_STOP) == 0) {
            rand = irand(100);
            if (rand >= 0x32) {
                return 6;
            }
        }
    }

    accuracy = WEAPON_ACCURACY(attackWeapon);

    status = BtlUnit_CheckStatus(attacker, STATUS_DIZZY);
    if (status != 0) {
        accuracy = (accuracy * 0x32) / 100;
    }

    if (BATTLE_FOG_ACTIVE(battleWork) != 0) {
        accuracy = (s32)((f32)accuracy * float_0p5_804221c4);
    }

    // AUTOTEST_BEGIN final_accuracy_branch
rand = irand(100);
if (rand >= accuracy) {
    return 2;
}

return 1;
// AUTOTEST_END
}


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCheckDamage size 0xA00 */
s32 BattleCheckDamage(BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32 flags) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleDamageDirect size 0xB24 */
void BattleDamageDirect(s32 unk, BattleWorkUnit* unit, BattleWorkUnitPart* part, s32 damage, s32 zero, s32 flags, s32 arg5, s32 arg6) {
    ;
}

/* AUTOSTUB BattleCheckCounter size 0x5A4 */
s32 BattleCheckCounter(void* work, BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32* result) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleAttackDeclareAll size 0x118 */
/* signature source: prototype_search */
void BattleAttackDeclareAll(BattleWork* battleWork) {
    ;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _checkDamageCode_EmergencyRevival size 0xC0 */
/* signature source: manual_signatures */
s32 _checkDamageCode_EmergencyRevival(int param_1, u32* param_2) {
    extern s32 pouchCheckItem(s32 item);
    s32 kind;

    kind = *(s32*)((s32)param_1 + 8);
    if (kind == UNIT_MARIO) {
        if (pouchCheckItem(0x97) > 0) {
            *param_2 |= 0x800;
            return 1;
        }
        return 0;
    }
    if (kind >= UNIT_GOOMBELLA && kind < UNIT_MAX) {
        if (pouchCheckItem(0x97) > 0) {
            *param_2 |= 0x800;
            return 1;
        }
        return 0;
    }
    if (*(s32*)((s32)param_1 + 0x308) == 0x97) {
        *param_2 |= 0x800;
        return 1;
    }
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _getRegistStatus size 0xDC */
/* signature source: manual_signatures */
u32 _getRegistStatus(BattleWorkUnit* unit, StatusEffectType statusType) {
    u8* rates = *(u8**)((s32)unit + 0x144);

    switch (statusType) {
        case 0:
            return rates[0];
        case 1:
            return rates[1];
        case 2:
            return rates[2];
        case 3:
            return rates[3];
        case 4:
            return rates[4];
        case 5:
            return rates[5];
        case 6:
            return rates[6];
        case 7:
            return rates[7];
        case 8:
            return rates[8];
        case 9:
            return rates[9];
        case 10:
            return rates[10];
        case 11:
            return rates[11];
        case 12:
            return rates[12];
        case 13:
            return rates[13];
        case 14:
            return rates[14];
        case 15:
            return rates[21];
        case 16:
            return rates[17];
        case 17:
            return rates[18];
        case 18:
            return rates[15];
        case 19:
            return rates[16];
        case 20:
            return rates[19];
        case 21:
            return rates[20];
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        default:
            return 100;
    }
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _getSickStatusRate size 0x1F0 */
/* signature source: manual_signatures */
u32 _getSickStatusRate(void* weapon, StatusEffectType statusType) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCheckPikkyoro size 0x1A8 */
/* signature source: manual_signatures */
u32 BattleCheckPikkyoro(void* weapon, u32* param_2) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCalculateFpDamage size 0x110 */
/* signature source: manual_signatures */
int BattleCalculateFpDamage(BattleWorkUnit* unit1, BattleWorkUnit* unit2, BattleWorkUnitPart* part, void* weapon, u32* param_5, u32 param_6) {
    s32 (*callback)(BattleWorkUnit*, void*, BattleWorkUnit*, BattleWorkUnitPart*);
    BattleWork* battleWork;
    BattleWorkUnit* target;
    BattleWorkUnitPart* targetPart;
    s32 damage;
    s32 sub;
    u32 code;

    target = unit2;
    targetPart = part;
    sub = 0;
    battleWork = _battleWorkPointer;
    damage = 0;
    code = *param_5;
    if ((code & 0x2000) != 0) {
        return 0;
    }

    callback = *(s32 (**)(BattleWorkUnit*, void*, BattleWorkUnit*, BattleWorkUnitPart*))((s32)weapon + 0x40);
    if (callback == NULL) {
        return 0;
    }
    if ((*(u32*)((s32)targetPart + 0x1AC) & 0x20000000) != 0) {
        return 0;
    }

    if (unit1 != NULL &&
        ((*(u32*)((s32)weapon + 0x74) & 1) != 0) &&
        (*(u8*)((s32)unit1 + 0x2E6) != 0) &&
        ((*(u32*)((s32)battleWork + 0x1CB8) & 2) == 0)) {
        return 0;
    }

    if (callback != NULL) {
        damage = callback(unit1, weapon, target, targetPart);
    }
    if ((param_6 & 0x40000) != 0) {
        sub = 1;
    }
    damage -= sub;
    if (damage < 0) {
        damage = 0;
    }
    if (damage > 0) {
        code = *param_5;
        if ((code & 0xFF) == 0x1E) {
            *param_5 = (code & ~0xFF) | 0x17;
        }
    }
    return damage;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleSetStatusDamageFromWeapon size 0x14C */
/* signature source: manual_signatures */
u32 BattleSetStatusDamageFromWeapon(BattleWorkUnit* unit1, BattleWorkUnit* unit2, BattleWorkUnitPart* part, void* weapon, u32 param_5) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB __declare size 0x15C */
/* signature source: manual_signatures */
void __declare(int param_1, s32 param_2, int* param_3, int* param_4) {
    extern s32 BtlUnit_CanGuardStatus(BattleWorkUnit*);
    extern void BattleRunHitEvent(void*, s32);
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 i;
    s32 count;
    s32 partCount;

    *param_3 = -1;
    *param_4 = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 64; i++) {
        if (i != param_1) {
            unit = BattleGetUnitPtr(battleWork, i);
            if (unit != NULL &&
                *(s32*)((s32)unit + 0x140) == param_1 &&
                ((*(u32*)((s32)unit + 0x27C) & 1) == 0) &&
                ((*(u32*)((s32)unit + 0x27C) & 4) == 0) &&
                BtlUnit_CanGuardStatus(unit) != 0) {
                part = *(BattleWorkUnitPart**)((s32)unit + 0x14);
                partCount = *(u8*)(*(s32*)((s32)unit + 0x10) + 0xB4);
                count = 0;
                while (count < partCount) {
                    if ((*(u32*)((s32)part + 0x1AC) & 8) != 0) {
                        break;
                    }
                    part = *(BattleWorkUnitPart**)part;
                    count++;
                }
                if (count < partCount) {
                    break;
                }
            }
        }
    }

    if (i < 64) {
        BattleWorkUnit* target = BattleGetUnitPtr(battleWork, param_1);
        *(s32*)((s32)target + 0x274) = 0;
        target = BattleGetUnitPtr(battleWork, param_1);
        BattleRunHitEvent(target, 0x1029);
        target = BattleGetUnitPtr(battleWork, param_1);
        *(u32*)((s32)target + 0x27C) |= 4;
        battleWork->flags |= 1;
        *(s32*)((s32)unit + 0x274) = 0;
        BattleRunHitEvent(unit, 0x1038);
        *(u32*)((s32)unit + 0x27C) |= 2;
        *param_3 = i;
        *param_4 = **(s32**)((s32)part + 4);
    }
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleAttackDeclare size 0x150 */
/* signature source: manual_signatures */
void BattleAttackDeclare(int* unitIdx, int* partIdx) {
    extern s32 BtlUnit_CanGuardStatus(BattleWorkUnit*);
    extern void BattleRunHitEvent(void*, s32);
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 i;
    s32 count;
    s32 partCount;

    battleWork = _battleWorkPointer;
    for (i = 0; i < 64; i++) {
        if (i != *unitIdx) {
            unit = BattleGetUnitPtr(battleWork, i);
            if (unit != NULL &&
                *(s32*)((s32)unit + 0x140) == *unitIdx &&
                ((*(u32*)((s32)unit + 0x27C) & 1) == 0) &&
                ((*(u32*)((s32)unit + 0x27C) & 4) == 0) &&
                BtlUnit_CanGuardStatus(unit) != 0) {
                part = *(BattleWorkUnitPart**)((s32)unit + 0x14);
                partCount = *(u8*)(*(s32*)((s32)unit + 0x10) + 0xB4);
                count = 0;
                while (count < partCount) {
                    if ((*(u32*)((s32)part + 0x1AC) & 8) != 0) {
                        break;
                    }
                    part = *(BattleWorkUnitPart**)part;
                    count++;
                }
                if (count < partCount) {
                    break;
                }
            }
        }
    }

    if (i < 64) {
        BattleWorkUnit* target = BattleGetUnitPtr(battleWork, *unitIdx);
        *(s32*)((s32)target + 0x274) = 0;
        target = BattleGetUnitPtr(battleWork, *unitIdx);
        BattleRunHitEvent(target, 0x1029);
        target = BattleGetUnitPtr(battleWork, *unitIdx);
        *(u32*)((s32)target + 0x27C) |= 4;
        battleWork->flags |= 1;
        *(s32*)((s32)unit + 0x274) = 0;
        BattleRunHitEvent(unit, 0x1038);
        *(u32*)((s32)unit + 0x27C) |= 2;
        *unitIdx = i;
        *partIdx = **(s32**)((s32)part + 4);
    }
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _getSickStatusParam size 0x460 */
/* signature source: manual_signatures */
void _getSickStatusParam(BattleWorkUnit* unit, void* weapon, StatusEffectType statusType, int* turns, int* strength) {
    ;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCalculateDamage size 0x69C */
/* signature source: manual_signatures */
int BattleCalculateDamage(BattleWorkUnit* unit1, BattleWorkUnit* unit2, BattleWorkUnitPart* part, void* weapon, u32* param_5, u32 param_6) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleSetStatusDamage size 0x107C */
/* signature source: manual_signatures */
s32 BattleSetStatusDamage(u32* param_1, BattleWorkUnit* unit, BattleWorkUnitPart* part, u32 attackPropertyFlags, StatusEffectType statusType, int rate, int galeFactor, char turns, char strength) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */
