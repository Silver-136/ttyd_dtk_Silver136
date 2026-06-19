#include "motion/mot_walk.h"


u8 mot_dash(void) {
    return 0;
}


u8 mot_walk(void) {
    return 0;
}


u8 marioWalkDashSe(void* pHit, u32 param_2) {
    return 0;
}


f32 marioGetDashSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x188);

    if (*(u32*)mario & 0x100000) {
        s32 x = *(s8*)((s32)mario + 0x252);
        s32 y = *(s8*)((s32)mario + 0x253);
        speed = *(f32*)((s32)mario + 0x184);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

float marioGetWalkSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x184);

    if (*(u32*)mario & 0x100000) {
        register s32 x = *(s8*)((s32)mario + 0x252);
        register s32 y = *(s8*)((s32)mario + 0x253);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

