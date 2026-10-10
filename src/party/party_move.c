#include "party/party_move.h"

s32 readId[2];
s32 writeId[2];

u8 footmarkBuf[0xA00];
extern const f32 float_0_804215b8;
extern const f32 float_0p75_80421608;
extern const f32 float_1_804215f8;

s32 partyMoveMain(void* party, s32 hosei);
f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
void psndSFXOn_3D(s32 id, void* pos);
extern void* marioGetPtr(void);
extern const f32 float_150_80421594;
extern f32 __fabsf(f32 value);
extern s32 marioGetPartyId(void);
extern void* partyGetPtr(s32 id);
extern s32 marioChkSlitThrouh(void);
extern const f32 float_0p5_80421610;
extern const f32 float_0p05_8042160c;
extern const f32 float_neg1_804215c4;
extern const f32 float_0p1_80421614;
extern s32 N_marioGetMovementSpeed(void);
extern const f32 float_1p65_804215f4;
extern const f32 float_2_804215cc;
extern const f32 float_0p8_804215fc;
extern void partyGetMoveDirSpd(void* party, f32* dir, f32* spd);
extern const f32 float_neg1p4_80421630;
extern const f32 float_10_80421600;
extern void unk_JP_US_EU_54_80187d74(void* party);
extern void unk_JP_US_EU_53_80187d10(void* party);
extern void party_motion_fall(void* party);
extern void vivianShadow(void* party);
extern const f32 float_50_80421590;
extern const f32 float_1p8_80421604;
extern void party_motion_stay(void* party);
extern void party_motion_homing_walk(void* party);
extern void party_motion_homing_fly(void* party);
extern void party_motion_jump(void* party);
extern void party_motion_upstairs(void* party);
extern void party_motion_damage(void* party);
extern void party_motion_behind_mario(void* party);
extern void party_motion_behind_mario_fly(void* party);
extern void party_motion_jump2(void* party);
extern void party_motion_beside_mario(void* party);

void* partyGetFootmarkPtr(s32 id) {
    typedef struct { u8 bytes[16]; } Footmark;
    Footmark* bank = (Footmark*)(footmarkBuf + id * 0x500);
    return bank + readId[id];
}

s32 partyGetFootmarkReadId(s32 id) {
    return readId[id];
}

s32 partyGetFootmarkWriteId(s32 id) {
    return writeId[id];
}

s32 partyGetFootmarkId(void* party) {
    return (*(u32*)((s32)party + 0x0) >> 2) & 1;
}

u8 L_getTargetPos(u32 param_1, u32* param_2) {
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
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
        ((f32*)param_2)[0] = *(f32*)((s32)party + 0x58);
        ((f32*)param_2)[1] = *(f32*)((s32)party + 0x5C);
        ((f32*)param_2)[2] = *(f32*)((s32)party + 0x60);
    } else {
        ((f32*)param_2)[0] = *(f32*)((s32)mario + 0x8C);
        ((f32*)param_2)[1] = *(f32*)((s32)mario + 0x90);
        ((f32*)param_2)[2] = *(f32*)((s32)mario + 0x94);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
f32 partyGetTargetDir(s32 useParty, f32 x, f32 z) {
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
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
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
float partyGetTargetDist(float x, float z, int useParty) {
    extern void* anotherPartyGetPtr(int id);
    extern float distABf(float x1, float z1, float x2, float z2);
    extern int nokonokoGetStatus(void* party);
    extern float PSVECDistance(void* a, void* b);

    void* player;
    void* mario;
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
        if (nokonokoGetStatus(party) == 3 || PSVECDistance((void*)((int)player + 0x8C), (void*)((int)party + 0x58)) >= float_150_80421594) {
            invalid = 1;
        }
    }
    if (party != 0 && invalid == 0) {
    mario = marioGetPtr();
    invalid = 0;
    player = marioGetPtr();
    party = anotherPartyGetPtr(((unsigned int)(-useParty) | (unsigned int)useParty) >> 31);
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 || PSVECDistance((void*)((int)player + 0x8C), (void*)((int)party + 0x58)) >= float_150_80421594) {
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
        targetX = *(float*)((int)mario + 0x8C);
        targetZ = *(float*)((int)mario + 0x94);
    }
    return distABf(x, z, targetX, targetZ);
    }
    return distABf(x, z, *(float*)((int)player + 0x8C), *(float*)((int)player + 0x94));
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma opt_common_subs reset

#pragma opt_common_subs off
f32 partyGetTargetDist3D(s32 useParty, void* pos) {
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    void* mario;
    void* mario2;
    void* party;
    f32 target[3];
    s32 invalid;

    mario = marioGetPtr();
    invalid = 0;
    party = anotherPartyGetPtr(((u32)(-useParty) | (u32)useParty) >> 31);
    if (useParty == 0) {
        return PSVECDistance((void*)((s32)marioGetPtr() + 0x8C), pos);
    }
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
            target[0] = *(f32*)((s32)party + 0x58);
            target[1] = *(f32*)((s32)party + 0x5C);
            target[2] = *(f32*)((s32)party + 0x60);
        } else {
            target[0] = *(f32*)((s32)mario2 + 0x8C);
            target[1] = *(f32*)((s32)mario2 + 0x90);
            target[2] = *(f32*)((s32)mario2 + 0x94);
        }
        return PSVECDistance(target, pos);
    }
    return PSVECDistance((void*)((s32)marioGetPtr() + 0x8C), pos);
}
#pragma opt_common_subs reset

/* fallback stub-fill: map=unk_800ccb34 addr=0x800ccb34 size=0x00000160 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
f32 unk_800ccb34(s32 useParty, f32 y) {
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
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

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
f32 partyGetTargetDistY(s32 useParty) {
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
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

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
f32 partyGetTargetMovespd2(void* pParty) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
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

void partyNextReadId(s32 id) {
    if (readId[id] == writeId[id]) {
        return;
    }

    readId[id]++;

    if (readId[id] >= 0x50) {
        readId[id] = 0;
    }
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

void partyClearFootmark(void) {
    void* mario;
    u8* mark;
    typedef struct { u32 x, y, z; } Position;
    Position pos;
    s32 i;

    mario = marioGetPtr();
    pos = *(Position*)((s32)mario + 0x8C);
    mark = footmarkBuf;
    for (i = 0; i < 10; i++, mark += 0x80) {
#define CLEAR_MARK(offset) \
        *(u32*)(mark + (offset) + 4) = pos.x; \
        *(u32*)(mark + (offset) + 8) = pos.y; \
        *(u32*)(mark + (offset) + 0xC) = pos.z; \
        *(u8*)(mark + (offset)) = 0
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

void partyClearFootmark2(void) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    typedef struct { u32 x, y, z; } Position;
    void* mario = marioGetPtr();
    void* party = partyGetPtr(marioGetPartyId());
    Position pos;
    u8* mark;
    s32 i;
    if (party != 0) {
        if (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)mario + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594) {
            pos = *(Position*)((s32)mario + 0x8C);
        } else {
            pos = *(Position*)((s32)party + 0x58);
        }
    } else {
        pos = *(Position*)((s32)mario + 0x8C);
    }
    mark = footmarkBuf + 0x500;
    for (i = 0; i < 10; i++, mark += 0x80) {
#define CLEAR_MARK2(offset) \
        *(u32*)(mark + (offset) + 4) = pos.x; \
        *(u32*)(mark + (offset) + 8) = pos.y; \
        *(u32*)(mark + (offset) + 0xC) = pos.z; \
        *(u8*)(mark + (offset)) = 0
        CLEAR_MARK2(0x00);
        CLEAR_MARK2(0x10);
        CLEAR_MARK2(0x20);
        CLEAR_MARK2(0x30);
        CLEAR_MARK2(0x40);
        CLEAR_MARK2(0x50);
        CLEAR_MARK2(0x60);
        CLEAR_MARK2(0x70);
#undef CLEAR_MARK2
    }
    writeId[1] = 0;
    readId[1] = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
void partyRecordFootmark(void* pParty) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyClearFootmark2(void);
    typedef struct { f32 x, y, z; } Position;
    Position first, fourth, clearMario, last;
    void* player;
    void* move;
    void* party;
    u8* mark;
    u8* base;
    s32 id, i, flags, useMario, jumping;
    flags = 0;
    move = *(void**)((s32)pParty + 0x160);
    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        player = marioGetPtr();
        jumping = 0;
        if (*(u32*)player & 0x10000) jumping = 1;
        if (jumping) flags |= 1;
        if (marioChkSlitThrouh()) flags |= 4;
        first = *(Position*)((s32)move + 0x8C);
        id = (*(u32*)pParty >> 2) & 1;
        marioGetPtr();
        base = footmarkBuf + id * 0x500;
        mark = base + writeId[id] * 0x10;
        if (((*mark & 1) == 0 || (flags & 1) == 0) &&
            (*(f32*)(mark + 4) != first.x || *(f32*)(mark + 8) != first.y || *(f32*)(mark + 12) != first.z) &&
            readId[id] != writeId[id] + 1) {
            writeId[id]++;
            if (writeId[id] >= 80) writeId[id] = 0;
            mark = base + writeId[id] * 0x10;
            *(Position*)(mark + 4) = first;
            if (flags & 1) *mark |= 1; else *mark &= ~1;
            if (flags & 4) *mark |= 4; else *mark &= ~4;
        }
    } else {
        useMario = 0;
        party = partyGetPtr(marioGetPartyId());
        if (party != 0 && (nokonokoGetStatus(party) == 3 ||
            PSVECDistance((void*)((s32)move + 0x8C), (void*)((s32)party + 0x58)) >= float_150_80421594))
            useMario = 1;
        if (party != 0 && !useMario) {
            if (*(s8*)((s32)pParty + 0x51) == 0) {
                *(u8*)((s32)pParty + 0x51) = 1;
        player = marioGetPtr();
        clearMario = *(Position*)((s32)player + 0x8C);
        mark = footmarkBuf + 0;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(Position*)(mark + 4) = clearMario;
            *(u8*)(mark + 0) = 0;
            *(Position*)(mark + 20) = clearMario;
            *(u8*)(mark + 16) = 0;
            *(Position*)(mark + 36) = clearMario;
            *(u8*)(mark + 32) = 0;
            *(Position*)(mark + 52) = clearMario;
            *(u8*)(mark + 48) = 0;
            *(Position*)(mark + 68) = clearMario;
            *(u8*)(mark + 64) = 0;
            *(Position*)(mark + 84) = clearMario;
            *(u8*)(mark + 80) = 0;
            *(Position*)(mark + 100) = clearMario;
            *(u8*)(mark + 96) = 0;
            *(Position*)(mark + 116) = clearMario;
            *(u8*)(mark + 112) = 0;
        }
        writeId[0] = 0;
        readId[0] = 0;
            }
            jumping = 0;
            if (*(u32*)party & 0x10) jumping = 1;
            if (jumping) flags |= 1;
            fourth = *(Position*)((s32)party + 0x58);
        id = (*(u32*)pParty >> 2) & 1;
        marioGetPtr();
        base = footmarkBuf + id * 0x500;
        mark = base + writeId[id] * 0x10;
        if (((*mark & 1) == 0 || (flags & 1) == 0) &&
            (*(f32*)(mark + 4) != fourth.x || *(f32*)(mark + 8) != fourth.y || *(f32*)(mark + 12) != fourth.z) &&
            readId[id] != writeId[id] + 1) {
            writeId[id]++;
            if (writeId[id] >= 80) writeId[id] = 0;
            mark = base + writeId[id] * 0x10;
            *(Position*)(mark + 4) = fourth;
            if (flags & 1) *mark |= 1; else *mark &= ~1;
            *mark &= ~4;
        }
        } else {
            if (*(s8*)((s32)pParty + 0x51) == 1) {
                *(u8*)((s32)pParty + 0x51) = 0;
                partyClearFootmark2();
            }
        player = marioGetPtr();
        jumping = 0;
        if (*(u32*)player & 0x10000) jumping = 1;
        if (jumping) flags |= 1;
        if (marioChkSlitThrouh()) flags |= 4;
            last = *(Position*)((s32)move + 0x8C);
        id = (*(u32*)pParty >> 2) & 1;
        marioGetPtr();
        base = footmarkBuf + id * 0x500;
        mark = base + writeId[id] * 0x10;
        if (((*mark & 1) == 0 || (flags & 1) == 0) &&
            (*(f32*)(mark + 4) != last.x || *(f32*)(mark + 8) != last.y || *(f32*)(mark + 12) != last.z) &&
            readId[id] != writeId[id] + 1) {
            writeId[id]++;
            if (writeId[id] >= 80) writeId[id] = 0;
            mark = base + writeId[id] * 0x10;
            *(Position*)(mark + 4) = last;
            if (flags & 1) *mark |= 1; else *mark &= ~1;
            if (flags & 4) *mark |= 4; else *mark &= ~4;
        }
        }
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void partyClearJumpPara(void* party) {
    f32 zero;

    zero = float_0_804215b8;
    *(u32*)((s32)party + 0x0) &= ~0x70;
    *(f32*)((s32)party + 0x114) = zero;
    *(f32*)((s32)party + 0x118) = zero;
    *(f32*)((s32)party + 0x11C) = zero;
}

void unk_800cbfbc(void* party, f32 speed) {
    void* move;
    f32 current;
    f32 target;
    f32 result;
    f32 factor, base;

    if (speed != float_0_804215b8) {
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
        base = *(f32*)((s32)party + 0x108) * *(f32*)((s32)move + 0x228);
        if ((*(u32*)party & 0x80) != 0) factor = float_0p75_80421608;
        else factor = float_1_804215f8;
        target = base * factor;
    }
    if (target > float_0_804215b8) {
        result = current + float_0p05_8042160c * ((float_0p5_80421610 + target) - current);
        if (result >= target) {
            result = target;
        }
        *(f32*)((s32)party + 0x104) = result;
    } else {
        result = current + float_0p1_80421614 * (float_neg1_804215c4 - current);
        if (result <= float_0_804215b8) {
            result = float_0_804215b8;
        }
        *(f32*)((s32)party + 0x104) = result;
    }
    } else {
        *(f32*)((s32)party + 0x108) = float_0_804215b8;
    }
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

f32 unk_800cbeb0(void* party) {
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

void N_partyPlaySfxPartyLanding1(void* party) {
    psndSFXOn_3D(0x928, (void*)((s32)party + 0x58));
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

/* fallback stub-fill: map=unk_800cbb10 addr=0x800cbb10 size=0x00000120 */
void unk_800cbb10(void* party, f32 value) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_800cbcc0(void* party, s32 param_2);
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

