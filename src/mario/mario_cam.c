// The field camera that follows Mario: target, zoom and shift per motion (walk, jump, slit, plane, cloud ride).

#include "version.h"
#include "mario/mario_cam.h"
#include "driver/camdrv.h"
#include "evt/evt_cam.h"
#include "manager/evtmgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "party.h"
#include "party/party_cloud.h"
#include "system.h"

/**
 * Clears the camera shift targets and zoom offsets so the camera recentres on Mario;
 * run when Mario is (re)initialised in mario.c.
 */
void marioResetCamShift(void) {
    MarioWork* mario = marioGetPtr();

    mario->camShiftTargetX = 0.0f;
    mario->camShiftTargetY = 0.0f;
    mario->camShiftTargetZ = 0.0f;
    mario->camZoomWeight = 0.0f;
    mario->camZoom = 2.0f;
    mario->camZoomOfs = (Vec){ 0.0f, 0.0f, 0.0f };
}

/**
 * Restores the default per-frame approach rates of the camera shift (x 0.03, y and z 0.06)
 * and clears the zoom offsets; run from mario.c at init and from Bowser's motions in koopa_motion.c.
 */
void marioResetCamShiftRate(void) {
    MarioWork* mario = marioGetPtr();

    mario->camShiftRate.x = 0.03;
    mario->camShiftRate.y = 0.06;
    mario->camShiftRate.z = 0.06;
    mario->camZoomWeight = 0.0f;
    mario->camZoom = 2.0f;
    mario->camZoomOfs = (Vec){ 0.0f, 0.0f, 0.0f };
}

/** Sets Mario back to drawing on the normal 3D camera (CAMERA_3D); run on map change and by evt_mario scripts. */
void marioInitCamId(void) {
    marioGetPtr()->camId = CAMERA_3D;
}

/** Camera Mario and the partners draw on (mario.c dispEntry, party.c copies it to each partner). */
s32 marioGetCamId(void) {
    return marioGetPtr()->camId;
}

/**
 * Moves Mario's drawing onto another camera, e.g. the effect cameras while an item is used (itemdrv.c)
 * or in tutorial and story scripts.
 */
void marioSetCamId(CameraId camId) {
    marioGetPtr()->camId = camId;
}

/** Sets the vertical camera follow rate back to the default 0.2; used when Mario lands, stands or walks. */
void marioResetCamFollowRate(void) {
    marioGetPtr()->camFollowRate = 0.2;
}

/** Sets how fast the 3D camera catches up to Mario vertically (fraction per frame); set during jumps in mot_jump.c. */
void marioSetCamFollowRate(f32 followRate) {
    marioGetPtr()->camFollowRate = followRate;
}

/** Vertical camera follow rate, read by camdrv.c when it moves the camera towards Mario's target height. */
f32 marioGetCamFollowRate(void) {
    return marioGetPtr()->camFollowRate;
}

/**
 * Copies the 3D camera's yaw into mario->targetYaw,
 * the screen-to-world angle offset used by toMovedir and the stick.
 * The demos pass yaw plus 0.05 degree to revise360 without rounding first; JP and later round it to 0.001 degree.
 */
void marioPreCamera(void) {
#if VERSION <= VERSION_DEMO_US
    marioGetPtr()->targetYaw = revise360((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);
}
#else
    f32 roundingFactor;
    if (camGetPtr(CAMERA_3D)->viewYaw >= 0.0f) {
        roundingFactor = 0.5f;
    } else {
        roundingFactor = -0.5f;
    }
    marioGetPtr()->targetYaw = revise360((s32)((1000.0f * camGetPtr(CAMERA_3D)->viewYaw) + roundingFactor) / 1000.0f);
}
#endif

/**
 * Per-frame camera update from Mario's main loop (also Bowser's and Peach's):
 * moves the follow target (except in a pipe), runs the level zoom and the zoom-off request,
 * then clears the camera collision flag for the next frame.
 */
void marioCamMain(void) {
    MarioWork* mario = marioGetPtr();
    CameraEntry* camera;
    MarioWork* mario2;

    if (mario->currMotionId != MARIO_MOTION_DOKAN) {
        marioUpdateCam();
    }
    marioCamZoomUpLevelMain();
    marioCamZoomOff();

    camera = camGetPtr(CAMERA_3D);
    mario2 = marioGetPtr();
    if (mario2->controlStickSensitivity) {
        if (camera->cameraPos.x == mario2->prevCamPosX) {
            mario2->dispFlags |= MARIO_DISP_FLAG_CAM_STILL_UNUSED;
        } else {
            mario2->dispFlags &= ~MARIO_DISP_FLAG_CAM_STILL_UNUSED;
        }
    }
    mario2->prevCamPosX = camera->cameraPos.x;
    mario->dispFlags &= ~MARIO_DISP_FLAG_CAM_COLLISION;
}

