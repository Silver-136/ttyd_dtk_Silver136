/* "system" - Useful Functions
 * Lots of helper functions for complex math.
 */

#include "version.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/pad.h>
#include <math.h>
#include <stdlib.h>
#include <demo/DEMOPad.h>
#include "mario/mariost.h"
#include "R_build.h"
#include "driver/mapdrv.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


#define HALF_PI 1.570796f
#define PI 3.141592653589793f
#define TWO_PI 6.283185307179586f

//.data
static f32 angleABTBL[] = { 1.0f,       1.273187f,  1.27303f,   1.272768f,  1.272402f,  1.271932f,  1.271358f,
                            1.270681f,  1.269902f,  1.269021f,  1.268038f,  1.266956f,  1.265774f,  1.2644939f,
                            1.263116f,  1.2616431f, 1.260075f,  1.258413f,  1.256659f,  1.254815f,  1.2528811f,
                            1.250859f,  1.248752f,  1.24656f,   1.244285f,  1.2419291f, 1.239494f,  1.236981f,
                            1.234393f,  1.2317311f, 1.228997f,  1.226192f,  1.22332f,   1.220382f,  1.217379f,
                            1.2143151f, 1.211189f,  1.208006f,  1.204766f,  1.201471f,  1.1981241f, 1.1947269f,
                            1.191281f,  1.1877871f, 1.18425f,   1.180669f,  1.177047f,  1.173386f,  1.169687f,
                            1.165952f,  1.162184f,  1.158384f,  1.1545531f, 1.1506931f, 1.146806f,  1.142893f,
                            1.138957f,  1.134998f,  1.131018f,  1.127019f,  1.1230021f, 1.118969f,  1.11492f,
                            1.110858f,  1.106783f,  1.102697f,  1.098601f,  1.094496f,  1.090384f,  1.086266f,
                            1.082142f,  1.078014f,  1.0738831f, 1.06975f,   1.065616f,  1.061482f,  1.057348f,
                            1.053216f,  1.049087f,  1.04496f,   1.040838f,  1.036721f,  1.0326101f, 1.028504f,
                            1.024406f,  1.020316f,  1.016234f,  1.0121599f, 1.0080971f, 1.004043f,  1.0f };

/**
 * The build date and time of this release (from R_build.c);
 * the title screen shows them while Z is held in debug mode (JP and later).
 */
const char* build_date_ptr = R_build_date;
const char* build_time_ptr = R_build_time;

/** Returns the root directory for the game's disc file paths: "." (the disc root). */
const char* getMarioStDvdRoot(void) {
    return ".";
}

/**
 * Assertion failure: halts with OSPanic. Only the PAL demo and PAL keep it; the JP demo map lists it UNUSED
 * (0x28), and the other versions strip it too.
 */
BOOL __assert(const char* filename, int line, const char* assertion) {
    OSPanic(filename, line, assertion);
    return TRUE;
}

/**
 * Assertion failure with a printf-style message; formats it and halts with OSPanic. Kept only in the PAL demo
 * and PAL; the JP demo map lists it UNUSED (0xdc). Its format string stays in every version's pool.
 */
BOOL __assert2(const char* filename, int line, const char* assertion, const char* message, ...) {
    char buf[1024];
    va_list args;

    va_start(args, message);
    vsprintf(buf, message, args);
    OSPanic(filename, line, "assertion \"%s\" failed.\n \"%s\"\n", assertion, buf);
    return TRUE;
}

/**
 * Rounds to the nearest integer, halves away from zero; no version calls it (JP demo map: UNUSED, 0x54). The body
 * is angleABf's rounding: it compiles to the map size (the float-constant and plain x - 0.5 forms do not) and adds
 * no pool constants, as the map's numbering shows.
 */
s32 roundi(f32 x) {
    if (x >= 0.0f) {
        return (s32)(0.5 + x);
    } else {
        return -(s32)(0.5 - x);
    }
}

/** Wraps an angle in degrees into [0, 360); NaN becomes 0. */
f32 reviseAngle(f32 angle) {
    f32 result, temp;

    // FAKE: spare temporary temp; result = (f32)fmod(angle, 360.0) changes the object
    temp = (f32)fmod(angle, 360.0);
    result = temp;

    if (result != result) { // check for NaN
        result = 0.0f;
    }

    if (result < 0.0f) {
        result += 360.0f;
        if (result >= 360.0f) {
            result = 0.0f;
        }
    }
    return result;
}

/** Returns the 2D distance between (x1, y1) and (x2, y2). */
f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 xdiff, ydiff;

    ydiff = y2 - y1;
    xdiff = x2 - x1;
    return sqrtf((xdiff * xdiff) + (ydiff * ydiff));
}

/** Returns the signed shortest turn in degrees between the angles from and to (within -180..180). */
f32 compAngle(f32 from, f32 to) {
    if (fabsf(to - from) >= 180.0f) {
        if (to < from) {
            to += 360.0f;
        } else {
            to -= 360.0f;
        }
    }
    return to - from;
}

