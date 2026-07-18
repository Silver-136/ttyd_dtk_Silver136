#include "event/evt_msg.h"

typedef struct EvtEntry {
    u8 pad_0[0x18];
    s32* args;
    u8 pad_1[0x15C];
    s32 msgWindowId;
    u8 pad_2[0x14];
    s32 value;
} EvtEntry;

s32 evtGetValue(EvtEntry* evt, s32 value);
void msgWindow_ForceClose(s32 id);
void msgWindow_Continue(s32 id);
s32 windowCheckID(s32 id);

void evt_msg_init(void) {
    ;
}

s32 evt_msg_pri(EvtEntry* evt) {
    evt->value = evtGetValue(evt, evt->args[0]);
    return 2;
}

s32 evt_msg_close(EvtEntry* evt) {
    msgWindow_ForceClose(evtGetValue(evt, evt->args[0]));
    return 2;
}

s32 evt_msg_continue(EvtEntry* evt, s32 first) {
    if (first != 0) {
        msgWindow_Continue(evt->msgWindowId);
    }
    if (windowCheckID(evt->msgWindowId) != 0) {
        return 0;
    }
    return 2;
}

s32 unk_800d0998(EvtEntry* evt) {
    extern char* msgSearch(char* msg);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d0a4c(char* dst, char* value);
    extern void evtSetValue(EvtEntry* evt, s32 index, s32 value);
    extern char unk_803dccc8[];
    s32* args = evt->args;
    s32 mode = args[0];
    s32 dst = args[1];
    char* msg = (char*)evtGetValue(evt, args[2]);
    char* value = (char*)evtGetValue(evt, args[3]);
    char* selected;

    switch (mode) {
        case 0:
            selected = msgSearch(msg);
            break;
        case 1:
            selected = msg;
            break;
    }
    strcpy(unk_803dccc8, selected);
    unk_800d0a4c(unk_803dccc8, value);
    evtSetValue(evt, dst, (s32)unk_803dccc8);
    return 2;
}

