#include "eff/eff_nice.h"
#include "eff/n64/eff_vapor_n64.h"
#include "version.h"
#include "mario/mario_pouch.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_ac.h"
#include "battle/battle_ac_help.h"
#include "eff/eff_bomb.h"
#include "eff/n64/eff_ice_n64.h"
#include "eff/eff_damage_star.h"
#include "eff/eff_recovery.h"
#include "battle/battle_audience.h"
#include "battle/battle_actrecord.h"
#include "battle/battle_damage.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_unit_event.h"
#include "battle/battle_database_common.h"
#include "eff/eff_hit.h"
#include "eff/eff_miss_star.h"
#include "eff/n64/eff_kemuri12_n64.h"
#include <string.h>
#include "battle/battle_mario.h"
#include "system.h"
#include "pmario_sound.h"
#include "eff/n64/eff_stamp_n64.h"
#include "battle/battle_disp.h"


void _getSickStatusParam(BattleUnit* unit, BattleWeapon* weapon, u32 type, s8* outTime, s8* outStrength);
s32 BattleSetStatusDamage(u32* result, BattleUnit* unit, BattleUnitPart* part, u32 special, s32 type, s32 rate, s32 levelDiff, s8 time, s8 strength);
void BattleDamageDirect(s32 unitId, BattleUnit* target, BattleUnitPart* part, s32 damage, s32 fpDamage, u32 result, u32 pattern, s32 flag);
u32 BattleCheckPikkyoro(BattleWeapon* weapon, u32* flags);
s32 BattleCalculateDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32* result, u32 flags);
s32 BattleCalculateFpDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32* resultFlags, u32 flags);
s32 BattleSetStatusDamageFromWeapon(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags);
s32 _checkDamageCode_EmergencyRevival(BattleUnit* unit, u32* flags);
void __declare(s32 unitIdx, s32 partIdx, s32* outUnit, s32* outPart);

EffectEntry* effFukidashiEntry(s32 a, s32 b, s32 c, s32 d, f32 x, f32 y, f32 z, f32 scale, f32 e);

void BattleDamageDirect(s32 unitId, BattleUnit* target, BattleUnitPart* part, s32 damage, s32 fpDamage, u32 result, u32 pattern, s32 flag) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    Vec pos;

    if (unitId != -5) {
        attacker = BattleGetUnitPtr(wp, unitId);
    } else {
        attacker = NULL;
    }
    if (damage > 99) {
        damage = 99;
    } else if (damage < -99) {
        damage = -99;
    }
    if (attacker) {
        attacker->totalDamageDealt += damage;
    }
    if (part) {
        target->currentTarget = part;
    } else {
        part = target->currentTarget = BtlUnit_GetPartsPtr(target, BtlUnit_GetBodyPartsId(target));
    }
    target->damagePattern = pattern;
    if (target->mAttributeFlags & kUnitNoSpin) {
        if ((u32)target->damagePattern == ATTACKPATTERN_CHUCHURINA_BINTA_L || (u32)target->damagePattern == ATTACKPATTERN_CHUCHURINA_BINTA_R ||
            (u32)target->damagePattern == ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST || (u32)target->damagePattern == ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST) {
            target->damagePattern = 0;
        }
    }
    BtlUnit_GetHitPos(target, part, &pos.x, &pos.y, &pos.z);

    if (!(result & 0x2000)) {
        if (damage != 0 || fpDamage != 0) {
            f32 x, y, z;
            BtlUnit_GetHitPos(target, part, &x, &y, &z);
            switch ((u8)result) {
                case DAMAGE_CODE_FIRE:
                case DAMAGE_CODE_EXPLOSION:
                    effKemuri12N64Entry(0, 0x1e, x, y, z, 1.0f);
                    effHitEntry(1, 3, x, y, z, 1.0f);
                    break;
                case DAMAGE_CODE_ELECTRIC:
                    effHitEntry(2, 3, x, y, z, 1.0f);
                    break;
                case DAMAGE_CODE_ICE:
                    effIceN64Entry(0, x, y, z);
                    effHitEntry(3, 3, x, y, z, 1.0f);
                    break;
                default:
                    effHitEntry(0, 3, x, y, z, 1.0f);
                    break;
            }
            if (BtlUnit_GetBelong(target) == 0) {
                if (damage != 0 && !(target->mAttributeFlags & kNoDamageNumber)) {
                    effDamageStarEntry(0, damage, x - 8.0f, 5.0f + y, z, 10.0f, 30.0f);
                }
                if (fpDamage != 0) {
                    if (damage != 0) {
                        effDamageStarEntry(1, fpDamage, x - 8.0f, 5.0f + y, z, 10.0f, 60.0f);
                    } else {
                        effDamageStarEntry(1, fpDamage, x - 8.0f, 5.0f + y, z, 10.0f, 30.0f);
                    }
                }
            } else {
                if (damage != 0 && !(target->mAttributeFlags & kNoDamageNumber)) {
                    effDamageStarEntry(0, damage, x, 10.0f + y, z, 10.0f, -30.0f);
                }
                if (fpDamage != 0) {
                    if (damage != 0) {
                        effDamageStarEntry(1, fpDamage, x, 10.0f + y, z, 10.0f, -60.0f);
                    } else {
                        effDamageStarEntry(1, fpDamage, x, 10.0f + y, z, 10.0f, -30.0f);
                    }
                }
            }
            if (unitId != -5 && attacker && attacker->badgesEquipped.MegaRush &&
                attacker->currentHp <= attacker->mKindParams->mPerilHP) {
                f32 x2, y2, z2;
                BtlUnit_GetHitPos(target, part, &x2, &y2, &z2);
                effFukidashiEntry(0, 0, 0, 0x3c, x2, y2, z2, 10.0f, 0.0f);
            }
        } else {
            f32 x, y, z;
            BtlUnit_GetHitPos(target, part, &x, &y, &z);
            if (pattern != 2 && pattern != 3 && pattern != 0x15 && pattern != 0x16 && pattern != 0x14 &&
                pattern != 0x13 && pattern != 0x1b && pattern != 7 && (u8)result != 0x16) {
                s32 dir = BtlUnit_GetBelong(target) == 0 ? -1 : 1;
                if (result & 0x20000) {
                    effMissStarEntry(1, 1, x, y, z, dir);
                } else {
                    effMissStarEntry(0, 1, x, y, z, dir);
                }
            }
        }
    } else {
        f32 x, y, z;
        BtlUnit_GetHitPos(target, part, &x, &y, &z);
        effRecoveryEntry(0, __fabs(damage), x, y, z);
    }

    if (!(result & 0x2000) && damage > 0) {
        s32 n = 0;
        s32 i = target->badgesEquipped.PityFlower;
        for (; i > 0; i--) {
            if (irand(100) < 30) {
                n++;
            }
        }
        if (n > 0) {
            f32 x, y, z;
            BtlUnit_RecoverFp(target, n);
            BtlUnit_GetPos(target, &x, &y, &z);
            y += target->mSizeMultiplier * BtlUnit_GetHeight(target) + 5.0f;
            effRecoveryEntry(1, n, x, y, z);
        }
    }

    if (pattern != 0x13 && pattern != 0x1b) {
        if (result & 0x1000000) {
            psndSFXOn_3D("SFX_MARIO_HAMMER_PIKKYO_R1", &pos);
        } else if (result & 0x2000000) {
            psndSFXOn_3D("SFX_MARIO_HAMMER_PIKKYO_Y1", &pos);
        } else if (result & 0x4000000) {
            psndSFXOn_3D("SFX_MARIO_HAMMER_PIKKYO_B1", &pos);
        } else if (result & 0x8000000) {
            psndSFXOn_3D("SFX_MARIO_HAMMER_PIKKYO_G1", &pos);
        } else if (result & 0x10000000) {
            psndSFXOn_3D("SFX_MARIO_HAMMER_PIKKYO_P1", &pos);
        } else if (BtlUnit_GetBelong(target) == 0 && (result & 0x40000)) {
            psndSFXOn_3D("SFX_MARIO_GUARD1", &pos);
        } else {
            if (damage > 0) {
                switch ((u8)result) {
                    case DAMAGE_CODE_FIRE:
                        psndSFXOn_3D(target->mKindParams->fireDamageSfxName, &pos);
                        psndSFXOn_3D(target->mKindParams->damageSfxName, &pos);
                        break;
                    case DAMAGE_CODE_ICE:
                        psndSFXOn_3D(target->mKindParams->iceDamageSfxName, &pos);
                        psndSFXOn_3D(target->mKindParams->damageSfxName, &pos);
                        break;
                    case DAMAGE_CODE_ELECTRIC:
                        psndSFXOn_3D(target->mKindParams->electricDamageSfxName, &pos);
                        psndSFXOn_3D(target->mKindParams->damageSfxName, &pos);
                        break;
                    default:
                        psndSFXOn_3D(target->mKindParams->damageSfxName, &pos);
                        break;
                }
            }
            if (fpDamage > 0 && BtlUnit_GetBelong(target) == 0) {
                psndSFXOn_3D("SFX_MARIO_DAMAGE_FP1", &pos);
            }
        }
    }

    target->hpDamageTaken = damage;
    target->totalHpDamageTaken += target->hpDamageTaken;
    target->fpDamageTaken = fpDamage;
    target->fpLost = fpDamage;
    target->totalFpDamageTaken += target->fpDamageTaken;
    if (!(target->mAttributeFlags & kUnitHpFixed) && !(part->attributes & kNoHpLoss)) {
        target->hpGauge.prevHp = target->currentHp;
        target->currentHp -= target->hpDamageTaken;
        if (target->currentHp > target->maxHp) {
            target->currentHp = target->maxHp;
        }
        if (target->currentHp < 0) {
            target->currentHp = 0;
        }
        target->hpGauge.targetHp = target->currentHp;
        if (target->hpDamageTaken) {
            target->hpGauge.drainTimer = target->hpGauge.drainLength;
            target->hpGauge.visibleTimer = target->hpGauge.visibleLength;
        }
        if (result & 0x100) {
            target->hpGauge.flags |= 1;
        } else {
            target->hpGauge.flags &= ~1;
        }
    }
    if (target->fpDamageTaken) {
        s32 fp = BtlUnit_GetFp(target);
        s32 newFp;
        if (fp < fpDamage) {
            target->fpLost = fp;
        } else {
            target->fpLost = fpDamage;
        }
        newFp = fp - fpDamage;
        if (newFp < 0) {
            newFp = 0;
        }
        BtlUnit_SetFp(target, newFp);
        target->totalFpLost += target->fpLost;
    }
    if (damage > 0) {
        if (target->currentKind == UNIT_MARIO) {
            BtlActRec_AddCount(&wp->actRecord.marioHitsTaken);
            BtlActRec_AddPoint(&wp->actRecord.marioDamageTaken, damage);
        } else if (target->currentKind >= TYPE_PARTNER_MIN && target->currentKind < TYPE_PARTNER_MAX) {
            BtlActRec_AddCount(&wp->actRecord.partnerHitsTaken);
            BtlActRec_AddPoint(&wp->actRecord.partnerDamageTaken, damage);
        }
    }
    if (flag && (damage > 0 || fpDamage > 0) && BtlUnit_CheckStatus(target, STATUS_SLEEP) && irand(100) < 50) {
        f32 x, y, z;
        target->mSleepTurns = 1;
        target->mSleepStrength = 100;
        BtlUnit_CheckRecoveryStatus(target, STATUS_SLEEP);
        BtlUnit_GetPos(target, &x, &y, &z);
        y += BtlUnit_GetHeight(target) / 2;
        effFukidashiEntry(0, 0, 0, 0x3c, x, y, z, 10.0f, 0.0f);
    }
    if (target->currentKind == UNIT_MARIO) {
        BattleAudience_Case_MarioBigDamage(target->hpDamageTaken);
    }
    if (target->currentHp <= 0) {
        target->currentHp = 0;
        if (result & 0x100) {
            result |= DAMAGE_RESULT_ZERO_HP;
            if (target->mAttributeFlags & kUndead) {
                switch ((u8)result) {
                    case DAMAGE_CODE_FIRE:
                    case DAMAGE_CODE_EXPLOSION:
                    case DAMAGE_CODE_COUNTER_FIERY:
                    case DAMAGE_CODE_COUNTER_EXPLOSIVE:
                    case DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE:
                    case DAMAGE_CODE_COUNTER_HOLD_FAST:
                        result |= DAMAGE_RESULT_DEATH;
                        break;
                }
            } else {
                result |= DAMAGE_RESULT_DEATH;
                _checkDamageCode_EmergencyRevival(target, &result);
            }
            if ((u32)target->damagePattern == ATTACKPATTERN_BLOW) {
                s32 b = 1;
                if ((result & 0x400) && !(result & 0x800) && (result & 0x100) && target->vulnStatus->galeForceRate) {
                    b = 0;
                }
                if (b) {
                    target->damagePattern = 0;
                }
            }
            if (result & 0x400) {
                if (target->currentKind >= TYPE_PARTNER_MIN && target->currentKind < TYPE_PARTNER_MAX) {
                    BattleAudience_Case_PartyDown();
                }
                if (target->currentKind >= UNIT_GOOMBA && target->currentKind < UNIT_UNUSED_TREE) {
                    BattleAudience_Case_KillEnemy(target->mLevel);
                    BattleAudience_Case_FinalAttack();
                }
            }
        }
    } else if ((u32)target->damagePattern == ATTACKPATTERN_BLOW) {
        target->damagePattern = 0;
    }
    target->mTokenFlags |= 1;
    BattleRunHitEvent(target, result);
}

