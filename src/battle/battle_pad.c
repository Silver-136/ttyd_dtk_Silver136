/* "battle_pad" - Battle (Game)Pad Helper
 * Helper functions for reading/writing controller inputs.
 */
#include "battle/battle_pad.h"
#include "battle/battle.h"
#include "mario/mariost.h"


/** Clears the battle input history of all four controllers; called from the battle's init in battle.c. */
void BattlePadInit(void) {
    int i;

    for (i = 0; i < 4; i++) {
        BtlPad_WorkInit(&_battleWorkPointer->padWork[i]);
    }
}

/** Records this frame's input for all four controllers; called once per frame from the battle main loop in battle.c. */
void BattlePadManager(void) {
    int i;

    for (i = 0; i < 4; i++) {
        BtlPad_WorkUpdate(&_battleWorkPointer->padWork[i], i);
    }
}

/**
 * Zeroes one controller's battle input record: the current and previous pad state,
 * the 30-frame button histories and the hold-repeat timer.
 */
void BtlPad_WorkInit(BattleWorkPad* pad) {
    int i;

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

void BtlPad_WorkUpdate(BattleWorkPad* pad, s32 chan) {
    int i;

    pad->padData[1] = pad->padData[0];

    for (i = 28; i >= 0; i--) {
        pad->buttonsPressedHistory[i + 1] = pad->buttonsPressedHistory[i];
        pad->buttonsHeldHistory[i + 1] = pad->buttonsHeldHistory[i];
        pad->buttonsHistory[i + 1] = pad->buttonsHistory[i];
    }

    // after shifting the history back one frame, store this frame's input
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
        pad->buttonsHistory[0] |= BATTLE_PAD_STICK_UP;
    }
    if (pad->padData[0].stickY <= -30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_STICK_DOWN;
    }

    if (pad->padData[0].stickX <= -30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_STICK_LEFT;
    }
    if (pad->padData[0].stickX >= 30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_STICK_RIGHT;
    }

    if (pad->padData[0].substickY >= 30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_SUBSTICK_UP;
    }
    if (pad->padData[0].substickY <= -30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_SUBSTICK_DOWN;
    }

    if (pad->padData[0].substickX <= -30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_SUBSTICK_LEFT;
    }
    if (pad->padData[0].substickX >= 30) {
        pad->buttonsHistory[0] |= BATTLE_PAD_SUBSTICK_RIGHT;
    }

    pad->buttonsPressedHistory[0] |= pad->buttonsHistory[0] & ~pad->buttonsHistory[1] & 0xFFFF0000;

    if (!pad->buttonsHistory[0]) {
        pad->buttonsHeldHistory[0] = 0;
        pad->holdRepeatState = 0;
        pad->holdRepeatFrameCounter = 0;
    } else if (pad->buttonsHistory[0] != pad->buttonsHistory[1]) {
        pad->buttonsHeldHistory[0] |= pad->buttonsHistory[0] & 0xFFFF0000;
        pad->buttonsHeldHistory[0] &= (pad->buttonsPressedHistory[0] & 0xFFFF0000) | 0xFFFF;
        pad->holdRepeatState = 1;
        pad->holdRepeatFrameCounter = 0;
    } else {
        pad->holdRepeatFrameCounter++;
        if (pad->holdRepeatFrameCounter >= (pad->holdRepeatState == 1 ? 16 : 8)) {
            pad->buttonsHeldHistory[0] |= pad->buttonsHistory[0] & 0xFFFF0000;
            pad->holdRepeatState = 2;
            pad->holdRepeatFrameCounter = 0;
        }
    }
    pad->buttonsReleasedHistory[0] |= pad->buttonsHistory[1] & (pad->buttonsHistory[1] ^ pad->buttonsHistory[0]) & 0xFFFF0000;
}

/** Buttons newly pressed this frame on controller 1, with the stick-direction bits. */
u32 BattlePadGetTrigger(void) {
    return _battleWorkPointer->padWork[0].buttonsPressedHistory[0];
}

/** Buttons held this frame on controller 1, with the stick-direction bits. */
u32 BattlePadGetNow(void) {
    return _battleWorkPointer->padWork[0].buttonsHistory[0];
}

/** Nonzero if any button in mask was newly pressed this frame on controller 1. */
BOOL BattlePadCheckTrigger(s32 mask) {
    return BattlePadMultiCheckTrigger(0, mask);
}

/** Nonzero if any button in mask was newly pressed this frame on controller chan. */
BOOL BattlePadMultiCheckTrigger(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].buttonsPressedHistory;
    return history[0] & mask;
}

/**
 * Nonzero if any button in mask was newly pressed on controller 1 frame frames ago (0 = this frame, up to 29).
 * Timed action commands use it to accept a press made a few frames early.
 */
BOOL BattlePadCheckRecordTrigger(s32 frame, s32 mask) {
    return BattlePadMultiCheckRecordTrigger(0, frame, mask);
}

/** Nonzero if any button in mask was newly pressed on controller chan frame frames ago (0 = this frame, up to 29). */
u32 BattlePadMultiCheckRecordTrigger(s32 chan, s32 frame, s32 mask) {
    BattleWorkPad* pad = &_battleWorkPointer->padWork[chan];
    return pad->buttonsPressedHistory[frame] & mask;
}

/** Nonzero if any button in mask is held this frame on controller 1. */
s32 BattlePadCheckNow(s32 mask) {
    return BattlePadMultiCheckNow(0, mask);
}

/** Nonzero if any button in mask is held this frame on controller chan. */
BOOL BattlePadMultiCheckNow(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].buttonsHistory;
    return history[0] & mask;
}

/** Nonzero if any button in mask was released this frame on controller 1. */
s32 BattlePadCheckUp(s32 mask) {
    return BattlePadMultiCheckUp(0, mask);
}

/** Nonzero if any button in mask was released this frame on controller chan. */
u32 BattlePadMultiCheckUp(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].buttonsReleasedHistory;
    return history[0] & mask;
}

/** Nonzero if any button in mask fired its press-or-hold repeat this frame on controller 1 (menu cursor movement). */
s32 BattlePadCheckRepeat(s32 mask) {
    return BattlePadMultiCheckRepeat(0, mask);
}

/** Nonzero if any button in mask fired its press-or-hold repeat this frame on controller chan. */
u32 BattlePadMultiCheckRepeat(s32 chan, s32 mask) {
    u32* history = _battleWorkPointer->padWork[chan].buttonsHeldHistory;
    return history[0] & mask;
}