/**
 * Returns the direction in degrees from (x1, y1) to (x2, y2) in the game's convention (0 toward -y,
 * 90 toward +x; on the field the second axis is z),
 * computed with an arctangent correction table instead of atan2.
 */
f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 xdiff, ydiff;
    f32 xabs, yabs;
    f32 approxX;
    f32 tablePos;
    int tableIndex;
    f32 angle;
    f32 approxY;
    f32 corrected;

    xdiff = x2 - x1;
    ydiff = y2 - y1;
    xabs = fabsf(xdiff);
    yabs = fabsf(ydiff);
    if (xabs > yabs) {
        approxX = 45.0f * (yabs / xabs);
        tablePos = 2.0f * approxX;
        if (tablePos >= 0.0f) {
            tableIndex = (int)(0.5 + tablePos);
        } else {
            tableIndex = -(int)(0.5 - tablePos);
        }
        corrected = approxX * angleABTBL[tableIndex];
        if (xdiff >= 0.0f) {
            if (ydiff >= 0.0f) {
                angle = 90.0f + corrected;
            } else {
                angle = 90.0f - corrected;
            }
        } else {
            if (ydiff >= 0.0f) {
                angle = 270.0f - corrected;
            } else {
                angle = 270.0f + corrected;
            }
        }
    } else {
        if (yabs == 0.0f) {
            return 0.0f;
        } else {
            approxY = 45.0f * (xabs / yabs);
            tablePos = 2.0f * approxY;
            if (tablePos >= 0.0f) {
                tableIndex = (int)(0.5 + tablePos);
            } else {
                tableIndex = -(int)(0.5 - tablePos);
            }
            corrected = approxY * angleABTBL[tableIndex];
            if (ydiff >= 0.0f) {
                if (xdiff >= 0.0f) {
                    angle = 180.0f - corrected;
                } else {
                    angle = 180.0f + corrected;
                }
            } else {
                if (xdiff >= 0.0f) {
                    angle = 0.0f + corrected;
                } else {
                    angle = 360.0f - corrected;
                }
            }
        }
    }
    return angle;
}

/**
 * Interpolates from start to end at step currStep of steps, along curve mode:
 * 0 linear, 1-3 accelerating (power 2-4), 4-6 decelerating, 7-10
 * wobble ("bururun") and bounce curves, 11 sine ease-in-out, 12 sine ease-out, 13 cosine ease-in.
 * Any other mode value is called as a custom curve function. Returns end when steps is 0.
 * Used everywhere for movement, fades and camera moves.
 */
f32 intplGetValue(s32 mode, f32 start, f32 end, s32 currStep, s32 steps) {
    f32 step;
    f32 total;
    f32 f0, f1, f2, f3, f4;

    if (!steps) {
        return end;
    }
    step = (f32)currStep;
    total = (f32)steps;
    switch (mode) {
        case EASING_LINEAR:
            return start + ((step * (end - start)) / total);
        case EASING_QUADRATIC_IN:
            return start + (((step * step) * (end - start)) / (total * total));
        case EASING_CUBIC_IN:
            return start + (((step * step * step) * (end - start)) / (total * total * total));
        case EASING_QUARTIC_IN:
            return start + (((step * step * step * step) * (end - start)) / (total * (total * (total * total))));
        case EASING_WAVE_SHRINKING:
            f2 = (f32)cos(4.0f * (3.141592f * (step / total)));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case EASING_WAVE_SHRINKING_FASTER:
            f2 = (f32)cos((4.0f * (3.141592f * ((step * step) / total))) / ((15.0f * total) / 100.0f));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case EASING_WAVE_GROWING_FASTER:
            return end - ((step * (step * ((end - start) * (f32)cos((4.0f * (3.141592f * ((step * step) / total))) /
                                                                    ((15.0f * total) / 100.0f))))) /
                          (total * total));
        case EASING_QUADRATIC_OUT:
            f1 = (total - step);
            return (start + (end - start)) - (((f1 * f1) * (end - start)) / (total * total));
        case EASING_CUBIC_OUT:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4) * (end - start)) / (total * total * total));
        case EASING_QUARTIC_OUT:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4 * f4) * (end - start)) / (total * total * total * total));
        case EASING_BOUNCE_SHRINKING:
            f2 = (f32)cos((4.0f * (3.141592f * ((step * step) / total))) / ((40.0f * total) / 100.0f));
            f3 = (total - step);
            f1 = (f3 * (f3 * f2)) / (total * total);
            if (f1 < 0.0f) {
                f1 = -f1;
            }
            return -((f1 * (end - start)) - end);
        case EASING_SINE_IN_OUT:
            f3 = (f32)cos((3.141592f * step) / total);
            f2 = (end - start);
            f0 = 0.5f;
            f1 = (f2 * (1.0f - f3));
            return start + (f1 * f0);
        case EASING_SINE_OUT:
            f1 = (f32)sin((HALF_PI * step) / total);
            f0 = (end - start);
            return start + (f0 * f1);
        case EASING_SINE_IN:
            f2 = (f32)cos((HALF_PI * step) / total);
            f1 = (end - start);
            return start + (f1 * (1.0f - f2));
        default:
            return ((f32 (*)(s32, s32, f32, f32))mode)(currStep, steps, start, end);
    }
}

