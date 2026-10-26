#include "evt/evt_npc.h"
#include "party.h"
#include "mario/mario_party.h"
#include "driver/seqdrv.h"
#include "driver/offscreendrv.h"
#include "battle/battle_information.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include "evt/evt_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "eff/n64/eff_kemuri10_n64.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>
#include "sfx_name_word.h"


#ifdef __MWERKS__
#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)
#else
#endif

// clang-format off
// .sdata
EVT_BEGIN(npc_init_evt)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
    END
EVT_END()
// clang-format on

// const char* npcDesc, const char* npcName
USER_FUNC(evt_npc_entry) {
    EvtWord* args = event->args;
    const char *desc, *name;

    desc = (const char*)evtGetValue(event, *args++);
    name = (const char*)evtGetValue(event, *args++);
    if (!animGroupBaseAsync(name, gp->inBattle != 0, NULL)) {
        return EVT_RETURN_BLOCK;
    }
    npcEntry(desc, name);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 slaveId, const char* animName, s32 retIndex, void* deadEvent
USER_FUNC(evt_npc_slave_entry) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 id;
    const char* animName;
    s32 index;
    u8 slaveBuf[0xB8];
    char slaveName[0x20];
    char ident[0x4];
    NpcEntry* slave;
    void* deadEvent;

    name = (const char*)evtGetValue(event, *args++);
    npc = evtNpcNameToPtr(event, name);
    id = evtGetValue(event, *args++);
    animName = (const char*)evtGetValue(event, *args++);
    index = *args++;
    deadEvent = (void*)evtGetValue(event, *args++);
    memset(&slaveBuf, 0, sizeof(slaveBuf));
    sprintf(ident, "_s%d", id);
    strcpy(slaveName, npc->description);
    strcat(slaveName, ident);
    npcEntry(slaveName, animName);
    slave = npcNameToPtr(slaveName);
    slave->flags |= (NPC_FLAG_GRAVITY | NPC_FLAG_UPDATE_BY_EVENT) | 0x18;
    slave->deadEvent = deadEvent;
    slave->battleInfo = npc->battleInfo;
    npcSetSlave(npc, slave, id);
    evtSetValue(event, index, (EvtWord)&slave->description);
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_delete) {
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, *event->args);
    npc = evtNpcNameToPtr(event, name);
    npcDeleteGroup(npc);
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_check_delete) {
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, *event->args);
    npc = evtNpcNameToPtr_NoAssert(event, name);
    if (npc) {
        npcDeleteGroup(npc);
    }
    return EVT_RETURN_DONE;
}

// BOOL inBattle, s32 evtIndex
USER_FUNC(evt_npc_get_ReactionOfLivingBody) {
    EvtWord* args = event->args;
    s32 index, value;
    BOOL inBattle;

    inBattle = evtGetValue(event, args[0]);
    index = args[1];
    value = npcGetReactionOfLivingBody(inBattle);
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

// NpcSetupInfo* setuplist
USER_FUNC(evt_npc_setup) {
    NpcSetupInfo *setuplist, *setup;
    EventEntry* newEvt;
    NpcTribeInfo* tribe;
    NpcEntry* npc;
    s32 evtId;

    setuplist = (NpcSetupInfo*)evtGetValue(event, *event->args);
    fbatGetPointer(); // unused
    if (isFirstCall) {
        for (setup = setuplist; setup->name; setup++) {
            npc = npcNameToPtr_NoAssert(setup->name);
            if (npc) {
                tribe = npc->tribe;
                npc->flags |= setup->flags;
                npc->reactionFlags |= setup->reactionFlags;
                if (setup->initEvent) {
                    evtId = npc->initEvtId;
                    if (evtId) {
                        evtDeleteID(evtId);
                    }
                    newEvt = evtEntry(setup->initEvent, 0, 0);
                    newEvt->wNpcEventType = NPC_EVENT_INIT;
                    newEvt->ownerNPC = npc;
                    npc->initEvtId = newEvt->eventId;
                } else {
                    npc->initEvtId = 0;
                }

                npc->talkEvt = setup->talkEvent;
                npc->deadEvent = setup->deadEvent;
                npc->moveEvent = setup->regularEvent;
                npc->findEvent = setup->findEvent;
                npc->lostEvent = setup->lostEvent;
                npc->returnEvent = setup->returnEvent;
                npc->blowEvent = setup->blowEvent;
                npc->territoryType = setup->territoryType;
                npc->territoryBase = setup->territoryBase;
                npc->territoryLoiter = setup->territoryLoiter;
                npc->territoryHoming = setup->territoryLoiter;
                npc->searchRange = setup->searchRange;
                npc->searchAngle = setup->searchAngle;
                npc->homingRange = setup->homingRange;
                npc->homingAngle = setup->homingAngle;
                npcSetBattleInfo(npc, setup->battleInfoId);
                if (tribe->stayAnim) {
                    strcpy(npc->stayAnim, tribe->stayAnim);
                }
                if (tribe->talkAnim) {
                    strcpy(npc->talkAnim, tribe->talkAnim);
                }
                if (tribe->initialAnim) {
                    strcpy(npc->currentAnim, tribe->initialAnim);
                    animPoseSetAnim(npc->poseId, npc->currentAnim, 1);
                }
            }
        }
        fbatChangeMode(1);
        return EVT_RETURN_BLOCK; // call this function again
    } else {
        for (setup = setuplist; setup->name; setup++) {
            evtId = npcNameToPtr(setup->name)->initEvtId;
            if (evtId && evtCheckID(evtId)) {
                return EVT_RETURN_BLOCK; // call this function again
            }
        }
        for (setup = setuplist; setup->name; setup++) {
            npc = npcNameToPtr(setup->name);
            if (npc->moveEvent) {
                evtId = npc->regularEvtId;
                if (evtId) {
                    evtDeleteID(evtId);
                }
                newEvt = evtEntry(npc->moveEvent, 0, EVENT_START_IMMEDIATE);
                newEvt->wNpcEventType = NPC_EVENT_MOVE;
                newEvt->ownerNPC = npc;
                npc->regularEvtId = newEvt->eventId;
            }
        }
        return EVT_RETURN_DONE;
    }
}

// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_set_position) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    f32 x, y, z;
    Vec temp;

    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    temp = (Vec){ x, y, z };
    npc->prevPosition = temp;
    npc->position = npc->prevPosition;
    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 width
USER_FUNC(evt_npc_set_width) {
    EvtWord* args = event->args;
    const char* name;
    f32 width;

    name = (const char*)evtGetValue(event, args[0]);
    width = evtGetFloat(event, args[1]);
    evtNpcNameToPtr(event, name)->width = width;
    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 width
USER_FUNC(evt_npc_set_height) {
    EvtWord* args = event->args;
    const char* name;
    f32 height;

    name = (const char*)evtGetValue(event, args[0]);
    height = evtGetFloat(event, args[1]);
    evtNpcNameToPtr(event, name)->height = height;
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 evtStoreIndex
USER_FUNC(evt_npc_get_height) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 index;

    name = (const char*)evtGetValue(event, args[0]);
    index = args[1];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, index, npc->height);
    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_set_scale) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    f32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    npc->scale.x = x;
    npc->scale.y = y;
    npc->scale.z = z;
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USER_FUNC(evt_npc_get_scale) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, x, npc->scale.x);
    evtSetFloat(event, y, npc->scale.y);
    evtSetFloat(event, z, npc->scale.z);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USER_FUNC(evt_npc_get_position) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, x, npc->position.x);
    evtSetFloat(event, y, npc->position.y);
    evtSetFloat(event, z, npc->position.z);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USER_FUNC(evt_npc_get_home_position) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, x, npc->territoryBase.x);
    evtSetFloat(event, y, npc->territoryBase.y);
    evtSetFloat(event, z, npc->territoryBase.z);
    return EVT_RETURN_DONE;
}

// devs forgot to change s32 to f32 when copypasting, fix in rewrite
// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_set_home_position) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = (s32)evtGetFloat(event, args[1]);
    y = (s32)evtGetFloat(event, args[2]);
    z = (s32)evtGetFloat(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    npc->territoryBase.x = (f32)x;
    npc->territoryBase.y = (f32)y;
    npc->territoryBase.z = (f32)z;
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USER_FUNC(evt_npc_get_rotate) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, x, npc->rotation.x);
    evtSetFloat(event, y, npc->rotation.y);
    evtSetFloat(event, z, npc->rotation.z);
    return EVT_RETURN_DONE;
}

// devs forgot to use evtGetFloat when copypasting, fix in rewrite
// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_set_rotate) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    f32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    npc->rotation = (Vec){ x, y, z };
    return EVT_RETURN_DONE;
}

// devs forgot to use evtGetFloat when copypasting, fix in rewrite
// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_add_rotate) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    f32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    npc->rotation.x += x;
    npc->rotation.y += y;
    npc->rotation.z += z;
    return EVT_RETURN_DONE;
}

