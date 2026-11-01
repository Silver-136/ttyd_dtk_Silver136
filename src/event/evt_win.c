// Script commands for the unit windows (unitwin): the shop, partner and item list windows.

#include "version.h"
#if VERSION <= VERSION_DEMO_US
#define DTRIGF_PLAIN_DATA
#include <dtrigf.h>
#include "evt/evt_cmd.h"
#else
#include "evt/evt_cmd.h"
#endif
#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/hitdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "data/item_data.h"
#include "driver/windowdrv.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_lottery.h"
#include "pmario_sound.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <stdio.h>
#include <string.h>
#if VERSION <= VERSION_DEMO_US
#include <stdlib.h>
#include "icon_ids.h"
#else
#include "icon_ids.h"
#endif
#include "mario/mario_party.h"
#include "driver/itemdrv.h"
#include "statuswindow.h"
#include "win/win_main.h"
#if VERSION <= VERSION_DEMO_US
#include "driver/casedrv.h"
#include "evt/evt_case.h"
#include "data_tables.h"
#else
#include "data_tables.h"
#endif
#include <dolphin/pad.h>
#if VERSION <= VERSION_DEMO_US
#include "demo/DEMOPad.h"
#endif


void FontDrawString(f32 x, f32 y, const char* str);

typedef enum UnitWinItemFlags {
    UNITWIN_ITEM_UNAVAILABLE = 1 << 0, // empty goods slot or already purchased
} UnitWinItemFlags;

// UnitWinWork.flags
typedef enum UnitWinFlags {
    UNITWIN_FLAG_SELECTED = (1 << 0),             // 0x1, an entry is under the cursor
    UNITWIN_FLAG_BUYING = (1 << 1),               // 0x2, the entry's event is running
    UNITWIN_FLAG_WINMGR = (1 << 7),               // 0x80, evt_unitwin_alloc(1): window-manager windows, no main event
    UNITWIN_FLAG_LOTTERY = (1 << 8),              // 0x100, unitwin_lottery_disp_onoff
    UNITWIN_FLAG_COIN = (1 << 12),                // 0x1000, unitwincoin_disp_onoff: coin counter
    UNITWIN_FLAG_SHINE_WINDOW_UNUSED = (1 << 13), // 0x2000, Shine Sprite counter window; nothing sets it
    UNITWIN_FLAG_SHINE_UNUSED = (1 << 14),        // 0x4000, Shine Sprite counter; nothing sets it
    UNITWIN_FLAG_STAR_PIECE_UNUSED = (1 << 15),   // 0x8000, Star Piece counter; nothing sets it
} UnitWinFlags;

// UnitWinWork.flags2: evt_unitwin_free asks, the running event agrees
#define UNITWIN_FREE_REQUEST (1 << 0)
#define UNITWIN_FREE_OK (1 << 1)

typedef struct UnitWinWork {
    u16 flags;             // 0x0, UnitWinFlags
    u16 pad2;              // 0x2
    UnitWinSell* entries;  // 0x4
    s32* goods;            // 0x8
#if VERSION > VERSION_DEMO_US
    u16 itemFlags[6];      // 0xC
#endif
    s32 goodsMax;          // 0x18
    EventEntry* evt;       // 0x1C
    EventEntry* childEvt;  // 0x20
    s32 goodsIdx;          // 0x24
    s32 timer;             // 0x28
    s32 coin;              // 0x2C
    s32 coinDisp;          // 0x30
    f32 cursor;            // 0x34
#if VERSION <= VERSION_DEMO_US
    f32 scrollTop;         // 0x2C
#else
    u8 pad38[4];           // 0x38
#endif
    f32 cursorX;           // 0x3C
    f32 cursorY;           // 0x40
    f32 scroll;            // 0x44
    u8 pad48[4];           // 0x48
    s32 skipEntry;         // 0x4C, never written (0 after the memset); both lists skip the entry equal to it
    s32 mode;              // 0x50
    u16 flags2;            // 0x54, UNITWIN_FREE_*
#if VERSION <= VERSION_DEMO_US
    u8 pad4A[2];           // 0x4A
    s32 caseId[16];        // 0x4C, one case event per counter hit object
#else
    u8 pad56[2];           // 0x56
#endif
    s32 count;             // 0x58
    s32 coinTick;          // 0x5C
#if VERSION > VERSION_DEMO_US
    s32 winId[3];          // 0x60
    u8 pad6C[0xE0 - 0x6C]; // 0x6C
    s32 selected;          // 0xE0
    s32 bought;            // 0xE4
#endif
} UnitWinWork;

static s16* mario_status_point_table[3];
s32 unitwin_coin_se_wait;
static UnitWinWork* _wp;

USER_FUNC(evt_unitwin_main_func);
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(evt_unitwin_select_func);
#else
USER_FUNC(evt_unitwin_select_func);
#endif
void evt_unitwin_disp_func(CameraId cameraId, void* param);

void unitwin_help_main(WinMgrEntry* entry);
void unitwin_help_disp(WinMgrEntry* entry);
void unitwin_point_disp(WinMgrEntry* entry);
void unitwin_name_disp(WinMgrEntry* entry);

/** The three windows of the unit window: the name plate, the help text box and the points counter. */
#if VERSION > VERSION_DEMO_US
WinMgrDesc unitwin_desc[3] = {
    {0, 0, 8, -150, 120, 300, 50, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, unitwin_name_disp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, unitwin_help_main, unitwin_help_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, unitwin_point_disp},
};
#endif

/**
 * Tables of the unit window (partner and Mario lists): the partner ids in menu order (Goombella, Koops, Flurrie,
 * Yoshi, Vivian, Bobbery, Ms. Mowz, ended by 0), their icons and name labels, and the HP, FP and BP rows
 * (icon, label, name and the amount a level-up adds: 5, 5 and 3).
 */
static s32 party_id_table[8] = {PARTY_MEMBER_GOOMBELLA, PARTY_MEMBER_KOOPS, PARTY_MEMBER_FLURRIE, PARTY_MEMBER_YOSHI, PARTY_MEMBER_VIVIAN, PARTY_MEMBER_BOBBERY, PARTY_MEMBER_MS_MOWZ, PARTY_MEMBER_NONE};
static s32 party_icon_table[7] = {ICON_PARTY_GOOMBELLA, ICON_PARTY_KOOPS, ICON_PARTY_FLURRIE, ICON_PARTY_YOSHI, ICON_PARTY_VIVIAN, ICON_PARTY_BOBBERY, ICON_PARTY_MS_MOWZ};
static const char* party_labelname_table[7] = {"_kur", "_nok", "_win", "_yos", "_viv", "_bom", "_chu"};
static s32 mario_status_icon_table[3] = {ICON_HP, ICON_FP, ICON_BP};
static const char* mario_status_labelname_table[3] = {"_hp", "_fp", "_bp"};
static const char* mario_status_name_table[3] = {"\202g\202o", "\202e\202o", "\202a\202o"};
static s32 mario_status_henka_table[3] = {5, 5, 3};

/** Demo: the unit window's per-frame event (evt_unitwin_alloc starts it); runs evt_unitwin_main_func. */
static EVT_BEGIN(main_evt)
    CALL(evt_unitwin_main_func)
    RETURN
EVT_END()

EVT_BEGIN(select_evt)
    CALL(evt_unitwin_select_func)
    RETURN
EVT_END()

// Descriptive name (not in the symbol maps); placeholder unk_8017599c, JP 0x8017599C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
void unitwin_name_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    const char* msg = msgSearch(itemDataTable[wp->goods[wp->goodsIdx]].name);
    s32 i;
    if (!winMgrAction(wp->winId[0])) {
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString(-FontGetMessageWidth(msg) / 2, 108.0f, msg);
        FontDrawEdgeOff();
    }
    for (i = 0; i < wp->goodsMax; i++) {
        if (!(wp->itemFlags[i] & UNITWIN_ITEM_UNAVAILABLE)) {
            Mtx mtx;
            char buf[64];
            Vec pos;
            mapObjGetPos(wp->entries[i].mapObj, &pos);
            pos.y += 30.0f;
            PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &pos, &pos);
            PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos, &pos);
            pos.x *= gp->fbWidth / 2.0f;
            pos.y *= gp->efbHeight / 2.0f;
            sprintf(buf, "%d", itemDataTable[wp->goods[i]].discount_price);
            PSMTXTrans(mtx, 12.0f * (strlen(buf) - 1) + pos.x, pos.y, pos.z);
            iconNumberDispGx(mtx, itemDataTable[wp->goods[i]].discount_price, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
}
#endif

