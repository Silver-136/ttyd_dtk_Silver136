#include "sdk/DEMOInit.h"

const char str_DEMOInit_c_802bfe68[] = "DEMOInit.c";
const char str_DEMOInit_invalid_TV__802bfe74[] = "DEMOInit: invalid TV format\n";
const char str_GP_status_PCTdPCTdPC_802bfea0[] = "GP status %d%d%d%d%d%d -->\n";
const char str_GP_hang_due_to_XF_st_802bfebc[] = "GP hang due to XF stall bug.\n";
const char str_GP_hang_due_to_unter_802bfedc[] = "GP hang due to unterminated primitive.\n";
const char str_GP_hang_due_to_illeg_802bff04[] = "GP hang due to illegal instruction.\n";
const char str_GP_appears_to_be_not_802bff2c[] = "GP appears to be not hung (waiting for input).\n";
const char str_GP_is_in_unknown_sta_802bff5c[] = "GP is in unknown state.\n";
const char str_WARNING_HANG_AT_HIGH_802bff78[] = "---------WARNING : HANG AT HIGH WATERMARK----------\n";

u8 rmodeobj[0x3C];
char __GXErrorMessage[0x400];

u8 DemoFirstFrame = 1;

s32 GPHangWorkaround;
u32 FrameMissThreshold;
u32 FrameCount;
void* rmode;
void* DefaultFifoObj;
void* DefaultFifo;
void* DemoCurrentBuffer;
void* DemoFrameBuffer2;
void* DemoFrameBuffer1;

extern u8 GXNtsc480IntDf[];
extern u8 GXMpal480IntDf[];
extern u8 GXPal528IntDf[];
extern u8 GXEurgb60Hz480IntDf[];
extern u32 sysGetToken(void);
extern void __DEMODiagnoseHang(void);
extern u32 unk_80429548;
extern const f32 float_0_8041fa10;
extern const f32 float_1_8041fa14;
extern s32 VIGetTvFormat(void);
extern void OSPanic(const char* file, s32 line, const char* msg, ...);
extern void GXAdjustForOverscan(void* src, void* dst, s32 x, s32 y);
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

void DEMOInit(void* param_1) {
    typedef struct { u32 data[15]; } GXRenderModeObj;
    extern void OSInit(void);
    extern void DVDInit(void);
    extern void VIInit(void);
    extern void DEMOPadInit(void);
    extern void* OSGetArenaLo(void);
    extern void* OSGetArenaHi(void);
    extern void OSSetArenaLo(void* lo);
    extern void memInit(void);
    u32 color;
    u32 yScale;
    u32 fbSize;
    u32 arena;
    u32 copyA;
    u32 copyB;
    s32 tvFormat;

    OSInit();
    DVDInit();
    VIInit();
    DEMOPadInit();
    if (param_1 != 0) {
        copyA = ((u32*)param_1)[0]; copyB = ((u32*)param_1)[1];
        ((u32*)rmodeobj)[0] = copyA; ((u32*)rmodeobj)[1] = copyB;
        copyA = ((u32*)param_1)[2]; copyB = ((u32*)param_1)[3];
        ((u32*)rmodeobj)[2] = copyA; ((u32*)rmodeobj)[3] = copyB;
        copyA = ((u32*)param_1)[4]; copyB = ((u32*)param_1)[5];
        ((u32*)rmodeobj)[4] = copyA; ((u32*)rmodeobj)[5] = copyB;
        copyA = ((u32*)param_1)[6]; copyB = ((u32*)param_1)[7];
        ((u32*)rmodeobj)[6] = copyA; ((u32*)rmodeobj)[7] = copyB;
        copyA = ((u32*)param_1)[8]; copyB = ((u32*)param_1)[9];
        ((u32*)rmodeobj)[8] = copyA; ((u32*)rmodeobj)[9] = copyB;
        copyA = ((u32*)param_1)[10]; copyB = ((u32*)param_1)[11];
        ((u32*)rmodeobj)[10] = copyA; ((u32*)rmodeobj)[11] = copyB;
        copyA = ((u32*)param_1)[12]; copyB = ((u32*)param_1)[13];
        ((u32*)rmodeobj)[12] = copyA; ((u32*)rmodeobj)[13] = copyB;
        ((u32*)rmodeobj)[14] = ((u32*)param_1)[14];
        rmode = rmodeobj;
    } else {
        tvFormat = VIGetTvFormat();
        if (tvFormat == 2) rmode = GXMpal480IntDf;
        else if (tvFormat < 2) {
            if (tvFormat == 0) rmode = GXNtsc480IntDf;
            else if (tvFormat >= 0) rmode = GXPal528IntDf;
            else OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
        } else if (tvFormat == 5) rmode = GXEurgb60Hz480IntDf;
        else OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
        GXAdjustForOverscan(rmode, rmodeobj, 0, 0x10);
        rmode = rmodeobj;
    }
    arena = (u32)OSGetArenaLo();
    OSGetArenaHi();
    {
    GXRenderModeObj* mode = (GXRenderModeObj*)rmode;
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
    if (*(u8*)((s32)mode + 0x19) == 0) GXSetPixelFmt(1, 0);
    else GXSetPixelFmt(2, 0);
    color = unk_80429548;
    GXSetCopyClear(&color, 0xFFFFFF);
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDispCopyGamma(0);
    VISetNextFrameBuffer(DemoFrameBuffer1);
    DemoCurrentBuffer = DemoFrameBuffer2;
    VIFlush();
    VIWaitForRetrace();
    if ((*(u32*)rmode & 1) != 0) VIWaitForRetrace();
    }
}

