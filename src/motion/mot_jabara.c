#include "version.h"
#include "dolphin/types.h"
#include "evt/evt_cmd.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "driver/animdrv.h"
#include "mario/mario_hit.h"
#include <dolphin/pad.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "party.h"
#include "evt/evt_cam.h"
#include "mot/mot_stay.h"
#include "mot/mot_slit.h"

BOOL marioGetRub(s32 mask, s32* a, s32* b, s32* c);

typedef struct MotJabaraWork {
    u32 flags;          // 0x00: 1 = swing input taken (swingMain); 0x10000 / 0x20000 / 0x40000 = one-shot charge-level sounds played
    f32 swingAccel;     // 0x04: added to the swing angle while the push timer runs (2.4 start, 0.1 walk)
    f32 swingDecel;     // 0x08: added once the push timer hits 0 (-0.4 start, -0.1 walk)
    f32 field_0xC;      // 0x0C: cleared at sub-motion 0x32
    f32 swingRotStep;   // 0x10: 0.18, swing tilt step
    s32 wobbleCount;    // 0x14: wobble direction changes, picks SFX 0x189..0x18B
    f32 wobbleDir;      // 0x18: +-0.6, last wobble direction
    s32 headBlockTimer; // 0x1C: frames with a ceiling overhead, > 5 s ends the spring
} MotJabaraWork;        // size 0x20


static inline void jabaraSetSpd(f32 a, f32 b, f32 c) {
    MarioWork* mp = marioGetPtr();
    mp->camShiftTargetZ = a;
    mp->camShiftRate.z = b;
    mp->camZoom = c;
    mp->camZoomOfs = (Vec){0.0f, 40.0f, 0.0f};
}

static inline void jabaraResetPose(void) {
    MarioWork* m2 = marioGetPtr();
    m2->displayDirection = m2->mot.jabara.dir;
    m2->displayDirectionTarget = m2->mot.jabara.dir;
    m2->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    m2->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    m2->rot = (Vec){0.0f, 0.0f, 0.0f};
    m2->wPlayerCollisionBox.x = 20.0f;
    m2->wPlayerCollisionBox.y = 37.0f;
    marioAdjustMoveDir();
    m2->dispFlags &= ~0x4;
    m2->dispFlags &= ~0x100;
}

static inline void* jabaraHitFront(f32 ang) {
    f32 sn, cs, hx, hy, hz, dist, nx, ny, nz;
    MarioWork* mp = marioGetPtr();
    f32 px = mp->position.x;
    f32 py = 2.0f + (mp->position.y + mp->wPlayerCollisionBox.y);
    f32 pz = mp->position.z;
    dist = 0.5f * mp->wPlayerCollisionBox.x;
    sincosf(&sn, &cs, ang);
    return hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, px, py, pz, sn, 0.0f, cs);
}


static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, 4, 0xF24A7A80, 0xF24A7A8F, 0xF0)
    RETURN
EVT_END()

s32 marioGetJabaraState(void) {
    s32 state = 7;
    MarioWork* mario = marioGetPtr();
    if ((u16)mario->currMotionId != MARIO_MOTION_JABARA) {
        state = 0;
    } else {
        s32 id = (s32)mario->currSubMotionId;
        if (id == 0x14) {
            state = 1;
        } else if (id >= 0x46 && id <= 0x47) {
            state = 2;
        } else if (id == 0x50) {
            state = 3;
        } else if (id >= 0x28 && id <= 0x2b) {
            state = 4;
        } else if (id >= 0x32 && id <= 0x34) {
            state = 5;
        } else if (id >= 0x1e && id <= 0x1f) {
            state = 6;
        }
    }
    return state;
}

BOOL marioPriCheckJabara(void) {
    MarioWork* mario = marioGetPtr();
    u32 flags = mario->flags;
    if ((flags & 0x80000000) || (flags & VERSION_PICK(0x100000, 0x2100000))) {
        mario->wMotionTimer = 0;
        return FALSE;
    }
    return pouchCheckItem(ITEM_ULTRA_BOOTS) != 0;
}

static inline BOOL priChk(void) {
    MarioWork* mario = marioGetPtr();
    u32 flags = mario->flags;
    BOOL ret;
    if ((flags & 0x80000000) || (flags & VERSION_PICK(0x100000, 0x2100000))) {
        mario->wMotionTimer = 0;
        ret = FALSE;
    } else {
        if ((s32)pouchCheckItem(ITEM_ULTRA_BOOTS) == 0) {
            ret = FALSE;
        } else {
            ret = TRUE;
        }
    }
    return ret;
}

