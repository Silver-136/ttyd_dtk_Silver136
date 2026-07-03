#include "party/party_motion.h"

extern f32 float_60_8042318c;
extern f32 float_0_804230bc;
extern f32 float_3_804230dc;
extern f32 float_270_80423104;
extern f32 float_180_80423108;
extern f32 unk_8041ec08;
extern s32 unk_8041ec0c;
f32 touch_wall_ct;

void* camGetPtr(s32 id);
f32 revise360(f32 angle);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void* partyGetPtr(s32 id);
void partyPaperOff(void* party);
void partyChgPoseId(void* party, s32 poseId);
void partyChgMot(void* party, s32 mot);

#define PARTY_CAN_FORCE_SLIT_OFF(party) \
    (((*(u32*)(party) & 0x1000) != 0) || ((*(u32*)((s32)(party) + 4) & 0x100) != 0))

#define PARTY_FORCE_SLIT_OFF_BODY(party) \
    do { \
        *(f32*)((s32)(party) + 0x10C) = \
            revise360(*(f32*)((s32)(party) + 0x100) - *(f32*)((s32)(party) + 0xF8)) >= float_180_80423108 \
                ? float_0_804230bc \
                : float_180_80423108; \
        { \
            f32 dir270 = float_270_80423104; \
        *(f32*)((s32)(party) + 0x110) = *(f32*)((s32)(party) + 0x10C); \
            *(f32*)((s32)(party) + 0xFC) = revise360(dir270 - *(f32*)((s32)(party) + 0x10C)); \
        } \
        *(f32*)((s32)(party) + 0x100) = *(f32*)((s32)(party) + 0xFC); \
        *(u32*)((s32)(party) + 4) &= ~0x3C0; \
        partyPaperOff(party); \
        *(u32*)(party) &= ~0x3000; \
        *(u8*)((s32)(party) + 0x37) = 0; \
        partyChgPoseId(party, 1); \
    } while (0)

void N_partyFollowCloseOff(void* party) {
    *(u32*)((s32)party + 8) &= ~0x20000;
}

void N_partyFollowCloseOn(void* party) {
    *(u32*)((s32)party + 8) |= 0x20000;
}

void unk_80151d20(void) {
    touch_wall_ct = float_60_8042318c;
}

void partyChgMotSlitFloor(void* party) {
    if (!(*(u32*)party & 0x2000)) {
        void* unk = *(void**)((s32)party + 0x160);

        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
        *(u32*)party |= 0x2000;
    }
}

void party_set_behind_dir(void* party, f32 dir) {
    void* cam = camGetPtr(4);

    *(f32*)((s32)party + 0x54) = revise360(dir + *(f32*)((s32)cam + 0x114));
    *(u32*)((s32)party + 8) |= 0x100000;
}

void allPartySlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }
}


u8 party_motion_homing_walk(s32 pParty) {
    return 0;
}


u8 party_motion_homing_fly(s32 pParty) {
    return 0;
}


u8 party_motion_jump(s32 pParty) {
    return 0;
}


u8 party_motion_behind_mario(s32 pParty) {
    return 0;
}


u8 party_motion_stay(s32 pParty) {
    return 0;
}


s32 party_slit(void* pParty) {
    return 0;
}


u8 party_motion_behind_mario_fly(s32 pParty) {
    return 0;
}


u8 party_motion_behind_mario_continue(s32 pParty) {
    return 0;
}


u8 party_force_reset_outofscreen(void* pParty) {
    return 0;
}


u8 party_motion_beside_mario(s32 pParty) {
    return 0;
}


u8 getFrontFloor(void) {
    return 0;
}


s32 party_roll(void* pParty) {
    return 0;
}


s32 party_motion_fall(void* pParty) {
    return 0;
}


u8 party_motion_jump2(void* pParty) {
    return 0;
}


