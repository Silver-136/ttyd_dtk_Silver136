#include "battle/battle_pad.h"

void BtlPad_WorkUpdate(void* work, s32 flags);

extern void* _battleWorkPointer;

extern s32 BattlePadMultiCheckNow(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckUp(s32 controller, s32 mask);
extern s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask);
extern void BtlPad_WorkInit(void* work);

void BattlePadInit(void) {
    s32 offset;
    s32 i;
    s32 addr;

    i = 0;
    offset = 0;

    for (; i < 4; i++) {
        addr = offset + 0xF2C;
        addr = (s32)_battleWorkPointer + addr;
        BtlPad_WorkInit((void*)addr);
        offset += 0x1FC;
    }
}


void BattlePadManager(void) {
    s32 offset;
    s32 i;

    i = 0;
    offset = 0;
    for (; i < 4; i++) {
        BtlPad_WorkUpdate((void*)((s32)_battleWorkPointer + offset + 0xF2C), i);
        offset += 0x1FC;
    }
}

void BtlPad_WorkInit(void* work) {
    typedef struct PadData {
        u16 button;
        s8 stickX;
        s8 stickY;
        s8 substickX;
        s8 substickY;
        u8 triggerLeft;
        u8 triggerRight;
        u8 analogA;
        u8 analogB;
        s8 err;
        u8 pad_B;
    } PadData;
    typedef struct BattleWorkPadLocal {
        PadData padData[2];
        u32 buttonsPressedHistory[30];
        u32 buttonsHeldHistory[30];
        u32 buttonsHistory[30];
        u32 buttonsReleasedHistory[30];
        u8 holdRepeatState;
        u8 holdRepeatFrameCounter;
    } BattleWorkPadLocal;
    BattleWorkPadLocal* pad = (BattleWorkPadLocal*)work;
    s32 i;

    for (i = 0; i < 2; i++) {
        pad->padData[i].button = 0;
        pad->padData[i].stickX = 0;
        pad->padData[i].stickY = 0;
        pad->padData[i].substickX = 0;
        pad->padData[i].substickY = 0;
        pad->padData[i].triggerLeft = 0;
        pad->padData[i].triggerRight = 0;
        pad->padData[i].analogA = 0;
        pad->padData[i].analogB = 0;
        pad->padData[i].err = 0;
    }

    for (i = 0; i < 30; i++) {
        pad->buttonsPressedHistory[i] = 0;
        pad->buttonsHeldHistory[i] = 0;
        pad->buttonsHistory[i] = 0;
    }

    pad->holdRepeatState = 0;
    pad->holdRepeatFrameCounter = 0;
}

