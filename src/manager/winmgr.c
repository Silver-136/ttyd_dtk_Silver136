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

extern ItemData itemDataTable[];
void winFontSet(Vec pos, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetEdge(Vec pos, Vec scale, GXColor color, const char* fmt, ...);
void qqsort(void* base, u32 n, u32 size, int (*compare)(void*, void*));
#ifdef VERSION_G8ME01
u16 L_pouchEquipBadge(s32 index);
int select_comp_bteresa(void* a, void* b);
#endif
void winIconSet(u32 id, Vec translate, Vec scale, GXColor color);




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
void winMgrDisp(CameraId camId, void* param);
void winMgrSeq(WinMgrEntry* entry);
int select_comp_type(void* a, void* b);
int select_comp_starpiece(void* a, void* b);

//.data

static WinMgrWork work;
static s16* mario_status_point_table[3];

//.sdata
static WinMgrWork* wp = &work;
static u16 party_id_table[] = {1, 2, 5, 4, 6, 3, 7, 0};
static u16 party_icon_table[] = {ICON_155, ICON_156, ICON_15F, ICON_158, ICON_160, ICON_157, ICON_161};
static const char* party_labelname_table[] = {"_kur", "_nok", "_win", "_yos", "_viv", "_bom", "_chu"};
static s32 mario_status_icon_table[] = {ICON_19F, ICON_1A0, ICON_1A1};
static const char* mario_status_labelname_table[] = {"_hp", "_fp", "_bp"};
#ifdef VERSION_G8ME01
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
static s32 mario_status_henka_table[] = {5, 5, 3};
s32 mario_status_max_table[] = {200, 200, 99};

WinMgrDesc select_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3},
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3},
};

WinMgrDesc select_party_desc[] = {
    {0, 4, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_party},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3_party},
};

WinMgrDesc select_mario_desc[] = {
    {0, 5, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_mario},
    {2, 0, 8, -280, 120, 240, 69, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
    {0, 0, 8, -240, -130, 500, 80, {0xFF, 0xFF, 0xFF, 0xFF}, select_main3, select_disp3_mario},
};

WinMgrDesc select_luigi_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_luigi},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
};

WinMgrDesc select_johoya_perfect_desc[] = {
    {0, 1, 8, -24, 116, 300, 240, {0xFF, 0xFF, 0xFF, 0xFF}, select_main, select_disp_luigi},
    {2, 0, 8, -280, 120, 240, 45, {0xFF, 0xFF, 0xFF, 0xFF}, NULL, select_disp2},
};

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
    {3, &select_desc[3]},
    {3, select_desc},
    {3, select_desc},
    {3, &select_desc[3]},
    {2, select_johoya_perfect_desc},
    {2, select_luigi_desc},
};

//.sbss


static const char name_party0[12];
static const char name_party1[12];
static const char name_party4[12];
static const char name_party5[12];
static const char name_party2[12];
static const char name_party6[12];

