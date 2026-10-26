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

double cos(float);
HitEntry* hitEstimate(s32* out);
static HitEntry* hammerHitWall(f32 ang, f32* s, f32* co, f32* len, f32* hx, f32* hy, f32* hz, f32* nx, f32* ny, f32* nz);
s32 marioGetRub(s32 mask, s32* a, s32* b, f32* c);



s32 se_data[] = {315, 357, 316, 359, 317, 358, 318, 360, 319, 361};

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

inline BOOL chkRevDir(f32 angle) {
    BOOL ret = FALSE;
    if (angle >= 90.0f && angle <= 270.0f) {
        ret = TRUE;
    }
    return ret;
}

#define HAM_DIR(mario, dst)                                                                                     \
    {                                                                                                           \
        f32 a = mario->displayDirection;                                                                        \
        MarioWork* p = marioGetPtr(); f32 r;                                                                    \
        f32 off = 0.0f;                                                                                         \
        if (p->dispFlags & MARIO_DISP_FLAG_REAR_POSE) {                                                                        \
            off = chkRevDir(p->displayDirection) ? 30.0f : -30.0f;                                              \
        } if (a >= 90.0f && a <= 270.0f) { r = revise360(180.0f + off); } else { r = revise360(off); } dst = r; \
    }

static inline s32 hamLv(void) {
    return pouchGetHammerLv();
}


typedef struct { u8 pad[0x15]; char name[0x10]; } HamMobj;

static const char isS1_str[14];
static const char isB1_str[17];
static const char isS2_str[14];
static const char isB2_str[17];
static const char isS3_str[14];
static const char isB3_str[17];
#define isB3(n) (strcmp((n), isB3_str) == 0)

#define isS3(n) (strcmp((n), isS3_str) == 0)

#define isB2(n) (strcmp((n), isB2_str) == 0)

#define isS2(n) (strcmp((n), isS2_str) == 0)

#define isB1(n) (strcmp((n), isB1_str) == 0)

#define isS1(n) (strcmp((n), isS1_str) == 0)

static inline s32 chkBlock(HamMobj* obj) {
    s32 ok = 0;
    if ((isS1(obj->name) || isB1(obj->name))) {
        if (pouchGetPtr()->hammerLevel >= 1) {
            ok = 1;
        }
    } else if ((isS2(obj->name) || isB2(obj->name))) {
        if (pouchGetPtr()->hammerLevel >= 2) {
            ok = 1;
        }
    } else if ((isS3(obj->name) || isB3(obj->name))) {
        if (pouchGetPtr()->hammerLevel >= 3) {
            ok = 1;
        }
    }
    return ok;
}

