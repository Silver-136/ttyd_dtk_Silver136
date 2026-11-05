// Mario's hammer motions: the normal swing (mot_hammer) and the charged spin (mot_hammer2), with block and wall
// hits.

#include "version.h"
#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "driver/animdrv.h"
#include "eff/eff_kemu_test.h"
#include "eff/eff_ultra_hammer.h"
#include "system.h"
#include <string.h>
#include "driver/mobjdrv.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "evt/evt_cam.h"
#include "mot/mot_stay.h"
#include "mot/mot_hammer.h"

#if VERSION >= VERSION_JP
double cos(float);
#endif

static HitEntry* hitEstimate(s32* out);

// hitEstimate's *out: what the swing will hit; it picks the pose, the effect and the swing length.
enum {
    HAMMER_HIT_NONE = 0,
    HAMMER_HIT_HIGH = 4,     // a wall or object at high height
    HAMMER_HIT_MIDDLE = 5,
    HAMMER_HIT_LOW = 6,
    HAMMER_HIT_OVERHEAD = 7, // just above Mario's head height
};
static HitEntry* hammerHitWall(f32 ang, f32* s, f32* co, f32* len, f32* hx, f32* hy, f32* hz, f32* nx, f32* ny, f32* nz);



/**
 * Hammer hit sounds of the Attack FX badges: pairs of badge (R, B, G, Y, P) and the sound it swaps in;
 * with several equipped, each swing moves on to the next one worn.
 */
s32 se_data[] = {
    ITEM_ATTACK_FX_R, PSND_SFX_MARIO_HAMMER_PIKKYO_R1,
    ITEM_ATTACK_FX_B, PSND_SFX_MARIO_HAMMER_PIKKYO_B1,
    ITEM_ATTACK_FX_G, PSND_SFX_MARIO_HAMMER_PIKKYO_G1,
    ITEM_ATTACK_FX_Y, PSND_SFX_MARIO_HAMMER_PIKKYO_Y1,
    ITEM_ATTACK_FX_P, PSND_SFX_MARIO_HAMMER_PIKKYO_P1,
};

/** Light camera shake at the impact frame of every swing with the plain Hammer (level 1). */
EVT_BEGIN(lv1quake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 90)
    RETURN
EVT_END()

EVT_BEGIN(lv2quake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 110)
    RETURN
EVT_END()

EVT_BEGIN(lv3quake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.029296875f), 140)
    RETURN
EVT_END()

static inline BOOL chkRevDir(f32 angle) {
    BOOL ret = FALSE;
    if (angle >= 90.0f && angle <= 270.0f) {
        ret = TRUE;
    }
    return ret;
}

/**
 * The hammer swing direction: 180 (left) or 0 (right) from Mario's facing,
 * turned 30 degrees when Mario shows his back.
 */
// The hammer swing direction: straight left or right, turned 30 degrees when Mario faces away from the camera.
static inline f32 hamDir(MarioWork* mario) {
    f32 a = mario->displayDirection;
    MarioWork* p = marioGetPtr();
    f32 r;
    f32 off = 0.0f;
    if (p->dispFlags & MARIO_DISP_FLAG_REAR_POSE) {
        off = chkRevDir(p->displayDirection) ? 30.0f : -30.0f;
    }
    if (a >= 90.0f && a <= 270.0f) {
        r = revise360(180.0f + off);
    } else {
        r = revise360(off);
    }
    return r;
}


static const char isS1_str[14];
static const char isB1_str[17];
static const char isS2_str[14];
static const char isB2_str[17];
static const char isS3_str[14];
static const char isB3_str[17];

/**
 * 1 if the hammer block mobj obj can be broken with Mario's current hammer level,
 * so the swing goes through instead of bouncing off.
 */
static inline s32 chkBlock(MapObjectEntry* obj) {
    s32 ok = 0;
    if (strcmp(obj->animName, isS1_str) == 0 || strcmp(obj->animName, isB1_str) == 0) {
        if (pouchGetPtr()->hammerLevel >= 1) {
            ok = 1;
        }
    } else if (strcmp(obj->animName, isS2_str) == 0 || strcmp(obj->animName, isB2_str) == 0) {
        if (pouchGetPtr()->hammerLevel >= 2) {
            ok = 1;
        }
    } else if (strcmp(obj->animName, isS3_str) == 0 || strcmp(obj->animName, isB3_str) == 0) {
        if (pouchGetPtr()->hammerLevel >= 3) {
            ok = 1;
        }
    }
    return ok;
}

