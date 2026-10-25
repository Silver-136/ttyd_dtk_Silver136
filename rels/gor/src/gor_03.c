#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "driver/mapdrv.h"
#include "driver/shadowdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
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
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "eff/eff_bomb.h"

#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "mario/mariost.h"
#include "system.h"
#include "dolphin/os.h"
#include "memory.h"
#include "string.h"
#include "driver/msgdrv.h"
#include "evt/evt_yuugijou.h"
#include "stdio.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "mario/mario.h"
#include "driver/dispdrv.h"
#include "pmario_sound.h"
#include "driver/casedrv.h"
#include "dolphin/gx.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_badgeshop.h"
#include "driver/effdrv.h"
#include "driver/animdrv.h"
#include "icon_ids.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_sub.h"
#include "gor.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#ifdef VERSION_G8ME01
USER_FUNC(evt_msg_fmt_int);
USER_FUNC(evt_msg_fmt_str);
#endif

typedef struct FuwaTable { f32 v[40]; } FuwaTable;
static const FuwaTable fuwa_table = {{0.0f, 0.5f, 1.0f, 1.4f, 1.8f, 2.0f, 2.3f, 2.5f, 2.6f, 2.7f, 2.7f, 2.7f, 2.6f, 2.5f, 2.3f, 2.0f, 1.8f, 1.4f, 1.0f, 0.5f, -0.0f, -0.5f, -1.0f, -1.4f, -1.8f, -2.0f, -2.3f, -2.5f, -2.6f, -2.7f, -2.7f, -2.7f, -2.6f, -2.5f, -2.3f, -2.0f, -1.8f, -1.4f, -1.0f, -0.5f}};
typedef struct ExchangeRyokin { s32 ryokin; s32 medal; } ExchangeRyokin;
#define RYOKIN_TBL ((ExchangeRyokin(*)[3])exchange_ryokin_medal_tbl)
s32 exchange_ret_tbl_no(void);
s32 yuugijou_montemedal_keep(EventEntry* event, BOOL isFirstCall);
void monteCountDisp(void);
typedef struct GoryuuWork {
    EventEntry* event;
    EventEntry* subEvent;
    u16 flags;
    s32 type;
    s32 (*init)(EventEntry* event, BOOL isFirstCall);
    s32 (*main)(EventEntry* event, BOOL isFirstCall);
    void (*disp)(CameraId camera, void* param);
    void* data;
} GoryuuWork;
typedef struct SlotWork {
    u32 flags;
    s32 phase;
    f32 reberAngle;
    f32 angle[2][3];
    f32 speed[3];
    s8 state[3];
    s8 face[2][3];
    s8 stopCount[3];
    s32 wait[3];
    s32 extra[3];
    s32 counter;
    s32 neonCount;
    s32 monteCount;
    s32 nowSlot;
    s32 caseId[2];
    EventEntry* monteEvt;
    s32 sfxId;
    s16 cursor;
} SlotWork;
typedef struct GMonteSub {
    s32 unk0;
    Vec pos;
    f32 scale;
    u8 unk14[0x38 - 0x14];
} GMonteSub;
typedef struct GMonte {
    u8 hidden;
    u8 state;
    s16 wait;
    s16 timer;
    u8 unk6[2];
    GMonteSub sub;
    Vec vel;
    SlotWork* owner;
} GMonte;
static s32 dataadr[8];
static GMonte* monte_p;
NpcEntry* npc;
static EffectEntry* eff;
static s32 xlw[16];
char* exchange_msgp;
static void monte_disp(CameraId camera, void* param);
static BOOL get_monte_check(Vec* a, Vec* b);
typedef struct SlotDrum {
    const char* drum[3];
    const char* reber;
    const char* anim;
    const char* window[3][3];
    const char* neon[3];
} SlotDrum;
void goryuugijo_upsidewindow_disp_sub(s32 slot, s32 reel);
#define UPSIDEGARA ((s32(*)[3][8])upsidegara)
void goryuugijo_upsidewindow_dispoff_sub(s32 slot, s32 reel);
#define SLOTGARA ((s32(*)[3][8])slotgara)
s32 slot_rotate_sub(GoryuuWork* wp, SlotWork* data, s32 reel);
void slot_neon_sub(GoryuuWork* wp, SlotWork* data);
void mapObjGetPos(const char* name, Vec* pos);
EffectEntry* effMarioBalloonEntry(s32 type, NpcEntry* npc);

USER_FUNC(desk_talk_func);
USER_FUNC(desk_talk_init_func);
USER_FUNC(dummy_shadow);
static USER_FUNC(enzan_lw_load);
static USER_FUNC(enzan_lw_save);
USER_FUNC(evt_bero_get_info_num);
USER_FUNC(evt_bero_id_filter);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_cam3d_evt_set_xyz);
USER_FUNC(evt_cam_shift_reset);
USER_FUNC(evt_door_end_wait);
USER_FUNC(evt_door_param);
USER_FUNC(evt_exchange_msg_alloc);
USER_FUNC(evt_exchange_msg_free);
USER_FUNC(evt_exchange_msg_set);
USER_FUNC(evt_goryuugijo_alloc);
USER_FUNC(evt_goryuugijo_set_caseid);
USER_FUNC(evt_goryuugijo_set_nowslot);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_item_end_wait);
USER_FUNC(evt_lottery);
USER_FUNC(evt_lottery_buy);
USER_FUNC(evt_lottery_error_check);
USER_FUNC(evt_lottery_flag);
USER_FUNC(evt_lottery_todaynum);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_set_offscreen);
USER_FUNC(evt_mario_balloon_bikkuri);
USER_FUNC(evt_mario_balloon_off);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_name_hitobj_front);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_party_use_check);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mobj_check);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_npc_flag_check);
USER_FUNC(evt_npc_get_reglid);
USER_FUNC(evt_npc_majo_disp_on);
USER_FUNC(evt_npc_restart_regular_event);
USER_FUNC(evt_npc_set_ry_lr);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_party_flg_onoff);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_envon_f);
USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(evt_sub_get_language);
USER_FUNC(evt_sub_system_flag_onoff);
USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(evt_win_item_select);
USER_FUNC(evt_win_other_select);
USER_FUNC(evt_yuuranking_alloc);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(exchange_ryokin_medal);
static USER_FUNC(fall_rtn);
USER_FUNC(get_mario_dispReqDir);
USER_FUNC(gor_npc_fuwafuwa);
USER_FUNC(goryuugijo_door_init_slot);
USER_FUNC(goryuugijo_get_dataadr);
USER_FUNC(goryuugijo_get_slot_phase);
USER_FUNC(goryuugijo_set_dataadr);
USER_FUNC(goryuugijo_set_slot_phase);
USER_FUNC(kuzi_bomb_bomb_eff);
USER_FUNC(kuzi_get_keihin);
static USER_FUNC(mario_keyoff_wait);
USER_FUNC(mario_walkpoint_enzan);
USER_FUNC(minigameselect_msg_set);
static USER_FUNC(setmertn);
USER_FUNC(teresa_badge_item_list_init);
USER_FUNC(unitwincoin_disp_onoff);
USER_FUNC(unitwincoin_wait);
USER_FUNC(unitwin_lottery_disp_onoff);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
static USER_FUNC(viewport);
USER_FUNC(yuugijou_add_montemedal);
USER_FUNC(yuugijou_add_montemedal_moto);
USER_FUNC(yuugijou_get_montemedal);
USER_FUNC(yuugijou_get_montemedal_keep);
USER_FUNC(yuugijou_get_takeoutflag);
USER_FUNC(yuugijou_montemedal_disp_onoff);
USER_FUNC(yuugijou_montemedal_keep);
USER_FUNC(yuugijou_montemedal_return);
USER_FUNC(yuugijou_montemedal_wait);
USER_FUNC(yuugijou_set_yupflag);
USER_FUNC(yuuranking_wait_button);
void kuji_bomb_055_callback(s32 type, MsgWinData** work);
void goryuugijo_disp_slot(CameraId camera, void* param);
void goryuugijo_main_slot(EventEntry* event, BOOL isFirstCall);
void goryuugijo_init_slot(EventEntry* event, BOOL isFirstCall);
USER_FUNC(evt_goryuugijo_get_caseid);
void goryuugijo_disp_montemedal(CameraId camera, void* param);
void goryuugijo_main_montemedal(EventEntry* event, BOOL isFirstCall);
void goryuugijo_init_montemedal(EventEntry* event, BOOL isFirstCall);

EvtWord debug_event[16];
EvtWord gor_03_down_evt[8];
static BeroEntry bero_entry_data[6];
BeroEntry yuu_00_data[1];
BeroEntry yuu_01_data[1];
BeroEntry yuu_02_data[1];
BeroEntry yuu_03_data[1];
EvtWord maffiaboss_ticket_put[31];
EvtWord maffiaboss_ticket_check[241];
EvtWord maffiaboss_init[73];
EvtWord maffiaboss_maffiaboss[495];
EvtWord maffiaboss_airship_ticket[768];
EvtWord maffiaboss_teshita_irai[253];
EvtWord maffiaboss_train_ticket_init[50];
EvtWord maffiaboss_train_ticket[824];
EvtWord maffiaboss_talk_common[106];
EvtWord maffiaboss_talk[10];
EvtWord maffiaboss_desk_talk[VERSION_PICK(46, 54)];
EvtWord maffiaboss_casino_init[17];
EvtWord maffiaboss_casino_talk[45];
EvtWord maffiaboss_nekomu00[14];
EvtWord maffiaboss_nekomu01[14];
EvtWord maffiaboss_nekomu02[14];
EvtWord maffia1_init[23];
EvtWord maffia1_talk[128];
EvtWord maffia2_init[23];
EvtWord maffia2_talk[128];
EvtWord maffia3_init[8];
EvtWord maffia3_talk[68];
static EvtWord mony_init[54];
static EvtWord mony_talk[53];
static EvtWord peton_init[69];
s32 iri_19_item_tbl[2];
EvtWord peton_talk_common[235];
static EvtWord peton_talk[4];
EvtWord peton_desk_talk[47];
static EvtWord init_gurume[28];
static EvtWord talk_gurume[4];
static EvtWord luigi_init[18];
static EvtWord luigi_talk[4];
static EvtWord luigi_npcEnt[19];
EvtWord cherry_init[12];
EvtWord cherry_talk[8];
NpcSetupInfo cherryEnt[2];
EvtWord bteresa_n_fadein[45];
EvtWord bteresa_n_fadeout[42];
EvtWord exchange_return_sub[VERSION_PICK(111, 132)];
EvtWord exchange_keep_sub[VERSION_PICK(111, 132)];
EvtWord gor_03_dokan_init_evt[35];
EvtWord gor_03_dokan_up_evt[60];
EvtWord gor_03_dokan_down_evt[63];
s32 exchange_ryokin_medal_tbl[54];
EvtWord bteresa_talk_sub_minigame[245];
EvtWord bteresa_talk_sub_keihin[VERSION_PICK(220, 242)];
EvtWord bteresa_table_talk[437];
EvtWord bteresa_init[18];
EvtWord bteresa_talk[81];
EvtWord kyaku1_init[2];
EvtWord kyaku1_talk[66];
EvtWord kyaku4_init[2];
EvtWord kyaku4_talk[66];
f32 montemedalzpos[2];
EvtWord slot_fall_monte[4];
EvtWord slot_set[VERSION_PICK(148, 168)];
EvtWord slot1_set[7];
EvtWord slot2_set[7];
SlotDrum drumname[2];
EvtWord* slotevttbl[2];
s32 slotgara[48];
s32 upsidegara[48];
EvtWord ranking_set[65];
EvtWord exchange_set[VERSION_PICK(195, 210)];
EvtWord roten_4_init[13];
EvtWord roten_4_regl[19];
EvtWord roten_4_talk[68];
EvtWord kinopiko_a_init[8];
EvtWord kinopiko_a_talk[68];
static EvtWord kinopiko_b_init[13];
EvtWord kinopiko_b_regl[19];
static EvtWord kinopiko_b_talk[159];
EvtWord kinopio_a_init[8];
EvtWord kinopio_a_talk[68];
EvtWord kokinopio_a_init[8];
EvtWord kokinopio_a_talk[68];
EvtWord kokinopio_b_init[13];
EvtWord kokinopio_b_regl[19];
EvtWord kokinopio_b_talk[68];
EvtWord nokonoko_a_init[8];
EvtWord nokonoko_a_talk[68];
EvtWord kuribo_1_init[8];
EvtWord kuribo_1_talk[68];
static NpcSetupInfo npcEnt[22];
static EvtWord evt_door_01_00[39];
static EvtWord evt_wall_01[28];
EvtWord evt_door_01_10[39];
static const char* door_data_01_npc[2];
EvtWord evt_door_01_0_open[31];
EvtWord evt_door_01_0_close[69];
EvtWord evt_door_01_1_open[132];
EvtWord evt_door_01_1_close[146];
DoorParam door_data_01_0;
DoorParam door_data_01_1;
EvtWord evt_door_01_entry[39];
static EvtWord evt_door_02_00[44];
EvtWord evt_wall_02_0[15];
const char* door_data_02_0_npc[6];
EvtWord door_data_02_0_open_sub[13];
EvtWord door_data_02_0_open[28];
EvtWord door_data_02_0_close[51];
const char* door_data_02_mobj[2];
DoorParam door_data_02_0l;
DoorParam door_data_02_0r;
EvtWord evt_door_02_10[15];
EvtWord evt_door_02_11[15];
EvtWord evt_wall_02_1[28];
const char* door_data_02_1_npc[6];
EvtWord desk_talk_check[173];
EvtWord evt_door_02_1_open[118];
EvtWord evt_door_02_1_close[48];
DoorParam door_data_02_1;
static EvtWord evt_door_03_00[12];
static EvtWord evt_wall_03[28];
static const char* door_data_03_npc[2];
EvtWord door_data_03_open[17];
EvtWord door_data_03_close[19];
static DoorParam door_data_03;
static EvtWord evt_door_03_caseopen[6];
static EvtWord evt_door_03_open[170];
static EvtWord evt_door_03_close[19];
static DoorParam door_data_03_roll;
static EvtWord evt_door_03_entry[24];
static EvtWord evt_door_04_00[12];
static EvtWord evt_wall_04[28];
static const char* door_data_04_npc[3];
static EvtWord evt_door_04_open[17];
EvtWord evt_door_04_close_snd[19];
static DoorParam door_data_04;
EvtWord evt_door_04_caseclose[6];
static EvtWord evt_door_04_close[70];
DoorParam door_data_04_roll;
static EvtWord evt_door_04_entry[20];
EvtWord wall_bomb_init[51];
static EvtWord wall_bomb[146];
EvtWord uraguchi_tenin[23];
EvtWord uraguchi_event[323];
EvtWord uraguchi_check[34];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
EvtWord majorin_nusumigiki[331];
EvtWord yuureturn_rtn[101];
static EvtWord peach_mail[248];
EvtWord kuzi_bomb_talk_normal[372];
EvtWord kuzi_bomb_talk_husei[589];
EvtWord kuzi_bomb_bomb[152];
EvtWord kuzi_keiziban_normal[391];
EvtWord kuzi_keiziban_husei[14];
EvtWord kuzi_bomb_talk[16];
EvtWord kuzi_keiziban_entry[27];
EvtWord tenin_fuwafuwa[7];
EvtWord casino_fuwafuwa[7];
EvtWord debug_event_mode[144];
EvtWord monte_evt_door_close[16];
EvtWord monte_evt_door_entry[73];
EvtWord gor_03_init_evt[VERSION_PICK(462, 681)];

static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(gor_npc_fuwafuwa) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    f32 speed = evtGetFloat(event, args[1]);
    s32 duration = evtGetValue(event, args[2]);
    FuwaTable table = fuwa_table;
    Vec* base = (Vec*)&event->userdataf[1];
    s32 elapsed;
    s32 frame;
    f32 y;
    f32 t;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        base->x = npc->position.x;
        base->y = npc->position.y;
        base->z = npc->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    t = 0.5f + sysMsec2FrameFloat(elapsed);
    frame = t * speed;
    y = base->y;
    y += table.v[(u32)frame % 40];
    npc->position.y = npc->prevPosition.y = y;
    if (duration == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (elapsed < duration) {
        return EVT_RETURN_BLOCK;
    }
    npc->position.y = npc->prevPosition.y = base->y;
    return EVT_RETURN_YIELD;
}

s32 exchange_ret_tbl_no(void) {
    s32 n = evtGetValue(NULL, GSW_STORY_PROGRESS);
    s32 no = 0;

    if (n >= 402) {
        no = 8;
    } else if (n >= 375) {
        no = 7;
    } else if (n >= 344) {
        no = 6;
    } else if (n >= 267) {
        no = 5;
    } else if (n >= 218) {
        no = 4;
    } else if (n >= 171) {
        no = 3;
    } else if (n >= 117) {
        no = 2;
    } else if (n >= 61) {
        no = 1;
    }
    return no;
}

USER_FUNC(exchange_ryokin_medal) {
    EvtWord* args = event->args;
    ExchangeRyokin (*tbl)[3];
    s32 idx = evtGetValue(event, args[0]);
    s32 no = exchange_ret_tbl_no();

    evtSetValue(event, args[1], RYOKIN_TBL[no][idx].ryokin);
    tbl = RYOKIN_TBL; // FAKE: table pointer reloaded right before use; direct RYOKIN_TBL[no][idx] is 12 B shorter
    evtSetValue(event, args[2], tbl[no][idx].medal);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_exchange_msg_free) {
    if (exchange_msgp == NULL) {
        return EVT_RETURN_DONE;
    }
    _mapFree(exchange_msgp);
    exchange_msgp = NULL;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_exchange_msg_alloc) {
    exchange_msgp = _mapAlloc(0x100);
    memset(exchange_msgp, 0, 0x100);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_exchange_msg_set) {
    EvtWord* args = event->args;
    s32 value = evtGetValue(event, args[0]);
    const char* msg = msgSearch((const char*)evtGetValue(event, args[1]));
    s32 n = exchange_ret_tbl_no();

#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        sprintf(exchange_msgp, msg, value, exchange_ryokin_medal_tbl[n * 6 + 1], exchange_ryokin_medal_tbl[n * 6], exchange_ryokin_medal_tbl[n * 6 + 3], exchange_ryokin_medal_tbl[n * 6 + 2], exchange_ryokin_medal_tbl[n * 6 + 5], exchange_ryokin_medal_tbl[n * 6 + 4]);
    } else {
        sprintf(exchange_msgp, msg, value);
    }
#else
    sprintf(exchange_msgp, msg, value, exchange_ryokin_medal_tbl[n * 6 + 1], exchange_ryokin_medal_tbl[n * 6], exchange_ryokin_medal_tbl[n * 6 + 3], exchange_ryokin_medal_tbl[n * 6 + 2], exchange_ryokin_medal_tbl[n * 6 + 5], exchange_ryokin_medal_tbl[n * 6 + 4]);
#endif
    evtSetValue(event, args[2], (EvtWord)exchange_msgp);
    return EVT_RETURN_DONE;
}

