#include "party/party_bomhei.h"

s32 bomheiGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);

    void* party = partyGetPtr(marioGetPartyId());
    s32 status = 2;

    if (party == NULL || ((s32)*(u8*)((s32)party + 0x31) == 3 && !(*(u32*)party & 0x100))) {
        return 0;
    }

    if (*(u8*)((s32)party + 0x39) == 0x3C) {
        status = 1;
    }
    return status;
}

void bomhei_init(void* party) {
    extern f32 float_28_804213d8;

    *(u8*)((s32)party + 0x33) = 3;
    {
        f32 temp = float_28_804213d8;

        *(u8*)((s32)party + 0x32) = 0;
        *(f32*)((s32)party + 0xF0) = temp;
        *(f32*)((s32)party + 0xF4) = temp;
    }
    *(u8*)((s32)party + 0x17A) = 100;
    *(s32*)((s32)party + 0x17C) = -1;
    *(s32*)((s32)party + 0x180) = -1;
    *(s32*)((s32)party + 0x184) = -1;
}


u8 bomhei_use_main(void* party) {
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void __memFree(s32, void*);
    extern void partyChgPose(void*, char*);
    extern f64 toMovedirSimple(f64);
    extern void movePos(f32, f32, f32*, f32*);
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern void partyMoveNoHosei(void*);
    extern void partyMove(void*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void psndSFXOff(s32);
    extern void partyGetAppearPos(void*, void*);
    extern void partyChgRunMode(void*, s32);
    void* player = *(void**)((s32)party + 0x160);
    void* work;
    u8 state;

    if ((*(u32*)((s32)party + 8) & 0x1000) != 0) {
        *(u32*)((s32)party + 8) &= ~0x1000;
        work = __memAlloc(0, 0x20);
        *(void**)((s32)party + 0x164) = work;
        memset(work, 0, 0x20);
        *(s32*)((s32)work + 0x1C) = -1;
        *(u8*)((s32)party + 0x38) = 0;
    }
    work = *(void**)((s32)party + 0x164);
    state = *(u8*)((s32)party + 0x38);
    switch (state) {
    case 0:
        partyChgPose(party, (char*)0x802CB7C8);
        *(f32*)((s32)party + 0x104) = (f32)toMovedirSimple(*(f32*)((s32)player + 0x1AC));
        *(f32*)((s32)party + 0x100) = 3.0f;
        *(s32*)((s32)party + 0x9C) = 20;
        *(u8*)((s32)party + 0x38) = 1;
        *(s32*)((s32)work + 0x1C) = psndSFXOn_3D(0x3F0, (void*)((s32)party + 0x58));
        break;
    case 1:
        movePos(*(f32*)((s32)party + 0x100), *(f32*)((s32)party + 0x104),
                (f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60));
        partyMove(party);
        if (--*(s32*)((s32)party + 0x9C) < 1) {
            *(u8*)((s32)party + 0x38) = 2;
            *(f32*)((s32)party + 0xFC) = 8.0f;
            *(f32*)((s32)party + 0x108) = 0.5f;
        }
        break;
    case 2:
        partyMoveNoHosei(party);
        *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0xFC);
        *(f32*)((s32)party + 0xFC) -= *(f32*)((s32)party + 0x108);
        if (*(f32*)((s32)party + 0xFC) <= 0.0f) *(u8*)((s32)party + 0x38) = 3;
        break;
    case 3:
        if (*(s32*)((s32)work + 0x1C) != -1) psndSFXOff(*(s32*)((s32)work + 0x1C));
        partyGetAppearPos(party, (void*)((s32)party + 0xB8));
        *(u8*)((s32)party + 0x38) = 4;
        break;
    case 4:
        if (work != 0) { __memFree(0, work); *(void**)((s32)party + 0x164) = 0; }
        partyChgRunMode(party, 1);
        break;
    }
    return 0;
}

