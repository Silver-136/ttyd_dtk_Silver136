#pragma defer_codegen on
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "party.h"
#include "cam_shift.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_yuugijou.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "mario/mario_motion.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/icondrv.h"
#include "evt/evt_movefloor.h"
#include "yuu.h"

#ifdef VERSION_G8ME01
#define YUU_ICON_OFS 8
#else
#define YUU_ICON_OFS 0
#endif
typedef struct HikokiWork { u32 flags; s32 phase; s32 coins; s32 result; s32 monteCount; EventEntry* viewEvent; } HikokiWork;
s32 yuucounter_get_data(void);
void yuucounter_flag_on(s32 flag);
#ifdef VERSION_G8ME01
USER_FUNC(yuuminigame1_floor_chk);
USER_FUNC(yuuminigame1_floor_on_off);
#endif
typedef struct MonteSub { s32 unk0; Vec pos; f32 scale; u8 unk14[0x38 - 0x14]; } MonteSub;
typedef struct Monte { u8 hidden; u8 unk1; s16 num; u8 state; u8 unk5[3]; MonteSub sub; MiniGameWork* wp; } Monte;
typedef struct MonteTable { f32 x0; f32 x1; f32 y0; f32 y1; s32 num; } MonteTable;
static void* yuunpc_wp;
static Monte* monte_p;
#define MONTETABLE (montetable)
static void monte_disp(CameraId camera, void* param);

USER_FUNC(eff_call_minigame);
USER_FUNC(evt_change_plane);
USER_FUNC(evt_yuucounter_alloc);
USER_FUNC(evt_yuucounter_free);
USER_FUNC(evt_yuuminigame_alloc);
USER_FUNC(evt_yuuranking_alloc);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(evt_yuutitle_alloc);
USER_FUNC(evt_yuutitle_free);
static USER_FUNC(fall_rtn);
static USER_FUNC(re_fall_rtn);
USER_FUNC(yuu_00_cam_shift_reset);
USER_FUNC(yuu_atai_init);
USER_FUNC(yuu_atai_pop);
USER_FUNC(yuu_atai_push);
USER_FUNC(yuuminigame1_opening_chk);
USER_FUNC(yuuminigame1_opening_on_off);
USER_FUNC(yuuminigame1_set_phase);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuuranking_get_monte);
USER_FUNC(yuuranking_wait_button);
#ifdef VERSION_G8ME01
#endif
BOOL yuuminigame1_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, HikokiWork* data);
void yuuminigame_disp_1hikoki(CameraId camera, void* param);
void yuuminigame_main_1hikoki(EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_1hikoki(EventEntry* event, BOOL isFirstCall);

static MonteTable montetable[4];
static EvtWord minigame_view_monte[4];
EvtWord yuu_00_kaijo_init[12];
EvtWord yuu_00_kaijo_init2[12];
EvtWord light_sub_event[497];
EvtWord minigame01_main[35];
EvtWord minigame01_start_cam[40];
EvtWord minigame01_start_cam_cancel[20];
#ifdef VERSION_G8ME01
EvtWord minigame01_start[1504];
#else
EvtWord minigame01_start[355];
#endif
#ifdef VERSION_G8ME01
EvtWord minigame01_end[224];
#else
EvtWord minigame01_end[236];
#endif
#ifdef VERSION_G8ME01
EvtWord yuu_00_init_evt[437];
#else
EvtWord yuu_00_init_evt[11];
#endif

static MonteTable montetable[] = {
    {-1200.0f, -800.0f, 450.0f, 200.0f, 2},
    {-800.0f, -300.0f, 300.0f, 100.0f, 2},
    {-300.0f, 200.0f, 200.0f, 100.0f, 1},
    {0},
};

static EVT_BEGIN(minigame_view_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(minigame01_floor_delete)
    CALL(yuuminigame1_floor_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(yuuminigame1_floor_on_off, LW(0), 0)
        CALL(yuuminigame_get_dataadr, 1, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 2, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 3, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 4, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 5, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
    END_IF
    RETURN
EVT_END()
#endif

EVT_BEGIN(yuu_00_kaijo_init)
    CALL(evt_mario_set_pos, -1550, 400, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(yuu_00_kaijo_init2)
    CALL(evt_mario_set_pos, -1350, 400, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(light_sub_event)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 96)
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_NOT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), 30)
                SET_VALUE(LW(1), 1)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 60)
                SET_VALUE(LW(1), 2)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 78)
                SET_VALUE(LW(1), 3)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            IF_INT_EQUAL(LW(0), 96)
                SET_VALUE(LW(0), 0)
                SET_VALUE(LW(1), 0)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            SWITCH(LW(1))
                CASE_EQUAL(0)
                SET_VALUE(LW(3), 35)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 30)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(1)
                SET_VALUE(LW(3), 35)
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(2)
                SET_VALUE(LW(3), 0)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 18)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(3)
                SET_VALUE(LW(3), 0)
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
            END_SWITCH
            ADD(LW(0), 1)
            ADD(LW(2), 1)
        END_LOOP
        WAIT_MS(300)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(5), LW(3))
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(2), 18)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(3), 0)
            SUBTRACT(LW(3), LW(5))
            MULTIPLY(LW(3), LW(2))
            DIVIDE(LW(3), 18)
            ADD(LW(3), LW(5))
            CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
            ADD(LW(2), 1)
        END_LOOP
        SET_VALUE(LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 30)
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_NOT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), 30)
                SET_VALUE(LW(1), 1)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 60)
                SET_VALUE(LW(1), 2)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 78)
                SET_VALUE(LW(1), 3)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            IF_INT_EQUAL(LW(0), 96)
                SET_VALUE(LW(0), 0)
                SET_VALUE(LW(1), 0)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            SWITCH(LW(1))
                CASE_EQUAL(0)
                SET_VALUE(LW(3), 35)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 30)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(1)
                SET_VALUE(LW(3), 35)
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(2)
                SET_VALUE(LW(3), 0)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 18)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(3)
                SET_VALUE(LW(3), 0)
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
            END_SWITCH
            ADD(LW(0), 1)
            ADD(LW(2), 1)
        END_LOOP
        WAIT_MS(300)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(5), LW(3))
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(2), 18)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(3), 0)
            SUBTRACT(LW(3), LW(5))
            MULTIPLY(LW(3), LW(2))
            DIVIDE(LW(3), 18)
            ADD(LW(3), LW(5))
            CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
            ADD(LW(2), 1)
        END_LOOP
        SET_VALUE(LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(minigame01_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(yuu_00_kaijo_init))
    CALL(evt_yuuminigame_alloc, LW(0), 0)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL_CHILD(PTR(light_sub_event))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame1_set_phase, LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame01_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-1230.0f), FLOAT(360.0f), FLOAT(1140.0f), FLOAT(-1230.0f), FLOAT(160.0f), FLOAT(-10.0f), 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, FLOAT(700.0f), FLOAT(360.0f), FLOAT(1140.0f), FLOAT(700.0f), FLOAT(160.0f), FLOAT(-10.0f), 6000, 0)
    WAIT_MS(1000)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame1"))
    WAIT_MS(3000)
    CALL(evt_yuutitle_free)
    WAIT_MS(2000)
    RETURN