// devs forgot to use evtGetFloat when copypasting, fix in rewrite
// const char* lookupName, f32 x, f32 y, f32 z
USER_FUNC(evt_npc_set_rotate_offset) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    f32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    npc = evtNpcNameToPtr(event, name);
    npc->rotationOffset = (Vec){ x, y, z };
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_move_position) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    f32 speed = evtGetFloat(event, *args++);
    s32 moveFlags = *args;

    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    Vec diff;
    f32 dist;
    f32 step;
    s32 sfxFlag;

    npc->statusFlags |= NPC_STATUS_UPDATE;
    if (moveFlags & 0x2) {
        npc->statusFlags |= NPC_STATUS_NO_FIND_ONCE;
    }

    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->moveAnimState = 0;

        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = npc->position.y;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;

        VECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf((diff.x * diff.x) + (diff.z * diff.z));

        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);
        npc->moveSpeed = speed;
        npc->moveTime = moveTime;
        npc->moveOffset.y = 0.0f;

        // No speed AND no time -> default to 1.0
        if (npc->moveSpeed <= 0.1f && npc->moveTime == 0.0f) {
            npc->moveSpeed = 1.0f;
        }
        // Time given but no speed -> derive speed, then run as speed-mode
        if (npc->moveSpeed <= 0.1f) {
            npc->moveSpeed = dist / npc->moveTime;
            npc->moveTime = 0.0f;
        }

        npc->footstepAccum = 0.0f;
        npc->footstepToggle = TRUE;
        event->userdata[0] = 0;
    }

    event->userdata[0]++;

    // flag 0x4: bail if blocked and stuck > 5 frames
    if ((moveFlags & 0x4) && npc->wallHit && event->userdata[0] > 5) {
        return EVT_RETURN_DONE;
    }

    VECSubtract(&npc->jumpTargetPos, &npc->position, &diff);
    dist = sqrtf((diff.x * diff.x) + (diff.z * diff.z));
    step = (20.0f * npc->moveSpeed) / 1000.0f;

    if (npc->moveTime) {
        // timed move: moveElapsed is the progress, advanced externally
        if (npc->moveTime <= npc->moveElapsed) {
            npc->moveSpeed = 0.0f;
            return EVT_RETURN_DONE;
        }
    } else {
        // speed move: snap when within a step (or essentially on top of target)
        if (!npc->moveTime && dist <= step) {
            npc->moveSpeed = 0.0f;
            return EVT_RETURN_DONE;
        }
        if (dist < 0.0005f) {
            npc->moveSpeed = 0.0f;
            return EVT_RETURN_DONE;
        }
    }

    // Re-aim at the target each frame unless direction is locked
    if ((npc->flags & NPC_FLAG_IN_USE) && !(npc->statusFlags & NPC_STATUS_AIRBORNE)) {
        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);
    }

    // Turn the body toward travel (unless mid battle-transition states 5/6)
    if ((npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) && npc->moveSpeed > 0.1f) {
        if (npc != fbatGetPointer()->target || (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
            npcTuningRy(npc, npc->moveDir);
        }
    }

    // Integrate move offset along heading (flag 0x10000 freezes it)
    if (!(npc->statusFlags & NPC_STATUS_STOPPED)) {
        f32 temp = (f32)sin(npc->moveDir * 6.2831855f / 360.0f);
        npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
        temp = (f32)cos(npc->moveDir * 6.2831855f / 360.0f);
        npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);
    }

    // Footstep SFX
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
        npc->footstepToggle = (s8)(1 - npc->footstepToggle); // alternate L/R
    }

    if (npc->footstepSfxB != 0 || npc->footstepToggle != 0) {
        npc->footstepAccum += 1000.0f * npc->moveDt;
    }

    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_npc_jump_position) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetY = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    f32 speed = evtGetFloat(event, *args++);
    f32 arcHeight = evtGetFloat(event, *args++);
    s32 mode = evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    f32 arcWork = arcHeight;
    Vec diff;
    f32 dist;
    f32 dy;
    f32 t;
    f32 halfT;
    f32 peak;
    s32 sfxFlag;
    f32 temp;

    npc->statusFlags |= NPC_STATUS_UPDATE;
    npc->statusFlags |= NPC_STATUS_NO_FIND_ONCE;

    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->jumpTicks = 0;
        npc->moveAnimState = 0;

        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = targetY;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;

        VECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf(diff.x * diff.x + diff.z * diff.z);
        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);

        npc->moveSpeed = speed;
        npc->moveTime = 0.0f;
        npc->moveOffset.y = 0.0f;

        sfxFlag = 0;
        if (psndGetFlag() & 0x100 && !(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
        if (npc->jumpSfx != 0) {
            psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->jumpSfx)), &npc->position, 0xff, 0xff, 0, 8);
        }

        npc->jumpVelY = 0.0f;
        npc->jumpYScale = 1.0f;
        dy = npc->jumpTargetPos.y - npc->jumpStartPos.y;

        if (arcHeight) {
            if (dy > 0.0f) {
                arcWork += dy;
            }
            npc->jumpVelY = 0.0f;

            switch (mode) {
                case 0:
                    if (moveTime == 0.0f) {
                        npc->jumpVelY = arcHeight;
                        goto end1;
                    }
                    if (npc->moveSpeed > 0.1) {
                        t = dist / npc->moveSpeed;
                        if (t == 0.0f) {
                            t = moveTime;
                        }
                    } else {
                        npc->moveSpeed = dist / moveTime;
                        t = moveTime;
                    }
                    if (t == 0.0f) {
                        t = 1.0f;
                    }
                    halfT = 0.5f * t;
                    npc->jumpVelY = -((-980.0f * npc->jumpGravity) * t * t) / t;
                    peak = (npc->jumpVelY * halfT) + ((-980.0f * npc->jumpGravity) * halfT * halfT);
                    if (peak != 0.0f) {
                        npc->jumpYScale = arcWork / peak;
                    }
                    goto end1;

                case 1:
                    if (moveTime == 0.0f) {
                        npc->jumpVelY = arcHeight;
                        goto end1;
                    }
                    halfT = 0.5f * moveTime;
                    npc->jumpVelY = -((-980.0f * npc->jumpGravity) * moveTime * moveTime) / moveTime;
                    peak = (npc->jumpVelY * halfT) + ((-980.0f * npc->jumpGravity) * halfT * halfT);
                    if (peak != 0.0f) {
                        npc->jumpYScale = arcWork / peak;
                    }
                    goto end1;
                default:
                    return EVT_RETURN_YIELD;
            }
        } else {
            if (dy >= 0.0f) {
                return EVT_RETURN_YIELD;
            }
            switch (mode) {
                case 0:
                    if (npc->moveSpeed > 0.1) {
                        if (moveTime > 0.0f) {
                            t = moveTime;
                        } else {
                            t = dist / npc->moveSpeed;
                        }
                        if (t > 0.0f) {
                            npc->jumpYScale = dy / (-980.0f * npc->jumpGravity * t * t);
                        } else {
                            npc->jumpYScale = 1.0f;
                        }
                    } else if (moveTime > 0.0f) {
                        npc->jumpGravity = (dy / (moveTime * moveTime)) / -980.0f;
                    }
                    goto end1;
                default:
                    return EVT_RETURN_YIELD;
                case 1:
                end1:
                    return EVT_RETURN_BLOCK;
            }
        }
    }

    // ---- per-frame ----
    if ((npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) && npc->moveSpeed > 0.1f) {
        if (npc != fbatGetPointer()->target || (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
            npcTuningRy(npc, npc->moveDir);
        }
    }

    temp = (f32)sin(npc->moveDir * 6.2831855f / 360.0f);
    npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
    temp = (f32)cos(npc->moveDir * 6.2831855f / 360.0f);
    npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);

    if (npc->statusFlags & NPC_STATUS_AIRBORNE) {
        return EVT_RETURN_BLOCK; // still airborne
    }

    // landing
    npc->moveSpeed = 0.0f;
    npc->jumpVelY = 0.0f;
    npc->jumpYScale = 1.0f;
    npc->jumpGravity = 1.0f;

    sfxFlag = 0;
    if (psndGetFlag() & 0x100) {
        if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
    }
    if (npc->landSfx != 0) {
        psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->landSfx)), &npc->position, 0xff, 0xff, 0, 8);
    }
    return EVT_RETURN_YIELD;
}

