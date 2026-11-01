// Script commands for system windows: item and party selection lists, name entry and
// single-message windows.

#include "version.h"
#include "evt/evt_cmd.h"
#include "driver/icondrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "memory.h"
#include "system.h"
#include "win/win_main.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "nameent.h"
#include "statuswindow.h"
#include "icon_ids.h"
#if VERSION <= VERSION_DEMO_US
#include "mario/mariost.h"
#endif

typedef struct CoinWin {
    s32 id;    // 0x0
    s32 coins; // 0x4
    s32 timer; // 0x8
    s32 type;  // 0xC
} CoinWin;

typedef struct OneMsgWin {
    s32 state; // 0x0
    s32 type;  // 0x4
    s32 id;    // 0x8
} OneMsgWin;

#if VERSION > VERSION_DEMO_US
s32 coinwin_se_wait;
#endif
s32 unk_80418b3c;
static void coin_disp(WinMgrEntry* entry);
static void oneMessageDisp(WinMgrEntry* entry);

/**
 * The pale yellow one-line message window near the top of the screen (the new mail notice)
 * that evt_win_one_message opens.
 */
WinMgrDesc one_msg_desc = {
    0, 0, 8, -175, 120, 350, 42, {0xD7, 0xD0, 0x88, 0xFF}, NULL, oneMessageDisp
};
/**
 * The small counter window for coins, Shine Sprites or Star Pieces that shops show while buying;
 * see evt_win_coin_on.
 */
WinMgrDesc coin_desc = {
    2, 0, 8, -250, -75, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, coin_disp
};

/**
 * Retail: args[0] flag, args[1] select window type;
 * args[2] gets the chosen entry (-1 if cancelled) and, when the flag is set, args[3] the cursor slot.
 * Mario's controls are locked while the window is open.
 */
USER_FUNC(evt_win_item_select) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_US
    s32 type = evtGetValue(event, args[0]);
#else
    s32 getCursor = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
#endif
    WinMgrSelect* sel;
    s32 result;
#if VERSION > VERSION_DEMO_US
    s32 cursor;
#endif
    if (isFirstCall) {
        event->userdatap[0] = winMgrSelectEntry(type, 0, 1);
        marioKeyOff();
    }
    sel = event->userdatap[0];
    result = winMgrSelect(sel);
    if (result == 0) {
        return EVT_RETURN_BLOCK;
    }
#if VERSION > VERSION_DEMO_US
    cursor = winMgrSelectGetCursor(sel);
#endif
    winMgrSelectDelete(sel);
    marioKeyOn();
#if VERSION <= VERSION_DEMO_US
    evtSetValue(event, args[1], result);
#else
    evtSetValue(event, args[2], result);
    if (getCursor != 0) {
        evtSetValue(event, args[3], cursor);
    }
#endif
    return EVT_RETURN_DONE;
}

/**
 * Opens select window type args[0] for partners, stat upgrades or shop lists and waits;
 * winMgrSelectOther writes the choice into LW(1..7) (id, name, price, ...).
 * LW(0) gets 1 if something was chosen, 0 if cancelled.
 */
USER_FUNC(evt_win_other_select) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    WinMgrSelect* sel;
    s32 result;
    if (isFirstCall) {
        event->userdatap[0] = winMgrSelectEntry(type, 0, 1);
        marioKeyOff();
    }
    sel = event->userdatap[0];
    result = winMgrSelectOther(sel, event);
    if (result == 0) {
        return EVT_RETURN_BLOCK;
    }
    winMgrSelectDelete(sel);
    marioKeyOn();
    if (result == -1) {
        event->lwData[0] = 0;
    } else {
        event->lwData[0] = 1;
    }
    return EVT_RETURN_DONE;
}

/**
 * Filters the -1-terminated item list args[0] down to the items Mario carries;
 * args[1] gets the new list (allocated from map memory, free it with evt_win_item_desttable) or 0
 * if he has none.
 */
USER_FUNC(evt_win_item_maketable) {
    s32* dst;
    s32* item;
    s32* list;
    EvtWord* args = event->args;
    s32* table;
    s32 count;
    s32 i;
    s32 n;
    list = (s32*)evtGetValue(event, args[0]);
    count = 0;
    item = list;
    for (;;) {
        if (*item == -1) {
            break;
        }
        item++;
        count++;
    }
    if (count == 0) {
        evtSetValue(event, args[1], 0);
        return EVT_RETURN_DONE;
    }
    table = _mapAlloc((count + 1) * 4);
    item = list;
    i = 0;
    n = 0;
    dst = table;
    for (; i < count; i++) {
        s32 has = pouchCheckItem(*item);
        if (has) {
            *dst++ = *item;
            n++;
        }
        item++;
    }
    table[n] = -1;
    if (n == 0) {
        table = NULL;
    }
    evtSetValue(event, args[1], (EvtWord)table);
    return EVT_RETURN_DONE;
}

