// aji_17 (敵のアジト, X-Naut Fortress): TEC's room: Peach's talks with TEC, the e-mails to Mario, the dance
// (peach_evt_dance), and TEC's shutdown (evt_emergency, evt_last).

#include "version.h"
#include "driver/fadedrv.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "pmario_sound.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include <dolphin/pad.h>

#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "mario/mario.h"
#include "string.h"
#include "eff/eff_peach_fukidashi.h"
#include "manager/evtmgr.h"
#include "driver/effdrv.h"
#include "system.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "dolphin/gx.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "aji.h"
#include "driver/seqdrv.h"

typedef struct DanceData {
    u16 flags;
    u8 step;
    u8 movePos;
    f32 cx;
    f32 cy;
    f32 rot;
} DanceData;
typedef struct HorogramWork {
    void* buf0;
    void* buf1;
    GXTexObj tex0;
    GXTexObj tex1;
    u16 ready;
    u16 flags;
} HorogramWork;
HorogramWork* horogram_wp;
DanceData dance_dt;

static s32 bgm_start_wait(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(bgm_start_wait);
#if VERSION == VERSION_EU
static USER_FUNC(wait_move_end_50hz);
#endif
s32 btn_check(EventEntry* event, BOOL isFirstCall);
s32 compute_horogram_pos(EventEntry* event, BOOL isFirstCall);
s32 compute_move_pos(EventEntry* event, BOOL isFirstCall);
s32 compute_peach_pos(EventEntry* event, BOOL isFirstCall);
s32 dance_data_init(EventEntry* event, BOOL isFirstCall);
s32 dance_flag_check(EventEntry* event, BOOL isFirstCall);
s32 dance_flag_onoff(EventEntry* event, BOOL isFirstCall);
s32 dance_get_step(EventEntry* event, BOOL isFirstCall);
s32 dance_reverse(EventEntry* event, BOOL isFirstCall);
s32 disp_horogram_evt(EventEntry* event, BOOL isFirstCall);
s32 evt_fuki_btn(EventEntry* event, BOOL isFirstCall);
s32 evt_fuki_btn_set_batu(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_dispflag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_shuryolight_init(EventEntry* event, BOOL isFirstCall);
s32 evt_shuryolight_run_evt(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_envoff_f(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_envon_f(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfx_all_stop(EventEntry* event, BOOL isFirstCall);
s32 get_center(EventEntry* event, BOOL isFirstCall);
s32 get_local_rotate(EventEntry* event, BOOL isFirstCall);
s32 horogram_cap_evt(EventEntry* event, BOOL isFirstCall);
s32 horogram_flag_onoff(EventEntry* event, BOOL isFirstCall);
s32 horogram_init(EventEntry* event, BOOL isFirstCall);
s32 horogram_set_pose(EventEntry* event, BOOL isFirstCall);
s32 move_center(EventEntry* event, BOOL isFirstCall);
s32 noise(EventEntry* event, BOOL isFirstCall);
s32 peach_dance_after_fail(EventEntry* event, BOOL isFirstCall);
static s32 peach_set_pose(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(peach_set_pose);
s32 set_local_rotate(EventEntry* event, BOOL isFirstCall);
s32 set_npc_pos(EventEntry* event, BOOL isFirstCall);
static s32 viewport(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(viewport);
static void polygon(f32 x, f32 y, f32 w, f32 h, f32 s, f32 t);
#if VERSION >= VERSION_US
// US: map save blocks call the US save wrapper script
#define SAVE_EVT evt_memcard_save_wrapper
#else
#define SAVE_EVT evt_memcard_save
#endif

#if VERSION <= VERSION_DEMO_US
static EvtWord w_door_close[];
#else
static EvtWord w_door_close[22];
#endif
#if VERSION <= VERSION_DEMO_US
static EvtWord w_door_open[];
#else
static EvtWord w_door_open[22];
#endif
static BeroEntry bero_entry_data[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_emergency[];
#else
EvtWord evt_emergency[48];
#endif
EvtWord evt_last[728];
EvtWord evt_last2[339];
EvtWord evt_camera[68];
EvtWord mail_sousin_se[36];
static EvtWord peach_evt_stg1[1131];
f32 move_pos_list[15][2];
EvtWord peach_dance_move[95];
EvtWord peach_dance_pose[44];
EvtWord peach_dance_action_step[2];
EvtWord dance_action_turn_sub[108];
EvtWord peach_dance_action_turn[12];
EvtWord peach_dance_action_reverse[18];
EvtWord peach_dance_action_fail[28];
EvtWord dance_btn_disp_sub[49];
EvtWord dance_btn_disp[25];
EvtWord dance_btn_batu[20];
EvtWord dance_btn_clear[56];
EvtWord dance_btn_clear_all[47];
EvtWord wait_action_time[16];
EvtWord peach_dance_dark[27];
EvtWord peach_dance_light[27];
EvtWord peach_evt_dance[];
static EvtWord peach_evt_stg2[900];
static EvtWord peach_evt_stg3[352];
static EvtWord peach_evt_stg3_1[650];
EvtWord peach_evt_stg4_quiz_data[5][4];
static EvtWord peach_evt_stg4[980];
static EvtWord peach_evt_stg5[408];
static EvtWord peach_evt_stg5_1[528];
static EvtWord peach_evt_stg6[1467];
static EvtWord teshita1_init[9];
static EvtWord teshita2_init[9];
static EvtWord syuryo_init[9];
static NpcSetupInfo npcEnt[4];
EvtWord tec_talk[36];
EvtWord aji_17_init_evt[VERSION_PICK(253, 255)];

#pragma defer_codegen off
/**
 * Script function: returns row args[0] of the quiz table peach_evt_stg4_quiz_data in args[1] to args[4] (the
 * four values of one question).
 */
s32 peach_evt_stg4_read_quiz_data(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 no = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], peach_evt_stg4_quiz_data[no][0]);
    evtSetValue(event, args[2], peach_evt_stg4_quiz_data[no][1]);
    evtSetValue(event, args[3], peach_evt_stg4_quiz_data[no][2]);
    evtSetValue(event, args[4], peach_evt_stg4_quiz_data[no][3]);
    return EVT_RETURN_DONE;
}


/**
 * Script function: waits until the channel 1 background music has actually started (psndBGMStartCheck),
 * so the dance can be timed to the waltz.
 */
static s32 bgm_start_wait(EventEntry* event, BOOL isFirstCall) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

#if VERSION == VERSION_EU
/** PAL: at 50 Hz the next dance move waits for the previous one to end (dance_dt flag 4). */
static s32 wait_move_end_50hz(EventEntry* event, BOOL isFirstCall) {
    if (gp->framerate != 50) {
        return EVT_RETURN_DONE;
    }
    return (dance_dt.flags & 4) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

#endif
#pragma defer_codegen on

static EVT_BEGIN(w_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING(("S_door1_1")), 0, 0, LW(0))
#endif
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
#else
    CALL(evt_mapobj_trans, 0, STRING(("S_door1_2")), 0, 0, LW(0))
#endif
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING(("S_door1_1")), 0, 0, LW(0))
#endif
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
#else
    CALL(evt_mapobj_trans, 0, STRING(("S_door1_2")), 0, 0, LW(0))
#endif
    RETURN
EVT_END()

/** Exit of this room: the west door to aji_01. */
static BeroEntry bero_entry_data[] = {
    {
        ("w_bero"), // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        ("e_bero_5"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};
/** Script function: sets the viewport of the 3D and 2D cameras to x, y, width and height from args[0] to args[3]. */
static s32 viewport(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 w = evtGetFloat(event, args[2]);
    f32 h = evtGetFloat(event, args[3]);
    CameraEntry* cam;

    cam = camGetPtr(CAMERA_3D);
    cam->viewport[0] = x;
    cam->viewport[1] = y;
    cam->viewport[2] = w;
    cam->viewport[3] = h;
    cam = camGetPtr(CAMERA_2D);
    cam->viewport[0] = x;
    cam->viewport[1] = y;
    cam->viewport[2] = w;
    cam->viewport[3] = h;
    return EVT_RETURN_DONE;
}

/**
 * Draw callback of the TV-noise effect used in TEC's last scene:
 * fills a 64 by 64 texture with random horizontal streaks
 * and draws it over the screen with the alpha passed as the callback parameter.
 */
void noiseDisp(CameraId cameraId, void* param) {
    s32 x;
    s32 ry;
    s32 y;
    s32 by;
    s32 off;
    s32 w;
    s32 i;
    CameraEntry* cam;
    s32 len;
    u8 col;
    GXTexObj tex;
    s32 n;
    u8* buf;
    Mtx mtx;
    s32 start;
    cam = camGetPtr(cameraId);
    buf = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_I8, GX_FALSE, 0), 3)->address;
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            by = y / 4;
            ry = y % 4;
            off = x / 8;
            w = x % 8;
            buf[((off << 5) + (by << 8)) + (w + ry * 8)] = (y & 1) ? 0xFF : 0;
        }
        n = rand() % 100 + 100;
        for (i = 0; i < n; i++) {
            len = rand() % 4 + 2;
            start = rand() % (65 - len);
            col = rand() % 255;
            for (x = start; x < start + len; x++) {
                by = y / 4;
                ry = y % 4;
                off = x / 8;
                w = x % 8;
                buf[((off << 5) + (by << 8)) + (w + ry * 8)] = col;
            }
        }
    }
    DCFlushRange(buf, 0x1000);
    GXInitTexObj(&tex, buf, 64, 64, GX_TF_I8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(mtx, 9.5f, 7.5f, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, (u8)(EvtWord)param});
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-gp->fbWidth / 2, gp->efbHeight / 2, 0);
    GXTexCoord2s16(0, 1);
    GXPosition3s16(gp->fbWidth / 2, gp->efbHeight / 2, 0);
    GXTexCoord2s16(1, 1);
    GXPosition3s16(gp->fbWidth / 2, -gp->efbHeight / 2, 0);
    GXTexCoord2s16(1, 0);
    GXPosition3s16(-gp->fbWidth, -gp->efbHeight / 2, 0);
    GXTexCoord2s16(0, 0);
}

EVT_BEGIN(evt_emergency)
#if VERSION <= VERSION_DEMO_US
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG7_25_THE_LAST_MAIL)
#else
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG7_25_THE_LAST_MAIL)
#endif
        LOOP(0)
#if VERSION <= VERSION_DEMO_US
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_25_THE_LAST_MAIL)
#else
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_25_THE_LAST_MAIL)
#endif
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
#if VERSION <= VERSION_DEMO_US
CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 32, 1000)
#else
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
#endif
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, 255, LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_map_set_blend, 0, 255, 32, 32, 255)
#else
    CALL(evt_map_set_blend, 0, 255, 96, 96, 255)
#endif
    RETURN
EVT_END()

/**
 * First scene when the party returns to TEC's room at STG7_25_00_1_BOWSER_POSHLEY_SANCTUM:
 * TEC's lights flicker, the party reacts (stg7_aji_46),
 * and TEC speaks over its screen with noise and fades (stg7_aji_47 to 57)
 * before the map changes to aji_16 entered as "peach" for the Last Mail scene.
 * It is TEC's farewell: it introduces itself, asks Mario to save Peach from Grodus in the Palace of Shadow,
 * tells him to use the teleporter while backup power lasts, and says it will cease to exist.
 */
