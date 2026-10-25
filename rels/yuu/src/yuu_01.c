#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "party.h"
#include "mario/mario_motion.h"
#include "cam_shift.h"
#include "evt/evt_map.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_sub.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mot/mot_ship.h"
#include "evt/evt_yuugijou.h"
#include "driver/mapdrv.h"
#include "rels/yuu/include/yuu.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "icon_ids.h"
#include "evt/evt_movefloor.h"
#include "yuu.h"
#ifdef VERSION_G8ME01
#endif

typedef struct FuneWork {
    u32 flags;
    s32 phase;
    s32 coins;
    s32 result;
    s32 index;
    s32 state;
    f32 dist;
    Vec dir;
    s32 timer;
    f32 y;
    f32 x;
    s32 monteCount;
    EventEntry* viewEvent;
} FuneWork;
typedef struct MonteSub {
    s32 unk0;
    Vec pos;
    f32 scale;
    u8 unk14[0x38 - 0x14];
} MonteSub;
typedef struct Monte {
    u8 hidden;
    u8 unk1;
    s16 num;
    u8 state;
    u8 unk5[3];
    MonteSub sub;
    MiniGameWork* wp;
} Monte;
typedef struct MonteTable {
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    s32 num;
} MonteTable;
static void* yuunpc_wp;
static Monte* monte_p;
static void monte_disp(CameraId camera, void* param);
static inline BOOL monte_check_inl(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

static inline f32 randRange(f32 d) { return irand(100.0f * d) / 100.0f; }

USER_FUNC(eff_call_minigame);
USER_FUNC(evt_yuucounter_alloc);
USER_FUNC(evt_yuucounter_free);
USER_FUNC(evt_yuuminigame_alloc);
USER_FUNC(evt_yuuranking_alloc);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(evt_yuutitle_alloc);
USER_FUNC(evt_yuutitle_free);
static USER_FUNC(fall_rtn);
static USER_FUNC(re_fall_rtn);
USER_FUNC(yuu_01_cam_shift_reset);
USER_FUNC(yuuminigame2_get_name);
USER_FUNC(yuuminigame2_opening_chk);
USER_FUNC(yuuminigame2_opening_on_off);
USER_FUNC(yuuminigame2_set_phase);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuuranking_get_monte);
USER_FUNC(yuuranking_wait_button);
BOOL yuuminigame2_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, FuneWork* data);
void yuuminigame_main_2fune(EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_2fune(EventEntry* event, BOOL isFirstCall);
static BOOL get_monte_check(Vec* a, Vec* b);

const char* s_nametbl[21];
const char* a2_nametbl[21];
const char* a_nametbl[21];
s32 turn_tbl[22];
s32 hoko_tbl[22];
static MonteTable montetable[6];
static EvtWord minigame_view_monte[4];
EvtWord yuu_01_kaijo_init[12];
EvtWord yuu_01_kaijo_init2[12];
EvtWord minigame02_main[37];
EvtWord minigame02_start_cam[40];
EvtWord minigame02_start_cam_cancel[17];
EvtWord minigame02_start[101];
EvtWord minigame02_end[172];
EvtWord yuu_01_init_evt[VERSION_PICK(1076, 1166)];

const char* s_nametbl[] = {
    NULL, "S_dai_uzu1", "S_dai_uzu2", "S_dai_uzu3", "S_dai_uzu4", "S_uzu1", "S_uzu2", "S_uzu3", "S_uzu4", "S_uzu5",
    "S_uzu6", "S_uzu7", "S_uzu8", "S_taru01", "S_taru02", "S_taru03", "S_taru04", "S_taru05", "S_taru06",
    "S_taru07", "S_taru08",
};

const char* a2_nametbl[] = {
    NULL,
    "polySurface439_q00000",
    "polySurface439_q00001",
    "polySurface439_q00002",
    "polySurface439_q00003",
    "polySurface439_q00008",
    "polySurface439_q00010",
    "polySurface439_q00012",
    "polySurface439_q00014",
    "polySurface439_q00016",
    "polySurface439_q00018",
    "polySurface439_q00020",
    "polySurface439_q00022",
    "A_taru01",
    "A_taru02",
    "A_taru03",
    "A_taru04",
    "A_taru05",
    "A_taru06",
    "A_taru07",
    "A_taru08",
};

const char* a_nametbl[] = {
    NULL, "A_dai_uzu1", "A_dai_uzu2", "A_dai_uzu3", "A_dai_uzu4", "A_uzu1", "A_uzu2", "A_uzu3", "A_uzu4", "A_uzu5",
    "A_uzu6", "A_uzu7", "A_uzu8", "A_taru01", "A_taru02", "A_taru03", "A_taru04", "A_taru05", "A_taru06",
    "A_taru07", "A_taru08",
};

s32 turn_tbl[] = {
    0, 5, -5, 5, -5, 10, -10, 10,
    -10, 10, -10, 10, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};

s32 hoko_tbl[] = {
    0, 0, 0, 0, 0, 45, 135, 45,
    135, 45, 45, 135, 180, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};

static MonteTable montetable[] = {
    {0.0f, 0.0f, 50.0f, 340.0f, 1},
    {250.0f, 250.0f, 50.0f, 340.0f, 1},
    {450.0f, 730.0f, -330.0f, 50.0f, 1},
    {-155.0f, -155.0f, -330.0f, -60.0f, 1},
    {150.0f, -155.0f, -330.0f, -60.0f, 1},
    {0},
};

static EVT_BEGIN(minigame_view_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

EVT_BEGIN(yuu_01_kaijo_init)
    CALL(evt_mario_set_pos, -800, 0, 200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(yuu_01_kaijo_init2)
    CALL(evt_mario_set_pos, -800, 0, 200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(minigame02_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(yuu_01_kaijo_init))
    CALL(evt_yuuminigame_alloc, LW(0), 1)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_BOAT_WATER1"), GW(11))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame2_set_phase, LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame02_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-560.0f), FLOAT(565.0f), FLOAT(1185.0f), FLOAT(-560.0f), FLOAT(75.0f), FLOAT(155.0f), 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, FLOAT(550.0f), FLOAT(565.0f), FLOAT(1185.0f), FLOAT(550.0f), FLOAT(75.0f), FLOAT(155.0f), 4000, 0)
    WAIT_MS(500)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame2"))
    WAIT_MS(3000)
    CALL(evt_yuutitle_free)
    WAIT_MS(500)
    RETURN
EVT_END()

EVT_BEGIN(minigame02_start_cam_cancel)
    CALL(evt_seq_wait, 2)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL_CHILD(PTR(yuu_01_kaijo_init2))
    RETURN
EVT_END()

EVT_BEGIN(minigame02_start)
    CALL(yuuminigame2_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame02_start_cam), LW(8))
        LOOP(0)
            IS_EVENT_RUNNING(LW(8), LW(9))
            IF_INT_EQUAL(LW(9), 0)
                CALL(evt_seq_wait, 2)
                BREAK_LOOP
                ELSE
                CALL(evt_key_get_buttontrg, 0, LW(9))
                IF_BITS_SET(LW(9), 768)
                    DELETE_EVENT(LW(8))
                    CALL(evt_seq_wait, 2)
                    CALL_CHILD(PTR(minigame02_start_cam_cancel))
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame2_opening_on_off, LW(0), 0)
        CALL_CHILD(PTR(yuu_01_kaijo_init2))
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
    END_IF
    CALL(evt_hitobj_onoff, STRING("taisaku"), 0, 0)
    CALL(evt_mario_mov_pos2, -660, 200, 120)
    CALL(evt_hitobj_onoff, STRING("taisaku"), 0, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MINIGAME1"))
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_yuucounter_alloc, 0, 70, 1, 120)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame02_end)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
    IF_INT_LESS(LW(0), 0)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        WAIT_MS(4000)
        ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(4000)
    END_IF
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 3, 0, LW(0), LW(2), LW(1), 120)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 3, 1, LW(0), LW(2), LW(1), 120)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
        ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_01_00"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF(1229), 1)
        CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
        CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
        RETURN
    END_IF
    MULTIPLY(LW(1), -1)
    CALL(yuugijou_add_montemedal, 0, LW(1))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(yuugijou_montemedal_disp_onoff, 0)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_sfxoff, GW(10))
    CALL(evt_snd_sfxoff, GW(11))
    CALL(evt_fade_out, 1000)
    CALL(re_fall_rtn)
    CALL_CHILD(PTR(yuu_01_kaijo_init2))
    CALL(yuu_01_cam_shift_reset)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_BOAT_WATER1"), GW(11))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

