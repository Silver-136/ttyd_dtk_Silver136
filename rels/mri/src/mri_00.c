#include "battle/battle_unit.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"

#include "driver/offscreendrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "driver/imgdrv.h"
#include "string.h"
#include "memory.h"
#include "stdio.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "driver/npcdrv.h"
#include "stdlib.h"
#include "driver/effdrv.h"
#include "driver/fadedrv.h"
#include "evt/evt_bero.h"
#include "mri.h"
#include "evt/evt_npc.h"

USER_FUNC(evt_img_alloc_capture);
typedef struct CloudWork { s32 used; s32 state; char animName[0x20]; char imgName[0x20]; char offscreenName[0x20]; char groupNames[8][0x20]; char hitName[0x20]; s32 limit; s32 counter; EventEntry* evt; void* endEvt; s32 timer; s32 x; s32 y; s32 w; s32 h; f32 lastFrame; } CloudWork;
void mapGrpSetOffScreen(const char* name, const char* offscreen);
void mapGrpClearOffScreen(const char* name);
static CloudWork* cloud;
static u8 cloud_once_flag;
static void _mapsetofs_all(CloudWork* wp);
static void _mapclsofs_all(CloudWork* wp);
static s32 _ofs_get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _img_alloc_capture(EvtWord a0, EvtWord a1, EvtWord a2, EvtWord a3, EvtWord a4, EvtWord a5, EvtWord a6, EvtWord a7);
void envSetYamiView(Mtx mtx);

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos);
static USER_FUNC(anim_tevcallback);
USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_bg_set_color);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_letter_box_onoff);
USER_FUNC(evt_cam_road_reset);
static USER_FUNC(evt_cloud_main);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_get_target_dir);
USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mario_clear_party);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_get_party);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_party_use_check);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_party);
USER_FUNC(evt_mario_set_party_pos);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mri_cloud_ent);
USER_FUNC(evt_mri_cloud_init);
USER_FUNC(evt_mri_get16bitdata);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_change_interrupt);
USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_get_reglid);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_reverse_ry);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_npc_wait_pera);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_jump_pos);
USER_FUNC(evt_party_move_beside_mario);
USER_FUNC(evt_party_move_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_all_party_recovery);
USER_FUNC(evt_pouch_mario_recovery);
USER_FUNC(evt_puni_flag_onoff);
USER_FUNC(evt_puni_get_dir);
USER_FUNC(evt_puni_get_position);
USER_FUNC(evt_puni_init_puni);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_pera_onoff);
USER_FUNC(evt_puni_set_dir);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_puni_wait_systemmode);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_spline_get_value);
USER_FUNC(evt_sub_spline_init);
USER_FUNC(evt_telop_entry);
USER_FUNC(get_puni_pos);
#ifdef VERSION_G8ME01
USER_FUNC(N_evt_sub_status_gauge_force_update);
USER_FUNC(evt_sub_get_language);
#endif
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
static USER_FUNC(sysflag);
static USER_FUNC(yami_view);
static void _img_alloc_capture(EvtWord a0, EvtWord a1, EvtWord a2, EvtWord a3, EvtWord a4, EvtWord a5, EvtWord a6, EvtWord a7);
static s32 _ofs_get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _mapsetofs_all(CloudWork* wp);
static void _mapclsofs_all(CloudWork* wp);
extern s32 envSetYami[];

static s16 puni_rand_position[196];
EvtWord mario_see_you_init[54];
static f32 spline_dt[24];
EvtWord seal_bigtree[213];
EvtWord seal_bigtree2[143];
static EvtWord guide_init[72];
static EvtWord guide_talk[29];
static EvtWord rival_init[21];
static EvtWord rival_talk[8];
static EvtWord chibitoge_init[25];
static EvtWord chibitoge_talk[60];
static EvtWord sister_init[21];
static EvtWord sister_talk[8];
static EvtWord elder_init[21];
static EvtWord elder_talk[8];
static EvtWord fabio_init[20];
static EvtWord fabio_talk[20];
static EvtWord odoriko_1_talk[20];
static EvtWord odoriko_1_init[33];
static EvtWord odoriko_2_talk[20];
static EvtWord odoriko_2_init[33];
static EvtWord odoriko_3_talk[20];
static EvtWord odoriko_3_init[33];
static NpcSetupInfo npcEnt[7];
static EvtWord bero_01_doa_close[7];
static EvtWord bero_01_doa_open[7];
static EvtWord first_time_w_bero[18];
EvtWord out_ne_bero[44];
static EvtWord out_w_bero[12];
EvtWord out_dokan_1[30];
BeroEntry bero_lostitem[1];
static BeroEntry bero_entry_data[4];
EvtWord first_bigtree[99];
static const char* capturemap[4];
static EvtWord evt_cloud_main_evt[4];
f32 guide_move_data[15];
EvtWord open_bigtree[446];
EvtWord mario_see_you[210];
EvtWord puni_set[59];
static EvtWord sekai_yami_npc[59];
EvtWord sekai_yami_puni[70];
static EvtWord sekai_yami_evt[322];
static EvtWord minnnanokoe_evt[620];
static EvtWord evt_switch_init[17];
#ifdef VERSION_G8ME01
EvtWord mri_00_init_evt[377];
#else
EvtWord mri_00_init_evt[296];
#endif