EVT_BEGIN(evt_last)
    // The party enters TEC's room; the music stops and the lights flicker.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_party_stop, 0)
    WAIT_MS(800)
    // Mario and his partner react to the flickering.
    CALL(evt_msg_print_party, STRING(("stg7_aji_46")))
    WAIT_MS(300)
    LOOP(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FIND1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("eff0")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(("eff1")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_TEC_ACTIVE1")), 0, 0, -120, 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG7_TEC_ACTIVE1")), 0, 0, -100, 0)
        WAIT_MS(800)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING(("moni_1")), 0, 0)
    CALL(evt_map_checkanim, STRING(("moni_1")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    // TEC speaks haltingly over its screen, which flickers with noise.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_47")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    CALL(evt_cam3d_evt_set, 0, 100, -170, 0, 40, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_fade_tec_onoff, 1, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 20, 20, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -20, 20, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC introduces itself as the fortress's main computer.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_48")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING(("SFX_STG7_TEC_SANDSTORM1")), LW(10))
    CALL(evt_snd_sfx_vol, LW(10), 0)
    SET_VALUE(LF(0), 0)
    LOOP(2)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 128, 300)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(noise, LW(0))
        IF_INT_GREATER(LW(0), 127)
            SET_VALUE(LW(0), 127)
        END_IF
        CALL(evt_snd_sfx_vol, LW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            SET_VALUE(LF(0), 1)
            CALL(evt_mario_set_pose, STRING(("M_I_2")))
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC apologizes: most of its data was erased.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_49")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 152, 565, 0, 57, 24, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_reardir)
    CALL(evt_fade_tec_onoff, 0, 0)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC tells Mario that Peach is no longer here.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_50")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 100, -170, 0, 40, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_mario_set_reardir)
    CALL(evt_fade_tec_onoff, 1, 0)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC begs Mario to save Peach.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_51")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING(("SFX_STG7_TEC_SANDSTORM1")), LW(10))
    CALL(evt_snd_sfx_vol, LW(10), 0)
    LOOP(2)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 300)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(noise, LW(0))
        IF_INT_GREATER(LW(0), 127)
            SET_VALUE(LW(0), 127)
        END_IF
        CALL(evt_snd_sfx_vol, LW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 2000)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(noise, LW(0))
    IF_INT_GREATER(LW(0), 127)
        SET_VALUE(LW(0), 127)
    END_IF
    CALL(evt_snd_sfx_vol, LW(10), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 128, 300)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(noise, LW(0))
    IF_INT_GREATER(LW(0), 127)
        SET_VALUE(LW(0), 127)
    END_IF
    CALL(evt_snd_sfx_vol, LW(10), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 2000)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(noise, 255)
    CALL(evt_snd_sfx_vol, LW(10), 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC warns that its backup power is failing.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_52")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING(("stg7_aji_53")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // It urges Mario to hurry to the teleporter while the power lasts.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_54")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 152, 565, 0, 57, 24, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING(("M_I_N")))
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_reardir)
    CALL(evt_fade_tec_onoff, 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING(("stg7_aji_55")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 100, -170, 0, 40, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_fade_tec_onoff, 1, 0)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC says it will cease to exist when the power is gone.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_56")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 152, 565, 0, 57, 24, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_reardir)
    CALL(evt_fade_tec_onoff, 0, 0)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC tells Mario not to worry about it, and the map changes to aji_16 for the Last Mail scene.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_57")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_bero_mapchange, STRING(("aji_16")), STRING(("peach")))
    RETURN
EVT_END()

/**
 * Second scene in TEC's room, after the Last Mail was shown in aji_16:
 * the party reacts to the machine (stg7_aji_59 to 62), the screen mosaic plays,
 * the emergency lights start (evt_emergency) and the story becomes STG7_25_THE_LAST_MAIL
 * before control returns. The partner follows once Mario moves.
 */
EVT_BEGIN(evt_last2)
    // The party is back in TEC's room after the Last Mail; the music stops.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_set_pos, 20, 0, 20)
    CALL(evt_party_set_pos, 0, -20, 0, 20)
    CALL(evt_cam3d_evt_set, 0, 100, -170, 0, 40, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_mario_set_reardir)
    CALL(evt_fade_softfocus_onoff, 0)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    // Mario and his partner react to the machine (stg7_aji_59 to 62).
    CALL(evt_msg_print_party, STRING(("stg7_aji_59")))
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    // TEC replies, then its screen shrinks away as the machine fails.
    CALL(evt_msg_print, 0, STRING(("stg7_aji_60")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(300)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 2000)
        LABEL(10)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(noise, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(10)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG7_TEC_SHUTDOWN1")), 0, 0, -100, 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 239, 200)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(LW(2), 480)
    SUBTRACT(LW(2), LW(0))
    SUBTRACT(LW(2), LW(0))
    CALL(viewport, 0, LW(0), 608, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 303, 300)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(LW(2), 608)
    SUBTRACT(LW(2), LW(0))
    SUBTRACT(LW(2), LW(0))
    CALL(viewport, LW(0), 240, LW(2), 1)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(viewport, 0, 0, 1, 1)
    CALL(evt_fade_tec_onoff, 0, 0)
    WAIT_MS(2000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_set_pos, 20, 0, 35)
    CALL(evt_party_set_pos, 0, -30, 0, 35)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_cam3d_evt_set, 0, 152, 565, 0, 57, 24, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(viewport, 0, 0, 608, 480)
    WAIT_MS(300)
    // The emergency lights come on, a voice announces the system failure and the story advances to the Last
    // Mail.
    SPAWN_EVENT(PTR(evt_emergency))
    CALL(evt_msg_print, 0, STRING(("stg7_aji_61")), 0, 0)
    WAIT_MS(300)
    SET_VALUE(GSW_STORY_PROGRESS, STG7_25_THE_LAST_MAIL)
    CALL(evt_cam3d_evt_set, 0, 92, 320, 0, 40, 24, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING(("stg7_aji_62")))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_NOD1_4")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_N_2")))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        IF_FLOAT_NOT_EQUAL(LW(0), LW(3))
            BREAK_LOOP
        END_IF
        IF_FLOAT_NOT_EQUAL(LW(1), LW(4))
            BREAK_LOOP
        END_IF
        IF_FLOAT_NOT_EQUAL(LW(2), LW(5))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Camera sweep for the TEC room: slides the camera between two positions given in LW(0) to LW(2) over two
 * seconds, back and forth, forever.
 */
EVT_BEGIN(evt_camera)
    SET_VALUE(LW(5), LW(0))
    SET_VALUE(LW(6), LW(1))
    SET_VALUE(LW(7), LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(6), LW(7), 2000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, LW(5), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        WAIT_MS(1000)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(7), LW(6), 2000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, LW(5), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

/**
 * Plays the mail-sent sound effects of the message animation:
 * SFX_PEACH_MAIL_SOUSIN1 three times and SFX_PEACH_MAIL_SOUSIN2 at the end, in 600, 600 and 900 ms steps.
 */
EVT_BEGIN(mail_sousin_se)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_MAIL_SOUSIN1")), 0, 0, -100, 0)
    WAIT_MS(600)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_MAIL_SOUSIN1")), 0, 0, -100, 0)
    WAIT_MS(600)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_MAIL_SOUSIN1")), 0, 0, -100, 0)
    WAIT_MS(900)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_MAIL_SOUSIN2")), 0, 0, -100, 0)
    RETURN
EVT_END()

/**
 * Peach's first meeting with TEC (chapter 1): doors open and close with sound,
 * dialogue peach_aji_42 to 48 plays, the story progress becomes STG1_19_PEACH_TEC
 * and the map changes to kpa_00 with the story-change fade.
 */
static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_mario_set_mode, 1)
    // Peach is brought to TEC's room through the west door.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -230, 0, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -140, 0, 60)
    WAIT_MS(800)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    WAIT_MS(500)
    LOOP(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("eff0")), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(peach_set_pose, STRING(("P_S_4")))
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_TEC_ACTIVE1")), 0, 0, -120, 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG7_TEC_ACTIVE1")), 0, 0, -100, 0)
        WAIT_MS(800)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING(("moni_1")), 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    WAIT_MS(300)
    // An unseen voice greets her: TEC speaks for the first time (peach_aji_42 to 45).
    CALL(evt_msg_print, 0, STRING(("peach_aji_42")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_43")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 450, 0, 105, -150, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 20, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 20, 45, 2700)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -30, -68, 2700)
    END_INLINE_EVENT
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_44")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_wait_move_end)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1R")), STRING(("P_T_1R")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_45")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC defends Sir Grodus; Peach argues back (peach_aji_48 to 52).
    CALL(evt_msg_print, 0, STRING(("peach_aji_48")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(peach_set_pose, STRING(("P_S_4")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_49")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_50")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_52")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    // TEC says it wants to observe Peach, and she starts to suspect what is wrong with it.
    CALL(evt_msg_print, 0, STRING(("peach_aji_53")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(peach_set_pose, STRING(("P_B_2")))
    WAIT_MS(1200)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_53_01")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_54")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_55")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_56")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(peach_set_pose, STRING(("P_S_4")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_57")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 230, 45, 372, 22, 85, -25, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_P_1")), STRING(("P_T_9")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_01")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_02")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_1")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_03")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_04")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_05")), 0, STRING(("mario")))
    CALL(peach_set_pose, STRING(("P_P_1")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_06")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_C_1")), STRING(("P_T_8")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_58_07")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC says she may contact someone, and Peach types a mail to Mario (peach_aji_65 to 69).
    CALL(evt_msg_print, 0, STRING(("peach_aji_65")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_1")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_66")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_67")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 92, 171, 0, 59, -13, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    CALL(evt_msg_print, 0, STRING(("peach_aji_68")), 0, STRING(("mario")))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, -120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(peach_set_pose, STRING(("P_S_1R")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_69")), 0, STRING(("mario")))
    WAIT_MS(500)
    SPAWN_EVENT(PTR(mail_sousin_se))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("mel")), 0, 0)
    CALL(evt_map_checkanim, STRING(("mel")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC sends the mail and tells her to go back to her room.
    CALL(evt_msg_print, 0, STRING(("peach_aji_70")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -140, 92, 226, -140, 59, 55, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -140, 0, 2000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    // Peach leaves through the door and says good night.
    CALL(evt_msg_print, 0, STRING(("peach_aji_71")), 0, STRING(("mario")))
    WAIT_MS(600)
    CALL(evt_mario_mov_pos, -230, 0, 1200)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_close))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_72")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    // The story advances to Peach's meeting with TEC, the game is saved and the map changes to kpa_00.
    SET_VALUE(GSW_STORY_PROGRESS, STG1_19_PEACH_TEC)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 2)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("kpa_00")), STRING(""))
    RETURN
EVT_END()

/**
 * Lighting and camera setup for drawing the hologram dancer: ambient colour, camera position, up direction
 * and look-at point.
 */
static const GXColor horogram_amb = {0xFF, 0xFF, 0xFF, 0xFF};
static const Vec horogram_pos = {0.0f, 0.0f, 20.0f};
static const Vec horogram_up = {0.0f, 1.0f, 0.0f};
static const Vec horogram_target = {0.0f, 0.0f, 0.0f};

/** The 15 points of a circle (radius about 110) that the dance partner steps around, one per beat. */
f32 move_pos_list[15][2] = {
    {0.0f, -80.0f},
    {39.37592f, -74.69887f},
    {75.489975f, -58.18756f},
    {102.47971f, -29.07189f},
    {109.125275f, 10.068815f},
    {90.71562f, 45.247093f},
    {58.11264f, 67.92478f},
    {19.846874f, 78.68708f},
    {-19.921722f, 78.67708f},
    {-58.15683f, 67.90478f},
    {-90.7439f, 45.2171f},
    {-109.13224f, 10.028812f},
    {-102.458244f, -29.111893f},
    {-75.446236f, -58.217556f},
    {-39.30409f, -74.71887f},
};

/**
 * Moves the dance one beat: advances the dancing couple's center to the next point of the waltz path
 * (move_center) over 0.6 seconds while the camera follows,
 * and every frame places Peach and her hologram partner around the center (compute_peach_pos,
 * compute_horogram_pos).
 */
