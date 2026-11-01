// Mario's standing and idle motions: idle poses, low-HP and sleep poses, and the automatic motion changes.

#include "version.h"
#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mot/mot_slit.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "koopa_motion.h"
#include "peach.h"
#include "mario/mario_pouch.h"
#include "mot/mot_jabara.h"
#include "mot/mot_hammer.h"

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif
#include "mario/mario_sbr.h"

/** TRUE when Mario is at 5 HP or less, which switches his idle to the tired pose (M_S_3) and breathing sounds. */
static inline BOOL lowHP(void) {
    // FAKE: the result goes through a spare temporary; return pouchGetHP() <= 5 changes the object
    BOOL r = pouchGetHP() <= 5;
    return r;
}


/**
 * Starts Mario's charge-up voice (sound 0xDC) in mario->soundId[1] if it is not already playing;
 * used while he glares holding B for the spin hammer or A for the Spring Jump.
 */
void marioVoiceGlareOn(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->soundId[1] == (u32)-1) {
        mario->soundId[1] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_POWER2), &mario->position);
    }
}

/** Stops the charge-up voice started by marioVoiceGlareOn. */
void marioVoiceGlareOff(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->soundId[1] != (u32)-1) {
        psndSFXOff(mario->soundId[1]);
        mario->soundId[1] = -1;
    }
}

/**
 * TRUE once Mario has been asleep in the idle animation for 10 seconds;
 * the partners then fall asleep too (party_motion.c).
 */
BOOL marioChkDeepSleep(void) {
    BOOL result = FALSE;
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_STAY && MARIO_STAY(mario).deepSleepTimer >= sysMsec2Frame(10000)) {
        result = TRUE;
    }
    return result;
}

/**
 * Standing motion: idle pose (tired at low HP, with breathing sounds), turning into Paper Mode,
 * starting the spin hammer, Spring Jump, jumps, items and partner actions,
 * and walking or dashing when the stick moves.
 * After 50 seconds without input Mario yawns and then naps (M_N_5A, snoring).
 * Bowser and Peach run their own versions.
 */
/**
 * Standing motion: idle pose (tired at low HP, with breathing sounds), turning into Paper Mode,
 * starting the spin hammer, Spring Jump, jumps, items and partner actions,
 * and walking or dashing when the stick moves.
 * After 50 seconds without input Mario yawns and then naps (M_N_5A, snoring).
 * Bowser and Peach run their own versions.
 */
void mot_stay(void) {
    MarioWork* mario;
    MarioWork* m;
    BOOL newMot;
    s32 frames;
    BOOL held;

    mario = marioGetPtr();
    newMot = FALSE;
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_stay();
        return;
    }
    if (mario->characterId == CHARACTER_PEACH) {
        peach_stay();
        return;
    }
#if VERSION > VERSION_DEMO_US
#if VERSION >= VERSION_US
    if (!(mario->buttonHold & PAD_BUTTON_A)) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_MOVE_CHK;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_START_JABARA_SPIN;
    }
#endif
#endif
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING);
        marioClearJumpPara();
        newMot = TRUE;
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
#if VERSION <= VERSION_DEMO_US
        mario->lastGroundPos = mario->position;
        mario->stay.jabaraWait = 0;
        mario->stay.holdTimer = 0;
        mario->stay.deepSleepTimer = 0;
#else
#if VERSION >= VERSION_US
        if (marioPriCheckJabara()) {
            mario->wMotionTimer = (mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK) ? 10 : 0;
        } else {
            mario->wMotionTimer = 0;
        }
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
#else
        mario->wMotionTimer = 0;
#endif
        mario->lastGroundPos = mario->position;
        MARIO_STAY(mario).jabaraWait = 0;
        MARIO_STAY(mario).holdTimer = 0;
        MARIO_STAY(mario).deepSleepTimer = 0;
#endif
        mario->idleKeyTimer = 0;
        marioSoundInit();
        marioResetCamFollowRate();
#if VERSION <= VERSION_DEMO_US
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
                motSlitContinue();
            } else if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
                marioChgMot2(MARIO_MOTION_SLIT);
                motSlitCancel();
            }
            if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
                if (pouchGetHP() == 0) {
                    marioChgPose("M_I_S");
                    psndSFXOn_3D((char*)0x173, &mario->position);
                } else if (lowHP() && marioChkKey()) {
                    mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
                    marioChgPose("M_S_3");
                    mario->idleTimer = 0;
                } else if (strcmp(mario->animName, "M_I_Y") != 0) {
                    marioChgPose("M_S_1");
                }
            } else {
                marioChgPoseTime("M_U_2", 10);
            }
