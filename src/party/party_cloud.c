// Flurrie (cloud): partner init, follow, hover and the gust (breath) ability.

#include "version.h"
// FAKE: _MATH_H_ is defined so math.h stays out of this unit; including it changes the object
#define _MATH_H_
#include "party/party_cloud.h"
#include "dolphin/types.h"
#include "musyx/musyx.h"
#include "party.h"
#include "mario/mario.h"
#include "mario/mario_sbr.h"
#include "mario/mario_party.h"
#include "mario/mario_cam.h"
#include "system.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "party/party_move.h"

// Flurrie use-motion states, not Mario motion ids.
typedef enum CloudUseState {
    CLOUD_USE_APPROACH_INIT = 0,
    CLOUD_USE_APPROACH = 1,
    CLOUD_USE_BREATH_INIT = 10,
    CLOUD_USE_BREATH_START = 11,
    CLOUD_USE_BREATHING = 12,
    CLOUD_USE_BREATH_FINISH = 13,
    CLOUD_USE_CLEANUP = 20,
} CloudUseState;

static Vec cloud_at;

#include "driver/hitdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "pmario_sound.h"
#include "mario/mario_motion.h"
#include "eff/n64/eff_kumokumo_n64.h"
void _cloud_use(PartyEntry* party);
static void getHitBreatheout2(PartyEntry* party, f32 angle);

// Flurrie's work (PartyEntry.useStruct). A macro, not an inline function: as a function the object changes.
#define CLOUD_WORK(p) ((PartyCloud*)(p)->useStruct)
static inline EffKumokumoN64Particle* breathParticles(PartyEntry* p) {
    return (EffKumokumoN64Particle*)p->work.cloud.breathEffect->userdata;
}

/**
 * Swings one of the two side rays of Flurrie's gust back and forth:
 * the ray's angle steps 1 degree per frame between -20 and +20 and turns around at either end.
 */
static inline void cloudSweep(PartyEntry* party, s32 i) {
    PartyCloud* cl = CLOUD_WORK(party);
    if (cl->sweep[i].reverse == 0) {
        cl->sweep[i].angle += 1.0f;
        cl = CLOUD_WORK(party);
        if (cl->sweep[i].angle >= 20.0f) {
            cl->sweep[i].angle = 20.0f;
            CLOUD_WORK(party)->sweep[i].reverse = 1;
        }
    } else {
        cl->sweep[i].angle -= 1.0f;
        cl = CLOUD_WORK(party);
        if (cl->sweep[i].angle <= -20.0f) {
            cl->sweep[i].angle = -20.0f;
            CLOUD_WORK(party)->sweep[i].reverse = 0;
        }
    }
}

#if VERSION >= VERSION_JP

/**
 * Mario's motion while Flurrie gusts (MARIO_MOTION_CLOUD):
 * on its first frame clears his jump, step and fall flags and timers so he stands still.
 * The early demos keep this handler in mario_motion.c.
 */
void mot_cloud(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_JUMPING);
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
    }
}

/**
 * Saves the 3D camera's look-at point into cloud_at and, if Mario's camera is zoomed in,
 * requests a zoom-out (marioCamZoomOffReq2(600)).
 * Called when Mario's Flurrie motion ends (mot_cloud_post in mario_motion.c).
 */
void cloudResetAt(void) {
    MarioWork* mario = marioGetPtr();
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);

    if (party != NULL && party->currentMemberId == PARTY_FLURRIE) {
        cloud_at.x = camGetPtr(CAMERA_3D)->cameraAt.x;
        cloud_at.y = camGetPtr(CAMERA_3D)->cameraAt.y;
        cloud_at.z = camGetPtr(CAMERA_3D)->cameraAt.z;

        marioCamZoomOffReq2(600);
    }

    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    marioAdjustMoveDir();
    mario->camShiftRate.x = 0.01f;
    mario->camShiftTargetX = 0.0f;
}
#endif

/**
 * Gale Force state for evt_party.c's script wait: 0 when there is no partner, or Flurrie is out but not in use.
 * Otherwise 1 for use motion 10, 2 for other motions; the motion-10 test does not require the partner to be Flurrie.
 */
