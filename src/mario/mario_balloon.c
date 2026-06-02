#include "mario/mario_balloon.h"


u8 marioBalloonMain(void) {
    return 0;
}


s32 chkBalloon(u32 param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioBalloonOn(s32 type) {
    extern void* marioGetPtr(void);
    extern s32 effMarioBalloonEntry(s32 type, s32 arg1);
    void* mario;
    u32 flags;

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x02000000) == 0) {
        *(u32*)((s32)mario + 4) = flags | 0x02000000;
        *(s32*)((s32)mario + 0x264) = effMarioBalloonEntry(type, 0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void marioBalloonOff(void) {
    extern void* marioGetPtr(void);
    extern void effDelete(s32 effect);
    void* mario;
    u32 flags;

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x02000000) != 0) {
        *(u32*)((s32)mario + 4) = flags & ~0x02000000;
        effDelete(*(s32*)((s32)mario + 0x264));
        *(s32*)((s32)mario + 0x264) = 0;
        *(s32*)((s32)mario + 0x268) = 0;
    }
}

void marioBaloonInit(void) {
    extern void* marioGetPtr(void);

    *(s32*)((s32)marioGetPtr() + 0x264) = 0;
}
