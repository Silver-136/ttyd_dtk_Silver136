// Battle icons: floating item and badge icons, with their script commands.

#include "battle/battle_icon.h"
#include "battle/battle.h"
#include "data/item_data.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>
#include "battle/battle_sub.h"
#include "libc_decls.h"

#include "system.h"

/**
 * Finds the in-use battle icon (of the 16 slots in BattleWork) whose slot index matches; NULL if none.
 * Scripts refer to icons by this index.
 */
BattleIcon* BtlIconGetPtr(s32 index) {
    BattleIcon* icon;
    int i;

    for (i = 0; i < 16; i++) {
        icon = &_battleWorkPointer->icons[i];
        if ((icon->flags & BATTLE_ICON_FLAG_ACTIVE) && icon->index == index) {
            return icon;
        }
    }
    return NULL;
}

/** Clears all 16 battle icon slots; called from battle start. */
void BattleIconInit(void) {
    memset(_battleWorkPointer->icons, 0, sizeof(_battleWorkPointer->icons));
}

/**
 * Per-frame update from the battle main loop: copies each live icon's position
 * and scale (scale * scaleMul) to its icondrv icon, and mirrors flag 2 onto the icon's flag 2.
 */
void BattleIconMain(void) {
    BattleWork* wp;
    BattleIcon* icon;
    int i;

    wp = _battleWorkPointer;
    for (i = 0; i < 16; i++) {
        icon = &wp->icons[i];
        if (icon->flags & BATTLE_ICON_FLAG_ACTIVE) { // in use
            if (icon->base) {
                iconSetPos(icon->base->name, icon->position.x, icon->position.y, icon->position.z);
                iconSetScale(icon->base->name, icon->scale * icon->scaleMul);
                if (icon->flags & BATTLE_ICON_FLAG_HIDDEN) {
                    iconFlagOn(icon->base->name, ICON_FLAG_HIDDEN);
                } else {
                    iconFlagOff(icon->base->name, ICON_FLAG_HIDDEN);
                }
            }
        }
    }
}

/** Deletes every battle icon when the battle ends. */
void BattleIconEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    int i;

    for (i = 0; i < 16; i++) {
        BtlIcon_Delete(&wp->icons[i]);
    }
}

/**
 * Takes a free battle icon slot and creates an icondrv icon named "_biu_<slot in hex>" showing iconId at
 * (x, y, z), scale 1. Returns the slot, or NULL when all 16 are in use.
 */
BattleIcon* BtlIcon_Entry(u16 iconId, f32 x, f32 y, f32 z) {
    BattleIcon* icon;
    int i;
    char name[8];

    for (i = 0; i < 16; i++) {
        icon = &_battleWorkPointer->icons[i];
        if (!(icon->flags & BATTLE_ICON_FLAG_ACTIVE)) {
            break; // free slot
        }
    }
    if (i >= 16) {
        return NULL;
    }
    memset(icon, 0, sizeof(BattleIcon));
    icon->index = i;
    icon->flags = BATTLE_ICON_FLAG_ACTIVE;
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

/** Removes a live battle icon's icondrv icon and frees its slot; safe on an unused slot. */
void BtlIcon_Delete(BattleIcon* icon) {
    if (icon->flags & BATTLE_ICON_FLAG_ACTIVE) {
        iconDelete(icon->base->name);
        icon->flags &= ~BATTLE_ICON_FLAG_ACTIVE;
    }
}

/**
 * Script: creates a battle icon (BtlIcon_Entry) of icon id args[0] at args[1], args[2], args[3] (read as integers)
 * and stores its index in the variable args[4], which the other btlevtcmd_BtlIcon* commands take.
 */
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

    // BtlIcon_Entry returns NULL when all 16 icon slots are in use; icon is used below without a check.
    icon = BtlIcon_Entry(iconId, x, y, z);
    evtSetValue(event, index, icon->index);
    return EVT_RETURN_DONE;
}

/**
 * s32 itemId (index for itemDataTable), x, y, z (read as integers), s32 evtIndex Script:
 * like btlevtcmd_BtlIconEntry but takes an item id and shows that item's icon from itemDataTable.
 */