static void _img_alloc_capture(EvtWord a0, EvtWord a1, EvtWord a2, EvtWord a3, EvtWord a4, EvtWord a5, EvtWord a6, EvtWord a7) {
    EventEntry event;
    EvtWord args[10];

    event.args = args;
    args[0] = a0;
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;
    args[5] = a5;
    args[6] = a6;
    args[7] = a7;
    evt_img_alloc_capture(&event, TRUE);
}

static s32 _ofs_get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h) {
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;

    if (!offscreenGetBoundingBox_BeforeFrame(offscreenNameToId(name), &left, &top, &right, &bottom)) {
        return 0;
    }
    *x = left;
    *y = top;
    *w = right - left + 50;
    *h = bottom - top;
    if (*x + *w >= 608) {
        *w = 608 - *x;
    }
    return 2;
}

static void _mapsetofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpSetOffScreen(wp->groupNames[i], wp->offscreenName);
    }
}

static void _mapclsofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpClearOffScreen(wp->groupNames[i]);
    }
}

static s16 puni_rand_position[] = {
    83, 10, 33, 31, 176, 118, -98, 49,
    148, 147, 160, 59, 195, 142, 223, 45,
    10, -12, -69, 3, -50, -13, 114, 26,
    174, 74, -61, 62, 195, 90, 217, 66,
    -9, -11, 217, 107, 140, 58, -7, 46,
    157, 128, -81, 60, 157, 20, -61, 24,
    181, 55, -47, 48, 116, 84, 205, 181,
    26, 51, 111, -16, 56, 63, -120, 15,
    -67, 43, 120, 63, 70, 26, 77, -33,
    -1, 22, -117, 39, 172, 150, 196, 15,
    -20, 29, -103, 25, -85, 34, 87, 45,
    176, 98, -87, -6, 215, 86, 128, 42,
    204, 51, 92, -19, -49, 7, -69, -16,
    -86, 14, -115, 59, 108, 47, 103, 5,
    146, 3, 14, 7, 155, 80, 183, 171,
    163, 184, -27, 50, 157, 166, 130, -10,
    97, 64, 135, 77, 94, 26, -136, 47,
    210, 30, 76, 64, 181, 1, 156, 100,
    46, 46, -98, 71, 132, 97, 67, 46,
    213, 126, 50, 22, 13, 36, 168, 40,
    193, 197, 122, 8, 137, 22, 148, 39,
    195, 70, 33, 10, -41, 27, 7, 59,
    163, -7, 175, 207, -29, -10, 141, 115,
    176, 21, 190, 36, -104, 4, 37, 69,
    197, 110, 212, 162,
};

EVT_BEGIN(mario_see_you_init)
    SET_VALUE(LW(10), 5)
    SET_VALUE(LW(11), 0)
    SET_INT_READ_ADDR(PTR(puni_rand_position))
    LOOP(0)
        CALL(evt_mri_get16bitdata, PTR(puni_rand_position), LW(11), LW(0))
        ADD(LW(11), 1)
        CALL(evt_mri_get16bitdata, PTR(puni_rand_position), LW(11), LW(2))
        ADD(LW(11), 1)
        CALL(evt_puni_init_puni, LW(10), 30, LW(0), 0, LW(2), 0)
        CALL(evt_puni_flag_onoff, LW(10), 0, 8388608)
        CALL(evt_puni_pera_onoff, LW(10), 1)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

static f32 spline_dt[] = {
    140.0f, 0.0f, -140.0f, -16.6162f, 0.0f, 14.542748f, -170.98557f, 0.0f,
    12.748371f, -257.16162f, 0.0f, -40.786213f, -192.75406f, 39.299995f, -129.2951f, -128.91687f,
    74.6f, -167.18973f, -49.43834f, 143.94997f, -205.0f, 10.0f, 155.0f, -220.0f,
};

