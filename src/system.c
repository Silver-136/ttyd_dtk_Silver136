#include "system.h"
#define FSORT_COMP(a, b) comp((const void*)(a), (const void*)(b))
extern s32 rand(void);
extern f64 sin(f64 x);
extern f64 cos(f64 x);
extern s32 gp;
extern u8 DemoPad[];
extern f32 angleABTBL[];
extern void DEMOPadRead(void);
extern void U_PADControlMotor(s32 controller, s32 command);
extern f32 fabsf(f32 x);
static const f32 float_6p2832_8041f458 = 6.2832f;
static const f32 float_360_8041f45c = 360.0f;
extern void* memcpy(void* dst, const void* src, u32 size);
extern void fsort(void** table, u32 count);
typedef f32 (*IntplCallback)(s32 current, s32 total, f32 start, f32 end);
typedef s32 (*QSortCompareFunc)(const void* a, const void* b);
static void* tmp0[256];
static u8 tmp1[0x400];
static QSortCompareFunc comp;
extern const f32 float_180_8041f464;
extern const f32 float_360_8041f45c;
extern f64 fmod(f64 x, f64 y);
extern const f64 double_360_802bf240;
extern const f32 float_0_8041f448;
extern u8 DemoPad[];
extern void U_PADControlMotor(s32 channel, s32 command);
extern const f64 double_0p5_802bf1f8;
extern const f64 double_3_802bf200;
extern const f64 double_0_802bf208;
extern f32 __float_nan[];
extern void GXResetOverflowCount(void);
extern s32 dbg_lotteryinfo;
extern f64 __frsqrte(f64);


