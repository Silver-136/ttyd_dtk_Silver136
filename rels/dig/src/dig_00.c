// dig_00 (ダイジェスト, digest movie): the digest map; no code or script names it as a destination. It plays demo.thp
// full screen while random characters walk, jump, fall and pop up in front of it, until a button press or the
// movie's end returns to the title.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "dolphin/exi.h"
#include "rels/dig/include/dig.h"
#include "system.h"

#include "seq/seq_demo.h"
#include "memory.h"
#include "manager/fontmgr.h"
#include "mario/mariost.h"
#include "sdk/DEMOInit.h"
#include "driver/dispdrv.h"
#include "string.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "stdlib.h"
#include "driver/seqdrv.h"
#include <dolphin/pad.h>

typedef struct DigAnim {
    const char* name;
    u32 mask;
    s32 anim;
} DigAnim;
static s32 work[1];

static USER_FUNC(evt_action_sub);
static USER_FUNC(gravity_scale);
static USER_FUNC(movie);
static USER_FUNC(sound_fade);
static USER_FUNC(z);

static s32* wp[1];
static DigAnim anim_tbl[48];
static EvtWord evt_action_1_2[154];
static EvtWord evt_action_1[7];
static EvtWord evt_action_2[7];
static EvtWord evt_action_jump[42];
static EvtWord evt_action_fall[45];
static EvtWord evt_action_walk[57];
static EvtWord evt_action[111];
EvtWord dig_00_init_evt[107];

/**
 * Prepares the THP movie player for demo.thp: initialises the simple THP decoder, opens the file,
 * gives it a buffer from the map heap, preloads and starts the audio at volume 80.
 * Skipped on a development unit with an IS-Viewer attached (EXI channel 0 device 2), which has no movie.
 */
// dig_00 (JP demo, JP and PAL releases; both US versions drop this REL): a credits test left on the disc. It plays
// demo.thp (Super Mario Sunshine's credits video) while TTYD characters pop up, like the real staff roll in end_00.
void dig_thpInit(void) {
    u32 type;

    EXIGetType(0, 2, &type);
    if (type != EXI_IS_VIEWER) {
        THPSimpleInit(2);
        THPSimpleOpen("demo.thp");
        THPSimpleSetBuffer(_mapAlloc(THPSimpleCalcNeedMemory()));
        THPSimpleAudioStop();
        THPSimplePreLoad(0);
        THPSimpleAudioStart();
        THPSimpleSetVolume(80, 0);
    }
}

/**
 * Shuts the movie down in the opposite order: stops the audio and the loader,
 * closes the file and quits the THP player. Skipped on an IS-Viewer unit.
 */
void dig_thpStop(void) {
    u32 type;

    EXIGetType(0, 2, &type);
    if (type != EXI_IS_VIEWER) {
        THPSimpleAudioStop();
        THPSimpleLoadStop();
        THPSimpleClose();
        THPSimpleQuit();
    }
}

/**
 * Draw callback for the digest map: decodes and draws the next frame of demo.thp over the whole screen
 * and records the frame number in work[0].
 * On an IS-Viewer unit it draws the line "the movie plays here" (in Japanese) instead and counts frames.
 * The EU release logs a message when a frame fails to decode.
 */
