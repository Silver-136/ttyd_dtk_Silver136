/* "dispdrv" - Display Driver
 * dispEntry queues a draw callback per camera with a sort key (render mode offset +/- order), dispSort orders the
 * queue and dispDraw calls the callbacks of one camera after setting each render mode's blend, alpha and Z state.
 */

#include "version.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "driver/fadedrv.h"

#define DISP_MAX_ENTRIES 1024

#if VERSION >= VERSION_US
// US and later read their 1.0f from a fadedrv global (the name float_1_8041f770 is a label, the original name is unknown)
#define DISP_ONE float_1_8041f770
#else
#define DISP_ONE 1.0f
#endif

//.sdata
#if VERSION <= VERSION_DEMO_JP
f32 offset_tbl[] = {
#else
static f32 offset_tbl[] = {
#endif

    400000.0f, 400000.0f, 600000.0f, 100000.0f, 100000.0f,
    300000.0f, 900000.0f, 1100000.0f, 1000000.0f, 500000.0f,
#if VERSION >= VERSION_JP
    700000.0f // render mode 10
#endif
};

//.sbss
static DispEntry* currentWorkPtr;
static s32 entry_n;
static DispEntry** pSortWork;
static DispEntry* pDispWork;

// local prototypes
static s32 _sort(const void* sortA, const void* sortB);

/** Allocates the display list of 1024 draw requests and its sort table at boot. */
void dispInit(void) {
    pDispWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry) * DISP_MAX_ENTRIES);
    pSortWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry*) * DISP_MAX_ENTRIES);
    memset(pDispWork, 0, sizeof(DispEntry) * DISP_MAX_ENTRIES);
    memset(pSortWork, 0, sizeof(DispEntry*) * DISP_MAX_ENTRIES);
    entry_n = 0;
}

/** Empties the display list at the start of each frame. */
void dispReInit(void) {
    entry_n = 0;
}

/**
 * Queues a draw callback for camera camId this frame.
 * renderMode picks the blend, alpha test and depth setup (0 opaque, 1 alpha-tested, 2/5 translucent,
 * 3/4 no depth) and a base depth from offset_tbl;
 * order shifts it within that band (translucent modes draw far to near, the rest near to far).
 * NaN orders are dropped. In JP and later, modes 9 and 10 are translucent too (mode 10 is new there).
 */
void dispEntry(CameraId camId, f32 order, s32 renderMode, DispCallback callback, void* param) {
    DispEntry* entry = &pDispWork[entry_n];

    if (order != order) return;
    entry->cameraId = camId;
    entry->renderMode = (u8)renderMode;
    entry->callback = callback;
    entry->param = param;

    switch (renderMode) {
        case DISP_MODE_XLU:
        case DISP_MODE_XLU_BEFORE_OPAQUE:
#if VERSION >= VERSION_JP
        case DISP_MODE_XLU_BEFORE:
        case DISP_MODE_XLU_AFTER:
#endif
            entry->order = offset_tbl[renderMode] + order;
            break;
        default:
            entry->order = offset_tbl[renderMode] - order;
            break;
    }

    pSortWork[entry_n] = entry;
    entry_n++;
}

/** qsort comparator for draw requests: ascending by their combined order. */
static s32 _sort(const void* sortA, const void* sortB) {
    f32 orderB;
    f32 orderA;

    orderA = (*(DispEntry* const*)sortA)->order;
    orderB = (*(DispEntry* const*)sortB)->order;
    if (orderA > orderB) {
        return 1;
    }
    if (orderA < orderB) {
        return -1;
    }
    return 0;
}

/** Sorts this frame's draw requests by order before the cameras are drawn. */
void dispSort(void) {
    qqsort(pSortWork, entry_n, sizeof(DispEntry*), _sort);
}

/**
 * Calls every queued draw callback for camera camId in sorted order, first setting the GX blend,
 * alpha compare and depth state its render mode asks for.
 * Colour updates are always on; alpha updates are off for cameras 4, 5 and 7.
 */
void dispDraw(CameraId camId) {
    s32 i;

    for (i = 0; i < entry_n; i++) {
        DispEntry* entry = pSortWork[i];
        if (entry->cameraId == camId) {
            GXSetColorUpdate(GX_TRUE);
            switch (camId) {
                case CAMERA_3D:
                case CAMERA_3D_EFFECTA:
                case CAMERA_3D_EFFECTB:
                    GXSetAlphaUpdate(GX_FALSE);
                    break;
                default:
                case CAMERA_3D_IMAGE:
                    GXSetAlphaUpdate(GX_TRUE);
                    break;
            }
            GXSetZScaleOffset(DISP_ONE, 0.0f);
            switch (entry->renderMode) {
                case DISP_MODE_OPAQUE:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
                    GXSetZCompLoc(GX_TRUE);
                    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                    break;
                case DISP_MODE_OPAQUE_NO_Z:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_TRUE);
                    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                    break;
                case DISP_MODE_ALPHA_TEST:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_FALSE);
                    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                    break;
                case DISP_MODE_ALPHA_TEST_NO_Z:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_FALSE);
                    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                    break;
                case DISP_MODE_XLU:
                case DISP_MODE_XLU_BEFORE_OPAQUE:
#if VERSION >= VERSION_JP
                case DISP_MODE_XLU_BEFORE:
                case DISP_MODE_XLU_AFTER:
#endif
                    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_TRUE);
                    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
                    break;
            }
            currentWorkPtr = entry;
            entry->callback(camId, entry->param);
        }
    }
}

/**
 * Turns a world position into a dispEntry order from its projected depth under the 3D camera,
 * scaled to 0..10000 (clamped) and negated, so callers can sort objects by distance.
 */
f32 dispCalcZ(Vec* input) {
    CameraEntry* camera;
    Vec output;
    f32 calc;

    camera = camGetPtr(CAMERA_3D);
    MTXMultVec(camera->view, input, &output);
    MTX44MultVec(camera->projection, &output, &output);
    calc = (5000.0f * output.z) + 5000.0f;
    if (calc < 0.0f) {
        calc = 0.0f;
    } else if (calc > 10000.0f) {
        calc = 10000.0f;
    }
    calc *= -1.0f;
    return calc;
}

/** Returns the draw request whose callback is running now, so a callback can read its own render mode. */
DispEntry* dispGetCurWork(void) {
    return currentWorkPtr;
}

// Padding object: 28 zero bytes the JP and later .sdata holds after offset_tbl; no source form for them is known
// (layout debt, docs/CLEANUP_BACKLOG.md)
#if VERSION >= VERSION_JP
#pragma force_active on
u32 gap_07_8040F344_sdata[7] = {0, 0, 0, 0, 0, 0, 0};
#pragma force_active reset
#endif
