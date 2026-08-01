#include "battle/battle_party.h"

typedef struct PartyEntry {
    void* data;
    u32 unk4;
    u32 flags;
    f32 maxHp;
    f32 unk10;
    f32 maxFp;
    u8 reserved[0x18];
} PartyEntry;

extern u8 unitdata_Party_Christine[];
extern u8 unitdata_Party_Nokotarou[];
extern u8 unitdata_Party_Yoshi[];
extern u8 unitdata_Party_Clauda[];
extern u8 unitdata_Party_Vivian[];
extern u8 unitdata_Party_Sanders[];
extern u8 unitdata_Party_Chuchurina[];

#define PARTY_ENTRY(unitData) \
    { unitData, 0, 0x04000002, 1000.0f, 0.0f, 1000.0f, { 0 } }

PartyEntry entryunit_party[] = {
    PARTY_ENTRY(unitdata_Party_Christine),
    PARTY_ENTRY(unitdata_Party_Nokotarou),
    PARTY_ENTRY(unitdata_Party_Yoshi),
    PARTY_ENTRY(unitdata_Party_Clauda),
    PARTY_ENTRY(unitdata_Party_Vivian),
    PARTY_ENTRY(unitdata_Party_Sanders),
    PARTY_ENTRY(unitdata_Party_Chuchurina),
};

#undef PARTY_ENTRY

__declspec(section ".data") f32 lbl_803638E0[2] = { -1.0f, 1.0f };