void party_motion_damage(void* pParty) {
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void marioGetScreenPos(void* pos, f32* x, f32* y, f32* z);
    extern void partyGetReturnPos(void* pos);
    extern void unk_800cbf84(void* party);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    extern f32 float_0_804230bc;
    extern f32 float_neg1_804230c8;
    extern f32 float_neg6_804230fc;
    extern f32 float_15_80423130;
    extern f32 float_neg300_80423134;
    extern f32 float_240_80423138;
    extern f32 float_200_804230a8;
    extern f32 float_neg2000_804230cc;
    void* player;
    f32 posA[3];
    f32 posB[3];
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    u8 state;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    state = *(u8*)((s32)pParty + 0x38);
    switch (state) {
        case 0:
            partyChgPoseId(pParty, 8);
            *(f32*)((s32)pParty + 0x114) = float_15_80423130;
            *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
            *(s32*)((s32)pParty + 0x24) = 0x3C;
            *(u8*)((s32)pParty + 0x38) = 1;
        case 1:
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            posA[0] = *(f32*)((s32)pParty + 0x58);
            posA[1] = *(f32*)((s32)pParty + 0x5C);
            posA[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posA, &x, &y, &z);
            if (y <= float_neg300_80423134) {
                *(s32*)((s32)pParty + 0x24) = 0x14;
                *(u8*)((s32)pParty + 0x38) = 0xA;
            }
            break;
        case 10:
            if (*(u16*)((s32)player + 0x2E) != 0x1F && *(u16*)((s32)player + 0x2E) != 0x20) {
                timer = *(s32*)((s32)pParty + 0x24) - 1;
                *(s32*)((s32)pParty + 0x24) = timer;
                if (timer <= 0) {
                    partyGetReturnPos(posB);
                    *(f32*)((s32)pParty + 0x58) = posB[0];
                    *(f32*)((s32)pParty + 0x5C) = posB[1] + float_200_804230a8;
                    *(f32*)((s32)pParty + 0x60) = posB[2];
                    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                    *(f32*)((s32)pParty + 0x118) = float_neg1_804230c8;
                    unk_800cbf84(pParty);
                    *(u32*)pParty |= 0x20;
                    *(u32*)pParty |= 0x02000000;
                    *(u8*)((s32)pParty + 0x38) = 0xB;
                }
            }
            break;
        case 11:
            posB[0] = *(f32*)((s32)pParty + 0x58);
            posB[1] = *(f32*)((s32)pParty + 0x5C);
            posB[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posB, &x, &y, &z);
            if (y > float_240_80423138) {
                *(u32*)pParty &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_804230fc) {
                *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
                *(f32*)((s32)pParty + 0x114) = float_neg6_804230fc;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
                *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
                *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
                *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
                *(f32*)((s32)pParty + 0x5C) += float_200_804230a8;
                *(u32*)pParty &= ~0xE0;
            }
            if (*(void**)((s32)pParty + 0x138) != 0 && (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x200) == 0) {
                *(u8*)((s32)pParty + 0x38) = 0xC;
                *(s32*)((s32)pParty + 0x24) = 0x1E;
            }
            break;
        case 12:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer <= 0) {
                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                *(u32*)((s32)pParty + 4) &= ~0x18;
            }
            break;
    }
}

void unk_8014c330(void* party) {
    if (party != 0) {
        if (*(u8*)((s32)party + 0x37) == 1) {
            if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
                PARTY_FORCE_SLIT_OFF_BODY(party);
            }
        }
        *(u8*)((s32)party + 0x37) = 0;
    }
}


void L_partyForceSlitOff(void* party) {
    if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}


void U_allPartySlitOffReq(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }
}


void allPartyForceSlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}


void party_motion_upstairs(void* party) {
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        f32 divisor;
        *(u32*)((s32)party + 8) &= ~2;
        divisor = float_3_804230dc;
        *(u32*)party |= 0x40;
        unk_8041ec0c = 3;
        unk_8041ec08 = (*(f32*)((s32)party + 0x120) - *(f32*)((s32)party + 0x5C)) / divisor;
    }
    *(f32*)((s32)party + 0x5C) += unk_8041ec08;
    unk_8041ec0c--;
    if (unk_8041ec0c <= 0) {
        *(u32*)party &= ~0x40;
        partyChgMot(party, 1);
    }
}


