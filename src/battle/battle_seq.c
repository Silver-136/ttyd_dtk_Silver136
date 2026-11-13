// Battle sequence: the phase and turn flow, and the action, move and command-selection steps of a battle.

#include "version.h"
#include "battle/battle_sub.h"
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
#include "battle/battle_damage.h"
#include "battle/battle_seq_command.h"
#include "battle/battle_seq_end.h"
#include "battle/unit_work.h"

// external prototypes

// local prototypes
static USER_FUNC(_set_haikei_entry_scale);
static void battleMakePhaseEvtTable(BattleWork* wp);
static void btlseqPhaseFirstProcess(BattleWork* wp);
static void btlseqTurnFirstProcess(BattleWork* wp);
static void btlseqAct(BattleWork* wp);
static BOOL BattlePhaseEndCheck(void);
static void btlseqMove(BattleWork* wp);
static void btlseqPhase(BattleWork* wp);
static void _rule_disp(CameraId cameraId, void* param);
static USER_FUNC(_set_effect_luck);

/**
 * Lucky Start event, started by btlseqFirstAct when any living unit wears the badge:
 * applies the random luck status at once (_set_effect_luck) and holds the battle start for at least 60 frames.
 */
static EVT_BEGIN(nanika_luck_event)
    BEGIN_SIBLING_EVENT_GET_ID(PTR(0xfe363c8f))
        WAIT_FRAMES(60)
    END_SIBLING_EVENT
    CALL(_set_effect_luck)
    CALL(btlevtcmd_WaitEventEnd, PTR(0xfe363c8f))
    RETURN
EVT_END()

/**
 * Started by btlseqFirstAct on a normal battle start (no first strike):
 * pops the stage background layers in with _set_haikei_entry_scale.
 */
#if VERSION >= VERSION_JP
static EVT_BEGIN(_default_haikei_set_entry_event)
    CALL(_set_haikei_entry_scale)
    RETURN
EVT_END()
#endif

static void BattleCheckAllPinchStatus(BattleWork* wp, BOOL noAudience);
static void btlseqInit(BattleWork* wp);
static void btlseqFirstAct(BattleWork* wp);
static void btlseqTurn(BattleWork* wp);
#if VERSION <= VERSION_DEMO_JP
static EvtWord _default_haikei_set_entry_event[4];
#endif

/**
 * Per-frame driver of the battle flow, called from the battle main loop (battle.c):
 * dispatches on the top-level SEQ_UNKNOWN step to setup (0), the opening act (1),
 * the turn loop (2) and the end of battle (3); 4 means finished. Also keeps the low-HP warning beep running.
 */
void BattleSequenceManager(void) {
    BattleWork* wp = _battleWorkPointer;
    switch (BattleGetSeq(wp, SEQ_UNKNOWN)) {
        case BATTLE_STAGE_INIT:
            btlseqInit(wp);
            BattleCheckAllPinchStatus(wp, TRUE);
            break;
        case BATTLE_STAGE_FIRST_ACT:
            BattleCheckAllPinchStatus(wp, FALSE);
            btlseqFirstAct(wp);
            break;
        case BATTLE_STAGE_TURN:
            BattleCheckAllPinchStatus(wp, FALSE);
            btlseqTurn(wp);
            break;
        case BATTLE_STAGE_END:
            btlseqEnd(wp);
            break;
        case BATTLE_STAGE_DONE:
            break;
    }
}

/**
 * Refreshes every unit's Danger/Peril state (noAudience is passed on to BtlUnit_CheckPinchStatus;
 * TRUE on the setup frame), then, while the action menu is open, beeps every 60 frames:
 * SFX_BTL_DANGER1 if Mario or the partner is in Peril, else SFX_BTL_PINCH1 for Danger.
 * Only the first unit with the flag is looked at. With neither, clears Mario's work[MARIO_WORK_PINCH_VOICE] and the timer.
 */
static void BattleCheckAllPinchStatus(BattleWork* wp, BOOL noAudience) {
    BattleUnit* unit;
    BOOL alert;
    int i;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            BtlUnit_CheckPinchStatus(unit, noAudience);
        }
    }

    alert = FALSE;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            if ((unit->statusFlags & UNIT_STATUS_FLAG_PERIL) &&
                !BtlUnit_CheckStatus(unit, STATUS_DEAD) &&
                !BtlUnit_GetBelong(unit)) {
                if (unit->currentKind == UNIT_MARIO ||
                    unit->currentKind >= TYPE_PARTNER_MIN &&
                    unit->currentKind < TYPE_PARTNER_MAX) {
                    if (unit->currentHp > 0) {
                        alert = TRUE;
                    }
                }
                break;
            }
        }
    }

    if (alert) {
        wp->alertTick--;
        if (wp->alertTick <= 0) {
            wp->alertTick = 60;
            if (wp->commandMenu.flags & CMD_MENU_ACT) {
                psndSFXOn("SFX_BTL_DANGER1");
            }
        }
    } else {
        // alert is already FALSE in this branch (the repeated store is in the retail code)
        alert = FALSE;
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            unit = BattleGetUnitPtr(wp, i);
            if (unit) {
                if ((unit->statusFlags & UNIT_STATUS_FLAG_DANGER) &&
                    !BtlUnit_CheckStatus(unit, STATUS_DEAD) &&
                    !BtlUnit_GetBelong(unit)) {
                    if (unit->currentKind == UNIT_MARIO ||
                        unit->currentKind >= TYPE_PARTNER_MIN &&
                        unit->currentKind < TYPE_PARTNER_MAX) {
                        if (unit->currentHp > 0) {
                            alert = TRUE;
                        }
                    }
                    break;
                }
            }
        }
        if (alert) {
            wp->alertTick--;
            if (wp->alertTick <= 0) {
                wp->alertTick = 60;
                if (wp->commandMenu.flags & CMD_MENU_ACT) {
                    psndSFXOn("SFX_BTL_PINCH1");
                }
            }
        } else {
            unit = BattleGetMarioPtr(wp);
            if (unit) {
                unit->work[MARIO_WORK_PINCH_VOICE] = 0;
            }
            wp->alertTick = 0;
        }
    }
}

