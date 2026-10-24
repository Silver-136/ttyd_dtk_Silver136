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
// Their article strings and tables are the US data by name.
// clang-format off
#pragma force_active on // FAKE: the helpers reach most of these strings as words + offset from one base,
                        // so force_active keeps the unnamed ones in the pool
const char str_Einen_802cc608[8] = {0x45, 0x69, 0x6E, 0x65, 0x6E, 0x20, 0x00, 0x00};
const char str_einen_802cc610[8] = {0x65, 0x69, 0x6E, 0x65, 0x6E, 0x20, 0x00, 0x00};
const char str_COIN_802cc618[8] = {0x3C, 0x43, 0x4F, 0x49, 0x4E, 0x3E, 0x00, 0x00};
const char str_M_nzen_802cc620[8] = {0x4D, 0xFC, 0x6E, 0x7A, 0x65, 0x6E, 0x00, 0x00};
const char str_POINT_802cc628[8] = {0x3C, 0x50, 0x4F, 0x49, 0x4E, 0x54, 0x3E, 0x00};
const char str_Punkte_802cc630[8] = {0x50, 0x75, 0x6E, 0x6B, 0x74, 0x65, 0x00, 0x00};
const char str_moneta_802cc638[8] = {0x6D, 0x6F, 0x6E, 0x65, 0x74, 0x61, 0x00, 0x00};
const char str_monete_802cc640[8] = {0x6D, 0x6F, 0x6E, 0x65, 0x74, 0x65, 0x00, 0x00};
const char str_point_802cc648[8] = {0x3C, 0x70, 0x6F, 0x69, 0x6E, 0x74, 0x3E, 0x00};
const char str_OGGETTO_802cc650[12] = {0x3C, 0x4F, 0x47, 0x47, 0x45, 0x54, 0x54, 0x4F, 0x3E, 0x00, 0x00, 0x00};
const char str_oggetto_802cc65c[8] = {0x6F, 0x67, 0x67, 0x65, 0x74, 0x74, 0x6F, 0x00};
const char str_oggtti_802cc664[8] = {0x6F, 0x67, 0x67, 0x74, 0x74, 0x69, 0x00, 0x00};
const char str_ITEM_802cc66c[8] = {0x3C, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00};
const char str_AN_ITEM_802cc674[12] = {0x3C, 0x41, 0x4E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00, 0x00};
const char str_MPLdie_802cc680[12] = {0x3C, 0x4D, 0x50, 0x4C, 0x64, 0x69, 0x65, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_FPLdie_802cc68c[12] = {0x3C, 0x46, 0x50, 0x4C, 0x64, 0x69, 0x65, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_NPLdie_802cc698[12] = {0x3C, 0x4E, 0x50, 0x4C, 0x64, 0x69, 0x65, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_Einen_802cc6a4[8] = {0x3C, 0x45, 0x69, 0x6E, 0x65, 0x6E, 0x3E, 0x00};
const char str_Ein_ITEM_802cc6ac[12] = {0x3C, 0x45, 0x69, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00};
const char str_ein_ITEM_802cc6b8[12] = {0x3C, 0x65, 0x69, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00};
const char str_Einen_ITEM_802cc6c4[16] = {0x3C, 0x45, 0x69, 0x6E, 0x65, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_einen_ITEM_802cc6d4[16] = {0x3C, 0x65, 0x69, 0x6E, 0x65, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_PLles_802cc6e4[8] = {0x3C, 0x50, 0x4C, 0x6C, 0x65, 0x73, 0x3E, 0x00};
const char str_Un_ITEM_802cc6ec[12] = {0x3C, 0x55, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00, 0x00};
const char str_un_ITEM_802cc6f8[12] = {0x3C, 0x75, 0x6E, 0x5F, 0x49, 0x54, 0x45, 0x4D, 0x3E, 0x00, 0x00, 0x00};
const char str_PLunos_802cc704[12] = {0x3C, 0x50, 0x4C, 0x75, 0x6E, 0x6F, 0x73, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_PLunas_802cc710[12] = {0x3C, 0x50, 0x4C, 0x75, 0x6E, 0x61, 0x73, 0x3E, 0x00, 0x00, 0x00, 0x00};
const char str_Ein_80421764[] = "Ein ";
const char str_Eine_8042176c[] = "Eine ";
const char str_Die_80421774[] = "Die ";
const char str_ein_8042177c[] = "ein ";
const char str_eine_80421784[] = "eine ";
const char str_die_8042178c[] = "die ";
const char str_Un_80421794[4] = {0x55, 0x6E, 0x20, 0x00};
const char str_Une_80421798[] = "Une ";
const char str_Les_804217a0[] = "Les ";
const char str_un_804217a8[4] = {0x75, 0x6E, 0x20, 0x00};
const char str_une_804217ac[] = "une ";
const char str_les_804217b4[] = "les ";
const char str_Una_804217bc[] = "Una ";
const char str_Unos_804217c4[] = "Unos ";
const char str_Unas_804217cc[] = "Unas ";
const char str_una_804217d4[] = "una ";
const char str_unos_804217dc[] = "unos ";
const char str_unas_804217e4[] = "unas ";
const char str_NUM_804217ec[] = "<NUM>";
const char str_PCTd_804217f4[4] = {0x25, 0x64, 0x00, 0x00};
const char str_S_804217f8[4] = {0x3C, 0x53, 0x3E, 0x00};
const char str_s_804217fc[4] = {0x73, 0x00, 0x00, 0x00};
const char str_N_80421800[4] = {0x3C, 0x4E, 0x3E, 0x00};
const char str_X_80421804[4] = {0x3C, 0x58, 0x3E, 0x00};
const char str_x_80421808[4] = {0x78, 0x00, 0x00, 0x00};
const char str_M_nze_8042180c[] = "M\xFCnze";
const char str_Punkt_80421814[] = "Punkt";
const char str_punto_8042181c[] = "punto";
const char str_punti_80421824[] = "punti";
const char str_Punto_8042182c[] = "Punto";
const char str_Punti_80421834[] = "Punti";
const char str_an_8042183c[4] = {0x61, 0x6E, 0x20, 0x00};
const char str_a_80421840[4] = {0x61, 0x20, 0x00, 0x00};
const char str_AN_80421844[] = "<AN>";
const char str_M_8042184c[4] = {0x3C, 0x4D, 0x3E, 0x00};
const char str_F_80421850[4] = {0x3C, 0x46, 0x3E, 0x00};
const char str_un_80421854[] = "<un>";
const char str_MNA_8042185c[] = "<MNA>";
const char str_FNA_80421864[] = "<FNA>";
const char* msg_de_article_nom_cap_tbl[6] = {(const char*)str_Ein_80421764, (const char*)str_Eine_8042176c, (const char*)str_Ein_80421764, (const char*)str_Die_80421774, (const char*)str_Die_80421774, (const char*)str_Die_80421774};
const char* msg_de_article_nom_tbl[6] = {(const char*)str_ein_8042177c, (const char*)str_eine_80421784, (const char*)str_ein_8042177c, (const char*)str_die_8042178c, (const char*)str_die_8042178c, (const char*)str_die_8042178c};
const char* msg_de_article_acc_cap_tbl[6] = {(const char*)str_Einen_802cc608, (const char*)str_Eine_8042176c, (const char*)str_Ein_80421764, (const char*)str_Die_80421774, (const char*)str_Die_80421774, (const char*)str_Die_80421774};
const char* msg_de_article_acc_tbl[6] = {(const char*)str_einen_802cc610, (const char*)str_eine_80421784, (const char*)str_ein_8042177c, (const char*)str_die_8042178c, (const char*)str_die_8042178c, (const char*)str_die_8042178c};
const char* msg_fr_article_cap_tbl[3] = {(const char*)str_Un_80421794, (const char*)str_Une_80421798, (const char*)str_Les_804217a0};
const char* msg_fr_article_tbl[3] = {(const char*)str_un_804217a8, (const char*)str_une_804217ac, (const char*)str_les_804217b4};
const char* msg_es_article_cap_tbl[1] = {(const char*)str_Un_80421794};
const char* msg_es_article_cap_tbl_tail[5] = {(const char*)str_Una_804217bc, "", "", (const char*)str_Unos_804217c4, (const char*)str_Unas_804217cc};
const char* msg_es_article_tbl[6] = {(const char*)str_un_804217a8, (const char*)str_una_804217d4, "", "", (const char*)str_unos_804217dc, (const char*)str_unas_804217e4};

#pragma force_active reset
char* strcat(char* dst, const char* src);
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
	strcat(partyLabel, battlePartyMsgStr[(s32)unit->currentKind - 0xE0]);
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
    const char* words = str_Einen_802cc608; /* FAKE: one base for the pooled .rodata strings */
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
            article = str_an_8042183c;
            break;
        default:
            article = str_a_80421840;
            break;
        }
        if ((marker = strstr(buf, str_AN_80421844)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, article);
            strcat(tmp, marker + 4);
            strcpy(buf, tmp);
            if ((marker = strstr(buf, words + 0x64)) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
        } else if ((marker = strstr(buf, words + 0x6C)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, article);
            strcat(tmp, name);
            strcat(tmp, marker + 9);
        } else if ((marker = strstr(buf, words + 0x64)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 2:
        form = 0;
        if (strstr(name, str_M_8042184c) != NULL) {
            form = 0;
        } else if (strstr(name, str_F_80421850) != NULL) {
            form = 1;
        } else if (strstr(name, str_N_80421800) != NULL) {
            form = 2;
        } else if (strstr(name, words + 0x78) != NULL) {
            form = 3;
        } else if (strstr(name, words + 0x84) != NULL) {
            form = 4;
        } else if (strstr(name, words + 0x90) != NULL) {
            form = 5;
        }
        if ((marker = strstr(buf, words + 0x9C)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_de_article_acc_cap_tbl[form]);
            strcat(tmp, marker + 7);
            strcpy(buf, tmp);
            if ((marker = strstr(buf, words + 0x64)) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
        } else if ((marker = strstr(buf, words + 0xA4)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_de_article_nom_cap_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 10);
        } else if ((marker = strstr(buf, words + 0xB0)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_de_article_nom_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 10);
        } else if ((marker = strstr(buf, words + 0xBC)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_de_article_acc_cap_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 12);
        } else if ((marker = strstr(buf, words + 0xCC)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_de_article_acc_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 12);
        } else if ((marker = strstr(buf, words + 0x64)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 3:
        form = 0;
        if (strstr(name, str_M_8042184c) != NULL) {
            form = 0;
        } else if (strstr(name, str_F_80421850) != NULL) {
            form = 1;
        } else if (strstr(name, words + 0xDC) != NULL) {
            form = 2;
        }
        if ((marker = strstr(buf, str_un_80421854)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_fr_article_tbl[form]);
            strcat(tmp, marker + 4);
            strcpy(buf, tmp);
            if ((marker = strstr(buf, words + 0x64)) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
        } else if ((marker = strstr(buf, words + 0xE4)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_fr_article_cap_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 9);
        } else if ((marker = strstr(buf, words + 0xF0)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_fr_article_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 9);
        } else if ((marker = strstr(buf, words + 0x64)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 4:
        form = 0;
        if (strstr(name, str_M_8042184c) != NULL) {
            form = 0;
        } else if (strstr(name, str_F_80421850) != NULL) {
            form = 1;
        } else if (strstr(name, str_MNA_8042185c) != NULL) {
            form = 2;
        } else if (strstr(name, str_FNA_80421864) != NULL) {
            form = 3;
        } else if (strstr(name, words + 0xFC) != NULL) {
            form = 4;
        } else if (strstr(name, words + 0x108) != NULL) {
            form = 5;
        }
        if ((marker = strstr(buf, str_un_80421854)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_es_article_tbl[form]);
            strcat(tmp, marker + 4);
            strcpy(buf, tmp);
            if ((marker = strstr(buf, words + 0x64)) != NULL) {
                len = strlen(buf) - strlen(marker);
                strncpy(tmp, buf, len);
                tmp[len] = 0;
                strcat(tmp, name);
                strcat(tmp, marker + 6);
            }
        } else if ((marker = strstr(buf, words + 0xE4)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_es_article_cap_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 9);
        } else if ((marker = strstr(buf, words + 0xF0)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, msg_es_article_tbl[form]);
            strcat(tmp, name);
            strcat(tmp, marker + 9);
        } else if ((marker = strstr(buf, words + 0x64)) != NULL) {
            len = strlen(buf) - strlen(marker);
            strncpy(tmp, buf, len);
            tmp[len] = 0;
            strcat(tmp, name);
            strcat(tmp, marker + 6);
        }
        break;
    case 5:
        if ((marker = strstr(buf, words + 0x64)) != NULL) {
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
    const char* words = str_Einen_802cc608; /* FAKE: one base for the pooled .rodata strings */
    char* marker;
    u32 len;
    char num[16];
    u32 numLen;
    char* found;

    found = strstr(buf, str_NUM_804217ec);
    if (found == NULL) {
        return;
    }
    numLen = strlen(buf) - strlen(found);
    strncpy(msg_num_buf, buf, numLen);
    msg_num_buf[numLen] = 0;
    sprintf(num, str_PCTd_804217f4, count);
    strcat(msg_num_buf, num);
    strcat(msg_num_buf, found + 5);

    switch (gp->language) {
    case 1:
    case 4:
    case 6:
        if ((marker = strstr(msg_num_buf, str_S_804217f8)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count != 1) {
                strcat(msg_plural_buf, str_s_804217fc);
            }
            strcat(msg_plural_buf, marker + 3);
        } else if ((marker = strstr(msg_num_buf, str_N_80421800)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            strcat(msg_plural_buf, marker + 3);
        }
        break;
    case 3:
        if ((marker = strstr(msg_num_buf, str_X_80421804)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count > 1) {
                strcat(msg_plural_buf, str_x_80421808);
            }
            strcat(msg_plural_buf, marker + 3);
        } else if ((marker = strstr(msg_num_buf, str_S_804217f8)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count > 1) {
                strcat(msg_plural_buf, str_s_804217fc);
            }
            strcat(msg_plural_buf, marker + 3);
        } else if ((marker = strstr(msg_num_buf, str_N_80421800)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            strcat(msg_plural_buf, marker + 3);
        }
        break;
    case 2:
        if ((marker = strstr(msg_num_buf, words + 0x10)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, str_M_nze_8042180c);
            } else {
                strcat(msg_plural_buf, words + 0x18);
            }
            strcat(msg_plural_buf, marker + 6);
        } else if ((marker = strstr(msg_num_buf, words + 0x20)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, str_Punkt_80421814);
            } else {
                strcat(msg_plural_buf, words + 0x28);
            }
            strcat(msg_plural_buf, marker + 7);
        } else if ((marker = strstr(msg_num_buf, str_S_804217f8)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count != 1) {
                strcat(msg_plural_buf, str_s_804217fc);
            }
            strcat(msg_plural_buf, marker + 3);
        } else if ((marker = strstr(msg_num_buf, str_N_80421800)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            strcat(msg_plural_buf, marker + 3);
        }
        break;
    case 5:
        if ((marker = strstr(msg_num_buf, words + 0x10)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, words + 0x30);
            } else {
                strcat(msg_plural_buf, words + 0x38);
            }
            strcat(msg_plural_buf, marker + 6);
        } else if ((marker = strstr(msg_num_buf, words + 0x40)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, str_punto_8042181c);
            } else {
                strcat(msg_plural_buf, str_punti_80421824);
            }
            strcat(msg_plural_buf, marker + 7);
        } else if ((marker = strstr(msg_num_buf, words + 0x20)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, str_Punto_8042182c);
            } else {
                strcat(msg_plural_buf, str_Punti_80421834);
            }
            strcat(msg_plural_buf, marker + 7);
        } else if ((marker = strstr(msg_num_buf, words + 0x48)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count == 1) {
                strcat(msg_plural_buf, words + 0x54);
            } else {
                strcat(msg_plural_buf, words + 0x5C);
            }
            strcat(msg_plural_buf, marker + 7);
        } else if ((marker = strstr(msg_num_buf, str_S_804217f8)) != NULL) {
            len = strlen(msg_num_buf) - strlen(marker);
            strncpy(msg_plural_buf, msg_num_buf, len);
            msg_plural_buf[len] = 0;
            if (count != 1) {
                strcat(msg_plural_buf, str_s_804217fc);
            }
            strcat(msg_plural_buf, marker + 3);
        } else if ((marker = strstr(msg_num_buf, str_N_80421800)) != NULL) {
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
