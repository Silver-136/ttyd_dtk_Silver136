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


void mot_upstairs(void) {
    extern void* marioGetPtr(void);
    extern void marioClearJumpPara(void);
    extern void motSlitContinue(void);
    extern void marioChgMot2(s32 motion);
    extern void marioChgMotAuto(void);
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)((s32)mario + 0x0) |= 0x40000;
        *(u32*)((s32)mario + 0x0) |= 0x80;
        marioClearJumpPara();
        *(s32*)((s32)mario + 0x48) = 3;
        *(f32*)((s32)mario + 0x7C) = (*(f32*)((s32)mario + 0x1C8) - *(f32*)((s32)mario + 0x90)) / (f32)*(s32*)((s32)mario + 0x48);
        *(s32*)((s32)mario + 0x44) = 0;
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            break;
        default:
            return;
    }
    *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    if (--*(s32*)((s32)mario + 0x48) <= 0) {
            *(u32*)((s32)mario + 0x0) &= ~0x80;
            *(u32*)((s32)mario + 0x0) &= ~0x40;
            *(u32*)((s32)mario + 0x0) &= ~0x40000;
            if (*(u32*)((s32)mario + 0x0) & 0x100000) {
                motSlitContinue();
            } else if (*(u32*)((s32)mario + 0x4) & 0x1000000) {
                marioChgMot2(0x15);
            } else {
                marioChgMotAuto();
            }
    }
}

void mot_land(void) {
    extern void* marioGetPtr(void);
    extern void marioResetCamFollowRate(void);
    extern void psndSFXOff(s32 id);
    extern void marioChgMot(s32 motion);
    extern f32 float_0_80420cc8;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        s32 sound;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        {
            f32 zero = float_0_80420cc8;
        *(u32*)((s32)mario + 0x0) &= ~0xF0000;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s16*)((s32)mario + 0x52) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        }
        marioResetCamFollowRate();
        sound = *(s32*)((s32)mario + 0x284);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)mario + 0x284) = -1;
        }
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            marioChgMot(0);
            break;
        case 1:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                marioChgMot(0);
            }
            break;
        case 10:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                marioChgMot(0);
            }
            break;
    }
}

u8 marioPreJump(void) {
    extern void clear_hitobj_ride(void);
    extern u8 marioSetJumpPara(void);
    void* mario = marioGetPtr();
    s32 x;
    s32 y;

    *(u32*)mario &= ~0x20000;
    *(u32*)mario |= 0x10000;
    *(s16*)((s32)mario + 0x50) = 0;
    *(s16*)((s32)mario + 0x52) = 0;
    *(s32*)((s32)mario + 0x1EC) = *(s32*)((s32)mario + 0x1E8);
    clear_hitobj_ride();
    marioSetJumpPara();
    x = *(s32*)((s32)mario + 0x8C);
    y = *(s32*)((s32)mario + 0x90);
    *(s32*)((s32)mario + 0x11C) = x;
    *(s32*)((s32)mario + 0x120) = y;
    *(s32*)((s32)mario + 0x124) = *(s32*)((s32)mario + 0x94);
    *(f32*)((s32)mario + 0x190) = *(f32*)((s32)mario + 0x180);
}
