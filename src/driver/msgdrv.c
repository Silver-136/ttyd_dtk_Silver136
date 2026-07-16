#include "driver/msgdrv.h"

void* msgWork;
extern void* gp;

void* windowGetPointer(s32 id);
void smartFree(void* ptr);
void* animGetPtr(void* anim);
void GXSetZMode(s32 enable, s32 func, s32 update);
void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
void GXSetZCompLoc(s32 beforeTex);
void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);

void* L_msgGetWork(void) {
    return &msgWork;
}

s32 _ismbblead(s32 c) {
    u32 value;

    if (*(void**)((s32)gp + 0x16C) != 0) {
        return 0;
    }

    value = (u8)c;
    if ((value >= 0x81 && value <= 0x9F) || (value >= 0xE0 && value <= 0xFC)) {
        return 1;
    }
    return 0;
}

void msgWindow_Continue(s32 id) {
    void* win = windowGetPointer(id);
    void* obj;
    void* work;
    s32 x;
    s32 y;

    *(u16*)win = 1;
    *(u16*)((s32)win + 2) |= 2;
    work = gp;
    obj = *(void**)((s32)win + 0x28);
    x = *(s32*)((s32)work + 0x38);
    y = *(s32*)((s32)work + 0x3C);
    obj = *(void**)obj;
    *(s32*)((s32)obj + 0x34) = y;
    *(s32*)((s32)obj + 0x30) = x;
}

void msgWindow_Delete(void* win) {
    void* ptr = *(void**)((s32)win + 0x28);

    if (ptr != 0) {
        s32 count = *(s32*)(*(s32*)ptr + 0x38);

        while (count > 0) {
            count--;
        }
        smartFree(ptr);
        *(void**)((s32)win + 0x28) = 0;
    }
}

void msgDispKeyWait_render(void* anim) {
    void* ptr = animGetPtr(anim);

    GXSetZMode(1, 7, 0);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    *(void**)((s32)ptr + 0xE0) = anim;
}


u8 msgDisp(f64 baseX, f64 baseY, s32* smart, u8 alpha) {
    extern void* gp;
    extern void FontDrawStart_alpha(u8);
    extern u32 FontGetDrawColor(void);
    extern void FontDrawColor(void*);
    extern void FontDrawColor_(void);
    extern void FontDrawNoise(void);
    extern void FontDrawNoiseOff(void);
    extern void FontDrawScale(f32);
    extern void FontDrawScaleVec(void*);
    extern void FontDrawCode(f64, f64, u16);
    extern void FontDrawCodeMtx(void*, u16);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern void FontDrawRainbowColor(void);
    extern void FontDrawRainbowColorOff(void);
    extern void FontDrawColorIDX(s32);
    extern u16 kanjiGetWidth(u32);
    extern s32 irand(s32);
    extern void iconDispGxAlpha(f64, void*, s32, u16, u8);
    extern void PSMTXIdentity(void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);
    extern void PSMTXScaleApply(void*, void*, f32, f32, f32);
    extern void animPoseMain(s32);
    extern void animPoseDrawMtx(s32, void*, s32, f32, f32);
    extern double sin(double);
    extern double cos(double);
    extern f32 float_0_80420600;
    extern f32 float_1_80420618;
    extern f32 float_0p5_80420620;
    extern f32 float_20_8042077c;
    extern f32 float_40_80420608;
    extern f32 float_90_80420770;
    extern f32 float_270_80420774;
    extern f32 float_neg1_80420778;
    extern f32 float_12_80420780;
    extern f32 float_0p01_80420788;
    extern f32 float_0p02_8042078c;
    extern f32 float_3p2_80420790;
    extern f32 float_4_8042076c;
    s32 work = *smart;
    u32* entry = (u32*)(work + 0x3C);
    s32 count = *(s32*)(work + 0x10);
    s32 i;
    u8 result = 0;

    FontDrawStart_alpha(alpha);
    for (i = 0; i < count; i++, entry += 6) {
        u32 flags = entry[0];
        u16 code = *(u16*)((u8*)entry + 4);
        f32 x = (f32)baseX + *(s16*)((u8*)entry + 6);
        f32 y = (f32)baseY + *(s16*)((u8*)entry + 8) + *(s32*)(work + 0x28);
        f32 scale = *(f32*)((u8*)entry + 0x10);

        switch (code) {
            case 0xFFF4: {
                f32 mtx[12];
                u32 color = FontGetDrawColor();
                PSMTXIdentity(mtx);
                if (*(f32*)((u8*)entry + 0x14) >= float_90_80420770 &&
                    *(f32*)((u8*)entry + 0x14) <= float_270_80420774) {
                    PSMTXScaleApply(mtx, mtx, float_1_80420618,
                                    float_1_80420618, float_neg1_80420778);
                }
                PSMTXTransApply(mtx, mtx, x, y, (f32)*(s32*)(work + 0x28));
                animPoseMain(entry[0]);
                animPoseDrawMtx(entry[0], mtx, 2,
                                *(f32*)((u8*)entry + 0x14), scale);
                FontDrawStart_alpha(alpha);
                FontDrawColor(&color);
                break;
            }
            case 0xFFF5:
            case 0xFFF9:
            case 0xFFFA:
            case 0xFFFB:
            case 0xFFFC:
            case 0xFFFD:
                break;
            case 0xFFF6:
                if (entry[0] == 0) FontDrawNoiseOff(); else FontDrawNoise();
                break;
            case 0xFFF7: {
                u32 color = entry[0];
                FontDrawColor(&color);
                break;
            }
            case 0xFFF8: {
                f32 pos[3];
                u32 color = FontGetDrawColor();
                if (*(s32*)(work + 0x28) + *(s16*)((u8*)entry + 8) - 40 +
                    *(s16*)((u8*)entry + 10) < 1) {
                    pos[0] = x + float_20_8042077c;
                    pos[1] = y + *(s16*)((u8*)entry + 10) - float_40_80420608;
                    pos[2] = float_0_80420600;
                    iconDispGxAlpha(scale, pos, 0x10, (u16)entry[0], alpha);
                    FontDrawStart_alpha(alpha);
                    FontDrawColor(&color);
                }
                break;
            }
            case 0xFFFE:
                if (*(u16*)(work + 8) == *(u16*)((u8*)entry + 6)) result = 1;
                break;
            case 0xFFFF:
                return 0;
            default:
                if ((flags & 1) != 0) {
                    f32 mtx[12];
                    s32 halfWidth = kanjiGetWidth(code) >> 1;
                    PSMTXTrans(mtx, (f32)-halfWidth, float_12_80420780,
                               float_0_80420600);
                    PSMTXScaleApply(mtx, mtx, *(f32*)((u8*)entry + 0x14),
                                    *(f32*)((u8*)entry + 0x14),
                                    *(f32*)((u8*)entry + 0x14));
                    PSMTXTransApply(mtx, mtx, x + halfWidth, y, float_0_80420600);
                    PSMTXScaleApply(mtx, mtx, scale, scale, scale);
                    FontDrawCodeMtx(mtx, code);
                    *(f32*)((u8*)entry + 0x14) =
                        float_0p5_80420620 *
                        (*(f32*)((u8*)entry + 0x14) - float_1_80420618) +
                        float_1_80420618;
                } else if ((flags & 2) != 0) {
                    FontDrawScale(scale);
                    FontDrawCode(x + (irand(10000) % 3),
                                 y + (irand(10000) % 3), code);
                } else if ((flags & 4) != 0) {
                    s64 elapsed = (s64)(*(u64*)((u8*)gp + 0x38) - *(u64*)(work + 0x18));
                    f32 phase = float_0p01_80420788 *
                                (f32)(elapsed / (*(u32*)0x800000F8 / 4000)) -
                                float_0p02_8042078c * x;
                    FontDrawScale(scale);
                    FontDrawCode(x + float_3p2_80420790 * (f32)cos(phase),
                                 y + float_3p2_80420790 * (f32)sin(phase), code);
                } else if ((flags & (0x100 << (*(s8*)(work + 0xF243) & 31))) != 0) {
                    s64 elapsed = (s64)(*(u64*)((u8*)gp + 0x38) - *(u64*)(work + 0x18));
                    f32 phase = float_0p01_80420788 *
                                (f32)(elapsed / (*(u32*)0x800000F8 / 4000)) -
                                float_0p02_8042078c * x;
                    f32 drawX = x + float_3p2_80420790 * (f32)cos(phase);
                    f32 drawY = y + float_3p2_80420790 * (f32)sin(phase);
                    u32 color = FontGetDrawColor();
                    FontDrawScale(scale);
                    FontDrawColor(&color);
                    FontDrawCode(drawX, drawY, code);
                    FontDrawEdge();
                    FontDrawRainbowColor();
                    FontDrawCode(drawX - float_4_8042076c,
                                 drawY + float_4_8042076c, code);
                    FontDrawEdgeOff();
                    FontDrawRainbowColorOff();
                    FontDrawColorIDX(0);
                } else {
                    FontDrawScale(scale);
                    FontDrawCode(x, y, code);
                }
                break;
        }
    }
    return result;
}