/* fallback stub-fill: map=unk_800cba60 addr=0x800cba60 size=0x000000b0 */
void unk_800cba60(void* party) {
    extern void* camGetPtr(s32 id);
    extern f32 revise360(f32 angle);
    extern const f32 float_270_80421634;
    extern const f32 float_90_80421598;
    extern const f32 float_180_804215ac;
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
/* fallback stub-fill: map=unk_800cb9bc addr=0x800cb9bc size=0x000000a4 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void partyMove(void* party) {
    partyMoveMain(party, 1);
}

void partyMoveNoHosei(void* party) {
    partyMoveMain(party, 0);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyMoveMain(void* party, s32 hosei) {
    extern void* partySearchFrontWall(f32 spd, f32 dir, void* party, f32* pos);
    extern void unk_800c27c0(f32 spd, f32 dir, void* party, void* pos, f32 width, f32 height);
    extern void movePos(f32* x, f32* z, f32 spd, f32 dir);
    extern void* partySearchHead(f32 angle, void* party, f32* pos, f32* dist);
    extern f32 toMovedirSimple(f32 angle);
    extern void* partySearchGround(f32 hi, f32 lo, void* party);
    extern u32 hitGetAttr(void* hit);

    typedef struct { f32 x, y, z; } Position;
    Position pos, headPos;
    f32 factor;
    f32 headDist;
    f32 dir;
    f32 spd;
    void* hit;

    *(void**)((s32)party + 0x12C) = 0;
    *(void**)((s32)party + 0x134) = 0;
    pos = *(Position*)((s32)party + 0x58);
    spd = float_0_804215b8;

    if (hosei != 0) {
        partyGetMoveDirSpd(party, &dir, &spd);
    } else {
        dir = *(f32*)((s32)party + 0x100);
        if ((*(u32*)party & 0x80) != 0) {
            factor = float_0p75_80421608;
        } else {
            factor = float_1_804215f8;
        }
        spd = *(f32*)((s32)party + 0x104) * factor;
    }

    if ((*(u32*)party & 0x1000000) == 0) {
        hit = partySearchFrontWall(spd, dir, party, (f32*)&pos);
        if (hit != 0) {
            *(void**)((s32)party + 0x12C) = hit;
            if (*(f32*)((s32)party + 0x104) != float_0_804215b8) {
                *(void**)((s32)party + 0x134) = hit;
            }
        }
        if (hit != 0) {
            *(f32*)((s32)party + 0x58) = pos.x;
            *(f32*)((s32)party + 0x60) = pos.z;
        } else {
            unk_800c27c0(spd, dir, party, &pos, *(f32*)((s32)party + 0xF4), *(f32*)((s32)party + 0xF0));
            *(f32*)((s32)party + 0x58) = pos.x;
            *(f32*)((s32)party + 0x60) = pos.z;
        }
    } else {
        movePos((f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60), spd, dir);
    }

    if ((*(u32*)party & 0x2000000) == 0) {
        headPos = *(Position*)((s32)party + 0x58);
        headDist = *(f32*)((s32)party + 0xF0);
        hit = partySearchHead(*(f32*)((s32)party + 0x10C), party, (f32*)&headPos, &headDist);
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
        *(u32*)party &= ~2;
        return 1;
    } else {
        hit = 0;
        if (((*(u32*)party & 0x2000000) == 0) && (*(f32*)((s32)party + 0x114) <= float_0_804215b8)) {
            hit = partySearchGround(__fabsf(*(f32*)((s32)party + 0x114)), __fabsf(*(f32*)((s32)party + 0x114)), party);
            pos.y = *(f32*)((s32)party + 0xE4);
            if (hit != 0) {
                if ((hitGetAttr(hit) & 0x200) != 0) {
                    *(void**)((s32)party + 0x138) = hit;
                    *(u32*)party &= ~2;
                    if ((*(u32*)party & 0x20) == 0) {
                        *(u32*)party |= 0x20;
                        if (*(f32*)((s32)party + 0x118) == float_0_804215b8) {
                            *(f32*)((s32)party + 0x118) = float_neg1p4_80421630;
                        }
                    }
                    *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
                    *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x118);
                    return 2;
                }
                if (float_0_804215b8 == *(f32*)((s32)party + 0xE8)) {
                    *(f32*)((s32)party + 0x5C) = pos.y;
                } else {
                    hit = 0;
                }
            }
        }
        *(void**)((s32)party + 0x138) = hit;
        if (hit == 0) {
            if ((*(u32*)party & 0x4000000) == 0) {
                *(u32*)party &= ~2;
                if ((*(u32*)party & 0x20) == 0) {
                    *(u32*)party |= 0x20;
                    if (*(f32*)((s32)party + 0x118) == float_0_804215b8) {
                        *(f32*)((s32)party + 0x118) = float_neg1p4_80421630;
                    }
                }
                *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
                *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x118);
                return 2;
            }
        } else if ((*(u32*)party & 0x20) != 0) {
            *(u32*)party &= ~0x20;
            *(u32*)party |= 2;
            *(f32*)((s32)party + 0x114) = float_0_804215b8;
            *(f32*)((s32)party + 0x118) = float_0_804215b8;
            *(f32*)((s32)party + 0x5C) = pos.y;
            return 3;
        } else {
            *(f32*)((s32)party + 0x5C) = pos.y;
        }
    }
    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void partyChgMoveMode(void* party, s32 mode) {
    switch (*(u8*)((s32)party + 0x3B)) {
        case 6:
            *(u32*)((s32)party + 0x0) &= ~0x8000;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        default:
            break;
    }

    *(u8*)((s32)party + 0x3B) = mode;
    *(u32*)((s32)party + 0x8) |= 4;
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

void partySetDir(void* party, f32 x, f32 z) {
    *(f32*)((s32)party + 0x100) =
        angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
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

u8 partyMoveCheckWall(void* pParty) {
    extern void* partySearchFrontWall(f32 dir, f32 spd, void* party, void* pos);
    extern void* partySearchGround(f32 height, f32 y, void* party);
    extern u32 hitGetAttr(void* hit);
    typedef struct { f32 x, y, z; } Position;
    Position pos;
    f32 spd;
    f32 dir;
    void* hit;

    pos = *(Position*)((s32)pParty + 0x58);
    partyGetMoveDirSpd(pParty, &dir, &spd);
    hit = partySearchFrontWall(spd, dir, pParty, &pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804215b8) {
            *(void**)((s32)pParty + 0x134) = hit;
        }
    }
    *(f32*)((s32)pParty + 0x58) = pos.x;
    *(f32*)((s32)pParty + 0x60) = pos.z;
    hit = partySearchGround(float_10_80421600, float_10_80421600, pParty);
    pos.y = *(f32*)((s32)pParty + 0xE4);
    if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
        hit = 0;
    }
    *(void**)((s32)pParty + 0x138) = hit;
    if (hit == 0) {
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
        if (float_0_804215b8 == *(f32*)((s32)pParty + 0x118)) {
            *(f32*)((s32)pParty + 0x118) = float_neg1p4_80421630;
        }
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x118);
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
void partyGetMoveDirSpd(void* pParty, f32* outDir, f32* outSpd) {
    extern s32 partyMarioHosei(void* party);
    extern void* anotherPartyGetPtr(s32 id);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f32 sqrtf(f32 x);
    extern f32 __float_nan;
    extern char str_MOBJ_TreasureBox_802cbfc0[];
    extern s32 readId[2];
    extern s32 writeId[2];

    register char* pool = str_MOBJ_TreasureBox_802cbfc0;
    void* player;
    void* source;
    u8* mark;
    typedef struct { u32 x, y, z; } Position;
    Position first, second;
    f32 s;
    f32 c;
    f32 dx;
    f32 dz;
    f32 mult;
    f32 square;
    f32 distance;
    f64 value;
    f64 inv;
    f64 half;
    f64 three;
    u32 bits;
    u32 exponent;
    s32 kind;
    s32 hosei;
    s32 ok;
    s32 i;

    player = *(void**)((s32)pParty + 0x160);
    hosei = partyMarioHosei(pParty);

    if (hosei != 0 && *(u8*)((s32)pParty + 0x3B) != 2) {
        if ((*(u32*)pParty & 0x8) == 0) {
            player = marioGetPtr();
            first = *(Position*)((s32)player + 0x8C);

            mark = footmarkBuf;
            for (i = 0; i < 10; i++) {
                *(u32*)(mark + 0x04) = first.x; *(u32*)(mark + 0x08) = first.y; *(u32*)(mark + 0x0C) = first.z; *(u8*)(mark + 0x00) = 0;
                *(u32*)(mark + 0x14) = first.x; *(u32*)(mark + 0x18) = first.y; *(u32*)(mark + 0x1C) = first.z; *(u8*)(mark + 0x10) = 0;
                *(u32*)(mark + 0x24) = first.x; *(u32*)(mark + 0x28) = first.y; *(u32*)(mark + 0x2C) = first.z; *(u8*)(mark + 0x20) = 0;
                *(u32*)(mark + 0x34) = first.x; *(u32*)(mark + 0x38) = first.y; *(u32*)(mark + 0x3C) = first.z; *(u8*)(mark + 0x30) = 0;
                *(u32*)(mark + 0x44) = first.x; *(u32*)(mark + 0x48) = first.y; *(u32*)(mark + 0x4C) = first.z; *(u8*)(mark + 0x40) = 0;
                *(u32*)(mark + 0x54) = first.x; *(u32*)(mark + 0x58) = first.y; *(u32*)(mark + 0x5C) = first.z; *(u8*)(mark + 0x50) = 0;
                *(u32*)(mark + 0x64) = first.x; *(u32*)(mark + 0x68) = first.y; *(u32*)(mark + 0x6C) = first.z; *(u8*)(mark + 0x60) = 0;
                *(u32*)(mark + 0x74) = first.x; *(u32*)(mark + 0x78) = first.y; *(u32*)(mark + 0x7C) = first.z; *(u8*)(mark + 0x70) = 0;
                mark += 0x80;
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
                if (source != 0) {
                    if (nokonokoGetStatus(source) == 3 ||
                        PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)source + 0x58)) >= float_150_80421594) {
                        second = *(Position*)((s32)player + 0x8C);
                    } else {
                        second = *(Position*)((s32)source + 0x58);
                    }
                } else {
                    second = *(Position*)((s32)player + 0x8C);
                }

                mark = footmarkBuf + 0x500;
                for (i = 0; i < 10; i++) {
                    *(u32*)(mark + 0x04) = second.x; *(u32*)(mark + 0x08) = second.y; *(u32*)(mark + 0x0C) = second.z; *(u8*)(mark + 0x00) = 0;
                    *(u32*)(mark + 0x14) = second.x; *(u32*)(mark + 0x18) = second.y; *(u32*)(mark + 0x1C) = second.z; *(u8*)(mark + 0x10) = 0;
                    *(u32*)(mark + 0x24) = second.x; *(u32*)(mark + 0x28) = second.y; *(u32*)(mark + 0x2C) = second.z; *(u8*)(mark + 0x20) = 0;
                    *(u32*)(mark + 0x34) = second.x; *(u32*)(mark + 0x38) = second.y; *(u32*)(mark + 0x3C) = second.z; *(u8*)(mark + 0x30) = 0;
                    *(u32*)(mark + 0x44) = second.x; *(u32*)(mark + 0x48) = second.y; *(u32*)(mark + 0x4C) = second.z; *(u8*)(mark + 0x40) = 0;
                    *(u32*)(mark + 0x54) = second.x; *(u32*)(mark + 0x58) = second.y; *(u32*)(mark + 0x5C) = second.z; *(u8*)(mark + 0x50) = 0;
                    *(u32*)(mark + 0x64) = second.x; *(u32*)(mark + 0x68) = second.y; *(u32*)(mark + 0x6C) = second.z; *(u8*)(mark + 0x60) = 0;
                    *(u32*)(mark + 0x74) = second.x; *(u32*)(mark + 0x78) = second.y; *(u32*)(mark + 0x7C) = second.z; *(u8*)(mark + 0x70) = 0;
                    mark += 0x80;
                }
                writeId[1] = 0;
                readId[1] = 0;
            }
        }
    }

    if ((*(u32*)((s32)pParty + 8) & 0x200000) != 0) {
        hosei = 0;
    }
    if (hosei >= 30) {
        *(s16*)((s32)pParty + 0x52) = 0;
        if (*(s8*)((s32)pParty + 0x31) != 4 || *(u8*)((s32)pParty + 0x34) != 3) {
        *(u8*)((s32)pParty + 0x3D) = 0;
        switch (*(u8*)((s32)pParty + 0x3B)) {
        case 0: case 1: case 2: case 3: break;
        case 6: *(u32*)pParty &= ~0x8000; break;
        default: break;
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

    }
    sincosf(*(f32*)((s32)pParty + 0x100), &s, &c);
    dx = (*(f32*)((s32)pParty + 0xAC) + *(f32*)((s32)pParty + 0xA0)) +
         *(f32*)((s32)pParty + 0x104) * s;
    dz = (*(f32*)((s32)pParty + 0xB4) + *(f32*)((s32)pParty + 0xA8)) +
         *(f32*)((s32)pParty + 0x104) * c;
    *outDir = angleABf(float_0_804215b8, float_0_804215b8, dx, dz);
    if ((*(u32*)pParty & 0x80) != 0) mult = float_0p75_80421608;
    else mult = float_1_804215f8;
    square = dx * dx + dz * dz;
    if (square > float_0_804215b8) {
        value = square;
        inv = __frsqrte(value);
        half = *(f64*)(pool + 0x600);
        three = *(f64*)(pool + 0x608);
        inv = (half * inv) * (three - value * (inv * inv));
        inv = (half * inv) * (three - value * (inv * inv));
        inv = (half * inv) * (three - value * (inv * inv));
        distance = (f32)(value * inv);
    } else if ((f64)square < *(f64*)(pool + 0x610)) {
        distance = __float_nan;
    } else {
        bits = *(u32*)&square;
        exponent = bits & 0x7F800000;
        switch (exponent) {
        case 0x7F800000:
            kind = (bits & 0x007FFFFF) != 0 ? 1 : 2;
            break;
        case 0:
            kind = (bits & 0x007FFFFF) != 0 ? 5 : 3;
            break;
        default:
            kind = 4;
            break;
        }
        if (kind == 1) {
            distance = __float_nan;
        } else {
            distance = square;
        }
    }
    *outSpd = mult * distance;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

int partyMarioHosei(void* pParty) {
    extern double sin(double x);
    extern double cos(double x);
    extern const f32 float_0p4_80421618, float_0p3_8042161c;
    extern const f32 float_0p2_80421620, float_0p15_80421624;
    extern const f32 float_4_80421628, float_90_80421598;
    extern const f32 float_3p1416_8042162c, float_180_804215ac;
    void* player;
    f32 xFactor, zFactor, yFactor;
    f32 widthSum, heightSum;
    f32 angle, speed, difference;
    s16 count;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 0x40000) == 0) {
        xFactor = float_0p4_80421618;
        zFactor = float_0p3_8042161c;
        yFactor = xFactor;
    } else {
        xFactor = float_0p2_80421620;
        zFactor = float_0p15_80421624;
        yFactor = xFactor;
    }
    if ((*(u32*)player & 0x8000) != 0) {
        xFactor = float_0p2_80421620;
        zFactor = float_0p15_80421624;
        yFactor = xFactor;
    }
    widthSum = *(f32*)((s32)pParty + 0xF4) + *(f32*)((s32)player + 0x1B8);
    heightSum = *(f32*)((s32)player + 0x1BC) + *(f32*)((s32)pParty + 0xF0);
    *(f32*)((s32)pParty + 0xA0) = float_0_804215b8;
    *(f32*)((s32)pParty + 0xA8) = float_0_804215b8;
    xFactor *= widthSum;
    zFactor *= widthSum;
    yFactor *= heightSum;
    if (__fabsf(*(f32*)((s32)pParty + 0x58) - *(f32*)((s32)player + 0x8C)) <= xFactor &&
        __fabsf(*(f32*)((s32)pParty + 0x60) - *(f32*)((s32)player + 0x94)) <= zFactor &&
        __fabsf(*(f32*)((s32)pParty + 0x5C) - *(f32*)((s32)player + 0x90)) <= yFactor) {
        angle = angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                         *(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60));
        if (*(f32*)((s32)player + 0x180) != float_0_804215b8) {
            difference = angle - *(f32*)((s32)player + 0x1A4);
            if (__fabsf(difference) <= float_4_80421628) {
                if (difference >= float_0_804215b8) angle += float_90_80421598;
                else angle -= float_90_80421598;
            }
            count = *(s16*)((s32)pParty + 0x52) + 1;
            *(s16*)((s32)pParty + 0x52) = count;
            if (count > 100) *(s16*)((s32)pParty + 0x52) = 100;
            speed = float_1_804215f8;
        } else {
            angle = angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                             *(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60));
            speed = float_4_80421628;
        }
        if (*(f32*)((s32)player + 0x180) > float_1_804215f8)
            speed = float_0p8_804215fc * *(f32*)((s32)player + 0x180);
        angle = (float_3p1416_8042162c * angle) / float_180_804215ac;
        *(f32*)((s32)pParty + 0xA0) = speed * (f32)sin(angle);
        *(f32*)((s32)pParty + 0xA8) = speed * -(f32)cos(angle);
    } else if (*(s16*)((s32)pParty + 0x52) > 0) {
        *(s16*)((s32)pParty + 0x52) -= 1;
    }
    return *(s16*)((s32)pParty + 0x52);
}

u8 partyChkWall(void* pParty) {
    extern void* partySearchFrontWall(f32 dir, f32 spd, void* party, void* pos);
    typedef struct { f32 x, y, z; } Position;
    Position pos;
    f32 spd;
    f32 dir;
    void* hit;

    pos = *(Position*)((s32)pParty + 0x58);
    *(f32*)((s32)pParty + 0x104) = float_0_804215b8;
    partyGetMoveDirSpd(pParty, &dir, &spd);
    hit = partySearchFrontWall(spd, dir, pParty, &pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804215b8) {
            *(void**)((s32)pParty + 0x134) = hit;
        }
    }
    *(f32*)((s32)pParty + 0x58) = pos.x;
    *(f32*)((s32)pParty + 0x60) = pos.z;
}

/* CHATGPT FALLBACK MISSING STUBS: main/party/party_move 20260624_191429 */

/* fallback stub-fill: map=unk_800ca884 addr=0x800ca884 size=0x000001e8 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800ca884(void* party) {
    extern void partyChgPoseId(void* party, int pose);
    extern void unk_JP_US_EU_35_8015146c(void* party);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyWalkInit(void* pParty) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);

    void* player;
    void* move;
    void* other;
    u8* mark;
    typedef struct { u32 x, y, z; } Position;
    Position pos0, pos1;
    s32 i;

    move = *(void**)((s32)pParty + 0x160);
    *(u32*)pParty |= 2;
    if ((*(u32*)pParty & 8) == 0) {
        player = marioGetPtr();
        pos0 = *(Position*)((s32)player + 0x8C);
        mark = footmarkBuf;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(Position*)(mark + 0x04) = pos0; *(u8*)(mark + 0x0) = 0;
            *(Position*)(mark + 0x14) = pos0; *(u8*)(mark + 0x10) = 0;
            *(Position*)(mark + 0x24) = pos0; *(u8*)(mark + 0x20) = 0;
            *(Position*)(mark + 0x34) = pos0; *(u8*)(mark + 0x30) = 0;
            *(Position*)(mark + 0x44) = pos0; *(u8*)(mark + 0x40) = 0;
            *(Position*)(mark + 0x54) = pos0; *(u8*)(mark + 0x50) = 0;
            *(Position*)(mark + 0x64) = pos0; *(u8*)(mark + 0x60) = 0;
            *(Position*)(mark + 0x74) = pos0; *(u8*)(mark + 0x70) = 0;
        }
        writeId[0] = 0;
        readId[0] = 0;
    } else {
        player = marioGetPtr();
        other = partyGetPtr(marioGetPartyId());
        if (other != 0) {
            if ((nokonokoGetStatus(other) == 3) ||
                   (PSVECDistance((void*)((s32)player + 0x8C),
                                  (void*)((s32)other + 0x58)) >= float_150_80421594)) {
            pos1 = *(Position*)((s32)player + 0x8C);
                    } else {
            pos1 = *(Position*)((s32)other + 0x58);
                    }
        } else {
            pos1 = *(Position*)((s32)player + 0x8C);
                }
        mark = footmarkBuf + 0x500;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(Position*)(mark + 0x04) = pos1; *(u8*)(mark + 0x0) = 0;
            *(Position*)(mark + 0x14) = pos1; *(u8*)(mark + 0x10) = 0;
            *(Position*)(mark + 0x24) = pos1; *(u8*)(mark + 0x20) = 0;
            *(Position*)(mark + 0x34) = pos1; *(u8*)(mark + 0x30) = 0;
            *(Position*)(mark + 0x44) = pos1; *(u8*)(mark + 0x40) = 0;
            *(Position*)(mark + 0x54) = pos1; *(u8*)(mark + 0x50) = 0;
            *(Position*)(mark + 0x64) = pos1; *(u8*)(mark + 0x60) = 0;
            *(Position*)(mark + 0x74) = pos1; *(u8*)(mark + 0x70) = 0;
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
        switch (*(u8*)((s32)pParty + 0x3B)) {
            case 0: case 1: case 2: case 3: break;
            case 6: *(u32*)pParty &= ~0x8000; break;
            default: break;
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyMoveWalk(void* pParty) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void partyWalkMain(void* party);

    void* player;
    void* party;
    void* move;
    u8* mark;
    typedef struct { u32 x, y, z; } Position;
    Position pos0, pos1;
    f32 speed;
    f32 target;
    s32 i;

    if ((*(u32*)((s32)pParty + 8) & 8) != 0) {
        *(u32*)((s32)pParty + 8) &= ~8;
        *(u32*)pParty &= ~0x100;
        move = *(void**)((s32)pParty + 0x160);
        *(u32*)pParty |= 2;
        if ((*(u32*)pParty & 8) == 0) {
            player = marioGetPtr();
            pos0 = *(Position*)((s32)player + 0x8C);
            mark = footmarkBuf;
            for (i = 0; i < 10; i++, mark += 0x80) {
#define INIT_MARK(n) \
                *(Position*)(mark + (n) * 0x10 + 4) = pos0; \
                *(u8*)(mark + (n) * 0x10) = 0
                INIT_MARK(0); INIT_MARK(1); INIT_MARK(2); INIT_MARK(3);
                INIT_MARK(4); INIT_MARK(5); INIT_MARK(6); INIT_MARK(7);
#undef INIT_MARK
            }
            writeId[0] = 0;
            readId[0] = 0;
        } else {
            player = marioGetPtr();
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                if ((nokonokoGetStatus(party) == 3) ||
                       (PSVECDistance((void*)((s32)player + 0x8C),
                                      (void*)((s32)party + 0x58)) >= float_150_80421594)) {
                pos1 = *(Position*)((s32)player + 0x8C);
                            } else {
                pos1 = *(Position*)((s32)party + 0x58);
                            }
            } else {
                pos1 = *(Position*)((s32)player + 0x8C);
                        }
            mark = footmarkBuf + 0x500;
            for (i = 0; i < 10; i++, mark += 0x80) {
#define INIT_MARK(n) \
                *(Position*)(mark + (n) * 0x10 + 4) = pos1; \
                *(u8*)(mark + (n) * 0x10) = 0
                INIT_MARK(0); INIT_MARK(1); INIT_MARK(2); INIT_MARK(3);
                INIT_MARK(4); INIT_MARK(5); INIT_MARK(6); INIT_MARK(7);
#undef INIT_MARK
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
            switch (*(u8*)((s32)pParty + 0x3B)) {
                case 0: case 1: case 2: case 3: break;
                case 6: *(u32*)pParty &= ~0x8000; break;
                default: break;
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
        if ((*(u32*)move & 0x8000) != 0) {
            target = *(f32*)((s32)move + 0x180);
        } else {
            target = (*(f32*)((s32)pParty + 0x108) * *(f32*)((s32)move + 0x228)) *
                ((*(u32*)pParty & 0x80) != 0 ? float_0p75_80421608 : float_1_804215f8);
        }
        if (target > float_0_804215b8) {
            speed = speed + float_0p05_8042160c * ((float_0p5_80421610 + target) - speed);
            if (speed >= target) speed = target;
            *(f32*)((s32)pParty + 0x104) = speed;
        } else {
            speed = speed + float_0p1_80421614 * (float_neg1_804215c4 - speed);
            if (speed <= float_0_804215b8) speed = float_0_804215b8;
            *(f32*)((s32)pParty + 0x104) = speed;
        }
    }
    partyWalkMain(pParty);
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyWalkMain(void* pParty) {
    extern void* anotherPartyGetPtr(s32 slot);
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void walkMain(void* party);
    void* player;
    void* source;
    u8* mark;
    u32 state = 0;
    s32 jumping;
    typedef struct { f32 x, y, z; } Position;
    Position pos0, pos1, pos2, pos3;

#define RECORD_WALK_MARK(markId, position) \
    do { \
        u8* base = footmarkBuf + (markId) * 0x500; \
        mark = base + writeId[(markId)] * 0x10; \
        if (((*mark & 1) == 0 || (state & 1) == 0) && \
            (*(f32*)(mark + 4) != position.x || \
             *(f32*)(mark + 8) != position.y || \
             *(f32*)(mark + 0xC) != position.z) && \
            readId[(markId)] != writeId[(markId)] + 1) { \
            writeId[(markId)]++; \
            if (writeId[(markId)] > 0x4F) writeId[(markId)] = 0; \
            mark = base + writeId[(markId)] * 0x10; \
            *(Position*)(mark + 4) = position; \
            if ((state & 1) != 0) *mark |= 1; \
            else *mark &= ~1; \
            if ((state & 4) != 0) *mark |= 4; \
            else *mark &= ~4; \
        } \
    } while (0)

    player = *(void**)((s32)pParty + 0x160);
    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        source = marioGetPtr();
        jumping = 0;
        if ((*(u32*)source & 0x10000) != 0) jumping = 1;
        if (jumping != 0) state |= 1;
        if (marioChkSlitThrouh() != 0) state |= 4;
        pos0 = *(Position*)((s32)player + 0x8C);
        marioGetPtr();
        RECORD_WALK_MARK(0, pos0);
    } else {
        source = anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        if (source != 0) {
            if ((*(s8*)((s32)source + 0x31) == 3 && *(u8*)((s32)source + 0x34) == 3) ||
                nokonokoGetStatus(source) == 3 ||
                PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)source + 0x58)) >= float_150_80421594) {
                source = marioGetPtr();
                state = 0;
                if ((*(u32*)source & 0x10000) != 0) state = 1;
                source = marioGetPtr();
                pos1 = *(Position*)((s32)source + 0x8C);
                marioGetPtr();
                RECORD_WALK_MARK(1, pos1);
            } else {
                state = 0;
                if ((*(u32*)source & 0x10) != 0) state = 1;
                pos2 = *(Position*)((s32)source + 0x58);
                marioGetPtr();
                RECORD_WALK_MARK(1, pos2);
            }
        } else {
            source = marioGetPtr();
            state = 0;
            if ((*(u32*)source & 0x10000) != 0) state = 1;
            source = marioGetPtr();
            pos3 = *(Position*)((s32)source + 0x8C);
            marioGetPtr();
            RECORD_WALK_MARK(1, pos3);
        }
    }
#undef RECORD_WALK_MARK
    walkMain(pParty);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_move_mode_stay(void* party) {
    extern void partyChgPoseId(void* party, int pose);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_move_mode_walk(void* party) {
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


void party_move_mode_behind_mario(void* pParty) {

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

void party_move_mode_beside_mario(void* pParty) {

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyForceWalk(void* pParty) {
    extern void bomhei_use_cancel(void);
    extern void nokonoko_finish(void* party);
    extern void unk_JP_US_EU_12_800bc660(void* party);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_JP_US_EU_18_800cbcc0(void* party, s32 arg);
    extern s32 partySearchFrontWall(f32 distance, f32 angle, void* party, f32* pos);
    extern void* partySearchGround(f32 height, f32 y, void* party);
    extern u32 hitGetAttr(void* hit);

    typedef struct { f32 x, y, z; } Position;
    Position pos;
    f32 speed;
    f32 upper;
    f32 lower;
    s16 timer;
    void* hit;
    u32 attr;
    s32 motion;

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
    switch (motion) {
        case 0: case 1:
        if (*(s16*)((s32)pParty + 0x28) < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
            unk_JP_US_EU_12_800bc660(pParty);
        } else {
            speed = *(f32*)((s32)pParty + 0x104);
            if ((*(u32*)((s32)pParty + 4) & 0x100) != 0) {
                partyChgPoseId(pParty, 3);
                unk_JP_US_EU_18_800cbcc0(pParty, 1);
            } else {
                upper = float_1_804215f8 == *(f32*)(*(s32*)((s32)pParty + 0x160) + 0x228) ? float_1p65_804215f4 : float_2_804215cc;
                lower = float_1_804215f8 == *(f32*)(*(s32*)((s32)pParty + 0x160) + 0x228) ? float_0p8_804215fc : float_0_804215b8;
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

            pos = *(Position*)((s32)pParty + 0x58);
            if (partySearchFrontWall(*(f32*)((s32)pParty + 0x104),
                                     *(f32*)((s32)pParty + 0x100), pParty, (f32*)&pos) == 0) {
                *(f32*)((s32)pParty + 0x58) = pos.x;
                *(f32*)((s32)pParty + 0x60) = pos.z;
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
            break;
        case 2:
        timer = *(s16*)((s32)pParty + 0x28) - 1;
        *(s16*)((s32)pParty + 0x28) = timer;
        if (timer < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
        }
        party_motion_jump(pParty);
            break;
        case 3:
        timer = *(s16*)((s32)pParty + 0x28) - 1;
        *(s16*)((s32)pParty + 0x28) = timer;
        if (timer < 1) {
            *(s16*)((s32)pParty + 0x28) = 0;
        }
        party_motion_fall(pParty);
            break;
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
u8 partyMoveFlyInit(void* pParty, s32 param_2) {
    extern s32 nokonokoGetStatus(void* party);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyChgPoseId(void* party, s32 pose);
    void* player;
    void* partner;
    u8* mark;
    typedef struct { u32 x, y, z; } Position;
    Position pos0, pos1;
    s32 i;
    s8 state;

#define SET_FLY_MODE(mode) \
    switch (*(u8*)((s32)pParty + 0x3B)) { \
        case 0: case 1: case 2: case 3: break; \
        case 6: *(u32*)pParty &= ~0x8000; break; \
        default: break; \
    } \
    *(u8*)((s32)pParty + 0x3B) = (mode); \
    *(u32*)((s32)pParty + 8) |= 4

    marioGetPtr();
    *(u32*)pParty |= 2;
    if ((*(u32*)pParty & 0x100) == 0) {
        player = marioGetPtr();
        pos0 = *(Position*)((s32)player + 0x8C);
        mark = footmarkBuf;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(Position*)(mark + 0x04) = pos0; *(u8*)(mark + 0x0) = 0;
            *(Position*)(mark + 0x14) = pos0; *(u8*)(mark + 0x10) = 0;
            *(Position*)(mark + 0x24) = pos0; *(u8*)(mark + 0x20) = 0;
            *(Position*)(mark + 0x34) = pos0; *(u8*)(mark + 0x30) = 0;
            *(Position*)(mark + 0x44) = pos0; *(u8*)(mark + 0x40) = 0;
            *(Position*)(mark + 0x54) = pos0; *(u8*)(mark + 0x50) = 0;
            *(Position*)(mark + 0x64) = pos0; *(u8*)(mark + 0x60) = 0;
            *(Position*)(mark + 0x74) = pos0; *(u8*)(mark + 0x70) = 0;
        }
        writeId[0] = 0;
        readId[0] = 0;
    } else {
        player = marioGetPtr();
        partner = partyGetPtr(marioGetPartyId());
        if (partner == 0 || nokonokoGetStatus(partner) == 3 ||
            PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)partner + 0x58)) >= float_150_80421594) {
            pos1 = *(Position*)((s32)player + 0x8C);
        } else {
            pos1 = *(Position*)((s32)partner + 0x58);
        }
        mark = footmarkBuf + 0x500;
        for (i = 0; i < 10; i++, mark += 0x80) {
            *(Position*)(mark + 0x04) = pos1; *(u8*)(mark + 0x0) = 0;
            *(Position*)(mark + 0x14) = pos1; *(u8*)(mark + 0x10) = 0;
            *(Position*)(mark + 0x24) = pos1; *(u8*)(mark + 0x20) = 0;
            *(Position*)(mark + 0x34) = pos1; *(u8*)(mark + 0x30) = 0;
            *(Position*)(mark + 0x44) = pos1; *(u8*)(mark + 0x40) = 0;
            *(Position*)(mark + 0x54) = pos1; *(u8*)(mark + 0x50) = 0;
            *(Position*)(mark + 0x64) = pos1; *(u8*)(mark + 0x60) = 0;
            *(Position*)(mark + 0x74) = pos1; *(u8*)(mark + 0x70) = 0;
        }
        writeId[1] = 0;
        readId[1] = 0;
    }
    if (*(u8*)((s32)pParty + 0x3B) == 2) {
        *(f32*)((s32)pParty + 0x40) = float_50_80421590;
        return;
    }
    *(u8*)((s32)pParty + 0x3D) = (~-(param_2 == 0)) & 2;
    SET_FLY_MODE(0);
    state = *(s8*)((s32)pParty + 0x3F);
    if (state == 1 || state == 3) {
        *(u8*)((s32)pParty + 0x3F) = 0;
        *(u8*)((s32)pParty + 0x3D) = 0;
        SET_FLY_MODE(1);
    } else if (state == 2) {
        *(u8*)((s32)pParty + 0x3D) = 0;
        SET_FLY_MODE(0x3C);
    }
    *(u32*)((s32)pParty + 0x44) = 0x10;
    *(f32*)((s32)pParty + 0x40) = float_50_80421590;
    *(u8*)((s32)pParty + 0x3E) = 0;
    partyChgPoseId(pParty, 1);
    SET_FLY_MODE(1);
    if (*(u8*)((s32)pParty + 0x36) == 3) {
        *(u32*)pParty &= ~0x70;
        *(u32*)pParty &= ~0x03000000;
        *(u32*)pParty &= ~0x04000000;
    }
    *(u8*)((s32)pParty + 0x36) = 0;
    *(u32*)((s32)pParty + 8) |= 2;
#undef SET_FLY_MODE
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void partyFlyMain(void* pParty) {
    extern void* marioGetPtr(void);
    extern void* anotherPartyGetPtr(s32 slot);
    extern void* partyGetPtr(s32 id);
    extern void flyMain(void* party);
    typedef struct { f32 x, y, z; } Position;
    void* source;
    u8* mark;
    s32 next;
    u32 flags;
    Position pos0, pos1, pos2;

    if (*(s8*)((s32)pParty + 0x2F) == 0) {
        source = marioGetPtr();
        flags = 0;
        if (*(u32*)source & 0x10000) flags |= 1;
        source = marioGetPtr();
        pos0 = *(Position*)((s32)source + 0x8C);
        marioGetPtr();
        mark = footmarkBuf + writeId[0] * 0x10;
        if (((*mark & 1) == 0 || (flags & 1) == 0) &&
            (*(f32*)(mark + 4) != pos0.x || *(f32*)(mark + 8) != pos0.y || *(f32*)(mark + 12) != pos0.z) &&
            readId[0] != writeId[0] + 1) {
            writeId[0]++;
            if (writeId[0] >= 80) writeId[0] = 0;
            next = writeId[0];
            mark = footmarkBuf + next * 16;
            *(Position*)(mark + 4) = pos0;
            if (flags & 1) *mark |= 1;
            else *mark &= ~1;
            if (flags & 4) *mark |= 4;
            else *mark &= ~4;
        }
    } else {
        anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        source = partyGetPtr(0);
        if (source != 0) {
            flags = 0;
            if (*(u32*)source & 0x10) flags |= 1;
            pos1 = *(Position*)((s32)source + 0x58);
            marioGetPtr();
            mark = footmarkBuf + 0x500 + writeId[1] * 0x10;
            if (((*mark & 1) == 0 || (flags & 1) == 0) &&
                (*(f32*)(mark + 4) != pos1.x || *(f32*)(mark + 8) != pos1.y || *(f32*)(mark + 12) != pos1.z) &&
                readId[1] != writeId[1] + 1) {
                writeId[1]++;
                if (writeId[1] >= 80) writeId[1] = 0;
                next = writeId[1];
                mark = footmarkBuf + 0x500 + next * 16;
                *(Position*)(mark + 4) = pos1;
                if (flags & 1) *mark |= 1;
                else *mark &= ~1;
                if (flags & 4) *mark |= 4;
                else *mark &= ~4;
            }
        } else {
            source = marioGetPtr();
            flags = 0;
            if (*(u32*)source & 0x10000) flags |= 1;
            source = marioGetPtr();
            pos2 = *(Position*)((s32)source + 0x8C);
            marioGetPtr();
            mark = footmarkBuf + writeId[0] * 0x10;
            if (((*mark & 1) == 0 || (flags & 1) == 0) &&
                (*(f32*)(mark + 4) != pos2.x || *(f32*)(mark + 8) != pos2.y || *(f32*)(mark + 12) != pos2.z) &&
                readId[0] != writeId[0] + 1) {
                writeId[0]++;
                if (writeId[0] >= 80) writeId[0] = 0;
                next = writeId[0];
                mark = footmarkBuf + next * 16;
                *(Position*)(mark + 4) = pos2;
                if (flags & 1) *mark |= 1;
                else *mark &= ~1;
                if (flags & 4) *mark |= 4;
                else *mark &= ~4;
            }
        }
    }
    flyMain(pParty);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void walkMain(void* pParty) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void party_motion_behind_mario_continue(void* party);
    extern void marioSetSplash(u32 type, void* pos);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    u32 pos0[3];
    u32 pos1[3];
    s32 timer;
    s32 motion;
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
            switch (motion) {
                case 0:
                    party_motion_stay(pParty);
                    if (*(s8*)((s32)pParty + 0x31) == 5) {
                        unk_JP_US_EU_53_80187d10(pParty);
                    }
                    break;
                case 1:
                case 2:
                case 6:
                    if (*(u8*)((s32)pParty + 0x36) == 3) {
                        *(u32*)pParty &= ~0x70;
                        *(u32*)pParty &= ~0x03000000;
                        *(u32*)pParty &= ~0x04000000;
                    }
                    *(u8*)((s32)pParty + 0x36) = 0;
                    *(u32*)((s32)pParty + 8) |= 2;
                    break;
                case 3:
                    party_motion_fall(pParty);
                    break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            if ((*(u32*)((s32)pParty + 8) & 0x01000000) != 0) {
                *(u32*)((s32)pParty + 8) &= ~0x01000000;
                pos0[0] = *(u32*)((s32)pParty + 0x58);
                pos0[1] = *(u32*)((s32)pParty + 0x5C);
                pos0[2] = *(u32*)((s32)pParty + 0x60);
                marioSetSplash(0, pos0);
                *(u8*)((s32)pParty + 0x4C) = 0x14;
            }
            timer = *(u8*)((s32)pParty + 0x4C) - 1;
            *(s8*)((s32)pParty + 0x4C) = timer;
            if ((s8)timer < 0) {
                *(u8*)((s32)pParty + 0x4C) = 0;
            }
            if (*(u8*)((s32)pParty + 0x34) == 0xD) {
                return;
            }
            hit = *(void**)((s32)pParty + 0x138);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x140);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
            }
            return;
        case 1:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            switch (*(u8*)((s32)pParty + 0x36)) {
                case 0: party_motion_stay(pParty); break;
                case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) party_motion_homing_walk(pParty);
                    else party_motion_homing_fly(pParty);
                    break;
                case 2: party_motion_jump(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            if ((*(u32*)((s32)pParty + 8) & 0x01000000) != 0) {
                *(u32*)((s32)pParty + 8) &= ~0x01000000;
                pos1[0] = *(u32*)((s32)pParty + 0x58);
                pos1[1] = *(u32*)((s32)pParty + 0x5C);
                pos1[2] = *(u32*)((s32)pParty + 0x60);
                marioSetSplash(0, pos1);
                *(u8*)((s32)pParty + 0x4C) = 0x14;
            }
            timer = *(u8*)((s32)pParty + 0x4C) - 1;
            *(s8*)((s32)pParty + 0x4C) = timer;
            if ((s8)timer < 0) {
                *(u8*)((s32)pParty + 0x4C) = 0;
            }
            if (*(u8*)((s32)pParty + 0x34) == 0xD) {
                return;
            }
            hit = *(void**)((s32)pParty + 0x138);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x140);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
            }
            return;
        case 2:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                    break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 3:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1: party_motion_beside_mario(pParty); break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 6:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario_continue(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                    break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        default:
            return;
    }

}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset



/* CHATGPT STUB FILL: main/party/party_move 20260624_183901 */

/* stub-fill: flyMain | missing_definition | ghidra_signature */
void flyMain(void* pParty) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void party_motion_behind_mario_continue(void* party);
    extern void marioSetSplash(u32 type, void* pos);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    u32 pos0[3];
    u32 pos1[3];
    s32 timer;
    s32 motion;
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
            switch (motion) {
                case 0:
                    party_motion_stay(pParty);
                    if (*(s8*)((s32)pParty + 0x31) == 5) {
                        unk_JP_US_EU_53_80187d10(pParty);
                    }
                    break;
                case 1:
                case 2:
                case 6:
                    if (*(u8*)((s32)pParty + 0x36) == 3) {
                        *(u32*)pParty &= ~0x70;
                        *(u32*)pParty &= ~0x03000000;
                        *(u32*)pParty &= ~0x04000000;
                    }
                    *(u8*)((s32)pParty + 0x36) = 0;
                    *(u32*)((s32)pParty + 8) |= 2;
                    break;
                case 3:
                    party_motion_fall(pParty);
                    break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            if ((*(u32*)((s32)pParty + 8) & 0x01000000) != 0) {
                *(u32*)((s32)pParty + 8) &= ~0x01000000;
                pos0[0] = *(u32*)((s32)pParty + 0x58);
                pos0[1] = *(u32*)((s32)pParty + 0x5C);
                pos0[2] = *(u32*)((s32)pParty + 0x60);
                marioSetSplash(0, pos0);
                *(u8*)((s32)pParty + 0x4C) = 0x14;
            }
            timer = *(u8*)((s32)pParty + 0x4C) - 1;
            *(s8*)((s32)pParty + 0x4C) = timer;
            if ((s8)timer < 0) {
                *(u8*)((s32)pParty + 0x4C) = 0;
            }
            if (*(u8*)((s32)pParty + 0x34) == 0xD) {
                return;
            }
            hit = *(void**)((s32)pParty + 0x138);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x140);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
            }
            return;
        case 1:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            switch (*(u8*)((s32)pParty + 0x36)) {
                case 0: party_motion_stay(pParty); break;
                case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) party_motion_homing_walk(pParty);
                    else party_motion_homing_fly(pParty);
                    break;
                case 2: party_motion_jump(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            if ((*(u32*)((s32)pParty + 8) & 0x01000000) != 0) {
                *(u32*)((s32)pParty + 8) &= ~0x01000000;
                pos1[0] = *(u32*)((s32)pParty + 0x58);
                pos1[1] = *(u32*)((s32)pParty + 0x5C);
                pos1[2] = *(u32*)((s32)pParty + 0x60);
                marioSetSplash(0, pos1);
                *(u8*)((s32)pParty + 0x4C) = 0x14;
            }
            timer = *(u8*)((s32)pParty + 0x4C) - 1;
            *(s8*)((s32)pParty + 0x4C) = timer;
            if ((s8)timer < 0) {
                *(u8*)((s32)pParty + 0x4C) = 0;
            }
            if (*(u8*)((s32)pParty + 0x34) == 0xD) {
                return;
            }
            hit = *(void**)((s32)pParty + 0x138);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x12C);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
                return;
            }
            hit = *(void**)((s32)pParty + 0x140);
            if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) {
                partyChgRunMode(pParty, 0xD);
            }
            return;
        case 2:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                    break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 3:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1: party_motion_beside_mario(pParty); break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        case 6:
            if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
                *(u32*)((s32)pParty + 8) &= ~4;
            }
            motion = *(u8*)((s32)pParty + 0x36);
            switch (motion) {
                case 0: case 1:
                    if (*(u8*)((s32)pParty + 0x32) == 0) {
                        party_motion_behind_mario_continue(pParty);
                    } else {
                        party_motion_behind_mario_fly(pParty);
                    }
                    break;
                case 2: party_motion_jump2(pParty); break;
                case 3: party_motion_fall(pParty); break;
                case 6: party_motion_upstairs(pParty); break;
                case 7: party_motion_damage(pParty); break;
            }
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            return;
        default:
            return;
    }

}

