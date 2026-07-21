#include "driver/dispdrv.h"

typedef void (*DispCallback)(s32 cameraId, void* param);
typedef struct DispWork {
    u8 cameraId;          // 0x00
    u8 drawMode;          // 0x01
    u8 pad_02[0x02];      // 0x02
    f32 z;                // 0x04
    DispCallback callback; // 0x08
    void* param;          // 0x0C
} DispWork;

typedef struct DispVec {
    f32 x;
    f32 y;
    f32 z;
} DispVec;
extern void* __memAlloc(s32 heap, u32 size);
extern void* memset(void* dst, int value, unsigned long size);
extern DispWork** pSortWork;
extern s32 entry_n;
extern void* currentWorkPtr;
extern DispWork* pDispWork;
extern const f32 offset_tbl[11];
extern void* camGetPtr(s32 cameraId);
extern void PSMTXMultVec(void* mtx, void* src, void* dst);
extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
extern void qqsort(void* base, u32 count, u32 size, void* compare);
extern s32 _sort(const void* a, const void* b);
extern const f32 float_5000_8041f760;
extern const f32 float_0_8041f764;
extern const f32 float_10000_8041f768;
extern const f32 float_neg1_8041f76c;
extern void GXSetColorUpdate(s32 enable);
extern void GXSetAlphaUpdate(s32 enable);
extern void GXSetZScaleOffset(f32 scale, f32 offset);
extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
extern void GXSetZCompLoc(s32 beforeTex);
extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
extern const f32 float_1_8041f770;

void dispInit(void) {
    pDispWork = __memAlloc(0, 0x4000);
    pSortWork = __memAlloc(0, 0x1000);

    memset(pDispWork, 0, 0x4000);
    memset(pSortWork, 0, 0x1000);

    entry_n = 0;
}

void dispReInit(void) {
    entry_n = 0;
}

void dispEntry(s32 cameraId, s32 mode, void* callback, void* param, f32 z) {
    DispWork* work;

    work = (DispWork*)((s32)pDispWork + (entry_n << 4));

    if (z != z) {
        return;
    }

    work->cameraId = cameraId;
    work->drawMode = mode;
    work->callback = callback;
    work->param = param;

    if ((mode == 5) || (mode == 2) || ((mode >= 9) && (mode < 11))) {
        work->z = z + offset_tbl[mode];
    } else {
        work->z = offset_tbl[mode] - z;
    }

    pSortWork[entry_n] = work;
    entry_n++;
}

s32 _sort(const void* a, const void* b) {
    DispWork* workA;
    DispWork* workB;

    workA = *(DispWork**)a;
    workB = *(DispWork**)b;

    if (workA->z > workB->z) {
        return 1;
    }

    if (workA->z < workB->z) {
        return -1;
    }

    return 0;
}

void dispSort(void) {
    qqsort(pSortWork, entry_n, 4, _sort);
}

void dispDraw(s32 cameraId) {
    s32 offset;
    s32 i;
    DispWork* work;

    for (offset = 0, i = 0; i < entry_n; offset += 4, i++) {
        work = *(DispWork**)((s32)pSortWork + offset);

        if (work->cameraId == cameraId) {
            GXSetColorUpdate(1);

            if ((cameraId >= 4) && (cameraId < 8) && (cameraId != 6)) {
                GXSetAlphaUpdate(0);
            } else {
                GXSetAlphaUpdate(1);
            }

            GXSetZScaleOffset(float_1_8041f770, float_0_8041f764);

            switch (work->drawMode) {
                case 0:
                    GXSetBlendMode(0, 1, 0, 7);
                    GXSetZCompLoc(1);
                    GXSetAlphaCompare(7, 0, 0, 7, 0);
                    GXSetZMode(1, 3, 1);
                    break;

                case 1:
                    GXSetBlendMode(0, 1, 0, 0);
                    GXSetZCompLoc(1);
                    GXSetAlphaCompare(7, 0, 0, 7, 0);
                    GXSetZMode(0, 3, 0);
                    break;

                case 2:
                    GXSetBlendMode(0, 1, 0, 0);
                    GXSetZCompLoc(0);
                    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                    GXSetZMode(1, 3, 1);
                    break;

                case 4:
                    GXSetBlendMode(0, 1, 0, 0);
                    GXSetZCompLoc(0);
                    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                    GXSetZMode(0, 3, 0);
                    break;

                case 3:
                    GXSetBlendMode(1, 4, 5, 0);
                    GXSetZCompLoc(1);
                    GXSetAlphaCompare(7, 0, 0, 7, 0);
                    GXSetZMode(1, 3, 0);
                    break;

                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                default:
                    break;
            }

            currentWorkPtr = work;
            work->callback(cameraId, work->param);
        }
    }
}

f32 dispCalcZ(void* pos) {
    void* cam;
    DispVec work;
    f32 z;

    cam = camGetPtr(4);

    PSMTXMultVec(
        (void*)((s32)cam + 0x11C),
        pos,
        &work
    );

    PSMTX44MultVec(
        (void*)((s32)cam + 0x15C),
        &work,
        &work
    );

    z = (work.z * float_5000_8041f760) + float_5000_8041f760;

    if (z < float_0_8041f764) {
        z = float_0_8041f764;
    } else if (z > float_10000_8041f768) {
        z = float_10000_8041f768;
    }

    return z * float_neg1_8041f76c;
}

void* dispGetCurWork(void) {
    return currentWorkPtr;
}
