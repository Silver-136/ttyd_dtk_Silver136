#include "driver/imgdrv.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "libc_decls.h"
#include "party/party_nokonoko.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "poly_trig.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr.h"
#include "driver/mapdrv.h"
#include "string.h"
#include "memory.h"
#include "dolphin/os.h"
#include "dolphin/mtx.h"
#include "mario/mariost.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "dolphin/gx.h"
#include "driver/camdrv.h"
#include "driver/offscreendrv.h"
#include "system.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "driver/dispdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/extdrv.h"
#include "moamoa.h"

static MoamoaWork* wp;
static f32 randf(f32 min, f32 max);
static void moamoa_pose_init(void);
static void moamoa_pose_main(void);
static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim);
extern const char* g_moamoa_chain_tbl[][2];
static void ext_moamoa_main(void);
#pragma inline_max_total_size(100000)
// Same body as polySinf/polyCosf (poly_trig.h), kept as a second pair: using the shared pair for these calls too
// changes the code, so this unit inlined two distinct copies.
static inline f32 moa_sin(f32 x) {
    f32 a = 0.00761f;
    f32 b = 0.16605f;
    f32 c = 1.0f;
    f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
        } else {
            x = 1.5707964f - (x - 1.5707964f);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
        }
    } else if (x < 4.712389f) {
        x = x - 3.1415927f;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * x;
        a = -a;
    } else {
        x = 1.5707964f - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * x;
        a = -a;
    }
    return a;
}

static inline f32 moa_cos(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
        } else {
            x = 1.5707964f - (x - 1.5707964f);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
        }
    } else if (x < 4.712389f) {
        x = x - 3.1415927f;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
    } else {
        x = 1.5707964f - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
    }
    return a;
}

static s32 moamoa_hit_check(ExtdrvPose* pose, Moamoa* moa);
static void moamoa_jump(ExtdrvPose* pose, Moamoa* moa, s32 next, s32 checkGround, f32 x, f32 y, f32 z, f32 speed, f32 gravity);
static void ext_disp(CameraId camera, void* param);
static void ext_shadow_disp(CameraId camera, void* param);

