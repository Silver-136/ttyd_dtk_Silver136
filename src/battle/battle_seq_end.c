#include "battle/battle_seq_end.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "battle/battle_audience.h"
#include "battle/battle_stage_object.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "driver/npcdrv.h"
#include "driver/windowdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "battle/battle_stage.h"
#include "driver/dispdrv.h"
#include "driver/seqdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "battle/battle_pad.h"
#include "battle/battle_information.h"
#include "battle/battle_break_slot.h"
#include "driver/fadedrv.h"
#include "driver/effdrv.h"
#include "pmario_sound.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "driver/mapdrv.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "battle/battle_disp.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_audience.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include <stdio.h>
#include <string.h>
#include "battle/battle_mario.h"
#include "eff/eff_recovery.h"
#include "battle/battle_actrecord.h"
#include "battle/battle_camera.h"
#include "battle/battle_seq.h"

#ifdef VERSION_G8ME01
#define EXP_ICON 0x194
#else
#define EXP_ICON 0x18C
#endif

void FontDrawString(f32 x, f32 y, const char* msg);


typedef struct SeqEndObj {
    Vec pos;                    // 0x0
    Vec scale;                  // 0xC
    Vec rot;                    // 0x18
    u16 value;                  // 0x24
    u16 nextValue;              // 0x26
    u8 paramDisp;               // 0x28
    u8 blinkTimer;              // 0x29, level-up object blink countdown (8 frames)
    u8 pad_2A[0x2C - 0x2A];     // 0x2A
    s32 blinkOn;                // 0x2C, drawn while nonzero
    GXColor color;              // 0x30
} SeqEndObj;

typedef struct SeqEndWork {
    s32 state;                     // 0x0
    s32 timer;                     // 0x4
    s16 exp;                       // 0x8, star points won (doubled by the badge)
    s16 expDisp;                   // 0xA, counts down while exp is added to the pouch
    s32 levelUp;                   // 0xC, set when the pouch exp reaches 100
    s32 selected;                  // 0x10
    s32 unk14;                     // 0x14
    s32 objCount;                  // 0x18
    u8 field_0x1C[0x24 - 0x1C];    // 0x1C
    s32 cursor;                    // 0x24
    s32 unk28;                     // 0x28
    s32 cursorCount;               // 0x2C, number of level-up choices
    u32 flags;                     // 0x30
    IconEntry* icons[19];          // 0x34
    s32 alpha[19];                 // 0x80
    s32 counter[19];               // 0xCC
    Vec iconPos[19];               // 0x118
    s32 evtId;                     // 0x1FC, running _jump_event / level-up event
    SeqEndObj objs[3];             // 0x200
    EffectEntry* expEffect;        // 0x29C, star point counter effect
    s32 whistleTimer;              // 0x2A0
    s32 confettiEvtId;             // 0x2A4
    s32 rankUpEvtId;               // 0x2A8
} SeqEndWork;
void _GetExpIcon_Init(SeqEndWork* work);

typedef struct SeqEndUnitSetup {
    u8* kind;                 // 0x0
    u8 field_0x4[0x30 - 0x4]; // 0x4
} SeqEndUnitSetup;

typedef struct SeqEndGroup {
    s32 count;              // 0x0
    SeqEndUnitSetup* units; // 0x4
} SeqEndGroup;

typedef struct SeqEndSpot {
    u16 flags;               // 0x0
    u8 pad_02[0x4 - 0x2]; // 0x2
    Vec pos;                 // 0x4
    Vec rot;                 // 0x10
    Vec scale;               // 0x1C
    GXColor color;           // 0x28
} SeqEndSpot;

void _GetExpIcon_Main(BattleWork* wp);
void _LvupParamConfirmDisp(CameraId camId, void* param);
void L__LvupParamHelpMsgDisp(CameraId camId, void* param);
void _MarioExpDisp(CameraId camId, void* param);
void _GetExpIcon_Disp(CameraId camId, void* param);
void _lvup_select_object_disp(CameraId camId, void* param);


#define SEW(wp) ((SeqEndWork*)(wp)->seqEndWork)

//.data
USER_FUNC(_check_guard_koura);
USER_FUNC(_disable_enemy_unit_disp);
USER_FUNC(_get_rank_up_msg);
USER_FUNC(_lvup_ap_recover_full);
USER_FUNC(_lvup_disp_flag_onoff);
USER_FUNC(_lvup_obj_set_color);
USER_FUNC(_lvup_object_decide_bound);
USER_FUNC(_lvup_object_noselect_delete);
USER_FUNC(_lvup_object_select_move);
USER_FUNC(_lvup_select_obj_disp_on);
USER_FUNC(_lvup_select_obj_lvup_param_disp);
USER_FUNC(_lvup_select_object_init);
USER_FUNC(_lvup_spot_move);
USER_FUNC(_lvup_spot_off);
USER_FUNC(_lvup_spot_on);
USER_FUNC(_nozzle_set_offset);
USER_FUNC(_set_confetti_control_evtid);
USER_FUNC(btlevtcmd_BtlStageObjFlagOnOff);
USER_FUNC(btlevtcmd_BtlStageObjGetRotate);
USER_FUNC(btlevtcmd_BtlStageObjSetRotate);










RankUpData _rank_up_data[] = {
    {1, 0, "msg_menu_mario_name_0", "butai_01"},
    {10, 1, "msg_menu_mario_name_1", "butai_01"},
    {20, 2, "msg_menu_mario_name_2", "butai_02"},
    {30, 3, "msg_menu_mario_name_3", "butai_03"},
    {0xFFFF, 0, NULL, NULL},
};
EVT_BEGIN(_event_MajinaiEnd)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -36, STRING("btl_msg_mj_last"), 60)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
s32 hp_tex_tbl[6] = {52, 52, 53, 54, 54, 55};
s32 fp_tex_tbl[6] = {56, 56, 57, 58, 58, 58};
s32 bp_tex_tbl[6] = {59, 59, 60, 61, 62, 63};
#endif

EVT_BEGIN(_rank_up_event)
    CALL(_get_rank_up_msg, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, 0)
    RETURN
EVT_END()




RankUpData* _get_rank_data(s32 level) {
    RankUpData* rank;

    for (rank = _rank_up_data; level != rank->startLevel; rank++) {
        if (!rank->rankUpMessage) {
            return NULL;
        }
    }
    return rank;
}

const char* BattleGetRankNameLabel(s32 level) {
    RankUpData* found = NULL;
    RankUpData* rank = _rank_up_data;

    while (level >= rank->startLevel) {
        if (rank->startLevel < 0) {
            break;
        }
        found = rank;
        rank++;
    }
    if (!found) {
        return NULL;
    }
    return found->rankUpName;
}


void _ExecAllUnitBattleEndEvent(void);
void _check_audience_battle_end_exec(void);
void _BattleMarioKouraDelete(void);
void _audience_Whistle_control(void);
void btlseqEnd_DispInit(BattleWork* wp);
void btlseqEnd_DispMain(CameraId camId, void* param);

#define SS(v) BattleSetSeq(wp, (BattleSequence)7, (v))
void _GetExpIcon_End(SeqEndWork* work);
EffectEntry* effPointGetEntry(s32 a, s32 exp, f32 x, f32 y, f32 z, s32 c);

extern PoseTableEntry pose_table_nokotarou_stay[];

