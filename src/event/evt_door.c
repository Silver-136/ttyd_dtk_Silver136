#include "data_tables.h"
#include "evt/evt_door.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "driver/icondrv.h"
#include "driver/itemdrv.h"
#include "driver/mapdrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "party.h"
#include "memory.h"
#include "pmario_sound.h"
#include <dolphin/mtx.h>

double atan(double);
double cos(double);

typedef struct HouseData {
    s32 type;               // 0x0
    const char* hit[4];     // 0x4, hit object names
    s32 anim[4];            // 0x14
    s32 outside;            // 0x24
    s32 door;               // 0x28
    s32 inside[2];          // 0x2C
    s32 initInside[2];      // 0x34
    const char** npcs;      // 0x3C
    const char** noclip;    // 0x40
    const char** mobjs;     // 0x44
    const char** items;     // 0x48
} HouseData;

typedef struct HouseWork {
    HouseData* data;        // 0x0
    Vec in;                 // 0x4
    Vec out;                // 0x10
} HouseWork;

s32 DoorFLAG;
s32 door_mapflag;

void door_position_sub(s32 type, const char* map, const char* hitIn, const char* hitOut, Vec* in, Vec* out, BOOL mario);
void door_dark_flag_sub(BOOL on, const char** npcs, const char** mobjs, const char** items);


USER_FUNC(N_evt_mario_party_door_halve_hitbox);
USER_FUNC(animation);
USER_FUNC(door_dark_flag);
USER_FUNC(door_entry);
USER_FUNC(door_group);
USER_FUNC(evt_cam3d_event_from_road);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_set_xyz);
USER_FUNC(evt_cam3d_evt_xyz_off);
USER_FUNC(evt_cam3d_get_shift);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_door_load_mapflag);
USER_FUNC(evt_door_param);
USER_FUNC(evt_door_save_mapflag);
USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_map_blend_off);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_blend);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_party_door_move);
USER_FUNC(evt_party_move_beside_mario);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
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
USER_FUNC(snd_door_in);
USER_FUNC(snd_door_out);
USER_FUNC(evt_door_end_wait);
USER_FUNC(evt_door_set_param);
USER_FUNC(evt_door_data_copy);

EvtWord door_open_se[151];
EvtWord door_close_se[151];
EvtWord door_down_se[33];
EvtWord house_open_se[32];
EvtWord evt_dark[34];
EvtWord evt_dark2[34];
static EvtWord evt_light[30];
EvtWord evt_light2[30];
EvtWord door_up_se[2];
EvtWord house_close_se[4];
EvtWord in_houseanim[80];
EvtWord out_houseanim[72];
EvtWord in_dooranim[134];
EvtWord out_dooranim[121];
EvtWord house_in[216];
EvtWord house_out[196];
EvtWord evt_door_setup[56];
EvtWord door_dark[34];
EvtWord door_dark2[34];
EvtWord door_light[30];
EvtWord door_light2[30];
EvtWord evt_door_open[508];
#ifdef VERSION_G8ME01
EvtWord evt_door_open_bom_01[336];
#else
EvtWord evt_door_open_bom_01[338];
#endif
EvtWord evt_door_open_roll[225];
EvtWord evt_door_close[447];
EvtWord evt_door_close_roll[247];
EvtWord evt_door_open_quick[220];
EvtWord evt_door_entry[77];

