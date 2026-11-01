// Vivian: partner init, follow and the shadow (hide in the floor) ability.

#include "version.h"
#include "party/party_vivian.h"
#include "driver/animdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "party.h"
#include "system.h"
#include <string.h>
#include "mario/mario_sbr.h"
#include <dolphin/pad.h>
#include "party/party_move.h"
#include "party/party_motion.h"
#include "mario/mario_motion.h"
#include "party/party_hit.h"
#include "mario/mario_hit.h"
#include "eff/eff_ripple.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"

#if VERSION != VERSION_JP
// US has no local duplicate: this is the global vivian_init
#define vivian_init_801809C0 vivian_init
#endif

#if VERSION >= VERSION_US
#define VIVIAN_FOLLOW_DIST 30.0f
#else
#define VIVIAN_FOLLOW_DIST 22.0f
#endif

s32 vivian_cancel_req;
/**
 * Mario motion MARIO_MOTION_VIVIAN (entered while Veil hides him in the floor):
 * on entry it stops any jump or fall, zeroes his speed and timers and, in retail,
 * clears his wall and push collision; nothing runs on later frames.
 */
#if VERSION > VERSION_DEMO_US

#pragma explicit_zero_data on
/**
 * Re-applies Mario's sunk-into-the-shadow look (the "p_bibi" paper pose M_B_3 / PM_B_1, held at frame 24)
 * when mario.c refreshes his animation during the Vivian motion. Not in the demos.
 */
// Descriptive name (not in the symbol maps).
void marioForceVivianAnime(void) {
    // FAKE: padding constant kept in .sdata2 by explicit_zero_data and the (void)& use; removing it changes the section layout
    static const u32 gap_09_8041DF6C_sdata2 = 0;
    MarioWork* mario = marioGetPtr();
    (void)&gap_09_8041DF6C_sdata2;

    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn("p_bibi");
    marioChgPose("M_B_3");
    marioChgPaper("PM_B_1");
    mario->poseTime = 24;
    animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], mario->poseTime);
}

/**
 * Mario motion MARIO_MOTION_VIVIAN (entered while Veil hides him in the floor):
 * on entry it stops any jump or fall, zeroes his speed and timers and, in retail,
 * clears his wall and push collision; nothing runs on later frames.
 */
#endif
void mot_vivian(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
#if VERSION <= VERSION_DEMO_US
        }
    }

    /** Demo copy: hides Vivian's shadow tail (bit 0 of her tail flags), when she sinks into the floor with Mario. */
    static inline void partyVivianTailStop(PartyEntry* party) {
        if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
            party->work.vivian.tailFlags |= 1; // the demo keeps the tail flags in the Vivian work
        }
    }

    /** Demo copy: shows Vivian's shadow tail again. */
    static inline void partyVivianTailStart(PartyEntry* party) {
        if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
            party->work.vivian.tailFlags &= ~1;
#else
        mario->hitobjFront = NULL;
        mario->hitobjFrontA = NULL;
        mario->hitobjPush = NULL;
        mario->hitobjWall = NULL;
#endif
    }
}


/**
 * Post-step for Mario's Vivian motion: if Mario takes damage while hidden,
 * pops both of them back out at once (paper off, offsets and walls restored, Vivian back to follow mode);
 * otherwise, while Veil is in use, it sets vivian_cancel_req so vivian_use starts rising out.
 */
void mot_vivian_post(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    MarioWork* mario = marioGetPtr();

    if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_DAMAGE) {
        mario->dispFlags &= ~MARIO_DISP_FLAG_SHADOW_OFF;
        marioPaperOff();
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
#if VERSION <= VERSION_DEMO_US
            mario->dispOfs = zero;
#else
            mario->dispOfs2 = zero;
#endif
        }
        mario->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        if (party) {
            partyPaperOff(party);
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            party->flags &= ~PARTY_FLAG_IS_EVT_USE;
            party->flags2 &= ~PARTY_DISP_FLAG_SHADOW_NODECAY;
            party->work.vivian.shadowSize = 10.0f;
            {
                Vec zero2 = {0.0f, 0.0f, 0.0f};
                party->dispOfs = zero2;
            }
            partyChgRunMode(party, 2);
            party->moveMode = PARTY_MOVE_MODE_STAY;
            partyChgPoseId(party, 1);
            partyVivianTailStart(party);
        }
