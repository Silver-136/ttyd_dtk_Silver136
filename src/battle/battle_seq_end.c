#include "battle/battle_seq_end.h"

#include "manager/evtmgr.h"

__declspec(section ".rodata") const char str_levelup_802fe808[8] = "levelup";

extern void* _battleWorkPointer;
extern s32 evtGetValue();
extern void dispEntry(s32, s32, void*, void*, f32);
extern void N__draw_select_one_to_upgrade_text();
extern void* BattleAudienceBaseGetPtr();
extern s32 pouchGetMaxAP();
extern void pouchSetAP();
extern void windowDispGX_Waku_col(s32, void*, f32, f32, f32, f32, f32);
extern u32 dat_80427158;
extern void* BattleGetMarioPtr();
extern void* evtEntry();
extern void _koura_delete_event();
extern void* pouchGetPtr();
extern void evtSetValue();
extern f32 evtGetFloat();
extern void* BattleSearchObjectPtr();
extern void iconDelete();
extern s32 irand();
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BtlUnit_GetData(void* unit, s32 dataId);
extern void BattleAudienceSoundWhistle(void);

typedef struct RankUpData {
    s16 level;
    s16 field_0x2;
    const char* name;
    const char* message;
} RankUpData;

extern RankUpData _rank_up_data[];

RankUpData* _get_rank_data(s32 level) {
    RankUpData* data = _rank_up_data;

    goto compare;
loop:
    if (data->message == NULL) {
        return NULL;
    }
    data++;
compare:
    if (level != data->level) {
        goto loop;
    }
    return data;
}