EVT_BEGIN(door_open_se)
    CALL(evt_door_param, LW(0), 8, LW(1))
    CALL(evt_door_param, LW(0), 1, LW(2), LW(3), LW(4))
    CALL(evt_hit_get_position, LW(3), LW(10), LW(11), LW(12))
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(3)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(4)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_IRON1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(6)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_IRON2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(7)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_GRILLE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(8)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(9)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_TURN1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(10)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(11)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_STONE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(12)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(13)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(14)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_METAL1"), LW(10), LW(11), LW(12), 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(door_close_se)
    CALL(evt_door_param, LW(0), 8, LW(1))
    CALL(evt_door_param, LW(0), 1, LW(2), LW(3), LW(4))
    CALL(evt_hit_get_position, LW(3), LW(10), LW(11), LW(12))
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(3)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(4)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_IRON1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(6)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_IRON2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(7)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_GRILLE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(8)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(9)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_TURN1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(10)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_ELEC1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(11)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_STONE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(12)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_CASTLE1"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(13)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_CASTLE2"), LW(10), LW(11), LW(12), 0)
        CASE_EQUAL(14)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_METAL1"), LW(10), LW(11), LW(12), 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(door_down_se)
    CALL(evt_door_param, LW(0), 8, LW(1))
    CALL(evt_door_param, LW(0), 1, LW(2), LW(3), LW(4))
    CALL(evt_hit_get_position, LW(3), LW(10), LW(11), LW(12))
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_FALL1"), LW(10), LW(11), LW(12), 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(house_open_se)
    CALL(evt_door_param, LW(0), 9, LW(1))
    IF_BITS_SET(LW(1), 2)
        RETURN
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(2), LW(3), LW(4))
    CALL(evt_hit_get_position, LW(3), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_OPEN1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

USER_FUNC(snd_door_in) {
    psndSetFlag(0x100);
    return EVT_RETURN_DONE;
}

USER_FUNC(snd_door_out) {
    psndClearFlag(0x100);
    return EVT_RETURN_DONE;
}

USER_FUNC(door_entry) {
    EvtWord* args = event->args;
    HouseData* d = (HouseData*)evtGetValue(event, args[0]);
    Vec pos = {0.0f, 0.0f, 0.0f};
    Vec nrm = {0.0f, 0.0f, 0.0f};
    HouseWork* w = _mapAlloc(sizeof(HouseWork));

    w->data = d;
    switch (d->type) {
        case 0:
        case 1:
            hitObjGetPos(d->hit[1], &pos);
            hitObjGetNormal(d->hit[1], &nrm);
            w->out.x = pos.x + 40.0f * nrm.x - 10.0f * nrm.z;
            w->out.z = pos.z + 40.0f * nrm.z + 10.0f * nrm.x;
            hitObjGetPos(d->hit[3], &pos);
            hitObjGetNormal(d->hit[3], &nrm);
            w->in.x = pos.x + 30.0f * nrm.x;
            w->in.z = pos.z + 30.0f * nrm.z;
            break;
        case 2:
            hitObjGetPos(d->hit[1], &pos);
            hitObjGetNormal(d->hit[1], &nrm);
            w->out.x = pos.x + 40.0f * nrm.x + 10.0f * nrm.z;
            w->out.z = pos.z + 40.0f * nrm.z - 10.0f * nrm.x;
            hitObjGetPos(d->hit[3], &pos);
            hitObjGetNormal(d->hit[3], &nrm);
            w->in.x = pos.x + 30.0f * nrm.x;
            w->in.z = pos.z + 30.0f * nrm.z;
            break;
        case 3:
            hitObjGetPos(d->hit[1], &pos);
            hitObjGetNormal(d->hit[1], &nrm);
            w->out.x = pos.x + 40.0f * nrm.x;
            w->out.z = pos.z + 40.0f * nrm.z;
            hitObjGetPos(d->hit[3], &pos);
            hitObjGetNormal(d->hit[3], &nrm);
            w->in.x = pos.x + 40.0f * nrm.x;
            w->in.z = pos.z + 40.0f * nrm.z;
            break;
    }
    evtSetValue(event, args[1], (EvtWord)w);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_dark)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_dark2)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_light)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_blend_off, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_light2)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_blend_off, 1)
    RETURN
EVT_END()

EVT_BEGIN(door_up_se)
    RETURN
EVT_END()

EVT_BEGIN(house_close_se)
    CALL_CHILD(PTR(house_open_se))
    RETURN
EVT_END()

