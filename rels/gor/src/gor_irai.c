#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_item.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_case.h"
#include "evt/evt_door.h"
#include "driver/casedrv.h"

#include "driver/camdrv.h"
#include "driver/msgdrv.h"
#include "driver/swdrv.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "dolphin/gx.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include "string.h"
#include "mario/mario_pouch.h"
#include "evt/evt_cmd.h"
#include "gor.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>

typedef struct IraiWork {
    s32 state;
    s32 count;
    s32 list[30];
    s32 cursor;
    s32 last;
    s32 top;
    s32 yubiX;
    s32 yubiY;
    s32 field_0x94;
    f32 scroll;
    char client[30][21];
    char contents[30][31];
    u8 field_0x6B4[0x416C - 0x6B4];
} IraiWork;
void FontDrawString(const char* message, f32 x, f32 y);
void irai_window_disp(CameraId camera, void* param);
void irai_flag_disp(void);
void irai_flag_clear(void);

USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(irai_cancel_func);
USER_FUNC(irai_check_func);
USER_FUNC(irai_end_func);
USER_FUNC(irai_init_func);
USER_FUNC(irai_main_func);
USER_FUNC(irai_start_func);

const char* irai_msg_client[30];
const char* irai_msg_contents[30];
const char* irai_msg_exposition[30];
const char* irai_msg_kakunin[30];
const char* irai_msg_hikiukechu[30];
s32 irai_item_data01[2];
s32 irai_item_data02[2];
s32 irai_item_data13[2];
s32 irai_item_data16[2];
s32 irai_item_data17[2];
s32 irai_item_data19[2];
s32 irai_item_data23[2];
s32 irai_item_data25[2];
s32 irai_item_data29[2];
s32 irai_item_null[1];
s32* irai_item_table[30];
EvtWord irai_start_event_null[2];
EvtWord irai_start_event01[13];
EvtWord irai_start_event04[8];
EvtWord* irai_start_event[30];
EvtWord irai_end_event_null[2];
EvtWord irai_end_event01[13];
EvtWord irai_end_event04[8];
#ifdef VERSION_G8ME01
EvtWord irai_end_event19[15];
#endif
EvtWord* irai_end_event[30];
EvtWord gor_irai_init[18];
EvtWord gor_irai_lecture[40];
EvtWord gor_irai_main[VERSION_PICK(105, 126)];
EvtWord gor_irai_tenshu_main[VERSION_PICK(258, 261)];

const char* irai_msg_client[] = {
    "irai_client_01", "irai_client_02", "irai_client_03", "irai_client_04", "irai_client_05", "irai_client_06",
    "irai_client_07", "irai_client_08", "irai_client_09", "irai_client_10", "irai_client_11", "irai_client_12",
    "irai_client_13", "irai_client_14", "irai_client_15", "irai_client_16", "irai_client_17", "irai_client_18",
    "irai_client_19", "irai_client_20", "irai_client_21", "irai_client_22", "irai_client_23", "irai_client_24",
    "irai_client_25", "irai_client_26", "irai_client_27", "irai_client_28", "irai_client_29", "irai_client_30",
};

const char* irai_msg_contents[] = {
    "irai_contents_01", "irai_contents_02", "irai_contents_03", "irai_contents_04", "irai_contents_05",
    "irai_contents_06", "irai_contents_07", "irai_contents_08", "irai_contents_09", "irai_contents_10",
    "irai_contents_11", "irai_contents_12", "irai_contents_13", "irai_contents_14", "irai_contents_15",
    "irai_contents_16", "irai_contents_17", "irai_contents_18", "irai_contents_19", "irai_contents_20",
    "irai_contents_21", "irai_contents_22", "irai_contents_23", "irai_contents_24", "irai_contents_25",
    "irai_contents_26", "irai_contents_27", "irai_contents_28", "irai_contents_29", "irai_contents_30",
};

