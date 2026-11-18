#pragma once

#include "version.h"
#include "dolphin/gx/GXStruct.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include <dolphin/types.h>
#include "evt/evt_word.h"
#include <dolphin/mtx.h>

struct EventEntry;

typedef struct MapObjectHitEntry {
    HitEntry* hitObject;       // 0x0
    char name[16];             // 0x4
    u8 jointData[0x88 - 0x14]; // 0x14, 0x1C: MapFileJoint copied from the cube/head/front joint templates (jointData + 8)
} MapObjectHitEntry;

// MapObjectEntry.flags, named from mobjMain and the evt_mobj setters. The MOBJ_FLAG_HIT_* bits arm one hit source
// each; mobjMain stores the bit that fired in MapObjectEntry.hitFlag and sets MOBJ_FLAG_HIT.
// MOBJ_FLAG_SHOW_HINT is read by mobjGetHint and Mario's interaction-balloon/jump checks.
typedef enum MapObjectFlags {
    MOBJ_FLAG_ACTIVE = (1 << 0),               // 0x1, slot in use (mobjEntry; cleared on delete)
    MOBJ_FLAG_NO_SHADOW = (1 << 1),            // 0x2, no shadow (set once a falling/floating object rests)
    MOBJ_FLAG_UPDATE_MTX = (1 << 2),           // 0x4, matrix recalculated on the next update, then cleared
    MOBJ_FLAG_HIT = (1 << 3),                  // 0x8, a hit fired; no new hit checks until cleared
    MOBJ_FLAG_HITEVT_OFF = (1 << 4),           // 0x10, evt_mobj_hitevt_onoff(name, 0)
    MOBJ_FLAG_SHOW_HINT = (1 << 5),           // 0x20, mobjGetHint; lock/item-box interaction prompt
    MOBJ_FLAG_GRAVITY = (1 << 6),              // 0x40, falls and bounces off the floor
    MOBJ_FLAG_NO_CULL = (1 << 7),              // 0x80, drawn without the view frustum check
    MOBJ_FLAG_HIT_EVERY_FRAME = (1 << 8),      // 0x100, MOBJ_FLAG_HIT is cleared again every frame
    MOBJ_FLAG_BUSY = (1 << 9),                 // 0x200, the pause menu does not open (mobjCheckExec); LLCOOLT's name
    MOBJ_FLAG_PAPER_TEX = (1 << 10),           // 0x400, drawn with its paper texture (mobjDisp*)
    MOBJ_FLAG_HIDDEN = (1 << 11),              // 0x800, hidden block (no shadow) until it is hit
    MOBJ_FLAG_EXEC_CANCEL = (1 << 12),         // 0x1000, evt_mobj_exec_cancel
    MOBJ_FLAG_BLINK = (1 << 13),               // 0x2000, drawn every other 8 frames
    MOBJ_FLAG_NO_DISP = (1 << 14),             // 0x4000, not drawn
    MOBJ_FLAG_HIT_HAMMER = (1 << 16),          // 0x10000, Mario's hammer
    MOBJ_FLAG_HIT_A_BUTTON = (1 << 18),        // 0x40000, A pressed in front of it
    MOBJ_FLAG_HIT_KOOPA_FRONT = (1 << 19),     // 0x80000, Bowser touches it from the front
    MOBJ_FLAG_HIT_STAND = (1 << 21),           // 0x200000, Mario or Bowser stands on it
    MOBJ_FLAG_HIT_HEAD = (1 << 22),            // 0x400000, Mario's head from below
    MOBJ_FLAG_HIT_PARTY = (1 << 23),           // 0x800000, the partner's action
    MOBJ_FLAG_HIT_SPIN_JUMP = (1 << 24),       // 0x1000000, Mario's hip bump while standing on it
    MOBJ_FLAG_HIT_BOBBERY = (1 << 25),         // 0x2000000, Bobbery's action
    MOBJ_FLAG_HIT_KOOPA_HEAD = (1 << 26),      // 0x4000000, Bowser's head
    MOBJ_FLAG_HIT_KOOPA_BODY_LV3 = (1 << 27),  // 0x8000000, Bowser's body at power level 3
    MOBJ_FLAG_HIT_KOOPA_HIP = (1 << 28),       // 0x10000000, Bowser's hip attack while riding it
    MOBJ_FLAG_HIT_KOOPA_FLAME = (1 << 29),     // 0x20000000, Bowser's flame
} MapObjectFlags;