USER_FUNC(evt_npc_jump_position_nohit) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    f32 targetX = (f32)evtGetValue(event, *args++);
    f32 targetY = (f32)evtGetValue(event, *args++);
    f32 targetZ = (f32)evtGetValue(event, *args++);
    s32 duration = evtGetValue(event, *args++);
    f32 arcHeight = (f32)evtGetValue(event, *args++);
    s32 sfxFlag;
    u32 ticksPerMs;
    s32 elapsed;
    f32 t;
    Vec endPos;

    if (isFirstCall) {
        sfxFlag = 0;
        event->savedTime = event->lifetime;
        npc->jumpStartPos = npc->position;

        event->userdataf[1] = -4.0f * arcHeight;
        event->userdataf[2] = (targetY - npc->jumpStartPos.y) - event->userdataf[1];
        event->userdata3 = npc->flags & NPC_FLAG_SHADOW_NO_FLOOR;
        npc->flags &= ~NPC_FLAG_SHADOW_NO_FLOOR;

        if (psndGetFlag() & 0x100) {
            if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
                sfxFlag = 0x10000;
            }
        }
        if (npc->jumpSfx != 0) {
            psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->jumpSfx)), &npc->position, 0xff, 0xff, 0, 8);
        }
    }

    elapsed = (s32)OSTicksToMilliseconds((OSTick)event->lifetime - (OSTick)event->savedTime);

    if (elapsed < duration) {
        t = (f32)elapsed / (f32)duration;
        npc->position.x = intplGetValue(EASING_LINEAR, npc->jumpStartPos.x, targetX, (s32)elapsed, duration);
        npc->position.z = intplGetValue(EASING_LINEAR, npc->jumpStartPos.z, targetZ, (s32)elapsed, duration);
        npc->position.y = npc->jumpStartPos.y + t * ((event->userdataf[1] * t) + event->userdataf[2]);
        return EVT_RETURN_BLOCK;
    }

    npc->position = (Vec){ targetX, targetY, targetZ };
    npc->flags |= event->userdata3;

    sfxFlag = 0;
    if (psndGetFlag() & 0x100) {
        if (!(animPoseGetMaterialFlag(npc->poseId) & 0x1800)) {
            sfxFlag = 0x10000;
        }
    }
    if (npc->landSfx != 0) {
        psndSFXOnEx_3D(SFX_NAME_WORD(sfxFlag | searchPSSFXList(npc->landSfx)), &npc->position, 0xff, 0xff, 0, 8);
    }

    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 x, f32 y, f32 z, s32 msec, f32 speed, f32 arcHeight,