const char* BattleGetRankNameLabel(s32 level) {
    RankUpData* data = _rank_up_data;
    RankUpData* best = NULL;

    while (level >= data->level) {
        if (data->level < 0) {
            break;
        }
        best = data;
        data++;
    }
    if (best == NULL) {
        return NULL;
    }
    return best->name;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: btlseqEnd | missing_definition | ghidra_signature */
void btlseqEnd(void* battleWork) {
    extern void _audience_Whistle_control(void);
    extern s32 BattleGetSeq(void*, s32);
    extern void BattleSetSeq(void*, s32, s32);
    extern void btlsubResetMoveColorLvAll(void*);
    extern void* BattleAlloc(s32);
    extern void btlseqEnd_DispInit(void*);
    extern void btlseqEnd_DispMain(s32, void*);
    extern s32 irand(s32);
    extern void _ExecAllUnitBattleEndEvent(void);
    extern void _check_audience_battle_end_exec(void);
    extern void BattleAudienceSoundSetVolAll(s32, s32);
    extern void BattleInformationSetResult(void*, s32);
    extern s32 BattleInformationGetResult(void*);
    extern void btl_camera_set_moveSpeedLv(s32, s32);
    extern void btl_camera_set_mode(s32, s32);
    extern void btl_camera_set_posoffset(f64, f64, f64, s32);
    extern void fadeEntry(s32, s32, void*);
    extern void psndBGMOff(s32);
    extern s32 fadeIsFinish(void);
    extern void seqSetSeq(s32, void*, void*);
    extern void* fbatGetPointer(void);
    extern void* evtEntry(void*, s32, s32);
    extern s32 evtCheckID(s32);
    extern void evtDeleteID(s32);
    extern s32 BattleWaitAllActiveEvtEnd(void*);
    extern void _BattleMarioKouraDelete(void);
    extern void* BattleGetPartyPtr(void*);
    extern s32 BtlUnit_GetEnemyBelong(void*);
    extern u32 BtlUnit_CheckStatusFlag(void*, u32);
    extern void BtlUnit_OffStatusFlag(void*, s32);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void BattleConsumeReserveItem(void);
    extern void BtlUnit_SetStatus(void*, s32, s32, s32);
    extern void BattleInformationSetDropMaterial(void*);
    extern void BattleMajinaiDone(void);
    extern void BattleMajinaiEndCheck(void);
    extern u32 BtlUnit_CheckStatus(void*, s32);
    extern void BattleFogForceStop(void);
    extern s32 BattleGetStockExp(void*);
    extern void _GetExpIcon_Init(void*);
    extern void BattleAudienceSoundCheer(s32, s32);
    extern void BattleAudienceSoundClap(s32, s32);
    extern u32 BattlePadCheckNow(u32);
    extern void* effPointGetEntry(f64, f64, f64, s32, s32, s32);
    extern void psndSFXOn(void*);
    extern void _GetExpIcon_End(void*);
    extern void effSoftDelete(void*);
    extern void BattleAudience_WinSetActive(s32);
    extern void BattleBreakSlot_HideReel(void);
    extern void BattleStatusWindowSystemOff(void);
    extern void BattleStatusWindowSystemOn(void);
    extern void BattleStatusWindowEventOn(void);
    extern void psndBGMOn(s32, void*);
    extern u32 BattlePadCheckTrigger(u32);
    extern void pouchReviseMarioParam(void);
    extern void BtlUnit_SetParamFromPouch(void*);
    extern s32 BattleTransPartyId(s32);
    extern void BtlUnit_snd_se(void*, const char*, s32, s16);
    extern void BtlUnit_SetBodyAnim(void*, const char*);
    extern void effRecoveryEntry(f64, f64, f64, s32, s32);
    extern void BtlUnit_SetBodyAnimType(void*, s32);
    extern void BattleSetMarioParamToFieldBattle(void*);
    extern void BattleFree(void*);
    extern void* marioGetPtr(void);
    extern s32 marioPartyEntry(s32);
    extern void marioPartyKill(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void BtlActRec_JudgeRuleKeep(void);
    extern void BattleAudienceSoundStop(s32);

    extern s32 _curtain_close_event[];
    extern s32 _move_to_flower_event[];
    extern s32 _move_to_heart_event[];
    extern s32 _move_to_badge_point_event[];
    extern s32 _jump_event[];
    extern s32 _rank_up_event[];
    extern s32 _event_MajinaiEnd[];
    extern s32 marioAttackEvent_MajinaiExpUp[];
    extern u8 pose_table_nokotarou_stay[];

    extern char str_BGM_BATTLE_WIN1_802fea88[];
    extern char str_BGM_BATTLE_WIN2_802fea98[];
    extern char str_BGM_BATTLE_WIN3_802feabc[];
    extern char str_SFX_STAR_POINT_GET1_802feaa8[];
    extern char str_SFX_VOICE_MARIO_PAUS_802feae4[];
    extern char str_SFX_VOICE_MARIO_PAUS_802feacc[];
    extern char str_SFX_VOICE_MARIO_RELI_802feb14[];
    extern char str_SFX_VOICE_MARIO_RELI_802feafc[];
    extern char str_M_V_1_80427298[];
    extern char str_M_V_2_80427160[];

    extern u32 dat_80427140;
    extern u32 dat_80427144;
    extern f32 float_0_804271c0;
    extern f32 float_90_804271e4;
    extern f32 float_40_80427294;

    u8* bw = (u8*)battleWork;
    u8* work = *(u8**)(bw + 0xF28);
    u8* info = *(u8**)(bw + 0x2738);
    u8* pouch = (u8*)pouchGetPtr();
    u8* fbat;
    u8* evt;
    u8* mario;
    u8* party;
    u8* part;
    u8* npcInfo;
    u8* npc;
    u8* player;
    u8* fieldParty;
    RankUpData* rank;
    s32 state;
    s32 result;
    s32 alliance;
    s32 enemyAlliance;
    s32 status;
    s32 partyId;
    s32 unitKind;
    s32 valid;
    u32 flags;
    u32 fadeColor;

    _audience_Whistle_control();
    state = BattleGetSeq(battleWork, 7);

    switch (state) {
        case 0x07000000:
            btlsubResetMoveColorLvAll(battleWork);
            mario = (u8*)BattleGetMarioPtr(battleWork);
            alliance = *(s8*)(mario + 0xC);
            enemyAlliance = BtlUnit_GetEnemyBelong(mario);
            flags = *(u32*)(bw + 0xEF4);

            if ((flags & 0x40) != 0) {
                BattleInformationSetResult(info, 4);
            } else if ((flags & 0x20) != 0) {
                BattleInformationSetResult(info, 5);
            } else if (*(s32*)(bw + 0xC + alliance * 8) != 0) {
                BattleInformationSetResult(info, 3);
            } else if (*(s32*)(bw + 0xC + enemyAlliance * 8) != 0) {
                BattleInformationSetResult(info, 1);
            } else if ((flags & 0x10) != 0) {
                BattleInformationSetResult(info, 4);
            }

            result = BattleInformationGetResult(info);
            if (result == 3) {
                BattleSetSeq(battleWork, 7, 0x07000001);
            } else if (result < 3 && result == 1) {
                fbat = (u8*)fbatGetPointer();
                if (*(void**)(fbat + 0x3C) != 0) {
                    BattleSetSeq(battleWork, 7, 0x07000003);
                    return;
                }
                _check_audience_battle_end_exec();
                BattleSetSeq(battleWork, 7, 0x07000005);
                _BattleMarioKouraDelete();
            } else {
                _ExecAllUnitBattleEndEvent();
                BattleSetSeq(battleWork, 7, 0x0700001D);
                BattleAudienceSoundSetVolAll(0, 30);
            }

            work = (u8*)BattleAlloc(0x2AC);
            *(u8**)(bw + 0xF28) = work;
            btlseqEnd_DispInit(battleWork);
            *(s32*)(work + 0x0) = 0;
            *(void**)(work + 0x29C) = 0;
            *(s32*)(work + 0x2A0) = irand(120) + 60;
            *(s32*)(work + 0x2A4) = 0;
            break;

        case 0x07000001:
            _ExecAllUnitBattleEndEvent();
            btl_camera_set_moveSpeedLv(0, 2);
            btl_camera_set_mode(0, 0x13);
            btl_camera_set_posoffset(
                (f64)float_0_804271c0,
                (f64)float_0_804271c0,
                (f64)float_0_804271c0,
                0);
            fadeColor = dat_80427140;
            fadeEntry(10, 200, &fadeColor);
            psndBGMOff(0x201);
            BattleSetSeq(battleWork, 7, 0x07000002);
            break;

        case 0x07000002:
            if (fadeIsFinish() != 0) {
                npcInfo = *(u8**)(info + 0xC);
                if ((*(u32*)(npcInfo + 4) & 0x40) == 0) {
                    seqSetSeq(5, (void*)1, 0);
                } else {
                    BattleSetSeq(battleWork, 7, 0x0700001F);
                }
            }
            break;

        case 0x07000003:
            fbat = (u8*)fbatGetPointer();
            evt = (u8*)evtEntry(*(void**)(fbat + 0x3C), 10, 0);
            *(s32*)(fbat + 0x40) = *(s32*)(evt + 0x15C);
            BattleSetSeq(battleWork, 7, 0x07000004);
            return;

        case 0x07000004:
            fbat = (u8*)fbatGetPointer();
            if (!evtCheckID(*(s32*)(fbat + 0x40)) &&
                BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                if (*(s32*)(fbat + 0x44) != 0) {
                    BattleSetSeq(battleWork, 3, 0x03000001);
                    BattleSetSeq(battleWork, 0, 2);
                    return;
                }
                BattleSetSeq(battleWork, 7, 0x07000005);
                _BattleMarioKouraDelete();
                work = (u8*)BattleAlloc(0x2AC);
                *(u8**)(bw + 0xF28) = work;
                btlseqEnd_DispInit(battleWork);
                *(s32*)(work + 0x0) = 0;
                return;
            }
            break;

        case 0x07000005:
            mario = (u8*)BattleGetMarioPtr(battleWork);
            party = (u8*)BattleGetPartyPtr(battleWork);

            if (party != 0 && BtlUnit_CheckStatusFlag(party, 1) != 0) {
                *(u8*)(party + 0x12B) = 0;
                BtlUnit_OffStatusFlag(party, 1);
                result = BtlUnit_GetBodyPartsId(party);
                part = (u8*)BtlUnit_GetPartsPtr(party, result);
                *(void**)(part + 0x1BC) = pose_table_nokotarou_stay;
            }

            BattleConsumeReserveItem();
            for (status = 0; (s8)status < 28; status++) {
                if ((s8)status >= 28 || (s8)status < 25) {
                    if (party != 0) {
                        BtlUnit_SetStatus(party, status, 0, 0);
                    }
                    BtlUnit_SetStatus(mario, status, 0, 0);
                }
            }

            if ((*(u32*)(mario + 0x104) & 0x10) != 0) {
                party = (u8*)BattleGetPartyPtr(battleWork);
                if (party != 0) {
                    evt = (u8*)evtEntry(BtlUnit_GetData(party, 3), 10, 0);
                    *(s32*)(evt + 0x160) = *(s32*)(party + 0x0);
                    *(s32*)(party + 0x2A8) = *(s32*)(evt + 0x15C);
                }
                break;
            }

            BattleSetSeq(battleWork, 7, 0x07000006);
            /* fall through */

        case 0x07000006:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                BattleAudienceSoundClap(-1, 0);
                _ExecAllUnitBattleEndEvent();
                BattleInformationSetDropMaterial(info);
                pouch = (u8*)pouchGetPtr();
                mario = (u8*)BattleGetMarioPtr(battleWork);

                if (*(u8*)(pouch + 0x5BA) == 3) {
                    *(u8*)(bw + 0x18FF9) = 3;
                    BattleMajinaiDone();
                    BattleMajinaiEndCheck();
                    evt = (u8*)evtEntry(marioAttackEvent_MajinaiExpUp, 10, 0);
                    *(s32*)(evt + 0x160) = *(s32*)(mario + 0x0);
                    *(s32*)(mario + 0x2A8) = *(s32*)(evt + 0x15C);
                    BattleSetSeq(battleWork, 7, 0x07000007);
                } else {
                    BattleSetSeq(battleWork, 7, 0x07000008);
                }
            }
            break;

        case 0x07000007:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                BattleSetSeq(battleWork, 7, 0x07000008);
            }
            break;

        case 0x07000008:
            BattleFogForceStop();
            *(s32*)(work + 0xC) = 0;
            *(s16*)(work + 8) = (s16)BattleGetStockExp(battleWork);

            if (*(u8*)(bw + 0x18FF9) == 3) {
                *(s16*)(work + 8) <<= 1;
            }
            if (*(s16*)(pouch + 0x8A) > 98) {
                *(s16*)(work + 8) = 0;
            }
            if (*(s16*)(work + 8) > 100) {
                *(s16*)(work + 8) = 100;
            }

            *(s16*)(work + 0xA) = *(s16*)(work + 8);
            *(u32*)(bw + 0xEF8) &= ~2U;

            if (*(s16*)(work + 8) > 0) {
                _GetExpIcon_Init(work);
            }

            *(s32*)(work + 4) = 120;
            btl_camera_set_moveSpeedLv(0, 2);
            if ((*(u32*)(bw + 0xEF4) & 0x100) == 0) {
                btl_camera_set_mode(0, 0x11);
            } else {
                btl_camera_set_mode(0, 0x12);
            }
            btl_camera_set_posoffset(
                (f64)float_0_804271c0,
                (f64)float_0_804271c0,
                (f64)float_0_804271c0,
                0);
            BattleSetSeq(battleWork, 7, 0x07000009);
            BattleAudienceSoundCheer(60, 60);
            BattleAudienceSoundClap(-1, 0);
            BattleAudienceSoundWhistle();

            valid = 0;
            party = (u8*)BattleGetPartyPtr(battleWork);
            if (party != 0 && BtlUnit_CheckStatus(party, 0x1B) != 0) {
                valid = 1;
            }
            psndBGMOff(0x400);
            if (valid) {
                psndBGMOn(1, str_BGM_BATTLE_WIN2_802fea98);
            } else {
                psndBGMOn(1, str_BGM_BATTLE_WIN1_802fea88);
            }
            /* fall through */

        case 0x07000009:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) < 1) {
                BattleSetSeq(battleWork, 7, 0x0700000A);
            }
            break;

        case 0x0700000A:
            if (*(s16*)(work + 8) < 1) {
                if ((*(u32*)(bw + 0xEF4) & 0x400000) == 0) {
                    BattleSetSeq(battleWork, 7, 0x07000019);
                } else {
                    BattleSetSeq(battleWork, 7, 0x0700000E);
                }
            } else {
                mario = (u8*)BattleGetMarioPtr(battleWork);
                party = (u8*)BattleGetPartyPtr(battleWork);

                evt = (u8*)evtEntry(BtlUnit_GetData(mario, 0x40), 10, 0);
                if (evt != 0) {
                    *(s32*)(evt + 0x160) = *(s32*)(mario + 0x0);
                    *(s32*)(mario + 0x2A8) = *(s32*)(evt + 0x15C);
                }
                if (party != 0) {
                    evt = (u8*)evtEntry(BtlUnit_GetData(party, 0x40), 10, 0);
                    if (evt != 0) {
                        *(s32*)(evt + 0x160) = *(s32*)(party + 0x0);
                        *(s32*)(party + 0x2A8) = *(s32*)(evt + 0x15C);
                    }
                }

                if (*(void**)(work + 0x29C) == 0) {
                    *(void**)(work + 0x29C) = effPointGetEntry(
                        (f64)float_0_804271c0,
                        (f64)float_90_804271e4,
                        (f64)float_0_804271c0,
                        0,
                        *(s16*)(work + 8),
                        0);
                }
                BattleSetSeq(battleWork, 7, 0x0700000B);
            }
            break;

        case 0x0700000B:
            if (*(s16*)(work + 0xA) < 1) {
                if (*(s32*)(work + 4) < 1000) {
                    if (*(s32*)(work + 4) < 120) {
                        *(s32*)(work + 4) = 140 - *(s32*)(work + 4);
                    } else {
                        *(s32*)(work + 4) = 20;
                    }
                } else {
                    *(s32*)(work + 4) = 10;
                }
                BattleSetSeq(battleWork, 7, 0x0700000C);
            } else {
                *(s32*)(work + 4) += 1;
                if (BattlePadCheckNow(0x300) != 0 && *(s32*)(work + 4) < 1000) {
                    *(s32*)(work + 4) += 1000;
                }
                if ((*(s32*)(work + 4) & 3) == 0 || *(s32*)(work + 4) > 999) {
                    *(s16*)(work + 0xA) -= 1;
                    *(s16*)(pouch + 0x96) += 1;
                    if ((*(s32*)(work + 4) & 7) == 0) {
                        psndSFXOn(str_SFX_STAR_POINT_GET1_802feaa8);
                    }
                }

                if (*(s16*)(pouch + 0x96) > 99) {
                    *(s16*)(pouch + 0x96) = 0;
                    *(s32*)(work + 0xC) = 1;
                    if (*(s16*)(pouch + 0x8A) + 1 > 98) {
                        *(s16*)(work + 0xA) = 0;
                    }
                }
            }
            break;

        case 0x0700000C:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) < 1) {
                *(s32*)(work + 4) = 60;
                _GetExpIcon_End(work);
                BattleSetSeq(battleWork, 7, 0x0700000D);
                effSoftDelete(*(void**)(work + 0x29C));
                if (*(s32*)(work + 0xC) == 0) {
                    *(s32*)(work + 4) = 5;
                } else {
                    *(s32*)(work + 4) = 60;
                }
            }
            break;

        case 0x0700000D:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) < 1) {
                *(void**)(work + 0x29C) = 0;
                BattleSetSeq(battleWork, 7, 0x0700000E);
            }
            break;

        case 0x0700000E:
            if ((*(u32*)(bw + 0xEF4) & 0x400000) == 0) {
                BattleSetSeq(battleWork, 7, 0x07000010);
            } else {
                mario = (u8*)BattleGetMarioPtr(battleWork);
                evt = (u8*)evtEntry(_event_MajinaiEnd, 10, 0);
                *(s32*)(evt + 0x160) = *(s32*)(mario + 0x0);
                *(s32*)(mario + 0x2A8) = *(s32*)(evt + 0x15C);
                BattleSetSeq(battleWork, 7, 0x0700000F);
            }
            break;

        case 0x0700000F:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                if (*(s16*)(work + 8) < 1) {
                    BattleSetSeq(battleWork, 7, 0x07000019);
                } else {
                    BattleSetSeq(battleWork, 7, 0x07000010);
                }
            }
            break;

        case 0x07000010:
            *(u32*)(work + 0x30) &= ~1U;
            if (*(s32*)(work + 0xC) == 0) {
                BattleSetSeq(battleWork, 7, 0x07000019);
            } else {
                *(s32*)(work + 0x10) = 1;
                *(s32*)(work + 0x14) = 0;
                *(s32*)(work + 0x18) = 3;
                *(s32*)(work + 4) = 0;
                BattleAudience_WinSetActive(0);
                BattleBreakSlot_HideReel();
                BattleSetSeq(battleWork, 7, 0x07000011);
            }
            break;

        case 0x07000011:
            if (*(s32*)(work + 4) == 0) {
                evt = (u8*)evtEntry(_curtain_close_event, 10, 0);
                *(s32*)(work + 0x1FC) = *(s32*)(evt + 0x15C);
                BattleStatusWindowSystemOff();
                psndBGMOff(0x400);
                psndBGMOn(1, str_BGM_BATTLE_WIN3_802feabc);
            }
            *(s32*)(work + 4) += 1;
            if (!evtCheckID(*(s32*)(work + 0x1FC))) {
                *(s32*)(work + 0x1FC) = 0;
                BattleStatusWindowSystemOn();
                BattleStatusWindowEventOn();
                BattleSetSeq(battleWork, 7, 0x07000012);
            }
            break;

        case 0x07000012:
            result = *(s32*)(work + 0x10);

            if (*(s32*)(work + 0x1FC) != 0) {
                if (evtCheckID(*(s32*)(work + 0x1FC))) {
                    break;
                }
                *(s32*)(work + 0x1FC) = 0;
            }

            if (BattlePadCheckTrigger(0x80000) != 0 &&
                (u32)*(s32*)(work + 0x10) < (u32)(*(s32*)(work + 0x18) - 1)) {
                *(s32*)(work + 0x10) += 1;
            }
            if (BattlePadCheckTrigger(0x40000) != 0 &&
                *(s32*)(work + 0x10) > 0) {
                *(s32*)(work + 0x10) -= 1;
            }

            if (*(s32*)(work + 0x10) != result) {
                evt = 0;
                if (*(s32*)(work + 0x10) == 1) {
                    evt = (u8*)evtEntry(_move_to_flower_event, 10, 0);
                } else if (*(s32*)(work + 0x10) < 1) {
                    if (*(s32*)(work + 0x10) > -1) {
                        evt = (u8*)evtEntry(_move_to_heart_event, 10, 0);
                    }
                } else if (*(s32*)(work + 0x10) < 3) {
                    evt = (u8*)evtEntry(_move_to_badge_point_event, 10, 0);
                }
                *(u32*)(work + 0x30) &= ~0x20U;
                if (evt != 0) {
                    *(s32*)(work + 0x1FC) = *(s32*)(evt + 0x15C);
                    break;
                }
            }

            if (BattlePadCheckTrigger(0x100) != 0) {
                valid = 0;
                result = *(s32*)(work + 0x10);
                if (result == 1) {
                    if (*(s16*)(pouch + 0x90) < 200) {
                        valid = 1;
                    }
                } else if (result < 1) {
                    if (result > -1 && *(s16*)(pouch + 0x8E) < 200) {
                        valid = 1;
                    }
                } else if (result < 3 && *(s16*)(pouch + 0x94) < 99) {
                    valid = 1;
                }

                if (valid) {
                    *(s32*)(work + 0x24) = 0;
                    *(s32*)(work + 0x28) = 0;
                    *(s32*)(work + 0x2C) = 2;
                    *(u32*)(work + 0x30) &= ~0x20U;
                    BattleSetSeq(battleWork, 7, 0x07000014);
                    evt = (u8*)evtEntry(_jump_event, 10, 0);
                    *(s32*)(work + 0x1FC) = *(s32*)(evt + 0x15C);
                }
            }
            break;

        case 0x07000013:
            if (BattlePadCheckTrigger(0x20000) != 0) {
                *(s32*)(work + 0x24) += 1;
                if (*(s32*)(work + 0x24) >= *(s32*)(work + 0x2C)) {
                    *(s32*)(work + 0x24) = 0;
                }
            }
            if (BattlePadCheckTrigger(0x10000) != 0) {
                *(s32*)(work + 0x24) -= 1;
                if (*(s32*)(work + 0x24) < 0) {
                    *(s32*)(work + 0x24) = *(s32*)(work + 0x2C) - 1;
                }
            }
            if (BattlePadCheckTrigger(0x200) != 0) {
                *(u32*)(work + 0x30) &= ~0x10U;
                BattleSetSeq(battleWork, 7, 0x07000012);
            }
            if (BattlePadCheckTrigger(0x100) != 0) {
                if (*(s32*)(work + 0x24) == 0) {
                    *(u32*)(work + 0x30) &= ~0x18U;
                    BattleSetSeq(battleWork, 7, 0x07000015);
                } else {
                    *(u32*)(work + 0x30) &= ~0x10U;
                    BattleSetSeq(battleWork, 7, 0x07000012);
                }
            }
            break;

        case 0x07000014:
            if (!evtCheckID(*(s32*)(work + 0x1FC))) {
                *(s32*)(work + 0x1FC) = 0;
                BattleSetSeq(battleWork, 7, 0x07000015);
            }
            break;

        case 0x07000015:
            *(s16*)(pouch + 0x8A) += 1;

            result = *(s32*)(work + 0x10);
            if (result == 1) {
                *(s16*)(pouch + 0x90) += 5;
            } else if (result < 1) {
                if (result > -1) {
                    *(s16*)(pouch + 0x8E) += 5;
                }
            } else if (result < 3) {
                *(s16*)(pouch + 0x94) += 3;
            }

            pouchReviseMarioParam();
            *(s16*)(pouch + 0x70) = *(s16*)(pouch + 0x72);
            *(s16*)(pouch + 0x74) = *(s16*)(pouch + 0x76);

            mario = (u8*)BattleGetMarioPtr(battleWork);
            BtlUnit_SetParamFromPouch(mario);

            party = (u8*)BattleGetPartyPtr(battleWork);
            if (party != 0) {
                partyId = BattleTransPartyId(*(s32*)(party + 8));
                if (partyId != 0 && partyId < 8) {
                    *(s16*)(pouch + partyId * 0xE + 6) =
                        *(s16*)(pouch + partyId * 0xE + 2);
                    *(s16*)(party + 0x10C) = *(s16*)(party + 0x108);
                }
            }

            valid = 1;
            npcInfo = *(u8**)(info + 0xC);
            mario = (u8*)BattleGetMarioPtr(battleWork);
            party = (u8*)BattleGetPartyPtr(battleWork);
            flags = *(u32*)(mario + 0x138);
            if ((flags & 0x10000000) == 0 && (flags & 0x20000000) == 0) {
                if (party != 0 && BtlUnit_CheckStatus(party, 0x1B) != 0) {
                    valid = 0;
                }
            } else {
                valid = 0;
            }

            if (valid) {
                if ((*(u32*)(npcInfo + 4) & 0x20000000) == 0) {
                    BtlUnit_snd_se(
                        mario,
                        str_SFX_VOICE_MARIO_PAUS_802feae4,
                        (s32)0xF1194D80,
                        0);
                } else {
                    BtlUnit_snd_se(
                        mario,
                        str_SFX_VOICE_MARIO_PAUS_802feacc,
                        (s32)0xF1194D80,
                        0);
                }
            }

            mario = (u8*)BattleGetMarioPtr(battleWork);
            BtlUnit_SetBodyAnim(mario, str_M_V_2_80427160);
            BattleAudienceSoundCheer(180, 40);
            *(s32*)(work + 4) = 180;
            BattleSetSeq(battleWork, 7, 0x07000016);

            mario = (u8*)BattleGetMarioPtr(battleWork);
            party = (u8*)BattleGetPartyPtr(battleWork);
            if (mario != 0) {
                effRecoveryEntry(
                    (f64)*(f32*)(mario + 0x3C),
                    (f64)(float_40_80427294 + *(f32*)(mario + 0x40)),
                    (f64)*(f32*)(mario + 0x44),
                    4,
                    0);
            }
            if (party != 0) {
                effRecoveryEntry(
                    (f64)*(f32*)(party + 0x3C),
                    (f64)(float_40_80427294 + *(f32*)(party + 0x40)),
                    (f64)*(f32*)(party + 0x44),
                    4,
                    0);
            }
            break;

        case 0x07000016:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) == 120) {
                mario = (u8*)BattleGetMarioPtr(battleWork);
                if (mario != 0) {
                    effRecoveryEntry(
                        (f64)*(f32*)(mario + 0x3C),
                        (f64)(float_40_80427294 + *(f32*)(mario + 0x40)),
                        (f64)*(f32*)(mario + 0x44),
                        5,
                        0);
                }
            }
            if (*(s32*)(work + 4) < 1) {
                if (*(s32*)(work + 0x2A4) != 0) {
                    evtDeleteID(*(s32*)(work + 0x2A4));
                }
                BattleSetSeq(battleWork, 7, 0x07000017);
            }
            break;

        case 0x07000017:
            rank = _get_rank_data(*(s16*)(pouch + 0x8A));
            if (rank == 0) {
                BattleSetSeq(battleWork, 7, 0x0700001C);
                break;
            }
            rank = _get_rank_data(*(s16*)(pouch + 0x8A));
            *(s16*)(pouch + 0x88) = rank->field_0x2;
            evt = (u8*)evtEntry(_rank_up_event, 10, 0);
            *(s32*)(work + 0x2A8) = *(s32*)(evt + 0x15C);
            BattleSetSeq(battleWork, 7, 0x07000018);
            /* fall through */

        case 0x07000018:
            if (!evtCheckID(*(s32*)(work + 0x2A8))) {
                BattleSetSeq(battleWork, 7, 0x0700001C);
            }
            break;

        case 0x07000019:
            valid = 1;
            npcInfo = *(u8**)(info + 0xC);
            mario = (u8*)BattleGetMarioPtr(battleWork);
            party = (u8*)BattleGetPartyPtr(battleWork);
            flags = *(u32*)(mario + 0x138);

            if ((flags & 0x10000000) == 0 && (flags & 0x20000000) == 0) {
                if (party != 0 && BtlUnit_CheckStatus(party, 0x1B) != 0) {
                    valid = 0;
                }
            } else {
                valid = 0;
            }

            if (!valid) {
                if ((*(u32*)(npcInfo + 4) & 0x20000000) == 0) {
                    BtlUnit_snd_se(
                        mario,
                        str_SFX_VOICE_MARIO_RELI_802feb14,
                        (s32)0xF1194D80,
                        0);
                } else {
                    BtlUnit_snd_se(
                        mario,
                        str_SFX_VOICE_MARIO_RELI_802feafc,
                        (s32)0xF1194D80,
                        0);
                }
            }

            mario = (u8*)BattleGetMarioPtr(battleWork);
            BtlUnit_SetBodyAnim(mario, str_M_V_1_80427298);
            *(s32*)(work + 4) = 60;
            BattleSetSeq(battleWork, 7, 0x0700001A);
            break;

        case 0x0700001A:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) < 1) {
                mario = (u8*)BattleGetMarioPtr(battleWork);
                BtlUnit_SetBodyAnimType(mario, 0x2A);

                party = (u8*)BattleGetPartyPtr(battleWork);
                if (party != 0) {
                    party = (u8*)BattleGetPartyPtr(battleWork);
                    if (*(s32*)(party + 8) != 0xE1 ||
                        *(u8*)(party + 0x12B) < 1) {
                        party = (u8*)BattleGetPartyPtr(battleWork);
                        BtlUnit_SetBodyAnimType(party, 0x2A);
                    }
                }
                BattleSetSeq(battleWork, 7, 0x0700001B);
            }
            break;

        case 0x0700001B:
            BattleSetSeq(battleWork, 7, 0x0700001C);
            break;

        case 0x0700001C:
            BattleSetSeq(battleWork, 7, 0x0700001D);
            BattleAudienceSoundSetVolAll(0, 30);
            fbat = (u8*)fbatGetPointer();
            npc = *(u8**)(fbat + 8);
            if (npc != 0) {
                *(u32*)(npc + 0x140) |= 0x100;
            }
            break;

        case 0x0700001D:
            fadeColor = dat_80427144;
            fadeEntry(10, 200, &fadeColor);
            BattleSetSeq(battleWork, 7, 0x0700001E);
            psndBGMOff(0x201);
            /* fall through */

        case 0x0700001E:
            if (fadeIsFinish() != 0) {
                BattleSetSeq(battleWork, 7, 0x0700001F);
            }
            break;

        case 0x0700001F:
            party = (u8*)BattleGetPartyPtr(battleWork);
            if (party == 0) {
                unitKind = 0;
            } else {
                unitKind = *(s32*)(party + 8);
            }
            partyId = BattleTransPartyId(unitKind);

            npcInfo = *(u8**)(info + 0xC);
            mario = (u8*)BattleGetMarioPtr(battleWork);
            if ((*(u32*)(npcInfo + 4) & 0x40) != 0 &&
                *(s16*)(mario + 0x10C) < 1) {
                *(s16*)(mario + 0x10C) = 1;
            }

            BattleSetMarioParamToFieldBattle(battleWork);
            BattleFree(*(void**)(bw + 0xF28));
            *(void**)(bw + 0xF28) = 0;
            BattleSetSeq(battleWork, 0, 4);

            player = (u8*)marioGetPtr();
            *(u32*)(player + 4) &= ~1U;

            if (unitKind == 0) {
                marioPartyKill();
            } else {
                marioPartyEntry(partyId);
            }

            partyId = marioGetPartyId();
            fieldParty = (u8*)partyGetPtr(partyId);
            if (fieldParty != 0) {
                *(u32*)(fieldParty + 4) &= ~1U;
            }

            BtlActRec_JudgeRuleKeep();
            BattleAudienceSoundStop(1);
            break;
    }

    btlseqEnd_DispMain(8, battleWork);
}

