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

