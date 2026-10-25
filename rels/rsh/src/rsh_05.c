#include "version.h"
#include "sfx_name_word.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "party/party_nokonoko.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "poly_trig.h"
#pragma inline_max_total_size(11500)
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "string.h"
#include "memory.h"
#include "dolphin/os.h"
#include "dolphin/mtx.h"
#include "mario/mariost.h"
#include "driver/mapdrv.h"
#include "manager/evtmgr.h"
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "driver/hitdrv.h"
#include "dolphin/gx.h"
#include "driver/camdrv.h"
#include "driver/offscreendrv.h"
#include "system.h"
#define _MATH_H_
#define fabs(x) __fabs(x)
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "driver/dispdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/extdrv.h"
// target .bss/.sbss pad (uninitialised objects are emitted in reverse order, so this sits last)

typedef struct MoamoaPoseDef { char model[16]; char anim[8]; s16 loop; f32 speed; s32 start; s32 count; } MoamoaPoseDef;
typedef struct Moamoa { u16 flags; s16 state; s16 timer; u8 field_0x6[2]; Vec target; f32 vy; s16 jumpTimer; u8 field_0x1a[2]; f32 speed; f32 gravity; s16 next; u8 field_0x26[2]; f32 lastX; f32 lastZ; u8 field_0x30[0x3c - 0x30]; s16 pose; u8 field_0x3e[2]; OSTime startTime; u8 field_0x48[4]; u8 kind; u8 field_0x4d[3]; } Moamoa;
typedef struct MoamoaWork { u16 flags; u8 field_0x2[2]; s32 count; Moamoa* moa; MapObject* mobj; f32 mobjY; Vec target[3]; s16 hitCount; s16 field_0x3a[2]; s16 field_0x3e; s16 field_0x40; s16 idleFrames; EventEntry* escapeEvt; u8 field_0x48[0x4c - 0x48]; s32 numDefs; MoamoaPoseDef defs[32]; f32 angle; f32 dist; s32 bounceCount; f32 breathDir; f32 breathDist; Vec breathPos; u8 field_0x570[0x870 - 0x570]; } MoamoaWork;
typedef struct MoamoaData { const char* model; const char* anim; f32 speed; } MoamoaData;
static MoamoaWork* wp;
static f32 randf(f32 min, f32 max);
static void moamoa_pose_init(void);
static void moamoa_pose_main(void);
static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim);
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



static void ext_moamoa_main(void);
static s32 moamoa_hit_check(ExtdrvPose* pose, Moamoa* moa);
static void moamoa_jump(ExtdrvPose* pose, Moamoa* moa, s32 next, s32 checkGround, f32 x, f32 y, f32 z, f32 speed, f32 gravity);
static void ext_disp(CameraId camera, void* param);
static void ext_shadow_disp(CameraId camera, void* param);

