#include "bowser/koopa_motion.h"

void quake_kpaHipAttack(void) {
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaHipAttack_evt[8];
    evtEntry(quake_kpaHipAttack_evt, 0, 0);
}

void quake_kpaLandOn(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetStageType(void);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern s32 kpaGetLevel(void);
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaLandOn_evt[8];

    void* mario = marioGetPtr();
    if (kpaGetStageType() != 1) {
        psndSFXOn_3D(0x81D, (void*)((u8*)mario + 0x8C));
        if (kpaGetLevel() >= 2) {
            evtEntry(quake_kpaLandOn_evt, 0, 0);
        }
    }
}

s32 kpaPowDown(void) {
    extern void* marioGetPtr(void);
    extern void kpaCancelAttackMode(void);
    extern s32 kpaGetLevel(void);
    extern s32 sysMsec2Frame(s32 msec);
    extern void marioChgMot(s32 mot);

    void* mario = marioGetPtr();
    void* work;

    kpaCancelAttackMode();
    if (kpaGetLevel() > 0) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) - 1;
        *(s16*)((s32)mario + 0x78) = sysMsec2Frame(3000);
        marioChgMot(0x27);
        return 1;
    }
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work |= 0x10000000;
    return 0;
}

s32 kpaPowUp(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetLevel(void);
    extern void kpaCancelAttackMode(void);
    extern void marioChgMot(s32 mot);
    extern void kpaMSetMutekiTimer(void);
    extern void kpaMutekiOn(void);

    void* mario = marioGetPtr();
    void* work;

    if (kpaGetLevel() < 3) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) + 1;
        kpaCancelAttackMode();
        marioChgMot(0x26);
        return 1;
    }
    kpaMSetMutekiTimer();
    kpaMutekiOn();
    return 0;
}

s32 kpaHipAttackOk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0x10) {
        return 0;
    }
    return ((*(u32*)*(void**)((s32)mario + 0x298) >> 4) & 1) ^ 1;
}

void kpaSwimMove(void) {
    extern void* marioGetPtr(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 float_0_804272e0;
    extern f32 float_0p01_80427354;
    extern f32 float_0p06_80427328;
    extern f32 float_2_80427320;
    extern f32 float_40_8042735c;
    extern f32 float_1225_80427358;

    void* mario = marioGetPtr();
    f32 dir = *(s8*)((s32)mario + 0x252);
    f32 speed = *(f32*)((s32)mario + 0x2DC);
    f32 move;
    f32 angle;

    if (dir != float_0_804272e0) {
        if (dir * speed < float_0_804272e0) {
            speed = float_0_804272e0;
        }
        if (speed * speed <= float_1225_80427358) {
            speed += dir * float_0p01_80427354;
        } else {
            speed += dir * float_0p01_80427354;
            if (speed < -float_40_8042735c) {
                speed = -float_40_8042735c;
            }
            if (speed > float_40_8042735c) {
                speed = float_40_8042735c;
            }
        }
    }

    *(f32*)((s32)mario + 0x2DC) = speed;
    move = float_0p06_80427328 * __fabs(speed);
    if (move >= float_2_80427320) {
        move = float_2_80427320;
    }
    angle = angleABf(float_0_804272e0, float_0_804272e0, speed, float_0_804272e0);
    if (move < float_0p01_80427354) {
        move = float_0_804272e0;
    }
    *(f32*)((s32)mario + 0x180) = move;
    if (move != float_0_804272e0) {
        *(f32*)((s32)mario + 0x1A4) = angle;
    }
    *(f32*)((s32)mario + 0x18C) = move;
}

void kpaSetSpec(void) {
    extern void* marioGetPtr(void);
    extern f32 kpaGetWidth(s32 level);
    extern f32 float_24_80427360;
    extern f32 float_74_80427364;
    extern f32 float_103p6_80427368;
    extern f32 float_222_8042736c;

    void* mario = marioGetPtr();
    void* work = *(void**)((s32)mario + 0x298);

    *(f32*)((s32)mario + 0x1B8) = kpaGetWidth(*(s32*)((s32)work + 4));
    work = *(void**)((s32)mario + 0x298);
    if (*(s32*)((s32)work + 4) == 0) {
        *(f32*)((s32)mario + 0x1BC) = float_24_80427360;
    } else if (*(s32*)((s32)work + 4) == 1) {
        *(f32*)((s32)mario + 0x1BC) = float_74_80427364;
    } else if (*(s32*)((s32)work + 4) == 2) {
        *(f32*)((s32)mario + 0x1BC) = float_103p6_80427368;
    } else if (*(s32*)((s32)work + 4) == 3) {
        *(f32*)((s32)mario + 0x1BC) = float_222_8042736c;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpa_jump(void) {
    extern void* marioGetPtr(void);
    extern void marioPreJump(void);
    extern void camFollowYOff(void);
    extern void kpaChgPose(const char* pose, const char* pose2);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void kpaSetFallPara(void);
    extern void marioChgMot(s32 mot);
    extern f32 float_0_804272e0;
    extern f32 float_neg0p526_80427388;
    extern char str_KPA_J_1A_802feb94[];
    extern char str_KPA2_J_1A_802feba0[];
    typedef struct {
        s32 x;
        s32 y;
        s32 z;
    } VecCopy;

    void* mario = marioGetPtr();
    f32 oldY;
    f32 y;

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)mario |= 0x80000;
        *(s32*)((s32)mario + 0x48) = 2;
        *(s32*)((s32)mario + 0x44) = 0;
        marioPreJump();
        if (*(u16*)((s32)mario + 0x2E) != 8) {
            camFollowYOff();
        }
        kpaChgPose(str_KPA_J_1A_802feb94, str_KPA2_J_1A_802feba0);
        psndSFXOn_3D(0x81E, (void*)((u8*)mario + 0x8C));
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(VecCopy*)((s32)mario + 0xD4) = *(VecCopy*)((s32)mario + 0x8C);
        }
        *(s16*)((s32)mario + 0x2B8) = 0;
    }

    *(s16*)((s32)mario + 0x50) = *(s16*)((s32)mario + 0x50) + 1;
    if (*(u16*)((s32)mario + 0x2E) != 8) {
        oldY = *(f32*)((s32)mario + 0x7C);
        marioMakeJumpPara();
        if (*(f32*)((s32)mario + 0x7C) > float_0_804272e0 &&
            *(s16*)((s32)mario + 0x50) >= 4 &&
            !(*(u16*)((s32)mario + 0x24A) & 0x100)) {
            *(f32*)((s32)mario + 0x7C) += float_neg0p526_80427388;
        }
        y = *(f32*)((s32)mario + 0x7C);
        if (y <= float_0_804272e0) {
            if (oldY > float_0_804272e0) {
                *(s16*)((s32)mario + 0x52) = *(s16*)((s32)mario + 0x50);
            }
            marioSetPeakYpos();
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(0xA);
        }
        *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