/* Functions merged into their canonical party_move translation unit. */
extern f64 revise360(f64 angle);
extern f64 toMovedir(f64 angle);
extern f64 toMovedir2(f64 angle, f64 offset);
extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
extern void movePos(f64 distance, f64 angle, f32* x, f32* z);
extern s32 hitCheckVecFilter(void* work, void* filter);
extern u32 hitGetAttr(void* hit);
extern s32 strcmp(char* a, char* b);
extern char* dat_ptrarr_80314fe8[];
extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
extern const f32 float_30_804215c0;
extern const f32 float_37_804215b0;
extern const f32 float_150_80421594;
extern const f32 float_180_804215ac;
extern const f32 float_87_804215b4;

/* stub-fill: partyGetAppearPos | missing_definition | ghidra_signature */
typedef struct PartyAppearVec3 {
    f32 x, y, z;
} PartyAppearVec3;

extern const PartyAppearVec3 vec3_802cc1cc;
extern const PartyAppearVec3 vec3_802cc1d8;
extern const PartyAppearVec3 vec3_802cc1e4;
extern const PartyAppearVec3 vec3_802cc1f0;
extern const PartyAppearVec3 vec3_802cc1fc;
extern const PartyAppearVec3 vec3_802cc208;
extern const PartyAppearVec3 vec3_802cc214;
extern const PartyAppearVec3 vec3_802cc220;
extern const PartyAppearVec3 vec3_802cc22c;
extern const PartyAppearVec3 vec3_802cc238;
extern const PartyAppearVec3 vec3_802cc244;
extern const PartyAppearVec3 vec3_802cc250;
extern const PartyAppearVec3 vec3_802cc25c;
extern const PartyAppearVec3 vec3_802cc268;
extern const PartyAppearVec3 vec3_802cc274;
extern const PartyAppearVec3 vec3_802cc280;
extern const PartyAppearVec3 vec3_802cc28c;
extern const PartyAppearVec3 vec3_802cc298;
extern const PartyAppearVec3 vec3_802cc2a4;
extern const PartyAppearVec3 vec3_802cc2b0;
extern const PartyAppearVec3 vec3_802cc2bc;
extern const PartyAppearVec3 vec3_802cc2c8;
extern const PartyAppearVec3 vec3_802cc2d4;
extern const PartyAppearVec3 vec3_802cc2e0;
extern const PartyAppearVec3 vec3_802cc2ec;
extern const PartyAppearVec3 vec3_802cc2f8;
extern const PartyAppearVec3 vec3_802cc304;
extern const PartyAppearVec3 vec3_802cc310;
extern const PartyAppearVec3 vec3_802cc31c;
extern const PartyAppearVec3 vec3_802cc328;
extern const PartyAppearVec3 vec3_802cc334;
extern const PartyAppearVec3 vec3_802cc340;