/**
 * Mario's hammer level (0 none, 1 Hammer, 2 Super Hammer, 3 Ultra Hammer);
 * the motion code uses 0 to block swinging and 2+ for the spin hammer.
 */
s32 marioGetHammerLevel(void) {
    return pouchGetPtr()->hammerLevel;
}


/**
 * Normal hammer swing (B button): picks the swing pose for the hammer level,
 * checks what the hammer will hit (hitEstimate) and plays a clang or a breakable-block swing,
 * shakes the camera per level on impact, spawns the hit effect (JP and later),
 * plays the Attack FX badge sound in place of the wooden impact sound when one is equipped,
 * then returns to standing after about 20 frames.
 */
void mot_hammer(void) {
    MarioWork* mario = marioGetPtr();
    s32 snd;
    s32 kind;
    f32 px, pz;
    s32 done;
    s32 i;
    f32 py, ez;
    s32 efType;
    HitEntry* hit;
    s32 n;
    MarioWork* m;
    s32 lv;

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->mot.hammer.dir = hamDir(mario);
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        mario->hitobjHammerNew = NULL;
        hit = hitEstimate(&kind);
        mario->hitobjHammerNew = hit;
        if (hit) {
            f32 a = mario->displayDirection;
            f32 off = marioGetPtr()->dispFlags & MARIO_DISP_FLAG_REAR_POSE ? -30.0f : 0.0f;
            f32 r;
            if (a >= 90.0f && a <= 270.0f) {
                r = toMovedir2(180.0f, off);
            } else {
                r = toMovedir2(0.0f, off);
            }
            mario->directionView = r;
            mario->mot.hammer.dir = hamDir(mario);
        }
        if (hitGetAttr(hit) & HIT_ATTR_MOBJ) {
            s32 ok = chkBlock(mobjHitObjPtrToPtr(hit));
            if (ok) {
                kind = HAMMER_HIT_NONE;
            }
        }
        switch (pouchGetHammerLv()) {
        case 1:
            marioChgPose(kind == HAMMER_HIT_OVERHEAD ? "M_H_2" : "M_H_1");
            break;
        case 2:
            marioChgPose(kind == HAMMER_HIT_OVERHEAD ? "M_H_5" : "M_H_4");
            break;
        case 3:
            marioChgPose(kind == HAMMER_HIT_OVERHEAD ? "M_H_8" : "M_H_7");
            break;
        }
        if (kind != HAMMER_HIT_NONE) {
            {
                // FAKE: spare copy of kind in its own block; using kind directly changes the object
                s32 k = kind;
                mario->mot.hammer.hitFrames = k == HAMMER_HIT_OVERHEAD ? 3 : k;
            }
            mario->flags |= MARIO_FLAG_HAMMER_HIT_WALL;
        } else {
            mario->flags &= ~MARIO_FLAG_HAMMER_HIT_WALL;
        }
        m = marioGetPtr();
        snd = PSND_SFX_MARIO_HAMMER_FURU1;
        switch (pouchGetHammerLv()) {
        case 1:
            snd = PSND_SFX_MARIO_HAMMER_FURU1;
            break;
        case 2:
            snd = PSND_SFX_MARIO_HAMMER_FURU2;
            break;
        case 3:
            snd = PSND_SFX_MARIO_HAMMER_FURU3;
            break;
        }
        psndSFXOn_3D(SFX_NAME_WORD(snd), &m->position);
#if VERSION >= VERSION_JP
        mario->mot.hammer.hitKind = kind;
#endif
    }
    {
        f32 a = mario->mot.hammer.dir;
        BOOL r = FALSE;
        if (a >= 90.0f && a <= 270.0f) {
            r = TRUE;
        }
        mario->displayDirection = r ? 180.0f : 0.0f;
    }
    mario->displayDirectionTarget = mario->displayDirection;
    mario->moveDirection = toMovedir(mario->mot.hammer.dir);
    done = 0;
    mario->directionView = mario->moveDirection;
    if (mario->flags & MARIO_FLAG_HAMMER_HIT_WALL) {
        s32 t = mario->multiTimer;
        s32 lim = mario->mot.hammer.hitFrames;
        n = t <= lim ? t : lim;
        if (t == lim) {
            done = 1;
        }
    } else {
        n = mario->multiTimer <= 10 ? mario->multiTimer : 10;
        if (mario->multiTimer == 10) {
            done = 1;
        }
    }
    mario->poseTime = n;
    animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], (f32)n);
    if (done) {
        lv = pouchGetPtr()->hammerLevel;
        if (lv == 1) {
            evtEntry(lv1quake_evt, 0, 0);
        } else if (lv == 2) {
            evtEntry(lv2quake_evt, 0, 0);
        } else if (lv == 3) {
            evtEntry(lv3quake_evt, 0, 0);
        }
        mario->currSubMotionId = 1;
        mario->hitobjHammer = mario->hitobjHammerNew;
#if VERSION >= VERSION_JP
        px = mario->position.x;
        py = mario->position.y;
        pz = mario->position.z;
        if (!(mario->dispFlags & MARIO_DISP_FLAG_REAR_POSE)) {
            efType = -1;
            switch (mario->mot.hammer.hitKind) {
                case HAMMER_HIT_OVERHEAD:
                    movePos(&px, &pz, 16.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                    ez = 53.0f;
                    efType = 0x16;
                    break;
                case HAMMER_HIT_NONE:
                case HAMMER_HIT_LOW:
                    movePos(&px, &pz, 25.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                    ez = -3.0f;
                    efType = 0x15;
                    break;
                case HAMMER_HIT_MIDDLE:
                    movePos(&px, &pz, 31.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                    ez = 5.0f;
                    efType = 0x15;
                    break;
                case HAMMER_HIT_HIGH:
                    movePos(&px, &pz, 30.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                    ez = 30.0f;
                    efType = 0x16;
                    break;
            }
        } else {
            efType = -1;
            switch (mario->mot.hammer.hitKind) {
                case HAMMER_HIT_OVERHEAD:
                    movePos(&px, &pz, 16.0f, toMovedir2(mario->displayDirectionTarget, -2.0f));
                    ez = 53.0f;
                    efType = 0x16;
                    break;
                case HAMMER_HIT_NONE:
                case HAMMER_HIT_LOW:
                    movePos(&px, &pz, 30.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                    ez = -3.0f;
                    efType = 0x15;
                    break;
                case HAMMER_HIT_MIDDLE:
                    movePos(&px, &pz, 36.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                    ez = 20.0f;
                    efType = 0x16;
                    break;
                case HAMMER_HIT_HIGH:
                    movePos(&px, &pz, 37.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                    ez = 28.0f;
                    efType = 0x16;
                    break;
            }
        }
        if (efType > 0) {
            effKemuTestEntry(efType, px, py + ez, pz, 1.0f);
        }
#endif
        m = marioGetPtr();
        snd = PSND_SFX_MARIO_HAMMER_WOOD_DON1;
        switch (pouchGetHammerLv()) {
        case 1:
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_DON1), &m->position);
            break;
        case 2:
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_DON2), &m->position);
            break;
        case 3:
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_DON3), &m->position);
            break;
        }
#if VERSION <= VERSION_DEMO_JP
        for (i = 0; i < 5; i++) {
#else
        // FAKE: unsigned compare of the s32 loop counter; i < 10 changes the object
        for (i = 0; (u32)i < 10; i++) {
#endif
            if (pouchEquipCheckBadge(se_data[m->hammerSfxIdx])) {
                snd = se_data[m->hammerSfxIdx + 1];
                m->hammerSfxIdx += 2;
                if (m->hammerSfxIdx >= 10) {
                    m->hammerSfxIdx -= 10;
                }
                break;
            } else {
                m->hammerSfxIdx += 2;
                if (m->hammerSfxIdx >= 10) {
                    m->hammerSfxIdx -= 10;
                }
            }
        }
        psndSFXOn_3D(SFX_NAME_WORD(snd), &m->position);
    }
    if (++mario->multiTimer > 20) {
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioChgMot(MARIO_MOTION_STAY);
    }
}


/**
 * Checks for the Super Hammer spin from the stay motion:
 * while B is held Mario glares (M_I_Y pose and voice),
 * and once the stick has been rotated (marioGetRub) returns 1 with mario->hammerDir set.
 * Returns 0 below the Super Hammer or when B is released.
 */
s32 marioChkHammer2(void) {
    MarioWork* mario = marioGetPtr();
    s32 rubCount, rubDir;
    f32 rubMag;
    if (pouchGetPtr()->hammerLevel < 2) {
        return 0;
    }
    if (!(mario->buttonHold & PAD_BUTTON_B)) {
        if (!(mario->buttonHold & PAD_BUTTON_A)) {
            if (strcmp(mario->animName, "M_I_Y") == 0 && mario->currMotionId == MARIO_MOTION_STAY) {
                marioChgPose("M_S_1");
                marioVoiceGlareOff();
            }
            mario->wMotionTimer = 0;
        }
        return 0;
    }
#if VERSION >= VERSION_US
    if (mario->flags & MARIO_FLAG_BG_MODE) {
        mario->wMotionTimer = 0;
    } else {
        mario->wMotionTimer++;
    }
#else
    mario->wMotionTimer++;
#endif
    if (mario->controlStickSensitivity && mario->wMotionTimer > 10 && mario->currMotionId == MARIO_MOTION_STAY && !(mario->dispFlags & MARIO_DISP_FLAG_PAPER)) {
        marioChgPose("M_I_Y");
        marioVoiceGlareOn();
    }
    if (mario->wMotionTimer >= 50) {
        mario->wMotionTimer = 50;
        if (mario->currMotionId == MARIO_MOTION_STAY && !(mario->dispFlags & MARIO_DISP_FLAG_PAPER)) {
            marioChgPose("M_I_Y");
            marioVoiceGlareOn();
        }
    }
    if (marioGetRub(PAD_BUTTON_B, &rubDir, &rubCount, &rubMag) != 1) {
        return 0;
    }
    mario->baseSpeed = 0.0f;
    if (rubCount < 1) {
        return 0;
    }
    mario->hammerDir = rubDir;
    return 1;
}




/** Moves the camera in for a spin hammer charge level: z shift and zoom, with a 40-unit upward zoom offset. */
static inline void mot_ham(f32 shiftZ, f32 zoom) {
    MarioWork* mario = marioGetPtr();
    mario->camShiftTargetZ = shiftZ;
    mario->camShiftRate.z = 0.005f;
    mario->camZoom = zoom;
    mario->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

static inline void h2LevelUp(MarioWork* mario, f32 shiftZ, f32 zoom) {
    mot_ham(shiftZ, zoom);
    mario->mot.hammer2.flags |= HAMMER2_FLAG_LEVEL_UP;
}

static inline void h2Ramp(MarioWork* mario, f32 maxv, f32 midv, f32 minv, s32 lvl, f32 shiftZ, f32 zoom) {
    marioChgPaper("P_H_1A");
    if (!(mario->mot.hammer2.flags & HAMMER2_FLAG_AT_LEVEL)) {
        mario->mot.hammer2.animTime += 6.0f;
        if (mario->mot.hammer2.animTime >= maxv) {
            mario->mot.hammer2.animTime = maxv;
            mario->mot.hammer2.flags |= HAMMER2_FLAG_AT_LEVEL;
            mario->hammerChargeLv = lvl;
            h2LevelUp(mario, shiftZ, zoom);
        }
    } else if (!(mario->mot.hammer2.flags & HAMMER2_FLAG_SWAY_BACK)) {
        mario->mot.hammer2.animTime += 1.0f;
        if (mario->mot.hammer2.animTime >= midv) {
            mario->mot.hammer2.animTime = midv;
            mario->mot.hammer2.flags |= HAMMER2_FLAG_SWAY_BACK;
        }
    } else {
        mario->mot.hammer2.animTime -= 1.0f;
        if (mario->mot.hammer2.animTime <= minv) {
            mario->mot.hammer2.animTime = minv;
            mario->mot.hammer2.flags &= ~HAMMER2_FLAG_SWAY_BACK;
        }
    }
}

static inline void h2LightOn(void) {
#if VERSION < VERSION_US
    marioPaperLightOn();
#endif
}

static inline void h2ClrPaper(MarioWork* mario) {
    marioPaperOff();
    h2LightOn();
    marioChgPose("M_S_1");
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->dispFlags &= ~MARIO_DISP_FLAG_FLIP;
}

/**
 * Spin hammer motion (Super and Ultra Hammer): Mario winds up while B is held,
 * raising the charge level (hammerChargeLv, camera zooms in each level),
 * then spins around hitting everything nearby; level 3 spins fastest and turns the most. Ends back in the stay motion.
 */
void mot_hammer2(void) {
    MarioWork* mario = marioGetPtr();
    s32 rubCount, rubDir;
    f32 rubMag;
    HitEntry* hit;
    f32 ang2, ang;
    f32 nz, ny, nx, len, hz, hy, hx;
    f32 co, s;
    MarioWork* m;

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->mot.hammer2.flags = 0;
        mario->currSubMotionId = 0;
        mario->mot.hammer2.sfxId = -1;
#if VERSION >= VERSION_US
        mario->savedDisplayDirection = mario->displayDirection;
#endif
    }
    switch (mario->currSubMotionId) {
    case 0:
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
        mario->hammerChargeTimer = 0;
        mario->hammerChargeLv = 0;
        mario->baseSpeed = 0.0f;
        mario->airTimer = 0;
        mario->currSubMotionId = 1;
        mario->multiTimer = 30;
        if (mario->hammerChargeLv == 3) {
            mario->mot.hammer2.spinSpeed = 38.0f;
        } else if (mario->hammerChargeLv <= 2) {
            mario->mot.hammer2.spinSpeed = 15.0f;
        } else {
            mario->mot.hammer2.spinSpeed = 10.0f;
        }
        switch (pouchGetHammerLv()) {
        case 1:
            marioChgPose("M_H_3");
            break;
        case 2:
            marioChgPose("M_H_6");
            break;
        case 3:
            marioChgPose("M_H_9");
            break;
        }
        mario->hitobjHammerNew = NULL;
        mario->currSubMotionId = 1;
        mario->multiTimer = 1;
        break;
    case 1:
        if (--mario->multiTimer > 0) {
            break;
        }
        if (180.0f == mario->displayDirectionTarget && mario->hammerDir == 1) {
            mario->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
        }
        if (0.0f == mario->displayDirectionTarget && mario->hammerDir == 0) {
            mario->dispFlags |= MARIO_DISP_FLAG_FLIP;
        }
        marioPaperOn("p_kaiten_h");
        marioChgPaper("P_H_1C");
        marioPaperLightOff();
        mario->mot.hammer2.sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN1), &mario->position);
        mario->mot.hammer2.animTime = 0.0f;
        mario->poseTime = 0;
        animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], mario->mot.hammer2.animTime);
        mario->currSubMotionId = 10;
        mario->multiTimer = 30;
    case 10:
        if (marioGetRub(PAD_BUTTON_B, &rubDir, &rubCount, &rubMag) == 1) {
            mario->mot.hammer2.flags &= ~HAMMER2_FLAG_AT_LEVEL;
            mario->hammerChargeTimer++;
            if (mario->hammerChargeTimer >= 20) {
                mario->hammerChargeTimer = 20;
            }
        }
        if (mario->hammerChargeTimer >= 4) {
            mario->mot.hammer2.animTime += 6.0f;
            if (mario->mot.hammer2.animTime >= 72.0f) {
                mario->mot.hammer2.animTime = 72.0f;
            }
            mario->currSubMotionId = 11;
            marioChgPaper("P_H_1B");
            mario->hammerChargeLv = 3;
            h2LevelUp(mario, 0.4f, 1.7f);
        } else if (mario->hammerChargeTimer >= 3) {
            h2Ramp(mario, 54.0f, 54.0f, 42.0f, 3, 0.4f, 1.7f);
        } else if (mario->hammerChargeTimer >= 2) {
            h2Ramp(mario, 36.0f, 36.0f, 30.0f, 2, 0.3f, 1.6f);
        } else {
            h2Ramp(mario, 18.0f, 24.0f, 18.0f, 1, 0.2f, 1.5f);
        }
        if (mario->mot.hammer2.flags & HAMMER2_FLAG_LEVEL_UP) {
            mario->mot.hammer2.flags &= ~HAMMER2_FLAG_LEVEL_UP;
            if (!(mario->mot.hammer2.flags & HAMMER2_FLAG_CHARGED_POSE)) {
                mario->mot.hammer2.flags |= HAMMER2_FLAG_CHARGED_POSE;
                switch (pouchGetHammerLv()) {
                case 1:
                    marioChgPose("M_H_3A");
                    break;
                case 2:
                    marioChgPose("M_H_6A");
                    break;
                case 3:
                    marioChgPose("M_H_9A");
                    break;
                }
            }
        }
        if (strcmp(mario->paperAnimName, "P_H_1A") == 0) {
            animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], mario->mot.hammer2.animTime / 6.0f);
        }
    case 11:
        if (!(mario->buttonHold & PAD_BUTTON_B)) {
            // FAKE: u32 cast against 0xFFFFFFFFU; the plain s32 compare with -1 changes the object
            if ((u32)mario->mot.hammer2.sfxId != 0xFFFFFFFFU) {
                psndSFXOff(mario->mot.hammer2.sfxId);
                mario->mot.hammer2.sfxId = -1;
            }
            m = marioGetPtr();
            m->camShiftTargetZ = 0.0f;
            m->camShiftRate.z = 0.01f;
            switch (pouchGetHammerLv()) {
            case 1:
                marioChgPose("M_H_3B");
                break;
            case 2:
                marioChgPose("M_H_6B");
                break;
            case 3:
                marioChgPose("M_H_9B");
                break;
            }
            marioChgPaper("P_H_1C");
            mario->currSubMotionId = 20;
            if (mario->hammerChargeLv == 3) {
                mario->multiTimer = 0x30;
            } else if (mario->hammerChargeLv == 2) {
                mario->multiTimer = 0x42;
            } else {
                mario->multiTimer = 0x48;
            }
            mario->mot.hammer2.timer = 0;
            if (mario->hammerChargeLv == 3) {
                mario->mot.hammer2.spinSpeed = 38.0f;
            } else if (mario->hammerChargeLv == 2) {
                mario->mot.hammer2.spinSpeed = 22.0f;
            } else if (mario->hammerChargeLv <= 1) {
                mario->mot.hammer2.spinSpeed = 15.0f;
            }
            effUltraHammerEntry(mario->position.x, 20.0f + mario->position.y, mario->position.z, 1.0f, mario->hammerDir != 0, mario->multiTimer);
        }
        break;
    case 20:
        ang = mario->displayDirection;
        mario->mot.hammer2.timer = 10;
        mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        ang2 = revise360(270.0f - mario->displayDirection);
        hit = hammerHitWall(ang2, &s, &co, &len, &hx, &hy, &hz, &nx, &ny, &nz);
        if (hit && (hitGetAttr(hit) & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO))) {
            ang = ang; // FAKE: self-assignment keeps the branch; removing it (or the if) changes codegen
        }
        if (hit) {
            mario->hitobjHammer = hit;
        }
        mario->displayDirection = mario->mot.hammer2.spinSpeed * (mario->hammerDir ? -1.0f : 1.0f) + mario->displayDirection;
        if ((s32)ang / 360 != (s32)mario->displayDirection / 360) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN2), &mario->position);
        }
        if (mario->mot.hammer2.timer == 1) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN2), &mario->position);
        }
        mario->multiTimer--;
        if (mario->hammerChargeLv > 2 && mario->multiTimer == 10) {
            h2ClrPaper(mario);
        }
        if (mario->multiTimer > 0) {
            break;
        }
        if (mario->hammerChargeLv <= 2) {
            mario->multiTimer = sysMsec2Frame(300);
        } else {
            mario->displayDirection = revise360(mario->displayDirection);
            marioAdjustMoveDir();
            mario->multiTimer = 30;
        }
        mario->currSubMotionId = 22;
    case 22:
        if (--mario->multiTimer > 0) {
            break;
        }
        if (mario->hammerChargeLv <= 2) {
            h2ClrPaper(mario);
            mario->displayDirection = revise360(mario->displayDirection);
            marioAdjustMoveDir();
        }
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mario->currSubMotionId = 23;
        mario->multiTimer = 1;
    case 23:
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->multiTimer = 1;
    case 24:
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioAdjustMoveDir();
        marioChgMotSub(MARIO_MOTION_STAY, 0);
        break;
    }
}


