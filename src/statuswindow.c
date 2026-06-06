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


u8 gaugeDisp(void) {
    return 0;
}


s32 valueCheck(void) {
    return 0;
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