u32 BattleCheckPikkyoro(BattleWeapon* weapon, u32* flags) {
    BattleWork* wp = _battleWorkPointer;
    s32 table[5] = {0};
    s32 count = 0;
    s32 idx = 0;
    u32 result = 0;
    u32 f;
    s32 r;

    if (flags) {
        result = *flags;
    }
    if (weapon->special & SPECIAL_ATTACK_FX) {
        f = (u32)wp->badgeEquippedFlags;
        if (f & 0x100) {
            table[0] = 1;
            idx = 0;
            count++;
        }
        if (f & 0x200) {
            table[1] = 1;
            idx = 1;
            count++;
        }
        if (f & 0x400) {
            table[2] = 1;
            idx = 2;
            count++;
        }
        if (f & 0x800) {
            table[3] = 1;
            idx = 3;
            count++;
        }
        if (f & 0x1000) {
            table[4] = 1;
            idx = 4;
            count++;
        }
    }
    if (count > 0) {
        if (count >= 2) {
            r = irand(count);
            for (idx = 4; idx > 0; idx--) {
                r -= table[idx];
                if (r < 0) {
                    break;
                }
            }
        }
        switch (idx) {
            case 0:
                result |= 0x1000000;
                break;
            case 1:
                result |= 0x2000000;
                break;
            case 2:
                result |= 0x4000000;
                break;
            case 3:
                result |= 0x8000000;
                break;
            case 4:
                result |= 0x10000000;
                break;
        }
    }
    if (flags) {
        *flags = result;
    }
    return result;
}

