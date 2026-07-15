#include "battle/battle_sub.h"
extern void* _battleWorkPointer;
extern double cos(double);
extern double sin(double);

extern f32 float_6p2832_80422690;
extern f32 float_360_80422694;

extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BattleGetMarioPtr(void* battleWork);
extern void* BattleGetPartyPtr(void* battleWork);
extern void* BattleGetSystemPtr(void* battleWork);
extern double atan2(double y, double x);

f32 cosfd(f32 degree) {
    return cos((float_6p2832_80422690 * degree) / float_360_80422694);
}

f32 sinfd(f32 degree) {
    return sin((float_6p2832_80422690 * degree) / float_360_80422694);
}

void btlsubResetMoveColorLvAll(void* battleWork) {
    s32 i;
    s32 color;
    void* unit;

    color = 0xFF;

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            *(u8*)((s32)unit + 0x311) = color;
        }
    }
}


double intpl_sub(double outStart, double outEnd, u32 intplType, u32 inStart, u32 inEnd) {
    extern double cos(double);
    extern double sin(double);
    extern f32 float_4_80422674;
    extern f32 float_3p1416_80422678;
    extern f32 float_15_8042267c;
    extern f32 float_40_80422670;
    extern f32 float_0_80422680;
    extern f32 float_1_80422684;
    extern f32 float_0p5_80422688;
    extern f32 float_1p5708_8042268c;
    f32 start = (f32)outStart;
    f32 delta = (f32)(outEnd - outStart);
    f32 t = (f32)inStart;
    f32 end = (f32)inEnd;
    f32 remain;
    f32 value;
    f32 wave;

    switch (intplType) {
        case 0:
            value = start + t * delta / end;
            break;
        case 1:
            value = start + delta * (t * t) / (end * end);
            break;
        case 2:
            value = start + delta * (t * t * t) / (end * end * end);
            break;
        case 3:
            value = start + delta * (t * t * t * t) / (end * end * end * end);
            break;
        case 4:
            remain = (f32)(inEnd - inStart);
            value = start + delta - delta * (remain * remain) / (end * end);
            break;
        case 5:
            remain = (f32)(inEnd - inStart);
            value = start + delta - delta * (remain * remain * remain) / (end * end * end);
            break;
        case 6:
            remain = (f32)(inEnd - inStart);
            value = start + delta - delta * (remain * remain * remain * remain) /
                    (end * end * end * end);
            break;
        case 7:
            remain = (f32)(inEnd - inStart);
            wave = (f32)cos((double)(float_4_80422674 * float_3p1416_80422678 * (t / end)));
            value = (f32)outEnd - remain * remain * delta * wave / (end * end);
            break;
        case 8:
            remain = (f32)(inEnd - inStart);
            wave = (f32)cos((double)((float_4_80422674 * float_3p1416_80422678 *
                                     ((t * t) / end)) / float_15_8042267c));
            value = (f32)outEnd - remain * remain * delta * wave / (end * end);
            break;
        case 10:
            remain = (f32)(inEnd - inStart);
            wave = (f32)cos((double)((float_4_80422674 * float_3p1416_80422678 *
                                     ((t * t) / end)) / float_40_80422670));
            value = remain * remain * wave / (end * end);
            if (value < float_0_80422680) {
                value = -value;
            }
            value = -(value * delta - (f32)outEnd);
            break;
        case 11:
            wave = (f32)cos((double)(float_3p1416_80422678 * t / end));
            value = delta * (float_1_80422684 - wave) * float_0p5_80422688 + start;
            break;
        case 12:
            wave = (f32)sin((double)(float_1p5708_8042268c * t / end));
            value = delta * wave + start;
            break;
        case 13:
            wave = (f32)cos((double)(float_1p5708_8042268c * t / end));
            value = delta * (float_1_80422684 - wave) + start;
            break;
        default:
            value = float_0_80422680;
            break;
    }
    return (double)value;
}

s32 BattleTransID(EventEntry* event, s32 type) {
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;
    if (type < 0) {
        switch (type) {
            case -2:
                return *(s32*)((s32)event + 0x160);
            case -3:
                return *(s32*)BattleGetMarioPtr(battleWork);
            case -4:
                unit = BattleGetPartyPtr(battleWork);
                if (unit != 0) {
                    return *(s32*)unit;
                }
                return -1;
            case -1:
                return -1;
            case -5:
                return *(s32*)BattleGetSystemPtr(battleWork);
            case -6:
                return *(s32*)((s32)battleWork + 0x424);
            default:
                break;
        }
    }

    return type;
}


void btlMovePos(f32 distance, f32 angle, f32* moveXPos, f32* moveZPos) {
    f32 s;
    f32 radians;
    f32 c;
    f32 delta;

    radians = (float_6p2832_80422690 * angle) / float_360_80422694;
    s = sin(radians);
    c = cos(radians);
    delta = distance * s;
    *moveXPos = *moveXPos + delta;
    delta = -distance * c;
    *moveZPos = *moveZPos + delta;
}

f32 atan2f_safety(f32 y, f32 x) {
    f32 abs;

    if (x >= 0.0f) {
        abs = x;
    } else {
        abs = -x;
    }
    if (abs < 0.0001) {
        x = 0.0001f;
    }

    if (y >= 0.0f) {
        abs = y;
    } else {
        abs = -y;
    }
    if (abs < 0.0001) {
        y = 0.0001f;
    }

    return atan2(y, x);
}


s32 BtlCompForwardLv(f32 position, s32 direction) {
    s32 level;
    s32 result;

    level = 0;
    if (position > -800.0f) {
        level = (s32)((position + 800.0f) / 40.0f);
        if (level > 0x30) {
            level = 0x30;
        }
    }

    result = 0x30 - level;
    if (direction >= 0) {
        result = level;
    }
    return result;
}