EVT_BEGIN(yuu_01_init_evt)
    CALL(evt_map_playanim, STRING("den1"), 1, 0)
    CALL(evt_map_playanim, STRING("den2"), 1, 0)
    CALL(evt_map_playanim, STRING("kumo"), 1, 0)
    CALL(evt_map_playanim, STRING("wakka"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("nagare"), 1, 0)
    CALL(yuuminigame2_get_name, 1, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -140)
    SET_VALUE(LW(3), 140)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 1, LW(15))
    CALL(yuuminigame2_get_name, 2, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -220)
    SET_VALUE(LW(3), 50)
    SET_VALUE(LW(4), 3500)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 2, LW(15))
    CALL(yuuminigame2_get_name, 3, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 20)
    SET_VALUE(LW(3), -60)
    SET_VALUE(LW(4), 800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 115)
    SET_VALUE(LW(9), -40)
    SET_VALUE(LW(10), 1600)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 3, LW(15))
    CALL(yuuminigame2_get_name, 4, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -50)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 800)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 4, LW(15))
    CALL(yuuminigame2_get_name, 5, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 5, LW(15))
    CALL(yuuminigame2_get_name, 6, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 6, LW(15))
    CALL(yuuminigame2_get_name, 7, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 7, LW(15))
    CALL(yuuminigame2_get_name, 8, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
    SET_VALUE(LW(3), 40)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 8, LW(15))
    CALL(yuuminigame2_get_name, 9, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 120)
    SET_VALUE(LW(3), -40)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 9, LW(15))
    CALL(yuuminigame2_get_name, 10, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 60)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 10, LW(15))
    CALL(yuuminigame2_get_name, 11, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 40)
    SET_VALUE(LW(3), -50)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 11, LW(15))
    CALL(yuuminigame2_get_name, 12, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 40)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 12, LW(15))
    CALL(yuuminigame2_get_name, 13, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -20)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 3600)
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 13, LW(15))
    CALL(yuuminigame2_get_name, 14, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 800)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 14, LW(15))
    CALL(yuuminigame2_get_name, 15, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -70)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 15, LW(15))
    CALL(yuuminigame2_get_name, 16, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 16, LW(15))
    CALL(yuuminigame2_get_name, 17, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 100)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 300)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 17, LW(15))
    CALL(yuuminigame2_get_name, 18, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 240)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 18, LW(15))
    CALL(yuuminigame2_get_name, 19, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -60)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 1000)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 19, LW(15))
    CALL(yuuminigame2_get_name, 20, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -25)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 20, LW(15))
    SPAWN_EVENT(PTR(minigame02_main))
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_yuunpc_minigame2)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    END_SWITCH
#else
    CALL_CHILD(PTR(evt_yuunpc_minigame2))
