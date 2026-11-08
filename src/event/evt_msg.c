// Script commands for message windows: printing, choices, inserted strings and closing.

#include "version.h"
#include "evt/evt_msg.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "driver/windowdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>
#include <stdio.h>
#include "battle/battle_unit.h"
#include "evt/evt_npc.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "libc_decls.h"

/** Message-script setup hook called at startup; empty in every release. */
void evt_msg_init(void) {
    // empty in every version
}

// FAKE: private const char* prototype; the definition takes char* (it passes msg to msgAnalize, which takes char*
// in every version after the early demos), so a header prototype rejects the const char* arguments here without casts.
void msgWindow_Add(const char* msg, s32 id);

extern float __fabsf(float);

// A message window's MsgWinData lives in its userdata block. Kept as a macro: an inline function changes the object.
#define MSG_WIN_DATA(w) ((MsgWinData*)(w)->userdata->address)

/**
 * Shared body of the evt_msg_print commands.
 * On the first call opens a message window for msg (a message id looked up with msgSearch,
 * or literal text when flags bit 0 is set) and points its speech-bubble tail at the speaker:
 * the NPC named by name (or the event's owner NPC), the partner ("party"), the second partner ("extparty"),
 * Mario ("mario"), or with flags bit 1 a battle unit. evt_msg_toge/togelr settings then override the tail.
 * Blocks until the window closes.
 */
