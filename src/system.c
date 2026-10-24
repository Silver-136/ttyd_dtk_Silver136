/* "system" - Useful Functions
 * Status: 1:1 for all code, just need to fixup .rodata
 *
 * Function: lots of helper functions for complex math
 *
 * Last Update: 6/9/2026, finally complete fsort and qqsort
 */

#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/pad.h>
#include <math.h>
#include <stdlib.h>
// #include <string.h>
#include <demo/DEMOPad.h>
#include "mario/mariost.h"
#include "R_build.h"
#include "driver/mapdrv.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

#pragma pool_data on

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





const char* build_date_ptr = R_build_date;
const char* build_time_ptr = R_build_time;

const char* systemStrippedPool0(void) {
    return "assertion \"%s\" failed.\n \"%s\"\n\0\0";
}


const char* getMarioStDvdRoot(void) {
    return ".\0\0";
}

f32 reviseAngle(f32 angle) {
    f32 result, temp;

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

f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 xdiff, ydiff;

    ydiff = y2 - y1;
    xdiff = x2 - x1;
    return sqrtf((xdiff * xdiff) + (ydiff * ydiff));
}

f32 compAngle(f32 a1, f32 a2) {
    if (fabsf(a2 - a1) >= 180.0f) {
        if (a2 < a1) {
            a2 += 360.0f;
        } else {
            a2 -= 360.0f;
        }
    }
    return a2 - a1;
}

f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 xdiff, ydiff;
    f32 xabs, yabs;
    f32 v8;
    f32 v9;
    int v10;
    f32 v13;
    f32 v15;
    f32 test;

    xdiff = x2 - x1;
    ydiff = y2 - y1;
    xabs = fabsf(xdiff);
    yabs = fabsf(ydiff);
    if (xabs > yabs) {
        v8 = 45.0f * (yabs / xabs);
        v9 = 2.0f * v8;
        if (v9 >= 0.0f) {
            v10 = (int)(0.5 + v9);
        } else {
            v10 = -(int)(0.5 - v9);
        }
        test = v8 * angleABTBL[v10];
        if (xdiff >= 0.0f) {
            if (ydiff >= 0.0f) {
                v13 = 90.0f + test;
            } else {
                v13 = 90.0f - test;
            }
        } else {
            if (ydiff >= 0.0f) {
                v13 = 270.0f - test;
            } else {
                v13 = 270.0f + test;
            }
        }
    } else {
        if (yabs == 0.0f) {
            return 0.0f;
        } else {
            v15 = 45.0f * (xabs / yabs);
            v9 = 2.0f * v15;
            if (v9 >= 0.0f) {
                v10 = (int)(0.5 + v9);
            } else {
                v10 = -(int)(0.5 - v9);
            }
            test = v15 * angleABTBL[v10];
            if (ydiff >= 0.0f) {
                if (xdiff >= 0.0f) {
                    v13 = 180.0f - test;
                } else {
                    v13 = 180.0f + test;
                }
            } else {
                if (xdiff >= 0.0f) {
                    v13 = 0.0f + test;
                } else {
                    v13 = 360.0f - test;
                }
            }
        }
    }
    return v13;
}

