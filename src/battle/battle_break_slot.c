#include "battle/battle_break_slot.h"
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_disp.h"
#include "eff/eff_break.h"
#include "pmario_sound.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include <stdlib.h>
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include <dolphin/pad.h>
#include "battle/battle_camera.h"
#include "eff/eff_magic1_n64.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "evt/evt_eff.h"

typedef struct { s32 v[8]; } SlotCards8;
typedef struct { s32 v[5]; } SlotCards5;


BreakSlotReel* BattleBreakSlotReelGetPtr(s32 no);
void BattleBreakSlot_Start(void);
void BattleBreakSlotDispReel(CameraId camId, void* param);
void item_appear_force(BreakSlotReel* reel, s32 item);



EVT_BEGIN(kinoko_effect)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetMaxHp, LW(3), LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_eff, STRING(""), STRING("recovery"), 4, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), LW(4))
    CALL(btlevtcmd_GetUnitId, -4, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_GetMaxHp, LW(3), LW(5))
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            ADD(LW(1), 50)
            CALL(evt_eff, STRING(""), STRING("recovery"), 4, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
            CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), LW(4))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(flower_effect)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetMaxFp, LW(3), LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_eff, STRING(""), STRING("recovery"), 5, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_RecoverFp, LW(3), LW(4), LW(5))
    RETURN
EVT_END()

EVT_BEGIN(shine_effect)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetMaxHp, LW(3), LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_eff, STRING(""), STRING("recovery"), 4, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), LW(4))
    CALL(btlevtcmd_GetUnitId, -4, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_GetMaxHp, LW(3), LW(5))
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            ADD(LW(1), 50)
            CALL(evt_eff, STRING(""), STRING("recovery"), 4, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
            CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), LW(4))
        END_IF
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetMaxFp, LW(3), LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_eff, STRING(""), STRING("recovery"), 5, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_RecoverFp, LW(3), LW(4), LW(5))
    RETURN
EVT_END()

BattleWeapon weapon_doku_kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerHPHalf, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_doku_kinoko_fp = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    weaponGetFPPowerFPHalf, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(doku_kinoko_effect)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_CheckDamage, -3, LW(3), LW(4), PTR(&weapon_doku_kinoko), 0, LW(5))
    CALL(btlevtcmd_GetUnitId, -4, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_CheckDamage, -3, LW(3), LW(4), PTR(&weapon_doku_kinoko), 256, LW(5))
        END_IF
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_CheckDamage, -3, LW(3), LW(4), PTR(&weapon_doku_kinoko_fp), 256, LW(5))
    RETURN
EVT_END()



#ifdef VERSION_G8ME01
// US battle textures are 0x16 higher
u16 tex_id_list_small[5] = {0x41, 0x43, 0x45, 0x44, 0x42};
u16 tex_id_list_big[5] = {0x46, 0x48, 0x4A, 0x49, 0x47};
#define BBS_ICON_OFS 8
#else
u16 tex_id_list_small[5] = {0x2B, 0x2D, 0x2F, 0x2E, 0x2C};
u16 tex_id_list_big[5] = {0x30, 0x32, 0x34, 0x33, 0x31};
#define BBS_ICON_OFS 0
#endif

EVT_BEGIN(star_effect)
    RETURN
EVT_END()

EVT_BEGIN(msg_hp_max)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -205, STRING("msg_slot_HP_max"), 120)
    RETURN
EVT_END()

EVT_BEGIN(msg_fp_max)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -205, STRING("msg_slot_FP_max"), 120)
    RETURN
EVT_END()

EVT_BEGIN(msg_star_max)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -205, STRING("msg_slot_SP_max"), 120)
    RETURN
EVT_END()

EVT_BEGIN(msg_super_max)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -205, STRING("msg_slot_HPFPSP_max"), 120)
    RETURN
EVT_END()

EVT_BEGIN(msg_fault)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -205, STRING("msg_slot_HPFPSP_half"), 120)
    RETURN
EVT_END()


#pragma dont_inline on
BreakSlotWork* BattleBreakSlotGetPtr(void) {
    return &_battleWorkPointer->breakSlot;
}

BreakSlotReel* BattleBreakSlotReelGetPtr(s32 no) {
    return &BattleBreakSlotGetPtr()->reels[no];
}
#pragma dont_inline reset

