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
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 slot);
    extern s32 marioChkSlitThrouh(void);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void walkMain(void* party);
    extern f32 float_150_80421594;
    void* player;
    void* source;
    u8* mark;
    s32 next;
    s32 grounded;
    s32 slit;
    u32 x;
    u32 y;
    u32 z;

#define RECORD_WALK_MARK(markId) \
    do { \
        mark = footmarkBuf + (markId) * 0x500 + writeId[(markId)] * 0x10; \
        if (((*(u32*)mark & 1) == 0 || grounded) && \
            (*(u32*)(mark + 4) != x || *(u32*)(mark + 8) != y || *(u32*)(mark + 0xC) != z) && \
            readId[(markId)] != writeId[(markId)] + 1) { \
            next = writeId[(markId)] + 1; \
            if (next > 0x4F) next = 0; \
            writeId[(markId)] = next; \
            mark = footmarkBuf + (markId) * 0x500 + next * 0x10; \
            *(u32*)(mark + 4) = x; \
            *(u32*)(mark + 8) = y; \
            *(u32*)(mark + 0xC) = z; \
            if (grounded) *(u32*)mark &= ~1; \
            else *(u32*)mark |= 1; \
            if (slit == 0) *(u32*)mark &= ~4; \
            else *(u32*)mark |= 4; \
        } \
    } while (0)

    player = *(void**)((s32)pParty + 0x160);
    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        source = marioGetPtr();
        grounded = (*(u32*)source & 0x10000) == 0;
        slit = marioChkSlitThrouh();
        x = *(u32*)((s32)player + 0x8C);
        y = *(u32*)((s32)player + 0x90);
        z = *(u32*)((s32)player + 0x94);
        marioGetPtr();
        RECORD_WALK_MARK(0);
    } else {
        source = anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        if (source == 0 ||
            (*(s8*)((s32)source + 0x31) == 3 && *(u8*)((s32)source + 0x34) == 3) ||
            nokonokoGetStatus(source) == 3 ||
            PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)source + 0x58)) >= float_150_80421594) {
            source = marioGetPtr();
            grounded = (*(u32*)source & 0x10000) == 0;
            source = marioGetPtr();
            x = *(u32*)((s32)source + 0x8C);
            y = *(u32*)((s32)source + 0x90);
            z = *(u32*)((s32)source + 0x94);
        } else {
            grounded = (*(u32*)source & 0x10) == 0;
            x = *(u32*)((s32)source + 0x58);
            y = *(u32*)((s32)source + 0x5C);
            z = *(u32*)((s32)source + 0x60);
        }
        slit = 0;
        marioGetPtr();
        RECORD_WALK_MARK(1);
    }