static inline const char* select_party_name(u16 id) /* */ {
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

USER_FUNC(evt_win_party_lankup_chk) {
    PouchData* pouch;
    u16* p;
    EvtWord* args = event->args;
    s32 max;

    pouch = pouchGetPtr();
    for (p = party_id_table; *p != 0; p++) {
        if (partyChkJoin(*p)) {
            if (!evtGetValue(event, GSWF(1186))) {
                if (pouch->partyData[*p].techLevel < 1) { max = 0; } else { max = 1; }
            } else {
                if (pouch->partyData[*p].techLevel < 2) { max = 0; } else { max = 2; }
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
            if (!evtGetValue(event, GSWF(1186))) {
                if (pouch->partyData[*p].techLevel < 1) { max = 0; } else { max = 1; }
            } else {
                if (pouch->partyData[*p].techLevel < 2) { max = 0; } else { max = 2; }
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


void winMgrInit(void) {
    wp->mNumEntries = 32;
    wp->mEntries = (WinMgrEntry*)__memAlloc(HEAP_DEFAULT, wp->mNumEntries * sizeof(WinMgrEntry));
    memset(wp->mEntries, 0, wp->mNumEntries * sizeof(WinMgrEntry));
}

void winMgrReInit(void) {
    memset(wp->mEntries, 0, wp->mNumEntries * sizeof(WinMgrEntry));
}

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

void winMgrMain(void) {
    WinMgrEntry* entry;
    int i;

    entry = wp->mEntries;
    for (i = 0; i < wp->mNumEntries; i++, entry++) {
        if (entry->flags & 1) {
            winMgrSeq(entry);
            if (entry->description->main) {
                entry->description->main(entry);
            }
            if (entry->flags & 2) {
                dispEntry((CameraId)entry->description->camId, entry->priority + 300, 0, winMgrDisp, entry);
            }
        }
    }
}

void winMgrSeq(WinMgrEntry* entry) {
    entry->flags |= 4;
    switch (entry->description->type) {
        case 0:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~4;
                    break;
                case 1:
                    entry->flags |= 2;
                    entry->rotation = intplGetValue(12, 30.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->scale = intplGetValue(12, 0.0f, 1.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = 255;
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                    }
                    break;
                case 2:
                    entry->rotation = intplGetValue(12, 0.0f, 30.0f, entry->timer, FPS_FRAMES(8));
                    entry->scale = intplGetValue(12, 1.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = 255;
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                        entry->flags &= ~2;
                        if (entry->flags & 8) {
                            entry->flags &= ~1;
                        }
                    }
                    break;
            }
            break;
        case 1:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~4;
                    break;
                case 1:
                    entry->flags |= 2;
                    entry->rotation = 0.0f;
                    entry->scale = intplGetValue(12, 0.0f, 1.0f, entry->timer, FPS_FRAMES(8));
                    entry->alpha = intplGetValue(12, 0.0f, 255.0f, entry->timer, FPS_FRAMES(8));
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                    }
                    break;
                case 2:
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = intplGetValue(12, 255.0f, 0.0f, entry->timer, FPS_FRAMES(8));
                    entry->timer++;
                    if (entry->timer > FPS_FRAMES(8)) {
                        entry->state = 0;
                        entry->flags &= ~2;
                        if (entry->flags & 8) {
                            entry->flags &= ~1;
                        }
                    }
                    break;
            }
            break;
        case 2:
            switch (entry->state) {
                case 0:
                    entry->flags &= ~4;
                    break;
                case 1:
                    entry->flags |= 2;
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = 255;
                    entry->state = 0;
                    break;
                case 2:
                    entry->flags &= ~2;
                    entry->rotation = 0.0f;
                    entry->scale = 1.0f;
                    entry->alpha = 255;
                    entry->state = 0;
                    if (entry->flags & 8) {
                        entry->flags &= ~1;
                    }
                    break;
            }
            break;
    }
}

s32 winMgrEntry(WinMgrDesc* desc) {
    WinMgrEntry* entry;
    int i;

    entry = wp->mEntries;
    for (i = 0; i < wp->mNumEntries; i++, entry++) {
        if (!(entry->flags & 1)) {
            break;
        }
    }
    entry->flags = 0;
    entry->flags |= 1;
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

void winMgrSetParam(s32 id, void* param) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->param = param;
    }
}

void winMgrOpen(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->state = 1;
        entry->timer = 0;
        entry->scroll = 0;
        entry->lines = 0;
        entry->msg = NULL;
    }
}

void winMgrClose(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->state = 2;
        entry->timer = 0;
    }
}

void winMgrCloseAutoDelete(s32 id) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->state = 2;
        entry->timer = 0;
        entry->flags |= 8;
    }
}

void winMgrDelete(s32 id) {
    wp->mEntries[id].flags &= ~1;
}

BOOL winMgrAction(s32 id) {
    return (wp->mEntries[id].flags >> 2) & 1;
}