#if VERSION >= VERSION_JP
u8 cloudGetStatus(void) {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    u8 status = 2;

    if (party == NULL || (party->currentMemberId == PARTY_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        status = 0;
        return status;
    } else if (party->useMotionId == CLOUD_USE_BREATH_INIT && party->useMotionId < CLOUD_USE_CLEANUP) {
        status = 1;
    }

    return status;
}
#endif

/**
 * Direction of Flurrie's gust in movement degrees:
 * -1 while she is still moving into position, 0 when there is no gust work.
 */
#if VERSION >= VERSION_JP
f32 cloudGetBreathDir(void) {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathDir;

    if (party == NULL) {
        return 0.0f;
    }

    if ((party->flags & PARTY_FLAG_IS_BEING_USED) != 0 && party->useMotionId < CLOUD_USE_BREATH_INIT) {
        breathDir = -1.0f;
    } else if (party->useStruct == NULL) {
        breathDir = 0.0f;
    } else {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDir = toMovedir(cloud->breathDir);
    }

    return breathDir;
}
#endif

/**
 * Gust strength (0-100) at a point: nonzero only while Flurrie is mid-gust,
 * the point is within 10 units of 18 below her,
 * within reach (allowing half the target's width)
 * and within 30 degrees of the gust's direction (cosine falloff).
 * npcdrv.c uses it to tell whether an NPC is caught by the gust.
 */
#if VERSION >= VERSION_JP
f32 cloudGetBreathPower(Vec pos, f32 size) {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathPower;
    f32 dist;
    f32 angle;
    f32 reach;

    breathPower = 0.0f;
    if (party == NULL) {
        return breathPower;
    }

    if (party->currentMemberId == PARTY_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) != 0 &&
        party->useMotionId >= CLOUD_USE_BREATH_INIT && __fabsf(party->position.y - pos.y - 18.0f) <= 10.0f) {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        reach = cloud->breathDist;
        if (-((0.5f * size) - distABf(party->position.x, party->position.z, pos.x, pos.z)) <= reach) {
            dist = toMovedir(CLOUD_WORK(party)->breathDir);
            angle = angleABf(party->position.x, party->position.z, pos.x, pos.z);
            dist = revise360(angle - dist);

            if (dist <= 30.0f || dist >= 330.0f) {
                breathPower = cos((3.1415927f * dist) / 180.0f);
                breathPower = 100.0f * __fabsf(breathPower);
            }
        }
    }

    return breathPower;
}

struct HitEntry* cloudGetHitBreathout(s32 which) {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
#else
static void getHitBreatheout2(PartyEntry* party, f32 tilt);
struct HitEntry* cloudGetHitBreathout(s32 which) {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
#endif

    struct HitEntry* hitBreathout;

    if (party == NULL) {
        return NULL;
    }

    if (party->currentMemberId != PARTY_FLURRIE || (party->flags & PARTY_FLAG_IS_BEING_USED) == 0 ||
        party->useMotionId < CLOUD_USE_BREATH_INIT) {
        return NULL;
    }

    hitBreathout = NULL;
    if (which == 0) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitFront;
    } else if (which == 1) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitDown;
    } else if (which == 2) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitSweep[0];
    } else if (which == 3) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitSweep[1];
    }

    return hitBreathout;
}

/**
 * Current reach of Flurrie's gust in units (grows 2 per frame up to 120), or 0 when she is not mid-gust.
 */
f32 cloudGetBreathDist(void) {
#if VERSION <= VERSION_DEMO_JP
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
#else
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
#endif
    f32 breathDist;

    if (party == NULL) {
        breathDist = 0.0f;
        return breathDist;
    }

    if (party->currentMemberId != PARTY_FLURRIE || (party->flags & PARTY_FLAG_IS_BEING_USED) == 0 ||
        party->useMotionId < CLOUD_USE_BREATH_INIT) {
        breathDist = 0.0f;
    } else {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDist = cloud->breathDist;
    }

    return breathDist;
}

#if VERSION >= VERSION_US

/**
 * Lets Flurrie follow and bob again after a cutscene froze her,
 * and puts her back in her standing pose (PWD_S_1).
 * Called by the evt_party_cloud_lock_animations_on_off script helper (US and later).
 * Descriptive name (not in the symbol maps).
 */
