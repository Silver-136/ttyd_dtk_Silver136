#include "version.h"
#include "battle/battle_status_effect.h"
#include "battle/battle.h"
#include "battle/battle_monosiri.h"
#include "battle/battle_unit.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_sub.h"
#include "battle/battle_detect.h"
#include "mario/mario_pouch.h"



void _btlSamplingEnemy(BattleWorkWeaponTargets* d) {
    BattleUnitKindPart* kp;
    BattleWorkTarget* tg4;
    s32 k;
    s32 n = 0;
    BattleWorkTarget* t = d->targets;
    BattleWork* wp = _battleWorkPointer;
    s32 j;
    BattleUnitPart* part;
    s32 i;
    BOOL ok4;
    BattleUnit* u1;
    BattleWorkTarget* o4;
    s32 np2;
    BattleUnit* u2;

    if (d->classFlags & TARGET_CLASS_NONE) {
        d->numTargets = -1;
        return;
    }
    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) && !(unit->mAttributeFlags & kUnitNeverTargetable)) {
            part = unit->mParts;
            np2 = unit->mKindParams->numParts & 0xFF;
            for (j = 0; j < np2; j++) {
                s32 dir;
                f32 x, y, z;
                f32 ox, oy, oz;
                if (part->attributes & kNotTargetable) {
                    part = part->mNextPart;
                    continue;
                }
                kp = part->mKindPartParams;
                dir = part->faceDirection;
                BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
                if (!(unit->mAttributeFlags & kCeiling)) {
                    y += part->mPositionOffset.y * unit->mSizeMultiplier;
                }
                ox = x;
                oy = y;
                oz = 5.0f + z;
                x += dir * (unit->mSizeMultiplier * (part->mHitCursorBaseOffset.x + part->mHitCursorOffset.x));
                y += unit->mSizeMultiplier * (part->mHitCursorBaseOffset.y + part->mHitCursorOffset.y);
                z += unit->mSizeMultiplier * (part->mHitCursorBaseOffset.z + part->mHitCursorOffset.z);
                t->unitIdx = i;
                t->partIdx = kp->partNum;
                t->posX = x;
                t->posY = y;
                t->posZ = z;
                t->score = 0;
                t->reserved_20 = -1;
                if ((unit->mAttributeFlags & kUnitMapObject) && !(part->attributes & kIgnoreMapObjectOffset)) {
                    t->score += d->direction * 300;
                }
                t->score += part->hitOffsetX * dir;
                t->x = (s16)(ox + t->score);
                t->y = (s16)oy;
                t->z = (s16)oz;
                t->forwardLv = BtlCompForwardLv(d->direction, t->x);
                if (t->z < -30) {
                    t->side = 0;
                } else {
                    t->side = 1;
                }
                part = part->mNextPart;
                n++;
                t++;
            }
        }
    }
    d->currentTarget = i = 0;
    d->numTargets = n;
    n = d->numTargets;
    t = d->targets;
    for (; n > i; i++) {
        BattleWorkTarget* tg = &t[i];
        BattleUnit* unit = BattleGetUnitPtr(wp, tg->unitIdx);
        BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, tg->partIdx);
        BOOL ok;

        do {
            u32 flags = d->classFlags;
            if (flags & TARGET_CLASS_NONE) {
                ok = FALSE;
                break;
            }
            if (flags & TARGET_CLASS_ONLY_MARIO) {
                if (unit->currentKind != UNIT_MARIO) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_ONLY_TREE_OR_SWITCH) {
                if (unit->currentKind < UNIT_UNUSED_TREE || unit->currentKind >= UNIT_UNUSED_TESTNPC) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_MARIO_OR_SHELL) {
                if (unit->currentKind == UNIT_MARIO || unit->currentKind == UNIT_SHELL_SHIELD) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_PARTNER) {
                if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_ENEMY) {
                if (unit->currentKind >= UNIT_GOOMBA && unit->currentKind < UNIT_UNUSED_TREE) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_SYSTEM_UNIT) {
                if (unit->currentKind >= UNIT_UNUSED_TESTNPC && unit->currentKind < UNIT_MARIO) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_TREE_OR_SWITCH) {
                if (unit->currentKind >= UNIT_UNUSED_TREE && unit->currentKind < UNIT_UNUSED_TESTNPC) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_SELF) {
                if (d->unitId == unit->unitId) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_OPPOSITE_SIDE) {
                if (d->belong == unit->mAlliance) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_OWN_SIDE) {
                if (d->belong != unit->mAlliance && unit->mAlliance != ALLIANCE_NEUTRAL) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_NO_SAME_KIND) {
                if (d->unitId != unit->unitId && unit->currentKind == BattleGetUnitPtr(wp, d->unitId)->currentKind) {
                    ok = FALSE;
                    break;
                }
            }
            flags = d->classFlags;
            if (flags & TARGET_CLASS_ONLY_SELF) {
                if (d->unitId != unit->unitId) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_ONLY_MAIN_PARTS) {
                u32 a = part->attributes;
                if (!((a & kMainPart) || (a & kImportantPart)) || (a & kNotMainTarget)) {
                    ok = FALSE;
                    break;
                }
            }
            if (flags & TARGET_CLASS_ONLY_MAIN_OR_SEMI_PARTS) {
                u32 a = part->attributes;
#ifdef VERSION_G8ME01
                if (!((a & kMainPart) || (a & kImportantPart) || (a & kSemiImportantPart)) || (a & 0x10) || (a & kNotMainTarget)) {
#else
                if (!((a & kMainPart) || (a & kImportantPart) || (a & kSemiImportantPart)) || (a & kNotMainTarget)) {
#endif
                    ok = FALSE;
                    break;
                }
            }
            ok = TRUE;
        } while (0);
        if (!ok) {
            for (k = i; k < n - 1; k++) {
                d->targets[k] = d->targets[k + 1];
            }
            n--;
            i--;
        }
    }

    for (i = 0; n > i; i++) {
        BattleWorkTarget* tg = &t[i];
        BattleUnit* unit = BattleGetUnitPtr(wp, tg->unitIdx);
        BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, tg->partIdx);
        BOOL ok;

        do {
            if (d->propFlags & TARGET_PROP_TATTLE) {
                if (part->attributes & kUntattleable) {
                    ok = FALSE;
                    break;
                }
                if (!battleGetUnitMonosiriPtr(unit->currentKind)) {
                    ok = FALSE;
                    break;
                }
                if (!battleGetUnitMonosiriPtr(unit->currentKind)->tattleBtlTag) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_NO_FLOATING2) {
                if (unit->mAttributeFlags & kFloating) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_NO_CEILING) {
                if (unit->mAttributeFlags & kCeiling) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_NO_FLOATING) {
                if (unit->mAttributeFlags & kFloating) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_NO_GROUNDED) {
                u32 a = unit->mAttributeFlags;
                if (!(a & kFloating) && !(a & kCeiling)) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_JUMPLIKE) {
                if (part->attributes & kJumplikeCannotTarget) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_HAMMERLIKE) {
                if (part->attributes & kHammerlikeCannotTarget) {
                    if (!(d->propFlags & TARGET_PROP_RECOIL) || d->unitId != unit->unitId) {
                        ok = FALSE;
                        break;
                    }
                }
            }
            if (d->propFlags & TARGET_PROP_SHELL_TOSSLIKE) {
                if (part->attributes & kShellTosslikeCannotTarget) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_ONLY_SHELL_TOSS_IMMUNE) {
                if (!(part->attributes & kShellTosslikeCannotTarget)) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_NO_SHELL_SHIELDED) {
                if (unit->mAttributeFlags & kShellShielded) {
                    ok = FALSE;
                    break;
                }
            }
            if (d->propFlags & TARGET_PROP_SHADOWED_ONLY) {
                if (part->attributes & kNoShadow) {
                    ok = FALSE;
                    break;
                }
            }
            ok = TRUE;
        } while (0);
        if (!ok) {
            for (k = i; k < n - 1; k++) {
                d->targets[k] = d->targets[k + 1];
            }
            n--;
            i--;
        }
    }

    if (d->classFlags & TARGET_CLASS_ONLY_MAIN_PARTS) {
        for (i = 0; n > i; i++) {
            BattleWorkTarget* tg = &t[i];
            BattleUnitPart* part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(wp, tg->unitIdx), tg->partIdx);
            BOOL keep = TRUE;
            if (part->attributes & kImportantPart) {
                for (j = 0; j < n; j++) {
                    BattleWorkTarget* o = &t[j];
                    if (tg != o && tg->unitIdx == o->unitIdx) {
                        if (BtlUnit_GetPartsPtr(BattleGetUnitPtr(wp, o->unitIdx), o->partIdx)->attributes & kMainPart) {
                            keep = FALSE;
                            break;
                        }
                    }
                }
            }
            if (!keep) {
                for (k = i; k < n - 1; k++) {
                    d->targets[k] = d->targets[k + 1];
                }
                n--;
                i--;
            }
        }
    }

    for (i = 0; n > i; i++) {
        BOOL keep;
        BattleUnitPart* part;

        tg4 = &t[i];
        part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(wp, tg4->unitIdx), tg4->partIdx);
        do {
            if ((d->propFlags & TARGET_PROP_ONLY_FRONTMOST) && !(part->attributes & VERSION_PICK(0x10, 0x40))) {
                ok4 = TRUE;
                for (j = 0; j < n; j++) {
                    s32 a;
                    s32 b;
                    o4 = &t[j];
                    if (tg4 == o4 || tg4->side != o4->side) {
                        continue;
                    }
                    a = tg4->x;
                    b = o4->x;
                    if (d->direction < 0) {
                        a *= -1;
                        b *= -1;
                    }
                    if (a > b) {
                        BattleUnitPart* op = BattleGetUnitPartsPtr(o4->unitIdx, o4->partIdx);
                        if (!(op->attributes & VERSION_PICK(0x10, 0x40)) && !(op->attributes & kShellTosslikeCannotTarget)) {
                            u1 = BattleGetUnitPtr(wp, d->unitId);
                            u2 = BattleGetUnitPtr(wp, o4->unitIdx);
                            if (!(u1->mTokenFlags & kConfusedAction) || u1 != u2) {
                                ok4 = FALSE;
                                break;
                            }
                        }
                    }
                }
                if (!ok4) {
                    keep = FALSE;
                    break;
                }
            }
            keep = TRUE;
        } while (0);

        if (!keep) {
            for (k = i; k < n - 1; k++) {
                d->targets[k] = d->targets[k + 1];
            }
            n--;
            i--;
        }
    }

    d->numTargets = n;
    {
        s8* idx = d->targetIndices;

        t = d->targets;
        n = d->numTargets;
        for (i = 0; i < n; i++) {
            idx[i] = i;
        }
        if (d->direction > 0) {
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    BattleWorkTarget* ta = &t[idx[i]];
                    BattleWorkTarget* tb = &t[idx[j]];
                    if (ta->posX + ta->score * 10 > tb->posX + tb->score * 10) {
                        s8 a = idx[i];
                        idx[i] = idx[j];
                        idx[j] = a;
                    }
                }
            }
        } else {
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    BattleWorkTarget* ta = &t[idx[i]];
                    BattleWorkTarget* tb = &t[idx[j]];
                    if (ta->posX + ta->score * 10 <= tb->posX + tb->score * 10) {
                        s8 a = idx[i];
                        idx[i] = idx[j];
                        idx[j] = a;
                    }
                }
            }
        }
    }
}

