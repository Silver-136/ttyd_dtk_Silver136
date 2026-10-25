#pragma once

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include "driver/extdrv.h"

/* Layouts of data tables read by the event/battle code (see the readers named in each comment). */

/* btlevtcmd_AnimeSetPoseSoundTable */
typedef struct PoseSoundTiming {
    const char* animName; // 0x0
    f32 time;             // 0x4
    void* callback;       // 0x8
    const char* sfxName;  // 0xC
    s16 pitch;            // 0x10
    u16 flags;            // 0x12
} PoseSoundTiming;

/* evt_ext_entry: ExtTexInfo tables (driver/extdrv.h) */

/* door_entry / evt_door_set_param: per-door parameter block (npcs/mobjs/items are NULL-terminated name lists) */
typedef struct DoorParam {
    s32 flags;              // 0x0, evt_door_param kind 9; 1 = hide/show NPCs through the door, 2 = skip the open SFX
    s32 type;               // 0x4
    s32 se;                 // 0x8
    const char* mapobj;     // 0xC
    const char* hitIn;      // 0x10
    const char* hitOut;     // 0x14
    const char* outside;    // 0x18
    const char* outsideHit; // 0x1C
    const char* inside;     // 0x20
    const char* insideHit;  // 0x24
    void* evtIn;            // 0x28
    void* evtOut;           // 0x2C
    void* evtWall;          // 0x30
    const char** npcs;      // 0x34
    const char** mobjs;     // 0x38
    const char** items;     // 0x3C
    void* evtOpen;          // 0x40
    void* evtClose;         // 0x44
} DoorParam;

/* evt_unitwin_selltable_setup */
typedef struct UnitWinSell {
    const char* mapObj;  // 0x0
    const char* hitName; // 0x4
    void* evt;           // 0x8
} UnitWinSell;

/* evt_kinopio_setup */
typedef struct KinopioData {
    s32 breakfastItem;  // 0x0
    Vec breakfastPos;   // 0x4
    s32 ryoukin;        // 0x10
    Vec bed;            // 0x14
    Vec bedside;        // 0x20
    f32 cam[6];         // 0x2C
    f32 bedDeg;         // 0x44
    Vec npcTalk;        // 0x48
    const char* npcName; // 0x54
    const char* msg[14]; // 0x58
    void* toBedEvt;     // 0x90
    void* fromBedEvt;   // 0x94
} KinopioData;

/* evt_shop_setup arg2: shopper name, hooks and the 35 shop messages */
typedef struct ShopperData35 {
    const char* name;     // 0x0
    void* fook_evt;       // 0x4
    void* buy_evt;        // 0x8
    const char* msgs[35]; // 0xC
} ShopperData35;

/* evt_sub_kusa_access_entry: NULL-terminated by a type 4 entry */
typedef struct KusaAccess {
    s32 type;         // 0x0
    const char* hit;  // 0x4
    const char* mobj; // 0x8
    void* evt;        // 0xC
} KusaAccess;

/* evt_sub_tree_access_entry: terminated by a type 7 entry */
typedef struct TreeAccess {
    s32 type;          // 0x0
    const char* hit;   // 0x4
    const char* trunk; // 0x8
    const char* leaf;  // 0xC
    const char* leafBack; // 0x10, back leaf model (0 in most tables, no C reader)
    void* evt;         // 0x14
} TreeAccess;

/* door_entry (house entrance with inside/outside map groups); each hit pair is {kind, hit name} */
typedef struct HouseParam {
    s32 type;                // 0x0
    s32 hitInKind;           // 0x4
    const char* hitIn;       // 0x8
    s32 hitOutKind;          // 0xC
    const char* hitOut;      // 0x10
    const char* anim[4];     // 0x14
    const char* outside;     // 0x24
    const char* door;        // 0x28
    const char* inside[2];   // 0x2C
    const char* initInside[2]; // 0x34
    const char** npcs;       // 0x3C
    const char** noclip;     // 0x40
    const char** mobjs;      // 0x44
    const char** items;      // 0x48
} HouseParam;

/* s32 holding an evt float constant (see FLOAT()) */
typedef s32 EvtFloat;

/* map quiz table read by READ_INT4: anim, question msg, choice msg, correct choice index */
typedef struct QuizEntry {
    const char* anim;      // 0x0
    const char* msg;       // 0x4
    const char* choiceMsg; // 0x8
    s32 answer;            // 0xC
} QuizEntry;