// s32 intplMode, s32 flags
USER_FUNC(evt_npc_glide_position) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetY = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    f32 speed = evtGetFloat(event, *args++);
    f32 arcHeight = evtGetFloat(event, *args++);
    s32 mode = evtGetValue(event, *args++);
    s32 moveFlags = *args++;

    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    Vec diff;
    f32 dist;
    f32 total, elapsed, dt;
    f32 curr, prev, delta, dx, dz, sinPrev, sinCurr;
    BOOL finished;
    s32 sfxFlag;

    npc->statusFlags |= NPC_STATUS_UPDATE;
    if (!(moveFlags & 0x2)) {
        npc->statusFlags |= NPC_STATUS_NO_FIND_ONCE;
    }

    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->moveAnimState = 0;

        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = targetY;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;

        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

        if (moveTime <= 0.0f) {
            if (speed == 0.0f) {
                return EVT_RETURN_YIELD;
            }
            if (dist == 0.0f) {
                return EVT_RETURN_YIELD;
            }
            moveTime = dist / speed;
        }
        npc->moveTime = moveTime;
        npc->moveSpeed = speed;
        if (npc->moveSpeed <= 0.1f) {
            npc->moveSpeed = dist / npc->moveTime;
        }

        npc->footstepAccum = 0.0f;
        npc->footstepToggle = TRUE;
        return EVT_RETURN_BLOCK;
    }

    // flag 0x1: bail if blocked
    if ((moveFlags & 0x1) && npc->wallHit) {
        return EVT_RETURN_DONE;
    }

    elapsed = npc->moveElapsed;
    finished = FALSE;
    total = npc->moveTime;
    dt = npc->moveDt;
    if (elapsed >= total) {
        finished = TRUE;
        if (elapsed > total) {
            dt -= total - elapsed;
            if (dt <= 0.0f) {
                return EVT_RETURN_DONE;
            }
        }
    }

    dt = (elapsed - dt) / total;
    curr = _intplGetFloat(mode, elapsed / total);
    prev = _intplGetFloat(mode, dt);
    delta = curr - prev;
    dx = npc->jumpTargetPos.x - npc->jumpStartPos.x;
    dz = npc->jumpTargetPos.z - npc->jumpStartPos.z;
    npc->moveOffset.x += dx * delta;
    npc->moveOffset.z += dz * delta;

    // Turn the body toward travel (unless mid battle-transition states 5/6)
    if (npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) {
        if (dx || dz) {
            if (npc != fbatGetPointer()->target ||
                (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
                // bug: uses dx for both components
                npcTuningRy(npc, angleABf(0.0f, 0.0f, dx, dx));
            }
        }
    }

    dx = npc->jumpTargetPos.y - npc->jumpStartPos.y;
    npc->moveOffset.y += dx * delta;

    sinPrev = (f32)sin(3.1415927f * prev);
    sinCurr = (f32)sin(3.1415927f * curr);
    npc->moveOffset.y += arcHeight * sinCurr - arcHeight * sinPrev;

    // Footstep SFX
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

    if (finished) {
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

// const char* lookupName
USER_FUNC(evt_npc_return_interrupt) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    npc->interruptFlags |= NPC_INTERRUPT_MOVE;
    return EVT_RETURN_YIELD;
}

// const char* lookupName, const char* targetName, s32 msec, f32 speed, f32 stopDistance,
// f32 turnRate, s32 flags
USER_FUNC(evt_npc_homing_target) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    const char* targetName = (const char*)evtGetValue(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    f32 speed = evtGetFloat(event, *args++);
    f32 stopDistance = evtGetFloat(event, *args++);
    f32 turnRate = evtGetFloat(event, *args++);
    s32 moveFlags = *args++;

    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    Vec diff;
    f32 dist, angle, delta, temp, dx, dz;
    BOOL finished;
    s32 sfxFlag;

    npc->statusFlags |= NPC_STATUS_UPDATE;

    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->moveAnimState = 0;

        if (strcmp(targetName, "mario") == 0) {
            MarioWork* mario;
            event->userdata[0] = 0;
            event->userdatap[1] = marioGetPtr();
            mario = event->userdatap[1];
            dx = mario->position.x - npc->position.x;
            dz = mario->position.z - npc->position.z;
        } else {
            NpcEntry* other;
            event->userdata[0] = 1;
            event->userdatap[1] = evtNpcNameToPtr(event, targetName);
            other = event->userdatap[1];
            dx = other->position.x - npc->position.x;
            dz = other->position.z - npc->position.z;
        }
        npc->moveDir = angleABf(0.0f, 0.0f, dx, dz);
        npc->moveSpeed = speed;
        if (speed == 0.0f) {
            return EVT_RETURN_DONE;
        }
        npc->moveTime = moveTime;

        // Turn the body toward travel (unless mid battle-transition states 5/6)
        if ((npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) && npc->moveSpeed) {
            if (npc != fbatGetPointer()->target ||
                (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
                npcTuningRy(npc, npc->moveDir);
            }
        }

        npc->footstepAccum = 0.0f;
        npc->footstepToggle = TRUE;
        return EVT_RETURN_BLOCK;
    }

    dist = 1000.0f;
    if (!(npc->statusFlags & NPC_STATUS_AIRBORNE)) {
        // track the target's current position
        if (event->userdata[0] == 0) {
            MarioWork* mario = event->userdatap[1];
            npc->jumpTargetPos.x = mario->position.x;
            npc->jumpTargetPos.y = mario->position.y;
            npc->jumpTargetPos.z = mario->position.z;
        } else {
            NpcEntry* other = event->userdatap[1];
            npc->jumpTargetPos.x = other->position.x;
            npc->jumpTargetPos.y = other->position.y;
            npc->jumpTargetPos.z = other->position.z;
        }
        npc->jumpStartPos = npc->position;

        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf((diff.x * diff.x) + (diff.z * diff.z));
        angle = angleABf(0.0f, 0.0f, diff.x, diff.z);

        // turn toward the target, limited by turnRate
        delta = compAngle(npc->moveDir, angle);
        if (fabsf(delta) <= 60.0f * turnRate * npc->moveDt) {
            npc->moveDir = angle;
        } else {
            if (delta > 0.0f) {
                npc->moveDir += 60.0f * turnRate * npc->moveDt;
            } else {
                npc->moveDir -= 60.0f * turnRate * npc->moveDt;
            }
            npc->moveDir = reviseAngle(npc->moveDir);
        }

        if ((npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) && npc->moveSpeed) {
            if (npc != fbatGetPointer()->target ||
                (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
                npcTuningRy(npc, npc->moveDir);
            }
        }
    }

    finished = FALSE;
    PSVECSubtract(&npc->jumpTargetPos, &npc->position, &diff);
    sqrtf((diff.x * diff.x) + (diff.z * diff.z));

    if (npc->moveTime && npc->moveTime <= npc->moveElapsed) {
        finished = TRUE;
    } else if (stopDistance > 0.0f && dist < stopDistance) {
        finished = TRUE;
    } else if ((moveFlags & 0x4) && npc->wallHit) {
        finished = TRUE;
    }
    if (finished) {
        npc->moveSpeed = 0.0f;
        return EVT_RETURN_YIELD;
    }

    temp = (f32)sin(6.2831855f * npc->moveDir / 360.0f);
    npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
    temp = (f32)cos(6.2831855f * npc->moveDir / 360.0f);
    npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);

    // Footstep SFX
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

    return EVT_RETURN_BLOCK;
}

// intplGetValue, but for a normalized t in [0, 1]
f32 _intplGetFloat(s32 mode, f32 t) {
    f32 v, u, c;
    f32 r;

    if (t > 1.0f) {
        t = 1.0f;
    }
    switch (mode) {
        case 0:
            return t;
        case 1:
            return t * t;
        case 2:
            return t * (t * t);
        case 3:
            return t * (t * (t * t));
        case 7:
            c = (f32)cos(4.0f * (t / 3.1415927f));
            u = 1.0f - t;
            v = u * (u * c);
            return 1.0f - v;
        case 8:
            c = (f32)cos((4.0f * ((t * t) / 3.1415927f)) / 15.0f);
            u = 1.0f - t;
            v = u * (u * c);
            return 1.0f - v;
        case 4:
            u = 1.0f - t;
            return 1.0f - u * u;
        case 5:
            u = 1.0f - t;
            return 1.0f - u * (u * u);
        case 6:
            u = 1.0f - t;
            return 1.0f - u * (u * (u * u));
        case 10:
            c = (f32)cos((4.0f * ((t * t) / 3.1415927f)) / 40.0f);
            u = 1.0f - t;
            v = u * (u * c);
            if (v < 0.0f) {
                v = -v;
            }
            return 1.0f - v;
        case 11:
            c = (f32)cos(3.1415927f * t);
            v = 0.5f;
            u = 1.0f - c;
            return u * v;
        case 12:
            c = (f32)sin(1.5707964f * t);
            r = c;
            break;
        case 13:
            return 1.0f - (f32)cos(1.5707964f * t);
        default:
            r = 0.0f;
            break;
    }
    return r;
}

// const char* lookupName, f32 ry
USER_FUNC(evt_npc_set_ry) {
    EvtWord* args = event->args;
    const char* name;
    f32 ry;

    name = (const char*)evtGetValue(event, args[0]);
    ry = evtGetFloat(event, args[1]);
    evtNpcNameToPtr(event, name)->direction = ry;
    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 side (0 = left of camera, else right)
USER_FUNC(evt_npc_set_ry_lr) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 side, angle;
    f32 left, right;

    name = (const char*)evtGetValue(event, args[0]);
    side = evtGetFloat(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    angle = reviseAngle((camGetPtr(CAMERA_3D)->viewYaw * 100.0f + 5.0f) / 100.0f);
    left = (angle - 90) % 360;
    right = (angle + 90) % 360;
    if (side == 0) {
        npc->direction = left;
    } else {
        npc->direction = right;
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_reverse_ry) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    npc->direction = reviseAngle(180.0f + npc->direction);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 dirStoreIndex
USER_FUNC(evt_npc_get_dir) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[1], (s32)npc->direction);
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* animName
USER_FUNC(evt_npc_set_anim) {
    EvtWord* args = event->args;
    const char* name;
    const char* anim;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    anim = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    if (event->wNpcEventType == NPC_EVENT_MOVE) {
        strcpy(npc->currentAnim, anim);
    }
    animPoseSetAnim(npc->poseId, anim, TRUE);
    return EVT_RETURN_DONE;
}

#define NPC_SET_TRIBE_ANIM(funcName, field)                                      \
    USER_FUNC(funcName) {                                                        \
        EvtWord* args = event->args;                                             \
        const char* name;                                                        \
        NpcEntry* npc;                                                           \
        const char* anim;                                                        \
                                                                                 \
        name = (const char*)evtGetValue(event, args[0]);                         \
        npc = evtNpcNameToPtr(event, name);                                      \
        anim = npc->tribe->field;                                                \
        if (event->wNpcEventType == NPC_EVENT_MOVE) {                            \
            strcpy(npc->currentAnim, anim);                                      \
        }                                                                        \
        animPoseSetAnim(npc->poseId, anim, TRUE);                                \
        return EVT_RETURN_DONE;                                                  \
    }

// const char* lookupName
NPC_SET_TRIBE_ANIM(evt_npc_set_damage_anim, damageAnimation)
NPC_SET_TRIBE_ANIM(evt_npc_set_confuse_anim, confuseAnimation)
NPC_SET_TRIBE_ANIM(evt_npc_set_stop_anim, stopAnimation)
NPC_SET_TRIBE_ANIM(evt_npc_set_stay_anim, stayAnim)
NPC_SET_TRIBE_ANIM(evt_npc_set_talk_anim, talkAnim)
NPC_SET_TRIBE_ANIM(evt_npc_set_walk_anim, walkAnimation)
NPC_SET_TRIBE_ANIM(evt_npc_set_run_anim, runAnimation)

// const char* lookupName, const char* paperGroup
USER_FUNC(evt_npc_set_paper) {
    EvtWord* args = event->args;
    const char* name;
    const char* group;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    group = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    animPoseSetPaperAnimGroup(npc->poseId, group, TRUE);
    animPosePaperPeraOn(npc->poseId);
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* paperAnim
USER_FUNC(evt_npc_set_paper_anim) {
    EvtWord* args = event->args;
    const char* name;
    const char* anim;

    name = (const char*)evtGetValue(event, args[0]);
    anim = (const char*)evtGetValue(event, args[1]);
    animPoseSetPaperAnim(evtNpcNameToPtr(event, name)->poseId, anim, TRUE);
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_clear_paper) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    animPoseSetPaperAnimGroup(npc->poseId, NULL, TRUE);
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* animName
USER_FUNC(evt_npc_set_force_regl_anim) {
    EvtWord* args = event->args;
    const char* name;
    const char* anim;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    anim = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    strcpy(npc->currentAnim, anim);
    animPoseSetAnim(npc->poseId, anim, TRUE);
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_wait_pera) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    if (animPoseGetPeraEnd(npc->poseId)) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

// const char* lookupName, f32 loopCount
USER_FUNC(evt_npc_wait_anim) {
    EvtWord* args = event->args;
    const char* name;
    f32 loops;

    name = (const char*)evtGetValue(event, args[0]);
    loops = evtGetFloat(event, args[1]);
    if (animPoseGetLoopTimes(evtNpcNameToPtr(event, name)->poseId) >= loops) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

// s32 type (off/on), const char* npcName, s32 mask
USER_FUNC(evt_npc_flag_onoff) {
    EvtWord* args = event->args;
    s32 type, mask;
    const char* name;
    NpcEntry* npc;

    type = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    mask = evtGetValue(event, args[2]);
    npc = evtNpcNameToPtr(event, name);
    switch (type) {
        case 0:
            npc->flags &= (u32)~mask;
            break;
        case 1:
            npc->flags |= mask;
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 mask, s32 storeIndex
USER_FUNC(evt_npc_flag_check) {
    EvtWord* args = event->args;
    const char* name;
    s32 mask;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    mask = evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], npc->flags & mask);
    return EVT_RETURN_DONE;
}

// s32 type (off/on), const char* npcName, s32 mask
USER_FUNC(evt_npc_reaction_flag_onoff) {
    EvtWord* args = event->args;
    s32 type, mask;
    const char* name;
    NpcEntry* npc;

    type = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    mask = evtGetValue(event, args[2]);
    npc = evtNpcNameToPtr(event, name);
    switch (type) {
        case 0:
            npc->reactionFlags &= ~mask;
            break;
        case 1:
            npc->reactionFlags |= mask;
    }
    return EVT_RETURN_DONE;
}

// s32 type (off/on), const char* npcName, s32 mask
USER_FUNC(evt_npc_status_onoff) {
    EvtWord* args = event->args;
    s32 type, mask;
    const char* name;
    NpcEntry* npc;

    type = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    mask = evtGetValue(event, args[2]);
    npc = evtNpcNameToPtr(event, name);
    switch (type) {
        case 0:
            npc->statusFlags &= ~(u32)mask;
            break;
        case 1:
            npc->statusFlags |= mask;
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 mask, s32 storeIndex
USER_FUNC(evt_npc_status_check) {
    EvtWord* args = event->args;
    const char* name;
    s32 mask;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    mask = evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], npc->statusFlags & mask);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 type (off/on)
USER_FUNC(evt_npc_pera_onoff) {
    EvtWord* args = event->args;
    const char* name;
    s32 type;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    switch (type) {
        case 0:
            animPosePeraOff(npc->poseId);
            break;
        case 1:
            animPosePeraOn(npc->poseId);
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, CameraId cameraId
USER_FUNC(evt_npc_set_camid) {
    EvtWord* args = event->args;
    const char* name;
    s32 cameraId;

    name = (const char*)evtGetValue(event, args[0]);
    cameraId = evtGetValue(event, args[1]);
    evtNpcNameToPtr(event, name)->cameraId = cameraId;
    return EVT_RETURN_DONE;
}

f32 _get_target_dir(EventEntry* event, NpcEntry* npc, const char* targetName) {
    Vec target, position, diff;

    if (strcmp(targetName, "mario") == 0) {
        MarioWork* mario = marioGetPtr();
        target.x = mario->position.x;
        target.y = mario->position.y;
        target.z = mario->position.z;
    } else {
        NpcEntry* other = evtNpcNameToPtr(event, targetName);
        target.x = other->position.x;
        target.y = other->position.y;
        target.z = other->position.z;
    }
    position = npc->position;
    PSVECSubtract(&target, &position, &diff);
    return angleABf(0.0f, 0.0f, diff.x, diff.z);
}

// const char* lookupName, const char* targetName, f32 dirStoreIndex
USER_FUNC(evt_get_target_dir) {
    EvtWord* args = event->args;
    const char* name;
    const char* target;
    s32 index;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    target = (const char*)evtGetValue(event, args[1]);
    index = args[2];
    npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, index, _get_target_dir(event, npc, target));
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* targetName
USER_FUNC(evt_set_dir_to_target) {
    EvtWord* args = event->args;
    const char* name;
    const char* target;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    target = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    npc->direction = _get_target_dir(event, npc, target);
    return EVT_RETURN_DONE;
}

// const char* lookupName, unused
USER_FUNC(evt_set_dir_to_home) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    npc->direction = angleABf(npc->position.x, npc->position.z, npc->territoryBase.x,
                              npc->territoryBase.z);
    return EVT_RETURN_DONE;
}

// f32 x (in/out), f32 z (in/out), f32 dir, f32 dist
USER_FUNC(evt_npc_add_dirdist) {
    EvtWord* args = event->args;
    s32 xIndex, zIndex;
    f32 dir, dist, x, sinv, rad, z, cosv;

    xIndex = args[0];
    zIndex = args[1];
    dir = evtGetFloat(event, args[2]);
    dist = evtGetFloat(event, args[3]);
    x = evtGetFloat(event, xIndex);
    z = evtGetFloat(event, zIndex);
    rad = 6.2831855f * dir / 360.0f;
    sinv = (f32)sin(rad);
    cosv = (f32)cos(rad);
    x = x + dist * sinv;
    z = z - dist * cosv;
    evtSetFloat(event, xIndex, x);
    evtSetFloat(event, zIndex, z);
    return EVT_RETURN_DONE;
}

// f32 dir (in/out), f32 min, f32 max
USER_FUNC(evt_npc_get_loiter_dir) {
    EvtWord* args = event->args;
    s32 index;
    f32 min, max, range, dir;

    index = args[0];
    min = evtGetFloat(event, args[1]);
    max = evtGetFloat(event, args[2]);
    dir = evtGetFloat(event, index);
    range = max - min;
    if (range <= 0.0f) {
        dir = dir + min;
    } else {
        dir = dir + (min + rand() / range);
    }
    evtSetFloat(event, index, reviseAngle(dir));
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 type, void* evt
USER_FUNC(evt_npc_change_interrupt) {
    EvtWord* args = event->args;
    const char* name;
    s32 type;
    void* evt;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    evt = (void*)args[2];
    npc = evtNpcNameToPtr(event, name);
    switch (type) {
        case 1:
            npc->moveEvent = evt;
            break;
        case 2:
            npc->findEvent = evt;
            break;
        case 3:
            npc->lostEvent = evt;
            break;
        case 4:
            npc->returnEvent = evt;
            break;
        case 5:
            npc->blowEvent = evt;
            break;
        case 6:
            npc->talkEvt = evt;
            break;
        case 8:
            npc->deadEvent = evt;
            break;
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 storeIndex
USER_FUNC(evt_npc_get_reglid) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[1], npc->regularEvtId);
    return EVT_RETURN_YIELD;
}

// const char* lookupName
USER_FUNC(evt_npc_restart_regular_event) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    npc->interruptFlags |= NPC_INTERRUPT_MOVE;
    npcCheckInterrupt(npc);
    return EVT_RETURN_DONE;
}

// const char* lookupName, f32 base, f32 amount, (out) s32 base storeIndex
USER_FUNC(evt_npc_facedirection_add) {
    EvtWord* args = event->args;
    const char* name;
    f32 base, amount, sign;

    name = (const char*)evtGetValue(event, args[0]);
    base = evtGetFloat(event, args[1]);
    amount = evtGetFloat(event, args[2]);
    if (!strcmp(name, "mario")) {
        MarioWork* mario = marioGetPtr();
        if (180.0f == mario->displayDirection) {
            sign = 1.0f;
        } else {
            sign = -1.0f;
        }
    } else {
        if (90.0f == npcTransRytoFaceDir(evtNpcNameToPtr(event, name))) {
            sign = 1.0f;
        } else {
            sign = -1.0f;
        }
    }
    evtSetValue(event, event->args[1], (s32)(base + amount * sign));
    return EVT_RETURN_DONE;
}

NpcEntry* evtNpcNameToPtr(EventEntry* event, const char* name) {
    if (!strcmp(name, "me")) {
        return event->ownerNPC;
    }
    if (!strcmp(name, "party")) {
        return (NpcEntry*)-1;
    }
    if (!strcmp(name, "(^x^)party")) {
        return (NpcEntry*)-1;
    }
    if (!strcmp(name, "extparty")) {
        return (NpcEntry*)-2;
    }
    if (!strcmp(name, "mario")) {
        return (NpcEntry*)-3;
    }
    if (!strcmp(name, "master")) {
        return event->ownerNPC->master;
    }
    if (!strcmp(name, "slave_0")) {
        return event->ownerNPC->slaves[0];
    }
    if (!strcmp(name, "slave_1")) {
        return event->ownerNPC->slaves[1];
    }
    if (!strcmp(name, "slave_2")) {
        return event->ownerNPC->slaves[2];
    }
    if (!strcmp(name, "slave_3")) {
        return event->ownerNPC->slaves[3];
    }
    return npcNameToPtr(name);
}

NpcEntry* evtNpcNameToPtr_NoAssert(EventEntry* event, const char* name) {
    if (!strcmp(name, "me")) {
        return event->ownerNPC;
    }
    if (!strcmp(name, "party")) {
        return (NpcEntry*)-1;
    }
    if (!strcmp(name, "(^x^)party")) {
        return (NpcEntry*)-1;
    }
    if (!strcmp(name, "extparty")) {
        return (NpcEntry*)-2;
    }
    if (!strcmp(name, "mario")) {
        return (NpcEntry*)-3;
    }
    if (!strcmp(name, "master")) {
        return event->ownerNPC->master;
    }
    if (!strcmp(name, "slave_0")) {
        return event->ownerNPC->slaves[0];
    }
    if (!strcmp(name, "slave_1")) {
        return event->ownerNPC->slaves[1];
    }
    if (!strcmp(name, "slave_2")) {
        return event->ownerNPC->slaves[2];
    }
    if (!strcmp(name, "slave_3")) {
        return event->ownerNPC->slaves[3];
    }
    return npcNameToPtr_NoAssert(name);
}

USER_FUNC(evt_npc_stop_for_event) {
    npcStopForEvent();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_start_for_event) {
    npcStartForEvent();
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_stop_for_one_event) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    npcStopForOneEvent(npc);
    return EVT_RETURN_DONE;
}

// const char* lookupName
USER_FUNC(evt_npc_start_for_one_event) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    npcStartForOneEvent(npc);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_change_fbat_mode) {
    s16 mode = evtGetValue(event, *event->args);
    fbatChangeMode(mode);
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* offscreenName
USER_FUNC(evt_npc_set_offscreen) {
    EvtWord* args = event->args;
    const char* name;
    const char* offscreen;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    offscreen = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    if (offscreen == NULL) {
        npc->offscreenId = -1;
    } else {
        npc->offscreenId = offscreenNameToId(offscreen);
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, u8 r, u8 g, u8 b, u8 a
USER_FUNC(evt_npc_set_color) {
    EvtWord* args = event->args;
    const char* name;
    u8 r, g, b, a;

    name = (const char*)evtGetValue(event, args[0]);
    r = evtGetValue(event, args[1]);
    g = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    a = evtGetValue(event, args[4]);
    name = evtNpcNameToPtr(event, name)->description;
    npcSetColor(name, (GXColor){ r, g, b, a });
    return EVT_RETURN_DONE;
}

// BOOL enable, const char* lookupName
USER_FUNC(evt_npc_blur_onoff) {
    EvtWord* args = event->args;
    BOOL enable;
    const char* name;

    enable = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    name = evtNpcNameToPtr(event, name)->description;
    if (enable) {
        npcBlurOn(name);
    } else {
        npcBlurOff(name);
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, BOOL storeIndex (out)
USER_FUNC(evt_npc_check) {
    EvtWord* args = event->args;

    if (npcNameToPtr_NoAssert((const char*)evtGetValue(event, args[0])) == NULL) {
        evtSetValue(event, args[1], FALSE);
    } else {
        evtSetValue(event, args[1], TRUE);
    }
    return EVT_RETURN_DONE;
}

// const char* lookupName, s16 mode
USER_FUNC(evt_npc_set_attack_mode) {
    EvtWord* args = event->args;
    const char* name;
    s16 mode;

    name = (const char*)evtGetValue(event, args[0]);
    mode = evtGetValue(event, args[1]);
    evtNpcNameToPtr(event, name)->attackMode = mode;
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 battleInfoId
USER_FUNC(evt_npc_set_battle_info) {
    EvtWord* args = event->args;
    const char* name;
    s32 battleInfoId;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    battleInfoId = evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    npcSetBattleInfo(npc, battleInfoId);
    return EVT_RETURN_DONE;
}

// const char* lookupName, u8 rule0, u8 rule1, u8 rule2
USER_FUNC(evt_npc_set_battle_rule) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 rule0, rule1, rule2;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    rule0 = evtGetValue(event, args[1]);
    rule1 = evtGetValue(event, args[2]);
    rule2 = evtGetValue(event, args[3]);
    npc->battleInfo.battleRule[0] = rule0;
    npc->battleInfo.battleRule[1] = rule1;
    npc->battleInfo.battleRule[2] = rule2;
    return EVT_RETURN_DONE;
}

// const char* lookupName, unused
USER_FUNC(evt_npc_battle_start) {
    EvtWord* args = event->args;
    const char* name;
    FieldBattleData* fbat;
    FieldBattleInfo* info;
    NpcEntry* npc;
    PartyEntry* party;

    name = (const char*)evtGetValue(event, args[0]);
    evtGetValue(event, args[1]);
    fbat = fbatGetPointer();
    npc = evtNpcNameToPtr(event, name);
    info = &fbat->info;
    if (isFirstCall) {
        fbat->target = npc;
#ifdef VERSION_G8ME01
        marioFBattlePrepare();
#endif
        marioCtrlOff();
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            partyCtrlOff();
        }
        BattleInformationSetResult(info, 0);
        psndSetFlag(0x10);
        seqSetSeq(SEQ_BATTLE, NULL, NULL);
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_wait_battle_end) {
    FieldBattleInfo* info = &fbatGetPointer()->info;
    PartyEntry* party;

    if (seqGetSeq() != SEQ_GAME) {
        return EVT_RETURN_BLOCK;
    }
    if (BattleInformationGetResult(info) == 0) {
        return EVT_RETURN_BLOCK;
    }
    marioCtrlOn();
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        partyCtrlOn();
    }
    L_psndBGM_stop();
#ifdef VERSION_G8ME01
    marioFBattlePost();
#endif
    return EVT_RETURN_DONE;
}

// s32 resultStoreIndex
USER_FUNC(evt_npc_get_battle_result) {
    EvtWord* args = event->args;
    s32 index = args[0];

    evtSetValue(event, index, BattleInformationGetResult(&fbatGetPointer()->info));
    return EVT_RETURN_DONE;
}

// s32 result
USER_FUNC(evt_npc_check_escape_battle) {
    EvtWord* args = event->args;

    if (evtGetValue(event, args[0]) == 4) {
        marioSetMutekiTime(3000);
    }
    return EVT_RETURN_DONE;
}

// s32 storeIndex
USER_FUNC(evt_npc_get_battle_rule_keep_result) {
    EvtWord* args = event->args;
    s32 index = args[0];

    FieldBattleData* fbat = fbatGetPointer();

    evtSetValue(event, index, fbat->info.ruleKeepResult);
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 workIndex, s32 storeIndex
USER_FUNC(evt_npc_get_btlsetup_work) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 workIndex, storeIndex;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    workIndex = evtGetValue(event, args[1]);
    storeIndex = args[2];
    evtSetValue(event, storeIndex, npcGetBtlSetupWork(npc, workIndex));
    return EVT_RETURN_DONE;
}

// const char* lookupName, s32 workIndex, s32 value
USER_FUNC(evt_npc_set_btlsetup_work) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;
    s32 workIndex, value;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    workIndex = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    npcSetBtlSetupWork(npc, workIndex, value);
    return EVT_RETURN_DONE;
}

// const char* lookupName, const char* otherName
USER_FUNC(evt_npc_set_link) {
    EvtWord* args = event->args;
    const char* name;
    const char* otherName;
    NpcEntry* npc;
    NpcEntry* other;

    name = (const char*)evtGetValue(event, args[0]);
    otherName = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    other = evtNpcNameToPtr(event, otherName);
    npcSetLink(npc, other);
    return EVT_RETURN_DONE;
}

// const char* lookupName, ItemType storeIndex (out)
USER_FUNC(evt_npc_get_drop_fixitem) {
    EvtWord* args = event->args;
    const char* name;
    NpcEntry* npc;

    name = (const char*)evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[1], npc->battleInfo.fixedItem);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_get_drop_item) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = *args++;

    if (npc->battleInfo.battleFlags & NPC_BATTLE_NO_ITEM_DROP) {
        evtSetValue(event, index, 0);
    } else {
        evtSetValue(event, index, npc->battleInfo.itemDropped);
    }
    return 2;
}

USER_FUNC(evt_npc_get_drop_heart) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = *args++;

    if (npc->battleInfo.battleFlags & NPC_BATTLE_NO_HEART_DROP) {
        evtSetValue(event, index, 0);
    } else {
        s32 count = pouchEquipCheckBadge(ITEM_HEART_FINDER);
        s32 heartsDropped = npc->battleInfo.heartsDropped;

        if (count > 0) {
            if (count == 1) {
                heartsDropped += irand(3) + 1;
            } else {
                heartsDropped += irand(count + 3) + 1;
            }
        }

        evtSetValue(event, index, heartsDropped);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_get_drop_flower) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = *args++;

    if (npc->battleInfo.battleFlags & NPC_BATTLE_NO_FLOWER_DROP) {
        evtSetValue(event, index, 0);
    } else {
        s32 count = pouchEquipCheckBadge(ITEM_FLOWER_FINDER);
        s32 flowersDropped = npc->battleInfo.flowersDropped;

        if (count > 0) {
            if (count == 1) {
                flowersDropped += irand(3) + 1;
            } else {
                flowersDropped += irand(count + 3) + 1;
            }
        }

        evtSetValue(event, index, flowersDropped);
    }
    return EVT_RETURN_DONE;
}

void _npc_check_coin(NpcEntry* npc, s32* value) {
    *value += npc->btlCoinDropCount;
}

void _npc_check_coin_group(NpcEntry* npc, s32* value) {
    NpcEntry* next = npc->next;
    int i;

    if (next) {
        _npc_check_coin_group(next, value);
    }

    for (i = 0; i < 4; i++) {
        if (npc->slaves[i]) {
            _npc_check_coin(npc->slaves[i], value);
        }
    }
    _npc_check_coin(npc, value);
}

USER_FUNC(evt_npc_get_drop_coin) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = *args++;
    s32 value = 0;
    if (npc->battleInfo.battleFlags & NPC_BATTLE_NO_COIN_DROP) {
        evtSetValue(event, index, 0);
        return EVT_RETURN_DONE;
    } else {
        _npc_check_coin_group(npc, &value);
        evtSetValue(event, index, value);
        return EVT_RETURN_DONE;
    }
}

USER_FUNC(evt_npc_getback_item_entry) {
    const char* npcName = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    npcGetBackItemEntry(npc);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_get_unitwork) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = evtGetValue(event, *args++);
    evtSetValue(event, *args++, (EvtWord)npc->unitWork[index]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_set_unitwork) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    s32 index = evtGetValue(event, *args++);
    npc->unitWork[index] = (void*)evtGetValue(event, *args++);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_wait_msec) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);

    s32 temp;
    OSTime lifetime = event->lifetime;
    npc->statusFlags |= NPC_STATUS_UPDATE;

    if (isFirstCall) {
        event->userdata[0] = evtGetValue(event, args[1]);
        event->userdata[1] = ((u64)lifetime >> 32) & 0xFFFFFFFF;
        event->userdata[2] = lifetime & 0xFFFFFFFF;
    }

    temp = event->userdata[0];
    if (!temp) {
        return EVT_RETURN_DONE;
    }

    {
        s32 now = lifetime;
        u64 elapsed = now - event->userdata[2];
        s32 ms = OSTicksToMilliseconds(elapsed);
        return temp <= ms;
    }
}

