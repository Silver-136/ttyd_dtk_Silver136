// Mario's tube (roll) motion: rolling up, moving through small gaps and unrolling.

#include "version.h"
#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include <string.h>
#include "libc_decls.h"
#include <math.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "mario/mario_sbr.h"
#include "system.h"
#include "mario/mario_hit.h"
#include "mot/mot_roll.h"
#include "party.h"

typedef struct MotRollWork {
    f32 spdX;           // 0x00: roll velocity x (marioResetRollSpd clears it)
    f32 spdZ;           // 0x04: roll velocity z
    s32 squashTrigger;  // 0x08: nonzero restarts the squash animation (-> squashState 1)
    s32 squashState;    // 0x0C: y-scale squash state machine (1.4 / back to 1.0)
    s32 squashTimer;     // 0x10, never accessed
#if VERSION >= VERSION_US
    u32 sfxId;          // 0x14: US and later only, rolling sound (PSND_SFX_MARIO_ROLL_MOVE1), -1 when off
#endif
} MotRollWork;          // size 0x14 before US, 0x18 US and later

f32 roll_grx;
f32 roll_grz;

// flyout_vy, flyout_vx, roll_flyout_flag and roll_flyout_trig: stripped (UNUSED in the JP demo map, 4 bytes each,
// .sbss order trig, flag, flag2, vx, vy); nothing reads them. Types from the names and roll_grx/roll_flyout_flag2.
f32 flyout_vy;
f32 flyout_vx;
s32 roll_flyout_flag2;
s32 roll_flyout_flag;
s32 roll_flyout_trig;

/**
 * Tuning values of Mario's paper tube rolling: top speed, slowing, the slope push (180 or 140
 * by which way he rolls across the tilt), and the speed factor on slopes.
 */
static f32 rollData[] = {3.0f, -0.05f, -0.5f, 0.5f, 7.0f, 180.0f, 140.0f, 10.0f, 1.2f, 2.0f, 0.8f};

/** sin and cos of an angle in degrees. */
static inline f32 sind(f32 deg) { return sin(3.1415927f * deg / 180.0f); }
static inline f32 cosd(f32 deg) { return cos(3.1415927f * deg / 180.0f); }

void roll_upstairs(void);
static f32 makeMovedir(f32 dir, f32 target);
static s32 chkReverseDir(f32 center, f32 range, f32 x);
static s32 chkReverseDir2(f32 range);
static s32 chkDirSide(f32 dir, f32 target);
static void makeOfs(void);

/** Tuning value i of rollData. */
// Original name and size (0x14) from the JP demo map (UNUSED there: every call is inlined).
static f32 getRollVal(s32 i) {
    return rollData[i];
}

/** Marks the rolling sound as not playing (sfxId -1) when the roll work is set up. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void roll_se_init(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION >= VERSION_US
    if (marioGetPtr()->motStruct) {
        marioGetPtr()->motRoll->sfxId = -1;
    }
#else
    mario->mot.roll.sfxId = -1;
#endif
}

/** Starts the looping rolling sound (SFX_MARIO_ROLL_MOVE1) at Mario unless it is already playing. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void move_se_on(void) {
    MarioWork* m = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    if (m->mot.roll.sfxId == -1) {
        m->mot.roll.sfxId = (s32)psndSFXOn_3D((char*)PSND_SFX_MARIO_ROLL_MOVE1, &m->position);
#elif VERSION >= VERSION_US
    if (marioGetPtr()->motStruct && marioGetPtr()->motRoll->sfxId == -1) {
        marioGetPtr()->motRoll->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_MOVE1), &m->position);
#else
    if (m->mot.roll.sfxId == -1) {
        m->mot.roll.sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_MOVE1), &m->position);
#endif
        m->mot.roll.flags |= ROLL_FLAG_SFX_UNUSED;
    }
}

/** Stops the looping rolling sound if it is playing. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void move_se_off(void) {
    MarioWork* m = marioGetPtr();
#if VERSION >= VERSION_US
    if (marioGetPtr()->motStruct && marioGetPtr()->motRoll->sfxId != -1) {
        psndSFXOff(marioGetPtr()->motRoll->sfxId);
        marioGetPtr()->motRoll->sfxId = -1;
#else
    if (m->mot.roll.sfxId != -1) {
        psndSFXOff(m->mot.roll.sfxId);
        m->mot.roll.sfxId = -1;
#endif
    }
}

/** The tube jump sound (SFX_MARIO_ROLL_JUMP1) at Mario. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void roll_jump_se(void) {
#if VERSION <= VERSION_DEMO_JP
    psndSFXOn_3D((char*)PSND_SFX_MARIO_ROLL_JUMP1, &marioGetPtr()->position);
#else
    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_JUMP1), &marioGetPtr()->position);
#endif
}

/** Moves the camera in while Mario rolls up: z shift 0.4 at rate 0.01, zoom 2, with a 40-unit upward zoom offset. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void zoomUp(void) {
    MarioWork* m = marioGetPtr();
    m->camShiftTargetZ = 0.4f;
    m->camShiftRate.z = 0.01f;
    m->camZoom = 2.0f;
    m->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

/** Eases the camera's forward shift back to 0 (rate 0.004). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void zoomOff(void) {
    MarioWork* m = marioGetPtr();
    m->camShiftTargetZ = 0.0f;
    m->camShiftRate.z = 0.004f;
}

/** Locks Tube Mode on (ROLL_FLAG_FIX) so Mario cannot unroll; only while in the roll motion. */
void marioRollFixOn(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        mario->mot.roll.flags |= ROLL_FLAG_FIX;
    }
}

/** Releases the Tube Mode lock (ROLL_FLAG_FIX); only while in the roll motion. */
// Original name and size (0x38) from the JP demo map (UNUSED there: nothing calls it).
void marioRollFixOff(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        mario->mot.roll.flags &= ~ROLL_FLAG_FIX;
    }
}

/**
 * 1 when Mario could not unroll right now (locked, under a ceiling, not on the ground,
 * or walls too close around him); the item menu refuses items then.
 * Descriptive name (not in the symbol maps); placeholder unk_8009fcbc, JP 0x8009FCBC. See docs/NAME_PROVENANCE.md.
 */
s32 marioRollCancelDisable(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->mot.roll.flags & ROLL_FLAG_FIX) {
        return 1;
    }
    if ((mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING) != 0) {
        return 1;
    }
    return !mario->hitobjStandOn ? 1 : (marioSlitChkWallAround() != 0);
}

