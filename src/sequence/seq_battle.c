#include "sequence/seq_battle.h"

void seq_battleExit(void) {
    extern void battle_exit(void);
    battle_exit();
}


u8 seq_battleMain(int param_1) {
    return 0;
}


s32 battle_init(void) {
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