/** Frees an item list made by evt_win_item_maketable (args[0]). */
USER_FUNC(evt_win_item_desttable) {
    EvtWord* args = event->args;
#if VERSION >= VERSION_US
    void* p = (void*)evtGetValue(event, args[0]);
    _mapFree(p);
#else
    _mapFree((void*)evtGetValue(event, args[0]));
#endif
    return EVT_RETURN_DONE;
}

/**
 * Opens the name entry (on-screen keyboard) screen of type args[0], waiting until it has loaded;
 * the X-Naut Fortress map aji_01 uses type 4.
 */
USER_FUNC(evt_win_nameent_on) {
    s32 type = evtGetValue(event, event->args[0]);
    if (!nameEntPrepare()) {
        return EVT_RETURN_BLOCK;
    }
    nameEntOn(type);
    return EVT_RETURN_DONE;
}

/** Blocks until the player has finished the name entry screen. */
USER_FUNC(evt_win_nameent_wait) {
    return nameEntWait() != 0 ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** Returns a pointer to the name typed on the name entry screen in args[0]. */
USER_FUNC(evt_win_nameent_name) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (EvtWord)nameEntGetName());
    return EVT_RETURN_DONE;
}

/** Closes the name entry screen. */
USER_FUNC(evt_win_nameent_off) {
    nameEntOff();
    return EVT_RETURN_DONE;
}

/** Draws the one-line message window's text in red; type 0 is "You got mail!" (msg_you_gotta_mail). */
static void oneMessageDisp(WinMgrEntry* entry) {
    OneMsgWin* w = entry->param;
    const char* msg;
    if (winMgrAction(w->id) == 0) {
        switch (w->type) {
        case 0:
            msg = msgSearch("msg_you_gotta_mail");
            break;
        }
        FontDrawStart();
        {
            GXColor color = {0xFF, 0x00, 0x00, 0xFF};
            FontDrawColor(color);
        }
        FontDrawMessage(-(s32)FontGetMessageWidth(msg) / 2, entry->y - 8, msg);
    }
}

/**
 * Shows a one-line message window of type args[0] until the player presses A, then closes it.
 * Type 0 is the new mail notice from evt_sub.c, which plays the mail jingle on A.
 */
USER_FUNC(evt_win_one_message) {
    s32 type = evtGetValue(event, event->args[0]);
    OneMsgWin* w;
    if (isFirstCall) {
        OneMsgWin* nw;
        nw = event->userdatap[0] = _mapAlloc(sizeof(OneMsgWin));
        memset(nw, 0, sizeof(OneMsgWin));
        nw->id = winMgrEntry(&one_msg_desc);
        nw->state = 0;
        nw->type = type;
        winMgrSetParam(nw->id, nw);
    }
    w = event->userdatap[0];
    switch (w->state) {
    case 0:
        winMgrOpen(w->id);
        w->state++;
        break;
    case 1:
        if (winMgrAction(w->id) == 0) {
            w->state++;
        }
        break;
    case 2:
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            switch (w->type) {
            case 0:
                psndSFXOn_3D("SFX_MAIL_RECEPTION1", &marioGetPtr()->position);
                break;
            }
            w->state++;
        }
        break;
    case 3:
        winMgrClose(w->id);
        w->state++;
        break;
    case 4:
        if (winMgrAction(w->id) == 0) {
            w->state++;
        }
        break;
    case 5:
        winMgrDelete(w->id);
        _mapFree(w);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

#define COIN_WINDOW_COINS 0
#define COIN_WINDOW_SHINE_SPRITES 1
#define COIN_WINDOW_STAR_PIECES 2

/**
 * Draws the coin counter window (type 0 coins, 1 Shine Sprites, 2 Star Pieces)
 * and rolls the shown number toward the pouch value,
 * playing the coin count sound and keeping the status bar in step.
 */
static void coin_disp(WinMgrEntry* entry) {
    CoinWin* w;
    s32 target;
    s16 icon;
    Mtx pos;
    Mtx scale;
    s32 diff;
    pouchGetPtr();
    w = entry->param;
    if (entry->flags & WINMGR_FLAG_ANIMATING) {
        return;
    }
    switch (w->type) {
    case COIN_WINDOW_COINS:
        target = pouchGetCoin();
#if VERSION <= VERSION_DEMO_US
        icon = 0x18B;
#else
        icon = ICON_ITEM_COIN;
#endif
        break;
    case COIN_WINDOW_SHINE_SPRITES:
#if VERSION <= VERSION_DEMO_US
        target = pouchGetSuperCoin();
        icon = 0x102;
#else
        target = pouchGetSuperCoin();
        icon = ICON_ITEM_SHINE_SPRITE;
#endif
        break;
    case COIN_WINDOW_STAR_PIECES:
#if VERSION <= VERSION_DEMO_US
        target = pouchGetStarPiece();
        icon = 0x18D;
#else
        target = pouchGetStarPiece();
        icon = ICON_ITEM_STAR_PIECE;
#endif
        break;
    }
    PSMTXTrans(pos, -220.0f, -113.0f, 0.0f);
    PSMTXScale(scale, 0.7f, 0.7f, 0.7f);
    PSMTXConcat(pos, scale, pos);
    iconDispGx2(pos, ICON_FLAG_2D, icon);
    PSMTXTrans(pos, -192.0f, -113.0f, 0.0f);
#if VERSION <= VERSION_DEMO_US
    iconDispGx2(pos, ICON_FLAG_2D, 0x1D6);
#else
    iconDispGx2(pos, ICON_FLAG_2D, ICON_TIMES);
#endif
    PSMTXTrans(pos, -126.0f, -113.0f, 0.0f);
    {
        GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
        iconNumberDispGx(pos, w->coins, 0, white);
    }
    diff = target - w->coins;
    if (diff != 0) {
        diff = diff / 10;
        if (diff == 0) {
            diff = target > w->coins ? 1 : -1;
        }
        if (w->timer & 1) {
            w->coins += diff;
            statusWinForceUpdateCoin();
        }
        w->timer++;
        w->timer = w->timer % 1200;
#if VERSION <= VERSION_DEMO_US
        if (__abs(target - w->coins) == 1 || gp->retraceCount % 6 == 0) {
            psndSFXOn("SFX_COUNT_COIN1");
#else
        if (coinwin_se_wait == 0) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_COUNT_COIN1 | PSND_SFX_BYPASS_MUTE));
            coinwin_se_wait = 7;
#endif
        }
    }
