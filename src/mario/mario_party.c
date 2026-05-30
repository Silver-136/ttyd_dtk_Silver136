#include "mario/mario_party.h"

s32 partyGetTechLv(s32 partyId) {
    extern void* pouchGetPtr(void);

    s32 value = 0;
    if (partyId < 8) {
        value = ((s16*)pouchGetPtr())[partyId * 7 + 6];
    }
    return value;
}

s32 partyGetHp(s32 partyId) {
    extern void* pouchGetPtr(void);

    s32 value = 0;
    if (partyId < 8) {
        value = ((s16*)pouchGetPtr())[partyId * 7 + 3];
    }
    return value;
}


s32 marioUseParty(void) {
    return 0;
}


int marioPartyEntry(int partyMemberId) {
    return 0;
}


int marioGetParty(void) {
    return 0;
}


int marioPartyHello(int partyMemberId) {
    return 0;
}


s32 marioGetExtraPartyId(void) {
    return 0;
}


s32 marioGetPartyId(void) {
    return 0;
}


u8 marioPartyKill(void) {
    return 0;
}


u32 partyChkJoin(int param_1) {
    return 0;
}


u32 marioPartyGoodbye(void) {
    return 0;
}


void partyLeft(s32 partyId) {
}


void partyJoin(s32 partyId) {
}
