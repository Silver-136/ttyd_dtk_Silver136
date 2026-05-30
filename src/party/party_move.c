#include "party/party_move.h"

s32 readId[2];
s32 writeId[2];

extern u8 footmarkBuf[];
extern f32 float_0_804215b8;
extern f32 float_0p75_80421608;
extern f32 float_1_804215f8;

void partyMoveMain(void* party, s32 hosei);
f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
void psndSFXOn_3D(s32 id, void* pos);

s32 partyGetFootmarkId(void* party) {
    return (*(u32*)((s32)party + 0x0) >> 2) & 1;
}

s32 partyGetFootmarkReadId(s32 id) {
    return readId[id];
}

s32 partyGetFootmarkWriteId(s32 id) {
    return writeId[id];
}

void partyClearJumpPara(void* party) {
    f32 zero;

    zero = float_0_804215b8;
    *(u32*)((s32)party + 0x0) &= ~0x70;
    *(f32*)((s32)party + 0x114) = zero;
    *(f32*)((s32)party + 0x118) = zero;
    *(f32*)((s32)party + 0x11C) = zero;
}

void* partyGetFootmarkPtr(s32 id) {
    s32 slotOfs;
    s32 idOfs;
    s32 read;
    u8* mark;

    slotOfs = id * 0x500;
    idOfs = id << 2;
    read = *(s32*)((s32)readId + idOfs);

    mark = footmarkBuf;
    mark += slotOfs;
    mark += read << 4;

    return mark;
}

void partyGetReadId(s32 id, f32* x, f32* y, f32* z) {
    s32 slotOfs;
    s32 idOfs;
    s32 read;
    u8* mark;

    slotOfs = id * 0x500;
    idOfs = id << 2;
    read = *(s32*)((s32)readId + idOfs);

    mark = footmarkBuf;
    mark += slotOfs;
    mark += read << 4;

    *x = *(f32*)(mark + 0x4);
    *y = *(f32*)(mark + 0x8);
    *z = *(f32*)(mark + 0xC);
}

void partyGetReadId2(s32 id, f32* out) {
    s32 slotOfs;
    s32 idOfs;
    s32 read;
    u8* mark;

    slotOfs = id * 0x500;
    idOfs = id << 2;
    read = *(s32*)((s32)readId + idOfs);

    mark = footmarkBuf;
    mark += slotOfs;
    mark += read << 4;

    out[0] = *(f32*)(mark + 0x4);
    out[1] = *(f32*)(mark + 0x8);
    out[2] = *(f32*)(mark + 0xC);
}

void partySetDir(void* party, f32 x, f32 z) {
    *(f32*)((s32)party + 0x100) =
        angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
}

void partyMoveNoHosei(void* party) {
    partyMoveMain(party, 0);
}

void partyMove(void* party) {
    partyMoveMain(party, 1);
}

void N_partyPlaySfxPartyLanding1(void* party) {
    psndSFXOn_3D(0x928, (void*)((s32)party + 0x58));
}

void unk_800cbf84(void* party, f32 value) {
    void* move;
    f32 base;
    f32 mult;

    move = *(void**)((s32)party + 0x160);
    base = value * *(f32*)((s32)move + 0x228);

    if ((*(u32*)((s32)party + 0x0) & 0x80) != 0) {
        mult = float_0p75_80421608;
    } else {
        mult = float_1_804215f8;
    }

    *(f32*)((s32)party + 0x108) = base * mult;
    *(f32*)((s32)party + 0x104) = *(f32*)((s32)party + 0x108);
}

void partyChgMoveMode(void* party, s32 mode) {
    switch (*(u8*)((s32)party + 0x3B)) {
        case 6:
            *(u32*)((s32)party + 0x0) &= ~0x8000;
            break;
        case 4:
        case 5:
        default:
            break;
    }

    *(u8*)((s32)party + 0x3B) = mode;
    *(u32*)((s32)party + 0x8) |= 4;
}

void partyNextReadId(s32 id) {
    if (readId[id] == writeId[id]) {
        return;
    }

    readId[id]++;

    if (readId[id] >= 0x50) {
        readId[id] = 0;
    }
}


u8 partyWalkMain(void* pParty) {
    return 0;
}


u8 partyRecordFootmark(void* pParty) {
    return 0;
}


u8 walkMain(void* pParty) {
    return 0;
}


u8 partyGetMoveDirSpd(s32 pParty, s32 param_2, s32 param_3) {
    return 0;
}


u8 partyMoveFlyInit(void* pParty, s32 param_2) {
    return 0;
}


u8 partyMoveWalk(void* pParty) {
    return 0;
}


void partyFlyMain(void* pParty) {
}


u8 partyForceWalk(void* pParty) {
    return 0;
}


void partyMoveMain(void* party, s32 hosei) {
}


u8 partyWalkInit(void* pParty) {
    return 0;
}


u8 partyMarioHosei(void* pParty) {
    return 0;
}


u8 party_move_mode_stay(void* party) {
    return 0;
}


u8 party_move_mode_walk(void* party) {
    return 0;
}


u8 partyGetTargetDist(void) {
    return 0;
}


u8 partyClearFootmark2(void) {
    return 0;
}


u8 partyGetTargetDist3D(u32 param_1, s32 param_2) {
    return 0;
}


u8 partyGetTargetDir(void) {
    return 0;
}


u8 partyMoveCheckWall(void* pParty) {
    return 0;
}


double partyGetTargetMovespd2(void* pParty) {
    return 0.0;
}


u8 partyMoveBehindMario3(void* pParty, u8 param_2) {
    return 0;
}


double partyGetTargetDistY(void* pParty) {
    return 0.0;
}


u8 partyClearFootmark(void) {
    return 0;
}


u8 party_move_mode_behind_mario(void* pParty) {
    return 0;
}


u8 L_getTargetPos(u32 param_1, u32* param_2) {
    return 0;
}


u8 party_move_mode_beside_mario(void* pParty) {
    return 0;
}


void partyMoveBesideMario(void* pParty, u8 param_2) {
}


u8 partyMoveBehindMario(void* pParty, u8 param_2) {
    return 0;
}


u8 partyChkWall(void* pParty) {
    return 0;
}


void partyChgMot(void* party, s32 mot) {
}
