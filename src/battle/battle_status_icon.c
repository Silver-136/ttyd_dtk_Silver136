#include <dolphin/types.h>

void* memset(void*, int, unsigned long);
typedef struct BattleWorkUnit BattleWorkUnit;
void BtlUnit_GetStatus(BattleWorkUnit* unit, s8 status, s8* turns, s8* strength);
BOOL BtlUnit_CheckStatus(BattleWorkUnit* unit, s8 status);
extern void* _battleWorkPointer;
extern u32 vec3_80302838[];
extern u32 dat_80428790;
extern u32 dat_80428794;
extern u32 dat_80428798;
extern f32 float_0_804287b4;
extern f32 float_4_804287b8;

typedef struct BSITriplet {
    u32 x;
    u32 y;
    u32 z;
} BSITriplet;

typedef struct BSIVec {
    f32 x;
    f32 y;
    f32 z;
} BSIVec;

extern BSITriplet vec3_80302844;
extern BSITriplet vec3_80302850;
extern BSITriplet vec3_8030285c;
extern BSITriplet vec3_80302868;
extern BSITriplet vec3_80302874;
extern BSITriplet vec3_80302880;
extern BSITriplet vec3_8030288c;
extern BSITriplet vec3_80302898;
extern BSITriplet vec3_803028a4;
extern BSITriplet vec3_803028b0;
extern BSITriplet vec3_803028bc;

void BattleStatusIconDelete(void) {
    ;
}

void BattleStatusIconInit(BattleWorkUnit* unit) {
    memset((void*)((s32)unit + 0x348), 0, 0x7A0);
}

s32 _bsi_allergy(BattleWorkUnit* unit, s32* desc, s32* out) {
    s8 turns;
    s8 strength;
    s32 status = desc[0];
    BSITriplet* base = (BSITriplet*)vec3_80302838;
    s32 turnCount;
    s16 counter;

    if (!BtlUnit_CheckStatus(unit, status)) {
        *(s16*)((s32)unit + 0xADE) = 0;
        return 0;
    }

    BtlUnit_GetStatus(unit, status, &turns, &strength);
    out[0] = status;
    turnCount = -turns;
    if (turns >= 0) {
        turnCount = turns;
    }
    out[1] = turnCount;
    out[2] = desc[2];
    *(BSITriplet*)&out[3] = base[8];
    *(BSITriplet*)&out[6] = base[9];
    *(BSITriplet*)&out[9] = base[10];
    out[15] = dat_80428798;
    out[16] = desc[3];
    *(BSITriplet*)&out[12] = base[11];

    counter = *(s16*)((s32)unit + 0xADE) + 1;
    *(s16*)((s32)unit + 0xADE) = counter;
    if (*(s16*)((s32)unit + 0xADE) & 0x10) {
        out[16] |= 2;
    }
    return 1;
}

s32 _bsi_guruguru(BattleWorkUnit* unit, s32* desc, s32* out) {
    s8 turns;
    s8 strength;
    s32 status = desc[0];
    BSITriplet* base = (BSITriplet*)vec3_80302838;
    s32 turnCount;
    BSIVec temp;

    if (!BtlUnit_CheckStatus(unit, status)) {
        *(f32*)((s32)unit + 0xAD8) = float_0_804287b4;
        return 0;
    }

    *(f32*)((s32)unit + 0xAD8) += float_4_804287b8;
    BtlUnit_GetStatus(unit, status, &turns, &strength);
    out[0] = status;
    turnCount = -turns;
    if (turns >= 0) {
        turnCount = turns;
    }
    out[1] = turnCount;
    out[2] = desc[2];
    *(BSITriplet*)&out[3] = base[4];
    *(BSITriplet*)&out[6] = base[5];
    temp = *(BSIVec*)&base[6];
    temp.z = *(f32*)((s32)unit + 0xAD8);
    *(BSITriplet*)&out[9] = *(BSITriplet*)&temp;
    out[15] = dat_80428794;
    out[16] = desc[3];
    *(BSITriplet*)&out[12] = base[7];
    return 1;
}

s32 _bsi_default_turn(BattleWorkUnit* unit, s32* desc, s32* out) {
    s8 turns;
    s8 strength;
    s32 status = desc[0];
    BSITriplet* base = (BSITriplet*)vec3_80302838;
    s32 turnCount;
    s32 kind;

    if (*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x10000) {
        if (status != 1 && !BtlUnit_CheckStatus(unit, status)) {
            return 0;
        }

        kind = *(s32*)((s32)unit + 8);
        if (!((kind >= 1 && kind < 0xD8) || (kind >= 0xDE && kind < 0xE0) ||
              (kind >= 0xE0 && kind < 0xE7))) {
            return 0;
        }
    } else if (!BtlUnit_CheckStatus(unit, status)) {
        return 0;
    }

    BtlUnit_GetStatus(unit, status, &turns, &strength);
    out[0] = status;
    turnCount = -turns;
    if (turns >= 0) {
        turnCount = turns;
    }
    out[1] = turnCount;
    out[2] = desc[2];
    *(BSITriplet*)&out[3] = base[0];
    *(BSITriplet*)&out[6] = base[1];
    *(BSITriplet*)&out[9] = base[2];
    out[15] = dat_80428790;
    out[16] = desc[3];
    *(BSITriplet*)&out[12] = base[3];
    return 1;
}



/* CHATGPT STUB FILL: main/battle/battle_status_icon 20260624_184929 */

/* stub-fill: BattleStatusIconDisp | missing_definition | ghidra_signature */
void BattleStatusIconDisp(s32 param_1, void* unit) {
    return;
}


/* CHATGPT STUB FILL: main/battle/battle_status_icon 20260624_184929 */

/* stub-fill: BattleStatusIconMain | missing_definition | ghidra_signature */
u8 BattleStatusIconMain(void* unit) {
    return 0;
}


/* CHATGPT FALLBACK MISSING STUBS: main/battle/battle_status_icon 20260624_191429 */

/* fallback stub-fill: map=_bsi_regeneration addr=0x802530fc size=0x00000280 */
int _bsi_regeneration() {
    return 0;
}

/* fallback stub-fill: map=_bsi_biribiri addr=0x8025337c size=0x000001e8 */
int _bsi_biribiri() {
    return 0;
}