/**
 * SEQ_INIT steps: hides the field Mario and partner, sets up the battle units,
 * waits for their init events and for the p_b_st,
 * p_jyabara and p_kaiten_h animation effects and the Nice effect to load,
 * then turns the status window on and moves on to SEQ_FIRST_ACT.
 */
static void btlseqInit(BattleWork* wp) {
    MarioWork* mario;
    PartyEntry* party;

    switch (BattleGetSeq(wp, SEQ_INIT)) {
        case SEQ_INIT_START: // falls through
            BattleSetSeq(wp, SEQ_INIT, SEQ_INIT_UNIT_SETUP);

        case SEQ_INIT_UNIT_SETUP:
            mario = marioGetPtr();
            mario->dispFlags |= MARIO_DISP_FLAG_HIDE;
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
            }
            Btl_UnitSetup(wp);
            BattleSetSeq(wp, SEQ_INIT, SEQ_INIT_UNIT_INIT_WAIT);
            break;

        case SEQ_INIT_UNIT_INIT_WAIT:
            if (BattleCheckEndUnitInitEvent(wp)) {
                BattleSetSeq(wp, SEQ_INIT, SEQ_INIT_LOAD_ANIMS);
            }
            break;

        case SEQ_INIT_LOAD_ANIMS:
            if (animEffectAsync("p_b_st", 1) &&
                animEffectAsync("p_jyabara", 1) &&
                animEffectAsync("p_kaiten_h", 1) &&
                effNiceAsync(1)) {
                BattleSetSeq(wp, SEQ_INIT, SEQ_INIT_PAUSE);
            }
            break;

        case SEQ_INIT_PAUSE:
            BattleSetSeq(wp, SEQ_INIT, SEQ_INIT_FINISH);
            break;

        case SEQ_INIT_FINISH:
            BattleStatusWindowEventOn();
            BattleStatusWindowSystemOn();
            BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_FIRST_ACT);
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000000);
            break;
    }
}

/**
 * Weapon whose attack script opens the battle for a field first strike,
 * by the type in FieldBattleInfo.firstAttack (set by BattleInformationSetFirstAttack):
 * 1 and 3 Jump, 2 Spin Jump, 4 Hammer, 5 Super Hammer spin, 6 Ultra Hammer, 7 Koops, 8 Bobbery.
 * 0 (no first strike) gives NULL.
 */
static BattleWeapon* _GetFirstAttackWeapon(s32 type) {
    switch (type) {
        case FIRST_ATTACK_JUMP:
            return &marioDefaultWeapon_Jump;
        case FIRST_ATTACK_SPIN_JUMP:
            return &marioWeapon_KururinJump;
        case FIRST_ATTACK_JUMP_UNUSED:
            return &marioDefaultWeapon_Jump;
        case FIRST_ATTACK_HAMMER:
            return &marioDefaultWeapon_Hammer;
        case FIRST_ATTACK_SPIN_HAMMER:
            return &marioDefaultWeapon_FirstAttackKaitenHammer;
        case FIRST_ATTACK_ULTRA_HAMMER:
            return &marioDefaultWeapon_FirstAttackUltraHammer;
        case FIRST_ATTACK_SHELL:
            return &partyWeapon_NokotarouFirstAttack;
        case FIRST_ATTACK_BOMB:
            return &partyWeapon_SandersFirstAttack;
        case FIRST_ATTACK_NONE:
        default:
            return NULL;
    }
}

/**
 * SEQ_FIRST_ACT steps, the opening of the battle:
 * runs every unit's entry event, or on a first strike the striker's first-attack script with the target's damage
 * event and the audience's reaction; raises the curtain; settles status messages and audience reactions;
 * closes the walls in no-escape battles; applies Lucky Start; runs the stage nozzle check;
 * then starts the turn loop. Ends the battle at once if the first strike decided it.
 */
