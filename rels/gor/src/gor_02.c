#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "string.h"
#include "memory.h"
#include "cam_road.h"
#include "battle/battle_monosiri.h"
#include "driver/msgdrv.h"
#include "driver/swdrv.h"
#include "driver/imgdrv.h"
#include "mario/mariost.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "mario/mario.h"
#include "stdio.h"
#include "driver/bgdrv.h"
#include "dolphin/mtx.h"
#include "eff/eff_stardust.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "evt/evt_door.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "gor.h"
#include "driver/hitdrv.h"
#include "data_tables.h"
#include "pmario_sound.h"


f32 reviseAngle(f32 angle);
typedef struct MonosiriFollow { s32 seq; s32* list; s32 count; } MonosiriFollow;
#define FOLLOW (follow)
void winLectureOn(void);
BOOL winLectureCheck(void);
void statusWinForceOff(void);
void statusWinForceOpen(void);
void door_position_sub(s32 type, const char* mapObj, const char* hitA, const char* hitB, Vec* outPos, Vec* inPos, s32 flag);
void mapGrpFlagOn(const char* name, u32 flags);
void mapGrpFlagOff(const char* name, u32 flags);
void bgTransOffsetOn(void);
void bgTransOffsetOff(void);
f32 intplGetValue(s32 mode, s32 currStep, f32 start, f32 end, s32 steps);
typedef struct DoorColorWork { GXColor color; GXColor color2; } DoorColorWork;
DoorColorWork* _door;
static u8* _ap;
static s32 xlw[16];
s32* iri_13_item_tbl;
char* camstring;

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry);
USER_FUNC(_door_color2_main_func);
USER_FUNC(_door_color_init_func);
USER_FUNC(_door_color_main_func);
static USER_FUNC(bgScrollControll);
static USER_FUNC(bgm_start_wait);
USER_FUNC(camera_front_check);
USER_FUNC(camera_move_check);
static USER_FUNC(effect_stardust);
static USER_FUNC(enzan_cam_turn_keisan_f);
static USER_FUNC(enzan_lw_load);
static USER_FUNC(enzan_lw_save);
static USER_FUNC(evt_StatusClose);
static USER_FUNC(evt_StatusOpen);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_set_xyz);
USER_FUNC(evt_cam_ctrl_onoff);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_cam_letter_box_onoff);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_get_mode);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_door_data_copy);
USER_FUNC(evt_door_end_wait);
USER_FUNC(evt_door_param);
USER_FUNC(evt_door_set_param);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_eff_softdelete);
USER_FUNC(evt_exit_case_evt);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_goki_sub_intpl_get_value);
USER_FUNC(evt_goki_sub_intpl_init);
USER_FUNC(evt_gor_door_in_info);
USER_FUNC(evt_gor_door_out_info);
USER_FUNC(evt_gor_villager_urouro_check_func);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_set_shadow);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_delete);
USER_FUNC(evt_item_delete_check);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_position);
USER_FUNC(evt_item_set_position);
USER_FUNC(evt_item_set_scale);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_map_blend_set_flag);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_blend);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_set_offscreen);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_cancel_roll_motion);
USER_FUNC(evt_mario_clear_party);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_dispdir);
USER_FUNC(evt_mario_get_exparty);
USER_FUNC(evt_mario_get_party);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_goodbye_party);
USER_FUNC(evt_mario_hello_party);
USER_FUNC(evt_mario_jump_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_party_door_move);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_dispdir);
USER_FUNC(evt_mario_set_party_pos);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_set_prev_party_pos);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_kururing_floor);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_set_position);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_msg_continue);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_add_dirdist);
USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_battle_result);
USER_FUNC(evt_npc_get_dir);
USER_FUNC(evt_npc_get_loiter_dir);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_color);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_rotate);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_set_ry_lr);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_npc_wait_pera);
USER_FUNC(evt_offscreen_delete);
USER_FUNC(evt_offscreen_entry);
USER_FUNC(evt_offscreen_get_boundingbox);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_dispflg_onoff);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_move_beside_mario);
USER_FUNC(evt_party_move_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_outofscreen);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_breed_pose);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_dir_npc);
USER_FUNC(evt_party_set_dir_pos);
USER_FUNC(evt_party_set_dispdir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_set_pose);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_get_max_ap);
USER_FUNC(evt_pouch_get_supercoin);
USER_FUNC(evt_pouch_get_yoshiname);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_pouch_set_ap);
USER_FUNC(evt_pouch_set_coin);
USER_FUNC(evt_pouch_set_supercoin);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_set_bgtpl);
USER_FUNC(evt_set_dir_to_target);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfx_pos);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_check_intersect);
USER_FUNC(evt_sub_get_coin);
USER_FUNC(evt_sub_get_dir);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_load_progresstime);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_save_playtime);
USER_FUNC(evt_sub_status_gauge_check_update);
USER_FUNC(evt_sub_status_gauge_force_open);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_item_select);
USER_FUNC(evt_win_other_select);
USER_FUNC(get_camcurve);
USER_FUNC(get_camcurve_init);
USER_FUNC(iri_13_free_item_tbl);
USER_FUNC(iri_13_make_item_tbl);
USER_FUNC(iri_13_update_item_tbl);
USER_FUNC(johoya_data_alloc);
USER_FUNC(johoya_data_free);
USER_FUNC(johoya_perfect_newjoho_setreadflag);
USER_FUNC(lecture);
USER_FUNC(monosiri_check);
USER_FUNC(monosiri_on);
static USER_FUNC(set_depthPlaneOffset);
USER_FUNC(set_talk_memory_ry);
static USER_FUNC(setmertn);
USER_FUNC(status_gauge_force_off);
USER_FUNC(stonechangertn);
USER_FUNC(unitwin_getpartylank);
USER_FUNC(unitwin_setpartylank);
USER_FUNC(evt_mario_set_dir_pos);
USER_FUNC(evt_win_party_lankup_chk);
USER_FUNC(get_mario_dir_target);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
#ifdef VERSION_G8ME01
USER_FUNC(evt_sub_get_language);
USER_FUNC(evt_msg_fmt_int);
#endif
void kuragarisan_msgcb(s32 type, void* param);
static USER_FUNC(enzan_set_f_i);
static USER_FUNC(enzan_set_i_f);
static USER_FUNC(enzan_cam_turn_keisan);
static USER_FUNC(wariai_enzan_0f);
static USER_FUNC(wariai_enzan_0);
static USER_FUNC(wariai_enzan_1f_intpl);
static USER_FUNC(wariai_enzan_1f);
static USER_FUNC(wariai_enzan_1);
static USER_FUNC(alpha_set_func);
static USER_FUNC(alpha_init_func);
static void alpha_tev(MapTevCallbackInfo* info);
void _door_color2_tev(MapTevCallbackInfo* info);
void _door_color_tev(MapTevCallbackInfo* info);

void _door_color_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;
    DoorColorWork* door = _door;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, door->color);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

void _door_color2_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;
    DoorColorWork* door = _door;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, door->color2);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

USER_FUNC(_door_color_init_func) {
    _door = _mapAlloc(sizeof(DoorColorWork));
    _door->color.r = 255;
    _door->color.g = 255;
    _door->color.b = 255;
    _door->color.a = 255;
    _door->color2.r = 255;
    _door->color2.g = 255;
    _door->color2.b = 255;
    _door->color2.a = 255;
    return EVT_RETURN_DONE;
}

