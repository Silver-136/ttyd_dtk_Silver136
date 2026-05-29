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
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _checkDamageCode_EmergencyRevival size 0xC0 */
/* signature source: manual_signatures */
s32 _checkDamageCode_EmergencyRevival(int param_1, u32* param_2) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _getRegistStatus size 0xDC */
/* signature source: manual_signatures */
u32 _getRegistStatus(BattleWorkUnit* unit, StatusEffectType statusType) {
    return 0;
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
    return 0;
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
u8 __declare(int param_1, s32 param_2, int* param_3, int* param_4) {
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleAttackDeclare size 0x150 */
/* signature source: manual_signatures */
void BattleAttackDeclare(int* unitIdx, int* partIdx) {
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB _getSickStatusParam size 0x460 */
/* signature source: manual_signatures */
void _getSickStatusParam(BattleWorkUnit* unit, void* weapon, StatusEffectType statusType, int* turns, int* strength) {
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
