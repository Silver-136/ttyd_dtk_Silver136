#include "sequence/seq_battle.h"

void seq_battleExit(void) {
    extern void battle_exit(void);
    battle_exit();
}


u8 seq_battleMain(int param_1) {
    extern void fadeEntry(s32, s32, void*);
    extern s32 getMarioStDvdRoot(void);
    extern u32 fileAsyncf(s32, s32, char*, s32, char*, ...);
    extern u32 fadeIsFinish(void);
    extern void* pouchGetPtr(void);
    extern void* fbatGetPointer(void);
    extern void* marioGetPtr(void);
    extern void marioCtrlOff(void);
    extern void partyCtrlOff(void);
    extern void padRumbleOff(s32);
    extern void bmapLoad(char*, char*);
    extern s32 battle_init(void);
    extern u32 battleSeqEndCheck(void);
    extern void* camGetPtr(s32);
    extern void psndSetPosDirListener(void*, f32);
    extern void seqSetSeq(s32, s32, s32);
    extern void* gp;
    extern void* _battleWorkPointer;
    extern s32 debug_battle_flag;
    extern u32 dat_804203b0;
    extern u32 dat_804203b4;
    extern char str_PCTs_PCTs_804203bc[];
    extern char str_battle_audience_audi_802c2ae4[];
    extern char str_battle_audience_audi_802c2b08[];
    extern char str_bti_01_802c2ac4[];
    extern char str_bti_02_802c2acc[];
    extern char str_bti_03_802c2ad4[];
    extern char str_bti_04_802c2adc[];
    extern const f32 float_0_804203c4;
    extern const f32 float_400_804203c8;
    extern const f32 float_200_804203cc;
    u32 color;
    void* fbat;
    void* pouch;
    void* stageInfo;
    void* cam;
    f32 listener[3];
    s32 keepWaiting;
    s32 type;
    s16 rank;

    switch (*(s32*)(param_1 + 4)) {
        case 0:
            color = dat_804203b0;
            fadeEntry(0x16, 1000, &color);
            *(s32*)(param_1 + 4) += 1;
            *(s32*)(param_1 + 0x10) = 0;
            break;
        case 1:
            if (fileAsyncf(4, 0, str_PCTs_PCTs_804203bc, getMarioStDvdRoot(),
                           str_battle_audience_audi_802c2ae4) != 0) {
                if (fileAsyncf(4, 0, str_PCTs_PCTs_804203bc, getMarioStDvdRoot(),
                               str_battle_audience_audi_802c2b08) != 0) {
                    *(s32*)(param_1 + 0x10) += 1;
                    if (fadeIsFinish() != 0) {
                        *(s32*)(param_1 + 4) += 1;
                    }
                }
            }
            break;
        case 2:
            pouchGetPtr();
            fbat = fbatGetPointer();
            type = *(s32*)((s32)fbat + 0x10);
            if (type == 0x01000000 || type == 0x00800000) {
                if (debug_battle_flag == 0) {
                    *(u32*)marioGetPtr() &= ~1;
                }
            } else if (debug_battle_flag == 0) {
                marioCtrlOff();
                partyCtrlOff();
                *(u32*)marioGetPtr() &= ~1;
                padRumbleOff(0);
            }
            fbat = fbatGetPointer();
            *(s32*)((s32)fbat + 0x570) = 0;
            *(s32*)(param_1 + 4) += 1;
        case 3:
            fbat = fbatGetPointer();
            stageInfo = *(void**)(*(s32*)(*(s32*)((s32)fbat + 8) + 0x230) + 4);
            pouch = pouchGetPtr();
            keepWaiting = 0;
            if (*(s32*)((s32)fbat + 0x550) != 0) {
                keepWaiting = 1;
                if (*(s32*)((s32)fbat + 0x558) >= 1 && *(s32*)((s32)fbat + 0x558) < 3 &&
                    *(s32*)((s32)fbat + 0x554) != 0) {
                    keepWaiting = 0;
                }
                if (keepWaiting == 0) {
                    *(s32*)((s32)fbat + 0x570) += 1;
                    if (*(s32*)((s32)fbat + 0x570) < 200) {
                        keepWaiting = 1;
                    }
                }
            }
            if (keepWaiting == 0) {
                rank = *(s16*)((s32)pouch + 0x88);
                if (rank == 0) {
                    bmapLoad(str_bti_01_802c2ac4, stageInfo);
                } else if (rank == 1) {
                    bmapLoad(str_bti_02_802c2acc, stageInfo);
                } else if (rank == 2) {
                    bmapLoad(str_bti_03_802c2ad4, stageInfo);
                } else {
                    bmapLoad(str_bti_04_802c2adc, stageInfo);
                }
                *(s32*)(param_1 + 4) += 1;
                if (battle_init() == 0) {
                    *(s32*)(param_1 + 4) += 1;
                    if (battleSeqEndCheck() != 0) {
                        *(s32*)(param_1 + 0x10) = 0;
                        *(s32*)(param_1 + 4) += 1;
                        if (*(s32*)((s32)gp + 0xC) != 0) {
                            *(s32*)(param_1 + 4) = 0xB;
                        }
                    }
                    marioGetPtr();
                    camGetPtr(4);
                    listener[0] = float_0_804203c4;
                    listener[1] = float_0_804203c4;
                    listener[2] = float_400_804203c8;
                    if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x100) != 0) {
                        listener[2] = float_400_804203c8 + float_200_804203cc;
                    }
                    cam = camGetPtr(4);
                    psndSetPosDirListener(listener, *(f32*)((s32)cam + 0x114));
                }
            }
            break;
        case 4:
            if (battle_init() == 0) {
                *(s32*)(param_1 + 4) += 1;
            }
        case 5:
            if (battleSeqEndCheck() != 0) {
                *(s32*)(param_1 + 0x10) = 0;
                *(s32*)(param_1 + 4) += 1;
                if (*(s32*)((s32)gp + 0xC) != 0) {
                    *(s32*)(param_1 + 4) = 0xB;
                }
            }
            marioGetPtr();
            camGetPtr(4);
            listener[0] = float_0_804203c4;
            listener[1] = float_0_804203c4;
            listener[2] = float_400_804203c8;
            if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x100) != 0) {
                listener[2] = float_400_804203c8 + float_200_804203cc;
            }
            cam = camGetPtr(4);
            psndSetPosDirListener(listener, *(f32*)((s32)cam + 0x114));
            break;
        case 6:
            *(s32*)(param_1 + 0x10) += 1;
            if (*(s32*)(param_1 + 0x10) > 30) {
                if ((*(u32*)gp & 0x1000) != 0 && (*(u32*)gp & 0x2000) != 0) {
                    seqSetSeq(5, 1, 0);
                } else {
                    seqSetSeq(2, 1, 0);
                }
            }
            break;
        case 10:
            color = dat_804203b4;
            fadeEntry(0x16, 200, &color);
            *(s32*)(param_1 + 4) += 1;
            break;
        case 11:
            if (fadeIsFinish() != 0) {
                *(s32*)((s32)gp + 0xC) = 0;
                seqSetSeq(5, 1, 0);
            }
            break;
    }
    return 0;
}