static void movie_draw(CameraId camera, void* param) {
    u16 width;
    u16 height;
    const char* msg;
    u32 type;

    EXIGetType(0, 2, &type);
    if (type == EXI_IS_VIEWER) {
        msg = "ここにムービーがながれます";
        FontDrawStart();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString(-FontGetMessageWidth(msg) / 2, 0.0f, msg);
        (*wp[0])++;
    } else {
        width = gp->fbWidth;
        height = gp->efbHeight;
#if VERSION >= VERSION_DEMO_EU
        if (THPSimpleDecode(0) == 1) {
            sysOSReport("dig_00.c", 130, "THP Video Decode 失敗\n");
        } else {
#else
        if (THPSimpleDecode(0) != 1) {
#endif
            *wp[0] = THPSimpleDrawCurrentFrame(DEMOGetRenderModeObj(), 0, 0, width, height);
        }
    }
}

/**
 * Script helper, started once by the map init: resets the frame counter and GF(0),
 * then queues movie_draw every frame.
 * Sets GF(0) to TRUE when the movie is within two frames of its end (or after 300 frames on an IS-Viewer unit).
 * Never finishes by itself.
 */
static USER_FUNC(movie) {
    u32 type;

    if (isFirstCall) {
        memset(wp[0], 0, 4);
        evtSetValue(event, GF(0), FALSE);
    }
    EXIGetType(0, 2, &type);
    if (type != 0x05070000) {
        if ((u32)*wp[0] >= THPSimpleGetTotalFrame() - 2) {
            evtSetValue(event, GF(0), TRUE);
        }
    } else if (*wp[0] >= 300) {
        evtSetValue(event, GF(0), TRUE);
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_OPAQUE_NO_Z, movie_draw, NULL);
    return EVT_RETURN_BLOCK;
}

/** Script helper: fades the movie sound to zero over two seconds. */
static USER_FUNC(sound_fade) {
    THPSimpleSetVolume(0, 2000);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is an NPC name; sets its jump gravity to 0.1
 * so it floats down slowly (the "fall" parade action).
 */
static USER_FUNC(gravity_scale) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    npc->jumpGravity = 0.1f;
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_action_sub) {
    EvtWord* args = event->args;
    s32 idx;
    u32* maskp;
    u32 mask;
    s32 n;
    s32 k;
    s32 i;
    s32 bit;
    const char* name;
    u32 type;

    rand();
    do {
        idx = (u32)rand() % 48;
    } while (evtNpcNameToPtr_NoAssert(event, anim_tbl[idx].name) != NULL);
    maskp = &anim_tbl[idx].mask;
    mask = *maskp;
    n = 0;
    for (i = 0; i < 6; i++) {
        if ((1 << i) & mask) {
            n++;
        }
    }
    *maskp = mask;
    k = rand() % n;
    for (i = 0; i < 6; i++) {
        if ((1 << i) & *maskp) {
            if (--k < 0) {
                break;
            }
        }
    }
    name = anim_tbl[idx].name;
    bit = 1 << i;
    EXIGetType(0, 2, &type);
    if (type != 0x05070000) {
        evtSetValue(event, *args++, (EvtWord)npcGetTribe(name)->modelName);
        evtSetValue(event, *args++, (EvtWord)name);
    } else {
        evtSetValue(event, *args++, (EvtWord)"a_mario");
        evtSetValue(event, *args++, (EvtWord)"にせマリオ２");
    }
    evtSetValue(event, *args++, bit);
    evtSetValue(event, *args++, anim_tbl[idx].anim);
    return EVT_RETURN_DONE;
}

/**
 * LW(0) = the camera distance at which a 25-degree field of view shows 480 units of height (half height 240 over
 * tan(12.5 degrees)), as an event float; dig_00_init_evt places the camera there.
 */
static USER_FUNC(z) {
    f32 t = tan(0.21816612780094147);

    event->lwData[0] = FLOAT(240.0f / t);
    return EVT_RETURN_DONE;
}

/** Pointer to the digest map's work counter, reached through a one-entry table. */
static s32* wp[] = {
    work,
};

/**
 * Cast of the digest movie: each entry names a character that walks, jumps, falls or pops up in front of the
 * movie, with the set of actions it may perform.
 */
static DigAnim anim_tbl[] = {
    {"ダイジェストマリオ", 7, 0},
    {"ルイージ", 7, 0},
    {"ダイジェストピーチ", 3, 0},
    {"ダイジェストピーチ２", 3, 0},
    {"クッパ", 11, 0},
    {"カメックババ", 3, 0},
    {"第三勢力総統", 3, 0},
    {"第三勢力幹部", 3, 0},
    {"軍団ザコ敵", 3, 0},
    {"マジョリン", 3, 0},
    {"マリリン", 3, 0},
    {"クリスチーヌ", 7, 0},
    {"ノコタロウ", 3, 0},
    {"クラウダ", 3, 0},
    {"ヨッシー", 3, 0},
    {"サンダース", 3, 0},
    {"チュチュリーナ", 3, 0},
    {"キノじい", 3, 0},
    {"クリハカセ", 3, 0},
    {"パラシュートボム", 8, 0},
    {"ノコノコ", 3, 0},
    {"クリボー", 3, 0},
    {"パワーアップ屋", 3, 0},
    {"キノピオ", 3, 0},
    {"ノコリン", 3, 0},
    {"プニ族", 32, 0},
    {"トゲ族", 32, 0},
    {"キノシコワ", 3, 0},
    {"美女", 3, 0},
    {"ガンス", 3, 0},
    {"ブレッドハート", 3, 0},
    {"ランペル", 3, 0},
    {"パンジーさん", 19, 0},
    {"ガラの悪い水夫", 3, 0},
    {"マルコ", 3, 0},
    {"ポワン探偵", 3, 0},
    {"グルメキノピオ", 3, 0},
    {"ホワイト将軍", 3, 0},
    {"モニー", 3, 0},
    {"ボッタクール", 3, 0},
    {"フラワーさん", 3, 0},
    {"星マニア", 3, 0},
    {"バニーテレサ", 17, 0},
    {"キザ野郎", 3, 0},
    {"ウスグラ店おじさん", 3, 0},
    {"ハンマーブロス", 35, 0},
    {"ホネノコ", 3, 0},
    {"チョロボン", 3, 0},
};

/**
 * Shared body of the two pop-up actions (NPC name in LW(10), LF(0) set means wait instead of look around):
 * the character appears at a random X below the bottom of the screen facing a random side, rises for 0.7 s,
 * then either turns left and right (LF(0) = 0) or just waits for a second, sinks away in 0.4 s and is deleted.
 */
static EVT_BEGIN(evt_action_1_2)
    CALL(evt_sub_random, 400, LW(0))
    SUBTRACT(LW(0), 200)
    CALL(evt_npc_get_height, LW(10), LW(9))
    MULTIPLY_FLOAT(LW(9), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(-240.0f))
    SUBTRACT_FLOAT(LW(1), LW(9))
    CALL(evt_npc_set_position, LW(10), LW(0), LW(1), 0)
    CALL(evt_npc_get_position, LW(10), LW(6), LW(7), LW(8))
    CALL(evt_sub_random, 1000, LW(0))
    IF_INT_LESS(LW(0), 500)
        CALL(evt_npc_set_ry, LW(10), 90)
        CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_INSTANT_TURN)
    ELSE
        CALL(evt_npc_set_ry, LW(10), 270)
        CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_INSTANT_TURN)
    END_IF
    MULTIPLY_FLOAT(LW(9), FLOAT(10.0f))
    SET_VALUE(LW(5), LW(10))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, LW(9), 700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        ADD_FLOAT(LW(0), LW(7))
        CALL(evt_npc_set_position, LW(5), LW(6), LW(0), LW(8))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_flag_onoff, 0, LW(5), NPC_FLAG_INSTANT_TURN)
    IF_INT_EQUAL(LF(0), 0)
        WAIT_MS(500)
        CALL(evt_npc_reverse_ry, LW(5))
        WAIT_MS(500)
        CALL(evt_npc_reverse_ry, LW(5))
        WAIT_MS(500)
    ELSE
        WAIT_MS(1000)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(9), 0, 400)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        ADD_FLOAT(LW(0), LW(7))
        CALL(evt_npc_set_position, LW(5), LW(6), LW(0), LW(8))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(evt_action_1)
    SET_VALUE(LF(0), 0)
    CALL_CHILD(evt_action_1_2)
    RETURN
