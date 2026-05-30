#include "driver/extdrv.h"

typedef struct ExtWork {
    s32 poseNum;
    f32 unk4;
    void* posePtr;
    u8 padC[0x30];
    f32 mtx[3][4];
    u32 pad6C;
} ExtWork;

typedef struct ExtGp {
    u8 pad0[0x14];
    s32 useAlt;
} ExtGp;

ExtWork work[2];
extern ExtGp* gp;

void GXTexModeSync(void);
void PSMTXCopy(void* dst, void* src);

void extLoadTextureExit(void) {
    GXTexModeSync();
}

s32 extGetPoseNum(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(s32*)wp;
}

void* extGetPosePtr(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    return *(void**)(wp + 8);
}

void extInit(void) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    *(s32*)wp = 0;
    *(void**)(wp + 8) = 0;
}

s32 compare(void* a, void* b) {
    if (*(f32*)((s32)a + 4) > *(f32*)((s32)b + 4)) {
        return 1;
    }
    if (*(f32*)((s32)a + 4) < *(f32*)((s32)b + 4)) {
        return -1;
    }
    return 0;
}

void extLoadShadowMtx(void* dst) {
    s32 wp = (s32)work;

    if (gp->useAlt != 0) {
        wp += 0x70;
    }
    PSMTXCopy(dst, (void*)(wp + 0x3C));
}


u8 extMain(void) {
    return 0;
}


u8 extMakeTexture(void) {
    return 0;
}


u8 extEntry(int param_1, int* param_2, void* initFunc, int param_4, int param_5) {
    return 0;
}


u8 extDrawShadow(void) {
    return 0;
}


u8 extLoadShadowTev(void) {
    return 0;
}


u8 extLoadTexture(void) {
    return 0;
}


u8 extPoseDraw(int param_1) {
    return 0;
}


u8 extLoadTev(void) {
    return 0;
}


u8 extDraw(void) {
    return 0;
}


void extReset(void) {
}


u8 extLoadVertex(void) {
    return 0;
}


u8 extLoadShadowVertex(void) {
    return 0;
}


u8 extLoadShadowRenderMode(void) {
    return 0;
}


u8 extLoadRenderMode(void) {
    return 0;
}
