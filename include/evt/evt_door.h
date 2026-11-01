// evt_door.c: scripts and commands for house doors (door_entry, evt_door_open/close and the door sound scripts).

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

// evt_door_param's second argument: which DoorParam fields it returns.
typedef enum DoorParamKind {
    DOOR_PARAM_TYPE = 0,
    DOOR_PARAM_HITS = 1,               // mapobj, hitIn, hitOut
    DOOR_PARAM_OUTSIDE = 2,            // outside, outsideHit
    DOOR_PARAM_INSIDE = 3,             // inside, insideHit
    DOOR_PARAM_IN_POS = 4,
    DOOR_PARAM_OUT_POS = 5,
    DOOR_PARAM_EVTS = 6,               // evtIn, evtOut, evtWall
    DOOR_PARAM_OPEN_CLOSE_EVTS = 7,    // evtOpen, evtClose
    DOOR_PARAM_SE = 8,
    DOOR_PARAM_FLAGS = 9,
    DOOR_PARAM_IN_POS_FROM_MARIO = 10,
    DOOR_PARAM_OUT_POS_FROM_MARIO = 11,
} DoorParamKind;

// DoorParam.flags
#define DOOR_FLAG_HIDE_NPCS 0x1        // npc_hide_onoff hides the listed NPCs while inside
#define DOOR_FLAG_NO_OPEN_SE 0x2       // house_open_se stays silent

// LW(15) of evt_door_open / evt_door_close: option bits the caller sets
#define DOOR_OPT_KEEP_INSIDE_SHOWN 0x1 // closing leaves the inside group visible
#define DOOR_OPT_NO_DARK 0x2           // no door_dark map blend fade
#define DOOR_OPT_HOUSE_ANIM_FIRST 0x4  // the house animation runs before the move instead of after it
#define DOOR_OPT_NO_MARIO_MOVE 0x8     // closing does not walk Mario and the partner
#define DOOR_OPT_POS_FROM_MARIO 0x10   // the walk target is measured from Mario

USER_FUNC(snd_door_in);
USER_FUNC(snd_door_out);
USER_FUNC(door_entry);

extern EvtWord door_close_se[];
extern EvtWord door_dark[];
extern EvtWord door_light[];
extern EvtWord door_light2[];
extern EvtWord door_open_se[];
extern EvtWord door_up_se[];
extern EvtWord evt_door_close[];
extern EvtWord evt_door_close_roll[];
USER_FUNC(evt_door_data_copy);
extern EvtWord evt_door_entry[];
extern EvtWord evt_door_open[];
extern EvtWord evt_door_open_bom_01[];
extern EvtWord evt_door_open_quick[];
extern EvtWord evt_door_open_roll[];
extern EvtWord evt_door_setup[];
extern EvtWord house_close_se[];
USER_FUNC(animation);
USER_FUNC(door_dark_flag);
USER_FUNC(door_group);
USER_FUNC(evt_door_end_wait);
USER_FUNC(evt_door_load_mapflag);
USER_FUNC(evt_door_param);
USER_FUNC(evt_door_save_mapflag);
USER_FUNC(evt_door_set_param);
USER_FUNC(get_noclip_map);
USER_FUNC(in_pos);
USER_FUNC(init_inside_group);
USER_FUNC(inout_hit);
USER_FUNC(inside_group);
USER_FUNC(npc_dark);
USER_FUNC(npc_hide_onoff);
USER_FUNC(npc_light);
USER_FUNC(out_pos);
USER_FUNC(outside_group);
void door_position_sub(s32 type, const char* map, const char* hitIn, const char* hitOut, Vec* in, Vec* out, BOOL mario);
