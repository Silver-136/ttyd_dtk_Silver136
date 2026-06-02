#include "mario/mario_motion.h"

extern f32 float_0_804209a0;
const char str_M_I_U_80420a14[] = "M_I_U";
extern void (*marioMotTbl[][3])(void);

void* marioGetPtr(void);
void marioReInit_ship(void);
void marioAdjustMoveDir(void);
void marioChgPose(const void* pose);

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioMotion(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x200) = 0;
    do {
        marioMotTbl[*(u16*)((s32)mario + 0x2E)][0]();
    } while (*(u32*)((s32)mario + 0xC) & 1);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 marioLandOn(void) {
    return 0;
}


u8 mot_bottomless(void) {
    return 0;
}


u8 marioFall(void) {
    return 0;
}


u8 marioJump(void) {
    return 0;
}


u8 marioChgMoveMotion(void) {
    return 0;
}


u8 mot_kaze(void) {
    return 0;
}


s32 marioChkJump(void) {
    return 0;
}


u8 mot_talk(void) {
    return 0;
}


s32 marioChkTransform(void) {
    return 0;
}


u8 marioSetJumpPara(void) {
    return 0;
}


s32 marioChkItemMotion(void) {
    return 0;
}


u8 marioChgJumpStandMotion(void) {
    return 0;
}


u8 marioBoots(void) {
    return 0;
}


s32 marioRollChgChk(void) {
    return 0;
}


u8 mot_forceReset(void) {
    return 0;
}


u8 mot_shadow(void) {
    return 0;
}


u8 marioChgSmallJumpMotion(void) {
    return 0;
}


u8 marioChgMotSub(s16 newMotionId, int param_2) {
    return 0;
}


u8 marioChgGetItemMotion(void) {
    return 0;
}


u8 marioChgStayMotion(void) {
    return 0;
}


u8 marioChgMotJump2(void) {
    return 0;
}


void marioChgMot(s32 motion) {
}


s32 marioChkTalkable(void) {
    return 0;
}


void marioChgTalkMotion(void) {
}


double marioGetFallSpd(void) {
    return 0.0;
}


u8 marioSetFallPara(void) {
    return 0;
}


u32 marioChkItemGetMotion(void) {
    return 0;
}


double marioMakeJumpPara(void) {
    return 0.0;
}


void marioChgMot2(s32 motionId) {
}
