#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "libc_decls.h"
#include "evt/evt_bero.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "stdio.h"
#include "string.h"
#include "evt/evt_npc.h"
#include "driver/npcdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "driver/mapdrv.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "driver/imgdrv.h"
#include "data/mapdata.h"
#include "driver/swdrv.h"
#include "npc_event.h"
#include "rsh.h"

typedef struct RshRndNpc { const char* name; const char* model; EvtWord* talkEvt; } RshRndNpc;
typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

BOOL img_wait(void);
RshRndNpc* _get_rnd_npc_data(s32 room, s32 group, s32 index);
void rsh_window_tev(MapTevCallbackInfo* info);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord simi_check[57];
EvtWord simi_offscreen_check[41];
EvtWord kami_check[85];
EvtWord kami_offscreen_check[41];
EvtWord evt_rsh_door_init[23];
EvtWord evt_rsh_door_entry[46];
EvtWord evt_rsh_door_open[548];
EvtWord evt_rsh_door_close[404];
EvtWord evt_rsh_window[30];
RshRndNpc rsh_01_rnd_npc_1[5];
RshRndNpc rsh_01_rnd_npc_2[3];
RshRndNpc rsh_01_rnd_npc_3[2];
RshRndNpc rsh_03_rnd_npc_1[13];
RshRndNpc rsh_03_rnd_npc_2[7];
RshRndNpc rsh_04_rnd_npc_1[2];
RshRndNpc rsh_04_rnd_npc_2[2];
RshRndNpc rsh_04_rnd_npc_3[2];
RshRndNpc rsh_04_rnd_npc_4[4];
EvtWord evt_rsh_rnd_npc_2_1_talk[8];
EvtWord evt_rsh_rnd_npc_2_2_talk[16];
EvtWord evt_rsh_rnd_npc_2_3_talk[16];
EvtWord evt_rsh_rnd_npc_2_4_talk[8];
EvtWord evt_rsh_rnd_npc_2_5_talk[8];
EvtWord evt_rsh_rnd_npc_2_6_talk[8];
EvtWord evt_rsh_rnd_npc_2_7_talk[8];
EvtWord evt_rsh_rnd_npc_2_8_talk[8];
EvtWord evt_rsh_rnd_npc_2_9_talk[8];
EvtWord evt_rsh_rnd_npc_2_10_talk[8];
EvtWord evt_rsh_rnd_npc_3_1_talk[8];
EvtWord evt_rsh_rnd_npc_3_2_talk[8];
EvtWord evt_rsh_rnd_npc_3_3_talk[8];
EvtWord evt_rsh_rnd_npc_3_4_talk[8];
EvtWord evt_rsh_rnd_npc_3_5_talk[25];
EvtWord evt_rsh_rnd_npc_3_6_talk[8];
EvtWord evt_rsh_rnd_npc_3_7_talk[8];
EvtWord evt_rsh_rnd_npc_3_8_talk[8];
EvtWord evt_rsh_rnd_npc_3_9_talk[8];
EvtWord evt_rsh_rnd_npc_3_10_talk[8];
EvtWord evt_rsh_rnd_npc_4_1_talk[8];
EvtWord evt_rsh_rnd_npc_4_2_talk[8];
EvtWord evt_rsh_rnd_npc_4_3_talk[16];
EvtWord evt_rsh_rnd_npc_4_4_talk[8];
EvtWord evt_rsh_rnd_npc_4_5_talk[8];
EvtWord evt_rsh_rnd_npc_4_6_talk[8];
EvtWord evt_rsh_rnd_npc_4_7_talk[8];
EvtWord evt_rsh_rnd_npc_4_8_talk[39];
EvtWord evt_rsh_rnd_npc_4_9_talk[8];
EvtWord evt_rsh_rnd_npc_4_10_talk[8];
EvtWord evt_rsh_rnd_npc_5_1_talk[8];
EvtWord evt_rsh_rnd_npc_5_2_talk[8];
EvtWord evt_rsh_rnd_npc_5_3_talk[47];
EvtWord evt_rsh_rnd_npc_5_4_talk[8];
EvtWord evt_rsh_rnd_npc_5_5_talk[8];
EvtWord evt_rsh_rnd_npc_5_6_talk[8];
EvtWord evt_rsh_rnd_npc_5_7_talk[8];
EvtWord evt_rsh_rnd_npc_5_8_talk[8];
EvtWord evt_rsh_rnd_npc_5_9_talk[8];
EvtWord evt_rsh_rnd_npc_5_10_talk[8];
const char* rsh_01_rnd_npc_1_list[5];
const char* rsh_01_rnd_npc_2_list[3];
const char* rsh_01_rnd_npc_3_list[2];
const char* rsh_03_rnd_npc_1_list[13];
const char* rsh_03_rnd_npc_2_list[7];
const char* rsh_04_rnd_npc_1_list[2];
const char* rsh_04_rnd_npc_2_list[2];
const char* rsh_04_rnd_npc_3_list[2];
const char* rsh_04_rnd_npc_4_list[4];
EvtWord debug_print_rnd_npc[61];
EvtWord evt_rsh_rnd_npc_init[94];
EvtWord evt_rsh_01_rnd_npc_entry[137];
NpcSetupInfo rsh_03_rnd_npc_2_ent[2];
EvtWord evt_rsh_03_rnd_npc_entry[95];
NpcSetupInfo rsh_04_rnd_npc_3_ent[2];
EvtWord evt_rsh_04_rnd_npc_entry[185];

