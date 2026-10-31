// aji_06 (敵のアジト, X-Naut Fortress): Peach's potion lab: she mixes the coloured medicines (kusuri) in the
// machine, shrinks and grows, and turns invisible (peach_evt_kusuri_nomu).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_pouch.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "mario/mario.h"
#include "dolphin/gx.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "aji.h"
#include "driver/seqdrv.h"

typedef struct YukaAnimData { u16 flags; u8 field_0x2[6]; OSTime time; f32 frame; f32 speed; struct { u8 header[8]; f32 length; }* data; } YukaAnimData;
typedef struct YukaAnim {
    YukaAnimData* anim;
    u32 flags;
} YukaAnim;
#define YUKA_NOW() OSTicksToMilliseconds(gp->inBattle ? gp->renderTime : gp->renderFieldTime)
YukaAnim yuka_anim;
s32 g_Kusuri1_No;
s32 g_Kusuri_B;
s32 g_Kusuri_G;
s32 g_Kusuri_R;

USER_FUNC(kusuri_color_get);
USER_FUNC(kusuri_color_init);
USER_FUNC(kusuri_color_set);
USER_FUNC(kusuri_color_set_rgb);
USER_FUNC(kusuri_list_add);
USER_FUNC(kusuri_list_get_no);
USER_FUNC(kusuri_list_init);
USER_FUNC(peach_chg_pose);
USER_FUNC(peach_evt_seikai_check);
USER_FUNC(peach_evt_toumei_onoff);
USER_FUNC(peach_set_scale);
USER_FUNC(yuka_anim_reverse_end);
USER_FUNC(yuka_anim_reverse_play);
USER_FUNC(yuka_anim_reverse_pause);
USER_FUNC(yuka_anim_reverse_replay);
void kusuri_4_color_tev(MapTevCallbackInfo* info);
void kusuri_3_color_tev(MapTevCallbackInfo* info);
void kusuri_2_color_tev(MapTevCallbackInfo* info);
void kusuri_1_color_tev(MapTevCallbackInfo* info);
void kusuri_naka_color_tev(MapTevCallbackInfo* info);

static EvtWord s_door_open[22];
static EvtWord s_door_close[22];
static BeroEntry bero_entry_data[2];
static EvtWord bero_peach_evt[31];
static BeroEntry bero_peach_data[2];
GXColor g_Kusuri_Col;
GXColor g_Kusuri_Src_Col[4];
s32 g_Kusuri2_No;
s32 g_Kusuri3_No;
s32 g_Kusuri4_No;
EvtWord peach_evt_kaihei_open[51];
EvtWord peach_evt_kaihei_open_all[28];
EvtWord peach_evt_kaihei_close[46];
EvtWord peach_evt_kaihei_close_all[28];
s32 kusuri_tyogo_list[8];
s32 kusuri_list[5];
s32 kusuri_id[4];
EvtWord peach_evt_kusuri_select[73];
EvtWord peach_evt_pick_kusuri[55];
EvtWord peach_evt_check_all_kusuri_set[72];
EvtWord peach_evt_kusuri_dasu[141];
EvtWord peach_evt_seikai_evt[33];
EvtWord peach_evt_kaihei1[187];
EvtWord peach_evt_kaihei2[174];
EvtWord peach_evt_kaihei3[174];
EvtWord peach_evt_kaihei4[174];
EvtWord peach_evt_stop_renji[15];
EvtWord peach_evt_pato_on[46];
EvtWord peach_evt_pato_off[18];
EvtWord peach_evt_hari_on[100];
EvtWord peach_evt_hari_off[27];
EvtWord peach_evt_sugata_modoru[85];
EvtWord peach_evt_kusuri_nomu[435];
EvtWord kusuri_color_seiko[52];
EvtWord kusuri_color_sippai[93];
s32 peach_evt_bika_move_data[33];
EvtWord peach_evt_tyogo_sub[432];
EvtWord peach_evt_tyogo[557];
EvtWord peach_evt_tyogo_entry[6];
EvtWord peach_evt_kusuri_aka[19];
EvtWord peach_evt_kusuri_ao[19];
EvtWord peach_evt_kusuri_ki[19];
EvtWord peach_evt_kusuri_mid[19];
EvtWord peach_evt_wait_memo[18];
EvtWord peach_evt_memo1[20];
EvtWord peach_evt_memo2[20];
EvtWord peach_evt_memo3[20];
EvtWord peach_evt_memo4[20];
static EvtWord peach_evt_stg5[385];
EvtWord peach_evt_matigai_kusuri[14];
EvtWord peach_evt_tadashii_kusuri[155];
EvtWord peach_evt_door[75];
static EvtWord peach_evt_stg5_1[258];
EvtWord kikai_init[137];
static EvtWord evt_memo1[14];
static EvtWord evt_memo2[14];
EvtWord evt_memo3[14];
EvtWord evt_memo4[14];
EvtWord aji_06_init_evt[];

/**
 * Door open animation step: moves the two door leaves S_door1_2
 * and S_door1_1 in opposite directions along X by the animation value in LW(0) times 0.16601563;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(s_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Door close animation step: moves the two door leaves S_door1_2
 * and S_door1_1 in opposite directions along X by the animation value in LW(0) times 0.16601563;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(s_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

/** Exit of this room: the south door to aji_17. */
static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "n_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Exit event of the room's door: freezes Mario and, at STG5_44_PEACH_TECS_REQUEST_2,
 * prints peach_aji_226 (TEC has locked the door until the potion is done; LSWF(11) is raised while the text
 * is open) and returns LW(0) = 1, which makes bero_case_init_check cancel the exit; otherwise LW(0) = 0.
 */
static EVT_BEGIN(bero_peach_evt)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_44_PEACH_TECS_REQUEST_2)
        SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 1)
        CALL(evt_msg_print, 0, STRING("peach_aji_226"), 0, 0)
        SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
        SET_VALUE(LW(0), 1)
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Replacement for the south door exit, swapped in with evt_bero_overwrite during Peach's scene so it leads
 * on to aji_01 (n_bero_1).
 */
static BeroEntry bero_peach_data[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        bero_peach_evt, // exitEvent
        "aji_01", // nextMapName
        "n_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

/** The colour of the potion currently in the beaker, packed as RGBA (initially opaque). */
GXColor g_Kusuri_Col = {0, 0, 0, 255};

/** Base colours of the four potions in RGBA: red, blue, yellow and green. */
GXColor g_Kusuri_Src_Col[] = {
    {255, 0, 0, 255},
    {0, 0, 255, 255},
    {255, 255, 0, 255},
    {0, 255, 0, 255},
};

s32 g_Kusuri2_No = 1;

/** Potion number (minus 1) placed in the third mixing slot. */
s32 g_Kusuri3_No = 2;

/** Potion number (minus 1) placed in the fourth mixing slot. */
s32 g_Kusuri4_No = 3;

/**
 * Opens one mixing-slot hatch (the map object named in LW(0), S_kaihei1 to 4):
 * slides it down 25 units over 0.8 seconds with SFX_PEACH_MACHINE_OPEN1, unless it is already open.
 */
EVT_BEGIN(peach_evt_kaihei_open)
    SET_VALUE(LW(5), LW(0))
    CALL(evt_mapobj_get_position, LW(5), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 45)
        RETURN
    END_IF
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_OPEN1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -25, 800)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 0, LW(5), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_snd_sfxoff, LW(3))
    RETURN
EVT_END()

/** Opens all four hatches of the potion machine at once (S_kaihei1 to S_kaihei4). */
EVT_BEGIN(peach_evt_kaihei_open_all)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei1"))
        CALL_CHILD(PTR(peach_evt_kaihei_open))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei2"))
        CALL_CHILD(PTR(peach_evt_kaihei_open))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei3"))
        CALL_CHILD(PTR(peach_evt_kaihei_open))
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("S_kaihei4"))
    CALL_CHILD(PTR(peach_evt_kaihei_open))
    RETURN
EVT_END()

/** Closes one mixing-slot hatch (LW(0)): slides it back up 25 units with SFX_PEACH_MACHINE_OPEN1. */
EVT_BEGIN(peach_evt_kaihei_close)
    SET_VALUE(LW(5), LW(0))
    CALL(evt_mapobj_get_position, LW(5), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_OPEN1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -25, 0, 800)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 0, LW(5), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(3))
    RETURN
EVT_END()

/** Closes all four hatches of the potion machine at once. */
EVT_BEGIN(peach_evt_kaihei_close_all)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei1"))
        CALL_CHILD(PTR(peach_evt_kaihei_close))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei2"))
        CALL_CHILD(PTR(peach_evt_kaihei_close))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_kaihei3"))
        CALL_CHILD(PTR(peach_evt_kaihei_close))
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("S_kaihei4"))
    CALL_CHILD(PTR(peach_evt_kaihei_close))
    RETURN
EVT_END()

/** The correct recipe: the order in which the four potions must be dispensed into the beaker. */
s32 kusuri_tyogo_list[] = {
    1, 2, 3, 2, 3, 1, 3, 4,
};

