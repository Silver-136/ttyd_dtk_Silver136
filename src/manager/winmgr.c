// Window manager: menu windows built from descriptors, and the selection lists of shops, partners,
// level-ups, badges and the informant (winMgrSelect*).

#include "version.h"
#include "icon_ids.h"
#include "manager/winmgr.h"
#include "data/item_data.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_lottery.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <stdio.h>
#include <string.h>
#include "win/win_item.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "statuswindow.h"
#include "win/win_main.h"

typedef struct WinMgrSelectTbl {
    u16 numDescriptors; // 0x0
    WinMgrDesc* descriptors; // 0x4
} WinMgrSelectTbl;

void qqsort(void* base, u32 n, u32 size, int (*compare)(void*, void*));
#if VERSION >= VERSION_US
u16 pouchEquipBadge(s32 index);
int select_comp_bteresa(void* a, void* b);
#endif

void select_main(WinMgrEntry* entry);
void select_disp(WinMgrEntry* entry);
void select_disp_party(WinMgrEntry* entry);
void select_disp_mario(WinMgrEntry* entry);
void select_disp_luigi(WinMgrEntry* entry);
void select_disp2(WinMgrEntry* entry);
void select_main3(WinMgrEntry* entry);
void select_disp3(WinMgrEntry* entry);
void select_disp3_party(WinMgrEntry* entry);
void select_disp3_mario(WinMgrEntry* entry);
void winMgrSeq(WinMgrEntry* entry);
int select_comp_type(void* a, void* b);
int select_comp_starpiece(void* a, void* b);

//.sbss
static WinMgrWork work;
static s16* mario_status_point_table[3];

//.sdata
static WinMgrWork* wp = &work;
#if VERSION <= VERSION_DEMO_US
static f32 unusedSignTbl[16] = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
#endif
static u16 party_id_table[] = {
    PARTNER_GOOMBELLA, PARTNER_KOOPS, PARTNER_FLURRIE, PARTNER_YOSHI,
    PARTNER_VIVIAN, PARTNER_BOBBERY, PARTNER_MSMOWZ, PARTNER_NONE
};
static u16 party_icon_table[] = {ICON_PARTY_GOOMBELLA, ICON_PARTY_KOOPS, ICON_PARTY_FLURRIE, ICON_PARTY_YOSHI, ICON_PARTY_VIVIAN, ICON_PARTY_BOBBERY, ICON_PARTY_MS_MOWZ};
static const char* party_labelname_table[] = {"_kur", "_nok", "_win", "_yos", "_viv", "_bom", "_chu"};
static s32 mario_status_icon_table[] = {ICON_HP, ICON_FP, ICON_BP};
static const char* mario_status_labelname_table[] = {"_hp", "_fp", "_bp"};
#if VERSION >= VERSION_US
static const char* mario_status_name_table[][3] = {
    {"\x82\x67\x82\x6F", "\x82\x65\x82\x6F", "\x82\x61\x82\x6F"},
    {"HP", "FP", "BP"},
    {"KP", "BP", "OP"},
    {"PC", "PF", "PB"},
    {"PC", "PF", "PM"},
    {"PV", "PF", "PT"},
    {"HP", "FP", "BP"},
};
#define MARIO_STATUS_NAME(id) mario_status_name_table[gp->language][id]
#else
static const char* mario_status_name_table[] = {"\x82\x67\x82\x6F", "\x82\x65\x82\x6F", "\x82\x61\x82\x6F"};
#define MARIO_STATUS_NAME(id) mario_status_name_table[id]
#endif
/** What a level-up adds to Mario's HP, FP and BP: 5, 5 and 3. */
static s32 mario_status_henka_table[] = {5, 5, 3};
/** The ceiling of Mario's HP, FP and BP (200, 200 and 99). */
#if VERSION > VERSION_DEMO_US
s32 mario_status_max_table[] = {200, 200, 99};
#endif

//.data
WinMgrDesc select_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3},
#if VERSION > VERSION_DEMO_US
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3},
#endif
};

/** Windows of the partner selection menu: the list, the side window and the help box. */
WinMgrDesc select_party_desc[] = {
    {0, 4, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_party},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3_party},
};

/** Windows of the Mario status selection menu (HP, FP, BP): the list, the side window and the help box. */
WinMgrDesc select_mario_desc[] = {
    {0, 5, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_mario},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3_mario},
};

/** Windows of the menu type drawn by select_disp_luigi: the list and the side window (no help box). */
WinMgrDesc select_luigi_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_luigi},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
};

WinMgrDesc select_johoya_perfect_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_luigi},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
};

/**
 * Window set (count and descriptors) of each selection menu type,
 * indexed by the type passed to winMgrSelectEntry.
 */
WinMgrSelectTbl select_desc_tbl[] = {
    {3, select_desc},
    {3, select_desc},
    {3, select_desc},
    {3, select_desc},
    {3, select_desc},
    {3, select_desc},
    {3, select_party_desc},
    {3, select_party_desc},
    {3, select_party_desc},
    {3, select_mario_desc},
    {3, select_mario_desc},
    {3, select_desc},
    {3, select_desc},
#if VERSION > VERSION_DEMO_US
    {3, &select_desc[3]},
#endif
    {3, select_desc},
    {3, select_desc},
#if VERSION <= VERSION_DEMO_US
    {3, select_desc},
    {3, select_desc},
#else
    {3, &select_desc[3]},
#endif
    {2, select_johoya_perfect_desc},
    {2, select_luigi_desc},
};

//.rodata (defined at the end of the file)
static const char name_party0[12];
static const char name_party1[12];
static const char name_party4[12];
static const char name_party5[12];
static const char name_party2[12];
static const char name_party6[12];

static inline const char* select_party_name(u16 id) {
    const char* name;
    switch (id) {
        case 0:
            name = msgSearch(name_party0);
            break;
        case 1:
            name = msgSearch(name_party1);
            break;
        case 2:
            name = msgSearch(name_party4);
            break;
        case 3:
            name = pouchGetYoshiName();
            break;
        case 4:
            name = msgSearch(name_party5);
            break;
        case 5:
            name = msgSearch(name_party2);
            break;
        case 6:
            name = msgSearch(name_party6);
            break;
    }
    return name;
}

// Descriptive name (not in the symbol maps); placeholder unk_80239b2c, JP 0x80239B2C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
USER_FUNC(evt_win_party_lankup_chk) {
    PouchData* pouch;
    u16* p;
    EvtWord* args = event->args;
    s32 max;

    pouch = pouchGetPtr();
    for (p = party_id_table; *p != 0; p++) {
        if (partyChkJoin(*p)) {
            if (!evtGetValue(event, GSWF_GOR_UP_ARROW_USED)) {
                if (pouch->partyData[*p].techLevel < 1) {
                    max = 0;
                } else {
                    max = 1;
                }
            } else {
                if (pouch->partyData[*p].techLevel < 2) {
                    max = 0;
                } else {
                    max = 2;
                }
            }
            if (max == 0) {
                evtSetValue(event, args[0], 1);
                return EVT_RETURN_DONE;
            }
        }
    }
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}
#endif

/**
 * The power-down shop's check (Rogueport Sewers, tik_11) before trading partner ranks:
 * sets args[0] to 1 when at least two partners have joined,
 * one of them can still be ranked up (same limit as evt_win_party_lankup_chk) and one has a rank to give;
 * otherwise 0.
 */
