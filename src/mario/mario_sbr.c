#include "mario/mario_sbr.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "driver/hitdrv.h"
#include "driver/seqdrv.h"
#include "party.h"
#include "system.h"
#include <string.h>
#include "party/party_yoshi.h"
#include <demo/DEMOPad.h>
#include "koopa.h"
#include "mot/mot_slit.h"
#include "mot/mot_walk.h"
#include "libc_decls.h"

// A macro, not an inline function: as a function a call to revise360() is not inlined.
#define REVISE360(a)              \
    while ((a) < 0.0f) {          \
        (a) += 360.0f;            \
    } while ((a) >= 360.0f) { (a) \
    -= 360.0f;                    \
}

typedef struct RubWork {
    s32 unk0;       // 0x0
    s32 count;      // 0x4
    s32 num;        // 0x8
    s32 idx;        // 0xC
    s32 dir;        // 0x10
    s32 start;      // 0x14
    s32 unk18;      // 0x18
    s32 unk1C;      // 0x1C
    s32 revDir;     // 0x20
    s32 idleTimer;  // 0x24
    u8 unk28[0x3C - 0x28]; // 0x28
    s16 hist[21];   // 0x3C
} RubWork;


RubWork rubwork;

void marioBottomless(void) {
    MarioWork* mp = marioGetPtr();
    PartyEntry* party;

    if (mp->position.y > -2000.0f) {
        return;
    }
    if (marioBgmodeChk() == TRUE) {
        marioChgMot(MARIO_MOTION_BOTTOMLESS);
    } else if (mp->bottomlessReload) {
        seqSetSeq(SEQ_MAP_CHANGE, gp->currentMapName, gp->beroEnterName);
    } else {
        mp->position = mp->bottomlessResetPosition;
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            party->position = mp->position;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            party->position = mp->position;
        }
        if (marioBgmodeChk() == TRUE) {
            mp->position.z = -450.0f;
        }
        marioChgMot(MARIO_MOTION_FORCE_RESET);
    }
}

// Descriptive name (not in the symbol maps).
void marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z) {
    MarioWork* wp = marioGetPtr();
    Vec position;

    position = (Vec){ x, y + 37.0f, z };
    wp->bottomlessResetPosition = position;
}

// Descriptive name (not in the symbol maps).
void marioSetBottomlessRespawnPos(f32 x, f32 y, f32 z) {
    MarioWork* wp = marioGetPtr();
    Vec position;

    position = (Vec){ x, y, z };
    wp->bottomlessResetPosition = position;
}

// Descriptive name (not in the symbol maps).
void marioReloadMapOnBottomlessOn(void) {
    MarioWork* mp = marioGetPtr();
    mp->bottomlessReload = 1;
}

// Descriptive name (not in the symbol maps).
void marioReloadMapOnBottomlessOff(void) {
    MarioWork* mp = marioGetPtr();
    mp->bottomlessReload = 0;
}

BOOL marioSetDirEventMain(void) {
    MarioWork* mp = marioGetPtr();
    s32 frames;
    f32 target;

    if (!(mp->dispFlags & MARIO_DISP_FLAG_DIR_EVENT)) {
        return TRUE;
    }
    frames = sysMsec2Frame(mp->dirEvtMsec);
    if (frames > 0) {
        target = revise360(270.0f - mp->dirEvtTargetDir);
        target = (target - mp->dirEvtStartDir) * ((f32)mp->dirEvtFrame / (f32)frames);
        mp->displayDirection = revise360(mp->dirEvtStartDir + target);
        mp->displayDirectionTarget = mp->displayDirection;
        mp->directionView = toMovedirSimple(mp->displayDirection);
        mp->moveDirection = mp->directionView;
        if (++mp->dirEvtFrame > frames) {
            mp->dispFlags &= ~MARIO_DISP_FLAG_DIR_EVENT;
            return TRUE;
        }
        return FALSE;
    } else {
        mp->displayDirection = revise360(270.0f - mp->dirEvtTargetDir);
        mp->displayDirectionTarget = mp->displayDirection;
        mp->directionView = toMovedirSimple(mp->displayDirection);
        mp->moveDirection = mp->directionView;
        mp->dispFlags &= ~MARIO_DISP_FLAG_DIR_EVENT;
        return TRUE;
    }
}