EVT_BEGIN(simi_check)
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_03_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_20_END_OF_DAY_1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_yuka"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_yuka_simi"), MAP_OBJ_FLAG_HIDDEN)
            CASE_DEFAULT
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_yuka"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_yuka_simi"), MAP_OBJ_FLAG_HIDDEN)
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(simi_offscreen_check)
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_03_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_20_END_OF_DAY_1)
            CALL(evt_mapobj_clear_offscreen, 0, STRING("s_yuka"))
            CASE_DEFAULT
            CALL(evt_mapobj_clear_offscreen, 0, STRING("s_yuka_simi"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kami_check)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_01_a"))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_22_DOCUMENT)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG6_24_A_VITAL_CLUE, STG6_24_02_ANOTHER_CLUE)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
            CASE_DEFAULT
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kami_offscreen_check)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_01_a"))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_22_DOCUMENT)
            CALL(evt_mapobj_clear_offscreen, 0, STRING("yuka_3"))
            ELSE
            CALL(evt_mapobj_clear_offscreen, 0, STRING("yuka_3_kami"))
        END_IF
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG6_24_A_VITAL_CLUE, STG6_24_02_ANOTHER_CLUE)
            CALL(evt_mapobj_clear_offscreen, 0, STRING("yuka_5"))
            CASE_DEFAULT
            CALL(evt_mapobj_clear_offscreen, 0, STRING("yuka_5_kami"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_door_init)
    CALL(evt_paper_entry, STRING("OFF_d_meku"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_meku"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_door_entry)
    SET_INT_READ_ADDR(LW(0))
    READ_INT(LW(1))
    READ_INT(LW(2))
    READ_INT(LW(3))
    READ_INT(LW(4))
    READ_INT(LW(5))
    READ_INT(LW(6))
    READ_INT(LW(7))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(4), 0, PTR(evt_rsh_door_open), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(6), 0, PTR(evt_rsh_door_close), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_door_open)
    SET_INT_READ_ADDR(LW(0))
    READ_INT(LW(1))
    READ_INT(LW(2))
    READ_INT(LW(3))
    READ_INT(LW(4))
    READ_INT(LW(5))
    READ_INT(LW(6))
    READ_INT(LW(7))
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(4), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(6), 8388608)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(img_wait)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
    CALL_CHILD(PTR(kami_check))
    CALL(evt_hit_get_position, LW(4), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_map_playanim, LW(5), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, LW(5), LW(10), LW(11))
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(3), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hit_get_position, LW(4), LW(10), LW(11), LW(12))
    SUBTRACT(LW(12), 50)
    CALL(evt_mario_party_door_move, LW(10), LW(12), FLOAT(80.0f))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_free_capture, STRING("capture_img"), 2)
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, 0, 0, 0, 0)
    CALL(evt_mapobj_set_offscreen, 1, STRING("rouka"), STRING("offscreen"))
    CALL_CHILD(PTR(simi_offscreen_check))
    CALL(evt_mapobj_set_offscreen, 1, STRING("w_tobira"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("e_tobira"), STRING("offscreen"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_check, STRING("車掌"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_offscreen, STRING("車掌"), STRING("offscreen"))
        CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_NO_DRAW)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF6_ROOM_IN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_clear_offscreen, 1, STRING("rouka"))
        CALL(evt_mapobj_clear_offscreen, 1, STRING("w_tobira"))
        CALL(evt_mapobj_clear_offscreen, 1, STRING("e_tobira"))
        CALL(evt_npc_check, STRING("車掌"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_set_offscreen, STRING("車掌"), 0)
        END_IF
        CALL(evt_img_wait_animend, STRING("capture_img"))
        CALL(evt_img_onoff, STRING("capture_img"), 0)
    END_INLINE_EVENT
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_hitobj_attr_onoff, 0, 0, LW(4), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 0, LW(6), 8388608)
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_b"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_c"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        IF_STR_EQUAL(LW(1), STRING("heya_5"))
            SWITCH(GSW_STORY_PROGRESS)
                CASE_EQUAL(STG6_19_GIVE_BLANKET)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_img_wait_animend, STRING("capture_img"))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mario_get_party, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_0"), 0, STRING("party"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_1"), 0, STRING("party"))
                    CASE_EQUAL(4)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_2"), 0, STRING("party"))
                    CASE_EQUAL(5)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_3"), 0, STRING("party"))
                    CASE_EQUAL(6)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_4"), 0, STRING("party"))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_5"), 0, STRING("party"))
                    CASE_EQUAL(7)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_146_6"), 0, STRING("party"))
                END_SWITCH
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
                CASE_BETWEEN(STG6_24_A_VITAL_CLUE, STG6_24_02_ANOTHER_CLUE)
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
                CASE_EQUAL(STG6_43_CONDUCTOR_APPEARS)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_img_wait_animend, STRING("capture_img"))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mario_get_party, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_0"), 0, STRING("party"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_1"), 0, STRING("party"))
                    CASE_EQUAL(4)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_2"), 0, STRING("party"))
                    CASE_EQUAL(5)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_3"), 0, STRING("party"))
                    CASE_EQUAL(6)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_4"), 0, STRING("party"))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_5"), 0, STRING("party"))
                    CASE_EQUAL(7)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_224_6"), 0, STRING("party"))
                END_SWITCH
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
                CASE_DEFAULT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_img_wait_animend, STRING("capture_img"))
                CALL(evt_mario_key_onoff, 1)
            END_SWITCH
        END_IF
    END_IF
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_b"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_02_c"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        IF_STR_EQUAL(LW(1), STRING("heya_4"))
            IF_INT_EQUAL(GSWF_RSH_GOURMET_NOISE_TALK_DONE, 1)
                SET_VALUE(GSWF_RSH_NOISY_ROOM_CHECKED, 1)
            END_IF
            IF_INT_EQUAL(GSWF_RSH_GHOST_APPEARED, 1)
                CALL(evt_snd_bgmoff, PSND_SILENT)
            END_IF
            SET_VALUE(GSWF_RSH_02_IN_ROOM_4, 1)
        END_IF
    END_IF
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(10))
    IF_STR_EQUAL(LW(10), STRING("rsh_04_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_04_b"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(10), STRING("rsh_04_c"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        IF_STR_EQUAL(LW(1), STRING("heya_5"))
            SET_VALUE(GSWF_RSH_04_IN_ROOM_5, 1)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("heya_3"))
            SET_VALUE(GSWF_RSH_04_IN_ROOM_3, 1)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_door_close)
    SET_INT_READ_ADDR(LW(0))
    READ_INT(LW(1))
    READ_INT(LW(2))
    READ_INT(LW(3))
    READ_INT(LW(4))
    READ_INT(LW(5))
    READ_INT(LW(6))
    READ_INT(LW(7))
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(4), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(6), 8388608)
    CALL(img_wait)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_06_PENNINGTONS_ASSISTANT)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
                WAIT_MS(1000)
                SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
                CALL(evt_bero_mapchange, STRING("rsh_04_b"), STRING("e_bero"))
            END_IF
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_12_GIVE_AUTOGRAPH)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_04_b"))
                WAIT_MS(1000)
                SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
                CALL(evt_bero_mapchange, STRING("rsh_04_c"), STRING("e_bero"))
            END_IF
        END_IF
    END_INLINE_EVENT
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_15_GHOST_REQUEST)
        CALL(evt_sub_get_mapname, LW(10))
        IF_STR_EQUAL(LW(10), STRING("rsh_02_c"))
            IF_INT_EQUAL(GSWF_RSH_GHOST_DIARY_WARNING_SEEN, 0)
                SET_VALUE(GSWF_RSH_GHOST_DIARY_WARNING_SEEN, 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_123"), 0, STRING("ゆうれい"))
            END_IF
        END_IF
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_sub_get_mapname, LW(10))
        IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
            IF_STR_EQUAL(LW(1), STRING("heya_5"))
                SWITCH(GSW_STORY_PROGRESS)
                    CASE_BETWEEN(STG6_24_A_VITAL_CLUE, STG6_24_02_ANOTHER_CLUE)
                    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                END_SWITCH
            END_IF
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_get_mapname, LW(10))
        SET_VALUE(LF(0), 0)
        IF_STR_EQUAL(LW(10), STRING("rsh_02_a"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(10), STRING("rsh_02_b"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(10), STRING("rsh_02_c"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            IF_STR_EQUAL(LW(1), STRING("heya_4"))
                IF_INT_EQUAL(GSWF_RSH_GHOST_APPEARED, 1)
                    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                END_IF
            END_IF
        END_IF
    END_INLINE_EVENT
    SET_VALUE(GSWF_RSH_04_IN_ROOM_5, 0)
    SET_VALUE(GSWF_RSH_04_IN_ROOM_3, 0)
    SET_VALUE(GSWF_RSH_02_IN_ROOM_4, 0)
    CALL(evt_map_playanim, LW(5), MAP_ANIM_PLAY_AT_END, 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_free_capture, STRING("capture_img"), 2)
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, 0, 0, 608, 480)
    CALL(evt_mapobj_set_offscreen, 1, STRING("rouka"), STRING("offscreen"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL_CHILD(PTR(kami_offscreen_check))
    CALL_CHILD(PTR(simi_offscreen_check))
    CALL(evt_npc_check, STRING("車掌"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_offscreen, STRING("車掌"), STRING("offscreen"))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF6_ROOM_OUT1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_2"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_hit_get_position, LW(6), LW(13), LW(14), LW(15))
        CALL(evt_mario_mov_pos, LW(13), LW(12), 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_clear_offscreen, 1, STRING("rouka"))
        CALL(evt_mapobj_clear_offscreen, 1, LW(1))
        CALL(evt_mapobj_clear_offscreen, 1, LW(2))
        CALL(evt_npc_check, STRING("車掌"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_flag_onoff, 0, STRING("車掌"), NPC_FLAG_NO_DRAW)
        END_IF
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_set_offscreen, STRING("車掌"), 0)
        END_IF
    END_INLINE_EVENT
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
    CALL_CHILD(PTR(simi_check))
    BEGIN_INLINE_EVENT
        CALL(evt_hit_get_position, LW(6), LW(10), LW(11), LW(12))
        ADD(LW(12), 50)
        CALL(evt_mario_party_door_move, LW(10), LW(12), FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_hit_get_position, LW(4), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_2"), LW(10), LW(11), LW(12), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(3), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, LW(7), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, LW(7), LW(10), LW(11))
    WAIT_MS(LW(11))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_attr_onoff, 0, 0, LW(4), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 0, LW(6), 8388608)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_window)
    CALL(evt_map_set_tevcallback, 1, PTR(rsh_window_tev))
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT(LW(1))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_flag_onoff, 0, 1, LW(1), MAP_OBJ_TEV_CALLBACK(1))
        CALL(evt_mapobj_flag_onoff, 0, 0, LW(1), MAP_OBJ_FLAG_DISPLAY_LIST)
    END_LOOP
    RETURN
EVT_END()

static const s32 rsh_texmtx_tbl[] = {
    GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4, GX_TEXMTX5, GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9,
};

RshRndNpc rsh_01_rnd_npc_1[] = {
    {"c_kinopiko_v", /*"キノピオ女紫"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x8E\x87", evt_rsh_rnd_npc_2_1_talk},
    {"c_kinopio_b", /*"キノピオ青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x90\xC2", evt_rsh_rnd_npc_2_2_talk},
    {"c_kinopio_g", /*"キノピオ緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x97\xCE", evt_rsh_rnd_npc_2_3_talk},
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_2_4_talk},
    {"c_kuribo_n", /*"NPCクリボー"*/"\x4E\x50\x43\x83\x4E\x83\x8A\x83\x7B\x81\x5B", evt_rsh_rnd_npc_2_5_talk},
};

RshRndNpc rsh_01_rnd_npc_2[] = {
    {"c_kinopiko_p", /*"キノピオ女桃"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x93\x8D", evt_rsh_rnd_npc_2_6_talk},
    {"c_kinopio_b", /*"キノピオ青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x90\xC2", evt_rsh_rnd_npc_2_8_talk},
    {"c_kinopiko", /*"キノピオ女"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97", evt_rsh_rnd_npc_2_9_talk},
};

RshRndNpc rsh_01_rnd_npc_3[] = {
    {"c_kokinopio_y", /*"キノピオ子黄緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x89\xA9\x97\xCE", evt_rsh_rnd_npc_2_7_talk},
    {"c_kokinopio_b", /*"キノピオ子青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x90\xC2", evt_rsh_rnd_npc_2_10_talk},
};

RshRndNpc rsh_03_rnd_npc_1[] = {
    {"c_kinopio", /*"キノピオ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49", evt_rsh_rnd_npc_3_1_talk},
    {"c_kinopiko_p", /*"キノピオ女桃"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x93\x8D", evt_rsh_rnd_npc_3_2_talk},
    {"c_kinopiko_v", /*"キノピオ女紫"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x8E\x87", evt_rsh_rnd_npc_3_3_talk},
    {"c_kinopio_b", /*"キノピオ青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x90\xC2", evt_rsh_rnd_npc_3_4_talk},
    {"c_kinopiko_p", /*"キノピオ女桃"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x93\x8D", evt_rsh_rnd_npc_3_5_talk},
    {"c_kinopiko_v", /*"キノピオ女紫"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x8E\x87", evt_rsh_rnd_npc_3_7_talk},
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_3_8_talk},
    {"c_yoshi", /*"ヨッシー大"*/"\x83\x88\x83\x62\x83\x56\x81\x5B\x91\xE5", evt_rsh_rnd_npc_3_9_talk},
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_4_1_talk},
    {"c_yoshi", /*"ヨッシー大"*/"\x83\x88\x83\x62\x83\x56\x81\x5B\x91\xE5", evt_rsh_rnd_npc_4_2_talk},
    {"c_jugem_n", /*"ＮＰＣジュゲム"*/"\x82\x6D\x82\x6F\x82\x62\x83\x57\x83\x85\x83\x51\x83\x80", evt_rsh_rnd_npc_4_3_talk},
    {"c_kinopio_b", /*"キノピオ青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x90\xC2", evt_rsh_rnd_npc_4_4_talk},
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_4_5_talk},
};

RshRndNpc rsh_03_rnd_npc_2[] = {
    {"c_kokinopio_g", /*"キノピオ子緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x97\xCE", evt_rsh_rnd_npc_3_6_talk},
    {"c_kokinopio_y", /*"キノピオ子黄緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x89\xA9\x97\xCE", evt_rsh_rnd_npc_3_10_talk},
    {"c_kinopio", /*"キノピオ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49", evt_rsh_rnd_npc_4_6_talk},
    {"c_kokinopio_g", /*"キノピオ子緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x97\xCE", evt_rsh_rnd_npc_4_7_talk},
    {"c_kuribo_n", /*"NPCクリボー"*/"\x4E\x50\x43\x83\x4E\x83\x8A\x83\x7B\x81\x5B", evt_rsh_rnd_npc_4_8_talk},
    {"c_kinopio_g", /*"キノピオ緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x97\xCE", evt_rsh_rnd_npc_4_9_talk},
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_4_10_talk},
};

RshRndNpc rsh_04_rnd_npc_1[] = {
    {"c_kinopio_g", /*"キノピオ緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x97\xCE", evt_rsh_rnd_npc_5_2_talk},
    {"c_kinopio_b", /*"キノピオ青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x90\xC2", evt_rsh_rnd_npc_5_10_talk},
};

RshRndNpc rsh_04_rnd_npc_2[] = {
    {"c_nokonoko", /*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", evt_rsh_rnd_npc_5_8_talk},
    {"c_kinopio", /*"キノピオ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49", evt_rsh_rnd_npc_5_9_talk},
};

RshRndNpc rsh_04_rnd_npc_3[] = {
    {"c_kokinopio_g", /*"キノピオ子緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x97\xCE", evt_rsh_rnd_npc_5_5_talk},
    {"c_kokinopio", /*"キノピオ子"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71", evt_rsh_rnd_npc_5_6_talk},
};

RshRndNpc rsh_04_rnd_npc_4[] = {
    {"c_kokinopio_b", /*"キノピオ子青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x90\xC2", evt_rsh_rnd_npc_5_1_talk},
    {"c_kokinopio_y", /*"キノピオ子黄緑"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x89\xA9\x97\xCE", evt_rsh_rnd_npc_5_3_talk},
    {"c_kokinopio_b", /*"キノピオ子青"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x90\xC2", evt_rsh_rnd_npc_5_4_talk},
    {"c_kinopiko", /*"キノピオ女"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97", evt_rsh_rnd_npc_5_7_talk},
};

EVT_BEGIN(evt_rsh_rnd_npc_2_1_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_15"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_2_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_16"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_c_16_sel"))
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_16_01"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_3_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_17"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_c_17_sel"))
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_17_01"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_4_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_18"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_5_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_19"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_6_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_20"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_7_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_21"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_8_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_22"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_9_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_23"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_2_10_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_24"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_1_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_25"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_2_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_26"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_3_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_27"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_4_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_28"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_5_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_29"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_c_29_sel"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_29_01"))
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_29_02"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_6_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_30"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_7_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_31"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_8_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_32"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_9_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_33"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_3_10_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_34"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_1_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_35"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_2_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_36"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_3_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_37"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_c_37_sel"))
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_37_01"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_4_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_38"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_5_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_39"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_6_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_40"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_7_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_41"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_8_talk)
    IF_INT_EQUAL(GSWF_RSH_TRAVELER_SECRETS_HEARD, 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_42"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg6_rsh_c_42_sel"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_42_01"))
            SET_VALUE(GSWF_RSH_TRAVELER_SECRETS_HEARD, 1)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_42_02"))
        END_IF
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_42_03"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_9_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_43"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_4_10_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_44"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_1_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_45"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_2_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_46"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_3_talk)
    IF_INT_EQUAL(GSWF_RSH_QUIZ_PASSENGER_DONE, 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_47"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg6_rsh_c_47_sel"))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_47_01"))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_47_02"))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_c_47_03"))
        END_SWITCH
        SET_VALUE(GSWF_RSH_QUIZ_PASSENGER_DONE, 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_47_04"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_4_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_48"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_5_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_49"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_6_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_50"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_7_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_51"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_8_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_52"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_9_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_53"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_rnd_npc_5_10_talk)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_54"), 0, STRING("me"))
    RETURN
EVT_END()

const char* rsh_01_rnd_npc_1_list[] = {
    /*"２−１"*/"\x82\x51\x81\x7C\x82\x50",
    /*"２−２"*/"\x82\x51\x81\x7C\x82\x51",
    /*"２−３"*/"\x82\x51\x81\x7C\x82\x52",
    /*"２−４"*/"\x82\x51\x81\x7C\x82\x53",
    /*"２−５"*/"\x82\x51\x81\x7C\x82\x54",
};

const char* rsh_01_rnd_npc_2_list[] = {
    /*"２−６"*/"\x82\x51\x81\x7C\x82\x55",
    /*"２−８"*/"\x82\x51\x81\x7C\x82\x57",
    /*"２−９"*/"\x82\x51\x81\x7C\x82\x58",
};

const char* rsh_01_rnd_npc_3_list[] = {
    /*"２−７"*/"\x82\x51\x81\x7C\x82\x56",
    /*"２−１０"*/"\x82\x51\x81\x7C\x82\x50\x82\x4F",
};

const char* rsh_03_rnd_npc_1_list[] = {
    /*"３−１"*/"\x82\x52\x81\x7C\x82\x50",
    /*"３−２"*/"\x82\x52\x81\x7C\x82\x51",
    /*"３−３"*/"\x82\x52\x81\x7C\x82\x52",
    /*"３−４"*/"\x82\x52\x81\x7C\x82\x53",
    /*"３−５"*/"\x82\x52\x81\x7C\x82\x54",
    /*"３−７"*/"\x82\x52\x81\x7C\x82\x56",
    /*"３−８"*/"\x82\x52\x81\x7C\x82\x57",
    /*"３−９"*/"\x82\x52\x81\x7C\x82\x58",
    /*"４−１"*/"\x82\x53\x81\x7C\x82\x50",
    /*"４−２"*/"\x82\x53\x81\x7C\x82\x51",
    /*"４−３"*/"\x82\x53\x81\x7C\x82\x52",
    /*"４−４"*/"\x82\x53\x81\x7C\x82\x53",
    /*"４−５"*/"\x82\x53\x81\x7C\x82\x54",
};

const char* rsh_03_rnd_npc_2_list[] = {
    /*"３−６"*/"\x82\x52\x81\x7C\x82\x55",
    /*"３−１０"*/"\x82\x52\x81\x7C\x82\x50\x82\x4F",
    /*"４−６"*/"\x82\x53\x81\x7C\x82\x55",
    /*"４−７"*/"\x82\x53\x81\x7C\x82\x56",
    /*"４−８"*/"\x82\x53\x81\x7C\x82\x57",
    /*"４−９"*/"\x82\x53\x81\x7C\x82\x58",
    /*"４−１０"*/"\x82\x53\x81\x7C\x82\x50\x82\x4F",
};

const char* rsh_04_rnd_npc_1_list[] = {
    /*"５−２"*/"\x82\x54\x81\x7C\x82\x51",
    /*"５−１０"*/"\x82\x54\x81\x7C\x82\x50\x82\x4F",
};

const char* rsh_04_rnd_npc_2_list[] = {
    /*"５−８"*/"\x82\x54\x81\x7C\x82\x57",
    /*"５−９"*/"\x82\x54\x81\x7C\x82\x58",
};

const char* rsh_04_rnd_npc_3_list[] = {
    /*"５−５"*/"\x82\x54\x81\x7C\x82\x54",
    /*"５−６"*/"\x82\x54\x81\x7C\x82\x55",
};

const char* rsh_04_rnd_npc_4_list[] = {
    /*"５−１"*/"\x82\x54\x81\x7C\x82\x50",
    /*"５−３"*/"\x82\x54\x81\x7C\x82\x52",
    /*"５−４"*/"\x82\x54\x81\x7C\x82\x53",
    /*"５−７"*/"\x82\x54\x81\x7C\x82\x56",
};

EVT_BEGIN(debug_print_rnd_npc)
    SET_VALUE(LW(0), 0)
    LOOP(9)
        SWITCH(LW(0))
            CASE_LESS(3)
            SET_VALUE(LW(1), 0)
            SET_VALUE(LW(2), LW(0))
            CASE_LESS(5)
            SET_VALUE(LW(1), 1)
            SET_VALUE(LW(2), LW(0))
            SUBTRACT(LW(2), 3)
            CASE_DEFAULT
            SET_VALUE(LW(1), 2)
            SET_VALUE(LW(2), LW(0))
            SUBTRACT(LW(2), 5)
        END_SWITCH
        CALL(evt_rsh_get_rnd_npc, LW(1), LW(2), LW(3))
        CALL(evt_rsh_get_rnd_npc_data, LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(debug_print_func, LW(1), LW(2), LW(3), LW(4))
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

typedef struct RshDebugStr {
    char str[256];
} RshDebugStr;

static const RshDebugStr rsh_debug_str_init = {0};

EVT_BEGIN(evt_rsh_rnd_npc_init)
    SET_VALUE(GSW_RSH_01_RANDOM_NPCS, 0)
    SET_VALUE(GSW_RSH_03_RANDOM_NPCS, 0)
    SET_VALUE(GSW_RSH_04_RANDOM_NPCS, 0)
    CALL(evt_sub_random, 4, LW(0))
    CALL(evt_rsh_set_rnd_npc, 0, 0, LW(0))
    CALL(evt_sub_random, 2, LW(0))
    CALL(evt_rsh_set_rnd_npc, 0, 1, LW(0))
    CALL(evt_sub_random, 1, LW(0))
    CALL(evt_rsh_set_rnd_npc, 0, 2, LW(0))
    CALL(evt_sub_random, 12, LW(0))
    CALL(evt_rsh_set_rnd_npc, 1, 0, LW(0))
    CALL(evt_sub_random, 6, LW(0))
    CALL(evt_rsh_set_rnd_npc, 1, 1, LW(0))
    CALL(evt_sub_random, 1, LW(0))
    CALL(evt_rsh_set_rnd_npc, 2, 0, LW(0))
    CALL(evt_sub_random, 1, LW(0))
    CALL(evt_rsh_set_rnd_npc, 2, 1, LW(0))
    CALL(evt_sub_random, 1, LW(0))
    CALL(evt_rsh_set_rnd_npc, 2, 2, LW(0))
    CALL(evt_sub_random, 3, LW(0))
    CALL(evt_rsh_set_rnd_npc, 2, 3, LW(0))
    CALL_CHILD(PTR(debug_print_rnd_npc))
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_01_rnd_npc_entry)
    CALL(evt_rsh_get_rnd_npc, 0, 0, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 0, 0, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 0, 0, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客１"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客１"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客１"), -275, 0, -225)
    CALL(evt_npc_set_ry, STRING("乗客１"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客１"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客１"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 0, 1, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 0, 1, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 0, 1, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客２"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客２"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客２"), -45, 0, -175)
    CALL(evt_npc_set_ry, STRING("乗客２"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客２"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客２"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 0, 2, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 0, 2, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 0, 2, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客３"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客３"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客３"), 85, 16, -255)
    CALL(evt_npc_set_ry, STRING("乗客３"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客３"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客３"), LW(3))
    RETURN
EVT_END()

NpcSetupInfo rsh_03_rnd_npc_2_ent[] = {
    {
        /*"乗客２"*/"\x8F\xE6\x8B\x71\x82\x51", // name
        0, 0, // flags..reactionFlags
        npc_urouro_init_event, // initEvent
        npc_urouro_move_event, // regularEvent
        NULL, NULL, NULL, NULL, // talkEvent..lostEvent
        npc_urouro_return_event, // returnEvent
        NULL, // blowEvent
        1, // territoryType
        {-360.0f, 0.0f, -10.0f}, // territoryBase
        {50.0f, 0.0f, 0.0f}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, // searchRange..homingAngle
        -1, // battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_rsh_03_rnd_npc_entry)
    CALL(evt_rsh_get_rnd_npc, 1, 0, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 1, 0, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 1, 0, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客１"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客１"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客１"), 460, 0, -60)
    CALL(evt_npc_set_ry, STRING("乗客１"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客１"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客１"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 1, 1, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 1, 1, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 1, 1, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客２"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客２"), LW(2))
    CALL(evt_npc_setup, PTR(rsh_03_rnd_npc_2_ent))
    CALL(evt_npc_set_position, STRING("乗客２"), -360, 0, -10)
    CALL(evt_npc_set_ry, STRING("乗客２"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客２"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客２"), LW(3))
    RETURN
EVT_END()

NpcSetupInfo rsh_04_rnd_npc_3_ent[] = {
    {
        /*"乗客３"*/"\x8F\xE6\x8B\x71\x82\x52", // name
        0, 0, // flags..reactionFlags
        npc_urouro_init_event, // initEvent
        npc_urouro_move_event, // regularEvent
        NULL, NULL, NULL, NULL, // talkEvent..lostEvent
        npc_urouro_return_event, // returnEvent
        NULL, // blowEvent
        1, // territoryType
        {-180.0f, 0.0f, -145.0f}, // territoryBase
        {50.0f, 0.0f, 50.0f}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, // searchRange..homingAngle
        -1, // battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_rsh_04_rnd_npc_entry)
    CALL(evt_rsh_get_rnd_npc, 2, 0, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 2, 0, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 2, 0, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客１"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客１"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客１"), 265, 0, -150)
    CALL(evt_npc_set_ry, STRING("乗客１"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客１"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客１"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 2, 1, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 2, 1, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 2, 1, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客２"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客２"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客２"), -230, 0, -215)
    CALL(evt_npc_set_ry, STRING("乗客２"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客２"), NPC_FLAG_TALK_POSE)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客２"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 2, 2, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 2, 2, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 2, 2, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客３"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客３"), LW(2))
    CALL(evt_npc_setup, PTR(rsh_04_rnd_npc_3_ent))
    CALL(evt_npc_set_position, STRING("乗客３"), -180, 0, -145)
    CALL(evt_npc_set_ry, STRING("乗客３"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客３"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客３"), LW(3))
    CALL(evt_rsh_get_rnd_npc, 2, 3, LW(0))
    CALL(evt_rsh_get_rnd_npc_data, 2, 3, LW(0), LW(1), LW(2))
    CALL(evt_rsh_get_rnd_npc_talk_evt, 2, 3, LW(0), LW(3))
    CALL(evt_npc_entry, STRING("乗客４"), LW(1))
    CALL(evt_npc_set_tribe, STRING("乗客４"), LW(2))
    CALL(evt_npc_set_position, STRING("乗客４"), -85, 16, -255)
    CALL(evt_npc_set_ry, STRING("乗客４"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("乗客４"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_rsh_set_rnd_npc_talk_evt, STRING("乗客４"), LW(3))
    RETURN
EVT_END()

USER_FUNC(debug_print_func) {
    EvtWord* args = event->args;
    s32 room = evtGetValue(event, args[0]);
    s32 group = evtGetValue(event, args[1]);
    s32 index = evtGetValue(event, args[2]);
    s32 value = evtGetValue(event, args[3]);
    char header[256];
    char buf[256];
    const char** list;

    *(RshDebugStr*)header = rsh_debug_str_init;

    switch (room) {
    case 0:
        strcpy(header, "RSH_01:");
        switch (group) {
        case 0:
            list = (const char**)rsh_01_rnd_npc_1_list;
            break;
        case 1:
            list = (const char**)rsh_01_rnd_npc_2_list;
            break;
        case 2:
            list = (const char**)rsh_01_rnd_npc_3_list;
            break;
        }
        break;
    case 1:
        strcpy(header, "RSH_03:");
        switch (group) {
        case 0:
            list = (const char**)rsh_03_rnd_npc_1_list;
            break;
        case 1:
            list = (const char**)rsh_03_rnd_npc_2_list;
            break;
        }
        break;
    case 2:
        strcpy(header, "RSH_04:");
        switch (group) {
        case 0:
            list = (const char**)rsh_04_rnd_npc_1_list;
            break;
        case 1:
            list = (const char**)rsh_04_rnd_npc_2_list;
            break;
        case 2:
            list = (const char**)rsh_04_rnd_npc_3_list;
            break;
        case 3:
            list = (const char**)rsh_04_rnd_npc_4_list;
            break;
        }
        break;
    }
    sprintf(buf, "%s%d(%s, %s)\n", header, group, list[index], value);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_rsh_set_rnd_npc_talk_evt) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    EvtWord* evt = (EvtWord*)evtGetValue(event, args[1]); // event script for the npc talk

    evtNpcNameToPtr(event, name)->talkEvt = evt;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_rsh_get_rnd_npc_talk_evt) {
    EvtWord* args = event->args;
    s32 room = evtGetValue(event, args[0]);
    s32 group = evtGetValue(event, args[1]);
    s32 index = evtGetValue(event, args[2]);

    evtSetValue(event, args[3], (EvtWord)_get_rnd_npc_data(room, group, index)->talkEvt);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_rsh_get_rnd_npc_data) {
    EvtWord* args = event->args;
    s32 room = evtGetValue(event, args[0]);
    s32 group = evtGetValue(event, args[1]);
    s32 index = evtGetValue(event, args[2]);
    RshRndNpc* data = _get_rnd_npc_data(room, group, index);

    evtSetValue(event, args[3], (EvtWord)data->name);
    evtSetValue(event, args[4], (EvtWord)data->model);
    return EVT_RETURN_DONE;
}

RshRndNpc* _get_rnd_npc_data(s32 room, s32 group, s32 index) {
    RshRndNpc* data = NULL;

    switch (room) {
    case 0:
        switch (group) {
        case 0:
            data = rsh_01_rnd_npc_1;
            break;
        case 1:
            data = rsh_01_rnd_npc_2;
            break;
        case 2:
            data = rsh_01_rnd_npc_3;
            break;
        }
        break;
    case 1:
        switch (group) {
        case 0:
            data = rsh_03_rnd_npc_1;
            break;
        case 1:
            data = rsh_03_rnd_npc_2;
            break;
        }
        break;
    case 2:
        switch (group) {
        case 0:
            data = rsh_04_rnd_npc_1;
            break;
        case 1:
            data = rsh_04_rnd_npc_2;
            break;
        case 2:
            data = rsh_04_rnd_npc_3;
            break;
        case 3:
            data = rsh_04_rnd_npc_4;
            break;
        }
        break;
    }
    if (data == NULL) {
        data = rsh_01_rnd_npc_1;
        index = 0;
    }
    return &data[index];
}

USER_FUNC(evt_rsh_set_rnd_npc) {
    EvtWord* args = event->args;
    s32 room = evtGetValue(event, args[0]);
    s32 group = evtGetValue(event, args[1]);
    s32 index = evtGetValue(event, args[2]);
    s32 value;

    switch (room) {
    case 0:
        value = evtGetValue(NULL, GSW_RSH_01_RANDOM_NPCS);
        switch (group) {
        case 0:
            value |= index & 7;
            break;
        case 1:
            value |= (index & 3) << 3;
            break;
        case 2:
            value |= (index & 1) << 5;
            break;
        }
        evtSetValue(NULL, GSW_RSH_01_RANDOM_NPCS, value);
        break;
    case 1:
        value = evtGetValue(NULL, GSW_RSH_03_RANDOM_NPCS);
        switch (group) {
        case 0:
            value |= index & 0xF;
            break;
        case 1:
            value |= (index & 7) << 4;
            break;
        }
        evtSetValue(NULL, GSW_RSH_03_RANDOM_NPCS, value);
        break;
    case 2:
        value = evtGetValue(NULL, GSW_RSH_04_RANDOM_NPCS);
        switch (group) {
        case 0:
            value |= index & 1;
            break;
        case 1:
            value |= (index & 1) << 1;
            break;
        case 2:
            value |= (index & 1) << 2;
            break;
        case 3:
            value |= (index & 3) << 3;
            break;
        }
        evtSetValue(NULL, GSW_RSH_04_RANDOM_NPCS, value);
        break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_rsh_get_rnd_npc) {
    EvtWord* args = event->args;
    s32 room = evtGetValue(event, args[0]);
    s32 group = evtGetValue(event, args[1]);
    s32 value;
    s32 index;

    switch (room) {
    case 0:
        value = evtGetValue(NULL, GSW_RSH_01_RANDOM_NPCS);
        switch (group) {
        case 0:
            index = value & 7;
            break;
        case 1:
            index = (value >> 3) & 3;
            break;
        case 2:
            index = (value >> 5) & 1;
            break;
        default:
            index = 0;
            break;
        }
        break;
    case 1:
        value = evtGetValue(NULL, GSW_RSH_03_RANDOM_NPCS);
        switch (group) {
        case 0:
            index = value & 0xF;
            break;
        case 1:
            index = (value >> 4) & 7;
            break;
        default:
            index = 0;
            break;
        }
        break;
    case 2:
        value = evtGetValue(NULL, GSW_RSH_04_RANDOM_NPCS);
        switch (group) {
        case 0:
            index = value & 1;
            break;
        case 1:
            index = (value >> 1) & 1;
            break;
        case 2:
            index = (value >> 2) & 1;
            break;
        case 3:
            index = (value >> 3) & 3;
            break;
        default:
            index = 0;
            break;
        }
        break;
    }
    evtSetValue(event, args[2], index);
    return EVT_RETURN_DONE;
}

void rsh_window_tev(MapTevCallbackInfo* info) {
    MapObject* p;
    u32 flags;
    GXTevStageID tevStage;
    GXTexMapID texMap;
    GXTexCoordID texCoord;
    int indStage;
    u16 x0, y0, x1, y1;
    s32 height, width;
    s32 halfHeight, halfWidth;
    void* buf;
    GXTexObj tex;
    Mtx mtx;
    f32 t;

    p = info->mapObj;
    tevStage = info->tevStage;
    texMap = info->texMap;
    texCoord = info->texCoord;
    indStage = info->indStage;
    flags = p->flags;
    if (flags & MAP_OBJ_FLAG_GROUP_MEMBER) {
        while ((p = p->parent) != NULL) {
            u32 f = p->flags;
            if (f & MAP_OBJ_FLAG_GROUP) {
                if (!(f & MAP_OBJ_FLAG_SCREEN_RECT)) {
                    p->flags = f | MAP_OBJ_FLAG_SCREEN_RECT;
                    return;
                }
                x0 = p->screenRect[0];
                y0 = p->screenRect[1];
                x1 = p->screenRect[2];
                y1 = p->screenRect[3];
                break;
            }
        }
    } else {
        if (!(flags & MAP_OBJ_FLAG_SCREEN_RECT)) {
            p->flags = flags | MAP_OBJ_FLAG_SCREEN_RECT;
            return;
        }
        x0 = p->screenRect[0];
        y0 = p->screenRect[1];
        x1 = p->screenRect[2];
        y1 = p->screenRect[3];
    }
    if (x0 == 0x3FF || y0 == 0x3FF || x1 == 0x3FF || y1 == 0x3FF) {
        return;
    }
    if (x0 == 0 && y0 == 0 && x1 == 0 && y1 == 0) {
        return;
    }
    x0 -= x0 & 1;
    y0 -= y0 & 1;
    x1 += x1 & 1;
    y1 += y1 & 1;
    if (x1 > gp->fbWidth) {
        x1 = gp->fbWidth;
    }
    if (y1 > gp->efbHeight) {
        y1 = gp->efbHeight;
    }
    width = x1 - x0;
    height = y1 - y0;
    halfWidth = width / 2;
    halfHeight = height / 2;
    buf = smartAlloc(GXGetTexBufferSize(halfWidth, halfHeight, GX_TF_RGB5A3, GX_FALSE, 0), 3)->address;
    GXSetTexCopySrc(x0, y0, width, height);
    GXSetTexCopyDst(halfWidth, halfHeight, GX_TF_RGB5A3, GX_TRUE);
    GXCopyTex(buf, GX_FALSE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj(&tex, buf, halfWidth, halfHeight, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    t = tan(6.2831855f * (camGetPtr(CAMERA_3D)->fovY / 2.0f) / 360.0f);
    C_MTXLightFrustum(mtx, gp->efbHeight / 2 - y1, gp->efbHeight / 2 - y0, x0 - gp->fbWidth / 2,
                      x1 - gp->fbWidth / 2, gp->efbHeight / 2.0f / t, 0.5f, 0.5f, 0.4f, 0.6f);
    PSMTXConcat(mtx, camGetCurPtr()->view, mtx);
    PSMTXConcat(mtx, info->mtx, mtx);
    GXSetTexCoordGen2(texCoord, GX_TG_MTX3x4, GX_TG_POS, rsh_texmtx_tbl[texMap], GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(mtx, rsh_texmtx_tbl[texMap], GX_MTX3x4);
    GXLoadTexObj(&tex, texMap);
    GXSetTevOrder(tevStage, texCoord, texMap, GX_COLOR_NULL);
    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_3_4);
    tevStage++;
    texCoord++;
    texMap++;
    info->tevStage = tevStage;
    info->texCoord = texCoord;
    info->indStage = indStage;
    info->texMap = texMap;
}

BOOL img_wait(void) {
    ImageEntry* img = imgNameToPtr("capture_img", gp->inBattle != 0);

    if (!(img->flags & IMAGE_FLAG_HIDE) && img->loopTimes < 1.0f) {
        return FALSE;
    }
    return TRUE;
}

void _unresolved(void) {
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    if (swByteGet(0) < STG6_54_WELCOME_BACK) {
        relSetEvtAddr("rsh_00_a", rsh_00_a_init_evt);
        relSetEvtAddr("rsh_00_b", rsh_00_b_init_evt);
        relSetEvtAddr("rsh_00_c", rsh_00_c_init_evt);
        relSetEvtAddr("rsh_01_a", rsh_01_a_init_evt);
        relSetEvtAddr("rsh_01_b", rsh_01_b_init_evt);
        relSetEvtAddr("rsh_01_c", rsh_01_c_init_evt);
        relSetEvtAddr("rsh_02_a", rsh_02_a_init_evt);
        relSetEvtAddr("rsh_02_b", rsh_02_b_init_evt);
        relSetEvtAddr("rsh_02_c", rsh_02_c_init_evt);
        relSetEvtAddr("rsh_03_a", rsh_03_a_init_evt);
        relSetEvtAddr("rsh_03_b", rsh_03_b_init_evt);
        relSetEvtAddr("rsh_03_c", rsh_03_c_init_evt);
        relSetEvtAddr("rsh_04_a", rsh_04_a_init_evt);
        relSetEvtAddr("rsh_04_b", rsh_04_b_init_evt);
        relSetEvtAddr("rsh_04_c", rsh_04_c_init_evt);
        relSetEvtAddr("rsh_05_a", rsh_05_a_init_evt);
        relSetEvtAddr("rsh_06_a", rsh_06_a_init_evt);
        relSetEvtAddr("rsh_07_a", rsh_07_a_init_evt);
        relSetEvtAddr("rsh_07_b", rsh_07_b_init_evt);
        relSetEvtAddr("rsh_07_c", rsh_07_c_init_evt);
        relSetEvtAddr("rsh_08_a", rsh_08_a_init_evt);
    } else {
        relSetEvtAddr("rsh_00_a", rsh_00_a_init_evt_nidome);
        relSetEvtAddr("rsh_00_b", rsh_00_b_init_evt_nidome);
        relSetEvtAddr("rsh_00_c", rsh_00_c_init_evt_nidome);
        relSetEvtAddr("rsh_01_a", rsh_01_a_init_evt_nidome);
        relSetEvtAddr("rsh_01_b", rsh_01_b_init_evt_nidome);
        relSetEvtAddr("rsh_01_c", rsh_01_c_init_evt_nidome);
        relSetEvtAddr("rsh_02_a", rsh_02_a_init_evt_nidome);
        relSetEvtAddr("rsh_02_b", rsh_02_b_init_evt_nidome);
        relSetEvtAddr("rsh_02_c", rsh_02_c_init_evt_nidome);
        relSetEvtAddr("rsh_03_a", rsh_03_a_init_evt_nidome);
        relSetEvtAddr("rsh_03_b", rsh_03_b_init_evt_nidome);
        relSetEvtAddr("rsh_03_c", rsh_03_c_init_evt_nidome);
        relSetEvtAddr("rsh_04_a", rsh_04_a_init_evt_nidome);
        relSetEvtAddr("rsh_04_b", rsh_04_b_init_evt_nidome);
        relSetEvtAddr("rsh_04_c", rsh_04_c_init_evt_nidome);
        relSetEvtAddr("rsh_05_a", rsh_05_a_init_evt_nidome);
        relSetEvtAddr("rsh_06_a", rsh_06_a_init_evt_nidome);
        relSetEvtAddr("rsh_07_a", rsh_07_a_init_evt);
        relSetEvtAddr("rsh_07_b", rsh_07_b_init_evt);
        relSetEvtAddr("rsh_07_c", rsh_07_c_init_evt);
        relSetEvtAddr("rsh_08_a", rsh_08_a_init_evt);
    }
    relSetBtlAddr("rsh", btlsetup_rsh_tbl, btl_rsh_setup_no_tbl);
}