/**
 * Current facing angle of the spinning hammer, for npcdrv to see whether it hits an NPC:
 * -1 if Mario is not doing the spin hammer, -2 if not in the spinning phase.
 */
f32 marioGetAngleKaitenHammer(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_HAMMER_2) {
        return -1.0f;
    }
    if (mario->currSubMotionId != 20) {
        return -2.0f;
    }
    return revise360(270.0f - mario->displayDirection);
}


/**
 * Cleanup run whenever the spin hammer motion ends:
 * paper animation off, camera shift reset and the facing and (JP and later) mirror locks released.
 */
void mot_ham2_post(void) {
    MarioWork* mario = marioGetPtr();
    MarioWork* m;
    if (mario->dispFlags & MARIO_DISP_FLAG_PAPER) {
        marioPaperOff();
    }
    m = marioGetPtr();
    m->camShiftTargetZ = 0.0f;
    m->camShiftRate.z = 0.01f;
    marioPaperOff();
#if VERSION <= VERSION_DEMO_JP
    marioPaperLightOn();
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
#else
#if VERSION < VERSION_US
    marioPaperLightOn();
#endif
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->dispFlags &= ~MARIO_DISP_FLAG_FLIP;
#endif
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioAdjustMoveDir();
}

/**
 * Probes in front of Mario where the hammer will land and returns the hit object, or NULL;
 * *out gets the hit kind (0 nothing, 4/5/6 a wall or object at high, middle or low height,
 * 7 something just above Mario's head height) that picks the swing pose, effect and length.
 */