#else
        if (mario->flags & MARIO_FLAG_IS_8_BIT) {
            marioChgPose("M_S_1");
            if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                mario->baseSpeed = 0.0f;
            }
            mario->flags &= ~MARIO_FLAG_IS_PUSHING;
        } else {
            if (mario->flags & MARIO_FLAG_PAPER_MODE) {
                motSlitContinue();
            } else if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
                marioChgMot2(MARIO_MOTION_SLIT);
                motSlitCancel();
            }
            if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
                if (pouchGetHP() == 0) {
                    marioChgPose("M_I_S");
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FURAFURA1), &mario->position);
                } else if (lowHP() && marioChkKey()) {
                    mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
                    marioChgPose("M_S_3");
                    mario->idleTimer = 0;
                } else if (strcmp(mario->animName, "M_I_Y") != 0) {
                    marioChgPose("M_S_1");
                }
            } else {
                marioChgPoseTime("M_U_2", 10);
            }
        }
#endif
        if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mario->baseSpeed = 0.0f;
        }
        mario->flags &= ~MARIO_FLAG_IS_PUSHING;
#if VERSION > VERSION_DEMO_US
    }
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        marioChkJump();
        if (mario->controlStickSensitivity > 0.0f) {
            if (mario->wStickDir1 * mario->wStickDir1 + mario->wStickDir2 * mario->wStickDir2 > 0xBD1) {
                if (pouchEquipCheckBadge(ITEM_SLOW_GO) == 0) {
                    marioChgMot(MARIO_MOTION_DASH);
                } else {
                    marioChgMot(MARIO_MOTION_WALK);
                }
            } else {
                marioChgMot(MARIO_MOTION_WALK);
            }
        }
        return;
