// Mario's paper airplane motion (plane panel): take-off, gliding, landing and the camera while flying.

#include "version.h"
#include "dolphin/types.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cmd.h"
#include "evt/evt_snd.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mot/mot_plane.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_sbr.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "mario/mariost.h"
#include "manager/evtmgr.h"
#include <dolphin/pad.h>
#include "mario/mario_hit.h"
#include "party.h"
#include "mot/mot_damage.h"
#include "driver/animdrv.h"
#include "mot/mot_jump.h"

static BOOL chkCollision(f32* outY);
static void flyMain(void);


/**
 * Tuning values of Mario's paper airplane flight: top speed, gravity and drag, the nose-up and nose-down pitch
 * limits, turn and glide rates, and the pose offsets for each phase of the flight.
 */
f32 planeData[41] = {
    5.0f, -0.02f, -0.03f, 25.0f, -45.0f, 0.4f, 1.0f, 0.1f, -0.1f, 1.5f,
    0.2f, 0.05f, 3.4f, 1.0f, 0.025f, 0.045f, 0.8f, 1.5f, -0.3f, -5.0f,
    3.9f, -0.112f, 0.118f, 0.5f, 0.4f, -8.0f, -2.0f, -4.0f, -2.0f, 0.2f,
    0.0f, 1.2f, 1.0f, 15.0f, -30.0f, 12.0f, 0.0f, 30.0f, -30.0f, 6.0f,
    28.0f,
};

/**
 * Plays the paper airplane fold sound (SFX_MARIO_AIRPLANE1) four times, 12 frames apart,
 * at the position in LW(0..2) while Mario folds into the plane on take-off.
 */
EVT_BEGIN(sound_evt)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_AIRPLANE1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(12)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_AIRPLANE1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(12)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_AIRPLANE1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(12)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_AIRPLANE1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

static inline void planeSetCamZoom(f32 targetZ, f32 rate, f32 zoom) {
    MarioWork* mario = marioGetPtr();
    mario->camShiftTargetZ = targetZ;
    mario->camShiftRate.z = rate;
    mario->camZoom = zoom;
    mario->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

/** Paper airplane animation for each of Mario's colours (marioGetColor). */
const char* paper_plane[4] = {"p_hikohki", "p_hikohki_g", "p_hikohki_y", "p_hikohki_v"};

/** Stops the plane's looping flight sound if it is playing. */
static inline void planeSfxOff(void) {
    MarioWork* mario = marioGetPtr();
    // FAKE: unsigned compares with -1 (0xFFFFFFFF) on a signed sound id here and below in this function; the plain
    // signed `!= -1` / `== -1` compiles cmpwi instead of cmplwi and changes the object.
    if (mario->motStruct != NULL && (u32)mario->motPlane->sfxId != 0xFFFFFFFF) {
        psndSFXOff(mario->motPlane->sfxId);
        mario->motPlane->sfxId = -1;
    }
}

/**
 * Turns Mario back from the plane: normal 20x37 collision box, rotation and offsets cleared,
 * paper animation off, mirror and facing locks released.
 */
static inline void planeReset(void) {
    MarioWork* mario = marioGetPtr();
    mario->wPlayerCollisionBox.x = 20.0f;
    mario->wPlayerCollisionBox.y = 37.0f;
    mario->rot = (Vec){0.0f, 0.0f, 0.0f};
    mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    marioPaperOff();
    marioChgPaper(NULL);
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioAdjustMoveDir();
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
}

/**
 * Inline form of mario_plane_cancel (JP and later):
 * starts the landing (sub-motion 50) if Mario is in the gliding phase.
 */
static inline BOOL planeCancel(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_PLANE) {
        return FALSE;
    }
    if (mario->currSubMotionId >= 12 && mario->currSubMotionId < 20) {
        mario->currSubMotionId = 50;
        mario->mot.plane.groundY = mario->position.y;
        return TRUE;
    }
    return FALSE;
}

/**
 * Plane heading for the partner follow code: 0 when not flying,
 * 1 or -1 for the two directions (motPlane flag 1 set means 1).
 */
s32 marioPlaneGetFlyingDir(void) {
    MarioWork* mario = marioGetPtr();
    s32 ret;
    if (mario->currMotionId != MARIO_MOTION_PLANE) {
        return 0;
    }
    ret = -1;
    if (mario->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
        ret = 1;
    }
    return ret;
}

/**
 * 0 when Mario is not in the plane motion, 2 while gliding (sub-motions 12-19), 1 during take-off or landing.
 * Used by evt_mario and mario.c's pause-menu check (marioCheckMenuDisable).
 */
s32 marioGetPlaneStatus(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->currMotionId != MARIO_MOTION_PLANE) {
        return PLANE_STATUS_NONE;
    }
    if (mp->currSubMotionId >= 12 && mp->currSubMotionId < 20) {
        return PLANE_STATUS_GLIDING;
    }
    return PLANE_STATUS_TRANSITION;
}

