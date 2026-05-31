#include "party/party_kuribo.h"

extern s32 msg_ep;
extern s32 marioGetPartyId();
extern void* partyGetPtr();
extern void marioStSystemLevel();
extern void psndClearFlag();

s32 pre_kurio_use(void* pEvt) {
    return 0;
}


u8 kuribo_use(void* pParty) {
    return 0;
}


s32 post_kurio_use(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party == NULL) {
        return 2;
    }
    *(u8*)((s32)party + 0x39) = 0x14;
    marioStSystemLevel(0);
    psndClearFlag(0x80);
    *(u32*)party &= ~0x2000000;
    *(u32*)party &= ~0x4000000;
    return 2;
}


u8 kuribo_bye(void* pParty) {
    return 0;
}


s32 christineGetStatus(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != NULL && *(s8*)((s32)party + 0x31) == 1 && (*(u32*)party & 0x100)) {
        return 1;
    }
    return 0;
}


u8 kuribo_init(void* pParty) {
    *(u8*)((s32)pParty + 0x33) = 1;
    *(u8*)((s32)pParty + 0x32) = 0;
    *(f32*)((s32)pParty + 0xF0) = 24.0f;
    *(f32*)((s32)pParty + 0xF4) = 26.0f;
    msg_ep = 0;
}


void kuribo_use_post(void* pParty) {
    *(u32*)pParty &= ~0x2000000;
    *(u32*)pParty &= ~0x4000000;
}