EVT_END()

EVT_BEGIN(minigame01_start_cam_cancel)
    CALL(evt_seq_wait, 2)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_00_kaijo_init2))
    RETURN
EVT_END()

EVT_BEGIN(minigame01_start)
    CALL(yuu_atai_init)
    CALL(yuu_atai_push, LW(0))
    CALL(evt_map_playanim, STRING("neon_01"), 1, 0)
#ifdef VERSION_G8ME01
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_01"))
    SET_VALUE(LW(1), STRING("A_dai_01"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), STRING("S_dai_01_k"))
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
#ifdef VERSION_G8ME01
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 500)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 1, LW(15))
#ifdef VERSION_G8ME01
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_02"))
    SET_VALUE(LW(1), STRING("A_dai_02"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), STRING("S_dai_02_k"))
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
#ifdef VERSION_G8ME01
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 500)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 2, LW(15))
#ifdef VERSION_G8ME01
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_03"))
    SET_VALUE(LW(1), STRING("A_dai_03"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
#ifdef VERSION_G8ME01
        SET_VALUE(LW(6), 4)
#else
    SET_VALUE(LW(6), 3)
#endif
    SET_VALUE(LW(7), STRING("S_dai_03_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
#ifdef VERSION_G8ME01
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 3, LW(15))
#ifdef VERSION_G8ME01
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_04"))
    SET_VALUE(LW(1), STRING("A_dai_04"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), STRING("S_dai_04_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
#ifdef VERSION_G8ME01
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 4, LW(15))
#ifdef VERSION_G8ME01
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_05"))
    SET_VALUE(LW(1), STRING("A_dai_05"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), STRING("S_dai_05_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
#ifdef VERSION_G8ME01
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 5, LW(15))
    CALL(yuu_atai_pop, LW(0))
#ifdef VERSION_G8ME01
    CALL(yuuminigame1_floor_on_off, LW(0), 1)
#endif
    CALL(yuuminigame1_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame01_start_cam), LW(8))
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
                    CALL_CHILD(PTR(minigame01_start_cam_cancel))
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame1_opening_on_off, LW(0), 0)
        CALL_CHILD(PTR(yuu_00_kaijo_init2))
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
    END_IF
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MINIGAME1"))
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_1"))
    WAIT_MS(400)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(600)
    CALL(evt_map_playanim, STRING("neon_01"), 2, 0)
    SET_VALUE(GF(0), 0)
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)
    CALL(evt_change_plane)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    WAIT_MS(1000)
    CALL(evt_yuucounter_alloc, 0, 70, 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame01_end)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
#ifdef VERSION_G8ME01
    CALL(yuu_atai_push, LW(0))
    CALL(yuu_atai_push, LW(1))
    CALL(yuu_atai_push, LW(2))
    SET_VALUE(LW(0), LW(3))
    CALL_CHILD(PTR(minigame01_floor_delete))
    CALL(yuu_atai_pop, LW(2))
    CALL(yuu_atai_pop, LW(1))
    CALL(yuu_atai_pop, LW(0))
#else
    CALL(yuuminigame_get_dataadr, 1, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 2, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 3, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 4, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 5, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
#endif
    CALL(evt_mario_adjust_dir)
    IF_INT_LESS(LW(0), 0)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        CALL(evt_mario_set_pose, STRING("M_I_S"))
        WAIT_MS(3800)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        WAIT_MS(200)
        ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(500)
        IF_INT_LESS(LW(1), 1)
            CALL(evt_mario_set_pose, STRING("M_X_1"))
            ELSE
            IF_INT_GREATER(LW(1), 1)
                CALL(evt_mario_set_pose, STRING("M_V_2"))
                ELSE
                CALL(evt_mario_set_pose, STRING("M_I_1"))
            END_IF
        END_IF
        WAIT_MS(3500)
    END_IF
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 0, 0, LW(0), LW(2), LW(1), 0)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 0, 1, LW(0), LW(2), LW(1), 0)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
        ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_00_00"))
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
    CALL(evt_fade_out, 1000)
    CALL(re_fall_rtn)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_00_kaijo_init2))
    CALL(yuu_00_cam_shift_reset)
    WAIT_MS(500)
    CALL_CHILD(PTR(light_sub_event))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