EVT_BEGIN(in_houseanim)
    WAIT_MS(500)
    CALL(animation, LW(10), LW(0), LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_INLINE_EVENT
            IF_STR_EQUAL(LW(0), STRING("S_mise_open"))
                CALL(door_group, LW(10), LW(0))
                CALL(evt_mapobj_get_position, LW(0), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_OPEN1"), LW(2), LW(3), LW(4), 0)
                LOOP(4)
                    WAIT_MS(166)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(2), LW(3), LW(4), 0)
                END_LOOP
                ELSE
                CALL(door_group, LW(10), LW(0))
                CALL(evt_mapobj_get_position, LW(0), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_OPEN1"), LW(2), LW(3), LW(4), 0)
            END_IF
        END_INLINE_EVENT
        CALL(evt_map_playanim, LW(0), 0, 0)
        CALL(evt_map_checkanim, LW(0), LW(4), LW(5))
        WAIT_MS(LW(5))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(out_houseanim)
    CALL(animation, LW(10), LW(0), LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        BEGIN_INLINE_EVENT
            IF_STR_EQUAL(LW(2), STRING("S_mise_close"))
                CALL(door_group, LW(10), LW(0))
                CALL(evt_mapobj_get_position, LW(0), LW(2), LW(3), LW(4))
                LOOP(4)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(2), LW(3), LW(4), 0)
                    WAIT_MS(166)
                END_LOOP
            END_IF
        END_INLINE_EVENT
        CALL(evt_map_playanim, LW(2), 0, 0)
        CALL(evt_map_checkanim, LW(2), LW(4), LW(5))
        WAIT_MS(LW(5))
        BEGIN_INLINE_EVENT
            CALL(door_group, LW(10), LW(0))
            CALL(evt_mapobj_get_position, LW(0), LW(2), LW(3), LW(4))
            CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_SHUT1"), LW(2), LW(3), LW(4), 0)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(in_dooranim)
    CALL(door_group, LW(10), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(9), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(2), LW(3), LW(4), 0)
        WAIT_MS(1500)
        CALL(evt_mapobj_get_position, LW(9), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(2), LW(3), LW(4), 0)
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, LW(9), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_FALL1"), LW(2), LW(3), LW(4), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 5, 0, 80, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 2, 80, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_sub_intpl_msec_init, 2, 0, 90, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(out_dooranim)
    CALL(door_group, LW(10), LW(9))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, LW(9), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(2), LW(3), LW(4), 0)
        WAIT_MS(1000)
        CALL(evt_mapobj_get_position, LW(9), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(2), LW(3), LW(4), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 5, 90, 0, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_msec_init, 5, 0, 80, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 5, 80, 0, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(house_in)
    PEEK_INT(LW(0), PTR(&DoorFLAG))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    POKE_INT(1, PTR(&DoorFLAG))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_disable, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(in_pos, LW(10), LW(0), LW(2))
        ADD(LW(1), 1)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(1), LW(2), 0, 150, 11)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(get_noclip_map, LW(10), LW(0), LW(1))
        IF_INT_EQUAL(LW(1), -1)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2)
        ADD(LW(0), 1)
    END_LOOP
    CALL(outside_group, LW(10), LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 2048)
    CALL(door_group, LW(10), LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 2048)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 4096)
    CALL(inside_group, LW(10), LW(0), LW(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 1)
    CALL(evt_hitobj_onoff, LW(1), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 2048)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 4096)
    CALL(N_evt_mario_party_door_halve_hitbox)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(out_pos, LW(10), LW(0), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(135.0f))
    CALL(evt_party_move_beside_mario, 0)
    CALL(evt_party_move_beside_mario, 1)
    SPAWN_EVENT_GET_ID(PTR(in_dooranim), LW(15))
    BEGIN_INLINE_EVENT
        CALL(npc_dark, LW(10))
        CALL_CHILD(PTR(in_houseanim))
        SPAWN_EVENT(PTR(evt_dark2))
    END_INLINE_EVENT
    WAIT_MS(300)
    SPAWN_EVENT(PTR(evt_dark))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(in_pos, LW(10), LW(0), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    LOOP(0)
        IS_EVENT_RUNNING(LW(15), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(snd_door_in)
    POKE_INT(0, PTR(&DoorFLAG))
    RETURN
EVT_END()

EVT_BEGIN(house_out)
    PEEK_INT(LW(0), PTR(&DoorFLAG))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    POKE_INT(1, PTR(&DoorFLAG))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_disable, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(out_pos, LW(10), LW(0), LW(2))
        ADD(LW(1), 1)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(1), LW(2), 0, 150, 11)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SPAWN_EVENT(PTR(evt_light2))
        CALL_CHILD(PTR(out_houseanim))
        SPAWN_EVENT(PTR(evt_light))
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(out_dooranim), LW(15))
    WAIT_MS(1000)
    CALL(N_evt_mario_party_door_halve_hitbox)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(out_pos, LW(10), LW(0), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(get_noclip_map, LW(10), LW(0), LW(1))
        IF_INT_EQUAL(LW(1), -1)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2)
        ADD(LW(0), 1)
    END_LOOP
    CALL(evt_cam3d_evt_xyz_off, 100, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    LOOP(0)
        IS_EVENT_RUNNING(LW(15), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    POKE_INT(0, PTR(&DoorFLAG))
    CALL(inside_group, LW(10), LW(0), LW(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 1)
    CALL(evt_hitobj_onoff, LW(1), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 2048)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 4096)
    CALL(outside_group, LW(10), LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 2048)
    CALL(door_group, LW(10), LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 2048)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 4096)
    CALL(npc_light, LW(10))
    CALL(snd_door_out)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_setup)
    CALL(door_entry, LW(0), LW(10))
    POKE_INT(0, PTR(&DoorFLAG))
    CALL(init_inside_group, LW(10), LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hitobj_onoff, LW(1), 1, 0)
    END_IF
    CALL(inout_hit, LW(10), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_run_case_evt, LW(0), 1, LW(1), 0, PTR(house_in), 0)
    CALL(evt_run_case_evt, LW(2), 1, LW(3), 0, PTR(house_out), 0)
    RETURN
