#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mot/mot_slit.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>

#ifdef VERSION_G8ME01
/* the pouch header declares these with other return types; this file uses the s32 forms */
#define pouchGetHP pouchGetHP_hdr
#define pouchEquipCheckBadge pouchEquipCheckBadge_hdr
#include "mario/mariost.h"
#include "mario/mario_sbr.h"
#undef pouchGetHP
#undef pouchEquipCheckBadge
#endif
s32 pouchGetHP(void);
s32 pouchEquipCheckBadge(s32 badgeId);
void kpa_stay(void);
void peach_stay(void);
void marioClearJumpPara(void);
void marioChgMot2(MarioMotion);
void motSlitCancel(void);
BOOL marioChkJabara(void);

static inline BOOL lowHP(void) {
	BOOL r = pouchGetHP() <= 5;
	return r;
}


void marioVoiceGlareOn(void) {
	MarioWork* mario = marioGetPtr();
	if (mario->soundId[1] == (u32)-1) {
		mario->soundId[1] = psndSFXOn_3D((char*)0xDC, &mario->position);
	}
}

void marioVoiceGlareOff(void) {
	MarioWork* mario = marioGetPtr();
	if (mario->soundId[1] != (u32)-1) {
		psndSFXOff(mario->soundId[1]);
		mario->soundId[1] = -1;
	}
}

BOOL marioChkDeepSleep(void) {
	BOOL result = FALSE;
	MarioWork* mario = marioGetPtr();
	if (mario->currMotionId == MARIO_MOTION_STAY && MARIO_STAY(mario).deepSleepTimer >= sysMsec2Frame(10000)) {
		result = TRUE;
	}
	return result;
}