/**
 * Ends a glide early: switches to the landing phase (sub-motion 50) from the current height.
 * Called from evt_mario's plane command and when the field motion is changed while flying.
 */
#if VERSION <= VERSION_DEMO_JP
void mario_plane_cancel(void) {
    MarioWork* mario = marioGetPtr();
    if ((u16)mario->currMotionId == MARIO_MOTION_PLANE && (s32)mario->currSubMotionId >= 12 && (s32)mario->currSubMotionId < 20) {
        mario->currSubMotionId = 50;
        mario->mot.plane.groundY = mario->position.y;
    }
#else
BOOL mario_plane_cancel(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_PLANE) {
        return FALSE;
    }
    if (mario->currSubMotionId >= 12 && mario->currSubMotionId < 20) {
        mario->currSubMotionId = 50;
        mario->mot.plane.groundY = mario->position.y;
        return TRUE;
    }
    return FALSE;
#endif

}

/**
 * Re-applies the paper airplane animation in Mario's current colour (paper_plane[marioGetColor()])
 * when mario.c rebuilds his look while flying.
 */
void marioForcePlaneAnime(void) {
    MarioWork* mario = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn(paper_plane[marioGetColor()]);
    marioChgPaper("PM_P_1B");
    marioChgPose("M_Z_1");
    mario->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
}

// Eases turnSpeed toward max, signed by the facing direction (+ right, - left).
static inline void planeRoll1(MarioWork* mp, f32 rate, f32 max) {
    f32* p = &mp->mot.plane.turnSpeed;
    f32 target = max * ((mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? 1.0f : -1.0f);
    *p += rate * (target - *p);
}

/** Eases the plane's turn speed toward max (degrees per frame) in the flying direction by rate. */
// Same easing as planeRoll1.
// FAKE: a second copy of planeRoll1's easing; calling planeRoll1 at both sites changes the object
static inline void planeRoll2(MarioWork* mario, f32 rate, f32 max) {
    f32* p = &mario->mot.plane.turnSpeed;
    *p += rate * (max * ((mario->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? 1.0f : -1.0f) - *p);
}

#if VERSION >= VERSION_US
/**
 * Height below which the camera stops following the plane down, per map:
 * tik_01, tik_19 (Rogueport Underground), mri_19 (Great Tree), las_25 (Palace of Shadow), 90 elsewhere.
 * US and later only.
 */
static inline f32 planeCamFloor(void) {
    if (strcmp(gp->currentMapName, "tik_01") == 0) {
        return 30.0f;
    } else if (strcmp(gp->currentMapName, "tik_19") == 0) {
        return -510.0f;
    } else if (strcmp(gp->currentMapName, "mri_19") == 0) {
        return -230.0f;
    } else if (strcmp(gp->currentMapName, "las_25") == 0) {
        return -510.0f;
    }
    return 90.0f;
}
#endif

/**
 * Plane Mode motion (from a plane panel): Mario jumps, spins and folds into a paper airplane,
 * then glides with the stick pitching it up and down (flyMain); A or B or a key-off lands him.
 * Handles landing on the ground or against a wall, hitting a ceiling (dropping straight down),
 * dropping into water (splash, then the water damage motion) and the camera following the flight.
 */
void mot_plane(void) {
    MarioWork* mp;
    BOOL done;
    EventEntry* evt;
    f32 y, b, c;
    Vec p;
    HitEntry* hit;
    MarioWork* m;
    MarioWork* m2;

    mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->motStruct = __memAlloc(HEAP_DEFAULT, sizeof(MotPlaneWork));
        memset(mp->motStruct, 0, sizeof(MotPlaneWork));
        m2 = marioGetPtr();
        m2->motPlane->sfxId = -1;
        mp->currSubMotionId = 0;
    }
    switch (mp->currSubMotionId) {
    case 0:
        planeSetCamZoom(0.4f, 0.01f, 2.0f);
        marioPreJump();
        marioChgPose("M_J_1B");
        mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mp->currSubMotionId = 1;
        mp->airTimer = 0;
        mp->baseSpeed = 0.0f;
        mp->mot.plane.turnSpeed = 0.0f;
        mp->mot.plane.startY = mp->position.y;
        mp->mot.plane.groundY = 0.0f;
        // bit 4 of the take-off panel is HIT_ATTR_PLANE_PANEL_R -> MOT_PLANE_FLAG_RIGHT
        mp->motPlane->flags = (hitGetAttr(mp->hitobjJumpFrom) >> 4) & 1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_AIRPLANE_JUMP1), &mp->position);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_5), &mp->position);
    case 1:
        marioMakeJumpPara();
        planeRoll1(mp, 0.125f, 30.0f);
        done = FALSE;
        mp->displayDirection += mp->mot.plane.turnSpeed;
        if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
            if (mp->displayDirection >= 630.0f) {
                done = TRUE;
            }
        } else {
            if (mp->displayDirection <= -630.0f) {
                done = TRUE;
            }
        }
        if (done) {
            mp->currSubMotionId = 2;
            mp->multiTimer = 20;
            marioPaperOn(paper_plane[marioGetColor()]);
            marioChgPaper("PM_P_1A");
            marioChgPose("M_Z_1");
            mp->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
            animPoseSetLocalTime(mp->paperPoseId, 0.0f);
            animPoseSetLocalTime(mp->paperAnimGroupId[marioAnimeId()], 0.0f);
            evt = evtEntry(sound_evt, 0, 0);
            evt->lwData[0] = FLOAT(mp->position.x);
            evt->lwData[1] = FLOAT(mp->position.y);
            evt->lwData[2] = FLOAT(mp->position.z);
        }
        if (mp->jumpVelocityY > 0.0f) {
            mp->position.y += mp->jumpVelocityY;
        }
        break;
    case 2:
        animPoseSetLocalTime(mp->paperPoseId, 0.0f);
        animPoseSetLocalTime(mp->paperAnimGroupId[marioAnimeId()], 0.0f);
        planeRoll1(mp, 0.125f, 30.0f);
        done = FALSE;
        mp->displayDirection += mp->mot.plane.turnSpeed;
        if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
            if (mp->displayDirection >= 1260.0f) {
                mp->displayDirection = 1260.0f;
                done = TRUE;
            }
        } else {
            if (mp->displayDirection <= -1080.0f) {
                mp->displayDirection = -1080.0f;
                done = TRUE;
            }
        }
        if (done) {
            mp->multiTimer = 60;
            mp->currSubMotionId = 3;
        }
        break;
    case 3:
        if (--mp->multiTimer > 0) {
            if (mp->multiTimer == 40) {
                allPartyRidePlane();
            }
            break;
        }
        marioChgPaper("PM_P_1B");
        mp->motPlane->yaw = (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? 90.0f : 270.0f;
        mp->mot.plane.turnSpeed = 0.0f;
        mp->motPlane->pitch = 0.0f;
        mp->motPlane->roll = 0.0f;
        mp->motPlane->pitchSpeed = 0.0f;
        mp->motPlane->diveSpeed = 0.0f;
        if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
            mp->rotOfs.x = planeData[33];
            mp->dispOfs.x = planeData[34];
        } else {
            mp->rotOfs.x = planeData[35];
            mp->dispOfs.x = planeData[36];
        }
        mp->wPlayerCollisionBox.x = 20.0f;
        mp->wPlayerCollisionBox.y = 12.333333f;
        // FAKE: dead store overwritten below (baseSpeed = 1.0f); removing it changes the object
        mp->baseSpeed = __fabsf(0.02f * mp->wStickDir1);
        mp->currSubMotionId = 12;
        mp->baseSpeed = 1.0f;
        mp->motPlane->diveSpeed = 2.0f;
        m = marioGetPtr();
        m->camShiftTargetZ = 0.0f;
        m->camShiftRate.z = 0.004f;
        mp->camShiftTargetX = 70.0f * ((mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? -1.0f : 1.0f);
        mp->camShiftTargetY = -70.0f;
        break;
    }