void btlseqEnd(BattleWork* wp) {
    SeqEndWork* work = (SeqEndWork*)wp->seqEndWork;
    FieldBattleInfo* info = wp->fbatInfo;
    PouchData* pouch = pouchGetPtr();

    _audience_Whistle_control();
    switch (BattleGetSeq(wp, (BattleSequence)7)) {
    case 0x7000000: {
        s32 marioBelong;
        s32 enemyBelong;
        u32 flags;
        BattleUnit* mu;
        SeqEndWork* nw;
        btlsubResetMoveColorLvAll(wp);
        mu = BattleGetMarioPtr(wp);
        marioBelong = mu->mAlliance;
        enemyBelong = BtlUnit_GetEnemyBelong(mu);
        flags = wp->flags;
        if (flags & 0x40) {
            BattleInformationSetResult(info, 4);
        } else if (flags & 0x20) {
            BattleInformationSetResult(info, 5);
        } else if (wp->alliances[marioBelong].clearConditionMet) {
            BattleInformationSetResult(info, 3);
        } else if (wp->alliances[enemyBelong].clearConditionMet) {
            BattleInformationSetResult(info, 1);
        } else if (flags & 0x10) {
            BattleInformationSetResult(info, 4);
        }
        switch (BattleInformationGetResult(info)) {
        case 1:
            if (fbatGetPointer()->postBattleEvent != NULL) {
                SS(0x7000003);
                return;
            }
            _check_audience_battle_end_exec();
            SS(0x7000005);
            _BattleMarioKouraDelete();
            break;
        case 3:
            SS(0x7000001);
            break;
        case 4:
        default:
            _ExecAllUnitBattleEndEvent();
            SS(0x700001D);
            BattleAudienceSoundSetVolAll(0, 30);
            break;
        }
        nw = BattleAlloc(0x2AC);
        wp->seqEndWork = (BattleSeqEndWork*)nw;
        btlseqEnd_DispInit(wp);
        nw->state = 0;
        nw->expEffect = NULL;
        nw->whistleTimer = irand(120) + 60;
        nw->confettiEvtId = 0;
        break;
    }
    case 0x7000003: {
        FieldBattleData* fb = fbatGetPointer();
        EventEntry* e = evtEntry(fb->postBattleEvent, 10, 0);
        fb->postBattleEventId = e->eventId;
        SS(0x7000004);
        return;
    }
    case 0x7000004: {
        FieldBattleData* fb = fbatGetPointer();
        SeqEndWork* nw;
        if (evtCheckID(fb->postBattleEventId)) break;
        if (!BattleWaitAllActiveEvtEnd(wp)) break;
        if (fb->postBattleMode) {
            BattleSetSeq(wp, (BattleSequence)3, 0x3000001);
            BattleSetSeq(wp, (BattleSequence)0, 2);
            return;
        }
        SS(0x7000005);
        _BattleMarioKouraDelete();
        nw = BattleAlloc(0x2AC);
        wp->seqEndWork = (BattleSeqEndWork*)nw;
        btlseqEnd_DispInit(wp);
        nw->state = 0;
        return;
    }
    case 0x7000001: {
        _ExecAllUnitBattleEndEvent();
        btl_camera_set_moveSpeedLv(0, 2);
        btl_camera_set_mode(0, 0x13);
        btl_camera_set_posoffset(0, 0.0f, 0.0f, 0.0f);
        fadeEntry(FADE_OUT_BLACK, 200, (GXColor){0, 0, 0, 255});
        psndBGMOff(0x201);
        SS(0x7000002);
        break;
    }
    case 0x7000002:
        if (!fadeIsFinish()) break;
        if (!((u32)wp->fbatInfo->setup->battleFlags & 0x40)) {
            seqSetSeq(SEQ_GAME_OVER, (const char*)1, NULL);
            break;
        }
        SS(0x700001F);
        break;
    case 0x7000005: {
        BattleUnit* mario = BattleGetMarioPtr(wp);
        BattleUnit* party = BattleGetPartyPtr(wp);
        s32 i;
        if (party && BtlUnit_CheckStatusFlag(party, 1)) {
            party->flippedTurns = 0;
            BtlUnit_OffStatusFlag(party, 1);
            BtlUnit_GetPartsPtr(party, BtlUnit_GetBodyPartsId(party))->poseTable = pose_table_nokotarou_stay;
        }
        BattleConsumeReserveItem();
        for (i = 0; (s8)i < 0x1C; i++) {
            if ((s8)i >= 0x1C || (s8)i < 0x19) {
                if (party) BtlUnit_SetStatus(party, i, 0, 0);
                BtlUnit_SetStatus(mario, i, 0, 0);
            }
        }
        if ((u32)mario->mAttributeFlags & kVeiled) {
            BattleUnit* p2 = BattleGetPartyPtr(wp);
            EventEntry* e;
            if (!p2) break;
            e = evtEntry(BtlUnit_GetData(p2, 3), 10, 0);
            e->unitId = p2->unitId;
            p2->attackEventId = e->eventId;
            break;
        }
        SS(0x7000006);
    }
    case 0x7000006: {
        BattleUnit* mario;
        PouchData* pp;
        if (!BattleWaitAllActiveEvtEnd(wp)) break;
        BattleAudienceSoundClap(-1, 0);
        _ExecAllUnitBattleEndEvent();
        BattleInformationSetDropMaterial(info);
        pp = pouchGetPtr();
        mario = BattleGetMarioPtr(wp);
        if (pp->mMerleeNextCurseType == 3) {
            EventEntry* e;
            wp->majinaiState = 3;
            BattleMajinaiDone();
            BattleMajinaiEndCheck();
            e = evtEntry(marioAttackEvent_MajinaiExpUp, 10, 0);
            e->unitId = mario->unitId;
            mario->attackEventId = e->eventId;
            SS(0x7000007);
            break;
        }
        SS(0x7000008);
        break;
    }
    case 0x7000007:
        if (!BattleWaitAllActiveEvtEnd(wp)) break;
        SS(0x7000008);
        break;
    case 0x7000008: {
        s32 flag;
        BattleFogForceStop();
        work->levelUp = 0;
        work->exp = BattleGetStockExp(wp);
        if (wp->majinaiState == 3) {
            work->exp = work->exp * 2;
        }
        if (pouch->level >= 99) {
            work->exp = 0;
        }
        if (work->exp > 100) {
            work->exp = 100;
        }
        work->expDisp = work->exp;
        wp->dispFlags &= ~2;
        if (work->exp > 0) {
            _GetExpIcon_Init(work);
        }
        work->timer = 0x78;
        btl_camera_set_moveSpeedLv(0, 2);
        if (!(wp->flags & 0x100)) {
            btl_camera_set_mode(0, 0x11);
        } else {
            btl_camera_set_mode(0, 0x12);
        }
        btl_camera_set_posoffset(0, 0.0f, 0.0f, 0.0f);
        SS(0x7000009);
        BattleAudienceSoundCheer(0x3C, 0x3C);
        BattleAudienceSoundClap(-1, 0);
        BattleAudienceSoundWhistle();
        flag = 0;
        {
            BattleUnit* party = BattleGetPartyPtr(wp);
            if (party && BtlUnit_CheckStatus(party, (StatusEffectType)0x1B)) {
                flag = 1;
            }
        }
        if (!flag) {
            psndBGMOff(0x400);
            psndBGMOn(1, "BGM_BATTLE_WIN1");
        } else {
            psndBGMOff(0x400);
            psndBGMOn(1, "BGM_BATTLE_WIN2");
        }
    }
    case 0x7000009:
        work->timer = work->timer - 1;
        if (work->timer > 0) break;
        SS(0x700000A);
        break;
    case 0x700000A:
        if (work->exp > 0) {
            BattleUnit* mario = BattleGetMarioPtr(wp);
            BattleUnit* party = BattleGetPartyPtr(wp);
            EventEntry* e = evtEntry(BtlUnit_GetData(mario, 0x40), 10, 0);
            if (e) {
                e->unitId = mario->unitId;
                mario->attackEventId = e->eventId;
            }
            if (party) {
                e = evtEntry(BtlUnit_GetData(party, 0x40), 10, 0);
                if (e) {
                    e->unitId = party->unitId;
                    party->attackEventId = e->eventId;
                }
            }
            if (!work->expEffect) {
                work->expEffect = ((EffectEntry* (*)(f32, f32, f32, s32, s32, s32))effPointGetEntry)(0.0f, 90.0f, 0.0f, 0, work->exp, 0);
            }
            SS(0x700000B);
            break;
        }
        if (!(wp->flags & 0x400000)) {
            SS(0x7000019);
            break;
        }
        SS(0x700000E);
        break;
    case 0x700000B:
        if (work->expDisp > 0) {
            work->timer = work->timer + 1;
            if ((s32)(s32)BattlePadCheckNow(0x300) != 0) {
                if (work->timer < 1000) {
                    work->timer = work->timer + 1000;
                }
            }
            if ((work->timer & 3) == 0 || work->timer >= 1000) {
                work->expDisp = work->expDisp - 1;
                pouch->mStarPoints = pouch->mStarPoints + 1;
                if ((work->timer & 7) == 0) {
                    psndSFXOn("SFX_STAR_POINT_GET1");
                }
            }
            if (pouch->mStarPoints < 100) break;
            pouch->mStarPoints = 0;
            work->levelUp = 1;
            if (pouch->level + 1 < 99) break;
            work->expDisp = 0;
            break;
        }
        if (work->timer >= 1000) {
            work->timer = 10;
        } else if (work->timer < 0x78) {
            work->timer = 0x8C - work->timer;
        } else {
            work->timer = 0x14;
        }
        SS(0x700000C);
        break;
    case 0x700000C:
        work->timer = work->timer - 1;
        if (work->timer > 0) break;
        work->timer = 0x3C;
        _GetExpIcon_End(work);
        SS(0x700000D);
        effSoftDelete(work->expEffect);
        if (work->levelUp) {
            work->timer = 0x3C;
            break;
        }
        work->timer = 5;
        break;
    case 0x700000D:
        work->timer = work->timer - 1;
        if (work->timer > 0) break;
        work->expEffect = NULL;
        SS(0x700000E);
        break;
    case 0x700000E:
        if (wp->flags & 0x400000) {
            BattleUnit* mario = BattleGetMarioPtr(wp);
            EventEntry* e = evtEntry(_event_MajinaiEnd, 10, 0);
            e->unitId = mario->unitId;
            mario->attackEventId = e->eventId;
            SS(0x700000F);
            break;
        }
        SS(0x7000010);
        break;
    case 0x700000F:
        if (!BattleWaitAllActiveEvtEnd(wp)) break;
        if (work->exp > 0) {
            SS(0x7000010);
            break;
        }
        SS(0x7000019);
        break;
    case 0x7000010:
        work->flags &= ~1;
        if (work->levelUp) {
            work->selected = 1;
            work->unk14 = 0;
            work->objCount = 3;
            work->timer = 0;
            BattleAudience_WinSetActive(0);
            BattleBreakSlot_HideReel();
            SS(0x7000011);
            break;
        }
        SS(0x7000019);
        break;
    case 0x7000011:
        if (work->timer == 0) {
            EventEntry* e = evtEntry(_curtain_close_event, 10, 0);
            work->evtId = e->eventId;
            BattleStatusWindowSystemOff();
            psndBGMOff(0x400);
            psndBGMOn(1, "BGM_BATTLE_WIN3");
        }
        work->timer = work->timer + 1;
        if (evtCheckID(work->evtId)) break;
        work->evtId = 0;
        BattleStatusWindowSystemOn();
        BattleStatusWindowEventOn();
        SS(0x7000012);
        break;
    case 0x7000012: {
        s32 sel = work->selected;
        EventEntry* e;
        if (work->evtId) {
            if (evtCheckID(work->evtId)) break;
            work->evtId = 0;
        }
        if (BattlePadCheckTrigger(0x80000)) {
            if (work->selected < work->objCount - 1) {
                work->selected = work->selected + 1;
            }
        }
        if (BattlePadCheckTrigger(0x40000)) {
            if (work->selected > 0) {
                work->selected = work->selected - 1;
            }
        }
        if (work->selected != sel) {
            e = NULL;
            switch (work->selected) {
            case 0:
                e = evtEntry(_move_to_heart_event, 10, 0);
                break;
            case 1:
                e = evtEntry(_move_to_flower_event, 10, 0);
                break;
            case 2:
                e = evtEntry(_move_to_badge_point_event, 10, 0);
                break;
            }
#ifdef VERSION_G8ME01
            work->flags &= ~0x20;
#endif
            if (e) {
                work->evtId = e->eventId;
                break;
            }
        }
        if (BattlePadCheckTrigger(0x100)) {
            s32 ok = 0;
            switch (work->selected) {
            case 0:
                if (pouch->baseMaxHP < 200) ok = 1;
                break;
            case 1:
                if (pouch->baseMaxFP < 200) ok = 1;
                break;
            case 2:
                if (pouch->maximumBP < 99) ok = 1;
                break;
            }
            if (ok) {
                work->cursor = 0;
                work->unk28 = 0;
                work->cursorCount = 2;
                work->flags &= ~0x20;
                SS(0x7000014);
                e = evtEntry(_jump_event, 10, 0);
                work->evtId = e->eventId;
            }
        }
        break;
    }
    case 0x7000013:
        if (BattlePadCheckTrigger(0x20000)) {
            work->cursor = work->cursor + 1;
            if (work->cursor >= work->cursorCount) {
                work->cursor = 0;
            }
        }
        if (BattlePadCheckTrigger(0x10000)) {
            work->cursor = work->cursor - 1;
            if (work->cursor < 0) {
                work->cursor = work->cursorCount - 1;
            }
        }
        if (BattlePadCheckTrigger(0x200)) {
            work->flags &= ~0x10;
            SS(0x7000012);
        }
        if (BattlePadCheckTrigger(0x100)) {
            if (work->cursor == 0) {
                work->flags &= ~0x18;
                SS(0x7000015);
                break;
            }
            work->flags &= ~0x10;
            SS(0x7000012);
        }
        break;
    case 0x7000014:
        if (evtCheckID(work->evtId)) break;
        work->evtId = 0;
        SS(0x7000015);
        break;
    case 0x7000015: {
        BattleUnit* party;
        BattleUnit* party2;
        s32 ok;
        NpcBattleInfo* fd;
        BattleUnit* mario;
        pouch->level = pouch->level + 1;
        switch (work->selected) {
        case 0:
            pouch->baseMaxHP = pouch->baseMaxHP + 5;
            break;
        case 1:
            pouch->baseMaxFP = pouch->baseMaxFP + 5;
            break;
        case 2:
            pouch->maximumBP = pouch->maximumBP + 3;
            break;
        }
        pouchReviseMarioParam();
        pouch->currentHP = pouch->maximumHP;
        pouch->currentFP = pouch->maximumFP;
        BtlUnit_SetParamFromPouch(BattleGetMarioPtr(wp));
        party = BattleGetPartyPtr(wp);
        if (party) {
            s32 id = BattleTransPartyId((s32)party->currentKind);
            if (id != 0 && id < 8) {
                PouchPartyData* pe = &pouch->partyData[id];
                pe->currentHP = pe->maximumHP;
                party->currentHp = party->maxHp;
            }
        }
        fd = wp->fbatInfo->setup;
        ok = 1;
        mario = BattleGetMarioPtr(wp);
        party2 = BattleGetPartyPtr(wp);
        if (((u32)mario->mStatusFlags & 0x10000000) || ((u32)mario->mStatusFlags & 0x20000000)) {
            ok = 0;
        } else if (party2) {
            if (((u32)mario->mStatusFlags & 0x10000000) || ((u32)mario->mStatusFlags & 0x20000000) ||
                BtlUnit_CheckStatus(party2, (StatusEffectType)0x1B)) {
                ok = 0;
            }
        }
        if (ok) {
            if ((u32)fd->battleFlags & 0x20000000) {
                BtlUnit_snd_se(mario, "SFX_VOICE_MARIO_PAUSE3", EVT_NULLPTR, 0);
            } else {
                BtlUnit_snd_se(mario, "SFX_VOICE_MARIO_PAUSE1", EVT_NULLPTR, 0);
            }
        }
        BtlUnit_SetBodyAnim(BattleGetMarioPtr(wp), "M_V_2");
        BattleAudienceSoundCheer(0xB4, 0x28);
        work->timer = 0xB4;
        SS(0x7000016);
        mario = BattleGetMarioPtr(wp);
        party = BattleGetPartyPtr(wp);
        if (mario) {
            effRecoveryEntry(4, 0, mario->mPosition.x, 40.0f + mario->mPosition.y, mario->mPosition.z);
        }
        if (party) {
            effRecoveryEntry(4, 0, party->mPosition.x, 40.0f + party->mPosition.y, party->mPosition.z);
        }
        break;
    }
    case 0x7000016:
        work->timer = work->timer - 1;
        if (work->timer == 0x78) {
            BattleUnit* mario = BattleGetMarioPtr(wp);
            if (mario) {
                effRecoveryEntry(5, 0, mario->mPosition.x, 40.0f + mario->mPosition.y, mario->mPosition.z);
            }
        }
        if (work->timer > 0) break;
        if (work->confettiEvtId) evtDeleteID(work->confettiEvtId);
        SS(0x7000017);
        break;
    case 0x7000017: {
        EventEntry* e;
        if (!_get_rank_data(pouch->level)) {
            SS(0x700001C);
            break;
        }
        pouch->rank = _get_rank_data(pouch->level)->rankId;
        e = evtEntry(_rank_up_event, 10, 0);
        work->rankUpEvtId = e->eventId;
        SS(0x7000018);
    }
    case 0x7000018:
        if (evtCheckID(work->rankUpEvtId)) break;
        SS(0x700001C);
        break;
    case 0x7000019: {
        s32 ok = 1;
        NpcBattleInfo* fd = wp->fbatInfo->setup;
        BattleUnit* mario = BattleGetMarioPtr(wp);
        BattleUnit* party = BattleGetPartyPtr(wp);
        if (((u32)mario->mStatusFlags & 0x10000000) || ((u32)mario->mStatusFlags & 0x20000000)) {
            ok = 0;
        } else if (party) {
            if (((u32)mario->mStatusFlags & 0x10000000) || ((u32)mario->mStatusFlags & 0x20000000) ||
                BtlUnit_CheckStatus(party, (StatusEffectType)0x1B)) {
                ok = 0;
            }
        }
        if (!ok) {
            if ((u32)fd->battleFlags & 0x20000000) {
                BtlUnit_snd_se(mario, "SFX_VOICE_MARIO_RELIEF1", EVT_NULLPTR, 0);
            } else {
                BtlUnit_snd_se(mario, "SFX_VOICE_MARIO_RELIEF2", EVT_NULLPTR, 0);
            }
        }
        BtlUnit_SetBodyAnim(BattleGetMarioPtr(wp), "M_V_1");
        work->timer = 0x3C;
        SS(0x700001A);
        break;
    }
    case 0x700001A:
        if (--work->timer > 0) break;
        BtlUnit_SetBodyAnimType(BattleGetMarioPtr(wp), 0x2A);
        if (BattleGetPartyPtr(wp)) {
            BattleUnit* party = BattleGetPartyPtr(wp);
            if ((s32)party->currentKind != UNIT_KOOPS || party->flippedTurns < 1) {
                BtlUnit_SetBodyAnimType(BattleGetPartyPtr(wp), 0x2A);
            }
        }
        SS(0x700001B);
        break;
    case 0x700001B:
        SS(0x700001C);
        break;
    case 0x700001C: {
        FieldBattleData* fb;
        NpcEntry* p;
        SS(0x700001D);
        BattleAudienceSoundSetVolAll(0, 30);
        fb = fbatGetPointer();
        p = fb->target;
        if (p) p->interruptFlags |= 0x100;
        break;
    }
    case 0x700001D: {
        fadeEntry(FADE_OUT_BLACK, 200, (GXColor){0, 0, 0, 255});
        SS(0x700001E);
        psndBGMOff(0x201);
    }
    case 0x700001E:
        if (!fadeIsFinish()) break;
        SS(0x700001F);
        break;
    case 0x700001F: {
        BattleUnit* party = BattleGetPartyPtr(wp);
        s32 kind;
        s32 id;
        NpcBattleInfo* fd;
        BattleUnit* mario;
        MarioWork* mw;
        void* pe;
        if (party) kind = (s32)party->currentKind; else kind = 0;
        id = BattleTransPartyId(kind);
        fd = wp->fbatInfo->setup;
        mario = BattleGetMarioPtr(wp);
        if ((u32)fd->battleFlags & 0x40) {
            if (mario->currentHp <= 0) mario->currentHp = 1;
        }
        BattleSetMarioParamToFieldBattle(wp);
        BattleFree(wp->seqEndWork);
        wp->seqEndWork = NULL;
        BattleSetSeq(wp, (BattleSequence)0, 4);
        mw = marioGetPtr();
        mw->dispFlags &= ~1;
        if (kind) marioPartyEntry((PartyId)id); else marioPartyKill();
        pe = partyGetPtr(marioGetPartyId());
        if (pe) ((PartyEntry*)pe)->flags2 &= ~1;
        BtlActRec_JudgeRuleKeep();
        BattleAudienceSoundStop(1);
        break;
    }
    }
    btlseqEnd_DispMain((CameraId)8, wp);
}

