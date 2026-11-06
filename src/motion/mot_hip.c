// Mario's spin jump / hip drop (mot_hip): the spin, the drop and the ground pound.

#include "version.h"
#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "manager/evtmgr.h"
#include "system.h"
#include "evt/evt_cam.h"
#include "evt/evt_cmd.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "mario/mario_hit.h"
#include "mot/mot_damage.h"
#include "koopa_motion.h"
#include "mot/mot_hip.h"


// Ids of the two hip-drop sounds (the drop sound and Mario's voice): US and later keep them in
// MarioWork.hipSfxId, JP and the early demos in the hip motion work.
#if VERSION >= VERSION_US
#define HIP_SFX_DROP (mario->hipSfxId[0])
#define HIP_SFX_VOICE (mario->hipSfxId[1])
#else
#define HIP_SFX_DROP (mario->mot.hip.sfxId[0])
#define HIP_SFX_VOICE (mario->mot.hip.sfxId[1])
#endif

/** Vertical camera shake (0.0195 for 110 ms) when Mario's spin jump lands on the ground. */
static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 110)
    RETURN
EVT_END()

/** The half-strength shake (0.0098 for 110 ms) used instead of quake_evt in background mode. */
static EVT_BEGIN(quake_bgmode_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 110)
    RETURN
EVT_END()

s32 chkground(void);

/**
 * TRUE when the spin jump should start: Mario is past the jump peak and reached it at least 10 frames into the jump,
 * A was just pressed and he has the Super Boots or better. Checked from the jump motions in mario_motion.c.
 */
BOOL marioChkHipAttack(void) {
    BOOL ret;
    MarioWork* mario;

    ret = FALSE;
    mario = marioGetPtr();
    if (mario->jumpPeakAirTime >= 10 && (mario->buttonTrg & PAD_BUTTON_A) && pouchGetJumpLv() >= 2) {
        ret = TRUE;
    }
    return ret;
}

/**
 * TRUE while Mario is in the landing phase of the spin jump (MARIO_MOTION_HIP_2);
 * mobjdrv uses it to trigger spin-jump switches and blocks he lands on.
 */
BOOL marioChkHipBump(void) {
    BOOL ret = FALSE;

    if (marioGetPtr()->currMotionId == MARIO_MOTION_HIP_2) {
        ret = TRUE;
    }
    return ret;
}

/** Starts the landing camera shake: quake_evt, or the half-strength quake_bgmode_evt in background mode. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void quakeOn(void) {
#if VERSION <= VERSION_DEMO_JP
    evtEntry(marioBgmodeChk() == 0 ? quake_evt : quake_bgmode_evt, 0, 0);
#else
    evtEntryType(marioBgmodeChk() == 0 ? quake_evt : quake_bgmode_evt, 0, 0, 0);
#endif
}

// The hip-drop sounds. The US and later code looks MarioWork up again inside these helpers (its build has the extra
// marioGetPtr calls) and starts each sound only when it is not already playing.
static inline void hip_snd_reset(MarioWork* mario) {
#if VERSION >= VERSION_US
    mario = marioGetPtr();
#endif
    HIP_SFX_DROP = -1;
    HIP_SFX_VOICE = -1;
}

static inline void hip_snd_on(MarioWork* mario) {
#if VERSION >= VERSION_US
    mario = marioGetPtr();
    if ((u32)HIP_SFX_DROP == 0xFFFFFFFFU) {
        HIP_SFX_DROP = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HIP_DROP1), &mario->position);
    }
    if ((u32)HIP_SFX_VOICE == 0xFFFFFFFFU) {
        HIP_SFX_VOICE = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_POWER1), &mario->position);
    }
#else
    HIP_SFX_DROP = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HIP_DROP1), &mario->position);
    HIP_SFX_VOICE = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_POWER1), &mario->position);
#endif
}

static inline void hip_snd_off(MarioWork* mario) {
#if VERSION >= VERSION_US
    mario = marioGetPtr();
#endif
    if ((u32)HIP_SFX_DROP != 0xFFFFFFFFU) {
        psndSFXOff(HIP_SFX_DROP);
        HIP_SFX_DROP = -1;
    }
    if ((u32)HIP_SFX_VOICE != 0xFFFFFFFFU) {
        psndSFXOff(HIP_SFX_VOICE);
        HIP_SFX_VOICE = -1;
    }
}

/**
 * Spin jump (Super Boots): Mario hops up, spins in place with the looping sounds, then drops straight down;
 * on landing it switches to MARIO_MOTION_HIP_2, shakes the camera and stands,
 * or goes to the water or spike damage motion when the floor has those attributes. Bowser runs kpa_hip instead.
 */