#if VERSION >= VERSION_US
    // US and later: per-map floor below which the camera stops following the plane
    if (mp->position.y < planeCamFloor()) {
        mp->camShiftTargetY = __fabsf(planeData[38]);
        camFollowYOff();
#else
    if (mp->position.y < 100.0f) {
        mp->camShiftTargetY = __fabsf(planeData[38]);
#endif
    }
    flyMain();
    if (marioKeyOffChk() && !(mp->trigFlags & MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON)) {
#if VERSION <= VERSION_DEMO_JP
        MarioWork* m = marioGetPtr();
        if ((u16)m->currMotionId == MARIO_MOTION_PLANE && (s32)m->currSubMotionId >= 12 && (s32)m->currSubMotionId < 20) {
            m->currSubMotionId = 50;
            m->mot.plane.groundY = m->position.y;
        }
        return;
    }
    if ((s32)mp->currSubMotionId == 20 || (s32)mp->currSubMotionId == 30) {
#else
        if (planeCancel()) {
            return;
        }
    }
    if (mp->currSubMotionId == 20 || mp->currSubMotionId == 30) {
#endif
        camFollowYOff();
        mp->camShiftTargetY = 0.0f;
        mp->camShiftTargetX = 0.0f;
        marioClearJumpPara();
        mp->baseSpeed = 0.0f;
        planeSfxOff();
        planeReset();
        allPartyRideOff2();
        if (mp->currSubMotionId == 20) {
            marioChgMot(MARIO_MOTION_JUMP);
        } else if (mp->currSubMotionId == 30) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
    switch (mp->currSubMotionId) {
    case 40:
        camFollowYOff();
        planeSfxOff();
        mp->multiTimer = sysMsec2Frame(520);
        mp->currSubMotionId = 41;
        break;
    case 41:
        if (--mp->multiTimer > 0) {
            break;
        }
        planeReset();
        marioChgPose("M_J_1C");
        mp->position.y = 1.0f + (mp->mot.plane.groundY - 37.0f);
        mp->baseSpeed = 0.0f;
        marioSetFallPara();
        mp->jumpVelocityY = 0.0f;
        mp->jumpAccelerationY *= 0.5f;
        mp->jumpJerkY *= 0.5f;
        mp->jumpSnapY *= 0.5f;
        mp->currSubMotionId = 90;
        mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        allPartyRideOff2();
        break;
    case 60:
        camFollowYOff();
        mp->multiTimer = sysMsec2Frame(520);
        mp->currSubMotionId = 61;
    case 61:
        if (mp->baseSpeed > 1.0f) {
            mp->currSubMotionId = 14;
            break;
        }
        if (--mp->multiTimer > 0) {
            break;
        }
        mp->currSubMotionId = 52;
        break;
    case 50:
        camFollowYOff();
        mp->multiTimer = sysMsec2Frame(520);
        mp->currSubMotionId = 52;
        break;
    case 51:
        camFollowYOff();
        mp->multiTimer = sysMsec2Frame(100);
        mp->currSubMotionId = 52;
        break;
    case 52:
        if (--mp->multiTimer > 0) {
            break;
        }
    case 53:
        planeSfxOff();
        planeReset();
        mp->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mp->baseSpeed = 0.0f;
        mp->mot.plane.turnSpeed = 0.0f;
        mp->position.y = 1.0f + mp->mot.plane.groundY;
        marioPreJump();
        mp->jumpVelocityY *= 0.5f;
        marioChgPose("M_J_1B");
        mp->currSubMotionId = 80;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_AIRPLANE3), &mp->position);
        allPartyRideOff2();
    case 80:
        planeRoll2(mp, 0.3f, 28.0f);
        mp->displayDirection = revise360(mp->displayDirection + mp->mot.plane.turnSpeed);
        marioMakeJumpPara();
        mp->position.y += mp->jumpVelocityY;
        marioChkOverhead();
        if (mp->jumpVelocityY < 0.0f || mp->hitobjHead != NULL) {
            marioSetFallPara();
            mp->jumpVelocityY *= 0.5f;
            mp->jumpAccelerationY *= 0.5f;
            mp->jumpJerkY *= 0.5f;
            mp->jumpSnapY *= 0.5f;
            mp->currSubMotionId = 90;
            mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            mp->multiTimer = 20;
        }
        break;
    case 90:
        if (mp->multiTimer > 0) {
            planeRoll2(mp, 0.3f, 28.0f);
            mp->displayDirection = revise360(mp->displayDirection + mp->mot.plane.turnSpeed);
            if (--mp->multiTimer <= 0) {
                mp->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            }
        }
        marioMakeJumpPara();
        if (mp->jumpVelocityY <= -20.0f) {
            mp->jumpVelocityY = -20.0f;
        }
        mp->hitobjStandOn = NULL;
        hit = marioSearchGround(&y, &b, &c, mp->jumpVelocityY, mp->jumpVelocityY);
        if (hit != NULL) {
            mp->position.y = y;
            mp->hitobjStandOn = hit;
        } else {
            mp->position.y += mp->jumpVelocityY;
        }
        if (mp->hitobjStandOn != NULL) {
            camFollowYOn();
            allPartyRideOff2();
            mp->airTimer = 0;
            mp->jumpPeakAirTime = 0;
            mp->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
            marioAdjustMoveDir();
            planeReset();
            if (mp->motStruct != NULL) {
                __memFree(HEAP_DEFAULT, mp->motStruct);
                mp->motStruct = NULL;
            }
            mp->camShiftTargetX = 0.0f;
            mp->camShiftTargetY = 0.0f;
            marioChgMot(MARIO_MOTION_STAY);
        }
#if VERSION >= VERSION_US
        marioChkWallAround(&mp->position, 0, 0.0f, 0.0f, mp->wPlayerCollisionBox.z, mp->wPlayerCollisionBox.y);
#endif
        break;
    case 70:
#if VERSION >= VERSION_JP
        set_damage_root_ypos(mp->position.y);
#endif
        p = mp->position;
        marioSetSplash(0, &p);
        mp->multiTimer = 16;
        mp->currSubMotionId = 71;
        mp->jumpVelocityY = -2.0f;
        mp->jumpAccelerationY = 0.0f;
        mp->jumpJerkY = 0.0f;
        mp->jumpSnapY = 0.0f;
        camFollowYOff();
    case 71:
        marioMakeJumpPara();
        mp->position.y += mp->jumpVelocityY;
        if (mp->position.y >= mp->mot.plane.groundY - 100.0f) {
            break;
        }
        mp->currSubMotionId = 72;
    case 72:
        mp->camShiftTargetY = 0.0f;
        mp->camShiftTargetX = 0.0f;
        marioClearJumpPara();
        planeSfxOff();
        mp->baseSpeed = 0.0f;
        planeReset();
        allPartyRideOff2();
        marioSetFallPara();
        camFollowYOn();
        marioChgMot(MARIO_MOTION_DAMAGE);
    }
    marioUpdateCamCollision(mp->position);
    mp->moveDist = mp->baseSpeed;
}