s32 BattleCalculateDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32* result, u32 flags) {
    u32 defaultResult;
    BattleWork* wp = _battleWorkPointer;
    s32 damage;
    s32 defense;
    s32 defenseAttr;
    u8 element;
    s32 n;
    u32 badgePower;
    s8 turns;
    s8 strength;
    s32 (*fn)(BattleUnit*, BattleWeapon*, BattleUnit*, BattleUnitPart*);

    if (target->mTokenFlags & 2) {
        flags |= 0x1000;
    }
    element = weapon->element;
    if (attacker && (weapon->special & SPECIAL_IGNITABLE) && BtlUnit_CheckStatus(attacker, STATUS_BURN)) {
        element = ELEMENT_FIRE;
    }
    defaultResult = flags | DAMAGE_CODE_NO_DAMAGE;
    *result = defaultResult;
    fn = (s32 (*)(BattleUnit*, BattleWeapon*, BattleUnit*, BattleUnitPart*))weapon->hpDamageFn;
    if (fn == NULL) {
        return 0;
    }
    if (part->attributes & kImmuneToDamageAndStatus) {
        return 0;
    }
    if (attacker && (weapon->special & SPECIAL_BADGE_POWER) && attacker->badgesEquipped.AllOrNothing &&
        !((u32)wp->actionCommands.result & 2) && !(flags & 0x20000)) {
        return 0;
    }
    damage = weapon->hpDamageWork[0];
    if (fn) {
        damage = fn(attacker, weapon, target, part);
    }
    if (wp->majinaiState == 1 && (weapon->special & SPECIAL_STATUS_POWER)) {
        damage += 3;
    }
    if (weapon->special & SPECIAL_BADGE_POWER) {
        damage += attacker->badgesEquipped.PowerPlus;
        damage += attacker->badgesEquipped.PUpDDown;
        if (attacker->badgesEquipped.AllOrNothing) {
            if (((u32)wp->actionCommands.result & 2) || (flags & 0x20000)) {
                damage += attacker->badgesEquipped.AllOrNothing;
            } else {
                damage = 0;
                goto skip;
            }
        }
        if (attacker->currentHp <= attacker->mKindParams->mPerilHP && attacker->badgesEquipped.MegaRush) {
            damage += attacker->badgesEquipped.MegaRush * 5;
        }
        if (attacker->currentHp <= attacker->mKindParams->mDangerHP && attacker->badgesEquipped.PowerRush) {
            damage += attacker->badgesEquipped.PowerRush * 2;
        }
        if (attacker->badgesEquipped.IcePower && (part->attributes & kWeakToIcePower)) {
            damage += attacker->badgesEquipped.IcePower;
        }
    }
    if (weapon->special & SPECIAL_CHARGEABLE) {
        if (BtlUnit_CheckStatus(attacker, STATUS_CHARGE)) {
            BtlUnit_GetStatus(attacker, STATUS_CHARGE, &turns, &strength);
            if ((damage += strength) < 0) {
                damage = 0;
            }
        }
        attacker->mTokenFlags |= kChargeExpended;
    }
    if ((weapon->special & SPECIAL_STATUS_POWER) && BtlUnit_CheckStatus(attacker, STATUS_HUGE)) {
        BtlUnit_GetStatus(attacker, STATUS_HUGE, &turns, &strength);
        if ((damage += strength) < 0) {
            damage = 0;
        }
    }
    if ((weapon->special & SPECIAL_STATUS_POWER) && BtlUnit_CheckStatus(attacker, STATUS_ATTACK_UP)) {
        BtlUnit_GetStatus(attacker, STATUS_ATTACK_UP, &turns, &strength);
        if ((damage += strength) < 0) {
            damage = 0;
        }
    }
    badgePower = (weapon->special & SPECIAL_BADGE_POWER) != 0;
    if (badgePower) {
        damage -= attacker->badgesEquipped.PDownDUp;
    }
    if (badgePower && attacker->badgesEquipped.HpDrain) {
        damage -= attacker->badgesEquipped.HpDrain;
    }
    if (badgePower && attacker->badgesEquipped.FpDrain) {
        damage -= attacker->badgesEquipped.FpDrain;
    }
    if ((weapon->special & SPECIAL_STATUS_POWER) && BtlUnit_CheckStatus(attacker, STATUS_TINY)) {
        BtlUnit_GetStatus(attacker, STATUS_TINY, &turns, &strength);
        if ((damage += strength) < 0) {
            damage = 0;
        }
    }
    if ((weapon->special & SPECIAL_STATUS_POWER) && BtlUnit_CheckStatus(attacker, STATUS_ATTACK_DOWN)) {
        BtlUnit_GetStatus(attacker, STATUS_ATTACK_DOWN, &turns, &strength);
        if ((damage += strength) < 0) {
            damage = 0;
        }
    }
    if (damage < 0) {
        damage = 0;
    }
skip:
    defense = part->defense->defenses[element];
    defenseAttr = part->defenseAttr->defenseAttrs[element];
    switch (element) {
        case ELEMENT_FIRE:
            *result = flags | DAMAGE_CODE_FIRE;
            break;
        case ELEMENT_ICE:
            *result = flags | DAMAGE_CODE_ICE;
            break;
        case ELEMENT_ELECTRIC:
            *result = flags | DAMAGE_CODE_ELECTRIC;
            break;
        case ELEMENT_EXPLOSION:
            *result = flags | DAMAGE_CODE_EXPLOSION;
            break;
        case ELEMENT_NORMAL:
        default:
            *result = flags | DAMAGE_CODE_NORMAL;
            break;
    }
    if (defenseAttr != DEFENSE_ATTR_ABSORB) {
        if (target->badgesEquipped.DefendPlus) {
            defense += target->badgesEquipped.DefendPlus;
        }
        if (target->badgesEquipped.DamageDodge && (flags & 0x40000)) {
            defense += target->badgesEquipped.DamageDodge;
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
            if ((defense += strength) < 0) {
                defense = 0;
            }
        }
        if (wp->majinaiState == 2 && target->currentKind == UNIT_MARIO) {
            defense += 3;
        }
        if (part->attributes & VERSION_PICK(0x20, 0x80)) {
            if ((defense += target->work[1]) < 0) {
                defense = 0;
            }
        }
        if ((weapon->special & SPECIAL_PIERCE_DEFENSE) && defense > 0) {
            defense = 0;
        }
    }
    damage = (defenseAttr == DEFENSE_ATTR_ABSORB) ? damage : damage - defense;
    if ((damage += target->badgesEquipped.PUpDDown) > 0) {
        if (weapon->special & SPECIAL_DIMINISH_BY_HIT) {
            if ((damage -= target->hpDamageCount) <= 0) {
                damage = 1;
            }
        }
        if (weapon->special & SPECIAL_DIMINISH_BY_TARGET) {
            if ((damage -= attacker->superguardedCount) <= 0) {
                damage = 1;
            }
        }
    }
    if (flags & 0x40000) {
        damage--;
    }
    if (element == ELEMENT_FIRE && target->badgesEquipped.IcePower) {
        damage -= target->badgesEquipped.IcePower;
    }
    damage -= target->badgesEquipped.PDownDUp;
    if (target->badgesEquipped.DoublePain) {
        damage *= target->badgesEquipped.DoublePain + 1;
    }
    if (target->badgesEquipped.LastStand && target->currentHp <= target->mKindParams->mDangerHP) {
        n = target->badgesEquipped.LastStand + 1;
        damage = (damage + n - 1) / n;
    }
    switch (defenseAttr) {
        case DEFENSE_ATTR_WEAK:
            damage++;
            break;
        case DEFENSE_ATTR_IMMUNE:
            damage = 0;
            break;
        case DEFENSE_ATTR_IMMUNE_EXCEPT_GULP:
            if (!(flags & 0x80000000)) {
                damage = 0;
            }
            break;
        case DEFENSE_ATTR_ABSORB:
            break;
    }
    if (damage < 0) {
        damage = 0;
    }
    if (defenseAttr == DEFENSE_ATTR_ABSORB) {
        *result |= 0x2000;
        return (u32)damage * -1;
    }
    if (damage <= 0) {
        *result = defaultResult;
        return 0;
    }
    if (flags & 0x100) {
        if ((weapon->special & 0x8000) || !(target->mAttributeFlags & 0x100)) {
            *result |= 0x10000;
        }
    }
    return damage;
}

s32 BattleCalculateFpDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32* resultFlags, u32 flags) {
    BattleWork* wp = _battleWorkPointer;
    s32 damage = 0;
    s32 reduce = 0;
    s32 (*fn)(BattleUnit*, BattleWeapon*, BattleUnit*, BattleUnitPart*);

    if (*resultFlags & 0x2000) {
        return 0;
    }
    fn = (s32 (*)(BattleUnit*, BattleWeapon*, BattleUnit*, BattleUnitPart*))weapon->fpDamageFn;
    if (fn == NULL) {
        return 0;
    }
    if (part->attributes & kImmuneToDamageAndStatus) {
        return 0;
    }
    if (attacker && (weapon->special & SPECIAL_BADGE_POWER) && attacker->badgesEquipped.AllOrNothing &&
        !((u32)wp->actionCommands.result & 2)) {
        return 0;
    }
    if (fn) {
        damage = fn(attacker, weapon, target, part);
    }
    if (flags & 0x40000) {
        reduce = 1;
    }
    damage -= reduce;
    if (damage < 0) {
        damage = 0;
    }
    if (damage > 0 && (*resultFlags & 0xFF) == 0x1E) {
        *resultFlags = (*resultFlags & ~0xFF) | 0x17;
    }
    return damage;
}

s32 _getSickStatusRate(BattleWeapon* weapon, s32 type) {
    BattleWeapon* w = weapon;
    switch (type) {
        case 1: return w->sleepChance;
        case 2: return w->stopChance;
        case 3: return w->dizzyChance;
        case 4: return w->poisonChance;
        case 5: return w->confuseChance;
        case 6: return w->electricChance;
        case 7: return w->dodgyChance;
        case 8: return w->burnChance;
        case 9: return w->freezeChance;
        case 10:
            if (w->sizeChangeStrength > 0) {
                return w->sizeChangeChance;
            }
            return 0;
        case 11:
            if (w->sizeChangeStrength <= 0) {
                return w->sizeChangeChance;
            }
            return 0;
        case 12:
            if (w->atkChangeStrength > 0) {
                return w->atkChangeChance;
            }
            return 0;
        case 13:
            if (w->atkChangeStrength <= 0) {
                return w->atkChangeChance;
            }
            return 0;
        case 14:
            if (w->defChangeStrength > 0) {
                return w->defChangeChance;
            }
            return 0;
        case 15:
            if (w->defChangeStrength <= 0) {
                return w->defChangeChance;
            }
            return 0;
        case 0: return w->allergicChance;
        case 19: return w->fastChance;
        case 20: return w->slowChance;
        case 25: return w->frightChance;
        case 26: return w->galeForceChance;
        case 27: return (u8)w->ohkoChance;
        case 18: return w->invisibleChance;
        case 23: return w->hpRegenTime > 0 ? 100 : 0;
        case 24: return w->fpRegenTime > 0 ? 100 : 0;
        case 16: return w->chargeStrength != 0 ? 100 : 0;
        case 21: return w->paybackTime != 0 ? 100 : 0;
        case 22: return w->holdFastTime > 0 ? 100 : 0;
        case 17: return 0;
    }
    return 0;
}