void mot_hip(void) {
    MarioWork* mario;
    HitEntry* hit;
    f32 fall;
    f32 y;
    f32 tmp1;
    f32 tmp2;
    f32 d;
    u32 attr;
    s32 f;

    mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_hip();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->flags |= MARIO_FLAG_IS_FALLING;
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioAdjustMoveDir();
        f = 0;
        if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
            f = 1;
        }
        mario->moveDirection = toMovedir(f ? 180.0f : 0.0f);
        mario->directionView = mario->moveDirection;
        marioChgPose("M_A_1A");
        mario->jumpVelocityY = 5.2f;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        d = -2.0f * marioGetScale() / 2.0f;
        mario->position.y += d;
        d = 14.0f * marioGetScale() / 2.0f;
        mario->rotOfs.y += d;
        mario->dispOfs2.y = 0.0f;
        mario->currSubMotionId = 0;
        hip_snd_reset(mario);
    }
    switch (mario->currSubMotionId) {
    case 0:
        hip_snd_on(mario);
        mario->mot.hip.angle = 0.0f;
        mario->baseSpeed = 0.0f;
        mario->currSubMotionId = 1;
    case 1:
        if ((mario->mot.hip.angle += 20.0f) >= 360.0f) {
            mario->mot.hip.angle = 360.0f;
        }
        mario->jumpVelocityY -= 0.42f;
        if (mario->hitobjHead == NULL) {
            mario->dispOfs2.y += mario->jumpVelocityY;
        }
        if (360.0f == mario->mot.hip.angle && mario->jumpVelocityY <= 0.0f) {
            mario->multiTimer = 5;
            mario->currSubMotionId = 2;
            mario->jumpVelocityY = 2.0f;
        }
        switch (chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(MARIO_MOTION_STAY, 0);
            mot_hip_post();
            return;
        }
        break;
    case 2:
        if (mario->jumpVelocityY >= 0.0f) {
            mario->jumpVelocityY -= 1.4f;
            if (mario->hitobjHead == NULL) {
                mario->dispOfs2.y += mario->jumpVelocityY;
            }
        }
        if (--mario->multiTimer <= 0) {
            mario->currSubMotionId = 10;
        }
        switch (chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(MARIO_MOTION_STAY, 0);
            mot_hip_post();
            return;
        }
        break;
    case 10:
        marioChkLandon(&y, 0.0f);
        if ((mario->mot.hip.angle += 22.5f) >= 360.0f) {
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
            mario->mot.hip.angle = 0.0f;
            mario->currSubMotionId = 20;
            mario->jumpAccelerationY = -3.4744f;
            mario->jumpJerkY = 0.4704f;
            mario->jumpSnapY = -1.1904f;
            mario->jumpVelocityY = mario->jumpAccelerationY;
        }
        switch (chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(MARIO_MOTION_STAY, 0);
            mot_hip_post();
            return;
        }
        break;
    case 20:
        mario->multiTimer = sysMsec2Frame(200);
        mario->currSubMotionId = 21;
    case 21:
        if (--mario->multiTimer > 0) {
            break;
        }
        fall = marioGetFallSpd();
#if VERSION <= VERSION_DEMO_JP
        hit = marioChkLandon(&y, fall);
#else
        hit = marioSearchGround(&y, &tmp1, &tmp2, fall, fall);
#endif
        if (fall < -100.0f) {
            fall = -100.0f;
            mario->jumpSnapY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpAccelerationY = 0.0f;
            mario->jumpVelocityY = fall;
        }
        if (hit != NULL) {
            mario->currMotionId = MARIO_MOTION_HIP_2;
            mario->currSubMotionId = 22;
            mario->multiTimer = 20;
            mario->hitobjStandOn = hit;
            mario->hitobjJumpFrom = NULL;
            mario->position.y = y;
            mario->mot.hip.angle = 0.0f;
            mario->rotOfs.y = 0.0f;
            mario->airTimer = 0;
            mario->jumpPeakAirTime = 0;
#if VERSION >= VERSION_JP
            mario->dispOfs2.y = 0.0f;
#endif
            marioChgPose("M_A_1B");
            attr = hitGetAttr(hit);
            if (attr & HIT_ATTR_WATER_HAZARD) {
                set_damage_root_ypos(mario->position.y);
#if VERSION >= VERSION_JP
                d = -0.4f;
                mario->jumpVelocityY = d;
                mario->jumpAccelerationY = d;
                mario->jumpJerkY = 0.0f;
                mario->jumpSnapY = 0.0f;
#endif
                marioChgMot(MARIO_MOTION_DAMAGE);
                return;
            }
            if (attr & HIT_ATTR_DAMAGE_LIQUID) {
                set_damage_root_ypos(mario->position.y);
                marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
                return;
            }
            if (attr & HIT_ATTR_SPIKES) {
                set_damage_root_ypos(mario->position.y);
                marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
                return;
            }
#if VERSION <= VERSION_DEMO_JP
            psndSFXOn_3D((char*)PSND_SFX_MARIO_HIO_DROP2, &mario->position);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HIO_DROP2), &mario->position);
