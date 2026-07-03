#include "party/party_nokonoko.h"

extern s32 nokoSe;
extern f32 float_37_80421678;
extern f32 float_24_80421708;

void partyGoodbyeInit(void* party);
s32 partyGoodbyeMain(void* party);
void partyChgRunMode(void* party, s32 mode);
s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

void nokotaro_hold_cancel(void* party) {
    *(u8*)((s32)party + 0x39) = 0x14;
}

void nokonoko_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 2;
    height = float_37_80421678;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_24_80421708;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    nokoSe = -1;
}

void nokonoko_bye(void* party) {
    if (*(u32*)((s32)party + 8) & 8) {
        *(u32*)((s32)party + 8) &= ~8;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party) != 0) {
        partyChgRunMode(party, 0xE);
    }
}

s32 nokonoko_holdItem(void) {
    void* party = partyGetPtr(marioGetPartyId());
    s32 item;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 2 || !(*(u32*)party & 0x100)) {
        return 0;
    }

    item = *(s32*)((s32)party + 0x174);
    return ((u32)(-item) | (u32)item) >> 31;
}


u8 nokonoko_use(void* party) {
    return 0;
}


u8 ccwall(void* party, s32 param_2) {
    return 0;
}


u8 lookupSafetyPos(void) {
    return 0;
}


void lookupSafetyPos2(float height, float* inPos, float* outPos) {
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6, void* out7);

    int outA;
    int outB;
    int outC;
    int outD;
    int outE;
    int outF;
    float limit[2];
    void* player;
    float radius;
    float x;
    float y;
    float z;
    float probeLimit;
    int hit;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    probeLimit = height + 0.1f;

    x = inPos[0] + radius * (float)sin(1.5708);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(1.5708);
    limit[0] = probeLimit;
    hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x; outPos[1] = y; outPos[2] = z;
    } else {
        x = inPos[0] + radius * (float)sin(4.7124);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(4.7124);
        limit[0] = probeLimit;
        hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x; outPos[1] = y; outPos[2] = z;
        } else {
            sin(3.1416);
            cos(3.1416);
            x = inPos[0];
            y = inPos[1];
            z = inPos[2];
            limit[0] = probeLimit;
            hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
            if (hit != 0) {
                outPos[0] = x; outPos[1] = y; outPos[2] = z;
            } else {
                x = inPos[0] + radius * (float)sin(0.0);
                y = inPos[1];
                z = inPos[2] + radius * -(float)cos(0.0);
                limit[0] = probeLimit;
                hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
                if (hit != 0) {
                    outPos[0] = x; outPos[1] = y; outPos[2] = z;
                } else {
                    outPos[0] = inPos[0]; outPos[1] = inPos[1]; outPos[2] = inPos[2];
                }
            }
        }
    }
}

void lookupSafetyPos3(float height, float* inPos, float* outPos) {
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6, void* out7);

    int outA;
    int outB;
    int outC;
    int outD;
    int outE;
    int outF;
    float limit[2];
    void* player;
    float radius;
    float x;
    float y;
    float z;
    int hit;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    x = inPos[0] + radius * (float)sin(1.5708);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(1.5708);
    limit[0] = height + 0.1f;
    hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0,
                         &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x;
        outPos[1] = y;
        outPos[2] = z;
    } else {
        x = inPos[0] + radius * (float)sin(4.7124);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(4.7124);
        limit[0] = height + 0.1f;
        hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0,
                             &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x;
            outPos[1] = y;
            outPos[2] = z;
        } else {
            outPos[0] = inPos[0];
            outPos[1] = inPos[1];
            outPos[2] = inPos[2];
        }
    }
}

u8 lookupSafetyPosSub(void) {
    return 0;
}


void nokonoko_finish(void* party) {
    extern void itemNokoForceGet(s32 item);
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);
    u32 item;
    f32 height;
    u32 flags;

    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) != 2) {
        return;
    }
    if (!(*(u32*)party & 0x100)) {
        return;
    }

    item = *(u32*)((s32)party + 0x174);
    if (item != 0) {
        itemNokoForceGet(item);
        *(s32*)((s32)party + 0x174) = 0;
    }
    marioGetPtr();
    if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
    height = float_37_80421678;
    *(u32*)party &= ~0x100;
    *(u32*)party &= ~0x04000000;
    *(u32*)party &= ~0x01000000;
    *(u32*)party &= ~0x02000000;
    *(u32*)((s32)party + 4) &= ~0x20;
    *(u32*)((s32)party + 4) &= ~0x10;
    *(f32*)((s32)party + 0xF0) = height;
    *(s32*)((s32)party + 0x178) = 0;
    *(s32*)((s32)party + 0x17C) = 0;
    *(s32*)((s32)party + 0x180) = 0;

    if (*(u8*)((s32)party + 0x34) != 9) {
        flags = *(u32*)party;
        if (!(flags & 0x80000000)) {
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x3B) = 0;
        } else {
            *(u32*)party = flags & 0x7FFFFFFF;
            partyChgRunMode(party, 0);
        }
    }
}


