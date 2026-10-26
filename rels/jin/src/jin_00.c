#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_pouch.h"
#include "mot/mot_hammer.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "mario/mariost.h"
#include "party.h"
#include "evt/evt_bero.h"
#include "pmario_sound.h"
#include "driver/dispdrv.h"
#include "dolphin/gx.h"
#include "driver/camdrv.h"
#include "driver/extdrv.h"
#include "eff/eff_teresa.h"
#include "evt/evt_cam.h"
#include "evt/evt_ext.h"
#include "evt/evt_item.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "jin.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"

typedef struct Teresa {
    Vec vel;
    f32 angle;
    Vec ofs;
    f32 bobPhase;
    f32 turnSpeed;
    Vec from;
    Vec to;
    s32 wait;
    u8 alpha;
    u8 seq;
} Teresa;
static Teresa* pTeresa;
Vec g_vecRotCenter;
s32 g_bMove;
s32 g_nMarioMoveCnt;
s32 g_nNumHuttobasu[1];
s32 g_nNumHarituku[1];
static s32 g_nNumEndProc[1];
f32 g_fSpeed;
static f32 randf(f32 min, f32 max);
static inline f32 sinf(f32 x) { return sin(x); }
static void computePos(Teresa* ter, Vec* out);
s32 computeAlpha(Teresa* ter, MarioWork* mp, f32 angle);
void computeMtx(ExtdrvPose* pose, f32 angle);
f32 computeAngle(ExtdrvPose* pose, MarioWork* mp);
static void senteresa_ext_disp(CameraId camera, void* param);
void senteresa_loadTev(void);
void senteresa_loadRenderMode(void);
static inline f32 cosf(f32 x) { return cos(x); }

USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_win_item_select);
USER_FUNC(get_npc_color);
USER_FUNC(teresa_eff);
USER_FUNC(senteresa_mario_spd);
USER_FUNC(senteresa_get_rot_center);
USER_FUNC(senteresa_wait_end_seq);
USER_FUNC(senteresa_end);
USER_FUNC(senteresa_turiageru);
USER_FUNC(senteresa_huttobu);
USER_FUNC(senteresa_dispoff);
USER_FUNC(senteresa_syugo);
USER_FUNC(senteresa_tikaduku_stop);
USER_FUNC(senteresa_get_random_position);
USER_FUNC(senteresa_get_seq);
static void senteresa_ext_shadow_disp(void);
static void senteresa_ext_dispent(void);
static void senteresa_ext_main(void);
static void senteresa_ext_init(void);
s32 randn(s32 min, s32 max);

static EvtWord w_open[22];
static EvtWord w_close[22];
EvtWord n3_open[12];
EvtWord n3_close[12];
static EvtWord s_open[12];
static EvtWord s_close[12];
static EvtWord n2_open[12];
static EvtWord n2_close[12];
EvtWord e3_open[12];
EvtWord e3_close[12];
EvtWord e1_open[12];
EvtWord e1_close[12];
EvtWord e2_open[12];
EvtWord e2_close[12];
EvtWord jin_00_dai[16];
EvtWord jin_00_dai_in_sub[83];
EvtWord jin_00_dai_in[9];
static EvtWord check_roll[16];
EvtWord teresa_n_fadein[48];
EvtWord teresa_n_fadeout[35];
EvtWord door_teresa_n[90];
EvtWord teresa_s_fadein[48];
EvtWord teresa_s_fadeout[35];
EvtWord door_teresa_s[90];
BeroEntry bero_stg4majo3nin[1];
static BeroEntry bero_entry_data[10];
EvtWord first_jin00[69];
static EvtWord push_evt[297];
EvtWord nopush_evt[5];
static EvtWord evt_key[5];
static s32 lock_tbl[2];
static EvtWord lock_item_check[23];
static EvtWord evt_unlock[10];
EvtWord tyuou_teresa_kusukusu[42];
EvtWord tyuou_teresa_look_mario[14];
EvtWord tyuou_teresa_init[35];
EvtWord tyuou_teresa_talk[391];
EvtWord oku_teresa_init[21];
EvtWord temae_teresa_init[21];
EvtWord atomic_teresa_init[2];
static NpcSetupInfo npcEnt[12];
static ExtTexInfo senteresa_ext_entry_data[47];
EvtWord senteresa_evt[1248];
EvtWord jin_00_init_evt[221];

USER_FUNC(get_npc_color) {
    EvtWord* args = event->args;
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, args[0]));

    evtSetValue(event, args[1], npc->color.r);
    evtSetValue(event, args[2], npc->color.g);
    evtSetValue(event, args[3], npc->color.b);
    evtSetValue(event, args[4], npc->color.a);
    return EVT_RETURN_DONE;
}

s32 randn(s32 min, s32 max) {
    return min + rand() % (max - min);
}


static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

f32 computeAngle(ExtdrvPose* pose, MarioWork* mp) {
    f32 a = angleABf(mp->position.x, mp->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);

    if (0.0f <= a && a < 90.0f) {
        return 90.0f - a;
    }
    if (90.0f <= a && a < 180.0f) {
        return 360.0f - (a - 90.0f);
    }
    if (180.0f <= a && a < 270.0f) {
        return 180.0f + (270.0f - a);
    }
    return 90.0f + (360.0f - a);
}

void computeMtx(ExtdrvPose* pose, f32 angle) {
    Mtx m;

    PSMTXRotRad(m, 'Y', 0.017453292f * angle);
    (*pose->mtx)[0][0] = m[0][0];
    (*pose->mtx)[0][1] = m[0][1];
    (*pose->mtx)[0][2] = m[0][2];
    (*pose->mtx)[1][0] = m[1][0];
    (*pose->mtx)[1][1] = m[1][1];
    (*pose->mtx)[1][2] = m[1][2];
    (*pose->mtx)[2][0] = m[2][0];
    (*pose->mtx)[2][1] = m[2][1];
    (*pose->mtx)[2][2] = m[2][2];
}

s32 computeAlpha(Teresa* ter, MarioWork* mp, f32 angle) {
    f32 dir;

    if (angle > 180.0f) {
        angle = 360.0f - angle;
    }
    dir = mp->displayDirectionTarget - 180.0f;
    if (dir < 0.0f) {
        dir += 360.0f;
    }
    if (dir < 90.0f) {
        if (angle < 90.0f) {
            ter->alpha = 128.0f * (angle / 90.0f) + 128.0f;
            return 0;
        }
        ter->alpha = 255;
        return 1;
    } else {
        if (angle > 90.0f) {
            ter->alpha = 128.0f * ((180.0f - angle) / 90.0f) + 128.0f;
            return 0;
        }
        ter->alpha = 255;
        return 1;
    }
}

static void computePos(Teresa* ter, Vec* out) {
    Vec v;
    Mtx m;

    v.x = ter->ofs.x;
    v.y = 0.0f;
    v.z = ter->ofs.z;

    PSMTXRotRad(m, 'Y', 0.017453292f * ter->angle);
    PSMTXMultVec(m, &v, out);
}