s32 msgMain(int* param_1) {
    extern u32 keyGetButtonTrg(s32 pad);
    extern u32 psndSFXOn(s32 id);
    extern u32 psndSFXOnVol(s32 id, s8 volume);
    extern void* npcNameToPtr(char* name);
    extern void animPoseSetPaperAnimGroup(s32 poseId, char* name, s32 flags);
    extern void animPosePaperPeraOn(s32 poseId);
    extern void animPoseSetPaperAnim(s32 poseId, char* name);
    extern void* gp;
    extern u8 msgSEflag;
    extern char* paperPoseTbl[];
    extern char str_p_b_st_802c3424[];
    extern f32 float_0p36_80420794;

    int work = *param_1;
    s32 index;
    s32 elapsed;
    s32 result;
    s32 poseId;
    s32* event;
    u32 buttons;
    u32 accept;
    u64 now;
    void* npc;
    void (*callback)(s32, int*);

#define I32(o) (*(s32*)(work + (o)))
#define U32(o) (*(u32*)(work + (o)))
#define U16(o) (*(u16*)(work + (o)))
#define S16(o) (*(s16*)(work + (o)))
#define U8V(o) (*(u8*)(work + (o)))
#define SAVE_TIME() do { *(u64*)(work + 0x30) = *(u64*)((s32)gp + 0x18); } while (0)
#define ELAPSED_MS() ((s32)((*(u64*)((s32)gp + 0x18) - *(u64*)(work + 0x30)) / (*(u32*)0x800000F8 / 4000)))
#define CALLBACK(which) do { callback = *(void (**)(s32, int*))(work + 0xF1FC); if (callback != 0) callback((which), param_1); } while (0)

    msgSEflag = 0;

    if (U32(4) & 2) {
        CALLBACK(0);
        U32(4) &= ~2;
        if ((U32(4) & 0x200) == 0 && I32(0xF040) < 0x6E &&
            (I32(0xF040) == 0 || I32(0xF040 + I32(0xF040) * 4) != 0)) {
            index = I32(0xF040)++;
            I32(0xF044 + index * 4) = 0;
        }
    }

    if (I32(0x28) < I32(0x24)) {
        elapsed = ELAPSED_MS();
        I32(0x28) += (s32)(float_0p36_80420794 * (f32)elapsed);
        if ((U32(4) & 0x80) == 0) {
            accept = (U32(4) & 0x100) ? 0x500 : 0x100;
            if (keyGetButtonTrg(0) & (accept | 0x200)) {
                U32(4) |= 4;
            }
        }
        if ((U32(4) & 0x10) == 0 && (U32(4) & 4)) {
            I32(0x28) += (s32)(float_0p36_80420794 * (f32)elapsed);
        }
        if (I32(0x28) > I32(0x24)) {
            I32(0x28) = I32(0x24);
        }
        SAVE_TIME();
        return 3;
    }

    if (U16(10) == 2) {
        U16(12) = 1;
        S16(8)++;
        U16(10) = 0;
        psndSFXOn(0x2003D);
        I32(0x20) += 100;
        CALLBACK(3);
    } else {
        if (U16(10) == 1) {
            return 2;
        }
        elapsed = ELAPSED_MS();
        if ((U32(4) & 0x80) == 0) {
            accept = (U32(4) & 0x100) ? 0x500 : 0x100;
            if (keyGetButtonTrg(0) & (accept | 0x200)) {
                U32(4) |= 4;
            }
        }
        if ((U32(4) & 0x10) == 0 && (U32(4) & 4)) {
            elapsed *= 100;
        }
        I32(0x20) += elapsed;
    }

    SAVE_TIME();
    index = I32(0x10);
    event = (s32*)(work + 0x3C + index * 0x18);

    while (index < 0xA00 && (u32)event[3] <= U32(0x20)) {
        switch (*(u16*)((s32)event + 4)) {
            case 0xFFF2:
                npc = (void*)event[0];
                if (U16(10) == 0) {
                    if (npc == 0) {
                        npc = npcNameToPtr((char*)(work + 0xF204));
                    }
                    poseId = *(s32*)((s32)npc + 0x104);
                    animPoseSetPaperAnimGroup(poseId, 0, 1);
                } else {
                    index--;
                }
                break;

            case 0xFFF3:
                npc = (void*)event[0];
                if (npc == 0) {
                    npc = npcNameToPtr((char*)(work + 0xF204));
                }
                poseId = *(s32*)((s32)npc + 0x104);
                animPoseSetPaperAnimGroup(poseId, str_p_b_st_802c3424, 1);
                animPosePaperPeraOn(poseId);
                animPoseSetPaperAnim(poseId, paperPoseTbl[event[3]]);
                break;

            case 0xFFF4:
            case 0xFFF8:
                break;

            case 0xFFF5:
                if (event[0] == 0) {
                    U32(4) &= ~0x80;
                } else {
                    U32(4) |= 0x80;
                    U32(4) &= ~4;
                    U32(0x20) = event[3];
                }
                break;

            case 0xFFF9:
                U8V(0xF22C) = (u8)event[0];
                break;

            case 0xFFFA:
                U32(0xF228) = event[0];
                break;

            case 0xFFFB:
                CALLBACK(4);
                break;

            case 0xFFFC:
                I32(0x10) = index + 1;
                U32(0x20) = event[3];
                U32(4) &= ~4;
                return 1;

            case 0xFFFD:
                I32(0x24) -= *(s16*)((s32)event + 8);
                U32(0x20) = event[3];
                if ((U32(4) & 0x200) == 0 && I32(0xF040) < 0x6E &&
                    (I32(0xF040) == 0 || I32(0xF040 + I32(0xF040) * 4) != I32(0x24))) {
                    result = I32(0xF040)++;
                    I32(0xF044 + result * 4) = I32(0x24);
                }
                break;

            case 0xFFFE:
                if (*(s16*)((s32)event + 6) == (u16)U16(8)) {
                    U16(10) = 1;
                    U32(0x20) = event[3];
                    U32(4) &= ~4;
                    CALLBACK(2);
                }
                break;

            case 0xFFFF:
                CALLBACK(1);
                I32(0x10) = index;
                return 0;

            default:
                CALLBACK(5);
                if ((event[0] & 0x10) != 0 || (event[0] & 8) != 0) {
                    result = *(s32*)(*(s32*)(*param_1 + 0xF24C) + 8);
                    if ((event[0] & 8) && result != 3) {
                        I32(0xF250) = 15;
                    }
                    if (result != 1 && msgSEflag == 0) {
                        psndSFXOnVol(0x20044, *(s8*)(*param_1 + 0xF22C));
                        msgSEflag = 1;
                    }
                }
                break;
        }
        index++;
        event += 6;
    }

    I32(0x10) = index;
    return U16(10) == 0 ? 3 : 2;

#undef CALLBACK
#undef ELAPSED_MS
#undef SAVE_TIME
#undef U8V
#undef S16
#undef U16
#undef U32
#undef I32
}

