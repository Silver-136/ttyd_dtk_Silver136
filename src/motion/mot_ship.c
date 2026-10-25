#include "dolphin/types.h"
#include "evt/evt_cmd.h"
#include "evt/evt_snd.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_sbr.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_funemizu.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "manager/evtmgr.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "mario/mariost.h"
#include <dolphin/pad.h>

typedef struct MotShipWork {
    u32 flags;                    // 0x00: bit0 = facing (SHIP_DIR), bit1 = N_marioShipGetDir; set to 0/1/2 by shipDecideDir
    f32 baseY;                    // 0x04: y the hull floats at; copied from harborPos.y when docking
    f32 bobAngle;                 // 0x08: bob phase in degrees, dispOfs.y = 3 sin(bobAngle) - 13
    f32 bobSpeed;                 // 0x0C: 4.25 degrees per frame
    f32 wakeScale;                // 0x10: wake effect scale, eased to 0.3 / 0.6
    f32 dispX;                    // 0x14: drift velocity x (shipMove)
    f32 dispY;                    // 0x18: drift velocity z (shipMove)
    f32 turnSpeed;                // 0x1C: added to displayDirection every frame
    f32 turnAccel;                // 0x20
    struct EffectEntry* wakeEff;  // 0x24: effFunemizuEntry
    f32 swirlY;                   // 0x28: whirlpool push (N_marioShipSwirl)
    f32 swirlX;                   // 0x2C
    s32 swirlTime;                // 0x30: frames of whirlpool push left
    u8 field_0x34[0x38 - 0x34];   // 0x34
    s32 sfxId;                    // 0x38: engine loop, -1 when off
    Vec harborPos;                // 0x3C: docking target (checkHarbor)
} MotShipWork;                    // size 0x48

static EVT_BEGIN(sound_evt)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1_1"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1_1"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1_1"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1_1"), LW(0), LW(1), LW(2), 0)
RETURN
EVT_END()

EVT_BEGIN(sound_evt2)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP2"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP2"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP2"), LW(0), LW(1), LW(2), 0)
WAIT_FRAMES(12)
CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP2"), LW(0), LW(1), LW(2), 0)
RETURN
EVT_END()

f32 shipData[6] = {-0.05f, -0.7f, 0.02f, 0.02f, 2.0f, 0.01f};
const char* paper_ship[4] = {"p_hune", "p_hune_g", "p_hune_y", "p_hune_v"};


static inline s32 harborKind(HitEntry* hit) {
    s32 k = 0;
    s32 attr = hitGetAttr(hit);
    if (attr & 0x4000) {
        k = 1;
    } else if (attr & 0x8000) {
        k = 2;
    } else if (attr & 0x10000) {
        k = 3;
    } else if (attr & 0x20000) {
        k = 4;
    }
    return k;
}

#define HARBOR_PROBE(ANG, POS)  sincosf(&sn, &cs, toMovedir(ANG)); tx = 40.0f * sn + m->position.x; tz = 40.0f * cs + m->position.z; ty = 20.0f + m->position.y; dist = 40.0f; hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, tx, ty, tz, 0.0f, -1.0f, 0.0f); if (hit != NULL && harborKind(hit) != 0) { hitObjGetPos(hitGetName(hit), &POS); ix = (s32)((100.0f * POS.x + 0.5f) / 100.0f); iy = (s32)((100.0f * POS.y + 0.5f) / 100.0f); iz = (s32)((100.0f * POS.z + 0.5f) / 100.0f); out->x = ix; out->y = iy; out->z = iz; return TRUE; }

BOOL checkHarbor(Vec* out);


void allPartyRideOff(void);
void allPartyForceRideOn(void);
void* effFunemizuEntry(s32, f32, f32, f32, f32, f32);

void marioPreJump(void);
void allPartyRideShip(void);
void marioClearJumpPara(void);
void N_marioReloadMapOnBottomlessOff(void);

double __fabs(double);
HitEntry* marioChkLandon(f32, f32*);

#define SHIP_DIR (marioGetPtr()->motShip->flags & 1)

void marioCheckWallShip(f32, f32);
double sin(double);
double cos(double);
void shipMove(void);
#define EFFP() ((EffFunemizuWork*)marioGetPtr()->motShip->wakeEff->userdata)
s32 N_marioShipGetDir(void) {
    marioGetPtr();
    if (marioGetPtr()->motStruct != NULL) {
        return (marioGetPtr()->motShip->flags >> 1) & 1;
    }
    return 0;
}

s32 marioShipChanging(void) {
    MarioWork* m = marioGetPtr();
    if (m->currMotionId != MARIO_MOTION_SHIP) {
        return 0;
    }
    if ((s32)m->currSubMotionId == 10) {
        return 2;
    }
    if ((s32)m->currSubMotionId >= 0 && (s32)m->currSubMotionId <= 12 || (s32)m->currSubMotionId >= 30 && (s32)m->currSubMotionId <= 48) {
        return 1;
    }
    if ((s32)m->currSubMotionId == 50 || (s32)m->currSubMotionId == 52) {
        return 3;
    }
    return 0;
}

void marioSetSwirlPower(s32 msec, f32 a, f32 b) {
    s32 t;
    marioGetPtr();
    t = sysMsec2Frame(msec);
    marioGetPtr()->motShip->swirlTime = t;
    marioGetPtr()->motShip->swirlX = a;
    marioGetPtr()->motShip->swirlY = b;
}

