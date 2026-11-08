// Piders / Churantalar (the Pider and Arantula spiders): the thread drawn from the ceiling to the spider, shared by every copy
// of the units (jon, mri, tou2 and tst).

#pragma once
#include <dolphin/types.h>

typedef struct PidersYarn {
    f32 x0;
    f32 y0;
    f32 z0;
    f32 x1;
    f32 y1;
    f32 z1;
    u8 alpha;
} PidersYarn;
