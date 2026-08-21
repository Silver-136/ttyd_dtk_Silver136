#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party/party.h"
#include "party/party_motion.h"
#include "party/party_move.h"
#include "party/party_vivian.h"
#include "pmario_sound.h"
#include "system.h"

extern s32 vivian_cancel_req;
extern f32 float_35_804242d8;
extern f32 float_24_804242bc;
extern f32 float_10_80424270;
extern void marioPaperOff(void);
extern char str_p_bibi_802f898c[];
extern char str_PM_B_1_802f8994[];
extern f32 float_0p1_804242cc;
extern f32 float_30_804242d0;

u8 N_marioForceVivianAnime(void) {
    extern void marioChgPaper(void* name);
    extern void marioPaperOn(void* name);
    extern void marioChgPose(void* pose);
    extern s32 marioAnimeId(void);
    extern void animPoseSetLocalTime(void* pose, f32 time);
    extern char str_M_B_3_804242a4;

    void* mario = marioGetPtr();
    s32 animeId;
    u16 frame;

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(str_p_bibi_802f898c);
    marioChgPose(&str_M_B_3_804242a4);
    marioChgPaper(str_PM_B_1_802f8994);
    *(u16*)((s32)mario + 0x28) = 0x18;
    animeId = marioAnimeId();
    frame = *(u16*)((s32)mario + 0x28);
    animPoseSetLocalTime(*(void**)((s32)mario + 0x22C + animeId * 4), (f32)frame);
}

u8 mot_vivian(void) {
    extern f32 float_0_80424274;

    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        zero = float_0_80424274;
        *(u32*)mario &= ~0xF0000;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        *(s32*)((s32)mario + 0x1E4) = 0;
        *(s32*)((s32)mario + 0x1E0) = 0;
        *(s32*)((s32)mario + 0x1F0) = 0;
        *(s32*)((s32)mario + 0x1F4) = 0;
    }
}

u8 mot_vivian_post(void) {
    extern void partyPaperOff(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void partyChgPoseId(void* party, s32 pose);
    extern s32 vec3_802f8948[];
    extern s32 vec3_802f8954[];
    void* party;
    void* mario;

    party = partyGetPtr(marioGetPartyId());
    mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x20 || *(u16*)((s32)mario + 0x2E) == 0x1F) {
        *(u32*)((s32)mario + 4) &= ~0x1000;
        marioPaperOff();
        *(s32*)((s32)mario + 0xA4) = vec3_802f8948[0];
        *(s32*)((s32)mario + 0xA8) = vec3_802f8948[1];
        *(s32*)((s32)mario + 0xAC) = vec3_802f8948[2];
        *(u32*)mario &= ~0x400;
        if (party != 0) {
            partyPaperOff(party);
            *(u32*)party &= ~0x100;
            *(u32*)party &= 0x7FFFFFFF;
            *(u32*)((s32)party + 4) &= ~0x200000;
            *(f32*)((s32)party + 0x174) = float_10_80424270;
            *(s32*)((s32)party + 0x7C) = vec3_802f8954[0];
            *(s32*)((s32)party + 0x80) = vec3_802f8954[1];
            *(s32*)((s32)party + 0x84) = vec3_802f8954[2];
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x3B) = 0;
            partyChgPoseId(party, 1);
            if (*(s8*)((s32)party + 0x31) == 6) {
                *(u8*)((s32)party + 0x4F) &= ~1;
            }
        }
    } else if (party != 0 && *(s8*)((s32)party + 0x31) == 6 && (*(u32*)party & 0x100)) {
        vivian_cancel_req = 1;
    }
}

