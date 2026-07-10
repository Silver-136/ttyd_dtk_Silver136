#include "motion/mot_jabara.h"

void mot_grasp(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)mario &= ~0xF0000;
    }
}

s32 marioPriCheckJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    s32 item;

    if ((flags & 0x80000000) || (flags & 0x02100000)) {
        *(u8*)((s32)mario + 0x40) = 0;
        return 0;
    }
    item = pouchCheckItem(8);
    return ((u32)(-item) | (u32)item) >> 31;
}


u8 mot_jabara(void) {
    return 0;
}


u8 swingMain(void) {
    return 0;
}


void mot_jabara_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void allPartyRideOff(void);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void marioClearJumpPara(void);
    extern f32 vec3_802c3fd0[];
    extern f32 float_0_80420bd4;
    extern f32 float_0p01_80420c00;
    extern f32 float_20_80420c04;
    extern f32 float_37_80420c08;
    typedef struct Vec { f32 x, y, z; } Vec;
    f32* data = vec3_802c3fd0;
    void* mario = marioGetPtr();
    void* mario2;
    void* mario3;

    *(u32*)((s32)mario + 0x0) &= ~0x800;
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x148) = float_0_80420bd4;
    *(f32*)((s32)mario2 + 0x158) = float_0p01_80420c00;
    if (*(u32*)((s32)mario + 0x4) & 0x40000000) {
        marioPaperOff();
    }
    allPartyRideOff();
    mario3 = marioGetPtr();
    *(f32*)((s32)mario3 + 0x1AC) = *(f32*)((s32)mario3 + 0x2BC);
    *(f32*)((s32)mario3 + 0x1B0) = *(f32*)((s32)mario3 + 0x2BC);
    *(Vec*)((s32)mario3 + 0xB0) = *(Vec*)(data + 3);
    *(Vec*)((s32)mario3 + 0x98) = *(Vec*)(data + 6);
    *(Vec*)((s32)mario3 + 0xBC) = *(Vec*)(data + 9);
    *(f32*)((s32)mario3 + 0x1B8) = float_20_80420c04;
    *(f32*)((s32)mario3 + 0x1BC) = float_37_80420c08;
    marioAdjustMoveDir();
    *(u32*)((s32)mario3 + 0x4) &= ~0x4;
    *(u32*)((s32)mario3 + 0x4) &= ~0x100;
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        void* ptr = *(void**)((s32)marioGetPtr() + 0x294);
        __memFree(0, ptr);
        *(void**)((s32)marioGetPtr() + 0x294) = 0;
    }
    if (*(u16*)((s32)mario + 0x2E) < 3 || (*(u16*)((s32)mario + 0x2E) >= 9 && *(u16*)((s32)mario + 0x2E) < 0xC)) {
        marioClearJumpPara();
    }
}

int marioChkJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);
    extern s32 strcmp(const char* a, const char* b);
    extern void marioChgPose(const char* pose);
    extern void marioVoiceGlareOff(void);
    extern void marioVoiceGlareOn(void);
    extern s32 marioGetRub(s32 buttonFlags, s32* out1, s32* out2, s32* out3);
    extern char str_M_I_Y_80420cbc[];
    extern char str_M_S_1_80420c80[];
    extern f32 float_0_80420bd4;
    void* mario = marioGetPtr();
    s32 v1;
    s32 v2;
    s32 v3;
    s32 ok;

    ok = marioPriCheckJabara();
    if (ok == 0) {
        return 0;
    }
    if (!(*(u16*)((s32)mario + 0x24A) & 0x100)) {
        if (!(*(u16*)((s32)mario + 0x24A) & 0x200)) {
            if (strcmp(*(const char**)((s32)mario + 0x18), str_M_I_Y_80420cbc) == 0 && *(u16*)((s32)mario + 0x2E) == 0) {
                marioChgPose(str_M_S_1_80420c80);
                marioVoiceGlareOff();
            }
            *(u8*)((s32)mario + 0x40) = 0;
        }
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 0x400000) && *(u16*)((s32)mario + 0x2E) == 0 &&
        !(*(u32*)((s32)mario + 0x4) & 0x40000000) && *(f32*)((s32)mario + 0x194) != float_0_80420bd4) {
        marioChgPose(str_M_I_Y_80420cbc);
        marioVoiceGlareOn();
    }
    if (marioGetRub(0x100, &v1, &v2, &v3) == 0) {
        return 0;
    }
    return v2 > 0;
}

s32 marioGetJabaraState(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 pose;
    register s32 state = 7;

    if (*(u16*)((s32)mario + 0x2E) != 0x14) {
        state = 0;
    } else {
        pose = *(s32*)((s32)mario + 0x44);
        if (pose == 0x14) {
            state = 1;
        } else if (pose >= 0x46 && pose <= 0x47) {
            state = 2;
        } else if (pose == 0x50) {
            state = 3;
        } else if (pose >= 0x28 && pose <= 0x2B) {
            state = 4;
        } else if (pose >= 0x32 && pose <= 0x34) {
            state = 5;
        } else if (pose >= 0x1E && pose <= 0x1F) {
            state = 6;
        }
    }
    return state;
}

s32 jabaraNoHitChk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);
    s32 pose;

    if (motion != 0x14) {
        return 0;
    }

    pose = *(s32*)((s32)mario + 0x44);
    if (pose >= 0x28 && pose < 0x32) {
        return 1;
    }

    return 0;
}

