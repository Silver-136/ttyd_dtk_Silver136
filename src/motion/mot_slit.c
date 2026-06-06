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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 U_marioSlitContinueChk(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioBgmodeChk(void);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioCreviceWallChk(void* pos);
    SlitVecCopy pos;
    void* mario = marioGetPtr();
    void* mario2 = marioGetPtr();
    s32 button;

    if (*(u32*)((s32)mario2 + 0xC) & 2) {
        button = 0;
    } else if (marioKeyOffChk() != 0) {
        button = 0;
    } else if (marioBgmodeChk() == 1) {
        button = 0;
    } else if (!(*(u32*)((s32)mario2 + 0x4) & 0x1000000)) {
        button = (*(u16*)((s32)mario2 + 0x24A) & 0x20) ? 1 : 0;
    } else if (*(u16*)((s32)mario2 + 0x24A) & 0x20) {
        button = 2;
    } else if (*(u8*)((s32)mario2 + 0x257) >= 0x32) {
        button = 2;
    } else {
        button = 0;
    }
    if (button != 0) {
        return 1;
    }
    if (marioSlitChkWallAround() != 0) {
        return 1;
    }
    pos.x = *(s32*)((s32)mario + 0x8C);
    pos.y = *(s32*)((s32)mario + 0x90);
    pos.z = *(s32*)((s32)mario + 0x94);
    button = marioCreviceWallChk(&pos);
    return ((u32)(-button) | (u32)button) >> 31;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioSlitForceCancel(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void mapResetPaperAmbColor(void);
    extern void marioAdjustMoveDir(void);
    extern void allPartyForceSlitOff(void);
    extern f32 float_180_80420fa8;
    extern f32 float_0_80420f9c;
    extern f32 float_10_80421000;
    extern f32 float_0p004_80420fbc;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    if (flags & 0x100000) {
        *(u32*)mario = flags & ~0x200000;
        if (*(f32*)((s32)mario + 0x1A4) >= float_180_80420fa8) {
            *(f32*)((s32)mario + 0x1AC) = float_0_80420f9c;
        } else {
            *(f32*)((s32)mario + 0x1AC) = float_180_80420fa8;
        }
        *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
        *(u32*)((s32)mario + 0x4) &= 0xFEFFFFFE;
        *(u32*)mario &= ~0x400000;
        *(u32*)mario &= ~0x100000;
        *(f32*)((s32)mario + 0x1B8) = float_10_80421000;
        *(u32*)((s32)mario + 0x4) &= 0xFEFFFFFE;
        marioPaperOff();
        mapResetPaperAmbColor();
        *(u32*)((s32)mario + 0x4) &= ~0x4;
        marioAdjustMoveDir();
        mario = marioGetPtr();
        *(f32*)((s32)mario + 0x148) = float_0_80420f9c;
        *(f32*)((s32)mario + 0x158) = float_0p004_80420fbc;
        allPartyForceSlitOff();
    }
}

s32 marioSlitButton(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioBgmodeChk(void);
    void* mario = marioGetPtr();
    if (*(u32*)((s32)mario + 0xC) & 2) {
        return 0;
    }
    if (marioKeyOffChk() != 0) {
        return 0;
    }
    if (marioBgmodeChk() == 1) {
        return 0;
    }
    if (!(*(u32*)((s32)mario + 0x4) & 0x1000000)) {
        if (*(u16*)((s32)mario + 0x24A) & 0x20) {
            return 1;
        }
    } else {
        if (*(u16*)((s32)mario + 0x24A) & 0x20) {
            return 2;
        }
        if (*(u8*)((s32)mario + 0x257) >= 0x32) {
            return 2;
        }
    }
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
