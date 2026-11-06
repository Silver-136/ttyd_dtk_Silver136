// Battle sequence: loads the battle REL and map, runs the battle and returns to the field.

#include "version.h"
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
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "seq/seq_battle.h"
static void battle_exit(void);
#if VERSION >= VERSION_JP
static BOOL battle_init(void);
#endif

#if VERSION <= VERSION_DEMO_JP
#include "data/mapdata.h"

static void battle_init(void);
BOOL bmapPreLoad(const char* map, const char* map2);
static const char bti01[7];
static const char bti02[7];
static const char bti03[7];
static const char bti04[7];
#endif

s32 debug_battle_flag;
// FAKE: unreferenced word in neither symbol map; removing it changes the object
s32 seq_battle_unk_8041874c;


/** Requests an async load of a TPL under the disc root; true once the file is resident (seq_battleMain polls it). */
static inline BOOL loadTpl(const char* path) {
    // FAKE: the (u32) cast; comparing the s32 result directly changes the object
    return (u32)fileAsyncf(FILE_TYPE_TPL, NULL, "%s/%s", getMarioStDvdRoot(), path) != 0;
}

/**
 * Enters the battle sequence: switches to the enemy's battle BGM (kept during the Great Tree time-bomb countdown
 * and after the final battle), stops field events and rumble and mutes field sound effects.
 * Later versions take Mario's control here for hit kinds 0x800000/0x1000000;
 * the early demos start preloading the audience map for Mario's rank.
 */
void seq_battleInit(SequenceWork* work) {
    FieldBattleData* fbat;
    NpcEntry* npc;
#if VERSION <= VERSION_DEMO_JP
    MapData* map;
#endif

    debug_battle_flag = gp->debugBattle;
#if VERSION <= VERSION_DEMO_JP
    // the early demos start reading the battle map now
    fbatGetPointer();
    switch (pouchGetPtr()->rank) {
        case 0:
            map = mapDataPtr(bti01);
            break;
        case 1:
            map = mapDataPtr(bti02);
            break;
        case 2:
            map = mapDataPtr(bti03);
            break;
        case 3:
        default:
            map = mapDataPtr(bti04);
            break;
    }
    bmapPreLoad(map->mapName, NULL);
#endif
    fbat = fbatGetPointer();
    npc = fbat->target;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG2_23_TIME_BOMB_ACTIVATED || evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG2_24_BOSS_BATTLE) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) != STG8_END_AFTER_THE_BATTLE) {
            psndBGMOff(PSND_PAUSE);
            psndBGMOn(PSND_SLOT(1), (char*)npc->battleInfo.bgmName);
        }
    }
    psndENVOff(PSND_PAUSE);
    padRumbleOff(0);
    evtStopAll(4);
    psndSetFlag(PSND_FLAG_MUTE_SFX);
#if VERSION >= VERSION_JP
    switch (fbatGetPointer()->hitKind) {
        case FBAT_HIT_BOMB:
        case FBAT_HIT_SHELL:
            if (debug_battle_flag == 0) {
                marioCtrlOff();
                partyCtrlOff();
                padRumbleOff(0);
            }
            break;
    }
#endif
}

/** Leaves the battle sequence; battle_exit tears the battle down and restores the field. */
void seq_battleExit(SequenceWork* work) {
    battle_exit();
}

static const char gon11[7];
static const char bti01[7];
static const char bti02[7];
static const char bti03[7];
static const char bti04[7];

/**
 * Per-frame battle sequence: curtain fade, audience texture loads,
 * waits out a pending first strike (up to 200 frames), loads the bti_01..04 audience stage for Mario's rank,
 * starts the battle via battle_init, then once the battle ends returns to the field (or game over on a kiosk
 * timeout; states 10/11 end a debug battle).
 */