#if VERSION <= VERSION_DEMO_US
    camFollowYOn();
#endif
    } else if (party && party->currentMemberId == PARTY_MEMBER_VIVIAN && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        // FAKE: the store is reached by a goto past the return; writing it here changes the object
        goto cancel;
    }
    return;
cancel:
    vivian_cancel_req = 1;
}

void vivian_init_801809C0(PartyEntry* party) {
    party->memberKind = 6;
    party->moveKind = 1;
    party->height = 35.0f;
    party->width = 24.0f;
    party->work.vivian.shadowSize = 10.0f;
#if VERSION <= VERSION_DEMO_US
    party->work.vivian.tailFlags = 0;
#else
    party->tailFlags = 0;
#endif
    party->trailPos = party->position;
    party->work.vivian.rippleEffect = NULL;
    vivian_cancel_req = 0;
}

/**
 * Where Vivian is in her Veil: 0 not in use (or not Vivian), 1 sinking into the floor with Mario, 2 hidden,
 * 3 rising back out, 4 anything else.
 * Mario's collision, item pickup, NPC contact and the event commands read it (1 to 3 skip several of Mario's hit
 * checks).
 */
s32 vivianGetStatus(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    u8 st;

    if (party == NULL) {
        return 0;
    }
    if (party->currentMemberId != PARTY_MEMBER_VIVIAN || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 0;
    }
    st = party->useMotionId;
    if (st < 10) {
        return 1;
    }
    if (st == 10) {
        return 2;
    }
    if (st >= 20 && st <= 27) {
        return 3;
    }
    return 4;
}

/**
 * Ends a hidden Veil from code or a script (evt_party, evt_mario):
 * if Vivian is in the hidden state it starts the rise-out sequence.
 */
void vivianUnhold(PartyEntry* party) {
    if (party == NULL || party->currentMemberId != PARTY_MEMBER_VIVIAN || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return;
    }
    if (party->useMotionId == 10) {
        party->useMotionId = 20;
    }
}

#if VERSION <= VERSION_DEMO_US

/**
 * Moves Vivian's tail point (trailPos) a tenth of the way toward her each frame,
 * keeping it within VIVIAN_FOLLOW_DIST (30 units in the US and PAL releases, 22 before);
 * party.c draws her shadow tail to it.
 */
void vivianShadow(PartyEntry* party) {
    f32 dx, dy, dz;

    {
#else
/** Demo copy: hides Vivian's shadow tail (bit 0 of her tail flags), when she sinks into the floor with Mario. */
void partyVivianTailStop(PartyEntry* party) {
    if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
        party->tailFlags |= 1;
    }
}

/** Demo copy: shows Vivian's shadow tail again. */
void partyVivianTailStart(PartyEntry* party) {
    if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
        party->tailFlags &= ~1;
    }
}

/**
 * Whether party.c draws Vivian's shadow tail this frame:
 * only for Vivian, with the tail not hidden, on screen,
 * and not while Fahr Outpost's bom_01 has parked her at y = -1000.
 */
// Descriptive name (not in the symbol maps).
BOOL vivianDisplayTailCheck(PartyEntry* party) {
    if (party->currentMemberId != PARTY_MEMBER_VIVIAN) {
        return FALSE;
    }
    if ((party->tailFlags & 1)) {
        return FALSE;
    }
    if (party->flags2 & PARTY_DISP_FLAG_OUT_OF_SCREEN) {
        return FALSE;
    }
    if (strcmp(gp->currentMapName, "bom_01") == 0 && -1000.0f == party->position.y) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Moves Vivian's tail point (trailPos) a tenth of the way toward her each frame,
 * keeping it within VIVIAN_FOLLOW_DIST (30 units in the US and PAL releases, 22 before);
 * party.c draws her shadow tail to it.
 */
void vivianShadow(PartyEntry* party) {
    f32 dx, dy, dz;

    if (party && party->currentMemberId == PARTY_MEMBER_VIVIAN) {
#endif
        dx = 0.1f * (party->position.x - party->trailPos.x);
        dy = 0.1f * (party->position.y - party->trailPos.y);
        dz = 0.1f * (party->position.z - party->trailPos.z);
        party->trailPos.x += dx;
        party->trailPos.y += dy;
        party->trailPos.z += dz;
        if (distABf(party->position.x, party->position.z, party->trailPos.x, party->trailPos.z) >= VIVIAN_FOLLOW_DIST) {
            party->trailPos = party->position;
            movePos(&party->trailPos.x, &party->trailPos.z, VIVIAN_FOLLOW_DIST,
                    angleABf(party->position.x, party->position.z, party->trailPos.x, party->trailPos.z));
        }
    }
}

/** Vivian's follow routine: the shared flying-partner movement plus her shadow-tail update. */
void vivian_move(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyMoveFlyInit(party, 1);
    }
    partyFlyMain(party);
    vivianShadow(party);
}

