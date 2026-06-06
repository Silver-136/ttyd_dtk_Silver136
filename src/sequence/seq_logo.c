#include "sequence/seq_logo.h"


u8 logoDisp(void) {
    return 0;
}


u8 seq_logoMain(int param_1) {
    return 0;
}


u8 progDisp(int param_1) {
    return 0;
}


u8 seq_logoInit(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void dummy_draw(void) {
    extern s32 OSGetProgressiveMode(void);
    extern u16 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
    extern u32 GXSetDispCopyYScale(u16 yScale);
    extern void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht);
    extern void GXSetDispCopyDst(u16 wd, u16 ht);
    extern void GXSetPixelFmt(s32 pix_fmt, s32 z_fmt);
    extern void GXSetScissor(u32 x, u32 y, u32 wd, u32 ht);
    extern void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
    extern u8 GXNtsc480ProgMarioSt[];
    extern u8 GXNtsc480IntDfMarioSt[];
    extern f32 float_0_80420824;
    extern f32 float_608_80420828;
    extern f32 float_480_8042082c;
    extern f32 float_1_80420818;
    void* mode;
    u16 y;

    if (OSGetProgressiveMode() == 1) {
        mode = GXNtsc480ProgMarioSt;
    } else {
        mode = GXNtsc480IntDfMarioSt;
    }
    y = GXSetDispCopyYScale(GXGetYScaleFactor(*(u16*)((s32)mode + 6), *(u16*)((s32)mode + 8)));
    GXSetDispCopySrc(0, 0, *(u16*)((s32)mode + 4), *(u16*)((s32)mode + 6));
    GXSetDispCopyDst(*(u16*)((s32)mode + 4), y);
    GXSetPixelFmt(1, 0);
    GXSetScissor(0, 0, 0x260, 0x1E0);
    GXSetViewport(float_0_80420824, float_0_80420824, float_608_80420828, float_480_8042082c, float_0_80420824, float_1_80420818);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 seq_logoExit(void) {
    return 0;
}
