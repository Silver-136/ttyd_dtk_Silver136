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

HitEntry* marioChkLandon(f32* outY, f32 y);
HitEntry* marioSearchGround(f32* a, f32* b, f32* c, f32 d, f32 e);
void set_damage_root_ypos(f32 y);
void kpa_hip(void);

#ifdef VERSION_G8ME01
#define SND1 (mario->soundId[3])
#define SND2 (mario->soundId[4])
#else
#define SND1 (mario->mot.hip.sfxId[0])
#define SND2 (mario->mot.hip.sfxId[1])
#endif

static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 110)
    RETURN
EVT_END()

static EVT_BEGIN(quake_bgmode_evt)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 110)
    RETURN
EVT_END()

s32 U_chkground(void);
void mot_hip_post(void);
#ifdef VERSION_G8ME01
#define MHPOST() hip_post_inl()
#else
#define MHPOST() mot_hip_post()
#endif

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

BOOL marioChkHipBump(void) {
    BOOL ret = FALSE;

    if (marioGetPtr()->currMotionId == MARIO_MOTION_HIP_2) {
        ret = TRUE;
    }
    return ret;
}

#ifdef VERSION_G8ME01
static inline void hip_snd_reset(void) {
    MarioWork* mario = marioGetPtr();

    SND1 = -1;
    SND2 = -1;
}

static inline void hip_snd_on(void) {
    MarioWork* mario = marioGetPtr();

    if ((u32)SND1 == 0xFFFFFFFFU) {
        SND1 = psndSFXOn_3D((char*)0x158, &mario->position);
    }
    if ((u32)SND2 == 0xFFFFFFFFU) {
        SND2 = psndSFXOn_3D((char*)0xDB, &mario->position);
    }
}

static inline void hip_snd_off(void) {
    MarioWork* mario = marioGetPtr();

    if ((u32)SND1 != 0xFFFFFFFFU) {
        psndSFXOff(SND1);
        SND1 = -1;
    }
    if ((u32)SND2 != 0xFFFFFFFFU) {
        psndSFXOff(SND2);
        SND2 = -1;
    }
}

static inline void hip_post_inl(void) {
    MarioWork* mario;

    mario = marioGetPtr();
    marioOfsRotReset();
    mario->airTimer = 0;
    mario->jumpPeakAirTime = 0;
    hip_snd_off();
}

#endif
void mot_hip(void) {
    MarioWork* mario;
    HitEntry* hit;
    f32 fall;
    f32 y;
    f32 tmp1;
    f32 tmp2;
    f32 d;
    u32 attr;

    mario = marioGetPtr();
    if (mario->characterId == 2) {
        kpa_hip();
        return;
    }
    if (mario->trigFlags & 1) {
        mario->trigFlags &= ~1;
        mario->flags &= ~0xF0000;
        mario->flags |= 0x20000;
        mario->flags |= 0x80;
        marioAdjustMoveDir();
        {
            s32 f = 0;
            if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
                f = 1;
            }
            mario->moveDirection = toMovedir(f ? 180.0f : 0.0f);
        }
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
#ifdef VERSION_G8ME01
        hip_snd_reset();
#else
        SND1 = -1;
        SND2 = -1;
#endif
    }
    switch (mario->currSubMotionId) {
    case 0:
#ifdef VERSION_G8ME01
        hip_snd_on();
#else
        SND1 = psndSFXOn_3D((char*)0x158, &mario->position);
        SND2 = psndSFXOn_3D((char*)0xDB, &mario->position);
#endif
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
        switch (U_chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(0, 0);
            MHPOST();
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
        switch (U_chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(0, 0);
            MHPOST();
            return;
        }
        break;
    case 10:
        marioChkLandon(&y, 0.0f);
        if ((mario->mot.hip.angle += 22.5f) >= 360.0f) {
            mario->flags &= ~0x10000;
            mario->flags |= 0x20000;
            mario->mot.hip.angle = 0.0f;
            mario->currSubMotionId = 20;
            mario->jumpAccelerationY = -3.4744f;
            mario->jumpJerkY = 0.4704f;
            mario->jumpSnapY = -1.1904f;
            mario->jumpVelocityY = mario->jumpAccelerationY;
        }
        switch (U_chkground()) {
        case 1:
            return;
        case 2:
            marioChgMotSub(0, 0);
            MHPOST();
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
        hit = marioSearchGround(&y, &tmp1, &tmp2, fall, fall);
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
            mario->dispOfs2.y = 0.0f;
            marioChgPose("M_A_1B");
            attr = hitGetAttr(hit);
            if (attr & 0x200) {
                set_damage_root_ypos(mario->position.y);
                d = -0.4f;
                mario->jumpVelocityY = d;
                mario->jumpAccelerationY = d;
                mario->jumpJerkY = 0.0f;
                mario->jumpSnapY = 0.0f;
                marioChgMot(MARIO_MOTION_DAMAGE);
                return;
            }
            if (attr & 0x400) {
                set_damage_root_ypos(mario->position.y);
                marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
                return;
            }
            if (attr & 0x800) {
                set_damage_root_ypos(mario->position.y);
                marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
                return;
            }
            psndSFXOn_3D((char*)0x159, &mario->position);
            evtEntryType(marioBgmodeChk() == 0 ? quake_evt : quake_bgmode_evt, 0, 0, 0);
        } else {
            mario->position.y += fall;
        }
        if (mario->screenY > 400 && !marioBgmodeChk()) {
            camFollowYOn();
        }
        break;
    case 22:
        if (U_chkground() == 1) {
            return;
        }
        if (--mario->multiTimer <= 0) {
            mario->dispFlags &= ~0x08000000;
            if (!marioBgmodeChk()) {
                camFollowYOn();
            }
            mario->dispFlags &= ~4;
            mario->flags &= ~0x80;
            marioChgMotSub(0, 0);
            MHPOST();
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

#pragma dont_inline on
s32 U_chkground(void) {
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
        mario->dispFlags &= ~4;
        mario->flags &= ~0x80;
        camFollowYOn();
        attr = hitGetAttr(hit);
        if (attr & 0x200) {
            set_damage_root_ypos(mario->position.y);
            v = -0.4f;
            mario->jumpVelocityY = v;
            mario->jumpAccelerationY = v;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
            marioChgMot(MARIO_MOTION_DAMAGE);
            return 1;
        }
        if (attr & 0xC00) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
            return 1;
        }
        return 2;
    }
    return 0;
}

#pragma dont_inline reset

#ifdef VERSION_G8ME01
void mot_hip_post(void) {
    hip_post_inl();
}
#else
void mot_hip_post(void) {
    MarioWork* mario;

    mario = marioGetPtr();
    marioOfsRotReset();
    mario->airTimer = 0;
    mario->jumpPeakAirTime = 0;
    if ((u32)SND1 != 0xFFFFFFFFU) {
        psndSFXOff(SND1);
        SND1 = -1;
    }
    if ((u32)SND2 != 0xFFFFFFFFU) {
        psndSFXOff(SND2);
        SND2 = -1;
    }
}
#endif