BOOL BattleSamplingEnemy(BattleWorkWeaponTargets* d, BattleWeapon* weapon, s32 unitId, s8 belong, u32 classFlags, u32 propFlags,
                         s8 direction) {
    BOOL r = FALSE;
    d->weapon = weapon;
    d->unitId = unitId;
    d->belong = belong;
    d->classFlags = classFlags;
    d->propFlags = propFlags;
    d->direction = direction;
    if (BattleGetUnitPtr(_battleWorkPointer, unitId)->mTokenFlags & kConfusedAction) {
        if ((d->classFlags & TARGET_CLASS_NO_OPPOSITE_SIDE) && !(d->classFlags & TARGET_CLASS_NO_OWN_SIDE)) {
            r = TRUE;
            d->classFlags &= ~TARGET_CLASS_NO_OPPOSITE_SIDE;
            d->classFlags |= TARGET_CLASS_NO_OWN_SIDE;
        } else if (!(d->classFlags & TARGET_CLASS_NO_OPPOSITE_SIDE) && (d->classFlags & TARGET_CLASS_NO_OWN_SIDE)) {
            r = TRUE;
            d->classFlags |= TARGET_CLASS_NO_OPPOSITE_SIDE;
            d->classFlags &= ~TARGET_CLASS_NO_OWN_SIDE;
        }
    }
    _btlSamplingEnemy(d);
    return r;
}