static void btlseqFirstAct(BattleWork* wp) {
    s32 type;
    NpcBattleInfo* info;
    int i;
    BattleUnit* unit;
    BattleUnit* a;
    BattleUnit* b;
    EventEntry* evt;
    int found;
    s32 flag;
    int k;
    FieldBattleInfo* fb = wp->fbatInfo;

    s32* id;
    type = (s32)fb->firstAttack;

    switch (BattleGetSeq(wp, SEQ_FIRST_ACT)) {
        case 0x2000000:
            switch (type) {
                case 0:
                    wp->ruleDispTimer = 0;
                    evtEntry(_default_haikei_set_entry_event, 10, 0);
                    info = wp->fbatInfo->setup;
                    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                        unit = BattleGetUnitPtr(wp, i);
                        if (unit && unit->entryEventCode) {
                            if ((info->battleFlags & NPC_BATTLE_NO_PARTY_ENTRY) && (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX))) {
                                continue;
                            }
                            if (marioGetPtr()->currMotionId == MARIO_MOTION_YOSHI && unit->currentKind == UNIT_MARIO) {
                                evt = evtEntry(BtlUnit_GetData(unit, UNIT_DATA_RIDE_ENTRY_EVENT), 10, 0);
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
                    break;
                default:
                    switch (type) {
                        case FIRST_ATTACK_JUMP:
                        case FIRST_ATTACK_SPIN_JUMP:
                        case FIRST_ATTACK_JUMP_UNUSED:
                            wp->ruleDispTimer = 10;
                            break;
                        case FIRST_ATTACK_HAMMER:
                        case FIRST_ATTACK_SPIN_HAMMER:
                        case FIRST_ATTACK_ULTRA_HAMMER:
                        case FIRST_ATTACK_SHELL:
                        case FIRST_ATTACK_BOMB:
                            wp->ruleDispTimer = 0;
                            break;
                        default:
                            wp->ruleDispTimer = 0;
                            break;
                    }
                    wp->flags |= BATTLE_FLAG_FIRST_ATTACK;
                    BattleGetFirstAttackUnit(wp, &a, &b);
                    wp->activeUnitId = a->unitId;
                    a->tokenFlags |= kCurrentlyActing;
                    evt = evtEntry(b->damageEventCode, 10, EVENT_START_IMMEDIATE);
                    if (evt) {
                        evt->unitId = b->unitId;
                        b->damageCode = DAMAGE_CODE_FIRST_ATTACK_POS;
                        b->damageEventId = evt->eventId;
                    }
                    a->flags |= BATTLE_UNIT_FLAG_ACTING;
                    if (type < FIRST_ATTACK_ENEMY) {
                        a->attackEventCode = _GetFirstAttackWeapon(type)->attackEvtScript;
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
                        case FIRST_ATTACK_JUMP:
                        case FIRST_ATTACK_SPIN_JUMP:
                        case FIRST_ATTACK_JUMP_UNUSED:
                        case FIRST_ATTACK_HAMMER:
                        case FIRST_ATTACK_SPIN_HAMMER:
                        case FIRST_ATTACK_ULTRA_HAMMER:
                            unit = BattleGetPartyPtr(wp);
                            if (unit) {
                                evt = evtEntry(unit->entryEventCode, 10, 0);
                                unit->entryEventId = evt->eventId;
                                evt->unitId = unit->unitId;
                            }
                            break;
                        case FIRST_ATTACK_SHELL:
                        case FIRST_ATTACK_BOMB:
                            unit = BattleGetMarioPtr(wp);
                            if (unit) {
                                evt = evtEntry(unit->entryEventCode, 10, 0);
                                unit->entryEventId = evt->eventId;
                                evt->unitId = unit->unitId;
                            }
                    }
                    BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000001);
                    break;
            }
            break;

        case 0x2000001:
            if (--wp->ruleDispTimer > 0) {
                break;
            }
            {
                // black: the curtain fade that opens every battle
                GXColor c = {0, 0, 0, 0xFF};
                fadeEntry(FADE_CURTAIN_VERTICAL_A2, 200, c);
            }
            wp->curtainSfxId = psndSFXOn("SFX_OFF_BTL_START_CURTAIN_UP1");
            BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000002);

        case 0x2000002:
            if (fadeIsFinish() && wp->curtainSfxId != -1) {
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
                BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_END);
                BattleSetSeq(wp, SEQ_END, SEQ_END_START);
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
            if (!BattleCheckConcluded(wp) && BattleBreakSlot_CheckReaction()) {
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
            if (wp->fbatInfo->setup->battleFlags & NPC_BATTLE_NO_ESCAPE) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x2000009);
                BattleStage_WallCloseCheck();
            } else {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000A);
            }
            break;

        case 0x2000009:
            id = &wp->stageObjectWork.closeWallEvtId;
            if (*id != 0 && !evtCheckID(*id)) {
                *id = 0;
            }
            if (*id == 0) {
                BattleSetSeq(wp, SEQ_FIRST_ACT, 0x200000A);
            }
            break;

        case 0x200000A:
            wp->luckEventId = 0;
            flag = 0;
            k = 0;
            do {
                unit = BattleGetUnitPtr(wp, k);
                if (unit && unit->badgesEquipped.LuckyStart && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
                    flag = 1;
                }
            } while (++k < BATTLE_UNIT_MAX);
            if (flag) {
                evt = evtEntry(nanika_luck_event, 10, 0);
                evt->unitId = UNIT_ID_SYSTEM;
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
            BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_TURN);
            BattleSetSeq(wp, SEQ_TURN, 0x3000000);
            break;
    }
}

/**
 * Lucky Start: for every living unit wearing the badge,
 * applies one random status from its weapon (Electrified, Dodgy, HP Regen or FP Regen, 3 turns) with
 * SFX_SYSTEM_LUCKY1 and a Nice sparkle by the unit. No script arguments; run by nanika_luck_event.
 */
static USER_FUNC(_set_effect_luck) {
    BattleWork* wp = _battleWorkPointer;

    // the four statuses Lucky Start chooses from; one is kept at random per unit
    BattleWeapon weapon = {
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
        TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
        NULL, // attackEvtScript
        {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
    };
    BattleUnit* unit;
    BattleUnitPart* part;
    int i;
    BattleWeapon* pw = &weapon;
    Vec pos;
    EffectEntry* e;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->badgesEquipped.LuckyStart && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
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
            BattleSetStatusDamageFromWeapon(BattleGetSystemPtr(wp), unit, part, pw, DAMAGE_RESULT_FINAL_HIT);
            BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
            psndSFXOn(SFX_NAME_WORD("SFX_SYSTEM_LUCKY1"));

            e = effNiceEntry(NICE_EFFECT_LUCKY, pos.x - 45.0f, 60.0f + pos.y, pos.z);
            ((NiceEffectWork*)e->userdata)->overallScale = 0.75f;
        }
    }
    return EVT_RETURN_DONE;
}

static const char* _rule_msg_table_2bu[68];
static const char* _rule_msg_table_1bu[68];
static const char* _rule_msg_table_after[68];

/**
 * SEQ_TURN steps, one full turn: start-of-turn bookkeeping, the Glitz Pit bonus-condition banner on turn 1,
 * poison and burn damage, HP and FP Regen, Happy Heart and Happy Flower (from turn 2),
 * the per-status recovery checks with SFX_REST_CONDITION1, the command-disable countdowns,
 * then the phase loop (btlseqPhase) and the audience's turn-end check before starting the next turn.
 * A step with nothing to do skips its wait step.
 * In US and later two of the countdowns are per unit (usCommandDisabled).
 */