EVT_END()

static EVT_BEGIN(evt_action_2)
    SET_VALUE(LF(0), 1)
    CALL_CHILD(evt_action_1_2)
    RETURN
EVT_END()

static EVT_BEGIN(evt_action_jump)
    CALL(evt_sub_random, 400, LW(0))
    SUBTRACT(LW(0), 200)
    CALL(evt_sub_random, 360, LW(5))
    CALL(evt_npc_set_ry, LW(10), LW(5))
    CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_walk_anim, LW(10))
    CALL(evt_npc_set_position, LW(10), LW(0), -400, 0)
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), -400, 0, 1500, 250)
    CALL(evt_npc_delete, LW(10))
    RETURN
EVT_END()

static EVT_BEGIN(evt_action_fall)
    CALL(evt_sub_random, 400, LW(0))
    SUBTRACT(LW(0), 200)
    CALL(evt_npc_set_position, LW(10), LW(0), 300, 0)
    CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_GRAVITY)
    CALL(gravity_scale, LW(10))
    CALL(evt_npc_set_walk_anim, LW(10))
    LOOP(0)
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(1), -300)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_delete, LW(10))
    RETURN
EVT_END()

static EVT_BEGIN(evt_action_walk)
    CALL(evt_sub_random, 320, LW(0))
    SUBTRACT(LW(0), 160)
    SUBTRACT(LW(0), 100)
    CALL(evt_sub_random, 1000, LW(1))
    CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_INSTANT_TURN)
    IF_INT_GREATER(LW(1), 500)
        CALL(evt_npc_set_position, LW(10), -400, LW(0), 0)
        CALL(evt_npc_move_position, LW(10), 400, LW(0), 0, FLOAT(80.0f), 1)
    ELSE
        CALL(evt_npc_set_position, LW(10), 400, LW(0), 0)
        CALL(evt_npc_move_position, LW(10), -400, LW(0), 0, FLOAT(80.0f), 1)
    END_IF
    CALL(evt_npc_delete, LW(10))
    RETURN