/** demo: Vivian always hides 13 behind Mario, with no ground search outside the hidden state */
static inline void vivianFollow(PartyEntry* party) {
#if VERSION <= VERSION_DEMO_US
    MarioWork* mario = party->playerPtr;
    f32 ofs;

    party->moveTarget = mario->position;
    movePos(&party->moveTarget.x, &party->moveTarget.z, -13.0f, toMovedirSimple(mario->displayDirection));
    if (0.0f == mario->displayDirectionTarget) {
        ofs = -2.5f;
    } else {
        ofs = -2.5f;
    }
    movePos(&party->moveTarget.x, &party->moveTarget.z, ofs, toMovedir(270.0f + mario->displayDirection));
}

/**
 * Demo Veil: Vivian moves behind Mario and both sink into the floor shadow (ripple effect, sounds),
 * then stay hidden riding the ground under Mario until B
 * or X (or vivian_cancel_req) brings them back up with a small hop.
 */
void vivian_use(PartyEntry* party) {
    f32 gx, gy, gz;
    void* hit;
    void* rip;
    MarioWork* p;
#else
    HitEntry* hit;
    u8 id;
    MarioWork* mario = party->playerPtr;
    f32 groundY, gy, gz;
    f32 sx, sy, sz;
    f32 ofs;

    marioGetScreenPos(mario->position, &sx, &sy, &sz);
    hit = marioSearchGround(&groundY, &gy, &gz, 37.0f, -37.0f);
    if (hit && !(hitGetAttr(hit) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_TOGE))) {
        mario->hitobjStandOn = hit;
        mario->position.y = groundY;
        if (mario->floorMove.y < 0.0f) {
            groundY += 0.2f;
        }
    }
    id = party->useMotionId;
    if (id >= 20 && id < 30) {
        party->work.vivian.followOfs += -0.4f;
        if (party->work.vivian.followOfs < -13.0f) {
            party->work.vivian.followOfs = -13.0f;
        }
        party->moveTarget = mario->position;
        movePos(&party->moveTarget.x, &party->moveTarget.z, party->work.vivian.followOfs, toMovedirSimple(mario->displayDirection));
        movePos(&party->moveTarget.x, &party->moveTarget.z, 0.0f, toMovedirSimple(mario->displayDirection));
        if (0.0f == mario->displayDirectionTarget) {
            ofs = -2.5f;
        } else {
            ofs = 2.5f;
        }
        movePos(&party->moveTarget.x, &party->moveTarget.z, ofs, toMovedir(270.0f + mario->displayDirection));
    } else {
        if (id >= 4) {
            party->work.vivian.followOfs += 0.4f;
            if (party->work.vivian.followOfs >= 0.0f) {
                party->work.vivian.followOfs = 0.0f;
            }
        }
        party->moveTarget = mario->position;
        movePos(&party->moveTarget.x, &party->moveTarget.z, party->work.vivian.followOfs, toMovedirSimple(mario->displayDirection));
        if (0.0f == mario->displayDirectionTarget) {
            ofs = -2.5f;
        } else {
            ofs = 2.5f;
        }
        movePos(&party->moveTarget.x, &party->moveTarget.z, ofs, toMovedir(270.0f + mario->displayDirection));
    }
    if (strcmp(gp->currentMapName, "eki_02") == 0) {
#if VERSION >= VERSION_US
        partyChkWallAround(party, &mario->position, 0.0f, mario->directionView, mario->wPlayerCollisionBox.z, 0.5f);
#else
        marioChkWallAround(&mario->position, 1, 0.0f, mario->directionView, mario->wPlayerCollisionBox.z, 0.5f);
#endif
    }
}