/** Potions Peach has placed in the machine so far (-1 for an empty slot). */
s32 kusuri_list[] = {
    -1, -1, -1, -1, -1,
};

/** Ids of the four potion bottles, indexed by potion number minus 1. */
s32 kusuri_id[] = {
    102, 101, 103, 104,
};

/**
 * Peach's potion pick: builds the list of potions she still holds (red, blue, orange, green,
 * from the local flags LSWF(1) to 4) and shows the item menu.
 * Returns in LW(0) the potion number (1 to 4) she chose, or 0 if the menu was cancelled or she has no potion.
 */
EVT_BEGIN(peach_evt_kusuri_select)
    CALL(kusuri_list_init)
    SET_VALUE(LF(0), 0)
    IF_INT_NOT_EQUAL(LSWF_AJI_06_BLUE_POTION_HELD, 0)
        CALL(kusuri_list_add, 2)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LSWF_AJI_06_RED_POTION_HELD, 0)
        CALL(kusuri_list_add, 1)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LSWF_AJI_06_ORANGE_POTION_HELD, 0)
        CALL(kusuri_list_add, 3)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LSWF_AJI_06_GREEN_POTION_HELD, 0)
        CALL(kusuri_list_add, 4)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_win_item_select, 0, PTR(kusuri_list), LW(1), 0)
        ELSE
        SET_VALUE(LW(1), -1)
    END_IF
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(kusuri_list_get_no, LW(1), LW(0))
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    RETURN
EVT_END()

/**
 * Takes the potion LW(0) out of a slot: sets the matching "have it" flag (LSWF(1) to 4),
 * removes the potion item object (kusuri_aka, kusuri_ao, kusuri_ki or kusuri_mid) and,
 * if the machine had been started (LSWF(5)), cancels the start and silences the siren.
 */
EVT_BEGIN(peach_evt_pick_kusuri)
    SWITCH(LW(0))
        CASE_EQUAL(1)
        SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 1)
        CALL(evt_item_delete, STRING("kusuri_aka"))
        CASE_EQUAL(2)
        SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 1)
        CALL(evt_item_delete, STRING("kusuri_ao"))
        CASE_EQUAL(3)
        SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 1)
        CALL(evt_item_delete, STRING("kusuri_ki"))
        CASE_EQUAL(4)
        SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 1)
        CALL(evt_item_delete, STRING("kusuri_mid"))
    END_SWITCH
    IF_INT_EQUAL(LSWF_AJI_06_MIX_START_OFFERED, 1)
        CALL(evt_del_case_evt, 0, GW(5))
        DELETE_EVENT(GW(0))
        SET_VALUE(LW(0), STRING("S_pato1"))
        CALL_CHILD(PTR(peach_evt_pato_off))
    END_IF
    SET_VALUE(LSWF_AJI_06_MIX_START_OFFERED, 0)
    RETURN
EVT_END()

/**
 * Checks whether all four slots are filled (their potion numbers sum to 10, which is 1+2+3+4).
 * If so, asks whether to start mixing (peach_aji_227);
 * on yes the mixing case script (peach_evt_tyogo) is attached to the start switch A_sui1
 * and the siren S_pato1 starts to turn.
 */
EVT_BEGIN(peach_evt_check_all_kusuri_set)
    SET_VALUE(LW(0), LSW_AJI_06_SLOT_1_POTION)
    ADD(LW(0), LSW_AJI_06_SLOT_2_POTION)
    ADD(LW(0), LSW_AJI_06_SLOT_3_POTION)
    ADD(LW(0), LSW_AJI_06_SLOT_4_POTION)
    IF_INT_EQUAL(LSWF_AJI_06_DRINK_PENDING, 1)
        RETURN
    END_IF
    // Test the slot sum; uniqueness follows from the held-potion bookkeeping.
    IF_INT_EQUAL(LW(0), 10)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_227"), 0, 0)
        CALL(evt_msg_select, 0, STRING("peach_aji_227_sel"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("peach_aji_229"))
            SET_VALUE(LSWF_AJI_06_MIX_START_OFFERED, 1)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(peach_evt_tyogo_entry), GW(5))
            SET_VALUE(LW(0), STRING("S_pato1"))
            SPAWN_EVENT_GET_ID(PTR(peach_evt_pato_on), GW(0))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("peach_aji_228"))
            SET_VALUE(LSWF_AJI_06_MIX_START_OFFERED, 0)
        END_IF
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Pours potion number LW(1) through the stream object LW(0) (S_mizu1 to 4): plays the map animation "mizu"
 * with SFX_PEACH_MACHINE_MEDICINE1 and scales the stream up, holds it and scales it down over that time.
 * If a press was accepted in the timing window (LSWF(7)), the beaker contents S_naka also rise by an eighth
 * (GW(10)) and the potion's colour is blended in (kusuri_color_set); LSWF(7) is cleared at the end.
 */
EVT_BEGIN(peach_evt_kusuri_dasu)
    SET_VALUE(LW(5), LW(0))
    SET_VALUE(LW(6), LW(1))
    CALL(evt_map_playanim, STRING("mizu"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("mizu"), LW(0), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_MEDICINE1"), 0, 0, -100, 0)
    IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_PRESSED, 1)
        SET_VALUE(LW(3), LW(2))
        MULTIPLY_FLOAT(LW(3), FLOAT(0.7998047f))
        BEGIN_INLINE_EVENT
            CALL(kusuri_color_set, LW(6), LW(3))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 125, LW(3))
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            ADD_FLOAT(LW(0), GW(10))
            CALL(evt_mapobj_scale, 0, STRING("S_naka"), 1, LW(0), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
            ADD_FLOAT(GW(10), FLOAT(0.125f))
        END_INLINE_EVENT
    END_IF
    DIVIDE(LW(2), 3)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1000, LW(2))
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mapobj_scale, 0, LW(5), 1, LW(0), 1)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1000, 0, LW(2))
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mapobj_scale, 0, LW(5), 1, LW(0), 1)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 0)
    RETURN
EVT_END()

/**
 * Flashes the pair of map objects LW(0) and LW(1) (shown and hidden alternately) three times at 100 ms each;
 * the slot scripts run it on their lamp pair (hika1 and kesu1, ...) after a correct pour.
 */
EVT_BEGIN(peach_evt_seikai_evt)
    LOOP(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
    END_LOOP
    RETURN
EVT_END()

/**
 * Case script of the first mixing slot (A_sui2).
 * While the machine is idle (LSWF(6) = 0): shows the potion menu
 * and puts the chosen potion in the slot (at x = -110)
 * or takes the current one out again (peach_evt_pick_kusuri), then checks whether all slots are full.
 * While the machine is running (LSWF(6) = 1): pours the slot's potion (peach_evt_kusuri_dasu).
 * The pour counts only when the beaker is at this slot (GW(12) = 0) and the press window is open (LSWF(8),
 * from 400 ms before the beaker stops until 3 seconds after); it is then checked against step GW(11)
 * of kusuri_tyogo_list (peach_evt_seikai_check): right adds 1 to the score GW(13) and flashes the slot's lamps,
 * wrong subtracts 1.
 */
EVT_BEGIN(peach_evt_kaihei1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LSWF_AJI_06_MIXING_RUNNING, 0)
        IF_INT_EQUAL(LSW_AJI_06_SLOT_1_POTION, 0)
            CALL_CHILD(PTR(peach_evt_kusuri_select))
            SET_VALUE(LSW_AJI_06_SLOT_1_POTION, LW(0))
            SWITCH(LSW_AJI_06_SLOT_1_POTION)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), STRING("kusuri_aka"))
                SET_VALUE(LW(1), 102)
                SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 0)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), STRING("kusuri_ao"))
                SET_VALUE(LW(1), 101)
                SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 0)
                CASE_EQUAL(3)
                SET_VALUE(LW(0), STRING("kusuri_ki"))
                SET_VALUE(LW(1), 103)
                SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 0)
                CASE_EQUAL(4)
                SET_VALUE(LW(0), STRING("kusuri_mid"))
                SET_VALUE(LW(1), 104)
                SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 0)
            END_SWITCH
            IF_INT_GREATER(LSW_AJI_06_SLOT_1_POTION, 0)
                CALL(evt_item_entry, LW(0), LW(1), -110, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_pouch_check_item, LW(1), LW(2))
                IF_INT_GREATER(LW(2), 0)
                    CALL(evt_pouch_remove_item, LW(1), LW(2))
                END_IF
            END_IF
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_1_POTION)
            SUBTRACT(LW(0), 1)
            POKE_INT(LW(0), PTR(&g_Kusuri1_No))
            CALL_CHILD(PTR(peach_evt_check_all_kusuri_set))
            CALL(evt_map_set_flush_onoff, 0, 1, STRING("S_mizu1"))
            CALL(evt_map_set_flush_color, 0, STRING("S_mizu1"), 255, 0, 0, 255)
            ELSE
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_1_POTION)
            CALL_CHILD(PTR(peach_evt_pick_kusuri))
            SET_VALUE(LSW_AJI_06_SLOT_1_POTION, 0)
        END_IF
        ELSE
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(GW(12), 0)
            IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
                SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 1)
                CALL(peach_evt_seikai_check, GW(11), LSW_AJI_06_SLOT_1_POTION, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    ADD(GW(13), 1)
                    SET_VALUE(LF(0), 1)
                    ELSE
                    SUBTRACT(GW(13), 1)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(0), STRING("S_mizu1"))
        SET_VALUE(LW(1), LSW_AJI_06_SLOT_1_POTION)
        CALL_CHILD(PTR(peach_evt_kusuri_dasu))
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(0), STRING("hika1"))
            SET_VALUE(LW(1), STRING("kesu1"))
            SPAWN_EVENT(PTR(peach_evt_seikai_evt))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Case script of the second mixing slot (A_sui3):
 * the same as peach_evt_kaihei1 for slot 2 (potion placed at x = -65).
 */