USER_FUNC(_door_color_main_func) {
    EvtWord* args = event->args;
    DoorColorWork* door = _door;

    door->color.r = evtGetValue(event, args[0]);
    door->color.g = evtGetValue(event, args[1]);
    door->color.b = evtGetValue(event, args[2]);
    door->color.a = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(_door_color2_main_func) {
    EvtWord* args = event->args;
    DoorColorWork* door = _door;

    door->color2.r = evtGetValue(event, args[0]);
    door->color2.g = evtGetValue(event, args[1]);
    door->color2.b = evtGetValue(event, args[2]);
    door->color2.a = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

static void alpha_tev(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    u8* alpha = _ap;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, *alpha});
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

f32 fd_gor_02_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gor_02_neg_one_382;
f32 fd_gor_02___local_PI_383;
f32 fd_gor_02___local_3PIO2_385;
f32 fd_gor_02_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gor_02___local_PIO2_384;
f32 fd_gor_02_one_381;
f32 fd_gor_02_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gor_02_neg_one_365;
f32 fd_gor_02___local_PI_366;
f32 fd_gor_02___local_3PIO2_368;
f32 fd_gor_02_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gor_02___local_PIO2_367;
f32 fd_gor_02_one_364;
EvtWord gor_02_hitobj_off[27];
EvtWord gor_02_hitobj_on[54];
EvtWord gor_02_event_00[18];
EvtWord gor_02_event_01[30];
static BeroEntry bero_entry_data[3];
EvtWord powerupya_party_lank_rtn[786];
#ifdef VERSION_G8ME01
EvtWord powerupya_party_rtn[170];
#else
EvtWord powerupya_party_rtn[167];
#endif
EvtWord powerupya_table_talk[91];
EvtWord powerupya_direct_talk[96];
s32 powerupya_item_tbl[2];
EvtWord powerupya_stardust[32];
EvtWord powerupya_mario_move[48];
EvtWord powerupya_party_move[38];
EvtWord powerupya_orb_use[544];
EvtWord powerupya_talk[119];
EvtWord mapball_init[19];
EvtWord powerupya_init[58];
#ifdef VERSION_G8ME01
EvtWord johoyaperfect_talk[169];
#else
EvtWord johoyaperfect_talk[146];
#endif
static EvtWord kurihakase_init[134];
static EvtWord kurihakase_talk[421];
EvtWord oyabun_init[7];
EvtWord oyabun_talk[364];
EvtWord tozokudan1_init[36];
EvtWord tozokudan1_talk[285];
EvtWord tozokudan2_init[24];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
EvtWord tozokudan2_house_door[139];
EvtWord mario_yokeru[127];
EvtWord evt_unlock_mario[78];
static EvtWord evt_unlock[6];
EvtWord evt_unlock_npc[97];
EvtWord tozokudan2_talk[121];
EvtWord tozokudan3_init[17];
EvtWord tozokudan3_talk[22];
EvtWord sanders_open_door[56];
static EvtWord sanders_init[37];
static EvtWord sanders_talk[574];
EvtWord kuribo4_init[13];
EvtWord kuribo4_regl[19];
static s32 item_tbl[2];
EvtWord kuribo4_talk[276];
EvtWord chusan3_init[2];
EvtWord chusan3_talk[76];
EvtWord roten3_init[2];
EvtWord roten3_talk[174];
EvtWord borodo1_init[26];
EvtWord borodo1_regl[26];
EvtWord borodo1_talk[76];
static EvtWord borodo2_init[35];
static EvtWord borodo2_escape[51];
static EvtWord borodo2_talk[100];
EvtWord kuragarisan_init[7];
EvtWord kuragarisan_write_name[435];
EvtWord kuragarisan_talk[108];
static EvtWord luigi_init[18];
static EvtWord luigi_talk[4];
static NpcSetupInfo npcEnt[16];
static EvtWord evt_door_01_00[12];
static EvtWord evt_door_01_01[12];
static EvtWord evt_wall_01[15];
static const char* door_data_01_npc[2];
static EvtWord evt_door_01_open[88];
static EvtWord evt_door_01_close[85];
static DoorParam door_data_01;
static EvtWord evt_door_02_00[12];
EvtWord evt_door_02_01[12];
static EvtWord evt_wall_02[15];
static const char* door_data_02_npc[2];
EvtWord evt_door_02_open[127];
EvtWord evt_door_02_close[76];
static DoorParam door_data_02;
EvtWord evt_door_02_entry[64];
static EvtWord evt_door_03_00[12];
EvtWord evt_door_03_01[12];
static EvtWord evt_wall_03[15];
static const char* door_data_03_npc[2];
static const char* door_data_03_mobj[2];
static EvtWord evt_door_03_close[166];
static EvtWord evt_door_03_open[14];
static DoorParam door_data_03;
EvtWord evt_door_sanders_open[27];
EvtWord evt_door_sanders_close[27];
static EvtWord evt_door_03_entry[64];
static EvtWord evt_door_03_caseopen[51];
EvtWord evt_door_03_roll_open[163];
EvtWord evt_door_03_roll_close[12];
EvtWord evt_wall_03_roll[15];
static DoorParam door_data_03_roll;
EvtWord evt_door_03_roll_entry[20];
static EvtWord evt_door_04_00[12];
static EvtWord evt_door_04_01[12];
static EvtWord evt_wall_04[38];
#ifdef VERSION_G8ME01
static EvtWord evt_door_04_open[19];
#else
static EvtWord evt_door_04_open[14];
#endif
#ifdef VERSION_G8ME01
static EvtWord evt_door_04_close[21];
#else
static EvtWord evt_door_04_close[16];
#endif
static const char* door_data_04_npc[2];
static const char* door_data_04_item[4];
static DoorParam door_data_04;
EvtWord evt_door_04_junbi[20];
EvtWord evt_door_04_00_ura[12];
EvtWord evt_door_04_01_ura[12];
EvtWord evt_wall_04_ura[21];
DoorParam door_data_04_ura;
EvtWord evt_door_05_00[12];
EvtWord evt_wall_05[28];
const char* door_data_05_npc[4];
const char* door_data_05_npc_01[3];
EvtWord evt_door_05_open[14];
EvtWord evt_door_05_close[23];
DoorParam door_data_05;
EvtWord evt_door_06_00[12];
EvtWord evt_wall_06_00[62];
EvtWord evt_wall_06_01[59];
EvtWord evt_wall_06_02[62];
EvtWord evt_wall_06[12];
const char* door_data_06_npc[2];
const char* door_data_06_npc_01[1];
EvtWord evt_door_06_open[14];
EvtWord evt_door_06_close[16];
DoorParam door_data_06;
EvtWord evt_gor_door_open_sanders[80];
EvtWord evt_gor_door_open_sub[44];
EvtWord evt_gor_door_dark_sub[200];
EvtWord evt_gor_door_open[82];
EvtWord evt_gor_door_close_sanders[80];
EvtWord evt_gor_door_close_sub[31];
EvtWord evt_gor_door_light_sub[194];
EvtWord evt_gor_door_close[66];
EvtWord evt_gor_offscren_door_entry[26];
static EvtWord mario_kirimomi_event[67];
EvtWord cam_yurasu_event[8];
EvtWord steel_coin[386];
EvtWord kurihakase_first_talk[787];
EvtWord kurihakase_wall_remove[257];
EvtWord kurihakase_lecture[286];
EvtWord kurihakase_door[84];
#ifdef VERSION_G8ME01
EvtWord kurihakase_lecture_badge[282];
#else
EvtWord kurihakase_lecture_badge[272];
#endif
EvtWord Mario_Party_position[33];
EvtWord book_open[42];
EvtWord book_close[42];
EvtWord book_center_init[22];
#ifdef VERSION_G8ME01
EvtWord epigraphy_map[621];
#else
EvtWord epigraphy_map[614];
#endif
#ifdef VERSION_G8ME01
EvtWord epigraphy_map_after_stage1[258];
#else
EvtWord epigraphy_map_after_stage1[251];
#endif
#ifdef VERSION_G8ME01
EvtWord epigraphy_map_after_stage2[400];
#else
EvtWord epigraphy_map_after_stage2[393];
#endif
#ifdef VERSION_G8ME01
EvtWord epigraphy_map_after_stage3[368];
#else
EvtWord epigraphy_map_after_stage3[361];
#endif
#ifdef VERSION_G8ME01
EvtWord epigraphy_map_after_stage4[297];
#else
EvtWord epigraphy_map_after_stage4[290];
#endif
#ifdef VERSION_G8ME01
EvtWord epigraphy_map_after_stage5[225];
#else
EvtWord epigraphy_map_after_stage5[218];
#endif
EvtWord epigraphy_map_after_stage6[245];
EvtWord kurihakase_after3minutes[177];
static HitDamageReturn hitDmgRetPoints[13];
static EvtWord gor_goki1_urouro[116];
static EvtWord gor_goki2_urouro[109];
static EvtWord mobj_box01[34];
s32 stg0_list[1];
s32 stg1_list[4];
s32 stg2_list[4];
s32 stg3_list[1];
s32 stg5_list[2];
s32 stg6_list[1];
s32 stg7_list[6];
s32 stg8_list[10];
MonosiriFollow follow[8];
EvtWord evt_monosiri[39];
EvtWord evt_monosiri_follow[18];
EvtWord evt_cloud[10];
EvtWord iri_19_init[20];
static EvtWord epilogue_npc_entry[40];
static EvtWord epilogue_evt_sub[56];
static EvtWord epilogue_evt[101];
EvtWord dearu_event[13];
EvtWord front_wall_keshi[27];
EvtWord tobiori_check[48];
EvtWord gor_02_init_evt[543];

f32 fd_gor_02_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gor_02_neg_one_382 = -1.0f;

f32 fd_gor_02___local_PI_383 = 3.1415927f;

f32 fd_gor_02___local_3PIO2_385 = 4.712389f;

f32 fd_gor_02_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gor_02___local_PIO2_384 = 1.5707964f;

f32 fd_gor_02_one_381 = 1.0f;

f32 fd_gor_02_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gor_02_neg_one_365 = -1.0f;

f32 fd_gor_02___local_PI_366 = 3.1415927f;

f32 fd_gor_02___local_3PIO2_368 = 4.712389f;

f32 fd_gor_02_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gor_02___local_PIO2_367 = 1.5707964f;

f32 fd_gor_02_one_364 = 1.0f;

EVT_BEGIN(gor_02_hitobj_off)
    CALL(evt_hitobj_onoff, STRING("A_kabe"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_saku"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_kmn"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_gomi"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_l_ura_gomi"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(gor_02_hitobj_on)
    CALL(evt_hitobj_onoff, STRING("A_kabe"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_saku"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_kmn"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_gomi"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_l_ura_gomi"), 1, 1)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 10)
        CALL(evt_hitobj_onoff, STRING("A_saku_04"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(1213), 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
    END_IF
    IF_INT_EQUAL(GW(10), 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(gor_02_event_00)
    IF_INT_NOT_EQUAL(GSW(0), 7)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SPAWN_EVENT(PTR(steel_coin))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(gor_02_event_01)
    IF_INT_EQUAL(GSWF(1207), 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_party_stop, 1)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_party_pos, 1, 11, 0, -1000, 0)
    SET_VALUE(GSWF(1207), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)gor_02_event_00, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        (s32*)gor_02_event_01, // exitEvent
        "tik_01", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(effect_stardust) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 arg6 = evtGetFloat(event, args[4]);
    f32 arg7 = evtGetFloat(event, args[5]);
    s32 arg1 = evtGetValue(event, args[6]);
    s32 arg2 = evtGetValue(event, args[7]);

    effStardustEntry(type, arg1, arg2, x, y, z, arg6, arg7);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1f_intpl) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);
    s32 out = args[4];
    f32 t = evtGetFloat(event, args[5]);
    f32 r;

    if (t >= 14.0f) {
        t -= 14.0f;
        if (t >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r = 1.0f - intplGetValue(t, b - a, 1.0f, 0.0f, b);
    } else {
        r = intplGetValue(t, b - a, 0.0f, 1.0f, b);
    }
    evtSetFloat(event, out, (1.0f - r) * c + r * d);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_0) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_0f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_cam_turn_keisan) {
    EvtWord* args = event->args;
    f32 angle = evtGetValue(event, args[0]) % 360;
    s32 xVar = args[1];
    s32 zVar = args[2];
    Vec pos;
    Vec center;
    Mtx mtx;

    pos.x = evtGetValue(event, xVar);
    pos.z = evtGetValue(event, zVar);
    pos.y = 0.0f;
    center.x = evtGetValue(event, args[3]);
    center.z = evtGetValue(event, args[4]);
    center.y = 0.0f;
    PSVECSubtract(&pos, &center, &pos);
    PSMTXRotRad(mtx, 'Y', 0.017453292f * angle);
    PSMTXMultVec(mtx, &pos, &pos);
    PSVECAdd(&pos, &center, &pos);
    evtSetValue(event, xVar, pos.x);
    evtSetValue(event, zVar, pos.z);
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_cam_turn_keisan_f) {
    f32 t = evtGetFloat(event, event->args[0]) / 100.0f;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec base;
    Vec pos;
    Vec pivot;
    Mtx mtx;
    f32 x = -362.0f;

    base = (Vec){x, 193.0f + -62.0f * t, 686.0f + -354.0f * t};
    pos = base;
    pivot = (Vec){-362.0f, 37.0f, -200.0f};
    PSVECSubtract(&pos, &pivot, &pos);
    PSMTXRotRad(mtx, 'Y', 0.017453292f * (720.0f * t));
    PSMTXMultVec(mtx, &pos, &pos);
    PSVECAdd(&pos, &pivot, &pos);
    cam->presetPos = cam->cameraPos;
    cam->presetAt = cam->cameraAt;
    cam->storedPos = pos;
    cam->storedAt = cam->cameraAt;
    cam->startTimeA = gp->renderTime;
    cam->durationA = 0;
    cam->stateA = 3;
    cam->curveA = 0;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(powerupya_party_lank_rtn)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_party_set_dir_pos, 0, -362, -200)
            CALL(evt_mario_set_dir_pos, -362, -200)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(enzan_lw_save)
    CALL(evt_snd_bgmoff_f, 1024, 2000)
    CALL(evt_msg_print, 0, STRING("powerup_12"), 0, STRING("me"))
    CALL(evt_sub_intpl_msec_init, 11, 0, 20, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_i01_sui_ao"), 0, LW(0), 0)
        CALL(evt_mapobj_trans, 1, STRING("S_i01_sui_aka"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_POWERUP1"))
    CALL(bgm_start_wait)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -362, 44, -200, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_sui_ao"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_sui_aka"), 1)
    CALL(evt_msg_print, 0, STRING("powerup_14"), 0, STRING("me"))
    CALL(bgScrollControll, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
    CALL(evt_mario_get_exparty, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_map_blend_set_flag, 1, STRING("extparty"), 2048)
    END_IF
    CALL(evt_map_blend_set_flag, 1, STRING("パワーアップ屋"), 2048)
    CALL(evt_map_set_blend, 0, 255, 255, 255, 255)
    CALL(evt_set_bgtpl, STRING("powup"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 255, 0, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S_ie_01"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("S_i01_kmnC"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("S_i01_sui"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 90, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_s"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 1600)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -362, 193, 686, -362, 37, -200, 1600, 11)
    WAIT_MS(1600)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 100, 7000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(enzan_cam_turn_keisan_f, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(6), 7000)
        DIVIDE(LW(6), 1000)
        ADD(LW(6), 1)
        LOOP(LW(6))
            CALL(evt_eff64, 0, STRING("crystal_n64"), 3, -362, 44, -200, FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1000)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
        SET_FLOAT_VALUE(LW(7), LW(4))
        ADD_FLOAT(LW(7), FLOAT(18.0f))
        CALL(evt_eff64, STRING("cccx"), STRING("pokopi_pcharge_n64"), 2, LW(3), LW(7), LW(5), FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
        WAIT_MS(500)
        CALL(evt_mario_get_party, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_C_1"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_C_1"))
            CASE_EQUAL(3)
            CALL(evt_party_set_pose, 0, STRING("C_1"))
            CASE_EQUAL(4)
            CALL(evt_party_set_pose, 0, STRING("PYS_C_1"))
            CASE_EQUAL(5)
            CALL(evt_party_set_pose, 0, STRING("PWD_C_1"))
            CASE_EQUAL(6)
            CALL(evt_party_set_pose, 0, STRING("PTR_C_1"))
            CASE_EQUAL(7)
            CALL(evt_party_set_pose, 0, STRING("PCH_C_1"))
        END_SWITCH
    END_INLINE_EVENT
    WAIT_MS(7000)
    CALL(evt_eff_softdelete, STRING("aaax"))
    IF_INT_EQUAL(GSWF(1186), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_sui_ao"), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_sui_aka"), 1)
    END_IF
    WAIT_MS(500)
    CALL(evt_eff64, STRING("dddx"), STRING("release_n64"), 65283, -362, 44, -200, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("cccx"))
    CALL(evt_mario_get_party, LW(1))
    SWITCH(LW(1))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_I_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_I_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("I_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_I_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_I_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_I_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_I_1"))
    END_SWITCH
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    SET_FLOAT_VALUE(LW(7), LW(4))
    ADD_FLOAT(LW(7), FLOAT(18.0f))
    CALL(effect_stardust, 0, LW(3), LW(7), LW(5), FLOAT(50.0f), FLOAT(50.0f), 20, 120)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S_ie_01"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("S_i01_kmnC"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("S_i01_sui"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 90, 0, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_s"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("S_i01_honC_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(bgScrollControll, 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_naka"), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_soto"), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_doa"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
        CALL(evt_mario_get_exparty, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_map_blend_set_flag, 1, STRING("extparty"), 2048)
        END_IF
        CALL(evt_map_blend_set_flag, 1, STRING("パワーアップ屋"), 2048)
        CALL(evt_map_set_blend, 0, 0, 0, 0, 0)
        CALL(evt_set_bgtpl, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 1600, 11)
    WAIT_MS(1400)
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_sub_intpl_msec_init, 11, 20, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_i01_sui_ao"), 0, LW(0), 0)
        CALL(evt_mapobj_trans, 1, STRING("S_i01_sui_aka"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_get_party, LW(1))
    SWITCH(LW(1))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_Y_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_Y_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("Y_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_Y_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_Y_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_Y_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    END_SWITCH
    WAIT_MS(1500)
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(enzan_lw_load)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    RETURN
EVT_END()

EVT_BEGIN(powerupya_party_rtn)
    CALL(evt_win_coin_on, 1, LW(12))
    LOOP(0)
        CALL(evt_party_set_dir_pos, 0, -362, -200)
        CALL(evt_win_other_select, 6)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print, 0, STRING("powerup_13_01"), 0, STRING("me"))
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_party, LW(8))
        IF_INT_NOT_EQUAL(LW(1), LW(8))
            CALL(evt_mario_goodbye_party, 0)
            CALL(evt_mario_hello_party, 0, LW(1))
#ifdef VERSION_G8ME01
            CALL(evt_party_stop, 0)
#endif
        END_IF
        CALL(evt_msg_print, 0, STRING("powerup_02"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("powerup_03"))
        CALL(evt_msg_continue)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_pouch_get_supercoin, LW(0))
            SUBTRACT(LW(0), 1)
            CALL(evt_pouch_set_supercoin, LW(0))
            WAIT_FRAMES(4)
            SUBTRACT(LW(0), 1)
            CALL(evt_pouch_set_supercoin, LW(0))
            WAIT_FRAMES(4)
            SUBTRACT(LW(0), 1)
            CALL(evt_pouch_set_supercoin, LW(0))
            WAIT_FRAMES(4)
            CALL(evt_win_coin_wait, LW(12))
            WAIT_MS(500)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_mario_get_party, LW(8))
            CALL(unitwin_getpartylank, LW(8), LW(0))
            ADD(LW(0), 1)
            CALL(unitwin_setpartylank, LW(8), LW(0))
            CALL_CHILD(PTR(powerupya_party_lank_rtn))
            CALL(evt_pouch_get_supercoin, LW(0))
            IF_INT_LESS(LW(0), 3)
                CALL(evt_msg_print, 0, STRING("powerup_13"), 0, STRING("me"))
                BREAK_LOOP
            END_IF
            CALL(evt_win_party_lankup_chk, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 0, STRING("powerup_13"), 0, STRING("me"))
                BREAK_LOOP
                ELSE
                CALL(evt_msg_print, 0, STRING("powerup_15"), 0, STRING("me"))
                CALL(evt_msg_select, 0, STRING("powerup_03"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_continue)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("powerup_13_02"))
                    BREAK_LOOP
                END_IF
            END_IF
            CALL(evt_win_coin_on, 1, LW(12))
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(powerupya_table_talk)
    CALL(setmertn, STRING("パワーアップ屋"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_win_party_lankup_chk, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("powerup_16"), 0, STRING("me"))
        GOTO(0)
    END_IF
    IF_INT_EQUAL(GSWF(1185), 0)
        CALL(evt_msg_print, 0, STRING("powerup_00"), 0, STRING("me"))
        SET_VALUE(GSWF(1185), 1)
        SET_VALUE(GSWF(1250), 1)
        ELSE
        IF_INT_EQUAL(GSWF(1250), 0)
            CALL(evt_msg_print, 0, STRING("powerup_00_01"), 0, STRING("me"))
            SET_VALUE(GSWF(1250), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("powerup_00_02"), 0, STRING("me"))
        END_IF
    END_IF
    CALL(evt_pouch_get_supercoin, LW(2))
    IF_INT_LESS(LW(2), 3)
        CALL(evt_msg_print_add, 0, STRING("powerup_01"))
        ELSE
        CALL(evt_msg_continue)
        CALL_CHILD(PTR(powerupya_party_rtn))
    END_IF
    LABEL(0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(powerupya_direct_talk)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_01_naka"), 1)
    CALL(evt_mapobj_get_position, STRING("S_ie_01_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 104, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 11, 104, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_01_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_naka"), 1)
    RETURN
EVT_END()

s32 powerupya_item_tbl[] = {
    108, -1,
};

EVT_BEGIN(powerupya_stardust)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(powerupya_mario_move)
    WAIT_MS(200)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), -210)
        CALL(evt_mario_mov_pos2, -360, -270, FLOAT(120.0f))
        CALL(evt_mario_mov_pos2, -404, -220, FLOAT(120.0f))
        ELSE
        CALL(evt_mario_mov_pos2, -360, -160, FLOAT(120.0f))
        CALL(evt_mario_mov_pos2, -395, -160, FLOAT(120.0f))
    END_IF
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    RETURN
EVT_END()

EVT_BEGIN(powerupya_party_move)
    WAIT_MS(200)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), -210)
        CALL(evt_party_move_pos2, 0, -360, -270, FLOAT(120.0f))
        ELSE
        CALL(evt_party_move_pos2, 0, -360, -160, FLOAT(120.0f))
    END_IF
    CALL(evt_party_move_pos2, 0, -425, -186, FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 90, 200)
    RETURN
EVT_END()

EVT_BEGIN(powerupya_orb_use)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    ADD(LW(4), 20)
    ADD(LW(5), 10)
    CALL(get_mario_dir_target, LW(6))
    IF_FLOAT_EQUAL(LW(6), FLOAT(0.0f))
        ADD(LW(3), -20)
        ELSE
        ADD(LW(3), 20)
    END_IF
    CALL(evt_item_entry, STRING("watasu"), 108, LW(3), LW(4), LW(5), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("watasu"), 2)
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("watasu"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_msg_print, 0, STRING("powerup_ev_04"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_N_1"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("me"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -335, 100, 158, -335, 33, -236, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("powerup_ev_05"), 0, STRING("me"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_2"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 2160, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("me"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(powerupya_stardust), LW(5))
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_SHINE1"), LW(7), LW(8), LW(9), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LOTTERY_BOMB_BURST2"), LW(7), LW(8), LW(9), 0)
    SET_VALUE(LW(10), LW(8))
    ADD(LW(10), 200)
    CALL(evt_sub_intpl_msec_init, 0, LW(8), LW(10), 1600)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("me"), LW(7), LW(0), LW(9))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DELETE_EVENT(LW(5))
    CALL(evt_npc_set_ry_lr, STRING("me"), 1)
    WAIT_MS(400)
    CALL(evt_snd_sfxoff, LW(6))
    WAIT_MS(1600)
    CALL(evt_mario_get_pos, 0, LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(2000)
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BREAK_FLOOR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 1000)
    WAIT_MS(500)
    CALL(evt_item_entry, STRING("orb"), 88, -365, 150, -110, 13, -1, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), -365)
            CALL(evt_mario_set_dir, 90, 200, 0)
            ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), -365)
            CALL(evt_party_set_dir, 0, 90, 200)
            ELSE
            CALL(evt_party_set_dir, 0, 270, 200)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 2160, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("me"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING("me"), -400, 200, -100)
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))
    CALL(evt_sub_intpl_msec_init, 0, LW(8), 0, 1600)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("me"), LW(7), LW(0), LW(9))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_NPC_LANDING1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_MS(750)
    CALL(evt_msg_print, 0, STRING("powerup_ev_06"), 0, STRING("me"))
    SPAWN_EVENT(PTR(powerupya_mario_move))
    CALL(evt_party_stop, 0)
    SPAWN_EVENT(PTR(powerupya_party_move))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_move_position, STRING("me"), -365, -115, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
    CALL(evt_item_delete, STRING("orb"))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("me"), -338, -160, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    WAIT_MS(1500)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_POWERUP1"))
    CALL(bgm_start_wait)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -362, 27, -180, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_sui_ao"), 1)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("aaax"))
    SET_VALUE(GSWF(1186), 1)
    CALL(stonechangertn, STRING("S_i01_sui_aka"))
    CALL(evt_snd_bgmoff_f, 513, 2000)
    WAIT_MS(2000)
    CALL(evt_npc_jump_position_nohit, STRING("me"), -320, 0, -205, 500, 50)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("powerup_ev_07"), 0, STRING("me"))
    CALL(evt_pouch_remove_item, 108, LW(0))
    SET_VALUE(GSWF(1203), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmoff_f, 513, 2000)
    CALL(evt_snd_bgmon_f, 288, 0, 2000)
    RETURN
