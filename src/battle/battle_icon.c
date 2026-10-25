#include "battle/battle_icon.h"
#include "battle/battle.h"
#include "data/item_data.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>
#include "battle/battle_sub.h"
#include "libc_decls.h"

extern ItemData itemDataTable[];
#include "system.h"

BattleIcon* BtlIconGetPtr(s32 index) {
    BattleIcon* icon;
    int i;

    for (i = 0; i < 16; i++) {
        icon = &_battleWorkPointer->icons[i];
        if (icon->flags & 1 && icon->index == index) {
            return icon;
        }
    }
    return NULL;
}

void BattleIconInit(void) {
    memset(_battleWorkPointer->icons, 0, sizeof(_battleWorkPointer->icons));
}

void BattleIconMain(void) {
    BattleWork* wp;
    BattleIcon* icon;
    int i;

    wp = _battleWorkPointer;
    for (i = 0; i < 16; i++) {
        icon = &wp->icons[i];
        if (icon->flags & 1) { //in use
            if (icon->base) {
                iconSetPos(icon->base->name, icon->position.x, icon->position.y, icon->position.z);
                iconSetScale(icon->base->name, icon->scale * icon->scaleMul);
                if (icon->flags & 2) {
                    iconFlagOn(icon->base->name, 2);
                }
                else {
                    iconFlagOff(icon->base->name, 2);
                }
            }
        }
    }
}

void BattleIconEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    int i;

    for (i = 0; i < 16; i++) {
        BtlIcon_Delete(&wp->icons[i]);
    }
}

BattleIcon* BtlIcon_Entry(u16 iconId, f32 x, f32 y, f32 z) {
    BattleIcon* icon;
    int i;
    char name[8];

    for (i = 0; i < 16; i++) {
        icon = &_battleWorkPointer->icons[i];
        if (!(icon->flags & 1)) {
            break; //found one not in use
        }
    }
    if (i >= 16) {
        return NULL;
    }
    memset(icon, 0, sizeof(BattleIcon));
    icon->index = i;
    icon->flags = 1;
    icon->scale = 1.0f;
    icon->scaleMul = 1.0f;
    icon->position.x = x;
    icon->position.y = y;
    icon->position.z = z;
    icon->iconId = iconId;
    sprintf(name, "_biu_%02x", icon->index);
    iconEntry(name, icon->iconId);
    icon->base = iconNameToPtr(name);
    iconSetPos(icon->base->name, icon->position.x, icon->position.y, icon->position.z);
    return icon;
}

void BtlIcon_Delete(BattleIcon* icon) {
    if (icon->flags & 1) {
        iconDelete(icon->base->name);
        icon->flags &= ~1;
    }
}

//s32 iconId, f32 x, f32 y, f32 z, s32 evtIndex
USER_FUNC(btlevtcmd_BtlIconEntry) {
    EvtWord* args = event->args;
    BattleIcon* icon;
    s32 iconId, index;
    f32 x, y, z;

    iconId = evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    index = args[4];
    icon = BtlIcon_Entry(iconId, x, y, z);
    evtSetValue(event, index, icon->index);
    return EVT_RETURN_DONE;
}

//s32 itemId (index for itemDataTable) , f32 x, f32 y, f32 z, s32 evtIndex
USER_FUNC(btlevtcmd_BtlIconEntryItemId) {
    EvtWord* args = event->args;
    BattleIcon* icon;
    s32 itemId, index;
    f32 x, y, z;

    itemId = evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    index = args[4];
    icon = BtlIcon_Entry(itemDataTable[itemId].icon_id, x, y, z);
    evtSetValue(event, index, icon->index);
    return EVT_RETURN_DONE;
}

//s32 index for BtlIconGetPtr
USER_FUNC(btlevtcmd_BtlIconDelete) {
    BattleIcon* icon;
    s32 value;

    value = evtGetValue(event, *event->args);
    icon = BtlIconGetPtr(value);
    BtlIcon_Delete(icon);
    return EVT_RETURN_DONE;
}

//s32 index for BtlIconGetPtr, f32 x, f32 y, f32 z
USER_FUNC(btlevtcmd_BtlIconSetPosition) {
    EvtWord* args = event->args;
    BattleIcon* icon;
    f32 x, y, z;
    s32 index;

    index = evtGetValue(event, args[0]);
    icon = BtlIconGetPtr(index);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    icon->position.x = x;
    icon->position.y = y;
    icon->position.z = z;
    return EVT_RETURN_DONE;
}

//s32 index for BtlIconGetPtr, f32 fallAccel
USER_FUNC(btlevtcmd_BtlIconSetFallAccel) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    BattleIcon* icon = BtlIconGetPtr(index);
    icon->fallAccel = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_BtlIconJumpPosition) {
    EvtWord* args = event->args;
    BattleIcon* icon;
    s32 index, frames;
    f32 sx, sy, sz;
    f32 x, y, z;
    f32 dy;

    index = evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    icon = BtlIconGetPtr(index);
    if (isFirstCall) {
        event->blocked = 1;
        if (frames) {
            icon->jumpFrames = frames;
        }
        icon->jumpTarget.x = x;
        icon->jumpTarget.y = y;
        icon->jumpTarget.z = z;
        sx = icon->position.x;
        sz = icon->position.z;
        sy = icon->position.y;
        icon->jumpPos.x = sx;
        icon->jumpStart.x = sx;
        icon->jumpPos.y = sy;
        icon->jumpStart.y = sy;
        icon->jumpPos.z = sz;
        icon->jumpStart.z = sz;
        icon->jumpAngle = angleABf(sx, sz, x, z);
        icon->jumpDist = distABf(sx, sz, x, z);
        if (x - sx > 0.0f) {
            icon->jumpDir = 1;
        }
        else if (x - sx < 0.0f) {
            icon->jumpDir = -1;
        }
        dy = y - sy;
        if (icon->jumpFrames == 0) {
            icon->jumpFrames = (s32)(icon->jumpDist / icon->jumpSpeed);
            icon->jumpDist -= icon->jumpFrames * icon->jumpSpeed;
        }
        else {
            icon->jumpSpeed = icon->jumpDist / icon->jumpFrames;
            icon->jumpDist -= icon->jumpFrames * icon->jumpSpeed;
        }
        if (icon->jumpFrames == 0) {
            return 2;
        }
        icon->jumpVelY = dy / icon->jumpFrames + icon->fallAccel * icon->jumpFrames / 2.0f;
        icon->jumpSpeed += icon->jumpDist / icon->jumpFrames;
    }
    icon->jumpPos.y += icon->jumpVelY;
    icon->jumpVelY -= icon->fallAccel;
    if (icon->jumpVelY < 0.0f && icon->jumpPos.y < icon->jumpTarget.y) {
        icon->jumpPos.y = icon->jumpTarget.y;
    }
    btlMovePos(icon->jumpSpeed, icon->jumpAngle, &icon->jumpPos.x, &icon->jumpPos.z);
    icon->position.x = icon->jumpPos.x;
    icon->position.y = icon->jumpPos.y;
    icon->position.z = icon->jumpPos.z;
    icon->jumpFrames--;
    if (icon->jumpFrames <= 0) {
        icon->position.x = icon->jumpTarget.x;
        icon->position.y = icon->jumpTarget.y;
        icon->position.z = icon->jumpTarget.z;
        return 1;
    }
    return 0;
}

void btlIconStrippedPool(f32* f) {
    f[0] = 0.0f;
    f[1] = 1.0f;
    f[2] = 0.5f;
}
