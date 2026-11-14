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

/** Stops the idle sound (mario->soundId[0]) and the charge-up voice (soundId[1]) if they are playing. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void stopVoice(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->soundId[0] != (u32)-1) {
        psndSFXOff(mario->soundId[0]);
        mario->soundId[0] = -1;
    }
    mario = marioGetPtr();
    if (mario->soundId[1] != (u32)-1) {
        psndSFXOff(mario->soundId[1]);
        mario->soundId[1] = -1;
    }
}

/** Mario's yawn (SFX_VOICE_MARIO_YAWN1) in mario->soundId[0] as he gets sleepy in the idle animation. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void yawnVoice(void) {
    MarioWork* m = marioGetPtr();
    m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_YAWN1), &m->position);
}

/** Mario's snore (SFX_VOICE_MARIO_SLEEP1) in mario->soundId[0] while he naps. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void snoreVoice(void) {
    MarioWork* m = marioGetPtr();
    m->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_SLEEP1), &m->position);
}

/**
 * Mario's tired breathing in the low-HP pose (M_S_3): counts mario->idleTimer and plays SFX_VOICE_MARIO_RELIEF1
 * and RELIEF2 at two points of each breath (US and later time them from the frame rate).
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void weakVoice(void) {
    MarioWork* m = marioGetPtr();
    m->idleTimer = m->idleTimer + 1;
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

/** TRUE when Mario is at 5 HP or less, which switches his idle to the tired pose (M_S_3) and breathing sounds. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
BOOL chkWeak(void) {
    BOOL r = pouchGetHP() <= MARIO_DANGER_HP;
    return r;
}

/**
 * Standing motion: idle pose (tired at low HP, with breathing sounds), turning into Paper Mode,
 * starting the spin hammer, Spring Jump, jumps, items and partner actions,
 * and walking or dashing when the stick moves.
 * After 50 seconds without input Mario yawns and then naps (M_N_5A, snoring).
 * Bowser and Peach run their own versions.
 */
void mot_stay(void) {
    MarioWork* mario;
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
#if VERSION >= VERSION_US
    if (!(mario->buttonHold & PAD_BUTTON_A)) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_MOVE_CHK;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_START_JABARA_SPIN;
    }
#endif
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING);
        marioClearJumpPara();
        newMot = TRUE;
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
#if VERSION <= VERSION_DEMO_JP
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
#if VERSION <= VERSION_DEMO_JP
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
            motSlitContinue();
        } else if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
            marioChgMot2(MARIO_MOTION_SLIT);
            motSlitCancel();
        }
        if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
            if (pouchGetHP() == 0) {
                marioChgPose("M_I_S");
                psndSFXOn_3D((char*)PSND_SFX_MARIO_FURAFURA1, &mario->position);
            } else if (chkWeak() && marioChkKey()) {
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
                } else if (chkWeak() && marioChkKey()) {
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
    }
#if VERSION >= VERSION_JP
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
    }
#endif
    frames = sysMsec2Frame(1000);
    if (++mario->idleKeyTimer > frames && !(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && marioChkKey()) {
        if (chkWeak()) {
            if (strcmp(mario->animName, "M_S_3") != 0) {
                mario->idleTimer = 0;
            }
            mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
            marioChgPose("M_S_3");
        } else if (mario->currSubMotionId == 0 && mario->idleKeyTimer != 0) {
            mario->idleKeyTimer = 0;
            if (strcmp(mario->animName, "M_I_Y") != 0) {
                marioChgPose("M_S_1");
            }
        }
    }
    if (strcmp(mario->animName, "M_S_3") == 0 && marioChkKey()) {
        weakVoice();
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
        if (mario->currSubMotionId > 0 && strcmp(mario->animName, "M_S_1") != 0 &&
            strcmp(mario->animName, "M_S_1R") != 0) {
            marioChgPose("M_S_1");
            mario->currSubMotionId = 0;
        }
        return;
    }
    if (chkWeak()) {
        return;
    }
    switch (mario->currSubMotionId) {
        case 0:
            if ((s32)++mario->multiTimer >= sysMsec2Frame(50000)) {
                mario->currSubMotionId = 1;
            }
            break;
        case 1:
            marioChgEvtPose("M_I_5");
            marioAdjustMoveDir();
            yawnVoice();
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
                snoreVoice();
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
                snoreVoice();
            }
            if ((mario->flags & MARIO_FLAG_KEY_OFF) || chkWeak()) {
                marioChgPose("M_S_1");
                mario->currSubMotionId = 0;
            }
            break;
    }
}

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

/**
 * Cleanup when the stay motion ends: stops the idle sound and the charge-up voice
 * (JP and later keep the voice when the next motion is the spin hammer or the Spring Jump).
 */
void mot_stay_post(void) {
    MarioWork* mario;
#if VERSION >= VERSION_JP
    MarioMotion id = marioGetPtr()->currMotionId;
    if (id != MARIO_MOTION_HAMMER_2 && id != MARIO_MOTION_JABARA) {
        stopVoice();
    } else {
        mario = marioGetPtr();
        if (mario->soundId[0] != (u32)-1) {
            psndSFXOff(mario->soundId[0]);
            mario->soundId[0] = -1;
        }
    }
#else
    stopVoice();
#endif
}