static void* tmp0[0xC00];
static u8 tmp1[0x100];

static QSortCompare comp;

/** Sorts an array of n element pointers with a quicksort, comparing through the callback qqsort stored in comp. */
static void fsort(void** base, u32 n) {
    static void** lo;
    static void** hi;
    static void** gt;
    static void** tail;
    static void* p;
    void** end;
    void** mid;
    void** start;
    void** pv;
    int c;
    int s;
    int b;
    int half;
    u32 g;
    u32 t;
    u32 k;
    int step;
    u32 mask;

    do {
        end = &base[n - 1];
        tail = end;

        if (n == 2) {
            if (comp(base[0], *tail) > 0) {
                p = base[0];
                base[0] = *tail;
                *tail = p;
            }
            return;
        }

        if (n == 4) {
            lo = base;
            mid = tail;
            gt = tail;
            hi = tail - 1;
        } else if (n < 40) {
            mid = &base[n >> 1];
            if (comp(base[0], *tail) > 0) {
                if (comp(base[0], *mid) < 0) {
                    p = *tail;
                    *tail = *mid;
                    *mid = base[0];
                    base[0] = p;
                } else if (comp(*mid, *tail) < 0) {
                    p = base[0];
                    base[0] = *mid;
                    *mid = *tail;
                    *tail = p;
                } else {
                    p = base[0];
                    base[0] = *tail;
                    *tail = p;
                }
            } else {
                if (comp(base[0], *mid) > 0) {
                    p = base[0];
                    base[0] = *mid;
                    *mid = p;
                } else if (comp(*mid, *tail) > 0) {
                    p = *mid;
                    *mid = *tail;
                    *tail = p;
                }
            }
            if (n == 3) {
                return;
            }
            lo = base + 1;
            gt = tail;
            hi = tail - 1;
        } else {
            /* bit length of n */
            mask = 0xFFFFFFFF;
            step = 32;
            k = 63;
            while (mask != 0) {
                t = mask & n;
                half = step >> 1;
                mask &= mask << ((half == 0) ? 1 : half);

                // FAKE: two complementary ifs; if (t != 0) ... else ... changes the object
                if (t != 0)
                    k -= step;
                if (t == 0)
                    mask >>= step;
                step = half;
            }
            b = (k > 32) ? 0 : 32 - k;

            /* s ~ sqrt(n) */
            if (b & 1) {
                s = 1 << (b >> 1);
            } else {
                s = n >> (b >> 1);
            }

            g = n / s;
            mid = &base[(s / 2) * g];
            tail = &base[(s - 1) * g];
            start = base;

            /* quickselect the median of the strided sample to *mid */
            for (;;) {
                lo = start;
                gt = tail;
                hi = tail;
                pv = &start[(tail - start) / 2 / g * g];

            sel_scan:
                while (hi >= lo) {
                    c = comp(*hi, *pv);
                    if (c > 0) {
                        gt = hi;
                    } else if (c < 0) {
                        goto sel_lscan;
                    }
                    hi -= g;
                }
                goto sel_done;
            sel_lscan:
                while (lo < hi) {
                    c = comp(*lo, *pv);
                    if (c >= 0) {
                        p = *lo;
                        *lo = *hi;
                        *hi = p;
                        // FAKE: two complementary ifs; if (c > 0) ... else ... changes the object
                        if (c > 0)
                            gt = hi;
                        if (!(c > 0))
                            pv = hi;
                        lo += g;
                        hi -= g;
                        goto sel_scan;
                    }
                    lo += g;
                }
            sel_done:
                if (gt < pv) {
                    p = *gt;
                    *gt = *pv;
                    *pv = p;
                    gt += g;
                }

                if (gt <= mid) {
                    start = gt;
                } else if (hi >= mid) {
                    tail = hi;
                } else {
                    break;
                }

                if (start >= tail) {
                    break;
                }
            }

            lo = base;
            tail = end;
            gt = end;
            hi = end;
        }

        /* main partition around *mid */
    scan:
        while (hi >= lo) {
            c = comp(*hi, *mid);
            if (c > 0) {
                gt = hi;
            } else if (c < 0) {
                goto lscan;
            }
            hi--;
        }
        goto done;
    lscan:
        while (lo < hi) {
            c = comp(*lo, *mid);
            if (c >= 0) {
                p = *lo;
                *lo = *hi;
                *hi = p;
                // FAKE: two complementary ifs; if (c > 0) ... else ... changes the object
                if (c > 0)
                    gt = hi;
                if (!(c > 0))
                    mid = hi;
                lo++;
                hi--;
                goto scan;
            }
            lo++;
        }
    done:
        if (gt < mid) {
            p = *gt;
            *gt++ = *mid;
            *mid = p;
        }

        /* recurse smaller side, iterate larger side */
        if (hi - base < tail - gt) {
            lo = hi;
            hi = base;
            base = gt;
        } else {
            lo = tail;
            tail = hi;
            hi = gt;
        }
        n = (tail - base) + 1;
        if (lo > hi) {
            fsort(hi, (lo - hi) + 1);
        }
    } while (n > 1);
}

