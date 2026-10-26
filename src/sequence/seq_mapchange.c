#include "evt/evt_cmd.h"
#include "evt/evt_seq.h"
#include "manager/evtmgr.h"
#include "manager/filemgr.h"
#include "memory.h"
#include "mario/mario.h"
#include "driver/camdrv.h"
#include "driver/fadedrv.h"
#include "driver/animdrv.h"
#include "eff/eff_nice.h"
#include "evt/evt_bero.h"
#include "seq/seq_mapchange.h"
#include "data/mapdata.h"
#include "driver/bgdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "evt/evt_badgeshop.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "party.h"
#include "pmario_sound.h"
#include <stdio.h>
#include <string.h>
#include "evt/evt_lottery.h"
#include "database.h"
#include "driver/mobjdrv.h"
#include "cam_road.h"
#include "driver/seqdrv.h"
#include "statuswindow.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "mario/mario_sbr.h"
#include "driver/npcdrv.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_nannpc.h"

//.sbss
char _next_area[0x20];
char _next_map[0x20];
char _next_bero[0x20];

EVT_BEGIN(key_off)
CALL(evt_seq_wait, SEQ_GAME)
CALL(evt_fade_end_wait)
CALL(evt_mario_key_onoff, 1)
RETURN
EVT_END()

u8 rel_bss[0xF10];
void _load(const char* prevMapName, const char* newMapName, const char* beroName);

void evt_cam_road_reset(s32, s32);

static const char mc_tou[4];
static const char mc_tou2[5];
static const char mc_tou_03[7];
static const char mc_rel_fmt[14];
static const char mc_tst[4];
static const char mc_jon[4];

static inline const char* getAreaName(void) {
    const char* area_name = _next_area;
    if (!strcmp(_next_area, mc_tou)) {
        if (seqGetSeq() == SEQ_TITLE) {
            area_name = mc_tou2;
        } else if (!strcmp(_next_map, mc_tou_03)) {
            area_name = mc_tou2;
        }
    }
    return area_name;
}

void seq_mapChangeInit(SequenceWork* work) {
    strcpy(_next_area, "");
    strcpy(_next_map, "");
    strcpy(_next_bero, "");
    if (work->mapName) {
        strncpy(_next_area, work->mapName, 3);
        strncpy(_next_map, work->mapName, 0x20);
    }
    if (work->beroName) {
        strncpy(_next_bero, work->beroName, 0x20);
    }
    psndENVOff(0x200);
    psndENVOff(0x201);
    {
    int c0 = strcmp(gp->currentMapName, "");
    int c1 = strcmp(gp->currentMapName, "gor_01");
    int c2 = strcmp(_next_map, "gor_01");
    if (c0 && c1 && c2) {
        bdsw->restockFlags |= 1;
    } else if (!c2 && bdsw->restockFlags & 1) {
        badgeShop_bargainGeneration();
        badgeShop_bottakuruGeneration();
        bdsw->restockFlags &= 0xFFFE;
    }
    }
}

void seq_mapChangeExit(SequenceWork* work) {
    GXResetOverflowCount();
    dbg_lotteryinfo = FALSE;
}

