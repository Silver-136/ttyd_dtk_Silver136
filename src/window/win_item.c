// Pause menu: the item window (items, important items, mail and the Super Luigi books).

#include "version.h"
#include "evt/evt_story.h"
#include "win/win_main.h"
#include "win/win_party.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "data/item_data.h"
#include <dolphin/gx.h>
#include "mario/mariost.h"
#include "driver/swdrv.h"
#include "manager/filemgr.h"
#include "data/mail_data.h"
#include "icon_ids.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "mot/mot_roll.h"
#include "mot/mot_slit.h"
#include "win/win_item.h"
#include "evt/evt_cmd.h"
#include "evt/evt_save_flags.h"

static void itemUseDisp(WinMgrEntry* entry);
static void itemUseDisp2(WinMgrEntry* entry);
static void item_disp(WindowWork* w, f32 xo, f32 yo);

/** Pages of Super Luigi volume 1 for the book reader in the item menu, ending with NULL. */
static const char* luigi_book_1[] = {
    "msg_luigi_book1_txt1",
    "msg_luigi_book1_txt2",
    "msg_luigi_book1_txt3",
    "msg_luigi_book1_txt4",
    "msg_luigi_book1_txt5",
    "msg_luigi_book1_txt6",
    "msg_luigi_book1_txt7",
    "msg_luigi_book1_txt8",
    "msg_luigi_book1_txt9",
    "msg_luigi_book1_txt10",
    "msg_luigi_book1_txt11",
    "msg_luigi_book1_txt12",
    NULL,
};

/** Pages of Super Luigi volume 2 ("Allies in Adventure"). */
static const char* luigi_book_2[] = {
    "msg_luigi_book2_txt1",
    "msg_luigi_book2_txt2",
    "msg_luigi_book2_txt3",
    "msg_luigi_book2_txt4",
    "msg_luigi_book2_txt5",
    "msg_luigi_book2_txt6",
    "msg_luigi_book2_txt7",
    "msg_luigi_book2_txt8",
    "msg_luigi_book2_txt9",
    "msg_luigi_book2_txt10",
    "msg_luigi_book2_txt11",
    "msg_luigi_book2_txt12",
    NULL,
};

/** Pages of Super Luigi volume 3 ("The Voice of a Princess"). */
static const char* luigi_book_3[] = {
    "msg_luigi_book3_txt1",
    "msg_luigi_book3_txt2",
    "msg_luigi_book3_txt3",
    "msg_luigi_book3_txt4",
    "msg_luigi_book3_txt5",
    "msg_luigi_book3_txt6",
    "msg_luigi_book3_txt7",
    "msg_luigi_book3_txt8",
    "msg_luigi_book3_txt9",
    "msg_luigi_book3_txt10",
    "msg_luigi_book3_txt11",
    "msg_luigi_book3_txt12",
    NULL,
};

/** Pages of Super Luigi volume 4 ("The Shards of Truth"). */
static const char* luigi_book_4[] = {
    "msg_luigi_book4_txt1",
    "msg_luigi_book4_txt2",
    "msg_luigi_book4_txt3",
    "msg_luigi_book4_txt4",
    "msg_luigi_book4_txt5",
    "msg_luigi_book4_txt6",
    "msg_luigi_book4_txt7",
    "msg_luigi_book4_txt8",
    "msg_luigi_book4_txt9",
    "msg_luigi_book4_txt10",
    "msg_luigi_book4_txt11",
    "msg_luigi_book4_txt12",
    NULL,
};

/** Pages of Super Luigi volume 5 ("Journey's End"). */
static const char* luigi_book_5[] = {
    "msg_luigi_book5_txt1",
    "msg_luigi_book5_txt2",
    "msg_luigi_book5_txt3",
    "msg_luigi_book5_txt4",
    "msg_luigi_book5_txt5",
    "msg_luigi_book5_txt6",
    "msg_luigi_book5_txt7",
    "msg_luigi_book5_txt8",
    "msg_luigi_book5_txt9",
    "msg_luigi_book5_txt10",
    "msg_luigi_book5_txt11",
    "msg_luigi_book5_txt12",
    NULL,
};

/**
 * The five Super Luigi books, indexed by item id minus ITEM_SUPER_LUIGI1;
 * using one in the item menu opens the book reader.
 */
const char** luigi_book_list[] = {luigi_book_1, luigi_book_2, luigi_book_3, luigi_book_4, luigi_book_5};

#if VERSION <= VERSION_DEMO_JP
u16 menu_skip_list[] = {ITEM_STRANGE_SACK, ITEM_HAMMER, ITEM_SUPER_HAMMER, ITEM_ULTRA_HAMMER, ITEM_BOOTS, ITEM_SUPER_BOOTS, ITEM_ULTRA_BOOTS, ITEM_BOAT_CURSE, ITEM_TUBE_CURSE, ITEM_PAPER_CURSE, ITEM_PLANE_CURSE, ITEM_MAGICAL_MAP, ITEM_DIAMOND_STAR, ITEM_EMERALD_STAR, ITEM_GOLD_STAR, ITEM_RUBY_STAR, ITEM_SAPPHIRE_STAR, ITEM_GARNET_STAR, ITEM_CRYSTAL_STAR};
#else
u16 menu_skip_list[] = {ITEM_HAMMER, ITEM_SUPER_HAMMER, ITEM_ULTRA_HAMMER, ITEM_BOOTS, ITEM_SUPER_BOOTS, ITEM_ULTRA_BOOTS, ITEM_BOAT_CURSE, ITEM_TUBE_CURSE, ITEM_PAPER_CURSE, ITEM_PLANE_CURSE, ITEM_MAGICAL_MAP, ITEM_DIAMOND_STAR, ITEM_EMERALD_STAR, ITEM_GOLD_STAR, ITEM_RUBY_STAR, ITEM_SAPPHIRE_STAR, ITEM_GARNET_STAR, ITEM_CRYSTAL_STAR};
#endif

/**
 * Windows opened when an item is used from the menu: the target list of Mario and his partners, and the
 * small orange "Use it on whom?" title above it.
 */
static WinMgrDesc window_desc[2] = {
    {0, 0, 8, -70, 150, 350, 264, {0xF0, 0xF0, 0xF0, 0xFF}, 0, itemUseDisp},
    {2, 0, 8, -280, 145, 200, 50, {0xFF, 0x99, 0x00, 0xFF}, 0, itemUseDisp2},
};

#if VERSION >= VERSION_US
#define WI_SLASH (gp->language != LANGUAGE_JP ? "/" : "／")
#else
#define WI_SLASH "／"
#endif