// never called: the linker strips it, its "%s_%04d" string stays in this object's .rodata (name descriptive)
#if VERSION > VERSION_DEMO_US
static void unitwin_fmt(char* buf, const char* name, s32 n) {
    sprintf(buf, "%s_%04d", name, n);
}
#endif

/** Help window update: runs the shared help-window logic. */
// Descriptive name (not in the symbol maps); placeholder unk_8017597c, JP 0x8017597C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Help window update: runs the shared help-window logic. */
void unitwin_help_main(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}
#endif

/** Help window draw: shows the description of the item at the selected counter slot. */
// Descriptive name (not in the symbol maps); placeholder unk_801758fc, JP 0x801758FC. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Help window draw: shows the description of the item at the selected counter slot. */
void unitwin_help_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    s32 idx = wp->goodsIdx * sizeof(s32); // FAKE: byte offset of the goods slot computed before winMgrAction (slwi ahead of the call, lwzx after), as in the target; goods[wp->goodsIdx] and an index local both scale after the call
    s32* goods = wp->goods;
    if (!winMgrAction(wp->winId[1])) {
        entry->msg = msgSearch(itemDataTable[*(s32*)((u8*)goods + idx)].description);
        winMgrHelpDraw(entry);
    }
}
#endif

/**
 * Draws the shop-point window: the points label and the count,
 * which steps a tenth of the way to Mario's shop points each frame.
 * The label icon differs between Japanese (language 0) and other text.
 */
// Descriptive name (not in the symbol maps); placeholder unk_80175710, JP 0x80175710. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Draws the shop-point window: the points label and the count,
 * which steps a tenth of the way to Mario's shop points each frame.
 * The label icon differs between Japanese (language 0) and other text.
 */