#undef RECORD_WALK_MARK
    walkMain(pParty);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyRecordFootmark(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 marioChkSlitThrouh(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyClearFootmark2(void);
    extern u8 footmarkBuf[];
    extern f32 float_150_80421594;

    void* player;
    void* move;
    void* other;
    u8* mark;
    f32 x;
    f32 y;
    f32 z;
    s32 flags;
    s32 id;
    s32 i;
    s32 useOther;

    move = *(void**)((s32)pParty + 0x160);
    useOther = 0;
    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        player = marioGetPtr();
        flags = 0;
        if ((*(u32*)player & 0x10000) != 0) {
            flags |= 1;
        }
        if (marioChkSlitThrouh() != 0) {
            flags |= 4;
        }
        x = *(f32*)((s32)move + 0x8C);
        y = *(f32*)((s32)move + 0x90);
        z = *(f32*)((s32)move + 0x94);
    } else {
        player = partyGetPtr(marioGetPartyId());
        if ((player != 0) &&
            ((nokonokoGetStatus(player) == 3) ||
             (PSVECDistance((void*)((s32)move + 0x8C), (void*)((s32)player + 0x58)) >= float_150_80421594))) {
            useOther = 1;
        }
        if ((player == 0) || useOther) {
            if (*(s8*)((s32)pParty + 0x51) == 1) {
                *(u8*)((s32)pParty + 0x51) = 0;
                partyClearFootmark2();
            }
            other = marioGetPtr();
            flags = 0;
            if ((*(u32*)other & 0x10000) != 0) {
                flags |= 1;
            }
            if (marioChkSlitThrouh() != 0) {
                flags |= 4;
            }
            x = *(f32*)((s32)move + 0x8C);
            y = *(f32*)((s32)move + 0x90);
            z = *(f32*)((s32)move + 0x94);
        } else {
            if (*(s8*)((s32)pParty + 0x51) == 0) {
                *(u8*)((s32)pParty + 0x51) = 1;
                other = marioGetPtr();
                x = *(f32*)((s32)other + 0x8C);
                y = *(f32*)((s32)other + 0x90);
                z = *(f32*)((s32)other + 0x94);
                mark = footmarkBuf;
                for (i = 0; i < 0x50; i++, mark += 0x10) {
                    *(f32*)(mark + 4) = x;
                    *(f32*)(mark + 8) = y;
                    *(f32*)(mark + 0xC) = z;
                    *mark = 0;
                }
                writeId[0] = 0;
                readId[0] = 0;
            }
            flags = 0;
            if ((*(u32*)player & 0x10) != 0) {
                flags |= 1;
            }
            x = *(f32*)((s32)player + 0x58);
            y = *(f32*)((s32)player + 0x5C);
            z = *(f32*)((s32)player + 0x60);
        }
    }

    id = (*(u32*)pParty >> 2) & 1;
    marioGetPtr();
    mark = footmarkBuf + id * 0x500 + writeId[id] * 0x10;
    if ((((*mark & 1) == 0) || ((flags & 1) == 0)) &&
        ((*(f32*)(mark + 4) != x) || (*(f32*)(mark + 8) != y) || (*(f32*)(mark + 0xC) != z)) &&
        (readId[id] != writeId[id] + 1)) {
        writeId[id]++;
        if (writeId[id] >= 0x50) {
            writeId[id] = 0;
        }
        mark = footmarkBuf + id * 0x500 + writeId[id] * 0x10;
        *(f32*)(mark + 4) = x;
        *(f32*)(mark + 8) = y;
        *(f32*)(mark + 0xC) = z;
        if ((flags & 1) != 0) {
            *mark |= 1;
        } else {
            *mark &= ~1;
        }
        if ((flags & 4) != 0) {
            *mark |= 4;
        } else {
            *mark &= ~4;
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void walkMain(void* pParty) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_JP_US_EU_54_80187d74(void* party);
    extern void unk_JP_US_EU_53_80187d10(void* party);
    extern void party_motion_stay(void* party);
    extern void party_motion_homing_walk(void* party);
    extern void party_motion_homing_fly(void* party);
    extern void party_motion_jump(void* party);
    extern void party_motion_fall(void* party);
    extern void party_motion_upstairs(void* party);
    extern void party_motion_damage(void* party);
    extern void party_motion_behind_mario(void* party);
    extern void party_motion_behind_mario_fly(void* party);
    extern void party_motion_behind_mario_continue(void* party);
    extern void party_motion_jump2(void* party);
    extern void party_motion_beside_mario(void* party);
    extern void marioSetSplash(u32 type, void* pos);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);

    u32 pos[3];
    s32 timer;
    u8 motion;
    void* hit;

    switch (*(u8*)((s32)pParty + 0x3B)) {
        case 0:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
                partyChgPoseId(pParty, 1);
                if (*(s8*)((s32)pParty + 0x31) == 5) {
                    unk_JP_US_EU_54_80187d74(pParty);
                }
            }
            motion = *(u8*)((s32)pParty + 0x36);
            if (motion == 3) {
                party_motion_fall(pParty);
            } else if (motion < 3) {
                if (motion == 0) {
                    party_motion_stay(pParty);
                    if (*(s8*)((s32)pParty + 0x31) == 5) {
                        unk_JP_US_EU_53_80187d10(pParty);
                    }
                } else {
                    if (motion == 3) {
                        *(u32*)pParty &= ~0x70;
                        *(u32*)pParty &= ~0x03000000;
                        *(u32*)pParty &= ~0x04000000;
                    }
                    *(u8*)((s32)pParty + 0x36) = 0;
                    *(u32*)((s32)pParty + 8) |= 2;
                }
            } else if (motion == 6) {
                if (motion == 3) {
                    *(u32*)pParty &= ~0x70;
                    *(u32*)pParty &= ~0x03000000;
                    *(u32*)pParty &= ~0x04000000;
                }
                *(u8*)((s32)pParty + 0x36) = 0;
                *(u32*)((s32)pParty + 8) |= 2;
            }
            break;
        case 1:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            switch (*(u8*)((s32)pParty + 0x36)) {
                case 0:
                    party_motion_stay(pParty);
                    break;
                case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_homing_walk(pParty);
                    } else {
                        party_motion_homing_fly(pParty);
                    }
                    break;
                case 2:
                    party_motion_jump(pParty);
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
            }
            break;
        case 2:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            if (motion == 3) {
                party_motion_fall(pParty);
            } else if (motion < 3) {
                if (motion < 2) {
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                } else {
                    party_motion_jump2(pParty);
                }
            } else if (motion == 7) {
                party_motion_damage(pParty);
            } else if (motion == 6) {
                party_motion_upstairs(pParty);
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 3:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            if (motion == 3) {
                party_motion_fall(pParty);
            } else if (motion < 3) {
                if (motion < 2) {
                    party_motion_beside_mario(pParty);
                } else {
                    party_motion_jump2(pParty);
                }
            } else if (motion == 7) {
                party_motion_damage(pParty);
            } else if (motion == 6) {
                party_motion_upstairs(pParty);
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 6:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            if (motion == 3) {
                party_motion_fall(pParty);
            } else if (motion < 3) {
                if (motion < 2) {
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario_continue(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                } else {
                    party_motion_jump2(pParty);
                }
            } else if (motion == 7) {
                party_motion_damage(pParty);
            } else if (motion == 6) {
                party_motion_upstairs(pParty);
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        default:
            return;
    }

    *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
    if ((*(u32*)((s32)pParty + 8) & 0x01000000) != 0) {
        *(u32*)((s32)pParty + 8) &= ~0x01000000;
        pos[0] = *(u32*)((s32)pParty + 0x58);
        pos[1] = *(u32*)((s32)pParty + 0x5C);
        pos[2] = *(u32*)((s32)pParty + 0x60);
        marioSetSplash(0, pos);
        *(u8*)((s32)pParty + 0x4C) = 0x14;
    }
    timer = *(s8*)((s32)pParty + 0x4C) - 1;
    *(u8*)((s32)pParty + 0x4C) = (u8)timer;
    if ((s8)timer < 0) {
        *(u8*)((s32)pParty + 0x4C) = 0;
    }
    if (*(u8*)((s32)pParty + 0x34) != 0xD) {
        hit = *(void**)((s32)pParty + 0x138);
        if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
            partyChgRunMode(pParty, 0xD);
        } else {
            hit = *(void**)((s32)pParty + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
            } else {
                hit = *(void**)((s32)pParty + 0x140);
                if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                    partyChgRunMode(pParty, 0xD);
                }
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
void partyGetMoveDirSpd(void* pParty, f32* outDir, f32* outSpd) {
    extern s32 partyMarioHosei(void* party);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 sqrtf(f32 x);
    extern u8 footmarkBuf[];
    extern s32 readId[2];
    extern s32 writeId[2];
    extern f32 float_0_804215b8;
    extern f32 float_1_804215f8;
    extern f32 float_0p75_80421608;
    extern f32 float_150_80421594;

    void* player;
    void* source;
    u8* mark;
    f32 x;
    f32 y;
    f32 z;
    f32 s;
    f32 c;
    f32 dx;
    f32 dz;
    f32 mult;
    s32 hosei;
    s32 ok;
    s32 i;

    player = *(void**)((s32)pParty + 0x160);
    hosei = partyMarioHosei(pParty);

    if (hosei != 0 && *(u8*)((s32)pParty + 0x3B) != 2) {
        if ((*(u32*)pParty & 0x8) == 0) {
            player = marioGetPtr();
            x = *(f32*)((s32)player + 0x8C);
            y = *(f32*)((s32)player + 0x90);
            z = *(f32*)((s32)player + 0x94);

            mark = footmarkBuf;
            for (i = 0; i < 80; i++) {
                *(f32*)(mark + 0x4) = x;
                *(f32*)(mark + 0x8) = y;
                *(f32*)(mark + 0xC) = z;
                *(u8*)(mark + 0x0) = 0;
                mark += 0x10;
            }
            writeId[0] = 0;
            readId[0] = 0;
        } else {
            ok = 0;
            source = anotherPartyGetPtr(1);
            if (source != 0) {
                if (nokonokoGetStatus(source) == 3 ||
                    PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)source + 0x58)) >= float_150_80421594) {
                    ok = 1;
                }
            }

            if (ok != 0) {
                player = marioGetPtr();
                source = partyGetPtr(marioGetPartyId());
                if (source == 0) {
                    x = *(f32*)((s32)player + 0x8C);
                    y = *(f32*)((s32)player + 0x90);
                    z = *(f32*)((s32)player + 0x94);
                } else if (nokonokoGetStatus(source) == 3 ||
                           PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)source + 0x58)) >= float_150_80421594) {
                    x = *(f32*)((s32)player + 0x8C);
                    y = *(f32*)((s32)player + 0x90);
                    z = *(f32*)((s32)player + 0x94);
                } else {
                    x = *(f32*)((s32)source + 0x58);
                    y = *(f32*)((s32)source + 0x5C);
                    z = *(f32*)((s32)source + 0x60);
                }

                mark = footmarkBuf + 0x500;
                for (i = 0; i < 80; i++) {
                    *(f32*)(mark + 0x4) = x;
                    *(f32*)(mark + 0x8) = y;
                    *(f32*)(mark + 0xC) = z;
                    *(u8*)(mark + 0x0) = 0;
                    mark += 0x10;
                }
                writeId[1] = 0;
                readId[1] = 0;
            }
        }
    }

    if ((*(u32*)((s32)pParty + 8) & 0x2000) != 0) {
        hosei = 0;
    }
    if ((hosei > 0x1D) &&
        ((*(u8*)((s32)pParty + 0x31) != 4) || (*(u8*)((s32)pParty + 0x34) != 3))) {
        *(s16*)((s32)pParty + 0x52) = 0;
        *(u8*)((s32)pParty + 0x3D) = 0;
        if (*(u8*)((s32)pParty + 0x3B) == 6) {
            *(u32*)pParty &= ~0x8000;
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

    sincosf(*(f32*)((s32)pParty + 0x100), &s, &c);
    dx = (*(f32*)((s32)pParty + 0x104) * s) +
         *(f32*)((s32)pParty + 0xAC) + *(f32*)((s32)pParty + 0xA0);
    dz = (*(f32*)((s32)pParty + 0x104) * c) +
         *(f32*)((s32)pParty + 0xB4) + *(f32*)((s32)pParty + 0xA8);
    *outDir = angleABf(float_0_804215b8, float_0_804215b8, dx, dz);
    mult = float_1_804215f8;
    if ((*(u32*)pParty & 0x800000) != 0) {
        mult = float_0p75_80421608;
    }
    *outSpd = mult * sqrtf(dx * dx + dz * dz);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 partyMoveFlyInit(void* pParty, s32 param_2) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);
    extern f32 float_150_80421594;
    extern f32 float_50_80421590;
    void* player;
    void* partner;
    u8* mark;
    f32 x;
    f32 y;
    f32 z;
    s32 id;
    s32 i;
    u8 state;

    player = marioGetPtr();
    *(u32*)pParty |= 2;
    id = ((*(u32*)pParty >> 2) & 1);
    if ((*(u32*)pParty & 0x100) == 0) {
        x = *(f32*)((s32)player + 0x8C);
        y = *(f32*)((s32)player + 0x90);
        z = *(f32*)((s32)player + 0x94);
    } else {
        partner = partyGetPtr(marioGetPartyId());
        if (partner == 0 || nokonokoGetStatus(partner) == 3 ||
            PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)partner + 0x58)) >= float_150_80421594) {
            x = *(f32*)((s32)player + 0x8C);
            y = *(f32*)((s32)player + 0x90);
            z = *(f32*)((s32)player + 0x94);
        } else {
            x = *(f32*)((s32)partner + 0x58);
            y = *(f32*)((s32)partner + 0x5C);
            z = *(f32*)((s32)partner + 0x60);
        }
    }
    mark = footmarkBuf + id * 0x500;
    for (i = 0; i < 80; i++, mark += 0x10) {
        *(u32*)mark = 0;
        *(f32*)(mark + 4) = x;
        *(f32*)(mark + 8) = y;
        *(f32*)(mark + 0xC) = z;
    }
    writeId[id] = 0;
    readId[id] = 0;
    if (*(u8*)((s32)pParty + 0x3B) == 2) {
        *(f32*)((s32)pParty + 0x40) = float_50_80421590;
        return;
    }
    *(u8*)((s32)pParty + 0x3D) = param_2 == 0 ? 2 : 0;
    *(u8*)((s32)pParty + 0x3B) = 0;
    *(u32*)((s32)pParty + 8) |= 4;
    state = *(u8*)((s32)pParty + 0x3F);
    if (state == 1 || state == 3) {
        *(u8*)((s32)pParty + 0x3F) = 0;
        *(u8*)((s32)pParty + 0x3D) = 0;
        *(u8*)((s32)pParty + 0x3B) = 1;
        *(u32*)((s32)pParty + 8) |= 4;
    } else if (state == 2) {
        *(u8*)((s32)pParty + 0x3D) = 0;
        *(u8*)((s32)pParty + 0x3B) = 0x3C;
        *(u32*)((s32)pParty + 8) |= 4;
    }
    *(u32*)((s32)pParty + 0x44) = 0x10;
    *(f32*)((s32)pParty + 0x40) = float_50_80421590;
    *(u8*)((s32)pParty + 0x3E) = 0;
    partyChgPoseId(pParty, 1);
    *(u8*)((s32)pParty + 0x3B) = 1;
    *(u32*)((s32)pParty + 8) |= 4;
    if (*(u8*)((s32)pParty + 0x36) == 3) {
        *(u32*)pParty &= ~0x70;
        *(u32*)pParty &= ~0x03000000;
        *(u32*)pParty &= ~0x04000000;
    }
    *(u8*)((s32)pParty + 0x36) = 0;
    *(u32*)((s32)pParty + 8) |= 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyMoveWalk(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void partyWalkMain(void* party);
    extern u8 footmarkBuf[];
    extern f32 float_150_80421594;
    extern f32 float_50_80421590;
    extern f32 float_1p8_80421604;
    extern f32 float_0_804215b8;
    extern f32 float_1_804215f8;
    extern f32 float_0p75_80421608;
    extern f32 float_0p1_80421614;
    extern f32 float_neg1_804215c4;
    extern f32 float_0p05_8042160c;
    extern f32 float_0p5_80421610;

    void* player;
    void* party;
    void* move;
    u8* mark;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 target;
    s32 i;

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
        *(u32*)pParty &= ~0x100;
        move = *(void**)((s32)pParty + 0x160);
        *(u32*)pParty |= 2;
        if ((*(u32*)pParty & 8) == 0) {
            player = marioGetPtr();
            x = *(f32*)((s32)player + 0x8C);
            y = *(f32*)((s32)player + 0x90);
            z = *(f32*)((s32)player + 0x94);
            mark = footmarkBuf;
            for (i = 0; i < 0x50; i++, mark += 0x10) {
                *(f32*)(mark + 4) = x;
                *(f32*)(mark + 8) = y;
                *(f32*)(mark + 0xC) = z;
                *mark = 0;
            }
            writeId[0] = 0;
            readId[0] = 0;
        } else {
            player = marioGetPtr();
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                x = *(f32*)((s32)player + 0x8C);
                y = *(f32*)((s32)player + 0x90);
                z = *(f32*)((s32)player + 0x94);
            } else if ((nokonokoGetStatus(party) == 3) ||
                       (PSVECDistance((void*)((s32)player + 0x8C),
                                      (void*)((s32)party + 0x58)) >= float_150_80421594)) {
                x = *(f32*)((s32)player + 0x8C);
                y = *(f32*)((s32)player + 0x90);
                z = *(f32*)((s32)player + 0x94);
            } else {
                x = *(f32*)((s32)party + 0x58);
                y = *(f32*)((s32)party + 0x5C);
                z = *(f32*)((s32)party + 0x60);
            }
            mark = footmarkBuf + 0x500;
            for (i = 0; i < 0x50; i++, mark += 0x10) {
                *(f32*)(mark + 4) = x;
                *(f32*)(mark + 8) = y;
                *(f32*)(mark + 0xC) = z;
                *mark = 0;
            }
            writeId[1] = 0;
            readId[1] = 0;
        }
        *(u8*)((s32)pParty + 0x3D) = 0;
        *(u32*)((s32)pParty + 0x44) = 0x10;
        *(f32*)((s32)pParty + 0x40) = float_50_80421590;
        *(u8*)((s32)pParty + 0x3E) = 0;
        *(f32*)((s32)pParty + 0x118) = float_1p8_80421604;
        *(f32*)((s32)pParty + 0x114) = float_0_804215b8;
        if ((*(u32*)move & 0x400000) == 0) {
            partyChgPoseId(pParty, 1);
            if (*(u8*)((s32)pParty + 0x3B) == 6) {
                *(u32*)pParty &= ~0x8000;
            }
            *(u8*)((s32)pParty + 0x3B) = 1;
            *(u32*)((s32)pParty + 8) |= 4;
            if (*(u8*)((s32)pParty + 0x36) == 3) {
                *(u32*)pParty &= ~0x70;
                *(u32*)pParty &= ~0x3000000;
                *(u32*)pParty &= ~0x4000000;
            }
            *(u8*)((s32)pParty + 0x36) = 0;
            *(u32*)((s32)pParty + 8) |= 2;
        }
    }

    if ((*(s16*)((s32)pParty + 0x28) == 0) &&
        (*(u8*)((s32)pParty + 0x36) != 2) &&
        (*(u8*)((s32)pParty + 0x36) != 3) &&
        (*(u8*)((s32)pParty + 0x36) != 6)) {
        move = *(void**)((s32)pParty + 0x160);
        speed = *(f32*)((s32)pParty + 0x104);
        if ((*(u32*)move & 0x200000) == 0) {
            target = float_1_804215f8;
            if ((*(u32*)pParty & 0x800000) != 0) {
                target = float_0p75_80421608;
            }
            target = *(f32*)((s32)pParty + 0x108) * *(f32*)((s32)move + 0x228) * target;
        } else {
            target = *(f32*)((s32)move + 0x180);
        }
        if (target <= float_0_804215b8) {
            speed = speed + float_0p1_80421614 * (float_neg1_804215c4 - speed);
            if (speed <= float_0_804215b8) {
                speed = float_0_804215b8;
            }
        } else {
            speed = speed + float_0p05_8042160c * ((float_0p5_80421610 + target) - speed);
            if (target <= speed) {
                speed = target;
            }
        }
        *(f32*)((s32)pParty + 0x104) = speed;
    }
    partyWalkMain(pParty);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void partyFlyMain(void* pParty) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 slot);
    extern void* partyGetPtr(s32 id);
    extern void flyMain(void* party);
    void* source;
    u8* mark;
    s32 next;
    s32 grounded;
    f32 x;
    f32 y;
    f32 z;