/**
 * Sets the point the camera should look at this frame instead of Mario (camdrv.c uses it while
 * MARIO_DISP_FLAG_CAM_COLLISION is set); used by the plane, tube, Spring Jump and Yoshi motions and mario_hit.c.
 */
void marioUpdateCamCollision(Vec collision) {
    MarioWork* mario = marioGetPtr();
    mario->camCollision = collision;
    mario->dispFlags |= MARIO_DISP_FLAG_CAM_COLLISION;
}

// A macro, not an inline function: as a function the load order in marioUpdateCam changes.
#define UPDATE_CAM_SHIFT(cur, target, rate) \
    {                                       \
        f32 c_ = cur;                       \
        f32 t_ = target;                    \
        f32 r_ = rate;                      \
        f32 d_;                             \
        if (c_ > t_) {                      \
            d_ = r_ * ((t_ - 1.0f) - c_);   \
            cur = c_ + d_;                  \
            if (cur < target) {             \
                cur = target;               \
            }                               \
        } else if (c_ < t_) {               \
            d_ = r_ * ((1.0f + t_) - c_);   \
            cur = c_ + d_;                  \
            if (cur > target) {             \
                cur = target;               \
            }                               \
        }                                   \
    }

/**
 * Eases the camera shift towards its targets and sets the camera target to Mario's position plus the shift along
 * the screen's x axis; with follow-y off only x/z move. The boat raises the target by 10 units.
 */
void marioUpdateCam(void) {
    MarioWork* mario = marioGetPtr();
    f32 sinYaw;
    f32 cosYaw;

    UPDATE_CAM_SHIFT(mario->camShiftX, mario->camShiftTargetX, mario->camShiftRate.x);
    UPDATE_CAM_SHIFT(mario->camShiftY, mario->camShiftTargetY, mario->camShiftRate.y);
    UPDATE_CAM_SHIFT(mario->camShiftZ, mario->camShiftTargetZ, mario->camShiftRate.z);

    if (!(mario->dispFlags & MARIO_DISP_FLAG_NO_UPDATE_CAM_POS)) {
        camGetPtr(CAMERA_3D);
        sincosf(&sinYaw, &cosYaw, camGetPtr(CAMERA_3D)->viewYaw - 90.0f);
        if (!(mario->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
            mario->camTarget.x = mario->position.x + sinYaw * mario->camShiftX;
            mario->camTarget.z = mario->position.z + cosYaw * mario->camShiftX;
            mario->camTarget.y = mario->camShiftY + (mario->position.y + mario->dispOfs.y);
            if (mario->currMotionId == MARIO_MOTION_SHIP) {
                mario->camTarget.y += 10.0f;
            }
            mario->camZoomWeight = mario->camShiftZ;
        } else {
            mario->camTarget.x = mario->position.x + sinYaw * mario->camShiftX + sinYaw * mario->camShiftZ;
            mario->camTarget.z = mario->position.z + cosYaw * mario->camShiftX + cosYaw * mario->camShiftZ;
            mario->camZoomWeight = mario->camShiftZ;
        }
    }
}

/** Lets marioUpdateCam move the camera target with Mario again. */
void marioUpdateCamPos(void) {
    marioGetPtr()->dispFlags &= ~MARIO_DISP_FLAG_NO_UPDATE_CAM_POS;
}

/** Freezes the camera target where it is (marioUpdateCam skips it); used by scripts and the damage motion. */
void marioNoUpdateCamPos(void) {
    marioGetPtr()->dispFlags |= MARIO_DISP_FLAG_NO_UPDATE_CAM_POS;
}

/** Lets the 3D camera follow Mario's height again (clears the follow-y-off flags on the camera and Mario). */
void camFollowYOn(void) {
    camGetPtr(CAMERA_3D)->flags &= ~CAMERA_FLAG_NO_Y_FOLLOW;
    marioGetPtr()->dispFlags &= ~MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF;
}

/** Stops the 3D camera following Mario's height, e.g. during a jump or pipe, so it does not bob up and down. */
void camFollowYOff(void) {
    camGetPtr(CAMERA_3D)->flags |= CAMERA_FLAG_NO_Y_FOLLOW;
    marioGetPtr()->dispFlags |= MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF;
}

/**
 * Asks for the zoomed-in camera to pull back to normal after 32 frames, over 2.2 seconds;
 * ignored if no zoom is active or a request is already pending. Used by Bowser's motions.
 */
void marioCamZoomOffReq(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & MARIO_DISP_FLAG_ZOOM && !(mario->dispFlags & MARIO_DISP_FLAG_ZOOM_OFF_REQ)) {
        mario->dispFlags |= MARIO_DISP_FLAG_ZOOM_OFF_REQ;
        mario->zoomOffTime = 220;
        mario->zoomOffDelay = 32;
    }
}

