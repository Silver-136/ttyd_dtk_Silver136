#include "event/evt_memcard.h"
#include "event/evt_cmd.h"

extern void* gp;

void unk_800b2c08(void);
void unk_800b2c2c(void);
void cardBufReset(void);
void cardWriteHeader(void);
void cardCreate(void);
void cardFormat(void);
void cardErase(s32 fileNo);
void cardWrite(void* saveData);
void cardReadAll(void);
s32 cardIsExec(void);
s32 unk_800b2bdc(void);
s32 cardGetCode(void);
void* cardGetFilePtr(void);
void cardCopy(s32 srcFile, s32 dstFile);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(memcard_code) {
    s32* args;
    s32 code;

    args = event->args;
    code = cardGetCode();
    if (cardIsExec() != 0) {
        return 0;
    }
    evtSetValue(event, args[0], code);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(unk_8025c430) {
    cardBufReset();
    return 2;
}

USER_FUNC(unk_8025c40c) {
    unk_800b2c2c();
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(unk_8025c3a4) {
    s32* args;

    args = event->args;
    if (unk_800b2bdc() != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(unk_8025c380) {
    unk_800b2c08();
    return 2;
}
s32 memcard_load(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardBufReset();
        cardReadAll();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_format(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardFormat();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_create(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardCreate();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_delete(EventEntry* event, BOOL firstRun) {
    s32 fileNo = evtGetValue(event, event->args[0]) - 1;
    if (firstRun != 0) {
        cardErase(fileNo);
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(memcard_copy) {
    s32* args;
    s32 srcFile;
    s32 dstFile;

    args = event->args;
    srcFile = evtGetValue(event, args[0]) - 1;
    dstFile = evtGetValue(event, args[1]) - 1;
    if (isFirstCall != 0) {
        cardCopy(srcFile, dstFile);
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 memcard_header_write(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardWriteHeader();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_ipl(void) {
    *(s32*)((s32)gp + 0x1278) = 2;
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(memcard_file_existance) {
    s32* args;
    s32 fileNo;
    void* filePtr;

    args = event->args;
    fileNo = evtGetValue(event, args[0]) - 1;
    filePtr = cardGetFilePtr();
    if (*(u16*)((s32)filePtr + (fileNo << 14) + 0x2000) & 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 memcard_write(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardWrite(*(void**)((s32)gp + 0x11D0));
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

/* Forward declarations for recovered evt_memcard event bytecode. */
extern u32 evt_memcard_check[];
extern u32 evt_memcard_copy[];
extern u32 evt_memcard_delete[];
extern u32 evt_memcard_nosave[];
extern u32 evt_memcard_save[];
extern u32 evt_memcard_start[];
extern u32 unk_evt_803bac3c[];
extern s32 evt_fade_end_wait();
extern s32 evt_fade_entry();
extern s32 evt_msg_continue();
extern s32 evt_msg_print();
extern s32 evt_msg_print_add();
extern s32 evt_msg_print_add_insert();
extern s32 evt_msg_print_insert();
extern s32 evt_msg_select();
extern s32 unk_800f72e4();

/* Recovered evt_memcard .rodata message/script strings. */
const char str_msg_nosave_start_80302ba0[] = "msg_nosave_start";
const char str_msg_mcard_yesno2_80302bb4[] = "msg_mcard_yesno2";
const char str_msg_mcard_fileload_w_80302bc8[] = "msg_mcard_fileload_warnning";
const char str_msg_mcard_lord_finis_80302be4[] = "msg_mcard_lord_finish";
const char str_msg_mcard_nocard_80302bfc[] = "msg_mcard_nocard";
const char str_msg_mcard_different__80302c10[] = "msg_mcard_different_card";
const char str_scrl_auto_once_stop_80302c2c[] = "<scrl_auto><once_stop>";
const char str_msg_mcard_format_80302c44[] = "msg_mcard_format";
const char str_msg_mcard_format_all_80302c58[] = "msg_mcard_format_alldelete";
const char str_msg_out_mcard_warnni_80302c74[] = "msg_out_mcard_warnning";
const char str_msg_mcard_format_war_80302c8c[] = "msg_mcard_format_warning";
const char str_msg_mcard_format_mis_80302ca8[] = "msg_mcard_format_miss";
const char str_msg_mcard_format_fin_80302cc0[] = "msg_mcard_format_finish";
const char str_msg_mcard_make_data_80302cd8[] = "msg_mcard_make_data";
const char str_msg_mcard_yesno_80302cec[] = "msg_mcard_yesno";
const char str_msg_mcard_make_data__80302cfc[] = "msg_mcard_make_data_warning";
const char str_msg_mcard_make_data__80302d18[] = "msg_mcard_make_data_miss";
const char str_msg_mcard_make_data__80302d34[] = "msg_mcard_make_data_finish";
const char str_msg_mcard_nospace_80302d50[] = "msg_mcard_nospace";
const char str_msg_gc_mcard_open_80302d64[] = "msg_gc_mcard_open";
const char str_msg_mcard_different__80302d78[] = "msg_mcard_different_savedata_2";
const char str_msg_mcard_damage_80302d98[] = "msg_mcard_damage";
const char str_msg_savefile_start_80302dac[] = "msg_savefile_start";
const char str_msg_savefile_delete_80302dc0[] = "msg_savefile_delete";
const char str_msg_savefile_delete__80302dd4[] = "msg_savefile_delete_warnning";
const char str_msg_savefile_delete__80302df4[] = "msg_savefile_delete_copy_miss";
const char str_msg_savefile_delete__80302e14[] = "msg_savefile_delete_finish";
const char str_msg_savefile_delete__80302e30[] = "msg_savefile_delete_copy_point";
const char str_msg_savefile_copy_wa_80302e50[] = "msg_savefile_copy_warnning";
const char str_msg_savefile_copy_fi_80302e6c[] = "msg_savefile_copy_finish";
const char str_msg_mcard_save_selec_80302e88[] = "msg_mcard_save_select";
const char str_msg_mcard_yesno_endi_80302ea0[] = "msg_mcard_yesno_ending";
const char str_msg_savefile_first_w_80302eb8[] = "msg_savefile_first_warnning";
const char str_msg_mcard_save_warnn_80302ed4[] = "msg_mcard_save_warnning";
const char str_msg_mcard_save_finis_80302eec[] = "msg_mcard_save_finish";
const char str_msg_mcard_save_miss_80302f04[] = "msg_mcard_save_miss";
const char str_msg_mcard_different__80302f18[] = "msg_mcard_different_savedata";
const char str_msg_gc_mcard_nosave_80302f38[] = "msg_gc_mcard_nosave";
const char str_msg_savefile_nosave__80302f4c[] = "msg_savefile_nosave_warnning";
const char str_msg_mcard_yesno3_80302f6c[] = "msg_mcard_yesno3";

/*
 * Recovered evt_memcard event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 */
u32 evt_memcard_nosave[] = {
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_nosave_start_80302ba0, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x0001005B, (u32)evt_msg_continue, 0x00020018, 0xFE363C80,
    0x00000000, 0x00020032, 0xFAA2B580, 0x00000001, 0x00000021, 0x00000002,
    0x00000001,
};

u32 evt_memcard_check[] = {
    0x00020032, 0xFAA2B580, 0x00000000, 0x00020032, 0xFBD3E28A, 0x00000000,
    0x00010005, 0x00000000, 0x00020018, 0xFE363C8A, 0x00000000, 0x0005005B,
    (u32)evt_msg_print, 0x00000000, (u32)(char*)str_msg_mcard_fileload_w_80302bc8, 0x00000000, 0x00000000, 0x0001005B,
    (u32)memcard_load, 0x00000021, 0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020018,
    0xFE363C8A, 0x00000001, 0x0001005B, (u32)unk_8025c430, 0x00000021, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_lord_finis_80302be4, 0x00020032, 0xFAA2B580, 0x00000001, 0x00010024, 0xFFFFFFFD,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_nocard_80302bfc, 0x0001005E, (u32)evt_memcard_nosave,
    0x00010024, 0xFFFFFFFE, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_different__80302c10,
    0x0001005E, (u32)evt_memcard_nosave, 0x0001002B, 0xFFFFFFF3, 0x0001002B, 0xFFFFFFFA,
    0x0001005B, (u32)unk_8025c40c, 0x0003005B, (u32)evt_msg_print_add, 0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_80302c44, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_all_80302c58, 0x0003005B, (u32)evt_msg_select, 0x00000000,
    (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000, 0x0002005B, (u32)unk_8025c3a4,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x0001005E, (u32)evt_memcard_nosave, 0x00000030, 0x00000021,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_war_80302c8c, 0x0001005B, (u32)memcard_format,
    0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_mis_80302ca8, 0x00020032, 0xFBD3E28A,
    0x00000001, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_fin_80302cc0,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data_80302cd8, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec, 0x00020018, 0xFE363C80, 0x00000000, 0x0002005B,
    (u32)unk_8025c3a4, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x0001005E, (u32)evt_memcard_nosave, 0x00000030,
    0x00000021, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302cfc, 0x0001005B,
    (u32)memcard_create, 0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d18, 0x00020032,
    0xFBD3E28A, 0x00000001, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_make_data__80302d34, 0x0001005B, (u32)evt_msg_continue, 0x00020032, 0xFAA2B580, 0x00000001,
    0x00000021, 0x00000020, 0x0001005E, (u32)evt_memcard_nosave, 0x00000021, 0x00000021,
    0x00000020, 0x0001005E, (u32)evt_memcard_nosave, 0x00000021, 0x00000020, 0x0001005E,
    (u32)evt_memcard_nosave, 0x00000021, 0x0000002E, 0x00010024, 0xFFFFFFFC, 0x0001005B,
    (u32)unk_8025c40c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data_80302cd8, 0x0003005B,
    (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec, 0x00020018, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)unk_8025c3a4, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x0001005E, (u32)evt_memcard_nosave,
    0x00000030, 0x00000021, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302cfc,
    0x0001005B, (u32)memcard_create, 0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d18,
    0x00020032, 0xFBD3E28A, 0x00000001, 0x00000020, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d34, 0x0001005B, (u32)evt_msg_continue, 0x00020032, 0xFAA2B580,
    0x00000001, 0x00000021, 0x00000020, 0x0001005E, (u32)evt_memcard_nosave, 0x00000021,
    0x0001002B, 0xFFFFFFF8, 0x0001002B, 0xFFFFFFF7, 0x0003005B, (u32)evt_msg_print_add,
    0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_nospace_80302d50,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_gc_mcard_open_80302d64, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000, 0x0001005B,
    (u32)memcard_ipl, 0x00000020, 0x0001005E, (u32)evt_memcard_nosave, 0x00000021, 0x0000002E,
    0x00010024, 0xFFFFFFF6, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_different__80302d78,
    0x0001005E, (u32)evt_memcard_nosave, 0x0000002A, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_damage_80302d98, 0x0001005E, (u32)evt_memcard_nosave, 0x00000031, 0x0002005B, (u32)unk_8025c380,
    0xFE363C80, 0x00020018, 0xFBD3E28A, 0x00000001, 0x00020032, 0xFBD3E28A,
    0x00000000, 0x00020032, 0xFE363C8A, 0x00000001, 0x00000020, 0x00000007,
    0x00000021, 0x00000006, 0x00000002, 0x00000001,
};

u32 evt_memcard_start[] = {
    0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020035, 0xFE363C8A, 0x00000001,
    0x0006005B, (u32)evt_msg_print_insert, 0x00000000, (u32)(char*)str_msg_savefile_start_80302dac, 0x00000000, 0x00000000,
    0xFE363C8A, 0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec, 0x0001005B,
    (u32)evt_msg_continue, 0x00020018, 0xFE363C80, 0x00000000, 0x00020032, 0xFD050F80,
    0x00000001, 0x00000020, 0x00020032, 0xFD050F80, 0x00000000, 0x00000021,
    0x00000002, 0x00000001,
};

u32 evt_memcard_delete[] = {
    0x00020032, 0xFAA2B580, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80,
    0x00020035, 0xFE363C8A, 0x00000001, 0x0006005B, (u32)evt_msg_print_insert, 0x00000000,
    (u32)(char*)str_msg_savefile_delete_80302dc0, 0x00000000, 0x00000000, 0xFE363C8A, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_savefile_delete__80302dd4, 0x0002005B, (u32)memcard_delete, 0xFE363C8A,
    0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_savefile_delete__80302df4, 0x00000020, 0x0004005B,
    (u32)evt_msg_print_add_insert, 0x00000000, (u32)(char*)str_msg_savefile_delete__80302e14, 0xFE363C8A, 0x00000002, 0x00000021,
    0x00000020, 0x0001005B, (u32)evt_msg_continue, 0x00000002, 0x00000021, 0x0006005B,
    (u32)evt_fade_entry, 0x00000018, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0001005B, (u32)evt_fade_end_wait, 0x0006005B, (u32)evt_fade_entry, 0x0000001A, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0001005B,
    (u32)evt_msg_continue, 0x0001005B, (u32)unk_800f72e4, 0x00020032, 0xFAA2B580, 0x00000001,
    0x00000002, 0x00000001,
};

u32 evt_memcard_copy[] = {
    0x00020032, 0xFAA2B580, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80,
    0x00020032, 0xFE363C8B, 0xFE363C81, 0x00020035, 0xFE363C8A, 0x00000001,
    0x00020035, 0xFE363C8B, 0x00000001, 0x0003005B, (u32)memcard_file_existance, 0xFE363C8B,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0006005B, (u32)evt_msg_print_insert,
    0x00000000, (u32)(char*)str_msg_savefile_delete__80302e30, 0x00000000, 0x00000000, 0xFE363C8B, 0x0003005B,
    (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_savefile_copy_wa_80302e50, 0x0003005B, (u32)memcard_copy,
    0xFE363C8A, 0xFE363C8B, 0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_savefile_delete__80302df4,
    0x00000020, 0x0005005B, (u32)evt_msg_print_add_insert, 0x00000000, (u32)(char*)str_msg_savefile_copy_fi_80302e6c, 0xFE363C8A,
    0xFE363C8B, 0x00000002, 0x00000021, 0x00000020, 0x0001005B, (u32)evt_msg_continue,
    0x00000002, 0x00000021, 0x00000020, 0x0005005B, (u32)evt_msg_print, 0x00000000,
    (u32)(char*)str_msg_savefile_copy_wa_80302e50, 0x00000000, 0x00000000, 0x0003005B, (u32)memcard_copy, 0xFE363C8A,
    0xFE363C8B, 0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_savefile_delete__80302df4, 0x00000020,
    0x0005005B, (u32)evt_msg_print_add_insert, 0x00000000, (u32)(char*)str_msg_savefile_copy_fi_80302e6c, 0xFE363C8A, 0xFE363C8B,
    0x00000002, 0x00000021, 0x00000021, 0x0006005B, (u32)evt_fade_entry, 0x00000018,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait,
    0x0006005B, (u32)evt_fade_entry, 0x0000001A, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0001005B, (u32)evt_msg_continue, 0x0001005B,
    (u32)unk_800f72e4, 0x00020032, 0xFAA2B580, 0x00000001, 0x00000002, 0x00000001,
};

u32 evt_memcard_save[] = {
    0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020018, 0xFE363C8A, 0x00000017,
    0x00010004, 0x00000001, 0x00000021, 0x00010003, 0x00000000, 0x00020018,
    0xFBD3E28A, 0x00000001, 0x0003005B, (u32)evt_msg_print_add, 0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_selec_80302e88, 0x00020032, 0xFBD3E28A,
    0x00000000, 0x00000020, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_msg_mcard_save_selec_80302e88,
    0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFE363C8A, 0x00000015,
    0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno_endi_80302ea0, 0x00000020, 0x0003005B,
    (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec, 0x00000021, 0x00020019, 0xFE363C80,
    0x00000000, 0x0001005B, (u32)evt_msg_continue, 0x00000002, 0x00000021, 0x00010003,
    0x00000001, 0x00020032, 0xF5DE0184, 0xFE363C8A, 0x00020018, 0xFE363C8A,
    0x00000017, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_msg_savefile_first_w_80302eb8, 0x00000000,
    0x00000000, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_warnn_80302ed4,
    0x00000021, 0x00010004, 0x00000003, 0x00010003, 0x00000002, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_savefile_first_w_80302eb8, 0x00010003, 0x00000003, 0x0001005B, (u32)memcard_header_write, 0x0001005B,
    (u32)memcard_write, 0x00010003, 0x00000004, 0x00020032, 0xFBD3E28B, 0x00000000,
    0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_finis_80302eec, 0x00000002,
    0x00010024, 0xFFFFFFFD, 0x0003005B, (u32)evt_msg_print_add, 0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_nocard_80302bfc, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00010024, 0xFFFFFFFE, 0x0003005B, (u32)evt_msg_print_add,
    0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_different__80302c10,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x0001002B, 0xFFFFFFF3,
    0x0001002B, 0xFFFFFFFA, 0x0001005B, (u32)unk_8025c40c, 0x0003005B, (u32)evt_msg_print_add,
    0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_80302c44,
    0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_all_80302c58, 0x0003005B,
    (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020018, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)unk_8025c3a4, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x00000030, 0x00000021,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_war_80302c8c, 0x0001005B, (u32)memcard_format,
    0x0002005B, (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_mis_80302ca8, 0x00020032, 0xFBD3E28B,
    0x00000001, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_format_fin_80302cc0,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data_80302cd8, 0x0003005B, (u32)evt_msg_select,
    0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec, 0x00020018, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302cfc, 0x0002005B, (u32)unk_8025c3a4, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x00000030, 0x00000021, 0x0001005B, (u32)memcard_create, 0x0002005B,
    (u32)memcard_code, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d18, 0x00020032, 0xFBD3E28B, 0x00000001,
    0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d34, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_warnn_80302ed4, 0x0001005B, (u32)memcard_write, 0x0002005B,
    (u32)memcard_code, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_finis_80302eec, 0x00000002, 0x00000020, 0x00020032,
    0xFBD3E28B, 0x00000001, 0x00000021, 0x00000021, 0x00000020, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x00000021, 0x00000020,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x00000020,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x0000002E,
    0x00010024, 0xFFFFFFFC, 0x0001005B, (u32)unk_8025c40c, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_mcard_make_data_80302cd8, 0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno_80302cec,
    0x00020018, 0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_make_data__80302cfc, 0x0002005B, (u32)unk_8025c3a4, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_out_mcard_warnni_80302c74, 0x00000030,
    0x00000021, 0x0001005B, (u32)memcard_create, 0x0002005B, (u32)memcard_code, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_make_data__80302d18, 0x00020032, 0xFBD3E28B, 0x00000001, 0x00000020, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_make_data__80302d34, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_save_warnn_80302ed4, 0x0001005B, (u32)memcard_write, 0x0002005B, (u32)memcard_code, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_save_finis_80302eec, 0x00000002, 0x00000020, 0x00020032, 0xFBD3E28B, 0x00000001,
    0x00000021, 0x00000021, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x00010024, 0xFFFFFFF6, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_mcard_different__80302f18, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04,
    0x0001002B, 0xFFFFFFF8, 0x0001002B, 0xFFFFFFF7, 0x0003005B, (u32)evt_msg_print_add,
    0x00000001, (u32)(char*)str_scrl_auto_once_stop_80302c2c, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_nospace_80302d50,
    0x00020018, 0xFE363C8A, 0x00000017, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_gc_mcard_open_80302d64, 0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04,
    0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_gc_mcard_nosave_80302f38, 0x0003005B,
    (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno2_80302bb4, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000020, 0x0001005B,
    (u32)memcard_ipl, 0x00000021, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x00000020, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    (u32)(char*)str_msg_mcard_save_miss_80302f04, 0x00000021, 0x0000002E, 0x0000002A, 0x0003005B, (u32)evt_msg_print_add,
    0x00000000, (u32)(char*)str_msg_mcard_damage_80302d98, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, (u32)(char*)str_msg_mcard_save_miss_80302f04,
    0x00000031, 0x0001005B, (u32)unk_8025c380, 0x00020018, 0xFBD3E28B, 0x00000001,
    0x00010004, 0x00000004, 0x00000021, 0x00020018, 0xFE363C8A, 0x00000000,
    0x0001005B, (u32)evt_msg_continue, 0x00000002, 0x00000021, 0x0001005B, (u32)evt_msg_continue,
    0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_msg_savefile_nosave__80302f4c, 0x00000000, 0x00000000,
    0x00020018, 0xFE363C8A, 0x00000015, 0x0003005B, (u32)evt_msg_select, 0x00000000,
    (u32)(char*)str_msg_mcard_yesno_endi_80302ea0, 0x00000020, 0x0003005B, (u32)evt_msg_select, 0x00000000, (u32)(char*)str_msg_mcard_yesno3_80302f6c,
    0x00000021, 0x00020018, 0xFE363C80, 0x00000000, 0x00020018, 0xFE363C8A,
    0x00000017, 0x00010004, 0x00000002, 0x00000020, 0x00010004, 0x00000001,
    0x00000021, 0x00000021, 0x0001005B, (u32)evt_msg_continue, 0x00000002, 0x00000001,
};

u32 unk_evt_803bac3c[] = {
    0x00020018, 0xFE363C80, 0x00000000, 0x0001005E, (u32)evt_memcard_save, 0x00000020,
    0x0001005E, (u32)evt_memcard_save, 0x0001000A, 0x000003E8, 0x00000021, 0x00000002,
    0x00000001,
};
