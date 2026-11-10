// gor_02 (ゴロツキタウン, Rogueport): east side. Professor Frankly's house, Trouble Center, power-up shop, info shop;
// exits to gor_01 and down to the underground (tik).

#include "version.h"
#include "system.h"
#include "driver/animdrv.h"
#include "driver/fadedrv.h"
#include "evt/evt_story.h"
#include "evt/evt_cloud.h"
#include "evt/evt_urouro.h"
#include "evt/evt_win.h"
#include "evt/evt_window.h"
#include "manager/winmgr.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
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
#include "evt/evt_johoya.h"
#include "win/win_main.h"
#include "evt/evt_pouch.h"
#include "statuswindow.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"
#include "party.h"

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
#include "driver/seqdrv.h"
#include <dolphin/pad.h>
#include "evt/evt_save_flags.h"

typedef struct MonosiriFollow {
    s32 seq;
    s32* list;
    s32 count;
} MonosiriFollow;

typedef struct DoorColorWork {
    GXColor color;
    GXColor color2;
} DoorColorWork;
static DoorColorWork* _door;
static u8* _ap;
static s32 xlw[16];
static s32* iri_13_item_tbl;
static char* camstring;

static USER_FUNC(_door_color2_main_func);
static USER_FUNC(_door_color_init_func);
static USER_FUNC(_door_color_main_func);
static USER_FUNC(bgScrollControll);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(camera_front_check);
static USER_FUNC(camera_move_check);
static USER_FUNC(effect_stardust);
static USER_FUNC(enzan_cam_turn_keisan_f);
static USER_FUNC(enzan_lw_load);
static USER_FUNC(enzan_lw_save);
static USER_FUNC(evt_StatusClose);
static USER_FUNC(evt_StatusOpen);
static USER_FUNC(evt_gor_door_in_info);
static USER_FUNC(evt_gor_door_out_info);
USER_FUNC(evt_set_bgtpl);
static USER_FUNC(get_camcurve);
static USER_FUNC(get_camcurve_init);
static USER_FUNC(iri_13_free_item_tbl);
static USER_FUNC(iri_13_make_item_tbl);
static USER_FUNC(iri_13_update_item_tbl);
USER_FUNC(lecture);
USER_FUNC(monosiri_check);
static USER_FUNC(monosiri_on);
static USER_FUNC(set_depthPlaneOffset);
static USER_FUNC(set_talk_memory_ry);
static USER_FUNC(setmertn);
static USER_FUNC(status_gauge_force_off);
static USER_FUNC(stonechangertn);
USER_FUNC(get_mario_dir_target);
static void kuragarisan_msgcb(s32 type, void* param);
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

/**
 * Map TEV callback that multiplies the previous colour and alpha by the door colour (the first colour set by
 * _door_color_main_func). Installed by the door darkening scripts.
 */
static void _door_color_tev(MapTevCallbackInfo* info) {
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

/** Same as _door_color_tev for the second door colour set by _door_color2_main_func. */
static void _door_color2_tev(MapTevCallbackInfo* info) {
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

/** Allocates the door tint state and sets both colours to opaque white. Run once from the map init. */
static USER_FUNC(_door_color_init_func) {
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

/** Sets the first door tint colour from args[0] to args[3] (red, green, blue, alpha). */
static USER_FUNC(_door_color_main_func) {
    EvtWord* args = event->args;
    DoorColorWork* door = _door;

    door->color.r = evtGetValue(event, args[0]);
    door->color.g = evtGetValue(event, args[1]);
    door->color.b = evtGetValue(event, args[2]);
    door->color.a = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

/** Sets the second door tint colour from args[0] to args[3] (red, green, blue, alpha). */
static USER_FUNC(_door_color2_main_func) {
    EvtWord* args = event->args;
    DoorColorWork* door = _door;

    door->color2.r = evtGetValue(event, args[0]);
    door->color2.g = evtGetValue(event, args[1]);
    door->color2.b = evtGetValue(event, args[2]);
    door->color2.a = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

/**
 * Map TEV callback that leaves the polygon colour unchanged and multiplies its alpha by the single byte set by
 * alpha_set_func (used to fade a map object).
 */
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

#if VERSION >= VERSION_JP
EvtWord gor_02_hitobj_off[27];
EvtWord gor_02_hitobj_on[54];
#endif

static EvtWord gor_02_event_00[18];
static EvtWord gor_02_event_01[30];
static BeroEntry bero_entry_data[3];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord powerupya_party_lank_rtn[];
#else
EvtWord powerupya_party_lank_rtn[786];
#endif

#if VERSION >= VERSION_US
static EvtWord powerupya_party_rtn[170];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord powerupya_party_rtn[152];
#else
static EvtWord powerupya_party_rtn[167];
#endif
#endif

#if VERSION <= VERSION_DEMO_JP
extern EvtWord powerupya_table_talk[];
#else
EvtWord powerupya_table_talk[91];
#endif

#if VERSION == VERSION_DEMO_JP
static EvtWord powerupya_direct_talk[103];
#else
static EvtWord powerupya_direct_talk[96];
#endif

#if VERSION >= VERSION_JP
s32 powerupya_item_tbl[2];
EvtWord powerupya_stardust[32];
EvtWord powerupya_mario_move[48];
EvtWord powerupya_party_move[38];
EvtWord powerupya_orb_use[544];
EvtWord powerupya_talk[119];
#else
extern EvtWord powerupya_orb_use[];
#endif

static EvtWord mapball_init[19];
static EvtWord powerupya_init[58];

#if VERSION >= VERSION_US
static EvtWord johoyaperfect_talk[169];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord johoyaperfect_talk[133];
#else
static EvtWord johoyaperfect_talk[146];
#endif
#endif

static EvtWord kurihakase_init[134];

#if VERSION <= VERSION_DEMO_JP
static EvtWord kurihakase_talk[418];
#else
static EvtWord kurihakase_talk[421];
#endif

static EvtWord oyabun_init[7];
static EvtWord oyabun_talk[364];
static EvtWord tozokudan1_init[36];
static EvtWord tozokudan1_talk[285];
static EvtWord tozokudan2_init[24];
static s32 key_tbl[2];

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_chk[21];
#else
static EvtWord evt_chk[23];
#endif

static EvtWord tozokudan2_house_door[139];
static EvtWord mario_yokeru[127];
static EvtWord evt_unlock_mario[78];
static EvtWord evt_unlock[6];
static EvtWord evt_unlock_npc[97];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord tozokudan2_talk[];
#else
EvtWord tozokudan2_talk[121];
#endif

static EvtWord tozokudan3_init[17];
static EvtWord tozokudan3_talk[22];
static EvtWord sanders_open_door[56];
static EvtWord sanders_init[37];

#if VERSION >= VERSION_DEMO_EU
static EvtWord sanders_talk[580];
#else
static EvtWord sanders_talk[574];
#endif

static EvtWord kuribo4_init[13];
static EvtWord kuribo4_regl[19];
static s32 item_tbl[2];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord kuribo4_talk[];
#else
EvtWord kuribo4_talk[276];
#endif

static EvtWord chusan3_init[2];
static EvtWord chusan3_talk[76];
static EvtWord roten3_init[2];
static EvtWord roten3_talk[174];
static EvtWord borodo1_init[26];
static EvtWord borodo1_regl[26];
static EvtWord borodo1_talk[76];
static EvtWord borodo2_init[35];
static EvtWord borodo2_escape[51];
static EvtWord borodo2_talk[100];
static EvtWord kuragarisan_init[7];
static EvtWord kuragarisan_write_name[435];
static EvtWord kuragarisan_talk[108];
static EvtWord luigi_init[18];
static EvtWord luigi_talk[4];
static NpcSetupInfo npcEnt[16];
static EvtWord evt_door_01_00[12];
static EvtWord evt_door_01_01[12];
static EvtWord evt_wall_01[15];
static const char* door_data_01_npc[2];

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_door_01_open[86];
static EvtWord evt_door_01_close[83];
#else
static EvtWord evt_door_01_open[88];
static EvtWord evt_door_01_close[85];
#endif

static DoorParam door_data_01;
static EvtWord evt_door_02_00[12];
static EvtWord evt_door_02_01[12];
static EvtWord evt_wall_02[15];
static const char* door_data_02_npc[2];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_door_02_open[];
extern EvtWord evt_door_02_close[];
#else
EvtWord evt_door_02_open[127];
EvtWord evt_door_02_close[76];
#endif

static DoorParam door_data_02;
static EvtWord evt_door_02_entry[64];
static EvtWord evt_door_03_00[12];
static EvtWord evt_door_03_01[12];
static EvtWord evt_wall_03[15];
static const char* door_data_03_npc[2];
static const char* door_data_03_mobj[2];

#if VERSION >= VERSION_DEMO_EU
static EvtWord evt_door_03_close[178];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord evt_door_03_close[144];
#else
static EvtWord evt_door_03_close[166];
#endif
#endif

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_door_03_open[12];
#else
static EvtWord evt_door_03_open[14];
#endif

static DoorParam door_data_03;
static EvtWord evt_door_sanders_open[27];
static EvtWord evt_door_sanders_close[27];
static EvtWord evt_door_03_entry[64];
static EvtWord evt_door_03_caseopen[51];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_door_03_roll_open[];
#else
EvtWord evt_door_03_roll_open[163];
#endif

static EvtWord evt_door_03_roll_close[12];
static EvtWord evt_wall_03_roll[15];
static DoorParam door_data_03_roll;
static EvtWord evt_door_03_roll_entry[20];
static EvtWord evt_door_04_00[12];
static EvtWord evt_door_04_01[12];
static EvtWord evt_wall_04[38];

#if VERSION >= VERSION_US
static EvtWord evt_door_04_open[19];
static EvtWord evt_door_04_close[21];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord evt_door_04_open[12];
static EvtWord evt_door_04_close[14];
#else
static EvtWord evt_door_04_open[14];
static EvtWord evt_door_04_close[16];
#endif
#endif

static const char* door_data_04_npc[2];
static const char* door_data_04_item[4];
static DoorParam door_data_04;
static EvtWord evt_door_04_junbi[20];
static EvtWord evt_door_04_00_ura[12];
static EvtWord evt_door_04_01_ura[12];
static EvtWord evt_wall_04_ura[21];
static DoorParam door_data_04_ura;
static EvtWord evt_door_05_00[12];
static EvtWord evt_wall_05[28];
const char* door_data_05_npc[4];
const char* door_data_05_npc_01[3];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_door_05_open[];
extern EvtWord evt_door_05_close[];
#else
EvtWord evt_door_05_open[14];
EvtWord evt_door_05_close[23];
#endif

static DoorParam door_data_05;
static EvtWord evt_door_06_00[12];
static EvtWord evt_wall_06_00[62];
static EvtWord evt_wall_06_01[59];
static EvtWord evt_wall_06_02[62];
static EvtWord evt_wall_06[12];
static const char* door_data_06_npc[2];
static const char* door_data_06_npc_01[1];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_door_06_open[];
extern EvtWord evt_door_06_close[];
#else
EvtWord evt_door_06_open[14];
EvtWord evt_door_06_close[16];
#endif

static DoorParam door_data_06;
static EvtWord evt_gor_door_open_sanders[80];
static EvtWord evt_gor_door_open_sub[44];
static EvtWord evt_gor_door_dark_sub[200];
static EvtWord evt_gor_door_open[82];
static EvtWord evt_gor_door_close_sanders[80];
static EvtWord evt_gor_door_close_sub[31];
static EvtWord evt_gor_door_light_sub[194];
static EvtWord evt_gor_door_close[66];
static EvtWord evt_gor_offscren_door_entry[26];
static EvtWord mario_kirimomi_event[67];
static EvtWord cam_yurasu_event[8];
static EvtWord steel_coin[386];
static EvtWord kurihakase_first_talk[787];
static EvtWord kurihakase_wall_remove[257];
static EvtWord kurihakase_lecture[286];
static EvtWord kurihakase_door[84];

#if VERSION >= VERSION_US
static EvtWord kurihakase_lecture_badge[282];
#else
static EvtWord kurihakase_lecture_badge[272];
#endif

static EvtWord Mario_Party_position[33];
static EvtWord book_open[42];
static EvtWord book_close[42];
static EvtWord book_center_init[22];
EvtWord epigraphy_map[];

#if VERSION >= VERSION_US
static EvtWord epigraphy_map_after_stage1[258];
static EvtWord epigraphy_map_after_stage2[400];
static EvtWord epigraphy_map_after_stage3[368];
static EvtWord epigraphy_map_after_stage4[297];
static EvtWord epigraphy_map_after_stage5[225];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord epigraphy_map_after_stage1[246];
static EvtWord epigraphy_map_after_stage2[388];
static EvtWord epigraphy_map_after_stage3[356];
static EvtWord epigraphy_map_after_stage4[285];
static EvtWord epigraphy_map_after_stage5[213];
#else
static EvtWord epigraphy_map_after_stage1[251];
static EvtWord epigraphy_map_after_stage2[393];
static EvtWord epigraphy_map_after_stage3[361];
static EvtWord epigraphy_map_after_stage4[290];
static EvtWord epigraphy_map_after_stage5[218];
#endif
#endif

#if VERSION == VERSION_DEMO_JP
static EvtWord epigraphy_map_after_stage6[225];
static EvtWord kurihakase_after3minutes[172];
#else
static EvtWord epigraphy_map_after_stage6[245];
static EvtWord kurihakase_after3minutes[177];
#endif

static HitDamageReturn hitDmgRetPoints[13];
static EvtWord gor_goki1_urouro[116];
static EvtWord gor_goki2_urouro[109];
static EvtWord mobj_box01[34];
static s32 stg0_list[1];
static s32 stg1_list[4];

#if VERSION <= VERSION_DEMO_JP
static s32 stg2_list[3];
#else
static s32 stg2_list[4];
#endif

static s32 stg3_list[1];

#if VERSION <= VERSION_DEMO_JP
static s32 stg4_list[1];
static s32 stg5_list[3];
#else
static s32 stg5_list[2];
#endif

static s32 stg6_list[1];

#if VERSION <= VERSION_DEMO_JP
static s32 stg7_list[9];
static s32 stg8_list[9];
MonosiriFollow follow[9];
#else
static s32 stg7_list[6];
static s32 stg8_list[10];
MonosiriFollow follow[8];
#endif

static EvtWord evt_monosiri[39];
static EvtWord evt_monosiri_follow[18];
static EvtWord evt_cloud[10];
static EvtWord iri_19_init[20];
static EvtWord epilogue_npc_entry[40];
static EvtWord epilogue_evt_sub[56];
static EvtWord epilogue_evt[101];
EvtWord dearu_event[13];
static EvtWord front_wall_keshi[27];
static EvtWord tobiori_check[48];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord gor_02_init_evt[];
#else
EvtWord gor_02_init_evt[543];

/**
 * Turns off the east side's collision pieces ("A_kabe", "A_saku", "A_kmn", "A_gomi", "A_l_ura_gomi")
 * so Mario can pass through them. The house doors' open callbacks (evt_door_01_open to evt_door_06_open and
 * evt_door_03_roll_open) spawn it as he goes inside; gor_02_hitobj_on restores the pieces.
 * JP and later.
 */
EVT_BEGIN(gor_02_hitobj_off)
    CALL(evt_hitobj_onoff, STRING("A_kabe"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_saku"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_kmn"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_gomi"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_l_ura_gomi"), 1, 0)
    RETURN
EVT_END()
#endif

/**
 * Turns the east side's collision pieces back on after gor_02_hitobj_off,
 * then reopens the ones that must stay open: the fence piece "A_saku_04" once the fence has been removed,
 * and the center wall "A_center_kabe_atr" once Gus has been beaten or paid off.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(gor_02_hitobj_on)
    CALL(evt_hitobj_onoff, STRING("A_kabe"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_saku"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_kmn"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_gomi"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_l_ura_gomi"), 1, 1)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG0_10_REMOVING_THE_FENCE)
        CALL(evt_hitobj_onoff, STRING("A_saku_04"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_GOR_GATEKEEPER_DEFEATED, 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
    END_IF
    IF_INT_EQUAL(GW(10), 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
    END_IF
    RETURN
EVT_END()
#endif

/**
 * Enter event of the west exit "w_bero" (from the plaza):
 * while the story is at STG0_07_GOOMBELLA_JOINS it locks Mario's controls,
 * starts steel_coin and returns 1 in LW(0); otherwise returns 0.
 */
static EVT_BEGIN(gor_02_event_00)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SPAWN_EVENT(steel_coin)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/**
 * Exit event of the pipe "dokan" (to tik_01): the first time through,
 * places Frankly as the follower (party slot 1) out of sight below the map and sets GSWF_GOR_02_TUTORIAL_PIPE_TAKEN.
 * Returns 0 in LW(0) so the pipe still works.
 */
static EVT_BEGIN(gor_02_event_01)
    IF_INT_EQUAL(GSWF_GOR_02_TUTORIAL_PIPE_TAKEN, 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_party_stop, 1)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_party_pos, 1, PARTY_FRANKLY, 0, -1000, 0)
    SET_VALUE(GSWF_GOR_02_TUTORIAL_PIPE_TAKEN, 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exits of this room: the west exit w_bero back to gor_01 and the pipe dokan down to tik_01. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        gor_02_event_00, // enterEvent
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
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        gor_02_event_01, // exitEvent
        "tik_01", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Allocates the one-byte alpha used by alpha_tev and sets it to 255 (opaque). */
static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

/** Sets the alpha byte used by alpha_tev to args[0]. */
static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Starts a Stardust particle effect: args[0] type, args[1] to args[3] position,
 * args[4] and args[5] horizontal and vertical spread, args[6] particle count, args[7] lifetime.
 */
static USER_FUNC(effect_stardust) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 vx = evtGetFloat(event, args[4]);
    f32 vy = evtGetFloat(event, args[5]);
    s32 count = evtGetValue(event, args[6]);
    s32 camera = evtGetValue(event, args[7]);

    effStardustEntry(type, count, camera, x, y, z, vx, vy);
    return EVT_RETURN_DONE;
}

/**
 * Integer proportion helper: stores in args[4] (a*c + d*(b-a)) / b, where a is args[0] minus 1 and b, c,
 * d are args[1] to args[3]. Used by the Chapter 8 camera scripts to blend between two values.
 */
static USER_FUNC(wariai_enzan_1) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/** Float version of wariai_enzan_1, storing the result in args[4] as a float. */
static USER_FUNC(wariai_enzan_1f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/**
 * Float proportion along an easing curve, stored in args[4]:
 * step args[0] (counted from 1) of args[1] steps from args[3] toward args[2].
 * args[5] picks the intplGetValue mode, not a time: 0 to 13 use that mode,
 * 14 to 27 use mode args[5] - 14 mirrored (1 minus the curve from 1 to 0), and 28 or more just give args[2].
 */
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
        r = 1.0f - intplGetValue(t, 1.0f, 0.0f, b - a, b);
    } else {
        r = intplGetValue(t, 0.0f, 1.0f, b - a, b);
    }
    evtSetFloat(event, out, (1.0f - r) * c + r * d);
    return EVT_RETURN_DONE;
}

/** Same as wariai_enzan_1 but with b also reduced by 1 (an inclusive range). Integer result in args[4]. */
static USER_FUNC(wariai_enzan_0) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/** Float version of wariai_enzan_0, storing the result in args[4]. */
static USER_FUNC(wariai_enzan_0f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/**
 * Rotates the point (args[1], args[2]) around the center (args[3], args[4]) by args[0] degrees about the Y axis,
 * writing the new x and z back to the two variables.
 */
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

/**
 * Moves the 3D camera for the power-up ceremony: args[0] is 0 to 100 percent.
 * It places the camera on a spiral around (-362, 37, -200),
 * descending and moving closer while turning two full circles, and starts a zero-length camera move there.
 */
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

/**
 * Merlon's partner rank up ceremony in the power-up shop:
 * the stone glows, Merlon talks (powerup_12 and 14),
 * the shop interior fades and a lens flare and crystals gather while the partner shakes,
 * then a bright release and star dust finish. The selected partner gets its own poses (case on partner id 1 to 7).
 * Ends by restoring the interior; JP and later also restore the music.
 */
EVT_BEGIN(powerupya_party_lank_rtn)
    // The partner faces the stone (Mario also does so in JP and later), and the music dips.
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_party_set_dir_pos, 0, -362, -200)
#if VERSION >= VERSION_JP
            CALL(evt_mario_set_dir_pos, -362, -200)
#endif
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(enzan_lw_save)

    // Merlon announces the ceremony; the stone's glow rises.
    CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
    CALL(evt_msg_print, 0, STRING("powerup_12"), 0, STRING("me"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 20, 1000)
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
    CALL(evt_snd_bgmoff, PSND_PAUSE)

    // The ceremony music starts and a lens flare blooms over the stone.
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_POWERUP1"))
    CALL(bgm_start_wait)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -362, 44, -200, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i01_sui_ao"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i01_sui_aka"), MAP_OBJ_FLAG_HIDDEN)

    // Merlon speaks again; the shop interior fades out so only the stone, Mario and the partner remain.
    CALL(evt_msg_print, 0, STRING("powerup_14"), 0, STRING("me"))
    CALL(bgScrollControll, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
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
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 1200)
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
        // The crystal pillars swing up while the camera shakes and rises above the stone.
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 90, 1600)
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
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 1600)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -362, 193, 686, -362, 37, -200, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1600)
    BEGIN_INLINE_EVENT
        // The camera circles the stone as crystals and sparkles gather around the partner.
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 100, 7000)
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

        // The partner shakes and strikes the power-up pose, one pose per partner.
        CALL(evt_mario_get_party, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(PARTY_GOOMBELLA)
            CALL(evt_party_set_pose, 0, STRING("PKR_C_1"))
            CASE_EQUAL(PARTY_KOOPS)
            CALL(evt_party_set_pose, 0, STRING("PNK_C_1"))
            CASE_EQUAL(PARTY_BOBBERY)
            CALL(evt_party_set_pose, 0, STRING("C_1"))
            CASE_EQUAL(PARTY_YOSHI)
            CALL(evt_party_set_pose, 0, STRING("PYS_C_1"))
            CASE_EQUAL(PARTY_FLURRIE)
            CALL(evt_party_set_pose, 0, STRING("PWD_C_1"))
            CASE_EQUAL(PARTY_VIVIAN)
            CALL(evt_party_set_pose, 0, STRING("PTR_C_1"))
            CASE_EQUAL(PARTY_MS_MOWZ)
            CALL(evt_party_set_pose, 0, STRING("PCH_C_1"))
        END_SWITCH
    END_INLINE_EVENT
    WAIT_MS(7000)

    // The glow fades.
    CALL(evt_eff_softdelete, STRING("aaax"))
    IF_INT_EQUAL(GSWF_GOR_UP_ARROW_USED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i01_sui_ao"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i01_sui_aka"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    WAIT_MS(500)

    // A bright release flashes.
    CALL(evt_eff64, STRING("dddx"), STRING("release_n64"), 65283, -362, 44, -200, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("cccx"))

    // The partner relaxes and star dust sprays out around them.
    CALL(evt_mario_get_party, LW(1))
    SWITCH(LW(1))
        CASE_EQUAL(PARTY_GOOMBELLA)
        CALL(evt_party_set_pose, 0, STRING("PKR_I_1"))
        CASE_EQUAL(PARTY_KOOPS)
        CALL(evt_party_set_pose, 0, STRING("PNK_I_1"))
        CASE_EQUAL(PARTY_BOBBERY)
        CALL(evt_party_set_pose, 0, STRING("I_1"))
        CASE_EQUAL(PARTY_YOSHI)
        CALL(evt_party_set_pose, 0, STRING("PYS_I_1"))
        CASE_EQUAL(PARTY_FLURRIE)
        CALL(evt_party_set_pose, 0, STRING("PWD_I_1"))
        CASE_EQUAL(PARTY_VIVIAN)
        CALL(evt_party_set_pose, 0, STRING("PTR_I_1"))
        CASE_EQUAL(PARTY_MS_MOWZ)
        CALL(evt_party_set_pose, 0, STRING("PCH_I_1"))
    END_SWITCH
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    SET_FLOAT_VALUE(LW(7), LW(4))
    ADD_FLOAT(LW(7), FLOAT(18.0f))
    CALL(effect_stardust, 0, LW(3), LW(7), LW(5), FLOAT(50.0f), FLOAT(50.0f), 20, 120)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)

        // The shop interior fades back in and the pillars swing down.
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1200)
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
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 1600)
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
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_01_soto"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_01_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND)
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

    // The camera returns to normal and the partner cheers, again with its own pose.
    CALL(evt_cam3d_evt_off, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1400)
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 20, 0, 1000)
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
        CASE_EQUAL(PARTY_GOOMBELLA)
        CALL(evt_party_set_pose, 0, STRING("PKR_Y_1"))
        CASE_EQUAL(PARTY_KOOPS)
        CALL(evt_party_set_pose, 0, STRING("PNK_Y_1"))
        CASE_EQUAL(PARTY_BOBBERY)
        CALL(evt_party_set_pose, 0, STRING("Y_1"))
        CASE_EQUAL(PARTY_YOSHI)
        CALL(evt_party_set_pose, 0, STRING("PYS_Y_1"))
        CASE_EQUAL(PARTY_FLURRIE)
        CALL(evt_party_set_pose, 0, STRING("PWD_Y_1"))
        CASE_EQUAL(PARTY_VIVIAN)
        CALL(evt_party_set_pose, 0, STRING("PTR_Y_1"))
        CASE_EQUAL(PARTY_MS_MOWZ)
        CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    END_SWITCH
    WAIT_MS(1500)

    // In JP and later, the music returns to the shop theme.
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(enzan_lw_load)
#if VERSION >= VERSION_JP
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
#endif
    RETURN