#define RECORD_FLY_MARK(markId) \
    do { \
        mark = footmarkBuf + (markId) * 0x500 + writeId[(markId)] * 0x10; \
        if (((*(u32*)mark & 1) == 0 || grounded) && \
            (*(f32*)(mark + 4) != x || *(f32*)(mark + 8) != y || *(f32*)(mark + 0xC) != z) && \
            readId[(markId)] != writeId[(markId)] + 1) { \
            next = writeId[(markId)] + 1; \
            if (next > 0x4F) next = 0; \
            writeId[(markId)] = next; \
            mark = footmarkBuf + (markId) * 0x500 + next * 0x10; \
            *(f32*)(mark + 4) = x; \
            *(f32*)(mark + 8) = y; \
            *(f32*)(mark + 0xC) = z; \
            if (grounded) *(u32*)mark &= ~1; \
            else *(u32*)mark |= 1; \
            *(u32*)mark &= ~4; \
        } \
    } while (0)

    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        source = marioGetPtr();
        grounded = (*(u32*)source & 0x10) == 0;
        source = marioGetPtr();
        x = *(f32*)((s32)source + 0x8C);
        y = *(f32*)((s32)source + 0x90);
        z = *(f32*)((s32)source + 0x94);
        marioGetPtr();
        RECORD_FLY_MARK(0);
    } else {
        anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        source = partyGetPtr(0);
        if (source == 0) {
            source = marioGetPtr();
            grounded = (*(u32*)source & 0x10) == 0;
            source = marioGetPtr();
            x = *(f32*)((s32)source + 0x8C);
            y = *(f32*)((s32)source + 0x90);
            z = *(f32*)((s32)source + 0x94);
        } else {
            grounded = (*(u32*)source & 0x10) == 0;
            x = *(f32*)((s32)source + 0x58);
            y = *(f32*)((s32)source + 0x5C);
            z = *(f32*)((s32)source + 0x60);
        }
        marioGetPtr();
        RECORD_FLY_MARK(1);
    }