#define PI 3.1415927f

/**
 * TRUE when the floor under the plane has one of the attributes 0x38 (0x8 and the plane panel bits 0x10/0x20);
 * over such a floor flyMain speeds the plane up (to 2) and keeps its dive limit at -22.
 */
static inline BOOL planeChkUnder(void) {
    BOOL ret = FALSE;
    MarioWork* mario = marioGetPtr();
    if (mario->hitobjUnder != NULL && (hitGetAttr(mario->hitobjUnder) & 0x38)) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Per-frame flight: the climb into the glide, then pitch from the stick,
 * speed gain when diving and loss when climbing (planeData constants),
 * the yaw clamped to stay in the flying direction, and the collision checks that end the flight on the ground,
 * a wall or water.
 */
static void flyMain(void) {
    f32 s2, c2, s, c;
    MarioWork* mp;
    f32 t;
    f32 a;
    f32 v;
    BOOL yawInLeftHalf;
    s32 stallFrames;
    f32 speedSin;
    MarioWork* m;

    mp = marioGetPtr();
    switch (mp->currSubMotionId) {
    case 12:
        if (mp->mot.plane.startY >= mp->position.y) {
            mp->currSubMotionId = 14;
        }
        m = marioGetPtr();
        // FAKE: unsigned compares with -1 (0xFFFFFFFF) on a signed sound id here and below in this function; the plain
        // signed `!= -1` / `== -1` compiles cmpwi instead of cmplwi and changes the object.
        if ((u32)m->motPlane->sfxId == 0xFFFFFFFF) {
            m->motPlane->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_AIRPLANE2), &m->position);
        }
    case 14:
        if (mp->dispOfs.y > planeData[38]) {
            mp->dispOfs.y -= 2.0f;
            mp->rotOfs.y += 2.0f;
            mp->position.y += 2.0f;
        } else {
            mp->currSubMotionId = 15;
        }
        break;
    }
    if (mp->currSubMotionId != 15) {
        return;
    }
    if (mp->buttonTrg & PAD_BUTTON_A) {
        mp->currSubMotionId = 51;
        mp->mot.plane.groundY = mp->position.y;
        return;
    }
    if (mp->buttonTrg & PAD_BUTTON_B) {
        mp->currSubMotionId = 51;
        mp->mot.plane.groundY = mp->position.y;
        return;
    }
    mp->motPlane->yaw = revise360(mp->motPlane->roll * planeData[14] + mp->motPlane->yaw);
    if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
        if (mp->motPlane->yaw <= 60.0f) {
            mp->motPlane->yaw += 0.1f * (60.0f - mp->motPlane->yaw);
        }
        if (mp->motPlane->yaw >= 150.0f) {
            mp->motPlane->yaw += 0.1f * (150.0f - mp->motPlane->yaw);
        }
    } else {
        if (mp->motPlane->yaw >= 330.0f) {
            mp->motPlane->yaw += 0.1f * (330.0f - mp->motPlane->yaw);
        }
        if (mp->motPlane->yaw <= 210.0f) {
            mp->motPlane->yaw += 0.1f * (210.0f - mp->motPlane->yaw);
        }
    }
    yawInLeftHalf = FALSE;
    t = mp->motPlane->yaw;
    if (t >= 90.0f && t <= 270.0f) {
        yawInLeftHalf = TRUE;
    }
    mp->displayDirection = revise360(yawInLeftHalf ? -t - 90.0f : 270.0f - t);
    if (mp->wStickDir1 != 0) {
        if (mp->baseSpeed) {
            a = __fabsf((f32)mp->wStickDir1);
            if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
                if (mp->wStickDir1 < 0) {
                    if (mp->motPlane->pitch >= planeData[3]) {
                        mp->motPlane->pitch += 0.1f * (planeData[3] - mp->motPlane->pitch);
                    } else {
                        mp->motPlane->pitch += 0.1f * (a * planeData[5]);
                    }
                }
                if (mp->wStickDir1 > 0) {
                    if (mp->motPlane->pitch <= planeData[4]) {
                        mp->motPlane->pitch = planeData[4];
                    } else {
                        mp->motPlane->pitch -= 0.1f * (a * planeData[6]);
                    }
                }
            } else {
                if (mp->wStickDir1 > 0) {
                    if (mp->motPlane->pitch >= planeData[3]) {
                        mp->motPlane->pitch += 0.1f * (planeData[3] - mp->motPlane->pitch);
                    } else {
                        mp->motPlane->pitch += 0.1f * (a * planeData[5]);
                    }
                }
                if (mp->wStickDir1 < 0) {
                    if (mp->motPlane->pitch <= planeData[4]) {
                        mp->motPlane->pitch = planeData[4];
                    } else {
                        mp->motPlane->pitch -= 0.1f * (a * planeData[6]);
                    }
                }
            }
        }
    } else {
        if (mp->motPlane->pitch <= 0.0f || mp->motPlane->pitchSpeed <= 0.0f) {
            if (mp->motPlane->pitchSpeed > 0.0f && mp->motPlane->pitchSpeed < 1.0f) {
                mp->motPlane->pitch += planeData[30];
            }
            t = planeData[0] - mp->baseSpeed;
            if (t <= 0.0f) {
                t = 0.0f;
            }
            t *= 90.0f / planeData[0];
            t = planeData[27] * (f32)sin(PI * t / 180.0f);
            if (t <= planeData[28]) {
                t = planeData[28];
            }
            if (!planeChkUnder()) {
                mp->motPlane->pitchRecover = t;
                mp->motPlane->pitch += mp->motPlane->pitchRecover;
            } else {
                mp->motPlane->pitch += -0.001f;
                mp->baseSpeed += 0.5f;
                if (mp->baseSpeed > 2.0f) {
                    mp->baseSpeed = 2.0f;
                }
            }
        }
        if (mp->motPlane->pitch <= planeData[4]) {
            mp->motPlane->pitch = planeData[4];
        }
        if (mp->motPlane->pitch >= 6.0f) {
            mp->motPlane->pitchSpeed = 0.0f;
        }
    }
    mp->baseSpeed += mp->baseSpeed * planeData[1];
    if (mp->motPlane->pitch >= 0.0f) {
        mp->motPlane->flags |= MOT_PLANE_FLAG_DIVE_UNUSED;
        a = (f32)sin(PI * (mp->motPlane->pitch * planeData[7]) / 180.0f);
        a = mp->baseSpeed * (planeData[8] * a);
        mp->baseSpeed = mp->baseSpeed + a;
        if (mp->wStickDir1 != 0 && mp->motPlane->pitch >= planeData[3]) {
            mp->baseSpeed += planeData[2];
        }
        if (mp->baseSpeed <= 0.0f) {
            mp->baseSpeed = 0.0f;
        }
        t = 0.0f;
        if (mp->wStickDir1 == 0) {
            if (mp->baseSpeed < planeData[20]) {
                t = planeData[0] - mp->baseSpeed;
                t *= planeData[21] * (f32)sin(PI * mp->motPlane->pitch / 180.0f);
                t += mp->motPlane->pitchSpeed;
                mp->motPlane->pitchSpeed = t;
            } else {
                mp->motPlane->pitchSpeed = planeData[22];
            }
            mp->motPlane->pitch += mp->motPlane->pitchSpeed;
            if (t <= 0.0f && mp->baseSpeed <= planeData[0] * planeData[23]) {
                t = planeData[0] - mp->baseSpeed * planeData[24];
                if (t <= 0.0f) {
                    t = 0.0f;
                }
                t *= 90.0f / planeData[0];
                if (!planeChkUnder()) {
                    a = planeData[25] * (f32)sin(PI * t / 180.0f);
                    if (a <= planeData[26]) {
                        a = 2.0f * planeData[26];
                    }
                    mp->motPlane->pitchRecover = a;
                }
            }
            mp->motPlane->pitch += mp->motPlane->pitchRecover;
        }
        if (mp->wStickDir1 != 0) {
            v = planeData[18] * (f32)sin(PI * (planeData[0] - mp->baseSpeed) / 180.0f);
            mp->motPlane->sinkSpeed += v;
            if (mp->motPlane->sinkSpeed <= planeData[19]) {
                mp->motPlane->sinkSpeed = planeData[19];
            }
        } else {
            v = mp->motPlane->sinkSpeed + 0.1f * (0.0f - mp->motPlane->sinkSpeed);
            mp->motPlane->sinkSpeed = v;
        }
        mp->motPlane->pitch += v;
    } else if (mp->motPlane->pitch >= -80.0f) {
        if (mp->motPlane->flags & MOT_PLANE_FLAG_DIVE_UNUSED) {
            mp->motPlane->flags &= ~MOT_PLANE_FLAG_DIVE_UNUSED;
        }
        mp->motPlane->diveSpeed = planeData[10] * (f32)sin(PI * (planeData[9] * __fabsf(mp->motPlane->pitch)) / 180.0f);
        mp->baseSpeed += mp->motPlane->diveSpeed;
        a = (mp->motPlane->pitch > 0.0f) ? 0.0f : __fabsf(mp->motPlane->pitch);
        t = (f32)sin(PI * (planeData[12] * a) / 180.0f);
        speedSin = (f32)sin(PI * (mp->baseSpeed * planeData[11]) / 180.0f);
        a = planeData[13] * (speedSin * t);
        mp->motPlane->pitchSpeed = mp->motPlane->pitchSpeed + a;
        if (mp->wStickDir1 != 0) {
            if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
                if (mp->wStickDir1 > 0 && mp->motPlane->pitchSpeed >= 1.0f) {
                    mp->motPlane->pitchSpeed = 1.0f;
                }
            } else {
                if (mp->wStickDir1 < 0 && mp->motPlane->pitchSpeed >= 1.0f) {
                    mp->motPlane->pitchSpeed = 1.0f;
                }
            }
            if (!planeChkUnder()) {
                mp->motPlane->pitchRecover += 0.1f * (1.0f - mp->motPlane->pitchRecover);
                if (mp->motPlane->pitchRecover >= 0.0f) {
                    mp->motPlane->pitchRecover = 0.0f;
                }
            }
        }
        if (planeChkUnder()) {
            mp->motPlane->pitchMin = -22.0f;
        } else {
            mp->motPlane->pitchMin += 0.1f * (planeData[4] - mp->motPlane->pitchMin);
        }
        mp->motPlane->pitch += mp->motPlane->pitchSpeed;
        if (mp->motPlane->pitch <= mp->motPlane->pitchMin) {
            mp->motPlane->pitch = mp->motPlane->pitchMin;
        }
        mp->motPlane->sinkSpeed = mp->motPlane->sinkSpeed + 0.1f * (0.0f - mp->motPlane->sinkSpeed);
    }
    if (mp->baseSpeed >= planeData[0]) {
        mp->baseSpeed = planeData[0];
    }
    mp->position.y += mp->motPlane->sinkSpeed;
    mp->position.y -= planeData[29];
    mp->rot.z = revise360(-mp->motPlane->pitch);
    mp->directionView = mp->motPlane->yaw;
    sincosf(&s, &c, mp->directionView);
    mp->position.z += mp->baseSpeed * c;
    sincosf(&s2, &c2, 90.0f + mp->motPlane->pitch);
    mp->position.y += (mp->baseSpeed * c2) * ((c2 >= 0.0f) ? planeData[31] : planeData[32]);
    mp->position.x += s2 * (mp->baseSpeed * s);
    if (mp->baseSpeed <= planeData[16]) {
        stallFrames = sysMsec2Frame(1000.0f * planeData[17]);
        if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
            if ((f32)mp->wStickDir1 < 0.0f) {
                if (++mp->motPlane->stallTimer >= stallFrames) {
                    mp->mot.plane.groundY = mp->position.y;
                    mp->currSubMotionId = 60;
                    return;
                }
            }
        } else {
            if ((f32)mp->wStickDir1 > 0.0f) {
                if (++mp->motPlane->stallTimer >= stallFrames) {
                    mp->mot.plane.groundY = mp->position.y;
                    mp->currSubMotionId = 60;
                    return;
                }
            }
        }
    } else {
        mp->motPlane->stallTimer = 0;
    }
    mp->hitobjUnder = marioSearchUnder();
    if (chkCollision(&mp->mot.plane.groundY)) {
        if (mp->hitobjHead != NULL) {
            mp->currSubMotionId = 40;
        } else {
            HitEntry* h = NULL;
            mp->currSubMotionId = 50;
            if (mp->hitobjFront != NULL) {
                h = mp->hitobjFront;
            }
            if (mp->hitobjStandOn != NULL) {
                h = mp->hitobjStandOn;
            }
            if (h != NULL && (hitGetAttr(h) & HIT_ATTR_WATER_HAZARD)) {
                mp->currSubMotionId = 70;
            }
        }
    }
}