#if VERSION <= VERSION_DEMO_JP
static HitEntry* hitEstimate(s32* out) {
#else
static HitEntry* hitEstimate(s32* out) {
#endif
    MarioWork* mario;
    HitCheckQuery q;
    Vec pos;
    HitEntry* hit;
    f32 s;
    f32 c;
    f32 cy;
    f32 a;
    Vec dir4Copy;
    f32 h;
    s32 i;
    // FAKE: 5.0f held in k5 for four products; the literal changes the object
    f32 k5 = 5.0f;
#if VERSION <= VERSION_DEMO_JP
    f32 ang;
#endif
    mario = marioGetPtr();
    *out = HAMMER_HIT_NONE;
    a = mario->displayDirection;
    {
        f32 off = (marioGetPtr()->dispFlags & MARIO_DISP_FLAG_REAR_POSE) ? -20.0f : 0.0f;
        if (a >= 90.0f && a <= 270.0f) {
            a = toMovedir2(180.0f, off);
        } else {
            a = toMovedir2(0.0f, off);
        }
    }
    sincosf(&s, &c, a);
    h = mario->wPlayerCollisionBox.y;
#if VERSION <= VERSION_DEMO_JP
    ang = 3.14159265f;
    cy = (f32)cos(ang);
#else
    cy = (f32)cos(3.14159265f);
#endif
    pos = mario->position;
    {
        Vec dir1 = {0.0f, 0.0f, 0.0f};
        pos.y = 4.0f + (mario->position.y + h);
        dir1.x = s;
        dir1.y = -cy;
        dir1.z = c;
        q.targetPos = pos;
        q.targetDir = dir1;
        q.targetDistance = 14.0f;
        hit = hitCheckVecFilter(&q, NULL);
        if (hit && (!(hitGetAttr(hit) & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)))) {
            *out = HAMMER_HIT_OVERHEAD;
            return hit;
        }
    }
#if VERSION <= VERSION_DEMO_JP
    ang = 1.2217306f;
    cy = (f32)cos(ang);
#else
    cy = (f32)cos(1.2217306f);
#endif
    {
        Vec dir2 = {0.0f, 0.0f, 0.0f};
        pos.x = (s * k5) + mario->position.x;
        pos.z = (c * k5) + mario->position.z;
        pos.y = mario->position.y + h;
        dir2.x = s;
        dir2.y = -cy;
        dir2.z = c;
        q.targetPos = pos;
        q.targetDir = dir2;
        q.targetDistance = 26.0f;
        hit = hitCheckVecFilter(&q, NULL);
        if (hit && (!(hitGetAttr(hit) & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)))) {
            *out = (q.hitPos.y - mario->position.y > 30.0f) ? HAMMER_HIT_HIGH : HAMMER_HIT_MIDDLE;
            return hit;
        }
    }
