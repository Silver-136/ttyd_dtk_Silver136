// Train wheel animation tables ("sharin", wheel; "clank", crank rods) read by the map scripts of the
// train scenes (gor_04, hom_10, pik_00, rsh_06) with READ_INT4/READ_INT. A NULL name ends a table.

#pragma once
#include "data_tables.h"
#include <dolphin/types.h>

typedef struct ClankData {
    const char* name;
    s32 radiusType;
    s32 driveSource;
    EvtFloat radius;    // circle radius (scales the sin/cos offset)
    EvtFloat direction; // +1 / -1, multiplies the angle
} ClankData;

typedef struct SharinData {
    const char* name;
    s32 radiusType;
    s32 driveSource;
    EvtFloat speed;
} SharinData;