/**
 * Sorts n elements of size bytes at base with compare, like qsort:
 * sorts a table of pointers (at most 0xC00 elements, size at most 0x100 bytes) with fsort,
 * then moves the elements into place by following the permutation cycles.
 */
void qqsort(void* base, u32 n, u32 size, QSortCompare compare) {
    u8* dst;
    void** idx;
    u32 i;
    void** table;
    u8* buf;
    void** slot;
    u8* cur;
    void* p;
    u32 j;
    void** pp;
    u8* src;

    table = tmp0;
    buf = tmp1;
    comp = compare;
    if (n <= 1) {
        return;
    }

    src = (u8*)base;
    pp = table;
    j = 0;
    while (j++ < n) {
        *pp++ = src;
        src += size;
    }

    fsort(table, n);

    dst = (u8*)base;
    idx = table;
    i = 0;
    while (i++ < n) {
        p = *idx;
        if (p != NULL) {
            slot = idx;
            if (p != dst) {
                // FAKE: assignment inside the argument; cur = dst; memcpy(buf, cur, size) changes the object
                memcpy(buf, cur = dst, size);
                do {
                    memcpy(cur, *slot, size);
                    cur = (u8*)*slot;
                    *slot = NULL;
                    slot = &table[(u32)(cur - (u8*)base) / size];
                } while ((p = *slot) != dst);
                memcpy(cur, buf, size);
                *slot = NULL;
            }
        }
        dst += size;
        idx++;
    }
}

/** Returns the DEMO pad library's status for controller i. */
static inline DEMOPadStatus* getDemoPadStatus(u32 i) {
    return DemoPad + i;
}

#if VERSION >= VERSION_US
#define KEY_REPEAT_FRAMES(n) ((gp->framerate * (n)) / 60)
#else
#define KEY_REPEAT_FRAMES(n) (n)
#endif

/**
 * Reads the controllers once per frame (DEMOPadRead) into the global input state:
 * held, newly pressed, released and auto-repeat (24 frames first, then every 6) for buttons
 * and stick directions, the analogue sticks and triggers,
 * and, in the releases, applies rumble changes to the motors when rumble is enabled.
 */