void cloudLockAnimationsOff(PartyEntry* party) {
    party->work.cloud.lockAnims = 0;
    partyChgPose(party, "PWD_S_1");
}

/**
 * Freezes Flurrie for a cutscene: cloud_move stops following and bobbing, and she takes pose PWD_Z_1.
 * Called by the evt_party_cloud_lock_animations_on_off script helper (US and later).
 * Descriptive name (not in the symbol maps).
 */
void cloudLockAnimationsOn(PartyEntry* party) {
    party->work.cloud.lockAnims = 1;
    partyChgPose(party, "PWD_Z_1");
}
#endif

/** Resets Flurrie's idle bob: phase 0 degrees, advancing 4.25 degrees per frame. */
#if VERSION <= VERSION_DEMO_JP
static inline void cloud_hover_init(PartyEntry* party) {
#else
// Descriptive name (not in the symbol maps); placeholder unk_801820d0, JP 0x801820D0. See docs/NAME_PROVENANCE.md.
void cloud_hover_init(PartyEntry* party) {
#endif
    party->work.cloud.hoverAngle = 0.0f;
    party->work.cloud.hoverSpeed = 4.25f;
}

/** Advances Flurrie's idle bob and sets her display offset to a 0-6 unit sine wave above her position. */
#if VERSION <= VERSION_DEMO_JP
static inline void cloud_hover_main(PartyEntry* party) {
#else
// Descriptive name (not in the symbol maps); placeholder unk_8018206c, JP 0x8018206C. See docs/NAME_PROVENANCE.md.
void cloud_hover_main(PartyEntry* party) {
#endif
    f32 a;
    party->work.cloud.hoverAngle = revise360(party->work.cloud.hoverAngle - party->work.cloud.hoverSpeed);
    a = sin((3.1415927f * party->work.cloud.hoverAngle) / 180.0f);
    party->dispOfs.y = 3.0f + 3.0f * a;
}

/**
 * Flurrie's follow handler (party_data.c move slot): flies after Mario with partyFlyMain and bobs in the air.
 * The first frame after a run-mode change sets up the fly movement and the bob.
 */
void cloud_move(PartyEntry* party) {
#if VERSION >= VERSION_US
    f32 a;
#endif

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyMoveFlyInit(party, 1);
#if VERSION >= VERSION_US
        party->work.cloud.hoverAngle = 0.0f;
        party->work.cloud.hoverSpeed = 4.25f;
    }
    if (party->work.cloud.lockAnims == 0) {
        partyFlyMain(party);
        party->work.cloud.hoverAngle = revise360(party->work.cloud.hoverAngle - party->work.cloud.hoverSpeed);
        a = sin((3.1415927f * party->work.cloud.hoverAngle) / 180.0f);
        party->dispOfs.y = 3.0f + 3.0f * a;
        if (party->moveMode != PARTY_MOVE_MODE_BEHIND_MARIO) {
            party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
        }
#else
        cloud_hover_init(party);
    }
    partyFlyMain(party);
    cloud_hover_main(party);
    if (party->moveMode != PARTY_MOVE_MODE_BEHIND_MARIO) {
        party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
#endif
    }
}

// Flurrie's move target: beside Mario (3.3 units to his left or right of the facing direction), 10 units up
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void set_cloud_hold_pos(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 dir;

    party->moveTarget = mario->position;
    dir = mario->displayDirectionTarget;
    dir = toMovedir(dir + ((dir < 90.0f || dir >= 270.0f) ? -90.0f : 90.0f));
    movePos(&party->moveTarget.x, &party->moveTarget.z, 3.3f, dir);
    party->moveTarget.y += 10.0f;
}

/**
 * Flurrie's init handler (party_data.c): marks her a flying partner,
 * sets her collision size (38 tall, 36 wide in the early demos, 24 in JP and later) and clears her gust state.
 */
void cloud_init(PartyEntry* party) {
    party->memberKind = PARTY_FLURRIE;
    party->moveKind = 1;
    party->height = 38.0f;
#if VERSION <= VERSION_DEMO_JP
    party->width = 36.0f;
#else
    party->width = 24.0f;
#endif
    party->work.cloud.reserved_174 = 0;
    party->work.cloud.breathEffect = NULL;
    party->work.cloud.hoverAngleBits = 0;
    party->work.cloud.hoverSpeedBits = 0;
#if VERSION >= VERSION_US
    party->work.cloud.lockAnims = 0;
#endif
    party->useStruct = NULL;
    party->work.cloud.breathEffect = NULL;
}

/**
 * Gale Force (X): Flurrie moves beside Mario over 4 frames,
 * then blows a gust while X is held (or for actionTime ms when a script started it),
 * casting rays that map objects and NPCs read; ends by freeing the work and following.
 */
void cloud_use(PartyEntry* party) {
#if VERSION >= VERSION_JP
    MarioWork* mario = party->playerPtr;

    _cloud_use(party);
    if (mario->currMotionId == MARIO_MOTION_CLOUD || mario->currMotionId <= MARIO_MOTION_WALK || mario->currMotionId == MARIO_MOTION_DASH) {
        return;
    }
    party->flags &= ~PARTY_FLAG_IS_BEING_USED;

    if (CLOUD_WORK(party) != NULL && CLOUD_WORK(party)->sfxId != -1) {
        psndSFXOff(CLOUD_WORK(party)->sfxId);
        CLOUD_WORK(party)->sfxId = -1;
    }
    if (party->work.cloud.breathEffect != NULL) {
        effSoftDelete(party->work.cloud.breathEffect);
        party->work.cloud.breathEffect = NULL;
    }
    set_cloud_hold_pos(party);
    party->position = party->moveTarget;
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    party->flags &= ~PARTY_FLAG_IS_EVT_USE;
    if (party->useStruct != NULL) {
        __memFree(HEAP_DEFAULT, party->useStruct);
        party->useStruct = NULL;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
    } else {
        partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
        partyChgRunMode(party, PARTY_RUN_MOVE);
        partyChgMot(party, PARTY_MOTION_STAY);
    }
}

/**
 * The gust itself: Flurrie moves beside Mario over 4 frames,
 * then blows while X is held (or for actionTime ms when a script started it),
 * casting the rays map objects and NPCs read; 200 ms after release she goes back to following.
 * Descriptive name (not in the symbol maps).
 */
void _cloud_use(PartyEntry* party) {
#endif
    MarioWork* mario = party->playerPtr;
    f32 ofsX, ofsZ;
    f32 nz, ny, nx, dist, hz, hy, hx;
    f32 yc, ys, dirCos, dirSin;
    HitEntry* hit;
    s32 flag;
    f32 dir;
    PartyCloud* cl;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->work.cloud.hoverAngleBits = 0;
        party->work.cloud.hoverSpeedBits = 0;
        marioAdjustMoveDir();
        party->useMotionId = CLOUD_USE_APPROACH_INIT;
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(PartyCloud));
        memset(party->useStruct, 0, sizeof(PartyCloud));
#if VERSION <= VERSION_DEMO_JP
        CLOUD_WORK(party)->sfxId = -1;
#endif
        CLOUD_WORK(party)->reserved_0 = 100.0f;
        CLOUD_WORK(party)->breathDistMax = 120.0f;
        CLOUD_WORK(party)->sweep[0].angle = 0.0f;
        CLOUD_WORK(party)->sweep[0].reverse = 0;
        CLOUD_WORK(party)->sweep[1].angle = 0.0f;
        CLOUD_WORK(party)->sweep[1].reverse = 1;
        party->work.cloud.breathEffect = NULL;
#if VERSION >= VERSION_JP
        if (CLOUD_WORK(party) != NULL) {
            CLOUD_WORK(party)->sfxId = -1;
        }
#endif
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            CLOUD_WORK(party)->autoTimer = sysMsec2Frame(party->actionTime);
        }
    }

    switch (party->useMotionId) {
        case CLOUD_USE_APPROACH_INIT:
            mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->moveFrames = 4;
            party->useMotionId = CLOUD_USE_APPROACH;
        case CLOUD_USE_APPROACH:
            set_cloud_hold_pos(party);
            party->displayDirection = mario->displayDirection;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->position.x += (party->moveTarget.x - party->position.x) / (f32)party->moveFrames;
            party->position.y += (party->moveTarget.y - party->position.y) / (f32)party->moveFrames;
            party->position.z += (party->moveTarget.z - party->position.z) / (f32)party->moveFrames;
            if (--party->moveFrames > 0) {
                break;
            }
            party->useMotionId = CLOUD_USE_BREATH_INIT;
        case CLOUD_USE_BREATH_INIT:
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            if (party->work.cloud.breathEffect == NULL) {
                party->work.cloud.breathEffect = effKumokumoN64Entry(0, 0, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f, 0.0f);
                breathParticles(party)->camera = party->camId;
                breathParticles(party)->r1 = 0x4C;
                breathParticles(party)->g1 = 0xA7;
                breathParticles(party)->b1 = 0xCA;
                breathParticles(party)->r2 = 0x7D;
                breathParticles(party)->g2 = 0x1B;
                breathParticles(party)->b2 = 0xF5;
            }
            marioChgMot(MARIO_MOTION_CLOUD);
            marioChgPose("M_A_2A");
            partyChgPose(party, "PWD_A_5");
            party->moveFrames = 16;
            party->useMotionId = CLOUD_USE_BREATH_START;
            mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
            mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            CLOUD_WORK(party)->breathDist = 0.0f;
            CLOUD_WORK(party)->breathDir = mario->displayDirection;
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                mario->camShiftRate.x = 0.03f;
                mario->camShiftTargetX = 70.0f * (revise360(270.0f - mario->displayDirectionTarget) <= 180.0f ? -1.0f : 1.0f);
            }
