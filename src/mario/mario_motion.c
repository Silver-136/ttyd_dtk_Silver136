#include "mario/mario_motion.h"

extern f32 float_0_804209a0;
char str_M_I_U_80420a14[] = "M_I_U";
extern void (*marioMotTbl[][3])(void);

void* marioGetPtr(void);
void marioReInit_ship(void);
void marioAdjustMoveDir(void);
void marioChgPose(void* pose);

s32 L_marioChkRub(void) {
    return 0;
}

void marioChgShipMotion(void) {
    marioReInit_ship();
}

void mot_dummy(void) {
    marioGetPtr();
}

void mot_partyUse(void) {
    marioGetPtr();
}

void marioChgRollMotion(void) {
    void* mario = marioGetPtr();
    s32 mode = 0x32;
    s32 pose = 0x16;
    *(s32*)((s32)mario + 0x44) = mode;
    *(s16*)((s32)mario + 0x2E) = pose;
}

void marioClearJumpPara(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_804209a0;
    *(f32*)((s32)mario + 0x7C) = zero;
    *(f32*)((s32)mario + 0x80) = zero;
    *(f32*)((s32)mario + 0x84) = zero;
    *(f32*)((s32)mario + 0x88) = zero;
}

void mot_getItem(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        zero = float_0_804209a0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        marioAdjustMoveDir();
        marioChgPose(str_M_I_U_80420a14);
    }
}

void marioMotion(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x200) = 0;
    do {
        marioMotTbl[*(u16*)((s32)mario + 0x2E)][0]();
    } while (*(u32*)((s32)mario + 0xC) & 1);
}
