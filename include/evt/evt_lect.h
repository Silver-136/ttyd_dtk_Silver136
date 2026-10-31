// Types shared by the per-area lecture (tutorial) scripts, rels/<area>/src/event/evt_lect_<area>.c.
// Each copy keeps its own two on-screen button icons, `static LectButton lbtn[2]`.

#pragma once

// Button prompts of the per-REL lecture events (evt_lect_<rel>.c): every map REL with a tutorial carries the same
// two-button display.

#include <dolphin/types.h>
#include <dolphin/mtx.h>

typedef struct LectButtonDisp {
    s32 unk0;
    Vec pos;
} LectButtonDisp;

typedef struct LectButton {
    u8 flags;
    u8 seq;
    u8 pad_2[0x8 - 0x2];
    LectButtonDisp disp;
    u8 pad_18[0x40 - 0x18];
    s16 iconId;
    s16 alpha;
} LectButton;