USER_FUNC(minigameselect_msg_set) {
    EvtWord* args = event->args;
    const s8* msg = (const s8*)msgSearch((const char*)evtGetValue(event, args[0]));
    s32 flags[4];
    s32 k;
    s32 i;
    s32 val;
    s32 j;
    s32 count;

    flags[0] = evtGetValue(event, GSWF(1232));
    flags[1] = evtGetValue(event, GSWF(1235));
    flags[2] = evtGetValue(event, GSWF(1234));
    flags[3] = evtGetValue(event, GSWF(1233));
    j = 0;
    i = 0;
    while (msg[i] != '\n') {
        exchange_msgp[j++] = msg[i++];
    }
    exchange_msgp[j++] = msg[i++];
    count = 0;
    for (k = 0; k < 4; k++) {
        if (flags[k]) {
            while (msg[i] != '\n') {
                exchange_msgp[j++] = msg[i++];
            }
            count++;
            exchange_msgp[j++] = msg[i++];
        } else {
            while (msg[i] != '\n') {
                i++;
            }
            i++;
        }
    }
    while (msg[i] != '\0') {
        exchange_msgp[j++] = msg[i++];
    }
    exchange_msgp[j] = msg[i];
    k = 0;
    while (((s8*)exchange_msgp)[k] != '>') {
        k++;
    }
    while (((s8*)exchange_msgp)[k] != ' ') {
        k--;
    }
    val = 80 - (4 - count) * 12;
    k++;
    exchange_msgp[k] = val / 10 + '0';
    k++;
    exchange_msgp[k] = val % 10 + '0';
    evtSetValue(event, args[1], (EvtWord)exchange_msgp);
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_walkpoint_enzan) {
    EvtWord* args = event->args;
    Vec a;
    Vec b;
    Vec d;
    f32 dist;

    a.x = evtGetFloat(event, args[0]);
    a.y = 0.0f;
    a.z = evtGetFloat(event, args[1]);
    b.x = evtGetFloat(event, args[2]);
    b.y = 0.0f;
    b.z = evtGetFloat(event, args[3]);
    dist = evtGetFloat(event, args[4]);
    PSVECSubtract(&a, &b, &d);
    PSVECNormalize(&d, &d);
    PSVECScale(&d, &d, dist);
    PSVECAdd(&d, &b, &d);
    evtSetFloat(event, args[5], d.x);
    evtSetFloat(event, args[6], d.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(goryuugijo_get_dataadr) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);

    if (idx >= 8) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], dataadr[idx]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(goryuugijo_set_dataadr) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);

    if (idx < 8) {
        dataadr[idx] = evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

void goryuugijo_init_montemedal(EventEntry* event, BOOL isFirstCall) {
    if (!evtGetValue(event, GSWF(1229))) {
        yuugijou_montemedal_keep(event, isFirstCall);
    }
}

void goryuugijo_main_montemedal(EventEntry* event, BOOL isFirstCall) {
}

void goryuugijo_disp_montemedal(CameraId camera, void* param) {
    monteCountDisp();
}

USER_FUNC(evt_goryuugijo_set_caseid) {
    EvtWord* args = event->args;
    SlotWork* data = ((GoryuuWork*)evtGetValue(event, args[0]))->data;
    s32 idx = evtGetValue(event, args[1]);

    data->caseId[idx] = evtGetValue(event, args[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_goryuugijo_get_caseid) {
    EvtWord* args = event->args;
    SlotWork* data = ((GoryuuWork*)evtGetValue(event, args[0]))->data;
    s32 idx = evtGetValue(event, args[1]);

    evtSetValue(event, args[2], data->caseId[idx]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_goryuugijo_set_nowslot) {
    EvtWord* args = event->args;
    SlotWork* data = ((GoryuuWork*)evtGetValue(event, args[0]))->data;

    data->nowSlot = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

static void monte_disp(CameraId camera, void* param) {
    GMonte* monte = param;
    s32 i;

    for (i = 0; i < monte->owner->monteCount; i++) {
        if (monte[i].hidden == 0) {
            iconDispGx(&monte[i].sub.pos, 0x40, ICON_13F, 1.0f);
        }
    }
}

EVT_BEGIN(debug_event)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 64)
        CALL(evt_sub_system_flag_onoff, 0, 64)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(gor_03_down_evt)
    SET_VALUE(GSWF(1223), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)debug_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_04", // nextMapName
        "s_bero", // nextBeroName
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
        (s32*)debug_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)debug_event, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_07", // nextMapName
        "dokan_4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)debug_event, // enterEvent
        -1, // caseType
        (s32*)gor_03_down_evt, // exitEvent
        "tik_07", // nextMapName
        NULL, // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_dokan_02", // hitName
        8194, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "yuu_00", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry yuu_00_data[] = {
    {
        "A_dokan_02", // hitName
        8194, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "yuu_00", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

BeroEntry yuu_01_data[] = {
    {
        "A_dokan_02", // hitName
        8194, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "yuu_01", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

BeroEntry yuu_02_data[] = {
    {
        "A_dokan_02", // hitName
        8194, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "yuu_02", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

BeroEntry yuu_03_data[] = {
    {
        "A_dokan_02", // hitName
        8194, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "yuu_03", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

EVT_BEGIN(maffiaboss_ticket_put)
    CALL(evt_item_entry, STRING("ticket"), 52, 0, -1000, 0, 16, -1, 0)
    CALL(evt_item_flag_on, STRING("ticket"), 32768)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmon, 513, STRING("BGM_FF_GET_KEYITEM1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmon, 513, STRING("BGM_STG0_CSN1"))
    END_INLINE_EVENT
    CALL(evt_item_get_item, STRING("ticket"))
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_ticket_check)
    LOOP(0)
        SET_VALUE(LF(0), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -296)
            IF_INT_LESS_OR_EQUAL(LW(0), -104)
                IF_INT_GREATER_OR_EQUAL(LW(1), 155)
                    IF_INT_LESS_OR_EQUAL(LW(1), 180)
                        IF_INT_GREATER_OR_EQUAL(LW(2), -110)
                            ADD(LF(0), 1)
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        CALL(evt_mario_get_name_hitobj_ride, LW(0))
        IF_STR_EQUAL(LW(0), STRING("A_i02_tke_ue"))
            SET_VALUE(LW(5), 2)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(5), 0)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(15), 1)
            SET_VALUE(LW(5), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_snd_bgmon, 513, STRING("BGM_STG0_CSN1"))
    CALL(evt_msg_print, 0, STRING("mac_2_116"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_ry_lr, STRING("マフィアボス"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("マフィアボス"), LW(10), LW(11), LW(12))
        IF_INT_LESS_OR_EQUAL(LW(2), LW(12))
            CALL(evt_mario_set_dir, 90, 200, 0)
            ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("マフィアボス"), LW(10), LW(11), LW(12))
        IF_INT_LESS_OR_EQUAL(LW(2), LW(12))
            CALL(evt_party_set_dir, 0, 90, 200)
            ELSE
            CALL(evt_party_set_dir, 0, 270, 200)
        END_IF
    END_INLINE_EVENT
    CALL(evt_set_dir_to_target, STRING("マフィアボス"), STRING("mario"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_2_117"), 0, STRING("マフィアボス"))
    IF_INT_EQUAL(LW(5), 0)
#ifdef VERSION_G8ME01
        CALL(evt_npc_flag_onoff, 1, STRING("マフィアボス"), 1073741856)
#else
        CALL(evt_npc_flag_onoff, 1, STRING("マフィアボス"), 1073741824)
#endif
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("マフィアボス"), LW(10), LW(11), LW(12))
        CALL(evt_npc_move_position, STRING("マフィアボス"), LW(0), -68, 0, FLOAT(60.0f), 0)
        WAIT_MS(500)
        CALL_CHILD(PTR(maffiaboss_ticket_put))
        CALL(evt_msg_print, 0, STRING("mac_2_118"), 0, STRING("マフィアボス"))
        CALL(evt_npc_move_position, STRING("マフィアボス"), LW(10), LW(12), 0, FLOAT(60.0f), 0)
#ifdef VERSION_G8ME01
        CALL(evt_npc_flag_onoff, 0, STRING("マフィアボス"), 1073741856)
#else
        CALL(evt_npc_flag_onoff, 0, STRING("マフィアボス"), 1073741824)
#endif
        ELSE
        CALL_CHILD(PTR(maffiaboss_ticket_put))
        CALL(evt_msg_print, 0, STRING("mac_2_118"), 0, STRING("マフィアボス"))
    END_IF
    CALL(evt_npc_move_position, STRING("マフィアボス"), -198, -80, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_119"), 0, STRING("マフィアボス"))
    SET_VALUE(GSW_STORY_PROGRESS, 124)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GF(14), 0)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(123)
        CALL(evt_npc_set_position, STRING("me"), -198, 155, -80)
        IF_INT_EQUAL(GSWF(1236), 1)
            SET_VALUE(GF(15), 0)
            SPAWN_EVENT(PTR(maffiaboss_ticket_check))
        END_IF
        CASE_BETWEEN(268, 278)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 1090781184)
        CALL(evt_npc_set_position, STRING("me"), -200, 170, -152)
        CALL(evt_npc_set_rotate_offset, STRING("me"), 0, 20, 0)
        CALL(evt_npc_set_rotate, STRING("me"), 90, 0, 0)
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("S_2"), STRING("T_2"))
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
        CASE_GREATER(278)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), -198, 155, -80)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_maffiaboss)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -296)
            IF_INT_LESS_OR_EQUAL(LW(0), -104)
                IF_INT_GREATER_OR_EQUAL(LW(1), 155)
                    IF_INT_GREATER_OR_EQUAL(LW(2), -180)
                        IF_INT_LESS_OR_EQUAL(LW(2), -53)
                            BREAK_LOOP
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    SET_VALUE(LSWF(3), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), 1073741856)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -210, -165, FLOAT(120.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, -180, -180, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_2_045"), 0, STRING("マフィアボス"))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -485, 250, -95, -188, 205, -95, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_046"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -683, 289, -160, -189, 215, -160, 0, 11)
    CALL(evt_npc_get_position, STRING("マフィア１"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア１"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_047"), 0, STRING("マフィア１"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア１"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("マフィア２"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_048"), 0, STRING("マフィア２"))
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_049"), 0, STRING("マフィアボス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_050"), 0, STRING("マフィアボス"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_2_051"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_2_052"), 0, STRING("マフィア２"))
    CALL(evt_party_move_pos2, 0, -170, -175, FLOAT(120.0f))
    CALL(evt_msg_print_party, STRING("mac_2_053"))
    CALL(evt_cam3d_evt_set, -509, 252, -125, -188, 202, -125, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_056"), 0, STRING("マフィアボス"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("mac_2_057"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_2_060"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -509, 252, -230, -188, 202, -230, 500, 11)
    WAIT_MS(800)
    CALL(evt_npc_get_position, STRING("マフィア１"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア１"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_061"), 0, STRING("マフィア１"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア１"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("マフィア２"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_062"), 0, STRING("マフィア２"))
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_063"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -683, 289, -160, -189, 215, -160, 0, 11)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_2_064"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_2_065"), 0, STRING("マフィア２"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_066"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -509, 252, -125, -188, 202, -125, 500, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_067"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -385, 230, -83, -187, 200, -83, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 1, 500)
    END_INLINE_EVENT
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("mac_2_068"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア２"), 1073741856)
    SET_VALUE(GSW_STORY_PROGRESS, 122)
    SET_VALUE(LSWF(3), 0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_airship_ticket)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -296)
            IF_INT_LESS_OR_EQUAL(LW(0), -104)
                IF_INT_GREATER_OR_EQUAL(LW(1), 155)
                    IF_INT_GREATER_OR_EQUAL(LW(2), -180)
                        IF_INT_LESS_OR_EQUAL(LW(2), -53)
                            BREAK_LOOP
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    SET_VALUE(LSWF(3), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィアボス"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("mac_2_088"), 0, STRING("マフィアボス"))
    CALL(evt_msg_select, 0, STRING("mac_2_088_yesno"))
    CALL(evt_msg_continue)
    CALL(evt_cam3d_evt_set, -509, 252, -100, -188, 202, -100, 500, 11)
    WAIT_MS(500)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("mac_2_089"), 0, STRING("マフィアボス"))
        CALL(evt_cam3d_evt_set, -683, 289, -160, -189, 215, -160, 0, 11)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_2_090"), 0, STRING("マフィアボス"))
        ELSE
        CALL(evt_msg_print, 0, STRING("mac_2_091"), 0, STRING("マフィアボス"))
    END_IF
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("モニー"), -260, 155, -410)
    CALL(evt_npc_set_ry, STRING("モニー"), 180)
    CALL(evt_npc_set_position, STRING("ピートン"), -275, 155, -440)
    CALL(evt_npc_set_ry, STRING("ピートン"), 180)
    CALL(evt_npc_set_ry, STRING("マフィアボス"), 0)
    CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, 3, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("mac_2_092"), 0, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -683, 289, -385, -189, 215, -385, 500, 11)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -485, 250, -90, -188, 205, -90, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_093"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -683, 289, -385, -189, 215, -385, 0, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pos, -118, 155, -122)
        CALL(evt_mario_set_dir, 270, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_pos, 0, -118, 155, -157)
        CALL(evt_party_set_dir, 0, 270, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_2_094"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -683, 289, -160, -189, 215, -160, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_2_095"), 0, STRING("マフィアボス"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("モニー"), -220, -245, 0, FLOAT(100.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ピートン"), -190, -250, 0, FLOAT(100.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_096"), 0, STRING("モニー"))
    CALL(evt_cam3d_evt_set, -663, 284, -157, -189, 213, -157, 200, 11)
    CALL(evt_npc_move_position, STRING("ピートン"), -190, -230, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_097"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -643, 278, -154, -189, 210, -154, 200, 11)
    CALL(evt_npc_move_position, STRING("モニー"), -220, -225, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_098"), 0, STRING("モニー"))
    CALL(evt_cam3d_evt_set, -623, 273, -151, -189, 207, -151, 200, 11)
    CALL(evt_npc_move_position, STRING("ピートン"), -190, -210, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_099"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -603, 268, -148, -189, 204, -148, 200, 11)
    CALL(evt_npc_move_position, STRING("モニー"), -220, -205, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_100"), 0, STRING("モニー"))
    CALL(evt_cam3d_evt_set, -583, 262, -145, -188, 202, -145, 200, 11)
    CALL(evt_npc_move_position, STRING("ピートン"), -190, -190, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_101"), 0, STRING("ピートン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_102"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -534, 247, -150, -188, 194, -150, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("マフィアボス"), LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    CALL(evt_npc_move_position, STRING("マフィアボス"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_103"), 0, STRING("マフィアボス"))
    CALL(evt_npc_get_position, STRING("マフィアボス"), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    CALL(evt_npc_move_position, STRING("マフィアボス"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_104"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_ry, STRING("マフィアボス"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_105"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_2_106"), 0, STRING("ピートン"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_2_107"), 0, STRING("マフィアボス"))
    CALL(evt_npc_move_position, STRING("モニー"), -220, -190, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_108"), 0, STRING("モニー"))
    CALL(evt_npc_move_position, STRING("ピートン"), -190, -175, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_109"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -450, 237, -180, -188, 200, -180, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_110"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_2_111"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -450, 242, -85, -188, 205, -85, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("mac_2_112"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -683, 289, -160, -189, 215, -160, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("マフィア１"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア１"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("モニー"), 90)
        CALL(evt_npc_set_ry, STRING("ピートン"), 90)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_2_113"), 0, STRING("マフィア１"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア１"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("マフィア２"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), -20)
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(10), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_2_114"), 0, STRING("マフィア２"))
    CALL(evt_npc_move_position, STRING("マフィア２"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_npc_set_ry, STRING("モニー"), 270)
    WAIT_MS(833)
    CALL(evt_msg_print, 0, STRING("mac_2_115"), 0, STRING("モニー"))
    CALL(evt_npc_set_ry, STRING("モニー"), 90)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ピートン"), -190, -400, 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, STRING("ピートン"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("モニー"), -220, -400, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_position, STRING("モニー"), 0, -1000, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 0, STRING("マフィアボス"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("モニー"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア２"), 1073741856)
    SET_VALUE(GSWF(1236), 1)
    SET_VALUE(GF(15), 0)
    SPAWN_EVENT(PTR(maffiaboss_ticket_check))
    SET_VALUE(LSWF(3), 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_teshita_irai)
    CALL(evt_msg_print, 0, STRING("mac_5_012"), 0, STRING("マフィアボス"))
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -435, 248, -235, -188, 212, -235, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_013"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_014"), 0, STRING("マフィア２"))
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_cam3d_evt_off, 0, 11)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_015"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_016"), 0, STRING("マフィア２"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_5_017"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_018"), 0, STRING("マフィア２"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("mac_5_019"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_ry, STRING("マフィア１"), 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_020"), 0, STRING("マフィア１"))
    CALL(evt_npc_set_ry, STRING("マフィア２"), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_021"), 0, STRING("マフィア２"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_022"))
    CALL(evt_npc_set_ry, STRING("マフィア１"), 180)
    CALL(evt_npc_set_ry, STRING("マフィア２"), 180)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_023"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_024"), 0, STRING("マフィア２"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_025"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_026"), 0, STRING("マフィア２"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_027"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_028"), 0, STRING("マフィア２"))
    CALL(evt_msg_select, 0, STRING("mac_5_028_yesno"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print, 0, STRING("mac_5_029"), 0, STRING("マフィア１"))
        CALL(evt_msg_print, 0, STRING("mac_5_030"), 0, STRING("マフィア２"))
        CALL(evt_msg_select, 0, STRING("mac_5_030_yesno"))
        CALL(evt_msg_continue)
    END_IF
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_031"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_032"), 0, STRING("マフィア２"))
    SET_VALUE(GSW_STORY_PROGRESS, 273)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_train_ticket_init)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(2), 30)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_040"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_041"), 0, STRING("ピートン"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_train_ticket)
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("マフィアボス"))
    CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("マフィアボス"))
    CALL(evt_npc_flag_onoff, 1, STRING("マフィアボス"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), 1073741856)
    CALL(evt_msg_print, 0, STRING("mac_5_046"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_anim, STRING("マフィアボス"), STRING("A_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("マフィアボス"), STRING("S_1"), STRING("T_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_047"), 0, STRING("モニー"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_5_048"), 0, STRING("ピートン"))
    IF_INT_EQUAL(LW(15), 0)
        DEBUG_PUT_MSG(STRING("ベッドの前から"))
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -245, -185, FLOAT(60.0f))
            CALL(evt_mario_set_dir, 90, 200, 0)
        END_INLINE_EVENT
        ELSE
        DEBUG_PUT_MSG(STRING("ベッドの横から"))
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -245, -197, FLOAT(60.0f))
            WAIT_FRAMES(1)
            CALL(evt_mario_mov_pos2, -245, -185, FLOAT(60.0f))
            CALL(evt_mario_set_dir, 90, 200, 0)
        END_INLINE_EVENT
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -230, -200, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -584, 265, -145, -189, 200, -145, 500, 11)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_CSN1"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("マフィアボス"), LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(evt_msg_print, 0, STRING("mac_5_049"), 0, STRING("マフィアボス"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_050"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_051"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_rotate, STRING("マフィアボス"), 0, 0, 0)
    CALL(evt_npc_jump_position_nohit, STRING("マフィアボス"), -250, 155, -120, 400, 20)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィアボス"), 262144)
    CALL(evt_npc_set_ry, STRING("マフィアボス"), 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -485, 244, -125, -189, 195, -125, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_052"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -683, 282, -165, -189, 208, -165, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
        ADD(LW(1), 60)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_5_053"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
        ADD(LW(1), 80)
        ADD(LW(2), 10)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_5_054"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("マフィア２"), LW(0), LW(1), LW(2))
        ADD(LW(1), 60)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_5_056"), 0, 0)
    END_INLINE_EVENT
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("マフィア１"), LW(0), LW(1), LW(2))
    ADD(LW(1), 80)
    ADD(LW(2), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("mac_5_055"), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_057"), 0, STRING("マフィアボス"))
    CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ピートン"), LW(0), LW(1), LW(2), 200, 30)
    CALL(evt_msg_print, 0, STRING("mac_5_058"), 0, STRING("ピートン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_059"), 0, STRING("マフィアボス"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("mac_5_060"), 0, STRING("ピートン"))
    CALL(evt_cam3d_evt_set, -510, 243, -155, -189, 195, -155, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_061"), 0, STRING("マフィアボス"))
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_5_062"), 0, STRING("マフィア１"))
    CALL(evt_cam3d_evt_set, -685, 280, -170, -191, 206, -170, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_063"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_064"), 0, STRING("マフィアボス"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_065"), 0, STRING("マフィア２"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -510, 243, -155, -189, 195, -155, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("mac_5_066"), 0, STRING("マフィアボス"))
    CALL(evt_cam3d_evt_set, -685, 280, -170, -191, 206, -170, 0, 11)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), -165, FLOAT(120.0f))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("マフィア１"), LW(0), LW(1), LW(2))
        ADD(LW(1), 60)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_5_067"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("マフィア２"), LW(0), LW(1), LW(2))
        ADD(LW(1), 80)
        ADD(LW(2), 10)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_5_068"), 0, 0)
    END_INLINE_EVENT
    CALL(evt_item_entry, STRING("ticket"), 53, 0, -1000, 0, 16, -1, 0)
    CALL(evt_item_flag_on, STRING("ticket"), 32768)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 513, STRING("BGM_FF_GET_KEYITEM1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    CALL(evt_item_get_item, STRING("ticket"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_069"), 0, STRING("マフィアボス"))
    CALL(evt_npc_set_ry, STRING("マフィアボス"), 180)
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING("マフィアボス"), 0)
    WAIT_MS(1300)
    CALL(evt_msg_print, 0, STRING("mac_5_070"), 0, STRING("マフィアボス"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("モニー"), LW(0), LW(1), LW(2), 300, 30)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ピートン"), LW(0), LW(1), LW(2), 300, 30)
    CALL(evt_msg_print, 0, STRING("mac_5_071"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_072"), 0, STRING("ピートン"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_5_073"), 0, STRING("マフィアボス"))
    CALL(evt_npc_move_position, STRING("マフィアボス"), -290, -140, 0, FLOAT(80.0f), 0)
    CALL(evt_npc_move_position, STRING("マフィアボス"), -290, -400, 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("マフィアボス"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("マフィアボスカジノ"), -270, 0, -450)
    CALL(evt_npc_set_ry, STRING("マフィアボスカジノ"), 180)
    WAIT_MS(1200)
    CALL(evt_cam3d_evt_set, -534, 252, -115, -188, 200, -115, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("ピートン"), 180)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_074"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_075"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("mac_5_076"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_077"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("mac_5_078"), 0, STRING("モニー"))
    CALL(evt_cam3d_evt_set, -683, 285, -150, -188, 211, -150, 0, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_set_ry, STRING("マフィア１"), 180)
        CALL(evt_npc_move_position, STRING("マフィア１"), -135, -250, 0, FLOAT(80.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("マフィア２"), 180)
    CALL(evt_npc_move_position, STRING("マフィア２"), -135, -200, 0, FLOAT(80.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_5_079"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_080"), 0, STRING("マフィア２"))
    CALL(evt_msg_print, 0, STRING("mac_5_081"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("mac_5_082"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("mac_5_083"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_084"), 0, STRING("マフィア２"))
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア１"), 1073741856)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア２"), 1073741856)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 279)
    BEGIN_INLINE_EVENT
        CALL(evt_del_case_evt, 0, GW(12))
        CALL(evt_del_case_evt, 0, GW(13))
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_bgmoff, 18432)
    SET_VALUE(GF(5), 1)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_talk_common)
    SET_VALUE(LSWF(3), 1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_msg_print, 0, STRING("mac_2_044_01"), 0, STRING("マフィアボス"))
        CASE_EQUAL(121)
        CASE_EQUAL(122)
        CALL(evt_msg_print, 0, STRING("mac_2_069"), 0, STRING("マフィアボス"))
        CASE_EQUAL(123)
        SET_VALUE(GF(15), 1)
        SET_VALUE(GF(14), 1)
        WAIT_WHILE(GF(14))
        CASE_BETWEEN(124, 126)
        CALL(evt_msg_print, 0, STRING("mac_2_120"), 0, STRING("マフィアボス"))
        CASE_EQUAL(125)
        CALL(evt_msg_print, 0, STRING("mac_2_120"), 0, STRING("マフィアボス"))
        CASE_BETWEEN(268, 271)
        CALL(evt_msg_print, 0, STRING("mac_5_011"), 0, STRING("マフィアボス"))
        CASE_EQUAL(272)
        CALL_CHILD(PTR(maffiaboss_teshita_irai))
        CASE_EQUAL(273)
        CALL(evt_msg_print, 0, STRING("mac_5_037"), 0, STRING("マフィアボス"))
        CASE_BETWEEN(274, 275)
        CALL(evt_msg_print, 0, STRING("mac_5_037"), 0, STRING("マフィアボス"))
        CASE_EQUAL(278)
        CALL_CHILD(PTR(maffiaboss_train_ticket))
        CASE_LESS(121)
        CALL(evt_msg_print, 0, STRING("gor_03_01_01"), 0, STRING("マフィアボス"))
        CASE_LESS(230)
        CALL(evt_msg_print, 0, STRING("gor_03_01_03"), 0, STRING("マフィアボス"))
    END_SWITCH
    SET_VALUE(LSWF(3), 0)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_talk)
    CALL(evt_party_stop, 0)
    CALL_CHILD(PTR(maffiaboss_talk_common))
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_desk_talk)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir_npc, STRING("マフィアボス"))
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    SET_VALUE(GF(10), 1)
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
        SET_VALUE(GF(10), 0)