u8 partyGetAppearPos(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        PartyAppearVec3 start;
        PartyAppearVec3 dir;
        PartyAppearVec3 hitPos;
        PartyAppearVec3 normal;
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern f32 revise360(f32);
    extern f32 toMovedir(f32);
    extern f32 toMovedir2(f32, f32);
    extern void movePos(f32, f32, f32*, f32*);
    extern void* hitCheckVecFilter(void*, void*);
    extern s32 marioPlaneGetFlyingDir(void);
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern const f32 float_0_804215b8;
    extern const f32 float_100_804215dc;
    extern const f32 float_neg30_804215e0;
    extern const f32 float_neg90_804215d4;
    extern const f32 float_90_80421598;
    extern const f32 float_70_804215e4;
    extern const f32 float_110_804215e8;
    extern const f32 float_neg110_804215ec;
    extern const f32 float_neg70_804215f0;
    void* player;
    HitWork work;
    f32 baseDirection;
    f32 oppositeDirection;
    f32 angle;
    f32 sine;
    f32 cosine;
    f32 width;
    u32 attr;
    char** namePtr;
    s32 valid;
    s32 found;
    MobjName* mobj;
    void* hit;

#define TRY_APPEAR_ANGLE(scanAngleValue, moveAngleValue, moveDistance, \
                         startTemplate, dirTemplate, groundTemplate, downTemplate, firstPhase) \
    do { \
        PartyAppearVec3 startVec; \
        PartyAppearVec3 dirVec; \
        PartyAppearVec3 groundVec; \
        PartyAppearVec3 downVec; \
        angle = (scanAngleValue); \
        sincosf(angle, &sine, &cosine); \
        work.dist = width; \
        startVec = (startTemplate); \
        dirVec = (dirTemplate); \
        startVec.x = (firstPhase) ? outPos[0] : *(f32*)((s32)player + 0x8C); \
        startVec.y = float_37_804215b0 + ((firstPhase) ? outPos[1] : *(f32*)((s32)player + 0x90)); \
        startVec.z = (firstPhase) ? outPos[2] : *(f32*)((s32)player + 0x94); \
        dirVec.x = sine; \
        dirVec.z = cosine; \
        work.start = startVec; \
        work.dir = dirVec; \
        hit = hitCheckVecFilter(&work, 0); \
        if ((firstPhase) && *(u16*)((s32)player + 0x2E) == 0x18) return 1; \
        if (hit != 0) break; \
        if (!(firstPhase)) { \
            outPos[0] = *(f32*)((s32)player + 0x8C); \
            outPos[1] = *(f32*)((s32)player + 0x90); \
            outPos[2] = *(f32*)((s32)player + 0x94); \
        } \
        movePos((moveDistance), (moveAngleValue), outPos, outPos + 2); \
        work.dist = float_150_80421594; \
        groundVec = (groundTemplate); \
        downVec = (downTemplate); \
        groundVec.x = outPos[0]; \
        groundVec.y = outPos[1] + float_37_804215b0; \
        groundVec.z = outPos[2]; \
        work.start = groundVec; \
        work.dir = downVec; \
        hit = hitCheckVecFilter(&work, 0); \
        if (hit == 0) { valid = 0; } \
        else if ((hitGetAttr(hit) & 0xA00) != 0) { valid = 0; } \
        else { \
            if (hit == 0) { found = 0; } \
            else if ((hitGetAttr(hit) & 0x80000000) == 0) { found = 0; } \
            else { \
                namePtr = dat_ptrarr_80314fe8; \
                mobj = mobjHitObjPtrToPtr(hit); \
                found = 0; \
                while (*namePtr != 0) { \
                    if (strcmp(mobj->name, *namePtr) == 0) { found = 1; break; } \
                    namePtr++; \
                } \
            } \
            if (found) valid = 0; else valid = 1; \
        } \
        if (valid && work.dist <= float_100_804215dc) { \
            outPos[1] = work.hitPos.y; \
            return 1; \
        } \
    } while (0)

    player = *(void**)((s32)party + 0x160);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    if (*(u16*)((s32)player + 0x2E) == 0x18) {
        if (marioPlaneGetFlyingDir() == 1) {
            oppositeDirection = float_0_804215b8;
            baseDirection = float_180_804215ac;
        } else {
            oppositeDirection = float_180_804215ac;
            baseDirection = float_0_804215b8;
        }
    } else {
        oppositeDirection = (f32)revise360(float_180_804215ac + *(f32*)((s32)player + 0x1AC));
        baseDirection = *(f32*)((s32)player + 0x1AC);
    }
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);

    if (*(s8*)((s32)player + 0x245) >= 0) {
        if (*(s8*)((s32)player + 0x246) < 0) {
            TRY_APPEAR_ANGLE((f32)toMovedir(oppositeDirection),
                             (f32)toMovedir(baseDirection), -float_30_804215c0,
                             vec3_802cc1cc, vec3_802cc1d8, vec3_802cc1e4, vec3_802cc1f0, 1);
        } else {
            TRY_APPEAR_ANGLE((f32)toMovedir(oppositeDirection),
                             (f32)toMovedir2(baseDirection,
                                 *(s8*)((s32)party + 0x2F) == 0 ?
                                     float_neg30_804215e0 : float_30_804215c0),
                             -float_30_804215c0,
                             vec3_802cc1fc, vec3_802cc208, vec3_802cc214, vec3_802cc220, 1);
        }
    }
    if (*(s8*)((s32)player + 0x245) >= 0) {
        if (*(s8*)((s32)player + 0x246) < 0) {
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_neg90_804215d4),
                             (f32)toMovedir2(baseDirection, float_90_80421598),
                             -float_30_804215c0,
                             vec3_802cc22c, vec3_802cc238, vec3_802cc244, vec3_802cc250, 0);
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_90_80421598),
                             (f32)toMovedir2(baseDirection, float_neg90_804215d4),
                             -float_30_804215c0,
                             vec3_802cc25c, vec3_802cc268, vec3_802cc274, vec3_802cc280, 0);
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_0_804215b8),
                             (f32)toMovedir2(baseDirection, float_0_804215b8),
                             float_30_804215c0,
                             vec3_802cc28c, vec3_802cc298, vec3_802cc2a4, vec3_802cc2b0, 0);
        } else {
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_neg90_804215d4),
                             (f32)toMovedir2(baseDirection,
                                 *(s8*)((s32)party + 0x2F) == 0 ?
                                     float_70_804215e4 : float_110_804215e8),
                             -float_30_804215c0,
                             vec3_802cc2bc, vec3_802cc2c8, vec3_802cc2d4, vec3_802cc2e0, 0);
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_90_80421598),
                             (f32)toMovedir2(baseDirection,
                                 *(s8*)((s32)party + 0x2F) == 0 ?
                                     float_neg110_804215ec : float_neg70_804215f0),
                             -float_30_804215c0,
                             vec3_802cc2ec, vec3_802cc2f8, vec3_802cc304, vec3_802cc310, 0);
            TRY_APPEAR_ANGLE((f32)toMovedir2(baseDirection, float_0_804215b8),
                             (f32)toMovedir2(baseDirection,
                                 *(s8*)((s32)party + 0x2F) == 0 ?
                                     float_70_804215e4 : float_neg70_804215f0),
                             -float_30_804215c0,
                             vec3_802cc31c, vec3_802cc328, vec3_802cc334, vec3_802cc340, 0);
        }
    }
