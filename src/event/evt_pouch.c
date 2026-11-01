// Script commands for Mario's pouch: coins, star pieces, items, badges, HP/FP, partners and mail.

#include "version.h"
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

/**
 * Gives Mario item, key item or badge args[0] through pouchGetItem;
 * args[1] gets 0 if it was added and -1 if there was no room.
 */
USER_FUNC(evt_pouch_add_item) {
    EvtWord* args = event->args;
    if (pouchGetItem(evtGetValue(event, args[0])) == 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], -1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Takes one of item args[0] from the pouch and re-clamps Mario's
 * and the partners' stats (a removed badge can lower max HP/FP);
 * args[1] gets 0 on success or -1 if Mario had none.
 */
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

/**
 * Retail only: removes item args[0] from slot args[1] of the held list (the slot the shop's sell menu picked)
 * and re-clamps stats; args[2] gets 0 on success or -1 if that slot holds something else.
 */
// Descriptive name (not in the symbol maps).
#if VERSION > VERSION_DEMO_US
/**
 * Retail only: removes item args[0] from slot args[1] of the held list (the slot the shop's sell menu picked)
 * and re-clamps stats; args[2] gets 0 on success or -1 if that slot holds something else.
 */
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
#endif

/**
 * Puts item args[0] into storage (the items kept at the Rogueport item storage);
 * args[1] gets 0 if it fit or -1 if storage is full.
 */
USER_FUNC(evt_pouch_add_keepitem) {
    EvtWord* args = event->args;
    if (pouchAddKeepItem(evtGetValue(event, args[0]))) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], -1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Takes item args[0] out of storage; args[1] (US demo and older: the result) gets 0 on success or -1.
 * Later versions take the storage slot in args[1], close the gap in the list, and return the result in args[2].
 */
USER_FUNC(evt_pouch_remove_keepitem) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_US
if (pouchRemoveKeepItem(evtGetValue(event, args[0]))) {
        evtSetValue(event, args[1], 0);
#else
    s32 id = evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    if (pouchRemoveKeepItem(id, index)) {
        evtSetValue(event, args[2], 0);
#endif
    } else {
#if VERSION <= VERSION_DEMO_US
        evtSetValue(event, args[1], -1);
#else
        evtSetValue(event, args[2], -1);
#endif
    }
    return EVT_RETURN_DONE;
}

/** Counts how many of item args[0] Mario carries (held items, or whether he has a key item/badge) into args[1]. */
USER_FUNC(evt_pouch_check_item) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], pouchCheckItem(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

#if VERSION <= VERSION_DEMO_US
GETTER(evt_pouch_get_max_ap, pouchGetMaxAP)
SETTER(evt_pouch_set_ap, pouchSetAP)
#else
GETTER(evt_pouch_get_hp, pouchGetHP)
SETTER(evt_pouch_set_hp, pouchSetHP)
GETTER(evt_pouch_get_max_ap, pouchGetMaxAP)
SETTER(evt_pouch_set_ap, pouchSetAP)
#endif
SETTER(evt_pouch_party_join, partyJoin)
SETTER(evt_pouch_party_left, partyLeft)
#if VERSION <= VERSION_DEMO_US
#endif

USER_FUNC(evt_pouch_mario_recovery) {
    pouchSetHP(pouchGetMaxHP());
    pouchSetFP(pouchGetMaxFP());
    pouchSetAP(pouchGetMaxAP());
    return EVT_RETURN_DONE;
}

/** Refills the HP of all eight partner slots to their maximums. */
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
    pouchSetYoshiName((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}
/** Returns a pointer to the Yoshi partner's name string in args[0], for messages that print it. */
USER_FUNC(evt_pouch_get_yoshiname) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (EvtWord)pouchGetYoshiName());
    return EVT_RETURN_DONE;
}
