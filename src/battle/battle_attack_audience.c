// Audience attacks: audience members throwing items, and item announcements.

#include "version.h"
#include "driver/itemdrv.h"
#include "driver/camdrv.h"
#include "evt/evt_msg.h"
#include "battle/battle_sub.h"
#if VERSION >= VERSION_US
#include "mario/mariost.h"
#include <string.h>
#endif
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_database_common.h"
#include "battle/battle_unit.h"
#include "data/item_data.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "eff/eff_hit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_attack_audience.h"
#include "libc_decls.h"

ItemEntry* itemEntry(const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* evtCode, f32 x, f32 y, f32 z);
extern void psndSFX_pit(s32 id, s32 pitch);

/**
 * Script helper: finds the audience member currently holding an item to throw and returns its index (args[0])
 * and its position (args[1..3], whole units); any argument may be EVT_NULLPTR to skip it.
 * Used by the "attack the audience" scripts when Mario or a partner presses X to stop a troublemaker.
 */
s32 _get_attack_aud_no(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 noOut = args[0];
    s32 xOut = args[1];
    s32 yOut = args[2];
    s32 zOut = args[3];
    s32 no;
    f32 x;
    f32 y;
    f32 z;

    BattleAudience_GetItemOn2(&no, &x, &y, &z, NULL);
    if (noOut != EVT_NULLPTR) {
        evtSetValue(event, noOut, no);
    }
    if (xOut != EVT_NULLPTR) {
        evtSetValue(event, xOut, (s32)x);
    }
    if (yOut != EVT_NULLPTR) {
        evtSetValue(event, yOut, (s32)y);
    }
    if (zOut != EVT_NULLPTR) {
        evtSetValue(event, zOut, (s32)z);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: hits audience member args[0].
 * args[1] == 1 skips the hit spark and the "pliable" damage sound (Koops' shell variant);
 * otherwise a hit effect and the sound play at the member.
 * The member is then knocked out of the crowd by BattleAudience_Attack.
 */
s32 _attack_aud(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 no;
    s32 mode;
    const char* sfx;
    Vec pos;

    no = evtGetValue(event, args[0]);
    mode = evtGetValue(event, args[1]);
    sfx = NULL;
    BattleAudience_GetItemOn2(&no, &pos.x, &pos.y, &pos.z, NULL);
    pos.y += 32.0f;
    if (mode != 1) {
        effHitEntry(0, 3, pos.x, pos.y, pos.z, 1.0f);
        sfx = "SFX_BTL_DAMAGED_PLIABLE1";
    }
    BattleAudience_GetPosition(no, &pos.x, &pos.y, &pos.z);
    if (sfx) {
        psndSFX_pit(psndSFXOn_3D(sfx, &pos), 0x3FF);
    }
    BattleAudience_Attack(no);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sets args[0] to 1 when the item about to be thrown is junk (can, rock, bone or hammer, items
 * 0xEC-0xEF) or nothing is held, 0 when it is a real item.
 * Partner audience-attack scripts pick their reaction from it.
 */
s32 _check_aud_item_type(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 out = args[0];
    ItemType item;
    s32 flag;

    evtSetValue(event, out, 1);
    BattleAudience_GetItemOn2(NULL, NULL, NULL, NULL, &item);
    if (item == ITEM_NULL) {
        return EVT_RETURN_DONE;
    }
    if (item >= ITEM_AUDIENCE_CAN && item < BADGE_MIN) {
        flag = 1;
    } else {
        flag = 0;
    }
    evtSetValue(event, out, flag);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: clears the item an audience member is throwing,
 * returning its id in args[0] unless that is EVT_NULLPTR.
 */
s32 _delete_present_item(EventEntry* event, BOOL isFirstCall) {
    s32 out = event->args[0];
    s32 item = BattleAudience_GetPresentItemNo();

    if (out != EVT_NULLPTR) {
        evtSetValue(event, out, item);
    }
    BattleAudience_SetPresentItemNo(ITEM_NULL);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: gives the thrown item to its target:
 * into Mario's pouch for the player's side, otherwise as the enemy's held item if it has none.
 */
s32 _get_present_item(EventEntry* event, BOOL isFirstCall) {
    BattleWork* wp = _battleWorkPointer;
    s32 item = BattleAudience_GetPresentItemNo();
    s32 id = BattleAudience_GetPresentTargetUnitId();
    BattleUnit* unit = BattleGetUnitPtr(wp, id);

    if (unit->alliance == ALLIANCE_PLAYER) {
        pouchGetItem(item);
    } else if (unit->heldItem == ITEM_NULL) {
        unit->heldItem = item;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: returns the unit id the audience item was thrown at in args[0]. */
s32 _get_present_target_id(EventEntry* event, BOOL isFirstCall) {
    s32 out = event->args[0];

    evtSetValue(event, out, BattleAudience_GetPresentTargetUnitId());
    return EVT_RETURN_DONE;
}

/**
 * Draws the "Got <item>!" window (btl_msg_get_item) centred near the top of the screen for the thrown item.
 * Non-JP releases fill the item name in with evtMsgFmtStr for languages other than English.
 */
void _get_item_announce_disp(CameraId cameraId, void* param) {
    u16 lines;
    const char* itemName;
    char buf[128];
    const char* msg;
    s32 width;
    s32 no;
    f32 x;
    f32 y;

    no = BattleAudience_GetPresentItemNo();
    itemName = msgSearch(itemDataTable[no].name);
#if VERSION >= VERSION_US
    // FAKE: (u32) cast before the compare with LANGUAGE_JP; the plain compare changes the object
    if ((u32)gp->language == LANGUAGE_JP) {
        msg = msgSearch("btl_msg_get_item");
        sprintf(buf, msg, itemName);
    } else {
        strcpy(buf, msgSearch("btl_msg_get_item"));
        evtMsgFmtStr(buf, itemName);
    }
#else
    msg = msgSearch("btl_msg_get_item");
    sprintf(buf, msg, itemName);
#endif
    width = FontGetMessageWidthLine(buf, &lines);
    lines++;
    x = 0.0f;
    y = 60.0f;
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, (x - width / 2) - 10.0f, 34.0f, width + 20, lines * 29 + 3, 10.0f);
    FontDrawStart();
    FontDrawMessage(x - width / 2, (s32)y - 29, buf);
}

/**
 * Script helper: shows the caught item floating above the target unit
 * and the "Got <item>!" window for 90 frames, then removes the item entity.
 */
s32 _get_item_announce(EventEntry* event, BOOL isFirstCall) {
    BattleWork* wp = _battleWorkPointer;
    s32 item = BattleAudience_GetPresentItemNo();

    if (isFirstCall) {
        s32 id = BattleAudience_GetPresentTargetUnitId();
        BattleUnit* unit = BattleGetUnitPtr(wp, id);
        f32 x;
        f32 y;
        f32 z;

        BtlUnit_GetPos(unit, &x, &y, &z);
#if VERSION <= VERSION_DEMO_US
        y = y + (f32)(BtlUnit_GetHeight(unit) + 10);
        event->userdatap[1] = itemEntry(NULL, item, ITEM_TYPE_STILL_NO_TOUCH, -1, NULL, x, y, z);
#else
        y = y + (unit->sizeMultiplier * (f32)BtlUnit_GetHeight(unit) + 8.0f);
        event->userdatap[1] = itemEntry(NULL, item, ITEM_TYPE_STILL_NO_TOUCH, -1, NULL, x, y, z);
#endif
        event->userdata[2] = 90;
    } else {
        if (event->userdata[2] > 0) {
            event->userdata[2]--;
        }
        if (event->userdata[2] <= 0) {
            itemDelete(((ItemEntry*)event->userdatap[1])->name);
            return EVT_RETURN_DONE;
        }
    }
    dispEntry(CAMERA_2D, 900.0f, 1, (void*)_get_item_announce_disp, NULL);
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: applies the damage of a harmful thrown item (the audience's presentWeapon) to the target unit's
 * body part. args[0] is read but unused.
 */
s32 _damage(EventEntry* event, BOOL isFirstCall) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = BattleAudience_GetPresentTargetUnitId();
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    s32 body = BtlUnit_GetBodyPartsId(unit);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, body);
    BattleAudience* aud;

    evtGetValue(event, args[0]);
    aud = BattleAudienceBaseGetPtr();
    BattleActionCommandResetDefenceResult();
    BattleCheckDamage(BattleGetSystemPtr(wp), unit, part, &aud->presentWeapon, DAMAGE_RESULT_FINAL_HIT);
    return EVT_RETURN_DONE;
}

/**
 * Returns the script the target runs when an audience item lands:
 * btlataudevtPresentItem_Damage for harmful throws (type 1), btlataudevtPresentItem_Get for gifts.
 * Called by the battle command sequence.
 */
void* btlataudGetMoveEventPtr(s32 id) {
    switch (BattleAudience_GetPresentItemType()) {
        case 1:
            return btlataudevtPresentItem_Damage;
        case 0:
        default:
            return btlataudevtPresentItem_Get;
    }
}

#if VERSION < VERSION_US
__declspec(section ".data") static s32 btlataudevtDummyPad[1] = {0};
#endif
/** Fallback audience attack for units without their own: hit the member holding the item and resume. */
EVT_BEGIN(btlataudevtDummy)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(_attack_aud, LW(3), 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Returns the unit's own "attack the audience" script (unit data slot 6), or btlataudevtDummy when it has none. */
EvtWord* btlataudGetAttackEventPtr(s32 unitId) {
    EvtWord* p = BtlUnit_GetData(BattleGetUnitPtr(_battleWorkPointer, unitId), UNIT_DATA_ATTACK_AUDIENCE);
    if (p == NULL) {
        p = btlataudevtDummy;
    }
    return p;
}

/** Runs when a harmful item thrown by the audience hits its target: clears the item and applies the damage. */
EVT_BEGIN(btlataudevtPresentItem_Damage)
    CALL(_delete_present_item, LW(0))
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        CALL(_damage, LW(0))
    END_INLINE_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

/**
 * Runs when a gift from the audience reaches its target:
 * pauses the target's wait animation, hands over the item,
 * plays the catching pose and the "Got <item>!" announcement, then clears the item.
 */
EVT_BEGIN(btlataudevtPresentItem_Get)
    CALL(_get_present_target_id, LW(3))
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
    END_IF
    CALL(_get_present_item)
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_ITEM)
    CALL(_get_item_announce)
    CALL(_delete_present_item, EVT_NULLPTR)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

