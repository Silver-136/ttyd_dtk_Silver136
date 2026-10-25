#include "eff/eff_starstone.h"
#include "eff/n64/eff_crystal_n64.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "eff/eff_mist2.h"
#include "eff/eff_mahojin.h"
#include "eff/eff_treasure_map.h"
#include "eff/eff_itemget.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "dolphin/exi.h"
#include "eff/n64/eff_stardust2_n64.h"
#include "eff/n64/eff_stardust_n64.h"
#include "poly_trig.h"

#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "memory.h"
#include "string.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "driver/itemdrv.h"
#include "driver/imgdrv.h"
#include "driver/animdrv.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "driver/fadedrv.h"
#include "dolphin/os.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "libc_decls.h"
#include "evt/evt_npc.h"
#include "mario/mario_cam.h"
#include <math.h>

static inline f32 fid(f32 x) { return x; }
typedef struct KaitenParam { Vec pos; Vec target; f32 angle; f32 dist; } KaitenParam;
typedef struct BrokenPiece { s32 state; s32 delay; s32 dir; Vec vel; Vec pos; f32 rot; f32 rotSpeed; f32 gx; f32 gy; } BrokenPiece;
typedef struct BrokenWork { s32 copied; smartEntry* tex; smartEntry* pieces; } BrokenWork;
static u8* _ap;
static KaitenParam* ckp0;
static KaitenParam* ckp1;
static KaitenParam* ckpw;
BrokenWork broken_work;
s32 select;


void tiz_broken_draw(CameraId cameraId, void* param);
void effStarStoneDraw(CameraId camera, void* param);

static USER_FUNC(alpha_init_func);
static USER_FUNC(alpha_set_func);
static USER_FUNC(cam_kaiten_get_para);
static USER_FUNC(cam_kaiten_init);
static USER_FUNC(cam_kaiten_set_direct);
static USER_FUNC(cam_kaiten_set_para);
USER_FUNC(cam_mahojin_kaiten_main);
USER_FUNC(crystal_bgmode);
USER_FUNC(disp_lighteffect_func);
USER_FUNC(disp_treasure_map_func);
USER_FUNC(eff_starstone_sound_off);
USER_FUNC(effect_position);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_get_starstone);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfxon__);
static USER_FUNC(evtflash);
USER_FUNC(img_alpha);
USER_FUNC(is_dol_viewer);
USER_FUNC(kemuri_set_color);
USER_FUNC(kemuri_set_position);
USER_FUNC(mahojin_effect_alpha);
USER_FUNC(map_bgmode);
USER_FUNC(map_bgmode_end);
USER_FUNC(pcharge_set_position);
USER_FUNC(stardust_setting);
USER_FUNC(starstone_circle_link_func);
USER_FUNC(starstone_get_position);
USER_FUNC(starstone_set_alpha);
USER_FUNC(starstone_set_position);
static USER_FUNC(starstone_set_ry);
USER_FUNC(starstone_setting);
USER_FUNC(tiz_broken);
USER_FUNC(tiz_stone);
USER_FUNC(map_disp_onoff);
static void alpha_tev(MapTevCallbackInfo* info);
USER_FUNC(starstone_set_scale);

static EvtWord n_door_close[22];
static EvtWord n_door_open[22];
static BeroEntry bero_entry_data[4];
static EvtWord kurihakase_init[27];
static EvtWord kurihakase_talk[23];
static NpcSetupInfo npcEnt[2];
EvtWord mahojin_alpha[70];
EvtWord mahojin_alpha_up[5];
EvtWord mahojin_alpha_down[5];
EvtWord large_door_stg0_init[17];
EvtWord large_door_stg0[129];
EvtWord look_large_door[25];
EvtWord kaidan_event[69];
EvtWord evt_mahojin_move[51];
EvtWord evt_mahojin_move2[24];
EvtWord evt_fogset[42];
EvtWord evt_fogreset[82];
EvtWord mahojin_se_event[6];
EvtWord mahojin_se_event_end[2];
EvtWord disp_lighteffect[20];
EvtWord disp_treasure_map[21];
BrokenWork* broken_wp;
f32 param_tbl[5];
f32 add_tbl[5];
static const char* name_tbl[5];
EvtWord evt_mapwrite[1351];
EvtWord fake_fade[30];
EvtWord evt_starstone_up[127];
EvtWord evt_starstone_set[106];
EvtWord mahojin_event[363];
EvtWord mahojin_event1[110];
EvtWord mahojin_event2[105];
EvtWord mahojin_event3[102];
EvtWord mahojin_event4[102];
EvtWord mahojin_event5[102];
EvtWord mahojin_event6[102];
EvtWord starstone_circle_link[70];
EvtWord lastdungeon_anim_init[52];
EvtWord lastdungeon_init[36];
EvtWord lastdungeon[103];
EvtWord mahojin_event7[VERSION_PICK(2537, 2541)];
EvtWord tik_05_init_evt[VERSION_PICK(289, 287)];

static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door_a"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door_a"), 0, LW(0), 0)
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
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_06", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)look_large_door, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_04", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_00", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(kurihakase_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(378, 379)
        CALL(evt_npc_set_position, STRING("me"), -30, 23, -190)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_BETWEEN(380, 387)
        CALL(evt_npc_set_position, STRING("me"), -30, 23, -170)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kurihakase_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(378, 379)
        CALL(evt_msg_print, 0, STRING("mac_7_008"), 0, STRING("me"))
        CASE_BETWEEN(380, 387)
        CALL(evt_msg_print, 0, STRING("mac_7_014"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A", // name
        0x600, // flags
        0, // reactionFlags
        kurihakase_init, // initEvent
        NULL, // regularEvent
        kurihakase_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

USER_FUNC(starstone_set_position) {
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

USER_FUNC(starstone_get_position) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));
    f32 x = ((EffStarStoneWork*)eff->userdata)->position.x;
    f32 y = ((EffStarStoneWork*)eff->userdata)->position.y;
    f32 z = ((EffStarStoneWork*)eff->userdata)->position.z;

    evtSetFloat(event, args[1], x);
    evtSetFloat(event, args[2], y);
    evtSetFloat(event, args[3], z);
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->rotY = ry - 360.0f * ((s32)ry / 360);
    return EVT_RETURN_DONE;
}

USER_FUNC(starstone_set_alpha) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->a = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(starstone_set_scale) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 scale = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->scale = scale;
    return EVT_RETURN_DONE;
}

USER_FUNC(starstone_circle_link_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 idx = evtGetValue(event, args[1]) - 1;
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    f32 ry = evtGetFloat(event, args[5]);
    f32 r = evtGetFloat(event, args[6]);
    EffectEntry* eff = effNameToPtr(name);
    f32 angle;
    f32 s;
    f32 c;

    ry = ry - 360.0f * ((s32)ry / 360);
    angle = 6.2831855f * (360.0f * idx / 7.0f + ry) / 360.0f;
    s = sin(angle);
    c = cos(angle);

    ((EffStarStoneWork*)eff->userdata)->position.x = s * r + x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = -c * r + z;
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_starstone_sound_off) {
    ((EffStarStoneWork*)effNameToPtr((const char*)evtGetValue(event, event->args[0]))->userdata)->flags &= ~4;
    return EVT_RETURN_DONE;
}

static void alpha_tev(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    u8* alpha = _ap;

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, *alpha});
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