EVT_BEGIN(peach_evt_kaihei2)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LSWF_AJI_06_MIXING_RUNNING, 0)
        IF_INT_EQUAL(LSW_AJI_06_SLOT_2_POTION, 0)
            CALL_CHILD(PTR(peach_evt_kusuri_select))
            SET_VALUE(LSW_AJI_06_SLOT_2_POTION, LW(0))
            SWITCH(LSW_AJI_06_SLOT_2_POTION)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), STRING("kusuri_aka"))
                SET_VALUE(LW(1), 102)
                SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 0)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), STRING("kusuri_ao"))
                SET_VALUE(LW(1), 101)
                SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 0)
                CASE_EQUAL(3)
                SET_VALUE(LW(0), STRING("kusuri_ki"))
                SET_VALUE(LW(1), 103)
                SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 0)
                CASE_EQUAL(4)
                SET_VALUE(LW(0), STRING("kusuri_mid"))
                SET_VALUE(LW(1), 104)
                SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 0)
            END_SWITCH
            IF_INT_GREATER(LSW_AJI_06_SLOT_2_POTION, 0)
                CALL(evt_item_entry, LW(0), LW(1), -65, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_pouch_check_item, LW(1), LW(2))
                IF_INT_GREATER(LW(2), 0)
                    CALL(evt_pouch_remove_item, LW(1), LW(2))
                END_IF
            END_IF
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_2_POTION)
            SUBTRACT(LW(0), 1)
            POKE_INT(LW(0), PTR(&g_Kusuri2_No))
            CALL_CHILD(PTR(peach_evt_check_all_kusuri_set))
            ELSE
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_2_POTION)
            CALL_CHILD(PTR(peach_evt_pick_kusuri))
            SET_VALUE(LSW_AJI_06_SLOT_2_POTION, 0)
        END_IF
        ELSE
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(GW(12), 1)
            IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
                SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 1)
                CALL(peach_evt_seikai_check, GW(11), LSW_AJI_06_SLOT_2_POTION, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    ADD(GW(13), 1)
                    SET_VALUE(LF(0), 1)
                    ELSE
                    SUBTRACT(GW(13), 1)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(0), STRING("S_mizu2"))
        SET_VALUE(LW(1), LSW_AJI_06_SLOT_2_POTION)
        CALL_CHILD(PTR(peach_evt_kusuri_dasu))
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(0), STRING("hika2"))
            SET_VALUE(LW(1), STRING("kesu2"))
            SPAWN_EVENT(PTR(peach_evt_seikai_evt))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Case script of the third mixing slot (A_sui4):
 * the same as peach_evt_kaihei1 for slot 3 (potion placed at x = -10).
 */
EVT_BEGIN(peach_evt_kaihei3)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LSWF_AJI_06_MIXING_RUNNING, 0)
        IF_INT_EQUAL(LSW_AJI_06_SLOT_3_POTION, 0)
            CALL_CHILD(PTR(peach_evt_kusuri_select))
            SET_VALUE(LSW_AJI_06_SLOT_3_POTION, LW(0))
            SWITCH(LSW_AJI_06_SLOT_3_POTION)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), STRING("kusuri_aka"))
                SET_VALUE(LW(1), 102)
                SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 0)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), STRING("kusuri_ao"))
                SET_VALUE(LW(1), 101)
                SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 0)
                CASE_EQUAL(3)
                SET_VALUE(LW(0), STRING("kusuri_ki"))
                SET_VALUE(LW(1), 103)
                SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 0)
                CASE_EQUAL(4)
                SET_VALUE(LW(0), STRING("kusuri_mid"))
                SET_VALUE(LW(1), 104)
                SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 0)
            END_SWITCH
            IF_INT_GREATER(LSW_AJI_06_SLOT_3_POTION, 0)
                CALL(evt_item_entry, LW(0), LW(1), -10, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_pouch_check_item, LW(1), LW(2))
                IF_INT_GREATER(LW(2), 0)
                    CALL(evt_pouch_remove_item, LW(1), LW(2))
                END_IF
            END_IF
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_3_POTION)
            SUBTRACT(LW(0), 1)
            POKE_INT(LW(0), PTR(&g_Kusuri3_No))
            CALL_CHILD(PTR(peach_evt_check_all_kusuri_set))
            ELSE
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_3_POTION)
            CALL_CHILD(PTR(peach_evt_pick_kusuri))
            SET_VALUE(LSW_AJI_06_SLOT_3_POTION, 0)
        END_IF
        ELSE
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(GW(12), 2)
            IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
                SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 1)
                CALL(peach_evt_seikai_check, GW(11), LSW_AJI_06_SLOT_3_POTION, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    ADD(GW(13), 1)
                    SET_VALUE(LF(0), 1)
                    ELSE
                    SUBTRACT(GW(13), 1)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(0), STRING("S_mizu3"))
        SET_VALUE(LW(1), LSW_AJI_06_SLOT_3_POTION)
        CALL_CHILD(PTR(peach_evt_kusuri_dasu))
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(0), STRING("hika3"))
            SET_VALUE(LW(1), STRING("kesu3"))
            SPAWN_EVENT(PTR(peach_evt_seikai_evt))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Case script of the fourth mixing slot (A_sui5):
 * the same as peach_evt_kaihei1 for slot 4 (potion placed at x = 35).
 */
EVT_BEGIN(peach_evt_kaihei4)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LSWF_AJI_06_MIXING_RUNNING, 0)
        IF_INT_EQUAL(LSW_AJI_06_SLOT_4_POTION, 0)
            CALL_CHILD(PTR(peach_evt_kusuri_select))
            SET_VALUE(LSW_AJI_06_SLOT_4_POTION, LW(0))
            SWITCH(LSW_AJI_06_SLOT_4_POTION)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), STRING("kusuri_aka"))
                SET_VALUE(LW(1), 102)
                SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 0)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), STRING("kusuri_ao"))
                SET_VALUE(LW(1), 101)
                SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 0)
                CASE_EQUAL(3)
                SET_VALUE(LW(0), STRING("kusuri_ki"))
                SET_VALUE(LW(1), 103)
                SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 0)
                CASE_EQUAL(4)
                SET_VALUE(LW(0), STRING("kusuri_mid"))
                SET_VALUE(LW(1), 104)
                SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 0)
            END_SWITCH
            IF_INT_GREATER(LSW_AJI_06_SLOT_4_POTION, 0)
                CALL(evt_item_entry, LW(0), LW(1), 35, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_pouch_check_item, LW(1), LW(2))
                IF_INT_GREATER(LW(2), 0)
                    CALL(evt_pouch_remove_item, LW(1), LW(2))
                END_IF
            END_IF
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_4_POTION)
            SUBTRACT(LW(0), 1)
            POKE_INT(LW(0), PTR(&g_Kusuri4_No))
            CALL_CHILD(PTR(peach_evt_check_all_kusuri_set))
            ELSE
            SET_VALUE(LW(0), LSW_AJI_06_SLOT_4_POTION)
            CALL_CHILD(PTR(peach_evt_pick_kusuri))
            SET_VALUE(LSW_AJI_06_SLOT_4_POTION, 0)
        END_IF
        ELSE
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(GW(12), 3)
            IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
                SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 1)
                CALL(peach_evt_seikai_check, GW(11), LSW_AJI_06_SLOT_4_POTION, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    ADD(GW(13), 1)
                    SET_VALUE(LF(0), 1)
                    ELSE
                    SUBTRACT(GW(13), 1)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(0), STRING("S_mizu4"))
        SET_VALUE(LW(1), LSW_AJI_06_SLOT_4_POTION)
        CALL_CHILD(PTR(peach_evt_kusuri_dasu))
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(0), STRING("hika4"))
            SET_VALUE(LW(1), STRING("kesu4"))
            SPAWN_EVENT(PTR(peach_evt_seikai_evt))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Case script of the machine's stop switch (A_sui6):
 * in the last step (GW(11) = 8) while the machine is heating (LSWF(8)) it sets LSWF(7),
 * which stops the heating timer.
 */
EVT_BEGIN(peach_evt_stop_renji)
    IF_INT_EQUAL(GW(11), 8)
        IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
            SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 1)
        END_IF
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Starts a warning siren: turns the map object named in LW(0) (S_pato1, 2 or 3) around its Y axis at 12 degrees
 * per 30 ms and plays SFX_PEACH_MACHINE_SIREN1. Runs until deleted.
 */
