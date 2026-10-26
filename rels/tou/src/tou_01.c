#include "driver/mapdrv.h"
#include "eff/eff_starstone.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_telop.h"
#include "evt/evt_lecture.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "eff/eff_sleep.h"
#include "driver/camdrv.h"
#include "driver/mobjdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_sub.h"
#include "tou.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"

typedef struct ShadowWork { GXTexObj obj; smartEntry* tex; } ShadowWork;
void make_shadow_disp(CameraId camera, void* param);
void envSetYamiView(Mtx mtx);

static USER_FUNC(anim_tevcallback);
static USER_FUNC(anim_tevcallback_mobj);
static USER_FUNC(delete_npc_regl_evt);
USER_FUNC(eff_sleep_off);
USER_FUNC(eff_sleep_on);
USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_tou_get_ranking);
USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(make_shadow);
static USER_FUNC(mario_chk);
static USER_FUNC(proj_mtx);
static USER_FUNC(proj_off);
static USER_FUNC(proj_on);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(unitwincoin_disp_onoff);
USER_FUNC(unitwincoin_wait);
USER_FUNC(yami_oowareru_check);
#ifdef VERSION_G8ME01
USER_FUNC(tou_01_kanban_moji_flag_sync);
#endif
static USER_FUNC(yami_view);
extern s32 envSetYami[];

static EvtWord n_door_open[60];
static EvtWord n_door_close[58];
static BeroEntry bero_data[2];
static EvtWord evt_door[15];
static EvtWord evt_door2[12];
static EvtWord evt_wall[22];
EvtWord evt_door_1_open[12];
EvtWord evt_door_1_close[14];
static const char* mise_npc[2];
static DoorParam door_1;
static EvtWord evt_door_2[12];
static EvtWord evt_door2_2[12];
EvtWord evt_sakaba_1[26];
EvtWord evt_sakaba_2[26];
EvtWord evt_sakaba_3[26];
EvtWord evt_sakaba_4[29];
#ifdef VERSION_G8ME01
EvtWord evt_wall2[38];
#else
EvtWord evt_wall2[19];
#endif
const char* sakaba_npc[7];
const char* sakaba_item[2];
EvtWord evt_door_2_open[12];
EvtWord evt_door_2_close[14];
static DoorParam door_2;
static EvtWord evt_kanban[51];
static EvtWord evt_kanban2[175];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[13];
static ShopperData35 shopper_data;
EvtWord evt_hotdog[86];
EvtWord evt_ship_shadow[91];
EvtWord init_hikuu[12];
EvtWord talk_hikuu[84];
EvtWord init_sakaba[8];
EvtWord talk_sakaba[369];
EvtWord init_mise[12];
EvtWord talk_mise[2];
EvtWord init_hot[13];
EvtWord talk_hot[21];
static EvtWord init_papa[20];
static EvtWord talk_papa[78];
static EvtWord init_mama[20];
static EvtWord talk_mama[78];
static EvtWord init_konari[20];
static EvtWord talk_konari[78];
static EvtWord talk_musume[132];
EvtWord init_musume_1[14];
EvtWord talk_musume_1[4];
EvtWord init_musume_2[14];
EvtWord talk_musume_2[4];
EvtWord init_musume_3[14];
EvtWord talk_musume_3[4];
EvtWord init_kino_1[13];
EvtWord reg_kino_1[31];
EvtWord talk_kino_1[112];
EvtWord init_kino_2[8];
EvtWord talk_kino_2[112];
EvtWord init_roten_1[8];
EvtWord talk_roten_1[112];
EvtWord init_roten_2[8];
EvtWord talk_roten_2[112];
EvtWord init_jugem_1[8];
EvtWord talk_jugem_1[112];
EvtWord init_jugem_2[12];
EvtWord talk_jugem_2[112];
EvtWord init_bijyo[40];
EvtWord talk_bijyo[112];
static EvtWord init_kiza[21];
static EvtWord talk_kiza[60];
static EvtWord init_sarary[12];
EvtWord select_number[44];
static EvtWord talk_sarary[479];
static NpcSetupInfo npcEnt[20];
EvtWord evt_uron[195];
EvtWord evt_tamago_sleep[56];
EvtWord dummy[2];
EvtWord hot_move_evt[66];
EvtWord hot_move_evt2[24];
EvtWord evt_oyaji_move[60];
static EvtWord evt_tamago_scale[89];
static EvtWord evt_tamago_scale2[83];
s32 yoshi_data[30];
EvtWord evt_tamago_move[306];
EvtWord evt_tamago[100];
EvtWord talk_tamago[321];
EvtWord evt_gondora_updown[143];
EvtWord evt_gondora_mon[62];
EvtWord evt_gondora[169];
EvtWord evt_gondora2[200];
EvtWord evt_search_light[82];
EvtWord evt_dummy_sw[178];
EvtWord evt_peach_after[92];
static EvtWord sekai_yami_npc_init[84];
static EvtWord sekai_yami_npc[65];
static EvtWord sekai_yami_evt[215];
static EvtWord minnnanokoe_npc_entry[175];
static EvtWord minnnanokoe_evt[446];
EvtWord evt_telbox[124];
EvtWord evt_mail[175];
static EvtWord evt_key[7];
static EvtWord mobj_box00[36];
#ifdef VERSION_G8ME01
EvtWord tou_01_init_evt[1641];
#else
EvtWord tou_01_init_evt[627];
#endif