EVT_BEGIN(mahojin_alpha)
    CALL(evt_map_set_tevcallback, 1, PTR(alpha_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hosi_2"), 16777216)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("hosi_2"), 256)
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        ADD_FLOAT(LW(0), GW(1))
        IF_FLOAT_GREATER_OR_EQUAL(LW(0), FLOAT(255.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(255.0f))
        END_IF
        IF_FLOAT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        END_IF
        CALL(alpha_set_func, LW(0))
        IF_INT_EQUAL(GF(10), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("hosi_2"), 251658240)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hosi_2"), 256)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(mahojin_alpha_up)
    SET_VALUE(GW(1), FLOAT(6.0f))
    RETURN
EVT_END()

EVT_BEGIN(mahojin_alpha_down)
    SET_VALUE(GW(1), FLOAT(-6.0f))
    RETURN
EVT_END()

EVT_BEGIN(large_door_stg0_init)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set, -300, 700, 1135, -300, 250, 23, 0, 11)
    RETURN
EVT_END()

EVT_BEGIN(large_door_stg0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_pos, 620, 81, 40)
    CALL(evt_party_set_pos, 0, 640, 81, 40)
    CALL(evt_party_set_pos, 1, 660, 81, 40)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, 300, 700, 1135, 300, 250, 23, 6000, 0)
    WAIT_MS(6500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 1, 450, 30, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 520, 30, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 490, 30, FLOAT(120.0f))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_095_kur"), 0, STRING("extparty"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 390, 260, 11, 44, 147, -528, 1000, 11)
    WAIT_MS(1800)
    CALL(evt_cam3d_evt_set, 523, 154, 254, 448, 108, 22, 0, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_0_096_kur"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 16)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(look_large_door)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(15)
        SPAWN_EVENT(PTR(large_door_stg0))
        SET_VALUE(LW(0), 1)
        RETURN
        CASE_EQUAL(378)
        SET_VALUE(GSW_STORY_PROGRESS, 379)
        SET_VALUE(LW(0), 0)
        RETURN
    END_SWITCH
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(kaidan_event)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(0), -130)
            IF_INT_LESS(LW(0), 130)
                IF_INT_LESS(LW(2), 0)
                    CALL(evt_mario_key_onoff, 0)
                    CALL(evt_mario_normalize)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    IF_INT_LESS(LW(0), 0)
                        CALL(evt_mario_set_dir, 90, 0, 0)
                        ELSE
                        CALL(evt_mario_set_dir, 270, 0, 0)
                    END_IF
                    WAIT_MS(800)
                    CALL(evt_party_stop, 0)
                    CALL(evt_msg_print_party, STRING("mac_0_097"))
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 17)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_mahojin_move)
    SET_VALUE(GF(10), 1)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 0, -250, FLOAT(60.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_force_reset_outofscreen, 0)
        CALL(evt_party_move_pos2, 0, 30, -190, FLOAT(150.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    WAIT_MS(800)
    RETURN
EVT_END()

EVT_BEGIN(evt_mahojin_move2)
    CALL(evt_party_set_pos, 0, 30, 19, -190)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_party_set_pos, 1, -30, 19, -190)
    CALL(evt_party_set_dir, 1, 90, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_fogset)
    CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_sub_intpl_msec_init, 11, 600, 1000, 4000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), LW(5))
        ADD(LW(2), LW(0))
        CALL(evt_map_set_fog, LW(4), LW(2), 2000, LW(7), LW(8), LW(9))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_fogreset)
    CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    SET_VALUE(GW(2), LW(5))
    SET_VALUE(GW(3), LW(6))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, LW(5), 300, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(GW(2), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, LW(6), 1500, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(GW(3), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_map_set_fog, LW(4), GW(2), GW(3), LW(7), LW(8), LW(9))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(3000)
    DELETE_EVENT(LW(10))
    RETURN
EVT_END()

EVT_BEGIN(mahojin_se_event)
    WAIT_MS(3500)
    WAIT_MS(1600)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_se_event_end)
    RETURN
EVT_END()

EVT_BEGIN(disp_lighteffect)
    LOOP(0)
        CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
        CALL(disp_lighteffect_func, STRING("map_ball"), LW(0), LW(1), LW(2), GW(10))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(disp_treasure_map)
    LOOP(0)
        CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
        CALL(disp_treasure_map_func, STRING("map_map"), LW(0), LW(1), LW(2), GW(10), GW(11))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_init) {
    EvtWord* args = event->args;
    Vec target;
    Vec pos;
    f32 dist;

    ckp0 = _mapAlloc(sizeof(KaitenParam));
    memset(ckp0, 0, sizeof(KaitenParam));
    ckp1 = _mapAlloc(sizeof(KaitenParam));
    memset(ckp1, 0, sizeof(KaitenParam));
    ckpw = _mapAlloc(sizeof(KaitenParam));
    memset(ckp1, 0, sizeof(KaitenParam));
    ckp0->pos.x = evtGetFloat(event, args[0]);
    ckp0->pos.y = evtGetFloat(event, args[1]);
    ckp0->pos.z = evtGetFloat(event, args[2]);
    ckp0->target.x = evtGetFloat(event, args[3]);
    ckp0->target.y = evtGetFloat(event, args[4]);
    ckp0->target.z = evtGetFloat(event, args[5]);
    ckp1->pos.x = evtGetFloat(event, args[6]);
    ckp1->pos.y = evtGetFloat(event, args[7]);
    ckp1->pos.z = evtGetFloat(event, args[8]);
    ckp1->target.x = evtGetFloat(event, args[9]);
    ckp1->target.y = evtGetFloat(event, args[10]);
    ckp1->target.z = evtGetFloat(event, args[11]);

    pos = ckp0->pos;
    target = ckp0->target;
    target.y = 0.0f;
    pos.y = 0.0f;
    dist = PSVECDistance(&pos, &target);
    ckp0->angle = 57.29579f * (f32)atan2(-(target.x - pos.x), fid(target.z - pos.z));
    ckp0->dist = dist;

    pos = ckp1->pos;
    target = ckp1->target;
    target.y = 0.0f;
    pos.y = 0.0f;
    dist = PSVECDistance(&pos, &target);
    ckp1->angle = 57.29579f * (f32)atan2(-(target.x - pos.x), target.z - pos.z);
    ckp1->dist = dist;

    ckpw->pos = ckp0->pos;
    ckpw->target = ckp0->target;
    ckpw->angle = ckp0->angle;
    ckpw->dist = ckp0->dist;
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_set_direct) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 s = sin(6.2831855f * ckpw->angle / 360.0f);
    f32 c = cos(6.2831855f * ckpw->angle / 360.0f);

    ckpw->pos.x = ckpw->dist * s + ckpw->target.x;
    ckpw->pos.z = ckpw->target.z - ckpw->dist * c;
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = ckpw->pos;
    camera->storedAt = ckpw->target;
    camera->startTimeA = gp->renderTime;
    camera->durationA = 0;
    camera->stateA = 3;
    camera->curveA = 11;
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_get_para) {
    EvtWord* args = event->args;
    KaitenParam* p;

    switch (evtGetValue(event, args[0])) {
        case 0:
            p = ckp0;
            break;
        case 1:
            p = ckp1;
            break;
        case 2:
        default:
            p = ckpw;
            break;
    }
    evtSetFloat(event, args[1], p->pos.x);
    evtSetFloat(event, args[2], p->pos.y);
    evtSetFloat(event, args[3], p->pos.z);
    evtSetFloat(event, args[4], p->target.x);
    evtSetFloat(event, args[5], p->target.y);
    evtSetFloat(event, args[6], p->target.z);
    evtSetFloat(event, args[7], p->angle);
    evtSetFloat(event, args[8], p->dist);
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_set_para) {
    EvtWord* args = event->args;
    KaitenParam* p;

    switch (evtGetValue(event, args[0])) {
        case 0:
            p = ckp0;
            break;
        case 1:
            p = ckp1;
            break;
        case 2:
        default:
            p = ckpw;
            break;
    }
    p->pos.x = evtGetFloat(event, args[1]);
    p->pos.y = evtGetFloat(event, args[2]);
    p->pos.z = evtGetFloat(event, args[3]);
    p->target.x = evtGetFloat(event, args[4]);
    p->target.y = evtGetFloat(event, args[5]);
    p->target.z = evtGetFloat(event, args[6]);
    p->angle = evtGetFloat(event, args[7]);
    p->dist = evtGetFloat(event, args[8]);
    return EVT_RETURN_DONE;
}

USER_FUNC(disp_lighteffect_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 ry = evtGetFloat(event, args[4]);
    EffectEntry* eff = effNameToPtr(name);
    f32 s;
    f32 c;
    f32 angle = 6.2831855f * -ry / 360.0f;

    s = sin(angle);
    c = cos(angle);
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.x = 30.0f * s + x;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.y = y;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.z = 30.0f * -c + z;
    return EVT_RETURN_DONE;
}

