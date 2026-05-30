#include "party/party_chuchu.h"

extern f32 float_20_80424374;
extern f32 float_24_80424378;

s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

void chuchu_exit(void) {
}

void chuchu_reinit(void) {
}

void chuchu_init(void* party) {
    *(u8*)((s32)party + 0x33) = 7;
    *(u8*)((s32)party + 0x32) = 0;
    *(f32*)((s32)party + 0xF0) = float_20_80424374;
    *(f32*)((s32)party + 0xF4) = float_24_80424378;
}

s32 post_msg(void* party) {
    void* current;

    current = partyGetPtr(marioGetPartyId());
    *(s32*)((s32)party + 0x9C) = 0;
    if (current != 0) {
        (*(u8*)((s32)current + 0x39))++;
    }
    return 2;
}


u8 set_msg(void* pEvt) {
    return 0;
}


u8 chuchu_searchObject(void) {
    return 0;
}


s32 search_result(void* pEvt, int param_2) {
    return 0;
}


u8 chuchu_use(void* pParty) {
    return 0;
}


u8 chuchu_move(void* pParty) {
    return 0;
}
