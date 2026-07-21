#include "event/evt_pouch.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_coin) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetCoin(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetCoin());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_pouch_add_coin) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 pouchAddCoin(s32 coins);
    pouchAddCoin(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_set_coin) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 pouchSetCoin(s32 value);
    pouchSetCoin(evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_supercoin) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetSuperCoin(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetSuperCoin());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_pouch_set_supercoin) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s16 pouchSetSuperCoin(s32 newCount);
    pouchSetSuperCoin(evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_starpiece) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetStarPiece(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetStarPiece());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_pouch_add_starpiece) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 pouchAddStarPiece(s32 amount);
    pouchAddStarPiece(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_get_starstone) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchGetStarStone(s32 id);
    pouchGetStarStone(evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_pouch_add_item(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern s32 pouchGetItem(s32 item);

    s32* args;
    s32 itemType;
    s32 result;

    args = *(s32**)((s32)pEvt + 0x18);
    itemType = evtGetValue(pEvt, args[0]);
    result = pouchGetItem(itemType);

    if (result == 1) {
        evtSetValue(pEvt, args[1], 0);
    } else {
        evtSetValue(pEvt, args[1], -1);
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_pouch_remove_item(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern s32 pouchRemoveItem(s32 item);
    extern void pouchReviseMarioParam(void);
    extern void pouchRevisePartyParam(void);

    s32* args;
    s32 itemType;
    s32 result;

    args = *(s32**)((s32)pEvt + 0x18);
    itemType = evtGetValue(pEvt, args[0]);
    result = pouchRemoveItem(itemType);

    if (result == 1) {
        pouchReviseMarioParam();
        pouchRevisePartyParam();
        evtSetValue(pEvt, args[1], 0);
    } else {
        evtSetValue(pEvt, args[1], -1);
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_pouch_remove_item_index(void* pEvt) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchRemoveItemIndex(s32 itemId, s32 index);
    extern void pouchReviseMarioParam(void);
    extern void pouchRevisePartyParam(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 item = evtGetValue(pEvt, args[0]);
    s32 index = evtGetValue(pEvt, args[1]);

    if (pouchRemoveItemIndex(item, index) == 1) {
        pouchReviseMarioParam();
        pouchRevisePartyParam();
        evtSetValue(pEvt, args[2], 0);
    } else {
        evtSetValue(pEvt, args[2], -1);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_pouch_add_keepitem(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern s32 pouchAddKeepItem(s32 item);

    int* args;
    s32 item;
    s32 result;

    args = *(int**)((s32)param_1 + 0x18);
    item = evtGetValue((void*)param_1, *args);
    result = pouchAddKeepItem(item);

    if (result != 0) {
        evtSetValue((void*)param_1, args[1], 0);
    } else {
        evtSetValue((void*)param_1, args[1], -1);
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_pouch_remove_keepitem(int param_1) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchRemoveKeepItem(s32 itemId, s32 index);
    s32* args = *(s32**)(param_1 + 0x18);
    s32 item = evtGetValue((void*)param_1, args[0]);
    s32 index = evtGetValue((void*)param_1, args[1]);

    if (pouchRemoveKeepItem(item, index) != 0) {
        evtSetValue((void*)param_1, args[2], 0);
    } else {
        evtSetValue((void*)param_1, args[2], -1);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_check_item) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchCheckItem(s32 itemId);
    s32* args = event->args;
    evtSetValue(event, args[1], pouchCheckItem(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(L_evt_pouch_get_hp) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetHP(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetHP());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(L_evt_pouch_set_hp) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchSetHP(s32 hp);
    pouchSetHP(evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_max_ap) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetMaxAP(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetMaxAP());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_pouch_set_ap) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchSetAP(s32 sp);
    pouchSetAP(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_party_join) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void partyJoin(s32 partyId);
    partyJoin(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_party_left) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void partyLeft(s32 partyId);
    partyLeft(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_mario_recovery) {
    extern void pouchSetHP(s32 hp);
    extern void pouchSetFP(s32 fp);
    extern void pouchSetAP(s32 sp);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetMaxFP(void);
    extern s32 pouchGetMaxAP(void);
    pouchSetHP(pouchGetMaxHP());
    pouchSetFP(pouchGetMaxFP());
    pouchSetAP(pouchGetMaxAP());
    return 2;
}
s32 evt_pouch_all_party_recovery(void) {
    extern void* pouchGetPtr(void);
    void* pouch = pouchGetPtr();

    *(s16*)((s32)pouch + 0x6) = *(s16*)((s32)pouch + 0x2);
    *(s16*)((s32)pouch + 0x14) = *(s16*)((s32)pouch + 0x10);
    *(s16*)((s32)pouch + 0x22) = *(s16*)((s32)pouch + 0x1E);
    *(s16*)((s32)pouch + 0x30) = *(s16*)((s32)pouch + 0x2C);
    *(s16*)((s32)pouch + 0x3E) = *(s16*)((s32)pouch + 0x3A);
    *(s16*)((s32)pouch + 0x4C) = *(s16*)((s32)pouch + 0x48);
    *(s16*)((s32)pouch + 0x5A) = *(s16*)((s32)pouch + 0x56);
    *(s16*)((s32)pouch + 0x68) = *(s16*)((s32)pouch + 0x64);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_haveitemcnt) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetHaveItemCnt(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetHaveItemCnt());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_havebadgecnt) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetHaveBadgeCnt(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetHaveBadgeCnt());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_equipbadgecnt) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 pouchGetEquipBadgeCnt(void);
    s32* args = event->args;
    evtSetValue(event, args[0], pouchGetEquipBadgeCnt());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_pouch_majinai_set) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchMajinaiInit(s32 value);
    pouchMajinaiInit(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_receive_mail) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchReceiveMail(s32 mail);
    pouchReceiveMail(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_open_mail) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchOpenMail(s32 mail);
    pouchOpenMail(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_pouch_set_yoshiname) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchSetYoshiName(char* name);
    pouchSetYoshiName((char*)evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_pouch_get_yoshiname) {
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern char* pouchGetYoshiName(void);
    s32* args = event->args;
    evtSetValue(event, args[0], (s32)pouchGetYoshiName());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