EVT_BEGIN(peach_dance_move)
    CALL(get_center, LW(2), LW(3))
    CALL(compute_move_pos, LW(4), LW(5))
    BEGIN_INLINE_EVENT
        CALL(move_center, LW(2), LW(3), LW(4), LW(5), 600)
    END_INLINE_EVENT
    SET_VALUE(LW(6), LW(5))
    ADD(LW(5), 323)
    ADD(LW(6), 27)
    CALL(evt_cam3d_evt_set, LW(4), 95, LW(5), LW(4), 45, LW(6), 600, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    SPAWN_EVENT(PTR(peach_dance_pose))
    LOOP(0)
        CALL(dance_flag_check, 4, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(get_center, LW(0), LW(1))
        CALL(evt_mario_get_pos, 0, LW(2), LW(4), LW(3))
        CALL(compute_peach_pos, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_mario_set_pos, LW(2), LW(4), LW(3))
        CALL(compute_horogram_pos, LW(0), LW(1), LW(2), LW(3))
        CALL(set_npc_pos, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2), FLOAT(0.0f), LW(3))
        CALL(set_npc_pos, STRING(("\203_\203~\201[")), LW(0), FLOAT(5.0f), LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Sets Peach's and the hologram's dance pose for the current beat:
 * P_O_F_1, P_O_R_1 or P_O_N_1 for dance step 0, 1 or 2 (skipped during a failed move).
 */
EVT_BEGIN(peach_dance_pose)
    CALL(dance_flag_check, 8, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(dance_get_step, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(peach_set_pose, STRING(("P_O_F_1")))
        CALL(horogram_set_pose, STRING(("P_O_F_2")))
        CASE_EQUAL(1)
        CALL(peach_set_pose, STRING(("P_O_R_1")))
        CALL(horogram_set_pose, STRING(("P_O_R_2")))
        CASE_EQUAL(2)
        CALL(peach_set_pose, STRING(("P_O_N_1")))
        CALL(horogram_set_pose, STRING(("P_O_N_2")))
        CASE_DEFAULT
        DEBUG_PUT_MSG(STRING(("dance_dt.step error")))
    END_SWITCH
    RETURN
EVT_END()

/** Dance action for pressing the step button (A): nothing extra; the couple only takes the next waltz step. */
EVT_BEGIN(peach_dance_action_step)
    RETURN
EVT_END()

/**
 * Common part of the dance turn: Peach and the hologram swing around their center by half a turn in 0.4 seconds
 * with SFX_PEACH_DANCE_TURN1, and the local rotation angle turns by pi.
 */
EVT_BEGIN(dance_action_turn_sub)
    CALL(evt_mario_get_dispdir, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        SET_VALUE(LW(6), 360)
    END_IF
    SET_VALUE(LW(7), LW(6))
    SUBTRACT(LW(7), 180)
    CALL(get_local_rotate, LW(8))
    SET_FLOAT_VALUE(LW(9), LW(8))
    ADD_FLOAT(LW(9), FLOAT(3.140625f))
    MULTIPLY_FLOAT(LW(8), FLOAT(1000.0f))
    MULTIPLY_FLOAT(LW(9), FLOAT(1000.0f))
    CALL(evt_npc_get_position, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2), LW(0), LW(3))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_DANCE_TURN1")), LW(2), 0, LW(3), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(6), LW(7), 400)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 360)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_npc_set_ry, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2))
        CALL(evt_mario_set_dir, LW(0), 0, 0)
        CALL(evt_mario_set_dispdir, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(8), LW(9), 400)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(set_local_rotate, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    RETURN
EVT_END()

/** Dance action for a turn (button B): marks an action in progress (flag 2), does the half turn and clears the mark. */
EVT_BEGIN(peach_dance_action_turn)
    CALL(dance_flag_onoff, 2, 1)
    CALL_CHILD(PTR(dance_action_turn_sub))
    CALL(dance_flag_onoff, 2, 0)
    RETURN
EVT_END()

/**
 * Dance action for a reverse (button X): half turn as in a turn,
 * then the waltz path is run backwards (dance_reverse) from the next beat.
 */
EVT_BEGIN(peach_dance_action_reverse)
    CALL(dance_flag_onoff, 2, 1)
    CALL_CHILD(PTR(dance_action_turn_sub))
    CALL(dance_reverse)
    CALL(dance_flag_check, 1, LW(0))
    CALL(dance_flag_onoff, 2, 0)
    RETURN
EVT_END()

/**
 * Dance action for a wrong button or none: marks the action,
 * switches Peach to the fall pose P_O_D_1 and then back to P_O_F_1 (peach_dance_after_fail),
 * with flags 2 and 8 set meanwhile.
 */
EVT_BEGIN(peach_dance_action_fail)
    CALL(dance_flag_onoff, 2, 1)
    CALL(dance_flag_onoff, 8, 1)
    CALL(peach_set_pose, STRING(("P_O_D_1")))
    CALL(horogram_set_pose, STRING(("P_O_F_2")))
    WAIT_MS(400)
    CALL(peach_dance_after_fail)
    CALL(dance_flag_onoff, 8, 0)
    CALL(dance_flag_onoff, 2, 0)
    RETURN
EVT_END()

/**
 * Spawns the speech-bubble effect for a dance prompt on the dummy NPC ダミー:
 * LW(1) (0 to 2) picks the effect name btn_01 to btn_03 and LW(2) (0 to 2) the bubble type,
 * which also sets the bubble's tilt (0, +45 or -45 degrees).
 */
EVT_BEGIN(dance_btn_disp_sub)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        SET_VALUE(LW(4), STRING(("btn_01")))
        CASE_EQUAL(1)
        SET_VALUE(LW(4), STRING(("btn_02")))
        CASE_EQUAL(2)
        SET_VALUE(LW(4), STRING(("btn_03")))
    END_SWITCH
    SWITCH(LW(2))
        CASE_EQUAL(1)
        SET_VALUE(LW(3), 0)
        CASE_EQUAL(2)
        SET_VALUE(LW(3), 45)
        CASE_EQUAL(0)
        SET_VALUE(LW(3), -45)
    END_SWITCH
    CALL(evt_fuki_btn, LW(2), LW(4), STRING(("\203_\203~\201[")), 0, 0, 0, 0, LW(3), -1)
    RETURN
EVT_END()

/**
 * Shows a button prompt for the dance: sets the flag for the bubble that is used (4096, 8192 or 16384 for LW(1)
 * = 0, 1,
 * 2) and spawns it with dance_btn_disp_sub.
 */
EVT_BEGIN(dance_btn_disp)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CALL(dance_flag_onoff, 4096, 1)
        CASE_EQUAL(1)
        CALL(dance_flag_onoff, 8192, 1)
        CASE_EQUAL(2)
        CALL(dance_flag_onoff, 16384, 1)
    END_SWITCH
    CALL_CHILD(PTR(dance_btn_disp_sub))
    RETURN
EVT_END()

/** Marks the button bubble shown with a cross (batu = X mark) when the player pressed the wrong button. */
EVT_BEGIN(dance_btn_batu)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CALL(evt_fuki_btn_set_batu, STRING(("btn_01")))
        CASE_EQUAL(1)
        CALL(evt_fuki_btn_set_batu, STRING(("btn_02")))
        CASE_EQUAL(2)
        CALL(evt_fuki_btn_set_batu, STRING(("btn_03")))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Removes the button bubble of the dance prompt LW(1) (btn_01, btn_02 or btn_03) if it is shown,
 * and clears its flag.
 */
EVT_BEGIN(dance_btn_clear)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CALL(dance_flag_check, 4096, LW(3))
        IF_INT_EQUAL(LW(3), 1)
            CALL(evt_eff_softdelete, STRING(("btn_01")))
            CALL(dance_flag_onoff, 4096, 0)
        END_IF
        CASE_EQUAL(1)
        CALL(dance_flag_check, 8192, LW(3))
        IF_INT_EQUAL(LW(3), 1)
            CALL(evt_eff_softdelete, STRING(("btn_02")))
            CALL(dance_flag_onoff, 8192, 0)
        END_IF
        CASE_EQUAL(2)
        CALL(dance_flag_check, 16384, LW(3))
        IF_INT_EQUAL(LW(3), 1)
            CALL(evt_eff_softdelete, STRING(("btn_03")))
            CALL(dance_flag_onoff, 16384, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/** Removes every button bubble of the dance that is still shown. */
EVT_BEGIN(dance_btn_clear_all)
    CALL(dance_flag_check, 4096, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_eff_softdelete, STRING(("btn_01")))
        CALL(dance_flag_onoff, 4096, 0)
    END_IF
    CALL(dance_flag_check, 8192, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_eff_softdelete, STRING(("btn_02")))
        CALL(dance_flag_onoff, 8192, 0)
    END_IF
    CALL(dance_flag_check, 16384, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_eff_softdelete, STRING(("btn_03")))
        CALL(dance_flag_onoff, 16384, 0)
    END_IF
    RETURN
EVT_END()

/** Waits until the timer in LW(4) reaches 660 ms, the length of one dance move. */
EVT_BEGIN(wait_action_time)
    LOOP(0)
        CALL(evt_sub_get_stopwatch, LW(12), LW(4))
        IF_INT_GREATER_OR_EQUAL(LW(4), 660)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Dims the room for the dance: the map blend colour fades from 255 down to 192 over half a second. */
EVT_BEGIN(peach_dance_dark)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 192, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/** Brightens the room again after the dance: the map blend colour fades from 192 back up to 255 over half a second. */
EVT_BEGIN(peach_dance_light)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 192, 255, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Peach's waltz with her hologram (the dance minigame in TEC's room):
 * to BGM_EVT_PEACH_WALTZ1, for 32 seconds, a random button prompt (A: step, B: turn, X: reverse) appears every
 * second beat; pressing the right button in time makes the couple do it and counts a point in LW(15),
 * a wrong or missing press makes Peach stumble (SFX_PEACH_DANCE_NG1).
 * Afterwards the hologram fades out and the score is evaluated.
 */
EVT_BEGIN(peach_evt_dance)
#if VERSION >= VERSION_US
    CALL(evt_npc_pera_onoff, STRING("ホログラム"), 0)
    CALL(evt_npc_set_ry, STRING("ホログラム"), 180)
    CALL(evt_npc_pera_onoff, STRING("ホログラム2"), 0)
    CALL(evt_npc_set_ry, STRING("ホログラム2"), 180)
#else
    CALL(evt_npc_pera_onoff, STRING(("\203z\203\215\203O\203\211\203\200")), 0)
    CALL(evt_npc_set_ry, STRING(("\203z\203\215\203O\203\211\203\200")), 180)
    CALL(evt_npc_pera_onoff, STRING(("\203z\203\215\203O\203\211\203\2002")), 0)
    CALL(evt_npc_set_ry, STRING(("\203z\203\215\203O\203\211\203\2002")), 180)
#endif
    LABEL(100)
    SET_VALUE(LW(15), 0)
    SET_VALUE(LW(14), 0)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(12), 0)
    SET_VALUE(LW(11), 0)
    CALL(dance_data_init)
    CALL(get_center, LW(2), LW(3))
#if VERSION >= VERSION_US
    CALL(evt_npc_set_position, STRING("ダミー"), LW(2), FLOAT(5.0f), LW(3))
    CALL(peach_set_pose, STRING("P_O_Z_1"))
    CALL(evt_npc_set_anim, STRING("ホログラム"), STRING("P_O_Z_1"))
    CALL(evt_npc_set_anim, STRING("ホログラム2"), STRING("P_O_Z_1R"))
    WAIT_MS(350)
    CALL(peach_set_pose, STRING("P_O_S_1"))
    CALL(horogram_set_pose, STRING("P_O_S_2"))
    WAIT_MS(1000)
    // The waltz music starts and the dance timer begins.
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_PEACH_WALTZ1"))
#else
    CALL(evt_npc_set_position, STRING(("\203_\203~\201[")), LW(2), FLOAT(5.0f), LW(3))
    CALL(peach_set_pose, STRING(("P_O_Z_1")))
    CALL(evt_npc_set_anim, STRING(("\203z\203\215\203O\203\211\203\200")), STRING(("P_O_Z_1")))
    CALL(evt_npc_set_anim, STRING(("\203z\203\215\203O\203\211\203\2002")), STRING(("P_O_Z_1R")))
    WAIT_MS(350)
    CALL(peach_set_pose, STRING(("P_O_S_1")))
    CALL(horogram_set_pose, STRING(("P_O_S_2")))
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING(("BGM_EVT_PEACH_WALTZ1")))
#endif
    CALL(bgm_start_wait)
    WAIT_MS(4000)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(13))
    SPAWN_EVENT(PTR(peach_dance_move))
    LOOP(0)
        CALL(evt_sub_get_stopwatch, LW(13), LW(4))
        IF_INT_GREATER_OR_EQUAL(LW(4), 660)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(LW(11), 2000)
    LOOP(0)
        CALL(evt_sub_get_stopwatch, LW(13), LW(4))
        IF_INT_GREATER_OR_EQUAL(LW(4), 32000)
            BREAK_LOOP
        END_IF
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(12))
#if VERSION == VERSION_EU
        CALL(wait_move_end_50hz)
#endif
        SPAWN_EVENT(PTR(peach_dance_move))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(12), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(4), 660)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_random, 2, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_RAW_VALUE(LW(2), 1)
            CASE_EQUAL(1)
            SET_RAW_VALUE(LW(2), 0)
            CASE_EQUAL(2)
            SET_RAW_VALUE(LW(2), 2)
        END_SWITCH
        // A button prompt appears on the beat.
        CALL_CHILD(PTR(dance_btn_disp))
#if VERSION == VERSION_EU
        CALL(wait_move_end_50hz)
#endif
        SPAWN_EVENT(PTR(peach_dance_move))
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(14))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(14), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(4), 660)
                SET_VALUE(LW(3), 2)
                BREAK_LOOP
            END_IF
            CALL(btn_check, LW(2), LW(3))
            IF_INT_NOT_EQUAL(LW(3), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(14))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(12), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(4), 1320)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(13), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(4), LW(11))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        ADD(LW(11), 2000)
#if VERSION == VERSION_EU
        CALL(wait_move_end_50hz)
#endif
        SPAWN_EVENT(PTR(peach_dance_move))
        IF_INT_NOT_EQUAL(LW(3), 0)
            SWITCH(LW(3))
                CASE_EQUAL(1)
                ADD(LW(15), 1)
                CALL_CHILD(PTR(dance_btn_clear))
                SWITCH(LW(2))
                    CASE_EQUAL(1)
                    // The right button makes the couple step, turn or reverse and scores a point.
                    SPAWN_EVENT(PTR(peach_dance_action_step))
                    CASE_EQUAL(2)
                    SPAWN_EVENT(PTR(peach_dance_action_turn))
                    CASE_EQUAL(0)
                    SPAWN_EVENT(PTR(peach_dance_action_reverse))
                END_SWITCH
                CASE_EQUAL(2)
