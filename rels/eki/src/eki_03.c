#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "driver/envdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "memory.h"
#include "mario/mariost.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"

static smartEntry* ind_lp;
void envGlareFilter(s32 on, s16 x0, s16 y0, s16 x1, s16 y1);

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
static USER_FUNC(test_glare);
static void sun_tev(MapTevCallbackInfo* info);

static EvtWord w1_close[12];
static EvtWord w1_open[12];
static EvtWord w2_close[12];
static EvtWord w2_open[12];
static BeroEntry bero_data[3];
EvtWord evt_poster[10];
static EvtWord evt_chk[74];
static EvtWord evt_poster_1[7];
static EvtWord evt_poster_2[7];
static EvtWord evt_poster_3[7];
static EvtWord evt_poster_4[7];
static EvtWord evt_poster_5[7];
static EvtWord evt_poster_6[7];
static EvtWord evt_poster_7[7];
static EvtWord evt_poster_8[7];
static EvtWord evt_poster_9[7];
EvtWord evt_poster_10[12];
EvtWord evt_poster_11[27];
EvtWord evt_poster_12[22];
EvtWord evt_poster_13[7];
EvtWord evt_poster_14[7];
EvtWord evt_poster_15[7];
#ifdef VERSION_G8ME01
EvtWord eki_03_init_evt[470];
#else
EvtWord eki_03_init_evt[471];
#endif

static EVT_BEGIN(w1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "w_bero_1", // hitName
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
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_close, // enterAnimeArg
        w1_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_04", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_close, // enterAnimeArg
        w2_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_poster)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_33_BLOW_THE_POSTER)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(evt_chk)
    IF_INT_EQUAL(GSWF(3733), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3734), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3735), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3736), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3737), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3738), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3739), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3740), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3741), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3742), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3743), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_hitobj_onoff, STRING("a_kabe"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_33_BLOW_THE_POSTER)
        SET_VALUE(GSW_STORY_PROGRESS, STG6_33_BLOW_THE_POSTER)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_1)
    SET_VALUE(GSWF(3729), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_2)
    SET_VALUE(GSWF(3730), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_3)
    SET_VALUE(GSWF(3731), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_4)
    SET_VALUE(GSWF(3732), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_5)
    SET_VALUE(GSWF(3733), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_6)
    SET_VALUE(GSWF(3734), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_7)
    SET_VALUE(GSWF(3735), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_8)
    SET_VALUE(GSWF(3736), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_9)
    SET_VALUE(GSWF(3737), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_10)
    SET_VALUE(GSWF(3738), 1)
    CALL(evt_hitobj_onoff, STRING("a_kami_13"), 1, 0)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_11)
    SET_VALUE(GSWF(3739), 1)
    CALL(evt_hitobj_onoff, STRING("a_kami_14"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_kami_15"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_kami_6"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_kami_7"), 1, 0)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_12)
    SET_VALUE(GSWF(3740), 1)
    CALL(evt_hitobj_onoff, STRING("a_kami_8"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_kami_9"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_kami_5"), 1, 0)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_13)
    SET_VALUE(GSWF(3741), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_14)
    SET_VALUE(GSWF(3742), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(evt_poster_15)
    SET_VALUE(GSWF(3743), 1)
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

EVT_BEGIN(eki_03_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_HOM1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI4"))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    CALL(evt_item_entry, STRING("item"), ITEM_HP_PLUS, -550, 220, -50, ITEM_TYPE_STILL, GSWF(3746), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk01"), 450, 100, 10, 0, GSWF_SHINE_36, 0)
#ifdef VERSION_G8ME01
    CALL(evt_mobj_blk, STRING("blk01"), 250, 300, -100, 0, 0, LSWF(1))
#else
    CALL(evt_mobj_brick, STRING("blk01"), 250, 300, -100, ITEM_NULL, MOBJ_BRICK_PLAIN, 0, LSWF(1))
#endif
    CALL(evt_mobj_brick, STRING("blk02"), 250, 385, -100, ITEM_THUNDER_RAGE, MOBJ_BRICK_HIDDEN_ITEM, 0, GSWF(3754))
    CALL(evt_map_set_tevcallback, 2, PTR(sun_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sun"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sun"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sun"), MAP_OBJ_FLAG_NO_COLOR_STAGE)
    BEGIN_INLINE_EVENT
        CALL(test_glare)
    END_INLINE_EVENT
    CALL(evt_cloud_init)
    IF_INT_EQUAL(GSWF(3729), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_0"), STRING("a_kami_1"), PTR(evt_poster_1))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_0"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_1"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3730), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_17"), STRING("a_kami_2"), PTR(evt_poster_2))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_17"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_2"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3731), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_16"), STRING("a_kami_3"), PTR(evt_poster_3))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_16"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_3"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3732), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_4"), STRING("a_kami_4"), PTR(evt_poster_4))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_4"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3733), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_5"), STRING("a_kami_12"), PTR(evt_poster_5))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_12"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3734), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_6"), STRING("a_kami_11"), PTR(evt_poster_6))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_6"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_11"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3735), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_7"), STRING("a_kami_11"), PTR(evt_poster_7))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_7"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_11"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3736), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_8"), STRING("a_kami_12"), PTR(evt_poster_8))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_8"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_12"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3737), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_9"), STRING("a_kami_12"), PTR(evt_poster_9))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_9"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_12"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3738), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_10"), STRING("a_kami_10"), PTR(evt_poster_10))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_10"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_10"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_13"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3739), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_11"), STRING("a_kami_10"), PTR(evt_poster_11))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_11"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_10"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_14"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_15"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_6"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_7"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3740), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_12"), STRING("a_kami_10"), PTR(evt_poster_12))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_12"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_10"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_8"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_9"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kami_5"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3741), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_13"), STRING("a_kami_10"), PTR(evt_poster_13))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_13"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_10"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3742), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_14"), STRING("a_kami_11"), PTR(evt_poster_14))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_14"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_11"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(3743), 0)
        CALL(evt_cloud_ent, 0, STRING("kami_15"), STRING("a_kami_11"), PTR(evt_poster_15))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kami_15"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_11"), 1, 0)
    END_IF
    CALL(evt_hitobj_onoff, STRING("a_kabe"), 1, 0)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL_CHILD(PTR(evt_chk))
    RETURN