void msgWindow_Disp(s32 unused, void* win) {
    extern void* camGetCurPtr(void);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void getScreenPoint(void* src, f32* dst);
    extern f32 intplGetValue(s32 type, s32 currStep, s32 steps, f32 start, f32 end);
    extern void windowDispGX_Message(f32 x, f32 y, f32 w, f32 h, f32 sx, f32 sy, s32 type, s32 flagsArg, u8 alphaArg);
    extern void windowDispGX_Kanban(f32 x, f32 y, f32 w, f32 h, s32 type, u8 alphaArg);
    extern void windowDispGX_System(f32 x, f32 y, f32 w, f32 h, s32 type, u8 alphaArg);
    extern void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
    extern u8 msgDisp();
    extern void animPoseMain(s32 poseId);
    extern void animPoseDraw(s32 poseId, double x, double y, double z, double rot, double scale, s32 modeArg);
    extern u32 unk_80429580;
    extern void* msgw;
    extern u16 active_772;
    extern f32 float_0_80420600;
    extern f32 float_0p5_80420620;
    extern f32 float_10_80420624;
    extern f32 float_240_80420628;
    extern f32 float_36_8042062c;
    extern f32 float_62_80420630;
    extern f32 float_80_80420634;
    extern f32 float_60_80420638;
    extern f32 float_28_8042063c;
    extern f32 float_6_80420640;
    extern f32 float_8_80420644;
    extern f32 float_2_80420610;
    extern f32 float_50_80420648;
    extern f32 float_52_8042064c;
    extern f32 float_31_80420650;
    extern f32 float_180_80420654;
    extern f32 float_5_80420658;

#define W_MODE (*(volatile s32*)((s32)win + 0x08))
#define W_STEP (*(volatile s16*)((s32)win + 0x06))
#define W_ALPHA ((u8)W_STEP)
#define W_FLAGS (*(volatile s16*)((s32)win + 0x2C))

    void* cam;
    u32 fogColor;
    f32 screen[3];
    f32 ix;
    f32 iy;
    f32 iw;
    f32 ih;
    s32 drew;
    u32 scissorY;
    u32 scissorH;
    s32 signedY;

    (void)unused;

    cam = camGetCurPtr();
    drew = 0;
    fogColor = unk_80429580;
    GXSetFog(0, float_0_80420600, float_0_80420600, float_0_80420600, float_0_80420600, &fogColor);


    switch (W_MODE) {
        case 0:
        case 1:
        case 6:
        case 8:
        case 9:
            if (W_FLAGS == 0) {
                windowDispGX_Message(
                    *(f32*)((s32)win + 0x0C),
                    *(f32*)((s32)win + 0x10),
                    *(f32*)((s32)win + 0x14),
                    *(f32*)((s32)win + 0x18),
                    float_0_80420600,
                    float_0_80420600,
                    W_MODE,
                    0,
                    W_ALPHA);
                scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c) - float_8_80420644);
                GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), 0x7E);
                drew = msgDisp(
                    float_62_80420630 + *(f32*)((s32)win + 0x0C),
                    *(f32*)((s32)win + 0x10) - float_36_8042062c,
                    *(void**)((s32)win + 0x28),
                    W_ALPHA);
            } else {
                if ((W_FLAGS & 0x10) != 0) {
                    screen[0] = *(f32*)((s32)win + 0x30);
                    screen[1] = *(f32*)((s32)win + 0x34);
                    screen[2] = *(f32*)((s32)win + 0x38);
                } else {
                    getScreenPoint((void*)((s32)win + 0x30), screen);
                }

                if (W_MODE == 1) {
                    *(f32*)((s32)win + 0x0C) = screen[0] - (*(f32*)((s32)win + 0x14) * float_0p5_80420620);
                    if ((W_FLAGS & 8) != 0) {
                        *(f32*)((s32)win + 0x10) = screen[1] - float_80_80420634;
                    } else {
                        *(f32*)((s32)win + 0x10) = float_60_80420638 + screen[1];
                    }

                    ix = intplGetValue(0, W_STEP, 0xFF, screen[0], *(f32*)((s32)win + 0x0C) + (*(f32*)((s32)win + 0x14) * float_0p5_80420620));
                    iy = intplGetValue(0, W_STEP, 0xFF, screen[1], *(f32*)((s32)win + 0x10) - (*(f32*)((s32)win + 0x18) * float_0p5_80420620));
                    iw = intplGetValue(0, W_STEP, 0xFF, float_10_80420624, *(f32*)((s32)win + 0x14));
                    ih = intplGetValue(0, W_STEP, 0xFF, float_10_80420624, *(f32*)((s32)win + 0x18));
                    windowDispGX_Message(
                        ix - (iw * float_0p5_80420620),
                        iy + (ih * float_0p5_80420620),
                        iw,
                        ih,
                        screen[0],
                        screen[1],
                        W_MODE,
                        W_FLAGS,
                        W_ALPHA);
                    if (W_STEP == 0xFF) {
                        signedY = 0xF0 - ((s32)*(f32*)((s32)win + 0x10) - 10);
                        if (signedY < 0) {
                            signedY = 0;
                        }
                        if (signedY > 0x1E0) {
                            signedY = 0x1E0;
                        }
                        GXSetScissor(*(u16*)((s32)cam + 0xF4), signedY, *(u16*)((s32)cam + 0xF8), 0x7E);
                        msgDisp(
                            float_28_8042063c + *(f32*)((s32)win + 0x0C),
                            *(f32*)((s32)win + 0x10) - float_6_80420640,
                            *(void**)((s32)win + 0x28),
                            W_ALPHA);
                    }
                } else if (W_MODE == 0 || W_MODE == 6 || W_MODE == 8 || W_MODE == 9) {
                    ix = intplGetValue(0, W_STEP, 0xFF, screen[0], *(f32*)((s32)win + 0x0C) + (*(f32*)((s32)win + 0x14) * float_0p5_80420620));
                    iy = intplGetValue(0, W_STEP, 0xFF, screen[1], *(f32*)((s32)win + 0x10) - (*(f32*)((s32)win + 0x18) * float_0p5_80420620));
                    iw = intplGetValue(0, W_STEP, 0xFF, float_10_80420624, *(f32*)((s32)win + 0x14));
                    ih = intplGetValue(0, W_STEP, 0xFF, float_10_80420624, *(f32*)((s32)win + 0x18));
                    windowDispGX_Message(
                        ix - (iw * float_0p5_80420620),
                        iy + (ih * float_0p5_80420620),
                        iw,
                        ih,
                        screen[0],
                        screen[1],
                        W_MODE,
                        W_FLAGS,
                        W_ALPHA);
                    if (W_STEP == 0xFF) {
                        scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c));
                        GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), 100);
                        drew = msgDisp(
                            float_62_80420630 + *(f32*)((s32)win + 0x0C),
                            *(f32*)((s32)win + 0x10) - float_36_8042062c,
                            *(void**)((s32)win + 0x28),
                            W_ALPHA);
                    }
                }
            }
            break;

        case 2:
        case 4:
        case 10:
            windowDispGX_Kanban(
                *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10),
                *(f32*)((s32)win + 0x14),
                *(f32*)((s32)win + 0x18),
                W_MODE,
                W_ALPHA);
            scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c) - float_2_80420610);
            GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), 0x68);
            drew = msgDisp(
                float_62_80420630 + *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10) - float_36_8042062c,
                *(void**)((s32)win + 0x28),
                W_ALPHA);
            break;

        case 3:
            windowDispGX_System(
                *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10),
                *(f32*)((s32)win + 0x14),
                *(f32*)((s32)win + 0x18),
                W_MODE,
                W_ALPHA);
            scissorH = (u32)(*(f32*)((s32)win + 0x18) - float_50_80420648);
            scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c) - float_8_80420644);
            GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), scissorH);
            drew = msgDisp(
                float_62_80420630 + *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10) - float_36_8042062c,
                *(void**)((s32)win + 0x28),
                W_ALPHA);
            break;

        case 7:
            windowDispGX_Kanban(
                *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10),
                *(f32*)((s32)win + 0x14),
                *(f32*)((s32)win + 0x18),
                W_MODE,
                W_ALPHA);
            scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c) - float_8_80420644);
            GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), 0x7E);
            drew = msgDisp(
                float_62_80420630 + *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10) - float_36_8042062c,
                *(void**)((s32)win + 0x28),
                W_ALPHA);
            break;

        case 11:
            scissorH = (u32)(*(f32*)((s32)win + 0x18) - float_50_80420648);
            scissorY = (u32)(float_240_80420628 - (*(f32*)((s32)win + 0x10) - float_36_8042062c) - float_8_80420644);
            GXSetScissor(*(u16*)((s32)cam + 0xF4), scissorY, *(u16*)((s32)cam + 0xF8), scissorH);
            drew = msgDisp(
                float_62_80420630 + *(f32*)((s32)win + 0x0C),
                *(f32*)((s32)win + 0x10) - float_36_8042062c,
                *(void**)((s32)win + 0x28),
                W_ALPHA);
            break;
    }

    GXSetScissor(
        *(u16*)((s32)cam + 0xF4),
        *(u16*)((s32)cam + 0xF6),
        *(u16*)((s32)cam + 0xF8),
        *(u16*)((s32)cam + 0xFA));

    if (drew != 0) {
        if (active_772 == 0) {
            active_772 = 1;
        }
        animPoseMain(*(s32*)((s32)msgw + 0x20));
        animPoseDraw(
            *(s32*)((s32)msgw + 0x20),
            (*(f32*)((s32)win + 0x0C) + *(f32*)((s32)win + 0x14)) - float_52_8042064c,
            float_31_80420650 + (*(f32*)((s32)win + 0x10) - *(f32*)((s32)win + 0x18)),
            float_0_80420600,
            float_180_80420654,
            float_5_80420658,
            2);
    }