void _ExecAllUnitBattleEndEvent(void) {
    void* battleWork = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL) {
            void* data = BtlUnit_GetData(unit, 0x3F);

            if (data != NULL) {
                void* evt = evtEntry(data, 10, 0);

                *(s32*)((s32)evt + 0x160) = *(s32*)unit;
            }
        }
    }
}

void btlseqEnd_DispInit(void* unit) {
    *(u32*)((s32)*(void**)((s32)unit + 0xF28) + 0x30) = 0;
}

void btlseqEnd_DispMain(void* disp, void* battleWork) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    void* battle;
    void* work;
    Vec vec;
    u32 flags;

    extern f32 float_900_8042720c;
    extern const Vec vec3_802fe7cc;
    extern void _GetExpIcon_Main(void* battleWork);
    extern void _GetExpIcon_Disp(void* battleWork);
    extern void _MarioExpDisp(void* battleWork);
    extern void _lvup_select_object_disp(void* battleWork);
    extern void _LvupParamConfirmDisp(void* battleWork);
    extern void L__LvupParamHelpMsgDisp(void* battleWork);
    extern f32 dispCalcZ(Vec* pos);

    battle = battleWork;
    work = *(void**)((s32)battle + 0xF28);
    if (work != NULL) {
        flags = *(u32*)((s32)work + 0x30);
        if (flags & 2) {
            _GetExpIcon_Main(battle);
            dispEntry((s32)disp, 1, _GetExpIcon_Disp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 4) {
            dispEntry((s32)disp, 1, _MarioExpDisp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 8) {
            vec = vec3_802fe7cc;
            dispEntry(4, 2, _lvup_select_object_disp, battle, dispCalcZ(&vec));
        }
        if (*(u32*)((s32)work + 0x30) & 0x10) {
            dispEntry((s32)disp, 1, _LvupParamConfirmDisp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 0x20) {
            dispEntry((s32)disp, 1, L__LvupParamHelpMsgDisp, battle, float_900_8042720c);
        }
    }
}

void _MarioExpDisp(void* disp, void* battleWork) {
    u32 color;
    f32 pos[3];
    char buf[16];
    void* work;
    s32 len;

    extern f32 float_200_8042727c;
    extern f32 float_neg200_80427280;
    extern f32 float_0_804271c0;
    extern char str_PCT2d_80427284;
    extern u32 dat_80427148;
    extern f32 float_12p5_80427254;
    extern f32 float_65_804271ec;
    extern f32 float_180_804271f0;
    extern f32 float_neg160_80427288;
    extern f32 float_32_80427258;
    extern f32 float_10_80427214;
    extern f32 float_1_804271a8;
    extern f32 float_225_8042728c;
    extern f32 float_neg163_80427290;
    extern s32 sprintf(char* str, const char* format, ...);
    extern s32 strlen(const char* str);
    extern void iconDispGx(f32* pos, s32 icon, s32 color, f32 scale);
    extern void FontDrawStart(void);
    extern void FontDrawString(const char* str, f32 x, f32 y);

    work = *(void**)((s32)battleWork + 0xF28);
    pouchGetPtr();
    pos[0] = float_200_8042727c;
    pos[1] = float_neg200_80427280;
    pos[2] = float_0_804271c0;

    if (work != NULL) {
        sprintf(buf, &str_PCT2d_80427284, *(s16*)((s32)work + 0xA));
        len = strlen(buf);
        color = dat_80427148;
        windowDispGX_Waku_col(
            0,
            &color,
            float_180_804271f0,
            float_neg160_80427288,
            float_12p5_80427254 * len + float_65_804271ec,
            float_32_80427258,
            float_10_80427214);
        iconDispGx(pos, 0x10, 0x194, float_1_804271a8);
        FontDrawStart();
        FontDrawString(buf, float_225_8042728c, float_neg163_80427290);
    }
}

void _LvupParamConfirmDisp(void* disp, void* battleWork) {
    char buf[20];
    u32 color1;
    u32 color2;
    f32 pos[3];
    void* work;
    register char* base;
    u32 len;
    f32 halfWidth;

    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_8042714c;
    extern u32 dat_80427150;
    extern f32 float_6p25_8042724c;
    extern f32 float_neg30_80427248;
    extern f32 float_12p5_80427254;
    extern f32 float_65_804271ec;
    extern f32 float_30_80427250;
    extern f32 float_32_80427258;
    extern f32 float_10_80427214;
    extern f32 float_neg70_8042725c;
    extern f32 float_neg10_80427260;
    extern f32 float_130_80427264;
    extern f32 float_60_80427268;
    extern f32 float_0_804271c0;
    extern f32 float_27_8042726c;
    extern f32 float_neg20_804271d4;
    extern f32 float_neg13_80427270;
    extern f32 float_neg38_80427274;
    extern f32 float_neg40_80427278;
    extern f32 float_1_804271a8;
    extern const char* msgSearch(const char* msg);
    extern s32 sprintf(char* str, const char* format, ...);
    extern u32 strlen(const char* str);
    extern void FontDrawStart(void);
    extern void FontDrawString(const char* str, f32 x, f32 y);
    extern void iconDispGx(f32* pos, s32 icon, s32 color, f32 scale);

    base = str_msg_menu_mario_name__802fe738;
    work = *(void**)((s32)battleWork + 0xF28);
    pouchGetPtr();
    sprintf(buf, msgSearch(base + 0x314));
    len = strlen(buf);
    halfWidth = float_6p25_8042724c * len;
    color1 = dat_8042714c;
    windowDispGX_Waku_col(
        0,
        &color1,
        float_neg30_80427248 - halfWidth,
        float_30_80427250,
        float_12p5_80427254 * len + float_65_804271ec,
        float_32_80427258,
        float_10_80427214);
    color2 = dat_80427150;
    windowDispGX_Waku_col(
        0,
        &color2,
        float_neg70_8042725c,
        float_neg10_80427260,
        float_130_80427264,
        float_60_80427268,
        float_10_80427214);
    FontDrawStart();
    FontDrawString(buf, float_0_804271c0 - halfWidth, float_27_8042726c);
    FontDrawString(msgSearch(base + 0x324), float_neg20_804271d4, float_neg13_80427270);
    FontDrawString(msgSearch(base + 0x338), float_neg20_804271d4, float_neg38_80427274);

    pos[0] = float_neg40_80427278;
    pos[1] = (f32)(-50 - (*(s32*)((s32)work + 0x24) * 25));
    pos[2] = float_0_804271c0;
    iconDispGx(pos, 0x10, 0x194, float_1_804271a8);
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: L__LvupParamHelpMsgDisp | prototype_only | source_prototype */
void L__LvupParamHelpMsgDisp(void* disp, void* battleWork) {
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_80427154;
    extern f32 float_0_804271c0;
    extern f32 float_20_8042718c;
    extern f32 float_neg145_80427210;
    extern f32 float_10_80427214;
    extern f32 float_neg148_80427218;
    extern void* pouchGetPtr(void);
    extern const char* msgSearch(const char* msg);
    extern u32 FontGetMessageWidthLine(const char* msg, u16* lines);
    extern void FontDrawStart_alpha(s32 alpha);
    extern void FontDrawMessage(s32 x, s32 y, const char* msg);
    void* work;
    void* pouch;
    char* base;
    const char* msg;
    u16 lines;
    u32 color;
    s32 width;
    s32 lineCount;
    f32 x;

    base = str_msg_menu_mario_name__802fe738;
    work = *(void**)((s32)battleWork + 0xF28);
    pouch = pouchGetPtr();

    switch (*(s32*)((s32)work + 0x10)) {
        case 0:
            msg = msgSearch(base + 0x290);
            if (*(s16*)((s32)pouch + 0x8E) >= 0xC8) {
                msg = msgSearch(base + 0x2A4);
            }
            break;
        case 1:
            msg = msgSearch(base + 0x2BC);
            if (*(s16*)((s32)pouch + 0x90) >= 0xC8) {
                msg = msgSearch(base + 0x2D0);
            }
            break;
        case 2:
            msg = msgSearch(base + 0x2E8);
            if (*(s16*)((s32)pouch + 0x94) >= 0x63) {
                msg = msgSearch(base + 0x2FC);
            }
            break;
    }

    width = FontGetMessageWidthLine(msg, &lines) & 0xFFFF;
    lineCount = (u16)(lines + 1);
    color = dat_80427154;
    x = float_0_804271c0 - (f32)(width / 2);
    lines = lineCount;
    windowDispGX_Waku_col(
        0,
        &color,
        x - float_20_8042718c,
        float_neg145_80427210 + (f32)((lineCount - 1) * 0x1D),
        (f32)(width + 0x28),
        (f32)(lineCount * 0x1D + 1),
        float_10_80427214);
    FontDrawStart_alpha(0xFF);
    FontDrawMessage((s32)x, (s32)float_neg148_80427218 + ((s32)lines - 1) * 0x1D, msg);
}

/* CHATGPT FALLBACK MISSING STUBS: main/battle/battle_seq_end 20260624_191429 */

/* fallback stub-fill: map=_GetExpIcon_Init addr=0x802149ec size=0x0000025c */
void _GetExpIcon_Init(void* seqEndWork) {
    extern int sprintf(char* s, const char* fmt, ...);
    extern void iconEntry2D(char* name, u16 id);
    extern void* iconNameToPtr(char* name);
    extern void iconSetScale(f64 scale, char* name);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconSetPos(f64 x, f64 y, f64 z, char* name);
    extern const char str_BgspPCTd_802fe9c0[];
    extern f32 float_0_804271c0;
    extern f32 float_25_804271e0;
    extern f32 float_24_8042722c;
    extern f32 float_18_80427230;
    extern f32 float_36_80427234;
    extern f32 float_0p9_80427238;
    extern f32 float_0p45_8042723c;
    extern f32 float_86p9_80427240;
    extern f32 float_16p2_80427244;

    char iconEntryName[16];
    void* iconEntry;
    s32 i;
    s32 iconOffset;
    s32 posOffset;
    f32 f36 = float_36_80427234;
    f32 f18 = float_18_80427230;
    f32 f24 = float_24_8042722c;
    f32 f25 = float_25_804271e0;
    f32 zero = float_0_804271c0;

    iconOffset = 0;
    posOffset = 0;
    for (i = 0; i < 10; i++) {
        sprintf(iconEntryName, str_BgspPCTd_802fe9c0, i);
        iconEntry2D(iconEntryName, 0x194);
        iconEntry = iconNameToPtr(iconEntryName);
        *(void**)((s32)seqEndWork + 0x34 + iconOffset) = iconEntry;
        iconSetScale(float_0p9_80427238, iconEntryName);
        iconFlagOn(iconEntryName, 2);
        *(s32*)((s32)seqEndWork + 0xCC + iconOffset) = 0;
        *(f32*)((s32)seqEndWork + 0x118 + posOffset) =
            f36 * (f32)i - (f18 * (f32)(*(s16*)((s32)seqEndWork + 0xA) / 10) - f24);
        *(f32*)((s32)seqEndWork + 0x11C + posOffset) = f25;
        *(f32*)((s32)seqEndWork + 0x120 + posOffset) = zero;
        iconSetPos(*(f32*)((s32)seqEndWork + 0x118 + posOffset),
                   *(f32*)((s32)seqEndWork + 0x11C + posOffset),
                   *(f32*)((s32)seqEndWork + 0x120 + posOffset),
                   iconEntryName);
        iconOffset += 4;
        posOffset += 0xC;
    }

    {
        f32 f162 = float_16p2_80427244;
        f32 f869 = float_86p9_80427240;
        f32 zero2 = float_0_804271c0;

        iconOffset = 0x28;
        posOffset = 0x78;
        for (i = 10; i < 0x13; i++) {
            sprintf(iconEntryName, str_BgspPCTd_802fe9c0, i);
            iconEntry2D(iconEntryName, 0x194);
            iconEntry = iconNameToPtr(iconEntryName);
            *(void**)((s32)seqEndWork + 0x34 + iconOffset) = iconEntry;
            iconSetScale(float_0p45_8042723c, iconEntryName);
            iconFlagOn(iconEntryName, 2);
            *(s32*)((s32)seqEndWork + 0xCC + iconOffset) = 0;
            *(f32*)((s32)seqEndWork + 0x118 + posOffset) =
                -(f162 * (f32)(i - 9) - f869);
            *(f32*)((s32)seqEndWork + 0x11C + posOffset) = zero2;
            *(f32*)((s32)seqEndWork + 0x120 + posOffset) = zero2;
            iconSetPos(*(f32*)((s32)seqEndWork + 0x118 + posOffset),
                       *(f32*)((s32)seqEndWork + 0x11C + posOffset),
                       *(f32*)((s32)seqEndWork + 0x120 + posOffset),
                       iconEntryName);
            iconOffset += 4;
            posOffset += 0xC;
        }
    }
    *(u32*)((s32)seqEndWork + 0x30) |= 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _GetExpIcon_Main | prototype_only | source_prototype */
void _GetExpIcon_Main(void* battleWork) {
    extern void iconFlagOff(char* name, u16 flags);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconSetAlpha(char* name, u8 alpha);
    extern void iconSetPos(f32 x, f32 y, f32 z, char* name);
    extern f32 float_0_804271c0;
    extern f32 float_24_8042722c;
    extern f32 float_18_80427230;
    extern f32 float_36_80427234;

    void* seqEndWork;
    s16 remaining;
    s32 tens;
    s32 i;
    s32 posOffset;
    s32 iconOffset;
    s32 alpha;
    s32 ones;
    s32 zero;
    s32 alphaStep;
    s32 alphaMax;
    f32 step;
    f32 countScale;
    f32 centerOffset;
    f32 x;
    f32 baseY;

    seqEndWork = *(void**)((s32)battleWork + 0xF28);
    step = float_36_80427234;
    remaining = *(s16*)((s32)seqEndWork + 0xA);
    i = 0;
    posOffset = 0;
    tens = remaining / 10;
    countScale = float_18_80427230;
    centerOffset = float_24_8042722c;
    iconOffset = 0;

    for (; i < tens; i++) {
        void* slot = (void*)((s32)seqEndWork + iconOffset);

        iconFlagOff((char*)((s32)*(void**)((s32)slot + 0x34) + 0x18), 2);

        alpha = *(s32*)((s32)slot + 0x80);
        if (alpha <= 0) {
            *(s32*)((s32)slot + 0x80) = 0x40;
        } else {
            *(s32*)((s32)slot + 0x80) = alpha + 0x40;
            if (*(s32*)((s32)slot + 0x80) > 0xFF) {
                *(s32*)((s32)slot + 0x80) = 0xFF;
            }
        }

        alpha = *(s32*)((s32)slot + 0xCC);
        if (alpha < 0x10) {
            *(s32*)((s32)slot + 0xCC) = alpha + 1;
        }

        x = -(countScale * (f32)(*(s16*)((s32)seqEndWork + 0xA) / 10) - centerOffset);
        x = step * (f32)i + x;
        *(f32*)((s32)seqEndWork + 0x118 + posOffset) = x;

        posOffset += 0xC;
        iconOffset += 4;
    }

    iconOffset = i << 2;
    zero = 0;
    for (; i < 10; i++) {
        void* slot = (void*)((s32)seqEndWork + iconOffset);

        iconFlagOn((char*)((s32)*(void**)((s32)slot + 0x34) + 0x18), 2);
        *(s32*)((s32)slot + 0x80) = zero;
        *(s32*)((s32)slot + 0xCC) = zero;
        iconOffset += 4;
    }

    i = 10;
    iconOffset = 0x28;
    alphaStep = 0x40;
    alphaMax = 0xFF;
    ones = remaining % 10;

    for (; i - 10 < ones; i++) {
        void* slot = (void*)((s32)seqEndWork + iconOffset);

        iconFlagOff((char*)((s32)*(void**)((s32)slot + 0x34) + 0x18), 2);

        alpha = *(s32*)((s32)slot + 0x80);
        if (alpha <= 0) {
            *(s32*)((s32)slot + 0x80) = alphaStep;
        } else {
            *(s32*)((s32)slot + 0x80) = alpha + 0x40;
            if (*(s32*)((s32)slot + 0x80) > alphaMax) {
                *(s32*)((s32)slot + 0x80) = alphaMax;
            }
        }

        iconOffset += 4;
    }

    iconOffset = i << 2;
    zero = 0;
    for (; i < 0x13; i++) {
        void* slot = (void*)((s32)seqEndWork + iconOffset);

        iconFlagOn((char*)((s32)*(void**)((s32)slot + 0x34) + 0x18), 2);
        *(s32*)((s32)slot + 0x80) = zero;
        iconOffset += 4;
    }

    baseY = float_0_804271c0;
    i = 0;
    posOffset = 0;
    iconOffset = 0;
    do {
        void* slot = (void*)((s32)seqEndWork + iconOffset);

        iconSetAlpha((char*)((s32)*(void**)((s32)slot + 0x34) + 0x18),
                     (u8)*(s32*)((s32)slot + 0x80));
        iconSetPos(*(f32*)((s32)seqEndWork + 0x118 + posOffset),
                   *(f32*)((s32)seqEndWork + 0x11C + posOffset) + baseY,
                   *(f32*)((s32)seqEndWork + 0x120 + posOffset),
                   (char*)((s32)*(void**)((s32)slot + 0x34) + 0x18));

        i++;
        iconOffset += 4;
        posOffset += 0xC;
    } while (i < 0x13);
}

void _GetExpIcon_Disp(void) {
    u32 color = dat_80427158;

    windowDispGX_Waku_col(0, &color, -76.9f, 21.0f, 165.8f, 24.2f, 10.0f);
}

void _GetExpIcon_End(void* work) {
    s32 i;

    *(u32*)((s32)work + 0x30) &= ~2;
    for (i = 0; i < 19; i++) {
        iconDelete((void*)((s32)*(void**)((s32)work + i * 4 + 0x34) + 0x18));
    }
}

s32 N__set_initial_upgrade_cursor_position(void* event) {
    s32 value = evtGetValue(event, **(s32***)((s32)event + 0x18));
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    *(s32*)((s32)work + 0x10) = value;
    return 2;
}

void N__draw_select_one_to_upgrade_text(void) {
    const char* msg;
    u16 lines;
    u32 color;
    s32 width;
    s32 lineCount;
    f32 x;

    extern const char str_select_HPFPBP_802fe9b0[14];
    extern u32 dat_80427168;
    extern f32 float_0_804271c0;
    extern f32 float_20_8042718c;
    extern f32 float_neg145_80427210;
    extern f32 float_10_80427214;
    extern f32 float_neg148_80427218;
    extern const char* msgSearch(const char* msg);
    extern u32 FontGetMessageWidthLine(const char* msg, u16* lines);
    extern void FontDrawStart_alpha(s32 alpha);
    extern void FontDrawMessage(s32 x, s32 y, const char* msg);

    msg = msgSearch(str_select_HPFPBP_802fe9b0);
    width = FontGetMessageWidthLine(msg, &lines) & 0xFFFF;
    lineCount = (u16)(lines + 1);
    color = dat_80427168;
    x = float_0_804271c0 - (f32)(width / 2);
    lines = lineCount;
    windowDispGX_Waku_col(
        0,
        &color,
        x - float_20_8042718c,
        float_neg145_80427210 + (f32)((lineCount - 1) * 0x1D),
        (f32)(width + 0x28),
        (f32)(lineCount * 0x1D + 1),
        float_10_80427214);
    FontDrawStart_alpha(0xFF);
    FontDrawMessage((s32)x, (s32)float_neg148_80427218 + ((s32)lines - 1) * 0x1D, msg);
}

s32 N__display_select_one_to_upgrade_text(void) {
    dispEntry(8, 1, N__draw_select_one_to_upgrade_text, _battleWorkPointer, 900.0f);
    return 2;
}

s32 _nozzle_set_offset(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 id = evtGetValue(event, *(s32*)args);
    f32 offset = evtGetFloat(event, *(s32*)((s32)args + 4));
    void* obj = BattleSearchObjectPtr(id);

    if (obj != NULL) {
        *(f32*)((s32)obj + 0x14) = offset;
    }
    return 2;
}

s32 _lvup_ap_recover_full(void) {
    void* audience = BattleAudienceBaseGetPtr();

    *(s32*)((s32)audience + 0x13910) = 1;
    pouchSetAP(pouchGetMaxAP());
    return 2;
}

s32 _disable_enemy_unit_disp(void) {
    void* unit;
    s32 i;
    void* battleWork;
    s32 kind;
    void* data;
    void* evt;
    void* obj;

    extern const char str_if_body_802fe9a0[8];
    extern const char str_if_wire_802fe9a8[8];
    extern void mapGrpFlagOn(const char* name, s32 flag);

    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != NULL && (*(u32*)((s32)unit + 0x1C) & 0x80000000) == 0) {
            kind = *(s32*)((s32)unit + 8);
            if (kind != 0xDC && kind != 0xDE) {
                if (!(kind >= 0xE0 && kind < 0xE7 && *(s8*)((s32)unit + 0xC) == 0) || kind == 0xE5) {
                    data = BtlUnit_GetData(unit, 0x42);
                    if (data != NULL) {
                        evt = evtEntry(data, 10, 0);
                        *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                    } else if (*(s32*)((s32)unit + 8) != 0xE5) {
                        *(u32*)((s32)unit + 0x104) |= 0x21000000;
                    }
                }
            }
        }
    }

    obj = BattleSearchObjectPtr(str_if_body_802fe9a0);
    if (obj != NULL) {
        *(u8*)((s32)battleWork + 0x180F8) |= 0x20;
        *(u32*)((s32)obj + 0x68) |= 1;
        mapGrpFlagOn(str_if_body_802fe9a0, 1);
        mapGrpFlagOn(str_if_wire_802fe9a8, 1);
    }

    return 2;
}

s32 _set_confetti_control_evtid(void* event) {
    s32 value = **(s32**)((s32)event + 0x18);
    void* battleWork = _battleWorkPointer;
    void* work = *(void**)((s32)battleWork + 0xF28);

    *(s32*)((s32)work + 0x2A4) = evtGetValue(event, value);
    return 2;
}

s32 _lvup_select_obj_disp_on(void) {
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    *(s32*)((s32)work + 0x30) |= 8;
    return 2;
}

s32 _lvup_select_obj_lvup_param_disp(void) {
    s32 i;
    u8* work = *(u8**)((s32)_battleWorkPointer + 0xF28);

    for (i = 0; i < 3; i++) {
        work[i * 0x34 + 0x228] = 1;
    }
    return 2;
}

s32 _lvup_obj_set_color(void* event) {
    s32* args;
    void* work;
    s32 index;
    s32 entry;

    args = *(s32**)((s32)event + 0x18);
    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    index = evtGetValue(event, args[0]);
    entry = (s32)work + index * 0x34 + 0x200;

    *(u8*)(entry + 0x30) = evtGetValue(event, args[1]);
    *(u8*)(entry + 0x31) = evtGetValue(event, args[2]);
    *(u8*)(entry + 0x32) = evtGetValue(event, args[3]);
    *(u8*)(entry + 0x33) = evtGetValue(event, args[4]);
    return 2;
}

s32 _lvup_disp_flag_onoff(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);
    s32 on = evtGetValue(event, *(s32*)args);
    s32 flag = *(s32*)((s32)args + 4);

    if (on) {
        *(s32*)((s32)work + 0x30) |= flag;
    } else {
        *(s32*)((s32)work + 0x30) &= ~flag;
    }
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_spot_on | missing_definition | ghidra_signature */
s32 _lvup_spot_on(void* event) {
    typedef struct Vec3f_LvupSpotOn {
        f32 x;
        f32 y;
        f32 z;
    } Vec3f_LvupSpotOn;
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_8042716c;
    extern f32 float_25_804271e0;
    extern f32 float_neg50_80427208;
    extern void* BatSpotGetPtr(void);
    char* base = str_msg_menu_mario_name__802fe738;
    u8* spot = BatSpotGetPtr();
    s32* args = *(s32**)((u8*)event + 0x18);
    u32 id;
    u8* entry;
    Vec3f_LvupSpotOn pos0;
    Vec3f_LvupSpotOn pos1;
    Vec3f_LvupSpotOn pos2;
    Vec3f_LvupSpotOn pos3;
    Vec3f_LvupSpotOn pos4;

    *(s32*)(spot + 8) = 1;
    id = evtGetValue(event, *args);
    entry = *(u8**)(spot + 4) + id * 0x2C;
    switch (id) {
        case 0:
            pos0.x = float_neg50_80427208 + float_25_804271e0 * (f32)(s32)id;
            pos0.y = *(f32*)(base + 0x1A0);
            pos0.z = *(f32*)(base + 0x1A4);
            *(Vec3f_LvupSpotOn*)(entry + 4) = pos0;
            break;
        case 1:
            pos1.x = float_neg50_80427208 + float_25_804271e0 * (f32)id;
            pos1.y = *(f32*)(base + 0x1AC);
            pos1.z = *(f32*)(base + 0x1B0);
            *(Vec3f_LvupSpotOn*)(entry + 4) = pos1;
            break;
        case 2:
            pos2.x = float_neg50_80427208 + float_25_804271e0 * (f32)(s32)id;
            pos2.y = *(f32*)(base + 0x1B8);
            pos2.z = *(f32*)(base + 0x1BC);
            *(Vec3f_LvupSpotOn*)(entry + 4) = pos2;
            break;
        case 3:
            pos3.x = float_neg50_80427208 + float_25_804271e0 * (f32)id;
            pos3.y = *(f32*)(base + 0x1C4);
            pos3.z = *(f32*)(base + 0x1C8);
            *(Vec3f_LvupSpotOn*)(entry + 4) = pos3;
            break;
        case 4:
            pos4.x = float_neg50_80427208 + float_25_804271e0 * 4.0f;
            pos4.y = *(f32*)(base + 0x1D0);
            pos4.z = *(f32*)(base + 0x1D4);
            *(Vec3f_LvupSpotOn*)(entry + 4) = pos4;
            break;
    }
    *(u32*)(entry + 0x10) = *(u32*)(base + 0x1D8);
    *(u32*)(entry + 0x14) = *(u32*)(base + 0x1DC);
    *(u32*)(entry + 0x18) = *(u32*)(base + 0x1E0);
    *(u32*)(entry + 0x1C) = *(u32*)(base + 0x1E4);
    *(u32*)(entry + 0x20) = *(u32*)(base + 0x1E8);
    *(u32*)(entry + 0x24) = *(u32*)(base + 0x1EC);
    *(u32*)(entry + 0x28) = dat_8042716c;
    *(u16*)entry |= 2;
    return 2;
}

s32 _lvup_spot_move(void* event, s32 init) {
    typedef struct Vec3f_LvupSpotMove {
        f32 x;
        f32 y;
        f32 z;
    } Vec3f_LvupSpotMove;
    typedef struct BattleWorkStageSpotEntry_LvupSpotMove {
        u16 flags;
        u16 pad;
        Vec3f_LvupSpotMove translation;
        Vec3f_LvupSpotMove rotation;
        Vec3f_LvupSpotMove scale;
        u32 color;
    } BattleWorkStageSpotEntry_LvupSpotMove;
    typedef struct BattleWorkStageSpot_LvupSpotMove {
        s32 count;
        BattleWorkStageSpotEntry_LvupSpotMove* entries;
        s32 pad;
    } BattleWorkStageSpot_LvupSpotMove;

    extern BattleWorkStageSpot_LvupSpotMove* BatSpotGetPtr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern f32 reviseAngle(f32 angle);
    extern f32 sinfd(f32 angle);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 float_0p5_804271d0;
    extern f32 float_neg20_804271d4;
    extern f32 float_3_804271b0;
    extern f32 float_1p1_804271dc;
    extern f32 float_50_804271d8;
    extern f32 float_2p5_804271e8;
    extern f32 float_90_804271e4;
    extern f32 float_25_804271e0;
    extern f32 float_180_804271f0;
    extern f32 float_65_804271ec;
    extern f32 float_7p5_804271f4;
    extern f32 float_4_804271bc;
    extern f32 float_270_804271f8;
    extern f32 float_4p5_804271fc;
    extern f32 float_0_804271c0;
    extern f32 float_neg25_80427200;
    extern f32 float_255_804271cc;
    extern f32 float_128_80427204;
    extern u32 dat_80427170;

    BattleWorkStageSpot_LvupSpotMove* spot;
    BattleWorkStageSpotEntry_LvupSpotMove* entries;
    BattleWorkStageSpotEntry_LvupSpotMove* entry;
    void* mario;
    s32 id;
    s32 state;
    s32 timer;
    u32 color;
    f32 angle;
    f32 value;
    f32 wave;

    spot = BatSpotGetPtr();
    id = evtGetValue(event, **(s32**)((s32)event + 0x18));
    entries = spot->entries;
    entry = &entries[id];

    if (init != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = 0;
        *(s32*)((s32)event + 0x80) = (s32)entry->translation.x;
        *(s32*)((s32)event + 0x84) = (s32)entry->translation.y;
        *(s32*)((s32)event + 0x88) = 0;
    }

    *(s32*)((s32)event + 0x78) += 1;
    state = *(s32*)((s32)event + 0x7C);

    switch (state) {
        case 0:
            *(s32*)((s32)event + 0x78) = 1;
            *(s32*)((s32)event + 0x7C) += 1;
            /* fallthrough */
        case 1:
            switch (id) {
                case 0:
                    mario = BattleGetMarioPtr(_battleWorkPointer);
                    entry->translation.x = float_0p5_804271d0 * *(f32*)((s32)mario + 0x3C);
                    entry->translation.y = float_neg20_804271d4;
                    break;
                case 1:
                    angle = reviseAngle(float_1p1_804271dc * (float_3_804271b0 * (f32)*(s32*)((s32)event + 0x78)));
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value + float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) + 25);
                    entry->translation.y = value - float_25_804271e0 * wave;
                    break;
                case 2:
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_180_804271f0);
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value + float_65_804271ec * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_7p5_804271f4 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) - 25);
                    entry->translation.y = value + float_25_804271e0 * wave;
                    break;
                case 3:
                    angle = reviseAngle(float_1p1_804271dc * (float_4_804271bc * (f32)*(s32*)((s32)event + 0x78)));
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value - float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) + 25);
                    entry->translation.y = value - float_25_804271e0 * wave;
                    break;
                case 4:
                    angle = reviseAngle(float_1p1_804271dc * (float_3_804271b0 * (f32)*(s32*)((s32)event + 0x78)) + float_270_804271f8);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x80) - 50);
                    entry->translation.x = value - float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_4p5_804271fc * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) - 25);
                    entry->translation.y = value + float_25_804271e0 * wave;
                    break;
            }
            break;
        case 2:
            *(s32*)((s32)event + 0x88) = 20;
            *(s32*)((s32)event + 0x80) = (s32)entry->translation.x;
            *(s32*)((s32)event + 0x84) = (s32)entry->translation.y;
            *(s32*)((s32)event + 0x7C) += 1;
            /* fallthrough */
        case 3:
            timer = *(s32*)((s32)event + 0x88) - 1;
            *(s32*)((s32)event + 0x88) = timer;
            entry->translation.x = intplGetValue(2, timer, 21, float_0_804271c0, (f32)*(s32*)((s32)event + 0x80));
            entry->translation.y = intplGetValue(2, timer, 21, float_neg25_80427200, (f32)*(s32*)((s32)event + 0x84));
            color = dat_80427170;
            ((u8*)&color)[3] = (s32)intplGetValue(0, timer, 21, float_255_804271cc, float_128_80427204);
            entry->color = color;
            if (*(s32*)((s32)event + 0x88) <= 0) {
                *(s32*)((s32)event + 0x7C) += 1;
            }
            break;
    }

    state = *(s32*)((s32)event + 0x7C);
    return (state >= 4) ? 2 : 0;
}

