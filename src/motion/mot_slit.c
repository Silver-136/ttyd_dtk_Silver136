#include "motion/mot_slit.h"

void* marioGetPtr(void);
void marioChgMot2(s32 motionId);

typedef struct SlitVecCopy {
    s32 x;
    s32 y;
    s32 z;
} SlitVecCopy;

void motSlitContinue(void) {
    s32 timer;
    marioChgMot2(0x15);
    timer = 0x14;
    *(s16*)((s32)marioGetPtr() + 0x2F0) = timer;
}

void N_marioSlitSetAfterLand(void) {
    void* mario = marioGetPtr();
    marioChgMot2(0x15);
    *(u32*)mario &= ~0xF0000;
}

void marioClearSlitFloor(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x26C) = 0;
}

void motSlitCancel(void) {
    void* mario = marioGetPtr();
    if (*(u32*)mario & 0x100000) {
        *(s16*)((s32)mario + 0x2F0) = 0x64;
    }
}

void motSlitCancel3(void) {
    void* mario = marioGetPtr();
    if (*(u32*)mario & 0x100000) {
        *(s16*)((s32)mario + 0x2F0) = 0x78;
    }
}

s32 marioGetSlitFloorPos(SlitVecCopy* out) {
    void* mario = marioGetPtr();
    if (*(s32*)((s32)mario + 0x26C) != 0) {
        s32 x = *(s32*)((s32)mario + 0x270);
        s32 y = *(s32*)((s32)mario + 0x274);
        s32 z;
        out->x = x;
        out->y = y;
        z = *(s32*)((s32)mario + 0x278);
        out->z = z;
        return 1;
    }
    return 0;
}

s32 marioGetMotSlitCancel3(void) {
    register s32 ret = 0;
    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x15) {
        s32 timer = *(s16*)((s32)mario + 0x2F0);
        if ((timer >= 0x78 && timer <= 0x7A) || (timer >= 0x6E && timer <= 0x70)) {
            ret = 1;
        }
    }
    return ret;
}

s32 marioSlitKeyDisable(void) {
    void* mario = marioGetPtr();
    if ((*(u32*)mario & 0x100000) == 0) {
        return 1;
    }
    {
        s32 timer = *(s16*)((s32)mario + 0x2F0);
        if (timer >= 0x50 && timer <= 0x52) {
            return 0;
        }
        if (timer >= 0x5A && timer <= 0x5E) {
            return 0;
        }
    }
    return 1;
}

void unk_800a6ac8(void) {
    void* mario = marioGetPtr();

    marioChgMot2(0x15);
    *(s16*)((s32)mario + 0x2F0) = 0xA;
    *(u32*)mario &= ~0xF0000;
}

u8 mot_slit(void) {
    return 0;
}


u8 marioCreviceWallChk(void) {
    return 0;
}


u8 mot_slit_post(void) {
    return 0;
}


u8 marioReInit_slit(void) {
    return 0;
}


u32 U_marioSlitContinueChk(void) {
    return 0;
}


u8 marioSlitForceCancel(void) {
    return 0;
}


s32 marioSlitButton(void) {
    return 0;
}


u32 marioSlitAbilityChk(void) {
    return 0;
}


u8 marioForceSlitAnime(void) {
    return 0;
}


s32 marioChkSlitThrouhEnd(void) {
    return 0;
}


s32 marioChkSlitThrouh(void) {
    return 0;
}


int marioChkSlitEnd(void) {
    return 0;
}


u8 motSlitCancel2(void) {
    return 0;
}