#undef W_MODE
#undef W_STEP
#undef W_ALPHA
#undef W_FLAGS
}

int msgWindow_Entry(char* text, s32 param_2, short kind) {
    extern s32 windowEntry(short kind);
    extern void* windowGetPointer(s32 id);
    extern u32 strlen(char* str);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char* strcpy(char* dst, char* src);
    extern void* smartAlloc(void* ptr, s32 flag);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void msgAnalize(void* smart, void* text);
    extern void __memFree(s32 heap, void* ptr);
    extern void* getWakuTexObj(s32 id);
    extern u32 GXGetTexObjWidth(void* obj);
    extern u32 GXGetTexObjHeight(void* obj);
    extern u8 msgWindow_Main(void* win);
    extern void msgWindow_Delete(void* win);
    extern u8 selectWindow_Main(s32 win);
    extern u8 msgWindow_Clear_Main(void* win);
    extern f32 float_neg280_80420660;
    extern f32 float_225_80420664;
    extern f32 float_560_80420668;
    extern f32 float_176_8042066c;
    extern f32 float_0p9_80420670;
    s32 id = windowEntry(kind);
    void* win = windowGetPointer(id);
    char* copy;
    void* smart;
    char* work;
    s32 mode;
    void* texObj;

    *(void**)((s32)win + 0x3C) = msgWindow_Main;
    *(void**)((s32)win + 0x40) = msgWindow_Delete;
    *(u16*)win = 5;
    *(s32*)((s32)win + 8) = -1;
    *(u16*)((s32)win + 2) |= 2;

    copy = __memAlloc(0, strlen(text));
    strcpy(copy, text);
    smart = smartAlloc(copy, 1);
    work = *(char**)smart;
    memset(work, 0, 0xF258);

    *(void**)(work + 0xF24C) = win;
    *(s32*)(work + 4) = 3;
    *(s32*)(work + 0xF1FC) = param_2;
    *(s32*)(work + 0x34) = *(s32*)((s32)gp + 0x3C);
    *(s32*)(work + 0x30) = *(s32*)((s32)gp + 0x38);
    *(s32*)(work + 0x1C) = *(s32*)((s32)gp + 0x3C);
    *(s32*)(work + 0x18) = *(s32*)((s32)gp + 0x38);
    *(s32*)(work + 0xF228) = 0;
    *(u8*)(work + 0xF22C) = 0x7F;
    *(s32*)(work + 0xF230) = 0;
    *(u16*)(work + 0xF234) = 0;
    *(u16*)(work + 0xF236) = 0;
    *(u16*)(work + 0xF238) = 0;
    *(u16*)(work + 0xF23A) = 0;
    *(s32*)(work + 0xF23C) = 0x19;
    *(s32*)(work + 0xF040) = 0;

    msgAnalize(smart, copy);
    if (*(s32*)((s32)win + 8) == 1) {
        *(u32*)(work + 4) |= 0x200;
    }
    __memFree(0, copy);

    *(void**)((s32)win + 0x28) = smart;
    work = **(char***)((s32)win + 0x28);
    *(s32*)(work + 0xF250) = 0;
    *(s32*)(work + 0xF254) = 0;

    mode = *(s32*)((s32)win + 8);
    if (mode == 5) {
        *(void**)((s32)win + 0x3C) = selectWindow_Main;
        *(f32*)((s32)win + 0x0C) = (f32)(s16)(-(*(s16*)(work + 0xF244) / 2));
        *(f32*)((s32)win + 0x10) = (f32)*(s16*)(work + 0xF246);
        *(f32*)((s32)win + 0x14) = (f32)*(s16*)(work + 0xF244);
        *(f32*)((s32)win + 0x18) = (f32)(s16)((*(s8*)(work + 0xF240) * 0x24) + 0x28);
        *(u8*)(work + 0xF243) = *(u8*)(work + 0xF241);
        *(f32*)(work + 0xF248) = *(f32*)((s32)win + 0x10) - (f32)((*(s8*)(work + 0xF243) * 0x1F) + 0x3A);
        return id;
    }

    if (mode < 5) {
        if (mode == 1) {
            *(s32*)((s32)win + 4) = kind + 10;
            texObj = getWakuTexObj(3);
            *(f32*)((s32)win + 0x14) = float_0p9_80420670 * (f32)(u16)GXGetTexObjWidth(texObj);
            texObj = getWakuTexObj(3);
            *(f32*)((s32)win + 0x18) = float_0p9_80420670 * (f32)(u16)GXGetTexObjHeight(texObj);
            return id;
        }
        if (mode < 0) {
            return id;
        }
    } else if (mode == 0xB) {
        *(void**)((s32)win + 0x3C) = msgWindow_Clear_Main;
    } else if (mode > 10) {
        return id;
    }

    if ((*(u16*)((s32)win + 2) & 4) == 0) {
        *(f32*)((s32)win + 0x0C) = float_neg280_80420660;
        *(f32*)((s32)win + 0x10) = float_225_80420664;
        *(f32*)((s32)win + 0x14) = float_560_80420668;
        *(f32*)((s32)win + 0x18) = float_176_8042066c;
    }
    return id;
}

void selectWindow_Main(void* win) {
    extern s32 msgMain(void* work);
    extern u32 keyGetButtonTrg(s32 pad);
    extern u32 keyGetDirTrg(s32 pad);
    extern u32 keyGetButtonRep(s32 pad);
    extern u32 keyGetDirRep(s32 pad);
    extern void psndSFXOn(u32 id);
    extern void dispEntry(s32 camId, s32 order, void* callback, void* param, f32 prio);
    extern u8 selectWindow_Disp(s32 param_1, int param_2);
    extern f32 float_400_8042061c;
    void* smart = *(void**)((s32)win + 0x28);
    char* work = *(char**)smart;
    s8 oldSelect;
    s8 value;

    msgMain(smart);

    switch (*(u16*)win) {
        case 5:
            *(s16*)((s32)win + 6) += 0x19;
            if (*(s16*)((s32)win + 6) > 0xFF) {
                *(s16*)((s32)win + 6) = 0xFF;
                *(u16*)win = 1;
            }
            break;
        case 1:
            oldSelect = *(s8*)(work + 0xF243);
            if (((keyGetButtonTrg(0) & 8) != 0) || ((keyGetDirTrg(0) & 0x1000) != 0)) {
                *(u8*)(work + 0xF243) = *(u8*)(work + 0xF243) - 1;
            } else if (((keyGetButtonRep(0) & 8) != 0) || ((keyGetDirRep(0) & 0x1000) != 0)) {
                if (*(s8*)(work + 0xF243) != 0) {
                    *(u8*)(work + 0xF243) = *(u8*)(work + 0xF243) - 1;
                }
            }

            if (((keyGetButtonTrg(0) & 4) != 0) || ((keyGetDirTrg(0) & 0x2000) != 0)) {
                *(u8*)(work + 0xF243) = *(u8*)(work + 0xF243) + 1;
            } else if (((keyGetButtonRep(0) & 4) != 0) || ((keyGetDirRep(0) & 0x2000) != 0)) {
                if (*(s8*)(work + 0xF243) < (*(s8*)(work + 0xF240) - 1)) {
                    *(u8*)(work + 0xF243) = *(u8*)(work + 0xF243) + 1;
                }
            }

            if (*(s8*)(work + 0xF243) < 0) {
                *(u8*)(work + 0xF243) = *(u8*)(work + 0xF240) - 1;
            }
            if (*(s8*)(work + 0xF243) >= *(s8*)(work + 0xF240)) {
                *(u8*)(work + 0xF243) = 0;
            }
            if (oldSelect != *(s8*)(work + 0xF243)) {
                psndSFXOn(0x20005);
            }

            if ((keyGetButtonTrg(0) & 0x200) != 0) {
                value = *(s8*)(work + 0xF242);
                if (value != -1) {
                    if (value < -1 && value > -3) {
                        *(u8*)(work + 0xF243) = 0xFF;
                    } else {
                        *(s8*)(work + 0xF243) = value;
                    }
                    *(u16*)win = 7;
                    psndSFXOn(0x20013);
                }
            } else if ((keyGetButtonTrg(0) & 0x100) != 0) {
                *(u16*)win = 7;
                psndSFXOn(0x20012);
            }
            break;
        case 7:
            if (*(s16*)((s32)win + 6) == 0) {
                *(u16*)win = 4;
                *(u16*)((s32)win + 2) &= ~2;
                return;
            }
            *(s16*)((s32)win + 6) -= 0x19;
            if (*(s16*)((s32)win + 6) < 1) {
                *(s16*)((s32)win + 6) = 0;
            }
            break;
        default:
            if (*(u16*)win >= 4) {
                return;
            }
            break;
    }

    dispEntry(8, 0, selectWindow_Disp, win, float_400_8042061c - (f32)*(s16*)((s32)win + 4));
}

