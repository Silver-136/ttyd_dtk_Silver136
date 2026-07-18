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
s32 BattleCheckDamage(BattleWorkUnit* attacker, BattleWorkUnit* target,
                      BattleWorkUnitPart* part, void* weapon, s32 flags) {
    extern void BattleInitCounterPreCheckWork(void*);
    extern s32 BattleCheckCounter(void*, BattleWorkUnit*, BattleWorkUnit*, BattleWorkUnitPart*, void*, u32*);
    extern s32 BtlUnit_GetACPossibility(BattleWorkUnit*);
    extern s32 BattleActionCommandGetDefenceResult(void);
    extern void BtlUnit_GetHitPos(BattleWorkUnit*, BattleWorkUnitPart*, f32*, f32*, f32*);
    extern void* effNiceEntry(f32, f32, f32, s32);
    extern void psndSFXOn(const char*);
    extern void BattleAudience_Case_GuardGood(void);
    extern void BattleAudience_Case_GuardBad(void);
    extern void BattleRunHitEvent(BattleWorkUnit*, s32);
    extern s32 BtlUnit_CheckStatus(BattleWorkUnit*, s32);
    extern void BattleCheckPikkyoro(void*, u32*);
    extern s32 BattleCalculateDamage(BattleWorkUnit*, BattleWorkUnit*, BattleWorkUnitPart*, void*, u32*, u32);
    extern s32 BattleCalculateFpDamage(BattleWorkUnit*, BattleWorkUnit*, BattleWorkUnitPart*, void*, u32*, u32);
    extern u32 BattleSetStatusDamageFromWeapon(BattleWorkUnit*, BattleWorkUnit*, BattleWorkUnitPart*, void*, u32);
    extern void BattleDamageDirect(s32, BattleWorkUnit*, BattleWorkUnitPart*, s32, s32, s32, s32, s32);
    extern char str_SFX_SYSTEM_GREAT1_802ee064[];
    extern char str_SFX_SYSTEM_NICE1_802ee078[];
    u32 result[8];
    s32 counterWork[12];
    Vec hitPos;
    void* effect;
    u32 statusResult;
    s32 damage;
    s32 fpDamage;
    s32 defence;
    u8 counterType;
    BOOL flip;

    result[0] = flags;
    statusResult = 0;
    flip = FALSE;
    BattleInitCounterPreCheckWork(counterWork);
    if (target == NULL) {
        return 0x12;
    }

    if ((*(u32*)((u8*)weapon + 0x74) & 0x20) == 0 &&
        BtlUnit_GetACPossibility(target) != 0 &&
        BattleActionCommandGetDefenceResult() == 5) {
        BtlUnit_GetHitPos(target, part, &hitPos.x, &hitPos.y, &hitPos.z);
        effect = effNiceEntry(hitPos.x - 60.0f, hitPos.y + 45.0f,
                              hitPos.z - 10.0f, 2);
        *(f32*)((u8*)*(void**)((u8*)effect + 0xC) + 0x1C) = 0.75f;
        result[0] |= 0x80000;
        psndSFXOn(str_SFX_SYSTEM_GREAT1_802ee064);
        BattleAudience_Case_GuardGood();
        counterType = *(u8*)((u8*)weapon + 0x13);
        if (counterType == 3 || counterType == 4 || counterType >= 5) {
            result[7] = result[0] | 0x12A;
            result[0] |= 0x100;
            BattleDamageDirect(attacker->unitId, target, part, 0, 0,
                               result[7], *(u8*)((u8*)weapon + 0x6D), 0);
            return -1;
        }
        if (counterType == 0 || counterType == 1) {
            result[7] = result[0] | 0x12A;
        } else {
            result[7] = result[0] | 0x2A;
        }
        BattleDamageDirect(attacker->unitId, target, part, 0, 0,
                           result[7], *(u8*)((u8*)weapon + 0x6D), 0);
        return 0x15;
    }

    BattleCheckCounter(counterWork, attacker, target, part, weapon, result);
    if (counterWork[0] == 0) {
        goto finish;
    }
    if ((result[0] & 0x100) != 0) {
        if ((*(u32*)((u8*)part + 0x1B0) & 0x80000) != 0) {
            BattleRunHitEvent(target, 0x719);
            goto finish;
        }
        if ((*(u32*)((u8*)part + 0x1B0) & 0x100000) != 0 &&
            (*(u8*)((u8*)weapon + 0x6C) == 1 ||
             *(u8*)((u8*)weapon + 0x6C) == 2 ||
             *(u8*)((u8*)weapon + 0x6C) == 3 ||
             ((*(u32*)((u8*)weapon + 0x74) & 0x8000000) != 0 &&
              BtlUnit_CheckStatus(attacker, 9) != 0))) {
            BattleRunHitEvent(target, 0x719);
            goto finish;
        }
    }

    if ((*(u32*)((u8*)weapon + 0x74) & 0x20) == 0 &&
        BtlUnit_GetACPossibility(target) != 0) {
        defence = BattleActionCommandGetDefenceResult();
        if (defence < 4) {
            BattleAudience_Case_GuardBad();
        } else {
            BtlUnit_GetHitPos(target, part, &hitPos.x, &hitPos.y, &hitPos.z);
            effect = effNiceEntry(hitPos.x - 60.0f, hitPos.y + 45.0f,
                                  hitPos.z - 10.0f, 0);
            *(f32*)((u8*)*(void**)((u8*)effect + 0xC) + 0x1C) = 0.75f;
            result[0] |= 0x40000;
            psndSFXOn(str_SFX_SYSTEM_NICE1_802ee078);
            BattleAudience_Case_GuardGood();
        }
    }

    if ((*(u32*)((u8*)weapon + 0x74) & 0x400000) != 0 &&
        (*(u32*)((u8*)part + 0x1AC) & 0x4000000) != 0) {
        flip = TRUE;
        result[0] |= 0x200000;
    }
    if ((*(u32*)((u8*)weapon + 0x74) & 0x800000) != 0 &&
        (*(u32*)((u8*)part + 0x1AC) & 0x8000000) != 0) {
        flip = TRUE;
        result[0] |= 0x200000;
    }
    BattleCheckPikkyoro(weapon, result);
    damage = BattleCalculateDamage(attacker, target, part, weapon,
                                   result + 7, result[0]);
    fpDamage = BattleCalculateFpDamage(attacker, target, part, weapon,
                                       result + 7, result[0]);
    if ((result[0] & 0x40000) == 0) {
        if ((result[0] & 0x40000000) != 0) {
            statusResult = BattleSetStatusDamageFromWeapon(attacker, target,
                                                           part, weapon,
                                                           result[0]);
        } else if ((result[0] & 0x100) != 0) {
            statusResult = BattleSetStatusDamageFromWeapon(attacker, target,
                                                           part, weapon,
                                                           result[0]);
        }
    }
    if ((result[7] & 0xFF) == 0x1E && flip) {
        result[7] = result[0] | 0x17;
    }
    if ((result[7] & 0xFF) == 0x1E &&
        (*(u8*)((u8*)weapon + 0x6D) == 0x13 ||
         *(u8*)((u8*)weapon + 0x6D) == 0x1B)) {
        result[7] = result[0] | 0x17;
    }
    if ((result[7] & 0xFF) == 0x1E && statusResult != 0) {
        if ((statusResult & 1) == 0) {
            result[7] = result[0] | 0x16;
        } else {
            result[7] = result[0] | 0x17;
        }
    }
    if ((statusResult & 4) != 0) {
        result[7] = result[0] | 0x1C;
    }
    if ((statusResult & 8) != 0) {
        result[7] = result[0] | 0x1D;
    }
    *(s8*)((u8*)target + 0x119) = damage;
    *(s8*)((u8*)target + 0x11A) = fpDamage;

finish:
    return 0x12;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleDamageDirect size 0xB24 */
void BattleDamageDirect(s32 unitIdx, BattleWorkUnit* unit, BattleWorkUnitPart* part,
                        s32 damage, s32 fpDamage, s32 flags, s32 hitEffect, s32 runEvent) {
    extern BattleWork* _battleWorkPointer;
    extern BattleWorkUnit* BattleGetUnitPtr(BattleWork*, s32);
    extern BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit*, s32);
    extern s32 BtlUnit_GetBodyPartsId(BattleWorkUnit*);
    extern void BtlUnit_GetHitPos(BattleWorkUnit*, BattleWorkUnitPart*, f32*, f32*, f32*);
    extern s32 BtlUnit_GetFp(BattleWorkUnit*);
    extern void BtlUnit_SetFp(BattleWorkUnit*, s32);
    extern u32 BtlUnit_GetBelong(BattleWorkUnit*);
    extern void* effMissStarEntry(f64, f64, f64, s32, s32, s32);
    extern void effHitEntry(void);
    extern void effIceN64Entry(f32, f32, f32, s32);
    extern void effDamageStarEntry(f32, f32, f32, f32, f32, s32, s32);
    extern void BattleRunHitEvent(BattleWorkUnit*, s32);
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    s32 fp;
    BattleWorkUnit* attacker;

    if (unitIdx == -5) {
        attacker = 0;
    } else {
        attacker = BattleGetUnitPtr(_battleWorkPointer, unitIdx);
    }
    if (damage >= 100) {
        damage = 99;
    } else if (damage < -99) {
        damage = -99;
    }
    if (attacker != 0) {
        *(s32*)((s32)attacker + 0x284) += damage;
    }
    if (part == 0) {
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    }
    *(BattleWorkUnitPart**)((s32)unit + 0x258) = part;
    *(u32*)((s32)unit + 0x274) = hitEffect;
    if ((*(u32*)((s32)unit + 0x104) & 0x200000) != 0 &&
        (hitEffect == 0x15 || hitEffect - 0x16 < 2 || hitEffect == 0x18)) {
        *(u32*)((s32)unit + 0x274) = 0;
    }
    BtlUnit_GetHitPos(unit, part, &hitX, &hitY, &hitZ);

    if ((flags & 0x2000) == 0 && damage == 0 && fpDamage == 0) {
        BtlUnit_GetHitPos(unit, part, &hitX, &hitY, &hitZ);
        if (hitEffect != 2 && hitEffect != 3 && hitEffect != 0x15 && hitEffect != 0x16 &&
            hitEffect != 0x14 && hitEffect != 0x13 && hitEffect != 0x1B && hitEffect != 7 &&
            (flags & 0xFF) != 0x16) {
            effMissStarEntry(hitX, hitY, hitZ, (flags & 0x20000) != 0, 1,
                             BtlUnit_GetBelong(unit) == 0 ? -1 : 1);
        }
    }

    if ((flags & 0x2000) == 0 && (damage != 0 || fpDamage != 0)) {
        BtlUnit_GetHitPos(unit, part, &hitX, &hitY, &hitZ);
        if ((flags & 0xFF) == 0x1A) {
            effIceN64Entry(hitX, hitY, hitZ, 0);
        }
        effHitEntry();
        if (BtlUnit_GetBelong(unit) == 0) {
            if (damage != 0 && (*(u32*)((s32)unit + 0x104) & 0x400000) == 0) {
                effDamageStarEntry(hitX - 8.0f, hitY + 5.0f, hitZ, 10.0f, 30.0f, 0, damage);
            }
            if (fpDamage != 0) {
                effDamageStarEntry(hitX - 8.0f, hitY + 5.0f, hitZ, 10.0f,
                                   damage == 0 ? 30.0f : 60.0f, 1, fpDamage);
            }
        } else {
            if (damage != 0 && (*(u32*)((s32)unit + 0x104) & 0x400000) == 0) {
                effDamageStarEntry(hitX, hitY + 10.0f, hitZ, 10.0f, -30.0f, 0, damage);
            }
            if (fpDamage != 0) {
                effDamageStarEntry(hitX, hitY + 10.0f, hitZ, 10.0f,
                                   damage == 0 ? -30.0f : -60.0f, 1, fpDamage);
            }
        }
    }

    *(s8*)((s32)unit + 0x270) = damage;
    *(s32*)((s32)unit + 0x264) += *(s8*)((s32)unit + 0x270);
    *(s8*)((s32)unit + 0x271) = fpDamage;
    *(s8*)((s32)unit + 0x272) = fpDamage;
    *(s32*)((s32)unit + 0x268) += *(s8*)((s32)unit + 0x271);
    if ((flags & 0x2000) == 0 && (damage != 0 || fpDamage != 0)) {
        if ((*(u32*)((s32)unit + 0x104) & 0x40000000) == 0) {
            *(s32*)((s32)unit + 0x208) = *(s16*)((s32)unit + 0x10C);
            *(s16*)((s32)unit + 0x10C) -= *(s8*)((s32)unit + 0x270);
            if (*(s16*)((s32)unit + 0x10C) > *(s16*)((s32)unit + 0x108)) {
                *(s16*)((s32)unit + 0x10C) = *(s16*)((s32)unit + 0x108);
            }
            if (*(s16*)((s32)unit + 0x10C) < 0) {
                *(s16*)((s32)unit + 0x10C) = 0;
            }
        }
        if (fpDamage != 0) {
            fp = BtlUnit_GetFp(unit) - (s8)fpDamage;
            if (fp < 0) {
                *(s8*)((s32)unit + 0x272) = BtlUnit_GetFp(unit);
                fp = 0;
            }
            BtlUnit_SetFp(unit, fp);
            *(s32*)((s32)unit + 0x26C) += *(s8*)((s32)unit + 0x272);
        }
    }
    if (runEvent != 0) {
        BattleRunHitEvent(unit, flags);
    }
}