#ifdef VERSION_G8ME01
BOOL marioPriCheckJabara(void);
s32 marioGetHammerLevel(void);
#endif
void mot_stay(void) {
	MarioWork* mario;
	MarioWork* m;
	BOOL newMot;
	s32 frames;
	BOOL held;

	mario = marioGetPtr();
	newMot = FALSE;
	if (mario->characterId == 2) {
		kpa_stay();
		return;
	}
	if (mario->characterId == 1) {
		peach_stay();
		return;
	}
#ifdef VERSION_G8ME01
	if (!(mario->buttonHold & 0x100)) {
		mario->trigFlags &= ~0x200000;
		mario->trigFlags &= ~0x400000;
	}
#endif
	if (mario->trigFlags & 1) {
		mario->trigFlags &= ~1;
		mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING);
		marioClearJumpPara();
		newMot = TRUE;
		mario->airTimer = 0;
		mario->multiTimer = 0;
		mario->currSubMotionId = 0;
#ifdef VERSION_G8ME01
		if (marioPriCheckJabara()) {
			mario->wMotionTimer = (mario->trigFlags & 0x100000) ? 10 : 0;
		} else {
			mario->wMotionTimer = 0;
		}
		mario->trigFlags &= ~0x100000;
#else
		mario->wMotionTimer = 0;
#endif
		mario->lastGroundPos = mario->position;
		MARIO_STAY(mario).jabaraWait = 0;
		MARIO_STAY(mario).holdTimer = 0;
		MARIO_STAY(mario).deepSleepTimer = 0;
		mario->idleKeyTimer = 0;
		marioSoundInit();
		marioResetCamFollowRate();
		if (mario->flags & MARIO_FLAG_IS_8_BIT) {
			marioChgPose("M_S_1");
			if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
				mario->baseSpeed = 0.0f;
			}
			mario->flags &= ~MARIO_FLAG_IS_PUSHING;
		} else {
			if (mario->flags & MARIO_FLAG_PAPER_MODE) {
				motSlitContinue();
			} else if (mario->dispFlags & 0x1000000) {
				marioChgMot2(MARIO_MOTION_SLIT);
				motSlitCancel();
			}
			if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
				if (pouchGetHP() == 0) {
					marioChgPose("M_I_S");
					psndSFXOn_3D((char*)0x173, &mario->position);
				} else if (lowHP() && marioChkKey()) {
					mario->dispFlags |= 4;
					marioChgPose("M_S_3");
					mario->idleTimer = 0;
				} else if (strcmp(mario->animName, "M_I_Y") != 0) {
					marioChgPose("M_S_1");
				}
			} else {
				marioChgPoseTime("M_U_2", 10);
			}
		}
		if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
			mario->baseSpeed = 0.0f;
		}
		mario->flags &= ~MARIO_FLAG_IS_PUSHING;
	}
	if (mario->flags & MARIO_FLAG_IS_8_BIT) {
		marioChkJump();
		if (mario->controlStickSensitivity > 0.0f) {
			if ((s8)mario->wStickDir1 * (s8)mario->wStickDir1 + (s8)mario->wStickDir2 * (s8)mario->wStickDir2 > 0xBD1) {
				if (pouchEquipCheckBadge(0x143) == 0) {
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
	frames = sysMsec2Frame(1000);
	if (++mario->idleKeyTimer > frames && !(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && marioChkKey()) {
		if (lowHP()) {
			if (strcmp(mario->animName, "M_S_3") != 0) {
				mario->idleTimer = 0;
			}
			mario->dispFlags |= 4;
			marioChgPose("M_S_3");
		} else if ((s32)mario->currSubMotionId == 0 && mario->idleKeyTimer != 0) {
			mario->idleKeyTimer = 0;
			if (strcmp(mario->animName, "M_I_Y") != 0) {
				marioChgPose("M_S_1");
			}
		}
	}
	if (strcmp(mario->animName, "M_S_3") == 0 && marioChkKey()) {
#ifdef VERSION_G8ME01
		m = marioGetPtr();
		m->idleTimer = m->idleTimer + 1;
		if (m->idleTimer >= gp->framerate * 4) {
			m->idleTimer = 0;
			m->poseTime = 0;
		}
		if (m->idleTimer == 0) {
			m->soundId[0] = psndSFXOn_3D((char*)0xE1, &m->position);
		}
		if (m->idleTimer == gp->framerate * 2) {
			m->soundId[0] = psndSFXOn_3D((char*)0xE2, &m->position);
		}
#else
		m = marioGetPtr();
		m->idleTimer = m->idleTimer + 1;
		if (m->idleTimer > 0xF0) {
			m->idleTimer = 0;
		}
		if (m->idleTimer == 0xF) {
			m->soundId[0] = psndSFXOn_3D((char*)0xE1, &m->position);
		}
		if (m->idleTimer == 0x87) {
			m->soundId[0] = psndSFXOn_3D((char*)0xE2, &m->position);
		}
#endif
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
#ifdef VERSION_G8ME01
	if (marioGetHammerLevel() < 2) {
		if (mario->buttonTrg & 0x200) {
			return;
		}
	} else if (mario->buttonHold & 0x200) {
		return;
	}
#else
	if (mario->buttonHold & 0x200) {
		return;
	}
#endif
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
#ifdef VERSION_G8ME01
	held = mario->controlStickSensitivity && !(mario->trigFlags & 0x400000);
	if (held) {
#else
	held = FALSE;
	if (mario->buttonHold & 0x100) {
		mario->mot.stay.holdTimer = mario->mot.stay.holdTimer + 1;
	} else {
		mario->mot.stay.holdTimer = 0;
	}
	if (mario->buttonHold & 0x100) {
		if (mario->mot.stay.holdTimer < 10) {
			held = FALSE;
		} else {
			held = TRUE;
		}
	}
	if (mario->wMotionTimer == 0 && mario->controlStickSensitivity > 0.0f && held == FALSE) {
#endif
		if ((s8)mario->wStickDir1 * (s8)mario->wStickDir1 + (s8)mario->wStickDir2 * (s8)mario->wStickDir2 > 0xBD1) {
			if (pouchEquipCheckBadge(0x143) == 0) {
				marioChgMot(MARIO_MOTION_DASH);
			} else {
				marioChgMot(MARIO_MOTION_WALK);
			}
		} else {
			marioChgMot(MARIO_MOTION_WALK);
		}
	}
	if ((mario->flags & MARIO_FLAG_KEY_OFF) || (mario->dispFlags & 0x40000000)) {
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
#ifdef VERSION_G8ME01
				marioChgEvtPose("M_N_5A\0");
				mario->poseTime = 0;
				mario->multiTimer = 0;
#else
				marioChgEvtPose("M_N_5A\0");
				mario->multiTimer = 0;
#endif
				mario->currSubMotionId = 4;
				MARIO_STAY(mario).deepSleepTimer = 0;
				m = marioGetPtr();
				m->soundId[0] = psndSFXOn_3D((char*)0xE4, &m->position);
			}
			break;
		case 4:
			if (++MARIO_STAY(mario).deepSleepTimer >= sysMsec2Frame(10000)) {
				MARIO_STAY(mario).deepSleepTimer = sysMsec2Frame(10000);
			}
#ifdef VERSION_G8ME01
			if ((s32)++mario->multiTimer >= gp->framerate * 2) {
#else
			if ((s32)++mario->multiTimer >= 0x8C) {
#endif
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

void marioChgMotAuto(void) {
	MarioWork* mario = marioGetPtr();
	s32 a = (u8)mario->wStickDir1;
	if ((s8)a == 0 && (s8)mario->wStickDir2 == 0) {
		marioChgMot(MARIO_MOTION_STAY);
	} else if ((s8)a * (s8)a + (s8)mario->wStickDir2 * (s8)mario->wStickDir2 > 0xBD1) {
		if (pouchEquipCheckBadge(0x143) == 0) {
			marioChgMot(MARIO_MOTION_DASH);
		} else {
			marioChgMot(MARIO_MOTION_WALK);
		}
	} else {
		marioChgMot(MARIO_MOTION_WALK);
	}
}

void mot_stay_post(void) {
	MarioWork* mario;
	MarioMotion id = marioGetPtr()->currMotionId;
	if (id != MARIO_MOTION_HAMMER_2 && id != MARIO_MOTION_JABARA) {
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
	} else {
		mario = marioGetPtr();
		if (mario->soundId[0] != (u32)-1) {
			psndSFXOff(mario->soundId[0]);
			mario->soundId[0] = -1;
		}
	}
}
