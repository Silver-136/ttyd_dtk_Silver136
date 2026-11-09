// Mario subroutines: stick and rub input, falling out of the map (bottomless respawn), move rate, gravity,
// touched NPC, and the paper/plane/boat checks.

#include "version.h"
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

typedef struct RubWork {
    s32 state;       // 0x0
    s32 count;      // 0x4
    s32 num;        // 0x8
    s32 idx;        // 0xC
    s32 dir;        // 0x10
    s32 start;      // 0x14
    s32 lastAngle;      // 0x18
    s32 stallFrames;      // 0x1C
    s32 revDir;     // 0x20
    s32 idleTimer;  // 0x24
    u8 unk28[0x3C - 0x28]; // 0x28
    s16 hist[21];   // 0x3C
} RubWork;

RubWork rubwork;

/**
 * Handles falling to y <= -2000: background mode enters the bottomless motion.
 * The early demos otherwise use fixed positions for eki_04, gon_10 and muj_00, or (0, 74, 0) elsewhere;
 * only the generic fallback also moves the partners. JP and later reload the map when requested,
 * otherwise restores Mario and his partners to bottomlessResetPosition and force-resets his motion.
 */
void marioBottomless(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;

    if (mario->position.y > -2000.0f) {
        return;
    }
    if (marioBgmodeChk() == TRUE) {
        marioChgMot(MARIO_MOTION_BOTTOMLESS);
#if VERSION <= VERSION_DEMO_JP
        return;
    }
    if (strcmp(gp->currentMapName, "eki_04") == 0) {
        mario->position = (Vec){344.0f, 840.0f, 30.0f};
    } else if (strcmp(gp->currentMapName, "gon_10") == 0) {
        mario->position = (Vec){980.0f, 74.0f, -17.0f};
    } else if (strcmp(gp->currentMapName, "muj_00") == 0) {
        mario->position = (Vec){-182.0f, 25.0f, -104.0f};
#else
    } else if (mario->bottomlessReload) {
        seqSetSeq(SEQ_MAP_CHANGE, gp->currentMapName, gp->beroEnterName);
#endif
    } else {
#if VERSION <= VERSION_DEMO_JP
        mario->position = (Vec){0.0f, 74.0f, 0.0f};
#else
        mario->position = mario->bottomlessResetPosition;
#endif
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            party->position = mario->position;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            party->position = mario->position;
        }
        if (marioBgmodeChk() == TRUE) {
            mario->position.z = -450.0f;
        }
#if VERSION <= VERSION_DEMO_JP
    }
    marioChgMot(MARIO_MOTION_FORCE_RESET);
}
#else
        marioChgMot(MARIO_MOTION_FORCE_RESET);
    }
}
#endif

/**
 * Saves where Mario respawns after falling out of the map:
 * an entrance (evt_bero.c) or the boat's landing spot, 37 units above the given point.
 */
// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP

void marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z) {
    MarioWork* mario = marioGetPtr();
    Vec position;

    position = (Vec){ x, y + 37.0f, z };
    mario->bottomlessResetPosition = position;
}
#endif

/** Saves the bottomless-pit respawn point as given; set from seq_mapchange.c to 10 above Mario on map entry. */
// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP

void marioSetBottomlessRespawnPos(f32 x, f32 y, f32 z) {
    MarioWork* mario = marioGetPtr();
    Vec position;

    position = (Vec){ x, y, z };
    mario->bottomlessResetPosition = position;
}
#endif

/**
 * Makes falling out of the map reload the current map at its entrance instead of respawning;
 * set when Mario enters a map as the boat.
 */
// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP

void marioReloadMapOnBottomlessOn(void) {
    MarioWork* mario = marioGetPtr();
    mario->bottomlessReload = 1;
}
#endif

/** Goes back to respawning at the saved point after a fall; cleared when the boat lands. */
// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP

void marioReloadMapOnBottomlessOff(void) {
    MarioWork* mario = marioGetPtr();
    mario->bottomlessReload = 0;
}
#endif

