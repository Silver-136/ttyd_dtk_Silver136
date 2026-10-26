#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

// First argument of evt_msg_print / _add / _insert / _add_insert (_evt_msg_print)
typedef enum MsgPrintFlags {
    MSG_PRINT_DIRECT_TEXT = (1 << 0), // the argument is the text itself, not a message key (window tag "DIRECT")
    MSG_PRINT_NO_SPEAKER = (1 << 1),  // no speaker: the window takes no position or name from an NPC
} MsgPrintFlags;

// evt_msg_toge: how the next message window's tail is placed (event->printWindowFlags low bits)
typedef enum MsgTogeMode {
    MSG_TOGE_AUTO = 0,       // toward the speaker, chosen from where it is on screen
    MSG_TOGE_NONE = 1,       // no tail
    MSG_TOGE_AT_POS = 2,     // at the given point, laid out as for a speaker to Mario's right
    MSG_TOGE_AT_POS_FAR = 3, // at the given point, laid out as for a speaker far off to the side
} MsgTogeMode;

void evt_msg_init(void);

USER_FUNC(evt_msg_close);
USER_FUNC(evt_msg_continue);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_print_battle_party);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_repeat);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_msg_toge);

extern char msg_article_buf[0xA00];
extern char msg_count_buf[0xA00];
extern char msg_num_buf[0xA00];
extern char msg_plural_buf[0xA00];
void evtMsgFmtInt(char* buf, s32 count);
void evtMsgFmtStr(char* buf, const char* name);
USER_FUNC(evt_msg_fmt_int);
USER_FUNC(evt_msg_fmt_str);
USER_FUNC(evt_msg_npc);
USER_FUNC(evt_msg_pri);
USER_FUNC(evt_msg_print_add_insert);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_msg_print_party_add);
USER_FUNC(evt_msg_togelr);
