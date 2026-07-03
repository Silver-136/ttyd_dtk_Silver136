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
int unk_800ccb34() {
    return 0;
}
