#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "party.h"
#include "system.h"
#include "party/party_move.h"
#include "party/party_nokonoko.h"
#include "party/party_yoshi.h"
#include "party/party_vivian.h"
#include "mario/mario_sbr.h"
#include "mario/mario_hit.h"
#include "party/party_hit.h"

#define GETPARTY(args) partyGetPtr((args)[0] == 0 ? partyCtrlNo : 1)
#define PARTY_ID(args) ((args)[0] == 0 ? partyCtrlNo : 1)

extern s32 bomheiGetStatus(void);
extern void L_partyVivianTailStart();
extern void L_partyVivianTailStop();
extern s32 cloudGetStatus(void);

USER_FUNC(evt_party_flg_onoff) {
    PartyEntry* p;
    int mask;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    mask = args[1];
    if (evtGetValue(event, args[2])) {
        p->flags |= mask;
    } else {
        p->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_dispflg_onoff) {
    PartyEntry* p;
    int mask;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    mask = args[1];
    if (evtGetValue(event, args[2])) {
        p->flags2 |= mask;
    } else {
        p->flags2 &= ~mask;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_trigflg_onoff) {
    PartyEntry* p;
    int mask;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    mask = args[1];
    if (evtGetValue(event, args[2])) {
        p->trigFlags |= mask;
    } else {
        p->trigFlags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_cont_onoff) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (evtGetValue(event, args[1]) == 0) {
        partyCtrlOff();
    } else {
        partyCtrlOn();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_init_camid) {
    PartyEntry* p = GETPARTY(event->args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    partyInitCamId(p);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_camid) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    partySetCamId(p, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_force_reset_outofscreen) {
    void* mw;
    PartyEntry* p = GETPARTY(event->args);
    Vec from;
    Vec dir;
    Vec out1;
    Vec out2;
    Vec pos;
    Vec pos2;
    f32 sx, sy, sz;
    f32 h;
    f32 ang;
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    pos = p->position;
    marioGetScreenPos(pos, &sx, &sy, &sz);
    if (marioChkInScreen((s32)sx, (s32)sy)) {
        return EVT_RETURN_DONE;
    }
    partyClearFootmark();
    mw = p->playerPtr;
    dir = (Vec){0.0f, -1.0f, 0.0f};
    while (1) {
        pos2 = p->position;
        marioGetScreenPos(pos2, &sx, &sy, &sz);
        if (marioChkInScreen((s32)sx, (s32)sy)) {
            break;
        }
        ang = angleABf(p->position.x, p->position.z, ((MarioWork*)mw)->position.x, ((MarioWork*)mw)->position.z);
        movePos(&p->position.x, &p->position.z, 1.0f, ang);
        h = 100.0f;
        from = p->position;
        from.y += 50.0f;
        if (partyHitCheck(p, &from, &dir, &out1, &out2, &h) != NULL) {
            p->position.y = out1.y;
        } else {
            p->position.y = ((MarioWork*)mw)->position.y;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_outofscreen) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    evtSetFloat(event, args[1], (f32)(s32)(((p->flags2 >> 23) & 1) ^ 1));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_pos) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    p->position.x = evtGetFloat(event, args[1]);
    p->position.y = evtGetFloat(event, args[2]);
    p->position.z = evtGetFloat(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_hosei_xyz) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    p->floorMoveX = evtGetFloat(event, args[1]);
    p->floorMoveY = evtGetFloat(event, args[2]);
    p->floorMoveZ = evtGetFloat(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_get_pos) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        evtSetFloat(event, args[1], -5000.0f);
        evtSetFloat(event, args[2], -5000.0f);
        evtSetFloat(event, args[3], -5000.0f);
        return EVT_RETURN_DONE;
    }
    evtSetFloat(event, args[1], p->position.x);
    evtSetFloat(event, args[2], p->position.y);
    evtSetFloat(event, args[3], p->position.z);
    return EVT_RETURN_DONE;
}

inline float inline_fn(f32 a, f32 b, f32 c, s32 n) {
    return (a - b) * (c / (f32)n);
}

USER_FUNC(evt_party_set_dir) {
    EvtWord* args = event->args;
    PartyEntry* p;
    f32 ang, t, st;
    s32 fr, v, c;
    if (isFirstCall) {
        s32 id = (*args++ == 0) ? partyCtrlNo : 1;
        event->userdata[0] = id;
        p = partyGetPtr(event->userdata[0]);
        if (p == NULL) {
            return EVT_RETURN_DONE;
        }
        event->userdata[2] = 0;
        event->userdataf[1] = p->displayDirection;
    } else {
        args++;
    }
    ang = evtGetFloat(event, args[0]);
    v = evtGetValue(event, args[1]);
    if (v >= 0) {
        fr = sysMsec2Frame(v);
    } else {
        fr = v;
    }
    p = partyGetPtr(event->userdata[0]);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (fr > 0) {
        t = revise360(270.0f - ang);
        st = event->userdataf[1];
        p->displayDirection = revise360(st + inline_fn(t, st, (f32)event->userdata[2], fr));
        p->displayDirectionTarget = p->displayDirection;
        p->targetDir = toMovedirSimple(p->displayDirectionTarget);
        c = event->userdata[2] + 1;
        event->userdata[2] = c;
        return c > fr;
    } else {
        if (fr < 0) {
            p->displayDirectionTarget = revise360(270.0f - ang);
            p->targetDir = toMovedirSimple(p->displayDirectionTarget);
        } else if (fr == 0) {
            p->displayDirection = revise360(270.0f - ang);
            p->displayDirectionTarget = p->displayDirection;
            p->targetDir = toMovedirSimple(p->displayDirectionTarget);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_dir_npc) {
    EvtWord* args = event->args;
    PartyEntry* p = partyGetPtr(args[0]);
    NpcEntry* npc;
    const char* nm;
    f32 x, z, camYaw, a;
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        nm = (const char*)evtGetValue(event, args[1]);
        npc = evtNpcNameToPtr(event, nm);
        if (npc == NULL) {
            return EVT_RETURN_DONE;
        }
        x = npc->position.x;
        z = npc->position.z;
        camYaw = camGetPtr(CAMERA_3D)->viewYaw;
        a = revise360(angleABf(p->position.x, p->position.z, x, z) - camYaw);
        p->displayDirectionTarget = (a < 180.0f) ? 180.0f : 0.0f;
        p->targetDir = a;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_dir_pos) {
    EvtWord* args = event->args;
    PartyEntry* p = partyGetPtr(args[0]);
    f32 x, z, camYaw, a;
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    x = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    camYaw = camGetPtr(CAMERA_3D)->viewYaw;
    a = revise360(angleABf(p->position.x, p->position.z, x, z) - camYaw);
    p->displayDirectionTarget = (a < 180.0f) ? 180.0f : 0.0f;
    p->targetDir = a;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_get_dispdir) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        evtSetFloat(event, args[1], 0.0f);
    } else {
        evtSetFloat(event, args[1], p->displayDirection);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_dispdir) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    p->displayDirectionTarget = reviseAngle(evtGetFloat(event, args[1]));
    p->displayDirection = p->displayDirectionTarget;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_stop) {
    PartyEntry* p = GETPARTY(event->args);
    if (p != NULL) {
        partyStop(p);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_run) {
    PartyEntry* p = GETPARTY(event->args);
    if (p != NULL) {
        partyRun(p);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_move_pos) {
    PartyEntry* p;
    EvtWord* args = event->args;
    f32 x, z, spd;
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    event->userdata[0] = PARTY_ID(args);
    p = partyGetPtr(event->userdata[0]);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (event->userdata[2] == 0) {
        u8 t = p->runMode;
        if (t != 0 && (u8)(t - 1) > 1) {
            return EVT_RETURN_BLOCK;
        }
        event->userdata[2] = 1;
        x = evtGetFloat(event, args[1]);
        z = evtGetFloat(event, args[2]);
        event->userdata[1] = sysMsec2Frame(evtGetValue(event, args[3]));
        p->targetDir = angleABf(p->position.x, p->position.z, x, z);
        if (event->userdata[1] == 0) {
            event->userdata[1] = (s32)(distABf(p->position.x, p->position.z, x, z) / p->moveSpeed);
            spd = p->moveSpeed;
        } else {
            spd = distABf(p->position.x, p->position.z, x, z) / event->userdata[1];
        }
        partySetForceMove(p, event->userdata[1], p->targetDir, spd);
    }
    if (--event->userdata[1] < 0) {
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_party_move_pos2) {
    PartyEntry* p;
    EvtWord* args = event->args;
    f32 x, z, spd, dist;
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    event->userdata[0] = PARTY_ID(args);
    p = partyGetPtr(event->userdata[0]);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (event->userdata[2] == 0) {
        u8 t = p->runMode;
        if (t != 0 && (u8)(t - 1) > 1) {
            return EVT_RETURN_BLOCK;
        }
        event->userdata[2] = 1;
        x = evtGetFloat(event, args[1]);
        z = evtGetFloat(event, args[2]);
        spd = evtGetFloat(event, args[3]);
        dist = distABf(p->position.x, p->position.z, x, z);
        p->targetDir = angleABf(p->position.x, p->position.z, x, z);
        dist = gp->framerate * (dist / spd);
        spd = spd / gp->framerate;
        partySetForceMove(p, dist, p->targetDir, spd);
    }
    if (p->forceMoveFrames == 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_party_wait_landon) {
    PartyEntry* p = GETPARTY(event->args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    return ((p->flags >> 1) & 1) ? 2 : 0;
}

USER_FUNC(evt_party_jump_pos) {
    PartyEntry* p;
    EvtWord* args = event->args;
    f32 y, ff, half, dy, x, z, hgt, g;
    s32 ms, mode;
    f32 tmp[2];
    if (isFirstCall) {
        PartyEntry* q;
        s32 id = (*args++ == 0) ? partyCtrlNo : 1;
        event->userdata[0] = id;
        q = partyGetPtr(event->userdata[0]);
        if (q == NULL) {
            return EVT_RETURN_DONE;
        }
        if (q->runMode == 0xB) {
            return EVT_RETURN_DONE;
        }
        event->userdata3 = 0;
        partyChgMot(q, 8);
        partyChgRunMode(q, 9);
    } else {
        args++;
    }
    p = partyGetPtr(event->userdata[0]);
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    ms = evtGetValue(event, args[3]);
    mode = evtGetValue(event, args[4]);
    hgt = evtGetFloat(event, args[5]);
    if (hgt <= -1000.0f) {
        hgt = 20.0f;
    }
    ff = sysMsec2Frame(ms);
    half = 0.5f;
    dy = 1.0f;
    switch (event->userdata3) {
    case 0:
        p->hitobjJumpFrom = p->hitobjStandOn;
        p->hitobjStandOn = NULL;
        p->moveSpeed = distABf(p->position.x, p->position.z, x, z) / ff;
        p->targetDir = angleABf(p->position.x, p->position.z, x, z);
        if (mode == 0) {
            p->moveDir = p->targetDir;
        }
        partyChgPoseId(p, 4);
        dy = y - p->position.y;
        half = 0.5f * ff;
        if (__fabsf(dy) < 1.0f) {
            dy = hgt;
        } else if (dy >= 0.0f) {
            dy += 37.0f;
        } else if (searchUnder2(tmp, x, y, z) != NULL) {
            dy = hgt;
        }
        g = (dy + dy) / (half * half - half);
        p->velY = g * half;
        p->work.common.accelY = -g;
        event->userdata[2] = (s32)(0.5f + half);
        event->userdata3 = 1;
    case 1:
        movePos(&p->position.x, &p->position.z, p->moveSpeed, p->targetDir);
        p->position.y += p->velY;
        p->velY += p->work.common.accelY;
        if (--event->userdata[2] <= 0) {
            half = 0.5f * ff;
            dy = y - p->position.y;
            p->velY = 0.0f;
            p->work.common.accelY = (dy + dy) / (half * half - half);
            p->gravity = 0.0f;
            event->userdata[2] = (s32)(0.5f + half);
            event->userdata3 = 2;
        }
        break;
    case 2:
        movePos(&p->position.x, &p->position.z, p->moveSpeed, p->targetDir);
        p->velY += p->work.common.accelY;
        mode = 0;
        ff = p->velY;
        {
            HitEntry* hit = partySearchGround(p, __fabsf(ff), __fabsf(ff));
            if (hit != NULL && p->position.y + ff <= p->groundY) {
                p->hitobjStandOn = hit;
                mode = 1;
                p->position.y = p->groundY;
                p->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            }
        }
        if (mode) {
            event->userdata[2] = 0;
        } else {
            p->position.y += p->velY;
        }
        if (--event->userdata[2] <= 0) {
            partyChgPoseId(p, 1);
            p->gravity = 0.0f;
            p->work.common.accelY = 0.0f;
            p->moveSpeed = 0.0f;
            partyRestoreRunMode(p);
            return EVT_RETURN_YIELD;
        }
        break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_party_move_behind_mario) {
    PartyEntry* p;
    EvtWord* args = event->args;
    s32 v;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    v = evtGetValue(event, args[1]);
    partyMoveBehindMario(p, v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_move_beside_mario) {
    PartyEntry* p;
    EvtWord* args = event->args;
    s32 v;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    v = evtGetValue(event, args[1]);
    partyMoveBesideMario(p, v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_homing_dist) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    p->homingDist = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_pose) {
    PartyEntry* p;
    EvtWord* args = event->args;
    s32 v;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    v = evtGetValue(event, args[1]);
    partyChgPose(p, (const char*)v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_sleep_on) {
    PartyEntry* p = GETPARTY(event->args);
    if (p != NULL) {
        partyChgPoseId(p, 6);
        p->flags |= PARTY_FLAG_SLEEP_ON;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_sleep_off) {
    PartyEntry* p = GETPARTY(event->args);
    if (p != NULL) {
        partyChgPoseId(p, 1);
        p->flags &= ~PARTY_FLAG_SLEEP_ON;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_set_breed_pose) {
    PartyEntry* p;
    EvtWord* args = event->args;
    s32 v;
    p = GETPARTY(args);
    if (p != NULL) {
        v = evtGetValue(event, args[1]);
    partyChgPoseId(p, v);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(L_evt_party_dokan) {
    PartyEntry* p;
    s32 dst, v;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    dst = args[1];
    if (p->runMode == 0xB || p->runMode == 0xC) {
        v = 0;
    } else {
        v = 1;
    }
    evtSetValue(event, dst, v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_get_status) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 dst = args[1];
    s32 st;
    switch (id) {
    case 6:
        st = vivianGetStatus();
        evtSetValue(event, dst, st);
        break;
    case 3:
        evtSetValue(event, dst, bomheiGetStatus());
        break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_get_name_hitobj_head) {
    PartyEntry* p;
    EvtWord* args;
    const char* name = NULL;
    s32 dst;
    args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    dst = args[1];
    if (p->hitobjHead != NULL) {
        name = hitGetName(p->hitobjHead);
    }
    evtSetValue(event, dst, (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_get_name_hitobj_ride) {
    PartyEntry* p;
    EvtWord* args;
    const char* name = NULL;
    s32 dst;
    args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    dst = args[1];
    if (p->hitobjStandOn != NULL) {
        name = hitGetName(p->hitobjStandOn);
    }
    evtSetValue(event, dst, (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_thunders_use) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (bomheiGetStatus() != 0) {
            return EVT_RETURN_DONE;
        }
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        p->flags |= PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(p, 3);
    }
    if (bomheiGetStatus() != 0) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_vivian_hold) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (vivianGetStatus() != 0) {
            return EVT_RETURN_DONE;
        }
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        p->flags |= PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(p, 3);
    }
    if (vivianGetStatus() != 2) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_vivian_unhold) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (vivianGetStatus() == 2) {
            vivianUnhold(p);
        }
    }
    return vivianGetStatus() == 0 ? 2 : 0;
}

USER_FUNC(L_evt_party_vivian_tail) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (args[1] == 0) {
        L_partyVivianTailStop();
    } else {
        L_partyVivianTailStart();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_yoshi_ride) {
    EvtWord* args = event->args;
    PartyEntry* p;
    s32 ride;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    ride = args[1];
    if (isFirstCall) {
        if (ride) {
            if (yoshiGetStatus() != 0) {
                return EVT_RETURN_DONE;
            }
            p->flags |= PARTY_FLAG_IS_EVT_USE;
            p->flags |= PARTY_FLAG_IS_BEING_USED;
            partyChgRunMode(p, 3);
        } else {
            marioGetoffYoshi();
        }
    }
    if (ride) {
        if (yoshiGetStatus() == 1) {
            p->flags &= 0x7FFFFFFF;
            return EVT_RETURN_DONE;
        }
    } else {
        if (yoshiGetStatus() == 0) {
            p->flags &= 0x7FFFFFFF;
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_party_yoshi_fly) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (yoshiGetStatus() != 1) {
            return EVT_RETURN_DONE;
        }
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        evt_set_yoshi_spd(evtGetFloat(event, args[1]));
        evt_set_yoshi_frm(sysMsec2Frame(evtGetValue(event, args[2])));
    }
    if (evt_get_yoshi_frm() >= 0) {
        return EVT_RETURN_BLOCK;
    }
    p->flags &= 0x7FFFFFFF;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_nokotaro_kick) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (p->currentMemberId != PARTY_MEMBER_KOOPS) {
            return EVT_RETURN_DONE;
        }
        if (p->flags & PARTY_FLAG_IS_BEING_USED) {
            return EVT_RETURN_DONE;
        }
        p->actionTime = 0;
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        p->flags |= PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(p, 3);
    }
    if (nokonokoGetStatus(p) == 0) {
        p->flags &= 0x7FFFFFFF;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_party_nokotaro_kick_hold) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (p->currentMemberId != PARTY_MEMBER_KOOPS) {
            return EVT_RETURN_DONE;
        }
        if (p->flags & PARTY_FLAG_IS_BEING_USED) {
            return EVT_RETURN_DONE;
        }
        p->actionTime = 5000;
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        p->flags |= PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(p, 3);
    }
    if (nokonokoGetStatus(p) == 0) {
        p->flags &= 0x7FFFFFFF;
        return EVT_RETURN_DONE;
    }
    return nokonokoGetStatus(p) == 3 ? 2 : 0;
}

USER_FUNC(evt_party_nokotaro_hold_cancel) {
    PartyEntry* p;
    s32 ret = 0;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (p->currentMemberId != PARTY_MEMBER_KOOPS) {
            return EVT_RETURN_DONE;
        }
        if (nokonokoGetStatus(p) == 0) {
            return EVT_RETURN_DONE;
        }
        event->userdata[0] = 0;
    }
    switch (event->userdata[0]) {
    case 0:
        if (nokonokoGetStatus(p) != 3) {
            ret = 0;
            break;
        }
        nokotaro_hold_cancel(p);
        event->userdata[0] = 1;
        break;
    case 1:
        if (nokonokoGetStatus(p) != 0) {
            ret = 0;
            break;
        }
        ret = 2;
        p->flags &= 0x7FFFFFFF;
        break;
    }
    return ret;
}

USER_FUNC(evt_party_nokotaro_get_status) {
    s32 dst;
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (p->currentMemberId != PARTY_MEMBER_KOOPS) {
        return EVT_RETURN_DONE;
    }
    dst = args[1];
    evtSetValue(event, dst, nokonokoGetStatus(p));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_party_cloud_breathout) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (p->currentMemberId != PARTY_MEMBER_FLURRIE) {
            return EVT_RETURN_DONE;
        }
        if (p->flags & PARTY_FLAG_IS_BEING_USED) {
            return EVT_RETURN_DONE;
        }
        p->actionTime = args[1];
        p->flags |= PARTY_FLAG_IS_EVT_USE;
        p->flags |= PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(p, 3);
    }
    if (cloudGetStatus() == 0) {
        p->flags &= 0x7FFFFFFF;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

#ifdef VERSION_G8ME01
extern void N_cloudLockAnimationsOn(PartyEntry* party);
extern void N_cloudLockAnimationsOff(PartyEntry* party);

// US only.
USER_FUNC(N_evt_party_cloud_lock_animations_on_off) {
    PartyEntry* p;
    EvtWord* args = event->args;
    p = GETPARTY(args);
    if (p == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        if (p->currentMemberId != PARTY_MEMBER_FLURRIE) {
            return EVT_RETURN_DONE;
        }
        if (p->flags & PARTY_FLAG_IS_BEING_USED) {
            return EVT_RETURN_DONE;
        }
        if (args[1]) {
            N_cloudLockAnimationsOn(p);
        } else {
            N_cloudLockAnimationsOff(p);
        }
    }
    return EVT_RETURN_DONE;
}
#endif

void evtPartyStrippedPool(f32* f) {
    f[0] = 1.0f;
    f[1] = 0.5f;
    f[2] = 20.0f;
    f[3] = 0.0f;
    f[4] = 37.0f;
    f[5] = -1000.0f;
}
