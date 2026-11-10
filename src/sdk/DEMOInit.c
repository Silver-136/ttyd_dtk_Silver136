// TTYD's own version of the SDK's DEMOInit, set up with the game's memory manager
#include "version.h"
#include "sdk/DEMOInit.h"
#include "memory.h"
#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <string.h>
#include <demo/DEMOPad.h>
#include "system.h"
#include "sdk_decls.h"
#include "libc_decls.h"

char __GXErrorMessage[0x400];
static GXRenderModeObj rmodeobj;
void* DemoFrameBuffer1;
void* DemoFrameBuffer2;
void* DemoCurrentBuffer;
static void* DefaultFifo;
static GXFifoObj* DefaultFifoObj;
static GXRenderModeObj* rmode;
static vu32 FrameCount;
static u32 FrameMissThreshold;

/** Set by DEMOEnableGPHangWorkaround: DEMOBeforeRender and DEMODoneRender then guard against GPU hangs. */
static BOOL GPHangWorkaround = FALSE;

/** True until the first frame is shown; DEMOSwapBuffers turns the video output on then (black until then). */
static GXBool DemoFirstFrame = GX_TRUE;

/** File name and message of the panic DEMOInit raises when the TV format is not one it knows. */
static const char demo_file[] = "DEMOInit.c";
static const char demo_msg[] = "DEMOInit: invalid TV format\n";

static void __NoHangRetraceCallback(u32 count);
static void __NoHangDoneRender(void);
static void __DEMODiagnoseHang(void);
static void DEMOSwapBuffers(void);

/** Carves the two external frame buffers off the bottom of the arena, sized for the render mode. */
static void __DEMOInitMem(void) {
    void *arenaLo, *arenaHi;
    u32 fbSize;

    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();

    fbSize = (u32)((rmode->fbWidth + 15) & 0xFFF0) * rmode->xfbHeight * (u32)VI_DISPLAY_PIX_SZ;
    DemoFrameBuffer1 = (void*)OSRoundUp32B((u32)arenaLo);
    DemoFrameBuffer2 = (void*)OSRoundUp32B((u32)DemoFrameBuffer1 + fbSize);
    DemoCurrentBuffer = DemoFrameBuffer2;

    arenaLo = (void*)OSRoundUp32B((u32)DemoFrameBuffer2 + fbSize);
    OSSetArenaLo(arenaLo);
}

/** Shows the first frame buffer and waits for a retrace (two in non-interlaced modes). */
static void __DEMOInitVI(void) {
    VISetNextFrameBuffer(DemoFrameBuffer1);
    DemoCurrentBuffer = DemoFrameBuffer2;
    VIFlush();
    VIWaitForRetrace();
    if (rmode->viTVmode & 1)
        VIWaitForRetrace();
}

/**
 * Sets GX's viewport, scissor, display copy scale and filter and pixel format (RGB565 when anti-aliased),
 * then copies the cleared EFB into the back buffer.
 */
static void __DEMOInitGX(void) {
    u16 xfbHeight;
    f32 yScale;

    GXSetViewport(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, (u32)rmode->fbWidth, (u32)rmode->efbHeight);
    yScale = GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    xfbHeight = (u16)GXSetDispCopyYScale(yScale);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, xfbHeight);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);

    if (rmode->aa)
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    else
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);

    GXSetCopyClear((GXColor){ 0, 0, 0, 0 }, GX_MAX_Z24);
    GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
    GXSetDispCopyGamma(GX_GM_1_0);
}

/**
 * Picks the render mode: a copy of the given one,
 * or the default for the console's TV format (NTSC, PAL, EURGB60, MPAL) adjusted for overscan.
 * Panics on an unknown format.
 */
