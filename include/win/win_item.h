// win_item.c: the pause menu item window (items, important items, mail, Super Luigi books): winItemInit/Main/Disp.

#pragma once

#include "dolphin/types.h"

struct WindowWork;

/* WindowWork item.state: winItemMain's state machine (win_root.c's help box and mail list test WIN_ITEM_MAIL_READ) */
typedef enum WinItemMainState {
    WIN_ITEM_TABS = 0,         // pick the Items / Important Items tab; A enters its list, Z sorts, B back to the tabs
    WIN_ITEM_LIST = 10,        // move through the list; A uses a field item, opens a Super Luigi book or the
                               // Mailbox SP, or reads the Ragged Diary (closes the menu); Z sorts; B back to the tabs
    WIN_ITEM_BOOK = 100,       // page through a Super Luigi book with A / B; past either end back to WIN_ITEM_LIST
    WIN_ITEM_MAIL_LIST = 200,  // the mail list (Mailbox SP); A opens a letter (WIN_ITEM_MAIL_LOAD), B back to the list
    WIN_ITEM_MAIL_LOAD = 201,  // load mail.tpl if the letter has a picture, then on to WIN_ITEM_MAIL_READ
    WIN_ITEM_MAIL_READ = 202,  // read the letter (C stick scrolls); B back to WIN_ITEM_MAIL_LIST
    WIN_ITEM_USE_TARGET = 300, // pick who gets the item (the target window); A on Mario or the partner out with him
                               // closes the menu to use it in the field (not while Mario is a plane or boat, nor
                               // from JP on in the roll or slit motions), else the menu applies it (WIN_ITEM_USE_BLINK)
    WIN_ITEM_USE_BLINK = 301,  // the target blinks for 2 seconds, then the item is used up; back to WIN_ITEM_LIST
                               // (WIN_ITEM_TABS when no items are left)
    WIN_ITEM_SORT = 1000,      // the Z sort menu is up; back to savedState when it closes
} WinItemMainState;

extern const char** luigi_book_list[];
extern u16 menu_skip_list[];
void winItemDisp(s32 camId, void* param, s32 n);
void winItemExit(struct WindowWork* w);
void winItemInit(struct WindowWork* w);
void winItemInit2(struct WindowWork* w);
s32 winItemMain(struct WindowWork* w);
void winItemMain2(struct WindowWork* w);
void winMakeSkipList(struct WindowWork* w);