void _getSickStatusParam(BattleUnit* unit, BattleWeapon* weapon, u32 type, s8* outTime, s8* outStrength) {
    BattleWeapon* w = weapon;
    s32 time;
    s32 strength;
    s32 badge;
    s32 n;

    switch (type) {
        case 1:
            time = strength = w->sleepTime;
            break;
        case 2:
            time = strength = w->stopTime;
            break;
        case 3:
            time = strength = w->dizzyTime;
            break;
        case 4:
            time = w->poisonTime;
            strength = w->poisonStrength;
            break;
        case 5:
            time = strength = w->confuseTime;
            break;
        case 6:
            time = strength = w->electricTime;
            break;
        case 7:
            time = strength = w->dodgyTime;
            break;
        case 8:
            time = strength = w->burnTime;
            break;
        case 9:
            time = strength = w->freezeTime;
            break;
        case 10:
            if (w->sizeChangeStrength > 0) {
                time = w->sizeChangeTime;
                strength = w->sizeChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 11:
            if (w->sizeChangeStrength < 0) {
                time = w->sizeChangeTime;
                strength = w->sizeChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 12:
            if (w->atkChangeStrength > 0) {
                time = w->atkChangeTime;
                strength = w->atkChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 13:
            if (w->atkChangeStrength < 0) {
                time = w->atkChangeTime;
                strength = w->atkChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 14:
            if (w->defChangeStrength > 0) {
                time = w->defChangeTime;
                strength = w->defChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 15:
            if (w->defChangeStrength < 0) {
                time = w->defChangeTime;
                strength = w->defChangeStrength;
            } else {
                strength = 0;
                time = 0;
            }
            break;
        case 0:
            time = strength = w->allergicTime;
            break;
        case 19:
            time = strength = w->fastTime;
            break;
        case 20:
            time = strength = w->slowTime;
            break;
        case 27:
            time = strength = w->ohkoChance;
            break;
        case 18:
            time = strength = w->invisibleTime;
            break;
        case 23:
            time = w->hpRegenTime;
            strength = w->hpRegenStrength;
            break;
        case 24:
            time = w->fpRegenTime;
            strength = w->fpRegenStrength;
            break;
        case 16:
            time = w->chargeStrength;
            strength = w->chargeStrength;
            break;
        case 21:
            time = strength = w->paybackTime;
            break;
        case 22:
            time = strength = w->holdFastTime;
            break;
        default:
            strength = 0;
            time = 0;
            break;
    }

    badge = weapon->itemId;
    n = 0;
    if (badge >= 0xF0 && badge < 0x153) {
        n = pouchEquipCheckBadge(badge);
    }
    if (n > 0) {
        switch (badge) {
            case 0xF4:
                if (type == 11) {
                    time += (n - 1) * 2;
                }
                break;
            case 0xF5:
                if (type == 1) {
                    time += (n - 1) * 2;
                }
                break;
            case 0xF6:
                if (type == 15) {
                    time += (n - 1) * 2;
                }
                break;
            case 0xFB:
                if (type == 5) {
                    time += (n - 1) * 2;
                }
                break;
            case 0xFD:
                if (type == 9) {
                    time += (n - 1) * 2;
                }
                break;
            case 0x14F:
            case 0x150:
                if (type == 14 && unit->badgesEquipped.DefendUnused >= 2) {
                    strength += unit->badgesEquipped.DefendUnused - 1;
                }
                break;
        }
    }
    if (type == 16) {
        if (weapon == &badgeWeapon_Charge || weapon == &badgeWeapon_ChargeP) {
            if (unit->currentKind == UNIT_MARIO) {
                n = pouchEquipCheckBadge(ITEM_CHARGE);
            } else {
                n = pouchEquipCheckBadge(ITEM_CHARGE_P);
            }
            if (n >= 2) {
                strength += n - 1;
            }
        } else if (weapon == &badgeWeapon_SuperCharge || weapon == &badgeWeapon_SuperChargeP) {
            if (unit->currentKind == UNIT_MARIO) {
                n = pouchEquipCheckBadge(ITEM_SUPER_CHARGE);
            } else {
                n = pouchEquipCheckBadge(ITEM_SUPER_CHARGE_P);
            }
            if (n >= 2) {
                strength += (n - 1) * 2;
            }
        }
        if (strength >= 10) {
            strength = 9;
        }
    }
    *outTime = time;
    *outStrength = strength;
}

u8 _getRegistStatus(BattleUnit* unit, s32 type) {
    BattleVulnerableStatus* v = unit->vulnStatus;
    switch (type) {
        case 1: return v->sleepRate;
        case 2: return v->stopRate;
        case 3: return v->dizzyRate;
        case 4: return v->poisonRate;
        case 5: return v->confuseRate;
        case 6: return v->electricRate;
        case 8: return v->burnRate;
        case 9: return v->freezeRate;
        case 10: return v->hugeRate;
        case 11: return v->tinyRate;
        case 12: return v->attackUpRate;
        case 13: return v->attackDownRate;
        case 14: return v->defenseUpRate;
        case 15: return v->defenseDownRate;
        case 0: return v->allergicRate;
        case 27: return v->instaKillRate;
        case 19: return v->fastRate;
        case 20: return v->slowRate;
        case 25: return v->frightRate;
        case 26: return v->galeForceRate;
        case 7: return v->dodgyRate;
        case 18: return v->invisibleRate;
    }
    return 100;
}

EffectEntry* effUpdownEntry(s32 type, s32 strength, s32 time, f32 x, f32 y, f32 z);
void BattleStatusChangeInfoSetAnnouce(BattleUnit* unit, s32 type, s32 value, s32 flag);
void BattleStatusChangeMsgSetAnnouce(BattleUnit* unit, s32 type, s32 flag);

#define STAMP_COL(e, r0, g0, b0, r1, g1, b1)                 \
    ((EffStampN64Work*)(e)->userdata)->r = (r0);                  \
    ((EffStampN64Work*)(e)->userdata)->g = (g0);                  \
    ((EffStampN64Work*)(e)->userdata)->b = (b0);                  \
    ((EffStampN64Work*)(e)->userdata)->r2 = (r1);                  \
    ((EffStampN64Work*)(e)->userdata)->g2 = (g1);                  \
    ((EffStampN64Work*)(e)->userdata)->b2 = (b1)

s32 BattleSetStatusDamage(u32* result, BattleUnit* unit, BattleUnitPart* part, u32 special, s32 type, s32 rate, s32 levelDiff, s8 time, s8 strength) {
    s32 ok;
    s32 announce;
    s32 chance;
    s32 regist;
    f32 x, y, z;
    EffectEntry* eff;
    Vec hitPos;
    s8 prevStrength;
    s8 curTurns;
    s8 curStrength;

    ok = 0;
    BtlUnit_GetStatus(unit, STATUS_CHARGE, NULL, &prevStrength);
    switch (type) {
        case STATUS_HOLD_FAST:
        case STATUS_HP_REGEN:
        case STATUS_FP_REGEN:
            break;
        case STATUS_FAST:
        case STATUS_SLOW:
            if (time <= 0) {
                ok = 1;
            }
            break;
        case STATUS_ALLERGIC:
        case STATUS_PAYBACK:
            if (time <= 0) {
                ok = 1;
            }
            break;
        case STATUS_HUGE:
        case STATUS_TINY:
        case STATUS_ATTACK_UP:
        case STATUS_ATTACK_DOWN:
        case STATUS_DEFENSE_UP:
        case STATUS_DEFENSE_DOWN:
            if (strength == 0) {
                ok = 1;
            }
            break;
        case STATUS_SLEEP:
        case STATUS_STOP:
        case STATUS_DIZZY:
        case STATUS_POISON:
        case STATUS_CONFUSE:
        case STATUS_ELECTRIC:
        case STATUS_DODGY:
        case STATUS_BURN:
        case STATUS_FREEZE:
            if (time == 0) {
                ok = 1;
            }
            break;
    }
    if (part->attributes & kImmuneToDamageAndStatus) {
        return 0;
    }
    if (part->attributes & kImmuneToStatus) {
        return 0;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_ALLERGIC) && !ok) {
        return 0;
    }
    if (part->attributes & kImmuneToNegativeStatus) {
        switch (type) {
            case STATUS_ALLERGIC:
            case STATUS_SLEEP:
            case STATUS_STOP:
            case STATUS_DIZZY:
            case STATUS_POISON:
            case STATUS_CONFUSE:
            case STATUS_BURN:
            case STATUS_FREEZE:
            case STATUS_TINY:
            case STATUS_ATTACK_DOWN:
            case STATUS_DEFENSE_DOWN:
            case STATUS_SLOW:
            case STATUS_FRIGHT:
            case STATUS_GALE_FORCE:
            case STATUS_INSTANT_KILL:
                if (!ok) {
                    return 0;
                }
                break;
        }
    }
    if (unit->badgesEquipped.FeelingFine) {
        switch (type) {
            case STATUS_SLEEP:
            case STATUS_STOP:
            case STATUS_DIZZY:
            case STATUS_POISON:
            case STATUS_CONFUSE:
            case STATUS_ELECTRIC:
            case STATUS_TINY:
            case STATUS_ATTACK_DOWN:
            case STATUS_DEFENSE_DOWN:
            case STATUS_INSTANT_KILL:
                return 0;
        }
    }
    if ((u32)type == 8 && unit->badgesEquipped.IcePower) {
        return 0;
    }
    chance = rate;
    if (chance <= 0) {
        return 0;
    }
    regist = _getRegistStatus(unit, type);
    if (!(special & SPECIAL_IGNORE_VULNERABILITY) || regist == 0) {
        chance = chance * regist / 100;
        if (!(special & SPECIAL_IGNORE_VULNERABILITY) && regist > 0) {
            chance += levelDiff;
        }
    }
    if (irand(100) >= chance) {
        return 0;
    }

    ok = 0;
    if ((u32)type == 0x19) {
        *result |= 4;
    } else if ((u32)type == 0x1a) {
        *result |= 8;
    } else {
        if ((u32)type == 1) {
            *result |= 0x20;
        }
        if ((u32)type == 0x1b) {
            *result |= 0x10;
        }
        ok = BtlUnit_SetStatus(unit, type, time, strength);
    }
    switch (type) {
        case STATUS_ELECTRIC:
        case STATUS_DODGY:
        case STATUS_HUGE:
        case STATUS_ATTACK_UP:
        case STATUS_DEFENSE_UP:
        case STATUS_CHARGE:
        case STATUS_INVISIBLE:
        case STATUS_FAST:
        case STATUS_PAYBACK:
        case STATUS_HOLD_FAST:
        case STATUS_HP_REGEN:
        case STATUS_FP_REGEN:
            *result |= 2;
            break;
        default:
            if (time > 0) {
                *result |= 1;
            } else {
                *result |= 2;
            }
            break;
    }

    if (ok) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        if (unit->mAttributeFlags & kCeiling) {
            y -= BtlUnit_GetHeight(unit) / 2;
        } else {
            y += BtlUnit_GetHeight(unit) / 2;
        }
        switch (unit->currentKind) {
            case 0x17:
            case 0x84:
            case 0xab:
                x -= 300.0f;
                z += 30.0f;
                break;
#ifdef VERSION_G8ME01
            case 0x95:
            case 0x96:
                z += 40.0f;
                break;
#endif
        }
        switch (type) {
            case STATUS_SLEEP:
                effStampN64Entry(0, x, y, 10.0f + z);
                break;
            case STATUS_CONFUSE:
                effStampN64Entry(1, x, y, 10.0f + z);
                break;
            case STATUS_STOP:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 110, 110, 10, 10, 10);
                break;
            case STATUS_DIZZY:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 0, 110, 220, 10, 10, 10);
                break;
            case STATUS_POISON:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 0, 220, 0, 10, 10, 10);
                break;
            case STATUS_ELECTRIC:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 0, 10, 10, 10);
                break;
            case STATUS_CHARGE:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 220, 220, 0);
                break;
            case STATUS_PAYBACK:
            case STATUS_HOLD_FAST:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 0, 0, 0, 220);
                break;
            case STATUS_HUGE:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 220, 0, 0);
                break;
            case STATUS_TINY:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 0, 0, 10, 10, 10);
                break;
            case STATUS_ATTACK_UP:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 220, 0, 0);
                break;
            case STATUS_ATTACK_DOWN:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 0, 0, 10, 10, 10);
                break;
            case STATUS_DEFENSE_UP:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 0, 0, 220);
                break;
            case STATUS_DEFENSE_DOWN:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 0, 0, 220, 10, 10, 10);
                break;
            case STATUS_FAST:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 220, 110, 0);
                break;
            case STATUS_SLOW:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 110, 0, 10, 10, 10);
                break;
            case STATUS_BURN:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 0, 220, 0, 0);
                break;
            case STATUS_FREEZE:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 0, 110, 220);
                break;
            case STATUS_ALLERGIC:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 160, 220, 0, 10, 10, 10);
                break;
            case STATUS_INVISIBLE:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 110, 110, 110);
                break;
            case STATUS_HP_REGEN:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 220, 110, 110);
                break;
            case STATUS_FP_REGEN:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 160, 220, 0);
                break;
            case STATUS_DODGY:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 0, 220, 0);
                break;
            case STATUS_INSTANT_KILL:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 220, 220, 220, 10, 10, 10);
                break;
            default:
                eff = effStampN64Entry(2, x, y, 10.0f + z);
                STAMP_COL(eff, 200, 200, 200, 10, 10, 10);
                break;
        }
        switch (type) {
            case STATUS_DEFENSE_UP:
            case STATUS_DEFENSE_DOWN:
                effUpdownEntry(1, strength, 0x3c, x, y, 20.0f + z);
                break;
            case STATUS_HUGE:
            case STATUS_TINY:
            case STATUS_ATTACK_UP:
            case STATUS_ATTACK_DOWN:
            case STATUS_CHARGE:
                effUpdownEntry(0, strength, 0x3c, x, y, 15.0f + z);
                break;
        }
    } else {
        BtlUnit_GetPos(unit, &x, &y, &z);
        y += BtlUnit_GetHeight(unit) / 2;
        switch (type) {
            case STATUS_BURN:
                if (unit->iceEff) {
                    effSoftDelete(unit->iceEff);
                }
                break;
            case STATUS_FREEZE:
                eff = effVaporN64Entry(0, 0x3c, x, y, z, 1.0f);
                ((EffVaporN64Work*)eff->userdata)->r1 = 0xff;
                ((EffVaporN64Work*)eff->userdata)->g1 = 0xff;
                ((EffVaporN64Work*)eff->userdata)->b1 = 0xff;
                ((EffVaporN64Work*)eff->userdata)->r2 = 0xff;
                ((EffVaporN64Work*)eff->userdata)->g2 = 0xff;
                ((EffVaporN64Work*)eff->userdata)->b2 = 0xff;
                break;
        }
    }

    announce = 0;
    if (ok) {
        BtlUnit_GetStatus(unit, type, &curTurns, &curStrength);
        switch (type) {
            case STATUS_CHARGE:
                if (curStrength > 0) {
                    if (prevStrength != curStrength) {
                        BattleStatusChangeInfoSetAnnouce(unit, type, curStrength, 1);
                        BattleStatusChangeMsgSetAnnouce(unit, type, 1);
                        announce = 1;
                    } else {
                        BattleStatusChangeInfoSetAnnouce(unit, type, curStrength, 0);
                        BattleStatusChangeMsgSetAnnouce(unit, type, 0);
                        announce = 1;
                    }
                }
                break;
            case STATUS_POISON:
                if (curStrength > 0) {
                    BattleStatusChangeInfoSetAnnouce(unit, type, curStrength, 1);
                    BattleStatusChangeMsgSetAnnouce(unit, type, 1);
                    announce = 1;
                }
                break;
            case STATUS_ALLERGIC:
            case STATUS_SLEEP:
            case STATUS_STOP:
            case STATUS_DIZZY:
            case STATUS_CONFUSE:
            case STATUS_ELECTRIC:
            case STATUS_DODGY:
            case STATUS_BURN:
            case STATUS_FREEZE:
            case STATUS_HUGE:
            case STATUS_TINY:
            case STATUS_ATTACK_UP:
            case STATUS_ATTACK_DOWN:
            case STATUS_DEFENSE_UP:
            case STATUS_DEFENSE_DOWN:
            case STATUS_INVISIBLE:
            case STATUS_FAST:
            case STATUS_SLOW:
            case STATUS_PAYBACK:
            case STATUS_HOLD_FAST:
            case STATUS_HP_REGEN:
            case STATUS_FP_REGEN:
                if (curTurns > 0) {
                    BattleStatusChangeInfoSetAnnouce(unit, type, curTurns, 1);
                    BattleStatusChangeMsgSetAnnouce(unit, type, 1);
                    announce = 1;
                }
                break;
        }
    }
    BtlUnit_GetHitPos(unit, part, &hitPos.x, &hitPos.y, &hitPos.z);
    if (announce) {
        switch (type) {
            case STATUS_SLEEP:
                psndSFXOn_3D("SFX_CONDITION_SLEEP1", &hitPos);
                break;
            case STATUS_STOP:
                psndSFXOn_3D("SFX_CONDITION_STOP1", &hitPos);
                break;
            case STATUS_DIZZY:
                psndSFXOn_3D("SFX_CONDITION_GURUGURU1", &hitPos);
                break;
            case STATUS_CONFUSE:
                psndSFXOn_3D("SFX_CONDITION_CONFUSE1", &hitPos);
                break;
            case STATUS_POISON:
                psndSFXOn_3D("SFX_CONDITION_POISON1", &hitPos);
                break;
            case STATUS_DODGY:
                psndSFXOn_3D("SFX_CONDITION_YOKEYOKE1", &hitPos);
                break;
            case STATUS_ELECTRIC:
                psndSFXOn_3D("SFX_CONDITION_BIRIBIRI1", &hitPos);
                break;
            case STATUS_HUGE:
                psndSFXOn_3D("SFX_CONDITION_DEKADEKA1", &hitPos);
                break;
            case STATUS_TINY:
                psndSFXOn_3D("SFX_CONDITION_MINIMINI1", &hitPos);
                break;
            case STATUS_DEFENSE_UP:
                psndSFXOn_3D("SFX_CONDITION_KATIKATI1", &hitPos);
                break;
            case STATUS_DEFENSE_DOWN:
                psndSFXOn_3D("SFX_BTL_FUNYA1", &hitPos);
                break;
            case STATUS_SLOW:
                psndSFXOn_3D("SFX_CONDITION_SLOW1", &hitPos);
                break;
            case STATUS_BURN:
                psndSFXOn_3D("SFX_CONDITION_FIRE1", &hitPos);
                break;
            case STATUS_FREEZE:
                psndSFXOn_3D("SFX_CONDITION_ICE1", &hitPos);
                break;
            case STATUS_FAST:
                psndSFXOn_3D("SFX_CONDITION_FAST1", &hitPos);
                break;
            case STATUS_HP_REGEN:
                psndSFXOn_3D("SFX_CONDITION_REST_HP_SLOW1", &hitPos);
                break;
            case STATUS_FP_REGEN:
                psndSFXOn_3D("SFX_CONDITION_REST_FP_SLOW1", &hitPos);
                break;
            case STATUS_ALLERGIC:
                psndSFXOn_3D("SFX_CONDITION_ALLERGY1", &hitPos);
                break;
            case STATUS_ATTACK_UP:
                psndSFXOn_3D("SFX_CONDITION_TUYOTUYO1", &hitPos);
                break;
            case STATUS_ATTACK_DOWN:
                psndSFXOn_3D("SFX_CONDITION_YOWAYOWA1", &hitPos);
                break;
            case STATUS_CHARGE:
                psndSFXOn_3D("SFX_CONDITION_CHARGE1", &hitPos);
                break;
            case STATUS_INVISIBLE:
                psndSFXOn_3D("SFX_CONDITION_TOUMEI1", &hitPos);
                break;
            case STATUS_PAYBACK:
                psndSFXOn_3D("SFX_CONDITION_COUNTER1", &hitPos);
                break;
            case STATUS_HOLD_FAST:
                psndSFXOn_3D("SFX_BTL_THUNDERS_COUNTER1", &hitPos);
                break;
        }
    }
    return 1;
}