USER_FUNC(evt_npc_set_balloontype) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    npc->balloonType = evtGetValue(event, *args++);
    return EVT_RETURN_YIELD;
}

USER_FUNC(evt_npc_set_tribe) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    const char* tribeName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    NpcTribeInfo* tribe = npcGetTribe(tribeName);

    npc->tribe = tribe;
    npc->width = tribe->width;
    npc->height = tribe->height;
    if (tribe->stayAnim) {
        strcpy(npc->stayAnim, tribe->stayAnim);
    }
    if (tribe->talkAnim) {
        strcpy(npc->talkAnim, tribe->talkAnim);
    }
    if (tribe->initialAnim) {
        strcpy(npc->currentAnim, tribe->initialAnim);
        animPoseSetAnim(npc->poseId, npc->currentAnim, TRUE);
    }
    npc->footstepSfxA = tribe->footstepSfxA;
    npc->footstepSfxB = tribe->footstepSfxB;
    npc->jumpSfx = tribe->jumpSfx;
    npc->landSfx = tribe->landSfx;
    npc->footstepStride = tribe->footstepStride;
    npc->footstepToggle = TRUE;
    npc->footstepAccum = 0.0f;
    return EVT_RETURN_YIELD;
}

USER_FUNC(evt_npc_set_autotalkpose) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    const char* stayAnim = (const char*)evtGetValue(event, *args++);
    const char* talkAnim = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    if (npc == (NpcEntry*)-3) {
        npcSetMarioAutoTalkPose(stayAnim, talkAnim);
        return EVT_RETURN_YIELD;
    } else {
        if (stayAnim) {
            strcpy(npc->stayAnim, stayAnim);
        }
        if (talkAnim) {
            strcpy(npc->talkAnim, talkAnim);
        }
        return EVT_RETURN_YIELD;
    }
}