BOOL marioChkJabara(void) {
    MarioWork* mario = marioGetPtr();
    s32 a, b, c;
    if (!priChk()) {
        return FALSE;
    }
    if (!(mario->buttonHold & PAD_BUTTON_A)) {
        if (!(mario->buttonHold & PAD_BUTTON_B)) {
            if (strcmp(mario->animName, "M_I_Y") == 0 && (u16)mario->currMotionId == MARIO_MOTION_STAY) {
                marioChgPose("M_S_1");
                marioVoiceGlareOff();
            }
            mario->wMotionTimer = 0;
        }
        return FALSE;
    }
#ifdef VERSION_G8ME01
    if ((mario->trigFlags & MARIO_TRIG_FLAG_START_JABARA_SPIN) && (u16)mario->currMotionId == MARIO_MOTION_STAY && !((u32)mario->dispFlags & 0x40000000) &&
        mario->controlStickSensitivity) {
#else
    mario->wMotionTimer++;
    if (mario->controlStickSensitivity && mario->wMotionTimer > 10 && (u16)mario->currMotionId == MARIO_MOTION_STAY &&
        !((u32)mario->dispFlags & 0x40000000)) {
#endif
        marioChgPose("M_I_Y");
        marioVoiceGlareOn();
    }
#ifndef VERSION_G8ME01
    if (mario->wMotionTimer >= 0x32) {
        mario->wMotionTimer = 0x32;
        if ((u16)mario->currMotionId == MARIO_MOTION_STAY && !((u32)mario->dispFlags & 0x40000000)) {
            marioChgPose("M_I_Y");
            marioVoiceGlareOn();
        }
    }
#endif
    if (marioGetRub(0x100, &b, &a, &c) == FALSE) {
        return FALSE;
    } else {
        return a >= 1;
    }
}

static inline f32 jabaraHitPosZ(HitEntry* hit) {
    Vec v;
    marioGetPtr();
    hitObjGetPos(hitGetName(hit), &v);
    return v.z;
}

static inline void jabaraLand(f32 y_, void* h_) {
    MarioWork* mp_ = marioGetPtr();
    mp_->airTimer = 0;
    mp_->jumpPeakAirTime = 0;
    mp_->hitobjStandOn = h_;
    mp_->hitobjJumpFrom = NULL;
    mp_->position.y = y_;
    mp_->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}

#define POS (&mario->position)
#define SFX(id) psndSFXOn_3D((const char*)(id), POS)
#define ANIM_STEP() animPoseSetLocalTime(mario->paperAnimGroupId[marioAnimeId()], (f32)(s32)mario->mot.jabara.animFrame)
#define JAB_TAIL(MOTSUB) \
    { \
        jabaraResetPose(); \
        MOTSUB \
        if (marioGetPtr()->motStruct) { \
            __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct); \
            marioGetPtr()->motStruct = NULL; \
        } \
    }
#define MOTSUB_ marioChgMotSub(MARIO_MOTION_STAY, 0);
#define JAB_LAND(HIT, Y) \
    marioPaperOff(); \
    jabaraLand((Y), (HIT)); \
    camFollowYOn(); \
    marioGetPtr(); \
    allPartyRideOff(); \
    JAB_TAIL(MOTSUB_)

void mario_land_kemuri(void);
void* marioSearchHead(Vec* p, f32* h, f32 r);
void swingMain(void);
void* marioChkLandon(f32* outY, f32 y);
void jabaraSwingStart(s32 s);
void jabaraSwingWalk(void);

