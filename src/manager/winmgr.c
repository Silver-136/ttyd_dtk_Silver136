#include "manager/winmgr.h"
#include "manager/evtmgr.h"

#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "event/evt_badgeshop.h"
#include "mario/mario_pouch.h"
#include "statuswindow.h"

void* wp;
void* __memAlloc(s32 heap, u32 size);
void __memFree(s32 heap, void* ptr);
extern u8 itemDataTable[];
char* msgSearch(char* msg);
void winMgrHelpDraw(void* win);
extern char vec3_803003a0[];
extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
extern void PSMTXScale(f32[3][4], f32, f32, f32);
extern char str_msg_window_title_5_80300808[];
extern char str_help_80428008[];
extern char* mario_status_name_table[];
extern void FontDrawStart(void);
extern char name_party0[],name_party1[],name_party2[],name_party4[],name_party5[],name_party6[];
extern void PSMTXTrans(f32[3][4], f32, f32, f32);

s32 unk_8023f8d0(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 value;
    s32 threshold;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            if (evtGetValue(event, 0xF8406022) == 0) {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 1 ? 1 : 0;
            } else {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 2 ? 2 : 0;
            }
            if (threshold == 0) {
                evtSetValue(event, args[0], 1);
                return 2;
            }
        }
        party++;
    }
    evtSetValue(event, args[0], 0);
    return 2;
}

s32 unk_8023f79c(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 joined;
    s32 missing;
    s32 hasValue;
    s32 threshold;
    s32 value;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    joined = 0;
    missing = 0;
    hasValue = 0;
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            joined++;
            value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
            if (value != 0) {
                hasValue = 1;
                if (evtGetValue(event, 0xF8406022) == 0) {
                    threshold = value >= 1 ? 1 : 0;
                } else {
                    threshold = value >= 2 ? 2 : 0;
                }
                if (threshold == 0) {
                    missing = 1;
                }
            }
        }
        party++;
    }
    if (joined > 1 && missing != 0 && hasValue != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

void winMgrInit(void) {
    *(s32*)wp = 0x20;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x44);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

void winMgrReInit(void) {
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winMgrDisp(s32 cameraId, s32 winArg, void* unused) {
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void windowDispGX2_Waku_col(f32, f32, f32, f32, f32, f32[3][4], s32, u32*);
    extern f32 float_deg2rad_80428060;
    extern u32 dat_80427fa0;
    extern u32 dat_80427fa4;
    extern u32 dat_80427fa8;
    extern u32 dat_80427fac;
    extern u32 dat_80427fb0;

    typedef struct WinMgrDescLocal {
        s32 fadeMode;
        s32 headingType;
        s32 cameraId;
        s32 x;
        s32 y;
        s32 width;
        s32 height;
        u32 color;
        void* mainFunc;
        void (*dispFunc)(void*);
    } WinMgrDescLocal;
    typedef struct WinMgrEntryLocal {
        s32 flags;
        s32 fadeState;
        s32 fadeFrameCounter;
        u32 windowColAlpha;
        f32 scale;
        f32 zRotDeg;
        s32 x;
        s32 y;
        s32 width;
        s32 height;
        WinMgrDescLocal* desc;
    } WinMgrEntryLocal;

    f32 rot[3][4];
    f32 scaleMtx[3][4];
    WinMgrEntryLocal* win = (WinMgrEntryLocal*)winArg;
    u32 color;
    (void)cameraId;
    (void)unused;

    PSMTXRotRad(rot, 0x7A, float_deg2rad_80428060 * win->zRotDeg);
    PSMTXScale(scaleMtx, win->scale, win->scale, win->scale);
    PSMTXConcat(rot, scaleMtx, scaleMtx);

    color = win->desc->color;
    ((u8*)&color)[3] = (u8)win->windowColAlpha;
    windowDispGX2_Waku_col(
        (f32)win->x,
        (f32)win->y,
        (f32)win->width,
        (f32)win->height,
        20.0f, scaleMtx, 0, &color);

    if (win->desc->headingType == 1) {
        color = dat_80427fa0;
        ((u8*)&color)[3] = (u8)win->windowColAlpha;
        windowDispGX2_Waku_col(
            (f32)(win->x + (win->width - 150) / 2),
            (f32)(win->y + 14),
            150.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    } else if (win->desc->headingType == 3) {
        color = dat_80427fa4;
        ((u8*)&color)[3] = (u8)win->windowColAlpha;
        windowDispGX2_Waku_col(
            (f32)(win->x + 10),
            (f32)(win->y + 14),
            100.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
        color = dat_80427fa8;
        ((u8*)&color)[3] = (u8)win->windowColAlpha;
        windowDispGX2_Waku_col(
            (f32)(win->x + win->width / 2 - 10),
            (f32)(win->y + 14),
            100.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    } else if (win->desc->headingType >= 4 && win->desc->headingType < 6) {
        color = dat_80427fac;
        ((u8*)&color)[3] = (u8)win->windowColAlpha;
        windowDispGX2_Waku_col(
            (f32)(win->x + (((win->width * 3 / 5) - 120) * 2) / 3),
            (f32)(win->y + 14),
            120.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
        color = dat_80427fb0;
        ((u8*)&color)[3] = (u8)win->windowColAlpha;
        windowDispGX2_Waku_col(
            (f32)(win->x + win->width * 3 / 5 + ((win->width * 2 / 5) - 85) / 3),
            (f32)(win->y + 14),
            85.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    }

    if (win->desc->dispFunc != 0) {
        win->desc->dispFunc(win);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMgrMain(void) {
    extern void winMgrSeq(void* win);
    extern void winMgrDisp(s32 cameraId, s32 unused, void* win);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* param, f32 z);

    void* win;
    void* tpl;
    void (*mainFunc)(void*);
    s32 i;

    i = 0;
    win = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        if ((*(u32*)win & 1) != 0) {
            winMgrSeq(win);
            tpl = *(void**)((s32)win + 0x28);
            mainFunc = *(void (**)(void*))((s32)tpl + 0x20);
            if (mainFunc != 0) {
                mainFunc(win);
            }
            if ((*(u32*)win & 2) != 0) {
                tpl = *(void**)((s32)win + 0x28);
                dispEntry(*(s32*)((s32)tpl + 8), 0, winMgrDisp, win, (f32)(*(s32*)((s32)win + 0x30) + 0x12C));
            }
        }
        i++;
        win = (void*)((s32)win + 0x44);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void winMgrSeq(void* ptr) {
    typedef struct { s32 fadeMode; } Desc;
    typedef struct {
        u32 flags;
        s32 fadeState;
        s32 fadeFrameCounter;
        u32 windowColAlpha;
        f32 scale;
        f32 zRotDeg;
        s32 x, y, width, height;
        Desc* desc;
    } Win;
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern void* gpGlobals;
    Win* win = ptr;
    s32 duration;

    win->flags |= 4;
    switch (win->desc->fadeMode) {
    case 0:
        switch (win->fadeState) {
        case 0:
            win->flags &= ~4;
            break;
        case 1:
            win->flags |= 2;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->zRotDeg = (f32)intplGetValue(30.0, 0.0, 12, win->fadeFrameCounter, duration);
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->scale = (f32)intplGetValue(0.0, 1.0, 12, win->fadeFrameCounter, duration);
            win->windowColAlpha = 0xFF;
            win->fadeFrameCounter++;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            if (duration < win->fadeFrameCounter) {
                win->fadeState = 0;
            }
            break;
        case 2:
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->zRotDeg = (f32)intplGetValue(0.0, 30.0, 12, win->fadeFrameCounter, duration);
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->scale = (f32)intplGetValue(1.0, 0.0, 12, win->fadeFrameCounter, duration);
            win->windowColAlpha = 0xFF;
            win->fadeFrameCounter++;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            if (duration < win->fadeFrameCounter) {
                win->fadeState = 0;
                win->flags &= ~2;
                if (win->flags & 8) win->flags &= ~1;
            }
            break;
        }
        break;
    case 1:
        switch (win->fadeState) {
        case 0:
            win->flags &= ~4;
            break;
        case 1:
            win->flags |= 2;
            win->zRotDeg = 1.0f;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->scale = (f32)intplGetValue(0.0, 1.0, 12, win->fadeFrameCounter, duration);
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->windowColAlpha = (u32)intplGetValue(0.0, 255.0, 12, win->fadeFrameCounter, duration);
            win->fadeFrameCounter++;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            if (duration < win->fadeFrameCounter) win->fadeState = 0;
            break;
        case 2:
            win->zRotDeg = 0.0f;
            win->scale = 1.0f;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            win->windowColAlpha = (u32)intplGetValue(255.0, 0.0, 12, win->fadeFrameCounter, duration);
            win->fadeFrameCounter++;
            duration = (*(s32*)((u8*)gpGlobals + 4) * 8) / 60;
            if (duration < win->fadeFrameCounter) {
                win->fadeState = 0;
                win->flags &= ~2;
                if (win->flags & 8) win->flags &= ~1;
            }
            break;
        }
        break;
    case 2:
        switch (win->fadeState) {
        case 0:
            win->flags &= ~4;
            break;
        case 1:
            win->flags |= 2;
            win->zRotDeg = 1.0f;
            win->scale = 1.0f;
            win->windowColAlpha = 0xFF;
            win->fadeState = 0;
            break;
        case 2:
            win->flags &= ~2;
            win->zRotDeg = 1.0f;
            win->scale = 1.0f;
            win->windowColAlpha = 0xFF;
            win->fadeState = 0;
            if (win->flags & 8) win->flags &= ~1;
            break;
        }
        break;
    }
}

s32 winMgrEntry(void* tpl) {
    void* win;
    s32 id;
    s32 zero;
    void* savedTpl;

    id = 0;
    win = *(void**)((s32)wp + 4);
    for (id = 0; id < *(s32*)wp; id++, win = (void*)((s32)win + 0x44)) {
        if ((*(u32*)win & 1) == 0) {
            break;
        }
    }

    zero = 0;
    *(s32*)((s32)win + 0) = zero;
    *(u32*)win |= 1;
    *(s32*)((s32)win + 4) = zero;
    *(s32*)((s32)win + 8) = zero;
    *(void**)((s32)win + 0x28) = tpl;
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x18) = *(s32*)((s32)savedTpl + 0xC);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x1C) = *(s32*)((s32)savedTpl + 0x10);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x20) = *(s32*)((s32)savedTpl + 0x14);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x24) = *(s32*)((s32)savedTpl + 0x18);
    *(s32*)((s32)win + 0x30) = zero;
    *(s32*)((s32)win + 0x2C) = zero;
    *(s32*)((s32)win + 0x38) = zero;
    *(s32*)((s32)win + 0x3C) = zero;
    return id;
}

void winMgrSetParam(s32 id, s32 param) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x2C) = param;
    }
}

void winMgrOpen(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 1;
        *(s32*)((s32)win + 8) = 0;
        *(s32*)((s32)win + 0x38) = 0;
        *(s32*)((s32)win + 0x3C) = 0;
        *(s32*)((s32)win + 0x34) = 0;
    }
}

void winMgrClose(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
    }
}

