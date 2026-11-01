// jin_07 (ダイム寺院, Creepy Steeple): Boo (teresa) room with flames and a treasure chest, and a Star Piece;
// doors to jin_06 and jin_08.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_ext.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#pragma defer_codegen on
#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/extdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "memory.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/seqdrv.h"

static void senteresa_ext_disp(CameraId camera, void* param);
typedef struct Teresa {
    Vec pos;
    Vec dst;
    f32 t;
    f32 field_0x1C;
    f32 field_0x20;
    f32 startTangentX; // start tangent (x) of the Hermite path
    s32 wait;
    u8 state;
} Teresa;
static Teresa* pTeresa;
static s32 g_nNumEndProc[1];
s32 g_nNumOutBox[1];
f32 randf(f32 min, f32 max);
static inline f32 cosf(f32 x) { return cos(x); }

USER_FUNC(evt_kagemario_init);
USER_FUNC(senteresa_okure_move);
USER_FUNC(senteresa_okure_move_init);
USER_FUNC(senteresa_get_position);
static void senteresa_ext_shadow_disp(void);
static void senteresa_ext_dispent(void);
static void senteresa_ext_main(void);
static f32 computePos(f32 a, f32 b, f32 t);
static void senteresa_ext_init(void);
static f32 randf(f32 min, f32 max);

static EvtWord w_close[12];
static EvtWord w_open[12];
static EvtWord e_close[12];
static EvtWord e_open[12];
EvtWord jin_07_juyo_oumu_vivian[59];
static EvtWord check_roll[16];
static BeroEntry bero_entry_data[4];
EvtWord evt_locked[14];
static EvtWord evt_unlock[7];
static NpcSetupInfo npcEnt[2];
static ExtTexInfo senteresa_ext_entry_data[5];
EvtWord evt_teresa_box_open[576];
EvtWord jin_07_init_evt[151];

/**
 * Door animation callback: closes door08 as Mario arrives through w_bero_1 (from jin_08);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door08"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door08 as Mario leaves through w_bero_1 (to jin_08);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door08"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: closes door06 as Mario arrives through e_bero (from jin_06);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door06"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door06 as Mario leaves through e_bero (to jin_06);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door06"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Script run on entering jin_07 after the parrot room:
 * the partner comments (stg4_jin_46 and 47; Vivian's lines say they overheard Doopliss's name)
 * and the camera goes
 * back to normal. It runs once after the parrot's first special talk in jin_08 (oumu_juyo_talk sets LSWF(1)).
 */
EVT_BEGIN(jin_07_juyo_oumu_vivian)
    IF_INT_NOT_EQUAL(LSWF_JIN_PARROT_SCENE_PENDING, 0)
        SET_VALUE(LSWF_JIN_PARROT_SCENE_PENDING, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_bero_exec_wait, 65536)
        CALL(evt_party_stop, 0)
        CALL(evt_mario_set_dir, 270, 300, 0)
        CALL(evt_cam3d_evt_set, -214, 129, 454, -214, 57, 48, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        CALL(evt_msg_print_party, STRING("stg4_jin_46"))
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_msg_print_party, STRING("stg4_jin_47"))
        CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Exit event of the roll-through doorway: returns 0 in LW(0), which blocks the exit,
 * while Mario is in motion 22 (MARIO_MOTION_ROLL, the paper-roll form), and 1 otherwise.
 */
static EVT_BEGIN(check_roll)
    CALL(evt_mario_get_motion, LW(0))
    IF_INT_EQUAL(LW(0), MARIO_MOTION_ROLL)
        SET_VALUE(LW(0), 0)
        ELSE
        SET_VALUE(LW(0), 1)
    END_IF
    RETURN
EVT_END()

