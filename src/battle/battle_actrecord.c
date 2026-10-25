#include "battle/battle_actrecord.h"
#include "battle/battle.h"

s32 _check_turn_count_0_end(s32 n);
s32 _check_turn_count_0_turn(s32 n);
s32 _check_use(s32 n);
s32 _check_no_use(s32 n);

void BtlActRec_AddCount(u8* count) {
    if (*count < 100) {
        *count = *count + 1;
    }
}

void BtlActRec_AddPoint(u8* point, u8 amount) {
    *point += amount;
    if (*point > 100) {
        *point = 100;
    }
}

void BtlActRec_JudgeTurnRuleKeep(void) {
    s32 v;
    BattleWork* wp = _battleWorkPointer;
    FieldBattleInfo* rec = wp->fbatInfo;
    BattleActRecord* ar = &wp->actRecord;
    switch (rec->ruleType) {
    case 60:
    case 61:
    case 62:
        if (rec->ruleType == 62) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes + ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        } else if (rec->ruleType == 60) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes;
        } else {
            v = ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        }
        _check_turn_count_0_turn(v);
        break;
    case 63:
    case 64:
    case 65:
        if (rec->ruleType == 65) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems + ar->partnerAttackMoves + ar->partnerAttackItems;
        } else if (rec->ruleType == 63) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems;
        } else {
            v = ar->partnerAttackMoves + ar->partnerAttackItems;
        }
        _check_turn_count_0_turn(v);
        break;
    }
}

