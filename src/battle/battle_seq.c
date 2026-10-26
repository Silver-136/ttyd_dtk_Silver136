#include "battle/battle_seq.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_unit_event.h"
#include "driver/animdrv.h"
#include "driver/mapdrv.h"
#include "eff/eff_nice.h"
#include "battle/battle_database_common.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "mario/mario_party.h"
#include "party.h"
#include "pmario_sound.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "battle/battle_status_effect.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "driver/camdrv.h"
#include <stdio.h>
#include <string.h>
#include "battle/battle_event_default.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_cmd.h"
#include "unit/party/unit_party_nokotarou.h"
#include "unit/party/unit_party_sanders.h"
#include "manager/evtmgr_cmd.h"
#include "driver/fadedrv.h"
#include "mario/mario.h"
#include "battle/battle_audience.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_actrecord.h"
#include "battle/battle_camera.h"
#include "eff/eff_recovery.h"
#include "eff/n64/eff_freeze_n64.h"
#include "battle/battle_detect.h"


#define STAGE_WORK(wp) (&(wp)->stageObjectWork)

// external prototypes
f32 intpl_sub(s32 type, f32 start, f32 end, s32 cur, s32 max);
void BattleCommandInit(BattleWork* wp);
BOOL BattleCommandInput(BattleWork* wp);
void BattleStatusChangeMsgAdjust(BattleWork* wp);
BOOL BattleStatusChangeMsgMain(BattleWork* wp);
BOOL BattleStatusChangeAnnouceMain(BattleWork* wp);
s32 BattleDamageDirect(s32 unitIdx, BattleUnit* unit, BattleUnitPart* part, s32 damage, s32 fpDamage, u32 stopFlags, u32 damageEffect, u32 pattern);
void BattleSetStatusDamageFromWeapon(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags);
extern s32 marioDefaultWeapon_Jump[];
extern s32 marioWeapon_KururinJump[];
extern s32 marioDefaultWeapon_Hammer[];

//local prototypes
USER_FUNC(_set_haikei_entry_scale);
void _mapobj_data_touch_scale(s32 layer);
void battleMakePhaseEvtTable(BattleWork* wp);
void btlseqPhaseFirstProcess(BattleWork* wp);
void btlseqTurnFirstProcess(BattleWork* wp);
void btlseqAct(BattleWork* wp);
BOOL BattlePhaseEndCheck(void);
void btlseqMove(BattleWork* wp);
void btlseqPhase(BattleWork* wp);
void _rule_disp(CameraId cameraId, void* param);
USER_FUNC(_set_effect_luck);

s32 _set_haikei_entry_scale(EventEntry* event, BOOL isFirstCall);
EVT_BEGIN(nanika_luck_event)
    BEGIN_SIBLING_EVENT_GET_ID(PTR(0xfe363c8f))
        WAIT_FRAMES(60)
    END_SIBLING_EVENT
    CALL(_set_effect_luck)
    CALL(btlevtcmd_WaitEventEnd, PTR(0xfe363c8f))
    RETURN
EVT_END()
EVT_BEGIN(_default_haikei_set_entry_event)
    CALL(_set_haikei_entry_scale)
    RETURN
EVT_END()

BattleWeapon* _GetFirstAttackWeapon(s32 type);
void BattleCheckAllPinchStatus(BattleWork* wp, BOOL noAudience);
void btlseqInit(BattleWork* wp);
void btlseqFirstAct(BattleWork* wp);
void btlseqTurn(BattleWork* wp);
void btlseqEnd(BattleWork* wp);

// static data from an inlined header function (see unit_party_sanders.c)

const BattleWeapon N_weapon_lucky_start = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    3, // electricTime
    100, // dodgyChance
    3, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, // holdFastTime..invisibleTime
    3, // hpRegenTime
    2, // hpRegenStrength
    3, // fpRegenTime
    2, // fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static const GXColor c = {0, 0, 0, 0xFF};
static const GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};

const char* _rule_msg_table_2bu[] = {
    "a", "stg3_tou_cp_001", "a", "stg3_tou_cp_002", "a", "stg3_tou_cp_004", "stg3_tou_cp_008", "a",
    "a", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "stg3_tou_cp_000", "a", "a", "a", "a", "a",
    "a", "a", "stg3_tou_cp_005", "a", "a", "a", "a", "a",
    "stg3_tou_cp_006", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_009", "a", "a", "a", "a",
    "stg3_tou_cp_003", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_007", "a", "a", "a", "a",
    "a", "a", "a", "a",
};

const char* _rule_msg_table_1bu[] = {
    "test", "stg3_tou_cp_011", "a", "stg3_tou_cp_012", "a", "stg3_tou_cp_014", "stg3_tou_cp_019", "a",
    "a", "a", "a", "stg3_tou_cp_020", "a", "a", "a", "a",
    "a", "a", "stg3_tou_cp_010", "a", "stg3_tou_cp_018", "a", "a", "a",
    "a", "a", "stg3_tou_cp_015", "a", "a", "a", "a", "a",
    "stg3_tou_cp_016", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_021", "a", "a", "a", "a",
    "stg3_tou_cp_013", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_017", "a", "a", "a", "stg3_tou_cp_024",
    "stg3_tou_cp_023", "stg3_tou_cp_022", "a", "a",
};

const char* _rule_msg_table_after[] = {
    "a", "stg3_tou_cp_041", "a", "stg3_tou_cp_042", "a", "stg3_tou_cp_044", "stg3_tou_cp_049", "a",
    "a", "a", "a", "stg3_tou_cp_050", "a", "a", "a", "a",
    "a", "a", "stg3_tou_cp_040", "a", "stg3_tou_cp_048", "a", "a", "a",
    "a", "a", "stg3_tou_cp_045", "a", "a", "a", "a", "a",
    "stg3_tou_cp_046", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_051", "a", "a", "a", "a",
    "stg3_tou_cp_043", "a", "a", "a", "a", "a", "a", "a",
    "a", "a", "a", "stg3_tou_cp_047", "a", "a", "a", "stg3_tou_cp_054",
    "stg3_tou_cp_053", "stg3_tou_cp_052", "a", "a",
};

void BattleSequenceManager(void) {
    BattleWork* wp = _battleWorkPointer;
    switch (BattleGetSeq(wp, SEQ_UNKNOWN)) {
        case 0:
            btlseqInit(wp);
            BattleCheckAllPinchStatus(wp, TRUE);
            break;
        case 1:
            BattleCheckAllPinchStatus(wp, FALSE);
            btlseqFirstAct(wp);
            break;
        case 2:
            BattleCheckAllPinchStatus(wp, FALSE);
            btlseqTurn(wp);
            break;
        case 3:
            btlseqEnd(wp);
            break;
        case 4:
            break;
    }
}

void BattleCheckAllPinchStatus(BattleWork* wp, BOOL noAudience) {
    BattleUnit* unit;
    BOOL v6;
    int i;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            BtlUnit_CheckPinchStatus(unit, noAudience);
        }
    }

    v6 = FALSE;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            if ((unit->mStatusFlags & UNIT_STATUS_FLAG_PERIL) &&
                !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) &&
                !BtlUnit_GetBelong(unit)) {
                if (unit->currentKind == UNIT_MARIO ||
                    unit->currentKind >= TYPE_PARTNER_MIN &&
                    unit->currentKind < TYPE_PARTNER_MAX) {
                    if (unit->currentHp > 0) {
                        v6 = TRUE;
                    }
                }
                break;
            }
        }
    }

    if (v6) {
        wp->alertTick--;
        if (wp->alertTick <= 0) {
            wp->alertTick = 60;
            if (wp->commandMenu.flags & 1) {
                psndSFXOn("SFX_BTL_DANGER1");
            }
        }
    } else {
        v6 = FALSE;
        for (i = 0; i < 64; i++) {
            unit = BattleGetUnitPtr(wp, i);
            if (unit) {
                if ((unit->mStatusFlags & UNIT_STATUS_FLAG_DANGER) &&
                    !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) &&
                    !BtlUnit_GetBelong(unit)) {
                    if (unit->currentKind == UNIT_MARIO ||
                        unit->currentKind >= TYPE_PARTNER_MIN &&
                        unit->currentKind < TYPE_PARTNER_MAX) {
                        if (unit->currentHp > 0) {
                            v6 = TRUE;
                        }
                    }
                    break;
                }
            }
        }
        if (v6) {
            wp->alertTick--;
            if (wp->alertTick <= 0) {
                wp->alertTick = 60;
                if (wp->commandMenu.flags & 1) {
                    psndSFXOn("SFX_BTL_PINCH1");
                }
            }
        } else {
            unit = BattleGetMarioPtr(wp);
            if (unit) {
                unit->work[8] = 0;
            }
            wp->alertTick = 0;
        }
    }
}