#pragma dont_inline on
void _ExecAllUnitBattleEndEvent(void) {
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    void* data;

    EventEntry* evt;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            data = BtlUnit_GetData(unit, 0x3F);
            if (data) {
                evt = evtEntry(data, 10, 0);
                evt->unitId = unit->unitId;
            }
        }
    }
}

void btlseqEnd_DispInit(BattleWork* wp) {
    SEW(wp)->flags = 0;
}

void btlseqEnd_DispMain(CameraId camId, void* param) {
    BattleWork* wp = (BattleWork*)param;
    SeqEndWork* work = SEW(wp);


    if (work) {
        if (work->flags & 2) {
            _GetExpIcon_Main(wp);
            dispEntry(camId, 900.0f, 1, _GetExpIcon_Disp, wp);
        }
        if (work->flags & 4) {
            dispEntry(camId, 900.0f, 1, _MarioExpDisp, wp);
        }
        if (work->flags & 8) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, _lvup_select_object_disp, wp);
        }
        if (work->flags & 0x10) {
            dispEntry(camId, 900.0f, 1, _LvupParamConfirmDisp, wp);
        }
        if (work->flags & 0x20) {
            dispEntry(camId, 900.0f, 1, L__LvupParamHelpMsgDisp, wp);
        }
    }
}