void vivian_init(void* party) {
    f32 value;
    f32 offset;
    f32 scale;
    s32 six = 6;
    s32 one = 1;
    s32 zero = 0;
    s32 pos1;
    s32 pos0;

    *(u8*)((s32)party + 0x33) = six;
    scale = float_35_804242d8;
    *(u8*)((s32)party + 0x32) = one;
    offset = float_24_804242bc;
    *(f32*)((s32)party + 0xF0) = scale;
    value = float_10_80424270;
    *(f32*)((s32)party + 0xF4) = offset;
    *(f32*)((s32)party + 0x174) = value;
    *(u8*)((s32)party + 0x4F) = zero;
    pos1 = *(s32*)((s32)party + 0x5C);
    pos0 = *(s32*)((s32)party + 0x58);
    *(s32*)((s32)party + 0xB8) = pos0;
    *(s32*)((s32)party + 0xBC) = pos1;
    *(s32*)((s32)party + 0xC0) = *(s32*)((s32)party + 0x60);
    *(s32*)((s32)party + 0x184) = zero;
    vivian_cancel_req = zero;
}


s32 vivianGetStatus(void) {

    void* party = partyGetPtr(marioGetPartyId());
    u8 status;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 6 || !(*(u32*)party & 0x100)) {
        return 0;
    }
    status = *(u8*)((s32)party + 0x39);
    if (status < 10) {
        return 1;
    }
    if (status == 10) {
        return 2;
    }
    if (status >= 20 && status <= 27) {
        return 3;
    }
    return 4;
}

void vivianUnhold(void* party) {
    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) != 6) {
        return;
    }
    if (*(u32*)party & 0x100) {
        if (*(u8*)((s32)party + 0x39) == 10) {
            *(u8*)((s32)party + 0x39) = 0x14;
        }
    }
}

void L_partyVivianTailStop(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        *(u8*)((s32)party + 0x4F) |= 1;
    }
}

void L_partyVivianTailStart(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        s32 flags = *(u8*)((s32)party + 0x4F);
        *(u8*)((s32)party + 0x4F) = flags & ~1;
    }
}


s32 N_vivianDisplayTailCheck(void* pParty) {
    extern void* gp;
    extern s32 strcmp(char* a, char* b);
    extern char str_bom_01_802f89b0[];
    extern f32 float_neg1000_804242d4;

    if (*(s8*)((s32)pParty + 0x31) != 6) {
        return 0;
    }
    if (*(u8*)((s32)pParty + 0x4F) & 1) {
        return 0;
    }
    if (*(u32*)((s32)pParty + 4) & 0x00800000) {
        return 0;
    }
    if (strcmp((char*)((s32)gp + 0x12C), str_bom_01_802f89b0) == 0 &&
        float_neg1000_804242d4 == *(f32*)((s32)pParty + 0x5C)) {
        return 0;
    }
    return 1;
}


u8 vivianShadow(void* pParty) {
    f32 step;
    f32 dx;
    f32 dy;
    f32 dz;

    if (pParty != 0 && *(s8*)((s32)pParty + 0x31) == 6) {
        step = float_0p1_804242cc;
        dx = *(f32*)((s32)pParty + 0x58) - *(f32*)((s32)pParty + 0xB8);
        dy = *(f32*)((s32)pParty + 0x5C) - *(f32*)((s32)pParty + 0xBC);
        dz = *(f32*)((s32)pParty + 0x60) - *(f32*)((s32)pParty + 0xC0);
        dx = step * dx;
        dy = step * dy;
        dz = step * dz;
        *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0xB8) + dx;
        *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0xBC) + dy;
        *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0xC0) + dz;
        if (distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                    *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)) >= float_30_804242d0) {
            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
            movePos((f32*)((s32)pParty + 0xB8), (f32*)((s32)pParty + 0xC0),
                    float_30_804242d0,
                    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)));
        }
    }
}


u8 vivian_move(void* pParty) {
    f32 step;
    f32 dx;
    f32 dy;
    f32 dz;

    if (*(u32*)((s32)pParty + 8) & 8) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyMoveFlyInit(pParty, 1);
    }
    partyFlyMain(pParty);
    if (pParty != 0 && *(s8*)((s32)pParty + 0x31) == 6) {
        step = float_0p1_804242cc;
        dx = *(f32*)((s32)pParty + 0x58) - *(f32*)((s32)pParty + 0xB8);
        dy = *(f32*)((s32)pParty + 0x5C) - *(f32*)((s32)pParty + 0xBC);
        dz = *(f32*)((s32)pParty + 0x60) - *(f32*)((s32)pParty + 0xC0);
        dx = step * dx;
        dy = step * dy;
        dz = step * dz;
        *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0xB8) + dx;
        *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0xBC) + dy;
        *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0xC0) + dz;
        if (distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                    *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)) >= float_30_804242d0) {
            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
            movePos((f32*)((s32)pParty + 0xB8), (f32*)((s32)pParty + 0xC0),
                    float_30_804242d0,
                    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)));
        }
    }
}


