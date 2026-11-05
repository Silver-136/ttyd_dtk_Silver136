// Built for the early demos, PAL and the PAL demo (not JP, US or the September US demo): the UFO test enemy
// (UNIT_UNUSED_UFO, "bll_un_ufo" in tst.rel). It flies in, hovers over a target, drops an item part (part 2)
// that bounces on the floor and hides when it reaches the target, then flies away.
// The unit's 0x44-byte work (BtlUnit_Entry allocates it for this kind) sits in countersetWeapon.
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_detect.h"

typedef struct UfoWork {
    s32 targetUnitId; // 0x0, unit the UFO hovers over (-1 none)
    s32 targetPartId; // 0x4
    s32 state;        // 0x8, btlevtcmd_UFOAction state (1 fly in .. 11 delete)
    s32 swayFrame;    // 0xC, hover bob counter (_SetUfoPos_Float)
    s32 timer;        // 0x10, frames to wait after the drop
    s32 dropState;    // 0x14, dropped item: 0 none, 1 start, 2 falling, 3/4 blinking, 5 gone
    s32 reserved_18;  // 0x18, never accessed
    s32 dropTimer;    // 0x1C, blink frames left
    Vec dropPos;      // 0x20
    Vec dropTarget;   // 0x2C
    f32 dropGravity;  // 0x38
    f32 dropSpeedX;   // 0x3C
    f32 dropSpeedY;   // 0x40
} UfoWork;

// The functions are in reverse order of the binary: the compiler emits them last to first.
static void _UFOItemAction(EventEntry* event);

/**
 * Places the UFO at its home position plus a vertical bob of +-8 units (a 60-frame sine cycle);
 * advance steps the bob forward a frame, FALSE just re-applies the current offset.
 */
static void _SetUfoPos_Float(BattleUnit* unit, BOOL advance) {
    UfoWork* work;
    f32 x, y, z;

    work = (UfoWork*)unit->countersetWeapon;
    BtlUnit_GetHomePos(unit, &x, &y, &z);
    if (advance) {
        work->swayFrame++;
    }
    y = 8.0f * sinfd(work->swayFrame * 6) + y;
    BtlUnit_SetPos(unit, x, y, z);
}

/**
 * Runs the whole UFO sequence for the calling unit, blocking every frame until it ends:
 * flies in from x 300 to 60, picks a target, slides over it, drops part 2 as an item, waits 60 frames,
 * then flies off to x -300 and deletes itself.
 * Pauses while an action is being decided; btlevtcmd_UFODamageOn freezes it
 * and btlevtcmd_UFOEscapeOn sends it away.
 */