static void senteresa_ext_init(void) {
    ExtdrvPose* pose = extGetPosePtr();
    MarioWork* mp = marioGetPtr();
    s32 n = extGetPoseNum();
    s32 i;

    pTeresa = _mapAlloc(sizeof(Teresa) * 200);
    for (i = 0; i < n; i++, pose++) {
        pose->flags = 3;
        PSMTXIdentity(*pose->mtx);
        pTeresa[i].angle = 0.0f;
        pTeresa[i].wait = (i % 100 + 1) * 3;
        pTeresa[i].alpha = 0;
        pTeresa[i].seq = 0;
        pTeresa[i].turnSpeed = randf(0.0f, 0.1f);
        pTeresa[i].ofs.x = randf(-300.0f, 300.0f);
        pTeresa[i].ofs.y = 0.0f;
        pTeresa[i].ofs.z = randf(-235.0f, 235.0f);
        (*pose->mtx)[0][3] = mp->position.x + pTeresa[i].ofs.x;
        (*pose->mtx)[1][3] = randf(10.0f, 400.0f);
        (*pose->mtx)[2][3] = mp->position.z + pTeresa[i].ofs.z;
        if (mp->position.x < (*pose->mtx)[0][3]) {
            pose->dir = 90.0f;
        } else {
            pose->dir = 270.0f;
        }
    }
    g_nNumEndProc[0] = 0;
    g_nNumHarituku[0] = 0;
    g_nNumHuttobasu[0] = 0;
    g_nMarioMoveCnt = 0;
    g_fSpeed = 0.0f;
    g_bMove = 0;
    g_vecRotCenter.x = mp->position.x;
    g_vecRotCenter.y = 0.0f;
    g_vecRotCenter.z = mp->position.z;
}

static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door01L"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door01R"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door01L"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door01R"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n3_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n3_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door07"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door07"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e3_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door06"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e3_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door06"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door05"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(e2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door05"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(jin_00_dai)
    DEBUG_PUT_MSG(STRING("出る"))
    CALL(evt_mario_normalize)
    SET_VALUE(GSWF(2079), 1)
    CALL(evt_bero_mapchange, STRING("jin_06"), 0)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(jin_00_dai_in_sub)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, 390, -60, 0)
    CALL(evt_party_set_pos, 0, 390, -60, -10)
    CALL(evt_party_set_pos, 1, 390, -60, -20)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_party_set_dir, 1, 270, -1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_jump_pos, 365, 40, 0, 700, 0, -1000)
        CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_party_jump_pos, 0, 365, 40, -10, 700, 0, -1000)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_party_jump_pos, 1, 365, 40, -20, 700, 0, -1000)
    END_INLINE_EVENT
    WAIT_MS(1500)
    RETURN
EVT_END()