s32 _lvup_spot_off(void* event, s32 init) {
    void* spot;
    void* entry;
    s32 i;
    s32 offset;
    u32 color;
    s32 alpha;

    extern void* BatSpotGetPtr(void);
    extern u32 dat_80427174;
    extern f32 float_0_804271c0;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    spot = BatSpotGetPtr();
    if (init) {
        *(s32*)((s32)event + 0x78) = evtGetValue(event, **(s32**)((s32)event + 0x18));
        *(s32*)((s32)event + 0x7C) = *(s32*)((s32)event + 0x78);
        *(s32*)((s32)event + 0x80) = *(u8*)((s32)*(void**)((s32)spot + 4) + 0x2B);
    }

    *(s32*)((s32)event + 0x7C) -= 1;
    for (i = 0, offset = 0; i < 5; i++, offset += 0x2C) {
        entry = (void*)((s32)*(void**)((s32)spot + 4) + offset);
        alpha = (s32)intplGetValue(
            2,
            *(s32*)((s32)event + 0x7C),
            *(s32*)((s32)event + 0x78),
            float_0_804271c0,
            (f32)*(s32*)((s32)event + 0x80));
        color = dat_80427174;
        ((u8*)&color)[3] = alpha;
        *(u32*)((s32)entry + 0x28) = color;
        if (*(u8*)((s32)entry + 0x2B) == 0) {
            *(u16*)entry &= ~4;
        }
    }

    return (*(s32*)((s32)event + 0x7C) == 0) ? 2 : 0;
}