/**
 * Cleanup when the plane motion ends: camera follow and shift reset, flight sound off, Mario reset to normal,
 * the motion's work freed and the partners dismounted.
 */
void mot_plane_post(void) {
    MarioWork* mario = marioGetPtr();
    camFollowYOn();
    planeSfxOff();
    planeReset();
    mario->camShiftTargetX = 0.0f;
    mario->camShiftTargetY = 0.0f;
    if (mario->motStruct != NULL) {
        __memFree(HEAP_DEFAULT, mario->motStruct);
        mario->motStruct = NULL;
    }
    allPartyRideOff2();
}

// FAKE: stand-in that nothing calls; it only places its 270.0f literal in this object's constant pool
static f32 planeDead270(void) {
    return 270.0f;
}

/**
 * Checks the plane against the world: rays ahead of its front point for walls (mario->hitobjFront),
 * downward rays under its front and nose points for a floor (mario->hitobjStandOn,
 * plus from JP on a ray between the two), and with no floor a short upward ray for a ceiling
 * (mario->hitobjHead). Returns TRUE on any hit, with the hit height in *outY.
 */
static BOOL chkCollision(f32* outY) {
    Vec dir, hpos, v, v2;
    f32 s1, c1, s0, c0, s2, c2;
    f32 ox, oy, oz, dist, nx, ny, nz;
    f32 x, y, z, r;
    HitEntry* hit;
    BOOL ret;
    MarioWork* mario;

    r = 4.0f;
    ret = FALSE;
    mario = marioGetPtr();
    sincosf(&s0, &c0, 90.0f + mario->motPlane->pitch);
    sincosf(&s1, &c1, mario->motPlane->yaw);
    x = mario->rotOfs.x + (mario->position.x + mario->dispOfs.x);
    y = mario->rotOfs.y + (mario->position.y + mario->dispOfs.y);
    z = mario->rotOfs.z + (mario->position.z + mario->dispOfs.z);
    x += s1 * c0 * r;
    y += -s0 * r;
    z += c1 * c0 * r;
    v = (Vec){x, y, z};
    mario->motPlane->tipPos = v;
    sincosf(&s2, &c2, mario->motPlane->roll);
    sincosf(&s0, &c0, mario->motPlane->pitch);
    mario->hitobjFront = NULL;
    r = planeData[40];
    sincosf(&s1, &c1, mario->motPlane->yaw);
    x = mario->rotOfs.x + (mario->position.x + mario->dispOfs.x);
    y = mario->rotOfs.y + (mario->position.y + mario->dispOfs.y);
    z = mario->rotOfs.z + (mario->position.z + mario->dispOfs.z);
    x += r * (s1 * -c0);
    z += r * (c1 * -c0);
    y += s0 * r;
    sincosf(&s0, &c0, mario->motPlane->pitch - 90.0f);
    x += planeData[39] * (c0 * s1) - planeData[39] * (s2 * c1);
    y += planeData[39] * (s0 * c2);
    z += planeData[39] * (c0 * c1) - planeData[39] * (s2 * s1);
    v2 = (Vec){x, y, z};
    mario->motPlane->frontPos = v2;
    x = mario->motPlane->frontPos.x - 10.0f * s1;
    y = mario->motPlane->frontPos.y;
    z = mario->motPlane->frontPos.z - 10.0f * c1;
    dist = 10.0f + mario->baseSpeed;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, s1, 0.0f, c1);
    if (hit != NULL) {
        mario->hitobjFront = hit;
        ret = TRUE;
        *outY = oy;
    }
    x = mario->motPlane->tipPos.x;
    y = mario->motPlane->tipPos.y;
    z = mario->motPlane->tipPos.z;
    dist = mario->baseSpeed;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, s1, 0.0f, c1);
    if (hit != NULL) {
        mario->hitobjFront = hit;
        ret = TRUE;
        *outY = oy;
    }
    r = 10.0f;
    x = mario->motPlane->frontPos.x;
    y = mario->motPlane->frontPos.y;
    z = mario->motPlane->frontPos.z;
    mario->hitobjStandOn = NULL;
    dist = 10.1f;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y + r, z, 0.0f, -1.0f, 0.0f);
    if (hit != NULL) {
        mario->hitobjStandOn = hit;
        ret = TRUE;
        *outY = oy;
    }
    if (!ret) {
        x = mario->motPlane->tipPos.x;
        y = mario->motPlane->tipPos.y;
        z = mario->motPlane->tipPos.z;
        mario->hitobjStandOn = NULL;
#if VERSION <= VERSION_DEMO_JP
        dist = 2.0f;
        hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, 0.0f, -1.0f, 0.0f);
