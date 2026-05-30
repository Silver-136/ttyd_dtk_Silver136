#include "battle/battle_sub.h"
extern double cos(double);
extern double sin(double);

extern f32 float_6p2832_80422690;
extern f32 float_360_80422694;

extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);

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
    return 0;
}


void btlMovePos(double distance, double angle, float* moveXPos, float* moveZPos) {
}


double atan2f_safety(double y, double x) {
    return 0.0;
}


int BtlCompForwardLv(double position, int direction) {
    return 0;
}
