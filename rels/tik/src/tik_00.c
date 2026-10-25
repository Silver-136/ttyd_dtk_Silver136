#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_johoya.h"
#include "evt/evt_pouch.h"
#include "driver/mobjdrv.h"

#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "party.h"
#include "pmario_sound.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include "dolphin/gx.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "dolphin/mtx.h"
#include "string.h"
#include "stdio.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "evt/evt_door.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_movefloor.h"
#include "evt/evt_npc.h"
#include "tik.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#ifdef VERSION_G8ME01
#endif

typedef struct UranaiWork {
    s32 lwData[16];
    s32 gwData[32];
    smartEntry* buf;
    GXTexObj tex;
    s32 ready;
    EventEntry* capEvent;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
} UranaiWork;
void uranai_cap(CameraId cameraId, void* param);
static inline f32 tanf(f32 x) { return tan(x); }
const char* search_evt_moji(s32 id);
s32 uranaisi_ryokin_rtn(s32 type);
static s32 ssf;
UranaiWork* uranai_wp;
char* uranaisi_msgp;

USER_FUNC(card_camid);
USER_FUNC(evt_mario_dispflag_onoff);
static USER_FUNC(evt_osreport);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_add_starpiece);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_get_starpiece);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envon_f);
USER_FUNC(evt_uranaisi_msg_alloc);
USER_FUNC(evt_uranaisi_msg_free);
USER_FUNC(evt_uranaisi_msg_set);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_other_select);
static USER_FUNC(scale_func);
static USER_FUNC(setmertn);
static USER_FUNC(ss_set);
USER_FUNC(tik_snd_on);
USER_FUNC(tik_snd_off);
USER_FUNC(supotto_blendmode);
USER_FUNC(uranai_bbox);
USER_FUNC(uranai_cap_evt);
USER_FUNC(uranai_exit_func);
USER_FUNC(uranai_init_func);
USER_FUNC(uranaisi_data_alloc);
USER_FUNC(uranaisi_data_free);
USER_FUNC(uranaisi_next_check);
USER_FUNC(uranaisi_next_makelabel);
USER_FUNC(uranaisi_ryokin);
USER_FUNC(uranaisi_starpiece_check);
USER_FUNC(uranaisi_starpiece_makelabel);
USER_FUNC(uranaisi_starpiece_setreadflag);
USER_FUNC(uranaisi_supercoin_check);
USER_FUNC(uranaisi_supercoin_makelabel);
USER_FUNC(uranaisi_supercoin_setreadflag);
USER_FUNC(uranaisi_table_clear);
USER_FUNC(mario_key_onoff_wait);
USER_FUNC(card_xlu);
void card_tev(MapTevCallbackInfo* info);
static USER_FUNC(ss_check);

static BeroEntry bero_entry_data[4];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
EvtWord uranaisi_next_evt[152];
EvtWord uranaisi_starpiece_evt[60];
EvtWord uranaisi_supercoin_evt[60];
typedef struct CardName { const char* name; s32 value; } CardName;
static CardName name_tbl[9];
EvtWord evt_uranai_cap[4];
EvtWord uranai_init[147];
EvtWord uranai_main[595];
EvtWord uranai_exit[98];
EvtWord uranaisi_table_talk[226];
EvtWord uranaisi_direct_talk[8];
EvtWord uranaisi_init[10];
EvtWord starmaniac_init[14];
EvtWord starmaniac_get_event[5];
EvtWord starmaniac_talk[VERSION_PICK(227, 238)];
EvtWord johoyaketi_talk[VERSION_PICK(186, 209)];
EvtWord kuribo5_init[2];
EvtWord kuribo5_talk[76];
EvtWord chusan4_init[13];
EvtWord chusan4_regl[25];
EvtWord chusan4_talk[76];
EvtWord borodo3_init[2];
EvtWord borodo3_regl[4];
EvtWord borodo3_talk[76];
EvtWord borodo4_init[2];
EvtWord borodo4_talk[76];
EvtWord pinkbom_init[2];
EvtWord pinkbom_talk[76];
EvtWord master_2_init[8];
EvtWord master_2_talk[76];
static NpcSetupInfo npcEnt[11];
static EvtWord evt_door_01_00[18];
EvtWord evt_wall_01_00[62];
EvtWord evt_wall_01_01[66];
EvtWord evt_wall_01_02[66];
static const char* door_data_01_npc[2];
static EvtWord evt_wall_01[12];
EvtWord evt_open_01[39];
EvtWord evt_close_01[60];
static DoorParam door_data_01;
static EvtWord evt_door_02_00[12];
EvtWord evt_wall_02_00[62];
EvtWord evt_wall_02_01[66];
EvtWord evt_wall_02_02[66];
EvtWord evt_wall_02_03[62];
static EvtWord evt_wall_02[14];
static const char* mise_npc[2];
EvtWord evt_open_02[12];
EvtWord evt_close_02[14];
static DoorParam door_data_02;
static EvtWord evt_door_03_00[12];
EvtWord evt_wall_03_00[62];
EvtWord evt_wall_03_01[66];
EvtWord evt_wall_03_02[66];
EvtWord evt_wall_03_03[62];
static EvtWord evt_wall_03[14];
static const char* door_data_03_npc[5];
EvtWord evt_open_03[12];
EvtWord evt_close_03[14];
static const char* door_data_03_mobj[2];
static DoorParam door_data_03;
static EvtWord evt_door_04_00[12];
static EvtWord evt_door_04_01[12];
static EvtWord evt_wall_04[15];
EvtWord evt_open_04_open[179];
static EvtWord evt_door_04_close[87];
static DoorParam door_data_04;
EvtWord evt_door_stg7_open[27];
EvtWord evt_door_stg7_close[7];
static EvtWord evt_door_04_entry[64];
EvtWord tensou_return[243];
EvtWord tensou_init_event[182];
EvtWord tensou_event[209];
EvtWord tensou_anim_event[30];
EvtWord tensou_sw_event[94];
EvtWord kurihakase_event[221];
EvtWord hashira_bomb_sub[144];
EvtWord hashira_bomb[45];
static EvtWord epilogue_npc_entry[25];
static EvtWord epilogue_evt[127];
static EvtWord mobj_box01[34];
EvtWord tik_00_init_evt[214];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_07", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_01", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "tensou", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_00", // nextMapName
        "tensou", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static ShopObj obj_list[] = {
    {"sina_1", "a_sina1"},
    {"sina_2", "a_sina_2"},
    {"sina_3", "a_sina_3"},
    {"sina_4", "a_sina_4"},
    {"sina_5", "a_sina_5"},
    {"sina_6", "a_sina_6"},
};

static ShopItem goods_list[] = {
    {ITEM_SLOW_SHROOM, 15},
    {ITEM_GRADUAL_SYRUP, 15},
    {ITEM_ULTRA_SHROOM, 200},
    {ITEM_JAMMIN_JELLY, 200},
    {ITEM_GOLD_BAR, 110},
    {ITEM_GOLD_BAR_X3, 350},
};