#ifdef VERSION_G8ME01
typedef struct { const char* s[7]; } LangTbl;
static const LangTbl lang_tbl = {{"jp", "us", "ge", "fr", "sp", "it", "du"}};
#else
typedef struct { const char* s[2]; } LangTbl;
static const LangTbl lang_tbl = {{"jp", "us"}};
#endif
void seq_mapChangeMain(SequenceWork* work) {
    BOOL newarea = strcmp(_next_area, gp->currentAreaName) != 0;
    const char* area_name;
    EvtWord* evtCode;
    u32 fres;
    BOOL fok;
    switch (work->state) {
    case 0:
        if (strcmp(_next_map, "sys_01") == 0) {
            if (gp->areaFadeInType != FADE_OUT_BLACK) {
                fadeEntry(gp->areaFadeInType, gp->areaFadeInDuration, (GXColor){0, 0, 0, 0xFF});
            }
            fadeEntry(FADE_OUT_WHITE, 2000, (GXColor){0, 0, 0, 0xFF});
        } else if (!newarea) {
            fadeEntry(gp->mapFadeInType, gp->mapFadeInDuration, (GXColor){0, 0, 0, 0xFF});
        } else {
            fadeEntry(gp->areaFadeInType, gp->areaFadeInDuration, (GXColor){0, 0, 0, 0xFF});
        }
        gp->mapFadeInType = FADE_OUT_BLACK;
        gp->mapFadeInDuration = 300;
        gp->areaFadeInType = FADE_OUT_BLACK;
        gp->areaFadeInDuration = 300;
        psndSFXAllOff();
        if (!newarea) {
            gp->didAreaChange = FALSE;
        } else {
            gp->didAreaChange = TRUE;
        }
        if (strcmp(_next_map, mc_tou_03)) {
            area_name = getAreaName();
            fileAsyncf(0, 0, mc_rel_fmt, getMarioStDvdRoot(), area_name);
        }
        work->state++;
        break;
    case 1:
        psndSFXAllOff();
        if (fadeIsFinish()) {
            _relUnLoad();
            _unload(gp->currentMapName, _next_map, _next_bero);
            if (strcmp(_next_map, "dmo_00") && strcmp(_next_map, "title")) {
                if (OSGetSoundMode() == 0) {
                    SoundSetOutputMode(0);
                } else if (gp->soundMode == 0) {
                    SoundSetOutputMode(1);
                } else {
                    SoundSetOutputMode(2);
                }
            }
            work->state++;
        }
        break;
    case 2:
        if (strcmp(_next_map, "title") == 0) {
            strcpy(gp->currentAreaName + 0x10, gp->currentMapName);
            strcpy(gp->currentAreaName, "");
            strcpy(gp->currentMapName, "");
            seqSetSeq(SEQ_TITLE, 0, 0);
        } else {
            area_name = getAreaName();
            fres = fileAsyncf(0, 0, mc_rel_fmt, getMarioStDvdRoot(), area_name);
            if (fres == 0) {
                fok = FALSE;
            } else if ((s32)fres == -1) {
                fok = TRUE;
            } else {
                fok = TRUE;
            }
            if (fok) {
                if (mapPreLoad(_next_map)) {
                    FileEntry* file;
                    area_name = getAreaName();
                    file = fileAllocf(0, mc_rel_fmt, getMarioStDvdRoot(), area_name);
                    if (file) {
                        if (!strncmp(area_name, mc_tst, 3)) {
                            gp->relocationBase = _mapAlloc(((smartEntry*)file->data)->size);
                        } else if (!strncmp(area_name, mc_jon, 3)) {
                            gp->relocationBase = _mapAlloc(((smartEntry*)file->data)->size);
                        } else {
                            gp->relocationBase = gp->mapAlloc;
                        }
                        memcpy(gp->relocationBase, ((smartEntry*)file->data)->address, ((smartEntry*)file->data)->size);
                        fileFree(file);
                    }
                    if (gp->relocationBase) {
                        memset(rel_bss, 0, 0x3c4);
                        OSLink(gp->relocationBase, rel_bss);
                        ((void (*)(void))((OSModuleHeader*)gp->relocationBase)->prolog)();
                    }
                    _load(gp->currentMapName, _next_map, _next_bero);
                    work->state++;
                }
            }
        }
        break;
    case 3:
        if (setupDataCheck()) {
            if (psndGetFlag() & PSND_FLAG_BGM_WAIT_0) {
                if (psndBGMPlayTime(0) < 3000) break;
                psndClearFlag(PSND_FLAG_BGM_WAIT_0);
            }
            if (psndGetFlag() & PSND_FLAG_BGM_WAIT_1) {
                if (psndBGMPlayTime(1) < 3000) break;
                psndClearFlag(PSND_FLAG_BGM_WAIT_1);
            }
            setupDataBase(_next_area, _next_map);
            psndMapChange();
            evtCode = mapDataPtr(_next_map)->eventAddr;
            if (evtCode) {
                gp->mapEventId = evtEntry(evtCode, 0, 0)->eventId;
            } else {
                gp->mapEventId = 0;
            }
            work->state++;
        }
        break;
    case 4:
        if (gp->mapEventId == 0) {
            camCtrlOn(CAMERA_3D);
            work->state++;
        } else if (!evtCheckID(gp->mapEventId)) {
            camCtrlOn(CAMERA_3D);
            gp->mapEventId = 0;
            work->state++;
        }
        break;
    case 5:
        npcExecAllInitEvt();
        work->state++;
        break;
    case 6:
        if (!(camGetPtr(CAMERA_2D)->flags & CAMERA_FLAG_LETTER_BOX_OFF) && !(bero_get_BeroEXEC() & 1) &&
            (!marioChkKey() || !marioChkCtrl())) {
            camGetPtr(CAMERA_2D)->flags |= (CAMERA_FLAG_LETTER_BOX | CAMERA_FLAG_LETTER_BOX_INSTANT);
        }
        if (npcWaitAllInitEvtEnd()) {
            npcExecAllReglEvt();
            work->state++;
        }
        break;
    case 7: {
        MarioWork* mario;
        PartyEntry* party;
        PartyEntry* extraParty;
        if (strncmp(_next_map, "dmo", 3) && strncmp(_next_map, "dig", 3) && strncmp(_next_map, "end", 3) &&
            strncmp(_next_map, "sys", 3) && strncmp(_next_map, "gor_10", 6)) {
            {
            LangTbl lang = lang_tbl;
#ifdef VERSION_G8ME01
            fileAsyncf(4, 0, "%s/w/%s/win.tpl", getMarioStDvdRoot(), lang.s[gp->language]);
#else
            fileAsyncf(4, 0, "%s/window/%s/win.tpl", getMarioStDvdRoot(), lang.s[gp->language]);
#endif
            }
#ifdef VERSION_G8ME01
            animGroupBaseAsync("p_roll", 0, 0);
            animGroupBaseAsync("p_slit", 0, 0);
#endif
            animGroupBaseAsync("p_jyabara", 0, 0);
            animGroupBaseAsync("p_kaiten_h", 0, 0);
            animGroupBaseAsync("p_b_st", 0, 0);
            animGroupBaseAsync("OFF_d_maku_up_down", 0, 0);
            effNiceAsync(0);
            fileAsyncf(4, 0, "%s/battle/common/battle_common.tpl", getMarioStDvdRoot());
            fileAsyncf(4, 0, "%s/battle/audience/audience_normal.tpl", getMarioStDvdRoot());
            fileAsyncf(4, 0, "%s/battle/audience/audience_luigi.tpl", getMarioStDvdRoot());
        }
        if (gp->flags & 1) {
            mario = marioGetPtr();
            party = partyGetPtr(marioGetPartyId());
            extraParty = partyGetPtr(marioGetExtraPartyId());
            switch (swByteGet(4)) {
            case 0:
                mario->position = gp->savePlayerPos;
                evt_cam_road_reset(0, 0);
                if (party) {
                    party->position = gp->savePlayerPos;
                    party->position.x += 30.0f * (f32)cos(camGetPtr(CAMERA_3D)->viewYaw);
                    party->position.z += 30.0f * (f32)sin(camGetPtr(CAMERA_3D)->viewYaw);
                }
                if (extraParty) {
                    extraParty->position = gp->savePlayerPos;
                    extraParty->position.x += -30.0f * (f32)cos(camGetPtr(CAMERA_3D)->viewYaw);
                    extraParty->position.z += 30.0f * (f32)sin(camGetPtr(CAMERA_3D)->viewYaw);
                }
                if (!strcmp(gp->currentMapName, "dou_05") || !strcmp(gp->currentMapName, "gor_01") ||
                    !strcmp(gp->currentMapName, "muj_10") || !strcmp(gp->currentMapName, "tik_07")) {
                    mario->position.x -= 5.0f;
                    if (party) {
                        party->position = mario->position;
                        party->position.x -= 30.0f;
                        if (extraParty) {
                            extraParty->position = party->position;
                            extraParty->position.x -= 30.0f;
                        }
                    }
                }
                N_marioSetBottomlessRespawnPos(mario->position.x, 10.0f + mario->position.y, mario->position.z);
                marioKeyOff();
                evtEntryType(key_off, 0, 0, 0);
                break;
            default:
                fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 0});
                break;
            }
            statusWinForceUpdate();
            gp->flags &= ~1;
            marioStSystemLevel(0);
        }
        marioEntry();
        seqSetSeq(SEQ_GAME, 0, 0);
        break;
    }
    }
}