EVT_END()

EVT_BEGIN(powerupya_talk)
    IF_INT_EQUAL(GSWF(1203), 1)
        IF_INT_EQUAL(GSWF(1204), 0)
            CALL(evt_msg_print, 0, STRING("powerup_ev_01"), 0, STRING("me"))
#ifdef VERSION_G8ME01
            CALL(evt_cam3d_evt_set, -320, 114, 371, -320, 45, -23, 500, 11)
#else
            CALL(evt_cam3d_evt_set, -355, 114, 371, -355, 45, -23, 500, 11)
#endif
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("powerup_ev_02"), 0, STRING("me"))
            SPAWN_EVENT_GET_ID(PTR(powerupya_direct_talk), LW(10))
            WAIT_MS(500)
            CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
            CALL(evt_npc_move_position, STRING("me"), -320, -205, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
            LOOP(0)
                IS_EVENT_RUNNING(LW(10), LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_cam3d_evt_off, 500, 11)
            WAIT_MS(500)
            SET_VALUE(LW(0), PTR(&door_data_01))
            CALL_CHILD(PTR(evt_door_entry))
            SET_VALUE(GSWF(1204), 1)
            ELSE
            CALL(evt_pouch_check_item, 108, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_win_item_select, 0, PTR(powerupya_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL_CHILD(PTR(powerupya_orb_use))
                END_IF
                ELSE
                CALL(evt_msg_print, 0, STRING("powerup_ev_03"), 0, STRING("me"))
            END_IF
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("powerup_05"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(mapball_init)
    IF_INT_EQUAL(GSWF(1186), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_sui_aka"), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_sui_ao"), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(powerupya_init)
    CALL(evt_run_case_evt, 9, 1, STRING("A_power_up"), 0, PTR(powerupya_table_talk), 0)
    IF_INT_EQUAL(GSWF(1203), 1)
        IF_INT_EQUAL(GSWF(1204), 0)
            CALL(evt_npc_set_position, STRING("パワーアップ屋"), -320, 0, -10)
            SET_VALUE(LW(0), PTR(&door_data_01))
            CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
            END_IF
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(evt_hitobj_onoff, LW(2), 1, 0)
            END_IF
            RETURN
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_01))
    CALL_CHILD(PTR(evt_door_entry))
    RETURN
EVT_END()

EVT_BEGIN(johoyaperfect_talk)
    IF_INT_EQUAL(GSWF(4), 0)
        CALL(evt_msg_print, 0, STRING("johoyaperfect_00"), 0, STRING("me"))
        SET_VALUE(GSWF(4), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("johoyaperfect_00_01"), 0, STRING("me"))
    END_IF
    LOOP(0)
        CALL(johoya_data_alloc, STRING("jperfect_code"))
        CALL(evt_win_coin_on, 0, LW(12))
        CALL(evt_win_other_select, 17)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print, 0, STRING("johoyaperfect_03"), 0, STRING("me"))
            CALL(johoya_data_free)
            RETURN
        END_IF
        IF_INT_EQUAL(LW(4), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print_insert, 0, STRING("johoyaperfect_06"), 0, STRING("me"), LW(3), LW(2))
            CALL(evt_msg_print_add, 0, LW(1))
            CALL(evt_msg_print_add, 0, STRING("johoyaperfect_07"))
            ELSE
#ifdef VERSION_G8ME01
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
#endif
            CALL(evt_msg_print_insert, 0, STRING("johoyaperfect_01"), 0, STRING("me"), LW(3), LW(2), LW(4))
#ifdef VERSION_G8ME01
                CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("johoyaperfect_01"), LW(4))
                CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("me"), LW(3), LW(2))
            END_SWITCH