const char* irai_msg_exposition[] = {
    "irai_help_01",
    "irai_help_02",
    "irai_help_03",
    "irai_help_04",
    "irai_help_05",
    "irai_help_06",
    "irai_help_07",
    "irai_help_08",
    "irai_help_09",
    "irai_help_10",
    "irai_help_11",
    "irai_help_12",
    "irai_help_13",
    "irai_help_14",
    "irai_help_15",
    "irai_help_16",
    "irai_help_17",
    "irai_help_18",
    "irai_help_19",
    "irai_help_20",
    "irai_help_21",
    "irai_help_22",
    "irai_help_23",
    "irai_help_24",
    "irai_help_25",
    "irai_help_26",
    "irai_help_27",
    "irai_help_28",
    "irai_help_29",
    "irai_help_30",
};

const char* irai_msg_kakunin[] = {
    "irai_43", "irai_44", "irai_45", "irai_46", "irai_47", "irai_48", "irai_49", "irai_50", "irai_51", "irai_52",
    "irai_53", "irai_54", "irai_55", "irai_56", "irai_57", "irai_58", "irai_59", "irai_60", "irai_61", "irai_62",
    "irai_63", "irai_64", "irai_65", "irai_66", "irai_67", "irai_68", "irai_69", "irai_70", "irai_71", "irai_72",
};

const char* irai_msg_hikiukechu[] = {
    "irai_13",
    "irai_14",
    "irai_15",
    "irai_16",
    "irai_17",
    "irai_18",
    "irai_19",
    "irai_20",
    "irai_21",
    "irai_22",
    "irai_23",
    "irai_24",
    "irai_25",
    "irai_26",
    "irai_27",
    "irai_28",
    "irai_29",
    "irai_30",
    "irai_31",
    "irai_32",
    "irai_33",
    "irai_34",
    "irai_35",
    "irai_36",
    "irai_37",
    "irai_38",
    "irai_39",
    "irai_40",
    "irai_41",
    "irai_42",
};

s32 irai_item_data01[] = {
    ITEM_HOUSE_KEY, -1,
};

s32 irai_item_data02[] = {
    ITEM_BOX, -1,
};

s32 irai_item_data13[] = {
    ITEM_BATTLE_TRUNKS, -1,
};

s32 irai_item_data16[] = {
    ITEM_ATTACK_FX_B_KEY_ITEM, -1,
};

s32 irai_item_data17[] = {
    ITEM_ROUTING_SLIP, -1,
};

s32 irai_item_data19[] = {
    ITEM_WEDDING_RING, -1,
};

s32 irai_item_data23[] = {
    ITEM_WRESTLING_MAG, -1,
};

s32 irai_item_data25[] = {
    ITEM_PACKAGE, -1,
};

s32 irai_item_data29[] = {
    ITEM_PRESENT, -1,
};

s32 irai_item_null[] = {
    -1,
};

s32* irai_item_table[] = {
    irai_item_data01, irai_item_data02, irai_item_null, irai_item_null, irai_item_null, irai_item_null,
    irai_item_null, irai_item_null, irai_item_null, irai_item_null, irai_item_null, irai_item_null,
    irai_item_data13, irai_item_null, irai_item_null, irai_item_data16, irai_item_data17, irai_item_null,
    irai_item_data19, irai_item_null, irai_item_null, irai_item_null, irai_item_data23, irai_item_null,
    irai_item_data25, irai_item_null, irai_item_null, irai_item_null, irai_item_data29, irai_item_null,
};

EVT_BEGIN(irai_start_event_null)
    RETURN
EVT_END()

EVT_BEGIN(irai_start_event01)
    CALL(evt_npc_set_position, STRING("盗賊団２"), 445, 0, 120)
    CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc_01))
    RETURN
EVT_END()

EVT_BEGIN(irai_start_event04)
    CALL(evt_npc_set_position, STRING("ボロドー２"), -475, 0, -180)
    RETURN