// Descriptive name (not in the symbol maps); placeholder unk_802399f8, JP 0x802399F8. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
USER_FUNC(evt_win_party_lankdown_chk) {
    PouchData* pouch;
    u16* p;
    EvtWord* args = event->args;
    s32 num, canRank, hasRank;
    s32 max;

    pouch = pouchGetPtr();
    canRank = 0;
    num = 0;
    hasRank = 0;
    for (p = party_id_table; *p != 0; p++) {
        if (partyChkJoin(*p)) {
            num++;
            if (pouch->partyData[*p].techLevel != 0) {
                hasRank = 1;
            }
            if (!evtGetValue(event, GSWF_GOR_UP_ARROW_USED)) {
                if (pouch->partyData[*p].techLevel < 1) {
                    max = 0;
                } else {
                    max = 1;
                }
            } else {
                if (pouch->partyData[*p].techLevel < 2) {
                    max = 0;
                } else {
                    max = 2;
                }
            }
            if (max == 0) {
                canRank = 1;
            }
        }
    }
    if (num > 1 && canRank && hasRank) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
#endif

/** Allocates and clears the 32 window slots at boot (called from mariost.c). */
void winMgrInit(void) {
    wp->numEntries = 32;
    wp->entries = (WinMgrEntry*)__memAlloc(HEAP_DEFAULT, wp->numEntries * sizeof(WinMgrEntry));
    memset(wp->entries, 0, wp->numEntries * sizeof(WinMgrEntry));
}

/** Clears every window slot; called on a map change so no window survives into the next map. */
void winMgrReInit(void) {
    memset(wp->entries, 0, wp->numEntries * sizeof(WinMgrEntry));
}

/**
 * Display callback of one window: draws the frame in its descriptor colour,
 * rotated and scaled by the open/close animation, then the blue title tabs for dispType 1 (one),
 * 3 (two) or 4/5 (two, list and value columns), then the descriptor's own disp function.
 */
void winMgrDisp(CameraId camId, void* param) {
    WinMgrEntry* entry = param;
    Mtx rot;
    Mtx scale;

    PSMTXRotRad(rot, 'z', 0.017453292f * entry->rotation);
    PSMTXScale(scale, entry->scale, entry->scale, entry->scale);
    PSMTXConcat(rot, scale, scale);
    windowDispGX2_Waku_col(scale, 0, (GXColor){entry->description->color.r, entry->description->color.g, entry->description->color.b, entry->alpha},
                           entry->x, entry->y, entry->width, entry->height, 20.0f);
    switch (entry->description->dispType) {
        case 0:
        case 2:
            break;
        case 1:
            windowDispGX2_Waku_col(scale, 0, (GXColor){0x2B, 0xA4, 0xFF, entry->alpha}, entry->x + (entry->width - 150) / 2, entry->y + 14, 150.0f, 28.0f, 8.0f);
            break;
        case 3:
            windowDispGX2_Waku_col(scale, 0, (GXColor){0x2B, 0xA4, 0xFF, entry->alpha}, entry->x + 10, entry->y + 14, 100.0f, 28.0f, 8.0f);
            windowDispGX2_Waku_col(scale, 0, (GXColor){0x2B, 0xA4, 0xFF, entry->alpha}, entry->x + entry->width / 2 - 10, entry->y + 14, 100.0f, 28.0f, 8.0f);
            break;
        case 4:
        case 5:
            windowDispGX2_Waku_col(scale, 0, (GXColor){0x2B, 0xA4, 0xFF, entry->alpha}, entry->x + ((entry->width * 3 / 5 - 120) * 2) / 3, entry->y + 14, 120.0f, 28.0f, 8.0f);
            windowDispGX2_Waku_col(scale, 0, (GXColor){0x2B, 0xA4, 0xFF, entry->alpha}, entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3, entry->y + 14, 85.0f, 28.0f, 8.0f);
            break;
    }
    if (entry->description->disp) {
        entry->description->disp(entry);
    }
}

/**
 * Per-frame update of every window in use (called from the main loop):
 * advances the open/close animation, runs the descriptor's main function
 * and queues winMgrDisp at priority 300 + the window's priority while it is visible.
 */
void winMgrMain(void) {
    WinMgrEntry* entry;
    int i;

    entry = wp->entries;
    for (i = 0; i < wp->numEntries; i++, entry++) {
        if (entry->flags & WINMGR_FLAG_ACTIVE) {
            winMgrSeq(entry);
            if (entry->description->main) {
                entry->description->main(entry);
            }
            if (entry->flags & WINMGR_FLAG_VISIBLE) {
                dispEntry((CameraId)entry->description->camId, entry->priority + 300, 0, winMgrDisp, entry);
            }
        }
    }
}

void winMgrSeq(WinMgrEntry* entry) {
    entry->flags |= WINMGR_FLAG_ANIMATING;
    switch (entry->description->type) {
        case 0:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~WINMGR_FLAG_ANIMATING;
                    break;
                case 1:
                    entry->flags |= WINMGR_FLAG_VISIBLE;
                    entry->rotation = intplGetValue(EASING_SINE_OUT, 30.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->scale = intplGetValue(EASING_SINE_OUT, 0.0f, 1.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = 255;
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                    }
                    break;
                case 2:
                    entry->rotation = intplGetValue(EASING_SINE_OUT, 0.0f, 30.0f, entry->timer, FPS_FRAMES(8));
                    entry->scale = intplGetValue(EASING_SINE_OUT, 1.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = 255;
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                        entry->flags &= ~WINMGR_FLAG_VISIBLE;
                        if (entry->flags & WINMGR_FLAG_AUTO_DELETE) {
                            entry->flags &= ~WINMGR_FLAG_ACTIVE;
                        }
                    }
                    break;
            }
            break;
        case 1:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~WINMGR_FLAG_ANIMATING;
                    break;
                case 1:
                    entry->flags |= WINMGR_FLAG_VISIBLE;
                    entry->rotation = 0.0f;
                    entry->scale = intplGetValue(EASING_SINE_OUT, 0.0f, 1.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = intplGetValue(EASING_SINE_OUT, 0.0f, 255.0f, entry->timer, FPS_FRAMES(8));
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                    }
                    break;
                case 2:
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = intplGetValue(EASING_SINE_OUT, 255.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                        entry->flags &= ~WINMGR_FLAG_VISIBLE;
                        if (entry->flags & WINMGR_FLAG_AUTO_DELETE) {
                            entry->flags &= ~WINMGR_FLAG_ACTIVE;
                        }
                    }
                    break;
            }
            break;
        case 2:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~WINMGR_FLAG_ANIMATING;
                    break;
                case 1:
                    entry->flags |= WINMGR_FLAG_VISIBLE;
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = 255;
                    entry->state = 0;
                    break;
                case 2:
                    entry->flags &= ~WINMGR_FLAG_VISIBLE;
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = 255;
                    entry->state = 0;
                    if (entry->flags & WINMGR_FLAG_AUTO_DELETE) {
                        entry->flags &= ~WINMGR_FLAG_ACTIVE;
                    }
                    break;
            }
            break;
    }
}

/**
 * Claims the first free window slot for a descriptor, copying its position and size; returns the window id.
 * The window stays hidden until winMgrOpen. There is no check for a full table.
 */
s32 winMgrEntry(WinMgrDesc* desc) {
    WinMgrEntry* entry;
    int i;

    entry = wp->entries;
    for (i = 0; i < wp->numEntries; i++, entry++) {
        if (!(entry->flags & WINMGR_FLAG_ACTIVE)) {
            break;
        }
    }
    entry->flags = 0;
    entry->flags |= WINMGR_FLAG_ACTIVE;
    entry->state = 0;
    entry->timer = 0;
    entry->description = desc;
    entry->x = entry->description->x;
    entry->y = entry->description->y;
    entry->width = entry->description->width;
    entry->height = entry->description->height;
    entry->priority = 0;
    entry->param = NULL;
    entry->scroll = 0;
    entry->lines = 0;
    return i;
}

/** Stores the caller's data pointer that the descriptor's main and disp functions read (entry->param). */
void winMgrSetParam(s32 id, void* param) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->param = param;
    }
}

/** Starts a window's opening animation and resets its help-text scroll and message. */
void winMgrOpen(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->state = 1;
        entry->timer = 0;
        entry->scroll = 0;
        entry->lines = 0;
        entry->msg = NULL;
    }
}

/** Starts a window's closing animation; the slot stays allocated (see winMgrCloseAutoDelete). */
void winMgrClose(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->state = 2;
        entry->timer = 0;
    }
}

/** Starts a window's closing animation and frees the slot once it has finished. */
void winMgrCloseAutoDelete(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->state = 2;
        entry->timer = 0;
        entry->flags |= WINMGR_FLAG_AUTO_DELETE;
    }
}

/** Frees a window slot at once, without an animation. */
void winMgrDelete(s32 id) {
    wp->entries[id].flags &= ~WINMGR_FLAG_ACTIVE;
}

BOOL winMgrAction(s32 id) {
    return (wp->entries[id].flags >> 2) & 1;
}

/** Moves and resizes a window (x, y is the top left corner, y up) if the slot is in use. */
void winMgrSetSize(s32 id, s32 x, s32 y, s32 width, s32 height) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->x = x;
        entry->y = y;
        entry->width = width;
        entry->height = height;
    }
}

/** Sets a window's draw priority offset (added to 300 in winMgrMain) if the slot is in use. */
void winMgrSetPriority(s32 id, s32 priority) {
    WinMgrEntry* entry;

    entry = &wp->entries[id];
    if (entry->flags & WINMGR_FLAG_ACTIVE) {
        entry->priority = priority;
    }
}

/** The window slot for an id (no range or in-use check). */
WinMgrEntry* winMgrGetPtr(s32 id) {
    return &wp->entries[id];
}

/** Resets a help window: scroll back to the top, no line count, no message. */
void winMgrHelpInit(WinMgrEntry* entry) {
    entry->scroll = 0;
    entry->lines = 0;
    entry->msg = NULL;
}

void winMgrHelpMain(WinMgrEntry* entry) {
    u16 lines = 0;

    if (!(entry->flags & WINMGR_FLAG_ANIMATING)) {
        if (entry->msg) {
            FontGetMessageWidthLine(entry->msg, &lines);
            entry->lines = lines + 1;
        }
        if (entry->lines > 2) {
            if (keyGetDirTrg(0) & DEMO_SUBSTICK_UP) {
                entry->scroll -= 2;
                if (entry->scroll < 0) {
                    entry->scroll = 0;
                }
            } else if (keyGetDirTrg(0) & DEMO_SUBSTICK_DOWN) {
#if VERSION == VERSION_EU
                if (entry->scroll / 2 < (entry->lines + 1) / 2 - 1) {
#else
                if ((entry->lines + 1) / 2 - 1 > (entry->scroll + 1) / 2) {
#endif
                    entry->scroll += 2;
                }
            }
        }
    }
}

/**
 * Draws a help window's message clipped to the two visible lines at the bottom of the screen,
 * filling in the lottery number for msg_lottery_number,
 * plus blinking up/down arrows and a scroll hint icon when it can scroll.
 */
void winMgrHelpDraw(WinMgrEntry* entry) {
    Mtx mtx;
    Mtx scale;
    char buf[0x400];

    if (!(entry->flags & WINMGR_FLAG_ANIMATING)) {
        GXSetScissor(0, 384, 608, 56);
        if (!strcmp(entry->msg, msgSearch("msg_lottery_number"))) {
            sprintf(buf, entry->msg, lotteryGetPtr()->number);
            FontDrawStart();
            FontDrawMessage(-210, -144.0f + 28.0f * entry->scroll, buf);
        } else {
            FontDrawStart();
            FontDrawMessage(-210, -144.0f + 28.0f * entry->scroll, entry->msg);
        }
        GXSetScissor(0, 0, 608, 480);
        if (entry->lines > 2) {
            // FAKE: spare copy of entry->scroll; testing the field directly changes the object
            s32 scroll = entry->scroll;

            if (scroll == 0) {
                if ((gp->retraceCount & 31) < 20) {
                    PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height - 4, 0.0f);
                    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                    PSMTXConcat(mtx, scale, mtx);
                    iconDispGx2(mtx, ICON_FLAG_2D, ICON_ARROW_DOWN);
                }
#if VERSION == VERSION_EU
            } else if (entry->scroll / 2 == (entry->lines + 1) / 2 - 1) {
#else
            } else if (entry->scroll + 2 == entry->lines) {
#endif
                if ((gp->retraceCount & 31) < 20) {
                    PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 44, 0.0f);
                    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                    PSMTXConcat(mtx, scale, mtx);
                    iconDispGx2(mtx, ICON_FLAG_2D, ICON_ARROW_UP);
                }
            } else if ((gp->retraceCount & 31) < 20) {
                PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 44, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGx2(mtx, ICON_FLAG_2D, ICON_ARROW_UP);
                PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height - 4, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGx2(mtx, ICON_FLAG_2D, ICON_ARROW_DOWN);
            }
            PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 10, 0.0f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(mtx, scale, mtx);
            iconDispGx2(mtx, ICON_FLAG_2D, ICON_SUBSTICK_NEUTRAL);
        }
    }
}

