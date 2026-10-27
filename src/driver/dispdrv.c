/* "dispdrv" - Display Driver
 * dispEntry queues a draw callback per camera with a sort key (render mode offset +/- order), dispSort orders the
 * queue and dispDraw calls the callbacks of one camera after setting each render mode's blend, alpha and Z state.
 */

#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "driver/fadedrv.h"

#define DISP_MAX_ENTRIES 1024

#ifdef VERSION_G8ME01
// US reads its 1.0f from a fadedrv global (the name float_1_8041f770 is a label, the original name is unknown)
#define DISP_ONE float_1_8041f770
#else
#define DISP_ONE 1.0f
#endif

//.sdata
static f32 offset_tbl[] = {
    400000.0f, 400000.0f, 600000.0f, 100000.0f, 100000.0f,
    300000.0f, 900000.0f, 1100000.0f, 1000000.0f, 500000.0f,
    700000.0f // render mode 10
};

//.sbss
DispEntry* currentWorkPtr;
s32 entry_n;
DispEntry** pSortWork;
DispEntry* pDispWork;

//local prototypes
static s32 _sort(const void** sortA, const void** sortB);

void dispInit(void) {
    pDispWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry) * DISP_MAX_ENTRIES);
    pSortWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry*) * DISP_MAX_ENTRIES);
    memset(pDispWork, 0, sizeof(DispEntry) * DISP_MAX_ENTRIES);
    memset(pSortWork, 0, sizeof(DispEntry*) * DISP_MAX_ENTRIES);
    entry_n = 0;
}

void dispReInit(void) {
    entry_n = 0;
}

void dispEntry(CameraId camId, f32 order, s32 renderMode, DispCallback callback, void* param) {
    DispEntry* entry = &pDispWork[entry_n];

    if (order != order) return;
    entry->cameraId = camId;
    entry->renderMode = (u8)renderMode;
    entry->callback = callback;
    entry->param = param;

    switch (renderMode) {
        case 2:
        case 5:
        case 9:
        case 10:
            entry->order = offset_tbl[renderMode] + order;
            break;
        default:
            entry->order = offset_tbl[renderMode] - order;
            break;
    }

    pSortWork[entry_n] = entry;
    entry_n++;
}

static s32 _sort(const void** sortA, const void** sortB) {
    f32 orderB;
    f32 orderA;

    orderA = (*(DispEntry**)sortA)->order;
    orderB = (*(DispEntry**)sortB)->order;
    if (orderA > orderB) {
        return 1;
    }
    if (orderA < orderB) {
        return -1;
    }
    return 0;
}

void dispSort(void) {
    qqsort(pSortWork, entry_n, sizeof(DispEntry*), _sort);
}

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
                case 0:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
                    GXSetZCompLoc(GX_TRUE);
                    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                    break;
                case 3:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_TRUE);
                    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                    break;
                case 1:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_FALSE);
                    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                    break;
                case 4:
                    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
                    GXSetZCompLoc(GX_FALSE);
                    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                    break;
                case 2:
                case 5:
                case 9:
                case 10:
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

DispEntry* dispGetCurWork(void) {
    return currentWorkPtr;
}
// Padding object: 28 zero bytes the retail .sdata holds after offset_tbl; no source form for them is known
// (layout debt, docs/CLEANUP_BACKLOG.md)
#pragma force_active on
u32 gap_07_8040F344_sdata[7] = {0, 0, 0, 0, 0, 0, 0};
#pragma force_active reset