typedef struct MapObjectEntry {
    s32 flags;                               // 0x0, MapObjectFlags
    u8 camId;                                // 0x4, camera the object is drawn with (CAMERA_3D; evt_mobj can change it)
    char name[16];                           // 0x5, mobjEntry name; mobjNameToPtr finds the object by it
    char animName[16];                       // 0x15, animPoseEntry name of the model
    u8 pad_25[0x38 - 0x25];                  // 0x25, never accessed
    Vec position;                            // 0x38, world position (dispEntry depth, animPoseDraw)
    Vec scale;                               // 0x44
    Vec rotation;                            // 0x50, degrees; animPoseDraw uses y, the hit matrices x/y/z
    Vec velocity;                            // 0x5C, y used for falling/bouncing
    f32 gravity;                             // 0x68, subtracted from velocity.y each frame
    f32 bounce;                              // 0x6C, restitution on landing (-0.5 * bounce * vy)
    s32 poseId;                              // 0x70, animPoseEntry of the model
    MapObjectHitEntry hitObj[2];             // 0x74, collision "MOBJ_<name>" / "MOBJ~<name>" (mobjHitEntry)
    Vec scale2;                              // 0x184, collision box scale from the model's bounding box (mobjHitEntry)
    Vec translation;                         // 0x190, collision box centre offset from position (mobjHitEntry)
    s32 offscreenId;                         // 0x19C, -1: drawn normally; else its offscreen (evt_mobj)
    union {                      // 0x1A0, per-block-type data (item id / item list, cost, state, ...; evt_mobj.c)
        s32 typeData[4];
        u32 typeDataU[4];
        void* typeDataP[4];
        s32* typeDataList[4];    // -1-terminated item id lists
        f32 typeDataF[4];
    };
    s32 timer;                               // 0x1B0, per-type countdown (timer switch, blinking, respawn delays)
    s32 typeWork;                            // 0x1B4, per-type: timer switch reload length; fire bar loop SFX id (evt_mobj.c)
    char linkedName[0x1C8 - 0x1B8];          // 0x1B8, name of a paired mobj (mobjNameToPtr)
    struct EffectEntry* effect;              // 0x1C8, coin fukidashi effect (evt_mobj.c)
    s32 eventId;                             // 0x1CC, last script mobjRunEvent started (no restart while it runs)
    s32 (*mainFunc)(struct MapObjectEntry*); // 0x1D0, per-type update (mobjMain); -1 skips the rest of the update
    EvtWord event;                           // 0x1D4, script mobjRunEvent starts when triggered (evt_mobj)
    EvtWord extraEvent;                      // 0x1D8, second event script run via mobjRunEvent
    s32 state;                               // 0x1DC, per-type state (evt_mobj); 99 = finished (opened chest, ...)
    s32 hitFlag;                             // 0x1E0, the MOBJ_FLAG_HIT_* bit that fired
    s32 flagVar;                             // 0x1E4, evt variable set to 1 when used; already set at entry: finished
    GXTexObj paperTexObj;                    // 0x1E8, screen copy for MOBJ_FLAG_PAPER_TEX objects (evt_mobj)
    struct smartEntry* paperTexData;         // 0x208, texture data for paperTexObj
    Mtx paperTexMtx;                         // 0x20C, texture matrix for paperTexObj (identity)
} MapObjectEntry;

typedef struct MapObjectWork {
    s32 count;                // 0x0
    MapObjectEntry* entries;  // 0x4
    HitEntry* hit[5];         // 0x8, cached hit objects
} MapObjectWork;

void mobjInit(void);
void mobjMain(void);
MapObjectEntry* mobjNameToPtrNoAssert(const char* name);
MapObjectEntry* mobjNameToPtr(const char* name);
MapObjectEntry* mobjNumberToPtr(s32 index);
void mobjCalcMtx(MapObjectEntry* mobj);
void mobjCalcMtx2(MapObjectEntry* mobj);
void mobjHitCheckGround(MapObjectEntry* entry);

void mobjDelete(const char* name);
s32 mobjEntry(const char* name, const char* animPoseName);
void mobjHitEntry(MapObjectEntry* entry, s32 type);
MapObjectEntry* mobjHitObjPtrToPtr(HitEntry* hitObj);

BOOL mobjCheckExec(void);
void mobjReset(BOOL inBattle);
void mobjSetPosition(const char* name, f32 x, f32 y, f32 z);
void mobjGetPosition(const char* name, f32* x, f32* y, f32* z);
void mobjKoopaOn(void);
s32 mobjCheckItemboxOpen(MapObjectEntry* entry);
BOOL mobjGetHint(MapObjectEntry* entry);

#if VERSION <= VERSION_DEMO_JP
MapObjectEntry* mobjNearDistCheck(f32 x, f32 y, f32 z, f32 minMagnitude);
#endif

MapObjectEntry* mobjNearDistCheck2(f32 x, f32 y, f32 z, f32 minMagnitude, char** names);

int mobjCheckKururingFloorItem(MapObjectEntry* entry);

struct EventEntry* mobjRunEvent(MapObjectEntry* entry, EvtWord* eventCode);