#if VERSION >= VERSION_US
                CALL(evt_npc_get_position, STRING("ホログラム"), LW(5), LW(6), LW(7))
                CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DANCE_NG1"), LW(5), 0, LW(7), 0)
#else
                CALL(evt_npc_get_position, STRING(("\203z\203\215\203O\203\211\203\200")), LW(5), LW(6), LW(7))
                CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_DANCE_NG1")), LW(5), 0, LW(7), 0)
#endif
                CALL_CHILD(PTR(dance_btn_batu))
                SPAWN_EVENT(PTR(peach_dance_action_fail))
            END_SWITCH
        END_IF
#if VERSION == VERSION_EU
        CALL_CHILD(PTR(dance_btn_clear_all)) // PAL clears the buttons at once, as JP does
#else
#if VERSION >= VERSION_US
        BEGIN_INLINE_EVENT
            CALL(dance_flag_check, 4096, LW(5))
            IF_INT_NOT_EQUAL(LW(5), 0)
                WAIT_MS(400)
                CALL(evt_eff_softdelete, STRING("btn_01"))
                CALL(dance_flag_onoff, 4096, 0)
            END_IF
        END_INLINE_EVENT
#else
        CALL_CHILD(PTR(dance_btn_clear_all))
#endif
#endif
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(12), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(4), 1980)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(12))
    END_LOOP
    DELETE_EVENT(LW(13))
    // The dance ends and the normal music returns.
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
#if VERSION >= VERSION_US
    CALL(evt_snd_envon_f, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI4"), 2000)
    WAIT_MS(400)
    CALL(horogram_flag_onoff, 1, 4)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(horogram_set_pose, STRING("P_S_1"))
    WAIT_MS(300)
    SET_VALUE(LW(10), LW(15))
    CALL(evt_npc_get_position, STRING("ホログラム"), LW(2), LW(0), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DANCE_APPEAR1"), LW(2), 0, LW(3), 0)
#else
    CALL(evt_snd_envon_f, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")), 2000)
    WAIT_MS(400)
    CALL(horogram_flag_onoff, 1, 4)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(horogram_set_pose, STRING(("P_S_1")))
    WAIT_MS(300)
    SET_VALUE(LW(10), LW(15))
    CALL(evt_npc_get_position, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2), LW(0), LW(3))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_DANCE_APPEAR1")), LW(2), 0, LW(3), 0)
#endif
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 192, 0, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
#if VERSION >= VERSION_US
    CALL(evt_npc_set_color, STRING("ホログラム"), 255, 255, 255, LW(0))
#else
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\200")), 255, 255, 255, LW(0))
#endif
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
#if VERSION >= VERSION_US
    CALL(evt_npc_flag_onoff, 1, STRING("ホログラム"), NPC_FLAG_NO_DRAW)
#else
    CALL(evt_npc_flag_onoff, 1, STRING(("\203z\203\215\203O\203\211\203\200")), 128)
#endif
    CALL_CHILD(PTR(peach_dance_light))
    SET_VALUE(LW(15), LW(10))
    CALL(evt_cam3d_evt_set, 0, 105, 243, 0, 55, -53, 1500, EASING_SINE_IN_OUT)
    CALL(dance_data_init)
    CALL(compute_peach_pos, 0, -80, LW(0), LW(1))
    CALL(evt_mario_mov_pos, LW(0), LW(1), 1500)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    IF_INT_LESS(LW(15), 10)
#if VERSION >= VERSION_US
        CALL(evt_map_replayanim, 0, STRING("moza"))
        CALL(evt_msg_print, 0, STRING("peach_aji_94"), 0, 0)
        CALL(evt_map_pauseanim, 0, STRING("moza"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("peach_aji_95"), 0, STRING("ダミー"))
        CALL(horogram_flag_onoff, 0, 4)
        CALL(evt_npc_set_ry, STRING("ホログラム"), 180)
        CALL_CHILD(PTR(peach_dance_dark))
        CALL(compute_horogram_pos, 0, -80, LW(2), LW(3))
        CALL(evt_npc_set_position, STRING("ホログラム"), LW(2), 0, LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DANCE_APPEAR1"), LW(2), 0, LW(3), 0)
#else
        CALL(evt_map_replayanim, 0, STRING(("moza")))
        CALL(evt_msg_print, 0, STRING(("peach_aji_94")), 0, 0)
        CALL(evt_map_pauseanim, 0, STRING(("moza")))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING(("\203_\203~\201[")), LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING(("peach_aji_95")), 0, STRING(("\203_\203~\201[")))
        CALL(horogram_flag_onoff, 0, 4)
        CALL(evt_npc_set_ry, STRING(("\203z\203\215\203O\203\211\203\200")), 180)
        CALL_CHILD(PTR(peach_dance_dark))
        CALL(compute_horogram_pos, 0, -80, LW(2), LW(3))
        CALL(evt_npc_set_position, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2), 0, LW(3))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_DANCE_APPEAR1")), LW(2), 0, LW(3), 0)
#endif
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 192, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
#if VERSION >= VERSION_US
        CALL(evt_npc_set_color, STRING("ホログラム"), 255, 255, 255, LW(0))
#else
        CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\200")), 255, 255, 255, LW(0))
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_cam3d_evt_set, 0, 95, 243, 0, 45, -53, 700, EASING_SINE_IN_OUT)
        WAIT_MS(700)
        GOTO(100)
    END_IF
    RETURN
EVT_END()

/**
 * Peach and TEC in chapter 2: places dummy and hologram NPCs of Peach (ダミー, ホログラム, ホログラム2),
 * plays peach_aji_76 to 77, sets the story progress to STG2_25_02_PEACH_SHALL_WE_DANCE
 * and changes map to hei_00.
 */
static EVT_BEGIN(peach_evt_stg2)
    CALL(evt_mario_set_mode, 1)
    // The dummy Peach and two hologram Peaches are created for the dance.
    CALL(evt_npc_entry, STRING(("\203_\203~\201[")), STRING(("c_peach")))
    CALL(evt_npc_flag_onoff, 1, STRING(("\203_\203~\201[")), NPC_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING(("\203_\203~\201[")), NPC_FLAG_INTANGIBLE)
    CALL(evt_npc_set_width, STRING(("\203_\203~\201[")), FLOAT(20.0f))
    CALL(evt_npc_set_height, STRING(("\203_\203~\201[")), FLOAT(47.0f))
    CALL(evt_npc_entry, STRING(("\203z\203\215\203O\203\211\203\200")), STRING(("c_peach")))
    CALL(evt_npc_set_tribe, STRING(("\203z\203\215\203O\203\211\203\200")), STRING(("\203s\201[\203`\225P")))
    CALL(evt_npc_flag_onoff, 1, STRING(("\203z\203\215\203O\203\211\203\200")), NPC_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING(("\203z\203\215\203O\203\211\203\200")), NPC_FLAG_INTANGIBLE)
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\200")), 255, 255, 255, 255)
    CALL(evt_npc_set_width, STRING(("\203z\203\215\203O\203\211\203\200")), FLOAT(20.0f))
    CALL(evt_npc_set_height, STRING(("\203z\203\215\203O\203\211\203\200")), FLOAT(47.0f))
    CALL(evt_npc_entry, STRING(("\203z\203\215\203O\203\211\203\2002")), STRING(("c_peach_r")))
    CALL(evt_npc_set_tribe, STRING(("\203z\203\215\203O\203\211\203\2002")), STRING(("\203s\201[\203`\225P")))
    CALL(evt_npc_flag_onoff, 1, STRING(("\203z\203\215\203O\203\211\203\2002")), NPC_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING(("\203z\203\215\203O\203\211\203\2002")), NPC_FLAG_INTANGIBLE)
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\2002")), 255, 255, 255, 255)
    CALL(evt_npc_set_width, STRING(("\203z\203\215\203O\203\211\203\2002")), FLOAT(20.0f))
    CALL(evt_npc_set_height, STRING(("\203z\203\215\203O\203\211\203\2002")), FLOAT(47.0f))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(horogram_init)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\200")), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\2002")), 255, 255, 255, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(horogram_cap_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(disp_horogram_evt)
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // Peach asks TEC if her mail reached Mario, and TEC confirms it did.
    CALL(evt_msg_print, 0, STRING(("peach_aji_76")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_76_01")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_76_02")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_77")), 0, STRING(("mario")))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC offers to let her dance, having overheard her regret about balls (peach_aji_78 to 86).
    CALL(evt_msg_print, 0, STRING(("peach_aji_78")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 300, 1, 20)
    CALL(evt_msg_print, 0, STRING(("peach_aji_79")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_80")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_adjust_dir)
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_83")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_84")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(peach_set_pose, STRING(("P_C_1")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_C_1")), STRING(("P_T_8")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_85")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_86")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL_CHILD(PTR(peach_dance_dark))
    LABEL(100)
    CALL(evt_npc_set_position, STRING(("\203z\203\215\203O\203\211\203\200")), 30, 0, -68)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 192, 500)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_DANCE_APPEAR1")), 30, 0, -67, 0)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_color, STRING(("\203z\203\215\203O\203\211\203\200")), 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(peach_set_pose, STRING(("P_B_2")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    // TEC's hologram appears as a second Peach (peach_aji_91 to 92).
    CALL(evt_msg_print, 0, STRING(("peach_aji_91")), 0, STRING(("mario")))
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_cam3d_evt_set, 0, 95, 243, 0, 45, -53, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING(("peach_aji_92")), 0, STRING(("mario")))
    CALL(dance_data_init)
    CALL(compute_peach_pos, 0, -80, LW(0), LW(1))
    CALL(compute_horogram_pos, 0, -80, LW(2), LW(3))
    DEBUG_PUT_MSG(STRING(("move_pos")))
    DEBUG_PUT_REG(LW(0))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\203z\203\215\203O\203\211\203\200")), LW(2), LW(3), 0, FLOAT(48.0f), 0)
        CALL(evt_npc_set_ry, STRING(("\203z\203\215\203O\203\211\203\200")), 180)
        CALL(evt_npc_set_anim, STRING(("\203z\203\215\203O\203\211\203\200")), STRING(("P_S_1")))
        CALL(evt_npc_set_anim, STRING(("\203z\203\215\203O\203\211\203\2002")), STRING(("P_S_1R")))
        CALL(horogram_flag_onoff, 1, 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, LW(0), LW(1), FLOAT(48.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("peach_aji_93")), 0, 0)
    // The dance minigame plays.
    CALL_CHILD(PTR(peach_evt_dance))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC thanks her for the dance, falls oddly quiet and promises to keep its word (peach_aji_96 to 98).
    CALL(evt_msg_print, 0, STRING(("peach_aji_96")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_97")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_98")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 92, 171, 0, 59, -13, 1000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    WAIT_MS(150)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, 120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(peach_set_pose, STRING(("P_S_1R")))
    WAIT_MS(500)
    // Peach writes another mail to Mario and TEC sends it.
    CALL(evt_msg_print, 0, STRING(("peach_aji_99")), 0, STRING(("mario")))
    WAIT_MS(500)
    SPAWN_EVENT(PTR(mail_sousin_se))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("mel")), 0, 0)
    CALL(evt_map_checkanim, STRING(("mel")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_100")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -140, 88, 226, -140, 55, 55, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -140, 0, 2000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_peach_set_condition, 6)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_7")), STRING(("P_T_13")))
    // Peach leaves and the story advances; the map changes to hei_00.
    CALL(evt_msg_print, 0, STRING(("peach_aji_101")), 0, STRING(("mario")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_101_01")), 0, 0)
    CALL(evt_mario_mov_pos, -230, 0, 1000)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_close))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_102")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_peach_set_condition, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_25_02_PEACH_SHALL_WE_DANCE)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 5)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("hei_00")), STRING(""))
    RETURN
EVT_END()

/**
 * Peach's chapter 3 scene with TEC: peach_aji_115 to 119_02 play,
 * then the map changes to aji_01 at the e_bero_5 entrance with a black fade.
 */
static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    // Peach is called to TEC's room again.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_115")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_116")), 0, STRING(("mario")))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    // TEC asks her to go to where Sir Grodus is and ask him a question (peach_aji_117 to 119_02).
    CALL(evt_msg_print, 0, STRING(("peach_aji_117")), 0, 0)
    CALL(evt_msg_print, 0, STRING(("peach_aji_118")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_119")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING(("peach_aji_119_01")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_119_02")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_4")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    // Peach worries she will be caught; TEC says she can find a soldier's uniform (peach_aji_120 to 123).
    CALL(evt_msg_print, 0, STRING(("peach_aji_120")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_121")), 0, 0)
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1300)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(peach_set_pose, STRING(("P_G_1")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_G_1")), STRING(("P_G_10")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_122")), 0, STRING(("mario")))
    CALL(peach_set_pose, STRING(("P_E_1")))
    WAIT_MS(400)
    CALL(peach_set_pose, STRING(("P_E_2")))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_123")), 0, 0)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    // Peach agrees and the map changes to the central hall.
    CALL(evt_msg_print, 0, STRING(("peach_aji_124")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_125")), 0, 0)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_BLACK, 600, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING(("aji_01")), STRING(("e_bero_5")))
    RETURN
EVT_END()

/**
 * Peach's second chapter 3 scene with TEC: doors open and close with sound, peach_aji_156 to 157_03 play,
 * the story progress becomes STG3_33_04_PEACH_BACK_TO_TEC and the map changes to mri_01.
 */
static EVT_BEGIN(peach_evt_stg3_1)
    CALL(evt_mario_key_onoff, 0)
    // Peach returns to TEC's room after her errand.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_pos, -215, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -140, 0, 80)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(100)
    CALL(evt_mario_wait_move_end)
    // Peach accuses TEC of aiming to conquer the world; TEC says it was built for that (peach_aji_156 to
    // 157_01).
    CALL(evt_msg_print, 0, STRING(("peach_aji_156")), 0, STRING(("mario")))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_157")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING(("peach_aji_157_01")), 0, STRING(("mario")))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(peach_set_pose, STRING(("P_S_4")))
    CALL(evt_cam3d_evt_set, -30, 67, -220, -30, 37, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_mario_set_dispdir, 0)
    WAIT_MS(300)
    // TEC asks whether she dislikes the world; she answers that nobody likes everything about it.
    CALL(evt_msg_print, 0, STRING(("peach_aji_157_02")), 0, 0)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_157_03")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_157_04")), 0, 0)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    WAIT_MS(500)
    // She asks about the Crystal Stars, but TEC cannot answer.
    CALL(evt_msg_print, 0, STRING(("peach_aji_158")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_159")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING(("peach_aji_160")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_161")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    WAIT_MS(150)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, 120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(peach_set_pose, STRING(("P_S_1R")))
    WAIT_MS(500)
    // She asks TEC to send another mail.
    CALL(evt_msg_print, 0, STRING(("peach_aji_163")), 0, STRING(("mario")))
    WAIT_MS(500)
    SPAWN_EVENT(PTR(mail_sousin_se))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("mel")), 0, 0)
    CALL(evt_map_checkanim, STRING(("mel")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC sends it and says good night.
    CALL(evt_msg_print, 0, STRING(("peach_aji_164")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_165")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -140, 92, 226, -140, 59, 55, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -140, 0, 2000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(2500)
    CALL(evt_mario_mov_pos, -230, 0, 1000)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_close))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_166")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    // The story advances, the game is saved and the map changes to mri_01.
    SET_VALUE(GSW_STORY_PROGRESS, STG3_33_04_PEACH_BACK_TO_TEC)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 8)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("mri_01")), STRING(""))
    RETURN