/**
 * Opens a selection list (list, title and help windows) and fills it by type:
 * 0 key items, 1/3/4 items (use, sell, store), 2 badges, 5 stored items, 6/7 partners to rank up,
 * 8 partner to rank down, 9/10 HP/FP/BP to raise or lower,
 * 11-16 shop stock (badge shop, sell badges, Star Piece badges, bargains, the Pit's trader, Pianta Parlor),
 * 17/18 the informant's lists; a negative type is a -1-terminated item id list.
 * Rows flagged 3 are greyed and refused.
 * newItem is shown first, above a divider (the item being picked up when the bag is full).
 * Free with winMgrSelectDelete.
 */
#if VERSION <= VERSION_DEMO_US
WinMgrSelect* winMgrSelectEntry(EvtWord type, s32 newItem, BOOL cancelOk) {
    s32 n;
    WinMgrDesc* desc;
    WinMgrSelect* sel;
    InformantWork* jdt;
    PouchData* pouch;
    u16 id;
    s32 i, j;
    s32 m;
    WinMgrDesc* d;
    s32 cnt, over;
    u8* tbl;
    s32* list;

    pouch = pouchGetPtr();
    jdt = &_jdt;
    sel = __memAlloc(HEAP_DEFAULT, sizeof(WinMgrSelect));
    memset(sel, 0, sizeof(WinMgrSelect));
    n = type;
    sel->type = n;
    sel->cursor = 0;
    sel->scrollTop = 0;
    sel->cursorY = 0.0f;
    sel->cursorX = 0.0f;
    sel->scrollY = 0.0f;
    sel->newItem = newItem;
    if (cancelOk) {
        sel->flags |= SELECT_CANCEL_OK;
    }
    if (n < 0) {
        n = 0;
    }
    d = select_desc_tbl[n].descriptors;
    sel->numDescriptors = select_desc_tbl[n].numDescriptors;
    for (i = 0; i < sel->numDescriptors; i++) {
        sel->descriptorId[i] = winMgrEntry(d++);
        winMgrSetParam(sel->descriptorId[i], sel);
    }
    desc = select_desc_tbl[n].descriptors;
    sel->cursorX = desc->x;
    sel->cursorY = desc->y - 54;

    switch (m = sel->type) {
        case 0:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x1E8);
            memset(sel->items, 0, 0x1E8);
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_KEY_ITEMS; i++) {
                id = (u16)pouchKeyItem(i);
                if (id) {
                    for (j = 0; j < 19; j++) {
                        if (id == menu_skip_list[j]) {
                            break;
                        }
                    }
                    if (j >= 19) {
                        sel->items[sel->numItems++].id = id;
                    }
                }
            }
            break;
        case 1:
        case 3:
        case 4:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x54);
            memset(sel->items, 0, 0x54);
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_HELD_ITEMS; i++) {
                id = (u16)pouchHaveItem(i);
                if (id) {
                    sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 2:
        case 12:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x324);
            memset(sel->items, 0, 0x324);
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_BADGES; i++) {
                id = (u16)pouchHaveBadge(i);
                if (id) {
sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 5:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x84);
            memset(sel->items, 0, 0x84);
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_STORED_ITEMS; i++) {
                id = (u16)pouchKeepItem(i);
                if (id) {
                    sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 6:
        case 7: {
            sel->items = __memAlloc(HEAP_DEFAULT, 0x1C);
            memset(sel->items, 0, 0x1C);
            sel->numItems = 0;
            for (i = 0; party_id_table[i] != 0; i++) {
                if (partyChkJoin(party_id_table[i])) {
                    sel->items[sel->numItems].id = i;
                    if (!evtGetValue(NULL, GSWF_GOR_UP_ARROW_USED)) {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 1) {
                            sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                        }
                    } else {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 2) {
                            sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                        }
                    }
                    sel->numItems++;
                }
            }
            break;
        }
        case 8: {
            sel->items = __memAlloc(HEAP_DEFAULT, 0x1C);
            memset(sel->items, 0, 0x1C);
            sel->numItems = 0;
            for (i = 0; party_id_table[i] != 0; i++) {
                if (partyChkJoin(party_id_table[i]) && party_id_table[i] != evtGetValue(NULL, GW(1))) {
                    sel->items[sel->numItems].id = i;
                    if (pouch->partyData[party_id_table[i]].techLevel == 0) {
                        sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                    sel->numItems++;
                }
            }
            break;
        }
        case 9:
            mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
            mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
            mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
            sel->items = __memAlloc(HEAP_DEFAULT, 0xC);
            memset(sel->items, 0, 0xC);
            for (i = 0; i < 3; i++) {
                sel->items[i].id = i;
                cnt = 0;
                over = 0;
                for (j = 0; j < 3; j++) {
                    if (j != sel->items[i].id) {
                        if (*mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                            cnt++;
                        }
                    }
}
if (cnt == 2) {
                    sel->items[i].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                }
}
            sel->numItems = 3;
            break;
        case 10:
            mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
            mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
            mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
            sel->items = __memAlloc(HEAP_DEFAULT, 8);
            memset(sel->items, 0, 8);
            sel->numItems = 0;
            for (i = 0; i < 3; i++) {
                if (i != evtGetValue(NULL, GW(1))) {
                    sel->items[sel->numItems].id = i;
                    if (*mario_status_point_table[i] - mario_status_henka_table[i] <= 0) {
                        sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                    sel->numItems++;
                }
            }
            break;
        case 11:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x18C);
            memset(sel->items, 0, 0x18C);
            sel->numItems = 0;
            for (i = 0; i < BADGE_COUNT; i++) {
                m = (s16)badgeShop_get(bdsw->bargain_table, i + BADGE_MIN);
                if (m - badgeShop_ThrowCheck(i + BADGE_MIN) > 0) {
                    sel->items[sel->numItems++].id = i + BADGE_MIN;
                }
            }
break;
        case 13:
            m = getBadgeStarmaniacTableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * 4);
            memset(sel->items, 0, m * 4);
            sel->numItems = 0;
            for (i = 0; i < BADGE_COUNT; i++) {
                m = (s16)badgeShop_get(bdsw->starmaniac_table, i + BADGE_MIN);
                if (m - badgeShop_ThrowCheck(i + BADGE_MIN) > 0) {
sel->items[sel->numItems++].id = i + BADGE_MIN;
                }
            }
break;
        case 14:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x10);
            memset(sel->items, 0, 0x10);
            sel->numItems = 0;
            for (i = 0; i < 4; i++) {
                m = badge_bottakuru_table[evtGetValue(NULL, GSW_BADGESHOP_CHARLIETON_STOCK + i)];
                if (m) {
                    sel->items[sel->numItems++].id = m;
                }
            }
            break;
        case 15:
            m = getBadgeBottakuru100TableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * 4);
            memset(sel->items, 0, m * 4);
            sel->numItems = 0;
            for (i = 0; i < m; i++) {
                sel->items[sel->numItems++].id = badge_bottakuru100_table[i];
            }
            break;
        case 16:
            m = getBadgeBteresaTableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * 4);
            memset(sel->items, 0, m * 4);
            sel->numItems = 0;
            for (i = 0; i < 338; i++) {
                if ((s16)badgeShop_get(bdsw->bteresa_table, i + 1) > 0) {
                    sel->items[sel->numItems++].id = i + 1;
                }
            }
