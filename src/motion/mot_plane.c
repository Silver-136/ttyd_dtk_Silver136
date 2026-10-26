#include "dolphin/types.h"
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



BOOL chkCollision(f32* outY);
static void flyMain(void);


f32 planeData[41] = {
    5.0f, -0.02f, -0.03f, 25.0f, -45.0f, 0.4f, 1.0f, 0.1f, -0.1f, 1.5f,
    0.2f, 0.05f, 3.4f, 1.0f, 0.025f, 0.045f, 0.8f, 1.5f, -0.3f, -5.0f,
    3.9f, -0.112f, 0.118f, 0.5f, 0.4f, -8.0f, -2.0f, -4.0f, -2.0f, 0.2f,
    0.0f, 1.2f, 1.0f, 15.0f, -30.0f, 12.0f, 0.0f, 30.0f, -30.0f, 6.0f,
    28.0f,
};

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

static inline void planeSetSpd(f32 a, f32 b, f32 c) {
    MarioWork* m = marioGetPtr();
    m->camShiftTargetZ = a;
    m->camShiftRate.z = b;
    m->camZoom = c;
    m->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

const char* paper_plane[4] = {"p_hikohki", "p_hikohki_g", "p_hikohki_y", "p_hikohki_v"};

static inline void planeSfxOff(void) {
    MarioWork* m = marioGetPtr();
    if (m->motStruct != NULL && (u32)m->motPlane->sfxId != 0xFFFFFFFF) {
        psndSFXOff(m->motPlane->sfxId);
        m->motPlane->sfxId = -1;
    }
}

static inline void planeReset(void) {
    MarioWork* m = marioGetPtr();
    m->wPlayerCollisionBox.x = 20.0f;
    m->wPlayerCollisionBox.y = 37.0f;
    m->rot = (Vec){0.0f, 0.0f, 0.0f};
    m->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    m->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    marioPaperOff();
    marioChgPaper(NULL);
    m->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    m->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioAdjustMoveDir();
    m->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
}

static inline BOOL planeCancel(void) {
    MarioWork* m = marioGetPtr();
    if ((u16)m->currMotionId != MARIO_MOTION_PLANE) {
        return FALSE;
    }
    if ((s32)m->currSubMotionId >= 12 && (s32)m->currSubMotionId < 20) {
        m->currSubMotionId = 50;
        m->mot.plane.groundY = m->position.y;
        return TRUE;
    }
    return FALSE;
}

s32 marioPlaneGetFlyingDir(void) {
    MarioWork* mp = marioGetPtr();
    s32 ret;
    if ((u16)mp->currMotionId != MARIO_MOTION_PLANE) {
        return 0;
    }
    ret = -1;
    if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
        ret = 1;
    }
    return ret;
}

s32 marioGetPlaneStatus(void) {
    MarioWork* mp = marioGetPtr();
    if ((u16)mp->currMotionId != MARIO_MOTION_PLANE) {
        return 0;
    }
    if ((s32)mp->currSubMotionId >= 12 && (s32)mp->currSubMotionId < 20) {
        return 2;
    }
    return 1;
}

BOOL mario_plane_cancel(void) {
    MarioWork* mp = marioGetPtr();
    if ((u16)mp->currMotionId != MARIO_MOTION_PLANE) {
        return FALSE;
    }
    if ((s32)mp->currSubMotionId >= 12 && (s32)mp->currSubMotionId < 20) {
        mp->currSubMotionId = 50;
        mp->mot.plane.groundY = mp->position.y;
        return TRUE;
    }
    return FALSE;
}

void marioForcePlaneAnime(void) {
    MarioWork* mp = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn(paper_plane[marioGetColor()]);
    marioChgPaper("PM_P_1B");
    marioChgPose("M_Z_1");
    mp->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
}

#define DIRF(mp) ((mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? 1.0f : -1.0f)

static inline f32 planeDir(MarioWork* mp) {
    return DIRF(mp);
}

static inline void
planeRoll1(MarioWork* mp, f32 rate, f32 max) {
    f32* p = &mp->mot.plane.turnSpeed;
    f32 m = max * DIRF(mp);
    *p += rate * (m - *p);
}

static inline void planeRoll2(MarioWork* mp, f32 rate, f32 max) {
    f32* p = &mp->mot.plane.turnSpeed;
    *p += rate * (max * DIRF(mp) - *p);
}

#ifdef VERSION_G8ME01
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