static void btlseqTurn(BattleWork* wp) {
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
    BattleUnit* unit;
    EventEntry* evt;
    BOOL found;
    BOOL sfx;
    const char* sfxName;
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
            dispEntry(CAMERA_2D, 900.0f, DISP_MODE_ALPHA_TEST, _rule_disp, NULL);
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
#if VERSION <= VERSION_DEMO_JP
            BattleIncSeq(wp, SEQ_TURN);
#else
            if (BattleCheckConcluded(wp)) {
                BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_END);
                BattleSetSeq(wp, SEQ_END, SEQ_END_START);
            } else {
                BattleIncSeq(wp, SEQ_TURN);
            }
#endif
            break;

        case 0x3000012:
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (BtlUnit_CheckRecoveryStatus(unit, STATUS_FREEZE)) {
                        if (!BtlUnit_CheckShadowGuard(unit)) {
                            BattleDamageDirect(UNIT_ID_SYSTEM, unit, NULL, 1, 0, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_ICE, 0, 1);
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit) {
                    ret = BtlUnit_CheckRecoveryStatus(unit, STATUS_SLOW);
                    sfx |= ret;
                    if (ret) {
#if VERSION >= VERSION_JP
                        BtlUnit_ResetMoveStatus(unit);
#endif
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_POISON, &turns, &strength);
                    if ((strength > 0) && (!BtlUnit_CheckShadowGuard(unit))) {
                        found = 1;
                        BattleDamageDirect(UNIT_ID_SYSTEM, unit, NULL, strength, 0, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_NORMAL, 0, 1);
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
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
                    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                        unit = BattleGetUnitPtr(wp, i);
                        if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                            BtlUnit_GetStatus(unit, STATUS_BURN, &turns, &strength);
                            if (strength > 0) {
                                BSE_TurnFirstProcessEffectEntry(unit, 1);
                            }
                        }
                    }

                case 1:
                    done = 1;
                    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
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
#if VERSION <= VERSION_DEMO_JP
                    BattleIncSeq(wp, SEQ_TURN);
#else
                    if (BattleCheckConcluded(wp)) {
                        BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_END);
                        BattleSetSeq(wp, SEQ_END, SEQ_END_START);
                    } else {
                        BattleIncSeq(wp, SEQ_TURN);
                    }
