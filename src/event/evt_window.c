#include "event/evt_window.h"
#include "event/evt_cmd.h"

extern void* mapalloc_base_ptr;
extern void* coin_desc;
extern s32 dat_80426e98;
extern const char str_msg_you_gotta_mail_802fe4d4[];

void nameEntOff(void);
s32 nameEntWait(void);
s32 nameEntPrepare(void);
void nameEntOn(s32 value);
s32 nameEntGetName(void);
void winOpenDisable(void);
void winOpenEnable(void);
void* _mapAlloc(void* heap, u32 size);
void _mapFree(void* heap, void* ptr);
s32 pouchGetCoin(void);
s32 pouchGetSuperCoin(void);
s32 pouchGetStarPiece(void);
s32 pouchCheckItem(s32 item);
void* winMgrEntry(void* desc);
void winMgrSetParam(void* win, void* param);
void winMgrOpen(void* win);
void winMgrCloseAutoDelete(void* win);
s32 winMgrAction(void* win);
void* winMgrSelectEntry(void* table, s32 start, s32 type);
s32 winMgrSelectOther(void* select, EventEntry* event);
s32 winMgrSelect(void* select);
s32 unk_8023cf04(void* select);
void winMgrSelectDelete(void* select);
void marioKeyOff(void);
void marioKeyOn(void);
char* msgSearch(const char* msg);
void FontDrawStart(void);
void FontDrawColor(void* color);
s32 FontGetMessageWidth(char* msg);
void FontDrawMessage(s32 x, s32 y, char* msg);
USER_FUNC(evt_win_item_select) {
    s32* args = event->args;
    s32 writeIndex = evtGetValue(event, args[0]);
    void* table = (void*)evtGetValue(event, args[1]);
    void* select;
    s32 result;
    s32 index;

    if (isFirstCall != 0) {
        *(void**)((s32)event + 0x78) = winMgrSelectEntry(table, 0, 1);
        marioKeyOff();
    }
    select = *(void**)((s32)event + 0x78);
    result = winMgrSelect(select);
    if (result == 0) {
        return 0;
    }
    index = unk_8023cf04(select);
    winMgrSelectDelete(select);
    marioKeyOn();
    evtSetValue(event, args[2], result);
    if (writeIndex != 0) {
        evtSetValue(event, args[3], index);
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_win_other_select) {
    s32* args = event->args;
    void* table = (void*)evtGetValue(event, args[0]);
    void* select;
    s32 result;

    if (isFirstCall != 0) {
        *(void**)((s32)event + 0x78) = winMgrSelectEntry(table, 0, 1);
        marioKeyOff();
    }
    select = *(void**)((s32)event + 0x78);
    result = winMgrSelectOther(select, event);
    if (result == 0) {
        return 0;
    }
    winMgrSelectDelete(select);
    marioKeyOn();
    if (result == -1) {
        *(s32*)((s32)event + 0x9C) = 0;
    } else {
        *(s32*)((s32)event + 0x9C) = 1;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_win_item_maketable) {
    s32* args = event->args;
    s32* src = (s32*)evtGetValue(event, args[0]);
    s32* scan = src;
    s32* out;
    s32* dst;
    s32 count = 0;
    s32 kept = 0;
    s32 i;

    while (*scan != -1) {
        count++;
        scan++;
    }
    if (count == 0) {
        evtSetValue(event, args[1], 0);
        return 2;
    }
    out = _mapAlloc(mapalloc_base_ptr, (count + 1) * 4);
    dst = out;
    scan = src;
    for (i = 0; i < count; i++) {
        if (pouchCheckItem(*scan) != 0) {
            *dst++ = *scan;
            kept++;
        }
        scan++;
    }
    out[kept] = -1;
    if (kept == 0) {
        out = NULL;
    }
    evtSetValue(event, args[1], (s32)out);
    return 2;
}
USER_FUNC(evt_win_item_desttable) {
    void* ptr = (void*)evtGetValue(event, event->args[0]);

    _mapFree(mapalloc_base_ptr, ptr);
    return 2;
}

USER_FUNC(evt_win_nameent_on) {
    s32 value = evtGetValue(event, event->args[0]);

    if (nameEntPrepare() == 0) {
        return 0;
    }
    nameEntOn(value);
    return 2;
}

USER_FUNC(evt_win_nameent_wait) {
    if (nameEntWait() != 0) {
        return 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_win_nameent_name) {
    s32* args = event->args;
    s32 name = nameEntGetName();

    evtSetValue(event, args[0], name);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_win_nameent_off) {
    nameEntOff();
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void oneMessageDisp(void* entry) {
    void* data = *(void**)((s32)entry + 0x2C);
    char* msg;
    s32 width;
    s32 x;
    s32 color;

    if (winMgrAction(*(void**)((s32)data + 8)) != 0) {
        return;
    }
    if (*(s32*)((s32)data + 4) == 0) {
        msg = msgSearch(str_msg_you_gotta_mail_802fe4d4);
    }
    FontDrawStart();
    color = dat_80426e98;
    FontDrawColor(&color);
    width = FontGetMessageWidth(msg) & 0xFFFF;
    x = -width;
    FontDrawMessage((x + ((u32)x >> 31)) >> 1, *(s32*)((s32)entry + 0x1C) - 8, msg);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



/* CHATGPT STUB FILL: main/event/evt_window 20260624_184128 */

/* stub-fill: evt_win_one_message | missing_definition | ghidra_signature */
s32 evt_win_one_message(EventEntry* event, s32 isFirstCall) {
    extern void* memset(void* dst, int value, u32 size);
    extern void* one_msg_desc;
    extern u16 keyGetButtonTrg(s32 controller);
    extern void* marioGetPtr(void);
    extern u32 psndSFXOn_3D(const char* id, void* position);
    extern const char str_SFX_MAIL_RECEPTION1_802fe4c0[];
    extern void winMgrClose(void* win);
    extern void winMgrDelete(void* win);
    s32 value;
    s32* work;
    s32 state;

    value = evtGetValue(event, *(s32*)event->args);
    if (isFirstCall != 0) {
        work = _mapAlloc(mapalloc_base_ptr, 0xC);
        *(s32**)((s32)event + 0x78) = work;
        memset(work, 0, 0xC);
        work[2] = (s32)winMgrEntry(&one_msg_desc);
        work[0] = 0;
        work[1] = value;
        winMgrSetParam((void*)work[2], work);
    }

    work = *(s32**)((s32)event + 0x78);
    state = work[0];
    if (state == 3) {
        winMgrClose((void*)work[2]);
        work[0]++;
    } else if (state < 3) {
        if (state == 1) {
            if (winMgrAction((void*)work[2]) == 0) {
                work[0]++;
            }
        } else if (state < 1) {
            if (state >= 0) {
                winMgrOpen((void*)work[2]);
                work[0]++;
            }
        } else if ((keyGetButtonTrg(0) & 0x100) != 0) {
            if (work[1] == 0) {
                psndSFXOn_3D(str_SFX_MAIL_RECEPTION1_802fe4c0, (void*)((s32)marioGetPtr() + 0x8C));
            }
            work[0]++;
        }
    } else {
        if (state == 5) {
            winMgrDelete((void*)work[2]);
            _mapFree(mapalloc_base_ptr, work);
            return 2;
        }
        if (state < 5 && winMgrAction((void*)work[2]) == 0) {
            work[0]++;
        }
    }
    return 0;
}

/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_window 20260624_191429 */

/* fallback stub-fill: map=coin_disp addr=0x80209c44 size=0x000001fc */
void coin_disp(void* win) {
    typedef f32 Mtx[3][4];
    extern void* pouchGetPtr(void);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void iconDispGx2(Mtx m, s32 flags, u16 icon);
    extern void iconNumberDispGx(Mtx m, s32 number, s32 flags, void* color);
    extern void statusWinForceUpdateCoin(void);
    extern s32 psndSFXOn(const char* name);
    extern s32 unk_8041ed08;
    extern u32 dat_80426e9c;
    extern f32 float_neg220_80426ea0;
    extern f32 float_neg113_80426ea4;
    extern f32 float_0_80426ea8;
    extern f32 float_0p7_80426eac;
    extern f32 float_neg192_80426eb0;
    extern f32 float_neg126_80426eb4;
    void* work;
    s32 current;
    u16 icon;
    Mtx trans;
    Mtx scale;
    u32 color;
    s32 diff;
    s32 step;
    s32 timer;

    pouchGetPtr();
    work = *(void**)((s32)win + 0x2C);
    if ((*(u32*)win & 4) != 0) {
        return;
    }

    switch (*(s32*)((s32)work + 0xC)) {
        case 1:
            current = pouchGetSuperCoin();
            icon = 0x10A;
            break;
        case 0:
            current = pouchGetCoin();
            icon = 0x193;
            break;
        case 2:
            current = pouchGetStarPiece();
            icon = 0x195;
            break;
    }

    PSMTXTrans(trans, (double)float_neg220_80426ea0, (double)float_neg113_80426ea4, (double)float_0_80426ea8);
    PSMTXScale(scale, float_0p7_80426eac, float_0p7_80426eac, float_0p7_80426eac);
    PSMTXConcat(trans, scale, trans);
    iconDispGx2(trans, 0x10, icon);
    PSMTXTrans(trans, (double)float_neg192_80426eb0, (double)float_neg113_80426ea4, (double)float_0_80426ea8);
    iconDispGx2(trans, 0x10, 0x1DE);
    PSMTXTrans(trans, (double)float_neg126_80426eb4, (double)float_neg113_80426ea4, (double)float_0_80426ea8);
    color = dat_80426e9c;
    iconNumberDispGx(trans, *(s32*)((s32)work + 4), 0, &color);

    diff = current - *(s32*)((s32)work + 4);
    if (diff != 0) {
        step = (diff / 10) + (diff >> 31);
        step -= step >> 31;
        if (step == 0) {
            step = -1;
            if (*(s32*)((s32)work + 4) < current) {
                step = 1;
            }
        }
        if ((*(u32*)((s32)work + 8) & 1) != 0) {
            *(s32*)((s32)work + 4) += step;
            statusWinForceUpdateCoin();
        }
        *(s32*)((s32)work + 8) += 1;
        timer = *(s32*)((s32)work + 8);
        step = (timer / 0x4B0) + (timer >> 31);
        *(s32*)((s32)work + 8) = timer + (step - (step >> 31)) * -0x4B0;
        if (unk_8041ed08 == 0) {
            psndSFXOn((const char*)0x2005B);
            unk_8041ed08 = 7;
        }
    }
    if (unk_8041ed08 > 0) {
        unk_8041ed08--;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_win_coin_on) {
    void* work;
    s32* args;
    s32 type;

    args = event->args;
    work = _mapAlloc(mapalloc_base_ptr, 0x10);
    type = evtGetValue(event, args[0]);

    *(s32*)((s32)work + 0xC) = type;
    *(void**)work = winMgrEntry(coin_desc);
    *(s32*)((s32)work + 8) = 0;
    switch (type) {
        case 0:
            *(s32*)((s32)work + 4) = pouchGetCoin();
            break;
        case 1:
            *(s32*)((s32)work + 4) = pouchGetSuperCoin();
            break;
        case 2:
            *(s32*)((s32)work + 4) = pouchGetStarPiece();
            break;
    }
    winMgrSetParam(*(void**)work, work);
    winMgrOpen(*(void**)work);
    evtSetValue(event, args[1], (s32)work);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_win_coin_off) {
    s32* args = event->args;
    void* work = (void*)evtGetValue(event, args[0]);
    s32 current;

    switch (*(s32*)((s32)work + 0xC)) {
        case 0:
            current = pouchGetCoin();
            break;
        case 1:
            current = pouchGetSuperCoin();
            break;
        case 2:
            current = pouchGetStarPiece();
            break;
        default:
            current = 0;
            break;
    }
    if (*(s32*)((s32)work + 4) != current) {
        return 0;
    }
    winMgrCloseAutoDelete(*(void**)work);
    _mapFree(mapalloc_base_ptr, work);
    return 2;
}

USER_FUNC(evt_win_coin_wait) {
    s32* args = event->args;
    void* work = (void*)evtGetValue(event, args[0]);
    s32 current;
    s32 target;
    s32 diff;

    switch (*(s32*)((s32)work + 0xC)) {
        case 0:
            current = pouchGetCoin();
            break;
        case 1:
            current = pouchGetSuperCoin();
            break;
        case 2:
            current = pouchGetStarPiece();
            break;
        default:
            current = 0;
            break;
    }
    target = *(s32*)((s32)work + 4);
    diff = ((target - current) | (current - target)) >> 31;
    return 2 & ~diff;
}

USER_FUNC(mainwindow_openenable) {
    winOpenEnable();
    return 2;
}

USER_FUNC(mainwindow_opendisable) {
    winOpenDisable();
    return 2;
}