EVT_END()

/**
 * TEC's quiz in Peach's chapter 4 scene: five questions, each with its lead-in message, the question, the
 * answer menu and the number of the right answer.
 */
EvtWord peach_evt_stg4_quiz_data[5][4] = {
    (EvtWord)("peach_aji_174"), (EvtWord)("peach_aji_175"), (EvtWord)("peach_aji_175_sel"), 1, (EvtWord)("peach_aji_178"), (EvtWord)("peach_aji_179"), (EvtWord)("peach_aji_179_sel"), 0,
    (EvtWord)("peach_aji_180"), (EvtWord)("peach_aji_181"), (EvtWord)("peach_aji_183_sel"), 2, (EvtWord)("peach_aji_184"), (EvtWord)("peach_aji_185"), (EvtWord)("peach_aji_185_sel"), 1,
    (EvtWord)("peach_aji_186"), (EvtWord)("peach_aji_187"), (EvtWord)("peach_aji_187_sel"), 2,
};

static EVT_BEGIN(peach_evt_stg4)
    CALL(evt_mario_key_onoff, 0)
    // Peach is called to TEC's room again.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 115, 133, 430, 115, 55, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_set_pos, -215, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_cam3d_evt_set, -105, 133, 430, -105, 55, -13, 3000, EASING_LINEAR)
    WAIT_MS(3000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -140, 0, 80)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(w_door_close))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(100)
    CALL(evt_mario_wait_move_end)
    WAIT_MS(1000)
    // TEC seems conflicted and says it wants to give Peach a quiz (peach_aji_166_01 to 169).
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_01")), 0, STRING(("mario")))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_02")), 0, STRING(("mario")))
    WAIT_MS(2000)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_03")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_04")), 0, STRING(("mario")))
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 700, EASING_SINE_IN_OUT)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_05")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_06")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_07")), 0, STRING(("mario")))
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_4")))
    CALL(evt_cam3d_evt_set, -30, 67, -220, -30, 37, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_mario_set_dispdir, 0)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING(("peach_aji_166_08")), 0, 0)
    CALL(peach_set_pose, STRING(("P_S_1")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_167")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_168")), 0, 0)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_msg_print, 0, STRING(("peach_aji_169")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    SET_VALUE(LW(15), 0)
    SET_VALUE(LF(15), 0)
    LOOP(0)
        CALL(peach_evt_stg4_read_quiz_data, LW(15), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_map_replayanim, 0, STRING(("moza")))
        // The quiz: five multiple-choice questions about Grodus's plan, read from the quiz table.
        CALL(evt_msg_print, 0, LW(2), 0, 0)
        CALL(evt_map_pauseanim, 0, STRING(("moza")))
        CALL(peach_set_pose, STRING(("P_C_1")))
        CALL(evt_cam3d_evt_set, 0, 108, 233, 0, 65, -13, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        WAIT_MS(500)
        CALL(evt_map_replayanim, 0, STRING(("moza")))
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LW(15), 2)
            IF_INT_EQUAL(LF(15), 1)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            CALL(evt_msg_print, 0, LW(3), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING(("peach_aji_181_01")), 0, 0)
        END_IF
        CALL(evt_map_pauseanim, 0, STRING(("moza")))
        IF_INT_EQUAL(LW(15), 2)
            IF_INT_EQUAL(LF(15), 0)
                CALL(evt_mario_get_pos, 0, LW(12), LW(13), LW(14))
                CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_3")), LW(12), LW(13), LW(14), 0)
                CALL(peach_set_pose, STRING(("P_B_2")))
                WAIT_MS(500)
                CALL(evt_cam3d_evt_set, 230, 45, 327, 22, 85, -25, 0, EASING_SINE_IN_OUT)
                WAIT_FRAMES(1)
                CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
                CALL(evt_msg_print, 0, STRING(("peach_aji_182")), 0, STRING(("mario")))
                WAIT_MS(500)
                CALL(evt_map_replayanim, 0, STRING(("moza")))
                CALL(evt_msg_print, 0, STRING(("peach_aji_183")), 0, 0)
                CALL(evt_map_pauseanim, 0, STRING(("moza")))
                CALL(peach_set_pose, STRING(("P_C_1")))
                WAIT_FRAMES(1)
                CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
                SET_VALUE(LF(15), 1)
            END_IF
        END_IF
        CALL(evt_msg_select, 0, LW(4))
        CALL(evt_msg_continue)
        IF_INT_EQUAL(LW(5), LW(0))
            CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(15), 4)
                CALL(evt_map_replayanim, 0, STRING(("moza")))
                // After the fifth right answer TEC lets her use the communicator, and she wonders if it was
                // right for TEC to tell her such things.
                CALL(evt_msg_print, 0, STRING(("peach_aji_190")), 0, 0)
                CALL(evt_map_pauseanim, 0, STRING(("moza")))
                BREAK_LOOP
            END_IF
            ADD(LW(15), 1)
            ELSE
            CALL(evt_map_replayanim, 0, STRING(("moza")))
            CALL(evt_msg_print, 0, STRING(("peach_aji_188")), 0, 0)
            CALL(evt_map_pauseanim, 0, STRING(("moza")))
            // Restart the questions without clearing the alternate-introduction flag LF(15).
            SET_VALUE(LW(15), 0)
            CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
            WAIT_FRAMES(1)
        END_IF
        WAIT_MS(500)
    END_LOOP
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_190_01")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_190_02")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 92, 171, 0, 59, -13, 700, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    WAIT_MS(150)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, 120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(peach_set_pose, STRING(("P_S_1R")))
    WAIT_MS(500)
    // Peach types her mail and TEC sends it.
    CALL(evt_msg_print, 0, STRING(("peach_aji_192")), 0, STRING(("mario")))
    WAIT_MS(500)
    SPAWN_EVENT(PTR(mail_sousin_se))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("mel")), 0, 0)
    CALL(evt_map_checkanim, STRING(("mel")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_193")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -140, 92, 226, -140, 59, 55, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -140, 0, 2000)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_open_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_open))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_peach_set_condition, 6)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_7")), STRING(("P_T_13")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_194")), 0, STRING(("mario")))
    CALL(evt_mario_mov_pos, -230, 0, 1000)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_close))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC says good night; the story advances to the X-Naut PhD report and the map changes to gor_04.
    CALL(evt_msg_print, 0, STRING(("peach_aji_195")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_peach_set_condition, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 11)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("gor_04")), 0)
    RETURN
EVT_END()

/**
 * Peach's chapter 5 scene with TEC (peach_aji_206 to 217): TEC asks her to sneak into Grodus's room
 * for a data disk it cannot reach over the network (peach_aji_213 to 215 run under the TEC fade),
 * and sends her back to the elevator. The map animation moza runs only while TEC speaks (its screen).
 * The story progress then becomes STG5_44_PEACH_TECS_REQUEST_2 and control returns to the player.
 */
static EVT_BEGIN(peach_evt_stg5)
    CALL(evt_mario_key_onoff, 0)
    // Peach is called to TEC's room again.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_pos, -200, 0, 0)
    CALL(evt_cam3d_evt_set, 230, 45, 372, 22, 85, -25, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(2000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    WAIT_MS(500)
    CALL(evt_mario_mov_pos2, -140, 0, 60)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_G_1")))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_G_1")), STRING(("P_G_10")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_206")), 0, STRING(("mario")))
    CALL(peach_set_pose, STRING(("P_E_1")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_207")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(2000)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_208")), 0, STRING(("mario")))
    WAIT_MS(2000)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC asks her to do something more dangerous this time.
    CALL(evt_msg_print, 0, STRING(("peach_aji_209")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_C_1")), STRING(("P_T_8")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_210")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // She must sneak into Sir Grodus's room and look for a data disk (peach_aji_211 to 216).
    CALL(evt_msg_print, 0, STRING(("peach_aji_211")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("peach_aji_212")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -30, 67, -220, -30, 37, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_mario_set_dispdir, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("peach_aji_213")), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_214")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_215")), 0, 0)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_msg_print, 0, STRING(("peach_aji_216")), 0, STRING(("mario")))
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // TEC tells her to take the elevator, and the story advances to TEC's second request.
    CALL(evt_msg_print, 0, STRING(("peach_aji_217")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_44_PEACH_TECS_REQUEST_2)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Peach's second chapter 5 scene with TEC: peach_aji_260 to 264 play,
 * the story progress becomes STG5_47_PEACH_TEC_TALK and the map changes to usu_00.
 */
static EVT_BEGIN(peach_evt_stg5_1)
    CALL(evt_mario_key_onoff, 0)
    // Peach returns to TEC's room after her search.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_set_pos, -220, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 2000, EASING_SINE_IN_OUT)
        CALL(evt_mario_mov_pos, -30, -68, 2000)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_wait_move_end)
    WAIT_MS(500)
    // TEC is still analysing the data on the disk, which is heavily encrypted.
    CALL(evt_msg_print, 0, STRING(("peach_aji_260")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_261")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_262")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_263")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_264")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 92, 171, 0, 59, -13, 700, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    WAIT_MS(150)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, 120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(peach_set_pose, STRING(("P_S_1R")))
    WAIT_MS(500)
    // Peach asks TEC to send another message and TEC sends it.
    CALL(evt_msg_print, 0, STRING(("peach_aji_265")), 0, STRING(("mario")))
    WAIT_MS(500)
    SPAWN_EVENT(PTR(mail_sousin_se))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("mel")), 0, 0)
    CALL(evt_map_checkanim, STRING(("mel")), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_266")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_267")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -140, 92, 226, -140, 59, 55, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -140, 0, 2000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_msg_print, 0, STRING(("peach_aji_267_01")), 0, 0)
    CALL(evt_peach_set_condition, 6)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_7")), STRING(("P_T_13")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_267_02")), 0, STRING(("mario")))
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_mario_mov_pos, -230, 0, 1000)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_close))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    WAIT_MS(2000)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_268")), 0, 0)
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 96, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    // The story advances, the game is saved and the map changes to usu_00.
    SET_VALUE(GSW_STORY_PROGRESS, STG5_47_PEACH_TEC_TALK)
    CALL(evt_peach_set_condition, 0)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 14)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("usu_00")), STRING(""))
    RETURN
