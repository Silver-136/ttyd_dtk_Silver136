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
    extern void* memset(void* dst, s32 value, u32 size);
    extern char* getMarioStDvdRoot(void);
    extern void* fileAllocf(s32 heap, const char* fmt, ...);
    extern u32 OSGetResetCode(void);
    extern u32 U_VIGetDTVStatus(void);
    extern u32 OSGetProgressiveMode(void);
    extern void OSSetProgressiveMode(s32 progressive);
    extern u32 keyGetButton(s32 controller);
    extern void VIConfigure(void* mode);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void* wp;
    extern void* gp;
    extern const char str_PCTs_mariost_tpl_802c3b68[];
    extern u8 sRMObjHReso[];
    extern u8 sRMObjHReso_prog[];

    memset(wp, 0, 0x20);
    *(s32*)((s32)wp + 4) = -1;
    *(void**)wp = fileAllocf(4, str_PCTs_mariost_tpl_802c3b68, getMarioStDvdRoot());

    if ((OSGetResetCode() >> 31) == 0 || (OSGetResetCode() & 2) != 0) {
        if (U_VIGetDTVStatus() != 0) {
            if (OSGetProgressiveMode() == 1) {
                *(s32*)((s32)wp + 0x18) = 1;
            } else if ((keyGetButton(0) & 0x200) != 0) {
                *(s32*)((s32)wp + 0x18) = 1;
            } else {
                *(s32*)((s32)wp + 0x18) = 0;
            }
        } else {
            if (OSGetProgressiveMode() == 1) {
                OSSetProgressiveMode(0);
            }
            *(s32*)((s32)wp + 0x18) = 0;
        }
    }

    if ((OSGetResetCode() >> 31) == 0 || (OSGetResetCode() & 2) != 0) {
        VIConfigure(sRMObjHReso);
        VIFlush();
        VIWaitForRetrace();
    } else {
        switch (OSGetResetCode() & 1) {
            case 0:
                VIConfigure(sRMObjHReso);
                VIFlush();
                VIWaitForRetrace();
                break;
            case 1:
                VIConfigure(sRMObjHReso_prog);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                *(u32*)gp |= 8;
                break;
        }
        *(u32*)gp |= 4;
    }
}

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

void seq_logoExit(void) {
    extern void fontmgrTexSetup(void);
    extern void windowTexSetup(void);
    extern void fadeTexSetup(void);
    extern void iconTexSetup(void);
    extern void envTexSetup(void);
    extern void effTexSetup(void);
    extern void marioPoseInit(void);
    extern void fileFree(void* file);
    extern void* wp;

    fontmgrTexSetup();
    windowTexSetup();
    fadeTexSetup();
    iconTexSetup();
    envTexSetup();
    effTexSetup();
    marioPoseInit();
    fileFree(*(void**)wp);
}