void partyGetReturnPos(f32* out) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    extern void* marioGetPtr(void);
    extern f32 toMovedirSimple(f32 angle);
    extern f32 revise360(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void movePos(f32 distance, f32 angle, f32* x, f32* z);
    extern s32 hitCheckFilter(f32, f32, f32, f32, f32, f32, s32,
                              void*, void*, void*, void*, void*, void*, void*);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern const f32 float_0_804212dc;
    extern const f32 float_0p5_80421308;
    extern const f32 float_neg0p5_8042130c;
    extern const f32 float_2_804212fc;
    extern const f32 float_4_80421304;
    extern const f32 float_6_80421300;
    extern const f32 float_1000_80421310;
    extern const f32 float_180_80421314;
    void* player = marioGetPtr();
    HitWork work;
    f32 sinv;
    f32 cosv;
    f32 x = *(f32*)((s32)player + 0x8C);
    f32 y = *(f32*)((s32)player + 0x90);
    f32 z = *(f32*)((s32)player + 0x94);
    f32 radius = *(f32*)((s32)player + 0x1B8);
    f32 facing = *(f32*)((s32)player + 0x19C);
    s32 i;

    for (i = 0; i < 3; i++) {
        f32 angle;
        f32 distance;
        f32 wallDist = radius;
        if (i == 0) {
            angle = facing;
            distance = float_6_80421300;
        } else if (i == 1) {
            angle = (f32)revise360(toMovedirSimple(*(f32*)((s32)player + 0x1B0)) -
                                         float_180_80421314);
            distance = radius;
        } else {
            angle = (f32)toMovedirSimple(*(f32*)((s32)player + 0x1B0));
            distance = radius;
        }
        sincosf(angle, &sinv, &cosv);
        if (hitCheckFilter(x, y + float_2_804212fc, z, sinv, float_0_804212dc,
                           cosv, 0, 0, 0, 0, &wallDist, 0, 0, 0) == 0) {
            f32 testX = x;
            f32 testZ = z;
            movePos(distance, angle, &testX, &testZ);
            work.start[0] = testX;
            work.start[1] = y + float_2_804212fc;
            work.start[2] = testZ;
            work.dir[0] = float_0_804212dc;
            work.dir[1] = -1.0f;
            work.dir[2] = float_0_804212dc;
            work.dist = float_4_80421304;
            {
                void* hit = hitCheckVecFilter(&work, 0);
                if (hit != 0 && (hitGetAttr(hit) & 0xE00) == 0) {
                    f32 round;
                    round = work.hitPos[0] < float_0_804212dc ?
                            float_neg0p5_8042130c : float_0p5_80421308;
                    out[0] = (f32)(s32)(float_1000_80421310 * work.hitPos[0] + round) /
                             float_1000_80421310;
                    round = work.hitPos[1] < float_0_804212dc ?
                            float_neg0p5_8042130c : float_0p5_80421308;
                    out[1] = (f32)(s32)(float_1000_80421310 * work.hitPos[1] + round) /
                             float_1000_80421310;
                    round = work.hitPos[2] < float_0_804212dc ?
                            float_neg0p5_8042130c : float_0p5_80421308;
                    out[2] = (f32)(s32)(float_1000_80421310 * work.hitPos[2] + round) /
                             float_1000_80421310;
                    return;
                }
            }
        }
    }
    facing = (f32)revise360(float_180_80421314 + facing);
    movePos(radius, facing, &x, &z);
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void bomhei_use(void* pParty) {
    extern void bomhei_use_main(void* party);
    extern void partyChkGnd(void* party);
    extern void partyChgMot(void* party, s32 mot);
    extern s32 strcmp(const char* a, const char* b);
    extern char* hitGetName(void* hit);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void mario_controll(void* party);
    extern u32 hitGetAttr(void* hit);
    extern void* mobjHitObjPtrToPtr(void* hit);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void psndSFXOff(s32 sfx);
    extern void __memFree(s32 heap, s32 ptr);
    extern s32 marioKeyOffChk(void);
    extern void* gp;
    extern char str_muj_05_802cb400[];
    extern char str_A_dokuro_bero_802cb408[];
    extern char str_MOBJ_RedJumpStand_802cb418[];
    extern char str_MOBJ_RedJumpStand2_802cb42c[];
    extern char str_MOBJ_BlueJumpStand_802cb440[];
    extern char str_MOBJ_BlueJumpStand2_802cb454[];
    extern f32 float_0_804212dc;
    extern f32 float_638_804213b8;
    extern f32 float_12p5_804213bc;

    void* player;
    void* party;
    void* mobj;
    u8 state;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 0x0) & 0x100) == 0) {
        return;
    }

    bomhei_use_main(pParty);
    state = *(u8*)((s32)pParty + 0x39);
    if ((state >= 0x1E && state <= 0x20) || (state >= 0x32 && state <= 0x33) || state == 0x3C) {
        partyChkGnd(pParty);
        if ((*(u32*)((s32)pParty + 0x0) & 0x20) != 0) {
            partyChgMot(pParty, 0);
            *(f32*)((s32)pParty + 0x114) = float_0_804212dc;
        }
        if (strcmp(*(char**)((s32)gp + 0), str_muj_05_802cb400) == 0 && *(void**)((s32)pParty + 0x138) != 0) {
            if (strcmp(hitGetName(*(void**)((s32)pParty + 0x138)), str_A_dokuro_bero_802cb408) == 0) {
                *(f32*)((s32)pParty + 0x100) = angleABf(*(f32*)((s32)pParty + 0x58),
                                                        *(f32*)((s32)pParty + 0x60),
                                                        float_638_804213b8,
                                                        float_12p5_804213bc);
                *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)pParty + 0x100);
            }
        }
    }

    if (*(u8*)((s32)pParty + 0x17A) != 0x66) {
        if (*(u16*)((s32)player + 0x2E) == 0x1F || *(u16*)((s32)player + 0x2E) == 0x20) {
            *(u8*)((s32)pParty + 0x17A) = 0x66;
            return;
        }
        mario_controll(pParty);
    }

    if (*(u8*)((s32)pParty + 0x39) < 0xB && *(void**)((s32)player + 0x1EC) != 0 &&
        (hitGetAttr(*(void**)((s32)player + 0x1EC)) & 0x80000000) != 0) {
        mobj = mobjHitObjPtrToPtr(*(void**)((s32)player + 0x1EC));
        if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RedJumpStand_802cb418) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RedJumpStand2_802cb42c) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BlueJumpStand_802cb440) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BlueJumpStand2_802cb454) == 0) {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0 || *(s8*)((s32)party + 0x31) != 3 || (*(u32*)((s32)party + 0x0) & 0x100) == 0) {
                return;
            }
            *(u32*)*(void**)((s32)party + 0x160) &= ~0x4000;
            if (*(u8*)((s32)party + 0x34) != 9) {
                partyChgRunMode(party, 2);
                *(u8*)((s32)party + 0x39) = 0;
            }
            *(u32*)((s32)party + 0x4) &= ~0x10;
            *(u32*)((s32)party + 0x0) &= ~0x02000000;
            *(u32*)((s32)party + 0x0) &= ~0x200;
            *(u32*)((s32)party + 0x0) &= ~0x100;
            if (*(s32*)((s32)party + 0x17C) != -1) {
                psndSFXOff(*(s32*)((s32)party + 0x17C));
                *(s32*)((s32)party + 0x17C) = -1;
            }
            if (*(s32*)((s32)party + 0x180) != -1) {
                psndSFXOff(*(s32*)((s32)party + 0x180));
                *(s32*)((s32)party + 0x180) = -1;
            }
            if (*(void**)((s32)party + 0x184) != 0) {
                __memFree(0, (s32)*(void**)((s32)party + 0x184));
                *(void**)((s32)party + 0x184) = 0;
            }
            return;
        }
    }

    if ((*(u32*)((s32)pParty + 0x0) & 0x200) != 0 || marioKeyOffChk() == 0) {
        return;
    }

    party = partyGetPtr(marioGetPartyId());
    if (party == 0 || *(s8*)((s32)party + 0x31) != 3 || (*(u32*)((s32)party + 0x0) & 0x100) == 0) {
        return;
    }
    *(u32*)*(void**)((s32)party + 0x160) &= ~0x4000;
    if (*(u8*)((s32)party + 0x34) != 9) {
        partyChgRunMode(party, 2);
        *(u8*)((s32)party + 0x39) = 0;
    }
    *(u32*)((s32)party + 0x4) &= ~0x10;
    *(u32*)((s32)party + 0x0) &= ~0x02000000;
    *(u32*)((s32)party + 0x0) &= ~0x200;
    *(u32*)((s32)party + 0x0) &= ~0x100;
    if (*(s32*)((s32)party + 0x17C) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x17C));
        *(s32*)((s32)party + 0x17C) = -1;
    }
    if (*(s32*)((s32)party + 0x180) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x180));
        *(s32*)((s32)party + 0x180) = -1;
    }
    if (*(void**)((s32)party + 0x184) != 0) {
        __memFree(0, (s32)*(void**)((s32)party + 0x184));
        *(void**)((s32)party + 0x184) = 0;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* check_front(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct Probe {
        VecLocal base;
        VecLocal pos;
        VecLocal dir;
        u8 pad[0x18];
        f32 radius;
    } Probe;
    extern f32 partyToMovedir(f32 dir, void* party);
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern void* hitCheckVecFilter(void* work, s32 filter);
    extern void* camGetPtr(s32 camId);
    extern f32 revise360(f32 angle);
    extern u32 hitGetAttr(void* hit);
    extern void* caseCheckHitObj2(u32 hit);
    extern void* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    extern u32 vec3_802cb398[];
    extern char str_MOBJ_BombRock_802cb3dc[];
    extern f32 float_0p8_804212d8;
    extern f32 float_0_804212dc;
    extern f32 float_100_804212e0;
    extern f32 float_60_804212e4;
    extern f32 float_120_804212e8;
    extern f32 float_240_804212ec;
    extern f32 float_300_804212f0;
    extern f32 float_50_804212f4;
    extern f32 float_30_804212f8;
    extern f32 float_2_804212fc;

    Probe probe;
    u32* base;
    void* hit;
    void* savedHit;
    void* camera;
    void* caseObj;
    void* mobj;
    f32 height;
    f32 movedir;
    f32 angle;
    f32 limit;
    s32 i;
    s32 valid;

    base = vec3_802cb398;
    height = float_0p8_804212d8 * *(f32*)((s32)pParty + 0xF0);
    movedir = partyToMovedir(*(f32*)((s32)pParty + 0x10C), pParty);
    savedHit = 0;

    for (i = 0; i < 8; i++) {
        angle = movedir + (f32)((s32)base[i + 6]);
        sincosf(angle, &probe.dir.x, &probe.dir.z);
        probe.dir.y = float_0_804212dc;
        probe.pos.x = *(f32*)((s32)pParty + 0x58);
        probe.pos.y = *(f32*)((s32)pParty + 0x5C) + height;
        probe.pos.z = *(f32*)((s32)pParty + 0x60);
        probe.radius = float_100_804212e0;
        hit = hitCheckVecFilter(&probe, 0);
        if (hit != 0) {
            camera = camGetPtr(0);
            angle = revise360(angle - *(f32*)((s32)camera + 0x114));
            limit = float_50_804212f4;
            if (((angle < float_60_804212e4) || (float_120_804212e8 < angle)) &&
                ((angle < float_240_804212ec) || (float_300_804212f0 < angle))) {
                limit = float_30_804212f8;
            }
            if (probe.radius <= limit) {
                valid = 0;
                if ((hitGetAttr(hit) & 0x80000000) == 0) {
                    caseObj = caseCheckHitObj2((u32)hit);
                    if (caseObj != 0 && *(s32*)((s32)caseObj + 4) == 0xF) {
                        valid = 1;
                    }
                } else {
                    mobj = mobjHitObjPtrToPtr(hit);
                    if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BombRock_802cb3dc) == 0) {
                        valid = 1;
                    }
                }
                if (valid != 0) {
                    return hit;
                }
            }
        }
    }

    probe.pos.x = *(f32*)((s32)pParty + 0x58);
    probe.pos.y = *(f32*)((s32)pParty + 0x5C);
    probe.pos.z = *(f32*)((s32)pParty + 0x60);
    probe.radius = float_2_804212fc + height;
    probe.dir.x = *(f32*)((s32)base + 0x38);
    probe.dir.y = *(f32*)((s32)base + 0x3C);
    probe.dir.z = *(f32*)((s32)base + 0x40);
    hit = hitCheckVecFilter(&probe, 0);
    if (hit != 0) {
        savedHit = hit;
        valid = 0;
        if ((hitGetAttr(hit) & 0x80000000) == 0) {
            caseObj = caseCheckHitObj2((u32)hit);
            if (caseObj != 0 && *(s32*)((s32)caseObj + 4) == 0xF) {
                valid = 1;
            }
        } else {
            mobj = mobjHitObjPtrToPtr(hit);
            if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BombRock_802cb3dc) == 0) {
                valid = 1;
            }
        }
        if (valid != 0) {
            return hit;
        }
    }

    hit = *(void**)((s32)pParty + 0x138);
    if (hit != 0) {
        valid = 0;
        if ((hitGetAttr(hit) & 0x80000000) == 0) {
            caseObj = caseCheckHitObj2((u32)hit);
            if (caseObj != 0 && *(s32*)((s32)caseObj + 4) == 0xF) {
                valid = 1;
            }
        } else {
            mobj = mobjHitObjPtrToPtr(hit);
            if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BombRock_802cb3dc) == 0) {
                valid = 1;
            }
        }
        if (valid != 0) {
            return hit;
        }
    }

    return savedHit;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mario_controll(void* pParty) {
    extern void marioAdjustMoveDir(void);
    extern void marioChgPose(const char* pose);
    extern int strcmp(const char* s1, const char* s2);
    extern int marioAnimeId(void);
    extern void animPoseSetLocalTime(float time, int poseId);
    extern void marioChgMot(int mot);
    extern const char str_M_U_2_80421318[6];
    extern const char str_M_W_7_80421320[6];
    extern const char str_M_C_1_80421328[6];

    void* player;
    unsigned char state;
    int frame;
    int timer;

    player = *(void**)((int)pParty + 0x160);
    state = *(unsigned char*)((int)pParty + 0x17A);

    switch (state) {
        case 0xA:
            marioAdjustMoveDir();
            marioChgPose(str_M_U_2_80421318);
            *(unsigned int*)player |= 0x2000;
            *(unsigned char*)((int)pParty + 0x17A) = 0x65;
            break;

        case 0x14:
            *(unsigned int*)player |= 0x2000;
            *(unsigned char*)((int)pParty + 0x17A) = 0x15;
            break;

        case 0x15:
            if (strcmp(*(const char**)((int)player + 0x18), str_M_W_7_80421320) == 0) {
                animPoseSetLocalTime((float)*(unsigned char*)((int)pParty + 0x17B),
                                      *(int*)((int)player + 0x22C + (marioAnimeId() << 2)));
                frame = *(unsigned char*)((int)pParty + 0x17B) + 1;
                *(unsigned char*)((int)pParty + 0x17B) = frame;
                if ((unsigned char)frame >= 0x28) {
                    *(unsigned char*)((int)pParty + 0x17B) = 0;
                }
            } else {
                *(unsigned char*)((int)pParty + 0x17B) = 0;
            }
            break;

        case 0x28:
            marioChgMot(0);
            *(unsigned char*)((int)pParty + 0x17A) = 0x29;
            *(short*)((int)pParty + 0x178) = 0x28;
            break;

        case 0x29:
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer <= 0) {
                marioChgMot(0);
                *(unsigned char*)((int)pParty + 0x17A) = 0x64;
            }
            break;

        case 0x1E:
            marioChgMot(0);
            *(unsigned char*)((int)pParty + 0x17A) = 0x1F;
            break;

        case 0x1F:
            marioChgPose(str_M_C_1_80421328);
            *(unsigned int*)player &= ~0x2000;
            *(short*)((int)pParty + 0x178) = 0x32;
            *(unsigned char*)((int)pParty + 0x17A) = 0x20;
            break;

        case 0x20:
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer <= 0) {
                *(unsigned char*)((int)pParty + 0x17A) = 0x24;
                *(short*)((int)pParty + 0x178) = 10;
            }
            break;

        case 0x24:
            animPoseSetLocalTime((float)*(short*)((int)pParty + 0x178),
                                  *(int*)((int)player + 0x22C + (marioAnimeId() << 2)));
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer < 0) {
                marioChgMot(0);
                *(unsigned char*)((int)pParty + 0x17A) = 0x64;
            }
            break;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 bomhei_use_post(void* party) {
    extern void partyGetAppearPos(void*, void*);
    extern void marioChgPose(char*);
    extern void psndSFXOff(s32);
    extern void __memFree(s32, void*);
    extern char str_M_W_1_804213c0;
    extern char str_M_R_1_804213c8;
    extern char str_M_S_1_804213d0;
    extern char str_M_J_1B_802cb468[];
    void* player = *(void**)((s32)party + 0x160);
    char* pose;

    if (*(u8*)((s32)party + 0x39) > 0x45 && *(u8*)((s32)party + 0x39) < 100) {
        partyGetAppearPos(party, (void*)((s32)party + 0x58));
    }
    if ((*(u32*)player & 0x2000) != 0) {
        *(u32*)player &= ~0x2000;
        if (*(u16*)((s32)player + 0x2E) == 1) pose = &str_M_W_1_804213c0;
        else if (*(u16*)((s32)player + 0x2E) == 2) pose = &str_M_R_1_804213c8;
        else if ((*(u32*)player & 0x10000) == 0) pose = &str_M_S_1_804213d0;
        else pose = str_M_J_1B_802cb468;
        marioChgPose(pose);
    }
    *(u32*)((s32)party + 4) &= ~0x10;
    *(u32*)party &= ~0x02000000;
    *(u32*)party &= ~0x80000000;
    *(u32*)party &= ~0x100;
    if (*(u8*)((s32)party + 0x34) != 9) {
        *(u8*)((s32)party + 0x3B) = 0;
        *(u32*)((s32)party + 8) |= 8;
    }
    *(u32*)*(void**)((s32)party + 0x160) &= ~0x2000;
    if (*(s32*)((s32)party + 0x180) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x180));
        *(s32*)((s32)party + 0x180) = -1;
    }
    if (*(s32*)((s32)party + 0x184) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x184));
        *(s32*)((s32)party + 0x184) = -1;
    }
    if (*(void**)((s32)party + 0x170) != 0) {
        __memFree(0, *(void**)((s32)party + 0x170));
        *(void**)((s32)party + 0x170) = 0;
    }
    return 0;
}

