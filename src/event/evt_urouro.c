// Script commands for NPCs that wander about at random ("urouro").

#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "mario/mario_sbr.h"
#include "driver/mapdrv.h"
#include "sfx_name_word.h"



typedef enum UrouroState {
    UROURO_STATE_CHOOSE_WALK = 0,
    UROURO_STATE_WALK = 1,
    UROURO_STATE_CHOOSE_WAIT = 2,
    UROURO_STATE_WAIT = 3,
} UrouroState;

typedef struct UrouroWork {
    u32 flags;     // 0x0
    s32 state;     // 0x4
    s32 timer;     // 0x8
    s32 count;     // 0xC
    s32 walkTime;  // 0x10
    s32 waitTime;  // 0x14
    s32 turnTime;  // 0x18
    f32 x;         // 0x1C
    f32 y;         // 0x20
    f32 z;         // 0x24
    f32 radius;    // 0x28
} UrouroWork;

// UrouroWork.flags (the init script's sixth argument sets the whole word)
typedef enum UrouroFlags {
    UROURO_FLAG_OUTSIDE = (1 << 0),            // 0x1, wandered past the radius
    UROURO_FLAG_TURN_HOME = (1 << 1),          // 0x2, face the centre at the next step
    UROURO_FLAG_TURN_AROUND_UNUSED = (1 << 16), // 0x10000, turn round 2-4 times while waiting; every script passes 0
} UrouroFlags;

/**
 * Sets up NPC args[0] to wander around centre (args[1], args[2]) within radius args[3] at speed args[4];
 * args[5] is the flag word (0x10000: look around a few times while idle). The state lives in LW(10).
 * Run from the npc_event.c wander move script and map scripts (gor, dmo, ...).
 */
USER_FUNC(urouro_init_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    f32 x = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 radius = evtGetFloat(event, args[3]);
    f32 speed = evtGetFloat(event, args[4]);
    s32 flags = evtGetValue(event, args[5]);
    UrouroWork* work;

    event->lwData[10] = (EvtWord)_mapAlloc(sizeof(UrouroWork));
    work = (UrouroWork*)event->lwData[10];
    work->flags = flags;
    work->state = UROURO_STATE_CHOOSE_WALK;
    work->timer = 0;
    work->count = 0;
    work->walkTime = sysMsec2Frame(6000);
    work->waitTime = sysMsec2Frame(9000);
    work->turnTime = sysMsec2Frame(1500);
    work->x = x;
    work->y = npc->position.y;
    work->z = z;
    work->radius = radius;
    npc->moveSpeed = speed;
    npc->flags |= NPC_FLAG_RESUME_AFTER_TALK;
    return EVT_RETURN_DONE;
}

/**
 * Per-frame wander step for NPC args[0]: walk in a random direction for 3-6 s,
 * turn back toward the centre when outside the radius,
 * after hitting a wall or when Mario walks into it from behind, then stand 4.5-9 s.
 * Retail versions also play the NPC's footstep sounds. Never returns DONE.
 */
USER_FUNC(urouro_main_func) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    MarioWork* mario = marioGetPtr();
    UrouroWork* work = (UrouroWork*)event->lwData[10];
    f32 dist;
    f32 step;
    f32 lim;
#if VERSION <= VERSION_DEMO_US
    f32 temp;
#else
    s32 sfxFlag;
#endif
    switch (work->state) {
    case UROURO_STATE_CHOOSE_WALK:
        work->timer = work->walkTime / 2 + irand(work->walkTime / 2 + 1);
        work->timer = work->timer - work->timer % 20;
        work->count = 0;
        npc->direction = reviseAngle(npc->direction + (f32)irand(60) - 30.0f);
        npc->moveDir = npc->direction;
        work->state = UROURO_STATE_WALK;
        break;
    case UROURO_STATE_WALK:
        dist = distABf(npc->position.x, npc->position.z, work->x, work->z);
        if (animPoseGetPeraEnd(npc->poseId)) {
            if (dist > work->radius) {
                if (!(work->flags & UROURO_FLAG_OUTSIDE)) {
                    work->flags |= UROURO_FLAG_OUTSIDE | UROURO_FLAG_TURN_HOME;
                }
                if (work->flags & UROURO_FLAG_TURN_HOME) {
                    npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z, work->x, work->z));
                    npc->moveDir = npc->direction;
                    work->flags &= ~UROURO_FLAG_TURN_HOME;
                }
            } else if (work->flags & UROURO_FLAG_OUTSIDE) {
                work->flags &= ~(UROURO_FLAG_OUTSIDE | UROURO_FLAG_TURN_HOME);
            }
        }