EVT_END()

/**
 * Peach's chapter 6 scene with TEC (the shutdown):
 * peach_aji_282 to 284 and more play, the story progress becomes STG6_55_01_PEACH_TEC_DELETED
 * and the map changes to gor_01.
 */
static EVT_BEGIN(peach_evt_stg6)
    CALL(evt_mario_key_onoff, 0)
    // Peach visits TEC once more.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    CALL(evt_mario_set_mode, 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
    CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
    CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
    CALL(evt_map_playanim, STRING(("moza")), 1, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_282")), 0, STRING(("mario")))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 45, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING(("ren1")), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, -30, -68, 1500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_282_01")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_QUESTION1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING(("peach_aji_282_02")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_282_03")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -30, 67, -220, -30, 37, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    CALL(evt_mario_set_dispdir, 0)
    WAIT_MS(300)
    // TEC doesn't sound right; it tells her of a chance to escape (peach_aji_282_04 to 287).
    CALL(evt_msg_print, 0, STRING(("peach_aji_282_04")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_283")), 0, 0)
    CALL(peach_set_pose, STRING(("P_S_4")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_284")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    WAIT_MS(300)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_285")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_286")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_287")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 220, 200, 380, -147, 60, -4, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_mario_set_reardir)
    WAIT_MS(3000)
    CALL(evt_fade_out, 1000)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -25, 75, 135, -25, 50, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_adjust_dir)
    CALL(peach_set_pose, STRING(("P_B_2")))
    CALL(evt_fade_in, 1000)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    // Peach struggles to believe the news about Grodus's plans.
    CALL(evt_msg_print, 0, STRING(("peach_aji_288")), 0, STRING(("mario")))
    CALL(peach_set_pose, STRING(("P_C_1")))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_289")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_C_1")), STRING(("P_T_8")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_290")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_291")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, -30, 67, -220, -30, 37, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_set_dispdir, 0)
    WAIT_MS(300)
    WAIT_MS(500)
    // TEC explains where she is held and how she can escape (peach_aji_292 to 302).
    CALL(evt_msg_print, 0, STRING(("peach_aji_292")), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_293")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_294")), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_295")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_296")), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_297")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_298")), 0, 0)
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    CALL(peach_set_pose, STRING(("P_S_4")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_4")), STRING(("P_T_4")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_299")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_300")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    WAIT_MS(500)
    CALL(peach_set_pose, STRING(("P_P_1")))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_P_1")), STRING(("P_T_9")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_301")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_302")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    LABEL(100)
    CALL(evt_cam3d_evt_set, 0, 92, 171, 0, 59, -13, 700, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos, 0, -95, 700)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_KEYBOARD1")), 0, 25, 120, LW(15))
    CALL(peach_set_pose, STRING(("P_A_1R")))
    WAIT_MS(1000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    WAIT_MS(500)
    SET_VALUE(LW(0), 180)
    CALL_CHILD(PTR(w_door_open))
    CALL(evt_npc_set_position, STRING(("\216\350\211\272\202P")), -140, 0, 0)
    CALL(evt_npc_set_position, STRING(("\203V\203\205\203\212\203\207\201[")), -190, 0, 0)
    CALL(evt_npc_set_position, STRING(("\216\350\211\272\202Q")), -240, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202P")), -80, 0, 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\203V\203\205\203\212\203\207\201[")), -130, 0, 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING(("\216\350\211\272\202Q")), -180, 0, 0, FLOAT(60.0f), 0)
    // Sir Grodus bursts in and catches Peach; tense music starts.
    CALL(evt_msg_print, 0, STRING(("peach_aji_303")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED2_1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, 10)
    WAIT_MS(300)
    CALL(peach_set_pose, STRING(("P_C_1")))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_304")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -130, 121, 344, -130, 65, 66, 1500, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_EVT_STG8_3RDHEAD1")))
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING(("peach_aji_305")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202P")), 30, -70, 0, FLOAT(60.0f), 0)
        CALL(evt_npc_set_ry, STRING(("\216\350\211\272\202P")), 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING(("\203V\203\205\203\212\203\207\201[")), NPC_FLAG_INTANGIBLE)
        CALL(evt_npc_move_position, STRING(("\203V\203\205\203\212\203\207\201[")), -30, -70, 0, FLOAT(60.0f), 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202Q")), -100, -70, 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    // Grodus confronts TEC, who insists it is normal, while Peach protests (peach_aji_306 to 312).
    CALL(evt_msg_print, 0, STRING(("peach_aji_306")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 140, 332, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_WHILE(GF(0))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("peach_aji_307")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_308")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -30, 100, 184, -30, 65, -13, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING(("peach_aji_309")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(evt_cam3d_evt_set, 0, 80, 44, 0, 130, -150, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING(("P_C_1")))
    WAIT_MS(700)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_310")), 0, 0)
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_cam3d_evt_set, 0, 75, 387, 0, 75, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING(("peach_aji_311")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_312")), 0, STRING(("\216\350\211\272\202Q")))
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202P")), 105, -95, 0, 70, 4)
        CALL(evt_npc_set_ry, STRING(("\216\350\211\272\202P")), 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202Q")), -105, -95, 0, 60, 4)
        CALL(evt_npc_set_ry, STRING(("\216\350\211\272\202Q")), 90)
    END_INLINE_EVENT
    WAIT_MS(1400)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING(("\216\350\211\272\202P")), NPC_FLAG_NO_WALL_HIT)
        CALL(evt_npc_status_onoff, 0, STRING(("\216\350\211\272\202P")), NPC_STATUS_FACE_MOVE_DIR)
        WAIT_FRAMES(1)
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202P")), 105, -100, 100, 0, 0)
        WAIT_MS(100)
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202P")), 105, -95, 100, 0, 0)
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING(("\216\350\211\272\202P")), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_flag_onoff, 0, STRING(("\216\350\211\272\202P")), NPC_FLAG_NO_WALL_HIT)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING(("\216\350\211\272\202Q")), NPC_FLAG_NO_WALL_HIT)
        CALL(evt_npc_status_onoff, 0, STRING(("\216\350\211\272\202Q")), NPC_STATUS_FACE_MOVE_DIR)
        WAIT_FRAMES(1)
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202Q")), -105, -100, 100, 0, 0)
        WAIT_MS(100)
        CALL(evt_npc_move_position, STRING(("\216\350\211\272\202Q")), -105, -95, 100, 0, 0)
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING(("\216\350\211\272\202Q")), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_flag_onoff, 0, STRING(("\216\350\211\272\202Q")), NPC_FLAG_NO_WALL_HIT)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING(("S_sui2")), LW(0), LW(1), LW(2))
    CALL(evt_mapobj_get_position, STRING(("S_sui3")), LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_TEC_BUTTON1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING(("SFX_PEACH_TEC_BUTTON1")), LW(3), LW(4), LW(5), 0)
    WAIT_MS(500)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_mario_set_reardir)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_313")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, 0, 80, -249, 0, 25, 21, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(evt_mario_set_dispdir, 0)
    CALL(peach_set_pose, STRING(("P_C_1")))
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_C_1")), STRING(("P_T_8")))
    // TEC's last mail goes out as it is shut down (peach_aji_313 and 314).
    CALL(evt_msg_print, 0, STRING(("peach_aji_314")), 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 75, 387, 0, 75, -13, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(peach_set_pose, STRING(("P_S_1")))
    CALL(evt_mario_set_dispdir, 180)
    CALL(evt_mario_set_reardir)
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_DISP_DIR_SNAP)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_315")), 0, 0)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_map_pauseanim, 0, STRING(("kika_3")))
    WAIT_MS(500)
    CALL(evt_map_pauseanim, 0, STRING(("kika_2")))
    WAIT_MS(500)
    CALL(evt_map_pauseanim, 0, STRING(("kika_1")))
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("monihiraku")), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_1")), STRING(("P_T_1")))
    // Peach cries out for TEC and rebukes Grodus (peach_aji_316 to 321).
    CALL(evt_msg_print, 0, STRING(("peach_aji_316")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_317")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    CALL(peach_set_pose, STRING(("P_S_6")))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_S_6")), STRING(("P_T_6")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_318")), 0, STRING(("mario")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("peach_aji_319")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_PEACH_SURPRISED1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING(("mario")), STRING(("P_B_2")), STRING(("P_T_7")))
    CALL(evt_msg_print, 0, STRING(("peach_aji_320")), 0, STRING(("mario")))
    CALL(evt_cam3d_evt_set, -35, 100, 184, -35, 65, -13, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING(("peach_aji_321")), 0, STRING(("\203V\203\205\203\212\203\207\201[")))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_55_01_PEACH_TEC_DELETED)
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 15, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING(("S_aji_17")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 17)
    CALL_CHILD(PTR(SAVE_EVT))
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, 57, 0, 56, 0)
    CALL(evt_bero_mapchange, STRING(("gor_01")), STRING(""))
    RETURN
EVT_END()

/** Init of an X-Naut underling in TEC's room: runs the common NPC init and sets NPC_STATUS_FACE_MOVE_DIR. */
static EVT_BEGIN(teshita1_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING(("me")), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

/** Init of a second X-Naut underling in TEC's room: common NPC init plus NPC_STATUS_FACE_MOVE_DIR. */
static EVT_BEGIN(teshita2_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING(("me")), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

/** Init of Grodus (シュリョー) in TEC's room: common NPC init plus NPC_STATUS_FACE_MOVE_DIR. */
static EVT_BEGIN(syuryo_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING(("me")), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

/** Cast of TEC's room besides TEC: two X-Naut underlings and Sir Grodus. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"手下１"*/"\216\350\211\272\202P", // name
        0, 0, // flags..reactionFlags
        teshita1_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手下２"*/"\216\350\211\272\202Q", // name
        0, 0, // flags..reactionFlags
        teshita2_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"シュリョー"*/"\203V\203\205\203\212\203\207\201[", // name
        0, 0, // flags..reactionFlags
        syuryo_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(tec_talk)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_map_replayanim, 0, STRING(("moza")))
    IF_INT_EQUAL(GSWF_AJI_17_TEC_FIRST_TALK_DONE, 0)
        CALL(evt_msg_print, 0, STRING(("stg7_aji_tec_00")), 0, 0)
        SET_VALUE(GSWF_AJI_17_TEC_FIRST_TALK_DONE, 1)
        ELSE
        CALL(evt_msg_print, 0, STRING(("stg7_aji_tec_01")), 0, 0)
    END_IF
    CALL(evt_map_pauseanim, 0, STRING(("moza")))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(aji_17_init_evt)
    SET_VALUE(LW(0), STRING("S_camera_1"))
    SET_VALUE(LW(1), -30)
    SET_VALUE(LW(2), 60)
    // The camera sweep over TEC's room starts.
    SPAWN_EVENT(evt_camera)
    SET_VALUE(LW(0), STRING("S_camera_2"))
    SET_VALUE(LW(1), -60)
    SET_VALUE(LW(2), 30)
    SPAWN_EVENT(evt_camera)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_meil"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kikai_0"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("polySurface1161_q00063"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("polySurface1168_q00065"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_scale, 0, STRING("ren2"), 5, 5, 1)
    // Peach's scenes: each story point from chapter 1 to chapter 6 starts its own conversation with TEC.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_18_01_PEACH_AN_OPEN_DOOR)
        SPAWN_EVENT(peach_evt_stg1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_01_PEACH_CRUMP_X_NAUTS)
        SPAWN_EVENT(peach_evt_stg2)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_33_PEACH_THE_SECRET_WEAPON)
        SPAWN_EVENT(peach_evt_stg3)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_33_03_PEACH_GRODUS)
        SPAWN_EVENT(peach_evt_stg3_1)
        CALL(evt_mario_kill_party, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_04_SHADOW_SIRENS_DOOPLISS)
        SPAWN_EVENT(peach_evt_stg4)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_43_X_NAUTS_RETREAT)
        SPAWN_EVENT(peach_evt_stg5)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_46_PEACH_DISK_SEARCH)
        SPAWN_EVENT(peach_evt_stg5_1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_55_PEACH_GRODUS_X_NAUT)
        CALL(evt_npc_setup, PTR(npcEnt))
        CALL(evt_shuryolight_init, STRING("シュリョー"))
        CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_normal), PTR(shuryolight_head_normal), PTR(shuryolight_tue_normal))
        SPAWN_EVENT(peach_evt_stg6)
    END_IF
    // Mario's visits: at the story point after the Moon, the first visit restores HP and FP and plays TEC's
    // farewell (evt_last); the second plays the aftermath (evt_last2).
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        IF_INT_EQUAL(GSWF_AJI_17_LAST_SCENE_SEEN, 0)
            CALL(evt_mario_get_party, LW(15))
            IF_INT_EQUAL(LW(15), 0)
                CALL(evt_mario_set_party, 0, 1)
                CALL(evt_party_run, 0)