void makeKey(void) {
#if VERSION <= VERSION_DEMO_JP || VERSION == VERSION_DEMO_US_2 || VERSION == VERSION_DEMO_EU
    // the demos keep button/dirs in 32-bit locals
    s32 delay;
    u32 button;
    u32 dirs;
#else
    u16 button;
    u16 dirs;
#endif
    int i;

    DEMOPadRead();
    for (i = 0; i < 4; i++) {
        dirs = getDemoPadStatus(i)->dirs;
        gp->dirsNew[i] = dirs & (dirs ^ gp->dirs[i]);
        gp->dirsRepeat[i] = gp->dirsNew[i];
        if (dirs && dirs == gp->dirs[i]) {
            if (!--gp->dirsRepeatDelay[i]) {
                gp->dirsRepeat[i] = dirs;
                gp->dirsRepeatDelay[i] = KEY_REPEAT_FRAMES(6);
            }
        } else {
            gp->dirsRepeatDelay[i] = KEY_REPEAT_FRAMES(24);
        }
        gp->dirs[i] = dirs;
    }

    for (i = 0; i < 4; i++) {
        button = getDemoPadStatus(i)->pst.button;
        gp->buttonNew[i] = button & (button ^ gp->button[i]);
        gp->buttonRepeat[i] = gp->buttonNew[i];
        if (button && button == gp->button[i]) {
            if (!--gp->buttonRepeatDelay[i]) {
                gp->buttonRepeat[i] = button;
                gp->buttonRepeatDelay[i] = KEY_REPEAT_FRAMES(6);
            }
        } else {
            gp->buttonRepeatDelay[i] = KEY_REPEAT_FRAMES(24);
        }
        gp->button[i] = button;
        gp->buttonUp[i] = getDemoPadStatus(i)->buttonUp;
    }

    for (i = 0; i < 4; i++) {
        gp->stickX[i] = getDemoPadStatus(i)->pst.stickX;
        gp->stickY[i] = getDemoPadStatus(i)->pst.stickY;
        gp->substickX[i] = getDemoPadStatus(i)->pst.substickX;
        gp->substickY[i] = getDemoPadStatus(i)->pst.substickY;
        gp->triggerLeft[i] = getDemoPadStatus(i)->pst.triggerLeft;
        gp->triggerRight[i] = getDemoPadStatus(i)->pst.triggerRight;
    }
#if VERSION >= VERSION_US
    for (i = 0; i < 4; i++) {
        if ((gp->button[i] & (PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START)) == (PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START)) {
            gp->button[i] = 0;
            gp->buttonNew[i] = 0;
            gp->buttonRepeat[i] = 0;
            gp->buttonRepeatDelay[i] = KEY_REPEAT_FRAMES(24);
            gp->buttonUp[i] = 0;
            gp->dirs[i] = 0;
            gp->dirsNew[i] = 0;
            gp->dirsRepeat[i] = 0;
            gp->dirsRepeatDelay[i] = KEY_REPEAT_FRAMES(24);
            gp->stickX[i] = 0;
            gp->stickY[i] = 0;
            gp->substickX[i] = 0;
            gp->substickY[i] = 0;
            gp->triggerLeft[i] = 0;
            gp->triggerRight[i] = 0;
        }
    }
#endif

#if VERSION == VERSION_JP || VERSION == VERSION_US || VERSION == VERSION_EU
    // only the releases drive the rumble motors
    if (gp->rumbleEnabled) {
        for (i = 0; i < 4; i++) {
            if (gp->rumbleHardStop[i]) {
                PADControlMotor(i, PAD_MOTOR_STOP_HARD);
            } else {
                if (gp->prevRumbleStatus[i] != gp->rumbleStatus[i]) {
                    switch (gp->rumbleStatus[i]) {
                        case PAD_MOTOR_STOP:
                            PADControlMotor(i, PAD_MOTOR_STOP);
                            break;
                        case PAD_MOTOR_RUMBLE:
                            PADControlMotor(i, PAD_MOTOR_RUMBLE);
                            break;
                        case PAD_MOTOR_STOP_HARD:
                            PADControlMotor(i, PAD_MOTOR_STOP_HARD);
                            break;
                    }
                    gp->prevRumbleStatus[i] = gp->rumbleStatus[i];
                }
            }
        }
    }
#endif
    gp->keyDataValid = 1;
}

/** Returns the stick and d-pad directions held on controller chan. */
u32 keyGetDir(u32 chan) {
    return gp->dirs[chan];
}

/** Returns the buttons held on controller chan. */
u32 keyGetButton(u32 chan) {
    return gp->button[chan];
}

/** Returns the directions pressed this frame or auto-repeating on controller chan (for menu scrolling). */
u32 keyGetDirRep(u32 chan) {
    return gp->dirsRepeat[chan];
}

/** Returns the buttons pressed this frame or auto-repeating on controller chan. */
u32 keyGetButtonRep(u32 chan) {
    return gp->buttonRepeat[chan];
}

/** Returns the directions newly pressed this frame on controller chan. */
u32 keyGetDirTrg(u32 chan) {
    return gp->dirsNew[chan];
}

/** Returns the buttons newly pressed this frame on controller chan. */
u32 keyGetButtonTrg(u32 chan) {
    return gp->buttonNew[chan];
}

/** Returns the main stick's x position on controller chan (-128..127). */
s8 keyGetStickX(u32 chan) {
    return gp->stickX[chan];
}

/** Returns the main stick's y position on controller chan (-128..127). */
s8 keyGetStickY(u32 chan) {
    return gp->stickY[chan];
}

/** Returns the C stick's x position on controller chan; no version calls it. */
s8 keyGetSubStickX(u32 chan) {
    return gp->substickX[chan];
}

/** Returns the C stick's y position on controller chan. */
s8 keyGetSubStickY(u32 chan) {
    return gp->substickY[chan];
}

/** Returns the analog L trigger on controller chan; no version calls it (JP demo map: UNUSED, 0x10). */
u8 keyGetTriggerL(u32 chan) {
    return gp->triggerLeft[chan];
}

/** Returns the analog R trigger on controller chan; no version calls it (JP demo map: UNUSED, 0x10). */
u8 keyGetTriggerR(u32 chan) {
    return gp->triggerRight[chan];
}

/** Starts rumble on controller chan (applied by makeKey). */
void padRumbleOn(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_RUMBLE;
}

/** Stops rumble on controller chan, letting the motor spin down. */
void padRumbleOff(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_STOP;
}

/** Stops rumble on controller chan with the motor brake. */
void padRumbleHardOff(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_STOP_HARD;
}

/** Returns the requested rumble state of controller chan. */
u8 padGetRumbleStatus(u32 chan) {
    return gp->rumbleStatus[chan];
}