void mot_jabara(void) {
    MarioWork* mario = marioGetPtr();
    Vec v74;
    Vec v68;
    s32 rubCount;
    s32 rubDir;
    s32 rubMag;
    s32 overheadHit;
    f32 hitY;
    f32 overheadDist;
    f32 headDist;
    s32 headHit;
    f32 headRange;
    f32 rr;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->mot.word[0] = 0;
        mario->mot.word[1] = 0;
        mario->mot.jabara.floorY = 0;
        mario->mot.word[3] = 0;
        mario->mot.word[4] = 0;
        mario->mot.word[5] = 0;
        mario->mot.word[6] = 0;
        if (mario->dispFlags & 0x1000000) {
            mot_slit_post();
        }
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->currSubMotionId = 0;
        marioGetPtr()->motStruct = __memAlloc(HEAP_DEFAULT, 0x20);
        memset(marioGetPtr()->motStruct, 0, 0x20);
    }
    mario->hitobjHead = 0;
    switch (mario->currSubMotionId) {
    case 0:
        mario->dispFlags |= 0x100;
        mario->hammerChargeTimer = 1;
        mario->hammerChargeLv = 0;
        mario->airTimer = 0;
        mario->jumpPeakAirTime = 0;
        mario->baseSpeed = 0.0f;
        mario->currSubMotionId = 10;
        marioChgPose("M_J_1A");
        marioPaperOn("p_jyabara");
        marioChgPaper("PM_J_1B");
        mario->lastGroundPos = mario->position;
        marioChkGnd();
        if (mario->currMotionId != MARIO_MOTION_JABARA) {
            mario->currSubMotionId = 0x64;
        }
        break;
    case 10:
        if (mario->currMotionId != MARIO_MOTION_JABARA) {
            mario->currSubMotionId = 0x64;
            break;
        }
        L_allPartyRideOn();
        SFX(0x182);
        jabaraSetSpd(0.2f, 0.005f, 1.5f);
        mario->multiTimer = 8;
        mario->currSubMotionId = 11;
    case 11:
        if (--mario->multiTimer <= 0) {
            mario->currSubMotionId = 12;
            if (!(marioGetPtr()->motJabara->flags & 0x10000)) {
                marioGetPtr()->motJabara->flags |= 0x10000;
            }
        }
    case 12:
        hitY = marioChkOverheadJabara(&overheadHit, &overheadDist);
        if (marioGetRub(0x100, &rubDir, &rubCount, &rubMag) == 1) {
            mario->hammerChargeTimer++;
            if (mario->hammerChargeTimer >= 0x14) {
                mario->hammerChargeTimer = 0x14;
            }
        }
        if (mario->hammerChargeTimer == 3) {
            if (!(marioGetPtr()->motJabara->flags & 0x40000)) {
                marioGetPtr()->motJabara->flags |= 0x40000;
                SFX(0x184);
            }
            marioChgPaper("PM_J_3B");
            mario->hammerChargeLv = 3;
            jabaraSetSpd(0.4f, 0.005f, 1.7f);
        } else if (mario->hammerChargeTimer == 2) {
            if (!(marioGetPtr()->motJabara->flags & 0x20000)) {
                marioGetPtr()->motJabara->flags |= 0x20000;
                SFX(0x183);
            }
            marioChgPaper("PM_J_2B");
            mario->hammerChargeLv = 2;
            jabaraSetSpd(0.3f, 0.005f, 1.6f);
        }
        marioChkGnd();
        if (mario->currMotionId != MARIO_MOTION_JABARA) {
            mario->currSubMotionId = 0x64;
            return;
        }
        if ((f32)mario->screenY > 480.0f || (f32)mario->screenY < 50.0f) {
            camFollowYOn();
        }
        if (mario->position.y == mario->basePos.y) {
            camFollowYOff();
        }
        {
            BOOL found = FALSE;
            v74 = mario->position;
            headDist = 25.0f;
            if (marioSearchHead(&v74, &headDist, 200.0f)) {
                if (!(mario->buttonHold & PAD_BUTTON_A)) {
                    marioGetPtr()->motJabara->headBlockTimer++;
                    if (marioGetPtr()->motJabara->headBlockTimer > sysMsec2Frame(5000)) {
                        found = TRUE;
                    }
                }
                if (!found) {
                    break;
                }
            }
            if (!found && (mario->buttonHold & PAD_BUTTON_A) && !(mario->buttonUp & PAD_BUTTON_A)) {
                break;
            }
        }
        {
            MarioWork* mp = marioGetPtr();
            mp->camShiftTargetZ = 0.0f;
            mp->camShiftRate.z = 0.01f;
        }
        camFollowYOff();
        marioChgPose("M_J_5A");
        marioChgPaper("PM_J_1C");
        SFX(0x20185);
        SFX(0x200b5);
        if (mario->hammerChargeLv == 3) {
            mario->jumpVelocityY = 10.45f;
            mario->jumpAccelerationY = 0.278f;
            mario->jumpJerkY = -0.031f;
            mario->jumpSnapY = 0.0f;
        } else if (mario->hammerChargeLv == 2) {
            mario->jumpVelocityY = 8.0f;
            mario->jumpAccelerationY = -0.84f;
            mario->jumpJerkY = 0.08f;
            mario->jumpSnapY = -0.015f;
        } else {
            mario->jumpVelocityY = 4.3f;
            mario->jumpAccelerationY = -0.84f;
            mario->jumpJerkY = 0.08f;
            mario->jumpSnapY = -0.015f;
        }
        if (mario->hammerChargeLv == 3) {
            mario_land_kemuri();
        }
        mario->hitobjJumpFrom = mario->hitobjStandOn;
        mario->hitobjStandOn = 0;
        mario->mot.jabara.dir = mario->displayDirectionTarget;
        mario->currSubMotionId = 0x14;
        break;
    case 0x14:
        marioMakeJumpPara();
        if (mario->hammerChargeLv == 3) {
            hitY = marioChkOverheadJabara(&overheadHit, &overheadDist);
        } else {
            hitY = marioChkOverhead();
        }
        if (mario->hitobjHead == NULL) {
            if (mario->jumpVelocityY <= 0.0f) {
                mario->currSubMotionId = 0x1e;
                marioSetPeakYpos();
            }
            mario->position.y += mario->jumpVelocityY;
            break;
        }
        if ((f32)hitY < mario->lastGroundPos.y) {
            marioPaperOff();
            {
                void* h = mario->hitobjStandOn;
                jabaraLand(mario->lastGroundPos.y, h);
            }
            camFollowYOn();
            marioGetPtr();
            allPartyRideOff();
            JAB_TAIL(MOTSUB_)
            return;
        }
        mario->position.y = hitY;
        if (mario->hammerChargeLv <= 2) {
            mario->currSubMotionId = 0x1e;
            break;
        }
        if (hitGetAttr(mario->hitobjHead) & 0x40000) {
            mario->mot.jabara.dir = mario->displayDirectionTarget;
            mario->displayDirection = mario->displayDirectionTarget;
            mario->currSubMotionId = 0x46;
            mario->multiTimer = 10;
            mario->mot.jabara.floorY = (s32)(hitY + mario->wPlayerCollisionBox.y);
            {
                mario->position.z = (s32)((100.0f * jabaraHitPosZ(mario->hitobjHead) + 0.5f) / 100.0f);
                mario->position.y = (f32)mario->mot.jabara.floorY - mario->wPlayerCollisionBox.y - 3.5f;
                mario->rotOfs.y = 9.0f + mario->wPlayerCollisionBox.y;
            }
            marioPaperOff();
            camFollowYOn();
            mario->dispFlags |= 4;
            marioChgPose("M_J_5A");
            SFX(0x186);
                jabaraSwingStart(overheadHit);
        } else {
            mario->currSubMotionId = 0x28;
        }
        break;
    case 0x1e:
        marioChgPose("M_J_1C");
        marioChgPaper("PM_J_1D");
        marioSetFallPara();
        mario->currSubMotionId = 0x1f;
    case 0x1f: {
        f32 spd = marioGetFallSpd();
        void* land;
        if (spd < -6.0f) {
            spd = -6.0f;
        }
        land = marioChkLandon(&hitY, spd);
        if (land == NULL) {
            mario->position.y += spd;
            break;
        }
        JAB_LAND(land, hitY)
        return;
    }
    }
    switch (mario->currSubMotionId) {
    case 0x28:
        if (strcmp(gp->currentMapName, "eki_02") == 0 && mario->position.x >= -450.0f && mario->position.x <= -190.0f) {
            mario->flags |= MARIO_FLAG_WALLS_DISABLED;
        }
        SFX(0x188);
        evtEntryType(quake_evt, 0, 0, 0);
        marioClearJumpPara();
        marioChgPose("M_J_5A");
        marioChgPaper("PM_J_1F");
        mario->mot.jabara.animFrame = 1;
        mario->poseTime = mario->mot.jabara.animFrame;
        mario->dispOfs.y = -8.0f;
        mario->multiTimer = 0x32;
        mario->currSubMotionId = 0x29;
        break;
    case 0x29:
        ANIM_STEP();
        mario->mot.jabara.animFrame++;
        if (mario->mot.jabara.animFrame >= 8) {
            mario->mot.jabara.animFrame = 8;
            mario->wPlayerCollisionBox.y = 4.0f;
        }
        if (--mario->multiTimer <= 0) {
            mario->currSubMotionId = 0x2a;
        }
        break;
    case 0x2a:
        mario->dispOfs.y += 2.0f;
        if (mario->dispOfs.y >= 0.0f) {
            mario->dispOfs.y = 0.0f;
        }
        ANIM_STEP();
        if (++mario->mot.jabara.animFrame >= 0x10) {
            mario->mot.jabara.animFrame = 0x10;
            marioChgPaper("PM_J_1G");
            marioPaperLightOff();
            mario->currSubMotionId = 0x2b;
            mario->multiTimer = 0x18;
        }
        break;
    case 0x2b:
        if (--mario->multiTimer > 0) {
            break;
        }
        marioChgPose("M_S_1");
        marioChgPaper("PM_J_1H");
        marioPaperLightOff();
        mario->currSubMotionId = 0x32;
        mario->mot.jabara.animFrame = 0;
        marioGetPtr()->motJabara->field_0xC = 0.0f;
    case 0x32: {
        BOOL changed;
        f32 f28;
        ANIM_STEP();
        changed = FALSE;
        if (mario->mot.jabara.animFrame >= 0x23 && mario->mot.jabara.animFrame <= 0x62) {
            f28 = (180.0f == mario->displayDirectionTarget) ? 0.6f : -0.6f;
            if (f28 != marioGetPtr()->motJabara->wobbleDir) {
                marioGetPtr()->motJabara->wobbleCount++;
                changed = TRUE;
            }
            marioGetPtr()->motJabara->wobbleDir = f28;
        } else {
            f28 = (180.0f == mario->displayDirectionTarget) ? -0.6f : 0.6f;
            if (f28 != marioGetPtr()->motJabara->wobbleDir) {
                marioGetPtr()->motJabara->wobbleCount++;
                changed = TRUE;
            }
            marioGetPtr()->motJabara->wobbleDir = f28;
        }
        if (changed) {
            if (marioGetPtr()->motJabara->wobbleCount == 1) {
                SFX(0x189);
            } else if (marioGetPtr()->motJabara->wobbleCount == 2) {
                SFX(0x18a);
            } else if (marioGetPtr()->motJabara->wobbleCount == 3) {
                SFX(0x18b);
            }
        }
        mario->dispOfs.x += f28 * (f32)cos(3.1415927f * mario->targetYaw / 180.0f);
        mario->dispOfs.z += f28 * (f32)sin(3.1415927f * mario->targetYaw / 180.0f);
        f28 = -1.0f * (f32)sin(1.5707963705062866);
        {
            HitEntry* land = marioChkLandon2(&hitY, f28, mario->dispOfs.y);
            if (land == NULL) {
                mario->position.y += f28;
                if ((f32)mario->screenY > 480.0f) {
                    camFollowYOn();
                }
                if (++mario->mot.jabara.animFrame > 0x78) {
                    mario->mot.jabara.animFrame = 0;
                }
                if (mario->buttonTrg & PAD_BUTTON_B) {
                    mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
                    mario->flags &= ~MARIO_FLAG_WALLS_DISABLED;
                    marioPaperOff();
                    marioGetPtr();
                    allPartyRideOff();
                    JAB_TAIL(MOTSUB_)
                    return;
                }
            } else {
                mario->hitobjStandOn = land;
                mario->hitobjJumpFrom = 0;
                mario->position.y = hitY;
                mario->currSubMotionId = 0x34;
                mario->mot.jabara.animFrame = 0;
                mario->multiTimer = 0x14;
            }
        }
        break;
    }
    case 0x34:
        ANIM_STEP();
        if (--mario->multiTimer <= 0) {
            mario->currSubMotionId = 0x3c;
        }
        break;
    }
    switch (mario->currSubMotionId) {
    case 0x3c:
        allPartyRideOff();
        mario->currSubMotionId = 0x3d;
        mario->hitobjJumpFrom = mario->hitobjStandOn;
        mario->hitobjStandOn = 0;
        mario->baseSpeed = 0.0f;
        mario->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        JAB_TAIL()
        marioPaperOff();
        camFollowYOff();
        marioChgSmallJumpMotion();
        mario->baseSpeed = 0.0f;
        return;
    }
    {
    s32 st = mario->currSubMotionId;
    if (st >= 0x3c && st < 0x46) {
        return;
    }
    switch (st) {
    case 0x46:
        if (mario->multiTimer-- <= 0) {
            marioChgPose("M_S_4");
            mario->currSubMotionId = 0x47;
        }
    case 0x47:
        if (mario->mot.jabara.state == 5 || mario->mot.jabara.state == 0xf) {
            if (mario->wStickDir1 != 0 || mario->wStickDir2 != 0) {
                mario->currSubMotionId = 0x50;
                mario->rot.x = 0.0f;
                mario->rotOfs.x = 0.0f;
                mario->mot.jabara.state = 0;
                mario->mot.jabara.rotSpeed = 0.0f;
                mario->position.y = (f32)mario->mot.jabara.floorY - mario->wPlayerCollisionBox.y - 3.5f;
                mario->rotOfs.y = 9.0f + mario->wPlayerCollisionBox.y;
            }
        }
        if (mario->mot.jabara.state == 0 || mario->mot.jabara.state == 0xa) {
            mario->currSubMotionId = 0x50;
            mario->rot.x = 0.0f;
            mario->rotOfs.x = 0.0f;
            mario->mot.jabara.state = 0;
            mario->mot.jabara.rotSpeed = 0.0f;
            mario->position.y = (f32)mario->mot.jabara.floorY - mario->wPlayerCollisionBox.y - 3.5f;
            mario->rotOfs.y = 9.0f + mario->wPlayerCollisionBox.y;
        }
        break;
    case 0x50:
        if (mario->buttonTrg & PAD_BUTTON_A) {
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
            mario->baseSpeed = 0.0f;
            mario->rot.x = 0.0f;
            mario->currSubMotionId = 0x5a;
        } else if (mario->buttonTrg & PAD_BUTTON_B) {
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
            marioGetPtr();
            allPartyRideOff();
            JAB_TAIL(MOTSUB_)
            return;
        } else {
            ANIM_STEP();
            v68 = mario->position;
            headRange = 10.0f + mario->wPlayerCollisionBox.y;
            rr = 1.0f;
            v68.x = v68.x + rr * (f32)sin(3.1415927f * (mario->controlStickAngle + mario->targetYaw) / 180.0f);
            v68.z = v68.z - rr * (f32)cos(3.1415927f * (mario->controlStickAngle + mario->targetYaw) / 180.0f);
            mario->hitobjHead = marioSearchHead_jabara(&v68, &headRange, &headHit, 0.0f);
            if (mario->hitobjHead == NULL) {
                headRange = 5.0f + mario->wPlayerCollisionBox.y;
                mario->hitobjHead = marioSearchHead(&v68, &headRange, 0.0f);
            }
            if (mario->controlStickSensitivity) {
                marioChgPose("M_W_6");
                    jabaraSwingWalk();
                {
                    BOOL hit = FALSE;
                    f32 f31 = 1.0f;
                    if (mario->hitobjHead != NULL && (hitGetAttr(mario->hitobjHead) & 0x40000)) {
                        hit = TRUE;
                    }
                    if (hit) {
                        if (jabaraHitFront(mario->controlStickAngle) == NULL) {
                            f31 = f31 * (f32)sin(3.1415927f * mario->controlStickAngle / 180.0f);
                            mario->position.x += f31;
                            if (f31 > 0.0f) {
                                if (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) {
                                    mario->mot.jabara.animFrame++;
                                    if (mario->mot.jabara.animFrame > 0x20) {
                                        mario->mot.jabara.animFrame = 0;
                                    }
                                } else {
                                    mario->mot.jabara.animFrame--;
                                    if (mario->mot.jabara.animFrame < 0) {
                                        mario->mot.jabara.animFrame = 0x20;
                                    }
                                }
                            } else {
                                if (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) {
                                    mario->mot.jabara.animFrame--;
                                    if (mario->mot.jabara.animFrame < 0) {
                                        mario->mot.jabara.animFrame = 0x20;
                                    }
                                } else {
                                    mario->mot.jabara.animFrame++;
                                    if (mario->mot.jabara.animFrame > 0x20) {
                                        mario->mot.jabara.animFrame = 0;
                                    }
                                }
                            }
                            if (mario->mot.jabara.animFrame == 0) {
                                SFX(0x187);
                            }
                            if (mario->mot.jabara.animFrame == 0x10) {
                                SFX(0x187);
                            }
                        }
                    }
                }
                marioUpdateCamCollision(mario->position);
            }
        }
        break;
    }
    }
    switch (mario->currSubMotionId) {
    case 0x5a:
        marioChgPose("M_J_1B");
        mario->currSubMotionId = 0x5b;
        mario->dispFlags |= 4;
        marioAdjustMoveDir();
        mario->jumpVelocityY = 4.0f;
        mario->jumpAccelerationY = -0.5f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        mario->currSubMotionId = 0x5b;
    case 0x5b:
        mario->position.y += mario->jumpVelocityY;
        marioMakeJumpPara();
        if (mario->jumpVelocityY <= 0.0f) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
        break;
    }
    if (mario->currSubMotionId >= 0x46) {
        swingMain();
    }
}