USER_FUNC(btlevtcmd_UFOAction) {
    BattleWorkWeaponTargets targets;
    s32 targetId;
    s32 partId;
    f32 x1, y1, z1;
    f32 hx3, hy3, hz3;
    f32 tx3, ty3, tz3;
    f32 hx4, hy4, hz4;
    f32 tx4, ty4, tz4;
    f32 ox4, oy4, oz4;
    f32 x7, y7, z7;
    BattleWork* wp;
    s32 id;
    BattleUnit* unit;
    UfoWork* work;
    BOOL again;
    BattleUnit* target;
    BattleUnitPart* part;

    wp = _battleWorkPointer;
    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    work = (UfoWork*)unit->countersetWeapon;
    if (work == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        event->blocked = 1;
        work->targetUnitId = -1;
        BtlUnit_SetHomePos(unit, 300.0f, 150.0f, 0.0f);
        work->swayFrame = 0;
        work->state = 1;
        work->dropState = 0;
    }
    targetId = work->targetUnitId;
    partId = work->targetPartId;
    if (work->state < 6 && (wp->flags & BATTLE_FLAG_ACT_DECIDED_UNUSED)) {
        return EVT_RETURN_BLOCK;
    }
    if (work->state < 6 && targetId != -1) {
        target = BattleGetUnitPtr(wp, targetId);
        if (target == NULL) {
            work->state = 2;
        }
    }
    do {
        again = FALSE;
        switch (work->state) {
            case 1:
                BtlUnit_GetHomePos(unit, &x1, &y1, &z1);
                x1 -= 3.0f;
                BtlUnit_SetHomePos(unit, x1, y1, z1);
                if (x1 <= 60.0f) {
                    x1 = 60.0f;
                    BtlUnit_SetHomePos(unit, x1, y1, z1);
                    work->state = 2;
                }
                _SetUfoPos_Float(unit, TRUE);
                break;
            case 2:
                BattleSamplingEnemy(&targets, NULL, id, BtlUnit_GetEnemyBelong(unit), 0x1103100, 0,
                                    wp->alliances[unit->alliance].attackDirection);
                BattleChoiceSamplingEnemy(&targets, 0x80000000, &targetId, &partId);
                if (targetId == -1) {
                    work->state = 6;
                    again = TRUE;
                }
                work->targetUnitId = targetId;
                work->targetPartId = partId;
                _SetUfoPos_Float(unit, !again);
                work->state = 3;
                break;
            case 3:
                target = BattleGetUnitPtr(wp, targetId);
                BtlUnit_GetHomePos(unit, &hx3, &hy3, &hz3);
                BtlUnit_GetPos(target, &tx3, &ty3, &tz3);
                if (tx3 - hx3 > 0.0f) {
                    hx3 += 1.0f;
                    if (tx3 <= hx3) {
                        hx3 = tx3;
                    }
                } else {
                    hx3 -= 1.0f;
                    if (tx3 >= hx3) {
                        hx3 = tx3;
                    }
                }
                BtlUnit_SetHomePos(unit, hx3, hy3, hz3);
                if (hx3 == tx3) {
                    work->state = 4;
                }
                _SetUfoPos_Float(unit, TRUE);
                break;
            case 4:
                target = BattleGetUnitPtr(wp, targetId);
                BtlUnit_GetHomePos(unit, &hx4, &hy4, &hz4);
                BtlUnit_GetPos(target, &tx4, &ty4, &tz4);
                if (hx4 != tx4) {
                    work->state = 3;
                    again = TRUE;
                    break;
                }
                part = BtlUnit_GetPartsPtr(unit, 2);
                BtlUnit_GetPos(unit, &hx4, &hy4, &hz4);
                BtlUnit_GetPartsOffsetPos(part, &ox4, &oy4, &oz4);
                hx4 += ox4;
                hy4 += oy4;
                hz4 += oz4;
                BtlUnit_SetPartsPos(part, hx4, hy4, hz4);
                part->attributes |= kAbsolutePosition;
                work->dropState = 1;
                work->timer = 60;
                BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit))->attributes |= kNotTargetable;
                work->state = 5;
                _SetUfoPos_Float(unit, TRUE);
                break;
            case 5:
                work->timer--;
                if (work->timer <= 0) {
                    work->state = 6;
                }
                _SetUfoPos_Float(unit, TRUE);
                break;
            case 6:
                work->state = 7;
                break;
            case 7:
                BtlUnit_GetHomePos(unit, &x7, &y7, &z7);
                if (-300.0f - x7 > 0.0f) {
                    x7 += 1.0f;
                    if (-300.0f <= x7) {
                        x7 = -300.0f;
                    }
                } else {
                    x7 -= 1.0f;
                    if (-300.0f >= x7) {
                        x7 = -300.0f;
                    }
                }
                BtlUnit_SetHomePos(unit, x7, y7, z7);
                if (x7 <= -300.0f) {
                    work->state = 11;
                }
                if (x7 >= 240.0f) {
                    BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit))->attributes |= kNotTargetable;
                }
                _SetUfoPos_Float(unit, TRUE);
                break;
            case 8:
                work->state = 9;
                break;
            case 9:
                break;
            case 10:
                work->state = 6;
                BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit))->attributes |= kNotTargetable;
                break;
            case 11:
            default:
                BtlUnit_Delete(unit);
                return EVT_RETURN_DONE;
        }
    } while (again);
    _UFOItemAction(event);
    return EVT_RETURN_BLOCK;
}

/**
 * Puts the calling UFO into state 8, which settles into the idle state 9:
 * the UFO stops moving and only the dropped item keeps updating.
 * Called from the test UFO unit's damage event (rels/tst unit_ufo.c).
 */
USER_FUNC(btlevtcmd_UFODamageOn) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    UfoWork* work;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    work = (UfoWork*)unit->countersetWeapon;
    if (work == NULL) {
        return EVT_RETURN_DONE;
    }
    work->state = 8;
    return EVT_RETURN_DONE;
}

/**
 * Makes the calling UFO untargetable and starts its fly-away (state 10, then 6).
 * Called from the test UFO unit's damage event (rels/tst unit_ufo.c).
 */
USER_FUNC(btlevtcmd_UFOEscapeOn) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    UfoWork* work;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    work = (UfoWork*)unit->countersetWeapon;
    if (work == NULL) {
        return EVT_RETURN_DONE;
    }
    work->state = 10;
    return EVT_RETURN_DONE;
}