void winMgrSetSize(s32 id, s32 x, s32 y, s32 width, s32 height) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->x = x;
        entry->y = y;
        entry->width = width;
        entry->height = height;
    }
}

void winMgrSetPriority(s32 id, s32 priority) {
    WinMgrEntry* entry;

    entry = &wp->mEntries[id];
    if (entry->flags & 1) {
        entry->priority = priority;
    }
}

WinMgrEntry* winMgrGetPtr(s32 id) {
    return &wp->mEntries[id];
}

void winMgrHelpInit(WinMgrEntry* entry) {
    entry->scroll = 0;
    entry->lines = 0;
    entry->msg = NULL;
}

void winMgrHelpMain(WinMgrEntry* entry) {
    u16 lines = 0;

    if (!(entry->flags & 4)) {
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
                if ((entry->lines + 1) / 2 - 1 > (entry->scroll + 1) / 2) {
                    entry->scroll += 2;
                }
            }
        }
    }
}


void winMgrHelpDraw(WinMgrEntry* entry) {
    Mtx mtx;
    Mtx scale;
    char buf[0x400];

    if (!(entry->flags & 4)) {
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
        if (entry->lines > 2) { s32 scroll = entry->scroll;
            if (scroll == 0) {
                if ((gp->retraceCount & 31) < 20) {
                    PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height - 4, 0.0f);
                    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                    PSMTXConcat(mtx, scale, mtx);
                    iconDispGx2(mtx, 16, ICON_1B6);
                }
            } else if (entry->scroll + 2 == entry->lines) {
                if ((gp->retraceCount & 31) < 20) {
                    PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 44, 0.0f);
                    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                    PSMTXConcat(mtx, scale, mtx);
                    iconDispGx2(mtx, 16, ICON_1B5);
                }
            } else if ((gp->retraceCount & 31) < 20) {
                PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 44, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGx2(mtx, 16, ICON_1B5);
                PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height - 4, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGx2(mtx, 16, ICON_1B6);
            }
            PSMTXTrans(mtx, entry->x + entry->width - 12, entry->y - entry->height + 10, 0.0f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(mtx, scale, mtx);
            iconDispGx2(mtx, 16, ICON_070);
        }
    }
}

