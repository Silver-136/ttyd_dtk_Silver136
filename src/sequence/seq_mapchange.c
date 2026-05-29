#include "sequence/seq_mapchange.h"

// Types

typedef struct SeqMapVec {
    f32 x;
    f32 y;
    f32 z;
} SeqMapVec;

typedef struct SeqMapVecRaw {
    u32 x;
    u32 y;
    u32 z;
} SeqMapVecRaw;

// Data

extern s32 gp;
extern void* mapalloc_base_ptr;
extern char rel_bss[];
extern void* nanNPCWork;
extern char* dir_str[8];
extern s32 bdsw;
extern s32 dbg_lotteryinfo;

extern s32 zero_8041f508;
extern char lbl_802BF268[];
extern char* dir_str[];

extern s32 _next_area;
extern s32 _next_map;
extern s32 _next_bero;

extern char str_gor_01_802bf2d4[];
extern s32 str_title_8041f538;
extern s32 str_tou_8041f4f4;
extern s32 str_tou2_8041f4f8;
extern s32 str_tst_8041f500;
extern s32 str_jon_8041f504;
extern s32 str_dmo_8041f540;
extern s32 str_dig_8041f544;
extern s32 str_end_8041f548;
extern s32 str_sys_8041f54c;
extern s32 key_off;
extern s32 unk_80429500;

extern u32 dat_8041f4c4;
extern u32 dat_8041f4c8;
extern u32 dat_8041f4cc;
extern u32 dat_8041f4d0;

extern const f32 float_1E05_8041f50c;
extern const f32 float_3p1416_8041f510;
extern const f32 float_2_8041f514;
extern const f32 float_0p125_8041f518;
extern const f32 float_neg30_8041f51c;
extern const f32 float_30_8041f520;
extern const f32 float_0_8041f524;
extern const f32 float_10_8041f528;
extern const f32 float_neg1_8041f52c;
extern const f32 float_0p01_8041f530;
extern const f32 float_1_8041f534;
extern const f32 float_5_8041f550;

extern const SeqMapVec vec3_802bf290;
extern const SeqMapVec vec3_802bf29c;
extern const SeqMapVec vec3_802bf2a8;

// Standard/library helpers

extern char* strcpy(char* dst, const char* src);
extern char* strncpy(char* dst, const char* src, size_t count);
extern s32 strcmp(const char* a, const char* b);
extern s32 strncmp(const char* a, const char* b, size_t count);
extern size_t strlen(const char* str);
extern s32 sprintf(char* dst, const char* fmt, ...);
extern void* memset(void* dst, int value, size_t size);
extern void* memcpy(void* dst, const void* src, size_t size);

extern f64 sin(f64 x);
extern f64 cos(f64 x);

// External functions

extern void OSUnlink(void* module);
extern void OSLink(void* module, void* bss);
extern void _mapFree(void* heap, void* ptr);
extern void* _mapAlloc(void* heap, u32 size);

extern void psndENVOff(s32 envId);
extern void psndSFXAllOff(void);
extern u32 psndGetFlag(void);
extern s32 psndBGMPlayTime(s32 id);
extern void psndClearFlag(u32 flags);
extern void psndMapChange(void);

extern void badgeShop_bargainGeneration(void);
extern void badgeShop_bottakuruGeneration(void);

extern void marioReInit(void);
extern void marioInitCamId(void);
extern void* marioGetPtr(void);
extern s32 marioGetPartyId(void);
extern s32 marioGetExtraPartyId(void);
extern s32 marioChkKey(void);
extern s32 marioChkCtrl(void);
extern void marioKeyOff(void);
extern void marioStSystemLevel(s32 level);
extern void marioEntry(void);
extern void N_marioSetBottomlessRespawnPos(f32 x, f32 y, f32 z);
extern void* partyGetPtr(s32 id);