/**
 * 0 while Mario is rolling up or unrolling (ROLL_SUB_INIT to ROLL_UP, UNROLL_START to UNROLL_END), 1 otherwise;
 * marioChkKey treats 0 as no control.
 */
s32 marioRollKeyDisable(void) {
    MarioWork* mario = marioGetPtr();
    s32 sub;
    if (mario->currMotionId != MARIO_MOTION_ROLL) {
        return 1;
    }
    sub = mario->currSubMotionId;
    if (sub >= ROLL_SUB_INIT && sub <= ROLL_SUB_ROLL_UP) {
        return 0;
    }
    if (sub >= ROLL_SUB_UNROLL_START && sub <= ROLL_SUB_UNROLL_END) {
        return 0;
    }
    return 1;
}

/**
 * Marks the roll as script-driven (ROLL_FLAG_EVT);
 * set by evt_mario_jump_pos while it moves Mario in Tube Mode.
 */
void setRollEvtFlag(void) {
    MarioWork* mario = marioGetPtr();
    mario->mot.roll.flags |= ROLL_FLAG_EVT;
}

/** Clears the script-driven roll flag once evt_mario_jump_pos ends. */
void clrRollEvtFlag(void) {
    MarioWork* mario = marioGetPtr();
    mario->mot.roll.flags &= ~ROLL_FLAG_EVT;
}

/**
 * 1 while a script is moving Mario in Tube Mode (ROLL_FLAG_EVT);
 * mario.c and mario_motion.c then skip the player-driven movement.
 */
s32 getRollEvtFlag(void) {
    s32 r = 0;
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        if (mario->mot.roll.flags & ROLL_FLAG_EVT) {
            r = 1;
        }
    }
    return r;
}

/** Stops the roll: clears the roll velocity and Mario's speed, if he is in Tube Mode. */
void marioResetRollSpd(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        if (marioGetPtr()->motStruct) {
            marioGetPtr()->motRoll->spdX = 0.0f;
            marioGetPtr()->motRoll->spdZ = 0.0f;
        }
        mario->baseSpeed = 0.0f;
    }
}

/** Re-applies the tube paper animation (p_roll, PM_R_1B) when mario.c rebuilds Mario's look while he is rolled up. */
void marioForceRollAnime(void) {
    MarioWork* mario = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn("p_roll");
    marioChgPose("M_I_U");
    marioChgPaper("PM_R_1B");
    mario->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
}

/**
 * Starts unrolling (ROLL_SUB_UNROLL_START) if Mario is in Tube Mode and could stand up here; returns 1 if it started.
 * Used by evt_mario when a script needs Mario out of Tube Mode (JP and later).
 * Descriptive name (not in the symbol maps); placeholder unk_8009f9c8, JP 0x8009F9C8. See docs/NAME_PROVENANCE.md.
 */
s32 marioRollTryCancel(void) {
    s32 ret = 0;
    MarioWork* mario = marioGetPtr();
    s32 r;
    MarioWork* m;
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return 0;
    }
    if (mario->mot.roll.flags & ROLL_FLAG_FIX) {
        return 0;
    }
    m = marioGetPtr();
    if (m->mot.roll.flags & ROLL_FLAG_FIX) {
        r = 1;
    } else if (m->flags & MARIO_FLAG_IS_TOUCHING_CEILING) {
        r = 1;
    } else if (!m->hitobjStandOn) {
        r = 1;
    } else if (marioSlitChkWallAround()) {
        r = 1;
    } else {
        r = 0;
    }
    if (!r) {
        ret = 1;
        mario->currSubMotionId = ROLL_SUB_UNROLL_START;
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
    }
    return ret;
}

/**
 * Starts unrolling (ROLL_SUB_UNROLL_START) unless Tube Mode is locked (the early demos also refuse under a
 * ceiling); returns 1 if it started. Called from evt_mario.
 */
s32 marioRollCancel(void) {
#if VERSION <= VERSION_DEMO_JP
    s32 ret = 0;
#endif
    MarioWork* mario = marioGetPtr();
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return 0;
    }
    if (mario->mot.roll.flags & ROLL_FLAG_FIX) {
        return 0;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING)) {
        ret = 1;
        mario->currSubMotionId = ROLL_SUB_UNROLL_START;
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
    }
    return ret;
#else
    mario->currSubMotionId = ROLL_SUB_UNROLL_START;
    camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
    return 1;
#endif
}

/**
 * Cleanup when the roll motion ends: stops the roll sound (JP and later), resets the camera shift,
 * turns Mario to face left or right on screen, restores his normal collision (marioSetSpec),
 * clears Tube Mode and the paper animation, frees the motion's work and dismounts the partners.
 */
void mot_roll_post(void) {
    MarioWork* mario = marioGetPtr();

#if VERSION >= VERSION_JP
    move_se_off();
#endif
    zoomOff();
    {
        f32 d = revise360(mario->directionView - mario->targetYaw);
        if (d >= 180.0f) {
            mario->displayDirection = 0.0f;
            mario->displayDirectionTarget = 0.0f;
            mario->moveDirection = 270.0f;
            mario->directionView = 270.0f;
        } else {
            mario->displayDirection = 180.0f;
            mario->displayDirectionTarget = 180.0f;
            mario->moveDirection = 90.0f;
            mario->directionView = 90.0f;
        }
    }
    marioSetSpec();
    mario->rollFlags = ROLL_STATE_NONE;
    mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    mario->rot = (Vec){0.0f, 0.0f, 0.0f};
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->flags &= ~MARIO_FLAG_TUBE_MODE;
    marioPaperOff();
    marioAdjustMoveDir();
    marioChgPose("M_S_1");
    if (marioGetPtr()->motStruct) {
        __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
        marioGetPtr()->motStruct = NULL;
    }
    allPartyRideOff();
}

/**
 * Instantly turns Mario back from Tube Mode (the same reset as mot_roll_post) and puts him in the stay motion;
 * used by evt_mario.
 */
