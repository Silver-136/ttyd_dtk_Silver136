// Work record of the room copies of the cloud (Flurrie's gust) handler in gon_11, las_14 and mri_00.
// Each of those rooms carries its own copy of the code (evt_iri_16_, evt_las_ and evt_mri_cloud_init / _ent) and allocates 16 records.
// The record differs from evt_cloud.c's CloudWork in main.dol: a used word instead of flags, eight group names in place
// of one name plus a list pointer, and a bounding box (x, y, w, h).

#pragma once

#include <dolphin/types.h>
#include "manager/evtmgr.h"

typedef struct CloudRoomWork {
    s32 used;                 // 0x0
    s32 state;                // 0x4
    char animName[0x20];      // 0x8
    char imgName[0x20];       // 0x28
    char offscreenName[0x20]; // 0x48
    char groupNames[8][0x20]; // 0x68
    char hitName[0x20];       // 0x168
    s32 limit;                // 0x188
    s32 counter;              // 0x18C, advances by 2 during contact and the loss-of-contact grace period
    EventEntry* evt;          // 0x190
    void* endEvt;             // 0x194
    s32 timer;                // 0x198
    s32 x;                    // 0x19C
    s32 y;                    // 0x1A0
    s32 w;                    // 0x1A4
    s32 h;                    // 0x1A8
    f32 lastFrame;            // 0x1AC
} CloudRoomWork;