/**
 * Per-frame step of a scripted turn (MARIO_DISP_FLAG_DIR_EVENT):
 * turns Mario from dirEvtStartDir to dirEvtTargetDir over dirEvtMsec.
 * Returns TRUE when no turn is running or it has just finished.
 */
BOOL marioSetDirEventMain(void) {
    MarioWork* mario = marioGetPtr();
    s32 frames;
    f32 target;

    if (!(mario->dispFlags & MARIO_DISP_FLAG_DIR_EVENT)) {
        return TRUE;
    }
    frames = sysMsec2Frame(mario->dirEvtMsec);
    if (frames > 0) {
        target = revise360(270.0f - mario->dirEvtTargetDir);
        target = (target - mario->dirEvtStartDir) * ((f32)mario->dirEvtFrame / (f32)frames);
        mario->displayDirection = revise360(mario->dirEvtStartDir + target);
        mario->displayDirectionTarget = mario->displayDirection;
        mario->directionView = toMovedirSimple(mario->displayDirection);
        mario->moveDirection = mario->directionView;
        if (++mario->dirEvtFrame > frames) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_DIR_EVENT;
            return TRUE;
        }
        return FALSE;
    } else {
        mario->displayDirection = revise360(270.0f - mario->dirEvtTargetDir);
        mario->displayDirectionTarget = mario->displayDirection;
        mario->directionView = toMovedirSimple(mario->displayDirection);
        mario->moveDirection = mario->directionView;
        mario->dispFlags &= ~MARIO_DISP_FLAG_DIR_EVENT;
        return TRUE;
    }
}

/**
 * Speed multiplier for moving in world direction dir:
 * 1.0 across the screen up to 1.19 straight towards or away from the camera,
 * so depth moves look as fast as sideways ones. Used by Mario's and Yoshi's movement.
 */
f32 marioGetMoveRate(f32 dir) {
    f32 c = cos(3.1415927f * (dir - marioGetPtr()->targetYaw) / 180.0f);
    return 1.0f + (1.19f - 1.0f) * __fabsf(c);
}

/** Sets Mario's gravity multiplier (playerGravity; 1.0 is normal). */
static void marioSetGravity(f32 g) {
    marioGetPtr()->playerGravity = g;
}

/** Puts Mario's gravity back to normal (1.0); run from mario.c. */
void marioResetGravity(void) {
    marioSetGravity(1.0f);
}

void* marioGetTouchNpcPtr(void) {
    return marioGetPtr()->touchNpc;
}

/** TRUE if the surface is a plane panel (hit attribute bits 0x38); Mario's motion checks then turn him into a plane. */
BOOL marioChkPlanePlace(HitEntry* hit) {
    return (hitGetAttr(hit) & 0x38) != 0;
}

/** Records Mario's current height as the peak of the jump or fall (jumpPeakY). */
void marioSetPeakYpos(void) {
    MarioWork* mario = marioGetPtr();
    mario->jumpPeakY = mario->position.y;
}

/**
 * Snaps Mario to face straight left or right on screen, whichever is nearer,
 * and points his movement direction that way. JP and the early demos give both cases the 90-degree move direction.
 */
void marioAdjustMoveDir(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) {
#if VERSION >= VERSION_US
        mario->moveDirection = toMovedir(180.0f);
#else
        // BUG: both cases use the 90-degree move direction although they face 180 (here) and 0 (below); US and
        // later use 180 and 0.
        mario->moveDirection = toMovedir(90.0f);
#endif

        mario->directionView = mario->moveDirection;
        mario->displayDirection = 180.0f;
        mario->displayDirectionTarget = 180.0f;
    } else {
#if VERSION >= VERSION_US
        mario->moveDirection = toMovedir(0.0f);
#else
        mario->moveDirection = toMovedir(90.0f);
#endif

        mario->directionView = mario->moveDirection;
        mario->displayDirection = 0.0f;
        mario->displayDirectionTarget = 0.0f;
    }
}