#if VERSION > VERSION_DEMO_US
    if (coinwin_se_wait > 0) {
        coinwin_se_wait--;
    }
#endif
}

/**
 * Opens the counter window of type args[0] (0 coins, 1 Shine Sprites, 2 Star Pieces) at the top left,
 * as shops do while buying; args[1] gets the window handle for evt_win_coin_off/wait.
 */
USER_FUNC(evt_win_coin_on) {
    EvtWord* args = event->args;
    CoinWin* w = _mapAlloc(sizeof(CoinWin));
    s32 type = evtGetValue(event, args[0]);
    w->type = type;
    w->id = winMgrEntry(&coin_desc);
    w->timer = 0;
    switch (type) {
    case COIN_WINDOW_COINS:
        w->coins = pouchGetCoin();
        break;
    case COIN_WINDOW_SHINE_SPRITES:
        w->coins = pouchGetSuperCoin();
        break;
    case COIN_WINDOW_STAR_PIECES:
        w->coins = pouchGetStarPiece();
        break;
    }
    winMgrSetParam(w->id, w);
    winMgrOpen(w->id);
    evtSetValue(event, args[1], (EvtWord)w);
    return EVT_RETURN_DONE;
}

/** Waits until counter window args[0] has finished rolling to the pouch value, then closes and frees it. */
USER_FUNC(evt_win_coin_off) {
    CoinWin* w = (CoinWin*)evtGetValue(event, event->args[0]);
    s32 coins;
    switch (w->type) {
    case COIN_WINDOW_COINS:
        coins = pouchGetCoin();
        break;
    case COIN_WINDOW_SHINE_SPRITES:
        coins = pouchGetSuperCoin();
        break;
    case COIN_WINDOW_STAR_PIECES:
        coins = pouchGetStarPiece();
        break;
    }
    if (w->coins != coins) {
        return EVT_RETURN_BLOCK;
    }
    winMgrCloseAutoDelete(w->id);
    _mapFree(w);
    return EVT_RETURN_DONE;
}

/** Blocks until counter window args[0] has finished rolling to the pouch value. */
USER_FUNC(evt_win_coin_wait) {
    CoinWin* w = (CoinWin*)evtGetValue(event, event->args[0]);
    s32 coins;
    switch (w->type) {
    case COIN_WINDOW_COINS:
        coins = pouchGetCoin();
        break;
    case COIN_WINDOW_SHINE_SPRITES:
        coins = pouchGetSuperCoin();
        break;
    case COIN_WINDOW_STAR_PIECES:
        coins = pouchGetStarPiece();
        break;
    }
    return w->coins != coins ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** Allows the pause menu to be opened again. */
USER_FUNC(mainwindow_openenable) {
    winOpenEnable();
    return EVT_RETURN_DONE;
}

/** Stops the player from opening the pause menu (for cutscenes). */
USER_FUNC(mainwindow_opendisable) {
    winOpenDisable();
    return EVT_RETURN_DONE;
}
