#include "sequence/seq_gameover.h"

extern u8 psbgm[];
extern u8 sound[];

void psndStopAllFadeOut(void);
void marioStSystemLevel(s32 level);
void winReInit(void);
void L_camDispOn(s32 camId);
void psndSetReverb(s32 reverb);
void L_psndBGM_stop(void);
void psndClearFlag(s32 flag);
void N_pouchUnEquipBadgeID(s32 badgeId);

void seq_gameOverInit(void) {
    winReInit();
    L_camDispOn(4);
    psndStopAllFadeOut();
    psndSetReverb(0);
    L_psndBGM_stop();
    psndClearFlag(0x80);
    psndClearFlag(0x20);
    psndClearFlag(0x40);
    N_pouchUnEquipBadgeID(0x143);
    marioStSystemLevel(4);
}

void seq_gameOverExit(void) {
    psndStopAllFadeOut();
    marioStSystemLevel(0);
}

s32 N_evt_snd_bgm_unpause(void) {
    s32 id = *(s32*)(psbgm + 4);
    if ((u32)(id + 0x10000) != 0xFFFF) {
        u16* table = *(u16**)(sound + 0x100);
        table[id * 0x9C] &= 0x7FFF;
    }
    return 2;
}


void seq_gameOverMain(void* param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6) {
    extern void fadeEntry(s32 type, s32 duration, void* color);
    extern s32 fadeIsFinish(void);
    extern void fadeTecOff(void);
    extern void fadeSoftFocusOff(void);
    extern s32 mapPreLoad(void);
    extern void mapLoad(char* map);
    extern void _relUnLoad(void);
    extern void _unload(char* map, char* bero, s32 unk, s32 param_4, u32 param_5, u32 param_6);
    extern char* strcpy(char* dst, const char* src);
    extern void statusWinForceOff(void);
    extern void statusWinDispOff(void);
    extern void statusWinDispOn(void);
    extern void* camGetPtr(s32 camId);
    extern void npcClearDeadInfo(void);
    extern void countDownEnd(void);
    extern void nameEntReInit(void);
    extern void* marioGetPtr(void);
    extern void camRoadReset(void);
    extern void camShiftReset(void);
    extern void marioReset(void);
    extern void marioEntry(void);
    extern void psndBGMOn(u32 id, char* flag);
    extern void psndBGMOff_f_d(u32 id, u32 fade, s32 mode);
    extern void* evtEntryType(void* code, s8 priority, u32 flags, u32 type);
    extern s32 evtCheckID(s32 id);
    extern u32 keyGetButtonTrg(s32 pad);
    extern void seqSetSeq(s32 seq, char* map, char* bero);
    extern u32 dat_804204a0;
    extern u32 dat_804204a4;
    extern u32 dat_804204a8;
    extern u32 dat_804204ac;
    extern f32 float_0_804204b0;
    extern f32 float_0p01_804204b4;
    extern u32 vec3_802c2c0c[3];
    extern char str_sys_00_802c2c18[];
    extern char zero_804204b8;
    extern u8 evt_gameover[];
    extern void* wp;
    extern void* gp;
    void* gpLocal;
    void* cam;
    void* mario;
    void* evt;
    u32 color;
    s32 bgmId;
    u16* bgmTable;

    switch (*(s32*)((s32)param_1 + 4)) {
        case 0:
            if (*(void**)((s32)param_1 + 8) == 0) {
                color = dat_804204a0;
                fadeEntry(2, 1000, &color);
                gpLocal = gp;
                *(s32*)((s32)gpLocal + 0x110) = 10;
                *(s32*)((s32)gpLocal + 0x114) = 300;
            }
            mapPreLoad();
            *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            break;
        case 1:
            if (fadeIsFinish() != 0) {
                _relUnLoad();
                gpLocal = gp;
                _unload((char*)((s32)gpLocal + 0x12C), 0, 0, param_4, param_5, param_6);
                gpLocal = gp;
                strcpy((char*)((s32)gpLocal + 0x14C), (char*)((s32)gpLocal + 0x12C));
                statusWinForceOff();
                statusWinDispOff();
                cam = camGetPtr(4);
                *(f32*)((s32)cam + 0xFC) = float_0_804204b0;
                *(f32*)((s32)cam + 0x100) = float_0_804204b0;
                *(f32*)((s32)cam + 0x104) = (f32)*(u16*)((s32)gp + 0x170);
                *(f32*)((s32)cam + 0x108) = (f32)*(u16*)((s32)gp + 0x172);
                cam = camGetPtr(8);
                *(f32*)((s32)cam + 0xFC) = float_0_804204b0;
                *(f32*)((s32)cam + 0x100) = float_0_804204b0;
                *(f32*)((s32)cam + 0x104) = (f32)*(u16*)((s32)gp + 0x170);
                *(f32*)((s32)cam + 0x108) = (f32)*(u16*)((s32)gp + 0x172);
                cam = camGetPtr(8);
                *(u16*)cam &= ~0x100;
                cam = camGetPtr(8);
                *(u16*)cam |= 0x400;
                cam = camGetPtr(8);
                *(u16*)cam |= 0x200;
                fadeTecOff();
                fadeSoftFocusOff();
                npcClearDeadInfo();
                countDownEnd();
                nameEntReInit();
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        case 2:
            if (mapPreLoad() != 0) {
                mapLoad(str_sys_00_802c2c18);
                cam = camGetPtr(4);
                mario = marioGetPtr();
                *(f32*)((s32)cam + 0x94) = float_0_804204b0;
                *(f32*)((s32)cam + 0x98) = float_0p01_804204b4;
                *(f32*)((s32)cam + 0x9C) = float_0_804204b0;
                *(u32*)((s32)cam + 0xAC) = *(u32*)((s32)cam + 0x94);
                *(u32*)((s32)cam + 0xB0) = *(u32*)((s32)cam + 0x98);
                *(u32*)((s32)cam + 0xB4) = *(u32*)((s32)cam + 0x9C);
                *(u32*)((s32)mario + 0x8C) = vec3_802c2c0c[0];
                *(u32*)((s32)mario + 0x90) = vec3_802c2c0c[1];
                *(u32*)((s32)mario + 0x94) = vec3_802c2c0c[2];
                *(u32*)((s32)mario + 0xEC) = *(u32*)((s32)mario + 0x8C);
                *(u32*)((s32)mario + 0xF0) = *(u32*)((s32)mario + 0x90);
                *(u32*)((s32)mario + 0xF4) = *(u32*)((s32)mario + 0x94);
                camRoadReset();
                camShiftReset();
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        case 3:
            color = dat_804204a4;
            fadeEntry(0x1F, 200, &color);
            *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            break;
        case 4:
            if (fadeIsFinish() != 0) {
                color = dat_804204a8;
                fadeEntry(9, 1000, &color);
                marioReset();
                marioEntry();
                psndBGMOn(0x200, (char*)0x93);
                bgmId = *(s32*)(psbgm + 4);
                if ((u32)(bgmId + 0x10000) != 0xFFFF) {
                    bgmTable = *(u16**)(sound + 0x100);
                    bgmTable[(bgmId * 0x138) / 2] |= 0x8000;
                }
                evt = evtEntryType(evt_gameover, 0, 0, 0);
                *(void**)wp = evt;
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        case 5:
            if (evtCheckID(*(s32*)((s32)*(void**)wp + 0x15C)) == 0 || (keyGetButtonTrg(0) & 0x1100) != 0) {
                color = dat_804204ac;
                fadeEntry(0x24, 0, &color);
                psndBGMOff_f_d(0x200, 4000, 0);
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        case 6:
            if (fadeIsFinish() != 0) {
                _relUnLoad();
                _unload(str_sys_00_802c2c18, 0, 0, param_4, param_5, param_6);
                strcpy((char*)((s32)gp + 0x11C), &zero_804204b8);
                strcpy((char*)((s32)gp + 0x12C), &zero_804204b8);
                strcpy((char*)((s32)gp + 0x13C), &zero_804204b8);
                statusWinDispOn();
                cam = camGetPtr(8);
                *(u16*)cam &= ~0x200;
                if ((*(u32*)gp & 0x1000) == 0) {
                    seqSetSeq(1, 0, 0);
                } else {
                    seqSetSeq(7, 0, 0);
                }
            }
            break;
    }
}