s32 battle_init(void) {
    extern void* fbatGetPointer(void);
    extern void* pouchGetPtr(void);
    extern void camSetMode(s32, s32);
    extern char* DbgBtlSel_GetMsgDataPtr(void);
    extern void msgLoad(char*, s32);
    extern void N_battleMapAlloc(void);
    extern void animPoseBattleInit(void);
    extern void npcReleaseFiledNpc(void);
    extern void evtmgrReInit(void);
    extern void psndSetFlag(s32);
    extern void BattleInformationInit(void*);
    extern void BattleInformationSetMode(void*, s32);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern s32 BattleTransPartyIdToUnitKind(s32);
    extern void BattleInformationSetParty(void*, s32);
    extern void BattleInformationSetFirstAttack(void*, s32);
    extern void BattleInfomationSetBattleSetupInfo(void*, void*);
    extern void BattleInit(void*);
    extern void* gp;
    extern void* _battleWorkPointer;
    extern char str_gon_11_802c2abc[];
    u8* fbat;
    u8* pouch;
    u8* npc;
    u8* info;
    u8* party;
    char* room;
    s32 partyMember;
    s32 partyKind;
    s32 firstAttack;
    u32 hitFlags;

    fbat = fbatGetPointer();
    npc = *(u8**)(fbat + 8);
    pouch = pouchGetPtr();
    camSetMode(4, 2);

    if (*(s32*)((u8*)gp + 0xC) != 0) {
        room = DbgBtlSel_GetMsgDataPtr();
        if (room != 0) {
            msgLoad(room, 0);
        } else {
            msgLoad(str_gon_11_802c2abc, 0);
        }
    }

    N_battleMapAlloc();
    _battleWorkPointer = 0;
    animPoseBattleInit();
    npcReleaseFiledNpc();
    *(s32*)((u8*)gp + 0x14) = 1;
    evtmgrReInit();
    psndSetFlag(0x10);

    info = fbat + 0x20;
    BattleInformationInit(info);
    BattleInformationSetMode(info, 0);

    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        partyMember = *(s8*)(party + 0x31);
        partyKind = BattleTransPartyIdToUnitKind(partyMember);
        if ((*(u16*)(pouch + partyMember * 0xE) & 2) == 0) {
            BattleInformationSetParty(info, partyKind);
        }
    }

    if ((*(u32*)(fbat + 0x1C) & 1) != 0) {
        firstAttack = 0;
    } else {
        hitFlags = *(u32*)(fbat + 0x10);
        switch (hitFlags) {
            case 0x00020000:
                firstAttack = 1;
                break;
            case 0x00040000:
                firstAttack = 2;
                break;
            case 0x00080000:
                firstAttack = 3;
                break;
            case 0x00100000:
                firstAttack = 4;
                break;
            case 0x00200000:
                firstAttack = 5;
                break;
            case 0x00400000:
                firstAttack = 6;
                break;
            case 0x00800000:
                firstAttack = 7;
                break;
            case 0x01000000:
                firstAttack = 8;
                break;
            case 0x10000000:
                firstAttack = *(s32*)(fbat + 0x14);
                break;
            default:
                firstAttack = 0;
                break;
        }
    }
    BattleInformationSetFirstAttack(info, firstAttack);

    BattleInfomationSetBattleSetupInfo(info, npc + 0x230);
    info[0x18] = npc[0x2F4];
    info[0x1A] = npc[0x2F5];
    info[0x1B] = npc[0x2F6];
    BattleInit(info);
    return 0;
}

