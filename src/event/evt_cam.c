#include "evt/evt_cam.h"
#include "driver/camdrv.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/os.h>
#include "libc_decls.h"
#include "cam_road.h"
#include <math.h>
#include "cam_shift.h"

void OSReport(const char* msg, ...);

// CameraId id, BOOL on
USER_FUNC(evt_cam_ctrl_onoff) {
    EvtWord* args = event->args;
    CameraId id = evtGetValue(event, args[0]);

    if (evtGetValue(event, args[1])) {
        camCtrlOn(id);
    }
    else {
        camCtrlOff(id);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam_get_pos) {
    EvtWord* args = event->args;
    CameraEntry* camera = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], camera->cameraPos.x);
    evtSetFloat(event, args[2], camera->cameraPos.y);
    evtSetFloat(event, args[3], camera->cameraPos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam_get_at) {
    EvtWord* args = event->args;
    CameraEntry* camera = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], camera->cameraAt.x);
    evtSetFloat(event, args[2], camera->cameraAt.y);
    evtSetFloat(event, args[3], camera->cameraAt.z);
    return EVT_RETURN_DONE;
}

// CameraId id, f32 x, f32 y, s32 msec
USER_FUNC(evt_cam_shake) {
    EvtWord* args = event->args;
    int shift;
    int camId;
    CameraId id;
    u64 now;
    s32 msec;
    f32 x, y;
    f32 len;
    s32 onTime, offTime;
    u64 last;

    id = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    camId = id;
    y = evtGetFloat(event, args[2]);
    msec = evtGetValue(event, args[3]);
    now = *(u64*)&event->lifetime;
    if (isFirstCall) {
        event->userdata[0] = 0;
        ((u32*)&event->userdata[1])[0] = (now >> 32) & 0xFFFFFFFF;
        ((u32*)&event->userdata[1])[1] = now & 0xFFFFFFFF;
        event->userWork[3] = 0;
        ((u32*)&event->wInterpRelatedTime)[0] = (now >> 32) & 0xFFFFFFFF;
        ((u32*)&event->wInterpRelatedTime)[1] = now & 0xFFFFFFFF;
    }
    event->userdata[0]++;
    event->userdata[0] %= 4;
    if (event->userdata[0] < 2) {
        PSMTX44Trans(camGetPtr(camId)->projAdjust, x, y, 0.0f);
    }
    else {
        PSMTX44Trans(camGetPtr(camId)->projAdjust, -x, -y, 0.0f);
    }
    camGetPtr(camId)->flags |= 8;
    if (msec) {
        if ((u64)msec < OSTicksToMilliseconds(now - *(u64*)&event->userdata[1])) {
            camGetPtr(id)->flags &= ~8;
            padRumbleOff(0);
            return EVT_RETURN_DONE;
        }
    }
    len = sqrtf(x * x + y * y);
    onTime = 13000.0f * len;
    offTime = 50.0f * (1.0f - len);
    last = event->wInterpRelatedTime;
    if (onTime && offTime) {
        // FAKE: dead int guard (COLOURING W): its division temps survive to RA and add interference, then the
        // post-RA peephole folds the branch and deletes the arm (no code). Colours the first u64 compare's dummy r24.
        {
            s32 gz = 0;
            if (gz != 0) {
                offTime = ((offTime / onTime) / (onTime / offTime)) / (((offTime + 1) / onTime) / (onTime / (offTime + 1)));
            }
        }
        if (!event->userWork[3]) {
            padRumbleOn(0);
            if (onTime < OSTicksToMilliseconds(now - last)) {
                shift = 32;
                event->userWork[3] = 1 - event->userWork[3];
                ((u32*)&event->wInterpRelatedTime)[0] = (now >> shift) & 0xFFFFFFFF;
                ((u32*)&event->wInterpRelatedTime)[1] = now & 0xFFFFFFFF;
            }
        }
        else {
            padRumbleOff(0);
            if (offTime < OSTicksToMilliseconds(now - last)) {
                event->userWork[3] = 1 - event->userWork[3];
                ((u32*)&event->wInterpRelatedTime)[0] = (now >> 32) & 0xFFFFFFFF;
                ((u32*)&event->wInterpRelatedTime)[1] = now & 0xFFFFFFFF;
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

// s32 percent, s32 msec, s32 dist, s32 curve
USER_FUNC(evt_cam3d_evt_zoom_in) {
    EvtWord* args = event->args;
    s32 percent, msec, dist, curve;
    CameraEntry* camera;
    Vec pos;

    percent = evtGetValue(event, args[0]);
    msec = evtGetValue(event, args[1]);
    dist = evtGetValue(event, args[2]);
    curve = evtGetValue(event, args[3]);
    camera = camGetPtr(CAMERA_3D);
    PSVECSubtract(&camera->cameraAt, &camera->cameraPos, &pos);
    PSVECScale(&pos, &pos, (f32)percent / 100.0f);
    PSVECAdd(&pos, &camera->cameraPos, &pos);
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = pos;
    camera->storedAt = camera->cameraAt;
    camera->storedPos.x += (f32)dist * (f32)cos(camera->viewYaw);
    camera->storedPos.z += (f32)dist * (f32)sin(camera->viewYaw);
    camera->storedAt.x += (f32)dist * (f32)cos(camera->viewYaw);
    camera->storedAt.z += (f32)dist * (f32)sin(camera->viewYaw);
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// s32 x, s32 y, s32 z, s32 percent, s32 msec, s32 curve
USER_FUNC(evt_cam3d_evt_set_at) {
    EvtWord* args = event->args;
    s32 x, y, z, percent, msec, curve;
    CameraEntry* camera;
    Vec at;
    Vec pos;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    percent = evtGetValue(event, args[3]);
    msec = evtGetValue(event, args[4]);
    curve = evtGetValue(event, args[5]);
    camera = camGetPtr(CAMERA_3D);
    at = (Vec){x, y, z};
    (Vec){0.0f, 0.0f, 0.0f};
    PSVECSubtract(&at, &camera->cameraPos, &pos);
    PSVECScale(&pos, &pos, (f32)percent / 100.0f);
    PSVECAdd(&pos, &camera->cameraPos, &pos);
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = pos;
    camera->storedAt = at;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// s32 posX, posY, posZ, atX, atY, atZ, msec, curve
USER_FUNC(evt_cam3d_evt_set) {
    EvtWord* args = event->args;
    f32 px, py, pz, ax, ay, az;
    s32 msec, curve;
    CameraEntry* camera;
    Vec pos, at;

    px = evtGetValue(event, args[0]);
    py = evtGetValue(event, args[1]);
    pz = evtGetValue(event, args[2]);
    ax = evtGetValue(event, args[3]);
    ay = evtGetValue(event, args[4]);
    az = evtGetValue(event, args[5]);
    msec = evtGetValue(event, args[6]);
    curve = evtGetValue(event, args[7]);
    camera = camGetPtr(CAMERA_3D);
    pos = (Vec){px, py, pz};
    at = (Vec){ax, ay, az};
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = pos;
    camera->storedAt = at;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// s32 posX, posY, posZ (relative to at), atX, atY, atZ (relative to mario), msec, curve
USER_FUNC(evt_cam3d_evt_set_rel) {
    EvtWord* args = event->args;
    f32 px, py, pz, ax, ay, az;
    s32 msec, curve;
    CameraEntry* camera;
    MarioWork* mario;

    px = evtGetValue(event, args[0]);
    py = evtGetValue(event, args[1]);
    pz = evtGetValue(event, args[2]);
    ax = evtGetValue(event, args[3]);
    ay = evtGetValue(event, args[4]);
    az = evtGetValue(event, args[5]);
    msec = evtGetValue(event, args[6]);
    curve = evtGetValue(event, args[7]);
    camera = camGetPtr(CAMERA_3D);
    mario = marioGetPtr();
    {
        Vec pos, at;

        (Vec){0.0f, 0.0f, 0.0f};
        (Vec){0.0f, 0.0f, 0.0f};
        ax = mario->position.x + ax;
        ay = mario->position.y + ay;
        az = mario->position.z + az;
        pos.x = px + ax;
        pos.y = py + ay;
        pos.z = pz + az;
        at.x = ax;
        at.y = ay;
        at.z = az;

        camera->presetPos = camera->cameraPos;
        camera->presetAt = camera->cameraAt;
        camera->storedPos = pos;
        camera->storedAt = at;
    }
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// const char* npc, s32 posX, posY, posZ, atX, atY, atZ, msec, curve
USER_FUNC(evt_cam3d_evt_set_npc_rel) {
    EvtWord* args = event->args;
    const char* name;
    f32 px, py, pz, ax, ay, az;
    s32 msec, curve;
    CameraEntry* camera;
    NpcEntry* npc;
    Vec npcPos, npcAt;
    Vec pos, at;

    name = (const char*)evtGetValue(event, args[0]);
    px = evtGetValue(event, args[1]);
    py = evtGetValue(event, args[2]);
    pz = evtGetValue(event, args[3]);
    ax = evtGetValue(event, args[4]);
    ay = evtGetValue(event, args[5]);
    az = evtGetValue(event, args[6]);
    msec = evtGetValue(event, args[7]);
    curve = evtGetValue(event, args[8]);
    camera = camGetPtr(CAMERA_3D);
    npc = evtNpcNameToPtr(event, name);
    pos = (Vec){px, py, pz};
    at = (Vec){ax, ay, az};
    PSVECAdd(&npc->position, &at, &npcAt);
    PSVECAdd(&npcAt, &pos, &npcPos);
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = npcPos;
    camera->storedAt = npcAt;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// s32 posX, posY, posZ, atX, atY, atZ (rotated by camera yaw), msec, curve
USER_FUNC(evt_cam3d_evt_set_rel_dir) {
    EvtWord* args = event->args;
    f32 px, py, pz, ax, ay, az;
    s32 msec, curve;
    f32 yaw;
    CameraEntry* camera;
    MarioWork* mario;
    Mtx mtx;
    Vec marPos, marAt;
    Vec pos, at;

    px = evtGetValue(event, args[0]);
    py = evtGetValue(event, args[1]);
    pz = evtGetValue(event, args[2]);
    ax = evtGetValue(event, args[3]);
    ay = evtGetValue(event, args[4]);
    az = evtGetValue(event, args[5]);
    msec = evtGetValue(event, args[6]);
    curve = evtGetValue(event, args[7]);
    yaw = -camGetPtr(CAMERA_3D)->viewYaw;
    camera = camGetPtr(CAMERA_3D);
    mario = marioGetPtr();
    pos = (Vec){px, py, pz};
    at = (Vec){ax, ay, az};
    PSMTXRotRad(mtx, 'Y', 0.017453292f * yaw);
    PSMTXMultVec(mtx, &pos, &pos);
    PSMTXMultVec(mtx, &at, &at);
    PSVECAdd(&mario->position, &at, &marAt);
    PSVECAdd(&marAt, &pos, &marPos);
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = marPos;
    camera->storedAt = marAt;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 3;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// unused (dead-stripped): only its string and 100.0f survive
static void evt_cam_debug_print(f32 a) /* restore */ {
    OSReport("%f%f%f%f%f%f%d%s", a / 100.0f);
}

// s32 msec, s32 curve
USER_FUNC(evt_cam3d_evt_off) {
    EvtWord* args = event->args;
    s32 msec, curve;
    CameraEntry* camera;

    msec = evtGetValue(event, args[0]);
    curve = evtGetValue(event, args[1]);
    camera = camGetPtr(CAMERA_3D);
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = 1;
    camera->curveA = curve;
    return EVT_RETURN_DONE;
}

// BOOL on
USER_FUNC(evt_cam3d_road_shift_onoff) {
    s32 on = evtGetValue(event, *event->args);
    CameraEntry* camera = camGetPtr(CAMERA_3D);

    if (on) {
        camera->flags &= ~0x20;
    }
    else {
        camera->flags |= 0x20;
    }
    return EVT_RETURN_DONE;
}

// s32 x, s32 y, s32 z, s32 dolly, s32 msec, s32 curve
USER_FUNC(evt_cam3d_evt_set_xyz) {
    EvtWord* args = event->args;
    s32 x, y, z, dolly, msec, curve;
    CameraEntry* camera;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    dolly = evtGetValue(event, args[3]);
    msec = evtGetValue(event, args[4]);
    curve = evtGetValue(event, args[5]);
    camera = camGetPtr(CAMERA_3D);
    camera->anchorPrev = (Vec){camera->blendPos.x, camera->blendPos.y, camera->blendPos.z};
    camera->anchor = (Vec){x, y, z};
    camera->dollyStartAlt = camera->dollyPercent;
    camera->dollyEnd = dolly;
    camera->startTimeB = gp->renderTime;
    camera->durationB = OSMillisecondsToTicks(msec);
    camera->stateB = 3;
    camera->curveB = curve;
    return EVT_RETURN_DONE;
}

// s32 msec, s32 curve
USER_FUNC(evt_cam3d_evt_xyz_off) {
    EvtWord* args = event->args;
    s32 msec, curve;
    CameraEntry* camera;

    msec = evtGetValue(event, args[0]);
    curve = evtGetValue(event, args[1]);
    camera = camGetPtr(CAMERA_3D);
    camera->anchorPrev = (Vec){camera->blendPos.x, camera->blendPos.y, camera->blendPos.z};
    camera->dollyStartAlt = camera->dollyPercent;
    camera->startTimeB = gp->renderTime;
    camera->durationB = OSMillisecondsToTicks(msec);
    camera->stateB = 1;
    camera->curveB = curve;
    return EVT_RETURN_DONE;
}

// f32 x, f32 y, f32 z, out f32 shift
USER_FUNC(evt_cam3d_get_shift) {
    EvtWord* args = event->args;
    MarioWork* mario;
    CameraEntry* camera;
    f32 x, y, z;
    f32 sx, sy, sz;
    f32 shift;

    mario = marioGetPtr();
    camera = camGetPtr(CAMERA_3D);
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    sx = mario->position.x;
    sy = mario->position.y;
    sz = mario->position.z;
    mario->position.x = x;
    mario->position.y = y;
    mario->position.z = z;
    camShiftMain(camera, mario, &shift);
    mario->position.x = sx;
    mario->position.y = sy;
    mario->position.z = sz;
    evtSetFloat(event, args[3], shift);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam3d_evt_set_now) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);

    camera->storedPos = camera->cameraPos;
    camera->presetPos = camera->storedPos;
    camera->storedAt = camera->cameraAt;
    camera->presetAt = camera->storedAt;
    camera->startTimeA = gp->renderTime;
    camera->durationA = 0;
    camera->stateA = 3;
    camera->curveA = 11;
    return EVT_RETURN_DONE;
}

// f32 x, f32 y, f32 z, f32 w, out f32 posX, posY, posZ, atX, atY, atZ
USER_FUNC(evt_cam3d_event_from_road) {
    EvtWord* args = event->args;
    f32 x, y, z, w;
    CameraParams out;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    w = evtGetFloat(event, args[3]);
    camRoadMain(&out, x, y, z, x, y, z, w);
    evtSetFloat(event, args[4], out.pos.x);
    evtSetFloat(event, args[5], out.pos.y);
    evtSetFloat(event, args[6], out.pos.z);
    evtSetFloat(event, args[7], out.at.x);
    evtSetFloat(event, args[8], out.at.y);
    evtSetFloat(event, args[9], out.at.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam_road_reset) {
    MarioWork* mario = marioGetPtr();
    CameraEntry* camera = camGetPtr(CAMERA_3D);

    camera->marioPos.x = mario->position.x;
    camera->marioPos.y = 0.01f + mario->position.y;
    camera->marioPos.z = mario->position.z;
    camera->marioAt.x = camera->marioPos.x;
    camera->marioAt.y = camera->marioPos.y;
    camera->marioAt.z = camera->marioPos.z;
    mario->camTarget = mario->position;
    mario->camCollision = mario->position;
    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

// f32 x, f32 y, f32 z
USER_FUNC(evt_cam_road_reset2) {
    EvtWord* args = event->args;
    f32 x, y, z;
    MarioWork* mario;
    CameraEntry* camera;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    mario = marioGetPtr();
    camera = camGetPtr(CAMERA_3D);
    camera->marioPos.x = x;
    camera->marioPos.y = 0.01f + y;
    camera->marioPos.z = z;
    camera->dollyStart = 0.0f;
    camera->marioAt.x = camera->marioPos.x;
    camera->marioAt.y = camera->marioPos.y;
    camera->marioAt.z = camera->marioPos.z;
    mario->camTarget = (Vec){x, y, z};
    mario->camCollision = (Vec){x, y, z};
    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam_shift_reset) {
    marioGetPtr();
    camGetPtr(CAMERA_3D);
    camShiftReset();
    return EVT_RETURN_DONE;
}

// BOOL ortho, CameraId id
USER_FUNC(evt_cam_type_change) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    CameraId id = evtGetValue(event, args[1]);

    if (type == 0) {
        camSetTypePersp(id);
    }
    else {
        camSetTypeOrtho(id);
    }
    return EVT_RETURN_DONE;
}

// BOOL on, BOOL flag
USER_FUNC(evt_cam_letter_box_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    CameraEntry* camera = camGetPtr(CAMERA_2D);

    if (on) {
        camera->flags |= 0x100;
        psndSetFlag(0x20);
        camera->letterBoxAlpha = 255;
        camera->letterBoxTop = 0;
        camera->letterBoxBottom = 0;
        camera->letterBoxCamId = CAMERA_2D;
    }
    else {
        camera->flags &= ~0x100;
        psndClearFlag(0x20);
    }
    if (flag) {
        camera->flags |= 0x400;
    }
    return EVT_RETURN_DONE;
}

// BOOL disable
USER_FUNC(evt_cam_letter_box_disable) {
    s32 disable = evtGetValue(event, *event->args);
    CameraEntry* camera = camGetPtr(CAMERA_2D);

    if (disable) {
        camera->flags |= 0x200;
        psndSetFlag(0x40);
    }
    else {
        camera->flags &= ~0x200;
        psndClearFlag(0x40);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cam_letter_box_wait) {
    CameraEntry* camera = camGetPtr(CAMERA_2D);

    if ((camera->flags & 0x100) && camera->letterBoxTop == 49 && camera->letterBoxBottom == 44) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

// CameraId id
USER_FUNC(evt_cam_letter_box_camid) {
    CameraId id = evtGetValue(event, *event->args);

    camGetPtr(CAMERA_2D)->letterBoxCamId = id;
    return EVT_RETURN_DONE;
}