#undef RECORD_FLY_MARK
    flyMain(pParty);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyForceWalk(void* pParty) {
    extern void bomhei_use_cancel(void);
    extern void nokonoko_finish(void* party);
    extern void unk_JP_US_EU_12_800bc660(void* party);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_JP_US_EU_18_800cbcc0(void* party, s32 arg);
    extern s32 N_marioGetMovementSpeed(void);
    extern s32 partySearchFrontWall(f32 distance, f32 angle, void* party, f32* pos);
    extern void* partySearchGround(f32 height, f32 y, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void party_motion_jump(void* party);
    extern void party_motion_fall(void* party);
    extern void vivianShadow(void* party);
    extern f32 float_0_804215b8;
    extern f32 float_1_804215f8;
    extern f32 float_1p65_804215f4;
    extern f32 float_2_804215cc;
    extern f32 float_0p8_804215fc;
    extern f32 float_10_80421600;
    extern f32 float_neg1_804215c4;

    f32 pos[3];
    f32 speed;
    f32 upper;
    f32 lower;
    s16 timer;
    void* hit;
    u32 attr;
    u8 motion;

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
        if ((*(u32*)pParty & 0x100) != 0) {
            if (*(u8*)((s32)pParty + 0x31) == 3) {
                bomhei_use_cancel();
            }
            if (*(u8*)((s32)pParty + 0x31) == 2) {
                nokonoko_finish(pParty);
            }
            *(u32*)pParty &= ~0x100;
        }
    }

    motion = *(u8*)((s32)pParty + 0x36);
    if (motion == 2) {
        timer = *(s16*)((s32)pParty + 0x28) - 1;
        *(s16*)((s32)pParty + 0x28) = timer;
        if (timer < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
        }
        party_motion_jump(pParty);
    } else if (motion < 2) {
        if (*(s16*)((s32)pParty + 0x28) < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
            unk_JP_US_EU_12_800bc660(pParty);
        } else {
            speed = *(f32*)((s32)pParty + 0x104);
            if ((*(u32*)((s32)pParty + 4) & 0x100) != 0) {
                partyChgPoseId(pParty, 3);
                unk_JP_US_EU_18_800cbcc0(pParty, 1);
            } else {
                if (*(f32*)(*(s32*)((s32)pParty + 0x160) + 0x228) == float_1_804215f8) {
                    upper = float_1p65_804215f4;
                    lower = float_0p8_804215fc;
                } else {
                    upper = float_2_804215cc;
                    lower = float_0_804215b8;
                }
                if ((upper - lower < speed) && (speed < speed + lower)) {
                    if (N_marioGetMovementSpeed() == 1) {
                        partyChgPoseId(pParty, 2);
                        unk_JP_US_EU_18_800cbcc0(pParty, 0);
                    } else {
                        partyChgPoseId(pParty, 3);
                        unk_JP_US_EU_18_800cbcc0(pParty, 1);
                    }
                } else if (speed >= upper) {
                    partyChgPoseId(pParty, 3);
                    unk_JP_US_EU_18_800cbcc0(pParty, 1);
                } else {
                    partyChgPoseId(pParty, 2);
                    unk_JP_US_EU_18_800cbcc0(pParty, 0);
                }
            }

            pos[0] = *(f32*)((s32)pParty + 0x58);
            pos[1] = *(f32*)((s32)pParty + 0x5C);
            pos[2] = *(f32*)((s32)pParty + 0x60);
            if (partySearchFrontWall(*(f32*)((s32)pParty + 0x104),
                                     *(f32*)((s32)pParty + 0x100), pParty, pos) == 0) {
                *(f32*)((s32)pParty + 0x58) = pos[0];
                *(f32*)((s32)pParty + 0x60) = pos[2];
            } else {
                if (*(u8*)((s32)pParty + 0x36) == 3) {
                    *(u32*)pParty &= ~0x70;
                    *(u32*)pParty &= ~0x3000000;
                    *(u32*)pParty &= ~0x4000000;
                }
                *(u8*)((s32)pParty + 0x36) = 2;
                *(u32*)((s32)pParty + 8) |= 2;
            }

            timer = *(s16*)((s32)pParty + 0x28) - 1;
            *(s16*)((s32)pParty + 0x28) = timer;
            if (timer < 1) {
                *(s16*)((s32)pParty + 0x28) = 0;
                unk_JP_US_EU_12_800bc660(pParty);
            } else {
                *(u32*)pParty &= ~0x800000;
                hit = partySearchGround(float_10_80421600, float_10_80421600, pParty);
                attr = 0;
                if (hit != 0) {
                    attr = hitGetAttr(hit);
                    if ((attr & 0x200) != 0) {
                        hit = 0;
                    }
                }
                *(void**)((s32)pParty + 0x12C) = hit;
                if (hit == 0) {
                    if (*(u8*)((s32)pParty + 0x36) == 3) {
                        *(u32*)pParty &= ~0x70;
                        *(u32*)pParty &= ~0x3000000;
                        *(u32*)pParty &= ~0x4000000;
                    }
                    *(u8*)((s32)pParty + 0x36) = 3;
                    *(u32*)((s32)pParty + 8) |= 2;
                    *(f32*)((s32)pParty + 0x118) = float_neg1_804215c4;
                } else if ((attr & 0x100) == 0) {
                    *(u32*)pParty &= ~0x800000;
                } else {
                    *(u32*)pParty |= 0x800000;
                }
                *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
            }
        }
    } else if (motion < 4) {
        timer = *(s16*)((s32)pParty + 0x28) - 1;
        *(s16*)((s32)pParty + 0x28) = timer;
        if (timer < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
        }
        party_motion_fall(pParty);
    }

    *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
    if (*(u8*)((s32)pParty + 0x31) == 5) {
        vivianShadow(pParty);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyMoveMain(void* party, s32 hosei) {
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* spd);
    extern void* partySearchFrontWall(f32 spd, f32 dir, void* party, f32* pos);
    extern void unk_800c27c0(void* party, f32* pos, f32* dist, f32 width, f32 height);
    extern void movePos(f32* x, f32* z, f32 spd, f32 dir);
    extern void* partySearchHead(f32 angle, void* party, f32* pos, f32* dist);
    extern f32 toMovedirSimple(f32 angle);
    extern void* partySearchGround(f32 hi, f32 lo, void* party);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_0_804215b8;
    extern f32 float_0p75_80421608;
    extern f32 float_1_804215f8;
    extern f32 float_neg1p4_80421630;

    f32 pos[3];
    f32 headPos[3];
    f32 headDist;
    f32 dir;
    f32 spd;
    void* hit;

    *(void**)((s32)party + 0x12C) = 0;
    *(void**)((s32)party + 0x134) = 0;
    pos[0] = *(f32*)((s32)party + 0x58);
    pos[1] = *(f32*)((s32)party + 0x5C);
    pos[2] = *(f32*)((s32)party + 0x60);
    spd = float_0_804215b8;

    if (hosei != 0) {
        partyGetMoveDirSpd(party, &dir, &spd);
    } else {
        dir = *(f32*)((s32)party + 0x100);
        spd = *(f32*)((s32)party + 0x104);
        if ((*(u32*)party & 0x800000) != 0) {
            spd *= float_0p75_80421608;
        } else {
            spd *= float_1_804215f8;
        }
    }

    if ((*(u32*)party & 0x1000000) == 0) {
        hit = partySearchFrontWall(spd, dir, party, pos);
        if (hit != 0) {
            *(void**)((s32)party + 0x12C) = hit;
            if (*(f32*)((s32)party + 0x104) != float_0_804215b8) {
                *(void**)((s32)party + 0x134) = hit;
            }
        }
        if (hit != 0) {
            *(f32*)((s32)party + 0x58) = pos[0];
            *(f32*)((s32)party + 0x60) = pos[2];
        } else {
            unk_800c27c0(party, pos, &spd, *(f32*)((s32)party + 0xF4), *(f32*)((s32)party + 0xF0));
            *(f32*)((s32)party + 0x58) = pos[0];
            *(f32*)((s32)party + 0x60) = pos[2];
        }
    } else {
        movePos((f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60), spd, dir);
    }

    if ((*(u32*)party & 0x2000000) == 0) {
        headPos[0] = *(f32*)((s32)party + 0x58);
        headPos[1] = *(f32*)((s32)party + 0x5C);
        headPos[2] = *(f32*)((s32)party + 0x60);
        headDist = *(f32*)((s32)party + 0xF0);
        hit = partySearchHead(*(f32*)((s32)party + 0x10C), party, headPos, &headDist);
        if (hit != 0) {
            *(void**)((s32)party + 0x140) = hit;
        }
    }

    if ((*(u32*)party & 0x10) != 0) {
        dir = toMovedirSimple(*(f32*)((s32)party + 0x110));
        if (*(void**)((s32)party + 0x140) == 0) {
            *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
        }
        *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x118);
        *(u32*)party &= ~3;
    } else {
        hit = 0;
        if (((*(u32*)party & 0x2000000) == 0) && (*(f32*)((s32)party + 0x114) <= float_0_804215b8)) {
            hit = partySearchGround(*(f32*)((s32)party + 0x114), *(f32*)((s32)party + 0x114), party);
            pos[1] = *(f32*)((s32)party + 0xE4);
            if (hit != 0) {
                if ((hitGetAttr(hit) & 0x200) != 0) {
                    *(void**)((s32)party + 0x138) = hit;
                    *(u32*)party &= ~3;
                    if ((*(u32*)party & 0x20) == 0) {
                        *(u32*)party |= 0x20;
                        if (*(f32*)((s32)party + 0x118) == float_0_804215b8) {
                            *(f32*)((s32)party + 0x118) = float_neg1p4_80421630;
                        }
                    }
                    *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
                    *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x118);
                    return;
                }
                if (*(f32*)((s32)party + 0xE8) == float_0_804215b8) {
                    *(f32*)((s32)party + 0x5C) = pos[1];
                } else {
                    hit = 0;
                }
            }
        }
        *(void**)((s32)party + 0x138) = hit;
        if (hit == 0) {
            if ((*(u32*)party & 0x4000000) == 0) {
                *(u32*)party &= ~3;
                if ((*(u32*)party & 0x20) == 0) {
                    *(u32*)party |= 0x20;
                    if (*(f32*)((s32)party + 0x118) == float_0_804215b8) {
                        *(f32*)((s32)party + 0x118) = float_neg1p4_80421630;
                    }
                }
                *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
                *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x118);
            }
        } else if ((*(u32*)party & 0x20) != 0) {
            *(u32*)party &= ~0x30;
            *(u32*)party |= 2;
            *(f32*)((s32)party + 0x114) = float_0_804215b8;
            *(f32*)((s32)party + 0x118) = float_0_804215b8;
            *(f32*)((s32)party + 0x5C) = pos[1];
        } else {
            *(f32*)((s32)party + 0x5C) = pos[1];
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyWalkInit(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);
    extern u8 footmarkBuf[];
    extern f32 float_150_80421594;
    extern f32 float_50_80421590;
    extern f32 float_1p8_80421604;
    extern f32 float_0_804215b8;

    void* player;
    void* move;
    void* other;
    u8* mark;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    move = *(void**)((s32)pParty + 0x160);
    *(u32*)pParty |= 2;
    if ((*(u32*)pParty & 8) == 0) {
        player = marioGetPtr();
        x = *(f32*)((s32)player + 0x8C);
        y = *(f32*)((s32)player + 0x90);
        z = *(f32*)((s32)player + 0x94);
        mark = footmarkBuf;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(f32*)(mark + 0x04) = x; *(f32*)(mark + 0x08) = y; *(f32*)(mark + 0x0C) = z; *(u8*)(mark + 0x00) = 0;
            *(f32*)(mark + 0x14) = x; *(f32*)(mark + 0x18) = y; *(f32*)(mark + 0x1C) = z; *(u8*)(mark + 0x10) = 0;
            *(f32*)(mark + 0x24) = x; *(f32*)(mark + 0x28) = y; *(f32*)(mark + 0x2C) = z; *(u8*)(mark + 0x20) = 0;
            *(f32*)(mark + 0x34) = x; *(f32*)(mark + 0x38) = y; *(f32*)(mark + 0x3C) = z; *(u8*)(mark + 0x30) = 0;
            *(f32*)(mark + 0x44) = x; *(f32*)(mark + 0x48) = y; *(f32*)(mark + 0x4C) = z; *(u8*)(mark + 0x40) = 0;
            *(f32*)(mark + 0x54) = x; *(f32*)(mark + 0x58) = y; *(f32*)(mark + 0x5C) = z; *(u8*)(mark + 0x50) = 0;
            *(f32*)(mark + 0x64) = x; *(f32*)(mark + 0x68) = y; *(f32*)(mark + 0x6C) = z; *(u8*)(mark + 0x60) = 0;
            *(f32*)(mark + 0x74) = x; *(f32*)(mark + 0x78) = y; *(f32*)(mark + 0x7C) = z; *(u8*)(mark + 0x70) = 0;
        }
        writeId[0] = 0;
        readId[0] = 0;
    } else {
        player = marioGetPtr();
        other = partyGetPtr(marioGetPartyId());
        if (other == 0) {
            x = *(f32*)((s32)player + 0x8C);
            y = *(f32*)((s32)player + 0x90);
            z = *(f32*)((s32)player + 0x94);
        } else if ((nokonokoGetStatus(other) == 3) ||
                   (PSVECDistance((void*)((s32)player + 0x8C),
                                  (void*)((s32)other + 0x58)) >= float_150_80421594)) {
            x = *(f32*)((s32)player + 0x8C);
            y = *(f32*)((s32)player + 0x90);
            z = *(f32*)((s32)player + 0x94);
        } else {
            x = *(f32*)((s32)other + 0x58);
            y = *(f32*)((s32)other + 0x5C);
            z = *(f32*)((s32)other + 0x60);
        }
        mark = footmarkBuf + 0x500;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(f32*)(mark + 0x04) = x; *(f32*)(mark + 0x08) = y; *(f32*)(mark + 0x0C) = z; *(u8*)(mark + 0x00) = 0;
            *(f32*)(mark + 0x14) = x; *(f32*)(mark + 0x18) = y; *(f32*)(mark + 0x1C) = z; *(u8*)(mark + 0x10) = 0;
            *(f32*)(mark + 0x24) = x; *(f32*)(mark + 0x28) = y; *(f32*)(mark + 0x2C) = z; *(u8*)(mark + 0x20) = 0;
            *(f32*)(mark + 0x34) = x; *(f32*)(mark + 0x38) = y; *(f32*)(mark + 0x3C) = z; *(u8*)(mark + 0x30) = 0;
            *(f32*)(mark + 0x44) = x; *(f32*)(mark + 0x48) = y; *(f32*)(mark + 0x4C) = z; *(u8*)(mark + 0x40) = 0;
            *(f32*)(mark + 0x54) = x; *(f32*)(mark + 0x58) = y; *(f32*)(mark + 0x5C) = z; *(u8*)(mark + 0x50) = 0;
            *(f32*)(mark + 0x64) = x; *(f32*)(mark + 0x68) = y; *(f32*)(mark + 0x6C) = z; *(u8*)(mark + 0x60) = 0;
            *(f32*)(mark + 0x74) = x; *(f32*)(mark + 0x78) = y; *(f32*)(mark + 0x7C) = z; *(u8*)(mark + 0x70) = 0;
        }
        writeId[1] = 0;
        readId[1] = 0;
    }

    *(u8*)((s32)pParty + 0x3D) = 0;
    *(u32*)((s32)pParty + 0x44) = 0x10;
    *(f32*)((s32)pParty + 0x40) = float_50_80421590;
    *(u8*)((s32)pParty + 0x3E) = 0;
    *(f32*)((s32)pParty + 0x118) = float_1p8_80421604;
    *(f32*)((s32)pParty + 0x114) = float_0_804215b8;
    if ((*(u32*)move & 0x400000) == 0) {
        partyChgPoseId(pParty, 1);
        if (*(u8*)((s32)pParty + 0x3B) == 6) {
            *(u32*)pParty &= ~0x8000;
        }
        *(u8*)((s32)pParty + 0x3B) = 1;
        *(u32*)((s32)pParty + 8) |= 4;
        if (*(u8*)((s32)pParty + 0x36) == 3) {
            *(u32*)pParty &= ~0x70;
            *(u32*)pParty &= ~0x3000000;
            *(u32*)pParty &= ~0x4000000;
        }
        *(u8*)((s32)pParty + 0x36) = 0;
        *(u32*)((s32)pParty + 8) |= 2;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

int partyMarioHosei(void* pParty) {
    extern f32 __fabsf(f32 value);
    extern double sin(double x);
    extern double cos(double x);

    void* player;
    f32 xFactor;
    f32 zFactor;
    f32 yFactor;
    f32 widthSum;
    f32 heightSum;
    f32 partyX;
    f32 partyY;
    f32 partyZ;
    f32 playerX;
    f32 playerY;
    f32 playerZ;
    f32 angle;
    f32 speed;
    s16 count;

    player = *(void**)((int)pParty + 0x160);
    if ((*(unsigned int*)((int)pParty + 8) & 0x40000) == 0) {
        xFactor = 0.4f;
        zFactor = 0.3f;
        yFactor = xFactor;
    } else {
        xFactor = 0.2f;
        zFactor = 0.15f;
        yFactor = xFactor;
    }
    if ((*(unsigned int*)player & 0x8000) != 0) {
        xFactor = 0.2f;
        zFactor = 0.15f;
        yFactor = xFactor;
    }

    widthSum = *(f32*)((int)pParty + 0xF4) + *(f32*)((int)player + 0x1B8);
    heightSum = *(f32*)((int)pParty + 0xF0) + *(f32*)((int)player + 0x1BC);
    *(f32*)((int)pParty + 0xA0) = 0.0f;
    *(f32*)((int)pParty + 0xA8) = 0.0f;

    partyX = *(f32*)((int)pParty + 0x58);
    partyY = *(f32*)((int)pParty + 0x5C);
    partyZ = *(f32*)((int)pParty + 0x60);
    playerX = *(f32*)((int)player + 0x8C);
    playerY = *(f32*)((int)player + 0x90);
    playerZ = *(f32*)((int)player + 0x94);

    if (__fabsf(partyX - playerX) <= xFactor * widthSum &&
        __fabsf(partyZ - playerZ) <= zFactor * widthSum &&
        __fabsf(partyY - playerY) <= yFactor * heightSum) {
        angle = angleABf(playerX, playerZ, partyX, partyZ);
        if (*(f32*)((int)player + 0x180) == 0.0f) {
            angle = angleABf(playerX, playerZ, partyX, partyZ);
            speed = 4.0f;
        } else {
            angle = angleABf(playerX, playerZ, partyX, partyZ);
            if (__fabsf(angle - *(f32*)((int)player + 0x1A4)) <= 4.0f) {
                if (angle - *(f32*)((int)player + 0x1A4) < 0.0f) {
                    angle -= 90.0f;
                } else {
                    angle += 90.0f;
                }
            }
            count = *(s16*)((int)pParty + 0x52) + 1;
            *(s16*)((int)pParty + 0x52) = count;
            if (count > 100) {
                *(s16*)((int)pParty + 0x52) = 100;
            }
            speed = 1.0f;
        }
        if (*(f32*)((int)player + 0x180) > 1.0f) {
            speed = 0.8f * *(f32*)((int)player + 0x180);
        }
        angle = (3.1416f * angle) / 180.0f;
        *(f32*)((int)pParty + 0xA0) = speed * (f32)sin(angle);
        *(f32*)((int)pParty + 0xA8) = speed * -(f32)cos(angle);
    } else if (*(s16*)((int)pParty + 0x52) > 0) {
        *(s16*)((int)pParty + 0x52) = *(s16*)((int)pParty + 0x52) - 1;
    }

    return *(s16*)((int)pParty + 0x52);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_move_mode_stay(void* party) {
    extern void partyChgPoseId(void* party, int pose);
    extern void unk_JP_US_EU_54_80187d74(void* party);
    extern void party_motion_stay(void* party);
    extern void unk_JP_US_EU_53_80187d10(void* party);
    extern void party_motion_fall(void* party);
    extern void marioSetSplash(unsigned int type, void* pos);
    extern unsigned int hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, int mode);

    unsigned int pos[3];
    unsigned int posX;
    int timer;
    int motion;
    void* hit;

    if ((*(unsigned int*)((int)party + 8) & 4) != 0) {
        *(unsigned int*)((int)party + 8) &= ~4;
        partyChgPoseId(party, 1);
        if (*(signed char*)((int)party + 0x31) == 5) {
            unk_JP_US_EU_54_80187d74(party);
        }
    }

    motion = *(unsigned char*)((int)party + 0x36);
    if (motion == 3) {
        goto fall;
    }
    if (motion >= 3) {
        if (motion == 6) {
            goto reset_motion;
        }
        goto tail;
    }
    if (motion == 0) {
        party_motion_stay(party);
        if (*(signed char*)((int)party + 0x31) == 5) {
            unk_JP_US_EU_53_80187d10(party);
        }
        goto tail;
    }
    if (motion >= 0) {
        goto reset_motion;
    }
    goto tail;

reset_motion:
    if ((unsigned char)motion == 3) {
        *(unsigned int*)party &= ~0x70;
        *(unsigned int*)party &= ~0x03000000;
        *(unsigned int*)party &= ~0x04000000;
    }
    *(unsigned char*)((int)party + 0x36) = 0;
    *(unsigned int*)((int)party + 8) |= 2;
    goto tail;

fall:
    party_motion_fall(party);

tail:
    *(float*)((int)party + 0xFC) = *(float*)((int)party + 0x100);

    if ((*(unsigned int*)((int)party + 8) & 0x01000000) != 0) {
        *(unsigned int*)((int)party + 8) &= ~0x01000000;
        posX = *(unsigned int*)((int)party + 0x58);
        pos[0] = posX;
        pos[1] = *(unsigned int*)((int)party + 0x5C);
        pos[2] = *(unsigned int*)((int)party + 0x60);
        marioSetSplash(0, pos);
        *(unsigned char*)((int)party + 0x4C) = 0x14;
    }

    timer = *(unsigned char*)((int)party + 0x4C) - 1;
    *(unsigned char*)((int)party + 0x4C) = timer;
    if ((signed char)timer < 0) {
        *(unsigned char*)((int)party + 0x4C) = 0;
    }

    if (*(unsigned char*)((int)party + 0x34) != 0xD) {
        hit = *(void**)((int)party + 0x138);
        if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
            partyChgRunMode(party, 0xD);
        } else {
            hit = *(void**)((int)party + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(party, 0xD);
            } else {
                hit = *(void**)((int)party + 0x140);
                if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                    partyChgRunMode(party, 0xD);
                }
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_move_mode_walk(void* party) {
    extern void party_motion_stay(void* party);
    extern void party_motion_homing_walk(void* party);
    extern void party_motion_homing_fly(void* party);
    extern void party_motion_jump(void* party);
    extern void party_motion_fall(void* party);
    extern void party_motion_upstairs(void* party);
    extern void party_motion_damage(void* party);
    extern void marioSetSplash(unsigned int type, void* pos);
    extern unsigned int hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, int mode);

    unsigned int pos[3];
    unsigned int posX;
    int timer;
    void* hit;

    if ((*(unsigned int*)((int)party + 8) & 4) != 0) {
        *(unsigned int*)((int)party + 8) &= ~4;
    }

    switch (*(unsigned char*)((int)party + 0x36)) {
        case 0:
            party_motion_stay(party);
            break;
        case 1:
            if (*(unsigned char*)((int)party + 0x32) == 0) {
                party_motion_homing_walk(party);
            } else {
                party_motion_homing_fly(party);
            }
            break;
        case 2:
            party_motion_jump(party);
            break;
        case 3:
            party_motion_fall(party);
            break;
        case 6:
            party_motion_upstairs(party);
            break;
        case 7:
            party_motion_damage(party);
            break;
    }

    *(float*)((int)party + 0xFC) = *(float*)((int)party + 0x100);

    if ((*(unsigned int*)((int)party + 8) & 0x01000000) != 0) {
        *(unsigned int*)((int)party + 8) &= ~0x01000000;
        posX = *(unsigned int*)((int)party + 0x58);
        pos[0] = posX;
        pos[1] = *(unsigned int*)((int)party + 0x5C);
        pos[2] = *(unsigned int*)((int)party + 0x60);
        marioSetSplash(0, pos);
        *(unsigned char*)((int)party + 0x4C) = 0x14;
    }

    timer = *(unsigned char*)((int)party + 0x4C) - 1;
    *(unsigned char*)((int)party + 0x4C) = timer;
    if ((signed char)timer < 0) {
        *(unsigned char*)((int)party + 0x4C) = 0;
    }

    if (*(unsigned char*)((int)party + 0x34) != 0xD) {
        hit = *(void**)((int)party + 0x138);
        if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
            partyChgRunMode(party, 0xD);
        } else {
            hit = *(void**)((int)party + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(party, 0xD);
            } else {
                hit = *(void**)((int)party + 0x140);
                if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                    partyChgRunMode(party, 0xD);
                }
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

float partyGetTargetDist(float x, float z, int useParty) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(int id);
    extern float distABf(float x1, float z1, float x2, float z2);
    extern int nokonokoGetStatus(void* party);
    extern float PSVECDistance(void* a, void* b);

    void* player;
    void* party;
    int invalid;
    float targetX;
    float targetZ;

    player = marioGetPtr();
    invalid = 0;
    party = anotherPartyGetPtr(((unsigned int)(-useParty) | (unsigned int)useParty) >> 31);
    if (useParty == 0) {
        return distABf(x, z, *(float*)((int)player + 0x8C), *(float*)((int)player + 0x94));
    }

    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 || PSVECDistance((void*)((int)player + 0x8C), (void*)((int)party + 0x58)) >= 150.0f) {
            invalid = 1;
        }
    }
    if (party == 0 || invalid != 0) {
        return distABf(x, z, *(float*)((int)player + 0x8C), *(float*)((int)player + 0x94));
    }

    player = marioGetPtr();
    invalid = 0;
    player = marioGetPtr();
    party = anotherPartyGetPtr(((unsigned int)(-useParty) | (unsigned int)useParty) >> 31);
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 || PSVECDistance((void*)((int)player + 0x8C), (void*)((int)party + 0x58)) >= 150.0f) {
            invalid = 1;
        }
    }
    if (invalid != 0) {
        party = 0;
    }
    if (party != 0) {
        targetX = *(float*)((int)party + 0x58);
        targetZ = *(float*)((int)party + 0x60);
    } else {
        targetX = *(float*)((int)player + 0x8C);
        targetZ = *(float*)((int)player + 0x94);
    }
    return distABf(x, z, targetX, targetZ);
}