WinMgrSelect* winMgrSelectEntry(EvtWord type, s32 newItem, BOOL a3) {
    s32 n;
    WinMgrDesc* desc;
    WinMgrSelect* sel;
    InformantWork* jdt;
    u16* p;
    PouchData* pouch;
    WinMgrSelectTbl* table;
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
    if (a3) {
        sel->flags |= 0x100;
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
            for (i = 0; i < 121; i++) {
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
            for (i = 0; i < 20; i++) {
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
            for (i = 0; i < 200; i++) {
                id = (u16)pouchHaveBadge(i);
                if (id) {
#ifdef VERSION_G8ME01
                    if (sel->type == 12 && id == L_pouchEquipBadge(i)) {
                        sel->items[sel->numItems].flags |= 8;
                    }
#endif
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
            for (i = 0; i < 32; i++) {
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
                    if (!evtGetValue(NULL, GSWF(1186))) {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 1) {
                            sel->items[sel->numItems].flags |= 3;
                        }
                    } else {
                        if (pouch->partyData[party_id_table[i]].techLevel >= 2) {
                            sel->items[sel->numItems].flags |= 3;
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
                        sel->items[sel->numItems].flags |= 3;
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
                    } else if (*mario_status_point_table[j] + mario_status_henka_table[j] > mario_status_max_table[j]) {
                        over = 1;
                    }
                }
                if (cnt == 2 || over) {
                    if (over) {
                        sel->items[i].flags |= 7;
                    } else {
                        sel->items[i].flags |= 3;
                    }
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
                        sel->items[sel->numItems].flags |= 3;
                    }
                    sel->numItems++;
                }
            }
            break;
        case 11:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x18C);
            memset(sel->items, 0, 0x18C);
            sel->numItems = 0;
            for (i = 0; i < 99; i++) {
                m = (s16)badgeShop_get(bdsw->bargain_table, i + 240);
                if (m - badgeShop_ThrowCheck(i + 240) > 0) {
                    sel->items[sel->numItems++].id = i + 240;
                }
            }
            qqsort(sel->items, sel->numItems, 4, select_comp_type);
            break;
        case 13:
            m = getBadgeStarmaniacTableMaxCount();
            sel->items = __memAlloc(HEAP_DEFAULT, m * 4);
            memset(sel->items, 0, m * 4);
            sel->numItems = 0;
            for (i = 0; i < 99; i++) {
                m = (s16)badgeShop_get(bdsw->starmaniac_table, i + 240);
                if (m - badgeShop_ThrowCheck(i + 240) > 0) {
                    sel->items[sel->numItems].id = i + 240;
                    if (itemDataTable[i + 240].star_piece_price > pouchGetStarPiece()) {
                        sel->items[sel->numItems].flags |= 3;
                    }
                    sel->numItems++;
                }
            }
            qqsort(sel->items, sel->numItems, 4, select_comp_starpiece);
            break;
        case 14:
            sel->items = __memAlloc(HEAP_DEFAULT, 0x10);
            memset(sel->items, 0, 0x10);
            sel->numItems = 0;
            for (i = 0; i < 4; i++) {
                m = badge_bottakuru_table[evtGetValue(NULL, GSW(118) + i)];
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
#ifdef VERSION_G8ME01
            qqsort(sel->items, sel->numItems, 4, select_comp_bteresa);
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
            sel->items = __memAlloc(HEAP_DEFAULT, (sel->numItems + 1) * 4);
            memset(sel->items, 0, (sel->numItems + 1) * 4);
            for (i = 0; i < sel->numItems; i++) {
                sel->items[i].id = i;
                if (johoya_get(tbl, jdt->info[i].storyId)) {
                    sel->items[i].flags |= 1;
                }
            }
            break;
        default:
            if (type < 0) {
                s32 m;
                s32* it;
                list = (s32*)type;
                m = 0;
                while (list[m++] != -1) {
                }
                sel->items = __memAlloc(HEAP_DEFAULT, (m + 1) * 4);
                memset(sel->items, 0, (m + 1) * 4);
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

int select_comp_starpiece(void* a, void* b) {
    u16 x = itemDataTable[((WinMgrSelectItem*)a)->id].star_piece_price;
    u16 y = itemDataTable[((WinMgrSelectItem*)b)->id].star_piece_price;
    if (x > y) {
        return 1;
    }
    return -(x < y);
}

int select_comp_type(void* a, void* b) {
    s16 x = itemDataTable[((WinMgrSelectItem*)a)->id].type_sort_order;
    s16 y = itemDataTable[((WinMgrSelectItem*)b)->id].type_sort_order;
    if (x > y) {
        return 1;
    }
    return -(x < y);
}

#ifdef VERSION_G8ME01
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
s32 winMgrSelect(WinMgrSelect* sel) {
    if (sel->flags & 0x1000) {
        if (sel->flags & 0x2000) {
            return -1;
        }
        if (sel->newItem && sel->cursor == 0 && (sel->flags & 0x100)) {
            return -1;
        }
        return sel->items[sel->cursor].id;
    }
    return 0;
}



static const char* select_party_name(u16 id);
s32 winMgrSelectOther(WinMgrSelect* sel, EventEntry* evt) {
    InformantWork* jdt = &_jdt;
    s32 id;
    const char* name;
    char buf[0x40];

    if (sel->flags & 0x1000) {
    if (sel->flags & 0x2000) {
        return -1;
    }
    id = sel->items[sel->cursor].id;
    switch (sel->type) {
        case 6:
        case 7:
            evt->lwData[1] = party_id_table[id];
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

            evt->lwData[3] = (EvtWord)name;
            break;
        case 8:
            evt->lwData[2] = party_id_table[id];
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
            evt->lwData[4] = sel->cursor;
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
            evt->lwData[3] = ((evtGetValue(NULL, GSW_PIT_FLOOR) + 11) * itemDataTable[id].buy_price) / 10;
            evt->lwData[4] = itemDataTable[id].bp_cost;
            break;
        case 16:
            evt->lwData[1] = id;
            evt->lwData[2] = (EvtWord)VERSION_PICK(itemDataTable[id].id, msgSearch(itemDataTable[id].name));
            evt->lwData[3] = (itemDataTable[id].buy_price * 2 + 2) / 3;
            evt->lwData[4] = itemDataTable[id].bp_cost;
            break;
        case 17:
            sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
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
            sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[id].storyId);
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
    WinMgrSelectItem* items;
    int heap;
    WinMgrSelect* sel3;
    WinMgrSelectItem* items2;
    WinMgrSelectItem* items3;
    WinMgrSelect* sel2;
    items3 = (sel2 = (sel3 = sel))->items;
    items2 = items3;
    heap = (heap = HEAP_DEFAULT);
    items = items2;
    __memFree(HEAP_DEFAULT, items);
    __memFree(heap, sel3);
}

s32 winMgrSelectGetCursor(WinMgrSelect* sel) {
    return sel->cursor;
}

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
            if ((wp->mEntries[sel->descriptorId[0]].flags & 4)) {
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
            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                if (sel->items[sel->cursor].flags & 2) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                } else {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                    sel->state++;
                }
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                if (sel->flags & 0x100) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                    sel->flags |= 0x2000;
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
            if ((wp->mEntries[sel->descriptorId[0]].flags & 4)) {
                break;
            }
            for (i = 0; i < sel->numDescriptors; i++) {
                winMgrDelete(sel->descriptorId[i]);
            }
            sel->flags |= 0x1000;
            sel->state++;
            break;
        case 4:
            break;
    }
}

#ifdef VERSION_G8ME01
void winFontSetWidth(Vec translate, Vec scale, GXColor color, f32 width, const char* fmt, ...);
void winFontSetEdgeWidth(Vec translate, Vec scale, GXColor color, f32 width, const char* fmt, ...);
#endif
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
    tmp = white;
    iconcol = tmp;

    if (!(wp->mEntries[sel->descriptorId[0]].flags & 4)) {
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
#ifdef VERSION_G8ME01
            if (sel->items[i].flags & 8) {
                if (sel->items[i].flags & 1) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(ICON_092, (Vec){entry->x + 15, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xC0});
            }
#endif
            if (sel->items[i].flags & 1) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(item->icon_id, (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, iconcol);
            winFontInit();
#ifdef VERSION_G8ME01
            winFontSetWidth((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & 1) ? &gray : &black), 185.0f, msgSearch(item->name));
#else
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & 1) ? &gray : &black), msgSearch(item->name));
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
#ifdef VERSION_G8ME01
            tw = FontGetMessageWidth(buf);
            if (tw > 30.0) {
                tw = 30.0f;
            }
            winFontInit();
            winFontSetWidth((Vec){entry->x + entry->width - 10 - tw, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & 1) ? &gray : &black), 30.0f, buf);
#else
            tw = FontGetMessageWidth(buf);
            winFontInit();
            winFontSet((Vec){entry->x + entry->width - 10 - tw, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & 1) ? &gray : &black), buf);
