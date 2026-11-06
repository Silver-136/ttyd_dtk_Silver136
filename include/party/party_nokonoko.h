// party_nokonoko.c: Koops's (nokonoko) field partner functions (init, move, use, finish) and the shell hold cancel.

#pragma once

#include "dolphin/types.h"
#include "party.h"

// nokonokoGetStatus: where Koops is in his Shell Toss, from his use-motion state
typedef enum NokonokoStatus {
    NOKONOKO_STATUS_NONE = 0,           // not in use, or not Koops
    NOKONOKO_STATUS_PULL_IN = 1,        // pulling into his shell beside Mario (use motions 0-2)
    NOKONOKO_STATUS_SHELL_OUT = 2,      // shell flying out (10-13)
    NOKONOKO_STATUS_SHELL_HELD = 3,     // shell held spinning in place (20-21)
    NOKONOKO_STATUS_SHELL_BACK = 4,     // shell coming back (50-71)
    NOKONOKO_STATUS_HELD_SHOT_BACK = 5, // coming back after a held shot (80-100)
    NOKONOKO_STATUS_FROM_ABOVE = 6,     // dropping back in from above (110-114)
    NOKONOKO_STATUS_OTHER = 7,
} NokonokoStatus;

s32 nokonokoGetStatus(PartyEntry* party); // NokonokoStatus
void nokonoko_bye(PartyEntry* party);
void nokonoko_finish(PartyEntry* party);
void nokonoko_init(PartyEntry* party);
void nokonoko_move(PartyEntry* party);
void nokonoko_use(PartyEntry* party);
void nokonoko_use_post(PartyEntry* party);
void nokotaro_hold_cancel(PartyEntry* party);
BOOL nokonoko_holdItem(void);
void nokonoko_use_cancel(void);
