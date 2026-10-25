#pragma once

#include "dolphin/gx/GXStruct.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include <dolphin/types.h>
#include "evt/evt_word.h"
#include <dolphin/mtx.h>

typedef struct MapObjectHitEntry {
    HitEntry* hitObject;       // 0x0
    char name[16];             // 0x4
    u8 jointData[0x88 - 0x14]; // 0x14, 0x1C: MapFileJoint copied from the cube/head/front joint templates (jointData + 8)
} MapObjectHitEntry;

typedef struct MapObjectEntry {
    s32 flags;                               // 0x0
    u8 camId;                                // 0x4
    char name[16];                           // 0x5
    char animName[16];                       // 0x15
    u8 pad_25[0x38 - 0x25];                  // 0x25, never accessed
    Vec position;                            // 0x38
    Vec scale;                               // 0x44
    Vec rotation;                            // 0x50
    Vec velocity;                            // 0x5C, y used for falling/bouncing
    f32 gravity;                             // 0x68, subtracted from velocity.y each frame
    f32 bounce;                              // 0x6C, restitution on landing (-0.5 * bounce * vy)
    s32 poseId;                              // 0x70
    MapObjectHitEntry hitObj[2];             // 0x74
    Vec scale2;                              // 0x184
    Vec translation;                         // 0x190
    s32 offscreenId;                         // 0x19C
    union {                      // 0x1A0, per-block-type data (item id / item list, cost, state, ...; evt_mobj.c)
        s32 typeData[4];
        u32 typeDataU[4];
        void* typeDataP[4];
        s32* typeDataList[4];    // -1-terminated item id lists
        f32 typeDataF[4];
    };
    s32 timer;                               // 0x1B0, per-type countdown (timer switch, blinking, respawn delays)
    s32 typeWork;                            // 0x1B4, per-type: timer switch reload length; evt_mobj.c:4149 mobj loop SFX id
    char linkedName[0x1C8 - 0x1B8];          // 0x1B8, name of a paired mobj (mobjNameToPtr)
    struct EffectEntry* effect;              // 0x1C8, coin fukidashi effect (evt_mobj.c)
    s32 eventId;                             // 0x1CC
    s32 (*mainFunc)(struct MapObjectEntry*); // 0x1D0
    EvtWord event;                           // 0x1D4
    EvtWord extraEvent;                      // 0x1D8, second event script run via mobjRunEvent
    s32 state;                               // 0x1DC
    s32 hitFlag;                             // 0x1E0
    s32 flagVar;                             // 0x1E4
    GXTexObj paperTexObj;                    // 0x1E8
    struct smartEntry* paperTexData;         // 0x208, texture data for paperTexObj
    Mtx paperTexMtx;                         // 0x20C
} MapObjectEntry;

typedef struct MapObjectWork {
    s32 count;                // 0x0
    MapObjectEntry* entries;  // 0x4
    u8 pad_08[0x1C - 0x8]; // 0x8, never accessed
} MapObjectWork;

void mobjInit(void);
void mobjMain(void);
MapObjectEntry* mobjNameToPtrNoAssert(const char* name);
MapObjectEntry* mobjNameToPtr(const char* name);
void mobjCalcMtx(MapObjectEntry* mobj);
void mobjCalcMtx2(MapObjectEntry* mobj);

void mobjDelete(const char* name);
s32 mobjEntry(const char* name, const char* animPoseName);
void mobjHitEntry(MapObjectEntry* entry, s32 type);
MapObjectEntry* mobjHitObjPtrToPtr(HitEntry* hitObj);

extern const char kururinName[];

BOOL mobjCheckExec(void);
void mobjReset(BOOL inBattle);
void mobjSetPosition(const char* name, f32 x, f32 y, f32 z);
void mobjKoopaOn(void);
s32 mobjCheckItemboxOpen(MapObjectEntry* entry);
BOOL mobjGetHint(MapObjectEntry* entry);
MapObjectEntry* mobjNearDistCheck2(f32 x, f32 y, f32 z, f32 minMagnitude, char** names);
