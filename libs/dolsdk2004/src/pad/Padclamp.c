#include <math.h>
#include <dolphin.h>
#include <dolphin/pad.h>

// FAKE: wrapper pads the object to the original 12-byte symbol size
#ifdef VERSION_G8ME01
// US object is the bare 10-byte region (section ends at 0x803041D2)
static const struct { PADClampRegion r; } ClampRegion = { {
#else
static const struct { PADClampRegion r; u8 pad[2]; } ClampRegion = { {
#endif
    // Triggers
    30,
    180,

    // Left stick
    15,
    72,
    40,

    // Right stick
    15,
    59,
    31,

    // Stick radii
    56,
    44,
} };

// prototypes
static void ClampStick(s8* px, s8* py, s8 max, s8 xy, s8 min);
static void ClampCircle(s8* px, s8* py, s8 radius, s8 min);
static void ClampTrigger(u8* trigger, u8 min, u8 max);

static void ClampStick(s8* px, s8* py, s8 max, s8 xy, s8 min) {
    int x = *px;
    int y = *py;
    int signX;
    int signY;
    int d;

    if (0 <= x) {
        signX = 1;
    } else {
        signX = -1;
        x = -x;
    }

    if (0 <= y) {
        signY = 1;
    } else {
        signY = -1;
        y = -y;
    }

    if (x <= min) {
        x = 0;
    } else {
        x -= min;
    }
    if (y <= min) {
        y = 0;
    } else {
        y -= min;
    }

    if (x == 0 && y == 0) {
        *px = *py = 0;
        return;
    }

    if (xy * y <= xy * x) {
        d = xy * x + (max - xy) * y;
        if (xy * max < d) {
            x = (s8)(xy * max * x / d);
            y = (s8)(xy * max * y / d);
        }
    } else {
        d = xy * y + (max - xy) * x;
        if (xy * max < d) {
            x = (s8)(xy * max * x / d);
            y = (s8)(xy * max * y / d);
        }
    }

    *px = (s8)(signX * x);
    *py = (s8)(signY * y);
}

static void ClampCircle(s8* px, s8* py, s8 radius, s8 min) {
    int x = *px;
    int y = *py;
    int squared;
    int length;

    if (-min < x && x < min) {
        x = 0;
    } else if (0 < x) {
        x -= min;
    } else {
        x += min;
    }

    if (-min < y && y < min) {
        y = 0;
    } else if (0 < y) {
        y -= min;
    } else {
        y += min;
    }

    squared = x * x + y * y;
    if (radius * radius < squared) {
        length = sqrtf(squared);
        x = (x * radius) / length;
        y = (y * radius) / length;
    }

    *px = x;
    *py = y;
}

static void ClampTrigger(u8* trigger, u8 min, u8 max) {
    if (*trigger <= min) {
        *trigger = 0;
    } else {
        if (max < *trigger) {
            *trigger = max;
        }
        *trigger -= min;
    }
}

void PADClamp(PADStatus * status) {
    int i;

    for (i = 0; i < 4; i++, status++) {
        if (status->err == PAD_ERR_NONE) {
            ClampStick(&status->stickX, &status->stickY, ClampRegion.r.maxStick, ClampRegion.r.xyStick, ClampRegion.r.minStick);
            ClampStick(&status->substickX, &status->substickY, ClampRegion.r.maxSubstick, ClampRegion.r.xySubstick, ClampRegion.r.minSubstick);
            ClampTrigger(&status->triggerLeft, ClampRegion.r.minTrigger, ClampRegion.r.maxTrigger);
            ClampTrigger(&status->triggerRight, ClampRegion.r.minTrigger, ClampRegion.r.maxTrigger);
        }
    }
}

void PADClampCircle(PADStatus* status) {
    int i;
    for (i = 0; i < 4; ++i, status++) {
        if (status->err == PAD_ERR_NONE) {
            ClampCircle(&status->stickX, &status->stickY, ClampRegion.r.radStick, ClampRegion.r.minStick);
            ClampCircle(&status->substickX, &status->substickY, ClampRegion.r.radSubstick, ClampRegion.r.minSubstick);
            ClampTrigger(&status->triggerLeft, ClampRegion.r.minTrigger, ClampRegion.r.maxTrigger);
            ClampTrigger(&status->triggerRight, ClampRegion.r.minTrigger, ClampRegion.r.maxTrigger);
        }
    }
}