#endif
                    break;
            }
            break;

        case 0x300000A:
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_HP_REGEN, &turns, &strength);
                    if (strength > 0) {
                        if (!(unit->attributeFlags & kUnitHpFixed)) {
                            unit->hpGauge.prevHp = unit->currentHp;
                            BtlUnit_RecoverHp(unit, strength);
                            unit->hpGauge.targetHp = unit->currentHp;
                            unit->hpGauge.drainTimer = unit->hpGauge.drainLength;
                            unit->hpGauge.visibleTimer = unit->hpGauge.visibleLength;
                            unit->hpGauge.flags |= HP_GAUGE_FLAG_TIMED;
                        } else {
                            BtlUnit_RecoverHp(unit, strength);
                        }
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &hpX, &hpY, &hpZ);
                        hpY += unit->sizeMultiplier * (BtlUnit_GetHeight(unit) + 5);
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                    BtlUnit_GetStatus(unit, STATUS_FP_REGEN, &turns, &strength);
                    if (strength > 0) {
                        BtlUnit_RecoverFp(unit, strength);
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &fpX, &fpY, &fpZ);
                        fpY += unit->sizeMultiplier * (BtlUnit_GetHeight(unit) + 5);
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                    n = 0;
                    for (count = unit->badgesEquipped.HappyHeart; count > 0; count--) {
                        // integer division: currentHp / maxHp is 0 below full HP, so the chance is 33 %, at full HP 30 %
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
#if VERSION <= VERSION_DEMO_JP
                        heartY += BtlUnit_GetHeight(unit);
#else
                        heartY += unit->sizeMultiplier * BtlUnit_GetHeight(unit);
#endif
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
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if ((unit && (!BtlUnit_CheckStatus(unit, STATUS_DEAD))) && (unit->currentHp > 0)) {
                    n = 0;

                    // rate and count swap roles here compared with the Happy Heart case above (retail local reuse): rate is the badge counter, count the chance
                    for (rate = unit->badgesEquipped.HappyFlower; rate > 0; rate--) {
                        // integer division: currentFp / maxFp is 0 below full FP, so the chance is 33 %, at full FP 30 %
                        count = 33 - ((unit->currentFp / unit->maxFp) * 3);
                        if (irand(100) < count) {
                            n++;
                        }
                    }

                    if (n > 0) {
                        BtlUnit_RecoverFp(unit, n);
                        BtlUnit_GetPartsWorldPos(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), &flowerX, &flowerY, &flowerZ);
#if VERSION <= VERSION_DEMO_JP
                        flowerY += BtlUnit_GetHeight(unit);
#else
                        flowerY += unit->sizeMultiplier * BtlUnit_GetHeight(unit);
#endif
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
#if VERSION >= VERSION_US
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit && !BtlUnit_CheckStatus(unit, STATUS_DEAD) && unit->currentHp > 0 && unit->usCommandDisabled[0]) {
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
#if VERSION >= VERSION_US
            for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                unit = BattleGetUnitPtr(wp, i);
                if (unit && !BtlUnit_CheckStatus(unit, STATUS_DEAD) && unit->currentHp > 0 && unit->usCommandDisabled[1]) {
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
            BattleSetSeq(wp, SEQ_PHASE, BATTLE_PHASE_INIT);
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

static void _rule_disp(CameraId cameraId, void* param) {
    FieldBattleInfo* info = _battleWorkPointer->fbatInfo;
    char buf[256];
    f32 x;
    f32 y;
    u16 lines;
    u16 width;

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG3_34_RETURN_TO_ROGUEPORT) {
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
    {
        // white: the rule banner's window
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        windowDispGX_Waku_col((x - width / 2) - 10.0f, 94.0f, width + 20, lines * 29 + 3, 10.0f, 0, color);
    }
    FontDrawStart();
    FontDrawMessage(x - width / 2, (s32)y - 29, buf);
}

/**
 * Glitz Pit bonus-condition messages for minor-league fights, indexed by the battle's rule type ("a" = unused);
 * _rule_disp shows the one for the fight at the start of turn 1, e.g. "Don't use your jump!".
 */
static const char* _rule_msg_table_2bu[] = {
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

/** Glitz Pit bonus-condition messages for major-league fights (top ten), by rule type; shown by _rule_disp. */
static const char* _rule_msg_table_1bu[] = {
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

/** Glitz Pit bonus-condition messages after chapter 3, by rule type; shown by _rule_disp. */
static const char* _rule_msg_table_after[] = {
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

#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(_default_haikei_set_entry_event)
    CALL(_set_haikei_entry_scale)
    RETURN
EVT_END()
#endif

/**
 * SEQ_PHASE steps: runs btlseqMove through each of the turn's five phases (before player, player, after player,
 * enemy, after enemy), then hands back to the next SEQ_TURN step.
 */
static void btlseqPhase(BattleWork* wp) {
    switch (BattleGetSeq(wp, SEQ_PHASE)) {
        case BATTLE_PHASE_INIT:
            BattleIncSeq(wp, SEQ_PHASE);
            BattleSetSeq(wp, SEQ_MOVE, 0x5000000);
            break;
        case BATTLE_PHASE_BEFORE_PLAYER:
        case BATTLE_PHASE_PLAYER:
        case BATTLE_PHASE_AFTER_PLAYER:
        case BATTLE_PHASE_ENEMY:
        case BATTLE_PHASE_AFTER_ENEMY:
            btlseqMove(wp);
            break;
        case BATTLE_PHASE_END:
            BattleIncSeq(wp, SEQ_TURN);
            BattleSetSeq(wp, SEQ_PHASE, BATTLE_PHASE_INIT);
            break;
    }
}

static void btlseqMove(BattleWork* wp) {
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
                if (pouch->merleeNextCurseType != MAJINAI_DEFENCE_UP || BattleGetSeq(wp, SEQ_PHASE) != BATTLE_PHASE_ENEMY) {
                    BattleIncSeq(wp, SEQ_MOVE);
                    BattleIncSeq(wp, SEQ_MOVE);
                    loop = TRUE;
                    break;
                }
                wp->majinaiState = MAJINAI_DEFENCE_UP;
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
                for (j = 0; j < BATTLE_UNIT_MAX; j++) {
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
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    if (wp->phaseEvtTable[i].unitId != -1) {
                        unit = BattleGetUnitPtr(wp, wp->phaseEvtTable[i].unitId);
                        if (unit && unit->phaseEventCode) {
                            break;
                        }
                    }
                }
                if (i < BATTLE_UNIT_MAX) {
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
                    BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_END);
                    BattleSetSeq(wp, SEQ_END, SEQ_END_START);
                } else {
                    BattleSetSeq(wp, SEQ_MOVE, 0x5000006);
                    loop = TRUE;
                }
                break;
            case 0x5000008:
                memset(&wp->impendingBonuses, 0, sizeof(wp->impendingBonuses));
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (unit) {
#if VERSION <= VERSION_DEMO_JP
                        unit->tokenFlags &= kTokenPersistent_UNUSED;
#else
                        unit->tokenFlags &= kTokenPersistent_UNUSED | kHasRallyWinkStatus;
#endif
                        unit->totalHpDamageTaken = 0;
                        unit->hpDamageTaken = 0;
                        unit->hpDamageCount = 0;
                        unit->attackerHitCountTaken = 0;
                        unit->totalFpDamageTaken = 0;
                        unit->fpDamageTaken = 0;
                        unit->totalFpLost = 0;
                        unit->fpLost = 0;
                        unit->attackHitCount = 0;
                    }
                }
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    id = wp->movePriorityQueue[i];
                    if (id != -1) {
                        unit = BattleGetUnitPtr(wp, id);
#if VERSION <= VERSION_DEMO_JP
                        if (unit) {
#else
                        if (unit && unit->movesRemaining > 0) {
#endif
                            state = unit->moveState;
                            if (state != MOVE_STATE_DONE && (!BtlUnit_CheckStatus(unit, STATUS_DEAD) || (unit->attributeFlags & kUndead))) {
                                if (state != MOVE_STATE_POSITION_CHANGE) {
                                    break;
                                }
                                unit->moveState = MOVE_STATE_NORMAL;
                            }
                        }
                    }
                }
                wp->activeUnitId = id;
                BattleGetUnitPtr(wp, id)->tokenFlags |= kCurrentlyActing;
                BattleIncSeq(wp, SEQ_MOVE);
                BattleSetSeq(wp, SEQ_ACT, 0x6000000);
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    id = wp->movePriorityQueue[i];
                    if (id != -1) {
                        unit = BattleGetUnitPtr(wp, id);
                        if (unit && unit->moveState == MOVE_STATE_POSITION_CHANGE) {
                            unit->moveState = MOVE_STATE_NORMAL;
                        }
                    }
                }
                unit = BattleGetUnitPtr(wp, wp->activeUnitId);
                if (unit->currentKind == UNIT_MARIO) {
                    pouch = pouchGetPtr();
                    wp->flags |= (BATTLE_FLAG_JUMP_NO_SUBMENU | BATTLE_FLAG_HAMMER_NO_SUBMENU);
                    for (i = 0; i < MAX_BADGES; i++) {
                        if (pouch->equippedBadges[i] >= ITEM_POWER_JUMP && pouch->equippedBadges[i] < ITEM_POWER_SMASH) {
                            wp->flags &= ~BATTLE_FLAG_JUMP_NO_SUBMENU;
                            break;
                        }
                    }
                    if (pouch->jumpLevel >= 2) {
                        wp->flags &= ~BATTLE_FLAG_JUMP_NO_SUBMENU;
                    }
                    for (i = 0; i < MAX_BADGES; i++) {
                        if (pouch->equippedBadges[i] >= ITEM_POWER_SMASH && pouch->equippedBadges[i] < ITEM_DOUBLE_DIP) {
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
                    BattleSetSeq(wp, SEQ_UNKNOWN, BATTLE_STAGE_END);
                    BattleSetSeq(wp, SEQ_END, SEQ_END_START);
                    break;
                }
                for (i = 0; i < BATTLE_UNIT_MAX; i++) {
                    unit = BattleGetUnitPtr(wp, i);
                    if (unit) {
                        BtlUnit_SetTotalHitDamage(unit, 0);
                        unit->tokenFlags &= ~kCurrentlyActing;
                    }
                }
                end = BattlePhaseEndCheck();
                if (wp->flags & BATTLE_FLAG_RESET_TURN) {
                    end = TRUE;
                }
                if (end) {
                    if (wp->majinaiState == MAJINAI_DEFENCE_UP) {
                        wp->majinaiState = MAJINAI_NONE;
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

/**
 * TRUE when no unit in this phase's move queue can still act (done for the turn or KO'd).
 * Outside the early demos a unit also needs movesRemaining above 0.
 */
static BOOL BattlePhaseEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    int i;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        if (wp->movePriorityQueue[i] != -1) {
            unit = BattleGetUnitPtr(wp, wp->movePriorityQueue[i]);
#if VERSION <= VERSION_DEMO_JP
            if (unit && unit->moveState != MOVE_STATE_DONE && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
#else
            if (unit && unit->moveState != MOVE_STATE_DONE && unit->movesRemaining > 0 &&
                !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
#endif
                break;
            }
        }
    }
    if (i >= 64) {
        return TRUE;
    }
    return FALSE;
}

/**
 * SEQ_ACT steps, one unit's action: Mario or the partner gets the command menu (BattleCommandInput)
 * unless asleep, stopped, frozen or KO'd (turn skipped), veiled by Vivian or flipped (their own data scripts).
 * Then runs the chosen attack script, or the zero-gravity can't-move script,
 * or the confused action half the time when Confused;
 * afterwards updates moves left, Charge, Double/Triple Dip progress,
 * the audience reaction and the unit's act-end event, and returns to btlseqMove.
 */
static void btlseqAct(BattleWork* wp) {
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
                            !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
                            cantMove = FALSE;
                        }
                        break;
                    default:
                        if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
                            if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) &&
                                !BtlUnit_CheckStatus(unit, STATUS_STOP) &&
                                !BtlUnit_CheckStatus(unit, STATUS_FREEZE) &&
                                !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
                                cantMove = FALSE;
                            }
                        }
                        break;
                }
                if (cantMove) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000003);
                } else if (unit->attributeFlags & kVeiled) {
                    BattleSetSeq(wp, SEQ_ACT, 0x6000004);
                } else if (unit->currentKind == UNIT_VIVIAN && (BattleGetMarioPtr(wp)->attributeFlags & kVeiled)) {
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
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000003:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
            unit->flags |= BATTLE_UNIT_FLAG_ACTING;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000004:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            unit->attackEventCode = BtlUnit_GetData(unit, UNIT_DATA_HIDE_EVENT);
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS | ACT_EVENT_IN_ZERO_GRAVITY | ACT_EVENT_ALWAYS;
            BattleSetSeq(wp, SEQ_ACT, 0x6000006);
            break;
        case 0x6000006:
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            if (!(unit->attackEventFlags & ACT_EVENT_KEEP_STATUS_WINDOW)) {
                BattleStatusWindowSystemOff();
            }
            unit = BattleGetUnitPtr(wp, wp->activeUnitId);
            evt = NULL;
            if ((wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED) && !(unit->attributeFlags & kActsInZeroGravity) &&
                !(unit->attackEventFlags & ACT_EVENT_IN_ZERO_GRAVITY) && !(unit->attackEventFlags & ACT_EVENT_ALWAYS)) {
                evt = evtEntry(btldefaultevt_CantMoveZeroGravity, 10, 0);
            } else if (unit->attackEventFlags & ACT_EVENT_ALWAYS) {
                if (unit->attackEventCode) {
                    evt = evtEntry(unit->attackEventCode, 10, 0);
                }
            } else if (unit->attackEventFlags & ACT_EVENT_IGNORE_STATUS) {
                evt = evtEntry(unit->attackEventCode, 10, 0);
            } else if (!BtlUnit_CheckStatus(unit, STATUS_SLEEP) &&
                !BtlUnit_CheckStatus(unit, STATUS_STOP) &&
                !BtlUnit_CheckStatus(unit, STATUS_FREEZE) &&
                (!BtlUnit_CheckStatus(unit, STATUS_DEAD) || (unit->attributeFlags & kUndead))) {
                // BUG: irand(100) < 0 never holds, so an electrified unit never runs btldefaultevt_BiribiriMove.
                // With a real chance, the shock would replace the unit's action for the turn, like the status tests
                // above. Feeling Fine blocks Electric along with the harmful statuses (BattleSetStatusDamage).
                if (BtlUnit_CheckStatus(unit, STATUS_ELECTRIC) && irand(100) < 0) {
                    evt = evtEntry(btldefaultevt_BiribiriMove, 10, 0);
                } else if (BtlUnit_CheckStatus(unit, STATUS_CONFUSE) && irand(100) < 50) {
                    unit->tokenFlags |= kConfusedAction;
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
                if (unit->tokenFlags & kChargeExpended) {
                    BtlUnit_SetStatus(unit, STATUS_CHARGE, 1, -99);
                    unit->tokenFlags &= ~kChargeExpended;
                }
                if (wp->majinaiState == MAJINAI_POWER_UP) {
                    wp->majinaiState = MAJINAI_NONE;
                }
                unit->reserved_258 = 0;
                switch (unit->moveState) {
                    case MOVE_STATE_TRIPLE_DIP_1:
                        unit->moveState = MOVE_STATE_TRIPLE_DIP_2;
                        break;
                    case MOVE_STATE_TRIPLE_DIP_2:
                        unit->moveState = MOVE_STATE_TRIPLE_DIP_3;
                        break;
                    case MOVE_STATE_DOUBLE_DIP_1:
                        unit->moveState = MOVE_STATE_DOUBLE_DIP_2;
                        break;
                    case MOVE_STATE_TRIPLE_DIP_3:
                    case MOVE_STATE_DOUBLE_DIP_2:
                        unit->moveState = MOVE_STATE_NORMAL;
                        break;
                }
                if (unit->moveState == MOVE_STATE_NORMAL) {
                    // decrement is always TRUE; the flag, its second store and its test are all in the retail code
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
                            unit->moveState = MOVE_STATE_DONE;
#if VERSION >= VERSION_JP
                            if (!(unit->tokenFlags & kHasRallyWinkStatus)) {
                                unit->activeTurns = 0;
                            } else {
                                unit->tokenFlags &= ~kHasRallyWinkStatus;
                            }
#else
                            unit->activeTurns = 0;
#endif
                            if (!BtlUnit_GetBelong(unit) && unit->attackPhase == BATTLE_PHASE_PLAYER &&
                                !BattlePhaseEndCheck() && !(wp->flags & BATTLE_FLAG_RESET_TURN)) {
                                unit->moveColorLv = MOVE_COLOR_LV_DONE;
                            }
                        }
                    }
                } else if (unit->moveState == MOVE_STATE_QUICK_CHANGE) {
                    unit->moveState = MOVE_STATE_NORMAL;
                } else if (unit->moveState == MOVE_STATE_AUDIENCE_ATTACK) {
                    unit->moveState = MOVE_STATE_NORMAL;
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
            if (!BattleCheckConcluded(wp) && BattleBreakSlot_CheckReaction()) {
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

/**
 * Start-of-turn bookkeeping: judges the battle-rule record for the last turn,
 * bumps turnCount (Merlee's charm check from turn 2), clears the status-change announce masks,
 * rolls each unit's move priority from its kind's turnOrder and variance, drops Defending,
 * and checks whether the fog ends.
 */
static void btlseqTurnFirstProcess(BattleWork* wp) {
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
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            unit->activeTurns++;
            BtlUnit_ResetMoveStatus(unit);
            kind = unit->kindParams;
            unit->movePriority = kind->turnOrder + irand(kind->turnOrderVariance) - kind->turnOrderVariance / 2;
            if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND)) {
                BtlUnit_OffStatusFlag(unit, UNIT_STATUS_FLAG_DEFEND);
            }
        }
    }
    BattleFogEndCheck();
}