void btlseqInit(BattleWork* wp) {
    MarioWork* mario;
    PartyEntry* party;

    switch (BattleGetSeq(wp, SEQ_INIT)) {
        case 0x1000000: //continues through to 0x1000001
            BattleSetSeq(wp, SEQ_INIT, 0x1000001);

        case 0x1000001:
            mario = marioGetPtr();
            mario->dispFlags |= MARIO_DISP_FLAG_HIDE;
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
            }
            Btl_UnitSetup(wp);
            BattleSetSeq(wp, SEQ_INIT, 0x1000002);
            break;

        case 0x1000002:
            if (BattleCheckEndUnitInitEvent(wp)) {
                BattleSetSeq(wp, SEQ_INIT, 0x1000003u);
            }
            break;

        case 0x1000003:
            if (animEffectAsync("p_b_st", 1) &&
                animEffectAsync("p_jyabara", 1) &&
                animEffectAsync("p_kaiten_h", 1) &&
                effNiceAsync(1)) {
                BattleSetSeq(wp, SEQ_INIT, 0x1000004);
            }
            break;

        case 0x1000004:
            BattleSetSeq(wp, SEQ_INIT, 0x1000005);
            break;

        case 0x1000005:
            BattleStatusWindowEventOn();
            BattleStatusWindowSystemOn();
            BattleSetSeq(wp, SEQ_UNKNOWN, 1);
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000000);
            break;
    }
}

BattleWeapon* _GetFirstAttackWeapon(s32 type) {
    switch (type) {
        case 1:
            return (BattleWeapon*)marioDefaultWeapon_Jump;
        case 2:
            return (BattleWeapon*)marioWeapon_KururinJump;
        case 3:
            return (BattleWeapon*)marioDefaultWeapon_Jump;
        case 4:
            return (BattleWeapon*)marioDefaultWeapon_Hammer;
        case 5:
            return &marioDefaultWeapon_FirstAttackKaitenHammer;
        case 6:
            return &marioDefaultWeapon_FirstAttackUltraHammer;
        case 7:
            return &partyWeapon_NokotarouFirstAttack;
        case 8:
            return &partyWeapon_SandersFirstAttack;
        case 0:
        default:
            return NULL;
    }
}



void btlseqFirstAct(BattleWork* wp) {
    s32 type;
    int i;
    BattleUnit* unit;
    BattleUnit* a;
    BattleUnit* b;
    EventEntry* evt;
    int found;
    s32 flag;
    int k;
    FieldBattleInfo* fb = wp->fbatInfo;
    u8* info;
    type = (s32)fb->partyId;

    switch (BattleGetSeq(wp, SEQ_FIRST_ACT)) {
        case 0x2000000:
            switch (type) {
                case 0: {
                        wp->ruleDispTimer = 0;
                        evtEntry(_default_haikei_set_entry_event, 10, 0);
                        info = (u8*)wp->fbatInfo->setup;
                        for (i = 0; i < 64; i++) {
                            unit = BattleGetUnitPtr(wp, i);
                            if (unit && unit->entryEventCode) {
                                if ((((NpcBattleInfo*)info)->battleFlags & NPC_BATTLE_NO_PARTY_ENTRY) && (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX))) {
                                    continue;
                                }
                                if (marioGetPtr()->currMotionId == MARIO_MOTION_YOSHI && unit->currentKind == UNIT_MARIO) {
                                    evt = evtEntry((s32*)BtlUnit_GetData(unit, UNIT_DATA_RIDE_ENTRY_EVENT), 10, 0);
                                    unit->entryEventId = evt->eventId;
                                    evt->unitId = unit->unitId;
                                } else {
                                    evt = evtEntry(unit->entryEventCode, 10, 0);
                                    unit->entryEventId = evt->eventId;
                                    evt->unitId = unit->unitId;
                                }
                            }
                        }
                        BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000001);
                        goto done_fa;
                    }
            }
            switch (type) {
                case 1: case 2: case 3:
                    wp->ruleDispTimer = 10;
                    break;
                case 4: case 5: case 6: case 7: case 8:
                    wp->ruleDispTimer = 0;
                    break;
                default:
                    wp->ruleDispTimer = 0;
                    break;
            }
            wp->flags |= BATTLE_FLAG_FIRST_ATTACK;
            BattleGetFirstAttackUnit(wp, &a, &b);
            wp->activeUnitId = a->unitId;
            a->mTokenFlags |= kCurrentlyActing;
            evt = evtEntry(b->damageEventCode, 10, EVENT_START_IMMEDIATE);
            if (evt) {
                evt->unitId = b->unitId;
                b->damageCode = DAMAGE_CODE_FIRST_ATTACK_POS;
                b->damageEventId = evt->eventId;
            }
            a->flags |= BATTLE_UNIT_FLAG_ACTING;
            if (type < 9) {
                a->attackEventCode = (void*)_GetFirstAttackWeapon(type)->mAttackEvtScript;
                a->attackEventFlags = 0;
                BattleStatusWindowEventOff();
            }
            if (BtlUnit_GetBelong(a) == 0) {
                BattleAudience_Case_FirstAttack_Good();
            } else {
                BattleAudience_Case_FirstAttack_Bad();
            }
            evt = evtEntry(a->attackEventCode, 10, 0);
            if (evt) {
                evt->unitId = a->unitId;
                a->attackEventId = evt->eventId;
            }
            switch (type) {
                case 1: case 2: case 3: case 4: case 5: case 6:
                    unit = BattleGetPartyPtr(wp);
                    if (unit) {
                        evt = evtEntry(unit->entryEventCode, 10, 0);
                        unit->entryEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    break;
                case 7: case 8:
                    unit = BattleGetMarioPtr(wp);
                    if (unit) {
                        evt = evtEntry(unit->entryEventCode, 10, 0);
                        unit->entryEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
            }
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000001);
            done_fa:
            break;

        case 0x2000001:
            if (--wp->ruleDispTimer > 0) {
                break;
            }
            {
                fadeEntry(0x17, 200, c);
            }
            wp->curtainSfxId = psndSFXOn("SFX_OFF_BTL_START_CURTAIN_UP1");
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000002);

        case 0x2000002:
            if (fadeIsFinish() && (u32)wp->curtainSfxId != 0xFFFFFFFFU) {
                psndSFXOff(wp->curtainSfxId);
                wp->curtainSfxId = -1;
            }
            if (BattleWaitAllActiveEvtEnd(wp)) {
                BattleStatusWindowEventOn();
            }
            if (BattleWaitAllActiveEvtEnd(wp) && fadeIsFinish()) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000003);
                unit = BattleGetUnitPtr(wp, wp->activeUnitId);
                if (unit) {
                    unit->flags &= ~BATTLE_UNIT_FLAG_ACTING;
                }
            }
            break;

        case 0x2000003:
            if (BattleCheckConcluded(wp)) {
                BattleAudience_PerAct();
                BattleSetSeq(wp, SEQ_UNKNOWN, 3);
                BattleSetSeq(wp, SEQ_END, 0x7000000);
                break;
            }
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000004);
            BattleStatusChangeMsgAdjust(wp);

        case 0x2000004:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            if (BattleStatusChangeAnnouceMain(wp) | BattleStatusChangeMsgMain(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_FIRST_ACT);

        case 0x2000005:
            if (!(wp->flags & (BATTLE_FLAG_ESCAPED | BATTLE_FLAG_INTERRUPT_STOP))) {
                BattleStatusWindowSystemOn();
            }
            BattleConsumeReserveItem();
            wp->flags &= ~BATTLE_FLAG_ACT_DECIDED_UNUSED;
            BattleAudience_PerAct();
            BattleIncSeq(wp, SEQ_FIRST_ACT);

        case 0x2000006:
            if (BattleAudience_CheckReaction()) {
                break;
            }
            if (!BattleCheckConcluded(wp) && (u8)BattleBreakSlot_CheckReaction()) {
                break;
            }
            BattleIncSeq(wp, SEQ_FIRST_ACT);

        case 0x2000007:
            if (BattleStatusChangeAnnouceMain(wp) | BattleStatusChangeMsgMain(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_FIRST_ACT);

        case 0x2000008:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            if ((u32)wp->fbatInfo->setup->battleFlags & NPC_BATTLE_NO_ESCAPE) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000009);
                BattleStage_WallCloseCheck();
            } else {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000A);
            }
            break;

        case 0x2000009: {
                s32* id = &wp->stageObjectWork.closeWallEvtId;
                if (*id != 0 && !evtCheckID(*id)) {
                    *id = 0;
                }
                if (*id == 0) {
                    BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000A);
                }
            }
            break;

        case 0x200000A:
            wp->luckEventId = 0;
            flag = 0;
            k = 0;
            do {
                unit = BattleGetUnitPtr(wp, k);
                if (unit && unit->badgesEquipped.LuckyStart && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                    flag = 1;
                }
            } while (++k < 64);
            if (flag) {
                evt = evtEntry(nanika_luck_event, 10, 0);
                evt->unitId = -5;
                wp->luckEventId = evt->eventId;
                *(s64*)&wp->announce.stChg.maskHi |= -1;
                BattleStatusChangeMsgAdjust(wp);
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000B);
                wp->luckTimer = 0;
            } else {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000C);
            }
            break;

        case 0x200000B:
            found = 1;
            if (wp->luckTimer < 300) {
                wp->luckTimer++;
            }
            if (wp->luckEventId != 0) {
                if (evtCheckID(wp->luckEventId)) {
                    found = 0;
                } else {
                    wp->luckEventId = 0;
                }
            }
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                found = 0;
            }
            if (BattleStatusChangeAnnouceMain(wp)) {
                found = 0;
            }
            if (BattleStatusChangeMsgMain(wp)) {
                found = 0;
            }
            if (found) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000C);
            }
            break;

        case 0x200000C:
            if (wp->flags & BATTLE_FLAG_FIRST_ATTACK) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000E);
                break;
            }
            BattleStage_NozzleWorkCheck(0);
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000D);

        case 0x200000D:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000E);

        case 0x200000E:
            wp->flags &= ~BATTLE_FLAG_FIRST_ATTACK;
            BattleSetSeq(wp, SEQ_UNKNOWN, 2);
            BattleSetSeq(wp, SEQ_TURN, 0x3000000);
            break;
    }
}