EVT_END()

EvtWord* irai_start_event[] = {
    irai_start_event01, irai_start_event_null, irai_start_event_null, irai_start_event04, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null, irai_start_event_null, irai_start_event_null, irai_start_event_null,
    irai_start_event_null,
};

EVT_BEGIN(irai_end_event_null)
    RETURN
EVT_END()

EVT_BEGIN(irai_end_event01)
    CALL(evt_npc_set_position, STRING("盗賊団２"), 10, 0, -460)
    CALL(evt_door_set_param, GW(11), 0, PTR(&door_data_05_npc))
    RETURN
EVT_END()

EVT_BEGIN(irai_end_event04)
    CALL(evt_npc_set_position, STRING("ボロドー２"), -475, 0, -180)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(irai_end_event19)
    DEBUG_PUT_MSG(STRING("ピートンの依頼キャンセル"))
    CALL(evt_item_delete_check, STRING("wr"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_delete, STRING("wr"))
    END_IF
    RETURN
EVT_END()
#endif

EvtWord* irai_end_event[] = {
    irai_end_event01, irai_end_event_null, irai_end_event_null, irai_end_event04, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null,
    irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null,
#ifdef VERSION_G8ME01
    irai_end_event_null, irai_end_event_null, irai_end_event19, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null,
#else
    irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null,
#endif
    irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null, irai_end_event_null,
};

EVT_BEGIN(gor_irai_init)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_i04_keizi"), 0, PTR(gor_irai_main), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_i04_kunter"), 0, PTR(gor_irai_tenshu_main), 0)
    RETURN
EVT_END()