#undef TRY_APPEAR_ANGLE
    return 0;
}

/* stub-fill: partyGetAppearPos2 | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyGetAppearPos2(void* party, f32* outPos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern f32 revise360(f32);
    extern f32 toMovedir(f32);
    extern f32 toMovedir2(f32, f32);
    extern void movePos(f32, f32, f32*, f32*);
    extern void* hitCheckVecFilter(void*, void*);
    extern const VecLocal vec3_802cc34c, vec3_802cc358, vec3_802cc364, vec3_802cc370;
    extern const VecLocal vec3_802cc37c, vec3_802cc388, vec3_802cc394, vec3_802cc3a0;
    extern const VecLocal vec3_802cc3ac, vec3_802cc3b8, vec3_802cc3c4, vec3_802cc3d0;
    extern const f32 float_38_804215d0;
    extern const f32 float_neg90_804215d4;
    extern const f32 float_90_80421598;
    extern const f32 float_neg20_804215d8;
    extern const f32 float_20_8042159c;
    void* player;
    HitWork work;
    const volatile VecLocal* templates = &vec3_802cc34c;
    VecLocal firstStart0, firstDir0, secondStart0;
    VecLocal firstStart1, firstDir1, secondStart1;
    VecLocal firstStart2, firstDir2, secondStart2;
    f32 sine;
    f32 cosine;
    f32 direction;
    f32 width;
    f32 angle;
    f32 side;

    player = *(void**)((s32)party + 0x160);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    revise360(float_180_804215ac + *(f32*)((s32)player + 0x1AC));
    direction = *(f32*)((s32)player + 0x1AC);
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);

    if (*(s8*)((s32)player + 0x246) < 0) {
        angle = (f32)toMovedir(float_180_804215ac + direction);
        sincosf(angle, &sine, &cosine);
        work.dist = width;
        firstStart0 = templates[0];
        firstStart0.x = outPos[0];
        firstStart0.y = float_37_804215b0 + outPos[1];
        firstStart0.z = outPos[2];
        *(VecLocal*)work.start = firstStart0;
        firstDir0 = templates[1];
        firstDir0.x = sine;
        firstDir0.z = cosine;
        *(VecLocal*)work.dir = firstDir0;
        if (hitCheckVecFilter(&work, 0) == 0) {
            movePos(-float_30_804215c0, toMovedir(*(f32*)((s32)player + 0x1AC)), outPos, outPos + 2);
            secondStart0 = templates[2];
            secondStart0.x = outPos[0];
            secondStart0.y = float_37_804215b0 + outPos[1];
            secondStart0.z = outPos[2];
            *(VecLocal*)work.start = secondStart0;
            work.dir[0] = 0.0f;
            work.dir[1] = -1.0f;
            work.dir[2] = 0.0f;
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }
    } else {
        *(VecLocal*)outPos = *(VecLocal*)((s32)player + 0x8C);
        angle = (f32)toMovedir2(direction, float_neg90_804215d4);
        sincosf(angle, &sine, &cosine);
        work.dist = width;
        firstStart1 = templates[4];
        firstStart1.x = *(f32*)((s32)player + 0x8C);
        firstStart1.y = float_37_804215b0 + *(f32*)((s32)player + 0x90);
        firstStart1.z = *(f32*)((s32)player + 0x94);
        *(VecLocal*)work.start = firstStart1;
        firstDir1 = templates[5];
        firstDir1.x = sine;
        firstDir1.y = 0.0f;
        firstDir1.z = cosine;
        *(VecLocal*)work.dir = firstDir1;
        if (hitCheckVecFilter(&work, 0) == 0) {
            *(VecLocal*)outPos = *(VecLocal*)((s32)player + 0x8C);
            side = *(s8*)((s32)party + 0x2F) == 0 ? float_20_8042159c : float_neg20_804215d8;
            movePos(-float_30_804215c0, toMovedir2(direction, side), outPos, outPos + 2);
            secondStart1 = templates[6];
            secondStart1.x = outPos[0];
            secondStart1.y = float_37_804215b0 + outPos[1];
            secondStart1.z = outPos[2];
            *(VecLocal*)work.start = secondStart1;
            *(VecLocal*)work.dir = templates[7];
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }

        angle = (f32)toMovedir2(direction, float_90_80421598);
        sincosf(angle, &sine, &cosine);
        work.dist = width;
        firstStart2 = templates[8];
        firstStart2.x = *(f32*)((s32)player + 0x8C);
        firstStart2.y = float_37_804215b0 + *(f32*)((s32)player + 0x90);
        firstStart2.z = *(f32*)((s32)player + 0x94);
        *(VecLocal*)work.start = firstStart2;
        firstDir2 = templates[9];
        firstDir2.x = sine;
        firstDir2.z = cosine;
        *(VecLocal*)work.dir = firstDir2;
        if (hitCheckVecFilter(&work, 0) == 0) {
            outPos[0] = *(f32*)((s32)player + 0x8C);
            outPos[1] = *(f32*)((s32)player + 0x90);
            outPos[2] = *(f32*)((s32)player + 0x94);
            side = *(s8*)((s32)party + 0x2F) == 0 ? float_neg20_804215d8 : float_20_8042159c;
            movePos(-float_30_804215c0, toMovedir2(direction, side), outPos, outPos + 2);
            secondStart2 = templates[10];
            secondStart2.x = outPos[0];
            secondStart2.y = float_37_804215b0 + outPos[1];
            secondStart2.z = outPos[2];
            *(VecLocal*)work.start = secondStart2;
            *(VecLocal*)work.dir = templates[11];
            work.dist = float_38_804215d0;
            if (hitCheckVecFilter(&work, 0) != 0) {
                outPos[1] = work.hitPos[1];
                return 1;
            }
        }
    }
    return 0;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

/* stub-fill: partyGetAppearPos3 | missing_definition | ghidra_signature */
u8 partyGetAppearPos3(void* party, f32* outPos) {
    typedef struct VecLocal { f32 x,y,z; } VecLocal;
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;

    extern f32 toMovedir2(f32,f32);
    extern void movePos(f32,f32,f32*,f32*);
    extern void* hitCheckVecFilter(void*,void*);
    extern const VecLocal vec3_802cc40c, vec3_802cc424;
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern f32 partyNrmToAngle(f32* normal);
    extern f32 PSVECDistance(void* a, void* b);
    extern void unk_800c27c0(void* party, f32* outPos, f32 arg2, f32 arg3, f32 width, f32 height);
    extern const f32 float_2_804215cc;
    extern const f32 float_0_804215b8;
    extern const f32 float_60_804215c8;
    extern const f32 float_neg1_804215c4;

    s32 angles[9] = { 45, 135, -135, 315, -315, 180, -180, 90, -90 };
    void* player;
    HitWork work;
    s32 angleStack[9];
    VecLocal normalCopy;
    VecLocal checkStart, checkDir, secondStart;
    MobjName* mobj;
    char** names;
    f32 sine;
    f32 cosine;
    f32 direction;
    f32 width;
    f32 angle;
    f32 bestDist;
    f32 bestX;
    f32 bestY;
    f32 bestZ;
    u32 i;
    s32* angleIter;
    void* hit;
    void* npc;
    s32 blocked;

    angleStack[0] = angles[0];
    angleStack[1] = angles[1];
    angleStack[2] = angles[2];
    angleStack[3] = angles[3];
    angleStack[4] = angles[4];
    angleStack[5] = angles[5];
    angleStack[6] = angles[6];
    angleStack[7] = angles[7];
    angleStack[8] = angles[8];

    player = *(void**)((s32)party + 0x160);
    direction = *(f32*)((s32)player + 0x1B0);
    width = float_30_804215c0 + *(f32*)((s32)party + 0xF4);
    outPos[0] = *(f32*)((s32)player + 0x8C);
    outPos[1] = *(f32*)((s32)player + 0x90);
    outPos[2] = *(f32*)((s32)player + 0x94);
    bestDist = float_neg1_804215c4;

    angleIter = angleStack;
    for (i = 0; i < 9; i++, angleIter++) {
        angle = (f32)toMovedir2(direction, (f32)*angleIter);
        sincosf(angle, &sine, &cosine);

        checkStart = *(VecLocal*)((s32)player + 0x8C);
        checkStart.y = float_37_804215b0 + *(f32*)((s32)player + 0x90);
        checkDir = *(const volatile VecLocal*)&vec3_802cc40c;
        checkDir.x = sine;
        checkDir.z = cosine;
        *(VecLocal*)work.start = checkStart;
        *(VecLocal*)work.dir = checkDir;
        work.dist = width;

        if (hitCheckVecFilter(&work, 0) != 0) {
            continue;
        }

        outPos[0] = *(f32*)((s32)player + 0x8C);
        outPos[1] = *(f32*)((s32)player + 0x90);
        outPos[2] = *(f32*)((s32)player + 0x94);
        movePos(float_30_804215c0, angle, outPos, outPos + 2);

        secondStart = *(VecLocal*)outPos;
        secondStart.y = float_37_804215b0 + outPos[1];
        *(VecLocal*)work.start = secondStart;
        *(VecLocal*)work.dir = *(const volatile VecLocal*)&vec3_802cc424;
        work.dist = float_87_804215b4;

        hit = (void*)hitCheckVecFilter(&work, 0);
        if (hit == 0) {
            continue;
        }

        blocked = 0;
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            mobj = mobjHitObjPtrToPtr(hit);
            names = dat_ptrarr_80314fe8;
            while (*names != 0) {
                if (strcmp(mobj->name, *names) == 0) {
                    blocked = 1;
                    break;
                }
                names++;
            }
        }
        if (blocked) {
            continue;
        }

        normalCopy = *(VecLocal*)work.normal;

        if (partyNrmToAngle((f32*)&normalCopy) <= float_60_804215c8) {
            s32 valid;
            blocked = 0;
            if (hit == 0) valid = 0;
            else if ((hitGetAttr(hit) & 0xA00) != 0) valid = 0;
            else {
                if (hit != 0 && (hitGetAttr(hit) & 0x80000000) != 0) {
                    mobj = mobjHitObjPtrToPtr(hit);
                    names = dat_ptrarr_80314fe8;
                    while (*names != 0) {
                        if (strcmp(mobj->name, *names) == 0) {
                            blocked = 1;
                            break;
                        }
                        names++;
                    }
                }
                valid = blocked ? 0 : 1;
            }
            if (valid) {
                    npc = npcNearDistCheck(
                        outPos[0],
                        outPos[1],
                        outPos[2],
                        *(f32*)((s32)party + 0xF4));

                    if (npc == 0) {
                        outPos[1] = work.hitPos[1];
                        unk_800c27c0(
                            party,
                            outPos,
                            float_2_804215cc,
                            float_0_804215b8,
                            *(f32*)((s32)party + 0xF4),
                            *(f32*)((s32)party + 0xF0));
                        return 1;
                    }

                    if (bestDist < PSVECDistance((void*)((s32)npc + 0x8C), outPos)) {
                        bestDist = PSVECDistance((void*)((s32)npc + 0x8C), outPos);
                        bestX = outPos[0];
                        bestY = outPos[1];
                        bestZ = outPos[2];
                    }
            }
        }
    }

    if (bestDist >= float_0_804215b8) {
        outPos[0] = bestX;
        outPos[1] = bestY;
        outPos[2] = bestZ;
        return 1;
    }

    return 0;
}