void nokonoko_use_post(void* party) {
    extern void itemNokoForceGet(s32 item);
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);
    u32 item;
    f32 height;

    item = *(u32*)((s32)party + 0x174);
    if (item != 0) {
        itemNokoForceGet(item);
        *(s32*)((s32)party + 0x174) = 0;
    }
    marioGetPtr();
    if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
    height = float_37_80421678;
    *(u32*)party &= ~0x100;
    *(u32*)party &= ~0x04000000;
    *(u32*)party &= ~0x01000000;
    *(u32*)party &= ~0x02000000;
    *(u32*)((s32)party + 4) &= ~0x20;
    *(u32*)((s32)party + 4) &= ~0x10;
    *(f32*)((s32)party + 0xF0) = height;
    *(s32*)((s32)party + 0x178) = 0;
    *(s32*)((s32)party + 0x17C) = 0;
    *(s32*)((s32)party + 0x180) = 0;
}

s32 nokonokoGetStatus(void* party) {
    u8 state;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 2 || !(*(u32*)party & 0x100)) {
        return 0;
    }

    state = *(u8*)((s32)party + 0x39);
    if (state <= 2) {
        return 1;
    }
    if (state >= 0xA && state <= 0xD) {
        return 2;
    }
    if (state >= 0x14 && state <= 0x15) {
        return 3;
    }
    if (state >= 0x32 && state <= 0x47) {
        return 4;
    }
    if (state >= 0x50 && state <= 0x64) {
        return 5;
    }
    if (state >= 0x6E && state <= 0x72) {
        return 6;
    }
    return 7;
}

void nokonoko_move(void* party) {
    extern void* marioGetPtr(void);
    extern void partyWalkInit(void* party, s32 param);
    extern void unk_800cbeb0(void* party);
    extern void partyRecordFootmark(void* party);
    extern void party_move_mode_stay(void* party);
    extern void party_move_mode_walk(void* party);
    extern void party_move_mode_behind_mario(void* party);
    extern void party_move_mode_beside_mario(void* party);

    marioGetPtr();
    if (*(u32*)((s32)party + 8) & 8) {
        *(u32*)((s32)party + 8) &= ~8;
        partyWalkInit(party, 1);
    }
    unk_800cbeb0(party);
    partyRecordFootmark(party);
    switch (*(u8*)((s32)party + 0x3B)) {
        case 0:
            party_move_mode_stay(party);
            break;
        case 1:
            party_move_mode_walk(party);
            break;
        case 2:
            party_move_mode_behind_mario(party);
            break;
        case 3:
            party_move_mode_beside_mario(party);
            break;
        case 4:
        default:
            break;
    }
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_nokonoko 20260624_191429 */

/* fallback stub-fill: map=unk_800d058c addr=0x800d058c size=0x000001a8 */
void unk_800d058c(void) {
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);

    void* party;
    unsigned char state;
    int status;
    float height;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    status = 0;
    if (party != 0) {
        if (*(signed char*)((int)party + 0x31) == 2 && (*(unsigned int*)party & 0x100) != 0) {
            state = *(unsigned char*)((int)party + 0x39);
            status = 7;
            if (state < 3) {
                status = 1;
            } else if (state >= 0xA && state <= 0xD) {
                status = 2;
            } else if (state >= 0x14 && state <= 0x15) {
                status = 3;
            } else if (state >= 0x32 && state <= 0x47) {
                status = 4;
            } else if (state >= 0x50 && state <= 0x64) {
                status = 5;
            } else if (state >= 0x6E && state <= 0x72) {
                status = 6;
            }
        }
    }
    if (status != 0) {
        if ((*(unsigned int*)party & 0x100) != 0) {
            marioGetPtr();
            if ((unsigned int)(nokoSe + 0x10000) != 0xFFFF) {
                psndSFXOff(nokoSe);
                nokoSe = -1;
            }
            height = float_37_80421678;
            *(unsigned int*)party &= ~0x100;
            *(unsigned int*)party &= ~0x04000000;
            *(unsigned int*)party &= ~0x01000000;
            *(unsigned int*)party &= ~0x02000000;
            *(unsigned int*)((int)party + 4) &= ~0x20;
            *(unsigned int*)((int)party + 4) &= ~0x10;
            *(float*)((int)party + 0xF0) = height;
            *(int*)((int)party + 0x178) = 0;
            *(int*)((int)party + 0x17C) = 0;
            *(int*)((int)party + 0x180) = 0;
        }
        if (*(unsigned char*)((int)party + 0x34) != 8 && *(unsigned char*)((int)party + 0x34) != 0xD) {
            partyChgRunMode(party, 2);
            *(unsigned char*)((int)party + 0x3B) = 0;
        }
    }
}
