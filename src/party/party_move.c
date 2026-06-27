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
    f32 mult;
    f32 base;

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
    ;
}


u8 partyForceWalk(void* pParty) {
    return 0;
}


void partyMoveMain(void* party, s32 hosei) {
    ;
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


void party_move_mode_behind_mario(void* pParty) {
    extern void party_motion_behind_mario(void* party);
    extern void party_motion_behind_mario_fly(void* party);
    extern void party_motion_jump2(void* party);
    extern void party_motion_fall(void* party);
    extern void party_motion_upstairs(void* party);
    extern void party_motion_damage(void* party);

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
    }
    switch (*(u8*)((s32)pParty + 0x36)) {
        case 0:
        case 1:
            if (*(u8*)((s32)pParty + 0x32) == 0) {
                party_motion_behind_mario(pParty);
            } else {
                party_motion_behind_mario_fly(pParty);
            }
            break;
        case 2:
            party_motion_jump2(pParty);
            break;
        case 3:
            party_motion_fall(pParty);
            break;
        case 6:
            party_motion_upstairs(pParty);
            break;
        case 7:
            party_motion_damage(pParty);
            break;
        default:
            break;
    }
    *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
}

u8 L_getTargetPos(u32 param_1, u32* param_2) {
    return 0;
}


void party_move_mode_beside_mario(void* pParty) {
    extern void party_motion_beside_mario(void* party);
    extern void party_motion_jump2(void* party);
    extern void party_motion_fall(void* party);
    extern void party_motion_upstairs(void* party);
    extern void party_motion_damage(void* party);

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
    }
    switch (*(u8*)((s32)pParty + 0x36)) {
        case 0:
        case 1:
            party_motion_beside_mario(pParty);
            break;
        case 2:
            party_motion_jump2(pParty);
            break;
        case 3:
            party_motion_fall(pParty);
            break;
        case 6:
            party_motion_upstairs(pParty);
            break;
        case 7:
            party_motion_damage(pParty);
            break;
        default:
            break;
    }
    *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
}

void partyMoveBesideMario(void* pParty, u8 param_2) {
    if (*(s8*)((s32)pParty + 0x31) == 4 && *(u8*)((s32)pParty + 0x34) == 3) {
        return;
    }
    *(u8*)((s32)pParty + 0x3D) = param_2;
    switch (*(u8*)((s32)pParty + 0x3B)) {
        case 6:
            *(u32*)pParty &= ~0x8000;
            break;
        case 4:
        case 5:
        default:
            break;
    }
    *(u8*)((s32)pParty + 0x3B) = 3;
    *(u32*)((s32)pParty + 8) |= 4;
    if (*(u8*)((s32)pParty + 0x36) == 3) {
        *(u32*)pParty &= ~0x70;
        *(u32*)pParty &= ~0x3000000;
        *(u32*)pParty &= ~0x4000000;
    }
    *(u8*)((s32)pParty + 0x36) = 1;
    *(u32*)((s32)pParty + 8) |= 2;
}

void partyMoveBehindMario(void* pParty, u8 param_2) {
    if (*(s8*)((s32)pParty + 0x31) == 4 && *(u8*)((s32)pParty + 0x34) == 3) {
        return;
    }
    *(u8*)((s32)pParty + 0x3D) = param_2;
    switch (*(u8*)((s32)pParty + 0x3B)) {
        case 6:
            *(u32*)pParty &= ~0x8000;
            break;
        case 4:
        case 5:
        default:
            break;
    }
    *(u8*)((s32)pParty + 0x3B) = 2;
    *(u32*)((s32)pParty + 8) |= 4;
    if (*(u8*)((s32)pParty + 0x36) == 3) {
        *(u32*)pParty &= ~0x70;
        *(u32*)pParty &= ~0x3000000;
        *(u32*)pParty &= ~0x4000000;
    }
    *(u8*)((s32)pParty + 0x36) = 1;
    *(u32*)((s32)pParty + 8) |= 2;
}

u8 partyChkWall(void* pParty) {
    return 0;
}


void partyChgMot(void* party, s32 mot) {
    ;
}

void unk_800cbc30(void* party) {
    extern s32 marioChkDeepSleep(void);
    extern s32 chuchu_searchObject(void);
    extern void partyChgPoseId(void* party, s32 poseId);

    if (*(u8*)((s32)party + 0x34) != 0) {
        if (marioChkDeepSleep() == 1 || (*(u32*)party & 0x400000) != 0) {
            partyChgPoseId(party, 6);
        } else if (*(s8*)((s32)party + 0x31) != 7) {
            partyChgPoseId(party, 1);
        } else if (chuchu_searchObject() == 0) {
            partyChgPoseId(party, 1);
        }
    }
}

