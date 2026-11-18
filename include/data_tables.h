// Layouts of data tables that scripts and battle code read (PoseSoundTiming, DoorParam, QuizEntry...).

#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"
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
    u16 flags;            // 0x12, BtlPoseSoundFlags (battle/battle_unit.h)
} PoseSoundTiming;

/* evt_ext_entry: ExtTexInfo tables (driver/extdrv.h) */

/* door_entry / evt_door_set_param: per-door parameter block (npcs/mobjs/items are NULL-terminated name lists) */
typedef struct DoorParam {
    s32 flags;              // 0x0, evt_door_param kind 9; 1 = hide/show NPCs through the door, 2 = skip the open SFX
    s32 type;               // 0x4, hinge side for door_position_sub (0/1 or 2: which way the outside spot shifts)
    s32 se;                 // 0x8, door sound set (door_open_se; 0 = silent)
    const char* mapobj;     // 0xC, the door's map object; its width sets the outside spot's sideways shift
    const char* hitIn;      // 0x10, inside spot is measured from it (its normal); door sounds play there
    const char* hitOut;     // 0x14, hit object the outside spot is measured from (its normal)
    const char* outside;    // 0x18, map object group of the outside (evt_door_open saves its flags)
    const char* outsideHit; // 0x1C
    const char* inside;     // 0x20, map object group of the inside (evt_door_open shows it)
    const char* insideHit;  // 0x24, hit object evt_door_open switches on with the inside
    EvtWord* evtIn;            // 0x28
    EvtWord* evtOut;           // 0x2C
    EvtWord* evtWall;          // 0x30, script evt_door_open runs with house_open_se under DOOR_OPT_HOUSE_ANIM_FIRST
    const char** npcs;      // 0x34, NPCs left out of the map blend (door_dark_flag)
    const char** mobjs;     // 0x38, mobjs left out of the map blend
    const char** items;     // 0x3C, items left out of the map blend
    EvtWord* evtOpen;          // 0x40, script spawned when evt_door_open starts
    EvtWord* evtClose;         // 0x44
} DoorParam;

/* evt_unitwin_selltable_setup */
typedef struct UnitWinSell {
    const char* mapObj;  // 0x0
    const char* hitName; // 0x4
    EvtWord* evt;           // 0x8
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
    EvtWord* toBedEvt;     // 0x90
    EvtWord* fromBedEvt;   // 0x94
} KinopioData;

/* evt_shop_setup arg2: shopper name, hooks and the 35 shop messages */
typedef struct ShopperData35 {
    const char* name;     // 0x0
    EvtWord* fook_evt;       // 0x4
    EvtWord* buy_evt;        // 0x8
    const char* msgs[35]; // 0xC
} ShopperData35;

/* evt_sub_kusa_access_entry: NULL-terminated by a type 4 entry */
typedef struct KusaAccess {
    s32 type;         // 0x0
    const char* hit;  // 0x4
    const char* mobj; // 0x8
    EvtWord* evt;        // 0xC
} KusaAccess;

/* evt_sub_tree_access_entry: terminated by a type 7 entry */
typedef struct TreeAccess {
    s32 type;          // 0x0
    const char* hit;   // 0x4
    const char* trunk; // 0x8
    const char* leaf;  // 0xC
    const char* leafBack; // 0x10, back leaf model (0 in most tables, no C reader)
    EvtWord* evt;         // 0x14
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