static USER_FUNC(proj_off) {
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[0], 0x400);
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[1], 0x400);
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[2], 0x400);
    if (partyGetPtr(marioGetPartyId()) != NULL) {
        animPoseSetMaterialFlagOff(partyGetPtr(marioGetPartyId())->paperAnimGroup, 0x400);
    }
    if (partyGetPtr(marioGetExtraPartyId()) != NULL) {
        animPoseSetMaterialFlagOff(partyGetPtr(marioGetExtraPartyId())->paperAnimGroup, 0x400);
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(n_door_open)
    IF_FLOAT_EQUAL(LW(0), FLOAT(0.0f))
        IF_INT_EQUAL(GSWF(2526), 0)
            SET_VALUE(GSWF(2526), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AUTO_DOOR1"), LW(0), LW(1), LW(2), GW(13))
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_FLOAT_EQUAL(LW(0), FLOAT(180.0f))
        SET_VALUE(GSWF(2526), 0)
        CALL(evt_snd_sfxoff, GW(13))
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(0.3330078f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    CALL(proj_off)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_close)
    IF_FLOAT_EQUAL(LW(0), FLOAT(180.0f))
        IF_INT_EQUAL(GSWF(2526), 0)
            SET_VALUE(GSWF(2526), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AUTO_DOOR1"), LW(0), LW(1), LW(2), GW(13))
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_FLOAT_EQUAL(LW(0), FLOAT(0.0f))
        SET_VALUE(GSWF(2526), 0)
        CALL(evt_snd_sfxoff, GW(13))
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(0.3330078f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "a_door_mon", // hitName
        1, // type
        0, 0, // sfxId..directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        9, // caseType
        NULL, // exitEvent
        "tou_02", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5546875f))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door2)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("mise_N"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("mise_H"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static const char* mise_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

static DoorParam door_1 = {
    1, // flags
    2, // type
    4, // se
    "door_2", // mapobj
    "a_door_1_N", // hitIn
    "a_door_1", // hitOut
    "mise_soto", // outside
    NULL, // outsideHit
    "mise_naka", // inside
    "a_mise_naka", // insideHit
    evt_door, // evtIn
    evt_door2, // evtOut
    evt_wall, // evtWall
    mise_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_1_open, // evtOpen
    evt_door_1_close, // evtClose
};

static EVT_BEGIN(evt_door_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_3"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door2_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_3"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_sakaba_1)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_get_position, STRING("sak_soto_1"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(10), LW(11), LW(12), 0)
    END_IF
    CALL(evt_mapobj_rotate, 1, STRING("sak_soto_1"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_sakaba_2)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_get_position, STRING("sak_soto_2"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(10), LW(11), LW(12), 0)
    END_IF
    CALL(evt_mapobj_rotate, 1, STRING("sak_soto_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_sakaba_3)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_get_position, STRING("sak_soto_3"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(10), LW(11), LW(12), 0)
    END_IF
    CALL(evt_mapobj_rotate, 1, STRING("sak_soto_3"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_sakaba_4)
    IF_INT_EQUAL(LW(6), 0)
        CALL(evt_mapobj_get_position, STRING("sak_soto_4"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(10), LW(11), LW(12), 0)
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("sak_soto_4"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_wall2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
#ifdef VERSION_G8ME01
    IF_INT_EQUAL(LW(1), 0)
        SPAWN_EVENT(PTR(evt_sakaba_4))
        WAIT_MS(150)
#endif
    SPAWN_EVENT(PTR(evt_sakaba_1))
#ifdef VERSION_G8ME01
        WAIT_MS(150)
#else
    WAIT_MS(100)
#endif
    SPAWN_EVENT(PTR(evt_sakaba_2))
#ifdef VERSION_G8ME01
        WAIT_MS(150)
#else
    WAIT_MS(100)
#endif
    SPAWN_EVENT(PTR(evt_sakaba_3))
#ifdef VERSION_G8ME01
        ELSE
        SPAWN_EVENT(PTR(evt_sakaba_3))
        WAIT_MS(150)
        SPAWN_EVENT(PTR(evt_sakaba_2))
        WAIT_MS(150)
        SPAWN_EVENT(PTR(evt_sakaba_1))
        WAIT_MS(150)
#else
    WAIT_MS(100)
#endif
    SPAWN_EVENT(PTR(evt_sakaba_4))
#ifdef VERSION_G8ME01
    END_IF
#endif
    RETURN
EVT_END()

const char* sakaba_npc[] = {
    /*"酒場店主"*/"\x8E\xF0\x8F\xEA\x93\x58\x8E\xE5",
    /*"美女"*/"\x94\xFC\x8F\x97",
    /*"ロテン2"*/"\x83\x8D\x83\x65\x83\x93\x32",
    /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59",
    /*"ジュゲム2"*/"\x83\x57\x83\x85\x83\x51\x83\x80\x32",
    /*"キノピコ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x52",
    NULL,
};

const char* sakaba_item[] = {
    "item_03",
    NULL,
};

EVT_BEGIN(evt_door_2_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_2_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_2 = {
    3, // flags
    2, // type
    4, // se
    "door_3", // mapobj
    "a_door_2_N", // hitIn
    "a_door_2", // hitOut
    "sakaba_soto", // outside
    NULL, // outsideHit
    "sakaba_naka", // inside
    "a_sakaba_naka", // insideHit
    evt_door_2, // evtIn
    evt_door2_2, // evtOut
    evt_wall2, // evtWall
    (const char**)&sakaba_npc, // npcs
    NULL, // mobjs
    (const char**)&sakaba_item, // items
    evt_door_2_open, // evtOpen
    evt_door_2_close, // evtClose
};

static EVT_BEGIN(evt_kanban)
    LOOP(0)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 2000, -2000, 60)
        LOOP(61)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("kanban_e"), LW(0), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, -2000, 2000, 60)
        LOOP(61)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("kanban_e"), LW(0), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban2)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SIN_OUT, 0, 1000, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("taore"), 0, 0, LW(0))
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(GF(10), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 1000, -1000, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("taore"), 0, 0, LW(0))
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(GF(10), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -1000, 1000, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("taore"), 0, 0, LW(0))
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(GF(10), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN, 1000, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("taore"), 0, 0, LW(0))
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(GF(10), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_random, 3000, LW(0))
        ADD(LW(0), 3000)
        WAIT_MS(3000)
    END_LOOP
    WAIT_FRAMES(1)
    CALL(evt_mapobj_rotate, 1, STRING("taore"), 0, 0, 0)
    RETURN
EVT_END()

static ShopObj obj_list[] = {
    {"sina_1", "a_sina1"},
    {"sina_2", "a_sina_2"},
    {"sina_3", "a_sina_3"},
    {"sina_4", "a_sina_4"},
    {"sina_5", "a_sina_5"},
    {"sina_6", "a_sina_6"},
};

static ShopItem goods_list[] = {
    {ITEM_EARTH_QUAKE, 15},
    {ITEM_THUNDER_BOLT, 12},
    {ITEM_POWER_PUNCH, 15},
    {ITEM_REPEL_CAPE, 15},
    {ITEM_POINT_SWAP, 5},
    {ITEM_SUPER_SHROOM, 15},
};

static ShopDb trade_list[] = {
    {ITEM_EARTH_QUAKE, 7, 0},
    {ITEM_THUNDER_BOLT, 6, 0},
    {ITEM_POWER_PUNCH, 7, 0},
    {ITEM_REPEL_CAPE, 7, 0},
    {ITEM_POINT_SWAP, 2, 0},
    {ITEM_SUPER_SHROOM, 7, 0},
    {ITEM_ZESS_DINNER, 15, 0},
    {ITEM_ZESS_SPECIAL, 25, 0},
    {ITEM_ZESS_DELUXE, 75, 0},
    {ITEM_COCONUT, 3, 0},
    {ITEM_KEEL_MANGO, 3, 0},
    {ITEM_PEACHY_PEACH, 6, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"stg3_shop_00", "stg3_shop_01", "stg3_shop_02", "stg3_shop_03", "stg3_shop_04", "stg3_shop_05", "stg3_shop_06", "stg3_shop_07", "stg3_shop_08", "stg3_shop_09", "stg3_shop_10", "stg3_shop_11", "stg3_shop_12", "stg3_shop_13", "stg3_shop_14", "stg3_shop_15", "stg3_shop_16", "stg3_shop_17", "stg3_shop_18", "stg3_shop_19", "stg3_shop_20", "stg3_shop_21", "stg3_shop_22", "stg3_shop_23", "stg3_shop_24", "stg3_shop_25", "stg3_shop_26", "stg3_shop_27", "stg3_shop_28", "stg3_shop_29", "stg3_shop_30", "stg3_shop_31", "stg3_shop_32", "stg3_shop_33", "stg3_shop_34"}, // msgs
};

EVT_BEGIN(evt_hotdog)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_set_dir_to_target, STRING("ホットドッグ"), STRING("mario"))
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL(evt_msg_print, 0, STRING("stg3_tou_13_01"), 0, STRING("ホットドッグ"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_13"), 0, STRING("ホットドッグ"))
    END_IF
    CALL(evt_win_coin_on, 0, LW(10))
    CALL(evt_msg_select, 0, STRING("stg3_tou_13_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_pouch_get_coin, LW(0))
        IF_INT_LESS(LW(0), 10)
            CALL(evt_msg_print_add, 0, STRING("stg3_tou_15"))
            ELSE
            CALL(evt_pouch_add_item, ITEM_HOT_DOG, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_pouch_add_coin, -10)
                CALL(evt_msg_print_add, 0, STRING("stg3_tou_14"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("stg3_shop_18"))
            END_IF
        END_IF
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg3_tou_16"))
    END_IF
    CALL(evt_win_coin_off, LW(10))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static USER_FUNC(proj_on) {
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[0], 0x400);
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[1], 0x400);
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[2], 0x400);
    if (partyGetPtr(marioGetPartyId()) != NULL) {
        animPoseSetMaterialFlagOn(partyGetPtr(marioGetPartyId())->paperAnimGroup, 0x400);
    }
    if (partyGetPtr(marioGetExtraPartyId()) != NULL) {
        animPoseSetMaterialFlagOn(partyGetPtr(marioGetExtraPartyId())->paperAnimGroup, 0x400);
    }
    animPoseSetMaterialFlagOn(npcNameToPtr("\224\362\213\363\221D")->poseId, 0x400);
    return EVT_RETURN_DONE;
}

void make_shadow_disp(CameraId camera, void* param) {
    ShadowWork* wp = param;
    Mtx44 proj;
    Mtx view;
    Mtx mtx;
    f32 viewport[6];
    f32 projv[7];
    u32 left;
    u32 top;
    u32 width;
    u32 height;
    f32 t;

    GXGetViewportv(viewport);
    GXGetProjectionv(projv);
    GXGetScissor(&left, &top, &width, &height);
    GXSetViewport(0.0f, 0.0f, 256.0f, 256.0f, 0.0f, 1.0f);
    GXSetScissor(1, 1, 254, 254);
    C_MTXPerspective(proj, 25.0f, 1.2666667f, 1.0f, 1000.0f);
    GXSetProjection(proj, GX_PERSPECTIVE);
    C_MTXLookAt(view, &(Vec){0.0f, (t = tan(0.21816615760326385), 128.0f / t), 0.0f}, &(Vec){0.0f, 0.0f, -1.0f},
                &(Vec){0.0f, 0.0f, 0.0f});
    PSMTXScale(mtx, 128.0f, 64.0f, 64.0f);
    PSMTXConcat(view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0x80, 0x80, 0x80, 0x40});
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXDrawSphere(16, 8);
    GXSetTexCopySrc(0, 0, 256, 256);
    GXSetTexCopyDst(128, 128, GX_TF_RGB5A3, GX_TRUE);
    GXCopyTex(wp->tex->address, GX_TRUE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projv);
    GXSetScissor(left, top, width, height);
}

USER_FUNC(make_shadow) {
    ShadowWork* wp;

    if (isFirstCall) {
        wp = _mapAlloc(sizeof(ShadowWork));
        event->userdata[0] = (EvtWord)wp;
        wp->tex = smartAlloc(GXGetTexBufferSize(128, 128, GX_TF_RGB5A3, GX_FALSE, 0), 1);
    }
    wp = (ShadowWork*)event->userdata[0];
    GXInitTexObj(&wp->obj, wp->tex->address, 128, 128, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&wp->obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    mapSetProjTexObj(&wp->obj, wp->tex);
    dispEntry(CAMERA_SHADOW, 0.0f, 3, make_shadow_disp, wp);
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(proj_mtx) {
    EvtWord* args = event->args;
    f32 px = evtGetFloat(event, args[0]);
    f32 py = evtGetFloat(event, args[1]);
    f32 pz = evtGetFloat(event, args[2]);
    f32 tx = evtGetFloat(event, args[3]);
    f32 ty = evtGetFloat(event, args[4]);
    f32 tz = evtGetFloat(event, args[5]);
    Mtx view;
    Mtx proj;
    Vec pos;
    Vec target;
    Vec up;

    pos = (Vec){px, py, pz};
    target = (Vec){tx, ty, tz};
    up = (Vec){0.0f, 0.0f, -1.0f};
    C_MTXLightPerspective(proj, 25.0f, 1.2666667f, 0.5f, 0.5f, 0.5f, 0.5f);
    C_MTXLookAt(view, &pos, &up, &target);
    PSMTXConcat(proj, view, proj);
    mapSetProjMtx(proj);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_ship_shadow)
    CALL(proj_on)
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(make_shadow)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tou_jimenn"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tou_jimenn"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gondora"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("den"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("den"), MAP_OBJ_FLAG_DISPLAY_LIST)
    LOOP(0)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 2000, 2300, 100)
        LOOP(101)
            CALL(evt_sub_intpl_get_value)
            CALL(proj_mtx, 0, LW(0), 420, 0, 0, 420)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 2300, 2000, 100)
        LOOP(101)
            CALL(evt_sub_intpl_get_value)
            CALL(proj_mtx, 0, LW(0), 420, 0, 0, 420)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(init_hikuu)
    CALL(evt_npc_set_position, STRING("me"), 100, 0, 430)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

EVT_BEGIN(talk_hikuu)
    CALL(evt_msg_print, 0, STRING("stg3_tou_00"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg3_tou_00_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg3_tou_02"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
    CALL(evt_msg_print_add, 0, STRING("stg3_tou_02_01"))
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_dispdir, LW(3))
    IF_INT_LESS(LW(3), 180)
        ADD(LW(0), -20)
        ADD(LW(1), 15)
        ELSE
        ADD(LW(0), 20)
        ADD(LW(1), 15)
    END_IF
    CALL(evt_item_entry, STRING("ticket"), ITEM_BLIMP_TICKET, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("ticket"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_01"), 0, 0)
    CALL_CHILD(PTR(evt_gondora2))
    RETURN
EVT_END()

EVT_BEGIN(init_sakaba)
    CALL(evt_npc_set_position, STRING("me"), 650, 10, 12)
    RETURN
EVT_END()

EVT_BEGIN(talk_sakaba)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(4), 0)
                CALL(evt_msg_print, 0, STRING("irai_25_00"), 0, 0)
                SET_VALUE(GSWF_TROUBLE_WORK(3), 1)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_tou_05"), 0, 0)
        CASE_EQUAL(139)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 650, 45, FLOAT(80.0f))
            CALL(evt_mario_set_dir, 270, 200, 0)
            CALL(evt_mario_adjust_dir)
        END_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 670, 80, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_06"), 0, 0)
        CALL(evt_mario_set_pose, STRING("M_N_1"))
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_06_01"), 0, 0)
        CALL(lect_juyoitem_rel_set, PTR(all_juyoitem_lecture_rel))
        CALL(evt_item_entry, STRING("dummy"), ITEM_SUPER_HAMMER, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_item_get_item, STRING("dummy"))
        CALL(evt_item_get_item_end_wait, STRING("dummy"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_08"), 0, 0)
        BEGIN_INLINE_EVENT_GET_ID(LW(11))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
            CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1500)
        END_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION2"))
        SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
        WAIT_MS(500)
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING("stg3_tou_297_10"))
        CALL(evt_party_run, 0)
        LOOP(0)
            IS_EVENT_RUNNING(LW(11), LW(12))
            IF_INT_EQUAL(LW(12), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_balloon_bikkuri)
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_INT_EQUAL(LW(0), 256)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_balloon_off)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
        DELETE_EVENT(LW(10))
        WAIT_FRAMES(1)
        CALL(evt_sub_rumble_onoff, 1, 0)
        CALL(evt_mario_set_pose, STRING("M_N_9"))
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_10"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_11"), 0, 0)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print_party, STRING("stg3_tou_297_12"))
        CALL(evt_pouch_receive_mail, 8)
        CALL(evt_pouch_open_mail, 8)
        SET_VALUE(GSW_STORY_PROGRESS, 140)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
        CASE_EQUAL(140)
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_09"), 0, 0)
        CASE_BETWEEN(354, 354)
        IF_INT_EQUAL(GSWF(3877), 1)
            SET_VALUE(GSWF(3879), 1)
            CALL(evt_msg_print, 0, STRING("stg7_tou_01"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_06"), 0, 0)
        END_IF
        CASE_LESS(172)
        CALL(evt_msg_print, 0, STRING("stg3_tou_06"), 0, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_re_144"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_146"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_146_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_tou_re_145"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_tou_re_146_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_mise)
    CALL(evt_npc_set_position, STRING("me"), -600, 10, 140)
    CALL(evt_npc_set_ry, STRING("me"), 200)
    RETURN
EVT_END()

EVT_BEGIN(talk_mise)
    RETURN
EVT_END()

EVT_BEGIN(init_hot)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_set_position, STRING("me"), -400, 20, -125)
    RETURN
EVT_END()

EVT_BEGIN(talk_hot)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(136)
        CALL(evt_msg_print, 0, STRING("stg3_tou_243"), 0, 0)
        CASE_LESS(137)
        CALL(evt_msg_print, 0, STRING("stg3_tou_244"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(279)
        CALL(evt_npc_set_position, STRING("me"), -175, 0, -25)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_07"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_01_07_1"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(279)
        CALL(evt_npc_set_position, STRING("me"), -130, 0, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_07"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_02_07_1"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(279)
        CALL(evt_npc_set_position, STRING("me"), -200, 0, 10)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_07"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_03_07_1"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_musume)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_01"), 0, STRING("３人娘１"))
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_02"), 0, STRING("３人娘２"))
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_03"), 0, STRING("３人娘３"))
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_04"), 0, STRING("３人娘１"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_05"), 0, STRING("３人娘２"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_06"), 0, STRING("３人娘３"))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_07"), 0, STRING("３人娘１"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_08"), 0, STRING("３人娘２"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_09"), 0, STRING("３人娘３"))
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_10"), 0, STRING("３人娘１"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_11"), 0, STRING("３人娘２"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_12"), 0, STRING("３人娘３"))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_13"), 0, STRING("３人娘１"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_14"), 0, STRING("３人娘２"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_15"), 0, STRING("３人娘３"))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_16"), 0, STRING("３人娘１"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_17"), 0, STRING("３人娘２"))
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_08_18"), 0, STRING("３人娘３"))
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_musume_1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 164)
        CALL(evt_npc_set_position, STRING("me"), 300, 10, -150)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume_1)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_musume_2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 164)
        CALL(evt_npc_set_position, STRING("me"), 330, 10, -180)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume_2)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_musume_3)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 164)
        CALL(evt_npc_set_position, STRING("me"), 370, 10, -140)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume_3)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_kino_1)
    CALL(evt_npc_set_position, STRING("me"), -350, 0, 75)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

EVT_BEGIN(reg_kino_1)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), -350, 75, 0, FLOAT(48.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING("me"))
        CALL(evt_npc_move_position, STRING("me"), 350, 75, 0, FLOAT(48.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING("me"))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(talk_kino_1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_04_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_002"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_002_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_001"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_002_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_kino_2)
    CALL(evt_npc_set_position, STRING("me"), -450, 0, 625)
    RETURN
EVT_END()

EVT_BEGIN(talk_kino_2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_05_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_004"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_004_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_003"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_004_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_roten_1)
    CALL(evt_npc_set_position, STRING("ロテン1"), 350, 0, 700)
    RETURN
EVT_END()

EVT_BEGIN(talk_roten_1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_06_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_006"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_006_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_005"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_006_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_roten_2)
    CALL(evt_npc_set_position, STRING("me"), 725, 10, 40)
    RETURN
EVT_END()

EVT_BEGIN(talk_roten_2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_14_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_022"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_022_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_021"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_022_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_jugem_1)
    CALL(evt_npc_set_position, STRING("me"), -400, 0, 625)
    RETURN
EVT_END()

EVT_BEGIN(talk_jugem_1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_07_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_008"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_008_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_007"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_008_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_jugem_2)
    CALL(evt_npc_set_position, STRING("me"), 700, 10, 105)
    CALL(evt_npc_set_ry, STRING("me"), 340)
    RETURN
EVT_END()

EVT_BEGIN(talk_jugem_2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_13_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_020"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_020_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_019"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_020_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_bijyo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(139, 142)
        CASE_BETWEEN(144, 147)
        IF_INT_EQUAL(GSWF(2445), 1)
            CALL(evt_npc_set_position, STRING("me"), 610, 10, -120)
        END_IF
        CASE_LESS(172)
        CALL(evt_npc_set_position, STRING("me"), 610, 10, -120)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 1)
            CALL(evt_npc_set_position, STRING("me"), 610, 10, -120)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_bijyo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_06"), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_11_07"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_016"), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_016_1"), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_015"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_re_016_1"), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_kiza)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 164)
        CALL(evt_npc_set_position, STRING("me"), 555, 10, -30)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_kiza)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_01"), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_02"), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_03"), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_04"), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_05"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_01_12_06"), 0, 0)
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_sarary)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 345)
        CALL(evt_npc_set_position, STRING("me"), -240, 10, -90)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(select_number)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_msg_select, 0, STRING("irai_24_010"))
            IF_INT_EQUAL(LW(0), 5)
                SET_VALUE(LW(15), 1)
                ELSE
                BREAK_LOOP
            END_IF
            ELSE
            CALL(evt_msg_select, 0, STRING("irai_24_011"))
            IF_INT_EQUAL(LW(0), 5)
                SET_VALUE(LW(15), 0)
                ELSE
                ADD(LW(0), 5)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(talk_sarary)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SECURITY_CODE)
        CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), NPC_FLAG_NO_HIT)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, -160, -95, 60)
            CALL(evt_party_set_dir_npc, 0, STRING("サラリーマン"))
        END_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -200, -90, 60)
        CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
        CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("サラリーマン"))
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 0, STRING("サラリーマン"), NPC_FLAG_NO_HIT)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_24_001"), 0, 0)
            CALL(evt_msg_print_add, 0, STRING("irai_24_002"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_24_006"), 0, 0)
            CALL(evt_msg_select, 0, STRING("irai_24_007"))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("irai_24_029"))
                RETURN
            END_IF
            SET_VALUE(GW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("irai_24_009"))
            CALL_CHILD(PTR(select_number))
            IF_INT_EQUAL(LW(0), 2)
                ADD(GW(0), 1)
            END_IF
            CALL(evt_msg_print_add, 0, STRING("irai_24_012"))
            CALL_CHILD(PTR(select_number))
            IF_INT_EQUAL(LW(0), 6)
                ADD(GW(0), 1)
            END_IF
            CALL(evt_msg_print_add, 0, STRING("irai_24_015"))
            CALL_CHILD(PTR(select_number))
            IF_INT_EQUAL(LW(0), 2)
                ADD(GW(0), 1)
            END_IF
            CALL(evt_msg_print_add, 0, STRING("irai_24_018"))
            CALL_CHILD(PTR(select_number))
            IF_INT_EQUAL(LW(0), 5)
                ADD(GW(0), 1)
            END_IF
            IF_INT_LESS(GW(0), 4)
                CALL(evt_msg_print_add, 0, STRING("irai_24_028"))
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("irai_24_021"))
            BEGIN_INLINE_EVENT
                WAIT_MS(500)
                CALL(evt_mario_set_dir, 270, 300, 0)
            END_INLINE_EVENT
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
            CALL(evt_npc_move_position, STRING("me"), -353, -54, 0, FLOAT(60.0f), 0)
            CALL(evt_npc_status_onoff, 0, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
            WAIT_FRAMES(1)
            CALL(evt_msg_print, 0, STRING("irai_24_022"), 0, 0)
            CALL(evt_msg_print, 0, STRING("irai_24_023"), 0, STRING("ホットドッグ"))
            CALL(evt_fade_out, 800)
            CALL(evt_npc_set_position, STRING("me"), -240, 10, -90)
            CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
            WAIT_MS(2000)
            CALL(evt_fade_in, 800)
            CALL(evt_msg_print, 0, STRING("irai_24_024"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("irai_24_025"), 0, 0)
            CALL(evt_pouch_get_haveitemcnt, LW(0))
            CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(1), 10)
                ELSE
                SET_VALUE(LW(1), 20)
            END_IF
            IF_INT_GREATER_OR_EQUAL(LW(0), LW(1))
                CALL(evt_msg_print_add, 0, STRING("irai_24_025_1"))
                CALL(evt_npc_set_ry, STRING("サラリーマン"), 270)
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("サラリーマン"))
                WAIT_MS(300)
                CALL(evt_item_entry, STRING("iri_24"), ITEM_HOT_SAUCE, -265, 10, -70, ITEM_TYPE_STILL, GSWF(2528), 0)
                WAIT_MS(300)
                CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("サラリーマン"))
                ELSE
                CALL(evt_msg_continue)
                CALL(evt_item_entry, STRING("rk"), ITEM_HOT_SAUCE, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("rk"))
                CALL(evt_item_get_item_end_wait, STRING("rk"))
                SET_VALUE(GSWF(2528), 1)
            END_IF
            CALL(evt_msg_print, 0, STRING("irai_24_027"), 0, 0)
            SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
            SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_SECURITY_CODE), 1)
            CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        END_IF
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SECURITY_CODE), 1)
        CALL(evt_msg_print, 0, STRING("irai_24_030"), 0, 0)
        CALL(evt_msg_select, 0, STRING("irai_24_031"))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("irai_24_035"))
            RETURN
        END_IF
        CALL(evt_msg_print_add, 0, STRING("irai_24_032"))
        CALL(evt_unitwin_alloc, 0)
        CALL(unitwincoin_disp_onoff, 1)
        CALL(evt_msg_select, 0, STRING("irai_24_033"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_pouch_get_coin, LW(0))
            IF_INT_LESS(LW(0), 10)
                CALL(unitwincoin_disp_onoff, 0)
                CALL(evt_unitwin_free)
                CALL(evt_msg_print_add, 0, STRING("irai_24_034_1"))
                RETURN
                ELSE
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    SET_VALUE(LW(1), 10)
                    ELSE
                    SET_VALUE(LW(1), 20)
                END_IF
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(1))
                    CALL(unitwincoin_disp_onoff, 0)
                    CALL(evt_unitwin_free)
                    CALL(evt_msg_print_add, 0, STRING("irai_24_034_2"))
                    RETURN
                END_IF
                CALL(evt_pouch_add_coin, -10)
                CALL(unitwincoin_wait)
                CALL(unitwincoin_disp_onoff, 0)
                CALL(evt_msg_print_add, 0, STRING("irai_24_034"))
                CALL(evt_item_entry, STRING("rk"), ITEM_HOT_SAUCE, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("rk"))
                CALL(evt_item_get_item_end_wait, STRING("rk"))
            END_IF
            ELSE
            CALL(unitwincoin_disp_onoff, 0)
            CALL(evt_msg_print_add, 0, STRING("irai_24_035"))
        END_IF
        CALL(evt_unitwin_free)
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("stg3_tou_16_1"), 0, 0)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"飛空船"*/"\x94\xF2\x8B\xF3\x91\x44", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_hikuu, // initEvent
        NULL, // regularEvent
        talk_hikuu, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"酒場店主"*/"\x8E\xF0\x8F\xEA\x93\x58\x8E\xE5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_sakaba, // initEvent
        NULL, // regularEvent
        talk_sakaba, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_mise, // initEvent
        NULL, // regularEvent
        talk_mise, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ホットドッグ"*/"\x83\x7A\x83\x62\x83\x67\x83\x68\x83\x62\x83\x4F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_hot, // initEvent
        NULL, // regularEvent
        talk_hot, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"たまご"*/"\x82\xBD\x82\xDC\x82\xB2", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"パパ"*/"\x83\x70\x83\x70", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_papa, // initEvent
        NULL, // regularEvent
        talk_papa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ママ"*/"\x83\x7D\x83\x7D", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_mama, // initEvent
        NULL, // regularEvent
        talk_mama, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_konari, // initEvent
        NULL, // regularEvent
        talk_konari, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘１"*/"\x82\x52\x90\x6C\x96\xBA\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_musume_1, // initEvent
        NULL, // regularEvent
        talk_musume_1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘２"*/"\x82\x52\x90\x6C\x96\xBA\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_musume_2, // initEvent
        NULL, // regularEvent
        talk_musume_2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘３"*/"\x82\x52\x90\x6C\x96\xBA\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_musume_3, // initEvent
        NULL, // regularEvent
        talk_musume_3, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ1"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x31", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kino_1, // initEvent
        reg_kino_1, // regularEvent
        talk_kino_1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ2"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x32", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kino_2, // initEvent
        NULL, // regularEvent
        talk_kino_2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロテン2"*/"\x83\x8D\x83\x65\x83\x93\x32", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_roten_2, // initEvent
        NULL, // regularEvent
        talk_roten_2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ジュゲム1"*/"\x83\x57\x83\x85\x83\x51\x83\x80\x31", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_jugem_1, // initEvent
        NULL, // regularEvent
        talk_jugem_1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ジュゲム2"*/"\x83\x57\x83\x85\x83\x51\x83\x80\x32", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_jugem_2, // initEvent
        NULL, // regularEvent
        talk_jugem_2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"美女"*/"\x94\xFC\x8F\x97", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_bijyo, // initEvent
        NULL, // regularEvent
        talk_bijyo, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kiza, // initEvent
        NULL, // regularEvent
        talk_kiza, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        init_sarary, // initEvent
        NULL, // regularEvent
        talk_sarary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_uron)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_telop_entry, 6)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 600, 2150, 0, 200, 394, 2000, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -25, 400, FLOAT(60.0f))
    END_INLINE_EVENT
    CALL(evt_party_move_pos2, 0, 25, 400, FLOAT(60.0f))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_03"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_mario_set_dir, 90, 300, 0)
            CALL(evt_sub_random, 1000, LW(0))
            ADD(LW(0), 500)
            WAIT_MS(LW(0))
            CALL(evt_mario_set_dir, 270, 300, 0)
            CALL(evt_sub_random, 1000, LW(0))
            ADD(LW(0), 500)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        LOOP(0)
            CALL(evt_sub_random, 500, LW(0))
            ADD(LW(0), 1000)
            WAIT_MS(LW(0))
            CALL(evt_party_set_dir, 0, 90, 300)
            CALL(evt_sub_random, 1000, LW(0))
            ADD(LW(0), 500)
            WAIT_MS(LW(0))
            CALL(evt_party_set_dir, 0, 270, 300)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -88, 40, 1216, -500, 135, 453, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, 730, 40, 774, 318, 135, 11, 10000, 0)
    WAIT_MS(10000)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_cam3d_evt_set, 0, 10, 740, 0, 60, 393, 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_04"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgm_scope, 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 127)
    RETURN
EVT_END()

USER_FUNC(eff_sleep_on) {
    EffectEntry* eff = effNameToPtr("eff_sleep");
    NpcEntry* npc = npcNameToPtr_NoAssert("\202\275\202\334\202\262");

    if (eff == NULL) {
        effSetName(effSleepEntry(0, 0, npc->position.x, 16.0f + npc->position.y, npc->position.z, 1.0f, 65.0f),
                   "eff_sleep");
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_sleep_off) {
    EffectEntry* eff = effNameToPtr("eff_sleep");

    if (eff != NULL) {
        effSoftDelete(eff);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(mario_chk) {
    EvtWord* args = event->args;
    BOOL ok = FALSE;

    if (marioChkKey() && marioChkCtrl()) {
        ok = TRUE;
    }
    evtSetValue(event, args[0], ok != FALSE);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_tamago_sleep)
    LOOP(0)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 136)
            WAIT_FRAMES(1)
            ELSE
            CALL(evt_sub_intpl_init, EASING_SIN_OUT, 100, 120, 100)
            LOOP(100)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
                CALL(evt_npc_set_scale, STRING("たまご"), LW(0), LW(0), LW(0))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init, EASING_COS_IN, 120, 100, 100)
            LOOP(100)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
                CALL(evt_npc_set_scale, STRING("たまご"), LW(0), LW(0), LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(dummy)
    RETURN
EVT_END()

EVT_BEGIN(hot_move_evt)
    CALL(evt_npc_flag_onoff, 0, STRING("ホットドッグ"), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_flag_onoff, 1, STRING("ホットドッグ"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("ホットドッグ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("ホットドッグ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    LABEL(0)
    CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("ホットドッグ"), LW(3), LW(4), LW(5))
    SUBTRACT(LW(0), LW(3))
    SUBTRACT(LW(2), LW(5))
    DIVIDE(LW(0), 10)
    DIVIDE(LW(2), 10)
    ADD(LW(0), LW(3))
    ADD(LW(2), LW(5))
    CALL(evt_npc_move_position, STRING("ホットドッグ"), LW(0), LW(2), 0, FLOAT(60.0f), 1)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(hot_move_evt2)
    CALL(evt_npc_flag_onoff, 0, STRING("ホットドッグ"), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_flag_onoff, 1, STRING("ホットドッグ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("ホットドッグ"), -288, -110, 0, FLOAT(60.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ホットドッグ"), STRING("たまご"))
    RETURN
EVT_END()

EVT_BEGIN(evt_oyaji_move)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("ホットドッグ"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("ホットドッグ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ホットドッグ"), -470, -95, 0, FLOAT(100.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ホットドッグ"), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_set_position, STRING("ホットドッグ"), -333, 10, -258)
    CALL(evt_npc_move_position, STRING("ホットドッグ"), -238, -180, 0, FLOAT(100.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_242"), 0, STRING("ホットドッグ"))
    SET_VALUE(GF(0), 1)
    CALL(evt_npc_change_interrupt, STRING("ホットドッグ"), 1, PTR(hot_move_evt))
    CALL(evt_npc_restart_regular_event, STRING("ホットドッグ"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_tamago_scale)
    SET_FLOAT_VALUE(GW(10), FLOAT(1.1992188f))
    SET_FLOAT_VALUE(GW(11), FLOAT(0.89941406f))
    BEGIN_INLINE_EVENT
        DIVIDE(LW(3), 2)
        ADD(LW(3), 1)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 120, 90, LW(3))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        DIVIDE(LW(3), 2)
        ADD(LW(3), 1)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 90, 110, LW(3))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(11), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    CONVERT_FRAME_TO_MS(LW(3), LW(4))
    ADD(LW(4), 1)
    LOOP(LW(4))
        CALL(evt_npc_set_scale, STRING("たまご"), GW(10), GW(11), FLOAT(1.0f))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_tamago_scale2)
    BEGIN_INLINE_EVENT
        DIVIDE(LW(3), 2)
        ADD(LW(3), 1)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 90, 100, LW(3))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        DIVIDE(LW(3), 2)
        ADD(LW(3), 1)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 110, 100, LW(3))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(11), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    CONVERT_FRAME_TO_MS(LW(3), LW(4))
    ADD(LW(4), 1)
    LOOP(LW(4))
        CALL(evt_npc_set_scale, STRING("たまご"), GW(10), GW(11), FLOAT(1.0f))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

s32 yoshi_data[] = {
    -340, 0, 60, -285, 0, 0, -245, 0,
    40, -295, 0, 115, -330, 0, 185, -375,
    0, 85, -430, 10, 70, -440, 10, 100,
    -360, 0, 150, -290, 0, 40,
};

EVT_BEGIN(evt_tamago_move)
    CALL(evt_npc_flag_onoff, 1, STRING("たまご"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("たまご"), -400, 10, -135)
    SET_VALUE(LW(3), 600)
    CALL(evt_npc_jump_position_nohit, STRING("たまご"), -400, 40, -60, LW(3), 60)
    SET_VALUE(LW(3), 400)
    CALL(evt_npc_jump_position_nohit, STRING("たまご"), -390, 10, -15, LW(3), 50)
    SET_VALUE(LW(10), 0)
    LABEL(0)
    SET_INT_READ_ADDR(PTR(yoshi_data))
    LOOP(10)
        READ_INT3(LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 400, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 30, LW(4))
        ADD(LW(4), 30)
        MULTIPLY(LW(4), 120)
        DIVIDE(LW(4), 100)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
        CALL(mario_chk, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("たまご"), LW(3), LW(4), LW(5))
            CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
            IF_INT_LESS(LW(6), 20)
                ADD(LW(10), 1)
                DEBUG_PUT_REG(LW(10))
                IF_INT_GREATER_OR_EQUAL(LW(10), 2)
                    GOTO(10)
                END_IF
            END_IF
        END_IF
    END_LOOP
    GOTO(0)
    LABEL(10)
    LOOP(0)
        CALL(mario_chk, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_change_interrupt, STRING("ホットドッグ"), 1, PTR(dummy))
    CALL(evt_npc_restart_regular_event, STRING("ホットドッグ"))
    CALL(evt_set_dir_to_target, STRING("ホットドッグ"), STRING("たまご"))
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale))
    CALL(evt_npc_jump_position_nohit, STRING("たまご"), -360, 10, -60, LW(3), 30)
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale))
    CALL(evt_npc_jump_position_nohit, STRING("たまご"), -400, 10, -45, LW(3), 40)
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale))
    CALL(evt_npc_jump_position_nohit, STRING("たまご"), -400, 110, -120, LW(3), 50)
    LOOP(3)
        CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 200, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 20, LW(4))
        ADD(LW(4), 20)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
    END_LOOP
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale2))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_244"), 0, STRING("ホットドッグ"))
    WAIT_MS(300)
    SET_VALUE(GSW_STORY_PROGRESS, 136)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_cam3d_evt_set, -400, 211, 321, -400, 156, -132, 3000, 11)
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_245"), 0, STRING("たまご"))
    CALL(eff_sleep_on)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_change_interrupt, STRING("ホットドッグ"), 1, PTR(hot_move_evt2))
    CALL(evt_npc_restart_regular_event, STRING("ホットドッグ"))
    CALL(evt_npc_change_interrupt, STRING("たまご"), 6, PTR(talk_tamago))
    CALL(evt_npc_flag_onoff, 0, STRING("たまご"), NPC_FLAG_NO_HIT)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG3_URN1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_tamago)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_wait_move_end)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_241"), 0, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("e"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_cam3d_evt_set, -345, 169, 544, -345, 38, -204, 1000, 11)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    SPAWN_EVENT(PTR(evt_tamago_move))
    SPAWN_EVENT(PTR(evt_oyaji_move))
    WAIT_MS(6000)
    LABEL(0)
    IF_INT_EQUAL(GF(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 135)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(talk_tamago)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(eff_sleep_off)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GSW_STORY_PROGRESS, 137)
    CALL(evt_cam3d_evt_set, -400, 211, 304, -400, 156, -142, 500, 11)
    WAIT_MS(500)
    LOOP(2)
        CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 200, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 20, LW(4))
        ADD(LW(4), 20)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
    END_LOOP
    CALL(evt_msg_print_party, STRING("stg3_tou_247"))
    CALL(evt_msg_select, 0, STRING("stg3_tou_247_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_party_add, STRING("stg3_tou_248"))
        ELSE
        CALL(evt_msg_print_party_add, STRING("stg3_tou_249"))
    END_IF
    CALL(evt_cam3d_evt_set, -300, 111, 306, -300, 56, -140, 500, 11)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING("ホットドッグ"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_250"), 0, STRING("ホットドッグ"))
    CALL(evt_cam3d_evt_set, -400, 211, 304, -400, 156, -142, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_251"))
    LOOP(3)
        CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 200, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 20, LW(4))
        ADD(LW(4), 20)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
    END_LOOP
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale2))
    CALL(evt_msg_print_party, STRING("stg3_tou_252"))
    LOOP(1)
        CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 200, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 20, LW(4))
        ADD(LW(4), 20)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
    END_LOOP
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale2))
    CALL(evt_msg_print_party, STRING("stg3_tou_253"))
    CALL(evt_msg_select, 0, STRING("stg3_tou_253_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_party_add, STRING("stg3_tou_254"))
        ELSE
        CALL(evt_msg_print_party_add, STRING("stg3_tou_255"))
    END_IF
    LOOP(3)
        CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 200, LW(3))
        ADD(LW(3), 200)
        CALL(evt_sub_random, 20, LW(4))
        ADD(LW(4), 20)
        SPAWN_EVENT(PTR(evt_tamago_scale))
        CALL(evt_npc_jump_position_nohit, STRING("たまご"), LW(0), LW(1), LW(2), LW(3), LW(4))
    END_LOOP
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(evt_tamago_scale2))
    CALL(evt_npc_flag_onoff, 0, STRING("ホットドッグ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("ホットドッグ"), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_set_position, STRING("ホットドッグ"), -400, 20, -125)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kaunta_"), 0, PTR(evt_hotdog), 0)
    CALL(evt_npc_get_position, STRING("たまご"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("たまご"), 0, -1000, 0)
    CALL(evt_mario_set_party_pos, 1, 8, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_sub_save_playtime, GSW(508))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_gondora_updown)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    IF_INT_EQUAL(LW(2), 1)
        SET_VALUE(LF(0), 1)
        ELSE
        SET_VALUE(LF(0), 0)
    END_IF
    CALL(evt_mapobj_get_position, STRING("gondora"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), LW(5))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gondora"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hit_bind_mapobj, STRING("a_gondora"), STRING("gondora"))
    MULTIPLY_FLOAT(LW(3), FLOAT(10.0f))
    MULTIPLY_FLOAT(LW(4), FLOAT(10.0f))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(3), LW(4), 4000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, STRING("gondora"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("a_gondora"))
    CALL(evt_mapobj_get_position, STRING("gondora"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfx_pos, LW(5), LW(7), LW(8), LW(9))
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_get_pos, 0, LW(7), LW(8), LW(9))
        SET_VALUE(LW(6), LW(0))
        ADD(LW(6), 9)
        CALL(evt_mario_set_pos, LW(7), LW(6), LW(9))
        CALL(evt_party_get_pos, 0, LW(7), LW(8), LW(9))
        SET_VALUE(LW(6), LW(0))
        ADD(LW(6), 9)
        CALL(evt_party_set_pos, 0, LW(7), LW(6), LW(9))
    END_IF
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    IF_INT_GREATER(LW(4), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_snd_sfxoff, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(evt_gondora_mon)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    CALL(evt_mapobj_get_position, STRING("g_mon"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_DOOR1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_hit_bind_mapobj, STRING("a_g_mon"), STRING("g_mon"))
    MULTIPLY_FLOAT(LW(3), FLOAT(10.0f))
    MULTIPLY_FLOAT(LW(4), FLOAT(10.0f))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, LW(3), LW(4), 800)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_rotate, 1, STRING("g_mon"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("a_g_mon"))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_gondora)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_pos, -20, 315, 410)
    CALL(evt_party_set_pos, 0, 20, 315, 410)
    CALL(evt_party_set_pos, 1, 0, 315, 400)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_cam3d_evt_set, 0, 473, 1000, 0, 400, 403, 0, 11)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gondora"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hit_bind_mapobj, STRING("a_gondora"), STRING("gondora"))
    CALL(evt_mapobj_trans, 1, STRING("gondora"), 0, 300, 0)
    CALL(evt_hit_bind_update, STRING("a_gondora"))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 0, 115, 1000, 0, 42, 403, 4000, 0)
    SET_VALUE(LW(0), 300)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 1)
    CALL_CHILD(PTR(evt_gondora_updown))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 120)
    CALL_CHILD(PTR(evt_gondora_mon))
    CALL(evt_party_move_pos2, 0, 20, 500, FLOAT(60.0f))
    CALL(evt_party_stop, 0)
    CALL(evt_mario_mov_pos2, 20, 410, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, 20, 490, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, -20, 500, FLOAT(60.0f))
    CALL(evt_mario_set_dir_npc, STRING("飛空船"))
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    SET_VALUE(LW(0), 120)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(PTR(evt_gondora_mon))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 800)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(PTR(evt_gondora_updown))
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1500)
    IF_INT_LESS(GSW_STORY_PROGRESS, 127)
        CALL_CHILD(PTR(evt_uron))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_gondora2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(10))
    IF_INT_GREATER(LW(10), 100)
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("飛空船"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_flag_onoff, 1, STRING("飛空船"), NPC_FLAG_NO_HIT)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_mov_pos2, 20, 490, FLOAT(80.0f))
    CALL(evt_party_move_pos2, 0, -20, 490, FLOAT(80.0f))
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 1, -40, 490, FLOAT(80.0f))
    CALL(evt_party_stop, 1)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    SET_VALUE(LW(0), 300)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    CALL_CHILD(PTR(evt_gondora_updown))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 120)
    CALL_CHILD(PTR(evt_gondora_mon))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 0, 20, 490, FLOAT(80.0f))
        CALL(evt_party_move_pos2, 0, 20, 410, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_party_move_pos2, 1, 20, 490, FLOAT(80.0f))
        CALL(evt_party_move_pos2, 1, 0, 400, FLOAT(80.0f))
        CALL(evt_party_stop, 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 20, 410, FLOAT(80.0f))
    CALL(evt_mario_mov_pos2, -20, 410, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    SET_VALUE(LW(0), 120)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(PTR(evt_gondora_mon))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 172)
        IF_INT_EQUAL(GSWF(2496), 0)
            SET_VALUE(GSWF(2496), 1)
            CALL(evt_rankingReset)
        END_IF
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_AIRSHIP1"))
        WAIT_MS(2000)
        CALL(proj_off)
        CALL(evt_bero_mapchange, STRING("tou_00"), STRING("modoru"))
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 300)
    SET_VALUE(LW(2), 1)
    CALL_CHILD(PTR(evt_gondora_updown))
    RETURN