s32 _lvup_object_noselect_delete(void* event, s32 init) {
    void* work;
    s32 selected;
    s32 i;
    s32 offset;
    s32 alpha;

    extern f32 float_255_804271cc;
    extern f32 float_0_804271c0;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    selected = *(s32*)((s32)work + 0x10);

    if (init) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x80) = 0x1E;
        for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
            if (i != selected) {
                *(u8*)((s32)work + offset + 0x228) = 3;
            }
        }
    }

    *(s32*)((s32)event + 0x78) += 1;
    if (*(s32*)((s32)event + 0x78) < *(s32*)((s32)event + 0x80)) {
        for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
            if (i != selected) {
                alpha = (s32)intplGetValue(
                    5,
                    *(s32*)((s32)event + 0x78),
                    *(s32*)((s32)event + 0x80),
                    float_255_804271cc,
                    float_0_804271c0);
                *(u8*)((s32)work + offset + 0x233) = alpha;
            }
        }
        return 0;
    }

    for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
        if (i != selected) {
            *(u8*)((s32)work + offset + 0x233) = 0;
        }
    }
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_object_select_move | missing_definition | ghidra_signature */
s32 _lvup_object_select_move(void* event, s32 isFirstCall) {
    extern f32 float_0_804271c0;
    extern f32 float_2160_804271c8;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 reviseAngle(f32 angle);
    void* battleWork;
    void* work;
    void* mario;
    s32 selected;
    u8* entry;
    s32 timer;
    s32 total;

    battleWork = _battleWorkPointer;
    work = *(void**)((s32)battleWork + 0xF28);
    selected = *(s32*)((s32)work + 0x10);
    mario = BattleGetMarioPtr(battleWork);
    entry = (u8*)((s32)work + selected * 0x34 + 0x200);
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = 0x3C;
        *(s32*)((s32)event + 0x80) = (s32)*(f32*)entry;
        *(s32*)((s32)event + 0x84) = (s32)*(f32*)(entry + 8);
        *(s32*)((s32)event + 0x88) = (s32)*(f32*)((s32)mario + 0x3C);
        *(s32*)((s32)event + 0x8C) = (s32)*(f32*)((s32)mario + 0x44);
    }

    *(s32*)((s32)event + 0x78) += 1;
    timer = *(s32*)((s32)event + 0x78);
    total = *(s32*)((s32)event + 0x7C);
    if (timer < total) {
        *(f32*)entry = intplGetValue(5, timer, total, (f32)*(s32*)((s32)event + 0x80), (f32)*(s32*)((s32)event + 0x88));
        *(f32*)(entry + 8) = intplGetValue(5, timer, total, (f32)*(s32*)((s32)event + 0x84), (f32)*(s32*)((s32)event + 0x8C));
        *(f32*)(entry + 0x1C) = reviseAngle(intplGetValue(5, timer, total, float_0_804271c0, float_2160_804271c8));
        return 0;
    }

    *(f32*)entry = (f32)*(s32*)((s32)event + 0x88);
    *(f32*)(entry + 8) = (f32)*(s32*)((s32)event + 0x8C);
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_object_decide_bound | missing_definition | ghidra_signature */
s32 _lvup_object_decide_bound(void* event, s32 isFirstCall) {
    extern f32 float_0_804271c0;
    extern f32 float_1p5_804271c4;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void* effPokopiPchargeN64Entry(s32 type, s32 duration, s32 unused, f32 x, f32 y, f32 z, f32 scale);
    void* work;
    s32 selected;
    u8* entry;
    s32 timer;
    s32 half;
    s32 startY;

    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    selected = *(s32*)((s32)work + 0x10);
    entry = (u8*)((s32)work + selected * 0x34 + 0x200);
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = (s32)*(f32*)(entry + 4);
        *(s32*)((s32)event + 0x80) = 0x10;
        *(s32*)((s32)event + 0x84) = 0x1E;
        entry[0x28] = 2;
    }

    *(s32*)((s32)event + 0x78) += 1;
    timer = *(s32*)((s32)event + 0x78);
    if (timer < *(s32*)((s32)event + 0x80)) {
        half = *(s32*)((s32)event + 0x80) / 2;
        startY = *(s32*)((s32)event + 0x7C);
        if (timer < half) {
            *(f32*)(entry + 4) = (f32)startY + intplGetValue(5, timer, *(s32*)((s32)event + 0x84), float_0_804271c0, (f32)*(s32*)((s32)event + 0x84));
        } else {
            *(f32*)(entry + 4) = (f32)startY + intplGetValue(5, timer - half, *(s32*)((s32)event + 0x84), (f32)*(s32*)((s32)event + 0x84), float_0_804271c0);
        }
        return 0;
    }

    *(f32*)(entry + 4) = (f32)*(s32*)((s32)event + 0x7C);
    entry[0x28] = 4;
    effPokopiPchargeN64Entry(3, 0x78, 0, *(f32*)entry, *(f32*)(entry + 4), *(f32*)(entry + 8), float_1p5_804271c4);
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_select_object_init | missing_definition | ghidra_signature */
s32 _lvup_select_object_init(void) {
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_80427178;
    extern u32 dat_8042717c;
    extern u32 dat_80427180;
    void* pouch;
    u8* work;
    char* base;

    base = str_msg_menu_mario_name__802fe738;
    work = *(u8**)((s32)_battleWorkPointer + 0xF28);
    pouch = pouchGetPtr();

    *(u32*)(work + 0x200) = *(u32*)(base + 0x1F0);
    *(u32*)(work + 0x204) = *(u32*)(base + 0x1F4);
    *(u32*)(work + 0x208) = *(u32*)(base + 0x1F8);
    *(u32*)(work + 0x20C) = *(u32*)(base + 0x1FC);
    *(u32*)(work + 0x210) = *(u32*)(base + 0x200);
    *(u32*)(work + 0x214) = *(u32*)(base + 0x204);

    *(u32*)(work + 0x234) = *(u32*)(base + 0x208);
    *(u32*)(work + 0x238) = *(u32*)(base + 0x20C);
    *(u32*)(work + 0x23C) = *(u32*)(base + 0x210);
    *(u32*)(work + 0x240) = *(u32*)(base + 0x214);
    *(u32*)(work + 0x244) = *(u32*)(base + 0x218);
    *(u32*)(work + 0x248) = *(u32*)(base + 0x21C);

    *(u32*)(work + 0x268) = *(u32*)(base + 0x220);
    *(u32*)(work + 0x26C) = *(u32*)(base + 0x224);
    *(u32*)(work + 0x270) = *(u32*)(base + 0x228);
    *(u32*)(work + 0x274) = *(u32*)(base + 0x22C);
    *(u32*)(work + 0x278) = *(u32*)(base + 0x230);
    *(u32*)(work + 0x27C) = *(u32*)(base + 0x234);

    work[0x228] = 0;
    *(u32*)(work + 0x230) = dat_80427178;
    *(u16*)(work + 0x224) = *(s16*)((s32)pouch + 0x8E);
    *(u32*)(work + 0x218) = *(u32*)(base + 0x238);
    *(u32*)(work + 0x21C) = *(u32*)(base + 0x23C);
    *(u32*)(work + 0x220) = *(u32*)(base + 0x240);
    if (*(u16*)(work + 0x224) >= 0xC8) {
        *(u16*)(work + 0x226) = *(u16*)(work + 0x224);
    } else {
        *(u16*)(work + 0x226) = *(u16*)(work + 0x224) + 5;
    }

    work[0x25C] = 0;
    *(u32*)(work + 0x264) = dat_8042717c;
    *(u16*)(work + 0x258) = *(s16*)((s32)pouch + 0x90);
    *(u32*)(work + 0x24C) = *(u32*)(base + 0x244);
    *(u32*)(work + 0x250) = *(u32*)(base + 0x248);
    *(u32*)(work + 0x254) = *(u32*)(base + 0x24C);
    if (*(u16*)(work + 0x258) >= 0xC8) {
        *(u16*)(work + 0x25A) = *(u16*)(work + 0x258);
    } else {
        *(u16*)(work + 0x25A) = *(u16*)(work + 0x258) + 5;
    }

    work[0x290] = 0;
    *(u32*)(work + 0x298) = dat_80427180;
    *(u16*)(work + 0x28C) = *(s16*)((s32)pouch + 0x94);
    *(u32*)(work + 0x280) = *(u32*)(base + 0x250);
    *(u32*)(work + 0x284) = *(u32*)(base + 0x254);
    *(u32*)(work + 0x288) = *(u32*)(base + 0x258);
    if (*(u16*)(work + 0x28C) >= 0x63) {
        *(u16*)(work + 0x28E) = *(u16*)(work + 0x28C);
    } else {
        *(u16*)(work + 0x28E) = *(u16*)(work + 0x28C) + 3;
    }

    *(u32*)(work + 0x30) |= 8;
    return 2;
}

