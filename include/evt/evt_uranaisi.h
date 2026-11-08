// evt_uranaisi.c: the fortune teller (uranaisi) script commands: fortune tables and message labels by story progress.

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

s32 uranaisi_ryokin_rtn(s32 mode);

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
