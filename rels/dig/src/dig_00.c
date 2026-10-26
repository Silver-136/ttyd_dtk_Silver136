#include "battle/battle_unit.h"
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

void FontDrawString(f32 x, f32 y, const char* str);
void movie_draw(CameraId camera, void* param);
typedef struct DigAnim {
    const char* name;
    u32 mask;
    s32 anim;
} DigAnim;
static s32 work[1];

USER_FUNC(evt_action_sub);
USER_FUNC(evt_npc_set_walk_anim);
USER_FUNC(gravity_scale);
USER_FUNC(movie);
USER_FUNC(sound_fade);
static USER_FUNC(z);

static s32* wp[1];
static DigAnim anim_tbl[48];
EvtWord evt_action_1_2[154];
EvtWord evt_action_1[7];
EvtWord evt_action_2[7];
EvtWord evt_action_jump[42];
EvtWord evt_action_fall[45];
EvtWord evt_action_walk[57];
EvtWord evt_action[111];
EvtWord dig_00_init_evt[107];

// dig_00 (JP demo and JP release only; the US release drops this REL): a credits test left on the disc. It plays
// demo.thp (Super Mario Sunshine's credits video) while TTYD characters pop up, like the real staff roll in end_00.
void dig_thpInit(void) {
    u32 type;

    EXIGetType(0, 2, &type);
    if (type != 0x05070000) {
        THPSimpleInit(2);
        THPSimpleOpen("demo.thp");
        THPSimpleSetBuffer(_mapAlloc(THPSimpleCalcNeedMemory()));
        THPSimpleAudioStop();
        THPSimplePreLoad(0);
        THPSimpleAudioStart();
        THPSimpleSetVolume(80, 0);
    }
}

void dig_thpStop(void) {
    u32 type;

    EXIGetType(0, 2, &type);
    if (type != 0x05070000) {
        THPSimpleAudioStop();
        THPSimpleLoadStop();
        THPSimpleClose();
        THPSimpleQuit();
    }
}

void movie_draw(CameraId camera, void* param) {
    u16 width;
    u16 height;
    const char* msg;
    u32 type;

    EXIGetType(0, 2, &type);
    if (type == 0x05070000) {
        msg = "\202\261\202\261\202\311\203\200\201[\203r\201[\202\252\202\310\202\252\202\352\202\334\202\267";
        FontDrawStart();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString(-FontGetMessageWidth(msg) / 2, 0.0f, msg);
        (*wp[0])++;
    } else {
        width = gp->fbWidth;
        height = gp->efbHeight;
        if (THPSimpleDecode(0) != 1) {
            *wp[0] = THPSimpleDrawCurrentFrame(DEMOGetRenderModeObj(), 0, 0, width, height);
        }
    }
}

USER_FUNC(movie) {
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
    dispEntry(CAMERA_3D, 0.0f, 3, movie_draw, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(sound_fade) {
    THPSimpleSetVolume(0, 2000);
    return EVT_RETURN_DONE;
}

USER_FUNC(gravity_scale) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    npc->jumpGravity = 0.1f;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_action_sub) {
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
        evtSetValue(event, *args++, (EvtWord)"\202\311\202\271\203}\203\212\203I\202Q");
    }
    evtSetValue(event, *args++, bit);
    evtSetValue(event, *args++, anim_tbl[idx].anim);
    return EVT_RETURN_DONE;
}

static USER_FUNC(z) {
    f32 t = tan(0.21816612780094147);

    event->lwData[0] = FLOAT(240.0f / t);
    return EVT_RETURN_DONE;
}

static s32* wp[] = {
    work,
};