#if VERSION <= VERSION_DEMO_US
            if (animPoseGetPeraEnd(npc->poseId) && npc == marioGetTouchNpcPtr() && __fabsf(compAngle(npc->moveDir, mario->directionView)) < 90.0f && __fabsf(compAngle(angleABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z), mario->directionView)) < 90.0f && dist < work->radius) {
#else
        if (animPoseGetPeraEnd(npc->poseId) && npc == marioGetTouchNpcPtr()
            && __fabsf(compAngle(npc->moveDir, mario->directionView)) < 90.0f
            && __fabsf(compAngle(angleABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z),
                              mario->directionView)) < 90.0f
            && dist < work->radius) {
#endif
            npc->direction = reviseAngle(npc->moveDir - 180.0f);
            npc->moveDir = npc->direction;
            work->flags &= ~(UROURO_FLAG_OUTSIDE | UROURO_FLAG_TURN_HOME);
        }
        if (animPoseGetPeraEnd(npc->poseId) && npc->wallHit != 0) {
            npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z, work->x, work->z));
            npc->moveDir = npc->direction;
            work->flags &= ~(UROURO_FLAG_OUTSIDE | UROURO_FLAG_TURN_HOME);
        }
        work->timer--;
        if (work->timer == 0) {
            work->state = UROURO_STATE_CHOOSE_WAIT;
            break;
        }
        if (animPoseGetPeraEnd(npc->poseId)) {
            lim = (20.0f * npc->moveSpeed) / 1000.0f;
            step = npc->moveSpeed * npc->moveDt;
            if (step >= lim) {
                step = lim;
            }
            npc->moveOffset.x = step * (f32)sin(npc->moveDir * 6.2831855f / 360.0f);
#if VERSION <= VERSION_DEMO_US
                temp = (f32)cos(npc->moveDir * 6.2831855f / 360.0f);
                npc->moveOffset.z = step * -temp;
#else
            npc->moveOffset.z = step * -(f32)cos(npc->moveDir * 6.2831855f / 360.0f);
            sfxFlag = 0;
            if (psndGetFlag() & PSND_FLAG_DOOR_IN) {
                if (!(animPoseGetMaterialFlag(npc->poseId) & (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2))) {
                    sfxFlag = PSND_SFX_DOOR_MUTE;
                }
            }
            if (npc->footstepAccum > (f32)npc->footstepStride / npc->moveSpeed) {
                if (npc->footstepToggle != 0) {
                    if (npc->footstepSfxA != 0) {
                        psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->footstepSfxA)), &npc->position, 0xff, 0xff, 0, 8);
                    }
                } else {
                    if (npc->footstepSfxB != 0) {
                        psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->footstepSfxB)), &npc->position, 0xff, 0xff, 0, 8);
                    }
                }
                npc->footstepAccum = 0.0f;
                npc->footstepToggle = (s8)(1 - npc->footstepToggle);
            }
            if (npc->footstepSfxB != 0 || npc->footstepToggle != 0) {
                npc->footstepAccum += 1000.0f * npc->moveDt;
            }
#endif
        }
        break;
    case UROURO_STATE_CHOOSE_WAIT:
        work->timer = work->waitTime / 2 + irand(work->waitTime / 2 + 1);
        npc->direction = reviseAngle(npc->direction + (f32)irand(60) - 30.0f);
        npc->moveDir = npc->direction;
        work->count = 0;
        if (work->flags & UROURO_FLAG_TURN_AROUND_UNUSED) {
            work->count = irand(1000) % 3 + 2;
        }
        work->state = UROURO_STATE_WAIT;
        break;
    case UROURO_STATE_WAIT:
        work->timer--;
        if (work->timer == 0) {
            work->timer = work->turnTime / 2 + irand(work->turnTime / 2 + 1);
            if (work->count == 0) {
                work->state = UROURO_STATE_CHOOSE_WALK;
            } else {
                work->count--;
                npc->direction = reviseAngle(180.0f + npc->direction);
            }
        }
        break;
    }
    return EVT_RETURN_BLOCK;
}