#if VERSION <= VERSION_DEMO_JP
            CLOUD_WORK(party)->sfxId = psndSFXOn_3D((const char*)PSND_SFX_PARTY_CLAUD_WIND1, &mario->position);
#else
            if (CLOUD_WORK(party) != NULL && CLOUD_WORK(party)->sfxId == -1) {
                CLOUD_WORK(party)->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CLAUD_WIND1), &party->playerPtr->position);
            }
#endif
        case CLOUD_USE_BREATH_START:
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            if (--party->moveFrames > 0) {
                break;
            }
            partyChgPose(party, "PWD_A_6");
#if VERSION >= VERSION_US
            if (mario->currMotionId == MARIO_MOTION_CLOUD)
#endif
            marioChgPose("M_A_2B");
            party->useMotionId = CLOUD_USE_BREATHING;
        case CLOUD_USE_BREATHING:
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            partyUpdateKeyData(party);
            mario->displayDirection = CLOUD_WORK(party)->breathDir;
            mario->displayDirectionTarget = CLOUD_WORK(party)->breathDir;
            party->displayDirection = CLOUD_WORK(party)->breathDir;
            party->displayDirectionTarget = CLOUD_WORK(party)->breathDir;
            ofsX = mario->position.x;
            ofsZ = mario->position.z;
            flag = 0;
            dir = mario->displayDirection;
            if (dir < 90.0f || dir >= 270.0f) {
                flag = 1;
            }
            movePos(&ofsX, &ofsZ, 3.3f, toMovedir(dir + (flag ? -90.0f : 90.0f)));
            party->position.x = ofsX;
            party->position.z = ofsZ;
            party->position.y = 10.0f + mario->position.y;
            {
                Vec zero = {0.0f, 0.0f, 0.0f};
                party->dispOfs = zero;
            }
            CLOUD_WORK(party)->breathDist += 2.0f;
            cl = CLOUD_WORK(party);

            // re-reads the player pointer mario already holds (the reload is in the retail code)
            mario = party->playerPtr;
            if (cl->breathDist >= cl->breathDistMax) {
                cl->breathDist = cl->breathDistMax;
            }
            sincosf(&dirSin, &dirCos, toMovedir(CLOUD_WORK(party)->breathDir));
            sincosf(&ys, &yc, -45.0f);
            dist = CLOUD_WORK(party)->breathDist;
            hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                                 party->position.x, 18.0f + mario->position.y, party->position.z,
                                 dirSin, ys, dirCos);
            if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
                hit = NULL;
            }
            CLOUD_WORK(party)->hitDown = hit;
            getHitBreatheout2(party, 0.0f);
            if (++CLOUD_WORK(party)->breathLevel > 2) {
                CLOUD_WORK(party)->breathLevel = 0;
            }
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                if (party->buttonHold & PAD_BUTTON_X) {
                    if (marioKeyOffChk() == 0) {
                        break;
                    }
                }
            } else if (--CLOUD_WORK(party)->autoTimer > 0) {
                break;
            }