s32 BattleSetStatusDamageFromWeapon(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags) {
    s32 rate;
    u32 i;
    s32 levelDiff;
    u32 result = 0;
    s8 time;
    s8 strength;

    if (!(flags & 0x20000000)) {
        for (i = 0; i < 28; i++) {
            _getSickStatusParam(attacker, weapon, i, &time, &strength);
            rate = (s8)_getSickStatusRate(weapon, i);
            levelDiff = 0;
            switch (i) {
                case 10:
                    if (!time && !strength) {
                        continue;
                    }
                    break;
                case 14:
                    if (!time && !strength) {
                        continue;
                    }
                    break;
                case 12:
                    if (!time && !strength) {
                        continue;
                    }
                    break;
                case 26:
                    if (rate > 0) {
                        levelDiff = attacker->mLevel - target->mLevel;
                    }
                    break;
            }
            if (BattleSetStatusDamage(&result, target, part, weapon->special, i, (s8)rate, levelDiff, time, strength) && i == 18) {
                btlDispPoseAnime(part);
            }
        }
    }
    return result;
}

s32 BattlePreCheckDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags) {
    BattleWork* wp = _battleWorkPointer;
    s32 n;

    if (!target || !part) {
        return 1;
    }
    if (weapon->special & SPECIAL_CHARGEABLE) {
        attacker->mTokenFlags |= kChargeExpended;
    }
    if ((part->attributes & kClone) && !(weapon->special & SPECIAL_CONTACTS_CLONES)) {
        return 2;
    }
    if (weapon->special & SPECIAL_CANNOT_MISS) {
        return 1;
    }
    if (BtlUnit_CheckStatus(target, STATUS_INVISIBLE) || (target->mAttributeFlags & kPermanentlyInvisible)) {
        return 4;
    }
    if (target->mAttributeFlags & kVeiled) {
        return 4;
    }
    if ((part->counterAttributes & kCounterPreemptiveFrontSpiky) && !(weapon->counterResistFlags & COUNTER_RESIST_PREEMPTIVE_SPIKY) && !attacker->badgesEquipped.SpikeShield) {
        return 5;
    }
    if (part->attributes & VERSION_PICK(0x10, 0x40)) {
        return 4;
    }
    if (flags & 0x100000) {
        return 1;
    }
    if (target->badgesEquipped.PrettyLucky) {
        for (n = target->badgesEquipped.PrettyLucky; n > 0; n--) {
            if (irand(100) >= 90) {
                return 3;
            }
        }
    }
    if (target->badgesEquipped.LuckyDay) {
        for (n = target->badgesEquipped.LuckyDay; n > 0; n--) {
            if (irand(100) >= 75) {
                return 3;
            }
        }
    }
    if (target->currentHp <= target->mKindParams->mDangerHP && target->badgesEquipped.CloseCall) {
        for (n = target->badgesEquipped.CloseCall; n > 0; n--) {
            if (irand(100) >= 67) {
                return 3;
            }
        }
    }
    if (BtlUnit_CheckStatus(target, STATUS_DODGY) && !BtlUnit_CheckStatus(target, STATUS_SLEEP) && !BtlUnit_CheckStatus(target, STATUS_STOP) && irand(100) >= 50) {
        return 6;
    }
    n = (u8)weapon->baseAccuracy;
    if (BtlUnit_CheckStatus(attacker, STATUS_DIZZY)) {
        n = n * 50 / 100;
    }
    if (wp->stageObjectWork.fogEffect) {
        n *= 0.5f;
    }
    if (irand(100) >= n) {
        return 2;
    }
    return 1;
}