//==== written below (reverse order) ====

USER_FUNC(_set_effect_luck) {
    BattleWork* wp = _battleWorkPointer;
    f32* pz;
    f32* py;
    BattleWeapon weapon = *&N_weapon_lucky_start;
    f32 z;
    f32 y;
    BattleWeapon* pw;
    f32 x;
    const char* sfx;
    BattleUnit* unit;
    BattleUnitPart* part;
    NiceEffectWork* effect;
    int i;

    // FAKE: comma temps so the pointers are coloured in assignment order (r31 pw ... r28 sfx)
    pw = (0, &weapon);
    pz = (0, &z);
    py = (0, &y);
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfx = (0, "SFX_SYSTEM_LUCKY1");
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->badgesEquipped.LuckyStart && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            switch (irand(4)) {
                case 0:
                    weapon.dodgyChance = 0;
                    weapon.dodgyTime = 0;
                    weapon.hpRegenTime = 0;
                    weapon.hpRegenStrength = 0;
                    weapon.fpRegenTime = 0;
                    weapon.fpRegenStrength = 0;
                    break;
                case 1:
                    weapon.electricChance = 0;
                    weapon.electricTime = 0;
                    weapon.hpRegenTime = 0;
                    weapon.hpRegenStrength = 0;
                    weapon.fpRegenTime = 0;
                    weapon.fpRegenStrength = 0;
                    break;
                case 2:
                    weapon.electricChance = 0;
                    weapon.electricTime = 0;
                    weapon.dodgyChance = 0;
                    weapon.dodgyTime = 0;
                    weapon.fpRegenTime = 0;
                    weapon.fpRegenStrength = 0;
                    break;
                default:
                    weapon.electricChance = 0;
                    weapon.electricTime = 0;
                    weapon.dodgyChance = 0;
                    weapon.dodgyTime = 0;
                    weapon.hpRegenTime = 0;
                    weapon.hpRegenStrength = 0;
                    break;
            }
            part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            BattleSetStatusDamageFromWeapon(BattleGetSystemPtr(wp), unit, part, pw, 0x100);
            BtlUnit_GetPos(unit, &x, py, pz);
            psndSFXOn(sfx);
            {
                EffectEntry* e = effNiceEntry(6, x - 45.0f, 60.0f + y, z);
                effect = e->userdata;
                effect->overallScale = 0.75f;
            }
        }
    }
    return EVT_RETURN_DONE;
}

#define TURN_NEXT(wp) \
    BattleIncSeq(wp, SEQ_TURN); \
    btlseqTurn(wp)

#define TURN_SKIP(wp) \
    BattleIncSeq(wp, SEQ_TURN); \
    BattleIncSeq(wp, SEQ_TURN); \
    btlseqTurn(wp)

#define TURN_WAIT_CASE(wp) \
    if (!BattleWaitAllActiveEvtEnd(wp)) { \
        break; \
} \
    if (BattleCheckConcluded(wp)) { \
        BattleSetSeq(wp, SEQ_UNKNOWN, 3); \
        BattleSetSeq(wp, SEQ_END, 0x7000000); \
} \
    else { \
        BattleIncSeq(wp, SEQ_TURN); \
} \
    break

#define TURN_RECOVERY_CASE(wp, status) \
    for (i = 0; i < 64; i++) { \
        unit = BattleGetUnitPtr(wp, i); \
        ret = BtlUnit_CheckRecoveryStatus(unit, status); \
        sfx |= ret; \
        if (ret) { \
            evt = evtEntry(subsetevt_status_recovery, 10, 0); \
            if (evt) { \
                unit->attackEventId = evt->eventId; \
                evt->unitId = unit->unitId; \
        } \
        found = TRUE; \
    } \
} \
    if (sfx) { \
        psndSFXOn("SFX_REST_CONDITION1"); \
} \
    if (!found) { \
        TURN_SKIP(wp); \
        break; \
} \
    TURN_NEXT(wp); \
    break

#define TURN_RECOVERY_CASE2(wp, status) \
    for (i = 0; i < 64; i++) { \
        unit = BattleGetUnitPtr(wp, i); \
        if (unit) { \
            ret = BtlUnit_CheckRecoveryStatus(unit, status); \
            sfx |= ret; \
            if (ret) { \
                evt = evtEntry(subsetevt_status_recovery, 10, 0); \
                if (evt) { \
                    unit->attackEventId = evt->eventId; \
                    evt->unitId = unit->unitId; \
            } \
            found = TRUE; \
        } \
    } \
} \
    if (sfx) { \
        psndSFXOn("SFX_REST_CONDITION1"); \
} \
    if (!found) { \
        TURN_SKIP(wp); \
        break; \
} \
    TURN_NEXT(wp); \
    break

#define TURN_COMMAND_DISABLED_CASE(wp, n) \
    if (wp->commandDisabled[n]) { \
        wp->commandDisabled[n]--; \
} \
    TURN_SKIP(wp); \
    break