EVT_END()

EVT_BEGIN(door_dark)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(door_dark2)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(door_light)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_blend_off, 0)
    RETURN
EVT_END()

EVT_BEGIN(door_light2)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 1, LW(0), LW(0), LW(0), 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_blend_off, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_open)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    POKE_INT(1, PTR(&DoorFLAG))
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_door_param, LW(0), 4, LW(1), LW(2), LW(3))
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        ADD(LW(5), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(1), LW(5), LW(3), 0, 150, 11)
    END_INLINE_EVENT
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(evt_door_param, LW(0), 9, LW(1))
    IF_BITS_SET(LW(1), 1)
        CALL(npc_hide_onoff, LW(0), 0)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    IF_BITS_SET(LW(15), 4)
        CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
        IF_INT_NOT_EQUAL(LW(9), 0)
            BEGIN_INLINE_EVENT
                WAIT_MS(500)
                SPAWN_EVENT(PTR(house_open_se))
                CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
                SET_VALUE(LW(6), 0)
                CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
                LABEL(2)
                CALL(evt_sub_intpl_msec_get_value)
                SET_VALUE(LW(5), LW(1))
                SET_VALUE(LW(1), 0)
                SET_VALUE(LF(0), 0)
                SPAWN_EVENT(LW(9))
                SET_VALUE(LW(6), 1)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(5), 1)
                    GOTO(2)
                END_IF
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(N_evt_mario_party_door_halve_hitbox)
    CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
    CALL(evt_mario_mov_pos2, LW(1), LW(3), FLOAT(135.0f))
    CALL(evt_party_move_beside_mario, 0)
    CALL(evt_party_move_beside_mario, 1)
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_dark))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 0, 180, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    CALL(evt_door_param, LW(0), 4, LW(1), LW(2), LW(3))
    CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 1)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
    END_IF
    IF_BITS_CLEAR(LW(15), 4)
        CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
        IF_INT_NOT_EQUAL(LW(9), 0)
            BEGIN_INLINE_EVENT
                SPAWN_EVENT(PTR(house_open_se))
                CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
                SET_VALUE(LW(6), 0)
                CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
                LABEL(2)
                CALL(evt_sub_intpl_msec_get_value)
                SET_VALUE(LW(5), LW(1))
                SET_VALUE(LW(1), 0)
                SET_VALUE(LF(0), 0)
                SPAWN_EVENT(LW(9))
                SET_VALUE(LW(6), 1)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(5), 1)
                    GOTO(2)
                END_IF
            END_INLINE_EVENT
        END_IF
    END_IF
    WAIT_MS(500)
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_dark2))
    END_IF
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            SPAWN_EVENT(PTR(door_down_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    POKE_INT(0, PTR(&DoorFLAG))
    WAIT_FRAMES(2)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(snd_door_in)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_open_bom_01)
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    SPAWN_EVENT(PTR(door_open_se))
#ifndef VERSION_G8ME01
    CALL(N_evt_mario_party_door_halve_hitbox)
#endif
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 0, 180, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    SET_VALUE(GF(0), 1)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SPAWN_EVENT(PTR(door_dark))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 1)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
    END_IF
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(house_open_se))
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            SET_VALUE(LW(6), 0)
            CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SPAWN_EVENT(LW(9))
            SET_VALUE(LW(6), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(2)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    SPAWN_EVENT(PTR(door_dark2))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            SPAWN_EVENT(PTR(door_down_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    CALL(snd_door_in)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_open_roll)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    SPAWN_EVENT(PTR(door_dark))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            SET_VALUE(LW(6), 0)
            CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SPAWN_EVENT(LW(9))
            SET_VALUE(LW(6), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(2)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    SPAWN_EVENT(PTR(door_dark2))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(snd_door_in)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_close)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    POKE_INT(1, PTR(&DoorFLAG))
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LF(0), 1)
        SPAWN_EVENT(LW(2))
    END_IF
    BEGIN_INLINE_EVENT
        IF_BITS_CLEAR(LW(15), 16)
            CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
            ELSE
            CALL(evt_door_param, LW(0), 11, LW(1), LW(2), LW(3))
        END_IF
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        ADD(LW(5), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(1), LW(5), LW(3), 0, 150, 11)
    END_INLINE_EVENT
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_light2))
    END_IF
    SPAWN_EVENT(PTR(door_up_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            SPAWN_EVENT(PTR(house_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            SET_VALUE(LW(6), 0)
            CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 3)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(9))
            SET_VALUE(LW(6), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(2)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_light))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 0, 180, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 2)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    IF_BITS_CLEAR(LW(15), 16)
        CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
        ELSE
        CALL(evt_door_param, LW(0), 11, LW(1), LW(2), LW(3))
    END_IF
    IF_BITS_CLEAR(LW(15), 8)
        CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
    END_IF
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 3)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
        WAIT_MS(500)
    END_IF
    CALL(evt_cam3d_evt_xyz_off, 100, 11)
    CALL(door_dark_flag, LW(0), 0)
    CALL(evt_door_param, LW(0), 9, LW(1))
    IF_BITS_SET(LW(1), 1)
        CALL(npc_hide_onoff, LW(0), 1)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        IF_BITS_CLEAR(LW(15), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_load_mapflag, LW(1))
    END_IF
    POKE_INT(0, PTR(&DoorFLAG))
    WAIT_FRAMES(2)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(snd_door_out)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_close_roll)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SPAWN_EVENT(LW(2))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        ADD(LW(5), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(1), LW(5), LW(3), 0, 150, 11)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(door_light2))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
            SET_VALUE(LW(6), 0)
            CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 3)
            SPAWN_EVENT(LW(9))
            SET_VALUE(LW(6), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(2)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    SPAWN_EVENT(PTR(door_light))
    CALL(evt_cam3d_evt_xyz_off, 100, 11)
    CALL(door_dark_flag, LW(0), 0)
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        IF_BITS_CLEAR(LW(15), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_load_mapflag, LW(1))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(snd_door_out)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_open_quick)
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 2)
    END_IF
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(10), LW(0))
        CALL(evt_door_param, LW(10), 4, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        SET_VALUE(LW(1), LW(5))
        CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_cam3d_evt_off, 0, 11)
    END_INLINE_EVENT
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 4096)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 0)
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(0), 180)
            SPAWN_EVENT(LW(9))
        END_INLINE_EVENT
    END_IF
    CALL(evt_map_set_blend, 1, 0, 0, 0, 0)
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(0), 180)
            SPAWN_EVENT(LW(8))
        END_INLINE_EVENT
    END_IF
    CALL(snd_door_in)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_entry)
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    CALL(evt_door_param, LW(0), 9, LW(1))
    IF_BITS_SET(LW(1), 1)
        CALL(npc_hide_onoff, LW(0), 1)
    END_IF
    RETURN