break;
        case 17:
        case 18:
            if (m == 17) {
                tbl = jdt->readFlags.perfectRead;
            } else {
                tbl = jdt->readFlags.luigiRead;
            }
            sel->numItems = jdt->count;
            sel->items = __memAlloc(HEAP_DEFAULT, (sel->numItems + 1) * 4);
            memset(sel->items, 0, (sel->numItems + 1) * 4);
            for (i = 0; i < sel->numItems; i++) {
                sel->items[i].id = i;
                if (johoya_get(tbl, jdt->info[i].storyId)) {
                    sel->items[i].flags |= SELECT_ITEM_GRAY;
                }
            }
            break;
        default:
            if (type < 0) {
                s32 count;
                s32* it;
                list = (s32*)type;
                count = 0;
                while (list[count++] != -1) {
                }
                sel->items = __memAlloc(HEAP_DEFAULT, (count + 1) * 4);
                memset(sel->items, 0, (count + 1) * 4);
                sel->numItems = 0;
                if (newItem) {
                    sel->items[sel->numItems++].id = newItem;
                }
                for (it = list, i = 0; *it != -1; it++) {
                    sel->items[sel->numItems++].id = list[i++];
                }
            }
            break;
    }
    if (sel->numItems < 8) {
        winMgrSetSize(sel->descriptorId[0], desc->x, desc->y, desc->width, desc->height - (8 - sel->numItems) * 24);
    }
    return sel;
}
#else
WinMgrSelect* winMgrSelectEntry(EvtWord type, s32 newItem, BOOL cancelOk) {
    s32 n;
    WinMgrDesc* desc;
    WinMgrSelect* sel;
    InformantWork* jdt;
    PouchData* pouch;
    u16 id;
    s32 i, j;
    s32 m;
    WinMgrDesc* d;
    s32 cnt, over;
    u8* tbl;
    s32* list;

    pouch = pouchGetPtr();
    jdt = &_jdt;
    sel = __memAlloc(HEAP_DEFAULT, sizeof(WinMgrSelect));
    memset(sel, 0, sizeof(WinMgrSelect));
    n = type;
    sel->type = n;
    sel->cursor = 0;
    sel->scrollTop = 0;
    sel->cursorY = 0.0f;
    sel->cursorX = 0.0f;
    sel->scrollY = 0.0f;
    sel->newItem = newItem;
    if (cancelOk) {
        sel->flags |= SELECT_CANCEL_OK;
    }
    if (n < 0) {
        n = 0;
    }
    d = select_desc_tbl[n].descriptors;
    sel->numDescriptors = select_desc_tbl[n].numDescriptors;
    for (i = 0; i < sel->numDescriptors; i++) {
        sel->descriptorId[i] = winMgrEntry(d++);
        winMgrSetParam(sel->descriptorId[i], sel);
    }
    desc = select_desc_tbl[n].descriptors;
    sel->cursorX = desc->x;
    sel->cursorY = desc->y - 54;

    switch (m = sel->type) {
        case 0:
            sel->items = __memAlloc(HEAP_DEFAULT, (MAX_KEY_ITEMS + 1) * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, (MAX_KEY_ITEMS + 1) * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_KEY_ITEMS; i++) {
                id = (u16)pouchKeyItem(i);
                if (id) {
                    for (j = 0; j < 19; j++) {
                        if (id == menu_skip_list[j]) {
                            break;
                        }
                    }
                    if (j >= 19) {
                        sel->items[sel->numItems++].id = id;
                    }
                }
            }
            break;
        case 1:
        case 3:
        case 4:
            sel->items = __memAlloc(HEAP_DEFAULT, (MAX_HELD_ITEMS + 1) * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, (MAX_HELD_ITEMS + 1) * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_HELD_ITEMS; i++) {
                id = (u16)pouchHaveItem(i);
                if (id) {
                    sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 2:
        case 12:
            sel->items = __memAlloc(HEAP_DEFAULT, (MAX_BADGES + 1) * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, (MAX_BADGES + 1) * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_BADGES; i++) {
                id = (u16)pouchHaveBadge(i);
                if (id) {
#if VERSION >= VERSION_US
                    if (sel->type == 12 && id == pouchEquipBadge(i)) {
                        sel->items[sel->numItems].flags |= SELECT_ITEM_EQUIPPED;
                    }
#endif
                    sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 5:
            sel->items = __memAlloc(HEAP_DEFAULT, (MAX_STORED_ITEMS + 1) * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, (MAX_STORED_ITEMS + 1) * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            if (newItem) {
                sel->items[sel->numItems++].id = newItem;
            }
            for (i = 0; i < MAX_STORED_ITEMS; i++) {
                id = (u16)pouchKeepItem(i);
                if (id) {
                    sel->items[sel->numItems++].id = id;
                }
            }
            break;
        case 6:
        case 7: {
            sel->items = __memAlloc(HEAP_DEFAULT, 7 * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, 7 * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; party_id_table[i] != 0; i++) {
                if (partyChkJoin(party_id_table[i])) {
                    sel->items[sel->numItems].id = i;
                    if (!evtGetValue(NULL, GSWF_GOR_UP_ARROW_USED)) {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 1) {
                            sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                        }
                    } else {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 2) {
                            sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                        }
                    }
                    sel->numItems++;
                }
            }
            break;
        }
        case 8: {
            sel->items = __memAlloc(HEAP_DEFAULT, 7 * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, 7 * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; party_id_table[i] != 0; i++) {
                if (partyChkJoin(party_id_table[i]) && party_id_table[i] != evtGetValue(NULL, GW(1))) {
                    sel->items[sel->numItems].id = i;
                    if (pouch->partyData[party_id_table[i]].techLevel == 0) {
                        sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                    sel->numItems++;
                }
            }
            break;
        }
        case 9:
            mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
            mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
            mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
            sel->items = __memAlloc(HEAP_DEFAULT, 3 * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, 3 * sizeof(WinMgrSelectItem));
            for (i = 0; i < 3; i++) {
                sel->items[i].id = i;
                cnt = 0;
                over = 0;
                for (j = 0; j < 3; j++) {
                    if (j != sel->items[i].id) {
                        if (*mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                            cnt++;
                        }
                    } else if (*mario_status_point_table[j] + mario_status_henka_table[j] > mario_status_max_table[j]) {
                        over = 1;
                    }
                }
                if (cnt == 2 || over) {
                    if (over) {
                        sel->items[i].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED | SELECT_ITEM_OVER_MAX);
                    } else {
                        sel->items[i].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                }
            }
            sel->numItems = 3;
            break;
        case 10:
            mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
            mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
            mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
            sel->items = __memAlloc(HEAP_DEFAULT, 2 * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, 2 * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < 3; i++) {
                if (i != evtGetValue(NULL, GW(1))) {
                    sel->items[sel->numItems].id = i;
                    if (*mario_status_point_table[i] - mario_status_henka_table[i] <= 0) {
                        sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                    sel->numItems++;
                }
            }
            break;
        case 11:
            sel->items = __memAlloc(HEAP_DEFAULT, BADGE_COUNT * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, BADGE_COUNT * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < BADGE_COUNT; i++) {
                m = (s16)badgeShop_get(bdsw->bargain_table, i + BADGE_MIN);
                if (m - badgeShop_ThrowCheck(i + BADGE_MIN) > 0) {
                    sel->items[sel->numItems++].id = i + BADGE_MIN;
                }
            }
            qqsort(sel->items, sel->numItems, sizeof(WinMgrSelectItem), select_comp_type);
            break;
        case 13:
            m = getBadgeStarmaniacTableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, m * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < BADGE_COUNT; i++) {
                m = (s16)badgeShop_get(bdsw->starmaniac_table, i + BADGE_MIN);
                if (m - badgeShop_ThrowCheck(i + BADGE_MIN) > 0) {
                    sel->items[sel->numItems].id = i + BADGE_MIN;
                    if (itemDataTable[i + BADGE_MIN].star_piece_price > pouchGetStarPiece()) {
                        sel->items[sel->numItems].flags |= (SELECT_ITEM_GRAY | SELECT_ITEM_DISABLED);
                    }
                    sel->numItems++;
                }
            }
            qqsort(sel->items, sel->numItems, sizeof(WinMgrSelectItem), select_comp_starpiece);
            break;
        case 14:
            sel->items = __memAlloc(HEAP_DEFAULT, 4 * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, 4 * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < 4; i++) {
                m = badge_bottakuru_table[evtGetValue(NULL, GSW_BADGESHOP_CHARLIETON_STOCK + i)];
                if (m) {
                    sel->items[sel->numItems++].id = m;
                }
            }
            break;
        case 15:
            m = getBadgeBottakuru100TableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, m * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < m; i++) {
                sel->items[sel->numItems++].id = badge_bottakuru100_table[i];
            }
            break;
        case 16:
            m = getBadgeBteresaTableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, m * sizeof(WinMgrSelectItem));
            sel->numItems = 0;
            for (i = 0; i < BADGE_MAX - KEY_ITEM_MIN; i++) {
                if ((s16)badgeShop_get(bdsw->bteresa_table, i + 1) > 0) {
                    sel->items[sel->numItems++].id = i + 1;
                }
            }
#if VERSION >= VERSION_US
            qqsort(sel->items, sel->numItems, sizeof(WinMgrSelectItem), select_comp_bteresa);
#endif
            break;
        case 17:
        case 18:
            if (m == 17) {
                tbl = jdt->readFlags.perfectRead;
            } else {
                tbl = jdt->readFlags.luigiRead;
            }
            sel->numItems = jdt->count;
            sel->items = __memAlloc(HEAP_DEFAULT, (sel->numItems + 1) * sizeof(WinMgrSelectItem));
            memset(sel->items, 0, (sel->numItems + 1) * sizeof(WinMgrSelectItem));
            for (i = 0; i < sel->numItems; i++) {
                sel->items[i].id = i;
                if (johoya_get(tbl, jdt->info[i].storyId)) {
                    sel->items[i].flags |= SELECT_ITEM_GRAY;
                }
            }
            break;
        default:
            if (type < 0) {
                s32 count;
                s32* it;
                list = (s32*)type;
                count = 0;
                while (list[count++] != -1) {
                }
                sel->items = __memAlloc(HEAP_DEFAULT, (count + 1) * sizeof(WinMgrSelectItem));
                memset(sel->items, 0, (count + 1) * sizeof(WinMgrSelectItem));
                sel->numItems = 0;
                if (newItem) {
                    sel->items[sel->numItems++].id = newItem;
                }
                for (it = list, i = 0; *it != -1; it++) {
                    sel->items[sel->numItems++].id = list[i++];
                }
            }
            break;
    }
    if (sel->numItems < 8) {
        winMgrSetSize(sel->descriptorId[0], desc->x, desc->y, desc->width, desc->height - (8 - sel->numItems) * 24);
    }
    return sel;
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_80237a2c, JP 0x80237A2C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
#if VERSION == VERSION_EU
// PAL: equal prices fall back to the type order
int select_comp_starpiece(void* a, void* b) {
    if (itemDataTable[((WinMgrSelectItem*)a)->id].star_piece_price > itemDataTable[((WinMgrSelectItem*)b)->id].star_piece_price) {
        return 1;
    }
    if (itemDataTable[((WinMgrSelectItem*)a)->id].star_piece_price < itemDataTable[((WinMgrSelectItem*)b)->id].star_piece_price) {
        return -1;
    }
    if (itemDataTable[((WinMgrSelectItem*)a)->id].type_sort_order > itemDataTable[((WinMgrSelectItem*)b)->id].type_sort_order) {
        return 1;
    }
    return -(itemDataTable[((WinMgrSelectItem*)a)->id].type_sort_order < itemDataTable[((WinMgrSelectItem*)b)->id].type_sort_order);
#else
int select_comp_starpiece(void* a, void* b) {
    u16 x = itemDataTable[((WinMgrSelectItem*)a)->id].star_piece_price;
    u16 y = itemDataTable[((WinMgrSelectItem*)b)->id].star_piece_price;
    if (x > y) {
        return 1;
    }
    return -(x < y);
#endif
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_802379e4, JP 0x802379E4. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
int select_comp_type(void* a, void* b) {
    s16 x = itemDataTable[((WinMgrSelectItem*)a)->id].type_sort_order;
    s16 y = itemDataTable[((WinMgrSelectItem*)b)->id].type_sort_order;
    if (x > y) {
        return 1;
    }
    return -(x < y);
}
#endif

#if VERSION >= VERSION_US
/** US: badge list sorted by (buy price * 2 + 2) / 3 */
// US: badge list sorted by (buy price * 2 + 2) / 3
int select_comp_bteresa(void* a, void* b) {
    s32 x = (itemDataTable[((WinMgrSelectItem*)a)->id].buy_price * 2 + 2) / 3;
    s32 y = (itemDataTable[((WinMgrSelectItem*)b)->id].buy_price * 2 + 2) / 3;
    if (x > y) {
        return 1;
    }
    return -(x < y);
}
#endif

/**
 * The result of an item-style selection list: 0 while it is still open,
 * -1 if cancelled (or, from JP on, if the new item at the top was chosen), otherwise the chosen item id.
 */
s32 winMgrSelect(WinMgrSelect* sel) {
    if (sel->flags & SELECT_DONE) {
        if (sel->flags & SELECT_CANCELLED) {
#if VERSION > VERSION_DEMO_US
            return -1;
        }
        if (sel->newItem && sel->cursor == 0 && (sel->flags & SELECT_CANCEL_OK)) {
#endif
            return -1;
        }
        return sel->items[sel->cursor].id;
    }
    return 0;
}

/**
 * Result of the shop, partner, stat and informant lists for evt_win_other_select:
 * 0 while open, -1 if cancelled, 1 when chosen,
 * after writing the choice into the script's LW slots (id, name, price and BP cost by type;
 * the informant types also build the message name). For types 9/10 it also writes the stats after the trade.
 */
s32 winMgrSelectOther(WinMgrSelect* sel, EventEntry* evt) {
    InformantWork* jdt = &_jdt;
    s32 id;
    const char* name;
    char buf[0x40];

    if (sel->flags & SELECT_DONE) {
        if (sel->flags & SELECT_CANCELLED) {
            return -1;
        }
        id = sel->items[sel->cursor].id;
        switch (sel->type) {
            case 6:
            case 7:
                evt->lwData[1] = party_id_table[id];
#if VERSION <= VERSION_DEMO_US
                name = select_party_name(id);
#else
                switch (id) {
                    case 0:
                        name = msgSearch(name_party0);
                        break;
                    case 1:
                        name = msgSearch(name_party1);
                        break;
                    case 2:
                        name = msgSearch(name_party4);
                        break;
                    case 3:
                        name = pouchGetYoshiName();
                        break;
                    case 4:
                        name = msgSearch(name_party5);
                        break;
                    case 5:
                        name = msgSearch(name_party2);
                        break;
                    case 6:
                        name = msgSearch(name_party6);
                        break;
                }
#endif
                evt->lwData[3] = (EvtWord)name;
                break;
            case 8:
                evt->lwData[2] = party_id_table[id];
#if VERSION <= VERSION_DEMO_US
                name = select_party_name(id);
#else
                switch (id) {
                    case 0:
                        name = msgSearch(name_party0);
                        break;
                    case 1:
                        name = msgSearch(name_party1);
                        break;
                    case 2:
                        name = msgSearch(name_party4);
                        break;
                    case 3:
                        name = pouchGetYoshiName();
                        break;
                    case 4:
                        name = msgSearch(name_party5);
                        break;
                    case 5:
                        name = msgSearch(name_party2);
                        break;
                    case 6:
                        name = msgSearch(name_party6);
                        break;
                }
#endif
                evt->lwData[4] = (EvtWord)name;
                break;
            case 9:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)MARIO_STATUS_NAME(id);
                evt->lwData[5] = *mario_status_point_table[0];
                evt->lwData[6] = *mario_status_point_table[1];
                evt->lwData[7] = *mario_status_point_table[2];
                evt->lwData[5 + id] += mario_status_henka_table[id];
                break;
            case 10:
                evt->lwData[1] = id;
                evt->lwData[3] = (EvtWord)MARIO_STATUS_NAME(id);
                evt->lwData[5 + id] -= mario_status_henka_table[id];
                break;
            case 11:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
                evt->lwData[3] = itemDataTable[id].buy_price;
                evt->lwData[4] = itemDataTable[id].bp_cost;
                break;
            case 12:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
                evt->lwData[3] = itemDataTable[id].sell_price;
#if VERSION > VERSION_DEMO_US
                evt->lwData[4] = sel->cursor;
#endif
                break;
            case 13:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
                evt->lwData[3] = itemDataTable[id].star_piece_price;
                evt->lwData[4] = itemDataTable[id].bp_cost;
                break;
            case 14:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
                evt->lwData[3] = (itemDataTable[id].buy_price * 12) / 10;
                evt->lwData[4] = itemDataTable[id].bp_cost;
                break;
            case 15:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
#if VERSION <= VERSION_DEMO_US
                evt->lwData[3] = ((evtGetValue(NULL, GSW_PIT_FLOOR) + 11) * itemDataTable[id].buy_price) / 10;
#else
                evt->lwData[3] = ((evtGetValue(NULL, GSW_PIT_FLOOR) + 11) * itemDataTable[id].buy_price) / 10;
#endif
                evt->lwData[4] = itemDataTable[id].bp_cost;
                break;
            case 16:
                evt->lwData[1] = id;
                evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
                evt->lwData[3] = (itemDataTable[id].buy_price * 2 + 2) / 3;
                evt->lwData[4] = itemDataTable[id].bp_cost;
                break;
            case 17:
#if VERSION <= VERSION_DEMO_US
                sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
#else
                sprintf(jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
#endif
                sprintf(buf, "jperfect_title_%04d", jdt->info[id].storyId);
                jdt->curIndex = id;
                name = msgSearch(buf);
                evt->lwData[1] = (EvtWord)jdt->infoEnd;
                evt->lwData[2] = (EvtWord)name;
                evt->lwData[3] = strlen(name);
                if (johoya_get(jdt->readFlags.perfectRead, jdt->info[id].storyId)) {
                    evt->lwData[4] = 0;
                } else {
                    evt->lwData[4] = jdt->info[id].cost;
                }
                break;
            case 18:
#if VERSION <= VERSION_DEMO_US
                sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
#else
                sprintf(jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
#endif
                jdt->curIndex = id;
                evt->lwData[1] = (EvtWord)jdt->infoEnd;
                evt->lwData[2] = (EvtWord)jdt->info[id].initEndPos;
                evt->lwData[3] = jdt->info[id].tagEndPos;
                if (johoya_get(jdt->readFlags.luigiRead, jdt->info[id].storyId)) {
                    evt->lwData[4] = 0;
                } else {
                    evt->lwData[4] = 1;
                }
                break;
        }
        return 1;
    }
    return 0;
}

void winMgrSelectDelete(WinMgrSelect* sel) {
    // FAKE: chain of spare copies of sel, sel->items and the heap id; the plain
    // __memFree(HEAP_DEFAULT, sel->items); __memFree(HEAP_DEFAULT, sel); changes the object
    WinMgrSelectItem* items;
    int heap;
    WinMgrSelect* sel3;
    WinMgrSelectItem* items2;
    WinMgrSelectItem* items3;
    WinMgrSelect* sel2;
    items3 = (sel2 = (sel3 = sel))->items;
    items2 = items3;
    heap = HEAP_DEFAULT;
    items = items2;
    __memFree(HEAP_DEFAULT, items);
    __memFree(heap, sel3);
}

// Descriptive name (not in the symbol maps); placeholder unk_80237448, JP 0x80237448. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
s32 winMgrSelectGetCursor(WinMgrSelect* sel) {
    return sel->cursor;
}
#endif

void select_main(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    s32 prev;
    s32 i;

    switch (sel->state) {
        case 0:
            for (i = 0; i < sel->numDescriptors; i++) {
                winMgrOpen(sel->descriptorId[i]);
            }
            if (3 <= sel->numDescriptors) {
                winMgrHelpInit(winMgrGetPtr(sel->descriptorId[2]));
            }
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1 | PSND_SFX_BYPASS_MUTE));
            statusWinForceOpen();
            sel->state++;
            break;
        case 1:
            if (wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING) {
                break;
            }
            prev = sel->cursor;
            if (keyGetDirRep(0) & DEMO_STICK_DOWN) {
                sel->cursor++;
            }
            if (keyGetDirRep(0) & DEMO_STICK_UP) {
                sel->cursor--;
            }
            if (sel->cursor >= sel->numItems) {
                if (keyGetDirTrg(0) & DEMO_STICK_DOWN) {
                    sel->cursor = 0;
                    sel->scrollTop = 0;
                } else {
                    sel->cursor--;
                }
            }
            if (sel->cursor < 0) {
                if (keyGetDirTrg(0) & DEMO_STICK_UP) {
                    sel->cursor = sel->numItems - 1;
                    if ((i = sel->numItems - 8) > 0) {
                        sel->scrollTop = i;
                    }
                } else {
                    sel->cursor++;
                }
            }
            if (prev < sel->cursor) {
                if (sel->cursor > 5 && sel->scrollTop < sel->numItems - 8) {
                    sel->scrollTop++;
                }
            } else if (prev > sel->cursor) {
                if (sel->cursor - sel->scrollTop < 3 && sel->scrollTop > 0) {
                    sel->scrollTop--;
                }
            }
            if (prev != sel->cursor) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            }
#if VERSION == VERSION_EU
            // PAL: moving the cursor resets the help window's text
            if (prev != sel->cursor && sel->numDescriptors >= 3) {
                winMgrHelpInit(&wp->entries[sel->descriptorId[2]]);
            }
#endif
            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                if (sel->items[sel->cursor].flags & SELECT_ITEM_DISABLED) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                } else {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                    sel->state++;
                }
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                if (sel->flags & SELECT_CANCEL_OK) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                    sel->flags |= SELECT_CANCELLED;
                    sel->state++;
                }
            }
            sel->scrollY += (24.0f * sel->scrollTop - sel->scrollY) / 6.0f;
            sel->cursorX += (entry->x - sel->cursorX) / 6.0f;
            sel->cursorY += ((entry->y - 54) - 24.0f * (sel->cursor - sel->scrollTop) - sel->cursorY) / 6.0f;
            break;
        case 2:
            for (i = 0; i < sel->numDescriptors; i++) {
                winMgrClose(sel->descriptorId[i]);
            }
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1 | PSND_SFX_BYPASS_MUTE));
            statusWinForceOff();
            sel->state++;
            break;
        case 3:
            if (wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING) {
                break;
            }
            for (i = 0; i < sel->numDescriptors; i++) {
                winMgrDelete(sel->descriptorId[i]);
            }
            sel->flags |= SELECT_DONE;
            sel->state++;
            break;
        case 4:
            break;
    }
}