s32 evt_cam3d_evt_set_at(EventEntry* event, BOOL isFirstCall);
s32 evt_cam3d_evt_set_xyz(EventEntry* event, BOOL isFirstCall);
s32 evt_cam3d_evt_xyz_off(EventEntry* event, BOOL isFirstCall);
s32 evt_item_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_item_get_item(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_flag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_pos(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_end_wait(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_exit(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_flag_onoff(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_ry_reset(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_target_off(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_tribe(EventEntry* event, BOOL isFirstCall);
static s32 npc_jump(EventEntry* event, BOOL isFirstCall);
static void ext_moamoa_dispent(void);
static void ext_moamoa_init(void);
static s32 evt_moamoa_fusion_debug(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_blow(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_kind_onoff(EventEntry* event, BOOL isFirstCall);
static s32 evt_moamoa_fusion(EventEntry* event, BOOL isFirstCall);

static ExtTexInfo ext_moamoa_data[47];
static EvtWord evt_moamoa_init[57];
static EvtWord evt_meet_moamoa[185];
EvtWord evt_moamoa_help[166];
EvtWord evt_moamoa_escape0[15];
EvtWord evt_moamoa_escape1[15];
static EvtWord evt_moamoa_escape[228];
static EvtWord init_syashou[30];
static EvtWord talk_syashou[19];
static EvtWord init_waitless[30];
static EvtWord talk_waitless[19];
static BeroEntry bero_data_a[3];
static BeroEntry bero_data_b[3];
static BeroEntry bero_data_c[3];
static NpcSetupInfo npc_data[3];
EvtWord evt_ghost_diary[5];
EvtWord evt_ghost_diary_read[53];
/* shelf access table: hit, msg; a NULL hit ends it */
typedef struct ShelfMsg { const char* hit; const char* msg; } ShelfMsg;
typedef struct ShelfMsgTbl6 { ShelfMsg rows[6]; const char* end[1]; } ShelfMsgTbl6;
static ShelfMsgTbl6 access_tbl;
static EvtWord evt_access[14];
static EvtWord evt_access2[47];
static EvtWord evt_access_entry[31];
EvtWord rsh_05_init_evt[96];
EvtWord rsh_05_a_init_evt[103];
EvtWord rsh_05_init_evt_nidome[59];
EvtWord rsh_05_a_init_evt_nidome[29];

static ExtTexInfo ext_moamoa_data[] = {
    {("c_n_moa_a"), ("S_1"), 0.0f},
    {("c_n_moa_a"), ("S_1"), 2.0f},
    {("c_n_moa_a"), ("S_1"), 4.0f},
    {("c_n_moa_a"), ("S_1"), 6.0f},
    {("c_n_moa_a"), ("S_1"), 8.0f},
    {("c_n_moa_a"), ("S_1"), 10.0f},
    {("c_n_moa_a"), ("S_1"), 12.0f},
    {("c_n_moa_a"), ("S_1"), 14.0f},
    {("c_n_moa_a"), ("S_1"), 16.0f},
    {("c_n_moa_a"), ("S_1"), 18.0f},
    {("c_n_moa_a"), ("S_1"), 20.0f},
    {("c_n_moa_a"), ("S_1"), 22.0f},
    {("c_n_moa_a"), ("S_1"), 24.0f},
    {("c_n_moa_a"), ("S_1"), 26.0f},
    {("c_n_moa_a"), ("S_1"), 28.0f},
    {("c_n_moa_a"), ("S_1"), 30.0f},
    {("c_n_moa_a"), ("S_1"), 32.0f},
    {("c_n_moa_a"), ("S_1"), 34.0f},
    {("c_n_moa_a"), ("S_1"), 36.0f},
    {("c_n_moa_a"), ("S_1"), 38.0f},
    {("c_n_moa_a"), ("D_1"), 0.0f},
    {("c_n_moa_a"), ("D_1"), 4.0f},
    {("c_n_moa_a"), ("D_1"), 8.0f},
    {("c_n_moa_b"), ("S_1"), 0.0f},
    {("c_n_moa_b"), ("S_1"), 2.0f},
    {("c_n_moa_b"), ("S_1"), 4.0f},
    {("c_n_moa_b"), ("S_1"), 6.0f},
    {("c_n_moa_b"), ("S_1"), 8.0f},
    {("c_n_moa_b"), ("S_1"), 10.0f},
    {("c_n_moa_b"), ("S_1"), 12.0f},
    {("c_n_moa_b"), ("S_1"), 14.0f},
    {("c_n_moa_b"), ("S_1"), 16.0f},
    {("c_n_moa_b"), ("S_1"), 18.0f},
    {("c_n_moa_b"), ("S_1"), 20.0f},
    {("c_n_moa_b"), ("S_1"), 22.0f},
    {("c_n_moa_b"), ("S_1"), 24.0f},
    {("c_n_moa_b"), ("S_1"), 26.0f},
    {("c_n_moa_b"), ("S_1"), 28.0f},
    {("c_n_moa_b"), ("S_1"), 30.0f},
    {("c_n_moa_b"), ("S_1"), 32.0f},
    {("c_n_moa_b"), ("S_1"), 34.0f},
    {("c_n_moa_b"), ("S_1"), 36.0f},
    {("c_n_moa_b"), ("S_1"), 38.0f},
    {("c_n_moa_b"), ("D_1"), 0.0f},
    {("c_n_moa_b"), ("D_1"), 4.0f},
    {("c_n_moa_b"), ("D_1"), 8.0f},
    {0},
};

#pragma dont_inline on
static void moamoa_pose_init(void) {
    MoamoaData* data = (MoamoaData*)ext_moamoa_data;
    s32 i = 0;
    s32 start;
    char model[32];
    char anim[32];

    strcpy(model, "");
    strcpy(anim, "");
    wp->numDefs = -1;
    do {
        if (strcmp(data->model, model)) {
            strcpy(model, data->model);
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
        wp->defs[wp->numDefs].speed = data->speed;
        wp->defs[wp->numDefs].loop = 1;
        if (!strncmp(anim, "J", 1)) {
            wp->defs[wp->numDefs].loop = 0;
        }
        i++;
    } while ((++data)->model != NULL);
    wp->numDefs++;
}

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
            moa->startTime = now;
            t = 0.0f;
        }
        pose->texId = def->start + (s32)((def->count - 1) * t);
    }
}
#pragma dont_inline reset

static const char* group_tbl[][2] = {
    {("c_n_moa_a"), ("S_1")},
    {("c_n_moa_b"), ("S_1")},
    {("c_kinodebu"), ("G_1")},
    {("c_puku_c"), ("U_1")},
    {("c_powan"), ("O_1")},
    {("c_smoko"), ("T_2")},
    {("c_rose"), ("G_1")},
    {("c_papab"), ("G_1")},
    {("c_mamab"), ("G_1")},
    {("c_konari"), ("G_1")},
    {("c_n_moa_a"), ("S_1")},
};
static inline void /**/ moamoa_pose_change_i(Moamoa* moa, const char* model, const char* anim) {
    MoamoaPoseDef* def = wp->defs;
    s32 i;

    if (moa->flags & 0x800) {
        model = group_tbl[moa->kind][0];
        if (moa->kind >= 2) {
            anim = group_tbl[moa->kind][1];
        }
        if (moa->kind == 10) {
            anim = "S_2";
        }
    }
    for (i = 0; i < wp->numDefs; i++, def++) {
        if (!strcmp(def->model, model) && !strcmp(def->anim, anim)) {
            break;
        }
    }
    if (i < wp->numDefs && moa->pose != i) {
        if (moa->flags & 4) {
            if (strncmp(anim, "D", 1)) {
                return;
            }
        }
        if (moa->flags & 0x10) {
            if (strncmp(anim, "R", 1)) {
                return;
            }
        }
        moa->pose = i;
        moa->startTime = gp->renderTime;
    }
}

#pragma dont_inline on
static void moamoa_pose_change(Moamoa* moa, const char* model, const char* anim) {
    MoamoaPoseDef* def = wp->defs;
    s32 i;

    if (moa->flags & 0x800) {
        model = group_tbl[moa->kind][0];
        if (moa->kind >= 2) {
            anim = group_tbl[moa->kind][1];
        }
        if (moa->kind == 10) {
            anim = "S_2";
        }
    }
    for (i = 0; i < wp->numDefs; i++, def++) {
        if (!strcmp(def->model, model) && !strcmp(def->anim, anim)) {
            break;
        }
    }
    if (i < wp->numDefs && moa->pose != i) {
        if (moa->flags & 4) {
            if (strncmp(anim, "D", 1)) {
                return;
            }
        }
        if (moa->flags & 0x10) {
            if (strncmp(anim, "R", 1)) {
                return;
            }
        }
        moa->pose = i;
        moa->startTime = gp->renderTime;
    }
}
#pragma dont_inline reset

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
        pose->ry = 0.0f;
        pose->targetRy = 0.0f;
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_end_wait(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    Moamoa* moa;

    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        if (!(moa->flags & 0x20)) {
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
            (*pose->mtx)[2][2] = 1.0f;
            (*pose->mtx)[1][1] = 1.0f;
            (*pose->mtx)[0][0] = 1.0f;
            moa->next = 14;
            moa->state = 14;
            moa->timer = i < 100 ? i % 100 : i % 60 + 100;
            moa->flags |= 0x100;
        }
    }
    extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, moa++) {
        if (!(moa->flags & 0x200)) {
            return EVT_RETURN_BLOCK;
        }
    }
    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        pose->flags |= 4;
        moa->flags |= 0x400;
        moa->state = 61;
        moamoa_pose_change_i(moa, "c_n_moa_a", "S_1");
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
                if (moa->kind == 1) {
                    moa->flags |= 0x800;
                }
            } else {
                moa->flags |= 0x800;
            }
        } else {
            if (!all) {
                if (moa->kind != 1) {
                    moa->flags &= ~0x800;
                }
            } else {
                moa->flags &= ~0x800;
            }
        }
        moamoa_pose_change_i(moa, "c_n_moa_a", "S_1");
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
        moa->flags &= ~0x400;
        moa->state = 63;
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_fusion_debug(EventEntry* event, BOOL isFirstCall) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();
    Moamoa* moa = wp->moa;

    for (i = 0; i < wp->count; i++, pose++, moa++) {
        pose->flags |= 4;
        moa->flags |= 0x400;
        moa->flags |= 0x100;
        moa->flags |= 0x200;
        moa->state = 61;
        moa->flags |= 0x800;
        moamoa_pose_change_i(moa, "c_n_moa_a", "S_1");
    }
    return EVT_RETURN_DONE;
}

static s32 evt_moamoa_target_off(EventEntry* event, BOOL isFirstCall) {
    s32 i = evtGetValue(event, event->args[0]);

    wp->target[i] = wp->target[1 - i];
    wp->field_0x3a[i] = 100;
    if (i == 0) {
        wp->flags |= 0x1000;
    } else {
        wp->flags |= 0x2000;
    }
    return EVT_RETURN_DONE;
}


static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

static void ext_moamoa_init(void) {
    s32 i;
    ExtdrvPose* pose;
    Moamoa* moa;
    f32 hitX, hitY, hitZ, hitDist, nx, ny, nz;

    wp = _mapAlloc(sizeof(MoamoaWork));
    memset(wp, 0, sizeof(MoamoaWork));
    wp->target[0] = npcNameToPtr("ウェイトレス")->position;
    wp->target[1] = npcNameToPtr("車掌")->position;
    wp->target[2] = (Vec){-290.0f, 65.0f, -95.0f};
    wp->count = extGetPoseNum();
    wp->moa = _mapAlloc(wp->count * sizeof(Moamoa));
    memset(wp->moa, 0, wp->count * sizeof(Moamoa));
    moamoa_pose_init();
    pose = extGetPosePtr();
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        moa->flags = 0;
        moa->state = 0;
        moa->timer = 0;
        moamoa_pose_change_i(moa, "c_n_moa_a", "S_1");
        pose->flags |= 3;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = randf(-290.0f, 240.0f);
        (*pose->mtx)[1][3] = 0.0f;
        (*pose->mtx)[2][3] = randf(-130.0f, 50.0f);
        hitX = hitY = hitZ = 0.0f;
        hitDist = 1000000.0f;
        if (hitCheckFilter(NULL, &hitX, &hitY, &hitZ, &hitDist, &nx, &ny, &nz, (*pose->mtx)[0][3], 100.0f + (*pose->mtx)[1][3], (*pose->mtx)[2][3], 0.0f, -1.0f, 0.0f)) {
            (*pose->mtx)[1][3] = hitY;
        }
    }
    wp->mobj = mapGetMapObj("ressha");
    wp->mobjY = wp->mobj->animMtx[1][3];
    for (i = 0; i < 600; i++) {
        ext_moamoa_main();
    }
}

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
            if (fabs(y - hitY) > 30.0) {
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
    moa->state = 3;
    moa->next = next;
    pose->dir = 180.0f + angle;
    moa->flags |= 2;
    pose->flags |= 0x10;
    pose->flags &= ~4;
}

#pragma dont_inline reset
#pragma opt_propagation off
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
    if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], mario->position.x, mario->position.z) < (20.0f + mario->wPlayerCollisionBox.x) / 2.0f
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
    if (mario->currMotionId == MARIO_MOTION_HAMMER && wp->field_0x40 > 10) {
        rad = 6.2831855f * reviseAngle(camGetPtr(CAMERA_3D)->viewYaw + (mario->displayDirection - 90.0f)) / 360.0f;
        dist = 35.0f * polySinf(rad) + mario->position.x;
        if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], dist, mario->position.z - 35.0f * polyCosf(rad)) < 35.0f) {
            return 2;
        }
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER_2 && (s32)mario->currSubMotionId == 0x14) {
        f32 (*mtx)[4] = *pose->mtx;
        a = (Vec){mtx[0][3], mtx[1][3], mtx[2][3]};
        if (PSVECDistance(&mario->position, &a) < 65.0f) {
            return 2;
        }
    }
    if (wp->breathDir >= 0.0f) {
        f32 (*mtx)[4] = *pose->mtx;
        b = (Vec){mtx[0][3], mtx[1][3], mtx[2][3]};
        if (PSVECDistance(&wp->breathPos, &b) < 30.0f) {
            return 3;
        }
    }
    if (nokonokoGetStatus(partyGetPtr(0)) == 2) {
        f32 (*mtx)[4] = *pose->mtx;
        c = (Vec){mtx[0][3], mtx[1][3], mtx[2][3]};
        if (PSVECDistance(&partyGetPtr(0)->position, &c) < 30.0f) {
            return 2;
        }
    }
    if (partyGetPtr(0)->currentMemberId == 3 && partyGetPtr(0)->useMotionId == 0x46) {
        f32 (*mtx)[4] = *pose->mtx;
        d = (Vec){mtx[0][3], mtx[1][3], mtx[2][3]};
        if (PSVECDistance(&partyGetPtr(0)->position, &d) < 50.0f) {
            return 4;
        }
    }
    e = (Vec){mario->position.x, (*pose->mtx)[1][3], mario->position.z};
    f = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
    range = (20.0f + mario->wPlayerCollisionBox.x) / 2.0f;
    if (PSVECDistance(&e, &f) < range) {
        (*pose->mtx)[0][3] = moa->lastX;
        (*pose->mtx)[2][3] = moa->lastZ;
        {
        f32 (*mtx)[4] = *pose->mtx;
        f = (Vec){mtx[0][3], mtx[1][3], mtx[2][3]};
        }
        dist = PSVECDistance(&e, &f);
        if (dist < range) {
            angle = angleABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
            if (__fabsf(compAngle(angle, mario->directionView)) < 40.0f) {
                mario->floorMove.x = -((range - dist) * polySinf(6.2831855f * mario->directionView / 360.0f));
                mario->floorMove.z = (range - dist) * polyCosf(6.2831855f * mario->directionView / 360.0f);
            } else {
                mario->floorMove.x = (*pose->mtx)[0][3] - range * polySinf(6.2831855f * angle / 360.0f) - mario->position.x;
                mario->floorMove.z = (*pose->mtx)[2][3] + range * polyCosf(6.2831855f * angle / 360.0f) - mario->position.z;
            }
        }
    }
    return 0;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset


#pragma opt_propagation off
#pragma opt_lifetimes off
__declspec(section ".rodata") static Vec moamoa_zero_vec = {0.0f, 0.0f, 0.0f};
static void ext_moamoa_main(void) {
    MarioWork* mario;
    PartyEntry* party;
    ExtdrvPose* pose;
    Moamoa* moa;
    s32 i;
    s32 hit;
    s32 sfx;
    s32 ret;
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
    f32 a;
    f32 d;
    f32 t;
    f32 r;
    f32 s;
    Mtx m1;
    Mtx m;
    Vec sp2_;
    Vec v;

    mario = marioGetPtr();
    party = partyGetPtr(0);
    camGetPtr(CAMERA_3D);
    hit = 0;
    if (mario->currMotionId == MARIO_MOTION_HAMMER) {
        wp->field_0x40++;
    } else {
        wp->field_0x40 = 0;
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
        wp->flags |= 0x10;
    } else {
        wp->flags &= ~0x10;
    }
    wp->breathDir = -1.0f;
    wp->breathDist = -1.0f;
    if (party->currentMemberId == 5 && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        wp->breathDir = cloudGetBreathDir();
        wp->breathDist = cloudGetBreathDist();
        wp->breathPos.x = wp->breathDist * polySinf(6.2831855f * wp->breathDir / 360.0f) + party->position.x;
        wp->breathPos.y = party->position.y;
        wp->breathPos.z = party->position.z - wp->breathDist * polyCosf(6.2831855f * wp->breathDir / 360.0f);
    }
    if (wp->field_0x3a[0] > 50) {
        wp->target[0] = wp->target[1];
        if (!(wp->flags & 0x1000)) {
            wp->flags |= 0x1000;
            evtEntry(evt_moamoa_escape0, 0, 0);
        }
    }
    if (wp->field_0x3a[1] > 50) {
        wp->target[1] = wp->target[0];
        if (!(wp->flags & 0x2000)) {
            wp->flags |= 0x2000;
            evtEntry(evt_moamoa_escape1, 0, 0);
        }
    }
    if ((wp->flags & 0x1000) && (wp->flags & 0x2000)) {
        s32 j;
        Moamoa* m2;

        extGetPosePtr();
        m2 = wp->moa;
        for (j = 0; j < wp->count; j++, m2++) {
            if (m2->flags & 4) {
                continue;
            }
            if (m2->flags & 0x10) {
                break;
            }
            m2->state = 0x19;
            m2->timer = 0;
            m2->flags |= 0x10;
        }
        if (wp->escapeEvt == NULL) {
            wp->escapeEvt = evtEntry(evt_moamoa_escape, 0, 0);
        }
    }

    pose = extGetPosePtr();
    sfx = 0;
    moa = wp->moa;
    for (i = 0; i < wp->count; i++, pose++, moa++) {
        wp->angle = angleABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
        wp->dist = distABf(mario->position.x, mario->position.z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
        if ((wp->flags & 1) && !(wp->flags & 4) && !marioKeyOffChk() && !marioCtrlOffChk() && !hit && !(moa->flags & 0x154)) {
            ret = moamoa_hit_check(pose, moa);
            if (ret == 1) {
                moa->flags |= 4;
                moa->state = 0x24;
                {
                s32 idx;
                wp->hitCount++;
                idx = i % 3;
                if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[idx].x, wp->target[idx].z) < VERSION_PICK(200.0f, 80.0f)) {
                    wp->field_0x3a[idx]++;
                }
                }
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                hit = 1;
            } else if (ret == 2 || ret == 4) {
                moa->flags |= 0xc;
                moa->state = 0x2f;
                {
                s32 idx;
                wp->hitCount++;
                idx = i % 3;
                if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[idx].x, wp->target[idx].z) < VERSION_PICK(200.0f, 80.0f)) {
                    wp->field_0x3a[idx]++;
                }
                }
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                if (ret != 4) {
                    hit = 1;
                }
            } else if (ret == 3) {
                angle = (wp->breathDir + wp->angle) / 2.0f;
                moa->flags |= 0x84;
                moa->state = 0x30;
                moa->target = moamoa_zero_vec;
                spd = randf(0.1f, 0.2f);
                rad = (6.2831855f * angle) / 360.0f;
                moa->vy = spd * polySinf(rad);
                moa->speed = randf(0.03f, 0.06f);
                moa->gravity = spd * (-polyCosf(rad));
                moa->jumpTimer = 120;
                {
                s32 idx;
                wp->hitCount++;
                idx = i % 3;
                if (distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->target[idx].x, wp->target[idx].z) < VERSION_PICK(200.0f, 80.0f)) {
                    wp->field_0x3a[idx]++;
                }
                }
                moamoa_pose_change(moa, "c_n_moa_a", "D_1");
                hit = 1;
            }
        }
        if ((wp->flags & 1) && !(moa->flags & 0x155) && wp->dist < 50.0f) {
            if (moa->flags & 2) {
                moa->next = 1;
            } else {
                moa->state = 1;
            }
        }
        if ((wp->flags & 1) && !(moa->flags & 0x444) && i == gp->retraceCount % wp->count && irand(1000) < 200) {
            {
                Vec tmp = {0.0f, 0.0f, 0.0f};
                tmp.x = (*pose->mtx)[0][3];
                tmp.y = (*pose->mtx)[1][3];
                tmp.z = (*pose->mtx)[2][3];
                sp2_ = tmp;
            }
            // FAKE: integer cast hoists the literal as an @ temp (same values)
            psndSFXOn_3D(SFX_NAME_WORD("SFX_STG6_MOAMOA_VOICE1"), &sp2_);
        }
    switch (moa->state)
    {
      case 0:
        if ((i % 133) < 100)
      {
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
        moa->flags |= 1;
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
        if (d < VERSION_PICK(200.0f, 80.0f))
        {
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
        moa->flags &= ~1;
        break;

      case 50:
        if ((--moa->timer) < 0)
      {
        moa->timer = 0;
        moa->state++;
      }
        break;

      case 51:
        moa->speed = randf(0.0f, 200.0f);
        moa->gravity = randf(0.0f, (300.0f * moa->speed) / 200.0f);
        moa->state++;
        moa->timer = 0;

      case 52:
        s = (moa->timer < 200) ? (1.0f) : ((800 - moa->timer) / 600.0f);
        if (s < 0.0f)
      {
        s = 0.0f;
        moa->flags |= 0x200;
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

      case 1:
      {
        f32 rad;
        {
          f32 k; f32 r1 = rand() / 32767.0f;
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

      case 2:
      {
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

      case 3:
        (*pose->mtx)[1][3] += moa->vy;
        moa->vy -= moa->gravity;
      {
        f32 rad = (6.2831855f * angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], moa->target.x, moa->target.z)) / 360.0f;
        (*pose->mtx)[0][3] += moa->speed * polySinf(rad);
        (*pose->mtx)[2][3] = (*pose->mtx)[2][3] + (moa->speed * (-polyCosf(rad)));
      }
        if ((--moa->jumpTimer) < 0)
      {
        (*pose->mtx)[0][3] = moa->target.x;
        (*pose->mtx)[1][3] = moa->target.y;
        (*pose->mtx)[2][3] = moa->target.z;
        moa->flags &= ~2;
        pose->flags &= ~0x10;
        moa->timer = 0;
        moa->state++;
      }
        break;

      case 4:
        if ((++moa->timer) > 4)
      {
        moa->state = moa->next;
      }
        break;

      case 36:

      case 37:

      case 38:
      {
        f32 tz;
        if (!(moa->flags & 1))
        {
          tx = mario->position.x;
          tz = mario->position.z;
        }
        else
        {
          tx = wp->target[i % 3].x;
          tz = wp->target[i % 3].z;
        }
      {
        f32 z;
        {
          f32 rad; f32 k;
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

      case 39:
        if ((++moa->timer) < 30)
      {
        (*pose->mtx)[0][0] *= 0.9f;
        (*pose->mtx)[1][1] *= 0.9f;
        (*pose->mtx)[2][2] *= 0.9f;
      }
      else
      {
        moa->state = 49;
      }
        break;

      case 47:
      {
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
        if (!sfx)
      {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = (*pose->mtx)[0][3];
        pos.y = (*pose->mtx)[1][3];
        pos.z = (*pose->mtx)[2][3];
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG6_MOAMOA_DAMAGED1), &pos);
        sfx = 1;
      }
        dist = -100.0f;
        moamoa_jump(pose, moa, 49, 0, x, dist, y, spd, s);
        break;

      case 48:
        if ((--moa->jumpTimer) > 0)
      {
        moa->target.x += moa->vy;
        moa->target.y += moa->speed;
        moa->target.z += moa->gravity;
        (*pose->mtx)[0][3] += moa->target.x;
        (*pose->mtx)[1][3] += moa->target.y;
        (*pose->mtx)[2][3] += moa->target.z;
      }
      else
      {
        moa->state = 49;
      }
        break;

      case 49:
        if ((wp->flags & 0x1000) && (wp->flags & 0x2000))
      {
        moa->state = 62;
        break;
      }
        if (wp->hitCount < 500)
      {
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = wp->target[i % 3].x;
        (*pose->mtx)[1][3] = wp->target[i % 3].y;
        (*pose->mtx)[2][3] = wp->target[i % 3].z;
        moa->flags = 0;
        moa->state = 0;
        moa->timer = 0;
      }
      else
      {
        moa->state = 62;
      }
        break;

      case 25:
        if (moa->timer < 30)
      {
        PSMTXRotRad(m1, 'y', 0.20943952f);
        PSMTXConcat(*pose->mtx, m1, *pose->mtx);
        pose->flags &= ~2;
        moa->timer++;
      }
      else
      {
        pose->flags |= 2;
        moa->timer = rand() % 30;
        moa->state++;
      }
        break;

      case 26:
        if ((--moa->timer) < 0)
      {
        moa->timer = 0;
        moa->state++;
      }
        break;

            case 27:
                d = distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], -275.0f, -100.0f);
                { f32 a3; f32 k; tz = rand() / 32767.0f;
                a3 = angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], -275.0f, -100.0f);
                a3 += ((tz * (k = 45.0f - -45.0f)) + -45.0f);
                if (d > 6.0f && (x = (*pose->mtx)[1][3]) < 60.0f) {
                    f32 rad = (6.2831855f * a3) / 360.0f;
                    dist = (15.0f * polySinf(rad)) + (*pose->mtx)[0][3];
                    angle = (*pose->mtx)[2][3] - (15.0f * polyCosf(rad));
                    {
                        f32 k;
                        f32 r1 = rand() / 32767.0f;
                        f32 r2 = rand() / 32767.0f;
                        moamoa_jump(pose, moa, moa->state, 1, dist, x, angle, 3.0f * ((r1 * (k = 1.0f - 1.0f)) + 1.0f), (r2 * k) + 1.0f);
                    }
                } else {
                    moa->state++;
                }
                }
                break;
            case 28:
                if ((*pose->mtx)[0][3] > -350.0f) {
                    f32 rad;
                    s = rand() / 32767.0f;
                    {
                        f32 k;
                        f32 a3 = angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], -350.0f, -100.0f);
                        a3 += ((s * (k = 45.0f - -45.0f)) + -45.0f);
                        rad = (6.2831855f * a3) / 360.0f;
                    }
                    x = (15.0f * polySinf(rad)) + (*pose->mtx)[0][3];
                    y = (*pose->mtx)[1][3];
                    grav = (*pose->mtx)[2][3] - (15.0f * polyCosf(rad));
                    {
                        f32 k;
                        f32 r1 = rand() / 32767.0f;
                        f32 r2 = rand() / 32767.0f;
                        moamoa_jump(pose, moa, moa->state, 1, x, y, grav, 3.0f * ((r1 * (k = 1.0f - 1.0f)) + 1.0f), (r2 * k) + 1.0f);
                    }
                } else {
                    moa->state = 0x3e;
                }
                break;
      case 62:
        moa->flags |= 0x20;
        pose->flags &= ~1;
        pose->flags |= 4;
        break;

      case 63:
      {
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

      case 64:
        if ((--moa->timer) < 0)
      {
        moa->state++;
      }
        break;

      case 65:
        moa->target.x *= 1.1f;
        moa->target.y *= 1.02f;
        moa->target.z *= 1.1f;
        (*pose->mtx)[0][3] += moa->target.x;
        (*pose->mtx)[1][3] += moa->target.y;
        (*pose->mtx)[2][3] += moa->target.z;
        PSMTXRotRad(m, 'y', 0.20943952f);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        pose->flags &= ~2;
        if ((*pose->mtx)[0][3] < (-1000.0f))
      {
        moa->state = 62;
      }
        break;

    }

        if (!(moa->flags & 0x98)) {
            if ((*pose->mtx)[0][3] > 290.0f) {
                (*pose->mtx)[0][3] = 290.0f;
            }
            if ((*pose->mtx)[0][3] < -290.0f) {
                (*pose->mtx)[0][3] = -290.0f;
            }
            if ((*pose->mtx)[2][3] > 50.0f) {
                (*pose->mtx)[2][3] = 50.0f;
            }
            if ((*pose->mtx)[2][3] < -130.0f) {
                (*pose->mtx)[2][3] = -130.0f;
            }
        }
        (*pose->mtx)[1][3] += wp->mobj->modelWorldMtx[1][3] - wp->mobjY;
        moa->lastX = (*pose->mtx)[0][3];
        moa->lastZ = (*pose->mtx)[2][3];
    }
    moamoa_pose_main();
    wp->mobjY = wp->mobj->modelWorldMtx[1][3];
    if (wp->flags & 0x10) {
        mario->floorMove.x = 0.0f;
        mario->floorMove.y = 0.0f;
        mario->floorMove.z = 0.0f;
    } else {
        v = (Vec){mario->floorMove.x, mario->floorMove.y, mario->floorMove.z};
        { static const Vec zv2 = {0.0f, 0.0f, 0.0f}; } // FAKE: unreferenced 12-byte .rodata pad object, defined after the compound literal's template so the template comes first (target order)
        if (PSVECMag(&v) > 2.0f) {
            PSVECNormalize(&v, &v);
            mario->floorMove.x = 2.0f * v.x;
            mario->floorMove.y = 2.0f * v.y;
            mario->floorMove.z = 2.0f * v.z;
        }
    }
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset


static void ext_moamoa_dispent(void) {
    if (wp->flags & 1) {
        dispEntry(CAMERA_3D, 0.0f, 1, ext_disp, NULL);
        dispEntry(CAMERA_3D, 0.0f, 2, ext_shadow_disp, NULL);
    }
    if (wp->flags & 2) {
        dispEntry(CAMERA_OFFSCREEN2, -5000.0f, 1, ext_disp, NULL);
        dispEntry(CAMERA_OFFSCREEN2, -5000.0f, 2, ext_shadow_disp, NULL);
    }
}

#pragma opt_propagation off
#pragma opt_lifetimes off
static s32 npc_jump(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 speed = evtGetFloat(event, args[4]);
    f32 gravity = evtGetFloat(event, args[5]);
    s32 frames = evtGetValue(event, args[6]);
    f32 t = 60.0f * frames / 1000.0f;
    NpcEntry* npc = npcNameToPtr(name);
    f32 dy;
    f32 vy;
    f32 dist;
    f32 angle;

    if (isFirstCall) {
        static const Vec va = {0.0f, 0.0f, 0.0f};
        static const Vec vb = {0.0f, 0.0f, 0.0f};
        Vec a;
        Vec b;

        b = vb;
        b.x = npc->position.x;
        b.z = npc->position.z;
        a = va;
        a.x = x;
        a.z = z;
        dist = PSVECDistance(&a, &b);
        dy = y - npc->position.y;
        if (t == 0.0f) {
            t = dist / speed;
        } else {
            speed = dist / t;
        }
        vy = dy / t + gravity * t / 2.0f;
        event->userdata[0] = *(s32*)&t;
        event->userdata[1] = *(s32*)&vy;
        event->userdata[2] = *(s32*)&speed;
    }
    t = event->userdataf[0];
    vy = event->userdataf[1];
    speed = event->userdataf[2];
    angle = 6.2831855f * angleABf(npc->position.x, npc->position.z, x, z) / 360.0f;
    npc->position.x = speed * polySinf(angle) + npc->position.x;
    npc->position.z = npc->position.z - speed * polyCosf(angle);
    npc->position.y += vy;
    vy -= gravity;
    t -= 1.0f;
    if (t < 0.0f) {
        npc->position = (Vec){x, y, z};
        return 1;
    }
    event->userdata[0] = *(s32*)&t;
    event->userdata[1] = *(s32*)&vy;
    return 0;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

static EVT_BEGIN(evt_moamoa_init)
    CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a"))
    CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b"))
    CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
    CALL(evt_ext_entry, 400, PTR(ext_moamoa_data), PTR(ext_moamoa_init), PTR(ext_moamoa_main), PTR(ext_moamoa_dispent))
    CALL(evt_moamoa_flag_onoff, 1, 1)
    IF_INT_EQUAL(GSWF(3450), 1)
        CALL(evt_moamoa_target_off, 0)
        CALL(evt_npc_change_interrupt, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 6, PTR(talk_waitless))
    END_IF
    IF_INT_EQUAL(GSWF(3451), 1)
        CALL(evt_moamoa_target_off, 1)
        CALL(evt_npc_change_interrupt, STRING("\x8e\xd4\x8f\xb6"), 6, PTR(talk_syashou))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_meet_moamoa)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1500)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_anim, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), STRING("G_1"))
    CALL(evt_npc_set_anim, STRING("\x8e\xd4\x8f\xb6"), STRING("G_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("eff1"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_237"), 0, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_zoom_in, 25, 1500, 0, 11)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set_at, -200, 60, -80, 0, 1000, 11)
    WAIT_MS(1000)
    WAIT_MS(2000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_moamoa_ry_reset)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_238_6"), 0, STRING("party"))
    END_SWITCH
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 330)
    RETURN