static s32 _evt_msg_print(EventEntry* event, BOOL isFirstCall, s32 flags, const char* msg, s32 callback, EvtWord name) {
    NpcEntry* npc = NULL;
    WindowEntry* win;
    const char* m;
    if (isFirstCall) {
        if (!(flags & MSG_PRINT_DIRECT_TEXT)) {
            m = msgSearch(msg);
        } else {
            m = msg;
            msg = "DIRECT";
        }

        // FAKE: the priority goes in sign-extended from s16 through an s32 parameter; a call through msgdrv.h's
        // u16 prototype changes the object.
        event->printWindowId = ((s32 (*)(const char*, s32, s32))msgWindow_Entry)(m, callback, (s16)event->msgPriority);
        event->msgPriority = 0;
        win = windowGetPointer(event->printWindowId);
        MSG_WIN_DATA(win)->tag = msg;
        MSG_WIN_DATA(win)->event = event;
        if (!(flags & MSG_PRINT_BATTLE_UNIT)) {
            if (event->wNpcEventType == NPC_EVENT_TALK) {
                npc = event->ownerNPC;
            }
            if ((const char*)name) {
                npc = evtNpcNameToPtr(event, (const char*)name);
            }
            if (npc == EVT_NPC_PTR_PARTY) {
                if (!(event->printWindowFlags & MSG_PRINT_WIN_TOGE_MASK)) {
                    PartyEntry* p = partyGetPtr(marioGetPartyId());
                    if (!p) {
                        win->speakerPos = marioGetPtr()->position;
                        win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                    } else {
                        win->speakerPos = p->position;
                        win->speakerPos.y += p->height;
                    }
                }
                strcpy(MSG_WIN_DATA(win)->speakerName, (const char*)name);
            } else if (npc == EVT_NPC_PTR_EXTPARTY) {
                if (!(event->printWindowFlags & MSG_PRINT_WIN_TOGE_MASK)) {
                    PartyEntry* p = partyGetPtr(marioGetExtraPartyId());
                    if (!p) {
                        win->speakerPos = marioGetPtr()->position;
                        win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                    } else {
                        win->speakerPos = p->position;
                        win->speakerPos.y += p->height;
                    }
                }
                strcpy(MSG_WIN_DATA(win)->speakerName, (const char*)name);
            } else if (npc == EVT_NPC_PTR_MARIO) {
                if (!(event->printWindowFlags & MSG_PRINT_WIN_TOGE_MASK)) {
                    win->speakerPos = marioGetPtr()->position;
                    win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                }
                strcpy(MSG_WIN_DATA(win)->speakerName, (const char*)name);
            } else if (npc) {
                if (!(event->printWindowFlags & MSG_PRINT_WIN_TOGE_MASK)) {
                    win->speakerPos = npc->position;
                    win->speakerPos.y += npc->height;
                }
                strcpy(MSG_WIN_DATA(win)->speakerName, npc->description);
            }
        } else {
            MSG_WIN_DATA(win)->flags |= MSG_WIN_FLAG_BATTLE_UNIT;
            MSG_WIN_DATA(win)->unitId = BattleTransID(event, name);
            BtlUnit_GetTalkTogePos(MSG_WIN_DATA(win)->unitId, &win->speakerPos.x, &win->speakerPos.y, &win->speakerPos.z);
            win->tailMode = 3;
        }
        switch (event->printWindowFlags & MSG_PRINT_WIN_TOGE_MASK) {
            case MSG_TOGE_AUTO:
                if (npc) {
                    Vec scr, mscr;
                    marioGetPtr();
                    getScreenPoint(&win->speakerPos, &scr);
                    if (win->type == WINDOW_TYPE_SMALL) {
                        win->tailMode = 7;
                    } else if (__fabsf(scr.x) >= 300.0f) {
                        win->tailMode = 2;
                    } else {
                        getScreenPoint(&marioGetPtr()->position, &mscr);
                        if (scr.x < mscr.x) {
                            win->tailMode = 3;
                        } else {
                            win->tailMode = 7;
                        }
                    }
                }
                break;
            case MSG_TOGE_NONE:
                win->tailMode = 0;
                break;
            case MSG_TOGE_AT_POS:
                win->tailMode = 7;
                win->speakerPos = event->msgPosVec;
                break;
            case MSG_TOGE_AT_POS_FAR:
                win->tailMode = 2;
                win->speakerPos = event->msgPosVec;
                break;
        }
        if (event->printWindowFlags & MSG_PRINT_WIN_TOGELR) {
            win->tailMode = 3;
            if (event->printWindowFlags & MSG_PRINT_WIN_TOGELR_RIGHT) {
                win->tailMode |= 4;
            }
            if (event->printWindowFlags & MSG_PRINT_WIN_TOGELR_ABOVE) {
                win->tailMode |= 8;
            }
        }
        if (event->printWindowFlags & MSG_PRINT_WIN_SCREEN_POS) {
            win->tailMode |= 0x10;
        }
        event->printWindowFlags = 0;
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Opens a message window and waits until it closes.
 * Args: flags (bit 0 literal text, bit 1 battle speaker), the message id,
 * a window callback and the speaker (an NPC name, "party", "extparty", "mario" or 0).
 */
USER_FUNC(evt_msg_print) {
    EvtWord* args = event->args;
    s32 flags = evtGetValue(event, args[0]);
    const char* msg = (const char*)evtGetValue(event, args[1]);
    s32 callback = evtGetValue(event, args[2]);
    EvtWord name = evtGetValue(event, args[3]);
    return _evt_msg_print(event, isFirstCall, flags, msg, callback, name);
}

/**
 * Message label suffix of each partner, indexed by partner id (0 none, 1 Goombella, 2 Koops, 3 Bobbery, 4 Yoshi,
 * 5 Flurrie, 6 Vivian, 7 Ms. Mowz, then the extra ids).
 */
const char* partyMsgStr[] = {"", "_kur", "_nok", "_bom", "_yos", "_win", "_viv", "_chu", "_jug", "_tel", "_lui"};
/**
 * Message label suffix of each partner battle unit, indexed by unit kind minus 0xE0
 * (Goombella, Koops, Yoshi, Flurrie, Vivian, Bobbery, Ms. Mowz).
 */
const char* battlePartyMsgStr[] = {"_kur", "_nok", "_yos", "_win", "_viv", "_bom", "_chu"};

#if VERSION >= VERSION_US
#include "mario/mariost.h"
// US and later: localized article/plural helpers (evt_msg_fmt_int / evtMsgFmtInt, evt_msg_fmt_str / evtMsgFmtStr, below).
// clang-format off
#if VERSION >= VERSION_DEMO_EU
const char* msg_de_article_nom_cap_tbl[7] = {"Ein ", "Eine ", "Ein ", "Die ", "Die ", "Die ", ""};
const char* msg_de_article_nom_tbl[7] = {"ein ", "eine ", "ein ", "die ", "die ", "die ", ""};
const char* msg_de_article_acc_cap_tbl[7] = {"Einen ", "Eine ", "Ein ", "Die ", "Die ", "Die ", ""};
const char* msg_de_article_acc_tbl[7] = {"einen ", "eine ", "ein ", "die ", "die ", "die ", ""};
const char* msg_de_article_dat_tbl[7] = {"einem ", "einer ", "einem ", "die ", "die ", "die ", ""};
const char* msg_fr_article_cap_tbl[4] = {"Un ", "Une ", "Les ", "Des "};
const char* msg_fr_article_tbl[4] = {"un ", "une ", "les ", "des "};
#else
const char* msg_de_article_nom_cap_tbl[6] = {"Ein ", "Eine ", "Ein ", "Die ", "Die ", "Die "};
const char* msg_de_article_nom_tbl[6] = {"ein ", "eine ", "ein ", "die ", "die ", "die "};
const char* msg_de_article_acc_cap_tbl[6] = {"Einen ", "Eine ", "Ein ", "Die ", "Die ", "Die "};
const char* msg_de_article_acc_tbl[6] = {"einen ", "eine ", "ein ", "die ", "die ", "die "};
const char* msg_fr_article_cap_tbl[3] = {"Un ", "Une ", "Les "};
const char* msg_fr_article_tbl[3] = {"un ", "une ", "les "};
#endif
const char* msg_es_article_cap_tbl[6] = {"Un ", "Una ", "", "", "Unos ", "Unas "};
const char* msg_es_article_tbl[6] = {"un ", "una ", "", "", "unos ", "unas "};
#if VERSION >= VERSION_DEMO_EU
// FAKE: unused zero object, read by nothing; removing it changes the object.
static const char* msgArticlePad[7] = {0};
#endif

// clang-format on
#endif

/**
 * Prints message args[0] spoken by the current partner,
 * with the partner's suffix appended to the id (_kur for Goombella, _nok Koops, _bom Bobbery, _yos Yoshi,
 * _win Flurrie, _viv Vivian, _chu Ms. Mowz), so each partner gets a line of their own;
 * no suffix with no partner out.
 */
USER_FUNC(evt_msg_print_party) {
    static char partyLabel[0x40];
    s32 id;
    const char* msg = (const char*)evtGetValue(event, *event->args);
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        id = party->currentMemberId;
    } else {
        id = 0;
    }
    strcpy(partyLabel, msg);
    strcat(partyLabel, partyMsgStr[id]);
    return _evt_msg_print(event, isFirstCall, 0, partyLabel, 0, (EvtWord)"party");
}

/**
 * Adds the current partner's version of message args[0] (id plus partner suffix, as evt_msg_print_party) to the
 * open message window and waits until the window closes or continues.
 */
USER_FUNC(evt_msg_print_party_add) {
    static char partyLabel[0x40];
    s32 id;
    const char* msg = (const char*)evtGetValue(event, *event->args);
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        id = party->currentMemberId;
    } else {
        id = 0;
    }
    if (isFirstCall) {
        strcpy(partyLabel, msg);
        strcat(partyLabel, partyMsgStr[id]);
        msgWindow_Add(msgSearch(partyLabel), event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * In battle: prints message args[0] spoken by the active partner unit,
 * with that partner's suffix appended to the id; does nothing if there is no partner unit.
 */
USER_FUNC(evt_msg_print_battle_party) {
    static char partyLabel[0x40];
    const char* msg = (const char*)evtGetValue(event, *event->args);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, UNIT_ID_PARTNER));
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    strcpy(partyLabel, msg);
    strcat(partyLabel, battlePartyMsgStr[(s32)unit->currentKind - TYPE_PARTNER_MIN]);
    return _evt_msg_print(event, isFirstCall, MSG_PRINT_BATTLE_UNIT, partyLabel, 0, UNIT_ID_PARTNER);
}

/**
 * Like evt_msg_print, but first fills the message's printf placeholders with up to four extra script arguments
 * (counts, slot numbers, coin amounts) into a local buffer (twice as large in US and later).
 */
USER_FUNC(evt_msg_print_insert) {
    char buf[MSG_BUF_SIZE];
    s32 vals[4];
    s32* p;
    EvtWord* args = event->args;
    s32 n = (args[-2] >> 16) - 5;
    s32 flags = evtGetValue(event, *args++);
    const char* msg = (const char*)evtGetValue(event, *args++);
    s32 callback;
    EvtWord name;
    s32 i;
    callback = evtGetValue(event, *args++);
    name = evtGetValue(event, *args++);
    for (i = 0, p = vals; i < n; i++) {
        *p++ = evtGetValue(event, *args++);
    }
    if (isFirstCall) {
        const char* m;
        if (flags & MSG_PRINT_DIRECT_TEXT) {
            m = msg;
        } else {
            m = msgSearch(msg);
        }
        switch (n) {
            case 0:
                break;
            case 1:
                sprintf(buf, m, vals[0]);
                break;
            case 2:
                sprintf(buf, m, vals[0], vals[1]);
                break;
            case 3:
                sprintf(buf, m, vals[0], vals[1], vals[2]);
                break;
            case 4:
                sprintf(buf, m, vals[0], vals[1], vals[2], vals[3]);
                break;
        }
    }
    return _evt_msg_print(event, isFirstCall, flags | MSG_PRINT_DIRECT_TEXT, buf, callback, name);
}

/**
 * Appends message args[1] (literal text when args[0] bit 0 is set) to the script's open message window,
 * so a conversation continues in the same window, and waits until it closes or pauses.
 */
USER_FUNC(evt_msg_print_add) {
    EvtWord* args = event->args;
    s32 flags = evtGetValue(event, args[0]);
    const char* msg = (const char*)evtGetValue(event, args[1]);
    if (isFirstCall) {
        if (!(flags & MSG_PRINT_DIRECT_TEXT)) {
            msgWindow_Add(msgSearch(msg), event->printWindowId);
        } else {
            msgWindow_Add(msg, event->printWindowId);
        }
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** evt_msg_print_add with up to four printf values filled into the message first. */
USER_FUNC(evt_msg_print_add_insert) {
    char buf[MSG_BUF_SIZE];
    s32 vals[4];
    s32* p;
    EvtWord* args = event->args;
    s32 n = (args[-2] >> 16) - 3;
    s32 flags = evtGetValue(event, *args++);
    const char* msg = (const char*)evtGetValue(event, *args++);
    s32 i;
    for (i = 0, p = vals; i < n; i++) {
        *p++ = evtGetValue(event, *args++);
    }
    if (isFirstCall) {
        const char* m;
        if (flags & MSG_PRINT_DIRECT_TEXT) {
            m = msg;
        } else {
            m = msgSearch(msg);
        }
        switch (n) {
            case 0:
                break;
            case 1:
                sprintf(buf, m, vals[0]);
                break;
            case 2:
                sprintf(buf, m, vals[0], vals[1]);
                break;
            case 3:
                sprintf(buf, m, vals[0], vals[1], vals[2]);
                break;
            case 4:
                sprintf(buf, m, vals[0], vals[1], vals[2], vals[3]);
                break;
        }
        msgWindow_Add(buf, event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Lets the script's paused message window carry on (after a choice, for example) and waits until it closes. */
USER_FUNC(evt_msg_continue) {
    if (isFirstCall) {
        msgWindow_Continue(event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Restarts the script's message window from its first page (scroll and print state reset) and waits
 * until it closes.
 */
USER_FUNC(evt_msg_repeat) {
    if (isFirstCall) {
        msgWindow_Repeat(event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Force-closes the message window whose tag is the message id args[0]. */
USER_FUNC(evt_msg_close) {
    msgWindow_ForceClose((const char*)evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

/**
 * Shows a choice window (message args[1], e.g. msg_mcard_yesno; literal text when args[0] bit 0 is set),
 * waits for the player to pick, then returns the chosen option in LW(0) (0 = first) and deletes the window.
 */
USER_FUNC(evt_msg_select) {
    EvtWord* args = event->args;
    s32 flags = evtGetValue(event, args[0]);
    const char* msg = (const char*)evtGetValue(event, args[1]);
    if (isFirstCall) {
        const char* m;
        if (!(flags & MSG_PRINT_DIRECT_TEXT)) {
            m = msgSearch(msg);
        } else {
            m = msg;
            msg = "DIRECT";
        }
        event->selectWindowId = msgWindow_Entry(m, 0, 0x14);
        MSG_WIN_DATA(windowGetPointer(event->selectWindowId))->tag = msg;
    }
    if (windowCheckID(event->selectWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    event->lwData[0] = MSG_WIN_DATA(windowGetPointer(event->selectWindowId))->sel;
    windowDeleteID(event->selectWindowId);
    return EVT_RETURN_DONE;
}

/**
 * Sets where the next message window's tail points:
 * args[0] is the tail kind (1 no tail; 2 and 3 point at the X, Y, Z position in args[1]-args[3],
 * as tail modes 7 and 2) for the window that evt_msg_print opens next.
 */
USER_FUNC(evt_msg_toge) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    event->printWindowFlags &= ~MSG_PRINT_WIN_TOGE_MASK;
    event->printWindowFlags |= mode;
    event->msgPos.x = evtGetFloat(event, args[1]);
    event->msgPos.y = evtGetFloat(event, args[2]);
    event->msgPosZf = evtGetFloat(event, args[3]);
    return EVT_RETURN_DONE;
}

/** Forces tail mode 3 on the next message window; args[0] adds tail bit 4 (1), bit 8 (2) or both (3). */
USER_FUNC(evt_msg_togelr) {
    s32 side = evtGetValue(event, *event->args);
    event->printWindowFlags |= MSG_PRINT_WIN_TOGELR;
    switch (side) {
        case 0:
            break;
        case 1:
            event->printWindowFlags |= MSG_PRINT_WIN_TOGELR_RIGHT;
            break;
        case 2:
            event->printWindowFlags |= MSG_PRINT_WIN_TOGELR_ABOVE;
            break;
        case 3:
            event->printWindowFlags |= (MSG_PRINT_WIN_TOGELR_RIGHT | MSG_PRINT_WIN_TOGELR_ABOVE);
            break;
    }
    return EVT_RETURN_DONE;
}

/** Sets the priority of the next message window the script opens (reset to 0 after one use). */
USER_FUNC(evt_msg_pri) {
    event->msgPriority = evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

/** Changes the speaker name stored in the script's open message window to args[0]. */
USER_FUNC(evt_msg_npc) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    strcpy(MSG_WIN_DATA(windowGetPointer(event->printWindowId))->speakerName, name);
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_US
/**
 * US and later: localized article/plural helpers.
 * These builds emit functions in reverse source order,
 * so these sit at the end of the file to come first in .text.
 * Replaces the article and item markers in buf (<AN>, <AN_ITEM>, <ITEM>, and the German,
 * French and Spanish <Ein_ITEM>/<un_ITEM> forms) with the right article for the item name and the name itself,
 * using the gender and plural tags in the name (<M>, <F>, <PLles>, ...).
 * Picks the rules by gp->language; Italian (5) only substitutes <ITEM>.
 */
void evtMsgFmtStr(char* buf, const char* name) {
    char tmp[0xA00];
    u32 len;
    char* marker;
    const char* article;
    s32 form;

    switch (gp->language) {
        case LANGUAGE_US:
        case 6:
            // FAKE: (signed char) cast on the first character; switch (*name) changes the object in US, PAL and the September demos
            switch ((signed char)*name) {
            case 'A':
            case 'E':
            case 'I':
            case 'O':
            case 'U':
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                article = "an ";
                break;
            default:
                article = "a ";
                break;
            }
            if ((marker = strstr(buf, "<AN>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, article);
                strcat(tmp, marker + 4);
                strcpy(buf, tmp);
                if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                    len = strlen(buf) - strlen(marker);
                    strncpy(tmp, buf, len);
                    tmp[len] = 0;
                    strcat(tmp, name);
                    strcat(tmp, marker + 6);
                }
            } else if ((marker = strstr(buf, "<AN_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, article);
                strcat(tmp, name);
                strcat(tmp, marker + 9);
            } else if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
            break;
        case LANGUAGE_GE:
            form = 0;
            if (strstr(name, "<M>") != NULL) {
                form = 0;
            } else if (strstr(name, "<F>") != NULL) {
                form = 1;
            } else if (strstr(name, "<N>") != NULL) {
                form = 2;
            } else if (strstr(name, "<MPLdie>") != NULL) {
                form = 3;
            } else if (strstr(name, "<FPLdie>") != NULL) {
                form = 4;
            } else if (strstr(name, "<NPLdie>") != NULL) {
                form = 5;
#if VERSION >= VERSION_DEMO_EU
            } else if (strstr(name, "<PL") != NULL) {
                form = 6;
#endif
            }
            if ((marker = strstr(buf, "<Einen>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_acc_cap_tbl[form]);
                strcat(tmp, marker + 7);
                strcpy(buf, tmp);
                if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                    len = strlen(buf) - strlen(marker);
                    strncpy(tmp, buf, len);
                    tmp[len] = 0;
                    strcat(tmp, name);
                    strcat(tmp, marker + 6);
                }
            } else if ((marker = strstr(buf, "<Ein_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_nom_cap_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 10);
            } else if ((marker = strstr(buf, "<ein_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_nom_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 10);
            } else if ((marker = strstr(buf, "<Einen_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_acc_cap_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 12);
            } else if ((marker = strstr(buf, "<einen_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_acc_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 12);
#if VERSION >= VERSION_DEMO_EU
            } else if ((marker = strstr(buf, "<einem_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_de_article_dat_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 12);
#endif
            } else if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
            break;
        case LANGUAGE_FR:
            form = 0;
            if (strstr(name, "<M>") != NULL) {
                form = 0;
            } else if (strstr(name, "<F>") != NULL) {
                form = 1;
            } else if (strstr(name, "<PLles>") != NULL) {
                form = 2;
#if VERSION >= VERSION_DEMO_EU
            } else if (strstr(name, "<PLdes>") != NULL) {
                form = 3;
#endif
            }
            if ((marker = strstr(buf, "<un>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_fr_article_tbl[form]);
                strcat(tmp, marker + 4);
                strcpy(buf, tmp);
                if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                    len = strlen(buf) - strlen(marker);
                    strncpy(tmp, buf, len);
                    tmp[len] = 0;
                    strcat(tmp, name);
                    strcat(tmp, marker + 6);
                }
            } else if ((marker = strstr(buf, "<Un_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_fr_article_cap_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 9);
            } else if ((marker = strstr(buf, "<un_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_fr_article_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 9);
            } else if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
            break;
        case LANGUAGE_SP:
            form = 0;
            if (strstr(name, "<M>") != NULL) {
                form = 0;
            } else if (strstr(name, "<F>") != NULL) {
                form = 1;
            } else if (strstr(name, "<MNA>") != NULL) {
                form = 2;
            } else if (strstr(name, "<FNA>") != NULL) {
                form = 3;
            } else if (strstr(name, "<PLunos>") != NULL) {
                form = 4;
            } else if (strstr(name, "<PLunas>") != NULL) {
                form = 5;
            }
            if ((marker = strstr(buf, "<un>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_es_article_tbl[form]);
                strcat(tmp, marker + 4);
                strcpy(buf, tmp);
                if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                    len = strlen(buf) - strlen(marker);
                    strncpy(tmp, buf, len);
                    tmp[len] = 0;
                    strcat(tmp, name);
                    strcat(tmp, marker + 6);
                }
            } else if ((marker = strstr(buf, "<Un_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_es_article_cap_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 9);
            } else if ((marker = strstr(buf, "<un_ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, msg_es_article_tbl[form]);
                strcat(tmp, name);
                strcat(tmp, marker + 9);
            } else if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
            break;
        case LANGUAGE_IT:
            if ((marker = strstr(buf, "<ITEM>")) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
            break;
        case LANGUAGE_JP:
            strcpy(tmp, buf);
            break;
    }

    // BUG: when the text has none of the markers its language looks for (or the language has no case), tmp is
    // never written and buf receives uninitialized stack data.
    strcpy(buf, tmp);
}

/**
 * US and later: builds a message with an item name and its article:
 * args[2] is the message (id when args[0] is 0, literal text when 1), args[3] the item name;
 * returns the formatted text in args[1] (a shared static buffer).
 */
USER_FUNC(evt_msg_fmt_str) {
    EvtWord* args = event->args;
    s32 mode = args[0];
    s32 out = args[1];
    const char* src = (const char*)evtGetValue(event, args[2]);
    const char* item = (const char*)evtGetValue(event, args[3]);
    const char* name;

    switch (mode) {
        case 0:
            name = msgSearch(src);
            break;
        case 1:
            name = src;
            break;
    }
    strcpy(msg_article_buf, name);
    evtMsgFmtStr(msg_article_buf, item);
    evtSetValue(event, out, (EvtWord)msg_article_buf);
    return EVT_RETURN_DONE;
}

/**
 * US and later: replaces <NUM> in buf with count and then fixes the plural markers for the language:
 * <S> (and <X> in French) adds the plural ending, <N> is dropped,
 * and German and Italian spell out coin and point words (<COIN>, <POINT>, ...).
 */
void evtMsgFmtInt(char* buf, s32 count) {
    char* marker;
    u32 len;
    char num[16];
    u32 numLen;
    char* found;

    found = strstr(buf, "<NUM>");
    if (found == NULL) {
        return;
    }
    numLen = strlen(buf) - strlen(found);
    strncpy(msg_num_buf, buf, numLen);
    msg_num_buf[numLen] = 0;
    sprintf(num, "%d", count);
    strcat(msg_num_buf, num);
    strcat(msg_num_buf, found + 5);

    switch (gp->language) {
        case LANGUAGE_US:
        case LANGUAGE_SP:
        case 6:
            if ((marker = strstr(msg_num_buf, "<S>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count != 1) {
                    strcat(msg_plural_buf, "s");
                }
                strcat(msg_plural_buf, marker + 3);
            } else if ((marker = strstr(msg_num_buf, "<N>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                strcat(msg_plural_buf, marker + 3);
            }
            break;
        case LANGUAGE_FR:
            if ((marker = strstr(msg_num_buf, "<X>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count > 1) {
                    strcat(msg_plural_buf, "x");
                }
                strcat(msg_plural_buf, marker + 3);
            } else if ((marker = strstr(msg_num_buf, "<S>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count > 1) {
                    strcat(msg_plural_buf, "s");
                }
                strcat(msg_plural_buf, marker + 3);
            } else if ((marker = strstr(msg_num_buf, "<N>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                strcat(msg_plural_buf, marker + 3);
            }
            break;
        case LANGUAGE_GE:
            if ((marker = strstr(msg_num_buf, "<COIN>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "M\xFCnze");
                } else {
                    strcat(msg_plural_buf, "M\xFCnzen");
                }
                strcat(msg_plural_buf, marker + 6);
            } else if ((marker = strstr(msg_num_buf, "<POINT>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "Punkt");
                } else {
                    strcat(msg_plural_buf, "Punkte");
                }
                strcat(msg_plural_buf, marker + 7);
            } else if ((marker = strstr(msg_num_buf, "<S>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count != 1) {
                    strcat(msg_plural_buf, "s");
                }
                strcat(msg_plural_buf, marker + 3);
            } else if ((marker = strstr(msg_num_buf, "<N>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                strcat(msg_plural_buf, marker + 3);
            }
            break;
        case LANGUAGE_IT:
            if ((marker = strstr(msg_num_buf, "<COIN>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "moneta");
                } else {
                    strcat(msg_plural_buf, "monete");
                }
                strcat(msg_plural_buf, marker + 6);
            } else if ((marker = strstr(msg_num_buf, "<point>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "punto");
                } else {
                    strcat(msg_plural_buf, "punti");
                }
                strcat(msg_plural_buf, marker + 7);
            } else if ((marker = strstr(msg_num_buf, "<POINT>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "Punto");
                } else {
                    strcat(msg_plural_buf, "Punti");
                }
                strcat(msg_plural_buf, marker + 7);
            } else if ((marker = strstr(msg_num_buf, "<OGGETTO>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count == 1) {
                    strcat(msg_plural_buf, "oggetto");
                } else {
                    strcat(msg_plural_buf, "oggtti");
                }

                // BUG: <OGGETTO> is 9 characters but only 7 are skipped, so "O>" stays in the text; the plural above
                // is spelled "oggtti".
                strcat(msg_plural_buf, marker + 7);
            } else if ((marker = strstr(msg_num_buf, "<S>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                if (count != 1) {
                    strcat(msg_plural_buf, "s");
                }
                strcat(msg_plural_buf, marker + 3);
            } else if ((marker = strstr(msg_num_buf, "<N>")) != NULL) {
                len = strlen(msg_num_buf) - strlen(marker);
                strncpy(msg_plural_buf, msg_num_buf, len);
                msg_plural_buf[len] = 0;
                strcat(msg_plural_buf, marker + 3);
            }
            break;
        case LANGUAGE_JP:
            strcpy(msg_plural_buf, buf);
            break;
    }

    // BUG: when none of its language's markers is found (or the language has no case), msg_plural_buf still holds an
    // earlier call's text; language 0 copies buf itself, dropping the <NUM> substitution made in msg_num_buf.
    strcpy(buf, msg_plural_buf);
}

/**
 * US and later: builds a message with a number: args[2] is the message (id when args[0] is 0, literal text when 1),
 * args[3] the count; returns the text with the number and matching plural in args[1] (a shared static buffer).
 */
USER_FUNC(evt_msg_fmt_int) {
    EvtWord* args = event->args;
    s32 mode = args[0];
    s32 out = args[1];
    const char* src = (const char*)evtGetValue(event, args[2]);
    s32 count = evtGetValue(event, args[3]);
    const char* name;

    switch (mode) {
        case 0:
            name = msgSearch(src);
            break;
        case 1:
            name = src;
            break;
    }
    strcpy(msg_count_buf, name);
    evtMsgFmtInt(msg_count_buf, count);
    evtSetValue(event, out, (EvtWord)msg_count_buf);
    return EVT_RETURN_DONE;
}

// US and later: the scratch buffers are defined after the functions (their .bss follows the partyLabel statics, reversed).
char msg_article_buf[0xA00];
char msg_num_buf[0xA00];
char msg_plural_buf[0xA00];
char msg_count_buf[0xA00];
#endif
