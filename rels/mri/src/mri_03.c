#include "mri_puni.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"
#include "party.h"

#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "memory.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"

static u8* _ap;
void puniMoveAtari(PuniData* puni, Vec pos, Vec dir, s32 flag, f32 speed);
void puniModeEventparent(PuniData* puni);

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_win_item_desttable);
USER_FUNC(evt_win_item_maketable);
USER_FUNC(evt_win_item_select);
USER_FUNC(puni_away);
USER_FUNC(puni_parent);
static void alpha_tev(MapTevCallbackInfo* info);
static USER_FUNC(alpha_init_func);
static USER_FUNC(alpha_set_func);

EvtWord jail_alpha_mario_check[24];
EvtWord jail_alpha[220];
EvtWord after_2hours[73];
static f32 spline_dt[24];
f32 spline_dt1[15];
EvtWord elder_rescue[771];
static EvtWord elder_init[13];
static EvtWord elder_talk[13];
static EvtWord sister_init[13];
EvtWord sister_regl[21];
static EvtWord sister_talk[8];
static EvtWord guide_init[16];
static EvtWord guide_talk[8];
EvtWord puni_3_init[17];
EvtWord puni_3_talk[20];
static NpcSetupInfo npcEnt[5];
EvtWord bero_custom_00[37];
static BeroEntry bero_entry_data[3];
static s16 puni_rand_position[178];
EvtWord jail_init_00[52];
EvtWord red_jail_open[63];
EvtWord evt_opendoor00[19];
static EvtWord evt_unlock00[6];
EvtWord evt_unlock00_01[4];
s32 key_table00[2];
EvtWord evt_check00[23];
#ifdef VERSION_G8ME01
EvtWord blue_jail_open[60];
#else
EvtWord blue_jail_open[65];
#endif
EvtWord evt_opendoor01[19];
#ifdef VERSION_G8ME01
static EvtWord evt_unlock01[9];
#else
static EvtWord evt_unlock01[8];
#endif
s32 key_table01[3];
EvtWord evt_check01[90];
EvtWord bluejail_alpha[119];
EvtWord bluejail_event[249];
static EvtWord mario_kirimomi_event[50];
EvtWord party_kirimomi_event[57];
s32 JailAwayTable_New[15];
EvtWord kinoko_small[35];
#ifdef VERSION_G8ME01
EvtWord guide_sister[769];
#else
EvtWord guide_sister[775];
#endif
EvtWord elder_shaberi[13];
EvtWord toraware_elder[423];
EvtWord guide_sister_before[128];
static KusaAccess ueki_access_data[5];
EvtWord mri_03_init_evt[206];

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