u16 sysGetToken(void) {
    static u16 token;

    return token++;
}
s32 irand(s32 range) {
    s32 value;

    if (range == 0) {
        return 0;
    }

    value = rand();
    return value % range;
}
void movePos(f32* x, f32* z, f32 distance, f32 angle) {
    f32 sinValue;
    f32 radians;
    f32 cosValue;

    radians = (float_6p2832_8041f458 * angle) / float_360_8041f45c;

    sinValue = sin(radians);
    cosValue = cos(radians);

    *x += distance * sinValue;
    *z -= distance * cosValue;
}
void sincosf(f32 angle, f32* outSin, f32* outCos) {
    f32 radians;
    f32 sinValue;
    f32 cosValue;

    radians = (3.1416f * angle) / 180.0f;

    sinValue = sin(radians);
    *outSin = sinValue;

    cosValue = cos(radians);
    *outCos = -cosValue;
}
u8 padGetRumbleStatus(s32 controller) {
    controller = gp + controller;

    return *(u8*)(controller + 0x13D0);
}
void padRumbleHardOff(s32 controller) {
    s32 value;

    value = 2;
    controller = gp + controller;

    *(u8*)(controller + 0x13D0) = value;
}
void padRumbleOff(s32 controller) {
    controller = gp + controller;

    *(u8*)(controller + 0x13D0) = 0;
}
void padRumbleOn(s32 controller) {
    controller = gp + controller;

    *(u8*)(controller + 0x13D0) = 1;
}
u8 keyGetSubStickY(s32 controller) {
    controller = gp + controller;

    return *(u8*)(controller + 0x13C4);
}
u8 keyGetStickY(s32 controller) {
    controller = gp + controller;

    return *(u8*)(controller + 0x13BC);
}
u8 keyGetStickX(s32 controller) {
    controller = gp + controller;

    return *(u8*)(controller + 0x13B8);
}
u32 keyGetButtonTrg(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1338);
}
u32 keyGetDirTrg(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1388);
}
u32 keyGetButtonRep(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1348);
}
u32 keyGetDirRep(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1398);
}
u32 keyGetButton(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1328);
}
u32 keyGetDir(s32 controller) {
    controller = gp + controller * 4;

    return *(u32*)(controller + 0x1378);
}
void qqsort(void* base, u32 count, u32 size, QSortCompareFunc compare) {
    void** tableBase;
    void** table;
    void** slot;
    u8* tmp;
    u8* item;
    u8* current;
    u8* next;
    u32 i;
    u32 fillCount;
    u32 chunks;
    u32 remainder;

    tableBase = tmp0;
    tmp = tmp1;
    comp = compare;

    if (count <= 1) {
        return;
    }

    fillCount = count;
    item = base;
    table = tableBase;

    if (fillCount > 0) {
        chunks = fillCount >> 3;

        if (chunks != 0) {
            for (; chunks > 0; chunks--) {
                table[0] = item;
                item += size;
                table[1] = item;
                item += size;
                table[2] = item;
                item += size;
                table[3] = item;
                item += size;
                table[4] = item;
                item += size;
                table[5] = item;
                item += size;
                table[6] = item;
                item += size;
                table[7] = item;
                item += size;
                table += 8;
            }
        }

        remainder = fillCount & 7;

        if (remainder != 0) {
            for (; remainder > 0; remainder--) {
                *table = item;
                item += size;
                table++;
            }
        }
    }

    fsort(tableBase, count);

    item = base;
    table = tableBase;
    i = 0;

    for (; i < count; i++) {
        if (*table != 0) {
            if (*table != item) {
                slot = table;
                current = item;

                memcpy(tmp, item, size);

                do {
                    memcpy(current, *slot, size);

                    next = *slot;
                    *slot = 0;

                    slot = &tableBase[(u32)(next - (u8*)base) / size];
                    current = next;
                } while (*slot != item);

                memcpy(current, tmp, size);
                *slot = 0;
            }
        }

        item += size;
        table++;
    }
}
f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end) {
    f32 cur;
    f32 len;
    f32 delta;
    f32 rem;
    f32 value;
    f32 angle;
    f32 period;

    if (total == 0) {
        return end;
    }

    cur = current;
    len = total;

    switch (type) {
        case 0:
            return start + ((cur * (end - start)) / len);

        case 1:
            return start + ((cur * cur * (end - start)) / (len * len));

        case 2:
            return start + ((cur * cur * cur * (end - start)) / (len * len * len));

        case 3:
            return start + ((cur * cur * cur * cur * (end - start)) / (len * len * len * len));

         case 4:
            angle = 4.0f * (3.1416f * (cur / len));
            value = cos(angle);
            delta = end - start;
            rem = len - cur;
            period = len * len;
            value = delta * value;
            value = rem * value;
            value = rem * value;
            return end - (value / period);

        case 5:
            period = (15.0f * len) / 100.0f;
            angle = (4.0f * (3.1416f * ((cur * cur) / len))) / period;
            value = cos(angle);
            delta = end - start;
            rem = len - cur;
            period = len * len;
            value = delta * value;
            value = rem * value;
            value = rem * value;
            return end - (value / period);

        case 6:
            period = (15.0f * len) / 100.0f;
            angle = (4.0f * (3.1416f * ((cur * cur) / len))) / period;
            value = cos(angle);
            delta = end - start;
            period = len * len;
            value = delta * value;
            value = cur * value;
            value = cur * value;
            return end - (value / period);

        case 7:
            rem = len - cur;
            delta = end - start;
            period = len * len;
            value = start + delta;
            rem = rem * rem;
            rem = rem * delta;
            return value - (rem / period);

        case 8:
            rem = len - cur;
            delta = end - start;
            period = len * len;
            value = start + delta;
            period = len * period;
            rem = rem * rem;
            rem = (len - cur) * rem;
            rem = rem * delta;
            return value - (rem / period);

        case 9:
            rem = len - cur;
            delta = end - start;
            period = len * len;
            value = start + delta;
            period = len * period;
            rem = rem * rem;
            period = len * period;
            rem = (len - cur) * rem;
            rem = (len - cur) * rem;
            rem = rem * delta;
            return value - (rem / period);

        case 10:
            period = (40.0f * len) / 100.0f;
            angle = (4.0f * (3.1416f * ((cur * cur) / len))) / period;
            value = cos(angle);
            rem = len - cur;
            period = len * len;
            value = rem * value;
            value = rem * value;
            value = value / period;

            if (value < 0.0f) {
                value = -value;
            }

            return end - (value * (end - start));

        case 11:
            angle = (3.1416f * cur) / len;
            value = cos(angle);
            return start + (((end - start) * (1.0f - value)) * 0.5f);

        case 12:
            angle = (1.5708f * cur) / len;
            value = sin(angle);
            return start + ((end - start) * value);

        case 13:
            angle = (1.5708f * cur) / len;
            value = cos(angle);
            return start + ((end - start) * (1.0f - value));

        default:
            return ((IntplCallback)type)(current, total, start, end);
    }
}
f32 angleABf(f32 ax, f32 ay, f32 bx, f32 by) {
    f32 dx;
    f32 dy;
    f32 absX;
    f32 absY;
    f32 ratio;
    f32 angle;
    f32 indexValue;
    s32 index;

    dx = bx - ax;
    dy = by - ay;

    absX = __fabsf(dx);
    absY = __fabsf(dy);

    if (absX > absY) {
        ratio = absY / absX;
        angle = 45.0f * ratio;
        indexValue = 2.0f * angle;

        if (indexValue >= 0.0f) {
            index = (s32)(indexValue + 0.5);
        } else {
            index = -(s32)(0.5 - indexValue);
        }

        angle = angle * angleABTBL[index];

        if (dx >= 0.0f) {
            if (dy >= 0.0f) {
                return 90.0f + angle;
            } else {
                return 90.0f - angle;
            }
        } else {
            if (dy >= 0.0f) {
                return 270.0f - angle;
            } else {
                return 270.0f + angle;
            }
        }
    } else {
        if (absY == 0.0f) {
            return 0.0f;
        }

        ratio = absX / absY;
indexValue = 45.0f * ratio;
angle = 2.0f * indexValue;

if (angle >= 0.0f) {
    index = (s32)(angle + 0.5);
} else {
    index = -(s32)(0.5 - angle);
}

indexValue = indexValue * angleABTBL[index];

if (dy >= 0.0f) {
    if (dx >= 0.0f) {
        return 180.0f - indexValue;
    } else {
        return 180.0f + indexValue;
    }
} else {
    if (dx >= 0.0f) {
        return indexValue;
    } else {
        return 360.0f - indexValue;
    }
}
    }
}
f32 compAngle(f32 angleA, f32 angleB) {
    if (__fabsf(angleB - angleA) >= float_180_8041f464) {
        if (angleB < angleA) {
            angleB += float_360_8041f45c;
        } else {
            angleB -= float_360_8041f45c;
        }
    }

    return angleB - angleA;
}
f32 reviseAngle(f32 angle) {
    f32 value;

    value = fmod(angle, double_360_802bf240);
    angle = value;

    if (value != value) {
        angle = float_0_8041f448;
    }

    if (angle < float_0_8041f448) {
        angle += float_360_8041f45c;

        if (angle >= float_360_8041f45c) {
            angle = float_0_8041f448;
        }
    }

    return angle;
}
char* getMarioStDvdRoot(void) {
    return "";
}
void makeKey(void) {
    s32 padOfs;
    s32 wordOfs;
    s32 byteOfs;
    s32 count;
    s32 slow;
    s32 status;
    u32 value;
    u32 old;
    u32 trigger;
    u32 rumbleIndex;
    u8 rumbleStatus;

    DEMOPadRead();

    padOfs = 0;
    wordOfs = 0;
    for (count = 0; count < 4; count++) {
        value = *(u16*)(DemoPad + padOfs + 0x10);

        old = *(u32*)(gp + wordOfs + 0x1378);
        trigger = value & (value ^ old);
        *(u32*)(gp + wordOfs + 0x1388) = trigger;
        *(u32*)(gp + wordOfs + 0x1398) = trigger;

        if ((value == 0) || (value != *(u32*)(gp + wordOfs + 0x1378))) {
            slow = *(s32*)(gp + 4) * 0x18;
            slow = slow / 0x3C + (slow >> 31);
            slow = slow - (slow >> 31);
            *(s32*)(gp + wordOfs + 0x13A8) = slow;
        } else {
            slow = *(s32*)(gp + wordOfs + 0x13A8) - 1;
            *(s32*)(gp + wordOfs + 0x13A8) = slow;

            if (slow == 0) {
                *(u32*)(gp + wordOfs + 0x1398) = value;

                slow = *(s32*)(gp + 4) * 6;
                slow = slow / 0x3C + (slow >> 31);
                slow = slow - (slow >> 31);
                *(s32*)(gp + wordOfs + 0x13A8) = slow;
            }
        }

        *(u32*)(gp + wordOfs + 0x1378) = value;

        padOfs += 0x1E;
        wordOfs += 4;
    }

    padOfs = 0;
    wordOfs = 0;
    for (count = 0; count < 4; count++) {
        value = *(u16*)(DemoPad + padOfs + 0x0);

        old = *(u32*)(gp + wordOfs + 0x1328);
        trigger = value & (value ^ old);
        *(u32*)(gp + wordOfs + 0x1338) = trigger;
        *(u32*)(gp + wordOfs + 0x1348) = trigger;

        if ((value == 0) || (value != *(u32*)(gp + wordOfs + 0x1328))) {
            slow = *(s32*)(gp + 4) * 0x18;
            slow = slow / 0x3C + (slow >> 31);
            slow = slow - (slow >> 31);
            *(s32*)(gp + wordOfs + 0x1358) = slow;
        } else {
            slow = *(s32*)(gp + wordOfs + 0x1358) - 1;
            *(s32*)(gp + wordOfs + 0x1358) = slow;

            if (slow == 0) {
                *(u32*)(gp + wordOfs + 0x1348) = value;

                slow = *(s32*)(gp + 4) * 6;
                slow = slow / 0x3C + (slow >> 31);
                slow = slow - (slow >> 31);
                *(s32*)(gp + wordOfs + 0x1358) = slow;
            }
        }

        *(u32*)(gp + wordOfs + 0x1328) = value;
        *(u32*)(gp + wordOfs + 0x1368) = *(u16*)(DemoPad + padOfs + 0xE);

        padOfs += 0x1E;
        wordOfs += 4;
    }

    padOfs = 0;
    byteOfs = 0;
    for (count = 0; count < 2; count++) {
        *(u8*)(gp + byteOfs + 0x13B8) = *(u8*)(DemoPad + padOfs + 0x2);
        *(u8*)(gp + byteOfs + 0x13BC) = *(u8*)(DemoPad + padOfs + 0x3);
        *(u8*)(gp + byteOfs + 0x13C0) = *(u8*)(DemoPad + padOfs + 0x4);
        *(u8*)(gp + byteOfs + 0x13C4) = *(u8*)(DemoPad + padOfs + 0x5);
        *(u8*)(gp + byteOfs + 0x13C8) = *(u8*)(DemoPad + padOfs + 0x6);
        *(u8*)(gp + byteOfs + 0x13CC) = *(u8*)(DemoPad + padOfs + 0x7);

        padOfs += 0x1E;

        *(u8*)(gp + byteOfs + 1 + 0x13B8) = *(u8*)(DemoPad + padOfs + 0x2);
        *(u8*)(gp + byteOfs + 1 + 0x13BC) = *(u8*)(DemoPad + padOfs + 0x3);
        *(u8*)(gp + byteOfs + 1 + 0x13C0) = *(u8*)(DemoPad + padOfs + 0x4);
        *(u8*)(gp + byteOfs + 1 + 0x13C4) = *(u8*)(DemoPad + padOfs + 0x5);
        *(u8*)(gp + byteOfs + 1 + 0x13C8) = *(u8*)(DemoPad + padOfs + 0x6);
        *(u8*)(gp + byteOfs + 1 + 0x13CC) = *(u8*)(DemoPad + padOfs + 0x7);

        padOfs += 0x1E;
        byteOfs += 2;
    }

    wordOfs = 0;
    byteOfs = 0;
    for (count = 0; count < 4; count++) {
        if ((*(u32*)(gp + wordOfs + 0x1328) & 0x1C00) == 0x1C00) {
            *(u32*)(gp + wordOfs + 0x1328) = 0;
            *(u32*)(gp + wordOfs + 0x1338) = 0;
            *(u32*)(gp + wordOfs + 0x1348) = 0;

            slow = *(s32*)(gp + 4) * 0x18;
            slow = slow / 0x3C + (slow >> 31);
            slow = slow - (slow >> 31);
            *(s32*)(gp + wordOfs + 0x1358) = slow;

            *(u32*)(gp + wordOfs + 0x1368) = 0;
            *(u32*)(gp + wordOfs + 0x1378) = 0;
            *(u32*)(gp + wordOfs + 0x1388) = 0;
            *(u32*)(gp + wordOfs + 0x1398) = 0;

            slow = *(s32*)(gp + 4) * 0x18;
            slow = slow / 0x3C + (slow >> 31);
            slow = slow - (slow >> 31);
            *(s32*)(gp + wordOfs + 0x13A8) = slow;

            *(u8*)(gp + byteOfs + 0x13B8) = 0;
            *(u8*)(gp + byteOfs + 0x13BC) = 0;
            *(u8*)(gp + byteOfs + 0x13C0) = 0;
            *(u8*)(gp + byteOfs + 0x13C4) = 0;
            *(u8*)(gp + byteOfs + 0x13C8) = 0;
            *(u8*)(gp + byteOfs + 0x13CC) = 0;
        }

        wordOfs += 4;
        byteOfs++;
    }

    if (*(s32*)(gp + 0x1294) != 0) {
        rumbleIndex = 0;
        do {
            if (*(u8*)(gp + rumbleIndex + 0x1310) != 0) {
                U_PADControlMotor(rumbleIndex, 2);
            } else {
                rumbleStatus = *(u8*)(gp + rumbleIndex + 0x13D0);

                if (*(u8*)(gp + rumbleIndex + 0x13D4) != rumbleStatus) {
                    status = rumbleStatus;

                    if (status == 1) {
                        U_PADControlMotor(rumbleIndex, 1);
                    } else if (status >= 1) {
                        if (status < 3) {
                            U_PADControlMotor(rumbleIndex, 2);
                        }
                    } else if (status >= 0) {
                        U_PADControlMotor(rumbleIndex, 0);
                    }

                    *(u8*)(gp + rumbleIndex + 0x13D4) = *(u8*)(gp + rumbleIndex + 0x13D0);
                }
            }

            rumbleIndex++;
        } while ((s32)rumbleIndex < 4);
    }

    *(s32*)(gp + 0x1324) = 1;
}
void fsort(void** base, u32 count) {
    static void** lo;
    static void** hi;
    static void** gt;
    static void** tail;
    static void* p;

    s32 cmpResult;
    s32 half;
    s32 shift;
    s32 bits;
    u32 mask;
    u32 bitCount;
    u32 stepCount;
    u32 stride;
    u32 dist;
    u32 leftSize;
    u32 rightSize;
    void** pivot;
    void** work;
    void** saveTail;

    do {
        tail = base + (count - 1);

        if (count == 2) {
            cmpResult = FSORT_COMP(*base, *tail);
            if (cmpResult <= 0) {
                return;
            }

            p = *base;
            *base = *tail;
            *tail = p;
            return;
        }

        if (count == 4) {
            gt = tail;
            hi = tail - 1;
            lo = base;
            pivot = tail;
        } else if (count < 0x28) {
            pivot = (void**)((s32)base + ((count & 0x7FFFFFFE) * 2));

            cmpResult = FSORT_COMP(*base, *tail);
if (cmpResult > 0) {
    cmpResult = FSORT_COMP(*base, *pivot);
    if (cmpResult < 0) {
        p = *tail;
        *tail = *pivot;
        *pivot = *base;
        *base = p;
    } else {
        cmpResult = FSORT_COMP(*pivot, *tail);
        if (cmpResult < 0) {
            p = *base;
            *base = *pivot;
            *pivot = *tail;
            *tail = p;
        } else {
            p = *base;
            *base = *tail;
            *tail = p;
        }
    }
} else {
    cmpResult = FSORT_COMP(*base, *pivot);
    if (cmpResult > 0) {
        p = *base;
        *base = *pivot;
        *pivot = p;
    } else {
        cmpResult = FSORT_COMP(*pivot, *tail);
        if (cmpResult > 0) {
            p = *pivot;
            *pivot = *tail;
            *tail = p;
        }
    }
}

            if (count == 3) {
                return;
            }

            gt = tail;
            hi = tail - 1;
            lo = base + 1;
        } else {
            mask = 0xFFFFFFFF;
            bitCount = 0x3F;
            shift = 0x20;

            while (mask != 0) {
                half = shift >> 1;
                bits = mask & count;

                if (half != 0) {
                    mask = mask & (mask << half);
                } else {
                    mask = mask & (mask << 1);
                }

                if (bits == 0) {
                    mask = mask >> shift;
                    shift = half;
                } else {
                    bitCount -= shift;
                    shift = half;
                }
            }

            stepCount = (0x20 - bitCount) & ~-(u32)(bitCount > 0x20);
            bitCount = (s32)stepCount >> 1;

            stride = count >> (bitCount & 0x3F);
            if ((stepCount & 1) != 0) {
                stride = 1 << (bitCount & 0x3F);
            }

            stepCount = count / stride;
            tail = base + (stride - 1) * stepCount;
            pivot = base + ((s32)stride / 2) * stepCount;

            work = base;
            do {
                dist = (s32)tail - (s32)work;

                gt = tail;
                hi = tail;
                lo = work;

                pivot = work + (stepCount *
                    ((u32)(((s32)dist >> 2) + ((s32)dist < 0 && (dist & 3) != 0)) / 2)) /
                    stepCount;

                while (lo <= hi) {
                    cmpResult = FSORT_COMP(*hi, *pivot);
                    if (cmpResult <= 0) {
                        if (cmpResult >= 0) {
                            hi -= stepCount;
                        } else {
                            while (1) {
                                if (hi <= lo) {
                                    goto large_partition_done;
                                }

                                cmpResult = FSORT_COMP(*lo, *pivot);
                                if (cmpResult >= 0) {
                                    break;
                                }

                                lo += stepCount;
                            }

                            p = *lo;
                            *lo = *hi;
                            *hi = p;

                            if (cmpResult > 0) {
                                gt = hi;
                            }

                            if ((s32)(-cmpResult & ~cmpResult) >= 0) {
                                pivot = hi;
                            }

                            lo += stepCount;
                            hi -= stepCount;
                        }
                    } else {
                        gt = hi;
                        hi -= stepCount;
                    }
                }

large_partition_done:
                if (gt < pivot) {
                    p = *gt;
                    *gt = *pivot;
                    *pivot = p;
                    gt += stepCount;
                }

                saveTail = tail;

                if (pivot < gt) {
                    gt = tail;
                    lo = base;

                    if (hi < pivot) {
                        break;
                    }

                    tail = hi;
                    gt = work;
                }

                work = gt;
                gt = saveTail;
                lo = base;
            } while (work < tail);

            pivot = saveTail;
        }

partition_again:
        if (lo <= hi) {
            cmpResult = FSORT_COMP(*hi, *pivot);
            if (cmpResult <= 0) {
                if (cmpResult >= 0) {
                    hi -= 1;
                    goto partition_again;
                }

                while (1) {
                    if (hi <= lo) {
                        goto partition_done;
                    }

                    cmpResult = FSORT_COMP(*lo, *pivot);
                    if (cmpResult >= 0) {
                        break;
                    }

                    lo += 1;
                }

                p = *lo;
                *lo = *hi;
                *hi = p;

                if (cmpResult > 0) {
                    gt = hi;
                }

                if ((s32)(-cmpResult & ~cmpResult) >= 0) {
                    pivot = hi;
                }

                hi -= 1;
                lo += 1;
            } else {
                gt = hi;
                hi -= 1;
            }

            goto partition_again;
        }

partition_done:
        if (gt < pivot) {
            p = *gt;
            *gt = *pivot;
            gt += 1;
            *pivot = p;
        }

        leftSize = (s32)hi - (s32)base;
        rightSize = (s32)tail - (s32)gt;

        if ((s32)(((s32)leftSize >> 2) + ((s32)leftSize < 0 && (leftSize & 3) != 0)) <
            (s32)(((s32)rightSize >> 2) + ((s32)rightSize < 0 && (rightSize & 3) != 0))) {
            lo = hi;
            saveTail = gt;
            hi = base;
        } else {
            lo = tail;
            tail = hi;
            hi = gt;
            saveTail = base;
        }

        dist = (s32)tail - (s32)saveTail;
        count = ((s32)dist >> 2) + ((s32)dist < 0 && (dist & 3) != 0) + 1;

        if (hi < lo) {
            dist = (s32)lo - (s32)hi;
            fsort(hi, ((s32)dist >> 2) + ((s32)dist < 0 && (dist & 3) != 0) + 1);
        }

        base = saveTail;

        if (count < 2) {
            return;
        }
    } while (1);
}
f32 distABf(f32 ax, f32 ay, f32 bx, f32 by) {
    f32 dx;
f32 dy;
f64 dvalue;
f64 inv;
u32 bits;
u32 exp;
s32 type;
f32 classifyValue;
f64 square;
dy = by - ay;
dx = bx - ax;

ax = dy * dy;
ax = dx * dx + ax;

dvalue = ax;

    if (dvalue > (f64)float_0_8041f448) {
    inv = __frsqrte(dvalue);

    square = inv * inv;
    inv = double_0p5_802bf1f8 * inv *
          (double_3_802bf200 - dvalue * square);

    square = inv * inv;
    inv = double_0p5_802bf1f8 * inv *
          (double_3_802bf200 - dvalue * square);

    square = inv * inv;
    inv = double_0p5_802bf1f8 * inv *
          (double_3_802bf200 - dvalue * square);

    return (f32)(dvalue * inv);
}

    if (dvalue < double_0_802bf208) {
        return __float_nan[0];
    }

    classifyValue = ax;
bits = *(u32*)&classifyValue;
exp = bits;
exp &= 0x7F800000;
    if (exp >= 0x7F800000) {
    if (exp == 0x7F800000) {
        if ((bits & 0x7FFFFF) != 0) {
            type = 1;
        } else {
            type = 2;
        }
    } else {
        type = 4;
    }
} else if (exp == 0) {
    if ((bits & 0x7FFFFF) != 0) {
        type = 5;
    } else {
        type = 3;
    }
} else {
    type = 4;
}

    if (type == 1) {
        return __float_nan[0];
    }

    return ax;
}
void seq_mapChangeExit(void) {
    GXResetOverflowCount();
    dbg_lotteryinfo = 0;
}