EVT_BEGIN(jin_00_dai_in)
    DEBUG_PUT_MSG(STRING("入る"))
    SPAWN_EVENT(PTR(jin_00_dai_in_sub))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(check_roll)
    CALL(evt_mario_get_motion, LW(0))
    IF_INT_EQUAL(LW(0), 22)
        SET_VALUE(LW(0), 0)
        ELSE
        SET_VALUE(LW(0), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(teresa_n_fadein)
    IS_EVENT_RUNNING(GW(0), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        DELETE_EVENT(GW(0))
    END_IF
    CALL(get_npc_color, STRING("奥扉テレサ"), LW(0), LW(1), LW(2), LW(3))
    LOOP(0)
        ADD(LW(3), 4)
        IF_INT_GREATER_OR_EQUAL(LW(3), 255)
            SET_VALUE(LW(3), 255)
            BREAK_LOOP
        END_IF
        CALL(evt_npc_set_color, STRING("奥扉テレサ"), 255, 255, 255, LW(3))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_color, STRING("奥扉テレサ"), 255, 255, 255, 255)
    RETURN
EVT_END()

EVT_BEGIN(teresa_n_fadeout)
    SET_VALUE(LW(0), 255)
    LOOP(0)
        SUBTRACT(LW(0), 4)
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_npc_set_color, STRING("奥扉テレサ"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_color, STRING("奥扉テレサ"), 255, 255, 255, 0)
    RETURN
EVT_END()

EVT_BEGIN(door_teresa_n)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 197)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_start_for_event)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("奥扉テレサ"), LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_npc_set_ry, STRING("奥扉テレサ"), 270)
        CALL(evt_mario_set_dir, 90, 0, 0)
        SET_VALUE(LW(6), -20)
        SET_VALUE(LW(7), 90)
        ELSE
        CALL(evt_npc_set_ry, STRING("奥扉テレサ"), 90)
        CALL(evt_mario_set_dir, 270, 0, 0)
        SET_VALUE(LW(6), 20)
        SET_VALUE(LW(7), 270)
    END_IF
    CALL_CHILD(PTR(teresa_n_fadein))
    IF_INT_LESS(GSW_STORY_PROGRESS, 196)
        CALL(evt_msg_print, 0, STRING("stg4_jin_49"), 0, STRING("奥扉テレサ"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg4_jin_51"), 0, STRING("奥扉テレサ"))
    END_IF
    SPAWN_EVENT_GET_ID(PTR(teresa_n_fadeout), GW(0))
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(teresa_s_fadein)
    IS_EVENT_RUNNING(GW(1), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        DELETE_EVENT(GW(1))
    END_IF
    CALL(get_npc_color, STRING("手前扉テレサ"), LW(0), LW(1), LW(2), LW(3))
    LOOP(0)
        ADD(LW(3), 4)
        IF_INT_GREATER_OR_EQUAL(LW(3), 255)
            SET_VALUE(LW(3), 255)
            BREAK_LOOP
        END_IF
        CALL(evt_npc_set_color, STRING("手前扉テレサ"), 255, 255, 255, LW(3))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_color, STRING("手前扉テレサ"), 255, 255, 255, 255)
    RETURN
EVT_END()

EVT_BEGIN(teresa_s_fadeout)
    SET_VALUE(LW(0), 255)
    LOOP(0)
        SUBTRACT(LW(0), 4)
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_npc_set_color, STRING("手前扉テレサ"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_color, STRING("手前扉テレサ"), 255, 255, 255, 0)
    RETURN
EVT_END()

EVT_BEGIN(door_teresa_s)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 197)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_start_for_event)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("手前扉テレサ"), LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_npc_set_ry, STRING("手前扉テレサ"), 270)
        CALL(evt_mario_set_dir, 90, 0, 0)
        SET_VALUE(LW(6), -30)
        SET_VALUE(LW(7), 90)
        ELSE
        CALL(evt_npc_set_ry, STRING("手前扉テレサ"), 90)
        CALL(evt_mario_set_dir, 270, 0, 0)
        SET_VALUE(LW(6), 20)
        SET_VALUE(LW(7), 270)
    END_IF
    CALL_CHILD(PTR(teresa_s_fadein))
    IF_INT_LESS(GSW_STORY_PROGRESS, 196)
        CALL(evt_msg_print, 0, STRING("stg4_jin_50"), 0, STRING("手前扉テレサ"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg4_jin_52"), 0, STRING("手前扉テレサ"))
    END_IF
    SPAWN_EVENT_GET_ID(PTR(teresa_s_fadeout), GW(1))
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

BeroEntry bero_stg4majo3nin[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "usu_00", // nextMapName
        "evt_majo3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "n_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        check_roll, // exitEvent
        "jin_05", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "ana", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        jin_00_dai_in, // enterEvent
        0, // caseType
        jin_00_dai, // exitEvent
        "jin_06", // nextMapName
        NULL, // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        120, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gra_06", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {
        "n_bero_3", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        door_teresa_n, // exitEvent
        "jin_01", // nextMapName
        "s_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n3_close, // enterAnimeArg
        n3_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        door_teresa_s, // exitEvent
        "jin_02", // nextMapName
        "n_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_04", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n2_close, // enterAnimeArg
        n2_open, // exitAnimeArg
    },
    {
        "e_bero_3", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_03", // nextMapName
        "w_bero3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e3_close, // enterAnimeArg
        e3_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_03", // nextMapName
        "w_bero1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_close, // enterAnimeArg
        e1_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_03", // nextMapName
        "w_bero2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_close, // enterAnimeArg
        e2_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(first_jin00)
    CALL(evt_mario_set_pos, -640, 0, 0)
    CALL(evt_party_set_pos, 0, -640, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 227, 365, 1120, 227, 128, -223, 0, 11)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -328, 365, 1120, -328, 128, -223, 5000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_INLINE_EVENT
    WAIT_MS(5000)
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, 194)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(push_evt)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_get_pose, LW(0))
    IF_STR_NOT_EQUAL(LW(0), STRING("M_O_1"))
        CALL(evt_mario_set_pose, STRING("M_O_1"))
    END_IF
    SET_VALUE(LW(0), GW(10))
    MODULO(LW(0), 4)
    DIVIDE(LW(0), 2)
    IF_INT_LESS(GW(10), 40)
        MULTIPLY(LW(0), 1)
        ELSE
        MULTIPLY(LW(0), 2)
        BEGIN_INLINE_EVENT
            CALL(evt_sub_random, 100, LW(0))
            IF_INT_LESS(LW(0), 10)
                CALL(evt_mapobj_get_position, STRING("oki01"), LW(0), LW(1), LW(2))
                CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 35, 40, 0, 0, 0, 0, 0, 0, 0, 0)
                ADD(LW(0), 20)
                CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 35, 40, 0, 0, 0, 0, 0, 0, 0, 0)
            END_IF
        END_INLINE_EVENT
    END_IF
    CALL(evt_mapobj_trans, 1, STRING("oki01"), LW(0), 0, 0)
    ADD(GW(10), 1)
    IF_INT_GREATER_OR_EQUAL(GW(10), 80)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_set_pose, STRING("M_O_1"))
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1600)
        END_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MOVE_PEDESTAL1"), LW(2), LW(3), LW(4), LW(8))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MOVE_PEDESTAL2"), LW(2), LW(3), LW(4), LW(9))
        BEGIN_INLINE_EVENT
            LOOP(8)
                CALL(evt_mapobj_get_position, STRING("oki01"), LW(0), LW(1), LW(2))
                CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 35, 40, 0, 0, 0, 0, 0, 0, 0, 0)
                ADD(LW(0), 20)
                CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 35, 40, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(200)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 500, 1600)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("oki01"), LW(0), 0, 0)
        CALL(evt_hit_bind_update, STRING("a_oki01"))
        ADD(LW(0), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 396)
            CALL(evt_mario_get_pose, LW(5))
            IF_STR_NOT_EQUAL(LW(5), STRING("M_N_7"))
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
                END_INLINE_EVENT
                CALL(evt_mario_set_pose, STRING("M_N_7"))
            END_IF
            ELSE
            CALL(evt_mario_set_pos, LW(0), LW(3), LW(4))
        END_IF
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_snd_sfxoff, LW(8))
        CALL(evt_snd_sfxoff, LW(9))
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_hitobj_onoff, STRING("a_futa"), 0, 0)
        SET_VALUE(GSW_STORY_PROGRESS, 195)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(nopush_evt)
    SET_VALUE(GW(10), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_key)
    SET_VALUE(GSW_STORY_PROGRESS, 198)
    RETURN
EVT_END()

static s32 lock_tbl[] = {
    ITEM_STEEPLE_KEY_0x16, -1,
};

static EVT_BEGIN(lock_item_check)
    CALL(evt_pouch_check_item, ITEM_STEEPLE_KEY_0x16, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(lock_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock)
    SET_VALUE(GSW_STORY_PROGRESS, 199)
    SET_VALUE(LW(0), STRING("n_bero_2"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(tyuou_teresa_kusukusu)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("中央テレサ"), LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
        IF_INT_LESS(LW(6), 125)
            CALL(evt_msg_print, 0, STRING("stg4_jin_53"), 0, STRING("中央テレサ"))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(tyuou_teresa_look_mario)
    CALL(evt_seq_wait, SEQ_GAME)
    LOOP(0)
        CALL(evt_set_dir_to_target, STRING("中央テレサ"), STRING("mario"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(tyuou_teresa_init)
    SPAWN_EVENT(PTR(npc_init_evt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 196)
        CALL(evt_npc_set_position, STRING("me"), 70, 0, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 197)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, 0)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(tyuou_teresa_talk)
    CALL(evt_msg_print, 0, STRING("stg4_jin_54"), 0, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"))
    CALL(evt_msg_select, 0, STRING("stg4_jin_54_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg4_jin_55"))
        RETURN
    ELSE
        CALL(evt_msg_continue)
    END_IF
    CALL(evt_cam3d_evt_set, 70, 165, 472, 70, 72, -45, 1000, 11)
    CALL(evt_npc_set_anim, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), STRING("A_1"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_O"))
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(3), LW(4), LW(5))
        IF_INT_LESS(LW(0), LW(3))
            CALL(evt_npc_set_ry, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 270)
            SUBTRACT(LW(0), 40)
        ELSE
            CALL(evt_npc_set_ry, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 90)
            ADD(LW(0), 40)
        END_IF
        CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 300, 0, -1000)
        WAIT_MS(300)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_dispflg_onoff, 0, 16, 1)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(3), LW(4), LW(5))
        IF_INT_LESS(LW(0), LW(3))
            SUBTRACT(LW(0), 40)
        ELSE
            ADD(LW(0), 40)
        END_IF
        CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 300, 0, -1000)
        WAIT_MS(300)
        CALL(evt_party_dispflg_onoff, 0, 16, 0)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_cam3d_evt_set, 70, 193, 689, 70, 72, 0, 60, 11)
            WAIT_MS(60)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_cam3d_evt_set, 70, 165, 472, 70, 72, -45, 60, 11)
            WAIT_MS(60)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 100, 400, 100)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_npc_set_scale, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_TELESA_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), STRING("S_1"))
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_56"), 0, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    SET_VALUE(GF(0), 1)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, 70, 165, 472, 70, 72, -45, 500, 11)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 400, 100, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_scale, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_jin_57"), 0, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"))
    CALL(evt_cam3d_evt_set, 70, 400, 1125, 70, 200, -13, 1000, 11)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, 500)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_color, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
        CALL(evt_npc_set_color, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 255, 255, 255, 0)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_change_interrupt, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 6, 0)
    SPAWN_EVENT(senteresa_evt)
    CALL(senteresa_wait_end_seq)
    WAIT_MS(3500)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 197)
    RETURN
