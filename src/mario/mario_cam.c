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

void marioResetCamShift(void) {
    MarioWork* mario = marioGetPtr();

    mario->camShiftTargetX = 0.0f;
    mario->camShiftTargetY = 0.0f;
    mario->camShiftTargetZ = 0.0f;
    mario->camZoomWeight = 0.0f;
    mario->camZoom = 2.0f;
    mario->camZoomOfs = (Vec){ 0.0f, 0.0f, 0.0f };
}

void marioResetCamShiftRate(void) {
    MarioWork* mario = marioGetPtr();

    mario->camShiftRate.x = 0.03;
    mario->camShiftRate.y = 0.06;
    mario->camShiftRate.z = 0.06;
    mario->camZoomWeight = 0.0f;
    mario->camZoom = 2.0f;
    mario->camZoomOfs = (Vec){ 0.0f, 0.0f, 0.0f };
}

void marioInitCamId(void) {
    marioGetPtr()->camId = CAMERA_3D;
}

s32 marioGetCamId(void) {
    return marioGetPtr()->camId;
}

void marioSetCamId(CameraId camId) {
    marioGetPtr()->camId = camId;
}

void marioResetCamFollowRate(void) {
    marioGetPtr()->camFollowRate = 0.2;
}

void marioSetCamFollowRate(f32 followRate) {
    marioGetPtr()->camFollowRate = followRate;
}

f32 marioGetCamFollowRate(void) {
    return marioGetPtr()->camFollowRate;
}

void marioPreCamera(void) {
    f32 roundingFactor;
    if (camGetPtr(CAMERA_3D)->viewYaw >= 0.0f) {
        roundingFactor = 0.5f;
    } else {
        roundingFactor = -0.5f;
    }
    marioGetPtr()->targetYaw = revise360((s32)((1000.0f * camGetPtr(CAMERA_3D)->viewYaw) + roundingFactor) / 1000.0f);
}

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
            mario2->dispFlags |= 0x20;
        } else {
            mario2->dispFlags &= ~0x20;
        }
    }
    mario2->prevCamPosX = camera->cameraPos.x;
    mario->dispFlags &= ~0x40000;
}

void marioUpdateCamCollision(Vec collision) {
    MarioWork* mario = marioGetPtr();
    mario->camCollision = collision;
    mario->dispFlags |= 0x40000;
}

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

void marioUpdateCam(void) {
    MarioWork* mario = marioGetPtr();
    f32 sin;
    f32 cos;

    UPDATE_CAM_SHIFT(mario->camShiftX, mario->camShiftTargetX, mario->camShiftRate.x);
    UPDATE_CAM_SHIFT(mario->camShiftY, mario->camShiftTargetY, mario->camShiftRate.y);
    UPDATE_CAM_SHIFT(mario->camShiftZ, mario->camShiftTargetZ, mario->camShiftRate.z);

    if (!(mario->dispFlags & 0x10000)) {
        camGetPtr(CAMERA_3D);
        sincosf(&sin, &cos, camGetPtr(CAMERA_3D)->viewYaw - 90.0f);
        if (!(mario->dispFlags & 0x20000)) {
            mario->camTarget.x = mario->position.x + sin * mario->camShiftX;
            mario->camTarget.z = mario->position.z + cos * mario->camShiftX;
            mario->camTarget.y = mario->camShiftY + (mario->position.y + mario->dispOfs.y);
            if (mario->currMotionId == MARIO_MOTION_SHIP) {
                mario->camTarget.y += 10.0f;
            }
            mario->camZoomWeight = mario->camShiftZ;
        } else {
            mario->camTarget.x = mario->position.x + sin * mario->camShiftX + sin * mario->camShiftZ;
            mario->camTarget.z = mario->position.z + cos * mario->camShiftX + cos * mario->camShiftZ;
            mario->camZoomWeight = mario->camShiftZ;
        }
    }
}

void marioUpdateCamPos(void) {
    marioGetPtr()->dispFlags &= ~0x10000;
}

void marioNoUpdateCamPos(void) {
    marioGetPtr()->dispFlags |= 0x10000;
}

void camFollowYOn(void) {
    camGetPtr(CAMERA_3D)->flags &= ~0x10;
    marioGetPtr()->dispFlags &= ~0x20000;
}

void camFollowYOff(void) {
    camGetPtr(CAMERA_3D)->flags |= 0x10;
    marioGetPtr()->dispFlags |= 0x20000;
}

void marioCamZoomOffReq(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & 0x100000 && !(mario->dispFlags & 0x200000)) {
        mario->dispFlags |= 0x200000;
        mario->zoomOffTime = 220;
        mario->zoomOffDelay = 32;
    }
}

void marioCamZoomOffReq2(s32 value) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & 0x100000) {
        mario->dispFlags |= 0x200000;
        mario->zoomOffTime = value / 10;
        mario->zoomOffDelay = 32;
    }
}

BOOL marioCamZoomOff(void) {
    MarioWork* mario = marioGetPtr();
    EventEntry event;
    s32 args[10];
    s32 durationMs;

    if (!(mario->dispFlags & 0x100000)) {
        mario->dispFlags &= ~0x200000;
        return FALSE;
    }

    if (!(mario->dispFlags & 0x200000)) {
        return FALSE;
    }

    if (--mario->zoomOffDelay > 0) {
        return FALSE;
    }

    mario->zoomOffDelay = 0;
    mario->zoomLevel = 0;
    mario->dispFlags &= ~0x300000;
    durationMs = mario->zoomOffTime * 10;
    mario->zoomOffTimer = ((durationMs * 60) / 1000);
    args[0] = durationMs;
    args[1] = 11;
    event.args = args;
    evt_cam3d_evt_off(&event, TRUE);
    return TRUE;
}

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

    if (!party || party->currentMemberId != 5 || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        if (camGetPtr(CAMERA_3D)->stateA) {
            return FALSE;
        }

        if (mario->dispFlags & 0x100000) {
            return TRUE;
        }
    } else {
        camGetPtr(CAMERA_3D);
        if (camGetPtr(CAMERA_3D)->stateA && mario->dispFlags & 0x100000) {
            return TRUE;
        }
    }

    mario->dispFlags |= 0x100000;
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
    args[7] = 3;

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
    if (!(mario->trigFlags & 0x10)) {
        return FALSE;
    }
    mario->trigFlags &= ~0x10;

    if (marioBgmodeChk() == 1) {
        return FALSE;
    }

    mario->dispFlags |= 0x100000;
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
    args[7] = 3;

    event.args = args;
    evt_cam3d_evt_set(&event, TRUE);
    return TRUE;
}