static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(jail_alpha_mario_check)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(2), -81)
            SET_VALUE(GW(2), 80)
            ELSE
            SET_VALUE(GW(2), 255)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(jail_alpha)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_saku"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_01"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_saku"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_01"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("プニ族長老"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド妹"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("プニ族"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("プニ族３"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイドsp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド妹sp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ちびトゲsp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ライバルsp"), 2048)
    IF_INT_EQUAL(GSWF(2832), 0)
        CALL(evt_map_blend_set_mobj_flag, 1, STRING("mobj_lock00"), 2048)
    END_IF
    IF_INT_EQUAL(GSWF(2834), 0)
        CALL(evt_map_blend_set_mobj_flag, 1, STRING("mobj_lock01"), 2048)
    END_IF
    IF_INT_EQUAL(GSWF_STARPIECE_46, 0)
        CALL(evt_map_blend_set_mobj_flag, 1, STRING("kururin1"), 2048)
    END_IF
    SPAWN_EVENT(PTR(jail_alpha_mario_check))
    SET_VALUE(LW(10), 255)
    SET_VALUE(GW(2), 255)
    SET_VALUE(GW(3), 255)
    SET_VALUE(GF(2), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(2), 0)
            IF_INT_LESS(GW(2), LW(10))
                ADD(LW(10), -5)
                IF_INT_GREATER(GW(2), LW(10))
                    SET_VALUE(LW(10), GW(2))
                END_IF
            END_IF
            IF_INT_GREATER(GW(2), LW(10))
                ADD(LW(10), 5)
                IF_INT_LESS(GW(2), LW(10))
                    SET_VALUE(LW(10), GW(2))
                END_IF
            END_IF
            CALL(evt_map_set_blend, 0, 255, 255, 255, LW(10))
            ELSE
            IF_INT_LESS(GW(3), LW(10))
                ADD(LW(10), -5)
                IF_INT_GREATER(GW(3), LW(10))
                    SET_VALUE(LW(10), GW(3))
                END_IF
            END_IF
            IF_INT_GREATER(GW(3), LW(10))
                ADD(LW(10), 5)
                IF_INT_LESS(GW(3), LW(10))
                    SET_VALUE(LW(10), GW(3))
                END_IF
            END_IF
            CALL(evt_map_set_blend, 0, 255, 255, 255, LW(10))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(after_2hours)
    CALL(evt_map_blend_off, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_map_blend_set_flag, 0, STRING("mario"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("party"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("プニ族長老"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ガイド妹"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ガイド"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("プニ族"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("プニ族３"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ガイドsp"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ガイド妹sp"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ちびトゲsp"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("ライバルsp"), 2048)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    RETURN
EVT_END()

static f32 spline_dt[] = {
    132.0f, 0.0f, -153.0f, 95.267204f, 0.0f, -108.09037f, 81.30669f, 0.0f,
    -100.69795f, 79.727806f, 0.0f, -73.32686f, 117.33458f, 0.0f, -59.17779f, 162.0448f,
    0.0f, -53.076424f, 228.65108f, 0.0f, 36.325317f, 245.45486f, 0.0f, 22.319448f,
};

f32 spline_dt1[] = {
    98.0f, 0.0f, -156.0f, 65.11044f, 0.0f, -80.236275f, 56.63583f, 0.0f,
    -57.388996f, -4.335236f, 0.0f, -40.86129f, -145.0f, 0.0f, -60.0f,
};

EVT_BEGIN(elder_rescue)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_cam3d_evt_set, 105, 82, 92, 105, 30, -203, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族長老"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL(evt_puni_set_mode, 1, 30)
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_puni_set_position, 1, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    SET_VALUE(GW(3), 80)
    SET_VALUE(GF(2), 1)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), 98, -156, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_00"), 0, STRING("ガイド"))
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_mov_pos2, 69, -144, FLOAT(100.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_move_pos2, 0, 63, -170, FLOAT(150.0f))
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(300)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, FLOAT(105.0f), FLOAT(73.59961f), FLOAT(44.299805f), FLOAT(105.0f), FLOAT(30.0f), FLOAT(-203.0f), 1000, EASING_WAVE_SHRINKING_FASTER)
        WAIT_MS(900)
        CALL(evt_cam3d_evt_set, 105, 82, 92, 105, 30, -203, 400, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_BIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_WAVE_SHRINKING_FASTER, FLOAT(1.0f), FLOAT(2.5f), 700)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 2)
        CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 1, -1000)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG2_PUNI1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_01"), 0, STRING("プニ族長老"))
    WAIT_WHILE(GF(0))
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, FLOAT(2.5f), FLOAT(1.0f), 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_continue)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_02"), 0, STRING("ガイド"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_03"), 0, STRING("プニ族長老"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_04"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族長老"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族長老"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_05"), 0, STRING("プニ族長老"))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SLOT(1) | PSND_SILENT_LEVEL(6))
    CALL(evt_fade_out, 1000)
    WAIT_MS(100)
    CALL(evt_fade_entry, 12, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_06"), 0, 0)
    BEGIN_INLINE_EVENT_GET_ID(GW(1))
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SLEEP1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(3000)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_sleep_on)
    CALL(evt_party_sleep_on, 0)
    WAIT_MS(1000)
    CALL(evt_fade_entry, 11, 0, 0, 0, 0)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT | PSND_SLOT(1), 0)
    CALL(evt_fade_in, 1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_07"), 0, STRING("プニ族長老"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_08"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族長老"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族長老"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_09"), 0, STRING("プニ族長老"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_10"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族長老"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_11"), 0, STRING("プニ族長老"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 1000)
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_status_onoff, 1, STRING("プニ族長老"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_sub_spline_init, 0, 0, PTR(spline_dt), 8, 30, 0)
    LABEL(0)
    CALL(evt_sub_spline_get_value, LW(3), LW(4), LW(5), LW(6))
    SET_VALUE(LW(0), STRING("プニ族長老"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("プニ族長老"), LW(3), LW(5), 160, 0, 0)
    DELETE_EVENT(LW(9))
    IF_INT_EQUAL(LW(6), 1)
        GOTO(0)
    END_IF
    CALL(evt_npc_set_position, STRING("プニ族長老"), 0, -1000, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_12"), 0, STRING("ガイド"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_cam3d_evt_set, -155, 85, 204, -155, 50, 7, 1500, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("ガイド"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_sub_spline_init, 0, 0, PTR(spline_dt1), 5, 15, 0)
    LABEL(1)
    CALL(evt_sub_spline_get_value, LW(3), LW(4), LW(5), LW(6))
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), LW(3), LW(5), 160, 0, 0)
    DELETE_EVENT(LW(9))
    IF_INT_EQUAL(LW(6), 1)
        GOTO(1)
    END_IF
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_12_01"), 0, STRING("ガイド"))
    DELETE_EVENT(GW(1))
    CALL(evt_mario_sleep_off)
    CALL(evt_party_sleep_off, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pos, -10, 0, -52)
        CALL(evt_mario_mov_pos2, -110, -52, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_pos, 0, 32, 0, -52)
        CALL(evt_party_move_pos2, 0, -68, -52, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_12_02"), 0, STRING("ガイド妹"))
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    CALL(evt_puni_set_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_puni_set_mode, 1, 1)
    SET_VALUE(GF(2), 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_14_PUNI_ELDER_FREED)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(elder_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG2_14_PUNI_ELDER_FREED)
        CALL(evt_npc_set_position, STRING("me"), 132, 0, -153)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(elder_talk)
    IF_INT_EQUAL(GSWF(2832), 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e14_13"), 0, STRING("me"))
        ELSE
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(sister_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG2_20_PUNIO_PETUNI)
        CALL(evt_npc_set_position, STRING("me"), -192, 0, -142)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(sister_regl)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_01_A_DARK_FIGURE)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_18_JABBLE)
        RETURN
    END_IF
    LOOP(0)
        CALL(evt_set_dir_to_target, STRING("me"), STRING("ガイドsp"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sister_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e14_14"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(guide_init)
    IF_INT_EQUAL(GSWF(2836), 1)
        CALL(evt_npc_set_position, STRING("me"), -145, 0, -60)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(guide_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_04_02"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(puni_3_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG2_26_PRAISE_FROM_THE_PUNI)
        IF_INT_EQUAL(GSWF_MRI_LEFT_AFTER_BOSS_BATTLE, 1)
            CALL(evt_npc_set_position, STRING("me"), 50, 0, 30)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_3_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_018"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_018_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
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
        /*"ガイド妹"*/"\x83\x4B\x83\x43\x83\x68\x96\x85", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        sister_init, // initEvent
        sister_regl, // regularEvent
        sister_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガイド"*/"\x83\x4B\x83\x43\x83\x68", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
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
        /*"プニ族３"*/"\x83\x76\x83\x6A\x91\xB0\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        puni_3_init, // initEvent
        NULL, // regularEvent
        puni_3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(bero_custom_00)
    SET_VALUE(GF(0), 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_01_A_DARK_FIGURE)
        CALL(evt_bero_exec_onoff, 0, 4)
        BEGIN_INLINE_EVENT
            WAIT_MS(2000)
            CALL(evt_bero_exec_onoff, 0, 2)
            WAIT_MS(1000)
            SET_VALUE(GF(0), 0)
            CALL(evt_bero_exec_onoff, 1, 2)
            CALL(evt_bero_exec_onoff, 1, 4)
        END_INLINE_EVENT
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_custom_00, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_20", // nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_04", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static s16 puni_rand_position[] = {
    -68, -143, -146, -199, -101, -286, -126, -253,
    -144, -149, -229, -226, -183, -191, -40, -200,
    -166, -202, -218, -153, -78, -160, -197, -271,
    -244, -212, -145, -179, -59, -233, -64, -214,
    -177, -240, -44, -180, -230, -247, -147, -298,
    -115, -152, -260, -201, -211, -216, -110, -268,
    -140, -239, -175, -282, -98, -162, -217, -174,
    -40, -240, -78, -199, -161, -252, -165, -144,
    -176, -219, -131, -164, -249, -253, -87, -246,
    -50, -282, -212, -237, -235, -291, -165, -182,
    -73, -180, -141, -218, -93, -182, -96, -210,
    -223, -275, -197, -156, -267, -263, -107, -248,
    -89, -144, -226, -202, -80, -227, -208, -288,
    -90, -267, -244, -273, -144, -263, -123, -209,
    -179, -167, -128, -189, -236, -179, -248, -233,
    -237, -158, -48, -148, -120, -294, -67, -271,
    -59, -195, -44, -220, -121, -230, -129, -276,
    -48, -258, -179, -262, -256, -151, -155, -280,
    -58, -166, -256, -181, -129, -136, -194, -250,
    -114, -175, -193, -209, -213, -258, -159, -163,
    -67, -251, -109, -194, -208, -195, -107, -134,
    -159, -231, -194, -229, -100, -229, -81, -286,
    -197, -176,
};

EVT_BEGIN(jail_init_00)
    SET_VALUE(LW(10), 14)
    SET_VALUE(LW(11), 0)
    LOOP(0)
        CALL(evt_mri_get16bitdata, PTR(puni_rand_position), LW(11), LW(0))
        ADD(LW(11), 1)
        CALL(evt_mri_get16bitdata, PTR(puni_rand_position), LW(11), LW(2))
        ADD(LW(11), 1)
        CALL(evt_puni_init_puni, LW(10), 14, LW(0), 0, LW(2), 0)
        CALL(evt_puni_flag_onoff, LW(10), 0, PUNI_FLAG_NOT_IN_PARTY)
        CALL(evt_puni_pera_onoff, LW(10), 1)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(red_jail_open)
    SET_VALUE(GSWF(2833), 1)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("S_hako_r_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_GRID_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 85, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_hako_r_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("A_hako_r1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_hako_r_01"), 1, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_opendoor00)
    CALL(evt_mapobj_rotate, 1, STRING("S_hako_r_doa"), 0, 85, 0)
    CALL(evt_hitobj_onoff, STRING("A_hako_r1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_hako_r_01"), 1, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock00)
    CALL_CHILD(PTR(red_jail_open))
    SPAWN_EVENT(PTR(elder_rescue))
    RETURN
EVT_END()

EVT_BEGIN(evt_unlock00_01)
    SPAWN_EVENT(PTR(red_jail_open))
    RETURN
EVT_END()

s32 key_table00[] = {
    ITEM_RED_KEY, -1,
};

EVT_BEGIN(evt_check00)
    CALL(evt_pouch_check_item, ITEM_RED_KEY, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_table00), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(blue_jail_open)
    SET_VALUE(GSWF(2835), 1)
#ifndef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 0)
#endif
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("S_hako_l_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_GRID_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 85, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("S_hako_l_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("A_hako_l_01"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("S_hako_l1"), 1, 0)
#ifdef VERSION_G8ME01
    CALL_CHILD(guide_sister)
#endif
    CALL(evt_mario_key_onoff, 1)
#ifndef VERSION_G8ME01
    SPAWN_EVENT(PTR(guide_sister))
    CALL(evt_exit_case_evt)
#endif
    RETURN
EVT_END()

EVT_BEGIN(evt_opendoor01)
    CALL(evt_mapobj_rotate, 1, STRING("S_hako_l_doa"), 0, 85, 0)
    CALL(evt_hitobj_onoff, STRING("A_hako_l_01"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("S_hako_l1"), 1, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock01)
#ifdef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
#else
    CALL(evt_pouch_remove_item, ITEM_BLUE_KEY, LW(0))
#endif
    SPAWN_EVENT(PTR(blue_jail_open))
    RETURN
EVT_END()

s32 key_table01[] = {
    ITEM_RED_KEY, ITEM_BLUE_KEY, -1,
};

EVT_BEGIN(evt_check01)
    CALL(evt_win_item_maketable, PTR(key_table01), LW(10))
    IF_INT_NOT_EQUAL(LW(10), 0)
        CALL(evt_win_item_select, 0, LW(10), LW(0), 0)
        SWITCH(LW(0))
            CASE_EQUAL(17)
            CASE_EQUAL(16)
            CALL(evt_mobj_exec_cancel, STRING("me"))
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_e14_15"), 0, 0)
            CALL(evt_puni_get_lsw_index, LSW(0), 0, LW(0), LW(1), LW(2), LW(3))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
                CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
                CALL(evt_msg_print, 0, STRING("stg2_mri_e14_16"), 0, STRING("ガイドsp"))
                CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
                CALL(evt_msg_print, 0, STRING("stg2_mri_e14_17"), 0, STRING("ガイド妹"))
            END_IF
            CALL(evt_mario_key_onoff, 1)
            CASE_DEFAULT
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_SWITCH
    END_IF
    CALL(evt_win_item_desttable, LW(10), 2)
EVT_END()

EVT_BEGIN(bluejail_alpha)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_saku"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_l_01"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_saku"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_01"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("プニ族長老"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド妹"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("プニ族"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイドsp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ガイド妹sp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ちびトゲsp"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ライバルsp"), 2048)
    CALL(evt_map_set_blend, 0, 255, 255, 255, 255)
    IF_INT_EQUAL(GSWF(2832), 0)
        CALL(evt_map_blend_set_mobj_flag, 1, STRING("mobj_lock00"), 2048)
    END_IF
    IF_INT_EQUAL(GSWF(2834), 0)
        CALL(evt_map_blend_set_mobj_flag, 1, STRING("mobj_lock01"), 2048)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(bluejail_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_puni_puninum_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    DELETE_EVENT(GW(0))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL_CHILD(PTR(bluejail_alpha))
    CALL(evt_npc_set_position, STRING("ガイド妹"), -192, 0, -120)
    CALL(evt_npc_set_ry_lr, STRING("ガイド妹"), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド妹"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_puni_set_position, 14, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("プニ族"), -114, 0, -120)
    CALL(evt_npc_set_ry_lr, STRING("プニ族"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_cam3d_evt_set, -160, 160, 540, -160, 60, 0, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 80, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_map_set_blend, 0, 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -160, 90, 147, -160, 60, 0, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_00"), 0, STRING("プニ族"))
    CALL(evt_npc_get_position, STRING("ガイド妹"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNIKO_KINOKO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_item_entry, STRING("item00"), ITEM_DRIED_SHROOM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    ADD(LW(0), 20)
    CALL(evt_item_move_3d_position, STRING("item00"), LW(0), LW(1), LW(2), 300, EASING_LINEAR)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_01"), 0, STRING("ガイド妹"))
    CALL(evt_cam3d_evt_set, -159, 197, 270, -159, 81, -32, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_02"), 0, STRING("プニ族"))
    CALL(evt_npc_get_position, STRING("ガイド妹"), LW(0), LW(1), LW(2))
    CALL(evt_item_move_3d_position, STRING("item00"), LW(0), LW(1), LW(2), 300, EASING_LINEAR)
    CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_03"), 0, STRING("ガイド妹"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_04"), 0, STRING("プニ族"))
    WAIT_MS(1500)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_19_INSIDE_THE_BLUE_CELL)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("mri_08"), STRING("e_bero"))
    RETURN
EVT_END()

static EVT_BEGIN(mario_kirimomi_event)
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_get_dispdir, LW(8))
    SET_VALUE(LW(9), 1980)
    ADD(LW(9), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(8), LW(9), 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mario_set_dispdir, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(party_kirimomi_event)
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
    CALL(evt_party_get_dispdir, 0, LW(8))
    SET_VALUE(LW(9), 1980)
    ADD(LW(9), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(8), LW(9), 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_party_set_dispdir, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

s32 JailAwayTable_New[] = {
    -83, -216, -153, -195, -205, -154, -241, -95,
    -220, -25, -190, 0, -160, 40, 10000,
};

void puniModeEventparent(PuniData* puni) {
    s32* tbl = JailAwayTable_New;
    s32 i;
    s32 bestIdx;
    f32 best;
    f32 d;
    Vec dir;
    Vec p;
    Vec pos;
    f32 mag;

    switch (puni->modeStep) {
        case 0:
            best = 1000000.0f;
            bestIdx = -1;
            for (i = 0; i < 4; i++) {
                p.x = tbl[i * 2];
                p.y = 0.0f;
                p.z = tbl[i * 2 + 1];
                d = PSVECSquareDistance(&p, &puni->pos);
                if (best > d) {
                    best = d;
                    bestIdx = i;
                }
            }
            puni->modeVar0 = bestIdx;
            puni->prevAnim = 2;
            puni->modeStep = 1;
        case 1:
            p.x = tbl[puni->modeVar0 * 2];
            p.y = 0.0f;
            p.z = tbl[puni->modeVar0 * 2 + 1];
            if (PSVECSquareDistance(&p, &puni->pos) > 100.0f) {
                PSVECSubtract(&p, &puni->pos, &dir);
                PSVECNormalize(&dir, &dir);
                dir.y = 0.0f;
                PSVECScale(&dir, &dir, 1.5f);
                PSVECAdd(&puni->vel, &dir, &puni->vel);
            } else if (puni->modeVar0 == puni->modeVar1 + 5) {
                puni->vel = (Vec){0.0f, 0.0f, 0.0f};
                puni->modeStep = 2;
            } else if (tbl[puni->modeVar0 * 2 + 2] != 10000) {
                puni->modeVar0++;
            } else {
                puni->vel = (Vec){0.0f, 0.0f, 0.0f};
                puni->modeStep = 2;
            }
            break;
        case 2:
            puni->prevAnim = 0;
            puni->modeStep = 3;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            p = (Vec){-160.0f, 0.0f, 40.0f};
            if (PSVECSquareDistance(&p, &puni->pos) < 4900.0f) {
                PSVECSubtract(&puni->pos, &p, &dir);
                PSVECNormalize(&dir, &dir);
                dir.y = 0.0f;
                PSVECScale(&dir, &dir, 0.7f);
                PSVECAdd(&puni->vel, &dir, &puni->vel);
                puni->maxSpeed = 2.1f;
                mag = PSVECMag(&puni->vel);
                pos = puni->pos;
                pos.y += 1.0f;
                if (mag != 0.0f) {
                    PSVECNormalize(&puni->vel, &dir);
                } else {
                    dir = (Vec){0.0f, 0.0f, 0.0f};
                }
                puniMoveAtari(puni, pos, dir, 0, mag);
            } else {
                puni->modeStep = 11;
            }
            break;
        case 11:
            puni->vel = (Vec){0.0f, 0.0f, 0.0f};
            if (puni->pos.x < -160.0f) {
                puni->dir = 90.0f;
            } else {
                puni->dir = 270.0f;
            }
            puni->modeStep = 12;
            break;
        case 12:
            break;
    }
}

USER_FUNC(puni_parent) {
    s32 offset = evtGetValue(event, event->args[0]) * sizeof(PuniData);
    PuniData* puni = (PuniData*)((u8*)puniGetPunidat() + offset);
    s16 side;

    if (puni->pos.x > -180.0f) {
        side = 1;
        puni->modeVar1 = event->lwData[11] % 2;
        event->lwData[11]++;
    } else {
        side = 2;
        puni->modeVar1 = event->lwData[12] % 2;
        event->lwData[12]++;
    }
    puni->flags |= PUNI_FLAG_COLLIDE;
    puni->groupId = side;
    puni->mode = 29;
    puni->modeStep = 0;
    puni->prevAnim = 1000;
    puni->modeFunc = puniModeEventparent;
    puni->moveFunc = puniMoveDecel;
    return EVT_RETURN_DONE;
}

USER_FUNC(puni_away) {
    s32 offset = evtGetValue(event, event->args[0]) * sizeof(PuniData);
    PuniData* puni = (PuniData*)((u8*)puniGetPunidat() + offset);

    puni->modeStep = 10;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(kinoko_small)
    CALL(evt_item_get_position, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), STRING("item00"))
    SET_FLOAT_VALUE(LW(11), FLOAT(0.69921875f))
    CALL(evt_item_set_scale, LW(10), LW(11))
    WAIT_MS(750)
    SET_FLOAT_VALUE(LW(11), FLOAT(0.34960938f))
    CALL(evt_item_set_scale, LW(10), LW(11))
    WAIT_MS(750)
    CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(guide_sister)
#ifndef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), 14)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    LOOP(0)
        CALL(puni_parent, LW(10))
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_puni_set_mode, 4, 14)
    CALL(evt_puni_set_anim, 4, 0)
    CALL(evt_puni_flag_onoff, 4, 0, PUNI_FLAG_COLLIDE)
    CALL(evt_puni_set_mode, 3, 14)
    CALL(evt_puni_set_anim, 3, 0)
    CALL(evt_puni_flag_onoff, 3, 0, PUNI_FLAG_COLLIDE)
    SET_VALUE(LW(10), 5)
    LOOP(9)
        CALL(evt_puni_set_mode, LW(10), 14)
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_flag_onoff, LW(10), 0, PUNI_FLAG_COLLIDE)
        ADD(LW(10), 1)
    END_LOOP
    SPAWN_EVENT(PTR(mario_kirimomi_event))
    SPAWN_EVENT(PTR(party_kirimomi_event))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_NOKO_SAIKAI1"))
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド妹"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_MS(2000)
    CALL(evt_npc_get_reglid, STRING("ガイド妹"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
    SET_VALUE(LW(0), STRING("ガイド妹"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド妹"), -245, -108, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 90)
    SET_VALUE(LW(0), STRING("ガイド妹"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド妹"), -180, 40, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_00"), 0, STRING("ガイド妹"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -230, -45, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -250, -40, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
        SET_VALUE(LW(0), STRING("ガイド"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイド"), -140, 40, 0, FLOAT(180.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(LW(10), 14)
    LOOP(0)
        CALL(puni_away, LW(10))
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, -160, 90, 352, -160, 30, 7, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    SET_VALUE(LW(10), STRING("ガイド"))
    SET_VALUE(LW(11), STRING("ガイド妹"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_01"), 0, LW(10))
    CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(11), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(11), LW(0), LW(1), LW(2), 200, 18)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(11), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(11), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_02"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_03"), 0, LW(10))
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_04"), 0, LW(11))
    CALL(evt_npc_set_ry, LW(11), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_05"), 0, LW(11))
    CALL(evt_npc_set_ry, LW(11), 90)
    WAIT_MS(200)
    CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNIKO_KINOKO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_item_entry, STRING("item00"), ITEM_DRIED_SHROOM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    ADD(LW(0), 20)
    CALL(evt_item_move_3d_position, STRING("item00"), LW(0), LW(1), LW(2), 500, EASING_LINEAR)
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_06"), 0, LW(11))
    CALL(evt_cam3d_evt_set, -150, 73, 278, -150, 25, 7, 500, EASING_SINE_IN_OUT)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), -150, 40, 0, FLOAT(60.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_autotalkpose, LW(10), STRING("T_1"), STRING("T_1"))
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        LOOP(0)
            CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_EAT1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(30)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, LW(10), STRING("T_1"))
    WAIT_MS(2000)
    SPAWN_EVENT(PTR(kinoko_small))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_07"), 0, LW(10))
    WAIT_FRAMES(1)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_anim, LW(10), STRING("S_1"))
    CALL(evt_npc_set_autotalkpose, LW(10), STRING("S_1"), STRING("T_1"))
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_08"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_09"), 0, LW(10))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), LW(11), 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_10"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_11"), 0, LW(10))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    CALL(evt_cam3d_evt_set, -175, 73, 278, -175, 25, 7, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_12"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_13"), 0, LW(10))
    WAIT_MS(2000)
    CALL(evt_msg_togelr, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e20_14"), 0, LW(11))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSWF(2836), 0)
    SET_VALUE(GSWF(2877), 1)
    SET_VALUE(GSW(651), 98)
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    CALL(evt_puni_init_puni, 1, 1, LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_get_position, STRING("ガイド妹"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド妹"), 0, -1000, 0)
    CALL(evt_puni_init_puni, 2, 1, LW(0), LW(1), LW(2), 0)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 1)
        CALL(evt_puni_flag_onoff, LW(10), 1, PUNI_FLAG_NOT_IN_PARTY)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GET_PARTY1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    IF_INT_EQUAL(GSWF(2878), 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e20_16"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg2_mri_e20_16_1"), 0, 0)
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG2_20_PUNIO_PETUNI)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
#ifndef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 1)
#endif
    RETURN
EVT_END()

EVT_BEGIN(elder_shaberi)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_06"), 0, LW(10))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(toraware_elder)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_puni_wait_systemmode, 2)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_WHILE(GF(0))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL_CHILD(PTR(mri_guide_hagure_check))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_puni_set_lsw_index, LSW(0), 0, 0, 0, 0, 0)
        CALL(evt_puni_set_trap_index, 1, 0)
        CALL(evt_puni_init_puni, 1, 30, 0, -1000, 0, 0)
        CALL(evt_puni_flag_onoff, 1, 1, 65536)
        CALL(evt_puni_set_position, 1, 240, 0, 0)
        CALL(evt_puni_move_position, 1, 240, 40, FLOAT(120.0f))
        CALL(evt_puni_set_mode, 1, 1)
    END_IF
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_anim, 1, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("ガイドsp"), 270)
    SET_VALUE(LW(0), STRING("ガイドsp"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイドsp"), 195, 40, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ガイドsp"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_01"), 0, STRING("ガイドsp"))
    CALL(evt_cam3d_evt_set, 135, 85, 204, 135, 50, 7, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("ガイドsp"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイドsp"), 145, -60, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SET_VALUE(LW(10), STRING("プニ族長老"))
    CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, LW(10), 90)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), 115, -100, 0, FLOAT(60.0f), 0)
    DELETE_EVENT(LW(9))
    WAIT_WHILE(GF(0))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_02"), 0, LW(10))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 175, -30, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, 210, -30, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_03"), 0, STRING("ガイドsp"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_04"), 0, LW(10))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_05"), 0, STRING("ガイドsp"))
    SPAWN_EVENT_GET_ID(PTR(elder_shaberi), LW(15))
    SET_VALUE(LW(10), STRING("ガイド妹"))
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_npc_set_position, LW(10), -165, 0, -100)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_07"), 0, LW(10))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ガイドsp"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -155, 85, 240, -155, 50, 7, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        DELETE_EVENT(LW(15))
        CALL(evt_msg_close, STRING("stg2_mri_e12_26_06"))
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("ガイドsp"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイドsp"), -140, -40, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_08"), 0, STRING("ガイドsp"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -110, -30, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -75, -30, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_09"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_10"), 0, STRING("ガイドsp"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_11"), 0, LW(10))
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("ガイドsp"), 90)
    WAIT_MS(833)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_12"), 0, STRING("ガイドsp"))
    SET_VALUE(LW(0), STRING("ガイドsp"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイドsp"), -125, -10, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_npc_get_position, STRING("ガイドsp"), LW(0), LW(1), LW(2))
    CALL(evt_puni_set_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_12_02_PUNI_ELDER_IMPRISONED)
    CALL(evt_npc_restart_regular_event, STRING("ガイド妹"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(guide_sister_before)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_puni_wait_systemmode, 2)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(500)
    CALL_CHILD(PTR(mri_guide_hagure_check))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_puni_set_position, 1, 240, 0, 0)
        CALL(evt_puni_move_position, 1, 240, 40, FLOAT(120.0f))
    END_IF
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_puni_set_position, 1, 0, -1000, 0)
    CALL(evt_puni_set_mode, 1, 30)
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_NOT_IN_PARTY)
    SET_VALUE(GSWF(2836), 1)
    CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e19_04_01"), 0, STRING("ガイド"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_change_interrupt, STRING("ガイド"), 6, 0)
        SET_VALUE(LW(0), STRING("ガイド"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイド"), -145, -60, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_change_interrupt, STRING("ガイド"), 6, PTR(guide_talk))
    END_INLINE_EVENT
    SET_VALUE(GSW_STORY_PROGRESS, STG2_19_01_PUNIO_PETUNI_BEFORE)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_03_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 3, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_EQUAL(STG2_18_JABBLE)
        CASE_GREATER_OR_EQUAL(STG2_20_PUNIO_PETUNI)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG2_PUNI2"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI4"))
    END_SWITCH
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hako_r_saku"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -150, 0, -200, STRING("S_yuka_oku"), 125, GSWF_STARPIECE_46)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hikari_01"), MAP_ANIM_PLAY_LOOP, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_12_RIVAL_ARRIVAL, STG2_19_01_PUNIO_PETUNI_BEFORE)
        SPAWN_EVENT(PTR(jail_init_00))
    END_SWITCH
    CALL(evt_mobj_lock, STRING("mobj_lock00"), 16, 65, 10, -70, 0, PTR(evt_check00), PTR(evt_unlock00), GSWF(2832))
    IF_INT_EQUAL(GSWF(2832), 1)
        IF_INT_EQUAL(GSWF(2833), 1)
            SPAWN_EVENT(PTR(evt_opendoor00))
            ELSE
            SPAWN_EVENT(PTR(evt_unlock00_01))
        END_IF
    END_IF
    CALL(evt_mobj_lock, STRING("mobj_lock01"), PTR(key_table01), -245, 10, -70, 0, PTR(evt_check01), PTR(evt_unlock01), GSWF(2834))
    IF_INT_EQUAL(GSWF(2834), 1)
        IF_INT_EQUAL(GSWF(2835), 1)
            SPAWN_EVENT(PTR(evt_opendoor01))
            ELSE
            SPAWN_EVENT(PTR(evt_unlock01))
        END_IF
    END_IF
    SPAWN_EVENT_GET_ID(PTR(jail_alpha), GW(0))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_18_JABBLE)
        SPAWN_EVENT(PTR(bluejail_event))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_01_A_DARK_FIGURE)
        SPAWN_EVENT(PTR(toraware_elder))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_19_INSIDE_THE_BLUE_CELL)
        SPAWN_EVENT(PTR(guide_sister_before))
    END_IF
    RETURN
EVT_END()