extern void mapLoad(char* name);
extern void msgLoad(char* name, s32 unk);
extern void mapUnLoad(void);
extern s32 mapPreLoad(char* map);
extern char** mapDataPtr(char* map);
extern void setupDataLoad(char* name);
extern void setupDataBase(char* area, char* map);
extern s32 setupDataCheck(void);
extern void bgEntry(char* name);
extern void bgReInit(void);

extern void npcReset(s32 value);
extern void npcExecAllInitEvt(void);
extern s32 npcWaitAllInitEvtEnd(void);
extern void npcExecAllReglEvt(void);
extern void extReset(void);
extern void mobjReset(s32 value);
extern void offscreenReset(s32 value);
extern void imgAutoRelease(s32 value);
extern void effAutoRelease(s32 value);
extern void animPoseAutoRelease(s32 value);
extern void evtmgrReInit(void);
extern void caseReInit(void);
extern void itemReInit(void);
extern void iconReInit(void);
extern void windowReInit(void);
extern void winReInit(void);
extern void statusWinReInit(void);
extern void statusWinForceUpdate(void);
extern void winMgrReInit(void);

extern s32 seqGetPrevSeq(void);
extern s32 seqGetSeq(void);
extern void seqSetSeq(s32 seq, void* arg1, void* arg2);
extern void fadeEntry(s32 type, s32 time, void* data);
extern s32 fadeIsFinish(void);
extern void fadeReset(s32 type);

extern void padRumbleHardOff(s32 controller);
extern void swReInit(void);
extern s32 swByteGet(s32 id);
extern void memClear(s32 heap);
extern void smartAutoFree(s32 heap);
extern void GXResetOverflowCount(void);

extern char* getMarioStDvdRoot(void);
extern s32 fileAsyncf(s32, s32, const char*, ...);
extern void* fileAllocf(s32, const char*, ...);
extern void fileFree(void* file);

extern u32 OSGetSoundMode(void);
extern void SoundSetOutputMode(s32 mode);

extern void* evtEntry(void* script, s32 type, s32 flags);
extern s32 evtCheckID(s32 id);
extern void evtEntryType(void* script, s32 type, s32 a, s32 b);
extern void evt_cam_road_reset(s32 a, s32 b);

extern void camCtrlOff(s32 id);
extern void camCtrlOn(s32 id);
extern void* camGetPtr(s32 id);
extern void camRoadReset(void);
extern void camShiftReset(void);
extern void camRoadMain(
    f32 x1,
    f32 y1,
    f32 z1,
    f32 x2,
    f32 y2,
    f32 z2,
    f32 unkZero,
    void* work
);

extern void hitObjGetPos(char* name, SeqMapVec* outPos);
extern s32 hitCheckFilter(
    f32 x,
    f32 y,
    f32 z,
    f32 vx,
    f32 vy,
    f32 vz,
    s32 flags,
    f32* outZ,
    f32* outY,
    f32* outX,
    f32* outDist,
    void* outA,
    void* outB,
    void* outC
);

extern s32 bero_get_BeroEXEC(void);
extern void animGroupBaseAsync(char* name, s32 a, s32 b);
extern void effNiceAsync(s32 a);

#define NEXT_AREA ((char*)&_next_area)
#define NEXT_MAP ((char*)&_next_map)
#define NEXT_BERO ((char*)&_next_bero)
#define ZERO_STR ((char*)&zero_8041f508)

#define STATE(seq) (*(s32*)((s32)(seq) + 4))