EVT_END()

EVT_BEGIN(oku_teresa_init)
    SPAWN_EVENT(PTR(npc_init_evt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 197)
        CALL(evt_npc_set_position, STRING("me"), -60, 45, -238)
        CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(temae_teresa_init)
    SPAWN_EVENT(PTR(npc_init_evt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 197)
        CALL(evt_npc_set_position, STRING("me"), -60, 45, 238)
        CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(atomic_teresa_init)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"テレサ"*/"\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        tyuou_teresa_init, // initEvent
        NULL, // regularEvent
        tyuou_teresa_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"奥扉テレサ"*/"\x89\x9C\x94\xE0\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        oku_teresa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手前扉テレサ"*/"\x8E\xE8\x91\x4F\x94\xE0\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        temae_teresa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, // reactionFlags
        atomic_teresa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1001テレサ"*/"\x31\x30\x30\x31\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1002テレサ"*/"\x31\x30\x30\x32\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1003テレサ"*/"\x31\x30\x30\x33\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1004テレサ"*/"\x31\x30\x30\x34\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1005テレサ"*/"\x31\x30\x30\x35\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"1006テレサ"*/"\x31\x30\x30\x36\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW | NPC_FLAG_NO_SHADOW, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static ExtTexInfo senteresa_ext_entry_data[] = {
    {"c_teresa", "S_1", 0.0f},
    {"c_teresa", "S_1", 5.0f},
    {"c_teresa", "S_1", 10.0f},
    {"c_teresa", "S_1", 15.0f},
    {"c_teresa", "S_1", 20.0f},
    {"c_teresa", "S_1", 25.0f},
    {"c_teresa", "S_1", 30.0f},
    {"c_teresa", "S_1", 35.0f},
    {"c_teresa", "S_1", 40.0f},
    {"c_teresa", "S_1", 45.0f},
    {"c_teresa", "S_1", 50.0f},
    {"c_teresa", "S_1", 55.0f},
    {"c_teresa", "S_1", 60.0f},
    {"c_teresa", "S_1", 65.0f},
    {"c_teresa", "S_1", 70.0f},
    {"c_teresa", "S_1", 75.0f},
    {"c_teresa", "W_1", 0.0f},
    {"c_teresa", "W_1", 1.0f},
    {"c_teresa", "W_1", 2.0f},
    {"c_teresa", "W_1", 3.0f},
    {"c_teresa", "W_1", 4.0f},
    {"c_teresa", "W_1", 5.0f},
    {"c_teresa", "W_1", 6.0f},
    {"c_teresa", "W_1", 7.0f},
    {"c_teresa", "W_1", 8.0f},
    {"c_teresa", "W_1", 9.0f},
    {"c_teresa", "D_1", 0.0f},
    {"c_teresa", "D_1", 1.0f},
    {"c_teresa", "D_1", 2.0f},
    {"c_teresa", "D_1", 3.0f},
    {"c_teresa", "D_1", 4.0f},
    {"c_teresa", "D_1", 5.0f},
    {"c_teresa", "D_1", 6.0f},
    {"c_teresa", "D_1", 7.0f},
    {"c_teresa", "A_2", 0.0f},
    {"c_teresa", "A_2", 1.0f},
    {"c_teresa", "A_2", 2.0f},
    {"c_teresa", "A_2", 3.0f},
    {"c_teresa", "A_2", 4.0f},
    {"c_teresa", "A_2", 5.0f},
    {"c_teresa", "A_2", 6.0f},
    {"c_teresa", "A_2", 7.0f},
    {"c_teresa", "A_2", 8.0f},
    {"c_teresa", "A_2", 9.0f},
    {"c_teresa", "A_2", 10.0f},
    {"c_teresa", "A_2", 11.0f},
    {0},
};

static void senteresa_ext_main(void) {
    ExtdrvPose* pose = extGetPosePtr();
    MarioWork* mp = marioGetPtr();
    s32 n = extGetPoseNum();
    s32 i;
    s32 hammer = 0;
    s32 bero;
    s32 party;
    s32 res;
    u8 alpha;
    f32 angle;
    f32 spd;
    Vec v;
    Vec d;

    if (marioGetAngleKaitenHammer() > 0.0f) {
        hammer = 1;
    }
    PSVECSubtract(&mp->position, &g_vecRotCenter, &d);
    if (mp->baseSpeed > 0.0f) {
        g_bMove = 1;
    }
    if (g_bMove) {
        g_nMarioMoveCnt++;
    }
    if (g_nMarioMoveCnt > 12) {
        g_fSpeed += 0.01f;
        if (g_fSpeed > 2.6f) {
            g_fSpeed = 2.6f;
        }
        if (d.x == 0.0f && d.y == 0.0f && d.z == 0.0f) {
            g_vecRotCenter = mp->position;
            if (mp->baseSpeed == 0.0f) {
                g_fSpeed = 0.0f;
                g_bMove = 0;
                g_nMarioMoveCnt = 0;
            }
        } else {
            f32 mag = PSVECMag(&d);
            if (mag > 50.0f) {
                spd = 2.6f;
            } else {
                spd = 2.6f * mag / 50.0f;
            }
            if (g_fSpeed > spd) {
                g_fSpeed = spd;
            }
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d, g_fSpeed);
            PSVECAdd(&g_vecRotCenter, &d, &g_vecRotCenter);
            PSVECSubtract(&g_vecRotCenter, &mp->position, &v);
            if (PSVECMag(&v) < 0.3f) {
                g_vecRotCenter = mp->position;
                if (mp->baseSpeed == 0.0f) {
                    g_fSpeed = 0.0f;
                    g_bMove = 0;
                    g_nMarioMoveCnt = 0;
                }
            }
        }
    }
    bero = (bero_get_BeroEXEC() & 3) != 0;
    party = 0;
    if (partyGetPtr(0) != NULL && (partyGetPtr(0)->flags & 0x100)) {
        party = 1;
    }
    for (i = 0; i < n; i++, pose++) {
        switch (pTeresa[i].seq) {
            case 0:
                pose->texId = 0;
                pTeresa[i].wait--;
                if (pTeresa[i].wait == 0) {
                    if (i == 0) {
                        Vec pos = {0.0f, 200.0f, 0.0f};
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_APPEAR2), &pos);
                    }
                    pTeresa[i].seq++;
                }
                break;
            case 1:
                pose->texId = (gp->retraceCount / 5 + i * i) % 16;
                if (mp->position.x < (*pose->mtx)[0][3]) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                computePos(&pTeresa[i], &v);
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                angle = computeAngle(pose, mp);
                pTeresa[i].alpha += 3;
                alpha = pTeresa[i].alpha;
                computeAlpha(&pTeresa[i], mp, angle);
                if (pTeresa[i].alpha < alpha) {
                    pTeresa[i].seq++;
                    g_nNumEndProc[0]++;
                    if (i == 0) {
                        Vec pos = {0.0f, 200.0f, 0.0f};
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_APPEAR2), &pos);
                    }
                } else {
                    pTeresa[i].alpha = alpha;
                    if (pTeresa[i].alpha >= 250) {
                        pTeresa[i].alpha = 255;
                        pTeresa[i].seq++;
                        g_nNumEndProc[0]++;
                        if (i == 0) {
                            Vec pos = {0.0f, 200.0f, 0.0f};
                            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_APPEAR2), &pos);
                        }
                    }
                }
                break;
            case 2:
                pose->texId = (gp->retraceCount / 5 + i * i) % 16;
                if (mp->position.x < (*pose->mtx)[0][3]) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                computePos(&pTeresa[i], &v);
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                angle = computeAngle(pose, mp);
                computeAlpha(&pTeresa[i], mp, angle);
                if (g_nNumEndProc[0] == 200) {
                    pTeresa[i].wait = i + 1;
                    pTeresa[i].seq++;
                }
                break;
            case 3:
                pose->texId = (gp->retraceCount / 5 + i * i) % 16;
                if (mp->position.x < (*pose->mtx)[0][3]) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                computePos(&pTeresa[i], &v);
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                pTeresa[i].bobPhase += 0.08f;
                pTeresa[i].wait--;
                if (pTeresa[i].wait == 0) {
                    pose->flags &= ~2;
                    pTeresa[i].wait = i * 30 + rand() % 10 + 295;
                    pTeresa[i].seq++;
                }
                break;
            case 4:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                computePos(&pTeresa[i], &v);
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[1][3] = v.y;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                pTeresa[i].turnSpeed += 0.01f;
                if (pTeresa[i].turnSpeed > 1.2f) {
                    pTeresa[i].turnSpeed = 1.2f;
                }
                pTeresa[i].angle += pTeresa[i].turnSpeed;
                if (pTeresa[i].angle > 360.0f) {
                    pTeresa[i].angle -= 360.0f;
                }
                angle = computeAngle(pose, mp);
                computeMtx(pose, angle);
                res = computeAlpha(&pTeresa[i], mp, angle);
                if (g_nNumHuttobasu[0] >= 20) {
                    break;
                }
                if (res && rand() % 1000 == 312) {
                    pTeresa[i].wait = 100;
                    pTeresa[i].seq = 9;
                }
                if (!party && g_nNumHuttobasu[0] < 20) {
                    pTeresa[i].wait--;
                    if (pTeresa[i].wait == 0) {
                        pTeresa[i].bobPhase = 0.0f;
                        pTeresa[i].seq++;
                    }
                }
                break;
            case 5:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                v.x = (*pose->mtx)[0][3];
                v.y = (*pose->mtx)[1][3];
                v.z = (*pose->mtx)[2][3];
                PSVECSubtract(&mp->position, &v, &d);
                PSVECNormalize(&d, &d);
                PSVECScale(&d, &d, 1.8f);
                PSVECAdd(&v, &d, &v);
                (*pose->mtx)[0][3] = v.x;
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                (*pose->mtx)[2][3] = v.z;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                if (!party) {
                    PSVECSubtract(&v, &mp->position, &v);
                    if (PSVECMag(&v) <= 25.0f) {
                        f32 dy = (*pose->mtx)[1][3] - mp->position.y;
                        if (dy > -2.0f && dy < 10.0f) {
                            pTeresa[i].ofs.x = (*pose->mtx)[0][3] - mp->position.x;
                            pTeresa[i].ofs.y = (*pose->mtx)[1][3] - mp->position.y;
                            pTeresa[i].ofs.z = (*pose->mtx)[2][3] - mp->position.z;
                            pTeresa[i].seq++;
                            if (!bero) {
                                g_nNumHarituku[0]++;
                            }
                        }
                    }
                }
                angle = computeAngle(pose, mp);
                computeMtx(pose, angle);
                computeAlpha(&pTeresa[i], mp, angle);
                break;
            case 6:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                (*pose->mtx)[0][3] = mp->position.x + pTeresa[i].ofs.x;
                (*pose->mtx)[1][3] = mp->position.y + pTeresa[i].ofs.y;
                (*pose->mtx)[2][3] = mp->position.z + pTeresa[i].ofs.z;
                angle = computeAngle(pose, mp);
                computeAlpha(&pTeresa[i], mp, angle);
                if (hammer) {
                    v.x = (*pose->mtx)[0][3];
                    v.y = (*pose->mtx)[1][3];
                    v.z = (*pose->mtx)[2][3];
                    PSVECSubtract(&v, &mp->position, &d);
                    PSVECNormalize(&d, &d);
                    pTeresa[i].vel.x = 4.2f * d.x;
                    if (d.z > 0.0f) {
                        spd = d.z * randf(5.2f, 12.7f);
                    } else {
                        spd = d.z * randf(-5.0f, -2.0f);
                    }
                    pTeresa[i].vel.z = spd;
                    pTeresa[i].vel.y = randf(6.0f, 10.0f);
                    pTeresa[i].seq = 10;
                    computeMtx(pose, 0.0f);
                    g_nNumHarituku[0]--;
                    g_nNumHuttobasu[0]++;
                    {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_ESCAPE1), &pos);
                    }
                }
                break;
            case 9:
                pose->texId = (s32)(12.0f * ((100.0f - pTeresa[i].wait) / 100.0f)) + 34;
                computePos(&pTeresa[i], &v);
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                pTeresa[i].angle += 1.2f;
                if (pTeresa[i].angle > 360.0f) {
                    pTeresa[i].angle -= 360.0f;
                }
                angle = computeAngle(pose, mp);
                computeMtx(pose, angle);
                res = computeAlpha(&pTeresa[i], mp, angle);
                pTeresa[i].wait--;
                if (!res || pTeresa[i].wait == 0) {
                    pTeresa[i].wait = rand() % 300 + 200;
                    pTeresa[i].seq = 4;
                }
                break;
            case 10:
                pose->texId = (gp->retraceCount + i * i) % 8 + 26;
                (*pose->mtx)[0][3] += pTeresa[i].vel.x;
                (*pose->mtx)[1][3] += pTeresa[i].vel.y;
                (*pose->mtx)[2][3] += pTeresa[i].vel.z;
                pTeresa[i].vel.y -= 0.3f;
                if ((*pose->mtx)[1][3] < -20.0f) {
                    pose->flags = 0;
                    pTeresa[i].seq = 99;
                }
                break;
            case 20:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                computePos(&pTeresa[i], &v);
                v.y = (*pose->mtx)[1][3];
                (*pose->mtx)[0][3] = g_vecRotCenter.x + v.x;
                (*pose->mtx)[1][3] = 0.4f * sinf(1.2f * pTeresa[i].bobPhase) + v.y;
                (*pose->mtx)[2][3] = g_vecRotCenter.z + v.z;
                pTeresa[i].bobPhase += 0.08f;
                if (pTeresa[i].bobPhase > 3.1415927f) {
                    pTeresa[i].bobPhase -= 3.1415927f;
                }
                pTeresa[i].angle += 1.2f;
                if (pTeresa[i].angle > 360.0f) {
                    pTeresa[i].angle -= 360.0f;
                }
                angle = computeAngle(pose, mp);
                computeMtx(pose, angle);
                computeAlpha(&pTeresa[i], mp, angle);
                pTeresa[i].wait--;
                if (pTeresa[i].wait == 0) {
                    pose->flags = 3;
                    pTeresa[i].from.x = (*pose->mtx)[0][3];
                    pTeresa[i].from.y = (*pose->mtx)[1][3];
                    pTeresa[i].from.z = (*pose->mtx)[2][3];
                    pTeresa[i].bobPhase = 0.0f;
                    pTeresa[i].seq++;
                }
                break;
            case 21: {
                f32 t;

                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                t = pTeresa[i].bobPhase;
                if (t >= 1.0f) {
                    pTeresa[i].seq = 22;
                    {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_TOGETHER1), &pos);
                    }
                    g_nNumEndProc[0]++;
                } else {
                    v.x = (1.0f - t) * pTeresa[i].from.x + t * pTeresa[i].to.x;
                    v.y = (1.0f - t) * pTeresa[i].from.y + t * pTeresa[i].to.y;
                    v.z = (1.0f - t) * pTeresa[i].from.z + t * pTeresa[i].to.z;
                    (*pose->mtx)[0][3] = v.x;
                    (*pose->mtx)[1][3] = 20.0f * (f32)sin(6.28f * pTeresa[i].bobPhase) + v.y;
                    (*pose->mtx)[2][3] = v.z;
                    pTeresa[i].bobPhase += 0.01f;
                }
                break;
            }
            case 22:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                if ((pTeresa[i].wait & 7) == 0) {
                    (*pose->mtx)[0][3] += 2.0f;
                } else if ((pTeresa[i].wait & 7) == 3) {
                    (*pose->mtx)[0][3] -= 2.0f;
                }
                pTeresa[i].wait++;
                break;
            case 30:
                pose->texId = (gp->retraceCount + i * i) % 8 + 26;
                (*pose->mtx)[0][3] += pTeresa[i].vel.x;
                (*pose->mtx)[1][3] += pTeresa[i].vel.y;
                (*pose->mtx)[2][3] += pTeresa[i].vel.z;
                pTeresa[i].vel.y -= 0.3f;
                if ((*pose->mtx)[1][3] < -10.0f) {
                    pose->flags = 0;
                    pTeresa[i].seq = 99;
                    g_nNumEndProc[0]++;
                }
                break;
            case 80:
                pose->texId = (gp->retraceCount + i * i) % 10 + 16;
                (*pose->mtx)[1][3] = mp->position.y + pTeresa[i].ofs.y;
                break;
            case 99:
                break;
        }
    }
}