u8 bomhei_use_cancel2(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void marioChgPose(char*);
    extern void partyChgRunMode(void*, s32);
    extern void psndSFXOff(s32);
    extern void __memFree(s32, void*);
    extern char str_M_S_1_804213d0;
    void* party = partyGetPtr(marioGetPartyId());
    void* player;

    if (party == 0 || *(s8*)((s32)party + 0x31) != 3 || (*(u32*)party & 0x100) == 0) return 0;
    player = *(void**)((s32)party + 0x160);
    if ((*(u32*)player & 0x2000) == 0) return 0;
    *(u32*)player &= ~0x2000;
    marioChgPose(&str_M_S_1_804213d0);
    if (*(u8*)((s32)party + 0x34) != 9) {
        partyChgRunMode(party, 2);
        *(u8*)((s32)party + 0x3B) = 0;
    }
    *(u32*)((s32)party + 4) &= ~0x10;
    *(u32*)party &= ~0x02000000;
    *(u32*)party &= ~0x80000000;
    *(u32*)party &= ~0x100;
    if (*(s32*)((s32)party + 0x180) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x180));
        *(s32*)((s32)party + 0x180) = -1;
    }
    if (*(s32*)((s32)party + 0x184) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x184));
        *(s32*)((s32)party + 0x184) = -1;
    }
    if (*(void**)((s32)party + 0x170) != 0) {
        __memFree(0, *(void**)((s32)party + 0x170));
        *(void**)((s32)party + 0x170) = 0;
    }
    return 0;
}

