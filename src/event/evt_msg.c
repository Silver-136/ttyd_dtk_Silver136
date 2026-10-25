#include "evt/evt_msg.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "driver/windowdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>
#include <stdio.h>

// US message buffer is twice as large
#ifdef VERSION_G8ME01
#define MSG_BUF_SIZE 0xA00
#else
#define MSG_BUF_SIZE 0x500
#endif

void evt_msg_init(void) {
    //stubbed in retail
}


s32 _evt_msg_print(EventEntry* event, BOOL isFirstCall, s32 flags, const char* msg, s32 a3, EvtWord name);
void msgWindow_Add(const char* msg, s32 id);

#include "evt/evt_npc.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
extern float __fabsf(float);

s32 msgWindow_Entry(const char* msg, s32 a2, s32 a3);
s32 BattleTransID(EventEntry* event, s32 type);
void BtlUnit_GetTalkTogePos(s32 unit, f32* x, f32* y, f32* z);

// the message window's MsgWinData lives in its userdata block
#define MSGW(w) ((MsgWinData*)(w)->userdata->address)

s32 _evt_msg_print(EventEntry* event, BOOL isFirstCall, s32 flags, const char* msg, s32 a3, EvtWord name) {
    NpcEntry* npc = NULL;
    WindowEntry* win;
    const char* m;
    if (isFirstCall) {
        if (!(flags & 1)) {
            m = msgSearch(msg);
        } else {
            m = msg;
            msg = "DIRECT";
        }
        event->printWindowId = msgWindow_Entry(m, a3, (s16)event->msgPriority);
        event->msgPriority = 0;
        win = windowGetPointer(event->printWindowId);
        MSGW(win)->tag = msg;
        MSGW(win)->event = event;
        if (!(flags & 2)) {
            if (event->wNpcEventType == 6) {
                npc = event->ownerNPC;
            }
            if ((const char*)name) {
                npc = evtNpcNameToPtr(event, (const char*)name);
            }
            if (npc == (NpcEntry*)-1) {
                if (!(event->printWindowFlags & 0x1EFFFFFF)) {
                    PartyEntry* p = partyGetPtr(marioGetPartyId());
                    if (!p) {
                        win->speakerPos = marioGetPtr()->position;
                        win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                    } else {
                        win->speakerPos = p->position;
                        win->speakerPos.y += p->height;
                    }
                }
                strcpy(MSGW(win)->speakerName, (const char*)name);
            } else if (npc == (NpcEntry*)-2) {
                if (!(event->printWindowFlags & 0x1EFFFFFF)) {
                    PartyEntry* p = partyGetPtr(marioGetExtraPartyId());
                    if (!p) {
                        win->speakerPos = marioGetPtr()->position;
                        win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                    } else {
                        win->speakerPos = p->position;
                        win->speakerPos.y += p->height;
                    }
                }
                strcpy(MSGW(win)->speakerName, (const char*)name);
            } else if (npc == (NpcEntry*)-3) {
                if (!(event->printWindowFlags & 0x1EFFFFFF)) {
                    win->speakerPos = marioGetPtr()->position;
                    win->speakerPos.y += marioGetPtr()->wPlayerCollisionBox.y;
                }
                strcpy(MSGW(win)->speakerName, (const char*)name);
            } else if (npc) {
                if (!(event->printWindowFlags & 0x1EFFFFFF)) {
                    win->speakerPos = npc->position;
                    win->speakerPos.y += npc->height;
                }
                strcpy(MSGW(win)->speakerName, npc->description);
            }
        } else {
            MSGW(win)->flags |= 8;
            MSGW(win)->unitId = BattleTransID(event, name);
            BtlUnit_GetTalkTogePos(MSGW(win)->unitId, &win->speakerPos.x, &win->speakerPos.y, &win->speakerPos.z);
            win->tailMode = 3;
        }
        switch (event->printWindowFlags & 0x1EFFFFFF) {
            case 0:
                if (npc) {
                    Vec scr, mscr;
                    marioGetPtr();
                    getScreenPoint(&win->speakerPos, &scr);
                    if (win->type == 1) {
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
            case 1:
                win->tailMode = 0;
                break;
            case 2:
                win->tailMode = 7;
                win->speakerPos = event->msgPosVec;
                break;
            case 3:
                win->tailMode = 2;
                win->speakerPos = event->msgPosVec;
                break;
        }
        if (event->printWindowFlags & 0x80000000) {
            win->tailMode = 3;
            if (event->printWindowFlags & 0x40000000) {
                win->tailMode |= 4;
            }
            if (event->printWindowFlags & 0x20000000) {
                win->tailMode |= 8;
            }
        }
        if (event->printWindowFlags & 0x01000000) {
            win->tailMode |= 0x10;
        }
        event->printWindowFlags = 0;
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_print) {
    EvtWord* args = event->args;
    s32 a = evtGetValue(event, args[0]);
    const char* b = (const char*)evtGetValue(event, args[1]);
    s32 c = evtGetValue(event, args[2]);
    EvtWord d = evtGetValue(event, args[3]);
    return _evt_msg_print(event, isFirstCall, a, b, c, d);
}

#include "party.h"
#include "mario/mario_party.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"

s32 msgWindow_Entry(const char* msg, s32 a2, s32 a3);

const char* partyMsgStr[] = {"", "_kur", "_nok", "_bom", "_yos", "_win", "_viv", "_chu", "_jug", "_tel", "_lui"};
const char* battlePartyMsgStr[] = {"_kur", "_nok", "_yos", "_win", "_viv", "_bom", "_chu"};
#ifndef VERSION_G8ME01
static const char* battlePartyMsgPad[6] = {0};
#endif

#ifdef VERSION_G8ME01
#include "mario/mariost.h"
// US-only localized article/plural helpers (evt_msg_fmt_int / evtMsgFmtInt, evt_msg_fmt_str / evtMsgFmtStr, below).
// clang-format off
const char* msg_de_article_nom_cap_tbl[6] = {"Ein ", "Eine ", "Ein ", "Die ", "Die ", "Die "};
const char* msg_de_article_nom_tbl[6] = {"ein ", "eine ", "ein ", "die ", "die ", "die "};
const char* msg_de_article_acc_cap_tbl[6] = {"Einen ", "Eine ", "Ein ", "Die ", "Die ", "Die "};
const char* msg_de_article_acc_tbl[6] = {"einen ", "eine ", "ein ", "die ", "die ", "die "};
const char* msg_fr_article_cap_tbl[3] = {"Un ", "Une ", "Les "};
const char* msg_fr_article_tbl[3] = {"un ", "une ", "les "};
const char* msg_es_article_cap_tbl[6] = {"Un ", "Una ", "", "", "Unos ", "Unas "};
const char* msg_es_article_tbl[6] = {"un ", "una ", "", "", "unos ", "unas "};

char* strstr(const char* s, const char* sub);

s32 evt_msg_fmt_int(EventEntry* event, BOOL isFirstCall);
void evtMsgFmtInt(char* buf, s32 count);
s32 evt_msg_fmt_str(EventEntry* event, BOOL isFirstCall);
void evtMsgFmtStr(char* buf, const char* name);

// clang-format on
#endif

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

USER_FUNC(evt_msg_print_battle_party) {
    static char partyLabel[0x40];
    const char* msg = (const char*)evtGetValue(event, *event->args);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, -4));
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    strcpy(partyLabel, msg);
    strcat(partyLabel, battlePartyMsgStr[(s32)unit->currentKind - TYPE_PARTNER_MIN]);
    return _evt_msg_print(event, isFirstCall, 2, partyLabel, 0, -4);
}

USER_FUNC(evt_msg_print_insert) {
    char buf[MSG_BUF_SIZE];
    s32 vals[4];
    s32* p;
    EvtWord* args = event->args;
    s32 n = (args[-2] >> 16) - 5;
    s32 flags = evtGetValue(event, *args++);
    const char* msg = (const char*)evtGetValue(event, *args++);
    s32 c, d;
    s32 i;
    c = evtGetValue(event, *args++);
    d = evtGetValue(event, *args++);
    for (i = 0, p = vals; i < n; i++) {
        *p++ = evtGetValue(event, *args++);
    }
    if (isFirstCall) {
        const char* m;
        if (flags & 1) {
            m = msg;
        } else {
            m = msgSearch(msg);
        }
        switch (n) {
            case 0: break;
            case 1: sprintf(buf, m, vals[0]); break;
            case 2: sprintf(buf, m, vals[0], vals[1]); break;
            case 3: sprintf(buf, m, vals[0], vals[1], vals[2]); break;
            case 4: sprintf(buf, m, vals[0], vals[1], vals[2], vals[3]); break;
        }
    }
    return _evt_msg_print(event, isFirstCall, flags | 1, buf, c, d);
}

USER_FUNC(evt_msg_print_add) {
    EvtWord* args = event->args;
    s32 flags = evtGetValue(event, args[0]);
    const char* msg = (const char*)evtGetValue(event, args[1]);
    if (isFirstCall) {
        if (!(flags & 1)) {
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
        if (flags & 1) {
            m = msg;
        } else {
            m = msgSearch(msg);
        }
        switch (n) {
            case 0: break;
            case 1: sprintf(buf, m, vals[0]); break;
            case 2: sprintf(buf, m, vals[0], vals[1]); break;
            case 3: sprintf(buf, m, vals[0], vals[1], vals[2]); break;
            case 4: sprintf(buf, m, vals[0], vals[1], vals[2], vals[3]); break;
        }
        msgWindow_Add(buf, event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

void msgWindow_ForceClose(s32 id);
void msgWindow_Repeat(s32 id);
void msgWindow_Continue(s32 id);

USER_FUNC(evt_msg_continue) {
    if (isFirstCall) {
        msgWindow_Continue(event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_repeat) {
    if (isFirstCall) {
        msgWindow_Repeat(event->printWindowId);
    }
    if (windowCheckID(event->printWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_close) {
    msgWindow_ForceClose(evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_select) {
    EvtWord* args = event->args;
    s32 flags = evtGetValue(event, args[0]);
    const char* msg = (const char*)evtGetValue(event, args[1]);
    if (isFirstCall) {
        const char* m;
        if (!(flags & 1)) {
            m = msgSearch(msg);
        } else {
            m = msg;
            msg = "DIRECT";
        }
        event->selectWindowId = msgWindow_Entry(m, 0, 0x14);
        MSGW(windowGetPointer(event->selectWindowId))->tag = msg;
    }
    if (windowCheckID(event->selectWindowId)) {
        return EVT_RETURN_BLOCK;
    }
    event->lwData[0] = MSGW(windowGetPointer(event->selectWindowId))->sel;
    windowDeleteID(event->selectWindowId);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_toge) {
    EvtWord* args = event->args;
    s32 v = evtGetValue(event, args[0]);
    event->printWindowFlags &= 0xE1000000;
    event->printWindowFlags |= v;
    event->msgPos.x = evtGetFloat(event, args[1]);
    event->msgPos.y = evtGetFloat(event, args[2]);
    event->msgPosZf = evtGetFloat(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_togelr) {
    s32 v = evtGetValue(event, *event->args);
    event->printWindowFlags |= 0x80000000;
    switch (v) {
        case 0:
            break;
        case 1:
            event->printWindowFlags |= 0x40000000;
            break;
        case 2:
            event->printWindowFlags |= 0x20000000;
            break;
        case 3:
            event->printWindowFlags |= 0x60000000;
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_pri) {
    event->msgPriority = evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_msg_npc) {
    const char* name = (const char*)evtGetValue(event, *event->args);
    strcpy(MSGW(windowGetPointer(event->printWindowId))->speakerName, name);
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
// US-only localized article/plural helpers. The US build emits functions in reverse source
// order, so these sit at the end of the file to come first in .text.
void evtMsgFmtStr(char* buf, const char* name) {
    char tmp[0xA00];
    u32 len;
    char* marker;
    const char* article;
    s32 form;

    switch (gp->language) {
    case 1:
    case 6:
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
    case 2:
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
        } else if ((marker = strstr(buf, "<ITEM>")) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 3:
        form = 0;
        if (strstr(name, "<M>") != NULL) {
            form = 0;
        } else if (strstr(name, "<F>") != NULL) {
            form = 1;
        } else if (strstr(name, "<PLles>") != NULL) {
            form = 2;
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
    case 4:
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
    case 5:
        if ((marker = strstr(buf, "<ITEM>")) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 0:
        strcpy(tmp, buf);
        break;
    }
    strcpy(buf, tmp);
}

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
    case 1:
    case 4:
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
    case 3:
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
    case 2:
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
    case 5:
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
    case 0:
        strcpy(msg_plural_buf, buf);
        break;
    }
    strcpy(buf, msg_plural_buf);
}

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
#endif

#ifdef VERSION_G8ME01
// US: the scratch buffers are defined after the functions (their .bss follows the partyLabel statics, reversed).
char msg_article_buf[0xA00];
char msg_num_buf[0xA00];
char msg_plural_buf[0xA00];
char msg_count_buf[0xA00];
#endif