EVT_END()

EVT_BEGIN(evt_search_light)
    SET_VALUE(LW(8), LW(0))
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -50, 50, 3000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, LW(8), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_sub_random, 5, LW(0))
        ADD(LW(0), 1)
        MULTIPLY(LW(0), 500)
        WAIT_MS(LW(0))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 50, -50, 3000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, LW(8), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_random, 5, LW(0))
        ADD(LW(0), 1)
        MULTIPLY(LW(0), 500)
        WAIT_MS(LW(0))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_dummy_sw)
    SET_VALUE(GF(10), 1)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 459, 175, 856, 684, 95, 240, 800, 11)
    WAIT_MS(800)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neon_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_3"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        LOOP(7)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_KAIDAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(675)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(3200)
        LOOP(3)
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(100)
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
        END_LOOP
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(100)
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_kan"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_kan"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neon_3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_sak_kaidan"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("a_hasira"), 1, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 0, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_peach_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 0, 130, 134, 0, 75, -312, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -25, -305, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, 25, -308, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_479"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_set_position, STRING("美女"), 0, -1000, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 172)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static USER_FUNC(yami_view) {
    f32 s;
    f32 c;
    f32 dist = evtGetValue(event, event->args[0]);
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 angle = 6.2831855f * (90.0f + camera->viewYaw) / 360.0f;
    f32 x;
    f32 z;
    f32 ox;
    f32 oz;
    Mtx view;

    s = sin(angle);
    c = cos(angle);
    x = dist * (f32)sin(angle);
    z = dist * -(f32)cos(angle);
    ox = -100.0f * s;
    oz = -100.0f * -c;

    C_MTXLookAt(view, &(Vec){ox + x, 100.0f, oz + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){x, 0.0f, z});
    envSetYamiView(view);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(sekai_yami_npc_init)
    CALL(evt_npc_entry, STRING("キノシコワ"), STRING("c_ksikowa"))
    CALL(evt_npc_set_tribe, STRING("キノシコワ"), STRING("キノシコワ"))
    CALL(evt_npc_set_position, STRING("キノシコワ"), 4, 0, 420)
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_position, STRING("飛空船"), -170, 0, -20)
    CALL(evt_npc_set_position, STRING("ジュゲム1"), 50, 0, 200)
    CALL(evt_npc_set_position, STRING("ロテン2"), -130, 0, 220)
    CALL(evt_npc_flag_onoff, 0, STRING("ロテン2"), NPC_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("パパ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ママ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("コナリキン"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("キノピオ2"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("キノピオ1"), 0, -1000, 0)
    CALL(delete_npc_regl_evt, STRING("キノピオ1"))
    CALL(delete_npc_regl_evt, STRING("ロテン1"))
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_npc)
    LOOP(0)
        CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(yami_oowareru_check, GW(0), LW(1), LW(3), LW(4))
        IF_INT_EQUAL(LW(4), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_reglid, LW(0), LW(1))
    DELETE_EVENT(LW(1))
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(0), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(1000)
    LOOP(0)
        CALL(evt_sub_random, 400, LW(1))
        ADD(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_npc_reverse_ry, LW(0))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, LW(0))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_cam3d_evt_set, 420, 200, 1500, -305, 100, -360, 0, 11)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 64, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_bg_set_color, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("キノシコワ"), 1)
    CALL(anim_tevcallback, STRING("飛空船"), 1)
    CALL(anim_tevcallback, STRING("ジュゲム1"), 1)
    CALL(anim_tevcallback, STRING("ロテン2"), 1)
    CALL(anim_tevcallback, STRING("ホットドッグ"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    SET_VALUE(GW(0), -600)
    SET_VALUE(LW(0), STRING("キノシコワ"))
    SPAWN_EVENT_GET_ID(PTR(sekai_yami_npc), GW(1))
    SET_VALUE(LW(0), STRING("飛空船"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ジュゲム1"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ロテン2"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    CALL(evt_snd_sfxon, STRING("SFX_STG8_WORLD_DARK1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -600, 900, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(yami_view, LW(0))
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 130, 70, 756, -305, 10, -360, 1000, 11)
    WAIT_MS(1000)
    DELETE_EVENT(GW(1))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("キノシコワ"))
    CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("キノシコワ"))
    CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("キノシコワ"))
    CALL(evt_msg_print, 0, STRING("stg8_dark_03"), 0, STRING("キノシコワ"))
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("las_29"), STRING("sekai_yami1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_npc_entry)
    CALL(evt_npc_entry, STRING("キノシコワ"), STRING("c_ksikowa"))
    CALL(evt_npc_set_tribe, STRING("キノシコワ"), STRING("キノシコワ"))
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, 0, 350)
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("ブレッドハート"), STRING("c_bred_h"))
    CALL(evt_npc_set_tribe, STRING("ブレッドハート"), STRING("ブレッドハート"))
    CALL(evt_npc_set_position, STRING("ブレッドハート"), -50, 0, 250)
    CALL(evt_npc_set_ry, STRING("ブレッドハート"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ブレッドハート"), NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("シンエモンＡ"), STRING("c_sinemon_a"))
    CALL(evt_npc_set_tribe, STRING("シンエモンＡ"), STRING("アイアンシンエモン"))
    CALL(evt_npc_set_position, STRING("シンエモンＡ"), 50, 0, 375)
    CALL(evt_npc_set_ry, STRING("シンエモンＡ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("シンエモンＡ"), NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_scale, STRING("シンエモンＡ"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    CALL(evt_npc_entry, STRING("シンエモンＢ"), STRING("c_sinemon_m"))
    CALL(evt_npc_set_tribe, STRING("シンエモンＢ"), STRING("アイアンシンエモン2"))
    CALL(evt_npc_set_position, STRING("シンエモンＢ"), -50, 0, 400)
    CALL(evt_npc_set_ry, STRING("シンエモンＢ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("シンエモンＢ"), NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_scale, STRING("シンエモンＢ"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    CALL(evt_npc_entry, STRING("レポーター"), STRING("c_report"))
    CALL(evt_npc_set_tribe, STRING("レポーター"), STRING("リポーター"))
    CALL(evt_npc_set_position, STRING("レポーター"), 75, 0, 350)
    CALL(evt_npc_set_ry, STRING("レポーター"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("レポーター"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("カメラマン"), STRING("c_kinopio_b"))
    CALL(evt_npc_set_tribe, STRING("カメラマン"), STRING("キノピオ青"))
    CALL(evt_npc_set_position, STRING("カメラマン"), 100, 0, 375)
    CALL(evt_npc_set_ry, STRING("カメラマン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("カメラマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("ウラノコ"), STRING("c_uranoko"))
    CALL(evt_npc_set_tribe, STRING("ウラノコ"), STRING("ウラノコ"))
    CALL(evt_npc_set_position, STRING("ウラノコ"), -100, 0, 300)
    CALL(evt_npc_set_ry, STRING("ウラノコ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ウラノコ"), NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 132, 888, 0, 65, 342, 0, 11)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("酒場店主"), 50, 0, 450)
    CALL(evt_npc_set_ry, STRING("酒場店主"), 90)
    CALL(evt_npc_set_position, STRING("キノピオ1"), -150, 0, 60)
    CALL(evt_npc_set_ry, STRING("キノピオ1"), 90)
    CALL(evt_npc_set_position, STRING("キノピオ2"), -270, 0, 361)
    CALL(evt_npc_set_ry, STRING("キノピオ2"), 90)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("キノシコワ"), 1)
    CALL(anim_tevcallback, STRING("ブレッドハート"), 1)
    CALL(anim_tevcallback, STRING("シンエモンＡ"), 1)
    CALL(anim_tevcallback, STRING("シンエモンＢ"), 1)
    CALL(anim_tevcallback, STRING("酒場店主"), 1)
    CALL(anim_tevcallback, STRING("レポーター"), 1)
    CALL(anim_tevcallback, STRING("カメラマン"), 1)
    CALL(anim_tevcallback, STRING("ウラノコ"), 1)
    CALL(anim_tevcallback, STRING("キノピオ1"), 1)
    CALL(anim_tevcallback, STRING("キノピオ2"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(anim_tevcallback_mobj, STRING("pblk01"), 1)
    CALL(anim_tevcallback_mobj, STRING("hblk01"), 1)
    CALL(yami_view, 700)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tou_jimenn"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gondora"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("den"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(starstone_entry, STRING("starstone"), 0, 60, 415)
    SET_VALUE(GW(0), 60)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), 0, GW(0), 415)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("starstone"), LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 6000, 6500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 6500, 6000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_18"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(300)
    CALL(evt_npc_set_stay_anim, STRING("キノシコワ"))
    CALL(evt_cam3d_evt_set, 500, 60, 1278, 0, 140, 415, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_19"), 0, STRING("キノシコワ"))
    CALL(evt_cam3d_evt_set, 0, 101, 763, 0, 60, 415, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_20"), 0, STRING("キノシコワ"))
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(300)
    CALL(evt_npc_set_stay_anim, STRING("キノシコワ"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_21"), 0, STRING("ブレッドハート"))
    CALL(evt_npc_set_anim, STRING("ブレッドハート"), STRING("H_3"))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, STRING("ブレッドハート"), STRING("S_5"), STRING("T_5"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_22"), 0, STRING("ブレッドハート"))
    CALL(evt_npc_set_stay_anim, STRING("ブレッドハート"))
    CALL(evt_npc_get_position, STRING("シンエモンＡ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("シンエモンＡ"), LW(0), LW(1), LW(2), 300, 10)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_23"), 0, STRING("シンエモンＡ"))
    CALL(evt_npc_get_position, STRING("シンエモンＢ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("シンエモンＢ"), LW(0), LW(1), LW(2), 300, 10)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_24"), 0, STRING("シンエモンＢ"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_24_01"), 0, STRING("ウラノコ"))
    CALL(evt_cam3d_evt_set, 500, 60, 1278, 0, 140, 415, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_25"), 0, STRING("キノシコワ"))
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(300)
    CALL(evt_npc_set_stay_anim, STRING("キノシコワ"))
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("usu_00"), STRING("minnnanokoe"))
    RETURN
EVT_END()

EVT_BEGIN(evt_telbox)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_TELEPHONE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_random, 100, LW(0))
    MODULO(LW(0), 10)
    CALL(evt_mapobj_get_position, STRING("tel_box"), LW(10), LW(11), LW(12))
    CALL(evt_msg_toge, 2, LW(10), LW(11), LW(12))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print, 0, STRING("stg3_tel_01"), 0, 0)
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg3_tel_02"), 0, 0)
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg3_tel_03"), 0, 0)
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg3_tel_04"), 0, 0)
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg3_tel_05"), 0, 0)
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg3_tel_06"), 0, 0)
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg3_tel_07"), 0, 0)
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg3_tel_08"), 0, 0)
        CASE_EQUAL(8)
        CALL(evt_msg_print, 0, STRING("stg3_tel_09"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tel_10"), 0, 0)
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_mail)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -675)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1500)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION2"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    LOOP(0)
        IS_EVENT_RUNNING(LW(11), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_28"), 0, 0)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_29"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_pouch_receive_mail, 11)
    CALL(evt_pouch_open_mail, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    NpcEntry* obj = npcNameToPtr_NoAssert(name);

    if (obj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(obj->poseId, (index & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    MapObjectEntry* obj = mobjNameToPtrNoAssert(name);

    if (obj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(obj->poseId, (index & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static USER_FUNC(delete_npc_regl_evt) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    if (npc->regularEvtId != 0) {
        evtDeleteID(npc->regularEvtId);
        npc->moveEvent = NULL;
        npc->regularEvtId = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yami_oowareru_check) {
    EvtWord* args = event->args;
    f32 limit = evtGetFloat(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 s;
    f32 c;
    f32 angle = 6.2831855f * (90.0f + camera->viewYaw) / 360.0f;

    s = sin(angle);
    c = cos(angle);

    if (s * x + -c * z < limit) {
        evtSetValue(event, args[3], TRUE);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[3], FALSE);
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(2, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_key)
    SET_VALUE(GSW_STORY_PROGRESS, 146)
    SPAWN_EVENT(PTR(evt_mail))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_POWER_PLUS_P, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tou_01_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -225, 70, -225, 0, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk01"), 245, 100, -220, 0, GSWF(5546), 0)
    CALL(evt_mobj_brick, STRING("hblk01"), 245, 10, -220, 121, MOBJ_BRICK_HIDDEN_ITEM, 0, GSWF(2507))
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 700, 160, -10, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF(2519))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 0, 0, 800, STRING("jimen"), 125, GSWF(5622))
    CALL(evt_item_entry, STRING("item_03"), ITEM_STAR_PIECE, 675, 10, -165, ITEM_TYPE_STILL, GSWF(5618), 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, -785, 10, 390, ITEM_TYPE_STILL, GSWF(5619), 0)
    CALL(evt_item_entry, STRING("item_02"), ITEM_STAR_PIECE, 60, 30, -365, ITEM_TYPE_STILL, GSWF(5620), 0)
    CALL(evt_item_entry, STRING("item_04"), ITEM_STAR_PIECE, 615, 160, -75, ITEM_TYPE_STILL, GSWF(5621), 0)
    CALL(evt_item_entry, STRING("item_05"), ITEM_INN_COUPON, 450, 10, -90, ITEM_TYPE_STILL, GSWF(2533), 0)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_npc_setup, PTR(npcEnt))
        CALL_CHILD(PTR(init_roten_1))
        CALL(evt_npc_change_interrupt, STRING("ロテン1"), 6, PTR(talk_roten_1))
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SECURITY_CODE), 1)
        CALL(evt_item_entry, STRING("iri_24"), ITEM_HOT_SAUCE, -265, 10, -70, ITEM_TYPE_STILL, GSWF(2528), 0)
    END_IF
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_2))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_map_playanim, STRING("anm_kemuri"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_aisu"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_den"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_denden"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_raito"), 1, 0)
    SET_VALUE(LW(0), STRING("sati_1"))
    SPAWN_EVENT(PTR(evt_search_light))
    SET_VALUE(LW(0), STRING("sati_2"))
    SPAWN_EVENT(PTR(evt_search_light))
    SPAWN_EVENT(PTR(evt_kanban))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_telbox"), 0, PTR(evt_telbox), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 145)
        CALL(evt_item_entry, STRING("key"), ITEM_STORAGE_KEY_0x12, -745, 20, 435, ITEM_TYPE_STILL, -1, PTR(evt_key))
    END_IF
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    SPAWN_EVENT(PTR(evt_ship_shadow))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi1"))
    IF_INT_EQUAL(GSWF(2522), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neon_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_sak_kaidan"), 1, 0)
        CALL(evt_mobj_tornadoswitch_blue, 0, STRING("dummy_sw"), 735, 10, 455, PTR(evt_dummy_sw), GSWF(2522))
        SET_VALUE(GF(10), 0)
        SPAWN_EVENT(PTR(evt_kanban2))
        ELSE
        CALL(evt_map_playanim, STRING("anm_kan"), 2, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("neon_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neon_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_sak_kaidan"), 1, 1)
        CALL(evt_hitobj_onoff, STRING("a_hasira"), 1, 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 136)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 15)
            ELSE
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kaunta_"), 0, PTR(evt_hotdog), 0)
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 137)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kaunta_"), 0, PTR(evt_hotdog), 0)
    END_IF
    CALL(evt_hit_bind_mapobj, STRING("a_gondora"), STRING("gondora"))
    CALL(evt_mapobj_trans, 1, STRING("gondora"), 0, 300, 0)
    CALL(evt_hit_bind_update, STRING("a_gondora"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora"), MAP_OBJ_FLAG_HIDDEN)
#ifdef VERSION_G8ME01
    CALL(evt_mobj_jumpstand_blue, 0, STRING("jump"), 180, 610, 160, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_maku5"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_E_gai_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_gai_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_gai_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_gai_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_gai_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_E_gai_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_gai_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_gai_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_gai_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_gai_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_gai_2"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_gai_2"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_gai_2"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_gai_2"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_gai_2"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_maku5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_gai_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_gai_2"), MAP_OBJ_FLAG_DISABLED)
    END_SWITCH
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_hot"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_wel"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_moji"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_saka"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_maku"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_maku5"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_kan_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_E_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_F_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_maku"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_kan_1"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_hot"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_wel"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_moji"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_saka"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_maku5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_kan_1"), MAP_OBJ_FLAG_DISABLED)
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(tou_01_kanban_moji_flag_sync)
    END_INLINE_EVENT
#else
    CALL(evt_mobj_jumpstand_blue, 0, STRING("jump"), 180, 610, 160, 20, 0, 0)
#endif
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 1)
        ELSE
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_NOT_EQUAL(LW(0), STRING("a_door_mon"))
            IF_INT_EQUAL(LF(0), 0)
                SPAWN_EVENT(PTR(evt_gondora))
            END_IF
            ELSE
            CALL(evt_cam_road_reset)
        END_IF
    END_IF
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_WITCH1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 15)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(135)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_EGG1"))
                CALL(evt_snd_envoff, PSND_FADEOUT)
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                SPAWN_EVENT(PTR(evt_tamago))
                SPAWN_EVENT(PTR(evt_tamago_sleep))
                CASE_LESS(136)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_EGG1"))
                CALL(evt_snd_envoff, PSND_FADEOUT)
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                SPAWN_EVENT(PTR(evt_tamago_move))
                SPAWN_EVENT(PTR(evt_tamago_sleep))
                CALL(evt_npc_set_position, STRING("ホットドッグ"), -288, 10, -110)
                CALL(evt_npc_change_interrupt, STRING("ホットドッグ"), 1, PTR(hot_move_evt))
                CALL(evt_npc_restart_regular_event, STRING("ホットドッグ"))
                CASE_LESS(137)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_URN1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU1"))
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                CALL(evt_npc_flag_onoff, 0, STRING("ホットドッグ"), NPC_FLAG_NO_SHADOW)
                CALL(evt_npc_set_position, STRING("たまご"), -400, 110, -120)
                CALL(evt_npc_set_position, STRING("ホットドッグ"), -288, 10, -110)
                CALL(evt_set_dir_to_target, STRING("ホットドッグ"), STRING("たまご"))
                CALL(evt_npc_change_interrupt, STRING("たまご"), 6, PTR(talk_tamago))
                CALL(eff_sleep_on)
                SPAWN_EVENT(PTR(evt_tamago_sleep))
                CASE_DEFAULT
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_URN1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU1"))
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            END_SWITCH
            ELSE
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_URN1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU1"))
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 171)
        CALL(evt_mario_get_party, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_set_party, 0, 1)
            CALL(evt_party_run, 0)
#ifdef VERSION_G8ME01
        END_IF
#endif
            CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
#ifdef VERSION_G8ME01
        CALL(N_evt_sub_status_gauge_force_update)
#else
        END_IF
#endif
        SPAWN_EVENT(PTR(evt_peach_after))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            CALL_CHILD(PTR(sekai_yami_npc_init))
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL_CHILD(PTR(minnnanokoe_npc_entry))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
// US: copy the sign board's map-blend exemptions (MAP_OBJ_FLAG_NO_MAP_BLEND/2) onto the English letter objects
static inline void kanbanCopyFlags(MapObject* kanban, const char* name) {
    MapObject* obj = mapGetMapObj(name);
    if (obj != NULL) {
        obj->flags &= ~(MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2);
        obj->flags |= kanban->flags & (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2);
    }
}

USER_FUNC(tou_01_kanban_moji_flag_sync) {
    MapObject* kanban = mapGetMapObj("kanban");
    if (kanban != NULL) {
        kanbanCopyFlags(kanban, "Sp_moji_3");
        kanbanCopyFlags(kanban, "D_moji_3");
        kanbanCopyFlags(kanban, "J_E_moji_3");
        kanbanCopyFlags(kanban, "I_moji_3");
    }
    return EVT_RETURN_BLOCK;
}
#endif