EVT_BEGIN(seal_bigtree)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_move_beside_mario, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_00"), 0, STRING("me"))
    CALL(evt_cam3d_evt_set, FLOAT(142.19922f), FLOAT(160.09961f), FLOAT(440.0996f), FLOAT(142.19922f), FLOAT(64.59961f), FLOAT(-50.799805f), 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg2_mri_e03_01"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 140, 90, 193, 140, 45, -154, 750, 11)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), 140, -140, 1000, 0, 0)
    DELETE_EVENT(LW(9))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_02"), 0, STRING("ガイド"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, FLOAT(113.299805f), FLOAT(83.59961f), FLOAT(143.5f), FLOAT(113.299805f), FLOAT(45.0f), FLOAT(-154.0f), 750, 11)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos, 0, 110, -120, 1000)
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg2_mri_e03_03"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, FLOAT(140.89941f), FLOAT(83.59961f), FLOAT(143.5f), FLOAT(140.89941f), FLOAT(45.0f), FLOAT(-154.0f), 500, 11)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_04"), 0, STRING("ガイド"))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 131088)
    CALL(evt_npc_status_onoff, 1, STRING("ガイド"), 2)
    CALL(evt_npc_change_interrupt, STRING("ガイド"), 6, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_spline_init, 0, 0, PTR(spline_dt), 8, 50, 0)
        LABEL(0)
        CALL(evt_sub_spline_get_value, LW(3), LW(4), LW(5), LW(6))
        SET_VALUE(LW(0), STRING("ガイド"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイド"), LW(3), LW(5), 160, 0, 0)
        DELETE_EVENT(LW(9))
        IF_INT_EQUAL(LW(6), 1)
            GOTO(0)
        END_IF
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
        CALL(evt_npc_change_interrupt, STRING("ガイド"), 6, PTR(guide_talk))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW(0), 75)
    SET_VALUE(GSWF(2881), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(seal_bigtree2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_06"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg2_mri_e03_07"))
    WAIT_MS(300)
    SET_VALUE(LW(0), STRING("me"))
    SPAWN_EVENT(PTR(mri_puni_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("me"), 10, 155, -230, 200, 20)
    SET_VALUE(LW(0), STRING("me"))
    SPAWN_EVENT(PTR(mri_puni_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("me"), 10, 155, -230, 200, 20)
    CALL(evt_cam3d_evt_set, 5, 277, 176, 5, 200, -216, 500, 11)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_08"), 0, STRING("me"))
    WAIT_MS(200)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e03_09"), 0, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_party_pos, 1, 10, LW(0), LW(1), LW(2))
    CALL(evt_party_run, 1)
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW(0), 76)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_lostitem))
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(guide_init)
    SWITCH(GSW(0))
        CASE_BETWEEN(73, 74)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 16777216)
        CALL(evt_npc_set_position, STRING("me"), 135, 0, -70)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_EQUAL(75)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 16777216)
        CALL(evt_npc_set_position, STRING("me"), 10, 155, -220)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_EQUAL(88)
        IF_INT_EQUAL(GSWF(2826), 0)
            CALL(evt_npc_set_position, STRING("me"), 140, 0, -140)
            CALL(evt_npc_set_ry, STRING("me"), 90)
        END_IF
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            CALL(evt_npc_set_position, STRING("me"), -34, 0, 177)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(guide_talk)
    SWITCH(GSW(0))
        CASE_EQUAL(74)
        SPAWN_EVENT(PTR(seal_bigtree))
        CASE_EQUAL(75)
        SPAWN_EVENT(PTR(seal_bigtree2))
        CASE_EQUAL(88)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e10_05"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(118)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_07"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(rival_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            CALL(evt_npc_set_position, STRING("me"), 38, 0, 143)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(rival_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_09"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(chibitoge_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            IF_INT_NOT_EQUAL(GSWF(2878), 0)
                CALL(evt_npc_set_position, STRING("me"), 50, 0, 191)
                CALL(evt_npc_set_ry, STRING("me"), 270)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(chibitoge_talk)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 18)
    RETURN
EVT_END()

static EVT_BEGIN(sister_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            CALL(evt_npc_set_position, STRING("me"), -10, 0, 121)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(sister_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_08"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(elder_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            CALL(evt_npc_set_position, STRING("me"), 10, 0, 214)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(elder_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_10"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(fabio_init)
    IF_INT_NOT_EQUAL(GSW(0), 400)
        IF_INT_EQUAL(GSWF(5402), 1)
            CALL(evt_npc_set_position, STRING("me"), -375, FLOAT(0.5595703f), -134)
            CALL(evt_npc_set_ry, STRING("me"), 90)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(fabio_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("irai_28_006"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("irai_28_006_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_1_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_07"), 0, STRING("踊り子１"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_08"), 0, STRING("踊り子２"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_09"), 0, STRING("踊り子３"))
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_1_init)
    SET_VALUE(LW(0), STRING("踊り子１"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_1_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), 525824)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 90, 0, 400)
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_2_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_07"), 0, STRING("踊り子１"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_08"), 0, STRING("踊り子２"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_09"), 0, STRING("踊り子３"))
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_2_init)
    SET_VALUE(LW(0), STRING("踊り子２"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子ピンク"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_2_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), 525824)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 120, 0, 370)
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_3_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_07"), 0, STRING("踊り子１"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_08"), 0, STRING("踊り子２"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_00_09"), 0, STRING("踊り子３"))
    RETURN
EVT_END()

static EVT_BEGIN(odoriko_3_init)
    SET_VALUE(LW(0), STRING("踊り子３"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子黄緑"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_3_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), 525824)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 150, 0, 420)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガイド"*/"\x83\x4B\x83\x43\x83\x68", // name
        0x80600, // flags
        0, // reactionFlags
        guide_init, // initEvent
        NULL, // regularEvent
        guide_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ライバル"*/"\x83\x89\x83\x43\x83\x6F\x83\x8B", // name
        0x80600, // flags
        0, // reactionFlags
        rival_init, // initEvent
        NULL, // regularEvent
        rival_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ちびトゲ"*/"\x82\xBF\x82\xD1\x83\x67\x83\x51", // name
        0x80600, // flags
        0, // reactionFlags
        chibitoge_init, // initEvent
        NULL, // regularEvent
        chibitoge_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガイド妹"*/"\x83\x4B\x83\x43\x83\x68\x96\x85", // name
        0x80600, // flags
        0, // reactionFlags
        sister_init, // initEvent
        NULL, // regularEvent
        sister_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56", // name
        0x80600, // flags
        0, // reactionFlags
        elder_init, // initEvent
        NULL, // regularEvent
        elder_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ファビオ"*/"\x83\x74\x83\x40\x83\x72\x83\x49", // name
        0x80600, // flags
        0, // reactionFlags
        fabio_init, // initEvent
        NULL, // regularEvent
        fabio_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(bero_01_doa_close)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_naname_close))
    RETURN
EVT_END()

static EVT_BEGIN(bero_01_doa_open)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_naname_open))
    RETURN
EVT_END()

static EVT_BEGIN(first_time_w_bero)
    IF_INT_EQUAL(GSW(0), 73)
        SET_VALUE(GF(0), 1)
        WAIT_WHILE(GF(0))
        SET_VALUE(LW(0), 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(out_ne_bero)
    IF_INT_EQUAL(GSWF(2826), 0)
        WAIT_MS(50)
        CALL(evt_npc_set_ry, STRING("ガイド"), 90)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
            CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
            CALL(evt_mario_set_party_pos, 1, 10, LW(0), LW(1), LW(2))
            CALL(evt_party_set_dir, 1, 90, 0)
        END_INLINE_EVENT
        SET_VALUE(GSWF(2826), 1)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(out_w_bero)
    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(out_dokan_1)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 112)
        IF_INT_EQUAL(GSWF(2884), 0)
            SET_VALUE(GSWF(2884), 1)
        END_IF
        IF_INT_EQUAL(GSWF(2840), 1)
            SET_VALUE(GSWF(2840), 0)
        END_IF
        IF_INT_EQUAL(GSWF(2827), 1)
            SET_VALUE(GSWF(2827), 0)
        END_IF
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

BeroEntry bero_lostitem[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)out_w_bero, // exitEvent
        "win_03", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)first_time_w_bero, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_01", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "ne_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)out_ne_bero, // exitEvent
        "mri_01", // nextMapName
        "w_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        bero_01_doa_close, // enterAnimeArg
        bero_01_doa_open, // exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        (s32*)out_dokan_1, // exitEvent
        "tik_16", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(first_bigtree)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, -700, 500, 916, 275, 500, -195, 0, 0)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_seq_wait, 2)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_telop_entry, 5)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 275, 250, 1795, 275, 280, -195, 5000, 11)
    WAIT_MS(6000)
    CALL(evt_cam3d_evt_set, FLOAT(-601.0996f), FLOAT(208.2998f), FLOAT(720.8994f), FLOAT(-601.0996f), FLOAT(65.09961f), FLOAT(-15.699219f), 1000, 11)
    WAIT_MS(550)
    CALL(evt_party_run, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, -618, LW(2), FLOAT(120.0f))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GF(0), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_snd_bgm_scope, 0, 0)
    SET_VALUE(GSW(0), 74)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static const char* capturemap[] = {
    "S_non",
    "S_ha_10_07",
    "S_kesu",
    "\0",
};

