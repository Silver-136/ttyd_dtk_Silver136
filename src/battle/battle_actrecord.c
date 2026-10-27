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
    case kConditionWaitTurnsBeforeMarioActions:
    case kConditionWaitTurnsBeforePartnerActions:
    case kConditionWaitTurnsBeforePartyActions:
        if (rec->ruleType == kConditionWaitTurnsBeforePartyActions) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes + ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        } else if (rec->ruleType == kConditionWaitTurnsBeforeMarioActions) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes;
        } else {
            v = ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        }
        _check_turn_count_0_turn(v);
        break;
    case kConditionWaitTurnsBeforeMarioAttacks:
    case kConditionWaitTurnsBeforePartnerAttacks:
    case kConditionWaitTurnsBeforePartyAttacks:
        if (rec->ruleType == kConditionWaitTurnsBeforePartyAttacks) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems + ar->partnerAttackMoves + ar->partnerAttackItems;
        } else if (rec->ruleType == kConditionWaitTurnsBeforeMarioAttacks) {
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
    case kNoFailureCondition:
        rec->ruleKeepResult = 6;
        break;
    case kConditionJumpLess:
        rec->ruleKeepResult = _check_no_use(ar->marioJumpMoves);
        break;
    case kConditionJumpMore:
        rec->ruleKeepResult = _check_use(ar->marioJumpMoves);
        break;
    case kConditionHammerLess:
        rec->ruleKeepResult = _check_no_use(ar->marioHammerMoves);
        break;
    case kConditionHammerMore:
        rec->ruleKeepResult = _check_use(ar->marioHammerMoves);
        break;
    case kConditionUseSpecialMovesLess:
        rec->ruleKeepResult = _check_no_use(ar->marioSpecialAttacks + ar->marioSpecialSupports);
        break;
    case kConditionUseSpecialMovesMore:
        rec->ruleKeepResult = _check_use(ar->marioSpecialAttacks + ar->marioSpecialSupports);
        break;
    case kConditionMarioTakeLessDamage:
        rec->ruleKeepResult = _check_no_use(ar->marioDamageTaken);
        break;
    case kConditionMarioTakeMoreDamage:
        rec->ruleKeepResult = _check_use(ar->marioDamageTaken);
        break;
    case kConditionPartnersTakeLessDamage:
        rec->ruleKeepResult = _check_no_use(ar->partnerDamageTaken);
        break;
    case kConditionPartnersTakeMoreDamage:
        rec->ruleKeepResult = _check_use(ar->partnerDamageTaken);
        break;
    case kConditionPartyTakeLessDamage:
        rec->ruleKeepResult = _check_no_use(ar->marioDamageTaken + ar->partnerDamageTaken);
        break;
    case kConditionPartyTakeMoreDamage:
        rec->ruleKeepResult = _check_use(ar->marioDamageTaken + ar->partnerDamageTaken);
        break;
    case kConditionMarioTakeFewerHits:
        rec->ruleKeepResult = _check_no_use(ar->marioHitsTaken);
        break;
    case kConditionMarioTakeMoreHits:
        rec->ruleKeepResult = _check_use(ar->marioHitsTaken);
        break;
    case kConditionPartnersTakeFewerHits:
        rec->ruleKeepResult = _check_no_use(ar->partnerHitsTaken);
        break;
    case kConditionPartnersTakeMoreHits:
        rec->ruleKeepResult = _check_use(ar->partnerHitsTaken);
        break;
    case kConditionPartyTakeFewerHits:
        rec->ruleKeepResult = _check_no_use(ar->marioHitsTaken + ar->partnerHitsTaken);
        break;
    case kConditionPartyTakeMoreHits:
        rec->ruleKeepResult = _check_use(ar->marioHitsTaken + ar->partnerHitsTaken);
        break;
    case kConditionMarioWinMoreHp:
        if (mario != NULL) {
            rec->ruleKeepResult = _check_use(mario->currentHp);
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case kConditionMarioWinLessHp:
        if (mario != NULL) {
            rec->ruleKeepResult = _check_no_use(mario->currentHp);
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case kConditionPowerBounceCombo:
        rec->ruleKeepResult = _check_use(ar->powerBounceCombo);
        break;
    case kConditionMarioUseFewerItems:
        rec->ruleKeepResult = _check_no_use(ar->marioAttackItems + ar->marioSupportItems);
        break;
    case kConditionMarioUseMoreItems:
        rec->ruleKeepResult = _check_use(ar->marioAttackItems + ar->marioSupportItems);
        break;
    case kConditionPartnersUseFewerItems:
        rec->ruleKeepResult = _check_no_use(ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case kConditionPartnersUseMoreItems:
        rec->ruleKeepResult = _check_use(ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case kConditionPartyUseFewerItems:
        rec->ruleKeepResult = _check_no_use(ar->marioAttackItems + ar->marioSupportItems + ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case kConditionPartyUseMoreItems:
        rec->ruleKeepResult = _check_use(ar->marioAttackItems + ar->marioSupportItems + ar->partnerAttackItems + ar->partnerSupportItems);
        break;
    case kConditionMarioSwapPartnersLess:
        rec->ruleKeepResult = _check_no_use(ar->marioPartyChanges);
        break;
    case kConditionMarioSwapPartnersMore:
        rec->ruleKeepResult = _check_use(ar->marioPartyChanges);
        break;
    case kConditionPartnersSwapPartnersLess:
        rec->ruleKeepResult = _check_no_use(ar->partnerPartyChanges);
        break;
    case kConditionPartnersSwapPartnersMore:
        rec->ruleKeepResult = _check_use(ar->partnerPartyChanges);
        break;
    case kConditionPartySwapPartnersLess:
        rec->ruleKeepResult = _check_no_use(ar->marioPartyChanges + ar->partnerPartyChanges);
        break;
    case kConditionPartySwapPartnersMore:
        rec->ruleKeepResult = _check_use(ar->marioPartyChanges + ar->partnerPartyChanges);
        break;
    case kConditionMarioAttackAudienceLess:
        rec->ruleKeepResult = _check_no_use(ar->marioAudienceAttacks);
        break;
    case kConditionMarioAttackAudienceMore:
        rec->ruleKeepResult = _check_use(ar->marioAudienceAttacks);
        break;
    case kConditionPartnersAttackAudienceLess:
        rec->ruleKeepResult = _check_no_use(ar->partnerAudienceAttacks);
        break;
    case kConditionPartnersAttackAudienceMore:
        rec->ruleKeepResult = _check_use(ar->partnerAudienceAttacks);
        break;
    case kConditionMarioAppealLess:
        rec->ruleKeepResult = _check_no_use(ar->marioAppeals);
        break;
    case kConditionMarioAppealMore:
        rec->ruleKeepResult = _check_use(ar->marioAppeals);
        break;
    case kConditionPartnersAppealLess:
        rec->ruleKeepResult = _check_no_use(ar->partnerAppeals);
        break;
    case kConditionPartnersAppealMore:
        rec->ruleKeepResult = _check_use(ar->partnerAppeals);
        break;
    case kConditionPartyAppealLess:
        rec->ruleKeepResult = _check_no_use(ar->marioAppeals + ar->partnerAppeals);
        break;
    case kConditionPartyAppealMore:
        rec->ruleKeepResult = _check_use(ar->marioAppeals + ar->partnerAppeals);
        break;
    case kConditionMarioSpendLessFp:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent);
        break;
    case kConditionMarioSpendMoreFp:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent);
        break;
    case kConditionPartnersSpendLessFp:
        rec->ruleKeepResult = _check_no_use(ar->partnerFpSpent);
        break;
    case kConditionPartnersSpendMoreFp:
        rec->ruleKeepResult = _check_use(ar->partnerFpSpent);
        break;
    case kConditionPartySpendLessFp_0x30:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case kConditionPartySpendMoreFp_0x31:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case kConditionMarioUseMovesLess:
        rec->ruleKeepResult = _check_no_use(ar->marioFpMoves);
        break;
    case kConditionMarioUseMovesMore:
        rec->ruleKeepResult = _check_use(ar->marioFpMoves);
        break;
    case kConditionPartnerUseMovesLess:
        rec->ruleKeepResult = _check_no_use(ar->partnerFpMoves);
        break;
    case kConditionPartnerUseMovesMore:
        rec->ruleKeepResult = _check_use(ar->partnerFpMoves);
        break;
    case kConditionPartySpendLessFp_0x36:
        rec->ruleKeepResult = _check_no_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case kConditionPartySpendMoreFp_0x37:
        rec->ruleKeepResult = _check_use(ar->marioFpSpent + ar->partnerFpSpent);
        break;
    case kConditionPartyUseMovesLess:
        rec->ruleKeepResult = _check_no_use(ar->marioFpMoves + ar->partnerFpMoves);
        break;
    case kConditionPartyUseMovesMore:
        rec->ruleKeepResult = _check_use(ar->marioFpMoves + ar->partnerFpMoves);
        break;
    case kConditionWinInMoreTurns:
        if (ar->turnCount > rec->ruleParam) {
            rec->ruleKeepResult = 6;
        } else if (ar->turnCount > rec->ruleParam2) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case kConditionWinInFewerTurns:
        if (ar->turnCount <= rec->ruleParam) {
            rec->ruleKeepResult = 6;
        } else if (ar->turnCount <= rec->ruleParam2) {
            rec->ruleKeepResult = 2;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case kConditionWaitTurnsBeforeMarioActions:
    case kConditionWaitTurnsBeforePartnerActions:
    case kConditionWaitTurnsBeforePartyActions:
        if (rec->ruleType == kConditionWaitTurnsBeforePartyActions) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes + ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        } else if (rec->ruleType == kConditionWaitTurnsBeforeMarioActions) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioEscapes;
        } else {
            v = ar->partnerAttackMoves + ar->partnerSupportMoves + ar->partnerAttackItems + ar->partnerSupportItems + ar->partnerPartyChanges + ar->partnerCharges + ar->partnerSuperCharges + ar->partnerEscapes;
        }
        _check_turn_count_0_end(v);
        break;
    case kConditionWaitTurnsBeforeMarioAttacks:
    case kConditionWaitTurnsBeforePartnerAttacks:
    case kConditionWaitTurnsBeforePartyAttacks:
        if (rec->ruleType == kConditionWaitTurnsBeforePartyAttacks) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems + ar->partnerAttackMoves + ar->partnerAttackItems;
        } else if (rec->ruleType == kConditionWaitTurnsBeforeMarioAttacks) {
            v = ar->marioJumpMoves + ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioAttackItems;
        } else {
            v = ar->partnerAttackMoves + ar->partnerAttackItems;
        }
        _check_turn_count_0_end(v);
        break;
    case kConditionMarioOnlyJumpOrDefend:
        if (ar->marioHammerMoves + ar->marioSpecialAttacks + ar->marioSpecialSupports + ar->marioAttackItems + ar->marioSupportItems + ar->marioPartyChanges + ar->marioCharges + ar->marioSuperCharges + ar->marioAppeals + ar->marioEscapes == 0) {
            rec->ruleKeepResult = 6;
        } else {
            rec->ruleKeepResult = 1;
        }
        break;
    case kConditionMarioOnlyHammerOrDefend:
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