void winMgrCloseAutoDelete(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
        *(u32*)win |= 8;
    }
}

void winMgrDelete(s32 id) {
    u32* flags = (u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    *flags &= ~1;
}

s32 winMgrAction(s32 id) {
    return (*(u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44) >> 2) & 1;
}

void winMgrSetSize(s32 id, s32 x, s32 y, s32 w, s32 h) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x18) = x;
        *(s32*)((s32)win + 0x1C) = y;
        *(s32*)((s32)win + 0x20) = w;
        *(s32*)((s32)win + 0x24) = h;
    }
}

void winMgrSetPriority(s32 id, s32 priority) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x30) = priority;
    }
}

void* winMgrGetPtr(s32 id) {
    return (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
}

void winMgrHelpInit(void* win) {
    *(s32*)((s32)win + 0x38) = 0;
    *(s32*)((s32)win + 0x3C) = 0;
    *(s32*)((s32)win + 0x34) = 0;
}

void winMgrHelpMain(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

void winMgrHelpDraw(void* win) {
    extern void GXSetScissor(s32, s32, s32, s32);
    extern char* msgSearch(char*);
    extern s32 strcmp(char*, char*);
    extern void* lotteryGetPtr(void);
    extern s32 sprintf(char*, char*, ...);
    extern void FontDrawStart(void);
    extern void FontDrawMessage(s32, s32, char*);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void iconDispGx2(f32[3][4], s32, s32);
    extern char vec3_803003a0[];
    extern void* gpGlobals;

    f32 trans[3][4];
    f32 scale[3][4];
    char buffer[0x400];
    char* help = *(char**)((s32)win + 0x34);
    s32 cursor = *(s32*)((s32)win + 0x38);
    s32 lineCount = *(s32*)((s32)win + 0x3C);
    s32 x;
    s32 y;

    if ((*(u32*)win & 4) != 0) {
        return;
    }

    GXSetScissor(0, 0x180, 0x260, 0x38);
    if (strcmp(help, msgSearch(&vec3_803003a0[0x554])) == 0) {
        void* lottery = lotteryGetPtr();
        sprintf(buffer, help, *(s16*)((s32)lottery + 0x20));
        FontDrawStart();
        FontDrawMessage(-210, cursor * 28 - 144, buffer);
    } else {
        FontDrawStart();
        FontDrawMessage(-210, cursor * 28 - 144, help);
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (lineCount > 2) {
        x = *(s32*)((s32)win + 0x18) + *(s32*)((s32)win + 0x20) - 12;
        if (cursor == 0) {
            if ((*(u32*)((s32)gpGlobals + 0x1C) & 0x1F) < 0x14) {
                y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) - 4;
                PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(trans, scale, trans);
                iconDispGx2(trans, 0x10, 0x1BE);
            }
        } else if (cursor + 2 == lineCount) {
            if ((*(u32*)((s32)gpGlobals + 0x1C) & 0x1F) < 0x14) {
                y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) + 44;
                PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(trans, scale, trans);
                iconDispGx2(trans, 0x10, 0x1BD);
            }
        } else if ((*(u32*)((s32)gpGlobals + 0x1C) & 0x1F) < 0x14) {
            y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) + 44;
            PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, 0x10, 0x1BD);

            y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) - 4;
            PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, 0x10, 0x1BE);
            }

        y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) + 10;
        PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
        PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
        PSMTXConcat(trans, scale, trans);
        iconDispGx2(trans, 0x10, 0x78);
    }
}

s32* winMgrSelectEntry(u32 selectType, s32 newItem, s32 isCancellable) {
    extern void* wp;
    extern u8 select_desc_tbl[];
    extern void* pouchGetPtr(void);
    extern void* __memAlloc(s32, u32);
    extern void memset(void*, s32, u32);
    extern s32 pouchKeyItem(s32);
    extern s32 pouchHaveItem(s32);
    extern s32 pouchHaveBadge(s32);
    extern s32 pouchKeepItem(s32);
    extern s32 partyChkJoin(s32);
    extern s32 evtGetValue(void*,s32);
    extern u16 party_id_table[];
    extern s32 DAT_803ad0c4; extern u8 DAT_803ad0e1,DAT_803ad0ee,_jdt[];
    extern s32 johoya_get(s32,s32);
    extern u16 badge_bottakuru_table[];
    extern s32 getBadgeBottakuru100TableMaxCount(void); extern u16 badge_bottakuru100_table[];
    u8* select;
    u8* descList;
    u8* desc;
    u8* entries;
    u8* rows;
    s32 descIndex;
    s32 descCount;
    s32 i;
    s32 count = 0;
    u8* pouch;

    pouch = pouchGetPtr();
    select = __memAlloc(0, 0x3C);
    memset(select, 0, 0x3C);
    *(u32*)(select + 4) = selectType;
    *(s32*)(select + 0xC) = 0;
    *(s32*)(select + 0x10) = 0;
    *(f32*)(select + 0x14) = 0.0f;
    *(f32*)(select + 0x18) = 0.0f;
    *(f32*)(select + 0x1C) = 0.0f;
    *(s32*)(select + 0x38) = newItem;
    if (isCancellable != 0) {
        *(u16*)select |= 0x100;
    }
    descIndex = (s32)selectType < 0 ? 0 : (s32)selectType;
    descList = select_desc_tbl + descIndex * 8;
    descCount = *(u16*)descList;
    desc = *(u8**)(descList + 4);
    *(s32*)(select + 0x20) = descCount;
    for (i = 0; i < descCount; i++, desc += 0x24) {
        s32 id = 0;
        s32 left = *(s32*)wp;
        u8* entry = *(u8**)((u8*)wp + 4);
        while (left > 0 && (*(u32*)entry & 1) != 0) {
            id++;
            entry += 0x44;
            left--;
        }
        *(u32*)entry = 1;
        *(s32*)(entry + 4) = 0;
        *(s32*)(entry + 8) = 0;
        *(u8**)(entry + 0xC) = desc;
        *(s32*)(entry + 0x18) = *(s32*)(desc + 0xC);
        *(s32*)(entry + 0x1C) = *(s32*)(desc + 0x10);
        *(s32*)(entry + 0x20) = *(s32*)(desc + 0x14);
        *(s32*)(entry + 0x24) = *(s32*)(desc + 0x18);
        *(s32*)(entry + 0x28) = 0;
        *(void**)(entry + 0x2C) = 0;
        *(s32*)(entry + 0x38) = 0;
        *(s32*)(entry + 0x3C) = 0;
        *(s32*)(select + 0x2C + i * 4) = id;
        entries = *(u8**)((u8*)wp + 4);
        if ((*(u32*)(entries + id * 0x44) & 1) != 0) {
            *(u8**)(entries + id * 0x44 + 0x2C) = select;
        }
    }
    *(f32*)(select + 0x14) = (f32)*(s32*)(desc + 0xC - descCount * 0x24);
    *(f32*)(select + 0x18) = (f32)*(s32*)(desc + 0x10 - descCount * 0x24);

    if (selectType == 0) descCount = 0x1E8;
    else if (selectType == 1 || selectType == 3 || selectType == 4) descCount = 0x54;
    else if (selectType == 2 || selectType == 0xC) descCount = 0x324;
    else if (selectType == 5) descCount = 0x84;
    else if (selectType == 6 || selectType == 7 || selectType == 8) descCount = 0x1C;
    else if (selectType == 0x11 || selectType == 0x12) descCount = (DAT_803ad0c4 + 1) * 4;
    else if (selectType == 0xE) descCount = 0x10;
    else if (selectType == 0xF) descCount = getBadgeBottakuru100TableMaxCount() * 4;
    else descCount = 0x324;
    rows = __memAlloc(0, descCount);
    memset(rows, 0, descCount);
    *(u8**)(select + 0x30) = rows;
    if (newItem != 0) {
        *(u16*)(rows + 2) = (u16)newItem;
        count = 1;
    }
    if (selectType == 0) {
        for (i = 0; i < 0x79; i++) {
            s32 item = pouchKeyItem(i);
            if (item != 0) { *(u16*)(rows + count++ * 4 + 2) = (u16)item; }
        }
    } else if (selectType == 1 || selectType == 3 || selectType == 4) {
        for (i = 0; i < 0x14; i++) {
            s32 item = pouchHaveItem(i);
            if (item != 0) { *(u16*)(rows + count++ * 4 + 2) = (u16)item; }
        }
    } else if (selectType == 2 || selectType == 0xC) {
        for (i = 0; i < 0x78; i++) {
            s32 item = pouchHaveBadge(i);
            if (item != 0) { *(u16*)(rows + count++ * 4 + 2) = (u16)item; }
        }
    } else if (selectType == 5) {
        for (i = 0; i < 0x20; i++) {
            s32 item = pouchKeepItem(i);
            if (item != 0) { *(u16*)(rows + count++ * 4 + 2) = (u16)item; }
        }
    } else if (selectType == 6 || selectType == 7 || selectType == 8) {
        for (i = 0; party_id_table[i] != 0; i++) {
            s32 party = party_id_table[i];
            if (partyChkJoin(party) != 0) {
                if (selectType != 8 || *(s16*)(pouch + party * 14 + 0xC) != 0) {
                    *(u16*)(rows + count * 4 + 2) = (u16)i;
                    if (selectType == 8 && *(s16*)(pouch + party * 14 + 0xC) == 0)
                        *(u16*)(rows + count * 4) |= 3;
                    count++;
                }
            }
        }
    } else if (selectType == 0x11 || selectType == 0x12) {
        u8* check = selectType == 0x11 ? &DAT_803ad0e1 : &DAT_803ad0ee;
        count = DAT_803ad0c4;
        for (i = 0; i < count; i++) {
            *(u16*)(rows + i * 4 + 2) = (u16)i;
            if ((s16)johoya_get((s32)check, *(s16*)(_jdt + i * 0x10 + 0xC)) != 0)
                *(u16*)(rows + i * 4) |= 1;
        }
    } else if (selectType == 0xE) {
        for (i = 0; i < 4; i++) {
            s32 item = badge_bottakuru_table[evtGetValue(0, i - 0xA21FE0A)];
            if (item != 0) *(u16*)(rows + count++ * 4 + 2) = (u16)item;
        }
    } else if (selectType == 0xF) {
        s32 shopCount = getBadgeBottakuru100TableMaxCount();
        for (i = 0; i < shopCount; i++)
            *(u16*)(rows + count++ * 4 + 2) = badge_bottakuru100_table[i];
    }
    *(s32*)(select + 0x34) = count;
    return (s32*)select;
}

