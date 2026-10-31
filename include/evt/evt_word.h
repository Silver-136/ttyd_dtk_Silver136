// EvtWord: the type of one script word, which holds a value or a pointer to a string, script or data table.

#pragma once

#include <dolphin/types.h>

/* One script word: scripts store values and pointers (strings, other
 * scripts, data tables) in their words and variables. */
typedef s32 EvtWord;