void BtlActRec_JudgeRuleKeep(void) {
    s32 v;
    BattleWork* wp = _battleWorkPointer;
    BattleActRecord* ar;
    FieldBattleInfo* rec;
    BattleUnit* mario;
    rec = wp->fbatInfo;
    ar = &wp->actRecord;
    mario = BattleGetMarioPtr(wp);
    BattleGetPartyPtr(wp);
    switch (rec->ruleType) {
    case 0:
        rec->ruleKeepResult = 6;
        break;
    case 1:
        rec->ruleKeepResult = _check_no_use(ar->marioJumpMoves);
        break;
    case 2:
        rec->ruleKeepResult = _check_use(ar->marioJumpMoves);
        break;
    case 3:
        rec->ruleKeepResult = _check_no_use(ar->marioHammerMoves);
        break;
    case 4:
        rec->ruleKeepResult = _check_use(ar->marioHammerMoves);
        break;
    case 5:
        rec->ruleKeepResult = _check_no_use(ar->marioSpecialAttacks + ar->marioSpecialSupports);
        break;
    case 6:
        rec->ruleKeepResult = _check_use(ar->marioSpecialAttacks + ar->marioSpecialSupports);
        break;
    case 7:
        rec->ruleKeepResult = _check_no_use(ar->marioDamageTaken);
        break;
    case 8:
        rec->ruleKeepResult = _check_use(ar->marioDamageTaken);
        break;
    case 9:
        rec->ruleKeepResult = _check_no_use(ar->partnerDamageTaken);
        break;
    case 10:
        rec->ruleKeepResult = _check_use(ar->partnerDamageTaken);
        break;
    case 11:
        rec->ruleKeepResult = _check_no_use(ar->marioDamageTaken + ar->partnerDamageTaken);
        break;
    case 12:
        rec->ruleKeepResult = _check_use(ar->marioDamageTaken + ar->partnerDamageTaken);
        break;
    case 13:
        rec->ruleKeepResult = _check_no_use(ar->marioHitsTaken);
        break;
    case 14:
        rec->ruleKeepResult = _check_use(ar->marioHitsTaken);
        break;
    case 15:
        rec->ruleKeepResult = _check_no_use(ar->partnerHitsTaken);
        break;
    case 16:
        rec->ruleKeepResult = _check_use(ar->partnerHitsTaken);
        break;
    case 17:
        rec->ruleKeepResult = _check_no_use(ar->marioHitsTaken + ar->partnerHitsTaken);
        break;
    case 18:
        rec->ruleKeepResult = _check_use(ar->marioHitsTaken + ar->partnerHitsTaken);
        break;
    case 19:
        if (mario != NULL) {
            rec->ruleKeepResult = _check_use(mario->currentHp);
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case 20:
        if (mario != NULL) {
            rec->ruleKeepResult = _check_no_use(mario->currentHp);
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case 21:
        rec->ruleKeepResult = _check_use(ar->powerBounceCombo);
        break;
    case 22:
        rec->ruleKeepResult = _check_no_use(ar->marioAttackItems + ar->marioSupportItems);
        break;
    case 23:
        rec->ruleKeepResult = _check_use(ar->marioAttackItems + ar->marioSupportItems);
        break;
    case 24:
        rec->ruleKeepResult = _check_no_use(ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case 25:
        rec->ruleKeepResult = _check_use(ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case 26:
        rec->ruleKeepResult = _check_no_use(ar->marioAttackItems + ar->marioSupportItems + ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case 27:
        rec->ruleKeepResult = _check_use(ar->marioAttackItems + ar->marioSupportItems + ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case 28:
        rec->ruleKeepResult = _check_no_use(ar->marioPartyChanges);
        break;
    case 29:
        rec->ruleKeepResult = _check_use(ar->marioPartyChanges);
        break;
    case 30:
        rec->ruleKeepResult = _check_no_use(ar->partnerPartyChanges);
        break;
    case 31:
        rec->ruleKeepResult = _check_use(ar->partnerPartyChanges);
        break;
    case 32:
        rec->ruleKeepResult = _check_no_use(ar->marioPartyChanges + ar->partnerPartyChanges);
        break;
    case 33:
        rec->ruleKeepResult = _check_use(ar->marioPartyChanges + ar->partnerPartyChanges);
        break;
    case 34:
        rec->ruleKeepResult = _check_no_use(ar->marioAudienceAttacks);
        break;
    case 35:
        rec->ruleKeepResult = _check_use(ar->marioAudienceAttacks);
        break;
    case 36:
        rec->ruleKeepResult = _check_no_use(ar->partnerAudienceAttacks);
        break;
    case 37:
        rec->ruleKeepResult = _check_use(ar->partnerAudienceAttacks);
        break;
    case 38:
        rec->ruleKeepResult = _check_no_use(ar->marioAppeals);
        break;
    case 39:
        rec->ruleKeepResult = _check_use(ar->marioAppeals);
        break;
    case 40:
        rec->ruleKeepResult = _check_no_use(ar->partnerAppeals);
        break;
    case 41:
        rec->ruleKeepResult = _check_use(ar->partnerAppeals);
        break;
    case 42:
        rec->ruleKeepResult = _check_no_use(ar->marioAppeals + ar->partnerAppeals);
        break;
    case 43:
        rec->ruleKeepResult = _check_use(ar->marioAppeals + ar->partnerAppeals);
        break;
    case 44:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent);
        break;
    case 45:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent);
        break;
    case 46:
        rec->ruleKeepResult = _check_no_use(ar->partnerFpSpent);
        break;
    case 47:
        rec->ruleKeepResult = _check_use(ar->partnerFpSpent);
        break;
    case 48:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case 49:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case 50:
        rec->ruleKeepResult = _check_no_use(ar->marioFpMoves);
        break;
    case 51:
        rec->ruleKeepResult = _check_use(ar->marioFpMoves);
        break;
    case 52:
        rec->ruleKeepResult = _check_no_use(ar->partnerFpMoves);
        break;
    case 53:
        rec->ruleKeepResult = _check_use(ar->partnerFpMoves);
        break;
    case 54:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case 55:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case 56:
        rec->ruleKeepResult = _check_no_use(ar->marioFpMoves + ar->partnerFpMoves);
        break;
    case 57:
        rec->ruleKeepResult = _check_use(ar->marioFpMoves + ar->partnerFpMoves);
        break;
    case 58:
        if (ar->turnCount > rec->ruleParam) {
            rec->ruleKeepResult = 6;
        } else if (ar->turnCount > rec->ruleParam2) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case 59:
        if (ar->turnCount <= rec->ruleParam) {
            rec->ruleKeepResult = 6;
        } else if (ar->turnCount <= rec->ruleParam2) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case 60:
    case 61:
    case 62:
        if (rec->ruleType == 62) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes + ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        } else if (rec->ruleType == 60) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes;
        } else {
            v = ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        }
        _check_turn_count_0_end(v);
        break;
    case 63:
    case 64:
    case 65:
        if (rec->ruleType == 65) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems + ar->partnerAttackMoves + ar->partnerAttackItems;
        } else if (rec->ruleType == 63) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems;
        } else {
            v = ar->partnerAttackMoves + ar->partnerAttackItems;
        }
        _check_turn_count_0_end(v);
        break;
    case 66:
        if (ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioAppeals + ar->marioEscapes == 0) {
            rec->ruleKeepResult = 6;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case 67:
        if (ar->marioJumpMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioAppeals + ar->marioEscapes == 0) {
            rec->ruleKeepResult = 6;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    }
}


s32 _check_use(s32 n) {
    FieldBattleInfo* rec = _battleWorkPointer->fbatInfo;
    if (n >= rec->ruleParam) {
        return 6;
    }
    return (n >= rec->ruleParam2) + 1;
}

s32 _check_no_use(s32 n) {
    FieldBattleInfo* rec = _battleWorkPointer->fbatInfo;
    if (n >= rec->ruleParam2) {
        return 1;
    }
    return n >= rec->ruleParam ? 2 : 6;
}

s32 _check_turn_count_0_turn(s32 n) {
    BattleWork* wp = _battleWorkPointer;
    FieldBattleInfo* rec = wp->fbatInfo;
    BattleActRecord* ar = &wp->actRecord;
    if (ar->turnCount <= rec->ruleParam2 || rec->ruleParam2 == 0xff) {
        if (n == 0) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
    }
    if (ar->turnCount <= rec->ruleParam || rec->ruleParam == 0xff) {
        if (n == 0) {
            rec->ruleKeepResult = 6;
        } else if (!(rec->ruleKeepResult & 2)) {
            rec->ruleKeepResult = 1;
        }
    }
    return rec->ruleKeepResult;
}

s32 _check_turn_count_0_end(s32 n) {
    BattleWork* wp = _battleWorkPointer;
    FieldBattleInfo* rec = wp->fbatInfo;
    BattleActRecord* ar = &wp->actRecord;
    if (ar->turnCount <= rec->ruleParam2 || rec->ruleParam2 == 0xff) {
        if (n == 0) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
    }
    if (ar->turnCount <= rec->ruleParam || rec->ruleParam == 0xff) {
        if (n == 0) {
            rec->ruleKeepResult = 6;
        } else if (!(rec->ruleKeepResult & 2)) {
            rec->ruleKeepResult = 1;
        }
    }
    return rec->ruleKeepResult;
}