CALL(evt_pouch_mario_recovery)
                CALL(evt_pouch_all_party_recovery)
            END_IF
SET_VALUE(GSWF_AJI_17_LAST_SCENE_SEEN, 1)
            SPAWN_EVENT(evt_last)
        ELSE
            SPAWN_EVENT(evt_last2)
            SET_VALUE(LW(0), PTR(bero_entry_data))
            CALL(evt_bero_get_info)
            CALL_CHILD(bero_case_entry)
            RETURN
        END_IF
    END_IF
    // Otherwise the emergency lights start and the room's entrances are set up.
    SPAWN_EVENT(evt_emergency)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG1_18_01_PEACH_AN_OPEN_DOOR)
        CASE_EQUAL(STG3_33_03_PEACH_GRODUS)
        CASE_EQUAL(STG4_15_04_SHADOW_SIRENS_DOOPLISS)
        CASE_DEFAULT
        CALL_CHILD(evt_bero_info_run)
    END_SWITCH
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_kill_party, 0)
    ELSE
        // As Mario, the fortress music and ambience play; as Peach, her partner is removed.
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI4"))
    END_IF
    // After the final battle the room's machine runs again and the keyboard can be used to talk to TEC
    // (tec_talk).
    IF_INT_GREATER(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kurai"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kikai"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING("kika_1"), 1, 0)
        CALL(evt_map_playanim, STRING("kika_2"), 1, 0)
        CALL(evt_map_playanim, STRING("kika_3"), 1, 0)
        CALL(evt_map_playanim, STRING("moni_1"), 2, 0)
        CALL(evt_map_playanim, STRING("moza"), 1, 0)
        CALL(evt_map_pauseanim, 0, STRING("moza"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("keyboard"), 0, PTR(tec_talk), 0)
    END_IF
RETURN
EVT_END()
#else
EVT_BEGIN(aji_17_init_evt)
#if VERSION >= VERSION_US
    SET_VALUE(LW(0), STRING("S_camera_1"))
#else
    SET_VALUE(LW(0), STRING(("S_camera_1")))
#endif
    SET_VALUE(LW(1), -30)
    SET_VALUE(LW(2), 60)
    // The camera sweep over TEC's room starts.
    SPAWN_EVENT(PTR(evt_camera))
#if VERSION >= VERSION_US
    SET_VALUE(LW(0), STRING("S_camera_2"))
#else
    SET_VALUE(LW(0), STRING(("S_camera_2")))
#endif
    SET_VALUE(LW(1), -60)
    SET_VALUE(LW(2), 30)
    SPAWN_EVENT(PTR(evt_camera))
#if VERSION >= VERSION_US
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_meil"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kikai_0"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("polySurface1161_q00063"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("polySurface1168_q00065"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_scale, 0, STRING("ren2"), 5, 5, 1)
#else
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("S_meil")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("S_kikai_0")), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("polySurface1161_q00063")), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("polySurface1168_q00065")), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_scale, 0, STRING(("ren2")), 5, 5, 1)
#endif
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_18_01_PEACH_AN_OPEN_DOOR)
        SPAWN_EVENT(PTR(peach_evt_stg1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_01_PEACH_CRUMP_X_NAUTS)
        SPAWN_EVENT(PTR(peach_evt_stg2))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_33_PEACH_THE_SECRET_WEAPON)
        SPAWN_EVENT(PTR(peach_evt_stg3))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_33_03_PEACH_GRODUS)
        SPAWN_EVENT(PTR(peach_evt_stg3_1))
        CALL(evt_mario_kill_party, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_04_SHADOW_SIRENS_DOOPLISS)
        SPAWN_EVENT(PTR(peach_evt_stg4))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_43_X_NAUTS_RETREAT)
        SPAWN_EVENT(PTR(peach_evt_stg5))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_46_PEACH_DISK_SEARCH)
        SPAWN_EVENT(PTR(peach_evt_stg5_1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_55_PEACH_GRODUS_X_NAUT)
        CALL(evt_npc_setup, PTR(npcEnt))
#if VERSION >= VERSION_US
        CALL(evt_shuryolight_init, STRING("シュリョー"))
#else
        CALL(evt_shuryolight_init, STRING(("\203V\203\205\203\212\203\207\201[")))
#endif
        CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_normal), PTR(shuryolight_head_normal), PTR(shuryolight_tue_normal))
        SPAWN_EVENT(PTR(peach_evt_stg6))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        IF_INT_EQUAL(GSWF_AJI_17_LAST_SCENE_SEEN, 0)
            CALL(evt_mario_get_party, LW(15))
            IF_INT_EQUAL(LW(15), 0)
                CALL(evt_mario_set_party, 0, 1)
                CALL(evt_party_run, 0)
#if VERSION >= VERSION_US
            END_IF
            CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
            CALL(evt_sub_status_gauge_force_update)
#else
                CALL(evt_pouch_mario_recovery)
                CALL(evt_pouch_all_party_recovery)
            END_IF
#endif
            SET_VALUE(GSWF_AJI_17_LAST_SCENE_SEEN, 1)
            SPAWN_EVENT(PTR(evt_last))
            ELSE
            SPAWN_EVENT(PTR(evt_last2))
            SET_VALUE(LW(0), PTR(bero_entry_data))
            CALL(evt_bero_get_info)
#if VERSION >= VERSION_US
            CALL_CHILD(bero_case_entry)
#else
            CALL_CHILD(PTR(bero_case_entry))
#endif
            RETURN
        END_IF
    END_IF
    // Otherwise the emergency lights start and the room's entrances are set up.
    SPAWN_EVENT(PTR(evt_emergency))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG1_18_01_PEACH_AN_OPEN_DOOR)
        CASE_EQUAL(STG3_33_03_PEACH_GRODUS)
        CASE_EQUAL(STG4_15_04_SHADOW_SIRENS_DOOPLISS)
        CASE_DEFAULT
#if VERSION >= VERSION_US
        CALL_CHILD(evt_bero_info_run)
#else
        CALL_CHILD(PTR(evt_bero_info_run))
#endif
    END_SWITCH
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_kill_party, 0)
        ELSE
#if VERSION >= VERSION_US
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI4"))
    END_IF
    // After the final battle the room's machine runs again and the keyboard can be used to talk to TEC
    // (tec_talk).
    IF_INT_GREATER(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kurai"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kikai"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING("kika_1"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("kika_2"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("kika_3"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("moni_1"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_map_playanim, STRING("moza"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_pauseanim, 0, STRING("moza"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("keyboard"), 0, PTR(tec_talk), 0)
#else
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG7_DUN2")))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG7_AJI4")))
    END_IF
    IF_INT_GREATER(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("S_kurai")), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("S_kikai")), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING(("kika_1")), 1, 0)
        CALL(evt_map_playanim, STRING(("kika_2")), 1, 0)
        CALL(evt_map_playanim, STRING(("kika_3")), 1, 0)
        CALL(evt_map_playanim, STRING(("moni_1")), 2, 0)
        CALL(evt_map_playanim, STRING(("moza")), 1, 0)
        CALL(evt_map_pauseanim, 0, STRING(("moza")))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("keyboard")), 0, PTR(tec_talk), 0)
#endif
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    RETURN
EVT_END()
#endif

// target .data tail pad word

s32 noise(EventEntry* event, BOOL isFirstCall) {
    EvtWord alpha = evtGetValue(event, event->args[0]);
    dispEntry(CAMERA_2D, 1100.0f, 0, noiseDisp, (void*)alpha);
    return EVT_RETURN_DONE;
}

/** Script function: sets Mario's (Peach's) pose to the animation named in args[0]. */
static s32 peach_set_pose(EventEntry* event, BOOL isFirstCall) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}

/**
 * Draws one textured quad of width w and height h with its top-left corner at (x, y)
 * and texture coordinates up to s and t.
 */
static void polygon(f32 x, f32 y, f32 w, f32 h, f32 s, f32 t) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(s, 0.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(s, t);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, t);
}

/**
 * Draw callback that draws the dance hologram: the captured Peach picture (colour texture buf0, depth-like buf1)
 * on a quad at the hologram NPC's position, turned to face the camera (rotated 180 degrees when flag 4 is set),
 * blended with the NPC's colour.
 */
