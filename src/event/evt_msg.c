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


s32 _evt_msg_print(void* param_1, int param_2, u32 param_3, char* param_4, s32 param_5, char* param_6) {
    return 0;
}


u8 evt_msg_print_insert(int param_1, int param_2) {
    return 0;
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
    return 0;
}


s32 evt_msg_repeat(void* pEvt, int param_2) {
    return 0;
}


/* CHATGPT STUB FILL: main/event/evt_msg 20260624_184008 */

/* stub-fill: unk_800d0a4c | prototype_only | source_prototype */
void unk_800d0a4c(char* dst, char* value) {
    return;
}

/* stub-fill: unk_800d1364 | prototype_only | source_prototype */
void unk_800d1364(char* dst, char* value) {
    return;
}
