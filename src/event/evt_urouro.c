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

#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)


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
    work->state = 0;
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
    npc->flags |= 0x400000;
    return EVT_RETURN_DONE;
}

USER_FUNC(urouro_main_func) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    MarioWork* mario = marioGetPtr();
    UrouroWork* work = (UrouroWork*)event->lwData[10];
    f32 dist;
    f32 step;
    f32 lim;
    f32 temp;
    s32 sfxFlag;

    switch (work->state) {
    case 0:
        work->timer = work->walkTime / 2 + irand(work->walkTime / 2 + 1);
        work->timer = work->timer - work->timer % 20;
        work->count = 0;
        npc->direction = reviseAngle(npc->direction + (f32)irand(60) - 30.0f);
        npc->moveDir = npc->direction;
        work->state = 1;
        break;
    case 1:
        dist = distABf(npc->position.x, npc->position.z, work->x, work->z);
        if (animPoseGetPeraEnd(npc->poseId)) {
            if (dist > work->radius) {
                if (!(work->flags & 0x1)) {
                    work->flags |= 0x3;
                }
                if (work->flags & 0x2) {
                    npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z, work->x, work->z));
                    npc->moveDir = npc->direction;
                    work->flags &= ~0x2;
                }
            } else if (work->flags & 0x1) {
                work->flags &= ~0x3;
            }
        }
        if (animPoseGetPeraEnd(npc->poseId) && npc == marioGetTouchNpcPtr()
            && fabsf(compAngle(npc->moveDir, mario->directionView)) < 90.0f
            && fabsf(compAngle(angleABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z),
                              mario->directionView)) < 90.0f
            && dist < work->radius) {
            npc->direction = reviseAngle(npc->moveDir - 180.0f);
            npc->moveDir = npc->direction;
            work->flags &= ~0x3;
        }
        if (animPoseGetPeraEnd(npc->poseId) && npc->wallHit != 0) {
            npc->direction = reviseAngle(angleABf(npc->position.x, npc->position.z, work->x, work->z));
            npc->moveDir = npc->direction;
            work->flags &= ~0x3;
        }
        work->timer--;
        if (work->timer == 0) {
            work->state = 2;
            break;
        }
        if (animPoseGetPeraEnd(npc->poseId)) {
            lim = (20.0f * npc->moveSpeed) / 1000.0f;
            step = npc->moveSpeed * npc->moveDt;
            if (step >= lim) {
                step = lim;
            }
            npc->moveOffset.x = step * (f32)sin(npc->moveDir * 6.2831855f / 360.0f);
            temp = (f32)cos(npc->moveDir * 6.2831855f / 360.0f);
            npc->moveOffset.z = step * -temp;
            sfxFlag = 0;
            if (psndGetFlag() & 0x100) {
                if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
                    sfxFlag = 0x10000;
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
        }
        break;
    case 2:
        work->timer = work->waitTime / 2 + irand(work->waitTime / 2 + 1);
        npc->direction = reviseAngle(npc->direction + (f32)irand(60) - 30.0f);
        npc->moveDir = npc->direction;
        work->count = 0;
        if (work->flags & 0x10000) {
            work->count = irand(1000) % 3 + 2;
        }
        work->state = 3;
        break;
    case 3:
        work->timer--;
        if (work->timer == 0) {
            work->timer = work->turnTime / 2 + irand(work->turnTime / 2 + 1);
            if (work->count == 0) {
                work->state = 0;
            } else {
                work->count--;
                npc->direction = reviseAngle(180.0f + npc->direction);
            }
        }
        break;
    }
    return EVT_RETURN_BLOCK;
}