EVT_END()

/**
 * Merlon's partner rank up menu: lets Mario pick a partner from the other-party window,
 * takes 3 Shine Sprites (supercoins) for each rank up, runs powerupya_party_lank_rtn,
 * and loops until Mario stops or has fewer than 3 sprites; JP and later also stop automatically when no partner can be raised.
 */
static EVT_BEGIN(powerupya_party_rtn)
    CALL(evt_win_coin_on, COIN_WINDOW_SHINE_SPRITES, LW(12))
    LOOP(0)
        CALL(evt_party_set_dir_pos, 0, -362, -200)
        CALL(evt_win_other_select, SELECT_TYPE_PARTY_RANK_UP)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print, 0, STRING("powerup_13_01"), 0, STRING("me"))
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_party, LW(8))
        IF_INT_NOT_EQUAL(LW(1), LW(8))
            CALL(evt_mario_goodbye_party, 0)
            CALL(evt_mario_hello_party, 0, LW(1))
#if VERSION >= VERSION_US
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
            CALL_CHILD(powerupya_party_lank_rtn)
            CALL(evt_pouch_get_supercoin, LW(0))
            IF_INT_LESS(LW(0), 3)
                CALL(evt_msg_print, 0, STRING("powerup_13"), 0, STRING("me"))
                BREAK_LOOP
            END_IF
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_msg_print, 0, STRING("powerup_15"), 0, STRING("me"))
                CALL(evt_msg_select, 0, STRING("powerup_03"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_continue)
                ELSE
                    CALL(evt_msg_print_add, 0, STRING("powerup_13_02"))
                    BREAK_LOOP
                END_IF
#else
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
#endif
            CALL(evt_win_coin_on, COIN_WINDOW_SHINE_SPRITES, LW(12))
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Talk event of Merlon at his table.
 * Greets Mario (powerup_00, 00_01, 00_02), says if no partner can be raised (JP and later),
 * and otherwise offers the rank up through powerupya_party_rtn while Mario has at least 3 Shine Sprites.
 * Locks controls for the talk and frees them at the end.
 */
EVT_BEGIN(powerupya_table_talk)
    CALL(setmertn, STRING("パワーアップ屋"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
#if VERSION >= VERSION_JP
    CALL(evt_win_party_lankup_chk, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("powerup_16"), 0, STRING("me"))
        GOTO(0)
    END_IF
#endif
    IF_INT_EQUAL(GSWF_GOR_MERLON_FIRST_TALK_DONE, 0)
        CALL(evt_msg_print, 0, STRING("powerup_00"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_MERLON_FIRST_TALK_DONE, 1)
        SET_VALUE(GSWF_GOR_MERLON_GREETED_THIS_VISIT, 1)
    ELSE
        IF_INT_EQUAL(GSWF_GOR_MERLON_GREETED_THIS_VISIT, 0)
            CALL(evt_msg_print, 0, STRING("powerup_00_01"), 0, STRING("me"))
            SET_VALUE(GSWF_GOR_MERLON_GREETED_THIS_VISIT, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("powerup_00_02"), 0, STRING("me"))
        END_IF
    END_IF
    CALL(evt_pouch_get_supercoin, LW(2))
    IF_INT_LESS(LW(2), 3)
        CALL(evt_msg_print_add, 0, STRING("powerup_01"))
    ELSE
        CALL(evt_msg_continue)
        CALL_CHILD(powerupya_party_rtn)
    END_IF
#if VERSION >= VERSION_JP
    LABEL(0)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
/**
 * Shop-door animation of JP and later: reveals the interior, swings the door open and shut,
 * then hides the interior again.
 */
static EVT_BEGIN(powerupya_direct_talk)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("S_ie_01_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 104, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 104, 0, 500)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()
#endif

/** Items the item picker offers in Merlon's shop: the Up Arrow; -1 ends the list. */
#if VERSION >= VERSION_JP
s32 powerupya_item_tbl[] = {
    108, -1,
};
#endif

/**
 * Loops forever spawning a Stardust particle burst above Merlon's NPC every 200 milliseconds;
 * spawned while he spins up in the air in powerupya_orb_use and stopped with DELETE_EVENT.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(powerupya_stardust)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Walks Mario to the spot beside the stone (-395, -160) or (-404, -220),
 * depending on whether he is south of z=-210, and faces him right.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(powerupya_mario_move)
    WAIT_MS(200)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), -210)
        CALL(evt_mario_mov_pos2, -360, -270, FLOAT(120.0f))
        CALL(evt_mario_mov_pos2, -404, -220, FLOAT(120.0f))
    ELSE
        CALL(evt_mario_mov_pos2, -360, -160, FLOAT(120.0f))
        CALL(evt_mario_mov_pos2, -395, -160, FLOAT(120.0f))
    END_IF
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    RETURN
EVT_END()
#endif

/**
 * Walks the active partner to the spot beside the stone (-425, -186),
 * by the same two routes as powerupya_mario_move.
 */
#if VERSION >= VERSION_JP
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
    // Mario holds out the Up Arrow; it is displayed beside him and then vanishes.
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
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
    CALL(evt_item_entry, STRING("watasu"), ITEM_UP_ARROW, LW(3), LW(4), LW(5), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("watasu"), ITEM_FLAG_NO_TOUCH_GET)
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("watasu"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)

    // Merlon reacts to the arrow and speaks.
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("powerup_ev_04"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_N_1"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("me"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)

    // The camera moves in on Merlon as he muses over it.
    CALL(evt_cam3d_evt_set, -335, 100, 158, -335, 33, -236, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("powerup_ev_05"), 0, STRING("me"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)

    // Merlon spins on the spot; star dust sprays and the shine sounds play.
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_2"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 2160, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("me"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT

    // He spins up into the air while Mario looks on in surprise.
    SPAWN_EVENT_GET_ID(PTR(powerupya_stardust), LW(5))
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_SHINE1"), LW(7), LW(8), LW(9), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LOTTERY_BOMB_BURST2"), LW(7), LW(8), LW(9), 0)
    SET_VALUE(LW(10), LW(8))
    ADD(LW(10), 200)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(8), LW(10), 1600)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(2000)
    CALL(evt_npc_get_position, STRING("me"), LW(7), LW(8), LW(9))

    // A thud and camera shake accompany the Ultra Stone falling into the room; Merlon is still in the air.
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BREAK_FLOOR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 1000)
    WAIT_MS(500)
    CALL(evt_item_entry, STRING("orb"), ITEM_ULTRA_STONE, -365, 150, -110, ITEM_TYPE_FALL, -1, 0)
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
        // He spins again and drops back to the floor, then speaks.
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 2160, 1600)
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
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(8), 0, 1600)
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

    // Mario and the partner walk over to the stone while Merlon picks it up.
    SPAWN_EVENT(powerupya_mario_move)
    CALL(evt_party_stop, 0)
    SPAWN_EVENT(powerupya_party_move)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("me"), -365, -115, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
    CALL(evt_item_delete, STRING("orb"))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("me"), -338, -160, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    WAIT_MS(1500)

    // The ceremony music starts and a lens flare shines over the stone.
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_POWERUP1"))
    CALL(bgm_start_wait)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -362, 27, -180, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i01_sui_ao"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("aaax"))
#else

/**
 * In the early demos, Merlon takes the Up Arrow in this talk event and changes the shop's blue crystal to red.
 * This talk event has the name that JP and later give to the door animation.
 */
static EVT_BEGIN(powerupya_direct_talk)
    IF_INT_EQUAL(GSWF_GOR_MERLON_HINT_PENDING, 1)
        IF_INT_EQUAL(GSWF_GOR_MERLON_STAR_CHART_HINT_DONE, 0)
            CALL(evt_msg_print, 0, STRING("powerup_ev_01"), 0, STRING("me"))
            CALL(evt_cam3d_evt_set, -355, 114, 371, -355, 45, -23, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("powerup_ev_02"), 0, STRING("me"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSWF_GOR_MERLON_STAR_CHART_HINT_DONE, 1)
        ELSE
            CALL(evt_pouch_check_item, ITEM_UP_ARROW, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, FUKIDASHI_AT_MARIO, STRING("powerup_ev_04"), 0, STRING("me"))
                CALL(evt_msg_print, 0, STRING("powerup_ev_05"), 0, STRING("me"))
    CALL(evt_msg_print, 0, STRING("powerup_ev_06"), 0, STRING("me"))
#endif

    SET_VALUE(GSWF_GOR_UP_ARROW_USED, 1)

    // The blue stone turns red; in JP and later, the ceremony music fades.
    CALL(stonechangertn, STRING("S_i01_sui_aka"))
#if VERSION >= VERSION_JP
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    WAIT_MS(2000)

    // Merlon hops back to his table and speaks; the Up Arrow is removed from the inventory.
    CALL(evt_npc_jump_position_nohit, STRING("me"), -320, 0, -205, 500, 50)
    WAIT_MS(500)
#endif
    CALL(evt_msg_print, 0, STRING("powerup_ev_07"), 0, STRING("me"))
    CALL(evt_pouch_remove_item, ITEM_UP_ARROW, LW(0))
    SET_VALUE(GSWF_GOR_MERLON_HINT_PENDING, 0)
#if VERSION <= VERSION_DEMO_JP
    ELSE
    CALL(evt_msg_print, 0, STRING("powerup_ev_03"), 0, STRING("me"))
    END_IF
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("powerup_05"), 0, STRING("me"))
#else

    // Control returns to the player and the shop music resumes.
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 2000)
#endif
    RETURN
EVT_END()

/**
 * Talk event of Merlon in JP and later, run when the player talks to him at the portent point.
 * If the portent is pending he describes it (powerup_ev_01 and 02) while the door opens;
 * otherwise, if Mario has the Up Arrow, asks him to hand it over and runs powerupya_orb_use.
 * Falls back to powerup_ev_03 or powerup_05.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(powerupya_talk)
    IF_INT_EQUAL(GSWF_GOR_MERLON_HINT_PENDING, 1)
        IF_INT_EQUAL(GSWF_GOR_MERLON_STAR_CHART_HINT_DONE, 0)
            CALL(evt_msg_print, 0, STRING("powerup_ev_01"), 0, STRING("me"))
#if VERSION >= VERSION_US
            CALL(evt_cam3d_evt_set, -320, 114, 371, -320, 45, -23, 500, EASING_SINE_IN_OUT)
#else
            CALL(evt_cam3d_evt_set, -355, 114, 371, -355, 45, -23, 500, EASING_SINE_IN_OUT)
#endif
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("powerup_ev_02"), 0, STRING("me"))
            SPAWN_EVENT_GET_ID(PTR(powerupya_direct_talk), LW(10))
            WAIT_MS(500)
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_move_position, STRING("me"), -320, -205, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            LOOP(0)
                IS_EVENT_RUNNING(LW(10), LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
            WAIT_MS(500)
            SET_VALUE(LW(0), PTR(&door_data_01))
            CALL_CHILD(evt_door_entry)
            SET_VALUE(GSWF_GOR_MERLON_STAR_CHART_HINT_DONE, 1)
        ELSE
            CALL(evt_pouch_check_item, ITEM_UP_ARROW, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_win_item_select, 0, PTR(powerupya_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL_CHILD(powerupya_orb_use)
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
#endif

/**
 * Map init for the shop's crystal: hides the red one ("S_i01_sui_aka") until the Up Arrow has been used,
 * then hides the blue one.
 */
static EVT_BEGIN(mapball_init)
    IF_INT_EQUAL(GSWF_GOR_UP_ARROW_USED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i01_sui_aka"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i01_sui_ao"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

/**
 * Init event of Merlon in his shop: registers the front-facing case event that runs powerupya_table_talk
 * when Mario speaks to him at "A_power_up", and while the Up Arrow portent is pending lines up Merlon
 * and the shop door for the scene.
 */
static EVT_BEGIN(powerupya_init)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_power_up"), 0, PTR(powerupya_table_talk), 0)
    IF_INT_EQUAL(GSWF_GOR_MERLON_HINT_PENDING, 1)
        IF_INT_EQUAL(GSWF_GOR_MERLON_STAR_CHART_HINT_DONE, 0)
            CALL(evt_npc_set_position, STRING("パワーアップ屋"), -320, 0, -10)
            SET_VALUE(LW(0), PTR(&door_data_01))
            CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
            END_IF
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(evt_hitobj_onoff, LW(2), 1, 0)
            END_IF
            RETURN
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_01))
    CALL_CHILD(evt_door_entry)
    RETURN
EVT_END()

/**
 * Talk event of Grifty (情報屋完璧), the traveling minstrel who sells tales: after his greeting (johoyaperfect_00,
 * or 00_01 on later talks in JP and later) Mario picks a tale from a list (johoya_data_alloc,
 * evt_win_other_select 17).
 * A tale already heard is retold for free (johoyaperfect_06); a new one is offered for its coin price
 * (johoyaperfect_01 and 02), refused when Mario is short (09), otherwise paid for, told and marked as read.
 * The data is then freed; JP and later loop back to the list.
 */
static EVT_BEGIN(johoyaperfect_talk)
    IF_INT_EQUAL(GSWF_GOR_GRIFTY_FIRST_TALK_DONE, 0)
        CALL(evt_msg_print, 0, STRING("johoyaperfect_00"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_GRIFTY_FIRST_TALK_DONE, 1)
#if VERSION <= VERSION_DEMO_JP
    END_IF
        CALL(johoya_data_alloc, STRING("jperfect_code"))
        CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
        CALL(evt_win_other_select, SELECT_TYPE_INFORMANT_PERFECT)
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
            CALL(evt_msg_print_insert, 0, STRING("johoyaperfect_01"), 0, STRING("me"), LW(3), LW(2), LW(4))
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
            END_IF
        END_IF
        CALL(johoya_data_free)
#else
    ELSE
        CALL(evt_msg_print, 0, STRING("johoyaperfect_00_01"), 0, STRING("me"))
    END_IF
    LOOP(0)
        CALL(johoya_data_alloc, STRING("jperfect_code"))
        CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
        CALL(evt_win_other_select, SELECT_TYPE_INFORMANT_PERFECT)
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
#if VERSION >= VERSION_US
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
#endif
            CALL(evt_msg_print_insert, 0, STRING("johoyaperfect_01"), 0, STRING("me"), LW(3), LW(2), LW(4))
#if VERSION >= VERSION_US
                CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("johoyaperfect_01"), LW(4))
                CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"), LW(3), LW(2))
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
#endif
    RETURN
