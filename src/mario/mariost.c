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


u8 L_gcResetCheck(void) {
    return 0;
}


void marioStSystemLevel(s32 level) {
}
