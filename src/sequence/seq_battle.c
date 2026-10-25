#ifdef VERSION_G8ME01
void N_battleMapFree(void);
void N_battleMapAlloc(void);
#endif
#include "battle/battle.h"
#include "battle/battle_information.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "driver/extdrv.h"
#include "driver/fadedrv.h"
#include "driver/icondrv.h"
#include "driver/imgdrv.h"
#include "driver/itemdrv.h"
#include "driver/msgdrv.h"
#include "driver/npcdrv.h"
#include "driver/lightdrv.h"
#include "driver/offscreendrv.h"
#include "driver/seqdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/filemgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "memory.h"
#include "party.h"
#include "pmario_sound.h"
#include "seq/seq_title.h"
#include "system.h"
#include <dolphin/mtx.h>

typedef struct BattleSeqWork {
    s32 sequence;   // 0x0
    s32 state;      // 0x4
    u8 unk8[8];     // 0x8
    s32 counter;    // 0x10
} BattleSeqWork;

BOOL bmapLoad(const char* a, const char* b);
void bmapUnLoad(void);
BattleUnitType BattleTransPartyIdToUnitKind(s32 id);
void psndBGMOn_f(u32 a, u32 b, u32 c, u32 d);
BOOL psndENVOn(u32 a, char* b);
void mobjReset(BOOL inBattle);

s32 debug_battle_flag;
s32 seq_battle_unk_8041874c;


static inline BOOL loadTpl(const char* path) {
    return (u32)fileAsyncf(4, NULL, "%s/%s", getMarioStDvdRoot(), path) != 0;
}
#define GSW0 (-170000000)

void seq_battleInit(BattleSeqWork* work) {
    FieldBattleData* fbat;
    NpcEntry* npc;

    debug_battle_flag = gp->debugBattle;
    fbat = fbatGetPointer();
    npc = fbat->target;
    if (evtGetValue(NULL, GSW0) < 109 || evtGetValue(NULL, GSW0) >= 111) {
        if (evtGetValue(NULL, GSW0) != 402) {
            psndBGMOff(0x400);
            psndBGMOn(1, (char*)npc->battleInfo.bgmName);
        }
    }
    psndENVOff(0x400);
    padRumbleOff(0);
    evtStopAll(4);
    psndSetFlag(0x80);
    switch (fbatGetPointer()->hitKind) {
        case 0x1000000:
        case 0x800000:
            if (debug_battle_flag == 0) {
                marioCtrlOff();
                partyCtrlOff();
                padRumbleOff(0);
            }
            break;
    }
}

void seq_battleExit(BattleSeqWork* work) {
    battle_exit();
}

static const char gon11[7];
static const char bti01[7];
static const char bti02[7];
static const char bti03[7];
static const char bti04[7];

void seq_battleMain(BattleSeqWork* work) {
    FieldBattleData* fbat;
    BattleStageData* stage;
    PouchData* pouch;

    switch (work->state) {
        case 0: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(0x16, 1000, color);
            work->state++;
            work->counter = 0;
            break;
        }
        case 1:
            if (!loadTpl("battle/audience/audience_normal.tpl")) {
                break;
            }
            if (!loadTpl("battle/audience/audience_luigi.tpl")) {
                break;
            }
            work->counter++;
            if (fadeIsFinish()) {
                work->state++;
            }
            break;
        case 2:
            pouchGetPtr();
            switch (fbatGetPointer()->hitKind) {
                case 0x1000000:
                case 0x800000:
                    if (debug_battle_flag == 0) {
                        marioGetPtr()->flags &= ~MARIO_FLAG_ACTIVE;
                    }
                    break;
                default:
                    if (debug_battle_flag == 0) {
                        marioCtrlOff();
                        partyCtrlOff();
                        marioGetPtr()->flags &= ~MARIO_FLAG_ACTIVE;
                        padRumbleOff(0);
                    }
                    break;
            }
            fbat = fbatGetPointer();
            fbat->battleWait = 0;
            work->state++;
        case 3: {
            BOOL wait;
            fbat = fbatGetPointer();
            stage = fbat->target->battleInfo.stageData;
            pouch = pouchGetPtr();
            if (fbat->firstStrike.enabled != 0) {
                BOOL ok = TRUE;
                switch (fbat->firstStrike.type) {
                    case 1:
                    case 2:
                        if (fbat->firstStrike.hitKind != 0) {
                            ok = FALSE;
                        }
                        break;
                }
                if (ok == FALSE) {
                    fbat->battleWait++;
                    if (fbat->battleWait < 200) {
                        wait = TRUE;
                        goto check;
                    }
                }
            }
            switch (pouch->rank) {
                case 0:
                    bmapLoad(bti01, stage->mSpecificStageDataDir);
                    break;
                case 1:
                    bmapLoad(bti02, stage->mSpecificStageDataDir);
                    break;
                case 2:
                    bmapLoad(bti03, stage->mSpecificStageDataDir);
                    break;
                case 3:
                default:
                    bmapLoad(bti04, stage->mSpecificStageDataDir);
                    break;
            }
            wait = FALSE;
        check:
            if (wait != 0) {
                break;
            }
            work->state++;
        }
        case 4:
            if (battle_init()) {
                break;
            }
            work->state++;
        case 5: {
            Vec pos;
            if (battleSeqEndCheck()) {
                work->counter = 0;
                work->state++;
                if (gp->debugBattle) {
                    work->state = 11;
                }
            }
            marioGetPtr();
            camGetPtr(CAMERA_3D);
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 400.0f;
            if (_battleWorkPointer->flags & 0x100) {
                pos.z += 200.0f;
            }
            psndSetPosDirListener(&pos, camGetPtr(CAMERA_3D)->viewYaw);
            break;
        }
        case 6:
            if (++work->counter > 30) {
                if ((gp->flags & 0x1000) && (gp->flags & 0x2000)) {
                    seqSetSeq(SEQ_GAME_OVER, (const char*)1, NULL);
                } else {
                    seqSetSeq(SEQ_GAME, (const char*)1, NULL);
                }
            }
            break;
        case 10: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(0x16, 200, color);
            work->state++;
            break;
        }
        case 11:
            if (fadeIsFinish()) {
                gp->debugBattle = 0;
                seqSetSeq(SEQ_GAME_OVER, (const char*)1, NULL);
            }
            break;
    }
}