void btlseqTurn(BattleWork *wp) {
    Vec pos;
    f32 hpX;
    f32 hpY;
    f32 hpZ;
    f32 fpX;
    f32 fpY;
    f32 fpZ;
    f32 heartX;
    f32 heartY;
    f32 heartZ;
    f32 flowerX;
    f32 flowerY;
    f32 flowerZ;
    s8 turns;
    s8 strength;
    BattleUnit *unit;
    EventEntry *evt;
    BOOL found;
    BOOL sfx;
    const char *sfxName;
    BOOL ret;
    BOOL done;
    s32 count;
    s32 n;
    s32 rate;
    int i;
    found = 0;
    sfx = 0;
    switch (BattleGetSeq(wp, SEQ_TURN)) {
        case 0x3000000:
            BattleIncSeq(wp, SEQ_TURN);
            break;

        case 0x3000001:
            btlseqTurnFirstProcess(wp);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000003:
            if (wp->turnCount != 1) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            if (wp->fbatInfo->ruleType == 0) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            wp->ruleDispTimer = 120;
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000004:
            if (wp->ruleDispTimer) {
                wp->ruleDispTimer--;
            }
            if ((wp->ruleDispTimer <= 0) || BattlePadCheckTrigger(PAD_BUTTON_A | PAD_BUTTON_B)) {
                BattleIncSeq(wp, SEQ_TURN);
                break;
            }
            dispEntry(CAMERA_2D, 900.0f, 1, _rule_disp, NULL);
            break;

        case 0x3000002:
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000005:
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000007:

        case 0x300000B:

        case 0x300000D:

        case 0x300000F:

        case 0x3000011:

        case 0x3000013:

        case 0x3000015:

        case 0x3000017:

        case 0x3000019:

        case 0x300001B:

        case 0x300001D:

        case 0x300001F:

        case 0x3000021:

        case 0x3000023:

        case 0x3000025:

        case 0x3000027:

        case 0x3000029:

        case 0x300002B:

        case 0x300002D:

        case 0x300002F:

        case 0x3000031:

        case 0x3000033:

        case 0x3000035:

        case 0x3000037:

        case 0x3000039:

        case 0x300003B:

        case 0x300003D:

        case 0x300003F:

        case 0x3000041:

        case 0x3000043:

        case 0x3000045:

        case 0x3000047:

        case 0x3000049:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            if (BattleCheckConcluded(wp)) {
                BattleSetSeq(wp, SEQ_UNKNOWN, 3);
                BattleSetSeq(wp, SEQ_END, 0x7000000);
            } else {
                BattleIncSeq(wp, SEQ_TURN);
            }
            break;

        case 0x3000012:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_SLEEP);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                sfxName = "SFX_REST_CONDITION1";
                psndSFXOn(sfxName);
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            break;

        case 0x3000014:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_STOP);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000016:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_DIZZY);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000018:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_CONFUSE);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300001A:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_ELECTRIC);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300001C:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_DODGY);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300001E:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_BURN);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
                dummy_label_899231: ;
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000020: {
                BattleUnit *unit;
                sfxName = "SFX_CONDITION_BREAK_ICE1";
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (BtlUnit_CheckRecoveryStatus(unit, STATUS_FREEZE)) {
                        if (!BtlUnit_CheckShadowGuard(unit)) {
                            BattleDamageDirect(-5, unit, NULL, 1, 0, 0x11A, 0, 1);
                        }
                        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
                        pos.y += BtlUnit_GetHeight(unit);
                        effFreezeN64Entry(0, 1, pos.x, pos.y, pos.z, 1.0f);
                        psndSFXOn_3D(sfxName, &pos);
                        found = 1;
                    }
                }

                if (!found) {
                    BattleIncSeq(wp, SEQ_TURN);
                    BattleIncSeq(wp, SEQ_TURN);
                    btlseqTurn(wp);
                    break;
                }
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }

        case 0x3000022:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_HUGE);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000024:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_TINY);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000026:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_POISON);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000028:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_ATTACK_UP);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300002A:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_ATTACK_DOWN);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300002C:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_DEFENSE_UP);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300002E:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_DEFENSE_DOWN);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000030:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_ALLERGIC);
                sfx |= ret;
                if (ret) {
                    evt = evtEntry(subsetevt_status_recovery, 10, 0);
                    if (evt) {
                        unit->attackEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                    }
                    found = 1;
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000032:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_HP_REGEN);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000034:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_FP_REGEN);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000036:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_INVISIBLE);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000038:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_FAST);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300003A:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_SLOW);
                    sfx |= ret;
                    if (ret) {
                        BtlUnit_ResetMoveStatus(unit);
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300003C:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_PAYBACK);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300003E:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_HOLD_FAST);
                    sfx |= ret;
                    if (ret) {
                        evt = evtEntry(subsetevt_status_recovery, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (sfx) {
                psndSFXOn("SFX_REST_CONDITION1");
            }
            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000006:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_POISON, &turns, &strength);
                    if ((strength > 0) && (!BtlUnit_CheckShadowGuard(unit))) {
                        found = 1;
                        BattleDamageDirect(-5, unit, NULL, strength, 0, 0x117, 0, 1);
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000008:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_BURN, &turns, &strength);
                    if (strength > 0) {
                        found = 1;
                        break;
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            wp->turnSubSeq = 0;
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000009:
            switch (wp->turnSubSeq) {
                case 0:
                    wp->turnSubSeq++;
                    for (i = 0; i < 64; i++) {
                        unit = BattleGetUnitPtr(wp, i);
                        if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                            BtlUnit_GetStatus(unit, STATUS_BURN, &turns, &strength);
                            if (strength > 0) {
                                BSE_TurnFirstProcessEffectEntry(unit, 1);
                            }
                        }
                    }

                case 1:
                    done = 1;
                    for (i = 0; i < 64; i++) {
                        unit = BattleGetUnitPtr(wp, i);
                        if (unit && BSE_TurnFirstProcessEffectMain(unit)) {
                            done = 0;
                        }
                    }

                    if (done) {
                        wp->turnSubSeq++;
                    }
                    break;

                default:
                    if (!BattleWaitAllActiveEvtEnd(wp)) {
                        break;
                    }
                    if (BattleCheckConcluded(wp)) {
                        BattleSetSeq(wp, SEQ_UNKNOWN, 3);
                        BattleSetSeq(wp, SEQ_END, 0x7000000);
                    } else {
                        BattleIncSeq(wp, SEQ_TURN);
                    }
                    break;

            }

            break;

        case 0x300000A:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_HP_REGEN, &turns, &strength);
                    if (strength > 0) {
                        if (!(unit->mAttributeFlags & kUnitHpFixed)) {
                            unit->hpGauge.prevHp = unit->currentHp;
                            BtlUnit_RecoverHp(unit, strength);
                            unit->hpGauge.targetHp = unit->currentHp;
                            unit->hpGauge.drainTimer = unit->hpGauge.drainLength;
                            unit->hpGauge.visibleTimer = unit->hpGauge.visibleLength;
                            unit->hpGauge.flags |= 1;
                        } else {
                            BtlUnit_RecoverHp(unit, strength);
                        }
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &hpX, &hpY, &hpZ);
                        hpY += unit->mSizeMultiplier * (BtlUnit_GetHeight(unit) + 5);
                        effRecoveryEntry(0, strength, hpX, hpY, hpZ);
                        evt = evtEntry(subsetevt_regeneration, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300000C:
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_FP_REGEN, &turns, &strength);
                    if (strength > 0) {
                        BtlUnit_RecoverFp(unit, strength);
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &fpX, &fpY, &fpZ);
                        fpY += unit->mSizeMultiplier * (BtlUnit_GetHeight(unit) + 5);
                        effRecoveryEntry(1, strength, fpX, fpY, fpZ);
                        evt = evtEntry(subsetevt_regeneration, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300000E:
            if (wp->turnCount <= 1) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    n = 0;
                    for (count = unit->badgesEquipped.HappyHeart; count > 0; count--) {
                        rate = 33 - ((unit->currentHp / unit->maxHp) * 3);
                        if (irand(100) < rate) {
                            n++;
                        }
                    }

                    if (n > 0) {
                        unit->currentHp += n;
                        if (unit->currentHp > unit->maxHp) {
                            unit->currentHp = unit->maxHp;
                        }
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &heartX, &heartY, &heartZ);
                        heartY += unit->mSizeMultiplier * BtlUnit_GetHeight(unit);
                        effRecoveryEntry(0, n, heartX, heartY, heartZ);
                        evt = evtEntry(subsetevt_happy_heart, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            break;

        case 0x3000010:
            if (wp->turnCount <= 1) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL))) && (unit->currentHp > 0)) {
                    n = 0;
                    for (rate = unit->badgesEquipped.HappyFlower; rate > 0; rate--) {
                        count = 33 - ((unit->currentFp / unit->maxFp) * 3);
                        if (irand(100) < count) {
                            n++;
                        }
                    }

                    if (n > 0) {
                        BtlUnit_RecoverFp(unit, n);
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &flowerX, &flowerY, &flowerZ);
                        flowerY += unit->mSizeMultiplier * BtlUnit_GetHeight(unit);
                        effRecoveryEntry(1, n, flowerX, flowerY, flowerZ);
                        evt = evtEntry(subsetevt_happy_flower, 10, 0);
                        if (evt) {
                            unit->attackEventId = evt->eventId;
                            evt->unitId = unit->unitId;
                        }
                        found = 1;
                    }
                }
            }

            if (!found) {
                BattleIncSeq(wp, SEQ_TURN);
                BattleIncSeq(wp, SEQ_TURN);
                btlseqTurn(wp);
                break;
            }
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000040:
            if (wp->commandDisabled[0]) {
                wp->commandDisabled[0]--;
            }
            BattleIncSeq(wp, SEQ_TURN);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000042:
            if (wp->commandDisabled[1]) {
                wp->commandDisabled[1]--;
            }
            BattleIncSeq(wp, SEQ_TURN);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000044:
            if (wp->commandDisabled[2]) {
                wp->commandDisabled[2]--;
            }
            BattleIncSeq(wp, SEQ_TURN);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000046:
#ifdef VERSION_G8ME01
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) && unit->currentHp > 0 && unit->usCommandDisabled[0]) {
                    unit->usCommandDisabled[0]--;
                }
            }
#else
            if (wp->commandDisabled[3]) {
                wp->commandDisabled[3]--;
            }
#endif
            BattleIncSeq(wp, SEQ_TURN);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x3000048:
#ifdef VERSION_G8ME01
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) && unit->currentHp > 0 && unit->usCommandDisabled[1]) {
                    unit->usCommandDisabled[1]--;
                }
            }
#else
            if (wp->commandDisabled[4]) {
                wp->commandDisabled[4]--;
            }
#endif
            BattleIncSeq(wp, SEQ_TURN);
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300004A:
            BattleIncSeq(wp, SEQ_TURN);
            btlseqTurn(wp);
            break;

        case 0x300004B:
            BattleIncSeq(wp, SEQ_TURN);
            BattleSetSeq(wp, SEQ_PHASE, 0x4000000);
            break;

        case 0x300004C:
            btlseqPhase(wp);
            break;

        case 0x300004D:
            BattleAudience_Case_TurnEnd();
            BattleSetSeq(wp, SEQ_TURN, 0x3000001);
            wp->flags &= ~BATTLE_FLAG_RESET_TURN;
            break;

    }

}

void _rule_disp(CameraId cameraId, void* param) {
    FieldBattleInfo* info = _battleWorkPointer->fbatInfo;
    char buf[256];
    f32 x;
    f32 y;
    u16 lines;
    // FAKE: s32 width holding the u16 result, halved as (u16)width / 2 (twin _get_item_announce_disp shape)
    s32 width;

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 0xAC) {
        if (evtGetValue(NULL, GSW_GLITZ_PIT_RANK) < 11) {
            sprintf(buf, msgSearch(_rule_msg_table_1bu[info->ruleType]), info->ruleParam);
        } else {
            sprintf(buf, msgSearch(_rule_msg_table_2bu[info->ruleType]), info->ruleParam);
        }
    } else {
        sprintf(buf, msgSearch(_rule_msg_table_after[info->ruleType]), info->ruleParam);
    }
    width = FontGetMessageWidthLine(buf, &lines);
    lines++;
    x = 0.0f;
    y = 120.0f;
    windowDispGX_Waku_col(0, color, (x - (u16)width / 2) - 10.0f, 94.0f, width + 20, lines * 29 + 3, 10.0f);
    FontDrawStart();
    FontDrawMessage(x - (u16)width / 2, (s32)y - 29, buf);
}

void btlseqPhase(BattleWork* wp) {
    switch (BattleGetSeq(wp, SEQ_PHASE)) {
        case 0x4000000:
            BattleIncSeq(wp, SEQ_PHASE);
            BattleSetSeq(wp, SEQ_MOVE, 0x5000000);
            break;
        case 0x4000001:
        case 0x4000002:
        case 0x4000003:
        case 0x4000004:
        case 0x4000005:
            btlseqMove(wp);
            break;
        case 0x4000006:
            BattleIncSeq(wp, SEQ_TURN);
            BattleSetSeq(wp, SEQ_PHASE, 0x4000000);
            break;
    }
}

void btlseqMove(BattleWork* wp) {
    int j;
    BattleUnit* unit;
    PouchData* pouch;
    BattleUnit* mario;
    EventEntry* evt;
    BOOL loop;
    s32 id;
    s32 state;
    BOOL end;
    int i;

    do {
        loop = FALSE;
        switch (BattleGetSeq(wp, SEQ_MOVE)) {
            case 0x5000000:
                btlseqPhaseFirstProcess(wp);
                BattleIncSeq(wp, SEQ_MOVE);
                loop = TRUE;
                break;
            case 0x5000001:
                BattleIncSeq(wp, SEQ_MOVE);
                loop = TRUE;
                break;
            case 0x5000002:
                pouch = pouchGetPtr();
                mario = BattleGetMarioPtr(wp);
                if (pouch->mMerleeNextCurseType != 2 || BattleGetSeq(wp, SEQ_PHASE) != 0x4000004) {
                    BattleIncSeq(wp, SEQ_MOVE);
                    BattleIncSeq(wp, SEQ_MOVE);
                    loop = TRUE;
                    break;
                }
                wp->majinaiState = 2;
                BattleMajinaiDone();
                BattleMajinaiEndCheck();
                evt = evtEntry(marioAttackEvent_MajinaiDefenceUp, 10, 0);
                evt->unitId = mario->unitId;
                mario->attackEventId = evt->eventId;
                BattleIncSeq(wp, SEQ_MOVE);
            case 0x5000003:
                if (!BattleWaitAllActiveEvtEnd(wp)) {
                    break;
                }
                BattleIncSeq(wp, SEQ_MOVE);
                loop = TRUE;
                break;
            case 0x5000004:
                for (j = 0; j < 64; j++) {
                    unit = BattleGetUnitPtr(wp, j);
                    if (unit && unit->unisonPhaseEventCode) {
                        BattleRunPhaseEvent(unit, TRUE);
                    }
                }
                BattleIncSeq(wp, SEQ_MOVE);
                loop = TRUE;
                break;
            case 0x5000005:
                if (!BattleWaitAllActiveEvtEnd(wp)) {
                    break;
                }
                BattleIncSeq(wp, SEQ_MOVE);
                loop = TRUE;
                break;
            case 0x5000006:
                for (i = 0; i < 64; i++) {
                    if (wp->phaseEvtTable[i].unitId != -1) {
                        unit = BattleGetUnitPtr(wp, wp->phaseEvtTable[i].unitId);
                        if (unit && unit->phaseEventCode) {
                            break;
                        }
                    }
                }
                if (i < 64) {
                    BattleRunPhaseEvent(unit, FALSE);
                    wp->phaseEvtTable[i].unitId = -1;
                    BattleIncSeq(wp, SEQ_MOVE);
                } else {
                    BattleSetSeq(wp, SEQ_MOVE, 0x500000A);
                    loop = TRUE;
                }
                break;
            case 0x5000007:
                if (!BattleWaitAllActiveEvtEnd(wp)) {
                    break;
                }
                if (BattleCheckConcluded(wp)) {
                    BattleSetSeq(wp, SEQ_UNKNOWN, 3);
                    BattleSetSeq(wp, SEQ_END, 0x7000000);
                } else {
                    BattleSetSeq(wp, SEQ_MOVE, 0x5000006);
                    loop = TRUE;
                }
                break;
            case 0x5000008:
                memset(&wp->impendingBonuses, 0, sizeof(wp->impendingBonuses));
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (unit) {
                        unit->mTokenFlags &= 0x80002000;
                        unit->totalHpDamageTaken = 0;
                        unit->hpDamageTaken = 0;
                        unit->hpDamageCount = 0;
                        unit->superguardCountTaken = 0;
                        unit->totalFpDamageTaken = 0;
                        unit->fpDamageTaken = 0;
                        unit->totalFpLost = 0;
                        unit->fpLost = 0;
                        unit->superguardedCount = 0;
                    }
                }
                for (i = 0; i < 64; i++) {
                    id = wp->movePriorityQueue[i];
                    if (id != -1) {
                        unit = BattleGetUnitPtr(wp, id);
                        if (unit && unit->movesRemaining > 0) {
                            state = unit->moveState;
                            if (state != 10 && (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) || (unit->mAttributeFlags & kUndead))) {
                                if (state != 1) {
                                    break;
                                }
                                unit->moveState = 0;
                            }
                        }
                    }
                }
                wp->activeUnitId = id;
                BattleGetUnitPtr(wp, id)->mTokenFlags |= kCurrentlyActing;
                BattleIncSeq(wp, SEQ_MOVE);
                BattleSetSeq(wp, SEQ_ACT, 0x6000000);
                for (i = 0; i < 64; i++) {
                    id = wp->movePriorityQueue[i];
                    if (id != -1) {
                        unit = BattleGetUnitPtr(wp, id);
                        if (unit && unit->moveState == 1) {
                            unit->moveState = 0;
                        }
                    }
                }
                unit = BattleGetUnitPtr(wp, wp->activeUnitId);
                if (unit->currentKind == UNIT_MARIO) {
                    pouch = pouchGetPtr();
                    wp->flags |= (BATTLE_FLAG_JUMP_NO_SUBMENU | BATTLE_FLAG_HAMMER_NO_SUBMENU);
                    for (i = 0; i < 200; i++) {
                        if (pouch->equippedBadges[i] >= 0xF0 && pouch->equippedBadges[i] < 0xF7) {
                            wp->flags &= ~BATTLE_FLAG_JUMP_NO_SUBMENU;
                            break;
                        }
                    }
                    if (pouch->jumpLevel >= 2) {
                        wp->flags &= ~BATTLE_FLAG_JUMP_NO_SUBMENU;
                    }
                    for (i = 0; i < 200; i++) {
                        if (pouch->equippedBadges[i] >= 0xF7 && pouch->equippedBadges[i] < 0xFE) {
                            wp->flags &= ~BATTLE_FLAG_HAMMER_NO_SUBMENU;
                            break;
                        }
                    }
                    if (pouch->hammerLevel >= 2) {
                        wp->flags &= ~BATTLE_FLAG_HAMMER_NO_SUBMENU;
                    }
                }
                break;
            case 0x5000009:
                btlseqAct(wp);
                break;
            case 0x500000A:
                if (BattleCheckConcluded(wp)) {
                    BattleSetSeq(wp, SEQ_UNKNOWN, 3);
                    BattleSetSeq(wp, SEQ_END, 0x7000000);
                    break;
                }
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (unit) {
                        BtlUnit_SetTotalHitDamage(unit, 0);
                        unit->mTokenFlags &= ~kCurrentlyActing;
                    }
                }
                end = BattlePhaseEndCheck();
                if (wp->flags & BATTLE_FLAG_RESET_TURN) {
                    end = TRUE;
                }
                if (end) {
                    if (wp->majinaiState == 2) {
                        wp->majinaiState = 0;
                    }
                    BattleAudience_PerPhase(BattleGetSeq(wp, SEQ_PHASE));
                    BattleIncSeq(wp, SEQ_MOVE);
                } else {
                    BattleSetSeq(wp, SEQ_MOVE, 0x5000008);
                }
                break;
            case 0x500000B:
                if (BattleAudience_CheckReactionPerPhase()) {
                    break;
                }
                if (!BattleWaitAllActiveEvtEnd(wp)) {
                    break;
                }
                BattleIncSeq(wp, SEQ_MOVE);
                break;
            case 0x500000C:
                if (BattleStatusChangeAnnouceMain(wp) | BattleStatusChangeMsgMain(wp)) {
                    break;
                }
                BattleIncSeq(wp, SEQ_PHASE);
                BattleSetSeq(wp, SEQ_MOVE, 0x5000000);
                break;
            case 0x500000D:
                BattleIncSeq(wp, SEQ_MOVE);
                break;
            case 0x500000E:
                BattleIncSeq(wp, SEQ_PHASE);
                BattleSetSeq(wp, SEQ_MOVE, 0x5000000);
                break;
        }
    } while (loop);
}

