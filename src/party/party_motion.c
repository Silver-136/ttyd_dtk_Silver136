#include "party/party_motion.h"

extern f32 float_60_8042318c;
f32 touch_wall_ct;

void* camGetPtr(s32 id);
f32 revise360(f32 angle);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void* partyGetPtr(s32 id);

void N_partyFollowCloseOff(void* party) {
    *(u32*)((s32)party + 8) &= ~0x20000;
}

void N_partyFollowCloseOn(void* party) {
    *(u32*)((s32)party + 8) |= 0x20000;
}

void unk_80151d20(void) {
    touch_wall_ct = float_60_8042318c;
}

void partyChgMotSlitFloor(void* party) {
    if (!(*(u32*)party & 0x2000)) {
        void* unk = *(void**)((s32)party + 0x160);

        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
        *(u32*)party |= 0x2000;
    }
}

void party_set_behind_dir(void* party, f32 dir) {
    void* cam = camGetPtr(4);

    *(f32*)((s32)party + 0x54) = revise360(dir + *(f32*)((s32)cam + 0x114));
    *(u32*)((s32)party + 8) |= 0x100000;
}

void allPartySlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }
}