USER_FUNC(evt_npc_majo_disp_on) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 prevX = evtGetFloat(event, *args++);
    f32 prevY = evtGetFloat(event, *args++);
    f32 prevZ = evtGetFloat(event, *args++);
    BOOL toggle = evtGetValue(event, *args++);
    s32 group = gp->inBattle != 0;
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);

    if (isFirstCall) {
        event->userdata[0] = -1;
        event->userdata[1] = 0;
    }

    if (!animGroupBaseAsync("p_bibi", group, NULL)) {
        return EVT_RETURN_BLOCK;
    }

    if (toggle) {
        return EVT_RETURN_BLOCK;
    }

    switch (event->userdata[0]) {
        case -1:
            npc->position = npc->prevPosition = (Vec){ prevX, prevY, prevZ };
            animPoseSetEffect(npc->poseId, "p_bibi", 1);
            animPoseSetEffectAnim(npc->poseId, "PM_B_2", TRUE);
            animPosePaperPeraOn(npc->poseId);
            animPoseSetLocalTime(npc->poseId, 0.0f);
            event->userdata[0]++;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_WITCH_SHADOW_SPREAD1), &npc->position);
            break;

        default:
            break;

        case 0:
            npc->shadowScale += (3.0f - npc->shadowScale) / 10.0f;
            animPoseSetLocalTime(npc->poseId, 0.0f);
            event->userdata[1]++;
            if (event->userdata[1] > 30) {
                event->userdata[0]++;
                event->userdata[1] = 0;
                switch (animPoseGetVivianType(npc->poseId)) {
                    case 0:
                        break;
                    case 1:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW2), &npc->position);
                        break;
                    case 2:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_MARI_SHADOW2), &npc->position);
                        break;
                    case 3:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_MAJO_SHADOW2), &npc->position);
                        break;
                }
            }
            break;

        case 1:
            animPoseSetEffect(npc->poseId, "p_bibi", 1);
            animPoseSetEffectAnim(npc->poseId, "PM_B_2", TRUE);
            animPosePaperPeraOn(npc->poseId);
            event->userdata[0]++;
            break;

        case 2:
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(npc->poseId)->paperId) >= 0.66666666f) {
                animPoseSetPaperAnimGroup(npc->poseId, NULL, TRUE);
                event->userdata[0]++;
            }
            break;

        case 3:
            npc->shadowScale += (1.0f - npc->shadowScale) / 10.0f;
            event->userdata[1]++;
            if (event->userdata[1] > 30) {
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_npc_majo_disp_off) {
    const char* npcName = (const char*)evtGetValue(event, *event->args);
    s32 group = gp->inBattle != 0;
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_WITCH_SHADOW_SPREAD1), &npc->position);
    }

    if (!animGroupBaseAsync("p_bibi", group, NULL)) {
        return EVT_RETURN_BLOCK;
    }

    switch (event->userdata[0]) {
        case 0:
            npc->shadowScale += (3.0f - npc->shadowScale) / 10.0f;
            event->userdata[1]++;
            if (event->userdata[1] > 30) {
                event->userdata[0]++;
                event->userdata[1] = 0;
                switch (animPoseGetVivianType(npc->poseId)) {
                    case 0:
                        break;
                    case 1:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_VIVIAN_SHADOW1), &npc->position);
                        break;
                    case 2:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_MARI_SHADOW1), &npc->position);
                        break;
                    case 3:
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_MAJO_SHADOW1), &npc->position);
                        break;
                }
            }
            break;

        default:
            break;

        case 1:
            animPoseSetEffect(npc->poseId, "p_bibi", 1);
            animPoseSetEffectAnim(npc->poseId, "PM_B_1", TRUE);
            animPosePaperPeraOn(npc->poseId);
            animPoseSetLocalTime(animPoseGetAnimPosePtr(npc->poseId)->paperId, 8.0f);
            event->userdata[0]++;
            break;

        case 2:
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(npc->poseId)->paperId) >= 1.0f) {
                event->userdata[0]++;
            }
            break;

        case 3:
            npc->shadowScale += (-npc->shadowScale / 10.0f);
            event->userdata[1]++;
            if (event->userdata[1] > 30) {
                npc->shadowScale = 0.0f;
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_npc_get_kpencount_type) {
    EvtWord* args = event->args;
    const char* npcName = (const char*)evtGetValue(event, *args++);
    s32 index = *args++;
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    evtSetValue(event, index, npc->koopaEncounterType);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_calc_score) {
    const char* npcName = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    npcCalcScore(npc);
    return EVT_RETURN_DONE;
}