// args: item id (its itemDataTable icon is shown), x, y, z (read as integers), variable that receives the battle
// icon index
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

    // BtlIcon_Entry returns NULL when all 16 icon slots are in use; icon is used below without a check.
    icon = BtlIcon_Entry(itemDataTable[itemId].icon_id, x, y, z);
    evtSetValue(event, index, icon->index);
    return EVT_RETURN_DONE;
}

/**
 * s32 index for BtlIconGetPtr Script: deletes the battle icon with the given index.
 */
// args: battle icon index (BtlIconGetPtr)
USER_FUNC(btlevtcmd_BtlIconDelete) {
    BattleIcon* icon;
    s32 value;

    value = evtGetValue(event, *event->args);
    icon = BtlIconGetPtr(value);
    BtlIcon_Delete(icon);
    return EVT_RETURN_DONE;
}

/**
 * s32 index for BtlIconGetPtr, f32 x, f32 y, f32 z Script:
 * moves the battle icon with the given index to (x, y, z).
 */
// args: battle icon index, x, y, z (floats)
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

// btlevtcmd_BtlIconSetMoveSpeed and btlevtcmd_BtlIconSetJumpSpeed: original names and sizes (0x50 each) from the JP
// demo map (UNUSED there: no script uses them; listed in this order between btlevtcmd_BtlIconSetPosition and
// btlevtcmd_BtlIconSetFallAccel, as btlevtcmd_SetMoveSpeed, _SetJumpSpeed and _SetFallAccel are for units). Both
// are btlevtcmd_BtlIconSetFallAccel's form. btlevtcmd_BtlIconJumpPosition is btlevtcmd_JumpPosition on the icon's
// fields, which pairs them: jumpSpeed is the unit's moveSpeedXZ (BtlUnit_SetMoveSpeed) and jumpVelY its
// moveSpeedY (BtlUnit_SetJumpSpeed).

/** Script: sets the horizontal speed of battle icon args[0] (read by btlevtcmd_BtlIconJumpPosition). */
USER_FUNC(btlevtcmd_BtlIconSetMoveSpeed) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    BattleIcon* icon = BtlIconGetPtr(index);
    icon->jumpSpeed = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script: sets the vertical speed of battle icon args[0] (btlevtcmd_BtlIconJumpPosition recomputes it). */
USER_FUNC(btlevtcmd_BtlIconSetJumpSpeed) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    BattleIcon* icon = BtlIconGetPtr(index);
    icon->jumpVelY = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * s32 index for BtlIconGetPtr, f32 fallAccel Script:
 * sets the gravity per frame used by btlevtcmd_BtlIconJumpPosition for that icon.
 */