#if VERSION <= VERSION_DEMO_JP
            if (CLOUD_WORK(party)->sfxId != -1) {
                psndSFXOff(CLOUD_WORK(party)->sfxId);
                CLOUD_WORK(party)->sfxId = -1;
            }
#endif
            CLOUD_WORK(party)->hitFront = NULL;
            CLOUD_WORK(party)->hitDown = NULL;
            CLOUD_WORK(party)->hitSweep[0] = NULL;
            CLOUD_WORK(party)->hitSweep[1] = NULL;
            party->moveFrames = sysMsec2Frame(200);
            party->useMotionId = CLOUD_USE_BREATH_FINISH;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                party->useMotionId = CLOUD_USE_CLEANUP;
#if VERSION >= VERSION_JP
            }
            if (CLOUD_WORK(party) != NULL && CLOUD_WORK(party)->sfxId != -1) {
                psndSFXOff(CLOUD_WORK(party)->sfxId);
                CLOUD_WORK(party)->sfxId = -1;
#endif
            }
            break;
        case CLOUD_USE_BREATH_FINISH:
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            partyUpdateKeyData(party);
            if (party->buttonTrg & PAD_BUTTON_X) {
                party->useMotionId = CLOUD_USE_BREATH_INIT;
            }
            if (--party->moveFrames <= 0) {
                party->useMotionId = CLOUD_USE_CLEANUP;
                break;
            }
            if (party->stickX == 0 && party->stickY == 0) {
                break;
            }
            party->useMotionId = CLOUD_USE_CLEANUP;
        case CLOUD_USE_CLEANUP:
#if VERSION <= VERSION_DEMO_JP
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
            marioAdjustMoveDir();
            party->trigFlags |= PARTY_TRIG_HALF_PUSH;
#else
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            if (CLOUD_WORK(party) != NULL && CLOUD_WORK(party)->sfxId != -1) {
                psndSFXOff(CLOUD_WORK(party)->sfxId);
                CLOUD_WORK(party)->sfxId = -1;
            }
            if (party->work.cloud.breathEffect != NULL) {
                effSoftDelete(party->work.cloud.breathEffect);
                party->work.cloud.breathEffect = NULL;
            }
            set_cloud_hold_pos(party);
            party->position = party->moveTarget;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            if (mario->currMotionId == MARIO_MOTION_CLOUD) {
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
                marioAdjustMoveDir();
                party->trigFlags |= PARTY_TRIG_HALF_PUSH;
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                marioChgMot(MARIO_MOTION_STAY);
            }
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
#endif
            partyChgRunMode(party, PARTY_RUN_MOVE);
            party->moveArg = 0;
            partyChgMoveMode(party, PARTY_MOVE_MODE_BEHIND_MARIO);
            partyChgMot(party, PARTY_MOTION_WALK);
#if VERSION <= VERSION_DEMO_JP
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            marioChgMot(MARIO_MOTION_STAY);
#endif
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                mario->camShiftRate.x = 0.01f;
                mario->camShiftTargetX = 0.0f;
            }
            party->flags &= ~PARTY_FLAG_IS_EVT_USE;
#if VERSION <= VERSION_DEMO_JP
            if (party->work.cloud.breathEffect != NULL) {
                effSoftDelete(party->work.cloud.breathEffect);
                party->work.cloud.breathEffect = NULL;
            }
#endif
            if (party->useStruct != NULL) {
                __memFree(HEAP_DEFAULT, party->useStruct);
                party->useStruct = NULL;
            }
            break;
    }
}

/**
 * Flurrie's exit handler (party_data.c), run when she leaves the field:
 * stops the gust sound and effect, frees the gust work
 * and puts Mario back in his standing motion with the camera shift cleared.
 */