/**
 * Draws an item or badge list: icon, name and (for the sell and shop types) price per row,
 * the divider under a new item, the title by type,
 * a currency icon above the price column (one for 13, another for 16), the hand cursor and scroll arrows.
 * From US on, worn badges in the sell list get a mark.
 */
#if VERSION <= VERSION_DEMO_US
static void select_disp(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor tmp;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor iconcol;
    s32 k;
    s32 i;
    s32 price;
    ItemData* item;
    BOOL show;
    const char* msg;
    u16 w;
    f32 tw;
    f32 ofs;
    char buf[0x20];
    Mtx mtx;
    Mtx scale;
    tmp = white;
    iconcol = tmp;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(entry->x + 304, 274 - entry->y, entry->width, entry->height - 50);
        ofs = 0.0f;
        for (i = 0; i < sel->numItems; i++) {
            f32 py;
            item = &itemDataTable[sel->items[i].id];
            py = sel->scrollY + (ofs + (entry->y - 44 - i * 24));
            if ((f32)entry->y < py - 32.0f || (f32)(entry->y - entry->height) > py + 32.0f) {
                continue;
            }
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
winIconInit();
winIconSet(item->icon_id, (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, iconcol);
            winFontInit();
winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       black, msgSearch(item->name));
show = FALSE;
            switch (sel->type) {
                case 11:
                    price = item->buy_price;
                    show = TRUE;
                    break;
                case 3:
                case 12:
                    price = item->sell_price;
                    show = TRUE;
                    break;
                case 13:
                    price = item->star_piece_price;
                    show = TRUE;
                    break;
                case 14:
                    price = item->buy_price * 12 / 10;
                    show = TRUE;
                    break;
                case 15:
                    price = item->buy_price * (evtGetValue(NULL, GSW_PIT_FLOOR) + 11) / 10;
                    show = TRUE;
                    break;
                case 16:
                    price = (item->buy_price * 2 + 2) / 3;
                    show = TRUE;
                    break;
            }
            if (!show) { // price is set exactly where show is
                continue;
            }
            sprintf(buf, "%d", price);
tw = FontGetMessageWidth(buf);
            winFontInit();
            winFontSet((Vec){entry->x + entry->width - 10 - tw, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       black, buf);
}
        if (sel->newItem) {
            winIconInit();
            for (k = 2; k < 30; k++) {
                winIconSet(ICON_SP_ORB_1, (Vec){entry->x + k * entry->width / 31, (entry->y - 56) + sel->scrollY, 0.0f}, (Vec){0.15f, 0.15f, 0.15f}, black);
            }
        }
        GXSetScissor(left, top, wd, ht);
        switch (sel->type) {
case 11:
            case 12:
            case 13:
                msg = msgSearch("msg_window_title_2");
                break;
default:
                msg = msgSearch("msg_window_title_1");
                break;
        }
w = (entry->width - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
switch (sel->type) {
            case 3:
            case 11:
            case 12:
case 15:
            case 17:
PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_COIN, 0.6f);
                break;
            case 13:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_STAR_PIECE, 0.6f);
                break;
            case 16:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_PIANTA_TOKEN, 0.6f);
                break;
        }
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, (ICON_DISP_2D | ICON_FLAG_FLIP_X), ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#else
void select_disp(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor tmp;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor iconcol;
    s32 k;
    s32 i;
    s32 price;
    ItemData* item;
    BOOL show;
    const char* msg;
    u16 w;
    f32 tw;
    f32 ofs;
    char buf[0x20];
    Mtx mtx;
    Mtx scale;

    // FAKE: white copied through a spare tmp; iconcol = white changes the object
    tmp = white;
    iconcol = tmp;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(entry->x + 304, 274 - entry->y, entry->width, entry->height - 50);
        ofs = 0.0f;
        for (i = 0; i < sel->numItems; i++) {
            f32 py;
            item = &itemDataTable[sel->items[i].id];
            py = sel->scrollY + (ofs + (entry->y - 44 - i * 24));
            if ((f32)entry->y < py - 32.0f || (f32)(entry->y - entry->height) > py + 32.0f) {
                continue;
            }
            // FAKE: no-op update of ofs (always 0.0f); removing it changes the object
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
#if VERSION >= VERSION_US
            if (sel->items[i].flags & SELECT_ITEM_EQUIPPED) {
                if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(ICON_AC_LAMP_1, (Vec){entry->x + 15, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xC0});
            }
#endif
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(item->icon_id, (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, iconcol);
            winFontInit();
#if VERSION >= VERSION_US
            winFontSetWidth((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), 185.0f, msgSearch(item->name));
#else
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), msgSearch(item->name));
#endif
            show = FALSE;
            switch (sel->type) {
                case 11:
                    price = item->buy_price;
                    show = TRUE;
                    break;
                case 3:
                case 12:
                    price = item->sell_price;
                    show = TRUE;
                    break;
                case 13:
                    price = item->star_piece_price;
                    show = TRUE;
                    break;
                case 14:
                    price = item->buy_price * 12 / 10;
                    show = TRUE;
                    break;
                case 15:
                    price = item->buy_price * (evtGetValue(NULL, GSW_PIT_FLOOR) + 11) / 10;
                    show = TRUE;
                    break;
                case 16:
                    price = (item->buy_price * 2 + 2) / 3;
                    show = TRUE;
                    break;
            }
            if (!show) {
                continue;
            }
            sprintf(buf, "%d", price);
#if VERSION >= VERSION_US
            tw = FontGetMessageWidth(buf);
            if (tw > 30.0) {
                tw = 30.0f;
            }
            winFontInit();
            winFontSetWidth((Vec){entry->x + entry->width - 10 - tw, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), 30.0f, buf);
#else
            tw = FontGetMessageWidth(buf);
            winFontInit();
            winFontSet((Vec){entry->x + entry->width - 10 - tw, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), buf);
#endif
        }
        if (sel->newItem) {
            winIconInit();
            for (k = 2; k < 30; k++) {
                winIconSet(ICON_SP_ORB_1, (Vec){entry->x + k * entry->width / 31, (entry->y - 56) + sel->scrollY, 0.0f}, (Vec){0.15f, 0.15f, 0.15f}, black);
            }
        }
        GXSetScissor(left, top, wd, ht);
        switch (sel->type) {
            case 2:
            case 11:
            case 12:
            case 13:
                msg = msgSearch("msg_window_title_2");
                break;
            case 14:
                msg = msgSearch("msg_window_title_3");
                break;
            case 16:
                msg = msgSearch("msg_window_title_4");
                break;
            case 17:
                msg = msgSearch("msg_window_title_5");
                break;
            default:
                msg = msgSearch("msg_window_title_1");
                break;
        }
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 120.0f) {
            tw = 120.0f;
        }
        w = (entry->width - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){entry->x + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, 120.0f, msg);