u8 bomhei_exit(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyChgRunMode(void*, s32);
    extern void psndSFXOff(s32);
    extern void __memFree(s32, void*);
    void* party;

    *(u32*)*(void**)((s32)pParty + 0x160) &= ~0x2000;
    party = partyGetPtr(marioGetPartyId());
    if (party == 0 || *(s8*)((s32)party + 0x31) != 3 || (*(u32*)party & 0x100) == 0) return 0;
    *(u32*)*(void**)((s32)party + 0x160) &= ~0x2000;
    if (*(u8*)((s32)party + 0x34) != 9) {
        partyChgRunMode(party, 2);
        *(u8*)((s32)party + 0x3B) = 0;
    }
    *(u32*)((s32)party + 4) &= ~0x10;
    *(u32*)party &= ~0x02000000;
    *(u32*)party &= ~0x80000000;
    *(u32*)party &= ~0x100;
    if (*(s32*)((s32)party + 0x180) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x180));
        *(s32*)((s32)party + 0x180) = -1;
    }
    if (*(s32*)((s32)party + 0x184) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x184));
        *(s32*)((s32)party + 0x184) = -1;
    }
    if (*(void**)((s32)party + 0x170) != 0) {
        __memFree(0, *(void**)((s32)party + 0x170));
        *(void**)((s32)party + 0x170) = 0;
    }
    return 0;
}

