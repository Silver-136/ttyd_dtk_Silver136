#include "statuswindow.h"

void* wp;
s32 valueCheck(void);
void statusGetValue(void* out);
void statusWinForceOff(void);
void* __memAlloc(s32 heap, u32 size);
extern s16 alwaysDt[];
extern f32 float_0_80422be8;
extern f32 float_1_80422bf4;
extern f32 float_neg304_80422c68;
extern f32 float_208_80422c6c;
extern f32 float_334_80422c70;

void statusWinDispOff(void) {
    *(u16*)((s32)wp + 4) |= 0x1000;
}

void statusWinDispOn(void) {
    *(u16*)((s32)wp + 4) &= ~0x1000;
}

void N_statusClearBlink(void) {
    *(s32*)((s32)wp + 0x80) = 0;
}

void statusWinForceCloseClear(void) {
    *(u16*)((s32)wp + 4) &= ~0xA0;
}

void statusFPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 2;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusMarioHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 1;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusPartyHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 8;
    *(s32*)((s32)wp + 0x88) = value;
}

s32 statusWinCheck(void) {
    s32 value = *(u16*)((s32)wp + 4) & 0x11;
    return ((u32)(-value) | (u32)value) >> 31;
}

s32 statusWinCheckUpdate(void) {
    s32 value = valueCheck();
    return ((u32)(-value) | (u32)value) >> 31;
}

void statusAPBlink(s32 value) {
    *(s32*)((s32)wp + 0x80) = 0;
    *(u32*)((s32)wp + 0x80) |= 4;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusWinClose(void) {
    *(u16*)((s32)wp + 4) |= 0xA;
    *(u16*)((s32)wp + 4) &= ~5;
}

void statusWinOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) |= 5;
}

void statusWinForceUpdateCoin(void) {
    u8 values[0x14];
    statusGetValue(values);
    *(s16*)((s32)wp + 0x60) = *(s16*)(values + 0x10);
}

void statusWinForceClose(void) {
    *(u16*)((s32)wp + 4) &= ~5;
    *(u16*)((s32)wp + 4) &= ~0x50;
    *(u16*)((s32)wp + 4) |= 0xA0;
}

void statusWinForceOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) &= ~0xA0;
    *(u16*)((s32)wp + 4) |= 0x50;
}


u8 statusWinDisp(void) {
    return 0;
}


u8 statusWinMain(void) {
    return 0;
}


u8 valueUpdate(void) {
    return 0;
}


u8 gaugeDisp(s32 value, f32 x, f32 y) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* pouchGetPtr(void);
    extern void iconDispGx(LocalVec3* pos, s32 size, s32 iconId, f32 scale);
    extern u16 gauge_wakka[];
    extern u16 gauge_back[];
    extern f32 float_0_80422be8;
    extern f32 float_1_80422bf4;
    extern f32 float_12_80422c58;
    s32 max;
    s32 i;
    s32 enabled;
    s32 full;
    s32 rest;
    s32 fill;
    LocalVec3 pos;

    max = 0;
    for (i = 0; i < 8; i++) {
        if ((*(u16*)((s32)pouchGetPtr() + 0x8C) & (1 << i)) != 0) {
            max += 100;
        }
    }
    if (value > max) {
        value = max;
    }
    if (value < 0) {
        value = 0;
    }

    full = value / 100;
    rest = value % 100;
    fill = (rest * 15) / 100;
    if (rest != 0 && value != 0 && fill == 0) {
        fill = 1;
    }

    if (fill != 0) {
        pos.x = x + (f32)(full << 5);
        pos.y = y;
        pos.z = float_0_80422be8;
        iconDispGx(&pos, 0x10, gauge_wakka[fill], float_1_80422bf4);
    }

    y += float_12_80422c58;
    for (i = 0; i < 8; i++) {
        if ((*(u16*)((s32)pouchGetPtr() + 0x8C) & (1 << i)) != 0) {
            pos.x = x + (f32)(i << 5);
            pos.y = y;
            pos.z = float_0_80422be8;
            if (i < full) {
                enabled = gauge_back[i];
            } else {
                enabled = 0x1C7;
            }
            iconDispGx(&pos, 0x10, enabled, float_1_80422bf4);
        }
    }
    return 0;
}