f32 marioGetMoveRate(f32 dir) {
    f32 c = cos(3.1415927f * (dir - marioGetPtr()->targetYaw) / 180.0f);
    return 1.0f + (1.19f - 1.0f) * __fabsf(c);
}

static void marioSetGravity(f32 g) {
    marioGetPtr()->playerGravity = g;
}

void marioResetGravity(void) {
    marioSetGravity(1.0f);
}

void* marioGetTouchNpcPtr(void) {
    return marioGetPtr()->touchNpc;
}

BOOL marioChkPlanePlace(HitEntry* hit) {
    return (hitGetAttr(hit) & 0x38) != 0;
}

void marioSetPeakYpos(void) {
    MarioWork* mp = marioGetPtr();
    mp->jumpPeakY = mp->position.y;
}

void marioAdjustMoveDir(void) {
    MarioWork* mp = marioGetPtr();

    if (mp->displayDirection >= 90.0f && mp->displayDirection <= 270.0f) {
#ifdef VERSION_G8ME01
        mp->moveDirection = toMovedir(180.0f);
#else
        mp->moveDirection = toMovedir(90.0f);
#endif
        mp->directionView = mp->moveDirection;
        mp->displayDirection = 180.0f;
        mp->displayDirectionTarget = 180.0f;
    } else {
#ifdef VERSION_G8ME01
        mp->moveDirection = toMovedir(0.0f);
#else
        mp->moveDirection = toMovedir(90.0f);
#endif
        mp->directionView = mp->moveDirection;
        mp->displayDirection = 0.0f;
        mp->displayDirectionTarget = 0.0f;
    }
}

f32 marioGetDispDir(void) {
    MarioWork* mp = marioGetPtr();
    return revise360(mp->displayDirection - 90.0f + mp->targetYaw);
}