EVT_BEGIN(yuu_00_init_evt)
    CALL(evt_map_playanim, STRING("neon_02"), 1, 0)
    SPAWN_EVENT(PTR(minigame01_main))
    CALL_CHILD(PTR(evt_yuunpc_minigame1))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_1"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_2"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_3"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_4"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_5"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_6"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_7"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("saka_k_8"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_mon"), 16384)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_mon"), 16384)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_mon"), 16384)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_mon"), 16384)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_mon"), 16384)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_mon"), 16384)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_1"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_2"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_3"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_4"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_5"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_6"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka_k_7"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("i_saka_k_8"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_mon"), 16384)
    END_SWITCH
#endif
    RETURN
EVT_END()

USER_FUNC(yuu_00_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}

static const char yuu00_gnd_02[] = "A_ground_02";
static const char yuu00_gnd_05[] = "A_ground_05";
static const char yuu00_gnd_03[] = "A_ground_03";
static const char yuu00_gnd_06[] = "A_ground_06";
static const char yuu00_gnd_04[] = "A_ground_04";

static inline BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

static inline void monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                iconDispGx(&monte[i].sub.pos, 0, 0x13F + YUU_ICON_OFS, 1.0f);
            }
        }
    }
}

#define randRange(d) (irand(100.0f * (d)) / 100.0f)