#endif
    RETURN
EVT_END()

BOOL yuuminigame2_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, FuneWork* data) {
    MarioWork* mario = marioGetPtr();
    Vec box[3];
    Mtx mtx;
    Vec pos;
    Vec tmp;
    Vec ofs;
    f32 angle;
    s32 i;
    s32 j;

    if (data->index == 0x15) {
        if ((mario->position.x <= -700.0f && mario->position.z <= 0.0f) || yuucounter_get_data() < 0) {
            data->coins = yuucounter_get_data();
            marioKeyOff();
            partyKeyOff();
            yuucounter_flag_on(2);
            return TRUE;
        }
    }
    box[2] = mario->position;
    box[1] = mario->position;
    box[0] = mario->position;
    box[0].x -= 12.5f;
    box[2].x += 12.5f;
    while (data->index < 1 || data->index >= 5) {
        for (i = 1; i < 5; i++) {
            mapObjGetPos(s_nametbl[i], &pos);
            pos.y = mario->position.y;
            for (j = 0; j < 3; j++) {
                if (PSVECSquareDistance(&box[j], &pos) <= 3306.25f) {
                    tmp = mario->position;
                    if (tmp.x == pos.x && tmp.z == pos.z) {
                        tmp.x += 1.0f;
                    }
                    data->index = i;
                    marioKeyOff();
                    partyKeyOff();
                    mario->flags |= MARIO_FLAG_FORCE_WALL;
                    mario->flags |= MARIO_FLAG_FLOORS_DISABLED;
                    data->state = 0;
                    PSVECSubtract(&tmp, &pos, &data->dir);
                    data->dir.y = 0.0f;
                    data->dist = PSVECMag(&data->dir);
                    PSVECNormalize(&data->dir, &data->dir);
                    data->timer = 120;
                    data->y = mario->position.y;
                    mario->dispFlags |= 0x100;
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_GAME_BOAT_UZUSIO1), &mario->position);
                    data->result--;
                    break;
                }
            }
            if (j < 3) {
                break;
            }
        }
        if (i < 5) {
            break;
        }
        {
            for (i = 5; i < 13; i++) {
                mapObjGetPos(s_nametbl[i], &pos);
                pos.y = mario->position.y;
                for (j = 0; j < 3; j++) {
                    if (PSVECSquareDistance(&box[j], &pos) <= 225.0f) {
                        if (!marioChkSwirlPower()) {
                            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_GAME_BOAT_UZUSIO2), &mario->position);
                            data->result--;
                        }
                        data->index = i;
                        marioSetSwirlPower(1000, 8.0f, irand(91) + hoko_tbl[data->index] - 45);
                        mario->dispFlags |= 0x100;
                        break;
                    }
                }
                if (j < 3) {
                    break;
                }
            }
        }
        if (i < 13) {
            break;
        }
        break;
    }
    if (data->index != 0x15) {
    angle = mario->displayDirection + turn_tbl[data->index];
    if (data->index >= 5 && data->index < 13) {
        if (!marioChkSwirlPower()) {
            data->index = 0x15;
            mario->dispFlags &= ~0x100;
            if (mario->displayDirection >= -90.0f && mario->displayDirection < 90.0f) {
                angle = 0.0f;
            } else {
                angle = 180.0f;
            }
        }
    } else if (data->index >= 1 && data->index < 5) {
        switch (data->state) {
            case 0:
                data->timer--;
                PSMTXRotRad(mtx, 'Y', 0.017453292f * turn_tbl[data->index]);
                PSMTXMultVec(mtx, &data->dir, &data->dir);
                data->dir.y = 0.0f;
                PSVECNormalize(&data->dir, &data->dir);
                PSVECScale(&data->dir, &ofs, data->timer >= 30 ? data->dist * ((data->timer - 30) / 90.0f) : 0.0f);
                mapObjGetPos(s_nametbl[data->index], &pos);
                PSVECAdd(&ofs, &pos, &ofs);
                mario->position.x = ofs.x;
                mario->position.z = ofs.z;
                mario->position.y = data->y - 30.0f * ((120 - data->timer) / 120.0f);
                if (data->timer <= 0) {
                    data->state++;
                    angle = 0.0f;
                    data->timer = 30;
                    mario->dispFlags &= ~0x100;
                    data->x = mario->position.x;
                }
                break;
            case 1:
                data->timer--;
                angle = 0.0f;
                mario->position.x = data->x + -100.0f * ((30 - data->timer) / 30.0f);
                mario->position.y = data->y - 30.0f;
                if (data->timer <= 0) {
                    data->state++;
                    data->timer = 30;
                }
                break;
            case 2:
                data->timer--;
                angle = 0.0f;
                mario->position.y = data->y - 30.0f * (data->timer / 30.0f);
                if (data->timer <= 0) {
                    data->index = 0x15;
                    marioKeyOn();
                    partyKeyOn();
                    mario->flags &= ~MARIO_FLAG_FORCE_WALL;
                    mario->flags &= ~MARIO_FLAG_FLOORS_DISABLED;
                }
                break;
        }
    }
    mario->displayDirectionTarget = reviseAngle(angle);
    mario->displayDirection = mario->displayDirectionTarget;
    }
    return FALSE;
}