void _kamek_houki_kemuri(NpcEntry* npc, s32 animState) {
    static u8 cnt;
    f32 a2, a5;
    u8 mask;
    EffectEntry* effect;

    cnt += 1;
    switch (animState) {
        case 3:
            mask = 0x7;
            break;
        case 2:
            mask = 0xF;
            break;
        default:
            return;
    }

    if (!(cnt & mask)) {
        if ((0.0f < npc->direction) && (npc->direction < 180.0f)) {
            a5 = 1.0f;
            a2 = -((36.0f * npc->scale.x) - npc->position.x);
        } else {
            a5 = -1.0f;
            a2 = (36.0f * npc->scale.x) + npc->position.x;
        }
        effect = effKemuri10N64Entry(0, a2, (18.0f * npc->scale.x) + npc->position.y, npc->position.z, a5, 0.0f, 0.0f,
                                     npc->scale.x);
        if (!strcmp(gp->currentMapName, "kpa_01")) {
            ((EffKemuri10N64Work*)effect->userdata)->camera = 7;
        }
    }
}

USER_FUNC(evt_npc_kamek_move_position) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 temp1;
    f32 temp2;
    f32 delta;
    f32 targetX = evtGetFloat(event, *args++);
    f32 targetZ = evtGetFloat(event, *args++);
    f32 moveOffset = evtGetFloat(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    f32 moveSpeed = evtGetFloat(event, *args++);
    s32 moveFlags = *args++;

    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    Vec diff;
    f32 dist;
    f32 dist1, dist2;
    f32 step;
    s32 sfxFlag;

    npc->statusFlags |= NPC_STATUS_UPDATE;
    if (moveFlags & 0x2) {
        npc->statusFlags |= NPC_STATUS_NO_FIND_ONCE;
    }

    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->moveAnimState = 0;

        npc->jumpTargetPos.x = targetX;
        npc->jumpTargetPos.y = npc->position.y;
        npc->jumpTargetPos.z = targetZ;
        npc->jumpStartPos = npc->position;

        VECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf((diff.x * diff.x) + (diff.z * diff.z));

        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);
        npc->moveSpeed = moveSpeed;
        npc->moveTime = moveTime;
        npc->moveOffset.y = 0.0f;

        // No speed AND no time -> default to 1.0
        if (npc->moveSpeed <= 0.1f && npc->moveTime == 0.0f) {
            npc->moveSpeed = 1.0f;
        }
        // Time given but no speed -> derive speed, then run as speed-mode
        if (npc->moveSpeed <= 0.1f) {
            npc->moveSpeed = dist / npc->moveTime;
            npc->moveTime = 0.0f;
        }
    }

    // flag 0x4: bail if blocked
    if ((moveFlags & 0x4) && npc->wallHit) {
        return EVT_RETURN_DONE;
    }

    VECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
    dist1 = sqrtf((diff.x * diff.x) + (diff.z * diff.z));
    VECSubtract(&npc->jumpTargetPos, &npc->position, &diff);
    dist2 = sqrtf((diff.x * diff.x) + (diff.z * diff.z));
    step = (20.0f * npc->moveSpeed) / 1000.0f;

    if (npc->moveTime) {
        // timed move: moveElapsed is the progress, advanced externally
        if (npc->moveTime <= npc->moveElapsed) {
            npc->prevPosition = npc->jumpTargetPos;
            npc->position = npc->prevPosition;
            npc->moveSpeed = 0.0f;
            return EVT_RETURN_DONE;
        }
    } else {
        // speed move: snap when within a step (or essentially on top of target)
        if (!npc->moveTime && dist2 <= step) {
            npc->prevPosition = npc->jumpTargetPos;
            npc->position = npc->prevPosition;
            npc->moveSpeed = 0.0f;
            return EVT_RETURN_DONE;
        }
    }

    // Re-aim at the target each frame unless direction is locked
    if ((moveFlags & 0x1) && !(npc->statusFlags & NPC_STATUS_AIRBORNE)) {
        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);
    }

    // Turn the body toward travel (unless mid battle-transition states 5/6)
    if ((npc->statusFlags & NPC_STATUS_FACE_MOVE_DIR) && npc->moveSpeed > 0.1f) {
        if (npc != fbatGetPointer()->target || (fbatGetPointer()->mode != 5 && fbatGetPointer()->mode != 6)) {
            npcTuningRy(npc, npc->moveDir);
        }
    }

    {
        f32 temp;
        temp = (f32)sin(((6.2831855f * npc->moveDir) / 360.0f));
        npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * temp);
        temp = (f32)cos(((6.2831855f * npc->moveDir) / 360.0f));
        npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -temp);
    }
    {
        delta = dist1 - dist2;

        temp2 = (f32)sin(3.1415927f * (2.0f * ((delta) / dist1)));
        temp1 = (f32)sin(3.1415927f * (2.0f * (((npc->moveSpeed * npc->moveDt) + delta) / dist1)));
        npc->moveOffset.y += (moveOffset * temp1) - (moveOffset * temp2);
    }

    if ((moveFlags & 8) == 0) {
        _kamek_houki_kemuri(npc, npc->moveAnimState);
    }

    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_npc_kamek_kemuri1) {
    const char* npcName = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = npcNameToPtr(npcName);

    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }

    if (npc->moveTime) {
        if (npc->moveTime > npc->moveElapsed) {
            _kamek_houki_kemuri(npc, npc->moveAnimState);
            return EVT_RETURN_BLOCK;
        }
    } else if (npc->moveSpeed > 0.0f) {
        _kamek_houki_kemuri(npc, npc->moveAnimState);
        return EVT_RETURN_BLOCK;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_kamek_kemuri2) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    f32 moveTime = (f32)evtGetValue(event, *args++) / 1000.0f;
    s32 unk = evtGetValue(event, *args++);
    NpcEntry* npc = npcNameToPtr(npcName);

    if (isFirstCall) {
        if (moveTime <= 0.0f) {
            return EVT_RETURN_DONE;
        }

        npc->moveTicks = 0;
        npc->moveTime = moveTime;
        return EVT_RETURN_BLOCK;
    }

    if (npc->moveTime <= npc->moveElapsed) {
        return EVT_RETURN_DONE;
    }

    _kamek_houki_kemuri(npc, (unk != 0) + 2);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_fbat_trans_floor_position) {
    EvtWord* args = event->args;

    f32 hitX, hitY, hitZ;
    f32 distance;
    f32 hitNX, hitNY, hitNZ;

    s32 xArg = *args++;
    f32 x = evtGetFloat(event, xArg);
    s32 yArg = *args++;
    f32 y = evtGetFloat(event, yArg);
    s32 zArg = *args++;
    f32 z = evtGetFloat(event, zArg);
    f32 floor = evtGetFloat(event, *args++);

    distance = evtGetFloat(event, *args++);

    if (hitCheckFilter(0, &hitX, &hitY, &hitZ, &distance, &hitNX, &hitNY, &hitNZ, x, 1.0f + y, z, 0.0f, -1.0f, 0.0f)) {
        y = (hitY + floor);
    }

    evtSetFloat(event, xArg, x);
    evtSetFloat(event, yArg, y);
    evtSetFloat(event, zArg, z);
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
// US-only: like evt_fbat_trans_floor_position, but also probes at x +/- width/2 and keeps the highest floor
USER_FUNC(evt_fbat_trans_floor_position_wide) {
    EvtWord* args = event->args;

    f32 hitX, hitY, hitZ;
    f32 distance;
    f32 hitNX, hitNY, hitNZ;
    f32 half, h;

    s32 xArg = *args++;
    f32 x = evtGetFloat(event, xArg);
    s32 yArg = *args++;
    f32 y = evtGetFloat(event, yArg);
    s32 zArg = *args++;
    f32 z = evtGetFloat(event, zArg);
    f32 floor = evtGetFloat(event, *args++);
    f32 dist = evtGetFloat(event, *args++);
    f32 width = evtGetFloat(event, *args++);

    f32 y2 = y;
    distance = dist;

    y = 1.0f + y;
    if (hitCheckFilter(0, &hitX, &hitY, &hitZ, &distance, &hitNX, &hitNY, &hitNZ, x, y, z, 0.0f, -1.0f, 0.0f)) {
        y2 = (hitY + floor);
    }
    half = width * 0.5f;
    if (hitCheckFilter(0, &hitX, &hitY, &hitZ, &distance, &hitNX, &hitNY, &hitNZ, x + half, y, z, 0.0f, -1.0f, 0.0f)) {
        h = hitY + floor;
        if (h > y2) {
            y2 = h;
        }
    }
    if (hitCheckFilter(0, &hitX, &hitY, &hitZ, &distance, &hitNX, &hitNY, &hitNZ, x - half, y, z, 0.0f, -1.0f, 0.0f)) {
        h = hitY + floor;
        if (h > y2) {
            y2 = h;
        }
    }

    evtSetFloat(event, xArg, x);
    evtSetFloat(event, yArg, y2);
    evtSetFloat(event, zArg, z);
    return EVT_RETURN_DONE;
}
#endif

USER_FUNC(evt_npc_sound_data_reset) {
    const char* npcName = (const char*)evtGetValue(event, *event->args);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);
    NpcTribeInfo* tribe = npc->tribe;
    if (tribe) {
        npc->footstepSfxA = tribe->footstepSfxA;
        npc->footstepSfxB = tribe->footstepSfxB;
        npc->jumpSfx = tribe->jumpSfx;
        npc->landSfx = tribe->landSfx;
        npc->footstepStride = tribe->footstepStride;
        npc->footstepToggle = 1;
        npc->footstepAccum = 0.0f;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_sound_data_set) {
    EvtWord* args = event->args;

    const char* npcName = (const char*)evtGetValue(event, *args++);
    NpcEntry* npc = evtNpcNameToPtr(event, npcName);

    s32 footstepSfxAArg = *args++;
    const char* footstepSfxA = (const char*)evtGetValue(event, footstepSfxAArg);
    s32 footstepSfxBArg = *args++;
    const char* footstepSfxB = (const char*)evtGetValue(event, footstepSfxBArg);
    s32 footstepStrideArg = *args++;
    u16 footstepStride = evtGetValue(event, footstepStrideArg);
    s32 jumpSfxArg = *args++;
    const char* jumpSfx = (const char*)evtGetValue(event, jumpSfxArg);
    s32 landSfxArg = *args++;
    const char* landSfx = (const char*)evtGetValue(event, landSfxArg);

    if (footstepSfxAArg != EVT_NULLPTR) {
        npc->footstepSfxA = footstepSfxA;
    }
    if (footstepSfxBArg != EVT_NULLPTR) {
        npc->footstepSfxB = footstepSfxB;
    }
    if (jumpSfxArg != EVT_NULLPTR) {
        npc->jumpSfx = jumpSfx;
    }
    if (landSfxArg != EVT_NULLPTR) {
        npc->landSfx = landSfx;
    }
    if (footstepStrideArg != EVT_NULLPTR) {
        npc->footstepStride = footstepStride;
    }
    npc->footstepToggle = TRUE;
    npc->footstepAccum = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_npc_release_filednpc) {
    BOOL set = evtGetValue(event, *event->args);
    NpcWork* wp = npcGetWorkPtr();
    if (set) {
        wp->flags |= 2;
    } else {
        wp->flags &= ~2;
    }
    return EVT_RETURN_DONE;
}