/**
 * Doors of this room: the west exit (w_bero_2) to jin_08, the west exit (w_bero_1) to jin_08 and the east
 * exit (e_bero) to jin_06.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        check_roll, // exitEvent
        "jin_08", // nextMapName
        "e_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_1", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        70, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_08", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_06", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {0},
};

/** Case script for the locked west door: Mario is told the door is locked (stg4_jin_48). */
EVT_BEGIN(evt_locked)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_48"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Runs when the lock is opened: switches the w_bero_1 door entry on. */
static EVT_BEGIN(evt_unlock)
    SET_VALUE(LW(0), STRING("w_bero_1"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

/** NPC of the room: the Boo (Teresa), which cannot be touched. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"テレサ"*/"\x83\x65\x83\x8C\x83\x54", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Four Boo models of the room's effect (c_teresa), all playing animation W_1 from start frames 1 to 4. */
static ExtTexInfo senteresa_ext_entry_data[] = {
    {"c_teresa", "W_1", 1.0f},
    {"c_teresa", "W_1", 2.0f},
    {"c_teresa", "W_1", 3.0f},
    {"c_teresa", "W_1", 4.0f},
    {0},
};

/** Returns a random float between min and max. */
static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

/**
 * Setup callback of the Boo swarm that pours out of the Boo box:
 * gives the 200 Boos start positions at the box (0, 6, 80),
 * random target heights and sideways tangents and staggered release delays.
 */
static void senteresa_ext_init(void) {
    ExtdrvPose* pose = extGetPosePtr();
    s32 n = extGetPoseNum();
    s32 i;
    f32 a;
    f32 r;

    pTeresa = _mapAlloc(sizeof(Teresa) * 200);
    for (i = 0; i < n; i++, pose++) {
        pose->flags = EXT_POSE_FLAG_BILLBOARD;
        pose->dir = 270.0f;
        PSMTXIdentity(*pose->mtx);
        pTeresa[i].pos.x = 0.0f;
        (*pose->mtx)[0][3] = 0.0f;
        pTeresa[i].pos.y = 6.0f;
        (*pose->mtx)[1][3] = 6.0f;
        pTeresa[i].pos.z = 80.0f;
        (*pose->mtx)[2][3] = 80.0f;
        a = randf(0.0f, 360.0f);
        r = randf(10.0f, 30.0f);
        pTeresa[i].dst.x = 200.0f;
        pTeresa[i].dst.y = pTeresa[i].pos.y + randf(27.0f, 170.0f);
        pTeresa[i].dst.z = pTeresa[i].pos.z + r * cosf(a);
        pTeresa[i].field_0x1C = 0.0f;
        pTeresa[i].field_0x20 = randf(-0.1f, 0.1f);
        pTeresa[i].t = 0.0f;
        pTeresa[i].wait = 1.7f * (i + 1);
        pTeresa[i].state = 0;
        pTeresa[i].startTangentX = randf(0.0f, 350.0f);
    }
    g_nNumOutBox[0] = 0;
    g_nNumEndProc[0] = 0;
}

/** Linear interpolation between a and b by t (0..1). */
static f32 computePos(f32 a, f32 b, f32 t) {
    return (1.0f - t) * a + t * b;
}

/** Returns the product of the three arguments (an inline kept for the Hermite curve code). */
// FAKE: one-use inline helper; writing the call out changes the object in both versions
inline f32 jin07HermiteMul3(f32 a, f32 b, f32 c) {
    return (a * b) * c;
}

/**
 * Moves a Boo along a cubic Hermite curve from the box to its target using the Boo's tangent values
 * and its progress t, writing the position into the pose matrix.
 */
void computePosHermite(ExtdrvPose* pose, Teresa* ter) {
    Vec m0 = {0.0f, 0.0f, 0.0f};
    Vec m1 = {1.0f, 0.0f, 0.0f};
    f32 t = ter->t;
    f32 t2;
    f32 u;
    f32 h00, h10, h11, h01;
    u = 1.0f - t;
    t2 = 2.0f * t;
    h00 = jin07HermiteMul3(1.0f + t2, u, u);
    h10 = t * u * u;
    h11 = (t * t) * (t - 1.0f);
    h01 = (3.0f - t2) * (t * t);
    m0.x = ter->startTangentX;
    m0.y = 3.4f * ter->dst.y;
    (*pose->mtx)[0][3] = ter->pos.x * h00 + m0.x * h10 + m1.x * h11 + ter->dst.x * h01;
    (*pose->mtx)[1][3] = ter->pos.y * h00 + m0.y * h10 + m1.y * h11 + ter->dst.y * h01;
    (*pose->mtx)[2][3] = ter->pos.z * h00 + m0.z * h10 + m1.z * h11 + ter->dst.z * h01;
}

/**
 * Per-frame update of the Boo swarm: each Boo waits for its release time,
 * then flies along its Hermite curve (t grows 0.015 per frame) and stops at the end;
 * when all 200 have finished the swarm effect is reset.
 */
// Release each Boo after its delay, count completed paths, and reset the renderer when all 200 finish.
static void senteresa_ext_main(void) {
    ExtdrvPose* pose = extGetPosePtr();
    s32 n;
    s32 i;

    marioGetPtr();
    n = extGetPoseNum();
    for (i = 0; i < n; i++, pose++) {
        switch (pTeresa[i].state) {
            case 0:
                pose->texId = 0;
                pTeresa[i].wait--;
                if (pTeresa[i].wait == 0) {
                    pose->flags |= EXT_POSE_FLAG_VISIBLE;
                    g_nNumOutBox[0]++;
                    pTeresa[i].state++;
                }
                break;
            case 1:
                pose->texId = (gp->retraceCount + i * i) % 3 + 1;
                computePosHermite(pose, &pTeresa[i]);
                pTeresa[i].t += 0.015f;
                if (pTeresa[i].t >= 1.0f) {
                    pTeresa[i].pos.x = (*pose->mtx)[0][3];
                    pTeresa[i].pos.y = (*pose->mtx)[1][3];
                    pTeresa[i].pos.z = (*pose->mtx)[2][3];
                    pTeresa[i].dst.x = 200.0f;
                    pTeresa[i].dst.y = (*pose->mtx)[1][3];
                    pTeresa[i].dst.z = 50.0f;
                    pTeresa[i].t = 0.01f;
                    pTeresa[i].state = 99;
                    g_nNumEndProc[0]++;
                }
                break;
            case 99:
                break;
        }
    }
    if (g_nNumEndProc[0] == 200) {
        extReset();
    }
}

/** Registers the swarm's draw callback with the 3D camera. */
static void senteresa_ext_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, senteresa_ext_disp, NULL);
}

