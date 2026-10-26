// NOTE: seq_gameOverExit/seq_gameOverInit and N_evt_snd_bgm_unpause live in the
// event/evt_snd.c split (0x80077EC4..0x80077F88), which is really the tail of this TU.
#include "driver/camdrv.h"
#include "driver/fadedrv.h"
#include "driver/seqdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "manager/evtmgr.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "nameent.h"
#include "pmario_sound.h"
#include "sound.h"
#include "system.h"
#include <string.h>
#include "seq/seq_gameover.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "statuswindow.h"
#include "countdown.h"
#include "cam_road.h"
#include "cam_shift.h"
#include "driver/npcdrv.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "seq/seq_mapchange.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"

typedef struct GameOverWork {
    EventEntry* evt; // 0x0
} GameOverWork;



USER_FUNC(evt_mario_dispflag_onoff);

static GameOverWork work;
static GameOverWork* wp = &work;

s32 fall_data[] = {
    200, 170, 140, 110, 80, 50, 20, 10, 26, 29, 30, 29, 26,
    10, 0, 12, 15, 16, 15, 11, 6, 0, 4, 3, 1, 0,
};

s32 fall_data2[] = {
    200, 170, 140, 110, 80, 50, 20, 0, 12, 15, 16, 15, 11, 6, 0, 4, 3, 1, 0,
};