const SlotCards8 R_slot_1_cards = {{0, 1, 2, 1, 2, 0, 3, 4}};
const SlotCards8 R_slot_2_cards = {{3, 1, 0, 1, 4, 2, 0, 2}};
const SlotCards5 R_slot_3_cards = {{0, 1, 2, 3, 4}};

void BattleBreakSlot_Init(void) {
    BreakSlotWork* wp = BattleBreakSlotGetPtr();
    SlotCards8 c1 = R_slot_1_cards;
    SlotCards8 c2 = R_slot_2_cards;
    SlotCards5 c3 = R_slot_3_cards;
    BreakSlotReel* reel;
    s32 i;

    memset(wp, 0, sizeof(BreakSlotWork));
    i = 0;
    wp->phase = 0;
    reel = BattleBreakSlotReelGetPtr(0);
    for (; i < 3; i++, reel++) {
        reel->kind = i;
        reel->state = 0;
    }
    reel = BattleBreakSlotReelGetPtr(0);
    reel->numCards = 8;
    reel->cards = BattleAlloc(reel->numCards * 4);
    memcpy(reel->cards, &c1, reel->numCards * 4);
    reel = BattleBreakSlotReelGetPtr(1);
    reel->numCards = 8;
    reel->cards = BattleAlloc(reel->numCards * 4);
    memcpy(reel->cards, &c2, reel->numCards * 4);
    reel = BattleBreakSlotReelGetPtr(2);
    reel->numCards = 5;
    reel->cards = BattleAlloc(reel->numCards * 4);
    memcpy(reel->cards, &c3, reel->numCards * 4);
}