s32 _checkDamageCode_EmergencyRevival(BattleUnit* unit, u32* flags) {
    s32 kind = unit->currentKind;
    if (kind == UNIT_MARIO) {
        if ((s32)pouchCheckItem(ITEM_LIFE_SHROOM) > 0) {
            *flags |= 0x800;
            return 1;
        }
        return 0;
    }
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        if ((s32)pouchCheckItem(ITEM_LIFE_SHROOM) > 0) {
            *flags |= 0x800;
            return 1;
        }
        return 0;
    }
    if (unit->heldItem == ITEM_LIFE_SHROOM) {
        *flags |= 0x800;
        return 1;
    }
    return 0;
}

s32 BattleCheckDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, s32 flags) {
    BattleWork* wp = _battleWorkPointer;
    u32 pattern;
    s32 ret = -1;
    u32 status = 0;
    s32 damage;
    s32 fpDamage;
    EffectEntry* eff;
    CounterWork cw;
    Vec hit;
    Vec hit2;
    CounterWork* cwp;
    Vec pos;
    u32 result;
    s32 code;
    s32 code2;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    s32 pierce;

    pierce = 0;
    cwp = &cw;
    BattleInitCounterPreCheckWork(cwp);
    if (!target) {
        return 18;
    }
    if (part) {
        target->currentTarget = part;
    }
    damage = 0;
    if (!(weapon->special & SPECIAL_UNGUARDABLE) && BtlUnit_GetACPossibility(target) && BattleActionCommandGetDefenceResult() == 5) {
        BtlUnit_GetHitPos(target, part, &hit.x, &hit.y, &hit.z);
        eff = effNiceEntry(2, hit.x - 60.0f, 45.0f + hit.y, hit.z - 10.0f);
        ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
        flags |= DAMAGE_RESULT_SUPERGUARD;
        psndSFXOn("SFX_SYSTEM_GREAT1");
        BattleAudience_Case_GuardGood();
        attacker->superguardedCount++;
        target->superguardCountTaken = attacker->superguardedCount;
        switch (weapon->superguardsAllowed) {
            case 2:
                result = flags | DAMAGE_CODE_SUPERGUARDED;
                BattleDamageDirect(attacker->unitId, target, part, 0, 0, result, weapon->damagePattern, 0);
                return 21;
            case 3:
                flags |= 0x100;
                result = flags | DAMAGE_CODE_SUPERGUARDED;
                BattleDamageDirect(attacker->unitId, target, part, 0, 0, result, weapon->damagePattern, 0);
                return 21;
            case 4:
                flags |= 0x100;
                result = flags | DAMAGE_CODE_SUPERGUARDED;
                BattleDamageDirect(attacker->unitId, target, part, 0, 0, result, weapon->damagePattern, 0);
                if (attacker->hitEventCode) {
                    BattleRunHitEventDirect(attacker, 0x137, attacker->hitEventCode);
                } else {
                    BattleDamageDirect(-5, attacker, NULL, 1, 0, 0x137, 0, 1);
                }
                return -1;
            case 1:
            default:
                flags |= 0x100;
                result = flags | DAMAGE_CODE_SUPERGUARDED;
                BattleDamageDirect(attacker->unitId, target, part, 0, 0, result, weapon->damagePattern, 0);
                BattleDamageDirect(-5, attacker, NULL, 1, 0, 0x137, 0, 1);
                return -1;
        }
    }
    BattleCheckCounter(cwp, attacker, target, part, weapon, (u32*)&flags);
    while (cw.attackHits) {
        if (flags & 0x100) {
            if (part->counterAttributes & kCounterVolatileExplosive) {
                BattleRunHitEvent(target, 0x719);
                ret = 18;
                break;
            }
            if ((part->counterAttributes & kCounterExplosive) &&
                (weapon->element == ELEMENT_EXPLOSION || weapon->element == ELEMENT_ELECTRIC || weapon->element == ELEMENT_FIRE ||
                 ((weapon->special & SPECIAL_IGNITABLE) && BtlUnit_CheckStatus(attacker, STATUS_BURN)))) {
                BattleRunHitEvent(target, 0x719);
                ret = 18;
                break;
            }
        }
        if (!(weapon->special & SPECIAL_UNGUARDABLE) && BtlUnit_GetACPossibility(target)) {
            if (BattleActionCommandGetDefenceResult() > 3) {
                BtlUnit_GetHitPos(target, part, &hit2.x, &hit2.y, &hit2.z);
                eff = effNiceEntry(0, hit2.x - 60.0f, 45.0f + hit2.y, hit2.z - 10.0f);
                ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
                flags |= DAMAGE_RESULT_GUARD;
                psndSFXOn("SFX_SYSTEM_NICE1");
                BattleAudience_Case_GuardGood();
            } else {
                BattleAudience_Case_GuardBad();
            }
        }
        if ((weapon->special & SPECIAL_FLIP_SHELLED) && (part->attributes & kIsShelled)) {
            pierce = 1;
            flags |= 0x200000;
        }
        if ((weapon->special & SPECIAL_FLIP_BOMB_FLIPPABLE) && (part->attributes & kIsBombFlippable)) {
            pierce = 1;
            flags |= 0x200000;
        }
        if ((weapon->special & SPECIAL_GROUND_WINGED) && (part->attributes & kIsWinged)) {
            pierce = 1;
            if (flags & 0x100) {
                flags |= 0x400000;
            }
        }
        BattleCheckPikkyoro(weapon, (u32*)&flags);
        damage = BattleCalculateDamage(attacker, target, part, weapon, &result, flags);
        fpDamage = BattleCalculateFpDamage(attacker, target, part, weapon, &result, flags);
        target->hpDamageCount++;
        if (!(flags & 0x40000)) {
            if (flags & 0x40000000) {
                status = BattleSetStatusDamageFromWeapon(attacker, target, part, weapon, flags);
            } else if (flags & 0x100) {
                status = BattleSetStatusDamageFromWeapon(attacker, target, part, weapon, flags);
            }
        }
        if ((result & 0xFF) == DAMAGE_CODE_NO_DAMAGE && pierce) {
            result = flags | DAMAGE_CODE_NORMAL;
        }
        if ((result & 0xFF) == DAMAGE_CODE_NO_DAMAGE && (weapon->damagePattern == ATTACKPATTERN_ULTRA_HAMMER_FINISH || weapon->damagePattern == ATTACKPATTERN_NO_DAMAGE_FINISH)) {
            result = flags | DAMAGE_CODE_NORMAL;
        }
        if ((result & 0xFF) == DAMAGE_CODE_NO_DAMAGE && status) {
            if (status & 1) {
                result = flags | DAMAGE_CODE_NORMAL;
            } else {
                result = flags | 0x16;
            }
        }
        if (status & 4) {
            result = flags | 0x1C;
        }
        if (status & 8) {
            result = flags | 0x1D;
        }
        if (((u32)flags & 0x100) && (status & 0x10)) {
            code = 0x717;
            target->damagePattern = 0;
            _checkDamageCode_EmergencyRevival(target, (u32*)&code);
            BattleRunHitEvent(target, code);
            ret = 18;
            break;
        }
        if (((u32)flags & 0x100) && (weapon->special & SPECIAL_FREEZE_CRUSH) && BtlUnit_CheckStatus(target, STATUS_FREEZE) && irand(100) < 0) {
            code2 = 0x8717;
            _checkDamageCode_EmergencyRevival(target, (u32*)&code2);
            BattleRunHitEvent(target, code2);
            ret = 18;
            break;
        }
        if (!status && !damage && !fpDamage && !pierce) {
            ret = 19;
            if (target->currentKind >= UNIT_GOOMBA && target->currentKind < UNIT_UNUSED_TREE && weapon->damagePattern != ATTACKPATTERN_NO_DAMAGE_FINISH) {
                BattleAudience_Case_EnemyNoDamage();
            }
        } else {
            ret = 18;
            if (target->currentKind >= UNIT_GOOMBA && target->currentKind < UNIT_UNUSED_TREE && (damage > 0 || status)) {
                BattleAudience_Case_EnemyDamage(damage);
            }
        }
        pattern = weapon->damagePattern;
        if (pattern == 0x14 && cw.msg) {
            pattern = 0;
        }
        if (pattern == 0xE || pattern == 0x10) {
            if (target->mAttributeFlags & 0x4000) {
                pattern = 0;
            }
            if (irand(100) >= target->mKindParams->hammerKnockbackChance) {
                pattern = 0;
            }
            if (!(flags & 0x20000)) {
                pattern = 0;
            }
        }
        if (status & 0x20) {
            BattleDamageDirect(attacker->unitId, target, part, damage, fpDamage, result, pattern, 0);
        } else {
            BattleDamageDirect(attacker->unitId, target, part, damage, fpDamage, result, pattern, 1);
        }
        break;
    }
    attacker->superguardedCount++;
    target->superguardCountTaken = attacker->superguardedCount;
    if (cw.msg) {
        if (cw.payback) {
            cw.count += damage * 50 / 100;
        }
        if (cw.holdFast) {
            cw.count += damage * 50 / 100;
        }
        if (cw.returnPostage) {
            cw.count += damage * 50 / 100;
        }
        if (cw.poisonStatus) {
            r1 = 0;
            BattleSetStatusDamage(&r1, attacker, part, 0, STATUS_POISON, 100, 0, 3, 1);
        }
        if (cw.freezeStatus) {
            r2 = 0;
            BattleSetStatusDamage(&r2, attacker, part, 0, STATUS_FREEZE, 100, 0, 3, 1);
        }
        if (cw.burnStatus) {
            r3 = 0;
            BattleSetStatusDamage(&r3, attacker, part, 0, STATUS_BURN, 100, 0, 3, 1);
        }
        if (cw.electricStatus) {
            r4 = 0;
            BattleSetStatusDamage(&r4, attacker, part, 0, STATUS_ELECTRIC, 100, 0, 3, 1);
        }
        if (!cw.count) {
            cw.count = 1;
        }
        if (!attacker->mAlliance) {
            BattleAudience_Case_Countered();
        }
        if (cw.holdFast) {
            BtlUnit_GetHitPos(target, part, &pos.x, &pos.y, &pos.z);
            effBombEntry(0, pos.x, pos.y, pos.z, 1.0f);
            psndSFXOn_3D("SFX_BTL_THUNDERS_ATTACK4", &pos);
            BattleFogForceStop();
        }
        BattleActionCommandStop(wp);
        BattleAcHelpSetDispType(0);
        BattleDamageDirect(-5, attacker, NULL, cw.count, 0, cw.msg, 0, 1);
        if (cw.hitEvent) {
            target->damagePattern = 0;
            BattleRunHitEvent(target, cw.hitEvent);
        }
        return -1;
    }
    return ret;
}