EVT_BEGIN(gor_irai_lecture)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 150, 12, -156)
    CALL(evt_msg_print, 0, STRING("irai_00"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_01"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("irai_03"))
        SET_VALUE(GSWF_TROUBLE_BOARD_EXPLAINED, 1)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("irai_02"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(gor_irai_main)
    IF_INT_EQUAL(GSWF_TROUBLE_BOARD_EXPLAINED, 0)
#ifdef VERSION_G8ME01
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_dir, 0, 270, 200)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_dir, 1, 270, 200)
        END_INLINE_EVENT
#endif
        CALL_CHILD(PTR(gor_irai_lecture))
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(irai_init_func)
    LABEL(10)
    CALL(irai_main_func)
    CALL(irai_check_func)
    IF_INT_EQUAL(LW(0), -1)
        GOTO(100)
    END_IF
    SET_INT_READ_ADDR(PTR(&irai_msg_exposition))
    SET_VALUE(LW(0), LW(11))
    ADD(LW(0), -1)
    READ_INT_INDEXED(LW(1), LW(0))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, LW(12), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_01"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("irai_73"))
        CALL(irai_start_func)
        SET_INT_READ_ADDR(PTR(irai_start_event))
        SET_VALUE(LW(0), LW(11))
        ADD(LW(0), -1)
        READ_INT_INDEXED(LW(1), LW(0))
        SPAWN_EVENT(LW(1))
        SET_VALUE(GSW_TROUBLE_ACCEPTED, LW(11))
        DEBUG_PUT_REG(GSW_TROUBLE_ACCEPTED)
        ELSE
        CALL(evt_msg_continue)
        GOTO(10)
    END_IF
    LABEL(100)
    CALL(irai_end_func)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(gor_irai_tenshu_main)
    SET_VALUE(LW(0), GSWF_TROUBLE_DONE(TROUBLE_NEED_A_KEY))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_SAFE_DELIVERY))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_PRICE_ADJUSTMENT))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_FIND_THIS_GUY))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_HIT_ME_PLEASE))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_IM_HUNGRY))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_TRY_TO_FIND_ME))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_LISTEN_TO_ME))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_ORDER_ME_AN_ITEM))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_EMERGENCY_SHROOM))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_PLAY_WITH_ME))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_HELP_MY_DADDY))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_HELP_WANTED))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_HEARTFUL_CAKE_RECIPE))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_THE_FOOD_I_WANT))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_ELUSIVE_BADGE))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_NEWSLETTER))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_IMPORTANT_THING))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_TELL_THAT_PERSON))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_LOOKING_FOR_A_GAL))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_GET_THESE_INGREDIENTS))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_I_MUST_HAVE_THAT_BOOK))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_SECURITY_CODE))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_DELIVERY_PLEASE))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_I_CANT_SPEAK))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_I_WANNA_MEET_LUIGI))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_ROUST_THESE_CADS))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_HELP_ME_MAKE_UP))
    ADD(LW(0), GSWF_TROUBLE_DONE(TROUBLE_ERASE_THAT_GRAFFITI))
    IF_INT_EQUAL(LW(0), 30)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 150, 12, -156)
        CALL(evt_msg_print, 0, STRING("irai_74"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_BOARD_EXPLAINED, 0)
        CALL_CHILD(PTR(gor_irai_lecture))
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 150, 12, -156)
        CALL(evt_msg_print, 0, STRING("irai_06"), 0, 0)
        ELSE
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 150, 12, -156)
        CALL(evt_msg_print, 0, STRING("irai_07"), 0, 0)
        CALL(evt_msg_select, 0, STRING("irai_08"))
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_msg_continue)
            DEBUG_PUT_MSG(STRING("依頼メニューキャンセル"))
        END_IF
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(10), GSW_TROUBLE_ACCEPTED)
            SET_INT_READ_ADDR(PTR(&irai_msg_hikiukechu))
            ADD(LW(10), -1)
            READ_INT_INDEXED(LW(11), LW(10))
            CALL(evt_msg_print_add, 0, LW(11))
        END_IF
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("irai_09"))
            CALL(evt_msg_select, 0, STRING("irai_01"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_pouch_get_coin, LW(10))
                IF_INT_GREATER_OR_EQUAL(LW(10), 30)
                    CALL(evt_msg_print_add, 0, STRING("irai_10"))
                    CALL(evt_pouch_add_coin, -30)
                    SET_VALUE(LW(11), GSW_TROUBLE_ACCEPTED)
                    CALL(irai_cancel_func, GSW_TROUBLE_ACCEPTED)
                    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                    SET_INT_READ_ADDR(PTR(irai_end_event))
                    SET_VALUE(LW(0), LW(11))
                    ADD(LW(0), -1)
                    READ_INT_INDEXED(LW(1), LW(0))
                    SPAWN_EVENT(LW(1))
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_11"))
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("irai_12"))
            END_IF
#ifdef VERSION_G8ME01
            SET_VALUE(LW(0), 1)
#endif
        END_IF
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_msg_print_add, 0, STRING("irai_04"))
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

void irai_flag_clear(void) {
    s32 i;

    for (i = GSWF_TROUBLE_WORK(0); i <= GSWF_TROUBLE_WORK(16); i++) {
        swClear(i + EVTDAT_GSWF_BASE);
    }
    for (i = GSW_TROUBLE_WORK(0); i <= GSW_TROUBLE_WORK(5); i++) {
        swByteSet(i + EVTDAT_GSW_BASE, 0);
    }
}

void irai_flag_disp(void) {
    s32 i;

    for (i = GSWF_TROUBLE_WORK(0); i <= GSWF_TROUBLE_WORK(16); i++) {
    }
    for (i = GSW_TROUBLE_WORK(0); i <= GSW_TROUBLE_WORK(5); i++) {
    }
}

USER_FUNC(irai_start_func) {
    irai_flag_clear();
    return EVT_RETURN_DONE;
}

static inline s32 irai_get_no(EventEntry* event) {
    return evtGetValue(event, event->args[0]) - 1;
}

