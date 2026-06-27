#include "motion/mot_hammer.h"

s32 marioGetHammerLevel(void) {
    extern void* pouchGetPtr(void);

    return *(s8*)((s32)pouchGetPtr() + 0x99);
}

f32 marioGetAngleKaitenHammer(void) {
    extern void* marioGetPtr(void);
    extern f32 revise360(f32 angle);
    extern f32 float_neg1_80420b04;
    extern f32 float_neg2_80420b08;
    extern f32 float_270_80420ac4;

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x13) {
        return float_neg1_80420b04;
    }
    if (*(s32*)((s32)mario + 0x44) != 0x14) {
        return float_neg2_80420b08;
    }
    return revise360(float_270_80420ac4 - *(f32*)((s32)mario + 0x1AC));
}


u8 mot_hammer2(void) {
    return 0;
}


u8 mot_hammer(void) {
    return 0;
}


u8 hitEstimate(void) {
    return 0;
}


s32 marioChkHammer2(void) {
    extern void* marioGetPtr(void);
    extern void* pouchGetPtr(void);
    extern s32 strcmp(char*, char*);
    extern void marioChgPose(char*);
    extern void marioVoiceGlareOff(void);
    extern void marioVoiceGlareOn(void);
    extern s32 marioGetRub(s32, void*, void*, void*);
    extern f32 float_0_80420ab8;
    extern char str_M_I_Y_80420b74;
    extern char str_M_S_1_80420b6c;

    void* mario = marioGetPtr();
    void* pouch = pouchGetPtr();
    s32 count;
    s32 dir;
    s32 stack8;

    if (*(s8*)((s32)pouch + 0x99) < 2) {
        return 0;
    }
    if ((*(u16*)((s32)mario + 0x24A) & 0x200) == 0) {
        if ((*(u16*)((s32)mario + 0x24A) & 0x100) == 0) {
            if (strcmp(*(char**)((s32)mario + 0x18), &str_M_I_Y_80420b74) == 0 &&
                *(u16*)((s32)mario + 0x2E) == 0) {
                marioChgPose(&str_M_S_1_80420b6c);
                marioVoiceGlareOff();
            }
            *(u8*)((s32)mario + 0x40) = 0;
        }
        return 0;
    }
    if ((*(u32*)((s32)mario + 0x0) & 0x02000000) != 0) {
        *(u8*)((s32)mario + 0x40) = 0;
    } else {
        *(u8*)((s32)mario + 0x40) = *(u8*)((s32)mario + 0x40) + 1;
    }
    if (*(f32*)((s32)mario + 0x194) != float_0_80420ab8 &&
        *(s8*)((s32)mario + 0x40) > 10) {
        if (*(u16*)((s32)mario + 0x2E) == 0 && (*(u32*)((s32)mario + 0x4) & 0x40000000) == 0) {
            marioChgPose(&str_M_I_Y_80420b74);
            marioVoiceGlareOn();
        }
    }
    if (*(s8*)((s32)mario + 0x40) >= 0x32) {
        *(u8*)((s32)mario + 0x40) = 0x32;
        if (*(u16*)((s32)mario + 0x2E) == 0 && (*(u32*)((s32)mario + 0x4) & 0x40000000) == 0) {
            marioChgPose(&str_M_I_Y_80420b74);
            marioVoiceGlareOn();
        }
    }
    if (marioGetRub(0x200, &dir, &count, &stack8) != 1) {
        return 0;
    }
    *(f32*)((s32)mario + 0x180) = float_0_80420ab8;
    if (count < 1) {
        return 0;
    }
    *(u8*)((s32)mario + 0x43) = dir;
    return 1;
}

void mot_ham2_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioAdjustMoveDir(void);
    extern f32 float_0_80420ab8;
    extern f32 float_0p01_80420b00;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0x4);
    if (flags & 0x40000000) {
        marioPaperOff();
    }
    {
        void* mario2 = marioGetPtr();
        f32 small = float_0p01_80420b00;
        f32 zero = float_0_80420ab8;
        *(f32*)((s32)mario2 + 0x148) = zero;
        *(f32*)((s32)mario2 + 0x158) = small;
    }
    marioPaperOff();
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    *(u32*)((s32)mario + 0x4) &= ~0x10;
    *(u32*)((s32)mario + 0x4) &= ~0x4;
    *(u32*)((s32)mario + 0x0) &= ~0x80;
    marioAdjustMoveDir();
}