s32 unk_800d12b0(EvtEntry* evt) {
    extern char* msgSearch(char* msg);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d1364(char* dst, char* value);
    extern void evtSetValue(EvtEntry* evt, s32 index, s32 value);
    extern char unk_803deac8[];
    s32* args = evt->args;
    s32 mode = args[0];
    s32 dst = args[1];
    char* msg = (char*)evtGetValue(evt, args[2]);
    char* value = (char*)evtGetValue(evt, args[3]);
    char* selected;

    switch (mode) {
        case 0:
            selected = msgSearch(msg);
            break;
        case 1:
            selected = msg;
            break;
    }
    strcpy(unk_803deac8, selected);
    unk_800d1364(unk_803deac8, value);
    evtSetValue(evt, dst, (s32)unk_803deac8);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _evt_msg_print(void* param_1, int param_2, u32 param_3, char* param_4, s32 param_5, char* param_6) {
    extern char* msgSearch(char* msg);
    extern s32 msgWindow_Entry(char* msg, s32 type, s16 flags);
    extern void* windowGetPointer(s32 id);
    extern void* evtNpcNameToPtr(void* evt, char* name);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* marioGetPtr(void);
    extern char* strcpy(char* dst, const char* src);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void BtlUnit_GetTalkTogePos(s32 unitIdx, f32* x, f32* y, f32* z);
    extern f32 getScreenPoint(f32* src, f32* dst);
    extern f32 __fabsf(f32 value);
    extern char str_DIRECT_802cc71c[];
    extern f32 float_300_80421874;

    void* evt;
    void* win;
    void* workHead;
    void* work;
    void* npc;
    void* party;
    void* mario;
    char* searchMsg;
    char* displayMsg;
    u32 mode;
    s32 unitId;
    f32 screenWin[5];
    f32 screenMario[3];

    evt = param_1;
    npc = 0;

    if (param_2 != 0) {
        if ((param_3 & 1) == 0) {
            searchMsg = msgSearch(param_4);
            displayMsg = param_4;
        } else {
            searchMsg = param_4;
            displayMsg = str_DIRECT_802cc71c;
        }

        *(s32*)((s32)evt + 0x178) = msgWindow_Entry(searchMsg, param_5, *(s16*)((s32)evt + 0x190));
        *(s32*)((s32)evt + 0x190) = 0;
        win = windowGetPointer(*(s32*)((s32)evt + 0x178));
        workHead = *(void**)((s32)win + 0x28);
        work = *(void**)workHead;
        *(char**)work = displayMsg;
        *(void**)((s32)work + 0xF200) = evt;

        if ((param_3 & 2) == 0) {
            if (*(u8*)((s32)evt + 0x10) == 6) {
                npc = *(void**)((s32)evt + 0x170);
            }
            if (param_6 != 0) {
                npc = evtNpcNameToPtr(evt, param_6);
            }

            if ((s32)npc == -1) {
                if ((*(u32*)((s32)evt + 0x180) & 0x1EFFFFFF) == 0) {
                    party = partyGetPtr(marioGetPartyId());
                    if (party == 0) {
                        mario = marioGetPtr();
                        *(u32*)((s32)win + 0x30) = *(u32*)((s32)mario + 0x8C);
                        *(u32*)((s32)win + 0x34) = *(u32*)((s32)mario + 0x90);
                        *(u32*)((s32)win + 0x38) = *(u32*)((s32)mario + 0x94);
                        mario = marioGetPtr();
                        *(f32*)((s32)win + 0x34) += *(f32*)((s32)mario + 0x1BC);
                    } else {
                        *(u32*)((s32)win + 0x30) = *(u32*)((s32)party + 0x58);
                        *(u32*)((s32)win + 0x34) = *(u32*)((s32)party + 0x5C);
                        *(u32*)((s32)win + 0x38) = *(u32*)((s32)party + 0x60);
                        *(f32*)((s32)win + 0x34) += *(f32*)((s32)party + 0xF0);
                    }
                }
                strcpy((char*)((s32)work + 0xF204), param_6);
            } else if ((s32)npc == -2) {
                if ((*(u32*)((s32)evt + 0x180) & 0x1EFFFFFF) == 0) {
                    party = partyGetPtr(marioGetExtraPartyId());
                    if (party == 0) {
                        mario = marioGetPtr();
                        *(u32*)((s32)win + 0x30) = *(u32*)((s32)mario + 0x8C);
                        *(u32*)((s32)win + 0x34) = *(u32*)((s32)mario + 0x90);
                        *(u32*)((s32)win + 0x38) = *(u32*)((s32)mario + 0x94);
                        mario = marioGetPtr();
                        *(f32*)((s32)win + 0x34) += *(f32*)((s32)mario + 0x1BC);
                    } else {
                        *(u32*)((s32)win + 0x30) = *(u32*)((s32)party + 0x58);
                        *(u32*)((s32)win + 0x34) = *(u32*)((s32)party + 0x5C);
                        *(u32*)((s32)win + 0x38) = *(u32*)((s32)party + 0x60);
                        *(f32*)((s32)win + 0x34) += *(f32*)((s32)party + 0xF0);
                    }
                }
                strcpy((char*)((s32)work + 0xF204), param_6);
            } else if ((s32)npc == -3) {
                if ((*(u32*)((s32)evt + 0x180) & 0x1EFFFFFF) == 0) {
                    mario = marioGetPtr();
                    *(u32*)((s32)win + 0x30) = *(u32*)((s32)mario + 0x8C);
                    *(u32*)((s32)win + 0x34) = *(u32*)((s32)mario + 0x90);
                    *(u32*)((s32)win + 0x38) = *(u32*)((s32)mario + 0x94);
                    mario = marioGetPtr();
                    *(f32*)((s32)win + 0x34) += *(f32*)((s32)mario + 0x1BC);
                }
                strcpy((char*)((s32)work + 0xF204), param_6);
            } else if (npc != 0) {
                if ((*(u32*)((s32)evt + 0x180) & 0x1EFFFFFF) == 0) {
                    *(u32*)((s32)win + 0x30) = *(u32*)((s32)npc + 0x8C);
                    *(u32*)((s32)win + 0x34) = *(u32*)((s32)npc + 0x90);
                    *(u32*)((s32)win + 0x38) = *(u32*)((s32)npc + 0x94);
                    *(f32*)((s32)win + 0x34) += *(f32*)((s32)npc + 0x150);
                }
                strcpy((char*)((s32)work + 0xF204), (char*)((s32)npc + 8));
            }
        } else {
            *(u32*)((s32)work + 4) |= 8;
            unitId = BattleTransID(evt, (s32)param_6);
            *(s32*)((s32)work + 0xF224) = unitId;
            BtlUnit_GetTalkTogePos(*(s32*)((s32)work + 0xF224), (f32*)((s32)win + 0x30),
                                   (f32*)((s32)win + 0x34), (f32*)((s32)win + 0x38));
            *(u16*)((s32)win + 0x2C) = 3;
        }

        mode = *(u32*)((s32)evt + 0x180) & 0x1EFFFFFF;
        if (mode == 2) {
            *(u16*)((s32)win + 0x2C) = 7;
            *(u32*)((s32)win + 0x30) = *(u32*)((s32)evt + 0x184);
            *(u32*)((s32)win + 0x34) = *(u32*)((s32)evt + 0x188);
            *(u32*)((s32)win + 0x38) = *(u32*)((s32)evt + 0x18C);
        } else if (mode < 2) {
            if (mode == 0) {
                if (npc != 0) {
                    marioGetPtr();
                    getScreenPoint((f32*)((s32)win + 0x30), screenWin);
                    if (*(s32*)((s32)win + 8) == 1) {
                        *(u16*)((s32)win + 0x2C) = 7;
                    } else if (__fabsf(screenWin[0]) >= float_300_80421874) {
                        *(u16*)((s32)win + 0x2C) = 2;
                    } else {
                        mario = marioGetPtr();
                        getScreenPoint((f32*)((s32)mario + 0x8C), screenMario);
                        if (screenMario[0] <= screenWin[0]) {
                            *(u16*)((s32)win + 0x2C) = 7;
                        } else {
                            *(u16*)((s32)win + 0x2C) = 3;
                        }
                    }
                }
            } else {
                *(u16*)((s32)win + 0x2C) = 0;
            }
        } else if (mode < 4) {
            *(u16*)((s32)win + 0x2C) = 2;
            *(u32*)((s32)win + 0x30) = *(u32*)((s32)evt + 0x184);
            *(u32*)((s32)win + 0x34) = *(u32*)((s32)evt + 0x188);
            *(u32*)((s32)win + 0x38) = *(u32*)((s32)evt + 0x18C);
        }

        if ((*(u32*)((s32)evt + 0x180) & 0x80000000) != 0) {
            *(u16*)((s32)win + 0x2C) = 3;
            if ((*(u32*)((s32)evt + 0x180) & 0x40000000) != 0) {
                *(u16*)((s32)win + 0x2C) |= 4;
            }
            if ((*(u32*)((s32)evt + 0x180) & 0x20000000) != 0) {
                *(u16*)((s32)win + 0x2C) |= 8;
            }
        }
        if ((*(u32*)((s32)evt + 0x180) & 0x01000000) != 0) {
            *(u16*)((s32)win + 0x2C) |= 0x10;
        }
        *(u32*)((s32)evt + 0x180) = 0;
    }

    if (windowCheckID(*(s32*)((s32)evt + 0x178)) == 0) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_msg_print_insert(int param_1, int param_2) {
    extern int sprintf(char* str, const char* format, ...);
    extern char* msgSearch(char* msg);

    s32 values[4];
    char buffer[0xA00];
    EvtEntry* evt;
    s32 first;
    s32* args;
    s32* extraArgs;
    s32 count;
    s32 flags;
    char* msg;
    s32 value1;
    s32 value2;
    s32 i;

    evt = (EvtEntry*)param_1;
    first = param_2;
    args = evt->args;
    count = (args[-2] >> 16) - 5;
    flags = evtGetValue(evt, args[0]);
    msg = (char*)evtGetValue(evt, args[1]);
    extraArgs = args + 2;
    value1 = evtGetValue(evt, extraArgs[0]);
    value2 = evtGetValue(evt, extraArgs[1]);
    extraArgs += 2;
    args = values;
    i = 0;
    while (i < count) {
        *args = evtGetValue(evt, *extraArgs);
        extraArgs++;
        args++;
        i++;
    }
    if (first != 0) {
        if ((flags & 1) != 0) {
            msg = msg;
        } else {
            msg = msgSearch(msg);
        }
        switch (count) {
            case 1:
                sprintf(buffer, msg, values[0]);
                break;
            case 2:
                sprintf(buffer, msg, values[0], values[1]);
                break;
            case 3:
                sprintf(buffer, msg, values[0], values[1], values[2]);
                break;
            case 4:
                sprintf(buffer, msg, values[0], values[1], values[2], values[3]);
                break;
            case 0:
            default:
                break;
        }
    }
    return _evt_msg_print(evt, first, flags | 1, buffer, value1, (char*)value2);
}

s32 evt_msg_print_add_insert(int param_1, int param_2) {
    extern int sprintf(char* str, const char* format, ...);
    extern char* msgSearch(char* msg);
    extern u8 msgWindow_Add(char* msg, s32 windowId);
    s32 values[4];
    char buffer[0xA00];
    EvtEntry* evt = (EvtEntry*)param_1;
    s32 first = param_2;
    s32* args = evt->args;
    s32 count = (args[-2] >> 16) - 3;
    s32 flags;
    char* msg;
    s32 i;

    flags = evtGetValue(evt, args[0]);
    msg = (char*)evtGetValue(evt, args[1]);
    for (i = 0; i < count; i++) {
        values[i] = evtGetValue(evt, args[i + 2]);
    }
    if (first != 0) {
        if (!(flags & 1)) {
            msg = msgSearch(msg);
        }
        switch (count) {
            case 1:
                sprintf(buffer, msg, values[0]);
                break;
            case 2:
                sprintf(buffer, msg, values[0], values[1]);
                break;
            case 3:
                sprintf(buffer, msg, values[0], values[1], values[2]);
                break;
            case 4:
                sprintf(buffer, msg, values[0], values[1], values[2], values[3]);
                break;
            case 0:
            default:
                break;
        }
        msgWindow_Add(buffer, evt->msgWindowId);
    }
    if (windowCheckID(evt->msgWindowId) != 0) {
        return 0;
    }
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_msg_select(void* pEvt, int param_2) {
    extern char* msgSearch(char* msg);
    extern s32 msgWindow_Entry(char* msg, s32 type, s32 flags);
    extern void* windowGetPointer(s32 id);
    extern void windowDeleteID(s32 id);
    extern char str_DIRECT_802cc71c[];
    EvtEntry* evt = pEvt;
    s32 first = param_2;
    s32* args = evt->args;
    s32 flags;
    char* msg;
    char* displayMsg;
    void* window;
    void* selectWork;

    flags = evtGetValue(evt, args[0]);
    msg = (char*)evtGetValue(evt, args[1]);
    displayMsg = msg;
    if (first != 0) {
        if (!(flags & 1)) {
            msg = msgSearch(msg);
        } else {
            displayMsg = str_DIRECT_802cc71c;
        }
        *(s32*)((s32)evt + 0x17C) = msgWindow_Entry(msg, 0, 0x14);
        window = windowGetPointer(*(s32*)((s32)evt + 0x17C));
        selectWork = *(void**)((s32)window + 0x28);
        *(char**)selectWork = displayMsg;
    }

    if (windowCheckID(*(s32*)((s32)evt + 0x17C)) != 0) {
        return 0;
    }
    window = windowGetPointer(*(s32*)((s32)evt + 0x17C));
    selectWork = *(void**)((s32)window + 0x28);
    *(s32*)((s32)evt + 0x9C) = *(s8*)(*(s32*)selectWork + 0xF243);
    windowDeleteID(*(s32*)((s32)evt + 0x17C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_msg_print_party_add(int param_1, int param_2) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern char* msgSearch(char* msg);
    extern u8 msgWindow_Add(char* msg, s32 windowId);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern char* partyMsgStr[11];
    static char partyLabel[0x40];
    EvtEntry* evt = (EvtEntry*)param_1;
    s32 first = param_2;
    void* party;
    char* msg;
    s32 kind;

    msg = (char*)evtGetValue(evt, evt->args[0]);
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        kind = *(s8*)((s32)party + 0x31);
    } else {
        kind = 0;
    }
    if (first != 0) {
        strcpy(partyLabel, msg);
        strcat(partyLabel, partyMsgStr[kind]);
        msgWindow_Add(msgSearch(partyLabel), evt->msgWindowId);
    }
    if (windowCheckID(evt->msgWindowId) != 0) {
        return 0;
    }
    return 2;
}


s32 evt_msg_print_battle_party(int param_1, int param_2) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 BattleTransID(s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void* _battleWorkPointer;
    extern char* battlePartyMsgStr[7];
    static char partyLabel[0x40];
    EvtEntry* evt = (EvtEntry*)param_1;
    s32 first = param_2;
    s32 unitId;
    char* msg;
    void* unit;

    msg = (char*)evtGetValue(evt, evt->args[0]);
    unitId = BattleTransID(-4);
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    if (unit == 0) {
        return 2;
    }
    strcpy(partyLabel, msg);
    strcat(partyLabel, battlePartyMsgStr[*(s32*)((s32)unit + 8)]);
    return _evt_msg_print(evt, first, 2, partyLabel, 0, (char*)-4);
}


s32 evt_msg_print_party(void* pEvt, int param_2) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern char* partyMsgStr[11];
    extern char str_party_8042186c[];
    static char partyLabel[0x40];
    EvtEntry* evt = pEvt;
    s32 first = param_2;
    void* party;
    char* msg;
    s32 kind;

    msg = (char*)evtGetValue(evt, evt->args[0]);
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        kind = *(s8*)((s32)party + 0x31);
    } else {
        kind = 0;
    }
    strcpy(partyLabel, msg);
    strcat(partyLabel, partyMsgStr[kind]);
    return _evt_msg_print(evt, first, 0, partyLabel, 0, str_party_8042186c);
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_msg_print_add(void* pEvt, int param_2) {
    extern char* msgSearch(char* msg);
    extern u8 msgWindow_Add(char* msg, s32 windowId);
    s32 first;
    EvtEntry* evt;
    s32 flags;
    s32* args;
    s32 status;
    s32 mask;
    char* msg;

    first = param_2;
    evt = pEvt;
    args = evt->args;
    flags = evtGetValue(evt, args[0]);
    msg = (char*)evtGetValue(evt, args[1]);
    if (first != 0) {
        if (!(flags & 1)) {
            msgWindow_Add(msgSearch(msg), evt->msgWindowId);
        } else {
            msgWindow_Add(msg, evt->msgWindowId);
        }
    }
    status = windowCheckID(evt->msgWindowId);
    mask = ((-status) | status) >> 31;
    return 2 & ~mask;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_msg_togelr(int param_1) {
    EvtEntry* evt = (EvtEntry*)param_1;
    s32 value;

    value = evtGetValue(evt, evt->args[0]);
    *(u32*)((s32)evt + 0x180) |= 0x80000000;
    switch (value) {
        case 1:
            *(u32*)((s32)evt + 0x180) |= 0x40000000;
            break;
        case 2:
            *(u32*)((s32)evt + 0x180) |= 0x20000000;
            break;
        case 3:
            *(u32*)((s32)evt + 0x180) |= 0x60000000;
            break;
        case 0:
        default:
            break;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_msg_toge(void* pEvt) {
    extern f32 evtGetFloat(EvtEntry* evt, s32 value);
    s32* args;
    EvtEntry* evt = pEvt;
    s32 flags;

    args = evt->args;
    flags = evtGetValue(evt, args[0]);
    *(u32*)((s32)evt + 0x180) &= 0xE1000000;
    *(u32*)((s32)evt + 0x180) |= flags;
    *(f32*)((s32)evt + 0x184) = evtGetFloat(evt, args[1]);
    *(f32*)((s32)evt + 0x188) = evtGetFloat(evt, args[2]);
    *(f32*)((s32)evt + 0x18C) = evtGetFloat(evt, args[3]);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_msg_print(void* evtEntry, int param_2) {
    s32* args;
    u32 value0;
    char* value1;
    EvtEntry* evt;
    s32 first;
    s32 value2;
    char* value3;

    first = param_2;
    evt = evtEntry;
    args = evt->args;
    value0 = evtGetValue(evt, args[0]);
    value1 = (char*)evtGetValue(evt, args[1]);
    value2 = evtGetValue(evt, args[2]);
    value3 = (char*)evtGetValue(evt, args[3]);
    return _evt_msg_print(evt, first, value0, value1, value2, value3);
}


s32 evt_msg_npc(int param_1) {
    extern void* windowGetPointer(s32 id);
    extern char* strcpy(char* dst, const char* src);
    EvtEntry* evt = (EvtEntry*)param_1;
    char* msg;
    void* window;
    void* work;

    msg = (char*)evtGetValue(evt, evt->args[0]);
    window = windowGetPointer(*(s32*)((s32)evt + 0x178));
    work = *(void**)((s32)window + 0x28);
    strcpy((char*)(*(s32*)work + 0xF204), msg);
    return 2;
}

s32 evt_msg_repeat(void* pEvt, int param_2) {
    extern void msgWindow_Repeat(s32 windowId);
    EvtEntry* evt = pEvt;
    s32 status;

    if (param_2 != 0) {
        msgWindow_Repeat(*(s32*)((s32)evt + 0x178));
    }
    status = windowCheckID(*(s32*)((s32)evt + 0x178));
    if (status != 0) {
        return 0;
    }
    return 2;
}

/* CHATGPT STUB FILL: main/event/evt_msg 20260624_184008 */

/* stub-fill: unk_800d0a4c | prototype_only | source_prototype */
void unk_800d0a4c(char* dst, char* value) {
    extern char* strstr(const char* text, const char* find);
    extern u32 strlen(const char* text);
    extern char* strncpy(char* dst, const char* src, u32 count);
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 sprintf(char* dst, const char* format, ...);
    extern char unk_803dd6c8[], unk_803de0c8[];
    extern char str_COIN_802cc618[], str_POINT_802cc628[];
    extern char str_Punkte_802cc630[], str_moneta_802cc638[], str_monete_802cc640[];
    extern char str_point_802cc648[], str_OGGETTO_802cc650[];
    extern char str_oggetto_802cc65c[], str_oggtti_802cc664[];
    extern void* gp;
    char number[16];
    char* marker;
    u32 prefix;
    s32 count = (s32)value;
    s32 language = *(s32*)((s32)gp + 0x14);

    marker = strstr(dst, "<NUM>");
    if (marker == 0) {
        return;
    }
    prefix = strlen(dst) - strlen(marker);
    strncpy(unk_803de0c8, dst, prefix);
    unk_803de0c8[prefix] = 0;
    sprintf(number, "%d", count);
    strcat(unk_803de0c8, number);
    strcat(unk_803de0c8, marker + 5);

    if (language == 0) {
        strcpy(unk_803dd6c8, dst);
        goto finish;
    }
    if (language == 2) {
        marker = strstr(unk_803de0c8, str_COIN_802cc618);
        if (marker != 0) {
            prefix = strlen(unk_803de0c8) - strlen(marker);
            strncpy(unk_803dd6c8, unk_803de0c8, prefix);
            unk_803dd6c8[prefix] = 0;
            strcat(unk_803dd6c8, count == 1 ? "M\x81nze" : "M\x81nzen");
            strcat(unk_803dd6c8, marker + 6);
            goto finish;
        }
        marker = strstr(unk_803de0c8, str_POINT_802cc628);
        if (marker != 0) {
            prefix = strlen(unk_803de0c8) - strlen(marker);
            strncpy(unk_803dd6c8, unk_803de0c8, prefix);
            unk_803dd6c8[prefix] = 0;
            strcat(unk_803dd6c8, count == 1 ? "Punkt" : str_Punkte_802cc630);
            strcat(unk_803dd6c8, marker + 7);
            goto finish;
        }
    }
    if (language == 5) {
        marker = strstr(unk_803de0c8, str_COIN_802cc618);
        if (marker != 0) {
            prefix = strlen(unk_803de0c8) - strlen(marker);
            strncpy(unk_803dd6c8, unk_803de0c8, prefix);
            unk_803dd6c8[prefix] = 0;
            strcat(unk_803dd6c8, count == 1 ? str_moneta_802cc638 : str_monete_802cc640);
            strcat(unk_803dd6c8, marker + 6);
            goto finish;
        }
        marker = strstr(unk_803de0c8, str_point_802cc648);
        if (marker != 0) {
            prefix = strlen(unk_803de0c8) - strlen(marker);
            strncpy(unk_803dd6c8, unk_803de0c8, prefix);
            unk_803dd6c8[prefix] = 0;
            strcat(unk_803dd6c8, count == 1 ? "punto" : "punti");
            strcat(unk_803dd6c8, marker + 7);
            goto finish;
        }
        marker = strstr(unk_803de0c8, str_OGGETTO_802cc650);
        if (marker != 0) {
            prefix = strlen(unk_803de0c8) - strlen(marker);
            strncpy(unk_803dd6c8, unk_803de0c8, prefix);
            unk_803dd6c8[prefix] = 0;
            strcat(unk_803dd6c8, count == 1 ? str_oggetto_802cc65c : str_oggtti_802cc664);
            strcat(unk_803dd6c8, marker + 7);
            goto finish;
        }
    }
    marker = strstr(unk_803de0c8, "<X>");
    if (marker != 0 && language == 3) {
        prefix = strlen(unk_803de0c8) - strlen(marker);
        strncpy(unk_803dd6c8, unk_803de0c8, prefix);
        unk_803dd6c8[prefix] = 0;
        if (count > 1) strcat(unk_803dd6c8, "x");
        strcat(unk_803dd6c8, marker + 3);
        goto finish;
    }
    marker = strstr(unk_803de0c8, "<S>");
    if (marker != 0) {
        prefix = strlen(unk_803de0c8) - strlen(marker);
        strncpy(unk_803dd6c8, unk_803de0c8, prefix);
        unk_803dd6c8[prefix] = 0;
        if ((language == 3 && count > 1) || (language != 3 && count != 1)) {
            strcat(unk_803dd6c8, "s");
        }
        strcat(unk_803dd6c8, marker + 3);
        goto finish;
    }
    marker = strstr(unk_803de0c8, "<N>");
    if (marker != 0) {
        prefix = strlen(unk_803de0c8) - strlen(marker);
        strncpy(unk_803dd6c8, unk_803de0c8, prefix);
        unk_803dd6c8[prefix] = 0;
        strcat(unk_803dd6c8, marker + 3);
    } else {
        strcpy(unk_803dd6c8, unk_803de0c8);
    }
finish:
    strcpy(dst, unk_803dd6c8);
}

/* stub-fill: unk_800d1364 | prototype_only | source_prototype */
void unk_800d1364(char* dst, char* value) {
    extern void* gp;
    extern char* strcpy(char*, const char*);
    extern char* strncpy(char*, const char*, u32);
    extern char* strcat(char*, const char*);
    extern char* strstr(const char*, const char*);
    extern u32 strlen(const char*);
    extern char str_AN_80421844[];
    extern char str_AN_ITEM_802cc674[];
    extern char str_ITEM_802cc66c[];
    extern char str_an_8042183c[];
    extern char str_a_80421840[];
    extern char str_M_8042184c[];
    extern char str_F_80421850[];
    extern char str_N_80421800[];
    extern char str_MPLdie_802cc680[];
    extern char str_FPLdie_802cc68c[];
    extern char str_NPLdie_802cc698[];
    extern char str_Einen_802cc6a4[];
    extern char str_Ein_ITEM_802cc6ac[];
    extern char str_ein_ITEM_802cc6b8[];
    extern char str_Einen_ITEM_802cc6c4[];
    extern char str_einen_ITEM_802cc6d4[];
    extern char* PTR_str_Einen_802cc608_804163e8[];
    extern char* dat_ptrarr_804163b8[];
    extern char* PTR_str_ein_8042177c_804163d0[];
    extern char* PTR_str_einen_802cc610_80416400[];
    char buffer[2572];
    char* marker;
    char* article;
    u32 prefix;
    s32 form;

    switch (*(s32*)((s32)gp + 0x11D8)) {
    case 0:
        strcpy(buffer, dst);
        break;
    case 1:
    case 6:
        switch (*value) {
        case 'A': case 'E': case 'I': case 'O': case 'U':
        case 'a': case 'e': case 'i': case 'o': case 'u': article = str_an_8042183c; break;
        default: article = str_a_80421840; break;
        }
        marker = strstr(dst, str_AN_80421844);
        if (marker != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix); buffer[prefix] = 0;
            strcat(buffer, article); strcat(buffer, marker + 4);
            strcpy(dst, buffer);
        }
        marker = strstr(dst, str_AN_ITEM_802cc674);
        if (marker != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix); buffer[prefix] = 0;
            strcat(buffer, article); strcat(buffer, value); strcat(buffer, marker + 9);
        } else {
            marker = strstr(dst, str_ITEM_802cc66c);
            if (marker != 0) {
                prefix = strlen(dst) - strlen(marker);
                strncpy(buffer, dst, prefix); buffer[prefix] = 0;
                strcat(buffer, value); strcat(buffer, marker + 6);
            }
        }
        break;
    case 2:
        form = 0;
        if (strstr(value, str_M_8042184c) != 0) {
            form = 0;
        } else if (strstr(value, str_F_80421850) != 0) {
            form = 1;
        } else if (strstr(value, str_N_80421800) != 0) {
            form = 2;
        } else if (strstr(value, str_MPLdie_802cc680) != 0) {
            form = 3;
        } else if (strstr(value, str_FPLdie_802cc68c) != 0) {
            form = 4;
        } else if (strstr(value, str_NPLdie_802cc698) != 0) {
            form = 5;
        }
        marker = strstr(dst, str_Einen_802cc6a4);
        if (marker != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, PTR_str_Einen_802cc608_804163e8[form]);
            strcat(buffer, marker + 7);
            strcpy(dst, buffer);
            marker = strstr(dst, str_ITEM_802cc66c);
            if (marker != 0) {
                prefix = strlen(dst) - strlen(marker);
                strncpy(buffer, dst, prefix);
                buffer[prefix] = 0;
                strcat(buffer, value);
                strcat(buffer, marker + 6);
            }
        } else if ((marker = strstr(dst, str_Ein_ITEM_802cc6ac)) != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, dat_ptrarr_804163b8[form]);
            strcat(buffer, value);
            strcat(buffer, marker + 10);
        } else if ((marker = strstr(dst, str_ein_ITEM_802cc6b8)) != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, PTR_str_ein_8042177c_804163d0[form]);
            strcat(buffer, value);
            strcat(buffer, marker + 10);
        } else if ((marker = strstr(dst, str_Einen_ITEM_802cc6c4)) != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, PTR_str_Einen_802cc608_804163e8[form]);
            strcat(buffer, value);
            strcat(buffer, marker + 12);
        } else if ((marker = strstr(dst, str_einen_ITEM_802cc6d4)) != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, PTR_str_einen_802cc610_80416400[form]);
            strcat(buffer, value);
            strcat(buffer, marker + 12);
        } else if ((marker = strstr(dst, str_ITEM_802cc66c)) != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix);
            buffer[prefix] = 0;
            strcat(buffer, value);
            strcat(buffer, marker + 6);
        }
        break;
    default:
        marker = strstr(dst, str_ITEM_802cc66c);
        if (marker != 0) {
            prefix = strlen(dst) - strlen(marker);
            strncpy(buffer, dst, prefix); buffer[prefix] = 0;
            strcat(buffer, value); strcat(buffer, marker + 6);
        } else strcpy(buffer, dst);
        break;
    }
    strcpy(dst, buffer);
}

