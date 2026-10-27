#include "eff/eff_peach_fukidashi.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "manager/fontmgr.h"
#include "mario/mario.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "icon_ids.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffPeachFukidashiWork {
    u32 flags;      // 0x0, bits 0-1: icon, 0x80: show a cross
    Vec position;   // 0x4
    f32 angle;      // 0x10
    f32 size;       // 0x14
    f32 rot;        // 0x18
    s32 timer;      // 0x1C
    NpcEntry* npc;  // 0x20, NULL = Mario
} EffPeachFukidashiWork;

// place the bubble next to the speaker's head, rotated around the camera's view axis
static inline void _setPosition(EffPeachFukidashiWork* work, f32 offset) {
    CameraEntry* camera;
    Vec axis;
    Vec base;
    f32 height, dist;
    Mtx rot;

    camera = camGetPtr(CAMERA_3D);
    axis = (Vec){ (f32)sin(MTXDegToRad(camera->viewYaw)), 0.0f, -(f32)cos(MTXDegToRad(camera->viewYaw)) };
    dist = 0.5f * (offset + work->size);
    if (work->npc) {
        base = work->npc->position;
        height = work->npc->height;
    } else {
        MarioWork* mario = marioGetPtr();
        base = mario->position;
        height = mario->wPlayerCollisionBox.y;
    }
    PSMTXRotAxisRad(rot, &axis, MTXDegToRad(-work->angle));
    work->position.x = rot[0][1] * dist + base.x;
    work->position.y = rot[1][1] * dist + (base.y + height);
    work->position.z = rot[2][1] * dist + base.z;
    work->rot = work->angle;
}

EffectEntry* effPeachFukidashiEntry(s32 type, NpcEntry* npc, s32 duration, f32 x, f32 y, f32 z,
                                    f32 size, f32 angle) {
    EffectEntry* effect;
    EffPeachFukidashiWork* work;

    effect = effEntry();
    effect->type = "PeachFukidashi";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPeachFukidashiWork) * effect->count);
    effect->userdata = work;
    effect->callback = effPeachFukidashiMain;
    work->flags = type;
    work->size = size;
    work->angle = angle;
    work->timer = duration;
    work->npc = npc;
    _setPosition(work, 16.0f);
    return effect;
}

void effPeachFukidashiMain(EffectEntry* effect) {
    EffPeachFukidashiWork* work = effect->userdata;

    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, 0.0f, 2, (DispCallback)effPeachFukidashiDisp, effect);
}

void effPeachFukidashiDisp(CameraId cameraId, EffectEntry* effect) {
    EffPeachFukidashiWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx mtx2;
    Mtx out;

    camGetPtr(cameraId);

    // bubble
    _setPosition(work, 16.0f);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx2);
    PSMTXScale(scale, 0.4f, 0.34f, 0.4f);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->rot));
    PSMTXConcat(mtx2, scale, out);
    PSMTXConcat(out, rot, out);
    iconDispGx2(out, 0, ICON_FUKIDASHI);

    // icon inside the bubble
    _setPosition(work, 40.0f);
    PSMTXTrans(mtx, work->position.x,
               7.25f + ((work->position.y - 14.0f) + __fabsf(1.5f * work->angle / 90.0f)),
               work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx2);
    PSMTXTrans(mtx, 0.0f, 0.0f, 1.0f);
    PSMTXConcat(mtx2, mtx, mtx2);
    PSMTXScale(scale, 0.4f, 0.4f, 0.4f);
    PSMTXConcat(mtx2, scale, mtx2);
    switch (work->flags & 3) {
        case 0:
            iconDispGx2(mtx2, 0, ICON_BUTTON_X_ALT);
            break;
        case 1:
            iconDispGx2(mtx2, 0, ICON_BUTTON_A_ALT);
            break;
        case 2:
        default:
            iconDispGx2(mtx2, 0, ICON_BUTTON_B_ALT);
            break;
    }

    // red cross over the icon
    if (work->flags & 0x80) {
#ifdef VERSION_G8ME01
        char text[32] = "\xD7"; // US: Latin-1 multiplication sign
#else
        char text[32] = "\x81\x7E";
#endif
        GXColor red = { 0xFF, 0x00, 0x00, 0xFF };

        PSMTXScale(scale, 0.75f, 0.75f, 0.75f);
        PSMTXTrans(mtx, 6.0f + (work->position.x - 11.5f),
                   (30.0f + ((work->position.y - 14.0f) + __fabsf(1.5f * work->angle / 90.0f))) - 8.0f,
                   work->position.z);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx2);
        PSMTXTrans(mtx, 0.0f, 0.0f, 1.0f);
        PSMTXConcat(mtx2, mtx, mtx2);
        PSMTXConcat(mtx2, scale, mtx2);
        FontDrawStart();
        FontDrawColor(red);
        FontDrawStringMtx(mtx2, text);
    }
}

void effSetPeachFukiBatu(const char* name) {
    ((EffPeachFukidashiWork*)effNameToPtr(name)->userdata)->flags |= 0x80;
}