/* stub-fill: partyGetAppearPos4 | missing_definition | ghidra_signature */
u8 partyGetAppearPos4(void* party, f32* outPos) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern void* anotherPartyGetPtr(s32 slot);
    extern f32 revise360(f32 angle);
    extern f32 toMovedir2(f32 angle, f32 offset);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f32 distance, f32 angle, f32* x, f32* z);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern MobjName* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(char* a, char* b);
    extern char* dat_ptrarr_80314fe8[];
    extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern u8 partyGetAppearPos(void* party, f32* pos);
    extern const f32 float_26_804215a8;
    extern const f32 float_28_804215a4;
    extern const f32 float_32_804215a0;
    extern const f32 float_37_804215b0;
    extern const f32 float_87_804215b4;
    extern const f32 float_180_804215ac;
    extern const f32 float_5_804215bc;
    static const s32 normalAngles[11] = { 180, 150, -150, 120, -120, 90, -90, 60, -60, 30, -30 };
    s32 normalAnglesWork[11];
    static const s32 partnerAngles[10] = { 150, -150, 120, -120, 90, -90, 60, -60, 30, -30 };
    s32 partnerAnglesWork[10];
    static const s32 alternateAngles[11] = { -150, 150, -120, 120, -90, 90, -60, 60, -30, 30, -180 };
    s32 alternateAnglesWork[11];
    void* player;
    void* other;
    HitWork work;
    VecLocal secondStart, firstDir, firstStart;
    extern const VecLocal vec3_802cc528, vec3_802cc540;
    extern const char str_MOBJ_TreasureBox_802cbfc0[];
    const s32 *normalIter, *partnerIter, *alternateIter;
    const s32* angles;
    extern void* gpGlobals;
    extern const char str_usu_00_802cc5b8[];
    extern void unk_800c27c0(void*, f32*, f32, f32, f32, f32);
    extern const s32 dat_802cc484[38];
    s32 doorWork[13], specialWork[13];
    const s32* doorAngles;
    const s32* specialAngles;
    f32 radius;
    f32 width;
    f32 direction;
    f32 angle;
    f32 sine;
    f32 cosine;
    s32 count;
    s32 i;
    u32 j;
    u32 attr;
    MobjName* mobj;
    void* hit;

    normalAnglesWork[0] = normalAngles[0];
    normalAnglesWork[1] = normalAngles[1];
    normalAnglesWork[2] = normalAngles[2];
    normalAnglesWork[3] = normalAngles[3];
    normalAnglesWork[4] = normalAngles[4];
    normalAnglesWork[5] = normalAngles[5];
    normalAnglesWork[6] = normalAngles[6];
    normalAnglesWork[7] = normalAngles[7];
    normalAnglesWork[8] = normalAngles[8];
    normalAnglesWork[9] = normalAngles[9];
    normalAnglesWork[10] = normalAngles[10];
    partnerAnglesWork[0] = partnerAngles[0];
    partnerAnglesWork[1] = partnerAngles[1];
    partnerAnglesWork[2] = partnerAngles[2];
    partnerAnglesWork[3] = partnerAngles[3];
    partnerAnglesWork[4] = partnerAngles[4];
    partnerAnglesWork[5] = partnerAngles[5];
    partnerAnglesWork[6] = partnerAngles[6];
    partnerAnglesWork[7] = partnerAngles[7];
    partnerAnglesWork[8] = partnerAngles[8];
    partnerAnglesWork[9] = partnerAngles[9];
    alternateAnglesWork[0] = alternateAngles[0];
    alternateAnglesWork[1] = alternateAngles[1];
    alternateAnglesWork[2] = alternateAngles[2];
    alternateAnglesWork[3] = alternateAngles[3];
    alternateAnglesWork[4] = alternateAngles[4];
    alternateAnglesWork[5] = alternateAngles[5];
    alternateAnglesWork[6] = alternateAngles[6];
    alternateAnglesWork[7] = alternateAngles[7];
    alternateAnglesWork[8] = alternateAngles[8];
    alternateAnglesWork[9] = alternateAngles[9];
    alternateAnglesWork[10] = alternateAngles[10];

    doorWork[0] = dat_802cc484[12];
    doorWork[1] = dat_802cc484[13];
    doorWork[2] = dat_802cc484[14];
    doorWork[3] = dat_802cc484[15];
    doorWork[4] = dat_802cc484[16];
    doorWork[5] = dat_802cc484[17];
    doorWork[6] = dat_802cc484[18];
    doorWork[7] = dat_802cc484[19];
    doorWork[8] = dat_802cc484[20];
    doorWork[9] = dat_802cc484[21];
    doorWork[10] = dat_802cc484[22];
    doorWork[11] = dat_802cc484[23];
    doorWork[12] = dat_802cc484[24];
    specialWork[0] = dat_802cc484[25];
    specialWork[1] = dat_802cc484[26];
    specialWork[2] = dat_802cc484[27];
    specialWork[3] = dat_802cc484[28];
    specialWork[4] = dat_802cc484[29];
    specialWork[5] = dat_802cc484[30];
    specialWork[6] = dat_802cc484[31];
    specialWork[7] = dat_802cc484[32];
    specialWork[8] = dat_802cc484[33];
    specialWork[9] = dat_802cc484[34];
    specialWork[10] = dat_802cc484[35];
    specialWork[11] = dat_802cc484[36];
    specialWork[12] = dat_802cc484[37];

    player = *(void**)((s32)party + 0x160);
    other = 0;
    if (*(s8*)((s32)party + 0x31) == 5) {
        radius = float_32_804215a0;
    } else if ((*(u32*)((s32)party + 8) & 0x100000) == 0) {
        radius = float_26_804215a8;
    } else {
        radius = float_28_804215a4;
    }
    width = radius + *(f32*)((s32)party + 0xF4);
    direction = (f32)revise360(float_180_804215ac + *(f32*)((s32)party + 0x54));
    if ((*(u32*)party & 0x100) == 0) {
        outPos[0] = *(f32*)((s32)player + 0x8C);
        outPos[1] = *(f32*)((s32)player + 0x90);
        outPos[2] = *(f32*)((s32)player + 0x94);
    } else {
        other = anotherPartyGetPtr(*(s8*)((s32)party + 0x2F));
        if (other != 0) {
            outPos[0] = *(f32*)((s32)other + 0x58);
            outPos[2] = *(f32*)((s32)other + 0x60);
            outPos[1] = *(f32*)((s32)other + 0x5C);
        } else {
            outPos[0] = *(f32*)((s32)player + 0x8C);
            outPos[2] = *(f32*)((s32)player + 0x94);
            outPos[1] = *(f32*)((s32)player + 0x90);
        }
    }
    normalIter = normalAnglesWork - 1;
    partnerIter = partnerAnglesWork - 1;
    alternateIter = alternateAnglesWork - 1;
    doorAngles = doorWork - 1;
    specialAngles = specialWork - 1;
    count = (*(u32*)((s32)party + 8) & 0x100000) ? 13 : 11;
    for (i = 0; i <= count; i++, normalIter++, partnerIter++, alternateIter++, doorAngles++, specialAngles++) {
        work.dist = width;
        if (i == 0) angle = *(f32*)((s32)party + 0x54);
        else if ((*(u32*)((s32)party + 8) & 0x100000) != 0) {
            if (strcmp((char*)gpGlobals + 0x12C, (char*)str_usu_00_802cc5b8) == 0)
                angle = toMovedir2(*(f32*)((s32)party + 0x54), (f32)*specialAngles);
            else angle = toMovedir2(*(f32*)((s32)party + 0x54), (f32)*doorAngles);
        } else if (other == 0) angle = toMovedir2(direction, (f32)*normalIter);
        else if ((*(u32*)party & 0x100) == 0) angle = toMovedir2(direction, (f32)*partnerIter);
        else angle = toMovedir2(direction, (f32)*alternateIter);
        sincosf(angle, &sine, &cosine);
        firstStart = *(VecLocal*)outPos;
        firstStart.y = float_37_804215b0 + outPos[1];
        firstDir = *(const volatile VecLocal*)(str_MOBJ_TreasureBox_802cbfc0 + 0x568);
        firstDir.x = sine;
        firstDir.z = cosine;
        *(VecLocal*)work.start = firstStart;
        *(VecLocal*)work.dir = firstDir;
        if (hitCheckVecFilter(&work, 0) != 0) {
            continue;
        }
        movePos(radius, angle, outPos, outPos + 2);
        secondStart = *(VecLocal*)outPos;
        secondStart.y = float_37_804215b0 + outPos[1];
        *(VecLocal*)work.start = secondStart;
        *(VecLocal*)work.dir = *(const volatile VecLocal*)(str_MOBJ_TreasureBox_802cbfc0 + 0x580);
        work.dist = float_87_804215b4;
        hit = (void*)hitCheckVecFilter(&work, 0);
        if (hit == 0) {
            continue;
        }
        {
            s32 found = 0;
            s32 valid;
            attr = hitGetAttr(hit);
            if ((attr & 0xA00) == 0) {
                if (hit != 0 && (hitGetAttr(hit) & 0x80000000) != 0) {
                    char** names;
                    mobj = mobjHitObjPtrToPtr(hit);
                    names = dat_ptrarr_80314fe8;
                    while (*names != 0) {
                        if (strcmp(mobj->name, *names) == 0) {
                            found = 1;
                            break;
                        }
                        names++;
                    }
                }
                valid = found ? 0 : 1;
            } else valid = 0;
            if (valid && npcNearDistCheck(outPos[0], outPos[1], outPos[2], *(f32*)((s32)party + 0xF4)) == 0) {
            outPos[1] = work.hitPos[1];
            unk_800c27c0(party, outPos, float_0_804215b8, float_0_804215b8, float_28_804215a4, float_5_804215bc);
            if (*(u8*)((s32)party + 0x32) == 1 && outPos[1] < *(f32*)((s32)player + 0x90)) {
                outPos[1] = *(f32*)((s32)player + 0x90) + float_5_804215bc;
            }
            return 1;
            }
        }
    }
    partyGetAppearPos(party, outPos);
    if (*(u8*)((s32)party + 0x32) == 1 && outPos[1] < *(f32*)((s32)player + 0x90))
        outPos[1] = float_5_804215bc + *(f32*)((s32)player + 0x90);
    return 0;
}