EVT_END()

static void sun_tev(MapTevCallbackInfo* info) {
    static const s32 texmtx_tbl[] = {
        GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4,
        GX_TEXMTX5, GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9,
    };
    s32 stage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;
    GXTexObj obj;
    f32 mtx[2][3];
    u8* buf;
    s32 i;
    s32 j;

    // a scrolling sine-wave warp texture for the sun's heat haze
    if (ind_lp == NULL) {
        ind_lp = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), 1);
    }
    buf = ind_lp->address;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            f32 v = sin(31.41592f * j / 64 - gp->retraceCount / 8.0f);
            s32 off;

            off = i / 4;
            off = (off << 5) + ((j / 4) << 9);
            off += ((i % 4) + ((j % 4) << 2)) * 2;
            buf[off] = 3.0f * v + 128.0f;
            buf[off + 1] = 0x80;
        }
    }
    DCFlushRange(buf, 0x2000);
    GXInitTexObj(&obj, buf, 64, 64, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, texMap);
    mtx[0][0] = 0.5f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.5f;
    mtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, mtx, 1);
    GXSetIndTexOrder(indStage, GX_TEXCOORD0, texMap);
    GXSetIndTexCoordScale(indStage, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, indStage, GX_TRUE, GX_FALSE, GX_ITM_0);
    info->tevStage = stage;
    info->texCoord = texCoord;
    info->indStage = indStage + 1;
    info->texMap = texMap + 1;
}

static USER_FUNC(test_glare) {
    MapObject* sun = mapGetMapObj("sun");
    u16* rect;

    marioGetPtr();
    if (isFirstCall) {
        sun->flags |= MAP_OBJ_FLAG_SCREEN_RECT;
        return EVT_RETURN_BLOCK;
    }
    rect = sun->screenRect;
    if (rect[0] == 0x3FF || rect[1] == 0x3FF || rect[2] == 0x3FF || rect[3] == 0x3FF) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else if (rect[0] == 0 && rect[1] == 0 && rect[2] == 0 && rect[3] == 0) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else {
        envGlareFilter(1, rect[1] - 50, rect[0] - 50, rect[3] + 50, rect[2] + 50);
        envGlareFilterZ(1.148f);
    }
    return EVT_RETURN_BLOCK;
}

