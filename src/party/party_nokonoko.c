#include "party/party_nokonoko.h"

extern s32 nokoSe;
extern f32 float_37_80421678;
extern f32 float_24_80421708;

void partyGoodbyeInit(void* party);
s32 partyGoodbyeMain(void* party);
void partyChgRunMode(void* party, s32 mode);
s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

void nokotaro_hold_cancel(void* party) {
    *(u8*)((s32)party + 0x39) = 0x14;
}

void nokonoko_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 2;
    height = float_37_80421678;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_24_80421708;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    nokoSe = -1;
}

void nokonoko_bye(void* party) {
    if (*(u32*)((s32)party + 8) & 8) {
        *(u32*)((s32)party + 8) &= ~8;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party) != 0) {
        partyChgRunMode(party, 0xE);
    }
}

s32 nokonoko_holdItem(void) {
    void* party = partyGetPtr(marioGetPartyId());
    s32 item;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 2 || !(*(u32*)party & 0x100)) {
        return 0;
    }

    item = *(s32*)((s32)party + 0x174);
    return ((u32)(-item) | (u32)item) >> 31;
}


u8 nokonoko_use(void* party) {
    return 0;
}


u8 ccwall(void* party, s32 param_2) {
    return 0;
}


u8 lookupSafetyPos(void) {
    return 0;
}


u8 lookupSafetyPos2(void) {
    return 0;
}


u8 lookupSafetyPos3(void) {
    return 0;
}


u8 lookupSafetyPosSub(void) {
    return 0;
}


u8 nokonoko_finish(void* party) {
    return 0;
}


u8 nokonoko_use_post(void* party) {
    return 0;
}


s32 nokonokoGetStatus(void* party) {
    return 0;
}


u8 nokonoko_move(void* party) {
    return 0;
}