static USER_FUNC(fall_rtn) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    BOOL exists = FALSE;
    HikokiWork* data = wp->data;
    s32 total;
    s32 i;
    s32 t;
    s32 k;
    SfxNameWord sfx;
    s32 j;
    MonteSub* sub;
    f32 lo;
    f32 hi;
    Vec v;
    Monte* m;

    (void)&get_monte_check;
    if (isFirstCall) {
        if (monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; MONTETABLE[i].num; i++) {
            total += MONTETABLE[i].num;
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
            sub->pos.z = 0.0f;
            do {
                if (MONTETABLE[t].x0 < MONTETABLE[t].x1) {
                    lo = MONTETABLE[t].x0;
                    hi = MONTETABLE[t].x1;
                } else {
                    lo = MONTETABLE[t].x1;
                    hi = MONTETABLE[t].x0;
                }
                hi -= lo;
                sub->pos.x = lo + randRange(hi);
                if (MONTETABLE[t].y0 < MONTETABLE[t].y1) {
                    lo = MONTETABLE[t].y0;
                    hi = MONTETABLE[t].y1;
                } else {
                    lo = MONTETABLE[t].y1;
                    hi = MONTETABLE[t].y0;
                }
                hi -= lo;
                sub->pos.y = lo + randRange(hi);
                for (j = 0; j < k; j++) {
                    PSVECSubtract(&sub->pos, &(sub - (j + 1))->pos, &v);
                    if (PSVECMag(&v) < 20.0f) {
                        break;
                    }
                }
            } while (j != k);
            if (++k >= MONTETABLE[t].num) {
                k = 0;
                t++;
            }
            sub->scale = 1.0f;
        }
    }
    for (i = 0; i < monte_p->num; i++) {
        sfx = (0, (SfxNameWord)"SFX_EVT_GAME_MONTE_COUNT1");
        m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (get_monte_check(&m->sub.pos, &marioGetPtr()->position) == 1) {
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

static USER_FUNC(re_fall_rtn) {
    HikokiWork* data;

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

USER_FUNC(evt_change_plane) {
    marioChgMot(MARIO_MOTION_PLANE);
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame1_set_phase) {
    EvtWord* args = event->args;
    HikokiWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame1_opening_on_off) {
    EvtWord* args = event->args;
    HikokiWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame1_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((HikokiWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
USER_FUNC(yuuminigame1_floor_on_off) {
    EvtWord* args = event->args;
    u32* flags = &((HikokiWork*)((MiniGameWork*)evtGetValue(event, args[0]))->data)->flags;

    if (evtGetValue(event, args[1])) {
        *flags |= 2;
    } else {
        *flags &= ~2;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame1_floor_chk) {
    EvtWord* args = event->args;
    u32* flags = &((HikokiWork*)((MiniGameWork*)evtGetValue(event, args[0]))->data)->flags;

    if (*flags & 2) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

#endif

void yuuminigame_init_1hikoki(EventEntry* event, BOOL isFirstCall) {
    HikokiWork* data = ((MiniGameWork*)event->lwData[0])->data;

    data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
}

void yuuminigame_main_1hikoki(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    HikokiWork* data = wp->data;

    switch (data->phase) {
        case 0:
            break;
        case 4:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame01_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            if (yuuminigame1_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 3:
            wp->subEvent = evtEntry(minigame01_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
            wp->subEvent->lwData[1] = data->result;
            wp->subEvent->lwData[2] = data->monteCount;
#ifdef VERSION_G8ME01
            wp->subEvent->lwData[3] = event->lwData[0];
#endif
            data->phase++;
            break;
    }
}

void yuuminigame_disp_1hikoki(CameraId camera, void* param) {
    monteCountDisp();
}

#pragma dont_inline on
BOOL yuuminigame1_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, HikokiWork* data) {
    MarioWork* mario = marioGetPtr();

#ifdef VERSION_G8ME01
    EventEntry* sub;

    if (mario->hitobjStandOn != NULL) {
        sub = evtEntry(minigame01_floor_delete, 0, 0);
        sub->lwData[0] = event->lwData[0];
        if ((u16)((mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) ? mario->prevMotionId : mario->currMotionId) != MARIO_MOTION_PLANE) {
#else
    if (mario->hitobjStandOn != NULL && (u16)((mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) ? mario->prevMotionId : mario->currMotionId) != MARIO_MOTION_PLANE) {
#endif
        data->result = 1;
        data->coins = yuucounter_get_data();
        if (!strcmp("A_dai_01", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_02", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_03", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_04", hitGetName(mario->hitobjStandOn))) {
#ifdef VERSION_G8ME01
            data->result = 2;
#else
            data->result = 3;
#endif
        }
        if (!strcmp("A_dai_05", hitGetName(mario->hitobjStandOn))) {
#ifdef VERSION_G8ME01
            data->result = 3;
#else
            data->result = 5;
#endif
        }
        if (!strcmp(yuu00_gnd_02, hitGetName(mario->hitobjStandOn))) {
            data->result = -5;
        }
        if (!strcmp(yuu00_gnd_05, hitGetName(mario->hitobjStandOn))) {
            data->result = -5;
        }
        if (!strcmp(yuu00_gnd_03, hitGetName(mario->hitobjStandOn))) {
            data->result = -10;
        }
        if (!strcmp(yuu00_gnd_06, hitGetName(mario->hitobjStandOn))) {
            data->result = -15;
        }
        if (!strcmp(yuu00_gnd_04, hitGetName(mario->hitobjStandOn))) {
            data->result = -20;
        }
        marioKeyOff();
        partyKeyOff();
        yuucounter_flag_on(2);
        return TRUE;
#ifdef VERSION_G8ME01
        }
#endif
    }
    return FALSE;
}
#pragma dont_inline reset