#endif
        }
        if (sel->newItem) {
            winIconInit();
            for (k = 2; k < 30; k++) {
                winIconSet(ICON_1B7, (Vec){entry->x + k * entry->width / 31, (entry->y - 56) + sel->scrollY, 0.0f}, (Vec){0.15f, 0.15f, 0.15f}, black);
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
#ifdef VERSION_G8ME01
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
                iconDispGxCol(mtx, 16, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, 16, ICON_18B, 0.6f);
                break;
            case 13:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, 16, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, 16, ICON_18D, 0.6f);
                break;
            case 16:
                PSMTXTrans(mtx, entry->x + 268, entry->y - 18, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, 16, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){entry->x + 268, entry->y - 12, 0.0f}, 16, ICON_13F, 0.6f);
                break;
        }
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, 20, ICON_1F0, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, 16, ICON_1B5, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, 16, ICON_1B6, 0.6f);
            }
        }
    }
}


static inline s32 select_party_icon(u16 id) /* */ {
    s32 icon = party_icon_table[id];
    if (icon == ICON_158) {
        switch (pouchGetPartyColor(PARTNER_YOSHI)) {
            case 0:
                icon = ICON_158;
                break;
            case 1:
                icon = ICON_159;
                break;
            case 2:
                icon = ICON_15A;
                break;
            case 3:
                icon = ICON_15B;
                break;
            case 4:
                icon = ICON_15C;
                break;
            case 5:
                icon = ICON_15D;
                break;
            case 6:
            default:
                icon = ICON_15E;
                break;
        }
    }
    return icon;
}