#endif
    }
    frames = sysMsec2Frame(1000);
    if (++mario->idleKeyTimer > frames && !(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && marioChkKey()) {
        if (lowHP()) {
            if (strcmp(mario->animName, "M_S_3") != 0) {
                mario->idleTimer = 0;
            }
            mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
            marioChgPose("M_S_3");
        } else if ((s32)mario->currSubMotionId == 0 && mario->idleKeyTimer != 0) {
            mario->idleKeyTimer = 0;
            if (strcmp(mario->animName, "M_I_Y") != 0) {
                marioChgPose("M_S_1");
            }
        }
    }
    if (strcmp(mario->animName, "M_S_3") == 0 && marioChkKey()) {
        m = marioGetPtr();
        m->idleTimer = m->idleTimer + 1;
#if VERSION <= VERSION_DEMO_US
        if (m->idleTimer > 240) {
            m->idleTimer = 0;
        }
        if (m->idleTimer == 15) {
            m->soundId[0] = psndSFXOn_3D((char*)0xE1, &m->position);
        }
        if (m->idleTimer == 135) {
            m->soundId[0] = psndSFXOn_3D((char*)0xE2, &m->position);
        }
}
    if (mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) {
        if (mario->controlStickSensitivity > 0.0f) {
            marioChgMot(MARIO_MOTION_WALK);
        }
        return;
    }
    if (!newMot && marioChkItemMotion()) {
        return;
    }
    if (mario->buttonHold & PAD_BUTTON_B) {
        return;
    }
    if (marioSlitAbilityChk() && marioSlitButton() == 1) {
        marioChgMot(MARIO_MOTION_SLIT);
        return;
    }
    if (++mario->stay.jabaraWait >= 10) {
        mario->stay.jabaraWait = 10;
        if (marioChkJabara()) {
            marioChgMot(MARIO_MOTION_JABARA);
            return;
        }
    }
    marioChkJump();
    marioChkTransform();
    held = FALSE;
    if (mario->buttonHold & PAD_BUTTON_A) {
        mario->stay.holdTimer = mario->stay.holdTimer + 1;
    } else {
        mario->stay.holdTimer = 0;
    }
    if (mario->buttonHold & PAD_BUTTON_A) {
        if (mario->stay.holdTimer < 10) {
            held = FALSE;
        } else {
            held = TRUE;
        }
    }
    if (mario->wMotionTimer == 0 && mario->controlStickSensitivity > 0.0f && held == FALSE) {
        if ((s8)mario->wStickDir1 * (s8)mario->wStickDir1 + (s8)mario->wStickDir2 * (s8)mario->wStickDir2 > 0xBD1) {
            if (pouchEquipCheckBadge(ITEM_SLOW_GO) == 0) {
                marioChgMot(MARIO_MOTION_DASH);
            } else {
                marioChgMot(MARIO_MOTION_WALK);
            }
        } else {
            marioChgMot(MARIO_MOTION_WALK);
        }
    }
    if ((mario->flags & MARIO_FLAG_KEY_OFF) || (mario->dispFlags & MARIO_DISP_FLAG_PAPER)) {
        if ((s32)mario->currSubMotionId > 0 && strcmp(mario->animName, "M_S_1") != 0 &&
            strcmp(mario->animName, "M_S_1R") != 0) {
            marioChgPose("M_S_1");
            mario->currSubMotionId = 0;
        }
        return;
    }
    if (lowHP()) {
        return;
    }
    switch ((s32)mario->currSubMotionId) {
        case 0:
            if ((s32)++mario->multiTimer >= sysMsec2Frame(50000)) {
                mario->currSubMotionId = 1;
            }
            break;
        case 1:
            marioChgEvtPose("M_I_5");
            marioAdjustMoveDir();
            m = marioGetPtr();
            m->soundId[0] = psndSFXOn_3D((char*)0xEF, &m->position);
            mario->multiTimer = 0x32;
            mario->currSubMotionId = 2;
        case 2:
            if ((s32)--mario->multiTimer < 0) {
                marioChgPose("M_S_1");
                mario->multiTimer = 100;
                mario->currSubMotionId = 3;
            }
            break;
        case 3:
            if ((s32)--mario->multiTimer < 0) {
                marioChgEvtPose("M_N_5A");
                mario->multiTimer = 0;
                mario->currSubMotionId = 4;
                mario->stay.deepSleepTimer = 0;
                m = marioGetPtr();
                m->soundId[0] = psndSFXOn_3D((char*)0xE4, &m->position);
            }
            break;
        case 4:
            if (++mario->stay.deepSleepTimer >= sysMsec2Frame(10000)) {
                mario->stay.deepSleepTimer = sysMsec2Frame(10000);
            }
            if ((s32)++mario->multiTimer >= 140) {
                mario->multiTimer = 0;
                m = marioGetPtr();
                m->soundId[0] = psndSFXOn_3D((char*)0xE4, &m->position);
            }
            if ((mario->flags & MARIO_FLAG_KEY_OFF) || lowHP()) {
                marioChgPose("M_S_1");
                mario->currSubMotionId = 0;
            }
            break;
    }
}
#else
#if VERSION >= VERSION_US
        if (m->idleTimer >= gp->framerate * 4) {
            m->idleTimer = 0;
            m->poseTime = 0;
        }
        if (m->idleTimer == 0) {
            m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_RELIEF1), &m->position);
        }
        if (m->idleTimer == gp->framerate * 2) {
#else
        if (m->idleTimer > 240) {
            m->idleTimer = 0;
        }
        if (m->idleTimer == 15) {
            m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_RELIEF1), &m->position);
        }
        if (m->idleTimer == 135) {
#endif
            m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_RELIEF2), &m->position);
        }
    }
    if (mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) {
        if (mario->controlStickSensitivity > 0.0f) {
            marioChgMot(MARIO_MOTION_WALK);
        }
        return;
    }
    if (!newMot && marioChkItemMotion()) {
        return;
    }
