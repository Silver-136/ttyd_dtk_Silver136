#include "motion/mot_plane.h"

s32 marioPlaneGetFlyingDir(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    if (*(u32*)*(s32*)((s32)mario + 0x294) & 1) {
        return 1;
    }
    return -1;
}

s32 marioGetPlaneStatus(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    s32 status;

    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    status = *(s32*)((s32)mario + 0x44);
    if (status >= 0xC && status < 0x14) {
        return 2;
    }
    return 1;
}


u8 mot_plane(void) {
    return 0;
}


u8 flyMain(void) {
    return 0;
}


u8 chkCollision(void) {
    return 0;
}


void mot_plane_post(void) {
    extern void* marioGetPtr(void);
    extern void camFollowYOn(void);
    extern void psndSFXOff(s32 id);
    extern void marioPaperOff(void);
    extern void marioChgPaper(s32 mode);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void allPartyRideOff2(void);
    extern u8 str_SFX_MARIO_AIRPLANE1_802c40d8[];
    extern f32 float_20_80420d34;
    extern f32 float_37_80420d38;
    extern f32 float_0_80420d1c;
    typedef struct Vec { u32 x, y, z; } Vec;
    u8* data = str_SFX_MARIO_AIRPLANE1_802c40d8;
    void* mario = marioGetPtr();
    void* mario2;
    void* plane;

    camFollowYOn();
    mario2 = marioGetPtr();
    plane = *(void**)((s32)mario2 + 0x294);
    if (plane != 0) {
        s32 sound = *(s32*)((s32)plane + 0x28);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)*(void**)((s32)mario2 + 0x294) + 0x28) = -1;
        }
    }
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x1B8) = float_20_80420d34;
    *(f32*)((s32)mario2 + 0x1BC) = float_37_80420d38;
    *(Vec*)((s32)mario2 + 0xBC) = *(Vec*)(data + 0x50);
    *(Vec*)((s32)mario2 + 0xB0) = *(Vec*)(data + 0x5C);
    *(Vec*)((s32)mario2 + 0x98) = *(Vec*)(data + 0x68);
    marioPaperOff();
    marioChgPaper(0);
    *(u32*)((s32)mario2 + 0x4) &= ~0x8;
    *(u32*)((s32)mario2 + 0x0) |= 0x80;
    marioAdjustMoveDir();
    *(u32*)((s32)mario2 + 0x4) &= ~0x100;
    *(f32*)((s32)mario + 0x138) = float_0_80420d1c;
    *(f32*)((s32)mario + 0x140) = float_0_80420d1c;
    plane = *(void**)((s32)mario + 0x294);
    if (plane != 0) {
        __memFree(0, plane);
        *(void**)((s32)mario + 0x294) = 0;
    }
    allPartyRideOff2();
}

u8 marioForcePlaneAnime(void) {
    return 0;
}


s32 mario_plane_cancel(void) {
    return 0;
}