EVT_BEGIN(_fall_char)
    SET_INT_READ_ADDR(PTR(fall_data))
    LOOP(26)
        READ_INT(LW(1))
        CALL(evt_mapobj_trans, 1, LW(0), 0, LW(1), 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(_fall_char_last)
    SET_INT_READ_ADDR(PTR(fall_data2))
    LOOP(19)
        READ_INT(LW(1))
        CALL(evt_mapobj_trans, 1, LW(0), 0, LW(1), 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(fall_gameover)
    SET_VALUE(LW(0), STRING("g"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(166)
#else
    WAIT_FRAMES(10)
#endif
    SET_VALUE(LW(0), STRING("a"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(333)
#else
    WAIT_FRAMES(20)
#endif
    SET_VALUE(LW(0), STRING("m"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(166)
#else
    WAIT_FRAMES(10)
#endif
    SET_VALUE(LW(0), STRING("e"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(166)
#else
    WAIT_FRAMES(10)
#endif
    SET_VALUE(LW(0), STRING("o"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(166)
#else
    WAIT_FRAMES(10)
#endif
    SET_VALUE(LW(0), STRING("v"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(166)
#else
    WAIT_FRAMES(10)
#endif
    SET_VALUE(LW(0), STRING("e2"))
    SPAWN_EVENT(_fall_char)
#ifdef VERSION_G8ME01
    WAIT_MS(500)
#else
    WAIT_FRAMES(30)
#endif
    SET_VALUE(LW(0), STRING("r"))
    SPAWN_EVENT_GET_ID(PTR(_fall_char_last), LW(10))
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#ifdef VERSION_G8ME01
    WAIT_MS(1000)
#else
    WAIT_FRAMES(60)
#endif
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(fall_gameover_es)
    SET_VALUE(LW(0), STRING("f"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("i"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(233)
    SET_VALUE(LW(0), STRING("n"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("d"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("e3"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("l"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(233)
    SET_VALUE(LW(0), STRING("j"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("u"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("e4"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(133)
    SET_VALUE(LW(0), STRING("g1"))
    SPAWN_EVENT(_fall_char)
    WAIT_MS(400)
    SET_VALUE(LW(0), STRING("o289"))
    SPAWN_EVENT_GET_ID(PTR(_fall_char_last), LW(10))
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    RETURN
    END
EVT_END()
#endif

EVT_BEGIN(evt_gameover)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_cam_road_reset)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_mario_clear_party, 1)
    WAIT_FRAMES(1)
    CALL(N_evt_snd_bgm_unpause)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_D_2"))
    CALL(evt_mario_set_dir, 180, 0, 0)
    CALL(evt_mario_adjust_dir)
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("g"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("o"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("v"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("r"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_trans, 1, STRING("f"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("i"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("n"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("d"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("e3"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("l"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("j"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("u"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("e4"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("g1"), 0, 200, 0)
        CALL(evt_mapobj_trans, 1, STRING("o289"), 0, 200, 0)
        ELSE
#endif
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("f"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("i"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("d"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("l"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("j"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("u"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("g1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("o289"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("g"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("a"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("m"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("e"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("o"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("v"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("e2"), 0, 200, 0)
    CALL(evt_mapobj_trans, 1, STRING("r"), 0, 200, 0)
#ifdef VERSION_G8ME01
    END_IF
#endif
    CALL(evt_cam_ctrl_onoff, 4, 1)
    CALL(evt_cam3d_evt_set, 0, 208, 653, 0, 91, -30, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("anm_hokori"), MAP_ANIM_PLAY_LOOP, 0)
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL_CHILD(fall_gameover_es)
        ELSE
#endif
    CALL_CHILD(fall_gameover)
#ifdef VERSION_G8ME01
    END_IF
#endif
    RETURN
EVT_END()

void seq_gameOverMain(SequenceWork* work) {
    CameraEntry* camera;
    MarioWork* mario;

    switch (work->state) {
        case 0:
            if (work->mapName == NULL) {
                GXColor color = {0, 0, 0, 255};
                fadeEntry(2, 1000, color);
                gp->areaFadeInType = FADE_OUT_BLACK;
                gp->areaFadeInDuration = 300;
            }
            mapPreLoad("sys_00");
            work->state++;
            break;
        case 1:
            if (!fadeIsFinish()) {
                break;
            }
            _relUnLoad();
            _unload(gp->currentMapName, NULL, NULL);
            strcpy(gp->currentAreaName + 0x10, gp->currentMapName);
            statusWinForceOff();
            statusWinDispOff();
            camera = camGetPtr(CAMERA_3D);
            camera->viewport[0] = 0.0f;
            camera->viewport[1] = 0.0f;
            camera->viewport[2] = gp->fbWidth;
            camera->viewport[3] = gp->efbHeight;
            camera = camGetPtr(CAMERA_2D);
            camera->viewport[0] = 0.0f;
            camera->viewport[1] = 0.0f;
            camera->viewport[2] = gp->fbWidth;
            camera->viewport[3] = gp->efbHeight;
            camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX;
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_INSTANT;
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
            fadeTecOff();
            fadeSoftFocusOff();
            npcClearDeadInfo();
            countDownEnd();
            nameEntReInit();
            work->state++;
            break;
        case 2:
            if (!mapPreLoad("sys_00")) {
                break;
            }
            mapLoad("sys_00");
            camera = camGetPtr(CAMERA_3D);
            mario = marioGetPtr();
            {
                camera->marioPos.x = 0.0f;
                camera->marioPos.y = 0.01f;
                camera->marioPos.z = 0.0f;
                camera->marioAt.x = camera->marioPos.x;
                camera->marioAt.y = camera->marioPos.y;
                camera->marioAt.z = camera->marioPos.z;
                mario->position = (Vec){0.0f, 0.0f, 0.0f};
                mario->camTarget = mario->position;
            }
            camRoadReset();
            camShiftReset();
            work->state++;
            break;
        case 3: {
            GXColor color = {0, 0, 0, 255};
            fadeEntry(31, 200, color);
            work->state++;
            break;
        }
        case 4:
            if (!fadeIsFinish()) {
                break;
            } else {
                GXColor color = {0, 0, 0, 255};
                fadeEntry(FADE_IN_BLACK, 1000, color);
            }
            marioReset();
            marioEntry();
            psndBGMOn(PSND_FADEOUT, (char*)PSND_BGM_BATTLE_LOSE1);
            if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                sound.streams[psbgm[0].streamId].flags |= 0x8000;
            }
            wp->evt = evtEntryType(evt_gameover, 0, 0, 0);
            work->state++;
            break;
        case 5:
            if (evtCheckID(wp->evt->eventId)) {
                if (!(keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_START))) {
                    break;
                }
            }
            {
                GXColor color = {0, 0, 0, 255};
                fadeEntry(36, 0, color);
            }
            psndBGMOff_f_d(PSND_FADEOUT, 4000, FALSE);
            work->state++;
            break;
        case 6:
            if (!fadeIsFinish()) {
                break;
            }
            _relUnLoad();
            _unload("sys_00", NULL, NULL);
            strcpy(gp->beroEnterName, "");
            strcpy(gp->currentMapName, "");
            strcpy(gp->currentAreaName, "");
            statusWinDispOn();
            camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
            if (gp->flags & GW_FLAG_E3) {
                seqSetSeq(SEQ_E3, NULL, NULL);
            } else {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
    }
}