void unitwin_point_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
    Mtx trans;
    Mtx scale;
    s32 d;
    if (!(entry->flags & WINMGR_FLAG_ANIMATING)) {
        PSMTXTrans(trans, 10.0f + (30.0f + entry->x), entry->y - 36.0f, 0.0f);
        if (gp->language == LANGUAGE_JP) {
            PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, ICON_FLAG_2D, VERSION_PICK(0x1F1, 0x226));
        } else {
            PSMTXScale(scale, 0.8f, 1.1f, 1.1f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, ICON_FLAG_2D, VERSION_PICK(0x20A, 0x227));
        }
        PSMTXTrans(trans, 124.0f + entry->x, entry->y - 38.0f, 0.0f);
        {
            GXColor c = {0xFF, 0xFF, 0xFF, 0xFF};
            iconNumberDispGx(trans, wp->coinDisp, 0, c);
        }
        d = pouch->shopPoints - wp->coinDisp;
        if (d) {
            d /= 10;
            if (!d) {
                d = pouch->shopPoints > wp->coinDisp ? 1 : -1;
            }
            wp->coinDisp += d;
        }
    }
}
#else
static s32 iconid_conv(s32 icon) {
    if (icon == ICON_PARTY_YOSHI) {
        switch (pouchGetPartyColor(4)) {
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
 * Returns the display name of the partner in list row i (Goombella, Koops, Flurrie,
 * Yoshi by the player's chosen name, Vivian, Bobbery, Ms. Mowz order).
 */
static const char* party_name_table_rtn(s32 i) {
    const char* name;
    switch (i) {
        case 0:
            name = msgSearch("name_party0");
            break;
        case 1:
            name = msgSearch("name_party1");
            break;
        case 2:
            name = msgSearch("name_party4");
            break;
        case 3:
            name = pouchGetYoshiName();
            break;
        case 4:
            name = msgSearch("name_party5");
            break;
        case 5:
            name = msgSearch("name_party2");
            break;
        case 6:
            name = msgSearch("name_party6");
            break;
    }
    return name;
}
#endif
/**
 * Demo: sets up the unit window work (_wp), the pointing-finger cursor icon and the per-frame main_evt,
 * and points the HP/FP/BP table at Mario's base stats for the stat windows.
 * Must run before the other unitwin commands.
 */
USER_FUNC(evt_unitwin_alloc) {
#if VERSION > VERSION_DEMO_US
    s32 useWinMgr = evtGetValue(event, event->args[0]);
#endif
    UnitWinWork* wp = _mapAlloc(sizeof(UnitWinWork));
    memset(wp, 0, sizeof(UnitWinWork));
    wp->mode = 0;
    _wp = wp;
#if VERSION > VERSION_DEMO_US
    if (useWinMgr) {
        wp->flags |= UNITWIN_FLAG_WINMGR;
    } else {
        wp->flags &= ~UNITWIN_FLAG_WINMGR;
    }
#endif
    iconEntry2D("yubicursor_2", ICON_YUBI_CURSOR);
    iconFlagOn("yubicursor_2", ICON_FLAG_HIDDEN);
    iconFlagOn("yubicursor_2", ICON_FLAG_FLIP_X);
#if VERSION > VERSION_DEMO_US
    if (!(wp->flags & UNITWIN_FLAG_WINMGR)) {
        wp->evt = evtEntry(main_evt, 0, 0);
        wp->evt->lwData[0] = (EvtWord)wp;
    }
#else
    wp->evt = evtEntry(main_evt, 0, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
#endif
    mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
    mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
    mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
    return EVT_RETURN_DONE;
}

/**
 * Demo: tears the unit window down.
 * Blocks until main_evt has stopped, then removes the counter item icons and their case events,
 * the cursor icon and the work.
 */
USER_FUNC(evt_unitwin_free) {
#if VERSION > VERSION_DEMO_US
    UnitWinWork* wp = _wp;
    s32 i;
    char name[32];
#else
    s32 i;
    UnitWinWork* wp = _wp;
    char name[32];
    CaseEntry* entry;
#endif
    if (wp == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        wp->flags2 |= UNITWIN_FREE_REQUEST;
    }
    if (!(wp->flags2 & UNITWIN_FREE_OK)) {
        return EVT_RETURN_BLOCK;
    }
    for (i = 0; i < wp->goodsMax; i++) {
        mapObjFlagOff(wp->entries[i].mapObj, MAP_OBJ_FLAG_DISABLED);
        sprintf(name, "uwn%d", i);
        iconDelete(name);
#if VERSION <= VERSION_DEMO_US
        entry = caseIdToPtr(wp->caseId[i]);
        if (evtCheckID(entry->evtId)) {
            evtDeleteID(entry->evtId);
        }
        caseDelete(wp->caseId[i]);
#else
    }
    if (wp->flags & UNITWIN_FLAG_WINMGR) {
        for (i = 0; i < 3; i++) {
            winMgrDelete(wp->winId[i]);
        }
#endif
    }
    iconDelete("yubicursor_2");
    _mapFree(_wp);
    _wp = NULL;
    return EVT_RETURN_DONE;
}

/**
 * Demo: puts goods on a shop counter.
 * args[0] is a null-terminated table of (map object, hit object, script) rows, args[1] the item id for each row;
 * shows each item's icon over its map object and registers a case on its hit object that runs select_evt.
 */
USER_FUNC(evt_unitwin_selltable_setup) {
    s32 i;
    EvtWord* args = event->args;
    UnitWinSell* tbl = (UnitWinSell*)evtGetValue(event, args[0]);
    s32* goods = (s32*)evtGetValue(event, args[1]);
    UnitWinWork* wp = _wp;
#if VERSION <= VERSION_DEMO_US
    EvtWord caseWork[16];
#endif
    char name[32];
    Vec pos;
    wp->entries = tbl;
    wp->goods = goods;
    wp->goodsMax = 0;
    for (i = 0; wp->entries[i].mapObj != NULL; i++) {
        wp->goodsMax++;
        mapObjGetPos(wp->entries[i].mapObj, &pos);
        mapObjFlagOn(wp->entries[i].mapObj, MAP_OBJ_FLAG_DISABLED);
        if (wp->goods[i] != 0) {
            sprintf(name, "uwn%d", i);
            iconEntry(name, itemDataTable[wp->goods[i]].icon_id);
            iconSetPos(name, pos.x, pos.y + 6.0f, pos.z);
            iconFlagOn(name, ICON_FLAG_NO_MAP_BLEND);
#if VERSION > VERSION_DEMO_US
        } else {
            wp->itemFlags[i] |= UNITWIN_ITEM_UNAVAILABLE;
#endif
        }
#if VERSION <= VERSION_DEMO_US
        caseWork[0] = (EvtWord)wp;
        caseWork[1] = i;
        wp->caseId[i] = evtRunCaseEntry(0, 0, wp->entries[i].hitName, 0, select_evt, caseWork);
#endif
    }
#if VERSION > VERSION_DEMO_US
    wp->evt = evtEntryType(select_evt, 0, EVENT_START_IMMEDIATE, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->winId[i] = winMgrEntry(&unitwin_desc[i]);
    }
    wp->selected = -1;
#endif
    return EVT_RETURN_DONE;
}

/**
 * Shows (args[0] nonzero) or hides the coin counter window,
 * starting it from Mario's current coins so it counts up or down to later changes.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(evt_unitwin_select_func) {
    UnitWinWork* wp = (UnitWinWork*)event->lwData[0];
    if (wp->mode == 0) {
        wp->goodsIdx = event->lwData[1];
        if (wp->goods[wp->goodsIdx] != 0) {
            wp->timer = 1;
            wp->flags |= UNITWIN_FLAG_SELECTED;
            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                marioKeyOff();
                wp->childEvt = evtEntry(wp->entries[wp->goodsIdx].evt, 0, 0);
                wp->childEvt->lwData[0] = (EvtWord)wp;
                wp->mode = 1;
                return EVT_RETURN_BLOCK;
            }
        }
        return EVT_RETURN_DONE;
    }
    if (evtCheckID(wp->childEvt->eventId)) {
        return EVT_RETURN_BLOCK;
    }
    wp->mode = 0;
    marioKeyOn();
    return EVT_RETURN_YIELD;
}
#endif
/**
 * Shows (args[0] nonzero) or hides the coin counter window,
 * starting it from Mario's current coins so it counts up or down to later changes.
 */
USER_FUNC(unitwincoin_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    UnitWinWork* wp;
    pouchGetPtr();
    wp = _wp;
    wp->coin = pouchGetCoin();
    wp->coinTick = 0;
    if (on) {
        wp->flags |= UNITWIN_FLAG_COIN;
    } else {
        wp->flags &= ~UNITWIN_FLAG_COIN;
    }
    return EVT_RETURN_DONE;
}

/** Blocks until the coin counter window has caught up with Mario's coin total. */
USER_FUNC(unitwincoin_wait) {
    UnitWinWork* wp = _wp;
    pouchGetPtr();
    if (wp->coin != pouchGetCoin()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Shows (args[0] nonzero) or hides the lottery ticket number (from evt_lottery) in the unit window;
 * it is drawn only while the lottery reports a ticket.
 */
// Descriptive name (not in the symbol maps); placeholder unk_8017524c, JP 0x8017524C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Shows (args[0] nonzero) or hides the lottery ticket number (from evt_lottery) in the unit window;
 * it is drawn only while the lottery reports a ticket.
 */
USER_FUNC(unitwin_lottery_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    UnitWinWork* wp = _wp;
    if (on) {
        wp->flags |= UNITWIN_FLAG_LOTTERY;
    } else {
        wp->flags &= ~UNITWIN_FLAG_LOTTERY;
    }
    return EVT_RETURN_DONE;
}
#endif

/**
 * Tells the counter whether the purchase script finished with a sale:
 * args[0] is the unit window work, args[1] nonzero marks the slot sold so its item disappears.
 */
// Descriptive name (not in the symbol maps); placeholder unk_801751f8, JP 0x801751F8. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Tells the counter whether the purchase script finished with a sale:
 * args[0] is the unit window work, args[1] nonzero marks the slot sold so its item disappears.
 */
USER_FUNC(unitwin_set_bought) {
    EvtWord* args = event->args;
    UnitWinWork* wp = (UnitWinWork*)evtGetValue(event, args[0]);
    wp->bought = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static inline void unitWinClose(UnitWinWork* wp) {
    if (wp->selected != -1) {
        wp->selected = -1;
        winMgrClose(wp->winId[0]);
        winMgrClose(wp->winId[1]);
    }
}

// Descriptive name (not in the symbol maps); placeholder unk_80174e98, JP 0x80174E98. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_unitwin_select_func) {
    UnitWinWork* wp = (UnitWinWork*)event->lwData[0];
    s32 found = FALSE;
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    HitEntry* hit = mario->hitobjStandOn;
    u32 btn;
    s32 i;
    char name[32];
    MapObject* obj;
    if (wp->flags2 & UNITWIN_FREE_REQUEST) {
        wp->flags2 |= UNITWIN_FREE_OK;
        return EVT_RETURN_DONE;
    }
    wp->flags &= ~UNITWIN_FLAG_SELECTED;
    btn = keyGetButtonTrg(0);
    // FAKE: `x ? FALSE : TRUE` in place of `!x`; !marioChkCtrl() || !marioChkKey() changes the object
    if ((marioChkCtrl() ? FALSE : TRUE) || (marioChkKey() ? FALSE : TRUE)) {
        btn = 0;
    }
    if (party != NULL && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        unitWinClose(wp);
        return EVT_RETURN_BLOCK;
    }
    if (winGhostDiaryChk() || itemPickUpFromFieldCheck()) {
        unitWinClose(wp);
        return EVT_RETURN_BLOCK;
    }
    if (!(wp->flags & UNITWIN_FLAG_BUYING)) {
        if (hit != NULL) {
            for (i = 0; i < wp->goodsMax; i++) {
                const char* hitName = hitGetName(hit);
                if (!strcmp(wp->entries[i].hitName, hitName)) {
                    break;
                }
            }
            if (!(wp->itemFlags[i] & UNITWIN_ITEM_UNAVAILABLE) && i < wp->goodsMax) {
                if (wp->selected != i) {
                    if (wp->selected == -1) {
                        winMgrOpen(wp->winId[0]);
                        winMgrOpen(wp->winId[1]);
                    }
                    wp->selected = i;
                    wp->goodsIdx = i;
                    winMgrHelpInit(winMgrGetPtr(wp->winId[1]));
                }
                found = TRUE;
            }
        }
        if (!found) {
            unitWinClose(wp);
        }
        if (wp->selected != -1) {
            wp->flags |= UNITWIN_FLAG_SELECTED;
            if (btn & PAD_BUTTON_A) {
                marioKeyOff();
                wp->childEvt = evtEntry(wp->entries[wp->goodsIdx].evt, 0, 0);
                wp->childEvt->lwData[0] = (EvtWord)wp;
                wp->flags |= UNITWIN_FLAG_BUYING;
                winMgrClose(wp->winId[0]);
                winMgrClose(wp->winId[1]);
                wp->selected = -1;
                wp->bought = 0;
            }
        }
    } else if (!evtCheckID(wp->childEvt->eventId)) {
        wp->flags &= ~UNITWIN_FLAG_BUYING;
        marioKeyOn();
        if (wp->bought) {
            wp->itemFlags[wp->goodsIdx] |= 1;
        }
    }
    for (i = 0; i < wp->goodsMax; i++) {
        obj = mapGetMapObj(wp->entries[i].mapObj);
        if (obj != NULL) {
            sprintf(name, "uwn%d", i);
            if (wp->itemFlags[i] & UNITWIN_ITEM_UNAVAILABLE) {
                iconFlagOn(name, ICON_FLAG_HIDDEN);
            } else if (obj->flags & MAP_OBJ_FLAG_HIDDEN) {
                iconFlagOn(name, ICON_FLAG_HIDDEN);
            } else {
                iconFlagOff(name, ICON_FLAG_HIDDEN);
#else
// Descriptive name (not in the symbol maps); placeholder unk_80174e98, JP 0x80174E98. See docs/NAME_PROVENANCE.md.
/** list window: arg = window kind; returns lwData[0] = 1 and the picked entry in lwData[1..] */
USER_FUNC(unitwin_window_select_func) {
    EvtWord* args = event->args;
    s32 arg = evtGetValue(event, *args++);
    UnitWinWork* wp = _wp;
    PouchData* pouch;
    const char* name;
    s32 prev;
    s32 i;
    s32 k;
    s32 j;
    s32 n;
    s32 ng;
    s32* p;
    s32 t;
    s32 id;
    InformantWork* jdt = &_jdt;
    // FAKE: `x ? FALSE : TRUE` in place of `!x`; !marioChkCtrl() || !marioChkKey() changes the object
    if (isFirstCall) {
        wp->mode = arg;
        wp->cursor = 0.0f;
        wp->scrollTop = 0.0f;
        wp->scroll = 0.0f;
        wp->cursorX = -25.0f;
        wp->cursorY = 60.0f - 24.0f * (wp->cursor - wp->scrollTop);
        statusWinForceOpen();
        return EVT_RETURN_BLOCK;
    }
    switch (wp->mode) {
        case 2:
        case 3:
            wp->count = 0;
            for (k = 0; k < 20; k++) {
                if (pouchHaveItem(k)) {
                    wp->count++;
                }
            }
            break;
        case 12:
            wp->count = 0;
            for (k = 0; k < 200; k++) {
                if (pouchHaveBadge(k)) {
                    wp->count++;
            }
        }
        break;
    case 11:
        wp->count = 0;
        for (k = 0; k < 99; k++) {
            if ((s16)badgeShop_get(bdsw->bargain_table, k + 0xF0) - badgeShop_ThrowCheck(k + 0xF0) > 0) {
                wp->count++;
            }
        }
    break;
    case 17:
    case 18:
    wp->count = jdt->count;
    break;
    case 13:
    wp->count = 0;
    for (k = 0; k < 99; k++) {
        if ((s16)badgeShop_get(bdsw->starmaniac_table, k + 0xF0) > 0) {
            wp->count++;
        }
    }
    break;
    case 14:
    wp->count = 0;
    for (k = 0; k < 0x152; k++) {
        if ((s16)badgeShop_get(bdsw->bottakuru_table, k + 1) > 0) {
            wp->count++;
        }
    }
    break;
    case 15:
    wp->count = 0;
    for (k = 0; badge_bottakuru100_table[k] != 0; k++) {
        wp->count++;
    }
    break;
    case 16:
    wp->count = 0;
    for (k = 0; k < 0x152; k++) {
        if ((s16)badgeShop_get(bdsw->bteresa_table, k + 1) > 0) {
            wp->count++;
        }
    }
    break;
    case 4:
    wp->count = 0;
    for (k = 0; k < 32; k++) {
        if (pouchKeepItem(k)) {
            wp->count++;
        }
    }
    break;
    case 5:
    case 6:
    wp->count = 0;
    for (k = 0; (id = party_id_table[k]) != 0; k++) {
        if (partyChkJoin(id)) {
            wp->count++;
        }
    }
    break;
    case 7:
    wp->skipEntry = event->lwData[1];
    wp->count = 0;
    for (k = 0; (id = party_id_table[k]) != 0; k++) {
        if (partyChkJoin(id) && wp->skipEntry != party_id_table[k]) {
            wp->count++;
        }
    }
    break;
    case 8:
    case 9:
    wp->count = 3;
    break;
    case 10:
    wp->skipEntry = event->lwData[1];
    wp->count = 2;
    break;
    }

    prev = wp->cursor;
    if (keyGetDirRep(0) & DEMO_STICK_DOWN) {
        wp->cursor += 1.0f;
    }
    if (keyGetDirRep(0) & DEMO_STICK_UP) {
        wp->cursor -= 1.0f;
    }
    if (prev != (s32)wp->cursor) {
        psndSFXOn((const char*)5);
    }
    if (wp->cursor >= wp->count) {
        wp->cursor = 0.0f;
        wp->scrollTop = 0.0f;
    }
    if (wp->cursor < 0.0f) {
        wp->cursor = wp->count - 1;
        if ((t = wp->count - 8) > 0) {
    wp->scrollTop = t;
}
    }
    if (prev < wp->cursor) {
        if (wp->cursor > 5.0f && wp->scrollTop < wp->count - 8) {
    wp->scrollTop += 1.0f;
}
    } else if (prev > wp->cursor) {
        if (wp->cursor - wp->scrollTop < 3.0f && wp->scrollTop > 0.0f) {
    wp->scrollTop -= 1.0f;
}
    }
    wp->scroll += (24.0f * wp->scrollTop - wp->scroll) / 6.0f;
    wp->cursorX += (-25.0f - wp->cursorX) / 6.0f;
    wp->cursorY += ((60.0f - 24.0f * (wp->cursor - wp->scrollTop)) - wp->cursorY) / 6.0f;

    if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
        n = 0;
        switch (wp->mode) {
    case 2:
    case 3:
        statusWinForceOff();
        for (i = 0; i < 20; i++) {
            if (pouchHaveItem(i)) {
                if (n == wp->cursor) {
                    break;
                }
                n++;
            }
        }
        event->lwData[0] = 1;
        event->lwData[1] = pouchHaveItem(i);
        event->lwData[2] = (EvtWord)itemDataTable[pouchHaveItem(i)].id;
        event->lwData[3] = itemDataTable[pouchHaveItem(i)].sell_price;
        wp->mode = 0;
        return EVT_RETURN_YIELD;
    case 12:
        statusWinForceOff();
        for (i = 0; i < 200; i++) {
            if (pouchHaveBadge(i)) {
                if (n == wp->cursor) {
                    break;
                }
                n++;
            }
        }
        event->lwData[0] = 1;
        event->lwData[1] = pouchHaveBadge(i);
        event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
        event->lwData[3] = itemDataTable[event->lwData[1]].sell_price;
        wp->mode = 0;
        return EVT_RETURN_YIELD;
    case 11:
        statusWinForceOff();
        for (i = 0; i < 99; i++) {
            if ((s16)badgeShop_get(bdsw->bargain_table, i + 0xF0) - badgeShop_ThrowCheck(i + 0xF0) > 0) {
                if (n == wp->cursor) {
                    break;
                }
                n++;
            }
        }
        event->lwData[0] = 1;
        event->lwData[1] = i + 0xF0;
        event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
        event->lwData[3] = itemDataTable[event->lwData[1]].buy_price;
        event->lwData[4] = itemDataTable[event->lwData[1]].bp_cost;
        wp->mode = 0;
        return EVT_RETURN_YIELD;
    case 17:
        statusWinForceOff();
        j = wp->cursor;
        sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[j].storyId);
        jdt->curIndex = j;
        event->lwData[0] = 1;
        event->lwData[1] = (EvtWord)jdt->infoEnd;
        event->lwData[2] = (EvtWord)jdt->info[j].initEndPos;
        event->lwData[3] = jdt->info[j].tagEndPos;
        if (johoya_get(jdt->readFlags.perfectRead, jdt->info[j].storyId)) {
            event->lwData[4] = 0;
            } else {
                event->lwData[4] = jdt->info[j].cost;
            }
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 18:
            statusWinForceOff();
            psndSFXOn((const char*)0x12);
            j = wp->cursor;
            sprintf((char*)jdt->infoEnd, "%s_%04d", jdt->prefix, jdt->info[j].storyId);
            jdt->curIndex = j;
            event->lwData[0] = 1;
            event->lwData[1] = (EvtWord)jdt->infoEnd;
            event->lwData[2] = (EvtWord)jdt->info[j].initEndPos;
            event->lwData[3] = jdt->info[j].tagEndPos;
            if (johoya_get(jdt->readFlags.luigiRead, jdt->info[j].storyId)) {
                event->lwData[4] = 0;
            } else {
                event->lwData[4] = 1;
            }
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 13:
            statusWinForceOff();
            for (i = 0; i < 99; i++) {
                if ((s16)badgeShop_get(bdsw->starmaniac_table, i + 0xF0) > 0) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            if (itemDataTable[i + 0xF0].star_piece_price > pouchGetStarPiece()) {
                break;
            }
            event->lwData[0] = 1;
            event->lwData[1] = i + 0xF0;
            event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
            event->lwData[3] = itemDataTable[event->lwData[1]].star_piece_price;
            event->lwData[4] = itemDataTable[event->lwData[1]].bp_cost;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 14:
            statusWinForceOff();
            for (i = 0; i < 0x152; i++) {
                if ((s16)badgeShop_get(bdsw->bottakuru_table, i + 1) > 0) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            event->lwData[0] = 1;
            event->lwData[1] = i + 1;
            event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
            event->lwData[3] = (itemDataTable[event->lwData[1]].buy_price * 12) / 10;
            event->lwData[4] = itemDataTable[event->lwData[1]].bp_cost;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 15:
            statusWinForceOff();
            for (i = 0; badge_bottakuru100_table[i] != 0; i++) {
                if (n == wp->cursor) {
                    break;
                }
                n++;
            }
            event->lwData[0] = 1;
            event->lwData[1] = badge_bottakuru100_table[i];
            event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
            event->lwData[3] = ((evtGetValue(NULL, GSW_PIT_FLOOR) + 11) * itemDataTable[event->lwData[1]].buy_price) / 10;
            event->lwData[4] = itemDataTable[event->lwData[1]].bp_cost;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 16:
            statusWinForceOff();
            for (i = 0; i < 0x152; i++) {
                if ((s16)badgeShop_get(bdsw->bteresa_table, i + 1) > 0) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            event->lwData[0] = 1;
            event->lwData[1] = i + 1;
            event->lwData[2] = (EvtWord)itemDataTable[event->lwData[1]].id;
            event->lwData[3] = (itemDataTable[event->lwData[1]].buy_price * 2 + 2) / 3;
            event->lwData[4] = itemDataTable[event->lwData[1]].bp_cost;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 4:
            statusWinForceOff();
            for (i = 0; i < 32; i++) {
                if (pouchKeepItem(i)) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            event->lwData[0] = 1;
            event->lwData[1] = pouchKeepItem(i);
            event->lwData[2] = (EvtWord)itemDataTable[pouchKeepItem(i)].id;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 5:
        case 6:
            pouch = pouchGetPtr();
            for (i = 0; (id = party_id_table[i]) != 0; i++) {
                if (partyChkJoin(id)) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            if (!evtGetValue(event, GSWF_GOR_UP_ARROW_USED)) {
                if (pouch->partyData[party_id_table[i]].techLevel < 1) {
                    ng = 0;
                } else {
                    ng = 1;
                }
            } else {
                if (pouch->partyData[party_id_table[i]].techLevel < 2) {
                    ng = 0;
                } else {
                    ng = 2;
                }
            }
            if (ng) {
                break;
            }
            statusWinForceOff();
            event->lwData[0] = 1;
            event->lwData[1] = party_id_table[i];
            if (arg == 6) {
                name = party_name_table_rtn(i);
                event->lwData[3] = (EvtWord)name;
            }
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 7:
            pouch = pouchGetPtr();
            for (i = 0, p = party_id_table; (id = *p) != 0; p++, i++) {
                if (partyChkJoin(id) && wp->skipEntry != *p) {
                    if (n == wp->cursor) {
                        break;
                    }
                    n++;
                }
            }
            if ((pouch->partyData[party_id_table[i]].techLevel > 0 ? 0 : 1) != 0) {
                break;
            }
            statusWinForceOff();
            event->lwData[0] = 1;
            event->lwData[2] = party_id_table[i];
            name = party_name_table_rtn(i);
            event->lwData[4] = (EvtWord)name;
            wp->mode = 0;
            return EVT_RETURN_YIELD;
        case 8:
            pouchGetPtr();
            for (i = 0; i < wp->count; i++) {
                if (n == wp->cursor) {
                    break;
                }
                n++;
            }
            ng = 0;
            for (j = 0; j < wp->count; j++) {
                if (j != wp->cursor) {
                    if (*mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                        ng = 1;
#endif
            }
        }
    }
#if VERSION <= VERSION_DEMO_US
    if (ng) {
        break;
    }
    statusWinForceOff();
    event->lwData[0] = 1;
    event->lwData[1] = i;
    event->lwData[2] = (EvtWord)mario_status_name_table[i];
    event->lwData[3] = (EvtWord)mario_status_name_table[i == 0];
    event->lwData[4] = (EvtWord)mario_status_name_table[i != 2 ? 2 : 1];
    for (j = 0; j < wp->count; j++) {
        if (j == wp->cursor) {
            event->lwData[5 + j] = *mario_status_point_table[j] + mario_status_henka_table[j] * 2;
        } else {
            event->lwData[5 + j] = *mario_status_point_table[j] - mario_status_henka_table[j];
        }
    }
    wp->mode = 0;
    return EVT_RETURN_YIELD;
    case 9:
    pouchGetPtr();
    for (i = 0; i < wp->count; i++) {
        if (n == wp->cursor) {
            break;
        }
        n++;
    }
    ng = 0;
    for (j = 0; j < wp->count; j++) {
        if (j != wp->cursor) {
            if (*mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                ng++;
            }
        }
    }
    if (ng >= 2) {
        break;
    }
    statusWinForceOff();
    event->lwData[0] = 1;
    event->lwData[1] = i;
    event->lwData[2] = (EvtWord)mario_status_name_table[i];
    for (j = 0; j < wp->count; j++) {
        if (j == wp->cursor) {
            event->lwData[5 + j] = *mario_status_point_table[j] + mario_status_henka_table[j];
        } else {
            event->lwData[5 + j] = *mario_status_point_table[j];
        }
    }
    wp->mode = 0;
    return EVT_RETURN_YIELD;
    case 10:
    pouchGetPtr();
    for (i = 0; i < 3; i++) {
        if (i != wp->skipEntry) {
            if (n == wp->cursor) {
                break;
            }
            n++;
        }
    }
    if (*mario_status_point_table[i] - mario_status_henka_table[i] <= 0) {
        break;
    }
    statusWinForceOff();
    event->lwData[0] = 1;
    event->lwData[1] = i;
    event->lwData[3] = (EvtWord)mario_status_name_table[i];
    event->lwData[5 + i] = *mario_status_point_table[i] - mario_status_henka_table[i];
    wp->mode = 0;
    return EVT_RETURN_YIELD;
        }
    } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
        statusWinForceOff();
        psndSFXOn((const char*)0x13);
        event->lwData[0] = 0;
        wp->mode = 0;
        return EVT_RETURN_YIELD;
    }
#endif
    return EVT_RETURN_BLOCK;
}

/**
 * Demo: per-frame loop of the unit window: counts the name/description timer down
 * and queues evt_unitwin_disp_func for drawing, until evt_unitwin_free asks it to stop.
 */
USER_FUNC(evt_unitwin_main_func) {
    UnitWinWork* wp = (UnitWinWork*)event->lwData[0];
    if (wp->flags2 & UNITWIN_FREE_REQUEST) {
        wp->flags2 |= UNITWIN_FREE_OK;
        return EVT_RETURN_DONE;
    }
    if (wp->timer > 0) {
        wp->timer--;
    } else {
        wp->flags &= ~UNITWIN_FLAG_SELECTED;
    }
    dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, evt_unitwin_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

static inline s32 unitwin_party_icon(s32 icon) {
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

#define ICONGX2 ((void (*)(Vec*, u16, f32, u16))iconDispGx)

static inline const char* unitwin_party_name(s32 i) {
    const char* name;
    switch (i) {
        case 0:
            name = msgSearch("name_party0");
            break;
        case 1:
            name = msgSearch("name_party1");
            break;
        case 2:
            name = msgSearch("name_party4");
            break;
        case 3:
            name = pouchGetYoshiName();
            break;
        case 4:
            name = msgSearch("name_party5");
            break;
        case 5:
            name = msgSearch("name_party2");
            break;
        case 6:
            name = msgSearch("name_party6");
            break;
    }
    return name;
}
/**
 * Draws the unit window each frame: the item name and description for the highlighted counter slot with prices,
 * the counter window that counts toward Mario's coins (or Star Pieces, or pouchGetSuperCoin with flag 0x4000),
 * the demo's Shine Sprite counter, the lottery number (not in the demos), and the list windows by mode:
 * badge lists (2-3, 11-16), the informant's rumour lists (17-18),
 * the partner lists (5-7) and the HP/FP/BP lists (8-10),
 * with the finger cursor and the help text of the highlighted row.
 */
#if VERSION <= VERSION_DEMO_US
static void evt_unitwin_disp_func(CameraId cameraId, void* param) {
#else
void evt_unitwin_disp_func(CameraId cameraId, void* param) {
#endif
    PouchData* pouch = pouchGetPtr();
    UnitWinWork* wp = (UnitWinWork*)param;
    s32 mode;
    s32 row;
    s32 i;
    s32 flag;
    s32 lv;

    flag = 0; // FAKE: dead store, moves flag's variable record before i so the case 5-7 flag web is a split temp coloured before i's (target r21)
    if (wp->flags & UNITWIN_FLAG_SELECTED) {
        const char* name = itemDataTable[wp->goods[wp->goodsIdx]].id;
        const char* desc = itemDataTable[wp->goods[wp->goodsIdx]].description;

        windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, -150.0f, 120.0f, 300.0f, 50.0f, 20.0f);
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString((s32)(strlen(name) * 24 / 4) * -1, 108.0f, name);
        FontDrawEdgeOff();
        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
        FontDrawStart();
        FontDrawMessage(-220, -138, msgSearch(desc));

        for (i = 0; i < wp->goodsMax; i++) {
            if (wp->goods[i] != 0) {
                Vec pos;
                f32 proj[7];
                f32 vp[6];
                f32 sx, sy, sz;
                Mtx mtx;
                CameraEntry* cam;
                s32 id;
                u8 price;

                mapObjGetPos(wp->entries[i].mapObj, &pos);
                cam = camGetPtr(CAMERA_3D);
                GXSetProjection(cam->projection, cam->projType);
                GXGetProjectionv(proj);
                GXGetViewportv(vp);
                GXProject(pos.x, pos.y, pos.z, cam->view, proj, vp, &sx, &sy, &sz);
                cam = camGetPtr(cameraId);
                GXSetProjection(cam->projection, cam->projType);
                id = wp->goods[i];
                if (id >= BADGE_MIN && id < BADGE_MAX) {
                    price = itemDataTable[id].discount_price;
                } else {
                    price = itemDataTable[id].buy_price;
                }
                PSMTXTrans(mtx, 8.0f + (-304.0f + sx), 240.0f - (sy - 60.0f), 0.0f);
                iconNumberDispGx(mtx, price, 1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
        }
    }

    if (wp->flags & (UNITWIN_FLAG_COIN | UNITWIN_FLAG_SHINE_UNUSED | UNITWIN_FLAG_STAR_PIECE_UNUSED)) {
        Mtx mtx;
        Mtx scale;
        s32 coin;
        s32 delta;

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -75.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -220.0f, -113.0f, 0.0f);
        PSMTXScale(scale, 0.7f, 0.7f, 0.7f);
        PSMTXConcat(mtx, scale, mtx);
        if (wp->flags & UNITWIN_FLAG_COIN) {
            iconDispGx2(mtx, ICON_FLAG_2D, ICON_ITEM_COIN);
            coin = pouchGetCoin();
        } else if (wp->flags & UNITWIN_FLAG_SHINE_UNUSED) {
            iconDispGx2(mtx, ICON_FLAG_2D, ICON_ITEM_SHINE_SPRITE);
            coin = pouchGetSuperCoin();
        } else {
            iconDispGx2(mtx, ICON_FLAG_2D, ICON_ITEM_STAR_PIECE);
            coin = pouchGetStarPiece();
        }
        PSMTXTrans(mtx, -192.0f, -113.0f, 0.0f);
        iconDispGx2(mtx, ICON_FLAG_2D, ICON_TIMES);
        PSMTXTrans(mtx, -126.0f, -113.0f, 0.0f);
        iconNumberDispGx(mtx, wp->coin, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        if ((delta = coin - wp->coin)) {
            delta /= 10;
            if (!delta) {
                delta = coin > wp->coin ? 1 : -1;
            }
            if (wp->coinTick & 1) {
                wp->coin += delta;
                statusWinForceUpdateCoin();
            }
            wp->coinTick++;
            wp->coinTick %= 1200;
#if VERSION <= VERSION_DEMO_US
            if (abs(coin - wp->coin) == 1 || gp->retraceCount % 6 == 0) {
#else
            if (!unitwin_coin_se_wait) {
#endif
                psndSFXOn("SFX_COUNT_COIN1");
#if VERSION > VERSION_DEMO_US
                unitwin_coin_se_wait = 7;
#endif
            }
#if VERSION > VERSION_DEMO_US
        }
        if (unitwin_coin_se_wait > 0) {
            unitwin_coin_se_wait--;
#endif
        }
    }

    if (wp->flags & UNITWIN_FLAG_SHINE_WINDOW_UNUSED) {
        Mtx mtx;
        Mtx scale;
        s32 delta;

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -130.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -210.0f, -166.0f, 0.0f);
        PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
        PSMTXConcat(mtx, scale, mtx);
        iconDispGx2(mtx, ICON_FLAG_2D, VERSION_PICK(0x1F1, 0x226));
        PSMTXTrans(mtx, -126.0f, -168.0f, 0.0f);
        iconNumberDispGx(mtx, wp->coinDisp, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        if (!(gp->retraceCount & 3)) {
            if ((delta = pouch->shineSpriteCount - wp->coinDisp)) {
                delta /= 10;
                if (!delta) {
                    delta = pouch->shineSpriteCount > wp->coinDisp ? 1 : -1;
                }
                wp->coinDisp += delta;
            }
        }
    }
#if VERSION > VERSION_DEMO_US
    if ((wp->flags & UNITWIN_FLAG_LOTTERY) && (lotteryGetPtr()->flags & LOTTERY_FLAG_TICKET)) {
        Mtx mtx;
        char buf[32];

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -130.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -220.0f, -172.0f, 0.0f);
        iconDispGx2(mtx, ICON_FLAG_2D, ICON_ITEM_LOTTERY_PICK);
        sprintf(buf, "%04d", lotteryGetPtr()->number);
        for (i = 0; i < 4; i++) {
            PSMTXTrans(mtx, 20.0f * i + -126.0f - 60.0f, -168.0f, 0.0f);
            iconNumberDispGx(mtx, (s8)buf[i] - '0', 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
#endif
    switch (mode = wp->mode) {
        case 0:
        case 1:
            break;
        case 4: {
            const char* desc;
            s32 h;
            u32 sl, st, sw, sh;

            row = 0;
            desc = NULL;

            h = (8 - pouchGetKeepItemCnt() > 0 ? 8 - pouchGetKeepItemCnt() : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(81.0f, 111.0f, msgSearch("msg_window_title_1"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < MAX_STORED_ITEMS; i++) {
                s32 id = pouchKeepItem(i);
                ItemData* item = &itemDataTable[id];
                if (id) {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, ICON_FLAG_2D, 0.5f, item->icon_id);
                    FontDrawStart();
                    FontDrawString(36.0f, wp->scroll + (80 - row * 24), item->id);
                    if (row == wp->cursor) {
                        desc = item->description;
                    }
                    row++;
                }
            }
            GXSetScissor(sl, st, sw, sh);
            if (desc) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(desc));
            }
            break;
        }
        case 2:
        case 3:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16: {
            s32 h;
            s32* tbl;
            const char* title;
            s32 titleX;
            const char* desc;
            s32 max;
            u32 sl, st, sw, sh;

            row = 0;
            desc = NULL;
            if (mode == 2 || mode == 3) {
                h = (8 - pouchGetHaveItemCnt() > 0 ? 8 - pouchGetHaveItemCnt() : 0) * 24;
                max = MAX_HELD_ITEMS;
                title = msgSearch("msg_window_title_1");
                titleX = 30;
            } else if (mode == 12) {
                h = (8 - pouchGetHaveBadgeCnt() > 0 ? 8 - pouchGetHaveBadgeCnt() : 0) * 24;
                max = MAX_BADGES;
                title = msgSearch("msg_window_title_2");
                titleX = 40;
            } else if (mode == 14) {
                h = 8 - wp->count;
                h = (h > 0 ? h : 0) * 24;
                max = ITEM_TYPE_MAX - 1;
                title = msgSearch("msg_window_title_3");
                titleX = 20;
            } else if (mode == 15) {
                h = 8 - wp->count;
                h = (h > 0 ? h : 0) * 24;
                max = wp->count;
                title = msgSearch("msg_window_title_3");
                titleX = 20;
            } else if (mode == 16) {
                h = 8 - wp->count;
                h = (h > 0 ? h : 0) * 24;
                max = ITEM_TYPE_MAX - 1;
                title = msgSearch("msg_window_title_4");
                titleX = 30;
            } else {
                h = 8 - wp->count;
                h = (h > 0 ? h : 0) * 24;
                max = BADGE_COUNT;
                title = msgSearch("msg_window_title_2");
                titleX = 40;
            }
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(titleX + 51, 111.0f, title);
            FontDrawEdgeOff();
            if (wp->mode != 3) {
                Mtx mtx;
                Mtx scale;
                s32 icon;
                PSMTXTrans(mtx, 244.0f, 82.0f, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                if (wp->mode == 13) {
                    icon = ICON_ITEM_STAR_PIECE;
                } else if (wp->mode == 16) {
                    icon = ICON_ITEM_PIANTA_TOKEN;
                } else {
                    icon = ICON_ITEM_COIN;
                }
                {
                    iconDispGx(&(Vec){244.0f, 86.0f, 0.0f}, ICON_FLAG_2D, icon, 0.6f);
                }
            }
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (tbl = badge_bottakuru100_table, i = 0; i < max; i++) {
                GXColor color = {0x00, 0x00, 0x00, 0xFF};
                s32 id;
                ItemData* item;
                flag = 0;
                switch (wp->mode) {
                    case 2:
                    case 3:
                        id = pouchHaveItem(i);
                        break;
                    case 12:
                        id = pouchHaveBadge(i);
                        break;
                    case 11:
                        id = (s16)badgeShop_get(bdsw->bargain_table, i + BADGE_MIN);
                        if (id - badgeShop_ThrowCheck(i + BADGE_MIN) > 0) {
                            id = i + BADGE_MIN;
                        } else {
                            id = 0;
                        }
                        break;
                    case 13:
                        if ((s16)badgeShop_get(bdsw->starmaniac_table, i + BADGE_MIN) > 0) {
                            id = i + BADGE_MIN;
                        } else {
                            id = 0;
                        }
                        break;
                    case 14:
                        if ((s16)badgeShop_get(bdsw->bottakuru_table, i + 1) > 0) {
                            id = i + 1;
                        } else {
                            id = 0;
                        }
                        break;
                    case 15:
                        id = tbl[i];
                        break;
                    case 16:
                        if ((s16)badgeShop_get(bdsw->bteresa_table, i + 1) > 0) {
                            id = i + 1;
                        } else {
                            id = 0;
                        }
                        break;
                }
                item = &itemDataTable[id];
                if (id) {
                    if (wp->mode == 13 && item->star_piece_price > pouchGetStarPiece()) {
                        color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                        flag = ICON_FLAG_GRAY;
                    }
                    {
                        ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | ICON_FLAG_2D, 0.5f, item->icon_id);
                    }
                    FontDrawStart();
                    FontDrawScale(0.8f);
                    FontDrawColor(color);
                    FontDrawString(36.0f, wp->scroll + (80 - row * 24) - 2.0f, item->id);
                    if (wp->mode != 3) {
                        char buf[32];
                        f32 sc;
                        s32 xoff;
                        switch (wp->mode) {
                            case 14:
                                sprintf(buf, "%2d", item->buy_price * 12 / 10);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 15:
                                sprintf(buf, "%3d", item->buy_price * (evtGetValue(NULL, GSW_PIT_FLOOR) + 11) / 10);
                                sc = 1.0f;
                                xoff = -14;
                                break;
                            case 16:
                                sprintf(buf, "%4d", (item->buy_price * 2 + 2) / 3);
                                sc = 0.8f;
                                xoff = -20;
                                break;
                            case 11:
                                sprintf(buf, "%2d", item->buy_price);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 13:
                                sprintf(buf, "%2d", item->star_piece_price);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 12:
                            default:
                                sprintf(buf, "%2d", item->sell_price);
                                xoff = 0;
                                sc = 1.0f;
                                break;
                        }
                        FontDrawStart();
                        FontDrawScale(sc);
                        FontDrawColor(color);
                        FontDrawString(xoff + 226, wp->scroll + (80 - row * 24), buf);
                    }
                    if (row == wp->cursor) {
                        desc = item->description;
                    }
                    row++;
                }
            }
            GXSetScissor(sl, st, sw, sh);
            if (desc) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(desc));
            }
            break;
        }
        case 17:
        case 18: {
            InformantWork* jdt = &_jdt;
            InformantInfo* info;
            u8* tbl;
            const char* title;
            s32 h;
            s32 n;
            u32 sl, st, sw, sh;

            n = 8 - wp->count;
            h = (n > 0 ? n : 0) * 24;
            if (mode == 17) {
                title = msgSearch("msg_window_title_5");
                tbl = jdt->readFlags.perfectRead;
            } else {
                title = msgSearch("msg_window_title_6");
                tbl = jdt->readFlags.luigiRead;
            }
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(71.0f, 111.0f, title);
            FontDrawEdgeOff();
            if (wp->mode == 17) {
                Mtx mtx;
                Mtx scale;
                PSMTXTrans(mtx, 244.0f, 82.0f, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, ICON_FLAG_2D, ICON_CURRENCY_PLATE, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                {
                    iconDispGx(&(Vec){244.0f, 86.0f, 0.0f}, ICON_FLAG_2D, ICON_ITEM_COIN, 0.6f);
                }
            }
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0, info = jdt->info; i < wp->count; i++, info++) {
                char buf[32];
                GXColor color;
                char c;
                if (johoya_get(tbl, info->storyId)) {
                    sprintf(buf, "");
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                } else {
                    sprintf(buf, "%3d", info->cost);
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                }
                FontDrawStart();
                c = info->initEndPos[info->tagEndPos];
                info->initEndPos[info->tagEndPos] = 0;
                FontDrawColor(color);
                FontDrawScale(0.8f);
                FontDrawString(1.0f, wp->scroll + (80 - i * 24), info->initEndPos);
                info->initEndPos[info->tagEndPos] = c;
                if (wp->mode == 17) {
                    FontDrawStart();
                    FontDrawColor(color);
                    FontDrawString(221.0f, wp->scroll + (80 - i * 24), buf);
                }
            }
            GXSetScissor(sl, st, sw, sh);
            break;
        }
        case 5:
        case 6:
        case 7: {
            char msgName[64] = {0};
            s32 lv;
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;
            GXColor color;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(41.0f, 111.0f, msgSearch("msg_window_title_7"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_8"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; party_id_table[i]; i++) {
                const char* name;
                s32 ty;
                char buf[32];
                char help[16];
                char dir[16];
                if (wp->mode == 5 || wp->mode == 6) {
                    if (!partyChkJoin(party_id_table[i])) {
                        continue;
                    }
                    if (evtGetValue(NULL, GSWF_GOR_UP_ARROW_USED) == 0) {
                        if (pouch->partyData[party_id_table[i]].techLevel < 1) {
                            color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                            lv = 0;
                            flag = 0;
                        } else {
                            color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                            lv = 1;
                            flag = ICON_FLAG_GRAY;
                        }
                    } else {
                        if (pouch->partyData[party_id_table[i]].techLevel < 2) {
                            color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                            lv = 0;
                            flag = 0;
                        } else {
                            color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                            lv = 2;
                            flag = ICON_FLAG_GRAY;
                        }
                    }
                } else {
                    if (!partyChkJoin(party_id_table[i])) {
                        continue;
                    }
                    if (wp->skipEntry == party_id_table[i]) {
                        continue;
                    }
                    if (pouch->partyData[party_id_table[i]].techLevel > 0) {
                        color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                        lv = 0;
                        flag = 0;
                    } else {
                        color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                        lv = 1;
                        flag = ICON_FLAG_GRAY;
                    }
                }
                ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | ICON_FLAG_2D, 0.5f, unitwin_party_icon(party_icon_table[i]));
                FontDrawStart();
                FontDrawColor(color);
                name = unitwin_party_name(i);
                ty = 80 - row * 24;
                FontDrawString(36.0f, wp->scroll + ty, name);
                sprintf(buf, "%d", pouch->partyData[party_id_table[i]].techLevel);
                FontDrawStart();
                FontDrawString(236.0f, wp->scroll + ty, buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    if (wp->mode == 5 || wp->mode == 6) {
                        sprintf(dir, "lankup");
                    } else {
                        sprintf(dir, "lankdown");
                    }
                    sprintf(msgName, "%s_%s_lank%d%s", help, dir, pouch->partyData[party_id_table[i]].techLevel + 1, party_labelname_table[i]);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 8: {
            char msgName[64] = {0};
            s32 h;
            u32 sl, st, sw, sh;

            s32 j;
            s32 flag;
            s32 lv;
            s32 bob;
            s32 row = 0;
            s32 v;
            GXColor color;
            s32 icon;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < wp->count; i++) {
                char buf[32];
                char help[16];
                flag = 0;
                for (j = 0; j < wp->count; j++) {
                    if (j != wp->cursor && *mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                        flag = 1;
                    }
                }
                if (!flag) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    flag = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    flag = ICON_FLAG_GRAY;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | ICON_FLAG_2D, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                bob = (gp->retraceCount & 0xF) > 7 ? 0 : 2;
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_levelup%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] + mario_status_henka_table[i] * 2;
                    if (v > 0) {
                        icon = ICON_ARROW_UP;
                    } else {
                        icon = ICON_UNAVAILABLE;
                        bob = 0;
                    }
                    {
                        ICONGX2(&(Vec){166.0f, bob + (wp->scroll + (56 - row * 24)), 0.0f}, flag | ICON_FLAG_2D, 0.5f, icon);
                    }
                    {
                        ICONGX2(&(Vec){186.0f, bob + (wp->scroll + (56 - row * 24)), 0.0f}, flag | ICON_FLAG_2D, 0.5f, icon);
                    }
                } else {
                    v = *mario_status_point_table[i] - mario_status_henka_table[i];
                    if (v > 0) {
                        icon = ICON_ARROW_DOWN;
                    } else {
                        icon = ICON_UNAVAILABLE;
                        bob = 0;
                    }
                    {
                        ICONGX2(&(Vec){176.0f, (wp->scroll + (56 - row * 24)) - bob, 0.0f}, flag | ICON_FLAG_2D, 0.5f, icon);
                    }
                }
                if (v > 0) {
                    sprintf(buf, "%3d", v);
                } else {
                    sprintf(buf, "  ？", v);
                }
                FontDrawStart();
                FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 9: {
            char msgName[64] = {0};
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;

            s32 j;
            s32 cnt;
            s32 v;
            GXColor color;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < wp->count; i++) {
                char buf[32];
                char help[16];
                cnt = 0;
                for (j = 0; j < wp->count; j++) {
                    if (j != wp->cursor && *mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                        cnt++;
                    }
                }
                if (cnt < 2) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    j = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    j = ICON_FLAG_GRAY;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, j | ICON_FLAG_2D, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_levelup%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] + mario_status_henka_table[i];
                    if (v > 0) {
                        sprintf(buf, "%3d", v);
                    } else {
                        sprintf(buf, "  ？", v);
                    }
                    FontDrawStart();
                    FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 10: {
            char msgName[64] = {0};
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;

            s32 v;
            s32 flag;
            GXColor color;
            s32 lv;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < 3; i++) {
                char buf[32];
                char help[16];
                if (i == wp->skipEntry) {
                    continue;
                }
                if (*mario_status_point_table[i] - mario_status_henka_table[i] > 0) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    flag = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    flag = ICON_FLAG_GRAY;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | ICON_FLAG_2D, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_leveldown%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] - mario_status_henka_table[i];
                    if (v > 0) {
                        sprintf(buf, "%3d", v);
                    } else {
                        sprintf(buf, "  ？", v);
                    }
                    FontDrawStart();
                    FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
    }

    if (wp->mode != 0 && wp->mode != 1) {
        char text[136];
        char* line1;
        char* line2 = NULL;
        s32 k;
        s32 lines;
        s32 w2;

        switch (wp->mode) {
            case 0:
            case 1:
                break;
            case 11:
            case 14:
            case 15:
                strcpy(text, msgSearch("msg_window_select_6"));
                break;
            case 17:
                strcpy(text, msgSearch("msg_window_select_8"));
                break;
            case 18:
                strcpy(text, msgSearch("msg_window_select_7"));
                break;
            case 13:
            case 16:
                strcpy(text, msgSearch("msg_window_select_10"));
                break;
            case 2:
            case 12:
                strcpy(text, msgSearch("msg_window_select_2"));
                break;
            case 3:
                strcpy(text, msgSearch("msg_window_select_3"));
                break;
            case 4:
                strcpy(text, msgSearch("msg_window_select_4"));
                break;
            case 5:
                strcpy(text, msgSearch("msg_window_select_11"));
                break;
            case 6:
                strcpy(text, msgSearch("msg_window_select_11"));
                break;
            case 7:
                strcpy(text, msgSearch("msg_window_select_12"));
                break;
            case 8:
            case 9:
                strcpy(text, msgSearch("msg_window_select_9"));
                break;
            case 10:
                strcpy(text, msgSearch("msg_window_select_9_1"));
                break;
        }
        line1 = text;
        lines = 1;
        for (k = 0; ((s8*)line1)[k]; k++) {
            if (((s8*)line1)[k] == '\n') {
                text[k] = 0;
                lines = 2;
                line2 = &text[k] + 1;
                break;
            }
        }
        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -280.0f, 100.0f, 240.0f, (lines - 1) * 24 + 45, 20.0f);
        FontDrawStart();
        k = line1 ? FontGetMessageWidth(line1) : 0;
        if (line2) {
            w2 = FontGetMessageWidth(line2);
        } else {
            w2 = 0;
        }
        if (w2 > k) {
            k = w2;
        }
        if (k <= 228) {
            k = (228 - k) / 2;
        } else {
            FontDrawScale(228.0f / k);
            k = 0;
        }
        FontDrawString(k - 274, 90.0f, line1);
        if (line2) {
            FontDrawString(k - 274, 66.0f, line2);
        }
        iconFlagOff("yubicursor_2", ICON_FLAG_HIDDEN);
        iconSetPos("yubicursor_2", wp->cursorX, wp->cursorY, 0.0f);
    } else {
        iconFlagOn("yubicursor_2", ICON_FLAG_HIDDEN);
    }
}

/**
 * Replaces the item at the selected counter slot (args[0] is the unit window work) with item args[1],
 * swapping its icon, or removes the icon when args[1] is 0.
 */
USER_FUNC(set_new_goods_list) {
    EvtWord* args = event->args;
    UnitWinWork* wp = (UnitWinWork*)evtGetValue(event, args[0]);
    s32 item = evtGetValue(event, args[1]);
    char name[32];
    wp->goods[wp->goodsIdx] = item;
    if (wp->goodsIdx < wp->goodsMax) {
        sprintf(name, "uwn%d", wp->goodsIdx);
        if (item != 0) {
            iconChange(name, itemDataTable[item].icon_id);
        } else {
            iconDelete(name);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Returns the unit window work pointer in args[0],
 * for scripts that pass it to set_new_goods_list or unitwin_set_bought.
 */
USER_FUNC(unitwin_get_work_ptr) {
    evtSetValue(event, event->args[0], (EvtWord)_wp);
    return EVT_RETURN_DONE;
}

/** Returns the rank (0-2) of partner args[0] in args[1]. */
USER_FUNC(unitwin_getpartylank) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], pouch->partyData[idx].techLevel);
    return EVT_RETURN_DONE;
}

/**
 * Sets partner args[0] to rank args[1] (HP, attack and tech levels together),
 * recomputes its stats and refills its HP.
 * Used by Merlon's partner rank-up (gor_02)
 * and by Chet Rippo's Adjustment House in the Rogueport Sewers (tik_11), which lowers ranks.
 */
USER_FUNC(unitwin_setpartylank) {
    PouchPartyData* party;
    u32 ofs;
    s16 v;
    s32 ofs2;
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    ofs = evtGetValue(event, args[0]) * sizeof(PouchPartyData); // byte offset of the partner's partyData entry
    v = evtGetValue(event, args[1]);
    ofs2 = ofs; // FAKE: signed copy for the first stores, so pouch + ofs is added again after pouchRevisePartyParam as in the target
    party = (PouchPartyData*)((u8*)pouch->partyData + ofs2);
    party->HPLevel = party->attackLevel = party->techLevel = v;
    pouchRevisePartyParam();
    party = (PouchPartyData*)((u8*)pouch->partyData + ofs);
    party->currentHP = party->maximumHP;
    return EVT_RETURN_DONE;
}

/**
 * Sets Mario's base HP, FP and BP to args[0..2], refills HP and FP,
 * and if the BP no longer covers the equipped badges takes them all off; args[3] returns 1 when that happened.
 * Used by Chet Rippo's Adjustment House (tik_11), which boosts one of Mario's stats by lowering another.
 */
USER_FUNC(unitwin_setmariolevel) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 i;
    BOOL unequipped;
    pouch->baseMaxHP = evtGetValue(event, args[0]);
    pouch->baseMaxFP = evtGetValue(event, args[1]);
    pouch->maximumBP = evtGetValue(event, args[2]);
    pouchReviseMarioParam();
    pouch->currentHP = pouch->maximumHP;
    pouch->currentFP = pouch->maximumFP;
    if (pouch->availableBP < 0) {
        for (i = 0; i < MAX_BADGES; i++) {
            if (pouchEquipCheckBadgeIndex(i)) {
                pouchUnEquipBadgeIndex(i);
            }
        }
        pouchReviseMarioParam();
        unequipped = TRUE;
    } else {
        unequipped = FALSE;
    }
    evtSetValue(event, args[3], unequipped);
    return EVT_RETURN_DONE;
}