#endif
            CALL(evt_msg_select, 0, STRING("johoyaperfect_02"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_pouch_get_coin, LW(0))
                IF_INT_LESS(LW(0), LW(4))
                    CALL(evt_msg_print_add, 0, STRING("johoyaperfect_09"))
                    CALL(evt_win_coin_off, LW(12))
                    CALL(johoya_data_free)
                    RETURN
                END_IF
                MULTIPLY(LW(4), -1)
                CALL(evt_pouch_add_coin, LW(4))
                CALL(evt_win_coin_wait, LW(12))
                WAIT_MS(500)
                CALL(evt_win_coin_off, LW(12))
                CALL(evt_msg_print_add, 0, LW(1))
                CALL(johoya_perfect_newjoho_setreadflag)
                CALL(evt_msg_print_add, 0, STRING("johoyaperfect_07"))
                ELSE
                CALL(evt_win_coin_off, LW(12))
                CALL(evt_msg_print_add, 0, STRING("johoyaperfect_04"))
                CALL(johoya_data_free)
                RETURN
            END_IF
        END_IF
        CALL(johoya_data_free)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(kurihakase_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    SWITCH(GSW(0))
        CASE_EQUAL(8)
        CALL(evt_npc_set_position, STRING("me"), -135, 0, -240)
        CASE_EQUAL(9)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_EQUAL(10)
        CALL(evt_npc_set_position, STRING("me"), -220, 0, 305)
        CASE_EQUAL(11)
        IF_INT_EQUAL(GSWF(1207), 0)
            CALL(evt_npc_set_position, STRING("me"), -220, 0, 305)
            ELSE
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_IF
        CASE_BETWEEN(11, 17)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(376, 402)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_EQUAL(18)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(64)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(120)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(174)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(221)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(271)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(347)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -250)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kurihakase_talk)
    SWITCH(GSW(0))
        CASE_EQUAL(11)
        CALL(evt_msg_print, 0, STRING("mac_0_085"), 0, STRING("me"))
        CASE_LESS(23)
        CALL(evt_msg_print, 0, STRING("gor_02_013"), 0, STRING("me"))
        CASE_LESS(57)
        CALL(evt_msg_print, 0, STRING("gor_02_014"), 0, STRING("me"))
        CASE_BETWEEN(56, 64)
        CALL(evt_msg_print, 0, STRING("mac_1_000"), 0, STRING("me"))
        CASE_LESS(71)
        CALL(evt_msg_print, 0, STRING("gor_02_015"), 0, STRING("me"))
        CASE_LESS(113)
        CALL(evt_msg_print, 0, STRING("gor_02_016"), 0, STRING("me"))
        CASE_BETWEEN(112, 120)
        CALL(evt_msg_print, 0, STRING("mac_2_000"), 0, STRING("me"))
        CASE_LESS(127)
        CALL(evt_msg_print, 0, STRING("gor_02_017"), 0, STRING("me"))
        CASE_LESS(165)
        CALL(evt_msg_print, 0, STRING("gor_02_018"), 0, STRING("me"))
        CASE_BETWEEN(165, 174)
        CALL(evt_msg_print, 0, STRING("mac_3_000"), 0, STRING("me"))
        CASE_LESS(178)
        IF_INT_EQUAL(GSWF(1341), 0)
            CALL(evt_msg_print, 0, STRING("gor_02_019"), 0, STRING("me"))
            ELSE
            IF_INT_EQUAL(GSWF(1218), 0)
                CALL(evt_party_stop, 0)
                CALL(evt_msg_print_party, STRING("mac_3_008"))
                CALL(evt_msg_print, 0, STRING("mac_3_009"), 0, STRING("me"))
                WAIT_MS(1500)
                CALL(evt_eff_fukidashi, 1, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 60)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("mac_3_010"), 0, STRING("me"))
                SET_VALUE(GSWF(1218), 1)
                CALL(evt_party_run, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("gor_02_020"), 0, STRING("me"))
            END_IF
        END_IF
        CASE_BETWEEN(212, 221)
        CALL(evt_msg_print, 0, STRING("mac_4_000"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_021"), 0, STRING("me"))
        CASE_LESS(224)
        CALL(evt_msg_print, 0, STRING("gor_02_022"), 0, STRING("me"))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 0, STRING("mac_4_048"), 0, STRING("me"))
        SET_VALUE(GSWF(1214), 1)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 0, STRING("mac_4_060"), 0, STRING("me"))
        CASE_BETWEEN(262, 271)
        CALL(evt_msg_print, 0, STRING("mac_5_000"), 0, STRING("me"))
        CASE_LESS(269)
        CALL(evt_msg_print, 0, STRING("gor_02_023"), 0, STRING("me"))
        CASE_LESS(282)
        CALL(evt_msg_print, 0, STRING("gor_02_024"), 0, STRING("me"))
        CASE_LESS(340)
        CALL(evt_msg_print, 0, STRING("gor_02_025"), 0, STRING("me"))
        CASE_BETWEEN(340, 347)
        CALL(evt_msg_print, 0, STRING("mac_6_000"), 0, STRING("me"))
        CASE_EQUAL(348)
        CALL(evt_sub_load_progresstime, GSW(100), LW(0))
        IF_INT_EQUAL(GSWF(1220), 1)
            CALL(evt_msg_print, 0, STRING("mac_6_006"), 0, STRING("クリハカセ"))
            RETURN
        END_IF
        IF_INT_LESS(LW(0), 180000)
            SWITCH(LW(0))
                CASE_BETWEEN(0, 29999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_01"), 0, STRING("クリハカセ"))
                CASE_BETWEEN(30000, 59999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_02"), 0, STRING("クリハカセ"))
                CASE_BETWEEN(60000, 89999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_03"), 0, STRING("クリハカセ"))
                CASE_BETWEEN(90000, 119999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_04"), 0, STRING("クリハカセ"))
                CASE_BETWEEN(120000, 149999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_05"), 0, STRING("クリハカセ"))
                CASE_BETWEEN(150000, 179999)
                CALL(evt_msg_print, 0, STRING("mac_6_006_06"), 0, STRING("クリハカセ"))
            END_SWITCH
            ELSE
            CALL(evt_cam_letter_box_disable, 1)
            CALL(evt_cam_letter_box_onoff, 1, 0)
            WAIT_MS(1000)
            SPAWN_EVENT(PTR(kurihakase_after3minutes))
        END_IF
        CASE_LESS(354)
        IF_INT_EQUAL(GSWF(3884), 0)
            CALL(evt_pouch_check_item, 11, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 0, STRING("gor_02_026_01"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("gor_02_026"), 0, STRING("me"))
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_027"), 0, STRING("me"))
        END_IF
        CASE_LESS(374)
        CALL(evt_msg_print, 0, STRING("gor_02_028"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(403)
        IF_INT_EQUAL(GSWF(1249), 0)
            CALL(evt_msg_print, 0, STRING("gor_02_029"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("gor_02_029_01"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_N"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("gor_02_029_02"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            SET_VALUE(GSWF(1249), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_029_03"), 0, STRING("me"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(oyabun_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    RETURN
EVT_END()

EVT_BEGIN(oyabun_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(120)
        IF_INT_EQUAL(GSWF(1209), 0)
            CALL(evt_msg_print, 0, STRING("gor_02_000"), 0, STRING("me"))
            SET_VALUE(GSWF(1209), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_001"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(121)
        IF_INT_EQUAL(GSWF(1211), 1)
            CALL(evt_msg_print, 0, STRING("mac_2_040"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF(1210), 0)
            CALL(evt_snd_bgmoff, 1024)
            CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_DANGER3"))
            CALL(evt_snd_envoff, 1024)
            CALL(evt_msg_print, 0, STRING("mac_2_012"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
            END_INLINE_EVENT
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(800)
            CALL(evt_msg_print, 0, STRING("mac_2_013"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(800)
            CALL(evt_msg_print, 0, STRING("mac_2_014"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(800)
            CALL(evt_msg_print, 0, STRING("mac_2_015"), 0, STRING("me"))
            SET_VALUE(GSWF(1210), 1)
            ELSE
            CALL(evt_snd_envoff, 1024)
            CALL(evt_msg_print, 0, STRING("mac_2_016"), 0, STRING("me"))
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_2"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_2_017"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("mac_2_017_yesno"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("mac_2_018"))
            CALL(evt_msg_select, 0, STRING("mac_2_018_yesno"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_pouch_get_coin, LW(3))
                IF_INT_LESS(LW(3), 64)
                    CALL(evt_msg_print_add, 0, STRING("mac_2_020"))
                    CALL(evt_snd_envoff, 513)
                    CALL(evt_snd_envon, 288, 0)
                    RETURN
                    ELSE
                    CALL(evt_msg_continue)
                    CALL(evt_sub_status_gauge_force_open)
                    WAIT_MS(300)
                    CALL(evt_pouch_add_coin, -64)
                    LOOP(0)
                        CALL(evt_sub_status_gauge_check_update, LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(status_gauge_force_off)
                    WAIT_MS(300)
                    CALL(evt_msg_print, 0, STRING("mac_2_019"), 0, STRING("me"))
                    GOTO(38)
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("mac_2_021"))
                CALL(evt_snd_envoff, 513)
                CALL(evt_snd_envon, 288, 0)
                RETURN
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("mac_2_022"))
            CALL(evt_snd_envoff, 513)
            CALL(evt_snd_envon, 288, 0)
            RETURN
        END_IF
        LABEL(38)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_cam3d_evt_set, LW(0), 72, -753, LW(0), 52, -600, 500, 11)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_2_038"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("mac_2_025_yesno"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_continue)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(800)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("mac_2_039"))
        END_IF
        SET_VALUE(GSWF(1211), 1)
        CALL(evt_cam3d_evt_off, 500, 11)
        CASE_BETWEEN(122, 126)
        CALL(evt_msg_print, 0, STRING("gor_02_002"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_003"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_003_01"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_003_02"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_003_03"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_003_04"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_003_05"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_003_06"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(tozokudan1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    IF_INT_EQUAL(GSWF(1213), 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
        CALL(evt_npc_set_position, STRING("me"), -153, 0, -459)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        ELSE
        CALL(evt_npc_set_position, STRING("me"), -82, 0, 134)
    END_IF
    SET_VALUE(GW(10), 0)
    RETURN
EVT_END()

EVT_BEGIN(tozokudan1_talk)
    IF_INT_EQUAL(GSWF(1213), 0)
        IF_INT_EQUAL(GW(10), 1)
            CALL(evt_msg_print, 0, STRING("gor_02_006_01"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF(1212), 0)
            CALL(evt_msg_print, 0, STRING("gor_02_004"), 0, STRING("me"))
            SET_VALUE(GSWF(1212), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_005"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_select, 0, STRING("gor_02_005_select"))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_pouch_get_coin, LW(0))
            IF_INT_LESS(LW(0), 10)
                CALL(evt_msg_print_add, 0, STRING("gor_02_006_02"))
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("gor_02_006"))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
            CALL(evt_npc_move_position, STRING("me"), -102, 66, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
            CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
            SET_VALUE(GW(10), 1)
            CALL(evt_pouch_add_coin, -10)
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("gor_02_007"))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING("gor_02_008"))
            CALL(evt_npc_set_battle_info, STRING("me"), 5)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_npc_battle_start, STRING("me"))
            CALL(evt_npc_wait_battle_end)
            CALL(evt_mario_set_pos, -150, 0, 135)
            CALL(evt_party_set_pos, 0, -180, 0, 135)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_party_set_dir, 0, 90, -1)
            WAIT_MS(500)
            CALL(evt_npc_get_battle_result, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print, 0, STRING("gor_02_009"), 0, STRING("me"))
                CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
                CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
                CALL(evt_npc_set_ry, STRING("me"), 90)
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_MONBAN_ESCAPE1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_npc_move_position, STRING("me"), 150, 155, 0, FLOAT(240.0f), 0)
                CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
                CALL(evt_npc_set_position, STRING("me"), -153, 0, -459)
                SET_VALUE(GSWF(1213), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("gor_02_008_00"), 0, STRING("me"))
            END_IF
            CALL(evt_mario_key_onoff, 1)
        END_SWITCH
        ELSE
        SWITCH(GSW(0))
            CASE_LESS(56)
            CALL(evt_msg_print, 0, STRING("gor_02_010"), 0, STRING("me"))
            CASE_LESS(112)
            CALL(evt_msg_print, 0, STRING("gor_02_010_01"), 0, STRING("me"))
            CASE_LESS(164)
            CALL(evt_msg_print, 0, STRING("gor_02_010_02"), 0, STRING("me"))
            CASE_LESS(215)
            CALL(evt_msg_print, 0, STRING("gor_02_010_03"), 0, STRING("me"))
            CASE_LESS(261)
            CALL(evt_msg_print, 0, STRING("gor_02_010_04"), 0, STRING("me"))
            CASE_LESS(339)
            CALL(evt_msg_print, 0, STRING("gor_02_010_05"), 0, STRING("me"))
            CASE_LESS(373)
            CALL(evt_msg_print, 0, STRING("gor_02_010_06"), 0, STRING("me"))
            CASE_LESS(401)
            CALL(evt_msg_print, 0, STRING("gor_02_010_07"), 0, STRING("me"))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("gor_02_010_08"), 0, STRING("me"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(tozokudan2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    IF_INT_EQUAL(GSW(1420), 1)
        CALL(evt_npc_set_position, STRING("me"), 445, 0, 120)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), 10, 0, -460)
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_HOUSE_KEY, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, 49, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(tozokudan2_house_door)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_06_naka"), 1)
    SET_VALUE(LW(0), PTR(&door_data_06))
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
            SET_VALUE(GF(0), 0)
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
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
            SET_VALUE(GF(1), 0)
        END_INLINE_EVENT
    END_IF
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_06_naka"), 1)
    RETURN
EVT_END()

EVT_BEGIN(mario_yokeru)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("盗賊団２"), LW(10), LW(11), LW(12))
    SET_VALUE(LW(3), LW(10))
    SET_VALUE(LW(4), LW(10))
    SUBTRACT(LW(3), 20)
    ADD(LW(4), 20)
    IF_INT_LESS(LW(2), LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(0), LW(3))
            IF_INT_LESS_OR_EQUAL(LW(0), LW(4))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                    BEGIN_INLINE_EVENT
                        ADD(LW(2), 15)
                        CALL(evt_party_move_pos, 0, 415, LW(2), 300)
                        CALL(evt_party_set_dir, 0, 90, 200)
                    END_INLINE_EVENT
                    BEGIN_INLINE_EVENT
                        ADD(LW(2), 30)
                        CALL(evt_party_move_pos, 1, 415, LW(2), 300)
                        CALL(evt_party_set_dir, 1, 90, 200)
                    END_INLINE_EVENT
                    CALL(evt_mario_mov_pos, 415, LW(2), 300)
                    CALL(evt_mario_set_dir, 90, 200, 0)
                    ELSE
                    BEGIN_INLINE_EVENT
                        ADD(LW(2), 15)
                        CALL(evt_party_move_pos, 0, 480, LW(2), 300)
                        CALL(evt_party_set_dir, 0, 270, 200)
                    END_INLINE_EVENT
                    BEGIN_INLINE_EVENT
                        ADD(LW(2), 30)
                        CALL(evt_party_move_pos, 1, 480, LW(2), 300)
                        CALL(evt_party_set_dir, 1, 270, 200)
                    END_INLINE_EVENT
                    CALL(evt_mario_mov_pos, 480, LW(2), 300)
                    CALL(evt_mario_set_dir, 270, 200, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_unlock_mario)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("irai_01_03"), 0, STRING("盗賊団２"))
    CALL(evt_sub_get_coin, 20)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    SPAWN_EVENT(PTR(mario_yokeru))
    CALL(evt_npc_jump_position_nohit, STRING("盗賊団２"), 445, 8, 80, 300, 30)
    WAIT_MS(500)
    SPAWN_EVENT(PTR(tozokudan2_house_door))
    CALL(evt_npc_move_position, STRING("盗賊団２"), 445, -85, 0, FLOAT(60.0f), 0)
    SET_VALUE(GSW(1420), 0)
    SET_VALUE(GSWF(5375), 1)
    SET_VALUE(LW(0), PTR(&door_data_06))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_npc_flag_onoff, 0, STRING("盗賊団２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock)
    WAIT_MS(500)
    SPAWN_EVENT(PTR(evt_unlock_mario))
    RETURN
EVT_END()

EVT_BEGIN(evt_unlock_npc)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("irai_01_03"), 0, STRING("盗賊団２"))
    CALL(evt_sub_get_coin, 20)
    SPAWN_EVENT(PTR(mario_yokeru))
    CALL(evt_npc_jump_position_nohit, STRING("盗賊団２"), 445, 8, 80, 300, 30)
    WAIT_MS(500)
    CALL(evt_mobj_set_position, STRING("mobj_lock00"), 0, -1000, 0)
    SET_VALUE(GSWF(1219), 1)
    CALL(evt_npc_get_position, STRING("盗賊団２"), LW(0), LW(1), LW(2))
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_LOCK_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_UNLOCK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_pouch_remove_item, 49, LW(0))
    SPAWN_EVENT(PTR(tozokudan2_house_door))
    CALL(evt_npc_move_position, STRING("盗賊団２"), 445, -85, 0, FLOAT(60.0f), 0)
    SET_VALUE(GSW(1420), 0)
    SET_VALUE(GSWF(5375), 1)
    SET_VALUE(LW(0), PTR(&door_data_06))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_npc_flag_onoff, 0, STRING("盗賊団２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(tozokudan2_talk)
    IF_INT_EQUAL(GSW(1420), 1)
        CALL(evt_pouch_check_item, 49, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_msg_print, 0, STRING("irai_01_02"), 0, STRING("me"))
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL_CHILD(PTR(evt_unlock_npc))
            END_IF
            RETURN
            ELSE
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_01_00"), 0, STRING("me"))
                SET_VALUE(GSWF(5406), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_01_01"), 0, STRING("me"))
            END_IF
        END_IF
        RETURN
    END_IF
    SWITCH(GSW(0))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_011"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_011_01"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_011_02"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_011_03"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_011_04"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_011_05"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_011_06"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_011_07"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(tozokudan3_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    CALL(evt_npc_set_position, STRING("me"), 220, 0, -185)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

EVT_BEGIN(tozokudan3_talk)
    IF_INT_EQUAL(GSWF(1222), 0)
        CALL(evt_msg_print, 0, STRING("irai_oku_00"), 0, STRING("me"))
        SET_VALUE(GSWF(1222), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("irai_oku_01"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(sanders_open_door)
    IF_INT_EQUAL(GSWF(1216), 1)
        RETURN
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_move_position, STRING("me"), 0, 25, 0, FLOAT(60.0f), 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_MOBJ_UNLOCK1"), 0)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("me"), 50, 25, 0, FLOAT(60.0f), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
    CALL(set_talk_memory_ry)
    SET_VALUE(GSWF(1216), 1)
    CALL(evt_msg_print, 0, STRING("mac_4_056_01"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(sanders_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    SWITCH(GSW(0))
        CASE_LESS(224)
        CALL(evt_npc_set_position, STRING("me"), 0, 0, -50)
        CASE_LESS(227)
        IF_INT_EQUAL(GSWF(1216), 0)
            CALL(evt_npc_set_position, STRING("me"), 0, 0, -50)
            ELSE
            CALL(evt_npc_set_position, STRING("me"), 50, 0, -30)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(sanders_talk)
    SWITCH(GSW(0))
        CASE_LESS(224)
        CALL(evt_msg_print, 0, STRING("mac_4_049_01"), 0, STRING("me"))
        CALL_CHILD(PTR(sanders_open_door))
        CASE_EQUAL(224)
        IF_INT_EQUAL(GSWF(1214), 0)
            CALL(evt_msg_print, 0, STRING("mac_4_050"), 0, STRING("me"))
            CALL_CHILD(PTR(sanders_open_door))
            SET_VALUE(GSWF(1215), 1)
            ELSE
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print, 0, STRING("mac_4_051"), 0, STRING("me"))
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_msg_print_party, STRING("mac_4_052"))
            CALL(evt_party_run, 0)
            CALL(evt_msg_print, 0, STRING("mac_4_053"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("mac_4_054"), 0, STRING("me"))
            CALL(evt_msg_print_party, STRING("mac_4_055"))
            CALL(evt_msg_print, 0, STRING("mac_4_056"), 0, STRING("me"))
            CALL_CHILD(PTR(sanders_open_door))
            SET_VALUE(GSW(0), 225)
        END_IF
        CASE_EQUAL(225)
        CALL(evt_msg_print, 0, STRING("mac_4_057"), 0, STRING("me"))
        CASE_EQUAL(226)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
        CALL(evt_msg_print, 0, STRING("mac_4_077"), 0, STRING("me"))
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, -20, -10, FLOAT(120.0f))
            CALL(evt_party_set_dir, 0, 90, -1)
        END_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 20, -30, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_mario_set_dir, 90, 200, 0)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        WAIT_MS(500)
        CALL(evt_item_entry, STRING("item00"), 69, LW(0), LW(1), LW(2), 17, -1, 0)
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
        SUBTRACT(LW(3), 15)
        ADD(LW(4), 22)
        ADD(LW(5), 5)
        CALL(evt_item_set_position, STRING("item00"), LW(3), LW(4), LW(5))
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        SUBTRACT_FLOAT(LW(0), 15)
        ADD_FLOAT(LW(1), 2)
        ADD_FLOAT(LW(2), 5)
        CALL(evt_item_set_position, STRING("item00"), LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_4_078"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        BEGIN_INLINE_EVENT_GET_ID(GW(0))
            LOOP(0)
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                SUBTRACT_FLOAT(LW(0), 15)
                ADD_FLOAT(LW(1), 2)
                ADD_FLOAT(LW(2), 5)
                CALL(evt_item_set_position, STRING("item00"), LW(0), LW(1), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 225, 30)
        WAIT_MS(500)
        DELETE_EVENT(GW(0))
        CALL(evt_snd_bgmoff_f, 1024, 2000)
        CALL(evt_msg_print, 0, STRING("mac_4_079"), 0, STRING("me"))
        CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
        CALL(evt_item_entry, STRING("tegami"), 80, LW(0), LW(1), LW(2), 17, -1, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_item_get_position, STRING("tegami"), LW(2), LW(3), LW(4))
            ADD(LW(4), 10)
            SET_VALUE(LW(5), LW(2))
            ADD(LW(5), 30)
            CALL(evt_sub_intpl_msec_init, 0, 0, 1000, 300)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            SET_FLOAT_VALUE(LW(6), LW(5))
            SUBTRACT_FLOAT(LW(6), LW(2))
            MULTIPLY_FLOAT(LW(6), LW(0))
            ADD_FLOAT(LW(6), LW(2))
            CALL(evt_item_set_position, STRING("tegami"), LW(6), LW(3), LW(4))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("me"), 90)
        WAIT_MS(500)
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_ORGEL1"))
        CALL(evt_msg_print, 0, STRING("mac_4_080"), 0, STRING("me"))
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_081"), 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_082"), 0, STRING("me"))
        CALL(evt_item_set_position, STRING("tegami"), 0, -1000, 0)
        WAIT_MS(300)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_move_position, STRING("me"), 0, -140, 0, FLOAT(60.0f), 0)
        SPAWN_EVENT(PTR(evt_gor_door_open_sanders))
        CALL(evt_npc_move_position, STRING("me"), 0, -90, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), 0, -193, 0, FLOAT(60.0f), 0)
        WAIT_MS(3000)
        CALL(evt_mario_set_dir, 270, 200, 0)
        WAIT_MS(2000)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(3000)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_083"), 0, 0)
        WAIT_MS(2000)
        SPAWN_EVENT(PTR(evt_gor_door_close_sanders))
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CALL(evt_npc_move_position, STRING("me"), 0, -80, 0, FLOAT(60.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), 50, -30, 0, FLOAT(60.0f), 0)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
        WAIT_MS(500)
        CALL(evt_snd_bgmoff_f, 513, 2000)
        CALL(evt_msg_print, 0, STRING("mac_4_084"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_4_085"), 0, STRING("me"))
        CALL(evt_msg_print_party, STRING("mac_4_086"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CALL(evt_party_stop, 1)
        CALL(evt_mario_set_party_pos, 1, 15, LW(0), LW(1), LW(2))
        CALL(evt_party_run, 1)
        CALL(evt_pouch_remove_item, 69, LW(0))
        CALL(evt_party_run, 0)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
        SET_VALUE(GSW(0), 227)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuribo4_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    CALL(evt_npc_set_position, STRING("me"), -370, 0, 130)
    RETURN
EVT_END()

EVT_BEGIN(kuribo4_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(70.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

static s32 item_tbl[] = {
    ITEM_BOX, -1,
};

EVT_BEGIN(kuribo4_talk)
    IF_INT_EQUAL(GSW(1420), 2)
        IF_INT_EQUAL(GSWF(5405), 1)
            CALL(evt_pouch_check_item, 95, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(evt_msg_print, 0, STRING("irai_02_03"), 0, STRING("me"))
                CALL(evt_win_item_select, 0, PTR(item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_msg_print, 0, STRING("irai_02_04"), 0, STRING("me"))
                    CALL(evt_pouch_remove_item, 95, LW(0))
                    SET_VALUE(GSWF(5406), 1)
                END_IF
                RETURN
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSW(1420), 13)
        IF_INT_EQUAL(GSWF(5406), 1)
            CALL(evt_pouch_check_item, 107, LW(0))
            IF_INT_GREATER(LW(0), 0)
                CALL(iri_13_make_item_tbl)
                IF_INT_EQUAL(GSWF(5407), 0)
                    CALL(evt_msg_print, 0, STRING("irai_13_001"), 0, 0)
                    CALL(evt_mario_set_pose, STRING("M_N_5B"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_N_C"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print, 0, STRING("irai_13_002"), 0, 0)
                    CALL(iri_13_update_item_tbl, LW(1), LW(0))
                    LOOP(0)
                        CALL(evt_win_item_select, 0, LW(1), LW(0), 0)
                        IF_INT_NOT_EQUAL(LW(0), 107)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_pouch_remove_item, 107, LW(0))
                        CALL(iri_13_update_item_tbl, LW(1), LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_msg_print, 0, STRING("irai_13_003"), 0, 0)
                    END_LOOP
                    SET_VALUE(GSWF(5407), 1)
                    ELSE
                    LOOP(0)
                        CALL(iri_13_update_item_tbl, LW(1), LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_msg_print, 0, STRING("irai_13_005"), 0, 0)
                        CALL(evt_win_item_select, 0, LW(1), LW(0), 0)
                        IF_INT_NOT_EQUAL(LW(0), 107)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_pouch_remove_item, 107, LW(0))
                    END_LOOP
                END_IF
                CALL(evt_pouch_check_item, 107, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_13_004"), 0, 0)
                END_IF
                CALL(iri_13_free_item_tbl)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW(0))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_030_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_030_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_030_02_01"), 0, STRING("me"))
        CASE_LESS(175)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03"), 0, STRING("me"))
        CASE_LESS(176)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03_01"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03_02"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_030_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_030_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_030_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_030_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_030_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(chusan3_init)
    RETURN
EVT_END()

EVT_BEGIN(chusan3_talk)
    SWITCH(GSW(0))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_031_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_031_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_031_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_031_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_031_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_031_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_031_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_031_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_031_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(roten3_init)
    RETURN
EVT_END()

EVT_BEGIN(roten3_talk)
    IF_INT_EQUAL(GSW(1420), 3)
        SET_VALUE(LW(1), 0)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(3), 0)
        CALL(evt_msg_print, 0, STRING("irai_03_00"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("irai_03_00_s"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("irai_03_02"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("irai_03_01"))
            RETURN
        END_IF
        CALL(evt_msg_select, 0, STRING("irai_03_02_s"))
        IF_INT_EQUAL(LW(0), 1)
            SET_VALUE(LW(1), 1)
        END_IF
        CALL(evt_msg_print_add, 0, STRING("irai_03_03"))
        CALL(evt_msg_select, 0, STRING("irai_03_03_s"))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(2), 1)
        END_IF
        CALL(evt_msg_print_add, 0, STRING("irai_03_04"))
        CALL(evt_msg_select, 0, STRING("irai_03_04_s"))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(3), 1)
        END_IF
        CALL(evt_msg_print_add, 0, STRING("irai_03_05"))
        CALL(evt_sub_get_coin, 20)
        SET_VALUE(GSW(1420), 0)
        SET_VALUE(GSWF(5377), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        RETURN
    END_IF
    SWITCH(GSW(0))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_032_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_032_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_032_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_032_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_032_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_032_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_032_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_032_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_032_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borodo1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    SWITCH(GSW(0))
        CASE_BETWEEN(215, 261)
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_position, STRING("me"), 475, 0, 275)
#else
        CALL(evt_npc_set_position, STRING("me"), 550, 0, 275)
#endif
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 90, 0, 150)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borodo1_regl)
    SWITCH(GSW(0))
        CASE_BETWEEN(215, 261)
        CASE_DEFAULT
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(70.0f), FLOAT(30.0f), 0)
        CALL(urouro_main_func, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borodo1_talk)
    SWITCH(GSW(0))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_033_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_033_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_033_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_033_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_033_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_033_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_033_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_033_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_033_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(borodo2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    IF_INT_EQUAL(GSW(1420), 4)
        IF_INT_EQUAL(GSWF(5406), 1)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
            RETURN
        END_IF
        CALL(evt_npc_set_position, STRING("me"), -475, 0, -180)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), -475, 0, -180)
    RETURN
EVT_END()

static EVT_BEGIN(borodo2_escape)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_npc_set_ry, STRING("me"), 180)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("me"), -475, -85, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), -530, -85, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), -530, 20, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(borodo2_talk)
    IF_INT_EQUAL(GSW(1420), 4)
        IF_INT_EQUAL(GSWF(5405), 1)
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_04_003"), 0, STRING("me"))
                SET_VALUE(GSWF(5406), 1)
                SPAWN_EVENT(PTR(borodo2_escape))
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW(0))
        CASE_LESS(23)
        CALL(evt_msg_print, 0, STRING("gor_02_033_00_2"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_033_01_2"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_02_033_02_2"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_033_03_2"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_033_04_2"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_033_05_2"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_033_06_2"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_033_07_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_033_08_2"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuragarisan_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741824)
    RETURN
EVT_END()

EVT_BEGIN(kuragarisan_write_name)
    CALL(evt_npc_get_position, STRING("me"), LW(13), LW(14), LW(15))
    CALL(evt_npc_get_dir, STRING("me"), LW(12))
    CALL(evt_party_stop, 0)
    CALL(evt_party_outofscreen, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_party_set_pos, 0, -516, 0, -146)
    END_IF
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(2), -320)
            CALL(evt_party_move_pos2, 0, -480, -335, FLOAT(60.0f))
        END_IF
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 40)
        SUBTRACT(LW(2), 40)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(10000)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_3_011"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_3_012"), 0, STRING("me"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_3_013"), 0, STRING("me"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_3_014"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_3_015"), 0, STRING("me"))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(1), 0)
            DEBUG_PUT_MSG(STRING("タイムアウトしました"))
            DELETE_EVENT(LW(10))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 40)
            SUBTRACT(LW(2), 40)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_party_set_dir, 0, 90, 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 1073741856)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 5)
    SUBTRACT(LW(2), 19)
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_KURAGARI_PEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1A"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1B"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1C"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("mac_3_016"), PTR(kuragarisan_msgcb), STRING("me"))
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL(evt_pouch_get_yoshiname, LW(1))
        CALL(evt_msg_print_insert, 0, STRING("mac_3_017_yos"), 0, STRING("party"), LW(1))
        ELSE
        CALL(evt_msg_print_party, STRING("mac_3_017"))
    END_IF
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_party, LW(3))
    SWITCH(LW(3))
        CASE_EQUAL(1)
        SET_VALUE(LW(3), 22)
        CASE_EQUAL(2)
        SET_VALUE(LW(3), 23)
        CASE_EQUAL(4)
        SET_VALUE(LW(3), 15)
        CASE_EQUAL(5)
        SET_VALUE(LW(3), 31)
        CASE_DEFAULT
        SET_VALUE(LW(3), 0)
    END_SWITCH
    ADD(LW(0), 5)
    SUBTRACT(LW(2), LW(3))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_KURAGARI_PEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1A"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1B"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1C"))
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("mac_3_018"))
    CALL(evt_msg_print, 0, STRING("mac_3_019"), 0, STRING("me"))
    CALL(evt_npc_move_position, STRING("me"), LW(13), LW(15), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING("me"), LW(12))
    CALL(evt_npc_status_onoff, 0, STRING("me"), 2)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741856)
    SET_VALUE(GSW(0), 176)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(kuragarisan_talk)
    SWITCH(GSW(0))
        CASE_EQUAL(175)
        IF_INT_EQUAL(GSWF(1341), 0)
            CALL(evt_msg_print, 0, STRING("gor_02_034_03"), 0, STRING("me"))
            ELSE
            CALL_CHILD(PTR(kuragarisan_write_name))
        END_IF
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_02_034_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_02_034_01"), 0, STRING("me"))
        CASE_LESS(172)
        CALL(evt_msg_print, 0, STRING("gor_02_034_02"), 0, STRING("me"))
        CASE_LESS(176)
        CALL(evt_msg_print, 0, STRING("gor_02_034_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_034_04"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_02_034_05"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_02_034_06"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_02_034_07"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_02_034_08"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_02_034_09"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(401)
        CALL(evt_msg_print, 0, STRING("gor_02_034_10"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(luigi_init)
    SWITCH(GSW(0))
        CASE_BETWEEN(56, 111)
        CALL(evt_npc_set_position, STRING("me"), -400, 0, 0)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(luigi_talk)
    CALL_CHILD(PTR(luigi_gor_first_talk))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"パワーアップ屋"*/"\x83\x70\x83\x8F\x81\x5B\x83\x41\x83\x62\x83\x76\x89\xAE", // name
        0x80600, // flags
        0, // reactionFlags
        powerupya_init, // initEvent
        NULL, // regularEvent
        powerupya_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A", // name
        0x80600, // flags
        0, // reactionFlags
        kurihakase_init, // initEvent
        NULL, // regularEvent
        kurihakase_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"情報屋完璧"*/"\x8F\xEE\x95\xF1\x89\xAE\x8A\xAE\xE0\xF8", // name
        0x80600, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        johoyaperfect_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"盗賊団親分"*/"\x93\x90\x91\xAF\x92\x63\x90\x65\x95\xAA", // name
        0x80600, // flags
        0, // reactionFlags
        oyabun_init, // initEvent
        NULL, // regularEvent
        oyabun_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"盗賊団１"*/"\x93\x90\x91\xAF\x92\x63\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        tozokudan1_init, // initEvent
        NULL, // regularEvent
        tozokudan1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"盗賊団２"*/"\x93\x90\x91\xAF\x92\x63\x82\x51", // name
        0x80600, // flags
        0, // reactionFlags
        tozokudan2_init, // initEvent
        NULL, // regularEvent
        tozokudan2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"盗賊団３"*/"\x93\x90\x91\xAF\x92\x63\x82\x52", // name
        0x80600, // flags
        0, // reactionFlags
        tozokudan3_init, // initEvent
        NULL, // regularEvent
        tozokudan3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58", // name
        0x80600, // flags
        0, // reactionFlags
        sanders_init, // initEvent
        NULL, // regularEvent
        sanders_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー４"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x53", // name
        0x80600, // flags
        0, // reactionFlags
        kuribo4_init, // initEvent
        kuribo4_regl, // regularEvent
        kuribo4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チューさん３"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x52", // name
        0x80600, // flags
        0, // reactionFlags
        chusan3_init, // initEvent
        NULL, // regularEvent
        chusan3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロテン３"*/"\x83\x8D\x83\x65\x83\x93\x82\x52", // name
        0x80600, // flags
        0, // reactionFlags
        roten3_init, // initEvent
        NULL, // regularEvent
        roten3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボロドー１"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        borodo1_init, // initEvent
        borodo1_regl, // regularEvent
        borodo1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボロドー２"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x82\x51", // name
        0x80600, // flags
        0, // reactionFlags
        borodo2_init, // initEvent
        NULL, // regularEvent
        borodo2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クラガリさん"*/"\x83\x4E\x83\x89\x83\x4B\x83\x8A\x82\xB3\x82\xF1", // name
        0x80600, // flags
        0, // reactionFlags
        kuragarisan_init, // initEvent
        NULL, // regularEvent
        kuragarisan_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57", // name
        0x80600, // flags
        0, // reactionFlags
        luigi_init, // initEvent
        NULL, // regularEvent
        luigi_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_door_01_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_01_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_01)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01B"), LW(2), 0, 0)
    RETURN
EVT_END()

static const char* door_data_01_npc[] = {
    /*"パワーアップ屋"*/"\x83\x70\x83\x8F\x81\x5B\x83\x41\x83\x62\x83\x76\x89\xAE",
    NULL,
};

static EVT_BEGIN(evt_door_01_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL_CHILD(PTR(mapball_init))
    CALL(evt_item_delete_check, STRING("item_star1"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star1"), LW(0), LW(1), LW(2))
        ADD(LW(1), -1000)
        CALL(evt_item_set_position, STRING("item_star1"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_item_delete_check, STRING("item_star2"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star2"), LW(0), LW(1), LW(2))
        ADD(LW(1), -1000)
        CALL(evt_item_set_position, STRING("item_star2"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_item_delete_check, STRING("item_star3"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star3"), LW(0), LW(1), LW(2))
        ADD(LW(1), -1000)
        CALL(evt_item_set_position, STRING("item_star3"), LW(0), LW(1), LW(2))
    END_IF
    SET_VALUE(GSWF(1250), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_01_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    CALL(evt_item_delete_check, STRING("item_star1"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star1"), LW(0), LW(1), LW(2))
        ADD(LW(1), 1000)
        CALL(evt_item_set_position, STRING("item_star1"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_item_delete_check, STRING("item_star2"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star2"), LW(0), LW(1), LW(2))
        ADD(LW(1), 1000)
        CALL(evt_item_set_position, STRING("item_star2"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_item_delete_check, STRING("item_star3"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_star3"), LW(0), LW(1), LW(2))
        ADD(LW(1), 1000)
        CALL(evt_item_set_position, STRING("item_star3"), LW(0), LW(1), LW(2))
    END_IF
    RETURN
EVT_END()

static DoorParam door_data_01 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_01_doa", // mapobj
    "A_i01_doa02", // hitIn
    "A_i01_doa01", // hitOut
    "S_ie_01_soto", // outside
    NULL, // outsideHit
    "S_ie_01_naka", // inside
    "A_ie_01C", // insideHit
    evt_door_01_00, // evtIn
    evt_door_01_01, // evtOut
    evt_wall_01, // evtWall
    door_data_01_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_01_open, // evtOpen
    evt_door_01_close, // evtClose
};

static EVT_BEGIN(evt_door_02_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_02)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02B"), LW(2), 0, 0)
    RETURN
EVT_END()

static const char* door_data_02_npc[] = {
    /*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A",
    NULL,
};

EVT_BEGIN(evt_door_02_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    SWITCH(GSW(0))
        CASE_OR_EQUAL(18)
        CASE_OR_EQUAL(64)
        CASE_OR_EQUAL(120)
        CASE_OR_EQUAL(174)
        CASE_OR_EQUAL(221)
        CASE_OR_EQUAL(271)
        CASE_OR_EQUAL(347)
        CALL(evt_snd_envoff, 18432)
        CALL(evt_snd_env_lpf, 0, 800)
        CASE_END
        CASE_EQUAL(348)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_FRANKURI1"))
        CALL(evt_snd_envoff, 18432)
        CALL(evt_snd_env_lpf, 0, 800)
        CASE_DEFAULT
        IF_INT_NOT_EQUAL(GSW(0), 402)
            CALL(evt_snd_bgmoff, 18432)
            CALL(evt_snd_envoff, 18432)
            CALL(evt_snd_env_lpf, 0, 800)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(GSW(0), 8)
        SPAWN_EVENT(PTR(kurihakase_first_talk))
    END_IF
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 180)
    SWITCH(GSW(0))
        CASE_BETWEEN(56, 70)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
        CASE_BETWEEN(164, 177)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
        CASE_BETWEEN(339, 351)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    END_SWITCH
    IF_INT_NOT_EQUAL(GSW(0), 18)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tizu_on"), 1)
    END_IF
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_memo"), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
    SWITCH(GSW(0))
        CASE_OR_EQUAL(347)
        CASE_OR_EQUAL(348)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
        CALL(evt_snd_envon, 160, 0)
        CALL(evt_snd_env_lpf, 0, 0)
        CASE_END
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 160, 0)
        CALL(evt_snd_envon, 160, 0)
        CALL(evt_snd_env_lpf, 0, 0)
    END_SWITCH
    IF_INT_EQUAL(GSWF(1220), 1)
        SET_VALUE(GSWF(1220), 0)
    END_IF
    IF_INT_EQUAL(GSW(0), 9)
        SPAWN_EVENT(PTR(kurihakase_wall_remove))
        GOTO(100)
    END_IF
    IF_INT_EQUAL(GSW(0), 19)
        SPAWN_EVENT(PTR(kurihakase_lecture_badge))
        GOTO(100)
    END_IF
    LABEL(100)
    WAIT_MS(500)
    CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
    CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
    RETURN
EVT_END()

static DoorParam door_data_02 = {
    0, // flags
    3, // type
    1, // se
    "S_i02_doa", // mapobj
    "A_ie_02_doa02", // hitIn
    "A_ie_02_doa01", // hitOut
    "S_ie_02_soto", // outside
    NULL, // outsideHit
    "S_ie_02_naka", // inside
    "A_ie_02C", // insideHit
    evt_door_02_00, // evtIn
    evt_door_02_01, // evtOut
    evt_wall_02, // evtWall
    door_data_02_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_02_open, // evtOpen
    evt_door_02_close, // evtClose
};

EVT_BEGIN(evt_door_02_entry)
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 8)
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doam"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_03_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doam"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_03)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03B"), LW(2), 0, 0)
    RETURN
EVT_END()

static const char* door_data_03_npc[] = {
    /*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58",
    NULL,
};

static const char* door_data_03_mobj[] = {
    "pblk2",
    NULL,
};

static EVT_BEGIN(evt_door_03_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    IF_INT_EQUAL(GSW(0), 225)
        IF_INT_EQUAL(GSWF(1217), 0)
            CALL(evt_door_end_wait)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            ADD(LW(0), 40)
            CALL(evt_party_move_pos2, 0, LW(0), LW(2), 60)
            CALL(evt_party_set_dir, 0, 270, -1)
            CALL(evt_mario_set_dir, 90, 300, 0)
            CALL(evt_msg_print_party, STRING("mac_4_058"))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1_2"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_I_Z"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print_party, STRING("mac_4_059"))
            SET_VALUE(GSWF(1217), 1)
            CALL(evt_mario_key_onoff, 1)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(GSW(0), 227)
        RETURN
    END_IF
    CALL(evt_door_end_wait)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_party_stop, 1)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), 39)
        CALL(evt_party_set_pos, 1, -17, 0, 57)
        CALL(evt_party_move_pos2, 1, -26, 108, FLOAT(120.0f))
    END_IF
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_msg_print, 0, STRING("mac_4_087"), 0, STRING("extparty"))
    CALL(evt_party_set_dir, 1, 270, -1)
    CALL(evt_party_move_pos2, 1, -36, 140, FLOAT(120.0f))
    CALL(evt_party_move_pos2, 1, -165, 140, FLOAT(120.0f))
    CALL(evt_mario_clear_party, 1)
    SET_VALUE(GSW(0), 228)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static DoorParam door_data_03 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_doam", // mapobj
    "A_ie_03_doa_03", // hitIn
    "A_ie_03_doa_01", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    "A_ie_03C", // insideHit
    evt_door_03_00, // evtIn
    evt_door_03_01, // evtOut
    evt_wall_03, // evtWall
    door_data_03_npc, // npcs
    door_data_03_mobj, // mobjs
    NULL, // items
    evt_door_03_open, // evtOpen
    evt_door_03_close, // evtClose
};

EVT_BEGIN(evt_door_sanders_open)
    IF_INT_EQUAL(GSWF(1216), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("gor_02_012"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SPAWN_EVENT(PTR(evt_door_open))
    RETURN
EVT_END()

EVT_BEGIN(evt_door_sanders_close)
    IF_INT_EQUAL(GSWF(1216), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("gor_02_012"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SPAWN_EVENT(PTR(evt_door_close))
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_entry)
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
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_door_sanders_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_sanders_open), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_caseopen)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("A_i03_rol"), 1, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.009765625f), 1000)
    END_INLINE_EVENT
    CALL(evt_mario_wait_landon)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_ROLL_DOWN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_ROLL_DOWN2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_door_open_roll))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_03_roll_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("S_i01_dan_mC"), LW(1), LW(2), LW(3))
        SUBTRACT(LW(2), 40)
        ADD(LW(3), 30)
        LOOP(3)
            CALL(evt_sub_random, 4, LW(4))
            SUBTRACT(LW(4), 2)
            ADD(LW(4), LW(1))
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 10, LW(4), LW(2), LW(3), FLOAT(1.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set_xyz, 65, 10, -55, 0, 150, 11)
    WAIT_MS(150)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_hitobj_onoff, STRING("A_i03_rol"), 1, 1)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_mario_mov_pos2, 65, -55, FLOAT(120.0f))
    IF_INT_EQUAL(GSWF(1216), 0)
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_mario_cancel_roll_motion)
    END_IF
    IF_INT_EQUAL(GSWF(1216), 0)
        CALL(evt_npc_set_ry, STRING("サンダース"), 90)
    END_IF
    IF_INT_EQUAL(GSWF(1216), 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dispdir, 0, 0)
    END_IF
    IF_INT_EQUAL(GSWF(1216), 0)
        CALL(evt_msg_print, 0, STRING("mac_4_049"), 0, STRING("サンダース"))
    END_IF
    CALL(evt_cam_ctrl_onoff, 4, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, 0, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_03_roll_close)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_roll)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03B"), LW(2), 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_03_roll = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_doam", // mapobj
    NULL, // hitIn
    "A_i03_rol", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    "A_ie_03C", // insideHit
    evt_door_03_00, // evtIn
    evt_door_03_01, // evtOut
    evt_wall_03_roll, // evtWall
    door_data_03_npc, // npcs
    door_data_03_mobj, // mobjs
    NULL, // items
    evt_door_03_roll_open, // evtOpen
    evt_door_03_roll_close, // evtClose
};

EVT_BEGIN(evt_door_03_roll_entry)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, 0, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_04)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i04_msk"), 4096)
    END_IF
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_B"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_ura_B"), LW(3), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
#ifdef VERSION_G8ME01
    CALL(evt_hitobj_onoff, STRING("polySurface1317"), 1, 0)
#endif
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
#ifdef VERSION_G8ME01
    CALL(evt_hitobj_onoff, STRING("polySurface1317"), 1, 1)
#endif
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static const char* door_data_04_npc[] = {
    /*"盗賊団３"*/"\x93\x90\x91\xAF\x92\x63\x82\x52",
    NULL,
};

static const char* door_data_04_item[] = {
    "item_star4",
    "item_star5",
    "item_star6",
    NULL,
};

static DoorParam door_data_04 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_04_doa", // mapobj
    "A_ie_04_doa02", // hitIn
    "A_ie_04_doa01", // hitOut
    "S_ie_04_soto", // outside
    NULL, // outsideHit
    "S_ie_04_naka", // inside
    "A_ie_04C", // insideHit
    evt_door_04_00, // evtIn
    evt_door_04_01, // evtOut
    evt_wall_04, // evtWall
    door_data_04_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_04_open, // evtOpen
    evt_door_04_close, // evtClose
};

EVT_BEGIN(evt_door_04_junbi)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("gor_02_035"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_04_00_ura)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa_02"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_04_01_ura)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa_02"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_04_ura)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_ura_B"), LW(3), 0, 0)
    RETURN
EVT_END()

DoorParam door_data_04_ura = {
    0, // flags
    3, // type
    1, // se
    "S_ie_04_doa_02", // mapobj
    "A_ie_04_doa_02_01", // hitIn
    "A_ie_04_doa_02_02", // hitOut
    "S_ie_04_soto", // outside
    NULL, // outsideHit
    "S_ie_04_naka", // inside
    "A_ie_04C", // insideHit
    evt_door_04_00_ura, // evtIn
    evt_door_04_01_ura, // evtOut
    evt_wall_04_ura, // evtWall
    door_data_04_npc, // npcs
    NULL, // mobjs
    door_data_04_item, // items
    evt_door_04_open, // evtOpen
    evt_door_04_close, // evtClose
};

EVT_BEGIN(evt_door_05_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_05)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05sB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05uB"), LW(3), 0, 0)
    RETURN
EVT_END()

const char* door_data_05_npc[] = {
    /*"盗賊団親分"*/"\x93\x90\x91\xAF\x92\x63\x90\x65\x95\xAA",
    /*"盗賊団１"*/"\x93\x90\x91\xAF\x92\x63\x82\x50",
    /*"盗賊団２"*/"\x93\x90\x91\xAF\x92\x63\x82\x51",
    NULL,
};

const char* door_data_05_npc_01[] = {
    /*"盗賊団親分"*/"\x93\x90\x91\xAF\x92\x63\x90\x65\x95\xAA",
    /*"盗賊団１"*/"\x93\x90\x91\xAF\x92\x63\x82\x50",
    NULL,
};

EVT_BEGIN(evt_door_05_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_05_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam door_data_05 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_05_doa", // mapobj
    "A_ie_05_doa02", // hitIn
    "A_ie_05_doa01", // hitOut
    "S_ie_05_soto", // outside
    NULL, // outsideHit
    "S_ie_05_naka", // inside
    "A_ie_05C", // insideHit
    evt_door_05_00, // evtIn
    NULL, // evtOut
    evt_wall_05, // evtWall
    (const char**)door_data_05_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_05_open, // evtOpen
    evt_door_05_close, // evtClose
};

EVT_BEGIN(evt_door_06_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_06_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_06_00)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            ELSE
            WAIT_MS(200)
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_i06mB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        CALL(evt_sub_intpl_msec_init, 2, 0, -180, 500)
        ELSE
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, -180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i06mB"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_06_01)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mapobj_get_position, STRING("S_i06nB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 0, -180, 500)
        ELSE
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, -180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i06nB"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_06_02)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(200)
            ELSE
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_i06oB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 0, -180, 500)
        ELSE
        CALL(evt_sub_intpl_msec_init, 2, -180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i06oB"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_06)
    IF_INT_EQUAL(LW(6), 0)
        SPAWN_EVENT(PTR(evt_wall_06_00))
        SPAWN_EVENT(PTR(evt_wall_06_01))
        SPAWN_EVENT(PTR(evt_wall_06_02))
    END_IF
    RETURN
EVT_END()

const char* door_data_06_npc[] = {
    /*"盗賊団２"*/"\x93\x90\x91\xAF\x92\x63\x82\x51",
    NULL,
};

const char* door_data_06_npc_01[] = {
    NULL,
};

EVT_BEGIN(evt_door_06_open)
    SPAWN_EVENT(PTR(gor_02_hitobj_off))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_06_close)
    SPAWN_EVENT(PTR(gor_02_hitobj_on))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam door_data_06 = {
    2, // flags
    3, // type
    1, // se
    "S_ie_06_doa", // mapobj
    "A_ie_06_doa02", // hitIn
    "A_ie_06_doa01", // hitOut
    "S_ie_06_soto", // outside
    NULL, // outsideHit
    "S_ie_06_naka", // inside
    "A_ie_06C", // insideHit
    evt_door_06_00, // evtIn
    NULL, // evtOut
    evt_wall_06, // evtWall
    (const char**)door_data_06_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_06_open, // evtOpen
    evt_door_06_close, // evtClose
};

EVT_BEGIN(evt_gor_door_open_sanders)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 2, 0, 100, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 2, 100, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_open_sub)
    CALL_CHILD(PTR(evt_gor_door_open_sanders))
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 2, 0, 90, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_DOWN1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

static USER_FUNC(enzan_set_i_f) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetValue(event, var, evtGetFloat(event, args[1]));
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_set_f_i) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetFloat(event, var, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_set_bgtpl) {
    char path[64];
    const char* name = (const char*)evtGetValue(event, event->args[0]);

    if (name == NULL) {
        name = gp->currentMapName;
    }
    sprintf(path, "./b/%s.tpl", name);
    bgReEntry(path);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bgScrollControll) {
    if (evtGetValue(event, event->args[0]) == 0) {
        bgTransOffsetOff();
    } else {
        bgTransOffsetOn();
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(stonechangertn) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);

    if (!strcmp(name, "S_i01_sui_ao")) {
        mapGrpFlagOff("S_i01_sui_ao", 1);
        mapGrpFlagOn("S_i01_sui_aka", 1);
    } else if (!strcmp(name, "S_i01_sui_aka")) {
        mapGrpFlagOn("S_i01_sui_ao", 1);
        mapGrpFlagOff("S_i01_sui_aka", 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(get_mario_dir_target) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], marioGetPtr()->displayDirectionTarget);
    return EVT_RETURN_DONE;
}

USER_FUNC(status_gauge_force_off) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(set_talk_memory_ry) {
    NpcEntry* npc = npcNameToPtr("\203T\203\223\203_\201[\203X");

    npc->savedDirection = 270.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_13_make_item_tbl) {
    s32 count = pouchCheckItem(0x6B);

    if (count > 0) {
        iri_13_item_tbl = _mapAlloc((count + 1) * sizeof(s32));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_13_update_item_tbl) {
    EvtWord* args = event->args;
    s32 count = pouchCheckItem(0x6B);
    s32 i;

    if (count > 0) {
        for (i = 0; i < count; i++) {
            iri_13_item_tbl[i] = 0x6B;
        }
        iri_13_item_tbl[i] = -1;
    }
    evtSetValue(event, args[0], (EvtWord)iri_13_item_tbl);
    evtSetValue(event, args[1], count);
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_13_free_item_tbl) {
    _mapFree(iri_13_item_tbl);
    iri_13_item_tbl = NULL;
    return EVT_RETURN_DONE;
}

void kuragarisan_msgcb(s32 type, void* param) {
    u8* msg = *(u8**)param;
    NpcEntry* npc;

    switch (type) {
        case 3:
            if (*(u16*)(msg + 8) == 1) {
                npc = npcNameToPtr("\203N\203\211\203K\203\212\202\263\202\361");
                npc->direction = 90.0f;
            }
            break;
    }
}

USER_FUNC(evt_gor_door_in_info) {
    EvtWord* args = event->args;
    Vec outPos = {0.0f, 0.0f, 0.0f};
    Vec inPos = {0.0f, 0.0f, 0.0f};

    door_position_sub(3, "S_ie_03_doao", "A_ie_03_doa_02", "A_ie_03_doa_04", &outPos, &inPos, 0);
    evtSetValue(event, args[0], outPos.x);
    evtSetValue(event, args[1], outPos.y);
    evtSetValue(event, args[2], outPos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_gor_door_out_info) {
    EvtWord* args = event->args;
    Vec outPos = {0.0f, 0.0f, 0.0f};
    Vec inPos = {0.0f, 0.0f, 0.0f};

    door_position_sub(3, "S_ie_03_doao", "A_ie_03_doa_02", "A_ie_03_doa_04", &outPos, &inPos, 0);
    evtSetValue(event, args[0], inPos.x);
    evtSetValue(event, args[1], inPos.y);
    evtSetValue(event, args[2], inPos.z);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_StatusOpen) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_StatusClose) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_depthPlaneOffset) {
    EvtWord* args = event->args;
    ImageEntry* img = imgNameToPtr((const char*)evtGetValue(event, args[0]), gp->inBattle != 0);

    img->offsetZ = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(lecture) {
    if (isFirstCall) {
        winLectureOn();
    }
    if (winLectureCheck()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(monosiri_check) {
    EvtWord* args = event->args;
    s32 seq = evtGetValue(event, GSW(0));
    u32 i;
    s32 j;
    MonosiriFollow* f = FOLLOW;

    for (i = 0; i < sizeof(follow) / sizeof(MonosiriFollow); i++, f++) {
        if (seq < f->seq) {
            break;
        }
        for (j = 0; j < f->count; j++) {
            if (!swGet(f->list[j] + 0x117A)) {
                evtSetValue(event, args[0], 1);
                evtSetValue(event, args[1], f->list[j]);
                return EVT_RETURN_DONE;
            }
        }
    }
    if (swGet(0x4BD) && !swGet(0x1181)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], 7);
        return EVT_RETURN_DONE;
    }
    if (swGet(0x13DD) && !swGet(0x1225)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], 0xAB);
        return EVT_RETURN_DONE;
    }
    if (swGet(0x8B2) && !swGet(0x11C6)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], 0x4C);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[0], 0);
    evtSetValue(event, args[1], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(monosiri_on) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    TattleEntry* tattle = battleGetUnitMonosiriPtr(id);

    swSet(id + 0x117A);
    evtSetValue(event, args[1], (EvtWord)msgSearch(tattle->enemyTag));
    return EVT_RETURN_DONE;
}

USER_FUNC(camera_front_check) {
    EvtWord* args = event->args;
    f32 angle = reviseAngle((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);

#ifdef VERSION_G8ME01
    if (marioGetPtr()->position.x < -300.0f) {
        evtSetValue(event, *args, 0);
        return EVT_RETURN_DONE;
    }
#endif
    if (angle >= 0.0f && angle < 90.0f) {
        evtSetValue(event, *args++, 1);
    }
    if (angle >= 90.0f && angle < 270.0f) {
        evtSetValue(event, *args++, 0);
    }
    if (angle >= 270.0f && angle < 360.0f) {
        evtSetValue(event, *args++, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(get_camcurve_init) {
    camstring = _mapAlloc(0x100);
    camstring[0] = '\0';
    return EVT_RETURN_DONE;
}

USER_FUNC(get_camcurve) {
    EvtWord* args = event->args;
    CamRoadWork* road = camRoadGetWorkPtr();
    s32 ret = 0;

    if (road->prev.on != 0 && road->prev.cam != NULL) {
        if (!strcmp(road->prev.cam->name, "|MarioSt_CameraRoad|C_ground_ok") && !strcmp(camstring, "|MarioSt_CameraRoad|C_ue")) {
            ret = 1;
        }
        strcpy(camstring, road->prev.cam->name);
    }
    evtSetValue(event, args[0], ret);
    return EVT_RETURN_DONE;
}

USER_FUNC(camera_move_check) {
    f32 angle;

    if (isFirstCall) {
        event->userdataf[0] = -1000.0f;
    }
    angle = reviseAngle((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);
    if (event->userdataf[0] != angle) {
        event->userdataf[0] = angle;
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_gor_door_dark_sub)
    CALL(evt_map_set_tevcallback, 1, PTR(_door_color_tev))
    CALL(evt_map_set_tevcallback, 2, PTR(_door_color2_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_del_mC"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_kmn_mC"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_yuka_mD"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_doam"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_sikiri_mC"), 33554432)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_del_mC"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_kmn_mC"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_yuka_mD"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_doam"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_sikiri_mC"), 256)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 255, 0, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("サンダース"), 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_del_mC"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_kmn_mC"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_yuka_mD"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_doam"), 1)
    CALL(evt_sub_intpl_msec_init, 1, 0, -90, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i03_sikiri_mC"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 0, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color2_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_sikiri_mC"), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_open)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(evt_gor_door_open_sub))
    SPAWN_EVENT(PTR(evt_gor_door_dark_sub))
    CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(135.0f))
    CALL(evt_party_move_beside_mario, 0)
    CALL(evt_party_move_beside_mario, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_gor_door_out_info, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        ADD(LW(4), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(4), LW(2), 0, 150, 11)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_gor_door_out_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_close_sanders)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 2, 0, 100, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 2, 100, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_close_sub)
    CALL(evt_sub_intpl_msec_init, 2, 90, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL_CHILD(PTR(evt_gor_door_close_sanders))
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_light_sub)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(_door_color2_main_func, LW(0), LW(0), LW(0), 255)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_sikiri_mC"), 1)
    CALL(evt_sub_intpl_msec_init, 1, -90, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i03_sikiri_mC"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_del_mC"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_kmn_mC"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_yuka_mD"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_doam"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("サンダース"), 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 0, 255, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_del_mC"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_kmn_mC"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_yuka_mD"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_doam"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_sikiri_mC"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_del_mC"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_kmn_mC"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_yuka_mD"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_doam"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_sikiri_mC"), 256)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_door_close)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(evt_gor_door_close_sub))
    SPAWN_EVENT(PTR(evt_gor_door_light_sub))
    BEGIN_INLINE_EVENT
        CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        ADD(LW(4), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(4), LW(2), 0, 150, 11)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gor_offscren_door_entry)
    CALL(_door_color_init_func)
    SET_VALUE(LW(4), STRING("A_ie_03_doa_02"))
    SET_VALUE(LW(6), STRING("A_ie_03_doa_04"))
    CALL(evt_run_case_evt, 9, 1, STRING("A_ie_03_doa_02"), 0, PTR(evt_gor_door_open), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_ie_03_doa_04"), 0, PTR(evt_gor_door_close), 0)
    RETURN
EVT_END()

static EVT_BEGIN(mario_kirimomi_event)
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    CALL(evt_snd_sfxon, STRING("SFX_STG0_MARIO_ROUND1"), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 1, 256)
    CALL(evt_mario_get_dispdir, LW(8))
    SET_VALUE(LW(9), 1800)
    ADD(LW(9), LW(8))
    CALL(evt_sub_intpl_msec_init, 11, LW(8), LW(9), 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mario_set_dispdir, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 0, 256)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    RETURN
EVT_END()

EVT_BEGIN(cam_yurasu_event)
    CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 100)
    RETURN
EVT_END()

EVT_BEGIN(steel_coin)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kmn"), 2)
    CALL(evt_npc_set_ry, STRING("盗賊"), 270)
    CALL(evt_npc_set_position, STRING("盗賊"), -300, 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊"), 1024)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊"), 1073741856)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 0)
        CALL(evt_party_set_pos, 0, -640, 0, 0)
        CALL(evt_party_move_pos2, 0, -565, 0, FLOAT(150.0f))
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_mario_set_pos, -600, 0, 0)
        CALL(evt_mario_mov_pos2, -525, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("盗賊"), -620, 0, 0, FLOAT(300.0f), 0)
        CALL(evt_npc_set_position, STRING("盗賊"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("盗賊"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_BORODO_RUN1"), LW(0), LW(1), LW(2), LW(10))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("盗賊"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(10), LW(0), LW(1), LW(2))
            IF_INT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("盗賊"), LW(0), LW(1), LW(2))
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            SUBTRACT(LW(0), 20)
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_BORODO_STEAL1"), LW(0), LW(1), LW(2), 0)
        SPAWN_EVENT(PTR(cam_yurasu_event))
        SPAWN_EVENT(PTR(mario_kirimomi_event))
        CALL(evt_msg_toge, 2, -600, 30, 0)
        CALL(evt_msg_print, 0, STRING("mac_0_062"), 0, 0)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    CALL(evt_mario_set_dir, 270, 200, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -550, 0, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -450, 103, 330, -525, 43, -6, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_063"))
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_064"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, -1000)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_StatusOpen)
    WAIT_MS(400)
    CALL(evt_pouch_get_coin, LW(0))
    SET_VALUE(LW(1), LW(0))
    DIVIDE(LW(1), 2)
    SUBTRACT(LW(0), LW(1))
    CALL(evt_pouch_set_coin, LW(0))
    SET_VALUE(LW(2), LW(1))
    AND_RAW(LW(2), 255)
    SET_VALUE(GSW(2), LW(2))
    SET_VALUE(LW(2), LW(1))
    AND_RAW(LW(2), 65280)
    DIVIDE(LW(2), 256)
    SET_VALUE(GSW(3), LW(2))
    DEBUG_PUT_MSG(STRING("盗まれた金額の下位と上位"))
    DEBUG_PUT_REG(GSW(2))
    DEBUG_PUT_REG(GSW(3))
    WAIT_MS(2000)
    CALL(evt_StatusClose)
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_065"))
    LABEL(100)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW(0), 8)
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_first_talk)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_naka"), 2)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -115, 0, -225)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_door_end_wait)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_move_pos2, 0, -190, -100, FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 90, 200)
    CALL(evt_msg_print_party, STRING("mac_0_066"))
    CALL(evt_cam3d_evt_set, -115, 65, -20, -115, 30, -217, 666, 11)
    WAIT_MS(666)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_FRANKURI1"))
    CALL(evt_snd_envoff, 1024)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_066_01"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -190, -215, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("クリハカセ"), -140, -225, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -225, -210, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_067"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_01"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_02"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -170, 84, 29, -170, 40, -217, 200, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_03"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -155, 72, -20, -155, 37, -217, 200, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 59, -69, -140, 33, -217, 200, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(700)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_067_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 500, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_067_06"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_0_068"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_069"), 0, STRING("クリハカセ"))
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_PAUSE2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_V_2"))
    WAIT_MS(500)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("mac_0_069_01"), 0, STRING("クリハカセ"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_D_8"))
    WAIT_MS(1500)
    WAIT_WHILE(GF(0))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_U_3"))
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_069_02"), 0, STRING("party"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_069_03"), 0, STRING("クリハカセ"))
    CALL(evt_party_move_pos2, 0, -185, -215, FLOAT(40.0f))
    CALL(evt_cam3d_evt_set, -185, 64, -21, -185, 30, -218, 300, 11)
    CALL(evt_msg_print_party, STRING("mac_0_070"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_071"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -250, 95, 122, -185, 45, -217, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_072"))
    CALL(evt_msg_print, 0, STRING("mac_0_073"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 0, 11)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_073_01"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -65, 95, 107, -185, 45, -217, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_074"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_075"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 64, -21, -185, 30, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_076"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_077"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_078"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("mac_0_079"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -170, -225, FLOAT(80.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_dispflg_onoff, 0, 16, 1)
        CALL(evt_party_move_pos2, 0, -190, -205, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
        CALL(evt_party_dispflg_onoff, 0, 16, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -160, 70, -19, -160, 40, -217, 500, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_item_entry, STRING("item00"), 96, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_item_set_scale, STRING("item00"), FLOAT(0.5498047f))
    WAIT_MS(660)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_item_set_position, STRING("item00"), -140, 0, -220)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_080"), 0, STRING("クリハカセ"))
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_item_set_position, STRING("item00"), LW(0), LW(1), LW(2))
    WAIT_MS(660)
    CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set, -155, 78, 30, -155, 40, -217, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_081"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_0_081_01"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), 1073741856)
    CALL(evt_msg_print, 0, STRING("mac_0_082"), 0, STRING("クリハカセ"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("クリハカセ"), 0, -1000, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_party_pos, 1, 11, LW(0), LW(1), LW(2))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 1)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW(0), 9)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_wall_remove)
    CALL(evt_door_end_wait)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_cam3d_evt_set, -270, 100, 595, -270, 30, 201, 2000, 11)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_mario_clear_party, 1)
    CALL(evt_npc_set_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), 1073741856)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -265, 187, 0, FLOAT(180.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_0_083"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_paper_entry, STRING("OFF_d_saku"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_saku"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_saku04"), STRING("ofs"))
    CALL(set_depthPlaneOffset, STRING("img"), FLOAT(-2.0f))
    CALL(evt_offscreen_get_boundingbox, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    IF_INT_GREATER(LW(2), LW(3))
        SET_VALUE(LW(3), LW(2))
        MULTIPLY(LW(3), 48)
        DIVIDE(LW(3), 64)
        ELSE
        SET_VALUE(LW(3), LW(2))
        MULTIPLY(LW(3), 64)
        DIVIDE(LW(3), 48)
    END_IF
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("Z_1"))
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_img_set_shadow, STRING("img"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_saku04"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_OFF_FENCE_OPEN1"), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon, STRING("SFX_OFF_FENCE_FALL1"), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_saku04"))
    CALL(evt_img_release, STRING("img"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_hitobj_onoff, STRING("A_saku_04"), 1, 0)
    CALL(evt_cam3d_evt_set, -210, 85, 595, -210, 15, 201, 1000, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -220, 305, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("mac_0_084"), 0, STRING("クリハカセ"))
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), 1073741856)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW(0), 10)
    SPAWN_EVENT(PTR(kurihakase_lecture))
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_lecture)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(3), LW(0))
    SET_FLOAT_VALUE(LW(4), LW(1))
    SET_FLOAT_VALUE(LW(5), LW(2))
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(3), LW(5), FLOAT(-300.0f), FLOAT(250.0f), FLOAT(-200.0f), FLOAT(250.0f), LW(6))
        IF_INT_EQUAL(LW(6), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
        SET_FLOAT_VALUE(LW(3), LW(0))
        SET_FLOAT_VALUE(LW(5), LW(1))
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(800)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_084_01"), 0, STRING("クリハカセ"))
    WAIT_WHILE(GF(0))
    CALL(evt_cam3d_evt_set, -245, 100, 639, -245, 40, 294, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -290, 280, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -270, 305, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_084_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_select, 0, STRING("mac_0_084_02_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(50)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("mac_0_084_03"))
    CALL(evt_msg_select, 0, STRING("mac_0_084_03_yesno"))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("mac_0_084_04"))
    GOTO(100)
    LABEL(50)
    CALL(evt_msg_print_add, 0, STRING("mac_0_084_05"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), 33408)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), 128)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_npc_jump_position_nohit, STRING("クリスチーヌ"), LW(0), LW(1), LW(2), 200, 20)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), 32768)
    CALL(evt_msg_print, 0, STRING("mac_0_084_06"), 0, STRING("クリスチーヌ"))
    CALL(evt_npc_set_battle_info, STRING("クリハカセ"), 3)
    CALL(evt_npc_battle_start, STRING("クリハカセ"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), 32768)
    CALL(evt_mario_set_prev_party_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 0, -1000, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_084_07"), 0, STRING("クリハカセ"))
    LABEL(100)
    SET_VALUE(GSW(0), 11)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_door)
    CALL(evt_mapobj_get_position, STRING("S_i02_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 104, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, 11, 104, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_i02_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_lecture_badge)
    CALL(evt_door_end_wait)
    SET_VALUE(GF(0), 1)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_I_O"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_121_01"), 0, 0)
    WAIT_WHILE(GF(0))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -290, -15, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -265, 0, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_naka"), 1)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -215, 0, -93)
    CALL(evt_item_entry, STRING("item00"), 247, 0, -1000, 0, 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
            ADD(LW(1), 20)
            CALL(evt_item_set_position, STRING("item00"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), 1073741856)
    SPAWN_EVENT(PTR(kurihakase_door))
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -235, 0, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("A_2"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("S_1"))
#endif
    DELETE_EVENT(LW(10))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_0_121_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_select, 0, STRING("mac_0_121_02_yesno"))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("mac_0_121_03"))
    CALL(evt_msg_select, 0, STRING("mac_0_121_02_yesno"))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("mac_0_121_05"))
    GOTO(20)
    LABEL(10)
    CALL(evt_msg_print_add, 0, STRING("mac_0_121_04"))
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), 4096)
            CALL(evt_msg_continue)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(lecture)
    CALL(evt_msg_print, 0, STRING("mac_0_121_05"), 0, STRING("クリハカセ"))
    LABEL(20)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL_CHILD(PTR(kurihakase_door))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_naka"), 1)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, -93, 0, FLOAT(120.0f), 0)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), 1073741856)
    SET_VALUE(GSW(0), 20)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(Mario_Party_position)
    CALL(evt_mario_set_pos, -190, 0, -215)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -225, 0, -215)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_set_breed_pose, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_party_set_breed_pose, 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(book_open)
    CALL(evt_mapobj_get_position, STRING("S_open_book_r"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_OPEN_BOOK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 360, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    RETURN
EVT_END()

EVT_BEGIN(book_close)
    CALL(evt_sub_intpl_msec_init, 11, 180, 360, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            CALL(evt_mapobj_get_position, STRING("S_open_book_r"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_SHUT_BOOK1"), LW(0), LW(1), LW(2), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    RETURN
EVT_END()

EVT_BEGIN(book_center_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_open_book"), 1)
    CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map)
    CALL(evt_pouch_get_max_ap, LW(0))
    CALL(evt_pouch_set_ap, LW(0))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_clear_party, 1)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_npc_entry, STRING("ピーチダミー"), STRING("c_peach"))
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_102"), 0, 0)
    WAIT_MS(2000)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_103"))
    CALL(evt_msg_print, 0, STRING("mac_0_103_01"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_103_02"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_103_03"), 0, STRING("クリハカセ"))
    CALL(evt_msg_select, 0, STRING("mac_0_103_03_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("mac_0_103_04"))
        CALL(evt_msg_select, 0, STRING("mac_0_103_03_yesno"))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(10)
        END_IF
        CALL(evt_msg_print_add, 0, STRING("mac_0_103_06"))
        GOTO(20)
    END_IF
    LABEL(10)
    CALL(evt_msg_print_add, 0, STRING("mac_0_103_05"))
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_party_move_pos2, 0, -220, -90, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_npc_move_position, STRING("クリハカセ"), -190, -210, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_move_position, STRING("クリハカセ"), -210, -90, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -210, -80, FLOAT(120.0f))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_battle_info, STRING("クリハカセ"), 4)
    CALL(evt_npc_battle_start, STRING("クリハカセ"))
    CALL(evt_npc_wait_battle_end)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL_CHILD(PTR(Mario_Party_position))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_104"), 0, STRING("クリハカセ"))
    LABEL(20)
    CALL(evt_msg_print_party, STRING("mac_0_105"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_0_106"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -250, 133, 93, -180, 43, -210, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_107"))
    CALL(evt_msg_print, 0, STRING("mac_0_108"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL_CHILD(PTR(book_close))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_109"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_110"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(400)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_111"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_112"))
    CALL(evt_msg_print, 0, STRING("mac_0_113"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_0_114"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -150, 100, 82, -150, 47, -217, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), 1073741856)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -150, -205, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tizu_on"), 1)
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_item_entry, STRING("item_map"), 96, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item_map"), 2)
    CALL(evt_item_set_scale, STRING("item_map"), FLOAT(0.5498047f))
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
            ADD(LW(2), 10)
            CALL(evt_item_set_position, STRING("item_map"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("クリハカセ"), -165, -215, 0, FLOAT(60.0f), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), 1073741856)
    WAIT_MS(500)
    DELETE_EVENT(LW(10))
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_item_set_position, STRING("item_map"), LW(0), LW(1), LW(2))
    WAIT_MS(700)
    CALL(evt_item_set_position, STRING("item_map"), 0, -1000, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
    CALL(evt_cam3d_evt_set, -180, 100, 82, -180, 47, -217, 500, 11)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_117"))
    CALL(evt_msg_print, 0, STRING("mac_0_118"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_0_119"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_120"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSWF(1246), 1)
    SET_VALUE(GSW(0), 19)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_1_004"), 0, 0)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    WAIT_FRAMES(1)
    CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 180)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(PTR(book_close))
    CALL(evt_msg_print, 0, STRING("mac_1_005"), 0, STRING("クリハカセ"))
    WAIT_MS(333)
    CALL(evt_msg_print_party, STRING("mac_1_006"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_1_007"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_1_008"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_1_009"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_1_010"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_1_010_01"), 0, STRING("クリハカセ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    ADD(LW(2), 20)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
    CALL(evt_msg_print_party, STRING("mac_1_010_02"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_1_010_03"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW(0), 65)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_2_002_01"), 0, 0)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(PTR(book_close))
    CALL(evt_msg_print, 0, STRING("mac_2_002_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_002_03"))
    CALL(evt_cam3d_evt_set, -140, 70, 4, -140, 32, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_002_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 63, -40, -208, 32, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_2_002_05"))
    CALL(evt_cam3d_evt_set, -140, 63, -40, -140, 32, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_002_06"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SURPRISED2"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_D_8"))
    CALL(evt_party_set_breed_pose, 0, 8)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    CALL(evt_party_set_breed_pose, 0, 1)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("mac_2_002_07"))
    CALL(evt_cam3d_evt_set, -140, 63, -40, -140, 32, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_002"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_003"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_004"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_2_005"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_006"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_007"))
    CALL(evt_msg_print, 0, STRING("mac_2_008"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_009"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 66, -20, -208, 33, -218, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("mac_2_010"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("mac_2_011"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW(0), 121)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_3_001_01"), 0, 0)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(PTR(evt_door_open_quick))
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(book_center_init))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_3_001_02"), 0, STRING("クリハカセ"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_03"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("mac_3_001_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_3_001_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_06"))
    CALL(evt_msg_print, 0, STRING("mac_3_001_07"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 0, 11)
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_07"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 500, 11)
    WAIT_MS(750)
    CALL(evt_msg_print, 0, STRING("mac_3_002"), 0, STRING("クリハカセ"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_003"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(750)
    CALL(evt_msg_print, 0, STRING("mac_3_004"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 500, 11)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("クリハカセ"), -145, -285, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -135, -225, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_open_book"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_PUT_BOOK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_open_book"), 1)
    WAIT_MS(500)
    CALL_CHILD(PTR(book_open))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -145, -225, 0, FLOAT(60.0f), 0)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_3_005"), 0, STRING("クリハカセ"))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(GSWF(1341), 0)
        CALL(evt_msg_print_party, STRING("mac_3_006"))
        ELSE
        CALL(evt_msg_print_party, STRING("mac_3_007"))
        CALL(evt_msg_print, 0, STRING("mac_3_009"), 0, STRING("クリハカセ"))
        WAIT_MS(1500)
        CALL(evt_eff_fukidashi, 1, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_3_010"), 0, STRING("クリハカセ"))
    END_IF
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW(0), 175)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_4_001_01"), 0, 0)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_4_001_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_4_001_03"))
    CALL(evt_msg_print, 0, STRING("mac_4_001_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_06"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_07"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_08"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -260, 125, 256, -180, 75, -54, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_09"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_10"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_11"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_12"))
    CALL(evt_msg_print, 0, STRING("mac_4_002"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_4_002"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_4_003"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW(0), 222)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_5_002"), 0, STRING("クリハカセ"))
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(PTR(book_close))
    CALL(evt_msg_print_party, STRING("mac_5_002"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_003"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_004"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_005"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_006"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_007"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_008"))
    CALL(evt_mario_set_dir, 270, 270, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_009"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW(0), 272)
    RETURN
EVT_END()

EVT_BEGIN(epigraphy_map_after_stage6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(Mario_Party_position))
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_6_002_00"), 0, STRING("クリハカセ"))
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(PTR(evt_door_open_quick))
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(book_center_init))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 1, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_6_002"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_003"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_6_004"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_005"), 0, STRING("クリハカセ"))
    CALL(evt_npc_move_position, STRING("クリハカセ"), -145, -285, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -135, -225, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_open_book"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_PUT_BOOK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_open_book"), 1)
    WAIT_MS(500)
    CALL_CHILD(PTR(book_open))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_005_01"), 0, STRING("クリハカセ"))
    CALL(evt_sub_save_playtime, GSW(100))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(1220), 1)
    SET_VALUE(GSW(0), 348)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
