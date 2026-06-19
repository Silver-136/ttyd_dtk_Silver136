#include "sdk/DEMOPad.h"


void DEMOPadRead(void) {
}


u8 DEMOPadInit(void) {
    return 0;
}


void DEMOPadCopy(void* newPadState, void* padState) {
    u16 dirs;
    u16 buttons;
    s8 stickX;
    s8 stickY;
    s8 substickX;
    s8 substickY;
    u8 triggerLeft;
    u8 triggerRight;
    u8 analogB;
    s8 error;
    u8 pad;

    if (*(s8*)((s32)newPadState + 0xA) != -3) {
        dirs = 0;
        if (*(s8*)((s32)newPadState + 2) < -0x30) {
            dirs |= 0x4000;
        }
        if (*(s8*)((s32)newPadState + 2) > 0x30) {
            dirs |= 0x8000;
        }
        if (*(s8*)((s32)newPadState + 3) < -0x30) {
            dirs |= 0x2000;
        }
        if (*(s8*)((s32)newPadState + 3) > 0x30) {
            dirs |= 0x1000;
        }
        if (*(s8*)((s32)newPadState + 4) < -0x30) {
            dirs |= 0x400;
        }
        if (*(s8*)((s32)newPadState + 4) > 0x30) {
            dirs |= 0x800;
        }
        if (*(s8*)((s32)newPadState + 5) < -0x30) {
            dirs |= 0x200;
        }
        if (*(s8*)((s32)newPadState + 5) > 0x30) {
            dirs |= 0x100;
        }
        *(u16*)((s32)padState + 0x12) = dirs & (*(u16*)((s32)padState + 0x10) ^ dirs);
        *(u16*)((s32)padState + 0x14) = *(u16*)((s32)padState + 0x10) & (*(u16*)((s32)padState + 0x10) ^ dirs);
        *(u16*)((s32)padState + 0x10) = dirs;
        buttons = *(u16*)newPadState;
        *(u16*)((s32)padState + 0xC) = buttons & (*(u16*)padState ^ buttons);
        *(u16*)((s32)padState + 0xE) = *(u16*)padState & (*(u16*)padState ^ buttons);
        *(s16*)((s32)padState + 0x16) = (s16)*(s8*)((s32)newPadState + 2) - (s16)*(s8*)((s32)padState + 2);
        *(s16*)((s32)padState + 0x18) = (s16)*(s8*)((s32)newPadState + 3) - (s16)*(s8*)((s32)padState + 3);
        *(s16*)((s32)padState + 0x1A) = (s16)*(s8*)((s32)newPadState + 4) - (s16)*(s8*)((s32)padState + 4);
        *(s16*)((s32)padState + 0x1C) = (s16)*(s8*)((s32)newPadState + 5) - (s16)*(s8*)((s32)padState + 5);
        stickX = *(s8*)((s32)newPadState + 2);
        stickY = *(s8*)((s32)newPadState + 3);
        substickX = *(s8*)((s32)newPadState + 4);
        substickY = *(s8*)((s32)newPadState + 5);
        triggerLeft = *(u8*)((s32)newPadState + 6);
        triggerRight = *(u8*)((s32)newPadState + 7);
        *(u16*)padState = *(u16*)newPadState;
        *(s8*)((s32)padState + 2) = stickX;
        *(s8*)((s32)padState + 3) = stickY;
        *(s8*)((s32)padState + 4) = substickX;
        *(s8*)((s32)padState + 5) = substickY;
        *(u8*)((s32)padState + 6) = triggerLeft;
        *(u8*)((s32)padState + 7) = triggerRight;
        analogB = *(u8*)((s32)newPadState + 9);
        error = *(s8*)((s32)newPadState + 0xA);
        pad = *(u8*)((s32)newPadState + 0xB);
        *(u8*)((s32)padState + 8) = *(u8*)((s32)newPadState + 8);
        *(u8*)((s32)padState + 9) = analogB;
        *(s8*)((s32)padState + 0xA) = error;
        *(u8*)((s32)padState + 0xB) = pad;
    } else {
        *(u16*)((s32)padState + 0x14) = 0;
        *(u16*)((s32)padState + 0x12) = 0;
        *(u16*)((s32)padState + 0xE) = 0;
        *(u16*)((s32)padState + 0xC) = 0;
        *(s16*)((s32)padState + 0x18) = 0;
        *(s16*)((s32)padState + 0x16) = 0;
        *(s16*)((s32)padState + 0x1C) = 0;
        *(s16*)((s32)padState + 0x1A) = 0;
    }
}