s32 unk_8023d5e4(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    u8* values = itemDataTable + 0x18;
    u16 av = *(u16*)(values + ai * 0x28);
    u16 bv = *(u16*)(values + bi * 0x28);
    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}
s32 unk_8023d59c(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 av = *(s16*)((s32)values + ai * 0x28);
    s32 bv = *(s16*)((s32)values + bi * 0x28);

    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

#pragma optimize_for_size off
int unk_8023d524(void* lhs, void* rhs) {
    extern u8 itemDataTable[];
    u8* prices = itemDataTable + 0x14;
    s32 left;
    s32 right;

    left = (*(u16*)(prices + *(u16*)((u8*)lhs + 2) * 0x28) * 2 + 2) / 3;
    right = (*(u16*)(prices + *(u16*)((u8*)rhs + 2) * 0x28) * 2 + 2) / 3;
    if (left > right) {
        return 1;
    }
    return -(left < right);
}
#pragma optimize_for_size on


s32 winMgrSelect(void* select) {
    u16 flags = *(u16*)select;

    if (flags & 0x1000) {
        if (flags & 0x2000) {
            return -1;
        }
        if (*(s32*)((s32)select + 0x38) != 0 && *(s32*)((s32)select + 0xC) == 0 && (flags & 0x100)) {
            return -1;
        }
        {
            s32 offset = *(s32*)((s32)select + 0xC) << 2;
            void* list = *(void**)((s32)select + 0x30);

            list = (void*)((s32)list + offset);
            return *(u16*)((s32)list + 2);
        }
    }
    return 0;
}

s32 winMgrSelectOther(void* selectRaw, EventEntry* event) {
    typedef struct SelectRow {
        u16 flags;
        u16 value;
    } SelectRow;

    typedef struct SelectEntry {
        u16 flags;
        u16 pad02;
        s32 selectType;
        s32 state;
        s32 cursorIndex;
        s32 listRowOffset;
        f32 cursorX;
        f32 cursorY;
        f32 listYOffset;
        u32 numEntries;
        s32 entryIndices[3];
        SelectRow* rowData;
        s32 rowCount;
        s32 newItem;
    } SelectEntry;

    typedef struct ItemDataMini {
        char* stringId;
        char* nameMsg;
        char* descMsg;
        char* menuDescMsg;
        u8 pad10[4];
        u16 buyPrice;
        u16 discountPrice;
        u16 starPiecePrice;
        u16 sellPrice;
        s8 bpCost;
        u8 pad1D[0x0B];
    } ItemDataMini;

    extern u16 party_id_table[];
    extern char* pouchGetYoshiName(void);
    extern s32 sprintf(char*, char*, ...);
    extern s32 johoya_get(s32, s32);
    extern u8 _jdt[];
    extern char DAT_803ad0cc[];
    extern char* DAT_803ad0c8;
    extern s32 DAT_803ad0d0;
    extern s32 strlen(char*);
    extern s32 evtGetValue(void*, s32);

    extern void* gp;
    extern char* lbl_803B5EA0[];
    extern s16* mario_status_point_table[];
    extern s32 mario_status_henka_table[];

    SelectEntry* select = (SelectEntry*)selectRaw;
    u8* rodata = (u8*)vec3_803003a0;
    u8* jdt = _jdt;
    ItemDataMini* item;
    char* partyName;
    char titleKey[68];
    u16 flags;
    u32 value;
    s32 language;
    s32 temp;
    s32 price;

    flags = select->flags;

    if ((flags & 0x1000) != 0) {
        if ((flags & 0x2000) != 0) {
            return -1;
        }

        value = select->rowData[select->cursorIndex].value;

        switch (select->selectType) {
        case 6:
        case 7:
            event->lwData[1] = party_id_table[value];

            switch (value) {
            case 0:
                partyName = msgSearch((char*)(rodata + 0x2B8));
                break;
            case 1:
                partyName = msgSearch((char*)(rodata + 0x2C4));
                break;
            case 2:
                partyName = msgSearch((char*)(rodata + 0x2D0));
                break;
            case 3:
                partyName = pouchGetYoshiName();
                break;
            case 4:
                partyName = msgSearch((char*)(rodata + 0x2DC));
                break;
            case 5:
                partyName = msgSearch((char*)(rodata + 0x2E8));
                break;
            case 6:
                partyName = msgSearch((char*)(rodata + 0x2F4));
                break;
            }

            event->lwData[3] = (s32)partyName;
            break;

        case 8:
            event->lwData[2] = party_id_table[value];

            switch (value) {
            case 0:
                partyName = msgSearch((char*)(rodata + 0x2B8));
                break;
            case 1:
                partyName = msgSearch((char*)(rodata + 0x2C4));
                break;
            case 2:
                partyName = msgSearch((char*)(rodata + 0x2D0));
                break;
            case 3:
                partyName = pouchGetYoshiName();
                break;
            case 4:
                partyName = msgSearch((char*)(rodata + 0x2DC));
                break;
            case 5:
                partyName = msgSearch((char*)(rodata + 0x2E8));
                break;
            case 6:
                partyName = msgSearch((char*)(rodata + 0x2F4));
                break;
            }

            event->lwData[4] = (s32)partyName;
            break;

        case 9:
            event->lwData[1] = value;

            language = *(s32*)((u8*)gp + 0x16C);
            event->lwData[2] =
                (s32)lbl_803B5EA0[language * 3 + value];

            event->lwData[5] = *mario_status_point_table[0];
            event->lwData[6] = *mario_status_point_table[1];
            event->lwData[7] = *mario_status_point_table[2];

            event->lwData[value + 5] =
                event->lwData[value + 5] +
                mario_status_henka_table[value];
            break;

        case 10:
            event->lwData[1] = value;

            language = *(s32*)((u8*)gp + 0x16C);
            event->lwData[3] =
                (s32)lbl_803B5EA0[language * 3 + value];

            event->lwData[value + 5] =
                event->lwData[value + 5] -
                mario_status_henka_table[value];
            break;

        case 0xB:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);
            event->lwData[3] = item->buyPrice;
            event->lwData[4] = item->bpCost;
            break;

        case 0xC:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);
            event->lwData[3] = item->sellPrice;
            event->lwData[4] = select->cursorIndex;
            break;

        case 0xD:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);
            event->lwData[3] = item->starPiecePrice;
            event->lwData[4] = item->bpCost;
            break;

        case 0xE:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);
            event->lwData[3] = (item->buyPrice * 12) / 10;
            event->lwData[4] = item->bpCost;
            break;

        case 0xF:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);

            temp = evtGetValue(0, -169998679);
            price = item->buyPrice * (temp + 11);
            price = price / 10 + (price >> 31);
            event->lwData[3] = price - (price >> 31);

            event->lwData[4] = item->bpCost;
            break;

        case 0x10:
            item = (ItemDataMini*)(itemDataTable + value * 0x28);
            event->lwData[1] = value;
            event->lwData[2] = (s32)msgSearch(item->nameMsg);
            event->lwData[3] = (item->buyPrice * 2 + 2) / 3;
            event->lwData[4] = item->bpCost;
            break;

        case 0x11:
            item = (ItemDataMini*)(jdt + value * 0x10);

            sprintf(
                DAT_803ad0cc,
                (char*)(rodata + 0x538),
                DAT_803ad0c8,
                *(s16*)((u8*)item + 0xC));

            sprintf(
                titleKey,
                (char*)(rodata + 0x540),
                *(s16*)((u8*)item + 0xC));

            DAT_803ad0d0 = value;
            partyName = msgSearch(titleKey);

            event->lwData[1] = (s32)DAT_803ad0cc;
            event->lwData[2] = (s32)partyName;
            event->lwData[3] = strlen(partyName);

            if ((s16)johoya_get(
                    -0x7FC52F1F,
                    *(s16*)((u8*)item + 0xC)) == 0) {
                event->lwData[4] = *(s16*)((u8*)item + 0xA);
            } else {
                event->lwData[4] = 0;
            }
            break;

        case 0x12:
            item = (ItemDataMini*)(jdt + value * 0x10);

            sprintf(
                DAT_803ad0cc,
                (char*)(rodata + 0x538),
                DAT_803ad0c8,
                *(s16*)((u8*)item + 0xC));

            DAT_803ad0d0 = value;

            event->lwData[1] = (s32)DAT_803ad0cc;
            event->lwData[2] = *(s32*)((u8*)item + 4);
            event->lwData[3] = *(s16*)((u8*)item + 8);

            if ((s16)johoya_get(
                    -0x7FC52F12,
                    *(s16*)((u8*)item + 0xC)) == 0) {
                event->lwData[4] = 1;
            } else {
                event->lwData[4] = 0;
            }
            break;
        }

        return 1;
    }

    return 0;
}