EVT_BEGIN(peach_evt_pato_on)
    SET_VALUE(LW(15), LW(0))
    CALL(evt_mapobj_get_position, LW(15), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_SIREN1"), LW(0), LW(1), LW(2), GW(4))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, LW(15), MAP_OBJ_FLAG_HIDDEN)
    LOOP(0)
        CALL(evt_mapobj_rotate, 0, LW(15), 0, LW(0), 0)
        ADD(LW(0), 12)
        IF_INT_GREATER_OR_EQUAL(LW(0), 360)
            SUBTRACT(LW(0), 360)
        END_IF
        WAIT_MS(30)
    END_LOOP
    RETURN
EVT_END()

/** Stops the siren named in LW(0): hides it, resets its rotation and stops the siren sound. */
EVT_BEGIN(peach_evt_pato_off)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_rotate, 0, LW(0), 0, 0, 0)
    CALL(evt_snd_sfxoff, GW(4))
    RETURN
EVT_END()

/**
 * Swings the machine's needle (hari) 130 units out over 0.5 seconds and keeps it moving back and forth
 * while the machine runs.
 */
EVT_BEGIN(peach_evt_hari_on)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -130, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 0, STRING("hari"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(LW(4), -130)
    LOOP(0)
        CALL(evt_sub_random, 15, LW(2))
        ADD(LW(2), -130)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(4), LW(2), 600)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 0, STRING("hari"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_random, 20, LW(3))
        SET_VALUE(LW(4), -130)
        SUBTRACT(LW(4), LW(3))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(2), LW(4), 600)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 0, STRING("hari"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_LOOP
    RETURN
EVT_END()

/** Returns the machine's needle (hari) to its rest position. */
EVT_BEGIN(peach_evt_hari_off)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, -130, 0, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 0, STRING("hari"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

/**
 * After Peach has grown or shrunk from a wrong potion,
 * scales her back to normal size over time (peach_set_scale), starting from big or small depending on LSW(5).
 */
EVT_BEGIN(peach_evt_sugata_modoru)
    WAIT_MS(15000)
    SWITCH(LSW_AJI_06_WRONG_POTION_EFFECT)
        CASE_EQUAL(0)
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MINIMINI1"), LW(3), LW(4), LW(5), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 2000, 1000, 1000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(peach_set_scale, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CASE_EQUAL(1)
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DEKADEKA1"), LW(3), LW(4), LW(5), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 500, 1000, 1000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(peach_set_scale, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Peach drinks the mixed potion (case script on the machine's cup A_bika1): she gulps it with a puff of smoke.
 * If the mix was wrong (LSWF(9) = 0) she randomly grows huge or shrinks tiny, comments (peach_aji_234 to 236),
 * returns to her size and the machine resets so she can try again.
 * If it was right she turns invisible (SFX_PEACH_TOUMEI1, peach_evt_toumei_onoff, evt_peach_set_condition 2),
 * the scene plays (peach_aji_237 to 242) and the story becomes STG5_45_PEACH_POTION_PUZZLE.
 */
EVT_BEGIN(peach_evt_kusuri_nomu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_adjust_dir)
    // Peach drinks the potion and a cloud of smoke covers her.
    CALL(peach_chg_pose, STRING("P_M_1"))
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_bika"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DRINK_MEDICINE1"), LW(0), LW(1), LW(2), 0)
    CALL(peach_chg_pose, STRING("P_M_3"))
    WAIT_MS(500)
    CALL(evt_mapobj_scale, 0, STRING("S_naka"), 0, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 13, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(100)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SURPRISED1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_bika"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_EQUAL(LSWF_AJI_06_MIX_CORRECT, 0)
        WAIT_MS(200)
        CALL(evt_sub_random, 1, LW(0))
        SET_VALUE(LW(2), LW(0))
        SWITCH(LW(2))
            CASE_EQUAL(0)
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DEKADEKA1"), LW(3), LW(4), LW(5), 0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1000, 2000, 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(peach_set_scale, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CASE_EQUAL(1)
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MINIMINI1"), LW(3), LW(4), LW(5), 0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1000, 500, 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(peach_set_scale, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
        END_SWITCH
        SET_VALUE(LSW_AJI_06_WRONG_POTION_EFFECT, LW(2))
        // She panics, TEC calls it unsuccessful and asks her to start over.
        CALL(evt_msg_print, 0, STRING("peach_aji_234"), 0, STRING("mario"))
        CALL(evt_msg_print, 0, STRING("peach_aji_235"), 0, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_236"), 0, STRING("mario"))
        // Peach returns to normal, the machine is reset and the buttons can be used again.
        SPAWN_EVENT(PTR(peach_evt_sugata_modoru))
        CALL_CHILD(PTR(peach_evt_kaihei_open_all))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(peach_evt_tyogo_entry), GW(5))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(peach_evt_kaihei1), GW(6))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(peach_evt_kaihei2), GW(7))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui4"), 0, PTR(peach_evt_kaihei3), GW(8))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui5"), 0, PTR(peach_evt_kaihei4), GW(9))
        SET_VALUE(LW(0), STRING("S_pato1"))
        SPAWN_EVENT_GET_ID(PTR(peach_evt_pato_on), GW(0))
        ELSE
        // The right potion: the music fades and Peach becomes invisible.
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_TOUMEI1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_peach_set_condition, 2)
        WAIT_MS(1000)
        // TEC congratulates her and asks her to take her clothes off; Peach objects but agrees.
        CALL(evt_msg_print, 0, STRING("peach_aji_237"), 0, 0)
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_3"), STRING("P_T_3"))
        CALL(evt_msg_print, 0, STRING("peach_aji_238"), 0, STRING("mario"))
        WAIT_MS(2000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 15)
        CALL(evt_mario_mov_pos2, LW(0), LW(2), 30)
        CALL(evt_msg_print, 0, STRING("peach_aji_239"), 0, STRING("mario"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        // Peach walks over toward the door of the lab.
        CALL(evt_mario_mov_pos2, 114, 76, FLOAT(72.0f))
        CALL(evt_mario_mov_pos2, 90, 104, FLOAT(72.0f))
        CALL(evt_mario_mov_pos2, -80, 115, FLOAT(72.0f))
        WAIT_MS(1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfx_dist, LW(3), 0)
        WAIT_MS(2000)
        CALL(peach_evt_toumei_onoff, 1)
        CALL(evt_mario_mov_pos2, 100, 115, FLOAT(72.0f))
        // TEC explains the plan: find the data disk in Sir Grodus's room and insert it into the computer.
        CALL(evt_msg_print, 0, STRING("peach_aji_240"), 0, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_241"), 0, STRING("mario"))
        CALL(evt_msg_print, 0, STRING("peach_aji_242"), 0, 0)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        SET_VALUE(GSW_STORY_PROGRESS, STG5_45_PEACH_POTION_PUZZLE)
    END_IF
    DELETE_EVENT(GW(2))
    SET_VALUE(LSWF_AJI_06_DRINK_PENDING, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Colour animation of the finished potion for a successful mix:
 * fades the fluid colour back and forth between green and blue-green forever.
 */
EVT_BEGIN(kusuri_color_seiko)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 255)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 64)
    DIVIDE_FLOAT(LW(3), FLOAT(100.0f))
    LOOP(0)
        LOOP(100)
            SUBTRACT_FLOAT(LW(1), LW(3))
            ADD_FLOAT(LW(2), LW(3))
            CALL(kusuri_color_set_rgb, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(100)
            ADD_FLOAT(LW(1), LW(3))
            SUBTRACT_FLOAT(LW(2), LW(3))
            CALL(kusuri_color_set_rgb, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/**
 * Colour animation of the finished potion for a failed mix:
 * fades the fluid colour back and forth between a dimmed version (70 percent) of its current colour
 * and a 30 percent version, forever.
 */
EVT_BEGIN(kusuri_color_sippai)
    CALL(kusuri_color_get, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    SET_VALUE(LW(5), LW(2))
    MULTIPLY_FLOAT(LW(0), FLOAT(0.69921875f))
    MULTIPLY_FLOAT(LW(1), FLOAT(0.69921875f))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.69921875f))
    MULTIPLY_FLOAT(LW(3), FLOAT(0.2998047f))
    MULTIPLY_FLOAT(LW(4), FLOAT(0.2998047f))
    MULTIPLY_FLOAT(LW(5), FLOAT(0.2998047f))
    SUBTRACT_FLOAT(LW(3), LW(0))
    SUBTRACT_FLOAT(LW(4), LW(1))
    SUBTRACT_FLOAT(LW(5), LW(2))
    DIVIDE_FLOAT(LW(3), FLOAT(100.0f))
    DIVIDE_FLOAT(LW(4), FLOAT(100.0f))
    DIVIDE_FLOAT(LW(5), FLOAT(100.0f))
    LOOP(0)
        LOOP(100)
            ADD_FLOAT(LW(0), LW(3))
            ADD_FLOAT(LW(1), LW(4))
            ADD_FLOAT(LW(2), LW(5))
            CALL(kusuri_color_set_rgb, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(100)
            SUBTRACT_FLOAT(LW(0), LW(3))
            SUBTRACT_FLOAT(LW(1), LW(4))
            SUBTRACT_FLOAT(LW(2), LW(5))
            CALL(kusuri_color_set_rgb, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/**
 * Route of the beaker along the machine: eight stops, each with a target x, travel time, slot and direction.
 */
s32 peach_evt_bika_move_data[] = {
    0, 77, 2310, 0, 0, 141, 1920, 1,
    0, 203, 1860, 2, 0, 141, 1860, 1,
    1, 203, 1860, 2, 0, 77, 3780, 0,
    1, 203, 3780, 2, 0, 266, 1890, 3,
    0,
};

/**
 * Part of the machine's run (peach_evt_tyogo_sub), after the potions are in:
 * TEC asks Peach to press the button where the beaker stops (peach_aji_230), then the beaker (S_bika) makes
 * eight stops from peach_evt_bika_move_data (target x, time, slot GW(12), direction), with the floor animation
 * "yuka" run forward or backward to match; each stop waits up to 3 seconds for a pour, scored by the slot
 * scripts in GW(13).
 * Then TEC asks for the heating to be stopped after 30 seconds (peach_aji_230_01): stopping it with A_sui6
 * between 25 and 35 seconds adds 1 more.
 * A score of 9 (all eight pours right and good heating) plays kusuri_color_seiko; anything less
 * kusuri_color_sippai.
 */
EVT_BEGIN(peach_evt_tyogo_sub)
    CALL(evt_mario_key_onoff, 0)
    // The machine stops its floor and TEC tells Peach to press the button where the beaker stops
    // (peach_aji_230).
    CALL(evt_map_pauseanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxoff, GW(3))
    CALL_CHILD(PTR(peach_evt_wait_memo))
    CALL(evt_msg_print, 0, STRING("peach_aji_230"), 0, 0)
    SET_VALUE(LW(0), STRING("S_pato2"))
    SPAWN_EVENT_GET_ID(PTR(peach_evt_pato_on), GW(0))
    SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
    SET_VALUE(GW(11), 0)
    SET_VALUE(GW(12), 0)
    SET_VALUE(GW(13), 0)
    SET_VALUE(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 0)
    SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 0)
    // The liquid colour is reset and Peach gets control back for the pouring round.
    CALL(kusuri_color_init)
    CALL(evt_mario_key_onoff, 1)
    // The beaker visits each stop in turn; Peach has up to 3 seconds to press the button to pour the potion
    // for that slot.
    SET_INT_READ_ADDR(PTR(peach_evt_bika_move_data))
    READ_INT(LW(2))
    LOOP(0)
        READ_INT4(LW(3), LW(4), GW(12), LW(5))
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(6), LW(4))
            SUBTRACT(LW(6), 400)
            WAIT_MS(LW(6))
            SET_VALUE(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
        END_INLINE_EVENT
        IF_INT_EQUAL(LW(5), 0)
            CALL(evt_map_replayanim, 0, STRING("yuka"))
            ELSE
            BEGIN_INLINE_EVENT
                CALL(yuka_anim_reverse_play)
            END_INLINE_EVENT
        END_IF
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_MOVE1"), 0, 0, -100, GW(3))
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(2), LW(3), LW(4))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_bika"), LW(0), 48, -152)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        IF_INT_EQUAL(LW(5), 0)
            CALL(evt_map_pauseanim, 0, STRING("yuka"))
            ELSE
            CALL(yuka_anim_reverse_end)
            WAIT_FRAMES(1)
        END_IF
        CALL(evt_snd_sfxoff, GW(3))
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 3000)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_PRESSED, 1)
                LOOP(0)
                    IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_PRESSED, 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 0)
        ADD(GW(11), 1)
        IF_INT_EQUAL(GW(11), 8)
            SET_VALUE(GW(12), 4)
            BREAK_LOOP
        END_IF
        SET_VALUE(LW(2), LW(3))
    END_LOOP
    CALL(evt_map_replayanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_MOVE1"), 0, 0, -100, GW(3))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 266, 345, 2370)
    LABEL(10)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), LW(0), 48, -152)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_map_pauseanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxoff, GW(3))
    DELETE_EVENT(GW(0))
    SET_VALUE(LW(0), STRING("S_pato2"))
    CALL_CHILD(PTR(peach_evt_pato_off))
    CALL_CHILD(PTR(peach_evt_wait_memo))
    // TEC asks Peach to stop the heating after exactly 30 seconds; the stopwatch runs.
    CALL(evt_msg_print, 0, STRING("peach_aji_230_01"), 0, 0)
    SET_VALUE(LW(0), STRING("S_pato3"))
    SPAWN_EVENT_GET_ID(PTR(peach_evt_pato_on), GW(0))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui6"), 0, PTR(peach_evt_stop_renji), 0)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_HOT1"), 104, 0, -100, LW(3))
    CALL(evt_map_playanim, STRING("ren1"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("ren1"), LW(1), LW(2))
    WAIT_MS(LW(2))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_map_playanim, STRING("ren2"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 1)
    SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 0)
    LOOP(0)
        IF_INT_EQUAL(LSWF_AJI_06_DISPENSE_PRESSED, 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_sub_get_stopwatch, LW(0), LW(1))
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_sfxoff, LW(3))
    CALL(evt_map_playanim, STRING("ren2"), MAP_ANIM_PLAY_AT_END, 0)
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("ren3"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("ren3"), LW(2), LW(3))
    WAIT_MS(LW(3))
    DELETE_EVENT(GW(0))
    SET_VALUE(LW(0), STRING("S_pato3"))
    CALL_CHILD(PTR(peach_evt_pato_off))
    // Both endpoints are excluded; the stopwatch also includes the opening animation.
    IF_INT_GREATER(LW(1), 25000)
        IF_INT_LESS(LW(1), 35000)
            DEBUG_PUT_MSG(STRING("ほどよい　かねつじかん"))
            ADD(GW(13), 1)
            ELSE
            DEBUG_PUT_MSG(STRING("かねつしすぎ"))
            DIVIDE(LW(1), 1000)
            DEBUG_PUT_REG(LW(1))
        END_IF
        ELSE
        DEBUG_PUT_MSG(STRING("かねつぶそく"))
        DIVIDE(LW(1), 1000)
        DEBUG_PUT_REG(LW(1))
    END_IF
    // A perfect score turns the beaker the right colour; anything less turns it the wrong colour.
    IF_INT_GREATER_OR_EQUAL(GW(13), 9)
        SPAWN_EVENT_GET_ID(PTR(kusuri_color_seiko), GW(2))
        ELSE
        SPAWN_EVENT_GET_ID(PTR(kusuri_color_sippai), GW(2))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_map_replayanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_MOVE1"), 0, 0, -100, GW(3))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 345, 439, 2820)
    LABEL(11)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), LW(0), 48, -152)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(11)
    END_IF
    SET_VALUE(LSWF_AJI_06_DISPENSE_WINDOW_OPEN, 0)
    SET_VALUE(LSWF_AJI_06_DISPENSE_PRESSED, 0)
    RETURN