/** Draw callback of the Boo swarm: loads the ext-pose render setup, vertices, textures and TEV setup and draws them. */
static void senteresa_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/** Draw callback for the swarm's shadows. */
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

/** Script helper: returns the position of Boo number args[0] in the swarm in args[1..3]. */
USER_FUNC(senteresa_get_position) {
    EvtWord* args = event->args;
    ExtdrvPose* pose = extGetPosePtr();

    pose += evtGetValue(event, args[0]);
    evtSetFloat(event, args[1], (*pose->mtx)[0][3]);
    evtSetFloat(event, args[2], (*pose->mtx)[1][3]);
    evtSetFloat(event, args[3], (*pose->mtx)[2][3]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores the start (args[0..2]) and target (args[3..5]) positions for the late Boo's flight into
 * LW(10..15).
 */
USER_FUNC(senteresa_okure_move_init) {
    EvtWord* args = event->args;

    event->lwData[10] = evtGetValue(event, args[0]);
    event->lwData[11] = evtGetValue(event, args[1]);
    event->lwData[12] = evtGetValue(event, args[2]);
    event->lwData[13] = evtGetValue(event, args[3]);
    event->lwData[14] = evtGetValue(event, args[4]);
    event->lwData[15] = evtGetValue(event, args[5]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: moves the late Boo NPC along a Hermite curve between the stored start
 * and target for the progress value args[0] (0..1).
 */
USER_FUNC(senteresa_okure_move) {
    EvtWord* args = event->args;
    NpcEntry* npc = npcNameToPtr("\203e\203\214\203T");
    f32 t = evtGetFloat(event, args[0]);
    Vec pos;
    Vec m0 = {0.0f, 250.0f, 0.0f};
    Vec m1 = {1.0f, 0.0f, 0.0f};
    f32 u = 1.0f - t;
    f32 t2 = 2.0f * t;
    f32 h00 = (1.0f + t2) * u * u;
    f32 h10 = t * u * u;
    f32 h11 = (t * t) * (t - 1.0f);
    f32 h01 = (3.0f - t2) * (t * t);

    pos.x = event->lwData[10] * h00 + m0.x * h10 + m1.x * h11 + event->lwData[13] * h01;
    pos.y = event->lwData[11] * h00 + m0.y * h10 + m1.y * h11 + event->lwData[14] * h01;
    pos.z = event->lwData[12] * h00 + m0.z * h10 + m1.z * h11 + event->lwData[15] * h01;
    npc->position = pos;
    return EVT_RETURN_DONE;
}

/**
 * Boo box scene (case script for the box a_hako):
 * the box shakes, its lid opens, 200 small Boos pour out and jeer with speech bubbles;
 * a late Boo then flies out, asks Mario a question (stg4_jin_69) and gives him an item before disappearing.
 * Sets the story to "Boo box".
 */
EVT_BEGIN(evt_teresa_box_open)
    // The music stops and Mario freezes.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 1, 100, 400, 0, 38, 48, 500, EASING_SINE_IN_OUT)
    // The box shakes violently.
    CALL(evt_mapobj_get_position, STRING("hako"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_BOX_SWING1"), LW(10), LW(11), LW(12), LW(13))
    LOOP(30)
        CALL(evt_sub_random, 3, LW(0))
        CALL(evt_mapobj_trans, 1, STRING("hako"), LW(0), 0, 0)
        WAIT_FRAMES(2)
        CALL(evt_sub_random, 3, LW(0))
        MULTIPLY(LW(0), -1)
        CALL(evt_mapobj_trans, 1, STRING("hako"), LW(0), 0, 0)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_mapobj_trans, 1, STRING("hako"), 0, 0, 0)
    WAIT_MS(300)
    // Its lid swings open.
    CALL(evt_mapobj_get_position, STRING("hako"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_TBOX_OPEN1"), LW(10), LW(11), LW(12), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 950, 50, 800)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_rotate, 1, STRING("hako_futa"), LW(0), 0, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(100)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 175, 703, 0, 57, 26, 300, EASING_QUADRATIC_OUT)
        WAIT_MS(350)
        CALL(evt_cam3d_evt_set, 0, 152, 567, 0, 57, 26, 250, EASING_QUADRATIC_IN)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    // A swarm of 200 little Boos pours out of the box.
    CALL(evt_ext_entry, 200, PTR(senteresa_ext_entry_data), PTR(senteresa_ext_init), PTR(senteresa_ext_main), PTR(senteresa_ext_dispent))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED2"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_TELESA_APPEAR1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_I_O"))
        WAIT_MS(5500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    // The Boos jeer at Mario with speech bubbles as they fly out.
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(0)
            PEEK_INT(LW(0), PTR(g_nNumEndProc))
            IF_INT_GREATER_OR_EQUAL(LW(0), 175)
                BREAK_LOOP
            END_IF
            PEEK_INT(LW(1), PTR(g_nNumOutBox))
            IF_INT_GREATER(LW(1), LW(0))
                CALL(evt_sub_random, 50, LW(1))
                ADD(LW(1), 30)
                CALL(evt_sub_random, LW(1), LW(0))
                CALL(evt_msg_toge, MSG_TOGE_AT_POS_FAR, LW(0), LW(1), 100)
                CALL(evt_sub_random, 1000, LW(0))
                MODULO(LW(0), 3)
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, 0, STRING("stg4_jin_66"), 0, 0)
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 0, STRING("stg4_jin_67"), 0, 0)
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING("stg4_jin_68"), 0, 0)
                END_SWITCH
            END_IF
            CALL(evt_sub_random, 200, LW(0))
            ADD(LW(0), 100)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    // Waits until the whole swarm has left the box.
    LOOP(0)
        PEEK_INT(LW(0), PTR(g_nNumEndProc))
        IF_INT_EQUAL(LW(0), 200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1200)
    // A late Boo floats in after the others and asks Mario a question; it tosses him an item depending on
    // his answer.
    CALL(evt_npc_set_position, STRING("テレサ"), 0, 6, 80)
    CALL(evt_npc_set_ry, STRING("テレサ"), 90)
    CALL(evt_npc_flag_onoff, 0, STRING("テレサ"), NPC_FLAG_NO_DRAW)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(14), 0)
        SET_VALUE(LW(15), 0)
        LOOP(100)
            CALL(evt_npc_get_position, STRING("テレサ"), LW(0), LW(1), LW(2))
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            CALL(evt_party_get_pos, 0, LW(6), LW(7), LW(8))
            IF_INT_GREATER(LW(0), LW(3))
                IF_INT_EQUAL(LW(14), 0)
                    CALL(evt_mario_set_dir, 90, 300, 0)
                    SET_VALUE(LW(14), 1)
                END_IF
            END_IF
            IF_INT_GREATER(LW(0), LW(6))
                IF_INT_EQUAL(LW(15), 0)
                    CALL(evt_party_set_dir, 0, 90, -1)
                    SET_VALUE(LW(15), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LW(14), 1)
                IF_INT_EQUAL(LW(15), 1)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(senteresa_okure_move_init, 0, 6, 80, 200, 30, 80)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("テレサ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_TELESA_APPEAR1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    SET_VALUE(LF(0), 0)
    SET_FLOAT_VALUE(LW(9), FLOAT(0.0f))
    LOOP(70)
        CALL(senteresa_okure_move, LW(9))
        ADD_FLOAT(LW(9), FLOAT(0.0146484375f))
        CALL(evt_npc_get_position, STRING("テレサ"), LW(1), LW(2), LW(3))
        IF_INT_GREATER(LW(1), 75)
            IF_INT_EQUAL(LF(0), 0)
                SET_VALUE(LF(0), 1)
                WAIT_MS(300)
                CALL(evt_set_dir_to_target, STRING("テレサ"), STRING("mario"))
                CALL(evt_msg_print, 0, STRING("stg4_jin_69"), 0, STRING("テレサ"))
                CALL(evt_msg_select, 0, STRING("stg4_jin_69_yn"))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print_add, 0, STRING("stg4_jin_71"))
                    SET_VALUE(LW(0), 121)
                    CASE_EQUAL(1)
                    CALL(evt_msg_print_add, 0, STRING("stg4_jin_71"))
                    SET_VALUE(LW(0), 121)
                    CASE_EQUAL(2)
                    CALL(evt_msg_print_add, 0, STRING("stg4_jin_70"))
                    SET_VALUE(LW(0), 150)
                END_SWITCH
                CALL(evt_item_entry, STRING("teresa"), LW(0), LW(1), LW(2), LW(3), ITEM_TYPE_SCATTER_NO_PICKUP, -1, 0)
                ADD(LW(2), 5)
                CALL(evt_item_set_move_dir_speed, STRING("teresa"), 0, FLOAT(0.0f))
                CALL(evt_item_set_gravity, STRING("teresa"), FLOAT(1.0f))
                CALL(evt_item_set_bound_rate, STRING("teresa"), FLOAT(1.0f))
                CALL(evt_item_set_jump_power, STRING("teresa"), FLOAT(100.0f))
                CALL(evt_item_status_on, STRING("teresa"), ITEM_FLAG2_SKIP_BOUND_INIT)
                CALL(evt_msg_print, 0, STRING("stg4_jin_72"), 0, STRING("テレサ"))
                CALL(evt_npc_reverse_ry, STRING("テレサ"))
                CALL(evt_npc_get_position, STRING("テレサ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_TELESA_APPEAR1_2"), LW(0), LW(1), LW(2), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // The late Boo vanishes, the story advances to "Boo box" and control returns.
    CALL(evt_npc_set_position, STRING("テレサ"), 0, -1000, 0)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_09_02_BOO_BOX)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Creepy Steeple jin_07 map init: sets the music for the room, the shadow-Mario tint and the exits,
 * places the Star Piece, handles Mario falling in from the hole,
 * unlocks or locks the west door by the jin_08 lock flag,
 * closes the Boo box before the "Boo box" story step and hides the flame objects.
 */
EVT_BEGIN(jin_07_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN3"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    SPAWN_EVENT(PTR(jin_07_juyo_oumu_vivian))
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 220, 75, -20, ITEM_TYPE_STILL, GSWF_STARPIECE_69, 0)
    IF_INT_NOT_EQUAL(GSWF_JIN_SCRIPTED_ENTRY_PENDING, 0)
        SET_VALUE(GSWF_JIN_SCRIPTED_ENTRY_PENDING, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_set_pos, 0, 365, -50)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_party_set_pos, 0, 0, 465, -50)
            END_IF
            CALL(evt_cam_road_reset)
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_mario_wait_landon)
            CALL(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    IF_INT_EQUAL(GSWF_JIN_08_DOOR_UNLOCKED, 1)
        SET_VALUE(LW(0), STRING("w_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
        ELSE
        SET_VALUE(LW(0), STRING("w_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_EQUAL(GSWF_JIN_08_DOOR_UNLOCKED, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("w_bero_1"), 0, PTR(evt_locked), 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_09_02_BOO_BOX)
        CALL(evt_mapobj_rotate, 1, STRING("hako_futa"), 95, 0, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_hako"), 0, PTR(evt_teresa_box_open), 0)
        ELSE
        CALL(evt_mapobj_rotate, 1, STRING("hako_futa"), 5, 0, 0)
    END_IF
    CALL(evt_map_replace_mapobj, STRING("honou01"), 0)
    CALL(evt_map_replace_mapobj, STRING("honou02"), 0)
    CALL(evt_map_replace_mapobj, STRING("honou03"), 0)
    CALL(evt_map_replace_mapobj, STRING("honou04"), 0)
    RETURN
EVT_END()