// A non-NULL map name tells seq_gameOver to skip its iris fade (the battle has already
// faded out); seq_game does not read it.
#if VERSION >= VERSION_JP
void seq_battleMain(SequenceWork* work) {
    FieldBattleData* fbat;
    BattleStageData* stage;
    PouchData* pouch;
    Vec pos;

    switch (work->state) {
        case 0: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(FADE_CURTAIN_UP_DOWN_A1, 1000, color);
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
                case FBAT_HIT_BOMB:
                case FBAT_HIT_SHELL:
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
            // FAKE: wait flag and goto check; a plain break while still waiting changes the object
            BOOL wait;
            fbat = fbatGetPointer();
            stage = fbat->target->battleInfo.stageData;
            pouch = pouchGetPtr();
            if (fbat->firstStrike.enabled != 0) {
                BOOL ok = TRUE;
                switch (fbat->firstStrike.type) {
                    case FIRST_STRIKE_ENEMY:
                    case FIRST_STRIKE_MARIO:
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
                    bmapLoad(bti01, stage->specificStageDataDir);
                    break;
                case 1:
                    bmapLoad(bti02, stage->specificStageDataDir);
                    break;
                case 2:
                    bmapLoad(bti03, stage->specificStageDataDir);
                    break;
                case 3:
                default:
                    bmapLoad(bti04, stage->specificStageDataDir);
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
        case 5:
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
            if (_battleWorkPointer->flags & BATTLE_FLAG_PARTY_IN_AUDIENCE) {
                pos.z += 200.0f;
            }
            psndSetPosDirListener(&pos, camGetPtr(CAMERA_3D)->viewYaw);
            break;
        case 6:
            if (++work->counter > 30) {
                if ((gp->flags & GW_FLAG_E3) && (gp->flags & GW_FLAG_E3_TIMEOUT)) {
                    seqSetSeq(SEQ_GAME_OVER, (const char*)1, NULL);
                } else {
                    seqSetSeq(SEQ_GAME, (const char*)1, NULL);
                }
            }
            break;
        case 10: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(FADE_CURTAIN_UP_DOWN_A1, 200, color);
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

/** Later versions: same setup, returning FALSE once done (seq_battleMain state 4 retries while TRUE). */
static BOOL battle_init(void) {
#else
void seq_battleMain(SequenceWork* work) {
    switch (work->state) {
        case 0: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(FADE_CURTAIN_UP_DOWN_A1, 1000, color);
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
                battle_init();
                work->state++;
        case 3: {
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
            if (_battleWorkPointer->flags & BATTLE_FLAG_PARTY_IN_AUDIENCE) {
                pos.z += 200.0f;
            }
            psndSetPosDirListener(&pos, camGetPtr(CAMERA_3D)->viewYaw);
            break;
        }
        case 4:
            if (++work->counter > 30) {
                if ((gp->flags & GW_FLAG_E3) && (gp->flags & GW_FLAG_E3_TIMEOUT)) {
                    seqSetSeq(SEQ_GAME_OVER, (const char*)1, NULL);
                } else {
                    seqSetSeq(SEQ_GAME, (const char*)1, NULL);
                }
            }
            break;
        case 10: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(FADE_CURTAIN_UP_DOWN_A1, 200, color);
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

/** The early demos: performs the battle setup, including the stage map load. */
static void battle_init(void) {
#endif

    FieldBattleData* fbat;
    NpcEntry* npc;
    PouchData* pouch;
#if VERSION <= VERSION_DEMO_JP
    MapData* map;
    MapData* map2;
    BattleStageData* stage;
#endif
    FieldBattleInfo* info;
    PartyEntry* party;

    fbat = fbatGetPointer();
    npc = fbat->target;
#if VERSION <= VERSION_DEMO_JP
    stage = npc->battleInfo.stageData;
    pouch = pouchGetPtr();
    if (debug_battle_flag == 0) {
        marioCtrlOff();
        partyCtrlOff();
        marioGetPtr()->flags &= ~MARIO_FLAG_ACTIVE;
        padRumbleOff(0);
    }
    switch (pouch->rank) {
        case 0:
            map = mapDataPtr(bti01);
            break;
        case 1:
            map = mapDataPtr(bti02);
            break;
        case 2:
            map = mapDataPtr(bti03);
            break;
        case 3:
        default:
            map = mapDataPtr(bti04);
            break;
    }
    map2 = NULL;
    if (stage->specificStageDataDir) {
        map2 = mapDataPtr(stage->specificStageDataDir);
    }
    bmapLoad(map->mapName, stage->specificStageDataDir);
#else
    pouch = pouchGetPtr();
#endif
    camSetMode(CAMERA_3D, CAM_MODE_BATTLE);
    if (gp->debugBattle) {
        const char* msg = DbgBtlSel_GetMsgDataPtr();
        if (msg) {
            msgLoad(msg, 0);
        } else {
            msgLoad(gon11, 0);
        }
    }
#if VERSION >= VERSION_US
    battleMapAlloc();
#else
    memClear(HEAP_BATTLE);
    smartAutoFree(SMART_GROUP_BATTLE);
#endif
    _battleWorkPointer = NULL;
    animPoseBattleInit();
    npcReleaseFiledNpc();
    gp->inBattle = 1;
    evtmgrReInit();
#if VERSION <= VERSION_DEMO_JP
    if (map && map->eventAddr) {
        evtEntry(map->eventAddr, 0, 0);
    }
    if (map2 && map2->eventAddr) {
        evtEntry(map2->eventAddr, 0, 0);
    }
#endif
    psndSetFlag(PSND_FLAG_BATTLE_START);
    info = &fbat->info;
    BattleInformationInit(info);
    BattleInformationSetMode(info, 0);
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        s32 id = party->currentMemberId;
        BattleUnitType kind = BattleTransPartyIdToUnitKind(id);
        if (!(pouch->partyData[id].flags & POUCH_PARTY_FLAG_AWAY)) {
            BattleInformationSetParty(info, kind);
        }
    }
    if (fbat->firstAttackFlags & 1) {
        BattleInformationSetFirstAttack(info, FIRST_ATTACK_NONE);
    } else {
        switch (fbat->hitKind) {
            case FBAT_HIT_JUMP:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_JUMP);
                break;
            case FBAT_HIT_HIP:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_SPIN_JUMP);
                break;
            case FBAT_HIT_JUMP_UNUSED:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_JUMP_UNUSED);
                break;
            case FBAT_HIT_HAMMER:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_HAMMER);
                break;
            case FBAT_HIT_SPIN_HAMMER:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_SPIN_HAMMER);
                break;
            case FBAT_HIT_ULTRA_SPIN_HAMMER:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_ULTRA_HAMMER);
                break;
            case FBAT_HIT_SHELL:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_SHELL);
                break;
            case FBAT_HIT_BOMB:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_BOMB);
                break;
            case FBAT_HIT_ENEMY_FIRST:
                BattleInformationSetFirstAttack(info, fbat->firstAttackParam);
                break;
            default:
                BattleInformationSetFirstAttack(info, FIRST_ATTACK_NONE);
                break;
        }
    }
    BattleInfomationSetBattleSetupInfo(info, &npc->battleInfo);
    info->ruleType = npc->battleInfo.battleRule[0];
    info->ruleParam = npc->battleInfo.battleRule[1];
    info->ruleParam2 = npc->battleInfo.battleRule[2];
    BattleInit(info);