void BattleAttackDeclare(s32* unitIdx, s32* partIdx) {
    s32 i;
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleWork* wp;
    s32 j;
    s32 n;

    i = 0;
    wp = _battleWorkPointer;
    for (; i < 64; i++) {
        if (i == *unitIdx) {
            continue;
        }
        unit = BattleGetUnitPtr(wp, i);
        if (!unit) {
            continue;
        }
        if (unit->protectId != *unitIdx) {
            continue;
        }
        if (unit->mTokenFlags & 1) {
            continue;
        }
        if (unit->mTokenFlags & 4) {
            continue;
        }
        if (!BtlUnit_CanGuardStatus(unit)) {
            continue;
        }
        n = (u8)unit->mKindParams->numParts;
        part = unit->mParts;
        for (j = 0; j < n; j++) {
            if (part->attributes & kGuardTargetPart) {
                break;
            }
            part = part->mNextPart;
        }
        if (j < n) {
            break;
        }
    }
    if (i < 64) {
        BattleGetUnitPtr(wp, *unitIdx)->damagePattern = 0;
        BattleRunHitEvent(BattleGetUnitPtr(wp, *unitIdx), 0x1029);
        BattleGetUnitPtr(wp, *unitIdx)->mTokenFlags |= 4;
        wp->flags |= BATTLE_FLAG_GUARD_MOVE;
        unit->damagePattern = 0;
        BattleRunHitEvent(unit, 0x1038);
        unit->mTokenFlags |= 2;
        *unitIdx = i;
        *partIdx = part->mKindPartParams->partNum;
    }
}