static ShopDb trade_list[] = {
    {ITEM_SLOW_SHROOM, 7, 0},
    {ITEM_GRADUAL_SYRUP, 7, 0},
    {ITEM_ULTRA_SHROOM, 50, 0},
    {ITEM_JAMMIN_JELLY, 50, 0},
    {ITEM_GOLD_BAR, 100, 0},
    {ITEM_GOLD_BAR_X3, 300, 0},
    {ITEM_POISON_SHROOM, 25, 0},
    {ITEM_COURAGE_MEAL, 20, 0},
    {ITEM_COCONUT_BOMB, 8, 0},
    {ITEM_EGG_BOMB, 15, 0},
    {ITEM_ZESS_DYNAMITE, 40, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"tik_shop_00", "tik_shop_01", "tik_shop_02", "tik_shop_03", "tik_shop_04", "tik_shop_05", "tik_shop_06", "tik_shop_07", "tik_shop_08", "tik_shop_09", "tik_shop_10", "tik_shop_11", "tik_shop_12", "tik_shop_13", "tik_shop_14", "tik_shop_15", "tik_shop_16", "tik_shop_17", "tik_shop_18", "tik_shop_19", "tik_shop_20", "tik_shop_21", "tik_shop_22", "tik_shop_23", "tik_shop_24", "tik_shop_25", "tik_shop_26", "tik_shop_27", "tik_shop_28", "tik_shop_29", "shop_30", "shop_31", "shop_32", "shop_33", "shop_34"}, // msgs
};

#ifdef VERSION_G8ME01
// FAKE: "pos" lives here in .rodata (before the uranaisi scripts' strings); copied as a 4-byte struct
typedef struct UranaiTag {
    char s[4];
} UranaiTag;
static const UranaiTag uranaisi_pos_tag = { "pos" };
#endif

EVT_BEGIN(uranaisi_next_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, 405)
        CALL(uranaisi_data_alloc, 0, STRING("un_next"))
        CALL(uranaisi_next_check, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(uranaisi_next_makelabel, 0, LW(1))
            CALL(evt_msg_print, 0, LW(1), 0, STRING("me"))
            ELSE
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_win_coin_on, 0, LW(12))
                CALL(evt_osreport, 0, LW(1))
                CALL(evt_msg_print, 1, STRING("ゆうこうな　データが\n見つかりませんでした\nメッセージを　とうろく　してください\n<key>\n<scrl_auto>\n<once_stop>\n"), 0, STRING("me"))
                CALL(evt_msg_print_add_insert, 1, STRING("げんざいの　イベントばんごうは\n『%s』\nです\n<key>"), LW(1))
                CALL(uranaisi_ryokin, 0, LW(2))
                CALL(evt_pouch_add_coin, LW(2))
                CALL(evt_win_coin_wait, LW(12))
                WAIT_MS(200)
                CALL(evt_win_coin_off, LW(12))
                ELSE
                CALL(evt_win_coin_on, 0, LW(12))
                CALL(evt_osreport, 0, LW(1))
                CALL(evt_msg_print, 1, STRING("ゆうこうな　データが\nじゅうふくして　みつかりました\nゆうこうな　メッセージは　１つにしてください\n<key>\n<scrl_auto>\n<once_stop>"), 0, STRING("me"))
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(3), LW(0))
                LOOP(LW(0))
                    CALL(uranaisi_next_makelabel, LW(2), LW(1))
                    CALL(evt_msg_print_add_insert, 1, STRING("『%s』\n<key>\n<scrl_auto>\n<once_stop>\n"), LW(1))
                    ADD(LW(2), 1)
                END_LOOP
                CALL(evt_msg_print_add_insert, 1, STRING("いじょう　%dこが\nじゅうふくしている　メッセージです\n<key>"), LW(3))
                CALL(uranaisi_ryokin, 0, LW(2))
                CALL(evt_pouch_add_coin, LW(2))
                CALL(evt_win_coin_wait, LW(12))
                WAIT_MS(200)
                CALL(evt_win_coin_off, LW(12))
            END_IF
        END_IF
        CALL(uranaisi_data_free)
        ELSE
        CALL(evt_msg_print, 0, STRING("uranaisi_17"), 0, STRING("me"))
        CALL(evt_win_coin_on, 0, LW(12))
        WAIT_MS(500)
        CALL(uranaisi_ryokin, 0, LW(2))
        CALL(evt_pouch_add_coin, LW(2))
        CALL(evt_win_coin_wait, LW(12))
        WAIT_MS(500)
        CALL(evt_win_coin_off, LW(12))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(uranaisi_starpiece_evt)
    CALL(uranaisi_data_alloc, 1, STRING("un_starpiece"))
    CALL(uranaisi_starpiece_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_on, 0, LW(12))
        CALL(evt_msg_print, 0, STRING("uranaisi_14"), 0, STRING("me"))
        CALL(uranaisi_ryokin, 1, LW(2))
        CALL(evt_pouch_add_coin, LW(2))
        CALL(evt_win_coin_wait, LW(12))
        WAIT_MS(200)
        CALL(evt_win_coin_off, LW(12))
        ELSE
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(0))
        CALL(uranaisi_starpiece_makelabel, LW(0), LW(0))
        CALL(evt_msg_print, 0, LW(0), 0, STRING("me"))
        CALL(uranaisi_starpiece_setreadflag)
    END_IF
    CALL(uranaisi_data_free)
    RETURN
EVT_END()

EVT_BEGIN(uranaisi_supercoin_evt)
    CALL(uranaisi_data_alloc, 2, STRING("un_supercoin"))
    CALL(uranaisi_supercoin_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_on, 0, LW(12))
        CALL(evt_msg_print, 0, STRING("uranaisi_16"), 0, STRING("me"))
        CALL(uranaisi_ryokin, 2, LW(2))
        CALL(evt_pouch_add_coin, LW(2))
        CALL(evt_win_coin_wait, LW(12))
        WAIT_MS(200)
        CALL(evt_win_coin_off, LW(12))
        ELSE
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(0))
        CALL(uranaisi_supercoin_makelabel, LW(0), LW(0))
        CALL(evt_msg_print, 0, LW(0), 0, STRING("me"))
        CALL(uranaisi_supercoin_setreadflag)
    END_IF
    CALL(uranaisi_data_free)
    RETURN
EVT_END()

static CardName name_tbl[] = {
    {"card_1", 0},
    {"card_2", 1},
    {"card_3", 2},
    {"card_4", 3},
    {"card_5", 1},
    {"card_6", 0},
    {"card_7", 3},
    {"card_8", 2},
    {"card_9", 1},
};

EVT_BEGIN(evt_uranai_cap)
    CALL(uranai_cap_evt)
    RETURN
EVT_END()

static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

static USER_FUNC(ss_check) {
    s32 a;
    s32 z;

    // FAKE: the dead condition moves the ssf load ahead of event->args; casts, temps and
    // u32 index variants do not reproduce it
    a = event->args[0];
    z = !ssf;
    if ((z && !ssf) != 0) {
    }
    evtSetFloat(event, a, ssf);
    return EVT_RETURN_DONE;
}