#pragma defer_codegen off
#pragma explicit_zero_data on
/**
 * Demo Veil: Vivian moves behind Mario and both sink into the floor shadow (ripple effect, sounds),
 * then stay hidden riding the ground under Mario until B
 * or X (or vivian_cancel_req) brings them back up with a small hop.
 */
void vivian_use(PartyEntry* party) {
    Vec s;
    EffectEntry* rip;
    f32 d;
#endif
    MarioWork* m = party->playerPtr;
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->useMotionId = 0;
        party->work.vivian.shadowSize = 10.0f;
        party->trailPos = party->position;
#if VERSION <= VERSION_DEMO_US
        party->work.vivian.followOfs = 0.0f;
        vivian_cancel_req = 0;
        camFollowYOff();
#else
        vivian_cancel_req = 0;
        partyForceSlitOff(party);
#endif
    }
    m->baseSpeed = 0.0f;
    party->moveSpeed = 0.0f;

    switch (party->useMotionId) {
    case 0:
        marioChgMot(MARIO_MOTION_VIVIAN);
#if VERSION > VERSION_DEMO_US
        party->work.vivian.followOfs = -13.0f;
#endif
        vivianFollow(party);
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        party->displayDirection = m->displayDirection;
        party->displayDirectionTarget = m->displayDirectionTarget;
        party->moveSpeed = 0.0f;
        party->velY = 0.0f;
        party->useMotionId = 1;
        party->moveFrames = 6;
        partyChgPose(party, "PTR_A2_1");
    case 1:
        vivianFollow(party);
        party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
        party->trailPos = party->position;
        if (--party->moveFrames > 0) {
            break;
        }
#if VERSION > VERSION_DEMO_US
        party->flags2 |= PARTY_DISP_FLAG_SHADOW_NODECAY;
#endif
        party->displayDirectionTarget = m->displayDirectionTarget;
        party->moveDir = toMovedirSimple(party->displayDirectionTarget);
        party->trailPos = party->position;
#if VERSION <= VERSION_DEMO_US
        party->flags2 |= PARTY_DISP_FLAG_SHADOW_NODECAY;
        partyChgPose(party, "PTR_A2_2");
        party->useMotionId = 2;
        party->moveFrames = 2;
    case 2:
        m->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        partyChgPose(party, "PTR_A2_3");
        partyPaperOn((PartyEntry*)party, "p_bibi");
        partyChgPaper(party, "PM_B_1");
        party->useMotionId = 3;
        party->moveFrames = 4;
    case 3:
#else
        m->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        partyChgPose(party, "PTR_A2_3");
        partyPaperOn(party, "p_bibi");
        partyChgPaper(party, "PM_B_1");
#endif
        party->dispOfs.y += 1.6f;
        m->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        marioPaperOn("p_bibi");
        marioChgPose("M_B_3");
        marioChgPaper("PM_B_1");
        party->useMotionId = 4;
        party->moveFrames = 30;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW1), &party->position);
    case 4:
#if VERSION <= VERSION_DEMO_US
        if (30 - party->moveFrames <= 24) {
            party->dispOfs.y += 0.4f;
            m->dispOfs.y += 0.4f;
#else
        vivianFollow(party);
        if (30 - party->moveFrames <= 24) {
            party->dispOfs.y += 0.4f;
            m->dispOfs2.y += 0.4f;
#endif
            if (party->dispOfs.y >= 8.0f) {
                party->dispOfs.y = 8.0f;
            }
#if VERSION <= VERSION_DEMO_US
            if (m->dispOfs.y >= 8.0f) {
                m->dispOfs.y = 8.0f;
#else
            if (m->dispOfs2.y >= 8.0f) {
                m->dispOfs2.y = 8.0f;
#endif
            }
        }
        party->work.vivian.shadowSize += 1.0f;
        if (party->work.vivian.shadowSize >= 20.0f) {
            party->work.vivian.shadowSize = 20.0f;
            m->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        }
        if (--party->moveFrames > 0) {
            break;
        }
        rip = effRippleEntry(1, party->playerPtr->position.x, 1.5f + party->playerPtr->position.y, party->playerPtr->position.z);
        party->work.vivian.rippleEffect = rip;
        if (party->camId != CAMERA_3D) {
            effRippleSetCamId(rip, party->camId);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW1_2), &party->position);
        partyVivianTailStop(party);
        party->useMotionId = 5;
    case 5:
#if VERSION > VERSION_DEMO_US
        vivianFollow(party);
#endif
        party->work.vivian.shadowSize -= 1.0f;
        if (party->work.vivian.shadowSize <= 10.0f) {
            party->work.vivian.shadowSize = 10.0f;
            party->useMotionId = 10;
            m->flags |= MARIO_FLAG_WALLS_DISABLED;
        }
        break;
    case 10:
#if VERSION <= VERSION_DEMO_US
        hit = marioSearchGround(&gx, &gy, &gz, 100.0f, -100.0f);
        if (hit && !(hitGetAttr(hit) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_TOGE))) {
            if (m->floorMove.y || m->floorMove.x) {
                if ((f32)m->screenY > 480.0f || (f32)m->screenY < 50.0f) {
                    camFollowYOn();
                }
            }
            if (m->floorMove.y < 0.0f) {
                gx += 0.2f;
            }
            m->hitobjStandOn = hit;
            m->position.y = gx;
            party->position.y = gx;
        }
#else
        vivianFollow(party);
#endif
        partyUpdateKeyData(party);
        if (vivian_cancel_req) {
            party->useMotionId = 20;
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE) && (party->buttonTrg & (PAD_BUTTON_B | PAD_BUTTON_X))) {
            party->useMotionId = 20;
        }
        break;
    }

    m->position.x += m->floorMove.x;
    m->position.z += m->floorMove.z;
    m->position.y += m->floorMove.y;
#if VERSION <= VERSION_DEMO_US
    party->position.x += m->floorMove.x;
    party->position.z += m->floorMove.z;
    party->position.y += m->floorMove.y;
    party->trailPos = m->position;
    p = party->playerPtr;
#else
    if (party->useMotionId >= 2) {
        party->position = party->moveTarget;
        party->trailPos = m->position;
    } else {
        party->trailPos = party->position;
    }
    d = m->position.y - m->basePos.y;
    if (d > 0.0f) {
        party->trailPos.y += d;
    }
    s = party->trailPos;
    // FAKE: stores NULL into an already NULL rippleEffect; dropping the store changes the object
#endif
    if (party->work.vivian.rippleEffect == NULL) {
        party->work.vivian.rippleEffect = NULL;
    } else if (party->work.vivian.rippleEffect->flags == 0) {
        party->work.vivian.rippleEffect = NULL;
    } else {
#if VERSION <= VERSION_DEMO_US
        effRippleSetPosition(party->work.vivian.rippleEffect, p->position.x, 1.5f + p->position.y, p->position.z);
    }
    if (party->useMotionId >= 4) {
        party->trailPos = m->position;
#else
        effRippleSetPosition(party->work.vivian.rippleEffect, s.x, 1.5f + s.y, s.z);
#endif
    }

    switch (party->useMotionId) {
    case 20:
#if VERSION > VERSION_DEMO_US
        party->work.vivian.followOfs = 0.0f;
#endif
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW1_1), &party->position);
        party->useMotionId = 21;
    case 21:
#if VERSION > VERSION_DEMO_US
        vivianFollow(party);
#endif
        party->work.vivian.shadowSize += 1.0f;
        if (party->work.vivian.shadowSize >= 20.0f) {
            party->work.vivian.shadowSize = 20.0f;
            party->useMotionId = 22;
        }
        break;
    case 22:
        marioAdjustMoveDir();
        partyChgPose(party, "PTR_A2_4");
        party->poseTime = 24;
        partyVivianTailStart(party);
        party->useMotionId = 23;
        break;
    case 23:
#if VERSION > VERSION_DEMO_US
        vivianFollow(party);
#endif
        animPoseSetLocalTime(party->paperAnimGroup, 24.0f);
        party->poseTime = 24;
        party->useMotionId = 24;
        party->moveFrames = 24;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW2), &party->position);
    case 24:
#if VERSION > VERSION_DEMO_US
        vivianFollow(party);
#endif
        animPoseSetLocalTime(party->paperAnimGroup, party->moveFrames);
        party->dispOfs.y -= 0.4f;
        if (party->dispOfs.y <= 2.7f) {
            party->dispOfs.y = 2.7f;
        }
        if (--party->moveFrames <= 0) {
            break;
        }
        if (party->moveFrames == 20) {
            marioChgPose("M_S_1");
            m->poseTime = 24;
            party->work.vivian.marioAnimTime = 24;
            party->useMotionId = 25;
        }
        break;
    case 25:
#if VERSION <= VERSION_DEMO_US
        animPoseSetLocalTime(party->paperAnimGroup, party->moveFrames);
        animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], party->work.vivian.marioAnimTime);
        m->dispOfs.y -= 0.4f;
        if (m->dispOfs.y <= 2.7f) {
            m->dispOfs.y = 2.7f;
#else
        vivianFollow(party);
        animPoseSetLocalTime(party->paperAnimGroup, party->moveFrames);
        animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], party->work.vivian.marioAnimTime);
        m->dispOfs2.y -= 0.4f;
        if (m->dispOfs2.y <= 2.7f) {
            m->dispOfs2.y = 2.7f;
#endif
        }
        party->dispOfs.y -= 0.4f;
        if (party->dispOfs.y <= 2.7f) {
            party->dispOfs.y = 2.7f;
        }
        party->work.vivian.marioAnimTime--;
        if (--party->moveFrames > 0) {
            break;
        }
        party->useMotionId = 26;
        party->moveFrames = 24;
    case 26:
#if VERSION <= VERSION_DEMO_US
        animPoseSetLocalTime(party->paperAnimGroup, 0.0f);
        animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], party->work.vivian.marioAnimTime);
        m->dispOfs.y -= 0.4f;
        if (m->dispOfs.y <= 2.7f) {
            m->dispOfs.y = 2.7f;
#else
        vivianFollow(party);
        animPoseSetLocalTime(party->paperAnimGroup, 0.0f);
        animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], party->work.vivian.marioAnimTime);
        m->dispOfs2.y -= 0.4f;
        if (m->dispOfs2.y <= 2.7f) {
            m->dispOfs2.y = 2.7f;
#endif
        }
        party->dispOfs.y -= 0.4f;
        if (party->dispOfs.y <= 2.7f) {
            party->dispOfs.y = 2.7f;
        }
        if (--party->work.vivian.marioAnimTime > 0) {
            party->work.vivian.marioAnimTime = 0;
        }
        if (--party->moveFrames > 0) {
            party->moveFrames = 0;
        }
        if (party->work.vivian.marioAnimTime == 0 && party->moveFrames == 0) {
            party->useMotionId = 27;
            marioPaperOff();
            m->reserved_20 = NULL;
            m->dispFlags &= ~MARIO_DISP_FLAG_SHADOW_OFF;
            m->hitobjJumpFrom = m->hitobjStandOn;
            m->hitobjStandOn = NULL;
            {
                Vec zero = {0.0f, 0.0f, 0.0f};
#if VERSION <= VERSION_DEMO_US
                m->dispOfs = zero;
#else
                m->dispOfs2 = zero;
#endif
            }
            marioChgSmallJumpMotion();
            m->baseSpeed = 0.0f;
        }
        break;
    case 27:
        animPoseSetLocalTime(party->paperAnimGroup, 0.0f);
        party->work.vivian.shadowSize -= 1.0f;
        if (!(party->work.vivian.shadowSize > 10.0f)) {
            party->work.vivian.shadowSize = 10.0f;
            partyPaperOff(party);
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            party->flags2 &= ~PARTY_DISP_FLAG_SHADOW_NODECAY;
            party->work.vivian.shadowSize = 10.0f;
            party->dispOfs.y = 0.0f;
            partyChgRunMode(party, 2);
            party->moveMode = PARTY_MOVE_MODE_STAY;
            partyChgPoseId(party, 1);
            party->flags &= ~PARTY_FLAG_IS_EVT_USE;
        }
        break;
    }
}

#pragma explicit_zero_data reset