USER_FUNC(disp_treasure_map_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 ry = evtGetFloat(event, args[4]);
    u8 alpha = evtGetValue(event, args[5]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffTreasureMapWork*)eff->userdata)->position.x = x;
    ((EffTreasureMapWork*)eff->userdata)->position.y = y;
    ((EffTreasureMapWork*)eff->userdata)->position.z = z;
    ((EffTreasureMapWork*)eff->userdata)->rotY = 180.0f + ry;
    ((EffTreasureMapWork*)eff->userdata)->alpha = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(mahojin_effect_alpha) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);

    ((EffMahojinWork*)effNameToPtr(name)->userdata)->alpha = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(effect_position) {
    ItemEntry* item = itemNameToPtr("item00");
    f32 angle = -(6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f);
    f32 x;
    f32 z;
    EffectEntry* eff;

    x = 5.0f * (f32)sin(angle);
    z = 5.0f * (f32)cos(angle);
    eff = effNameToPtr("map_wave");
    if (eff != NULL) {
        ((EffCrystalN64Work*)eff->userdata)->position.x = item->position.x + x;
        ((EffCrystalN64Work*)eff->userdata)->position.y = item->position.y;
        ((EffCrystalN64Work*)eff->userdata)->position.z = item->position.z + z;
    }
    x = 10.0f * (f32)sin(angle);
    z = 10.0f * (f32)cos(angle);
    eff = effNameToPtr("map_map");
    if (eff != NULL) {
        ((EffTreasureMapWork*)eff->userdata)->position.x = item->position.x + x;
        ((EffTreasureMapWork*)eff->userdata)->position.y = item->position.y;
        ((EffTreasureMapWork*)eff->userdata)->position.z = item->position.z + z;
        ((EffTreasureMapWork*)eff->userdata)->rotY = 180.0f - camGetPtr(CAMERA_3D)->viewYaw;
        ((EffTreasureMapWork*)eff->userdata)->alpha = evtGetValue(event, (s32)0xFD050F8B);
        ((EffTreasureMapWork*)eff->userdata)->scale = 0.9f;
    }
    x = 15.0f * (f32)sin(angle);
    z = 15.0f * (f32)cos(angle);
    eff = effNameToPtr("map_ball");
    if (eff != NULL) {
        ((EffPokopiPchargeN64Elem*)eff->userdata)->position.x = item->position.x + x;
        ((EffPokopiPchargeN64Elem*)eff->userdata)->position.y = item->position.y;
        ((EffPokopiPchargeN64Elem*)eff->userdata)->position.z = item->position.z + z;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(img_alpha) {
    Mtx trans;
    Mtx scale;
    Mtx rot;
    s32 alpha = evtGetValue(event, event->args[0]);
    ImageEntry* img = imgNameToPtr("img", gp->inBattle != 0);

    animPoseSetMaterialFlagOn(img->poseId, 0x40);
    animPoseSetMaterialEvtColor(img->poseId, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    PSMTXTrans(trans, 0.0f, 1.5f, 0.0f);
    PSMTXScale(scale, 0.9f, 0.9f, 0.9f);
    PSMTXRotRad(rot, 'x', -0.5235988f);
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat(trans, rot, img->mtx);
    return EVT_RETURN_DONE;
}

USER_FUNC(tiz_stone) {
    f32 t = evtGetValue(event, event->args[0]);
    EffectEntry* eff;

    t /= 10000.0f;
    eff = effNameToPtr("tiz_dust");
    if (eff != NULL) {
        effStardustN64SetDrawCam(eff, 7);
    }
    eff = effNameToPtr("tiz_stone");
    if (eff != NULL) {
        ((EffStarStoneWork*)eff->userdata)->cam = 7;
        ((EffStarStoneWork*)eff->userdata)->a = 255.0f * t;
        ((EffStarStoneWork*)eff->userdata)->rotY = 5.0f * (360.0f * t);
    }
    eff = effNameToPtr("tiz_dust2");
    if (eff != NULL) {
        effStardust2N64SetDrawCam(eff, 7);
    }
    return EVT_RETURN_DONE;
}

void tiz_broken_draw(CameraId cameraId, void* param) {
    GXTexObj tex;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx ident;
    CameraEntry* cam = camGetPtr(cameraId);
    s32 x;
    s32 y;
    BrokenPiece* p;
    s32 yo;
    s32 xo;
    f32 py;
    f32 rz;

    if (broken_wp->copied == 0) {
        GXSetTexCopySrc(0, 0, 608, 480);
        GXSetTexCopyDst(304, 240, GX_TF_RGB5A3, GX_TRUE);
        GXCopyTex(broken_wp->tex->address, GX_FALSE);
        GXPixModeSync();
        broken_wp->copied = 1;
    }
    GXSetNumChans(0);
    GXInitTexObj(&tex, broken_wp->tex->address, 304, 240, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCurrentMtx(0);
    PSMTXIdentity(ident);
    p = broken_wp->pieces->address;
    for (y = 0, yo = 0; y < 30; y++, yo += 16, p++) {
        py = 232.0f - yo;
        for (x = 0, xo = 0; x < 38; x++, xo += 16, p++) {
            if (p->delay == 0) {
                PSMTXScale(scale, 1.0f / 38.0f, 1.0f / 30.0f, 1.0f);
                PSMTXTrans(mtx, x, y, 0.0f);
                PSMTXConcat(scale, mtx, mtx);
                GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
                PSMTXTrans(mtx, -296.0f + xo, py, 0.0f);
                switch (p->dir) {
                    case 0:
                        rz = -45.0f;
                        break;
                    case 1:
                        rz = 45.0f;
                        break;
                    case 2:
                        rz = 225.0f;
                        break;
                    case 3:
                        rz = 135.0f;
                        break;
                }
                PSMTXTrans(rot, p->pos.x, p->pos.y, p->pos.z);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXRotRad(rot, 'z', 0.017453292f * rz);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXRotRad(rot, 'y', 0.017453292f * p->rot);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXRotRad(rot, 'z', 0.017453292f * -rz);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXConcat(cam->view, mtx, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(-8.0f, 8.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(8.0f, 8.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(8.0f, -8.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(-8.0f, -8.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
            }
        }
    }
}

#pragma opt_propagation off
#pragma opt_lifetimes off
USER_FUNC(tiz_broken) {
#ifdef VERSION_G8ME01
    f32 sq; // FAKE: tb_sqrtf expanded by hand (see below)
    f64 guess;
#endif
    s32 x;
    s32 y;
    f32 inv;
    s32 done = 0;
    BrokenPiece* p;
    BrokenPiece* q;
    s32 i;
    s32 j;
    f32 dx;
    f32 dy;
    f32 nx;
    f32 ny;
    f32 len;
    f32 angle;

    if (isFirstCall) {
        memset(broken_wp, 0, sizeof(BrokenWork));
        broken_wp->tex = smartAlloc(GXGetTexBufferSize(304, 240, GX_TF_RGB5A3, GX_FALSE, 0), 1);
        broken_wp->pieces = smartAlloc(sizeof(BrokenPiece) * 31 * 39, 1);
        broken_wp->copied = 0;
        p = broken_wp->pieces->address;
        for (y = 0; y < 30; y++) {
            for (x = 0; x < 38; x++) {
                dx = x / 37.0f - 0.5f;
                dy = 0.5f + -y / 29.0f;
                if (dy > 0.0f) {
                    if (dx < 0.0f) {
                        p->dir = 0;
                    } else {
                        p->dir = 1;
                    }
                } else {
                    if (dx < 0.0f) {
                        p->dir = 2;
                    } else {
                        p->dir = 3;
                    }
                }
                if (dx || dy) {
#ifdef VERSION_G8ME01
                    // FAKE: sqrtf(dx * dx + dy * dy) expanded by hand with the
                    // value and Newton guess in function-scope locals (sq, guess);
                    // same instructions, but the temps of the inline colour differently in US.
                    sq = dx * dx + dy * dy;
                    if (sq > 0.0f) {
                        guess = __frsqrte(sq);
                        guess = .5 * guess * (3.0 - guess * guess * sq);
                        guess = .5 * guess * (3.0 - guess * guess * sq);
                        sq = sq * (.5 * guess * (3.0 - guess * guess * sq));
                    } else if (sq < 0.0) {
                        sq = NAN;
                    } else if (isnan(sq)) {
                        sq = NAN;
                    }
                    nx = dx / sq;
                    ny = dy / sq;
                    len = sq;
#else
                    len = sqrtf(dx * dx + dy * dy);
                    inv = dx / len; // FAKE: copy temp; plain nx = dx / len changes regalloc (+4 B)
                    nx = inv;
                    ny = dy / len;
#endif
                } else {
                    nx = ny = len = 0.0f;
                }
                p->state = 0;
                p->delay = param_tbl[0] * len;
                p->pos.x = 0.0f;
                p->pos.y = 0.0f;
                p->pos.z = 0.0f;
                p->rotSpeed = 0.0f;
                p->rot = 0.0f;
                p->vel.x = -nx * param_tbl[1];
                p->vel.y = -ny * param_tbl[1];
                p->vel.z = 10.0f;
#ifdef VERSION_G8ME01
                p->vel.x *= 60.0f / gp->framerate;
                p->vel.y *= 60.0f / gp->framerate;
                p->vel.z *= 60.0f / gp->framerate;
#endif
                angle = 6.2831855f * angleABf(0.0f, 0.0f, dx, dy) / 360.0f;
#ifdef VERSION_G8ME01
                inv = param_tbl[3];
                p->gx = inv * polySinf(angle);
                p->gy = inv * -polyCosf(angle);
#else
                nx = param_tbl[3];
                p->gx = nx * polySinf(angle);
                p->gy = nx * -polyCosf(angle);
#endif
#ifdef VERSION_G8ME01
                p->gx *= 60.0f / gp->framerate;
                p->gy *= 60.0f / gp->framerate;
#endif
                p++;
            }
            p++;
        }
    }
    q = broken_wp->pieces->address;
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 38; j++) {
            switch (q->state) {
                case 0:
                    if (q->delay != 0) {
                        q->delay--;
                        break;
                    }
                    q->pos.x += q->gx;
                    q->pos.y += q->gy;
                    q->pos.x += q->vel.x;
                    q->pos.y += q->vel.y;
                    q->pos.z += q->vel.z;
                    q->vel.x *= param_tbl[2];
                    q->vel.y *= param_tbl[2];
                    q->vel.z *= param_tbl[2];
                    q->rotSpeed += param_tbl[4];
                    q->rot += q->rotSpeed;
                    if (__fabs(q->pos.x) > 340.0 || __fabs(q->pos.y) > 260.0) {
                        q->state++;
                    }
                    break;
                case 1:
                    done++;
                    break;
            }
            q++;
        }
        q++;
    }
    if (done >= 30 * 38) {
        smartFree(broken_wp->tex);
        smartFree(broken_wp->pieces);
        return EVT_RETURN_DONE;
    }
    dispEntry((CameraId)8, 0.0f, 0, tiz_broken_draw, NULL);
    return EVT_RETURN_BLOCK;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

BrokenWork* broken_wp = &broken_work;

f32 param_tbl[] = {
    90.0f, 23.0f, 0.98f, 19.4f, 2.0f,
};

f32 add_tbl[] = {
    1.0f, 0.1f, 0.01f, 0.1f, 1.0f,
};

static const char* name_tbl[] = {
    "delay",
    "v0",
    "vscale",
    "pow",
    "vr",
};

static USER_FUNC(evtflash) {
    EvtWord* args = event->args;
    s32 in = evtGetValue(event, args[0]);
    s32 wait = evtGetValue(event, args[1]);
    s32 out = evtGetValue(event, args[2]);

    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0: {
            GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
            fadeEntry(FADE_OUT_BLACK, in, color);
            event->userdata[2]++;
        }
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = wait;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3: {
            GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
            fadeEntry(FADE_IN_BLACK, out, color);
            event->userdata[2]++;
        }
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(map_disp_onoff) {
    if (evtGetValue(event, event->args[0])) {
        N_mapDispOn();
    } else {
        N_mapDispOff();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(crystal_bgmode) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((EffCrystalN64Work*)eff->userdata)->camera = 5;
    return EVT_RETURN_DONE;
}

USER_FUNC(map_bgmode) {
    EvtWord* args = event->args;
    const char* name1 = (const char*)evtGetValue(event, args[0]);
    const char* name2 = (const char*)evtGetValue(event, args[1]);
    EffectEntry* eff = effNameToPtr(name1);

    ((ItemGetWork*)eff->userdata)->camera = 5;
    ((ItemGetWork*)eff->userdata)->size = 4.3f;
    eff = effNameToPtr(name2);
    ((EffTreasureMapWork*)eff->userdata)->cameraId = 5;
    marioSetCamId(CAMERA_3D_EFFECTA);
    return EVT_RETURN_DONE;
}

USER_FUNC(map_bgmode_end) {
    EvtWord* args = event->args;
    const char* name1 = (const char*)evtGetValue(event, args[0]);
    const char* name2 = (const char*)evtGetValue(event, args[1]);
    EffectEntry* eff;

    eff = effNameToPtr(name1);
    ((ItemGetWork*)eff->userdata)->camera = 4;
    eff = effNameToPtr(name2);
    ((EffTreasureMapWork*)eff->userdata)->cameraId = 4;
    marioSetCamId(CAMERA_3D);
    return EVT_RETURN_DONE;
}

USER_FUNC(cam_mahojin_kaiten_main) {
    s32 frames;
    s32 msec;
    EvtWord* args = event->args;
    f32 start = evtGetFloat(event, args[0]);
    f32 end = evtGetFloat(event, args[1]);
    s32 cont;
    f32 cur;
    f32 total;
    f32 speed;
    s32 elapsed;
    s32 i;

    msec = evtGetValue(event, args[2]);
    frames = sysMsec2Frame(msec);
    cont = 1;

    if (event->lwData[15] == 0) {
        event->lwData[14] = 0;
        event->lwData[15] = 1;
        event->savedTime = gp->renderTime;
    }
    {s32 lo = frames * 20 / 100; s32 hi = frames * 80 / 100;
    total = 0.0f;
    speed = total;
    for (i = 0; i < frames; i++) {
        if (i <= lo) {
            speed += 1.0f;
        } else if (i >= hi) {
            speed -= 1.0f;
        }
        total += speed;
        if (i == event->lwData[14]) {
            cur = total;
        }
    }}
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed > msec) {
        elapsed = msec;
    }
    event->lwData[14] = sysMsec2Frame(elapsed);
    if (frames == event->lwData[14]) {
        cont = 0;
    }
    evtSetFloat(event, args[3], start + cur * (end - start) / total);
    evtSetValue(event, args[4], cont);
    return EVT_RETURN_DONE;
}

USER_FUNC(starstone_setting) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);
    EffectEntry* eff = effNameToPtr(name);
    EffectEntry* mahojin = effNameToPtr("mahojin");

    ((EffStarStoneWork*)eff->userdata)->flags |= 0x20;
    ((EffMahojinWork*)mahojin->userdata)[idx + 2].arg = eff;
    ((EffMahojinWork*)mahojin->userdata)[idx + 2].func = effStarStoneDraw;
    return EVT_RETURN_DONE;
}

USER_FUNC(kemuri_set_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffMist2Work*)eff->userdata)->position.x = x;
    ((EffMist2Work*)eff->userdata)->position.y = y;
    ((EffMist2Work*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(kemuri_set_color) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 r = evtGetValue(event, args[1]);
    u8 g = evtGetValue(event, args[2]);
    u8 b = evtGetValue(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffMist2Work*)eff->userdata)->r = r;
    ((EffMist2Work*)eff->userdata)->g = g;
    ((EffMist2Work*)eff->userdata)->b8 = b;
    ((EffMist2Work*)eff->userdata)->scale = 1.0f + irand(30) / 10.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(pcharge_set_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.x = x;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.y = y;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(stardust_setting) {
    EvtWord* args = event->args;
    const char* name1 = (const char*)evtGetValue(event, args[0]);
    const char* name2 = (const char*)evtGetValue(event, args[1]);
    EffectEntry* eff1 = effNameToPtr(name1);
    EffectEntry* eff2 = effNameToPtr(name2);

    effStardustN64SetDrawCam(eff1, 5);
    effStardustN64SetDrawCam(eff2, 5);
    return EVT_RETURN_DONE;
}

#ifndef VERSION_G8ME01
USER_FUNC(is_dol_viewer) {
    u32 type;

    EXIGetType(0, 2, &type);
    if (type == 0x05070000) {
        animPoseRelease(animPoseEntry("MOBJ_EFF_las_mon", 0));
        animPoseRelease(animPoseEntry("EFF_treasure_map", 0));
        animPoseRelease(animPoseEntry("MOBJ_EFF_mahojin_omote", 0));
        animPoseRelease(animPoseEntry("MOBJ_EFF_mahojin_ura", 0));
        animPoseRelease(animPoseEntry("MOBJ_EFF_laser", 0));
        animPoseRelease(animPoseEntry("OFF_d_tizu_tex", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_1", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_2", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_3", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_4", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_5", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_6", 0));
        animPoseRelease(animPoseEntry("TIZ_stg_7", 0));
    }
    return EVT_RETURN_DONE;
}
#endif

EVT_BEGIN(evt_mapwrite)
    WAIT_MS(800)
    IF_INT_EQUAL(GW(15), 0)
        CALL(evt_cam3d_evt_set, 0, 65, 40, 0, 65, -250, 1000, 11)
        WAIT_MS(1000)
        CALL(evt_msg_print_party, STRING("mac_0_098"))
        WAIT_MS(1000)
        DELETE_EVENT(LW(10))
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_dir, 90, 200, 0)
        END_INLINE_EVENT
        ELSE
        CALL(evt_mario_set_pose, STRING("M_I_N"))
        CALL(evt_cam3d_evt_set, 0, 102, 300, 0, 110, -250, 2000, 11)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    CALL(evt_item_entry, STRING("item00"), 96, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_item_set_scale, STRING("item00"), FLOAT(0.0f))
    SET_VALUE(GW(11), 0)
    BEGIN_INLINE_EVENT
        CALL(effect_position)
    END_INLINE_EVENT
    WAIT_FRAMES(1)
    IF_INT_EQUAL(GW(15), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_MAP_KAKAGERU2_2"), LW(0), LW(1), LW(2), 0)
    END_IF
    CALL(evt_eff64, STRING("map_wave"), STRING("crystal_n64"), 0, 0, -1000, 0, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_eff64, STRING("map_ball"), STRING("pokopi_pcharge_n64"), 1, 0, -1000, 0, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(200)
    CALL(evt_eff, STRING("map_map"), STRING("treasure_map"), 0, 0, -1000, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
    IF_INT_EQUAL(GW(15), 0)
        CALL(cam_kaiten_init, 0, 65, 40, 0, 65, -250, -24, 102, -800, 0, 110, -250)
        BEGIN_INLINE_EVENT_GET_ID(LW(7))
            LOOP(0)
                CALL(cam_kaiten_set_direct)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LW(10), LF(7))
            SET_FLOAT_VALUE(LW(11), LW(10))
            ADD_FLOAT(LW(11), FLOAT(720.0f))
            SET_VALUE(LW(15), 0)
            LOOP(0)
                CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                CALL(cam_mahojin_kaiten_main, LW(10), LW(11), 14000, LW(6), LW(9))
                SET_FLOAT_VALUE(GW(10), LW(6))
                MULTIPLY_FLOAT(GW(10), FLOAT(-1.0f))
                CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                IF_INT_EQUAL(LW(9), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LF(6), LW(0))
            CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LF(6), LW(1))
            CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
            LOOP(0)
                CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                CALL(evt_sub_intpl_msec_get_value_para, LW(7), LW(9))
                CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                IF_INT_EQUAL(LW(9), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(cam_kaiten_get_para, 0, LF(0), LW(0), LF(2), LF(3), LF(4), LF(5), LF(6), LF(7))
            CALL(cam_kaiten_get_para, 1, LF(0), LW(1), LF(2), LF(3), LF(4), LF(5), LF(6), LF(8))
            CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
            LOOP(0)
                CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                CALL(evt_sub_intpl_msec_get_value_para, LW(1), LW(9))
                CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                IF_INT_EQUAL(LW(9), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LW(0), LF(5), LF(6), LF(7))
            CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LW(1), LF(5), LF(6), LF(8))
            CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
            LOOP(0)
                CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                CALL(evt_sub_intpl_msec_get_value_para, LW(4), LW(9))
                CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
                IF_INT_EQUAL(LW(9), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(2000)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_yuka1"), 1)
            WAIT_MS(3250)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_yuka1"), 1)
            WAIT_MS(3500)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_yuka1"), 1)
            WAIT_MS(2700)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_yuka1"), 1)
        END_INLINE_EVENT
    END_IF
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 4000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(11), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
    IF_INT_EQUAL(GW(15), 0)
        CALL(evt_item_move_3d_position, STRING("item00"), LW(0), 115, LW(2), 14000, 11)
        ELSE
        CALL(evt_item_move_3d_position, STRING("item00"), LW(0), 115, LW(2), 9333, 11)
    END_IF
    CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        ADD(LW(2), 20)
        CALL(evt_eff64, STRING("map_line"), STRING("pokopi_pcharge_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_eff, 0, STRING("laser"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    WAIT_MS(500)
    CALL(evt_fade_entry, 21, 150, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, 20, 150, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(200)
    CALL(evt_fade_entry, 21, 3200, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(map_disp_onoff, 0)
    WAIT_FRAMES(1)
    CALL(evt_paper_entry, STRING("OFF_d_tizu_tex"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_tizu_tex"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 4600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(img_alpha, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    SWITCH(GW(15))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("0_A_1"))
        SET_VALUE(LW(1), STRING("0_A_2"))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("1_A_1"))
        SET_VALUE(LW(1), STRING("1_A_2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("2_A_1"))
        SET_VALUE(LW(1), STRING("2_A_2"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("3_A_1"))
        SET_VALUE(LW(1), STRING("3_A_2"))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("4_A_1"))
        SET_VALUE(LW(1), STRING("4_A_2"))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("5_A_1"))
        SET_VALUE(LW(1), STRING("5_A_2"))
        CASE_EQUAL(6)
        SET_VALUE(LW(0), STRING("6_A_1"))
        SET_VALUE(LW(1), STRING("6_A_2"))
        CASE_EQUAL(7)
        SET_VALUE(LW(0), STRING("7_A_1"))
        SET_VALUE(LW(1), STRING("7_A_2"))
    END_SWITCH
    CALL(evt_img_onoff, STRING("img"), 1)
    LOOP(5)
        CALL(evt_img_set_paper_anim, STRING("img"), LW(0))
        CALL(evt_img_wait_animend, STRING("img"))
    END_LOOP
    CALL(evt_img_set_paper_anim, STRING("img"), LW(1))
    CALL(evt_img_wait_animend, STRING("img"))
    WAIT_MS(1000)
    WAIT_MS(1000)
    SWITCH(GW(15))
        CASE_EQUAL(0)
        SET_VALUE(LW(5), 55)
        SET_VALUE(LW(6), 130)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_1"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG1_APPEAR1"))
        CASE_EQUAL(1)
        SET_VALUE(LW(5), 0)
        SET_VALUE(LW(6), 130)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_2"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG2_APPEAR1"))
        CASE_EQUAL(2)
        SET_VALUE(LW(5), 30)
        SET_VALUE(LW(6), 145)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_3"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG3_APPEAR1"))
        CASE_EQUAL(3)
        SET_VALUE(LW(5), -50)
        SET_VALUE(LW(6), 95)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_4"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG4_APPEAR1"))
        CASE_EQUAL(4)
        SET_VALUE(LW(5), 10)
        SET_VALUE(LW(6), 50)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_5"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG5_APPEAR1"))
        CASE_EQUAL(5)
        SET_VALUE(LW(5), 80)
        SET_VALUE(LW(6), 90)
        SET_VALUE(LW(7), -140)
        SET_VALUE(LW(8), STRING("TIZ_stg_6"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG6_APPEAR1"))
        CASE_EQUAL(6)
        SET_VALUE(LW(5), -71)
        SET_VALUE(LW(6), 145)
        SET_VALUE(LW(7), -138)
        SET_VALUE(LW(8), STRING("TIZ_stg_7"))
        SET_VALUE(LW(9), STRING("SFX_EVT_MAP_STG7_APPEAR1"))
    END_SWITCH
    ADD(LW(6), -10)
    ADD(LW(7), -20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_MAP_NEXT_APPEAR1"), LW(5), LW(6), LW(7), LW(4))
    CALL(evt_snd_sfx_pit, LW(4), 1365)
    CALL(evt_eff64, STRING("tiz_dust2"), STRING("stardust2_n64"), 5, LW(5), LW(6), LW(7), 50, 50, 30, 600, 0, 0, 0, 0)
    CALL(tiz_stone, 0)
    WAIT_MS(500)
    CALL(evt_npc_entry, STRING("tiz_obj"), LW(8))
    ADD(LW(6), 10)
    CALL(evt_npc_set_position, STRING("tiz_obj"), LW(5), LW(6), LW(7))
    CALL(evt_npc_set_scale, STRING("tiz_obj"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.5f))
    CALL(evt_npc_set_camid, STRING("tiz_obj"), 7)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_color, STRING("tiz_obj"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_anim, STRING("tiz_obj"), STRING("A_1"))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxon__, LW(9), 0)
    CALL(evt_npc_wait_anim, STRING("tiz_obj"), FLOAT(1.0f))
    ADD(LW(6), 10)
    ADD(LW(7), 20)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_MAP_NEXT_APPEAR1"), LW(5), LW(6), LW(7), 0)
    CALL(evt_eff64, STRING("tiz_dust"), STRING("stardust_n64"), 0, LW(5), LW(6), LW(7), FLOAT(15.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(tiz_stone, 0)
    WAIT_MS(500)
    CALL(evt_eff, STRING("tiz_stone"), STRING("starstone"), GW(15), LW(5), LW(6), LW(7), FLOAT(0.69921875f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 10000, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(tiz_stone, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_eff_softdelete, STRING("tiz_stone"))
    CALL(evt_npc_delete, STRING("tiz_obj"))
    SWITCH(GW(15))
        CASE_EQUAL(0)
        CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU2_3"), 0)
        CASE_OR_EQUAL(1)
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(4)
        CASE_OR_EQUAL(5)
        CASE_OR_EQUAL(6)
        CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU1_2"), 0)
        CASE_END
    END_SWITCH
    CALL(map_disp_onoff, 1)
    WAIT_FRAMES(1)
    CALL(evt_fade_entry, 20, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("indirect"), 0, LW(0), LW(1), LW(2), 120, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(tiz_broken)
    CALL(evt_eff_softdelete, STRING("map_wave"))
    CALL(evt_eff_softdelete, STRING("map_ball"))
    CALL(evt_eff_softdelete, STRING("map_line"))
    CALL(evt_mario_set_pose, STRING("M_P_2"))
    CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
    ADD(LW(2), -10)
    CALL(evt_eff, STRING("map_itemget"), STRING("itemget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU1_3"), 0)
    CALL(map_bgmode, STRING("map_itemget"), STRING("map_map"))
    WAIT_MS(500)
    SWITCH(GW(15))
        CASE_EQUAL(0)
        CALL(evt_msg_print, 0, STRING("mac_0_099"), 0, 0)
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("mac_1_001_01"), 0, 0)
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("mac_2_001_01"), 0, 0)
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("mac_3_000_01"), 0, 0)
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("mac_4_000_01"), 0, 0)
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("mac_5_000_01"), 0, 0)
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("mac_6_000_01"), 0, 0)
    END_SWITCH
    CALL(map_bgmode_end, STRING("map_itemget"), STRING("map_map"))
    CALL(evt_eff_softdelete, STRING("map_itemget"))
    SET_VALUE(GF(11), 1)
    CALL(evt_eff_softdelete, STRING("mahojin"))
    SPAWN_EVENT(PTR(mahojin_se_event_end))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(GW(11), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(evt_fogreset))
    IF_INT_NOT_EQUAL(GW(15), 0)
        CALL(evt_cam3d_evt_set, 0, 75, 50, 0, 80, -250, 1000, 11)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(1700)
        CALL(evt_mario_set_pose, STRING("M_L_3"))
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_item_move_3d_position, STRING("item00"), LW(0), LW(1), LW(2), 2000, 11)
    CALL(evt_item_delete, STRING("item00"))
    CALL(evt_eff_delete, STRING("map_map"))
    WAIT_MS(500)
    IF_INT_NOT_EQUAL(GW(15), 0)
        CALL(evt_party_jump_pos, 0, 30, 28, -250, 300, 0, -1000)
        WAIT_MS(1000)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(fake_fade)
    CALL(evt_cam3d_evt_set, 0, 225, 469, 0, 100, -270, 2000, 11)
    WAIT_MS(1200)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_set_mapchange_type, 1, 10, 0, 1, 500)
    RETURN
EVT_END()

EVT_BEGIN(evt_starstone_up)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_MAP_KAKAGERU1"), LW(4), LW(5), LW(6), 0)
    CALL(evt_mario_set_pose, STRING("M_P_2"))
    CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
    ADD(LW(5), 60)
    ADD(LW(6), -20)
    CALL(evt_eff, STRING("itemget"), STRING("itemget"), 9, LW(4), LW(5), LW(6), 0, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(0), GW(15))
    SUBTRACT(LW(0), 1)
    ADD(LW(6), 5)
    CALL(evt_eff, STRING("stone"), STRING("starstone"), LW(0), LW(4), LW(5), LW(6), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone"))
    WAIT_MS(1000)
    CALL(evt_eff_softdelete, STRING("itemget"))
    CALL(starstone_get_position, STRING("stone"), LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_softdelete, STRING("stone"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    RETURN
EVT_END()

EVT_BEGIN(evt_starstone_set)
    WAIT_MS(7000)
    SET_VALUE(LW(0), GW(15))
    SUBTRACT(LW(0), 1)
    CALL(evt_eff, STRING("stone"), STRING("starstone"), LW(0), 0, -1000, 0, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(starstone_setting, STRING("stone"), LW(0))
    CALL(eff_starstone_sound_off, STRING("stone"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(starstone_circle_link_func, STRING("stone"), GW(15), LW(0), 50, LW(2), 0, FLOAT(140.0f))
    CALL(starstone_set_alpha, STRING("stone"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(starstone_set_alpha, STRING("stone"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        CALL(starstone_set_ry, STRING("stone"), LW(0))
        ADD_FLOAT(LW(0), FLOAT(4.0f))
        IF_FLOAT_GREATER(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        IF_INT_EQUAL(GF(11), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_eff_softdelete, STRING("stone"))
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event)
    SET_VALUE(GW(15), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    SET_VALUE(GF(10), 1)
    CALL(evt_mario_mov_pos2, 0, -250, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    CALL(evtflash, 20, 300, 80)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 0, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_MAP_KAKAGERU2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_party_set_pos, 1, -30, 19, -190)
    CALL(evt_party_set_pos, 0, 30, 19, -190)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_mario_set_dir, 90, 200, 0)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
            CALL(evt_mario_set_dir, 270, 200, 0)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        LOOP(0)
            CALL(evt_party_set_dir, 0, 90, -1)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
            CALL(evt_party_set_dir, 0, 270, -1)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(12))
        LOOP(0)
            CALL(evt_party_set_dir, 1, 90, -1)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
            CALL(evt_party_set_dir, 1, 270, -1)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    DELETE_EVENT(LW(11))
    DELETE_EVENT(LW(12))
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 1, 90, -1)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_mapwrite))
    BEGIN_INLINE_EVENT
        CALL(evt_party_jump_pos, 0, 30, 28, -250, 300, 0, -1000)
    END_INLINE_EVENT
    CALL(evt_party_jump_pos, 1, -30, 28, -250, 300, 0, -1000)
    CALL(evt_cam3d_evt_set, 0, 100, -2, 0, 65, -250, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_100"))
    CALL(evt_msg_print, 0, STRING("mac_0_101"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, 0, 60, 447, 0, 120, -250, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_0_101_04"), 0, STRING("extparty"))
    CALL(evt_mario_set_pose, STRING("M_Z_1"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("クリハカセ"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_pouch_get_starstone, 0)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_set_mapchange_type, 1, 10, 0, 1, 500)
    SET_VALUE(GSW_STORY_PROGRESS, 18)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event1)
    SET_VALUE(GW(15), 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL(evt_msg_print_party, STRING("mac_1_001"))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 1, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_1_002"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1200)
    CALL(evt_msg_print_party, STRING("mac_1_003"))
    SET_VALUE(GSW_STORY_PROGRESS, 64)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event2)
    SET_VALUE(GW(15), 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL(evt_msg_print_party, STRING("mac_2_001"))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 2, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_2_002"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_Z"))
    SET_VALUE(GSW_STORY_PROGRESS, 120)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event3)
    SET_VALUE(GW(15), 3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 3, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_3_001"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    SET_VALUE(GSW_STORY_PROGRESS, 174)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event4)
    SET_VALUE(GW(15), 4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 4, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_4_001"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    SET_VALUE(GSW_STORY_PROGRESS, 221)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event5)
    SET_VALUE(GW(15), 5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 5, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_5_001"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    SET_VALUE(GSW_STORY_PROGRESS, 271)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event6)
    SET_VALUE(GW(15), 6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(evt_mahojin_move))
    CALL_CHILD(PTR(evt_starstone_up))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 6, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    SET_VALUE(GF(11), 0)
    SPAWN_EVENT(PTR(evt_starstone_set))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL_CHILD(PTR(evt_mahojin_move2))
    SPAWN_EVENT(PTR(evt_fogset))
    WAIT_MS(6000)
    CALL_CHILD(PTR(evt_mapwrite))
    CALL(evt_msg_print_party, STRING("mac_6_001"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    SET_VALUE(GSW_STORY_PROGRESS, 347)
    CALL_CHILD(PTR(fake_fade))
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_bero_mapchange, STRING("gor_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(starstone_circle_link)
    LOOP(0)
        CALL(starstone_circle_link_func, STRING("stone01"), 1, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone02"), 2, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone03"), 3, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone04"), 4, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone05"), 5, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone06"), 6, LW(0), GW(10), LW(2), GW(11), GW(12))
        CALL(starstone_circle_link_func, STRING("stone07"), 7, LW(0), GW(10), LW(2), GW(11), GW(12))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(lastdungeon_anim_init)
    CALL(evt_map_playanim, STRING("anm_hibi"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_1"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_2"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_3"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_4"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_5"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_6"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_hosi_7"), 2, 0)
    CALL(evt_map_playanim, STRING("anm_tobira"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_hibi"), 2, 0)
    RETURN
EVT_END()

EVT_BEGIN(lastdungeon_init)
    CALL(evt_hit_bind_mapobj, STRING("a_door_a"), STRING("door_a"))
    CALL(evt_hit_bind_mapobj, STRING("a_door_b"), STRING("door_b"))
    CALL(evt_mapobj_rotate, 0, STRING("door_b"), 0, 120, 0)
    CALL(evt_mapobj_rotate, 0, STRING("door_a"), 0, -120, 0)
    CALL(evt_hit_bind_update, STRING("a_door_a"))
    CALL(evt_hit_bind_update, STRING("a_door_b"))
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), 8388612)
    RETURN
EVT_END()

EVT_BEGIN(lastdungeon)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(2), -50)
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
        CALL(evt_mario_dispflag_onoff, 1, 1)
        CALL(evt_party_dispflg_onoff, 0, 1, 1)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU3_3"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 5000)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 200)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 120, 0, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("door_b"), 0, LW(0), 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 0, STRING("door_a"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_bero_mapchange, STRING("sys_01"), STRING("stg_8"))
    RETURN
EVT_END()

EVT_BEGIN(mahojin_event7)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
#endif
    CALL(evt_mario_mov_pos2, 0, -250, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(800)
    CALL(evt_cam_letter_box_disable, 0)
    SET_VALUE(GF(10), 1)
    CALL(evt_msg_print, 0, STRING("mac_7_009"), 0, STRING("クリハカセ"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU3_1"), 0)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 8, 0, 28, -250, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT(PTR(mahojin_se_event))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL(evt_party_set_pos, 0, 30, 25, -190)
    CALL(evt_party_set_dir, 0, 270, 0)
    WAIT_MS(2000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING("map_wave"), STRING("crystal_n64"), 6, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
    CALL(crystal_bgmode, STRING("map_wave"))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 65, 40, 0, 65, -250, 1000, 11)
    WAIT_MS(2000)
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING("charge"), STRING("pokopi_pcharge_n64"), 8, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("stone01"), STRING("starstone"), 0, 0, 50, -390, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone01"))
    CALL(evt_eff, STRING("stone02"), STRING("starstone"), 1, 112, 50, -340, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone02"))
    CALL(evt_eff, STRING("stone03"), STRING("starstone"), 2, 140, 50, -220, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone03"))
    CALL(evt_eff, STRING("stone04"), STRING("starstone"), 3, 62, 50, -120, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone04"))
    CALL(evt_eff, STRING("stone05"), STRING("starstone"), 4, -62, 50, -120, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone05"))
    CALL(evt_eff, STRING("stone06"), STRING("starstone"), 5, -140, 50, -220, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone06"))
    CALL(evt_eff, STRING("stone07"), STRING("starstone"), 6, -112, 50, -340, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(eff_starstone_sound_off, STRING("stone07"))
    CALL(evt_mario_get_pos, 0, LW(0), GW(10), LW(2))
    SET_FLOAT_VALUE(GW(11), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(12), FLOAT(40.0f))
    SPAWN_EVENT_GET_ID(PTR(starstone_circle_link), LW(8))
    CALL(starstone_set_alpha, STRING("stone01"), 0)
    CALL(starstone_set_alpha, STRING("stone02"), 0)
    CALL(starstone_set_alpha, STRING("stone03"), 0)
    CALL(starstone_set_alpha, STRING("stone04"), 0)
    CALL(starstone_set_alpha, STRING("stone05"), 0)
    CALL(starstone_set_alpha, STRING("stone06"), 0)
    CALL(starstone_set_alpha, STRING("stone07"), 0)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("stone01"), LW(0))
            CALL(starstone_set_ry, STRING("stone02"), LW(0))
            CALL(starstone_set_ry, STRING("stone03"), LW(0))
            CALL(starstone_set_ry, STRING("stone04"), LW(0))
            CALL(starstone_set_ry, STRING("stone05"), LW(0))
            CALL(starstone_set_ry, STRING("stone06"), LW(0))
            CALL(starstone_set_ry, STRING("stone07"), LW(0))
            ADD_FLOAT(LW(0), FLOAT(2.0f))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5500)
        SET_VALUE(LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            IF_INT_GREATER_OR_EQUAL(LW(0), 128)
                IF_INT_EQUAL(LW(2), 0)
                    CALL(evt_mario_set_pose, STRING("M_P_2"))
                    SET_VALUE(LW(2), 1)
                END_IF
            END_IF
            CALL(starstone_set_alpha, STRING("stone01"), LW(0))
            CALL(starstone_set_alpha, STRING("stone02"), LW(0))
            CALL(starstone_set_alpha, STRING("stone03"), LW(0))
            CALL(starstone_set_alpha, STRING("stone04"), LW(0))
            CALL(starstone_set_alpha, STRING("stone05"), LW(0))
            CALL(starstone_set_alpha, STRING("stone06"), LW(0))
            CALL(starstone_set_alpha, STRING("stone07"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(cam_mahojin_kaiten_main, 1080, 0, 14000, GW(11), LW(9))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        CALL(evt_sub_intpl_msec_init, 11, 45, 115, 14000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(10), LW(1))
            CALL(pcharge_set_position, STRING("charge"), LW(4), GW(10), LW(6))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_eff_softdelete, STRING("charge"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(7500)
        CALL(evt_sub_intpl_msec_init, 11, 40, 140, 4500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(12), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(cam_kaiten_init, 0, 65, 40, 0, 65, -250, 0, 102, 300, 0, 110, -250)
    BEGIN_INLINE_EVENT_GET_ID(LW(7))
        LOOP(0)
            CALL(cam_kaiten_set_direct)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LW(10), LF(7))
        CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LW(11), LF(7))
        ADD_FLOAT(LW(11), FLOAT(720.0f))
        SET_VALUE(LW(15), 0)
        LOOP(0)
            CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            CALL(cam_mahojin_kaiten_main, LW(10), LW(11), 14000, LW(6), LW(9))
            CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LF(6), LW(0))
        CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LF(6), LW(1))
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
        LOOP(0)
            CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            CALL(evt_sub_intpl_msec_get_value_para, LW(7), LW(9))
            CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_kaiten_get_para, 0, LF(0), LW(0), LF(2), LF(3), LF(4), LF(5), LF(6), LF(7))
        CALL(cam_kaiten_get_para, 1, LF(0), LW(1), LF(2), LF(3), LF(4), LF(5), LF(6), LF(8))
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
        LOOP(0)
            CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            CALL(evt_sub_intpl_msec_get_value_para, LW(1), LW(9))
            CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LW(0), LF(5), LF(6), LF(7))
        CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LW(1), LF(5), LF(6), LF(8))
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), 14000)
        LOOP(0)
            CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            CALL(evt_sub_intpl_msec_get_value_para, LW(4), LW(9))
            CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_yuka1"), 1)
        WAIT_MS(3250)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_yuka1"), 1)
        WAIT_MS(3500)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_yuka1"), 1)
        WAIT_MS(2700)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_yuka1"), 1)
    END_INLINE_EVENT
    WAIT_MS(14000)
    CALL(evt_mario_dispflag_onoff, 0, 4)
    DELETE_EVENT(LW(7))
    DELETE_EVENT(LW(8))
    WAIT_MS(1000)
    CALL(starstone_get_position, STRING("stone01"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone01"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone01"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone02"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone02"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone02"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone03"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone03"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone03"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone04"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone04"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone04"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone05"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone05"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone05"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone06"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone06"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone06"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(650)
    CALL(starstone_get_position, STRING("stone07"), LW(0), LW(1), LW(2))
    CALL(starstone_set_position, STRING("stone07"), 0, -1000, 0)
    CALL(starstone_set_alpha, STRING("stone07"), 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1300)
    CALL(starstone_setting, STRING("stone01"), 0)
    CALL(starstone_setting, STRING("stone02"), 1)
    CALL(starstone_setting, STRING("stone03"), 2)
    CALL(starstone_setting, STRING("stone04"), 3)
    CALL(starstone_setting, STRING("stone05"), 4)
    CALL(starstone_setting, STRING("stone06"), 5)
    CALL(starstone_setting, STRING("stone07"), 6)
    CALL(evt_cam3d_evt_set, 0, 634, 421, 0, 58, -270, 0, 11)
    CALL(evt_mario_set_pose, STRING("M_I_1"))
    CALL(evt_mario_get_pos, 0, LW(0), GW(10), LW(2))
    SET_FLOAT_VALUE(GW(10), FLOAT(50.0f))
    SET_FLOAT_VALUE(GW(11), FLOAT(0.0f))
    SPAWN_EVENT_GET_ID(PTR(starstone_circle_link), LW(8))
    WAIT_MS(1000)
    CALL(starstone_get_position, STRING("stone01"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone01"), 255)
    CALL(evt_eff64, STRING("sd_1_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_1_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_1_1"), STRING("sd_1_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone02"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone02"), 255)
    CALL(evt_eff64, STRING("sd_2_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_2_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_2_1"), STRING("sd_2_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone03"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone03"), 255)
    CALL(evt_eff64, STRING("sd_3_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_3_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_3_1"), STRING("sd_3_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone04"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone04"), 255)
    CALL(evt_eff64, STRING("sd_4_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_4_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_4_1"), STRING("sd_4_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone05"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone05"), 255)
    CALL(evt_eff64, STRING("sd_5_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_5_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_5_1"), STRING("sd_5_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone06"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone06"), 255)
    CALL(evt_eff64, STRING("sd_6_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_6_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_6_1"), STRING("sd_6_2"))
    WAIT_MS(500)
    CALL(starstone_get_position, STRING("stone07"), LW(0), LW(1), LW(2))
    CALL(starstone_set_alpha, STRING("stone07"), 255)
    CALL(evt_eff64, STRING("sd_7_1"), STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("sd_7_2"), STRING("stardust_n64"), 2, LW(0), LW(1), LW(2), FLOAT(30.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(stardust_setting, STRING("sd_7_1"), STRING("sd_7_2"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 65, 40, 0, 65, -250, 0, 11)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(1000)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0146484375f), 130)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0146484375f), 130)
    CALL(evt_msg_print_party, STRING("mac_7_010"))
    CALL(evt_cam3d_evt_set, -55, 62, -375, -55, 56, -575, 500, 11)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_MAP_KAKAGERU3_2"), 0)
    CALL(evt_map_playanim, STRING("anm_hosi_1"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_1"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -55, 115, -375, -55, 118, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_2"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, -43, 166, -375, -43, 169, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_3"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_3"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 0, 191, -375, 0, 194, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_4"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_4"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 43, 166, -375, 43, 169, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_5"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_5"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 55, 115, -375, 55, 118, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_6"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_6"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 55, 62, -375, 55, 65, -575, 250, 11)
    WAIT_MS(250)
    CALL(evt_map_playanim, STRING("anm_hosi_7"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_hosi_7"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 125, 0, 0, 130, -400, 500, 11)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 125, 0, 0, 130, -400, 500, 11)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING("anm_tobira"), 1, 0)
    BEGIN_INLINE_EVENT
        LOOP(2)
            CALL(evt_cam3d_evt_set, 0, 123, 99, 0, 130, -400, 100, 11)
            WAIT_MS(100)
            CALL(evt_cam3d_evt_set, 0, 125, 0, 0, 130, -400, 100, 11)
            WAIT_MS(1983)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 1, 400)
        WAIT_MS(1683)
        CALL(evt_env_blur_on, 1, 400)
        WAIT_MS(1683)
    END_INLINE_EVENT
    WAIT_MS(4166)
    CALL(evt_cam3d_evt_set, 0, 75, 50, 0, 80, -250, 500, 11)
    CALL(evt_mario_set_reardir)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0146484375f), 333)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_hibi"), 0, 0)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_eff, STRING("kemuri00"), STRING("mist2"), 0, 50, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, STRING("kemuri01"), STRING("mist2"), 0, -50, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(kemuri_set_color, STRING("kemuri00"), 70, 0, 50)
        CALL(kemuri_set_color, STRING("kemuri01"), 70, 0, 50)
        CALL(evt_sub_intpl_msec_init, 0, -580, -520, 3500)
        WAIT_MS(500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(kemuri_set_position, STRING("kemuri00"), 50, 40, LW(0))
            CALL(kemuri_set_position, STRING("kemuri01"), -50, 40, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_eff_softdelete, STRING("kemuri00"))
        CALL(evt_eff_softdelete, STRING("kemuri01"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_eff, STRING("kemuri02"), STRING("mist2"), 0, 90, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, STRING("kemuri03"), STRING("mist2"), 0, -90, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(kemuri_set_color, STRING("kemuri02"), 70, 0, 50)
        CALL(kemuri_set_color, STRING("kemuri03"), 70, 0, 50)
        CALL(evt_sub_intpl_msec_init, 0, -580, -560, 3500)
        WAIT_MS(500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(kemuri_set_position, STRING("kemuri02"), 90, 40, LW(0))
            CALL(kemuri_set_position, STRING("kemuri03"), -90, 40, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_eff_softdelete, STRING("kemuri02"))
        CALL(evt_eff_softdelete, STRING("kemuri03"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_eff, STRING("kemuri04"), STRING("mist2"), 0, 110, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, STRING("kemuri05"), STRING("mist2"), 0, -110, 40, -580, 20, 10, 0, 0, 0, 0, 0, 0)
        CALL(kemuri_set_color, STRING("kemuri04"), 70, 0, 50)
        CALL(kemuri_set_color, STRING("kemuri05"), 70, 0, 50)
        WAIT_MS(500)
        WAIT_MS(3500)
        CALL(evt_eff_softdelete, STRING("kemuri04"))
        CALL(evt_eff_softdelete, STRING("kemuri05"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_eff, STRING("las_mon"), STRING("las_mon"), 0, 0, 20, -560, 0, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 120, 8000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("door_b"), 0, LW(0), 0)
            MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
            CALL(evt_mapobj_rotate, 0, STRING("door_a"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 8000)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(8000)
        CALL(evt_sub_intpl_msec_init, 0, 255, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(mahojin_effect_alpha, STRING("mahojin"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_eff_softdelete, STRING("mahojin"))
        CALL(evt_eff_softdelete, STRING("las_mon"))
        SPAWN_EVENT(PTR(mahojin_se_event_end))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(8000)
        SET_VALUE(LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(starstone_set_alpha, STRING("stone01"), LW(0))
            CALL(starstone_set_alpha, STRING("stone02"), LW(0))
            CALL(starstone_set_alpha, STRING("stone03"), LW(0))
            CALL(starstone_set_alpha, STRING("stone04"), LW(0))
            CALL(starstone_set_alpha, STRING("stone05"), LW(0))
            CALL(starstone_set_alpha, STRING("stone06"), LW(0))
            CALL(starstone_set_alpha, STRING("stone07"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(8))
        CALL(starstone_set_position, STRING("stone01"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone02"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone03"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone04"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone05"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone06"), 0, -1000, 0)
        CALL(starstone_set_position, STRING("stone07"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -625, 550, 370, 0, 90, -545, 0, 11)
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 465, -5, -150, 18, 135, -525, 0, 11)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 102, 300, 0, 110, -250, 0, 11)
    WAIT_MS(4100)
    CALL(evt_cam3d_evt_set, 0, 60, 75, 0, 65, -250, 500, 11)
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("mac_7_011"), 0, STRING("クリハカセ"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, 0, 73, 75, 0, 78, -250, 500, 11)
    CALL(evt_party_jump_pos, 0, 30, 40, -230, 300, 0, -1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_msg_print_party, STRING("mac_7_013"))
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_cam3d_evt_off, 500, 11)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 300, 0, -1000)
    SET_VALUE(GSW_STORY_PROGRESS, 380)
    CALL(evt_party_run, 0)
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(lastdungeon_init))
    CALL(evt_run_case_evt, 10, 1, STRING("n_bero"), 0, PTR(lastdungeon), 0)
    SET_VALUE(GF(10), 0)
    SPAWN_EVENT(PTR(mahojin_alpha))
    CALL(evt_exit_case_evt)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_TIK2"), 3000)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(tik_05_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_TIK2"), 3000)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mapobj_trans, 1, STRING("hosi_1"), 0, -5, 0)
    CALL(alpha_init_func)
    SET_VALUE(GF(10), 0)
    SPAWN_EVENT(PTR(mahojin_alpha))
    CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_alpha_up), 0)
    CALL(evt_run_case_evt, 1, 1, STRING("a_dai"), 0, PTR(mahojin_alpha_down), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_doa_l"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(15, 17)
        SPAWN_EVENT(PTR(kaidan_event))
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event), 0)
        CASE_EQUAL(63)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event1), 0)
        CASE_EQUAL(119)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event2), 0)
        CASE_EQUAL(173)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event3), 0)
        CASE_EQUAL(220)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event4), 0)
        CASE_EQUAL(270)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event5), 0)
        CASE_EQUAL(346)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event6), 0)
        CASE_BETWEEN(378, 379)
        CALL(evt_run_case_evt, 0, 1, STRING("a_dai"), 0, PTR(mahojin_event7), 0)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(15)
        SET_VALUE(LW(0), STRING("n_bero"))
#ifdef VERSION_G8ME01
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
        CALL_CHILD(PTR(large_door_stg0_init))
        CASE_LESS(380)
        SET_VALUE(LW(0), STRING("n_bero"))
#ifdef VERSION_G8ME01
        CALL_CHILD(bero_case_switch_off)
        CASE_EQUAL(380)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
        CASE_EQUAL(380)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
        SPAWN_EVENT(PTR(lastdungeon_anim_init))
        SPAWN_EVENT(PTR(lastdungeon_init))
        CALL(evt_run_case_evt, 10, 1, STRING("n_bero"), 0, PTR(lastdungeon), 0)
        CASE_GREATER(380)
        SPAWN_EVENT(PTR(lastdungeon_anim_init))
    END_SWITCH
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou1"), STRING("hikou1_a"))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou2"), STRING("hikou2_a"))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -580, 380, -225, 0, GSWF(5532), 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 250, 550, 80, -125, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump01"), 250, -550, 80, -125, 0, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -350, 60, 175, 0, 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 0, 0, 400, STRING("jimen"), 125, GSWF(5592))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hasira"), 2)
    CALL(evt_sub_animgroup_async, STRING("MOBJ_EFF_las_mon"))
    CALL(evt_sub_animgroup_async, STRING("EFF_treasure_map"))
    CALL(evt_sub_animgroup_async, STRING("MOBJ_EFF_mahojin"))
    CALL(evt_sub_animgroup_async, STRING("MOBJ_EFF_laser"))
    CALL(evt_sub_animgroup_async, STRING("OFF_d_tizu_tex"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_1"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_2"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_3"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_4"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_5"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_6"))
    CALL(evt_sub_animgroup_async, STRING("TIZ_stg_7"))
#ifndef VERSION_G8ME01
    CALL(is_dol_viewer)
#endif
    RETURN
EVT_END()

