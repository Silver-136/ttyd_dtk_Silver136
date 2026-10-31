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