void vivian_use(void* pParty) {
    extern void marioChgMot(s32 mot);
    extern void marioPaperOn(char* name);
    extern void marioChgPaper(char* name);
    extern void marioChgPose(char* name);
    extern void partyPaperOn(void* party, char* name);
    extern void partyChgPaper(void* party, char* name);
    extern void partyChgPose(void* party, char* name);
    extern f32 toMovedirSimple(f32 dir);
    extern f32 toMovedir(f32 dir);
    extern void marioGetScreenPos(f32* position, f32* x, f32* y, f32* z);
    extern void* marioSearchGround(f32 width, f32 depth, f32* groundY,
                                   f32* outX, f32* outZ);
    extern s32 strcmp(const char* a, const char* b);
    extern void* gp;
    extern void unk_800c27c0(void* party, f32 zero, void* playerPos,
                             f32 direction, f32 value, f32 half);
    extern void* effRippleEntry(void);
    extern void effRippleSetCamId(void* effect, s32 camId);
    extern void effRippleSetPosition(void* effect, f32 x, f32 y, f32 z);
    extern void partyUpdateKeyData(void* party);
    extern void marioAdjustMoveDir(void);
    /*
     * Keep the target-shaped local prototype here. The target calls this as
     * time in FPR1 + poseId in GPR3; using f64 first reproduces the target's
     * integer-to-double conversion shape for timer values.
     */
    extern void animPoseSetLocalTime(f64 time, s32 poseId);
    extern s32 marioAnimeId(void);
    extern void marioPaperOff(void);
    extern void marioChgSmallJumpMotion(void);
    extern void partyPaperOff(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    extern char* partyChgPoseId(void* party, s32 poseId);
    extern s32 vec3_802f8948[];



    void* player;
    void* ground;
    void* ripple;
    f32 playerPos[3];
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 groundY;
    f32 groundX;
    f32 groundZ;
    f32 side;
    f32 delta;
    s32 animeId;

#define VIVIAN_MOVE_BLOCK()                                                        \
    do {                                                                           \
        player = *(void**)((s32)pParty + 0x160);                                   \
        playerPos[0] = *(f32*)((s32)player + 0x8C);                                \
        playerPos[1] = *(f32*)((s32)player + 0x90);                                \
        playerPos[2] = *(f32*)((s32)player + 0x94);                                \
        marioGetScreenPos(playerPos, &screenX, &screenY, &screenZ);                \
        ground = marioSearchGround(37.0f, -37.0f,        \
                                   &groundY, &groundX, &groundZ);                   \
        if (ground != 0 && (hitGetAttr(ground) & 0xA00) == 0) {                    \
            *(void**)((s32)player + 0x1E8) = ground;                               \
            *(f32*)((s32)player + 0x90) = groundY;                                 \
            if (*(f32*)((s32)player + 0x178) < 0.0f) {                 \
                groundY += 0.2f;                                     \
            }                                                                      \
        }                                                                          \
        if (*(u8*)((s32)pParty + 0x39) >= 0x14 &&                                  \
            *(u8*)((s32)pParty + 0x39) < 0x1E) {                                   \
            *(f32*)((s32)pParty + 0x17C) += -0.4f;                 \
            if (*(f32*)((s32)pParty + 0x17C) < -13.0f) {             \
                *(f32*)((s32)pParty + 0x17C) = -13.0f;               \
            }                                                                      \
            *(f32*)((s32)pParty + 0x94) = *(f32*)((s32)player + 0x8C);             \
            *(f32*)((s32)pParty + 0x98) = *(f32*)((s32)player + 0x90);             \
            *(f32*)((s32)pParty + 0x9C) = *(f32*)((s32)player + 0x94);             \
            movePos((f32*)((s32)pParty + 0x94), (f32*)((s32)pParty + 0x9C),        \
                    *(f32*)((s32)pParty + 0x17C),                                  \
                    toMovedirSimple(*(f32*)((s32)player + 0x1AC)));                \
            movePos((f32*)((s32)pParty + 0x94), (f32*)((s32)pParty + 0x9C),        \
                    0.0f,                                              \
                    toMovedirSimple(*(f32*)((s32)player + 0x1AC)));                \
        } else {                                                                   \
            if (*(u8*)((s32)pParty + 0x39) >= 4) {                                 \
                *(f32*)((s32)pParty + 0x17C) += 0.4f;                \
                if (*(f32*)((s32)pParty + 0x17C) >= 0.0f) {            \
                    *(f32*)((s32)pParty + 0x17C) = 0.0f;               \
                }                                                                  \
            }                                                                      \
            *(f32*)((s32)pParty + 0x94) = *(f32*)((s32)player + 0x8C);             \
            *(f32*)((s32)pParty + 0x98) = *(f32*)((s32)player + 0x90);             \
            *(f32*)((s32)pParty + 0x9C) = *(f32*)((s32)player + 0x94);             \
            movePos((f32*)((s32)pParty + 0x94), (f32*)((s32)pParty + 0x9C),        \
                    *(f32*)((s32)pParty + 0x17C),                                  \
                    toMovedirSimple(*(f32*)((s32)player + 0x1AC)));                \
        }                                                                          \
        side = 2.5f;                                                 \
        if (*(f32*)((s32)player + 0x1B0) == 0.0f) {                    \
            side = -2.5f;                                          \
        }                                                                          \
        movePos((f32*)((s32)pParty + 0x94), (f32*)((s32)pParty + 0x9C), side,      \
                toMovedir(270.0f +                                     \
                           *(f32*)((s32)player + 0x1AC)));                          \
        if (strcmp((char*)gp + 0x12C, "eki_02") == 0) {                 \
            unk_800c27c0(pParty, 0.0f,                                 \
                          (void*)((s32)player + 0x8C),                              \
                          *(f32*)((s32)player + 0x1A4),                             \
                          *(f32*)((s32)player + 0x1C0),                             \
                          0.5f);                                      \
        }                                                                          \
    } while (0)

    player = *(void**)((s32)pParty + 0x160);

    if ((*(u32*)((s32)pParty + 8) & 8) != 0) {
        *(u32*)((s32)pParty + 8) &= ~8;
        *(u8*)((s32)pParty + 0x39) = 0;
        *(f32*)((s32)pParty + 0x174) = 10.0f;
        *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
        *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
        *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
        vivian_cancel_req = 0;
        L_partyForceSlitOff(pParty);
    }

    *(f32*)((s32)player + 0x180) = 0.0f;
    *(f32*)((s32)pParty + 0x104) = 0.0f;

    switch (*(u8*)((s32)pParty + 0x39)) {
        case 0:
            marioChgMot(0x1C);
            *(f32*)((s32)pParty + 0x17C) = -13.0f;

            VIVIAN_MOVE_BLOCK();

            *(f32*)((s32)pParty + 0x100) =
                angleABf(*(f32*)((s32)pParty + 0x58),
                         *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)pParty + 0x94),
                         *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)pParty + 0x104) = 0.0f;
            *(f32*)((s32)pParty + 0x114) = 0.0f;
            *(u8*)((s32)pParty + 0x39) = 1;
            *(s32*)((s32)pParty + 0x24) = 6;
            partyChgPose(pParty, "PTR_A2_1");
            /* fallthrough */

        case 1:
            VIVIAN_MOVE_BLOCK();

            *(f32*)((s32)pParty + 0x58) +=
                (*(f32*)((s32)pParty + 0x94) - *(f32*)((s32)pParty + 0x58)) /
                (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x5C) +=
                (*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C)) /
                (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x60) +=
                (*(f32*)((s32)pParty + 0x9C) - *(f32*)((s32)pParty + 0x60)) /
                (f32)*(s32*)((s32)pParty + 0x24);

            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);

            if (--*(s32*)((s32)pParty + 0x24) > 0) {
                break;
            }

            *(u32*)((s32)pParty + 4) |= 0x00200000;
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)pParty + 0xFC) =
                toMovedirSimple(*(f32*)((s32)pParty + 0x110));
            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);

            *(u32*)((s32)player + 4) |= 0x1000;
            partyChgPose(pParty, "PTR_A2_3");
            partyPaperOn(pParty, "p_bibi");
            partyChgPaper(pParty, "PM_B_1");
            *(f32*)((s32)pParty + 0x80) += 1.6f;

            *(u32*)((s32)player + 4) |= 0x1000;
            marioPaperOn("p_bibi");
            marioChgPose("M_B_3");
            marioChgPaper("PM_B_1");

            *(u8*)((s32)pParty + 0x39) = 4;
            *(s32*)((s32)pParty + 0x24) = 30;
            psndSFXOn_3D(0x936, (void*)((s32)pParty + 0x58));
            /* fallthrough */

        case 4:
            VIVIAN_MOVE_BLOCK();

            if (30 - *(s32*)((s32)pParty + 0x24) < 25) {
                *(f32*)((s32)pParty + 0x80) += 0.4f;
                *(f32*)((s32)player + 0xA8) += 0.4f;

                if (*(f32*)((s32)pParty + 0x80) >= 8.0f) {
                    *(f32*)((s32)pParty + 0x80) = 8.0f;
                }
                if (*(f32*)((s32)player + 0xA8) >= 8.0f) {
                    *(f32*)((s32)player + 0xA8) = 8.0f;
                }
            }

            *(f32*)((s32)pParty + 0x174) += 1.0f;
            if (*(f32*)((s32)pParty + 0x174) >= 20.0f) {
                *(f32*)((s32)pParty + 0x174) = 20.0f;
                *(u32*)((s32)player + 4) |= 0x1000;
            }

            if (--*(s32*)((s32)pParty + 0x24) > 0) {
                break;
            }

            ripple = effRippleEntry();
            *(void**)((s32)pParty + 0x184) = ripple;

            if (*(s32*)((s32)pParty + 0x164) != 4) {
                effRippleSetCamId(ripple, *(s32*)((s32)pParty + 0x164));
            }

            psndSFXOn_3D(0x935, (void*)((s32)pParty + 0x58));

            if (*(s8*)((s32)pParty + 0x31) == 6) {
                *(u8*)((s32)pParty + 0x4F) |= 1;
            }

            *(u8*)((s32)pParty + 0x39) = 5;
            /* fallthrough */

        case 5:
            VIVIAN_MOVE_BLOCK();

            *(f32*)((s32)pParty + 0x174) -= 1.0f;
            if (*(f32*)((s32)pParty + 0x174) <= 10.0f) {
                *(f32*)((s32)pParty + 0x174) = 10.0f;
                *(u8*)((s32)pParty + 0x39) = 10;
                *(u32*)player |= 0x400;
            }
            break;

        case 10:
            VIVIAN_MOVE_BLOCK();

            partyUpdateKeyData(pParty);

            if (vivian_cancel_req != 0) {
                *(u8*)((s32)pParty + 0x39) = 0x14;
            }

            if ((*(u32*)pParty & 0x80000000) == 0 &&
                (*(u16*)((s32)pParty + 0x152) & 0x600) != 0) {
                *(u8*)((s32)pParty + 0x39) = 0x14;
            }
            break;

        default:
            break;
    }

    player = *(void**)((s32)pParty + 0x160);

    *(f32*)((s32)player + 0x8C) += *(f32*)((s32)player + 0x174);
    *(f32*)((s32)player + 0x94) += *(f32*)((s32)player + 0x17C);
    *(f32*)((s32)player + 0x90) += *(f32*)((s32)player + 0x178);

    if (*(u8*)((s32)pParty + 0x39) < 2) {
        *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
        *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
        *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
    } else {
        *(s32*)((s32)pParty + 0x58) = *(s32*)((s32)pParty + 0x94);
        *(s32*)((s32)pParty + 0x5C) = *(s32*)((s32)pParty + 0x98);
        *(s32*)((s32)pParty + 0x60) = *(s32*)((s32)pParty + 0x9C);

        *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)player + 0x8C);
        *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)player + 0x90);
        *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)player + 0x94);
    }

    delta = *(f32*)((s32)player + 0x90) - *(f32*)((s32)player + 0x114);
    if (delta > 0.0f) {
        *(f32*)((s32)pParty + 0xBC) += delta;
    }

    ripple = *(void**)((s32)pParty + 0x184);
    if (ripple == 0) {
        *(void**)((s32)pParty + 0x184) = 0;
    } else if (*(u32*)ripple == 0) {
        *(void**)((s32)pParty + 0x184) = 0;
    } else {
        effRippleSetPosition(
            ripple,
            *(f32*)((s32)pParty + 0xB8),
            *(f32*)((s32)pParty + 0xBC) + 1.5f,
            *(f32*)((s32)pParty + 0xC0));
    }


    /*
     * Second half of the target state machine: Vivian emerges from shadow
     * (states 0x14..0x1B). Keep the repeated movement block explicit through
     * the existing macro for this first reconstruction pass; later attempts
     * can split its stack locals if the remaining mismatch becomes layout-only.
     */
    switch (*(u8*)((s32)pParty + 0x39)) {
        case 0x14:
            *(f32*)((s32)pParty + 0x17C) = 0.0f;
            *(u32*)player &= ~0x400;
            psndSFXOn_3D(0x934, (void*)((s32)pParty + 0x58));
            *(u8*)((s32)pParty + 0x39) = 0x15;
            /* fallthrough */

        case 0x15:
            VIVIAN_MOVE_BLOCK();

            *(f32*)((s32)pParty + 0x174) += 1.0f;
            if (*(f32*)((s32)pParty + 0x174) >= 20.0f) {
                *(f32*)((s32)pParty + 0x174) = 20.0f;
                *(u8*)((s32)pParty + 0x39) = 0x16;
            }
            break;

        case 0x16:
            marioAdjustMoveDir();
            partyChgPose(pParty, "PTR_A2_4");
            *(u16*)((s32)pParty + 0x20) = 0x18;

            if (*(u8*)((s32)pParty + 0x31) == 6) {
                *(u8*)((s32)pParty + 0x4F) &= ~1;
            }

            *(u8*)((s32)pParty + 0x39) = 0x17;
            break;

        case 0x17:
            VIVIAN_MOVE_BLOCK();

            animPoseSetLocalTime(24.0f, *(s32*)((s32)pParty + 0xC));
            *(u16*)((s32)pParty + 0x20) = 0x18;
            *(u8*)((s32)pParty + 0x39) = 0x18;
            *(s32*)((s32)pParty + 0x24) = 0x18;
            psndSFXOn_3D(0x937, (void*)((s32)pParty + 0x58));
            /* fallthrough */

        case 0x18:
            VIVIAN_MOVE_BLOCK();

            animPoseSetLocalTime(
                (f64)*(s32*)((s32)pParty + 0x24),
                *(s32*)((s32)pParty + 0xC));

            *(f32*)((s32)pParty + 0x80) -= 0.4f;
            if (*(f32*)((s32)pParty + 0x80) <= 2.7f) {
                *(f32*)((s32)pParty + 0x80) = 2.7f;
            }

            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) > 0 &&
                *(s32*)((s32)pParty + 0x24) == 0x14) {
                marioChgPose("M_S_1");
                *(u16*)((s32)player + 0x28) = 0x18;
                *(s16*)((s32)pParty + 0x182) = 0x18;
                *(u8*)((s32)pParty + 0x39) = 0x19;
            }
            break;

        case 0x19:
            VIVIAN_MOVE_BLOCK();

            animPoseSetLocalTime(
                (f64)*(s32*)((s32)pParty + 0x24),
                *(s32*)((s32)pParty + 0xC));

            animeId = marioAnimeId();
            animPoseSetLocalTime(
                (f64)*(s16*)((s32)pParty + 0x182),
                *(s32*)((s32)player + 0x22C + animeId * 4));

            *(f32*)((s32)player + 0xA8) -= 0.4f;
            if (*(f32*)((s32)player + 0xA8) <= 2.7f) {
                *(f32*)((s32)player + 0xA8) = 2.7f;
            }

            *(f32*)((s32)pParty + 0x80) -= 0.4f;
            if (*(f32*)((s32)pParty + 0x80) <= 2.7f) {
                *(f32*)((s32)pParty + 0x80) = 2.7f;
            }

            *(s16*)((s32)pParty + 0x182) -= 1;
            *(s32*)((s32)pParty + 0x24) -= 1;

            if (*(s32*)((s32)pParty + 0x24) > 0) {
                return;
            }

            *(u8*)((s32)pParty + 0x39) = 0x1A;
            *(s32*)((s32)pParty + 0x24) = 0x18;
            /* fallthrough */

        case 0x1A:
            VIVIAN_MOVE_BLOCK();

            animPoseSetLocalTime(0.0f, *(s32*)((s32)pParty + 0xC));

            animeId = marioAnimeId();
            animPoseSetLocalTime(
                (f64)*(s16*)((s32)pParty + 0x182),
                *(s32*)((s32)player + 0x22C + animeId * 4));

            *(f32*)((s32)player + 0xA8) -= 0.4f;
            if (*(f32*)((s32)player + 0xA8) <= 2.7f) {
                *(f32*)((s32)player + 0xA8) = 2.7f;
            }

            *(f32*)((s32)pParty + 0x80) -= 0.4f;
            if (*(f32*)((s32)pParty + 0x80) <= 2.7f) {
                *(f32*)((s32)pParty + 0x80) = 2.7f;
            }

            *(s16*)((s32)pParty + 0x182) -= 1;
            if (*(s16*)((s32)pParty + 0x182) > 0) {
                *(s16*)((s32)pParty + 0x182) = 0;
            }

            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) > 0) {
                *(s32*)((s32)pParty + 0x24) = 0;
            }

            if (*(s16*)((s32)pParty + 0x182) == 0 &&
                *(s32*)((s32)pParty + 0x24) == 0) {
                *(u8*)((s32)pParty + 0x39) = 0x1B;

                marioPaperOff();
                *(u32*)((s32)player + 0x20) = 0;
                *(u32*)((s32)player + 4) &= ~0x1000;

                *(void**)((s32)player + 0x1EC) =
                    *(void**)((s32)player + 0x1E8);
                *(void**)((s32)player + 0x1E8) = 0;

                /*
                 * Target loads vec3_802f8948 + 0x18/+0x1C/+0x20 here,
                 * i.e. the adjacent vec3_802f8960 zero vector.
                 */
                *(s32*)((s32)player + 0xA4) = vec3_802f8948[6];
                *(s32*)((s32)player + 0xA8) = vec3_802f8948[7];
                *(s32*)((s32)player + 0xAC) = vec3_802f8948[8];

                marioChgSmallJumpMotion();
                *(f32*)((s32)player + 0x180) = 0.0f;
            }
            break;

        case 0x1B:
            animPoseSetLocalTime(0.0f, *(s32*)((s32)pParty + 0xC));

            *(f32*)((s32)pParty + 0x174) -= 1.0f;
            if (*(f32*)((s32)pParty + 0x174) <= 10.0f) {
                *(f32*)((s32)pParty + 0x174) = 10.0f;

                partyPaperOff(pParty);

                *(u32*)pParty &= ~0x100;
                *(u32*)((s32)pParty + 4) &= ~0x200000;
                *(f32*)((s32)pParty + 0x174) = 10.0f;
                *(f32*)((s32)pParty + 0x80) = 0.0f;

                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                partyChgPoseId(pParty, 1);
                *(u32*)pParty &= 0x7FFFFFFF;
            }
            break;

        default:
            break;
    }

#undef VIVIAN_MOVE_BLOCK
}

s32 vivian_cancel_req;