void yuuminigame_disp_2fune(CameraId camera, void* param) {
    monteCountDisp();
}

void yuuminigame_main_2fune(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    FuneWork* data = wp->data;

    switch (data->phase) {
        case 0:
            break;
        case 5:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame02_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            data->index = 0x15;
            data->phase = 3;
            data->result = 0;
        case 3:
            if (yuuminigame2_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 4:
            wp->subEvent = evtEntry(minigame02_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
            if (data->result + 5 <= 0) {
                data->result = 1;
            } else {
                data->result += 0x105;
            }
            wp->subEvent->lwData[1] = data->result;
            wp->subEvent->lwData[2] = data->monteCount;
            data->phase++;
            break;
    }
}

void yuuminigame_init_2fune(EventEntry* event, BOOL isFirstCall) {
    FuneWork* data = ((MiniGameWork*)event->lwData[0])->data;

    data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
    marioChgShipMotion();
}

USER_FUNC(yuuminigame2_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((FuneWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame2_opening_on_off) {
    EvtWord* args = event->args;
    FuneWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame2_get_name) {
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (EvtWord)s_nametbl[i]);
    evtSetValue(event, args[2], (EvtWord)a_nametbl[i]);
    evtSetValue(event, args[3], turn_tbl[i]);
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame2_set_phase) {
    EvtWord* args = event->args;
    FuneWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(re_fall_rtn) {
    FuneWork* data;

    if (monte_p != NULL) {
        data = monte_p->wp->data;
        if (data->viewEvent != NULL) {
            evtDeleteID(data->viewEvent->eventId);
            data->viewEvent = NULL;
            _mapFree(monte_p);
            data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
            data->viewEvent->lwData[0] = (EvtWord)monte_p->wp;
        }
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(fall_rtn) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    BOOL exists = FALSE;
    FuneWork* data = wp->data;
    s32 total;
    s32 i;
    s32 t;
    s32 k;
    SfxNameWord sfx;
    s32 j;
    f32 lo;
    f32 hi;
    Vec v;
    Monte* m;
    MonteSub* sub;

    if (isFirstCall) {
        if (monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; montetable[i].num; i++) {
            total += montetable[i].num;
        }
        monte_p = _mapAlloc(total * sizeof(Monte));
        memset(monte_p, 0, total * sizeof(Monte));
        monte_p->num = total;
        monte_p->wp = wp;
        data->monteCount = 0;
        t = 0;
        k = 0;
        for (i = 0; i < monte_p->num; i++) {
            sub = &monte_p[i].sub;
            sub->pos.y = 0.0f;
            do {
                if (montetable[t].x0 < montetable[t].x1) {
                    lo = montetable[t].x0;
                    hi = montetable[t].x1;
                } else {
                    lo = montetable[t].x1;
                    hi = montetable[t].x0;
                }
                hi -= lo;
                sub->pos.x = lo + randRange(hi);
                if (montetable[t].y0 < montetable[t].y1) {
                    lo = montetable[t].y0;
                    hi = montetable[t].y1;
                } else {
                    lo = montetable[t].y1;
                    hi = montetable[t].y0;
                }
                hi -= lo;
                sub->pos.z = lo + randRange(hi);
                for (j = 0; j < k; j++) {
                    PSVECSubtract(&sub->pos, &(sub - (j + 1))->pos, &v);
                    if (PSVECMag(&v) < 20.0f) {
                        break;
                    }
                }
            } while (j != k);
            if (++k >= montetable[t].num) {
                k = 0;
                t++;
            }
            sub->scale = 1.0f;
        }
    }
    for (i = 0; i < monte_p->num; i++) {
        sfx = (SfxNameWord)"SFX_EVT_GAME_MONTE_COUNT1";
        m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (monte_check_inl(&m->sub.pos, &marioGetPtr()->position) == TRUE) {
                    data->monteCount++;
                    psndSFXOn_3D((const char*)sfx, &marioGetPtr()->position);
                    m->hidden = 1;
                    m->state = 1;
                }
                break;
            case 1:
                break;
        }
    }
    if (!exists) {
        dispEntry(CAMERA_3D, 0.0f, 1, monte_disp, monte_p);
    }
    return EVT_RETURN_BLOCK;
}

static void monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                iconDispGx(&monte[i].sub.pos, 0, ICON_13F, 1.0f);
            }
        }
    }
}

static BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

USER_FUNC(yuu_01_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}