EVT_END()

EVT_BEGIN(evt_moamoa_help)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, LW(0), 512)
        IF_STR_EQUAL(LW(0), STRING("\x8e\xd4\x8f\xb6"))
            CALL(evt_msg_print, 0, STRING("stg6_rsh_249_00"), 0, LW(0))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_249_01"), 0, LW(0))
        END_IF
        CALL(evt_npc_flag_onoff, 1, LW(0), 512)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, LW(0), 32)
    CALL(evt_npc_flag_onoff, 0, LW(0), 1024)
    SET_VALUE(LW(10), LW(0))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_random, 1000, LW(4))
    IF_INT_LESS(LW(4), 500)
        SET_VALUE(LW(4), 90)
        ELSE
        SET_VALUE(LW(4), 270)
    END_IF
    CALL(evt_sub_get_sincos, LW(4), LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(30.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(30.0f))
    ADD_FLOAT(LW(1), LW(5))
    SUBTRACT_FLOAT(LW(3), LW(6))
    CALL(npc_jump, LW(0), LW(1), LW(2), LW(3), FLOAT(0.0f), FLOAT(0.5f), 400)
    CALL(evt_sub_get_sincos, LW(4), LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(20.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(20.0f))
    ADD_FLOAT(LW(1), LW(5))
    SUBTRACT_FLOAT(LW(3), LW(6))
    CALL(npc_jump, LW(0), LW(1), LW(2), LW(3), FLOAT(0.0f), FLOAT(0.2998047f), 200)
    CALL(evt_sub_get_sincos, LW(4), LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(10.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(10.0f))
    ADD_FLOAT(LW(1), LW(5))
    SUBTRACT_FLOAT(LW(3), LW(6))
    CALL(npc_jump, LW(0), LW(1), LW(2), LW(3), FLOAT(0.0f), FLOAT(0.19921875f), 100)
    CALL(evt_npc_flag_onoff, 1, LW(0), 1024)
    CALL(evt_npc_flag_onoff, 0, LW(0), 32)
    IF_STR_EQUAL(LW(10), STRING("\x8e\xd4\x8f\xb6"))
        CALL(evt_npc_set_anim, LW(10), STRING("O_1"))
        ELSE
        CALL(evt_npc_set_anim, LW(10), STRING("P_1"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_moamoa_escape0)
    SET_VALUE(LW(0), STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")))
    SPAWN_EVENT(PTR(evt_moamoa_help))
    SET_VALUE(GSWF(3450), 1)
    CALL(evt_npc_change_interrupt, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 6, PTR(talk_waitless))
    RETURN
EVT_END()

EVT_BEGIN(evt_moamoa_escape1)
    SET_VALUE(LW(0), STRING("\x8e\xd4\x8f\xb6"))
    SPAWN_EVENT(PTR(evt_moamoa_help))
    SET_VALUE(GSWF(3451), 1)
    CALL(evt_npc_change_interrupt, STRING("\x8e\xd4\x8f\xb6"), 6, PTR(talk_syashou))
    RETURN
EVT_END()

static EVT_BEGIN(evt_moamoa_escape)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set_xyz, -200, 0, 0, 0, 3000, 11)
    WAIT_MS(3000)
    CALL(evt_moamoa_end_wait)
    CALL(evt_moamoa_exit)
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("\x8e\xd4\x8f\xb6"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 32)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("\x8e\xd4\x8f\xb6"), 2)
        CALL(evt_npc_move_position, STRING("\x8e\xd4\x8f\xb6"), -80, 22, 1000, 0, 1)
        CALL(evt_npc_set_ry, STRING("\x8e\xd4\x8f\xb6"), 90)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 2)
        CALL(evt_npc_move_position, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 0, 15, 1000, 0, 1)
        CALL(evt_npc_set_ry, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -20, -10, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(2), -140)
        CALL(evt_mario_mov_pos2, LW(0), -100, FLOAT(80.0f))
    END_IF
    CALL(evt_mario_flag_onoff, 1, 1024)
    CALL(evt_mario_mov_pos2, -45, 5, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_mario_flag_onoff, 0, 1024)
    CALL(evt_cam3d_evt_set, -40, 90, 283, -40, 38, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_240_00"), 0, STRING("\x8e\xd4\x8f\xb6"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_241"), 0, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_240_01"), 0, STRING("\x8e\xd4\x8f\xb6"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\x8e\xd4\x8f\xb6"), 32)
    CALL(evt_npc_flag_onoff, 0, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 32)
    CALL(evt_npc_change_interrupt, STRING("\x8e\xd4\x8f\xb6"), 6, PTR(talk_syashou))
    CALL(evt_npc_change_interrupt, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 6, PTR(talk_waitless))
    SET_VALUE(GSW_STORY_PROGRESS, 331)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(init_syashou)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(329, 332)
        CALL(evt_npc_set_position, STRING("me"), -115, 0, 20)
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("O_1"), STRING("T_3"))
        CALL(evt_npc_set_tribe, STRING("me"), STRING("\x8e\xd4\x8f\xb6\x94\xdf"))
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_syashou)
    IF_INT_LESS(GSW_STORY_PROGRESS, 331)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_249_01_01"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_242"), 0, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(init_waitless)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(329, 332)
        CALL(evt_npc_set_position, STRING("me"), 50, 0, 0)
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("P_1"), STRING("T_5"))
        CALL(evt_npc_set_tribe, STRING("me"), STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X\x94\xdf")))
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_waitless)
    IF_INT_LESS(GSW_STORY_PROGRESS, 331)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_249_01_02"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_243"), 0, 0)
    END_IF
    RETURN