void _unload(const char* prevMap, const char* nextMap, const char* bero) {
    BOOL swReinit = FALSE;
    int i;

    if (prevMap && nextMap && strncmp(prevMap, nextMap, 3)) {
        swReinit = TRUE;
    }
    marioReInit();
    marioInitCamId();
    mapUnLoad();
    npcReset(0);
    extReset();
    mobjReset(0);
    offscreenReset(0);
    imgAutoRelease(0);
    effAutoRelease(0);
    animPoseAutoRelease(0);
    evtmgrReInit();
    caseReInit();
    itemReInit();
    iconReInit();
    windowReInit();
    winReInit();
    statusWinReInit();
    bgReInit();
    winMgrReInit();
    if (seqGetPrevSeq() != SEQ_LOAD && seqGetPrevSeq() != SEQ_TITLE && strcmp(prevMap, "sys_00") &&
        strcmp(prevMap, "sys_01") && strcmp(prevMap, "dig_00")) {
        fadeReset(1);
        gp->flags &= ~0x10;
    }
    for (i = 0; i < 4; i++) {
        padRumbleHardOff(i);
    }
    if (swReinit) {
        swReInit();
    }
    memClear(HEAP_MALLOC);
    memClear(HEAP_EXT);
    smartAutoFree(1);
    nanNPCWork = NULL;
}