void marioRollForceCancel(void) {
    MarioWork* mario = marioGetPtr();
    f32 d;
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
#if VERSION >= VERSION_JP
        move_se_off();
#endif
        zoomOff();
        d = revise360(mario->directionView - mario->targetYaw);
        if (d >= 180.0f) {
            mario->displayDirection = 0.0f;
            mario->displayDirectionTarget = 0.0f;
            mario->moveDirection = 270.0f;
            mario->directionView = 270.0f;
        } else {
            mario->displayDirection = 180.0f;
            mario->displayDirectionTarget = 180.0f;
            mario->moveDirection = 90.0f;
            mario->directionView = 90.0f;
        }
        marioSetSpec();
        mario->rollFlags = ROLL_STATE_NONE;
        mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
        mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
        mario->rot = (Vec){0.0f, 0.0f, 0.0f};
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
        mario->flags &= ~MARIO_FLAG_TUBE_MODE;
        marioPaperOff();
        marioAdjustMoveDir();
        marioChgPose("M_S_1");
        if (marioGetPtr()->motStruct) {
            __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
            marioGetPtr()->motStruct = NULL;
        }
        allPartyRideOff();
        marioChgMotSub(MARIO_MOTION_STAY, 0);
    }
}

void marioSetRollDispDir(void) {
    MarioWork* mario = marioGetPtr();
    f32 dir = makeMovedir(mario->moveDirection, mario->directionView);

    {
        f32 md = toMovedir(mario->displayDirection);

        if (mario->hammerDir) {
            md = revise360(180.0f + md);
        }
        if (chkReverseDir(dir, 270.0f, md) == 1) {
            mario->hammerDir = (mario->hammerDir + 1) & 1;
        }
    }
    {
        f32 t = 270.0f + mario->targetYaw - dir;

        mario->displayDirection = revise360(t + (mario->hammerDir ? -180.0f : 0.0f));
    }
    if (mario->displayDirection >= 200.0f) {
        mario->displayDirection = revise360(270.0f + mario->targetYaw - dir + (mario->hammerDir ? -180.0f : 0.0f));
    }
    if (mario->displayDirection < 170.0f) {
        mario->displayDirection = revise360(270.0f + mario->targetYaw - dir + (mario->hammerDir ? -180.0f : 0.0f));
    }
}

/** Starts a Tube Mode jump from the ground (A pressed): the jump speed curve, the jumping flag and the jump sound. */
// Original name and size (0x78) from the JP demo map (UNUSED there: every call is inlined).
void rollJumpSetup(void) {
    MarioWork* mario = marioGetPtr();
    camFollowYOff();
    mario->jumpVelocityY = 2.85f;
    mario->jumpAccelerationY = -0.14f;
    mario->jumpJerkY = 0.08f;
    mario->jumpSnapY = -0.02f;
    mario->flags |= MARIO_FLAG_IS_JUMPING;
    mario->airTimer = 1;
    roll_jump_se();
}

/** Starts a scripted jump in Tube Mode: evt_mario's jump command calls this instead of switching to the jump motion. */
void rollEvtJumpSetup(void) {
    MarioWork* mario = marioGetPtr();
    camFollowYOff();
    mario->flags |= MARIO_FLAG_IS_JUMPING;
    mario->airTimer = 1;
}

/** Tube Mode's base shape: the 14.8 collision box, roll speed, zero roll velocity, display offsets and scale. */
// Original name and size (0xb4) from the JP demo map (UNUSED there: every call is inlined).
static void roll_base_init(void) {
    MarioWork* mario = marioGetPtr();

    mario->wPlayerCollisionBox.x = 14.8f;
    mario->wPlayerCollisionBox.y = 14.8f;
    mario->baseDashSpeed = getRollVal(0);
    marioGetPtr()->motRoll->spdX = 0.0f;
    marioGetPtr()->motRoll->spdZ = 0.0f;
    mario->rotOfs.x = -6.0f;
    mario->rotOfs.y = 6.0f;
    mario->dispOfs.x = 6.0f;
    mario->dispOfs.y = 0.0f;
    mario->scale.x = 1.0f;
    mario->scale.y = 1.0f;
    mario->scale.z = 1.0f;
    mario->tubeScaleY = 1.0f;
}

/**
 * Restores Tube Mode when Mario is re-entered on a new map while rolled up (marioEntry):
 * allocates the roll work, sets the tube animation, the 14.8 collision box and roll speed,
 * and seats the partners.
 */
void marioReInit_roll(void) {
    void* p;
    MarioWork* mario = marioGetPtr();
    p = __memAlloc(HEAP_DEFAULT, sizeof(MotRollWork));
    marioGetPtr()->motStruct = p;
    memset(marioGetPtr()->motStruct, 0, sizeof(MotRollWork));
    roll_se_init();
    mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    mario->currMotionId = MARIO_MOTION_ROLL;
    mario->flags |= MARIO_FLAG_TUBE_MODE;
    mario->rollFlags = ROLL_STATE_ROLLING;
    marioPaperOn("p_roll");
    marioChgPose("M_I_U");
    marioChgPaper("PM_R_1B");
    mario->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->currSubMotionId = ROLL_SUB_ROLLING;
    roll_base_init();
    allPartyForceRideOn();
}