EVT_END()

/**
 * Starts the machine (case script on the start switch A_sui1):
 * closes the hatches, stops the sirens, runs the needle (peach_evt_hari_on) and the arm,
 * then calls peach_evt_tyogo_sub.
 * Afterwards asks whether to drink the result (peach_aji_231): yes attaches peach_evt_kusuri_nomu to A_bika1;
 * no empties the machine and opens the hatches for another try.
 */
EVT_BEGIN(peach_evt_tyogo)
    // The machine is started: the hatches close and the sirens stop.
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_ACCESS1"), LW(0), LW(1), LW(2), 0)
    CALL_CHILD(PTR(peach_evt_kaihei_close_all))
    DELETE_EVENT(GW(0))
    SET_VALUE(LW(0), STRING("S_pato1"))
    CALL_CHILD(PTR(peach_evt_pato_off))
    SET_VALUE(LSWF_AJI_06_MIXING_RUNNING, 1)
    CALL(evt_mapobj_get_position, STRING("S_sui_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_SWITCH1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 800)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 1, STRING("S_bika"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_kururi"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 0, 0, 0)
    // The needle and the arm start to run.
    SPAWN_EVENT_GET_ID(PTR(peach_evt_hari_on), GW(1))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_map_replayanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_MOVE1"), 0, 0, -100, GW(3))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mapobj_get_position, STRING("S_syata1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_OPEN1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -30, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 0, STRING("S_syata1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_snd_sfxoff, LW(3))
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, STRING("S_syata1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MACHINE_OPEN1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -30, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 0, STRING("S_syata1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, -42, 1260)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 48, 1440)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 0, LW(0), -42)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -42, -152, 3300)
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 0, 48, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    // The pouring and heating round (peach_evt_tyogo_sub) runs.
    CALL_CHILD(PTR(peach_evt_tyogo_sub))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -152, -42, 3300)
    LABEL(12)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 439, 48, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(12)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 48, 0, 1440)
    LABEL(13)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 439, LW(0), -42)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(13)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("S_syata2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("S_syata2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -30, 500)
        LABEL(14)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 0, STRING("S_syata2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(14)
        END_IF
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, STRING("S_syata2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("S_syata2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -30, 0, 500)
        LABEL(15)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 0, STRING("S_syata2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(15)
        END_IF
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -42, 0, 1260)
    LABEL(16)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 439, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(16)
    END_IF
    CALL(evt_map_pauseanim, 0, STRING("yuka"))
    CALL(evt_snd_sfxoff, GW(3))
    DELETE_EVENT(GW(1))
    SPAWN_EVENT(PTR(peach_evt_hari_off))
    SET_VALUE(LSWF_AJI_06_MIXING_RUNNING, 0)
    IF_INT_EQUAL(GW(13), 9)
        SET_VALUE(LSWF_AJI_06_MIX_CORRECT, 1)
        ELSE
        SET_VALUE(LSWF_AJI_06_MIX_CORRECT, 0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(peach_evt_wait_memo))
    // TEC asks whether the result came out right; if so Peach is asked to drink it.
    CALL(evt_msg_print, 0, STRING("peach_aji_231"), 0, 0)
    CALL(evt_msg_select, 0, STRING("peach_aji_227_sel"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("peach_aji_231_02"))
        CALL(evt_msg_print, 0, STRING("peach_aji_232"), 0, STRING("mario"))
        CALL(evt_msg_print, 0, STRING("peach_aji_233"), 0, 0)
        SET_VALUE(LSWF_AJI_06_DRINK_PENDING, 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_bika1"), 0, PTR(peach_evt_kusuri_nomu), 0)
        CALL(evt_del_case_evt, 0, GW(6))
        CALL(evt_del_case_evt, 0, GW(7))
        CALL(evt_del_case_evt, 0, GW(8))
        CALL(evt_del_case_evt, 0, GW(9))
        ELSE
        // If not, the machine is emptied and reopened for another try.
        CALL(evt_msg_print_add, 0, STRING("peach_aji_231_01"))
        CALL(evt_mapobj_scale, 0, STRING("S_naka"), 0, 0, 0)
        CALL(evt_mapobj_trans, 0, STRING("S_bika"), 0, -100, 0)
        SET_VALUE(LSWF_AJI_06_DRINK_PENDING, 0)
        DELETE_EVENT(GW(2))
        CALL_CHILD(PTR(peach_evt_kaihei_open_all))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(peach_evt_tyogo_entry), GW(5))
        SET_VALUE(LW(0), STRING("S_pato1"))
        SPAWN_EVENT_GET_ID(PTR(peach_evt_pato_on), GW(0))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Case script entry for the start switch: spawns peach_evt_tyogo and leaves the case
 * so the switch can be used again later.
 */
EVT_BEGIN(peach_evt_tyogo_entry)
    SPAWN_EVENT(PTR(peach_evt_tyogo))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Holds up the red potion item (kusuri_aka) when Peach takes it and waits for the get-item display to end. */
EVT_BEGIN(peach_evt_kusuri_aka)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_get_item, STRING("kusuri_aka"))
    CALL(evt_item_get_item_end_wait, STRING("kusuri_aka"))
    SET_VALUE(LSWF_AJI_06_RED_POTION_HELD, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Holds up the blue potion item (kusuri_ao) when Peach takes it and waits for the get-item display to end. */
EVT_BEGIN(peach_evt_kusuri_ao)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_get_item, STRING("kusuri_ao"))
    CALL(evt_item_get_item_end_wait, STRING("kusuri_ao"))
    SET_VALUE(LSWF_AJI_06_BLUE_POTION_HELD, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Holds up the orange potion item (kusuri_ki) when Peach takes it and waits for the get-item display to end. */
EVT_BEGIN(peach_evt_kusuri_ki)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_get_item, STRING("kusuri_ki"))
    CALL(evt_item_get_item_end_wait, STRING("kusuri_ki"))
    SET_VALUE(LSWF_AJI_06_ORANGE_POTION_HELD, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Holds up the green potion item (kusuri_mid) when Peach takes it and waits for the get-item display to end. */
EVT_BEGIN(peach_evt_kusuri_mid)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_get_item, STRING("kusuri_mid"))
    CALL(evt_item_get_item_end_wait, STRING("kusuri_mid"))
    SET_VALUE(LSWF_AJI_06_GREEN_POTION_HELD, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Waits until no memo text is being shown (LSWF(11) = 0), so that machine messages do not collide with a memo. */
EVT_BEGIN(peach_evt_wait_memo)
    LOOP(0)
        IF_INT_EQUAL(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Peach reads recipe memo 1 (peach_aji_225_1); LSWF(11) is set while the text is up. */
EVT_BEGIN(peach_evt_memo1)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_225_1"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
    RETURN
EVT_END()

/** Peach reads recipe memo 2 (peach_aji_225_2). */
EVT_BEGIN(peach_evt_memo2)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_225_2"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
    RETURN
EVT_END()

/** Peach reads recipe memo 3 (peach_aji_225_3). */
EVT_BEGIN(peach_evt_memo3)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_225_3"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
    RETURN
EVT_END()

/** Peach reads recipe memo 4 (peach_aji_225_4). */
EVT_BEGIN(peach_evt_memo4)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_225_4"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("S_dami1"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 3)
    CALL(evt_item_entry, STRING("kusuri_aka"), ITEM_RED_POTION, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mapobj_get_position, STRING("S_dami2"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 3)
    CALL(evt_item_entry, STRING("kusuri_ao"), ITEM_BLUE_POTION, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mapobj_get_position, STRING("S_dami3"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 3)
    CALL(evt_item_entry, STRING("kusuri_ki"), ITEM_ORANGE_POTION, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mapobj_get_position, STRING("S_dami4"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 3)
    CALL(evt_item_entry, STRING("kusuri_mid"), ITEM_GREEN_POTION, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusuri1"), 0, PTR(peach_evt_kusuri_aka), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusuri2"), 0, PTR(peach_evt_kusuri_ao), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusuri3"), 0, PTR(peach_evt_kusuri_ki), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusuri4"), 0, PTR(peach_evt_kusuri_mid), 0)
    CALL(evt_map_playanim, STRING("yuka"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_pauseanim, 0, STRING("yuka"))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(peach_evt_kaihei1), GW(6))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(peach_evt_kaihei2), GW(7))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui4"), 0, PTR(peach_evt_kaihei3), GW(8))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui5"), 0, PTR(peach_evt_kaihei4), GW(9))
    CALL(evt_map_set_tevcallback, 1, PTR(kusuri_naka_color_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_naka"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("S_naka"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_map_set_tevcallback, 2, PTR(kusuri_1_color_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_mizu1"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("S_mizu1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_map_set_tevcallback, 3, PTR(kusuri_2_color_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_mizu2"), MAP_OBJ_TEV_CALLBACK(3))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("S_mizu2"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_map_set_tevcallback, 4, PTR(kusuri_3_color_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_mizu3"), MAP_OBJ_TEV_CALLBACK(4))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("S_mizu3"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_map_set_tevcallback, 5, PTR(kusuri_4_color_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_mizu4"), MAP_OBJ_TEV_CALLBACK(5))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("S_mizu4"), MAP_OBJ_FLAG_DISPLAY_LIST)
    SET_VALUE(LSWF_AJI_06_PEACH_MSG_OPEN, 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo1"), 0, PTR(peach_evt_memo1), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo2"), 0, PTR(peach_evt_memo2), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo3"), 0, PTR(peach_evt_memo3), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo4"), 0, PTR(peach_evt_memo4), 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_adjust_dir)
    SET_VALUE(LW(0), STRING("s_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("s_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("s_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_msg_print, 0, STRING("peach_aji_221"), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_222"), 0, STRING("mario"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_dispdir, LW(0))
        IF_INT_EQUAL(LW(0), 180)
            CALL(evt_mario_set_dir, 270, 200, 0)
            WAIT_MS(300)
            CALL(evt_mario_set_dir, 90, 200, 0)
            WAIT_MS(300)
            ELSE
            CALL(evt_mario_set_dir, 90, 200, 0)
            WAIT_MS(300)
            CALL(evt_mario_set_dir, 270, 200, 0)
            WAIT_MS(300)
        END_IF
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("peach_aji_223"), 0, 0)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("peach_aji_224"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_225"), 0, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_COOKING1"))
    CALL_CHILD(PTR(peach_evt_kaihei_open_all))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Case script of the three wrong potions at the end of the puzzle: only shows peach_aji_256 (the wrong one). */