u8 bomhei_use_cancel(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyChgRunMode(void*, s32);
    extern void psndSFXOff(s32);
    extern void __memFree(s32, void*);
    void* party = partyGetPtr(marioGetPartyId());

    if (party == 0 || *(s8*)((s32)party + 0x31) != 3 || (*(u32*)party & 0x100) == 0) return 0;
    *(u32*)*(void**)((s32)party + 0x160) &= ~0x2000;
    if (*(u8*)((s32)party + 0x34) != 9) {
        partyChgRunMode(party, 2);
        *(u8*)((s32)party + 0x3B) = 0;
    }
    *(u32*)((s32)party + 4) &= ~0x10;
    *(u32*)party &= ~0x02000000;
    *(u32*)party &= ~0x80000000;
    *(u32*)party &= ~0x100;
    if (*(s32*)((s32)party + 0x180) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x180));
        *(s32*)((s32)party + 0x180) = -1;
    }
    if (*(s32*)((s32)party + 0x184) != -1) {
        psndSFXOff(*(s32*)((s32)party + 0x184));
        *(s32*)((s32)party + 0x184) = -1;
    }
    if (*(void**)((s32)party + 0x170) != 0) {
        __memFree(0, *(void**)((s32)party + 0x170));
        *(void**)((s32)party + 0x170) = 0;
    }
    return 0;
}

