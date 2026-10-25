#pragma defer_codegen on
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"

#include "string.h"
#include "dolphin/os.h"
#include "mario/mariost.h"
#include "memory.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "rsh.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/extdrv.h"

typedef struct MoamoaPoseDef {
    char model[16];
    char anim[8];
    s16 loop;
    f32 speed;
    s32 start;
    s32 count;
} MoamoaPoseDef;
typedef struct Moamoa {
    u16 flags;
    s16 state;
    s16 timer;
    Vec pos;
    f32 angle;
    s16 pose;
    u64 startTime;
} Moamoa;
typedef struct MoamoaWork {
    s32 field_0x0;
    s32 count;
    Moamoa* moa;
    s32 numDefs;
    MoamoaPoseDef defs[32];
} MoamoaWork;
static MoamoaWork* wp;
static f32 randf(f32 min, f32 max);
static void moamoa_pose_init(void);
static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim);
static void moamoa_pose_main(void);
static void ext_disp(CameraId camera, void* param);

USER_FUNC(evt_moamoa_set_one);
USER_FUNC(evt_moamoa_set_seq_all);
void moamoa_dispent(void);
void moamoa_main(void);
void moamoa_init(void);

ExtTexInfo moamoa_data[81];
static EvtWord evt_moamoa[324];
EvtWord evt_rsh_moamoa_on_window[46];
static BeroEntry bero_data_a[2];
static BeroEntry bero_data_b[2];
static BeroEntry bero_data_c[2];
EvtWord evt_handle[107];
EvtWord regu_unten[210];
static EvtWord init_unten[17];
static EvtWord talk_unten[236];
static NpcSetupInfo npc_data[2];
static const char* access_tbl[5];
static EvtWord evt_access[14];
static EvtWord evt_access_entry[23];
EvtWord rsh_00_init_evt[194];
EvtWord rsh_00_a_init_evt[51];
EvtWord rsh_00_b_init_evt[11];
static const char* window_tbl[5];
EvtWord rsh_00_c_init_evt[16];
EvtWord rsh_00_init_evt_nidome[164];
EvtWord rsh_00_a_init_evt_nidome[11];
EvtWord rsh_00_b_init_evt_nidome[11];
EvtWord rsh_00_c_init_evt_nidome[16];