BOOL jabaraNoHitChk(void) {
    MarioWork* mario = marioGetPtr();
    if ((u16)mario->currMotionId != MARIO_MOTION_JABARA) {
        return FALSE;
    }
    if ((s32)mario->currSubMotionId >= 0x28 && (s32)mario->currSubMotionId < 0x32) {
        return TRUE;
    }
    return FALSE;
}

void mot_jabara_post(void) {
    MarioWork* mario = marioGetPtr();
    MarioWork* m2;
    mario->flags &= ~MARIO_FLAG_WALLS_DISABLED;
    m2 = marioGetPtr();
    m2->camShiftTargetZ = 0.0f;
    m2->camShiftRate.z = 0.01f;
    if ((u32)mario->dispFlags & 0x40000000) {
        marioPaperOff();
    }
    allPartyRideOff();
    jabaraResetPose();
    if (marioGetPtr()->motStruct) {
        __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
        marioGetPtr()->motStruct = NULL;
    }
    switch ((u16)mario->currMotionId) {
    case 9:
    default:
        marioClearJumpPara();
        break;
    case 3: case 4: case 5: case 6: case 7: case 8:
    case 10: case 11:
        break;
    }
}

void jabaraSwingStart(s32 s) {
    MarioWork* mp = marioGetPtr();
    mp->mot.jabara.rotTimer = 4;
    mp->mot.jabara.rotSpeed = 24.0f;
    marioGetPtr()->motJabara->swingAccel = 2.4f;
    marioGetPtr()->motJabara->swingDecel = -0.4f;
    marioGetPtr()->motJabara->swingRotStep = 0.18f;
    if (s < 0) {
        mp->mot.jabara.state = 0xb;
    } else {
        mp->mot.jabara.state = 1;
    }
}