static USER_FUNC(ss_set) {
    ssf = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_uranaisi_msg_free) {
    if (uranaisi_msgp == NULL) {
        return EVT_RETURN_DONE;
    }
    _mapFree(uranaisi_msgp);
    uranaisi_msgp = NULL;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_uranaisi_msg_alloc) {
    uranaisi_msgp = _mapAlloc(0x100);
    memset(uranaisi_msgp, 0, 0x100);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_uranaisi_msg_set) {
    EvtWord* args = event->args;
    const char* msg = msgSearch((const char*)evtGetValue(event, args[0]));
#ifdef VERSION_G8ME01
    UranaiTag tag = uranaisi_pos_tag;
    char* t = tag.s; // FAKE: reading the tag through a pointer keeps tag[0] from being hoisted out of the loop
    char num[20];
    s32 ofs[10];
    char buf[256];
    s32 i;
    s32 j;
    s32 state;
    s32 count;
    s32 k;
    s32 sign;
    s32 c;
    s32 d;

    state = 0;
    count = 0;
    i = 0;
    j = 0;
    while ((s8)(c = msg[i]) != 0) {
        if ((s8)c == '<') {
            state = 1;
        } else if ((s8)c == '>') {
            state = 0;
        }
        if (state != 0) {
            if (state == 1 && (s8)c != ' ') {
                state = 2;
            }
            if (state == 2 && (s8)t[0] == (s8)c) {
                for (k = 1; (s8)(d = t[k]) != 0; k++) {
                    if ((s8)d != (s8)msg[i + k]) {
                        state = 3;
                    }
                }
                if (state == 2) {
                    if ((count & 1) == 0) {
                        buf[j] = c;
                        i++;
                        j++;
                        while ((s8)(c = msg[i]) != ' ') {
                            buf[j] = c;
                            i++;
                            j++;
                        }
                        while ((s8)(c = msg[i]) == ' ') {
                            buf[j] = c;
                            i++;
                            j++;
                        }
                        ofs[count / 2] = 0;
                        sign = 1;
                        while ((s8)(c = msg[i]) != ' ') {
                            c = (s8)c;
                            if (c == '-') {
                                sign = -1;
                            } else if (c >= '0' && c <= '9') {
                                ofs[count / 2] = c + ofs[count / 2] * 10 - '0';
                            }
                            i++;
                        }
                        ofs[count / 2] = sign * ofs[count / 2];
                        buf[j++] = '%';
                        buf[j++] = 'd';
                    }
                    count++;
                }
            } else if (state == 2 && (s8)c == ' ') {
                state = 3;
            }
        }
        buf[j] = msg[i];
        i++;
        j++;
    }
    buf[j] = 0;
    for (k = 0; k < count / 2; k++) {
        switch (k) {
            case 0:
                sprintf(num, "%3d", uranaisi_ryokin_rtn(k));
                break;
            default:
                sprintf(num, "%d", uranaisi_ryokin_rtn(k));
                break;
        }
        ofs[k] -= FontGetMessageWidth(num);
    }
    sprintf(uranaisi_msgp, buf, ofs[0], uranaisi_ryokin_rtn(0), ofs[1], uranaisi_ryokin_rtn(1), ofs[2], uranaisi_ryokin_rtn(2));
#else

    sprintf(uranaisi_msgp, msg, uranaisi_ryokin_rtn(0), uranaisi_ryokin_rtn(1), uranaisi_ryokin_rtn(2));
#endif
    evtSetValue(event, args[1], (EvtWord)uranaisi_msgp);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_osreport) {
    EvtWord* args = event->args;

    switch (evtGetValue(event, args[0])) {
        case 0:
            evtSetValue(event, args[1], (EvtWord)search_evt_moji(evtGetValue(event, GSW_STORY_PROGRESS)));
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(uranai_init_func) {
    char name[64];
    s32 i;

    uranai_wp = _mapAlloc(sizeof(UranaiWork));
    memset(uranai_wp, 0, sizeof(UranaiWork));
    memcpy(uranai_wp->lwData, event->lwData, sizeof(uranai_wp->lwData));
    memcpy(uranai_wp->gwData, evtGetWork()->gwData, sizeof(uranai_wp->gwData));
    for (i = 0; i < 9; i++) {
        sprintf(name, "card_omote_%d", i + 1);
        if (mapGetMapObj(name) != NULL) {
            mapGetMapObj(name)->cameraId = 5;
        }
        sprintf(name, "card_ura_%d", i + 1);
        if (mapGetMapObj(name) != NULL) {
            mapGetMapObj(name)->cameraId = 5;
        }
    }
    uranai_wp->capEvent = evtEntry(evt_uranai_cap, 0, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(uranai_exit_func) {
    memcpy(event->lwData, uranai_wp->lwData, sizeof(uranai_wp->lwData));
    memcpy(evtGetWork()->gwData, uranai_wp->gwData, sizeof(uranai_wp->gwData));
    evtDelete(uranai_wp->capEvent);
    _mapFree(uranai_wp);
    return EVT_RETURN_DONE;
}

void uranai_cap(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 half;
    u16 w = uranai_wp->w;
    u16 h = uranai_wp->h;
    f32 viewport[6];
    f32 proj[7];
    u32 sx, sy, sw, sh;
    Mtx44 projMtx;
    Mtx lookAt;
    Mtx ident;
    CameraEntry saved;
    NpcEntry* npc;
    AnimationPoseData* data;
    Vec bmin;
    Vec bmax;
    f32 scale;

    GXGetViewportv(viewport);
    GXGetProjectionv(proj);
    GXGetScissor(&sx, &sy, &sw, &sh);
    GXSetViewport(0.0f, 0.0f, w, h, 0.0f, 1.0f);
    GXSetScissor(1, 1, w - 2, h - 2);
    C_MTXPerspective(projMtx, 25.0f, w / h, 1.0f, 1000.0f);
    GXSetProjection(projMtx, GX_PERSPECTIVE);
    {
        static const Vec uranai_pos = {0.0f, 0.0f, 0.0f};
        static const Vec uranai_up = {0.0f, 1.0f, 0.0f};
        static const Vec uranai_target = {0.0f, 0.0f, 0.0f};
        Vec pos;
        Vec up;
        Vec target;

        target = uranai_target;
        target.y = h / 3.0f;
        up = uranai_up;
        pos = uranai_pos;
        half = 0.5f;
        pos.y = h / 3.0f;
        {
            f32 t = tan(0.21816616f);

            pos.z = 0.7f * ((h * half) / t);
        }
        C_MTXLookAt(lookAt, &pos, &up, &target);
    }
    saved = *camGetPtr(cameraId);
    npc = npcNameToPtr("\202\244\202\347\202\310\202\242\216t");
    data = animPoseGetAnimBaseDataPtr(npc->poseId);
    bmin = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    bmax = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    scale = w / (2.0f * (bmax.x - bmin.x));
    PSMTXCopy(lookAt, camGetCurPtr()->view);
    animPosePeraOff(npc->poseId);
    PSMTXIdentity(ident);
    animPoseDrawMtx(npc->poseId, ident, 1, 0.0f, scale);
    animPoseDrawMtx(npc->poseId, ident, 2, 0.0f, scale);
    animPoseDrawMtx(npc->poseId, ident, 3, 0.0f, scale);
    animPosePeraOn(npc->poseId);
    PSMTXCopy(saved.view, camGetCurPtr()->view);
    uranai_wp->buf = smartAlloc(GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0), 3);
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, GX_TF_RGB5A3, GX_FALSE);
    GXCopyTex(uranai_wp->buf->address, GX_TRUE);
    GXPixModeSync();
    GXInitTexObj(&uranai_wp->tex, uranai_wp->buf->address, w, h, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&uranai_wp->tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(proj);
    GXSetScissor(sx, sy, sw, sh);
    uranai_wp->ready = 1;
}

USER_FUNC(uranai_cap_evt) {
    dispEntry(CAMERA_SHADOW, 0.0f, 3, uranai_cap, NULL);
    return EVT_RETURN_BLOCK;
}

void card_tev(MapTevCallbackInfo* info) {
    if (uranai_wp->ready) {
        GXLoadTexObj(smartTexObj(&uranai_wp->tex, &uranai_wp->buf->address), GX_TEXMAP0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        info->tevStage = 1;
        info->texCoord = 1;
        info->texMap = 1;
    }
}

USER_FUNC(uranai_bbox) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 w = evtGetValue(event, args[2]);
    s32 h = evtGetValue(event, args[3]);

    uranai_wp->x = x;
    uranai_wp->y = x;
    uranai_wp->w = x;
    uranai_wp->h = x;
    return EVT_RETURN_DONE;
}

USER_FUNC(card_xlu) {
    const char* name = (const char*)evtGetValue(event, LW(2));

    if (isFirstCall) {
        event->userdata[0] = 255;
    }
    mapGrpSetColor(name, (GXColor){0xFF, 0xFF, 0xFF, event->userdata[0]});
    event->userdata[0] -= 8;
    if (event->userdata[0] < 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(uranai_init)
    CALL(uranai_init_func)
    SET_VALUE(GW(15), LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("supotto"), 1)
    CALL(supotto_blendmode)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("uranai_mae"), 6144)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_color, 1, STRING("uranai_mae"), LW(0), LW(0), LW(0), 255)
        CALL(evt_mapobj_color, 1, STRING("ie_1_door"), LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("uranai_mae"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ie_1_door"), 1)
    CALL(evt_mapobj_trans, 1, STRING("card"), 0, 0, 0)
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(9)
        READ_INT2(LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 6144)
        CALL(evt_mapobj_trans, 1, LW(0), 0, -1000, 0)
    END_LOOP
    CALL(evt_map_set_tevcallback, 1, PTR(card_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("card_omote_5"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("card_omote_5"), 256)
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_npc_set_offscreen, STRING("うらない師"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    CALL(uranai_bbox, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_npc_set_offscreen, STRING("うらない師"), 0)
    RETURN
EVT_END()

EVT_BEGIN(uranai_main)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_snd_sfxon, STRING("SFX_EVT_FORTUNE_CARD_MOVE1"), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -1300, 1300, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("card_1"), 0, -200, 0)
        CALL(evt_mapobj_rotate, 1, STRING("card_1"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("card_1"), 0, 180, 0)
        IF_FLOAT_GREATER(LW(0), FLOAT(-20.0f))
            CALL(evt_npc_flag_onoff, 1, STRING("うらない師"), NPC_FLAG_NO_DRAW)
        END_IF
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(9)
        READ_INT2(LW(0), LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(0)
            SET_VALUE(LW(2), 500)
            SET_VALUE(LF(0), 0)
            CASE_EQUAL(1)
            SET_VALUE(LW(2), -500)
            SET_VALUE(LF(0), 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(2), 500)
            SET_VALUE(LF(0), 1)
            CASE_EQUAL(3)
            SET_VALUE(LW(2), -500)
            SET_VALUE(LF(0), 1)
        END_SWITCH
        BEGIN_INLINE_EVENT
            WAIT_MS(100)
            CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_FORTUNE_CARD_MOVE2"), LW(10), LW(11), LW(12), 0)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(9), LW(0))
            MULTIPLY_FLOAT(LW(2), FLOAT(10.0f))
            CALL(evt_sub_intpl_msec_init, 11, LW(2), 0, 360)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_mapobj_trans, 1, LW(9), LW(8), 0, LW(0))
                    ELSE
                    CALL(evt_mapobj_trans, 1, LW(9), LW(8), LW(0), 0)
                END_IF
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_INLINE_EVENT
        ADD_FLOAT(LW(8), FLOAT(0.19921875f))
        WAIT_MS(300)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_FORTUNE_CARD_MOVE3"), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 600, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
        SET_INT_READ_ADDR(PTR(&name_tbl))
        LOOP(9)
            READ_INT2(LW(2), LW(3))
            CALL(evt_mapobj_trans, 1, LW(2), LW(8), -100, 0)
            CALL(evt_mapobj_rotate, 1, LW(2), LW(0), 0, 0)
            CALL(evt_mapobj_trans, 1, LW(2), LW(8), 100, 0)
            ADD_FLOAT(LW(8), FLOAT(0.19921875f))
        END_LOOP
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 11, 600, -600, 1500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(9), FLOAT(0.0f))
        SET_INT_READ_ADDR(PTR(&name_tbl))
        LOOP(9)
            READ_INT2(LW(2), LW(3))
            SET_FLOAT_VALUE(LW(6), FLOAT(60.0f))
            SET_FLOAT_VALUE(LW(7), LW(9))
            MULTIPLY_FLOAT(LW(7), FLOAT(15.0f))
            SUBTRACT_FLOAT(LW(6), LW(7))
            CALL(evt_mapobj_trans, 1, LW(2), LW(8), -100, 0)
            IF_FLOAT_LESS(LW(0), LW(6))
                CALL(evt_mapobj_rotate, 1, LW(2), LW(6), 0, 0)
                ELSE
                CALL(evt_mapobj_rotate, 1, LW(2), LW(0), 0, 0)
            END_IF
            CALL(evt_mapobj_trans, 1, LW(2), LW(8), 100, 0)
            ADD_FLOAT(LW(8), FLOAT(0.19921875f))
            ADD_FLOAT(LW(9), FLOAT(1.0f))
        END_LOOP
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(9), FLOAT(0.0f))
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(9)
        READ_INT2(LW(2), LW(3))
        SET_FLOAT_VALUE(LW(6), FLOAT(60.0f))
        SET_FLOAT_VALUE(LW(7), LW(9))
        MULTIPLY_FLOAT(LW(7), FLOAT(15.0f))
        SUBTRACT_FLOAT(LW(6), LW(7))
        BEGIN_INLINE_EVENT
            IF_INT_EQUAL(LW(9), 4)
                CALL(evt_snd_sfxon, STRING("SFX_EVT_FORTUNE_CARD_MOVE4"), 0)
                CALL(evt_sub_intpl_msec_init, 0, 0, 500, 1000)
                ELSE
                CALL(evt_sub_intpl_msec_init, 1, 0, -3000, 1000)
            END_IF
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
                CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
                CALL(evt_mapobj_trans, 1, LW(2), LW(8), -100, 0)
                CALL(evt_mapobj_rotate, 1, LW(2), LW(6), 0, 0)
                CALL(evt_mapobj_trans, 1, LW(2), LW(8), 100, 0)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_INLINE_EVENT
        ADD_FLOAT(LW(8), FLOAT(0.19921875f))
        ADD_FLOAT(LW(9), FLOAT(1.0f))
    END_LOOP
    WAIT_MS(1200)
    CALL(card_camid, STRING("card_omote_5"), 4)
    CALL(card_camid, STRING("card_ura_5"), 4)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(50.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(3), FLOAT(0.0f))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_FORTUNE_CARD_MOVE5"), 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("card_5"), GW(0), GW(1), GW(2))
            CALL(evt_mapobj_rotate, 1, STRING("card_5"), 0, GW(3), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 13, 500, 700, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(1), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, 12, 700, -200, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(1), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, -450, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(2), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 37800, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(3), LW(0))
            IF_FLOAT_GREATER(GW(3), FLOAT(3600.0f))
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("card_omote_5"), 1)
                CALL(evt_npc_flag_onoff, 0, STRING("うらない師"), NPC_FLAG_NO_DRAW)
            END_IF
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2500)
    DELETE_EVENT(LW(9))
    SET_VALUE(LW(0), GW(15))
    RETURN
EVT_END()

EVT_BEGIN(uranai_exit)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("uranai_mae"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_1_door"), 1)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_color, 1, STRING("uranai_mae"), LW(0), LW(0), LW(0), 255)
        CALL(evt_mapobj_color, 1, STRING("ie_1_door"), LW(0), LW(0), LW(0), 255)
        SET_VALUE(LW(2), 255)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_color, 1, STRING("supotto"), 255, 255, 255, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("supotto"), 1)
    CALL(evt_mapobj_color, 1, STRING("supotto"), 255, 255, 255, 255)
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(9)
        READ_INT2(LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 6144)
    END_LOOP
    CALL(uranai_exit_func)
    RETURN
EVT_END()

EVT_BEGIN(uranaisi_table_talk)
    CALL(setmertn, STRING("うらない師"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_set_ry, STRING("me"), 180)
    LOOP(0)
        IF_INT_EQUAL(GSWF(6), 0)
            IF_INT_EQUAL(GSWF(5), 0)
                CALL(evt_msg_print, 0, STRING("uranaisi_00"), 0, STRING("me"))
                SET_VALUE(GSWF(5), 1)
                SET_VALUE(GSWF(237), 1)
                ELSE
                IF_INT_EQUAL(GSWF(237), 0)
                    CALL(evt_msg_print, 0, STRING("uranaisi_01"), 0, STRING("me"))
                    SET_VALUE(GSWF(237), 1)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("uranaisi_19"), 0, STRING("me"))
                END_IF
            END_IF
            CALL(evt_msg_select, 0, STRING("uranaisi_03"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("uranaisi_06"))
                GOTO(0)
            END_IF
            WAIT_MS(200)
            ELSE
            CALL(evt_msg_print, 0, STRING("uranaisi_02"), 0, STRING("me"))
        END_IF
        SET_VALUE(GSWF(6), 1)
        CALL(evt_win_coin_on, 0, LW(12))
        CALL(evt_uranaisi_msg_alloc)
        CALL(evt_uranaisi_msg_set, STRING("uranaisi_04"), LW(1))
        CALL(evt_msg_select, 1, LW(1))
        CALL(evt_uranaisi_msg_free)
        IF_INT_EQUAL(LW(0), 4)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print_add, 0, STRING("uranaisi_06"))
            GOTO(0)
        END_IF
        CALL(uranaisi_ryokin, LW(0), LW(2))
        CALL(evt_pouch_get_coin, LW(1))
        IF_INT_LESS(LW(1), LW(2))
            CALL(evt_msg_print_add, 0, STRING("uranaisi_07"))
            CALL(evt_win_coin_off, LW(12))
            GOTO(0)
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("uranaisi_08"))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("uranaisi_09"))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING("uranaisi_11"))
        END_SWITCH
        MULTIPLY(LW(2), -1)
        CALL(evt_pouch_add_coin, LW(2))
        CALL(evt_win_coin_wait, LW(12))
        WAIT_MS(200)
        CALL(evt_win_coin_off, LW(12))
        CALL_CHILD(PTR(uranai_init))
        CALL(evt_msg_print, 0, STRING("uranaisi_12"), 0, STRING("me"))
        BEGIN_INLINE_EVENT
            CALL(evt_npc_set_anim, STRING("me"), STRING("I_1"))
            WAIT_MS(240)
            CALL(evt_npc_set_anim, STRING("me"), STRING("S_2"))
        END_INLINE_EVENT
        CALL_CHILD(PTR(uranai_main))
        CALL(evt_npc_set_anim, STRING("me"), STRING("D_1"))
        WAIT_MS(170)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL_CHILD(PTR(uranaisi_next_evt))
            CASE_EQUAL(1)
            CALL_CHILD(PTR(uranaisi_starpiece_evt))
            CASE_EQUAL(2)
            CALL_CHILD(PTR(uranaisi_supercoin_evt))
        END_SWITCH
        CALL_CHILD(PTR(uranai_exit))
    END_LOOP
    LABEL(0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(uranaisi_direct_talk)
    CALL(evt_msg_print, 0, STRING("uranaisi_18"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(uranaisi_init)
    CALL(evt_run_case_evt, 9, 1, STRING("uranai"), 0, PTR(uranaisi_table_talk), 0)
    RETURN
EVT_END()

EVT_BEGIN(starmaniac_init)
    CALL_CHILD(PTR(npc_init_evt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 20)
        CALL(evt_npc_set_position, STRING("me"), 400, -20, 175)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(starmaniac_get_event)
    CALL(ss_set, 1)
    RETURN
EVT_END()

EVT_BEGIN(starmaniac_talk)
    CALL(evt_badgeShop_starmaniac_get_kind_cnt, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("starmaniac_02"), 0, STRING("me"))
        RETURN
    END_IF
    CALL(tik_snd_off)
    IF_INT_EQUAL(GSWF(1325), 0)
        CALL(evt_msg_print, 0, STRING("starmaniac_00"), 0, STRING("me"))
        SET_VALUE(GSWF(1325), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("starmaniac_01"), 0, STRING("me"))
    END_IF
    CALL(evt_msg_select, 0, STRING("starmaniac_03"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("starmaniac_05"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_pouch_get_starpiece, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("starmaniac_06"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("starmaniac_07"))
    LABEL(0)
    CALL(evt_win_coin_on, 2, LW(12))
    CALL(evt_win_other_select, 13)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("starmaniac_08"), 0, STRING("me"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("starmaniac_09"), 0, STRING("me"), LW(2), LW(3), LW(4))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("starmaniac_09"), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("me"), LW(4))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_insert, 0, STRING("starmaniac_09"), 0, STRING("me"), LW(3), LW(2), LW(4))
        ELSE
        CALL(evt_msg_print_insert, 0, STRING("starmaniac_09"), 0, STRING("me"), LW(2), LW(3), LW(4))
    END_IF
#endif
    CALL(evt_msg_select, 0, STRING("starmaniac_10"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("starmaniac_11"))
        GOTO(0)
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("starmaniac_15"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_badgeShop_starmaniac_dec, LW(1))
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_starpiece, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    CALL(evt_badgeShop_starmaniac_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("starmaniac_12"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_pouch_get_starpiece, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("starmaniac_12"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("starmaniac_13"))
    CALL(evt_msg_select, 0, STRING("starmaniac_10"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("starmaniac_14"))
        CALL(tik_snd_on)
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    CALL(tik_snd_on)
    RETURN
EVT_END()

EVT_BEGIN(johoyaketi_talk)
    IF_INT_EQUAL(GSWF(3), 0)
        CALL(evt_msg_print, 0, STRING("johoyaketi_00"), 0, STRING("me"))
        SET_VALUE(GSWF(3), 1)
        RETURN
    END_IF
    CALL(johoya_keti_newjoho_check, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(johoya_keti_oldspeak_check, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("johoyaketi_08"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("johoyaketi_06"), 0, STRING("me"))
            SUBTRACT(LW(0), 1)
            CALL(evt_sub_random, LW(0), LW(0))
            CALL(johoya_keti_oldspeak_makelabel, LW(0), LW(0), LW(3), LW(4), LW(5))
            CALL(evt_msg_print_add, 0, LW(0))
            CALL(johoya_keti_oldspeak_setreadflag)
            CALL(evt_msg_print_add, 0, STRING("johoyaketi_07"))
        END_IF
        ELSE
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        CALL(johoya_keti_newjoho_makelabel, LW(0), LW(2), LW(3), LW(4), LW(5))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("johoyaketi_01"), 0, STRING("me"), LW(5), LW(4), LW(3))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("johoyaketi_01"), LW(5))
            CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("me"), LW(4), LW(3))
        END_SWITCH
#else
        CALL(evt_msg_print_insert, 0, STRING("johoyaketi_01"), 0, STRING("me"), LW(5), LW(4), LW(3))
#endif
        CALL(evt_win_coin_on, 0, LW(12))
        CALL(evt_msg_select, 0, STRING("johoyaketi_02"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_pouch_get_coin, LW(0))
            IF_INT_LESS(LW(0), LW(5))
                CALL(evt_msg_print_add, 0, STRING("johoyaketi_09"))
                CALL(evt_win_coin_off, LW(12))
                RETURN
            END_IF
            MULTIPLY(LW(5), -1)
            CALL(evt_pouch_add_coin, LW(5))
            CALL(evt_win_coin_wait, LW(12))
            WAIT_MS(500)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print_add, 0, LW(2))
            CALL(johoya_keti_newjoho_setreadflag)
            CALL(evt_msg_print_add, 0, STRING("johoyaketi_07"))
            ELSE
            CALL(evt_win_coin_off, LW(12))
            CALL(johoya_keti_oldspeak_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("johoyaketi_04"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("johoyaketi_05"))
                SUBTRACT(LW(0), 1)
                CALL(evt_sub_random, LW(0), LW(0))
                CALL(johoya_keti_oldspeak_makelabel, LW(0), LW(0), LW(3), LW(4), LW(5))
                CALL(evt_msg_print_add, 0, LW(0))
                CALL(johoya_keti_oldspeak_setreadflag)
                CALL(evt_msg_print_add, 0, STRING("johoyaketi_07"))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kuribo5_init)
    RETURN
EVT_END()

EVT_BEGIN(kuribo5_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_001_01"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_001_02"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_001_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_001_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_001_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_001_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_001_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_001_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_001_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(chusan4_init)
    CALL(evt_npc_set_position, STRING("me"), -175, -19, 235)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(chusan4_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), 175, 235, 0, FLOAT(40.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("me"), -175, 235, 0, FLOAT(40.0f), 0)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(chusan4_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_002_01"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_002_02"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_002_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_002_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_002_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_002_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_002_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_002_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_002_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borodo3_init)
    RETURN
EVT_END()

EVT_BEGIN(borodo3_regl)
    CALL_CHILD(PTR(tik_villager_urouro))
    RETURN
EVT_END()

EVT_BEGIN(borodo3_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_003_01"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_003_02"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_003_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_003_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_003_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_003_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_003_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_003_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_003_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borodo4_init)
    RETURN
EVT_END()

EVT_BEGIN(borodo4_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_004_01"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_004_02"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_004_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_004_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_004_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_004_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_004_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_004_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_004_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(pinkbom_init)
    RETURN
EVT_END()

EVT_BEGIN(pinkbom_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_005_01"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_005_02"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_005_03"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_005_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_005_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_005_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_005_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_005_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_005_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(master_2_init)
    CALL(evt_npc_set_position, STRING("me"), -85, 95, -535)
    RETURN
EVT_END()

EVT_BEGIN(master_2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("tik_005_10"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("tik_005_11"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("tik_005_12"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("tik_005_13"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("tik_005_14"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("tik_005_15"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("tik_005_16"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("tik_005_17"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("tik_005_18"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"うらない師"*/"\x82\xA4\x82\xE7\x82\xC8\x82\xA2\x8E\x74", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        uranaisi_init, // initEvent
        NULL, // regularEvent
        uranaisi_direct_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"星マニア"*/"\x90\xAF\x83\x7D\x83\x6A\x83\x41", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        starmaniac_init, // initEvent
        NULL, // regularEvent
        starmaniac_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"情報屋ケチ"*/"\x8F\xEE\x95\xF1\x89\xAE\x83\x50\x83\x60", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        johoyaketi_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー５"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x54", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kuribo5_init, // initEvent
        NULL, // regularEvent
        kuribo5_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チューさん４"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x53", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        chusan4_init, // initEvent
        chusan4_regl, // regularEvent
        chusan4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボロドー３"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        borodo3_init, // initEvent
        borodo3_regl, // regularEvent
        borodo3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボロドー４"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x82\x53", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        borodo4_init, // initEvent
        NULL, // regularEvent
        borodo4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ピンクボム兵"*/"\x83\x73\x83\x93\x83\x4E\x83\x7B\x83\x80\x95\xBA", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        pinkbom_init, // initEvent
        NULL, // regularEvent
        pinkbom_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マスター２"*/"\x83\x7D\x83\x58\x83\x5E\x81\x5B\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        master_2_init, // initEvent
        NULL, // regularEvent
        master_2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_door_01_00)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("supotto"), 1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_1_door"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_01_00)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            ELSE
            WAIT_MS(300)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_1_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_1_1"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_01_01)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(100)
            ELSE
            WAIT_MS(200)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_1_2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_1_2"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_01_02)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(200)
            ELSE
            WAIT_MS(100)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_1_3"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_1_3"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static const char* door_data_01_npc[] = {
    /*"うらない師"*/"\x82\xA4\x82\xE7\x82\xC8\x82\xA2\x8E\x74",
    NULL,
};

static EVT_BEGIN(evt_wall_01)
    IF_INT_EQUAL(LW(6), 0)
        SPAWN_EVENT(PTR(evt_wall_01_00))
        SPAWN_EVENT(PTR(evt_wall_01_01))
        SPAWN_EVENT(PTR(evt_wall_01_02))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_open_01)
    CALL(evt_mapobj_trans, 1, STRING("card"), 0, -1000, 0)
    CALL(evt_mapobj_trans, 1, STRING("ie_1_naka"), 0, FLOAT(0.19921875f), 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mobj_check, STRING("pblk1"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("pblk1"), MOBJ_FLAG_NO_SHADOW)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_close_01)
    CALL(evt_mapobj_trans, 1, STRING("card"), 0, -1000, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("mon"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kuro_maku"), 1)
    CALL(evt_door_end_wait)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("mon"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kuro_maku"), 1)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    CALL(evt_mobj_check, STRING("pblk1"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("pblk1"), MOBJ_FLAG_NO_SHADOW)
    END_IF
    RETURN
EVT_END()

static DoorParam door_data_01 = {
    2, // flags
    3, // type
    1, // se
    "ie_1_door", // mapobj
    "a_door_1_N", // hitIn
    "a_door_1", // hitOut
    "ie_1_soto", // outside
    NULL, // outsideHit
    "ie_1_naka", // inside
    NULL, // insideHit
    evt_door_01_00, // evtIn
    NULL, // evtOut
    evt_wall_01, // evtWall
    door_data_01_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_open_01, // evtOpen
    evt_close_01, // evtClose
};

static EVT_BEGIN(evt_door_02_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_2_door"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_00)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            ELSE
            WAIT_MS(300)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_2_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_2_1"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_01)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(100)
            ELSE
            WAIT_MS(200)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_2_2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_2_2"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_02)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(200)
            ELSE
            WAIT_MS(100)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_2_3"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_2_3"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_03)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(300)
            ELSE
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_2_4"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_2_4"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_02)
    IF_INT_EQUAL(LW(6), 0)
        SPAWN_EVENT(PTR(evt_wall_02_00))
        SPAWN_EVENT(PTR(evt_wall_02_01))
        SPAWN_EVENT(PTR(evt_wall_02_02))
        SPAWN_EVENT(PTR(evt_wall_02_03))
    END_IF
    RETURN
EVT_END()

static const char* mise_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(evt_open_02)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_close_02)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_02 = {
    2, // flags
    3, // type
    1, // se
    "ie_2_door", // mapobj
    "a_door_2_N", // hitIn
    "a_door_2", // hitOut
    "ie_2_soto", // outside
    NULL, // outsideHit
    "ie_2_naka", // inside
    NULL, // insideHit
    evt_door_02_00, // evtIn
    NULL, // evtOut
    evt_wall_02, // evtWall
    mise_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_open_02, // evtOpen
    evt_close_02, // evtClose
};

static EVT_BEGIN(evt_door_03_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_3_door"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_00)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            ELSE
            WAIT_MS(300)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_3_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_3_1"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_01)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(100)
            ELSE
            WAIT_MS(200)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_3_2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_3_2"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_02)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(200)
            ELSE
            WAIT_MS(100)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_3_3"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(200)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_3_3"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_03)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 0)
            WAIT_MS(300)
            ELSE
        END_IF
        CALL(evt_mapobj_get_position, STRING("ie_3_4"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HOUSE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(1), 0)
        WAIT_MS(300)
        CALL(evt_sub_intpl_msec_init, 2, 0, 180, 500)
        ELSE
        CALL(evt_sub_intpl_msec_init, 2, 180, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("ie_3_4"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_03)
    IF_INT_EQUAL(LW(6), 0)
        SPAWN_EVENT(PTR(evt_wall_03_00))
        SPAWN_EVENT(PTR(evt_wall_03_01))
        SPAWN_EVENT(PTR(evt_wall_03_02))
        SPAWN_EVENT(PTR(evt_wall_03_03))
    END_IF
    RETURN
EVT_END()

static const char* door_data_03_npc[] = {
    /*"情報屋ケチ"*/"\x8F\xEE\x95\xF1\x89\xAE\x83\x50\x83\x60",
    /*"ボロドー４"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x82\x53",
    /*"ピンクボム兵"*/"\x83\x73\x83\x93\x83\x4E\x83\x7B\x83\x80\x95\xBA",
    /*"マスター２"*/"\x83\x7D\x83\x58\x83\x5E\x81\x5B\x82\x51",
    NULL,
};

EVT_BEGIN(evt_open_03)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_close_03)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static const char* door_data_03_mobj[] = {
    "kururin1",
    NULL,
};

static DoorParam door_data_03 = {
    2, // flags
    3, // type
    1, // se
    "ie_3_door", // mapobj
    "a_door_3_N", // hitIn
    "a_door_3", // hitOut
    "ie_3_soto", // outside
    NULL, // outsideHit
    "ie_3_naka", // inside
    NULL, // insideHit
    evt_door_03_00, // evtIn
    NULL, // evtOut
    evt_wall_03, // evtWall
    door_data_03_npc, // npcs
    door_data_03_mobj, // mobjs
    NULL, // items
    evt_open_03, // evtOpen
    evt_close_03, // evtClose
};

static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_4door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_4door"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_04)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_4_1_"), 0, LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_open_04_open)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(377)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_4_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ie_4_2"), 1)
        IF_INT_EQUAL(GW(10), 0)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("wa_pu"), 1)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
        END_IF
        CASE_LESS(405)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ie_4_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_4_2"), 1)
        IF_INT_EQUAL(GW(10), 0)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("wa_pu"), 1)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
        END_IF
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_4_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ie_4_2"), 1)
        IF_INT_EQUAL(GW(10), 0)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("wa_pu"), 1)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
        END_IF
    END_SWITCH
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_close)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 377)
        IF_INT_EQUAL(GSWF(1329), 0)
            CALL(evt_door_end_wait)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_stop, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 40)
            CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_set_dir, 270, 200, 0)
            END_INLINE_EVENT
            CALL(evt_party_set_dir, 0, 90, 200)
            CALL(evt_msg_print_party, STRING("tik_00_tensou_1"))
            SET_VALUE(GSWF(1329), 1)
            CALL(evt_party_run, 0)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 377)
        CALL(evt_door_end_wait)
        CALL(evt_mario_key_onoff, 0)
        CALL_CHILD(PTR(kurihakase_event))
        CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    RETURN
EVT_END()

static DoorParam door_data_04 = {
    0, // flags
    3, // type
    1, // se
    "ie_4door", // mapobj
    "a_door_4_N", // hitIn
    "a_door_4", // hitOut
    "ie_4_soto", // outside
    NULL, // outsideHit
    "ie_4_naka", // inside
    NULL, // insideHit
    evt_door_04_00, // evtIn
    evt_door_04_01, // evtOut
    evt_wall_04, // evtWall
    NULL, NULL, NULL, // npcs..items
    evt_open_04_open, // evtOpen
    evt_door_04_close, // evtClose
};

EVT_BEGIN(evt_door_stg7_open)
    IF_INT_EQUAL(GSWF(1327), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("tik_000"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SPAWN_EVENT(PTR(evt_door_open))
    RETURN
EVT_END()

EVT_BEGIN(evt_door_stg7_close)
    SET_VALUE(GSWF(1327), 1)
    SPAWN_EVENT(PTR(evt_door_close))
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_entry)
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_door_stg7_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_stg7_open), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(tensou_return)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_map_playanim, STRING("anm_wa-p"), 1, 0)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_hit_get_position, STRING("A_tensou"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP_LIGHT1"), LW(0), LW(1), LW(2), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 5, 0, 5760, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MODULO(LW(0), 360)
            CALL(evt_party_set_dispdir, 0, LW(0))
            ADD(LW(0), 180)
            MODULO(LW(0), 360)
            CALL(evt_mario_set_dispdir, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 12, 0, 100, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(scale_func, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(7))
    CALL(evt_sub_intpl_msec_init, 12, 70, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(8), LW(0))
        ADD(LW(0), LW(3))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        ADD(LW(8), LW(6))
        CALL(evt_party_set_pos, 0, LW(5), LW(8), LW(7))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_color, 1, STRING("wa_pu"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("wa_pu"), 1)
    CALL(evt_mapobj_color, 1, STRING("wa_pu"), 255, 255, 255, 255)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_cont_onoff, 1, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    RETURN
EVT_END()

EVT_BEGIN(tensou_init_event)
    SET_VALUE(GW(10), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(377)
        CALL(evt_map_playanim, STRING("anm_PC"), 1, 0)
        CALL(evt_map_playanim, STRING("anm_Terop"), 1, 0)
        CALL(evt_map_playanim, STRING("anm_tiktik"), 1, 0)
        CASE_LESS(405)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("anm_PC"), 1, 0)
        CALL(evt_map_playanim, STRING("anm_Terop"), 1, 0)
        CALL(evt_map_playanim, STRING("anm_tiktik"), 1, 0)
    END_SWITCH
    IF_INT_EQUAL(GSWF(4175), 0)
        RETURN
    END_IF
    SET_VALUE(GSWF(4175), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -305, 38, -250)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_pos, 0, -275, 38, -250)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_party_set_pos, 1, -290, 38, -265)
    CALL(evt_party_set_dir, 1, 270, 0)
    CALL(evt_cam_road_reset)
    CALL(evt_seq_wait, 2)
    SET_VALUE(LW(0), PTR(&door_data_04))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    CALL_CHILD(PTR(tensou_return))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(377)
        IF_INT_EQUAL(GSWF(1328), 0)
            WAIT_MS(1500)
            CALL(evt_msg_print_party, STRING("tik_00_tensou_0"))
            SET_VALUE(GSWF(1328), 1)
        END_IF
        CASE_LESS(378)
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, -300, 109, 111, -300, 70, -184, 500, 11)
        WAIT_MS(500)
        CALL(evt_party_set_dir, 0, 270, 300)
        WAIT_MS(300)
        CALL(evt_party_set_dir, 0, 90, 300)
        WAIT_MS(300)
        CALL(evt_party_set_dir, 0, 270, 300)
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 300, 0)
        CALL(evt_msg_print_party, STRING("stg7_tik_01"))
        CALL(evt_mario_set_pose, STRING("M_N_1"))
        WAIT_MS(700)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_cam3d_evt_off, 500, 11)
        CASE_DEFAULT
    END_SWITCH
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

USER_FUNC(card_camid) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    if (mapGetMapObj(name) != NULL) {
        mapGetMapObj(name)->cameraId = camId;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(supotto_blendmode) {
    MapObject* obj = mapGetMapObj("supotto");

    obj->blendMode = 10;
    return EVT_RETURN_DONE;
}

USER_FUNC(tik_snd_off) {
    psndSetFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(tik_snd_on) {
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_key_onoff_wait) {
    marioGetPtr();
    switch (!marioChkKey()) {
        default:
            return EVT_RETURN_BLOCK;
        case 0:
            return EVT_RETURN_DONE;
    }
}

static USER_FUNC(scale_func) {
    f32 s = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);

    mario->scale = (Vec){s, s, s};
    if (party != NULL) {
        party->scale = (Vec){s, s, s};
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(tensou_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_mov_pos2, -305, -250, FLOAT(100.0f))
    CALL(evt_party_move_pos2, 0, -275, -250, FLOAT(100.0f))
    CALL(evt_hit_get_position, STRING("tensou"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP_LIGHT1"), LW(0), LW(1), LW(2), LW(9))
    SET_VALUE(GW(0), 0)
    CALL(evt_mario_get_dispdir, LW(0))
    CALL(evt_mario_get_dispdir, LW(1))
    BEGIN_INLINE_EVENT
        LOOP(0)
            ADD(LW(0), GW(0))
            ADD(LW(1), GW(0))
            CALL(evt_mario_set_dispdir, LW(0))
            CALL(evt_party_set_dispdir, 0, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 30, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_cam_off)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(7))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 12, 100, 0, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(scale_func, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 12, 0, 100, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(8), LW(0))
        ADD(LW(0), LW(3))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        ADD(LW(8), LW(6))
        CALL(evt_party_set_pos, 0, LW(5), LW(8), LW(7))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_snd_sfxoff, LW(9))
    SET_VALUE(GSWF(1326), 1)
    CALL(evt_bero_mapchange, STRING("aji_07"), 0)
    RETURN
EVT_END()

EVT_BEGIN(tensou_anim_event)
    SET_VALUE(GW(10), 1)
    CALL(evt_map_playanim, STRING("anm_saisho"), 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hikari"), 1)
    CALL(evt_map_checkanim, STRING("anm_saisho"), LW(1), LW(2))
    WAIT_MS(LW(2))
    CALL(evt_map_playanim, STRING("anm_wa-p"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(tensou_sw_event)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_SWITCH_WARP1"), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 377)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
        SPAWN_EVENT(PTR(tensou_anim_event))
        CALL(evt_run_case_evt, 0, 1, STRING("tensou"), 0, PTR(tensou_event), 0)
        CALL(evt_exit_case_evt)
        ELSE
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 403)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__1"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
            SPAWN_EVENT(PTR(tensou_anim_event))
            CALL(evt_run_case_evt, 0, 1, STRING("tensou"), 0, PTR(tensou_event), 0)
            CALL(evt_exit_case_evt)
            ELSE
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_msg_toge, 1, 0, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg7_tik_01_01"), 0, 0)
            CALL(evt_mario_key_onoff, 1)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kurihakase_event)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_envon_f, 272, STRING("ENV_STG0_TIK2"), 2000)
    CALL(evt_npc_set_position, STRING("クリハカセ"), -30, 0, 170)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 40)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_7_000"), 0, STRING("クリハカセ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(800)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_FRANKURI1"))
    CALL(evt_cam3d_evt_set, -175, 55, 409, 83, 30, -123, 400, 11)
    WAIT_MS(900)
    CALL(evt_msg_print, 0, STRING("mac_7_001"), 0, STRING("クリハカセ"))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -105, 125, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -71, 139, FLOAT(120.0f))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_7_002"), 0, STRING("クリハカセ"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_7_003"), 0, STRING("クリハカセ"))
    CALL(evt_party_move_pos2, 0, -100, 145, FLOAT(120.0f))
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_7_004"))
    CALL(evt_msg_print, 0, STRING("mac_7_005"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_7_006"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("mac_7_007"), 0, STRING("クリハカセ"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_set_ry_lr, STRING("クリハカセ"), 1)
    CALL(evt_npc_move_position, STRING("クリハカセ"), 160, 64, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_position, STRING("クリハカセ"), 0, -1000, 0)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 378)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_TIK1"), 2000)
    RETURN
EVT_END()

EVT_BEGIN(hashira_bomb_sub)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_before"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_hitobj_onoff, STRING("a_bom_mae"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_bom_go"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("ro_ru"), 1, 0)
    SET_VALUE(GSWF(1330), 1)
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("bom_mae"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(12), 0)
        IF_INT_EQUAL(LW(13), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_img_clear_virtual_point, STRING("capture_img"))
        CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_mae"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_go"), 1)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_mae"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_go"), 1)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("bom_mae"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

EVT_BEGIN(hashira_bomb)
    IF_INT_EQUAL(GSWF(1330), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_go"), 1)
        CALL(evt_hitobj_onoff, STRING("a_bom_go"), 1, 0)
        CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
        CALL(evt_run_case_evt, 15, 1, STRING("a_bom_mae"), 0, PTR(hashira_bomb_sub), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_mae"), 1)
        CALL(evt_hitobj_onoff, STRING("a_bom_mae"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("ro_ru"), 1, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -540, 20, -25)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, -350, 160, 582, -350, 65, 40, 0, 11)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_4_naka"), 1)
    CALL(evt_seq_wait, 2)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_npc_move_position, STRING("クリスチーヌ"), -345, -25, 0, 60, 0)
    CALL(evt_npc_move_position, STRING("クリスチーヌ"), -345, -5, 0, 60, 0)
    SET_VALUE(LW(0), PTR(&door_data_04))
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_sub_intpl_msec_init, 5, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_04_00))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_move_position, STRING("クリスチーヌ"), -345, -70, 0, 60, 0)
    SET_VALUE(LW(0), PTR(&door_data_04))
    SPAWN_EVENT(PTR(door_close_se))
    CALL(evt_sub_intpl_msec_init, 5, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_04_00))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("moo_04"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), 246, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item01"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_00_init_evt)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 402)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 377)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK2"))
            ELSE
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envoff, 512)
        END_IF
    END_IF
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(johoya_data_alloc, STRING("jketi_code"))
    SET_VALUE(GSWF(6), 0)
    SET_VALUE(GSWF(237), 0)
    CALL(uranaisi_table_clear)
    SET_VALUE(LW(0), PTR(&door_data_01))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_02))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_03))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_04))
    CALL_CHILD(PTR(evt_door_04_entry))
    SPAWN_EVENT(PTR(tensou_init_event))
    CALL(evt_run_case_evt, 9, 1, STRING("a_sui"), 0, PTR(tensou_sw_event), 0)
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    SET_VALUE(LW(0), STRING("ugo_yuka"))
    SET_VALUE(LW(1), STRING("a_ugo_yuka"))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    SPAWN_EVENT(PTR(hashira_bomb))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 525, 209, -175, 0, GSWF(5530), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box01"), -375, -60, 320, 0, 0, PTR(mobj_box01), GSWF(1354))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -220, 95, -360, STRING("ie_3_naka"), 125, GSWF(5588))
    CALL(evt_item_entry, STRING("item_00"), 125, 460, -40, 215, 16, GSWF(5585), 0)
    CALL(evt_item_entry, STRING("item_01"), 125, 530, 20, -220, 16, GSWF(5586), 0)
    CALL(evt_item_entry, STRING("item_02"), 125, -530, 20, -220, 16, GSWF(5587), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_1"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_2"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_3"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ie_4"), 2)
    RETURN
EVT_END()
