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


u8 party_motion_damage(void* pParty) {
    return 0;
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
