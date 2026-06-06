#include "effect/eff_splash.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSplashGetCol(void* unused, void* entry) {
    extern void* camGetPtr(void);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void GXGetProjectionv(void* proj);
    extern void GXGetViewportv(void* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* model, void* proj, void* viewport, f32* sx, f32* sy, f32* sz);
    extern void sysWaitDrawSync(void);
    extern void GXPeekARGB(s32 x, s32 y, void* color);
    extern u32 unk_802294a4(void);
    extern f32 float_0_80427868;
    extern f32 float_608_804278cc;
    extern f32 float_480_804278d0;

    void* work = *(void**)((s32)entry + 0xC);
    void* cam = camGetPtr();
    f32 proj[7];
    f32 viewport[6];
    f32 sx;
    f32 sy;
    f32 sz;
    u32 color;
    u32 fallback;

    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject(*(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC),
              (void*)((s32)cam + 0x11C), proj, viewport, &sx, &sy, &sz);
    if (sx > float_0_80427868 && sx < float_608_804278cc &&
        sy > float_0_80427868 && sy < float_480_804278d0) {
        sysWaitDrawSync();
        GXPeekARGB((s32)sx, (s32)sy, &color);
        *(u8*)((s32)work + 0x3C) = ((u8*)&color)[1];
        *(u8*)((s32)work + 0x3D) = ((u8*)&color)[2];
        *(u8*)((s32)work + 0x3E) = ((u8*)&color)[3];
        *(u8*)((s32)work + 0x3F) = 0xFF;
    }
    *(s32*)((s32)work + 0x38) = 1;
    fallback = unk_802294a4();
    *(u8*)((s32)work + 0x3C) = ((u8*)&fallback)[0];
    *(u8*)((s32)work + 0x3D) = ((u8*)&fallback)[1];
    *(u8*)((s32)work + 0x3E) = ((u8*)&fallback)[2];
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