BOOL BattlePhaseEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    int i;

    for (i = 0; i < 64; i++) {
        if (wp->movePriorityQueue[i] != -1) {
            unit = BattleGetUnitPtr(wp, wp->movePriorityQueue[i]);
            if (unit && unit->moveState != 10 && unit->movesRemaining > 0 &&
                !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                break;
            }
        }
    }
    if (i >= 64) {
        return TRUE;
    }
    return FALSE;
}

void btlseqAct(BattleWork* wp) {
    BattleUnit* unit;
    EventEntry* evt;
    BOOL decrement;

    switch (BattleGetSeq(wp, SEQ_ACT)) {
        case 0x6000000: {
                BattleUnit* unit;
                BOOL cantMove;

                unit = BattleGetUnitPtr(wp, wp->activeUnitId);
                effNiceAsync(1);
                unit->totalDamageDealt = 0;
                unit->hitEventCode = NULL;
                BattleAudience_ActInit();
                cantMove = TRUE;
                switch (unit->currentKind) {
                    case UNIT_MARIO:
                        if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) &&
                            !BtlUnit_CheckStatus(unit, STATUS_STOP) &&
                            !BtlUnit_CheckStatus(unit, STATUS_FREEZE) &&
                            !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                            cantMove = FALSE;
                        }
                        break;
                    default:
                        if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
                            if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) &&
                                !BtlUnit_CheckStatus(unit, STATUS_STOP) &&
                                !BtlUnit_CheckStatus(unit, STATUS_FREEZE) &&
                                !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                                cantMove = FALSE;
                            }
                        }
                        break;
                }
                if (cantMove) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000003);
                } else if (unit->mAttributeFlags & kUnitInVeil) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000004);
                } else if (unit->currentKind == UNIT_VIVIAN && (BattleGetMarioPtr(wp)->mAttributeFlags & kUnitInVeil)) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000004);
                } else if (unit->flippedTurns) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000005);
                } else {
                    BattleCommandInit(wp);
                    BattleSetSeq(wp, SEQ_ACT, 0x6000001);
                }
                break;
        }
        case 0x6000001:
            if (battleCameraGetPosMoveSpeed() > 2.0f) {
                break;
            }
            BattleSetSeq(wp, SEQ_ACT, 0x6000002);
        case 0x6000002:
            if (!BattleCommandInput(wp)) {
                break;
            }
            wp->flags &= ~BATTLE_FLAG_COMMAND_INPUT;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000005:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            unit->attackEventCode = BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
            unit->attackEventFlags = 1;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000003:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            unit->attackEventFlags = 1;
            unit->flags |= BATTLE_UNIT_FLAG_ACTING;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000004:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            unit->attackEventCode = BtlUnit_GetData(unit, UNIT_DATA_HIDE_EVENT);
            unit->attackEventFlags = 0x1B;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000006:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            if (!(unit->attackEventFlags & 1)) {
                BattleStatusWindowSystemOff();
            }
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            evt = NULL;
            if ((wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED) && !(unit->mAttributeFlags & kActsInZeroGravity) &&
                !(unit->attackEventFlags & 8) && !(unit->attackEventFlags & 0x10)) {
                evt = evtEntry(btldefaultevt_CantMoveZeroGravity, 10, 0);
            } else if (unit->attackEventFlags & 0x10) {
                if (unit->attackEventCode) {
                    evt = evtEntry(unit->attackEventCode, 10, 0);
                }
            } else if (unit->attackEventFlags & 2) {
                evt = evtEntry(unit->attackEventCode, 10, 0);
            } else if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) &&
                !BtlUnit_CheckStatus(unit, STATUS_STOP) &&
                !BtlUnit_CheckStatus(unit, STATUS_FREEZE) &&
                (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) || (unit->mAttributeFlags & kUndead))) {
                if (BtlUnit_CheckStatus(unit, STATUS_ELECTRIC) && irand(100) < 0) {
                    evt = evtEntry(btldefaultevt_BiribiriMove, 10, 0);
                } else if (BtlUnit_CheckStatus(unit, STATUS_CONFUSE) && irand(100) < 50) {
                    unit->mTokenFlags |= kConfusedAction;
                    evt = evtEntry(unit->confusionEventCode, 10, 0);
                } else {
                    evt = evtEntry(unit->attackEventCode, 10, 0);
                }
            }
            if (evt) {
                unit->attackEventId = evt->eventId;
                evt->unitId = unit->unitId;
                if (unit->waitEventId) {
                    evtDeleteID(unit->waitEventId);
                    unit->waitEventId = 0;
                }
            }
            BattleIncSeq(wp, SEQ_ACT);
            break;
        case 0x6000007:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_ACT);
            BattleStatusChangeMsgAdjust(wp);
        case 0x6000008:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            if (BattleStatusChangeAnnouceMain(wp) | BattleStatusChangeMsgMain(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_ACT);
        case 0x6000009:
            if (!(wp->flags & (BATTLE_FLAG_ESCAPED | BATTLE_FLAG_INTERRUPT_STOP))) {
                BattleStatusWindowSystemOn();
            }
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            if (unit) {
                unit->flags &= ~BATTLE_UNIT_FLAG_ACTING;
                if (unit->mTokenFlags & kChargeConsumed) {
                    BtlUnit_SetStatus(unit, STATUS_CHARGE, 1, -99);
                    unit->mTokenFlags &= ~kChargeConsumed;
                }
                if (wp->majinaiState == 1) {
                    wp->majinaiState = 0;
                }
                unit->reserved_258 = 0;
                switch (unit->moveState) {
                    case 5:
                        unit->moveState = 6;
                        break;
                    case 6:
                        unit->moveState = 7;
                        break;
                    case 8:
                        unit->moveState = 9;
                        break;
                    case 7:
                    case 9:
                        unit->moveState = 0;
                        break;
                }
                if (unit->moveState == 0) {
                    decrement = TRUE;
                    if (wp->flags & BATTLE_FLAG_RESET_TURN) {
                        decrement = TRUE;
                    }
                    if (decrement) {
                        if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_HUSTLE)) {
                            BtlUnit_OffStatusFlag(unit, UNIT_STATUS_FLAG_HUSTLE);
                        } else {
                            unit->movesRemaining--;
                        }
                        if (wp->flags & BATTLE_FLAG_RESET_TURN) {
                            unit->movesRemaining = 0;
                        }
                        if (unit->movesRemaining <= 0) {
                            unit->moveState = 10;
                            if (!(unit->mTokenFlags & kHasRallyWinkStatus)) {
                                unit->activeTurns = 0;
                            } else {
                                unit->mTokenFlags &= ~kHasRallyWinkStatus;
                            }
                            if (!BtlUnit_GetBelong(unit) && unit->attackPhase == 0x4000002 &&
                                !BattlePhaseEndCheck() && !(wp->flags & BATTLE_FLAG_RESET_TURN)) {
                                unit->mMoveColorLv = 0x60;
                            }
                        }
                    }
                } else if (unit->moveState == 3) {
                    unit->moveState = 0;
                } else if (unit->moveState == 4) {
                    unit->moveState = 0;
                }
            }
            BattleConsumeReserveItem();
            wp->flags &= ~BATTLE_FLAG_ACT_DECIDED_UNUSED;
            BattleAudience_PerAct();
            BattleIncSeq(wp, SEQ_ACT);
        case 0x600000A:
            if (BattleAudience_CheckReaction()) {
                break;
            }
            if (!BattleCheckConcluded(wp) && (u8)BattleBreakSlot_CheckReaction()) {
                break;
            }
            BattleIncSeq(wp, SEQ_ACT);
        case 0x600000B:
            if (BattleStatusChangeAnnouceMain(wp) | BattleStatusChangeMsgMain(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_ACT);
        case 0x600000C:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            if (unit) {
                unit->actEndEventId = 0;
                if (unit->actEndEvent) {
                    evt = evtEntry(unit->actEndEvent, 10, 0);
                    if (evt) {
                        unit->actEndEventId = evt->eventId;
                        evt->unitId = unit->unitId;
                        if (unit->waitEventId) {
                            evtDeleteID(unit->waitEventId);
                            unit->waitEventId = 0;
                        }
                    }
                }
            }
            BattleIncSeq(wp, SEQ_ACT);
        case 0x600000D:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            if (unit && unit->actEndEventId) {
                if (evtCheckID(unit->actEndEventId)) {
                    break;
                }
                unit->actEndEventId = 0;
            }
            BattleIncSeq(wp, SEQ_ACT);
        case 0x600000E:
            if (!BattleWaitAllActiveEvtEnd(wp)) {
                break;
            }
            BattleIncSeq(wp, SEQ_MOVE);
            break;
    }
}

