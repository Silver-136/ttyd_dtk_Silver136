#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord evt_memcard_save[];

extern EvtWord evt_memcard_check[];
extern EvtWord evt_memcard_copy[];
extern EvtWord evt_memcard_delete[];
#ifdef VERSION_G8ME01
extern EvtWord evt_memcard_save_wrapper[];
#endif
extern EvtWord evt_memcard_start[];
USER_FUNC(memcard_buf_reset);
USER_FUNC(memcard_code);
USER_FUNC(memcard_copy);
USER_FUNC(memcard_create);
USER_FUNC(memcard_delete);
USER_FUNC(memcard_file_existance);
USER_FUNC(memcard_format);
USER_FUNC(memcard_header_write);
USER_FUNC(memcard_ipl);
USER_FUNC(memcard_load);
USER_FUNC(memcard_watch_chk);
USER_FUNC(memcard_watch_end);
USER_FUNC(memcard_watch_start);
USER_FUNC(memcard_write);