/** 1 when Mario cannot unroll here: roll locked, ceiling overhead, no floor under him or walls around him. */
static inline s32 rollChk(void) {
    s32 r;
    MarioWork* mario = marioGetPtr();
    if (mario->mot.roll.flags & ROLL_FLAG_FIX) {
        r = 1;
    } else if (mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING) {
        r = 1;
    } else if (!mario->hitobjStandOn) {
        r = 1;
    } else if (marioSlitChkWallAround()) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

/**
 * Tube Mode motion: Mario rolls up into a tube, then rolls along with the stick,
 * momentum and slope push (rollData constants), jumps a little with A (not on eki_04's steep slopes),
 * and unrolls with B when there is room. Plays the rolling loop sound and squashes the tube on landing.
 */
void mot_roll(void) {
    void roll_upstairs(void);
    MarioWork* mario = marioGetPtr();
    f32 pushAng;
    f32 moveAng;
    f32 spdZSq;
    f32 dir;
    f32 spdX;
    f32 spdZ;
    f32 slopeZ;
    f32 scaleY;
    f32 tcA;
    f32 nx;
    f32 nz;
    f32 maxSpdX;
    f32 maxSpdZ;
    f32 s;
    f32 rd;
    s32 flip;
    f32 sgn;
    f32 mv;
    f32 pre;
    MarioWork* m;
    s32 z0;
    s32 z1;
    void* p;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->mot.roll.flags = 0;
        mario->mot.word[2] = 0;
        mario->mot.word[3] = 0;
        mario->mot.word[4] = 0;
        mario->mot.word[5] = 0;
        p = __memAlloc(HEAP_DEFAULT, sizeof(MotRollWork));
        marioGetPtr()->motStruct = p;
        memset(marioGetPtr()->motStruct, 0, sizeof(MotRollWork));
        roll_se_init();
        mario->currSubMotionId = ROLL_SUB_INIT;
    }
    switch (mario->currSubMotionId) {
        case ROLL_SUB_INIT:
            z0 = 0;
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
            marioAdjustMoveDir();
#if VERSION <= VERSION_DEMO_JP
            mario->hammerDir = 0;
#else
            dir = mario->displayDirection;
            if (dir < 90.0f || dir > 270.0f) {
                mario->hammerDir = 1;
                mario->directionView = revise360(270.0f - mario->targetYaw);
                mario->moveDirection = 270.0f;
            } else {
                mario->hammerDir = 0;
                mario->directionView = revise360(90.0f - mario->targetYaw);
                mario->moveDirection = 90.0f;
            }
#endif
            marioGetRubInit();
            mario->flags |= MARIO_FLAG_TUBE_MODE;
            mario->rollFlags = ROLL_STATE_ROLLING;
            mario->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
            mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            marioPaperOn("p_roll");
            marioChgPose("M_I_U");
            marioChgPaper("PM_R_1A");
            mario->multiTimer = 0x41;
            roll_base_init();
            mario->airTimer = 0;
            if (mario->forceMoveTimer == 0) {
                mario->baseSpeed = 0.0f;
            }
            zoomUp();
            marioGetPtr()->motRoll->squashTrigger = 0;
            marioGetPtr()->motRoll->squashState = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_ROLL1), &mario->position);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_FLD_JUMP3), &mario->position);
            mario->currSubMotionId = ROLL_SUB_ROLL_UP;
            break;
        case ROLL_SUB_ROLL_UP:
            if (mario->multiTimer == 0x36) {
                allPartyRideShip();
            }
            mario->dispOfs.y += 0.1f;
            if (mario->dispOfs.y > 1.0f) {
                mario->dispOfs.y = 1.0f;
            }
            if (--mario->multiTimer <= 0) {
#if VERSION <= VERSION_DEMO_JP
                dir = mario->displayDirection;
                if (dir < 90.0f || dir > 270.0f) {
                    mario->hammerDir = 1;
                    mario->directionView = revise360(270.0f - mario->targetYaw);
                    mario->moveDirection = 270.0f;
                } else {
                    mario->hammerDir = 0;
                    mario->directionView = revise360(90.0f - mario->targetYaw);
                    mario->moveDirection = 90.0f;
                }
#endif
                mario->currSubMotionId = ROLL_SUB_ROLLING;
                marioChgPose("M_I_U");
                marioChgPaper("PM_R_1B");
                camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
            }
            break;
        case ROLL_SUB_UNROLL_START:
            mario->baseSpeed = 0.0f;
            mario->rot.x = 0.0f;
            mario->rot.z = 0.0f;
            mario->rotOfs.x = 0.0f;
            mario->rotOfs.y = 0.0f;
            dir = revise360(mario->directionView - mario->targetYaw);
            mario->dispOfs.x = (dir < 180.0f) ? 3.0f : -3.0f;
            mario->dispOfs.y = 0.0f;
            mario->multiTimer = 0x41;
            mario->currSubMotionId = ROLL_SUB_UNROLL;
            mario->displayDirection = 0.0f;
#if VERSION >= VERSION_JP
            move_se_off();
#endif
            zoomOff();
            marioChgPaper("PM_R_1C");
            marioPaperLightOff();
#if VERSION <= VERSION_DEMO_JP
            psndSFXOn_3D((char*)PSND_SFX_MARIO_CHANGE_ROLL2, &mario->position);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_ROLL2), &mario->position);
