#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord door01_close[12];
static EvtWord door01_open[12];
EvtWord out_func[14];
static BeroEntry bero_entry_data[3];
static EvtWord shake_evt[150];
static EvtWord item_evt[268];
static EvtWord bomb_wall_evt[179];
EvtWord gon_07_init_evt[86];

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(out_func)
    CALL(evt_sub_countdown_end)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 43)
        CALL(evt_snd_sfxoff, GW(10))
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        out_func, // exitEvent
        "gon_06", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_13", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(shake_evt)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

static EVT_BEGIN(item_evt)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 43)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, 43)
    SET_VALUE(GSWF(1494), 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("gon07treasure"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("k_slit"), ITEM_BLACK_KEY_0x22, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("k_slit"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("gon07treasure"))
    CALL(evt_item_get_item, STRING("k_slit"))
    CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAKE4"), GW(10))
    SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(10))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 20, 35, FLOAT(60.0f))
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -20, 35, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 124, 408, 0, 57, 21, 500, 11)
    WAIT_MS(400)
    CALL(evt_snd_sfxon, STRING("SFX_STG1_TOGE_UP1"), 0)
    WAIT_MS(100)
    CALL(evt_hitobj_onoff, STRING("a_togeshita"), 1, 1)
    CALL(evt_map_playanim, STRING("a_to_shita"), 0, 0)
    LABEL(0)
    CALL(evt_map_checkanim, STRING("a_to_shita"), LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(2)
    CALL(evt_sub_rumble_onoff, 1, 0)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg1_gon_15"))
    CALL(evt_map_playanim, STRING("a_to_ue"), 0, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER1"))
    CALL(evt_cam3d_evt_set_at, 0, 300, 0, 0, 900, 11)
    WAIT_MS(2600)
    CALL(evt_cam3d_evt_set, 0, 124, 408, 0, 57, 21, 500, 11)
    WAIT_MS(500)
    CALL(evt_map_playanim, STRING("a_to_ue"), 0, 1)
    CALL(evt_map_pauseanim, 0, STRING("a_to_ue"))
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg1_gon_16"))
    CALL(evt_map_replayanim, 0, STRING("a_to_ue"))
    SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(10))
    CALL(evt_map_checkanim, STRING("a_to_ue"), LW(0), LW(1))
    CALL(evt_sub_countdown_start, LW(1), LW(1))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_bero_exec_get, LW(0))
            IF_BITS_SET(LW(0), 1)
                DEBUG_PUT_MSG(STRING("脱出成功"))
                BREAK_LOOP
            END_IF
            CALL(evt_sub_countdown_get_status, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                SET_VALUE(LW(0), STRING("w_bero"))
                CALL_CHILD(PTR(bero_case_switch_off))
                CALL(evt_seq_set_seq, SEQ_GAME_OVER, 0, 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    BEGIN_INLINE_EVENT
        LABEL(1)
        CALL(evt_map_checkanim, STRING("a_to_ue"), LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(1)
        END_IF
        CALL(evt_snd_sfxoff, GW(10))
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(bomb_wall_evt)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox2, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(LW(3), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_wait_animend, STRING("img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_img_release, STRING("img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_hitobj_onoff, STRING("san"), 1, 0)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSWF(1495), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(gon_07_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 43)
        SET_VALUE(GSWF(1494), 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("gon07treasure"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_evt), GSWF(1494))
    CALL(evt_hitobj_onoff, STRING("a_togeshita"), 1, 0)
    IF_INT_EQUAL(GSWF(1495), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("san"), 0, PTR(bomb_wall_evt), 0)
        ELSE
        CALL(evt_hitobj_onoff, STRING("san"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

