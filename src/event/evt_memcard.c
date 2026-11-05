// Scripts and commands for saving and loading on the memory card (cardmgr).

#include "version.h"
#include "evt/evt_cmd.h"
#include "seq/seq_load.h"
#include "evt/evt_fade.h"
#include "manager/cardmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "evt/evt_msg.h"


/**
 * Functions are emitted in reverse source order (deferred inlining). Waits
 * until the current memory card operation finishes, then stores cardmgr's result code in args[0]:
 * 0 on success, negative for errors (-3 no card, -2 a different card, -6/-13 needs formatting, -4 no save file,
 * -8/-9 no space, -10 save data it cannot use), as the save/load scripts below switch on it.
 */
USER_FUNC(memcard_code) {
    EvtWord* args = event->args;
    s32 code = cardGetCode();

    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, args[0], code);
    return EVT_RETURN_DONE;
}

/**
 * Clears cardmgr's read buffer so the next check rereads the card;
 * evt_memcard_check in JP and later calls it before a retry.
 */
// Descriptive name (not in the symbol maps); placeholder unk_802564b4, JP 0x802564B4. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(memcard_buf_reset) {
    cardBufReset();
    return EVT_RETURN_DONE;
}
#endif

/** Starts watching the card slot for the card being pulled or swapped while a format or create prompt is open. */
// Descriptive name (not in the symbol maps); placeholder unk_80256490, JP 0x80256490. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(memcard_watch_start) {
    cardInsertWatchStart();
    return EVT_RETURN_DONE;
}
#endif

/**
 * Sets args[0] to 1 if the card in the slot changed since memcard_watch_start, else 0;
 * the scripts then print msg_out_mcard_warnning and abandon the format or create.
 */
// Descriptive name (not in the symbol maps); placeholder unk_80256428, JP 0x80256428. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(memcard_watch_chk) {
    EvtWord* args = event->args;

    if (cardInsertChanged()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
#endif

/** Stops the card-slot watch started by memcard_watch_start. */
// Descriptive name (not in the symbol maps); placeholder unk_80256404, JP 0x80256404. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(memcard_watch_end) {
    cardInsertWatchEnd();
    return EVT_RETURN_DONE;
}
#endif

/**
 * Reads the whole memory card (save file and headers) on the first call,
 * blocking the script until the read is done.
 */
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

