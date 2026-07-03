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
s32 _bsi_regeneration(BattleWorkUnit* unit, s32* desc, s32* out) {
    extern BOOL BtlUnit_CheckStatus(BattleWorkUnit* unit, s8 status);
    extern void BtlUnit_GetStatus(BattleWorkUnit* unit, s8 status, s8* turns, s8* strength);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern u32 vec3_80302838[];
    extern u32 dat_804287a0;
    extern u32 dat_804287a4;
    extern u32 float_0p10207_804287a8;
    extern u32 dat_804287ac;
    extern u32 dat_804287b0;

    s8 turns;
    s8 strength;
    s32 status;
    s32 turnCount;
    s32 phase;
    s16* counter;
    u32* base;
    u8* from;
    u8* to;
    u32 color;

    base = vec3_80302838;
    status = desc[0];
    if (status == 0x17) {
        counter = (s16*)((s32)unit + 0xAE0);
        from = (u8*)&dat_804287a0;
        to = (u8*)&dat_804287a4;
    } else if (status == 0x18) {
        counter = (s16*)((s32)unit + 0xAE2);
        from = (u8*)&float_0p10207_804287a8;
        to = (u8*)&dat_804287ac;
    } else {
        return 0;
    }

    if (!BtlUnit_CheckStatus(unit, status)) {
        *counter = 0;
        return 0;
    }

    *counter += 1;
    phase = (*counter * 0xC) % 0x2D0;
    if (phase >= 0x168) {
        phase = 0x2D0 - phase;
    }

    BtlUnit_GetStatus(unit, status, &turns, &strength);
    out[0] = status;
    turnCount = -turns;
    if (turns >= 0) {
        turnCount = turns;
    }
    out[1] = turnCount;
    out[2] = desc[2];
    out[3] = base[0x36];
    out[4] = base[0x37];
    out[5] = base[0x38];
    out[6] = base[0x39];
    out[7] = base[0x3A];
    out[8] = base[0x3B];
    out[9] = base[0x3C];
    out[10] = base[0x3D];
    out[11] = base[0x3E];
    out[15] = dat_804287b0;

    color = 0;
    ((u8*)&color)[0] = (u8)(s32)intplGetValue(0xB, phase, (f32)from[0], (f32)to[0], 0x168);
    ((u8*)&color)[1] = (u8)(s32)intplGetValue(0xB, phase, (f32)from[1], (f32)to[1], 0x168);
    ((u8*)&color)[2] = (u8)(s32)intplGetValue(0xB, phase, (f32)from[2], (f32)to[2], 0x168);
    out[15] = color;
    out[16] = desc[3];
    out[12] = base[0x3F];
    out[13] = base[0x40];
    out[14] = base[0x41];
    return 1;
}

/* fallback stub-fill: map=_bsi_biribiri addr=0x8025337c size=0x000001e8 */
s32 _bsi_biribiri(BattleWorkUnit* unit, s32* desc, s32* out) {
    extern BOOL BtlUnit_CheckStatus(BattleWorkUnit* unit, s8 status);
    extern void BtlUnit_GetStatus(BattleWorkUnit* unit, s8 status, s8* turns, s8* strength);
    extern s32 irand(s32 max);
    extern u32 vec3_80302838[];
    extern u32 dat_8042879c;

    s8 turns;
    s8 strength;
    s32 status;
    s32 turnCount;
    u32* base;

    base = vec3_80302838;
    status = desc[0];
    if (!BtlUnit_CheckStatus(unit, status)) {
        *(s16*)((s32)unit + 0xAE4) = 0;
        *(s16*)((s32)unit + 0xAE6) = 0;
        return 0;
    }

    out[3] = base[0x24];
    out[4] = base[0x25];
    out[5] = base[0x26];

    switch (*(s16*)((s32)unit + 0xAE4)) {
        case 0:
            *(s16*)((s32)unit + 0xAE4) += 1;
            *(s16*)((s32)unit + 0xAE6) = irand(6) + 4;
            /* fallthrough */
        case 1:
            out[3] = base[0x27];
            out[4] = base[0x28];
            out[5] = base[0x29];
            *(s16*)((s32)unit + 0xAE6) -= 1;
            if (*(s16*)((s32)unit + 0xAE6) < 0) {
                *(s16*)((s32)unit + 0xAE4) += 1;
                *(s16*)((s32)unit + 0xAE6) = irand(0x64) + 0x1E;
            }
            break;
        case 2:
            out[3] = base[0x2A];
            out[4] = base[0x2B];
            out[5] = base[0x2C];
            *(s16*)((s32)unit + 0xAE6) -= 1;
            if (*(s16*)((s32)unit + 0xAE6) < 0) {
                *(s16*)((s32)unit + 0xAE6) = irand(6) + 4;
                *(s16*)((s32)unit + 0xAE4) = 1;
            }
            break;
    }

    BtlUnit_GetStatus(unit, status, &turns, &strength);
    out[0] = status;
    turnCount = -turns;
    if (turns >= 0) {
        turnCount = turns;
    }
    out[1] = turnCount;
    out[2] = desc[2];
    out[6] = base[0x2D];
    out[7] = base[0x2E];
    out[8] = base[0x2F];
    out[9] = base[0x30];
    out[10] = base[0x31];
    out[11] = base[0x32];
    out[15] = dat_8042879c;
    out[16] = desc[3];
    out[12] = base[0x33];
    out[13] = base[0x34];
    out[14] = base[0x35];
    return 1;
}