/**
 * Start-of-phase bookkeeping: queues the units whose attackPhase is this phase into movePriorityQueue (skipping
 * units done for the turn; outside the early demos Slowed units are still queued),
 * resets their protect target and pose sounds, then sorts the queue and builds the phase event order.
 */
static void btlseqPhaseFirstProcess(BattleWork* wp) {
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 count;
    int i;

    wp->commandMenu.recCommand = 0;
    wp->commandMenu.attackAudience = 0;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        wp->movePriorityQueue[i] = -1;
    }
    count = 0;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            unit->moveColorLv = MOVE_COLOR_LV_FULL;
#if VERSION <= VERSION_DEMO_JP
            if (unit->moveState == MOVE_STATE_DONE) {
#else
            if (unit->moveState == MOVE_STATE_DONE && !BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
#endif
                continue;
            }
            if (unit->attackPhase == BattleGetSeq(wp, SEQ_PHASE) &&
                !(unit->attributeFlags & kNoTurn)) {
                wp->movePriorityQueue[count] = unit->unitId;
                unit->protectId = -1;
                for (part = unit->parts; part; part = part->nextPart) {
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

/**
 * Sorts movePriorityQueue: by distance of the home position from the stage centre, then by movePriority,
 * highest first (both bubble sorts, so ties keep the first order).
 * count <= 0 counts the queue itself; btlevtcmd_SortPhaseMoveTable re-sorts this way mid-phase.
 */
void battleSortPhaseMoveTable(BattleWork* wp, s32 count) {
    int i, j;
    BattleUnit* unit1;
    BattleUnit* unit2;
    f32 x1, y1, z1;
    f32 x2, y2, z2;
    s32 tmp;

    BattleWork* const w = wp;

    if (count <= 0) {
        count = 0;
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            if (w->movePriorityQueue[i] == -1) {
                break;
            }
            count++;
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            unit1 = BattleGetUnitPtr(w, w->movePriorityQueue[j]);
            unit2 = BattleGetUnitPtr(w, w->movePriorityQueue[j + 1]);
            BtlUnit_GetHomePos(unit1, &x1, &y1, &z1);
            BtlUnit_GetHomePos(unit2, &x2, &y2, &z2);
            if (__fabs(x1) > __fabs(x2)) {
                tmp = w->movePriorityQueue[j];
                w->movePriorityQueue[j] = w->movePriorityQueue[j + 1];
                w->movePriorityQueue[j + 1] = tmp;
            }
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            unit1 = BattleGetUnitPtr(w, w->movePriorityQueue[j]);
            unit2 = BattleGetUnitPtr(w, w->movePriorityQueue[j + 1]);
            if (unit1->movePriority < unit2->movePriority) {
                tmp = w->movePriorityQueue[j];
                w->movePriorityQueue[j] = w->movePriorityQueue[j + 1];
                w->movePriorityQueue[j + 1] = tmp;
            }
        }
    }
}

/**
 * Fills phaseEvtTable with every unit that takes turns, in the order their phase events run:
 * units left of centre first, nearest the centre first, then units right of centre, nearest first.
 */
static void battleMakePhaseEvtTable(BattleWork* wp) {
    BattlePhaseEvtEntry* table;
    int i;
    BattleUnit* unit;
    BattlePhaseEvtEntry tmp;
    f32 x, y, z;
    s32 count;
    int j;

    table = wp->phaseEvtTable;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        table[i].unitId = -1;
        table[i].priority = 0;
    }
    count = 0;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && !(unit->attributeFlags & kNoTurn)) {
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

/**
 * TRUE once every unit's events and the after-reaction are done (BattleWaitAllActiveEvtEnd_NoBgSetEndWait)
 * and the stage's nozzle, falling-frame, background-collapse, falling-object and wall-closing events have ended;
 * clears the ids of finished ones. The sequence steps poll it before moving on.
 */
BOOL BattleWaitAllActiveEvtEnd(BattleWork* wp) {
    BattleStageObjectWork* stage = &wp->stageObjectWork;
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

/**
 * TRUE once no unit has a running phase, attack,
 * damage or entry event (the PAL demo and PAL also wait for the slide-return event)
 * and BattleAfterReactionMain is idle; clears the ids of finished events.
 * Unlike BattleWaitAllActiveEvtEnd it ignores the stage-object events,
 * so battle_stage_object.c can use it while those run.
 */
BOOL BattleWaitAllActiveEvtEnd_NoBgSetEndWait(BattleWork* wp) {
    BattleWork* const w = wp;
    BattleUnit* unit;
    BOOL done;
    int i;
    done = TRUE;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(w, i);
        if (unit) {
            // FAKE: while loops that always break on the first pass, used as ifs; plain if/else changes the object
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
#if VERSION >= VERSION_DEMO_EU
            while (unit->slideReturnEventId) {
                if (evtCheckID(unit->slideReturnEventId)) {
                    done = FALSE;
                    break;
                }
                unit->slideReturnEventId = 0;
                break;
            }
#endif
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

/**
 * TRUE when the battle is over: at least two of the three alliances meet their clear condition (ALLIANCE_CLEAR_ALL_OUT, 1: no active
 * member left standing; ALLIANCE_CLEAR_LEADER_OUT, 2: no leader left;
 * an unused alliance counts as met), or the party ran away or the battle was interrupted.
 * Records clearConditionMet per alliance.
 */
BOOL BattleCheckConcluded(BattleWork* wp) {
    BattleUnit* unit;
    s32 count;
    s32 i;
    s32 j;

    count = 0;
    for (i = 0; i < 3; i++) {
        wp->alliances[i].clearConditionMet = 0;
        if (wp->alliances[i].clearFlags == ALLIANCE_CLEAR_NONE) {
            wp->alliances[i].clearConditionMet = 1;
            count++;
            continue;
        }
        if (wp->alliances[i].clearFlags & ALLIANCE_CLEAR_ALL_OUT) {
            for (j = 0; j < BATTLE_UNIT_MAX; j++) {
                unit = BattleGetUnitPtr(wp, j);
                if (unit && unit->alliance == i &&
                    !BtlUnit_CheckStatus(unit, STATUS_DEAD) &&
                    !(unit->attributeFlags & kInactive) &&
                    !(unit->attributeFlags & kUnitNoncombatant)) {
                    break;
                }
            }
            if (j >= 64) {
                wp->alliances[i].clearConditionMet = 1;
                count++;
                continue;
            }
        }
        if (wp->alliances[i].clearFlags & ALLIANCE_CLEAR_LEADER_OUT) {
            for (j = 0; j < BATTLE_UNIT_MAX; j++) {
                unit = BattleGetUnitPtr(wp, j);
                if (unit && unit->alliance == i &&
                    (unit->attributeFlags & kUnitLeader) &&
                    !BtlUnit_CheckStatus(unit, STATUS_DEAD) &&
                    !(unit->attributeFlags & kUnitNoncombatant)) {
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

/**
 * Sets flag 2 on every stage object on background layer `layer` (0 = map object A1, 1 = A2)
 * after its scale was changed. BattleStageObjectMain treats flag 2 as "changed" (and clears it),
 * so it rewrites the object's map translation and rotation on its next update.
 */
static void _mapobj_data_touch_scale(s32 layer) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    int i;

    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->layer == layer) {
            obj->flags |= STAGE_OBJECT_FLAG_UPDATE;
        }
    }
}

/**
 * Background pop-in at the start of a normal battle (run by _default_haikei_set_entry_event):
 * keeps the background layers A1 and A2 flattened for 60 frames,
 * then pops each in with a squash-and-stretch (A1 over frames 15-40, A2 over 20-45) together with the stage
 * objects on that layer. No script arguments; blocks until both are done.
 */
static USER_FUNC(_set_haikei_entry_scale) {
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
                for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                    obj = &wp->stageObjects[i];
                    if (obj->id > 0 && obj->data->layer == 0) {
                        mapObjScale(obj->data->name, sx / 100.0f, sy / 100.0f, 1.0f);
                        obj->flags |= STAGE_OBJECT_FLAG_UPDATE;
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
                for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                    obj = &wp->stageObjects[i];
                    if (obj->id > 0 && obj->data->layer == STAGE_LAYER_A2) {
                        mapObjScale(obj->data->name, sx / 100.0f, sy / 100.0f, 1.0f);
                        obj->flags |= STAGE_OBJECT_FLAG_UPDATE;
                    }
                }
            }
        }
    }
    return busy ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}