void mot_plane(void) {
    MarioWork* mp;
    BOOL done;
    EventEntry* evt;
    f32 y, b, c;

    mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->motStruct = __memAlloc(HEAP_DEFAULT, 0x48);
        memset(mp->motStruct, 0, 0x48);
        { MarioWork* m2 = marioGetPtr(); m2->motPlane->sfxId = -1; }
        mp->currSubMotionId = 0;
    }
    switch (mp->currSubMotionId) {
    case 0:
        planeSetSpd(0.4f, 0.01f, 2.0f);
        marioPreJump();
        marioChgPose("M_J_1B");
        mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mp->currSubMotionId = 1;
        mp->airTimer = 0;
        mp->baseSpeed = 0.0f;
        mp->mot.plane.turnSpeed = 0.0f;
        mp->mot.plane.startY = mp->position.y;
        mp->mot.plane.groundY = 0.0f;
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
            if ((s32)mp->multiTimer == 40) {
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
        mp->baseSpeed = __fabsf(0.02f * mp->wStickDir1);
        mp->currSubMotionId = 12;
        mp->baseSpeed = 1.0f;
        mp->motPlane->diveSpeed = 2.0f;
        {
            MarioWork* m = marioGetPtr();
            m->camShiftTargetZ = 0.0f;
            m->camShiftRate.z = 0.004f;
        }
        mp->camShiftTargetX = 70.0f * ((mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) ? -1.0f : 1.0f);
        mp->camShiftTargetY = -70.0f;
        break;
    }
#ifdef VERSION_G8ME01
    // US: per-map floor below which the camera stops following the plane
    if (mp->position.y < planeCamFloor()) {
        mp->camShiftTargetY = __fabsf(planeData[38]);
        camFollowYOff();
    }
#else
    if (mp->position.y < 100.0f) {
        mp->camShiftTargetY = __fabsf(planeData[38]);
    }
#endif
    flyMain();
    if (marioKeyOffChk() && !(mp->trigFlags & MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON)) {
        if (planeCancel()) {
            return;
        }
    }
    if ((s32)mp->currSubMotionId == 20 || (s32)mp->currSubMotionId == 30) {
        camFollowYOff();
        mp->camShiftTargetY = 0.0f;
        mp->camShiftTargetX = 0.0f;
        marioClearJumpPara();
        mp->baseSpeed = 0.0f;
        planeSfxOff();
        planeReset();
        allPartyRideOff2();
        if ((s32)mp->currSubMotionId == 20) {
            marioChgMot(MARIO_MOTION_JUMP);
        } else if ((s32)mp->currSubMotionId == 30) {
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
        if ((s32)mp->multiTimer > 0) {
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
        {
            HitEntry* hit;
            mp->hitobjStandOn = NULL;
            hit = marioSearchGround(&y, &b, &c, mp->jumpVelocityY, mp->jumpVelocityY);
            if (hit != NULL) {
                mp->position.y = y;
                mp->hitobjStandOn = hit;
            } else {
                mp->position.y += mp->jumpVelocityY;
            }
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
#ifdef VERSION_G8ME01
        marioChkWallAround(&mp->position, 0, 0.0f, 0.0f, mp->wPlayerCollisionBox.z, mp->wPlayerCollisionBox.y);
#endif
        break;
    case 70: {
        Vec p;
        set_damage_root_ypos(mp->position.y);
        p = mp->position;
        marioSetSplash(0, &p);
        mp->multiTimer = 16;
        mp->currSubMotionId = 71;
        mp->jumpVelocityY = -2.0f;
        mp->jumpAccelerationY = 0.0f;
        mp->jumpJerkY = 0.0f;
        mp->jumpSnapY = 0.0f;
        camFollowYOff();
    }
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


double sin(double);

#define PI 3.1415927f
#define SPD (mp->baseSpeed)
#define STICK (mp->wStickDir1)

static inline BOOL planeChkUnder(void) {
    BOOL ret = FALSE;
    MarioWork* m = marioGetPtr();
    if (m->hitobjUnder != NULL && (hitGetAttr(m->hitobjUnder) & 0x38)) {
        ret = TRUE;
    }
    return ret;
}

static void flyMain(void) {
    f32 s2, c2, s, c;
    MarioWork* mp;
    f32 t;
    f32 a;
    f32 v;
    BOOL flag;
    s32 n;

    mp = marioGetPtr();
    switch (mp->currSubMotionId) {
    case 12:
        if (mp->mot.plane.startY >= mp->position.y) {
            mp->currSubMotionId = 14;
        }
        {
            MarioWork* m = marioGetPtr();
            if ((u32)m->motPlane->sfxId == 0xFFFFFFFF) {
                m->motPlane->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_AIRPLANE2), &m->position);
            }
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
    if ((s32)mp->currSubMotionId != 15) {
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
    flag = FALSE;
    t = mp->motPlane->yaw;
    if (t >= 90.0f && t <= 270.0f) {
        flag = TRUE;
    }
    mp->displayDirection = revise360(flag ? -t - 90.0f : 270.0f - t);
    if (STICK != 0) {
        if (SPD) {
            a = __fabsf((f32)STICK);
            if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
                if (STICK < 0) {
                    if (mp->motPlane->pitch >= planeData[3]) {
                        mp->motPlane->pitch += 0.1f * (planeData[3] - mp->motPlane->pitch);
                    } else {
                        mp->motPlane->pitch += 0.1f * (a * planeData[5]);
                    }
                }
                if (STICK > 0) {
                    if (mp->motPlane->pitch <= planeData[4]) {
                        mp->motPlane->pitch = planeData[4];
                    } else {
                        mp->motPlane->pitch -= 0.1f * (a * planeData[6]);
                    }
                }
            } else {
                if (STICK > 0) {
                    if (mp->motPlane->pitch >= planeData[3]) {
                        mp->motPlane->pitch += 0.1f * (planeData[3] - mp->motPlane->pitch);
                    } else {
                        mp->motPlane->pitch += 0.1f * (a * planeData[5]);
                    }
                }
                if (STICK < 0) {
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
            t = planeData[0] - SPD;
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
                SPD += 0.5f;
                if (SPD > 2.0f) {
                    SPD = 2.0f;
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
    SPD += SPD * planeData[1];
    if (mp->motPlane->pitch >= 0.0f) {
        mp->motPlane->flags |= MOT_PLANE_FLAG_DIVE_UNUSED;
        a = (f32)sin(PI * (mp->motPlane->pitch * planeData[7]) / 180.0f);
        a = SPD * (planeData[8] * a);
        SPD = SPD + a;
        if (STICK != 0 && mp->motPlane->pitch >= planeData[3]) {
            SPD += planeData[2];
        }
        if (SPD <= 0.0f) {
            SPD = 0.0f;
        }
        t = 0.0f;
        if (STICK == 0) {
            if (SPD < planeData[20]) {
                t = planeData[0] - SPD;
                t *= planeData[21] * (f32)sin(PI * mp->motPlane->pitch / 180.0f);
                t += mp->motPlane->pitchSpeed;
                mp->motPlane->pitchSpeed = t;
            } else {
                mp->motPlane->pitchSpeed = planeData[22];
            }
            mp->motPlane->pitch += mp->motPlane->pitchSpeed;
            if (t <= 0.0f && SPD <= planeData[0] * planeData[23]) {
                t = planeData[0] - SPD * planeData[24];
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
        if (STICK != 0) {
            v = planeData[18] * (f32)sin(PI * (planeData[0] - SPD) / 180.0f);
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
        SPD += mp->motPlane->diveSpeed;
        a = (mp->motPlane->pitch > 0.0f) ? 0.0f : __fabsf(mp->motPlane->pitch);
        t = (f32)sin(PI * (planeData[12] * a) / 180.0f);
        { f32 s = (f32)sin(PI * (SPD * planeData[11]) / 180.0f); a = planeData[13] * (s * t); }
        mp->motPlane->pitchSpeed = mp->motPlane->pitchSpeed + a;
        if (STICK != 0) {
            if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
                if (STICK > 0 && mp->motPlane->pitchSpeed >= 1.0f) {
                    mp->motPlane->pitchSpeed = 1.0f;
                }
            } else {
                if (STICK < 0 && mp->motPlane->pitchSpeed >= 1.0f) {
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
    if (SPD >= planeData[0]) {
        SPD = planeData[0];
    }
    mp->position.y += mp->motPlane->sinkSpeed;
    mp->position.y -= planeData[29];
    mp->rot.z = revise360(-mp->motPlane->pitch);
    mp->directionView = mp->motPlane->yaw;
    sincosf(&s, &c, mp->directionView);
    mp->position.z += SPD * c;
    sincosf(&s2, &c2, 90.0f + mp->motPlane->pitch);
    mp->position.y += (SPD * c2) * ((c2 >= 0.0f) ? planeData[31] : planeData[32]);
    mp->position.x += s2 * (SPD * s);
    if (SPD <= planeData[16]) {
        n = sysMsec2Frame(1000.0f * planeData[17]);
        if (mp->motPlane->flags & MOT_PLANE_FLAG_RIGHT) {
            if ((f32)STICK < 0.0f) {
                if (++mp->motPlane->stallTimer >= n) {
                    mp->mot.plane.groundY = mp->position.y;
                    mp->currSubMotionId = 60;
                    return;
                }
            }
        } else {
            if ((f32)STICK > 0.0f) {
                if (++mp->motPlane->stallTimer >= n) {
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

void mot_plane_post(void) {
    MarioWork* mp = marioGetPtr();
    camFollowYOn();
    planeSfxOff();
    planeReset();
    mp->camShiftTargetX = 0.0f;
    mp->camShiftTargetY = 0.0f;
    if (mp->motStruct != NULL) {
        __memFree(HEAP_DEFAULT, mp->motStruct);
        mp->motStruct = NULL;
    }
    allPartyRideOff2();
}

static f32 planeDead270(void) {
    return 270.0f;
}

BOOL chkCollision(f32* outY) {
    Vec dir, hpos, v, v2;
    f32 s1, c1, s0, c0, s2, c2;
    f32 ox, oy, oz, dist, nx, ny, nz;
    f32 x, y, z, r;
    HitEntry* hit;
    BOOL ret;
    MarioWork* mp;

    r = 4.0f;
    ret = FALSE;
    mp = marioGetPtr();
    sincosf(&s0, &c0, 90.0f + mp->motPlane->pitch);
    sincosf(&s1, &c1, mp->motPlane->yaw);
    {
        x = mp->rotOfs.x + (mp->position.x + mp->dispOfs.x);
        y = mp->rotOfs.y + (mp->position.y + mp->dispOfs.y);
        z = mp->rotOfs.z + (mp->position.z + mp->dispOfs.z);
        x += s1 * c0 * r;
        y += -s0 * r;
        z += c1 * c0 * r;
        v = (Vec){x, y, z};
    }
    mp->motPlane->tipPos = v;
    sincosf(&s2, &c2, mp->motPlane->roll);
    sincosf(&s0, &c0, mp->motPlane->pitch);
    mp->hitobjFront = NULL;
    r = planeData[40];
    sincosf(&s1, &c1, mp->motPlane->yaw);
    x = mp->rotOfs.x + (mp->position.x + mp->dispOfs.x);
    y = mp->rotOfs.y + (mp->position.y + mp->dispOfs.y);
    z = mp->rotOfs.z + (mp->position.z + mp->dispOfs.z);
    x += r * (s1 * -c0);
    z += r * (c1 * -c0);
    y += s0 * r;
    sincosf(&s0, &c0, mp->motPlane->pitch - 90.0f);
    x += planeData[39] * (c0 * s1) - planeData[39] * (s2 * c1);
    y += planeData[39] * (s0 * c2);
    z += planeData[39] * (c0 * c1) - planeData[39] * (s2 * s1);
    {
        v2 = (Vec){x, y, z};
    }
    mp->motPlane->frontPos = v2;
    x = mp->motPlane->frontPos.x - 10.0f * s1;
    y = mp->motPlane->frontPos.y;
    z = mp->motPlane->frontPos.z - 10.0f * c1;
    dist = 10.0f + mp->baseSpeed;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y,
                        z, s1, 0.0f, c1);
    if (hit != NULL) {
        mp->hitobjFront = hit;
        ret = TRUE;
        *outY = oy;
    }
    x = mp->motPlane->tipPos.x;
    y = mp->motPlane->tipPos.y;
    z = mp->motPlane->tipPos.z;
    dist = mp->baseSpeed;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, s1, 0.0f, c1);
    if (hit != NULL) {
        mp->hitobjFront = hit;
        ret = TRUE;
        *outY = oy;
    }
    r = 10.0f;
    x = mp->motPlane->frontPos.x;
    y = mp->motPlane->frontPos.y;
    z = mp->motPlane->frontPos.z;
    mp->hitobjStandOn = NULL;
    dist = 10.1f;
    hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y + r, z, 0.0f, -1.0f, 0.0f);
    if (hit != NULL) {
        mp->hitobjStandOn = hit;
        ret = TRUE;
        *outY = oy;
    }
    if (!ret) {
        x = mp->motPlane->tipPos.x;
        y = mp->motPlane->tipPos.y;
        z = mp->motPlane->tipPos.z;
        mp->hitobjStandOn = NULL;
        dist = 10.1f;
        hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y + r, z, 0.0f, -1.0f, 0.0f);
        if (hit != NULL) {
            mp->hitobjStandOn = hit;
            ret = TRUE;
            *outY = oy;
        }
    }
    if (!ret) {
        PSVECSubtract(&mp->motPlane->tipPos, &mp->motPlane->frontPos, &dir);
        dist = PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        hit = marioHitCheckVec(&mp->motPlane->frontPos, &dir, &hpos, NULL, &dist);
        if (hit != NULL) {
            mp->hitobjStandOn = hit;
            ret = TRUE;
            *outY = hpos.y;
        }
    }
    mp->hitobjHead = NULL;
    if (mp->hitobjStandOn == NULL) {
        x = mp->motPlane->frontPos.x;
        y = mp->motPlane->frontPos.y;
        z = mp->motPlane->frontPos.z;
        dist = 2.0f;
        hit = marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, x, y, z, 0.0f, 1.0f, 0.0f);
        if (hit != NULL) {
            mp->hitobjHead = hit;
            ret = TRUE;
            *outY = oy;
        }
    }
    return ret;
}

static f32 planeDead4(void) {
    return 4.0f;
}