u8 partyClearFootmark2(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 float_150_80421594;
    void* mario = marioGetPtr();
    void* party = partyGetPtr(marioGetPartyId());
    u32 x;
    u32 y;
    u32 z;
    u8* mark;
    s32 i;

    if (party != 0 &&
        nokonokoGetStatus(party) != 3 &&
        PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) < float_150_80421594) {
        x = *(u32*)((s32)party + 0x58);
        y = *(u32*)((s32)party + 0x5C);
        z = *(u32*)((s32)party + 0x60);
    } else {
        x = *(u32*)((s32)mario + 0x8C);
        y = *(u32*)((s32)mario + 0x90);
        z = *(u32*)((s32)mario + 0x94);
    }
    mark = footmarkBuf + 0x500;
    for (i = 0; i < 0x50; i++) {
        *(u32*)(mark + 4) = x;
        *(u32*)(mark + 8) = y;
        *(u32*)(mark + 0xC) = z;
        *(u8*)mark = 0;
        mark += 0x10;
    }
    writeId[1] = 0;
    readId[1] = 0;
}

f32 partyGetTargetDist3D(s32 useParty, void* pos) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 float_150_80421594;
    void* mario;
    void* mario2;
    void* party;
    u32 target[3];
    s32 invalid;

    mario = marioGetPtr();
    party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
    if (useParty == 0) {
        return PSVECDistance((void*)((s32)marioGetPtr() + 0x8C), pos);
    }
    invalid = 0;
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
        mario2 = marioGetPtr();
        invalid = 0;
        mario = marioGetPtr();
        party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
        if (party != 0) {
            if (nokonokoGetStatus(party) == 3 ||
                PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
                invalid = 1;
            }
        }
        if (invalid != 0) {
            party = 0;
        }
        if (party != 0) {
            target[0] = *(u32*)((s32)party + 0x58);
            target[1] = *(u32*)((s32)party + 0x5C);
            target[2] = *(u32*)((s32)party + 0x60);
        } else {
            target[0] = *(u32*)((s32)mario2 + 0x8C);
            target[1] = *(u32*)((s32)mario2 + 0x90);
            target[2] = *(u32*)((s32)mario2 + 0x94);
        }
        return PSVECDistance(target, pos);
    }
    return PSVECDistance((void*)((s32)marioGetPtr() + 0x8C), pos);
}