ExtTexInfo moamoa_data[] = {
    {"c_n_moa_a", "S_1", 0.0f},
    {"c_n_moa_a", "S_1", 1.0f},
    {"c_n_moa_a", "S_1", 2.0f},
    {"c_n_moa_a", "S_1", 3.0f},
    {"c_n_moa_a", "S_1", 4.0f},
    {"c_n_moa_a", "S_1", 5.0f},
    {"c_n_moa_a", "S_1", 6.0f},
    {"c_n_moa_a", "S_1", 7.0f},
    {"c_n_moa_a", "S_1", 8.0f},
    {"c_n_moa_a", "S_1", 9.0f},
    {"c_n_moa_a", "S_1", 10.0f},
    {"c_n_moa_a", "S_1", 11.0f},
    {"c_n_moa_a", "S_1", 12.0f},
    {"c_n_moa_a", "S_1", 13.0f},
    {"c_n_moa_a", "S_1", 14.0f},
    {"c_n_moa_a", "S_1", 15.0f},
    {"c_n_moa_a", "S_1", 16.0f},
    {"c_n_moa_a", "S_1", 17.0f},
    {"c_n_moa_a", "S_1", 18.0f},
    {"c_n_moa_a", "S_1", 19.0f},
    {"c_n_moa_a", "S_1", 20.0f},
    {"c_n_moa_a", "S_1", 21.0f},
    {"c_n_moa_a", "S_1", 22.0f},
    {"c_n_moa_a", "S_1", 23.0f},
    {"c_n_moa_a", "S_1", 24.0f},
    {"c_n_moa_a", "S_1", 25.0f},
    {"c_n_moa_a", "S_1", 26.0f},
    {"c_n_moa_a", "S_1", 27.0f},
    {"c_n_moa_a", "S_1", 28.0f},
    {"c_n_moa_a", "S_1", 29.0f},
    {"c_n_moa_a", "S_1", 30.0f},
    {"c_n_moa_a", "S_1", 31.0f},
    {"c_n_moa_a", "S_1", 32.0f},
    {"c_n_moa_a", "S_1", 33.0f},
    {"c_n_moa_a", "S_1", 34.0f},
    {"c_n_moa_a", "S_1", 35.0f},
    {"c_n_moa_a", "S_1", 36.0f},
    {"c_n_moa_a", "S_1", 37.0f},
    {"c_n_moa_a", "S_1", 38.0f},
    {"c_n_moa_a", "S_1", 39.0f},
    {"c_n_moa_b", "S_1", 0.0f},
    {"c_n_moa_b", "S_1", 1.0f},
    {"c_n_moa_b", "S_1", 2.0f},
    {"c_n_moa_b", "S_1", 3.0f},
    {"c_n_moa_b", "S_1", 4.0f},
    {"c_n_moa_b", "S_1", 5.0f},
    {"c_n_moa_b", "S_1", 6.0f},
    {"c_n_moa_b", "S_1", 7.0f},
    {"c_n_moa_b", "S_1", 8.0f},
    {"c_n_moa_b", "S_1", 9.0f},
    {"c_n_moa_b", "S_1", 10.0f},
    {"c_n_moa_b", "S_1", 11.0f},
    {"c_n_moa_b", "S_1", 12.0f},
    {"c_n_moa_b", "S_1", 13.0f},
    {"c_n_moa_b", "S_1", 14.0f},
    {"c_n_moa_b", "S_1", 15.0f},
    {"c_n_moa_b", "S_1", 16.0f},
    {"c_n_moa_b", "S_1", 17.0f},
    {"c_n_moa_b", "S_1", 18.0f},
    {"c_n_moa_b", "S_1", 19.0f},
    {"c_n_moa_b", "S_1", 20.0f},
    {"c_n_moa_b", "S_1", 21.0f},
    {"c_n_moa_b", "S_1", 22.0f},
    {"c_n_moa_b", "S_1", 23.0f},
    {"c_n_moa_b", "S_1", 24.0f},
    {"c_n_moa_b", "S_1", 25.0f},
    {"c_n_moa_b", "S_1", 26.0f},
    {"c_n_moa_b", "S_1", 27.0f},
    {"c_n_moa_b", "S_1", 28.0f},
    {"c_n_moa_b", "S_1", 29.0f},
    {"c_n_moa_b", "S_1", 30.0f},
    {"c_n_moa_b", "S_1", 31.0f},
    {"c_n_moa_b", "S_1", 32.0f},
    {"c_n_moa_b", "S_1", 33.0f},
    {"c_n_moa_b", "S_1", 34.0f},
    {"c_n_moa_b", "S_1", 35.0f},
    {"c_n_moa_b", "S_1", 36.0f},
    {"c_n_moa_b", "S_1", 37.0f},
    {"c_n_moa_b", "S_1", 38.0f},
    {"c_n_moa_b", "S_1", 39.0f},
    {0},
};

#pragma dont_inline on
static void moamoa_pose_init(void) {
    ExtTexInfo* data = moamoa_data;
    s32 i = 0;
    s32 start;
    char model[32];
    char anim[32];

    strcpy(model, "");
    strcpy(anim, "");
    wp->numDefs = -1;
    do {
        if (strcmp(data->group, model)) {
            strcpy(model, data->group);
            strcpy(anim, "");
        }
        if (strcmp(data->anim, anim)) {
            strcpy(anim, data->anim);
            start = i;
            wp->numDefs++;
        }
        strcpy(wp->defs[wp->numDefs].model, model);
        strcpy(wp->defs[wp->numDefs].anim, anim);
        wp->defs[wp->numDefs].start = start;
        wp->defs[wp->numDefs].count = i - start + 1;
        wp->defs[wp->numDefs].speed = data->frame;
        wp->defs[wp->numDefs].loop = 1;
        if (!strncmp(anim, "J", 1)) {
            wp->defs[wp->numDefs].loop = 0;
        }
        i++;
    } while ((++data)->group != NULL);
    wp->numDefs++;
}

#pragma dont_inline reset
static void moamoa_pose_main(void) {
    f32 t;
    u64 now;
    s32 i;
    ExtdrvPose* pose;
    Moamoa* moa;
    MoamoaPoseDef* def;

    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++, pose++) {
        now = gp->renderTime;
        def = &wp->defs[moa->pose];
        t = 60.0f * OSTicksToMilliseconds(now - moa->startTime) / 1000.0f / def->speed;

        if (t >= 1.0f) {
            if (def->loop) {
                moa->startTime = now;
                t = 0.0f;
            } else {
                t = 1.0f;
            }
        }
        pose->texId = def->start + (s32)((def->count - 1) * t);
    }
}