USER_FUNC(irai_cancel_func) {
    s32 no = irai_get_no(event);
    s32* items;
    ItemType item;

    irai_flag_clear();
    items = irai_item_table[no];
    while (TRUE) {
        item = *items;
        if (item == -1) {
            break;
        }
        while (TRUE) {
            if ((s32)pouchCheckItem(item) <= 0) {
                break;
            }
            pouchRemoveItem(item);
        }
        items++;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(irai_init_func) {
    IraiWork* wp;
    s32 seq;
    s32 n;
    s32 i;
    const char* msg;

    irai_flag_disp();
    wp = _mapAlloc(sizeof(IraiWork));
    memset(wp, 0, sizeof(IraiWork));
    wp->state = 0;
    seq = swByteGet(0);
    wp->list[0] = 1;
    wp->list[1] = 2;
    wp->list[2] = 3;
    wp->list[3] = 4;
    wp->list[4] = 5;
    wp->list[5] = 6;
    if (seq >= STG1_17_FATHER_SON_REUNITED) {
        wp->list[6] = 7;
        wp->list[7] = 8;
        wp->list[8] = 9;
    }
    if (seq >= STG2_25_AFTER_THE_BOSS_BATTLE) {
        wp->list[9] = 10;
        wp->list[10] = 11;
        wp->list[11] = 12;
    }
    if (seq >= STG3_32_BOSS_BATTLE) {
        wp->list[12] = 13;
        wp->list[13] = 14;
        wp->list[14] = 15;
    }
    if (seq >= STG4_15_05_PEACH_X_NAUT_PHD_REPORT) {
        wp->list[15] = 16;
        wp->list[16] = 17;
        wp->list[17] = 18;
    }
    if (seq >= STG5_43_X_NAUTS_RETREAT) {
        wp->list[18] = 20;
        wp->list[19] = 21;
    }
    if (seq >= STG5_END_THE_TRAIN_LEAVES) {
        wp->list[20] = 19;
    }
    if (seq >= STG6_54_WELCOME_BACK) {
        wp->list[21] = 22;
        wp->list[22] = 23;
        wp->list[23] = 24;
    }
    if (seq >= STG7_20_LORD_CRUMP_DEFEATED) {
        wp->list[24] = 25;
        wp->list[25] = 26;
        wp->list[26] = 27;
    }
    if (seq >= STG8_16_THE_RETURN_OF_THE_QUEEN) {
        wp->list[27] = 28;
        wp->list[28] = 29;
        wp->list[29] = 30;
    }
    n = 0;
    for (i = 0; i < 30; i++) {
        if (wp->list[i] != 0) {
            n++;
        }
    }
    wp->count = n;
    for (i = 0; i < 30; i++) {
    }
    for (i = 0; i < 30; i++) {
        msg = msgSearch(irai_msg_client[i]);
        strcpy(wp->client[i], msg);
        msg = msgSearch(irai_msg_contents[i]);
        strcpy(wp->contents[i], msg);
    }
    wp->cursor = 0;
    wp->last = wp->cursor;
    wp->top = 0;
    wp->field_0x94 = 0;
    wp->scroll = 0.0f;
    event->lwData[10] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

USER_FUNC(irai_main_func) {
    IraiWork* wp = (IraiWork*)event->lwData[10];
    s32 prev;
    s32 n;

    switch (wp->state) {
        case 0:
            iconEntry2D("iraiYubi", VERSION_PICK(0x1F0, 0x1F8));
            iconFlagOn("iraiYubi", ICON_FLAG_FLIP_X);
            iconEntry2D("iraiArrowUp", VERSION_PICK(0x205, 0x20C));
            iconFlagOn("iraiArrowUp", ICON_FLAG_FLIP_Y);
            iconEntry2D("iraiArrowDown", VERSION_PICK(0x205, 0x20C));
            iconEntry2D("iraiLButton", VERSION_PICK(0x7E, 0x86));
            iconSetScale("iraiLButton", 0.8f);
            iconEntry2D("iraiRButton", VERSION_PICK(0x80, 0x88));
            iconSetScale("iraiRButton", 0.8f);
            iconFlagOn("iraiYubi", ICON_FLAG_HIDDEN);
            iconFlagOn("iraiArrowUp", ICON_FLAG_HIDDEN);
            iconFlagOn("iraiArrowDown", ICON_FLAG_HIDDEN);
            iconFlagOn("iraiLButton", ICON_FLAG_HIDDEN);
            iconFlagOn("iraiRButton", ICON_FLAG_HIDDEN);
            wp->state = 1;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1));
            break;
        case 1:
            if ((keyGetButtonTrg(0) & PAD_BUTTON_A) && swByteGet(SW_TROUBLE_ACCEPTED) == 0 && !swGet(SWF_TROUBLE_DONE(wp->list[wp->cursor]))) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                wp->state = 100;
                break;
            }
            if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
                wp->state = 101;
                break;
            }
            prev = wp->cursor;
            if (keyGetDirRep(0) & DEMO_STICK_DOWN) {
                if (wp->count - 1 <= wp->cursor) {
                    if (keyGetDirTrg(0) & DEMO_STICK_DOWN) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
                        wp->cursor++;
                    }
                } else {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
                    wp->cursor++;
                }
            }
            if (keyGetDirRep(0) & DEMO_STICK_UP) {
                if (wp->cursor <= 0) {
                    if (keyGetDirTrg(0) & DEMO_STICK_UP) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
                        wp->cursor--;
                    }
                } else {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
                    wp->cursor--;
                }
            }
            if (wp->cursor >= wp->count) {
                wp->cursor = 0;
                wp->top = 0;
            }
            if (wp->cursor < 0) {
                wp->cursor = wp->count - 1;
                n = wp->count - 10;
                if (n > 0) {
                    wp->top = n;
                }
            }
            if (prev < wp->cursor) {
                if (wp->cursor - wp->top > 7 && wp->top < wp->count - 10) {
                    wp->top++;
                }
            } else if (prev > wp->cursor) {
                if (wp->cursor - wp->top < 3 && wp->top > 0) {
                    wp->top--;
                }
            }
            if (wp->top > 0 && (keyGetButtonTrg(0) & PAD_TRIGGER_L)) {
                n = 10;
                if (wp->top < 10) {
                    n = wp->top;
                }
                wp->top -= n;
                wp->cursor -= n;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
            }
            if (wp->top < wp->count - 10 && (keyGetButtonTrg(0) & PAD_TRIGGER_R)) {
                n = 10;
                if (-wp->top + (wp->count - 10) < 10) {
                    n = -wp->top + (wp->count - 10);
                }
                wp->top += n;
                wp->cursor += n;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
            }
            if (wp->cursor != wp->last) {
                wp->field_0x94 = 0;
            }
            wp->last = wp->cursor;
            wp->scroll += (wp->top * 30 - wp->scroll) / 6.0f;
            wp->yubiX = -129;
            wp->yubiY = 120 - (wp->cursor - wp->top) * 30;
            iconSetPos("iraiYubi", wp->yubiX, wp->yubiY, 0.0f);
            iconFlagOff("iraiYubi", ICON_FLAG_HIDDEN);
            iconSetPos("iraiArrowUp", 231.0f, 178.0f, 0.0f);
            iconSetPos("iraiArrowDown", 231.0f, -190.0f, 0.0f);
            iconSetPos("iraiLButton", 261.0f, 146.0f, 0.0f);
            iconSetPos("iraiRButton", 261.0f, -190.0f, 0.0f);
            if (wp->top == 0) {
                iconFlagOn("iraiArrowUp", ICON_FLAG_HIDDEN);
                iconFlagOn("iraiLButton", ICON_FLAG_HIDDEN);
            } else {
                iconFlagOff("iraiArrowUp", ICON_FLAG_HIDDEN);
                iconFlagOff("iraiLButton", ICON_FLAG_HIDDEN);
            }
            if (wp->top >= wp->count - 10) {
                iconFlagOn("iraiArrowDown", ICON_FLAG_HIDDEN);
                iconFlagOn("iraiRButton", ICON_FLAG_HIDDEN);
            } else {
                iconFlagOff("iraiArrowDown", ICON_FLAG_HIDDEN);
                iconFlagOff("iraiRButton", ICON_FLAG_HIDDEN);
            }
            break;
        case 100:
            event->lwData[0] = wp->list[wp->cursor];
            wp->state = 0;
            iconDelete("iraiYubi");
            iconDelete("iraiArrowUp");
            iconDelete("iraiArrowDown");
            iconDelete("iraiLButton");
            iconDelete("iraiRButton");
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1));
            return EVT_RETURN_DONE;
        case 101:
            event->lwData[0] = -1;
            wp->state = 0;
            iconDelete("iraiYubi");
            iconDelete("iraiArrowUp");
            iconDelete("iraiArrowDown");
            iconDelete("iraiLButton");
            iconDelete("iraiRButton");
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1));
            return EVT_RETURN_DONE;
    }
    dispEntry(CAMERA_2D, 300.0f, 0, irai_window_disp, wp);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(irai_check_func) {
    if (event->lwData[0] != -1) {
        event->lwData[12] = (EvtWord)irai_msg_kakunin[event->lwData[0] - 1];
        event->lwData[11] = event->lwData[0];
    } else {
        event->lwData[12] = 0;
        event->lwData[11] = -1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(irai_end_func) {
    _mapFree((void*)event->lwData[10]);
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01

// US: text centered in a boxW-wide box at x, x-squeezed to maxW; w is the width variable of the caller.
#define iraiDrawCentered(msg, x, y, boxW, maxW, scale, trans, w) { \
    w = FontGetMessageWidth(msg);                                    \
    if (w > (maxW)) {                                                \
        PSMTXScale(scale, (maxW) / w, 1.0f, 1.0f);                   \
        w = (maxW);                                                  \
    } else {                                                         \
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);                         \
    }                                                                \
    PSMTXTrans(trans, (x) + ((boxW) - w) / 2, y, 0.0f);              \
    PSMTXConcat(trans, scale, trans);                                \
    FontDrawStringMtx(trans, msg);                                   \
}

// US: one row cell, x-squeezed to fit maxW. "y" is stored in the caller's float var v (the
// original reuses one float local for these; this keeps the MWCC float register choice).
#define iraiDrawFit(msg, x, y, maxW, v) {                                              \
    f32 w_;                                                                            \
    v = (y);                                                                           \
    w_ = FontGetMessageWidth(msg);                                                     \
    FontDrawScaleVec((Vec){w_ > (maxW) ? 0.85f * ((maxW) / w_) : 0.85f, 0.85f, 0.85f}); \
    FontDrawString(msg, x, v);                                                         \
}