EVT_END()

USER_FUNC(inout_hit) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)w->data->hit[0]);
    evtSetValue(event, args[2], (EvtWord)w->data->hit[1]);
    evtSetValue(event, args[3], (EvtWord)w->data->hit[2]);
    evtSetValue(event, args[4], (EvtWord)w->data->hit[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(out_pos) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->out.x);
    evtSetValue(event, args[2], w->out.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(in_pos) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->in.x);
    evtSetValue(event, args[2], w->in.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(outside_group) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->data->outside);
    return EVT_RETURN_DONE;
}

USER_FUNC(door_group) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->data->door);
    return EVT_RETURN_DONE;
}

USER_FUNC(inside_group) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->data->inside[0]);
    evtSetValue(event, args[2], w->data->inside[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(init_inside_group) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->data->initInside[0]);
    evtSetValue(event, args[2], w->data->initInside[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(animation) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], w->data->anim[0]);
    evtSetValue(event, args[2], w->data->anim[1]);
    evtSetValue(event, args[3], w->data->anim[2]);
    evtSetValue(event, args[4], w->data->anim[3]);
    return EVT_RETURN_DONE;
}

#define NPC_LIGHT_BODY(FN, ICONFN)                                              \
    HouseWork* w = (HouseWork*)evtGetValue(event, *event->args);                \
    MarioWork* mario = marioGetPtr();                                           \
    PartyEntry* party = partyGetPtr(0);                                         \
    PartyEntry* follower = partyGetPtr(1);                                      \
    const char** list;                                                          \
    if (mario) {                                                                \
        FN(marioGetPtr()->paperAnimGroupId[0], 0x1800);                         \
        FN(marioGetPtr()->paperAnimGroupId[1], 0x1800);                         \
        FN(marioGetPtr()->paperAnimGroupId[2], 0x1800);                         \
    }                                                                           \
    if (party) {                                                                \
        FN(party->paperAnimGroup, 0x1800);                                      \
    }                                                                           \
    if (follower) {                                                             \
        FN(follower->paperAnimGroup, 0x1800);                                   \
    }                                                                           \
    list = w->data->npcs;                                                       \
    if (list) {                                                                 \
        for (; *list; list++) {                                                 \
            NpcEntry* npc = npcNameToPtr_NoAssert(*list);                       \
            if (npc) {                                                          \
                FN(npc->poseId, 0x1800);                                        \
            }                                                                   \
        }                                                                       \
    }                                                                           \
    list = w->data->mobjs;                                                      \
    if (list) {                                                                 \
        for (; *list; list++) {                                                 \
            MapObjectEntry* mobj = mobjNameToPtrNoAssert(*list);                \
            if (mobj) {                                                         \
                FN(mobj->poseId, 0x1800);                                       \
            }                                                                   \
        }                                                                       \
    }                                                                           \
    list = w->data->items;                                                      \
    if (list) {                                                                 \
        for (; *list; list++) {                                                 \
            ItemEntry* item = itemNameToPtr((char*)*list);                      \
            if (item) {                                                         \
                ICONFN(item->name, 0x40);                                       \
            }                                                                   \
        }                                                                       \
    }                                                                           \
    return EVT_RETURN_DONE;

