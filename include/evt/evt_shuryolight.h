// Shuryo (Grodus) lights: the five lamps (power, three head, staff) drawn on Sir Grodus's NPC, carried as evt_shuryolight.c by both
// aji (X-Naut fortress) and las (Palace of Shadow).
#pragma once

#include "evt/evt_cmd.h"

#include "driver/npcdrv.h"

// One lamp: a shape of a hidden c_shuryo helper NPC, drawn at a locator group of Grodus's pose.
typedef struct ShuryoLight {
    NpcEntry* npc;      // 0x00
    const char** anims; // 0x04, five animation names (green / orange / red / yellow lamp tables)
    s32 status;         // 0x08, index into anims last set
    s32 group;          // 0x0C, animPoseGetGroupIdx of the base pose ("locator6" ...)
    s32 shape;          // 0x10, animPoseGetShapeIdx of the lamp NPC
} ShuryoLight;

typedef struct ShuryoWork {
    s32 powerEvt;          // 0x00, eventId of the running "pow" script, 0 if none
    s32 headEvt;           // 0x04, "head" script
    s32 tueEvt;            // 0x08, "tue" (staff) script
    ShuryoLight lights[5]; // 0x0C
} ShuryoWork;

USER_FUNC(evt_shuryolight_init);
USER_FUNC(evt_shuryolight_run_evt);