void jabaraSwingWalk(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->mot.jabara.state != 0) {
    return;
    }
    mp->mot.jabara.state = 1;
    mp->mot.jabara.rotTimer = 10;
    mp->mot.jabara.rotSpeed = 0.0f;
    marioGetPtr()->motJabara->swingAccel = 0.1f;
    marioGetPtr()->motJabara->swingDecel = -0.1f;
    marioGetPtr()->motJabara->swingRotStep = 0.18f;
}

void swingMain(void) {
    MarioWork* m = marioGetPtr();
    if (marioGetPtr()->motStruct) {
        if ((s32)m->currSubMotionId < 0x5a && (m->buttonTrg & (PAD_BUTTON_A | PAD_BUTTON_B)) && !(marioGetPtr()->motJabara->flags & 1)) {
            m->mot.jabara.rotTimer = 0;
            marioGetPtr()->motJabara->flags |= 1;
        }
        switch (m->mot.jabara.state) {
        case 0:
            break;
        case 1:
            if (m->mot.jabara.rotTimer > 0) {
                m->mot.jabara.rotTimer--;
                m->mot.jabara.rotSpeed += marioGetPtr()->motJabara->swingAccel;
                if (m->mot.jabara.rotSpeed >= 40.0f) {
                    m->mot.jabara.rotSpeed = 40.0f;
                }
            } else if (m->mot.jabara.rotTimer == 0) {
                m->mot.jabara.rotSpeed += marioGetPtr()->motJabara->swingDecel;
                if (m->mot.jabara.rotSpeed <= 0.0f) {
                    m->mot.jabara.rotSpeed = 0.0f;
                    m->mot.jabara.rotAccel = 0.1f;
                    if (m->rot.x >= 180.0f) {
                        m->mot.jabara.state = 2;
                    } else {
                        m->mot.jabara.state = 4;
                    }
                }
            }
            if (marioGetPtr()->motJabara->flags & 1) {
                f32 old = m->rot.x;
                m->rot.x = revise360(old + m->mot.jabara.rotSpeed);
                if (m->rot.x < old) {
                    m->mot.jabara.rotSpeed = 1.4f;
                    m->mot.jabara.rotAccel = 0.1f;
                    if (m->rot.x >= 180.0f) {
                        m->mot.jabara.state = 2;
                    } else {
                        m->mot.jabara.state = 4;
                    }
                }
            } else {
                m->rot.x = revise360(m->rot.x + m->mot.jabara.rotSpeed);
            }
            break;
        default:
            break;
        }
        switch (m->mot.jabara.state) {
        case 2:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed + m->mot.jabara.rotAccel;
            m->rot.x = revise360(m->rot.x + m->mot.jabara.rotSpeed);
            if (m->rot.x < 180.0f) {
                m->mot.jabara.state = 3;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel + marioGetPtr()->motJabara->swingRotStep;
            }
            break;
        case 3:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed - m->mot.jabara.rotAccel;
            if (m->mot.jabara.rotSpeed <= 0.0f) {
                m->mot.jabara.rotSpeed = 0.0f;
                m->mot.jabara.state = 4;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel - marioGetPtr()->motJabara->swingRotStep;
                if (__fabsf(m->rot.x) < 0.8f) {
                    m->mot.jabara.state = 0;
                }
            }
            m->rot.x = revise360(m->rot.x + m->mot.jabara.rotSpeed);
            break;
        case 4:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed - m->mot.jabara.rotAccel;
            m->rot.x = revise360(m->rot.x + m->mot.jabara.rotSpeed);
            if (m->rot.x > 180.0f) {
                m->mot.jabara.state = 5;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel + marioGetPtr()->motJabara->swingRotStep;
            }
            break;
        case 5:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed + m->mot.jabara.rotAccel;
            if (m->mot.jabara.rotSpeed >= 0.0f) {
                m->mot.jabara.rotSpeed = 0.0f;
                m->mot.jabara.state = 2;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel - marioGetPtr()->motJabara->swingRotStep;
                if (__fabsf(m->rot.x) < 0.8f) {
                    m->mot.jabara.state = 0;
                }
            }
            m->rot.x = revise360(m->rot.x + m->mot.jabara.rotSpeed);
            break;
        }
        switch (m->mot.jabara.state) {
        case 0xa:
            break;
        case 0xb:
            if (m->mot.jabara.rotTimer > 0) {
                m->mot.jabara.rotTimer--;
                m->mot.jabara.rotSpeed += marioGetPtr()->motJabara->swingAccel;
                if (m->mot.jabara.rotSpeed >= 40.0f) {
                    m->mot.jabara.rotSpeed = 40.0f;
                }
            } else if (m->mot.jabara.rotTimer == 0) {
                m->mot.jabara.rotSpeed += marioGetPtr()->motJabara->swingDecel;
                if (m->mot.jabara.rotSpeed <= 0.0f) {
                    m->mot.jabara.rotSpeed = 1.4f;
                    m->mot.jabara.rotAccel = 0.1f;
                    if (m->rot.x >= 180.0f) {
                        m->mot.jabara.state = 0xc;
                    } else {
                        m->mot.jabara.state = 0xe;
                    }
                }
            }
            if (marioGetPtr()->motJabara->flags & 1) {
                f32 old = m->rot.x;
                m->rot.x = revise360(old - m->mot.jabara.rotSpeed);
                if (m->rot.x > old) {
                    m->mot.jabara.rotSpeed = 5.0f;
                    m->mot.jabara.rotAccel = 0.1f;
                    if (m->rot.x >= 180.0f) {
                        m->mot.jabara.state = 0xc;
                    } else {
                        m->mot.jabara.state = 0xe;
                    }
                }
            } else {
                m->rot.x = revise360(m->rot.x - m->mot.jabara.rotSpeed);
            }
            break;
        }
        switch (m->mot.jabara.state) {
        case 0xc:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed + m->mot.jabara.rotAccel;
            m->rot.x = revise360(m->rot.x - m->mot.jabara.rotSpeed);
            if (m->rot.x > 180.0f) {
                m->mot.jabara.state = 0xd;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel + marioGetPtr()->motJabara->swingRotStep;
            }
            break;
        case 0xd:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed - m->mot.jabara.rotAccel;
            if (m->mot.jabara.rotSpeed <= 0.0f) {
                m->mot.jabara.rotSpeed = 0.0f;
                m->mot.jabara.state = 0xe;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel - marioGetPtr()->motJabara->swingRotStep;
                if (__fabsf(m->rot.x) < 0.8f) {
                    m->mot.jabara.state = 0;
                }
            }
            m->rot.x = revise360(m->rot.x - m->mot.jabara.rotSpeed);
            break;
        case 0xe:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed - m->mot.jabara.rotAccel;
            m->rot.x = revise360(m->rot.x - m->mot.jabara.rotSpeed);
            if (m->rot.x < 180.0f) {
                m->mot.jabara.state = 0xf;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel + marioGetPtr()->motJabara->swingRotStep;
            }
            break;
        case 0xf:
            m->mot.jabara.rotSpeed = m->mot.jabara.rotSpeed + m->mot.jabara.rotAccel;
            if (m->mot.jabara.rotSpeed >= 0.0f) {
                m->mot.jabara.rotSpeed = 0.0f;
                m->mot.jabara.state = 0xc;
                m->mot.jabara.rotAccel = m->mot.jabara.rotAccel - marioGetPtr()->motJabara->swingRotStep;
                if (__fabsf(m->rot.x) < 0.8f) {
                    m->mot.jabara.state = 0xa;
                }
            }
            m->rot.x = revise360(m->rot.x - m->mot.jabara.rotSpeed);
            break;
        }
    }
}

void mot_grasp(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
    }
}