/**
 * Like marioCamZoomOffReq, but the pull-back time is given in ms (resolution 10 ms)
 * and a pending request is overridden; used when the boat (900) and Flurrie (600) end.
 */
void marioCamZoomOffReq2(s32 value) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & MARIO_DISP_FLAG_ZOOM) {
        mario->dispFlags |= MARIO_DISP_FLAG_ZOOM_OFF_REQ;
        mario->zoomOffTime = value / 10;
        mario->zoomOffDelay = 32;
    }
}

/**
 * Per-frame: once a zoom-off request's 32-frame delay has run out,
 * starts the camera move back to the normal view (evt_cam3d_evt_off) and clears the zoom.
 * Returns TRUE on the frame the move starts.
 */
BOOL marioCamZoomOff(void) {
    MarioWork* mario = marioGetPtr();
    EventEntry event;
    s32 args[10];
    s32 durationMs;

    if (!(mario->dispFlags & MARIO_DISP_FLAG_ZOOM)) {
        mario->dispFlags &= ~MARIO_DISP_FLAG_ZOOM_OFF_REQ;
        return FALSE;
    }

    if (!(mario->dispFlags & MARIO_DISP_FLAG_ZOOM_OFF_REQ)) {
        return FALSE;
    }

    if (--mario->zoomOffDelay > 0) {
        return FALSE;
    }

    mario->zoomOffDelay = 0;
    mario->zoomLevel = 0;
    mario->dispFlags &= ~(MARIO_DISP_FLAG_ZOOM | MARIO_DISP_FLAG_ZOOM_OFF_REQ);
    durationMs = mario->zoomOffTime * 10;
    mario->zoomOffTimer = ((durationMs * 60) / 1000);
    args[0] = durationMs;
    args[1] = EASING_SINE_IN_OUT;
    event.args = args;
    evt_cam3d_evt_off(&event, TRUE);
    return TRUE;
}

/**
 * Zooms the 3D camera in towards Mario (450 units, 750 for Bowser,
 * 550 off to his side while Flurrie carries him) over 1.5 s; run when Mario becomes the boat.
 * Returns FALSE when it cannot zoom (background mode, camera busy).
 */