#endif
        case ROLL_SUB_UNROLL:
            if (--mario->multiTimer > 0) {
                dir = revise360(mario->directionView - mario->targetYaw);

                // BUG: in both branches the parenthesis closes after the comparison, so the snap tests t - n < 1
                // instead of |t - n| < 1 (case 0x14 sets displayDirection to 0, below t, so both tests agree here).
                if (dir < 180.0f) {
                    f32 t = 90.0f;
                    f32 diff = t - mario->displayDirection;
                    f32 n = 0.1f * diff;
                    n += mario->displayDirection;
                    if (__fabsf((f32)((t - n) < 1.0f))) {
                        n = t;
                    }
                    mario->displayDirection = n;
                    mario->displayDirectionTarget = n;
                } else {
                    f32 t = 270.0f;
                    f32 diff = t - mario->displayDirection;
                    f32 n = 0.1f * diff;
                    n += mario->displayDirection;
                    if (__fabsf((f32)((t - n) < 1.0f))) {
                        n = t;
                    }
                    mario->displayDirection = n;
                    mario->displayDirectionTarget = n;
                }
            } else {
                dir = revise360(mario->directionView - mario->targetYaw);
                if (dir >= 180.0f) {
                    mario->displayDirection = 0.0f;
                    mario->displayDirectionTarget = 0.0f;
                    mario->moveDirection = 270.0f;
                    mario->directionView = 270.0f;
                } else {
                    mario->displayDirection = 180.0f;
                    mario->displayDirectionTarget = 180.0f;
                    mario->moveDirection = 90.0f;
                    mario->directionView = 90.0f;
                }
                marioSetSpec();
                mario->rollFlags = ROLL_STATE_NONE;
#if VERSION <= VERSION_DEMO_JP
                mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
                mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
                mario->rot = (Vec){0.0f, 0.0f, 0.0f};
#else
                marioOfsRotReset();
#endif
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
                mario->flags &= ~MARIO_FLAG_TUBE_MODE;
                marioPaperOff();
                marioAdjustMoveDir();
                marioChgPose("M_S_1");
                if (marioGetPtr()->motStruct) {
                    void* z;
                    __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
                    z = 0;
                    marioGetPtr()->motStruct = z;
                }
                allPartyRideOff();
                camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
                mario->currSubMotionId = ROLL_SUB_UNROLL_END;
            }
            break;
        case ROLL_SUB_UNROLL_END:
            marioChgMotSub(MARIO_MOTION_STAY, FALSE);
            break;
    }
    if (mario->currSubMotionId < ROLL_SUB_ROLLING) {
        return;
    }
    if (mario->flags & MARIO_FLAG_IS_STEPPING) {
        roll_upstairs();
        return;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->mot.roll.flags & ROLL_FLAG_FIX) && (mario->buttonTrg & PAD_BUTTON_B)) {
        if (!(mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING)) {
#else
    if (mario->buttonTrg & PAD_BUTTON_B) {
        if (!rollChk()) {
#endif
            mario->currSubMotionId = ROLL_SUB_UNROLL_START;
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        } else {
            mario->buttonTrg |= PAD_BUTTON_A;
        }
    }
    scaleY = mario->tubeScaleY;
    if (marioGetPtr()->motRoll->squashTrigger) {
#if VERSION <= VERSION_DEMO_JP
        s32 z = 0;
        s32 one;
        marioGetPtr()->motRoll->squashTrigger = z;
        one = 1;
#else
        marioGetPtr()->motRoll->squashTrigger = 0;
#endif
        marioGetPtr()->motRoll->squashState = 1;
        scaleY = 1.0f;
    }
    switch (marioGetPtr()->motRoll->squashState) {
        case 1:
            if (scaleY < 1.4f) {
                f32 k = 1.0f;
                scaleY = k * (1.4499999f - scaleY) + scaleY;
                if (scaleY >= 1.4f) {
                    scaleY = 1.4f;
                }
            }
            if (mario->airTimer == 0) {
#if VERSION <= VERSION_DEMO_JP
                s32 t = 2;
#endif
                marioGetPtr()->motRoll->squashState = 2;
            }
            break;
        case 2:
            if (scaleY > 0.4f) {
                scaleY = 0.4f * (0.35f - scaleY) + scaleY;
                if (scaleY <= 0.4f) {
#if VERSION <= VERSION_DEMO_JP
                    s32 t = 3;
#endif
                    scaleY = 0.4f;
                    marioGetPtr()->motRoll->squashState = 3;
                }
            }
            break;
        case 3:
            if (scaleY < 1.0f) {
                scaleY = 0.5f * (1.05f - scaleY) + scaleY;
                if (scaleY >= 1.0f) {
#if VERSION <= VERSION_DEMO_JP
                    s32 t = 0;
#endif
                    marioGetPtr()->motRoll->squashState = 0;
                    scaleY = 1.0f;
                }
            }
            break;
    }
    mario->tubeScaleY = scaleY;
#if VERSION <= VERSION_DEMO_JP
    if (mario->airTimer == 0) {
        camFollowYOn();
    }
#endif
    if (!mario->hitobjStandOn && mario->hitobjFront && !marioChkBero(mario->hitobjFront)) {
        f32 vx = marioGetPtr()->motRoll->spdX;
        f32 vz = marioGetPtr()->motRoll->spdZ;
        vx += 0.2f * -vx;
        vz += 0.2f * -vz;
        if (__fabsf(vx) < 1.0f) {
            vx = 0.0f;
        }
        if (__fabsf(vz) < 1.0f) {
            vz = 0.0f;
        }
        marioGetPtr()->motRoll->spdX = vx;
        marioGetPtr()->motRoll->spdZ = vz;
    }
#if VERSION <= VERSION_DEMO_JP
    if (mario->hitobjStandOn) {
        if (mario->airTimer == 0 && (mario->buttonTrg & PAD_BUTTON_A)) {
            rollJumpSetup();
        }
        marioUpdateCamCollision(mario->position);
    } else {
        mario->airTimer++;
        mario->airTimer--;
    }
#else
    if (mario->hitobjStandOn) {
        if (!(hitGetAttr(mario->hitobjStandOn) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_SPIKES)) && mario->airTimer == 0 && (mario->buttonTrg & PAD_BUTTON_A)) {
            if (!(strcmp(gp->currentMapName, "eki_04") == 0 && __fabsf(mario->groundSlope) >= 40.0f)) {
                rollJumpSetup();
                mario->hitobjJumpFrom = mario->hitobjStandOn;
                mario->hitobjStandOn = NULL;
                mario->fallStartPos = mario->position;
                mario->lastGroundPos = mario->position;
                camFollowYOff();
            }
        }
        marioUpdateCamCollision(mario->position);
        mario->airTimer = 0;
    }
#endif
    if (mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) {
        if (mario->hitobjHead) {
#if VERSION <= VERSION_DEMO_JP
            mario->jumpVelocityY = -0.03f;
            mario->jumpAccelerationY = -0.02f;
            mario->jumpJerkY = -0.02f;
            mario->jumpSnapY = -0.002f;
            mario->flags |= MARIO_FLAG_IS_FALLING;
        }
#else
            if (mario->jumpVelocityY >= 0.0f) {
                mario->jumpVelocityY = -0.03f;
                mario->jumpAccelerationY = -0.02f;
                mario->jumpJerkY = -0.02f;
                mario->jumpSnapY = -0.002f;
            }
            mario->flags |= MARIO_FLAG_IS_FALLING;
        }
        if (mario->airTimer == 0 && mario->jumpVelocityY < 0.0f) {
            mario->fallStartPos = mario->position;
            camFollowYOn();
        }
        if (mario->position.y < mario->lastGroundPos.y) {
            camFollowYOn();
        }
        mario->airTimer++;
#endif
    }
    spdX = marioGetPtr()->motRoll->spdX;
    spdZ = marioGetPtr()->motRoll->spdZ;
    nx = 0.0f;
    nz = 0.0f;
    if (!(mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
        f32 stickX, stickY;
        f32 inputAng;
        stickX = (f32)mario->wStickDir1;
        stickY = (f32)mario->wStickDir2;
        nz = distABf(nx, nx, stickX, stickY);
        inputAng = angleABf(0.0f, 0.0f, stickX, stickY);
        inputAng = revise360(mario->targetYaw + inputAng);
        nx = nz * sind(inputAng);
        nz = nz * -cosd(inputAng);
        inputAng = angleABf(0.0f, 0.0f, nx, nz);

        // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
        tcA = ((MarioWork*)mario)->directionView;
        if (chkReverseDir(tcA, 270.0f, inputAng) == 1) {
            nz = distABf(0.0f, 0.0f, nx, nz);
            nx = -nz * sind(mario->directionView);
            nz = nz * cosd(((MarioWork*)mario)->directionView);
        }
    }
    if (!(mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) && !(mario->mot.roll.flags & ROLL_FLAG_NO_SLOPE_UNUSED)) {
        f32 rollAng = angleABf(0.0f, 0.0f, spdX, -spdZ);
#if VERSION <= VERSION_DEMO_JP
        if (((MarioWork*)mario)->groundRot.z > 0.0f) {
#else
        if (((MarioWork*)mario)->groundRot.z > 0.05f) {
#endif
            f32 grip = (rollAng > 180.0f) ? getRollVal(5) : getRollVal(6);
            {
                f32 t = sind(((MarioWork*)mario)->groundRot.z);
                t = t * grip;
                nx = nx - t;
            }
            if (nx <= -100.0f) {
                nx = -100.0f;
            }
#if VERSION <= VERSION_DEMO_JP
        } else if (mario->groundRot.z < 0.0f) {
#else
        } else if (mario->groundRot.z < -0.05f) {
#endif
            f32 grip = (rollAng < 180.0f) ? getRollVal(5) : getRollVal(6);
            {
                f32 t = sind(mario->groundRot.z);
                t = t * grip;
                nx = nx - t;
            }
            if (nx >= 100.0f) {
                nx = 100.0f;
            }
        }
#if VERSION <= VERSION_DEMO_JP
        if (((MarioWork*)mario)->groundRot.x > 0.0f) {
#else
        if (((MarioWork*)mario)->groundRot.x > 0.05f) {
#endif
            f32 grip = (rollAng > 180.0f) ? getRollVal(5) : getRollVal(6);
            {
                f32 t = sind(((MarioWork*)mario)->groundRot.x);
                t = t * grip;
                nz = nz - t;
            }
            if (nz <= -100.0f) {
                nz = -100.0f;
            }
#if VERSION <= VERSION_DEMO_JP
        } else if (mario->groundRot.x < 0.0f) {
#else
        } else if (mario->groundRot.x < -0.05f) {
#endif
            f32 grip = (rollAng < 180.0f) ? getRollVal(5) : getRollVal(6);
            {
                f32 t = sind(mario->groundRot.x);
                t = t * grip;
                nz = nz - t;
            }
            if (nz >= 100.0f) {
                nz = 100.0f;
            }
        }
        pushAng = angleABf(0.0f, 0.0f, nx, nz);

        // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
        tcA = ((MarioWork*)mario)->directionView;
        if (chkReverseDir(tcA, 270.0f, pushAng) == 1) {
            nz = distABf(0.0f, 0.0f, nx, -nz);
            nx = -nz * sind(mario->directionView);
            nz = nz * cosd(((MarioWork*)mario)->directionView);
        }
    }
#if VERSION >= VERSION_JP
    if (mario->targetNpc) {
        if (!marioChkBero(mario->hitobjFront)) {
            spdX = 0.05f * -spdX + spdX;
            spdZ = 0.05f * -spdZ + spdZ;
            if (__fabsf(spdX) < 0.1f) {
                spdX = 0.0f;
            }
            if (__fabsf(spdZ) < 0.1f) {
                spdZ = 0.0f;
            }
        }
    }
#endif
    if (mario->hitobjFront && !mario->wStickDir1 && !mario->wStickDir2 && !marioChkBero(mario->hitobjFront)) {
        spdX = 0.6f * -spdX + spdX;
        spdZ = 0.6f * -spdZ + spdZ;
        if (__fabsf(spdX) < 0.1f) {
            spdX = 0.0f;
        }
        if (__fabsf(spdZ) < 0.1f) {
            spdZ = 0.0f;
        }
    }
    if (mario->forceMoveTimer == 0 && !nx && !nz) {
        if (mario->hitobjWall) {
            if (!marioChkBero(mario->hitobjWall)) {
                spdX = spdX * getRollVal(2) + spdX;
                spdZ = spdZ * getRollVal(2) + spdZ;
            }
        } else if (mario->hitobjStandOn && mario->airTimer == 0) {
            // BUG: the parenthesis closes after the comparison, so this tests groundSlope <= 1 (true for any
            // negative slope) instead of |groundSlope| <= 1.
            if (__fabsf((f32)(mario->groundSlope <= 1.0f))) {
                spdX = spdX * getRollVal(1) + spdX;
                spdZ = spdZ * getRollVal(1) + spdZ;
            } else {
                f32 a = spdX * getRollVal(1);
                f32 b = spdZ * getRollVal(1);
                spdX = 6.4f * a + spdX;
                spdZ = 6.4f * b + spdZ;
            }
        }
        if (__fabsf(spdX) < getRollVal(3)) {
            spdX = 0.0f;
        }
        if (__fabsf(spdZ) < getRollVal(3)) {
            spdZ = 0.0f;
        }
    } else {  // the speed clamp runs only on this path
        f32 accel = 0.025f;
        f32 slopeX;
        if ((mario->mot.roll.flags & ROLL_FLAG_SLOPE_DAMP_UNUSED) && mario->groundRot.z == 0.0f) {
            if (chkReverseDir2(270.0f) == 1) {
                spdX = 0.2f * -spdX + spdX;
                spdZ = 0.2f * -spdZ + spdZ;
                if (__fabsf(spdX) < 1.0f) {
                    spdX = 0.0f;
                }
                if (__fabsf(spdZ) < 1.0f) {
                    spdZ = 0.0f;
                }
                if (spdX == 0.0f && spdZ == 0.0f) {
                    mario->moveDirection = revise360(180.0f + mario->moveDirection);
                    mario->directionView = mario->moveDirection;
                }
            }
        }
        maxSpdX = getRollVal(0) / 0.05f;
        slopeX = 1.0f;
        if (mario->forceMoveTimer == 0 && __fabsf(mario->groundSlope) >= slopeX) {
            f32 ang = angleABf(0.0f, 0.0f, spdX, -spdZ);
            f32 sp = mario->groundRot.z;
            if (sp > 0.0f) {
                if (ang > 180.0f) {
                    slopeX = getRollVal(9);
                } else {
                    slopeX = 1.0f;
                }
            } else if (sp < 0.0f) {
                if (ang < 180.0f) {
                    slopeX = getRollVal(9);
                } else {
                    slopeX = 1.0f;
                }
            }
        }
        maxSpdX = maxSpdX * slopeX;
        spdZSq = spdZ * spdZ;
        if (spdX * spdX + spdZSq <= maxSpdX * maxSpdX) {
            spdX = nx * accel + spdX;
        } else {
            f32 n = -maxSpdX;
            spdX = nx * accel + spdX;
            if (spdX < n) {
                spdX = n;
            }
            if (spdX > maxSpdX) {
                spdX = maxSpdX;
            }
        }
        maxSpdZ = getRollVal(0) / 0.05f;
        slopeZ = 1.0f;
        if (mario->forceMoveTimer == 0 && __fabsf(mario->groundSlope) >= slopeZ) {
            f32 ang = angleABf(0.0f, 0.0f, spdX, -spdZ);
            f32 sp = mario->groundRot.x;
            if (sp > 0.0f) {
                if (ang > 180.0f) {
                    slopeZ = getRollVal(9);
                } else {
                    slopeZ = 1.0f;
                }
            } else if (sp < 0.0f) {
                if (ang < 180.0f) {
                    slopeZ = getRollVal(9);
                } else {
                    slopeZ = 1.0f;
                }
            }
        }
        maxSpdZ = maxSpdZ * slopeZ;
        if (spdX * spdX + spdZSq <= maxSpdZ * maxSpdZ) {
            spdZ = spdZ - nz * accel;
        } else {
            f32 n = -maxSpdZ;
            spdZ = spdZ - nz * accel;
            if (spdZ < n) {
                spdZ = n;
            }
            if (spdZ > maxSpdZ) {
                spdZ = maxSpdZ;
            }
        }
    }
    marioGetPtr()->motRoll->spdX = spdX;
    marioGetPtr()->motRoll->spdZ = spdZ;
    moveAng = angleABf(0.0f, 0.0f, spdX, -spdZ);
    s = 0.05f * distABf(0.0f, 0.0f, spdX * sind(moveAng), spdZ * cosd(moveAng));
    if (!getRollEvtFlag() && mario->forceMoveTimer == 0) {
        moveAng = angleABf(0.0f, 0.0f, spdX, -spdZ);
        if (!(mario->mot.roll.flags & ROLL_FLAG_NO_SLOPE_SPEED_UNUSED)) {
            s = s * mario->playerGravity;
            if (s < 0.05f) {
                s = 0.0f;
            }
            mario->baseSpeed = s;
            if (s) {
                mario->directionView = moveAng;
            }
        }
    }
    if (mario->baseSpeed) {
        move_se_on();
    } else {
        move_se_off();
    }
    // rollData[4] read directly here: as getRollVal(4) (every declaration slot tried) rd takes f26 instead of the
    // retail f28; the other rollData reads are getRollVal calls
    rd = rollData[4];
    mv = toMovedir(mario->displayDirection);

    // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
    tcA = ((MarioWork*)mario)->directionView;
    flip = chkReverseDir(tcA, 180.0f, mv);
    pre = mario->baseSpeed * rd;
    sgn = (flip == 1) ? -1.0f : 1.0f;
    mario->rot.z = revise360(pre * sgn + mario->rot.z);
    if (mario->forceMoveTimer == 0 && !getRollEvtFlag() && s) {
        dir = makeMovedir(mario->moveDirection, moveAng);
        {
            f32 md = toMovedir(mario->displayDirection);

            if (mario->hammerDir) {
                md = revise360(180.0f + md);
            }
            if (chkReverseDir(dir, 270.0f, md) == 1) {
                mario->hammerDir = (mario->hammerDir + 1) & 1;
            }
        }
        {
            f32 t = 270.0f + mario->targetYaw - dir;

            mario->displayDirection = revise360(t + (mario->hammerDir ? -180.0f : 0.0f));
        }
        if (mario->displayDirection >= 200.0f) {
            mario->displayDirection = revise360(270.0f + mario->targetYaw - dir + (mario->hammerDir ? -180.0f : 0.0f));
        }
        if (mario->displayDirection < 170.0f) {
            mario->displayDirection = revise360(270.0f + mario->targetYaw - dir + (mario->hammerDir ? -180.0f : 0.0f));
        }
        ((MarioWork*)mario)->directionView = dir;
        mario->moveDirection = dir;
    }
    if (mario->groundRot.z != 0.0f) {
        mario->rot.x = revise360(-mario->groundRot.z * sind(mario->displayDirection));
    } else {
        mario->rot.x = 0.0f;
    }
    if (mario->wStickDir1 || mario->wStickDir2) {
        mario->mot.roll.flags |= ROLL_FLAG_STICK_UNUSED;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->mot.roll.flags & ROLL_FLAG_FIX) && (mario->buttonTrg & PAD_BUTTON_B) && !(mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING) && mario->hitobjStandOn) {
        mario->currSubMotionId = ROLL_SUB_UNROLL_START;
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
    }
#endif
    if (mario->baseSpeed) {
        zoomOff();
    }
    makeOfs();
}

/**
 * Starts climbing a small step while rolling: Mario rises 4 units a frame for 3 frames (the velocity
 * toward mario->stepUpY is computed first, then overwritten).
 * Called from mario_hit.c when the step ahead is 5-11 units high.
 */
void roll_upstairs_on(void) {
    MarioWork* mario = marioGetPtr();
    mario->flags |= MARIO_FLAG_IS_STEPPING;
    mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioClearJumpPara();
    mario->multiTimer = 3;
    mario->jumpVelocityY = (mario->stepUpY - mario->position.y) / (f32)mario->multiTimer;

    // BUG: overwrites the step-height velocity just computed, so roll_upstairs always climbs 3 x 4 = 12 units.
    mario->jumpVelocityY = 4.0f;
    mario->jumpAccelerationY = -0.42f;
    mario->jumpJerkY = 0.04f;
    mario->jumpSnapY = -0.01f;
}

/** Advances the rolling step climb started by roll_upstairs_on, clearing the step flags when it ends. */
void roll_upstairs(void) {
    MarioWork* mario = marioGetPtr();
    mario->position.y = mario->position.y + mario->jumpVelocityY;
    if (--mario->multiTimer <= 0) {
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->flags &= ~MARIO_FLAG_USE_FORCE_DIRECTION;
        mario->flags &= ~MARIO_FLAG_IS_STEPPING;
    }
}

/** Falling in Tube Mode: drops at up to 10 units a frame and lands on the first floor found. */
void mot_fall_roll(void) {
    MarioWork* mario = marioGetPtr();
    f32 spd = marioGetFallSpd();
    f32 y;
    HitEntry* h;
    if (spd < -10.0f) {
        spd = -10.0f;
    }
    h = marioChkLandon(&y, spd);
    if (h) {
        mario->position.y = y;
        mario->hitobjStandOn = h;
        mario->hitobjJumpFrom = NULL;
    } else {
        mario->position.y += spd;
    }
    if (mario->hitobjStandOn) {
        marioLandOn();
        roll_flyout_flag2 = 0;
    }
}

/**
 * Turns the roll direction dir toward the target direction: a target more than 135 degrees off first flips dir
 * around, the two are brought within 180 degrees of each other on the side chkDirSide gives, and dir then moves
 * 1.2 times the way to the target (overshooting it).
 */
// Original name and size (0x340) from the JP demo map (UNUSED there: every call is inlined).
static f32 makeMovedir(f32 dir, f32 target) {
    MarioWork* mario = marioGetPtr();

    if (chkReverseDir(dir, 270.0f, target) == 1) {
        dir = revise360(180.0f + dir);
    }
    if (chkDirSide(dir, mario->targetYaw) == 0) {
        if (__fabsf(dir - target) > 90.0f) {
            if (target > dir) {
                dir += 360.0f;
            }
        }
    } else {
        if (__fabsf(dir - target) > 90.0f) {
            if (target < dir) {
                target += 360.0f;
            }
        }
    }
    if (chkReverseDir(dir, 180.0f, target) == 0) {
        if (__fabsf(dir - target) > 180.0f) {
            if (dir > target) {
                dir -= 360.0f;
            } else {
                target -= 360.0f;
            }
        }
    }
    dir = 1.2f * (target - dir) + dir;
    dir = revise360(dir);
    return dir;
}

/**
 * Returns 1 when angle x lies outside the arc of range degrees centred on center (center cut to 0.1 degree, the
 * arc wrapping at 360).
 */
// Original name and size (0xe4) from the JP demo map (UNUSED there: every call is inlined). The range parameter is
// a disclosed guess: `range * 0.5f`, `range / 2.0f` and `180.0f - range` (callers 45 / 90) give the same size and
// numbering, and the callers' constants fold.
static s32 chkReverseDir(f32 center, f32 range, f32 x) {
    s32 w = 0;
    s32 r = w;
    f32 c = (f32)(s32)(10.0f * center) / 10.0f;
    f32 lo;
    f32 hi;
    f32 half = range * 0.5f;

    lo = c - half;
    if (lo < 0.0f) {
        w = 1;
        lo += 360.0f;
    }
    hi = c + half;
    if (hi > 360.0f) {
        hi -= 360.0f;
        w = 2;
    }
    if (w == 1) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 2) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 0) {
        if (x < lo || hi < x) {
            r = 1;
        }
    }
    return r;
}