static USER_FUNC(evt_cloud_main) {
    CloudWork* wp = (CloudWork*)event->lwData[0];
    s32 group = gp->inBattle != 0;
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    HitEntry* hit0;
    HitEntry* hit1;
    HitEntry* hit2;
    HitEntry* hit3;
    HitEntry* hit;
    ImageEntry* img;

    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }
    if (party == NULL) {
        return EVT_RETURN_BLOCK;
    }
    hit0 = (HitEntry*)cloudGetHitBreathout(0);
    hit1 = (HitEntry*)cloudGetHitBreathout(1);
    hit2 = (HitEntry*)cloudGetHitBreathout(2);
    hit3 = (HitEntry*)cloudGetHitBreathout(3);
    switch (wp->state) {
        case 0:
            if ((s32)cloud_once_flag == 1) {
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if (hit0 == hit || hit1 == hit || hit2 == hit || hit3 == hit) {
                wp->state++;
            }
            if (wp->counter > 0) {
                wp->counter--;
            }
            break;
        case 1:
            if (mario->displayDirectionTarget > 90.0f && mario->displayDirectionTarget <= 270.0f) {
                strcpy(wp->animName, "P_oof_cloud_2");
            } else {
                strcpy(wp->animName, "P_oof_cloud_1");
            }
            if (!animGroupBaseAsync(wp->animName, group, NULL)) {
                break;
            }
            cloud_once_flag = 1;
            animPaperPoseEntry(wp->animName, group);
            offscreenEntry(wp->offscreenName);
            _mapsetofs_all(wp);
            wp->state++;
            break;
        case 2:
            imgEntry(wp->imgName, group);
            img = imgNameToPtr(wp->imgName, group);
            img->poseId = animPaperPoseGetId(wp->animName, group);
            img->animTime = animTimeGetTime(group);
            imgClearVirtualPoint(img);
            imgSetShadow(img, 2);
            img->flags |= 2;
            wp->state = 222;
            break;
        case 222:
            if (!_ofs_get_boundingbox(wp->offscreenName, &wp->x, &wp->y, &wp->w, &wp->h)) {
                break;
            }
            _img_alloc_capture((EvtWord)wp->imgName, 0, 0, 1, wp->x, wp->y, wp->w, wp->h);
            wp->state = 223;
            break;
        case 223:
            img = imgNameToPtr(wp->imgName, group);
            img->flags &= ~2;
            wp->state = 3;
        case 3:
            img = imgNameToPtr(wp->imgName, group);
            img->animName = "A_1";
            img->animTime = animTimeGetTime(group);
            img->flags &= ~2;
            wp->timer = 30;
            wp->lastFrame = 0.0f;
            wp->state++;
            break;
        case 4:
            img = imgNameToPtr(wp->imgName, group);
            if (wp->lastFrame == 0.0f || wp->lastFrame + 1.0f < img->loopTimes) {
                psndSFXOn_3D("SFX_OFF_CLAUD_WIND2", &mario->position);
                wp->lastFrame = img->loopTimes;
            }
            if (mario->currMotionId != 27) {
                wp->state = 10;
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if (hit0 != hit && hit1 != hit && hit2 != hit && hit3 != hit) {
                if (wp->timer == 0) {
                    wp->state = 10;
                } else {
                    wp->timer--;
                    wp->counter += 2;
                }
            } else {
                wp->timer = 30;
                wp->counter += 2;
                if (wp->counter > wp->limit) {
                    wp->state = 20;
                }
            }
            break;
        case 10:
            wp->state = 99;
            break;
        case 20:
            psndSFXOn_3D("SFX_OFF_CLAUD_WIND3", &mario->position);
            if (wp->endEvt != NULL) {
                evtEntry(wp->endEvt, 0, 0);
            }
            img = imgNameToPtr(wp->imgName, group);
            img->animName = "A_2";
            img->animTime = animTimeGetTime(group);
            wp->timer = 60;
            wp->state++;
            break;
        case 21:
            if (wp->timer != 0) {
                wp->timer--;
                break;
            }
            wp->timer = 30;
            wp->counter += 2;
            wp->state = 100;
            break;
        case 99:
        case 100:
            img = imgNameToPtr(wp->imgName, group);
            _mapclsofs_all(wp);
            animPaperPoseRelease(img->poseId);
            imgFreeCapture(img, 0);
            imgSetShadow(img, 0);
            imgRelease(img, group);
            offscreenDelete(wp->offscreenName);
            cloud_once_flag = 0;
            if (wp->state == 99) {
                wp->state = 0;
                break;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mri_cloud_init) {
    s32 i;

    cloud = _mapAlloc(sizeof(CloudWork) * 16);
    for (i = 0; i < 16; i++) {
        cloud[i].used = 0;
        sprintf(cloud[i].imgName, "i_name_%1d", i);
        sprintf(cloud[i].offscreenName, "o_name_%1d", i);
    }
    cloud_once_flag = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_cloud_ent) {
    EvtWord* args = event->args;
    char** groups;
    char* hitName;
    void* endEvt;
    CloudWork* wp;
    s32 i;
    s32 j;

    groups = (char**)evtGetValue(event, args[0]);
    hitName = (char*)evtGetValue(event, args[1]);
    endEvt = (void*)evtGetValue(event, args[2]);

    for (i = 0; i < 16; i++) {
        if (cloud[i].used == 0) {
            break;
        }
    }
    if (i >= 16) {
        return EVT_RETURN_DONE;
    }
    wp = &cloud[i];
    wp->used = 1;
    strcpy(wp->animName, "P_oof_cloud_1");
    for (j = 0; j < 8; j++) {
        if ((s8)groups[j][0] == 0) {
            break;
        }
        strcpy(wp->groupNames[j], groups[j]);
    }
    strcpy(wp->hitName, hitName);
    wp->endEvt = endEvt;
    wp->limit = 180;
    wp->counter = 0;
    wp->state = 0;
    wp->timer = 0;
    wp->evt = evtEntry(evt_cloud_main_evt, 0, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

f32 guide_move_data[] = {
    46.053036f, 145.50023f, -145.69612f, 84.1548f, 145.50023f, -166.70631f, 99.65739f, 143.0f,
    -188.3354f, 130.73201f, 155.65002f, -185.70927f, 160.16298f, 165.45027f, -184.42146f,
};

EVT_BEGIN(open_bigtree)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_non"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tree_futa"), 1)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_mov_pos2, -12, -206, FLOAT(120.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, -44, -206, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_party_jump_pos, 1, 20, 154, -220, 500, 0, -1000)
    CALL(evt_eff_fukidashi, 4, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_cam3d_evt_set, 21, 288, 213, 21, 205, -216, 500, 11)
    WAIT_MS(1200)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_00"), 0, STRING("extparty"))
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_mario_clear_party, 1)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT(PTR(mri_puni_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), 54, 143, -202, 200, 20)
    CALL(evt_cam3d_evt_set, 187, 288, 213, 187, 205, -216, 2000, 11)
    CALL(evt_npc_set_anim, STRING("ガイド"), STRING("W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 1073741856)
    CALL(evt_sub_spline_init, 1, 11, PTR(guide_move_data), 5, 0, 2000)
    LOOP(0)
        CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(3), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(200)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT(PTR(mri_puni_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), 191, 192, -184, 200, 20)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), 216, -214, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), 1073741856)
    CALL(evt_cam3d_evt_set, -26, 275, 213, -26, 192, -216, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_01"), 0, STRING("party"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_01_01"), 0, STRING("party"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_set, LW(0), 226, 30, LW(0), 178, -216, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_01_02"), 0, STRING("party"))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("ガイド"), 226, 0, -225)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 1073741856)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_02"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, 140, 90, 193, 140, 45, -154, 500, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_hit_get_position, STRING("ne_bero"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_UNLOCK"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_LAMP"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("light"), 0, 0)
    WAIT_MS(1000)
    SET_VALUE(LW(10), STRING("S_doa"))
    CALL_CHILD(PTR(evt_mri_door_naname_open_pure))
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        SET_VALUE(LW(10), STRING("S_doa"))
        SPAWN_EVENT(PTR(evt_mri_door_naname_close_pure))
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), 140, -140, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_03"), 0, STRING("ガイド"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -26, 275, 213, -26, 192, -216, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e10_04"), 0, STRING("party"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), 1073741856)
    SET_VALUE(GSW(0), 88)
    SET_VALUE(LW(0), STRING("ne_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mario_see_you)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, -85, 0, 185)
    CALL(evt_party_set_pos, 0, -125, 0, 185)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_cam3d_evt_set, 180, 130, 567, 180, 44, 127, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_puni_wait_systemmode, 2)
    CALL_CHILD(PTR(mario_see_you_init))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_seq_wait, 2)
    CALL(evt_fade_end_wait)
    CALL(evt_cam3d_evt_set, -29, 130, 567, -29, 44, 127, 3000, 0)
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_01"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, 9, 68, 429, 9, 12, 134, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_02"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_set, -11, 68, 333, -11, 12, 38, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_03"), 0, STRING("ガイド妹"))
    CALL(evt_cam3d_evt_set, -35, 73, 400, -35, 17, 105, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_04"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -65, 90, 575, -65, 30, 28, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg2_mri_e26_00_05"))
    CALL(evt_cam3d_evt_set, -29, 130, 567, -29, 44, 127, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e26_00_06"), 0, STRING("ガイド"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 1000, 11)
    SET_VALUE(GSW(0), 118)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static USER_FUNC(yami_view) {
    f32 x = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){-100.0f + x, 100.0f, 0.0f}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){x, 0.0f, 0.0f});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_puni_pos) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 x;
    s32 z;

    x = (id % 15) * 20 - 280 + rand() % 10;
    z = 180 - ((id / 15) * 20 + rand() % 30 - 15);
    evtSetValue(event, args[1], x);
    evtSetValue(event, args[2], z);
    return EVT_RETURN_DONE;
}