EVT_BEGIN(peach_evt_matigai_kusuri)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_256"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_tadashii_kusuri)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_del_case_evt, 0, LW(0))
    CALL(evt_del_case_evt, 0, LW(1))
    CALL(evt_del_case_evt, 0, LW(2))
    CALL(peach_chg_pose, STRING("P_M_5"))
    WAIT_MS(500)
    CALL(evt_item_delete, STRING("kusuri_mid"))
    WAIT_MS(200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DRINK_MEDICINE1"), LW(0), LW(1), LW(2), 0)
    CALL(peach_chg_pose, STRING("P_M_7"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_TOUMEI2"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 10)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(200)
    CALL(peach_chg_pose, STRING("P_M_8"))
    WAIT_MS(500)
    CALL(evt_peach_set_condition, 0)
    CALL(evt_mario_get_dir, LW(0))
    IF_INT_EQUAL(LW(0), 90)
        CALL(evt_mario_set_dir, 270, 200, 0)
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(300)
        ELSE
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 270, 200, 0)
        WAIT_MS(300)
    END_IF
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_msg_print, 0, STRING("peach_aji_257"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_258"), 0, 0)
    SET_VALUE(GSWF_AJI_06_RIGHT_POTION_CHOSEN, 1)
    CALL_CHILD(PTR(peach_evt_kaihei_close_all))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_door)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_AJI_06_RIGHT_POTION_CHOSEN, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_255_01"), 0, 0)
        ELSE
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_open_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL_CHILD(PTR(s_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 0, 300, FLOAT(150.0f))
        END_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_fade_entry, FADE_OUT_BLACK, 300, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_BLACK, 0, FADE_IN_BLACK, 300)
        CALL(evt_bero_mapchange, STRING("aji_17"), 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Chapter 5 setup of the potion lab on re-entry:
 * same potion shelves as peach_evt_stg5 at fixed positions,
 * with the wrong-potion scripts (peach_evt_matigai_kusuri) on three of the four machine slots
 * and the right-potion script (peach_evt_tadashii_kusuri) on the fourth.
 * It then switches off the s_bero entrance, attaches peach_evt_door to it, shows peach_aji_251 to 254,
 * walks Peach to the changing spot, plays the cloth-change sound,
 * removes her invisibility (peach_evt_toumei_onoff 0) and shows peach_aji_255.
 */
static EVT_BEGIN(peach_evt_stg5_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_item_entry, STRING("kusuri_aka"), ITEM_RED_POTION, -110, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_entry, STRING("kusuri_ao"), ITEM_BLUE_POTION, -65, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_entry, STRING("kusuri_ki"), ITEM_ORANGE_POTION, -10, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_entry, STRING("kusuri_mid"), ITEM_GREEN_POTION, 35, 31, -107, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(peach_evt_matigai_kusuri), LW(0))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(peach_evt_matigai_kusuri), LW(1))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui4"), 0, PTR(peach_evt_matigai_kusuri), LW(2))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui5"), 0, PTR(peach_evt_tadashii_kusuri), LW(3))
    CALL(evt_set_case_wrk, LW(3), 0, LW(0))
    CALL(evt_set_case_wrk, LW(3), 1, LW(1))
    CALL(evt_set_case_wrk, LW(3), 2, LW(2))
    CALL(evt_set_case_wrk, LW(3), 3, LW(3))
    CALL(evt_mapobj_trans, 0, STRING("S_kaihei1"), 0, -25, 0)
    CALL(evt_mapobj_trans, 0, STRING("S_kaihei2"), 0, -25, 0)
    CALL(evt_mapobj_trans, 0, STRING("S_kaihei3"), 0, -25, 0)
    CALL(evt_mapobj_trans, 0, STRING("S_kaihei4"), 0, -25, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    SET_VALUE(LW(0), STRING("s_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("s_bero"), 0, PTR(peach_evt_door), 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_251"), 0, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_3"), STRING("P_T_3"))
    CALL(evt_msg_print, 0, STRING("peach_aji_252"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_253"), 0, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_254"), 0, STRING("mario"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_mov_pos2, -170, 125, FLOAT(72.0f))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2000)
    CALL(peach_evt_toumei_onoff, 0)
    CALL(evt_mario_mov_pos2, 0, 90, FLOAT(72.0f))
    CALL(evt_msg_print, 0, STRING("peach_aji_255"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Initial state of the potion machine: hides the sirens, empties the tubes and the mixing tank,
 * turns the arm and needle to their rest pose and sets the lamp objects hika1 to 4 hidden and kesu1 to 4 shown.
 */
EVT_BEGIN(kikai_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_pato1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_pato2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("S_pato3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_scale, 0, STRING("S_mizu1"), 1, 0, 1)
    CALL(evt_mapobj_scale, 0, STRING("S_mizu2"), 1, 0, 1)
    CALL(evt_mapobj_scale, 0, STRING("S_mizu3"), 1, 0, 1)
    CALL(evt_mapobj_scale, 0, STRING("S_mizu4"), 1, 0, 1)
    CALL(evt_mapobj_scale, 0, STRING("S_naka"), 0, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_bika"), 0, 0, 180)
    CALL(evt_mapobj_trans, 1, STRING("S_bika"), 0, 2, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_kururi"), 0, 0, 180)
    CALL(evt_mapobj_rotate, 0, STRING("hari"), 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hika1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kesu1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hika2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kesu2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hika3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kesu3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hika4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kesu4"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/** Reading a note in the lab: shows stg7_aji_32_01 with controls frozen. */
static EVT_BEGIN(evt_memo1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_32_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Reading a second note in the lab: shows stg7_aji_32_02 with controls frozen. */
static EVT_BEGIN(evt_memo2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_32_02"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Case script of the wall note A_memo3 (Mario's visit): reads stg7_aji_32_03. */
EVT_BEGIN(evt_memo3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_32_03"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Case script of the wall note A_memo4 (Mario's visit): reads stg7_aji_32_04. */
EVT_BEGIN(evt_memo4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_32_04"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init script of aji_06, the potion laboratory:
 * music BGM_EVT_PEACH1 before STG7_00_START (Peach's visits) and BGM_STG7_DUN2 after, the exit to aji_01,
 * Mario dropping in from the ceiling when entered as "tenjo", the machine set- up (kikai_init),
 * Peach's scenes of STG5_44, 45 and 46 (peach_evt_stg5, the invisible state and peach_evt_stg5_1)
 * and the wall notes. The magician grunt 軍団ザコ魔法使い is removed once Lord Crump is beaten.
 */
EVT_BEGIN(aji_06_init_evt)
    // Peach's visits get her own music; later visits use the fortress music and ambience.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_00_START)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI2"))
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_bero_get_entername, LW(0))
    // Entering from the ceiling (tenjo): Mario and his partner drop into the lab and land.
    IF_STR_EQUAL(LW(0), STRING("tenjo"))
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_party_cont_onoff, 1, 0)
        CALL(evt_mario_set_pos, 175, 165, -110)
        CALL(evt_party_set_pos, 0, 185, 165, -100)
        CALL(evt_party_set_pos, 1, 195, 165, -90)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_mario_cont_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_party_cont_onoff, 1, 1)
            CALL(evt_mario_wait_landon)
#if VERSION == VERSION_EU
            LOOP(30)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
                ADD(LW(1), 20)
                IF_INT_LESS(LW(1), LW(4))
                    SUBTRACT(LW(4), 10)
                    CALL(evt_party_set_pos, 0, LW(3), LW(4), LW(5))
                ELSE
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
#endif
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_cam_letter_box_disable, 0)
        END_INLINE_EVENT
    END_IF
    // The alarm and the potion machine start up.
    SPAWN_EVENT(PTR(evt_emergency))
    SPAWN_EVENT(PTR(kikai_init))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_npc_check_delete, STRING("軍団ザコ魔法使い"))
    END_IF
    // Chapter 5: Peach takes over, her partner is dismissed and TEC's request scene starts.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_44_PEACH_TECS_REQUEST_2)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_npc_check_delete, STRING("軍団ザコ魔法使い"))
        SPAWN_EVENT(PTR(peach_evt_stg5))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_45_PEACH_POTION_PUZZLE)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_mario_kill_party, 0)
        CALL(peach_evt_toumei_onoff, 1)
        CALL(evt_npc_check_delete, STRING("軍団ザコ魔法使い"))
    END_IF
    // Disk search: Peach is still invisible and the next scene starts.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_46_PEACH_DISK_SEARCH)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_mario_kill_party, 0)
        CALL(peach_evt_toumei_onoff, 1)
        CALL(evt_npc_check_delete, STRING("軍団ザコ魔法使い"))
        SPAWN_EVENT(PTR(peach_evt_stg5_1))
    END_IF
    // Peach can read the notes on the walls; the machine's lamps start switched off.
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo1"), 0, PTR(evt_memo1), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo2"), 0, PTR(evt_memo2), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo3"), 0, PTR(evt_memo3), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_memo4"), 0, PTR(evt_memo4), 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_1_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_2_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_3_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_4_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_5_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_6_2"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

USER_FUNC(yuka_anim_reverse_end) {
    yuka_anim.flags |= 2;
    return EVT_RETURN_DONE;
}

/** Script function: pauses the reverse playback of the "yuka" animation. */
USER_FUNC(yuka_anim_reverse_pause) {
    yuka_anim.flags |= 1;
    return EVT_RETURN_DONE;
}

/** Script function: resumes the reverse playback of the "yuka" animation after a pause. */
USER_FUNC(yuka_anim_reverse_replay) {
    yuka_anim.flags &= ~1;
    return EVT_RETURN_DONE;
}

/**
 * Script function: plays the map animation "yuka" backwards (peach_evt_tyogo_sub runs it while the beaker moves
 * back toward a lower slot).
 * Each frame it subtracts the elapsed time times the animation speed from the animation frame
 * and wraps around at the start. Blocks until yuka_anim_reverse_end.
 */
USER_FUNC(yuka_anim_reverse_play) {
    f32 frame;

    if (isFirstCall) {
        yuka_anim.anim = (YukaAnimData*)mapSearchAnmObj("yuka");
        yuka_anim.flags = 0;
        yuka_anim.anim->time = YUKA_NOW();
        yuka_anim.anim->flags |= 0x10;
        yuka_anim.anim->flags &= ~0x20;
    }
    if (yuka_anim.flags & 2) {
        return EVT_RETURN_DONE;
    }
    if (!(yuka_anim.flags & 1)) {
        {
            f32 dt = 60.0f * (f32)(u64)(YUKA_NOW() - yuka_anim.anim->time) / 1000.0f;

            yuka_anim.anim->frame -= dt * yuka_anim.anim->speed;
        }
        if (yuka_anim.anim->frame <= 0.0f) {
            for (frame = yuka_anim.anim->frame; frame < 0.0f; frame += yuka_anim.anim->data->length) {
            }
            yuka_anim.anim->frame = frame;
        }
    }
    // Refresh the time baseline even while reverse playback is paused.
    yuka_anim.anim->time = YUKA_NOW();
    return EVT_RETURN_BLOCK;
}

/** Script function: sets Mario's (Peach's) pose to the animation named in args[0]. */
USER_FUNC(peach_chg_pose) {
    marioChgPose((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/** Script function: args[0] = 1 makes Peach invisible by setting Mario's scale to 0 on all axes; 0 restores scale 1. */
USER_FUNC(peach_evt_toumei_onoff) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();

    if (evtGetValue(event, args[0])) {
        mario->scale.x = 0.0f;
        mario->scale.y = 0.0f;
        mario->scale.z = 0.0f;
    } else {
        mario->scale.x = 1.0f;
        mario->scale.y = 1.0f;
        mario->scale.z = 1.0f;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function: sets Peach's size to the float in args[0] (Mario's scale on x, y and z),
 * scaling her collision box (20 by 47 units at scale 1) with it.
 */
USER_FUNC(peach_set_scale) {
    f32 scale = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();

    mario->scale.x = scale;
    mario->scale.y = scale;
    mario->scale.z = scale;
    mario->wPlayerCollisionBox.x = 20.0f * scale;
    mario->wPlayerCollisionBox.y = 47.0f * scale;
    return EVT_RETURN_DONE;
}

/**
 * Script function: checks whether the potion number in args[1] equals step args[0] of kusuri_tyogo_list (1, 2,
 * 3, 2, 3, 1, 3, 4); returns 1 or 0 in args[2].
 */
USER_FUNC(peach_evt_seikai_check) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 v = evtGetValue(event, args[1]);

    if (v == kusuri_tyogo_list[idx]) {
        evtSetValue(event, args[2], 1);
    } else {
        evtSetValue(event, args[2], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function: converts an item ID in args[0] (red 102, blue 101, orange 103, green 104,
 * the table kusuri_id) into its potion number 1 to 4 and returns it in args[1].
 */
USER_FUNC(kusuri_list_get_no) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 i;

    for (i = 0; id != kusuri_id[i]; i++) {
    }
    evtSetValue(event, args[1], i + 1);
    return EVT_RETURN_DONE;
}

/** Script function: appends the item ID of potion number args[0] (1 to 4) to the potion menu list kusuri_list. */
USER_FUNC(kusuri_list_add) {
    s32 no = evtGetValue(event, event->args[0]);
    s32 i;

    for (i = 0; kusuri_list[i] != -1; i++) {
    }
    kusuri_list[i] = kusuri_id[no - 1];
    return EVT_RETURN_DONE;
}

/** Script function: clears the potion menu list (kusuri_list) to empty (-1 entries). */
USER_FUNC(kusuri_list_init) {
    s32 i;

    for (i = 0; i < 5; i++) {
        kusuri_list[i] = -1;
    }
    return EVT_RETURN_DONE;
}

void kusuri_4_color_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, g_Kusuri_Src_Col[g_Kusuri4_No]);
    info->tevStage++;
}

/** Texture-environment callback for the third potion tube: the same as kusuri_4_color_tev with its own colour. */
void kusuri_3_color_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, g_Kusuri_Src_Col[g_Kusuri3_No]);
    info->tevStage++;
}

/** Texture-environment callback for the second potion tube: the same as kusuri_4_color_tev with its own colour. */
void kusuri_2_color_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, g_Kusuri_Src_Col[g_Kusuri2_No]);
    info->tevStage++;
}

/** Texture-environment callback for the first potion tube: the same as kusuri_4_color_tev with its own colour. */
void kusuri_1_color_tev(MapTevCallbackInfo* info) {
    GXTevStageID stage = info->tevStage;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, g_Kusuri_Src_Col[g_Kusuri1_No]);
    info->tevStage++;
}

void kusuri_naka_color_tev(MapTevCallbackInfo* info) {
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, g_Kusuri_Col);
    info->tevStage = 1;
    info->texCoord = 0;
    info->texMap = 0;
}

USER_FUNC(kusuri_color_get) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], g_Kusuri_Col.r);
    evtSetValue(event, args[1], g_Kusuri_Col.g);
    evtSetValue(event, args[2], g_Kusuri_Col.b);
    return EVT_RETURN_DONE;
}