f32 intplGetValue(s32 mode, s32 currStep, s32 steps, f32 start, f32 end) {
    f32 step;
    f32 total;
    f32 f0, f1, f2, f3, f4;

    if (!steps) {
        return end;
    }
    step = (f32)currStep;
    total = (f32)steps;
    switch (mode) {
        case 0:
            return start + ((step * (end - start)) / total);
        case 1:
            return start + (((step * step) * (end - start)) / (total * total));
        case 2:
            return start + (((step * step * step) * (end - start)) / (total * total * total));
        case 3:
            return start + (((step * step * step * step) * (end - start)) / (total * (total * (total * total))));
        case 7:
            f2 = (f32)cos(4.0f * (3.141592f * (step / total)));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case 8:
            f2 = (f32)cos((4.0f * (3.141592f * ((step * step) / total))) / ((15.0f * total) / 100.0f));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case 9:
            return end - ((step * (step * ((end - start) * (f32)cos((4.0f * (3.141592f * ((step * step) / total))) /
                                                                    ((15.0f * total) / 100.0f))))) /
                          (total * total));
        case 4:
            f1 = (total - step);
            return (start + (end - start)) - (((f1 * f1) * (end - start)) / (total * total));
        case 5:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4) * (end - start)) / (total * total * total));
        case 6:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4 * f4) * (end - start)) / (total * total * total * total));
        case 10:
            f2 = (f32)cos((4.0f * (3.141592f * ((step * step) / total))) / ((40.0f * total) / 100.0f));
            f3 = (total - step);
            f1 = (f3 * (f3 * f2)) / (total * total);
            if (f1 < 0.0f) {
                f1 = -f1;
            }
            return -((f1 * (end - start)) - end);
        case 11:
            f3 = (f32)cos((3.141592f * step) / total);
            f2 = (end - start);
            f0 = 0.5f;
            f1 = (f2 * (1.0f - f3));
            return start + (f1 * f0);
        case 12:
            f1 = (f32)sin((HALF_PI * step) / total);
            f0 = (end - start);
            return start + (f0 * f1);
        case 13:
            f2 = (f32)cos((HALF_PI * step) / total);
            f1 = (end - start);
            return start + (f1 * (1.0f - f2));
        default:
            return ((f32 (*)(s32, s32, f32, f32))mode)(currStep, steps, start, end);
    }
}

static void* tmp0[0xC00];
static u8 tmp1[0x100];

static int (*comp)(void*, void*);