static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(1, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rotY = ry;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(puni_set)
    SET_VALUE(LW(0), 5)
    SET_VALUE(LW(1), 0)
    LOOP(0)
        CALL(get_puni_pos, LW(1), LW(2), LW(3))
        CALL(evt_sub_random, 1, LW(5))
        IF_INT_EQUAL(LW(5), 0)
            SET_VALUE(LW(5), 90)
            ELSE
            SET_VALUE(LW(5), 270)
        END_IF
        CALL(evt_puni_init_puni, LW(0), 30, LW(2), 0, LW(3), 0)
        CALL(evt_puni_set_dir, LW(0), LW(5))
        CALL(evt_puni_flag_onoff, LW(0), 0, 8388608)
        ADD(LW(0), 1)
        ADD(LW(1), 1)
        IF_INT_EQUAL(LW(0), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_npc)
    LOOP(0)
        CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
        IF_INT_GREATER_OR_EQUAL(GW(0), LW(1))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_reglid, LW(0), LW(1))
    DELETE_EVENT(LW(1))
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(0), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(1000)
    LOOP(0)
        CALL(evt_sub_random, 400, LW(1))
        ADD(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_npc_reverse_ry, LW(0))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, LW(0))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sekai_yami_puni)
    LOOP(0)
        CALL(evt_puni_get_position, LW(0), LW(1), LW(2), LW(3))
        IF_INT_GREATER_OR_EQUAL(GW(0), LW(1))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_puni_get_position, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_eff_fukidashi, 2, STRING(""), 0, 0, 0, LW(1), LW(2), LW(3), 0, 0, 48)
    WAIT_MS(1000)
    LOOP(0)
        CALL(evt_sub_random, 400, LW(1))
        ADD(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_puni_get_dir, LW(0), LW(1))
        IF_INT_LESS(LW(1), 180)
            CALL(evt_puni_set_dir, LW(0), 270)
            ELSE
            CALL(evt_puni_set_dir, LW(0), 90)
        END_IF
        WAIT_MS(500)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_puni_wait_systemmode, 2)
    SPAWN_EVENT(PTR(puni_set))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ライバル"), -100, 0, 225)
    CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    CALL(evt_npc_set_position, STRING("ガイド"), -150, 0, 250)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_position, STRING("ガイド妹"), -175, 0, 275)
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
    CALL(evt_npc_set_position, STRING("プニ族長老"), -120, 0, 300)
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
    CALL(evt_cam3d_evt_set, -150, 258, 1648, -150, 180, 150, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, 11, 255, 64, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_bg_set_color, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), 256)
    CALL(anim_tevcallback, STRING("ライバル"), 1)
    CALL(anim_tevcallback, STRING("ガイド"), 1)
    CALL(anim_tevcallback, STRING("ガイド妹"), 1)
    CALL(anim_tevcallback, STRING("プニ族長老"), 1)
    SET_VALUE(LSWF(3), 1)
    SET_VALUE(GW(0), -800)
    SET_VALUE(LW(0), STRING("ライバル"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(sekai_yami_npc), GW(1))
    SET_VALUE(LW(0), STRING("ガイド妹"))
    SPAWN_EVENT_GET_ID(PTR(sekai_yami_npc), GW(2))
    SET_VALUE(LW(0), STRING("プニ族長老"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), 6)
    SPAWN_EVENT(PTR(sekai_yami_puni))
    SET_VALUE(LW(0), 39)
    SPAWN_EVENT(PTR(sekai_yami_puni))
    SET_VALUE(LW(0), 88)
    SPAWN_EVENT(PTR(sekai_yami_puni))
    SET_VALUE(LW(0), 46)
    SPAWN_EVENT(PTR(sekai_yami_puni))
    SET_VALUE(LW(0), 64)
    SPAWN_EVENT(PTR(sekai_yami_puni))
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S_yuka_hika_02"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka_hika_02"), 1)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_STG8_WORLD_DARK1"), 0)
    CALL(evt_sub_intpl_msec_init, 11, -800, 800, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(yami_view, LW(0))
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -150, 74, 650, -150, 40, 150, 1000, 11)
    WAIT_MS(1000)
    DELETE_EVENT(GW(1))
    DELETE_EVENT(GW(2))
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ガイド"))
    CALL(evt_npc_wait_pera, STRING("ガイド妹"))
    CALL(evt_msg_print, 0, STRING("stg8_dark_02"), 0, STRING("ガイド妹"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_dark_02_01"), 0, STRING("ガイド"))
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("tou_01"), STRING("sekai_yami1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_puni_wait_systemmode, 2)
    SPAWN_EVENT(PTR(puni_set))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ガイド"), -150, 0, 250)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_position, STRING("ガイド妹"), -175, 0, 275)
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 90)
    CALL(evt_npc_set_position, STRING("プニ族長老"), -120, 0, 300)
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
    CALL(evt_npc_set_position, STRING("ライバル"), -100, 0, 225)
    CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka_hika_02"), 1)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), 256)
    CALL(anim_tevcallback, STRING("ライバル"), 1)
    CALL(anim_tevcallback, STRING("ガイド"), 1)
    CALL(anim_tevcallback, STRING("ガイド妹"), 1)
    CALL(anim_tevcallback, STRING("プニ族長老"), 1)
    SET_VALUE(LSWF(3), 1)
    CALL(yami_view, 800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_cam3d_evt_set, -150, 258, 1648, -150, 180, 150, 0, 11)
    CALL(starstone_entry, STRING("starstone"), -150, 660, 200)
    CALL(evt_seq_wait, 2)
    WAIT_MS(500)
    SET_VALUE(GW(0), 660)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), -150, GW(0), 200)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("starstone"), LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, 660, 60, 4500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, 11, 6000, 6500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, 11, 6500, 6000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -150, 74, 650, -150, 40, 150, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), LW(0), LW(1), LW(2), 300, 10)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_10"), 0, STRING("ガイド"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_11"), 0, STRING("プニ族長老"))
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ガイド"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_12"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("ガイド妹"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド妹"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド妹"), LW(0), LW(1), LW(2), 300, 10)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド妹"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド妹"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_cam3d_evt_set, -150, 62, 550, -150, 35, 150, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_13"), 0, STRING("ガイド妹"))
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ガイド"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_14"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("ガイド妹"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド妹"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド妹"), LW(0), LW(1), LW(2), 300, 10)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド妹"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド妹"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_15"), 0, STRING("ガイド妹"))
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), LW(0), LW(1), LW(2), 300, 10)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ガイド"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_16"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -150, 74, 650, -150, 40, 150, 800, 11)
    WAIT_MS(800)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 30)
            CALL(evt_puni_get_position, LW(0), LW(1), LW(2), LW(3))
            CALL(evt_sub_random, 10, LW(4))
            ADD(LW(4), 25)
            ADD(LW(2), LW(4))
            CALL(evt_msg_toge, 3, LW(1), LW(2), LW(3))
            BEGIN_INLINE_EVENT
                CALL(evt_msg_print, 0, STRING("stg8_las_148_17"), 0, 0)
            END_INLINE_EVENT
            WAIT_MS(300)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(4000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("tou_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_init)
    IF_INT_EQUAL(GSWF(1345), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dakan"), 1)
        CALL(evt_hitobj_onoff, STRING("A_dokan"), 1, 0)
    END_IF
    RETURN
EVT_END()

static USER_FUNC(sysflag) {
    if (gp->flags & 1) {
        gp->flags &= ~1;
        fadeEntry(0x1C, 0, (GXColor){0, 0, 0, 0});
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(mri_00_init_evt)
    CALL(sysflag)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 0, 0, -310, -1000, -1000, 0, 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    CALL_CHILD(PTR(evt_switch_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    IF_INT_EQUAL(GSW(0), 117)
        CALL(evt_mario_get_party, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_set_party, 0, 1)
            CALL(evt_party_run, 0)
#ifdef VERSION_G8ME01
        END_IF
#endif
            CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
#ifdef VERSION_G8ME01
        CALL(N_evt_sub_status_gauge_force_update)
#else
        END_IF
#endif
        SPAWN_EVENT(PTR(mario_see_you))
    END_IF
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(117)
        IF_INT_EQUAL(GSWF(2884), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_puni_wait_systemmode, 2)
                SPAWN_EVENT(PTR(mario_see_you_init))
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    CALL(evt_map_playanim, STRING("hikari_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_sibu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("neon"), 1, 0)
    SET_VALUE(LW(0), -220)
    SET_VALUE(LW(1), 1)
    SET_VALUE(LW(2), -295)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    SET_VALUE(LW(0), 531)
    SET_VALUE(LW(1), 1)
    SET_VALUE(LW(2), 118)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW(0), 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
            CALL(evt_snd_envoff, 512)
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, 512)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        IF_INT_NOT_EQUAL(GSW(0), 73)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI2"))
            ELSE
            BEGIN_INLINE_EVENT
                WAIT_MS(2000)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
            END_INLINE_EVENT
            CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI2"))
        END_IF
    END_IF
    CALL(evt_item_entry, STRING("i"), 280, 528, 0, 110, 0, GSWF(2825), 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_GREATER_OR_EQUAL(GSW(0), 88)
        IF_INT_LESS_OR_EQUAL(GSW(0), 112)
            IF_INT_EQUAL(GSWF(2826), 1)
                CALL_CHILD(PTR(odoriko_1_init))
                CALL_CHILD(PTR(odoriko_2_init))
                CALL_CHILD(PTR(odoriko_3_init))
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSW(0), 73)
        SPAWN_EVENT(PTR(first_bigtree))
    END_IF
    IF_INT_LESS_OR_EQUAL(GSW(0), 87)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_non"), 1)
        CALL(evt_mri_cloud_init)
        CALL(evt_mri_cloud_ent, PTR(&capturemap), STRING("A_kaze"), PTR(open_bigtree))
        SET_VALUE(LW(0), STRING("ne_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_map_playanim, STRING("light"), 2, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tree_futa"), 1)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kesu"), 1)
    IF_INT_EQUAL(GSW(0), 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_kan"), 16384)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(0)
        CASE_OR_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_kan"), 16384)
        CASE_END
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_kan"), 16384)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_kan"), 16384)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_kan"), 16384)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_kan"), 16384)
    END_SWITCH
#endif
    RETURN
EVT_END()
