#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "manager/evtmgr.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "evt/evt_cam.h"
#include "evt/evt_damage.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "eff/eff_splash.h"

HitEntry* marioChkLandon(f32* outY, f32 y);
void marioClearSlitFloor(void);
void marioSlitForceCancel(void);
void marioYoshiForceCancel(void);
void nokonoko_use_cancel(void);
void bomhei_use_cancel2(void);
void mario_simple_kemuri(void);


static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, 4, PTR(0xF24A7A8A), PTR(0xF24A7A80), 50)
    RETURN
EVT_END()

static EVT_BEGIN(quake_bgmode_evt)
    CALL(evt_cam_shake, 4, PTR(0xF24A7A86), PTR(0xF24A7A80), 30)
    RETURN
EVT_END()

s32 gazigazi(EventEntry* evt, BOOL isFirstCall);

EVT_BEGIN(evt_gazigazi)
    CALL(gazigazi)
    SPAWN_EVENT(evt_gazigazi_appear)
    RETURN
EVT_END()

void marioDmgRetQuakeOn(void) {
    EvtWord* evt;
    if (marioBgmodeChk() == 0) {
        evt = quake_evt;
    } else {
        evt = quake_bgmode_evt;
    }
    evtEntry(evt, 0, 0);
}

void marioSetSplash(s32 kind, Vec* pos) {
    if (kind == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_WATER1), pos);
    }
    if (marioBgmodeChk() != 1) {
        effSplashEntry(kind != 0, pos->x, pos->y, pos->z, 1.0f);
    }
    if (kind == 1) {
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_DAMAGED2), pos);
    }
}

s32 gazigazi(EventEntry* evt, BOOL isFirstCall) {
    MarioWork* mario = marioGetPtr();
    evt->lwData[0] = (s32)mario->position.x;
    evt->lwData[1] = (s32)mario->mot.damage.rootY;
    evt->lwData[2] = (s32)mario->position.z;
    return 2;
}

BOOL marioChkDamageSink(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_DAMAGE && (s32)mario->currSubMotionId >= 0 && (s32)mario->currSubMotionId < 10) {
        return TRUE;
    }
    return FALSE;
}

void set_damage_root_ypos(f32 y) {
    MarioWork* mario = marioGetPtr();
    mario->mot.damage.rootY = y;
}