/** chkReverseDir for the control stick's direction against the arc of range degrees around Mario's move direction. */
// Original name and size (0x17c) from the JP demo map (UNUSED there: every call is inlined); `half` declared after
// lo / hi (declared first: 0x178).
static s32 chkReverseDir2(f32 range) {
    s32 w = 0;
    s32 r = w;
    MarioWork* mario = marioGetPtr();
    f32 c = (f32)(s32)(10.0f * mario->moveDirection) / 10.0f;
    f32 x = angleABf(0.0f, 0.0f, (f32)mario->wStickDir1, (f32)mario->wStickDir2);
    f32 lo;
    f32 hi;
    f32 half = range * 0.5f;

    lo = c - half;
    if (lo < 0.0f) {
        w = 1;
        lo += 360.0f;
    }
    hi = c + half;
    if (hi > 360.0f) {
        hi -= 360.0f;
        w = 2;
    }
    if (w == 1) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 2) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 0) {
        if (x < lo || hi < x) {
            r = 1;
        }
    }
    return r;
}

/** 0 when dir lies less than 180 degrees counterclockwise from target (both cut to 0.1 degree), else 1. */
// Original name and size (0x9c) from the JP demo map (UNUSED there: every call is inlined).
static s32 chkDirSide(f32 dir, f32 target) {
    f32 d;
    f32 diff;

    target = (f32)(s32)(10.0f * target) / 10.0f;
    d = (f32)(s32)(10.0f * dir) / 10.0f;
    diff = revise360(d - target);
    return (diff < 180.0f) ? 0 : 1;
}