void select_disp_party(WinMgrEntry* entry) {
#ifdef VERSION_G8ME01
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
    GXColor* col;
    s32 i;
    s32 j;
    const char* msg;
    u16 w;
    f32 sx;
    f32 ofs;
    tmp = white;
    iconcol = tmp;

    if (!(wp->mEntries[sel->descriptorId[0]].flags & 4)) {
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
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f},
                       (Vec){FontGetMessageWidth(select_party_name(sel->items[i].id)) > 170 ? 170.0f / FontGetMessageWidth(select_party_name(sel->items[i].id)) : 1.0f, 1.0f, 1.0f},
                       *((sel->items[i].flags & 1) ? &gray : &black), select_party_name(sel->items[i].id));
            if (sel->items[i].flags & 1) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(select_party_icon(sel->items[i].id), (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            for (j = 0; j < pouch->partyData[party_id_table[sel->items[i].id]].techLevel; j++) {
                if (sel->items[i].flags & 1) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(ICON_102, (Vec){entry->x + 253 + j * 27, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            }
        }
        GXSetScissor(left, top, wd, ht);
        msg = msgSearch("msg_window_title_7");
#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
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
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, 20, ICON_1F0, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, 16, ICON_1B5, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, 16, ICON_1B6, 0.6f);
            }
        }
    }
}

void select_disp_mario(WinMgrEntry* entry) {
#ifdef VERSION_G8ME01
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
    tmp = white;
    iconcol = tmp;

    if (!(wp->mEntries[sel->descriptorId[0]].flags & 4)) {
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
            if (sel->newItem == 0) {
                ofs = ofs + 0.0f;
            }
            winFontInit();
            col = &black;
            if (sel->items[i].flags & 1) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 60, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, MARIO_STATUS_NAME(id));
            if (sel->items[i].flags & 1) {
                winIconGrayInit();
            } else {
                winIconInit();
            }
            winIconSet(mario_status_icon_table[id], (Vec){entry->x + 35, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){0.55f, 0.55f, 0.55f}, iconcol);
            sprintf(buf, "%3d", *mario_status_point_table[id]);
            winFontInit();
            col = &black;
            if (sel->items[i].flags & 1) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 160, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            if (sel->items[i].flags & 2) {
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
            if (sel->items[i].flags & 1) {
                col = &gray;
            }
            winFontSet((Vec){entry->x + 240, sel->scrollY + (ofs + (entry->y - 32 - i * 24)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, *col, buf);
            winIconInit();
            winIconSet(ICON_163, (Vec){entry->x + 220, sel->scrollY + (ofs + (entry->y - 44 - i * 24)), 0.0f}, (Vec){-0.55f, 0.55f, 0.55f}, iconcol);
        }
        GXSetScissor(left, top, wd, ht);
        msg = msgSearch("msg_window_title_9");
#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
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
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, 20, ICON_1F0, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - 36, 1.0f}, 16, ICON_1B5, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){entry->x + entry->width / 2.0f, entry->y - entry->height - 12, 1.0f}, 16, ICON_1B6, 0.6f);
            }
        }
    }
}