void winMgrSelectDelete(void* select) {
    register s32 keep = (s32)select;

    __memFree(0, *(void**)(keep + 0x30));
    __memFree(0, (void*)keep);
}

s32 unk_8023cf04(void* win) {
    return *(s32*)((s32)win + 0xC);
}

void select_main(void* win) {
    extern void psndSFXOn(s32);
    extern u32 keyGetDirRep(s32);
    extern u32 keyGetDirTrg(s32);
    extern u16 keyGetButtonTrg(s32);

    void* select = *(void**)((s32)win + 0x2C);
    void* entries = *(void**)((s32)wp + 4);
    s32 state = *(s32*)((s32)select + 8);
    s32 count = *(s32*)((s32)select + 0x20);
    s32* entryIndices = (s32*)((s32)select + 0x24);
    s32 i;

    if (state == 2) {
        for (i = 0; i < count; i++) {
            void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
            if ((*(u32*)entry & 1) != 0) {
                *(s32*)((s32)entry + 4) = 2;
                *(s32*)((s32)entry + 8) = 0;
            }
        }
        psndSFXOn(0x2002B);
        statusWinForceOff();
        *(s32*)((s32)select + 8) += 1;
    } else if (state < 2) {
        if (state == 0) {
            for (i = 0; i < count; i++) {
                void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
                if ((*(u32*)entry & 1) != 0) {
                    *(s32*)((s32)entry + 4) = 1;
                    *(s32*)((s32)entry + 8) = 0;
                    *(s32*)((s32)entry + 0x38) = 0;
                    *(s32*)((s32)entry + 0x3C) = 0;
                    *(void**)((s32)entry + 0x34) = 0;
                }
            }
            if (count > 2) {
                void* help = (void*)((s32)entries + entryIndices[2] * 0x44);
                *(s32*)((s32)help + 0x38) = 0;
                *(s32*)((s32)help + 0x3C) = 0;
                *(void**)((s32)help + 0x34) = 0;
            }
            psndSFXOn(0x2002A);
            statusWinForceOpen();
            *(s32*)((s32)select + 8) += 1;
        } else if (state >= 0) {
            void* first = (void*)((s32)entries + entryIndices[0] * 0x44);
            if ((*(u32*)first & 4) == 0) {
                s32 previous = *(s32*)((s32)select + 0xC);
                s32 cursor;
                s32 rowCount = *(s32*)((s32)select + 0x34);
                s32 offset;
                u16 rowFlags;

                if (keyGetDirRep(0) & 0x2000) {
                    *(s32*)((s32)select + 0xC) += 1;
                }
                if (keyGetDirRep(0) & 0x1000) {
                    *(s32*)((s32)select + 0xC) -= 1;
                }
                cursor = *(s32*)((s32)select + 0xC);
                if (cursor >= rowCount) {
                    if ((keyGetDirTrg(0) & 0x2000) == 0) {
                        *(s32*)((s32)select + 0xC) -= 1;
                    } else {
                        *(s32*)((s32)select + 0xC) = 0;
                        *(s32*)((s32)select + 0x10) = 0;
                    }
                }
                if (*(s32*)((s32)select + 0xC) < 0) {
                    if ((keyGetDirTrg(0) & 0x1000) == 0) {
                        *(s32*)((s32)select + 0xC) += 1;
                    } else {
                        *(s32*)((s32)select + 0xC) = rowCount - 1;
                        if (rowCount - 8 > 0) {
                            *(s32*)((s32)select + 0x10) = rowCount - 8;
                        }
                    }
                }

                cursor = *(s32*)((s32)select + 0xC);
                offset = *(s32*)((s32)select + 0x10);
                if (previous < cursor) {
                    if (cursor > 5 && offset < (u32)(rowCount - 8)) {
                        *(s32*)((s32)select + 0x10) = offset + 1;
                    }
                } else if (cursor < previous && cursor - offset < 3 && offset > 0) {
                    *(s32*)((s32)select + 0x10) = offset - 1;
                }
                if (previous != cursor) {
                    psndSFXOn(0x20005);
                }

                if (keyGetButtonTrg(0) & 0x100) {
                    rowFlags = *(u16*)((s32)*(void**)((s32)select + 0x30) + cursor * 4);
                    if ((rowFlags & 1) == 0) {
                        psndSFXOn(0x20012);
                        *(s32*)((s32)select + 8) += 1;
                    } else {
                        psndSFXOn(0x20014);
                    }
                } else if ((keyGetButtonTrg(0) & 0x200) && (*(u16*)select & 0x100)) {
                    psndSFXOn(0x20013);
                    *(u16*)select |= 0x2000;
                    *(s32*)((s32)select + 8) += 1;
                }

                offset = *(s32*)((s32)select + 0x10);
                *(f32*)((s32)select + 0x1C) +=
                    ((f32)offset * 24.0f - *(f32*)((s32)select + 0x1C)) / 6.0f;
                *(f32*)((s32)select + 0x14) +=
                    ((f32)*(s32*)((s32)win + 0x18) - *(f32*)((s32)select + 0x14)) / 6.0f;
                *(f32*)((s32)select + 0x18) +=
                    (-((f32)(cursor - offset) * 24.0f -
                       (f32)(*(s32*)((s32)win + 0x1C) - 54)) -
                     *(f32*)((s32)select + 0x18)) / 6.0f;
            }
        }
    } else if (state != 4 && state < 4) {
        void* first = (void*)((s32)entries + entryIndices[0] * 0x44);
        if ((*(u32*)first & 4) == 0) {
            for (i = 0; i < count; i++) {
                void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
                *(u32*)entry &= ~1;
            }
            *(u16*)select |= 0x1000;
            *(s32*)((s32)select + 8) += 1;
        }
    }
}

void select_disp(void* win) {
    typedef struct Vec { f32 x,y,z; } Vec;
    extern void* wp; extern u8 itemDataTable[]; extern char* msgSearch(char*);
    extern void GXSetFog(s32,f32,f32,f32,f32,u32*); extern void GXGetScissor(s32*,s32*,s32*,s32*);
    extern void GXSetScissor(s32,s32,s32,s32); extern void winFontInit(void);
    extern void winFontSetWidth(Vec*,Vec*,void*,f32,char*,...); extern void winIconInit(void);
    extern void winIconGrayInit(void); extern void winIconSet(s32,Vec*,Vec*,void*);
    extern u16 FontGetMessageWidth(char*); extern void winFontSetEdgeWidth(Vec*,Vec*,void*,f32,char*,...);
    extern void iconDispGx(f64,Vec*,u16,u16); extern void* gpGlobals;
    extern char str_msg_window_title_1_803008c4[],str_msg_window_title_2_80300888[];
    extern char str_msg_window_title_3_8030089c[],str_msg_window_title_4_803008b0[];
    extern char str_msg_window_title_5_80300808[];
    extern s32 sprintf(char*,char*,...);
    u8* w=win; u8* sel=*(u8**)(w+0x2C); u8* rows=*(u8**)(sel+0x30);
    u8* entries=*(u8**)((u8*)wp+4); s32 oldX,oldY,oldW,oldH,i; Vec pos,scale;
    u32 color=0xFFFFFFFF,fog=0; char* title; f32 width; char priceText[32];
    if ((*(u32*)(entries + *(s32*)(sel+0x2C)*0x44) & 4) != 0) return;
    GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXGetScissor(&oldX,&oldY,&oldW,&oldH);
    GXSetScissor(*(s32*)(w+0x18)+0x130,0x112-*(s32*)(w+0x1C),*(s32*)(w+0x20),*(s32*)(w+0x24)-0x32);
    scale.x=scale.y=scale.z=1.0f;
    for(i=0;i<*(s32*)(sel+0x34);i++) {
        u16 flags=*(u16*)(rows+i*4),value=*(u16*)(rows+i*4+2); u8* item=itemDataTable+value*0x28;
        f32 y=*(f32*)(sel+0x1C)+(f32)(*(s32*)(w+0x1C)-0x2C-i*24);
        if(y-32.0f <= (f32)*(s32*)(w+0x1C) && y+32.0f >= (f32)(*(s32*)(w+0x1C)-*(s32*)(w+0x24))) {
            if(flags&2){if(flags&1)winIconGrayInit();else winIconInit();pos.x=(f32)(*(s32*)(w+0x18)+0xF);pos.y=y;pos.z=0.0f;winIconSet(0x9A,&pos,&scale,&color);}
            if(flags&1)winIconGrayInit();else winIconInit(); pos.x=(f32)(*(s32*)(w+0x18)+0x23);pos.y=y;pos.z=0.0f;
            winIconSet(*(s16*)(item+0x20),&pos,&scale,&color); winFontInit();
            pos.x=(f32)(*(s32*)(w+0x18)+0x3C);pos.y=y+12.0f;
            winFontSetWidth(&pos,&scale,&color,185.0f,msgSearch(*(char**)(item+8)));
            {
                s32 price=-1;
                switch(*(s32*)(sel+4)) {
                    case 3: price=*(u16*)(item+0x1A); break;
                    case 0xB: price=*(u16*)(item+0x14); break;
                    case 0x11: price=*(u16*)(item+0x18); break;
                }
                if(price>=0) {
                    sprintf(priceText,"%d",price); width=(f32)FontGetMessageWidth(priceText);
                    if(width>30.0f) width=30.0f; winFontInit();
                    pos.x=(f32)(*(s32*)(w+0x18)+*(s32*)(w+0x20)-10)-width;
                    pos.y=y+12.0f; winFontSetWidth(&pos,&scale,&color,30.0f,priceText);
                }
            }
        }
    }
    if(*(s32*)(sel+0x38)!=-1) {
        winIconInit();
        for(i=2;i<0x1E;i++) {
            pos.x=(f32)(*(s32*)(w+0x18)+(i**(s32*)(w+0x20))/0x1F);
            pos.y=(f32)(*(s32*)(w+0x1C)-0x38)+*(f32*)(sel+0x1C); pos.z=0.0f;
            winIconSet(0x1BF,&pos,&scale,&color);
        }
    }
    GXSetScissor(oldX,oldY,oldW,oldH);
    switch(*(s32*)(sel+4)) {
        case 2: case 0xB: title=msgSearch(str_msg_window_title_2_80300888); break;
        case 0x11: title=msgSearch(str_msg_window_title_5_80300808); break;
        default: title=msgSearch(str_msg_window_title_1_803008c4); break;
    }
    width=(f32)FontGetMessageWidth(title); if(width>120.0f) width=120.0f;
    winFontInit(); pos.x=(f32)*(s32*)(w+0x18)+((f32)*(s32*)(w+0x20)-width)*0.5f;
    pos.y=(f32)(*(s32*)(w+0x1C)+0xE); pos.z=0.0f;
    winFontSetEdgeWidth(&pos,&scale,&color,120.0f,title);
    pos.x=*(f32*)(sel+0x14); pos.y=*(f32*)(sel+0x18); pos.z=0.0f;
    iconDispGx(1.0,&pos,0x14,0x1F8);
    if(*(s32*)(sel+0x34)>8 && ((*(u32*)((u8*)gpGlobals+0x10)&0x1F)<0x14)) {
        pos.x=(f32)*(s32*)(w+0x18)+(f32)*(s32*)(w+0x20)*0.5f;
        if(*(s32*)(sel+0x10)!=0) { pos.y=(f32)(*(s32*)(w+0x1C)-0x24); iconDispGx(0.6,&pos,0x10,0x1BD); }
        if(*(s32*)(sel+0x10)!=*(s32*)(sel+0x34)-8) {
            pos.y=(f32)(*(s32*)(w+0x1C)-*(s32*)(w+0x24)-0xC); iconDispGx(0.6,&pos,0x10,0x1BE);
        }
    }
}

