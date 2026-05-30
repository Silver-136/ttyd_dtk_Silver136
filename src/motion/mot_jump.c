#include "motion/mot_jump.h"

extern void* gp;
extern char str_gon_10_802c40d0[];

void* marioGetPtr(void);
void psndSFXOff(s32 id);
s32 strcmp(const char* a, const char* b);
void camFollowYOn(void);

f32 marioGetJumpHeight(void) {
    void* mario = marioGetPtr();
    return *(f32*)((s32)mario + 0x2D0);
}

void mot_fall_post(void) {
    void* mario = marioGetPtr();
    s32 sound = *(s32*)((s32)mario + 0x284);
    if ((u32)(sound + 0x10000) != 0xFFFF) {
        psndSFXOff(sound);
        *(s32*)((s32)mario + 0x284) = -1;
    }
}

void N_mot_jumpNpc_post(void) {
    void* mario = marioGetPtr();
    if (strcmp((char*)((s32)gp + 0x12C), str_gon_10_802c40d0) == 0) {
        if (*(u32*)((s32)mario + 4) & 0x20000) {
            camFollowYOn();
        }
    }
}


u8 mot_jumpNpc(void) {
    return 0;
}


u8 mot_jumpSw(void) {
    return 0;
}


u8 mot_jumpStand(void) {
    return 0;
}


u8 mot_fall(void) {
    return 0;
}


u8 mot_jump(void) {
    return 0;
}


u8 mot_jumpSmall(void) {
    return 0;
}


u8 mot_upstairs(void) {
    return 0;
}


u8 mot_land(void) {
    return 0;
}


u8 marioPreJump(void) {
    return 0;
}