void BattleSamplingEnemyUpdate(BattleWorkWeaponTargets* d) {
    _btlSamplingEnemy(d);
}

void BattleChoiceSamplingEnemy(BattleWorkWeaponTargets* d, u32 flags, s32* unitOut, s32* partOut) {
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    s32 w[74];
    s32 i;
    BattleUnit* active;

    *unitOut = -1;
    *partOut = 0;
    if (d->numTargets > 0) {
        active = BattleGetUnitPtr(wp, d->unitId);
        i = d->targetIndices[0];
        *unitOut = d->targets[i].unitIdx;
        *partOut = d->targets[i].partIdx;
        for (i = 0; i < 74; i++) {
            w[i] = 0;
        }
        w[d->targetIndices[0]] += 10;
        for (i = 0; i < d->numTargets; i++) {
            s32 id = d->targetIndices[i];
            unit = BattleGetUnitPtr(wp, d->targets[id].unitIdx);
            w[id] += 100;
            if ((flags & TARGET_WEIGHT_MARIO) && unit->currentKind == UNIT_MARIO) {
                w[id] = 1.5f * w[id];
            }
            if ((flags & TARGET_WEIGHT_PARTNER) && unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
                w[id] = 1.5f * w[id];
            }
            if (flags & TARGET_WEIGHT_FRONT) {
                f32 t = 0.4f * (d->numTargets - i);
                w[id] *= 0.1f + t;
            }
            if (flags & TARGET_WEIGHT_BACK) {
                f32 t = 0.4f * i;
                w[id] = (0.5f + t) * w[id];
            }
            if ((flags & TARGET_WEIGHT_OWN_SIDE) && active->mAlliance == unit->mAlliance) {
                w[id] = 1.5f * w[id];
            }
            if ((flags & TARGET_WEIGHT_OPPOSITE_SIDE) && active->mAlliance != unit->mAlliance && unit->mAlliance != ALLIANCE_NEUTRAL) {
                w[id] = 1.5f * w[id];
            }
            if (flags & TARGET_WEIGHT_LOW_HP_RATIO) {
                w[id] = (2.0f - (f32)unit->currentHp / (f32)unit->maxHp) * w[id];
            }
            if (flags & TARGET_WEIGHT_HIGH_HP_RATIO) {
                w[id] = (1.0f + (f32)unit->currentHp / (f32)unit->maxHp) * w[id];
            }
            if (flags & TARGET_WEIGHT_LOW_HP) {
                w[id] = (1.5f - 0.05f * unit->currentHp) * w[id];
            }
            if (flags & TARGET_WEIGHT_HIGH_HP) {
                w[id] = (1.0f + 0.05f * unit->currentHp) * w[id];
            }
            if ((flags & TARGET_WEIGHT_IN_PERIL) && unit->currentHp <= unit->mKindParams->mPerilHP) {
                w[id] = 1.5f * w[id];
            }
            if (d->weapon) {
                if (unit->mAttributeFlags & kInactive) {
                    w[id] = 1;
                }
                switch (d->weapon->itemId) {
                case ITEM_NULL:
                    break;
                case ITEM_POWER_PUNCH:
                    if (!BtlUnit_CanActStatus(unit) || BtlUnit_CheckStatus(unit, STATUS_HUGE)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_MINI_MR_MINI:
                    if (BtlUnit_CheckStatus(unit, STATUS_TINY)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_MR_SOFTENER:
                    if (BtlUnit_CheckStatus(unit, STATUS_DEFENSE_DOWN)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_COURAGE_SHELL:
                    if (BtlUnit_CheckStatus(unit, STATUS_DEFENSE_UP)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_BOOS_SHEET:
                    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_VOLT_SHROOM:
                    if (BtlUnit_CheckStatus(unit, STATUS_ELECTRIC)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_REPEL_CAPE:
                    if (BtlUnit_CheckStatus(unit, STATUS_DODGY)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_RUIN_POWDER:
                    if (BtlUnit_CheckStatus(unit, STATUS_CONFUSE)) {
                        w[id] = 1;
                    }
                    break;
                case ITEM_TASTY_TONIC:
                    if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) && !BtlUnit_CheckStatus(unit, STATUS_STOP)
                        && !BtlUnit_CheckStatus(unit, STATUS_DIZZY) && !BtlUnit_CheckStatus(unit, STATUS_POISON)
                        && !BtlUnit_CheckStatus(unit, STATUS_CONFUSE) && !BtlUnit_CheckStatus(unit, STATUS_BURN)
                        && !BtlUnit_CheckStatus(unit, STATUS_FREEZE) && !BtlUnit_CheckStatus(unit, STATUS_TINY)
                        && !BtlUnit_CheckStatus(unit, STATUS_DEFENSE_DOWN) && !BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
                        w[id] = 1;
                    }
                    break;
                default:
                    if (itemDataTable[d->weapon->itemId].fp_restored) {
                        s32 mx = BtlUnit_GetMaxFp(unit);
                        if (BtlUnit_GetFp(unit) < mx) {
                            mx = BtlUnit_GetMaxFp(unit);
                            w[id] = w[id] * (1.0f + 0.1f * (mx - BtlUnit_GetFp(unit)));
                        } else if (!itemDataTable[d->weapon->itemId].hp_restored
                                   && !itemDataTable[d->weapon->itemId].sp_restored) {
                            w[id] = 1;
                        }
                    }
                    break;
                }
            }
        }
        if (!(flags & TARGET_WEIGHT_RANDOM)) {
            s32 best = 0;
            s32 bestVal = 0;
            for (i = 0; i < 74; i++) {
                if (w[i] > bestVal) {
                    best = i;
                    bestVal = w[i];
                }
            }
            *unitOut = d->targets[best].unitIdx;
            *partOut = d->targets[best].partIdx;
        } else {
            s32 sum = 0;
            s32 r;
            s32 pick;
            for (i = 0; i < 74; i++) {
                sum += w[i];
            }
            r = irand(sum);
            pick = 0;
            for (i = 0; i < 74; i++) {
                r -= w[i];
                if (r <= 0) {
                    break;
                }
                pick++;
            }
            *unitOut = d->targets[pick].unitIdx;
            *partOut = d->targets[pick].partIdx;
        }
    }
}

void BattleGetFirstAttackUnit(BattleWork* wp, BattleUnit** attacker, BattleUnit** target) {
    BattleUnit* found;
    BattleUnit* other;
    // FAKE: tmp holds the getter result across the swap; assigning it directly changes the object
    BattleUnit* tmp;
    s32 i;
    for (i = 0; i < 64; i++) {
        found = BattleGetUnitPtr(wp, i);
        if (found && found->mAlliance == ALLIANCE_ENEMY) {
            break;
        }
    }
    switch (wp->fbatInfo->firstAttack) {
    case FIRST_ATTACK_JUMP: case FIRST_ATTACK_SPIN_JUMP: case FIRST_ATTACK_JUMP_UNUSED:
    case FIRST_ATTACK_HAMMER: case FIRST_ATTACK_SPIN_HAMMER: case FIRST_ATTACK_ULTRA_HAMMER:
        tmp = BattleGetMarioPtr(wp);
        other = found;
        found = tmp;
        break;
    case FIRST_ATTACK_SHELL: case FIRST_ATTACK_BOMB:
        tmp = BattleGetPartnerPtr(wp, BattleGetMarioPtr(wp));
        other = found;
        found = tmp;
        break;
    case FIRST_ATTACK_ENEMY:
    default:
        other = BattleGetMarioPtr(wp);
        break;
    }
    *attacker = found;
    *target = other;
}
