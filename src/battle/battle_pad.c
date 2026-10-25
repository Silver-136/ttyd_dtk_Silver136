/* "battle_pad" - Battle (Game)Pad Helper
 * Helper functions for reading/writing controller inputs.
 */
#include "battle/battle_pad.h"
#include "battle/battle.h"
#include "mario/mariost.h"


void BattlePadInit(void) {
    int i;

    for (i = 0; i < 4; i++) {
        BtlPad_WorkInit(&_battleWorkPointer->padWork[i]);
    }
}

void BattlePadManager(void) {
    int i;

    for (i = 0; i < 4; i++) {
        BtlPad_WorkUpdate(&_battleWorkPointer->padWork[i], i);
    }
}

void BtlPad_WorkInit(BattleWorkPad* pad) {
    int i;

    for (i = 0; i < 2; i++) {
        pad->mPadData[i].button = 0;
        pad->mPadData[i].stickX = 0;
        pad->mPadData[i].stickY = 0;
        pad->mPadData[i].substickX = 0;
        pad->mPadData[i].substickY = 0;
        pad->mPadData[i].triggerLeft = 0;
        pad->mPadData[i].triggerRight = 0;
        pad->mPadData[i].analogA = 0;
        pad->mPadData[i].analogB = 0;
        pad->mPadData[i].err = 0;
    }

    for (i = 0; i < 30; i++) {
        pad->mButtonsPressedHistory[i] = 0;
        pad->mButtonsHeldHistory[i] = 0;
        pad->mButtonsHistory[i] = 0;
    }

    pad->mHoldRepeatState = 0;
    pad->mHoldRepeatFrameCounter = 0;
}

void BtlPad_WorkUpdate(BattleWorkPad* pad, s32 chan) {
    int i, repeat;

    pad->mPadData[1] = pad->mPadData[0];

    for (i = 28; i >= 0; i--) {
        pad->mButtonsPressedHistory[i + 1] = pad->mButtonsPressedHistory[i];
        pad->mButtonsHeldHistory[i + 1] = pad->mButtonsHeldHistory[i];
        pad->mButtonsHistory[i + 1] = pad->mButtonsHistory[i];
    }

    //after moving old data back, write new data
    pad->mPadData[0].button = (u16)gp->button[chan];
    pad->mPadData[0].stickX = gp->stickX[chan];
    pad->mPadData[0].stickY = gp->stickY[chan];
    pad->mPadData[0].substickX = gp->substickX[chan];
    pad->mPadData[0].substickY = gp->substickY[chan];
    pad->mPadData[0].triggerLeft = gp->triggerLeft[chan];
    pad->mPadData[0].triggerRight = gp->triggerRight[chan];
    pad->mPadData[0].analogA = 0;
    pad->mPadData[0].analogB = 0;
    pad->mButtonsHistory[0] = pad->mPadData[0].button;
    pad->mButtonsHeldHistory[0] = gp->buttonRepeat[chan];
    pad->mButtonsPressedHistory[0] = gp->buttonNew[chan];
    pad->mButtonsReleasedHistory[0] = gp->buttonUp[chan];

    if (pad->mPadData[0].stickY >= 30) {
        pad->mButtonsHistory[0] |= (1 << 16);
    }
    if (pad->mPadData[0].stickY <= -30) {
        pad->mButtonsHistory[0] |= (1 << 17);
    }

    if (pad->mPadData[0].stickX <= -30) {
        pad->mButtonsHistory[0] |= (1 << 18);
    }
    if (pad->mPadData[0].stickX >= 30) {
        pad->mButtonsHistory[0] |= (1 << 19);
    }

    if (pad->mPadData[0].substickY >= 30) {
        pad->mButtonsHistory[0] |= (1 << 20);
    }
    if (pad->mPadData[0].substickY <= -30) {
        pad->mButtonsHistory[0] |= (1 << 21);
    }

    if (pad->mPadData[0].substickX <= -30) {
        pad->mButtonsHistory[0] |= (1 << 22);
    }
    if (pad->mPadData[0].substickX >= 30) {
        pad->mButtonsHistory[0] |= (1 << 23);
    }

    pad->mButtonsPressedHistory[0] |= pad->mButtonsHistory[0] & ~pad->mButtonsHistory[1] & 0xFFFF0000;

    if (!pad->mButtonsHistory[0]) {
        pad->mButtonsHeldHistory[0] = 0;
        pad->mHoldRepeatState = 0;
        pad->mHoldRepeatFrameCounter = 0;
    }
    else if (pad->mButtonsHistory[0] != pad->mButtonsHistory[1]) {
        pad->mButtonsHeldHistory[0] |= pad->mButtonsHistory[0] & 0xFFFF0000;
        pad->mButtonsHeldHistory[0] &= pad->mButtonsPressedHistory[0] & 0xFFFF0000 | 0xFFFF;
        pad->mHoldRepeatState = 1;
        pad->mHoldRepeatFrameCounter = 0;
    }
    else {
        repeat = 8;
        pad->mHoldRepeatFrameCounter++;
        if (pad->mHoldRepeatFrameCounter >= (pad->mHoldRepeatState == 1 ? 16 : repeat)) {
            pad->mButtonsHeldHistory[0] |= pad->mButtonsHistory[0] & 0xFFFF0000;
            pad->mHoldRepeatState = 2;
            pad->mHoldRepeatFrameCounter = 0;
        }
    }
    pad->mButtonsReleasedHistory[0] |= pad->mButtonsHistory[1] & (pad->mButtonsHistory[1] ^ pad->mButtonsHistory[0]) & 0xFFFF0000;
}

u32 BattlePadGetTrigger(void) {
    return _battleWorkPointer->padWork[0].mButtonsPressedHistory[0];
}

u32 BattlePadGetNow(void) {
    return _battleWorkPointer->padWork[0].mButtonsHistory[0];
}

BOOL BattlePadCheckTrigger(s32 mask) {
    return BattlePadMultiCheckTrigger(0, mask);
}

BOOL BattlePadMultiCheckTrigger(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].mButtonsPressedHistory;
    return history[0] & mask;
}

BOOL BattlePadCheckRecordTrigger(s32 frame, s32 mask) {
    return BattlePadMultiCheckRecordTrigger(0, frame, mask);
}

u32 BattlePadMultiCheckRecordTrigger(s32 chan, s32 frame, s32 mask) {
    BattleWorkPad* pad = &_battleWorkPointer->padWork[chan];
    return pad->mButtonsPressedHistory[frame] & mask;
}

s32 BattlePadCheckNow(s32 mask) {
    return BattlePadMultiCheckNow(0, mask);
}

BOOL BattlePadMultiCheckNow(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].mButtonsHistory;
    return history[0] & mask;
}

s32 BattlePadCheckUp(s32 mask) {
    return BattlePadMultiCheckUp(0, mask);
}

u32 BattlePadMultiCheckUp(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].mButtonsReleasedHistory;
    return history[0] & mask;
}

s32 BattlePadCheckRepeat(s32 mask) {
    return BattlePadMultiCheckRepeat(0, mask);
}

u32 BattlePadMultiCheckRepeat(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].mButtonsHeldHistory;
    return history[0] & mask;
}
