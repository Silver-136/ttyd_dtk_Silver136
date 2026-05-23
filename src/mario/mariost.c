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