void BattleBreakSlot_Main(void) {
    static f32 updown;
    BattleWork* wp = _battleWorkPointer;
    BattleDebugCtrl* dbg2 = &wp->debug;
    BattleAudience* aud = BattleAudienceBaseGetPtr();
    BreakSlotWork* slot = BattleBreakSlotGetPtr();
    BattleUnit* mario = BattleGetMarioPtr(wp);
    BattleUnit* party = BattleGetPartyPtr(wp);
    BreakSlotReel* reel;
    BreakSlotReel* rr;
    s32 count;
    s32 item;
    s32 k;
    s32 i;
    f32 y;

    switch (slot->phase) {
        case 0:
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 2) {
                    count++;
                }
            }
            if (count == 2) {

                if (BattleBreakSlotReelGetPtr(0)->item != BattleBreakSlotReelGetPtr(1)->item) {
                    slot->phase = 1;
                    slot->phaseTimer = 0;
                }
            }
            break;
        case 1:
            if (slot->phaseTimer == 60) {
                reel = BattleBreakSlotReelGetPtr(0);
                for (i = 2; i != 0; i--, reel++) {
                    reel->state = 3;
                    reel->counter = 0;
                }
            }
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 0) {
                    count++;
                }
            }
            if (count == 3) {
                slot->phase = 8;
                slot->reelNo = 0;
            }
            slot->phaseTimer++;
            break;
        case 2:
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 6 || reel->state == 10) {
                    count++;
                }
            }
            if (count == 3) {
                slot->phase = 3;
                slot->phaseTimer = 0;
                reel = BattleBreakSlotReelGetPtr(0);
                for (i = 0; i < 3; i++, reel++) {
                    if (reel->state == 6) {
                        reel->state = 7;
                    }
                }
                slot->startSfxId = psndSFXOn("SFX_BTL_SLOT_START1");
            }
            break;
        case 3:
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 10) {
                    count++;
                }
            }
            if (count == 3) {
                slot->phase = 4;
                slot->phaseTimer = 0;
                psndSFXOff(slot->startSfxId);
            }
            if ((keyGetButtonTrg(0) & PAD_BUTTON_A) && slot->phaseTimer >= 15) {
                reel = BattleBreakSlotReelGetPtr(0);
                for (i = 0; i < 3; i++, reel++) {
                    if (reel->state == 8) {
                        reel->state = 9;
                        reel->spinAccel = -1.0f * (0.2f + 0.15f * rand() / 32767.0f);
                        reel->spinAccel = -1.0f;
                        break;
                    }
                }
            }
            slot->phaseTimer++;
            break;
        case 4:
            if (slot->phaseTimer == 20) {
                count = 0;
                reel = BattleBreakSlotReelGetPtr(0);
                for (i = 0; i < 2; i++, reel++) {
                    if (reel->item == reel[1].item) {
                        count++;
                    }
                }
                if (count >= 2) {
                    slot->phase = 5;
                    psndSFXOn("SFX_BTL_SLOT_SUCCESS1");
                    reel = BattleBreakSlotReelGetPtr(0);
                    for (i = 3; i != 0; i--, reel++) {
                        reel->state = 11;
                        reel->counter = 0;
                    }
                } else {
                    slot->phase = 7;
                    psndSFXOn("SFX_BTL_SYS_BREAK_FAILRE1");
                    reel = BattleBreakSlotReelGetPtr(0);
                    for (i = 3; i != 0; i--, reel++) {
                        reel->state = 12;
                        reel->counter = 0;
                    }
                }
            }
            slot->phaseTimer++;
            break;
        case 5:
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 0) {
                    count++;
                }
            }
            if (count == 3) {
                slot->phase = 6;
                slot->phaseTimer = 0;
            }
            break;
        case 6:
            if (slot->phaseTimer == 0) {
                if (BattleBreakSlotReelGetPtr(0)->item != 4) {
                    effBreakEntry(0, 240, 0.0f, 80.0f, 0.0f);
                } else {
                    slot->phaseTimer = 101;
                    BattleAudienceSoundBooing();
                }
            }
            if (slot->phaseTimer == 20 || slot->phaseTimer == 40 || slot->phaseTimer == 60 || slot->phaseTimer == 80 ||
                slot->phaseTimer == 100 || slot->phaseTimer == 120) {
                psndSFXOn("SFX_BTL_SYS_BREAK1");
                btl_camera_shake_h(0, 5.0f, 0.0f, 10, 0);
            }
            if (slot->phaseTimer == 120) {
                BattleAudienceSoundCheer(240, 240);
            }
            if (slot->phaseTimer == 130) {
                switch (BattleBreakSlotReelGetPtr(0)->item) {
                    case 0:
                        evtEntry(kinoko_effect, 0, EVENT_START_IMMEDIATE);
                        slot->breakTurn = 4;
                        slot->spMultiplier = 2.0f;
                        break;
                    case 1:
                        evtEntry(flower_effect, 0, EVENT_START_IMMEDIATE);
                        slot->breakTurn = 4;
                        slot->spMultiplier = 2.0f;
                        break;
                    case 2:
                        evtEntry(star_effect, 0, EVENT_START_IMMEDIATE);
                        pouchSetAP(pouchGetMaxAP());
                        psndSFXOn("SFX_HP_RECOVER_BIG1");
                        slot->breakTurn = 4;
                        slot->spMultiplier = 2.0f;
                        break;
                    case 3:
                        evtEntry(shine_effect, 0, EVENT_START_IMMEDIATE);
                        pouchSetAP(pouchGetMaxAP());
                        slot->breakTurn = 6;
                        slot->spMultiplier = 3.0f;
                        break;
                    case 4:
                        evtEntry(doku_kinoko_effect, 0, EVENT_START_IMMEDIATE);
                        pouchSetAP(pouchGetAP() / 2);
                        break;
                }
            }
            if (slot->phaseTimer == 140) {
                switch (BattleBreakSlotReelGetPtr(0)->item) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        btl_camera_set_mode(0, 6);
                        btl_camera_set_moveSpeedLv(0, 2);
                        break;
                }
                if (_battleWorkPointer->flags & 0x100) {
                    btl_camera_set_mode(0, 0);
                }
            }
            if (slot->phaseTimer == 180) {
                updown = 0.0f;
                switch (BattleBreakSlotReelGetPtr(0)->item) {
                    case 0:
                    case 1:
                    case 2:
                        updown = BattleAudienceAddAudienceNum(aud->mMaxAudience / 2);
                        break;
                    case 3:
                        updown = BattleAudienceAddAudienceNum(aud->mMaxAudience);
                        break;
                    case 4:
                        updown = BattleAudienceAddAudienceNum(-aud->mMaxAudience);
                        break;
                }
                if (_battleWorkPointer->flags & 0x100) {
                    updown = 0.0f;
                }
            }
            if (slot->phaseTimer == 209 && updown != 0.0f) {
                BattleAudienceSoundRun(0);
            }
            if (slot->phaseTimer == 210) {
                if (!((u32)_battleWorkPointer->cameraWork.flags & 2)) {
                    btl_camera_shake_h(0, 2.0f, 2.0f, 1000, 0);
                }
                for (k = 0; k < 200; k++) {
                    if ((u8)BattleAudience_GetExist(k) == 1) {
                        if (BattleAudienceGetPtr(k)->status == 11 || BattleAudienceGetPtr(k)->status == 12) {
                            slot->phaseTimer--;
                            break;
                        }
                    }
                }
                if (k == 200) {
                    btl_camera_noshake(0);
                    BattleAudienceSoundStop(13);
                }
            }
            if (slot->phaseTimer == 220 && k == 200) {
                btl_camera_set_mode(0, 0);
                btl_camera_noshake(0);
            }
            if (slot->phaseTimer == 270) {
                BattleAudienceAddTargetNum(0.0f, updown);
                slot->phase = 8;
                slot->flags &= ~4;
#ifdef VERSION_G8ME01
                {
#else
                if (gp->language == 0U) {
#endif
                    switch (BattleBreakSlotReelGetPtr(0)->item) {
                        case 0:
                            slot->evt = evtEntry(msg_hp_max, 0, EVENT_START_IMMEDIATE);
                            break;
                        case 1:
                            slot->evt = evtEntry(msg_fp_max, 0, EVENT_START_IMMEDIATE);
                            break;
                        case 2:
                            slot->evt = evtEntry(msg_star_max, 0, EVENT_START_IMMEDIATE);
                            break;
                        case 3:
                            slot->evt = evtEntry(msg_super_max, 0, EVENT_START_IMMEDIATE);
                            break;
                        case 4:
                            slot->evt = evtEntry(msg_fault, 0, EVENT_START_IMMEDIATE);
                            break;
                    }
                }
            }
            slot->phaseTimer++;
            break;
        case 7:
            count = 0;
            reel = BattleBreakSlotReelGetPtr(0);
            for (i = 0; i < 3; i++, reel++) {
                if (reel->state == 0) {
                    count++;
                }
            }
            if (count == 3) {
                slot->phase = 8;
            }
            break;
        case 8:
            slot->phase = 0;
            break;
    }

    i = 0;
    rr = BattleBreakSlotReelGetPtr(0);
    for (; i < 3; i++, rr++) {
        switch (rr->state) {
            case 0:
                rr->flags &= ~1;
                rr->alpha = 255;
                break;
            case 1:
                if (i != 2) {
                    rr->flags |= 1;
                }
                rr->counter++;
                if (rr->counter <= 5) {
                    y = intplGetValue(0, 110.0f, 142.0f, rr->counter, 5);
                    rr->pos = (Vec){42.0f * i + 226.0f, y, 0.0f};
                    rr->scale = (Vec){0.8f, 0.8f, 0.8f};
                    rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                    rr->pivot = (Vec){0.0f, 0.0f, 0.0f};
                    rr->alpha = 255;
                } else {
                    y = intplGetValue(4, 142.0f, 174.0f, rr->counter - 5, 20);
                    rr->pos = (Vec){42.0f * i + 226.0f, y, 0.0f};
                    rr->scale = (Vec){0.8f, 0.8f, 0.8f};
                    rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                    rr->pivot = (Vec){0.0f, 0.0f, 0.0f};
                    rr->alpha = 255;
                }
                if (rr->counter >= 25) {
                    rr->state = 2;
                    rr->index++;
                    if (rr->index >= rr->numCards) {
                        rr->index -= rr->numCards;
                    }
                    rr->item = rr->cards[rr->index];
                    rr->pos = (Vec){42.0f * i + 226.0f, 142.0f, 0.0f};
                }
                break;
            case 2:
                rr->pos = (Vec){42.0f * i + 226.0f, 142.0f, 0.0f};
                rr->scale = (Vec){0.8f, 0.8f, 0.8f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                if (rr->flags & 2) {
                    rr->flags &= ~2;
                    rr->state = 3;
                    rr->counter = 0;
                }
                break;
            case 3:
                y = intplGetValue(0, 142.0f, 174.0f, rr->counter, 20);
                rr->pos = (Vec){42.0f * i + 226.0f, y, 0.0f};
                rr->scale = (Vec){0.8f, 0.8f, 0.8f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                rr->counter++;
                if (rr->counter > 20) {
                    rr->state = 0;
                    rr->counter = 0;
                    rr->flags &= ~1;
                }
                break;
            case 4:
                y = intplGetValue(0, 142.0f, 174.0f, rr->counter, 20);
                rr->pos = (Vec){42.0f * i + 226.0f, y, 0.0f};
                rr->scale = (Vec){0.8f, 0.8f, 0.8f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                rr->counter++;
                if (rr->counter > 20) {
                    rr->state = 5;
                    rr->pos = (Vec){i * 128 - 128, -128.0f, 0.0f};
                    rr->counter = 0;
                }
                break;
            case 5:
                rr->flags |= 1;
                y = intplGetValue(0, -128.0f, 0.0f, rr->counter, 40);
                rr->pos = (Vec){i * 128 - 128, y, 0.0f};
                rr->scale = (Vec){1.0f, 1.0f, 1.0f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                rr->counter++;
                if (rr->counter > 40) {
                    if (rr->kind == 2) {
                        rr->state = 6;
                        rr->counter = 0;
                    } else {
                        rr->state = 10;
                        rr->counter = 0;
                    }
                }
                break;
            case 6:
                break;
            case 7:
                rr->spinSpeed = intplGetValue(0, 0.0f, 22.5f, rr->counter, 20);
                rr->pos = (Vec){128.0f * i + -128.0f, rr->pos.y + rr->spinSpeed, 0.0f};
                rr->scale = (Vec){1.0f, 1.0f, 1.0f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                rr->pivot = (Vec){0.0f, 0.0f, 0.0f};
                if (rr->pos.y >= 128.0f) {
                    rr->pos.y -= 128.0f;
                    rr->index++;
                    if (rr->index >= rr->numCards) {
                        rr->index = 0;
                    }
                    rr->item = rr->cards[rr->index];
                }
                rr->counter++;
                if (rr->counter > 20) {
                    rr->state = 8;
                    rr->counter = 0;
                }
                break;
            case 8:
                rr->spinSpeed = 22.5f;
                rr->pos = (Vec){128.0f * i + -128.0f, rr->pos.y + rr->spinSpeed, 0.0f};
                rr->scale = (Vec){1.0f, 1.0f, 1.0f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                if (rr->pos.y >= 128.0f) {
                    rr->pos.y -= 128.0f;
                    rr->index++;
                    if (rr->index >= rr->numCards) {
                        rr->index = 0;
                    }
                    rr->item = rr->cards[rr->index];
                }
                break;
            case 9:
                rr->spinSpeed += rr->spinAccel;
                if (rr->spinSpeed < 20.0f) {
                    rr->spinSpeed = 20.0f;
                }
                rr->pos = (Vec){128.0f * i + -128.0f, rr->pos.y + rr->spinSpeed, 0.0f};
                rr->scale = (Vec){1.0f, 1.0f, 1.0f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                if (rr->spinSpeed >= 0.0f && rr->pos.y >= 128.0f) {
                    rr->pos.y -= 128.0f;
                    rr->index++;
                    if (rr->index == rr->numCards) {
                        rr->index = 0;
                    }
                    rr->item = rr->cards[rr->index];
                    if ((aud->flags & 0x4000) && BattleBreakSlotReelGetPtr(0)->item == 4 &&
                        BattleBreakSlotReelGetPtr(1)->item == 4 && rr->item == 4) {
                        break;
                    }
                    if ((slot->flags & 2) && BattleBreakSlotReelGetPtr(0)->item == 4 &&
                        BattleBreakSlotReelGetPtr(1)->item == 4 && rr->item == 4) {
                        break;
                    }
                    if ((slot->flags & 1) && rr->item != 3) {
                        break;
                    }
                    if ((slot->flags & 4) && rr->item != 3) {
                        break;
                    }
                    if (((u32)dbg2->flags & 0x80000) && rr->item != dbg2->slotItem) {
                        break;
                    }
                    rr->pos.y = 0.0f;
                    rr->state = 10;
                    rr->counter = 0;
                    rr->spinAccel = rr->spinSpeed;
                }
                break;
            case 10:
                if (i == 2) {
                    rr->counter++;
                    rr->spinSpeed = intplGetValue(0, rr->spinSpeed, 0.0f, rr->counter, 20);
                    rr->pos.y = rr->spinSpeed * (f32)sin(3.141592f * (2.0f * rr->counter) / 8.0f);
                }
                break;
            case 11:
                rr->spinSpeed = 0.0f;
                rr->pos.y = 0.0f;
                y = intplGetValue(0, 1.0f, 2.0f, rr->counter, 60);
                rr->scale = (Vec){y, y, 1.0f};
                rr->alpha = intplGetValue(0, 255.0f, 0.0f, rr->counter, 60);
                rr->counter++;
                if (rr->counter > 60) {
                    rr->state = 13;
                    rr->flags &= ~1;
                }
                break;
            case 12:
                rr->spinSpeed = 0.0f;
                rr->pos.y = 0.0f;
                y = intplGetValue(0, 0.0f, 128.0f, rr->counter, 40);
                rr->pos = (Vec){128.0f * i + -128.0f, y, 0.0f};
                rr->scale = (Vec){1.0f, 1.0f, 1.0f};
                rr->rot = (Vec){0.0f, 0.0f, 0.0f};
                rr->counter++;
                if (rr->counter > 40) {
                    rr->state = 13;
                    rr->flags &= ~1;
                }
                break;
            case 13:
                rr->state = 0;
                break;
        }
    }
}

void BattleBreakSlot_Disp(void) {
    dispEntry(CAMERA_2D, 490.0f, 0, BattleBreakSlotDispReel, NULL);
}

void BattleBreakSlotDispReel(CameraId camId, void* param) {
    Mtx trans;
    Mtx scale;
    Mtx pivot;
    Mtx negPivot;
    Mtx rot;
    Mtx rx;
    Mtx ry;
    Mtx rz;
    Mtx m;
    s32 idx;
    u16 next;
    s32 j;
    BreakSlotReel* reel;
    u16* tex;
    s32 i;

    i = 0;
    reel = BattleBreakSlotReelGetPtr(0);
    for (; i < 3; i++, reel++) {
        if ((reel->flags & 1) && reel->state >= 1 && reel->state <= 4) {
            PSMTXTrans(trans, 42.0f * i + 226.0f, 122.0f, 0.0f);
            iconDispGxCol(trans, 0x10, 0x1D4 + BBS_ICON_OFS, (GXColor){0xFF, 0xFF, 0xFF, 0xD0});
        }
    }

    reel = BattleBreakSlotReelGetPtr(0);
    if (reel->state >= 1 && reel->state <= 4) {
        GXSetScissor(0, 0x52, 0x260, 0x20);
        tex = tex_id_list_small;
    } else {
        tex = tex_id_list_big;
        if (reel->state != 11) {
            GXSetScissor(0, 0xB0, 0x260, 0x80);
        }
    }

    i = 0;
    reel = BattleBreakSlotReelGetPtr(0);
    for (; i < 3; i++, reel++) {
        if (!(reel->flags & 1)) {
            continue;
        }
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        PSMTXTrans(trans, reel->pos.x, reel->pos.y, reel->pos.z);
        PSMTXScale(scale, reel->scale.x, reel->scale.y, reel->scale.z);
        PSMTXRotRad(rx, 'x', 0.017453292f * reel->rot.x);
        PSMTXRotRad(ry, 'y', 0.017453292f * reel->rot.y);
        PSMTXRotRad(rz, 'z', 0.017453292f * reel->rot.z);
        PSMTXConcat(rx, rz, rot);
        PSMTXConcat(ry, rot, rot);
        PSMTXTrans(pivot, reel->pivot.x, reel->pivot.y, reel->pivot.z);
        PSMTXTrans(negPivot, -reel->pivot.x, -reel->pivot.y, -reel->pivot.z);
        PSMTXConcat(rot, pivot, m);
        PSMTXConcat(negPivot, m, m);
        PSMTXConcat(scale, m, m);
        PSMTXConcat(trans, m, m);
        next = tex[reel->item];
        btlDispTexPlane2(m, next, (GXColor){0xFF, 0xFF, 0xFF, reel->alpha}, 0);
        if (reel->state <= 10 && reel->state != 3 && reel->state != 4) {
            idx = reel->index + 1;
            if (idx >= reel->numCards) {
                idx = 0;
            }
            next = tex[reel->cards[idx]];
            if (reel->state >= 1 && reel->state <= 4) {
                PSMTXTrans(trans, 0.0f, -32.0f, 0.0f);
            } else {
                PSMTXTrans(trans, 0.0f, -128.0f, 0.0f);
            }
            PSMTXConcat(trans, m, m);
            btlDispTexPlane2(m, next, (GXColor){0xFF, 0xFF, 0xFF, reel->alpha}, 0);
        }
    }
    GXSetScissor(0, 0, gp->fbWidth, gp->efbHeight);
}

void BattleBreakSlot_End(void) {
#ifdef VERSION_G8ME01
    BattleFree(BattleBreakSlotReelGetPtr(0)->cards);
    BattleFree(BattleBreakSlotReelGetPtr(1)->cards);
    BattleFree(BattleBreakSlotReelGetPtr(2)->cards);
#else
    //stubbed in retail
#endif
}

void BattleBreakSlot_PointInc(void) {
    BattleDebugCtrl* dbg = &_battleWorkPointer->debug;
    BreakSlotReel* reel;
    BreakSlotReel* reel2;
    BreakSlotWork* slot = BattleBreakSlotGetPtr();

    if (pouchGetPtr()->mStarPowersObtained == 0) {
        return;
    }
    reel = BattleBreakSlotReelGetPtr(slot->reelNo);
    reel->state = 1;
    reel->counter = 0;
    slot->reelNo++;
    if (slot->reelNo > 3) {
        slot->reelNo = 3;
        return;
    }
    switch (reel->kind) {
        case 0:
            reel->index = irand(reel->numCards) - 1;
            if (reel->index == -1) {
                reel->index = reel->numCards - 1;
            }
            reel->item = reel->cards[reel->index];
            if ((u32)dbg->flags & 0x80000) {
                item_appear_force(reel, dbg->slotItem);
            }
            if (slot->flags & 1) {
                item_appear_force(reel, 3);
            }
            if (slot->flags & 4) {
                item_appear_force(reel, 3);
            }
            break;
        case 1:
            reel->index = irand(reel->numCards) - 1;
            if (reel->index == -1) {
                reel->index = reel->numCards - 1;
            }
            reel->item = reel->cards[reel->index];
            if ((u32)dbg->flags & 0x80000) {
                item_appear_force(reel, dbg->slotItem);
            }
            if (slot->flags & 1) {
                item_appear_force(reel, 3);
            }
            if (slot->flags & 4) {
                item_appear_force(reel, 3);
            }
            break;
        case 2:
            do {
                reel2 = BattleBreakSlotReelGetPtr(1);
                reel->index = irand(reel->numCards);
                reel->item = reel->cards[reel->index];
            } while (reel->item == reel2->item);
            break;
    }
}

#pragma dont_inline on
void BattleBreakSlot_Start(void) {
    BreakSlotWork* slot = BattleBreakSlotGetPtr();
    BreakSlotReel* reel;
    s32 i;

    if (slot->reelNo >= 3) {
        slot->reelNo = 0;
        slot->phase = 2;
        reel = BattleBreakSlotReelGetPtr(0);
        for (i = 0; i < 3; i++, reel++) {
            reel->state = 4;
            reel->counter = 0;
        }
    }
}
#pragma dont_inline reset

BOOL BattleBreakSlot_CheckReaction(void) {
    BreakSlotWork* slot = BattleBreakSlotGetPtr();

    BattleBreakSlot_Start();
    if (slot->phase >= 1 && slot->phase <= 8) {
        return TRUE;
    }
    if (slot->evt && evtCheckID(slot->evt->eventId)) {
        return TRUE;
    }
    btl_camera_set_mode(0, 0);
    btl_camera_set_moveSpeedLv(0, 2);
    BattleAudienceNumToTarget();
    slot->evt = NULL;
    return FALSE;
}

s32 BattleBreakSlot_GetBreakTurn(void) {
    return BattleBreakSlotGetPtr()->breakTurn;
}

void BattleBreakSlot_DecBreakTurn(void) {
    BreakSlotWork* slot = BattleBreakSlotGetPtr();

    slot->breakTurn--;
    if (slot->breakTurn < 0) {
        slot->breakTurn = 0;
    }
}

void BattleBreakSlot_HideReel(void) {
    BreakSlotWork* slot = BattleBreakSlotGetPtr();
    BreakSlotReel* reel = BattleBreakSlotReelGetPtr(0);
    s32 i;

    for (i = 0; i < 3; i++, reel++) {
        reel->flags |= 2;
    }
    slot->reelNo = 0;
}

#pragma dont_inline on
void item_appear_force(BreakSlotReel* reel, s32 item) {
    s32 i;

    for (i = 0; i < reel->numCards; i++) {
        if (item == reel->cards[i]) {
            reel->index = i - 1;
        }
        if (reel->index == -1) {
            reel->index = reel->numCards - 1;
        }
    }
}
#pragma dont_inline reset