void btlseqTurnFirstProcess(BattleWork* wp) {
    BattleUnitKind* kind;
    int i;
    BattleUnit* unit;

    BtlActRec_JudgeTurnRuleKeep();
    wp->turnCount++;
    if (wp->turnCount >= 2) {
        BattleMajinaiCheck();
    }
    wp->announce.stChg.maskLo = 0;
    wp->announce.stChg.maskHi = 0;
    BtlActRec_AddCount(&wp->actRecord.turnCount);
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            unit->activeTurns++;
            BtlUnit_ResetMoveStatus(unit);
            kind = unit->mKindParams;
            unit->movePriority = kind->turnOrder + irand(kind->turnOrderVariance) - kind->turnOrderVariance / 2;
            if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND)) {
                BtlUnit_OffStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND);
            }
        }
    }
    BattleFogEndCheck();
}

void btlseqPhaseFirstProcess(BattleWork* wp) {
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 count;
    int i;

    wp->commandMenu.recCommand = 0;
    wp->commandMenu.attackAudience = 0;
    for (i = 0; i < 64; i++) {
        wp->movePriorityQueue[i] = -1;
    }
    count = 0;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            unit->mMoveColorLv = 0xFF;
            if (unit->moveState == 10 && !BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
                continue;
            }
            if (unit->attackPhase == BattleGetSeq(wp, SEQ_PHASE) &&
                !(unit->mAttributeFlags & kNoTurn)) {
                wp->movePriorityQueue[count] = unit->unitId;
                unit->protectId = -1;
                for (part = unit->mParts; part; part = part->mNextPart) {
                    if (part->poseId != -1) {
                        part->poseSound.flag = 1;
                    }
                }
                count++;
            }
        }
    }
    battleSortPhaseMoveTable(wp, count);
    battleMakePhaseEvtTable(wp);
}

void battleSortPhaseMoveTable(BattleWork* wp, s32 count) {
    int i, j;
    BattleUnit* unit1;
    BattleUnit* unit2;
    f32 x1, y1, z1;
    f32 x2, y2, z2;
    s32 tmp;
    void* w;

    w = (void*)wp;
    if (count <= 0) {
        count = 0;
        for (i = 0; i < 64; i++) {
            if (((BattleWork*)w)->movePriorityQueue[i] == -1) {
                break;
            }
            count = count + 1;
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            unit1 = BattleGetUnitPtr(((BattleWork*)w), ((BattleWork*)w)->movePriorityQueue[j]);
            unit2 = BattleGetUnitPtr(((BattleWork*)w), ((BattleWork*)w)->movePriorityQueue[j + 1]);
            BtlUnit_GetHomePos(unit1, &x1, &y1, &z1);
            BtlUnit_GetHomePos(unit2, &x2, &y2, &z2);
            if (__fabs(x1) > __fabs(x2)) {
                tmp = ((BattleWork*)w)->movePriorityQueue[j];
                ((BattleWork*)w)->movePriorityQueue[j] = ((BattleWork*)w)->movePriorityQueue[j + 1];
                ((BattleWork*)w)->movePriorityQueue[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            unit1 = BattleGetUnitPtr(((BattleWork*)w), ((BattleWork*)w)->movePriorityQueue[j]);
            unit2 = BattleGetUnitPtr(((BattleWork*)w), ((BattleWork*)w)->movePriorityQueue[j + 1]);
            if (unit1->movePriority < unit2->movePriority) {
                tmp = ((BattleWork*)w)->movePriorityQueue[j];
                ((BattleWork*)w)->movePriorityQueue[j] = ((BattleWork*)w)->movePriorityQueue[j + 1];
                ((BattleWork*)w)->movePriorityQueue[j + 1] = tmp;
            }
        }
    }
}

void battleMakePhaseEvtTable(BattleWork* wp) {
    BattlePhaseEvtEntry* table;
    int i;
    BattleUnit* unit;
    BattlePhaseEvtEntry tmp;
    f32 x, y, z;
    s32 count;
    int j;

    table = wp->phaseEvtTable;
    for (i = 0; i < 64; i++) {
        table[i].unitId = -1;
        table[i].priority = 0;
    }
    count = 0;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && !(unit->mAttributeFlags & kNoTurn)) {
            table[count].unitId = i;
            BtlUnit_GetHomePos(unit, &x, &y, &z);
            if (x >= 0.0f) {
                table[count].priority = 1000.0f + x;
            } else {
                table[count].priority = __fabs(x);
            }
            count++;
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (table[j].priority > table[j + 1].priority) {
                tmp = table[j];
                table[j] = table[j + 1];
                table[j + 1] = tmp;
            }
        }
    }
}

BOOL BattleWaitAllActiveEvtEnd(BattleWork* wp) {
    BattleStageObjectWork* stage = STAGE_WORK(wp);
    BOOL ret = BattleWaitAllActiveEvtEnd_NoBgSetEndWait(wp);

    if (!ret) {
        return ret;
    }
    if (stage->nozzleWorkEvtId) {
        if (evtCheckID(stage->nozzleWorkEvtId)) {
            return FALSE;
        }
        stage->nozzleWorkEvtId = 0;
    }
    if (stage->fallFrameEvtId) {
        if (evtCheckID(stage->fallFrameEvtId)) {
            return FALSE;
        }
        stage->fallFrameEvtId = 0;
    }
    if (stage->bgDestroyEvtId) {
        if (evtCheckID(stage->bgDestroyEvtId)) {
            return FALSE;
        }
        stage->bgDestroyEvtId = 0;
    }
    if (stage->objectFallEvtId) {
        if (evtCheckID(stage->objectFallEvtId)) {
            return FALSE;
        }
        stage->objectFallEvtId = 0;
    }
    if (stage->closeWallEvtId) {
        if (evtCheckID(stage->closeWallEvtId)) {
            return FALSE;
        }
        stage->closeWallEvtId = 0;
    }
    return TRUE;
}

