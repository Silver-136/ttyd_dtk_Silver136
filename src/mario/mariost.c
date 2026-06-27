#include "mario/mariost.h"

s32 marioStGetSystemLevel(void) {
    return *(s32*)((s32)gp + 0x18);
}

void marioStDisp(void) {
    extern void camDraw(void);
    extern s32 g_bFirstSmartAlloc;

    camDraw();
    g_bFirstSmartAlloc = 0;
}


u8 viPostCallback(s32 param_1) {
    return 0;
}


void marioStInit(void) {
    ;
}


u8 gcRumbleCheck(void) {
    return 0;
}


void marioStMain(void) {
    extern void OSYieldThread(void);
    extern void makeKey(void);
    extern void seqMain(void);
    extern void cardMain(void);
    extern void bgMain(void);
    extern void shadowMain(void);
    extern void animMain(void);
    extern void battleMain(void);
    extern void camMain(void);
    extern void windowMain(void);
    extern void mapMain(void);
    extern void hitMain(void);
    extern void evtmgrMain(void);
    extern void mapDisp(void);
    extern void marioMain(void);
    extern void mobjMain(void);
    extern void npcMain(void);
    extern void fadeMain(void);
    extern void imgMain(void);
    extern void lightMain(void);
    extern void offscreenMain(void);
    extern void itemMain(void);
    extern void caseMain(void);
    extern void iconMain(void);
    extern void extMain(void);
    extern void psndMain(void);
    extern void winMain(void);
    extern void statusWinMain(void);
    extern void envMain(void);
    extern void countDownMain(void);
    extern void nameEntMain(void);
    extern void winMgrMain(void);
    extern void effMain(void);
    extern void L_gcResetCheck(void);
    extern s32 seqGetSeq(void);
    extern s64 none_key;
    u8* bytes;
    u32* words;
    s32 i;
    s32 count;
    s64 now;

    while (*(s32*)((s32)gp + 0x10) != 0) {
        OSYieldThread();
    }

    makeKey();
    seqMain();
    cardMain();
    bgMain();
    shadowMain();
    animMain();
    battleMain();
    camMain();
    windowMain();
    mapMain();
    hitMain();
    evtmgrMain();
    mapDisp();
    marioMain();
    mobjMain();
    npcMain();
    fadeMain();
    imgMain();
    lightMain();
    offscreenMain();
    itemMain();
    caseMain();
    iconMain();
    extMain();
    psndMain();
    winMain();
    statusWinMain();
    envMain();
    countDownMain();
    nameEntMain();
    winMgrMain();
    effMain();
    L_gcResetCheck();

    if ((*(u32*)gp & 0x1000) != 0) {
        words = (u32*)gp;
        bytes = (u8*)gp;
        count = 0;
        for (i = 0; i < 2; i++) {
            if (*(u32*)((u8*)words + 0x1378) != 0) goto input_found;
            if (*(u32*)((u8*)words + 0x1328) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13B8) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13BC) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C0) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C4) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13C8) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13CC) != 0) goto input_found;
            count++;
            if (*(u32*)((u8*)words + 0x137C) != 0) goto input_found;
            if (*(u32*)((u8*)words + 0x132C) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13B9) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13BD) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C1) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C5) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13C9) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13CD) != 0) goto input_found;
            words = (u32*)((u8*)words + 8);
            bytes += 2;
            count++;
        }
input_found:
        if (count >= 4) {
            now = OSGetTime();
            if ((now - none_key) / (*(u32*)0x800000F8 >> 2) > 120) {
                if (seqGetSeq() == 2 || seqGetSeq() == 4) {
                    *(u32*)gp |= 0x2000;
                }
            }
        } else {
            none_key = OSGetTime();
        }
    }
}

u8 gcDvdCheckThread(void) {
    return 0;
}


void L_gcResetCheck(void) {
    extern s32 R_bReset;
    extern s32 DvdCheckTreadOn;
    extern char DvdCheckThread[];
    extern s32 cardIsExec(void);
    extern void cardMain(void);
    extern void VISetBlack(s32 black);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void psndExit(void);
    extern void DVDMgrDelete(void);
    extern void* OSGetCurrentThread(void);
    extern void OSCancelThread(void* thread);
    extern void OSResetSystem(s32 reset, u32 resetCode, s32 forceMenu);
    void* thread;
    u32 flags;
    u32 resetCode;

    if (*(s32*)((s32)gp + 0x1278) != 0 || R_bReset != 0) {
        R_bReset = 1;
        if (cardIsExec() == 0) {
            VISetBlack(1);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            VIWaitForRetrace();
            psndExit();
            DVDMgrDelete();
            thread = OSGetCurrentThread();
            if (thread != DvdCheckThread && DvdCheckTreadOn != 0) {
                OSCancelThread(DvdCheckThread);
                DvdCheckTreadOn = 0;
            }
            while (cardIsExec() != 0) {
                cardMain();
                VIWaitForRetrace();
            }
            if (*(s32*)((s32)gp + 0x1278) != 2) {
                flags = *(u32*)gp;
                if ((flags & 4) == 0) {
                    resetCode = 2;
                } else if ((flags & 8) == 0) {
                    resetCode = 0;
                } else {
                    resetCode = 1;
                }
                resetCode |= *(u32*)((s32)gp + 0x16C) << 2;
                OSResetSystem(0, resetCode, 0);
            } else {
                OSResetSystem(1, 0, 1);
            }
            for (;;) {
            }
        }
    }
}

void marioStSystemLevel(s32 level) {
    extern s32 _mariostSystemLevel;
    extern void evtStartAll(s32 mask);
    extern void evtStopAll(s32 mask);

    switch (level) {
        case 0:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xF;
            evtStartAll(0xEF);
            break;
        case 1:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xE;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 1;
            evtStopAll(1);
            break;
        case 2:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xC;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 3;
            evtStopAll(2);
            break;
        case 3:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~8;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 7;
            evtStopAll(0x10);
            break;
        case 4:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 0xF;
            evtStopAll(0xEF);
            break;
    }
}