#if VERSION <= VERSION_DEMO_JP
static inline void __DEMOInitRenderMode(GXRenderModeObj* mode) {
#else
static void __DEMOInitRenderMode(GXRenderModeObj* mode) {
#endif

    if (mode != NULL) {
        rmodeobj = *mode;
        rmode = &rmodeobj;
    } else {
        switch (VIGetTvFormat()) {
            case VI_NTSC:
                rmode = &GXNtsc480IntDf;
                break;
            case VI_PAL:
                rmode = &GXPal528IntDf;
                break;
            case VI_EURGB60:
                rmode = &GXEurgb60Hz480IntDf;
                break;
            case VI_MPAL:
                rmode = &GXMpal480IntDf;
                break;
            default:
#if VERSION <= VERSION_DEMO_JP
                OSPanic(demo_file, 0x203, demo_msg);
#else
                OSPanic(demo_file, 0x206, demo_msg);
#endif
                break;
        }
        GXAdjustForOverscan(rmode, &rmodeobj, 0, 16);
        rmode = &rmodeobj;
    }
}

/**
 * Boot-time setup: OS, DVD, VI, pads, render mode, frame buffers,
 * the game's heaps (memInit) and GX with its FIFO (0x60000 bytes in the early demos, 0x64000 in JP and later).
 */
void DEMOInit(GXRenderModeObj* mode) {
    OSInit();
    DVDInit();
    VIInit();
    DEMOPadInit();
    __DEMOInitRenderMode(mode);
    __DEMOInitMem();
    memInit();
    VIConfigure(rmode);
#if VERSION <= VERSION_DEMO_JP
    DefaultFifo = __memAlloc(HEAP_DEFAULT, 0x60000);
    DefaultFifoObj = GXInit(DefaultFifo, 0x60000);
#else
    DefaultFifo = __memAlloc(HEAP_DEFAULT, 0x64000);
    DefaultFifoObj = GXInit(DefaultFifo, 0x64000);
#endif
    __DEMOInitGX();
    __DEMOInitVI();
}

/**
 * Start of a frame: arms the hang-watch draw sync if enabled,
 * sets the viewport (jittered per field in field rendering) and invalidates the vertex and texture caches.
 */
void DEMOBeforeRender(void) {
    if (GPHangWorkaround) {
        GXSetDrawSync(sysGetToken());
        GXClearGPMetric();
    }

    if (rmode->field_rendering)
        GXSetViewportJitter(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f, VIGetNextField());
    else
        GXSetViewport(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f);

    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

/**
 * End of a frame: optional stats, resets the Z, colour and alpha state,
 * copies the EFB to the back buffer and swaps.
 * With the GP-hang workaround on it uses the watched variant instead.
 */
void DEMODoneRender(void) {
    if (GPHangWorkaround) {
        __NoHangDoneRender();
        return;
    }

    if (DemoStatEnable) {
        GXDrawDone();
        DEMOUpdateStats(GX_TRUE);
        DEMOPrintStats();
        GXDrawDone();
        DEMOUpdateStats(GX_FALSE);
    }

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetAlphaUpdate(GX_TRUE);
    GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
    GXDrawDone();
    DEMOSwapBuffers();
}

/** Queues the finished buffer for display (unblanking on the first frame), waits for retrace and flips buffers. */
static void DEMOSwapBuffers(void) {
    VISetNextFrameBuffer(DemoCurrentBuffer);
    if (DemoFirstFrame) {
        VISetBlack(FALSE);
        DemoFirstFrame = GX_FALSE;
    }
    VIFlush();
    VIWaitForRetrace();
    if (DemoCurrentBuffer == DemoFrameBuffer1)
        DemoCurrentBuffer = DemoFrameBuffer2;
    else
        DemoCurrentBuffer = DemoFrameBuffer1;
}

/** Returns the render mode in use. */
GXRenderModeObj* DEMOGetRenderModeObj(void) {
    return rmode;
}

/** Returns the frame buffer currently being drawn to. */
void* DEMOGetCurrentBuffer(void) {
    return DemoCurrentBuffer;
}

/** Turns the GP hang watchdog on (timeout in frames) or off (0), installing the retrace callback and metrics. */
void DEMOEnableGPHangWorkaround(u32 timeoutFrames) {
    if (timeoutFrames) {
        GPHangWorkaround = TRUE;
        FrameMissThreshold = timeoutFrames;
        VISetPreRetraceCallback(__NoHangRetraceCallback);
        DEMOSetGPHangMetric(GX_TRUE);
    } else {
        GPHangWorkaround = FALSE;
        FrameMissThreshold = 0;
        DEMOSetGPHangMetric(GX_FALSE);
        VISetPreRetraceCallback(NULL);
    }
}

/**
 * Retrace callback for the hang watchdog: counts frames and reports a hang
 * when the FIFO stays over the high watermark without progress for the timeout.
 */
static void __NoHangRetraceCallback(u32 count) {
#pragma unused (count)
    static u32 ovFrameCount = 0;
    static u32 lastOvc = 0;
    u32 ovc;
    GXBool overhi, junk;

    FrameCount++;
    GXGetGPStatus(&overhi, &junk, &junk, &junk, &junk);
    ovc = GXGetOverflowCount();
    if (overhi && (ovc == lastOvc)) {
        ovFrameCount++;
        if (ovFrameCount >= FrameMissThreshold) {
            __DEMODiagnoseHang();
            strcpy(__GXErrorMessage, "---------WARNING : HANG AT HIGH WATERMARK----------\n");
        }
    } else {
        lastOvc = ovc;
        ovFrameCount = 0;
    }
}

/**
 * Hang-safe frame end: copies the frame, then waits on a draw-sync token;
 * if the timeout passes, diagnoses the hang, re-initialises GX and continues.
 */
static void __NoHangDoneRender(void) {
    BOOL abort = FALSE;
    u16 token = sysGetToken();
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetAlphaUpdate(GX_TRUE);
    GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
    GXSetDrawSync(token);
    FrameCount = 0;

    while ((GXReadDrawSync() != token) && !abort) {
        if (FrameCount >= FrameMissThreshold) {
            abort = TRUE;
            __DEMODiagnoseHang();
            DEMOReInit(rmode);
            DEMOSetGPHangMetric(GX_TRUE);
        }
    }

    DEMOSwapBuffers();
}

/** Reads the GP performance counters twice and writes a guess at the hang's cause into __GXErrorMessage. */
static void __DEMODiagnoseHang(void) {
    u32 xfTop0, xfBot0, suRdy0, r0Rdy0;
    u32 xfTop1, xfBot1, suRdy1, r0Rdy1;
    u32 xfTopD, xfBotD, suRdyD, r0RdyD;
    GXBool readIdle, cmdIdle, junk;
    char newmsg[4096];
    char fullmsg[4096];

    GXReadXfRasMetric(&xfBot0, &xfTop0, &r0Rdy0, &suRdy0);
    GXReadXfRasMetric(&xfBot1, &xfTop1, &r0Rdy1, &suRdy1);

    xfTopD = (xfTop1 - xfTop0) == 0;
    xfBotD = (xfBot1 - xfBot0) == 0;
    suRdyD = (suRdy1 - suRdy0) > 0;
    r0RdyD = (r0Rdy1 - r0Rdy0) > 0;

    GXGetGPStatus(&junk, &junk, &readIdle, &cmdIdle, &junk);
    sprintf(fullmsg, "GP status %d%d%d%d%d%d -->\n", readIdle, cmdIdle, xfTopD, xfBotD, suRdyD, r0RdyD);
    if (!xfBotD && suRdyD) {
        sprintf(newmsg, "GP hang due to XF stall bug.\n");
        strcat(fullmsg, newmsg);
    } else if (!xfTopD && xfBotD && suRdyD) {
        sprintf(newmsg, "GP hang due to unterminated primitive.\n");
        strcat(fullmsg, newmsg);
    } else if (!cmdIdle && xfTopD && xfBotD && suRdyD) {
        sprintf(newmsg, "GP hang due to illegal instruction.\n");
        strcat(fullmsg, newmsg);
    } else if (readIdle && cmdIdle && xfTopD && xfBotD && suRdyD && r0RdyD) {
        sprintf(newmsg, "GP appears to be not hung (waiting for input).\n");
        strcat(fullmsg, newmsg);
    } else {
        sprintf(newmsg, "GP is in unknown state.\n");
        strcat(fullmsg, newmsg);
    }
    strcpy(__GXErrorMessage, newmsg);
}

/** Enables or disables the raw GP metric registers the hang diagnosis reads. */
void DEMOSetGPHangMetric(GXBool enable) {
    if (enable) {
        GXSetGPMetric(GX_PERF0_NONE, GX_PERF1_NONE);

        GXCmd1u8(GX_LOAD_BP_REG);
        GXParam1u32(0x2402C004);

        GXCmd1u8(GX_LOAD_BP_REG);
        GXParam1u32(0x23000020);

        GXCmd1u8(GX_LOAD_XF_REG);
        GXParam1u16(0x0000);
        GXParam1u16(0x1006);
        GXParam1u32(0x00084400);
    } else {
        GXCmd1u8(GX_LOAD_BP_REG);
        GXParam1u32(0x24000000);

        GXCmd1u8(GX_LOAD_BP_REG);
        GXParam1u32(0x23000000);

        GXCmd1u8(GX_LOAD_XF_REG);
        GXParam1u16(0x0000);
        GXParam1u16(0x1006);
        GXParam1u32(0x00000000);
    }
}

/**
 * Recovers from a GP hang: aborts the frame, swaps in a temporary FIFO,
 * then re-initialises GX and VI with the original FIFO.
 */
void DEMOReInit(GXRenderModeObj* mode) {
    GXFifoObj tmpobj;
    void* tmpFifo = __memAlloc(HEAP_DEFAULT, 64 * 1024);

    GXFifoObj* realFifoObj = GXGetCPUFifo();
    void* realFifoBase = GXGetFifoBase(realFifoObj);
    u32 realFifoSize = GXGetFifoSize(realFifoObj);

    GXAbortFrame();

    GXInitFifoBase(&tmpobj, tmpFifo, 64 * 1024);

    GXSetCPUFifo(&tmpobj);
    GXSetGPFifo(&tmpobj);

    __DEMOInitRenderMode(mode);

    DefaultFifoObj = GXInit(realFifoBase, realFifoSize);

    __DEMOInitGX();

    VIConfigure(rmode);
    __DEMOInitVI();

    __memFree(HEAP_DEFAULT, tmpFifo);
}