BOOL BattleWaitAllActiveEvtEnd_NoBgSetEndWait(BattleWork* wp) {
    void* w;
    BattleUnit* unit;
    BOOL done;
    int i;
    done = TRUE;
    w = (void*)wp;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr((BattleWork*)w, i);
        if (unit) {
            while (unit->phaseEventId) {
                if (evtCheckID(unit->phaseEventId)) {
                    done = FALSE;
                    break;
                }
                unit->phaseEventId = 0;
                break;
            }
            while (unit->attackEventId) {
                if (evtCheckID(unit->attackEventId)) {
                    done = FALSE;
                    break;
                }
                unit->attackEventId = 0;
                break;
            }
            while (unit->damageEventId) {
                if (evtCheckID(unit->damageEventId)) {
                    done = FALSE;
                    break;
                }
                unit->damageEventId = 0;
                break;
            }
            while (unit->entryEventId) {
                if (evtCheckID(unit->entryEventId)) {
                    done = FALSE;
                    break;
                }
                unit->entryEventId = 0;
                break;
            }
        }
    }
    if (!done) {
        return done;
    }
    if (BattleAfterReactionMain()) {
        done = FALSE;
    }
    return done;
}

BOOL BattleCheckConcluded(BattleWork* wp) {
    BattleUnit* unit;
    s32 count;
    s32 i;
    s32 j;

    count = 0;
    for (i = 0; i < 3; i++) {
        wp->alliances[i].clearConditionMet = 0;
        if (wp->alliances[i].allianceId == 0) {
            wp->alliances[i].clearConditionMet = 1;
            count++;
            continue;
        }
        if (wp->alliances[i].allianceId & 1) {
            for (j = 0; j < 64; j++) {
                unit = BattleGetUnitPtr(wp, j);
                if (unit && unit->mAlliance == i &&
                    !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) &&
                    !(unit->mAttributeFlags & kInactive) &&
                    !(unit->mAttributeFlags & kUnitNoncombatant)) {
                    break;
                }
            }
            if (j >= 64) {
                wp->alliances[i].clearConditionMet = 1;
                count++;
                continue;
            }
        }
        if (wp->alliances[i].allianceId & 2) {
            for (j = 0; j < 64; j++) {
                unit = BattleGetUnitPtr(wp, j);
                if (unit && unit->mAlliance == i &&
                    (unit->mAttributeFlags & kUnitLeader) &&
                    !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) &&
                    !(unit->mAttributeFlags & kUnitNoncombatant)) {
                    break;
                }
            }
            if (j >= 64) {
                wp->alliances[i].clearConditionMet = 1;
                count++;
            }
        }
    }
    if (count >= 2) {
        return TRUE;
    }
    return (wp->flags & (BATTLE_FLAG_ESCAPED | BATTLE_FLAG_INTERRUPT_STOP)) != 0;
}

void _mapobj_data_touch_scale(s32 layer) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    int i;

    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->mLayer == layer) {
            obj->mFlags |= 2;
        }
    }
}

USER_FUNC(_set_haikei_entry_scale) {
    BattleWork* wp = _battleWorkPointer;
    BOOL busy = FALSE;
    f32 sx, sy;
    BattleStageObject* obj;
    int i;

    if (isFirstCall) {
        event->userdata[0] = 60;
        event->userdata[1] = 0;
        if (mapGetMapObj("A1")) {
            mapObjScale("A1", 0.0f, 0.0f, 1.0f);
        }
        if (mapGetMapObj("A2")) {
            mapObjScale("A2", 0.0f, 0.0f, 1.0f);
        }
    }
    if (event->userdata[0] > 0) {
        event->userdata[0]--;
        if (event->userdata[0] > 0) {
            if (mapGetMapObj("A1")) {
                mapObjScale("A1", 0.0f, 0.0f, 1.0f);
                _mapobj_data_touch_scale(0);
            }
            if (mapGetMapObj("A2")) {
                mapObjScale("A2", 0.0f, 0.0f, 1.0f);
                _mapobj_data_touch_scale(1);
            }
            return EVT_RETURN_BLOCK;
        }
    }
    event->userdata[1]++;
    if (mapGetMapObj("A1")) {
        if (event->userdata[1] >= 40) {
            mapObjScale("A1", 1.0f, 1.0f, 1.0f);
            _mapobj_data_touch_scale(0);
        } else {
            busy = TRUE;
            if (event->userdata[1] < 15) {
                mapObjScale("A1", 0.0f, 0.0f, 1.0f);
                _mapobj_data_touch_scale(0);
            } else {
                mapObjScale("A1", 1.0f, 1.0f, 1.0f);
                _mapobj_data_touch_scale(0);
                if (event->userdata[1] < 25) {
                    sy = intpl_sub(EASING_CUBIC_OUT, 0.0f, 110.0f, event->userdata[1] - 15, 10);
                } else if (event->userdata[1] < 35) {
                    sy = intpl_sub(EASING_SINE_IN_OUT, 110.0f, 90.0f, event->userdata[1] - 25, 10);
                } else if (event->userdata[1] < 40) {
                    sy = intpl_sub(EASING_SINE_IN_OUT, 90.0f, 100.0f, event->userdata[1] - 35, 5);
                }
                if (event->userdata[1] < 25) {
                    sx = intpl_sub(EASING_CUBIC_OUT, 100.0f, 90.0f, event->userdata[1] - 15, 10);
                } else if (event->userdata[1] < 35) {
                    sx = intpl_sub(EASING_SINE_IN_OUT, 90.0f, 110.0f, event->userdata[1] - 25, 10);
                } else if (event->userdata[1] < 40) {
                    sx = intpl_sub(EASING_SINE_IN_OUT, 110.0f, 100.0f, event->userdata[1] - 35, 5);
                }
                for (i = 0; i < 32; i++) {
                    obj = &wp->stageObjects[i];
                    if (obj->id > 0 && obj->data->mLayer == 0) {
                        mapObjScale(obj->data->name, sx / 100.0f, sy / 100.0f, 1.0f);
                        obj->mFlags |= 2;
                    }
                }
            }
        }
    }
    if (mapGetMapObj("A2")) {
        if (event->userdata[1] >= 45) {
            mapObjScale("A2", 1.0f, 1.0f, 1.0f);
            _mapobj_data_touch_scale(1);
        } else {
            busy = TRUE;
            if (event->userdata[1] < 20) {
                mapObjScale("A2", 0.0f, 0.0f, 1.0f);
                _mapobj_data_touch_scale(1);
            } else {
                mapObjScale("A2", 1.0f, 1.0f, 1.0f);
                _mapobj_data_touch_scale(1);
                if (event->userdata[1] < 30) {
                    sy = intpl_sub(EASING_CUBIC_OUT, 0.0f, 110.0f, event->userdata[1] - 20, 10);
                } else if (event->userdata[1] < 40) {
                    sy = intpl_sub(EASING_SINE_IN_OUT, 110.0f, 90.0f, event->userdata[1] - 30, 10);
                } else if (event->userdata[1] < 45) {
                    sy = intpl_sub(EASING_SINE_IN_OUT, 90.0f, 100.0f, event->userdata[1] - 40, 5);
                }
                if (event->userdata[1] < 30) {
                    sx = intpl_sub(EASING_CUBIC_OUT, 100.0f, 90.0f, event->userdata[1] - 20, 10);
                } else if (event->userdata[1] < 40) {
                    sx = intpl_sub(EASING_SINE_IN_OUT, 90.0f, 110.0f, event->userdata[1] - 30, 10);
                } else if (event->userdata[1] < 45) {
                    sx = intpl_sub(EASING_SINE_IN_OUT, 110.0f, 100.0f, event->userdata[1] - 40, 5);
                }
                for (i = 0; i < 32; i++) {
                    obj = &wp->stageObjects[i];
                    if (obj->id > 0 && obj->data->mLayer == 1) {
                        mapObjScale(obj->data->name, sx / 100.0f, sy / 100.0f, 1.0f);
                        obj->mFlags |= 2;
                    }
                }
            }
        }
    }
    return busy ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