void msgLoad(char* roomName, s32 index) {
    typedef struct MsgFileLocal {
        char* data;
        u32 length;
        s32 count;
        void* lookup;
    } MsgFileLocal;
    extern void* msgWork;
    extern void* gp;
    extern char* language_dir[];
    extern char str_PCTs_PCTs_txt_802c342c[];
    extern char str_PCTs_PCTs_80420798[];
    extern s32 sprintf(char*, const char*, ...);
    extern s32 getMarioStDvdRoot(void);
    extern void* DVDMgrOpen(const char*, s32, s32);
    extern u32 DVDMgrGetLength(void*);
    extern void DVDMgrRead(void*, void*, u32, u32);
    extern void DVDMgrClose(void*);
    extern void* __memAlloc(s32, u32);
    extern void* _mapAlloc(void*, u32);
    extern void* mapalloc_base_ptr;
    extern u32 strlen(const char*);
    extern void smartFree(void*);
    extern void* smartAlloc(void*, s32);
    extern void qsort(void*, u32, u32, void*);
    extern s32 msg_compare(s32*, s32*);
    extern char* msgSortDvdData;
    MsgFileLocal* file = ((MsgFileLocal*)&msgWork) + index;
    char filepath[64];
    char dvdPath[136];
    void* entry;
    char* cursor;
    s32 maxLength;
    s32 i;
    s32 offset;
    s32* lookup;

    if (file->data == 0 && index == 1) {
        maxLength = 0;
        for (i = 0; i < 7; i++) {
            sprintf(filepath, str_PCTs_PCTs_txt_802c342c, language_dir[i], roomName);
            sprintf(dvdPath, str_PCTs_PCTs_80420798, getMarioStDvdRoot(), filepath);
            entry = DVDMgrOpen(dvdPath, 2, 0);
            if (entry != 0) {
                s32 length = DVDMgrGetLength(entry);
                DVDMgrClose(entry);
                if (maxLength < length) {
                    maxLength = length;
                }
            }
        }
        file->data = __memAlloc(0, (maxLength + 0x1F) & ~0x1F);
    }

    file->length = 0;
    sprintf(filepath, str_PCTs_PCTs_txt_802c342c,
            language_dir[*(s32*)((u8*)gp + 0x16C)], roomName);
    sprintf(dvdPath, str_PCTs_PCTs_80420798, getMarioStDvdRoot(), filepath);
    entry = DVDMgrOpen(dvdPath, 2, 0);
    if (entry != 0) {
        file->length = DVDMgrGetLength(entry);
        if (index >= 0 && index < 1) {
            file->data = _mapAlloc(mapalloc_base_ptr, (file->length + 0x1F) & ~0x1F);
        }
        DVDMgrRead(entry, file->data, (file->length + 0x1F) & ~0x1F, 0);
        DVDMgrClose(entry);
    }

    cursor = file->data;
    if (cursor == 0) {
        file->length = 0;
        file->count = 0;
    } else {
        i = 0;
        while (*cursor != 0) {
            cursor += strlen(cursor) + 1;
            i++;
        }
        file->count = i / 2;
    }
    if (file->lookup != 0) {
        smartFree(file->lookup);
    }
    if (file->count == 0) {
        file->lookup = 0;
        return;
    }

    file->lookup = smartAlloc(file->lookup, 0);
    cursor = file->data;
    lookup = *(s32**)file->lookup;
    offset = 0;
    for (i = 0; i < file->count; i++) {
        lookup[0] = offset;
        offset += strlen(cursor) + 1;
        cursor += strlen(cursor) + 1;
        lookup[1] = offset;
        offset += strlen(cursor) + 1;
        cursor += strlen(cursor) + 1;
        lookup += 2;
    }
    msgSortDvdData = file->data;
    qsort(*(void**)file->lookup, file->count, 8, msg_compare);
}

void selectWindow_Disp(s32 unused, void* win) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void windowDispGX_Waku_col(f32, f32, f32, f32, f32, s32, void*);
    extern u8 msgDisp(f64, f64, s32*, u8);
    extern void iconDispGxAlpha(f64, void*, s32, u16, u8);
    extern u32 unk_80429584;
    extern u32 dat_804205fc;
    extern f32 float_0_80420600;
    extern f32 float_30_80420604;
    extern f32 float_40_80420608;
    extern f32 float_25_8042060c;
    extern f32 float_2_80420610;
    extern f32 float_0p3_80420614;
    extern f32 float_1_80420618;
    u8* window = win;
    u8* work = **(u8***)(window + 0x28);
    u32 fogColor = unk_80429584;
    u32 frameColor = (dat_804205fc & 0xFFFFFF00) | *(u16*)(window + 6);
    VecLocal iconPos;
    f32 targetY;

    (void)unused;
    GXSetFog(0, float_0_80420600, float_0_80420600,
             float_0_80420600, float_0_80420600, &fogColor);
    windowDispGX_Waku_col(*(f32*)(window + 0x0C), *(f32*)(window + 0x10),
                          *(f32*)(window + 0x14), *(f32*)(window + 0x18),
                          float_30_80420604, 0, &frameColor);
    msgDisp(float_40_80420608 + *(f32*)(window + 0x0C),
            *(f32*)(window + 0x10) - float_25_8042060c,
            *(s32**)(window + 0x28), (u8)*(u16*)(window + 6));

    iconPos.x = float_2_80420610 + *(f32*)(window + 0x0C);
    iconPos.z = float_0_80420600;
    targetY = *(f32*)(window + 0x10) -
              (f32)((u8)work[0xF243] * 0x1F + 0x3A);
    *(f32*)(work + 0xF248) +=
        float_0p3_80420614 * (targetY - *(f32*)(work + 0xF248));
    iconPos.y = *(f32*)(work + 0xF248);
    iconDispGxAlpha(float_1_80420618, &iconPos, 0x14, 0x1F8,
                    (u8)*(u16*)(window + 6));
}

void* msgSearch(void* msgId) {
    extern void* msgWork;
    extern void* gp;
    extern s32 strcmp(const char*, const char*);
    extern char* ErrMessage[];
    u8* files = (u8*)&msgWork;
    char* key = msgId;
    s32 fileIndex;

    for (fileIndex = 0; fileIndex < 2; fileIndex++, files += 0x10) {
        void* smart = *(void**)(files + 0x0C);
        if (smart != 0) {
            s32* lookup = *(s32**)smart;
            s32 low = 0;
            s32 high = *(s32*)(files + 8);
            while (low <= high) {
                s32 middle = (low + high) / 2;
                s32 count = *(s32*)(files + 8);
                s32 result;
                if (middle >= count) {
                    break;
                }
                result = strcmp(key, (char*)*(void**)files + lookup[middle * 2]);
                if (result == 0) {
                    return (char*)*(void**)files + lookup[middle * 2 + 1];
                }
                if (result < 0) {
                    high = middle - 1;
                } else {
                    low = middle + 1;
                }
            }
        }
    }
    return ErrMessage[*(s32*)((u8*)gp + 0x16C)];
}