s32 _keta_get(s32 value) {
    s32 digits = 0;

    if (value < 0) {
        value *= -1;
    }
    if (value == 0) {
        return 1;
    }
    while (value > 0) {
        value /= 10;
        digits++;
    }
    return digits;
}


/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_select_object_disp | prototype_only | source_prototype */
void _lvup_select_object_disp(void) {
    typedef f32 Mtx[3][4];
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct ColorLocal {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } ColorLocal;
    typedef struct LvupObjectLocal {
        VecLocal translation;
        VecLocal scale;
        VecLocal rotation;
        u16 current;
        u16 maximum;
        u8 state;
        u8 timer;
        u8 pad_2a[2];
        s32 visible;
        ColorLocal color;
    } LvupObjectLocal;
    typedef struct SeqEndLocal {
        u8 pad_000[0x200];
        LvupObjectLocal object[3];
    } SeqEndLocal;

    extern void* _battleWorkPointer;
    extern void* gp;
    extern s32 hp_tex_tbl[6];
    extern s32 fp_tex_tbl[6];
    extern s32 bp_tex_tbl[6];

    extern void PSMTXIdentity(Mtx);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, s32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void btlDispTexPlane2(Mtx, s32, ColorLocal*, s32);
    extern void btlDispTexPlane(f32, f32, f32, f32, f32, s32, ColorLocal*, s32);
    extern void iconNumberDispGx3D(Mtx, s32, s32, ColorLocal*);
    extern void iconDispGxCol(Mtx, s32, s32, ColorLocal*);
    extern s32 _keta_get(s32);

    /* Constants not present in the accepted anonymous pool. */
    extern f32 float_8_80427194;
    extern f32 float_6_80427198;
    extern f32 float_1p2_804271ac;
    extern f32 float_3_804271b0;
    extern f32 float_4_804271bc;

    Mtx model;
    Mtx trans;
    Mtx scale;
    Mtx rotX;
    Mtx rotY;
    Mtx rotZ;
    Mtx currentMtx;
    Mtx maximumMtx;
    Mtx dividerMtx;
    Mtx overlayScale;
    VecLocal dividerPos;
    VecLocal currentPos;
    VecLocal maximumPos;
    ColorLocal color0a;
    ColorLocal color0b;
    ColorLocal color1a;
    ColorLocal color1b;
    ColorLocal color2a;
    ColorLocal color2b;
    ColorLocal currentColor;
    ColorLocal maximumColor;
    ColorLocal dividerColor;
    ColorLocal simpleColor;
    SeqEndLocal* seq;
    LvupObjectLocal* obj;
    s32 i;
    s32 offset;
    s32 count;
    s32 state;
    s32 digits;

    offset = 0;
    count = 3;
    seq = *(SeqEndLocal**)((u8*)_battleWorkPointer + 0xF28);

    do {
        obj = (LvupObjectLocal*)((u8*)seq + 0x200 + offset);
        state = obj->state;
        if (state == 5) {
            goto decrement_timer;
        }
        if (state < 5 && state > 3) {
            obj->state = 5;
            obj->timer = 1;
decrement_timer:
            obj->timer--;
            if (obj->timer == 0) {
                obj->timer = 8;
                if (obj->visible == 0) {
                    obj->visible = 1;
                } else {
                    obj->visible = 0;
                }
            }
        } else {
            obj->visible = 1;
        }
        offset += 0x34;
        count--;
    } while (count != 0);

    if (seq->object[0].visible != 0) {
        PSMTXIdentity(model);
        PSMTXTrans(trans,
                   seq->object[0].translation.x,
                   seq->object[0].translation.y,
                   seq->object[0].translation.z);
        PSMTXScale(scale,
                   seq->object[0].scale.x,
                   seq->object[0].scale.y,
                   seq->object[0].scale.z);
        PSMTXRotRad(rotX, 'x', 0.017453292f * seq->object[0].rotation.x);
        PSMTXRotRad(rotY, 'y', 0.017453292f * seq->object[0].rotation.y);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * seq->object[0].rotation.z);
        PSMTXConcat(rotZ, scale, model);
        PSMTXConcat(rotZ, model, model);
        PSMTXConcat(rotX, model, model);
        PSMTXConcat(rotY, model, model);
        PSMTXConcat(trans, model, model);

        color0a = seq->object[0].color;
        btlDispTexPlane2(model, 0x31, &color0a, 0);

        color0b = seq->object[0].color;
        btlDispTexPlane(
            seq->object[0].translation.x - 16.0f,
            20.0f + seq->object[0].translation.y,
            2.0f + seq->object[0].translation.z,
            seq->object[0].scale.x,
            seq->object[0].scale.y,
            hp_tex_tbl[*(s32*)((u8*)gp + 0x16C)],
            &color0b,
            0);
    }

    if (seq->object[1].visible != 0) {
        PSMTXIdentity(model);
        PSMTXTrans(trans,
                   seq->object[1].translation.x,
                   seq->object[1].translation.y,
                   seq->object[1].translation.z);
        PSMTXScale(scale,
                   seq->object[1].scale.x,
                   seq->object[1].scale.y,
                   seq->object[1].scale.z);
        PSMTXRotRad(rotX, 'x', 0.017453292f * seq->object[1].rotation.x);
        PSMTXRotRad(rotY, 'y', 0.017453292f * seq->object[1].rotation.y);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * seq->object[1].rotation.z);
        PSMTXConcat(rotZ, scale, model);
        PSMTXConcat(rotZ, model, model);
        PSMTXConcat(rotX, model, model);
        PSMTXConcat(rotY, model, model);
        PSMTXConcat(trans, model, model);

        color1a = seq->object[1].color;
        btlDispTexPlane2(model, 0x32, &color1a, 0);

        color1b = seq->object[1].color;
        btlDispTexPlane(
            seq->object[1].translation.x - 16.0f,
            20.0f + seq->object[1].translation.y,
            2.0f + seq->object[1].translation.z,
            seq->object[1].scale.x,
            seq->object[1].scale.y,
            fp_tex_tbl[*(s32*)((u8*)gp + 0x16C)],
            &color1b,
            0);
    }

    if (seq->object[2].visible != 0) {
        PSMTXIdentity(model);
        PSMTXTrans(trans,
                   seq->object[2].translation.x,
                   seq->object[2].translation.y,
                   seq->object[2].translation.z);
        PSMTXScale(scale,
                   seq->object[2].scale.x,
                   seq->object[2].scale.y,
                   seq->object[2].scale.z);
        PSMTXRotRad(rotX, 'x', 0.017453292f * seq->object[2].rotation.x);
        PSMTXRotRad(rotY, 'y', 0.017453292f * seq->object[2].rotation.y);
        PSMTXRotRad(rotZ, 'z', 0.017453292f * seq->object[2].rotation.z);
        PSMTXConcat(rotZ, scale, model);
        PSMTXConcat(rotZ, model, model);
        PSMTXConcat(rotX, model, model);
        PSMTXConcat(rotY, model, model);
        PSMTXConcat(trans, model, model);

        color2a = seq->object[2].color;
        btlDispTexPlane2(model, 0x33, &color2a, 0);

        color2b = seq->object[2].color;
        btlDispTexPlane(
            seq->object[2].translation.x - 16.0f,
            20.0f + seq->object[2].translation.y,
            2.0f + seq->object[2].translation.z,
            seq->object[2].scale.x,
            seq->object[2].scale.y,
            bp_tex_tbl[*(s32*)((u8*)gp + 0x16C)],
            &color2b,
            0);
    }

    i = 0;
    offset = 0;
    do {
        obj = (LvupObjectLocal*)((u8*)seq + 0x200 + offset);
        if (obj->visible != 0) {
            if (obj->state == 1) {
                if (obj->current != obj->maximum) {
                    currentPos.x = obj->translation.x - 14.0f;
                    currentPos.y =
                        obj->translation.y - (96.0f * obj->scale.y + 2.0f);
                    currentPos.z = obj->translation.z + 5.0f;
                    PSMTXScale(overlayScale, 0.8f, 0.8f, 1.0f);
                    PSMTXTrans(currentMtx, currentPos.x, currentPos.y, currentPos.z);
                    PSMTXConcat(currentMtx, overlayScale, currentMtx);
                    currentColor = obj->color;
                    iconNumberDispGx3D(currentMtx, obj->current, 0, &currentColor);

                    maximumPos = obj->translation;
                    digits = _keta_get(obj->maximum);
                    maximumPos.x =
                        float_1p2_804271ac * (f32)(digits * 8 + 6) + maximumPos.x;
                    maximumPos.y =
                        maximumPos.y - (96.0f * obj->scale.y + float_3_804271b0);
                    maximumPos.z = maximumPos.z + 5.0f;
                    PSMTXTrans(maximumMtx, maximumPos.x, maximumPos.y, maximumPos.z);
                    PSMTXScale(
                        overlayScale,
                        float_1p2_804271ac,
                        float_1p2_804271ac,
                        1.0f);
                    PSMTXConcat(maximumMtx, overlayScale, maximumMtx);
                    maximumColor = obj->color;
                    iconNumberDispGx3D(
                        maximumMtx, obj->maximum, 0, &maximumColor);

                    dividerPos = obj->translation;
                    dividerPos.y =
                        dividerPos.y - 96.0f * obj->scale.y;
                    dividerPos.z = dividerPos.z + 5.0f;
                    PSMTXTrans(
                        dividerMtx, dividerPos.x, dividerPos.y, dividerPos.z);
                    PSMTXScale(overlayScale, -0.6f, 0.6f, 1.0f);
                    PSMTXConcat(dividerMtx, overlayScale, dividerMtx);
                    dividerColor = obj->color;
                    iconDispGxCol(
                        dividerMtx, 0, 0x16B, &dividerColor);
                }
            } else if (obj->state == 2 || obj->state == 5) {
                maximumPos = obj->translation;
                digits = _keta_get(obj->maximum);
                maximumPos.y =
                    maximumPos.y - (96.0f * obj->scale.y + 2.0f);
                maximumPos.x =
                    maximumPos.x + ((f32)(digits * 4) - float_4_804271bc);
                maximumPos.z = maximumPos.z + 5.0f;
                PSMTXTrans(maximumMtx, maximumPos.x, maximumPos.y, maximumPos.z);
                simpleColor = obj->color;
                iconNumberDispGx3D(
                    maximumMtx, obj->maximum, 0, &simpleColor);
            }
        }
        i++;
        offset += 0x34;
    } while (i < 3);
}

