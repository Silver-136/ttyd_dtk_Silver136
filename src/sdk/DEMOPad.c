#include "sdk/DEMOPad.h"

u8 DemoPad[0x78];
u32 DemoNumValidPads;
u8 Pad[0x30];

u32 PadChanMask[] = {
    0x80000000,
    0x40000000,
    0x20000000,
    0x10000000,
};

void DEMOPadCopy(void* newPadState, void* padState) {
    u16 dirs;
    u16 buttons;
    u32 copyA;
    u32 copyB;

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
        *(u16*)((s32)padState + 0xE) = *(u16*)padState & (*(u16*)padState ^ *(u16*)newPadState);
        *(s16*)((s32)padState + 0x16) = (s16)*(s8*)((s32)newPadState + 2) - (s16)*(s8*)((s32)padState + 2);
        *(s16*)((s32)padState + 0x18) = (s16)*(s8*)((s32)newPadState + 3) - (s16)*(s8*)((s32)padState + 3);
        *(s16*)((s32)padState + 0x1A) = (s16)*(s8*)((s32)newPadState + 4) - (s16)*(s8*)((s32)padState + 4);
        *(s16*)((s32)padState + 0x1C) = (s16)*(s8*)((s32)newPadState + 5) - (s16)*(s8*)((s32)padState + 5);
        copyA = ((u32*)newPadState)[0];
        copyB = ((u32*)newPadState)[1];
        ((u32*)padState)[0] = copyA;
        ((u32*)padState)[1] = copyB;
        ((u32*)padState)[2] = ((u32*)newPadState)[2];
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

void DEMOPadRead(void) {
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
    typedef struct DemoPadStatus {
        u16 buttons;
        s8 stickX;
        s8 stickY;
        s8 substickX;
        s8 substickY;
        u8 triggerLeft;
        u8 triggerRight;
        u8 analogA;
        u8 analogB;
        s8 error;
        u8 pad;
        u16 buttonsPressed;
        u16 buttonsReleased;
        u16 stickDirs;
        u16 stickDirsPressed;
        u16 stickDirsReleased;
        s16 stickXDelta;
        s16 stickYDelta;
        s16 substickXDelta;
        s16 substickYDelta;
    } DemoPadStatus;
    extern void PADInit(void);
    DemoPadStatus* pad;
    s32 i;

    PADInit();
    pad = (DemoPadStatus*)DemoPad;
    for (i = 0; i < 4; i++, pad++) {
        pad->buttons = 0;
        pad->stickX = 0;
        pad->stickY = 0;
        pad->substickX = 0;
        pad->substickY = 0;
        pad->triggerLeft = 0;
        pad->triggerRight = 0;
        pad->analogA = 0;
        pad->analogB = 0;
        pad->error = 0;
        pad->buttonsPressed = 0;
        pad->buttonsReleased = 0;
        pad->stickDirs = 0;
        pad->stickDirsPressed = 0;
        pad->stickDirsReleased = 0;
        pad->stickXDelta = 0;
        pad->stickYDelta = 0;
        pad->substickXDelta = 0;
        pad->substickYDelta = 0;
    }
}