USER_FUNC(npc_dark) {
    NPC_LIGHT_BODY(animPoseSetMaterialFlagOn, iconFlagOn)
}

USER_FUNC(npc_light) {
    NPC_LIGHT_BODY(animPoseSetMaterialFlagOff, iconFlagOff)
}

USER_FUNC(get_noclip_map) {
    EvtWord* args = event->args;
    HouseWork* w = (HouseWork*)evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);
    const char** list = w->data->noclip;

    if (list && list[idx]) {
        evtSetValue(event, args[2], (EvtWord)list[idx]);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[2], -1);
    return EVT_RETURN_DONE;
}

#define DOOR_POS_GET(HIT)                       \
    if (!mario) {                               \
        hitObjGetPos(HIT, &pos);                \
    } else {                                    \
        pos = marioGetPtr()->position;          \
    }                                           \
    hitObjGetNormal(HIT, &nrm);

void door_position_sub(s32 type, const char* map, const char* hitIn, const char* hitOut, Vec* in, Vec* out, BOOL mario) {
    Mtx inv;
    Mtx m;
    Vec pos;
    Vec nrm;
    Vec max;
    Vec min;
    Vec d;
    MapObject* obj = mapGetMapObj(map);
    f32 w = 10.0f;
    f32 a, b;
    f32 t;

    if (!mario) {
        a = 40.0f;
        b = 30.0f;
    } else {
        a = 50.0f;
        b = a;
    }
    if (obj) {
        PSMTXInverse(obj->transformMtx, inv);
        PSMTXConcat(obj->modelWorldMtx, inv, m);
        PSMTXMultVec(m, &obj->joints->bboxMin, &min);
        PSMTXMultVec(m, &obj->joints->bboxMax, &max);
        PSVECSubtract(&max, &min, &d);
        if (d.x) {
            t = d.x / (f32)cos((f32)atan(d.z / d.x));
            w = t / 2.0f;
        } else {
            w = d.z / 2.0f;
        }
        t = w;
        w = t;
    }
    switch (type) {
        case 0:
        case 1:
            if (hitOut) {
                DOOR_POS_GET(hitOut)
                out->x = pos.x + nrm.x * a - nrm.z * w;
                out->z = pos.z + nrm.z * a + nrm.x * w;
            } else {
                *out = marioGetPtr()->position;
            }
            if (hitIn) {
                DOOR_POS_GET(hitIn)
                in->x = pos.x + nrm.x * b;
                in->z = pos.z + nrm.z * b;
            } else {
                *in = marioGetPtr()->position;
            }
            break;
        case 2:
            if (hitOut) {
                DOOR_POS_GET(hitOut)
                out->x = pos.x + nrm.x * a + nrm.z * w;
                out->z = pos.z + nrm.z * a - nrm.x * w;
            } else {
                *out = marioGetPtr()->position;
            }
            if (hitIn) {
                DOOR_POS_GET(hitIn)
                in->x = pos.x + nrm.x * b;
                in->z = pos.z + nrm.z * b;
            } else {
                *in = marioGetPtr()->position;
            }
            break;
        case 3:
            if (hitOut) {
                DOOR_POS_GET(hitOut)
                out->x = pos.x + nrm.x * a;
                out->z = pos.z + nrm.z * a;
            } else {
                *out = marioGetPtr()->position;
            }
            if (hitIn) {
                DOOR_POS_GET(hitIn)
                in->x = pos.x + nrm.x * a;
                in->z = pos.z + nrm.z * a;
            } else {
                *in = marioGetPtr()->position;
            }
            break;
    }
}