/* stub-fill: partyGetAppearPos5 | missing_definition | ghidra_signature */
u8 partyGetAppearPos5(void* pParty, void* hitObj, f32* outPos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct MobjLocal {
        u8 pad[0x38];
        VecLocal position;
    } MobjLocal;
    extern MobjLocal* mobjHitObjPtrToPtr(void* hit);
    extern s32 partyHitCheck(void* party, void* pos, void* dir, f32* hitPos, void* normal, f32* distance);
    extern const f32 float_50_80421590;
    extern const VecLocal vec3_802cc594;
    extern const VecLocal vec3_802cc5a0;
    static const s32 angles[12] = { 0, 30, -30, 60, -60, 90, -90, 120, -120, 150, -150, 180 };
    s32 angleStack[12];
    MobjLocal* mobj;
    VecLocal pos;
    VecLocal dir;
    VecLocal dirTemp;
    VecLocal hitPos;
    VecLocal normal;
    f32 distance;
    f32 sine;
    f32 cosine;
    u32 i;
    s32* angle;
    void* hit;

    angleStack[0] = angles[0];
    angleStack[1] = angles[1];
    angleStack[2] = angles[2];
    angleStack[3] = angles[3];
    angleStack[4] = angles[4];
    angleStack[5] = angles[5];
    angleStack[6] = angles[6];
    angleStack[7] = angles[7];
    angleStack[8] = angles[8];
    angleStack[9] = angles[9];
    angleStack[10] = angles[10];
    angleStack[11] = angles[11];
    if (hitObj == 0 || (hitGetAttr(hitObj) & 0x80000000) == 0) {
        return 0;
    }
    mobj = mobjHitObjPtrToPtr(hitObj);
    pos.x = mobj->position.x;
    pos.y = mobj->position.y + float_50_80421590;
    pos.z = mobj->position.z;
    i = 0;
    angle = angleStack;
    do {
        distance = float_50_80421590;
        sincosf(*(f32*)((s32)pParty + 0x100) + (f32)*angle, &sine, &cosine);
        dirTemp = *(const volatile VecLocal*)&vec3_802cc594;
        dirTemp.x = sine;
        dirTemp.z = cosine;
        dir = dirTemp;
        if (partyHitCheck(pParty, &pos, &dir, (f32*)&hitPos, &normal, &distance) == 0) {
            pos.x = mobj->position.x + sine * float_50_80421590;
            pos.z = mobj->position.z + cosine * float_50_80421590;
            dir = vec3_802cc5a0;
            distance = float_150_80421594;
            hit = (void*)partyHitCheck(pParty, &pos, &dir, (f32*)&hitPos, &normal, &distance);
            if (hit == 0 || (hitGetAttr(hit) & 0x200) == 0) {
                break;
            }
        }
        i++;
        angle++;
    } while (i < 12);
    if (i < 12) {
        outPos[0] = pos.x;
        outPos[1] = pos.y;
        outPos[2] = pos.z;
        return 1;
    }
    return 0;
}