void msgWindow_ForceClose(s32 id) {
    extern s32 strcmp(const char* a, const char* b);
    extern void BtlUnit_ChangeStayAnim(void* unit);
    extern void npcSetStayPose(void* npc);
    s32 i;
    void* win;
    void* obj;

    for (i = 0; i < 7; i++) {
        win = windowGetPointer(i);
        if (*(u16*)((s32)win + 2) != 0 && *(u16*)win != 0 && *(void**)((s32)win + 0x28) != NULL) {
            obj = *(void**)(*(void**)((s32)win + 0x28));
            if (*(void**)obj != NULL && strcmp(*(char**)obj, (char*)id) == 0) {
                *(u16*)win = 7;
                obj = *(void**)(*(void**)((s32)win + 0x28));
                if (*(u32*)((s32)obj + 4) & 8) {
                    BtlUnit_ChangeStayAnim(*(void**)((s32)obj + 0xF224));
                } else {
                    npcSetStayPose((void*)((s32)obj + 0xF204));
                }
            }
        }
    }
}

u8 msgInit(void) {
    extern void* msgw;
    extern s32 animPoseEntry(const char* name, s32 type);
    extern void animPoseSetAnim(s32 pose, const char* name, s32 loop);
    extern void animPoseSetMaterialFlagOn(s32 pose, s32 flags);
    extern void animPoseSetMaterialFlagOff(s32 pose, s32 flags);
    extern void animPoseSetGXFunc(s32 pose, void* func, s32 param);
    extern const char R_no_messages_JP[];
    char* base = (char*)R_no_messages_JP;
    void* work;
    s32 pose;

    pose = animPoseEntry(base + 0x390, 2);
    *(s32*)((s32)msgw + 0x20) = pose;
    animPoseSetAnim(*(s32*)((s32)msgw + 0x20), base + 0x398, 1);
    animPoseSetMaterialFlagOn(*(s32*)((s32)msgw + 0x20), 0x1800);
    animPoseSetMaterialFlagOff(*(s32*)((s32)msgw + 0x20), 0x40);
    animPoseSetGXFunc(*(s32*)((s32)msgw + 0x20), msgDispKeyWait_render, 0);
    work = msgw;
    *(s32*)((s32)work + 4) = 0;
    *(s32*)((s32)msgw + 0xC) = 0;
    *(s32*)((s32)msgw + 0x14) = 0;
    *(s32*)((s32)msgw + 0x1C) = 0;
    msgLoad(base + 0x3A0, 1);
}

char* msgGetCommand(char* param_1, char* param_2, char* param_3) {
    s8 c;
    s32 mode = 0;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        c = *(s8*)param_1;
        switch (c) {
            case '>':
                if (mode == 0) {
                    *param_2 = 0;
                } else {
                    *param_3 = 0;
                }
                return param_1 + 1;
            case ' ':
                if (mode == 0) {
                    *param_2 = 0;
                    mode = 1;
                } else {
                    *param_3++ = c;
                }
                break;
            default:
                if (mode == 0) {
                    *param_2++ = c;
                } else {
                    *param_3++ = c;
                }
                break;
        }
        param_1++;
    }
    return NULL;
}

s32 msgIconStr2ID(char* param_1) {
    typedef struct MsgIcon {
        const char* name;
        s32 id;
    } MsgIcon;
    extern MsgIcon msgIcon[];
    extern s32 strcmp(const char* a, const char* b);
    register MsgIcon* icon = msgIcon;
    u32 i;

    for (i = 0; i < 0x12; i++, icon++) {
        if (strcmp(icon->name, param_1) == 0) {
            return msgIcon[i].id;
        }
    }
    return -1;
}

void msgWindow_Repeat(s32 id) {
    extern void* windowGetPointer(s32);
    extern void* gp;
    u8* window = windowGetPointer(id);
    u8* work = **(u8***)(window + 0x28);
    u64 now = *(u64*)((u8*)gp + 0x38);

    *(u16*)window = 1;
    *(u16*)(window + 2) |= 2;
    *(u64*)(work + 0x30) = now;
    *(u64*)(work + 0x18) = now;
    *(u32*)(work + 0x20) = 0;
    *(u32*)(work + 0x24) = 0;
    *(u32*)(work + 0x28) = 0;
    *(u32*)(work + 0x10) = 0;
}

void msgWindow_Add(char* text, s32 id) {
    extern void* windowGetPointer(s32);
    extern void* gp;
    extern void msgAnalize(void*, void*);
    u8* window = windowGetPointer(id);
    void* smart = *(void**)(window + 0x28);
    u8* work = *(u8**)smart;

    *(u64*)(work + 0x30) = *(u64*)((u8*)gp + 0x38);
    msgAnalize(smart, text);
    *(u16*)window = 1;
    *(u16*)(window + 2) |= 2;
}

s32 msg_compare(s32* a, s32* b) {
    extern char* msgSortDvdData;
    extern s32 strcmp(const char*, const char*);
    s32 result = strcmp(msgSortDvdData + *a, msgSortDvdData + *b);

    if (result > 0) {
        return 1;
    }
    return result >> 31;
}

