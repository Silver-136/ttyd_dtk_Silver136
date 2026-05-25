#include "mario/mario_sbr.h"

void* marioGetPtr(void);
void* memset(void* dst, int value, u32 size);
s32 hitGetAttr(void);
f64 cos(f64 value);
extern f32 float_0_80420a60;
extern f32 float_360_80420a68;
extern f32 float_180_80420a70;
extern f32 float_1_80420a74;
extern f32 float_90_80420a98;
extern f32 float_3p1416_80420a9c;
extern f32 float_0p19_80420aa0;
extern u8 rubwork[0x68];

f32 revise360(f32 value) {
    while (value < float_0_80420a60) {
        value += float_360_80420a68;
    }
    while (value >= float_360_80420a68) {
        value -= float_360_80420a68;
    }
    return value;
}

void marioGetRubInit(void) {
    memset(rubwork, 0, 0x68);
    *(s32*)(rubwork + 0x20) = -1;
}

s32 N_marioGetMovementSpeed(void) {
    void* mario = marioGetPtr();
    s32 x = *(s8*)((s32)mario + 0x252);
    s32 y = *(s8*)((s32)mario + 0x253);
    s32 value;
    if (x == 0 && y == 0) {
        return 0;
    }
    value = x * x + y * y;
    return value > 0xBD1 ? 2 : 1;
}

f32 marioGetDispDir(void) {
    void* mario = marioGetPtr();
    return revise360(*(f32*)((s32)mario + 0x1AC) - float_90_80420a98 + *(f32*)((s32)mario + 0x19C));
}

void* marioGetTouchNpcPtr(void) {
    return *(void**)((s32)marioGetPtr() + 0x1D8);
}

void marioSetPeakYpos(void) {
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x12C) = *(f32*)((s32)mario + 0x90);
}

void marioResetGravity(void) {
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x228) = float_1_80420a74;
}

s32 marioChkPlanePlace(void) {
    s32 value = hitGetAttr() & 0x38;
    return ((u32)(-value) | (u32)value) >> 31;
}

void N_marioReloadMapOnBottomlessOff(void) {
    *(s32*)((s32)marioGetPtr() + 0x2E0) = 0;
}

void N_marioReloadMapOnBottomlessOn(void) {
    *(s32*)((s32)marioGetPtr() + 0x2E0) = 1;
}

f32 marioGetMoveRate(f32 angle) {
    void* mario = marioGetPtr();
    f32 value = (angle - *(f32*)((s32)mario + 0x19C)) * float_3p1416_80420a9c / float_180_80420a70;
    return (f32)cos(value) * float_0p19_80420aa0 + float_1_80420a74;
}