void _MarioExpDisp(CameraId camId, void* param) {
    SeqEndWork* work = SEW((BattleWork*)param);
    char buf[0x10];
    Vec pos;
    f32 width;

    pouchGetPtr();
    pos.x = 200.0f;
    pos.y = -200.0f;
    pos.z = 0.0f;
    if (work) {
        sprintf(buf, "%2d", work->expDisp);
        { u32 len = strlen(buf); width = 12.5f * len + 65.0f; }
        ((void (*)(f32, f32, f32, f32, f32, u16, GXColor))windowDispGX_Waku_col)(180.0f, -160.0f, width, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});

        iconDispGx(&pos, 0x10, EXP_ICON, 1.0f);
        FontDrawStart();
        FontDrawString(225.0f, -163.0f, buf);
    }
}

void _LvupParamConfirmDisp(CameraId camId, void* param) {
    Vec pos; f32 zero = 0.0f; u32 len; s32 *cursorPtr; f32 w; char buf[0x14]; SeqEndWork* work = SEW((BattleWork*)param);
    cursorPtr = &work->cursor; // FAKE: pointer to the field; direct work->cursor differs
    pouchGetPtr();
    sprintf(buf, msgSearch("btl_msg_lvup_ok"));
    len = strlen(buf); ((void (*)(f32, f32, f32, f32, f32, u16, GXColor))windowDispGX_Waku_col)(-30.0f - (w = 6.25f * len), 30.0f, 12.5f * len + 65.0f, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -70.0f, -10.0f, 130.0f, 60.0f, 10.0f);
    FontDrawStart();
    FontDrawString(zero - w, 27.0f, buf);
    FontDrawString(-20.0f, -13.0f, msgSearch("btl_msg_lvup_yes"));
    FontDrawString(-20.0f, -38.0f, msgSearch("btl_msg_lvup_no"));
    pos.x = -40.0f;
    pos.y = -50 - (*cursorPtr) * 25;
    pos.z = 0.0f;
    iconDispGx(&pos, 0x10, EXP_ICON, 1.0f);
}

void L__LvupParamHelpMsgDisp(CameraId camId, void* param) {
    SeqEndWork* work = SEW((BattleWork*)param);
    PouchData* pouch;
    const char* msg;
    u16 lines;
    s32 width;

    f32 y;
    f32 zero = 0.0f;

    pouch = pouchGetPtr();
    switch (work->selected) {
        case 0:
            msg = msgSearch("help_btl_levelup_hp");
            if (pouch->baseMaxHP >= 200) {
                msg = msgSearch("helperr_btl_levelup_hp");
            }
            break;
        case 1:
            msg = msgSearch("help_btl_levelup_fp");
            if (pouch->baseMaxFP >= 200) {
                msg = msgSearch("helperr_btl_levelup_fp");
            }
            break;
        case 2:
            msg = msgSearch("help_btl_levelup_bp");
            if (pouch->maximumBP >= 99) {
                msg = msgSearch("helperr_btl_levelup_bp");
            }
            break;
    }
    width = FontGetMessageWidthLine(msg, &lines);
    lines++;
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, (zero - width / 2) - 20.0f, -145.0f + (lines - 1) * 29, width + 40, lines * 29 + 1, 10.0f);
    FontDrawStart_alpha(0xFF);
    y = -148.0f;
    FontDrawMessage(zero - width / 2, (s32)y + (lines - 1) * 29, msg);
}

void _GetExpIcon_Init(SeqEndWork* work) {
    s32 i;
    char name[16];

    for (i = 0; i < 10; i++) {
        sprintf(name, "_Bgsp%d", i);
        iconEntry2D(name, EXP_ICON);
        work->icons[i] = iconNameToPtr(name);
        iconSetScale(name, 0.9f);
        iconFlagOn(name, 2);
        work->counter[i] = 0;
        work->iconPos[i].x = 24.0f - 18.0f * (work->expDisp / 10) + 36.0f * i;
        work->iconPos[i].y = 25.0f;
        work->iconPos[i].z = 0.0f;
        iconSetPos(name, work->iconPos[i].x, work->iconPos[i].y, work->iconPos[i].z);
    }
    for (i = 10; i < 19; i++) {
        sprintf(name, "_Bgsp%d", i);
        iconEntry2D(name, EXP_ICON);
        work->icons[i] = iconNameToPtr(name);
        iconSetScale(name, 0.45f);
        iconFlagOn(name, 2);
        work->counter[i] = 0;
        work->iconPos[i].x = 86.899994f - 16.199999f * (i - 9);
        work->iconPos[i].y = 0.0f;
        work->iconPos[i].z = 0.0f;
        iconSetPos(name, work->iconPos[i].x, work->iconPos[i].y, work->iconPos[i].z);
    }
    work->flags |= 2;
}

void _GetExpIcon_Main(BattleWork* wp) {
    s32 digits[2]; // FAKE: register-promoted array (tens, ones); two scalars differ
    SeqEndWork* work = SEW(wp);
    s32 exp = work->expDisp;
    s32 i;
    f32 ofs;

    digits[1] = exp / 10;
    for (i = 0; i < digits[1]; i++) {
        iconFlagOff(work->icons[i]->name, 2);
        if (work->alpha[i] <= 0) {
            work->alpha[i] = 0x40;
        } else {
            work->alpha[i] += 0x40;
            if (work->alpha[i] > 0xFF) {
                work->alpha[i] = 0xFF;
            }
        }
        if (work->counter[i] < 16) {
            work->counter[i]++;
        }
        work->iconPos[i].x = 24.0f - 18.0f * (work->expDisp / 10) + 36.0f * i;
    }
    for (; i < 10; i++) {
        iconFlagOn(work->icons[i]->name, 2);
        work->alpha[i] = 0;
        work->counter[i] = 0;
    }
    digits[0] = exp % 10;
    for (i = 10; i - 10 < digits[0]; i++) {
        iconFlagOff(work->icons[i]->name, 2);
        if (work->alpha[i] <= 0) {
            work->alpha[i] = 0x40;
        } else {
            work->alpha[i] += 0x40;
            if (work->alpha[i] > 0xFF) {
                work->alpha[i] = 0xFF;
            }
        }
    }
    for (; i < 19; i++) {
        iconFlagOn(work->icons[i]->name, 2);
        work->alpha[i] = 0;
    }
    ofs = 0.0f;
    for (i = 0; i < 19; i++) {
        iconSetAlpha(work->icons[i]->name, work->alpha[i]);
        iconSetPos(work->icons[i]->name, work->iconPos[i].x, work->iconPos[i].y + ofs, work->iconPos[i].z);
    }
}