USER_FUNC(kusuri_color_set_rgb) {
    EvtWord* args = event->args;
    s32 r = evtGetValue(event, args[0]);
    s32 g = evtGetValue(event, args[1]);
    s32 b = evtGetValue(event, args[2]);

    g_Kusuri_Col.r = r;
    g_Kusuri_Col.g = g;
    g_Kusuri_Col.b = b;
    return EVT_RETURN_DONE;
}

/**
 * Script function: blends potion colour number args[0] into the tank colour over args[1] ms:
 * adds the potion's source colour to what was already mixed,
 * scales the result so the largest channel is 255 and stores the sum when finished. Blocks until the time is up.
 */
USER_FUNC(kusuri_color_set) {
    EvtWord* args = event->args;
    s32 no = evtGetValue(event, args[0]);
    s32 frames = evtGetValue(event, args[1]);
    OSTime now = gp->renderTime;
    s32 elapsed;
    BOOL done;
    f32 t;
    s32 r, g, b;

    if (isFirstCall) {
        event->lwData[8] = frames;
        event->lwData[9] = (g_Kusuri_Col.r << 16) | (g_Kusuri_Col.g << 8) | g_Kusuri_Col.b;
        event->userdata[0] = ((u64)now >> 32) & 0xFFFFFFFF;
        event->userdata[1] = now & 0xFFFFFFFF;
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(now, event->userdata[1]));
    if (elapsed < event->lwData[8]) {
        done = FALSE;
        t = (f32)elapsed / (f32)event->lwData[8];
    } else {
        done = TRUE;
        t = 1.0f;
    }
    r = t * g_Kusuri_Src_Col[no - 1].r + g_Kusuri_R;
    g = t * g_Kusuri_Src_Col[no - 1].g + g_Kusuri_G;
    b = t * g_Kusuri_Src_Col[no - 1].b + g_Kusuri_B;
    if (r > g) {
        if (r > b) {
            g_Kusuri_Col.r = 255;
            g_Kusuri_Col.g = 255.0f * ((f32)g / (f32)r);
            g_Kusuri_Col.b = 255.0f * ((f32)b / (f32)r);
        } else {
            g_Kusuri_Col.r = 255.0f * ((f32)r / (f32)b);
            g_Kusuri_Col.g = 255.0f * ((f32)g / (f32)b);
            g_Kusuri_Col.b = 255;
        }
    } else if (g > b) {
        g_Kusuri_Col.r = 255.0f * ((f32)r / (f32)g);
        g_Kusuri_Col.g = 255;
        g_Kusuri_Col.b = 255.0f * ((f32)b / (f32)g);
    } else {
        g_Kusuri_Col.r = 255.0f * ((f32)r / (f32)b);
        g_Kusuri_Col.g = 255.0f * ((f32)g / (f32)b);
        g_Kusuri_Col.b = 255;
    }
    if (done) {
        g_Kusuri_R = r;
        g_Kusuri_G = g;
        g_Kusuri_B = b;
    }
    if (done) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script function: resets the tank colour and the accumulated mix to black (0, 0, 0). */
USER_FUNC(kusuri_color_init) {
    g_Kusuri_Col.r = 0;
    g_Kusuri_Col.g = 0;
    g_Kusuri_Col.b = 0;
    g_Kusuri_R = 0;
    g_Kusuri_G = 0;
    g_Kusuri_B = 0;
    return EVT_RETURN_DONE;
}
