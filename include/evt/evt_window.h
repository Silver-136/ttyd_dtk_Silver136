// evt_window.c: script commands for system windows: item and party selection, name entry and one-message windows.
#pragma once

#include "evt/evt_cmd.h"
#include "manager/winmgr.h"

// evt_win_coin_on args[0]: which pouch counter the window shows (coin_disp).
typedef enum CoinWindowType {
    COIN_WINDOW_COINS = 0,         // pouchGetCoin
    COIN_WINDOW_SHINE_SPRITES = 1, // pouchGetSuperCoin
    COIN_WINDOW_STAR_PIECES = 2,   // pouchGetStarPiece
} CoinWindowType;

USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_item_desttable);
USER_FUNC(evt_win_item_maketable);
USER_FUNC(evt_win_item_select);
USER_FUNC(evt_win_nameent_name);
USER_FUNC(evt_win_nameent_off);
USER_FUNC(evt_win_nameent_on);
USER_FUNC(evt_win_nameent_wait);
USER_FUNC(evt_win_one_message);
USER_FUNC(evt_win_other_select);
USER_FUNC(mainwindow_opendisable);
USER_FUNC(mainwindow_openenable);