const char str_MOBJ_TreasureBox_802cbfc0[] = "MOBJ_TreasureBox";
const char str_MOBJ_BigTreasureBox_802cbfd4[] = "MOBJ_BigTreasureBox";
const char str_MOBJ_GrayTreasureBox_802cbfe8[] = "MOBJ_GrayTreasureBox";
const char str_MOBJ_BlackTreasureBo_802cc000[] = "MOBJ_BlackTreasureBox";
const char str_MOBJ_GoldenTreasureB_802cc018[] = "MOBJ_GoldenTreasureBox";
const char str_MOBJ_BlueJumpStand_802cc030[] = "MOBJ_BlueJumpStand";
const char str_MOBJ_BlueJumpStand2_802cc044[] = "MOBJ_BlueJumpStand2";
const char str_MOBJ_RedJumpStand_802cc058[] = "MOBJ_RedJumpStand";
const char str_MOBJ_RedJumpStand2_802cc06c[] = "MOBJ_RedJumpStand2";
const char str_MOBJ_MapJumpStand_802cc080[] = "MOBJ_MapJumpStand";
const char str_MOBJ_SaveBlock_802cc094[] = "MOBJ_SaveBlock";
const char str_MOBJ_RecoveryBlock_802cc0a4[] = "MOBJ_RecoveryBlock";
const char str_MOBJ_CoinTakenRecove_802cc0b8[] = "MOBJ_CoinTakenRecoveryBlock";
const char str_MOBJ_eki_06_su1_802cc0d4[] = "MOBJ_eki_06_su1";
const char str_MOBJ_eki_06_su2_802cc0e4[] = "MOBJ_eki_06_su2";
const char str_MOBJ_eki_06_su3_802cc0f4[] = "MOBJ_eki_06_su3";
const char str_MOBJ_RedSwitch_802cc104[] = "MOBJ_RedSwitch";
const char str_MOBJ_BlueSwitch_802cc114[] = "MOBJ_BlueSwitch";
const char str_MOBJ_WhiteSwitch_802cc124[] = "MOBJ_WhiteSwitch";
const char str_MOBJ_BlackSwitch_802cc138[] = "MOBJ_BlackSwitch";
const char str_MOBJ_green_FlowBlock_802cc14c[] = "MOBJ_green_FlowBlockSwitch";
const char str_MOBJ_purple_FlowBloc_802cc168[] = "MOBJ_purple_FlowBlockSwitch";
const char str_MOBJ_orange_FlowBloc_802cc184[] = "MOBJ_orange_FlowBlockSwitch";
const char str_MOBJ_RedTimerSwitch_802cc1a0[] = "MOBJ_RedTimerSwitch";
const char str_MOBJ_BlueTimerSwitch_802cc1b4[] = "MOBJ_BlueTimerSwitch";

const PartyAppearVec3 vec3_802cc1cc = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc1d8 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc1e4 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc1f0 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc1fc = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc208 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc214 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc220 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc22c = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc238 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc244 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc250 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc25c = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc268 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc274 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc280 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc28c = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc298 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2a4 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2b0 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2bc = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2c8 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2d4 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2e0 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2ec = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc2f8 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc304 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc310 = { 0.0f, -1.0f, 0.0f };
const PartyAppearVec3 vec3_802cc31c = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc328 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc334 = { 0.0f, 0.0f, 0.0f };
const PartyAppearVec3 vec3_802cc340 = { 0.0f, -1.0f, 0.0f };

const char str_usu_00_802cc5b8[] = "usu_00";

const f32 float_50_80421590 = 50.0f;
const f32 float_150_80421594 = 150.0f;
const f32 float_90_80421598 = 90.0f;
const f32 float_20_8042159c = 20.0f;
const f32 float_32_804215a0 = 32.0f;
const f32 float_28_804215a4 = 28.0f;
const f32 float_26_804215a8 = 26.0f;
const f32 float_180_804215ac = 180.0f;
const f32 float_37_804215b0 = 37.0f;
const f32 float_87_804215b4 = 87.0f;
const f32 float_0_804215b8 = 0.0f;
const f32 float_5_804215bc = 5.0f;
const f32 float_30_804215c0 = 30.0f;
const f32 float_neg1_804215c4 = -1.0f;
const f32 float_60_804215c8 = 60.0f;
const f32 float_2_804215cc = 2.0f;
const f32 float_38_804215d0 = 38.0f;
const f32 float_neg90_804215d4 = -90.0f;
const f32 float_neg20_804215d8 = -20.0f;
const f32 float_100_804215dc = 100.0f;
const f32 float_neg30_804215e0 = -30.0f;
const f32 float_70_804215e4 = 70.0f;
const f32 float_110_804215e8 = 110.0f;
const f32 float_neg110_804215ec = -110.0f;
const f32 float_neg70_804215f0 = -70.0f;
const f32 float_1p65_804215f4 = 1.65f;
const f32 float_1_804215f8 = 1.0f;
const f32 float_0p8_804215fc = 0.8f;
const f32 float_10_80421600 = 10.0f;
const f32 float_1p8_80421604 = 1.8f;
const f32 float_0p75_80421608 = 0.75f;
const f32 float_0p05_8042160c = 0.05f;
const f32 float_0p5_80421610 = 0.5f;
const f32 float_0p1_80421614 = 0.1f;
const f32 float_0p4_80421618 = 0.4f;
const f32 float_0p3_8042161c = 0.3f;
const f32 float_0p2_80421620 = 0.2f;
const f32 float_0p15_80421624 = 0.15f;
const f32 float_4_80421628 = 4.0f;
const f32 float_3p1416_8042162c = 3.1415927f;
const f32 float_neg1p4_80421630 = -1.4f;
const f32 float_270_80421634 = 270.0f;
