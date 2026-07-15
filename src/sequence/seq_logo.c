#include "sequence/seq_logo.h"


void logoDisp(void) {
    extern void* wp;
    extern void* gp;
    extern u32 OSGetProgressiveMode(void);
    extern void GXGetProjectionv(f32* projection);
    extern void GXGetViewportv(f32* viewport);
    extern void GXGetScissor(u32* x, u32* y, u32* width, u32* height);
    extern void GXSetProjectionv(f32* projection);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetScissor(u32, u32, u32, u32);
    extern void GXSetPixelFmt(s32, s32);
    extern void GXSetDispCopySrc(u16, u16, u16, u16);
    extern void GXSetDispCopyDst(u16, u16);
    extern u32 GXSetDispCopyYScale(f32 scale);
    extern void GXSetDispCopyGamma(s32 gamma);
    extern void GXSetScissorBoxOffset(s32 x, s32 y);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void GXLoadTexObj(void* texObj, s32 map);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void C_MTXOrtho(f32* matrix, f32 top, f32 bottom, f32 left, f32 right, f32 nearZ, f32 farZ);
    extern void GXSetProjection(f32* matrix, s32 type);
    extern void PSMTXIdentity(f32* matrix);
    extern void GXLoadPosMtxImm(f32* matrix, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXBegin(s32 primitive, s32 format, s32 count);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u8 sRMObjHReso[];
    extern u8 sRMObjHReso_prog[];
    extern u32 dat_804207e8;
    extern u32 dat_804207ec;
    extern u32 dat_804207f0;
    extern u32 dat_804207f4;
    extern u32 dat_804207f8;
    extern u32 dat_804207fc;

    f32 projection[7];
    f32 viewport[6];
    f32 ortho[16];
    f32 model[12];
    u8 texObj[32];
    u32 scissorX;
    u32 scissorY;
    u32 scissorW;
    u32 scissorH;
    u32 color0;
    u32 color1;
    u8* mode;
    void* file;
    void* palette;
    s32 state;
    s32 texId;
    f32 left;
    f32 right;
    f32 top;
    f32 bottom;
    f32 width;
    f32 height;

#define FIFO (*(volatile f32*)0xCC008000)
#define VERTEX(px, py, s, t) do { FIFO=(px); FIFO=(py); FIFO=0.0f; FIFO=(s); FIFO=(t); } while (0)

    if (*(void**)wp == 0) {
        return;
    }
    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    GXGetScissor(&scissorX, &scissorY, &scissorW, &scissorH);
    mode = OSGetProgressiveMode() == 1 ? sRMObjHReso_prog : sRMObjHReso;
    GXSetPixelFmt(0, 0);
    GXSetDispCopySrc(0, 0, *(u16*)(mode + 4), *(u16*)(mode + 6));
    GXSetDispCopyDst(*(u16*)(mode + 4), *(u16*)(mode + 8));
    GXSetDispCopyYScale((f32)*(u16*)(mode + 8) / (f32)*(u16*)(mode + 6));
    GXSetDispCopyGamma(0);
    GXSetScissorBoxOffset(0, 0);
    GXSetScissor(0, 0, 608, 448);
    GXSetViewport(0.0f, 0.0f, 608.0f, 448.0f, -1.0f, 1.0f);

    file = *(void**)wp;
    palette = (void*)**(u32**)((s32)file + 0xA0);
    state = *(s32*)((s32)wp + 4);
    if (state == 2) {
        texId = 0;
        left = 117.0f; right = 493.0f; top = 154.0f; bottom = 258.0f;
        color0 = dat_804207f0;
        color1 = (*(s32*)((s32)gp + 0xD8) == 0) ? dat_804207f4 : dat_804207f8;
    } else if (state == 4) {
        texId = 2;
        left = 189.0f; right = 421.0f; top = 150.0f; bottom = 262.0f;
        color0 = dat_804207fc;
        color1 = dat_804207fc;
    } else if (state == 3) {
        texId = 1;
        left = 0.0f; right = 608.0f; top = 0.0f; bottom = 448.0f;
        color0 = 0xFFFFFFFF;
        color1 = 0xFFFFFFFF;
    } else {
        texId = 7;
        left = 0.0f; right = 608.0f; top = 0.0f; bottom = 448.0f;
        color0 = dat_804207e8;
        color1 = dat_804207ec;
    }

    TEXGetGXTexObjFromPalette(palette, texObj, texId);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(state == 4 ? 1 : 0);
    GXSetTevColor(1, &color0);
    GXSetTevColor(2, &color1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    C_MTXOrtho(ortho, state == 2 ? -23.0f : 0.0f, 448.0f, 0.0f, 608.0f, -100.0f, 100.0f);
    GXSetProjection(ortho, 1);
    PSMTXIdentity(model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    if (state == 3) {
        width = (f32)GXGetTexObjWidth(texObj);
        height = (f32)GXGetTexObjHeight(texObj);
        left = (608.0f - width) * 0.5f;
        right = left + width;
        top = (448.0f - height) * 0.5f - 20.0f;
        bottom = top + height;
    }
    GXBegin(0x80, 0, 4);
    VERTEX(left, top, 0.0f, 0.0f);
    VERTEX(right, top, 1.0f, 0.0f);
    VERTEX(right, bottom, 1.0f, 1.0f);
    VERTEX(left, bottom, 0.0f, 1.0f);

    GXSetProjectionv(projection);
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetScissor(scissorX, scissorY, scissorW, scissorH);

#undef VERTEX
#undef FIFO
}

u8 seq_logoMain(int param_1) {
    extern void fadeEntry(s32 type, s32 time, void* color);
    extern u32 fadeIsFinish(void);
    extern void DVDMgrSetupCallback(void* callback);
    extern s32 psndPushGroup(void);
    extern u32 keyGetButtonTrg(s32 pad);
    extern void VIConfigure(void* mode);
    extern void VIFlush(void);
    extern void VISetBlack(s32 black);
    extern void VIWaitForRetrace(void);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 order);
    extern void seqSetSeq(s32 seq, void* arg1, void* arg2);
    extern void dummy_draw(void);
    extern void progDisp(s32 camId);
    extern u8 logoDisp(void);
    extern void* wp;
    extern void* gp;
    extern u32 time_508;
    extern u32 DAT_8041ea7c;
    extern u32 dat_804207c0;
    extern u32 dat_804207c4;
    extern u32 dat_804207c8;
    extern u32 dat_804207cc;
    extern u32 dat_804207d0;
    extern u32 dat_804207d4;
    extern u32 dat_804207d8;
    extern u32 dat_804207dc;
    extern u32 dat_804207e0;
    extern u32 dat_804207e4;
    extern f32 float_0_80420824;
    extern f32 float_1_80420818;
    extern u8 GXNtsc480IntDfMarioSt[];
    extern u8 GXNtsc480ProgMarioSt[];
    extern char str_dmo_00_802c3b60[];

    u32 color;
    u64 now;
    u64 elapsed;
    s32* state = (s32*)(param_1 + 4);

#define START_FADE(kind, data) do { color = (data); fadeEntry((kind), 500, &color); } while (0)
#define SAVE_TIME() do { time_508 = *(u32*)((s32)gp + 0x18); DAT_8041ea7c = *(u32*)((s32)gp + 0x1C); } while (0)
#define ELAPSED_MS() ((*(u64*)((s32)gp + 0x18) - *(u64*)&time_508) / (*(u32*)0x800000F8 / 4000))

    switch (*state) {
        case 0:
            START_FADE(10, dat_804207c0);
            DVDMgrSetupCallback(psndPushGroup);
            (*state)++;
            break;
        case 1:
            (*state)++;
            break;
        case 2:
            if (fadeIsFinish()) {
                (*state)++;
            }
            break;
        case 3:
            if (*(s32*)((s32)wp + 0x18) == 0) {
                *(u32*)gp |= 4;
                SAVE_TIME();
                (*state)++;
            }
            break;
        case 4:
            START_FADE(10, dat_804207c4);
            (*state)++;
            *(s32*)((s32)wp + 4) = 0;
            break;
        case 5:
            elapsed = ELAPSED_MS();
            if (elapsed > 2000) {
                (*state)++;
                *(s32*)((s32)wp + 4) = 1;
            }
            break;
        case 6:
            if (keyGetButtonTrg(0) != 0 || ELAPSED_MS() > 59000) {
                (*state)++;
            }
            break;
        case 7:
            START_FADE(9, dat_804207c8);
            (*state)++;
            break;
        case 8:
            if (fadeIsFinish()) {
                START_FADE(10, dat_804207cc);
                (*state)++;
                *(s32*)((s32)wp + 4) = 2;
            }
            break;
        case 9:
            if (fadeIsFinish()) {
                SAVE_TIME();
                (*state)++;
            }
            break;
        case 10:
            if (ELAPSED_MS() > 1000) {
                START_FADE(9, dat_804207d0);
                (*state)++;
            }
            break;
        case 11:
            if (fadeIsFinish()) {
                START_FADE(10, dat_804207d4);
                (*state)++;
                *(s32*)((s32)wp + 4) = 3;
            }
            break;
        case 12:
            if (fadeIsFinish()) {
                (*state)++;
                SAVE_TIME();
            }
            break;
        case 13:
            if (ELAPSED_MS() > 1000) {
                START_FADE(9, dat_804207d8);
                (*state)++;
            }
            break;
        case 14:
            if (fadeIsFinish()) {
                START_FADE(10, dat_804207dc);
                (*state)++;
                *(s32*)((s32)wp + 4) = 4;
            }
            break;
        case 15:
            if (fadeIsFinish()) {
                (*state)++;
                SAVE_TIME();
            }
            break;
        case 16:
            if (ELAPSED_MS() > 1000) {
                START_FADE(9, dat_804207e0);
                *(s32*)((s32)wp + 0x10) = 0;
                (*state)++;
            }
            break;
        case 17:
            if (keyGetButtonTrg(0) & 0x1100) {
                *(s32*)((s32)wp + 0x10) = 1;
            }
            if (psndPushGroup() == 0) {
                DVDMgrSetupCallback(0);
                if (fadeIsFinish()) {
                    VIConfigure((*(u32*)gp & 8) ? GXNtsc480ProgMarioSt : GXNtsc480IntDfMarioSt);
                    VIFlush();
                    VISetBlack(1);
                    dispEntry(8, 0, dummy_draw, 0, float_0_80420824);
                    (*state)++;
                }
            }
            break;
        case 18:
            VISetBlack(0);
            VIWaitForRetrace();
            if (*(u32*)gp & 0x1000) {
                seqSetSeq(6, 0, 0);
                return 0;
            }
            if (*(s32*)((s32)wp + 0x10) == 0) {
                seqSetSeq(3, str_dmo_00_802c3b60, 0);
                return 0;
            }
            START_FADE(2, dat_804207e4);
            (*state)++;
            break;
        case 19:
            if (fadeIsFinish()) {
                seqSetSeq(1, 0, 0);
            }
            break;
    }

    if (*state > 1 && *state < 4) {
        dispEntry(4, 0, progDisp, 0, float_0_80420824);
    }
    if (*state > 3 && *state < 18) {
        dispEntry(4, 0, logoDisp, (void*)param_1, float_1_80420818);
    }

#undef ELAPSED_MS
#undef SAVE_TIME
#undef START_FADE
    return 0;
}

void progDisp(s32 camId) {
    extern void* camGetPtr(s32 camId);
    extern u64 OSGetTime(void);
    extern u32 keyGetButtonTrg(s32 pad);
    extern u32 keyGetDirTrg(s32 pad);
    extern s32 romFontGetMessage(s32 id);
    extern s32 romFontGetWidth(s32 msg, ...);
    extern void romFontPrintGX();
    extern void VISetBlack(s32 black);
    extern void OSSetProgressiveMode(s32 progressive);
    extern void VIConfigure(void* mode);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void* wp;
    extern void* gp;
    extern u8 sRMObjHReso_prog[];
    extern u32 dat_80420800;
    extern u32 dat_80420804;
    extern u32 dat_80420808;
    extern u32 dat_8042080c;
    extern f32 float_0p5_80420810;
    extern f32 float_neg80_80420814;
    extern f32 float_1_80420818;
    extern f32 float_neg160_8042081c;
    extern f32 float_neg100_80420820;
    u64 now;
    u32 buttons;
    s32 msg;
    s32 width;
    u32 color;
    f32 x;
    s32 state;

    camGetPtr(camId);

    if (*(s32*)((s32)wp + 0x18) == 0) {
        return;
    }

    state = *(s32*)((s32)wp + 0x14);
    switch (state) {
        case 0:
            *(u64*)((s32)wp + 8) = OSGetTime();
            *(s32*)((s32)wp + 0x14) = state + 1;
            break;

        case 1:
            buttons = keyGetButtonTrg(0);
            if (*(u32*)((s32)gp + 0x1378) == 0 &&
                *(u32*)((s32)gp + 0x1328) == 0 &&
                *(s8*)((s32)gp + 0x13B8) == 0 &&
                *(s8*)((s32)gp + 0x13BC) == 0 &&
                *(s8*)((s32)gp + 0x13C0) == 0 &&
                *(s8*)((s32)gp + 0x13C4) == 0 &&
                *(u8*)((s32)gp + 0x13C8) == 0 &&
                *(u8*)((s32)gp + 0x13CC) == 0) {
                now = OSGetTime();
                if ((now - *(u64*)((s32)wp + 8)) / (*(u32*)0x800000F8 >> 2) > 10) {
                    buttons = 0x100;
                }
            } else {
                *(u64*)((s32)wp + 8) = OSGetTime();
            }

            if ((buttons & 3) != 0 || (keyGetDirTrg(0) & 0xC000) != 0) {
                *(s32*)((s32)wp + 0x1C) = 1 - *(s32*)((s32)wp + 0x1C);
            } else if ((buttons & 0x100) != 0) {
                *(s32*)((s32)wp + 0x14) += 1;
            }

            msg = romFontGetMessage(0);
            width = romFontGetWidth(msg);
            color = dat_80420800;
            romFontPrintGX(-((f32)width * float_0p5_80420810), float_neg80_80420814,
                           float_1_80420818, &color, msg);

            msg = romFontGetMessage(1);
            width = romFontGetWidth(msg);
            color = dat_80420804;
            romFontPrintGX(-((f32)width * float_0p5_80420810), float_neg160_8042081c,
                           float_1_80420818, &color, msg);

            msg = romFontGetMessage(2);
            width = romFontGetWidth(msg);
            color = dat_80420808;
            x = ((f32)((*(s32*)((s32)wp + 0x1C) * 2 - 1) * 0x6A)) - ((f32)width * float_0p5_80420810);
            romFontPrintGX(x, float_neg160_8042081c, float_1_80420818, &color, msg);
            break;

        case 2:
            if (*(s32*)((s32)wp + 0x1C) == 0) {
                VISetBlack(1);
                OSSetProgressiveMode(1);
                VIConfigure(sRMObjHReso_prog);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                *(u64*)((s32)wp + 8) = *(u64*)((s32)gp + 0x18);
                *(s32*)((s32)wp + 0x14) += 1;
                *(u32*)gp |= 8;
            } else {
                OSSetProgressiveMode(0);
                *(u64*)((s32)wp + 8) = *(u64*)((s32)gp + 0x18);
                *(s32*)((s32)wp + 0x14) += 2;
                *(u32*)gp &= ~8;
            }
            *(u32*)gp |= 4;
            break;

        case 3:
            if ((*(u64*)((s32)gp + 0x18) - *(u64*)((s32)wp + 8)) / (*(u32*)0x800000F8 / 4000) > 2000) {
                VISetBlack(0);
                VIFlush();
                *(u64*)((s32)wp + 8) = *(u64*)((s32)gp + 0x18);
                *(s32*)((s32)wp + 0x14) += 1;
            }
            break;

        case 4:
            if ((*(u64*)((s32)gp + 0x18) - *(u64*)((s32)wp + 8)) / (*(u32*)0x800000F8 / 4000) > 2000) {
                *(s32*)((s32)wp + 0x18) = 0;
                *(s32*)((s32)wp + 0x14) += 1;
            }
            msg = romFontGetMessage(*(s32*)((s32)wp + 0x1C) != 0 ? 4 : 3);
            width = romFontGetWidth(msg, wp);
            color = dat_8042080c;
            romFontPrintGX(-((f32)width * float_0p5_80420810), float_neg100_80420820,
                           float_1_80420818, &color, msg);
            break;
    }
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