BOOL battle_init(void) {
    FieldBattleData* fbat;
    NpcEntry* npc;
    PouchData* pouch;
    FieldBattleInfo* info;
    PartyEntry* party;

    fbat = fbatGetPointer();
    npc = fbat->target;
    pouch = pouchGetPtr();
    camSetMode(4, 2);
    if (gp->debugBattle) {
        const char* msg = DbgBtlSel_GetMsgDataPtr();
        if (msg) {
            msgLoad(msg, 0);
        } else {
            msgLoad(gon11, 0);
        }
    }
#ifdef VERSION_G8ME01
    N_battleMapAlloc();
#else
    memClear(4);
    smartAutoFree(2);
#endif
    _battleWorkPointer = NULL;
    animPoseBattleInit();
    npcReleaseFieldNpc();
    gp->inBattle = 1;
    evtmgrReInit();
    psndSetFlag(0x10);
    info = &fbat->info;
    BattleInformationInit(info);
    BattleInformationSetMode(info, 0);
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        s32 id = party->currentMemberId;
        BattleUnitType kind = BattleTransPartyIdToUnitKind(id);
        if (!(pouch->partyData[id].flags & 2)) {
            BattleInformationSetParty(info, kind);
        }
    }
    if (fbat->firstAttackFlags & 1) {
        BattleInformationSetFirstAttack(info, 0);
    } else {
        switch (fbat->hitKind) {
            case 0x20000:
                BattleInformationSetFirstAttack(info, 1);
                break;
            case 0x40000:
                BattleInformationSetFirstAttack(info, 2);
                break;
            case 0x80000:
                BattleInformationSetFirstAttack(info, 3);
                break;
            case 0x100000:
                BattleInformationSetFirstAttack(info, 4);
                break;
            case 0x200000:
                BattleInformationSetFirstAttack(info, 5);
                break;
            case 0x400000:
                BattleInformationSetFirstAttack(info, 6);
                break;
            case 0x800000:
                BattleInformationSetFirstAttack(info, 7);
                break;
            case 0x1000000:
                BattleInformationSetFirstAttack(info, 8);
                break;
            case 0x10000000:
                BattleInformationSetFirstAttack(info, fbat->firstAttackParam);
                break;
            default:
                BattleInformationSetFirstAttack(info, 0);
                break;
        }
    }
    BattleInfomationSetBattleSetupInfo(info, &npc->battleInfo);
    info->ruleType = npc->battleInfo.battleRule[0];
    info->ruleParam = npc->battleInfo.battleRule[1];
    info->ruleParam2 = npc->battleInfo.battleRule[2];
    BattleInit(info);
    return FALSE;
}

void battle_exit(void) {
    LightEntry* light;
    Mtx mtx;

    BattleEnd();
#ifdef VERSION_G8ME01
    N_battleMapFree();
    smartAutoFree(2);
#endif
    itemReInit();
    iconReInit();
    extReset();
    bmapUnLoad();
    if (camGetPtr(CAMERA_3D)->flags & 4) {
        camSetMode(4, 1);
    } else {
        camSetMode(4, 3);
    }
    npcReset(TRUE);
    mobjReset(TRUE);
    offscreenReset(TRUE);
    imgAutoRelease(1);
    effAutoRelease(TRUE);
    animPoseAutoRelease(TRUE);
    evtmgrReInit();
    npcRecoveryFieldNpc();
    gp->inBattle = 0;
#ifndef VERSION_G8ME01
    memClear(4);
#endif
    evtStartAll(4);
    if (debug_battle_flag == 0) {
        marioCtrlOn();
        partyCtrlOn();
        marioGetPtr()->flags |= MARIO_FLAG_ACTIVE;
    }
    L_psndBGM_stop();
    psndClearFlag(0x80);
    light = lightGetPaperCraft();
    { Vec v = {-200.0f, 100.0f, 100.0f}; light->position = v; }
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, &light->position, &light->position);
    if (evtGetValue(NULL, GSW0) >= 109 && evtGetValue(NULL, GSW0) < 111) {
        psndBGMOn_f(0x120, 0, 0x2EE, 1);
    } else if (evtGetValue(NULL, GSW0) != 402 && seqGetNextSeq() != 5) {
        psndBGMOn(0x120, NULL);
        psndENVOn(0x120, NULL);
    }
}

static const char gon11[] = "gon_11";
static const char bti01[] = "bti_01";
static const char bti02[] = "bti_02";
static const char bti03[] = "bti_03";
static const char bti04[] = "bti_04";