static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim) {
    MoamoaPoseDef* def = wp->defs;
    s32 i;

    if (moa->flags & 2) {
        model = "c_n_moa_b";
    }
    for (i = 0; i < wp->numDefs; i++, def++) {
        if (!strcmp(def->model, model) && !strcmp(def->anim, anim)) {
            break;
        }
    }
    if (i < wp->numDefs && moa->pose != i) {
        moa->pose = i;
        moa->startTime = gp->renderTime;
    }
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

void moamoa_init(void) {
    s32 i;
    ExtdrvPose* pose;
    Moamoa* moa;
    f32 x, y, z;

    wp = _mapAlloc(sizeof(MoamoaWork));
    memset(wp, 0, sizeof(MoamoaWork));
    wp->count = extGetPoseNum();
    wp->moa = _mapAlloc(wp->count * sizeof(Moamoa));
    memset(wp->moa, 0, wp->count * sizeof(Moamoa));
    moamoa_pose_init();
    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        PSMTXIdentity(*pose->mtx);
        pose->flags |= 5;
        moa->flags = 0;
        moa->state = 0;
        moamoa_pose_change(moa, "c_n_moa_a", "S_1");
        if (!strncmp(gp->currentMapName, "rsh_00", 6)) {
            x = randf(-200.0f, 400.0f);
            if (x > 257.0f) {
                x = randf(257.0f, 262.0f);
            }
            y = randf(20.0f, 100.0f);
            if (x < 185.0f) {
                z = randf(-160.0f, -165.0f);
            } else if (x < 257.0f) {
#ifdef VERSION_G8ME01
                z = 0.8f * (x - 185.0f) + -160.0f;
                z += 50.0f;
#else
                z = 0.8f * (x - 185.0f) + -160.0f;
#endif
            } else {
#ifdef VERSION_G8ME01
                z = randf(-90.0f, -20.0f);
                z += 50.0f;
#else
                z = randf(-90.0f, -20.0f);
#endif
            }
            {
                Vec pos = {0.0f, 0.0f, 0.0f};

                pos.x = x;
                pos.y = y;
                pos.z = z - 100.0f;
                moa->pos = pos;
            }
            (*pose->mtx)[0][3] = 0.0f;
            (*pose->mtx)[1][3] = -1000.0f;
            (*pose->mtx)[2][3] = 0.0f;
            if (!(x < 185.0f)) {
                if (x < 257.0f) {
                    Mtx m;
                    PSMTXRotRad(m, 'y', -0.7853982f);
                    PSMTXConcat(*pose->mtx, m, *pose->mtx);
                    moa->flags |= 1;
                } else {
                    Mtx m;
                    PSMTXRotRad(m, 'y', 1.5707964f);
                    PSMTXConcat(*pose->mtx, m, *pose->mtx);
                    moa->flags |= 1;
                }
            }
        } else {
            if (!strncmp(gp->currentMapName, "rsh_03", 6)) {
                x = randf(-500.0f, 500.0f);
                y = randf(20.0f, 100.0f) - 10.0f;
                z = randf(-170.0f, -180.0f);
                z -= 100.0f;
            } else {
                x = randf(-350.0f, 350.0f);
                y = randf(20.0f, 100.0f);
                z = randf(-290.0f, -300.0f);
            }
            {
                Vec pos = {0.0f, 0.0f, 0.0f};

                pos.x = x;
                pos.y = y;
                pos.z = z;
                moa->pos = pos;
            }
            (*pose->mtx)[0][3] = 0.0f;
            (*pose->mtx)[1][3] = -1000.0f;
            (*pose->mtx)[2][3] = 0.0f;
        }
    }
}

static void ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