void _GetExpIcon_Disp(CameraId camId, void* param) {
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0x80}, -76.899994f, 21.0f, 165.79999f, 24.199999f, 10.0f);
}

void _GetExpIcon_End(SeqEndWork* work) {
    s32 i;

    work->flags &= ~2;
    for (i = 0; i < 19; i++) {
        iconDelete(work->icons[i]->name);
    }
}

#ifdef VERSION_G8ME01
USER_FUNC(N__display_select_one_to_upgrade_text);
USER_FUNC(N__set_initial_upgrade_cursor_position);

EVT_BEGIN(_curtain_close_event)
    BEGIN_SIBLING_EVENT
        CALL(evt_batstage_set_stg_dark, 80, 30, 0)
        CALL(evt_batstage_set_stg_dark_base, 80, 30, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_fade_entry, 24, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
    END_SIBLING_EVENT
    CALL(btlevtcmd_check_battleflag, LW(0), 256)
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_AnimeChangePoseType, -3, 1, 42)
            CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_MovePosition, -3, -300, LW(1), LW(2), 120, -1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetUnitId, -4, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
                CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 42)
                CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_MovePosition, LW(3), -330, LW(1), LW(2), 120, -1, 0)
            END_SIBLING_EVENT
        END_IF
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_l"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_c"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_r"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_onoff_battleflag, 0, 256)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_MS(1500)
    BEGIN_SIBLING_EVENT
        LOOP(6)
            CALL(evt_eff, STRING(""), STRING("confetti"), 13, 0, 300, 95, 180, 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    SPAWN_EVENT_GET_ID(PTR(_confetti_control_event), LW(0))
    CALL(_set_confetti_control_evtid, LW(0))
    CALL(evt_eff, STRING(""), STRING("levelup"), 0, 0, 50, 100, 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(48)
    CALL(_lvup_ap_recover_full)
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(2)
        CALL(_lvup_spot_on, 0)
        CALL(_lvup_spot_move, 0)
    END_SIBLING_EVENT
    CALL(_disable_enemy_unit_disp)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetUnitId, -4, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetPos, LW(3), -230, 0, 90)
            CALL(btlevtcmd_ClearAllStatus, LW(3))
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 40)
            SET_VALUE(LW(0), -155)
            SET_VALUE(LW(1), 0)
            SET_VALUE(LW(2), 90)
            SET_VALUE(LW(6), 28)
            CALL(btlevtcmd_GetUnitKind, LW(3), LW(5))
            SWITCH(LW(5))
                CASE_OR_EQUAL(224)
                CASE_OR_EQUAL(225)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 6, 12, 16, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_END
                CASE_EQUAL(226)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 6, 6, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(227)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 6, 12, 16, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(228)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 0, 15, 13, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(229)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 18, 18, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(230)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 15, 13, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
            END_SWITCH
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 43)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetPos, -3, -200, 0, 90)
    CALL(btlevtcmd_ClearAllStatus, -3)
    CALL(btlevtcmd_AnimeChangePoseType, -3, 1, 40)
    CALL(btlevtcmd_MovePosition, -3, 0, 0, 90, 75, -1, 0)
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_PAUSE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_V_2"))
    CALL(evt_audience_sound_cheer, 180, 40)
    WAIT_MS(1000)
    CALL(_lvup_select_object_init)
    BEGIN_SIBLING_EVENT
        CALL(_lvup_spot_off, 30)
    END_SIBLING_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_batstage_set_stg_dark, 0, 30, 0)
        CALL(evt_batstage_set_stg_dark_base, 0, 30, 0)
        CALL(_lvup_obj_set_color, 0, 100, 128, 128, 255)
        CALL(_lvup_obj_set_color, 1, 100, 128, 128, 255)
        CALL(_lvup_obj_set_color, 2, 100, 128, 128, 255)
        CALL(evt_batstage_set_aud_dark, 0, 60, 0)
        CALL(evt_fade_entry, 25, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
    END_INLINE_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(10))
    BEGIN_SIBLING_EVENT_GET_ID(LW(10))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(N__display_select_one_to_upgrade_text)
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    LABEL(60)
    CALL(btlevtcmd_PadCheckTrigger, 0, 262144, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(0))
            CALL(N__set_initial_upgrade_cursor_position, 2)
            CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, 70)
            CALL(btlevtcmd_BtlStageObjFlagOnOff, STRING("spotlight"), 1, 0)
            CALL(evt_sub_intpl_init, 5, 70, 30, 10)
            LOOP(11)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(_lvup_obj_set_color, 2, 255, 255, 255, 255)
            CALL(_lvup_disp_flag_onoff, 2, 32)
            CALL(_lvup_select_obj_disp_on)
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(0))
        GOTO(69)
    END_IF
    LABEL(61)
    CALL(btlevtcmd_PadCheckTrigger, 0, 524288, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(0))
            CALL(N__set_initial_upgrade_cursor_position, 0)
            CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, -70)
            CALL(btlevtcmd_BtlStageObjFlagOnOff, STRING("spotlight"), 1, 0)
            CALL(evt_sub_intpl_init, 5, -70, -30, 10)
            LOOP(11)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(_lvup_obj_set_color, 0, 255, 255, 255, 255)
            CALL(_lvup_disp_flag_onoff, 0, 32)
            CALL(_lvup_select_obj_disp_on)
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(0))
        GOTO(69)
    END_IF
    WAIT_FRAMES(1)
    GOTO(60)
    LABEL(69)
    DELETE_EVENT(LW(10))
    CALL(btlevtcmd_BtlStageObjFlagOnOff, STRING("spotlight"), 1, 0)
    CALL(_lvup_select_obj_lvup_param_disp)
    CALL(_lvup_disp_flag_onoff, 1, 32)
    RETURN
EVT_END()
#else
EVT_BEGIN(_curtain_close_event)
    BEGIN_SIBLING_EVENT
        CALL(evt_batstage_set_stg_dark, 80, 30, 0)
        CALL(evt_batstage_set_stg_dark_base, 80, 30, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_fade_entry, 24, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
    END_SIBLING_EVENT
    CALL(btlevtcmd_check_battleflag, LW(0), 256)
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_AnimeChangePoseType, -3, 1, 42)
            CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_MovePosition, -3, -300, LW(1), LW(2), 120, -1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetUnitId, -4, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
                CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 42)
                CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_MovePosition, LW(3), -330, LW(1), LW(2), 120, -1, 0)
            END_SIBLING_EVENT
        END_IF
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_l"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_c"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 0, 0, FLOAT(-40.0f), 29)
        LOOP(30)
            CALL(evt_sub_intpl_get_float, LW(0), EVT_NULLPTR)
            CALL(_nozzle_set_offset, STRING("nozzle_r"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_onoff_battleflag, 0, 256)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_MS(1500)
    BEGIN_SIBLING_EVENT
        LOOP(6)
            CALL(evt_eff, STRING(""), STRING("confetti"), 13, 0, 300, 95, 180, 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    SPAWN_EVENT_GET_ID(PTR(_confetti_control_event), LW(0))
    CALL(_set_confetti_control_evtid, LW(0))
    CALL(evt_eff, STRING(""), STRING("levelup"), 0, 0, 50, 100, 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(48)
    CALL(_lvup_ap_recover_full)
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(2)
        CALL(_lvup_spot_on, 0)
        CALL(_lvup_spot_move, 0)
    END_SIBLING_EVENT
    CALL(_disable_enemy_unit_disp)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetUnitId, -4, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetPos, LW(3), -230, 0, 90)
            CALL(btlevtcmd_ClearAllStatus, LW(3))
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 40)
            SET_VALUE(LW(0), -155)
            SET_VALUE(LW(1), 0)
            SET_VALUE(LW(2), 90)
            SET_VALUE(LW(6), 28)
            CALL(btlevtcmd_GetUnitKind, LW(3), LW(5))
            SWITCH(LW(5))
                CASE_OR_EQUAL(224)
                CASE_OR_EQUAL(225)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 6, 12, 16, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_END
                CASE_EQUAL(226)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 6, 6, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(227)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 6, 12, 16, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(228)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 0, 15, 13, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(229)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 18, 18, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
                CASE_EQUAL(230)
                CALL(btlevtcmd_SetMoveSoundDataWork, LW(3), STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 15, 13, 0, 0)
                CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), LW(6), -2, 1)
            END_SWITCH
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 43)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetPos, -3, -200, 0, 90)
    CALL(btlevtcmd_ClearAllStatus, -3)
    CALL(btlevtcmd_AnimeChangePoseType, -3, 1, 40)
    CALL(btlevtcmd_MovePosition, -3, 0, 0, 90, 75, -1, 0)
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_PAUSE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_V_2"))
    CALL(evt_audience_sound_cheer, 180, 40)
    WAIT_MS(1000)
    CALL(_lvup_select_object_init)
    BEGIN_SIBLING_EVENT
        CALL(_lvup_spot_off, 30)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_batstage_set_stg_dark, 0, 30, 0)
        CALL(evt_batstage_set_stg_dark_base, 0, 30, 0)
        CALL(btlevtcmd_BtlStageObjFlagOnOff, STRING("spotlight"), 1, 0)
        CALL(_lvup_obj_set_color, 0, 100, 128, 128, 255)
        CALL(_lvup_obj_set_color, 1, 100, 128, 128, 255)
        CALL(_lvup_obj_set_color, 2, 100, 128, 128, 255)
        CALL(evt_batstage_set_aud_dark, 0, 60, 0)
        CALL(evt_fade_entry, 25, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
    END_SIBLING_EVENT
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(_lvup_obj_set_color, 1, 255, 255, 255, 255)
        CALL(_lvup_disp_flag_onoff, 1, 32)
        CALL(_lvup_select_obj_disp_on)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    CALL(_lvup_select_obj_lvup_param_disp)
    RETURN
EVT_END()
#endif

EVT_BEGIN(_confetti_control_event)
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(5), 5)
    LOOP(LW(5))
        SUBTRACT(LW(0), 1)
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(0), 20)
            CALL(evt_eff, STRING(""), STRING("confetti"), 3, 0, 300, 5, 180, 0, 0, 0, 0, 0, 0, 0)
        END_IF
        WAIT_FRAMES(1)
        SET_VALUE(LW(5), 5)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(_jump_event)
    BEGIN_SIBLING_EVENT
        CALL(_lvup_object_noselect_delete)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_LVUP_BLOCK_MOVE1"), 0, 125, 0, 0)
    CALL(_lvup_object_select_move)
    CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -3, 1, 44)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(10)
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_LVUP_BLOCK_HIT1"), 0, 125, 0, 0)
        CALL(evt_btl_camera_shake_h, 0, 6, 0, 20, 13)
        CALL(_lvup_object_decide_bound)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetFallAccel, -3, FLOAT(0.39941406f))
    CALL(btlevtcmd_JumpPosition, -3, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_PAUSE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_V_2"))
    CALL(evt_audience_sound_cheer, 180, 40)
    RETURN
EVT_END()

EVT_BEGIN(_move_to_heart_event)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_LVUP_CURSOR1"), 0, 125, 0, 0)
    CALL(_lvup_obj_set_color, 0, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 1, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 2, 100, 128, 128, 255)
    CALL(_lvup_disp_flag_onoff, 0, 32)
    CALL(evt_sub_intpl_init, 5, 0, -30, 10)
    LOOP(11)
        CALL(evt_sub_intpl_get_value)
        CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, -30)
    CALL(_lvup_obj_set_color, 0, 255, 255, 255, 255)
    CALL(_lvup_disp_flag_onoff, 1, 32)
    RETURN