void irai_window_disp(CameraId camera, void* param) {
    IraiWork* wp = param;
    u32 left;
    u32 top;
    u32 width;
    u32 height;
    const char* msg2;
    s32 i;
    const char* msg;
    s32 no;
    Mtx trans;
    Mtx scale;
    Mtx trans2;
    Mtx scale2;
    f32 nameW;
    f32 w;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, -280.0f, 170.0f, 135.0f, 90.0f, 12.0f);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawScale(0.85f);
    msg = msgSearch("irai_name1_win");
    FontDrawEdge();
    iraiDrawCentered(msg, -280.0f, 152.0f, 135.0f, 115.0f, scale, trans, nameW);
    msg2 = msgSearch("irai_name2_win");
    iraiDrawCentered(msg2, -280.0f, 127.0f, 135.0f, 115.0f, scale, trans, nameW);
    FontDrawEdgeOff();
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -129.0f, 170.0f, 410.0f, 340.0f, 12.0f);
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, -103.0f, 190.0f, 120.0f, 40.0f, 12.0f);
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, 91.0f, 190.0f, 120.0f, 40.0f, 12.0f);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawScale(0.85f);
    msg2 = msgSearch("irai_client_win");
    FontDrawEdge();
    iraiDrawCentered(msg2, -103.0f, 180.0f, 120.0f, 100.0f, scale2, trans2, w);
    FontDrawEdgeOff();
    msg2 = msgSearch("irai_contents_win");
    FontDrawEdge();
    iraiDrawCentered(msg2, 91.0f, 180.0f, 120.0f, 100.0f, scale2, trans2, w);
    FontDrawEdgeOff();
    GXGetScissor(&left, &top, &width, &height);
    GXSetScissor(175, 98, 410, 304);
    FontDrawStart();
    FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
    FontDrawScale(0.85f);
    for (i = 0; i < wp->count; i++) {
        if (swGet(SWF_TROUBLE_DONE(wp->list[i]))) {
            FontDrawColor((GXColor){0x80, 0x80, 0x80, 0xFF});
        } else {
            FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        }
        if (wp->list[i] == swByteGet(SW_TROUBLE_ACCEPTED)) {
            FontDrawColor((GXColor){0xFF, 0x00, 0x00, 0xFF});
        }
        no = wp->list[i] - 1;
        iraiDrawFit(wp->client[no], -119.0f, wp->scroll + (140 - i * 30), 180.0f, w);
        iraiDrawFit(wp->contents[no], 51.0f, wp->scroll + (140 - i * 30), 260.0f, w);
#else
static inline f32 iraiAddI(f32 a, s32 b) {
    return a + b;
}

void irai_window_disp(CameraId camera, void* param) {
    f32 negX;
    IraiWork* wp = param;
    u32 left;
    u32 top;
    u32 width;
    u32 height;
    const char* msg;
    s32 i;
    s32 no;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, -280.0f, 170.0f, 135.0f, 90.0f, 12.0f);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawScale(0.85f);
    msg = msgSearch("irai_name1_win");
    FontDrawEdge();
    FontDrawString(msg, -270.0f, 152.0f);
    FontDrawString(msgSearch("irai_name2_win"), -270.0f, 127.0f);
    FontDrawEdgeOff();
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -129.0f, 170.0f, 410.0f, 340.0f, 12.0f);
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, -103.0f, 190.0f, 120.0f, 40.0f, 12.0f);
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, 91.0f, 190.0f, 120.0f, 40.0f, 12.0f);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawScale(0.85f);
    negX = -77.0f;
    msg = msgSearch("irai_client_win");
    FontDrawEdge();
    FontDrawString(msg, negX, 180.0f);
    FontDrawEdgeOff();
    msg = msgSearch("irai_contents_win");
    FontDrawEdge();
    FontDrawString(msg, 115.0f, 180.0f);
    FontDrawEdgeOff();
    GXGetScissor(&left, &top, &width, &height);
    GXSetScissor(175, 98, 410, 304);
    FontDrawStart();
    FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
    FontDrawScale(0.85f);
    for (i = 0; i < wp->count; i++) {
        if (swGet(SWF_TROUBLE_DONE(wp->list[i]))) {
            FontDrawColor((GXColor){0x80, 0x80, 0x80, 0xFF});
        } else {
            FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        }
        if (wp->list[i] == swByteGet(SW_TROUBLE_ACCEPTED)) {
            FontDrawColor((GXColor){0xFF, 0x00, 0x00, 0xFF});
        }
        no = wp->list[i] - 1;
        FontDrawString(wp->client[no], -119.0f, iraiAddI(wp->scroll, 140 - i * 30));
        FontDrawString(wp->contents[no], 51.0f, wp->scroll + (140 - i * 30));
#endif
    }
    GXSetScissor(left, top, width, height);
}