s32 marioGetHammerLevel(void) {
    return pouchGetPtr()->hammerLevel;
}


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

    if ((s32)mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        HAM_DIR(mario, mario->mot.hammer.dir);
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
            HAM_DIR(mario, mario->mot.hammer.dir);
        }
        if (hitGetAttr(hit) & HIT_ATTR_MOBJ) {
            s32 ok = chkBlock((HamMobj*)mobjHitObjPtrToPtr(hit));
            if (ok) {
                kind = 0;
            }
        }
        switch (pouchGetHammerLv()) {
        case 1:
            marioChgPose(kind == 7 ? "M_H_2" : "M_H_1");
            break;
        case 2:
            marioChgPose(kind == 7 ? "M_H_5" : "M_H_4");
            break;
        case 3:
            marioChgPose(kind == 7 ? "M_H_8" : "M_H_7");
            break;
        }
        if (kind != 0) {
            {
                s32 k = kind;
                mario->mot.hammer.hitFrames = k == 7 ? 3 : k;
            }
            mario->flags |= MARIO_FLAG_HAMMER_HIT_WALL;
        } else {
            mario->flags &= ~MARIO_FLAG_HAMMER_HIT_WALL;
        }
        {
            MarioWork* m = marioGetPtr();
            snd = 0x15B;
            switch (pouchGetHammerLv()) {
            case 1:
                snd = 0x15B;
                break;
            case 2:
                snd = 0x15C;
                break;
            case 3:
                snd = 0x15D;
                break;
            }
            psndSFXOn_3D(SFX_NAME_WORD(snd), &m->position);
        }
        mario->mot.hammer.hitKind = kind;
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
        {
            s32 lv = pouchGetPtr()->hammerLevel;
            if (lv == 1) {
                evtEntry(lv1quake_evt, 0, 0);
            } else if (lv == 2) {
                evtEntry(lv2quake_evt, 0, 0);
            } else if (lv == 3) {
                evtEntry(lv3quake_evt, 0, 0);
            }
        }
        mario->currSubMotionId = 1;
        mario->hitobjHammer = mario->hitobjHammerNew;
        px = mario->position.x;
        py = mario->position.y;
        pz = mario->position.z;
        if (!(mario->dispFlags & MARIO_DISP_FLAG_REAR_POSE)) {
            efType = -1;
            switch (mario->mot.hammer.hitKind) {
            case 7:
                movePos(&px, &pz, 16.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                ez = 53.0f;
                efType = 0x16;
                break;
            case 0:
            case 6:
                movePos(&px, &pz, 25.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                ez = -3.0f;
                efType = 0x15;
                break;
            case 5:
                movePos(&px, &pz, 31.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                ez = 5.0f;
                efType = 0x15;
                break;
            case 4:
                movePos(&px, &pz, 30.0f, toMovedir2(mario->displayDirectionTarget, 0.0f));
                ez = 30.0f;
                efType = 0x16;
                break;
            }
        } else {
            efType = -1;
            switch (mario->mot.hammer.hitKind) {
            case 7:
                movePos(&px, &pz, 16.0f, toMovedir2(mario->displayDirectionTarget, -2.0f));
                ez = 53.0f;
                efType = 0x16;
                break;
            case 0:
            case 6:
                movePos(&px, &pz, 30.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                ez = -3.0f;
                efType = 0x15;
                break;
            case 5:
                movePos(&px, &pz, 36.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                ez = 20.0f;
                efType = 0x16;
                break;
            case 4:
                movePos(&px, &pz, 37.0f, toMovedir2(mario->displayDirectionTarget, -6.0f));
                ez = 28.0f;
                efType = 0x16;
                break;
            }
        }
        if (efType > 0) {
            effKemuTestEntry(efType, px, py + ez, pz, 1.0f);
        }
        {
            MarioWork* m = marioGetPtr();
            snd = 0x161;
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
            for (i = 0; (u32)i < 10; i++) {
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
    }
    if (++mario->multiTimer > 20) {
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioChgMot(MARIO_MOTION_STAY);
    }
}


s32 marioChkHammer2(void) {
    MarioWork* mario = marioGetPtr();
    s32 b, a;
    f32 c;
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
#ifdef VERSION_G8ME01
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
    if (marioGetRub(0x200, &a, &b, &c) != 1) {
        return 0;
    }
    mario->baseSpeed = 0.0f;
    if (b < 1) {
        return 0;
    }
    mario->hammerDir = a;
    return 1;
}




static inline void mot_ham(f32 a148, f32 a68) {
    MarioWork* m = marioGetPtr();
    m->camShiftTargetZ = a148;
    m->camShiftRate.z = 0.005f;
    m->camZoom = a68;
    m->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

#define H2_LV(lvl, a148, a68) { mot_ham(a148, a68); mario->mot.hammer2.flags |= 0x1000; }

#define H2_RAMP(maxv, midv, minv, lvl, a148, a68)                         \
    marioChgPaper("P_H_1A");                                        \
    if (!(mario->mot.hammer2.flags & 1)) {                                         \
        mario->mot.hammer2.animTime += 6.0f;                                          \
        if (mario->mot.hammer2.animTime >= maxv) {                                    \
            mario->mot.hammer2.animTime = maxv;                                       \
            mario->mot.hammer2.flags |= 1;                                         \
            mario->hammerChargeLv = lvl;                                       \
            H2_LV(lvl, a148, a68)                                   \
        }                                                           \
    } else if (!(mario->mot.hammer2.flags & 0x10)) {                               \
        mario->mot.hammer2.animTime += 1.0f;                                          \
        if (mario->mot.hammer2.animTime >= midv) {                                    \
            mario->mot.hammer2.animTime = midv;                                       \
            mario->mot.hammer2.flags |= 0x10;                                      \
        }                                                           \
    } else {                                                        \
        mario->mot.hammer2.animTime -= 1.0f;                                          \
        if (mario->mot.hammer2.animTime <= minv) {                                    \
            mario->mot.hammer2.animTime = minv;                                       \
            mario->mot.hammer2.flags &= ~0x10;                                     \
        }                                                           \
    }

#ifdef VERSION_G8ME01
#define H2_LIGHT_ON()
#else
#define H2_LIGHT_ON() marioPaperLightOn();
#endif

#define H2_CLR_PAPER()                                              \
    marioPaperOff();                                                \
    H2_LIGHT_ON()                                            \
    marioChgPose("M_S_1");                                          \
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;                                       \
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;                                       \
    mario->dispFlags &= ~MARIO_DISP_FLAG_FLIP;

void mot_hammer2(void) {
    MarioWork* mario = marioGetPtr();
    s32 b, a;
    f32 c;
    HitEntry* hit;
    f32 ang2, ang;
    f32 nz, ny, nx, len, hz, hy, hx;
    f32 co, s;

    if ((s32)mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->mot.hammer2.flags = 0;
        mario->currSubMotionId = 0;
        mario->mot.hammer2.sfxId = -1;
#ifdef VERSION_G8ME01
        mario->savedDisplayDirection = mario->displayDirection;
#endif
    }
    switch ((s32)mario->currSubMotionId) {
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
        mario->mot.hammer2.sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN1), (Vec*)&mario->position.x);
        mario->mot.hammer2.animTime = 0.0f;
        mario->poseTime = 0;
        animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], mario->mot.hammer2.animTime);
        mario->currSubMotionId = 10;
        mario->multiTimer = 30;
    case 10:
        if (marioGetRub(0x200, &a, &b, &c) == 1) {
            mario->mot.hammer2.flags &= ~1;
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
            H2_LV(3, 0.4f, 1.7f)
        } else if (mario->hammerChargeTimer >= 3) {
            H2_RAMP(54.0f, 54.0f, 42.0f, 3, 0.4f, 1.7f)
        } else if (mario->hammerChargeTimer >= 2) {
            H2_RAMP(36.0f, 36.0f, 30.0f, 2, 0.3f, 1.6f)
        } else {
            H2_RAMP(18.0f, 24.0f, 18.0f, 1, 0.2f, 1.5f)
        }
        if (mario->mot.hammer2.flags & 0x1000) {
            mario->mot.hammer2.flags &= ~0x1000;
            if (!(mario->mot.hammer2.flags & 0x2000)) {
                mario->mot.hammer2.flags |= 0x2000;
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
            if ((u32)mario->mot.hammer2.sfxId != 0xFFFFFFFFU) {
                psndSFXOff(mario->mot.hammer2.sfxId);
                mario->mot.hammer2.sfxId = -1;
            }
            {
                MarioWork* m = marioGetPtr();
                m->camShiftTargetZ = 0.0f;
                m->camShiftRate.z = 0.01f;
            }
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
            effUltraHammerEntry(mario->position.x, 20.0f + mario->position.y, mario->position.z, 1.0f, mario->hammerDir != 0, (s32)mario->multiTimer);
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
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN2), (Vec*)&mario->position.x);
        }
        if (mario->mot.hammer2.timer == 1) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HAMMER_KAITEN2), (Vec*)&mario->position.x);
        }
        mario->multiTimer--;
        if (mario->hammerChargeLv > 2 && (s32)mario->multiTimer == 10) {
            H2_CLR_PAPER()
        }
        if ((s32)mario->multiTimer > 0) {
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
            H2_CLR_PAPER()
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


f32 marioGetAngleKaitenHammer(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_HAMMER_2) {
        return -1.0f;
    }
    if ((s32)mario->currSubMotionId != 0x14) {
        return -2.0f;
    }
    return revise360(270.0f - mario->displayDirection);
}