f32 unk_800cbeb0(void* party) {
    extern f32 float_0p5_80421610;
    extern f32 float_0p05_8042160c;
    extern f32 float_neg1_804215c4;
    extern f32 float_0p1_80421614;
    void* move;
    f32 current;
    f32 target;
    f32 result;

    if (*(s16*)((s32)party + 0x28) != 0 ||
        *(u8*)((s32)party + 0x36) == 2 ||
        *(u8*)((s32)party + 0x36) == 3 ||
        *(u8*)((s32)party + 0x36) == 6) {
        return *(f32*)((s32)party + 0x104);
    }
    move = *(void**)((s32)party + 0x160);
    current = *(f32*)((s32)party + 0x104);
    if ((*(u32*)move & 0x8000) != 0) {
        target = *(f32*)((s32)move + 0x180);
    } else {
        target = *(f32*)((s32)party + 0x108) * *(f32*)((s32)move + 0x228);
        if ((*(u32*)party & 0x80) != 0) {
            target *= float_0p75_80421608;
        } else {
            target *= float_1_804215f8;
        }
    }
    if (target > float_0_804215b8) {
        result = current + float_0p05_8042160c * ((float_0p5_80421610 + target) - current);
        if (result >= target) {
            result = target;
        }
    } else {
        result = current + float_0p1_80421614 * (float_neg1_804215c4 - current);
        if (result <= float_0_804215b8) {
            result = float_0_804215b8;
        }
    }
    *(f32*)((s32)party + 0x104) = result;
    return result;
}

void unk_800cbfbc(void* party, f32 speed) {
    extern f32 float_0p5_80421610;
    extern f32 float_0p05_8042160c;
    extern f32 float_neg1_804215c4;
    extern f32 float_0p1_80421614;
    void* move;
    f32 current;
    f32 target;
    f32 result;

    if (speed == float_0_804215b8) {
        *(f32*)((s32)party + 0x108) = speed;
        return;
    }
    *(f32*)((s32)party + 0x108) = speed;
    if (*(s16*)((s32)party + 0x28) != 0 ||
        *(u8*)((s32)party + 0x36) == 2 ||
        *(u8*)((s32)party + 0x36) == 3 ||
        *(u8*)((s32)party + 0x36) == 6) {
        return;
    }
    move = *(void**)((s32)party + 0x160);
    current = *(f32*)((s32)party + 0x104);
    if ((*(u32*)move & 0x8000) != 0) {
        target = *(f32*)((s32)move + 0x180);
    } else {
        target = *(f32*)((s32)party + 0x108) * *(f32*)((s32)move + 0x228);
        if ((*(u32*)party & 0x80) != 0) {
            target *= float_0p75_80421608;
        } else {
            target *= float_1_804215f8;
        }
    }
    if (target > float_0_804215b8) {
        result = current + float_0p05_8042160c * ((float_0p5_80421610 + target) - current);
        if (result >= target) {
            result = target;
        }
    } else {
        result = current + float_0p1_80421614 * (float_neg1_804215c4 - current);
        if (result <= float_0_804215b8) {
            result = float_0_804215b8;
        }
    }
    *(f32*)((s32)party + 0x104) = result;
}


/* CHATGPT STUB FILL: main/party/party_move 20260624_183901 */

/* stub-fill: flyMain | missing_definition | ghidra_signature */
u8 flyMain(void* pParty) {
    return 0;
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_move 20260624_191429 */

/* fallback stub-fill: map=unk_800ca884 addr=0x800ca884 size=0x000001e8 */
int unk_800ca884() {
    return 0;
}

/* fallback stub-fill: map=unk_800cb9bc addr=0x800cb9bc size=0x000000a4 */
int unk_800cb9bc() {
    return 0;
}

/* fallback stub-fill: map=unk_800cba60 addr=0x800cba60 size=0x000000b0 */
int unk_800cba60() {
    return 0;
}

/* fallback stub-fill: map=unk_800cbb10 addr=0x800cbb10 size=0x00000120 */
int unk_800cbb10() {
    return 0;
}

/* fallback stub-fill: map=unk_800cbcc0 addr=0x800cbcc0 size=0x000001c4 */
int unk_800cbcc0() {
    return 0;
}

/* fallback stub-fill: map=unk_800ccb34 addr=0x800ccb34 size=0x00000160 */
int unk_800ccb34() {
    return 0;
}
