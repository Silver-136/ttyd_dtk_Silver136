#include "motion/mot_yoshi.h"

void* marioGetPtr(void);
s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);
void psndSFXOff(s32 sfxId);
void bero_clear_Offset(void);

typedef struct YoshiVec {
    f32 x;
    f32 y;
    f32 z;
} YoshiVec;

extern YoshiVec vec3_802f38e0[];
extern YoshiVec vec3_802f38ec[];
extern f32 float_0_80422bc0;
void mot_yoshi(void) {
    void* mario = marioGetPtr();

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        f32 zero;

        *(u32*)((s32)mario + 0xC) &= ~1;
        zero = float_0_80422bc0;
        *(u32*)mario &= ~0xF0000;
        *(u32*)((s32)mario + 0x4) |= 4;
        *(u32*)((s32)mario + 0x4) |= 0x1000;
        *(s32*)((s32)mario + 0x48) = 0;
        *(u16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        *(s32*)((s32)mario + 0x1E0) = 0;
        *(s32*)((s32)mario + 0x1E4) = 0;
        *(s32*)((s32)mario + 0x1E8) = 0;
        *(s32*)((s32)mario + 0x1F0) = 0;
        *(s32*)((s32)mario + 0x1F4) = 0;
        *(s32*)((s32)mario + 0x1F8) = 0;
        *(s32*)((s32)mario + 0x1FC) = 0;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_yoshi_post(void) {
    void* party = partyGetPtr(marioGetPartyId());
    void* mario = marioGetPtr();
    s32 sfxId = *(s32*)((s32)party + 0x168);

    if ((u32)(sfxId + 0x10000) != 0xFFFF) {
        psndSFXOff(sfxId);
        *(s32*)((s32)party + 0x168) = -1;
    }
    bero_clear_Offset();
    *(u32*)((s32)mario + 0x4) &= ~4;
    *(u32*)((s32)mario + 0x4) &= ~0x1000;
    *(YoshiVec*)((s32)mario + 0x98) = vec3_802f38e0[0];
    *(YoshiVec*)((s32)mario + 0xA4) = vec3_802f38ec[0];
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