EVT_END()

static EVT_BEGIN(evt_action)
    LOOP(0)
        CALL(evt_action_sub, LW(0), LW(1), LW(2), LW(3))
        SET_VALUE(LW(10), LW(1))
        CALL(evt_npc_entry, LW(10), LW(0))
        CALL(evt_npc_set_position, LW(10), -1000, 0, 0)
        CALL(evt_npc_set_scale, LW(10), FLOAT(2.0f), FLOAT(2.0f), FLOAT(2.0f))
        CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, LW(10), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_tribe, LW(10), LW(1))
        IF_BITS_SET(LW(2), 1)
            SPAWN_EVENT(evt_action_1)
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 2)
            SPAWN_EVENT(evt_action_2)
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 4)
            SPAWN_EVENT(evt_action_jump)
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 8)
            SPAWN_EVENT(evt_action_fall)
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 16)
            SPAWN_EVENT(evt_action_walk)
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 32)
            IF_INT_NOT_EQUAL(LW(3), 0)
                SPAWN_EVENT(LW(3))
            ELSE
                CALL(evt_npc_delete, LW(10))
            END_IF
            GOTO(0)
        END_IF
        LABEL(0)
        CALL(evt_sub_random, 3000, LW(0))
        ADD(LW(0), 3000)
        WAIT_MS(LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Init script of dig_00, the digest movie map: hides Mario and the partner,
 * places the camera so 480 units fill the screen height, starts the character parade and the movie,
 * and fades in from white.
 * A button press (A or Start) or the end of the movie fades the sound and screen out
 * and returns to the title screen.
 */
EVT_BEGIN(dig_00_init_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(z, LW(0))
    CALL(evt_cam3d_evt_set, 0, 0, LW(0), 0, 0, 0, 0, EASING_SINE_IN_OUT)
    CALL(evt_cam_letter_box_disable, 1)
    SPAWN_EVENT(evt_action)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(movie)
    END_INLINE_EVENT
    CALL(evt_fade_set_mapchange_type, 1, -1, -1, FADE_IN_WHITE_3D, 1000)
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_fade_end_wait)
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_BITS_SET(LW(0), PAD_BUTTON_A | PAD_BUTTON_START)
                SET_VALUE(GF(0), 1)
            END_IF
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(sound_fade)
        CALL(evt_fade_entry, FADE_OUT_WHITE_3D, 1000, 255, 255, 255)
        CALL(evt_fade_end_wait)
        CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_WHITE_3D, 0, -1, -1)
        CALL(evt_seq_set_seq, SEQ_MAP_CHANGE, STRING("title"), 0)
    END_INLINE_EVENT
    RETURN
EVT_END()