static void senteresa_ext_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, senteresa_ext_disp, NULL);
}

void senteresa_loadRenderMode(void) {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void senteresa_loadTev(void) {
    GXColor fogColor = {0xFF, 0xFF, 0xFF, 0xFF};

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fogColor);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

static void senteresa_ext_disp(CameraId camera, void* param) {
    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 n = extGetPoseNum();
    s32 i;

    senteresa_loadRenderMode();
    extLoadVertex();
    extLoadTexture();
    senteresa_loadTev();
    for (i = 0; i < n; i++) {
        color.a = pTeresa[i].alpha;
        GXSetTevColor(GX_TEVREG0, color);
        extPoseDraw(i);
    }
    extLoadTextureExit();
}

static void senteresa_ext_shadow_disp(void) {
    static Mtx mt = {
        {10.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 10.0f, 0.0f, 0.1f},
        {0.0f, 0.0f, 10.0f, 0.0f},
    };

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    extDrawShadow();
}

USER_FUNC(senteresa_get_seq) {
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], pTeresa[i].seq);
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_get_random_position) {
    EvtWord* args = event->args;
    ExtdrvPose* pose = extGetPosePtr();
    s32 n = extGetPoseNum();

    pose += rand() % n;
    evtSetFloat(event, args[0], (*pose->mtx)[0][3]);
    evtSetFloat(event, args[1], (*pose->mtx)[1][3]);
    evtSetFloat(event, args[2], (*pose->mtx)[2][3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_tikaduku_stop) {
    s32 n = extGetPoseNum();
    s32 i;

    for (i = 0; i < n; i++) {
        if (pTeresa[i].seq != 10) {
            pTeresa[i].seq = 4;
            pTeresa[i].wait = 99999;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_syugo) {
    ExtdrvPose* pose = extGetPosePtr();
    NpcEntry* npc = npcNameToPtr("\222\206\211\233\203e\203\214\203T");
    s32 n = extGetPoseNum();
    s32 i;
    f32 a;
    f32 r;

    g_nNumEndProc[0] = 0;
    for (i = 0; i < n; i++, pose++) {
        pTeresa[i].from.x = (*pose->mtx)[0][3];
        pTeresa[i].from.y = (*pose->mtx)[1][3];
        pTeresa[i].from.z = (*pose->mtx)[2][3];
        a = randf(0.0f, 6.2831f);
        r = randf(0.0f, 25.0f);
        pTeresa[i].to.x = r * cosf(a) + npc->position.x;
        pTeresa[i].to.y = r * sinf(a) + npc->position.y;
        pTeresa[i].to.z = npc->position.z;
        pTeresa[i].bobPhase = 0.0f;
        pTeresa[i].wait = (i & 15) + rand() % 10 * 30 + 1;
        pTeresa[i].alpha = 255;
        pTeresa[i].seq = 20;
        if (pTeresa[i].from.x < pTeresa[i].to.x) {
            pose->dir = 270.0f;
        } else {
            pose->dir = 90.0f;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_dispoff) {
    ExtdrvPose* pose = extGetPosePtr();
    s32 n = extGetPoseNum();
    s32 i;

    for (i = 0; i < n; i++, pose++) {
        pose->flags = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_huttobu) {
    ExtdrvPose* pose = extGetPosePtr();
    NpcEntry* npc = npcNameToPtr("\203A\203g\203~\203b\203N\203e\203\214\203T");
    s32 n = extGetPoseNum();
    s32 i;
    f32 scale;

    g_nNumEndProc[0] = 0;
    for (i = 0; i < n; i++, pose++) {
        pose->flags = 3;
        (*pose->mtx)[0][3] = npc->position.x;
        (*pose->mtx)[1][3] = npc->position.y;
        (*pose->mtx)[2][3] = npc->position.z;
        pTeresa[i].vel.x = 4.0f * (f32)sin(pTeresa[i].angle);
        pTeresa[i].vel.y = randf(6.0f, 12.0f);
        pTeresa[i].vel.z = cos(pTeresa[i].angle);
        if (pTeresa[i].vel.z > 0.0f) {
            scale = randf(5.0f, 12.5f);
        } else {
            scale = randf(-5.0f, -2.0f);
        }
        pTeresa[i].vel.z *= scale;
        pTeresa[i].alpha = 255;
        pTeresa[i].seq = 30;
        if (i & 1) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = (*pose->mtx)[0][3];
            pos.y = (*pose->mtx)[1][3];
            pos.z = (*pose->mtx)[2][3];
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG4_TELESA_ESCAPE1), &pos);
        }
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(senteresa_evt)
    CALL(evt_ext_entry, 200, PTR(senteresa_ext_entry_data), PTR(senteresa_ext_init), PTR(senteresa_ext_main), PTR(senteresa_ext_dispent))
    LOOP(0)
        CALL(senteresa_get_seq, 199, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 4)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GW(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(8))
            CALL(senteresa_get_rot_center, LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(0))
            ADD(LW(3), LW(8))
            ADD(LW(3), 65)
            CALL(evt_sub_random, 65, LW(4))
            ADD(LW(3), LW(4))
            SET_VALUE(LW(5), LW(1))
            ADD(LW(5), 5)
            CALL(evt_sub_random, 40, LW(6))
            ADD(LW(5), LW(6))
            SET_VALUE(LW(7), LW(2))
            SUBTRACT(LW(7), 90)
            CALL(evt_msg_toge, 3, LW(3), LW(5), LW(7))
            CALL(evt_msg_print, 0, STRING("stg4_jin_58"), 0, 0)
            CALL(evt_sub_random, 200, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(8))
            CALL(senteresa_get_rot_center, LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(0))
            ADD(LW(3), LW(8))
            SUBTRACT(LW(3), 32)
            CALL(evt_sub_random, 65, LW(4))
            ADD(LW(3), LW(4))
            SET_VALUE(LW(5), LW(1))
            ADD(LW(5), 5)
            CALL(evt_sub_random, 70, LW(6))
            ADD(LW(5), LW(6))
            SET_VALUE(LW(7), LW(2))
            SUBTRACT(LW(7), 90)
            CALL(evt_msg_toge, 3, LW(3), LW(5), LW(7))
            CALL(evt_msg_print, 0, STRING("stg4_jin_59"), 0, 0)
            CALL(evt_sub_random, 200, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(8))
            CALL(senteresa_get_rot_center, LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(0))
            ADD(LW(3), LW(8))
            SUBTRACT(LW(3), 110)
            CALL(evt_sub_random, 45, LW(4))
            ADD(LW(3), LW(4))
            SET_VALUE(LW(5), LW(1))
            ADD(LW(5), 45)
            CALL(evt_sub_random, 30, LW(6))
            ADD(LW(5), LW(6))
            SET_VALUE(LW(7), LW(2))
            SUBTRACT(LW(7), 90)
            CALL(evt_msg_toge, 3, LW(3), LW(5), LW(7))
            CALL(evt_msg_print, 0, STRING("stg4_jin_60"), 0, 0)
            CALL(evt_sub_random, 300, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(750)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(8))
            CALL(senteresa_get_rot_center, LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(0))
            ADD(LW(3), LW(8))
            ADD(LW(3), 55)
            CALL(evt_sub_random, 75, LW(4))
            ADD(LW(3), LW(4))
            SET_VALUE(LW(5), LW(1))
            SUBTRACT(LW(5), 80)
            CALL(evt_sub_random, 30, LW(6))
            SUBTRACT(LW(5), LW(6))
            SET_VALUE(LW(7), LW(2))
            SUBTRACT(LW(7), 90)
            CALL(evt_msg_toge, 3, LW(3), LW(5), LW(7))
            CALL(evt_msg_print, 0, STRING("stg4_jin_59"), 0, 0)
            CALL(evt_sub_random, 200, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(8))
            CALL(senteresa_get_rot_center, LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(0))
            ADD(LW(3), LW(8))
            SUBTRACT(LW(3), 110)
            CALL(evt_sub_random, 75, LW(4))
            ADD(LW(3), LW(4))
            SET_VALUE(LW(5), LW(1))
            SUBTRACT(LW(5), 80)
            CALL(evt_sub_random, 30, LW(6))
            SUBTRACT(LW(5), LW(6))
            SET_VALUE(LW(7), LW(2))
            SUBTRACT(LW(7), 90)
            CALL(evt_msg_toge, 3, LW(3), LW(5), LW(7))
            CALL(evt_msg_print, 0, STRING("stg4_jin_58"), 0, 0)
            CALL(evt_sub_random, 300, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    LOOP(0)
        PEEK_INT(LW(0), PTR(g_nNumHuttobasu))
        IF_INT_GREATER_OR_EQUAL(LW(0), 20)
            SET_VALUE(GW(0), 1)
            BREAK_LOOP
        END_IF
        CALL(senteresa_mario_spd)
        WAIT_FRAMES(1)
        PEEK_INT(LW(0), PTR(g_nNumHarituku))
        IF_INT_GREATER_OR_EQUAL(LW(0), 20)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_normalize)
            CALL(evt_mario_cont_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
            CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
            CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, 11)
            CALL(evt_mario_set_pose, STRING("M_I_O"))
            CALL(evt_mario_cam_y_off)
            CALL(senteresa_turiageru)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            LOOP(150)
                ADD_FLOAT(LW(1), FLOAT(1.0f))
                CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
                CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
            SET_VALUE(GSWF(2227), 1)
            CALL(evt_mario_normalize)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, 201)
                CALL(evt_bero_mapchange, STRING("usu_00"), STRING("evt_majo3"))
            ELSE
                CALL(evt_bero_mapchange, STRING("gra_06"), STRING("n_bero"))
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(senteresa_tikaduku_stop)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(2500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 60)
    CALL(evt_npc_set_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_scale, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 1, 1, 1)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_TALK_BALLOON)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(1), 20)
        CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), 0, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 1000, 11)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    LOOP(25)
        ADD(LW(0), 10)
        CALL(evt_npc_set_color, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_color, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), 255, 255, 255, 255)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg4_jin_61"), 0, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 20)
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), 0, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    ADD(LW(5), 160)
    ADD(LW(8), 160)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 2000, 11)
        WAIT_MS(1000)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(2), LW(3), LW(4))
    SET_VALUE(LW(5), LW(3))
    ADD(LW(5), 200)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, LW(3), LW(5), 2000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_dir, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(3))
    CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(3))
    CALL(senteresa_syugo)
    LOOP(0)
        PEEK_INT(LW(0), PTR(g_nNumEndProc))
        IF_INT_EQUAL(LW(0), 200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(teresa_eff, LW(0), LW(1), LW(2))
    WAIT_MS(500)
    CALL(senteresa_dispoff)
    CALL(evt_npc_get_position, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 25)
    CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"中央テレサ"*/"\x92\x86\x89\x9B\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_DRAW)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_62"), 0, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"))
    CALL(evt_cam3d_evt_off, 400, 11)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_O"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 3)
    CALL(evt_npc_glide_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2), 0, FLOAT(450.0f), 1, 11, 0)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    BEGIN_INLINE_EVENT
        LOOP(100)
            CALL(evt_npc_get_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 4)
            CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            WAIT_MS(10)
            CALL(evt_npc_get_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            ADD(LW(1), 4)
            CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            WAIT_MS(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(50)
            CALL(evt_npc_get_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 4)
            CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            WAIT_MS(20)
            CALL(evt_npc_get_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            ADD(LW(0), 4)
            CALL(evt_npc_set_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
            WAIT_MS(20)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(300)
    SET_VALUE(GW(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 110)
        ADD(LW(1), 70)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_63"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 70)
        SUBTRACT(LW(1), 90)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_64"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        ADD(LW(1), 30)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_65"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 40)
        SUBTRACT(LW(1), 20)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_64"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 80)
        SUBTRACT(LW(1), 100)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_65"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 105)
        ADD(LW(1), 75)
        SUBTRACT(LW(2), 90)
        CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_jin_63"), 0, 0)
        WAIT_MS(750)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_battle_info, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), 7)
    CALL(evt_npc_battle_start, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"))
    CALL(evt_npc_wait_battle_end)
    ADD(LW(1), 50)
    CALL(evt_item_entry, STRING("item_99"), ITEM_LUCKY_START, LW(0), LW(1), LW(2), ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(2242), 0)
    CALL(evt_item_flag_on, STRING("item_99"), 256)
    SET_VALUE(GSWF(2242), 1)
    CALL(senteresa_huttobu)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"アトミックテレサ"*/"\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54"), NPC_FLAG_NO_DRAW)
    LOOP(0)
        PEEK_INT(LW(0), PTR(g_nNumEndProc))
        IF_INT_EQUAL(LW(0), 200)
            SET_VALUE(GW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(300)
    CALL(senteresa_end)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(2226), 1)
    RETURN
EVT_END()

EVT_BEGIN(jin_00_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -500, 260, -200, 0, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 194)
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        SPAWN_EVENT(PTR(first_jin00))
        ELSE
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 198)
        CALL(evt_item_entry, STRING("key00"), ITEM_STEEPLE_KEY_0x16, -555, 200, 190, ITEM_TYPE_GROUND, -1, PTR(evt_key))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 199)
        CALL(evt_mobj_lock, STRING("mobj_lock00"), 22, -435, 210, -245, 0, PTR(lock_item_check), PTR(evt_unlock), GSWF(2228))
        SET_VALUE(LW(0), STRING("n_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    SET_VALUE(GW(0), -1)
    SET_VALUE(GW(1), -1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 196)
        SPAWN_EVENT(PTR(tyuou_teresa_kusukusu))
        SPAWN_EVENT(PTR(tyuou_teresa_look_mario))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 197)
        IF_INT_EQUAL(GSWF(2226), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, SEQ_GAME)
                WAIT_MS(1000)
                SPAWN_EVENT(PTR(senteresa_evt))
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2226), 0)
        CALL(evt_npc_check_delete, STRING("テレサ１"))
        CALL(evt_npc_check_delete, STRING("テレサ２"))
        CALL(evt_npc_check_delete, STRING("テレサ３"))
        CALL(evt_npc_check_delete, STRING("テレサ４"))
        CALL(evt_npc_check_delete, STRING("テレサ５"))
        CALL(evt_npc_check_delete, STRING("テレサ８"))
    END_IF
    CALL(evt_hit_bind_mapobj, STRING("a_oki01"), STRING("oki01"))
    IF_INT_LESS(GSW_STORY_PROGRESS, 195)
        SET_VALUE(GW(10), 0)
        CALL(evt_run_case_evt, CASE_PUSH, 1, STRING("A_dai"), 0, PTR(push_evt), 0)
        CALL(evt_run_case_evt, CASE_NOT_PUSH, 1, STRING("A_dai"), 0, PTR(nopush_evt), 0)
        ELSE
        CALL(evt_mapobj_trans, 1, STRING("oki01"), FLOAT(50.0f), 0, 0)
        CALL(evt_hit_bind_update, STRING("a_oki01"))
        CALL(evt_hitobj_onoff, STRING("a_futa"), 0, 0)
    END_IF
    CALL(evt_map_playanim, STRING("hikari"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 201)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_stg4majo3nin))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("e_bero_2"))
        CALL(evt_cam_road_reset)
    END_IF
    CALL(evt_hitobj_attr_onoff, 0, 0, STRING("a_futa"), 4)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hahen"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

USER_FUNC(senteresa_turiageru) {
    s32 n = extGetPoseNum();
    s32 i;

    g_nNumEndProc[0] = 0;
    for (i = 0; i < n; i++) {
        if (pTeresa[i].seq == 6) {
            pTeresa[i].seq = 80;
        } else if (pTeresa[i].seq != 9) {
            pTeresa[i].wait = 99999;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_end) {
    extReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_wait_end_seq) {
    return g_nNumEndProc[0] == 200 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(senteresa_get_rot_center) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], g_vecRotCenter.x);
    evtSetValue(event, args[1], g_vecRotCenter.y);
    evtSetValue(event, args[2], g_vecRotCenter.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(senteresa_mario_spd) {
    MarioWork* mp = marioGetPtr();
    f32 scale = (20 - g_nNumHarituku[0]) / 20.0f;

    if (mp->currMotionId == MARIO_MOTION_WALK) {
        mp->baseSpeed = mp->mBaseWalkSpeed * scale;
    } else if (mp->currMotionId == MARIO_MOTION_DASH) {
        mp->baseSpeed = mp->mBaseDashSpeed * scale;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_eff) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effTeresaEntry(0, x, y + 20.0f, z + 20.0f);
    return EVT_RETURN_DONE;
}