f32 revise360(f32 angle) {
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

f32 toMovedir(f32 angle) {
    MarioWork* mp = marioGetPtr();

    if (mp->flags & MARIO_FLAG_PAPER_MODE) {
        if (mp->displayDirection == 280.0f || mp->displayDirection == 460.0f) {
            angle = 0.0f;
        } else if (mp->displayDirection == 260.0f || mp->displayDirection == 440.0f) {
            angle = 180.0f;
        } else if (revise360(mp->displayDirection) >= 180.0f) {
            angle = 0.0f;
        } else {
            angle = 180.0f;
        }
    }
#ifdef VERSION_G8ME01
    angle = (s32)(1000.0f * (270.0f - angle + mp->targetYaw) + ((270.0f - angle + mp->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mp->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

f32 partyToMovedir(PartyEntry* party, f32 angle) {
    MarioWork* player = party->playerPtr;

    if (party->flags & PARTY_FLAG_PAPER_MODE) {
        if (party->displayDirection == 280.0f || party->displayDirection == 460.0f) {
            angle = 0.0f;
        } else if (party->displayDirection == 260.0f || party->displayDirection == 440.0f) {
            angle = 180.0f;
        } else if (revise360(party->displayDirection) >= 180.0f) {
            angle = 0.0f;
        } else {
            angle = 180.0f;
        }
    }
#ifdef VERSION_G8ME01
    angle = (s32)(1000.0f * (270.0f - angle + player->targetYaw) + ((270.0f - angle + player->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + player->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

f32 toMovedirSimple(f32 angle) {
    MarioWork* mp = marioGetPtr();
#ifdef VERSION_G8ME01
    angle = (s32)(1000.0f * (270.0f - angle + mp->targetYaw) + ((270.0f - angle + mp->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mp->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

f32 toMovedir2(f32 angle, f32 ofs) {
    MarioWork* mp = marioGetPtr();

    if (mp->flags & MARIO_FLAG_PAPER_MODE) {
        if (mp->displayDirection == 280.0f || mp->displayDirection == 460.0f) {
            angle = 0.0f;
        } else if (mp->displayDirection == 260.0f || mp->displayDirection == 440.0f) {
            angle = 180.0f;
        } else if (revise360(mp->displayDirection) >= 180.0f) {
            angle = 0.0f;
        } else {
            angle = 180.0f;
        }
    }
#ifdef VERSION_G8ME01
    angle = (s32)(1000.0f * (270.0f - angle + mp->targetYaw) + ((270.0f - angle + mp->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mp->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    angle += revise360(angle - mp->targetYaw) >= 180.0f ? -ofs : ofs;
    return revise360(angle);
}

void marioForceMoveMain(void) {
    MarioWork* mp = marioGetPtr();

    if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        if (--mp->forceMoveTimer <= 0) {
            mp->forceMoveTimer = 0;
            mp->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
            mp->baseSpeed = 0.0f;
            if (mp->controlStickSensitivity > 1.0f) {
                mp->baseSpeed = marioGetDashSpd();
            } else if (mp->controlStickSensitivity > 0.0f) {
                mp->baseSpeed = marioGetWalkSpd();
            }
        }
    }
}

void marioSetForceMove(f32 dir, s32 time, f32 speed) {
    MarioWork* mp = marioGetPtr();

    mp->flags |= MARIO_FLAG_FORCED_MOVEMENT;
    mp->moveDirection = dir;
    mp->directionView = dir;
    mp->forceMoveTimer = time;
    mp->baseSpeed = speed;
    if (mp->currMotionId == MARIO_MOTION_SHIP || (mp->currMotionId == MARIO_MOTION_SLIT || mp->currMotionId == MARIO_MOTION_ROLL) || mp->currMotionId == MARIO_MOTION_YOSHI) {
        return;
    } else {
#ifdef VERSION_G8ME01
        marioChgMot(speed >= mp->mBaseDashSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
#else
        marioChgMot(speed > mp->mBaseWalkSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
#endif
        mp->moveDirection = dir;
        mp->directionView = dir;
        mp->forceMoveTimer = time;
        mp->baseSpeed = speed;
    }
}

void marioUpdateKeyData(void) {
    MarioWork* mp = marioGetPtr();
    u32 mask;
    u16 btn;

    mp->wStickDir1 = gp->stickX[0];
    mp->wStickDir2 = -gp->stickY[0];
    mp->substickX = gp->substickX[0];
    mp->substickY = -gp->substickY[0];
    mp->triggerL = gp->triggerLeft[0];
    mp->triggerR = gp->triggerRight[0];
    mp->analogA = 0;
    mp->analogB = 0;
    mask = mp->enableKeyMask ? mp->enableKeyMask : 0xFFFFFFFF;
    mp->buttonHold = gp->button[0] & mask;
    mp->buttonTrg = gp->buttonNew[0] & mask;
    mp->buttonRepeat = gp->buttonRepeat[0] & mask;
    mp->buttonUp = gp->buttonUp[0] & mask;
    if (!(mp->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_CTRL_OFF_2 | MARIO_FLAG_KEY_OFF))) {
        if (mp->trigFlags & MARIO_TRIG_FLAG_DISABLE_STICK) {
            mp->wStickDir1 = 0;
            mp->wStickDir2 = 0;
        }
        if (mp->trigFlags & MARIO_TRIG_FLAG_DISABLE_SUB_STICK) {
            mp->substickX = 0;
            mp->substickY = 0;
        }
        if (mp->trigFlags & MARIO_TRIG_FLAG_DISABLE_TRIGGERS) {
            mp->triggerL = 0;
            mp->triggerR = 0;
        }
        if (mp->trigFlags & MARIO_TRIG_FLAG_DISABLE_UNUSED_INPUT) {
            mp->analogA = 0;
            mp->analogB = 0;
        }
    }
    if ((mp->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_CTRL_OFF_2 | MARIO_FLAG_KEY_OFF)) || !marioChkKey()) {
        mp->wStickDir1 = 0;
        mp->wStickDir2 = 0;
        mp->substickX = 0;
        mp->substickY = 0;
        mp->triggerL = 0;
        mp->triggerR = 0;
        mp->analogA = 0;
        mp->analogB = 0;
        if (!mp->enableKeyMask) {
            mp->buttonHold = 0;
            mp->buttonTrg = 0;
            mp->buttonRepeat = 0;
            mp->buttonUp = 0;
        }
        if (!marioGetMotSlitCancel3() && (mp->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            mp->buttonHold = PAD_TRIGGER_R;
        }
    }
    btn = DemoPad[0].dirs;
    mp->demoButtonNew = btn & (btn ^ mp->demoButtonPrev);
    mp->demoButtonPrev = btn;
}

void marioGetStick(f32* dir, f32* mag) {
    MarioWork* mp = marioGetPtr();
    s32 x;
    s32 y;
    f32 m;
    f32 a;

    if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        *dir = mp->directionView;
        *mag = mp->baseSpeed;
        return;
    }
    x = mp->wStickDir1;
    y = ((mp->flags & MARIO_FLAG_BG_MODE) || kpaGetStageViewType() == 0) ? 0 : -mp->wStickDir2;
    if (yoshiGetStatus() == 2) {
        y = 0;
    }
    y = -y;
    m = distABf(0.0f, 0.0f, x, y);
    if (m >= 70.0f) {
        m = 70.0f;
    }
    a = angleABf(0.0f, 0.0f, x, y);
    if (!m) {
        a = mp->moveDirection;
    }
    *dir = revise360(a + mp->targetYaw);
    *mag = m;
}

// Descriptive name (not in the symbol maps).
s32 marioGetMovementSpeed(void) {
    MarioWork* mp = marioGetPtr();
    s8 x = mp->wStickDir1;

    if (x == 0 && mp->wStickDir2 == 0) {
        return 0;
    }
    if (x * x + mp->wStickDir2 * mp->wStickDir2 > 3025) {
        return 2;
    }
    return 1;
}

void marioGetRubInit(void) {
    memset(&rubwork, 0, sizeof(rubwork));
    rubwork.revDir = -1;
}

s32 chkRevolveDir(s32* cw, s32* ccw) {
    s32 base;
    s32 i;
    RubWork* rw = &rubwork;
    s32 off = 0;
    s32 cwCount = 0;
    s32 cur;
    s32 j;
    s32 ccwCount = 0;
    s32 prev;
    s32 d;

    base = rw->idx;
    for (i = 0; i < 10; i++) {
        j = base + off;
        while (j < 0) {
            j += 20;
        }
        cur = rw->hist[j];
        d = base + (off - 1);
        while (d < 0) {
            d += 20;
        }
        prev = rw->hist[d];
        off--;
        if (i == 0 && cur == prev) {
            continue;
        }
        d = cur - prev;
        if (__fabsf(d) > 180.0f) {
            cur += d > 0 ? 360 : -360;
        }
        d = cur - prev;
        if (d > -90 && d < 0) {
            cwCount++;
        }
        if (d > 0 && d < 90) {
            ccwCount++;
        }
    }
    if (cwCount == 0 && ccwCount >= 4) {
        rw->revDir = 0;
        *cw = cwCount;
        *ccw = ccwCount;
        return 0;
    }
    if (ccwCount == 0 && cwCount >= 4) {
        rw->revDir = 1;
        *cw = cwCount;
        *ccw = ccwCount;
        return 1;
    }
    *cw = cwCount;
    *ccw = ccwCount;
    rw->revDir = -1;
    return -1;
}

s32 checkOneRevolution(s32 dir) {
    RubWork* rw;
    s32 j;
    s32 cur;
    s32 v;
    s32 start;
    s32 i;
    s32 ret = 0;

    rw = &rubwork;
    if (rw->num < 2) {
        return -1;
    }
    start = rw->start;
    cur = rw->hist[rw->idx];
    for (i = 1; i <= 20; i++) {
        // FAKE: rubwork read directly instead of through rw; the plain form changes the object
        j = rubwork.idx - i;
        while (j < 0) {
            j += 20;
        }
        v = rw->hist[j];
        if (cur != v) {
            break;
        }
    }
    if (cur == v) {
        return -1;
    }
    if (v == -1) {
        return -1;
    }
    if (dir) {
        if (cur > v) {
            if (start < v || cur <= start) {
                ret = 1;
            }
        } else {
            if (cur <= start && start < v) {
                ret = 1;
            }
        }
    } else {
        if (cur < v) {
            if (start > v || start <= cur) {
                ret = 1;
            }
        } else {
            if (cur >= start && start > v) {
                ret = 1;
            }
        }
    }
    return ret;
}

static inline void rubClearHist(s32 v) {
    u32 i;
    s16* p = rubwork.hist;
    for (i = 0; i < 21; i++) {
        p[i] = v;
    }
}

s32 marioGetRub(u32 btn, s32* dirOut, s32* countOut, f32* magOut) {
    s32 ret = 0;
    MarioWork* mp = marioGetPtr();
    RubWork* rw = &rubwork;
    f32 dir;
    f32 mag;
    s32 a;
    s32 r;
    s32 j;
    s32 cw;
    s32 ccw;

    if (btn && !(mp->buttonHold & btn)) {
        marioGetRubInit();
        *dirOut = -1;
        *countOut = 0;
        *magOut = 0.0f;
        return -1;
    }
    marioGetStick(&dir, &mag);
    a = dir;
    if (mag < 50.0f) {
        if (++rw->idleTimer > 10) {
            rubClearHist(a);
            rw->num = 0;
        }
        return -1;
    }
    rw->idleTimer = 0;
    rw->hist[rw->idx] = a;
    // FAKE: rubwork read directly instead of through rw; the plain form changes the object
    j = rubwork.idx;
    j++;
    while (j < 0) {
        j += 20;
    }
    rw->hist[j] = -1;
    r = chkRevolveDir(&cw, &ccw);
    if (r == 0) {
        if (rw->dir == 2) {
            rw->num = 0;
            rw->count = 0;
        }
        if (rw->num == 0) {
            rw->start = a;
            rubClearHist(a);
        }
        rw->unk1C = 0;
        rw->num++;
        if (checkOneRevolution(0) == 1) {
            ret = 1;
            rubClearHist(rw->start);
            rw->count++;
        }
        rw->dir = 1;
    }
    if (r == 0) {
        *dirOut = rw->dir - 1;
        *countOut = rw->count;
        rw->idx++;
        if (rw->idx >= 20) {
            rw->idx -= 20;
        }
        return ret;
    }
    if (r == 1) {
        if (rw->dir == 1) {
            rw->num = 0;
            rw->count = 0;
        }
        if (rw->num == 0) {
            rw->start = a;
            rubClearHist(a);
        }
        rw->unk1C = 0;
        rw->num++;
        if (checkOneRevolution(1) == 1) {
            ret = 1;
            rubClearHist(rw->start);
            rw->count++;
        }
        rw->dir = 2;
    }
    if (r == 1) {
        *dirOut = rw->dir - 1;
        *countOut = rw->count;
        rw->idx++;
        if (rw->idx >= 20) {
            rw->idx -= 20;
        }
        return ret;
    }
    rw->idx++;
    if (rw->idx >= 20) {
        rw->idx -= 20;
    }
    return -1;
}
