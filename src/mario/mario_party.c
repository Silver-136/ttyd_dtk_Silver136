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
