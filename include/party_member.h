// PartyMember: the party member ids (the seven partners, the other followers, and the partners' follower forms),
// used by the party code (party.h) and the pouch (mario_pouch.h). No other includes, so either can use it.
// The enum is byte-sized (enumsalwaysint off); the pouch and mario_party functions take and keep partner ids as
// s32 (the int size the code was built with), so they are declared s32 there.

#pragma once

#pragma enumsalwaysint off
typedef enum PartyMember {
    PARTY_NONE,      // 0
    PARTY_GOOMBELLA, // 1
    PARTY_KOOPS,     // 2
    PARTY_BOBBERY,   // 3
    PARTY_YOSHI,     // 4
    PARTY_FLURRIE,   // 5
    PARTY_VIVIAN,    // 6
    PARTY_MS_MOWZ,   // 7
#define PARTNER_COUNT (PARTY_MS_MOWZ + 1) // the seven partners and PARTY_NONE
    PARTY_EGG,                // 8
    PARTY_FLAVIO,             // 9
    PARTY_PUNIO,              // 10
    PARTY_FRANKLY,            // 11
    PARTY_CRAW,               // 12
    PARTY_GOOMBELLA_FOLLOWER, // 13
    PARTY_KOOPS_FOLLOWER,     // 14
    PARTY_BOBBERY_FOLLOWER,   // 15
    PARTY_YOSHI_FOLLOWER,     // 16
    PARTY_FLURRIE_FOLLOWER,   // 17
    PARTY_VIVIAN_FOLLOWER,    // 18
    PARTY_MS_MOWZ_FOLLOWER    // 19
} PartyMember;
#pragma enumsalwaysint on