void moamoa_main(void) {
    s32 i;
    ExtdrvPose* pose;
    Moamoa* moa;
    f32 r;

    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        switch (moa->state) {
            case 0:
                break;
            case 1:
                moa->timer = rand() % 60;
                moa->state++;
                break;
            case 2:
                if (--moa->timer > 0) {
                    break;
                }
                moa->state++;
                moa->angle = randf(0.0f, 6.283184f);
                break;
            case 3:
                if (moa->timer < 20) {
                    if (!(moa->flags & 1)) {
                        r = intplGetValue(2, 50.0f, 0.0f, ++moa->timer, 20);
                        (*pose->mtx)[0][3] = moa->pos.x + r * (f32)sin(moa->angle);
                        (*pose->mtx)[1][3] = moa->pos.y - r * (f32)cos(moa->angle);
                        (*pose->mtx)[2][3] = moa->pos.z;
                    } else {
                        r = intplGetValue(2, 50.0f, 0.0f, ++moa->timer, 20);
                        (*pose->mtx)[0][3] = moa->pos.x;
                        (*pose->mtx)[1][3] = moa->pos.y - r * (f32)cos(moa->angle);
                        (*pose->mtx)[2][3] = moa->pos.z;
                    }
                } else {
                    moa->state++;
                }
                break;
            case 4:
                (*pose->mtx)[0][3] = moa->pos.x;
                (*pose->mtx)[1][3] = moa->pos.y;
                (*pose->mtx)[2][3] = moa->pos.z;
                break;
        }
        if ((*pose->mtx)[2][3] > 45.0f) {
            (*pose->mtx)[2][3] = 45.0f;
        }
    }
    moamoa_pose_main();
}

void moamoa_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, ext_disp, NULL);
}