void cloud_exit(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
#if VERSION >= VERSION_JP
    if (CLOUD_WORK(party) != NULL && CLOUD_WORK(party)->sfxId != -1) {
        psndSFXOff(CLOUD_WORK(party)->sfxId);
        CLOUD_WORK(party)->sfxId = -1;
    }
#endif
    if (party->work.cloud.breathEffect != NULL) {
        effSoftDelete(party->work.cloud.breathEffect);
        party->work.cloud.breathEffect = NULL;
    }
    if (party->useStruct != NULL) {
#if VERSION <= VERSION_DEMO_JP
        if (CLOUD_WORK(party)->sfxId != -1) {
            psndSFXOff(CLOUD_WORK(party)->sfxId);
            CLOUD_WORK(party)->sfxId = -1;
        }
#endif
        __memFree(HEAP_DEFAULT, party->useStruct);
        party->useStruct = NULL;
    }
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    marioAdjustMoveDir();
    marioChgMot(MARIO_MOTION_STAY);
    mario->camShiftRate.x = 0.01f;
    mario->camShiftTargetX = 0.0f;
}

#if VERSION <= VERSION_DEMO_JP

f32 cloudGetBreathPower(Vec pos, f32 width) {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathPower;
    f32 dist;
    f32 angle;
    f32 breathDist;

    breathPower = 0.0f;
    if (party == NULL) {
        return breathPower;
    }

    if (
        party->currentMemberId == PARTY_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) != 0 && party->useMotionId >= MARIO_MOTION_FALL && ((__fabsf(party->position.y - pos.y - 18.0f)) <= 10.0f)) {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDist = cloud->breathDist;
        if (-((0.5f * width) - distABf(party->position.x, party->position.z, pos.x, pos.z)) <= breathDist) {
            PartyCloud* cloud = (PartyCloud*)party->useStruct;
            dist = toMovedir(cloud->breathDir);
            angle = angleABf(party->position.x, party->position.z, pos.x, pos.z);
            dist = revise360(angle - dist);

            if (dist <= 30.0f || dist >= 330.0f) {
                breathPower = cos((3.1415927f * dist) / 180.0f);
                breathPower = 100.0f * __fabsf(breathPower);
            }
        }
    }

    return breathPower;
}

f32 cloudGetBreathDir() {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathDir;

    if (party == NULL) {
        return 0.0f;
    }

    if ((party->flags & PARTY_FLAG_IS_BEING_USED) != 0 && (party->useMotionId < MARIO_MOTION_FALL)) {
        breathDir = -1.0f;
    } else if (party->useStruct == NULL) {
        breathDir = 0.0f;
    } else {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDir = toMovedir(cloud->breathDir);
    }

    return breathDir;
}
#endif

/**
 * Per-frame gust update: casts the straight ray at one of three heights (8, 20, 40 above Mario, cycling each
 * frame), places and scales the cloud effect in front of Flurrie, then casts the two swinging side rays.
 * Rays that hit collision with attribute 4 are ignored.
 */
