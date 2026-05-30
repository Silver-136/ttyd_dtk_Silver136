#include "peach.h"

char str_P_S_1_804265cc[] = "P_S_1";

void* marioGetPtr(void);
void peachChgPose(void* pose);

void peachPreInit(void) {
}

void peachSetStsAftershower(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x10;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsAngry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x100;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsCarry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x400;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsHappy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x40;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsInvisible(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x20;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsOrdinary(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x200;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsUneasy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x80;
    peachChgPose(str_P_S_1_804265cc);
}


void peachChgPose(void* pose) {
}


u8 peach_dash(void) {
    return 0;
}


u8 peach_stay(void) {
    return 0;
}


s32 peachMain(void) {
    return 0;
}


u8 peach_walk(void) {
    return 0;
}


u8 peachSetStsNormal(void) {
    return 0;
}


u8 peachTransformOff(void) {
    return 0;
}


u8 peachTransformOn(void) {
    return 0;
}


u8 peach_talk(void) {
    return 0;
}