s32 evt_map_set_tevcallback(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_rotate(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_dispflag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_pos(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_blow(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_flag_onoff(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_fusion(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_fusion_debug(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_kind_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_reverse_ry(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_get_mapname(EventEntry* event, BOOL isFirstCall);
static void ext_moamoa_dispent(void);
static void ext_moamoa_init(void);
static s32 evt_moamoa_target_off(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_end_wait(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_ry_reset(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_exit(EventEntry* event, BOOL isFirstCall);

s32 evt_item_delete_check(EventEntry* event, BOOL isFirstCall);
s32 evt_item_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_item_get_position(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_bound_rate(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_gravity(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_jump_power(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_move_dir_speed(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_position(EventEntry* event, BOOL isFirstCall);
s32 evt_map_checkanim(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_clear_offscreen(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_set_offscreen(EventEntry* event, BOOL isFirstCall);
s32 evt_mobj_switch_blue(EventEntry* event, BOOL isFirstCall);
s32 evt_party_jump_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_pouch_remove_item(EventEntry* event, BOOL isFirstCall);
s32 evt_win_item_select(EventEntry* event, BOOL isFirstCall);
s32 img_set_color(EventEntry* event, BOOL isFirstCall);

static ExtTexInfo ext_moamoa_data[47];
static EvtWord e_door_close[12];
static EvtWord e_door_open[12];
static EvtWord s_door_close[22];
static EvtWord s_door_open[22];
static BeroEntry bero_data[3];
EvtWord evt_bridge[14];
EvtWord evt_bridge2[118];
static EvtWord evt_origami[198];
static EvtWord evt_moamoa_init[29];
static EvtWord evt_meet_moamoa[215];
static EvtWord evt_moamoa_escape[47];
EvtWord evt_elv_rumble[60];
static EvtWord evt_elv_init[49];
EvtWord evt_elv_down[554];
EvtWord evt_elv_up[456];
static s32 item_tbl[2];
EvtWord evt_elv[61];
EvtWord evt_elv_nomsg[14];
static EvtWord evt_head[42];
static EvtWord evt_kanban[14];
EvtWord eki_00_init_evt[99];

static ExtTexInfo ext_moamoa_data[] = {
    {"c_n_moa_a", "S_1", 0.0f},
    {"c_n_moa_a", "S_1", 2.0f},
    {"c_n_moa_a", "S_1", 4.0f},
    {"c_n_moa_a", "S_1", 6.0f},
    {"c_n_moa_a", "S_1", 8.0f},
    {"c_n_moa_a", "S_1", 10.0f},
    {"c_n_moa_a", "S_1", 12.0f},
    {"c_n_moa_a", "S_1", 14.0f},
    {"c_n_moa_a", "S_1", 16.0f},
    {"c_n_moa_a", "S_1", 18.0f},
    {"c_n_moa_a", "S_1", 20.0f},
    {"c_n_moa_a", "S_1", 22.0f},
    {"c_n_moa_a", "S_1", 24.0f},
    {"c_n_moa_a", "S_1", 26.0f},
    {"c_n_moa_a", "S_1", 28.0f},
    {"c_n_moa_a", "S_1", 30.0f},
    {"c_n_moa_a", "S_1", 32.0f},
    {"c_n_moa_a", "S_1", 34.0f},
    {"c_n_moa_a", "S_1", 36.0f},
    {"c_n_moa_a", "S_1", 38.0f},
    {"c_n_moa_a", "D_1", 0.0f},
    {"c_n_moa_a", "D_1", 4.0f},
    {"c_n_moa_a", "D_1", 8.0f},
    {"c_n_moa_b", "S_1", 0.0f},
    {"c_n_moa_b", "S_1", 2.0f},
    {"c_n_moa_b", "S_1", 4.0f},
    {"c_n_moa_b", "S_1", 6.0f},
    {"c_n_moa_b", "S_1", 8.0f},
    {"c_n_moa_b", "S_1", 10.0f},
    {"c_n_moa_b", "S_1", 12.0f},
    {"c_n_moa_b", "S_1", 14.0f},
    {"c_n_moa_b", "S_1", 16.0f},
    {"c_n_moa_b", "S_1", 18.0f},
    {"c_n_moa_b", "S_1", 20.0f},
    {"c_n_moa_b", "S_1", 22.0f},
    {"c_n_moa_b", "S_1", 24.0f},
    {"c_n_moa_b", "S_1", 26.0f},
    {"c_n_moa_b", "S_1", 28.0f},
    {"c_n_moa_b", "S_1", 30.0f},
    {"c_n_moa_b", "S_1", 32.0f},
    {"c_n_moa_b", "S_1", 34.0f},
    {"c_n_moa_b", "S_1", 36.0f},
    {"c_n_moa_b", "S_1", 38.0f},
    {"c_n_moa_b", "D_1", 0.0f},
    {"c_n_moa_b", "D_1", 4.0f},
    {"c_n_moa_b", "D_1", 8.0f},
    {0},
};

void envAddTev(s32 type, MapTevCallbackInfo* info);

#pragma dont_inline on
static void moamoa_pose_init(void) {
    ExtTexInfo* data = ext_moamoa_data;
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
        wp->defs[wp->numDefs].frames = data->frame;
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
        t = (60.0f) * OSTicksToMilliseconds(now - moa->startTime) / (1000.0f) / def->frames;

        if (t >= (1.0f)) {
            moa->startTime = now;
            t = (0.0f);
        }
        pose->texId = def->start + (s32)((def->count - 1) * t);
    }
}

static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim) {
    static const char* group_tbl[][2] = {
        {"c_n_moa_a", "S_1"},
        {"c_n_moa_b", "S_1"},
        {"c_kinodebu", "G_1"},
        {"c_puku_c", "U_1"},
        {"c_powan", "O_1"},
        {"c_smoko", "T_2"},
        {"c_rose", "G_1"},
        {"c_papab", "G_1"},
        {"c_mamab", "G_1"},
        {"c_konari", "G_1"},
        {"c_n_moa_a", "S_1"},
    };
    MoamoaPoseDef* def = wp->defs;
    s32 i;

    if (moa->flags & MOAMOA_FLAG_SHOW_KIND) {
        model = group_tbl[moa->kind][0];
        if (moa->kind >= MOAMOA_KIND_KINOPIO) {
            anim = group_tbl[moa->kind][1];
        }
        if (moa->kind == MOAMOA_KIND_MOA_A_S2_UNUSED) {
            anim = "S_2";
        }
    }
    for (i = 0; i < wp->numDefs; i++, def++) {
        if (!strcmp(def->model, model) && !strcmp(def->anim, anim)) {
            break;
        }
    }
    if (i < wp->numDefs && moa->pose != i) {
        if (moa->flags & MOAMOA_FLAG_HIT) {
            if (strncmp(anim, "D", 1)) {
                return;
            }
        }
        if (moa->flags & MOAMOA_FLAG_ESCAPING) {
            if (strncmp(anim, "R", 1)) {
                return;
            }
        }
        moa->pose = i;
        moa->startTime = gp->renderTime;
    }
}

static s32 evt_moamoa_flag_onoff(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);

    if (on) {
        wp->flags |= mask;
    } else {
        wp->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_exit(EventEntry* event, BOOL isFirstCall) {
    extReset();
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_ry_reset(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    for (i = 0; i < wp->count; i++, pose++) {
        pose->ry = (0.0f);
        pose->targetRy = (0.0f);
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_end_wait(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    Moamoa* moa;

    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        if (!(moa->flags & MOAMOA_FLAG_GONE)) {
            return EVT_RETURN_BLOCK;
        }
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_fusion(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    ExtdrvPose* pose;
    Moamoa* moa;

    if (isFirstCall) {
        pose = extGetPosePtr();
        moa = wp->moa;
        for (i = 0; i < wp->count; i++, pose++, moa++) {
            (*pose->mtx)[2][2] = (1.0f);
            (*pose->mtx)[1][1] = (1.0f);
            (*pose->mtx)[0][0] = (1.0f);
            moa->next = MOAMOA_STATE_FUSION_WAIT;
            moa->state = MOAMOA_STATE_FUSION_WAIT;
            moa->timer = i < 100 ? i % 100 : i % 60 + 100;
            moa->flags |= MOAMOA_FLAG_FUSING;
        }
    }
    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        if (!(moa->flags & MOAMOA_FLAG_FUSION_ARRIVED)) {
            return EVT_RETURN_BLOCK;
        }
    }
    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        pose->flags |= EXT_POSE_FLAG_NO_SHADOW;
        moa->flags |= MOAMOA_FLAG_FUSED;
        moa->state = MOAMOA_STATE_FUSED;
        moamoa_pose_change(moa, "c_n_moa_a", "S_1");
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_kind_onoff(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 all = evtGetValue(event, args[1]);
    s32 i;
    Moamoa* moa;

    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        if (on) {
            if (!all) {
                if (moa->kind == MOAMOA_KIND_MOA_B) {
                    moa->flags |= MOAMOA_FLAG_SHOW_KIND;
                }
            } else {
                moa->flags |= MOAMOA_FLAG_SHOW_KIND;
            }
        } else {
            if (!all) {
                if (moa->kind != MOAMOA_KIND_MOA_B) {
                    moa->flags &= ~MOAMOA_FLAG_SHOW_KIND;
                }
            } else {
                moa->flags &= ~MOAMOA_FLAG_SHOW_KIND;
            }
        }
        moamoa_pose_change(moa, "c_n_moa_a", "S_1");
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_blow(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    Moamoa* moa;

    evtGetValue(event, event->args[0]);
    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        moa->flags &= ~MOAMOA_FLAG_FUSED;
        moa->state = MOAMOA_STATE_BLOW_START;
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_fusion_debug(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();
    Moamoa* moa = wp->moa;

    for (i = 0; i < wp->count; i++, pose++, moa++) {
        pose->flags |= EXT_POSE_FLAG_NO_SHADOW;
        moa->flags |= MOAMOA_FLAG_FUSED;
        moa->flags |= MOAMOA_FLAG_FUSING;
        moa->flags |= MOAMOA_FLAG_FUSION_ARRIVED;
        moa->state = MOAMOA_STATE_FUSED;
        moa->flags |= MOAMOA_FLAG_SHOW_KIND;
        moamoa_pose_change(moa, "c_n_moa_a", "S_1");
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_target_off(EventEntry* event, BOOL isFirstCall) {
    s32 i = evtGetValue(event, event->args[0]);

    wp->target[i] = wp->target[1 - i];
    wp->targetHits[i] = 100;
    if (i == 0) {
        wp->flags |= MOAMOA_WORK_FLAG_TARGET0_CLEARED;
    } else {
        wp->flags |= MOAMOA_WORK_FLAG_TARGET1_CLEARED;
    }
    return EVT_RETURN_DONE;
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / (32767.0f);

    return r * (max - min) + min;
}
#pragma opt_lifetimes off
__declspec(section ".rodata") static Vec ext_moamoa_t0 = {0.0f, 0.0f, 10.0f};
__declspec(section ".rodata") static Vec ext_moamoa_t1 = {0.0f, 0.0f, 10.0f};
__declspec(section ".rodata") static Vec ext_moamoa_t2 = {0.0f, 0.0f, 10.0f};
static void ext_moamoa_init(void) {
    s32 i, j;
    ExtdrvPose* pose;
    Moamoa* moa;
    f32 rad;
    f32 hitX, hitY, hitZ, dist, hd, nx, ny, nz;

    wp = _mapAlloc(sizeof(MoamoaWork));
    memset(wp, 0, sizeof(MoamoaWork));
    wp->target[0] = ext_moamoa_t0;
    wp->target[1] = ext_moamoa_t1;
    wp->target[2] = ext_moamoa_t2;
    wp->count = extGetPoseNum();
    wp->moa = _mapAlloc(wp->count * sizeof(Moamoa));
    memset(wp->moa, 0, wp->count * sizeof(Moamoa));
    moamoa_pose_init();
    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        moa->flags = 0;
        moa->state = MOAMOA_STATE_WANDER;
        moa->timer = 0;
        moamoa_pose_change(moa, "c_n_moa_a", "S_1");
        rad = randf(0.0f, 6.2831855f);
        dist = randf(0.0f, 100.0f);
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_BILLBOARD | EXT_POSE_FLAG_SHADOW_POS);
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = wp->target[0].x + dist * moa_sin(rad);
        (*pose->mtx)[1][3] = 0.0f;
        // BUG: writes [0][3] (x) a second time, so x gets the z value and z stays 0 from PSMTXIdentity
        // ([2][3] was presumably intended); the ground probe below then starts from that point.
        (*pose->mtx)[0][3] = wp->target[0].z - dist * moa_cos(rad);
        hitX = hitY = hitZ = 0.0f;
        hd = 1000000.0f;
        if (hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hd, &nx, &ny, &nz, (*pose->mtx)[0][3], 100.0f + (*pose->mtx)[1][3], (*pose->mtx)[2][3], 0.0f, -1.0f, 0.0f)) {
            (*pose->mtx)[1][3] = hitY;
        }
    }
    for (j = 0; j < 600; j++) {
        ext_moamoa_main();
    }
}
#pragma opt_lifetimes reset
static void ext_disp(CameraId camera, void* param) {
    u16 left, top, right, bottom;

    if (camera == CAMERA_OFFSCREEN2) {
        GXClearBoundingBox();
    }
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    if (camera == CAMERA_OFFSCREEN2) {
        sysWaitDrawSync();
        GXReadBoundingBox(&left, &top, &right, &bottom);
        offscreenAddBoundingBox(offscreenNameToId("offscreen"), left, top, right, bottom);
    }
}

static void ext_shadow_disp(CameraId camera, void* param) {
    static Mtx mt = {
        {8.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 8.0f, 0.0f, 0.5f},
        {0.0f, 0.0f, 8.0f, 0.0f},
    };
    u16 left, top, right, bottom;

    if (camera == CAMERA_OFFSCREEN2) {
        GXClearBoundingBox();
    }
    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    extDrawShadow();
    if (camera == CAMERA_OFFSCREEN2) {
        sysWaitDrawSync();
        GXReadBoundingBox(&left, &top, &right, &bottom);
        offscreenAddBoundingBox(offscreenNameToId("offscreen"), left, top, right, bottom);
    }
}
#pragma dont_inline on
static void moamoa_jump(ExtdrvPose* pose, Moamoa* moa, s32 next, s32 checkGround, f32 x, f32 y, f32 z, f32 speed, f32 gravity) {
    f32 hitX, hitY, hitZ, dist, nx, ny, nz;
    f32 dy;
    f32 d;
    f32 angle;

    if (checkGround) {
        hitX = hitY = hitZ = 0.0f;
        dist = 1000000.0f;
        if (hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &dist, &nx, &ny, &nz, x, 100.0f + y, z, 0.0f, -1.0f, 0.0f)) {
            if (__fabs(y - hitY) > 30.0) {
                speed -= 0.1f;
                gravity += 0.1f;
            }
            y = hitY;
        }
    }
    moa->target = (Vec){x, y, z};
    moa->speed = speed;
    moa->gravity = gravity;
    dy = moa->target.y - (*pose->mtx)[1][3];
    d = distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], moa->target.x, moa->target.z);
    angle = angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], moa->target.x, moa->target.z);
    moa->jumpTimer = d / moa->speed;
    moa->vy = dy / moa->jumpTimer + moa->gravity * moa->jumpTimer / 2;
    moa->state = MOAMOA_STATE_JUMP;
    moa->next = next;
    pose->dir = 180.0f + angle;
    moa->flags |= MOAMOA_FLAG_JUMPING;
    pose->flags |= EXT_POSE_FLAG_SHADOW_POS;
    pose->flags &= ~EXT_POSE_FLAG_NO_SHADOW;
}
#pragma dont_inline reset
#pragma opt_lifetimes off
static s32 moamoa_hit_check(ExtdrvPose* pose, Moamoa* moa) {
    MarioWork* mario = marioGetPtr();
    CameraEntry* cam;
    f32 range;
    f32 dist;
    f32 angle;
    f32 rad;
    Vec a, b, c, d, e, f;

    if (mario->position.y > 20.0f + (*pose->mtx)[1][3]) {
        return 0;
    }
    if ((*pose->mtx)[1][3] > mario->position.y + mario->wPlayerCollisionBox.y) {
        return 0;
    }
    if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], mario->position.x, mario->position.z) < (mario->wPlayerCollisionBox.x + 20.0f) / 2
        && mario->position.y > 10.0f + (*pose->mtx)[1][3] && (mario->flags & MARIO_FLAG_IS_FALLING) && !(mario->flags & MARIO_FLAG_IS_JUMPING)
        && wp->bounceCount < 2) {
        if (mario->currMotionId != MARIO_MOTION_ROLL && mario->currMotionId != MARIO_MOTION_YOSHI) {
            marioChgMot(MARIO_MOTION_JUMP_SW);
        }
        wp->bounceCount++;
        return 1;
    }
    if (!(mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
        wp->bounceCount = 0;
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER && wp->hammerFrames > 10) {
        f32 r2;
        f32 t;
        cam = camGetPtr(CAMERA_3D);
        t = cam->viewYaw;
        r2 = 6.2831855f * reviseAngle((f32)(mario->displayDirection - 90.0f) + t) / 360.0f;
        angle = 35.0f * moa_sin(r2) + mario->position.x;
        dist = mario->position.z - 35.0f * moa_cos(r2);
        if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], angle, dist) < 35.0f) {
            return 2;
        }
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER_2 && (s32)mario->currSubMotionId == 0x14) {
        f32 (*m)[4] = *pose->mtx;
        a = (Vec){m[0][3], m[1][3], m[2][3]};
        if (PSVECDistance(&mario->position, &a) < 65.0f) {
            return 2;
        }
    }
    if (wp->breathDir >= 0.0f) {
        f32 (*m)[4] = *pose->mtx;
        b = (Vec){m[0][3], m[1][3], m[2][3]};
        if (PSVECDistance(&wp->breathPos, &b) < 30.0f) {
            return 3;
        }
    }
    if (nokonokoGetStatus(partyGetPtr(0)) == 2) {
        f32 (*m)[4] = *pose->mtx;
        c = (Vec){m[0][3], m[1][3], m[2][3]};
        if (PSVECDistance(&partyGetPtr(0)->position, &c) < 30.0f) {
            return 2;
        }
    }
    if (partyGetPtr(0)->currentMemberId == PARTY_MEMBER_BOBBERY && partyGetPtr(0)->useMotionId == 0x46) {
        f32 (*m)[4] = *pose->mtx;
        d = (Vec){m[0][3], m[1][3], m[2][3]};
        if (PSVECDistance(&partyGetPtr(0)->position, &d) < 50.0f) {
            return 4;
        }
    }
    e = (Vec){mario->position.x, (*pose->mtx)[1][3], mario->position.z};
    f = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
    range = (mario->wPlayerCollisionBox.x + 20.0f) / 2;
    if (PSVECDistance(&e, &f) < range) {
        (*pose->mtx)[0][3] = moa->lastX;
        (*pose->mtx)[2][3] = moa->lastZ;
        {
            f32 (*m)[4] = *pose->mtx;
            f = (Vec){m[0][3], m[1][3], m[2][3]};
        }
        dist = PSVECDistance(&e, &f);
        if (dist < range) {
            angle = angleABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
            if (__fabsf(compAngle(angle, mario->directionView)) < 40.0f) {
                mario->floorMove.x = -((range - dist) * polySinf(6.2831855f * mario->directionView / 360.0f));
                mario->floorMove.z = (range - dist) * polyCosf(6.2831855f * mario->directionView / 360.0f);
            } else {
                rad = 6.2831855f * angle / 360.0f;
                mario->floorMove.x = (*pose->mtx)[0][3] - range * moa_sin(rad) - mario->position.x;
                mario->floorMove.z = (*pose->mtx)[2][3] + range * moa_cos(rad) - mario->position.z;
            }
        }
    }
    return 0;
}
#pragma opt_lifetimes reset
#pragma opt_lifetimes off
__declspec(section ".rodata") static Vec moamoa_zero_vec = {0.0f, 0.0f, 0.0f};
static void ext_moamoa_main(void) {
    MarioWork *mario = marioGetPtr();
    PartyEntry *party = partyGetPtr(0);
    CameraEntry *cam = camGetPtr(CAMERA_3D);
    s32 sndFlag;
    s32 i;
    s32 ret;
    ExtdrvPose *pose;
    Moamoa *moa;
    f32 a2;
    f32 rad;
    f32 angle;
    f32 x;
    f32 y;
    f32 dist;
    f32 z;
    f32 spd;
    f32 grav;
    f32 tx;
    f32 tz;
    f32 ty;
    Vec sp;
    Vec pv;
    Mtx rot;
    Mtx rot2;
    s32 hit;
    f32 a;
    f32 d;
    f32 r;
    f32 s;
    hit = 0;
    if (mario->currMotionId == MARIO_MOTION_HAMMER) {
        wp->hammerFrames++;
    } else {
        wp->hammerFrames = 0;
    }
    if (mario->currMotionId == MARIO_MOTION_STAY) {
        wp->idleFrames++;
        if (wp->idleFrames > 1000) {
            wp->idleFrames = 1000;
        }
    } else {
        wp->idleFrames = 0;
    }
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        wp->flags |= MOAMOA_WORK_FLAG_PARTY_IN_USE;
    } else {
        wp->flags &= ~MOAMOA_WORK_FLAG_PARTY_IN_USE;
    }
    wp->breathDir = -1.0f;
    wp->breathDist = -1.0f;
    if ((party->currentMemberId == PARTY_MEMBER_FLURRIE) && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        wp->breathDir = cloudGetBreathDir();
        wp->breathDist = cloudGetBreathDist();
        wp->breathPos.x = (wp->breathDist * polySinf((6.2831855f * wp->breathDir) / 360.0f)) + party->position.x;
        wp->breathPos.y = party->position.y;
        wp->breathPos.z = party->position.z - (wp->breathDist * polyCosf((6.2831855f * wp->breathDir) / 360.0f));
    }
    if (wp->hitCount > 250) {
        s32 j;
        Moamoa *mm;
        extGetPosePtr();
        for (j = 0, mm = wp->moa; j < wp->count; j++, mm++) {
            if (mm->flags & MOAMOA_FLAG_HIT) {
                continue;
            }
            if (mm->flags & MOAMOA_FLAG_ESCAPING) {
                break;
            }
            mm->state = MOAMOA_STATE_ESCAPE_TURN;
            mm->timer = 0;
            mm->flags |= MOAMOA_FLAG_ESCAPING;
        }

        if (wp->escapeEvt == NULL) {
            wp->escapeEvt = evtEntry(evt_moamoa_escape, 0, 0);
        }
    }
    pose = extGetPosePtr();
    sndFlag = 0;
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        wp->angle = angleABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
        wp->dist = distABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
        if ((((((wp->flags & MOAMOA_WORK_FLAG_ACTIVE) && (!(wp->flags & MOAMOA_WORK_FLAG_NO_HIT))) && (!marioKeyOffChk())) && (!marioCtrlOffChk())) && (!hit)) && (!(moa->flags & (MOAMOA_FLAG_HIT | MOAMOA_FLAG_ESCAPING | MOAMOA_FLAG_OFF_ROOF | MOAMOA_FLAG_FUSING)))) {
            ret = moamoa_hit_check(pose, moa);
            if (ret == 1) {
                moa->flags |= MOAMOA_FLAG_HIT;
                moa->state = MOAMOA_STATE_STOMPED;
                wp->hitCount++;
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                hit = 1;
            } else
              if ((ret == 2) || (ret == 4)) {
                moa->flags |= (MOAMOA_FLAG_HIT | MOAMOA_FLAG_KNOCKED_AWAY);
                moa->state = MOAMOA_STATE_KNOCKED_AWAY;
                wp->hitCount++;
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                if (ret != 4) {
                    hit = 1;
                }
            } else
              if (ret == 3) {
                angle = (wp->breathDir + wp->angle) / 2.0f;
                moa->flags |= (MOAMOA_FLAG_HIT | MOAMOA_FLAG_BLOWN);
                moa->state = MOAMOA_STATE_BLOWN_DRIFT;
                moa->target = moamoa_zero_vec;
                spd = randf(0.1f, 0.2f);
                rad = (6.2831855f * angle) / 360.0f;
                moa->vy = spd * polySinf(rad);
                moa->speed = randf(0.03f, 0.06f);
                moa->gravity = spd * (-polyCosf(rad));
                moa->jumpTimer = 120;
                wp->hitCount++;
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                hit = 1;
            }
        }
        if (((wp->flags & MOAMOA_WORK_FLAG_ACTIVE) && (!(moa->flags & (MOAMOA_FLAG_TO_TARGET | MOAMOA_FLAG_HIT | MOAMOA_FLAG_ESCAPING | MOAMOA_FLAG_OFF_ROOF | MOAMOA_FLAG_FUSING)))) && (wp->dist < 50.0f)) {
            if (moa->flags & MOAMOA_FLAG_JUMPING) {
                moa->next = MOAMOA_STATE_FLEE;
            } else {
                moa->state = MOAMOA_STATE_FLEE;
            }
        }
        if ((((wp->flags & MOAMOA_WORK_FLAG_ACTIVE) && (!(moa->flags & (MOAMOA_FLAG_HIT | MOAMOA_FLAG_OFF_ROOF | MOAMOA_FLAG_FUSED)))) && (i == (gp->retraceCount % wp->count))) && (irand(1000) < 200)) {
            Vec tmp = {0.0f, 0.0f, 0.0f};
            tmp.x = (*pose->mtx)[0][3];
            tmp.y = (*pose->mtx)[1][3];
            tmp.z = (*pose->mtx)[2][3];
            sp = tmp;
            psndSFXOn_3D(SFX_NAME_WORD("SFX_STG6_MOAMOA_VOICE1"), &sp); // FAKE: integer cast hoists the literal as an @ temp
        }
        switch (moa->state) {
            case MOAMOA_STATE_WANDER:
                if ((i % 50) < 20) {
                    f32 x;
                    f32 z;
                    f32 y;
                    f32 ax;
                    f32 az;
                    f32 ar;
                    rad = randf(0.0f, 6.2831855f);
                    dist = randf(0.0f, 50.0f);
                    ax = wp->target[i % 3].x + (dist * polySinf(rad));
                    az = wp->target[i % 3].z - (dist * polyCosf(rad));
                    ar = (6.2831855f * angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], ax, az)) / 360.0f;
                    x = (15.0f * polySinf(ar)) + (*pose->mtx)[0][3];
                    y = (*pose->mtx)[1][3];
                    z = (*pose->mtx)[2][3] - (15.0f * polyCosf(ar));
                    {
                        f32 k1;
                        f32 k2;
                        f32 r1 = rand() / 32767.0f;
                        f32 r2 = rand() / 32767.0f;
                        moamoa_jump(pose, moa, moa->state, 1, x, y, z, (r1 * (k1 = 3.0f - 1.5f)) + 1.5f, (r2 * (k2 = 2.0f - 0.8f)) + 0.8f);
                    }
                    moa->flags |= MOAMOA_FLAG_TO_TARGET;
                    break;
                }
                {
                    f32 z;
                    {
                        f32 d;
                        f32 a;
                        f32 r;
                        d = distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[i % 3].x, wp->target[i % 3].z);
                        a = angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[i % 3].x, wp->target[i % 3].z);
                        if (d < 100.0f) {
                            a = randf(0.0f, 360.0f);
                        }
                        r = (6.2831855f * a) / 360.0f;
                        rad = (5.0f * polySinf(r)) + (*pose->mtx)[0][3];
                        y = (*pose->mtx)[1][3];
                        z = (*pose->mtx)[2][3] - (5.0f * polyCosf(r));
                    }
                    {
                        f32 k1;
                        f32 k2;
                        f32 r1 = rand() / 32767.0f;
                        f32 r2 = rand() / 32767.0f;
                        moamoa_jump(pose, moa, moa->state, 1, rad, y, z, (r1 * (k1 = 0.5f - 0.3f)) + 0.3f, (r2 * (k2 = 0.8f - 0.5f)) + 0.5f);
                    }
                }
                moa->flags &= ~MOAMOA_FLAG_TO_TARGET;
                break;

            case MOAMOA_STATE_GATHER_WAIT_UNUSED:
                if ((--moa->timer) < 0) {
                    moa->timer = 0;
                    moa->state++;
                }
                break;

            case MOAMOA_STATE_GATHER_INIT_UNUSED:
                moa->speed = randf(0.0f, 200.0f);
                moa->gravity = randf(0.0f, (300.0f * moa->speed) / 200.0f);
                moa->state++;
                moa->timer = 0;

            case MOAMOA_STATE_GATHER_UNUSED:
                s = (moa->timer < 200) ? (1.0f) : ((800 - moa->timer) / 600.0f);
                if (s < 0.0f) {
                    s = 0.0f;
                    moa->flags |= MOAMOA_FLAG_FUSION_ARRIVED;
                }
                tx = wp->target[i % 3].x;
                ty = wp->target[i % 3].y;
                tz = wp->target[i % 3].z;
                tx += s * (moa->gravity * ((f32) sin(((f32) (i + gp->retraceCount)) / 4.0f)));
                r = i + gp->retraceCount;
                ty += moa->speed * s;
                tz += s * ((-moa->gravity) * ((f32) cos(r / 4.0f)));
                (*pose->mtx)[0][3] += (tx - (*pose->mtx)[0][3]) / 15.0f;
                (*pose->mtx)[1][3] += (ty - (*pose->mtx)[1][3]) / 15.0f;
                (*pose->mtx)[2][3] += (tz - (*pose->mtx)[2][3]) / 15.0f;
                moa->timer++;
                break;

            case MOAMOA_STATE_FLEE: {
                    f32 rad;
                    {
                        f32 k;
                        f32 r1 = rand() / 32767.0f;
                        f32 a = wp->angle + ((r1 * (k = 45.0f - -45.0f)) + -45.0f);
                        rad = (6.2831855f * a) / 360.0f;
                    }
                    d = wp->dist;
                    x = (*pose->mtx)[0][3] + (15.0f * polySinf(rad));
                    dist = (*pose->mtx)[1][3];
                    z = (*pose->mtx)[2][3] - (15.0f * polyCosf(rad));
                    { f32 hopSpeed; f32 k; f32 r1 = rand() / 32767.0f; hopSpeed = r1 * (k = 1.0f - 1.0f) + 1.0f;
                        // FAKE: block-local copies of the float args (behaviour-identical) put the f1..f5 arg moves ahead of the ?: as in the target
                        { f32 g2 = randf(0.4f, 0.4f); { f32 x2 = x; f32 y2 = dist; f32 z2 = z; f32 s2 = hopSpeed; f32 g3 = g2; moamoa_jump(pose, moa, (d < 100.0f) ? (moa->state) : (0), 1, x2, y2, z2, s2, g3); } } }
                }
                break;

            case MOAMOA_STATE_APPROACH: {
                    f32 z;
                    f32 x;
                    {
                        f32 r = randf(-45.0f, 45.0f);
                        a = wp->angle;
                        a += r;
                    }
                    a += 180.0f;
                    d = wp->dist;
                    x = (*pose->mtx)[0][3] + (30.0f * polySinf((6.2831855f * a) / 360.0f));
                    y = (*pose->mtx)[1][3];
                    z = (*pose->mtx)[2][3];
                    {
                        f32 b;
                        f32 k;
                        f32 r1 = rand() / 32767.0f;
                        f32 r2;
                        a2 = 2.0f * ((r1 * (k = 1.0f - 1.0f)) + 1.0f);
                        r2 = rand() / 32767.0f;
                        b = 2.0f * ((r2 * (k = 0.4f - 0.4f)) + 0.4f);
                        // FAKE: block-local copies of the float args (behaviour-identical) put the f1..f5 arg moves ahead of the ?: as in the target
                        {
                            f32 x2 = x;
                            f32 y2 = y;
                            f32 z2 = z;
                            f32 s2 = a2;
                            f32 g2 = b;
                            moamoa_jump(pose, moa, (d < 100.0f) ? (moa->state) : (0), 1, x2, y2, z2, s2, g2);
                        }
                    }
                    break;
                }

            case MOAMOA_STATE_JUMP:
                (*pose->mtx)[1][3] += moa->vy;
                moa->vy -= moa->gravity;
                {
                    f32 rad = (6.2831855f * angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], moa->target.x, moa->target.z)) / 360.0f;
                    (*pose->mtx)[0][3] += moa->speed * polySinf(rad);
                    (*pose->mtx)[2][3] = (*pose->mtx)[2][3] + (moa->speed * (-polyCosf(rad)));
                }
                if ((--moa->jumpTimer) < 0) {
                    (*pose->mtx)[0][3] = moa->target.x;
                    (*pose->mtx)[1][3] = moa->target.y;
                    (*pose->mtx)[2][3] = moa->target.z;
                    moa->flags &= ~MOAMOA_FLAG_JUMPING;
                    pose->flags &= ~EXT_POSE_FLAG_SHADOW_POS;
                    moa->timer = 0;
                    moa->state++;
                }
                break;

            case MOAMOA_STATE_LAND:
                if ((++moa->timer) > 4) {
                    moa->state = moa->next;
                }
                break;

            case MOAMOA_STATE_STOMPED:

            case MOAMOA_STATE_STOMPED_HOP2:

            case MOAMOA_STATE_STOMPED_HOP3: {
                    f32 tz;
                    if (!(moa->flags & MOAMOA_FLAG_TO_TARGET)) {
                        tx = mario->position.x;
                        tz = mario->position.z;
                    } else {
                        tx = wp->target[i % 3].x;
                        tz = wp->target[i % 3].z;
                    }
                    {
                        f32 z;
                        {
                            f32 rad;
                            f32 k;
                            s = rand() / 32767.0f;
                            {
                                f32 a3 = angleABf(tx, tz, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
                                a3 += ((s * (k = 45.0f - -45.0f)) + -45.0f);
                                rad = (6.2831855f * a3) / 360.0f;
                            }
                            ty = (*pose->mtx)[0][3] + (15.0f * polySinf(rad));
                            y = (*pose->mtx)[1][3];
                            z = (*pose->mtx)[2][3] - (15.0f * polyCosf(rad));
                        }
                        {
                            f32 k;
                            f32 r1 = rand() / 32767.0f;
                            f32 r2 = rand() / 32767.0f;
                            k = 1.0f - 1.0f;
                            moamoa_jump(pose, moa, moa->state + 1, 1, ty, y, z, (r1 * k) + 1.0f, (r2 * k) + 1.0f);
                        }
                    }
                }
                break;

            case MOAMOA_STATE_SHRINK:
                if ((++moa->timer) < 30) {
                    (*pose->mtx)[0][0] *= 0.9f;
                    (*pose->mtx)[1][1] *= 0.9f;
                    (*pose->mtx)[2][2] *= 0.9f;
                } else {
                    moa->state = MOAMOA_STATE_RESPAWN;
                }
                break;

            case MOAMOA_STATE_KNOCKED_AWAY: {
                    f32 rad;
                    s = rand() / 32767.0f;
                    dist = randf(300.0f, 500.0f);
                    { f32 k; rad = (6.2831855f * ((s * (k = 225.0f - 135.0f)) + 135.0f)) / 360.0f; }
                    x = (dist * polySinf(rad)) + (*pose->mtx)[0][3];
                    y = (*pose->mtx)[2][3] - (dist * polyCosf(rad));
                }
                {
                    f32 k;
                    f32 r1 = rand() / 32767.0f;
                    spd = (r1 * (k = 6.0f - 5.0f)) + 5.0f;
                }
                {
                    f32 k;
                    f32 r1 = rand() / 32767.0f;
                    s = (r1 * (k = 0.3f - 0.2f)) + 0.2f;
                }
                if (!sndFlag) {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (*pose->mtx)[0][3];
                    pos.y = (*pose->mtx)[1][3];
                    pos.z = (*pose->mtx)[2][3];
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG6_MOAMOA_DAMAGED1), &pos);
                    sndFlag = 1;
                }
                dist = -100.0f;
                moamoa_jump(pose, moa, MOAMOA_STATE_RESPAWN, 0, x, dist, y, spd, s);
                break;

            case MOAMOA_STATE_BLOWN_DRIFT:
                if ((--moa->jumpTimer) > 0) {
                    moa->target.x += moa->vy;
                    moa->target.y += moa->speed;
                    moa->target.z += moa->gravity;
                    (*pose->mtx)[0][3] += moa->target.x;
                    (*pose->mtx)[1][3] += moa->target.y;
                    (*pose->mtx)[2][3] += moa->target.z;
                } else {
                    moa->state = MOAMOA_STATE_RESPAWN;
                }
                break;

            case MOAMOA_STATE_RESPAWN:
                if ((wp->flags & MOAMOA_WORK_FLAG_TARGET0_CLEARED) && (wp->flags & MOAMOA_WORK_FLAG_TARGET1_CLEARED)) {
                    moa->state = MOAMOA_STATE_GONE;
                    break;
                }
                if (wp->hitCount < 250) {
                    PSMTXIdentity(*pose->mtx);
                    (*pose->mtx)[0][3] = wp->target[i % 3].x;
                    (*pose->mtx)[1][3] = wp->target[i % 3].y;
                    (*pose->mtx)[2][3] = wp->target[i % 3].z;
                    moa->flags = 0;
                    moa->state = MOAMOA_STATE_WANDER;
                    moa->timer = 0;
                } else {
                    moa->state = MOAMOA_STATE_GONE;
                }
                break;

            case MOAMOA_STATE_ESCAPE_TURN:
                if (moa->timer < 30) {
                    PSMTXRotRad(rot, 'y', 0.20943952f);
                    PSMTXConcat(*pose->mtx, rot, *pose->mtx);
                    pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                    moa->timer++;
                } else {
                    pose->flags |= EXT_POSE_FLAG_BILLBOARD;
                    moa->timer = (100.0f - distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[i % 3].x, wp->target[i % 3].z)) / 2.0f;
                    moa->state++;
                }
                break;

            case MOAMOA_STATE_ESCAPE_WAIT:
                if ((--moa->timer) < 0) {
                    moa->state++;
                }
                break;

            case MOAMOA_STATE_ESCAPE_RUN: {
                    f32 rad;
                    if ((__fabs((*pose->mtx)[0][3]) < 300.0) && (__fabs((*pose->mtx)[2][3]) < 300.0)) {
                        s = rand() / 32767.0f;
                        {
                            f32 k;
                            f32 a3 = angleABf(wp->target[i % 3].x, wp->target[i % 3].z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
                            a3 += ((s * (k = 45.0f - -45.0f)) + -45.0f);
                            rad = (6.2831855f * a3) / 360.0f;
                        }
                        x = (15.0f * polySinf(rad)) + (*pose->mtx)[0][3];
                        grav = (*pose->mtx)[2][3] - (15.0f * polyCosf(rad));
                        {
                            f32 k;
                            f32 r1 = rand() / 32767.0f;
                            f32 r2 = rand() / 32767.0f;
                            moamoa_jump(pose, moa, moa->state, 1, x, 0.0f, grav, 3.0f * ((r1 * (k = 1.0f - 1.0f)) + 1.0f), (r2 * k) + 1.0f);
                        }
                    } else {
                        moa->state = MOAMOA_STATE_GONE;
                    }
                }
                break;

            case MOAMOA_STATE_GONE:
                moa->flags |= MOAMOA_FLAG_GONE;
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
                pose->flags |= EXT_POSE_FLAG_NO_SHADOW;
                break;

            case MOAMOA_STATE_BLOW_START: {
                    f32 k;
                    s = rand() / 32767.0f;
                    spd = randf(2.0f, 3.0f);
                    moa->timer = irand(i / 2);
                    rad = (6.2831855f * ((s * (k = 315.0f - 225.0f)) + 225.0f)) / 360.0f;
                }
                moa->target.x = spd * polySinf(rad);
                moa->target.y = randf(1.0f, 2.0f);
                moa->target.z = spd * (-polyCosf(rad));
                moa->state++;
                break;

            case MOAMOA_STATE_BLOW_WAIT:
                if ((--moa->timer) < 0) {
                    moa->state++;
                }
                break;

            case MOAMOA_STATE_BLOW_AWAY:
                moa->target.x *= 1.1f;
                moa->target.y *= 1.02f;
                moa->target.z *= 1.1f;
                (*pose->mtx)[0][3] += moa->target.x;
                (*pose->mtx)[1][3] += moa->target.y;
                (*pose->mtx)[2][3] += moa->target.z;
                PSMTXRotRad(rot2, 'y', 0.20943952f);
                PSMTXConcat(*pose->mtx, rot2, *pose->mtx);
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                if ((*pose->mtx)[0][3] < (-1000.0f)) {
                    moa->state = MOAMOA_STATE_GONE;
                }
                break;

        }

        if (!(moa->flags & (MOAMOA_FLAG_KNOCKED_AWAY | MOAMOA_FLAG_ESCAPING | MOAMOA_FLAG_BLOWN))) {
            if ((*pose->mtx)[0][3] > 280.0f) {
                (*pose->mtx)[0][3] = 280.0f;
            }
            if ((*pose->mtx)[0][3] < (-120.0f)) {
                (*pose->mtx)[0][3] = -120.0f;
            }
            if ((*pose->mtx)[2][3] > 70.0f) {
                (*pose->mtx)[2][3] = 70.0f;
            }
            if ((*pose->mtx)[2][3] < (-70.0f)) {
                (*pose->mtx)[2][3] = -70.0f;
            }
        }
        moa->lastX = (*pose->mtx)[0][3];
        moa->lastZ = (*pose->mtx)[2][3];
    }

    moamoa_pose_main();
    if (wp->flags & MOAMOA_WORK_FLAG_PARTY_IN_USE) {
        mario->floorMove.x = 0.0f;
        mario->floorMove.y = 0.0f;
        mario->floorMove.z = 0.0f;
    } else {
        pv = (Vec){mario->floorMove.x, mario->floorMove.y, mario->floorMove.z};
        if (PSVECMag(&pv) > 2.0f) {
            PSVECNormalize(&pv, &pv);
            mario->floorMove.x = 2.0f * pv.x;
            mario->floorMove.y = 2.0f * pv.y;
            mario->floorMove.z = 2.0f * pv.z;
        }
    }
}
#pragma opt_lifetimes reset
static void ext_moamoa_dispent(void) {
    if (wp->flags & MOAMOA_WORK_FLAG_ACTIVE) {
        dispEntry(CAMERA_3D, (0.0f), 1, ext_disp, NULL);
        dispEntry(CAMERA_3D, (0.0f), 2, ext_shadow_disp, NULL);
    }
    if (wp->flags & MOAMOA_WORK_FLAG_DRAW_OFFSCREEN) {
        dispEntry(CAMERA_OFFSCREEN2, -5000.0f, 1, ext_disp, NULL);
        dispEntry(CAMERA_OFFSCREEN2, -5000.0f, 2, ext_shadow_disp, NULL);
    }
}

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("e_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("e_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("s_door_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("s_door_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("s_door_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("s_door_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "e_bero", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_01", // nextMapName
        "w_dero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "hom_00", // nextMapName
        "n_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_bridge)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    SET_VALUE(GSWF(3576), 1)
    CALL(evt_bero_mapchange, STRING("hom_00"), STRING("n_bero_2"))
    RETURN
EVT_END()

EVT_BEGIN(evt_bridge2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_eki_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_07"), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_00"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_00"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_ori"), 2, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kaidann"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ori"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kaidan"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_ori"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kabe"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_ere_1"), 2, 0)
    CALL(evt_mario_set_pos, -210, 60, 0)
    CALL(evt_party_set_pos, 0, -230, 60, 0)
    CALL(evt_item_delete_check, STRING("item"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(1), 2000)
        CALL(evt_item_set_position, STRING("item"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_mapobj_rotate, 1, STRING("s_lever"), 0, 0, -80)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_42_DRAWBRIDGE_DOWN)
    SET_VALUE(GSWF(3576), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_origami)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mapobj_get_position, STRING("s_lever"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_SWITCH_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -800, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_rotate, 1, STRING("s_lever"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, 340, 80, 280, -130, 90, -15, 1000, 11)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 1)
    SUBTRACT(LW(2), 1)
    CALL(evt_mario_mov_pos, LW(0), LW(2), 30)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ORIGAMI_APPEAR1"), -220, 60, 0, 0)
    CALL(evt_map_playanim, STRING("anm_ori"), 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kaidann"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ori"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kaidan"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_ori"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("kaidann_A"), 1, 1)
    CALL(evt_map_checkanim, STRING("anm_ori"), LW(0), LW(1))
    WAIT_MS(1600)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kabe"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1000)
    CALL(evt_mobj_switch_blue, 0, STRING("bridge_sw"), -220, 60, 0, PTR(evt_bridge), GSWF(3725))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_APPEAR1"), -220, 60, 0, 0)
    CALL(evt_mobj_set_scale, STRING("bridge_sw"), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
    CALL(evt_sub_intpl_msec_init, EASING_WAVE_SHRINKING_FASTER, 100, 1000, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mobj_set_scale, STRING("bridge_sw"), LW(0), LW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG6_41_ORIGAMI_STAIRS)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(evt_moamoa_init)
    CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a"))
    CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b"))
    CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
    CALL(evt_ext_entry, 150, PTR(ext_moamoa_data), PTR(ext_moamoa_init), PTR(ext_moamoa_main), PTR(ext_moamoa_dispent))
    RETURN
EVT_END()

static EVT_BEGIN(evt_meet_moamoa)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_eki_02_6"), 0, STRING("party"))
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 32, 151, 528, 32, 56, -12, 500, 11)
    WAIT_MS(2000)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
    END_INLINE_EVENT
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03_6"), 0, STRING("party"))
    END_SWITCH
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_39_SMORG_1ST_ENCOUNTER)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_moamoa_escape)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 0, 151, 528, 0, 56, -12, 500, 11)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 180)
    BEGIN_INLINE_EVENT
        CALL(evt_moamoa_end_wait)
        CALL(evt_moamoa_exit)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_lever"), 0, PTR(evt_origami), GW(10))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 img_set_color(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 r = evtGetValue(event, args[1]);
    u8 g = evtGetValue(event, args[2]);
    u8 b = evtGetValue(event, args[3]);
    u8 a = evtGetValue(event, args[4]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    animPoseSetMaterialFlagOn(img->poseId, 0x40);
    animPoseSetMaterialEvtColor(img->poseId, (GXColor){r, g, b, a});
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_elv_rumble)
    IF_INT_EQUAL(GF(0), 0)
        CALL(evt_map_checkanim, STRING("anm_ere_1"), LW(0), LW(1))
        ELSE
        CALL(evt_map_checkanim, STRING("anm_ere_2"), LW(0), LW(1))
    END_IF
    SUBTRACT(LW(1), 500)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_SHAKE1"), LW(3), LW(4), LW(5), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0048828125f), 250)
    WAIT_MS(LW(1))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_SHAKE1"), LW(3), LW(4), LW(5), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0048828125f), 250)
    RETURN
EVT_END()

static EVT_BEGIN(evt_elv_init)
    CALL(evt_paper_entry, STRING("OFF_hei_07"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_hei_07"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 0, 0, 0, 608, 480)
    CALL(evt_img_alloc_capture, STRING("capture_img"), 2, STRING("offscreen"), 0, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_elv_down)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_1"), 0, 0)
    WAIT_FRAMES(20)
    CALL(evt_mario_mov_pos2, 190, -65, FLOAT(60.0f))
    CALL(evt_map_checkanim, STRING("anm_ere_door_1"), LW(0), LW(1))
    ADD(LW(1), 1)
    WAIT_MS(LW(1))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 190, -65, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
        CALL(evt_party_jump_pos, 0, 160, 0, -160, 500, 0, -1000)
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 190, -160, FLOAT(60.0f))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_eki_00"), STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_eki_07"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_00"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_2"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_ere_door_2"), LW(0), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_MOVE1"), LW(0), LW(1), LW(2), LW(10))
        CALL(evt_map_checkanim, STRING("anm_ere_1"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_1"), 0, 0)
    SPAWN_EVENT(PTR(evt_elv_rumble))
    CALL(evt_map_checkanim, STRING("anm_ere_1"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_BELL1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(400)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_eki_00"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_eki_07"), STRING("offscreen"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL(evt_moamoa_flag_onoff, 0, MOAMOA_WORK_FLAG_ACTIVE)
        CALL(evt_moamoa_flag_onoff, 1, MOAMOA_WORK_FLAG_DRAW_OFFSCREEN)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_1"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_ere_door_1"), LW(0), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL(evt_moamoa_flag_onoff, 1, MOAMOA_WORK_FLAG_ACTIVE)
        CALL(evt_moamoa_flag_onoff, 0, MOAMOA_WORK_FLAG_DRAW_OFFSCREEN)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_eki_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_07"), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_00"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_00"), 1, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_40_SWATTING_SMORGS)
        CASE_EQUAL(STG6_40_SWATTING_SMORGS)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_lever"), 0, PTR(evt_origami), GW(10))
        CASE_DEFAULT
        CALL(evt_mapobj_rotate, 1, STRING("s_lever"), 0, 0, -80)
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_41_ORIGAMI_STAIRS)
        CALL(evt_mobj_switch_blue, 0, STRING("bridge_sw"), -220, 60, 0, PTR(evt_bridge), GSWF(3725))
    END_IF
    CALL(evt_item_delete_check, STRING("item"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(1), 2000)
        CALL(evt_item_set_position, STRING("item"), LW(0), LW(1), LW(2))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_41_ORIGAMI_STAIRS)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kaidann"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ori"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_kaidan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("n_ori"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("kaidann_A"), 1, 0)
        ELSE
        CALL(evt_map_playanim, STRING("anm_ori"), 2, 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kaidann"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ori"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kaidan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_ori"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_kabe"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 190, -65, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 150, -65, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_mario_mov_pos2, 190, -65, FLOAT(60.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_2"), 0, 0)
    WAIT_MS(500)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_39_SMORG_1ST_ENCOUNTER)
        CALL_CHILD(PTR(evt_meet_moamoa))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_39_SMORG_1ST_ENCOUNTER)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
    END_IF
    CALL(evt_map_checkanim, STRING("anm_ere_door_2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    RETURN
EVT_END()

EVT_BEGIN(evt_elv_up)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_1"), 0, 0)
    WAIT_FRAMES(20)
    CALL(evt_mario_mov_pos2, 190, -65, FLOAT(60.0f))
    CALL(evt_map_checkanim, STRING("anm_ere_door_1"), LW(0), LW(1))
    ADD(LW(1), 1)
    WAIT_MS(LW(1))
    BEGIN_INLINE_EVENT
        CALL(evt_hitobj_onoff, STRING("ere__A"), 0, 0)
        CALL(evt_party_move_pos, 0, 190, -65, 500)
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos, 0, 160, -160, 500)
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 0)
        CALL(evt_hitobj_onoff, STRING("ere__A"), 0, 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, 190, -160, 500)
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_eki_00"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_eki_07"), STRING("offscreen"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL(evt_moamoa_flag_onoff, 0, MOAMOA_WORK_FLAG_ACTIVE)
        CALL(evt_moamoa_flag_onoff, 1, MOAMOA_WORK_FLAG_DRAW_OFFSCREEN)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_2"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_ere_door_2"), LW(0), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL(evt_moamoa_flag_onoff, 0, MOAMOA_WORK_FLAG_DRAW_OFFSCREEN)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_MOVE1"), LW(0), LW(1), LW(2), LW(10))
        CALL(evt_map_checkanim, STRING("anm_ere_2"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_2"), 0, 0)
    SPAWN_EVENT(PTR(evt_elv_rumble))
    CALL(evt_map_checkanim, STRING("anm_ere_2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_BELL1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(400)
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_eki_00"), STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_eki_07"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_1"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_ere_door_1"), LW(0), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_41_ORIGAMI_STAIRS)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_41_ORIGAMI_STAIRS)
        CALL(evt_mobj_delete, STRING("bridge_sw"))
    END_IF
    CALL(evt_item_delete_check, STRING("item"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item"), LW(0), LW(1), LW(2))
        ADD(LW(1), 2000)
        CALL(evt_item_set_position, STRING("item"), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_07"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_eki_00"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_00"), 1, 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL(evt_del_case_evt, 0, GW(10))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_party_jump_pos, 0, 190, 0, -65, 500, 0, -1000)
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos, 0, 150, -65, 1000)
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    WAIT_FRAMES(30)
    CALL(evt_mario_mov_pos, 190, -65, 500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("d_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_ELEVATOR_CLOSE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_ere_door_2"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_ere_door_2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    RETURN
EVT_END()

static s32 item_tbl[] = {
    ITEM_ELEVATOR_KEY_0x1A, -1,
};

EVT_BEGIN(evt_elv)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_38_RIDE_THE_ELEVATOR)
        CALL(evt_win_item_select, 0, PTR(item_tbl), LW(1), 0)
        IF_INT_EQUAL(LW(1), -1)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        CALL(evt_pouch_remove_item, ITEM_ELEVATOR_KEY_0x1A, LW(0))
        SET_VALUE(GSW_STORY_PROGRESS, STG6_38_RIDE_THE_ELEVATOR)
        CALL(evt_snd_sfxon, STRING("SFX_STG6_ELEVATOR_KEY1"), 0)
        CALL(evt_msg_print, 0, STRING("stg6_eki_07"), 0, 0)
    END_IF
    IF_INT_EQUAL(GF(0), 0)
        CALL_CHILD(PTR(evt_elv_down))
        SET_VALUE(GF(0), 1)
        ELSE
        CALL_CHILD(PTR(evt_elv_up))
        SET_VALUE(GF(0), 0)
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_elv_nomsg)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_eki_06"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_head)
    CALL(evt_item_delete, STRING("item"))
    CALL(evt_item_entry, STRING("item"), ITEM_CLOSE_CALL_P, -260, 160, -60, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(3745), 0)
    CALL(evt_item_set_move_dir_speed, STRING("item"), FLOAT(135.0f), FLOAT(90.0f))
    CALL(evt_item_set_gravity, STRING("item"), FLOAT(1.0f))
    CALL(evt_item_set_bound_rate, STRING("item"), FLOAT(0.5996094f))
    CALL(evt_item_set_jump_power, STRING("item"), FLOAT(500.0f))
    CALL(evt_item_status_on, STRING("item"), 16)
    CALL(evt_item_flag_on, STRING("item"), 2048)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_eki_00_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(eki_00_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    IF_INT_EQUAL(GSWF(3576), 0)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        CALL_CHILD(PTR(bero_case_entry))
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_eki_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_eki_07"), 1, 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_37_ELEVATOR_KEY)
        CALL_CHILD(PTR(evt_elv_init))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_suitti"), 0, PTR(evt_elv), 0)
        ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_suitti"), 0, PTR(evt_elv_nomsg), 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_40_SWATTING_SMORGS)
        CALL_CHILD(PTR(evt_moamoa_init))
    END_IF
    CALL(evt_item_entry, STRING("item"), ITEM_CLOSE_CALL_P, -260, 130, -60, ITEM_TYPE_STILL_NO_TOUCH, GSWF(3745), 0)
    IF_INT_EQUAL(GSWF(3745), 0)
        CALL(evt_run_case_evt, CASE_HEAD, 1, STRING("a_dai_sita"), 0, PTR(evt_head), 0)
    END_IF
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI2"))
    IF_INT_EQUAL(GSWF(3576), 1)
        SPAWN_EVENT(PTR(evt_bridge2))
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kan"), 0, PTR(evt_kanban), 0)
    RETURN
EVT_END()