/** Formats the memory card on the first call, blocking the script until it finishes; check memcard_code afterwards. */
USER_FUNC(memcard_format) {
    if (isFirstCall) {
        cardFormat();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Creates the game's save file on the card on the first call, blocking until done. */
USER_FUNC(memcard_create) {
    if (isFirstCall) {
        cardCreate();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Erases save slot args[0] (1-based, as shown on the file select screen), blocking until the card write finishes. */
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

/** Copies save slot args[0] onto slot args[1] (both 1-based), blocking until the card write finishes. */
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

/** Writes the save file's header block on the first call, blocking until done. */
USER_FUNC(memcard_header_write) {
    if (isFirstCall) {
        cardWriteHeader();
    }
    if (cardIsExec()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Asks for a reset to the GameCube main menu (IPL) so the player can free card space there;
 * gp->resetType 2 makes the main loop reset, and the script never resumes.
 */
USER_FUNC(memcard_ipl) {
    gp->resetType = RESET_TYPE_MENU;
    return EVT_RETURN_BLOCK;
}

/**
 * Sets args[1] to 1 if save slot args[0] (1-based) holds a save and 0
 * if it is empty (save flag bit 0 set reads as empty); evt_memcard_copy uses it to ask before overwriting.
 */
USER_FUNC(memcard_file_existance) {
    EvtWord* args = event->args;
    s32 slot = evtGetValue(event, args[0]) - 1;

    if (cardGetFilePtr()->saves[slot].data.flags & CARD_SAVE_FLAG_ERASED) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return EVT_RETURN_DONE;
}

/** Writes the current game to the save slot in gp->saveSlot, blocking until the card write finishes. */
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

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_memcard_check[];
#else
EvtWord evt_memcard_check[322];
#endif

EvtWord evt_memcard_start[32];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_memcard_delete[];
extern EvtWord evt_memcard_copy[];
extern EvtWord evt_memcard_save[];
#else
EvtWord evt_memcard_delete[80];
EvtWord evt_memcard_copy[126];
EvtWord evt_memcard_save[510];
#endif

/**
 * Tells the player they can continue without saving and asks yes or no;
 * yes sets GF(0) so the caller goes on without a memory card.
 */
EVT_BEGIN(evt_memcard_nosave)
    CALL(evt_msg_print_add, 0, STRING("msg_nosave_start"))
    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(GF(0), 1)
    END_IF
    RETURN
EVT_END()

/**
 * Boot-time card check: reads the card, offers to format it or create the save file, and sets
 * GF(0) once the game may start. JP and later also watch for the card being pulled mid-prompt and retry a
 * failed format or create; in the early demos a failure offers to play without saving instead.
 */
EVT_BEGIN(evt_memcard_check)
    SET_VALUE(GF(0), 0)

    // Load the save data from the Memory Card in Slot A and act on the result.
#if VERSION >= VERSION_JP
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
#else
    LOOP(0)
            CALL(evt_msg_print, 0, STRING("msg_mcard_fileload_warnning"), 0, 0)
            CALL(memcard_load)
        CALL(memcard_code, LW(0))
#endif
        SWITCH(LW(0))
            // Loaded: the game may start.
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_lord_finish"))
            SET_VALUE(GF(0), 1)

            // No card, or some other device in Slot A: offer to play without saving.
            CASE_EQUAL(CARD_RESULT_NOCARD)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_nocard"))
            CALL_CHILD(evt_memcard_nosave)
            CASE_EQUAL(CARD_RESULT_WRONGDEVICE)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_card"))
            CALL_CHILD(evt_memcard_nosave)

            // Corrupted card: offer to format it (asking twice), then to create a save file;
            // any refusal (and, in the early demos, any failure) offers to play without saving.
#if VERSION <= VERSION_DEMO_JP
            CASE_OR_EQUAL(-13)
            CASE_OR_EQUAL(-6)
#else
            CASE_OR_EQUAL(CARD_RESULT_ENCODING)
            CASE_OR_EQUAL(CARD_RESULT_BROKEN)
            CALL(memcard_watch_start)
#endif
            CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_format"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_alldelete"))
                CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
                IF_INT_EQUAL(LW(0), 0)
#if VERSION >= VERSION_JP
                    CALL(memcard_watch_chk, LW(0))
                    IF_INT_EQUAL(LW(0), 1)
                        CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                        CALL_CHILD(evt_memcard_nosave)
                        BREAK_SWITCH
                    END_IF
#endif
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_warning"))
                    CALL(memcard_format)
                    CALL(memcard_code, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_miss"))
#if VERSION <= VERSION_DEMO_JP
                        CALL_CHILD(evt_memcard_nosave)
#else
                        SET_VALUE(LF(10), 1)
#endif
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_finish"))
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
                        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
                        IF_INT_EQUAL(LW(0), 0)
#if VERSION >= VERSION_JP
                            CALL(memcard_watch_chk, LW(0))
                            IF_INT_EQUAL(LW(0), 1)
                                CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                                CALL_CHILD(evt_memcard_nosave)
                                BREAK_SWITCH
                            END_IF
#endif
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
                            CALL(memcard_create)
                            CALL(memcard_code, LW(0))
                            IF_INT_NOT_EQUAL(LW(0), 0)
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
#if VERSION <= VERSION_DEMO_JP
                                CALL_CHILD(evt_memcard_nosave)
#else
                                SET_VALUE(LF(10), 1)
#endif
                                ELSE
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                                CALL(evt_msg_continue)
                                SET_VALUE(GF(0), 1)
                            END_IF
                            ELSE
                            CALL_CHILD(evt_memcard_nosave)
                        END_IF
                    END_IF
                    ELSE
                    CALL_CHILD(evt_memcard_nosave)
                END_IF
                ELSE
                CALL_CHILD(evt_memcard_nosave)
            END_IF
            CASE_END
            // No save file on the card: offer to create one.
#if VERSION <= VERSION_DEMO_JP
            CASE_EQUAL(-4)
#else
            CASE_EQUAL(CARD_RESULT_NOFILE)
            CALL(memcard_watch_start)
#endif
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
            IF_INT_EQUAL(LW(0), 0)
#if VERSION >= VERSION_JP
                CALL(memcard_watch_chk, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                    CALL_CHILD(evt_memcard_nosave)
                    BREAK_SWITCH
                END_IF
#endif
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
                CALL(memcard_create)
                CALL(memcard_code, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
#if VERSION <= VERSION_DEMO_JP
                    CALL_CHILD(evt_memcard_nosave)
#else
                    SET_VALUE(LF(10), 1)
#endif
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                    CALL(evt_msg_continue)
                    SET_VALUE(GF(0), 1)
                END_IF
                ELSE
                CALL_CHILD(evt_memcard_nosave)
            END_IF
            CASE_OR_EQUAL(CARD_RESULT_NOENT)
            CASE_OR_EQUAL(CARD_RESULT_INSSPACE)
            CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_nospace"))
            CALL(evt_msg_print_add, 0, STRING("msg_gc_mcard_open"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(memcard_ipl)
            ELSE
                CALL_CHILD(evt_memcard_nosave)
            END_IF
            CASE_END
#if VERSION >= VERSION_JP
            CASE_EQUAL(CARD_RESULT_NOPERM)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_savedata_2"))
            CALL_CHILD(evt_memcard_nosave)
#endif

            // Damaged card: it cannot be used.
            CASE_DEFAULT
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_damage"))
            CALL_CHILD(evt_memcard_nosave)
        END_SWITCH
#if VERSION >= VERSION_JP
        CALL(memcard_watch_end, LW(0))
        IF_INT_EQUAL(LF(10), 1)
            SET_VALUE(LF(10), 0)
            SET_VALUE(LW(10), 1)
        ELSE
#endif
            BREAK_LOOP
#if VERSION >= VERSION_JP
        END_IF
#endif
    END_LOOP
    RETURN
EVT_END()

/**
 * File select: asks whether to start the game from save slot LW(0) (0-based; shown 1-based).
 * Sets GW(0) to 1 for yes and 0 for no.
 */
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

/**
 * File select: asks to delete save slot LW(0) (0-based) and erases it.
 * In JP and later, if the erase fails, the curtains close and open,
 * the load screen reloads the card and GF(0) is set so the file list is refreshed.
 */
EVT_BEGIN(evt_memcard_delete)
#if VERSION >= VERSION_JP
    SET_VALUE(GF(0), 0)
#endif
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
#if VERSION >= VERSION_JP
            RETURN
#endif
        END_IF
        ELSE
        CALL(evt_msg_continue)
#if VERSION >= VERSION_JP
        RETURN
#endif
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_fade_entry, FADE_CURTAIN_SIDES_A1_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, FADE_CURTAIN_SIDES_Z1_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_continue)
    CALL(evt_seq_load_keep_msg_window)
    SET_VALUE(GF(0), 1)
#endif
    RETURN
EVT_END()

/**
 * File select: copies save slot LW(0) onto slot LW(1) (0-based),
 * asking first when the target slot already holds a save.
 * In JP and later, on failure the curtains close and open and the file list is reloaded (GF(0) set), as in
 * evt_memcard_delete.
 */
EVT_BEGIN(evt_memcard_copy)
#if VERSION >= VERSION_JP
    SET_VALUE(GF(0), 0)
#endif
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
#if VERSION >= VERSION_JP
                RETURN
#endif
            END_IF
            ELSE
            CALL(evt_msg_continue)
#if VERSION >= VERSION_JP
            RETURN
#endif
        END_IF
        ELSE
        CALL(evt_msg_print, 0, STRING("msg_savefile_copy_warnning"), 0, 0)
        CALL(memcard_copy, LW(10), LW(11))
        CALL(memcard_code, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_savefile_delete_copy_miss"))
        ELSE
            CALL(evt_msg_print_add_insert, 0, STRING("msg_savefile_copy_finish"), LW(10), LW(11))
#if VERSION >= VERSION_JP
            RETURN
#endif
        END_IF
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_fade_entry, FADE_CURTAIN_SIDES_A1_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, FADE_CURTAIN_SIDES_Z1_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_continue)
    CALL(evt_seq_load_keep_msg_window)
    SET_VALUE(GF(0), 1)
#endif
    RETURN
EVT_END()

/**
 * Save script, run from Save Blocks, intermissions and the first save after name entry.
 * LW(0) is the save kind stored in GSW_SAVE_RESUME_POINT:
 * 0 is a Save Block, 23 the new-file save (saves without asking),
 * 21 the ending save (which uses the ending yes/no prompt in JP and later).
 * Handles format/create prompts, retries a failed write (JP and later),
 * and on failure warns that the game was not saved and offers to try again.
 */
EVT_BEGIN(evt_memcard_save)
    // Ask "Save all game progress up to this point?" (the new-file save skips the question).
    SET_VALUE(LW(10), LW(0))
    IF_INT_EQUAL(LW(10), 23)
        GOTO(1)
    END_IF
    LABEL(0)
    IF_INT_EQUAL(LF(10), 1)
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_select"))
        SET_VALUE(LF(10), 0)
    ELSE
        CALL(evt_msg_print, 0, STRING("msg_mcard_save_select"), 0, 0)
    END_IF
#if VERSION >= VERSION_JP
    IF_INT_EQUAL(LW(10), 21)
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno_ending"))
    ELSE
#endif
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
#if VERSION >= VERSION_JP
    END_IF
#endif
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_continue)
        RETURN
    END_IF

    // Remember the save kind and write the save file.
    LABEL(1)
    SET_VALUE(GSW_SAVE_RESUME_POINT, LW(10))
    IF_INT_EQUAL(LW(10), 23)
        CALL(evt_msg_print, 0, STRING("msg_savefile_first_warnning"), 0, 0)
    ELSE
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
    END_IF
#if VERSION >= VERSION_JP
    GOTO(3)
    LABEL(2)
    CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
    CALL(evt_msg_print_add, 0, STRING("msg_savefile_first_warnning"))
    LABEL(3)
#endif
    CALL(memcard_header_write)
    CALL(memcard_write)
#if VERSION >= VERSION_JP
    LABEL(4)
    SET_VALUE(LF(11), 0)
#endif
    CALL(memcard_code, LW(0))

    // Saved, or explain why not: no card, wrong device, a corrupted card (offer to format
    // and create a save file, then write again), no save file (offer to create one),
    // not the original card, no space (offer the Memory Card Screen) or a damaged card.
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
        RETURN
        CASE_EQUAL(CARD_RESULT_NOCARD)
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_nocard"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        CASE_EQUAL(CARD_RESULT_WRONGDEVICE)
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_card"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#if VERSION <= VERSION_DEMO_JP
        CASE_OR_EQUAL(-13)
        CASE_OR_EQUAL(-6)
#else
        CASE_OR_EQUAL(CARD_RESULT_ENCODING)
        CASE_OR_EQUAL(CARD_RESULT_BROKEN)
        CALL(memcard_watch_start)
#endif
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_format"))
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_alldelete"))
            CALL(evt_msg_select, 0, STRING("msg_mcard_yesno2"))
            IF_INT_EQUAL(LW(0), 0)
#if VERSION >= VERSION_JP
                CALL(memcard_watch_chk, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                    BREAK_SWITCH
                END_IF
#endif
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_warning"))
                CALL(memcard_format)
                CALL(memcard_code, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_miss"))
#if VERSION <= VERSION_DEMO_JP
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#else
                    SET_VALUE(LF(11), 1)
#endif
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_format_finish"))
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
                    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
#if VERSION >= VERSION_JP
                        CALL(memcard_watch_chk, LW(0))
                        IF_INT_EQUAL(LW(0), 1)
                            CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                            BREAK_SWITCH
                        END_IF
#endif
                        CALL(memcard_create)
                        CALL(memcard_code, LW(0))
                        IF_INT_NOT_EQUAL(LW(0), 0)
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
#if VERSION <= VERSION_DEMO_JP
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#else
                            SET_VALUE(LF(11), 1)
#endif
                            ELSE
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
                            CALL(memcard_write)
                            CALL(memcard_code, LW(0))
                            IF_INT_EQUAL(LW(0), 0)
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
                                RETURN
                            ELSE
#if VERSION <= VERSION_DEMO_JP
                                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#else
                                SET_VALUE(LF(11), 1)
#endif
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
#if VERSION <= VERSION_DEMO_JP
        CASE_EQUAL(-4)
#else
        CASE_EQUAL(CARD_RESULT_NOFILE)
        CALL(memcard_watch_start)
#endif
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data"))
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_warning"))
#if VERSION >= VERSION_JP
            CALL(memcard_watch_chk, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("msg_out_mcard_warnning"))
                BREAK_SWITCH
            END_IF
#endif
            CALL(memcard_create)
            CALL(memcard_code, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_miss"))
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#else
                SET_VALUE(LF(11), 1)
#endif
                ELSE
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_make_data_finish"))
                CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_warnning"))
                CALL(memcard_write)
                CALL(memcard_code, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_finish"))
                    RETURN
                ELSE
#if VERSION <= VERSION_DEMO_JP
                    CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
#else
                    SET_VALUE(LF(11), 1)
#endif
                END_IF
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        END_IF
        CASE_EQUAL(CARD_RESULT_NOPERM)
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_different_savedata"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        CASE_OR_EQUAL(CARD_RESULT_NOENT)
        CASE_OR_EQUAL(CARD_RESULT_INSSPACE)
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_nospace"))
#if VERSION >= VERSION_JP
        IF_INT_EQUAL(LW(10), 23)
#endif
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
#if VERSION >= VERSION_JP
            ELSE
            CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
        END_IF
#endif
        CASE_END
        CASE_DEFAULT
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_damage"))
        CALL(evt_msg_print_add, 0, STRING("msg_mcard_save_miss"))
    END_SWITCH
#if VERSION >= VERSION_JP
    CALL(memcard_watch_end)
    IF_INT_EQUAL(LF(11), 1)
        GOTO(4)
    END_IF
#endif

    // After a failure outside Save Blocks (and, in the early demos, the new-file save),
    // warn that progress was not saved and offer to save again.
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_msg_continue)
        RETURN
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("msg_savefile_nosave_warnning"), 0, 0)
    IF_INT_EQUAL(LW(10), 21)
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno_ending"))
    ELSE
        CALL(evt_msg_select, 0, STRING("msg_mcard_yesno3"))
    END_IF
    IF_INT_EQUAL(LW(0), 0)
#endif
        IF_INT_EQUAL(LW(10), 23)
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_msg_continue)
            RETURN
#else
            GOTO(2)
            ELSE
            GOTO(1)
#endif
        END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_msg_print_add, 0, STRING("msg_savefile_nosave_warnning"))
    CALL(evt_msg_select, 0, STRING("msg_mcard_yesno3"))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(1)
#endif
    END_IF
    CALL(evt_msg_continue)
    RETURN
EVT_END()

#if VERSION >= VERSION_US
/**
 * US and later: runs evt_memcard_save, then for any save other than a Save Block (LW(0) nonzero) waits one more
 * second. Save Blocks, intermissions and the new-file save on the load screen call it.
 */
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