void fsort(void** base, u32 n) {
    static void** lo;
    static void** hi;
    static void** gt;
    static void** tail;
    static void* p;
    void** end;   /* r30 */
    void** mid;   /* r29 */
    void** start; /* r28 */
    void** pv;    /* r27 */
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
                    if (c > 0)
                        gt = hi;
                    else if (c < 0)
                        goto sel_lscan;
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

                if (gt <= mid)
                    start = gt;
                else if (hi >= mid)
                    tail = hi;
                else
                    break;

                if (start >= tail)
                    break;
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
            if (c > 0)
                gt = hi;
            else if (c < 0)
                goto lscan;
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

void qqsort(void* base, u32 n, u32 size, int (*compare)(void*, void*)) {
    u8* dst;      /* r27 */
    void** idx;   /* r26 */
    u32 i;        /* r25 */
    void** table; /* r24 */
    u8* buf;      /* r23 */
    void** slot;  /* r22 */
    u8* cur;      /* r21 */
    void* p;      /* r0  */
    u32 j;
    void** pp; /* r4  */
    u8* src;   /* r5  */

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

static inline DEMOPadStatus* GET_DEMO_PAD_STATUS(u32 i) {
    return DemoPad + i;
}

#ifdef VERSION_G8ME01
#define KEYFR(n) ((gp->framerate * (n)) / 60)
#else
#define KEYFR(n) (n)
#endif
void makeKey(void) {
    s32 delay;
    u16 button;
    u16 dirs;
    int i;

    DEMOPadRead();
    for (i = 0; i < 4; i++) {
        dirs = GET_DEMO_PAD_STATUS(i)->dirs;
        gp->dirsNew[i] = dirs & (dirs ^ gp->dirs[i]);
        gp->dirsRepeat[i] = gp->dirsNew[i];
        if (dirs && dirs == gp->dirs[i]) {
            if (!--gp->dirsRepeatDelay[i]) {
                gp->dirsRepeat[i] = dirs;
                gp->dirsRepeatDelay[i] = KEYFR(6);
            }
        } else {
            gp->dirsRepeatDelay[i] = KEYFR(24);
        }
        gp->dirs[i] = dirs;
    }

    for (i = 0; i < 4; i++) {
        button = GET_DEMO_PAD_STATUS(i)->pst.button;
        gp->buttonNew[i] = button & (button ^ gp->button[i]);
        gp->buttonRepeat[i] = gp->buttonNew[i];
        if (button && button == gp->button[i]) {
            if (!--gp->buttonRepeatDelay[i]) {
                gp->buttonRepeat[i] = button;
                gp->buttonRepeatDelay[i] = KEYFR(6);
            }
        } else {
            gp->buttonRepeatDelay[i] = KEYFR(24);
        }
        gp->button[i] = button;
        gp->buttonUp[i] = GET_DEMO_PAD_STATUS(i)->buttonUp;
    }

    for (i = 0; i < 4; i++) {
        gp->stickX[i] = GET_DEMO_PAD_STATUS(i)->pst.stickX;
        gp->stickY[i] = GET_DEMO_PAD_STATUS(i)->pst.stickY;
        gp->substickX[i] = GET_DEMO_PAD_STATUS(i)->pst.substickX;
        gp->substickY[i] = GET_DEMO_PAD_STATUS(i)->pst.substickY;
        gp->triggerLeft[i] = GET_DEMO_PAD_STATUS(i)->pst.triggerLeft;
        gp->triggerRight[i] = GET_DEMO_PAD_STATUS(i)->pst.triggerRight;
    }

#ifdef VERSION_G8ME01
    for (i = 0; i < 4; i++) {
        if ((gp->button[i] & (PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START)) == (PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START)) {
            gp->button[i] = 0;
            gp->buttonNew[i] = 0;
            gp->buttonRepeat[i] = 0;
            gp->buttonRepeatDelay[i] = KEYFR(24);
            gp->buttonUp[i] = 0;
            gp->dirs[i] = 0;
            gp->dirsNew[i] = 0;
            gp->dirsRepeat[i] = 0;
            gp->dirsRepeatDelay[i] = KEYFR(24);
            gp->stickX[i] = 0;
            gp->stickY[i] = 0;
            gp->substickX[i] = 0;
            gp->substickY[i] = 0;
            gp->triggerLeft[i] = 0;
            gp->triggerRight[i] = 0;
        }
    }
#endif

    if (gp->rumbleEnabled) {
        for (i = 0; i < 4; i++) {
            if (gp->rumbleHardStop[i]) {
                PADControlMotor(i, 2);
            } else {
                if (gp->prevRumbleStatus[i] != gp->rumbleStatus[i]) {
                    switch (gp->rumbleStatus[i]) {
                        case 0:
                            PADControlMotor(i, 0);
                            break;
                        case 1:
                            PADControlMotor(i, 1);
                            break;
                        case 2:
                            PADControlMotor(i, 2);
                            break;
                    }
                    gp->prevRumbleStatus[i] = gp->rumbleStatus[i];
                }
            }
        }
    }
    gp->keyDataValid = 1;
}

u32 keyGetDir(u32 chan) {
    return gp->dirs[chan];
}

u32 keyGetButton(u32 chan) {
    return gp->button[chan];
}

u32 keyGetDirRep(u32 chan) {
    return gp->dirsRepeat[chan];
}

u32 keyGetButtonRep(u32 chan) {
    return gp->buttonRepeat[chan];
}

u32 keyGetDirTrg(u32 chan) {
    return gp->dirsNew[chan];
}

u32 keyGetButtonTrg(u32 chan) {
    return gp->buttonNew[chan];
}

s8 keyGetStickX(u32 chan) {
    return gp->stickX[chan];
}

s8 keyGetStickY(u32 chan) {
    return gp->stickY[chan];
}

// unused on retail
s8 keyGetSubStickX(u32 chan) {
    return gp->substickX[chan];
}

s8 keyGetSubStickY(u32 chan) {
    return gp->substickY[chan];
}

void padRumbleOn(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_RUMBLE;
}

void padRumbleOff(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_STOP;
}

void padRumbleHardOff(u32 chan) {
    gp->rumbleStatus[chan] = PAD_MOTOR_STOP_HARD;
}

u8 padGetRumbleStatus(u32 chan) {
    return gp->rumbleStatus[chan];
}

void sincosf(f32* ret_sin, f32* ret_cos, f32 degree) {
    float result; // fp31

    result = PI * degree / 180.0f; // convert to radian
    *ret_sin = (f32)sin(result);
    *ret_cos = -(f32)cos(result);
}

void movePos(float* ret_sin, float* ret_cos, f32 a3, f32 degree) {
    f32 result1, result2, radian;

    radian = (TWO_PI * degree) / 360.0f;
    result1 = (f32)sin(radian);
    result2 = (f32)cos(radian);
    *ret_sin = (a3 * result1) + *ret_sin;
    *ret_cos = -((a3 * result2) - *ret_cos);
}

s32 irand(s32 scalar) {
    if (scalar == 0) {
        return 0;
    }
    return rand() % scalar;
}

u16 sysGetToken(void) {
    static u16 token;
    return token++;
}

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

void sysDummyDraw(Mtx mtx) {
    f32 vtxMin, vtxMax;

    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
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

f32 getV60FPS(f32 a1, s64 a2, s64 a3) {
    u64 v4, v5;

    v4 = (u64)OSTicksToMilliseconds(a2);
    v5 = (u64)OSTicksToMilliseconds(a3);
    if (v5 >= v4) {
        v5 = v5 - v4;
    } else {
        v5 = (-1 - v4) + v5;
    }
    return ((60.0f * a1) * (f32)v5) / 1000.0f;
}

f32 sysFrame2SecFloat(f32 frame) {
    return frame / 60.0f;
}

s32 sysMsec2Frame(s32 msec) {
#ifdef VERSION_G8ME01
    return (msec * gp->framerate) / 1000;
#else
    return (msec * 60) / 1000;
#endif
}

f32 sysMsec2FrameFloat(f32 msec) {
#ifdef VERSION_G8ME01
    return (msec * (f32)gp->framerate) / 1000.0f;
#else
    return (msec * 60.0f) / 1000.0f;
#endif
}

void mtxGetRotationElement(Mtx arg0, Mtx arg1, s8 arg2, s8 arg3) {
    Vec vecX, vecY, vecZ;
    switch (arg2) {
        case 'x':
        case 'X':
            vecX.x = arg0[0][0];
            vecX.y = arg0[1][0];
            vecX.z = arg0[2][0];
            PSVECNormalize(&vecX, &vecX);

            switch (arg3) {
                case 'z':
                case 'Z':
                    vecZ.x = arg0[0][2];
                    vecZ.y = arg0[1][2];
                    vecZ.z = arg0[2][2];
                    PSVECNormalize(&vecZ, &vecZ);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    break;

                default:
                    vecY.x = arg0[0][1];
                    vecY.y = arg0[1][1];
                    vecY.z = arg0[2][1];
                    PSVECNormalize(&vecY, &vecY);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    break;
            }
            break;

        case 'y':
        case 'Y':
            vecY.x = arg0[0][1];
            vecY.y = arg0[1][1];
            vecY.z = arg0[2][1];
            PSVECNormalize(&vecY, &vecY);

            switch (arg3) {
                case 'x':
                case 'X':
                    vecX.x = arg0[0][0];
                    vecX.y = arg0[1][0];
                    vecX.z = arg0[2][0];
                    PSVECNormalize(&vecX, &vecX);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    break;

                default:
                    vecZ.x = arg0[0][2];
                    vecZ.y = arg0[1][2];
                    vecZ.z = arg0[2][2];
                    PSVECNormalize(&vecZ, &vecZ);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    PSVECCrossProduct(&vecX, &vecY, &vecZ);
                    break;
            }
            break;

        default:
            vecZ.x = arg0[0][2];
            vecZ.y = arg0[1][2];
            vecZ.z = arg0[2][2];
            PSVECNormalize(&vecZ, &vecZ);

            switch (arg3) {
                case 'y':
                case 'Y':
                    vecY.x = arg0[0][1];
                    vecY.y = arg0[1][1];
                    vecY.z = arg0[2][1];
                    PSVECNormalize(&vecY, &vecY);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    break;

                default:
                    vecX.x = arg0[0][0];
                    vecX.y = arg0[1][0];
                    vecX.z = arg0[2][0];
                    PSVECNormalize(&vecX, &vecX);
                    PSVECCrossProduct(&vecZ, &vecX, &vecY);
                    PSVECCrossProduct(&vecY, &vecZ, &vecX);
                    break;
            }
            break;
    }

    arg1[0][0] = vecX.x;
    arg1[1][0] = vecX.y;
    arg1[2][0] = vecX.z;
    arg1[0][1] = vecY.x;
    arg1[1][1] = vecY.y;
    arg1[2][1] = vecY.z;
    arg1[0][2] = vecZ.x;
    arg1[1][2] = vecZ.y;
    arg1[2][2] = vecZ.z;
    arg1[0][3] = 0.0f;
    arg1[1][3] = 0.0f;
    arg1[2][3] = 0.0f;
}

s32 LZ77Decode(u8* inputBuffer, u8* outputBuffer) {
    u8* output;
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

    output = outputBuffer;
    bitsLeft = 0;
    if (*inputBuffer != 0x10) {
        return 0;
    }
    expectedSize = inputBuffer[1] & 0xFF;
    expectedSize += (inputBuffer[2] << 8) & 0xFF00;
    expectedSize += (inputBuffer[3] << 16) & 0xFF0000;
    input = inputBuffer + 4;
    while (1) {
        bytesWritten = outputBuffer - output;
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
// clang-format on

// The original file had debug functions that the linker dead-stripped; their constants
// stayed in the literal pools. These stand-ins recreate the pool contents in order.
#ifdef VERSION_G8ME01
// US pools the int->float constant between the strings and the doubles; functions are emitted in
// reverse order, so the doubles come first in the source
void systemStrippedPool1b(f32* f, f64* d) {
    d[0] = 1.5;
    d[1] = 2.5;
    d[2] = 1.7;
    f[2] = 1000.0f;
}

f32 systemStrippedPool2(s32 x) {
    return (f32)x;
}

void systemStrippedPool1(const char** str, f32* f, f64* d, f32 x) {
    f[0] = 1.0f;
    str[0] = "LINEAR";
    str[1] = "DOUBLE_ACCEL";
    str[2] = "TRIPLE_ACCEL";
    str[3] = "DOUBLE_DECEL";
    str[4] = "TRIPLE_DECEL";
    str[5] = "GEMINI";
    str[6] = "BURURUN";
    str[7] = "BURURUN_DECEL";
    str[8] = "SIN_ACCEL";
    str[9] = "SIN_DECEL";
    f[1] = sqrtf(x);
    str[10] = "[%8s:% 4d] %s";
}
#else
void systemStrippedPool1(const char** str, f32* f, f64* d, f32 x) {
    f[0] = 1.0f;
    str[0] = "LINEAR";
    str[1] = "DOUBLE_ACCEL";
    str[2] = "TRIPLE_ACCEL";
    str[3] = "DOUBLE_DECEL";
    str[4] = "TRIPLE_DECEL";
    str[5] = "GEMINI";
    str[6] = "BURURUN";
    str[7] = "BURURUN_DECEL";
    str[8] = "SIN_ACCEL";
    str[9] = "SIN_DECEL";
    f[1] = sqrtf(x);
    str[10] = "[%8s:% 4d] %s";
    d[0] = 1.5;
    d[1] = 2.5;
    d[2] = 1.7;
    f[2] = 1000.0f;
}
#endif

#pragma pool_data off
