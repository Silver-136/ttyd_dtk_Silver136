#include "evt/evt_cmd.h"
#include "driver/itemdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "system.h"
#include <dolphin/os.h>
#include "poly_trig.h"

ItemEntry* itemEntry(const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* evtCode, f32 x, f32 y, f32 z);



#define ITEM(ev) itemNameToPtr((char*)evtGetValue(ev, args[0]))

USER_FUNC(evt_item_entry) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    s32 itemId = evtGetValue(event, args[1]);
    s32 x = evtGetValue(event, args[2]);
    s32 y = evtGetValue(event, args[3]);
    s32 z = evtGetValue(event, args[4]);
    s32 type = evtGetValue(event, args[5]);
    s32 collectFlag = args[6];
    EvtWord* evtCode = (EvtWord*)evtGetValue(event, args[7]);
    itemEntry(name, itemId, type, collectFlag, evtCode, x, y, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_delete) {
    itemDelete((char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_delete_check) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    if (item != NULL) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_flag_on) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    u16 flags = args[1];
    if (item == NULL) return EVT_RETURN_DONE;
    itemFlagOn(item, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_flag_off) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    u16 flags = args[1];
    if (item == NULL) return EVT_RETURN_DONE;
    itemFlagOff(item, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_status_on) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    u32 flags = args[1];
    if (item == NULL) return EVT_RETURN_DONE;
    itemStatusOn(item, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_move_dir_speed) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    s32 dir = evtGetValue(event, args[1]);
    f32 speed = evtGetFloat(event, args[2]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->direction = dir;
    item->speed = speed;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_jump_power) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    f32 v = evtGetFloat(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->velocityY = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_gravity) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    f32 v = evtGetFloat(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->gravityScale = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_bound_rate) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    f32 v = evtGetFloat(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->bounceFactor = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_bound_limit) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    s32 v = evtGetValue(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->maxBounces = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_get_item) {
    EvtWord* args = event->args;
    ItemEntry* item = itemNameToPtr((char*)evtGetValue(event, args[0]));
    if (isFirstCall) {
        itemForceGet(item);
        return EVT_RETURN_BLOCK;
    }
    if (item == NULL) return EVT_RETURN_DONE;
    if (!(item->flags & ITEM_FLAG_ACTIVE)) return EVT_RETURN_DONE;
    if (itemStatus(item, 2)) return EVT_RETURN_DONE;
    return itemStatus(item, 0x10000) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(evt_item_get_item_end_wait) {
    ItemEntry* item = itemNameToPtr((char*)evtGetValue(event, event->args[0]));
    if (item == NULL) return EVT_RETURN_DONE;
    if (!(item->flags & ITEM_FLAG_ACTIVE)) return EVT_RETURN_DONE;
    if (itemStatus(item, 2)) return EVT_RETURN_DONE;
    return itemStatus(item, 0x10000) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(evt_item_change_mode) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    ItemEntry* item = itemNameToPtr(name);
    if (item == NULL) return EVT_RETURN_DONE;
    itemModeChange(item, (u16)mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_bound_next_speed) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    f32 v = evtGetFloat(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->savedSpeed = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_bound_next_dir) {
    EvtWord* args = event->args;
    ItemEntry* item = ITEM(event);
    s32 v = evtGetValue(event, args[1]);
    if (item == NULL) return EVT_RETURN_DONE;
    item->savedDirection = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_position) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    ItemEntry* item = itemNameToPtr(name);
    if (item == NULL) return EVT_RETURN_DONE;
    item->position.x = x;
    item->position.y = y;
    item->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_get_position) {
    EvtWord* args = event->args;
    ItemEntry* item;
    s32 a1, a2, a3;
    char* name = (char*)evtGetValue(event, args[0]);
    a1 = args[1];
    a2 = args[2];
    a3 = args[3];
    item = itemNameToPtr(name);
    if (item == NULL) return EVT_RETURN_DONE;
    evtSetFloat(event, a1, item->position.x);
    evtSetFloat(event, a2, item->position.y);
    evtSetFloat(event, a3, item->position.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_scale) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 v = evtGetFloat(event, args[1]);
    ItemEntry* item = itemNameToPtr(name);
    if (item == NULL) return EVT_RETURN_DONE;
    item->scale = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_set_alpha) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    u8 v = evtGetValue(event, args[1]);
    ItemEntry* item = itemNameToPtr(name);
    if (item == NULL) return EVT_RETURN_DONE;
    item->alpha = v;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_move_3d_position) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 x = (f32)evtGetValue(event, args[1]);
    f32 y = (f32)evtGetValue(event, args[2]);
    f32 z = (f32)evtGetValue(event, args[3]);
    s32 ms = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    ItemEntry* item = itemNameToPtr(name);
    u32 elapsed;
    f32* z0 = (f32*)&event->userdata3;
    if (isFirstCall) {
        if (item == NULL) return EVT_RETURN_DONE;
        event->userdata[0] = (s32)gp->renderTime;
        event->userdataf[1] = item->position.x;
        event->userdataf[2] = item->position.y;
        *z0 = item->position.z;
    }
    if ((s32)(elapsed = OSTicksToMilliseconds((u32)gp->renderTime - event->userdata[0])) < ms) {
        item->position.x = intplGetValue(mode, event->userdataf[1], x, (s32)elapsed, ms);
        item->position.y = intplGetValue(mode, event->userdataf[2], y, (s32)elapsed, ms);
        item->position.z = intplGetValue(mode, *z0, z, (s32)elapsed, ms);
        return EVT_RETURN_BLOCK;
    }
    item->position.x = x;
    item->position.y = y;
    item->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_item_get_important_item) {
    ItemEntry* item = itemNameToPtr((char*)evtGetValue(event, event->args[0]));
    MarioWork* mario = marioGetPtr();
    f32 mx, my, mz;
    f32 tb;
    f32 spd;
    f32 ta;
    f32 rad, s, c;
    mx = mario->position.x;
    my = mario->position.y + mario->wPlayerCollisionBox.y;
    mz = mario->position.z;
    spd = 0.5f;
    if (isFirstCall) {
        f32 dist = distABf(mx, mz, item->position.x, item->position.z);
        f32 dy = my - item->position.y;
        ta = dist / spd;
        tb = dy / ta + (ta * 0.1f) / 2.0f;
        event->userdata[0] = *(s32*)&ta;
        event->userdata[1] = *(s32*)&tb;
        event->userdata[2] = *(s32*)&spd;
        event->userdata3 = 0;
    }
    ta = event->userdataf[0];
    tb = event->userdataf[1];
    spd = event->userdataf[2];
    rad = 6.2831855f * angleABf(item->position.x, item->position.z, mx, mz) / 360.0f;
    s = polySinf(rad);
    item->position.x = spd * s + item->position.x;
    c = polyCosf(rad);
    item->position.z = item->position.z - spd * c;
    item->position.y += tb;
    tb = tb - 0.1f;
    ta = ta - 1.0f;
    if (ta < 0.0f) {
        item->position.x = mx;
        item->position.y = my;
        item->position.z = mz;
        return EVT_RETURN_DONE;
    }
    event->userdata[0] = *(s32*)&ta;
    event->userdata[1] = *(s32*)&tb;
    return EVT_RETURN_BLOCK;
}