void _load(const char* prevMapName, const char* newMapName, const char* beroName) {
    MapData* map;
    char mapFile[64];
    CameraParams roadOut;
    MarioWork* mw;
    CameraEntry* cam;

    map = mapDataPtr(newMapName);
    strcpy(gp->beroEnterName, beroName ? beroName : "");
    strcpy(gp->currentMapName, map->mapName);
    strncpy(gp->currentAreaName, map->mapName, 3);
    if (!strcmp(newMapName, "dmo_00")) {
        mapLoad("gor_01");
    } else {
        mapLoad(map->mapName);
    }
    camCtrlOff(CAMERA_3D);
    if (gp->debugBattle) {
        msgLoad("gon_11", 0);
    } else {
        memset(mapFile, 0, sizeof(mapFile));
        strncpy(mapFile, map->mapName, 6);
        msgLoad(mapFile, 0);
    }
    setupDataLoad(map->mapName);
    sprintf(mapFile, "./b/%s.tpl", newMapName);
    bgEntry(mapFile);
    if (gp->flags & 1) {
        MarioWork* mario;
        PartyEntry* party;
        PartyEntry* extraParty;

        mario = marioGetPtr();
        party = partyGetPtr(marioGetPartyId());
        extraParty = partyGetPtr(marioGetExtraPartyId());
        mario->camTarget = mario->position = gp->savePlayerPos;
        camRoadReset();
        camShiftReset();
        if (party) {
            party->position = gp->savePlayerPos;
        }
        if (extraParty) {
            extraParty->position = gp->savePlayerPos;
        }
    } else {
        Vec pos;
        f32 hx, hy, hz;
        f32 dist;
        f32 nx, ny, nz;
        f32 ang;
        int i;
        static const char* dir_str[8] = {"e", "ne", "n", "nw", "w", "sw", "s", "se"};
        if (beroName) {
            dist = 100000.0f;
            hitObjGetPos(beroName, &pos);
            for (i = 0; i < 8; i++) {
                if (!strncmp(dir_str[i], beroName, strlen(dir_str[i]))) {
                    ang = 3.141592f * (2.0f * (f32)i) / 8.0f;
                    pos.x += -30.0f * (f32)cos(ang);
                    pos.z += 30.0f * (f32)sin(ang);
                    break;
                }
            }
            if (i < 8) {
                hz = 0.0f;
                hy = 0.0f;
                hx = 0.0f;
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, pos.x, 10.0f + pos.y, pos.z, 0.0f, -1.0f, 0.0f)) {
                    Vec t = {0.0f, 0.0f, 0.0f};
                    t.x = hx;
                    t.y = hy;
                    t.z = hz;
                    marioGetPtr()->position = t;
                } else {
                    Vec t = {0.0f, 0.0f, 0.0f};
                    marioGetPtr()->position = t;
                }
            } else {
                Vec t = {0.0f, 0.0f, 0.0f};
                marioGetPtr()->position = t;
            }
        } else {
            Vec t = {0.0f, 0.0f, 0.0f};
            marioGetPtr()->position = t;
        }
    }
    mw = marioGetPtr();
    cam = camGetPtr(CAMERA_3D);
    cam->marioPos.x = mw->position.x;
    cam->marioPos.y = 0.01f + mw->position.y;
    cam->marioPos.z = mw->position.z;
    cam->marioAt.x = cam->marioPos.x;
    cam->marioAt.y = cam->marioPos.y;
    cam->marioAt.z = cam->marioPos.z;
    mw->camTarget = mw->position;
    camRoadReset();
    camShiftReset();
    if (cam->flags & CAMERA_FLAG_ROAD) {
        camRoadMain(&roadOut, 1.0f + cam->marioPos.x, 1.0f + cam->marioPos.y, 1.0f + cam->marioPos.z,
                    1.0f + cam->marioAt.x, 1.0f + cam->marioAt.y, 1.0f + cam->marioAt.z, 0.0f);
    }
}

static const char mc_tou_03[] = "tou_03";
static const char mc_rel_fmt[] = "%s/rel/%s.rel";
static const char mc_tou[] = "tou";
static const char mc_tou2[] = "tou2";
static const char mc_tst[] = "tst";
static const char mc_jon[] = "jon";

void _relUnLoad(void) {
    if (gp->relocationBase) {
        ((void (*)(void))((OSModuleHeader*)gp->relocationBase)->epilog)();
        OSUnlink(gp->relocationBase);
        if (gp->relocationBase != gp->mapAlloc) {
            _mapFree(gp->relocationBase);
        }
        gp->relocationBase = NULL;
    }
}