#if VERSION >= VERSION_JP
    return FALSE;
#endif
}

/**
 * Tears the battle down and restores the field:
 * frees battle data and drivers, restores the camera mode and field NPCs,
 * resumes field events and Mario's control, resets the paper-craft light,
 * and resumes the field BGM and ambience unless heading to game over (or, in later versions,
 * resumes the time-bomb music in the Great Tree).
 */
static void battle_exit(void) {
    LightEntry* light;
    Mtx mtx;

    BattleEnd();
#if VERSION >= VERSION_US
    battleMapFree();
    smartAutoFree(SMART_GROUP_BATTLE);
#endif
    itemReInit();
    iconReInit();
    extReset();
    bmapUnLoad();
    if (camGetPtr(CAMERA_3D)->flags & CAMERA_FLAG_ROAD) {
        camSetMode(CAMERA_3D, CAM_MODE_FIELD);
    } else {
        camSetMode(CAMERA_3D, CAM_MODE_NO_ROAD);
    }
    npcReset(TRUE);
    mobjReset(TRUE);
    offscreenReset(TRUE);
    imgAutoRelease(1);
    effAutoRelease(TRUE);
    animPoseAutoRelease(TRUE);
    evtmgrReInit();
    npcRecoveryFiledNpc();
    gp->inBattle = 0;
#if VERSION < VERSION_US
    memClear(HEAP_BATTLE);
#endif
    evtStartAll(4);
    if (debug_battle_flag == 0) {
        marioCtrlOn();
        partyCtrlOn();
        marioGetPtr()->flags |= MARIO_FLAG_ACTIVE;
    }
#if VERSION <= VERSION_DEMO_JP
    psndClearFlag(PSND_FLAG_BATTLE_START);
#else
    L_psndBGM_stop();
#endif
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    light = lightGetPaperCraft();
    {
        Vec v = {-200.0f, 100.0f, 100.0f};
        light->position = v;
    }
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, &light->position, &light->position);
#if VERSION <= VERSION_DEMO_JP
    if ((evtGetValue(NULL, GSW_STORY_PROGRESS) < STG2_23_TIME_BOMB_ACTIVATED || evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG2_24_BOSS_BATTLE) && evtGetValue(NULL, GSW_STORY_PROGRESS) != STG8_END_AFTER_THE_BATTLE && seqGetNextSeq() != SEQ_GAME_OVER) {
#else
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG2_23_TIME_BOMB_ACTIVATED && evtGetValue(NULL, GSW_STORY_PROGRESS) < STG2_24_BOSS_BATTLE) {
        psndBGMOn_f(PSND_CONTINUE | PSND_FADEIN, 0, 0x2EE, 1);
    } else if (evtGetValue(NULL, GSW_STORY_PROGRESS) != STG8_END_AFTER_THE_BATTLE && seqGetNextSeq() != SEQ_GAME_OVER) {
#endif
        psndBGMOn(PSND_CONTINUE | PSND_FADEIN, NULL);
        psndENVOn(PSND_CONTINUE | PSND_FADEIN, NULL);
    }
}

/**
 * gon_11, whose message file a debug battle loads when its menu has none, and the battle stage maps for
 * Mario's rank (bti_01 to bti_04, the stage gets bigger as he rises).
 */
#if VERSION >= VERSION_JP
static const char gon11[] = "gon_11";
#endif
/**
 * Names of the battle stage maps for Mario's rank (bti_01 to bti_04, the stage gets bigger as he rises),
 * and gon_11, whose message file a debug battle loads when its menu has none.
 */
static const char bti01[] = "bti_01";
static const char bti02[] = "bti_02";
static const char bti03[] = "bti_03";
static const char bti04[] = "bti_04";
#if VERSION <= VERSION_DEMO_JP
static const char gon11[] = "gon_11";
#endif