f32 partyGetTargetDir(s32 useParty, f32 x, f32 z) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 float_150_80421594;
    void* mario;
    void* player;
    void* party;
    f32 targetX;
    f32 targetZ;
    s32 invalid;

    player = marioGetPtr();
    invalid = 0;
    party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
        mario = marioGetPtr();
        invalid = 0;
        player = marioGetPtr();
        party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
        if (party != 0) {
            if (nokonokoGetStatus(party) == 3 ||
                PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
                invalid = 1;
            }
        }
        if (invalid != 0) {
            party = 0;
        }
        if (party != 0) {
            targetX = *(f32*)((s32)party + 0x58);
            targetZ = *(f32*)((s32)party + 0x60);
        } else {
            targetX = *(f32*)((s32)mario + 0x8C);
            targetZ = *(f32*)((s32)mario + 0x94);
        }
        return angleABf(x, z, targetX, targetZ);
    }
    return angleABf(x, z, *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
}

u8 partyMoveCheckWall(void* pParty) {
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* spd);
    extern void* partySearchFrontWall(f32 dir, f32 spd, void* party, void* pos);
    extern void* partySearchGround(f32 height, f32 y, void* party);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_0_804215b8;
    extern f32 float_10_80421600;
    extern f32 float_neg1p4_80421630;
    u32 pos[3];
    f32 spd;
    f32 dir;
    void* hit;

    pos[0] = *(u32*)((s32)pParty + 0x58);
    pos[1] = *(u32*)((s32)pParty + 0x5C);
    pos[2] = *(u32*)((s32)pParty + 0x60);
    partyGetMoveDirSpd(pParty, &dir, &spd);
    hit = partySearchFrontWall(spd, dir, pParty, pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804215b8) {
            *(void**)((s32)pParty + 0x134) = hit;
        }
    }
    *(u32*)((s32)pParty + 0x58) = pos[0];
    *(u32*)((s32)pParty + 0x60) = pos[2];
    hit = partySearchGround(float_10_80421600, float_10_80421600, pParty);
    pos[1] = *(u32*)((s32)pParty + 0xE4);
    if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
        hit = 0;
    }
    *(void**)((s32)pParty + 0x138) = hit;
    if (hit == 0) {
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
        if (*(f32*)((s32)pParty + 0x118) == float_0_804215b8) {
            *(f32*)((s32)pParty + 0x118) = float_neg1p4_80421630;
        }
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x118);
    }
}