void mot_damage(void) {
    MarioWork* mario = marioGetPtr();
    Vec dest;
    f32 d;

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        if (mario->jumpVelocityY == 0.0f) {
            mario->jumpVelocityY = -6.0f;
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
        }
        marioClearSlitFloor();
        marioOfsRotReset();
        mario->dispFlags &= ~0x100;
        marioAdjustMoveDir();
        marioSlitForceCancel();
        bomhei_use_cancel2();
        marioNoUpdateCamPos();
        if (mario->characterId == 2) {
            marioChgPose("KPA_D_1");
        } else {
            marioChgEvtPose("M_F_1");
        }
        mario->currSubMotionId = 0;
        mario->airTimer = 0;
        mario->baseSpeed = 0.0f;
        mario->multiTimer = 20;
        if (mario->prevMotionId != MARIO_MOTION_YOSHI && mario->prevMotionId != MARIO_MOTION_PLANE) {
            Vec pos = mario->position;
            marioSetSplash(0, &pos);
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            mario->dispOfs = v;
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            mario->dispOfs2 = v;
        }
        npcStopForEvent();
    }

    switch (mario->currSubMotionId) {
    case 0:
        if (mario->position.y >= mario->mot.damage.rootY - 160.0f) {
            marioMakeJumpPara();
            if (mario->jumpVelocityY <= -6.0f) {
                mario->jumpVelocityY = -6.0f;
                mario->jumpAccelerationY = 0.0f;
                mario->jumpJerkY = 0.0f;
                mario->jumpSnapY = 0.0f;
            }
            mario->position.y += mario->jumpVelocityY;
        } else {
            mario->jumpVelocityY = 0.3f;
            mario->currSubMotionId = 1;
            mario->multiTimer = 40;
        }
        break;
    case 1:
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->multiTimer = 0;
        mario->currSubMotionId = 10;
        mario->mot.damage.riseSpeed = 20.0f;
        break;
    case 10:
        if (++mario->multiTimer == 2) {
            mario->currSubMotionId = 11;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_LEAP_WATER1), &mario->position);
            if (mario->damageHit != NULL) {
                evtEntry(evt_gazigazi, 0, 0);
            }
        }
    case 11: {
        f32 v;
        f32 prevY = mario->position.y;
        v = mario->mot.damage.riseSpeed;
        mario->position.y += v;
        v -= 1.0f;
        if (v < 10.0f) {
            v = 10.0f;
        }
        mario->mot.damage.riseSpeed = v;
        if (mario->position.y >= mario->mot.damage.rootY && mario->mot.damage.rootY > prevY) {
            Vec pos = mario->position;
            marioSetSplash(1, &pos);
        }
        if (mario->position.y < 500.0f + mario->mot.damage.rootY) {
            break;
        }
        {
            if (mario->damageHit != NULL) {
                Vec* p = hitGetDamageReturnPos((HitEntry*)mario->damageHit);
                Vec t = {0.0f, 0.0f, 0.0f};
                t.x = p->x;
                t.y = p->y;
                t.z = p->z;
                dest = t;
            } else {
                Vec z = {0.0f, 0.0f, 0.0f};
                dest = z;
                if (strcmp(gp->currentMapName, "muj_00") == 0) {
                    Vec a = {400.0f, 35.0f, -100.0f};
                    dest = a;
                }
                if (strcmp(gp->currentMapName, "gor_00") == 0) {
                    Vec a = {0.0f, 0.0f, -200.0f};
                    dest = a;
                }
                if (strcmp(gp->currentMapName, "gor_04") == 0) {
                    Vec a = {146.0f, 0.0f, 146.0f};
                    dest = a;
                }
            }
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                Vec t = {0.0f, 0.0f, 0.0f};
                t.x = mario->returnPos.x;
                t.y = mario->returnPos.y;
                t.z = mario->returnPos.z;
                dest = t;
            }
            mario->baseSpeed = 8.0f;
            d = distABf(dest.x, dest.z, mario->position.x, mario->position.z);
            mario->multiTimer = (s16)(d / 8);
            mario->directionView = angleABf(mario->position.x, mario->position.z, dest.x, dest.z);
            mario->mot.damage.camStepY = (dest.y - mario->mot.damage.rootY) / (f32)(s32)mario->multiTimer;
            mario->currSubMotionId = 12;
            mario->dispFlags |= 1;
            if ((s32)mario->multiTimer == 0) {
                mario->multiTimer = 10;
                mario->mot.damage.camStepY = (dest.y - mario->mot.damage.rootY) / (f32)(s32)mario->multiTimer;
                mario->currSubMotionId = 14;
            }
        }
        break;
    }
    case 12:
        mario->camTarget.y += mario->mot.damage.camStepY;
        movePos(&mario->position.x, &mario->position.z, mario->baseSpeed, mario->directionView);
        mario->camTarget.x = mario->position.x;
        mario->camTarget.z = mario->position.z;
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->currSubMotionId = 13;
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioSetFallPara();
        mario->dispFlags &= ~1;
        mario->multiTimer = 1;
        break;
    case 13:
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->currSubMotionId = 20;
        break;
    case 14:
        mario->camTarget.y += mario->mot.damage.camStepY;
        mario->camTarget.x = mario->position.x;
        mario->camTarget.z = mario->position.z;
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioSetFallPara();
        mario->dispFlags &= ~1;
        mario->currSubMotionId = 20;
        break;
    case 20: {
        HitEntry* h;
        f32 landY;
        f32 spd; f32 baseY;

        mario->hitobjStandOn = NULL;
        spd = marioGetFallSpd();
        if (spd <= -10.0f) {
            spd = -10.0f;
        }
        if (mario->damageHit != NULL) {
            Vec* p = hitGetDamageReturnPos((HitEntry*)mario->damageHit);
            baseY = p->y;
        } else {
            baseY = mario->position.y;
        }
        if (marioBgmodeChk() == 1 || mario->position.y - baseY <= 50.0f) {
            h = marioChkLandon(&landY, spd);
        } else {
            h = NULL;
        }
        if (h == NULL) {
            mario->position.y += spd;
        } else {
            mario->hitobjStandOn = h;
            mario->hitobjJumpFrom = NULL;
            mario->position.y = landY;
            if (mario->characterId == 2) {
                marioChgPose("KPA_D_2");
            } else {
                marioChgPose("M_D_2");
            }
            marioDmgRetQuakeOn();
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_LAND_DAMAGE2), &mario->position);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_CRUSH1), &mario->position);
            mario_simple_kemuri();
        }
        if (mario->hitobjStandOn == NULL) {
            break;
        }
    }
    case 30:
        mario->multiTimer = 50;
        mario->currSubMotionId = 31;
    case 31:
        if (--mario->multiTimer > 0) {
            break;
        }
        if (mario->characterId == 0) {
            pouchAddHP(-1);
        }
        if (pouchGetHP() <= 0) {
            mario->currSubMotionId = 33;
            marioUpdateCamPos();
            break;
        }
        if (mario->characterId != 2) {
            marioChgPose("M_U_3");
        }
        mario->multiTimer = 12;
        mario->currSubMotionId = 32;
    case 32:
        if (--mario->multiTimer > 0) {
            break;
        }
        marioChgMot(MARIO_MOTION_STAY);
        marioUpdateCamPos();
        npcStartForEvent();
        break;
    }
}

