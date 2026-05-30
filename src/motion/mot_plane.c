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


u8 mot_plane_post(void) {
    return 0;
}


u8 marioForcePlaneAnime(void) {
    return 0;
}


s32 mario_plane_cancel(void) {
    return 0;
}