/**
 * Per-frame update of the item the UFO dropped (part 2):
 * falls with gravity -0.25, drifts 2 units a frame toward the target and bounces off the floor at 0.6 speed,
 * and hides once its box overlaps the target part.
 * The 120-frame blink (dropState 3) is never started in this file.
 * Called at the end of each btlevtcmd_UFOAction frame.
 */
static void _UFOItemAction(EventEntry* event) {
    f32 ux, uy, uz;
    f32 tx, ty, tz;
    f32 px, py, pz;
    BattleWork* wp;
    BattleUnit* unit;
    BattleUnitPart* part;
    UfoWork* work;
    BattleUnit* target;
    s32 targetPartId;
    BattleUnitPart* targetPart;
    BOOL hit;
    f32 x, y;
    f32 left, right, bottom, top;
    f32 tLeft, tRight, tBottom, tTop;
    s32 id;

    wp = _battleWorkPointer;
    id = BattleTransID(event, -2);
    target = NULL;
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return;
    }
    part = BtlUnit_GetPartsPtr(unit, 2);
    work = (UfoWork*)unit->countersetWeapon;
    if (work->dropState == 0 || work->dropState == 5) {
        return;
    }
    targetPartId = work->targetPartId;
    if (work->targetUnitId != -1) {
        target = BattleGetUnitPtr(wp, work->targetUnitId);
    }
    switch (work->dropState) {
        case 1:
            BtlUnit_GetPartsPos(part, &work->dropPos.x, &work->dropPos.y, &work->dropPos.z);
            targetPart = BtlUnit_GetPartsPtr(target, targetPartId);
            if (targetPart->attributes & kAbsolutePosition) {
                BtlUnit_GetPartsPos(targetPart, &work->dropTarget.x, &work->dropTarget.y, &work->dropTarget.z);
            } else {
                BtlUnit_GetPos(target, &ux, &uy, &uz);
                BtlUnit_GetPartsOffsetPos(targetPart, &work->dropTarget.x, &work->dropTarget.y, &work->dropTarget.z);
                work->dropTarget.x += ux;
                work->dropTarget.y += uy;
                work->dropTarget.z += uz;
            }
            work->dropGravity = -0.25f;
            work->dropSpeedY = 0.0f;
            if (__fabs(work->dropTarget.x - work->dropPos.x) <= 5.0) {
                work->dropSpeedX = 0.0f;
            } else if (work->dropTarget.x > work->dropPos.x) {
                work->dropSpeedX = 2.0f;
            } else {
                work->dropSpeedX = -2.0f;
            }
            work->dropTimer = 0;
            work->dropState = 2;
            break;
        case 3:
            work->dropTimer = 120;
            work->dropState = 4;
            break;
    }
    work->dropPos.x += work->dropSpeedX;
    work->dropPos.y += work->dropSpeedY;
    if (work->dropPos.y <= 0.0f) {
        work->dropPos.y = 0.0f;
        work->dropSpeedY *= -0.6f;
    }
    work->dropSpeedY += work->dropGravity;
    BtlUnit_SetPartsPos(part, work->dropPos.x, work->dropPos.y, work->dropPos.z);
    if (work->dropTimer > 0) {
        work->dropTimer--;
        if (work->dropTimer <= 0) {
            part->attributes |= kHidden;
            work->dropState = 5;
            return;
        }
        if (work->dropTimer & 2) {
            part->attributes |= kHidden;
        } else {
            part->attributes &= ~kHidden;
        }
    }
    if (target == NULL) {
        return;
    }
    hit = FALSE;
    targetPart = BtlUnit_GetPartsPtr(target, targetPartId);
    x = work->dropPos.x;
    y = work->dropPos.y;
    if (targetPart->attributes & kAbsolutePosition) {
        BtlUnit_GetPartsPos(targetPart, &tx, &ty, &tz);
    } else {
        BtlUnit_GetPos(target, &px, &py, &pz);
        BtlUnit_GetPartsOffsetPos(targetPart, &tx, &ty, &tz);
        tx += px;
        ty += py;
        tz += pz;
    }
    left = x - part->shadowSize / 2;
    right = x + part->shadowSize / 2;
    bottom = y - part->hitHeight / 2;
    top = y + part->hitHeight / 2;
    tLeft = tx - targetPart->shadowSize / 2;
    tRight = tx + targetPart->shadowSize / 2;
    tBottom = ty - targetPart->hitHeight / 2;
    tTop = ty + targetPart->hitHeight / 2;
    if (right >= tLeft && left <= tRight && top >= tBottom && bottom <= tTop) {
        hit = TRUE;
    }
    if (hit) {
        part->attributes |= kHidden;
    }
}
