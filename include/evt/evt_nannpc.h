#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "evt/evt_cmd.h"

struct ExtdrvPose;
struct HitEntry;
struct TobiWork;

// One crowd NPC of the evt_nannpc system. The map rels (gor_00, muj_00, muj_10, muj_20) drive their NPCs through the
// func callback and keep their own state in the slots evt_nannpc leaves to the user; those slots are
// unions with the per-user names.
typedef struct NanNPC {
    s8 name[0x10];    // 0x0
    struct ExtdrvPose* pose; // 0x10, its extdrv pose
    u16 flags;        // 0x14
    u16 field_16;     // 0x16
    Vec pos;          // 0x18
    union {
        Vec hosei;    // 0x24, display offset (evt_nannpc_set_hosei_position)
        Vec start;    // 0x24, muj_00: jump start position
        Vec dest;     // 0x24, gor_00: walk destination (gundan1000tai_seq01: jump start)
    };
    union {
        Vec target;   // 0x30, move target (y -1000: none)
        Vec basePos;  // 0x30, muj_10: base position of the bob; gor_00: walk start / where it stopped
    };
    Vec move;         // 0x3C
    f32 sizeX;        // 0x48
    f32 sizeY;        // 0x4C
    OSTime startTime; // 0x50
    s16* animtbl;     // 0x58
    s16 animnum;      // 0x5C
    s16 anim;         // 0x5E
    f32 dir;          // 0x60
    void (*func)(struct NanNPC*); // 0x64
    union {
        s32 ipos[3];  // 0x68, integer start position of evt_nannpc_jump
        struct {
            s32 state;     // 0x68, map rel state machine
            union {
                s32 duration;  // 0x6C, ms for the current state
                s32 frame;     // 0x6C, gor_00: frames walked so far
            };
            union {
                s32 nextState; // 0x70
                s32 frameNum;  // 0x70, gor_00: frames the walk takes
                s32 index;     // 0x70, muj_20: index in the crowd
            };
        };
    };
    union {
        u8 pad_74[4];     // 0x74
        s32 slot;         // 0x74, gor_00: index in the crowd (wait group)
        s32 dist;         // 0x74, muj_20: distance from the centre
    };
    union {
        f32 vy;           // 0x78, muj_00: vertical speed
        f32 speed;        // 0x78, gor_00: walk speed; jump: t^2 coefficient
        f32 maxX;         // 0x78, gor_00: right end of the pacing range
    };
    union {
        f32 dy;           // 0x7C, muj_00: vertical speed step
        f32 moveDir;      // 0x7C, gor_00: walk direction; jump: t coefficient
        f32 minX;         // 0x7C, gor_00: left end of the pacing range
    };
    union {
        u8 pad_80[8];     // 0x80
        f32 stepX;        // 0x80, gor_00: pacing step (+1 or -1)
    };
    union {
        Vec startPos;     // 0x88, evt_nannpc_move_position2
        Vec jumpEnd;      // 0x88, gor_00: jump landing position
    };
    u8 pad_94[0xC];   // 0x94
    struct TobiWork* tobi; // 0xA0, muj_00: jump-at target data
    union {
        s32 work[4];  // 0xA4, evt_nannpc_set_work / get_work slots
        struct {
            s32 angle;  // 0xA4, muj_00: orbit angle
            s32 radius; // 0xA8, muj_00: orbit radius
        };
        struct {
            s32 arrived; // 0xA4, gor_00: reached its spot
            s32 recall;  // 0xA8, gor_00: gundan1000tai_totsuend called them back
        };
    };
    struct HitEntry* hit0; // 0xB4
    struct HitEntry* hit1; // 0xB8
    GXColor color;    // 0xBC
} NanNPC;

typedef struct NanNPCSort {
    s32 idx;
    f32 z;
} NanNPCSort;

typedef struct NanNPCWork {
    NanNPC* npc;       // 0x0
    NanNPCSort* sort;  // 0x4
    s16* tbl;          // 0x8
    s32 num;           // 0xC
    u32 flags;         // 0x10
    void (*func14)();  // 0x14
    void (*func18)();  // 0x18
    void (*func1c)();  // 0x1C
} NanNPCWork;

extern NanNPCWork* nanNPCWork;
void nanNPCOption(void);
void evt_nannpc_set_animtbl_sub(NanNPC* npc, s16* tbl);
void nannpc_ext_main_sub_fast();
USER_FUNC(evt_nannpc_flag_onoff);
USER_FUNC(evt_nannpc_get_position);
USER_FUNC(evt_nannpc_get_work);
USER_FUNC(evt_nannpc_init);
USER_FUNC(evt_nannpc_jump_position);
USER_FUNC(evt_nannpc_move_position2);
USER_FUNC(evt_nannpc_ppflag_onoff);
USER_FUNC(evt_nannpc_set_animtbl);
USER_FUNC(evt_nannpc_set_color);
USER_FUNC(evt_nannpc_set_dir);
USER_FUNC(evt_nannpc_set_func);
USER_FUNC(evt_nannpc_set_hosei_position);
USER_FUNC(evt_nannpc_set_position);
USER_FUNC(evt_nannpc_set_shadow_position);
USER_FUNC(evt_nannpc_set_size);
USER_FUNC(evt_nannpc_set_subfunc);
USER_FUNC(evt_nannpc_set_work);
USER_FUNC(evt_nannpcwork_flag_onoff);