s32 mario_bomhei_keychk(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || (state >= 0x14 && state <= 0x15) || state == 0x65 || (state >= 0x28 && state <= 0x29) || (state >= 0x1E && state <= 0x24)) return 0;
    state = *(u8*)((s32)party + 0x39);
    if ((state >= 0x32 && state <= 0x33) || state == 0x3C || (state >= 0x46 && state <= 0x47)) return 0;
    return 1;
}

u8 bomhei_bye(void* pParty) {
    extern void partyGoodbyeInit(void*);
    extern s32 partyGoodbyeMain(void*);
    extern void partyChgRunMode(void*, s32);
    extern void psndSFXOff(s32);

    if ((*(u32*)((s32)pParty + 8) & 8) != 0) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyGoodbyeInit(pParty);
        *(u32*)*(void**)((s32)pParty + 0x160) &= ~0x2000;
        if (*(s32*)((s32)pParty + 0x180) != -1) {
            psndSFXOff(*(s32*)((s32)pParty + 0x180));
            *(s32*)((s32)pParty + 0x180) = -1;
        }
        if (*(s32*)((s32)pParty + 0x184) != -1) {
            psndSFXOff(*(s32*)((s32)pParty + 0x184));
            *(s32*)((s32)pParty + 0x184) = -1;
        }
    }
    if (partyGoodbyeMain(pParty) != 0) {
        partyChgRunMode(pParty, 0xE);
    }
    return 0;
}

s32 mario_bomhei_keychk2(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || state == 0x65 || (state >= 0x1E && state <= 0x24)) return 0;
    return 1;
}