#define DARK_SUB_BODY(FN, ICONFN)                                   \
    if (mario) {                                                    \
        FN(mario->paperAnimGroupId[0], 0x1800);                     \
        FN(mario->paperAnimGroupId[1], 0x1800);                     \
        FN(mario->paperAnimGroupId[2], 0x1800);                     \
    }                                                               \
    if (party) {                                                    \
        FN(party->paperAnimGroup, 0x1800);                          \
    }                                                               \
    if (follower) {                                                 \
        FN(follower->paperAnimGroup, 0x1800);                       \
    }                                                               \
    if (npcs) {                                                     \
        for (list = npcs; *list; list++) {                          \
            NpcEntry* npc = npcNameToPtr_NoAssert(*list);           \
            if (npc) {                                              \
                FN(npc->poseId, 0x1800);                            \
            }                                                       \
        }                                                           \
    }                                                               \
    if (mobjs) {                                                    \
        for (list = mobjs; *list; list++) {                         \
            MapObjectEntry* mobj = mobjNameToPtrNoAssert(*list);    \
            if (mobj) {                                             \
                FN(mobj->poseId, 0x1800);                           \
            }                                                       \
        }                                                           \
    }                                                               \
    if (items) {                                                    \
        for (list = items; *list; list++) {                         \
            ItemEntry* item = itemNameToPtr((char*)*list);          \
            if (item) {                                             \
                ICONFN(item->name, 0x40);                           \
            }                                                       \
        }                                                           \
    }

void door_dark_flag_sub(BOOL on, const char** npcs, const char** mobjs, const char** items) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    PartyEntry* follower = partyGetPtr(1);
    const char** list;

    if (on) {
        DARK_SUB_BODY(animPoseSetMaterialFlagOn, iconFlagOn)
    } else {
        DARK_SUB_BODY(animPoseSetMaterialFlagOff, iconFlagOff)
    }
}

