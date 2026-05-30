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

void seq_gameOverExit(void) {
    psndStopAllFadeOut();
    marioStSystemLevel(0);
}

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

s32 N_evt_snd_bgm_unpause(void) {
    s32 id = *(s32*)(psbgm + 4);
    if ((u32)(id + 0x10000) != 0xFFFF) {
        u16* table = *(u16**)(sound + 0x100);
        table[id * 0x9C] &= 0x7FFF;
    }
    return 2;
}


u8 seq_gameOverMain(void* param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6) {
    return 0;
}