static DigAnim anim_tbl[] = {
    {/*"ダイジェストマリオ"*/"\x83\x5F\x83\x43\x83\x57\x83\x46\x83\x58\x83\x67\x83\x7D\x83\x8A\x83\x49", 7, 0},
    {/*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57", 7, 0},
    {/*"ダイジェストピーチ"*/"\x83\x5F\x83\x43\x83\x57\x83\x46\x83\x58\x83\x67\x83\x73\x81\x5B\x83\x60", 3, 0},
    {/*"ダイジェストピーチ２"*/"\x83\x5F\x83\x43\x83\x57\x83\x46\x83\x58\x83\x67\x83\x73\x81\x5B\x83\x60\x82\x51", 3, 0},
    {/*"クッパ"*/"\x83\x4E\x83\x62\x83\x70", 11, 0},
    {/*"カメックババ"*/"\x83\x4A\x83\x81\x83\x62\x83\x4E\x83\x6F\x83\x6F", 3, 0},
    {/*"第三勢力総統"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x91\x8D\x93\x9D", 3, 0},
    {/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94", 3, 0},
    {/*"軍団ザコ敵"*/"\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x47", 3, 0},
    {/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93", 3, 0},
    {/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93", 3, 0},
    {/*"クリスチーヌ"*/"\x83\x4E\x83\x8A\x83\x58\x83\x60\x81\x5B\x83\x6B", 7, 0},
    {/*"ノコタロウ"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45", 3, 0},
    {/*"クラウダ"*/"\x83\x4E\x83\x89\x83\x45\x83\x5F", 3, 0},
    {/*"ヨッシー"*/"\x83\x88\x83\x62\x83\x56\x81\x5B", 3, 0},
    {/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58", 3, 0},
    {/*"チュチュリーナ"*/"\x83\x60\x83\x85\x83\x60\x83\x85\x83\x8A\x81\x5B\x83\x69", 3, 0},
    {/*"キノじい"*/"\x83\x4C\x83\x6D\x82\xB6\x82\xA2", 3, 0},
    {/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A", 3, 0},
    {/*"パラシュートボム"*/"\x83\x70\x83\x89\x83\x56\x83\x85\x81\x5B\x83\x67\x83\x7B\x83\x80", 8, 0},
    {/*"ノコノコ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52", 3, 0},
    {/*"クリボー"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B", 3, 0},
    {/*"パワーアップ屋"*/"\x83\x70\x83\x8F\x81\x5B\x83\x41\x83\x62\x83\x76\x89\xAE", 3, 0},
    {/*"キノピオ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49", 3, 0},
    {/*"ノコリン"*/"\x83\x6D\x83\x52\x83\x8A\x83\x93", 3, 0},
    {/*"プニ族"*/"\x83\x76\x83\x6A\x91\xB0", 32, 0},
    {/*"トゲ族"*/"\x83\x67\x83\x51\x91\xB0", 32, 0},
    {/*"キノシコワ"*/"\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F", 3, 0},
    {/*"美女"*/"\x94\xFC\x8F\x97", 3, 0},
    {/*"ガンス"*/"\x83\x4B\x83\x93\x83\x58", 3, 0},
    {/*"ブレッドハート"*/"\x83\x75\x83\x8C\x83\x62\x83\x68\x83\x6E\x81\x5B\x83\x67", 3, 0},
    {/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B", 3, 0},
    {/*"パンジーさん"*/"\x83\x70\x83\x93\x83\x57\x81\x5B\x82\xB3\x82\xF1", 19, 0},
    {/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76", 3, 0},
    {/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52", 3, 0},
    {/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", 3, 0},
    {/*"グルメキノピオ"*/"\x83\x4F\x83\x8B\x83\x81\x83\x4C\x83\x6D\x83\x73\x83\x49", 3, 0},
    {/*"ホワイト将軍"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67\x8F\xAB\x8C\x52", 3, 0},
    {/*"モニー"*/"\x83\x82\x83\x6A\x81\x5B", 3, 0},
    {/*"ボッタクール"*/"\x83\x7B\x83\x62\x83\x5E\x83\x4E\x81\x5B\x83\x8B", 3, 0},
    {/*"フラワーさん"*/"\x83\x74\x83\x89\x83\x8F\x81\x5B\x82\xB3\x82\xF1", 3, 0},
    {/*"星マニア"*/"\x90\xAF\x83\x7D\x83\x6A\x83\x41", 3, 0},
    {/*"バニーテレサ"*/"\x83\x6F\x83\x6A\x81\x5B\x83\x65\x83\x8C\x83\x54", 17, 0},
    {/*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59", 3, 0},
    {/*"ウスグラ店おじさん"*/"\x83\x45\x83\x58\x83\x4F\x83\x89\x93\x58\x82\xA8\x82\xB6\x82\xB3\x82\xF1", 3, 0},
    {/*"ハンマーブロス"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x83\x75\x83\x8D\x83\x58", 35, 0},
    {/*"ホネノコ"*/"\x83\x7A\x83\x6C\x83\x6D\x83\x52", 3, 0},
    {/*"チョロボン"*/"\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", 3, 0},
};

EVT_BEGIN(evt_action_1_2)
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

EVT_BEGIN(evt_action_1)
    SET_VALUE(LF(0), 0)
    CALL_CHILD(PTR(evt_action_1_2))
    RETURN
EVT_END()

EVT_BEGIN(evt_action_2)
    SET_VALUE(LF(0), 1)
    CALL_CHILD(PTR(evt_action_1_2))
    RETURN
EVT_END()

EVT_BEGIN(evt_action_jump)
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

EVT_BEGIN(evt_action_fall)
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

EVT_BEGIN(evt_action_walk)
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

EVT_BEGIN(evt_action)
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
            SPAWN_EVENT(PTR(evt_action_1))
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 2)
            SPAWN_EVENT(PTR(evt_action_2))
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 4)
            SPAWN_EVENT(PTR(evt_action_jump))
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 8)
            SPAWN_EVENT(PTR(evt_action_fall))
            GOTO(0)
        END_IF
        IF_BITS_SET(LW(2), 16)
            SPAWN_EVENT(PTR(evt_action_walk))
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

EVT_BEGIN(dig_00_init_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(z, LW(0))
    CALL(evt_cam3d_evt_set, 0, 0, LW(0), 0, 0, 0, 0, EASING_SINE_IN_OUT)
    CALL(evt_cam_letter_box_disable, 1)
    SPAWN_EVENT(PTR(evt_action))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(movie)
    END_INLINE_EVENT
    CALL(evt_fade_set_mapchange_type, 1, -1, -1, 20, 1000)
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_fade_end_wait)
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_BITS_SET(LW(0), 4352)
                SET_VALUE(GF(0), 1)
            END_IF
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(sound_fade)
        CALL(evt_fade_entry, 21, 1000, 255, 255, 255)
        CALL(evt_fade_end_wait)
        CALL(evt_fade_set_mapchange_type, 1, 21, 0, -1, -1)
        CALL(evt_seq_set_seq, SEQ_MAP_CHANGE, STRING("title"), 0)
    END_INLINE_EVENT
    RETURN
EVT_END()