/**
 * Builds the Important Items list: every key item in the pouch except those in menu_skip_list (the curses,
 * boots, hammers, Magical Map and Crystal Stars, which other menu screens show;
 * the early demos also hide the Strange Sack).
 */
void winMakeSkipList(WindowWork* w) {
    s32 i;
    u32 j;
    u16 id;
    w->item.skipCount = 0;
    for (i = 0; i < MAX_KEY_ITEMS; i++) {
        id = pouchKeyItem(i);
        if (id) {
            for (j = 0; j < sizeof(menu_skip_list) / sizeof(menu_skip_list[0]); j++) {
                if (id == menu_skip_list[j]) {
                    break;
                }
            }
            if (j >= sizeof(menu_skip_list) / sizeof(menu_skip_list[0])) {
                w->item.skip[w->item.skipCount] = id;
                w->item.skipCount++;
            }
        }
    }
}

/**
 * Resets the Items window when the pause menu opens:
 * cursors, book and mail panels off screen, gives Mario the Mailbox SP if he lacks it,
 * copies the received mail list (newest first), builds the Important Items list,
 * creates the two item-use windows and starts loading mail.tpl.
 */
void winItemInit(WindowWork* w) {
    s32 i;
    w->item.state = 0;
    w->item.mode = 0;
    w->item.cursor[0] = 0;
    w->item.page[0] = 0;
    w->item.cursor[1] = 0;
    w->item.page[1] = 0;
    w->item.bookX = w->item.bookTgtX = 320.0f;
    w->item.bookY = w->item.bookTgtY = -240.0f;
    w->item.mailX = w->item.mailTgtX = 320.0f;
    w->item.mailY = w->item.mailTgtY = -240.0f;
    if (!pouchCheckItem(ITEM_MAILBOX_SP)) {
        pouchGetItem(ITEM_MAILBOX_SP);
    }
    w->item.mailNum = pouchReceiveMailCount();
    w->item.mailSel = 0;
    w->item.mailPage = 0;
    w->item.mailScroll = w->item.mailScrollTgt = 0.0f;
    for (i = 0; i < w->item.mailNum; i++) {
        w->item.mail[i].kind = w->item.mailNum - i - 1;
        w->item.mail[i].id = pouchGetPtr()->emailIds[w->item.mailNum - i - 1];
    }
    w->item.useItem = 0;
    winMakeSkipList(w);
    w->useWin[0] = winMgrEntry(&window_desc[0]);
    w->useWin[1] = winMgrEntry(&window_desc[1]);
    winMgrSetPriority(w->useWin[0], -200);
    winMgrSetPriority(w->useWin[1], -100);
#if VERSION >= VERSION_US
    fileAsyncf(FILE_TYPE_TPL, 0, "%s/w/%s/mail.tpl", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us");
#elif VERSION <= VERSION_DEMO_JP
    fileAsyncf(FILE_TYPE_TPL, 0, "%s/window/jp/mail.tpl", getMarioStDvdRoot());
#else
    fileAsyncf(FILE_TYPE_TPL, 0, "%s/w/jp/mail.tpl", getMarioStDvdRoot());
#endif
}

/**
 * Run from win_root.c each time the Items window comes to the front:
 * snaps scrolls to the saved pages, hides the book and mail panels and sets the help text for the current tab.
 */
void winItemInit2(WindowWork* w) {
    w->helpTgtX = -275.0f;
    w->helpTgtY = (f32)(125 - w->item.mode * 40);
    w->item.scroll[0] = w->item.scrollTgt[0] = 190.0f * (f32)w->item.page[0];
    w->item.scroll[1] = w->item.scrollTgt[1] = 190.0f * (f32)w->item.page[1];
    w->item.bookX = w->item.bookTgtX = 320.0f;
    w->item.bookY = w->item.bookTgtY = -240.0f;
    w->item.mailX = w->item.mailTgtX = 320.0f;
    w->item.mailY = w->item.mailTgtY = -240.0f;
    w->item.mailScroll = w->item.mailScrollTgt = (f32)(w->item.mailSel * 200);
    winMsgEntry(w, 0, w->item.mode == 0 ? "msg_menu_mochi_item" : "msg_menu_mochi_daiji", 0);
    w->item.useItem = 0;
}

/** Frees the two item-use windows created by winItemInit. */
void winItemExit(WindowWork* w) {
    winMgrDelete(w->useWin[0]);
    winMgrDelete(w->useWin[1]);
}

/**
 * Items window input.
 * State 0 picks the Items / Important Items tab; 10 moves through the two-column list;
 * 100 pages through one of Luigi's books; 200-202 pick and read mail (Mailbox SP);
 * 300 picks who gets a field-usable item and 301 blinks while it is applied in the menu;
 * 1000 is the Z sort menu.
 * Returns -1 on B from tab selection (state 0), -2 to close the menu (also to use an item or read the Ragged Diary in the field), else 0.
 * Luigi's books are the Super Luigi key items.
 */
s32 winItemMain(WindowWork* w) {
    s32 cnt;
    s32 mode;
    s32 pages;
    s32 id;
    s32 c;
    s32 old;
    s32 ok;
    const char** book;
    WinMgrEntry* entry;
    MarioWork* mp;
    MailData* mail;
    WindowPartyDt* list[10];
    s32 party;
    s32 n;
    s32 i;
    f32 yoff;
    s32 k;

    switch (w->item.state) {
        case 0:
            if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                return -1;
            }
            if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            if (w->dirRep & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                w->item.mode = 1 - w->item.mode;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_BUTTON_A) {
                if (w->item.mode == 0 && pouchGetHaveItemCnt() == 0) {
                    break;
                }
                if (w->item.mode == 1 && w->item.skipCount == 0) {
                    break;
                }
                w->item.state = 10;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                break;
            } else if (w->buttonTrg & PAD_TRIGGER_Z) {
                winSortEntry(w, w->item.mode != 0, -310.0f, 150.0f);
                w->savedState = w->item.state;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 1000;
            }
            w->helpTgtX = -275.0f;
            w->helpTgtY = (f32)(125 - w->item.mode * VERSION_PICK(40, 50));
            winMsgEntry(w, 0, w->item.mode == 0 ? "msg_menu_mochi_item" : "msg_menu_mochi_daiji", 0);
            break;
        case 10:
            if (w->item.mode == 0) {
                cnt = pouchGetHaveItemCnt();
            } else {
                cnt = w->item.skipCount;
            }
            mode = w->item.mode;
            pages = (cnt + 9) / 10;
            if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 0;
            }
            if (w->buttonTrg & PAD_BUTTON_A) {
                if (w->item.mode == 0) {
                    id = pouchHaveItem(w->item.cursor[mode]);
                    if (itemDataTable[id].usable_locations & ITEM_USABLE_MENU) {
                        w->item.useItem = id;
                        w->item.useIdx = w->item.cursor[mode];
                        w->item.state = 300;
                        w->item.useTarget = 0;
                        w->item.blink = 0;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                        entry = winMgrGetPtr(w->useWin[0]);
                        winMgrSetSize(w->useWin[0], entry->x, entry->y, entry->width, w->party.count * 28 + 68);
                        winMgrOpen(w->useWin[0]);
                        winMgrOpen(w->useWin[1]);
                    }
                } else {
                    c = w->item.skip[w->item.cursor[mode]];
                    if ((u32)(c - ITEM_SUPER_LUIGI1) <= ITEM_SUPER_LUIGI4 - ITEM_SUPER_LUIGI1 || c == ITEM_SUPER_LUIGI5) {
                        w->item.state = 100;
                        w->item.bookTgtX = -170.0f;
                        w->item.bookTgtY = 160.0f;
#if VERSION >= VERSION_JP
                        w->flags |= WINDOW_FLAG_BOOK_OPEN;
#endif
                        book = luigi_book_list[c - ITEM_SUPER_LUIGI1];
                        w->item.bookIdx = c - ITEM_SUPER_LUIGI1;
                        w->item.bookPage = 0;
                        for (w->item.bookPages = 0; book[w->item.bookPages] != NULL; w->item.bookPages++) {
                        }
                        w->item.bookPages = (w->item.bookPages + 1) / 2;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                    } else if (c == ITEM_MAILBOX_SP && pouchReceiveMailCount()) {
                        w->item.state = 200;
                        w->item.mailTgtX = -144.0f;
                        w->item.mailTgtY = 180.0f;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                    } else if (c == ITEM_RAGGED_DIARY) {
                        w->item.useItem = c;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                        return -2;
                    }
                }
            } else if (w->buttonRep & PAD_TRIGGER_L) {
                w->item.page[mode]--;
                if (w->item.page[mode] < 0) {
                    w->item.page[mode] = 0;
                }
                w->item.cursor[mode] = w->item.page[mode] * 10;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonRep & PAD_TRIGGER_R) {
                w->item.page[mode]++;
                if (w->item.page[mode] >= pages) {
                    w->item.page[mode] = pages - 1;
                }
                w->item.cursor[mode] = w->item.page[mode] * 10;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
                if (w->item.cursor[mode] % 2) {
                    if ((n = w->item.cursor[mode] - 1) >= 0 && n < cnt) {
                        w->item.cursor[mode]--;
                    }
                } else if ((n = w->item.cursor[mode] + 1) >= 0 && n < cnt) {
                    w->item.cursor[mode]++;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_UP) {
                c = w->item.cursor[mode];
                if ((n = c - 2) >= 0 && n < cnt) {
                    w->item.cursor[mode] = c - 2;
                }
                w->item.page[mode] = w->item.cursor[mode] / 10;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                c = w->item.cursor[mode];
                if ((n = c + 2) >= 0 && n < cnt) {
                    w->item.cursor[mode] = c + 2;
                } else if (c + (c & 1) < cnt) {
                    w->item.cursor[mode] = cnt - 1;
                }
                w->item.page[mode] = w->item.cursor[mode] / 10;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_TRIGGER_Z) {
                winSortEntry(w, w->item.mode != 0, -310.0f, 150.0f);
                w->savedState = w->item.state;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 1000;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            w->helpTgtX = (f32)(w->item.cursor[mode] % 10 % 2 * 204 - 155);
            w->helpTgtY = (f32)(110 - w->item.cursor[mode] % 10 / 2 * 38);
            if (w->item.mode == 0) {
                id = pouchHaveItem(w->item.cursor[mode]);
            } else {
                id = w->item.skip[w->item.cursor[mode]];
            }
            winMsgEntry(w, id, itemDataTable[id].description, 0);
            break;
        case 100:
            if (w->buttonTrg & PAD_BUTTON_A) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->item.bookPage++;
                if (w->item.bookPage >= w->item.bookPages) {
                    w->item.bookPage = w->item.bookPages - 1;
                    w->item.state = 10;
                    w->item.bookTgtX = 320.0f;
                    w->item.bookTgtY = -240.0f;
#if VERSION <= VERSION_DEMO_JP
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
#else
                    w->flags &= ~WINDOW_FLAG_BOOK_OPEN;
                    w->helpX = w->helpTgtX;
                    w->helpY = w->helpTgtY;
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
#endif
                w->item.bookPage--;
                if (w->item.bookPage < 0) {
                    w->item.bookPage = 0;
                    w->item.state = 10;
                    w->item.bookTgtX = 320.0f;
                    w->item.bookTgtY = -240.0f;
#if VERSION >= VERSION_JP
                    w->flags &= ~WINDOW_FLAG_BOOK_OPEN;
                    w->helpX = w->helpTgtX;
                    w->helpY = w->helpTgtY;
#endif
                }
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                w->item.bookTgtX = 320.0f;
                w->item.bookTgtY = -240.0f;
#if VERSION >= VERSION_JP
                w->flags &= ~WINDOW_FLAG_BOOK_OPEN;
                w->helpX = w->helpTgtX;
                w->helpY = w->helpTgtY;
#endif
                return -2;
            }
            w->helpTgtX = -170.0f;
            w->helpTgtY = 150.0f;
            break;
        case 200:
            if (w->buttonTrg & PAD_BUTTON_A) {
                if (pouchCheckMail(w->item.mail[w->item.mailSel].id) == 1) {
                    pouchOpenMail(w->item.mail[w->item.mailSel].id);
                }
                swSet(GSWF_ID(GSWF_MAIL_TUTORIAL_REPEAT_OFF));
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 201;
                mail = mailGetPtr(w->item.mail[w->item.mailSel].id);
                FontGetMessageWidthLine(msgSearch(mail->body), &w->item.mailLines);
                w->item.mailLines++;
#if VERSION <= VERSION_DEMO_JP
                w->item.mailLines += 7;
                w->item.mailLine = 0;
                if (mail->flags & MAIL_FLAG_PICTURE) {
                    w->item.mailLines += 7;
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
#else
                w->item.mailLine = 0;
                if (mail->flags & MAIL_FLAG_PICTURE) {
                    w->item.mailLines += 10;
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
#endif
                w->item.state = 10;
                w->item.mailTgtX = 320.0f;
                w->item.mailTgtY = -240.0f;
            } else if (w->dirRep & DEMO_STICK_UP) {
                old = w->item.mailSel;
                w->item.mailSel--;
                if (w->item.mailSel < 0) {
                    w->item.mailSel = 0;
                }
                w->item.mailPage = w->item.mailSel / 5;
                w->item.mailLine = 0;
                if (old != w->item.mailSel) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                old = w->item.mailSel;
                w->item.mailSel++;
                if (w->item.mailSel >= w->item.mailNum) {
                    w->item.mailSel = w->item.mailNum - 1;
                }
                w->item.mailPage = w->item.mailSel / 5;
                w->item.mailLine = 0;
                if (old != w->item.mailSel) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonRep & PAD_TRIGGER_L) {
                old = w->item.mailPage;
                w->item.mailPage--;
                if (w->item.mailPage < 0) {
                    w->item.mailPage = 0;
                }
                w->item.mailSel = w->item.mailPage * 5;
                if (old != w->item.mailPage) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonRep & PAD_TRIGGER_R) {
                pages = (w->item.mailNum + 4) / 5;
                old = w->item.mailPage;
                w->item.mailPage++;
                if (w->item.mailPage >= pages) {
                    w->item.mailPage = pages - 1;
                }
                w->item.mailSel = w->item.mailPage * 5;
                if (old != w->item.mailPage) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonTrg & PAD_TRIGGER_Z) {
                winSortEntry(w, WIN_SORT_MAIL, w->item.mailTgtX, w->item.mailTgtY - 60.0f);
                w->savedState = w->item.state;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 1000;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                w->item.mailTgtX = 320.0f;
                w->item.mailTgtY = -240.0f;
                return -2;
            }
            w->helpTgtX = -96.0f;
            w->helpTgtY = 120.0f - (f32)(w->item.mailSel % 5 * 40);
            break;
        case 201:
            mail = mailGetPtr(w->item.mail[w->item.mailSel].id);
            if (mail->flags & MAIL_FLAG_PICTURE) {
#if VERSION <= VERSION_DEMO_JP
                w->mailRes = fileAsyncf(FILE_TYPE_TPL, 0, "%s/window/jp/mail.tpl", getMarioStDvdRoot());
                if (w->mailRes) {
                    w->mailRes = fileAllocf(FILE_TYPE_TPL, "%s/window/jp/mail.tpl", getMarioStDvdRoot());
#else
#if VERSION >= VERSION_US
                w->mailRes = fileAsyncf(FILE_TYPE_TPL, 0, "%s/w/%s/mail.tpl", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us");
#else
                w->mailRes = fileAsyncf(FILE_TYPE_TPL, 0, "%s/w/jp/mail.tpl", getMarioStDvdRoot());
#endif
                if (w->mailRes) {
#if VERSION >= VERSION_US
                    w->mailRes = fileAllocf(FILE_TYPE_TPL, "%s/w/%s/mail.tpl", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us");
#else
                    w->mailRes = fileAllocf(FILE_TYPE_TPL, "%s/w/jp/mail.tpl", getMarioStDvdRoot());
#endif
#endif
                    w->item.state++;
                }
            } else {
                w->mailRes = NULL;
                w->item.state++;
            }
            break;
        case 202:
            if (w->item.mailLines > 8) {
                if (keyGetDirRep(0) & DEMO_SUBSTICK_UP) {
                    if (w->item.mailLine > 0) {
                        w->item.mailLine--;
                    }
                } else if (keyGetDirRep(0) & DEMO_SUBSTICK_DOWN) {
#if VERSION <= VERSION_DEMO_JP
                    if (w->item.mailLine + 8 < w->item.mailLines) {
#else
                    if (w->item.mailLine + 6 < w->item.mailLines) {
#endif
                        w->item.mailLine++;
                    }
                }
            }
            if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_MAIL_MOVE1 | PSND_SFX_BYPASS_MUTE));
                if (w->mailRes) {
                    fileFree(w->mailRes);
                }
                w->item.state = 200;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                if (w->mailRes) {
                    fileFree(w->mailRes);
                }
                w->item.mailTgtX = 320.0f;
                w->item.mailTgtY = -240.0f;
                return -2;
            }
            w->helpTgtX = -96.0f;
            w->helpTgtY = 120.0f;
            break;
        case 300:
            if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                w->item.state = 10;
                w->item.useItem = 0;
                winMgrClose(w->useWin[0]);
                winMgrClose(w->useWin[1]);
            } else if (w->buttonTrg & PAD_BUTTON_A) {
                mp = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                if (mp->currMotionId != MARIO_MOTION_SHIP && mp->currMotionId != MARIO_MOTION_PLANE &&
                    ((w->item.useTarget < 2 && !marioBgmodeChk()) || (w->item.useTarget < 1 && marioBgmodeChk()))) {
#else
                ok = 0;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                if (mp->currMotionId == MARIO_MOTION_SHIP || mp->currMotionId == MARIO_MOTION_PLANE) {
                    ok = 1;
                }
                if (marioRollCancelDisable() || marioSlitContinueChk()) {
                    ok = 1;
                }
                if (w->item.useTarget > 0 && marioBgmodeChk()) {
                    ok = 1;
                }
                if (w->item.useTarget > 1 && !marioBgmodeChk()) {
                    ok = 1;
                }
                if (!ok) {
#endif
                    winMgrClose(w->useWin[0]);
                    winMgrClose(w->useWin[1]);
                    return -2;
                }
                party = !marioBgmodeChk() ? marioGetParty() : marioGetPtr()->prevPartyMemberId[0];
                k = 0;
                for (i = 0; i < 7; i++) {
                    if (party == winPartyDt[i].partyId) {
                        list[k++] = &winPartyDt[i];
                    }
                }
                for (i = 0; i < 7; i++) {
#if VERSION >= VERSION_DEMO_EU
                    if (party != winPartyDt[i].partyId && (pouchGetPtr()->partyData[winPartyDt[i].partyId].flags & POUCH_PARTY_FLAG_JOINED) &&
                        !(pouchGetPtr()->partyData[winPartyDt[i].partyId].flags & POUCH_PARTY_FLAG_AWAY)) {
#else
                    if (party != winPartyDt[i].partyId) {
#endif
                        list[k++] = &winPartyDt[i];
                    }
                }

                // BUG (early demos): only hp_restored is applied here; the FP and Star Power amounts are added from JP on.
                if (w->item.useTarget < 1) {
                    pouchSetHP(pouchGetHP() + itemDataTable[w->item.useItem].hp_restored);
#if VERSION >= VERSION_JP
                    pouchSetFP(pouchGetFP() + itemDataTable[w->item.useItem].fp_restored);
                    pouchSetAP(pouchGetAP() + itemDataTable[w->item.useItem].sp_restored);
#endif
                } else {
                    party = list[w->item.useTarget - 1]->partyId;
                    pouchSetPartyHP(party, pouchGetPartyHP(party) + itemDataTable[w->item.useItem].hp_restored);
#if VERSION >= VERSION_JP
                    pouchSetFP(pouchGetFP() + itemDataTable[w->item.useItem].fp_restored);
                    pouchSetAP(pouchGetAP() + itemDataTable[w->item.useItem].sp_restored);
#endif
                }
                w->item.blink = 0;
                w->item.state = 301;
                break;
            } else if (w->dirRep & DEMO_STICK_UP) {
                old = w->item.useTarget;
                w->item.useTarget--;
                if (w->item.useTarget < 0) {
                    w->item.useTarget = w->party.count;
                }
                if (old != w->item.useTarget) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                old = w->item.useTarget;
                w->item.useTarget++;
                if (w->item.useTarget >= w->party.count + 1) {
                    w->item.useTarget = 0;
                }
                if (old != w->item.useTarget) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                w->item.useItem = 0;
                winMgrClose(w->useWin[0]);
                winMgrClose(w->useWin[1]);
                return -2;
            }
            w->helpTgtX = -80.0f;
            old = w->item.useTarget;
            if (old == 0) {
                yoff = 0.0f;
            } else if (old == 1) {
                yoff = -16.0f;
            } else {
                yoff = -28.0f;
            }
            w->helpTgtY = (130.0f - 28.0f * old) + yoff;
            break;
        case 301:
            w->item.blink++;
            if (w->item.blink > gp->framerate * 2) {
                w->item.blink = 0;
                pouchRemoveItemIndex(w->item.useItem, w->item.useIdx);
                w->item.state = 10;
                w->item.useItem = 0;
                if (pouchGetHaveItemCnt() == 0) {
                    w->item.state = 0;
                } else if (w->item.cursor[0] >= pouchGetHaveItemCnt()) {
                    w->item.cursor[0] = pouchGetHaveItemCnt() - 1;
                }
                winMgrClose(w->useWin[0]);
                winMgrClose(w->useWin[1]);
            }
            break;
        case 1000:
            if (!winSortWait(w)) {
                w->item.state = w->savedState;
            }
            if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            break;
    }
    return 0;
}

/** Every frame: eases the list scrolls toward their pages, and the book and mail panels toward their targets. */
void winItemMain2(WindowWork* w) {
    w->item.scrollTgt[0] = 190.0f * (f32)w->item.page[0];
    w->item.scroll[0] += (w->item.scrollTgt[0] - w->item.scroll[0]) / 4.0f;
    w->item.scrollTgt[1] = 190.0f * (f32)w->item.page[1];
    w->item.scroll[1] += (w->item.scrollTgt[1] - w->item.scroll[1]) / 4.0f;
    w->item.bookX += (w->item.bookTgtX - w->item.bookX) / 6.0f;
    w->item.bookY += (w->item.bookTgtY - w->item.bookY) / 6.0f;
    w->item.mailX += (w->item.mailTgtX - w->item.mailX) / 6.0f;
    w->item.mailY += (w->item.mailTgtY - w->item.mailY) / 6.0f;
    w->item.mailScrollTgt = (f32)(w->item.mailPage * 200);
    w->item.mailScroll += (w->item.mailScrollTgt - w->item.mailScroll) / 4.0f;
}

/**
 * Draws the Items window (menu slot n): background, the two tab labels, then the item list via item_disp.
 * (x, y) is the window's position (w->menu[n]); screen coordinates have the origin at the centre and y growing
 * upwards. Most y values in winItemDisp and item_disp are written "20.0f + (...)", a 20-pixel lift added
 * around the rest (win_badge.c's page arrows do the same); where it comes from is not known.
 */
void winItemDisp(s32 camId, void* param, s32 n) {
    GXColor white = {255, 255, 255, 255};
    f32 c;
    f32 b;
    f32 a;
    f32 x;
    f32 y;
    s32 off;
    WindowWork* w = param;
    s32 i;

    x = w->menu[n].x;
    y = w->menu[n].y;

    winBgGX(w, 2, x, y);
    // the inset list panel: top-left corner (x - 150, y + 140), 415 x 210
    winKirinukiGX(w, 0, x - 150.0f, 20.0f + (120.0f + y), 415.0f, 210.0f);
    // The two tabs (i = 0 Items, 1 Important Items) down the left side, centred at x - 215 and 40 (JP)
    // or 50 (US) pixels apart. Texture 0xB3 is the tab shape; 0x18 + i the tab's label, cut to that shape
    // (winTexSet_x2). The open tab draws a half-transparent black copy of the shape as a shadow, then the
    // label in white 4 pixels up and to the left of it; the other tab draws its label in gray.
    a = 105.0f + y;
    c = (x - 215.0f) - 4.0f;
    b = 4.0f + a;
    for (i = 0, off = 0; i < 2; i++, off += VERSION_PICK(40, 50)) {
        if (i == w->item.mode) {
            winTexInit(*w->res->data);
            winTexSet(0xB3, (Vec){x - 215.0f, 20.0f + (a - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x80});
            winTexInit_x2(*w->res->data);
            winTexSet_x2(i + 0x18, 0xB3, (Vec){c, 20.0f + (b - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        } else {
            winTexInit_x2(*w->res->data);
            winTexSet_x2(i + 0x18, 0xB3, (Vec){x - 215.0f, 20.0f + (a - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                         (GXColor){0x80, 0x80, 0x80, 0xFF});
        }
    }
    item_disp(w, x, y);
}

/**
 * Grid of the item list: two columns, five rows per page (10 items). winItemMain2 scrolls by 190.0f
 * (5 * ITEM_LIST_ROW_H) per page; winItemMain moves the hand cursor on the same grid
 * (helpTgtX = column * 204 - 155, helpTgtY = 110 - row * 38).
 */
enum ItemListLayout {
    ITEM_LIST_COL_W = 204,  // left column to right column
    ITEM_LIST_ROW_H = 38,   // one row
    ITEM_LIST_PAGE = 10,    // items per page
};

/**
 * Draws the scrolling two-column item list (items not usable in the field are grayed on the Items tab),
 * the page arrows, the Z sort hint, and the open book panel.
 *
 * Item i sits in column i % 2 and row i / 2. Row 0 has its icon at (xo - 125, yo + 110) (scale 0.8 on the
 * Items tab, 2/3 on Important Items) and its name 20 pixels to the right and 10 up, at (xo - 105, yo + 120);
 * each column moves ITEM_LIST_COL_W right, each row ITEM_LIST_ROW_H down, and the list scrolls up by
 * w->item.scroll[mode]. A scissor box clips the list to screen lines 109 to 300 (of 480), inside the panel.
 */
static void item_disp(WindowWork* w, f32 xo, f32 yo) {
    GXColor white = {255, 255, 255, 255};
    GXColor white2;
    GXColor black = {0, 0, 0, 255};
    GXColor grayc = {0x80, 0x80, 0x80, 255};
    GXColor white3;
    GXColor white4;
    f32 yBase;
    s32 row;
    WindowWork* p;
    s32 col;
    s32 cx;
    f32 y;
    s32 i;
    s32 grayed;
    s32 id;
    s32 cnt;
    f32 scroll;
    f32 upArrowX;
    f32 sortWidth;
    f32 downArrowX;
    f32 sortX;

    // FAKE: white is copied through white4 into white2 and white3; using white for all of them changes the object
    white4 = white;
    white2 = white4;
    white3 = white2;
    if (w->item.mode == 0) {
        cnt = pouchGetHaveItemCnt();
    } else {
        cnt = w->item.skipCount;
    }
    GXSetScissor(0, 0x6D, 0x260, 0xC0);
    p = w;

    // FAKE: p is the work stepped one s16 at a time, so p->item.skip[0] reads skip[i] with 0x2E8 kept in the load displacement (w->item.skip[i]: r25/r26 swap; an s16* walk: base precomputed with addi)
    for (i = 0; i < cnt; p = (WindowWork*)((s16*)p + 1), ++i) {
        col = i % 2;
        row = i / 2;
        if (w->item.mode == 0) {
            id = pouchHaveItem(i);
        } else {
            id = p->item.skip[0];
        }
        scroll = w->item.scroll[w->item.mode];
        grayed = (w->item.mode != 0 || (itemDataTable[id].usable_locations & ITEM_USABLE_MENU)) == 0;
        // y: the icon centre; yBase: the row's text line minus the 20-pixel lift
        y = scroll + (20.0f + ((yBase = (100.0f + yo) - row * ITEM_LIST_ROW_H) - 10.0f));
        // rows above the screen are skipped; the first row below it ends the list
        if (y > 240.0f) {
            continue;
        }
        if (y < -240.0f) {
            break;
        }
        if (grayed) {
            winIconGrayInit();
        } else {
            winIconInit();
        }
        winIconSet(itemDataTable[id].icon_id, (Vec){(xo - 125.0f) + (cx = col * ITEM_LIST_COL_W), y, 0.0f},
                   w->item.mode == 0 ? (Vec){0.8f, 0.8f, 0.8f} : (Vec){0.6666667f, 0.6666667f, 0.6666667f}, white3);
        winFontInit();
#if VERSION >= VERSION_US
        // the name, squeezed to at most 155 pixels wide
        winFontSetWidth((Vec){(xo - 105.0f) + cx, scroll + (20.0f + yBase), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                        grayed ? grayc : black, 155.0f, msgSearch(itemDataTable[id].name));
#else
        winFontSetPitch(20.0f, (Vec){(xo - 105.0f) + cx, scroll + (20.0f + yBase), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                        grayed ? grayc : black, msgSearch(itemDataTable[id].name));
#endif
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);
    // Page arrows right of the panel at xo + 250 (texture 0x17; scale -1 turns it to point up): up arrow at
    // yo + 148 with the L button icon under it when there is a previous page, down arrow at yo - 85 with the
    // R button icon above it when there is a next page
    if (w->item.page[w->item.mode] > 0) {
        winTexInit(*w->res->data);
        upArrowX = 250.0f + xo;
        winTexSet(0x17, (Vec){upArrowX, 20.0f + (18.0f + (110.0f + yo)), 0.0f}, (Vec){-1.0f, -1.0f, -1.0f}, white2);
        winIconInit();
        winIconSet(ICON_TRIGGER_L, (Vec){upArrowX, 20.0f + (113.0f + yo), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white2);
    }
    if (cnt > w->item.page[w->item.mode] * ITEM_LIST_PAGE + ITEM_LIST_PAGE) {
        winTexInit(*w->res->data);
        downArrowX = 250.0f + xo;
        winTexSet(0x17, (Vec){downArrowX, 20.0f + ((yo - 90.0f) - 15.0f), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2);
        winIconInit();
        winIconSet(ICON_TRIGGER_R, (Vec){downArrowX, 20.0f + (yo - 90.0f), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white2);
    }
    // Sort hint under the panel: the "sort" text (scale 0.7) ends at xo + 220, the Z button icon 30 pixels
    // left of it
    sortWidth = 0.7f * (f32)FontGetMessageWidth(msgSearch("msg_menu_sort_narabikae"));
    winFontInit();
    sortX = xo + (220.0f - sortWidth);
    winFontSet((Vec){sortX, 20.0f + ((yo - 92.0f) - 10.0f), 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black,
               msgSearch("msg_menu_sort_narabikae"));
    winIconInit();
    winIconSet(ICON_TRIGGER_Z_ALT, (Vec){sortX - 30.0f, 20.0f + ((yo - 100.0f) - 10.0f), 0.0f}, (Vec){0.6f, 0.6f, 0.6f},
               white2);
    // the book panel, once it has moved in from its parked position (320, -240)
    if (w->item.bookX < 320.0f && w->item.bookY > -240.0f) {
        winBookGX(w, 0, w->item.bookX, w->item.bookY);
    }
}

#if VERSION >= VERSION_US

/**
 * HP and FP labels by game language: Japanese (full-width), English, German, French, Spanish, Italian,
 * and an unused seventh row.
 */
static const char* hpfp_name_table[7][2] = {
    {"ＨＰ", "ＦＰ"},
    {"HP", "FP"},
    {"KP", "BP"},
    {"PC", "PF"},
    {"PC", "PF"},
    {"PV", "PF"},
    {"HP", "FP"},
};

// The HP/FP labels in the menu language (before US: full-width only). Kept as macros: the same lookup through an
// inline function changes register allocation in itemUseDisp (US).
#define WI_HP_NAME hpfp_name_table[gp->language][0]
#define WI_FP_NAME hpfp_name_table[gp->language][1]
#else
#define WI_HP_NAME "ＨＰ"
#define WI_FP_NAME "ＦＰ"
#endif

/**
 * Layout of the item target window (window_desc[0], 350 x 264 at (-70, 150)). Screen coordinates have the
 * origin at the centre and y growing upwards; entry->x / entry->y are the window's top-left corner.
 * Column x offsets from the window's left edge, and the line spacing (lines go down from the top edge):
 */
enum ItemUseLayout {
    USE_ICON_X = 30,  // centre of the Mario / partner icon
    USE_NAME_X = 50,   // name
    USE_STAT_X = 220,  // "HP" / "FP" label (a coloured plate is drawn behind it)
    USE_VALUE_X = 245, // current value, right-aligned to the width of "999" (winFontSetR)
    USE_SLASH_X = 267, // "/", right-aligned the same way
    USE_MAX_X = 310,   // maximum value
    USE_LINE_H = 22,   // Mario's HP line to his FP line
    USE_ROW_H = 28,    // one row of the list (partners; Mario's row is row 0)
};

/**
 * winMgr draw callback for the item target window: Mario with his HP and FP, then each partner with HP;
 * the chosen target blinks while the item takes effect and the others gray out. Also draws the hand cursor.
 *
 * Rows (text lines; icons sit 12 lower, the HP/FP plates 11 (Mario) or 14 (partners) lower):
 *   Mario      HP at entry->y - 8, FP one USE_LINE_H lower
 *   list[0]    entry->y - 8 - USE_ROW_H - 16 (the current partner, when there is one)
 *   -------    dotted line at entry->y - 84, drawn when there are two or more partners
 *   list[1..]  entry->y - 8 - i * USE_ROW_H - 28
 * winItemMain state 300 points the hand cursor at the same rows (helpTgtY = 130 - 28 * target + the same 0/-16/-28).
 */
static void itemUseDisp(WinMgrEntry* entry) {
    WindowWork* w = winGetPtr();
    GXColor black = {0, 0, 0, 255};
    GXColor white2;
    GXColor white3;
    GXColor white = {255, 255, 255, 255};
#if VERSION <= VERSION_DEMO_JP
    WindowPartyDt* list[9];
#else
    WindowPartyDt* list[11];
#endif
    const char* name;
    s32 party;
    s32 n;
    s32 i;
    s32 off;
    s32 off3;
    s32 gray;
    f32 yoff;

    // white is copied through white2 into white3 (both copies are in the retail code)
    white2 = white;
    white3 = white2;
    if (!winMgrAction(w->useWin[0])) {
        // List order: the current partner first (in background mode the one saved in prevPartyMemberId[0]),
        // then the others in winPartyDt order (PAL demo and later: only partners who have joined and are
        // not away). Only the first w->party.count entries are drawn.
        party = !marioBgmodeChk() ? marioGetParty() : marioGetPtr()->prevPartyMemberId[0];
        n = 0;
        for (i = 0; i < 7; i++) {
            if (party == winPartyDt[i].partyId) {
                list[n++] = &winPartyDt[i];
            }
        }
        for (i = 0; i < 7; i++) {
#if VERSION >= VERSION_DEMO_EU
            if (party != winPartyDt[i].partyId && (pouchGetPtr()->partyData[winPartyDt[i].partyId].flags & POUCH_PARTY_FLAG_JOINED) &&
                !(pouchGetPtr()->partyData[winPartyDt[i].partyId].flags & POUCH_PARTY_FLAG_AWAY)) {
#else
            if (party != winPartyDt[i].partyId) {
#endif
                list[n++] = &winPartyDt[i];
            }
        }
        // Row 0 is Mario, rows 1..count the partners in list[]
        for (i = 0; i < w->party.count + 1; i++) {
            off = i * USE_LINE_H; // only used for Mario (i == 0), so always 0
            // extra drop below row i: 16 for list[0], 28 for the rest (room for Mario's FP line and the dotted line)
            if (i == 0) {
                yoff = 0.0f;
            } else if (i == 1) {
                yoff = -16.0f;
            } else {
                yoff = -28.0f;
            }
            // While the item takes effect (w->item.blink counts up for two seconds, winItemMain state 301),
            // the target is drawn in black and the other rows in gray; the target also hides 4 frames in 20
            gray = 0;
            if (w->item.blink) {
                if (i == w->item.useTarget) {
                    black = (GXColor){0, 0, 0, 255};
                } else {
                    black = (GXColor){0x80, 0x80, 0x80, 255};
                    gray = 1;
                }
            }
            if (i == 0) {
                // Mario: icon and name, HP on the first line, FP (shared by the party) on the second
                if (w->item.blink % 20 > 15 && i == w->item.useTarget) {
                    continue;
                }
                if (gray) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(ICON_MARIO, (Vec){entry->x + USE_ICON_X, entry->y - 20 - off, 0.0f}, (Vec){0.65f, 0.65f, 0.65f},
                           white3);
                // HP (pink) and FP (yellow) plates, centred under the labels: texture 0xAF is used only as an alpha
                // mask (TEV colour = C0, the colour passed to winTexSet); alpha 0xFE makes winTexSet blend
                winTexInit(*w->res->data);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                winTexSet(0xAF,
                          (Vec){entry->x + USE_STAT_X + 0.9f * FontGetMessageWidth(WI_HP_NAME) / 2,
                                entry->y - 19 - off,
                                0.0f},
                          (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xCB, 0x8C, 0xA2, 0xFE});
                winTexSet(0xAF,
                          (Vec){entry->x + USE_STAT_X + 0.9f * FontGetMessageWidth(WI_HP_NAME) / 2,
                                entry->y - 19 - (off3 = (i + 1) * USE_LINE_H),
                                0.0f},
                          (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xCB, 0xBE, 0x89, 0xFE});
                winFontInit();
                winFontSet((Vec){entry->x + USE_NAME_X, entry->y - 8 - off, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
                           msgSearch("name_mario"));
                // HP line: "HP  cur / max" (the maximum is smaller and 2 lower)
                winFontSet((Vec){entry->x + USE_STAT_X, entry->y - 8 - off, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                           WI_HP_NAME);
#if VERSION <= VERSION_DEMO_JP
                winFontSetR((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                            "%d", pouchGetHP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, entry->y - 10 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                           "%d", pouchGetMaxHP());
                // FP line
                winFontInit();
                winFontSet((Vec){entry->x + USE_STAT_X, entry->y - 8 - off3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                           WI_FP_NAME);
                winFontSetR((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                            "%d", pouchGetFP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, entry->y - 10 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                           "%d", pouchGetMaxFP());
#else
                // (PAL demo and later squeeze the numbers to 30 px)
#if VERSION >= VERSION_DEMO_EU
                winFontSetRWidth((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off, 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                                 black, 30.0f, "%d", pouchGetHP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSetWidth((Vec){entry->x + USE_MAX_X, entry->y - 10 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                                black, 30.0f, "%d", pouchGetMaxHP());
#else
                winFontSetR((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                            "%d", pouchGetHP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, entry->y - 10 - off, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                           "%d", pouchGetMaxHP());
#endif
                // FP line: an FP item makes it blink and stay black whoever the target is (FP is shared)
                if (itemDataTable[w->item.useItem].fp_restored) {
                    if (w->item.blink % 20 > 15) {
                        continue;
                    }
                    black = (GXColor){0, 0, 0, 255};
                }
                winFontInit();
                winFontSet((Vec){entry->x + USE_STAT_X, entry->y - 8 - off3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                           WI_FP_NAME);
#if VERSION >= VERSION_DEMO_EU
                winFontSetRWidth((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                                 black, 30.0f, "%d", pouchGetFP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSetWidth((Vec){entry->x + USE_MAX_X, entry->y - 10 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                                black, 30.0f, "%d", pouchGetMaxFP());
#else
                winFontSetR((Vec){entry->x + USE_VALUE_X, entry->y - 8 - off3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                            "%d", pouchGetFP());
                winFontSetR((Vec){entry->x + USE_SLASH_X, entry->y - 8 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                            WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, entry->y - 10 - off3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black,
                           "%d", pouchGetMaxFP());
#endif
#endif
            } else {
                // Partner row i (list[i - 1]): HP plate, icon, name (Yoshi: the name the player gave him), HP
                name = list[i - 1]->partyId != PARTY_YOSHI ? msgSearch(list[i - 1]->name) : pouchGetYoshiName();
                if (w->item.blink % 20 > 15 && i == w->item.useTarget) {
                    continue;
                }
                winTexInit(*w->res->data);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                winTexSet(0xAF,
                          (Vec){entry->x + USE_STAT_X + 0.9f * FontGetMessageWidth(WI_HP_NAME) / 2,
                                (yoff + (entry->y - 8 - i * USE_ROW_H)) - 14.0f,
                                0.0f},
                          (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xCB, 0x8C, 0xA2, 0xF4});
                if (gray) {
                    winIconGrayInit();
                } else {
                    winIconInit();
                }
                winIconSet(list[i - 1]->iconId, (Vec){entry->x + USE_ICON_X, yoff + (entry->y - 20 - i * USE_ROW_H), 0.0f},
                           (Vec){0.65f, 0.65f, 0.65f}, white3);
                winFontInit();
#if VERSION <= VERSION_DEMO_JP
                winFontSet((Vec){entry->x + USE_NAME_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                           (Vec){1.0f, 1.0f, 1.0f}, black, name);
                winFontSet((Vec){entry->x + USE_STAT_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                           (Vec){0.9f, 0.9f, 0.9f}, black, WI_HP_NAME);
                winFontSetR((Vec){entry->x + USE_VALUE_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                            (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].currentHP);
                winFontSetR((Vec){entry->x + USE_SLASH_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                            (Vec){0.8f, 0.8f, 0.8f}, black, WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, (yoff + (entry->y - 8 - i * USE_ROW_H)) - 2.0f, 0.0f},
                           (Vec){0.8f, 0.8f, 0.8f}, black, "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].maximumHP);
#else
                // the name is squeezed horizontally to at most 160 px
                winFontSet((Vec){entry->x + USE_NAME_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                           (Vec){FontGetMessageWidth(name) < 160.0f ? 1.0f : 160.0f / FontGetMessageWidth(name), 1.0f, 1.0f},
                           black, name);
                winFontSet((Vec){entry->x + USE_STAT_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                           (Vec){0.9f, 0.9f, 0.9f}, black, WI_HP_NAME);
#if VERSION >= VERSION_DEMO_EU
                winFontSetRWidth((Vec){entry->x + USE_VALUE_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                                 (Vec){0.9f, 0.9f, 0.9f}, black, 30.0f,
                                 "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].currentHP);
                winFontSetR((Vec){entry->x + USE_SLASH_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                            (Vec){0.8f, 0.8f, 0.8f}, black, WI_SLASH);
                winFontSetWidth((Vec){entry->x + USE_MAX_X, (yoff + (entry->y - 8 - i * USE_ROW_H)) - 2.0f, 0.0f},
                                (Vec){0.8f, 0.8f, 0.8f}, black, 30.0f,
                                "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].maximumHP);
#else
                winFontSetR((Vec){entry->x + USE_VALUE_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                            (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].currentHP);
                winFontSetR((Vec){entry->x + USE_SLASH_X, yoff + (entry->y - 8 - i * USE_ROW_H), 0.0f},
                            (Vec){0.8f, 0.8f, 0.8f}, black, WI_SLASH);
                winFontSet((Vec){entry->x + USE_MAX_X, (yoff + (entry->y - 8 - i * USE_ROW_H)) - 2.0f, 0.0f},
                           (Vec){0.8f, 0.8f, 0.8f}, black, "%d", pouchGetPtr()->partyData[list[i - 1]->partyId].maximumHP);
#endif
#endif
            }
        }
        // Dotted line between list[0] and the other partners: 28 SP orb icons at 0.15 scale,
        // one every width / 31 px (11 px for this window), from dot 2 to dot 29
        if (w->party.count > 1) {
            winIconInit();
            for (i = 2; i < 30; i++) {
                winIconSet(ICON_SP_ORB_1, (Vec){entry->x + (entry->width / 31) * i, entry->y - 84, 0.0f},
                           (Vec){0.15f, 0.15f, 0.15f}, black);
            }
        }
    }
    // The hand cursor (texture 0 of win.tpl) at helpX / helpY, over a half-transparent black shadow 2 px right
    // and 2 px down
    winTexInit(*w->res->data);
    winTexSet(0, (Vec){2.0f + w->helpX, w->helpY - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x80});
    winTexSet(0, (Vec){w->helpX, w->helpY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
}

/**
 * winMgr draw callback for the small orange title window over the target list:
 * centres msg_window_select_5 in it (US and later resize it to the text's line count and squeeze
 * long text).
 */
static void itemUseDisp2(WinMgrEntry* entry) {
    WindowWork* wp = winGetPtr();
    GXColor color = {0, 0, 0, 255};
    if (!winMgrAction(wp->useWin[1])) {
        const char* msg = msgSearch("msg_window_select_5");
#if VERSION >= VERSION_US
        u16 lines;
        f32 w = (f32)FontGetMessageWidthLine(msg, &lines);
        winMgrSetSize(wp->useWin[1], entry->x, entry->y, entry->width, entry->description->height + lines * 22);
        winFontInit();
        if (w > entry->width - 20) {
            w = entry->width - 20;
        }
        winFontSetWidth((Vec){(f32)entry->x + ((f32)entry->width - w) / 2.0f, 134.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, color, entry->width - 20, msg);
#else
        f32 w = (f32)FontGetMessageWidth(msg);
        winFontInit();
        winFontSet((Vec){(f32)entry->x + ((f32)entry->width - w) / 2.0f, 134.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, color, msg);
#endif
    }
}