USER_FUNC(door_dark_flag) {
    EvtWord* args = event->args;
    DoorParam* d = (DoorParam*)evtGetValue(event, args[0]);
    BOOL on = evtGetValue(event, args[1]);
    door_dark_flag_sub(on, d->npcs, d->mobjs, d->items);
    return EVT_RETURN_DONE;
}

#define DOOR_PARAM_POS(V, M)                                                        \
    door_position_sub(d->type, d->mapobj, d->hitIn, d->hitOut, &in, &out, M);       \
    evtSetValue(event, args[2], V.x);                                               \
    evtSetValue(event, args[3], V.y);                                               \
    evtSetValue(event, args[4], V.z);

USER_FUNC(evt_door_param) {
    EvtWord* args = event->args;
    DoorParam* d = (DoorParam*)evtGetValue(event, args[0]);
    s32 kind = evtGetValue(event, args[1]);
    Vec in = {0.0f, 0.0f, 0.0f};
    Vec out = {0.0f, 0.0f, 0.0f};

    switch (kind) {
        case 0:
            evtSetValue(event, args[2], d->type);
            break;
        case 1:
            evtSetValue(event, args[2], (EvtWord)d->mapobj);
            evtSetValue(event, args[3], (EvtWord)d->hitIn);
            evtSetValue(event, args[4], (EvtWord)d->hitOut);
            break;
        case 2:
            evtSetValue(event, args[2], (EvtWord)d->outside);
            evtSetValue(event, args[3], (EvtWord)d->outsideHit);
            break;
        case 3:
            evtSetValue(event, args[2], (EvtWord)d->inside);
            evtSetValue(event, args[3], (EvtWord)d->insideHit);
            break;
        case 4:
            DOOR_PARAM_POS(in, FALSE)
            break;
        case 5:
            DOOR_PARAM_POS(out, FALSE)
            break;
        case 6:
            evtSetValue(event, args[2], (EvtWord)d->evtIn);
            evtSetValue(event, args[3], (EvtWord)d->evtOut);
            evtSetValue(event, args[4], (EvtWord)d->evtWall);
            break;
        case 7:
            evtSetValue(event, args[2], (EvtWord)d->evtOpen);
            evtSetValue(event, args[3], (EvtWord)d->evtClose);
            break;
        case 8:
            evtSetValue(event, args[2], d->se);
            break;
        case 9:
            evtSetValue(event, args[2], d->flags);
            break;
        case 10:
            DOOR_PARAM_POS(in, TRUE)
            break;
        case 11:
            DOOR_PARAM_POS(out, TRUE)
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_door_save_mapflag) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    MapObject* obj = mapGetMapObj(name);
    if (!obj) {
        mapErrorEntry(0, name);
        return EVT_RETURN_DONE;
    }
    door_mapflag = obj->flags;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_door_load_mapflag) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    MapObject* obj = mapGetMapObj(name);
    if (!obj) {
        mapErrorEntry(0, name);
        return EVT_RETURN_DONE;
    }
    obj->flags = door_mapflag;
    return EVT_RETURN_DONE;
}

USER_FUNC(npc_hide_onoff) {
    EvtWord* args = event->args;
    DoorParam* d = (DoorParam*)evtGetValue(event, args[0]);
    BOOL on = evtGetValue(event, args[1]);
    const char** list = d->npcs;
    s32 i;

    for (i = 0; list[i]; i++) {
        NpcEntry* npc = npcNameToPtr_NoAssert(list[i]);
        if (npc) {
            if (on) {
                npc->flags |= 0x800;
            } else {
                npc->flags &= ~0x800;
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_door_data_copy) {
    EvtWord* args = event->args;
    DoorParam* src = (DoorParam*)evtGetValue(event, args[0]);
    DoorParam* dst = _mapAlloc(sizeof(DoorParam));
    *dst = *src;
    evtSetValue(event, args[1], (EvtWord)dst);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_door_set_param) {
    EvtWord* args = event->args;
    DoorParam* d = (DoorParam*)evtGetValue(event, args[0]);
    s32 kind = evtGetValue(event, args[1]);
    EvtWord val = evtGetValue(event, args[2]);
    switch (kind) {
        case 0:
            d->npcs = (const char**)val;
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_door_end_wait) {
    return DoorFLAG ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}
