#include "sdk/DEMOInit.h"

void* DemoCurrentBuffer;
void* rmode;

void* L_DEMOGetCurrentBuffer(void) {
    return DemoCurrentBuffer;
}

void* DEMOGetRenderModeObj(void) {
    return rmode;
}


void __NoHangDoneRender(void) {
    typedef struct {
        u32 data[15];
    } GXRenderModeObj;
    extern u32 sysGetToken(void);
    extern void GXSetZMode(u32 enable, u32 func, u32 update);
    extern void GXSetColorUpdate(u32 enable);
    extern void GXSetAlphaCompare(u32 comp0, u32 ref0, u32 op, u32 comp1, u32 ref1);
    extern void GXSetAlphaUpdate(u32 enable);
    extern void GXCopyDisp(void* dest, u8 clear);
    extern void GXSetDrawSync(u32 token);
    extern u32 GXReadDrawSync(void);
    extern void VISetNextFrameBuffer(void* fb);
    extern void VISetBlack(s32 black);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void __DEMODiagnoseHang(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void __memFree(s32 heap, void* ptr);
    extern void* GXGetCPUFifo(void);
    extern void* GXGetFifoBase(void* fifo);
    extern u32 GXGetFifoSize(void* fifo);
    extern void GXAbortFrame(void);
    extern void GXInitFifoBase(void* fifo, void* base, u32 size);
    extern void GXSetCPUFifo(void* fifo);
    extern void GXSetGPFifo(void* fifo);
    extern s32 VIGetTvFormat(void);
    extern void OSPanic(const char* file, s32 line, const char* msg, ...);
    extern void GXAdjustForOverscan(void* src, void* dst, s32 x, s32 y);
    extern void* GXInit(void* base, u32 size);
    extern void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
    extern void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
    extern f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
    extern u32 GXSetDispCopyYScale(f32 scale);
    extern void GXSetDispCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetDispCopyDst(u16 width, u16 height);
    extern void GXSetCopyFilter(u8 aa, void* sample_pattern, u8 vf, void* vfilter);
    extern void GXSetPixelFmt(s32 pix_fmt, s32 z_fmt);
    extern void GXSetCopyClear(void* color, u32 z);
    extern void GXSetDispCopyGamma(s32 gamma);
    extern void VIConfigure(void* mode);
    extern void GXSetGPMetric(u32 perf0, u32 perf1);
    extern u8 rmodeobj[];
    extern u8 GXNtsc480IntDf[];
    extern u8 GXMpal480IntDf[];
    extern u8 GXPal528IntDf[];
    extern u8 GXEurgb60Hz480IntDf[];
    extern char str_DEMOInit_c_802bfe68[];
    extern char str_DEMOInit_invalid_TV__802bfe74[];
    extern void* rmode;
    extern void* DemoCurrentBuffer;
    extern void* DemoFrameBuffer1;
    extern void* DemoFrameBuffer2;
    extern void* DefaultFifoObj;
    extern u32 FrameCount;
    extern u32 FrameMissThreshold;
    extern u8 DemoFirstFrame;
    extern u32 unk_80429548;
    extern f32 float_0_8041fa10;
    extern f32 float_1_8041fa14;
    volatile u8* fifo8 = (volatile u8*)0xCC008000;
    volatile u16* fifo16 = (volatile u16*)0xCC008000;
    volatile u32* fifo32 = (volatile u32*)0xCC008000;
    GXRenderModeObj fifoObj[2];
    GXRenderModeObj* mode;
    void* oldMode;
    void* tempFifo;
    void* oldFifo;
    void* oldBase;
    u32 oldSize;
    u32 token;
    u32 yScale;
    u32 color;
    s32 tvFormat;
    u8 hung;

    hung = 0;
    token = sysGetToken();
    GXSetZMode(1, 3, 1);
    GXSetColorUpdate(1);
    GXSetAlphaCompare(7, 0, 1, 7, 0);
    GXSetAlphaUpdate(1);
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDrawSync(token);
    FrameCount = 0;
    token &= 0xFFFF;

    while (1) {
        if (((GXReadDrawSync() & 0xFFFF) == token) || hung) {
            VISetNextFrameBuffer(DemoCurrentBuffer);
            if (DemoFirstFrame != 0) {
                VISetBlack(0);
                DemoFirstFrame = 0;
            }
            VIFlush();
            VIWaitForRetrace();
            if (DemoCurrentBuffer == DemoFrameBuffer1) {
                DemoCurrentBuffer = DemoFrameBuffer2;
            } else {
                DemoCurrentBuffer = DemoFrameBuffer1;
            }
            return;
        }
        if (FrameCount < FrameMissThreshold) {
            continue;
        }
        hung = 1;
        __DEMODiagnoseHang();
        oldMode = rmode;
        tempFifo = __memAlloc(0, 0x10000);
        oldFifo = GXGetCPUFifo();
        oldBase = GXGetFifoBase(oldFifo);
        oldSize = GXGetFifoSize(oldFifo);
        GXAbortFrame();
        GXInitFifoBase(fifoObj, tempFifo, 0x10000);
        GXSetCPUFifo(fifoObj);
        GXSetGPFifo(fifoObj);
        if (oldMode != 0) {
            *(GXRenderModeObj*)rmodeobj = *(GXRenderModeObj*)oldMode;
            rmode = rmodeobj;
        } else {
            tvFormat = VIGetTvFormat();
            if (tvFormat == 2) {
                rmode = GXMpal480IntDf;
            } else if (tvFormat < 2) {
                if (tvFormat == 0) {
                    rmode = GXNtsc480IntDf;
                } else if (tvFormat >= 0) {
                    rmode = GXPal528IntDf;
                } else {
                    OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
                }
            } else if (tvFormat == 5) {
                rmode = GXEurgb60Hz480IntDf;
            } else {
                OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
            }
            GXAdjustForOverscan(rmode, rmodeobj, 0, 0x10);
            rmode = rmodeobj;
        }
        mode = (GXRenderModeObj*)rmode;
        DefaultFifoObj = GXInit(oldBase, oldSize);
        GXSetViewport(float_0_8041fa10, float_0_8041fa10, (f32)*(u16*)((s32)mode + 4), (f32)*(u16*)((s32)mode + 6), float_0_8041fa10, float_1_8041fa14);
        GXSetScissor(0, 0, *(u16*)((s32)mode + 4), *(u16*)((s32)mode + 6));
        yScale = GXSetDispCopyYScale(GXGetYScaleFactor(*(u16*)((s32)mode + 6), *(u16*)((s32)mode + 8)));
        GXSetDispCopySrc(0, 0, *(u16*)((s32)mode + 4), *(u16*)((s32)mode + 6));
        GXSetDispCopyDst(*(u16*)((s32)mode + 4), yScale);
        GXSetCopyFilter(*(u8*)((s32)mode + 0x19), (void*)((s32)mode + 0x1A), 1, (void*)((s32)mode + 0x32));
        if (*(u8*)((s32)mode + 0x19) == 0) {
            GXSetPixelFmt(1, 0);
        } else {
            GXSetPixelFmt(2, 0);
        }
        color = unk_80429548;
        GXSetCopyClear(&color, 0xFFFFFF);
        GXCopyDisp(DemoCurrentBuffer, 1);
        GXSetDispCopyGamma(0);
        VIConfigure(rmode);
        VISetNextFrameBuffer(DemoFrameBuffer1);
        DemoCurrentBuffer = DemoFrameBuffer2;
        VIFlush();
        VIWaitForRetrace();
        if ((*(u32*)rmode & 1) != 0) {
            VIWaitForRetrace();
        }
        __memFree(0, tempFifo);
        GXSetGPMetric(0x23, 0x16);
        *fifo8 = 0x61;
        *fifo32 = 0x2402C004;
        *fifo8 = 0x61;
        *fifo32 = 0x23000020;
        *fifo8 = 0x10;
        *fifo16 = 0;
        *fifo16 = 0x1006;
        *fifo32 = 0x84400;
    }
}

void DEMOInit(void* param_1) {
    typedef struct {
        u32 data[15];
    } GXRenderModeObj;
    extern void OSInit(void);
    extern void DVDInit(void);
    extern void VIInit(void);
    extern void DEMOPadInit(void);
    extern s32 VIGetTvFormat(void);
    extern void OSPanic(const char* file, s32 line, const char* msg, ...);
    extern void GXAdjustForOverscan(void* src, void* dst, s32 x, s32 y);
    extern void* OSGetArenaLo(void);
    extern void* OSGetArenaHi(void);
    extern void OSSetArenaLo(void* lo);
    extern void memInit(void);
    extern void VIConfigure(void* mode);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* GXInit(void* base, u32 size);
    extern void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
    extern void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
    extern f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
    extern u32 GXSetDispCopyYScale(f32 scale);
    extern void GXSetDispCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetDispCopyDst(u16 width, u16 height);
    extern void GXSetCopyFilter(u8 aa, void* sample_pattern, u8 vf, void* vfilter);
    extern void GXSetPixelFmt(s32 pix_fmt, s32 z_fmt);
    extern void GXSetCopyClear(void* color, u32 z);
    extern void GXCopyDisp(void* dest, u8 clear);
    extern void GXSetDispCopyGamma(s32 gamma);
    extern void VISetNextFrameBuffer(void* fb);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern u8 rmodeobj[];
    extern u8 GXNtsc480IntDf[];
    extern u8 GXMpal480IntDf[];
    extern u8 GXPal528IntDf[];
    extern u8 GXEurgb60Hz480IntDf[];
    extern char str_DEMOInit_c_802bfe68[];
    extern char str_DEMOInit_invalid_TV__802bfe74[];
    extern void* rmode;
    extern void* DemoCurrentBuffer;
    extern void* DemoFrameBuffer1;
    extern void* DemoFrameBuffer2;
    extern void* DefaultFifo;
    extern void* DefaultFifoObj;
    extern u32 unk_80429548;
    extern f32 float_0_8041fa10;
    extern f32 float_1_8041fa14;
    GXRenderModeObj* mode;
    u32 color;
    u32 yScale;
    u32 fbSize;
    u32 arena;
    s32 tvFormat;

    OSInit();
    DVDInit();
    VIInit();
    DEMOPadInit();

    if (param_1 != 0) {
        *(GXRenderModeObj*)rmodeobj = *(GXRenderModeObj*)param_1;
        rmode = rmodeobj;
    } else {
        tvFormat = VIGetTvFormat();
        if (tvFormat == 2) {
            rmode = GXMpal480IntDf;
        } else if (tvFormat < 2) {
            if (tvFormat == 0) {
                rmode = GXNtsc480IntDf;
            } else if (tvFormat >= 0) {
                rmode = GXPal528IntDf;
            } else {
                OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
            }
        } else if (tvFormat == 5) {
            rmode = GXEurgb60Hz480IntDf;
        } else {
            OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
        }
        GXAdjustForOverscan(rmode, rmodeobj, 0, 0x10);
        rmode = rmodeobj;
    }

    arena = (u32)OSGetArenaLo();
    OSGetArenaHi();
    mode = (GXRenderModeObj*)rmode;
    DemoFrameBuffer1 = (void*)((arena + 0x1F) & ~0x1F);
    fbSize = (((*(u16*)((s32)mode + 4) + 0xF) & 0xFFF0) * *(u16*)((s32)mode + 8) * 2) + 0x1F;
    DemoCurrentBuffer = (void*)(((u32)DemoFrameBuffer1 + fbSize) & ~0x1F);
    DemoFrameBuffer2 = DemoCurrentBuffer;
    OSSetArenaLo((void*)(((u32)DemoCurrentBuffer + fbSize) & ~0x1F));
    memInit();
    VIConfigure(rmode);
    DefaultFifo = __memAlloc(0, 0x64000);
    DefaultFifoObj = GXInit(DefaultFifo, 0x64000);
    GXSetViewport(float_0_8041fa10, float_0_8041fa10, (f32)*(u16*)((s32)mode + 4), (f32)*(u16*)((s32)mode + 6), float_0_8041fa10, float_1_8041fa14);
    GXSetScissor(0, 0, *(u16*)((s32)mode + 4), *(u16*)((s32)mode + 6));
    yScale = GXSetDispCopyYScale(GXGetYScaleFactor(*(u16*)((s32)mode + 6), *(u16*)((s32)mode + 8)));
    GXSetDispCopySrc(0, 0, *(u16*)((s32)mode + 4), *(u16*)((s32)mode + 6));
    GXSetDispCopyDst(*(u16*)((s32)mode + 4), yScale);
    GXSetCopyFilter(*(u8*)((s32)mode + 0x19), (void*)((s32)mode + 0x1A), 1, (void*)((s32)mode + 0x32));
    if (*(u8*)((s32)mode + 0x19) == 0) {
        GXSetPixelFmt(1, 0);
    } else {
        GXSetPixelFmt(2, 0);
    }
    color = unk_80429548;
    GXSetCopyClear(&color, 0xFFFFFF);
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDispCopyGamma(0);
    VISetNextFrameBuffer(DemoFrameBuffer1);
    DemoCurrentBuffer = DemoFrameBuffer2;
    VIFlush();
    VIWaitForRetrace();
    if ((*(u32*)rmode & 1) != 0) {
        VIWaitForRetrace();
    }
}

void __DEMODiagnoseHang(void) {
    extern void GXReadXfRasMetric(s32* xf, s32* ras, s32* xfiread, s32* xfwrite);
    extern void GXGetGPStatus(u8* overhi, u8* underlow, u8* readIdle, u8* cmdIdle, u8* brkpt);
    extern s32 sprintf(char* s, const char* fmt, ...);
    extern char* strcat(char* dst, const char* src);
    extern char* strcpy(char* dst, const char* src);
    extern char __GXErrorMessage[];
    extern const char str_GP_status_PCTdPCTdPC_802bfea0[];
    extern const char str_GP_hang_due_to_XF_st_802bfebc[];
    extern const char str_GP_hang_due_to_unter_802bfedc[];
    extern const char str_GP_hang_due_to_illeg_802bff04[];
    extern const char str_GP_appears_to_be_not_802bff2c[];
    extern const char str_GP_is_in_unknown_sta_802bff5c[];
    char msg[4096];
    char add[4112];
    s32 xfTop0;
    s32 rasTop0;
    s32 xfBot0;
    s32 rasBot0;
    s32 xfTop1;
    s32 rasTop1;
    s32 xfBot1;
    s32 rasBot1;
    u8 overhi;
    u8 cmdIdle;
    u8 readIdle[2];
    s32 sameXfTop;
    s32 sameRasTop;
    s32 diffRasBot;
    s32 diffXfBot;

    GXReadXfRasMetric(&xfTop0, &rasTop0, &xfBot0, &rasBot0);
    GXReadXfRasMetric(&xfTop1, &rasTop1, &xfBot1, &rasBot1);
    diffRasBot = rasBot1 != rasBot0;
    sameRasTop = rasTop1 == rasTop0;
    sameXfTop = xfTop1 == xfTop0;
    diffXfBot = xfBot1 != xfBot0;
    GXGetGPStatus(&overhi, &overhi, readIdle, &cmdIdle, &overhi);
    sprintf(msg, str_GP_status_PCTdPCTdPC_802bfea0, readIdle[0], cmdIdle, sameRasTop, sameXfTop, diffRasBot, diffXfBot);
    if (sameXfTop == 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_XF_st_802bfebc);
        strcat(msg, add);
    } else if (sameRasTop == 0 && sameXfTop != 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_unter_802bfedc);
        strcat(msg, add);
    } else if (cmdIdle == 0 && sameRasTop != 0 && sameXfTop != 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_illeg_802bff04);
        strcat(msg, add);
    } else if (readIdle[0] != 0 && cmdIdle != 0 && sameRasTop != 0 && sameXfTop != 0 && diffRasBot && diffXfBot) {
        sprintf(add, str_GP_appears_to_be_not_802bff2c);
        strcat(msg, add);
    } else {
        sprintf(add, str_GP_is_in_unknown_sta_802bff5c);
        strcat(msg, add);
    }
    strcpy(__GXErrorMessage, add);
}

void DEMOBeforeRender(void) {
    ;
}


u8 DEMOEnableGPHangWorkaround(int param_1) {
    return 0;
}


void DEMODoneRender(void) {
    ;
}


u8 __NoHangRetraceCallback(void) {
    return 0;
}