EVT_END()

/**
 * Init event of Professor Frankly (Kuri-hakase): lets him pass through walls and places him by story stage:
 * at (-135, 0, -240) at STG0_08_STOLEN_COINS and at (-220, 0, 305)
 * while he waits to start his tutorial (unless GSWF_GOR_02_TUTORIAL_PIPE_TAKEN is set),
 * hidden below the map during the early scenes and the late story,
 * at (-140, 0, -225) in his house at the Pedestal step and at each Thousand-Year Door step,
 * and at (-140, 0, -250) otherwise.
 */
static EVT_BEGIN(kurihakase_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG0_08_STOLEN_COINS)
        CALL(evt_npc_set_position, STRING("me"), -135, 0, -240)
        CASE_EQUAL(STG0_09_PROF_FRANKLY)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_EQUAL(STG0_10_REMOVING_THE_FENCE)
        CALL(evt_npc_set_position, STRING("me"), -220, 0, 305)
        CASE_EQUAL(STG0_10_01_FRANKLYS_TUTORIAL)
        IF_INT_EQUAL(GSWF_GOR_02_TUTORIAL_PIPE_TAKEN, 0)
            CALL(evt_npc_set_position, STRING("me"), -220, 0, 305)
        ELSE
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_IF
        CASE_BETWEEN(STG0_10_01_FRANKLYS_TUTORIAL, STG0_14_FRONT_OF_THE_PEDESTAL)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG7_25_THE_LAST_MAIL, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_EQUAL(STG0_15_THE_PEDESTAL)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG1_30_ZESS_T)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG2_40_THOUSAND_YEAR_DOOR)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG3_50_THOUSAND_YEAR_DOOR)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG4_30_THOUSAND_YEAR_DOOR)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG5_60_RETURN_TO_KEELHAUL)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_EQUAL(STG6_70_THOUSAND_YEAR_DOOR)
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -225)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), -140, 0, -250)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kurihakase_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG0_10_01_FRANKLYS_TUTORIAL)

        // Frankly's line for the story stage: after the tutorial, the Prologue.
        CALL(evt_msg_print, 0, STRING("mac_0_085"), 0, STRING("me"))
        CASE_LESS(STG1_00_PETAL_MEADOWS)
        CALL(evt_msg_print, 0, STRING("gor_02_013"), 0, STRING("me"))
        CASE_LESS(STG1_18_PEACH_CAUGHT)
        CALL(evt_msg_print, 0, STRING("gor_02_014"), 0, STRING("me"))
        CASE_BETWEEN(STG1_17_FATHER_SON_REUNITED, STG1_30_ZESS_T)

        // Chapter 1: after the Petal Meadows trip.
        CALL(evt_msg_print, 0, STRING("mac_1_000"), 0, STRING("me"))
        CASE_LESS(STG2_00_BOGGLY_WOODS)
        CALL(evt_msg_print, 0, STRING("gor_02_015"), 0, STRING("me"))
        CASE_LESS(STG2_25_01_PEACH_CRUMP_X_NAUTS)
        CALL(evt_msg_print, 0, STRING("gor_02_016"), 0, STRING("me"))
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG2_40_THOUSAND_YEAR_DOOR)

        // Chapter 2.
        CALL(evt_msg_print, 0, STRING("mac_2_000"), 0, STRING("me"))
        CASE_LESS(STG3_00_WELCOME_TO_GLITZVILLE)
        CALL(evt_msg_print, 0, STRING("gor_02_017"), 0, STRING("me"))
        CASE_LESS(STG3_33_PEACH_THE_SECRET_WEAPON)
        CALL(evt_msg_print, 0, STRING("gor_02_018"), 0, STRING("me"))
        CASE_BETWEEN(STG3_33_PEACH_THE_SECRET_WEAPON, STG3_50_THOUSAND_YEAR_DOOR)

        // Chapter 3.
        CALL(evt_msg_print, 0, STRING("mac_3_000"), 0, STRING("me"))
        CASE_LESS(STG4_00_WELCOME_TO_STAGE_4)
        IF_INT_EQUAL(GSWF_TIK_08_PIPE_REJECTED, 0)
            CALL(evt_msg_print, 0, STRING("gor_02_019"), 0, STRING("me"))
        ELSE
            IF_INT_EQUAL(GSWF_GOR_FRANKLY_DARKLY_HINT_DONE, 0)
                // After the Twilight Town pipe rejects Mario: the partner speaks up, Frankly stares in surprise,
                // then answers.
                CALL(evt_party_stop, 0)
                CALL(evt_msg_print_party, STRING("mac_3_008"))
                CALL(evt_msg_print, 0, STRING("mac_3_009"), 0, STRING("me"))
                WAIT_MS(1500)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 60)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("mac_3_010"), 0, STRING("me"))
                SET_VALUE(GSWF_GOR_FRANKLY_DARKLY_HINT_DONE, 1)
                CALL(evt_party_run, 0)
            ELSE
                // Later Chapter 3 lines.
                CALL(evt_msg_print, 0, STRING("gor_02_020"), 0, STRING("me"))
            END_IF
        END_IF
        CASE_BETWEEN(STG4_15_02_BACK_TO_NORMAL, STG4_30_THOUSAND_YEAR_DOOR)

        // Chapter 4, before the Bobbery story steps.
        CALL(evt_msg_print, 0, STRING("mac_4_000"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_021"), 0, STRING("me"))
        CASE_LESS(STG4_33_MEET_BOBBERY)
        CALL(evt_msg_print, 0, STRING("gor_02_022"), 0, STRING("me"))
        CASE_EQUAL(STG4_33_MEET_BOBBERY)
        CALL(evt_msg_print, 0, STRING("mac_4_048"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_BOBBERY_HOME_LEARNED, 1)
        CASE_EQUAL(STG4_34_BOBBERY_DECLINES)
        CALL(evt_msg_print, 0, STRING("mac_4_060"), 0, STRING("me"))
        CASE_BETWEEN(STG5_44_PEACH_TECS_REQUEST_2, STG5_60_RETURN_TO_KEELHAUL)

        // Chapter 5.
        CALL(evt_msg_print, 0, STRING("mac_5_000"), 0, STRING("me"))
        CASE_LESS(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_msg_print, 0, STRING("gor_02_023"), 0, STRING("me"))
        CASE_LESS(STG6_00_START)
        CALL(evt_msg_print, 0, STRING("gor_02_024"), 0, STRING("me"))
        CASE_LESS(STG6_55_PEACH_GRODUS_X_NAUT)
        CALL(evt_msg_print, 0, STRING("gor_02_025"), 0, STRING("me"))
        CASE_BETWEEN(STG6_55_PEACH_GRODUS_X_NAUT, STG6_70_THOUSAND_YEAR_DOOR)

        // Chapter 6.
        CALL(evt_msg_print, 0, STRING("mac_6_000"), 0, STRING("me"))
        CASE_EQUAL(STG6_71_FRANKLYS_HOUSE_STG_6)

        // Frankly's lines now depend on how much time has passed since the stored timestamp.
        CALL(evt_sub_load_progresstime, GSW_GOR_FRANKLY_STG6_START_TIME, LW(0))
        IF_INT_EQUAL(GSWF_GOR_FRANKLY_STG6_SAID_WAIT, 1)
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
            // After at least three minutes, the screen goes to letterbox and the follow-up scene starts.
            CALL(evt_cam_letter_box_disable, 1)
            CALL(evt_cam_letter_box_onoff, 1, 0)
            WAIT_MS(1000)
            SPAWN_EVENT(kurihakase_after3minutes)
        END_IF
#if VERSION <= VERSION_DEMO_JP
        CASE_LESS(STG7_03_GOLDBOB)
#else
        CASE_LESS(STG7_02_MAYOR_BOB)
        IF_INT_EQUAL(GSWF_BOM_FIRST_ARRIVAL_DONE, 0)
#endif
            CALL(evt_pouch_check_item, ITEM_ULTRA_HAMMER, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 0, STRING("gor_02_026_01"), 0, STRING("me"))
            ELSE
                CALL(evt_msg_print, 0, STRING("gor_02_026"), 0, STRING("me"))
            END_IF
#if VERSION <= VERSION_DEMO_JP
            CASE_LESS(STG7_04_BACK_ONLINE)
#else
            ELSE
#endif
            CALL(evt_msg_print, 0, STRING("gor_02_027"), 0, STRING("me"))
#if VERSION >= VERSION_JP
        END_IF
#endif
        CASE_LESS(STG7_25_00_BOWSER_POSHLEY_HEIGHTS)

        // Frankly's late-story line.
        CALL(evt_msg_print, 0, STRING("gor_02_028"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(STG9_00_EPILOGUE)

        // In the epilogue, Frankly reacts to Mario's poses on their first talk.
        IF_INT_EQUAL(GSWF_GOR_FRANKLY_EPILOGUE_TALK_DONE, 0)
            CALL(evt_msg_print, 0, STRING("gor_02_029"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("gor_02_029_01"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_N"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("gor_02_029_02"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            SET_VALUE(GSWF_GOR_FRANKLY_EPILOGUE_TALK_DONE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_029_03"), 0, STRING("me"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the bandit gang's boss ("盗賊団親分"): lets him walk through walls. */
static EVT_BEGIN(oyabun_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

static EVT_BEGIN(oyabun_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(STG2_40_THOUSAND_YEAR_DOOR)
        IF_INT_EQUAL(GSWF_GOR_ISHNAIL_FIRST_TALK_DONE, 0)
            // Before the Pianta Parlor story step: a first line, then a repeat line on later visits.
            CALL(evt_msg_print, 0, STRING("gor_02_000"), 0, STRING("me"))
            SET_VALUE(GSWF_GOR_ISHNAIL_FIRST_TALK_DONE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("gor_02_001"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(STG2_41_FRANKLYS_HOUSE_STG_2)
        IF_INT_EQUAL(GSWF_GOR_ISHNAIL_PASSWORD_TOLD, 1)
            // At the step where Mario needs to find the Parlor's secret:
            // the boss repeats the shop and password instructions for reaching Don Pianta.
            CALL(evt_msg_print, 0, STRING("mac_2_040"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF_GOR_ISHNAIL_DON_PIANTA_TALK_DONE, 0)
            // Danger music starts and Mario answers with a hand-up pose between the boss's lines.
            CALL(evt_snd_bgmoff, PSND_PAUSE)
            CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_DANGER3"))
            CALL(evt_snd_envoff, PSND_PAUSE)
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
            SET_VALUE(GSWF_GOR_ISHNAIL_DON_PIANTA_TALK_DONE, 1)
        ELSE
            CALL(evt_snd_envoff, PSND_PAUSE)
            CALL(evt_msg_print, 0, STRING("mac_2_016"), 0, STRING("me"))
        END_IF
        BEGIN_INLINE_EVENT
            // The boss offers to sell his inside information for 64 coins; Mario can agree or refuse.
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
                // If Mario cannot pay he is turned away; if he agrees the coins are taken,
                // with the coin gauge updating.
                CALL(evt_pouch_get_coin, LW(3))
                IF_INT_LESS(LW(3), 64)
                    CALL(evt_msg_print_add, 0, STRING("mac_2_020"))
                    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
                    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
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

                    // Mario pays and the boss crows over the sale.
                    CALL(evt_msg_print, 0, STRING("mac_2_019"), 0, STRING("me"))
                    GOTO(38)
                END_IF
            ELSE
                // Mario changes his mind or dithers: the boss throws him out.
                CALL(evt_msg_print_add, 0, STRING("mac_2_021"))
                CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
                CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
                RETURN
            END_IF
        ELSE
            CALL(evt_msg_print_add, 0, STRING("mac_2_022"))
            CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
            CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
            RETURN
        END_IF

        // The sale: the camera moves in on the boss and he gives the hint about the Pianta Parlor.
        LABEL(38)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_cam3d_evt_set, LW(0), 72, -753, LW(0), 52, -600, 500, EASING_SINE_IN_OUT)
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
        SET_VALUE(GSWF_GOR_ISHNAIL_PASSWORD_TOLD, 1)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        CASE_BETWEEN(STG2_44_FINDING_DON_PIANTA, STG2_END_TO_GLITZVILLE)

        // After the hint: one line per story stage.
        CALL(evt_msg_print, 0, STRING("gor_02_002"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_003"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_003_01"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_003_02"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_003_03"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_003_04"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_003_05"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_003_06"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of Gus, the bandit gang member who blocks the way in the east alley:
 * before he is beaten he stands at (-82, 0, 134);
 * afterwards at (-153, 0, -459) facing right with the center wall open. Clears GW(10).
 */
static EVT_BEGIN(tozokudan1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    IF_INT_EQUAL(GSWF_GOR_GATEKEEPER_DEFEATED, 1)
        CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
        CALL(evt_npc_set_position, STRING("me"), -153, 0, -459)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    ELSE
        CALL(evt_npc_set_position, STRING("me"), -82, 0, 134)
    END_IF
    SET_VALUE(GW(10), 0)
    RETURN
EVT_END()

static EVT_BEGIN(tozokudan1_talk)
    IF_INT_EQUAL(GSWF_GOR_GATEKEEPER_DEFEATED, 0)
        IF_INT_EQUAL(GW(10), 1)
            CALL(evt_msg_print, 0, STRING("gor_02_006_01"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF_GOR_GATEKEEPER_FIRST_TALK_DONE, 0)
            CALL(evt_msg_print, 0, STRING("gor_02_004"), 0, STRING("me"))
            SET_VALUE(GSWF_GOR_GATEKEEPER_FIRST_TALK_DONE, 1)
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
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_move_position, STRING("me"), -102, 66, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
            IF_INT_EQUAL(LW(0), BATTLE_RESULT_WIN)
                CALL(evt_msg_print, 0, STRING("gor_02_009"), 0, STRING("me"))
                CALL(evt_hitobj_onoff, STRING("A_center_kabe_atr"), 1, 0)
                CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
                CALL(evt_npc_set_ry, STRING("me"), 90)
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_MONBAN_ESCAPE1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_npc_move_position, STRING("me"), 150, 155, 0, FLOAT(240.0f), 0)
                CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
                CALL(evt_npc_set_position, STRING("me"), -153, 0, -459)
                SET_VALUE(GSWF_GOR_GATEKEEPER_DEFEATED, 1)
            ELSE
                CALL(evt_msg_print, 0, STRING("gor_02_008_00"), 0, STRING("me"))
            END_IF
            CALL(evt_mario_key_onoff, 1)
        END_SWITCH
    ELSE
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(STG1_17_FATHER_SON_REUNITED)
            CALL(evt_msg_print, 0, STRING("gor_02_010"), 0, STRING("me"))
            CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
            CALL(evt_msg_print, 0, STRING("gor_02_010_01"), 0, STRING("me"))
            CASE_LESS(STG3_32_BOSS_BATTLE)
            CALL(evt_msg_print, 0, STRING("gor_02_010_02"), 0, STRING("me"))
            CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
            CALL(evt_msg_print, 0, STRING("gor_02_010_03"), 0, STRING("me"))
            CASE_LESS(STG5_43_X_NAUTS_RETREAT)
            CALL(evt_msg_print, 0, STRING("gor_02_010_04"), 0, STRING("me"))
            CASE_LESS(STG6_54_WELCOME_BACK)
            CALL(evt_msg_print, 0, STRING("gor_02_010_05"), 0, STRING("me"))
            CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
            CALL(evt_msg_print, 0, STRING("gor_02_010_06"), 0, STRING("me"))
            CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
            CALL(evt_msg_print, 0, STRING("gor_02_010_07"), 0, STRING("me"))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("gor_02_010_08"), 0, STRING("me"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

/**
 * Init event of the thief at the house with the lock (Trouble Center job 1):
 * at (445, 0, 120) while the job is active, otherwise at (10, 0, -460).
 */
static EVT_BEGIN(tozokudan2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEED_A_KEY)
        CALL(evt_npc_set_position, STRING("me"), 445, 0, 120)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), 10, 0, -460)
    RETURN
EVT_END()

/** Items the item picker offers for the thief's door lock: the House Key; -1 ends the list. */
static s32 key_tbl[] = {
    ITEM_HOUSE_KEY, -1,
};

/**
 * Check for the thief's house door lock ("me"):
 * if Mario carries the House Key (ITEM_HOUSE_KEY), opens the item select window with key_tbl;
 * cancelling it cancels the lock execution so the door stays locked.
 */
static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, ITEM_HOUSE_KEY, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_win_item_select, PTR(key_tbl), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Animation of the thief's house door (door_data_06) opening to about 108 degrees
 * and closing again over about a second, with door sounds, hiding the interior when finished.
 */
static EVT_BEGIN(tozokudan2_house_door)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_06_naka"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), PTR(&door_data_06))
    SPAWN_EVENT(door_open_se)
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
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
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(door_close_se)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 180, 0, 500)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_06_naka"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Moves Mario to x=415 or x=480 over 300 ms, keeping his z, and moves the partners to the same side with z offsets,
 * when they stand in the thief's path as he jumps down; picks the side by Mario's x relative to the thief.
 */
static EVT_BEGIN(mario_yokeru)
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

static EVT_BEGIN(evt_unlock_mario)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("irai_01_03"), 0, STRING("盗賊団２"))
    CALL(evt_sub_get_coin, 20)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    SPAWN_EVENT(mario_yokeru)
    CALL(evt_npc_jump_position_nohit, STRING("盗賊団２"), 445, 8, 80, 300, 30)
    WAIT_MS(500)
    SPAWN_EVENT(tozokudan2_house_door)
    CALL(evt_npc_move_position, STRING("盗賊団２"), 445, -85, 0, FLOAT(60.0f), 0)
    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_NEED_A_KEY), 1)
    SET_VALUE(LW(0), PTR(&door_data_06))
    CALL_CHILD(evt_door_entry)
    CALL(evt_npc_flag_onoff, 0, STRING("盗賊団２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Runs when the thief's house lock is opened: waits 500 ms and starts evt_unlock_mario,
 * the ending of Trouble Center job 1.
 */
static EVT_BEGIN(evt_unlock)
    WAIT_MS(500)
    SPAWN_EVENT(evt_unlock_mario)
    RETURN
EVT_END()

/**
 * Trouble Center job 1 ending when Mario hands over the House Key:
 * the thief thanks him, pays 20 coins, the lock opens and he goes in;
 * the key is removed and the job marked solved.
 */
static EVT_BEGIN(evt_unlock_npc)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("irai_01_03"), 0, STRING("盗賊団２"))
    CALL(evt_sub_get_coin, 20)
    SPAWN_EVENT(mario_yokeru)
    CALL(evt_npc_jump_position_nohit, STRING("盗賊団２"), 445, 8, 80, 300, 30)
    WAIT_MS(500)
    CALL(evt_mobj_set_position, STRING("mobj_lock00"), 0, -1000, 0)
    SET_VALUE(GSWF_GOR_02_HOUSE_KEY_LOCK_UNLOCKED, 1)
    CALL(evt_npc_get_position, STRING("盗賊団２"), LW(0), LW(1), LW(2))
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_LOCK_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_UNLOCK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_pouch_remove_item, ITEM_HOUSE_KEY, LW(0))
    SPAWN_EVENT(tozokudan2_house_door)
    CALL(evt_npc_move_position, STRING("盗賊団２"), 445, -85, 0, FLOAT(60.0f), 0)
    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_NEED_A_KEY), 1)
    SET_VALUE(LW(0), PTR(&door_data_06))
    CALL_CHILD(evt_door_entry)
    CALL(evt_npc_flag_onoff, 0, STRING("盗賊団２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    RETURN
EVT_END()

/**
 * Talk event of the thief at the locked house.
 * While Trouble Center job 1 is active, asks for the House Key (irai_01_00 to 02)
 * and lets Mario hand it over (evt_unlock_npc); otherwise a line per story stage.
 */
EVT_BEGIN(tozokudan2_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEED_A_KEY)
        CALL(evt_pouch_check_item, ITEM_HOUSE_KEY, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_msg_print, 0, STRING("irai_01_02"), 0, STRING("me"))
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_win_item_select, PTR(key_tbl), LW(0))
#else
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
#endif
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL_CHILD(evt_unlock_npc)
            END_IF
            RETURN
        ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_msg_print, 0, STRING("irai_01_00"), 0, STRING("me"))
                SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
            ELSE
                CALL(evt_msg_print, 0, STRING("irai_01_01"), 0, STRING("me"))
            END_IF
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_02_011"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_011_01"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_011_02"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_011_03"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_011_04"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_011_05"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_011_06"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_011_07"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the gang member at the back door of the Trouble Center: stands at (220, 0, -185) facing right. */
static EVT_BEGIN(tozokudan3_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("me"), 220, 0, -185)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

/** Talk event of the gang member at the back door: first line, then a repeat line (irai_oku_00 and 01). */
static EVT_BEGIN(tozokudan3_talk)
    IF_INT_EQUAL(GSWF_GOR_TROUBLE_CENTER_BACKWAY_TALK, 0)
        CALL(evt_msg_print, 0, STRING("irai_oku_00"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_TROUBLE_CENTER_BACKWAY_TALK, 1)
    ELSE
        CALL(evt_msg_print, 0, STRING("irai_oku_01"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

/**
 * Makes Sanders walk to Bobbery's door and open it (50, 25) and sets GSWF_GOR_BOBBERY_DOOR_UNLOCKED,
 * then says mac_4_056_01. Does nothing if the door is already open.
 */
static EVT_BEGIN(sanders_open_door)
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 1)
        RETURN
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_move_position, STRING("me"), 0, 25, 0, FLOAT(60.0f), 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_MOBJ_UNLOCK1"), 0)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("me"), 50, 25, 0, FLOAT(60.0f), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(set_talk_memory_ry)
    SET_VALUE(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 1)
    CALL(evt_msg_print, 0, STRING("mac_4_056_01"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Init event of Sanders (Bobbery) at his house: lets him pass through walls and places him by story stage:
 * at (0, 0, -50) before Mario meets Bobbery, and until his decision at (0, 0, -50)
 * or (50, 0, -30) depending on GSWF_GOR_BOBBERY_DOOR_UNLOCKED (whether the door has been opened).
 */
static EVT_BEGIN(sanders_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_33_MEET_BOBBERY)
        CALL(evt_npc_set_position, STRING("me"), 0, 0, -50)
        CASE_LESS(STG4_36_BOBBERYS_DECISION)
        IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
            CALL(evt_npc_set_position, STRING("me"), 0, 0, -50)
        ELSE
            CALL(evt_npc_set_position, STRING("me"), 50, 0, -30)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of Sanders (Bobbery) at his house, by Chapter 4 story step:
 * before STG4_33_MEET_BOBBERY he speaks mac_4_049_01 and opens the door (sanders_open_door);
 * at STG4_33_MEET_BOBBERY he speaks mac_4_050 and opens the door until Mario has learned where he lives;
 * once that is known, the Bobbery talk (mac_4_051 to mac_4_056) runs with the partner,
 * leading to STG4_34_BOBBERY_DECLINES; at the letter step Mario hands him the old letter (item00, "tegami")
 * and the letter is read (mac_4_077 to mac_4_083).
 */
static EVT_BEGIN(sanders_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_33_MEET_BOBBERY)

        // Before Mario meets Bobbery: Sanders brushes him off and opens the door.
        CALL(evt_msg_print, 0, STRING("mac_4_049_01"), 0, STRING("me"))
        CALL_CHILD(sanders_open_door)
        CASE_EQUAL(STG4_33_MEET_BOBBERY)
        IF_INT_EQUAL(GSWF_GOR_BOBBERY_HOME_LEARNED, 0)
            // Before Mario has learned where Bobbery lives, he claims never to have heard of the Admiral.
            CALL(evt_msg_print, 0, STRING("mac_4_050"), 0, STRING("me"))
            CALL_CHILD(sanders_open_door)
            SET_VALUE(GSWF_GOR_BOBBERY_FIRST_DENIAL_HEARD, 1)
        ELSE
            // Once Mario has learned where Bobbery lives, Mario and the partner press him about being the Admiral
            // and about a navigator job; he refuses ("no means no").
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print, 0, STRING("mac_4_051"), 0, STRING("me"))
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_msg_print_party, STRING("mac_4_052"))
#if VERSION < VERSION_DEMO_EU
            CALL(evt_party_run, 0)
#endif
            CALL(evt_msg_print, 0, STRING("mac_4_053"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("mac_4_054"), 0, STRING("me"))
            CALL(evt_msg_print_party, STRING("mac_4_055"))
#if VERSION >= VERSION_DEMO_EU
            CALL(evt_party_run, 0)
#endif
            CALL(evt_msg_print, 0, STRING("mac_4_056"), 0, STRING("me"))
            CALL_CHILD(sanders_open_door)
            SET_VALUE(GSW_STORY_PROGRESS, STG4_34_BOBBERY_DECLINES)
        END_IF
        CASE_EQUAL(STG4_34_BOBBERY_DECLINES)

        // He refuses again before the letter step.
        CALL(evt_msg_print, 0, STRING("mac_4_057"), 0, STRING("me"))
        CASE_EQUAL(STG4_35_ADVICE_FROM_PODLEY)

        // At the letter step: Mario, the partner and Sanders line up and Mario hands over the old letter.
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
        CALL(evt_item_entry, STRING("item00"), ITEM_OLD_LETTER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
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

        // Sanders is puzzled by the letter, then recognizes the handwriting: it is from Scarlette.
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

        // The music turns sentimental; Sanders reads the letter aloud and breaks down.
        CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
        CALL(evt_msg_print, 0, STRING("mac_4_079"), 0, STRING("me"))
        CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
        CALL(evt_item_entry, STRING("tegami"), ITEM_VITAL_PAPER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_item_get_position, STRING("tegami"), LW(2), LW(3), LW(4))
            ADD(LW(4), 10)
            SET_VALUE(LW(5), LW(2))
            ADD(LW(5), 30)
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 1000, 300)
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
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_ORGEL1"))
        CALL(evt_msg_print, 0, STRING("mac_4_080"), 0, STRING("me"))
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_081"), 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_082"), 0, STRING("me"))
        CALL(evt_item_set_position, STRING("tegami"), 0, -1000, 0)
        WAIT_MS(300)

        // After asking for a moment alone, he walks to his door and opens it while the others wait; then closes it again.
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_move_position, STRING("me"), 0, -140, 0, FLOAT(60.0f), 0)
        SPAWN_EVENT(evt_gor_door_open_sanders)
        CALL(evt_npc_move_position, STRING("me"), 0, -90, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), 0, -193, 0, FLOAT(60.0f), 0)
        WAIT_MS(3000)
        CALL(evt_mario_set_dir, 270, 200, 0)
        WAIT_MS(2000)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_4_083"), 0, 0)
        WAIT_MS(2000)
        SPAWN_EVENT(evt_gor_door_close_sanders)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CALL(evt_npc_move_position, STRING("me"), 0, -80, 0, FLOAT(60.0f), 0)
        CALL(evt_npc_move_position, STRING("me"), 50, -30, 0, FLOAT(60.0f), 0)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        WAIT_MS(500)

        // He agrees to be the navigator; the partner reacts and the old letter is removed from the inventory.
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
        CALL(evt_msg_print, 0, STRING("mac_4_084"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_4_085"), 0, STRING("me"))
#if VERSION >= VERSION_DEMO_EU
        CALL(evt_party_stop, 0)
        CALL(evt_msg_print_party, STRING("mac_4_086"))
        CALL(evt_party_run, 0)
#else
        CALL(evt_msg_print_party, STRING("mac_4_086"))
#endif
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)

        // Bobbery joins the party and the map music resumes.
        CALL(evt_party_stop, 1)
        CALL(evt_mario_set_party_pos, 1, PARTY_BOBBERY_FOLLOWER, LW(0), LW(1), LW(2))
        CALL(evt_party_run, 1)
        CALL(evt_pouch_remove_item, ITEM_OLD_LETTER, LW(0))
        CALL(evt_party_run, 0)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG4_36_BOBBERYS_DECISION)
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the fourth Goomba villager: placed at (-370, 0, 130). */
static EVT_BEGIN(kuribo4_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("me"), -370, 0, 130)
    RETURN
EVT_END()

/** Regular event of the fourth Goomba villager: wanders around his post using urouro_main_func. */
static EVT_BEGIN(kuribo4_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(70.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/** Items the item picker offers the fourth Goomba for Trouble Center job 2: the Box; -1 ends the list. */
static s32 item_tbl[] = {
    ITEM_BOX, -1,
};

EVT_BEGIN(kuribo4_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SAFE_DELIVERY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
            // Trouble Center job 2: if Mario carries a Box, the Goomba asks for it
            // and takes the one Mario hands over.
            CALL(evt_pouch_check_item, ITEM_BOX, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(evt_msg_print, 0, STRING("irai_02_03"), 0, STRING("me"))
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_win_item_select, PTR(item_tbl), LW(0))
#else
                CALL(evt_win_item_select, 0, PTR(item_tbl), LW(0), 0)
#endif
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_msg_print, 0, STRING("irai_02_04"), 0, STRING("me"))
                    CALL(evt_pouch_remove_item, ITEM_BOX, LW(0))
                    SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                END_IF
                RETURN
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_WANTED)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            // Trouble Center job 13: the Goomba wants Battle Trunks;
            // Mario hands them over one at a time from a list.
            CALL(evt_pouch_check_item, ITEM_BATTLE_TRUNKS, LW(0))
            IF_INT_GREATER(LW(0), 0)
                CALL(iri_13_make_item_tbl)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                    // He misunderstands the job and Mario nods and shrugs between his lines.
                    CALL(evt_msg_print, 0, STRING("irai_13_001"), 0, 0)
                    CALL(evt_mario_set_pose, STRING("M_N_5B"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_N_C"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print, 0, STRING("irai_13_002"), 0, 0)
                    CALL(iri_13_update_item_tbl, LW(1), LW(0))
                    LOOP(0)
#if VERSION <= VERSION_DEMO_JP
                        CALL(evt_win_item_select, LW(1), LW(0))
#else
                        CALL(evt_win_item_select, 0, LW(1), LW(0), 0)
#endif
                        IF_INT_NOT_EQUAL(LW(0), ITEM_BATTLE_TRUNKS)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_pouch_remove_item, ITEM_BATTLE_TRUNKS, LW(0))
                        CALL(iri_13_update_item_tbl, LW(1), LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_msg_print, 0, STRING("irai_13_003"), 0, 0)
                    END_LOOP
                    SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
                ELSE
                    LOOP(0)
                        CALL(iri_13_update_item_tbl, LW(1), LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_msg_print, 0, STRING("irai_13_005"), 0, 0)
#if VERSION <= VERSION_DEMO_JP
                        CALL(evt_win_item_select, LW(1), LW(0))
#else
                        CALL(evt_win_item_select, 0, LW(1), LW(0), 0)
#endif
                        IF_INT_NOT_EQUAL(LW(0), ITEM_BATTLE_TRUNKS)
                            BREAK_LOOP
                        END_IF
                        CALL(evt_pouch_remove_item, ITEM_BATTLE_TRUNKS, LW(0))
                    END_LOOP
                END_IF

                // If no trunks remain, he thanks Mario; the list is freed even if Mario cancelled the handover.
                CALL(evt_pouch_check_item, ITEM_BATTLE_TRUNKS, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_13_004"), 0, 0)
                END_IF
                CALL(iri_13_free_item_tbl)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)

        // Otherwise: a line per story stage.
        CALL(evt_msg_print, 0, STRING("gor_02_030_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_030_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_030_02_01"), 0, STRING("me"))
        CASE_LESS(STG3_51_FRANKLYS_HOUSE_STG_3)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03"), 0, STRING("me"))
        CASE_LESS(STG3_52_DARKLY)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03_01"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_030_03_02"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_030_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_030_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_030_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_030_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_030_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the third Chusan: empty. */
static EVT_BEGIN(chusan3_init)
    RETURN
EVT_END()

static EVT_BEGIN(chusan3_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_02_031_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_031_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_031_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_031_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_031_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_031_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_031_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_031_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_031_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the third street-stall seller: empty. */
static EVT_BEGIN(roten3_init)
    RETURN
EVT_END()

static EVT_BEGIN(roten3_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_PRICE_ADJUSTMENT)
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
        SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
        SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_PRICE_ADJUSTMENT), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_02_032_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_032_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_032_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_032_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_032_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_032_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_032_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_032_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_032_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the first 'borodo' villager: stands at (90, 0, 150),
 * or at (475, 0, 275) (550 in earlier releases) between Chapters 4 and 5.
 */
static EVT_BEGIN(borodo1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_43_X_NAUTS_RETREAT)
#if VERSION >= VERSION_US
        CALL(evt_npc_set_position, STRING("me"), 475, 0, 275)
#else
        CALL(evt_npc_set_position, STRING("me"), 550, 0, 275)
#endif
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 90, 0, 150)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Regular event of the first 'borodo' villager:
 * wanders around his post, except during Chapter 4 to 5 when he stays put.
 */
static EVT_BEGIN(borodo1_regl)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_43_X_NAUTS_RETREAT)
        CASE_DEFAULT
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(70.0f), FLOAT(30.0f), 0)
        CALL(urouro_main_func, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(borodo1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_02_033_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_033_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_033_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_033_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_033_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_033_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_033_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_033_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_033_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(borodo2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_FIND_THIS_GUY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
            RETURN
        END_IF
        CALL(evt_npc_set_position, STRING("me"), -475, 0, -180)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), -475, 0, -180)
    RETURN
EVT_END()

/**
 * Borodo runs away: with Mario's controls locked he becomes intangible and faces 180, runs to (-475, -85),
 * (-530, -85) and (-530, 20) at speed 240, vanishes below the map and Mario gets control back after 1.5 seconds.
 */
static EVT_BEGIN(borodo2_escape)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_FIND_THIS_GUY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_msg_print, 0, STRING("irai_04_003"), 0, STRING("me"))
                SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                SPAWN_EVENT(borodo2_escape)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_00_PETAL_MEADOWS)
        CALL(evt_msg_print, 0, STRING("gor_02_033_00_2"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_033_01_2"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_033_02_2"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_033_03_2"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_033_04_2"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_033_05_2"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_033_06_2"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_033_07_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_02_033_08_2"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of Kuragarisan (Darkly): lets him walk through walls. */
static EVT_BEGIN(kuragarisan_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Cutscene at STG3_51_FRANKLYS_HOUSE_STG_3 (run by kuragarisan_talk once the Twilight Town pipe has been
 * reached)
 * in which Kuragarisan (Darkly) tells Mario he needs something with his name on it to pass the pipe to Twilight
 * Town and writes "MARIO" on the seat of his overalls,
 * then the partner's name on the partner (animations A_1A to
 * A_1C, mac_3_011 to 019; with Yoshi his chosen name goes into his line). The partner's walk to him times out
 * after 10 seconds. The story then becomes STG3_52_DARKLY.
 */
static EVT_BEGIN(kuragarisan_write_name)
    // The partner is moved to the left and walks over to Mario while Darkly looks on.
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

    // Darkly says Mario needs something with his name on it to get through the pipe; Mario nods,
    // then shakes his head.
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

    // Darkly asks the partner to come over; if the walk has not finished when the 10-second timer expires,
    // the partner is placed next to Mario.
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

    // Darkly walks up to Mario and writes "MARIO" on the seat of his overalls with a pen.
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
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

    // He tells Mario the writing is done; the partner reacts (Yoshi's chosen name is worked into his line).
    CALL(evt_msg_print, 0, STRING("mac_3_016"), PTR(kuragarisan_msgcb), STRING("me"))
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), PARTY_YOSHI)
        CALL(evt_pouch_get_yoshiname, LW(1))
        CALL(evt_msg_print_insert, 0, STRING("mac_3_017_yos"), 0, STRING("party"), LW(1))
    ELSE
        CALL(evt_msg_print_party, STRING("mac_3_017"))
    END_IF
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_party, LW(3))
    SWITCH(LW(3))
        CASE_EQUAL(PARTY_GOOMBELLA)
        SET_VALUE(LW(3), 22)
        CASE_EQUAL(PARTY_KOOPS)
        SET_VALUE(LW(3), 23)
        CASE_EQUAL(PARTY_YOSHI)
        SET_VALUE(LW(3), 15)
        CASE_EQUAL(PARTY_FLURRIE)
        SET_VALUE(LW(3), 31)
        CASE_DEFAULT
        SET_VALUE(LW(3), 0)
    END_SWITCH
    ADD(LW(0), 5)
    SUBTRACT(LW(2), LW(3))

    // Then he writes the partner's name on the partner the same way.
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

    // The partner and Darkly comment; Darkly walks back to his place and the story moves on.
    CALL(evt_msg_print_party, STRING("mac_3_018"))
    CALL(evt_msg_print, 0, STRING("mac_3_019"), 0, STRING("me"))
    CALL(evt_npc_move_position, STRING("me"), LW(13), LW(15), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING("me"), LW(12))
    CALL(evt_npc_status_onoff, 0, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    SET_VALUE(GSW_STORY_PROGRESS, STG3_52_DARKLY)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Talk event of Kuragarisan.
 * At STG3_51_FRANKLYS_HOUSE_STG_3, once Mario has reached the Twilight Town pipe,
 * he runs kuragarisan_write_name; otherwise a line per story stage (gor_02_034_00 to _10).
 */
static EVT_BEGIN(kuragarisan_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG3_51_FRANKLYS_HOUSE_STG_3)
        IF_INT_EQUAL(GSWF_TIK_08_PIPE_REJECTED, 0)
            CALL(evt_msg_print, 0, STRING("gor_02_034_03"), 0, STRING("me"))
        ELSE
            CALL_CHILD(kuragarisan_write_name)
        END_IF
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_02_034_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_02_034_01"), 0, STRING("me"))
        CASE_LESS(STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_034_02"), 0, STRING("me"))
        CASE_LESS(STG3_52_DARKLY)
        CALL(evt_msg_print, 0, STRING("gor_02_034_03"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_034_04"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_02_034_05"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_02_034_06"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_02_034_07"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_02_034_08"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_034_09"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_02_034_10"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of Luigi on the east side: from STG1_17_FATHER_SON_REUNITED to STG2_24_BOSS_BATTLE
 * he stands at (-400, 0, 0) facing right.
 */
static EVT_BEGIN(luigi_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG1_17_FATHER_SON_REUNITED, STG2_24_BOSS_BATTLE)
        CALL(evt_npc_set_position, STRING("me"), -400, 0, 0)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of Luigi: runs the shared first-talk scene luigi_gor_first_talk. */
static EVT_BEGIN(luigi_talk)
    CALL_CHILD(luigi_gor_first_talk)
    RETURN
EVT_END()

/**
 * NPCs of this room:
 * Merlon, Grifty, Professor Frankly, the bandit gang (the boss and three members), Sanders, a Goomba,
 * other villagers, Darkly and Luigi.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "パワーアップ屋", // name (Merlon)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        powerupya_init, // initEvent
        NULL, // regularEvent
#if VERSION <= VERSION_DEMO_JP
        powerupya_direct_talk,
#else
        powerupya_talk, // talkEvent
#endif
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "クリハカセ", // name (Professor Frankly)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "情報屋完璧", // name (Grifty)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "盗賊団親分", // name (Ishnail, leader of the Robbos)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "盗賊団１", // name (Gus of the Robbos)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "盗賊団２", // name (Garf of the Robbos)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "盗賊団３", // name (Goose of the Robbos)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "サンダース", // name (Admiral Bobbery)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "クリボー４", // name (Goomfrey)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "チューさん３", // name (Pierre)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "ロテン３", // name (Arfur, a Doogan)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "ボロドー１", // name (Swindell, a bandit)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "ボロドー２", // name (Larson, a bandit)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "クラガリさん", // name (Darkly)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        "ルイージ", // name (Luigi)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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

/**
 * Door animation callback of the house 01 door (the power-up shop):
 * rotates the door "S_ie_01_doa" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_01_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Second door callback of house 01: rotates the door "S_ie_01_doa" about X by 0.5 times the value in LW(0). */
static EVT_BEGIN(evt_door_01_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

/** Wall callback of house 01: rotates the wall piece "S_ie_01B" about X by LW(0). */
static EVT_BEGIN(evt_wall_01)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01B"), LW(2), 0, 0)
    RETURN
EVT_END()

/** NPCs shown inside house 01 (Merlon's shop) when its door opens. */
static const char* door_data_01_npc[] = {
    "パワーアップ屋",
    NULL,
};

static EVT_BEGIN(evt_door_01_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL_CHILD(mapball_init)
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
    SET_VALUE(GSWF_GOR_MERLON_GREETED_THIS_VISIT, 0)
    RETURN
EVT_END()

/**
 * Called when the power-up shop door closes: brings the music and ambience back
 * and moves the Star Pieces up again. The JP and later releases also turn the outside hit objects back on.
 */
static EVT_BEGIN(evt_door_01_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#endif
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
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

/**
 * Door of house 01, Merlon's power-up shop:
 * its door and wall pieces, hit areas and the scripts for entering, leaving and the open
 *  and close animation.
 */
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

/**
 * Door animation callback of the house 02 door (Professor Frankly's house):
 * rotates the door "S_i02_doa" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_02_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door-out callback for the Professor Frankly house door (door_data_02):
 * rotates "S_i02_doa" about X by half the value in LW(0) as it swings open from the inside.
 */
static EVT_BEGIN(evt_door_02_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

/** Wall callback of house 02: rotates the wall piece "S_ie_02B" about X by LW(0). */
static EVT_BEGIN(evt_wall_02)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02B"), LW(2), 0, 0)
    RETURN
EVT_END()

/** NPCs shown inside house 02 (Professor Frankly's house) when its door opens. */
static const char* door_data_02_npc[] = {
    "クリハカセ",
    NULL,
};

/**
 * Open callback of Professor Frankly's house (door_data_02, whose NPC is クリハカセ): in JP and later it turns
 * the east side's collision off. It dips and low-passes the ambience; at the epigraphy stages the music keeps playing, at
 * STG6_71_FRANKLYS_HOUSE_STG_6 it switches to BGM_EVT_FRANKURI1, otherwise it dips too (except after the final
 * battle).
 * It starts kurihakase_first_talk at STG0_08_STOLEN_COINS, places the open book by story stage, hides
 * the map "S_tizu_on" except at STG0_15_THE_PEDESTAL and hides the memo "S_i02_memo"
 * when monosiri_check gives 0.
 */
EVT_BEGIN(evt_door_02_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
    SWITCH(GSW_STORY_PROGRESS)
        CASE_OR_EQUAL(STG0_15_THE_PEDESTAL)
        CASE_OR_EQUAL(STG1_30_ZESS_T)
        CASE_OR_EQUAL(STG2_40_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG3_50_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG4_30_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG5_60_RETURN_TO_KEELHAUL)
        CASE_OR_EQUAL(STG6_70_THOUSAND_YEAR_DOOR)
        CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        CALL(evt_snd_env_lpf, 0, 800)
        CASE_END
        CASE_EQUAL(STG6_71_FRANKLYS_HOUSE_STG_6)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_FRANKURI1"))
        CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        CALL(evt_snd_env_lpf, 0, 800)
        CASE_DEFAULT
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
            CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
            CALL(evt_snd_env_lpf, 0, 800)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_08_STOLEN_COINS)
        SPAWN_EVENT(kurihakase_first_talk)
    END_IF
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 180)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG1_17_FATHER_SON_REUNITED, STG1_END_TO_STAGE_2)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
        CASE_BETWEEN(STG3_32_BOSS_BATTLE, STG3_END_TO_STAGE_4)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
        CASE_BETWEEN(STG6_54_WELCOME_BACK, STG6_END_ULTRA_HAMMER)
        CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    END_SWITCH
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG0_15_THE_PEDESTAL)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tizu_on"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i02_memo"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

/**
 * Close callback for Professor Frankly's house:
 * brings the Rogueport music and ambience back, clears the Chapter 6 research flag,
 * and starts kurihakase_wall_remove (STG0_09) or kurihakase_lecture_badge (STG0_16)
 * when those steps are pending.
 */
EVT_BEGIN(evt_door_02_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#endif
    SWITCH(GSW_STORY_PROGRESS)
        CASE_OR_EQUAL(STG6_70_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG6_71_FRANKLYS_HOUSE_STG_6)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
        CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
        CALL(evt_snd_env_lpf, 0, 0)
        CASE_END
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
        CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
        CALL(evt_snd_env_lpf, 0, 0)
    END_SWITCH
    IF_INT_EQUAL(GSWF_GOR_FRANKLY_STG6_SAID_WAIT, 1)
        SET_VALUE(GSWF_GOR_FRANKLY_STG6_SAID_WAIT, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_09_PROF_FRANKLY)
        SPAWN_EVENT(kurihakase_wall_remove)
        GOTO(100)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_16_FRANKLYS_HOUSE)
        SPAWN_EVENT(kurihakase_lecture_badge)
        GOTO(100)
    END_IF
    LABEL(100)
    WAIT_MS(500)
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUT_POS, LW(1), LW(2), LW(3))
    CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
    RETURN
EVT_END()

/**
 * Door of house 02, Professor Frankly's house:
 * its door and wall pieces, hit areas and the scripts for entering, leaving and the open
 *  and close animation.
 */
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

/**
 * Map-init set-up of Frankly's house door (LW(0) = door_data_02): hides the door's inside model group and
 * collision named in its parameters, then registers the case events that run evt_door_close (inside) and
 * evt_door_open (outside) when Mario presses A in front of the door.
 */
static EVT_BEGIN(evt_door_02_entry)
    CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 8)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(2), 0, PTR(evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    RETURN
EVT_END()

/**
 * Door animation callback of the house 03 door (Bobbery's house):
 * rotates the door "S_ie_03_doam" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_03_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doam"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door-out callback for house 03 (Bobbery's house, door_data_03):
 * rotates "S_ie_03_doam" about X by half the value in LW(0) as it swings open from the inside.
 */
static EVT_BEGIN(evt_door_03_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doam"), LW(0), 0, 0)
    RETURN
EVT_END()

/** Wall callback of house 03: rotates the wall piece "S_ie_03B" about X by 0.5 times LW(0). */
static EVT_BEGIN(evt_wall_03)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03B"), LW(2), 0, 0)
    RETURN
EVT_END()

/** NPCs shown inside house 03 (Bobbery's house) when its door opens. */
static const char* door_data_03_npc[] = {
    "サンダース",
    NULL,
};

/** Map objects shown inside house 03 when its door opens: the block pblk2. */
static const char* door_data_03_mobj[] = {
    "pblk2",
    NULL,
};

/**
 * Called when the house 03 door closes: brings the music and ambience back.
 * At STG4_34_BOBBERY_DECLINES, once (GSWF_GOR_BOBBERY_DECLINE_REMARK_DONE),
 * the partner comments on Bobbery declining (mac_4_058, mac_4_059).
 * At STG4_36_BOBBERYS_DECISION it runs the scene where Bobbery departs for the port,
 * moving the party members (mac_4_087) and advancing the story to STG4_37_TO_THE_PORT.
 * The JP and later releases also turn the outside hit objects back on.
 */
static EVT_BEGIN(evt_door_03_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#endif
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_34_BOBBERY_DECLINES)
        IF_INT_EQUAL(GSWF_GOR_BOBBERY_DECLINE_REMARK_DONE, 0)
            CALL(evt_door_end_wait)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            ADD(LW(0), 40)
            CALL(evt_party_move_pos2, 0, LW(0), LW(2), 60)
            CALL(evt_party_set_dir, 0, 270, -1)
            CALL(evt_mario_set_dir, 90, 300, 0)
#if VERSION >= VERSION_DEMO_EU
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print_party, STRING("mac_4_058"))
            CALL(evt_party_run, 0)
#else
            CALL(evt_msg_print_party, STRING("mac_4_058"))
#endif
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1_2"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_I_Z"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_DEMO_EU
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print_party, STRING("mac_4_059"))
            CALL(evt_party_run, 0)
#else
            CALL(evt_msg_print_party, STRING("mac_4_059"))
#endif
            SET_VALUE(GSWF_GOR_BOBBERY_DECLINE_REMARK_DONE, 1)
            CALL(evt_mario_key_onoff, 1)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG4_36_BOBBERYS_DECISION)
        RETURN
    END_IF
    CALL(evt_door_end_wait)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_party_stop, 1)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_party_move_pos2, 1, -36, 140, FLOAT(120.0f))
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_087"), 0, STRING("extparty"))
    CALL(evt_party_set_dir, 1, 270, -1)
#else
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
#endif
    CALL(evt_party_move_pos2, 1, -165, 140, FLOAT(120.0f))
    CALL(evt_mario_clear_party, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_37_TO_THE_PORT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Called when the house 03 door opens: dips the music and ambience to 50 percent and low-passes the ambience.
 * The JP and later releases also turn off the outside hit objects.
 */
static EVT_BEGIN(evt_door_03_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Door of house 03, Bobbery's house:
 * its door and wall pieces, hit areas and the scripts for entering, leaving and the open
 *  and close animation.
 */
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

/**
 * Door-open handler used with Sanders's door (house 03):
 * if Sanders has not opened the door yet (GSWF_GOR_BOBBERY_DOOR_UNLOCKED is 0) shows gor_02_012 and stays shut;
 * otherwise runs the normal evt_door_open.
 */
static EVT_BEGIN(evt_door_sanders_open)
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("gor_02_012"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SPAWN_EVENT(evt_door_open)
    RETURN
EVT_END()

/**
 * Door-close handler for house 03: shows gor_02_012 while the door was never opened by Sanders;
 * otherwise runs the normal evt_door_close.
 */
static EVT_BEGIN(evt_door_sanders_close)
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("gor_02_012"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SPAWN_EVENT(evt_door_close)
    RETURN
EVT_END()

/**
 * Entry for house 03's door: hides the interior and switches off its hit object named in the door parameters,
 * and registers A-button-facing case events that run evt_door_sanders_close
 * and evt_door_sanders_open in front of the door's two spots.
 */
static EVT_BEGIN(evt_door_03_entry)
    CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(2), 0, PTR(evt_door_sanders_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(3), 0, PTR(evt_door_sanders_open), 0)
    END_IF
    RETURN
EVT_END()

/**
 * Case event for the roll-in entrance of house 03 (Mario as a paper tube):
 * locks the controls, switches off the "A_i03_rol" hit object, shakes the camera,
 * plays the roll-down sounds (SFX_STG4_MARIO_ROLL_DOWN1 and 2) and runs evt_door_open_roll.
 */
static EVT_BEGIN(evt_door_03_caseopen)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("A_i03_rol"), 1, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.009765625f), 1000)
    END_INLINE_EVENT
    CALL(evt_mario_wait_landon)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_ROLL_DOWN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_ROLL_DOWN2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL_CHILD(evt_door_open_roll)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Open callback of house 03's roll-in entrance (door_data_03_roll),
 * the gap into Bobbery's (Sanders's) house that
 * Mario enters rolled up as a paper tube (evt_door_03_caseopen plays his roll-down sounds):
 * three smoke puffs rise at
 * the "S_i01_dan_mC" box, the camera moves in, the music and ambience dip and low-pass
 * and Mario rolls to (65, -55).
 * In the early demos he always unrolls facing left; in JP and later this happens only until GSWF_GOR_BOBBERY_DOOR_UNLOCKED is set.
 * While that flag is clear, Sanders turns and asks what they want (mac_4_049).
 * It then registers evt_door_03_caseopen again as a stand-on case event.
 */
EVT_BEGIN(evt_door_03_roll_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
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
    CALL(evt_cam3d_evt_set_xyz, 65, 10, -55, 0, 150, EASING_SINE_IN_OUT)
    WAIT_MS(150)
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 0)
    CALL(evt_hitobj_onoff, STRING("A_i03_rol"), 1, 1)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_mario_mov_pos2, 65, -55, FLOAT(120.0f))
#if VERSION >= VERSION_JP
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#endif
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_mario_cancel_roll_motion)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#else
    END_IF
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#endif
        CALL(evt_npc_set_ry, STRING("サンダース"), 90)
    END_IF
#if VERSION >= VERSION_JP
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#endif
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dispdir, 0, 0)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#else
    END_IF
    IF_INT_EQUAL(GSWF_GOR_BOBBERY_DOOR_UNLOCKED, 0)
#endif
        CALL(evt_msg_print, 0, STRING("mac_4_049"), 0, STRING("サンダース"))
    END_IF
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    RETURN
EVT_END()

/**
 * Close callback for the roll-in entrance of house 03:
 * dips the music and ambience and low-passes it again (no door animation).
 */
static EVT_BEGIN(evt_door_03_roll_close)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Wall callback for the roll-in entrance of house 03: tilts the wall "S_ie_03B" open by LW(2) degrees. */
static EVT_BEGIN(evt_wall_03_roll)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03B"), LW(2), 0, 0)
    RETURN
EVT_END()

/**
 * Roll-in side entrance of house 03:
 * it shares the interior with the front door but has its own wall script.
 */
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

/**
 * Entry for the roll-in entrance of house 03:
 * registers the stand-on case event (evt_door_03_caseopen) that opens it when Mario steps on the spot in front.
 */
static EVT_BEGIN(evt_door_03_roll_entry)
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    RETURN
EVT_END()

/**
 * Door animation callback of the house 04 door (the Trouble Center):
 * rotates the door "S_ie_04_doa" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Second door callback of house 04: rotates the door "S_ie_04_doa" about X by 0.5 times the value in LW(0). */
static EVT_BEGIN(evt_door_04_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_04)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i04_msk"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_B"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_ura_B"), LW(3), 0, 0)
    RETURN
EVT_END()

/**
 * Called when the house 04 door opens: dips the music and ambience to 50 percent and low-passes the ambience.
 * The JP and later releases also turn off the outside hit objects,
 * and the US and later releases switch off the hit object polySurface1317.
 */
static EVT_BEGIN(evt_door_04_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#if VERSION >= VERSION_US
    CALL(evt_hitobj_onoff, STRING("polySurface1317"), 1, 0)
#endif
#endif
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Called when the house 04 door closes: brings the music and ambience back.
 * The JP and later releases also turn the outside hit objects back on,
 * and the US and later releases switch polySurface1317 back on.
 */
static EVT_BEGIN(evt_door_04_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#if VERSION >= VERSION_US
    CALL(evt_hitobj_onoff, STRING("polySurface1317"), 1, 1)
#endif
#endif
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** NPCs shown inside house 04 (the Trouble Center) when its door opens. */
static const char* door_data_04_npc[] = {
    "盗賊団３",
    NULL,
};

/** Items shown inside house 04: the three star pieces item_star4 to item_star6. */
static const char* door_data_04_item[] = {
    "item_star4",
    "item_star5",
    "item_star6",
    NULL,
};

/**
 * Front door of house 04, the Trouble Center:
 * its door and wall pieces, hit areas and the scripts for entering, leaving and the open
 *  and close animation.
 */
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

/**
 * Message shown when the Trouble Center's door is tried while the shop is not ready (an out-of-office notice, gor_02_035):
 * locks the controls for the line, then frees them.
 */
static EVT_BEGIN(evt_door_04_junbi)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("gor_02_035"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Door-in callback for the Trouble Center's back door (door_data_04_ura):
 * rotates "S_ie_04_doa_02" about Y by approximately 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_04_00_ura)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door-out callback for the Trouble Center's back door (door_data_04_ura):
 * rotates "S_ie_04_doa_02" about X by half the value in LW(0).
 */
static EVT_BEGIN(evt_door_04_01_ura)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa_02"), LW(0), 0, 0)
    RETURN
EVT_END()

/** Wall callback for the back of the Trouble Center: tilts the wall "S_ie_04_ura_B" open by LW(3) degrees. */
static EVT_BEGIN(evt_wall_04_ura)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_ura_B"), LW(3), 0, 0)
    RETURN
EVT_END()

/** Back door of house 04, usable once Flurrie has blown the wall cover away. */
static DoorParam door_data_04_ura = {
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

/** Door callback for house 05 (door_data_05): rotates "S_ie_05_doa" about Y by approximately 0.6 times LW(0). */
static EVT_BEGIN(evt_door_05_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Wall callback for house 05: tilts "S_ie_05sB" by LW(2) degrees and "S_ie_05uB" by LW(3) degrees open. */
static EVT_BEGIN(evt_wall_05)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05sB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_05uB"), LW(3), 0, 0)
    RETURN
EVT_END()

/** NPCs shown inside house 05 (the bandit boss's house): the boss and two gang members. */
const char* door_data_05_npc[] = {
    "盗賊団親分",
    "盗賊団１",
    "盗賊団２",
    NULL,
};

/** Shorter NPC list of house 05: the boss and one gang member. */
const char* door_data_05_npc_01[] = {
    "盗賊団親分",
    "盗賊団１",
    NULL,
};

/**
 * Open callback for house 05: dips the music and ambience and low-passes the ambience.
 * JP and later also turn off the east side's collision pieces.
 */
EVT_BEGIN(evt_door_05_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Close callback for house 05: fades out music on slot 1 and restores the main music and ambience.
 * JP and later also turn the east side's collision back on.
 */
EVT_BEGIN(evt_door_05_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#endif
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of house 05, the bandit boss's house:
 * its door and wall pieces, hit areas and the entry script (there is no exit script).
 */
static DoorParam door_data_05 = {
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

/** Door callback for house 06, the thief's house (door_data_06): rotates "S_ie_06_doa" about Y by approximately 0.6 times LW(0). */
static EVT_BEGIN(evt_door_06_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_06_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Wall piece 0 of house 06, spawned by its wall callback: when the house opens (LW(1) = 0) it swings "S_i06mB"
 * 180 degrees about z over half a second with SFX_HOUSE_DOWN1, folding that wall piece away to show the inside;
 * when it closes it waits 200 ms and swings it back.
 */
static EVT_BEGIN(evt_wall_06_00)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
        ELSE
            WAIT_MS(200)
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_i06mB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, -180, 500)
    ELSE
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, -180, 0, 500)
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

/**
 * Wall piece 1 of house 06: swings "S_i06nB" down by 180 degrees (or back up) over half a second with a thump,
 * 100 ms after piece 0 when opening, but 100 ms before it when closing.
 */
static EVT_BEGIN(evt_wall_06_01)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mapobj_get_position, STRING("S_i06nB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, -180, 500)
    ELSE
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, -180, 0, 500)
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

/**
 * Wall piece 2 of house 06: swings "S_i06oB" down by 180 degrees (or back up) over half a second with a thump,
 * 100 ms after piece 1 when opening, but 100 ms before it when closing.
 */
static EVT_BEGIN(evt_wall_06_02)
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
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, -180, 500)
    ELSE
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, -180, 0, 500)
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

/** Wall callback for house 06: spawns the three wall pieces (evt_wall_06_00 to 02) together. */
static EVT_BEGIN(evt_wall_06)
    IF_INT_EQUAL(LW(6), 0)
        SPAWN_EVENT(evt_wall_06_00)
        SPAWN_EVENT(evt_wall_06_01)
        SPAWN_EVENT(evt_wall_06_02)
    END_IF
    RETURN
EVT_END()

/** NPCs shown inside house 06 (the thief's locked house): one gang member. */
static const char* door_data_06_npc[] = {
    "盗賊団２",
    NULL,
};

/** Empty NPC list of house 06, used when nobody is inside. */
static const char* door_data_06_npc_01[] = {
    NULL,
};

/**
 * Open callback for house 06: dips the music and ambience and low-passes the ambience.
 * JP and later also turn off the east side's collision pieces.
 */
EVT_BEGIN(evt_door_06_open)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_off)
#endif
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Close callback for house 06: restores the music and ambience; JP and later also turn the east side's collision back on. */
EVT_BEGIN(evt_door_06_close)
#if VERSION >= VERSION_JP
    SPAWN_EVENT(gor_02_hitobj_on)
#endif
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of house 06, the thief's locked house (Trouble Center job 1):
 * its door and wall pieces, hit areas and the entry script.
 */
static DoorParam door_data_06 = {
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

/**
 * Animation of Sanders's back door ("S_ie_03_doao") opening to 100 degrees about y with a door sound
 * and swinging back to shut with a closing thump;
 * used when Sanders walks out through it after reading Scarlette's
 * letter (mac_4_080 to 082) and as the first part of evt_gor_door_open_sub.
 */
static EVT_BEGIN(evt_gor_door_open_sanders)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 100, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 100, 0, 500)
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

/**
 * Door half of evt_gor_door_open (Mario going through Sanders's secret door):
 * plays evt_gor_door_open_sanders, waits
 * half a second, then tips "S_ie_03_doao" 90 degrees about x over half a second and plays SFX_DOOR_DOWN1.
 */
static EVT_BEGIN(evt_gor_door_open_sub)
    CALL_CHILD(evt_gor_door_open_sanders)
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 90, 500)
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

/** Stores the float args[1] into the integer variable args[0] (float to integer conversion). */
static USER_FUNC(enzan_set_i_f) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetValue(event, var, evtGetFloat(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Stores the integer args[1] into the float variable args[0] (integer to float conversion). */
static USER_FUNC(enzan_set_f_i) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetFloat(event, var, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Copies the event's 16 local words (LW) into a map-wide buffer so a nested script can reuse them. */
static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

/** Restores the 16 local words saved by enzan_lw_save. */
static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}

/**
 * Switches the map background texture to the file named by args[0] (the current map's name if NULL),
 * from "./bg/" in the early demos and "./b/" in JP and later.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(evt_set_bgtpl) {
#else
USER_FUNC(evt_set_bgtpl) {
#endif

    char path[64];
    const char* name = (const char*)evtGetValue(event, event->args[0]);

    if (name == NULL) {
        name = gp->currentMapName;
    }
#if VERSION <= VERSION_DEMO_JP
    sprintf(path, "./bg/%s.tpl", name);
#else
    sprintf(path, "./b/%s.tpl", name);
#endif
    bgReEntry(path);
    return EVT_RETURN_DONE;
}

/**
 * Turns the camera-position contribution to background scrolling on (args[0] non-zero) or off (zero);
 * the power-up ceremony turns it off while the interior is hidden.
 */
static USER_FUNC(bgScrollControll) {
    if (evtGetValue(event, event->args[0]) == 0) {
        bgTransOffsetOff();
    } else {
        bgTransOffsetOn();
    }
    return EVT_RETURN_DONE;
}

/** Blocks until the BGM stream on channel 1 has started playing, or returns immediately if that channel is empty. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Sets the event's owner NPC to the NPC named in args[0]; used so that "me" refers to Merlon in his shop scripts. */
static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/**
 * Swaps the crystal on the shop's floor: for the name "S_i01_sui_ao" shows the blue one,
 * for "S_i01_sui_aka" shows the red one (the other is hidden).
 */
static USER_FUNC(stonechangertn) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);

    if (!strcmp(name, "S_i01_sui_ao")) {
        mapGrpFlagOff("S_i01_sui_ao", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOn("S_i01_sui_aka", MAP_OBJ_FLAG_HIDDEN);
    } else if (!strcmp(name, "S_i01_sui_aka")) {
        mapGrpFlagOn("S_i01_sui_ao", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOff("S_i01_sui_aka", MAP_OBJ_FLAG_HIDDEN);
    }
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps); placeholder unk_gor_0000574c, JP gor .text+0x5680. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Returns in args[0] the direction Mario's sprite is turning toward (displayDirectionTarget: 0 for facing right,
 * other for left). JP and later; powerupya_orb_use uses it to place the offered item beside him.
 */
USER_FUNC(get_mario_dir_target) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], marioGetPtr()->displayDirectionTarget);
    return EVT_RETURN_DONE;
}
#endif

/** Forces the HP/FP/coin status window off the screen. */
static USER_FUNC(status_gauge_force_off) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

/** Sets Sanders's remembered facing (the direction he turns back to after talking) to 270 degrees. */
static USER_FUNC(set_talk_memory_ry) {
    NpcEntry* npc = npcNameToPtr("\203T\203\223\203_\201[\203X");

    npc->savedDirection = 270.0f;
    return EVT_RETURN_DONE;
}

/**
 * Trouble Center job 13: allocates a list with room for as many Battle Trunks (item 0x6B) as Mario carries,
 * plus a -1 end mark.
 */
static USER_FUNC(iri_13_make_item_tbl) {
    s32 count = pouchCheckItem(ITEM_BATTLE_TRUNKS);

    if (count > 0) {
        iri_13_item_tbl = _mapAlloc((count + 1) * sizeof(s32));
    }
    return EVT_RETURN_DONE;
}

/**
 * Trouble Center job 13: rebuilds the Battle Trunks list for the item select window
 * and returns its pointer in args[0] and the count in args[1].
 */
static USER_FUNC(iri_13_update_item_tbl) {
    EvtWord* args = event->args;
    s32 count = pouchCheckItem(ITEM_BATTLE_TRUNKS);
    s32 i;

    if (count > 0) {
        for (i = 0; i < count; i++) {
            iri_13_item_tbl[i] = ITEM_BATTLE_TRUNKS;
        }
        iri_13_item_tbl[i] = -1;
    }
    evtSetValue(event, args[0], (EvtWord)iri_13_item_tbl);
    evtSetValue(event, args[1], count);
    return EVT_RETURN_DONE;
}

/** Trouble Center job 13: frees the Battle Trunks list and clears the pointer. */
static USER_FUNC(iri_13_free_item_tbl) {
    _mapFree(iri_13_item_tbl);
    iri_13_item_tbl = NULL;
    return EVT_RETURN_DONE;
}

/**
 * Message callback for Kuragarisan's text: on callback 3, while the current page index is 1,
 * it turns him to face 90 degrees.
 */
static void kuragarisan_msgcb(s32 type, void* param) {
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

/**
 * Returns in args[0] to args[2] the in-position of Sanders's secret door ("S_ie_03_doao")
 * computed by door_position_sub.
 */
static USER_FUNC(evt_gor_door_in_info) {
    EvtWord* args = event->args;
    Vec outPos = {0.0f, 0.0f, 0.0f};
    Vec inPos = {0.0f, 0.0f, 0.0f};

    door_position_sub(3, "S_ie_03_doao", "A_ie_03_doa_02", "A_ie_03_doa_04", &outPos, &inPos, 0);
    evtSetValue(event, args[0], outPos.x);
    evtSetValue(event, args[1], outPos.y);
    evtSetValue(event, args[2], outPos.z);
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[0] to args[2] the out-position of Sanders's secret door ("S_ie_03_doao")
 * computed by door_position_sub.
 */
static USER_FUNC(evt_gor_door_out_info) {
    EvtWord* args = event->args;
    Vec outPos = {0.0f, 0.0f, 0.0f};
    Vec inPos = {0.0f, 0.0f, 0.0f};

    door_position_sub(3, "S_ie_03_doao", "A_ie_03_doa_02", "A_ie_03_doa_04", &outPos, &inPos, 0);
    evtSetValue(event, args[0], inPos.x);
    evtSetValue(event, args[1], inPos.y);
    evtSetValue(event, args[2], inPos.z);
    return EVT_RETURN_DONE;
}

/** Forces the status window open. */
static USER_FUNC(evt_StatusOpen) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

/** Closes the status window opened by evt_StatusOpen. */
static USER_FUNC(evt_StatusClose) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

/** Sets the depth offset (Z) of the background image named in args[0] to args[1]. */
static USER_FUNC(set_depthPlaneOffset) {
    EvtWord* args = event->args;
    ImageEntry* img = imgNameToPtr((const char*)evtGetValue(event, args[0]), gp->inBattle != 0);

    img->offsetZ = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Starts the lecture window and blocks until the player closes it. */
USER_FUNC(lecture) {
    if (isFirstCall) {
        winLectureOn();
    }
    if (winLectureCheck()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Finds the next enemy tattle that Mario can still learn at this story stage:
 * returns in args[0] 1 if one exists, and its enemy id in args[1].
 * Gus and Bonetail are checked after the per-stage lists; JP and later also check the Atomic Boo.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(monosiri_check) {
#else
USER_FUNC(monosiri_check) {
#endif
    EvtWord* args = event->args;
    s32 seq = evtGetValue(event, GSW_STORY_PROGRESS);
    u32 i;
    s32 j;
    MonosiriFollow* f = follow;

    for (i = 0; i < sizeof(follow) / sizeof(MonosiriFollow); i++, f++) {
        if (seq < f->seq) {
            break;
        }
        for (j = 0; j < f->count; j++) {
            if (!swGet(f->list[j] + GSWF_TATTLE_BASE)) {
                evtSetValue(event, args[0], 1);
                evtSetValue(event, args[1], f->list[j]);
                return EVT_RETURN_DONE;
            }
        }
    }
    if (swGet(GSWF_ID(GSWF_GOR_GATEKEEPER_DEFEATED)) && !swGet(UNIT_GUS + GSWF_TATTLE_BASE)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], UNIT_GUS);
        return EVT_RETURN_DONE;
    }
    if (swGet(GSWF_ID(GSWF_PIT_BONETAIL_DEFEATED)) && !swGet(UNIT_BONETAIL + GSWF_TATTLE_BASE)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], UNIT_BONETAIL);
        return EVT_RETURN_DONE;
    }
#if VERSION >= VERSION_JP
    if (swGet(GSWF_ID(GSWF_JIN_00_ATOMIC_BOO_DEFEATED)) && !swGet(UNIT_ATOMIC_BOO + GSWF_TATTLE_BASE)) {
        evtSetValue(event, args[0], 1);
        evtSetValue(event, args[1], UNIT_ATOMIC_BOO);
        return EVT_RETURN_DONE;
    }
#endif
    evtSetValue(event, args[0], 0);
    evtSetValue(event, args[1], 0);
    return EVT_RETURN_DONE;
}

/** Marks tattle entry args[0] as learned and returns in args[1] the pointer to its message. */
static USER_FUNC(monosiri_on) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    TattleEntry* tattle = battleGetUnitMonosiriPtr(id);

    swSet(id + GSWF_TATTLE_BASE);
    evtSetValue(event, args[1], (EvtWord)msgSearch(tattle->enemyTag));
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[0] whether the 3D camera is facing the front of the house (yaw below 90 or at least 270 degrees),
 * 1 for yes. US and later return 0 when Mario is west of x=-300.
 */
static USER_FUNC(camera_front_check) {
    EvtWord* args = event->args;
    f32 angle = reviseAngle((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);

#if VERSION >= VERSION_US
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

/** Allocates the 256-byte buffer that remembers the previous camera road name. */
static USER_FUNC(get_camcurve_init) {
    camstring = _mapAlloc(0x100);
    camstring[0] = '\0';
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[0] 1 when the tracked camera road changes from the remembered upper road "C_ue" to the ground road
 * "C_ground_ok", else 0; remembers the road name for the next call.
 */
static USER_FUNC(get_camcurve) {
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

/**
 * Blocks (returns not-done) for every frame the camera yaw has changed since the last call;
 * returns done when the camera has held still.
 */
static USER_FUNC(camera_move_check) {
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

/**
 * Spawned by evt_gor_door_open as Mario goes through Sanders's secret door: installs the two door TEV colour
 * callbacks (which multiply colour and alpha by the door tint) on the room pieces, fades them and Sanders's NPC
 * colour out over 0.3 seconds, hides them,
 * folds the divider "S_i03_sikiri_mC" away by 90 degrees and fades it out too.
 */
static EVT_BEGIN(evt_gor_door_dark_sub)
    CALL(evt_map_set_tevcallback, 1, PTR(_door_color_tev))
    CALL(evt_map_set_tevcallback, 2, PTR(_door_color2_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_del_mC"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_kmn_mC"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_yuka_mD"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_doam"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_sikiri_mC"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_del_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_kmn_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_yuka_mD"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_doam"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_sikiri_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("サンダース"), 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_del_mC"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_kmn_mC"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_yuka_mD"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_doam"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, -90, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i03_sikiri_mC"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color2_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_sikiri_mC"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Open handler for Sanders's secret door: locks Mario, spawns evt_gor_door_open_sub and evt_gor_door_dark_sub,
 * walks Mario and his partners from the in-position to the out-position of the door
 * and moves the 3D camera along with him, then frees the controls and letterbox.
 */
static EVT_BEGIN(evt_gor_door_open)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SPAWN_EVENT(evt_gor_door_open_sub)
    SPAWN_EVENT(evt_gor_door_dark_sub)
    CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(135.0f))
    CALL(evt_party_move_beside_mario, 0)
    CALL(evt_party_move_beside_mario, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_gor_door_out_info, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        ADD(LW(4), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(4), LW(2), 0, 150, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_gor_door_out_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Animation of Sanders's door ("S_ie_03_doao") opening to 100 degrees with a door sound
 * and swinging back to shut with a closing thump.
 */
static EVT_BEGIN(evt_gor_door_close_sanders)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_doao"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 100, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 100, 0, 500)
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

/**
 * Door half of evt_gor_door_close (Mario coming back out through Sanders's secret door):
 * tips "S_ie_03_doao" back
 * from 90 degrees to 0 about x over half a second, then plays the swing of Sanders's door
 * (evt_gor_door_close_sanders).
 */
static EVT_BEGIN(evt_gor_door_close_sub)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 90, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doao"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL_CHILD(evt_gor_door_close_sanders)
    RETURN
EVT_END()

/**
 * Undoes evt_gor_door_dark_sub when Mario walks back out through Sanders's door (spawned by evt_gor_door_close):
 * unfolds the divider "S_i03_sikiri_mC" and fades the room pieces
 * and Sanders's NPC colour back up to full over 0.3 seconds,
 * then removes the TEV callbacks and puts the pieces back in the display list.
 */
static EVT_BEGIN(evt_gor_door_light_sub)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(_door_color2_main_func, LW(0), LW(0), LW(0), 255)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_sikiri_mC"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -90, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_i03_sikiri_mC"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_del_mC"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_kmn_mC"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_yuka_mD"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_doam"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("サンダース"), 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(_door_color_main_func, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_del_mC"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_kmn_mC"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_yuka_mD"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_doam"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_sikiri_mC"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_del_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_kmn_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_yuka_mD"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_doam"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_sikiri_mC"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()

/**
 * Close handler for Sanders's secret door, when Mario presses A at its inner side (A_ie_03_doa_04):
 * locks the controls, spawns evt_gor_door_close_sub and evt_gor_door_light_sub,
 * walks Mario and his partners to the in-position of the door while the 3D camera follows,
 * then frees the controls.
 */
static EVT_BEGIN(evt_gor_door_close)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SPAWN_EVENT(evt_gor_door_close_sub)
    SPAWN_EVENT(evt_gor_door_light_sub)
    BEGIN_INLINE_EVENT
        CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        ADD(LW(4), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(0), LW(4), LW(2), 0, 150, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_gor_door_in_info, LW(0), LW(1), LW(2))
    CALL(evt_mario_party_door_move, LW(0), LW(2), FLOAT(135.0f))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Sets up Sanders's secret door "A_ie_03_doa_02" and "A_ie_03_doa_04":
 * initialises the door tint state (_door_color_init_func) and registers evt_gor_door_open
 * and evt_gor_door_close as the front-facing case events of the two doors.
 */
static EVT_BEGIN(evt_gor_offscren_door_entry)
    CALL(_door_color_init_func)
    SET_VALUE(LW(4), STRING("A_ie_03_doa_02"))
    SET_VALUE(LW(6), STRING("A_ie_03_doa_04"))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie_03_doa_02"), 0, PTR(evt_gor_door_open), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie_03_doa_04"), 0, PTR(evt_gor_door_close), 0)
    RETURN
EVT_END()

/**
 * Mario is spun around when the thief bumps into him in the Chapter 0 coin-stealing scene:
 * he plays pose M_N_H with SFX_STG0_MARIO_ROUND1 and a surprised voice,
 * turns five full circles (1800 degrees of display direction) in one second, then returns to his idle pose.
 */
static EVT_BEGIN(mario_kirimomi_event)
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    CALL(evt_snd_sfxon, STRING("SFX_STG0_MARIO_ROUND1"), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_get_dispdir, LW(8))
    SET_VALUE(LW(9), 1800)
    ADD(LW(9), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(8), LW(9), 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mario_set_dispdir, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    RETURN
EVT_END()

/** Shakes the camera briefly (0.1 second) for the bump in steel_coin. */
static EVT_BEGIN(cam_yurasu_event)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.01953125f), FLOAT(0.0f), 100)
    RETURN
EVT_END()

static EVT_BEGIN(steel_coin)
    // Setup: the thief is placed to their right and Mario and the partner come in from the left edge.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kmn"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_npc_set_ry, STRING("盗賊"), 270)
    CALL(evt_npc_set_position, STRING("盗賊"), -300, 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
        // The thief runs in from the right at full speed, with a running sound that follows him.
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
            // He barrels into Mario with a crash and a stealing sound; the camera shakes and Mario spins.
            CALL(evt_npc_get_position, STRING("盗賊"), LW(0), LW(1), LW(2))
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            SUBTRACT(LW(0), 20)
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_BORODO_STEAL1"), LW(0), LW(1), LW(2), 0)
        SPAWN_EVENT(cam_yurasu_event)
        SPAWN_EVENT(mario_kirimomi_event)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -600, 30, 0)

        // The thief calls out a quick "Oops! Pardon me" and runs off.
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

    // The partner shouts after the thief and wakes Mario up: half of his coins are gone.
    CALL(evt_cam3d_evt_set, -450, 103, 330, -525, 43, -6, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_064"))

    // Mario jumps in surprise while the status window shows the coins dropping; half of them are taken
    // and the amount is remembered so the thief can give them back later.
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, -1000)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_StatusOpen)
    WAIT_MS(400)
    CALL(evt_pouch_get_coin, LW(0))
    SET_VALUE(LW(1), LW(0))
    DIVIDE(LW(1), 2)
    SUBTRACT(LW(0), LW(1))
    CALL(evt_pouch_set_coin, LW(0))
    SET_VALUE(LW(2), LW(1))
    AND_RAW(LW(2), 255)
    SET_VALUE(GSW_GOR_STOLEN_COINS_LOW, LW(2))
    SET_VALUE(LW(2), LW(1))
    AND_RAW(LW(2), 65280)
    DIVIDE(LW(2), 256)
    SET_VALUE(GSW_GOR_STOLEN_COINS_HIGH, LW(2))
    DEBUG_PUT_MSG(STRING("盗まれた金額の下位と上位"))
    DEBUG_PUT_REG(GSW_GOR_STOLEN_COINS_LOW)
    DEBUG_PUT_REG(GSW_GOR_STOLEN_COINS_HIGH)
    WAIT_MS(2000)
    CALL(evt_StatusClose)

    // The partner fumes about the town and the scene ends.
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_065"))
    LABEL(100)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_08_STOLEN_COINS)
    RETURN
EVT_END()

/**
 * Cutscene when Mario and Goombella first enter Professor Frankly's house (STG0_08):
 * Frankly appears, introduces himself, looks at the Magical Map,
 * and sends them off to find the Thousand-Year Door pedestal. Ends by setting STG0_09_PROF_FRANKLY.
 * Frankly's talk uses the mac_0_066 to mac_0_082 lines.
 */
static EVT_BEGIN(kurihakase_first_talk)
    // Setup: Frankly is placed in his house and the door finishes closing behind Mario and the partner.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_02_naka"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -115, 0, -225)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_door_end_wait)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)

    // The partner calls out for the Professor; the camera pushes in and the Professor music starts.
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_move_pos2, 0, -190, -100, FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 90, 200)
    CALL(evt_msg_print_party, STRING("mac_0_066"))
    CALL(evt_cam3d_evt_set, -115, 65, -20, -115, 30, -217, 666, EASING_SINE_IN_OUT)
    WAIT_MS(666)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_FRANKURI1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)

    // Frankly wakes up, mumbling, and the partner and Mario walk over to his desk.
    CALL(evt_msg_print, 0, STRING("mac_0_066_01"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -190, -215, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("クリハカセ"), -140, -225, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -225, -210, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)

    // Frankly tries to recall where he knows the partner from, guessing wrong names
    // and fidgeting with a growing camera zoom.
    CALL(evt_msg_print, 0, STRING("mac_0_067"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_01"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_02"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -170, 84, 29, -170, 40, -217, 200, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_03"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -155, 72, -20, -155, 37, -217, 200, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_067_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 59, -69, -140, 33, -217, 200, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(700)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)

    // He remembers: she is Goombella, from his archaeology class.
    CALL(evt_msg_print, 0, STRING("mac_0_067_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_067_06"), 0, STRING("クリハカセ"))

    // Goombella cheers for U Goom; Frankly says he remembers her, but who is that with her?
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

    // Frankly shouts "WHO?" at Mario, who tumbles over; Goombella introduces him as the famous Mario.
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 500)
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

    // Frankly apologizes for being out of touch and asks what they want;
    // Goombella says they are after the legendary treasure.
    CALL(evt_msg_print, 0, STRING("mac_0_069_03"), 0, STRING("クリハカセ"))
    CALL(evt_party_move_pos2, 0, -185, -215, FLOAT(40.0f))
    CALL(evt_cam3d_evt_set, -185, 64, -21, -185, 30, -218, 300, EASING_SINE_IN_OUT)
    CALL(evt_msg_print_party, STRING("mac_0_070"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // Frankly knows the legend: the Thousand-Year Door lies below this town
    // and needs the seven Crystal Stars.
    CALL(evt_msg_print, 0, STRING("mac_0_071"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -250, 95, 122, -185, 45, -217, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_072"))
    CALL(evt_msg_print, 0, STRING("mac_0_073"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_073_01"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -65, 95, 107, -185, 45, -217, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_074"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_075"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 64, -21, -185, 30, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_076"))
    CALL(evt_cam3d_evt_set, -140, 64, -21, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // He points out the missing piece, the Magical Map, and Goombella says Mario has it.
    CALL(evt_msg_print, 0, STRING("mac_0_077"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 95, 82, -185, 42, -217, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_078"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)

    // Frankly is stunned and hops with shock.
    CALL(evt_msg_print, 0, STRING("mac_0_079"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -170, -225, FLOAT(80.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
        CALL(evt_party_move_pos2, 0, -190, -205, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -160, 70, -19, -160, 40, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // Mario hands over the Magical Map, which Frankly raises before him in awe.
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_item_entry, STRING("item00"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
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

    // Frankly names Mario his hero and says they must set off at once to look for the pedestal.
    CALL(evt_cam3d_evt_set, -155, 78, 30, -155, 40, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_081"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_0_081_01"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("mac_0_082"), 0, STRING("クリハカセ"))
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))

    // Frankly's NPC is hidden and his follower is set up; the partners run again and the music and ambience resume.
    CALL(evt_npc_set_position, STRING("クリハカセ"), 0, -1000, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_party_pos, 1, PARTY_FRANKLY, LW(0), LW(1), LW(2))
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 1)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_09_PROF_FRANKLY)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Cutscene when Mario comes out of Frankly's house at STG0_09_PROF_FRANKLY (spawned by evt_door_02_close):
 * Frankly shows that the fence "S_saku04" comes off (mac_0_083) and it peels away as a paper animation
 * (OFF_d_saku, fence sounds), opening the pipe down under the city (mac_0_084). Then it sets
 * STG0_10_REMOVING_THE_FENCE and spawns kurihakase_lecture.
 */
static EVT_BEGIN(kurihakase_wall_remove)
    CALL(evt_door_end_wait)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_cam3d_evt_set, -270, 100, 595, -270, 30, 201, 2000, EASING_SINE_IN_OUT)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_mario_clear_party, 1)
    CALL(evt_npc_set_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_saku04"), MAP_OBJ_FLAG_HIDDEN)
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
    CALL(evt_cam3d_evt_set, -210, 85, 595, -210, 15, 201, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -220, 305, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("クリハカセ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("mac_0_084"), 0, STRING("クリハカセ"))
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_10_REMOVING_THE_FENCE)
    SPAWN_EVENT(kurihakase_lecture)
    RETURN
EVT_END()

/**
 * Frankly's tutorial lecture after the fence is gone:
 * asks whether Mario knows Action Commands (mac_0_084_02), lets Goombella chip in,
 * and sets STG0_10_01_FRANKLYS_TUTORIAL. If he says he knows them and declines practice, he skips the lesson.
 */
static EVT_BEGIN(kurihakase_lecture)
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
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(800)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_084_01"), 0, STRING("クリハカセ"))
    WAIT_WHILE(GF(0))
    CALL(evt_cam3d_evt_set, -245, 100, 639, -245, 40, 294, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_DRAW | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), NPC_FLAG_NO_DRAW)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_npc_jump_position_nohit, STRING("クリスチーヌ"), LW(0), LW(1), LW(2), 200, 20)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("mac_0_084_06"), 0, STRING("クリスチーヌ"))
    CALL(evt_npc_set_battle_info, STRING("クリハカセ"), 3)
    CALL(evt_npc_battle_start, STRING("クリハカセ"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_npc_flag_onoff, 0, STRING("クリスチーヌ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_set_prev_party_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 0, -1000, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_084_07"), 0, STRING("クリハカセ"))
    LABEL(100)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_10_01_FRANKLYS_TUTORIAL)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Opens Frankly's front door ("S_i02_doa") to 104 degrees with the door sound, waits half a second and shuts it
 * again; kurihakase_lecture_badge plays it as Frankly comes out with the Power Smash badge
 * and as he goes back in.
 */
static EVT_BEGIN(kurihakase_door)
    CALL(evt_mapobj_get_position, STRING("S_i02_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 104, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_i02_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 104, 0, 500)
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

/**
 * Badge tutorial at STG0_16_FRANKLYS_HOUSE:
 * Frankly gives Mario a Power Smash badge, asks whether he wants a lesson on badges (mac_0_121_02 to 05)
 * and runs the lecture window (lecture) if he does, then sets STG0_17_BADGE_TUTORIAL.
 */
static EVT_BEGIN(kurihakase_lecture_badge)
    // Mario and the partner wait after leaving the house; Mario looks around.
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

    // A message asks Mario to wait; the Professor is placed at the front door.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_121_01"), 0, 0)
    WAIT_WHILE(GF(0))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -290, -15, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -265, 0, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_02_naka"), MAP_OBJ_FLAG_HIDDEN)

    // Frankly steps out of the house; the camera moves in and he hands Mario a Power Smash badge.
    CALL(evt_npc_set_position, STRING("クリハカセ"), -215, 0, -93)
    CALL(evt_item_entry, STRING("item00"), ITEM_POWER_SMASH, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
            ADD(LW(1), 20)
            CALL(evt_item_set_position, STRING("item00"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    SPAWN_EVENT(kurihakase_door)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -235, 0, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
#if VERSION >= VERSION_US
    CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("A_2"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("S_1"))
#endif
    DELETE_EVENT(LW(10))

    // Mario receives the Power Smash badge.
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)

    // Frankly asks whether Mario would like a lesson on badges; Mario can accept or decline.
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

    // He explains how the lesson works and waits for the Start button.
    LABEL(10)
    CALL(evt_msg_print_add, 0, STRING("mac_0_121_04"))
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), PAD_BUTTON_START)
            CALL(evt_msg_continue)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP

    // The badge lesson window opens; afterwards Frankly closes with a short remark.
    CALL(lecture)
    CALL(evt_msg_print, 0, STRING("mac_0_121_05"), 0, STRING("クリハカセ"))

    // Frankly walks back to his usual spot in the house and control returns to the player.
    LABEL(20)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, 0, 0, FLOAT(120.0f), 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL_CHILD(kurihakase_door)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_02_naka"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -215, -93, 0, FLOAT(120.0f), 0)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_17_BADGE_TUTORIAL)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(Mario_Party_position)
    CALL(evt_mario_set_pos, -190, 0, -215)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -225, 0, -215)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_set_breed_pose, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_party_set_breed_pose, 0, 1)
    RETURN
EVT_END()

/** Opens the book on Frankly's table ("S_open_book_r" turns 180 degrees) with its sound; used by the epigraphy scenes. */
static EVT_BEGIN(book_open)
    CALL(evt_mapobj_get_position, STRING("S_open_book_r"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_OPEN_BOOK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 360, 180, 500)
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

/** Closes the book on Frankly's table ("S_open_book_r" turns back) with its sound; used by the epigraphy scenes. */
static EVT_BEGIN(book_close)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 360, 500)
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

/**
 * Hides the open book "S_open_book", moves it to (-15, 0, 0) and resets the page "S_open_book_r";
 * the after-stage-3
 * and after-stage-6 epigraphy scenes run it before book_open.
 */
static EVT_BEGIN(book_center_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_open_book"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 0)
    RETURN
EVT_END()

/**
 * Frankly's first epigraphy scene, after the Thousand-Year Door (STG0_15_THE_PEDESTAL):
 * the pouch's Star Power
 * is filled, Frankly explains the light that shone on Mario (a special move) and offers to show it outside
 * (mac_0_103_01 to 06), then reads the Magical Map: the next Crystal Star is in Petal Meadows, reached by a pipe
 * under the city; he learns that Princess Peach sent the map and had visited him, and stays in town while Mario
 * and the partner head out (mac_0_104 to 120).
 * It ends with the Rogueport theme and STG0_16_FRANKLYS_HOUSE.
 */
EVT_BEGIN(epigraphy_map)
    // Mario's Star Power gauge is filled and control is taken away while Mario
    // and the partner are moved into place.
    CALL(evt_pouch_get_max_ap, LW(0))
    CALL(evt_pouch_set_ap, LW(0))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_clear_party, 1)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)

    // A hidden Peach stand-in is loaded; a short narration plays over a camera view of the house.
    CALL(evt_npc_entry, STRING("ピーチダミー"), STRING("c_peach")) // hidden Peach stand-in in the house scene
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_102"), 0, 0)
    WAIT_MS(2000)

    // The screen irises out, then back in on the inside of Frankly's house.
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(evt_door_open_quick)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)

    // The partner and Frankly talk about the light that shone on Mario;
    // Frankly offers to show the special move outside.
    CALL(evt_msg_print_party, STRING("mac_0_103"))
    CALL(evt_msg_print, 0, STRING("mac_0_103_01"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_103_02"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_103_03"), 0, STRING("クリハカセ"))

    // Mario can accept or decline; the choice determines whether the special-move demonstration battle runs.
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
        // The group walks outside and Frankly starts the special move demonstration battle.
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

    // Back in the house after the demonstration, Frankly reads the Magical Map.
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL_CHILD(Mario_Party_position)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_104"), 0, STRING("クリハカセ"))
    LABEL(20)

    // Frankly explains where the next Crystal Star lies and that a pipe under the city leads there.
    CALL(evt_msg_print_party, STRING("mac_0_105"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_0_106"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -250, 133, 93, -180, 43, -210, 0, EASING_SINE_IN_OUT)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_107"))
    CALL(evt_msg_print, 0, STRING("mac_0_108"), 0, STRING("クリハカセ"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(200)
    CALL_CHILD(book_close)
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
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)

    // He learns that Princess Peach sent the map and had visited him.
    CALL(evt_msg_print, 0, STRING("mac_0_111"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_112"))
    CALL(evt_msg_print, 0, STRING("mac_0_113"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_0_114"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -150, 100, 82, -150, 47, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)

    // He walks to the table, picks up the map and then hands it back to Mario.
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -150, -205, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tizu_on"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_item_entry, STRING("item_map"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item_map"), ITEM_FLAG_NO_TOUCH_GET)
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
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
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
    CALL(evt_cam3d_evt_set, -180, 100, 82, -180, 47, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(200)

    // The partner and Frankly wish them luck; Frankly stays behind in town.
    CALL(evt_msg_print_party, STRING("mac_0_117"))
    CALL(evt_msg_print, 0, STRING("mac_0_118"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_0_119"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_120"))

    // The camera returns to normal, the party follows Mario and the Rogueport theme resumes.
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_DEMO_EU
    SET_VALUE(GSWF_GOR_FRANKLY_FIRST_MAP_READ, 1)
#endif
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
#if VERSION < VERSION_DEMO_EU
    SET_VALUE(GSWF_GOR_FRANKLY_FIRST_MAP_READ, 1)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG0_16_FRANKLYS_HOUSE)
    RETURN
EVT_END()

/**
 * Frankly's cutscene after Chapter 1 (STG1_30_ZESS_T):
 * he deciphers the Magical Map for the next Crystal Star (mac_1_004 to 010_03)
 * and sets STG1_31_FRANKLYS_HOUSE_STG_1.
 */
static EVT_BEGIN(epigraphy_map_after_stage1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_1_004"), 0, 0)
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(evt_door_open_quick)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_trans, 1, STRING("S_open_book"), -15, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_open_book_r"), 0, 0, 180)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(book_close)
    CALL(evt_msg_print, 0, STRING("mac_1_005"), 0, STRING("クリハカセ"))
    WAIT_MS(333)
    CALL(evt_msg_print_party, STRING("mac_1_006"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_1_007"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_1_008"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_1_009"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_1_010_03"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG1_31_FRANKLYS_HOUSE_STG_1)
    RETURN
EVT_END()

/**
 * Frankly's cutscene after Chapter 2 (STG2_40_THOUSAND_YEAR_DOOR):
 * he deciphers the map for the next star (mac_2_002_01 to mac_2_011) and sets STG2_41_FRANKLYS_HOUSE_STG_2.
 */
static EVT_BEGIN(epigraphy_map_after_stage2)
    // Control is taken away; Mario and the partner are moved into place and the camera frames the town.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)

    // A caption narrates the visit; the screen irises out and back in on the inside of the house.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_2_002_01"), 0, 0)
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(evt_door_open_quick)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(book_close)

    // Frankly and the partner study the map; the camera moves in as he works out the next Crystal Star.
    CALL(evt_msg_print, 0, STRING("mac_2_002_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_002_03"))
    CALL(evt_cam3d_evt_set, -140, 70, 4, -140, 32, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_002_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 63, -40, -208, 32, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_2_002_05"))
    CALL(evt_cam3d_evt_set, -140, 63, -40, -140, 32, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_002_06"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        // Surprise: a startled sting plays.
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED2"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)

    // Mario and the partner fall over with a crash and the camera shakes, then they get back up.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_D_8"))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_party_set_breed_pose, 0, 9)
#else
    CALL(evt_party_set_breed_pose, 0, 8)
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    CALL(evt_party_set_breed_pose, 0, 1)
    WAIT_MS(500)

    // The partner and Frankly work out where to go next.
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("mac_2_002_07"))
    CALL(evt_cam3d_evt_set, -140, 63, -40, -140, 32, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_002"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_003"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_004"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_2_005"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_006"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_2_007"))
    CALL(evt_msg_print, 0, STRING("mac_2_008"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_009"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 66, -20, -208, 33, -218, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("mac_2_010"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("mac_2_011"), 0, STRING("クリハカセ"))

    // The camera returns to normal and control goes back to the player.
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)

    // The Rogueport theme and ambience resume.
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG2_41_FRANKLYS_HOUSE_STG_2)
    RETURN
EVT_END()

/**
 * Frankly's cutscene after Chapter 3 (STG3_50_THOUSAND_YEAR_DOOR):
 * he deciphers the map for the next star (mac_3_001_01 on);
 * once the Twilight Town pipe has rejected Mario, he discusses that rejection further. Sets STG3_51_FRANKLYS_HOUSE_STG_3.
 */
static EVT_BEGIN(epigraphy_map_after_stage3)
    // Control is taken away; Mario and the partner are moved into place and the camera frames the town.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)

    // A caption narrates the visit; the screen irises out and back in on the inside of the house.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_3_001_01"), 0, 0)
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(evt_door_open_quick)
    WAIT_FRAMES(1)
    CALL_CHILD(book_center_init)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)

    // Frankly and the partner study the map and he starts to decipher it.
    CALL(evt_msg_print, 0, STRING("mac_3_001_02"), 0, STRING("クリハカセ"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_03"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("mac_3_001_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_3_001_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_06"))
    CALL(evt_msg_print, 0, STRING("mac_3_001_07"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 70, 4, -208, 32, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_001_07"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(750)
    CALL(evt_msg_print, 0, STRING("mac_3_002"), 0, STRING("クリハカセ"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_3_003"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(750)

    // Frankly goes to fetch a book from his shelf and puts it on the table.
    CALL(evt_msg_print, 0, STRING("mac_3_004"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 500, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("クリハカセ"), -145, -285, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("クリハカセ"), -135, -225, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_open_book"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_PUT_BOOK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_open_book"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL_CHILD(book_open)
    WAIT_MS(500)

    // He gets an idea and explains where the next Crystal Star lies;
    // if the Twilight Town pipe has already rejected Mario, he discusses it further.
    CALL(evt_npc_move_position, STRING("クリハカセ"), -145, -225, 0, FLOAT(60.0f), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_3_005"), 0, STRING("クリハカセ"))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(GSWF_TIK_08_PIPE_REJECTED, 0)
        CALL(evt_msg_print_party, STRING("mac_3_006"))
    ELSE
        CALL(evt_msg_print_party, STRING("mac_3_007"))
        CALL(evt_msg_print, 0, STRING("mac_3_009"), 0, STRING("クリハカセ"))

        // The partner comments; Frankly stares in surprise before answering.
        WAIT_MS(1500)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クリハカセ"), 0, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_3_010"), 0, STRING("クリハカセ"))
    END_IF

    // The camera returns to normal and control goes back to the player.
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)

    // The Rogueport theme and ambience resume.
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG3_51_FRANKLYS_HOUSE_STG_3)
    RETURN
EVT_END()

/**
 * Frankly's epigraphy scene after Chapter 4 (STG4_30_THOUSAND_YEAR_DOOR):
 * hearing of Princess Peach's e-mail
 * he explains that the treasure is the spirit of an ancient demon beaten by four heroes
 * and that the Thousand-Year
 * Door may be weakening, so the stars must still be gathered (mac_4_001_01 to 12);
 * the map points to Keelhaul Key
 * (mac_4_002, 003). It sets STG4_31_FRANKLYS_HOUSE_STG_4.
 */
static EVT_BEGIN(epigraphy_map_after_stage4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_4_001_01"), 0, 0)
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(evt_door_open_quick)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_4_001_02"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_4_001_03"))
    CALL(evt_msg_print, 0, STRING("mac_4_001_04"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_05"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_06"))
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_07"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_08"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -260, 125, 256, -180, 75, -54, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_09"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_10"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_4_001_11"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_4_001_12"))
    CALL(evt_msg_print, 0, STRING("mac_4_002"), 0, STRING("クリハカセ"))
    CALL(evt_msg_print_party, STRING("mac_4_002"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_4_003"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG4_31_FRANKLYS_HOUSE_STG_4)
    RETURN
EVT_END()

/**
 * Frankly's epigraphy scene after Chapter 5 (STG5_60_RETURN_TO_KEELHAUL):
 * the next Crystal Star is in Poshley
 * Heights, reached on the Excess Express, and he suggests asking Don Pianta for tickets (mac_5_002 to 009).
 * It sets
 * STG5_61_FRANKLYS_HOUSE_STG_5.
 */
static EVT_BEGIN(epigraphy_map_after_stage5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_5_002"), 0, STRING("クリハカセ"))
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    SPAWN_EVENT(evt_door_open_quick)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL_CHILD(book_close)
    CALL(evt_msg_print_party, STRING("mac_5_002"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_003"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_004"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_005"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_006"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG5_61_FRANKLYS_HOUSE_STG_5)
    RETURN
EVT_END()

static EVT_BEGIN(epigraphy_map_after_stage6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(Mario_Party_position)
    CALL(evt_cam3d_evt_set, -170, 75, 717, -170, 75, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_6_002_00"), 0, STRING("クリハカセ"))
    CALL(evt_fade_entry, FADE_OUT_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(evt_door_open_quick)
    WAIT_FRAMES(1)
    CALL_CHILD(book_center_init)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_fade_entry, FADE_IN_IRIS, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_6_002"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -140, 60, -40, -140, 30, -218, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_003"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -208, 77, 28, -208, 35, -218, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_6_004"))
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_open_book"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL_CHILD(book_open)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_005_01"), 0, STRING("クリハカセ"))
    CALL(evt_sub_save_playtime, GSW_GOR_FRANKLY_STG6_START_TIME)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF_GOR_FRANKLY_STG6_SAID_WAIT, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_71_FRANKLYS_HOUSE_STG_6)
#if VERSION >= VERSION_JP
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
#else
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
#endif
    CALL(evt_snd_env_lpf, 0, 800)
#endif
    RETURN
EVT_END()

/**
 * Cutscene when Frankly finishes the Chapter 6 research after three minutes of play time:
 * Frankly and the partner discuss it (mac_6_007 to 015),
 * with a different line depending on whether Mario has the Ultra Hammer,
 * then sets STG6_72_3_MINUTES_LATER and restores the controls and music.
 */
static EVT_BEGIN(kurihakase_after3minutes)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("mac_6_007"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("mac_6_008"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_009"), 0, STRING("クリハカセ"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_set, LW(0), 95, 73, LW(0), 43, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_6_010"))
    CALL(evt_cam3d_evt_set, -157, 95, 73, -147, 43, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_011"), 0, STRING("クリハカセ"))
    CALL(evt_cam3d_evt_set, -197, 121, 185, -197, 50, -217, 0, EASING_SINE_IN_OUT)
    CALL(evt_pouch_check_item, ITEM_ULTRA_HAMMER, LW(0))
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
    CALL(evt_cam3d_evt_set, -157, 95, 73, -147, 43, -217, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_6_015"), 0, STRING("クリハカセ"))
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_72_3_MINUTES_LATER)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION >= VERSION_JP
    CALL(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry, -190, 30, 325)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
#if VERSION >= VERSION_US
    CALL(evt_snd_envoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(4))
#else
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
#endif
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Spots Mario is put back on when he falls or is hurt on a hazard:
 * each collision area names the position he returns to (a position far below the map marks the ship change zones).
 */
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

/** Same as gor_goki1_urouro for the second cockroach object, S_goki_02 (a different base offset). */
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

/**
 * Opens the chest mobj_box01 and gives Mario the Double Dip badge (ITEM_DOUBLE_DIP;
 * the JP and later releases add ITEM_FLAG_LOST_BADGE_TO_SHOP).
 */
static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), ITEM_DOUBLE_DIP, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_item_flag_on, STRING("item01"), ITEM_FLAG_NO_TOUCH_GET)
#else
    CALL(evt_item_flag_on, STRING("item01"), 258)
#endif
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Prologue enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg0_list[] = {
    8,
};

/** Chapter 1 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg1_list[] = {
    18, 20, 22, 23,
};

/** Chapter 2 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg2_list[] = {
#if VERSION <= VERSION_DEMO_JP
    29, 30, 34,
#else
    29, 30, 33, 34,
#endif
};

/** Chapter 3 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg3_list[] = {
    65,
};

/** Demo Chapter 4 or (JP and later) Chapter 5 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
#if VERSION <= VERSION_DEMO_JP
static s32 stg4_list[] = {
    76,
#else
static s32 stg5_list[] = {
    93, 99,
#endif
};

#if VERSION <= VERSION_DEMO_JP
static s32 stg5_list[] = {
    93, 94, 99,
};
#endif

/** Chapter 6 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg6_list[] = {
    107,
};

/** Chapter 7 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg7_list[] = {
#if VERSION <= VERSION_DEMO_JP
    114, 115, 116, 117, 118, 28, 120, 119, 121,
#else
    117, 118, 28, 120, 119, 121,
#endif
};

/** Chapter 8 enemies whose Tattle entries Frankly's notes can fill in (enemy ids). */
static s32 stg8_list[] = {
#if VERSION <= VERSION_DEMO_JP
    131, 132, 133, 134, 135, 144, 146, 147,
    150,
#else
    131, 132, 133, 134, 135, 144, 145, 146,
    147, 150,
#endif
};

/**
 * Frankly's notes table for the wastebasket:
 * from each story step on, the chapter list of enemy Tattles Mario can still learn, with the list's length.
 */
MonosiriFollow follow[] = {
    {23, stg0_list, 1},
    {56, stg1_list, 4},
#if VERSION <= VERSION_DEMO_JP
    {112, stg2_list, 3},
#else
    {112, stg2_list, 4},
#endif
    {164, stg3_list, 1},
#if VERSION <= VERSION_DEMO_JP
    {215, stg4_list, 1},
    {261, stg5_list, 3},
#else
    {261, stg5_list, 2},
#endif
    {339, stg6_list, 1},
#if VERSION <= VERSION_DEMO_JP
    {373, stg7_list, 9},
    {401, stg8_list, 9},
#else
    {373, stg7_list, 6},
    {401, stg8_list, 10},
#endif
};

/**
 * Case event for the wastebasket in Frankly's house ("A_i02_gomibako"):
 * shows the next tattle Mario can still learn (monosiri_follow_00), marks it as learned,
 * and removes the note "S_i02_memo" when none are left.
 */
static EVT_BEGIN(evt_monosiri)
    CALL(evt_mario_key_onoff, 0)
    CALL(monosiri_check, LW(0), LW(1))
    CALL(monosiri_on, LW(1), LW(2))
    CALL(evt_msg_print_insert, 0, STRING("monosiri_follow_00"), 0, 0, LW(2))
    CALL(evt_mario_key_onoff, 1)
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i02_memo"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

/**
 * Map-init helper that sets up the A-press case event for the wastebasket in Frankly's house (evt_monosiri)
 * while monosiri_check still finds a tattle Mario can learn at the current story stage.
 */
static EVT_BEGIN(evt_monosiri_follow)
    CALL(monosiri_check, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_i02_gomibako"), 0, PTR(evt_monosiri), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_cloud)
    SET_VALUE(GSWF_GOR_02_HIDDEN_WALL_BLOWN_AWAY, 1)
    SET_VALUE(LW(0), PTR(&door_data_04_ura))
    CALL_CHILD(evt_door_entry)
    RETURN
EVT_END()

/**
 * Init for Trouble Center job 19: while it is active and the ring has been lost (GSWF_TROUBLE_WORK_BASE),
 * places the Wedding Ring on the ground in the east side,
 * at (525, 0, 160) in US and later and at (-65, 120, 196) in earlier releases.
 */
static EVT_BEGIN(iri_19_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_IMPORTANT_THING)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
#if VERSION >= VERSION_US
            CALL(evt_item_entry, STRING("wr"), ITEM_WEDDING_RING, 525, 0, 160, ITEM_TYPE_STILL, GSWF_TROUBLE_WORK(1), 0)
#else
            CALL(evt_item_entry, STRING("wr"), ITEM_WEDDING_RING, -65, 120, 196, ITEM_TYPE_STILL, GSWF_TROUBLE_WORK(1), 0)
#endif
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Enters Goombella ("c_pkr") at (-240, 0, -260) facing 270 for the ending scene at Professor Frankly's house,
 * intangible, with no wall hits, the move animation on,
 * NPC status 0x2 (NPC_STATUS_FACE_MOVE_DIR) on (she turns to face her travel direction)
 * and map blending off.
 */
static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr")) // Goombella
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -240, 0, -260)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_map_blend_set_flag, 1, STRING("クリスチーヌ"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("クリスチーヌ"), 4096)
    RETURN
EVT_END()

/**
 * Background part of the ending scene in Professor Frankly's house:
 * loops Goombella walking back and forth in front of the professor,
 * with their talk animations (PKR_T_1 and T_1) alternating so they appear to talk.
 */
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
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_open_book"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tizu_on"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -140, 0, -225)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(evt_door_open_quick)
    CALL(evt_cam3d_evt_set, -185, 90, 78, -185, 40, -218, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 0, STRING("クリハカセ"), NPC_FLAG_INSTANT_TURN)
    SPAWN_EVENT(epilogue_evt_sub)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_print, 0, STRING("ep_36"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 900, FADE_IN_BLACK, 900)
    CALL(evt_bero_mapchange, STRING("hei_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(dearu_event)
    IF_INT_EQUAL(GSWF_GOR_UP_ARROW_USED, 0)
        SET_VALUE(GSWF_GOR_MERLON_HINT_PENDING, 1)
    ELSE
        SET_VALUE(GSWF_GOR_MERLON_HINT_PENDING, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Loops forever hiding the outer wall "S_gai_kabe_mae"
 * when the camera looks at it from the front (camera_front_check) and showing it otherwise.
 */
static EVT_BEGIN(front_wall_keshi)
    LOOP(0)
        CALL(camera_front_check, LF(0))
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_gai_kabe_mae"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_gai_kabe_mae"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Loops forever: when the tracked camera road changes from the upper road to the ground one and Mario is at x >= 370,
 * hides the house 05 exterior "S_ie_05_soto" until the camera has finished turning, then shows it again.
 */
static EVT_BEGIN(tobiori_check)
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
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_05_soto"), MAP_OBJ_FLAG_HIDDEN)
        CALL(camera_move_check)
        DEBUG_PUT_MSG(STRING("カメラ移動終了"))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_05_soto"), MAP_OBJ_FLAG_HIDDEN)
    END_LOOP
    RETURN
EVT_END()

/**
 * Map init for gor_02 (Rogueport east side).
 * Picks the music by story stage, sets the exits, creates the NPCs, houses and their doors, the item box,
 * the Shine Sprite blocks and the Star Pieces, and starts whichever story cutscene belongs to the current stage
 * (the epigraphy scenes at Professor Frankly's, Frankly's lecture, the epilogue).
 */
EVT_BEGIN(gor_02_init_evt)
    // Music and ambience: Frankly's theme during his scenes, otherwise the Rogueport theme.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_OR_EQUAL(STG0_15_THE_PEDESTAL)
        CASE_OR_EQUAL(STG1_30_ZESS_T)
        CASE_OR_EQUAL(STG2_40_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG3_50_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG4_30_THOUSAND_YEAR_DOOR)
        CASE_OR_EQUAL(STG5_60_RETURN_TO_KEELHAUL)
        CASE_OR_EQUAL(STG6_70_THOUSAND_YEAR_DOOR)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_FRANKURI1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CASE_END
        CASE_DEFAULT
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR3"))
        END_IF
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))

    // The exits are set up.
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_58_FAIR_WINDS)
        SPAWN_EVENT(dearu_event)
    ELSE
        SET_VALUE(GSWF_GOR_MERLON_HINT_PENDING, 0)
    END_IF

    // The NPCs are created, including the thief and Goombella stand-in used in cutscenes, and the water
    // and light animations start.
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
        CALL(evt_npc_entry, STRING("盗賊"), STRING("c_borodo_n")) // a bandit
        CALL(evt_npc_set_tribe, STRING("盗賊"), STRING("盗賊"))
        CALL(evt_npc_set_position, STRING("盗賊"), 0, -1000, 0)
        CALL(evt_npc_set_ry, STRING("盗賊"), 90)
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("noname"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("ie_01_light"), MAP_ANIM_PLAY_LOOP, 0)

    // Fall recovery points, the spinning Star Piece floors, the item box, the three Shine Sprite blocks
    // and the three Star Pieces are set up.
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(evt_gazigazi_entry)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -220, 0, 50, STRING("S_yuka_A01"), 125, GSWF_STARPIECE_07)
    CALL(evt_mobj_kururing_floor, STRING("kururin2"), 500, 124, -75, STRING("S_i06_kabe_01A"), 125, GSWF_STARPIECE_11)
    CALL(evt_mobj_itembox, STRING("mobj_box01"), 175, -50, 350, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box01), GSWF_GOR_02_DOUBLE_DIP_CHEST)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), -250, 288, -500, 0, GSWF_SHINE_00, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk2"), -30, 60, -215, 0, GSWF_SHINE_01, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk3"), 540, 60, -260, 0, GSWF_SHINE_04, 0)
    CALL(evt_item_entry, STRING("item_star1"), ITEM_STAR_PIECE, -3, 40, -325, ITEM_TYPE_STILL, GSWF_STARPIECE_08, 0)
    CALL(evt_item_entry, STRING("item_star2"), ITEM_STAR_PIECE, 65, 150, -165, ITEM_TYPE_STILL, GSWF_STARPIECE_09, 0)
    CALL(evt_item_entry, STRING("item_star3"), ITEM_STAR_PIECE, 50, 0, -390, ITEM_TYPE_STILL, GSWF_STARPIECE_10, 0)
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(evt_door_02_entry)
    SET_VALUE(LW(0), PTR(&door_data_03))
    CALL_CHILD(evt_door_03_entry)
    SET_VALUE(LW(0), PTR(&door_data_03_roll))
#if VERSION <= VERSION_DEMO_JP
    CALL_CHILD(evt_door_03_roll_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_00_PETAL_MEADOWS)
#else
    CALL_CHILD(evt_door_03_roll_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_00_PETAL_MEADOWS)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_04_naka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ie_04C"), 1, 0)
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie_04_doa01"), 0, PTR(evt_door_04_junbi), 0)
        ELSE
        SET_VALUE(LW(0), PTR(&door_data_04))
        CALL_CHILD(evt_door_entry)
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_05))
    CALL(evt_door_data_copy, LW(0), GW(11))
    SET_VALUE(LW(0), GW(11))
    IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEED_A_KEY)
        CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc))
    ELSE
        CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc_01))
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(2), 0, PTR(evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 4)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    IF_INT_EQUAL(GSWF_GOR_02_HOUSE_KEY_LOCK_UNLOCKED, 0)
        CALL(evt_mobj_lock, STRING("mobj_lock00"), ITEM_HOUSE_KEY, 446, 18, 50, 0, PTR(evt_chk), PTR(evt_unlock), GSWF_GOR_02_HOUSE_KEY_LOCK_UNLOCKED)
    ELSE
        SET_VALUE(LW(0), PTR(&door_data_06))
        CALL(evt_door_data_copy, LW(0), LW(1))
        SET_VALUE(LW(0), LW(1))
        IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_NEED_A_KEY), 0)
            CALL(evt_door_set_param, LW(0), 0, PTR(&door_data_06_npc))
        ELSE
            CALL(evt_door_set_param, LW(0), 0, PTR(&door_data_06_npc_01))
        END_IF
        CALL_CHILD(evt_door_entry)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG0_10_REMOVING_THE_FENCE)
        // The removed fence piece is hidden and its collision disabled.
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_saku04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_saku_04"), 1, 0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG0_10_REMOVING_THE_FENCE)
            CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
            CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
            CALL(evt_npc_set_position, STRING("クリスチーヌ"), 0, -1000, 0)
            CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_10_REMOVING_THE_FENCE)
        SPAWN_EVENT(kurihakase_lecture)
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG0_15_THE_PEDESTAL)
        SPAWN_EVENT(epigraphy_map)
        CASE_EQUAL(STG1_30_ZESS_T)
        SPAWN_EVENT(epigraphy_map_after_stage1)
        CASE_EQUAL(STG2_40_THOUSAND_YEAR_DOOR)
        SPAWN_EVENT(epigraphy_map_after_stage2)
        CASE_EQUAL(STG3_50_THOUSAND_YEAR_DOOR)
        SPAWN_EVENT(epigraphy_map_after_stage3)
        CASE_EQUAL(STG4_30_THOUSAND_YEAR_DOOR)
        SPAWN_EVENT(epigraphy_map_after_stage4)
        CASE_EQUAL(STG5_60_RETURN_TO_KEELHAUL)
        SPAWN_EVENT(epigraphy_map_after_stage5)
        CASE_EQUAL(STG6_70_THOUSAND_YEAR_DOOR)
        SPAWN_EVENT(epigraphy_map_after_stage6)
    END_SWITCH
    SPAWN_EVENT(evt_gor_offscren_door_entry)
    SPAWN_EVENT(gor_goki1_urouro)
    SPAWN_EVENT(gor_goki2_urouro)
    SPAWN_EVENT(gor_irai_init)
    IF_INT_EQUAL(GSWF_GOR_02_HIDDEN_WALL_BLOWN_AWAY, 0)
        // The wall cover Flurrie can blow away in front of the Trouble Center's back door is set up.
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("S_i04_kabe_msk"), STRING("A_i04_kabe_msk"), PTR(evt_cloud))
        BEGIN_INLINE_EVENT
            LOOP(0)
                IF_INT_EQUAL(GSWF_GOR_02_HIDDEN_WALL_BLOWN_AWAY, 1)
                    BREAK_LOOP
                END_IF
                CALL(evt_cloud_get_mode, LW(0))
                IF_INT_NOT_EQUAL(LW(0), -1)
                    IF_INT_EQUAL(LW(0), CLOUD_STATE_WAIT_BREATH)
                        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i04_ura_01B"), MAP_OBJ_FLAG_HIDDEN)
                    ELSE
                        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i04_ura_01B"), MAP_OBJ_FLAG_HIDDEN)
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    ELSE
        SET_VALUE(LW(0), PTR(&door_data_04_ura))
        CALL_CHILD(evt_door_entry)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i04_kabe_msk"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_i04_kabe_msk"), 1, 0)
    END_IF

    // The harbors for the ships, mail, and the epilogue scene are set up.
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_sanbashi_01"), STRING("Ship_Change01"))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_sanbashi_02"), STRING("Ship_Change02"))
    SPAWN_EVENT(front_wall_keshi)
    SPAWN_EVENT(tobiori_check)
    SPAWN_EVENT(evt_sub_mail_event)
    CALL_CHILD(evt_monosiri_follow)
    SPAWN_EVENT(iri_19_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    RETURN
EVT_END()