// FAKE: stand-in that nothing calls (the linker strips it); it only places 270.0f and 360.0f in this object's
// constant pool. JP demo map: this is the slot of the two stripped functions movedir2dispdir (0x118) and rollMove
// (0xc8), compiled right after makeOfs (the last function); 270.0f (@1006) and 360.0f (@1015) are their first new
// constants (chkDirSide's 10.0f follows at @1019, makeMovedir's 135.0f and 1.2f at @1101/@1102). Their bodies are
// not known: no linked code holds an inlined copy.
void marioRollStrippedPool(void) {
    MarioWork* mario = marioGetPtr();
    mario->rot.x = 270.0f;
    mario->rot.y = 360.0f;
}

/**
 * Offsets the tube's display position by the slope of the ground under Mario when the slope is under
 * 45 degrees (the side ray cast for steeper slopes has no effect).
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void makeOfs(void) {
    MarioWork* mario = marioGetPtr();
    f32 t1;
    f32 t2;
    {
        f32 s;
        f32 c;
#if VERSION <= VERSION_DEMO_JP
        f32 ox, oy, oz;
#endif
        f32 dist;
#if VERSION <= VERSION_DEMO_JP
        f32 nx, ny, nz;
#else
        Vec pos;
        Vec dir;
        Vec out;
        Vec nrm;
        Vec d2;
#endif
        dist = 100.0f;
#if VERSION <= VERSION_DEMO_JP
        if (marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y, mario->position.z, 0.0f, -1.0f, 0.0f)) {
            f32 sq = sqrtf(nx * nx + nz * nz);
            f32 ang = angleABf(0.0f, 0.0f, sq, -ny);
#else
        pos = mario->position;
        pos.y = pos.y + mario->wPlayerCollisionBox.y;
        dir = (Vec){0.0f, -1.0f, 0.0f};
        if (marioHitCheckVec(&pos, &dir, &out, &nrm, &dist)) {
            f32 sq = sqrtf(nrm.x * nrm.x + nrm.z * nrm.z);
            f32 ang = angleABf(0.0f, 0.0f, sq, -nrm.y);
#endif
            f32 r = (f32)(s32)(0.5f + ang);
            if (fabs(ang) < 45.0) {
                s = sind(r);
                c = cosd(r);
                t1 = s * 5.0f;
                t2 = 5.0f * c;
                mario->dispOfs.x = 6.0f + t1;
                mario->dispOfs.y = -4.0f + t2;
            } else {
                sincosf(&s, &c, 90.0f);
                dist = 100.0f;
#if VERSION <= VERSION_DEMO_JP
                marioHitCheck(&ox, &oy, &oz, &dist, &nx, &ny, &nz, mario->position.x, mario->position.y, mario->position.z, s, 0.0f, c);
#else
                pos = mario->position;
                d2 = (Vec){0.0f, 0.0f, 0.0f};
                d2.x = s;
                d2.z = c;
                dir = d2;
                marioHitCheckVec(&pos, &dir, &out, &nrm, &dist);
#endif
            }
        }
    }
}