#else
    CALL(evt_snd_envoff, 18432)
#endif
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_after3minutes)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("mac_6_007"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("mac_6_008"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_009"), 0, STRING("クリハカセ"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_set, LW(0), 95, 73, LW(0), 43, -217, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_6_010"))
    CALL(evt_cam3d_evt_set, -157, 95, 73, -147, 43, -217, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_011"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -197, 121, 185, -197, 50, -217, 0, 11)
    CALL(evt_pouch_check_item, 11, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_MS(500)
        CALL(evt_msg_print_party, STRING("mac_6_012"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_6_013"), 0, STRING("クリハカセ"))
        ELSE
        WAIT_MS(500)
        CALL(evt_msg_print_party, STRING("mac_6_012_01"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_6_013_01"), 0, STRING("クリハカセ"))
    END_IF
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_6_014"))
    CALL(evt_cam3d_evt_set, -157, 95, 73, -147, 43, -217, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_015"), 0, STRING("クリハカセ"))
    CALL(evt_party_run, 0)
    SET_VALUE(GSW(0), 350)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
    CALL(evt_snd_bgmoff, 18432)
#ifdef VERSION_G8ME01
    CALL(evt_snd_envoff, 18496)
#else
    CALL(evt_snd_envoff, 18432)
#endif
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"A_ground_01", {267.0f, 0.0f, 149.0f}},
    {"dokan", {-264.0f, 0.0f, 326.0f}},
    {"A_ground_03", {-264.0f, 0.0f, 326.0f}},
    {"Ship_Change01", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change02", {0.0f, -1000.0f, 0.0f}},
    {"A_ground_06", {267.0f, 0.0f, 149.0f}},
    {"A_ground_07", {267.0f, 0.0f, 149.0f}},
    {"A_ground_08", {267.0f, 0.0f, 149.0f}},
    {"A_ground_02", {252.0f, -59.0f, -297.0f}},
    {"A_ground_09", {252.0f, -59.0f, -297.0f}},
    {"A_ground_10", {252.0f, -59.0f, -297.0f}},
    {"S_i04_kabe_01A1", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(gor_goki1_urouro)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_goki_01"), 0, 270, 0)
    LOOP(0)
        SET_VALUE(LW(0), LW(4))
        SET_VALUE(LW(1), LW(5))
        CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
        CALL(evt_npc_add_dirdist, LW(0), LW(1), LW(3), FLOAT(50.0f))
        CALL(evt_gor_villager_urouro_check_func, LW(0), LW(1), 0, 0, FLOAT(70.0f))
        CALL(evt_sub_get_dir, LW(4), LW(5), LW(0), LW(1), LW(3))
        IF_INT_LESS(LW(3), 180)
            SET_VALUE(LW(6), 270)
            ELSE
            SET_VALUE(LW(6), 90)
        END_IF
        CALL(evt_goki_sub_intpl_init, 13, LW(4), LW(0), LW(5), LW(1), 50)
        LOOP(50)
            CALL(evt_goki_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(0))
            SET_VALUE(LW(3), LW(1))
            ADD(LW(2), -320)
            ADD(LW(3), -480)
            CALL(evt_mapobj_trans, 1, STRING("S_goki_01"), LW(2), 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("S_goki_01"), 0, LW(6), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(4), LW(0))
        SET_VALUE(LW(5), LW(1))
        CALL(evt_sub_random, 3000, LW(2))
        ADD(LW(2), 500)
        WAIT_MS(LW(2))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(gor_goki2_urouro)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    LOOP(0)
        SET_VALUE(LW(0), LW(4))
        SET_VALUE(LW(1), LW(5))
        CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
        CALL(evt_npc_add_dirdist, LW(0), LW(1), LW(3), FLOAT(50.0f))
        CALL(evt_gor_villager_urouro_check_func, LW(0), LW(1), 0, 0, FLOAT(70.0f))
        CALL(evt_sub_get_dir, LW(4), LW(5), LW(0), LW(1), LW(3))
        IF_INT_GREATER(LW(3), 180)
            SET_VALUE(LW(6), 0)
            ELSE
            SET_VALUE(LW(6), 180)
        END_IF
        CALL(evt_goki_sub_intpl_init, 13, LW(4), LW(0), LW(5), LW(1), 50)
        LOOP(50)
            CALL(evt_goki_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(0))
            SET_VALUE(LW(3), LW(1))
            ADD(LW(2), -400)
            ADD(LW(3), 290)
            CALL(evt_mapobj_trans, 1, STRING("S_goki_02"), LW(2), 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("S_goki_02"), 0, LW(6), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(4), LW(0))
        SET_VALUE(LW(5), LW(1))
        CALL(evt_sub_random, 3000, LW(2))
        ADD(LW(2), 500)
        WAIT_MS(LW(2))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), 254, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item01"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 stg0_list[] = {
    8,
};

s32 stg1_list[] = {
    18, 20, 22, 23,
};

s32 stg2_list[] = {
    29, 30, 33, 34,
};

s32 stg3_list[] = {
    65,
};

s32 stg5_list[] = {
    93, 99,
};

s32 stg6_list[] = {
    107,
};

s32 stg7_list[] = {
    117, 118, 28, 120, 119, 121,
};

s32 stg8_list[] = {
    131, 132, 133, 134, 135, 144, 145, 146,
    147, 150,
};

MonosiriFollow follow[] = {
    {23, stg0_list, 1},
    {56, stg1_list, 4},
    {112, stg2_list, 4},
    {164, stg3_list, 1},
    {261, stg5_list, 2},
    {339, stg6_list, 1},
    {373, stg7_list, 6},
    {401, stg8_list, 10},
};

EVT_BEGIN(evt_monosiri)
    CALL(evt_mario_key_onoff, 0)
    CALL(monosiri_check, LW(0), LW(1))
    CALL(monosiri_on, LW(1), LW(2))
    CALL(evt_msg_print_insert, 0, STRING("monosiri_follow_00"), 0, 0, LW(2))
    CALL(evt_mario_key_onoff, 1)
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_memo"), 1)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_monosiri_follow)
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_i02_gomibako"), 0, PTR(evt_monosiri), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_cloud)
    SET_VALUE(GSWF(1221), 1)
    SET_VALUE(LW(0), PTR(&door_data_04_ura))
    CALL_CHILD(PTR(evt_door_entry))
    RETURN
EVT_END()

EVT_BEGIN(iri_19_init)
    IF_INT_EQUAL(GSW(1420), 19)
        IF_INT_EQUAL(GSWF(5405), 1)
#ifdef VERSION_G8ME01
            CALL(evt_item_entry, STRING("wr"), 73, 525, 0, 160, 16, GSWF(5406), 0)
#else
            CALL(evt_item_entry, STRING("wr"), 73, -65, 120, 196, 16, GSWF(5406), 0)
#endif
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -240, 0, -260)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), 1073742880)
    CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), 2)
    CALL(evt_map_blend_set_flag, 1, STRING("クリスチーヌ"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("クリスチーヌ"), 4096)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt_sub)
    WAIT_MS(2000)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("クリスチーヌ"), -170, -225, 0, 60, 0)
        CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("クリハカセ"))
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("S_1"))
        CALL(evt_npc_move_position, STRING("クリスチーヌ"), -240, -260, 0, 60, 0)
        WAIT_MS(4000)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_open_book"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tizu_on"), 1)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), 32768)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_seq_wait, 2)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, 11)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), 32768)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    CALL(evt_msg_print, 0, STRING("ep_36"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, 512, 3000)
    CALL(evt_snd_envoff_f, 512, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("hei_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(dearu_event)
    IF_INT_EQUAL(GSWF(1186), 0)
        SET_VALUE(GSWF(1203), 1)
        ELSE
        SET_VALUE(GSWF(1203), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(front_wall_keshi)
    LOOP(0)
        CALL(camera_front_check, LF(0))
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gai_kabe_mae"), 1)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gai_kabe_mae"), 1)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(tobiori_check)
    CALL(get_camcurve_init)
    LOOP(0)
        LOOP(0)
            CALL(get_camcurve, LW(10))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_EQUAL(LW(10), 1)
                IF_INT_GREATER_OR_EQUAL(LW(0), 370)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DEBUG_PUT_MSG(STRING("カメラ移動開始"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_05_soto"), 1)
        CALL(camera_move_check)
        DEBUG_PUT_MSG(STRING("カメラ移動終了"))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_05_soto"), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(gor_02_init_evt)
    SWITCH(GSW(0))
        CASE_OR_EQUAL(18)
        CASE_OR_EQUAL(64)
        CASE_OR_EQUAL(120)
        CASE_OR_EQUAL(174)
        CASE_OR_EQUAL(221)
        CASE_OR_EQUAL(271)
        CASE_OR_EQUAL(347)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_FRANKURI1"))
        CALL(evt_snd_envoff, 512)
        CASE_END
        CASE_DEFAULT
        IF_INT_NOT_EQUAL(GSW(0), 402)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR3"))
        END_IF
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_GREATER_OR_EQUAL(GSW(0), 268)
        SPAWN_EVENT(PTR(dearu_event))
        ELSE
        SET_VALUE(GSWF(1203), 0)
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW(0), 7)
        CALL(evt_npc_entry, STRING("盗賊"), STRING("c_borodo_n"))
        CALL(evt_npc_set_tribe, STRING("盗賊"), STRING("盗賊"))
        CALL(evt_npc_set_position, STRING("盗賊"), 0, -1000, 0)
        CALL(evt_npc_set_ry, STRING("盗賊"), 90)
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("noname"), 1, 0)
    CALL(evt_map_playanim, STRING("ie_01_light"), 1, 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -220, 0, 50, STRING("S_yuka_A01"), 125, GSWF(5574))
    CALL(evt_mobj_kururing_floor, STRING("kururin2"), 500, 124, -75, STRING("S_i06_kabe_01A"), 125, GSWF(5578))
    CALL(evt_mobj_itembox, STRING("mobj_box01"), 175, -50, 350, 0, 0, PTR(mobj_box01), GSWF(1248))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), -250, 288, -500, 0, GSWF(5525), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk2"), -30, 60, -215, 0, GSWF(5526), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk3"), 540, 60, -260, 0, GSWF(5529), 0)
    CALL(evt_item_entry, STRING("item_star1"), 125, -3, 40, -325, 16, GSWF(5575), 0)
    CALL(evt_item_entry, STRING("item_star2"), 125, 65, 150, -165, 16, GSWF(5576), 0)
    CALL(evt_item_entry, STRING("item_star3"), 125, 50, 0, -390, 16, GSWF(5577), 0)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(PTR(evt_door_02_entry))
    SET_VALUE(LW(0), PTR(&door_data_03))
    CALL_CHILD(PTR(evt_door_03_entry))
    SET_VALUE(LW(0), PTR(&door_data_03_roll))
    CALL_CHILD(PTR(evt_door_03_roll_entry))
    IF_INT_LESS(GSW(0), 23)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_04_naka"), 1)
        CALL(evt_hitobj_onoff, STRING("A_ie_04C"), 1, 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ie_04_doa01"), 0, PTR(evt_door_04_junbi), 0)
        ELSE
        SET_VALUE(LW(0), PTR(&door_data_04))
        CALL_CHILD(PTR(evt_door_entry))
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_05))
    CALL(evt_door_data_copy, LW(0), GW(11))
    SET_VALUE(LW(0), GW(11))
    IF_INT_NOT_EQUAL(GSW(1420), 1)
        CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc))
        ELSE
        CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc_01))
    END_IF
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
        SET_VALUE(LW(15), 4)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    IF_INT_EQUAL(GSWF(1219), 0)
        CALL(evt_mobj_lock, STRING("mobj_lock00"), 49, 446, 18, 50, 0, PTR(evt_chk), PTR(evt_unlock), GSWF(1219))
        ELSE
        SET_VALUE(LW(0), PTR(&door_data_06))
        CALL(evt_door_data_copy, LW(0), LW(1))
        SET_VALUE(LW(0), LW(1))
        IF_INT_EQUAL(GSWF(5375), 0)
            CALL(evt_door_set_param, LW(0), 0, PTR(&door_data_06_npc))
            ELSE
            CALL(evt_door_set_param, LW(0), 0, PTR(&door_data_06_npc_01))
        END_IF
        CALL_CHILD(PTR(evt_door_entry))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW(0), 10)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_saku04"), 1)
        CALL(evt_hitobj_onoff, STRING("A_saku_04"), 1, 0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW(0), 7)
        IF_INT_LESS_OR_EQUAL(GSW(0), 10)
            CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
            CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
            CALL(evt_npc_set_position, STRING("クリスチーヌ"), 0, -1000, 0)
            CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW(0), 10)
        SPAWN_EVENT(PTR(kurihakase_lecture))
    END_IF
    SWITCH(GSW(0))
        CASE_EQUAL(18)
        SPAWN_EVENT(PTR(epigraphy_map))
        CASE_EQUAL(64)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage1))
        CASE_EQUAL(120)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage2))
        CASE_EQUAL(174)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage3))
        CASE_EQUAL(221)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage4))
        CASE_EQUAL(271)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage5))
        CASE_EQUAL(347)
        SPAWN_EVENT(PTR(epigraphy_map_after_stage6))
    END_SWITCH
    SPAWN_EVENT(PTR(evt_gor_offscren_door_entry))
    SPAWN_EVENT(PTR(gor_goki1_urouro))
    SPAWN_EVENT(PTR(gor_goki2_urouro))
    SPAWN_EVENT(PTR(gor_irai_init))
    IF_INT_EQUAL(GSWF(1221), 0)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("S_i04_kabe_msk"), STRING("A_i04_kabe_msk"), PTR(evt_cloud))
        BEGIN_INLINE_EVENT
            LOOP(0)
                IF_INT_EQUAL(GSWF(1221), 1)
                    BREAK_LOOP
                END_IF
                CALL(evt_cloud_get_mode, LW(0))
                IF_INT_NOT_EQUAL(LW(0), -1)
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i04_ura_01B"), 1)
                        ELSE
                        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i04_ura_01B"), 1)
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        ELSE
        SET_VALUE(LW(0), PTR(&door_data_04_ura))
        CALL_CHILD(PTR(evt_door_entry))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i04_kabe_msk"), 1)
        CALL(evt_hitobj_onoff, STRING("A_i04_kabe_msk"), 1, 0)
    END_IF
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_sanbashi_01"), STRING("Ship_Change01"))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_sanbashi_02"), STRING("Ship_Change02"))
    SPAWN_EVENT(PTR(front_wall_keshi))
    SPAWN_EVENT(PTR(tobiori_check))
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL_CHILD(PTR(evt_monosiri_follow))
    SPAWN_EVENT(PTR(iri_19_init))
    IF_INT_EQUAL(GSW(0), 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    RETURN
EVT_END()

BOOL psndBGMStartCheck(s32 id);