#endif
    END_INLINE_EVENT
    CALL(evt_npc_flag_check, STRING("マフィアボス"), 16777216, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("マフィアボス"), STRING("mario"))
        END_INLINE_EVENT
    END_IF
    CALL_CHILD(PTR(maffiaboss_talk_common))
#ifdef VERSION_G8ME01
    WAIT_WHILE(GF(10))
#endif
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_casino_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER(278)
        CALL(evt_npc_set_position, STRING("me"), -270, 0, -450)
        CALL(evt_npc_set_ry, STRING("me"), 180)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_casino_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(272)
        CALL(evt_msg_print, 0, STRING("gor_03_01_04"), 0, STRING("me"))
        CASE_LESS(352)
        CALL(evt_msg_print, 0, STRING("gor_03_01_05"), 0, STRING("me"))
        CASE_LESS(382)
        CALL(evt_msg_print, 0, STRING("gor_03_01_06"), 0, STRING("me"))
        CASE_LESS(403)
        CALL(evt_msg_print, 0, STRING("gor_03_01_07"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(403)
        CALL(evt_msg_print, 0, STRING("gor_03_01_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_nekomu00)
    CALL(evt_msg_print, 0, STRING("mac_5_009"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_010"), 0, STRING("マフィア２"))
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_nekomu01)
    CALL(evt_msg_print, 0, STRING("mac_5_033"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_034"), 0, STRING("マフィア２"))
    RETURN
EVT_END()

EVT_BEGIN(maffiaboss_nekomu02)
    CALL(evt_msg_print, 0, STRING("mac_5_035"), 0, STRING("マフィア１"))
    CALL(evt_msg_print, 0, STRING("mac_5_036"), 0, STRING("マフィア２"))
    RETURN
EVT_END()

EVT_BEGIN(maffia1_init)
    CALL(evt_npc_set_position, STRING("me"), -135, 155, -260)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(LSWF(3), 0)
                CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(maffia1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_msg_print, 0, STRING("mac_2_044_02"), 0, STRING("me"))
        CASE_BETWEEN(122, 123)
        IF_INT_EQUAL(GSWF(1236), 0)
            CALL(evt_msg_print, 0, STRING("mac_2_070"), 0, STRING("me"))
            ELSE
#ifdef VERSION_G8ME01
            CALL(evt_msg_print, 0, STRING(VERSION_PICK("mac_2_116_buka", "mac_2_116_buka1")), 0, STRING("me"))
#else
            CALL(evt_msg_print, 0, STRING("mac_2_116_buka1"), 0, STRING("me"))
#endif
        END_IF
        CASE_BETWEEN(124, 126)
        CALL(evt_msg_print, 0, STRING("mac_2_121"), 0, STRING("me"))
        CASE_BETWEEN(268, 272)
        CALL_CHILD(PTR(maffiaboss_nekomu00))
        CASE_EQUAL(273)
        CALL_CHILD(PTR(maffiaboss_nekomu01))
        CASE_BETWEEN(274, 275)
        CALL_CHILD(PTR(maffiaboss_nekomu02))
        CASE_EQUAL(278)
        CALL(evt_msg_print, 0, STRING("mac_5_042"), 0, STRING("me"))
        CASE_LESS(121)
        CALL(evt_msg_print, 0, STRING("gor_03_02_01"), 0, STRING("me"))
        CASE_LESS(122)
        CALL(evt_msg_print, 0, STRING("gor_03_02_02"), 0, STRING("me"))
        CASE_LESS(230)
        CALL(evt_msg_print, 0, STRING("gor_03_02_03"), 0, STRING("me"))
        CASE_LESS(272)
        CALL(evt_msg_print, 0, STRING("gor_03_02_04"), 0, STRING("me"))
        CASE_LESS(352)
        CALL(evt_msg_print, 0, STRING("gor_03_02_05"), 0, STRING("me"))
        CASE_LESS(382)
        CALL(evt_msg_print, 0, STRING("gor_03_02_06"), 0, STRING("me"))
        CASE_LESS(403)
        CALL(evt_msg_print, 0, STRING("gor_03_02_07"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(403)
        CALL(evt_msg_print, 0, STRING("gor_03_02_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(maffia2_init)
    CALL(evt_npc_set_position, STRING("me"), -135, 155, -210)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(LSWF(3), 0)
                CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(maffia2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_msg_print, 0, STRING("mac_2_044_03"), 0, STRING("me"))
        CASE_BETWEEN(122, 123)
        IF_INT_EQUAL(GSWF(1236), 0)
            CALL(evt_msg_print, 0, STRING("mac_2_071"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("mac_2_116_buka2"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(124, 126)
        CALL(evt_msg_print, 0, STRING("mac_2_122"), 0, STRING("me"))
        CASE_BETWEEN(268, 272)
        CALL_CHILD(PTR(maffiaboss_nekomu00))
        CASE_EQUAL(273)
        CALL_CHILD(PTR(maffiaboss_nekomu01))
        CASE_BETWEEN(274, 275)
        CALL_CHILD(PTR(maffiaboss_nekomu02))
        CASE_EQUAL(278)
        CALL(evt_msg_print, 0, STRING("mac_5_043"), 0, STRING("me"))
        CASE_LESS(121)
        CALL(evt_msg_print, 0, STRING("gor_03_03_01"), 0, STRING("me"))
        CASE_LESS(122)
        CALL(evt_msg_print, 0, STRING("gor_03_03_02"), 0, STRING("me"))
        CASE_LESS(230)
        CALL(evt_msg_print, 0, STRING("gor_03_03_03"), 0, STRING("me"))
        CASE_LESS(272)
        CALL(evt_msg_print, 0, STRING("gor_03_03_04"), 0, STRING("me"))
        CASE_LESS(352)
        CALL(evt_msg_print, 0, STRING("gor_03_03_05"), 0, STRING("me"))
        CASE_LESS(382)
        CALL(evt_msg_print, 0, STRING("gor_03_03_06"), 0, STRING("me"))
        CASE_LESS(403)
        CALL(evt_msg_print, 0, STRING("gor_03_03_07"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(403)
        CALL(evt_msg_print, 0, STRING("gor_03_03_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(maffia3_init)
    CALL(evt_npc_set_position, STRING("me"), -140, 0, 10)
    RETURN
EVT_END()

EVT_BEGIN(maffia3_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_008"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_009"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_010"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_011"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_012"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_013"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_014"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_015"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mony_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_npc_set_position, STRING("me"), -265, 155, -225)
        CALL(evt_npc_set_ry, STRING("me"), 180)
        CASE_EQUAL(278)
        CALL(evt_npc_set_position, STRING("me"), -145, 155, -95)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        CASE_BETWEEN(279, 281)
        CALL(evt_npc_set_position, STRING("me"), -240, 155, -95)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        CASE_GREATER(281)
        CALL(evt_npc_set_position, STRING("me"), -130, 155, -115)
        CALL(evt_npc_set_ry, STRING("me"), 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mony_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_msg_print, 0, STRING("mac_2_044_05"), 0, STRING("me"))
        CASE_EQUAL(278)
        CALL(evt_msg_print, 0, STRING("mac_5_044"), 0, STRING("me"))
        CASE_LESS(352)
        CALL(evt_msg_print, 0, STRING("gor_03_04_01"), 0, STRING("me"))
        CASE_LESS(382)
        CALL(evt_msg_print, 0, STRING("gor_03_04_02"), 0, STRING("me"))
        CASE_LESS(403)
        CALL(evt_msg_print, 0, STRING("gor_03_04_03"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(403)
        CALL(evt_msg_print, 0, STRING("gor_03_04_04"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(peton_init)
    IF_INT_EQUAL(GSW(1420), 19)
        CALL(evt_npc_set_position, STRING("me"), 275, 0, 413)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_npc_set_position, STRING("me"), -280, 155, -185)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        CASE_EQUAL(278)
        CALL(evt_npc_set_position, STRING("me"), -145, 155, -155)
        CALL(evt_npc_set_ry, STRING("me"), 180)
        CASE_BETWEEN(279, 281)
        CALL(evt_npc_set_position, STRING("me"), -280, 155, -80)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        CASE_GREATER(281)
        CALL(evt_npc_set_position, STRING("me"), -198, 155, -80)
        CALL(evt_npc_set_ry, STRING("me"), 0)
    END_SWITCH
    RETURN
EVT_END()

s32 iri_19_item_tbl[] = {
    ITEM_WEDDING_RING, -1,
};

EVT_BEGIN(peton_talk_common)
    IF_INT_GREATER(GSW_STORY_PROGRESS, 279)
        SET_VALUE(LSWF(3), 1)
    END_IF
    IF_INT_EQUAL(GSW(1420), 19)
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_19_00"), 0, STRING("ピートン"))
            SET_VALUE(GSWF(5405), 1)
            ELSE
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_19_01"), 0, STRING("ピートン"))
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_19_02"), 0, STRING("ピートン"))
                CALL(evt_win_item_select, 0, PTR(iri_19_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_pouch_remove_item, 73, LW(0))
                    CALL(evt_msg_print, 0, STRING("irai_19_03"), 0, STRING("ピートン"))
                    CALL(evt_item_entry, STRING("mg3"), 93, 0, -1000, 0, 17, -1, 0)
                    CALL(evt_item_get_item, STRING("mg3"))
                    CALL(evt_item_get_item_end_wait, STRING("mg3"))
                    CALL(evt_msg_print, 0, STRING("irai_19_04"), 0, STRING("ピートン"))
                    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), 32)
                    CALL(evt_npc_set_ry, STRING("ピートン"), 270)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_wait_pera, STRING("ピートン"))
                    CALL(evt_msg_print, 0, STRING("irai_19_05"), 0, STRING("ピートン"))
                    CALL(evt_npc_move_position, STRING("ピートン"), 224, 478, 0, 150, 0)
                    CALL(evt_npc_move_position, STRING("ピートン"), -20, 478, 0, 150, 0)
                    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), 32)
                    SWITCH(GSW_STORY_PROGRESS)
                        CASE_BETWEEN(279, 281)
                        CALL(evt_npc_set_position, STRING("ピートン"), -290, 155, -80)
                        CALL(evt_npc_set_ry, STRING("ピートン"), 90)
                        CASE_GREATER(281)
                        CALL(evt_npc_set_position, STRING("ピートン"), -198, 155, -90)
                        CALL(evt_npc_set_ry, STRING("ピートン"), 90)
                    END_SWITCH
                    SET_VALUE(GSW(1420), 0)
                    SET_VALUE(GSWF(5393), 1)
                    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
                END_IF
            END_IF
        END_IF
        RETURN
    END_IF
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(120)
        CALL(evt_msg_print, 0, STRING("mac_2_044_04"), 0, STRING("me"))
        CASE_EQUAL(278)
        CALL(evt_msg_print, 0, STRING("mac_5_045"), 0, STRING("ピートン"))
        CASE_LESS(352)
        CALL(evt_msg_print, 0, STRING("gor_03_05_01"), 0, STRING("ピートン"))
        CASE_LESS(382)
        CALL(evt_msg_print, 0, STRING("gor_03_05_02"), 0, STRING("ピートン"))
        CASE_LESS(403)
        CALL(evt_msg_print, 0, STRING("gor_03_05_03"), 0, STRING("ピートン"))
        CASE_GREATER_OR_EQUAL(403)
        CALL(evt_msg_print, 0, STRING("gor_03_05_04"), 0, STRING("ピートン"))
    END_SWITCH
    IF_INT_GREATER(GSW_STORY_PROGRESS, 279)
        SET_VALUE(LSWF(3), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(peton_talk)
    CALL_CHILD(PTR(peton_talk_common))
    RETURN
EVT_END()

EVT_BEGIN(peton_desk_talk)
    IF_INT_EQUAL(GSW(1420), 19)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    CALL(evt_npc_flag_check, STRING("ピートン"), 16777216, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("mario"))
        END_INLINE_EVENT
    END_IF
    CALL_CHILD(PTR(peton_talk_common))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_gurume)
    CALL(evt_npc_set_position, STRING("me"), -160, 0, 480)
    CALL(evt_lottery_error_check, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_eff_fukidashi, 1, STRING("husei_huki"), STRING("me"), 5, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_gurume)
    CALL_CHILD(PTR(kuzi_bomb_talk))
    RETURN
EVT_END()

static EVT_BEGIN(luigi_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(164, 214)
        CALL(evt_npc_set_position, STRING("me"), 440, 0, -20)
        CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(luigi_talk)
    CALL_CHILD(PTR(luigi_gor_first_talk))
    RETURN
EVT_END()

static EVT_BEGIN(luigi_npcEnt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(164, 214)
        CALL(evt_npc_entry, STRING("チェリー"), STRING("c_lp_bom"))
        CALL(evt_npc_set_tribe, STRING("チェリー"), STRING("チェリー"))
        CALL(evt_npc_setup, PTR(cherryEnt))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(cherry_init)
    CALL(evt_npc_set_position, STRING("me"), 480, 0, -20)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(cherry_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_02"), 0, STRING("me"))
    RETURN
EVT_END()

NpcSetupInfo cherryEnt[] = {
    {
        /*"チェリー"*/"\x83\x60\x83\x46\x83\x8A\x81\x5B", // name
        0x80600, // flags
        0, // reactionFlags
        cherry_init, // initEvent
        NULL, // regularEvent
        cherry_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(bteresa_n_fadein)
    CALL(evt_npc_get_position, STRING("バニーテレサ"), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_GAME_TELESA_MOVE2"), LW(1), LW(2), LW(3), 0)
    SET_VALUE(LW(1), LW(0))
    LOOP(LW(0))
        SET_VALUE(LW(2), LW(1))
        SUBTRACT(LW(2), LW(0))
        ADD(LW(2), 1)
        MULTIPLY(LW(2), 255)
        DIVIDE(LW(2), LW(1))
        CALL(evt_npc_set_color, STRING("バニーテレサ"), 255, 255, 255, LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(bteresa_n_fadeout)
    CALL(evt_npc_get_position, STRING("バニーテレサ"), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_GAME_TELESA_MOVE1"), LW(1), LW(2), LW(3), 0)
    SET_VALUE(LW(1), LW(0))
    LOOP(LW(0))
        SET_VALUE(LW(2), LW(0))
        SUBTRACT(LW(2), 1)
        MULTIPLY(LW(2), 255)
        DIVIDE(LW(2), LW(1))
        CALL(evt_npc_set_color, STRING("バニーテレサ"), 255, 255, 255, LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(exchange_return_sub)
    IF_INT_NOT_EQUAL(LW(11), 0)
        CALL(evt_cam3d_evt_set, FLOAT(-541.0f), FLOAT(125.0f), FLOAT(-407.0f), FLOAT(-197.0f), FLOAT(64.0f), FLOAT(-407.0f), 600, 11)
        WAIT_MS(600)
    END_IF
    CALL(evt_npc_get_position, STRING("バニーテレサ"), LW(6), LW(7), LW(10))
    CALL(evt_npc_get_dir, STRING("バニーテレサ"), LW(9))
    SET_VALUE(LW(0), 48)
    CALL_CHILD(PTR(bteresa_n_fadeout))
    IF_INT_NOT_EQUAL(LW(11), 0)
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 30)
    SUBTRACT(LW(2), 20)
    CALL(evt_npc_set_position, STRING("バニーテレサ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("バニーテレサ"), 180)
    SET_VALUE(LW(0), 48)
    CALL_CHILD(PTR(bteresa_n_fadein))
    CALL(yuugijou_get_montemedal_keep, LW(1))
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_90"), 0, STRING("バニーテレサ"), LW(1))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("bunny_teresa_90"), LW(1))
        CALL(evt_msg_print, 1, LW(14), 0, STRING("バニーテレサ"))
    END_SWITCH
#else
    CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_90"), 0, STRING("バニーテレサ"), LW(1))
#endif
    CALL(yuugijou_montemedal_return)
    SET_VALUE(LW(0), 48)
    CALL_CHILD(PTR(bteresa_n_fadeout))
    CALL(evt_npc_set_position, STRING("バニーテレサ"), LW(6), LW(7), LW(10))
    CALL(evt_npc_set_ry, STRING("バニーテレサ"), LW(9))
    SET_VALUE(LW(0), 48)
    IF_INT_EQUAL(LW(11), 0)
        CALL_CHILD(PTR(bteresa_n_fadein))
        ELSE
        BEGIN_INLINE_EVENT
            CALL_CHILD(PTR(bteresa_n_fadein))
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(exchange_keep_sub)
    CALL(yuugijou_get_takeoutflag, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(yuugijou_get_montemedal, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_cam3d_evt_set, FLOAT(-541.0f), FLOAT(125.0f), FLOAT(-407.0f), FLOAT(-197.0f), FLOAT(64.0f), FLOAT(-407.0f), 600, 11)
            WAIT_MS(600)
            CALL(evt_npc_get_position, STRING("バニーテレサ"), LW(6), LW(7), LW(10))
            CALL(evt_npc_get_dir, STRING("バニーテレサ"), LW(9))
            SET_VALUE(LW(0), 48)
            CALL_CHILD(PTR(bteresa_n_fadeout))
            CALL(evt_cam3d_evt_off, 0, 11)
            WAIT_FRAMES(1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 20)
            SUBTRACT(LW(2), 30)
            CALL(evt_npc_set_position, STRING("バニーテレサ"), LW(0), LW(1), LW(2))
            CALL(evt_npc_set_ry, STRING("バニーテレサ"), 180)
            SET_VALUE(LW(0), 48)
            CALL_CHILD(PTR(bteresa_n_fadein))
            CALL(yuugijou_get_montemedal, LW(1))
#ifdef VERSION_G8ME01
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_91"), 0, STRING("バニーテレサ"), LW(1))
                CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("bunny_teresa_91"), LW(1))
                CALL(evt_msg_print, 1, LW(14), 0, STRING("バニーテレサ"))
            END_SWITCH
#else
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_91"), 0, STRING("バニーテレサ"), LW(1))
#endif
            CALL(yuugijou_montemedal_keep)
            SET_VALUE(LW(0), 48)
            CALL_CHILD(PTR(bteresa_n_fadeout))
            CALL(evt_npc_set_position, STRING("バニーテレサ"), LW(6), LW(7), LW(10))
            CALL(evt_npc_set_ry, STRING("バニーテレサ"), LW(9))
            SET_VALUE(LW(0), 1)
            CALL_CHILD(PTR(bteresa_n_fadein))
            ELSE
            CALL(yuugijou_montemedal_keep)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(gor_03_dokan_init_evt)
    CALL(evt_hit_bind_mapobj, STRING("A_dokan_l_02"), STRING("S_dokan_1"))
    IF_INT_NOT_EQUAL(GSWF(1229), 0)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1"), 0, 0, 0)
        ELSE
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1"), 0, -31, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1_kage"), 0, -1000, 0)
    END_IF
    CALL(evt_hit_bind_update, STRING("A_dokan_l_02"))
    RETURN
EVT_END()

EVT_BEGIN(gor_03_dokan_up_evt)
    CALL(evt_mapobj_get_position, STRING("S_dokan_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_vol, LW(3), 76)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1_kage"), 0, 0, 0)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 60)
    LOOP(LW(0))
        SET_FLOAT_VALUE(LW(1), LW(0))
        SUBTRACT_FLOAT(LW(1), 1)
        MULTIPLY_FLOAT(LW(1), -31)
        DIVIDE_FLOAT(LW(1), 60)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1"), 0, LW(1), 0)
        CALL(evt_hit_bind_update, STRING("A_dokan_l_02"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(gor_03_dokan_down_evt)
    CALL(evt_mapobj_get_position, STRING("S_dokan_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_GOOUT1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_vol, LW(3), 76)
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1_kage"), 0, -1000, 0)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 60)
    LOOP(LW(0))
        SET_FLOAT_VALUE(LW(1), 60)
        ADD_FLOAT(LW(1), 1)
        SUBTRACT_FLOAT(LW(1), LW(0))
        MULTIPLY_FLOAT(LW(1), -31)
        DIVIDE_FLOAT(LW(1), 60)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan_1"), 0, LW(1), 0)
        CALL(evt_hit_bind_update, STRING("A_dokan_l_02"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

s32 exchange_ryokin_medal_tbl[] = {
    3, 1, 15, 5, 30, 10, 3, 1,
    15, 5, 30, 10, 3, 1, 15, 5,
    30, 10, 3, 1, 15, 5, 30, 10,
    3, 1, 15, 5, 30, 10, 3, 1,
    15, 5, 30, 10, 3, 1, 15, 5,
    30, 10, 3, 1, 15, 5, 30, 10,
    3, 1, 15, 5, 30, 10,
};

EVT_BEGIN(bteresa_talk_sub_minigame)
    SET_VALUE(LW(15), LW(0))
    LOOP(0)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_34"))
        CALL(evt_msg_select, 0, STRING("bunny_teresa_yesno2"))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("bunny_teresa_48"))
            RETURN
            ELSE
            IF_INT_EQUAL(LW(0), 2)
                CALL(evt_msg_continue)
                SWITCH(LW(15))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, 0, STRING("bunny_teresa_38"), 0, STRING("me"))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 0, STRING("bunny_teresa_39"), 0, STRING("me"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING("bunny_teresa_40"), 0, STRING("me"))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 0, STRING("bunny_teresa_41"), 0, STRING("me"))
                END_SWITCH
                ELSE
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_35"))
        RETURN
    END_IF
    MULTIPLY(LW(1), -1)
    CALL(yuugijou_add_montemedal, 0, LW(1))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(evt_msg_print_add, 0, STRING("bunny_teresa_36"))
    WAIT_MS(500)
    CALL_CHILD(PTR(gor_03_dokan_up_evt))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mapobj_get_position, STRING("S_dokan_1"), LW(3), LW(4), LW(5))
    CALL(mario_walkpoint_enzan, LW(0), LW(2), LW(3), LW(5), FLOAT(40.0f), LW(0), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(3), 30, LW(5), 500, 0, 10)
    CALL(evt_msg_print, 0, STRING("bunny_teresa_37"), 0, STRING("me"))
    SWITCH(LW(15))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("A_dokan_02"), PTR(yuu_00_data))
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("A_dokan_02"), PTR(yuu_03_data))
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("A_dokan_02"), PTR(yuu_02_data))
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("A_dokan_02"), PTR(yuu_01_data))
        SET_VALUE(LW(0), STRING("A_dokan_02"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_SWITCH
    CALL(yuugijou_montemedal_disp_onoff, 0)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_bgmoff_f, 513, 2000)
    CALL(evt_snd_envoff_f, 512, 2000)
    SET_VALUE(LW(1), STRING("A_dokan_02"))
    CALL(evt_bero_id_filter, LW(1))
    CALL(evt_bero_get_info_num, LW(1))
    CALL_CHILD(PTR(bero_case_exec_pipe))
    RETURN
EVT_END()

EVT_BEGIN(bteresa_talk_sub_keihin)
    CALL(evt_badgeShop_bteresa_get_kind_cnt, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_64"))
        GOTO(0)
    END_IF
    IF_INT_EQUAL(GSWF(1228), 0)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_60"))
        SET_VALUE(GSWF(1228), 1)
        ELSE
        CALL(evt_msg_continue)
    END_IF
    LABEL(1)
    CALL(yuugijou_set_yupflag, 1)
    CALL(evt_win_other_select, 16)
    CALL(yuugijou_set_yupflag, 0)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_74"), 0, STRING("me"))
        GOTO(0)
    END_IF
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_msg_print, 0, STRING("bunny_teresa_65"), 0, STRING("me"))
        GOTO(1)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(1), 240)
        CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_67"), 0, STRING("me"), LW(2), LW(3), LW(4))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bunny_teresa_67"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("me"), LW(4))
        END_SWITCH
        ELSE
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_66"), 0, STRING("me"), LW(2), LW(3))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bunny_teresa_66"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print, 1, LW(14), 0, STRING("me"))
        END_SWITCH
#else
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_67"), 0, STRING("me"), LW(3), LW(2), LW(4))
            ELSE
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_67"), 0, STRING("me"), LW(2), LW(3), LW(4))
        END_IF
        ELSE
        CALL(evt_sub_get_language, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_66"), 0, STRING("me"), LW(3), LW(2))
            ELSE
        END_IF
        CALL(evt_msg_print_insert, 0, STRING("bunny_teresa_66"), 0, STRING("me"), LW(2), LW(3))
#endif
    END_IF
    CALL(evt_msg_select, 0, STRING("bunny_teresa_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_68"))
        GOTO(0)
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        IF_INT_GREATER_OR_EQUAL(LW(1), 240)
            CALL(evt_msg_print_add, 0, STRING("bunny_teresa_72"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("bunny_teresa_73"))
        END_IF
        GOTO(0)
    END_IF
    MULTIPLY(LW(3), -1)
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(evt_badgeShop_bteresa_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_69"))
        GOTO(0)
    END_IF
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_69"))
        GOTO(0)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("bunny_teresa_70"))
    CALL(evt_msg_select, 0, STRING("bunny_teresa_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_71"))
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    GOTO(1)
    LABEL(0)
    RETURN
EVT_END()

EVT_BEGIN(bteresa_table_talk)
    CALL(setmertn, STRING("バニーテレサ"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    IF_INT_EQUAL(GSW(1420), 11)
        CALL(evt_msg_print, 0, STRING("irai_11_001"), 0, STRING("me"))
        CALL(yuugijou_get_takeoutflag, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(yuugijou_montemedal_return)
        END_IF
        CALL(yuugijou_montemedal_disp_onoff, 1)
        CALL(yuugijou_add_montemedal, 0, 10)
        CALL(yuugijou_montemedal_wait)
        CALL(evt_msg_print, 0, STRING("irai_11_003"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_N_B"))
        WAIT_MS(600)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("irai_11_004"), 0, STRING("me"))
        SET_VALUE(GSW(1420), 0)
        SET_VALUE(GSWF(5385), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_party_run, 0)
        RETURN
    END_IF
    CALL(yuugijou_get_takeoutflag, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(yuugijou_get_montemedal_keep, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(11), 0)
            CALL_CHILD(PTR(exchange_return_sub))
            ELSE
            CALL(yuugijou_montemedal_return)
        END_IF
    END_IF
    SET_VALUE(LW(1), 0)
    IF_INT_EQUAL(GSWF(1232), 0)
        CALL(evt_pouch_check_item, 91, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(1), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1233), 0)
        CALL(evt_pouch_check_item, 92, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(1), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1234), 0)
        CALL(evt_pouch_check_item, 93, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(1), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1235), 0)
        CALL(evt_pouch_check_item, 94, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(1), 1)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_42"), 0, STRING("me"))
        IF_INT_EQUAL(GSWF(1232), 0)
            CALL(evt_pouch_check_item, 91, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SET_VALUE(GSWF(1232), 1)
                CALL(evt_msg_print, 0, STRING("bunny_teresa_43"), 0, 0)
            END_IF
        END_IF
        IF_INT_EQUAL(GSWF(1235), 0)
            CALL(evt_pouch_check_item, 94, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SET_VALUE(GSWF(1235), 1)
                CALL(evt_msg_print, 0, STRING("bunny_teresa_44"), 0, 0)
            END_IF
        END_IF
        IF_INT_EQUAL(GSWF(1234), 0)
            CALL(evt_pouch_check_item, 93, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SET_VALUE(GSWF(1234), 1)
                CALL(evt_msg_print, 0, STRING("bunny_teresa_45"), 0, 0)
            END_IF
        END_IF
        IF_INT_EQUAL(GSWF(1233), 0)
            CALL(evt_pouch_check_item, 92, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SET_VALUE(GSWF(1233), 1)
                CALL(evt_msg_print, 0, STRING("bunny_teresa_46"), 0, 0)
            END_IF
        END_IF
        SET_VALUE(GSWF(1227), 1)
        SET_VALUE(LW(14), 1)
        ELSE
        SET_VALUE(LW(14), 0)
        IF_INT_NOT_EQUAL(GSWF(1232), 0)
            SET_VALUE(LW(14), 1)
        END_IF
        IF_INT_NOT_EQUAL(GSWF(1235), 0)
            SET_VALUE(LW(14), 1)
        END_IF
        IF_INT_NOT_EQUAL(GSWF(1234), 0)
            SET_VALUE(LW(14), 1)
        END_IF
        IF_INT_NOT_EQUAL(GSWF(1233), 0)
            SET_VALUE(LW(14), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1227), 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, 403)
            IF_INT_EQUAL(GSWF(1226), 0)
                CALL(evt_msg_print, 0, STRING("bunny_teresa_00"), 0, STRING("me"))
                SET_VALUE(GSWF(1226), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("bunny_teresa_01"), 0, STRING("me"))
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("bunny_teresa_01_1"), 0, STRING("me"))
        END_IF
        SET_VALUE(GSWF(1227), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("bunny_teresa_02"), 0, STRING("me"))
    END_IF
    CALL(evt_exchange_msg_alloc)
    CALL(minigameselect_msg_set, STRING("bunny_teresa_03"), LW(0))
    CALL(evt_msg_select, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 5)
        LOOP(0)
            IF_INT_NOT_EQUAL(GSWF(1232), 0)
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                ELSE
                ADD(LW(0), 1)
            END_IF
            IF_INT_NOT_EQUAL(GSWF(1235), 0)
                IF_INT_EQUAL(LW(0), 1)
                    BREAK_LOOP
                END_IF
                ELSE
                ADD(LW(0), 1)
            END_IF
            IF_INT_NOT_EQUAL(GSWF(1234), 0)
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                ELSE
                ADD(LW(0), 1)
            END_IF
            IF_INT_NOT_EQUAL(GSWF(1233), 0)
                IF_INT_EQUAL(LW(0), 3)
                    BREAK_LOOP
                END_IF
                ELSE
                ADD(LW(0), 1)
            END_IF
            BREAK_LOOP
        END_LOOP
    END_IF
    IF_INT_EQUAL(LW(0), 5)
        CALL(evt_msg_print_add, 0, STRING("bunny_teresa_13"))
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(0), 4)
        CALL_CHILD(PTR(bteresa_talk_sub_keihin))
        ELSE
        CALL_CHILD(PTR(bteresa_talk_sub_minigame))
    END_IF
    LABEL(0)
    CALL(evt_exchange_msg_free)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(bteresa_init)
    CALL(evt_run_case_evt, 9, 1, STRING("A_hako_tele_01"), 0, PTR(bteresa_table_talk), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_hako_tele_02"), 0, PTR(bteresa_table_talk), 0)
    RETURN
EVT_END()

EVT_BEGIN(bteresa_talk)
    SET_VALUE(LW(1), GSW(117))
    SET_VALUE(LW(0), 0)
    ADD(LW(0), GSWF(1240))
    ADD(LW(0), GSWF(1241))
    ADD(LW(0), GSWF(1242))
    ADD(LW(0), GSWF(1243))
    IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
        ADD(LW(1), 1)
    END_IF
    SWITCH(LW(1))
        CASE_EQUAL(0)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11"), 0, STRING("me"))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11"), 0, STRING("me"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11_1"), 0, STRING("me"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11_2"), 0, STRING("me"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11_3"), 0, STRING("me"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_11_4"), 0, STRING("me"))
    END_SWITCH
    SET_VALUE(GSW(117), LW(1))
    RETURN
EVT_END()

EVT_BEGIN(kyaku1_init)
    RETURN
EVT_END()

EVT_BEGIN(kyaku1_talk)
    SET_VALUE(LF(0), 0)
    CALL(evt_pouch_check_item, 91, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 92, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 93, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 94, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_msg_print, 0, STRING("kyaku1_00_00"), 0, STRING("me"))
        ELSE
        CALL(evt_msg_print, 0, STRING("kyaku1_00"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kyaku4_init)
    RETURN
EVT_END()

EVT_BEGIN(kyaku4_talk)
    SET_VALUE(LF(0), 0)
    CALL(evt_pouch_check_item, 91, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 92, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 93, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(evt_pouch_check_item, 94, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_msg_print, 0, STRING("kyaku4_00_00"), 0, STRING("me"))
        ELSE
        CALL(evt_msg_print, 0, STRING("kyaku4_00"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

f32 montemedalzpos[] = {
    -235.0f, -160.0f,
};

EVT_BEGIN(slot_fall_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

static inline Vec fall_pos0(void) { Vec v = {-113.0f, 70.0f, 0.0f}; return v; } /* @place "slot_mess_01" */
static inline Vec fall_vel0(void) { Vec v = {0.0f, 10.0f, 0.0f}; return v; } /* @place "slot_mess_01" */

EVT_BEGIN(slot_set)
    CALL(goryuugijo_get_dataadr, 1, LW(0))
    CALL(evt_goryuugijo_set_nowslot, LW(0), LW(8))
    CALL(goryuugijo_get_slot_phase, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
#ifdef VERSION_G8ME01
    CALL(evt_party_stop, 0)
#endif
    CALL(yuugijou_get_takeoutflag, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(yuugijou_get_montemedal_keep, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(11), 2)
            CALL_CHILD(PTR(exchange_return_sub))
            ELSE
            CALL(yuugijou_montemedal_return)
        END_IF
    END_IF
#ifdef VERSION_G8ME01
    BEGIN_SIBLING_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_move_pos2, 0, -225, LW(2), FLOAT(60.0f))
    END_SIBLING_EVENT
#endif
    SWITCH(LW(8))
        CASE_EQUAL(0)
        CALL(evt_cam3d_evt_set, FLOAT(-393.8994f), FLOAT(113.09961f), FLOAT(-235.0f), FLOAT(-197.0f), FLOAT(78.69922f), FLOAT(-235.0f), 600, 11)
        WAIT_MS(600)
        CASE_EQUAL(1)
        CALL(evt_cam3d_evt_set, FLOAT(-393.8994f), FLOAT(113.09961f), FLOAT(-160.0f), FLOAT(-197.0f), FLOAT(78.69922f), FLOAT(-160.0f), 600, 11)
        WAIT_MS(600)
    END_SWITCH
    LOOP(0)
        CALL(yuugijou_get_montemedal, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("slot_mess_01"), 0, 0)
            GOTO(0)
        END_IF
        CALL(evt_msg_select, 0, STRING("slot_mess_00"))
        IF_INT_EQUAL(LW(0), 1)
            GOTO(0)
        END_IF
        CALL(yuugijou_add_montemedal, 0, -1)
        CALL(goryuugijo_get_dataadr, 1, LW(0))
        CALL(goryuugijo_set_slot_phase, LW(0), 1)
        LOOP(0)
            CALL(goryuugijo_get_slot_phase, LW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                WAIT_MS(300)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(1), 8)
                GOTO(0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    LABEL(0)
    CALL(evt_cam3d_evt_off, 600, 11)
    WAIT_MS(600)
#ifdef VERSION_G8ME01
    CALL(evt_party_run, 0)
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(slot1_set)
    SET_VALUE(LW(8), 0)
    CALL_CHILD(PTR(slot_set))
    RETURN
EVT_END()

EVT_BEGIN(slot2_set)
    SET_VALUE(LW(8), 1)
    CALL_CHILD(PTR(slot_set))
    RETURN
EVT_END()

SlotDrum drumname[2] = { // flat initializer, 17 names per slot machine
    "S_srot01_dram_01",
    "S_srot01_dram_02",
    "S_srot01_dram_03",
    "S_srot01_reber",
    "A_srot01",
    "S_srot01_m_01",
    "S_srot01_s_01",
    "S_srot01_7_01",
    "S_srot01_m_02",
    "S_srot01_s_02",
    "S_srot01_7_02",
    "S_srot01_m_03",
    "S_srot01_s_03",
    "S_srot01_7_03",
    "neon_01",
    "neon_03",
    "neon_02",
    "S_srot02_dram_01",
    "S_srot02_dram_02",
    "S_srot02_dram_03",
    "S_srot02_reber",
    "A_srot02",
    "S_srot02_m_01",
    "S_srot02_s_01",
    "S_srot02_7_01",
    "S_srot02_m_02",
    "S_srot02_s_02",
    "S_srot02_7_02",
    "S_srot02_m_03",
    "S_srot02_s_03",
    "S_srot02_7_03",
    "neon_04",
    "neon_06",
    "neon_05",
};

EvtWord* slotevttbl[] = {
    slot1_set, slot2_set,
};

s32 slotgara[] = {
    0, 0, 2, 0, 1, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 2,
    1, 0, 0, 2, 0, 0, 1, 0,
    0, 0, 2, 0, 1, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 2,
    1, 0, 0, 2, 0, 0, 1, 0,
};

s32 upsidegara[] = {
    0, 0, 2, 0, 1, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 2,
    1, 0, 0, 2, 0, 0, 1, 0,
    0, 0, 2, 0, 1, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 2,
    1, 0, 0, 2, 0, 0, 1, 0,
};

typedef struct I3 { s32 v[3]; } I3;
static const I3 disp_slot_icons = {{ICON_1F3, ICON_1F3, ICON_1F3}};

EVT_BEGIN(ranking_set)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, FLOAT(-541.8994f), FLOAT(115.5f), FLOAT(-115.0f), FLOAT(-197.0f), FLOAT(56.799805f), FLOAT(-115.0f), 600, 11)
    WAIT_MS(600)
    LOOP(0)
        CALL(evt_msg_print, 0, STRING("ranking_mess_00"), 0, 0)
        CALL(evt_msg_select, 0, STRING("ranking_mess_01"))
        CALL(evt_msg_continue)
        IF_INT_EQUAL(LW(0), 5)
            GOTO(0)
        END_IF
        CALL(evt_yuuranking_alloc, 0, 0, LW(0), 2, 0, 0, 0, 0)
        WAIT_FRAMES(1)
        CALL(yuuranking_wait_button)
        CALL(evt_yuuranking_free)
    END_LOOP
    LABEL(0)
    CALL(evt_cam3d_evt_off, 600, 11)
    WAIT_MS(600)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(exchange_set)
    CALL(evt_mario_key_onoff, 0)
    CALL(yuugijou_get_takeoutflag, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(yuugijou_get_montemedal_keep, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(11), 1)
            CALL_CHILD(PTR(exchange_return_sub))
            SET_VALUE(LW(0), 0)
            ELSE
            CALL(yuugijou_montemedal_return)
            SET_VALUE(LW(0), 0)
        END_IF
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_cam3d_evt_set, FLOAT(-541.5996f), FLOAT(147.39941f), FLOAT(-314.5996f), FLOAT(-197.0f), FLOAT(86.899414f), FLOAT(-314.5996f), 600, 11)
        WAIT_MS(600)
    END_IF
    CALL(evt_msg_toge, 2, FLOAT(-147.0f), FLOAT(82.19922f), FLOAT(-307.8994f))
    IF_INT_EQUAL(GSWF(1225), 0)
        CALL(exchange_ryokin_medal, 0, LW(1), LW(2))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("exchange_mess_00"), 0, 0, LW(1), LW(2))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("exchange_mess_00"), 0, 0)
        END_SWITCH
#else
        CALL(evt_msg_print_insert, 0, STRING("exchange_mess_00"), 0, 0, LW(1), LW(2))
#endif
        SET_VALUE(GSWF(1225), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("exchange_mess_01"), 0, 0)
    END_IF
    CALL(evt_exchange_msg_alloc)
    CALL(evt_unitwin_alloc, 0)
    CALL(unitwincoin_disp_onoff, 1)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_exchange_msg_set, LW(0), STRING("exchange_mess_02"), LW(1))
        CALL(evt_msg_select, 1, LW(1))
        IF_INT_EQUAL(LW(0), 3)
            CALL(unitwincoin_disp_onoff, 0)
            CALL(evt_unitwin_free)
            CALL(evt_exchange_msg_free)
            CALL(evt_msg_print_add, 0, STRING("exchange_mess_05"))
            GOTO(0)
        END_IF
        CALL(exchange_ryokin_medal, LW(0), LW(1), LW(2))
        CALL(evt_pouch_get_coin, LW(3))
        IF_INT_LESS(LW(3), LW(1))
            CALL(evt_msg_print_add, 0, STRING("exchange_mess_03"))
            CALL(unitwincoin_disp_onoff, 0)
            CALL(evt_unitwin_free)
            CALL(evt_exchange_msg_free)
            GOTO(0)
        END_IF
        CALL(evt_msg_continue)
        MULTIPLY(LW(1), -1)
        CALL(evt_pouch_add_coin, LW(1))
        CALL(unitwincoin_wait)
        CALL(yuugijou_add_montemedal, 0, LW(2))
        CALL(yuugijou_add_montemedal_moto, LW(2))
        CALL(yuugijou_montemedal_wait)
        WAIT_MS(200)
        CALL(evt_msg_toge, 2, FLOAT(-147.0f), FLOAT(82.19922f), FLOAT(-307.8994f))
        CALL(evt_msg_print, 0, STRING("exchange_mess_04"), 0, 0)
    END_LOOP
    LABEL(0)
    CALL(evt_cam3d_evt_off, 600, 11)
    WAIT_MS(600)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(roten_4_init)
    CALL(evt_npc_set_position, STRING("me"), 0, -5, 335)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 131088)
    RETURN
EVT_END()

EVT_BEGIN(roten_4_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(roten_4_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_016"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_017"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_018"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_019"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_020"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_021"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_022"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_023"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kinopiko_a_init)
    CALL(evt_npc_set_position, STRING("me"), -145, 0, -110)
    RETURN
EVT_END()

EVT_BEGIN(kinopiko_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_048"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_049"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_050"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_051"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_052"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_053"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_054"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_055"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kinopiko_b_init)
    CALL(evt_npc_set_position, STRING("me"), 155, 0, -50)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 131088)
    RETURN
EVT_END()

EVT_BEGIN(kinopiko_b_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(kinopiko_b_talk)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_072_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_072"), 0, STRING("me"))
        END_IF
        CASE_LESS(164)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_073_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_073"), 0, STRING("me"))
        END_IF
        CASE_LESS(215)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_074_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_074"), 0, STRING("me"))
        END_IF
        CASE_LESS(261)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_075_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_075"), 0, STRING("me"))
        END_IF
        CASE_LESS(339)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_076_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_076"), 0, STRING("me"))
        END_IF
        CASE_LESS(373)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_077_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_077"), 0, STRING("me"))
        END_IF
        CASE_LESS(401)
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_078_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_078"), 0, STRING("me"))
        END_IF
        CASE_DEFAULT
        IF_INT_EQUAL(LW(0), 5)
            CALL(evt_msg_print, 0, STRING("gor_03_079_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("gor_03_079"), 0, STRING("me"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kinopio_a_init)
    CALL(evt_npc_set_position, STRING("me"), 330, 128, -170)
    RETURN
EVT_END()

EVT_BEGIN(kinopio_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_024"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_025"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_026"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_027"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_028"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_029"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_030"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_031"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kokinopio_a_init)
    CALL(evt_npc_set_position, STRING("me"), 460, 37, -152)
    RETURN
EVT_END()

EVT_BEGIN(kokinopio_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_032"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_033"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_034"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_035"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_036"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_037"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_038"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_039"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kokinopio_b_init)
    CALL(evt_npc_set_position, STRING("me"), 35, -5, 300)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), 131088)
    RETURN
EVT_END()

EVT_BEGIN(kokinopio_b_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kokinopio_b_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_040"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_041"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_042"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_043"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_044"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_045"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_046"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_047"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_a_init)
    CALL(evt_npc_set_position, STRING("me"), 110, 146, -310)
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_056"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_057"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_058"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_059"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_060"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_061"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_062"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_063"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuribo_1_init)
    CALL(evt_npc_set_position, STRING("me"), 470, 10, 295)
    RETURN
EVT_END()

EVT_BEGIN(kuribo_1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_03_064"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_03_065"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_03_066"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_03_067"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_03_068"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_03_069"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_03_070"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_03_071"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"マフィアボス"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x83\x7B\x83\x58", // name
        0x80600, // flags
        0, // reactionFlags
        maffiaboss_init, // initEvent
        NULL, // regularEvent
        maffiaboss_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マフィアボスカジノ"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x83\x7B\x83\x58\x83\x4A\x83\x57\x83\x6D", // name
        0x80600, // flags
        0, // reactionFlags
        maffiaboss_casino_init, // initEvent
        NULL, // regularEvent
        maffiaboss_casino_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マフィア１"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        maffia1_init, // initEvent
        NULL, // regularEvent
        maffia1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マフィア２"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x82\x51", // name
        0x80600, // flags
        0, // reactionFlags
        maffia2_init, // initEvent
        NULL, // regularEvent
        maffia2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マフィア３"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x82\x52", // name
        0x80600, // flags
        0, // reactionFlags
        maffia3_init, // initEvent
        NULL, // regularEvent
        maffia3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"モニー"*/"\x83\x82\x83\x6A\x81\x5B", // name
        0x80600, // flags
        0, // reactionFlags
        mony_init, // initEvent
        NULL, // regularEvent
        mony_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ピートン"*/"\x83\x73\x81\x5B\x83\x67\x83\x93", // name
        0x80600, // flags
        0, // reactionFlags
        peton_init, // initEvent
        NULL, // regularEvent
        peton_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"グルメボム兵"*/"\x83\x4F\x83\x8B\x83\x81\x83\x7B\x83\x80\x95\xBA", // name
        0x40000600, // flags
        0, // reactionFlags
        init_gurume, // initEvent
        NULL, // regularEvent
        talk_gurume, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57", // name
        0x80600, // flags
        0, // reactionFlags
        luigi_init, // initEvent
        NULL, // regularEvent
        luigi_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"バニーテレサ"*/"\x83\x6F\x83\x6A\x81\x5B\x83\x65\x83\x8C\x83\x54", // name
        0x600, // flags
        0, // reactionFlags
        bteresa_init, // initEvent
        NULL, // regularEvent
        bteresa_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"客１"*/"\x8B\x71\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        kyaku1_init, // initEvent
        NULL, // regularEvent
        kyaku1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"客４"*/"\x8B\x71\x82\x53", // name
        0x80600, // flags
        0, // reactionFlags
        kyaku4_init, // initEvent
        NULL, // regularEvent
        kyaku4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        0x200, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロテン４"*/"\x83\x8D\x83\x65\x83\x93\x82\x53", // name
        0x80600, // flags
        0, // reactionFlags
        roten_4_init, // initEvent
        roten_4_regl, // regularEvent
        roten_4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ女Ａ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x82\x60", // name
        0x80600, // flags
        0, // reactionFlags
        kinopiko_a_init, // initEvent
        NULL, // regularEvent
        kinopiko_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ女Ｂ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x82\x61", // name
        0x80600, // flags
        0, // reactionFlags
        kinopiko_b_init, // initEvent
        kinopiko_b_regl, // regularEvent
        kinopiko_b_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオＡ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x82\x60", // name
        0x80600, // flags
        0, // reactionFlags
        kinopio_a_init, // initEvent
        NULL, // regularEvent
        kinopio_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ子供Ａ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x8B\x9F\x82\x60", // name
        0x80600, // flags
        0, // reactionFlags
        kokinopio_a_init, // initEvent
        NULL, // regularEvent
        kokinopio_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ子供Ｂ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x8B\x9F\x82\x61", // name
        0x80600, // flags
        0, // reactionFlags
        kokinopio_b_init, // initEvent
        kokinopio_b_regl, // regularEvent
        kokinopio_b_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコＡ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x82\x60", // name
        0x80600, // flags
        0, // reactionFlags
        nokonoko_a_init, // initEvent
        NULL, // regularEvent
        nokonoko_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        kuribo_1_init, // initEvent
        NULL, // regularEvent
        kuribo_1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_door_01_00)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_doa_m"), 6144)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_doa_o"), 6144)
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i01_doa_m"), 0, LW(0), 0)
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_rotate, 1, STRING("S_i01_doa_o"), 0, LW(0), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_01)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01uB"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01sB"), 0, 0, LW(3))
    RETURN
EVT_END()

EVT_BEGIN(evt_door_01_10)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_doa_m"), 6144)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i01_doa_o"), 6144)
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i01_doa_o"), 0, LW(0), 0)
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_rotate, 1, STRING("S_i01_doa_m"), 0, LW(0), 0)
    END_IF
    RETURN
EVT_END()

static const char* door_data_01_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(evt_door_01_0_open)
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(11), 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mobj_check, STRING("pblk2"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("pblk2"), 16386)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_door_01_0_close)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_doa_m"), 6144)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_doa_o"), 6144)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 124)
        CALL(evt_door_end_wait)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_party_stop, 0)
        CALL(evt_msg_print_party, STRING("mac_2_123"))
        SET_VALUE(GSW_STORY_PROGRESS, 125)
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 279)
        SPAWN_EVENT(PTR(majorin_nusumigiki))
    END_IF
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    CALL(evt_mobj_check, STRING("pblk2"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("pblk2"), 16386)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_door_01_1_open)
    CALL(evt_hitobj_onoff, STRING("A_ie_03A"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_gaitou"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_kabe_01"), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_soto"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_04_soto"), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア３"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィアボスカジノ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("グルメボム兵"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("バニーテレサ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("客１"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("客４"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("ロテン４"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピオ女Ａ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピオ女Ｂ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピオＡ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピオ子供Ａ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピオ子供Ｂ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコＡ"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("クリボー１"), 128)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mobj_check, STRING("pblk2"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("pblk2"), 16386)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_door_01_1_close)
    CALL(evt_hitobj_onoff, STRING("A_ie_03A"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_gaitou"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_kabe_01"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_soto"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_04_soto"), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア３"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィアボスカジノ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("グルメボム兵"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("バニーテレサ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("客１"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("客４"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("ロテン４"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピオ女Ａ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピオ女Ｂ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピオＡ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピオ子供Ａ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピオ子供Ｂ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコＡ"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), 128)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_doa_m"), 6144)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i01_doa_o"), 6144)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    CALL(evt_mobj_check, STRING("pblk2"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("pblk2"), 16386)
    END_IF
    RETURN
EVT_END()

DoorParam door_data_01_0 = {
    0, // flags
    3, // type
    1, // se
    "S_i01_doa_m", // mapobj
    "A_ie01_doa_m02", // hitIn
    "A_ie01_doa_m01", // hitOut
    "S_ie_01_soto", // outside
    NULL, // outsideHit
    "S_ie_01_naka", // inside
    "A_ie_01C", // insideHit
    evt_door_01_00, // evtIn
    NULL, // evtOut
    evt_wall_01, // evtWall
    door_data_01_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_01_0_open, // evtOpen
    evt_door_01_0_close, // evtClose
};

DoorParam door_data_01_1 = {
    0, // flags
    3, // type
    1, // se
    "S_i01_doa_o", // mapobj
    "A_ie01_doa_o02", // hitIn
    "A_ie01_doa_o01", // hitOut
    "S_ie_01_soto", // outside
    NULL, // outsideHit
    "S_ie_01_naka", // inside
    "A_ie_01C", // insideHit
    evt_door_01_10, // evtIn
    NULL, // evtOut
    evt_wall_01, // evtWall
    door_data_01_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_01_1_open, // evtOpen
    evt_door_01_1_close, // evtClose
};

EVT_BEGIN(evt_door_01_entry)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_02_00)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_f1_doa01"), 6144)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_f1_doa02"), 6144)
    END_IF
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5996094f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1_doa01"), 0, LW(2), 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1_doa02"), 0, LW(3), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_0)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1B"), 0, 0, LW(2))
    RETURN
EVT_END()

const char* door_data_02_0_npc[] = {
    /*"バニーテレサ"*/"\x83\x6F\x83\x6A\x81\x5B\x83\x65\x83\x8C\x83\x54",
    /*"客１"*/"\x8B\x71\x82\x50",
    /*"客４"*/"\x8B\x71\x82\x53",
    /*"キノピオ女Ａ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8F\x97\x82\x60",
    /*"マフィアボスカジノ"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x83\x7B\x83\x58\x83\x4A\x83\x57\x83\x6D",
    NULL,
};

EVT_BEGIN(door_data_02_0_open_sub)
    WAIT_FRAMES(1)
    CALL(goryuugijo_get_dataadr, 1, LW(0))
    CALL(goryuugijo_door_init_slot, LW(0))
    CALL(mario_keyoff_wait)
    RETURN
EVT_END()

EVT_BEGIN(door_data_02_0_open)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_soto_kai"), 1)
    SPAWN_EVENT(PTR(door_data_02_0_open_sub))
    SET_VALUE(GSWF(1227), 0)
    CALL(yuugijou_set_yupflag, 0)
    CALL(teresa_badge_item_list_init)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_STG0_CSN1"))
    CALL(evt_snd_envoff, 1024)
    RETURN
EVT_END()

EVT_BEGIN(door_data_02_0_close)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_f1_doa01"), 6144)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_f1_doa02"), 6144)
    CALL(yuugijou_get_takeoutflag, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
    END_IF
    CALL(goryuugijo_get_dataadr, 1, LW(0))
    CALL(goryuugijo_set_slot_phase, LW(0), 11)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    CALL(mario_keyoff_wait)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i02_soto_kai"), 1)
    RETURN
EVT_END()

const char* door_data_02_mobj[] = {
    "kururin1",
    NULL,
};

DoorParam door_data_02_0l = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f1_doa01", // mapobj
    "A_ie_02_f1_doa_l02______", // hitIn
    "A_ie_02_f1_doa_l01", // hitOut
    "S_ie_02_soto", // outside
    NULL, // outsideHit
    "S_i02_f1", // inside
    "A_ie_02C", // insideHit
    evt_door_02_00, // evtIn
    NULL, // evtOut
    evt_wall_02_0, // evtWall
    door_data_02_0_npc, // npcs
    NULL, NULL, // mobjs..items
    door_data_02_0_open, // evtOpen
    door_data_02_0_close, // evtClose
};

DoorParam door_data_02_0r = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f1_doa02", // mapobj
    "A_ie_02_f1_doa_r02", // hitIn
    "A_ie_02_f1_doa_r01", // hitOut
    "S_ie_02_soto", // outside
    NULL, // outsideHit
    "S_i02_f1", // inside
    "A_ie_02C", // insideHit
    evt_door_02_00, // evtIn
    NULL, // evtOut
    evt_wall_02_0, // evtWall
    door_data_02_0_npc, // npcs
    door_data_02_mobj, // mobjs
    NULL, // items
    door_data_02_0_open, // evtOpen
    door_data_02_0_close, // evtClose
};

EVT_BEGIN(evt_door_02_10)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2_doa"), 0, LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_11)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2_doa"), 0, 0, LW(2))
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_1)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2sB"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2uB"), 0, 0, LW(3))
    RETURN
EVT_END()

const char* door_data_02_1_npc[] = {
    /*"マフィアボス"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x83\x7B\x83\x58",
    /*"マフィア１"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x82\x50",
    /*"マフィア２"*/"\x83\x7D\x83\x74\x83\x42\x83\x41\x82\x51",
    /*"モニー"*/"\x83\x82\x83\x6A\x81\x5B",
    /*"ピートン"*/"\x83\x73\x81\x5B\x83\x67\x83\x93",
    NULL,
};

EVT_BEGIN(desk_talk_check)
    CALL(desk_talk_init_func)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        CALL(evt_mario_get_name_hitobj_front, LW(10))
        SET_VALUE(LF(10), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(268)
            CALL(evt_npc_get_height, STRING("マフィアボス"), LW(5))
            CALL(evt_npc_get_position, STRING("マフィアボス"), LW(2), LW(3), LW(4))
            SET_VALUE(LW(11), STRING("A_i02_f2_set00C"))
            IF_STR_EQUAL(LW(10), LW(11))
                SET_VALUE(LF(10), 1)
            END_IF
            CASE_LESS(282)
            CALL(evt_npc_get_height, STRING("マフィアボス"), LW(5))
            CALL(evt_npc_get_position, STRING("マフィアボス"), LW(2), LW(3), LW(4))
            SET_VALUE(LW(11), STRING("A_i02_bed_l"))
            IF_STR_EQUAL(LW(10), LW(11))
                SET_VALUE(LF(10), 1)
            END_IF
            SET_VALUE(LW(11), STRING("A_i02_bed_m"))
            IF_STR_EQUAL(LW(10), LW(11))
                SET_VALUE(LF(10), 1)
            END_IF
            CASE_DEFAULT
            CALL(evt_npc_get_height, STRING("ピートン"), LW(5))
            CALL(evt_npc_get_position, STRING("ピートン"), LW(2), LW(3), LW(4))
            SET_VALUE(LW(11), STRING("A_i02_f2_set00C"))
            IF_STR_EQUAL(LW(10), LW(11))
                SET_VALUE(LF(10), 1)
            END_IF
        END_SWITCH
        SET_VALUE(LW(0), 0)
        IF_INT_EQUAL(LF(10), 1)
            IF_INT_EQUAL(LSWF(3), 0)
                CALL(get_mario_dispReqDir, LW(9))
                CALL(evt_mario_party_use_check, LW(13))
                CALL(evt_mario_get_name_hitobj_front, LW(14))
                IF_INT_NOT_EQUAL(LW(13), 4)
                    IF_STR_EQUAL(LW(14), STRING("A_i02_bed_m"))
                        IF_INT_EQUAL(LW(9), 0)
                            SET_VALUE(LW(0), 1)
                            SET_VALUE(LW(15), 5)
                        END_IF
                    END_IF
                    IF_INT_EQUAL(LW(9), 180)
                        SET_VALUE(LW(0), 1)
                        SET_VALUE(LW(15), 5)
                    END_IF
                END_IF
            END_IF
        END_IF
        IF_INT_NOT_EQUAL(LW(15), 0)
            SET_VALUE(LW(0), 1)
            ADD(LW(15), -1)
        END_IF
        ADD(LW(3), LW(5))
        CALL(desk_talk_func, LW(0), LW(1), LW(2), LW(3), LW(4))
        SET_VALUE(LW(1), LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_1_open)
    SET_VALUE(GF(5), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 278)
            CALL(evt_snd_bgmoff, 1024)
            CALL(evt_snd_bgmon, 1, STRING("BGM_STG0_CSN1"))
            CALL(evt_snd_envoff, 1024)
            ELSE
            CALL(evt_snd_bgmoff_f, 512, 2000)
            CALL(evt_snd_envoff, 1024)
        END_IF
    END_INLINE_EVENT
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(268)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_bed"), 1)
        CALL(evt_hitobj_onoff, STRING("A_i02_bed"), 1, 0)
        CASE_LESS(279)
        CALL(evt_npc_set_anim, STRING("マフィアボス"), STRING("S_2"))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 278)
            CALL(evt_mario_dispflag_onoff, 1, 4)
            CALL(evt_mario_dispflag_onoff, 1, 2048)
            CALL(evt_door_end_wait)
            CALL(evt_mario_dispflag_onoff, 0, 2048)
            CALL(evt_mario_dispflag_onoff, 0, 4)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL_CHILD(PTR(maffiaboss_train_ticket_init))
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_f2_set00C"), 1)
        CALL(evt_hitobj_onoff, STRING("Ai02_tke"), 1, 0)
        CASE_LESS(282)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_f2_set00C"), 1)
        CALL(evt_hitobj_onoff, STRING("Ai02_tke"), 1, 0)
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_bed"), 1)
        CALL(evt_hitobj_onoff, STRING("A_i02_bed"), 1, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_1_close)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 278)
        IF_INT_EQUAL(GF(5), 0)
            CALL(evt_snd_bgmoff, 513)
            CALL(evt_snd_bgmon, 288, 0)
            CALL(evt_snd_envoff, 513)
            CALL(evt_snd_envon, 288, 0)
            ELSE
            CALL(evt_snd_bgmon, 160, 0)
            CALL(evt_snd_envoff, 513)
            CALL(evt_snd_envon, 288, 0)
        END_IF
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
        CALL(evt_snd_envoff, 513)
        CALL(evt_snd_envon, 288, 0)
    END_IF
    RETURN
EVT_END()

DoorParam door_data_02_1 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f2_doa", // mapobj
    "A_ie_02_f2_doa02", // hitIn
    "A_ie_02_f2_doa01", // hitOut
    "S_ie_02_soto", // outside
    NULL, // outsideHit
    "S_i02_f2", // inside
    "A_ie_02C", // insideHit
    evt_door_02_10, // evtIn
    evt_door_02_11, // evtOut
    evt_wall_02_1, // evtWall
    door_data_02_1_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_02_1_open, // evtOpen
    evt_door_02_1_close, // evtClose
};

static EVT_BEGIN(evt_door_03_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_03)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_ueB"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_sitaB"), 0, 0, LW(3))
    RETURN
EVT_END()

static const char* door_data_03_npc[] = {
    /*"ノコノコＡ"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x82\x60",
    NULL,
};

EVT_BEGIN(door_data_03_open)
    CALL(evt_hitobj_onoff, STRING("A_ie_04A"), 1, 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(door_data_03_close)
    CALL(evt_hitobj_onoff, STRING("A_ie_04A"), 1, 1)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_03 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_doa", // mapobj
    "A_ie_03_doa02", // hitIn
    "A_ie_03_doa01", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    "S_ie_03_C", // insideHit
    evt_door_03_00, // evtIn
    NULL, // evtOut
    evt_wall_03, // evtWall
    door_data_03_npc, // npcs
    NULL, NULL, // mobjs..items
    door_data_03_open, // evtOpen
    door_data_03_close, // evtClose
};

static EVT_BEGIN(evt_door_03_caseopen)
    CALL_CHILD(PTR(evt_door_open_roll))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_open)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set_xyz, 193, 30, -396, 0, 150, 11)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_mario_mov_pos2, 275, LW(12), FLOAT(60.0f))
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG3_DRAWER_OPEN1"), LW(10), LW(11), LW(12), 88, 255, 0, 16, 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, -30, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(10), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_i03_tan_hiki"), LW(10), 0, 0)
        CALL(evt_hit_bind_update, STRING("A_i03_tan_hiki"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_mario_mov_pos2, 245, LW(12), FLOAT(60.0f))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG3_DRAWER_OPEN1"), LW(10), LW(11), LW(12), 88, 255, 0, 16, 0)
        CALL(evt_sub_intpl_msec_init, 11, -30, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(10), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("S_i03_tan_hiki"), LW(10), 0, 0)
            CALL(evt_hit_bind_update, STRING("A_i03_tan_hiki"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_jump_pos, 183, 20, -394, 500, 0, -1000)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    CALL(evt_run_case_evt, 8, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_bero_n"), 1)
    CALL(evt_hitobj_onoff, STRING("A_ie_04A"), 1, 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_03_close)
    CALL(evt_hitobj_onoff, STRING("A_ie_04A"), 1, 1)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_03_roll = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_doa", // mapobj
    NULL, // hitIn
    "A_ie_03_rol", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    "S_ie_03_C", // insideHit
    evt_door_03_00, // evtIn
    NULL, // evtOut
    evt_wall_03, // evtWall
    door_data_03_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_03_open, // evtOpen
    evt_door_03_close, // evtClose
};

static EVT_BEGIN(evt_door_03_entry)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, 8, 1, LW(3), 0, PTR(evt_door_03_caseopen), 0)
    CALL(evt_hit_bind_mapobj, STRING("A_i03_tan_hiki"), STRING("S_i03_tan_hiki"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_04)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_i04_ueB"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_i04_stB"), 0, 0, LW(3))
    RETURN
EVT_END()

static const char* door_data_04_npc[] = {
    /*"キノピオＡ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x82\x60",
    /*"キノピオ子供Ａ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x49\x8E\x71\x8B\x9F\x82\x60",
    NULL,
};

static EVT_BEGIN(evt_door_04_open)
    CALL(evt_hitobj_onoff, STRING("A_ie_03A"), 1, 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_04_close_snd)
    CALL(evt_hitobj_onoff, STRING("A_ie_03A"), 1, 1)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_04 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_04_doa", // mapobj
    "A_ie_04_doa02", // hitIn
    "A_ie_04_doa01", // hitOut
    "S_ie_04_soto", // outside
    NULL, // outsideHit
    "S_ie_04_naka", // inside
    "A_ie_04_C", // insideHit
    evt_door_04_00, // evtIn
    NULL, // evtOut
    evt_wall_04, // evtWall
    door_data_04_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_04_open, // evtOpen
    evt_door_04_close_snd, // evtClose
};

EVT_BEGIN(evt_door_04_caseclose)
    CALL_CHILD(PTR(evt_door_close_roll))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_close)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_bero_n"), 1)
    CALL(evt_cam3d_evt_set_xyz, 328, 37, -395, 0, 150, 11)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_mario_mov_pos2, LW(10), -400, FLOAT(60.0f))
    CALL(evt_mario_wait_landon)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, 8, 1, LW(2), 0, PTR(evt_door_04_caseclose), 0)
    CALL(evt_hitobj_onoff, STRING("A_ie_03A"), 1, 1)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam door_data_04_roll = {
    0, // flags
    3, // type
    1, // se
    "S_ie_04_doa", // mapobj
    "A_ie_04_rol", // hitIn
    NULL, // hitOut
    "S_ie_04_soto", // outside
    NULL, // outsideHit
    "S_ie_04_naka", // inside
    "A_ie_04_C", // insideHit
    evt_door_04_00, // evtIn
    NULL, // evtOut
    evt_wall_04, // evtWall
    door_data_04_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_04_open, // evtOpen
    evt_door_04_close, // evtClose
};

static EVT_BEGIN(evt_door_04_entry)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    SET_VALUE(LW(15), 0)
    CALL(evt_run_case_evt, 8, 1, LW(2), 0, PTR(evt_door_04_caseclose), 0)
    RETURN
EVT_END()

EVT_BEGIN(wall_bomb_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_after_01"), 1)
    IF_INT_EQUAL(GSWF(1224), 0)
        CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_before1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_before"), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after_01"), 1)
        CALL(evt_hitobj_onoff, STRING("A_ana_ana"), 1, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wall_bomb)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_before"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_hitobj_onoff, STRING("A_ana_ana"), 1, 0)
    SET_VALUE(GSWF(1224), 1)
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana_before"), STRING("offscreen"))
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after_01"), 1)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after_01"), 1)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana_before"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

EVT_BEGIN(uraguchi_tenin)
    CALL(evt_npc_move_position, STRING("店員"), -490, 24, 0, FLOAT(60.0f), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("店員"), 1024)
    CALL(evt_npc_set_anim, STRING("店員"), STRING("S_1"))
    CALL(evt_npc_set_ry, STRING("店員"), 0)
    RETURN
EVT_END()

EVT_BEGIN(uraguchi_event)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_envoff, 1024)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("店員"), 1073741856)
    CALL(evt_msg_print, 0, STRING("mac_2_041"), 0, STRING("店員"))
    CALL(evt_msg_select, 0, STRING("mac_2_041_select"))
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("mac_2_042"), 0, STRING("店員"))
    CALL(evt_msg_select, 0, STRING("mac_2_041_select"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(evt_msg_print, 0, STRING("mac_2_044"), 0, STRING("店員"))
        CALL(evt_npc_flag_onoff, 0, STRING("店員"), 1073741856)
        SET_VALUE(GW(10), 0)
        SET_VALUE(GW(11), 0)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
        CALL(evt_snd_envoff, 513)
        CALL(evt_snd_envon, 288, 0)
        RETURN
    END_IF
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
    CALL(evt_cam3d_evt_set, -873, 124, 6, -398, 53, 6, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_2_043"), 0, STRING("店員"))
    CALL(evt_npc_set_ry, STRING("店員"), 0)
    WAIT_MS(200)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_cam3d_evt_set, -873, 124, -106, -398, 53, -106, 2500, 11)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("店員"), LW(3), LW(4), LW(5))
            IF_INT_GREATER_OR_EQUAL(LW(2), LW(5))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("店員"), LW(3), LW(4), LW(5))
            IF_INT_GREATER_OR_EQUAL(LW(2), LW(5))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("店員"), 1024)
    CALL(evt_npc_status_onoff, 1, STRING("店員"), 2)
    CALL(evt_npc_move_position, STRING("店員"), -486, -195, 0, FLOAT(60.0f), 0)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("店員"), STRING("T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("店員"), STRING("S_1"))
    CALL(evt_npc_get_position, STRING("店員"), LW(0), LW(1), LW(2))
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_LOCK_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_UNLOCK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("店員"), -486, -165, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("店員"), 180)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_2_043_01"), 0, STRING("店員"))
    SET_VALUE(LW(0), PTR(&door_data_01_1))
    CALL_CHILD(PTR(evt_door_01_entry))
    SET_VALUE(GSWF(1230), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("店員"), 1073741856)
    CALL(evt_npc_flag_onoff, 1, STRING("店員"), 4194304)
    CALL(evt_npc_get_reglid, STRING("店員"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_change_interrupt, STRING("店員"), 1, PTR(uraguchi_tenin))
    CALL(evt_npc_restart_regular_event, STRING("店員"))
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(uraguchi_check)
    IF_INT_EQUAL(GSWF(1230), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GW(10), 0)
        SET_VALUE(GW(10), LW(0))
        ELSE
        IF_INT_EQUAL(GW(11), 0)
            SET_VALUE(GW(11), LW(0))
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(GW(10), 152)
        RETURN
    END_IF
    IF_INT_NOT_EQUAL(GW(11), 141)
        RETURN
    END_IF
    CALL_CHILD(PTR(uraguchi_event))
    RETURN
EVT_END()

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_SUPER_SHROOM, 20},
    {ITEM_VOLT_SHROOM, 10},
    {ITEM_DRIED_SHROOM, 2},
    {ITEM_LIFE_SHROOM, 50},
    {ITEM_DIZZY_DIAL, 12},
    {ITEM_THUNDER_BOLT, 10},
};

static ShopDb trade_list[] = {
    {ITEM_SUPER_SHROOM, 10, 0},
    {ITEM_VOLT_SHROOM, 5, 0},
    {ITEM_DRIED_SHROOM, 1, 0},
    {ITEM_LIFE_SHROOM, 25, 0},
    {ITEM_DIZZY_DIAL, 6, 0},
    {ITEM_THUNDER_BOLT, 5, 0},
    {ITEM_WHACKA_BUMP, 64, 0},
    {ITEM_COCONUT, 2, 0},
    {ITEM_KEEL_MANGO, 3, 0},
    {ITEM_TURTLEY_LEAF, 2, 0},
    {ITEM_HORSETAIL, 2, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, // fook_evt
    uraguchi_check, // buy_evt
    {"gor_shop2_00", "gor_shop2_01", "gor_shop2_02", "gor_shop2_03", "gor_shop2_04", "gor_shop2_05", "gor_shop2_06", "gor_shop2_07", "gor_shop2_08", "gor_shop2_09", "gor_shop2_10", "gor_shop2_11", "gor_shop2_12", "gor_shop2_13", "gor_shop2_14", "gor_shop2_15", "gor_shop2_16", "gor_shop2_17", "gor_shop2_18", "gor_shop2_19", "gor_shop2_20", "gor_shop2_21", "gor_shop2_22", "gor_shop2_23", "gor_shop2_24", "gor_shop2_25", "gor_shop2_26", "gor_shop2_27", "gor_shop2_28", "gor_shop2_29", "gor_shop2_30", "gor_shop2_31", "gor_shop2_32", "gor_shop2_33", "gor_shop2_34"}, // msgs
};

EVT_BEGIN(majorin_nusumigiki)
    CALL(evt_npc_set_ry_lr, STRING("マジョリン"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), 512)
    SET_FLOAT_VALUE(GW(9), FLOAT(1.0f))
    BEGIN_INLINE_EVENT
        CALL(dummy_shadow, -250, 0, 125, GW(9))
    END_INLINE_EVENT
    CALL(evt_door_end_wait)
    CALL(evt_cam3d_evt_set, -425, 134, 733, -425, 38, 192, 500, 11)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            DEBUG_PUT_REG(LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_stop, 0)
        CALL(evt_party_flg_onoff, 0, 16777216, 1)
        CALL(evt_party_move_pos2, 0, -450, 200, FLOAT(120.0f))
        CALL(evt_party_flg_onoff, 0, 16777216, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -400, 200, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_WHILE(GF(0))
    CALL(evt_msg_print_party, STRING("mac_5_900"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 3, -250, 0, 120)
    CALL(evt_msg_print, 0, STRING("mac_5_901"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        LOOP(3)
            CALL(evt_party_set_dir, 0, 270, 200)
            WAIT_MS(200)
            CALL(evt_party_set_dir, 0, 90, 200)
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    LOOP(3)
        CALL(evt_mario_set_dir, 270, 200, 0)
        WAIT_MS(200)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(200)
    END_LOOP
    CALL(evt_cam3d_evt_set, -250, 69, 350, -250, 30, 128, 500, 11)
    WAIT_MS(500)
    CALL(evt_mario_set_pos, -350, 0, 200)
    CALL(evt_party_set_pos, 0, -390, 0, 200)
    CALL(evt_msg_toge, 2, -250, 0, 120)
    CALL(evt_msg_print, 0, STRING("mac_5_902"), 0, 0)
    SET_FLOAT_VALUE(GW(9), FLOAT(0.0f))
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_WITCH1"))
    CALL(evt_npc_majo_disp_on, STRING("マジョリン"), -250, 0, 125, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -315, 114, 586, -315, 45, 192, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_903"))
    CALL(evt_cam3d_evt_set, -250, 69, 350, -250, 30, 128, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_5_904"), 0, STRING("マジョリン"))
    CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    CALL(evt_npc_set_position, STRING("マジョリン"), 0, -1000, 0)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -370, 88, 463, -370, 40, 192, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_5_905"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 280)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(yuureturn_rtn)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon_f, 272, STRING("ENV_STG0_GOR4"), 0)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_snd_bgmoff_f, 1024, 0)
    CALL(evt_snd_envoff_f, 1024, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    SET_VALUE(LW(0), PTR(&door_data_02_0r))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    SET_VALUE(LW(0), PTR(&door_data_02_0l))
    SPAWN_EVENT(PTR(evt_door_open_quick))
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    SUBTRACT(LW(5), 40)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP2"), LW(3), LW(4), LW(5), 0)
    CALL(evt_mario_jump_pos, LW(3), 0, LW(5), 500, 0, 20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_LANDING1"), LW(3), LW(4), LW(5), 0)
    WAIT_MS(500)
    CALL_CHILD(PTR(gor_03_dokan_down_evt))
    WAIT_MS(500)
    IF_INT_LESS(GSW_STORY_PROGRESS, 403)
        CALL(evt_msg_print, 0, STRING("bunny_teresa_17"), 0, STRING("バニーテレサ"))
        ELSE
        CALL(evt_msg_print, 0, STRING("bunny_teresa_17_1"), 0, STRING("バニーテレサ"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_mail)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(2), -300)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), -50)
        CALL(evt_mario_mov_pos, -50, LW(2), 500)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 50)
        CALL(evt_mario_mov_pos, 50, LW(2), 500)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION1"))
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 270, -1)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print_party, STRING("gor_03_mail3_00"))
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, LW(0), 91, -11, LW(0), 39, -307, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_msg_print, 0, STRING("gor_03_mail3_01"), 0, 0)
    CALL(evt_msg_print_party, STRING("gor_03_mail3_02"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 173)
    CALL(evt_pouch_receive_mail, 2)
    CALL(evt_pouch_open_mail, 2)
    RETURN
EVT_END()

EVT_BEGIN(kuzi_bomb_talk_normal)
    CALL(evt_unitwin_alloc, 0)
    CALL(evt_cam3d_evt_set, -150, 120, 890, -150, 50, 470, 800, 11)
    WAIT_MS(800)
    IF_INT_EQUAL(GSWF(1237), 0)
        SET_VALUE(GSWF(1237), 1)
        CALL(evt_msg_print, 0, STRING("kuji_bomb_000"), 0, STRING("グルメボム兵"))
        CALL(evt_msg_select, 0, STRING("kuji_bomb_001"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_002"))
            CALL(unitwincoin_disp_onoff, 1)
            CALL(evt_msg_select, 0, STRING("kuji_bomb_003"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_pouch_get_coin, LW(0))
                IF_INT_LESS(LW(0), 10)
                    CALL(unitwincoin_disp_onoff, 0)
                    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022_1"))
                    ELSE
                    CALL(evt_pouch_add_coin, -10)
                    CALL(unitwincoin_wait)
                    CALL(unitwincoin_disp_onoff, 0)
                    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_2"), STRING("T_2"))
                    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_004"))
                    CALL(evt_lottery_buy, LW(0))
                    CALL(evt_item_entry, STRING("kuzi"), 106, 0, -1000, 0, 17, -1, 0)
                    CALL(evt_item_get_item, STRING("kuzi"))
                    CALL(evt_item_get_item_end_wait, STRING("kuzi"))
                    CALL(evt_msg_print, 0, STRING("kuji_bomb_006"), 0, STRING("グルメボム兵"))
                END_IF
                ELSE
                CALL(unitwincoin_disp_onoff, 0)
                CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
                CALL(evt_msg_print_add, 0, STRING("kuji_bomb_007"))
            END_IF
            ELSE
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_007"))
        END_IF
        ELSE
        CALL(evt_msg_print, 0, STRING("kuji_bomb_008"), 0, STRING("グルメボム兵"))
        LABEL(0)
        CALL(evt_msg_select, 0, STRING("kuji_bomb_009"))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_pouch_check_item, 106, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("kuji_bomb_010"))
                CALL(unitwincoin_disp_onoff, 1)
                CALL(evt_msg_select, 0, STRING("kuji_bomb_011"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_pouch_get_coin, LW(0))
                    IF_INT_LESS(LW(0), 10)
                        CALL(unitwincoin_disp_onoff, 0)
                        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022_1"))
                        ELSE
                        CALL(evt_pouch_add_coin, -10)
                        CALL(unitwincoin_wait)
                        CALL(unitwincoin_disp_onoff, 0)
                        CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_2"), STRING("T_2"))
                        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_012"))
                        CALL(evt_lottery_buy, LW(0))
                        CALL(evt_item_entry, STRING("kuzi"), 106, 0, -1000, 0, 17, -1, 0)
                        CALL(evt_item_get_item, STRING("kuzi"))
                        CALL(evt_item_get_item_end_wait, STRING("kuzi"))
                        CALL(evt_msg_print, 0, STRING("kuji_bomb_014"), 0, STRING("グルメボム兵"))
                    END_IF
                    ELSE
                    CALL(unitwincoin_disp_onoff, 0)
                    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
                    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022"))
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("kuji_bomb_015"))
                CALL(unitwincoin_disp_onoff, 1)
                CALL(evt_msg_select, 0, STRING("kuji_bomb_016"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_pouch_get_coin, LW(0))
                    IF_INT_LESS(LW(0), 100)
                        CALL(unitwincoin_disp_onoff, 0)
                        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022_1"))
                        ELSE
                        CALL(evt_pouch_add_coin, -100)
                        CALL(unitwincoin_wait)
                        CALL(unitwincoin_disp_onoff, 0)
                        CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_2"), STRING("T_2"))
                        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_017"))
                        CALL(evt_lottery_buy, LW(0))
                        CALL(evt_pouch_remove_item, 106, LW(0))
                        CALL(evt_item_entry, STRING("kuzi"), 106, 0, -1000, 0, 17, -1, 0)
                        CALL(evt_item_get_item, STRING("kuzi"))
                        CALL(evt_item_get_item_end_wait, STRING("kuzi"))
                        CALL(evt_msg_print, 0, STRING("kuji_bomb_019"), 0, STRING("グルメボム兵"))
                    END_IF
                    ELSE
                    CALL(unitwincoin_disp_onoff, 0)
                    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
                    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022"))
                END_IF
            END_IF
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_020"))
            CALL(evt_msg_print, 0, STRING("kuji_bomb_021"), 0, STRING("グルメボム兵"))
            GOTO(0)
            CASE_EQUAL(2)
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_022"))
        END_SWITCH
    END_IF
    CALL(evt_unitwin_free)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_1"), STRING("T_1"))
    RETURN
EVT_END()

EVT_BEGIN(kuzi_bomb_talk_husei)
    CALL(evt_unitwin_alloc, 0)
    CALL(evt_lottery_flag, LW(0))
    IF_BITS_SET(LW(0), 16)
        SET_VALUE(LW(0), 0)
        ELSE
        SET_VALUE(LW(0), 1)
    END_IF
    IF_INT_EQUAL(GSWF(257), 1)
        SET_VALUE(LW(0), 1)
        ELSE
        SET_VALUE(GSWF(257), 1)
    END_IF
    CALL(evt_cam3d_evt_set, -150, 120, 890, -150, 50, 470, 800, 11)
    WAIT_MS(800)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_lottery_flag, LW(0))
        IF_BITS_SET(LW(0), 8)
            SET_VALUE(LW(0), 1)
            ELSE
            SET_VALUE(LW(0), 0)
        END_IF
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("kuji_bomb_061"), 0, STRING("グルメボム兵"))
            GOTO(0)
            ELSE
            CALL(evt_msg_print, 0, STRING("kuji_bomb_062"), 0, STRING("グルメボム兵"))
            GOTO(99)
        END_IF
    END_IF
    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
    CALL(evt_msg_print, 0, STRING("kuji_bomb_037"), 0, STRING("グルメボム兵"))
    CALL(evt_mario_set_pose, STRING("M_N_1"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("kuji_bomb_038"), 0, STRING("グルメボム兵"))
    CALL(evt_msg_select, 0, STRING("kuji_bomb_039"))
    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_040"))
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("kuji_bomb_041"), 0, STRING("グルメボム兵"))
    CALL(evt_msg_select, 0, STRING("kuji_bomb_042"))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(10)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_043"))
        CALL(evt_msg_select, 0, STRING("kuji_bomb_044"))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(10)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_045"))
            CALL(evt_msg_select, 0, STRING("kuji_bomb_046"))
            IF_INT_EQUAL(LW(0), 0)
                GOTO(10)
                ELSE
                CALL(evt_msg_print_add, 0, STRING("kuji_bomb_047"))
                CALL(evt_msg_select, 0, STRING("kuji_bomb_048"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("kuji_bomb_049"))
                    LABEL(0)
                    CALL(unitwincoin_disp_onoff, 1)
                    CALL(evt_msg_select, 0, STRING("kuji_bomb_050"))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(evt_pouch_get_coin, LW(0))
                        IF_INT_LESS(LW(0), 500)
                            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_4"), STRING("T_4"))
                            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_062_1"))
                            ELSE
                            CALL(evt_pouch_add_coin, -500)
                            CALL(unitwincoin_wait)
                            CALL(unitwincoin_disp_onoff, 0)
                            CALL(evt_eff_delete, STRING("husei_huki"))
                            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_051"))
                            CALL(evt_pouch_remove_item, 106, LW(0))
                            CALL(evt_lottery_buy, LW(0))
                            SET_VALUE(GSWF(257), 0)
                            CALL(evt_item_entry, STRING("kuzi"), 106, 0, -1000, 0, 17, -1, 0)
                            CALL(evt_item_get_item, STRING("kuzi"))
                            CALL(evt_item_get_item_end_wait, STRING("kuzi"))
                            CALL(evt_msg_print, 0, STRING("kuji_bomb_053"), 0, STRING("グルメボム兵"))
                        END_IF
                        ELSE
                        CALL(unitwincoin_disp_onoff, 0)
                        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_054"))
                    END_IF
                    ELSE
                    GOTO(10)
                END_IF
            END_IF
        END_IF
    END_IF
    GOTO(99)
    LABEL(10)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envoff, 512)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), GW(5))
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 30000)
                BREAK_LOOP
            END_IF
            IF_INT_NOT_EQUAL(GF(1), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 300)
        END_LOOP
        CALL(evt_snd_sfxoff, GW(5))
    END_INLINE_EVENT
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("kuji_bomb_055"), PTR(kuji_bomb_055_callback), STRING("グルメボム兵"))
    SET_VALUE(GF(1), 1)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_TEC_SHUTDOWN1"), 0)
    CALL(evt_sub_intpl_msec_init, 2, 0, 239, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 480)
        SUBTRACT(LW(2), LW(0))
        SUBTRACT(LW(2), LW(0))
        CALL(viewport, 0, LW(0), 608, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 2, 0, 303, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 608)
        SUBTRACT(LW(2), LW(0))
        SUBTRACT(LW(2), LW(0))
        CALL(viewport, LW(0), 240, LW(2), 1)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(viewport, 0, 0, 1, 1)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("グルメボム兵"), 512)
    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_3"), STRING("T_3"))
    CALL(evt_npc_set_anim, STRING("グルメボム兵"), STRING("S_3"))
    WAIT_MS(3000)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_TEC_SHUTDOWN1"), 0)
    CALL(evt_sub_intpl_msec_init, 2, 303, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 608)
        SUBTRACT(LW(2), LW(0))
        SUBTRACT(LW(2), LW(0))
        CALL(viewport, LW(0), 240, LW(2), 1)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 2, 239, 0, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 480)
        SUBTRACT(LW(2), LW(0))
        SUBTRACT(LW(2), LW(0))
        CALL(viewport, 0, LW(0), 608, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(viewport, 0, 0, 608, 480)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR1"))
    CALL(evt_msg_print, 0, STRING("kuji_bomb_056"), 0, STRING("グルメボム兵"))
    CALL(evt_msg_select, 0, STRING("kuji_bomb_057"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(10)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("kuji_bomb_058"))
        CALL(evt_msg_select, 0, STRING("kuji_bomb_059"))
        IF_INT_NOT_EQUAL(LW(0), 0)
            GOTO(10)
            ELSE
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_1"), STRING("T_1"))
            CALL(evt_msg_print_add, 0, STRING("kuji_bomb_060"))
            GOTO(0)
        END_IF
    END_IF
    LABEL(99)
    CALL(evt_unitwin_free)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_1"), STRING("T_1"))
    RETURN
EVT_END()

EVT_BEGIN(kuzi_bomb_bomb)
    SET_VALUE(LW(15), LW(0))
    WAIT_FRAMES(40)
    CALL(evt_npc_get_position, STRING("グルメボム兵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LOTTERY_BOMB_BURST1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("グルメボム兵"), STRING("J_1A"))
    SET_FLOAT_VALUE(LW(0), FLOAT(20.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(-0.7998047f))
    SET_VALUE(LW(2), 60)
    CALL(evt_npc_get_position, STRING("グルメボム兵"), LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LOTTERY_BOMB_BURST2"), LW(3), LW(4), LW(5), 0)
    SET_VALUE(LW(6), 0)
    LOOP(0)
        ADD_FLOAT(LW(4), LW(0))
        ADD_FLOAT(LW(0), LW(1))
        CALL(evt_npc_set_position, STRING("グルメボム兵"), LW(3), LW(4), LW(5))
        IF_INT_LESS(LW(0), FLOAT(1.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        END_IF
        CALL(kuzi_bomb_bomb_eff, LW(15), LW(6), LW(6))
        SUBTRACT(LW(2), 1)
        IF_INT_LESS_OR_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(evt_npc_set_anim, STRING("グルメボム兵"), STRING("J_1B"))
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(-1.0f))
    CALL(evt_npc_get_position, STRING("グルメボム兵"), LW(2), LW(3), LW(4))
    LOOP(0)
        ADD_FLOAT(LW(3), LW(0))
        ADD_FLOAT(LW(0), LW(1))
        CALL(evt_npc_set_position, STRING("グルメボム兵"), LW(2), LW(3), LW(4))
        IF_INT_LESS(LW(0), FLOAT(-6.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(-6.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(3), FLOAT(0.0f))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_position, STRING("グルメボム兵"), LW(2), FLOAT(0.0f), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PARTY_PBM_DOWN1"), LW(2), 0, LW(4), 0)
    RETURN
EVT_END()

EVT_BEGIN(kuzi_keiziban_normal)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_lottery_todaynum, LW(0))
    CALL(evt_msg_print_insert, 0, STRING("kuji_bomb_023"), 0, 0, LW(0))
    CALL(evt_pouch_check_item, 106, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_lottery_flag, LW(0))
    IF_BITS_SET(LW(0), 16)
        SET_VALUE(LW(0), 0)
        ELSE
        SET_VALUE(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_set_dir_to_target, STRING("グルメボム兵"), STRING("mario"))
        CALL(evt_mario_set_dir, 270, 300, 0)
        CALL(evt_lottery_flag, LW(1))
        SET_VALUE(LW(0), 4)
        IF_BITS_SET(LW(1), 32768)
            SET_VALUE(LW(0), 3)
        END_IF
        IF_BITS_SET(LW(1), 16384)
            SET_VALUE(LW(0), 2)
        END_IF
        IF_BITS_SET(LW(1), 8192)
            SET_VALUE(LW(0), 1)
        END_IF
        IF_BITS_SET(LW(1), 4096)
            SET_VALUE(LW(0), 0)
        END_IF
        IF_INT_NOT_EQUAL(LW(0), 4)
            CALL(evt_cam3d_evt_set, -150, 120, 890, -150, 50, 470, 800, 11)
            WAIT_MS(800)
            IF_INT_LESS_OR_EQUAL(LW(0), 1)
                CALL(evt_npc_flag_onoff, 0, STRING("グルメボム兵"), 1536)
                CALL(evt_npc_set_anim, STRING("グルメボム兵"), STRING("B_1"))
            END_IF
            CALL(evt_msg_print, 0, STRING("kuji_bomb_023_1"), 0, STRING("グルメボム兵"))
            IF_INT_LESS_OR_EQUAL(LW(0), 1)
                SET_VALUE(GW(0), LW(0))
                CALL_CHILD(PTR(kuzi_bomb_bomb))
                SET_VALUE(LW(0), GW(0))
            END_IF
            CALL(evt_npc_flag_onoff, 1, STRING("グルメボム兵"), 1536)
            SWITCH(LW(0))
                CASE_EQUAL(0)
                SET_VALUE(LW(1), STRING("kuji_bomb_024"))
                SET_VALUE(LW(2), STRING("M_N_7"))
                CASE_EQUAL(1)
                SET_VALUE(LW(1), STRING("kuji_bomb_025"))
                SET_VALUE(LW(2), STRING("M_N_7"))
                CASE_EQUAL(2)
                SET_VALUE(LW(1), STRING("kuji_bomb_026"))
                SET_VALUE(LW(2), STRING("M_N_5B"))
                CASE_EQUAL(3)
                SET_VALUE(LW(1), STRING("kuji_bomb_027"))
                SET_VALUE(LW(2), STRING("M_N_5B"))
            END_SWITCH
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_2"), STRING("T_2"))
            CALL(evt_msg_print, 0, LW(1), 0, STRING("グルメボム兵"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(1), LW(2), LW(3), 0)
                    CASE_EQUAL(1)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(1), LW(2), LW(3), 0)
                    CASE_EQUAL(2)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(1), LW(2), LW(3), 0)
                    CASE_EQUAL(3)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(1), LW(2), LW(3), 0)
                END_SWITCH
            END_INLINE_EVENT
            CALL(evt_mario_set_pose, LW(2))
            WAIT_MS(800)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_set_dir_to_target, STRING("グルメボム兵"), STRING("mario"))
            CALL(kuzi_get_keihin, LW(0), LW(1))
            SET_VALUE(LW(2), LW(0))
            ADD(LW(2), 1)
            CALL(evt_msg_print_insert, 0, STRING("kuji_bomb_028"), 0, STRING("グルメボム兵"), LW(2))
            CALL(evt_mario_mov_pos2, -120, 480, 60)
            CALL(evt_item_entry, STRING("keihin"), LW(1), 0, -1000, 0, 17, -1, 0)
            CALL(evt_item_get_item, STRING("keihin"))
            CALL(evt_item_get_item_end_wait, STRING("keihin"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 0, STRING("kuji_bomb_030"), 0, STRING("グルメボム兵"))
                CALL(evt_pouch_remove_item, 106, LW(0))
                CALL(evt_lottery_buy, LW(0))
                CALL(evt_item_entry, STRING("kuzi"), 106, 0, -1000, 0, 17, -1, 0)
                CALL(evt_item_get_item, STRING("kuzi"))
                CALL(evt_item_get_item_end_wait, STRING("kuzi"))
                CALL(evt_msg_print, 0, STRING("kuji_bomb_032"), 0, STRING("グルメボム兵"))
                ELSE
                CALL(evt_msg_print, 0, STRING("kuji_bomb_029"), 0, STRING("グルメボム兵"))
            END_IF
            CALL(evt_cam3d_evt_off, 500, 11)
            WAIT_MS(500)
            ELSE
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_1"), STRING("T_1"))
            CALL(evt_msg_print, 0, STRING("kuji_bomb_033"), 0, STRING("グルメボム兵"))
            CALL(evt_mario_mov_pos2, -120, 480, 60)
            CALL(evt_item_entry, STRING("kinoko"), 148, 0, -1000, 0, 17, -1, 0)
            CALL(evt_item_get_item, STRING("kinoko"))
            CALL(evt_item_get_item_end_wait, STRING("kinoko"))
            CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_2"), STRING("T_2"))
            CALL(evt_msg_print, 0, STRING("kuji_bomb_035"), 0, STRING("グルメボム兵"))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_set_autotalkpose, STRING("グルメボム兵"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_anim, STRING("グルメボム兵"), STRING("S_1"))
    RETURN
EVT_END()

EVT_BEGIN(kuzi_keiziban_husei)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("kuji_bomb_036"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kuzi_bomb_talk)
    CALL(evt_lottery, 0, LW(0), LW(1))
    IF_BITS_SET(LW(0), 4)
        CALL_CHILD(PTR(kuzi_bomb_talk_husei))
        ELSE
        CALL_CHILD(PTR(kuzi_bomb_talk_normal))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kuzi_keiziban_entry)
    CALL(evt_unitwin_alloc, 0)
    CALL(unitwin_lottery_disp_onoff, 1)
    CALL(evt_lottery, 1, LW(0), LW(1))
    IF_BITS_SET(LW(0), 4)
        CALL_CHILD(PTR(kuzi_keiziban_husei))
        ELSE
        CALL_CHILD(PTR(kuzi_keiziban_normal))
    END_IF
    CALL(unitwin_lottery_disp_onoff, 0)
    CALL(evt_unitwin_free)
    RETURN
EVT_END()

EVT_BEGIN(tenin_fuwafuwa)
    CALL(gor_npc_fuwafuwa, STRING("店員"), FLOAT(0.5f), 0)
    RETURN
EVT_END()

EVT_BEGIN(casino_fuwafuwa)
    CALL(gor_npc_fuwafuwa, STRING("バニーテレサ"), FLOAT(0.5f), 0)
    RETURN
EVT_END()

EVT_BEGIN(debug_event_mode)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 121)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 123)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 124)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 272)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 278)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 279)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        RETURN
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 64)
        CALL(evt_sub_system_flag_onoff, 0, 64)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_set_pos, -327, 155, -432)
        CALL(evt_party_set_pos, 0, -327, 155, -467)
        CALL(evt_mario_cont_onoff, 1)
        CALL(evt_party_cont_onoff, 0, 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam_road_reset)
        CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_cam3d_evt_off, 0, 11)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(monte_evt_door_close)
    CALL(enzan_lw_save)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(exchange_keep_sub))
    CALL(evt_mario_key_onoff, 1)
    CALL(enzan_lw_load)
    CALL_CHILD(PTR(evt_door_close))
    RETURN
EVT_END()

EVT_BEGIN(monte_evt_door_entry)
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
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(monte_evt_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 1, LW(3), 0, PTR(evt_door_open), 0)
    END_IF
    CALL(evt_door_param, LW(0), 9, LW(1))
    IF_BITS_SET(LW(1), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(gor_03_init_evt)
    IF_INT_EQUAL(GSWF(1229), 0)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR4"))
        ELSE
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envoff, 512)
    END_IF
    IF_INT_NOT_EQUAL(GSWF(1229), 0)
        CALL(evt_mario_normalize)
    END_IF
    CALL_CHILD(PTR(gor_03_dokan_init_evt))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_hitobj_attr_onoff, 1, 0, STRING("s_bero"), 4)
    CALL_CHILD(PTR(luigi_npcEnt))
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(278, 280)
        CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
        CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
        CASE_GREATER_OR_EQUAL(281)
        CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
        IF_INT_EQUAL(GSWF(5393), 1)
            CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
        END_IF
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    SET_VALUE(LW(0), PTR(&door_data_01_0))
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_door_entry)
    IF_INT_EQUAL(GSWF(1230), 1)
        SET_VALUE(LW(0), PTR(&door_data_01_1))
        CALL_CHILD(evt_door_entry)
#else
    CALL_CHILD(PTR(evt_door_entry))
    IF_INT_EQUAL(GSWF(1230), 1)
        SET_VALUE(LW(0), PTR(&door_data_01_1))
        CALL_CHILD(PTR(evt_door_entry))
#endif
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_02_0l))
    CALL_CHILD(PTR(monte_evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_02_0r))
    CALL_CHILD(PTR(monte_evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_02_1))
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_door_entry)
    SET_VALUE(LW(0), PTR(&door_data_03))
    CALL_CHILD(evt_door_entry)
#else
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_data_03))
    CALL_CHILD(PTR(evt_door_entry))
#endif
    SET_VALUE(LW(0), PTR(&door_data_03_roll))
    CALL_CHILD(PTR(evt_door_03_entry))
    SET_VALUE(LW(0), PTR(&door_data_04))
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_door_entry)
#else
    CALL_CHILD(PTR(evt_door_entry))
#endif
    SET_VALUE(LW(0), PTR(&door_data_04_roll))
    CALL_CHILD(PTR(evt_door_04_entry))
    SPAWN_EVENT(PTR(wall_bomb_init))
    CALL(evt_run_case_evt, 15, 1, STRING("A_ana_ana"), 0, PTR(wall_bomb), 0)
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    CALL(evt_item_entry, STRING("item_00"), 125, 550, 0, 180, 16, GSWF(5580), 0)
    CALL(evt_item_entry, STRING("item_01"), 125, -585, 0, 220, 16, GSWF(5581), 0)
    CALL(evt_item_entry, STRING("item_02"), 125, 220, 10, 225, 16, GSWF(5582), 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -200, 0, 125, STRING("S_ground_A"), 125, GSWF(5579))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 250, 60, -510, 0, GSWF(5527), 90)
    CALL(evt_mobj_powerupblk, STRING("pblk2"), -563, 0, -150, 0, GSWF(5528), 90)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kou_kusas01"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kou_kusas02"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kou_kusas03"), 1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas01"), 1)
        CASE_LESS(112)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas01"), 1)
        CASE_LESS(164)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas01"), 1)
        CASE_LESS(215)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas02"), 1)
        CASE_LESS(261)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas02"), 1)
        CASE_LESS(339)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas02"), 1)
        CASE_LESS(373)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas03"), 1)
        CASE_LESS(401)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas03"), 1)
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kusas03"), 1)
    END_SWITCH
    SPAWN_EVENT_GET_ID(PTR(desk_talk_check), GW(15))
    CALL_CHILD(PTR(debug_event_mode))
    CALL(evt_goryuugijo_alloc, LW(0), 0)
    CALL(goryuugijo_set_dataadr, 0, LW(0))
    CALL(evt_goryuugijo_alloc, LW(0), 1)
    CALL(goryuugijo_set_dataadr, 1, LW(0))
    CALL(evt_run_case_evt, 9, 1, STRING("A_srot01"), 0, PTR(slot1_set), LW(1))
    CALL(evt_goryuugijo_set_caseid, LW(0), 0, LW(1))
    CALL(evt_run_case_evt, 9, 1, STRING("A_srot02"), 0, PTR(slot2_set), LW(1))
    CALL(evt_goryuugijo_set_caseid, LW(0), 1, LW(1))
    CALL(evt_run_case_evt, 9, 1, STRING("A_gb_a"), 0, PTR(ranking_set), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_mot_a"), 0, PTR(exchange_set), 0)
    IF_INT_NOT_EQUAL(GSWF(1229), 0)
        SET_VALUE(GSWF(1229), 0)
        SPAWN_EVENT(PTR(yuureturn_rtn))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 172)
        SPAWN_EVENT(PTR(peach_mail))
    END_IF
    SPAWN_EVENT(PTR(tenin_fuwafuwa))
    SPAWN_EVENT(PTR(casino_fuwafuwa))
#ifdef VERSION_G8ME01
    SPAWN_EVENT(evt_sub_mail_event)
    CALL(evt_run_case_evt, 9, 1, STRING("A_kuzi"), 0, PTR(kuzi_keiziban_entry), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_i02_f1_kanD"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i02_f1_hon07"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_i02_f1_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_i02_f1_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_i02_f1_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_i02_f1_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_i02_f1_kan"), 16384)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i02_f1_hon07"), 16384)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_i02_f1_kan"), 16384)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_i02_f1_kan"), 16384)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_i02_f1_kan"), 16384)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_i02_f1_kan"), 16384)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_i02_f1_kanD"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_i02_f1_kan"), 16384)
    END_SWITCH
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_sro_light01"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_sro_light02"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_sro_light01"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_sro_light02"), 16384)
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_sro_light01"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_sro_light02"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_sro_light01"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_sro_light02"), 16384)
    END_SWITCH
#else
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL(evt_run_case_evt, 9, 1, STRING("A_kuzi"), 0, PTR(kuzi_keiziban_entry), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02"), 2)
#endif
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 121)
        SPAWN_EVENT(PTR(maffiaboss_maffiaboss))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 123)
        IF_INT_EQUAL(GSWF(1236), 0)
            SPAWN_EVENT(PTR(maffiaboss_airship_ticket))
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(268)
        CALL(evt_run_case_evt, 9, 0, STRING("A_i02_f2_set00C"), 0, PTR(maffiaboss_desk_talk), 0)
        CASE_LESS(279)
        SET_VALUE(LW(15), 1)
        CALL(evt_run_case_evt, 9, 0, STRING("A_i02_bed_l"), 0, PTR(maffiaboss_desk_talk), GW(12))
        SET_VALUE(LW(15), 0)
        CALL(evt_run_case_evt, 9, 0, STRING("A_i02_bed_m"), 0, PTR(maffiaboss_desk_talk), GW(13))
        CASE_LESS(282)
        CASE_DEFAULT
        CALL(evt_run_case_evt, 9, 0, STRING("A_i02_f2_set00C"), 0, PTR(peton_desk_talk), 0)
    END_SWITCH
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kou_kabe"), 2)
    RETURN
EVT_END()

static USER_FUNC(fall_rtn) {
    Vec pos;
    const char* sfx1;
    const char* sfx2;
    const char* sfx3;
    SlotWork* data;
    s32 i;
    s32 done = 0;

    if (isFirstCall) {
        SlotWork* data = ((GoryuuWork*)event->lwData[0])->data;
        monte_p = _mapAlloc(data->monteCount * sizeof(GMonte));
        memset(monte_p, 0, data->monteCount * sizeof(GMonte));
        monte_p->owner = data;
        for (i = 0; i < monte_p->owner->monteCount; i++) {
            GMonteSub* sub = &monte_p[i].sub;
            Vec pos = fall_pos0();
            f32 s;
            f32 c;
            f32 ofs;

            pos.z = montemedalzpos[monte_p->owner->nowSlot];
            sub->pos = pos;
            ofs = ((i & 1) * 2.0f - 1.0f) * ((rand() % 101) / 10.0f);
            sub->pos.z += ofs;
            sub->scale = 1.0f;
            {
                f32 spd = 2.0f + (rand() % 300) / 100.0f;
                c = 0.0f;
                s = 0.0f;
                movePos(&s, &c, spd, 270.0f - 3.0f * ofs);
            }
            {
                Vec vel = fall_vel0();

                vel.x = s;
                vel.z = c;
                monte_p[i].vel = vel;
            }
            monte_p[i].wait = ((rand() % 700 + 100) * 60) / 1000;
        }
    }
    sfx1 = (0, "SFX_EVT_GAME_MONTE_LEAPED1");
    sfx2 = (0, "SFX_EVT_GAME_MONTE_COUNT1");
    sfx3 = (0, "SFX_EVT_GAME_MONTE_COUNT1");
    for (i = 0; i < monte_p->owner->monteCount; i++) {
        GMonte* m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (m->wait != 0) {
                    m->wait--;
                } else {
                    m->state++;
                }
                break;
            case 1:
                PSVECAdd(&m->sub.pos, &m->vel, &m->sub.pos);
                m->vel.y -= 0.81666666f;
                if (m->sub.pos.y <= 0.0f) {
                    m->sub.pos.y = 0.0f;
                    psndSFXOn_3D(sfx1, &m->sub.pos);
                    if (__fabs(m->vel.y) <= 4.0) {
                        m->state++;
                        m->timer = 300;
                    } else {
                        f32 r = (rand() % 21) / 100.0f;
                        m->vel.y *= -(0.5f + r);
                        m->vel.x = 0.0f;
                        m->vel.z = 0.0f;
                    }
                }
                if (get_monte_check(&m->sub.pos, &marioGetPtr()->position) == TRUE) {
                    psndSFXOn_3D(sfx2, &m->sub.pos);
                    if (yuwp->tokens < 99999) {
                        yuwp->tokens++;
                    }
                    m->hidden = 1;
                    m->state = 3;
                }
                break;
            case 2:
                m->timer--;
                if (m->timer == 0) {
                    m->hidden = 1;
                    m->state++;
                }
                if (m->timer < 60) {
                    if (m->timer & 2) {
                        m->sub.pos.y = 0.0f;
                    } else {
                        m->sub.pos.y = -1000.0f;
                    }
                }
                pos = m->sub.pos;
                pos.y = 0.0f;
                if (get_monte_check(&pos, &marioGetPtr()->position) == TRUE) {
                    psndSFXOn_3D(sfx3, &m->sub.pos);
                    if (yuwp->tokens < 99999) {
                        yuwp->tokens++;
                    }
                    m->hidden = 1;
                    m->state = 3;
                }
                break;
            case 3:
                done++;
                break;
        }
    }
    if (done < monte_p->owner->monteCount) {
        dispEntry(CAMERA_3D, 0.0f, 1, monte_disp, monte_p);
        return EVT_RETURN_BLOCK;
    }
    monte_p->owner->monteCount = 0;
    monte_p->owner->monteEvt = NULL;
    _mapFree(monte_p);
    return EVT_RETURN_DONE;
}

USER_FUNC(goryuugijo_set_slot_phase) {
    EvtWord* args = event->args;
    SlotWork* data = ((GoryuuWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(goryuugijo_get_slot_phase) {
    EvtWord* args = event->args;
    SlotWork* data = ((GoryuuWork*)evtGetValue(event, args[0]))->data;

    evtSetValue(event, args[1], data->phase);
    return EVT_RETURN_DONE;
}

void goryuugijo_upsidewindow_dispoff_sub(s32 slot, s32 reel) {
    mapGrpFlagOn(drumname[slot].window[reel][0], 1);
    mapGrpFlagOn(drumname[slot].window[reel][1], 1);
    mapGrpFlagOn(drumname[slot].window[reel][2], 1);
}

void goryuugijo_upsidewindow_disp_sub(s32 slot, s32 reel) {
    mapGrpFlagOn(drumname[slot].window[reel][0], 1);
    mapGrpFlagOn(drumname[slot].window[reel][1], 1);
    mapGrpFlagOn(drumname[slot].window[reel][2], 1);
}

USER_FUNC(goryuugijo_door_init_slot) {
    GoryuuWork* wp = (GoryuuWork*)evtGetValue(event, event->args[0]);
    SlotWork* data = wp->data;
    s32 j;
    s32 i;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            data->angle[i][j] = 0.0f;
            if (data->angle[i][j] >= 360.0f) {
                data->angle[i][j] -= 360.0f;
            }
            data->angle[i][j] = 45.0f * irand(8);
            mapObjRotate(drumname[i].drum[j], 0.0f, 0.0f, data->angle[i][j]);
            data->face[i][j] = UPSIDEGARA[i][j][irand(8)];
            goryuugijo_upsidewindow_disp_sub(i, j);
        }
    }
    return EVT_RETURN_DONE;
}

void goryuugijo_init_slot(EventEntry* event, BOOL isFirstCall) {
    SlotWork* data = ((GoryuuWork*)event->lwData[0])->data;

    data->counter = 0;
    data->reberAngle = 0.0f;
    data->monteEvt = NULL;
    data->flags &= ~1;
}

void slot_neon_sub(GoryuuWork* wp, SlotWork* data) {
    s32 i;
    s32 r;

    if (data->neonCount > 78) {
        return;
    }
    if (data->neonCount == 0) {
        if (irand(10) == 0) {
            data->flags |= 2;
        }
        data->flags &= ~2;
    }
    if (data->neonCount < 27) {
        if (data->neonCount & 1) {
            for (i = 0; i < 3; i++) {
                r = irand(8);
                while (data->face[data->nowSlot][i] == UPSIDEGARA[data->nowSlot][i][r]) {
                    r = (r + 1) & 7;
                }
                data->face[data->nowSlot][i] = UPSIDEGARA[data->nowSlot][i][r];
                if (data->flags & 2) {
                    goryuugijo_upsidewindow_disp_sub(data->nowSlot, i);
                } else {
                    goryuugijo_upsidewindow_dispoff_sub(data->nowSlot, i);
                }
            }
        }
    } else if (data->neonCount == 27) {
        for (i = 0; i < 3; i++) {
            data->face[data->nowSlot][i] = i;
            goryuugijo_upsidewindow_disp_sub(data->nowSlot, i);
        }
        if (data->face[data->nowSlot][0] == data->face[data->nowSlot][1] && data->face[data->nowSlot][1] == data->face[data->nowSlot][2]) {
            data->flags |= 4;
        }
        if ((data->flags & 4) && (data->flags & 2)) {
            data->speed[0] = data->speed[1] = data->speed[2] = 2.8125f;
        } else {
            data->speed[0] = data->speed[1] = data->speed[2] = VERSION_PICK(11.25f, 15.0f);
        }
        data->state[0] = data->state[1] = data->state[2] = 2;
    } else if (data->flags & 4) {
        if (data->neonCount == 33 || data->neonCount == 51 || data->neonCount == 69) {
            for (i = 0; i < 3; i++) {
                goryuugijo_upsidewindow_dispoff_sub(data->nowSlot, i);
            }
        }
        if (data->neonCount == 42 || data->neonCount == 60 || data->neonCount == 78) {
            for (i = 0; i < 3; i++) {
                goryuugijo_upsidewindow_disp_sub(data->nowSlot, i);
            }
        }
    }
    if (data->flags & 2) {
        if (data->neonCount == 0 || data->neonCount == 12 || data->neonCount == 24) {
            mapPlayAnimationLv(drumname[data->nowSlot].neon[0], 0, 0);
        }
        if (data->neonCount == 6 || data->neonCount == 18) {
            mapPlayAnimationLv(drumname[data->nowSlot].neon[1], 0, 0);
        }
    } else if (data->neonCount == 24) {
        mapPlayAnimationLv(drumname[data->nowSlot].neon[0], 0, 0);
    }
    if (data->neonCount <= 78) {
        data->neonCount++;
    }
}

s32 slot_rotate_sub(GoryuuWork* wp, SlotWork* data, s32 reel) {
    s32 stopped = 0;
    f32 angle = data->angle[data->nowSlot][reel];
    f32 speed = data->speed[reel];
    s32 face = (s32)angle / 45.0f;
    s32 cross;
    f32 d;

    if (speed > 0.0f) {
        face = (face + 1) & 7;
        d = 45.0f * face - angle;
        if (d < 0.0f) {
            d += 360.0f;
        }
        if (d <= speed) {
            cross = 1;
        } else {
            cross = 0;
        }
    } else {
        d = 45.0f * face - angle;
        if (d > 0.0f) {
            d -= 360.0f;
        }
        if (d > speed) {
            cross = -1;
        } else {
            cross = 0;
        }
    }
    if ((data->state[reel] == 3 || data->state[reel] == 4) && cross != 0) {
        if (data->stopCount[reel] != 0) {
            if (data->state[reel] == 4) {
                if (data->wait[reel] == 0) {
                    if (data->extra[reel] != 0) {
                        data->extra[reel]--;
                        data->wait[reel] = 0;
                    } else {
                        data->wait[reel] = 18;
                    }
                } else {
                    data->wait[reel]--;
                }
            }
            if (data->wait[reel] != 0) {
                goto rotate;
            }
            data->stopCount[reel]--;
        } else {
            stopped = 1;
            data->state[reel] = 0;
            data->angle[data->nowSlot][reel] = 45.0f * face;
            goto rotate;
        }
    }
    data->angle[data->nowSlot][reel] += data->speed[reel];
    if (data->angle[data->nowSlot][reel] >= 360.0f) {
        data->angle[data->nowSlot][reel] -= 360.0f;
    }
    if (data->angle[data->nowSlot][reel] < 0.0f) {
        data->angle[data->nowSlot][reel] += 360.0f;
    }
rotate:
    mapObjRotate(drumname[data->nowSlot].drum[reel], 0.0f, 0.0f, data->angle[data->nowSlot][reel]);
    return stopped;
}

void goryuugijo_main_slot(EventEntry* event, BOOL isFirstCall) {
    GoryuuWork* wp = (GoryuuWork*)event->lwData[0];
    SlotWork* data = wp->data;
    s32 r;
    const char* sfx;
    s32 dir;
    s32 i;
    s32 j;
    s32 k;
    s32 count;
    s32 press;
    CaseSetup setup;
    s32 tbl[3][3];
    s32 gara[3];
    s32 gara2[3];

    switch (data->phase) {
        case 1:
            data->cursor = 0;
            data->counter = 0;
            data->neonCount = 0;
            data->wait[0] = data->wait[1] = data->wait[2] = 0;
            data->extra[0] = data->extra[1] = data->extra[2] = 0;
            data->state[0] = data->state[1] = data->state[2] = 1;
            data->speed[0] = data->speed[1] = data->speed[2] = 0.0f;
            data->flags &= ~2;
            data->flags &= ~4;
            data->phase++;
            break;
        case 2:
            slot_neon_sub(wp, data);
            if (data->counter >= 15) {
                data->counter = 0;
                data->phase++;
                data->sfxId = psndSFXOn("SFX_EVT_GAME_SLOT_ROUND1");
            } else {
                data->counter++;
                data->reberAngle += 4.0f;
                mapObjRotate(drumname[data->nowSlot].reber, 0.0f, 0.0f, data->reberAngle);
            }
            break;
        case 3:
            slot_neon_sub(wp, data);
            if (data->counter <= 6) {
                if (data->counter >= 6) {
                    data->reberAngle = 0.0f;
                } else {
                    data->reberAngle -= 10.0f;
                }
                mapObjRotate(drumname[data->nowSlot].reber, 0.0f, 0.0f, data->reberAngle);
                data->counter++;
            }
            if (data->state[data->cursor] != 2 || (keyGetDirRep(0) & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT))) {
                if (data->state[data->cursor] != 2) {
                    dir = 1;
                } else {
                    u32 key = keyGetDirRep(0);

                    dir = 1;
                    if (key & 0x4000) {
                        dir = -1;
                    }
                }
                for (i = 0; i < 3; i++) {
                    data->cursor += dir;
                    if (data->cursor < 0) {
                        data->cursor = 2;
                    } else if (data->cursor > 2) {
                        data->cursor = 0;
                    }
                    if (data->state[data->cursor] == 2) {
                        break;
                    }
                }
            }
            sfx = (0, "SFX_EVT_GAME_SLOT_STOP1");
            count = 0;
            for (j = 0; j < 3; j++) {
                if (data->state[j] == 0) {
                    count++;
                    continue;
                }
                if (j == data->cursor) {
                    switch (j) {
                        case 0:
                            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                                press = 1;
                            } else {
                                press = 0;
                            }
                            break;
                        case 1:
                            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                                press = 1;
                            } else {
                                press = 0;
                            }
                            break;
                        case 2:
                            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                                press = 1;
                            } else {
                                press = 0;
                            }
                            break;
                    }
                } else {
                    press = 0;
                }
                if (press && data->state[j] == 2) {
                    data->state[j] = 3;
                    if (data->flags & 2) {
                        data->stopCount[j] = 0;
                    } else {
                        data->stopCount[j] = irand(2);
                    }
                    psndSFXOn(sfx);
                }
                count += slot_rotate_sub(wp, data, j);
            }
            if (count == 3) {
                for (i = 0; i < 3; i++) {
                    goryuugijo_upsidewindow_disp_sub(data->nowSlot, i);
                }
                mapPlayAnimationLv(drumname[data->nowSlot].neon[1], 0, 0);
                data->counter = 0;
                data->phase++;
            }
            break;
        case 4:
            if (data->counter == 0) {
                for (i = 0; i < 3; i++) {
                    r = data->angle[data->nowSlot][i] / 45.0f;
                    gara[i] = SLOTGARA[data->nowSlot][i][r];
                }
                if (gara[0] == gara[1] && gara[1] == gara[2]) {
                    data->phase = 6;
                    break;
                }
                if (!(data->flags & 4)) {
                    data->phase = 6;
                    break;
                }
                count = 0;
                for (i = 0; i < 3; i++) {
                    if (data->face[data->nowSlot][i] == gara[i]) {
                        count++;
                    }
                }
                if (count < 2) {
                    data->phase = 6;
                    break;
                }
            }
            if (data->counter == 0 || data->counter == 18 || data->counter == 36) {
                for (i = 0; i < 3; i++) {
                    goryuugijo_upsidewindow_disp_sub(data->nowSlot, i);
                }
            }
            if (data->counter == 9 || data->counter == 27 || data->counter == 45) {
                for (i = 0; i < 3; i++) {
                    goryuugijo_upsidewindow_dispoff_sub(data->nowSlot, i);
                }
            }
            if (data->counter == 45) {
                for (j = 0; j < 3; j++) {
                    s32 f = data->angle[data->nowSlot][j] / 45.0f;

                    for (k = 0; k < 3; k++) {
                        tbl[k][j] = SLOTGARA[data->nowSlot][j][(f + k - 1) & 7];
                    }
                }
                for (i = 0; i < 3; i++) {
                    if (tbl[1][i] != data->face[data->nowSlot][i]) {
                        r = irand(2);
                        for (k = 0; k < 2; k++) {
                            r = (r + 1) & 1;
                            switch (r) {
                                case 0:
                                    if (data->face[data->nowSlot][i] == tbl[0][i] ||
                                        !(data->face[data->nowSlot][i] == tbl[0][i] || tbl[2][i] == data->face[data->nowSlot][i])) {
                                        if (data->face[data->nowSlot][i] == 0) {
                                            data->stopCount[i] = 1;
                                            data->extra[i] = 1;
                                            data->speed[i] = -1.40625f;
                                        } else {
                                            data->stopCount[i] = 6;
                                            if (data->face[data->nowSlot][i] == 2) {
                                                data->stopCount[i] += 8;
                                            }
                                            data->extra[i] = 0;
                                            data->speed[i] = 1.40625f;
                                        }
                                        data->state[i] = 4;
                                        k = 1;
                                    }
                                    break;
                                case 1:
                                    if (tbl[2][i] == data->face[data->nowSlot][i] ||
                                        !(data->face[data->nowSlot][i] == tbl[0][i] || tbl[2][i] == data->face[data->nowSlot][i])) {
                                        if (data->face[data->nowSlot][i] == 0) {
                                            data->stopCount[i] = 0;
                                            data->extra[i] = 0;
                                            data->speed[i] = 1.40625f;
                                        } else {
                                            data->stopCount[i] = 7;
                                            if (data->face[data->nowSlot][i] == 2) {
                                                data->stopCount[i] += 8;
                                            }
                                            data->extra[i] = 1;
                                            data->speed[i] = -1.40625f;
                                        }
                                        data->state[i] = 4;
                                        k = 1;
                                    }
                                    break;
                            }
                        }
                    }
                }
                data->phase++;
            }
            data->counter++;
            break;
        case 5:
{s32 n; s32 m;            n = 0;
            for (m = 0; m < 3; m++) {
                if (data->state[m] == 0) {
                    n++;
                } else {
                    n += slot_rotate_sub(wp, data, m);
                }
            }
            if (n == 3) {
                data->counter = 0;
                data->phase++;
            }
            break;}
        case 6:
            for (i = 0; i < 3; i++) {
                gara2[i] = SLOTGARA[data->nowSlot][i][(s32)(data->angle[data->nowSlot][i] / 45.0f)];
            }
            if (gara2[0] == gara2[1] && gara2[1] == gara2[2]) {
                data->counter = 0;
                data->phase++;
                psndSFXOn("SFX_EVT_GAME_SLOT_SUCCESS1");
                switch (gara2[0]) {
                    case 0:
                        data->monteCount = 3;
                        break;
                    case 1:
                        data->monteCount = 15;
                        break;
                    case 2:
                        data->monteCount = 100;
                        break;
                }
                caseDelete(data->caseId[0]);
                caseDelete(data->caseId[1]);
                data->flags |= 1;
                mapPlayAnimationLv(drumname[data->nowSlot].neon[2], 1, 0);
                data->flags |= 8;
            } else {
                data->phase = 11;
            }
            psndSFXOff(data->sfxId);
            break;
        case 7:
            if (data->counter >= 48) {
                data->counter = 0;
                data->phase++;
            }
            data->counter++;
            break;
        case 8:
            if (data->counter >= 36) {
                data->counter = 19;
                data->phase++;
            }
            data->counter++;
            break;
        case 9:
            if (data->counter >= 19) {
                data->counter = 0;
                if (data->monteEvt == NULL) {
                    data->monteEvt = evtEntry(slot_fall_monte, 0, 0);
                    data->monteEvt->lwData[0] = event->lwData[0];
                }
                data->phase++;
            } else {
                data->counter++;
            }
            break;
        case 10:
            if (data->monteEvt == NULL || !evtCheckID(data->monteEvt->eventId)) {
                mapPlayAnimationLv(drumname[data->nowSlot].neon[2], 2, 0);
                data->flags &= ~8;
                mapPlayAnimationLv(drumname[data->nowSlot].neon[1], 0, 0);
                data->counter = 0;
                data->monteEvt = NULL;
                data->phase++;
            } else {
                data->counter++;
            }
            break;
        case 11:
            if (data->monteEvt != NULL) {
                monte_p->owner->monteCount = 0;
                _mapFree(monte_p);
                evtDeleteID(data->monteEvt->eventId);
                data->monteEvt = NULL;
            }
            data->counter = 0;
            data->neonCount = 0;
            data->phase = 0;
            data->wait[0] = data->wait[1] = data->wait[2] = 0;
            data->extra[0] = data->extra[1] = data->extra[2] = 0;
            if (data->flags & 1) {
                data->flags &= ~1;
                {s32 n; CaseSetup* s = &setup;
                for (n = 0; n < 2; n++) {
                    s->conditionId = 0x8009;
                    s->hitObjName = drumname[n].anim;
                    s->swFlag = 0;
                    s->activeFunc = NULL;
                    s->evtCode = slotevttbl[n];
                    s->priority = 0;
                    memcpy(s->lwData, event->lwData, sizeof(s->lwData));
                    data->caseId[n] = caseEntry(s);
                }
                }
            }
            if (data->flags & 8) {
                mapPlayAnimationLv(drumname[data->nowSlot].neon[2], 2, 0);
                data->flags &= ~8;
                mapPlayAnimationLv(drumname[data->nowSlot].neon[1], 0, 0);
            }
            break;
    }
}

void goryuugijo_disp_slot(CameraId camera, void* param) {
    SlotWork* data = ((GoryuuWork*)param)->data;
    I3 icons = disp_slot_icons;
    s32 i;
    Vec pos;
    f32 sx;
    f32 sy;
    f32 sz;
    CameraEntry* cam;
    f32 proj[7];
    f32 vp[6];
    Mtx trans;
    Mtx scale;
    Mtx mtx;

    for (i = 0; i < 3; i++) {
        if (data->state[i] == 2 && i == data->cursor) {
            mapObjGetPos(drumname[data->nowSlot].drum[i], &pos);
            pos.x -= 25.0f;
            pos.y += 10.0f;
            cam = camGetPtr(CAMERA_3D);
            GXSetProjection(cam->projection, cam->projType);
            GXGetProjectionv(proj);
            GXGetViewportv(vp);
            GXProject(pos.x, pos.y, pos.z, cam->view, proj, vp, &sx, &sy, &sz);
            cam = camGetPtr(camera);
            GXSetProjection(cam->projection, cam->projType);
            PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
            PSMTXTrans(trans, -304.0f + sx, 240.0f - sy, 0.0f);
            PSMTXConcat(trans, scale, mtx);
            iconDispGx2(mtx, 0x10, ICON_1F2);
            PSMTXTrans(trans, -304.0f + sx, 31.35f + (240.0f - sy), 0.0f);
            PSMTXConcat(trans, scale, mtx);
            ((void (*)(Mtx, s16, u16))iconDispGx2)(mtx, 0x10, icons.v[i]);
        }
    }
}

static USER_FUNC(mario_keyoff_wait) {
    switch (!marioChkKey()) {
        default:
            return EVT_RETURN_BLOCK;
        case 0:
            return EVT_RETURN_YIELD;
    }
}

USER_FUNC(teresa_badge_item_list_init) {
    badgeShop_bteresaGeneration();
    return EVT_RETURN_DONE;
}

USER_FUNC(desk_talk_init_func) {
    npc = _mapAlloc(sizeof(NpcEntry));
    eff = _mapAlloc(sizeof(EffectEntry));
    return EVT_RETURN_DONE;
}

USER_FUNC(desk_talk_func) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 prev = evtGetValue(event, args[1]);
    s32 x = evtGetValue(event, args[2]);
    s32 y = evtGetValue(event, args[3]);
    s32 z = evtGetValue(event, args[4]);

    if (on == 1 && prev == 0) {
        eff = effMarioBalloonEntry(1, npc);
    }
    if (on == 0 && prev == 1) {
        effDelete(eff);
    }
    npc->position.x = x;
    npc->position.y = y;
    npc->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(get_mario_dispReqDir) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();

    evtSetValue(event, args[0], mario->displayDirectionTarget);
    return EVT_RETURN_DONE;
}

USER_FUNC(dummy_shadow) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 size = evtGetFloat(event, args[3]);

    shadowSetCollision(shadowEntry(x, y, z, 10.0f * size), 0);
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(viewport) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 w = evtGetFloat(event, args[2]);
    f32 h = evtGetFloat(event, args[3]);
    CameraEntry* cam;

    cam = camGetPtr(CAMERA_BACKGROUND);
    cam->viewport[0] = x;
    cam->viewport[1] = y;
    cam->viewport[2] = w;
    cam->viewport[3] = h;
    cam = camGetPtr(CAMERA_3D);
    cam->viewport[0] = x;
    cam->scissor[0] = x;
    cam->viewport[1] = y;
    cam->scissor[1] = y;
    cam->viewport[2] = w;
    cam->scissor[2] = w;
    cam->viewport[3] = h;
    cam->scissor[3] = h;
    cam = camGetPtr(CAMERA_2D);
    cam->viewport[0] = x;
    cam->viewport[1] = y;
    cam->viewport[2] = w;
    cam->viewport[3] = h;
    return EVT_RETURN_DONE;
}

void kuji_bomb_055_callback(s32 type, MsgWinData** work) {
    MsgWinData* wp = *work;
    NpcEntry* npc = npcNameToPtr("\203O\203\213\203\201\203{\203\200\225\272");

    switch (type) {
        case 3:
            if (wp->curPage == 1) {
                evtSetValue(NULL, GF(0), 1);
            }
            break;
        case 5:
            switch (wp->nextEntry) {
                case 0x1D:
                    animPoseSetAnim(npc->poseId, "T_3", 1);
                    break;
                case 0xC8:
                    animPoseSetAnim(npc->poseId, "T_4", 1);
                    break;
                case 0x158:
                    animPoseSetAnim(npc->poseId, "T_3", 1);
                    break;
                case 0x1F1:
                    animPoseSetAnim(npc->poseId, "T_4", 1);
                    break;
                case 0x2B7:
                case 0x2B8:
                    npc->flags &= ~0x200;
                    animPoseSetAnim(npc->poseId, "B_1", 1);
                    break;
            }
            break;
    }
}

USER_FUNC(kuzi_get_keihin) {
    EvtWord* args = event->args;
    s32 item;

    switch (evtGetValue(event, args[0])) {
        case 0:
            if (!evtGetValue(NULL, GSWF(1238))) {
                item = 0x104;
                evtSetValue(NULL, GSWF(1238), 1);
            } else {
                item = 0xC4;
            }
            break;
        case 1:
            if (!evtGetValue(NULL, GSWF(1239))) {
                item = 0x12F;
                evtSetValue(NULL, GSWF(1239), 1);
            } else {
                item = 0xC2;
            }
            break;
        case 2:
            item = 0x96;
            break;
        case 3:
            item = 0x97;
            break;
        default:
            item = 0x94;
            break;
    }
    evtSetValue(event, args[1], item);
    return EVT_RETURN_DONE;
}

USER_FUNC(kuzi_bomb_bomb_eff) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 phase = (0, evtGetValue(event, args[1]));
    NpcEntry* npc = npcNameToPtr("\203O\203\213\203\201\203{\203\200\225\272");

    if (type == 2 || type == 3) {
        return EVT_RETURN_DONE;
    }
    switch (phase) {
        case 0:
            if (npc->position.y > 25.0f) {
                effBombEntry(0, npc->position.x - 30.0f, npc->position.y, npc->position.z, 1.0f);
                if (type == 0) {
                    phase = 1;
                } else {
                    phase = 2;
                }
            }
            break;
        case 1:
            if (npc->position.y > 50.0f) {
                effBombEntry(0, 30.0f + npc->position.x, npc->position.y, npc->position.z, 1.0f);
                phase = 2;
            }
            break;
        case 2:
            if (npc->position.y > 75.0f) {
                effBombEntry(0, npc->position.x - 30.0f, npc->position.y, npc->position.z, 1.0f);
                phase = 3;
            }
            break;
        case 3:
            if (npc->position.y > 100.0f) {
                effBombEntry(0, 30.0f + npc->position.x, npc->position.y, npc->position.z, 1.0f);
                phase = 4;
            }
            break;
    }
    evtSetValue(event, args[2], phase);
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}