void _relUnLoad(void) {
    void* module;

    module = *(void**)(gp + 0x15C);

    if (module != NULL) {
        ((void (*)(void*))(*(void**)((s32)module + 0x38)))(module);

        OSUnlink(*(void**)(gp + 0x15C));

        if (*(void**)(gp + 0x15C) != *(void**)(gp + 0x160)) {
            _mapFree(mapalloc_base_ptr, *(void**)(gp + 0x15C));
        }

        *(void**)(gp + 0x15C) = NULL;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void _load(char* oldMap, char* mapName, char* beroName) {
    char* bero;
    char* rodata;
    char* map;
    char** mapData;
    char* srcBero;

    char msgName[0x44];

    void* mario;
    void* party;
    void* extraParty;
    void* cam;

    SeqMapVec hitPos;
    SeqMapVec hitTempVec;
    SeqMapVec failHitVec;
    SeqMapVec noDirVec;
    SeqMapVec noBeroVec;
    SeqMapVec outVec;
    SeqMapVec camRoadWork[3];

    f32 hitDist;
    f32 angle;
    f32 dirCos;
    f32 dirSin;
    f32 zero;
    f32 one;
    f32 outA;
    f32 outB;
    f32 outC;

    char** dirPtr;
    size_t dirLen;
    s32 dirIndex;

    (void)oldMap;

    map = mapName;
    rodata = lbl_802BF268;
    bero = beroName;

    mapData = mapDataPtr(map);

    if (bero != NULL) {
        srcBero = bero;
    } else {
        srcBero = (char*)&zero_8041f508;
    }

    strcpy((char*)(gp + 0x11C), srcBero);
    strcpy((char*)(gp + 0x12C), mapData[0]);
    strncpy((char*)(gp + 0x13C), mapData[0], 3);

    if (strcmp(map, rodata + 0x64) == 0) {
        mapLoad(rodata + 0x6C);
    } else {
        mapLoad(mapData[0]);
    }

    camCtrlOff(4);

    if (*(s32*)(gp + 0xC) != 0) {
        msgLoad(rodata + 0x74, 0);
    } else {
        memset(msgName, 0, 0x40);
        strncpy(msgName, mapData[0], 6);
        msgLoad(msgName, 0);
    }

    setupDataLoad(mapData[0]);

    sprintf(msgName, rodata + 0x7C, map);
    bgEntry(msgName);

    if ((*(u32*)gp & 1) != 0) {
        mario = marioGetPtr();

        party = partyGetPtr(marioGetPartyId());
        extraParty = partyGetPtr(marioGetExtraPartyId());

        *(SeqMapVec*)((s32)mario + 0x8C) = *(SeqMapVec*)(gp + 0x11D4);
        *(SeqMapVec*)((s32)mario + 0xEC) = *(SeqMapVec*)((s32)mario + 0x8C);

        camRoadReset();
        camShiftReset();

        if (party != NULL) {
            *(SeqMapVec*)((s32)party + 0x58) = *(SeqMapVec*)(gp + 0x11D4);
        }

        if (extraParty != NULL) {
            *(SeqMapVec*)((s32)extraParty + 0x58) = *(SeqMapVec*)(gp + 0x11D4);
        }
    } else {
        if (bero != NULL) {
            hitDist = float_1E05_8041f50c;
            hitObjGetPos(bero, &hitPos);

            dirIndex = 0;
            dirPtr = dir_str;

            while (dirIndex < 8) {
                if (strncmp(*dirPtr, bero, strlen(*dirPtr)) == 0) {
                    angle = float_3p1416_8041f510 *
                            float_2_8041f514 *
                            (f32)dirIndex *
                            float_0p125_8041f518;

                    dirCos = cos(angle);
                    hitPos.x = float_neg30_8041f51c * dirCos + hitPos.x;

                    dirSin = sin(angle);
                    hitPos.z = float_30_8041f520 * dirSin + hitPos.z;
                    break;
                }

                dirIndex++;
                dirPtr++;
            }

            if (dirIndex < 8) {
                outVec.x = float_0_8041f524;
                outVec.y = float_0_8041f524;
                outVec.z = float_0_8041f524;

                if ((u32)hitCheckFilter(
                        hitPos.x,
                        float_10_8041f528 + hitPos.y,
                        hitPos.z,
                        float_0_8041f524,
                        float_neg1_8041f52c,
                        float_0_8041f524,
                        0,
                        &outVec.z,
                        &outVec.y,
                        &outVec.x,
                        &hitDist,
                        &outA,
                        &outB,
                        &outC
                    ) != 0) {
                    hitTempVec = *(SeqMapVec*)(rodata + 0x1C);
                    hitTempVec.x = outVec.z;
                    hitTempVec.y = outVec.y;
                    hitTempVec.z = outVec.x;

                    mario = marioGetPtr();
                    *(SeqMapVec*)((s32)mario + 0x8C) = hitTempVec;
                } else {
                    failHitVec = *(SeqMapVec*)(rodata + 0x28);
                    mario = marioGetPtr();
                    *(SeqMapVec*)((s32)mario + 0x8C) = failHitVec;
                }
            } else {
                noDirVec = *(SeqMapVec*)(rodata + 0x34);
                mario = marioGetPtr();
                *(SeqMapVec*)((s32)mario + 0x8C) = noDirVec;
            }
        } else {
            noBeroVec = *(SeqMapVec*)(rodata + 0x40);
            mario = marioGetPtr();
            *(SeqMapVec*)((s32)mario + 0x8C) = noBeroVec;
        }
    }

    mario = marioGetPtr();
    cam = camGetPtr(4);

    *(f32*)((s32)cam + 0x94) = *(f32*)((s32)mario + 0x8C);
    *(f32*)((s32)cam + 0x98) = float_0p01_8041f530 + *(f32*)((s32)mario + 0x90);
    *(f32*)((s32)cam + 0x9C) = *(f32*)((s32)mario + 0x94);

    *(f32*)((s32)cam + 0xAC) = *(f32*)((s32)cam + 0x94);
    *(f32*)((s32)cam + 0xB0) = *(f32*)((s32)cam + 0x98);
    *(f32*)((s32)cam + 0xB4) = *(f32*)((s32)cam + 0x9C);

    *(SeqMapVec*)((s32)mario + 0xEC) = *(SeqMapVec*)((s32)mario + 0x8C);

    camRoadReset();
    camShiftReset();

    if ((*(u16*)cam & 4) != 0) {
        one = float_1_8041f534;
        zero = float_0_8041f524;

        camRoadMain(
            one + *(f32*)((s32)cam + 0x94),
            one + *(f32*)((s32)cam + 0x98),
            one + *(f32*)((s32)cam + 0x9C),
            one + *(f32*)((s32)cam + 0xAC),
            one + *(f32*)((s32)cam + 0xB0),
            one + *(f32*)((s32)cam + 0xB4),
            zero,
            camRoadWork
        );

        (void)zero;
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void _unload(char* oldMap, char* newMap) {
    s32 changedArea;
    char* rodata;
    s32 controller;

    rodata = lbl_802BF268;
    changedArea = 0;

    if (oldMap != NULL) {
        if (newMap != NULL) {
            if (strncmp(oldMap, newMap, 3) != 0) {
                changedArea = 1;
            }
        }
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

    if (seqGetPrevSeq() != 6) {
        if (seqGetPrevSeq() != 1) {
            if (strcmp(oldMap, rodata + 0x90) != 0) {
                if (strcmp(oldMap, rodata + 0x98) != 0) {
                    if (strcmp(oldMap, rodata + 0xA0) != 0) {
                        fadeReset(1);
                        *(u32*)gp &= ~0x10;
                    }
                }
            }
        }
    }

    controller = 0;
    do {
        padRumbleHardOff(controller);
        controller++;
    } while (controller < 4);

    if (changedArea != 0) {
        swReInit();
    }

    memClear(1);
    memClear(2);
    smartAutoFree(1);

    nanNPCWork = NULL;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void seq_mapChangeMain(void* seq) {
    char* rodata;
    s32 areaChanged;
    s32 areaCompare;
    s32 result;
    s32 ready;
    s32 state;

    u32 fadeDataC4;
    u32 fadeDataC8;
    u32 fadeDataCC;
    u32 fadeDataD0;
    u32 fadeDataKey;

    char* areaName;
    void* file;
    void* rel;
    char** mapData;
    void* evt;
    void* cam;
    void* mario;
    void* party;
    void* extraParty;
    char* langList[7];

    rodata = lbl_802BF268;

    areaCompare = strcmp(NEXT_AREA, (char*)(gp + 0x13C));
    areaChanged = ((u32)-areaCompare | (u32)areaCompare) >> 31;

    state = STATE(seq);

    switch (state) {
        case 0:
            if (strcmp(NEXT_MAP, rodata + 0x98) == 0) {
                if (*(s32*)(gp + 0x110) != 10) {
                    fadeDataC4 = dat_8041f4c4;
                    fadeEntry(*(s32*)(gp + 0x110), *(s32*)(gp + 0x114), &fadeDataC4);
                }

                fadeDataC8 = dat_8041f4c8;
                fadeEntry(0x11, 0x7D0, &fadeDataC8);
            } else if (areaChanged == 0) {
                fadeDataCC = dat_8041f4cc;
                fadeEntry(*(s32*)(gp + 0x100), *(s32*)(gp + 0x104), &fadeDataCC);
            } else {
                fadeDataD0 = dat_8041f4d0;
                fadeEntry(*(s32*)(gp + 0x110), *(s32*)(gp + 0x114), &fadeDataD0);
            }

            *(s32*)(gp + 0x100) = 10;
            *(s32*)(gp + 0x104) = 300;
            *(s32*)(gp + 0x110) = 10;
            *(s32*)(gp + 0x114) = 300;

            psndSFXAllOff();

            if (areaChanged == 0) {
                *(s32*)(gp + 0x118) = 0;
            } else {
                *(s32*)(gp + 0x118) = 1;
            }

            if (strcmp(NEXT_MAP, rodata + 0x4C) != 0) {
                areaName = NEXT_AREA;

                if (strcmp(NEXT_AREA, (char*)&str_tou_8041f4f4) == 0) {
                    if (seqGetSeq() == 1) {
                        areaName = (char*)&str_tou2_8041f4f8;
                    } else if (strcmp(NEXT_MAP, rodata + 0x4C) == 0) {
                        areaName = (char*)&str_tou2_8041f4f8;
                    }
                }

                fileAsyncf(0, 0, rodata + 0x54, getMarioStDvdRoot(), areaName);
            }

            STATE(seq)++;
            break;

        case 1:
            psndSFXAllOff();

            if (fadeIsFinish() != 0) {
                rel = *(void**)(gp + 0x15C);

                if (rel != NULL) {
                    ((void (*)(void))(*(void**)((s32)rel + 0x38)))();

                    OSUnlink(*(void**)(gp + 0x15C));

                    if (*(void**)(gp + 0x15C) != *(void**)(gp + 0x160)) {
                        _mapFree(mapalloc_base_ptr, *(void**)(gp + 0x15C));
                    }

                    *(void**)(gp + 0x15C) = NULL;
                }

                ((void (*)(char*, char*, char*))_unload)((char*)(gp + 0x12C), NEXT_MAP, NEXT_BERO);

                if (strcmp(NEXT_MAP, rodata + 0x64) != 0) {
                    if (strcmp(NEXT_MAP, (char*)&str_title_8041f538) != 0) {
                        if (OSGetSoundMode() == 0) {
                            SoundSetOutputMode(0);
                        } else if (*(s32*)(gp + 0x1274) == 0) {
                            SoundSetOutputMode(1);
                        } else {
                            SoundSetOutputMode(2);
                        }
                    }
                }

                STATE(seq)++;
            }
            break;

        case 2:
            if (strcmp(NEXT_MAP, (char*)&str_title_8041f538) == 0) {
                strcpy((char*)(gp + 0x14C), (char*)(gp + 0x12C));
                strcpy((char*)(gp + 0x13C), ZERO_STR);
                strcpy((char*)(gp + 0x12C), ZERO_STR);
                seqSetSeq(1, NULL, NULL);
            } else {
                areaName = NEXT_AREA;

                if (strcmp(NEXT_AREA, (char*)&str_tou_8041f4f4) == 0) {
                    if (seqGetSeq() == 1) {
                        areaName = (char*)&str_tou2_8041f4f8;
                    } else if (strcmp(NEXT_MAP, rodata + 0x4C) == 0) {
                        areaName = (char*)&str_tou2_8041f4f8;
                    }
                }

                result = fileAsyncf(0, 0, rodata + 0x54, getMarioStDvdRoot(), areaName);

                if ((u32)result == 0) {
                    ready = 0;
                } else if (result == -1) {
                    ready = 1;
                } else {
                    ready = 1;
                }

                if (ready != 0) {
                    if (mapPreLoad(NEXT_MAP) != 0) {
                        areaName = NEXT_AREA;

                        if (strcmp(NEXT_AREA, (char*)&str_tou_8041f4f4) == 0) {
                            if (seqGetSeq() == 1) {
                                areaName = (char*)&str_tou2_8041f4f8;
                            } else if (strcmp(NEXT_MAP, rodata + 0x4C) == 0) {
                                areaName = (char*)&str_tou2_8041f4f8;
                            }
                        }

                        file = fileAllocf(0, rodata + 0x54, getMarioStDvdRoot(), areaName);

                        if (file != NULL) {
                            if (strncmp(areaName, (char*)&str_tst_8041f500, 3) == 0) {
                                *(void**)(gp + 0x15C) =
                                    _mapAlloc(
                                        mapalloc_base_ptr,
                                        *(u32*)((s32)*(void**)((s32)file + 0xA0) + 4)
                                    );
                            } else if (strncmp(areaName, (char*)&str_jon_8041f504, 3) == 0) {
                                *(void**)(gp + 0x15C) =
                                    _mapAlloc(
                                        mapalloc_base_ptr,
                                        *(u32*)((s32)*(void**)((s32)file + 0xA0) + 4)
                                    );
                            } else {
                                *(void**)(gp + 0x15C) = *(void**)(gp + 0x160);
                            }

                            memcpy(
                                *(void**)(gp + 0x15C),
                                *(void**)((s32)*(void**)((s32)file + 0xA0) + 0),
                                *(u32*)((s32)*(void**)((s32)file + 0xA0) + 4)
                            );

                            fileFree(file);
                        }

                        if (*(void**)(gp + 0x15C) != NULL) {
                            memset(rel_bss, 0, 0x3C4);
                            OSLink(*(void**)(gp + 0x15C), rel_bss);

                            rel = *(void**)(gp + 0x15C);
                            ((void (*)(void))(*(void**)((s32)rel + 0x34)))();
                        }

                        _load((char*)(gp + 0x12C), NEXT_MAP, NEXT_BERO);

                        STATE(seq)++;
                    }
                }
            }
            break;

        case 3:
            if (setupDataCheck() != 0) {
                if ((psndGetFlag() & 0x1000) != 0) {
                    if (psndBGMPlayTime(0) < 3000) {
                        return;
                    }

                    psndClearFlag(0x1000);
                }

                if ((psndGetFlag() & 0x2000) != 0) {
                    if (psndBGMPlayTime(1) < 3000) {
                        return;
                    }

                    psndClearFlag(0x2000);
                }

                setupDataBase(NEXT_AREA, NEXT_MAP);
                psndMapChange();

                mapData = mapDataPtr(NEXT_MAP);

                if (mapData[1] != NULL) {
                    evt = evtEntry(mapData[1], 0, 0);
                    *(s32*)(gp + 0x164) = *(s32*)((s32)evt + 0x15C);
                } else {
                    *(s32*)(gp + 0x164) = 0;
                }

                STATE(seq)++;
            }
            break;

        case 4:
            if (*(s32*)(gp + 0x164) == 0) {
                camCtrlOn(4);
                STATE(seq)++;
            } else if (evtCheckID(*(s32*)(gp + 0x164)) == 0) {
                camCtrlOn(4);
                *(s32*)(gp + 0x164) = 0;
                STATE(seq)++;
            }
            break;

        case 5:
            npcExecAllInitEvt();
            STATE(seq)++;
            break;

        case 6:
            cam = camGetPtr(8);

            if ((*(u16*)cam & 0x200) == 0) {
                if ((bero_get_BeroEXEC() & 1) == 0) {
                    if ((marioChkKey() == 0) || (marioChkCtrl() == 0)) {
                        cam = camGetPtr(8);
                        *(u16*)cam |= 0x500;
                    }
                }
            }

            if (npcWaitAllInitEvtEnd() != 0) {
                npcExecAllReglEvt();
                STATE(seq)++;
            }
            break;

        case 7:
            if (strncmp(NEXT_MAP, (char*)&str_dmo_8041f540, 3) != 0) {
                if (strncmp(NEXT_MAP, (char*)&str_dig_8041f544, 3) != 0) {
                    if (strncmp(NEXT_MAP, (char*)&str_end_8041f548, 3) != 0) {
                        if (strncmp(NEXT_MAP, (char*)&str_sys_8041f54c, 3) != 0) {
                            if (strncmp(NEXT_MAP, rodata + 0xA8, 6) != 0) {
                                langList[0] = *(char**)(rodata + 0x00);
                                langList[1] = *(char**)(rodata + 0x04);
                                langList[2] = *(char**)(rodata + 0x08);
                                langList[3] = *(char**)(rodata + 0x0C);
                                langList[4] = *(char**)(rodata + 0x10);
                                langList[5] = *(char**)(rodata + 0x14);
                                langList[6] = *(char**)(rodata + 0x18);

                                fileAsyncf(
                                    4,
                                    0,
                                    rodata + 0xB0,
                                    getMarioStDvdRoot(),
                                    langList[*(s32*)(gp + 0x16C)]
                                );

                                animGroupBaseAsync(rodata + 0xC0, 0, 0);
                                animGroupBaseAsync(rodata + 0xC8, 0, 0);
                                animGroupBaseAsync(rodata + 0xD0, 0, 0);
                                animGroupBaseAsync(rodata + 0xDC, 0, 0);
                                animGroupBaseAsync(rodata + 0xE8, 0, 0);
                                animGroupBaseAsync(rodata + 0xF0, 0, 0);

                                effNiceAsync(0);

                                fileAsyncf(4, 0, rodata + 0x104, getMarioStDvdRoot());
                                fileAsyncf(4, 0, rodata + 0x128, getMarioStDvdRoot());
                                fileAsyncf(4, 0, rodata + 0x150, getMarioStDvdRoot());
                            }
                        }
                    }
                }
            }

            if ((*(u32*)gp & 1) != 0) {
                mario = marioGetPtr();
                party = partyGetPtr(marioGetPartyId());
                extraParty = partyGetPtr(marioGetExtraPartyId());

                switch (swByteGet(4)) {
                    case 0:
                        goto restoreSavedPosition;
                    default:
                        goto savedPositionFade;
                }

restoreSavedPosition:
                *(SeqMapVecRaw*)((s32)mario + 0x8C) = *(SeqMapVecRaw*)(gp + 0x11D4);

                evt_cam_road_reset(0, 0);

                if (party != NULL) {
                    *(SeqMapVecRaw*)((s32)party + 0x58) = *(SeqMapVecRaw*)(gp + 0x11D4);

                    cam = camGetPtr(4);
                    *(f32*)((s32)party + 0x58) =
                        float_30_8041f520 * (f32)cos(*(f32*)((s32)cam + 0x114)) +
                        *(f32*)((s32)party + 0x58);

                    cam = camGetPtr(4);
                    *(f32*)((s32)party + 0x60) =
                        float_30_8041f520 * (f32)sin(*(f32*)((s32)cam + 0x114)) +
                        *(f32*)((s32)party + 0x60);
                }

                if (extraParty != NULL) {
                    *(SeqMapVecRaw*)((s32)extraParty + 0x58) = *(SeqMapVecRaw*)(gp + 0x11D4);

                    cam = camGetPtr(4);
                    *(f32*)((s32)extraParty + 0x58) =
                        float_neg30_8041f51c * (f32)cos(*(f32*)((s32)cam + 0x114)) +
                        *(f32*)((s32)extraParty + 0x58);

                    cam = camGetPtr(4);
                    *(f32*)((s32)extraParty + 0x60) =
                        float_30_8041f520 * (f32)sin(*(f32*)((s32)cam + 0x114)) +
                        *(f32*)((s32)extraParty + 0x60);
                }

                if ((strcmp((char*)(gp + 0x12C), rodata + 0x178) == 0) ||
                    (strcmp((char*)(gp + 0x12C), rodata + 0x6C) == 0) ||
                    (strcmp((char*)(gp + 0x12C), rodata + 0x180) == 0) ||
                    (strcmp((char*)(gp + 0x12C), rodata + 0x188) == 0)) {
                    *(f32*)((s32)mario + 0x8C) -= float_5_8041f550;

                    if (party != NULL) {
                        *(SeqMapVecRaw*)((s32)party + 0x58) = *(SeqMapVecRaw*)((s32)mario + 0x8C);
                        *(f32*)((s32)party + 0x58) -= float_30_8041f520;

                        if (extraParty != NULL) {
                            *(SeqMapVecRaw*)((s32)extraParty + 0x58) = *(SeqMapVecRaw*)((s32)party + 0x58);
                            *(f32*)((s32)extraParty + 0x58) -= float_30_8041f520;
                        }
                    }
                }

                N_marioSetBottomlessRespawnPos(
                    *(f32*)((s32)mario + 0x8C),
                    float_10_8041f528 + *(f32*)((s32)mario + 0x90),
                    *(f32*)((s32)mario + 0x94)
                );

                marioKeyOff();
                evtEntryType(&key_off, 0, 0, 0);
                goto savedPositionDone;

savedPositionFade:
                fadeDataKey = unk_80429500;
                fadeEntry(0x1C, 0, &fadeDataKey);

savedPositionDone:
                statusWinForceUpdate();

                *(u32*)gp &= ~1;
                marioStSystemLevel(0);
            }

            marioEntry();
            seqSetSeq(2, NULL, NULL);
            break;
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void seq_mapChangeExit(void) {
    GXResetOverflowCount();
    dbg_lotteryinfo = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void seq_mapChangeInit(void* seq) {
    s32 oldMapEmptyCmp;
    s32 oldMapGorCmp;
    s32 nextMapGorCmp;

    strcpy((char*)&_next_area, (char*)&zero_8041f508);
    strcpy((char*)&_next_map, (char*)&zero_8041f508);
    strcpy((char*)&_next_bero, (char*)&zero_8041f508);

    if (*(char**)((s32)seq + 0x8) != 0) {
        strncpy((char*)&_next_area, *(char**)((s32)seq + 0x8), 3);
        strncpy((char*)&_next_map, *(char**)((s32)seq + 0x8), 0x20);
    }

    if (*(char**)((s32)seq + 0xC) != 0) {
        strncpy((char*)&_next_bero, *(char**)((s32)seq + 0xC), 0x20);
    }

    psndENVOff(0x200);
    psndENVOff(0x201);

    oldMapEmptyCmp = strcmp((char*)(gp + 0x12C), (char*)&zero_8041f508);
    oldMapGorCmp = strcmp((char*)(gp + 0x12C), str_gor_01_802bf2d4);
    nextMapGorCmp = strcmp((char*)&_next_map, str_gor_01_802bf2d4);

    if ((oldMapEmptyCmp != 0) && (oldMapGorCmp != 0) && (nextMapGorCmp != 0)) {
        *(u16*)(bdsw + 0x114) |= 1;
    } else if (nextMapGorCmp == 0) {
        if ((*(u16*)(bdsw + 0x114) & 1) != 0) {
            badgeShop_bargainGeneration();
            badgeShop_bottakuruGeneration();
            *(u16*)(bdsw + 0x114) &= ~1;
        }
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset