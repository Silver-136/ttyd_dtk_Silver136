#include <dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

static OSResetCallback ResetCallback;
static BOOL Down;
static BOOL LastState;
static OSTime HoldUp;
static OSTime HoldDown;

// __PIRegs[0] bit 0x10000 is clear while the reset button is down; writing 2 clears the reset-switch
// interrupt (cause bit 0x2). The press must last 100 us before the callback runs.
void __OSResetSWInterruptHandler(s16 exception, OSContext* context) {
    OSResetCallback callback;

    HoldDown = __OSGetSystemTime();
    while (__OSGetSystemTime() - HoldDown < OSMicrosecondsToTicks(100) &&
           !(__PIRegs[0] & 0x00010000)) {
        ;
    }
    if (!(__PIRegs[0] & 0x00010000)) {
        LastState = Down = TRUE;
        __OSMaskInterrupts(OS_INTERRUPTMASK_PI_RSW);
        if (ResetCallback) {
            callback = ResetCallback;
            ResetCallback = NULL;
            callback();
        }
    }
    __PIRegs[0] = 2;
}

OSResetCallback OSSetResetCallback(OSResetCallback callback) {
    BOOL enabled;
    OSResetCallback prevCallback;

    enabled = OSDisableInterrupts();
    prevCallback = ResetCallback;
    ResetCallback = callback;

    if (callback) {
        __PIRegs[0] = 2;
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_RSW);
    } else {
        __OSMaskInterrupts(OS_INTERRUPTMASK_PI_RSW);
    }
    OSRestoreInterrupts(enabled);
    return prevCallback;
}

// A press counts once held for 100 us; after a counted press is released the state stays TRUE for 40 ms.
// When the timer in the low 5 bits of __gUnknown800030E3 (minutes, see __OSSetResetButtonTimer) has run
// out since start-up, the state alternates every 2 seconds.
BOOL OSGetResetButtonState(void) {
    BOOL enabled = OSDisableInterrupts();
    int state;
    u32 reg;
    OSTime now;

    now = __OSGetSystemTime();
    ASSERTLINE(158, 0 <= now);
    ASSERTLINE(159, HoldUp == 0 || HoldUp < now);
    ASSERTLINE(160, HoldDown == 0 || HoldDown < now);

    reg = __PIRegs[0];
    if (!(reg & 0x00010000)) {
        if (!Down) {
            Down = TRUE;
            state = HoldUp ? TRUE : FALSE;
            HoldDown = now;
        } else {
            state = HoldUp || (OSMicrosecondsToTicks(100) < now - HoldDown)
                        ? TRUE
                        : FALSE;
        }
    } else if (Down) {
        Down = FALSE;
        state = LastState;
        if (state) {
            HoldUp = now;
        } else {
            HoldUp = 0;
        }
    } else if (HoldUp && (now - HoldUp < OSMillisecondsToTicks(40))) {
        state = TRUE;
    } else {
        state = FALSE;
        HoldUp = 0;
    }

    LastState = state;

    if (__gUnknown800030E3 & 0x1F) {
        OSTime fire = (__gUnknown800030E3 & 0x1F) * 60;
        fire = __OSStartTime + OSSecondsToTicks(fire);
        if (fire < now) {
            now -= fire;
            now = OSTicksToSeconds(now) / 2;
            if ((now & 1) == 0) {
                state = TRUE;
            } else {
                state = FALSE;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return state;
}

int OSGetResetSwitchState(void) {
    return OSGetResetButtonState();
}

void __OSSetResetButtonTimer(u8 min) {
    BOOL enabled = OSDisableInterrupts();
    if (min > 0x1F) {
        min = 0x1F;
    }

    __gUnknown800030E3 &= ~0x1F;
    __gUnknown800030E3 |= min;
    OSRestoreInterrupts(enabled);
}