void allPartySlitOn(void) {
    void* party = partyGetPtr(marioGetPartyId());
    void* unk;

    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_motion 20260624_191429 */

/* fallback stub-fill: map=unk_80150fac addr=0x80150fac size=0x00000334 */
int unk_80150fac() {
    return 0;
}

/* fallback stub-fill: map=unk_801512e0 addr=0x801512e0 size=0x0000018c */
void unk_801512e0(void* pParty) {
    extern void* partySearchHead(void* party, void* pos, f32* height, f32 dir);
    extern void* partySearchGround(void* party, f32 yOffHi, f32 yOffLo);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern s32 unk_800cb9bc(void* hit);
    extern void partyGetAppearPos5(void* party, void* hit, void* pos);
    extern f32 float_15_80423130;
    extern f32 float_neg15_8042319c;
    extern f32 float_neg1p5_804231a0;
    f32 pos[3];
    f32 height;
    void* hit;
    u32 attr;

    pos[0] = *(f32*)((s32)pParty + 0x58);
    pos[1] = *(f32*)((s32)pParty + 0x5C);
    pos[2] = *(f32*)((s32)pParty + 0x60);
    height = *(f32*)((s32)pParty + 0xF0);
    hit = partySearchHead(pParty, pos, &height, *(f32*)((s32)pParty + 0x10C));
    if (hit != 0) {
        *(void**)((s32)pParty + 0x140) = hit;
    }
    if ((*(u32*)pParty & 0x04000000) == 0) {
        *(u32*)pParty &= ~0x100;
        hit = partySearchGround(pParty, float_15_80423130, float_neg15_8042319c);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x200) != 0) {
                hit = 0;
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            *(u32*)pParty &= ~4;
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1p5_804231a0;
        } else {
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
            *(u32*)pParty |= 2;
            if ((attr & 0x100) != 0) {
                *(u32*)pParty |= 0x80;
            } else {
                *(u32*)pParty &= ~0x100;
            }
            if ((attr & 0x40000000) != 0) {
                *(void**)((s32)pParty + 0x14C) = hit;
            }
            if (unk_800cb9bc(hit) != 0) {
                partyGetAppearPos5(pParty, hit, (void*)((s32)pParty + 0x94));
                *(u32*)((s32)pParty + 8) |= 0x00400000;
                *(f32*)((s32)pParty + 0x98) = *(f32*)((s32)pParty + 0x5C);
                partyChgMot(pParty, 2);
            }
        }
    }
}

/* fallback stub-fill: map=unk_8015146c addr=0x8015146c size=0x000001e0 */
void unk_8015146c(void* party) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void partyClearFootmark(void);
    extern void partyClearFootmark2(void);
    extern void party_slit(void* party);
    extern void party_roll(void* party);
    extern void unk_800cbc30(void* party);
    extern void partyChkWall(void* party);
    extern void partyChkGnd(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void partyMoveBehindMario(void* party, s32 arg);
    extern void unk_80150fac(void* party);
    extern void unk_801512e0(void* party);
    extern s32 marioKeyOffChk(void);
    extern s32 marioChkDeepSleep(void);
    extern s32 chuchu_searchObject(void);
    extern void partyChgPose(void* party, char* name);
    extern char str_PCH_A2_1_802f46f4[];
    extern f32 float_31_804231a4;
    void* player;
    s32 timer;
    u8 state;

    player = *(void**)((s32)party + 0x160);
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        *(u32*)((s32)party + 8) &= ~2;
        partyChgPoseId(party, 1);
        if ((*(u32*)party & 8) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        *(s32*)((s32)party + 0x168) = -1;
    }
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if ((*(u32*)party & 0x01000000) == 0) {
        partyChkWall(party);
    }
    partyChkGnd(party);
    if (distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) > float_31_804231a4) {
        partyChgRunMode(party, 2);
    }
    if (*(u16*)((s32)player + 0x2E) == 0xE && (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
    } else if (*(u8*)((s32)party + 0x32) == 1) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        if (*(u8*)((s32)party + 0x31) == 7 && (*(u32*)((s32)party + 4) & 0x8000) == 0 && marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 && chuchu_searchObject() != 0) {
            state = *(u8*)((s32)party + 0x39);
            if (state == 1) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    partyChgPoseId(party, 1);
                    *(s32*)((s32)party + 0x24) = 0x1E;
                    *(u8*)((s32)party + 0x39) = 2;
                }
            } else if (state < 3 && state != 0) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    *(u8*)((s32)party + 0x39) = 0;
                }
            } else {
                partyChgPose(party, str_PCH_A2_1_802f46f4);
                *(u8*)((s32)party + 0x39) = 1;
                *(s32*)((s32)party + 0x24) = 0x1E;
            }
        }
    }
}