f32 partyGetTargetMovespd2(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 float_0_804215b8;
    extern f32 float_150_80421594;
    void* mario = *(void**)((s32)pParty + 0x160);
    void* party;
    s32 invalid;
    f32 speed;

    if ((*(u32*)pParty & 8) == 0) {
        speed = *(f32*)((s32)mario + 0x180);
        if (speed > float_0_804215b8) {
            return speed;
        }
        return *(f32*)((s32)mario + 0x188);
    }
    if ((*(u32*)((s32)pParty + 8) & 0x20000) != 0) {
        speed = *(f32*)((s32)mario + 0x180);
        if (speed > float_0_804215b8) {
            return speed;
        }
        return *(f32*)((s32)mario + 0x188);
    }
    invalid = 0;
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
        return *(f32*)((s32)party + 0x104);
    }
    speed = *(f32*)((s32)mario + 0x180);
    if (speed > float_0_804215b8) {
        return speed;
    }
    return *(f32*)((s32)mario + 0x188);
}

u8 partyMoveBehindMario3(void* pParty, u8 param_2) {
    extern void party_set_behind_dir(void* party);

    if (*(s8*)((s32)pParty + 0x31) == 4 && *(u8*)((s32)pParty + 0x34) == 3) {
        return;
    }
    *(u32*)((s32)pParty + 8) |= 0x100000;
    *(u32*)((s32)pParty + 8) |= 0x40000;
    party_set_behind_dir(pParty);
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

f32 partyGetTargetDistY(s32 useParty) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 float_0_804215b8;
    extern f32 float_150_80421594;
    void* mario = marioGetPtr();
    void* party;
    s32 invalid;
    f32 speed;

    if (useParty == 0) {
        speed = *(f32*)((s32)mario + 0x180);
        if (speed > float_0_804215b8) {
            return speed;
        }
        return *(f32*)((s32)mario + 0x188);
    }
    invalid = 0;
    party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
        return *(f32*)((s32)party + 0x104);
    }
    mario = marioGetPtr();
    speed = *(f32*)((s32)mario + 0x180);
    if (speed > float_0_804215b8) {
        return speed;
    }
    return *(f32*)((s32)mario + 0x188);
}

void partyClearFootmark(void) {
    extern void* marioGetPtr(void);
    void* mario;
    u8* mark;
    u32 x;
    u32 y;
    u32 z;
    s32 i;

    mario = marioGetPtr();
    x = *(u32*)((s32)mario + 0x8C);
    y = *(u32*)((s32)mario + 0x90);
    z = *(u32*)((s32)mario + 0x94);
    mark = footmarkBuf;
    for (i = 0; i < 10; i++, mark += 0x80) {
#define CLEAR_MARK(offset) \
        *(u32*)(mark + (offset) + 4) = x; \
        *(u32*)(mark + (offset) + 8) = y; \
        *(u32*)(mark + (offset) + 0xC) = z; \
        *(u32*)(mark + (offset)) = 0
        CLEAR_MARK(0x00);
        CLEAR_MARK(0x10);
        CLEAR_MARK(0x20);
        CLEAR_MARK(0x30);
        CLEAR_MARK(0x40);
        CLEAR_MARK(0x50);
        CLEAR_MARK(0x60);
        CLEAR_MARK(0x70);
#undef CLEAR_MARK
    }
    writeId[0] = 0;
    readId[0] = 0;
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
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 float_150_80421594;
    void* mario = marioGetPtr();
    void* party;
    void* mario2;
    s32 invalid = 0;

    mario2 = marioGetPtr();
    party = anotherPartyGetPtr(((u32)(-param_1) | param_1) >> 31);
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)mario2 + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (invalid != 0) {
        party = 0;
    }
    if (party != 0) {
        param_2[0] = *(u32*)((s32)party + 0x58);
        param_2[1] = *(u32*)((s32)party + 0x5C);
        param_2[2] = *(u32*)((s32)party + 0x60);
    } else {
        param_2[0] = *(u32*)((s32)mario + 0x8C);
        param_2[1] = *(u32*)((s32)mario + 0x90);
        param_2[2] = *(u32*)((s32)mario + 0x94);
    }
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
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* spd);
    extern void* partySearchFrontWall(f32 dir, f32 spd, void* party, void* pos);
    extern f32 float_0_804215b8;
    u32 pos[3];
    f32 spd;
    f32 dir;
    void* hit;

    pos[0] = *(u32*)((s32)pParty + 0x58);
    pos[1] = *(u32*)((s32)pParty + 0x5C);
    pos[2] = *(u32*)((s32)pParty + 0x60);
    *(f32*)((s32)pParty + 0x104) = float_0_804215b8;
    partyGetMoveDirSpd(pParty, &dir, &spd);
    hit = partySearchFrontWall(spd, dir, pParty, pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804215b8) {
            *(void**)((s32)pParty + 0x134) = hit;
        }
    }
    *(u32*)((s32)pParty + 0x58) = pos[0];
    *(u32*)((s32)pParty + 0x60) = pos[2];
}

void partyChgMot(void* party, s32 mot) {
    if (*(u8*)((s32)party + 0x36) == 3) {
        *(u32*)party &= ~0x70;
        *(u32*)party &= ~0x03000000;
        *(u32*)party &= ~0x04000000;
    }
    *(u8*)((s32)party + 0x36) = mot;
    *(u32*)((s32)party + 8) |= 2;
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
u8 flyMain(void* party) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_JP_US_EU_54_80187d74(void* party);
    extern void unk_JP_US_EU_53_80187d10(void* party);
    extern void party_motion_stay(void* party);
    extern void party_motion_fall(void* party);
    extern void marioSetSplash(u32 type, void* pos);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    u32 pos[3];
    s32 timer;
    u8 mode;
    u8 motion;
    void* hit;

    mode = *(u8*)((s32)party + 0x3B);
    if (mode == 3) {
        return;
    }
    if (mode > 2) {
        if (mode == 6) {
            return;
        }
        return;
    }
    if (mode == 1) {
        return;
    }
    if (mode != 0) {
        return;
    }

    if ((*(u32*)((s32)party + 8) & 4) != 0) {
        *(u32*)((s32)party + 8) &= ~4;
        partyChgPoseId(party, 1);
        if (*(s8*)((s32)party + 0x31) == 5) {
            unk_JP_US_EU_54_80187d74(party);
        }
    }
    motion = *(u8*)((s32)party + 0x36);
    if (motion == 3) {
        party_motion_fall(party);
    } else {
        if (motion < 3) {
            if (motion == 0) {
                party_motion_stay(party);
                if (*(s8*)((s32)party + 0x31) == 5) {
                    unk_JP_US_EU_53_80187d10(party);
                }
                goto tail;
            }
        } else if (motion != 6) {
            goto tail;
        }
        if (motion == 3) {
            *(u32*)party &= ~0x70;
            *(u32*)party &= ~0x03000000;
            *(u32*)party &= ~0x04000000;
        }
        *(u8*)((s32)party + 0x36) = 0;
        *(u32*)((s32)party + 8) |= 2;
    }

tail:
    *(f32*)((s32)party + 0xFC) = *(f32*)((s32)party + 0x100);
    if ((*(u32*)((s32)party + 8) & 0x01000000) != 0) {
        *(u32*)((s32)party + 8) &= ~0x01000000;
        pos[0] = *(u32*)((s32)party + 0x58);
        pos[1] = *(u32*)((s32)party + 0x5C);
        pos[2] = *(u32*)((s32)party + 0x60);
        marioSetSplash(0, pos);
        *(u8*)((s32)party + 0x4C) = 0x14;
    }
    timer = *(s8*)((s32)party + 0x4C) - 1;
    *(s8*)((s32)party + 0x4C) = timer;
    if ((s8)timer < 0) {
        *(u8*)((s32)party + 0x4C) = 0;
    }
    if (*(u8*)((s32)party + 0x34) == 0xD) {
        return;
    }
    hit = *(void**)((s32)party + 0x138);
    if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
        partyChgRunMode(party, 0xD);
        return;
    }
    hit = *(void**)((s32)party + 0x12C);
    if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
        partyChgRunMode(party, 0xD);
        return;
    }
    hit = *(void**)((s32)party + 0x140);
    if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
        partyChgRunMode(party, 0xD);
    }
}

/* CHATGPT FALLBACK MISSING STUBS: main/party/party_move 20260624_191429 */

