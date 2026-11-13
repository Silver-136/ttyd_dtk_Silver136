// evt_uranaisi.c: the fortune teller (uranaisi) script commands: fortune tables and message labels by story progress.

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

s32 uranaisi_ryokin_rtn(s32 mode);

// One fortune-table row: a flag name as written in the fortune messages and its flag id.
typedef struct {
    const char* name;
    s32 id;
} UranaiEntry;

s32 search_moji_no(UranaiEntry* table, const char* name);

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
