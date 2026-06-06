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
}


u8 gcRumbleCheck(void) {
    return 0;
}


void marioStMain(void) {
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