EVT_END()

static BeroEntry bero_data_a[] = {
    {
        ("e_bero"), // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_a", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        ("w_bero"), // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_06_a", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b[] = {
    {
        ("e_bero"), // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_b", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        ("w_bero"), // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_06_a", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c[] = {
    {
        ("e_bero"), // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_c", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        ("w_bero"), // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_06_a", // nextMapName
        ("w_bero"), // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static NpcSetupInfo npc_data[] = {
    {
        /*"車掌"*/"\x8e\xd4\x8f\xb6", // name
        0x600, // flags
        0, // reactionFlags
        init_syashou, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ウェイトレス"*/"\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X", // name
        0x600, // flags
        0, // reactionFlags
        init_waitless, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_ghost_diary)
    SET_VALUE(GSW_STORY_PROGRESS, 299)
    RETURN
EVT_END()

EVT_BEGIN(evt_ghost_diary_read)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_130"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_rsh_130_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_131"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_130_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_132"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_130_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_133"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_134"), 0, STRING("\x82\xe4\x82\xa4\x82\xea\x82\xa2"))
    RETURN
EVT_END()

static ShelfMsgTbl6 access_tbl = {
    {
        {"kihako_1", "stg6_rsh_275"},
        {"kibako_2", "stg6_rsh_275"},
        {"bin_1", "stg6_rsh_276"},
        {"wagon", "stg6_rsh_280"},
        {"hako_1", "stg6_rsh_281"},
        {"hako_2", "stg6_rsh_281"},
    },
    {NULL},
};

static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access2)
    IF_INT_EQUAL(LSWF(12), 0)
        SET_VALUE(LSWF(12), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_277"), 0, 0)
        CALL(evt_item_entry, STRING("a"), 142, 0, -1000, 0, 17, -1, 0)
        CALL(evt_item_get_item, STRING("a"))
        CALL(evt_mario_key_onoff, 1)
        ELSE
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_279"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
    END_IF
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
    CALL(evt_run_case_evt, 9, 1, STRING("bin_2"), 0, PTR(evt_access2), 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_05_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), 1, 0)
    END_SWITCH
    CALL_CHILD(PTR(evt_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(284, 286)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(304, 309)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(310, 325)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_BETWEEN(327, 328)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(329, 331)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(334, 344)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_05_a_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(288)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CASE_LESS(294)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CASE_LESS(303)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CASE_LESS(309)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CASE_LESS(326)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CASE_LESS(328)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CASE_DEFAULT
        SET_VALUE(LW(0), PTR(bero_data_a))
    END_SWITCH
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_05_init_evt))
    CALL(evt_npc_setup, PTR(npc_data))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(329)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CASE_EQUAL(329)
        CALL_CHILD(PTR(evt_moamoa_init))
        SPAWN_EVENT(PTR(evt_meet_moamoa))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CASE_EQUAL(330)
        CALL_CHILD(PTR(evt_moamoa_init))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CASE_DEFAULT
        CALL(evt_npc_change_interrupt, STRING("\x8e\xd4\x8f\xb6"), 6, PTR(talk_syashou))
        CALL(evt_npc_change_interrupt, STRING(("\x83" "E\x83" "F\x83" "C\x83g\x83\x8c\x83X")), 6, PTR(talk_waitless))
    END_SWITCH
    IF_INT_LESS(GSW_STORY_PROGRESS, 299)
        CALL(evt_item_entry, STRING("diary"), 78, -90, 0, -140, 0, -1, PTR(evt_ghost_diary))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_05_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3579), 0)
        IF_INT_EQUAL(GSWF(3580), 0)
            IF_INT_EQUAL(GSWF(3581), 0)
                CALL(evt_map_playanim, STRING("yure"), 1, 0)
            END_IF
        END_IF
    END_IF
    CALL(evt_npc_setup, PTR(npc_data))
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
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_05_a_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3453), 1)
        SET_VALUE(LW(0), PTR(bero_data_a))
    END_IF
    IF_INT_EQUAL(GSWF(3454), 1)
        SET_VALUE(LW(0), PTR(bero_data_b))
    END_IF
    IF_INT_EQUAL(GSWF(3455), 1)
        SET_VALUE(LW(0), PTR(bero_data_c))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_05_init_evt_nidome))
    RETURN
EVT_END()