/**
 * Mario's facing as a world angle (displayDirection turned by the camera yaw);
 * npcdrv.c uses it to compare with NPC directions.
 */
f32 marioGetDispDir(void) {
    MarioWork* mario = marioGetPtr();
    return revise360(mario->displayDirection - 90.0f + mario->targetYaw);
}

/** Wraps an angle in degrees into 0 <= angle < 360. */
f32 revise360(f32 angle) {
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

/**
 * Converts a screen-relative angle in degrees into a world move direction (270 - angle + camera yaw, wrapped).
 * In paper mode the input angle is forced to 0 or 180 from the side Mario faces before that conversion.
 * US and later round to 0.001 degree; JP and the early demos to 0.01.
 */
f32 toMovedir(f32 angle) {
    MarioWork* mario = marioGetPtr();

    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        if (mario->displayDirection == 280.0f || mario->displayDirection == 460.0f) {
            angle = 0.0f;
        } else if (mario->displayDirection == 260.0f || mario->displayDirection == 440.0f) {
            angle = 180.0f;
        } else if (revise360(mario->displayDirection) >= 180.0f) {
            angle = 0.0f;
        } else {
            angle = 180.0f;
        }
    }
#if VERSION >= VERSION_US
    angle = (s32)(1000.0f * (270.0f - angle + mario->targetYaw) + ((270.0f - angle + mario->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mario->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

/** toMovedir for a partner: uses the partner's paper mode and facing and the camera yaw from its playerPtr. */
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
#if VERSION >= VERSION_US
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

/** toMovedir without the paper-mode facing override: only converts the screen angle by the camera yaw. */
f32 toMovedirSimple(f32 angle) {
    MarioWork* mario = marioGetPtr();
#if VERSION >= VERSION_US
    angle = (s32)(1000.0f * (270.0f - angle + mario->targetYaw) + ((270.0f - angle + mario->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mario->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

/**
 * toMovedir, then turns the result ofs degrees further away from the camera yaw side it is on;
 * used to aim the hammer and partner moves a little off Mario's line.
 */
f32 toMovedir2(f32 angle, f32 ofs) {
    MarioWork* mario = marioGetPtr();

    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        if (mario->displayDirection == 280.0f || mario->displayDirection == 460.0f) {
            angle = 0.0f;
        } else if (mario->displayDirection == 260.0f || mario->displayDirection == 440.0f) {
            angle = 180.0f;
        } else if (revise360(mario->displayDirection) >= 180.0f) {
            angle = 0.0f;
        } else {
            angle = 180.0f;
        }
    }
#if VERSION >= VERSION_US
    angle = (s32)(1000.0f * (270.0f - angle + mario->targetYaw) + ((270.0f - angle + mario->targetYaw) >= 0.0f ? 0.5f : -0.5f)) / 1000.0f;
#else
    angle = (s32)(100.0f * (0.05f + (270.0f - angle + mario->targetYaw))) / 100.0f;
#endif
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    angle += revise360(angle - mario->targetYaw) >= 180.0f ? -ofs : ofs;
    return revise360(angle);
}

/**
 * Per-frame: counts down a forced move (marioSetForceMove) and when it ends gives Mario back the walk
 * or dash speed his stick asks for.
 */
void marioForceMoveMain(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        if (--mario->forceMoveTimer <= 0) {
            mario->forceMoveTimer = 0;
            mario->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
            mario->baseSpeed = 0.0f;
            if (mario->controlStickSensitivity > 1.0f) {
                mario->baseSpeed = marioGetDashSpd();
            } else if (mario->controlStickSensitivity > 0.0f) {
                mario->baseSpeed = marioGetWalkSpd();
            }
        }
    }
}

/**
 * Makes Mario walk or dash in direction dir at speed for time frames regardless of the stick;
 * scripts use it (evt_mario.c) to move him to a point.
 * The boat, slit, tube and Yoshi motions keep their own motion.
 */
void marioSetForceMove(f32 dir, s32 time, f32 speed) {
    MarioWork* mario = marioGetPtr();

    mario->flags |= MARIO_FLAG_FORCED_MOVEMENT;
    mario->moveDirection = dir;
    mario->directionView = dir;
    mario->forceMoveTimer = time;
    mario->baseSpeed = speed;
    if (mario->currMotionId == MARIO_MOTION_SHIP || (mario->currMotionId == MARIO_MOTION_SLIT || mario->currMotionId == MARIO_MOTION_ROLL) || mario->currMotionId == MARIO_MOTION_YOSHI) {
        return;
    } else {

#if VERSION >= VERSION_US
        marioChgMot(speed >= mario->baseDashSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
#else
        marioChgMot(speed > mario->baseWalkSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
#endif

#if VERSION >= VERSION_JP
        mario->moveDirection = dir;
        mario->directionView = dir;
        mario->forceMoveTimer = time;
        mario->baseSpeed = speed;
#endif

    }
}

/**
 * Per-frame: copies controller 0's sticks and triggers into MarioWork and masks its buttons by enableKeyMask.
 * Controls-off blanks analog input; it also clears buttons unless enableKeyMask is set, with a slit-mode R override.
 * Also records the demo pad's new presses.
 */
void marioUpdateKeyData(void) {
    MarioWork* mario = marioGetPtr();
    u32 mask;
    u16 btn;

    mario->wStickDir1 = gp->stickX[0];
    mario->wStickDir2 = -gp->stickY[0];
    mario->substickX = gp->substickX[0];
    mario->substickY = -gp->substickY[0];
    mario->triggerL = gp->triggerLeft[0];
    mario->triggerR = gp->triggerRight[0];
    mario->analogA = 0;
    mario->analogB = 0;
    mask = mario->enableKeyMask ? mario->enableKeyMask : 0xFFFFFFFF;
    mario->buttonHold = gp->button[0] & mask;
    mario->buttonTrg = gp->buttonNew[0] & mask;
    mario->buttonRepeat = gp->buttonRepeat[0] & mask;
    mario->buttonUp = gp->buttonUp[0] & mask;
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF))) {
#else
    if (!(mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_CTRL_OFF_2 | MARIO_FLAG_KEY_OFF))) {
#endif
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_STICK) {
            mario->wStickDir1 = 0;
            mario->wStickDir2 = 0;
        }
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_SUB_STICK) {
            mario->substickX = 0;
            mario->substickY = 0;
        }
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_TRIGGERS) {
            mario->triggerL = 0;
            mario->triggerR = 0;
        }
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_UNUSED_INPUT) {
            mario->analogA = 0;
            mario->analogB = 0;
        }
    }

#if VERSION <= VERSION_DEMO_JP
    if ((mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_KEY_OFF)) || !marioChkKey()) {
#else
    if ((mario->flags & (MARIO_FLAG_CTRL_OFF | MARIO_FLAG_CTRL_OFF_2 | MARIO_FLAG_KEY_OFF)) || !marioChkKey()) {
#endif

        mario->wStickDir1 = 0;
        mario->wStickDir2 = 0;
        mario->substickX = 0;
        mario->substickY = 0;
        mario->triggerL = 0;
        mario->triggerR = 0;
        mario->analogA = 0;
        mario->analogB = 0;
        if (!mario->enableKeyMask) {
            mario->buttonHold = 0;
            mario->buttonTrg = 0;
            mario->buttonRepeat = 0;
            mario->buttonUp = 0;
        }
        if (!marioGetMotSlitCancel3() && (mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            mario->buttonHold = PAD_TRIGGER_R;
        }
    }
    btn = DemoPad[0].dirs;
    mario->demoButtonNew = btn & (btn ^ mario->demoButtonPrev);
    mario->demoButtonPrev = btn;
}

/**
 * Stick input as a world direction (degrees, camera yaw applied) and magnitude (0..70);
 * during a forced move it returns the forced direction and speed instead.
 */
void marioGetStick(f32* dir, f32* mag) {
    MarioWork* mario = marioGetPtr();
    s32 x;
    s32 y;
    f32 m;
    f32 a;

    if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        *dir = mario->directionView;
        *mag = mario->baseSpeed;
        return;
    }
    x = mario->wStickDir1;
    y = ((mario->flags & MARIO_FLAG_BG_MODE) || kpaGetStageViewType() == KPA_VIEW_SIDE) ? 0 : -mario->wStickDir2;
    if (yoshiGetStatus() == YOSHI_STATUS_FLUTTER) {
        y = 0;
    }
    y = -y;
    m = distABf(0.0f, 0.0f, x, y);
    if (m >= 70.0f) {
        m = 70.0f;
    }
    a = angleABf(0.0f, 0.0f, x, y);
    if (!m) {
        a = mario->moveDirection;
    }
    *dir = revise360(a + mario->targetYaw);
    *mag = m;
}

#if VERSION >= VERSION_JP

/**
 * How far the main stick is pushed: 0 neutral, 1 a little, 2 past radius 55;
 * partner walking picks its walk or run pose from it.
 */
// Descriptive name (not in the symbol maps).
s32 marioGetMovementSpeed(void) {
    MarioWork* mario = marioGetPtr();
    s8 x = mario->wStickDir1;

    if (x == 0 && mario->wStickDir2 == 0) {
        return 0;
    }
    if (x * x + mario->wStickDir2 * mario->wStickDir2 > STICK_DASH_MIN_SQ) {
        return 2;
    }
    return 1;
}
#endif

/** Resets the stick-rotation (rub) tracker; run at Mario init and when the tube motion starts. */
void marioGetRubInit(void) {
    memset(&rubwork, 0, sizeof(rubwork));
    rubwork.revDir = -1;
}

/**
 * Looks at the last ten stick angles in the rub history and decides which way the stick is turning:
 * 0 one way, 1 the other (no steps against it and at least four with it), or -1 when it is not turning steadily.
 */
static s32 chkRevolveDir(s32* cw, s32* ccw) {
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

/**
 * 1 when the newest stick step has passed the angle where the current rotation started,
 * i.e. one full turn of the stick in direction dir; 0 otherwise, -1 when there is not enough history.
 */
static s32 checkOneRevolution(s32 dir) {
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

/** Fills the rub history with the angle v. */
// Original name and size (0x60) from the JP demo map (UNUSED there: every call is inlined).
void clearChkbuf(s32 v) {
    u32 i;
    s16* p = rubwork.hist;
    for (i = 0; i < 21; i++) {
        p[i] = v;
    }
}

/**
 * Tracks the stick being spun while button btn is held (0 = no button):
 * returns 1 on each completed turn and 0 between,
 * with the spin direction (0 or 1) in dirOut and the turn count in countOut;
 * -1 when the button is released or the stick is not spinning.
 * Used for the Spring Jump (A), hammer spin (B) and tube (R) inputs.
 */
s32 marioGetRub(u32 btn, s32* dirOut, s32* countOut, f32* magOut) {
    s32 ret = 0;
    MarioWork* mario = marioGetPtr();
    RubWork* rw = &rubwork;
    f32 dir;
    f32 mag;
    s32 a;
    s32 r;
    s32 j;
    s32 cw;
    s32 ccw;

    if (btn && !(mario->buttonHold & btn)) {
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
            clearChkbuf(a);
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
            clearChkbuf(a);
        }
        rw->stallFrames = 0;
        rw->num++;
        if (checkOneRevolution(0) == 1) {
            ret = 1;
            clearChkbuf(rw->start);
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
            clearChkbuf(a);
        }
        rw->stallFrames = 0;
        rw->num++;
        if (checkOneRevolution(1) == 1) {
            ret = 1;
            clearChkbuf(rw->start);
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