#if VERSION <= VERSION_DEMO_JP
    ang = 0.78539816f;
    cy = (f32)cos(ang);
#else
    cy = (f32)cos(0.78539816f);
#endif
    {
        Vec dir3 = {0.0f, 0.0f, 0.0f};
        pos.x = (s * k5) + mario->position.x;
        pos.z = (c * k5) + mario->position.z;
        pos.y = mario->position.y + h;
        dir3.x = s;
        dir3.y = -cy;
        dir3.z = c;
        q.targetPos = pos;
        q.targetDir = dir3;
        q.targetDistance = 30.0f;
        hit = hitCheckVecFilter(&q, NULL);
        if (hit && (!(hitGetAttr(hit) & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)))) {
            *out = HAMMER_HIT_MIDDLE;
            return hit;
        }
    }
#if VERSION <= VERSION_DEMO_JP
    ang = 0.0f;
    cy = (f32)cos(ang);
#else
    cy = (f32)cos(0.0f);
#endif
    {
        Vec dir4 = {0.0f, 0.0f, 0.0f};
        dir4.y = -cy;
        dir4Copy = dir4; // FAKE: Vec copy through a temp; Vec dir4b = dir4 differs
        {
            Vec dir4b = dir4Copy;
            for (i = 0; i < 2; i++) {
                f32 m;
                f32 d;
                if (i == 0) {
                    d = 36.0f;
                    m = 20.0f;
                } else {
                    d = 34.0f;
                    if (mario->dispFlags & MARIO_DISP_FLAG_REAR_POSE) {
                        m = 37.0f;
                    } else {
                        m = d;
                    }
                }
                pos.x = (s * m) + mario->position.x;
                pos.z = (c * m) + mario->position.z;
                pos.y = mario->position.y + h;
                q.targetPos = pos;
                q.targetDir = dir4b;
                q.targetDistance = d;
                hit = hitCheckVecFilter(&q, NULL);
                if (hit && (!(hitGetAttr(hit) & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)))) {
                    *out = (q.hitPos.y - mario->position.y > 13.0f) ? HAMMER_HIT_MIDDLE : HAMMER_HIT_LOW;
                    return hit;
                }
            }
        }
    }
    return hit;
}