/* AUTOSTUB BattleCheckCounter size 0x5A4 */
s32 BattleCheckCounter(void* work, BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32* result) {
    extern s32 BtlUnit_CheckStatus(BattleWorkUnit* unit, StatusEffectType type);
    u32 attr;
    u32 resist;
    s32 kind;

    if (attacker == NULL) {
        return 0;
    }

    *(s32*)((s32)work + 0x28) = 0;
    attr = *(u32*)((s32)part + 0x1B0);
    resist = *(u32*)((s32)weapon + 0x78);

    if ((attr & 1) && *(u8*)((s32)attacker + 0x302) == 0 && (resist & 4) == 0) {
        *(s32*)((s32)work + 0x20) = 0x2B;
        *(s32*)((s32)work + 0x24) = 0x12B;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x1F;
        *result |= 0x100;
        *(s32*)work = 0;
        goto done;
    }
    if ((attr & 2) && *(u8*)((s32)attacker + 0x302) == 0 && (resist & 8) == 0) {
        *(s32*)((s32)work + 0x20) = 0x2C;
        *(s32*)((s32)work + 0x24) = 0x12C;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x20;
        *result |= 0x100;
        *(s32*)work = 0;
        goto done;
    }
    if ((attr & 4) && *(u8*)((s32)attacker + 0x302) == 0 && (resist & 0x10) == 0) {
        *(s32*)((s32)work + 0x20) = 0x2D;
        *(s32*)((s32)work + 0x24) = 0x12D;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x21;
        *result |= 0x100;
        *(s32*)work = 0;
        goto done;
    }
    if (((attr & 0x30) != 0) && *(u8*)((s32)attacker + 0x301) == 0 && (resist & 0x20) == 0) {
        *(s32*)((s32)work + 0x20) = 0x2E;
        *(s32*)((s32)work + 0x24) = 0x12E;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x22;
        *result |= 0x100;
        *(s32*)work = 0;
        if (attr & 0x20) {
            *(s32*)((s32)work + 0x8) = 1;
        }
        goto done;
    }
    if (((attr & 0xC0) != 0) && (resist & 0x40) == 0) {
        *(s32*)((s32)work + 0x20) = 0x2F;
        *(s32*)((s32)work + 0x24) = 0x12F;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x23;
        *result |= 0x100;
        *(s32*)work = 0;
        if (attr & 0x80) {
            *(s32*)((s32)work + 0x14) = 1;
        }
        goto done;
    }
    if (((attr & 0xC00) != 0) && (resist & 1) == 0 && !BtlUnit_CheckStatus(attacker, STATUS_ELECTRIC)) {
        *(s32*)((s32)work + 0x20) = 0x30;
        *(s32*)((s32)work + 0x24) = 0x130;
        *(s32*)((s32)work + 0x28) += 1;
        *(s32*)((s32)work + 0x2C) = 0x24;
        *result |= 0x100;
        *(s32*)work = 0;
        if (attr & 0x800) {
            *(s32*)((s32)work + 0x10) = 1;
        }
        goto done;
    }

    kind = *(u8*)((s32)weapon + 0x6C);
    if ((attr & 0x1000) && (resist & 0x80) == 0 &&
        (kind == 3 || kind == 4 || kind == 1 ||
         ((*(u32*)((s32)weapon + 0x74) & 0x400) && BtlUnit_CheckStatus(attacker, STATUS_BURN)))) {
        *(s32*)((s32)work + 0x20) = 0x31;
        *(s32*)((s32)work + 0x24) = 0x131;
        *(s32*)((s32)work + 0x28) += 1;
        *result |= 0x100;
    }
    if ((attr & 0x2000) && (resist & 0x100) == 0) {
        *(s32*)((s32)work + 0x20) = 0x32;
        *(s32*)((s32)work + 0x24) = 0x132;
        *(s32*)((s32)work + 0x28) += 1;
        *result |= 0x100;
    }
    if ((BtlUnit_CheckStatus(target, STATUS_ELECTRIC) || (attr & 0xC00)) &&
        (resist & 1) == 0 && !BtlUnit_CheckStatus(attacker, STATUS_ELECTRIC)) {
        *(s32*)((s32)work + 0x20) = 0x34;
        *(s32*)((s32)work + 0x24) = 0x134;
        *(s32*)((s32)work + 0x28) += 1;
        *result |= 0x100;
        if (attr & 0x800) {
            *(s32*)((s32)work + 0x10) = 1;
        }
    }
    if (BtlUnit_CheckStatus(target, STATUS_PAYBACK) && (resist & 0x200) == 0) {
        *(s32*)((s32)work + 0x20) = 0x35;
        *(s32*)((s32)work + 0x24) = 0x135;
        *result |= 0x100;
        *(s32*)((s32)work + 4) = 1;
    }
    if (((attr & 0x300) != 0) && (resist & 0x40) == 0) {
        *(s32*)((s32)work + 0x20) = 0x33;
        *(s32*)((s32)work + 0x24) = 0x133;
        *result |= 0x100;
        *(s32*)((s32)work + 4) = 1;
        if (attr & 0x200) {
            *(s32*)((s32)work + 0xC) = 1;
        }
    }
    if (BtlUnit_CheckStatus(target, STATUS_HOLD_FAST) && (resist & 0x400) == 0) {
        *(s32*)((s32)work + 0x20) = 0x36;
        *(s32*)((s32)work + 0x24) = 0x136;
        *result |= 0x100;
        *(s32*)((s32)work + 0x18) = 1;
    }
    if (*(u8*)((s32)target + 0x2F0) != 0 && (resist & 0x200) == 0) {
        *(s32*)((s32)work + 0x20) = 0x35;
        *(s32*)((s32)work + 0x24) = 0x135;
        *result |= 0x100;
        *(s32*)((s32)work + 0x1C) = 1;
    }

done:
    switch (*(s32*)((s32)work + 0x20)) {
        case 0x2B: return 8;
        case 0x2C: return 7;
        case 0x2D: return 9;
        case 0x2E: return 10;
        case 0x2F: return 11;
        case 0x31: return 12;
        case 0x32: return 13;
        case 0x33: return 15;
        case 0x34: return 14;
        case 0x35: return 16;
        case 0x36: return 17;
    }
    return 0;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleAttackDeclareAll size 0x118 */
/* signature source: prototype_search */
void BattleAttackDeclareAll(BattleWork* battleWork) {
    extern void __declare(int param_1, s32 param_2, s32* param_3, s32* param_4);
    s32 unitId;
    s32 partId;
    s32 i;

    for (i = 0; i < *(s8*)((s32)battleWork + 0xE94); i++) {
        s32 index = *(s8*)((s32)battleWork + 0xE95 + i);
        s16* entry = (s16*)((s32)battleWork + 0x428 + index * 0x24 + 4);
        __declare(entry[0], entry[1], &unitId, &partId);
        if (unitId != -1) {
            s32 j;
            for (j = 0; j < *(s8*)((s32)battleWork + 0xE94); j++) {
                s32 otherIndex = *(s8*)((s32)battleWork + 0xE95 + j);
                s16* other = (s16*)((s32)battleWork + 0x428 + otherIndex * 0x24 + 4);
                if (other[0] == unitId) {
                    s32 k;
                    for (k = j; k < *(s8*)((s32)battleWork + 0xE94) - 1; k++) {
                        *(u8*)((s32)battleWork + 0xE95 + k) = *(u8*)((s32)battleWork + 0xE96 + k);
                    }
                    if (j <= i) {
                        i--;
                    }
                    *(u8*)((s32)battleWork + 0xE94) = *(u8*)((s32)battleWork + 0xE94) - 1;
                    break;
                }
            }
            entry[0] = unitId;
            entry[1] = partId;
        }
    }
    *(u8*)((s32)battleWork + 0xEDF) = 0;
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
    switch (statusType) {
        case 0:
            return *(u8*)((s32)weapon + 0x80);
        case 1:
            return *(u8*)((s32)weapon + 0x82);
        case 2:
            return *(u8*)((s32)weapon + 0x84);
        case 3:
            return *(u8*)((s32)weapon + 0x86);
        case 4:
            return *(u8*)((s32)weapon + 0x89);
        case 5:
            return *(u8*)((s32)weapon + 0x8B);
        case 6:
            return *(u8*)((s32)weapon + 0x8D);
        case 7:
            return *(u8*)((s32)weapon + 0x8F);
        case 8:
            return *(u8*)((s32)weapon + 0x91);
        case 9:
            if (*(s8*)((s32)weapon + 0x95) > 0) {
                return *(u8*)((s32)weapon + 0x93);
            }
            return 0;
        case 10:
            if (*(s8*)((s32)weapon + 0x95) <= 0) {
                return *(u8*)((s32)weapon + 0x93);
            }
            return 0;
        case 11:
            if (*(s8*)((s32)weapon + 0x98) > 0) {
                return *(u8*)((s32)weapon + 0x96);
            }
            return 0;
        case 12:
            if (*(s8*)((s32)weapon + 0x98) <= 0) {
                return *(u8*)((s32)weapon + 0x96);
            }
            return 0;
        case 13:
            if (*(s8*)((s32)weapon + 0x9B) > 0) {
                return *(u8*)((s32)weapon + 0x99);
            }
            return 0;
        case 14:
            if (*(s8*)((s32)weapon + 0x9B) <= 0) {
                return *(u8*)((s32)weapon + 0x99);
            }
            return 0;
        case 15:
            return *(u8*)((s32)weapon + 0x9C);
        case 16:
            return *(u8*)((s32)weapon + 0xA0);
        case 17:
            return *(u8*)((s32)weapon + 0xA2);
        case 18:
            return *(u8*)((s32)weapon + 0xA4);
        case 19:
            return *(u8*)((s32)weapon + 0xA5);
        case 20:
            return *(u8*)((s32)weapon + 0x9E);
        case 21:
            return *(u8*)((s32)weapon + 0xA8);
        case 22:
            if (*(s8*)((s32)weapon + 0xAA) > 0) {
                return 100;
            }
            return 0;
        case 23:
            if (*(s8*)((s32)weapon + 0xAC) > 0) {
                return 100;
            }
            return 0;
        case 24:
            if (*(s8*)((s32)weapon + 0x9F) != 0) {
                return 100;
            }
            return 0;
        case 25:
            if (*(s8*)((s32)weapon + 0xA6) != 0) {
                return 100;
            }
            return 0;
        case 26:
            if (*(s8*)((s32)weapon + 0xA7) > 0) {
                return 100;
            }
            return 0;
        case 27:
            return 0;
        default:
            return 0;
    }
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCheckPikkyoro size 0x1A8 */
/* signature source: manual_signatures */
u32 BattleCheckPikkyoro(void* weapon, u32* param_2) {
    s32 flags[5] = { 0, 0, 0, 0, 0 };
    void* battleWork;
    u32 result;
    s32 count;
    s32 chosen;
    s32 value;
    s32 offset;
    s32 i;

    battleWork = _battleWorkPointer;
    count = 0;
    chosen = 0;
    result = 0;
    if (param_2 != 0) {
        result = *param_2;
    }

    if ((*(u32*)((s32)weapon + 0x74) & 0x800) != 0) {
        value = *(s32*)((s32)battleWork + 0x163F4);
        if ((value & 0x100) != 0) {
            flags[0] = 1;
            chosen = 0;
            count++;
        }
        if ((value & 0x200) != 0) {
            flags[1] = 1;
            chosen = 1;
            count++;
        }
        if ((value & 0x400) != 0) {
            flags[2] = 1;
            chosen = 2;
            count++;
        }
        if ((value & 0x800) != 0) {
            flags[3] = 1;
            chosen = 3;
            count++;
        }
        if ((value & 0x1000) != 0) {
            flags[4] = 1;
            chosen = 4;
            count++;
        }
    }

    if (count > 0) {
        if (count >= 2) {
            value = irand(count);
            chosen = 4;
            offset = 4;
            for (i = 0; i < 4; i++) {
                value -= flags[offset];
                if (value < 0) {
                    break;
                }
                chosen--;
                offset--;
            }
        }

        switch (chosen) {
            case 0:
                result |= 0x01000000;
                break;
            case 1:
                result |= 0x02000000;
                break;
            case 2:
                result |= 0x04000000;
                break;
            case 3:
                result |= 0x08000000;
                break;
            case 4:
                result |= 0x10000000;
                break;
        }
    }

    if (param_2 != 0) {
        *param_2 = result;
    }
    return result;
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
    extern void _getSickStatusParam(BattleWorkUnit* unit, void* weapon, StatusEffectType statusType, s8* turns, s8* strength);
    extern u32 _getSickStatusRate(void* weapon, StatusEffectType statusType);
    extern s32 BattleSetStatusDamage(u32* param_1, BattleWorkUnit* unit, BattleWorkUnitPart* part, u32 attackPropertyFlags, StatusEffectType statusType, int rate, int galeFactor, char turns, char strength);
    extern void btlDispPoseAnime(void* part);
    u32 result = 0;
    s32 status;

    if ((param_5 & 0x20000000) == 0) {
        for (status = 0; status < 28; status++) {
            s8 turns;
            s8 strength;
            s32 rate;
            s32 galeFactor = 0;
            s32 doApply = 1;

            _getSickStatusParam(unit1, weapon, status, &turns, &strength);
            rate = _getSickStatusRate(weapon, status);
            switch (status) {
                case 10:
                case 11:
                case 12:
                    if (turns == 0 && strength == 0) {
                        doApply = 0;
                    }
                    break;
                case 26:
                    if (rate > 0) {
                        galeFactor = *(u8*)((s32)unit1 + 0xD) - *(u8*)((s32)unit2 + 0xD);
                    }
                    break;
            }
            if (doApply && BattleSetStatusDamage(&result, unit2, part, *(u32*)((s32)weapon + 0x74), status, rate, galeFactor, turns, strength) != 0) {
                if (status == 18) {
                    btlDispPoseAnime(part);
                }
            }
        }
    }
    return result;
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
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern void* badgeWeapon_Charge;
    extern void* badgeWeapon_ChargeP;
    extern void* badgeWeapon_SuperCharge;
    extern void* badgeWeapon_SuperChargeP;
    s32 t = 0;
    s32 s = 0;

    switch (statusType) {
        case 0:
            s = *(s8*)((s32)weapon + 0x81);
            t = s;
            break;
        case 1:
            s = *(s8*)((s32)weapon + 0x83);
            t = s;
            break;
        case 2:
            s = *(s8*)((s32)weapon + 0x85);
            t = s;
            break;
        case 3:
            t = *(s8*)((s32)weapon + 0x87);
            s = *(s8*)((s32)weapon + 0x88);
            break;
        case 4:
            s = *(s8*)((s32)weapon + 0x8A);
            t = s;
            break;
        case 5:
            s = *(s8*)((s32)weapon + 0x8C);
            t = s;
            break;
        case 6:
            s = *(s8*)((s32)weapon + 0x8E);
            t = s;
            break;
        case 7:
            s = *(s8*)((s32)weapon + 0x90);
            t = s;
            break;
        case 8:
            s = *(s8*)((s32)weapon + 0x92);
            t = s;
            break;
        case 9:
            if (*(s8*)((s32)weapon + 0x95) > 0) {
                t = *(s8*)((s32)weapon + 0x94);
                s = *(s8*)((s32)weapon + 0x95);
            }
            break;
        case 10:
            if (*(s8*)((s32)weapon + 0x95) < 0) {
                t = *(s8*)((s32)weapon + 0x94);
                s = *(s8*)((s32)weapon + 0x95);
            }
            break;
        case 11:
            if (*(s8*)((s32)weapon + 0x98) > 0) {
                t = *(s8*)((s32)weapon + 0x97);
                s = *(s8*)((s32)weapon + 0x98);
            }
            break;
        case 12:
            if (*(s8*)((s32)weapon + 0x98) < 0) {
                t = *(s8*)((s32)weapon + 0x97);
                s = *(s8*)((s32)weapon + 0x98);
            }
            break;
        case 13:
            if (*(s8*)((s32)weapon + 0x9B) > 0) {
                t = *(s8*)((s32)weapon + 0x9A);
                s = *(s8*)((s32)weapon + 0x9B);
            }
            break;
        case 14:
            if (*(s8*)((s32)weapon + 0x9B) < 0) {
                t = *(s8*)((s32)weapon + 0x9A);
                s = *(s8*)((s32)weapon + 0x9B);
            }
            break;
        case 15:
            s = *(s8*)((s32)weapon + 0x9D);
            t = s;
            break;
        case 16:
            s = *(s8*)((s32)weapon + 0xA1);
            t = s;
            break;
        case 17:
            s = *(s8*)((s32)weapon + 0xA3);
            t = s;
            break;
        case 18:
            s = *(s8*)((s32)weapon + 0x9E);
            t = s;
            break;
        case 19:
            s = *(s8*)((s32)weapon + 0xA9);
            t = s;
            break;
        case 20:
            t = *(s8*)((s32)weapon + 0xAA);
            s = *(s8*)((s32)weapon + 0xAB);
            break;
        case 21:
            t = *(s8*)((s32)weapon + 0xAC);
            s = *(s8*)((s32)weapon + 0xAD);
            break;
        case 24:
            t = *(s8*)((s32)weapon + 0x9F);
            s = t;
            break;
        case 25:
            s = *(s8*)((s32)weapon + 0xA6);
            t = s;
            break;
        case 26:
            s = *(s8*)((s32)weapon + 0xA7);
            t = s;
            break;
    }

    {
        s32 badge = *(s32*)((s32)weapon + 8);
        s32 count = 0;
        if (badge >= 0xF0 && badge < 0x153) {
            count = pouchEquipCheckBadge(badge);
        }
        if (count > 0) {
            switch (badge) {
                case 0xF4:
                    if (statusType == 11) {
                        t += (count - 1) * 2;
                    }
                    break;
                case 0xF5:
                    if (statusType == 1) {
                        t += (count - 1) * 2;
                    }
                    break;
                case 0xF6:
                    if (statusType == 15) {
                        t += (count - 1) * 2;
                    }
                    break;
                case 0xFB:
                    if (statusType == 5) {
                        t += (count - 1) * 2;
                    }
                    break;
                case 0xFD:
                    if (statusType == 9) {
                        t += (count - 1) * 2;
                    }
                    break;
                case 0x14F:
                case 0x150:
                    if (statusType == 14 && *(u8*)((s32)unit + 0x2FE) >= 2) {
                        s = s + *(u8*)((s32)unit + 0x2FE) - 1;
                    }
                    break;
            }
        }
    }

    if (statusType == 16) {
        if (weapon == &badgeWeapon_Charge || weapon == &badgeWeapon_ChargeP) {
            s32 count;
            if (*(s32*)((s32)unit + 8) == 0xDE) {
                count = pouchEquipCheckBadge(0x100);
            } else {
                count = pouchEquipCheckBadge(0x101);
            }
            if (count >= 2) {
                s = s + count - 1;
            }
        } else if (weapon == &badgeWeapon_SuperCharge || weapon == &badgeWeapon_SuperChargeP) {
            s32 count;
            if (*(s32*)((s32)unit + 8) == 0xDE) {
                count = pouchEquipCheckBadge(0x151);
            } else {
                count = pouchEquipCheckBadge(0x152);
            }
            if (count >= 2) {
                s += (count - 1) * 2;
            }
        }
        if (s >= 10) {
            s = 9;
        }
    }

    *(s8*)turns = (s8)t;
    *(s8*)strength = (s8)s;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleCalculateDamage size 0x69C */
/* signature source: manual_signatures */
int BattleCalculateDamage(BattleWorkUnit* attacker, BattleWorkUnit* target,
                          BattleWorkUnitPart* part, void* weapon, u32* flagsOut, u32 flags) {
    typedef s32 (*DamageFunc)(BattleWorkUnit*, void*, BattleWorkUnit*, BattleWorkUnitPart*);
    extern s32 BtlUnit_CheckStatus(BattleWorkUnit*, StatusEffectType);
    extern void BtlUnit_GetStatus(BattleWorkUnit*, StatusEffectType, s8*, s8*);
    extern s32 BtlUnit_CheckStatusFlag(BattleWorkUnit*, u32);
    u8* atk = (u8*)attacker;
    u8* def = (u8*)target;
    u8* wp = (u8*)weapon;
    DamageFunc damageFunc;
    s8 turns;
    s8 strength;
    s32 damage;
    s32 defense;
    s32 element;
    s8 defenseAttr;
    u32 special;

    if ((target->tokenFlags & 2) != 0) {
        flags |= 0x1000;
    }
    element = *(u8*)(wp + 0x6C);
    special = *(u32*)(wp + 0x74);
    if (attacker != NULL && (special & 0x400) != 0 &&
        BtlUnit_CheckStatus(attacker, STATUS_BURN)) {
        element = 1;
    }
    *flagsOut = flags | 0x1E;
    damageFunc = *(DamageFunc*)(wp + 0x1C);
    if (damageFunc == NULL || (*(u32*)((u8*)part + 0x1AC) & 0x200000) != 0) {
        return 0;
    }
    damage = damageFunc(attacker, weapon, target, part);

    if (attacker != NULL && (special & 0x20) != 0) {
        damage += atk[0x2E4] + atk[0x2E5];
        if (attacker->currentHP <= *(u8*)((u8*)attacker->data + 0x0D)) {
            damage += atk[0x2ED] * 5;
        }
        if (attacker->currentHP <= *(u8*)((u8*)attacker->data + 0x0C)) {
            damage += atk[0x2EA] * 2;
        }
    }
    if (attacker != NULL && (special & 4) != 0 && BtlUnit_CheckStatus(attacker, STATUS_CHARGED)) {
        BtlUnit_GetStatus(attacker, STATUS_CHARGED, &turns, &strength);
        damage += strength;
        attacker->tokenFlags |= 8;
    }
    if (attacker != NULL && (special & 0x10) != 0) {
        if (BtlUnit_CheckStatus(attacker, STATUS_HUGE)) {
            BtlUnit_GetStatus(attacker, STATUS_HUGE, &turns, &strength);
            damage += strength;
        }
        if (BtlUnit_CheckStatus(attacker, STATUS_ATTACK_UP)) {
            BtlUnit_GetStatus(attacker, STATUS_ATTACK_UP, &turns, &strength);
            damage += strength;
        }
        if (BtlUnit_CheckStatus(attacker, STATUS_TINY)) {
            BtlUnit_GetStatus(attacker, STATUS_TINY, &turns, &strength);
            damage += strength;
        }
        if (BtlUnit_CheckStatus(attacker, STATUS_ATTACK_DOWN)) {
            BtlUnit_GetStatus(attacker, STATUS_ATTACK_DOWN, &turns, &strength);
            damage += strength;
        }
    }
    if (damage < 0) {
        damage = 0;
    }

    defenseAttr = (*(s8**)((u8*)part + 0x1B8))[element];
    defense = (s32)((*(s8**)((u8*)part + 0x1B4))[element]);
    switch (element) {
        case 0: *flagsOut = flags | 0x17; break;
        case 1: *flagsOut = flags | 0x18; break;
        case 2: *flagsOut = flags | 0x1A; break;
        case 3: *flagsOut = flags | 0x19; break;
        case 4: *flagsOut = flags | 0x1B; break;
        default: *flagsOut = flags | 0x17; break;
    }
    if (defenseAttr != 3) {
        defense += def[0x302];
        if ((flags & 0x40000) != 0) {
            defense += def[0x2F6];
        }
        if (BtlUnit_CheckStatusFlag(target, 0x1000000)) {
            defense++;
        }
        if (BtlUnit_CheckStatus(target, STATUS_DEFENSE_UP)) {
            BtlUnit_GetStatus(target, STATUS_DEFENSE_UP, &turns, &strength);
            defense += strength;
        }
        if (BtlUnit_CheckStatus(target, STATUS_DEFENSE_DOWN)) {
            BtlUnit_GetStatus(target, STATUS_DEFENSE_DOWN, &turns, &strength);
            defense += strength;
        }
        if ((special & 0x40) != 0 && defense > 0) {
            defense = 0;
        }
    }
    damage -= defense;
    damage += def[0x2E5];
    damage -= def[0x2E6];
    if ((flags & 0x40000) != 0) {
        damage--;
    }
    if (def[0x2F8] != 0) {
        damage *= def[0x2F8] + 1;
    }
    if (def[0x2F0] != 0 && target->currentHP <= *(u8*)((u8*)target->data + 0x0C)) {
        damage = (damage + def[0x2F0]) / (def[0x2F0] + 1);
    }
    if (defenseAttr == 1) {
        damage++;
    } else if (defenseAttr == 2 || (defenseAttr == 4 && (flags & 0x80000000) == 0)) {
        damage = 0;
    }
    if (damage < 0) {
        damage = 0;
    }
    if (defenseAttr == 3) {
        damage = -damage;
        *flagsOut |= 0x2000;
    } else if (damage == 0) {
        *flagsOut = flags | 0x1E;
    } else if ((flags & 0x100) != 0 && ((special & 0x8000) != 0 || (target->attributes & 0x100) == 0)) {
        *flagsOut |= 0x10000;
    }
    return damage;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */


/* ========================================================================== */
/* MANUAL_AUTOMATION_STUBS_BEGIN main/battle/battle_damage */
/* Generated by object_stub_appender.py. Replace these with real decomp source. */

/* AUTOSTUB BattleSetStatusDamage size 0x107C */
/* signature source: manual_signatures */
s32 BattleSetStatusDamage(u32* result, BattleWorkUnit* unit, BattleWorkUnitPart* part,
                          u32 attackFlags, StatusEffectType status, int rate,
                          int galeFactor, char turns, char strength) {
    extern s32 irand(s32 max);
    extern s32 BtlUnit_CheckStatus(BattleWorkUnit*, StatusEffectType);
    extern void BtlUnit_GetStatus(BattleWorkUnit*, StatusEffectType, s8*, s8*);
    extern BOOL BtlUnit_SetStatus(BattleWorkUnit*, StatusEffectType, s8, s8);
    extern void BtlUnit_GetPos(BattleWorkUnit*, f32*, f32*, f32*);
    extern s32 BtlUnit_GetHeight(BattleWorkUnit*);
    extern void* effStampN64Entry(f32, f32, f32, s32);
    extern u32 _getRegistStatus(BattleWorkUnit*, StatusEffectType);
    s8 chargeStrength;
    s32 invalid = 0;
    s32 setResult = 0;
    u32 resistance;
    Vec position;
    void* effect;
    u8* effectWork;

    BtlUnit_GetStatus(unit, 0x10, NULL, &chargeStrength);
    switch ((u32)status) {
    case 0:
    case 0x13:
    case 0x14:
    case 0x15:
        if ((s8)turns < 1) invalid = 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if ((s8)turns == 0) invalid = 1;
        break;
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
        if ((s8)strength == 0) invalid = 1;
        break;
    }

    if ((*(u32*)((u8*)part + 0x1AC) & 0x20000000) != 0) return 0;
    if ((*(u32*)((u8*)part + 0x1AC) & 0x80000000) != 0) return 0;
    if (BtlUnit_CheckStatus(unit, 0) != 0 && invalid == 0) return 0;

    if ((*(u32*)((u8*)part + 0x1AC) & 0x40000000) != 0 && invalid == 0) {
        switch ((u32)status) {
        case 0: case 1: case 2: case 3: case 4: case 5:
        case 8: case 9: case 0xB: case 0xD: case 0xF:
        case 0x14: case 0x19: case 0x1A: case 0x1B:
            return 0;
        }
    }

    if (*(u8*)((s32)unit + 0x2F6) != 0) {
        switch ((u32)status) {
        case 1: case 2: case 3: case 4: case 5: case 6:
        case 0xB: case 0xD: case 0xF: case 0x1B:
            return 0;
        }
    }
    if ((u32)status == 8 && *(u8*)((s32)unit + 0x301) != 0) return 0;
    if (rate < 1) return 0;

    resistance = _getRegistStatus(unit, status) & 0xFF;
    if ((attackFlags & 0x100) == 0 || resistance == 0) {
        rate = rate * resistance / 100;
        if ((attackFlags & 0x100) == 0 && resistance != 0) {
            rate += galeFactor;
        }
    }
    if (irand(100) >= rate) return 0;

    if ((u32)status == 0x19) {
        *result |= 4;
    } else if ((u32)status == 0x1A) {
        *result |= 8;
    } else {
        if ((u32)status == 1) *result |= 0x20;
        if ((u32)status == 0x1B) *result |= 0x10;
        setResult = BtlUnit_SetStatus(unit, status, (s8)turns, (s8)strength);
    }

    switch ((u32)status) {
    case 6: case 7: case 0xA: case 0xC: case 0xE:
    case 0x10: case 0x12: case 0x13: case 0x15:
    case 0x16: case 0x17: case 0x18:
        *result |= 2;
        break;
    default:
        *result |= (s8)turns < 1 ? 2 : 1;
        break;
    }

    if (setResult != 0) {
        BtlUnit_GetPos(unit, &position.x, &position.y, &position.z);
        if ((*(u32*)((u8*)unit + 0x104) & 0x2000) == 0) {
            position.y += BtlUnit_GetHeight(unit) / 2;
        } else {
            position.y -= BtlUnit_GetHeight(unit) / 2;
        }
        if (*(s32*)((u8*)unit + 8) == 0x83 ||
            *(s32*)((u8*)unit + 8) == 0x94 ||
            *(s32*)((u8*)unit + 8) == 0xAA) {
            position.x -= 300.0f;
            position.z += 30.0f;
        } else if (*(s32*)((u8*)unit + 8) == 0x96) {
            position.z += 40.0f;
        }

        effect = NULL;
        switch ((u32)status) {
        case 0:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0xA0;
            effectWork[0x39] = 0xDC;
            effectWork[0x3A] = 0;
            effectWork[0x3B] = 10;
            effectWork[0x3C] = 10;
            effectWork[0x3D] = 10;
            break;
        case 1:
            effStampN64Entry(position.x, position.y, position.z + 10.0f, 0);
            break;
        case 2:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0xDC;
            effectWork[0x39] = 0x6E;
            effectWork[0x3A] = 0x6E;
            effectWork[0x3B] = 10;
            effectWork[0x3C] = 10;
            effectWork[0x3D] = 10;
            break;
        case 3:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0;
            effectWork[0x39] = 0x6E;
            effectWork[0x3A] = 0xDC;
            effectWork[0x3B] = 10;
            effectWork[0x3C] = 10;
            effectWork[0x3D] = 10;
            break;
        case 4:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0;
            effectWork[0x39] = 0xDC;
            effectWork[0x3A] = 0;
            effectWork[0x3B] = 10;
            effectWork[0x3C] = 10;
            effectWork[0x3D] = 10;
            break;
        case 5:
            effStampN64Entry(position.x, position.y, position.z + 10.0f, 1);
            break;
        case 6:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0xDC;
            effectWork[0x39] = 0xDC;
            effectWork[0x3A] = 0;
            effectWork[0x3B] = 10;
            effectWork[0x3C] = 10;
            effectWork[0x3D] = 10;
            break;
        case 7:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            effectWork = *(u8**)((u8*)effect + 0xC);
            effectWork[0x38] = 0xDC;
            effectWork[0x39] = 0xDC;
            effectWork[0x3A] = 0xDC;
            effectWork[0x3B] = 0;
            effectWork[0x3C] = 0xDC;
            effectWork[0x3D] = 0;
            break;
        case 8:
        case 9:
            effect = effStampN64Entry(position.x, position.y,
                                      position.z + 10.0f, 2);
            break;
        }
    }

    return setResult == 0 ? 1 : 1;
}

/* MANUAL_AUTOMATION_STUBS_END main/battle/battle_damage */
