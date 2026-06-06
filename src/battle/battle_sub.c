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
    return 0.0;
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


void btlMovePos(double distance, double angle, float* moveXPos, float* moveZPos) {
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


int BtlCompForwardLv(double position, int direction) {
    return 0;
}