USER_FUNC(evt_moamoa_set_one) {
    EvtWord* args = event->args;
    Moamoa* moa;
    s32 idx = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    Mtx m;
    ExtdrvPose* pose = extGetPosePtr();
    moa = wp->moa;
    moa[idx].state = 3;
    moa[idx].pos = (Vec){x, y, z};
    if (!(moa[idx].flags & 1)) {
        PSMTXRotRad(m, 'y', 1.5707964f);
        PSMTXConcat(*pose[idx].mtx, m, *pose[idx].mtx);
        moa[idx].flags |= 1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moamoa_set_seq_all) {
    s32 seq = evtGetValue(event, event->args[0]);
    s32 i;
    Moamoa* moa;

    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        moa->state = seq;
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_moamoa)
    CALL(evt_snd_bgmoff_f, 512, 120)
    CALL(evt_cam3d_evt_set, -70, 113, 373, 126, 38, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("運転手"), 32)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 85, -110, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 115, -110, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_set_dir_to_target, STRING("運転手"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_231"), 0, STRING("me"))
    CALL(evt_npc_flag_onoff, 0, STRING("運転手"), 32)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_MOAMOA_VOICE1"), 265, 25, -145, 0)
    CALL(evt_moamoa_set_one, 0, 265, 25, -145)
    CALL(evt_moamoa_set_one, 1, 265, 35, -230)
    WAIT_MS(1000)
    CALL(evt_npc_reverse_ry, STRING("me"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_232"), 0, STRING("me"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SURPRISED1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG6_MOAMOA_VOICE2"), LW(10))
        WAIT_MS(1000)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_moamoa_set_seq_all, 1)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 329)
    LOOP(3)
        CALL(evt_cam3d_evt_set, -2, 86, 230, 126, 38, -13, 350, 11)
        WAIT_MS(350)
        CALL(evt_cam3d_evt_set, -70, 113, 373, 126, 38, -13, 350, 11)
        WAIT_MS(350)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("a"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("b"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), 1536)
    CALL(evt_npc_set_anim, STRING("me"), STRING("B_1"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("T_2"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_233"), 0, STRING("me"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("B_2"))
    CALL(evt_cam3d_evt_set, -2, 86, 230, 126, 38, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_234_6"), 0, STRING("party"))
    END_SWITCH
    CALL(evt_party_run, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG6_MOAMOA_VOICE2"), LW(10))
    CALL(evt_snd_sfx_vol, LW(10), 88)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_235_00"), 0, STRING("me"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    RETURN
EVT_END()

EVT_BEGIN(evt_rsh_moamoa_on_window)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(329, 331)
        CALL(evt_snd_sfxon, STRING("SFX_STG6_MOAMOA_VOICE2"), LW(10))
        CALL(evt_snd_sfx_vol, LW(10), 88)
        CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a"))
        CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
        CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b"))
        CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
        CALL(evt_ext_entry, 1000, PTR(moamoa_data), PTR(moamoa_init), PTR(moamoa_main), PTR(moamoa_dispent))
        CALL(evt_moamoa_set_seq_all, 4)
    END_SWITCH
    RETURN
EVT_END()

static BeroEntry bero_data_a[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_b", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_c", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_handle)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 329)
            IF_INT_LESS(GSW_STORY_PROGRESS, 334)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(0)
            ADD(LW(0), 10)
            MODULO(LW(0), 360)
            CALL(evt_mapobj_rotate, 1, STRING("handoru_1"), 0, LW(0), 0)
            SET_VALUE(LW(1), LW(0))
            MULTIPLY(LW(1), -1)
            CALL(evt_mapobj_rotate, 1, STRING("handoru_2"), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, 11, 0, -40, 300)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("handoru_3"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_sub_intpl_msec_init, 11, -40, 0, 300)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("handoru_3"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(regu_unten)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 310)
            IF_INT_LESS(GSW_STORY_PROGRESS, 326)
                RETURN
            END_IF
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 329)
            IF_INT_LESS(GSW_STORY_PROGRESS, 339)
                RETURN
            END_IF
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 346)
            IF_INT_EQUAL(GSWF(3579), 1)
                RETURN
            END_IF
            IF_INT_EQUAL(GSWF(3581), 1)
                RETURN
            END_IF
            IF_INT_EQUAL(GSWF(3580), 1)
                RETURN
            END_IF
        END_IF
        CALL(evt_sub_random, 300, LW(10))
        SWITCH(LW(10))
            CASE_LESS(100)
            IF_INT_LESS(GSW_STORY_PROGRESS, 328)
            END_IF
            SET_VALUE(LW(2), LW(3))
            CALL(evt_sub_random, 135, LW(10))
            SUBTRACT(LW(10), 45)
            SET_VALUE(LW(3), LW(10))
            CALL(evt_sub_intpl_msec_init, 11, LW(2), LW(3), 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING("handoru_1"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CASE_LESS(200)
            IF_INT_LESS(GSW_STORY_PROGRESS, 328)
            END_IF
            SET_VALUE(LW(4), LW(5))
            CALL(evt_sub_random, 40, LW(10))
            MULTIPLY(LW(10), -1)
            SET_VALUE(LW(5), LW(10))
            CALL(evt_sub_intpl_msec_init, 11, LW(4), LW(5), 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("handoru_3"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CASE_DEFAULT
            IF_INT_LESS(GSW_STORY_PROGRESS, 328)
            END_IF
            SET_VALUE(LW(6), LW(7))
            CALL(evt_sub_random, 135, LW(10))
            SUBTRACT(LW(10), 90)
            SET_VALUE(LW(7), LW(10))
            CALL(evt_sub_intpl_msec_init, 11, LW(6), LW(7), 1000)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_rotate, 1, STRING("handoru_2"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_SWITCH
        CALL(evt_sub_random, 2, LW(10))
        ADD(LW(10), 1)
        MULTIPLY(LW(10), 1000)
        WAIT_MS(LW(10))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(init_unten)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(329, 331)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 1536)
        CALL(evt_npc_set_anim, STRING("me"), STRING("B_2"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_unten)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(288)
        IF_INT_EQUAL(LSWF(1), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_00"), 0, 0)
            SET_VALUE(LSWF(1), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_01"), 0, 0)
            SET_VALUE(LSWF(1), 0)
        END_IF
        CASE_LESS(292)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_01_01"), 0, 0)
        CASE_LESS(293)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_108"), 0, STRING("me"))
        CALL(evt_npc_set_anim, STRING("me"), STRING("A_1"))
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_DRIVER_SIGN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("A_2"))
        WAIT_MS(2000)
        CALL(evt_npc_set_anim, STRING("me"), STRING("A_3"))
        WAIT_MS(500)
        CALL(evt_item_entry, STRING("sign"), 77, 0, 0, 0, 17, -1, 0)
        CALL(evt_item_get_item, STRING("sign"))
        CALL(evt_item_get_item_end_wait, STRING("sign"))
        CALL(evt_npc_set_anim, STRING("me"), STRING("T_1"))
        CALL(evt_msg_print, 0, STRING("stg6_rsh_110"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, 293)
        CASE_LESS(294)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_01_01"), 0, 0)
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_01_02"), 0, 0)
        CASE_LESS(303)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_01"), 0, 0)
        CASE_LESS(310)
        IF_INT_EQUAL(LSWF(2), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_152"), 0, 0)
            SET_VALUE(LSWF(2), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_153"), 0, 0)
        END_IF
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_209"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_217"), 0, 0)
        CASE_LESS(329)
        CALL_CHILD(PTR(evt_moamoa))
        CASE_LESS(332)
        CALL(evt_npc_set_anim, STRING("me"), STRING("T_2"))
        CALL(evt_msg_print, 0, STRING("stg6_rsh_235"), 0, 0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("B_2"))
        CASE_LESS(346)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_253"), 0, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(3580), 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_c_08_1"), 0, 0)
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF(3578), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_c_07"), 0, 0)
            ELSE
            IF_INT_EQUAL(GSWF(3581), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_08"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_08_1"), 0, 0)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"運転手"*/"\x89\x5E\x93\x5D\x8E\xE8", // name
        0x40020610, // flags
        0, // reactionFlags
        init_unten, // initEvent
        regu_unten, // regularEvent
        talk_unten, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static const char* access_tbl[] = {
    "nittei",
    "stg6_rsh_259",
    "soujyuu",
    "stg6_rsh_260",
    NULL,
};

static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
        READ_INT2(LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 9, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CASE_GREATER_OR_EQUAL(333)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_hosi"), 1, 0)
    END_SWITCH
    CALL(evt_npc_setup, PTR(npc_data))
    CALL_CHILD(PTR(evt_access_entry))
    SPAWN_EVENT(PTR(evt_handle))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -210, 60, -145, 0, 0)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_NOT_EQUAL(LW(0), STRING("rsh_00_c"))
        CALL(evt_mobj_kururing_floor, STRING("kururin1"), -132, 0, -95, STRING("s_zentai"), 125, GSWF(5648))
        ELSE
        CALL(evt_mobj_kururing_floor, STRING("kururin1"), -132, 0, -95, STRING("s_yuka1"), 125, GSWF(5648))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(284, 286)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        CASE_BETWEEN(304, 309)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        CASE_BETWEEN(310, 325)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_BETWEEN(327, 328)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        CASE_BETWEEN(329, 331)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        CASE_BETWEEN(334, 344)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_00_a"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_00_b"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_00_c"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_a_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_a))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(328, 331)
        CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a"))
        CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
        CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b"))
        CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
        CALL(evt_ext_entry, 1000, PTR(moamoa_data), PTR(moamoa_init), PTR(moamoa_main), PTR(moamoa_dispent))
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 329)
            CALL(evt_moamoa_set_seq_all, 4)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_b_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_b))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt))
    RETURN
EVT_END()

static const char* window_tbl[] = {
    "mado_uturi_1",
    "mado_uturi_2",
    "mado_uturi_3",
    "mado_uturi_4",
    NULL,
};

EVT_BEGIN(rsh_00_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt))
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3579), 0)
        IF_INT_EQUAL(GSWF(3580), 0)
            IF_INT_EQUAL(GSWF(3581), 0)
                CALL(evt_map_playanim, STRING("yure"), 1, 0)
                IF_INT_EQUAL(GSWF(3578), 0)
                    CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
                    ELSE
                    CALL(evt_map_playanim, STRING("rt_hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_saboten"), 1, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -210, 60, -145, 0, 0)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_NOT_EQUAL(LW(0), STRING("rsh_00_c"))
        CALL(evt_mobj_kururing_floor, STRING("kururin1"), -132, 0, -95, STRING("s_zentai"), 125, GSWF(5648))
        ELSE
        CALL(evt_mobj_kururing_floor, STRING("kururin1"), -132, 0, -95, STRING("s_yuka1"), 125, GSWF(5648))
    END_IF
    CALL_CHILD(PTR(evt_access_entry))
    IF_INT_EQUAL(GSWF(3460), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        ELSE
        IF_INT_EQUAL(GSWF(3461), 1)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envoff, 512)
            CALL(evt_snd_bgmoff, 26624)
            ELSE
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_00_a"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_00_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_00_c"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_4"))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_a_init_evt_nidome)
    SET_VALUE(LW(0), PTR(bero_data_a))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt_nidome))
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_b_init_evt_nidome)
    SET_VALUE(LW(0), PTR(bero_data_b))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt_nidome))
    RETURN
EVT_END()

EVT_BEGIN(rsh_00_c_init_evt_nidome)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_00_init_evt_nidome))
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    RETURN
EVT_END()