void select_disp_party(void* win) {
    extern void* wp;
    extern char* pouchGetYoshiName(void);
    extern char* msgSearch(char*);
    extern u16 FontGetMessageWidth(char*);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winFontInit(void);
    extern void winFontSet(Vec*, Vec*, void*, char*, ...);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32, Vec*, Vec*, void*);
    extern s32 pouchGetPartyColor(s32);
    extern s16 party_icon_table[], party_id_table[];
    extern void winFontSetEdgeWidth(Vec*, Vec*, void*, f32, char*, ...);
    extern void iconDispGx(f64, Vec*, u16, u16);
    extern void* gpGlobals;
    extern char str_msg_window_title_7_80300858[];
    extern char str_msg_window_title_8_8030086c[];

    u8* w = win;
    u8* sel = *(u8**)(w + 0x2C);
    PouchData* pouch;

    s32 oldX;
    s32 oldY;
    s32 oldW;
    s32 oldH;

    s32 rowPixel;
    s32 rowByte;
    s32 rowIndex;
    char* name;
    char* title;
    f32 width;

#define ROW_BASE() (*(u8**)(sel + 0x30))
#define ROW_FLAGS() (*(u16*)(ROW_BASE() + rowByte))
#define ROW_VALUE() (*(u16*)(ROW_BASE() + rowByte + 2))
#define GET_PARTY_NAME() do { \
        switch (ROW_VALUE()) { \
        case 0: name = msgSearch(name_party0); break; \
        case 1: name = msgSearch(name_party1); break; \
        case 2: name = msgSearch(name_party4); break; \
        case 3: name = pouchGetYoshiName(); break; \
        case 4: name = msgSearch(name_party5); break; \
        case 5: name = msgSearch(name_party2); break; \
        case 6: name = msgSearch(name_party6); break; \
        } \
    } while (0)

    pouch = pouchGetPtr();

    {
    GXColor normal = {0, 0, 0, 0xFF};
    GXColor disabled = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor iconBase = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor titleBase = {0xFF, 0xFF, 0xFF, 0xFF};

    if ((*(u32*)(*(u8**)((u8*)wp + 4) + *(s32*)(sel + 0x2C) * 0x44) & 4) == 0) {
        GXColor fog = {0xFF, 0xFF, 0xFF, 0xFF};
        GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);

        GXGetScissor(&oldX, &oldY, &oldW, &oldH);
        GXSetScissor(
            *(s32*)(w + 0x18) + 0x130,
            0x112 - *(s32*)(w + 0x1C),
            *(s32*)(w + 0x20),
            *(s32*)(w + 0x24) - 0x32);

        rowPixel = 0;
        rowByte = 0;
        rowIndex = 0;

        while (rowIndex < *(s32*)(sel + 0x34)) {
            f32 y;
            s32 topY;
            s32 bottomY;

            y = *(f32*)(sel + 0x1C) +
                (f32)(*(s32*)(w + 0x1C) - 0x2C - rowPixel);
            topY = *(s32*)(w + 0x1C);
            bottomY = *(s32*)(w + 0x1C) - *(s32*)(w + 0x24);

            if ((y - 32.0f <= (f32)topY) &&
                ((f32)bottomY <= y + 32.0f)) {
                winFontInit();

                {
                Vec workPos = {0.0f, 0.0f, 0.0f};
                Vec textPos = {0.0f, 0.0f, 0.0f};
                Vec textScale = {1.0f, 1.0f, 1.0f};
                GXColor* colorPtr;
                GXColor textColor;
                u16 nameWidth;

                workPos.x = (f32)(*(s32*)(w + 0x18) + 0x3C);
                workPos.y = *(f32*)(sel + 0x1C) +
                    (f32)(*(s32*)(w + 0x1C) - 0x20 - rowPixel);
                textPos = workPos;

                GET_PARTY_NAME();
                nameWidth = FontGetMessageWidth(name);

                if (nameWidth > 0xAA) {
                    GET_PARTY_NAME();
                    textScale.x =
                        170.0f / (f32)(u16)FontGetMessageWidth(name);
                }

                colorPtr = &normal;
                if ((ROW_FLAGS() & 1) != 0) {
                    colorPtr = &disabled;
                }
                textColor = *colorPtr;

                GET_PARTY_NAME();
                winFontSet(&textPos, &textScale, &textColor, name);

                if ((ROW_FLAGS() & 1) == 0) {
                    winIconInit();
                } else {
                    winIconGrayInit();
                }

                {
                    Vec partyIconPos = {0.0f, 0.0f, 0.0f};
                    Vec partyIconScale = {1.0f, 1.0f, 1.0f};
                    GXColor partyIconColor;
                    s16 icon;

                    partyIconColor = iconBase;
                    partyIconPos.x =
                        (f32)(*(s32*)(w + 0x18) + 0x23);
                    partyIconPos.y = *(f32*)(sel + 0x1C) +
                        (f32)(*(s32*)(w + 0x1C) - 0x2C - rowPixel);
                    icon = party_icon_table[ROW_VALUE()];

                    if (icon == 0x160) {
                        s32 partyColor = pouchGetPartyColor(4);
                        switch (partyColor) {
                        case 0: icon = 0x160; break;
                        case 1: icon = 0x161; break;
                        case 2: icon = 0x162; break;
                        case 3: icon = 0x163; break;
                        case 4: icon = 0x164; break;
                        case 5: icon = 0x165; break;
                        default: icon = 0x166; break;
                        }
                    }

                    winIconSet(
                        icon, &partyIconPos, &partyIconScale, &partyIconColor);
                }

                {
                    GXColor techColor;
                    s32 techOffset = 0;
                    s32 techIndex = 0;

                    techColor = iconBase;

                    while (techIndex < pouch->partyData[
                               party_id_table[ROW_VALUE()]].techLevel) {
                        if ((ROW_FLAGS() & 1) == 0) {
                            winIconInit();
                        } else {
                            winIconGrayInit();
                        }

                        {
                        Vec techPos = {0.0f, 0.0f, 0.0f};
                        Vec techScale = {1.0f, 1.0f, 1.0f};

                        techPos.x = (f32)(*(s32*)(w + 0x18) +
                            techOffset + 0xFD);
                        techPos.y = *(f32*)(sel + 0x1C) +
                            (f32)(*(s32*)(w + 0x1C) -
                                  0x2C - rowPixel);

                        winIconSet(
                            0x10A, &techPos, &techScale, &techColor);
                        }

                        techOffset += 0x1B;
                        techIndex++;
                    }
                }
                }
            }

            rowPixel += 0x18;
            rowByte += 4;
            rowIndex++;
        }

        GXSetScissor(oldX, oldY, oldW, oldH);

        title = msgSearch(str_msg_window_title_7_80300858);
        width = (f32)(u16)FontGetMessageWidth(title);
        if (width > 100.0f) {
            width = 100.0f;
        }

        winFontInit();
        {
            GXColor titleColor;
            Vec titlePos = {0.0f, 0.0f, 0.0f};
            Vec titleScale = {1.0f, 1.0f, 1.0f};

            titleColor = titleBase;
            titlePos.x = (f32)*(s32*)(w + 0x18) +
                (f32)((*(s32*)(w + 0x20) * 3 / 5 - 120) * 2 / 3) +
                (120.0f - width) * 0.5f;
            titlePos.y = (f32)(*(s32*)(w + 0x1C) + 0xE);

            winFontSetEdgeWidth(
                &titlePos, &titleScale, &titleColor,
                100.0f, title);
        }

        title = msgSearch(str_msg_window_title_8_8030086c);
        width = (f32)(u16)FontGetMessageWidth(title);
        if (width > 65.0f) {
            width = 65.0f;
        }

        winFontInit();
        {
            GXColor titleColor;
            Vec titlePos = {0.0f, 0.0f, 0.0f};
            Vec titleScale = {1.0f, 1.0f, 1.0f};

            titleColor = titleBase;
            titlePos.x = (f32)*(s32*)(w + 0x18) +
                (f32)(*(s32*)(w + 0x20) * 3 / 5) +
                (f32)((*(s32*)(w + 0x20) * 2 / 5 - 85) / 3) +
                (85.0f - width) * 0.5f;
            titlePos.y = (f32)(*(s32*)(w + 0x1C) + 0xE);

            winFontSetEdgeWidth(
                &titlePos, &titleScale, &titleColor,
                65.0f, title);
        }

        {
            Vec cursorPos = {0.0f, 0.0f, 0.0f};

            cursorPos.x = *(f32*)(sel + 0x14);
            cursorPos.y = *(f32*)(sel + 0x18);
            iconDispGx(1.0, &cursorPos, 0x14, 0x1F8);
        }

        if ((*(s32*)(sel + 0x34) > 8) &&
            ((*(u32*)((u8*)gpGlobals + 0x10) & 0x1F) < 0x14)) {
            if (*(s32*)(sel + 0x10) != 0) {
                Vec upPos = {0.0f, 0.0f, 0.0f};

                upPos.x = (f32)*(s32*)(w + 0x18) +
                    (f32)*(s32*)(w + 0x20) * 0.5f;
                upPos.y = (f32)(*(s32*)(w + 0x1C) - 0x24);
                iconDispGx(0.6, &upPos, 0x10, 0x1BD);
            }

            if (*(s32*)(sel + 0x10) !=
                *(s32*)(sel + 0x34) - 8) {
                Vec downPos = {0.0f, 0.0f, 0.0f};

                downPos.x = (f32)*(s32*)(w + 0x18) +
                    (f32)*(s32*)(w + 0x20) * 0.5f;
                downPos.y = (f32)(*(s32*)(w + 0x1C) -
                    *(s32*)(w + 0x24) - 0xC);
                iconDispGx(0.6, &downPos, 0x10, 0x1BE);
            }
        }
    }
    }

