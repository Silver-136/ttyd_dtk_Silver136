// dispdrv.c: the display driver (DispEntry, dispEntry/dispDraw): per-camera draw callbacks sorted by depth.

#pragma once

#include <dolphin/types.h>
#include "driver/camdrv.h"

typedef void (*DispCallback)(CameraId cameraId, void* param);

typedef struct DispEntry {
    u8 cameraId;              // 0x0, CameraId
    u8 renderMode;         // 0x1
    f32 order;             // 0x4
    DispCallback callback; // 0x8
    void* param;           // 0xC
} DispEntry;

// dispEntry renderMode: picks the GX setup (dispDraw) and the depth band (offset_tbl; dispSort draws ascending, the
// value in the comment). The CUSTOM modes get no GX setup; their callbacks do their own and draw after the 3D scene.
typedef enum DispMode {
    DISP_MODE_OPAQUE = 0,             // 400000
    DISP_MODE_ALPHA_TEST = 1,         // 400000
    DISP_MODE_XLU = 2,                // 600000, blended, far to near (as all XLU modes)
    DISP_MODE_OPAQUE_NO_Z = 3,        // 100000
    DISP_MODE_ALPHA_TEST_NO_Z = 4,    // 100000
    DISP_MODE_XLU_BEFORE_OPAQUE = 5,  // 300000
    DISP_MODE_CUSTOM_FIRST = 6,       // 900000
    DISP_MODE_CUSTOM_LAST = 7,        // 1100000
    DISP_MODE_CUSTOM_MID = 8,         // 1000000
    DISP_MODE_XLU_BEFORE = 9,         // 500000
    DISP_MODE_XLU_AFTER = 10,         // 700000, not in the US demo
} DispMode;

void dispInit(void);
void dispReInit(void);
void dispEntry(CameraId camera, f32 order, s32 renderMode, DispCallback callback, void* param);
void dispSort(void);
void dispDraw(CameraId cameraId);
f32 dispCalcZ(Vec* input);
DispEntry* dispGetCurWork(void);
