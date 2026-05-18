#include "system.h"
extern s32 rand(void);
extern f64 sin(f64 x);
extern f64 cos(f64 x);
extern s32 gp;
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