BOOL marioChkSwirlPower(void) {
    marioGetPtr();
    return marioGetPtr()->motShip->swirlTime != 0;
}

void marioShipForceStop(void) {
    f32 a = 0.0f;
    f32 b;
    marioGetPtr();
    b = 0.0f;
    marioGetPtr()->motShip->dispX = a;
    marioGetPtr()->motShip->dispY = b;
}

void marioShipSetDispDir(void) {
    f32 a = 0.0f;
    f32 b;
    marioGetPtr();
    b = 0.0f;
    marioGetPtr()->motShip->dispX = a;
    marioGetPtr()->motShip->dispY = b;
}

BOOL marioChkShipPlace(HitEntry* hit) {
    return (hitGetAttr(hit) & 0x3C000) != 0;
}

void marioForceShipAnime(void) {
    MarioWork* m = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn(paper_ship[marioGetColor()]);
    marioChgPaper("PM_H_1B");
    marioChgPose("M_Z_1");
    m->dispFlags |= 8;
}

void marioReInit_ship(void) {
    MarioWork* m = marioGetPtr();
    void* p;
    s32 t, u;
    f32 a, b;
    m->trigFlags &= ~1;
    p = __memAlloc(HEAP_DEFAULT, 0x48);
    marioGetPtr()->motStruct = p;
    memset(marioGetPtr()->motStruct, 0, 0x48);
    m->currMotionId = 0x19;
    m->dispOfs.y = -15.0f;
    m->currSubMotionId = 0x32;
    m->wPlayerCollisionBox.x = 40.0f;
    m->wPlayerCollisionBox.y = 40.0f;
    marioPaperOn(paper_ship[marioGetColor()]);
    marioChgPaper("PM_H_1A");
    marioChgPose("M_Z_1");
    m->dispFlags |= 8;
    m->mot.ship.animTime = 0x58;
    marioSetPaperAnimeLocalTime(m->mot.ship.animTime);
    t = -1;
    marioGetPtr()->motShip->sfxId = t;
    u = 0;
    marioGetPtr()->motShip->flags = u;
    a = 0.0f;
    marioGetPtr()->motShip->baseY = a;
    b = 0.0f;
    marioGetPtr()->motShip->dispX = b;
    a = 0.0f;
    marioGetPtr()->motShip->dispY = a;
    b = 0.0f;
    marioGetPtr()->motShip->turnSpeed = b;
    a = 0.0f;
    marioGetPtr()->motShip->turnAccel = a;
    b = 0.0f;
    marioGetPtr()->motShip->bobAngle = b;
    a = 4.25f;
    marioGetPtr()->motShip->bobSpeed = a;
    marioGetPtr()->motShip->wakeEff = effFunemizuEntry(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    u = m->camId;
    ((EffFunemizuWork*)marioGetPtr()->motShip->wakeEff->userdata)->cam = u;
    allPartyForceRideOn();
}

void mot_ship_post(void) {
    MarioWork* m = marioGetPtr();
    s32 t;
    allPartyRideOff();
    if (m->dispFlags & 0x40000000) {
        marioPaperOff();
    }
    m->wPlayerCollisionBox.x = 20.0f;
    m->wPlayerCollisionBox.y = 37.0f;
    marioGetPtr();
    if (marioGetPtr()->motStruct != NULL) {
        if ((u32)marioGetPtr()->motShip->sfxId != 0xFFFFFFFFU) {
            psndSFXOff(marioGetPtr()->motShip->sfxId);
            t = -1;
            marioGetPtr()->motShip->sfxId = t;
        }
    }
    m->dispFlags &= ~4;
    m->dispFlags &= ~0x100;
    m->dispFlags &= ~8;
    marioOfsRotReset();
    if (marioGetPtr()->motStruct != NULL) {
        __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
        marioGetPtr()->motStruct = NULL;
    }
}

static inline void shipLerpWake(f32 t, f32 k) {
    marioGetPtr()->motShip->wakeScale += k * (t - marioGetPtr()->motShip->wakeScale);
}

static inline void shipLerpTurn(f32 t, f32 k) {
    marioGetPtr()->motShip->turnSpeed += k * (t - marioGetPtr()->motShip->turnSpeed);
}

static inline void shipEff(f32 s) {
    MarioWork* mm = marioGetPtr();
    f32 zz = 0.0f;
    if (!s) {
        s32 c;
        marioGetPtr();
        c = (marioGetPtr()->motShip->bobAngle > 180.0f) ? 0 : 1;
        s = c ? 0.3f : 0.6f;
        shipLerpWake(s, 0.1f);
        s = marioGetPtr()->motShip->wakeScale;
    }
    EFFP()->position.x = mm->position.x;
    EFFP()->position.y = mm->position.y + zz;
    EFFP()->position.z = mm->position.z;
    EFFP()->f10 = s;
    EFFP()->f14 = mm->displayDirection;
}

// FAKE: case-0 block as an inline helper so hit/attr/r are inline temps numbered
// above harborKind's k (register pick only; same behaviour as the inline block).
static inline void shipDecideDir(MarioWork* m, Vec* pp1, Vec* pp2) {
    MarioWork* m2;
    s32 r;
    f32 ff;
        {
            HitEntry* hit = (HitEntry*)m->hitobjJumpFrom;
            s32 attr;
            m2 = marioGetPtr();
            attr = hitGetAttr(hit);
            r = 0;
            if (attr & 0x4000) {
                hitObjGetPos(hitGetName((HitEntry*)m2->hitobjJumpFrom), pp1);
                ff = revise360(angleABf(m2->position.x, m2->position.z, pp1->x, pp1->z) - m2->targetYaw);
                r = (ff >= 180.0f) ? 2 : 1;
            } else if (attr & 0x8000) {
                hitObjGetPos(hitGetName((HitEntry*)m2->hitobjJumpFrom), pp2);
                ff = revise360(angleABf(m2->position.x, m2->position.z, pp2->x, pp2->z) - m2->targetYaw);
                r = (ff >= 180.0f) ? 2 : 1;
            } else if (attr & 0x10000) {
                r = 2;
            } else if (attr & 0x20000) {
                r = 1;
            }
            marioGetPtr()->motShip->flags = r;
        }
}

void mot_ship(void) {
    MarioWork* m = marioGetPtr();
    MotShipWork* newWork;
    MotShipWork* ms;
    s32 r29, r27;
    f32 f30, f31;
    Vec p2;
    Vec p1;
    Vec pos;

    if (m->trigFlags & 1) {
        m->trigFlags &= ~1;
        m->flags &= ~0xF0000;
        newWork = __memAlloc(0, 0x48);
        marioGetPtr()->motShip = newWork;
        memset(marioGetPtr()->motStruct, 0, 0x48);
        marioGetPtr()->motShip->sfxId = -1;
        marioGetPtr()->motShip->turnAccel = 0.0f;
        marioGetPtr()->motShip->bobAngle = 0.0f;
        marioGetPtr()->motShip->bobSpeed = 4.25f;
        m->currSubMotionId = 0;
        if (!(m->trigFlags & 4)) {
            camGetPtr(8)->flags |= 0x200;
        }
    }
    switch (m->currSubMotionId) {
    case 0:
        psndSFXOn_3D((const char*)0x18d, &m->position);
        psndSFXOn_3D((const char*)0xaf, &m->position);
        marioCamZoomUp();
        marioPreJump();
        marioChgPose("M_J_1B");
        camFollowYOff();
        m->dispFlags |= 0x100;
        m->multiTimer = 0;
        m->currSubMotionId = 1;
        m->airTimer = 0;
        m->jumpPeakAirTime = 0;
        m->baseSpeed = 0.0f;
        marioGetPtr()->motShip->turnSpeed = 0.0f;
        N_marioSetBottomlessRespawnPosOnBeroEntry(m->position.x, m->position.y, m->position.z);
        shipDecideDir(m, &p1, &p2);
        /* fallthrough */
    case 1:
        marioMakeJumpPara();
        f31 = SHIP_DIR ? 30.0f : -30.0f;
        shipLerpTurn(f31, 0.125f);
        if (SHIP_DIR) {
            f30 = 30.0f;
            if (marioGetPtr()->motShip->turnSpeed >= f30) {
                marioGetPtr()->motShip->turnSpeed = f30;
            }
        } else {
            f30 = -30.0f;
            if (marioGetPtr()->motShip->turnSpeed <= f30) {
                marioGetPtr()->motShip->turnSpeed = f30;
            }
        }
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        r27 = 0;
        if (SHIP_DIR) {
            if (m->displayDirection >= 270.0f) {
                r27 = 1;
            }
        } else {
            if (m->displayDirection <= -270.0f) {
                r27 = 1;
            }
        }
        if (r27) {
            m->currSubMotionId = 2;
            m->multiTimer = 20;
            marioPaperOn(paper_ship[marioGetColor()]);
            marioChgPaper("PM_H_1A");
            marioChgPose("M_Z_1");
            f30 = 15.0f;
            m->dispFlags |= 8;
            marioGetPtr()->motShip->turnAccel = f30;
            {
                EventEntry* e = evtEntry(sound_evt, 0, 0);
                e->lwData[0] = (s32)(1024.0f * m->position.x) - 230000000;
                e->lwData[1] = (s32)(1024.0f * m->position.y) - 230000000;
                e->lwData[2] = (s32)(1024.0f * m->position.z) - 230000000;
            }
        }
        if (m->jumpVelocityY > 0.0f) {
            m->position.y += m->jumpVelocityY;
            m->flags &= ~0xF0000;
        }
        break;
    case 2:
        f31 = marioGetPtr()->motShip->turnAccel * (SHIP_DIR ? 1.0f : -1.0f);
        shipLerpTurn(f31, 0.125f);
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        r27 = 0;
        if (SHIP_DIR) {
            if (m->displayDirection >= 1260.0f) {
                r27 = 1;
            }
        } else {
            if (m->displayDirection <= -1080.0f) {
                r27 = 1;
            }
        }
        if (r27) {
            m->multiTimer = 40;
            m->currSubMotionId = 3;
        }
        break;
    case 3:
        if ((s32)m->multiTimer == 14) {
            allPartyRideShip();
        }
        if ((s32)m->multiTimer == 20) {
            marioCamZoomOffReq2(900);
            camFollowYOn();
        }
        if (--m->multiTimer <= 0) {
            m->currSubMotionId = 4;
        }
        if (m->dispOfs.y > -15.0f) {
            m->dispOfs.y -= 1.0f;
            if (m->dispOfs.y <= -15.0f) {
                m->dispOfs.y = -15.0f;
            }
        }
    case 4:
        f31 = marioGetPtr()->motShip->turnAccel * (SHIP_DIR ? 1.0f : -1.0f);
        shipLerpTurn(f31, 0.125f);
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        marioGetPtr()->motShip->turnAccel -= 0.1f;
        f30 = 1.0f;
        if (marioGetPtr()->motShip->turnAccel <= f30) {
            marioGetPtr()->motShip->turnAccel = f30;
        }
        r27 = 0;
        if (SHIP_DIR) {
            if (m->displayDirection >= 1620.0f) {
                m->displayDirection = 1620.0f;
                r27 = 1;
            }
        } else {
            if (m->displayDirection <= -1440.0f) {
                m->displayDirection = -1440.0f;
                r27 = 1;
            }
        }
        if (r27) {
            if ((s32)m->currSubMotionId == 4) {
                m->currSubMotionId = 10;
            }
        }
        break;
    case 10: {
        MarioWork* mm;
        f32 fz, fx;

        HitEntry* hit;
        s32 attr;
        m->dispFlags &= ~0x100;
        m->displayDirection = revise360(m->displayDirection);
        m->wPlayerCollisionBox.x = 40.0f;
        m->wPlayerCollisionBox.y = 40.0f;
        // FAKE: motStruct derived back from &field (same pointer) so ms is a later-numbered codegen value (target r28)
        ms = (MotShipWork*)((u8*)&marioGetPtr()->motShip->harborPos - offsetof(MotShipWork, harborPos));
        hit = (HitEntry*)m->hitobjJumpFrom;
        mm = marioGetPtr();
        hitObjGetPos(hitGetName(hit), &pos);
        fx = (s32)((100.0f * pos.x + 0.5f) / 100.0f);
        fz = (s32)((100.0f * pos.z + 0.5f) / 100.0f);
        ms->harborPos.y = mm->position.y - 15.0f;
        r27 = harborKind((HitEntry*)mm->hitobjJumpFrom);
        if (r27 == 1) {
            ms->harborPos.x = fx;
            ms->harborPos.z = fz - 50.0f - 30.30303f;
        } else if (r27 == 2) {
            ms->harborPos.x = fx;
            ms->harborPos.z = 30.30303f + (50.0f + fz);
        } else if (r27 == 3) {
            ms->harborPos.x = fx - 50.0f - 30.30303f;
            ms->harborPos.z = fz;
        } else if (r27 == 4) {
            ms->harborPos.x = 30.30303f + (50.0f + fx);
            ms->harborPos.z = fz;
        } else {
            ms->harborPos.x = fx;
            ms->harborPos.z = fz;
        }
        f30 = 0.1f * distABf(marioGetPtr()->motShip->harborPos.x, marioGetPtr()->motShip->harborPos.z, m->position.x, m->position.z);
        switch (r27) {
        case 1: marioGetPtr()->motShip->harborPos.z -= f30; break;
        case 2: marioGetPtr()->motShip->harborPos.z += f30; break;
        case 3: marioGetPtr()->motShip->harborPos.x -= f30; break;
        case 4: marioGetPtr()->motShip->harborPos.x += f30; break;
        }
        m->baseSpeed = distABf(m->position.x, m->position.z, marioGetPtr()->motShip->harborPos.x, marioGetPtr()->motShip->harborPos.z) / 36.0f;
        m->directionView = angleABf(m->position.x, m->position.z, marioGetPtr()->motShip->harborPos.x, marioGetPtr()->motShip->harborPos.z);
        m->moveDirection = m->directionView;
        marioGetPtr()->motShip->baseY = marioGetPtr()->motShip->harborPos.y;
        {
            f32 dd = marioGetPtr()->motShip->harborPos.y - m->position.y;
            m->jumpVelocityY = 0.0f;
            m->jumpAccelerationY = (dd + dd) / 306.0f;
        }
        m->jumpJerkY = 0.0f;
        m->jumpSnapY = 0.0f;
        m->multiTimer = 36;
        m->currSubMotionId = 11;
        break;
    }
    case 11: {
        s32 ok;
        HitEntry* land;
        f32 ly;
        MarioWork* mm;
        movePos(&m->position.x, &m->position.z, m->baseSpeed, m->directionView);
        m->position.y += m->jumpVelocityY;
        marioMakeJumpPara();
        f30 = m->jumpVelocityY;
        mm = marioGetPtr();
        mm->wPlayerCollisionBox.x = 20.0f;
        land = marioChkLandon(f30, &ly);
        mm->wPlayerCollisionBox.x = 40.0f;
        {
            f32 d;
            if (land && ((d = mm->position.y - ly) <= 0.0f || d + f30 <= 0.0f)) {
                mm->hitobjStandOn = land;
                mm->position.y = ly;
                mm->flags &= ~0xF0000;
                mm->jumpDistance = __fabsf(distABf(mm->position.x, mm->position.z, mm->lastGroundPos.x, mm->lastGroundPos.z));
                mm->flags &= ~0x80;
                mm->airTimer = 0;
                mm->jumpPeakAirTime = 0;
                camFollowYOn();
                ok = 1;
            } else {
                ok = 0;
            }
        }
        if (ok) {
            f30 = 0.0f;
            m->jumpVelocityY = f30;
            m->jumpAccelerationY = f30;
            m->jumpJerkY = f30;
            m->jumpSnapY = f30;
            m->baseSpeed = f30;
            marioGetPtr()->motShip->turnSpeed = f30;
            m->flags &= ~0xF0000;
            m->displayDirection = revise360(m->displayDirection);
            m->displayDirectionTarget = m->displayDirection;
            m->directionView = toMovedir(m->displayDirection);
            m->moveDirection = m->directionView;
            m->dispFlags &= ~0x100;
            m->dispFlags |= 0x1000;
            psndSFXOn_3D((const char*)0x18f, &m->position);
            marioGetPtr()->motShip->wakeEff = effFunemizuEntry(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            ((EffFunemizuWork*)marioGetPtr()->motShip->wakeEff->userdata)->cam = m->camId;
            marioGetPtr()->motShip->bobAngle = 0.0f;
            marioGetPtr()->motShip->bobSpeed = 4.25f;
            m->currSubMotionId = 50;
            if (!(m->trigFlags & 4)) {
                camGetPtr(8)->flags &= ~0x200;
            }
        }
        break;
    }
    case 30:
        if (!(m->trigFlags & 4)) {
            camGetPtr(8)->flags |= 0x200;
        }
        m->currSubMotionId = 31;
        /* fallthrough */
    case 31: {
        m->dispOfs.y += -0.4f;
        if (m->dispOfs.y <= -18.0f) {
            m->multiTimer = 4;
            m->currSubMotionId = 32;
        }
        shipEff(1.6f);
        break;
    }
    case 32:
        if (--m->multiTimer > 0) {
            break;
        }
        m->currSubMotionId = 40;
        /* fallthrough */
    case 40: {
        f32 tt, qq;
        s32 n;
        MotShipWork* ms2;
        marioGetPtr();
        if (marioGetPtr()->motStruct != NULL) {
            if ((u32)marioGetPtr()->motShip->sfxId != 0xFFFFFFFFU) {
                psndSFXOff(marioGetPtr()->motShip->sfxId);
                r29 = -1;
                marioGetPtr()->motShip->sfxId = r29;
            }
        }
        camFollowYOff();
        m->dispFlags &= ~0x1000;
        marioGetPtr()->motShip->baseY = marioGetPtr()->motShip->harborPos.y;
        m->baseSpeed = 2.8f;
        ms2 = (MotShipWork*)((u8*)&marioGetPtr()->motShip->harborPos.z - offsetof(MotShipWork, harborPos.z)); // FAKE: same derive trick as case 10
        n = (s32)((10.0f * (2.0f * distABf(m->position.x, m->position.z, marioGetPtr()->motShip->harborPos.x, ms2->harborPos.z) / m->baseSpeed) + 0.5f) / 10.0f);
        ms2 = (MotShipWork*)((u8*)&marioGetPtr()->motShip->harborPos.z - offsetof(MotShipWork, harborPos.z)); // FAKE: same derive trick as case 10
        m->directionView = angleABf(m->position.x, m->position.z, marioGetPtr()->motShip->harborPos.x, ms2->harborPos.z);
        m->moveDirection = m->directionView;
        r29 = ((m->trigFlags >> 9) & 1) + 1;
        marioGetPtr()->motShip->flags = r29;
        tt = 0.5f * (f32)n;
        qq = 80.0f / (tt * tt - tt);
        f30 = 0.0f;
        m->jumpVelocityY = qq * tt;
        m->jumpAccelerationY = -qq;
        m->jumpJerkY = f30;
        m->jumpSnapY = f30;
        m->multiTimer = (s16)(s32)(0.5f + tt);
        m->currSubMotionId = 0x29;
        m->mot.ship.dispStepY = __fabsf(m->dispOfs.y / (f32)((s32)m->multiTimer + 0x58));
        effDelete((EffectEntry*)marioGetPtr()->motShip->wakeEff);
        break;
    }
    case 41:
        movePos(&m->position.x, &m->position.z, m->baseSpeed, m->directionView);
        m->position.y += m->jumpVelocityY;
        marioMakeJumpPara();
        m->dispOfs.y += m->mot.ship.dispStepY;
        f30 = 0.0f;
        if (m->dispOfs.y > f30) {
            m->dispOfs.y = f30;
        }
        if (--m->multiTimer > 0) {
            break;
        }
        marioChgPaper("PM_H_1A");
        marioChgPose("M_Z_1");
        m->mot.ship.animTime = 0x58;
        marioSetPaperAnimeLocalTime(m->mot.ship.animTime);
        f31 = 0.0f;
        marioGetPtr()->motShip->turnSpeed = f31;
        m->dispFlags |= 0x100;
        marioAdjustMoveDir();
        m->currSubMotionId = 0x2b;
        psndSFXOn_3D((const char*)0x192, &m->position);
        {
            EventEntry* e = evtEntry(sound_evt2, 0, 0);
            e->lwData[0] = (s32)(1024.0f * m->position.x) - 230000000;
            e->lwData[1] = (s32)(1024.0f * m->position.y) - 230000000;
            e->lwData[2] = (s32)(1024.0f * m->position.z) - 230000000;
        }
        break;
    case 43:
        m->dispOfs.y += m->mot.ship.dispStepY;
        f30 = 0.0f;
        if (m->dispOfs.y > f30) {
            m->dispOfs.y = f30;
        }
        marioSetPaperAnimeLocalTime(m->mot.ship.animTime);
        if ((m->mot.ship.animTime = m->mot.ship.animTime - 1) < 0x2c) {
            break;
        }
        f30 = SHIP_DIR ? 20.0f : -20.0f;
        shipLerpTurn(f30, 0.125f);
        if (SHIP_DIR) {
            f31 = 20.0f;
            if (marioGetPtr()->motShip->turnSpeed >= f31) {
                marioGetPtr()->motShip->turnSpeed = f31;
            }
        } else {
            f31 = -20.0f;
            if (marioGetPtr()->motShip->turnSpeed <= f31) {
                marioGetPtr()->motShip->turnSpeed = f31;
            }
        }
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        if (m->mot.ship.animTime == 0x2c) {
            m->currSubMotionId = 0x2c;
        }
        break;
    case 44:
        m->dispOfs.y += m->mot.ship.dispStepY;
        f30 = 0.0f;
        if (m->dispOfs.y > f30) {
            m->dispOfs.y = f30;
        }
        marioSetPaperAnimeLocalTime(m->mot.ship.animTime);
        if ((m->mot.ship.animTime = m->mot.ship.animTime - 1) <= 0) {
            m->mot.ship.animTime = 0;
            m->currSubMotionId = 0x2d;
            m->multiTimer = 6;
            m->displayDirection = revise360(m->displayDirection);
            if (marioGetPtr()->motShip->flags & 2) {
                m->displayDirection = m->displayDirection - 360.0f;
            }
        }
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        break;
    case 45:
        marioSetPaperAnimeLocalTime(m->mot.ship.animTime);
        f30 = marioGetPtr()->motShip->turnAccel * (SHIP_DIR ? 1.0f : -1.0f);
        shipLerpTurn(f30, 0.05f);
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        marioGetPtr()->motShip->turnAccel += -0.005f;
        f31 = 10.0f;
        if (marioGetPtr()->motShip->turnAccel <= f31) {
            marioGetPtr()->motShip->turnAccel = f31;
        }
        if (--m->multiTimer > 0) {
            break;
        }
    case 46:
        f30 = marioGetPtr()->motShip->turnAccel * (SHIP_DIR ? 1.0f : -1.0f);
        shipLerpTurn(f30, 0.05f);
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        marioGetPtr()->motShip->turnAccel += -0.005f;
        f31 = 10.0f;
        if (marioGetPtr()->motShip->turnAccel <= f31) {
            marioGetPtr()->motShip->turnAccel = f31;
        }
        r27 = 0;
        if (SHIP_DIR) {
            if (m->displayDirection >= 360.0f) {
                m->displayDirection = revise360(m->displayDirection);
                r27 = 1;
            }
        } else {
            if (m->displayDirection <= -360.0f) {
                m->displayDirection = revise360(m->displayDirection);
                r27 = 1;
            }
        }
        if (!r27) {
            break;
        }
        m->displayDirection = revise360(m->displayDirection);
        if (marioGetPtr()->motShip->flags & 2) {
            m->displayDirection = m->displayDirection - 360.0f;
        }
        marioPaperOff();
        m->dispFlags &= ~8;
        marioChgPose("M_J_1B");
        m->currSubMotionId = 0x2f;
        f31 = 10.0f;
        marioGetPtr()->motShip->turnAccel = f31;
    case 47:
        r27 = 0;
        marioGetPtr();
        marioGetPtr();
        f30 = SHIP_DIR ? 4.0f : -4.0f;
        shipLerpTurn(f30, 0.05f);
        m->displayDirection += marioGetPtr()->motShip->turnSpeed;
        if (SHIP_DIR) {
            if (m->displayDirection >= 540.0f) {
                m->displayDirection = 540.0f;
                r27 = 1;
            }
        } else {
            if (m->displayDirection <= -360.0f) {
                m->displayDirection = -360.0f;
                r27 = 1;
            }
        }
        if ((s32)m->currSubMotionId != 0x2f || !r27) {
            break;
        }
        m->displayDirection = revise360(m->displayDirection);
        m->displayDirectionTarget = m->displayDirection;
        m->moveDirection = toMovedir(m->displayDirection);
        m->directionView = m->moveDirection;
        m->dispFlags &= ~0x100;
        m->wPlayerCollisionBox.x = 20.0f;
        m->wPlayerCollisionBox.y = 37.0f;
        f30 = 0.0f;
        m->jumpVelocityY = f30;
        m->jumpAccelerationY = f30;
        m->jumpJerkY = f30;
        m->jumpSnapY = f30;
        m->baseSpeed = f30;
        m->flags &= ~0xF0000;
        m->dispOfs.y = f30;
        marioChgMot(10);
        marioSetFallPara();
        allPartyRideOff();
        N_marioSetBottomlessRespawnPosOnBeroEntry(m->position.x, m->position.y, m->position.z);
        N_marioReloadMapOnBottomlessOff();
        if (marioGetPtr()->motStruct != NULL) {
            __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
            marioGetPtr()->motStruct = NULL;
        }
        if (!(m->trigFlags & 4)) {
            camGetPtr(8)->flags &= ~0x200;
        }
        break;
    case 50:
        m->currSubMotionId = 0x34;
    case 52:
        if (m->hitobjStandOn != NULL) {
            marioGetPtr()->motShip->bobAngle = revise360(marioGetPtr()->motShip->bobAngle - marioGetPtr()->motShip->bobSpeed);
            m->dispOfs.y = 3.0f * (f32)sin(3.1415927f * marioGetPtr()->motShip->bobAngle / 180.0f) + -13.0f;
        }
        if ((s32)m->flags & 0x20) {
            shipEff(m->baseSpeed);
            break;
        }
        if (m->hitobjStandOn == NULL) {
            m->wStickDir1 = 0;
            m->wStickDir2 = 0;
        }
        shipMove();
        if ((s32)m->flags & 0x20000) {
            MarioWork* mm;
            s32 ok; HitEntry* land;
            f32 ly;
            marioMakeJumpPara();
            if (m->jumpVelocityY <= -7.0f) {
                m->jumpVelocityY = -7.0f;
            }
            f30 = m->jumpVelocityY;
            mm = marioGetPtr();
            mm->wPlayerCollisionBox.x = 20.0f;
            land = marioChkLandon(f30, &ly);
            mm->wPlayerCollisionBox.x = 40.0f;
            {
                f32 d;
                if (land && ((d = mm->position.y - ly) <= 0.0f || d + f30 <= 0.0f)) {
                    mm->hitobjStandOn = land;
                    mm->position.y = ly;
                    mm->flags &= ~0xF0000;
                    mm->jumpDistance = __fabsf(distABf(mm->position.x, mm->position.z, mm->lastGroundPos.x, mm->lastGroundPos.z));
                    mm->flags &= ~0x80;
                    mm->airTimer = 0;
                    mm->jumpPeakAirTime = 0;
                    camFollowYOn();
                    ok = 1;
                } else {
                    ok = 0;
                }
            }
            if (!ok) {
                m->position.y += m->jumpVelocityY;
            }
        } else {
            marioClearJumpPara();
        }
        if (strcmp(gp->currentMapName, "dou_03") == 0) {
            if (m->position.y <= 260.0f && m->position.y >= 50.0f) {
                if (0.0f != (f32)(s32)(10.0f * m->groundSlope + 0.5f) / 10.0f) {
                    m->position.y = 2.0f * (f32)sin(3.1415927f * m->groundSlope / 180.0f) + m->position.y;
                    { f32 c = (f32)cos(3.1415927f * m->groundSlope / 180.0f); m->position.z = 2.0f * -c + m->position.z; }
                }
            }
        }
        m->hitobjFront = NULL;
        m->hitobjFrontA = NULL;
        m->hitobjPush = NULL;
        m->hitobjWall = NULL;
        marioCheckWallShip(m->baseSpeed, m->directionView);
        if (m->baseSpeed < 0.5f) {
            m->baseSpeed = 0.0f;
            m->currSubMotionId = 0x32;
        }
        shipEff(m->baseSpeed);
        if (m->buttonTrg & (PAD_BUTTON_B | PAD_BUTTON_Y)) {
            if (checkHarbor(&marioGetPtr()->motShip->harborPos)) {
                m->currSubMotionId = 0x1e;
            }
        }
        break;
    }
}

void marioShipChgFall(void) {
    MarioWork* m = marioGetPtr();
    if (!(m->flags & 0x20000)) {
        m->flags |= 0x20000;
        m->jumpVelocityY = -0.1f;
        m->jumpAccelerationY = -0.2f;
        m->jumpJerkY = 0.024f;
        m->jumpSnapY = -0.004f;
    }
}

BOOL marioChkShipMoveMode(void) {
    MarioWork* m = marioGetPtr();
    if (m->currMotionId != MARIO_MOTION_SHIP) {
        return FALSE;
    }
    if ((s32)m->currSubMotionId >= 50 && (s32)m->currSubMotionId <= 52) {
        return TRUE;
    }
    return FALSE;
}

double sin(double);
double cos(double);

#define SHIP_WORK marioGetPtr()->motShip
#define SHIP_DISP marioGetPtr()->motShip

void shipMove(void)
{
  MarioWork *m = marioGetPtr();
  f32 len;
  unsigned long long rateIdx;
  f32 spd;
  f32 tz;
  f32 vx;
  f32 vz;
  f32 max;
  f32 tx;
  f32 dx;
  f32 dz;
  f32 fr;
  s32 snd;
  spd = m->controlStickSensitivity;
  dx = 0.0f;
  dx = 0.0f;
  if (marioGetPtr()->motShip->swirlTime != 0)
  {
    marioGetPtr()->motShip->swirlTime--;
    movePos(&dx, &dz, marioGetPtr()->motShip->swirlX, marioGetPtr()->motShip->swirlY);
    marioGetPtr()->motShip->dispX += dx;
    marioGetPtr()->motShip->dispY += dz;
    max = shipData[4];
    if (marioGetPtr()->motShip->dispX > max)
    {
      max = shipData[4];
      marioGetPtr()->motShip->dispX = max;
    }
    if (marioGetPtr()->motShip->dispX < (-shipData[4]))
    {
      marioGetPtr()->motShip->dispX = -shipData[4];
    }
    max = shipData[4];
    if (marioGetPtr()->motShip->dispY > max)
    {
      max = shipData[4];
      marioGetPtr()->motShip->dispY = max;
    }
    if (marioGetPtr()->motShip->dispY < (-shipData[4]))
    {
      marioGetPtr()->motShip->dispY = -shipData[4];
    }
  }
  vx = marioGetPtr()->motShip->dispX;
  vz = marioGetPtr()->motShip->dispY;
  rateIdx = 3;
  if (m->controlStickSensitivity)
  {
    tx = spd * ((f32) sin((3.1415927f * (m->controlStickAngle)) / 180.0f));
    tz = spd * (-((f32) cos((3.1415927f * (m->controlStickAngle)) / 180.0f)));
  }
  else
  {
    tx = vx;
    tz = vz;
  }
  max = shipData[4];
  if (spd || (marioGetPtr()->motShip->swirlTime != 0))
  {
    if (tx > vx)
    {
      vx += ((1.0f + tx) - vx) * shipData[rateIdx];
      if (vx >= max)
      {
        vx = max;
      }
    }
    else
    {
      vx += ((tx - 1.0f) - vx) * shipData[rateIdx];
      if (vx <= (-max))
      {
        vx = -max;
      }
    }
    if (tz > vz)
    {
      vz += ((1.0f + tz) - vz) * shipData[rateIdx];
      if (vz >= max)
      {
        vz = max;
      }
    }
    else
    {
      vz += ((tz - 1.0f) - vz) * shipData[rateIdx];
      if (vz <= (-max))
      {
        vz = -max;
      }
    }
  }
  if (!spd)
  {
    fr = shipData[0];
    if (m->hitobjWall != NULL)
    {
      fr = shipData[1];
    }
    fr = __fabsf(fr);
    vx += (-vx) * fr;
    vz += (-vz) * fr;
    if (__fabsf(vx) < 0.01f)
    {
      vx = 0.0f;
    }
    if (__fabsf(vz) < 0.01f)
    {
      vz = 0.0f;
    }
  }
  marioGetPtr()->motShip->dispX = vx;
  marioGetPtr()->motShip->dispY = vz;
  len = distABf(0.0f, 0.0f, vx, vz);
  if (spd)
  {
    if (len >= shipData[4])
    {
      len = shipData[4];
    }
  }
  else
    if (len < shipData[5])
  {
    len = 0.0f;
    marioGetPtr()->motShip->dispX = 0.0f;
    marioGetPtr()->motShip->dispY = 0.0f;
  }
  m->baseSpeed = len;
  if (len)
  {
    m->directionView = angleABf(0.0f, 0.0f, marioGetPtr()->motShip->dispX, marioGetPtr()->motShip->dispY);
  }
  if (len >= 1.0f)
  {
    MarioWork *mm = marioGetPtr();
    if ((marioGetPtr()->motStruct != ((void *) 0)) && (((u32) (marioGetPtr()->motShip->sfxId)) == 0xFFFFFFFFU))
    {
      snd = psndSFXOn_3D((const char *) 0x190, (&mm->position));
      marioGetPtr()->motShip->sfxId = snd;
    }
  }
  else
    if (len <= 0.2f)
  {
    marioGetPtr();
    if ((marioGetPtr()->motStruct != ((void *) 0)) && (((u32) (marioGetPtr()->motShip->sfxId)) != 0xFFFFFFFFU))
    {
      psndSFXOff(marioGetPtr()->motShip->sfxId);
      snd = -1;
      marioGetPtr()->motShip->sfxId = snd;
    }
  }
  if (((u32) (marioGetPtr()->motShip->sfxId)) != 0xFFFFFFFFU)
  {
    psndSFX_pos(marioGetPtr()->motShip->sfxId, (&m->position));
  }
  m->moveDist = len;
}
void shipStrippedPool(f32* p) { p[0] = 15.0f; p[1] = 50.0f; p[2] = 30.30303f; p[3] = 180.0f; p[4] = 3.1415927f; }

BOOL checkHarbor(Vec* out) {
    f32 ix, iy, iz;
    f32 tx, ty, tz, sn, cs, hx, hy, hz, dist, nx, ny, nz;
    Vec p1, p2, p3;
    HitEntry* hit;
    MarioWork* m = marioGetPtr();
    HARBOR_PROBE(m->displayDirection, p1)
    HARBOR_PROBE(90.0f + m->displayDirection, p2)
    HARBOR_PROBE(m->displayDirection - 90.0f, p3)
    return FALSE;
}

// prototypes for asm stopgap forward references
BOOL checkHarbor(Vec* out);
void shipMove(void);
BOOL marioChkShipMoveMode(void);
void marioShipChgFall(void);
void mot_ship(void);
void mot_ship_post(void);
void marioReInit_ship(void);
void marioForceShipAnime(void);
BOOL marioChkShipPlace(HitEntry* hit);
void marioShipSetDispDir(void);
void marioShipForceStop(void);
BOOL marioChkSwirlPower(void);
void marioSetSwirlPower(s32 msec, f32 a, f32 b);
s32 marioShipChanging(void);
s32 N_marioShipGetDir(void);