const f32 float_0_8041fa10 = 0.0f;
const f32 float_1_8041fa14 = 1.0f;

void DEMOBeforeRender(void) {
    extern void GXSetDrawSync(u32);
    extern void GXClearGPMetric(void);
    extern void GXSetViewportJitter(f32, f32, f32, f32, f32, f32, u32);
    extern u32 VIGetNextField(void);
    extern void GXInvalidateVtxCache(void);
    extern void GXInvalidateTexAll(void);
    u8* mode;

    if (GPHangWorkaround != 0) {
        GXSetDrawSync(sysGetToken());
        GXClearGPMetric();
    }
    mode = rmode;
    if (mode[0x18] == 0) {
        GXSetViewport(float_0_8041fa10, float_0_8041fa10,
                      (f32)*(u16*)(mode + 4), (f32)*(u16*)(mode + 6),
                      float_0_8041fa10, float_1_8041fa14);
    } else {
        GXSetViewportJitter(float_0_8041fa10, float_0_8041fa10,
                            (f32)*(u16*)(mode + 4), (f32)*(u16*)(mode + 6),
                            float_0_8041fa10, float_1_8041fa14, VIGetNextField());
    }
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

void DEMODoneRender(void) {
    extern u8 DemoStatEnable;
    extern void GXDrawDone(void);
    extern void DEMOUpdateStats(u8);
    extern void DEMOPrintStats(void);
    extern void GXSetZMode(u32, u32, u32);
    extern void GXSetColorUpdate(u32);
    extern void GXSetAlphaCompare(u32, u32, u32, u32, u32);
    extern void GXSetAlphaUpdate(u32);
    extern void VISetBlack(s32);
    extern void __NoHangDoneRender(void);

    if (GPHangWorkaround == 0) {
        if (DemoStatEnable != 0) {
            GXDrawDone();
            DEMOUpdateStats(1);
            DEMOPrintStats();
            GXDrawDone();
            DEMOUpdateStats(0);
        }
        GXSetZMode(1, 3, 1);
        GXSetColorUpdate(1);
        GXSetAlphaCompare(7, 0, 1, 7, 0);
        GXSetAlphaUpdate(1);
        GXCopyDisp(DemoCurrentBuffer, 1);
        GXDrawDone();
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
    } else {
        __NoHangDoneRender();
    }
}

void* DEMOGetRenderModeObj(void) {
    return rmode;
}

void* L_DEMOGetCurrentBuffer(void) {
    return DemoCurrentBuffer;
}

void DEMOEnableGPHangWorkaround(u32 threshold) {
    extern void VISetPreRetraceCallback(void*);
    extern void GXSetGPMetric(u32, u32);
    extern void __NoHangRetraceCallback(void);
    volatile u8* fifo8 = (volatile u8*)0xCC008000;
    volatile u16* fifo16 = (volatile u16*)0xCC008000;
    volatile u32* fifo32 = (volatile u32*)0xCC008000;

    if (threshold != 0) {
        GPHangWorkaround = 1;
        FrameMissThreshold = threshold;
        VISetPreRetraceCallback(__NoHangRetraceCallback);
        GXSetGPMetric(0x23, 0x16);
        *fifo8 = 0x61;
        *fifo32 = 0x2402C004;
        *fifo8 = 0x61;
        *fifo32 = 0x23000020;
        *fifo8 = 0x10;
        *fifo16 = 0;
        *fifo16 = 0x1006;
        *fifo32 = 0x84400;
    } else {
        GPHangWorkaround = 0;
        FrameMissThreshold = 0;
        *fifo8 = 0x61;
        *fifo32 = 0x24000000;
        *fifo8 = 0x61;
        *fifo32 = 0x23000000;
        *fifo8 = 0x10;
        *fifo16 = 0;
        *fifo16 = 0x1006;
        *fifo32 = 0;
        VISetPreRetraceCallback(0);
    }
}

void __NoHangRetraceCallback(void) {
    static u32 ovFrameCount;
    static u32 lastOvc;
    extern void GXGetGPStatus(u8*, u8*, u8*, u8*, u8*);
    extern u32 GXGetOverflowCount(void);
    extern char* strcpy(char*, const char*);
    u32 overflow;
    u8 overhi;
    u8 junk;

    FrameCount++;
    GXGetGPStatus(&overhi, &junk, &junk, &junk, &junk);
    overflow = GXGetOverflowCount();
    if (overhi && overflow == lastOvc) {
        ovFrameCount++;
        if (ovFrameCount >= FrameMissThreshold) {
            __DEMODiagnoseHang();
            strcpy(__GXErrorMessage, str_WARNING_HANG_AT_HIGH_802bff78);
        }
    } else {
        lastOvc = overflow;
        ovFrameCount = 0;
    }
}

void __NoHangDoneRender(void) {
    typedef struct {
        u32 d0;
        u32 d1;
        u32 d2;
        u32 d3;
        u32 d4;
        u32 d5;
        u32 d6;
        u32 d7;
        u32 d8;
        u32 d9;
        u32 d10;
        u32 d11;
        u32 d12;
        u32 d13;
        u32 d14;
    } GXRenderModeObj;
    extern u32 sysGetToken(void);
    extern void GXSetZMode(u32, u32, u32);
    extern void GXSetColorUpdate(u32);
    extern void GXSetAlphaCompare(u32, u32, u32, u32, u32);
    extern void GXSetAlphaUpdate(u32);
    extern void GXCopyDisp(void*, u8);
    extern void GXSetDrawSync(u32);
    extern u32 GXReadDrawSync(void);
    extern void VISetNextFrameBuffer(void*);
    extern void VISetBlack(s32);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void __DEMODiagnoseHang(void);
    extern void* __memAlloc(s32, u32);
    extern void __memFree(s32, void*);
    extern void* GXGetCPUFifo(void);
    extern void* GXGetFifoBase(void*);
    extern u32 GXGetFifoSize(void*);
    extern void GXAbortFrame(void);
    extern void GXInitFifoBase(void*, void*, u32);
    extern void GXSetCPUFifo(void*);
    extern void GXSetGPFifo(void*);
    extern s32 VIGetTvFormat(void);
    extern void OSPanic(const char*, s32, const char*, ...);
    extern void GXAdjustForOverscan(void*, void*, s32, s32);
    extern void* GXInit(void*, u32);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetScissor(u32, u32, u32, u32);
    extern f32 GXGetYScaleFactor(u16, u16);
    extern u32 GXSetDispCopyYScale(f32);
    extern void GXSetDispCopySrc(u32, u32, u32, u32);
    extern void GXSetDispCopyDst(u16, u16);
    extern void GXSetCopyFilter(u8, void*, u8, void*);
    extern void GXSetPixelFmt(s32, s32);
    extern void GXSetCopyClear(void*, u32);
    extern void GXSetDispCopyGamma(s32);
    extern void VIConfigure(void*);
    extern void GXSetGPMetric(u32, u32);
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
    u16 token;
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

    while (((GXReadDrawSync() & 0xFFFF) != token) && !hung) {
        if (FrameCount >= FrameMissThreshold) {
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
                ((u32*)rmodeobj)[0] = ((u32*)oldMode)[0];
                ((u32*)rmodeobj)[1] = ((u32*)oldMode)[1];
                ((u32*)rmodeobj)[2] = ((u32*)oldMode)[2];
                ((u32*)rmodeobj)[3] = ((u32*)oldMode)[3];
                ((u32*)rmodeobj)[4] = ((u32*)oldMode)[4];
                ((u32*)rmodeobj)[5] = ((u32*)oldMode)[5];
                ((u32*)rmodeobj)[6] = ((u32*)oldMode)[6];
                ((u32*)rmodeobj)[7] = ((u32*)oldMode)[7];
                ((u32*)rmodeobj)[8] = ((u32*)oldMode)[8];
                ((u32*)rmodeobj)[9] = ((u32*)oldMode)[9];
                ((u32*)rmodeobj)[10] = ((u32*)oldMode)[10];
                ((u32*)rmodeobj)[11] = ((u32*)oldMode)[11];
                ((u32*)rmodeobj)[12] = ((u32*)oldMode)[12];
                ((u32*)rmodeobj)[13] = ((u32*)oldMode)[13];
                ((u32*)rmodeobj)[14] = ((u32*)oldMode)[14];
                rmode = rmodeobj;
            } else {
                tvFormat = VIGetTvFormat();
                if (tvFormat == 2) rmode = GXMpal480IntDf;
                else if (tvFormat < 2) {
                    if (tvFormat == 0) rmode = GXNtsc480IntDf;
                    else if (tvFormat >= 0) rmode = GXPal528IntDf;
                    else OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
                } else if (tvFormat == 5) rmode = GXEurgb60Hz480IntDf;
                else OSPanic(str_DEMOInit_c_802bfe68, 0x206, str_DEMOInit_invalid_TV__802bfe74);
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
            if (*(u8*)((s32)mode + 0x19) == 0) GXSetPixelFmt(1, 0);
            else GXSetPixelFmt(2, 0);
            color = unk_80429548;
            GXSetCopyClear(&color, 0xFFFFFF);
            GXCopyDisp(DemoCurrentBuffer, 1);
            GXSetDispCopyGamma(0);
            VIConfigure(rmode);
            VISetNextFrameBuffer(DemoFrameBuffer1);
            DemoCurrentBuffer = DemoFrameBuffer2;
            VIFlush();
            VIWaitForRetrace();
            if ((*(u32*)rmode & 1) != 0) VIWaitForRetrace();
            __memFree(0, tempFifo);
            GXSetGPMetric(0x23, 0x16);
            *fifo8 = 0x61; *fifo32 = 0x2402C004;
            *fifo8 = 0x61; *fifo32 = 0x23000020;
            *fifo8 = 0x10; *fifo16 = 0; *fifo16 = 0x1006; *fifo32 = 0x84400;
        }
    }
    VISetNextFrameBuffer(DemoCurrentBuffer);
    if (DemoFirstFrame != 0) {
        VISetBlack(0);
        DemoFirstFrame = 0;
    }
    VIFlush();
    VIWaitForRetrace();
    if (DemoCurrentBuffer == DemoFrameBuffer1) DemoCurrentBuffer = DemoFrameBuffer2;
    else DemoCurrentBuffer = DemoFrameBuffer1;
}

void __DEMODiagnoseHang(void) {
    extern void GXReadXfRasMetric(s32* xf, s32* ras, s32* xfiread, s32* xfwrite);
    extern void GXGetGPStatus(u8* overhi, u8* underlow, u8* readIdle, u8* cmdIdle, u8* brkpt);
    extern s32 sprintf(char* s, const char* fmt, ...);
    extern char* strcat(char* dst, const char* src);
    extern char* strcpy(char* dst, const char* src);
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