/** Returns the sine and the negated cosine of an angle in degrees, the game's x/z direction for that facing. */
void sincosf(f32* ret_sin, f32* ret_cos, f32 degree) {
    f32 radian;

    radian = PI * degree / 180.0f;
    *ret_sin = (f32)sin(radian);
    *ret_cos = -(f32)cos(radian);
}

/**
 * Moves the point (*ret_sin, *ret_cos) by dist in direction degree (0 = toward -z, clockwise),
 * the same convention as sincosf.
 */
void movePos(float* ret_sin, float* ret_cos, f32 dist, f32 degree) {
    f32 result1, result2, radian;

    radian = (TWO_PI * degree) / 360.0f;
    result1 = (f32)sin(radian);
    result2 = (f32)cos(radian);
    *ret_sin = (dist * result1) + *ret_sin;
    *ret_cos = -((dist * result2) - *ret_cos);
}

/**
 * FAKE stand-in for the dead-stripped movePos2 (0xc8) and movePos3 (0xd0) of the JP demo map, which sat here:
 * their double constants 1.5, 2.5 and 1.7 (map @1111, @1112, @1117) stay in .rodata. Their bodies are not known.
 */
void systemStrippedPool1b(f64* d) {
    d[0] = 1.5;
    d[1] = 2.5;
    d[2] = 1.7;
}

/** Returns a random integer in [0, scalar), or 0 when scalar is 0. */
s32 irand(s32 scalar) {
    if (scalar == 0) {
        return 0;
    }
    return rand() % scalar;
}

/** Returns the next 16-bit draw sync token. */
u16 sysGetToken(void) {
    static u16 token;
    return token++;
}

/**
 * Waits (at most 100 ms) for the GPU to finish the commands sent so far, using a draw sync token;
 * skipped while the map driver is recording a display list.
 */
void sysWaitDrawSync(void) {
    u16 token = sysGetToken();
    OSTick tick = OSGetTick();
    if (!__mapdrv_make_dl) {
        GXSetDrawSync(token);
        while (GXReadDrawSync() != token) {
            if (OSTicksToMilliseconds(OSGetTick() - tick) > 100) {
                break;
            }
        }
    }
}