#undef ROW_BASE
#undef ROW_FLAGS
#undef ROW_VALUE
#undef GET_PARTY_NAME
}

void select_disp_mario(void* win) {
    extern void* wp;
    extern void* gp;
    extern void pouchGetPtr(void);
    extern void GXSetFog(s32, f32, f32, f32, f32, u32*);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winFontInit(void);
    extern void winFontSet(Vec*, Vec*, void*, char*, ...);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32, Vec*, Vec*, void*);
    extern s32 sprintf(char*, const char*, ...);
    extern char* msgSearch(char*);
    extern u16 FontGetMessageWidth(char*);
    extern void winFontSetEdgeWidth(Vec*, Vec*, void*, f32, char*, ...);
    extern void iconDispGx(f64, Vec*, u16, u16);

    extern char vec3_803003a0[];
    extern char str_msg_window_title_9_80300830[];
    extern char str_msg_window_title_10_80300844[];
    extern char* lbl_803B5EA0[];
    extern s32 mario_status_icon_table[];
    extern s16* mario_status_point_table[];
    extern s32 mario_status_henka_table[];

    extern const u32 dat_80427fe4;
    extern const u32 dat_80427fe8;
    extern const u32 dat_80427fec;
    extern const u32 dat_80427ff0;
extern const f32 float_1_80428010;
    extern const f32 float_0p5_80428014;
    extern const f32 float_0_80428018;
    extern const f32 float_32_8042801c;
    extern const char str_PCT3d_80428024[4];
    extern const f32 float_120_80428028;
    extern const f32 float_0p6_8042802c;
    extern const f32 float_100_80428030;
    extern const f32 float_65_80428034;
    extern const f32 float_85_80428038;


    u8* w;
    u8* sel;
    u8* catalog;

    s32 oldX;
    s32 oldY;
    s32 oldW;
    s32 oldH;

    s32 rowPixel;
    s32 rowByte;
    s32 rowIndex;

    u32 normalBase;
    u32 disabledBase;
    u32 iconBase;
    u32 titleBase;
    u32 extraBase;
    u32 fog;

    char number[4];
    char* title;
    f32 width;

#define ROW_BASE() (*(u8**)(sel + 0x30))
#define ROW_FLAGS() (*(u16*)(ROW_BASE() + rowByte))
#define ROW_VALUE() (*(u16*)(ROW_BASE() + rowByte + 2))

    w = (u8*)win;
    sel = *(u8**)(w + 0x2C);
    catalog = (u8*)vec3_803003a0;

    pouchGetPtr();

    normalBase = dat_80427fe4;
    disabledBase = dat_80427fe8;
    titleBase = dat_80427fec;
    iconBase = dat_80427fec;
    extraBase = dat_80427fec;

    if ((*(u32*)(*(u8**)((u8*)wp + 4) +
                  *(s32*)(sel + 0x24) * 0x44) & 4) == 0) {
        fog = dat_80427ff0;

        GXSetFog(0, float_0_80428018, float_0_80428018,
                 float_0_80428018, float_0_80428018, &fog);

        GXGetScissor(&oldX, &oldY, &oldW, &oldH);
        GXSetScissor(
            *(s32*)(w + 0x18) + 0x130,
            0x112 - *(s32*)(w + 0x1C),
            *(s32*)(w + 0x20),
            *(s32*)(w + 0x24) - 0x32);

        rowPixel = 0;
        rowByte = 0;
        rowIndex = 0;

        while (rowIndex < *(s32*)(sel + 0x34)) {
            f32 rowY;
            s32 topY;
            s32 bottomY;
            u16 value;

            value = ROW_VALUE();
            rowY = *(f32*)(sel + 0x1C) +
                (f32)(*(s32*)(w + 0x1C) - 0x2C - rowPixel);
            topY = *(s32*)(w + 0x1C);
            bottomY = *(s32*)(w + 0x1C) - *(s32*)(w + 0x24);

            if ((rowY - float_32_8042801c <= (f32)topY) &&
                ((f32)bottomY <= rowY + float_32_8042801c)) {
                u32* colorPtr;
                u32 textColor;

                Vec nameWork;
                Vec namePos;
                Vec nameScale;

                winFontInit();

                colorPtr = &normalBase;
                if ((ROW_FLAGS() & 1) != 0) {
                    colorPtr = &disabledBase;
                }
                textColor = *colorPtr;

                nameScale = *(Vec*)(catalog + 0x180);
                nameWork = *(Vec*)(catalog + 0x174);
                nameWork.x = (f32)(*(s32*)(w + 0x18) + 0x3C);
                nameWork.y = *(f32*)(sel + 0x1C) +
                    (f32)(*(s32*)(w + 0x1C) - 0x20 - rowPixel);
                namePos = nameWork;

                winFontSet(
                    &namePos,
                    &nameScale,
                    &textColor,
                    lbl_803B5EA0[
                        *(s32*)((u8*)gp + 0x16C) * 3 + value]);

                if ((ROW_FLAGS() & 1) == 0) {
                    winIconInit();
                } else {
                    winIconGrayInit();
                }

                {
                    u32 iconColor;
                    Vec iconWork;
                    Vec iconPos;
                    Vec iconScale;

                    iconColor = iconBase;
                    iconScale = *(Vec*)(catalog + 0x198);
                    iconWork = *(Vec*)(catalog + 0x18C);
                    iconWork.x = (f32)(*(s32*)(w + 0x18) + 0x23);
                    iconWork.y = *(f32*)(sel + 0x1C) +
                        (f32)(*(s32*)(w + 0x1C) - 0x2C - rowPixel);
                    iconPos = iconWork;

                    winIconSet(
                        mario_status_icon_table[value],
                        &iconPos,
                        &iconScale,
                        &iconColor);
                }

                sprintf(number, str_PCT3d_80428024, (s32)*mario_status_point_table[value]);

                winFontInit();

                {
                    u32* currentColorPtr;
                    u32 currentColor;
                    Vec currentWork;
                    Vec currentPos;
                    Vec currentScale;

                    currentColorPtr = &normalBase;
                    if ((ROW_FLAGS() & 1) != 0) {
                        currentColorPtr = &disabledBase;
                    }
                    currentColor = *currentColorPtr;

                    currentScale = *(Vec*)(catalog + 0x1B0);
                    currentWork = *(Vec*)(catalog + 0x1A4);
                    currentWork.x =
                        (f32)(*(s32*)(w + 0x18) + 0xA0);
                    currentWork.y = *(f32*)(sel + 0x1C) +
                        (f32)(*(s32*)(w + 0x1C) - 0x20 - rowPixel);
                    currentPos = currentWork;

                    winFontSet(
                        &currentPos, &currentScale,
                        &currentColor, number);
                }

                if ((ROW_FLAGS() & 2) == 0) {
                    switch (*(s32*)(sel + 4)) {
                    case 9:
                        sprintf(
                            number,
                            str_PCT3d_80428024,
                            (s32)*mario_status_point_table[value] +
                                mario_status_henka_table[value]);
                        break;
                    default:
                        sprintf(
                            number,
                            str_PCT3d_80428024,
                            (s32)*mario_status_point_table[value] -
                                mario_status_henka_table[value]);
                        break;
                    }

                    winFontInit();

                    {
                        u32* futureColorPtr;
                        u32 futureColor;
                        Vec futureWork;
                        Vec futurePos;
                        Vec futureScale;

                        futureColorPtr = &normalBase;
                        if ((ROW_FLAGS() & 1) != 0) {
                            futureColorPtr = &disabledBase;
                        }
                        futureColor = *futureColorPtr;

                        futureScale = *(Vec*)(catalog + 0x1C8);
                        futureWork = *(Vec*)(catalog + 0x1BC);
                        futureWork.x =
                            (f32)(*(s32*)(w + 0x18) + 0xF0);
                        futureWork.y = *(f32*)(sel + 0x1C) +
                            (f32)(*(s32*)(w + 0x1C) - 0x20 - rowPixel);
                        futurePos = futureWork;

                        winFontSet(
                            &futurePos, &futureScale,
                            &futureColor, number);
                    }

                    winIconInit();

                    {
                        u32 changeColor;
                        Vec changeWork;
                        Vec changePos;
                        Vec changeScale;

                        changeColor = iconBase;
                        changeScale = *(Vec*)(catalog + 0x1E0);
                        changeWork = *(Vec*)(catalog + 0x1D4);
                        changeWork.x =
                            (f32)(*(s32*)(w + 0x18) + 0xDC);
                        changeWork.y = *(f32*)(sel + 0x1C) +
                            (f32)(*(s32*)(w + 0x1C) - 0x2C - rowPixel);
                        changePos = changeWork;

                        winIconSet(
                            0x16B, &changePos, &changeScale, &changeColor);
                    }
                }
            }

            rowPixel += 0x18;
            rowByte += 4;
            rowIndex++;
        }

        GXSetScissor(oldX, oldY, oldW, oldH);

        title = msgSearch(str_msg_window_title_9_80300830);
        width = (f32)(u16)FontGetMessageWidth(title);
        if (width > float_100_80428030) {
            width = float_100_80428030;
        }

        winFontInit();

        {
            u32 titleColor;
            Vec titleWork;
            Vec titlePos;
            Vec titleScale;

            titleColor = titleBase;
            titleScale = *(Vec*)(catalog + 0x1F8);
            titleWork = *(Vec*)(catalog + 0x1EC);
            titleWork.x =
                (f32)*(s32*)(w + 0x18) +
                (f32)((*(s32*)(w + 0x20) * 3 / 5 - 120) * 2 / 3) +
                (float_120_80428028 - width) * float_0p5_80428014;
            titleWork.y = (f32)(*(s32*)(w + 0x1C) + 0xE);
            titlePos = titleWork;

            winFontSetEdgeWidth(
                &titlePos, &titleScale, &titleColor, float_100_80428030, title);
        }

        title = msgSearch(str_msg_window_title_10_80300844);
        width = (f32)(u16)FontGetMessageWidth(title);
        if (width > float_65_80428034) {
            width = float_65_80428034;
        }

        winFontInit();

        {
            u32 titleColor;
            Vec titleWork;
            Vec titlePos;
            Vec titleScale;

            titleColor = titleBase;
            titleScale = *(Vec*)(catalog + 0x210);
            titleWork = *(Vec*)(catalog + 0x204);
            titleWork.x =
                (f32)*(s32*)(w + 0x18) +
                (f32)(*(s32*)(w + 0x20) * 3 / 5) +
                (f32)((*(s32*)(w + 0x20) * 2 / 5 - 85) / 3) +
                (float_85_80428038 - width) * float_0p5_80428014;
            titleWork.y = (f32)(*(s32*)(w + 0x1C) + 0xE);
            titlePos = titleWork;

            winFontSetEdgeWidth(
                &titlePos, &titleScale, &titleColor, float_65_80428034, title);
        }

        {
            Vec cursorPos;

            cursorPos = *(Vec*)(catalog + 0x21C);
            cursorPos.x = *(f32*)(sel + 0x14);
            cursorPos.y = *(f32*)(sel + 0x18);

            iconDispGx(float_1_80428010, &cursorPos, 0x14, 0x1F8);
        }

        if ((*(s32*)(sel + 0x34) > 8) &&
            ((*(u32*)((u8*)gp + 0x1C) & 0x1F) < 0x14)) {
            if (*(s32*)(sel + 0x10) != 0) {
                Vec upPos;

                upPos = *(Vec*)(catalog + 0x228);
                upPos.x =
                    (f32)*(s32*)(w + 0x18) +
                    (f32)*(s32*)(w + 0x20) * float_0p5_80428014;
                upPos.y = (f32)(*(s32*)(w + 0x1C) - 0x24);

                iconDispGx(float_0p6_8042802c, &upPos, 0x10, 0x1BD);
            }

            if (*(s32*)(sel + 0x10) !=
                *(s32*)(sel + 0x34) - 8) {
                Vec downPos;

                downPos = *(Vec*)(catalog + 0x234);
                downPos.x =
                    (f32)*(s32*)(w + 0x18) +
                    (f32)*(s32*)(w + 0x20) * float_0p5_80428014;
                downPos.y =
                    (f32)(*(s32*)(w + 0x1C) -
                          *(s32*)(w + 0x24) - 0xC);

                iconDispGx(float_0p6_8042802c, &downPos, 0x10, 0x1BE);
            }
        }
    }