u8 msgAnalize(void* smart, s32 textAddress) {
    extern u16 kanjiSearch(u32);
    extern u16 hankakuSearch(u32);
    extern u16 kanjiGetWidth(u32);
    extern s32 strcmp(const char*, const char*);
    extern s32 sscanf(const char*, const char*, ...);
    extern u32 dat_804205ec;
    extern u32 dat_804205f0;
    extern u32 dat_804205f4;
    extern u32 dat_804205f8;
    extern f32 float_1_80420618;
    extern f32 float_10_80420624;
    s32 work = *(s32*)smart;
    u8* text = (u8*)textAddress;
    s32 count = *(s32*)(work + 0x38);
    s32 x = *(s16*)(work + 0xF234);
    s32 y = *(s16*)(work + 0xF236);
    s32 line = *(s16*)(work + 0xF238);
    s32 bottom = *(s16*)(work + 0xF23A);
    s32 speed = *(s32*)(work + 0xF23C);
    u32 time = *(u32*)(work + 0xF230);
    u32 flags = 0;
    f32 scaleX = float_1_80420618;
    f32 scaleY = float_1_80420618;
    f32 dynamic = 0.0f;
    s32 maxX = 70;
    s32* style = *(s32**)(work + 0xF24C);

    if (style[2] < 0) {
        style[2] = 0;
    }
    if (style[2] == 8 || style[2] == 3) {
        u32* entry = (u32*)(work + 0x3C + count * 0x18);
        entry[0] = style[2] == 8 ? dat_804205f0 : dat_804205ec;
        *(u16*)((u8*)entry + 4) = 0xFFF7;
        entry[3] = time;
        count++;
        *(s32*)(work + 0x38) = count;
    }

    for (;;) {
        u8 ch = *text;
        u32* entry = (u32*)(work + 0x3C + count * 0x18);

        if (ch == 0) {
            *(u16*)((u8*)entry + 4) = 0xFFFF;
            entry[3] = time;
            count++;
            if (*(s16*)(work + 0xF244) == 0) {
                if (x > maxX) maxX = x;
                *(s16*)(work + 0xF244) = maxX + 0x50;
            }
            *(s32*)(work + 0x38) = count;
            *(u32*)(work + 0xF230) = time;
            *(s16*)(work + 0xF234) = x;
            *(s16*)(work + 0xF236) = y;
            *(s16*)(work + 0xF238) = line;
            *(s16*)(work + 0xF23A) = bottom;
            *(s32*)(work + 0xF23C) = speed;
            return 0;
        }
        if (ch == '\n') {
            *(u16*)((u8*)entry + 4) = 0xFFFB;
            entry[3] = time;
            count++;
            time += speed;
            if (x > maxX) maxX = x;
            x = 0;
            y -= (s32)(32.0f * scaleY);
            bottom = y;
            line++;
            text++;
            continue;
        }
        if (ch == '<') {
            char command[128];
            char argument[128];
            s32 ci = 0;
            s32 ai = 0;
            s32 inArg = 0;
            s32 value = 0;

            text++;
            while (*text != 0 && *text != '>' && ci + ai < 126) {
                if (*text == ' ' && !inArg) {
                    command[ci] = 0;
                    inArg = 1;
                } else if (inArg) {
                    argument[ai++] = *text;
                } else {
                    command[ci++] = *text;
                }
                text++;
            }
            command[ci] = 0;
            argument[ai] = 0;
            if (*text == '>') text++;
            for (ci = 0; ci < ai; ci++) {
                if (argument[ci] >= '0' && argument[ci] <= '9') {
                    value = value * 10 + argument[ci] - '0';
                }
            }

            if (strcmp(command, "k") == 0 || strcmp(command, "key") == 0) {
                *(u16*)((u8*)entry + 4) = 0xFFFE;
                *(s16*)((u8*)entry + 6) = (s16)line;
                entry[3] = time;
                count++;
                time += 100;
            } else if (strcmp(command, "keyxon") == 0) {
                *(u32*)(work + 4) |= 0x100;
            } else if (strcmp(command, "p") == 0 || strcmp(command, "scrl_auto") == 0) {
                if (bottom != 0) {
                    *(u16*)((u8*)entry + 4) = 0xFFFD;
                    *(s16*)((u8*)entry + 8) = (s16)(bottom - 3);
                    entry[3] = time;
                    count++;
                    time += 50;
                    bottom = 0;
                }
            } else if (strcmp(command, "o") == 0 || strcmp(command, "once_stop") == 0) {
                *(u16*)((u8*)entry + 4) = 0xFFFC;
                entry[3] = time;
                count++;
                time += 50;
            } else if (strcmp(command, "pos") == 0) {
                sscanf(argument, "%d%d", &x, &y);
            } else if (strcmp(command, "scale") == 0) {
                sscanf(argument, "%f", &scaleX);
                scaleY = scaleX;
            } else if (strcmp(command, "scaleX") == 0) {
                sscanf(argument, "%f", &scaleX);
            } else if (strcmp(command, "/scale") == 0) {
                scaleX = float_1_80420618;
                scaleY = float_1_80420618;
            } else if (strcmp(command, "wait") == 0) {
                time += value;
            } else if (strcmp(command, "speed") == 0) {
                speed = value;
            } else if (strcmp(command, "/speed") == 0) {
                speed = *(s32*)(work + 0xF23C);
            } else if (strcmp(command, "dynamic") == 0) {
                sscanf(argument, "%f", &dynamic);
                flags |= 1;
            } else if (strcmp(command, "/dynamic") == 0) {
                flags &= ~1;
            } else if (strcmp(command, "shake") == 0) {
                flags |= 2;
            } else if (strcmp(command, "/shake") == 0) {
                flags &= ~2;
            } else if (strcmp(command, "wave") == 0) {
                flags |= 4;
            } else if (strcmp(command, "/wave") == 0) {
                flags &= ~4;
            } else if (strcmp(command, "col") == 0) {
                sscanf(argument, "%x", &entry[0]);
                *(u16*)((u8*)entry + 4) = 0xFFF7;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "/col") == 0) {
                entry[0] = 0xFF;
                *(u16*)((u8*)entry + 4) = 0xFFF7;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "noise") == 0 || strcmp(command, "/noise") == 0) {
                entry[0] = command[0] == '/' ? 0 : 1;
                *(u16*)((u8*)entry + 4) = 0xFFF6;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "dkey") == 0 || strcmp(command, "/dkey") == 0) {
                entry[0] = command[0] == '/' ? 0 : 1;
                *(u16*)((u8*)entry + 4) = 0xFFF5;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "plate") == 0) {
                style[2] = 2;
                speed = 0;
                *(s32*)(work + 0xF23C) = 0;
            } else if (strcmp(command, "system") == 0) {
                style[2] = 3;
                entry[0] = dat_804205f4;
                *(u16*)((u8*)entry + 4) = 0xFFF7;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "tec") == 0) {
                style[2] = 8;
                entry[0] = dat_804205f8;
                *(u16*)((u8*)entry + 4) = 0xFFF7;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "majo") == 0) {
                style[2] = 9;
            } else if (strcmp(command, "diary") == 0) {
                style[2] = 7;
            } else if (strcmp(command, "housou") == 0) {
                style[2] = 10;
            } else if (strcmp(command, "boss") == 0) {
                style[2] = 6;
            } else if (strcmp(command, "kanban") == 0) {
                style[2] = 4;
                speed = 0;
                *(s32*)(work + 0xF23C) = 0;
            } else if (strcmp(command, "small") == 0) {
                style[2] = 1;
                *(u32*)(work + 4) |= 0x10;
            } else if (strcmp(command, "clear") == 0) {
                style[2] = 11;
            } else if (strcmp(command, "se") == 0 || strcmp(command, "vol") == 0) {
                sscanf(argument, "%d", &entry[0]);
                *(u16*)((u8*)entry + 4) = strcmp(command, "se") == 0 ? 0xFFFA : 0xFFF9;
                entry[3] = time;
                count++;
            } else if (strcmp(command, "nowinse") == 0) {
                *(u16*)((u8*)style + 2) |= 8;
            }
            continue;
        }

        {
            u16 glyph;
            u16 width;
            s32 advance;
            if ((ch >= 0x81 && ch <= 0x9F) || (ch >= 0xE0 && ch <= 0xFC)) {
                glyph = kanjiSearch(*(u16*)text);
                text += 2;
                width = kanjiGetWidth(glyph) + 4;
            } else {
                glyph = hankakuSearch(ch);
                text++;
                width = kanjiGetWidth(glyph) + 2;
            }
            advance = (s32)(scaleX * width);
            entry[0] = flags | (ch == ' ' ? 0 : 8);
            *(u16*)((u8*)entry + 4) = glyph;
            *(s16*)((u8*)entry + 6) = (s16)x;
            *(s16*)((u8*)entry + 8) = (s16)(y - (s32)(float_10_80420624 * scaleY));
            entry[3] = time;
            *(f32*)((u8*)entry + 0x10) = scaleX;
            *(f32*)((u8*)entry + 0x14) = dynamic;
            x += advance;
            count++;
            time += speed;
        }
    }
}

u8 msgWindow_Main(void* pWindow) {
    extern void npcSetTalkPose(char* name);
    extern void npcSetStayPose(char* name);
    extern void BtlUnit_ChangeTalkAnim(s32 unit);
    extern void BtlUnit_ChangeStayAnim(s32 unit);
    extern u32 keyGetButtonTrg(s32 pad);
    extern u32 psndSFXOn(s32 id);
    extern void windowDelete(s32 id);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 order);
    extern s32 msgMain(int* work);
    extern void msgWindow_Disp(s32 camera, void* win);
    extern void* gp;
    extern f32 float_0p6_8042065c;
    extern f32 float_400_8042061c;
    extern u8 msgWindowOpen_SE_Data[];
    extern u8 DAT_8030f544[];

    int* holder = *(int**)((s32)pWindow + 0x28);
    int work = *holder;
    s32 result;
    s32 target;
    s32 elapsed;
    u32 buttons;
    u32 accept;
    u64 now;