void battle_exit(void) {
    extern void BattleEnd(void);
    extern void N_battleMapFree(void);
    extern void smartAutoFree(s32 heap);
    extern void itemReInit(void);
    extern void iconReInit(void);
    extern void extReset(void);
    extern void bmapUnLoad(void);
    extern void* camGetPtr(s32 cameraId);
    extern void camSetMode(s32 cameraId, s32 mode);
    extern void npcReset(s32 flag);
    extern void mobjReset(s32 flag);
    extern void offscreenReset(s32 flag);
    extern void imgAutoRelease(s32 value);
    extern void effAutoRelease(s32 value);
    extern void animPoseAutoRelease(s32 value);
    extern void evtmgrReInit(void);
    extern void npcRecoveryFiledNpc(void);
    extern void evtStartAll(s32 mask);
    extern void marioCtrlOn(void);
    extern void partyCtrlOn(void);
    extern void* marioGetPtr(void);
    extern void L_psndBGM_stop(void);
    extern void psndClearFlag(s32 flag);
    extern void* lightGetPaperCraft(void);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern s32 evtGetValue(void* evt, s32 value);
    extern void unk_800db778(s32 a, s32 b, s32 c, s32 d);
    extern s32 seqGetNextSeq(void);
    extern void psndBGMOn(s32 a, s32 bgm);
    extern void psndENVOn(s32 a, s32 env);
    extern void* gp;
    extern s32 debug_battle_flag;
    extern const u32 vec3_802c2ab0[];
    extern const f32 float_3_804203b8;
    void* craft;
    f32 mtx[3][4];

    BattleEnd();
    N_battleMapFree();
    smartAutoFree(2);
    itemReInit();
    iconReInit();
    extReset();
    bmapUnLoad();
    if ((*(u16*)camGetPtr(4) & 4) != 0) {
        camSetMode(4, 1);
    } else {
        camSetMode(4, 3);
    }
    npcReset(1);
    mobjReset(1);
    offscreenReset(1);
    imgAutoRelease(1);
    effAutoRelease(1);
    animPoseAutoRelease(1);
    evtmgrReInit();
    npcRecoveryFiledNpc();
    *(s32*)((s32)gp + 0x14) = 0;
    evtStartAll(4);
    if (debug_battle_flag == 0) {
        marioCtrlOn();
        partyCtrlOn();
        *(u32*)marioGetPtr() |= 1;
    }
    L_psndBGM_stop();
    psndClearFlag(0x80);
    craft = lightGetPaperCraft();
    *(u32*)((s32)craft + 0x24) = vec3_802c2ab0[0];
    *(u32*)((s32)craft + 0x28) = vec3_802c2ab0[1];
    *(u32*)((s32)craft + 0x2C) = vec3_802c2ab0[2];
    PSMTXScale(mtx, float_3_804203b8, float_3_804203b8, float_3_804203b8);
    PSMTXMultVec(mtx, (void*)((s32)craft + 0x24), (void*)((s32)craft + 0x24));
    if (evtGetValue(0, 0xF5DE0180) >= 0x6D && evtGetValue(0, 0xF5DE0180) < 0x6F) {
        unk_800db778(0x120, 0, 0x2EE, 1);
    } else if (evtGetValue(0, 0xF5DE0180) != 0x192 && seqGetNextSeq() != 5) {
        psndBGMOn(0x120, 0);
        psndENVOn(0x120, 0);
    }
}