#else
        dist = 10.1f;
        hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y + r, z, 0.0f, -1.0f, 0.0f);
#endif
        if (hit != NULL) {
            mario->hitobjStandOn = hit;
            ret = TRUE;
            *outY = oy;
#if VERSION >= VERSION_JP
        }
    }
    if (!ret) {
        PSVECSubtract(&mario->motPlane->tipPos, &mario->motPlane->frontPos, &dir);
        dist = PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        hit = marioHitCheckVec(&mario->motPlane->frontPos, &dir, &hpos, NULL, &dist);
        if (hit != NULL) {
            mario->hitobjStandOn = hit;
            ret = TRUE;
            *outY = hpos.y;
#endif
        }
    }
    mario->hitobjHead = NULL;
    if (mario->hitobjStandOn == NULL) {
        x = mario->motPlane->frontPos.x;
        y = mario->motPlane->frontPos.y;
        z = mario->motPlane->frontPos.z;
        dist = 2.0f;
        hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, 0.0f, 1.0f, 0.0f);
        if (hit != NULL) {
            mario->hitobjHead = hit;
            ret = TRUE;
            *outY = oy;
        }
    }
    return ret;
}

// FAKE: stand-in that nothing calls; it only places its 4.0f literal in this object's constant pool
static f32 planeDead4(void) {
    return 4.0f;
}
