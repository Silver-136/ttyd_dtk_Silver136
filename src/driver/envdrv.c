#include "driver/envdrv.h"

extern void* gp;
extern void* mapalloc_base_ptr;
u8 work[0x1E0];
void* env_tpl;
s32 tpl_ok;

void* memset(void* dst, int value, u32 size);
void PSMTXCopy(void* src, void* dst);
void UnpackTexPalette(void* tpl);
void DVDMgrClose(void* entry);
void _mapFree(void* heap, void* ptr);
void GXGetViewportv(f32* vp);
void GXGetProjectionv(f32* proj);
void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
void GXSetProjectionv(f32* proj);
void envDisp_DepthOfField(void);
void envDisp_Blur(void);
void envDisp_FF(void);

static void* env_current_work(void) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    return ptr;
}

void _callback(void* unused, void* entry) {
    void* closeEntry = *(void**)((s32)entry + 0x2C);
    UnpackTexPalette(env_tpl);
    DVDMgrClose(closeEntry);
    tpl_ok = 1;
}

void envInit(void) {
    memset(work, 0, 0xF0);
    memset(work + 0xF0, 0, 0xF0);
    env_tpl = 0;
    tpl_ok = 0;
}

void envDisp(void) {
    f32 proj[7];
    f32 vp[6];
    GXGetViewportv(vp);
    GXGetProjectionv(proj);
    envDisp_DepthOfField();
    envDisp_Blur();
    envDisp_FF();
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
}

void envSetYamiView(void* mtx) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    PSMTXCopy(mtx, (void*)((s32)ptr + 0xC0));
}

void envGlareFilterZ(f32 z) {
    void* entry = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    entry = (void*)((s32)entry + 0xB0);
    *(f32*)((s32)entry + 0xC) = z;
    *(u16*)entry |= 1;
}

void envReInit(void) {
    void* ptr = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        ptr = (void*)((s32)ptr + 0xF0);
    }
    memset(ptr, 0, 0xF0);
}

void envBlurOff(void) {
    void* entry = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        entry = (void*)((s32)entry + 0xF0);
    }
    *(u32*)entry &= ~4;
    *(u32*)entry &= ~0x40000000;
    _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x4C));
}