/* fallback stub-fill: map=unk_800ca884 addr=0x800ca884 size=0x000001e8 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800ca884(void* party) {
    extern void partyChgPoseId(void* party, int pose);
    extern void unk_JP_US_EU_54_80187d74(void* party);
    extern void unk_JP_US_EU_35_8015146c(void* party);
    extern void unk_JP_US_EU_53_80187d10(void* party);
    extern void party_motion_fall(void* party);
    extern void marioSetSplash(unsigned int type, void* pos);
    extern unsigned int hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, int mode);
    extern void vivianShadow(void* party);

    unsigned int pos[3];
    unsigned int posX;
    int timer;
    int motion;
    void* hit;

    if ((*(unsigned int*)((int)party + 8) & 4) != 0) {
        *(unsigned int*)((int)party + 8) &= ~4;
        partyChgPoseId(party, 1);
        *(unsigned int*)party &= ~0x100;
        if (*(signed char*)((int)party + 0x31) == 5) {
            unk_JP_US_EU_54_80187d74(party);
        }
    }

    motion = *(unsigned char*)((int)party + 0x36);
    if (motion == 3) {
        goto fall;
    }
    if (motion >= 3) {
        if (motion == 6) {
            goto reset_motion;
        }
        goto tail;
    }
    if (motion == 0) {
        unk_JP_US_EU_35_8015146c(party);
        if (*(signed char*)((int)party + 0x31) == 5) {
            unk_JP_US_EU_53_80187d10(party);
        }
        goto tail;
    }
    if (motion >= 0) {
        goto reset_motion;
    }
    goto tail;

reset_motion:
    if ((unsigned char)motion == 3) {
        *(unsigned int*)party &= ~0x70;
        *(unsigned int*)party &= ~0x03000000;
        *(unsigned int*)party &= ~0x04000000;
    }
    *(unsigned char*)((int)party + 0x36) = 0;
    *(unsigned int*)((int)party + 8) |= 2;
    goto tail;

fall:
    party_motion_fall(party);

tail:
    *(float*)((int)party + 0xFC) = *(float*)((int)party + 0x100);

    if ((*(unsigned int*)((int)party + 8) & 0x01000000) != 0) {
        *(unsigned int*)((int)party + 8) &= ~0x01000000;
        posX = *(unsigned int*)((int)party + 0x58);
        pos[0] = posX;
        pos[1] = *(unsigned int*)((int)party + 0x5C);
        pos[2] = *(unsigned int*)((int)party + 0x60);
        marioSetSplash(0, pos);
        *(unsigned char*)((int)party + 0x4C) = 0x14;
    }

    timer = *(unsigned char*)((int)party + 0x4C) - 1;
    *(unsigned char*)((int)party + 0x4C) = timer;
    if ((signed char)timer < 0) {
        *(unsigned char*)((int)party + 0x4C) = 0;
    }

    if (*(unsigned char*)((int)party + 0x34) != 0xD) {
        hit = *(void**)((int)party + 0x138);
        if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
            partyChgRunMode(party, 0xD);
        } else {
            hit = *(void**)((int)party + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(party, 0xD);
            } else {
                hit = *(void**)((int)party + 0x140);
                if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                    partyChgRunMode(party, 0xD);
                }
            }
        }
    }
    vivianShadow(party);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

/* fallback stub-fill: map=unk_800cb9bc addr=0x800cb9bc size=0x000000a4 */
s32 unk_800cb9bc(void* hit) {
    extern u32 hitGetAttr(void* hit);
    extern void* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    extern const char* lbl_80314FE8[];
    const char** names;
    void* obj;
    s32 found;

    if (hit == 0) {
        return 0;
    }
    if ((hitGetAttr(hit) & 0x80000000) == 0) {
        return 0;
    }
    names = lbl_80314FE8;
    obj = mobjHitObjPtrToPtr(hit);
    found = 0;
    while (*names != 0) {
        if (strcmp((const char*)((s32)obj + 0x15), *names) == 0) {
            found = 1;
            break;
        }
        names++;
    }
    return found;
}

/* fallback stub-fill: map=unk_800cba60 addr=0x800cba60 size=0x000000b0 */
void unk_800cba60(void* party) {
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void* camGetPtr(s32 id);
    extern f32 revise360(f32 angle);
    extern f32 float_270_80421634;
    extern f32 float_90_80421598;
    extern f32 float_180_804215ac;
    extern f32 float_0_804215b8;
    void* mario = *(void**)((s32)party + 0x160);
    void* cam;
    f32 angle;
    s32 behind;

    angle = angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                     *(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94));
    cam = camGetPtr(4);
    angle = revise360((float_270_80421634 + *(f32*)((s32)cam + 0x114)) - angle);
    behind = 0;
    if (angle >= float_90_80421598 && angle <= float_270_80421634) {
        behind = 1;
    }
    if (behind != 0) {
        *(f32*)((s32)party + 0x110) = float_180_804215ac;
    } else {
        *(f32*)((s32)party + 0x110) = float_0_804215b8;
    }
}

/* fallback stub-fill: map=unk_800cbb10 addr=0x800cbb10 size=0x00000120 */
void unk_800cbb10(void* party, f32 value) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_800cbcc0(void* party, s32 param_2);
    extern s32 N_marioGetMovementSpeed(void);
    extern f32 float_1_804215f8;
    extern f32 float_1p65_804215f4;
    extern f32 float_2_804215cc;
    extern f32 float_0p8_804215fc;
    extern f32 float_0_804215b8;
    void* mario;
    f32 upper;
    f32 lower;

    if ((*(u32*)((s32)party + 4) & 0x100) != 0) {
        partyChgPoseId(party, 3);
        unk_800cbcc0(party, 1);
        return;
    }
    mario = *(void**)((s32)party + 0x160);
    if (*(f32*)((s32)mario + 0x228) == float_1_804215f8) {
        upper = float_1p65_804215f4;
    } else {
        upper = float_2_804215cc;
    }
    if (*(f32*)((s32)mario + 0x228) == float_1_804215f8) {
        lower = float_0p8_804215fc;
    } else {
        lower = float_0_804215b8;
    }
    if (upper - lower < value && value < value + lower) {
        if (N_marioGetMovementSpeed() == 1) {
            partyChgPoseId(party, 2);
            unk_800cbcc0(party, 0);
        } else {
            partyChgPoseId(party, 3);
            unk_800cbcc0(party, 1);
        }
    } else if (value >= upper) {
        partyChgPoseId(party, 3);
        unk_800cbcc0(party, 1);
    } else {
        partyChgPoseId(party, 2);
        unk_800cbcc0(party, 0);
    }
}

/* fallback stub-fill: map=unk_800cbcc0 addr=0x800cbcc0 size=0x000001c4 */
void unk_800cbcc0(void* pParty, int param_2) {
    extern void psndSFXOn_3D(int id, void* pos);

    int timer;
    int step;
    int limit;
    int member;

    *(unsigned char*)((int)pParty + 0x2D) = *(unsigned char*)((int)pParty + 0x2D) + 1;
    member = *(signed char*)((int)pParty + 0x31);

    if (member == 9) {
        if (param_2 == 0) {
            step = 0x14;
            limit = 0x28;
        } else {
            step = 0xA;
            limit = 0x14;
        }
        timer = *(signed char*)((int)pParty + 0x2D);
        if (timer >= limit) {
            *(unsigned char*)((int)pParty + 0x2D) = 0;
            psndSFXOn_3D(0x60D, (void*)((int)pParty + 0x58));
        } else if (timer == step) {
            psndSFXOn_3D(0x60C, (void*)((int)pParty + 0x58));
        }
    } else if (member == 8) {
        if (*(signed char*)((int)pParty + 0x2D) >= 0xF) {
            *(unsigned char*)((int)pParty + 0x2D) = 0;
            psndSFXOn_3D(0x942, (void*)((int)pParty + 0x58));
        }
    } else if (member == 0xA) {
        if (param_2 == 0) {
            step = 5;
            limit = 0xA;
        } else {
            step = 5;
            limit = 0xA;
        }
        timer = *(signed char*)((int)pParty + 0x2D);
        if (timer >= limit) {
            *(unsigned char*)((int)pParty + 0x2D) = 0;
            psndSFXOn_3D(0x3FF, (void*)((int)pParty + 0x58));
        } else if (timer == step) {
            psndSFXOn_3D(0x3FF, (void*)((int)pParty + 0x58));
        }
    } else if (*(unsigned char*)((int)pParty + 0x32) == 1) {
        timer = *(signed char*)((int)pParty + 0x2D);
        limit = 0x14;
        if (param_2 == 0) {
            limit = 0x28;
        }
        if (timer >= limit) {
            *(unsigned char*)((int)pParty + 0x2D) = 0;
            psndSFXOn_3D(0x926, (void*)((int)pParty + 0x58));
        }
    } else {
        if (param_2 == 0) {
            step = 0x14;
            limit = 0x28;
        } else {
            step = 0xA;
            limit = 0x14;
        }
        timer = *(signed char*)((int)pParty + 0x2D);
        if (timer >= limit) {
            *(unsigned char*)((int)pParty + 0x2D) = 0;
            psndSFXOn_3D(0x925, (void*)((int)pParty + 0x58));
        } else if (timer == step) {
            psndSFXOn_3D(0x924, (void*)((int)pParty + 0x58));
        }
    }
}

/* fallback stub-fill: map=unk_800ccb34 addr=0x800ccb34 size=0x00000160 */
f32 unk_800ccb34(s32 useParty, f32 y) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 __fabsf(f32 value);
    extern f32 float_150_80421594;
    void* mario;
    void* player;
    void* party;
    f32 targetY;
    s32 invalid;

    player = marioGetPtr();
    party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
    if (useParty == 0) {
        return __fabsf(*(f32*)((s32)player + 0x90) - y);
    }
    invalid = 0;
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
        mario = marioGetPtr();
        invalid = 0;
        player = marioGetPtr();
        party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
        if (party != 0) {
            if (nokonokoGetStatus(party) == 3 ||
                PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
                invalid = 1;
            }
        }
        if (invalid != 0) {
            party = 0;
        }
        if (party != 0) {
            targetY = *(f32*)((s32)party + 0x5C);
        } else {
            targetY = *(f32*)((s32)mario + 0x90);
        }
        return __fabsf(targetY - y);
    }
    return __fabsf(*(f32*)((s32)player + 0x90) - y);
}