s32 valueCheck(void) {
    extern void* gp;
    s16 values[12];
    void* work;
    s32 changed;
    s32 value;

    changed = 0;
    statusGetValue(values);

    work = wp;
    if (*(s16*)((s32)work + 0x50) != values[0]) {
        *(u32*)((s32)work + 0x84) |= 1;
        changed = 1;
    }

    work = wp;
    if (*(s16*)((s32)work + 0x52) != values[1]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x54) != values[2]) {
        *(u32*)((s32)work + 0x84) |= 2;
        changed = 1;
    }

    work = wp;
    if (*(s16*)((s32)work + 0x56) != values[3]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x62) != values[9]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x64) != values[10]) {
        changed = 1;
    }

    if (*(s16*)((s32)work + 0x58) != values[4]) {
        *(s16*)((s32)work + 0x58) = values[4];
    } else if (*(s16*)((s32)work + 0x58) != 0) {
        if (*(s16*)((s32)work + 0x5A) != values[5]) {
            *(u32*)((s32)work + 0x84) |= 8;
            changed = 1;
        }
        if (*(s16*)((s32)wp + 0x5C) != values[6]) {
            changed = 1;
        }
    }

    work = wp;
    if (*(s16*)((s32)work + 0x5E) != values[7]) {
        changed = 1;
    }
    if (*(s16*)((s32)work + 0x60) != values[8]) {
        *(u32*)((s32)work + 0x84) |= 0x10;
        changed = 1;
    }

    if (changed == 0) {
        work = wp;
        value = *(s32*)((s32)work + 0x84);
        if (value != 0) {
            *(s32*)((s32)work + 0x80) = value;
            *(s32*)((s32)wp + 0x84) = 0;
            *(s32*)((s32)wp + 0x88) = *(s32*)((s32)gp + 4) << 1;
        }
    } else {
        *(s32*)((s32)wp + 0x80) = 0;
    }

    return changed;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void statusGetValue(void* out) {
    extern void* gp;
    extern void* _battleWorkPointer;
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxFP(void);
    extern s32 pouchGetAP(void);
    extern s32 pouchGetMaxAP(void);
    extern s32 pouchGetStarPoint(void);
    extern s32 pouchGetCoin(void);
    extern void* pouchGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern s32 marioGetParty(void);
    extern void* marioGetPtr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 partyId);
    s16* values = out;

    if (*(s32*)((s32)gp + 0x14) == 0) {
        void* pouch;
        s32 partyId;
        s32 offset;

        values[0] = pouchGetHP();
        values[1] = pouchGetMaxHP();
        values[2] = pouchGetFP();
        values[3] = pouchGetMaxFP();
        values[9] = pouchGetAP();
        values[10] = pouchGetMaxAP();
        values[7] = pouchGetStarPoint();
        values[8] = pouchGetCoin();
        if (marioBgmodeChk() == 0) {
            partyId = (s16)marioGetParty();
        } else {
            partyId = *(s8*)((s32)marioGetPtr() + 0x247);
        }
        values[4] = partyId;

        pouch = pouchGetPtr();
        offset = values[4] * 0xE;
        values[5] = *(s16*)((s32)pouch + offset + 6);

        pouch = pouchGetPtr();
        offset = values[4] * 0xE;
        values[6] = *(s16*)((s32)pouch + offset + 2);
    } else {
        void* battleWork = _battleWorkPointer;
        void* mario = BattleGetMarioPtr(battleWork);
        void* party = BattleGetPartyPtr(battleWork);

        values[0] = *(s16*)((s32)mario + 0x10C);
        values[1] = *(s16*)((s32)mario + 0x108);
        values[2] = *(s16*)((s32)mario + 0x112);
        values[3] = *(s16*)((s32)mario + 0x10E);
        values[9] = pouchGetAP();
        values[10] = pouchGetMaxAP();
        values[7] = pouchGetStarPoint();
        values[8] = pouchGetCoin();
        if (party != 0) {
            values[4] = BattleTransPartyId(*(s32*)((s32)party + 8));
            values[5] = *(s16*)((s32)party + 0x10C);
            values[6] = *(s16*)((s32)party + 0x108);
        } else {
            values[4] = 0;
            values[5] = 0;
            values[6] = 0;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void statusWinReInit(void) {
    void* work = wp;
    s32 zero = 0;
    u16 flags;

    *(s32*)((s32)work + 0x74) = zero;
    *(s32*)((s32)wp + 0x78) = zero;

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x10) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x10;
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    } else if ((flags & 1) != 0) {
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    }

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x20) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x20;
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    } else if ((flags & 2) != 0) {
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    }
}