#else
        w = (entry->width - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
#endif
        switch (sel->type) {
            case 3:
            case 11:
            case 12:
            case 14:
            case 15:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_COIN, 0.6f);
                break;
            case 13:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_STAR_PIECE, 0.6f);
                break;
            case 16:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_PIANTA_TOKEN, 0.6f);
                break;
        }
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#endif

static inline s32 select_party_icon(u16 id) {
    s32 icon = party_icon_table[id];
    if (icon == ICON_PARTY_YOSHI) {
        switch (pouchGetPartyColor(PARTNER_YOSHI)) {
            case 0:
                icon = ICON_PARTY_YOSHI;
                break;
            case 1:
                icon = ICON_PARTY_YOSHI_1;
                break;
            case 2:
                icon = ICON_PARTY_YOSHI_2;
                break;
            case 3:
                icon = ICON_PARTY_YOSHI_3;
                break;
            case 4:
                icon = ICON_PARTY_YOSHI_4;
                break;
            case 5:
                icon = ICON_PARTY_YOSHI_5;
                break;
            case 6:
            default:
                icon = ICON_PARTY_YOSHI_6;
                break;
        }
    }
    return icon;
}

/**
 * Draws the partner list of the rank-up and rank-swap menus:
 * name, portrait (Yoshi in his colour) and one rank icon per rank already earned, with the two column titles,
 * cursor and scroll arrows.
 */