BOOL marioWaitDamageReturnFall(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_DAMAGE) {
        return -1;
    }
    if ((s32)mario->currSubMotionId >= 20) {
        f32 x, y, z;
        Vec pos = mario->position;
        marioGetScreenPos(pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y)) {
            return TRUE;
        }
    }
    return FALSE;
}

void mot_damageToge(void) {
    MarioWork* mario = marioGetPtr();
    Vec dest;
    f32 d;

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioClearSlitFloor();
        mario->dispFlags &= ~0x100;
        marioAdjustMoveDir();
        marioSlitForceCancel();
        marioYoshiForceCancel();
        nokonoko_use_cancel();
        marioNoUpdateCamPos();
        bomhei_use_cancel2();
        mario->currSubMotionId = 0;
        mario->airTimer = 0;
        mario->baseSpeed = 0.0f;
        mario->mot.damage.rootY = mario->position.y;
        mario->multiTimer = 5;
        mario->mot.damage.riseSpeed = 4.0f;
    }

    switch (mario->currSubMotionId) {
    case 0:
        if (--mario->multiTimer > 0) {
            break;
        }
        if (mario->prevMotionId == MARIO_MOTION_HIP || mario->prevMotionId == MARIO_MOTION_HIP_2) {
            marioChgEvtPose("M_D_7");
        } else {
            marioChgPose("M_D_6");
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_DAMAGED2), &mario->position);
        mario_simple_kemuri();
        mario->currSubMotionId = 11;
    case 11: {
        f32 v = mario->mot.damage.riseSpeed;
        mario->position.y += v;
        v -= 1.0f;
        if (v < 10.0f) {
            v = 10.0f;
        }
        mario->mot.damage.riseSpeed = v;
        if (mario->position.y < 500.0f + mario->mot.damage.rootY) {
            break;
        }
        {
            if (mario->damageHit != NULL) {
                Vec* p = hitGetDamageReturnPos((HitEntry*)mario->damageHit);
                Vec t = {0.0f, 0.0f, 0.0f};
                t.x = p->x;
                t.y = p->y;
                t.z = p->z;
                dest = t;
            } else {
                Vec z = {0.0f, 0.0f, 0.0f};
                dest = z;
            }
            mario->baseSpeed = 8.0f;
            d = distABf(dest.x, dest.z, mario->position.x, mario->position.z);
            mario->multiTimer = (s16)(d / 8);
            mario->directionView = angleABf(mario->position.x, mario->position.z, dest.x, dest.z);
            mario->mot.damage.camStepY = (dest.y - mario->mot.damage.rootY) / (f32)(s32)mario->multiTimer;
            mario->currSubMotionId = 12;
        }
        break;
    }
    case 12:
        mario->camTarget.y += mario->mot.damage.camStepY;
        movePos(&mario->position.x, &mario->position.z, mario->baseSpeed, mario->directionView);
        mario->camTarget.x = mario->position.x;
        mario->camTarget.z = mario->position.z;
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->currSubMotionId = 13;
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioSetFallPara();
        break;
    case 13:
        if (--mario->multiTimer > 0) {
            break;
        }
        mario->currSubMotionId = 20;
        break;
    case 20: {
        f32 spd;
        HitEntry* h;
        f32 landY;
        mario->hitobjStandOn = NULL;
        spd = marioGetFallSpd();
        if (spd <= -10.0f) {
            spd = -10.0f;
        }
        h = marioChkLandon(&landY, spd);
        if (h == NULL) {
            mario->position.y += spd;
        } else {
            marioChgPose("M_D_2");
            marioDmgRetQuakeOn();
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_LAND_DAMAGE2), &mario->position);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_CRUSH1), &mario->position);
            mario_simple_kemuri();
            mario->hitobjStandOn = h;
            mario->hitobjJumpFrom = NULL;
            mario->position.y = landY;
        }
        if (mario->hitobjStandOn == NULL) {
            break;
        }
    }
    case 30:
        mario->multiTimer = 50;
        mario->currSubMotionId = 31;
    case 31:
        if (--mario->multiTimer > 0) {
            break;
        }
        pouchAddHP(-1);
        if (pouchGetHP() <= 0) {
            mario->currSubMotionId = 33;
            marioUpdateCamPos();
            break;
        }
        marioChgPose("M_U_3");
        mario->multiTimer = 12;
        mario->currSubMotionId = 32;
    case 32:
        if (--mario->multiTimer > 0) {
            break;
        }
        marioChgMot(MARIO_MOTION_STAY);
        marioUpdateCamPos();
        break;
    }
}

BOOL marioWaitDamageTogeReturnFall(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE) {
        return -1;
    }
    if ((s32)mario->currSubMotionId >= 20) {
        f32 x, y, z;
        Vec pos = mario->position;
        marioGetScreenPos(pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y)) {
            return TRUE;
        }
    }
    return FALSE;
}