/**
 * Early demos: not_fullswing_mobj, a list of map object kinds that nothing reads.
 * Then the names of the small and big blocks that only a level 1, 2 or 3 hammer (Hammer, Super, Ultra) can break.
 */
#if VERSION <= VERSION_DEMO_JP
const char* not_fullswing_mobj[] = {
    "MOBJ_RedJumpStand",
    "MOBJ_RedJumpStand2",
    "MOBJ_BlueJumpStand",
    "MOBJ_BlueJumpStand2",
    "MOBJ_MapJumpStand",
    "MOBJ_SaveBlock",
    "MOBJ_RecoverBlock",
    "MOBJ_Lv1BigBigBlock",
    "MOBJ_Lv2BigBigBlock",
    "MOBJ_Lv3BigBigBlock",
    "MOBJ_green_FlowBlock",
    "MOBJ_IceBlock",
    "MOBJ_PowerBlock",
    "MOBJ_TreasureBox",
    "MOBJ_BigTreasureBox",
    "MOBJ_Lock",
    "MOBJ_GreenPipeL",
    "MOBJ_GreenPipeW",
    "MOBJ_Singboard",
    "MOBJ_Arrow",
    "MOBJ_HardWoodBox",
    "MOBJ_BombRock",
    NULL,
};
#endif
static const char isS1_str[] = "MOBJ_Lv1Block";
static const char isB1_str[] = "MOBJ_Lv1BigBlock";
static const char isS2_str[] = "MOBJ_Lv2Block";
static const char isB2_str[] = "MOBJ_Lv2BigBlock";
static const char isS3_str[] = "MOBJ_Lv3Block";
static const char isB3_str[] = "MOBJ_Lv3BigBlock";

/**
 * Casts a ray from Mario's mid height along angle ang (degrees) for twice his collision width,
 * returning the wall hit and its position and normal through the pointers.
 */
static HitEntry* hammerHitWall(f32 ang, f32* s, f32* co, f32* len, f32* hx, f32* hy, f32* hz, f32* nx, f32* ny, f32* nz) {
    MarioWork* mario = marioGetPtr();
    sincosf(s, co, ang);
    *len = 2.0f * mario->wPlayerCollisionBox.x;
    return hitCheckFilter(NULL, hx, hy, hz, len, nx, ny, nz, mario->position.x, 0.5f * mario->wPlayerCollisionBox.y + mario->position.y, mario->position.z, *s, 0.0f, *co);
}