/** Draws an invisible triangle with a plain TEV setup through mtx, leaving the GX state in a known default. */
void sysDummyDraw(Mtx mtx) {
    f32 vtxMin, vtxMax;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_ONE, GX_LO_OR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80u, GX_AOP_OR, GX_NEVER, 0);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, 0x81 });
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
    vtxMin = 0.0f;
    vtxMax = 20.0f;
    GXPosition3f32(vtxMin, vtxMin, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(vtxMax, vtxMin, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(vtxMax, vtxMax, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
}

/** Scales a per-frame (60 fps) amount by the real time between two OSTime stamps. */
f32 getV60FPS(f32 perFrame, s64 startTick, s64 endTick) {
    u64 startMs, endMs;

    startMs = (u64)OSTicksToMilliseconds(startTick);
    endMs = (u64)OSTicksToMilliseconds(endTick);
    if (endMs >= startMs) {
        endMs = endMs - startMs;
    } else {
        endMs = (-1 - startMs) + endMs;
    }
    return ((60.0f * perFrame) * (f32)endMs) / 1000.0f;
}

/**
 * Converts seconds to frames at 60 frames per second; no version calls it (JP demo map: UNUSED, 0x8).
 * The stripped converters (sysSec2Frame* and sysFrame2Sec/Msec*) are written at 60 frames per second, which fits
 * their JP demo map sizes (the JP demo strips them too); whether US and later switched them to gp->framerate, like the kept ones, is unknown.
 */
s32 sysSec2Frame(s32 sec) {
    return sec * 60;
}

/** Converts seconds to frames as a float at 60 frames per second; no version calls it (JP demo map: UNUSED, 0xc). */
f32 sysSec2FrameFloat(f32 sec) {
    return sec * 60.0f;
}

/** Converts frames to whole seconds at 60 frames per second; no version calls it (JP demo map: UNUSED, 0x20). */
s32 sysFrame2Sec(s32 frame) {
    return frame / 60;
}

/** Converts frames to seconds at 60 frames per second. */
f32 sysFrame2SecFloat(f32 frame) {
    return frame / 60.0f;
}

/**
 * Converts milliseconds to whole frames at the current frame rate (50 or 60 in US and later;
 * always 60 in JP and the early demos).
 */
s32 sysMsec2Frame(s32 msec) {
#if VERSION >= VERSION_US
    return (msec * gp->framerate) / 1000;
#else
    return (msec * 60) / 1000;
#endif
}

/** Converts milliseconds to frames as a float, at the current frame rate (60 in JP and the early demos). */
f32 sysMsec2FrameFloat(f32 msec) {
#if VERSION >= VERSION_US
    return (msec * (f32)gp->framerate) / 1000.0f;
#else
    return (msec * 60.0f) / 1000.0f;
#endif
}

/**
 * Converts frames to milliseconds at 60 frames per second, the inverse of sysMsec2Frame; no version calls it
 * (JP demo map: UNUSED, 0x24).
 */
s32 sysFrame2Msec(s32 frame) {
    return (frame * 1000) / 60;
}

/**
 * Converts frames to milliseconds as a float at 60 frames per second; no version calls it (JP demo map: UNUSED,
 * 0x14). Its 1000.0f is the first float constant of the pool after the GX colour.
 */
f32 sysFrame2MsecFloat(f32 frame) {
    return (frame * 1000.0f) / 60.0f;
}

/**
 * Prints a printf-style debug line with file and line through OSReport. Only the PAL demo and PAL keep it (their
 * RELs call it); the JP demo map lists it UNUSED (0xc8) under this name (formerly called sysDebugReport here),
 * and its format string stays in every version's pool.
 */
void sysOSReport(const char* filename, int line, const char* format, ...) {
    char buf[2048];
    va_list args;

    va_start(args, format);
    vsprintf(buf, format, args);
    OSReport("[%8s:% 4d] %s", filename, line, buf);
}

/**
 * Extracts the pure rotation of src into dst: normalises the two named axes (firstAxis, secondAxis: 'x', 'y' or
 * 'z') and rebuilds the third with cross products, dropping scale and translation.
 */
void mtxGetRotationElement(Mtx src, Mtx dst, s8 firstAxis, s8 secondAxis) {
    Vec vecX, vecY, vecZ;
    switch (firstAxis) {
        case 'x':
        case 'X':
            vecX.x = src[0][0];
            vecX.y = src[1][0];
            vecX.z = src[2][0];
            PSVECNormalize(&vecX, &vecX);

            switch (secondAxis) {
                case 'z':
                case 'Z':
                    vecZ.x = src[0][2];
                    vecZ.y = src[1][2];
                    vecZ.z = src[2][2];
                    PSVECNormalize(&vecZ, &vecZ);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    break;

                default:
                    vecY.x = src[0][1];
                    vecY.y = src[1][1];
                    vecY.z = src[2][1];
                    PSVECNormalize(&vecY, &vecY);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    break;
            }
            break;

        case 'y':
        case 'Y':
            vecY.x = src[0][1];
            vecY.y = src[1][1];
            vecY.z = src[2][1];
            PSVECNormalize(&vecY, &vecY);

            switch (secondAxis) {
                case 'x':
                case 'X':
                    vecX.x = src[0][0];
                    vecX.y = src[1][0];
                    vecX.z = src[2][0];
                    PSVECNormalize(&vecX, &vecX);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    break;

                default:
                    vecZ.x = src[0][2];
                    vecZ.y = src[1][2];
                    vecZ.z = src[2][2];
                    PSVECNormalize(&vecZ, &vecZ);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    break;
            }
            break;

        default:
            vecZ.x = src[0][2];
            vecZ.y = src[1][2];
            vecZ.z = src[2][2];
            PSVECNormalize(&vecZ, &vecZ);

            switch (secondAxis) {
                case 'y':
                case 'Y':
                    vecY.x = src[0][1];
                    vecY.y = src[1][1];
                    vecY.z = src[2][1];
                    PSVECNormalize(&vecY, &vecY);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    break;

                default:
                    vecX.x = src[0][0];
                    vecX.y = src[1][0];
                    vecX.z = src[2][0];
                    PSVECNormalize(&vecX, &vecX);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    break;
            }
            break;
    }

    dst[0][0] = vecX.x;
    dst[1][0] = vecX.y;
    dst[2][0] = vecX.z;
    dst[0][1] = vecY.x;
    dst[1][1] = vecY.y;
    dst[2][1] = vecY.z;
    dst[0][2] = vecZ.x;
    dst[1][2] = vecZ.y;
    dst[2][2] = vecZ.z;
    dst[0][3] = 0.0f;
    dst[1][3] = 0.0f;
    dst[2][3] = 0.0f;
}

/**
 * Decompresses Nintendo LZ77 data (header byte 0x10 and a 24-bit size) from inputBuffer into outputBuffer.
 * Returns the decompressed size, 0 when the header is not LZ77.
 */
s32 LZ77Decode(u8* inputBuffer, u8* outputBuffer) {
    u8* outputStart;
    s8 flag;
    s8 bitsLeft;
    s32 bytesWritten;
    s32 expectedSize;
    u8* input;
    int byte1;
    int byte2;
    int size;
    u8* lookback;
    int i;

    outputStart = outputBuffer;
    bitsLeft = 0;
    if (*inputBuffer != 0x10) {
        return 0;
    }
    expectedSize = inputBuffer[1] & 0xFF;
    expectedSize += (inputBuffer[2] << 8) & 0xFF00;
    expectedSize += (inputBuffer[3] << 16) & 0xFF0000;
    input = inputBuffer + 4;
    while (1) {
        bytesWritten = outputBuffer - outputStart;
        if (expectedSize == bytesWritten) {
            break;
        }
        if (!bitsLeft) {
            flag = *input++;
            bitsLeft = 8;
        }
        if (flag >= 0) {
            *outputBuffer++ = *input++;
        } else {
            byte1 = input[0];
            byte2 = input[1];
            size = ((byte1 >> 4) & 0xF) + 3;
            byte1 = ((byte1 & 0xF) << 8);
            byte1 = byte1 + byte2;
            lookback = (outputBuffer - byte1) - 1;
            input += 2;
            for (i = 0; i < size; i++) {
                *outputBuffer++ = *lookback++;
            }
        }
        bitsLeft--;
        flag <<= 1;
    }
    return bytesWritten;
}

// clang-format off
#ifdef __MWERKS__
/**
 * Copies count / 4 words from src to dest. Returns the address of the last word written (r3 is advanced by
 * stwu) and, given count < 4, loops 2^32 times (ctr = 0); no caller uses the result or passes less than one word.
 */
asm void* memcpy_as4(void* dest, const void* src, u32 count) {
    nofralloc
    addi    r3, r3, -4
    srwi    r5, r5, 2
    addi    r4, r4, -4
    mtctr   r5
    @0:
    lwzu    r11, 4(r4)
    stwu    r11, 4(r3)
    bdnz    @0
    blr
}
#else
/**
 * The same word copy in C, except for the result:
 * the asm returns the address of the last word written (r3 is advanced by stwu) and, given count < 4,
 * loops 2^32 times (ctr = 0); no caller uses the result or passes less than one word.
 */
// the same word copy in C
void* memcpy_as4(void* dest, const void* src, u32 count) {
    u32* d = (u32*)dest;
    const u32* s = (const u32*)src;
    u32 n = count >> 2;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}
#endif

#ifdef __MWERKS__
/**
 * Compares count / 4 words of a and b: 0 when all are equal, else 1. Given count < 4 it loops 2^32 times (ctr = 0),
 * like memcpy_as4.
 */
// Original name and size from the JP demo's link map (UNUSED there, 0x34 after memset_as4): nothing calls it, so
// the linker strips it and only its size can be checked. Body from Super Paper Mario's memcmp_as4 (spm-decomp,
// CC0; same size), in asm like the kept memcpy_as4.
asm s32 memcmp_as4(const void* a, const void* b, u32 count) {
    nofralloc
    addi    r3, r3, -4
    srwi    r5, r5, 2
    addi    r4, r4, -4
    mtctr   r5
    @0:
    lwzu    r11, 4(r3)
    lwzu    r12, 4(r4)
    cmplw   r11, r12
    bne     @1
    bdnz    @0
    li      r3, 0
    blr
    @1:
    li      r3, 1
    blr
}
#else
/** Compares count / 4 words of a and b: 0 when all are equal, else 1. */
s32 memcmp_as4(const void* a, const void* b, u32 count) {
    const u32* p = (const u32*)a;
    const u32* q = (const u32*)b;
    u32 n = count >> 2;
    while (n--) {
        if (*p++ != *q++) {
            return 1;
        }
    }
    return 0;
}
#endif
// clang-format on

/**
 * FAKE stand-in for constants of functions the linker dead-stripped (JP demo map: intplStringData and
 * intplStringToEnum UNUSED, mtxGetScaleElement UNUSED 0x75c): the interpolation mode names, which stay in .rodata,
 * then 1.0f and the sqrtf constants. The original table has a 4-byte entry (map @491) between BOUND and SIN_ACCEL
 * that the pool does not show, and JP and later lack ACCEL, DECEL and BOUND, so the table cannot be rebuilt yet.
 * Functions are emitted in reverse source order, so this last function's constants come first.
 */
void systemStrippedPool1(const char** str, f32* f, f32 x) {
    f[0] = 1.0f;
    str[0] = "LINEAR";
#if VERSION <= VERSION_DEMO_JP
    str[1] = "ACCEL";
    str[2] = "DOUBLE_ACCEL";
    str[3] = "TRIPLE_ACCEL";
    str[4] = "DECEL";
    str[5] = "DOUBLE_DECEL";
    str[6] = "TRIPLE_DECEL";
    str[7] = "GEMINI";
    str[8] = "BURURUN";
    str[9] = "BURURUN_DECEL";
    str[10] = "BOUND";
    str[11] = "SIN_ACCEL";
    str[12] = "SIN_DECEL";
#else
    str[1] = "DOUBLE_ACCEL";
    str[2] = "TRIPLE_ACCEL";
    str[3] = "DOUBLE_DECEL";
    str[4] = "TRIPLE_DECEL";
    str[5] = "GEMINI";
    str[6] = "BURURUN";
    str[7] = "BURURUN_DECEL";
    str[8] = "SIN_ACCEL";
    str[9] = "SIN_DECEL";
#endif
    f[1] = sqrtf(x);
}