EVT_END()

EVT_BEGIN(_move_to_flower_event)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_LVUP_CURSOR1"), 0, 125, 0, 0)
    CALL(_lvup_obj_set_color, 0, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 1, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 2, 100, 128, 128, 255)
    CALL(_lvup_disp_flag_onoff, 0, 32)
    CALL(btlevtcmd_BtlStageObjGetRotate, STRING("spotlight"), EVT_NULLPTR, EVT_NULLPTR, LW(5))
    CALL(evt_sub_intpl_init, 5, LW(5), 0, 10)
    LOOP(11)
        CALL(evt_sub_intpl_get_value)
        CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, 0)
    CALL(_lvup_obj_set_color, 1, 255, 255, 255, 255)
    CALL(_lvup_disp_flag_onoff, 1, 32)
    RETURN
EVT_END()

EVT_BEGIN(_move_to_badge_point_event)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_LVUP_CURSOR1"), 0, 125, 0, 0)
    CALL(_lvup_obj_set_color, 0, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 1, 100, 128, 128, 255)
    CALL(_lvup_obj_set_color, 2, 100, 128, 128, 255)
    CALL(_lvup_disp_flag_onoff, 0, 32)
    CALL(evt_sub_intpl_init, 5, 0, 30, 10)
    LOOP(11)
        CALL(evt_sub_intpl_get_value)
        CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_BtlStageObjSetRotate, STRING("spotlight"), 0, 0, 30)
    CALL(_lvup_obj_set_color, 2, 255, 255, 255, 255)
    CALL(_lvup_disp_flag_onoff, 1, 32)
    RETURN
EVT_END()

EVT_BEGIN(_koura_delete_event)
    CALL(_check_guard_koura, -3, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 62)
    END_IF
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
USER_FUNC(N__set_initial_upgrade_cursor_position) {
    SEW(_battleWorkPointer)->selected = evtGetValue(event, event->args[0]);
    return 2;
}

void N__draw_select_one_to_upgrade_text(CameraId camId, void* param) {
    const char* msg;
    u16 lines;
    s32 width;
    f32 y;
    f32 zero = 0.0f;

    msg = msgSearch("select_HPFPBP");
    width = FontGetMessageWidthLine(msg, &lines);
    lines++;
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, (zero - width / 2) - 20.0f, -145.0f + (lines - 1) * 29, width + 40, lines * 29 + 1, 10.0f);
    FontDrawStart_alpha(0xFF);
    y = -148.0f;
    FontDrawMessage(zero - width / 2, (s32)y + (lines - 1) * 29, msg);
}

USER_FUNC(N__display_select_one_to_upgrade_text) {
    dispEntry(CAMERA_2D, 900.0f, 1, N__draw_select_one_to_upgrade_text, _battleWorkPointer);
    return 2;
}
#endif

USER_FUNC(_nozzle_set_offset) {
    EvtWord* args = event->args;
    const char* name;
    f32 ofs;
    BattleStageObject* obj;

    name = (const char*)evtGetValue(event, args[0]);
    ofs = evtGetFloat(event, args[1]);
    obj = BattleSearchObjectPtr(name);
    if (obj) {
        obj->positionOffset.y = ofs;
    }
    return 2;
}

USER_FUNC(_lvup_ap_recover_full) {
    BattleAudience* aud = BattleAudienceBaseGetPtr();

    aud->excited = TRUE;
    pouchSetAP(pouchGetMaxAP());
    return 2;
}

USER_FUNC(_disable_enemy_unit_disp) {
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleUnit* unit;
    BattleStageObject* obj;
    void* data;
    EventEntry* evt;
    s32 kind;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (!unit) continue;
        if (unit->flags & 0x80000000) continue;
        kind = unit->currentKind;
        if (kind == UNIT_SYSTEM) continue;
        if (kind == UNIT_MARIO) continue;
        if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX && unit->mAlliance == ALLIANCE_PLAYER && kind != UNIT_BOBBERY) continue;
        data = BtlUnit_GetData(unit, 0x42);
        if (data) {
            evt = evtEntry(data, 10, 0);
            evt->unitId = unit->unitId;
        } else if (unit->currentKind != UNIT_BOBBERY) {
            unit->mAttributeFlags |= 0x21000000;
        }
    }
    obj = BattleSearchObjectPtr("if_body");
    if (obj) {
        wp->stageObjectWork.flags |= 0x20;
        obj->mFlags |= 1;
        mapGrpFlagOn("if_body", 1);
        mapGrpFlagOn("if_wire", 1);
    }
    return 2;
}

USER_FUNC(_set_confetti_control_evtid) {
    EvtWord* args = event->args;
    SeqEndWork* work = SEW(_battleWorkPointer);
    if (!_battleWorkPointer->seqEndWork && !_battleWorkPointer->seqEndWork && !_battleWorkPointer->seqEndWork) {
    }
    work->confettiEvtId = evtGetValue(event, args[0]);

    return 2;
}

USER_FUNC(_lvup_select_obj_disp_on) {
    SeqEndWork* work = SEW(_battleWorkPointer);

    work->flags |= 8;
    return 2;
}

USER_FUNC(_lvup_select_obj_lvup_param_disp) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    s32 i;

    for (i = 0; i < 3; i++) {
        work->objs[i].paramDisp = 1;
    }
    return 2;
}

USER_FUNC(_lvup_obj_set_color) {
    EvtWord* args = event->args;
    SeqEndWork* work = SEW(_battleWorkPointer);
    SeqEndObj* obj;

    obj = &work->objs[evtGetValue(event, args[0])];
    obj->color.r = evtGetValue(event, args[1]);
    obj->color.g = evtGetValue(event, args[2]);
    obj->color.b = evtGetValue(event, args[3]);
    obj->color.a = evtGetValue(event, args[4]);
    return 2;
}

USER_FUNC(_lvup_disp_flag_onoff) {
    EvtWord* args = event->args;
    SeqEndWork* work = SEW(_battleWorkPointer);
    s32 on = evtGetValue(event, args[0]);
    u32 mask = args[1];

    if (on) {
        work->flags |= mask;
    } else {
        work->flags &= ~mask;
    }
    return 2;
}

