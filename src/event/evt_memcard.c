#include "evt/evt_cmd.h"
#include "evt/evt_fade.h"
#include "manager/cardmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "evt/evt_msg.h"

// cardmgr.c helpers (named cardE80/cardEAC/cardED0 in cardmgr.h)

USER_FUNC(evt_msg_print_add_insert);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_seq_load_keep_msg_window);

// Functions are emitted in reverse source order (deferred inlining)
USER_FUNC(memcard_code) {
    EvtWord* args = event->args;
    s32 code = cardGetCode();

    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, args[0], code);
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_buf_reset) {
    cardBufReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_watch_start) {
    cardInsertWatchStart();
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_watch_chk) {
    EvtWord* args = event->args;

    if (cardInsertChanged()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_watch_end) {
    cardInsertWatchEnd();
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_load) {
    if (isFirstCall) {
        cardBufReset();
        cardReadAll();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_format) {
    if (isFirstCall) {
        cardFormat();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_create) {
    if (isFirstCall) {
        cardCreate();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_delete) {
    EvtWord* args = event->args;
    s32 slot = evtGetValue(event, args[0]) - 1;

    if (isFirstCall) {
        cardErase(slot);
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_copy) {
    EvtWord* args = event->args;
    s32 from = evtGetValue(event, args[0]) - 1;
    s32 to = evtGetValue(event, args[1]) - 1;

    if (isFirstCall) {
        cardCopy(from, to);
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_header_write) {
    if (isFirstCall) {
        cardWriteHeader();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_ipl) {
    gp->resetType = 2;
    return EVT_RETURN_BLOCK;
}

USER_FUNC(memcard_file_existance) {
    EvtWord* args = event->args;
    s32 slot = evtGetValue(event, args[0]) - 1;

    if (cardGetFilePtr()->saves[slot].data.flags & 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(memcard_write) {
    if (isFirstCall) {
        cardWrite(gp->saveSlot);
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

EvtWord evt_memcard_nosave[19];
EvtWord evt_memcard_check[322];
EvtWord evt_memcard_start[32];
EvtWord evt_memcard_delete[80];
EvtWord evt_memcard_copy[126];
EvtWord evt_memcard_save[510];

EVT_BEGIN(evt_memcard_nosave)
    CALL(evt_msg_print_add, 0, STRING("msg_nosave_start"))
    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(GF(0), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_memcard_check)
    SET_VALUE(GF(0), 0)
    SET_VALUE(LF(10), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_msg_print, 0, STRING("msg_mcard_fileload_warnning"), 0, 0)
            CALL(memcard_load)
        END_IF
        CALL(memcard_code, LW(0))
        IF_INT_EQUAL(LW(10), 1)
            CALL(memcard_buf_reset)
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_lord_finish"))
            SET_VALUE(GF(0), 1)
            CASE_EQUAL(-3)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_nocard"))
            CALL_CHILD(PTR(evt_memcard_nosave))
            CASE_EQUAL(-2)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_card"))
            CALL_CHILD(PTR(evt_memcard_nosave))
            CASE_OR_EQUAL(-13)
            CASE_OR_EQUAL(-6)
            CALL(memcard_watch_start)
            CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_format"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_alldelete"))
                CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(memcard_watch_chk, LW(0))
                    IF_INT_EQUAL(LW(0), 1)
                        CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                        CALL_CHILD(PTR(evt_memcard_nosave))
                        BREAK_SWITCH
                    END_IF
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_warning"))
                    CALL(memcard_format)
                    CALL(memcard_code, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_miss"))
                        SET_VALUE(LF(10), 1)
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_finish"))
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
                        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
                        IF_INT_EQUAL(LW(0), 0)
                            CALL(memcard_watch_chk, LW(0))
                            IF_INT_EQUAL(LW(0), 1)
                                CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                                CALL_CHILD(PTR(evt_memcard_nosave))
                                BREAK_SWITCH
                            END_IF
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
                            CALL(memcard_create)
                            CALL(memcard_code, LW(0))
                            IF_INT_NOT_EQUAL(LW(0), 0)
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
                                SET_VALUE(LF(10), 1)
                                ELSE
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                                CALL(evt_msg_continue)
                                SET_VALUE(GF(0), 1)
                            END_IF
                            ELSE
                            CALL_CHILD(PTR(evt_memcard_nosave))
                        END_IF
                    END_IF
                    ELSE
                    CALL_CHILD(PTR(evt_memcard_nosave))
                END_IF
                ELSE
                CALL_CHILD(PTR(evt_memcard_nosave))
            END_IF
            CASE_END
            CASE_EQUAL(-4)
            CALL(memcard_watch_start)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(memcard_watch_chk, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                    CALL_CHILD(PTR(evt_memcard_nosave))
                    BREAK_SWITCH
                END_IF
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
                CALL(memcard_create)
                CALL(memcard_code, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
                    SET_VALUE(LF(10), 1)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                    CALL(evt_msg_continue)
                    SET_VALUE(GF(0), 1)
                END_IF
                ELSE
                CALL_CHILD(PTR(evt_memcard_nosave))
            END_IF
            CASE_OR_EQUAL(-8)
            CASE_OR_EQUAL(-9)
            CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_nospace"))
            CALL(evt_msg_print_add, 0, STRING("msg_gc_mcard_open"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(memcard_ipl)
                ELSE
                CALL_CHILD(PTR(evt_memcard_nosave))
            END_IF
            CASE_END
            CASE_EQUAL(-10)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_savedata_2"))
            CALL_CHILD(PTR(evt_memcard_nosave))
            CASE_DEFAULT
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_damage"))
            CALL_CHILD(PTR(evt_memcard_nosave))
        END_SWITCH
        CALL(memcard_watch_end, LW(0))
        IF_INT_EQUAL(LF(10), 1)
            SET_VALUE(LF(10), 0)
            SET_VALUE(LW(10), 1)
            ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_memcard_start)
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), 1)
    CALL(evt_msg_print_insert, 0, STRING("msg_savefile_start"), 0, 0, LW(10))
    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(GW(0), 1)
        ELSE
        SET_VALUE(GW(0), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_memcard_delete)
    SET_VALUE(GF(0), 0)
    SET_VALUE(LW(10), LW(0))
    ADD(LW(10), 1)
    CALL(evt_msg_print_insert, 0, STRING("msg_savefile_delete"), 0, 0, LW(10))
    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("msg_savefile_delete_warnning"))
        CALL(memcard_delete, LW(10))
        CALL(memcard_code, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_savefile_delete_copy_miss"))
            ELSE
            CALL(evt_msg_print_add_insert, 0, STRING("msg_savefile_delete_finish"), LW(10))
            RETURN
        END_IF
        ELSE
        CALL(evt_msg_continue)
        RETURN
    END_IF
    CALL(evt_fade_entry, 24, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, 26, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_continue)
    CALL(evt_seq_load_keep_msg_window)
    SET_VALUE(GF(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_memcard_copy)
    SET_VALUE(GF(0), 0)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(11), LW(1))
    ADD(LW(10), 1)
    ADD(LW(11), 1)
    CALL(memcard_file_existance, LW(11), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_insert, 0, STRING("msg_savefile_delete_copy_point"), 0, 0, LW(11))
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_savefile_copy_warnning"))
            CALL(memcard_copy, LW(10), LW(11))
            CALL(memcard_code, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_savefile_delete_copy_miss"))
                ELSE
                CALL(evt_msg_print_add_insert, 0, STRING("msg_savefile_copy_finish"), LW(10), LW(11))
                RETURN
            END_IF
            ELSE
            CALL(evt_msg_continue)
            RETURN
        END_IF
        ELSE
        CALL(evt_msg_print, 0, STRING("msg_savefile_copy_warnning"), 0, 0)
        CALL(memcard_copy, LW(10), LW(11))
        CALL(memcard_code, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_savefile_delete_copy_miss"))
            ELSE
            CALL(evt_msg_print_add_insert, 0, STRING("msg_savefile_copy_finish"), LW(10), LW(11))
            RETURN
        END_IF
    END_IF
    CALL(evt_fade_entry, 24, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, 26, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_continue)
    CALL(evt_seq_load_keep_msg_window)
    SET_VALUE(GF(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_memcard_save)
    SET_VALUE(LW(10), LW(0))
    IF_INT_EQUAL(LW(10), 23)
        GOTO(1)
    END_IF
    LABEL(0)
    IF_INT_EQUAL(LF(10), 1)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_select"))
        SET_VALUE(LF(10), 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("msg_mcard_save_select"), 0, 0)
    END_IF
    IF_INT_EQUAL(LW(10), 21)
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno_ending"))
        ELSE
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_continue)
        RETURN
    END_IF
    LABEL(1)
    SET_VALUE(GSW(4), LW(10))
    IF_INT_EQUAL(LW(10), 23)
        CALL(evt_msg_print, 0, STRING("msg_savefile_first_warnning"), 0, 0)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
    END_IF
    GOTO(3)
    LABEL(2)
    CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
    CALL(evt_msg_print_add, 0, STRING("msg_savefile_first_warnning"))
    LABEL(3)
    CALL(memcard_header_write)
    CALL(memcard_write)
    LABEL(4)
    SET_VALUE(LF(11), 0)
    CALL(memcard_code, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
        RETURN
        CASE_EQUAL(-3)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_nocard"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        CASE_EQUAL(-2)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_card"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        CASE_OR_EQUAL(-13)
        CASE_OR_EQUAL(-6)
        CALL(memcard_watch_start)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format"))
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_alldelete"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(memcard_watch_chk, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                    BREAK_SWITCH
                END_IF
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_warning"))
                CALL(memcard_format)
                CALL(memcard_code, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_miss"))
                    SET_VALUE(LF(11), 1)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_finish"))
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
                    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
                        CALL(memcard_watch_chk, LW(0))
                        IF_INT_EQUAL(LW(0), 1)
                            CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                            BREAK_SWITCH
                        END_IF
                        CALL(memcard_create)
                        CALL(memcard_code, LW(0))
                        IF_INT_NOT_EQUAL(LW(0), 0)
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
                            SET_VALUE(LF(11), 1)
                            ELSE
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
                            CALL(memcard_write)
                            CALL(memcard_code, LW(0))
                            IF_INT_EQUAL(LW(0), 0)
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
                                RETURN
                                ELSE
                                SET_VALUE(LF(11), 1)
                            END_IF
                        END_IF
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
                    END_IF
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        END_IF
        CASE_END
        CASE_EQUAL(-4)
        CALL(memcard_watch_start)
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
            CALL(memcard_watch_chk, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                BREAK_SWITCH
            END_IF
            CALL(memcard_create)
            CALL(memcard_code, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
                SET_VALUE(LF(11), 1)
                ELSE
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
                CALL(memcard_write)
                CALL(memcard_code, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
                    RETURN
                    ELSE
                    SET_VALUE(LF(11), 1)
                END_IF
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        END_IF
        CASE_EQUAL(-10)
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_savedata"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        CASE_OR_EQUAL(-8)
        CASE_OR_EQUAL(-9)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_nospace"))
        IF_INT_EQUAL(LW(10), 23)
            CALL(evt_msg_print_add, 0, STRING("msg_gc_mcard_open"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("msg_gc_mcard_nosave"))
                CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
                    ELSE
                    CALL(memcard_ipl)
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        END_IF
        CASE_END
        CASE_DEFAULT
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_damage"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
    END_SWITCH
    CALL(memcard_watch_end)
    IF_INT_EQUAL(LF(11), 1)
        GOTO(4)
    END_IF
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_msg_continue)
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("msg_savefile_nosave_warnning"), 0, 0)
    IF_INT_EQUAL(LW(10), 21)
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno_ending"))
        ELSE
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno3"))
    END_IF
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(LW(10), 23)
            GOTO(2)
            ELSE
            GOTO(1)
        END_IF
    END_IF
    CALL(evt_msg_continue)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_memcard_save_wrapper)
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(evt_memcard_save)
    ELSE
        CALL_CHILD(evt_memcard_save)
        WAIT_MS(1000)
    END_IF
    RETURN
EVT_END()

#endif