void BtlPad_WorkUpdate(void* work, s32 chan) {
    typedef struct PadData {
        u16 button;
        s8 stickX;
        s8 stickY;
        s8 substickX;
        s8 substickY;
        u8 triggerLeft;
        u8 triggerRight;
        u8 analogA;
        u8 analogB;
        s8 err;
        u8 pad_B;
    } PadData;
    typedef struct BattleWorkPadLocal {
        PadData padData[2];
        u32 buttonsPressedHistory[30];
        u32 buttonsHeldHistory[30];
        u32 buttonsHistory[30];
        u32 buttonsReleasedHistory[30];
        u8 holdRepeatState;
        u8 holdRepeatFrameCounter;
    } BattleWorkPadLocal;
    typedef struct GamePadLocal {
        u8 pad_0[0x1328];
        u32 button[4];
        u32 buttonNew[4];
        u32 buttonRepeat[4];
        u8 pad_1358[0x10];
        u32 buttonUp[4];
        u8 pad_1378[0x40];
        s8 stickX[4];
        s8 stickY[4];
        s8 substickX[4];
        s8 substickY[4];
        u8 triggerLeft[4];
        u8 triggerRight[4];
    } GamePadLocal;
    extern GamePadLocal* gp;
    BattleWorkPadLocal* pad = (BattleWorkPadLocal*)work;
    s32 i;
    s32 repeat;

    *(u32*)((u32)&pad->padData[1] + 0) =
        *(u32*)((u32)&pad->padData[0] + 0);
    *(u32*)((u32)&pad->padData[1] + 4) =
        *(u32*)((u32)&pad->padData[0] + 4);
    *(u32*)((u32)&pad->padData[1] + 8) =
        *(u32*)((u32)&pad->padData[0] + 8);

    for (i = 0; i < 29; i++) {
        pad->buttonsPressedHistory[30 - i - 1] =
            pad->buttonsPressedHistory[30 - i];
        pad->buttonsHeldHistory[30 - i - 1] =
            pad->buttonsHeldHistory[30 - i];
        pad->buttonsHistory[30 - i - 1] =
            pad->buttonsHistory[30 - i];
    }

    pad->padData[0].button = (u16)gp->button[chan];
    pad->padData[0].stickX = gp->stickX[chan];
    pad->padData[0].stickY = gp->stickY[chan];
    pad->padData[0].substickX = gp->substickX[chan];
    pad->padData[0].substickY = gp->substickY[chan];
    pad->padData[0].triggerLeft = gp->triggerLeft[chan];
    pad->padData[0].triggerRight = gp->triggerRight[chan];
    pad->padData[0].analogA = 0;
    pad->padData[0].analogB = 0;
    pad->buttonsHistory[0] = pad->padData[0].button;
    pad->buttonsHeldHistory[0] = gp->buttonRepeat[chan];
    pad->buttonsPressedHistory[0] = gp->buttonNew[chan];
    pad->buttonsReleasedHistory[0] = gp->buttonUp[chan];

    if (pad->padData[0].stickY >= 30) {
        pad->buttonsHistory[0] |= 1 << 16;
    }
    if (pad->padData[0].stickY <= -30) {
        pad->buttonsHistory[0] |= 1 << 17;
    }
    if (pad->padData[0].stickX <= -30) {
        pad->buttonsHistory[0] |= 1 << 18;
    }
    if (pad->padData[0].stickX >= 30) {
        pad->buttonsHistory[0] |= 1 << 19;
    }
    if (pad->padData[0].substickY >= 30) {
        pad->buttonsHistory[0] |= 1 << 20;
    }
    if (pad->padData[0].substickY <= -30) {
        pad->buttonsHistory[0] |= 1 << 21;
    }
    if (pad->padData[0].substickX <= -30) {
        pad->buttonsHistory[0] |= 1 << 22;
    }
    if (pad->padData[0].substickX >= 30) {
        pad->buttonsHistory[0] |= 1 << 23;
    }

    pad->buttonsPressedHistory[0] |=
        pad->buttonsHistory[0] & ~pad->buttonsHistory[1] & 0xFFFF0000;

    if (pad->buttonsHistory[0]) {
        if (pad->buttonsHistory[0] == pad->buttonsHistory[1]) {
            repeat = 8;
            pad->holdRepeatFrameCounter++;
            if (pad->holdRepeatState == 1) {
                repeat = 16;
            }
            if ((s8)pad->holdRepeatFrameCounter >= repeat) {
                pad->buttonsHeldHistory[0] |=
                    pad->buttonsHistory[0] & 0xFFFF0000;
                pad->holdRepeatState = 2;
                pad->holdRepeatFrameCounter = 0;
            }
        } else {
            pad->buttonsHeldHistory[0] |=
                pad->buttonsHistory[0] & 0xFFFF0000;
            pad->buttonsHeldHistory[0] &=
                pad->buttonsPressedHistory[0] & 0xFFFF0000 | 0xFFFF;
            pad->holdRepeatState = 1;
            pad->holdRepeatFrameCounter = 0;
        }
    } else {
        pad->buttonsHeldHistory[0] = 0;
        pad->holdRepeatState = 0;
        pad->holdRepeatFrameCounter = 0;
    }

    pad->buttonsReleasedHistory[0] |=
        pad->buttonsHistory[1] &
        (pad->buttonsHistory[1] ^ pad->buttonsHistory[0]) & 0xFFFF0000;
}

s32 BattlePadGetTrigger(void) {
    return *(s32*)((s32)_battleWorkPointer + 0xF44);
}

s32 BattlePadGetNow(void) {
    return *(s32*)((s32)_battleWorkPointer + 0x1034);
}

s32 BattlePadCheckTrigger(s32 mask) {
    return BattlePadMultiCheckTrigger(0, mask);
}

s32 BattlePadMultiCheckTrigger(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44);
    return value & mask;
}

s32 BattlePadCheckRecordTrigger(s32 recordIndex, s32 mask) {
    return BattlePadMultiCheckRecordTrigger(0, recordIndex, mask);
}

s32 BattlePadMultiCheckRecordTrigger(s32 controller, s32 recordIndex, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xF44 + recordIndex * 4);
    return value & mask;
}

s32 BattlePadCheckNow(s32 mask) {
    return BattlePadMultiCheckNow(0, mask);
}

s32 BattlePadMultiCheckNow(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x1034);
    return value & mask;
}

s32 BattlePadCheckUp(s32 mask) {
    return BattlePadMultiCheckUp(0, mask);
}

s32 BattlePadMultiCheckUp(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0x10AC);
    return value & mask;
}

s32 BattlePadCheckRepeat(s32 mask) {
    return BattlePadMultiCheckRepeat(0, mask);
}

s32 BattlePadMultiCheckRepeat(s32 controller, s32 mask) {
    s32 value = *(s32*)((s32)_battleWorkPointer + controller * 0x1FC + 0xFBC);
    return value & mask;
}