void mot_ham2_post(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->dispFlags & MARIO_DISP_FLAG_PAPER) {
        marioPaperOff();
    }
    {
        MarioWork* m = marioGetPtr();
        m->camShiftTargetZ = 0.0f;
        m->camShiftRate.z = 0.01f;
    }
    marioPaperOff();
#ifndef VERSION_G8ME01
    marioPaperLightOn();
#endif
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    mario->dispFlags &= ~MARIO_DISP_FLAG_FLIP;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioAdjustMoveDir();
}

HitEntry *hitEstimate(s32 *out) {
    MarioWork *mario;
    HitCheckQuery q;
    Vec pos;
    HitEntry *hit;
    f32 s;
    f32 c;
    f32 cy;
    f32 a;
    Vec dir4Copy;
    f32 h;
    s32 i;
    f32 k5 = 5.0f;
    mario = marioGetPtr();
    *out = 0;
    a = mario->displayDirection;
    {
        f32 off = (marioGetPtr()->dispFlags & MARIO_DISP_FLAG_REAR_POSE) ? (-20.0f) : (0.0f);
        if ((a >= 90.0f) && (a <= 270.0f)) {
            a = toMovedir2(180.0f, off);
        } else {
            a = toMovedir2(0.0f, off);
        }
    }
    sincosf(&s, &c, a);
    h = mario->wPlayerCollisionBox.y;
    cy = (f32) cos(3.14159265f);
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
            *out = 7;
            return hit;
        }
    }
    cy = (f32) cos(1.2217306f);
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
            *out = ((q.hitPos.y - mario->position.y) > 30.0f) ? (4) : (5);
            return hit;
        }
    }
    cy = (f32) cos(0.78539816f);
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
            *out = 5;
            return hit;
        }
    }
    cy = (f32) cos(0.0f);
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
                    *out = ((q.hitPos.y - mario->position.y) > 13.0f) ? (5) : (6);
                    return hit;
                }
            }

        }
    }
    return hit;
}

static const char isS1_str[] = "MOBJ_Lv1Block";
static const char isB1_str[] = "MOBJ_Lv1BigBlock";
static const char isS2_str[] = "MOBJ_Lv2Block";
static const char isB2_str[] = "MOBJ_Lv2BigBlock";
static const char isS3_str[] = "MOBJ_Lv3Block";
static const char isB3_str[] = "MOBJ_Lv3BigBlock";

static HitEntry* hammerHitWall(f32 ang, f32* s, f32* co, f32* len, f32* hx, f32* hy, f32* hz, f32* nx, f32* ny, f32* nz) {
    MarioWork* m = marioGetPtr();
    sincosf(s, co, ang);
    *len = 2.0f * m->wPlayerCollisionBox.x;
    return hitCheckFilter(0, hx, hy, hz, len, nx, ny, nz, m->position.x, 0.5f * m->wPlayerCollisionBox.y + m->position.y, m->position.z, *s, 0.0f, *co);
}
