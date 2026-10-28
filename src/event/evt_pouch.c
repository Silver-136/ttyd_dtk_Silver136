#include "evt/evt_cmd.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

#define SETTER(name, fn)                                 \
    USER_FUNC(name) {                                    \
        fn(evtGetValue(event, event->args[0]));          \
        return EVT_RETURN_DONE;                          \
    }

#define GETTER(name, fn)                   \
    USER_FUNC(name) {                      \
        EvtWord* args = event->args;       \
        evtSetValue(event, args[0], fn()); \
        return EVT_RETURN_DONE;            \
    }

GETTER(evt_pouch_get_coin, pouchGetCoin)
SETTER(evt_pouch_add_coin, pouchAddCoin)
SETTER(evt_pouch_set_coin, pouchSetCoin)
GETTER(evt_pouch_get_supercoin, pouchGetSuperCoin)
SETTER(evt_pouch_set_supercoin, pouchSetSuperCoin)
GETTER(evt_pouch_get_starpiece, pouchGetStarPiece)
SETTER(evt_pouch_add_starpiece, pouchAddStarPiece)
SETTER(evt_pouch_get_starstone, pouchGetStarStone)

USER_FUNC(evt_pouch_add_item) {
    EvtWord* args = event->args;
    if (pouchGetItem(evtGetValue(event, args[0])) == 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_pouch_remove_item) {
    EvtWord* args = event->args;
    if (pouchRemoveItem(evtGetValue(event, args[0])) == 1) {
        pouchReviseMarioParam();
        pouchRevisePartyParam();
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], -1);
    }
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps).
USER_FUNC(evt_pouch_remove_item_index) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    if (pouchRemoveItemIndex(type, index) == 1) {
        pouchReviseMarioParam();
        pouchRevisePartyParam();
        evtSetValue(event, args[2], 0);
    } else {
        evtSetValue(event, args[2], -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_pouch_add_keepitem) {
    EvtWord* args = event->args;
    if (pouchAddKeepItem(evtGetValue(event, args[0]))) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_pouch_remove_keepitem) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    if (pouchRemoveKeepItem(id, index)) {
        evtSetValue(event, args[2], 0);
    } else {
        evtSetValue(event, args[2], -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_pouch_check_item) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], pouchCheckItem(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

GETTER(evt_pouch_get_hp, pouchGetHP)
SETTER(evt_pouch_set_hp, pouchSetHP)
GETTER(evt_pouch_get_max_ap, pouchGetMaxAP)
SETTER(evt_pouch_set_ap, pouchSetAP)
SETTER(evt_pouch_party_join, partyJoin)
SETTER(evt_pouch_party_left, partyLeft)

USER_FUNC(evt_pouch_mario_recovery) {
    pouchSetHP(pouchGetMaxHP());
    pouchSetFP(pouchGetMaxFP());
    pouchSetAP(pouchGetMaxAP());
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_pouch_all_party_recovery) {
    PouchData* pouch = pouchGetPtr();
    int i;
    for (i = 0; i < PARTNER_COUNT; i++) {
        pouch->partyData[i].currentHP = pouch->partyData[i].maximumHP;
    }
    return EVT_RETURN_DONE;
}

GETTER(evt_pouch_get_haveitemcnt, pouchGetHaveItemCnt)
GETTER(evt_pouch_get_havebadgecnt, pouchGetHaveBadgeCnt)
GETTER(evt_pouch_get_equipbadgecnt, pouchGetEquipBadgeCnt)
SETTER(evt_pouch_majinai_set, pouchMajinaiInit)
SETTER(evt_pouch_receive_mail, pouchReceiveMail)
SETTER(evt_pouch_open_mail, pouchOpenMail)
USER_FUNC(evt_pouch_set_yoshiname) {
    pouchSetYoshiName((char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}
USER_FUNC(evt_pouch_get_yoshiname) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (EvtWord)pouchGetYoshiName());
    return EVT_RETURN_DONE;
}