BOOL marioCamZoomUp(void) {
    MarioWork* mario;
    PartyEntry* party;
    EventEntry event;
    s32 args[10];
    Vec cloudPos;
    Vec targetPos;
    Vec cameraDir;
    Vec cloudTarget;
    f32 xOffset;
    f32 yOffset;
    f32 angleAdjust;
    f32 zoomDistance;
    s32 tempX;
    CameraEntry* camera;

    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());

    if (marioBgmodeChk() == 1) {
        return FALSE;
    }

    if (!party || party->currentMemberId != PARTY_MEMBER_FLURRIE || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        if (camGetPtr(CAMERA_3D)->stateA) {
            return FALSE;
        }

        if (mario->dispFlags & MARIO_DISP_FLAG_ZOOM) {
            return TRUE;
        }
    } else {
        camGetPtr(CAMERA_3D);
        if (camGetPtr(CAMERA_3D)->stateA && mario->dispFlags & MARIO_DISP_FLAG_ZOOM) {
            return TRUE;
        }
    }

    mario->dispFlags |= MARIO_DISP_FLAG_ZOOM;
    mario->zoomOffTimer = 0;

    zoomDistance = (mario->characterId == CHARACTER_BOWSER) ? 750.0f : 450.0f;

    xOffset = (f32)(mario->screenX - 303) / 5.0f;
    yOffset = 0.0f;

    if ((f32)mario->screenY > 320.0f) {
        yOffset = (320.0f - (f32)mario->screenY) / 3.5f;
    }

    if (party && party->currentMemberId == PARTY_MEMBER_FLURRIE && party->flags & PARTY_FLAG_IS_BEING_USED) {
        angleAdjust = toMovedir(mario->displayDirection) >= 180.0f ? -70.0f : 70.0f;
        tempX = mario->screenX;
        zoomDistance = 550.0f;
        xOffset = ((f32)(tempX - 303) / 5.0f) + angleAdjust;
    }

    camera = camGetPtr(CAMERA_3D);

    cloudTarget = (Vec){ 0.0f, 0.0f, 0.0f };
    cloudTarget.x = camera->cameraAt.x + xOffset;
    cloudTarget.y = camera->cameraAt.y + yOffset;
    cloudTarget.z = camera->cameraAt.z;
    cloudPos = cloudTarget;

    cloudGetAt(&cloudPos);
    targetPos = cloudPos;

    PSVECSubtract(&camera->cameraPos, &camera->cameraAt, &cameraDir);
    PSVECNormalize(&cameraDir, &cameraDir);
    PSVECScale(&cameraDir, &cameraDir, zoomDistance);
    PSVECAdd(&targetPos, &cameraDir, &targetPos);

    args[0] = (s16)(s32)targetPos.x;
    args[1] = (s16)(s32)targetPos.y;
    args[2] = (s16)(s32)targetPos.z;
    args[3] = (s16)(s32)cloudPos.x;
    args[4] = (s16)(s32)cloudPos.y;
    args[5] = (s16)(s32)cloudPos.z;
    args[6] = 1500;
    args[7] = EASING_QUARTIC_IN;

    event.args = args;
    evt_cam3d_evt_set(&event, TRUE);
    return TRUE;
}

BOOL marioCamZoomUpLevelMain(void) {
    MarioWork* mario;
    EventEntry event;
    s32 args[10];
    Vec cloudPos;
    Vec targetPos;
    Vec cameraDir;
    Vec cloudTarget;
    f32 xOffset;
    f32 yOffset;
    f32 zoomDistance;
    CameraEntry* camera;

    mario = marioGetPtr();
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_ZOOM_UP_LEVEL_UNUSED)) {
        return FALSE;
    }
    mario->trigFlags &= ~MARIO_TRIG_FLAG_ZOOM_UP_LEVEL_UNUSED;

    if (marioBgmodeChk() == 1) {
        return FALSE;
    }

    mario->dispFlags |= MARIO_DISP_FLAG_ZOOM;
    mario->zoomOffTimer = 0;

    if (mario->zoomLevel == 1) {
        zoomDistance = 520.0f;
    } else if (mario->zoomLevel == 2) {
        zoomDistance = 475.0f;
    } else if (mario->zoomLevel == 3) {
        zoomDistance = 440.0f;
    } else {
        zoomDistance = 450.0f;
    }

    xOffset = (f32)(mario->screenX - 303) / 5.0f;
    yOffset = 0.0f;

    if ((f32)mario->screenY > 320.0f) {
        yOffset = (320.0f - (f32)mario->screenY) / 3.5f;
    }

    camera = camGetPtr(CAMERA_3D);

    cloudTarget = (Vec){ 0.0f, 0.0f, 0.0f };
    cloudTarget.x = camera->cameraAt.x + xOffset;
    cloudTarget.y = camera->cameraAt.y + yOffset;
    cloudTarget.z = camera->cameraAt.z;
    cloudPos = cloudTarget;
    targetPos = cloudPos;

    PSVECSubtract(&camera->cameraPos, &camera->cameraAt, &cameraDir);
    PSVECNormalize(&cameraDir, &cameraDir);
    PSVECScale(&cameraDir, &cameraDir, zoomDistance);
    PSVECAdd(&targetPos, &cameraDir, &targetPos);

    args[0] = (s16)(s32)targetPos.x;
    args[1] = (s16)(s32)targetPos.y;
    args[2] = (s16)(s32)targetPos.z;
    args[3] = (s16)(s32)cloudPos.x;
    args[4] = (s16)(s32)cloudPos.y;
    args[5] = (s16)(s32)cloudPos.z;
    args[6] = 1500;
    args[7] = EASING_QUARTIC_IN;

    event.args = args;
    evt_cam3d_evt_set(&event, TRUE);
    return TRUE;
}