// args: battle icon index, fall acceleration (float)
USER_FUNC(btlevtcmd_BtlIconSetFallAccel) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    BattleIcon* icon = BtlIconGetPtr(index);
    icon->fallAccel = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script: arcs a battle icon from its position to (x, y, z) over args[4] frames (0 keeps the icon's own frame
 * count or derives it from jumpSpeed), falling by fallAccel per frame.
 * Blocks the script until it lands; used for items tossed in battle.
 */
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
        } else if (x - sx < 0.0f) {
            icon->jumpDir = -1;
        }
        dy = y - sy;
        if (icon->jumpFrames == 0) {
            icon->jumpFrames = (s32)(icon->jumpDist / icon->jumpSpeed);
            icon->jumpDist -= icon->jumpFrames * icon->jumpSpeed;
        } else {
            icon->jumpSpeed = icon->jumpDist / icon->jumpFrames;
            icon->jumpDist -= icon->jumpFrames * icon->jumpSpeed;
        }
        if (icon->jumpFrames == 0) {
            return EVT_RETURN_DONE;
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
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: continues the fall of battle icon args[0] from its position with its current speeds (no target) until it
 * reaches the battle floor; blocks the script until then.
 */
// Original name and size (0xfc) from the JP demo map (UNUSED there: no script uses it), between
// btlevtcmd_BtlIconJumpPosition and btlevtcmd_BtlIconFallPosition, as btlevtcmd_JumpContinue sits between the unit
// commands. Body: btlevtcmd_JumpContinue (0x118) on the icon's fields, as btlevtcmd_BtlIconJumpPosition mirrors
// btlevtcmd_JumpPosition (the unit's moveCurrentPos is the icon's jumpPos, BtlUnit_SetPos the position stores);
// the size fits exactly. Copying the position through x/y/z locals, as the unit command does with BtlUnit_GetPos,
// gives the same bytes.
USER_FUNC(btlevtcmd_BtlIconJumpContinue) {
    BattleIcon* icon;
    s32 index;
    f32 h;

    index = evtGetValue(event, event->args[0]);
    icon = BtlIconGetPtr(index);
    if (isFirstCall) {
        event->blocked = 1;
        icon->jumpPos.x = icon->position.x;
        icon->jumpPos.y = icon->position.y;
        icon->jumpPos.z = icon->position.z;
    }
    icon->jumpPos.y += icon->jumpVelY;
    icon->jumpVelY -= icon->fallAccel;
    btlMovePos(icon->jumpSpeed, icon->jumpAngle, &icon->jumpPos.x, &icon->jumpPos.z);
    icon->position.x = icon->jumpPos.x;
    icon->position.y = icon->jumpPos.y;
    icon->position.z = icon->jumpPos.z;
    h = BattleGetFloorHeight(_battleWorkPointer, icon->jumpPos.x, icon->jumpPos.y, icon->jumpPos.z);
    if (icon->jumpPos.y < h) {
        icon->jumpPos.y = h;
        icon->position.x = icon->jumpPos.x;
        icon->position.y = icon->jumpPos.y;
        icon->position.z = icon->jumpPos.z;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: drops a battle icon from its position to (x, y, z) over args[4] frames (0 = from its speed), starting
 * with no upward speed and choosing the gravity to land exactly; blocks the script until it lands.
 */
// Original name and size (0x348) from the JP demo map (UNUSED there: no script uses it), after
// btlevtcmd_BtlIconJumpPosition. Body: btlevtcmd_FallPosition (also 0x348) on the icon's fields, as
// btlevtcmd_BtlIconJumpPosition mirrors btlevtcmd_JumpPosition; the size fits exactly. Its 1.0f is the constant the
// former stand-in btlIconStrippedPool kept in the pool.
USER_FUNC(btlevtcmd_BtlIconFallPosition) {
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
        icon->jumpFrames = frames;
        icon->jumpTarget.x = x;
        icon->jumpTarget.y = y;
        icon->jumpTarget.z = z;
        sx = icon->position.x;
        sz = icon->position.z;
        sy = icon->position.y;
        icon->jumpPos.x = sx;
        icon->jumpPos.y = sy;
        icon->jumpPos.z = sz;
        icon->jumpAngle = angleABf(sx, sz, x, z);
        icon->jumpDist = distABf(sx, sz, x, z);
        if (x - sx > 0.0f) {
            icon->jumpDir = 1;
        } else if (x - sx < 0.0f) {
            icon->jumpDir = -1;
        }
        dy = y - sy;
        if (icon->jumpFrames == 0) {
            if (icon->jumpSpeed == 0.0f) {
                icon->jumpSpeed = 1.0f;
            }
            icon->jumpFrames = icon->jumpDist / icon->jumpSpeed;
            if (icon->jumpFrames == 0) {
                icon->jumpFrames = 1;
            }
        } else {
            icon->jumpSpeed = icon->jumpDist / icon->jumpFrames;
        }
        icon->jumpVelY = 0.0f;
        icon->fallAccel = (dy / icon->jumpFrames - icon->jumpVelY) / (-icon->jumpFrames / 2.0f);
    }
    icon->jumpPos.y += icon->jumpVelY;
    icon->jumpVelY -= icon->fallAccel;
    btlMovePos(icon->jumpSpeed, icon->jumpAngle, &icon->jumpPos.x, &icon->jumpPos.z);
    icon->position.x = icon->jumpPos.x;
    icon->position.y = icon->jumpPos.y;
    icon->position.z = icon->jumpPos.z;
    icon->jumpFrames--;
    if (icon->jumpFrames <= 0) {
        icon->position.x = icon->jumpTarget.x;
        icon->position.y = icon->jumpTarget.y;
        icon->position.z = icon->jumpTarget.z;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}
