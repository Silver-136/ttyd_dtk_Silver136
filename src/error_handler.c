/* Original TU name unknown; separate object after eff_ultra_hammer in the retail DOL. */
#include "hwaccess.h"
#include "memory.h"
#include "system.h"
#include "addr_word.h"
#include "sdk/DEMOInit.h"
#include "pmario_sound.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <string.h>
#include "seq/seq_title.h"
#include "libc_decls.h"
#include "dolphin/base/PPCArch.h"

extern void* DemoCurrentBuffer, * DemoFrameBuffer1, * DemoFrameBuffer2;

//local prototypes
void N_systemErrorHandler(OSError error, OSContext* context, u32 dsisr, u32 dar);

void N_setupErrorHandler(void) {
    OSSetErrorHandler(OS_ERROR_DSI, (void (*)(u16, OSContext*, ...))N_systemErrorHandler);
    OSSetErrorHandler(OS_ERROR_ISI, (void (*)(u16, OSContext*, ...))N_systemErrorHandler);
    OSSetErrorHandler(OS_ERROR_PROGRAM, (void (*)(u16, OSContext*, ...))N_systemErrorHandler);
}

void N_systemErrorHandler(OSError error, OSContext* context, u32 dsisr, u32 dar) {
    //registers
    signed char* textBase;
    int i;
    OSFontHeader* fontHeader;
#ifdef VERSION_G8ME01
    s32 scroll;
#endif
    int px, py;
    u32 count;
    AddrWord stack; // stack frame address: back chain at +0, saved LR at +4
    s16 px2, py2, x2, y2;
    s32 tx, ty;
    //stack vars
    char print_buffer2[0x400]; // 0x10E0
    char print_buffer[0x1000]; // 0xE0
    Mtx44 projection; // 0xA0
    Mtx matrix; // 0x70
    Mtx matrix2; // 0x40
    GXTexObj texObj; // 0x20
    void* image; // 0x1C
    s32 x; // 0x18
    s32 y; // 0x14
    s32 width; // 0x10

    if (N_getDebugMode() >= 0) {
        psndExit();
#if defined(VERSION_G8ME01) && defined(__MWERKS__)
        // US: same MSR enable sequence as below (original inline asm), then halt
        HW_ENABLE_MSR_FP_RI();
        PPCHalt();
#endif
        while (1) {} //softlock
    }

    switch (error) {
        case OS_ERROR_DSI:
            strcpy(print_buffer, "---- OS_ERROR_DSI ----\n");
            break;
        case OS_ERROR_ISI:
            strcpy(print_buffer, "---- OS_ERROR_ISI ----\n");
            break;
        case OS_ERROR_PROGRAM:
            strcpy(print_buffer, "---- OS_ERROR_PROGRAM ----\n");
            break;
    }
    sprintf(print_buffer2, "---- Context 0x%08x ----\n", context);
    strcat(print_buffer, print_buffer2);
    for (i = 0; i < 16; i++) {
        sprintf(print_buffer2, "r%-2d  = 0x%08x r%-2d  = 0x%08x\n",
            i, context->gpr[i],
            i + 16, context->gpr[i + 16]
        );
        strcat(print_buffer, print_buffer2);
    }
    sprintf(print_buffer2, "LR   = 0x%08x CR   = 0x%08x\n", context->lr, context->cr);
    strcat(print_buffer, print_buffer2);
    sprintf(print_buffer2, "SRR0 = 0x%08x SRR1 = 0x%08x\n", context->srr0, context->srr1);
    strcat(print_buffer, print_buffer2);
    sprintf(print_buffer2, "DSISR= 0x%08x DAR  = 0x%08x\n", dsisr, dar);
    strcat(print_buffer, print_buffer2);
    strcat(print_buffer, "\n");
#ifdef VERSION_G8ME01
    sprintf(print_buffer2, "Address:     Back Chain   LR Save\n");
#else
    sprintf(print_buffer2, "Address:      Back Chain    LR Save\n");
#endif
    strcat(print_buffer, print_buffer2);
    stack = context->gpr[1]; //get r1 (stack)
    i = 0;
    while ((stack != 0) && (stack != 0xFFFFFFFF) && (i++ < 16)) {
#ifdef VERSION_G8ME01
        sprintf(print_buffer2, "0x%08x:  0x%08x   0x%08x\n", stack, *(AddrWord*)stack, *(AddrWord*)(stack + 4));
#else
        sprintf(print_buffer2, "0x%08x:   0x%08x    0x%08x\n", stack, *(AddrWord*)stack, *(AddrWord*)(stack + 4));
#endif
        strcat(print_buffer, print_buffer2);
        stack = *(AddrWord*)stack;
    }
    strcat(print_buffer, "\n");
    sprintf(print_buffer2, "Instruction at 0x%x\n(read from SRR0) attempted to access\ninvalid address 0x%x (read from DAR)\n", context->srr0, dar);
    strcat(print_buffer, print_buffer2);
    psndExit();
#ifdef VERSION_G8ME01
    scroll = 0;
#endif
    OSFillFPUContext(context);
    HW_ENABLE_MSR_FP_RI();
    L_smartReInit();
    if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
        fontHeader = smartAlloc(OS_FONT_SIZE_SJIS, 0)->address;
    } else {
        fontHeader = smartAlloc(OS_FONT_SIZE_ANSI, 0)->address;
    }
    OSInitFont(fontHeader);
    GXSetCopyClear((GXColor) { 0, 0, 0, 0xFF }, 0xFFFFFF);
    GXCopyDisp(DEMOGetCurrentBuffer(), 1);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 608, 480);
    MTXOrtho(projection, 0.0f, 608.0f, 0.0f, 480.0f, 0.0f, 1.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
#ifdef VERSION_G8ME01
    MTXScale(matrix, 0.8f, 1.0f, 1.0f);
#else
    MTXScale(matrix, 0.5f, 0.5f, 1.0f);
#endif
    GXLoadPosMtxImm(matrix, 0);
    GXSetCurrentMtx(0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    textBase = (signed char*)print_buffer;
    while (1) {
        count = VIGetRetraceCount();
#ifdef VERSION_G8ME01
        px = 40;
        py = 50 + scroll;
        if (--scroll < -960) {
            break;
        }
#else
        px = 50;
        py = 50;
#endif
        i = 0;
        while (textBase[i] != 0) {
            if ((s32)print_buffer[i] == '\n') {
#ifdef VERSION_G8ME01
                px = 40;
#else
                px = 50;
#endif
                py += 24;
                i++;
                continue;
            }
            OSGetFontTexture(&print_buffer[i], &image, &x, &y, &width);
            GXInitTexObj(&texObj, image, fontHeader->sheetWidth, fontHeader->sheetHeight,
                (GXTexFmt)fontHeader->sheetFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            MTXScale(matrix2, 1.0f / (float)fontHeader->sheetWidth, 1.0f / (float)fontHeader->sheetHeight, 1.0f);
            GXLoadTexMtxImm(matrix2, 0x1E, GX_MTX2x4);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            px2 = (s16)px + fontHeader->cellWidth;
            py2 = (s16)py + fontHeader->cellHeight;
            x2 = x + fontHeader->cellWidth;
            y2 = y + fontHeader->cellHeight;
            tx = x;
            ty = y;
            GXPosition1x16(px);
            GXPosition1x16(py);
            GXPosition1x16(0);
            GXPosition1x16(tx);
            GXPosition1x16(ty);
            GXPosition1x16(px2);
            GXPosition1x16(py);
            GXPosition1x16(0);
            GXPosition1x16(x2);
            GXPosition1x16(ty);
            GXPosition1x16(px2);
            GXPosition1x16(py2);
            GXPosition1x16(0);
            GXPosition1x16(x2);
            GXPosition1x16(y2);
            GXPosition1x16(px);
            GXPosition1x16(py2);
            GXPosition1x16(0);
            GXPosition1x16(tx);
            GXPosition1x16(y2);
            px += width;
            i++;
        }
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetColorUpdate(1);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetAlphaUpdate(1);
        GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
        VISetNextFrameBuffer(DemoCurrentBuffer);
        VISetBlack(0);
        VIFlush();
        do { ; } while (count == VIGetRetraceCount());
        if (DemoCurrentBuffer == DemoFrameBuffer1)
            DemoCurrentBuffer = DemoFrameBuffer2;
        else
            DemoCurrentBuffer = DemoFrameBuffer1;
    }
#ifdef VERSION_G8ME01
    PPCHalt();
    while (1) {}
#endif
}