void select_disp_luigi(WinMgrEntry* entry) {
#ifdef VERSION_G8ME01
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

    if (!(wp->mEntries[sel->descriptorId[0]].flags & 4)) {
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
            if (sel->items[i].flags & 1) {
                col = &gray;
            } else {
                col = &black;
            }
#ifdef VERSION_G8ME01
            winFontSetWidth((Vec){ent->x + 25, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, *col, 200.0f, info->initEndPos);
#else
            winFontSet((Vec){ent->x + 25, sel->scrollY + (zero + (ent->y - 32 - i * 24)), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, *col, info->initEndPos);
#endif
            info->initEndPos[info->tagEndPos] = save;
            if (sel->type == 17 && !(sel->items[i].flags & 1)) {
                sprintf(buf, "%3d", info->cost);
                winFontInit();
                col = &black;
                if (sel->items[i].flags & 1) {
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
#ifdef VERSION_G8ME01
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
                iconDispGxCol(mtx, 16, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                iconDispGx(&(Vec){ent->x + 268, ent->y - 12, 0.0f}, 16, ICON_18B, 0.6f);
                break;
        }
        iconDispGx(&(Vec){sel->cursorX, sel->cursorY, 1.0f}, 20, ICON_1F0, 1.0f);
        if (sel->numItems > 8 && (gp->retraceCount & 31) < 20) {
            if (sel->scrollTop != 0) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - 36, 1.0f}, 16, ICON_1B5, 0.6f);
            }
            if (sel->scrollTop != sel->numItems - 8) {
                iconDispGx(&(Vec){ent->x + ent->width / 2.0f, ent->y - ent->height - 12, 1.0f}, 16, ICON_1B6, 0.6f);
            }
        }
    }
}

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
    if (!(wp->mEntries[sel->descriptorId[1]].flags & 4)) {
#ifdef VERSION_G8ME01
        Mtx trans;
        Mtx scale;
        f32 tw;
        WinMgrEntry* e;

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
        {
            s32 ey, eh, ex, ew;
            s32 h;
            WinMgrDesc* d = entry->description;
            h = lines * 22;
            e = &wp->mEntries[sel->descriptorId[1]];
            eh = d->height + h;
            ew = entry->width;
            ex = entry->x;
            ey = d->y + h / 2;
            if (e->flags & 1) {
                e->x = ex;
                e->y = ey;
                e->width = ew;
                e->height = eh;
            }
        }
#else
        w = (entry->width - FontGetMessageWidthLine(msg, &lines)) / 2;
        FontDrawStart();
        FontDrawMessage(entry->x + w, entry->y - (entry->height - (lines + 1) * 24) / 2, msg);
#endif
    }
}

void select_main3(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}

void select_disp3(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;

    if (!(wp->mEntries[sel->descriptorId[2]].flags & 4)) {
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
    if (!(wp->mEntries[sel->descriptorId[2]].flags & 4)) {
        if (sel->items[sel->cursor].flags & 2) {
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

void select_disp3_mario(WinMgrEntry* entry) {
    WinMgrSelect* sel = entry->param;
    char buf[0x48];
    const char* help;

    pouchGetPtr();
    if (!(wp->mEntries[sel->descriptorId[2]].flags & 4)) {
        if (sel->items[sel->cursor].flags & 2) {
            if (sel->items[sel->cursor].flags & 4) {
                help = "helperr2";
            } else {
                help = "helperr";
            }
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

static const char name_party0[] = "name_party0";
static const char name_party1[] = "name_party1";
static const char name_party4[] = "name_party4";
static const char name_party5[] = "name_party5";
static const char name_party2[] = "name_party2";
static const char name_party6[] = "name_party6";