void __declare(s32 unitIdx, s32 partIdx, s32* outUnit, s32* outPart) {
    s32 i;
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleWork* wp;
    s32 j;
    s32 n;

    i = 0;
    wp = _battleWorkPointer;
    *outUnit = -1;
    *outPart = 0;
    for (; i < 64; i++) {
        if (i == unitIdx) {
            continue;
        }
        unit = BattleGetUnitPtr(wp, i);
        if (!unit) {
            continue;
        }
        if (unit->protectId != unitIdx) {
            continue;
        }
        if (unit->mTokenFlags & 1) {
            continue;
        }
        if (unit->mTokenFlags & 4) {
            continue;
        }
        if (!BtlUnit_CanGuardStatus(unit)) {
            continue;
        }
        n = (u8)unit->mKindParams->numParts;
        part = unit->mParts;
        for (j = 0; j < n; j++) {
            if (part->attributes & kGuardTargetPart) {
                break;
            }
            part = part->mNextPart;
        }
        if (j < n) {
            break;
        }
    }
    if (i < 64) {
        BattleGetUnitPtr(wp, unitIdx)->damagePattern = 0;
        BattleRunHitEvent(BattleGetUnitPtr(wp, unitIdx), 0x1029);
        BattleGetUnitPtr(wp, unitIdx)->mTokenFlags |= 4;
        wp->flags |= BATTLE_FLAG_GUARD_MOVE;
        unit->damagePattern = 0;
        BattleRunHitEvent(unit, 0x1038);
        unit->mTokenFlags |= 2;
        *outUnit = i;
        *outPart = part->mKindPartParams->partNum;
    }
}

void BattleAttackDeclareAll(BattleWork* wp) {
    s32 i;
    BattleWorkWeaponTargets* wt = &wp->weaponTargets;
    BattleWorkTarget* t;
    s32 j;
    s32 k;
    s32 unit;
    s32 part;

    for (i = 0; i < wt->numTargets; i++) {
        t = &wt->targets[wt->targetIndices[i]];
        __declare(t->unitIdx, t->partIdx, &unit, &part);
        if (unit != -1) {
            for (j = 0; j < wt->numTargets; j++) {
                if (wt->targets[wt->targetIndices[j]].unitIdx == unit) {
                    for (k = j; k < wt->numTargets - 1; k++) {
                        wt->targetIndices[k] = wt->targetIndices[k + 1];
                    }
                    if (j <= i) {
                        i--;
                    }
                    wt->numTargets--;
                    break;
                }
            }
            t->unitIdx = unit;
            t->partIdx = part;
        }
    }
    wt->currentTarget = 0;
}

void BattleInitCounterPreCheckWork(CounterWork* work) {
    memset(work, 0, sizeof(CounterWork));
    work->attackHits = 1;
}

s32 BattleCheckCounter(CounterWork* work, BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32* flags) {
    if (!attacker) {
        return 0;
    }
    work->count = 0;
    if ((part->counterAttributes & kCounterTopSpiky) && !attacker->badgesEquipped.SpikeShield && !(weapon->counterResistFlags & COUNTER_RESIST_TOP_SPIKY)) {
        work->type = DAMAGE_CODE_COUNTER_TOP_SPIKY;
        work->msg = 0x12B;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_TOP_SPIKY;
        *flags |= 0x100;
        work->attackHits = 0;
    } else if ((part->counterAttributes & kCounterPreemptiveFrontSpiky) && !attacker->badgesEquipped.SpikeShield && !(weapon->counterResistFlags & COUNTER_RESIST_PREEMPTIVE_SPIKY)) {
        work->type = DAMAGE_CODE_COUNTER_PREEMPTIVE_SPIKY;
        work->msg = 0x12C;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_PREEMPTIVE_SPIKY;
        *flags |= 0x100;
        work->attackHits = 0;
    } else if ((part->counterAttributes & kCounterFrontSpiky) && !attacker->badgesEquipped.SpikeShield && !(weapon->counterResistFlags & COUNTER_RESIST_FRONT_SPIKY)) {
        work->type = DAMAGE_CODE_COUNTER_FRONT_SPIKY;
        work->msg = 0x12D;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_FRONT_SPIKY;
        *flags |= 0x100;
        work->attackHits = 0;
    } else if (((part->counterAttributes & kCounterFiery) || (part->counterAttributes & kCounterFieryStatus)) && !attacker->badgesEquipped.IcePower && !(weapon->counterResistFlags & COUNTER_RESIST_FIERY)) {
        work->type = DAMAGE_CODE_COUNTER_FIERY;
        work->msg = 0x12E;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_FIERY;
        *flags |= 0x100;
        work->attackHits = 0;
        if (part->counterAttributes & kCounterFieryStatus) {
            work->burnStatus = 1;
        }
    } else if (((part->counterAttributes & kCounterIcy) || (part->counterAttributes & kCounterIcyStatus)) && !(weapon->counterResistFlags & COUNTER_RESIST_ICY)) {
        work->type = DAMAGE_CODE_COUNTER_ICY;
        work->msg = 0x12F;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_ICY;
        *flags |= 0x100;
        work->attackHits = 0;
        if (part->counterAttributes & kCounterIcyStatus) {
            work->freezeStatus = 1;
        }
    } else if (((part->counterAttributes & kCounterElectric) || (part->counterAttributes & kCounterElectricStatus)) && !(weapon->counterResistFlags & COUNTER_RESIST_ELECTRIC) && !BtlUnit_CheckStatus(attacker, STATUS_ELECTRIC)) {
        work->type = DAMAGE_CODE_COUNTER_ELECTRIC;
        work->msg = 0x130;
        work->count++;
        work->hitEvent = DAMAGE_CODE_AVOID_ELECTRIC;
        *flags |= 0x100;
        work->attackHits = 0;
        if (part->counterAttributes & kCounterElectricStatus) {
            work->electricStatus = 1;
        }
    } else {
        if ((part->counterAttributes & kCounterExplosive) && !(weapon->counterResistFlags & COUNTER_RESIST_EXPLOSIVE)) {
            if (weapon->element == ELEMENT_EXPLOSION || weapon->element == ELEMENT_ELECTRIC || weapon->element == ELEMENT_FIRE ||
                ((weapon->special & SPECIAL_IGNITABLE) && BtlUnit_CheckStatus(attacker, STATUS_BURN))) {
                work->type = DAMAGE_CODE_COUNTER_EXPLOSIVE;
                work->msg = 0x131;
                work->count++;
                *flags |= 0x100;
            }
        }
        if ((part->counterAttributes & kCounterVolatileExplosive) && !(weapon->counterResistFlags & COUNTER_RESIST_VOLATILE_EXPLOSIVE)) {
            work->type = DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE;
            work->msg = 0x132;
            work->count++;
            *flags |= 0x100;
        }
        if ((BtlUnit_CheckStatus(target, STATUS_ELECTRIC) || (part->counterAttributes & kCounterElectric) || (part->counterAttributes & kCounterElectricStatus)) &&
            !(weapon->counterResistFlags & COUNTER_RESIST_ELECTRIC) && !BtlUnit_CheckStatus(attacker, STATUS_ELECTRIC)) {
            work->type = DAMAGE_CODE_COUNTER_ELECTRIFIED;
            work->msg = 0x134;
            work->count++;
            *flags |= 0x100;
            if (part->counterAttributes & kCounterElectricStatus) {
                work->electricStatus = 1;
            }
        }
        if (BtlUnit_CheckStatus(target, STATUS_PAYBACK) && !(weapon->counterResistFlags & COUNTER_RESIST_PAYBACK)) {
            work->type = DAMAGE_CODE_COUNTER_PAYBACK;
            work->msg = 0x135;
            *flags |= 0x100;
            work->payback = 1;
        }
        if (((part->counterAttributes & kCounterPoison) || (part->counterAttributes & kCounterPoisonStatus)) && !(weapon->counterResistFlags & COUNTER_RESIST_POISON)) {
            work->type = DAMAGE_CODE_COUNTER_POISON;
            work->msg = 0x133;
            *flags |= 0x100;
            work->payback = 1;
            if (part->counterAttributes & kCounterPoisonStatus) {
                work->poisonStatus = 1;
            }
        }
        if (BtlUnit_CheckStatus(target, STATUS_HOLD_FAST) && !(weapon->counterResistFlags & COUNTER_RESIST_HOLD_FAST)) {
            work->type = DAMAGE_CODE_COUNTER_HOLD_FAST;
            work->msg = 0x136;
            *flags |= 0x100;
            work->holdFast = 1;
        }
        if (target->badgesEquipped.ReturnPostage && !(weapon->counterResistFlags & COUNTER_RESIST_PAYBACK)) {
            work->type = DAMAGE_CODE_COUNTER_PAYBACK;
            work->msg = 0x135;
            *flags |= 0x100;
            work->returnPostage = 1;
        }
    }
    switch (work->type) {
        case DAMAGE_CODE_COUNTER_TOP_SPIKY: return 8;
        case DAMAGE_CODE_COUNTER_PREEMPTIVE_SPIKY: return 7;
        case DAMAGE_CODE_COUNTER_FRONT_SPIKY: return 9;
        case DAMAGE_CODE_COUNTER_FIERY: return 10;
        case DAMAGE_CODE_COUNTER_ICY: return 11;
        case DAMAGE_CODE_COUNTER_EXPLOSIVE: return 12;
        case DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE: return 13;
        case DAMAGE_CODE_COUNTER_ELECTRIFIED: return 14;
        case DAMAGE_CODE_COUNTER_POISON: return 15;
        case DAMAGE_CODE_COUNTER_PAYBACK: return 16;
        case DAMAGE_CODE_COUNTER_HOLD_FAST: return 17;
    }
    return 0;
}