#if VERSION <= VERSION_DEMO_US
static void select_disp_party(WinMgrEntry* entry) {
s32 i;
    WinMgrSelect* sel = entry->param;
    PouchData* pouch = pouchGetPtr();
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor iconcol;
    GXColor tmp;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 j;
    const char* msg;
    u16 w;
    f32 ofs;
tmp = white;
    iconcol = tmp;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(entry->x + 304, 274 - entry->y, entry->width, entry->height - 50);
        ofs = 0.0f;
        for (i = 0; i < sel->numItems; i++) {
            f32 py;
            py = sel->scrollY + (ofs + (entry->y - 44 - i * 24));
            if ((f32)entry->y < py - 32.0f || (f32)(entry->y - entry->height) > py + 32.0f) {
                continue;
            }
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
            winFontInit();
winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), select_party_name(sel->items[i].id));
if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(select_party_icon(party_icon_table[sel->items[i].id]), (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
winIconInit();
            for (j = 0; j < pouch->partyData[party_id_table[sel->items[i].id]].techLevel; j++) {
winIconSet(ICON_ITEM_SHINE_SPRITE, (Vec){entry->x + 253 + j * 27, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            }
        }
        GXSetScissor(left, top, wd, ht);
        msg = msgSearch("msg_window_title_7");
w = (120 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + (entry->width * 3 / 5 - 120) * 2 / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
msg = msgSearch("msg_window_title_8");
w = (85 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, (ICON_DISP_2D | ICON_FLAG_FLIP_X), ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#else
void select_disp_party(WinMgrEntry* entry) {
#if VERSION >= VERSION_US
    f32 tw;
#endif
    WinMgrSelect* sel = entry->param;
    PouchData* pouch = pouchGetPtr();
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor iconcol;
    GXColor tmp;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 i;
    s32 j;
    const char* msg;
    u16 w;
    f32 ofs;

    // FAKE: white copied through a spare tmp; iconcol = white changes the object
    tmp = white;
    iconcol = tmp;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(entry->x + 304, 274 - entry->y, entry->width, entry->height - 50);
        ofs = 0.0f;
        for (i = 0; i < sel->numItems; i++) {
            f32 py;
            py = sel->scrollY + (ofs + (entry->y - 44 - i * 24));
            if ((f32)entry->y < py - 32.0f || (f32)(entry->y - entry->height) > py + 32.0f) {
                continue;
            }
            // FAKE: no-op update of ofs (always 0.0f); removing it changes the object
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
            winFontInit();
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f},
                       (Vec){FontGetMessageWidth(select_party_name(sel->items[i].id)) > 170 ? 170.0f / FontGetMessageWidth(select_party_name(sel->items[i].id)) : 1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & SELECT_ITEM_GRAY) ? &gray : &black), select_party_name(sel->items[i].id));
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(select_party_icon(sel->items[i].id), (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            for (j = 0; j < pouch->partyData[party_id_table[sel->items[i].id]].techLevel; j++) {
                if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(ICON_ITEM_SHINE_SPRITE, (Vec){entry->x + 253 + j * 27, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            }
        }
        GXSetScissor(left, top, wd, ht);
        msg = msgSearch("msg_window_title_7");
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 100.0f) {
            tw = 100.0f;
        }
        w = (120 - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){entry->x + (entry->width * 3 / 5 - 120) * 2 / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, 100.0f, msg);
#else
        w = (120 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + (entry->width * 3 / 5 - 120) * 2 / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
#endif
        msg = msgSearch("msg_window_title_8");
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 65.0f) {
            tw = 65.0f;
        }
        w = (85 - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, 65.0f, msg);
#else
        w = (85 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
#endif
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#endif

/**
 * Draws the HP/FP/BP list of the power-down shop:
 * the stat's name, icon, current value and, unless refused,
 * the value after raising or lowering it by its step (5, 5, 3).
 */
void select_disp_mario(WinMgrEntry* entry) {
#if VERSION >= VERSION_US
    f32 tw;
#endif
    WinMgrSelect* sel = entry->param;
    PouchData* pouch = pouchGetPtr();
    u32 left, top, wd, ht;
    char buf[4];
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor tmp;
    GXColor iconcol;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor white2 = white;
    GXColor* col;
    s32 i;
    s32 id;
    const char* msg;
    u16 w;
    f32 ofs;

    // FAKE: white copied through a spare tmp; iconcol = white changes the object
    tmp = white;
    iconcol = tmp;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(entry->x + 304, 274 - entry->y, entry->width, entry->height - 50);
        ofs = 0.0f;
        for (i = 0; i < sel->numItems; i++) {
            f32 py;
            py = sel->scrollY + (ofs + (entry->y - 44 - i * 24));
            id = sel->items[i].id;
            if ((f32)entry->y < py - 32.0f || (f32)(entry->y - entry->height) > py + 32.0f) {
                continue;
            }
            // FAKE: no-op update of ofs (always 0.0f); removing it changes the object
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
            winFontInit();
            col = &black;
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, MARIO_STATUS_NAME(id));
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(mario_status_icon_table[id], (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            sprintf(buf, "%3d", *mario_status_point_table[id]);
            winFontInit();
            col = &black;
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 160, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            if (sel->items[i].flags & SELECT_ITEM_DISABLED) {
                continue;
            }
            switch (sel->type) {
                case 9:
                    sprintf(buf, "%3d", *mario_status_point_table[id] + mario_status_henka_table[id]);
                    break;
                case 10:
                default:
                    sprintf(buf, "%3d", *mario_status_point_table[id] - mario_status_henka_table[id]);
                    break;
            }
            winFontInit();
            col = &black;
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 240, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            winIconInit();
            winIconSet(ICON_ARROW, (Vec){entry->x + 220, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){-0.55f, 0.55f, 0.55f}, iconcol);
        }
        GXSetScissor(left, top, wd, ht);
        msg = msgSearch("msg_window_title_9");
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 100.0f) {
            tw = 100.0f;
        }
        w = (120 - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){entry->x + (entry->width * 3 / 5 - 120) * 2 / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2, 100.0f, msg);
#else
        w = (120 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + (entry->width * 3 / 5 - 120) * 2 / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2, msg);
#endif
        msg = msgSearch("msg_window_title_10");
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 65.0f) {
            tw = 65.0f;
        }
        w = (85 - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2, 65.0f, msg);
#else
        w = (85 - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){entry->x + entry->width * 3 / 5 + (entry->width * 2 / 5 - 85) / 3 + w, entry->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2, msg);
#endif
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}

/**
 * Draws the informant lists: each entry's title cut at its tag end, read ones in grey,
 * and for type 17 the price of unread entries with a coin icon. Despite the name it serves both 17 and 18.
 */
#if VERSION <= VERSION_DEMO_US
static void select_disp_luigi(WinMgrEntry* entry) {
WinMgrEntry* ent = entry;
    WinMgrSelect* sel = ent->param;
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    InformantWork* jdt = &_jdt;
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor* col;
    s32 i;
    InformantInfo* info;
    const char* msg;
    u16 w;
    char save;
    f32 py;
    f32 zero = 0.0f;
    char buf[0x20];
    Mtx mtx;
    Mtx scale;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(ent->x + 304, 274 - ent->y, ent->width, ent->height - 50);
        info = jdt->info;
        for (i = 0; i < sel->numItems; i++, info++) {
            py = sel->scrollY + (zero + (ent->y - 44 - i * 24));
            if ((f32)ent->y < py - 32.0f || (f32)(ent->y - ent->height) > py + 32.0f) {
                continue;
            }
            winFontInit();
            save = info->initEndPos[info->tagEndPos];
            info->initEndPos[info->tagEndPos] = 0;
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                col = &gray;
            } else {
                col = &black;
            }
winFontSet((Vec){ent->x + 25, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, *col, info->initEndPos);
info->initEndPos[info->tagEndPos] = save;
            if (sel->type == 17 && !(sel->items[i].flags & SELECT_ITEM_GRAY)) {
                sprintf(buf, "%3d", info->cost);
                winFontInit();
                col = &black;
                if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                    col = &gray;
                }
                winFontSet((Vec){ent->x + 245, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            }
        }
        GXSetScissor(left, top, wd, ht);
msg = msgSearch("msg_window_title_6");
w = (ent->width - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){ent->x + w, ent->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, (ICON_DISP_2D | ICON_FLAG_FLIP_X), ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - ent->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#else
void select_disp_luigi(WinMgrEntry* entry) {
#if VERSION >= VERSION_US
    f32 tw;
#endif
    WinMgrEntry* ent = entry;
    WinMgrSelect* sel = ent->param;
    u32 left, top, wd, ht;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    InformantWork* jdt = &_jdt;
    GXColor gray = {0xA0, 0xA0, 0xA0, 0xFF};
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor* col;
    s32 i;
    InformantInfo* info;
    const char* msg;
    u16 w;
    char save;
    f32 py;
    f32 zero = 0.0f;
    char buf[0x20];
    Mtx mtx;
    Mtx scale;

    if (!(wp->entries[sel->descriptorId[0]].flags & WINMGR_FLAG_ANIMATING)) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        GXGetScissor(&left, &top, &wd, &ht);
        GXSetScissor(ent->x + 304, 274 - ent->y, ent->width, ent->height - 50);
        info = jdt->info;
        for (i = 0; i < sel->numItems; i++, info++) {
            py = sel->scrollY + (zero + (ent->y - 44 - i * 24));
            if ((f32)ent->y < py - 32.0f || (f32)(ent->y - ent->height) > py + 32.0f) {
                continue;
            }
            winFontInit();
            save = info->initEndPos[info->tagEndPos];
            info->initEndPos[info->tagEndPos] = 0;
            if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                col = &gray;
            } else {
                col = &black;
            }
#if VERSION >= VERSION_US
            winFontSetWidth((Vec){ent->x + 25, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, *col, 200.0f, info->initEndPos);
#else
            winFontSet((Vec){ent->x + 25, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, *col, info->initEndPos);
#endif
            info->initEndPos[info->tagEndPos] = save;
            if (sel->type == 17 && !(sel->items[i].flags & SELECT_ITEM_GRAY)) {
                sprintf(buf, "%3d", info->cost);
                winFontInit();
                col = &black;
                if (sel->items[i].flags & SELECT_ITEM_GRAY) {
                    col = &gray;
                }
                winFontSet((Vec){ent->x + 245, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            }
        }
        GXSetScissor(left, top, wd, ht);
        switch (sel->type) {
            case 17:
                msg = msgSearch("msg_window_title_5");
                break;
            case 18:
            default:
                msg = msgSearch("msg_window_title_6");
                break;
        }
#if VERSION >= VERSION_US
        tw = FontGetMessageWidth(msg);
        if (tw > 120.0f) {
            tw = 120.0f;
        }
        w = (ent->width - tw) / 2.0f;
        winFontInit();
        winFontSetEdgeWidth((Vec){ent->x + w, ent->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, 120.0f, msg);
#else
        w = (ent->width - FontGetMessageWidth(msg)) / 2;
        winFontInit();
        winFontSetEdge((Vec){ent->x + w, ent->y + 14, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, msg);
#endif
        switch (sel->type) {
            case 17:
                PSMTXTrans(mtx, ent->x + 268, ent->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){ent->x + 268, ent->y - 12, 0.0f}, ICON_FLAG_2D, ICON_ITEM_COIN, 0.6f);
                break;
        }
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - 36, 1.0f}, ICON_FLAG_2D, ICON_ARROW_UP, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - ent->height - 12, 1.0f}, ICON_FLAG_2D, ICON_ARROW_DOWN, 0.6f);
            }
        }
    }
}
#endif

void select_disp2(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    const char* msg;
    u16 w;
    u16 lines;

    msg = msgSearch("msg_window_select_1");
    switch (sel->type) {
        case 3:
        case 12:
            msg = msgSearch("msg_window_select_2");
            break;
        case 4:
            msg = msgSearch("msg_window_select_3");
            break;
        case 5:
            msg = msgSearch("msg_window_select_4");
            break;
        case 6:
        case 7:
            msg = msgSearch("msg_window_select_11");
            break;
        case 8:
            msg = msgSearch("msg_window_select_12");
            break;
        case 9:
            msg = msgSearch("msg_window_select_9");
            break;
        case 10:
            msg = msgSearch("msg_window_select_9_1");
            break;
        case 11:
        case 14:
        case 15:
            msg = msgSearch("msg_window_select_6");
            break;
        case 13:
        case 16:
            msg = msgSearch("msg_window_select_10");
            break;
        case 17:
            msg = msgSearch("msg_window_select_8");
            break;
        case 18:
            msg = msgSearch("msg_window_select_7");
            break;
    }
    if (!(wp->entries[sel->descriptorId[1]].flags & WINMGR_FLAG_ANIMATING)) {
#if VERSION >= VERSION_US
        Mtx trans;
        Mtx scale;
        f32 tw;

        tw = FontGetMessageWidthLine(msg, &lines);
        if (tw > entry->width - 30) {
            PSMTXScale(scale, (entry->width - 30) / tw, 1.0f, 1.0f);
            tw = entry->width - 30;
        } else {
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        }
        w = (entry->width - tw) / 2.0f;
        PSMTXTrans(trans, entry->x + w, entry->y - (entry->height - (lines + 1) * 24) / 2, 0.0f);
        PSMTXConcat(trans, scale, trans);
        FontDrawStart();
        FontDrawMessageMtx(trans, msg);
        winMgrSetSize(sel->descriptorId[1], entry->x, entry->description->y + lines * 22 / 2, entry->width,
                      entry->description->height + lines * 22);
#else
        w = (entry->width - FontGetMessageWidthLine(msg, &lines)) / 2;
        FontDrawStart();
        FontDrawMessage(entry->x + w, entry->y - (entry->height - (lines + 1) * 24) / 2, msg);
#endif
    }
}

/** Main function of a selection list's help window: scrolls its text (winMgrHelpMain). */
void select_main3(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}

/** Help window of an item list: shows the description of the item under the cursor. */
void select_disp3(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;

    if (!(wp->entries[sel->descriptorId[2]].flags & WINMGR_FLAG_ANIMATING)) {
        ItemData* item = &itemDataTable[sel->items[sel->cursor].id];
        entry->msg = msgSearch(item->description);
        winMgrHelpDraw(entry);
    }
}

void select_disp3_party(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    PouchData* pouch;
    char buf[0x48];
    const char* help;
    const char* lank;
    s32 id;

    pouch = pouchGetPtr();
    if (!(wp->entries[sel->descriptorId[2]].flags & WINMGR_FLAG_ANIMATING)) {
        if (sel->items[sel->cursor].flags & SELECT_ITEM_DISABLED) {
            help = "helperr";
        } else {
            help = "help";
        }
        if (sel->type == 6 || sel->type == 7) {
            lank = "lankup";
        } else {
            lank = "lankdown";
        }
        id = sel->items[sel->cursor].id;
        sprintf(buf, "%s_%s_lank%d%s", help, lank, pouch->partyData[party_id_table[id]].techLevel + 1,
                party_labelname_table[id]);
        entry->msg = msgSearch(buf);
        winMgrHelpDraw(entry);
    }
}

#if VERSION <= VERSION_DEMO_US
static void select_disp3_mario(WinMgrEntry* entry) {
#else
void select_disp3_mario(WinMgrEntry* entry) {
#endif
    WinMgrSelect* sel = entry->param;
    char buf[0x48];
    const char* help;

    pouchGetPtr();
    if (!(wp->entries[sel->descriptorId[2]].flags & WINMGR_FLAG_ANIMATING)) {
        if (sel->items[sel->cursor].flags & SELECT_ITEM_DISABLED) {
#if VERSION > VERSION_DEMO_US
            if (sel->items[sel->cursor].flags & SELECT_ITEM_OVER_MAX) {
                help = "helperr2";
            } else {
#endif
                help = "helperr";
#if VERSION > VERSION_DEMO_US
            }
#endif
        } else {
            help = "help";
        }
        switch (sel->type) {
            case 9:
                sprintf(buf, "%s_levelup%s", help, mario_status_labelname_table[sel->items[sel->cursor].id]);
                break;
            default:
            case 10:
                sprintf(buf, "%s_leveldown%s", help, mario_status_labelname_table[sel->items[sel->cursor].id]);
                break;
        }
        entry->msg = msgSearch(buf);
        winMgrHelpDraw(entry);
    }
}

/**
 * Message labels of the partners' names
 * (Goombella, Koops, Flurrie, Vivian, Bobbery, Ms. Mowz), looked up for the window text.
 */
static const char name_party0[] = "name_party0";
static const char name_party1[] = "name_party1";
static const char name_party4[] = "name_party4";
static const char name_party5[] = "name_party5";
static const char name_party2[] = "name_party2";
static const char name_party6[] = "name_party6";