void disp_horogram(CameraId cameraId, void* param) {
    GXCullMode cull;
    Mtx mtx;
    Mtx trans;
    Mtx rot;
    GXColor mat;
    f32 angle;
    NpcEntry* npc = npcNameToPtr("\203z\203\215\203O\203\211\203\200");

    if (horogram_wp->ready) {
        GXGetCullMode(&cull);
        GXInitTexObj(&horogram_wp->tex0, horogram_wp->buf0, 0x130, 0xF0, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&horogram_wp->tex0, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXInitTexObj(&horogram_wp->tex1, horogram_wp->buf1, 0x130, 0xF0, GX_TF_Z8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&horogram_wp->tex1, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        PSMTXTrans(trans, npc->position.x, npc->position.y, npc->position.z);
        angle = reviseAngle(-npc->direction);
        angle -= camGetPtr(cameraId)->viewYaw;
        angle += (horogram_wp->flags & 4) ? 180.0f : 0.0f;
        PSMTXRotRad(rot, 'y', 0.017453292f * angle);
        PSMTXConcat(trans, rot, mtx);
        PSMTXConcat(camGetPtr(cameraId)->view, mtx, mtx);
        GXSetCurrentMtx(GX_PNMTX0);
        GXLoadTexObj(&horogram_wp->tex0, GX_TEXMAP0);
        GXLoadTexObj(&horogram_wp->tex1, GX_TEXMAP1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        mat = npc->color;
        GXSetChanMatColor(GX_COLOR0A0, mat);
        GXSetChanAmbColor(GX_COLOR0A0, horogram_amb);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
        GXSetCullMode(GX_CULL_NONE);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-28.0f, 55.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(28.0f, 55.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(28.0f, 0.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-28.0f, 0.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXSetCullMode(cull);
    }
}

/** Script function that queues disp_horogram for the 3D camera every frame; never finishes. */
s32 disp_horogram_evt(EventEntry* event, BOOL isFirstCall) {
    dispEntry(CAMERA_3D, 0.0f, 2, disp_horogram, NULL);
    return EVT_RETURN_BLOCK;
}

/**
 * Draw callback that renders the picture of the hologram:
 * picks the normal or the mirrored hologram NPC (ホログラム or ホログラム2) by its facing direction,
 * sets up a 608 by 480 viewport and a close camera,
 * draws the NPC's pose into the frame buffer and copies the colour and depth into horogram_wp->buf0 and buf1.
 */
void peach_capture(CameraId cameraId, void* param) {
    CameraEntry saved = *camGetCurPtr();
    Mtx44 projMtx;
    Mtx lookAt;
    Mtx ident;
    f32 viewport[6];
    f32 proj[7];
    u32 sx, sy, sw, sh;
    NpcEntry* holo;
    NpcEntry* holo2;
    NpcEntry* npc;

    holo = npcNameToPtr("\203z\203\215\203O\203\211\203\200");
    holo2 = npcNameToPtr("\203z\203\215\203O\203\211\203\2002");
    if (90.0f <= holo->direction && holo->direction < 270.0f) {
        npc = holo2;
        if (!(horogram_wp->flags & 2)) {
            animPoseSetLocalTime(holo2->poseId, 0.0f);
        }
        horogram_wp->flags |= 2;
    } else {
        npc = holo;
        if (horogram_wp->flags & 2) {
            animPoseSetLocalTime(holo->poseId, 0.0f);
        }
        horogram_wp->flags &= ~2;
    }
    if (!(horogram_wp->flags & 1)) {
        npc = holo;
    }
    GXGetViewportv(viewport);
    GXGetProjectionv(proj);
    GXGetScissor(&sx, &sy, &sw, &sh);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(1, 1, 606, 478);
    C_MTXOrtho(projMtx, 28.0f, 0.0f, -14.0f, 14.0f, 1.0f, 100.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
    {
        Vec pos;
        Vec up;
        Vec target;

        target = horogram_target;
        up = horogram_up;
        pos = horogram_pos;

        C_MTXLookAt(lookAt, &pos, &up, &target);
    }
    PSMTXCopy(lookAt, camGetCurPtr()->view);
    PSMTXIdentity(ident);
    animPoseSetMaterialFlagOff(npc->poseId, MAP_OBJ_FLAG_COLOR);
    animPoseMain(npc->poseId);
    animPoseDrawMtx(npc->poseId, ident, 1, 0.0f, 1.0f);
    animPoseDrawMtx(npc->poseId, ident, 2, 0.0f, 1.0f);
    animPoseDrawMtx(npc->poseId, ident, 3, 0.0f, 1.0f);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x130, 0xF0, GX_TF_RGBA8, GX_TRUE);
    GXCopyTex(horogram_wp->buf0, GX_FALSE);
    GXPixModeSync();
    GXSetTexCopyDst(0x130, 0xF0, GX_TF_Z8, GX_TRUE);
    GXCopyTex(horogram_wp->buf1, GX_TRUE);
    GXPixModeSync();
    PSMTXCopy(saved.view, camGetCurPtr()->view);
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(proj);
    GXSetScissor(sx, sy, sw, sh);
    horogram_wp->ready = 1;
}

/** Script function that queues the capture of the hologram (peach_capture) every frame; blocks forever. */
s32 horogram_cap_evt(EventEntry* event, BOOL isFirstCall) {
    dispEntry(CAMERA_3D, 1000.0f, 0, peach_capture, NULL);
    return EVT_RETURN_BLOCK;
}

/**
 * Script function: allocates the hologram's work area
 * and its two texture buffers (RGBA8 colour and Z8 depth, 0x260 by 0x1E0 each) and clears its ready
 * and flag words.
 */
s32 horogram_init(EventEntry* event, BOOL isFirstCall) {
    u32 size = GXGetTexBufferSize(0x260, 0x1E0, GX_TF_RGBA8, GX_FALSE, 0);

    horogram_wp = _mapAlloc(sizeof(HorogramWork));
    horogram_wp->buf0 = _mapAlloc(size);
#if VERSION >= VERSION_US
    size = GXGetTexBufferSize(0x260, 0x1E0, GX_TF_Z8, GX_FALSE, 0);
    horogram_wp->buf1 = _mapAlloc(size);
#else
    horogram_wp->buf1 = _mapAlloc(GXGetTexBufferSize(0x260, 0x1E0, GX_TF_Z8, GX_FALSE, 0));
#endif
    horogram_wp->ready = 0;
    horogram_wp->flags = 0;
    return EVT_RETURN_DONE;
}

/** Script function: turns the hologram flag mask args[1] on (args[0] nonzero) or off. */
s32 horogram_flag_onoff(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);

    if (on) {
        horogram_wp->flags |= mask;
    } else {
        horogram_wp->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

/** Script function: returns the dance's center point (dance_dt.cx, dance_dt.cy) in args[0] and args[1]. */
s32 get_center(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], dance_dt.cx);
    evtSetFloat(event, args[1], dance_dt.cy);
    return EVT_RETURN_DONE;
}

/** Script function: returns the dance's local rotation angle (radians) in args[0]. */
s32 get_local_rotate(EventEntry* event, BOOL isFirstCall) {
    evtSetFloat(event, event->args[0], dance_dt.rot);
    return EVT_RETURN_DONE;
}

/** Script function: sets the dance's local rotation angle to args[0] (radians), wrapped into 0 to 2 pi. */
s32 set_local_rotate(EventEntry* event, BOOL isFirstCall) {
    f32 r;

    dance_dt.rot = evtGetFloat(event, event->args[0]);
    r = dance_dt.rot;
    // Adjust by at most one revolution rather than applying a full modulo.
    if (r > 6.2831855f) {
        dance_dt.rot = r - 6.2831855f;
    }
    if (dance_dt.rot < 0.0f) {
        dance_dt.rot += 6.2831855f;
    }
    return EVT_RETURN_DONE;
}

/** Script function: returns the current dance step (0 to 2) in args[0]. */
s32 dance_get_step(EventEntry* event, BOOL isFirstCall) {
    evtSetValue(event, event->args[0], dance_dt.step);
    return EVT_RETURN_DONE;
}

/**
 * Script function: resets the dance data: flags cleared, center at (0, -80),
 * rotation -0.471 radians (-27 degrees), step 0, waltz path index 0.
 */
s32 dance_data_init(EventEntry* event, BOOL isFirstCall) {
    dance_dt.flags = 0;
    dance_dt.cx = 0.0f;
    dance_dt.cy = -80.0f;
    dance_dt.rot = -0.4712389f;
    dance_dt.step = 0;
    dance_dt.movePos = 0;
    return EVT_RETURN_DONE;
}

/**
 * Script function: turns the dance flag mask args[0] on (args[1] nonzero) or off.
 * Flags: 1 = path runs backwards, 2 = action in progress, 4 = couple moving, 8 = stumbling, 4096,
 * 8192 and 16384 = button bubbles shown.
 */
s32 dance_flag_onoff(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 mask = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);

    if (on) {
        dance_dt.flags |= mask;
    } else {
        dance_dt.flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

/** Script function: returns 1 in args[1] if any bit of the dance flag mask args[0] is set, else 0. */
s32 dance_flag_check(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 mask = evtGetValue(event, args[0]);

    if (dance_dt.flags & mask) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/** Script function: toggles the dance flag 1 (the waltz path runs backwards). */
s32 dance_reverse(EventEntry* event, BOOL isFirstCall) {
    if (dance_dt.flags & 1) {
        dance_dt.flags &= ~1;
    } else {
        dance_dt.flags |= 1;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function: moves the dance center from (args[0], args[1]) to (args[2], args[3]) over args[4] ms with
 * smooth interpolation (flag 4 while moving), then advances the dance step modulo 3. Blocks until done.
 */
s32 move_center(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    f32 x0 = evtGetFloat(event, args[0]);
    f32 z0 = evtGetFloat(event, args[1]);
    f32 x1 = evtGetFloat(event, args[2]);
    f32 z1 = evtGetFloat(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    OSTime now = gp->renderTime;
    s32 elapsed;

    if (isFirstCall) {
        dance_dt.flags |= 4;
        event->userdata[0] = ((u64)now >> 32) & 0xFFFFFFFF;
        event->userdata[1] = now & 0xFFFFFFFF;
        return EVT_RETURN_BLOCK;
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(now, event->userdata[1]));
    if (elapsed < frames) {
        dance_dt.cx = intplGetValue(EASING_SINE_IN_OUT, x0, x1, elapsed, frames);
        dance_dt.cy = intplGetValue(EASING_SINE_IN_OUT, z0, z1, elapsed, frames);
        return EVT_RETURN_BLOCK;
    }
    dance_dt.cx = x1;
    dance_dt.cy = z1;
    dance_dt.flags &= ~4;
    if (++dance_dt.step >= 3) {
        dance_dt.step = 0;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function: moves the waltz path pointer one point forward (or backward in reverse mode) around the 15
 * points of move_pos_list and returns that point in args[0] and args[1].
 */
s32 compute_move_pos(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;

    if (!(dance_dt.flags & 1)) {
        dance_dt.movePos = (dance_dt.movePos + 1) % 15;
    } else if (dance_dt.movePos != 0) {
        dance_dt.movePos--;
    } else {
        dance_dt.movePos = 14;
    }
    evtSetFloat(event, args[0], move_pos_list[dance_dt.movePos][0]);
    evtSetFloat(event, args[1], move_pos_list[dance_dt.movePos][1]);
    return EVT_RETURN_DONE;
}

/**
 * Script function: converts the dance center args[0],
 * args[1] to Peach's position by offsetting 5.5 units along the dance rotation;
 * returns it in args[2] and args[3].
 */
s32 compute_peach_pos(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    f32 s = sin(dance_dt.rot);
    f32 c = cos(dance_dt.rot);

    evtSetFloat(event, args[2], x + 5.5f * s);
    evtSetFloat(event, args[3], z - 5.5f * c);
    return EVT_RETURN_DONE;
}

/**
 * Script function: converts the dance center args[0], args[1] to the hologram partner's position,
 * 5.5 units on the opposite side from Peach; returns it in args[2] and args[3].
 */
s32 compute_horogram_pos(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    f32 s = sin(dance_dt.rot);
    f32 c = cos(dance_dt.rot);

    evtSetFloat(event, args[2], x - 5.5f * s);
    evtSetFloat(event, args[3], z + 5.5f * c);
    return EVT_RETURN_DONE;
}

/** Script function: blocks while a dance action (flag 2) is in progress. */
s32 wait_action_end(EventEntry* event, BOOL isFirstCall) {
    return (dance_dt.flags & 2) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** Script function: blocks until a dance action (flag 2) begins. */
s32 wait_action_begin(EventEntry* event, BOOL isFirstCall) {
    return (dance_dt.flags & 2) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Script function: blocks while the couple is moving (flag 4). */
s32 wait_move_end(EventEntry* event, BOOL isFirstCall) {
    return (dance_dt.flags & 4) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/**
 * Script function: reads the controller's new button presses
 * and compares them with the expected button args[0] (1 = A, 2 = B, 0 = X).
 * Returns in args[1] 0 for nothing, 1 for the right button alone and 2 for any wrong button.
 */
s32 btn_check(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 dir = evtGetValue(event, args[0]);
    u32 btn = keyGetButtonTrg(0);
    s32 a = btn & PAD_BUTTON_A;
    s32 b = btn & PAD_BUTTON_B;
    s32 x = btn & PAD_BUTTON_X;
    s32 ret = 0;

    // A correct button combined with another dance button remains undecided.
    switch (dir) {
    case 1:
        if (a && !(b | x)) {
            ret = 1;
        } else if (!a && (b | x)) {
            ret = 2;
        }
        break;
    case 2:
        if (b && !(a | x)) {
            ret = 1;
        } else if (!b && (a | x)) {
            ret = 2;
        }
        break;
    case 0:
        if (x && !(a | b)) {
            ret = 1;
        } else if (!x && (a | b)) {
            ret = 2;
        }
        break;
    default:
        if (a | b | x) {
            ret = 2;
        }
        break;
    }
    evtSetValue(event, args[1], ret);
    return EVT_RETURN_DONE;
}

/**
 * Script function: sets the NPC named in args[0] to (args[1], args[2], args[3]),
 * both its current and previous position.
 */
s32 set_npc_pos(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    NpcEntry* npc = npcNameToPtr(name);

    npc->position.x = npc->prevPosition.x = x;
    npc->position.y = npc->prevPosition.y = y;
    npc->position.z = npc->prevPosition.z = z;
    return EVT_RETURN_DONE;
}

/** Script function: after a stumble puts Peach back into pose P_O_F_1 over 20 frames. */
s32 peach_dance_after_fail(EventEntry* event, BOOL isFirstCall) {
    marioChgPoseTime("P_O_F_1", 20);
    return EVT_RETURN_DONE;
}

/**
 * Script function: creates a speech-bubble effect for the button prompt (effPeachFukidashiEntry):
 * args[0] picks the bubble type, args[1] gives the effect a name,
 * args[2] names the NPC it is attached to ("me" means the script's own NPC) and args[7] is the angle.
 */
s32 evt_fuki_btn(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    const char* npcName = (const char*)evtGetValue(event, args[2]);
    f32 x = evtGetFloat(event, args[3]);
    f32 y = evtGetFloat(event, args[4]);
    f32 z = evtGetFloat(event, args[5]);
    f32 size = evtGetFloat(event, args[6]);
    f32 angle = evtGetFloat(event, args[7]);
    f32 unk = evtGetFloat(event, args[8]);
    NpcEntry* npc;
    EffectEntry* eff;

    if (strcmp(npcName, "me") == 0) {
        npc = event->ownerNPC;
    } else {
        npc = npcNameToPtr(npcName);
    }
    eff = effPeachFukidashiEntry(type, npc, 10000, 0.0f, 0.0f, 0.0f, 0.0f, angle);
    if (name != NULL && strcmp(name, "") != 0) {
        effSetName(eff, name);
    }
    return EVT_RETURN_DONE;
}

/** Script function: marks the named button bubble effect (args[0]) with the cross used for wrong presses. */
s32 evt_fuki_btn_set_batu(EventEntry* event, BOOL isFirstCall) {
    effSetPeachFukiBatu((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/** Suffix added to a pose name to get the matching pose of the mirrored hologram. */
static const char horogram_pose_suffix[] = "R";
/**
 * Script function: sets the pose args[0] on the hologram NPC
 * and the matching mirrored pose (name plus "R") on the second hologram.
 */
s32 horogram_set_pose(EventEntry* event, BOOL isFirstCall) {
    char name[24];
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    NpcEntry* npc = npcNameToPtr("\203z\203\215\203O\203\211\203\200");
    NpcEntry* npc2 = npcNameToPtr("\203z\203\215\203O\203\211\203\2002");
    MarioWork* mario = marioGetPtr();

    animPoseSetAnim(npc->poseId, pose, TRUE);
    strcpy(name, pose);
    strcat(name, horogram_pose_suffix);
    animPoseSetAnim(npc2->poseId, name, TRUE);
    return EVT_RETURN_DONE;
}