#if VERSION <= VERSION_DEMO_JP
static void getHitBreatheout2(PartyEntry* party, f32 angle) {
    f32 dirSin, dirCos, ys, yc;
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 ofsX, ofsZ;
#else
static void getHitBreatheout2(PartyEntry* party, f32 angle) {
    f32 dirSin, dirCos, ys, yc;
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 ofsZ, ofsX;
#endif
    f32 dir, sign, height;
    s16 kind;
    HitEntry* hit;
    PartyCloud* cl = CLOUD_WORK(party);
    MarioWork* mario = party->playerPtr;

    kind = cl->breathLevel;

    if (kind == 0) {
        height = 8.0f;
    } else if (kind == 1) {
        height = 20.0f;
    } else if (kind == 2) {
        height = 40.0f;
    }
    if (cl->breathDist >= cl->breathDistMax) {
        cl->breathDist = cl->breathDistMax;
    }
    sincosf(&ys, &yc, angle);
    dir = toMovedir(CLOUD_WORK(party)->breathDir);
    sincosf(&dirSin, &dirCos, dir);
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitFront = hit;
    ofsX = 0.0f;
    ofsZ = 0.0f;
#if VERSION <= VERSION_DEMO_JP
    movePos(&ofsX, &ofsZ, 24.0f, dir);
    breathParticles(party)->x = party->position.x + ofsX;
    breathParticles(party)->y = 18.0f + party->position.y;
    breathParticles(party)->z = party->position.z + ofsZ;
    cl = CLOUD_WORK(party);
    breathParticles(party)->scale = 0.6f * (cl->breathDist / cl->breathDistMax);
    cl = CLOUD_WORK(party);
    if (cl->breathDir >= 90.0f && cl->breathDir <= 270.0f) {
        breathParticles(party)->rot = -25.0f;
    } else {
        breathParticles(party)->rot = 205.0f;
#else
    if (party->work.cloud.breathEffect != NULL) {
        movePos(&ofsX, &ofsZ, 24.0f, dir);
        breathParticles(party)->x = party->position.x + ofsX;
        breathParticles(party)->y = 18.0f + party->position.y;
        breathParticles(party)->z = party->position.z + ofsZ;
        cl = CLOUD_WORK(party);
        breathParticles(party)->scale = 0.6f * (cl->breathDist / cl->breathDistMax);
        cl = CLOUD_WORK(party);
        if (cl->breathDir >= 90.0f && cl->breathDir <= 270.0f) {
            breathParticles(party)->rot = -25.0f;
        } else {
            breathParticles(party)->rot = 205.0f;
        }
#endif
    }
    cl = CLOUD_WORK(party);
    if (cl->breathDir >= 90.0f && cl->breathDir <= 270.0f) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }

    cloudSweep(party, 0);
    cl = CLOUD_WORK(party);
    sincosf(&dirSin, &dirCos, toMovedir(cl->sweep[0].angle * sign + cl->breathDir));
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitSweep[0] = hit;

    cloudSweep(party, 1);
    cl = CLOUD_WORK(party);
    sincosf(&dirSin, &dirCos, toMovedir(cl->sweep[1].angle * sign + cl->breathDir));
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitSweep[1] = hit;
}
#if VERSION <= VERSION_DEMO_JP

u8 cloudGetStatus() {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    u8 status = 2;

    if (party == NULL || (party->currentMemberId == PARTY_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        status = 0;
        return status;
    } else if (party->useMotionId == 10 && party->useMotionId < 20) {
        status = 1;
    }

    return status;
}

/**
 * Saves the 3D camera's look-at point into cloud_at and, if Mario's camera is zoomed in,
 * requests a zoom-out (marioCamZoomOffReq2(600)).
 * Called when Mario's Flurrie motion ends (mot_cloud_post in mario_motion.c).
 */
void cloudResetAt() {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);

    if (party == NULL || party->currentMemberId != PARTY_FLURRIE) {
        return;
    } else {
        cloud_at.x = camGetPtr(CAMERA_3D)->cameraAt.x;
        cloud_at.y = camGetPtr(CAMERA_3D)->cameraAt.y;
        cloud_at.z = camGetPtr(CAMERA_3D)->cameraAt.z;

        marioCamZoomOffReq2(600);
    }
}
#endif

/**
 * Called by marioCamZoomUp: with Mario's ZOOM flag clear, while the 3D camera's stateA is 0, remembers the given look-at point in cloud_at;
 * otherwise replaces it with the remembered point.
 * Does nothing with no partner or when Flurrie is out but not gusting.
 */
void cloudGetAt(Vec* position) {
    MarioWork* mario = marioGetPtr();
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);

    if (party == NULL || (party->currentMemberId == PARTY_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        return;
    }

    if ((mario->dispFlags & MARIO_DISP_FLAG_ZOOM) == 0) {
        CameraEntry* camera = camGetPtr(CAMERA_3D);

        if (camera->stateA == 0) {
            cloud_at = *position;
        } else {
            *position = cloud_at;
        }
    }
}

// FAKE: stand-in that nothing calls; it only puts 0.0f, 70.0f, 180.0f and the int-to-float conversion constant in
// this object's constant pool
void partyCloudStrippedPool(f32* f, s32 i) {
    f[0] = 0.0f;
    f[1] = 70.0f;
    f[2] = 180.0f;
    f[3] = (f32)i;
}