void _check_audience_battle_end_exec(void) {
    void* battleWork;
    void* entries;
    s32 firstLevel;
    s32 rank;
    s32 maxLevel;
    s32 sum;
    s32 count;
    s32 i;
    void* unit;

    extern void BattleAudience_Case_GreatVictory(void);
    extern void BattleAudience_Case_FastVictory(f32 value);

    battleWork = _battleWorkPointer;
    entries = *(void**)((s32)*(void**)((s32)*(void**)((s32)battleWork + 0x2738) + 0xC) + 0x18);
    firstLevel = *(u8*)((s32)*(void**)*(void**)((s32)entries + 4) + 0xE);
    if (firstLevel > *(s16*)((s32)pouchGetPtr() + 0x8A) + 1) {
        BattleAudience_Case_GreatVictory();
    }

    rank = *(s16*)battleWork;
    maxLevel = *(s16*)((s32)pouchGetPtr() + 0x8A) + 1;
    sum = 0;
    count = *(s32*)entries;
    entries = *(void**)((s32)entries + 4);
    for (i = 0; i < count; i++) {
        unit = *(void**)entries;
        entries = (void*)((s32)entries + 0x30);
        sum += *(u8*)((s32)unit + 0xE);
    }

    count = ((sum * 2) / maxLevel) / 3;
    if (rank < count) {
        BattleAudience_Case_FastVictory((f32)(count - rank));
    }
}