#undef ROW_BASE
#undef ROW_FLAGS
#undef ROW_VALUE
}

void select_disp_luigi(void* win) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void GXGetScissor(u32*, u32*, u32*, u32*);
    extern void GXSetScissor(u32, u32, u32, u32);
    extern void GXSetFog(s32, f32, f32, f32, f32, u32*);
    extern void winFontInit(void);
    extern void winFontSetWidth(Vec*, Vec*, u32*, f32, char*, ...);
    extern void winFontSet(Vec*, Vec*, u32*, char*, ...);
    extern void winFontSetEdgeWidth(Vec*, Vec*, u32*, f32, char*, ...);
    extern char* msgSearch(char*);
    extern u16 FontGetMessageWidth(char*);
    extern void iconDispGx(f64, Vec*, u16, u16);
    extern void iconDispGxCol(f32[3][4], s32, u16, u32*);
    extern s32 sprintf(char*, const char*, ...);
    extern void* gpGlobals;
    extern u32 dat_80427ff4;
    extern u32 dat_80427ff8;
    extern u32 dat_80427ffc;
    extern u32 dat_80428000;
    extern u32 dat_80428004;
    extern Vec vec3_803005e0;
    extern Vec vec3_803005ec;
    extern Vec vec3_803005f8;
    extern Vec vec3_80300604;
    extern Vec vec3_80300610;
    extern Vec vec3_8030061c;

    extern char str_msg_window_title_6_8030081c[];
    extern u8 _jdt[];
    u8* entry = (u8*)win;
    u8* select = *(u8**)(entry + 0x2C);
    u32 oldX, oldY, oldW, oldH;
    u32 fog;
    u32 normal = dat_80427ff4;
    u32 disabled = dat_80427ff8;
    u32 titleColor = dat_80427ffc;
    u32 listColor;
    u32 numberColor;
    u32 titleDrawColor;
    Vec listPos, listScale;
    Vec numberPos, numberScale;
    Vec titlePos, titleScale;
    Vec griftyPos;
    Vec cursorPos;
    Vec upPos;
    Vec downPos;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    union { u32 word; f32 value; } iconScale;
    u32 iconColor;
    char number[32];
    char* title;
    f32 titleWidth;
    s32 i;

    if ((*(u32*)(*(u8**)((u8*)wp + 4) + *(s32*)(select + 0x2C) * 0x44) & 4) != 0)
        return;
    fog = dat_80428000;
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fog);
    GXGetScissor(&oldX, &oldY, &oldW, &oldH);
    GXSetScissor(*(s32*)(entry + 0x18) + 0x130,
                 0x112 - *(s32*)(entry + 0x1C),
                 *(s32*)(entry + 0x20), *(s32*)(entry + 0x24) - 0x32);
    listScale = vec3_803005ec;
    for (i = 0; i < *(s32*)(select + 0x34); i++) {
        f32 y = *(f32*)(select + 0x1C) + (f32)(*(s32*)(entry + 0x1C) - 0x20 - i * 0x18);
        if (y - 32.0f <= (f32)*(s32*)(entry + 0x1C) &&
            y + 32.0f >= (f32)(*(s32*)(entry + 0x1C) - *(s32*)(entry + 0x24))) {
            u8* rowInfo = _jdt + i * 0x10;
            char* message = *(char**)(rowInfo + 4);
            s16 messageEnd = *(s16*)(rowInfo + 8);
            u8 saved = message[messageEnd];
            winFontInit();
            message[messageEnd] = 0;
            listColor = (*(u16*)(*(u8**)(select + 0x30) + i * 4) & 1) ? disabled : normal;
            listPos.x = (f32)(*(s32*)(entry + 0x18) + 0x19);
            listPos.y = y;
            listPos.z = vec3_803005e0.z;
            winFontSetWidth(&listPos, &listScale, &listColor, 200.0f, message);
            message[messageEnd] = saved;
            if (*(s32*)(select + 4) == 0x11 &&
                (*(u16*)(*(u8**)(select + 0x30) + i * 4) & 1) == 0) {
                sprintf(number, "%3d", *(s16*)(rowInfo + 0xA));
                winFontInit();
                numberColor = (*(u16*)(*(u8**)(select + 0x30) + i * 4) & 1) ? disabled : normal;
                numberScale = vec3_80300604;
                numberPos.x = (f32)(*(s32*)(entry + 0x18) + 0xF5);
                numberPos.y = y;
                numberPos.z = vec3_803005f8.z;
                winFontSet(&numberPos, &numberScale, &numberColor, number);
            }
        }
    }
    GXSetScissor(oldX, oldY, oldW, oldH);
    title = msgSearch(*(s32*)(select + 4) == 0x11 ? str_msg_window_title_5_80300808 : str_msg_window_title_6_8030081c);
    titleWidth = (f32)FontGetMessageWidth(title);
    if (titleWidth > 120.0f) titleWidth = 120.0f;
    winFontInit();
    titleDrawColor = titleColor;
    titleScale = vec3_8030061c;
    titlePos.x = (f32)*(s32*)(entry + 0x18) + ((f32)*(s32*)(entry + 0x20) - titleWidth) * 0.5f;
    titlePos.y = (f32)(*(s32*)(entry + 0x1C) + 0xE);
    titlePos.z = vec3_80300610.z;
    winFontSetEdgeWidth(&titlePos, &titleScale, &titleDrawColor, 120.0f, title);
    if (*(s32*)(select + 4) == 0x11) {
        iconScale.word = 0x3F19999A;
        PSMTXTrans(transMtx, (f32)(*(s32*)(entry + 0x18) + 0x10C),
                    (f32)(*(s32*)(entry + 0x1C) - 0x12), 0.0f);
        PSMTXScale(scaleMtx, iconScale.value, iconScale.value, iconScale.value);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        iconColor = dat_80428004;
        iconDispGxCol(transMtx, 0x10, 0x213, &iconColor);
        griftyPos.x = (f32)(*(s32*)(entry + 0x18) + 0x10C);
        griftyPos.y = (f32)(*(s32*)(entry + 0x1C) - 0xC);
        griftyPos.z = 0.0f;
        iconDispGx((f64)iconScale.value, &griftyPos, 0x10, 0x193);
    }
    cursorPos.x = *(f32*)(select + 0x14);
    cursorPos.y = *(f32*)(select + 0x18);
    cursorPos.z = 0.0f;
    iconDispGx(1.0, &cursorPos, 0x14, 0x1F8);
    if (*(s32*)(select + 0x34) > 8 && ((*(u32*)((u8*)gpGlobals + 0x10) & 0x1F) < 0x14)) {
        if (*(s32*)(select + 0x10) != 0) {
            upPos.x = (f32)*(s32*)(entry + 0x18) + (f32)*(s32*)(entry + 0x20) * 0.5f;
            upPos.y = (f32)(*(s32*)(entry + 0x1C) - 0x24);
            upPos.z = 0.0f;
            iconDispGx(0.6, &upPos, 0x10, 0x1BD);
        }
        if (*(s32*)(select + 0x10) != *(s32*)(select + 0x34) - 8) {
            downPos.x = (f32)*(s32*)(entry + 0x18) + (f32)*(s32*)(entry + 0x20) * 0.5f;
            downPos.y = (f32)(*(s32*)(entry + 0x1C) - *(s32*)(entry + 0x24) - 0xC);
            downPos.z = 0.0f;
            iconDispGx(0.6, &downPos, 0x10, 0x1BE);
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void select_disp2(void* win) {
    extern char* msgSearch(char*);
    extern u32 FontGetMessageWidthLine(char*, u16*);
    extern void FontDrawMessageMtx(f32[3][4], char*);

    typedef struct DescMini {
        u8 pad00[0x10];
        s32 y;
        u8 pad14[4];
        s32 height;
    } DescMini;
    typedef struct EntryMini {
        u32 flags;
        u8 pad04[0x14];
        s32 x;
        s32 y;
        s32 width;
        s32 height;
        u8 pad28[0x1C];
    } EntryMini;
    typedef struct WorkMini {
        u8 pad00[4];
        EntryMini* entries;
    } WorkMini;
    typedef struct SelectMini {
        u8 pad00[4];
        s32 type;
        u8 pad08[0x1C];
        s32 entryIndices[3];
    } SelectMini;
    typedef struct WindowMini {
        u8 pad00[0x18];
        s32 x;
        s32 y;
        s32 width;
        s32 height;
        DescMini* desc;
        SelectMini* select;
    } WindowMini;

    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    WindowMini* window = win;
    SelectMini* select = window->select;
    char* strings = vec3_803003a0;
    char* message = msgSearch(strings + 0x358);
    EntryMini* linked;
    u16 lines;
    u16 messageWidth;
    f32 drawnWidth;
    f32 x;
    f32 y;

    switch (select->type) {
        case 3:
        case 0xC:
            message = msgSearch(strings + 0x36C);
            break;
        case 4:
            message = msgSearch(strings + 0x380);
            break;
        case 5:
            message = msgSearch(strings + 0x394);
            break;
        case 6:
        case 7:
            message = msgSearch(strings + 0x3A8);
            break;
        case 8:
            message = msgSearch(strings + 0x3C0);
            break;
        case 9:
            message = msgSearch(strings + 0x3D8);
            break;
        case 10:
            message = msgSearch(strings + 0x3EC);
            break;
        case 0xB:
        case 0xE:
        case 0xF:
            message = msgSearch(strings + 0x404);
            break;
        case 0xD:
        case 0x10:
            message = msgSearch(strings + 0x418);
            break;
        case 0x11:
            message = msgSearch(strings + 0x430);
            break;
        case 0x12:
            message = msgSearch(strings + 0x444);
            break;
    }

    if ((((WorkMini*)wp)->entries[select->entryIndices[1]].flags & 4) == 0) {
        messageWidth = FontGetMessageWidthLine(message, &lines);
        drawnWidth = (f32)messageWidth;
        if (drawnWidth <= (f32)(window->width - 30)) {
            PSMTXScale(scaleMtx, 1.0f, 1.0f, 1.0f);
        } else {
            PSMTXScale(scaleMtx, (f32)(window->width - 30) / drawnWidth, 1.0f, 1.0f);
            drawnWidth = (f32)(window->width - 30);
        }
        x = (f32)(window->x + (u16)(((f32)window->width - drawnWidth) * 0.5f));
        y = (f32)(window->y - (window->height - (lines + 1) * 24) / 2);
        PSMTXTrans(transMtx, x, y, 0.0f);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        FontDrawStart();
        FontDrawMessageMtx(transMtx, message);

        linked = &((WorkMini*)wp)->entries[select->entryIndices[1]];
        if ((linked->flags & 1) != 0) {
            linked->x = window->x;
            linked->y = window->desc->y + ((u32)lines * 22 >> 1);
            linked->width = window->width;
            linked->height = window->desc->height + (u32)lines * 22;
        }
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void select_main3(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

void select_disp3(void* win) {
    typedef struct SelectEntry {
        u16 unk0;
        u16 item;
    } SelectEntry;

    void* data = *(void**)((s32)win + 0x2C);
    s32 id = *(s32*)((s32)data + 0x2C);
    void* mgr = *(void**)((s32)wp + 4);

    if ((*(u32*)((s32)mgr + id * 0x44) & 4) == 0) {
        s32 index = *(s32*)((s32)data + 0xC);
        u8* table = (u8*)itemDataTable;
        SelectEntry* list = *(SelectEntry**)((s32)data + 0x30);
        u16 item = list[index].item;

        *(char**)((s32)win + 0x34) = msgSearch(*(char**)(table + item * 0x28 + 8));
        winMgrHelpDraw(win);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void select_disp3_party(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);
    extern void winMgrHelpDraw(void* win);
    extern char vec3_803003a0[];
    extern char str_help_80428008[];
    extern u16 party_id_table[];
    extern char* party_labelname_table[];

    char buf[0x50];
    void* data;
    void* list;
    void* pouch;
    u16 flags;
    u16 party;
    char* label;
    char* state;
    s32 index;
    s32 count;
    char* base;

    base = vec3_803003a0;
    data = *(void**)((s32)win + 0x2C);
    pouch = pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        label = str_help_80428008;
        if (flags & 2) {
            label = &base[0x30C];
        }
        if (*(s32*)((s32)data + 4) == 6 || *(s32*)((s32)data + 4) == 7) {
            state = &base[0x334];
        } else {
            state = &base[0x33C];
        }
        party = *(u16*)((s32)list + (index << 2) + 2);
        count = *(s16*)((s32)pouch + party_id_table[party] * 0xE + 0xC) + 1;
        sprintf(buf, &base[0x348], label, state, count, party_labelname_table[party]);
        *(char**)((s32)win + 0x34) = msgSearch(buf);
        winMgrHelpDraw(win);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

/* Reorder pilot candidate: functions arranged in _main.map order for main/manager/winmgr. */

void select_disp3_mario(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);





    char buf[0x50];
    void* data;
    void* list;
    u16 flags;
    s32 index;
    char* label;
    u16 status;

    data = *(void**)((s32)win + 0x2C);
    pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        if (flags & 2) {
            if (flags & 4) {
                label = &vec3_803003a0[0x300];
            } else {
                label = &vec3_803003a0[0x30C];
            }
        } else {
            label = str_help_80428008;
        }
        status = *(u16*)((s32)list + (index << 2) + 2);
        if (*(s32*)((s32)data + 4) == 9) {
            sprintf(buf, &vec3_803003a0[0x314], label, mario_status_name_table[status]);
        } else {
            sprintf(buf, &vec3_803003a0[0x324], label, mario_status_name_table[status]);
        }
        *(char**)((s32)win + 0x34) = msgSearch(buf);
        winMgrHelpDraw(win);
    }
}

/* select_disp_mario canonical .sdata2 scalars: intentionally late so the
 * existing compiler-owned literal pool keeps its accepted order/offsets. */
const u32 dat_80427fe4 = 0x000000FF;
const u32 dat_80427fe8 = 0xA0A0A0FF;
const u32 dat_80427fec = 0xFFFFFFFF;
const u32 dat_80427ff0 = 0xFFFFFFFF;
const f32 float_1_80428010 = 1.0f;
const f32 float_0p5_80428014 = 0.5f;
const f32 float_0_80428018 = 0.0f;
const f32 float_32_8042801c = 32.0f;
const char str_PCT3d_80428024[4] = "%3d";
const f32 float_120_80428028 = 120.0f;
const f32 float_0p6_8042802c = 0.6f;
const f32 float_100_80428030 = 100.0f;
const f32 float_65_80428034 = 65.0f;
const f32 float_85_80428038 = 85.0f;