void statusWinForceOff(void) {
    u16 flags = *(u16*)((s32)wp + 4);

    if ((flags & 0x10) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x10;
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    } else if ((flags & 1) != 0) {
        *(u16*)((s32)wp + 4) &= ~0xA;
        *(u16*)((s32)wp + 4) |= 5;
    }

    flags = *(u16*)((s32)wp + 4);
    if ((flags & 0x20) != 0) {
        *(u16*)((s32)wp + 4) = flags & ~0x20;
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    } else if ((flags & 2) != 0) {
        *(u16*)((s32)wp + 4) |= 0xA;
        *(u16*)((s32)wp + 4) &= ~5;
    }
}

void statusWinForceUpdate(void) {
    s16 values[11];

    statusGetValue(values);
    *(s16*)((s32)wp + 0x50) = values[0];
    *(s16*)((s32)wp + 0x52) = values[1];
    *(s16*)((s32)wp + 0x54) = values[2];
    *(s16*)((s32)wp + 0x56) = values[3];
    *(s16*)((s32)wp + 0x62) = values[9];
    *(s16*)((s32)wp + 0x64) = values[10];
    *(s16*)((s32)wp + 0x5E) = values[7];
    *(s16*)((s32)wp + 0x60) = values[8];
    *(s16*)((s32)wp + 0x58) = values[4];
    if (*(s16*)((s32)wp + 0x58) != 0) {
        *(s16*)((s32)wp + 0x5A) = values[5];
        *(s16*)((s32)wp + 0x5C) = values[6];
    }
}


void statusWinInit(void) {
    f32 one;
    f32 negX;
    f32 posY;
    f32 posY2;

    wp = __memAlloc(0, 0x8C);
    memset(wp, 0, 0x8C);

    one = float_1_80422bf4;
    negX = float_neg304_80422c68;
    posY = float_208_80422c6c;
    posY2 = float_334_80422c70;

    *(f32*)((s32)wp + 0x1C) = one;
    *(f32*)((s32)wp + 0x30) = negX;
    *(f32*)((s32)wp + 0x34) = posY;
    *(f32*)((s32)wp + 0x40) = negX;
    *(f32*)((s32)wp + 0x44) = posY2;
    *(s32*)((s32)wp + 0x74) = 0;
    *(s32*)((s32)wp + 0x78) = 0;
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) |= 5;
}


void statusGetApPos(f32* out) {
    typedef struct StatusVec {
        f32 x;
        f32 y;
        f32 z;
    } StatusVec;
    StatusVec pos;
    s16 xOffset = alwaysDt[0x90 / 2];
    s16 yOffset = alwaysDt[0x92 / 2];
    void* work = wp;

    pos.x = *(f32*)((s32)work + 0x20) + xOffset;
    pos.y = *(f32*)((s32)work + 0x24) - yOffset;
    pos.z = float_0_80422be8;
    *(StatusVec*)out = pos;
}
