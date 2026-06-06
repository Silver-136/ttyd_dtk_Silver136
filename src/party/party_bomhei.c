#include "party/party_bomhei.h"

s32 bomheiGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);

    void* party = partyGetPtr(marioGetPartyId());
    s32 status = 2;

    if (party == NULL || ((s32)*(u8*)((s32)party + 0x31) == 3 && !(*(u32*)party & 0x100))) {
        return 0;
    }

    if (*(u8*)((s32)party + 0x39) == 0x3C) {
        status = 1;
    }
    return status;
}

void bomhei_init(void* party) {
    extern f32 float_28_804213d8;

    *(u8*)((s32)party + 0x33) = 3;
    {
        f32 temp = float_28_804213d8;

        *(u8*)((s32)party + 0x32) = 0;
        *(f32*)((s32)party + 0xF0) = temp;
        *(f32*)((s32)party + 0xF4) = temp;
    }
    *(u8*)((s32)party + 0x17A) = 100;
    *(s32*)((s32)party + 0x17C) = -1;
    *(s32*)((s32)party + 0x180) = -1;
    *(s32*)((s32)party + 0x184) = -1;
}


u8 bomhei_use_main(void* party) {
    return 0;
}


u8 partyGetReturnPos(void) {
    return 0;
}


u8 bomhei_use(void* pParty) {
    return 0;
}


u8 check_front(void* pParty) {
    return 0;
}


u8 mario_controll(void* pParty) {
    return 0;
}


u8 bomhei_use_post(void* party) {
    return 0;
}


u8 bomhei_use_cancel2(void) {
    return 0;
}


u8 bomhei_exit(void* pParty) {
    return 0;
}


u8 bomhei_use_cancel(void) {
    return 0;
}


s32 mario_bomhei_keychk(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || (state >= 0x14 && state <= 0x15) || state == 0x65 || (state >= 0x28 && state <= 0x29) || (state >= 0x1E && state <= 0x24)) return 0;
    state = *(u8*)((s32)party + 0x39);
    if ((state >= 0x32 && state <= 0x33) || state == 0x3C || (state >= 0x46 && state <= 0x47)) return 0;
    return 1;
}

u8 bomhei_bye(void* pParty) {
    return 0;
}


s32 mario_bomhei_keychk2(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || state == 0x65 || (state >= 0x1E && state <= 0x24)) return 0;
    return 1;
}