void seq_battleInit(void) {
    extern void* gp;
    extern s32 debug_battle_flag;
    extern void* fbatGetPointer(void);
    extern s32 evtGetValue(void* evt, s32 value);
    extern void psndBGMOff(s32 flags);
    extern void psndBGMOn(s32 a, s32 bgm);
    extern void psndENVOff(s32 flags);
    extern void padRumbleOff(s32 pad);
    extern void evtStopAll(s32 mask);
    extern void psndSetFlag(s32 flag);
    extern void marioCtrlOff(void);
    extern void partyCtrlOff(void);
    void* battle;
    s32 type;

    debug_battle_flag = *(s32*)((s32)gp + 0xC);
    battle = *(void**)((s32)fbatGetPointer() + 8);
    if (evtGetValue(0, 0xF5DE0180) < 0x6D ||
        evtGetValue(0, 0xF5DE0180) >= 0x6F) {
        if (evtGetValue(0, 0xF5DE0180) != 0x192) {
            psndBGMOff(0x400);
            psndBGMOn(1, *(s32*)((s32)battle + 0x2F0));
        }
    }
    psndENVOff(0x400);
    padRumbleOff(0);
    evtStopAll(4);
    psndSetFlag(0x80);
    type = *(s32*)((s32)fbatGetPointer() + 0x10);
    switch (type) {
        case 0x01000000:
        case 0x00800000:
            if (debug_battle_flag == 0) {
                marioCtrlOff();
                partyCtrlOff();
                padRumbleOff(0);
            }
            break;
    }
}