#if VERSION >= VERSION_US
    if (marioGetHammerLevel() < 2) {
        if (mario->buttonTrg & PAD_BUTTON_B) {
            return;
        }
    } else if (mario->buttonHold & PAD_BUTTON_B) {
#else
    if (mario->buttonHold & PAD_BUTTON_B) {
#endif
        return;
    }
    if (marioSlitAbilityChk() && marioSlitButton() == 1) {
        marioChgMot(MARIO_MOTION_SLIT);
        return;
    }
    if (++MARIO_STAY(mario).jabaraWait >= 10) {
        MARIO_STAY(mario).jabaraWait = 10;
        if (marioChkJabara()) {
            marioChgMot(MARIO_MOTION_JABARA);
            return;
        }
    }
    marioChkJump();
    marioChkTransform();
#if VERSION >= VERSION_US
    held = mario->controlStickSensitivity && !(mario->trigFlags & MARIO_TRIG_FLAG_START_JABARA_SPIN);
    if (held) {
#else
    held = FALSE;
    if (mario->buttonHold & PAD_BUTTON_A) {
        mario->mot.stay.holdTimer = mario->mot.stay.holdTimer + 1;
    } else {
        mario->mot.stay.holdTimer = 0;
    }
    if (mario->buttonHold & PAD_BUTTON_A) {
        if (mario->mot.stay.holdTimer < 10) {
            held = FALSE;
        } else {
            held = TRUE;
        }
    }
    if (mario->wMotionTimer == 0 && mario->controlStickSensitivity > 0.0f && held == FALSE) {
#endif
        if (mario->wStickDir1 * mario->wStickDir1 + mario->wStickDir2 * mario->wStickDir2 > 0xBD1) {
            if (pouchEquipCheckBadge(ITEM_SLOW_GO) == 0) {
                marioChgMot(MARIO_MOTION_DASH);
            } else {
                marioChgMot(MARIO_MOTION_WALK);
            }
        } else {
            marioChgMot(MARIO_MOTION_WALK);
        }
    }
    if ((mario->flags & MARIO_FLAG_KEY_OFF) || (mario->dispFlags & MARIO_DISP_FLAG_PAPER)) {
        if ((s32)mario->currSubMotionId > 0 && strcmp(mario->animName, "M_S_1") != 0 &&
            strcmp(mario->animName, "M_S_1R") != 0) {
            marioChgPose("M_S_1");
            mario->currSubMotionId = 0;
        }
        return;
    }
    if (lowHP()) {
        return;
    }
    switch ((s32)mario->currSubMotionId) {
        case 0:
            if ((s32)++mario->multiTimer >= sysMsec2Frame(50000)) {
                mario->currSubMotionId = 1;
            }
            break;
        case 1:
            marioChgEvtPose("M_I_5");
            marioAdjustMoveDir();
            m = marioGetPtr();
            m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_YAWN1), &m->position);
            mario->multiTimer = 0x32;
            mario->currSubMotionId = 2;
        case 2:
            if ((s32)--mario->multiTimer < 0) {
                marioChgPose("M_S_1");
                mario->multiTimer = 100;
                mario->currSubMotionId = 3;
            }
            break;
        case 3:
            if ((s32)--mario->multiTimer < 0) {
                marioChgEvtPose("M_N_5A");
#if VERSION >= VERSION_US
                mario->poseTime = 0;
#endif
                mario->multiTimer = 0;
                mario->currSubMotionId = 4;
                MARIO_STAY(mario).deepSleepTimer = 0;
                m = marioGetPtr();
                m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_SLEEP1), &m->position);
            }
            break;
        case 4:
            if (++MARIO_STAY(mario).deepSleepTimer >= sysMsec2Frame(10000)) {
                MARIO_STAY(mario).deepSleepTimer = sysMsec2Frame(10000);
            }
#if VERSION >= VERSION_US
            if ((s32)++mario->multiTimer >= gp->framerate * 2) {
#else
            if ((s32)++mario->multiTimer >= 140) {
#endif
                mario->multiTimer = 0;
                m = marioGetPtr();
                m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_SLEEP1), &m->position);
            }
            if ((mario->flags & MARIO_FLAG_KEY_OFF) || lowHP()) {
                marioChgPose("M_S_1");
                mario->currSubMotionId = 0;
            }
            break;
    }
}
#endif

/**
 * Picks the ground motion from the stick: stay when centred,
 * dash when pushed far (walk instead with the Slow Go badge), walk otherwise.
 */
void marioChgMotAuto(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->wStickDir1 == 0 && mario->wStickDir2 == 0) {
        marioChgMot(MARIO_MOTION_STAY);
    } else if (mario->wStickDir1 * mario->wStickDir1 + mario->wStickDir2 * mario->wStickDir2 > 0xBD1) {
        if (pouchEquipCheckBadge(ITEM_SLOW_GO) == 0) {
            marioChgMot(MARIO_MOTION_DASH);
        } else {
            marioChgMot(MARIO_MOTION_WALK);
        }
    } else {
        marioChgMot(MARIO_MOTION_WALK);
    }
}

/** Cleanup when the stay motion ends: stops the idle sound and the charge-up voice. */
void mot_stay_post(void) {
    MarioWork* mario;
#if VERSION > VERSION_DEMO_US
    MarioMotion id = marioGetPtr()->currMotionId;
    if (id != MARIO_MOTION_HAMMER_2 && id != MARIO_MOTION_JABARA) {
#endif
        mario = marioGetPtr();
        if (mario->soundId[0] != (u32)-1) {
            psndSFXOff(mario->soundId[0]);
            mario->soundId[0] = -1;
        }
        mario = marioGetPtr();
        if (mario->soundId[1] != (u32)-1) {
            psndSFXOff(mario->soundId[1]);
            mario->soundId[1] = -1;
        }
#if VERSION > VERSION_DEMO_US
    } else {
        mario = marioGetPtr();
        if (mario->soundId[0] != (u32)-1) {
            psndSFXOff(mario->soundId[0]);
            mario->soundId[0] = -1;
#endif
        }
#if VERSION > VERSION_DEMO_US
    }
}
#endif