USER_FUNC(_lvup_spot_on) {
    BattleSpotWork* spot = BatSpotGetPtr();
    EvtWord* args = event->args;
    s32 n;
    SeqEndSpot* e;

    spot->skipRender = 1;
    n = evtGetValue(event, args[0]);
    e = (SeqEndSpot*)spot->entries + n;
    switch (n) {
    case 0: {
        Vec v = {0.0f, 25.0f, -50.0f};
        v.x = 25.0f * n + -50.0f;
        e->pos = v;
        break;
    }
    case 1: {
        Vec v = {0.0f, -10.0f, -50.0f};
        v.x = 25.0f * n + -50.0f;
        e->pos = v;
        break;
    }
    case 2: {
        Vec v = {0.0f, 25.0f, -50.0f};
        v.x = 25.0f * n + -50.0f;
        e->pos = v;
        break;
    }
    case 3:
        {
            Vec v = {0.0f, -10.0f, -50.0f};
            v.x = 25.0f * n + -50.0f;
            e->pos = v;
            break;
        }
    case 4: {
        Vec v = {0.0f, 25.0f, -50.0f};
        v.x = 25.0f * n + -50.0f;
        e->pos = v;
        break;
    }
    }
    e->rot = (Vec){0.0f, 0.0f, 0.0f};
    e->scale = (Vec){15.0f, 15.0f, 150.0f};
    e->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    e->flags |= EVENT_STOPPED;
    return 2;
}


USER_FUNC(_lvup_spot_move) {
    BattleSpotWork* spot = BatSpotGetPtr();
    s32 n = evtGetValue(event, event->args[0]);
    SeqEndSpot* e = (SeqEndSpot*)spot->entries + n;
    f32 s;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        event->userdata[2] = e->pos.x;
        event->userdata[3] = e->pos.y;
        event->userdata[4] = 0;
    }
    event->userdata[0]++;
    switch (event->userdata[1]) {
    case 0:
        event->userdata[0] = 1;
        event->userdata[1]++;
    case 1:
        switch (n) {
        case 0:
            e->pos.x = 0.5f * BattleGetMarioPtr(_battleWorkPointer)->mPosition.x;
            e->pos.y = -20.0f;
            break;
        case 1:
            e->pos.x = 50.0f * sinfd(reviseAngle(1.1f * (3.0f * event->userdata[0]))) + event->userdata[2];
            e->pos.y = -(25.0f * sinfd(reviseAngle(1.1f * (2.5f * event->userdata[0]) + 90.0f)) - (event->userdata[3] + 25));
            break;
        case 2:
            e->pos.x = 65.0f * sinfd(reviseAngle(1.1f * (2.5f * event->userdata[0]) + 180.0f)) + event->userdata[2];
            e->pos.y = 25.0f * sinfd(reviseAngle(1.1f * (7.5f * event->userdata[0]) + 90.0f)) + (event->userdata[3] - 25);
            break;
        case 3:
            e->pos.x = -(50.0f * sinfd(reviseAngle(1.1f * (4.0f * event->userdata[0]))) - event->userdata[2]);
            e->pos.y = -(25.0f * sinfd(reviseAngle(1.1f * (2.5f * event->userdata[0]) + 90.0f)) - (event->userdata[3] + 25));
            break;
        case 4:
            e->pos.x = -(50.0f * sinfd(reviseAngle(1.1f * (3.0f * event->userdata[0]) + 270.0f)) - (event->userdata[2] - 50));
            e->pos.y = 25.0f * sinfd(reviseAngle(1.1f * (4.5f * event->userdata[0]) + 90.0f)) + (event->userdata[3] - 25);
            break;
        }
        break;
    case 2:
        event->userdata[4] = 20;
        event->userdata[2] = e->pos.x;
        event->userdata[3] = e->pos.y;
        event->userdata[1]++;
    case 3:
        event->userdata[4]--;
        e->pos.x = intplGetValue(2, event->userdata[4], 21, 0.0f, event->userdata[2]);
        e->pos.y = intplGetValue(2, event->userdata[4], 21, -25.0f, event->userdata[3]);
        {
            GXColor c = {0xFF, 0xFF, 0xFF, 0x00};
            c.a = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(255.0f, 128.0f, 0, event->userdata[4], 21);
            e->color = c;
        }
        if (event->userdata[4] <= 0) {
            event->userdata[1]++;
        }
        break;
    }
    return event->userdata[1] < 4 ? 0 : 2;
}

USER_FUNC(_lvup_spot_off) {
    BattleSpotWork* spot = BatSpotGetPtr();
    EvtWord* args = event->args;
    SeqEndSpot* e;
    s32 i;

    if (isFirstCall) {
        event->userdata[0] = evtGetValue(event, args[0]);
        event->userdata[1] = event->userdata[0];
        event->userdata[2] = ((SeqEndSpot*)spot->entries)->color.a;
    }
    event->userdata[1]--;
    for (i = 0; i < 5; i++) {
        GXColor c = {0xFF, 0xFF, 0xFF, 0x00};
        e = (SeqEndSpot*)spot->entries + i;
        c.a = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, event->userdata[2], 2, event->userdata[1], event->userdata[0]);

        e->color = c;
        if (e->color.a == 0) {
            e->flags &= ~EVENT_STOPPED;
        }
    }
    return event->userdata[1] > 0 ? 0 : 2;
}

USER_FUNC(_lvup_object_noselect_delete) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    s32 sel = work->selected;
    s32 i;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[2] = 30;
        for (i = 0; i < work->objCount; i++) {
            if (i != sel) {
                work->objs[i].paramDisp = 3;
            }
        }
    }
    event->userdata[0]++;
    if (event->userdata[0] < event->userdata[2]) {
        for (i = 0; i < work->objCount; i++) {
            if (i != sel) {
                work->objs[i].color.a = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(255.0f, 0.0f, 5, event->userdata[0], event->userdata[2]);

            }
        }
        return 0;
    }
    for (i = 0; i < work->objCount; i++) {
        if (i != sel) {
            work->objs[i].color.a = 0;
        }
    }
    return 2;
}


USER_FUNC(_lvup_object_select_move) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    s32 sel = work->selected;
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    SeqEndObj* obj = &work->objs[sel];

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 60;
        event->userdata[2] = obj->pos.x;
        event->userdata[3] = obj->pos.z;
        event->userdata[4] = mario->mPosition.x;
        event->userdata[5] = mario->mPosition.z;
    }
    event->userdata[0]++;
    if (event->userdata[0] < event->userdata[1]) {
        obj->pos.x = intplGetValue(5, event->userdata[0], event->userdata[1], event->userdata[2], event->userdata[4]);
        obj->pos.z = intplGetValue(5, event->userdata[0], event->userdata[1], event->userdata[3], event->userdata[5]);
        obj->rot.y = reviseAngle(((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, 2160.0f, 5, event->userdata[0], event->userdata[1]));
        return 0;
    }
    obj->pos.x = event->userdata[4];
    obj->pos.z = event->userdata[5];
    return 2;
}

USER_FUNC(_lvup_object_decide_bound) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    SeqEndObj* obj = &work->objs[work->selected];
    s32 half;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = obj->pos.y;
        event->userdata[2] = 16;
        event->userdata[3] = 30;
        obj->paramDisp = 2;
    }
    event->userdata[0]++;
    if (event->userdata[0] < event->userdata[2]) {
        half = event->userdata[2] / 2;
        if (event->userdata[0] < half) {
            obj->pos.y = event->userdata[1] + intplGetValue(5, event->userdata[0], half, 0.0f, event->userdata[3]);
        } else {
            obj->pos.y = event->userdata[1] + intplGetValue(5, event->userdata[0] - half, half, event->userdata[3], 0.0f);
        }
        return 0;
    }
    obj->pos.y = event->userdata[1];
    obj->paramDisp = 4;
    effPokopiPchargeN64Entry(3, 120, obj->pos.x, obj->pos.y, obj->pos.z, 1.5f);
    return 2;
}

USER_FUNC(_lvup_select_object_init) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    PouchData* pouch = pouchGetPtr();

    work->objs[0].pos = (Vec){-80.0f, 105.0f, 0.0f};
    work->objs[0].scale = (Vec){0.5f, 0.5f, 1.0f};
    work->objs[1].pos = (Vec){0.0f, 105.0f, 0.0f};
    work->objs[1].scale = (Vec){0.5f, 0.5f, 1.0f};
    work->objs[2].pos = (Vec){80.0f, 105.0f, 0.0f};
    work->objs[2].scale = (Vec){0.5f, 0.5f, 1.0f};

    work->objs[0].paramDisp = 0;
    work->objs[0].color = (GXColor){0xC8, 0xFF, 0xFF, 0xFF};
    work->objs[0].value = pouch->baseMaxHP;
    work->objs[0].rot = (Vec){0.0f, 0.0f, 0.0f};
    if (work->objs[0].value >= 200) {
        work->objs[0].nextValue = work->objs[0].value;
    } else {
        work->objs[0].nextValue = work->objs[0].value + 5;
    }

    work->objs[1].paramDisp = 0;
    work->objs[1].color = (GXColor){0xC8, 0xFF, 0xFF, 0xFF};
    work->objs[1].value = pouch->baseMaxFP;
    work->objs[1].rot = (Vec){0.0f, 0.0f, 0.0f};
    if (work->objs[1].value >= 200) {
        work->objs[1].nextValue = work->objs[1].value;
    } else {
        work->objs[1].nextValue = work->objs[1].value + 5;
    }

    work->objs[2].paramDisp = 0;
    work->objs[2].color = (GXColor){0xC8, 0xFF, 0xFF, 0xFF};
    work->objs[2].value = pouch->maximumBP;
    work->objs[2].rot = (Vec){0.0f, 0.0f, 0.0f};
    if (work->objs[2].value >= 99) {
        work->objs[2].nextValue = work->objs[2].value;
    } else {
        work->objs[2].nextValue = work->objs[2].value + 3;
    }

    work->flags |= 8;
    return 2;
}