#endif
            quakeOn();
        } else {
            mario->position.y += fall;
        }
        if (mario->screenY > 400 && !marioBgmodeChk()) {
            camFollowYOn();
        }
        break;
    case 22:
        if (chkground() == 1) {
            return;
        }
        if (--mario->multiTimer <= 0) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_BLUR;
            if (!marioBgmodeChk()) {
                camFollowYOn();
            }
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
            mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
            marioChgMotSub(MARIO_MOTION_STAY, 0);
            mot_hip_post();
        }
        break;
    case 50:
        marioChgPose("M_A_1A");
        mario->jumpVelocityY = 0.0f;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        mario->mot.hip.angle += (f32)keyGetSubStickY(2) / 10.0f;
        break;
    }
    mario->rot.z = mario->mot.hip.angle;
    if (mario->airTimer != 0) {
        mario->floorMove.x = 0.0f;
        mario->floorMove.z = 0.0f;
        mario->floorMove.y = 0.0f;
    }
}

#if VERSION >= VERSION_JP
// FAKE: dont_inline keeps chkground a call while mot_hip_post still inlines
#pragma dont_inline on
#endif

/**
 * Landing check for the spin jump: 0 while in the air,
 * 1 when the floor sent Mario into the water (0x200) or spike (0xC00) damage motion, 2 for a normal landing.
 * Also resets the spin rotation and turns camera y-follow back on (not in background mode in the PAL demo and PAL).
 */
s32 chkground(void) {
    MarioWork* mario;
    HitEntry* hit;
    u32 attr;
    f32 y;
    f32 v;

    mario = marioGetPtr();
    hit = marioChkLandon(&y, 1.0f);
    if (hit != NULL) {
        mario->hitobjStandOn = hit;
        mario->hitobjJumpFrom = NULL;
        mario->position.y = y;
        mario->mot.hip.angle = 0.0f;
        mario->rotOfs.y = 0.0f;
        mario->dispOfs2.y = 0.0f;
        mario->airTimer = 0;
        mario->jumpPeakAirTime = 0;
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
#if VERSION >= VERSION_DEMO_EU
        if (!marioBgmodeChk()) {
            camFollowYOn();
        }
#else
        camFollowYOn();
#endif
        attr = hitGetAttr(hit);
        if (attr & HIT_ATTR_WATER_HAZARD) {
            set_damage_root_ypos(mario->position.y);
#if VERSION >= VERSION_JP
            v = -0.4f;
            mario->jumpVelocityY = v;
            mario->jumpAccelerationY = v;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
#endif
            marioChgMot(MARIO_MOTION_DAMAGE);
            return 1;
        }
        if (attr & (HIT_ATTR_SPIKES | HIT_ATTR_DAMAGE_LIQUID)) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
            return 1;
        }
        return 2;
    }
    return 0;
}

#pragma dont_inline reset

/** Cleanup when the spin jump motion ends: resets the spin offsets and air timers and stops the looping sounds. */
void mot_hip_post(void) {
    MarioWork* mario;

    mario = marioGetPtr();
    marioOfsRotReset();
    mario->airTimer = 0;
    mario->jumpPeakAirTime = 0;
    hip_snd_off(mario);
}