void _audience_Whistle_control(void) {
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    if (work != NULL && *(s32*)work < 0x700001C && *(s32*)work >= 0x7000007) {
        s32 timer = *(s32*)((s32)work + 0x2A0) - 1;

        *(s32*)((s32)work + 0x2A0) = timer;
        if (timer < 0) {
            *(s32*)((s32)work + 0x2A0) = irand(120) + 60;
            BattleAudienceSoundWhistle();
        }
    }
}

void _BattleMarioKouraDelete(void) {
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    void* entry = evtEntry(_koura_delete_event, 10, 0);

    *(s32*)((s32)entry + 0x160) = *(s32*)mario;
}

s32 _check_guard_koura(void* event) {
    void* battleWork;
    s32* args;
    s32 value;
    s32 out;
    void* unit;
    void* guardKoura;

    extern s32 BattleTransID(void* event, s32 id);
    extern void* BtlUnit_GetGuardKouraPtr(void* unit);

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)event + 0x18);
    value = evtGetValue(event, args[0]);
    out = args[1];
    value = BattleTransID(event, value);
    unit = BattleGetUnitPtr(battleWork, value);

    if ((*(u32*)((s32)unit + 0x104) & 0x20) == 0) {
        evtSetValue(event, out, -1);
        return 2;
    }

    guardKoura = BtlUnit_GetGuardKouraPtr(unit);
    if (guardKoura == NULL) {
        evtSetValue(event, out, -1);
        return 2;
    }

    evtSetValue(event, out, *(s32*)guardKoura);
    return 2;
}

s32 _get_rank_up_msg(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 out = *(s32*)args;
    RankUpData* data = _get_rank_data(*(s16*)((s32)pouchGetPtr() + 0x8A));

    evtSetValue(event, out, data->message);
    return 2;
}