s32 _keta_get(s32 n) {

    s32 k = 0;

    if (n < 0) {
        n *= -1;
    }
    if (n == 0) {
        return 1;
    }
    while (n > 0) {
        n /= 10;
        k++;
    }
    return k;
}

#ifdef VERSION_G8ME01
#define LVOBJ_TEX2 0x31
#define LVOBJ_ICON 0x16B
// FAKE: same registers as (id, color, 0, x, y, z, sx, sy); listing the float arguments
// first makes MWCC evaluate the texture-table load last, as in the US build.
#define LVOBJ_TEXPLANE(o, n, tbl)                                                   \
    ((void (*)(f32, f32, f32, f32, f32, u32, GXColor, s32))btlDispTexPlane)(        \
        (o)->pos.x - 16.0f, 20.0f + (o)->pos.y, 2.0f + (o)->pos.z,                  \
        (o)->scale.x, (o)->scale.y, tbl[gp->language], (o)->color, 0)
#else
#define LVOBJ_TEX2 0x24
#define LVOBJ_ICON 0x163
#define LVOBJ_TEXPLANE(o, n, tbl)                                                   \
    ((void (*)(u32, GXColor, s32, f32, f32, f32, f32, f32))btlDispTexPlane)(        \
        0x27 + n, (o)->color, 0, (o)->pos.x - 16.0f, 20.0f + (o)->pos.y,             \
        2.0f + (o)->pos.z, (o)->scale.x, (o)->scale.y)
#endif
#define LVOBJ_PLANE(o, n, tbl)                                                            \
    if ((o)->blinkOn) {                                                             \
        PSMTXIdentity(m);                                                              \
        PSMTXTrans(trans, (o)->pos.x, (o)->pos.y, (o)->pos.z);                         \
        PSMTXScale(scale, (o)->scale.x, (o)->scale.y, (o)->scale.z);                   \
        PSMTXRotRad(rotX, 'x', 0.017453292f * (o)->rot.x);                             \
        PSMTXRotRad(rotY, 'y', 0.017453292f * (o)->rot.y);                             \
        PSMTXRotRad(rotZ, 'z', 0.017453292f * (o)->rot.z);                             \
        PSMTXConcat(rotZ, scale, m);                                                   \
        PSMTXConcat(rotZ, m, m);                                                       \
        PSMTXConcat(rotX, m, m);                                                       \
        PSMTXConcat(rotY, m, m);                                                       \
        PSMTXConcat(trans, m, m);                                                      \
        ((void (*)(Mtx, u32, GXColor, s32))btlDispTexPlane2)(m, LVOBJ_TEX2 + n, (o)->color, 0); \
        LVOBJ_TEXPLANE(o, n, tbl);                              \
    }

void _lvup_select_object_disp(CameraId camId, void* param) {
    SeqEndWork* work = SEW(_battleWorkPointer);
    SeqEndObj* obj;
    s32 i;
    Vec pos3;
    Vec pos;
    Vec pos2;
    Mtx m, trans, scale, rotX, rotY, rotZ;
    Mtx m1, m2, m3, ms;

    for (i = 0; i < 3; i++) {
        obj = &work->objs[i];
        switch (obj->paramDisp) {
        case 4:
            obj->paramDisp = 5;
            obj->blinkTimer = 1;
        case 5:
            obj->blinkTimer--;
            if (obj->blinkTimer == 0) {
                obj->blinkTimer = 8;
                if (obj->blinkOn) {
                    obj->blinkOn = 0;
                } else {
                    obj->blinkOn = 1;
                }
            }
            break;
        default:
            obj->blinkOn = 1;
            break;
        }
    }
    LVOBJ_PLANE(&work->objs[0], 0, hp_tex_tbl)
    LVOBJ_PLANE(&work->objs[1], 1, fp_tex_tbl)
    LVOBJ_PLANE(&work->objs[2], 2, bp_tex_tbl)

    for (i = 0; i < 3; i++) {
        obj = &work->objs[i];
        if (obj->blinkOn) {
            if (obj->paramDisp == 1) {
                if (obj->value != obj->nextValue) {
                    pos = obj->pos;
                    pos.x = pos.x - 8.0f - 6.0f;
                    pos.y = pos.y - (96.0f * obj->scale.y + 2.0f);
                    pos.z += 5.0f;
                    PSMTXScale(ms, 0.8f, 0.8f, 1.0f);
                    PSMTXTrans(m1, pos.x, pos.y, pos.z);
                    PSMTXConcat(m1, ms, m1);
                    iconNumberDispGx3D(m1, obj->value, 0, obj->color);

                    pos2 = obj->pos;
                    pos2.x = 1.2f * (_keta_get(obj->nextValue) * 8 + 6) + pos2.x;
                    pos2.y = pos2.y - (96.0f * obj->scale.y + 3.0f);
                    pos2.z += 5.0f;
                    PSMTXTrans(m2, pos2.x, pos2.y, pos2.z);
                    PSMTXScale(ms, 1.2f, 1.2f, 1.0f);
                    PSMTXConcat(m2, ms, m2);
                    iconNumberDispGx3D(m2, obj->nextValue, 0, obj->color);

                    pos3 = obj->pos;
                    pos3.y = pos3.y - 96.0f * obj->scale.y;
                    pos3.z += 5.0f;
                    PSMTXTrans(m3, pos3.x, pos3.y, pos3.z);
                    PSMTXScale(ms, -0.6f, 0.6f, 1.0f);
                    PSMTXConcat(m3, ms, m3);
                    iconDispGxCol(m3, 0, LVOBJ_ICON, obj->color);
                }
            } else if (obj->paramDisp == 2 || obj->paramDisp == 5) {
                pos2 = obj->pos;
                pos2.x = pos2.x + ((_keta_get(obj->nextValue) * 8) / 2 - 4.0f);
                pos2.y = pos2.y - (96.0f * obj->scale.y + 2.0f);
                pos2.z += 5.0f;
                PSMTXTrans(m2, pos2.x, pos2.y, pos2.z);
                iconNumberDispGx3D(m2, obj->nextValue, 0, obj->color);
            }
        }
    }
}
void _check_audience_battle_end_exec(void) {
    BattleWork* wp = _battleWorkPointer;
    SeqEndGroup* group = (SeqEndGroup*)wp->fbatInfo->setup->groupSetup;
    s32 level = group->units[0].kind[0xE];
    s32 turn;
    s32 sum;
    s32 i;
    s32 lv;
    s32 limit;
    SeqEndUnitSetup* unit;

    if (level > pouchGetPtr()->level + 1) {
        BattleAudience_Case_GreatVictory();
    }
    turn = wp->turnCount;
    lv = pouchGetPtr()->level + 1;
    sum = 0;
    unit = group->units;
    for (i = 0; i < group->count; i++, unit++) {
        sum += unit->kind[0xE];
    }
    limit = sum * 2 / lv / 3;
    if (turn < limit) {
        BattleAudience_Case_FastVictory(limit - turn);
    }
}

void _audience_Whistle_control(void) {
    SeqEndWork* work = SEW(_battleWorkPointer);

    if (work) {
        switch (work->state) {
        case 0x7000007:
        case 0x7000008:
        case 0x7000009:
        case 0x700000A:
        case 0x700000B:
        case 0x700000C:
        case 0x700000D:
        case 0x700000E:
        case 0x700000F:
        case 0x7000010:
        case 0x7000011:
        case 0x7000012:
        case 0x7000013:
        case 0x7000014:
        case 0x7000015:
        case 0x7000016:
        case 0x7000017:
        case 0x7000018:
        case 0x7000019:
        case 0x700001A:
        case 0x700001B:
            if (--work->whistleTimer < 0) {
                work->whistleTimer = irand(120) + 60;
                BattleAudienceSoundWhistle();
            }
            break;
        }
    }
}

void _BattleMarioKouraDelete(void) {
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    EventEntry* evt = evtEntry(_koura_delete_event, 10, 0);

    evt->unitId = mario->unitId;
}

USER_FUNC(_check_guard_koura) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnit* koura;
    s32 out;
    s32 id;

    id = evtGetValue(event, args[0]);
    out = args[1];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (!(unit->mAttributeFlags & kShellShielded)) {
        evtSetValue(event, out, -1);
        return 2;
    }
    koura = BtlUnit_GetGuardKouraPtr(unit);
    if (!koura) {
        evtSetValue(event, out, -1);
        return 2;
    }
    evtSetValue(event, out, koura->unitId);
    return 2;
}

USER_FUNC(_get_rank_up_msg) {
    s32 index = event->args[0];

    evtSetValue(event, index, (EvtWord)_get_rank_data(pouchGetPtr()->level)->rankUpMessage);
    return 2;
}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
