#include "sdk/DEMOPad.h"


void DEMOPadRead(void) {
    extern u8 Pad[];
    extern u8 DemoPad[];
    extern u32 PadChanMask[];
    extern s32 DemoNumValidPads;
    extern void PADRead(void*);
    extern void PADClamp(void*);
    extern void PADReset(u32);
    extern void DEMOPadCopy(void*, void*);
    u32 resetMask;
    u8* raw;
    u8* state;
    s32 i;
    s8 error;

    resetMask = 0;
    PADRead(Pad);
    PADClamp(Pad);
    DemoNumValidPads = 0;
    raw = Pad;
    state = DemoPad;
    for (i = 0; i < 4; i++, raw += 0xC, state += 0x1E) {
        error = *(s8*)(raw + 0xA);
        if (error == 0 || error == -3) {
            DemoNumValidPads++;
        } else if (error == -1) {
            resetMask |= PadChanMask[i];
        }
        DEMOPadCopy(raw, state);
    }
    if (resetMask != 0) {
        PADReset(resetMask);
    }
}

void DEMOPadInit(void) {
    extern void PADInit(void);
    extern u8 DemoPad[];
    u8* pad;
    s32 i;
    s32 j;

    PADInit();
    pad = DemoPad;
    for (i = 0; i < 4; i++, pad += 0x1E) {
        for (j = 0; j < 0x1E; j++) {
            pad[j] = 0;
        }
    }
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