#define W_ACTION (*(s32*)((s32)pWindow + 0x00))
#define W_FLAGS  (*(u16*)((s32)pWindow + 0x02))
#define W_ORDER  (*(s16*)((s32)pWindow + 0x04))
#define W_ALPHA  (*(s16*)((s32)pWindow + 0x06))
#define W_TYPE   (*(s32*)((s32)pWindow + 0x08))
#define W_START  (*(u64*)((s32)pWindow + 0x20))
#define WORK_S32(o) (*(s32*)(work + (o)))
#define WORK_U32(o) (*(u32*)(work + (o)))
#define WORK_U16(o) (*(u16*)(work + (o)))
#define SET_STAY() do { if ((WORK_U32(4) & 8) == 0) npcSetStayPose((char*)(work + 0xF204)); else BtlUnit_ChangeStayAnim(WORK_S32(0xF224)); } while (0)
#define SAVE_TIME() do { W_START = *(u64*)((s32)gp + 0x18); } while (0)
#define ELAPSED_MS() ((s32)((*(u64*)((s32)gp + 0x18) - W_START) / (*(u32*)0x800000F8 / 4000)))

    if (WORK_S32(0xF250) != 0) {
        if (WORK_S32(0xF254) == 0) {
            if ((WORK_U32(4) & 8) == 0) {
                npcSetTalkPose((char*)(work + 0xF204));
            } else {
                BtlUnit_ChangeTalkAnim(WORK_S32(0xF224));
            }
            WORK_S32(0xF254) = 1;
        }
        WORK_S32(0xF250)--;
        if (WORK_S32(0xF250) == 0) {
            SET_STAY();
            WORK_S32(0xF254) = 0;
        }
    }

    switch (W_ACTION) {
        case 1:
            result = msgMain(holder);
            if (result == 2) {
                W_ACTION = 3;
            } else if (result == 0) {
                W_ACTION = 7;
                SET_STAY();
            } else if (result == 1) {
                W_ACTION = 2;
                W_FLAGS &= ~2;
                SET_STAY();
            }
            break;

        case 3:
            accept = 0x100;
            if (WORK_U32(4) & 0x100) {
                accept |= 0x400;
            }
            buttons = keyGetButtonTrg(0);
            if (buttons & (accept | 0x200)) {
                W_ACTION = 1;
                WORK_U16(10) = 2;
                if ((buttons & 0x200) && ((WORK_U32(4) & 0x80) == 0)) {
                    WORK_U32(4) |= 4;
                }
            } else if (keyGetButtonTrg(0) & 0x10) {
                WORK_S32(0xF03C)--;
                if (WORK_S32(0xF03C) < 0) {
                    WORK_S32(0xF03C) = 0;
                } else {
                    W_ACTION = 9;
                    SAVE_TIME();
                    psndSFXOn(0x20041);
                    WORK_U32(4) |= 0x40;
                }
            }
            break;

        case 5:
            if ((W_FLAGS & 8) == 0 && *(s32*)((s32)&msgWindowOpen_SE_Data + W_TYPE * 8) >= 0) {
                psndSFXOn(*(s32*)((s32)&msgWindowOpen_SE_Data + W_TYPE * 8) | 0x20000);
            }
            SAVE_TIME();
            W_ACTION = 6;
        case 6:
            W_FLAGS |= 2;
            elapsed = ELAPSED_MS();
            W_ALPHA = (s16)((elapsed * 255) / 200);
            if (W_ALPHA > 255) {
                W_ALPHA = 255;
                W_ACTION = 1;
                WORK_S32(0x18) = *(s32*)((s32)gp + 0x18);
                WORK_S32(0x1C) = *(s32*)((s32)gp + 0x1C);
                WORK_S32(0x30) = *(s32*)((s32)gp + 0x18);
                WORK_S32(0x34) = *(s32*)((s32)gp + 0x1C);
            }
            break;

        case 7:
            if ((W_FLAGS & 8) == 0 && *(s32*)((s32)&DAT_8030f544 + W_TYPE * 8) >= 0) {
                psndSFXOn(*(s32*)((s32)&DAT_8030f544 + W_TYPE * 8) | 0x20000);
            }
            SAVE_TIME();
            W_ACTION = 8;
        case 8:
            if (W_ALPHA == 0) {
                windowDelete((s32)pWindow);
                return 0;
            }
            W_ALPHA = 255 - (s16)((ELAPSED_MS() * 255) / 200);
            if (W_ALPHA < 1) {
                W_ALPHA = 0;
            }
            break;

        case 9:
            target = WORK_S32(0xF044 + WORK_S32(0xF03C) * 4);
            if (target <= WORK_S32(0x28)) {
                WORK_S32(0x28) -= (s32)(float_0p6_8042065c * ELAPSED_MS());
                SAVE_TIME();
                if (WORK_S32(0x28) <= target) {
                    WORK_S32(0x28) = target;
                    W_ACTION = 11;
                }
            }
            break;

        case 10:
            target = WORK_S32(0xF044 + WORK_S32(0xF03C) * 4);
            if (WORK_S32(0x28) <= target) {
                WORK_S32(0x28) += (s32)(float_0p6_8042065c * ELAPSED_MS());
                SAVE_TIME();
                if (target <= WORK_S32(0x28)) {
                    WORK_S32(0x28) = target;
                    if (WORK_S32(0xF03C) == WORK_S32(0xF040) - 1) {
                        W_ACTION = 3;
                        WORK_U32(4) &= ~0x40;
                    } else {
                        W_ACTION = 11;
                    }
                }
            }
            break;

        case 11:
            accept = (WORK_U32(4) & 0x100) ? 0x500 : 0x100;
            buttons = keyGetButtonTrg(0);
            if (buttons & accept) {
                W_ACTION = 10;
                SAVE_TIME();
                WORK_S32(0xF03C)++;
                if (WORK_S32(0xF03C) >= WORK_S32(0xF040)) {
                    WORK_S32(0xF03C) = WORK_S32(0xF040) - 1;
                }
                psndSFXOn(0x20041);
            } else if (buttons & 0x200) {
                W_ACTION = 10;
                SAVE_TIME();
                WORK_S32(0xF03C) = WORK_S32(0xF040) - 1;
                psndSFXOn(0x20041);
            } else if (buttons & 0x10) {
                W_ACTION = 9;
                SAVE_TIME();
                WORK_S32(0xF03C)--;
                if (WORK_S32(0xF03C) < 0) {
                    WORK_S32(0xF03C) = 0;
                } else {
                    psndSFXOn(0x20041);
                }
            }
            break;
    }

    dispEntry(4, 0, msgWindow_Disp, pWindow,
              float_400_8042061c - (f32)W_ORDER);

#undef ELAPSED_MS
#undef SAVE_TIME
#undef SET_STAY
#undef WORK_U16
#undef WORK_U32
#undef WORK_S32
#undef W_START
#undef W_TYPE
#undef W_ALPHA
#undef W_ORDER
#undef W_FLAGS
#undef W_ACTION
    return 0;
}

void msgWindow_Clear_Main(void* win) {
    extern void* gp;
    extern s32 msgMain(void*);
    extern void windowDelete(void*);
    extern void npcSetStayPose(void*);
    extern void BtlUnit_ChangeStayAnim(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void msgWindow_Disp(s32, void*);
    extern f32 float_400_8042061c;
    u8* window = win;
    u8* work;
    s32 result;
    s64 elapsed;

    if (*(u16*)window == 5) {
        *(u16*)window = 1;
        *(u64*)(window + 0x20) = *(u64*)((u8*)gp + 0x38);
        work = **(u8***)(window + 0x28);
        *(u64*)(work + 0x18) = *(u64*)((u8*)gp + 0x38);
        *(u64*)(work + 0x30) = *(u64*)((u8*)gp + 0x38);
    } else if (*(u16*)window < 5) {
        if (*(u16*)window != 1) {
            goto display;
        }
    } else {
        if (*(u16*)window == 7) {
            if (*(u16*)(window + 6) == 0) {
                windowDelete(window);
                return;
            }
            elapsed = (s64)(*(u64*)((u8*)gp + 0x38) - *(u64*)(window + 0x20));
            elapsed /= (*(u32*)0x800000F8 / 4000);
            *(u16*)(window + 6) = 0xFF - (s16)((elapsed * 0xFF) / 200);
            if ((s16)*(u16*)(window + 6) < 1) {
                *(u16*)(window + 6) = 0;
            }
            goto display;
        }
        if (*(u16*)window > 6) {
            goto display;
        }
    }

    *(u16*)(window + 2) |= 2;
    if ((s16)*(u16*)(window + 6) < 0xFF) {
        elapsed = (s64)(*(u64*)((u8*)gp + 0x38) - *(u64*)(window + 0x20));
        elapsed /= (*(u32*)0x800000F8 / 4000);
        *(u16*)(window + 6) = (s16)((elapsed * 0xFF) / 200);
        if ((s16)*(u16*)(window + 6) > 0xFF) {
            *(u16*)(window + 6) = 0xFF;
        }
    }
    if (*(u16*)window != 2) {
        result = msgMain(*(void**)(window + 0x28));
        if (result == 2) {
            *(u16*)window = 3;
        } else if (result < 2) {
            if (result == 0) {
                *(u16*)window = 7;
                *(u64*)(window + 0x20) = *(u64*)((u8*)gp + 0x38);
                work = **(u8***)(window + 0x28);
                if ((*(u32*)(work + 4) & 8) == 0) {
                    npcSetStayPose(work + 0xF204);
                } else {
                    BtlUnit_ChangeStayAnim(*(void**)(work + 0xF224));
                }
            } else if (result > -1) {
                *(u16*)window = 2;
                *(u16*)(window + 2) &= 0xFFFD;
                work = **(u8***)(window + 0x28);
                if ((*(u32*)(work + 4) & 8) == 0) {
                    npcSetStayPose(work + 0xF204);
                } else {
                    BtlUnit_ChangeStayAnim(*(void**)(work + 0xF224));
                }
            }
        }
    }
display:
    dispEntry(8, 0, msgWindow_Disp, window,
              float_400_8042061c - (f32)*(s16*)(window + 4));
}

