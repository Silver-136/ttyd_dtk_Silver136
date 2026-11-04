// Battle units: creating, spawning, deleting and updating the units and parts that fight in a battle.

// FAKE: the demos' status functions take the enum here while their callers see the s8 prototypes.
#define BATTLE_UNIT_STATUS_ENUM
#include "version.h"
#include "battle/battle_unit.h"
#include "battle/battle.h"
#include "system.h"
#include <string.h>
#include "battle/battle_status_effect.h"
#include "battle/battle_audience.h"
#include "battle/battle_actrecord.h"
#include "battle/unit_work.h"

#include "battle/battle_disp.h"
#include "mario/mario_pouch.h"
#include "driver/npcdrv.h"
#include "pmario_sound.h"
#include "driver/animdrv.h"
#include "manager/evtmgr.h"
#include "memory.h"

double pow(double, double);
void psndSFX_pit(s32 id, s16 pitch);
void psndSFX_vol(s32 id, u8 vol);

/**
 * Items an enemy can use in battle (attack, status and healing items; 0 ends the list). Besides badges, only
 * these pass BtlUnit_EnemyItemCanUseCheck, which decides whether an enemy may be given an item.
 */
s32 _enemy_can_use_item_table[] = {
    ITEM_THUNDER_BOLT, ITEM_FIRE_FLOWER, ITEM_POW_BLOCK, ITEM_THUNDER_RAGE, ITEM_SHOOTING_STAR, ITEM_ICE_STORM,
    ITEM_EARTH_QUAKE, ITEM_HP_DRAIN_ITEM, ITEM_SLEEPY_SHEEP, ITEM_STOPWATCH, ITEM_DIZZY_DIAL, ITEM_MINI_MR_MINI,
    ITEM_MR_SOFTENER, ITEM_RUIN_POWDER, ITEM_POWER_PUNCH, ITEM_COURAGE_SHELL, ITEM_BOOS_SHEET, ITEM_VOLT_SHROOM,
    ITEM_REPEL_CAPE, ITEM_MUSHROOM, ITEM_SUPER_SHROOM, ITEM_ULTRA_SHROOM, ITEM_LIFE_SHROOM, ITEM_DRIED_SHROOM,
    ITEM_TASTY_TONIC, ITEM_HONEY_SYRUP, ITEM_MAPLE_SYRUP, ITEM_JAMMIN_JELLY, ITEM_SLOW_SHROOM, ITEM_GRADUAL_SYRUP,
    ITEM_HOT_DOG, ITEM_CAKE, 0,
};

//local prototypes

/** Clears all 64 unit slots in the battle work at battle start. Always returns TRUE. */
BOOL BtlUnit_Init(void) {
    int i;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        BattleSetUnitPtr(wp, i, NULL);
    }
    return TRUE;
}

void BtlUnit_HpGaugeInit(BattleUnit* unit);

/**
 * Creates a unit in the first free of the 64 slots from its setup entry:
 * copies stats, size and offsets from the unit kind's data, builds the linked list of parts,
 * picks the talk and stay poses, applies pouch stats for Mario and the partners
 * and starts the kind's init script. Returns NULL when no slot or memory is free.
 */
BattleUnit* BtlUnit_Entry(BattleUnitSetup* setup) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleUnitKindPart* kpart;
    s32 n;
    s32 i;
    BattleUnitPart* body;
    EventEntry* evt;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        if (!BattleGetUnitPtr(wp, i)) break;
    }
    if (i >= 64) return NULL;

    unit = (BattleUnit*)BattleAlloc(sizeof(BattleUnit));
    if (!unit) return NULL;

    memset(unit, 0, sizeof(BattleUnit));
    BattleSetUnitPtr(wp, i, unit);
    unit->unitId = i;
    unit->kindParams = setup->unitKindParams;
    unit->dataTable = unit->kindParams->dataTable;
    unit->initialKind = setup->unitKindParams->unitType;
    unit->currentKind = setup->unitKindParams->unitType;
    BattleStatusEffectInit(unit);
    BattleStatusIconInit(unit);
    BattleStatusChangeInfoWorkInit(unit);
    unit->groupSetupId = -1;
    unit->alliance = setup->alliance;
    unit->level = unit->kindParams->level;
    if (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
        unit->level = pouchGetPtr()->level;
    }
    unit->attackPhase = setup->attackPhase;
    unit->unisonPhaseEventCode = NULL;
    unit->phaseEventCode = NULL;
    unit->phaseEventId = 0;
    unit->waitEventCode = NULL;
    unit->waitEventId = 0;
    unit->attackEventCode = NULL;
    unit->attackEventId = 0;
    BtlUnit_SetPos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetOffsetPos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetHomePos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetBaseRotate(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetRotate(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetRotateOffset(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetScale(unit, 1.0f, 1.0f, 1.0f);
    BtlUnit_SetBaseScale(unit, 1.0f, 1.0f, 1.0f);
    unit->offsetCenter = unit->kindParams->centerOffset;
    unit->heldItemBaseOffset = unit->kindParams->heldItemBaseOffset;
    unit->bintaHitOffset = unit->kindParams->bintaHitOffset;
    unit->kissHitOffset = unit->kindParams->kissHitOffset;
    unit->baseMaxHp = setup->unitKindParams->maxHp;
    unit->maxHp = unit->baseMaxHp;
    unit->currentHp = setup->unitKindParams->maxHp;
    unit->baseMaxFp = setup->unitKindParams->maxFp;
    unit->maxFp = unit->baseMaxFp;
    unit->currentFp = setup->unitKindParams->maxFp;
    unit->sizeMultiplier = 1.0f;
    unit->tokenFlags = 0;
    unit->statusFlags = 0;
    unit->protectId = -1;
    BtlUnit_HpGaugeInit(unit);
    unit->width = unit->kindParams->width;
    unit->height = unit->kindParams->height;
    unit->statusIconOffset[0] = unit->kindParams->hitOffset[0];
    unit->statusIconOffset[1] = unit->kindParams->hitOffset[1];
    unit->hpGaugeOffset[0] = unit->kindParams->hpGaugeOffset[0];
    unit->hpGaugeOffset[1] = unit->kindParams->hpGaugeOffset[1];
    unit->cutBaseOffset = unit->kindParams->cutBaseOffset;
    unit->cutWidth = unit->kindParams->cutWidth;
    unit->cutHeight = unit->kindParams->cutHeight;
    unit->swallowRate = unit->kindParams->swallowChance;
    unit->swallowAttribute = unit->kindParams->swallowAttributes;
    BtlUnit_ClearStatus(unit);
    unit->attributeFlags = unit->kindParams->attributeFlags;
    unit->vulnStatus = (BattleVulnerableStatus*)unit->kindParams->statusVulnerability;
    unit->maxMoveCount = 1;
    unit->movesRemaining = 1;
    unit->moveDirection = 0;
    unit->faceDirection = wp->alliances[unit->alliance].attackDirection;
    unit->work[0] = setup->unitWork[0];
    unit->work[1] = setup->unitWork[1];
    unit->work[2] = setup->unitWork[2];
    unit->work[3] = setup->unitWork[3];

    n = (u8)unit->kindParams->numParts;
    part = (BattleUnitPart*)BattleAlloc(n * sizeof(BattleUnitPart));
    memset(part, 0, n * sizeof(BattleUnitPart));
    unit->parts = part;
    kpart = unit->kindParams->parts;
    while (TRUE) {
        part->kindPartParams = kpart;
        part->owner = unit;
        part->attributes = part->kindPartParams->attributes;
        part->counterAttributes = part->kindPartParams->counterAttributes;
        if (part->attributes & kMainPart) {
            unit->talkPartId = part->kindPartParams->partNum;
        }
        part->defense = part->kindPartParams->defense;
        part->defenseAttr = part->kindPartParams->defenseAttr;
        part->shadowSize = part->kindPartParams->shadowSize;
        part->hitHeight = part->kindPartParams->hitHeight;
        part->shadowScale = 1.0f;
        part->moveDirection = 0;
        part->faceDirection = wp->alliances[unit->alliance].attackDirection;
        part->positionOffset.x = part->kindPartParams->positionOffset.x;
        part->positionOffset.y = part->kindPartParams->positionOffset.y;
        part->positionOffset.z = part->kindPartParams->positionOffset.z;
        BtlUnit_SetPartsBaseScale(part, 1.0f, 1.0f, 1.0f);
        BtlUnit_SetPartsScale(part, 1.0f, 1.0f, 1.0f);
        BtlUnit_SetPartsBaseRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotateOffset(part, 0.0f, 0.0f, 0.0f);
        part->rotationOrder = 0;
        part->hitBaseOffset.x = part->kindPartParams->hitBaseOffset.x;
        part->hitBaseOffset.y = part->kindPartParams->hitBaseOffset.y;
        part->hitBaseOffset.z = part->kindPartParams->hitBaseOffset.z;
        part->hitOffset.x = 0.0f;
        part->hitOffset.y = 0.0f;
        part->hitOffset.z = 0.0f;
        part->hitCursorBaseOffset.x = part->kindPartParams->hitCursorBaseOffset.x;
        part->hitCursorBaseOffset.y = part->kindPartParams->hitCursorBaseOffset.y;
        part->hitCursorBaseOffset.z = part->kindPartParams->hitCursorBaseOffset.z;
        part->hitCursorOffset.x = 0.0f;
        part->hitCursorOffset.y = 0.0f;
        part->hitCursorOffset.z = 0.0f;
        part->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
        part->hitOffsetX = setup->hitOffsetX;
        part->poseTable = kpart->poseTable;
        part->poseId = -1;
        part->poseName[0] = 0;
        part->animSpeed = 1.0f;
        part->animSpeed2 = 1.0f;
        part->look = PART_LOOK_NONE;
        part->poseState = 0;
        part->handle = -1;
        part->partName = part->kindPartParams->partName;
        if (--n <= 0) break;
        body = part;
        part++;
        body->nextPart = part;
        kpart++;
    }
    part->nextPart = NULL;

    body = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    unit->talkPose = BtlUnit_GetPoseNameFromType(body, POSE_TYPE_TALK);
    unit->stayPose = BtlUnit_GetPoseNameFromType(body, POSE_TYPE_NEUTRAL);
    unit->moveColorLv = 0xFF;
    unit->moveColorLvDisp = 0xFF;
    btlDispEntAnime(unit);
    unit->countersetWeapon = NULL;
    unit->deleteFunc = NULL;
    switch (unit->currentKind) {
        case UNIT_UNUSED_UFO:
            unit->countersetWeapon = BattleAlloc(0x44);
            break;
        case UNIT_MARIO:
            break;
    }
    BtlUnit_SetParamFromPouch(unit);
    unit->initEventId = 0;
    if (unit->kindParams->initEventCode) {
        evt = evtEntry(unit->kindParams->initEventCode, 10, 0);
        evt->unitId = unit->unitId;
        unit->initEventId = evt->eventId;
    }
    return unit;
}

/**
 * Removes a unit from the battle: kills its wait, phase and damage scripts, drops it from the move order,
 * writes a stolen item back to the field enemy's setup so it can be recovered, releases status icons,
 * poses and memory and frees its slot. Returns FALSE when the unit is not in the table.
 */
BOOL BtlUnit_Delete(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleUnitPart* part;
    s32 j;
    s32 id;

    if (!unit) return FALSE;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        if (BattleGetUnitPtr(wp, i) == unit) break;
    }
    if (i >= 64) return FALSE;

    if (unit->waitEventId) {
        evtDeleteID(unit->waitEventId);
        unit->waitEventId = 0;
    }
    if (unit->phaseEventId) {
        evtDeleteID(unit->phaseEventId);
        unit->phaseEventId = 0;
    }
    if (unit->damageEventId) {
        evtDeleteID(unit->damageEventId);
        unit->damageEventId = 0;
    }
    id = unit->unitId;
    for (j = 0; j < BATTLE_UNIT_MAX; j++) {
        if (wp->movePriorityQueue[j] == id) {
            wp->movePriorityQueue[j] = -1;
        }
    }
    if (unit->flags & BATTLE_UNIT_FLAG_STOLEN_ITEM) {
        NpcBattleInfo* info = wp->fbatInfo->setup;
        if (unit->groupSetupId >= 0) {
            info->stolenItems[unit->groupSetupId] = unit->heldItem;
        }
    }
    BattleStatusEffectDelete(unit);
    BattleStatusIconDelete(unit);
    if (unit->countersetWeapon) {
        if (unit->deleteFunc) {
            unit->deleteFunc(unit);
        } else {
            BattleFree(unit->countersetWeapon);
        }
    }
    if (unit->screenTexBuf) {
        smartFree(unit->screenTexBuf);
        unit->screenTexBuf = NULL;
    }
    for (part = unit->parts; part; part = part->nextPart) {
        if (part->handle != -1) {
            part->handle = -1;
        }
        if (part->poseId != -1) {
            animPoseSetEffect(part->poseId, NULL, 0);
            animPoseRelease(part->poseId);
        }
    }
    BattleFree(unit->parts);
    BattleFree(unit);
    BattleSetUnitPtr(wp, i, NULL);
    return TRUE;
}

/**
 * Creates a unit mid-battle (BtlUnit_Entry) and, with flag 2, puts it first in the move order or, with flag 1,
 * last.
 */
BattleUnit* BtlUnit_Spawn(BattleUnitSetup* setup, s32 flags) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 i;
    s32 id;
    s32 tmp;

    unit = BtlUnit_Entry(setup);
    if (!unit) return unit;
    if (flags & 2) {
        unit->moveState = MOVE_STATE_NORMAL;
        id = unit->unitId;
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            if (id == -1) break;
            if (wp->movePriorityQueue[i] == -1) {
                wp->movePriorityQueue[i] = id;
                break;
            }
            tmp = wp->movePriorityQueue[i];
            wp->movePriorityQueue[i] = id;
            id = tmp;
        }
    } else if (flags & 1) {
        unit->moveState = MOVE_STATE_NORMAL;
        id = unit->unitId;
        for (i = 63; i >= 0; i--) {
            if (id == -1) break;
            if (wp->movePriorityQueue[i] == -1) {
                wp->movePriorityQueue[i] = id;
                break;
            }
            tmp = wp->movePriorityQueue[i];
            wp->movePriorityQueue[i] = id;
            id = tmp;
        }
    }
    return unit;
}

/** Returns the unit's slot index in the battle unit table. */
s32 BtlUnit_GetUnitId(BattleUnit* unit) {
    return unit->unitId;
}

/** Finds the part with the given part number; a negative number or a one-part unit gives the first part, while no match returns NULL. */
BattleUnitPart* BtlUnit_GetPartsPtr(BattleUnit* unit, s32 partNum) {
    BattleUnitPart* part = unit->parts;

    if (partNum < 0) {
        return part;
    }
    if (!part->nextPart) {
        return part;
    }
    for (; part; part = part->nextPart) {
        if (part->kindPartParams->partNum == partNum) {
            return part;
        }
    }
    return part;
}

/** Returns the part number of the unit's main (body) part, the one flagged PART_ATTR_MAIN, or -1. */
s32 BtlUnit_GetBodyPartsId(BattleUnit* unit) {
    BattleUnitPart* part;
    s32 ret = -1;

    if (unit) {
        for (part = unit->parts; part; part = part->nextPart) {
            if (part->attributes & kMainPart) {
                ret = part->kindPartParams->partNum;
                break;
            }
        }
    }
    return ret;
}

/** Reads the unit's battle position (stage coordinates). */
void BtlUnit_GetPos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->position.x;
    *y = unit->position.y;
    *z = unit->position.z;
}

/** Sets the unit's battle position (stage coordinates). */
void BtlUnit_SetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->position.x = x;
    unit->position.y = y;
    unit->position.z = z;
}

/** Moves the unit's battle position by the given amounts. */
void BtlUnit_AddPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->position.x += x;
    unit->position.y += y;
    unit->position.z += z;
}

/**
 * Reads a part's own position field (absolute only for PART_ATTR_WORLD_POSITION parts;
 * see BtlUnit_GetPartsWorldPos).
 */
void BtlUnit_GetPartsPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->position.x;
    *y = part->position.y;
    *z = part->position.z;
}

/** Sets a part's own position field. */
void BtlUnit_SetPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->position.x = x;
    part->position.y = y;
    part->position.z = z;
}

/** Moves a part's own position field by the given amounts. */
void BtlUnit_AddPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->position.x += x;
    part->position.y += y;
    part->position.z += z;
}

/**
 * Stage position of a part: its own position for PART_ATTR_WORLD_POSITION parts,
 * otherwise the unit's position plus the part's offset, with x mirrored by the unit's facing direction.
 */
void BtlUnit_GetPartsWorldPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    BattleUnit* unit = part->owner;
    f32 ux, ox, uy, oy, uz, oz;

    if (part->attributes & kAbsolutePosition) {
        *x = part->position.x;
        *y = part->position.y;
        *z = part->position.z;
    } else {
        BtlUnit_GetPos(unit, &ux, &uy, &uz);
        BtlUnit_GetPartsOffsetPos(part, &ox, &oy, &oz);
        *x = ox * unit->faceDirection + ux;
        *y = uy + oy;
        *z = uz + oz;
    }
}

/**
 * The point attacks aim at on a part: its world position plus the base and extra hit offsets,
 * scaled by the unit's size multiplier and with x mirrored for the side the unit's alliance attacks from.
 */
void BtlUnit_GetHitPos(BattleUnit* unit, BattleUnitPart* part, f32* x, f32* y, f32* z) {
    f32 y_multiply = 1.0f;
    s32 direction = _battleWorkPointer->alliances[unit->alliance].attackDirection;
    BtlUnit_GetPartsWorldPos(part, x, y, z);
    //add hit base offset
    *x = ((f32)direction * (part->hitBaseOffset.x * unit->sizeMultiplier)) + *x;
    *y = (y_multiply * (part->hitBaseOffset.y * unit->sizeMultiplier)) + *y;
    *z = (part->hitBaseOffset.z * unit->sizeMultiplier) + *z;
    //add hit offset
    *x = ((f32)direction * (part->hitOffset.x * unit->sizeMultiplier)) + *x;
    *y = (y_multiply * (part->hitOffset.y * unit->sizeMultiplier)) + *y;
    *z = (part->hitOffset.z * unit->sizeMultiplier) + *z;
}

/** Sets the extra hit offset added on top of a part's base hit offset. */
void BtlUnit_SetHitOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->hitOffset.x = x;
    part->hitOffset.y = y;
    part->hitOffset.z = z;
}

/** Sets the extra offset of the target cursor shown over a part. */
void BtlUnit_SetHitCursorOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->hitCursorOffset.x = x;
    part->hitCursorOffset.y = y;
    part->hitCursorOffset.z = z;
}

/** Reads the unit's home position, the spot it returns to after acting. */
void BtlUnit_GetHomePos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->homePosition.x;
    *y = unit->homePosition.y;
    *z = unit->homePosition.z;
}

/** Sets the unit's home position, the spot it returns to after acting. */
void BtlUnit_SetHomePos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->homePosition.x = x;
    unit->homePosition.y = y;
    unit->homePosition.z = z;
}

/** Moves the unit's home position by the given amounts. */
void BtlUnit_AddHomePos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->homePosition.x += x;
    unit->homePosition.y += y;
    unit->homePosition.z += z;
}

/** Sets a part's home position. */
void BtlUnit_SetPartsHomePos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->homePosition.x = x;
    part->homePosition.y = y;
    part->homePosition.z = z;
}

/** Sets the unit's rotation in degrees, each axis normalised to 0..360 with 360 folded to 0. */
void BtlUnit_SetRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 new_x, new_y, new_z;

    new_x = reviseAngle(x);
    if (new_x >= 360.0f) {
        new_x = 0.0f;
    }
    unit->rotation.x = new_x;

    new_y = reviseAngle(y);
    if (new_y >= 360.0f) {
        new_y = 0.0f;
    }
    unit->rotation.y = new_y;

    new_z = reviseAngle(z);
    if (new_z >= 360.0f) {
        new_z = 0.0f;
    }
    unit->rotation.z = new_z;
}

/** Reads the unit's rotation in degrees. */
void BtlUnit_GetRotate(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->rotation.x;
    *y = unit->rotation.y;
    *z = unit->rotation.z;
}

/** Turns the unit by the given degrees, normalising each axis to 0..360. */
void BtlUnit_AddRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->rotation.x = reviseAngle(unit->rotation.x + x);
    if (unit->rotation.x >= 360.0f) {
        unit->rotation.x = 0.0f;
    }
    unit->rotation.y = reviseAngle(unit->rotation.y + y);
    if (unit->rotation.y >= 360.0f) {
        unit->rotation.y = 0.0f;
    }
    unit->rotation.z = reviseAngle(unit->rotation.z + z);
    if (unit->rotation.z >= 360.0f) {
        unit->rotation.z = 0.0f;
    }
}

/** Sets a part's rotation in degrees (not normalised). */
void BtlUnit_SetPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotation.x = x;
    part->rotation.y = y;
    part->rotation.z = z;
}

/** Reads a part's rotation in degrees. */
void BtlUnit_GetPartsRotate(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->rotation.x;
    *y = part->rotation.y;
    *z = part->rotation.z;
}

/** Turns a part by the given degrees, wrapping each axis back into 0..360 once. */
void BtlUnit_AddPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotation.x += x;
    if (part->rotation.x > 360.0f) {
        part->rotation.x -= 360.0f;
    } else if (part->rotation.x < 0.0f) {
        part->rotation.x += 360.0f;
    }

    part->rotation.y += y;
    if (part->rotation.y > 360.0f) {
        part->rotation.y -= 360.0f;
    } else if (part->rotation.y < 0.0f) {
        part->rotation.y += 360.0f;
    }

    part->rotation.z += z;
    if (part->rotation.z > 360.0f) {
        part->rotation.z -= 360.0f;
    } else if (part->rotation.z < 0.0f) {
        part->rotation.z += 360.0f;
    }
}

/** Sets the unit's base rotation in degrees (normalised), applied under its animated rotation. */
void BtlUnit_SetBaseRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->baseRotation.x = reviseAngle(x);
    unit->baseRotation.y = reviseAngle(y);
    unit->baseRotation.z = reviseAngle(z);
}

/** Reads the unit's base rotation in degrees. */
void BtlUnit_GetBaseRotate(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->baseRotation.x;
    *y = unit->baseRotation.y;
    *z = unit->baseRotation.z;
}

/** Sets a part's base rotation in degrees. */
void BtlUnit_SetPartsBaseRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->baseRotation.x = x;
    part->baseRotation.y = y;
    part->baseRotation.z = z;
}

/** Reads a part's base rotation in degrees. */
void BtlUnit_GetPartsBaseRotate(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->baseRotation.x;
    *y = part->baseRotation.y;
    *z = part->baseRotation.z;
}

/**
 * Sets the pivot offset the unit rotates around.
 * Like the other setters here, an axis passed as -250000000.0 (the scripts' "keep" value) is left unchanged.
 */
void BtlUnit_SetRotateOffset(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->rotationOffset.x = x;
    }
    if (noValue != y) {
        unit->rotationOffset.y = y;
    }
    if (noValue != z) {
        unit->rotationOffset.z = z;
    }
}

/** Sets the pivot offset a part rotates around; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->rotationOffset.x = x;
    }
    if (noValue != y) {
        part->rotationOffset.y = y;
    }
    if (noValue != z) {
        part->rotationOffset.z = z;
    }
}

/** Moves a part's rotation pivot offset by the given amounts. */
void BtlUnit_AddPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotationOffset.x += x;
    part->rotationOffset.y += y;
    part->rotationOffset.z += z;
}

/** Sets the unit's base scale (its normal size, multiplied with the animated scale); -250000000.0 keeps an axis. */
void BtlUnit_SetBaseScale(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->baseScale.x = x;
    }
    if (noValue != y) {
        unit->baseScale.y = y;
    }
    if (noValue != z) {
        unit->baseScale.z = z;
    }
}

/** Reads the unit's animated scale. */
void BtlUnit_GetScale(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mScale.x;
    *y = unit->mScale.y;
    *z = unit->mScale.z;
}

/** Sets the unit's animated scale; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetScale(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mScale.x = x;
    }
    if (noValue != y) {
        unit->mScale.y = y;
    }
    if (noValue != z) {
        unit->mScale.z = z;
    }
}

/** Adds to the unit's animated scale; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_AddScale(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mScale.x += x;
    }
    if (noValue != y) {
        unit->mScale.y += y;
    }
    if (noValue != z) {
        unit->mScale.z += z;
    }
}

/** Sets a part's base scale; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetPartsBaseScale(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->baseScale.x = x;
    }
    if (noValue != y) {
        part->baseScale.y = y;
    }
    if (noValue != z) {
        part->baseScale.z = z;
    }
}

/** Sets a part's animated scale; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetPartsScale(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mScale.x = x;
    }
    if (noValue != y) {
        part->mScale.y = y;
    }
    if (noValue != z) {
        part->mScale.z = z;
    }
}

/** Adds to a part's animated scale; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_AddPartsScale(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mScale.x += x;
    }
    if (noValue != y) {
        part->mScale.y += y;
    }
    if (noValue != z) {
        part->mScale.z += z;
    }
}

/** Returns the unit's width, copied from its kind data at entry. */
s32 BtlUnit_GetWidth(BattleUnit* unit) {
    return unit->width;
}

/** Returns the unit's current height, copied from its kind data at entry unless a script changed it. */
s32 BtlUnit_GetHeight(BattleUnit* unit) {
    return unit->height;
}

/** Overrides the unit's height. */
void BtlUnit_SetHeight(BattleUnit* unit, s16 height) {
    unit->height = height;
}

/**
 * Sets the unit's position offset field; an axis passed as -250000000.0 is left unchanged.
 * Entry sets it to the start position.
 */
void BtlUnit_SetOffsetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->positionOffset.x = x;
    }
    if (noValue != y) {
        unit->positionOffset.y = y;
    }
    if (noValue != z) {
        unit->positionOffset.z = z;
    }
}

/** Reads a part's offset from its unit's position. */
void BtlUnit_GetPartsOffsetPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->positionOffset.x;
    *y = part->positionOffset.y;
    *z = part->positionOffset.z;
}

/** Sets a part's offset from its unit's position; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->positionOffset.x = x;
    }
    if (noValue != y) {
        part->positionOffset.y = y;
    }
    if (noValue != z) {
        part->positionOffset.z = z;
    }
}

/** Moves a part's offset from its unit's position by the given amounts. */
void BtlUnit_AddPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->positionOffset.x += x;
    part->positionOffset.y += y;
    part->positionOffset.z += z;
}

/** Sets the unit's draw offset (displayOffset); -250000000.0 keeps an axis. */
void BtlUnit_SetDispOffset(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->displayOffset.x = x;
    }
    if (noValue != y) {
        unit->displayOffset.y = y;
    }
    if (noValue != z) {
        unit->displayOffset.z = z;
    }
}

/** Sets a part's draw offset (displayOffset); an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_SetPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->displayOffset.x = x;
    }
    if (noValue != y) {
        part->displayOffset.y = y;
    }
    if (noValue != z) {
        part->displayOffset.z = z;
    }
}

/** Adds to a part's draw offset; an axis passed as -250000000.0 is left unchanged. */
void BtlUnit_AddPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->displayOffset.x += x;
    }
    if (noValue != y) {
        part->displayOffset.y += y;
    }
    if (noValue != z) {
        part->displayOffset.z += z;
    }
}

/** Sets where the unit's current scripted move (walk, jump) starts; -250000000.0 keeps an axis. */
void BtlUnit_SetMoveStartPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->moveStartPos.x = x;
    }
    if (noValue != y) {
        unit->moveStartPos.y = y;
    }
    if (noValue != z) {
        unit->moveStartPos.z = z;
    }
}

/** Sets where a part's current scripted move starts; -250000000.0 keeps an axis. */
void BtlUnit_SetPartsMoveStartPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->moveStartPos.x = x;
    }
    if (noValue != y) {
        part->moveStartPos.y = y;
    }
    if (noValue != z) {
        part->moveStartPos.z = z;
    }
}

/** Sets the position the unit's current scripted move has reached; -250000000.0 keeps an axis. */
void BtlUnit_SetMoveCurrentPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->moveCurrentPos.x = x;
    }
    if (noValue != y) {
        unit->moveCurrentPos.y = y;
    }
    if (noValue != z) {
        unit->moveCurrentPos.z = z;
    }
}

/** Sets the position a part's current scripted move has reached; -250000000.0 keeps an axis. */
void BtlUnit_SetPartsMoveCurrentPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->moveCurrentPos.x = x;
    }
    if (noValue != y) {
        part->moveCurrentPos.y = y;
    }
    if (noValue != z) {
        part->moveCurrentPos.z = z;
    }
}

/** Sets where the unit's current scripted move ends; -250000000.0 keeps an axis. */
void BtlUnit_SetMoveTargetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->moveTargetPos.x = x;
    }
    if (noValue != y) {
        unit->moveTargetPos.y = y;
    }
    if (noValue != z) {
        unit->moveTargetPos.z = z;
    }
}

/** Sets where a part's current scripted move ends; -250000000.0 keeps an axis. */
void BtlUnit_SetPartsMoveTargetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->moveTargetPos.x = x;
    }
    if (noValue != y) {
        part->moveTargetPos.y = y;
    }
    if (noValue != z) {
        part->moveTargetPos.z = z;
    }
}

/** Sets the gravity used by the unit's scripted jumps and falls. */
void BtlUnit_SetFallAccel(BattleUnit* unit, f32 fallAccel) {
    unit->fallAccel = fallAccel;
}

/** Sets the gravity used by a part's scripted jumps and falls. */
void BtlUnit_SetPartsFallAccel(BattleUnitPart* part, f32 fallAccel) {
    part->fallAccel = fallAccel;
}

/** Sets the horizontal speed of the unit's scripted moves. */
void BtlUnit_SetMoveSpeed(BattleUnit* unit, f32 moveSpeedXZ) {
    unit->moveSpeedXZ = moveSpeedXZ;
}

/** Sets the horizontal speed of a part's scripted moves. */
void BtlUnit_SetPartsMoveSpeed(BattleUnitPart* part, f32 moveSpeedXZ) {
    part->moveSpeedXZ = moveSpeedXZ;
}

/** Sets the vertical speed of the unit's scripted jumps. */
void BtlUnit_SetJumpSpeed(BattleUnit* unit, f32 moveSpeedY) {
    unit->moveSpeedY = moveSpeedY;
}

/** Returns the unit's alliance: which side it fights on. */
s8 BtlUnit_GetBelong(BattleUnit* unit) {
    return unit->alliance;
}

/**
 * Resets the unit's move status (BtlUnit_ResetMoveStatus) when a status changes outside its own action
 * while it still has all its moves this turn, so a new move count takes effect.
 */
void _CheckMoveCount(BattleUnit* unit) {
    s8 maxmoves;

    if (!(unit->tokenFlags & kCurrentlyActing)) {
        maxmoves = unit->maxMovesThisTurn;
        if (maxmoves > 0 && maxmoves == unit->movesRemaining) {
            BtlUnit_ResetMoveStatus(unit);
        }
    }
}

/**
 * Reads a status effect's turns and strength into the optional outputs;
 * for statuses with no strength (sleep, stop, dizzy, ...) both get the turn count.
 * A size, attack or defense change of the other sign reads as 0.
 */
#if VERSION <= VERSION_DEMO_JP
void BtlUnit_GetStatus(BattleUnit* unit, StatusEffectType type, s8* turns, s8* strength) {
#else
void BtlUnit_GetStatus(BattleUnit* unit, s8 type, s8* turns, s8* strength) {
#endif

    s8 status_turns = 0;
    s8 status_strength = 0;

#if VERSION <= VERSION_DEMO_JP
    switch ((s8)type) {
#else
    switch (type) {
#endif
        case STATUS_SLEEP:
            status_strength = unit->sleepTurns;
            status_turns = status_strength;
            break;
        case STATUS_STOP:
            status_strength = unit->stopTurns;
            status_turns = status_strength;
            break;
        case STATUS_DIZZY:
            status_strength = unit->dizzyTurns;
            status_turns = status_strength;
            break;
        case STATUS_POISON:
            status_turns = unit->poisonTurns;
            status_strength = unit->poisonStrength;
            break;
        case STATUS_CONFUSE:
            status_strength = unit->confusionTurns;
            status_turns = status_strength;
            break;
        case STATUS_ELECTRIC:
            status_strength = unit->electricTurns;
            status_turns = status_strength;
            break;
        case STATUS_DODGY:
            status_strength = unit->dodgyTurns;
            status_turns = status_strength;
            break;
        case STATUS_BURN:
            status_strength = unit->burnTurns;
            status_turns = status_strength;
            break;
        case STATUS_FREEZE:
            status_strength = unit->freezeTurns;
            status_turns = status_strength;
            break;
        case STATUS_HUGE:
            if (unit->sizeChangeStrength > 0) {
                status_turns = unit->sizeChangeTurns;
                status_strength = unit->sizeChangeStrength;
            }
            break;
        case STATUS_TINY:
            if (unit->sizeChangeStrength < 0) {
                status_turns = unit->sizeChangeTurns;
                status_strength = unit->sizeChangeStrength;
            }
            break;
        case STATUS_ATTACK_UP:
            if (unit->atkChangeStrength > 0) {
                status_turns = unit->atkChangeTurns;
                status_strength = unit->atkChangeStrength;
            }
            break;
        case STATUS_ATTACK_DOWN:
            if (unit->atkChangeStrength < 0) {
                status_turns = unit->atkChangeTurns;
                status_strength = unit->atkChangeStrength;
            }
            break;
        case STATUS_DEFENSE_UP:
            if (unit->defChangeStrength > 0) {
                status_turns = unit->defChangeTurns;
                status_strength = unit->defChangeStrength;
            }
            break;
        case STATUS_DEFENSE_DOWN:
            if (unit->defChangeStrength < 0) {
                status_turns = unit->defChangeTurns;
                status_strength = unit->defChangeStrength;
            }
            break;
        case STATUS_FAST:
            status_strength = unit->fastTurns;
            status_turns = status_strength;
            break;
        case STATUS_SLOW:
            status_strength = unit->slowTurns;
            status_turns = status_strength;
            break;
        case STATUS_CHARGE:
            status_strength = unit->chargeStrength;
            status_turns = status_strength;
            break;
        case STATUS_ALLERGIC:
            status_strength = unit->allergicTurns;
            status_turns = status_strength;
            break;
        case STATUS_FLIPPED:
            status_strength = unit->flippedTurns;
            status_turns = status_strength;
            break;
        case STATUS_INVISIBLE:
            status_strength = unit->invisibleTurns;
            status_turns = status_strength;
            break;
        case STATUS_PAYBACK:
            status_strength = unit->paybackTurns;
            status_turns = status_strength;
            break;
        case STATUS_HOLD_FAST:
            status_strength = unit->holdFastTurns;
            status_turns = status_strength;
            break;
        case STATUS_HP_REGEN:
            status_turns = unit->hpRegenTurns;
            status_strength = unit->hpRegenStrength;
            break;
        case STATUS_FP_REGEN:
            status_turns = unit->fpRegenTurns;
            status_strength = unit->fpRegenStrength;
            break;
        case STATUS_INSTANT_KILL:
            status_strength = unit->instantKillStrength;
            status_turns = status_strength;
            break;
        default:
            break;
    }
    if (turns) {
        *turns = status_turns;
    }
    if (strength) {
        *strength = status_strength;
    }
}

/**
 * Stores a status effect's turns and strength (Charge adds to its strength, capped 0..99).
 * Burn and freeze cancel each other, as do Fast and Slow, without applying the new one,
 * and a zero strength for a size, attack or defense change clears an effect of that sign.
 * Cancellation and zero-strength clear paths return FALSE; unhandled types return TRUE without changing a status.
 * Retail versions also return FALSE when Charge drops below 0
 * or Allergic or Payback is set to 0 turns, which the demos treat as set.
 */
#if VERSION <= VERSION_DEMO_JP
BOOL BtlUnit_SetStatus(BattleUnit* unit, StatusEffectType type, s8 turns, s8 strength) {
    switch ((s8)type) {
#else
BOOL BtlUnit_SetStatus(BattleUnit* unit, s8 type, s8 turns, s8 strength) {
    switch (type) {
#endif
        case STATUS_SLEEP:
            unit->sleepTurns = turns;
            unit->sleepStrength = 0;
            break;
        case STATUS_STOP:
            unit->stopTurns = turns;
            break;
        case STATUS_DIZZY:
            unit->dizzyTurns = turns;
            break;
        case STATUS_POISON:
            unit->poisonTurns = turns;
            unit->poisonStrength = strength;
            break;
        case STATUS_CONFUSE:
            unit->confusionTurns = turns;
            break;
        case STATUS_ELECTRIC:
            unit->electricTurns = turns;
            break;
        case STATUS_DODGY:
            unit->dodgyTurns = turns;
            break;
        case STATUS_BURN:
            if (unit->freezeTurns) {
                unit->freezeTurns = 0;
                return FALSE;
            }
            unit->burnTurns = turns;
            break;
        case STATUS_FREEZE:
            if (unit->burnTurns) {
                unit->burnTurns = 0;
                return FALSE;
            }
            unit->freezeTurns = turns;
            break;
        case STATUS_HUGE:
        case STATUS_TINY:
            if (!strength) {
#if VERSION <= VERSION_DEMO_JP
                if ((s8)type == STATUS_HUGE) {
#else
                if (type == STATUS_HUGE) {
#endif
                    if (unit->sizeChangeStrength >= 0) {
                        unit->sizeChangeTurns = 0;
                        unit->sizeChangeStrength = 0;
                        return FALSE;
                    }
                } else if (unit->sizeChangeStrength <= 0) {
                    unit->sizeChangeTurns = 0;
                    unit->sizeChangeStrength = 0;
                    return FALSE;
                }
                return FALSE;
            }
            unit->sizeChangeTurns = turns;
            unit->sizeChangeStrength = strength;
            break;
        case STATUS_ATTACK_UP:
            if (!strength) {
                if (unit->atkChangeStrength >= 0) {
                    unit->atkChangeTurns = 0;
                    unit->atkChangeStrength = 0;
                }
                return FALSE;
            }
            unit->atkChangeTurns = turns;
            unit->atkChangeStrength = strength;
            break;
        case STATUS_ATTACK_DOWN:
            if (!strength) {
                if (unit->atkChangeStrength <= 0) {
                    unit->atkChangeTurns = 0;
                    unit->atkChangeStrength = 0;
                }
                return FALSE;
            }
            unit->atkChangeTurns = turns;
            unit->atkChangeStrength = strength;
            break;
        case STATUS_DEFENSE_UP:
        case STATUS_DEFENSE_DOWN:
            if (strength > 0) {
                if (unit->defChangeStrength < 0) {
                    unit->defChangeTurns = 0;
                    unit->defChangeStrength = 0;
                    return FALSE;
                }
                unit->defChangeTurns = turns;
                unit->defChangeStrength = strength;
            } else if (strength == 0) {
#if VERSION <= VERSION_DEMO_JP
                if ((s8)type == STATUS_DEFENSE_DOWN) {
#else
                if (type == STATUS_DEFENSE_DOWN) {
#endif
                    if (unit->defChangeStrength <= 0) {
                        unit->defChangeTurns = 0;
                        unit->defChangeStrength = 0;
                    }
                } else if (unit->defChangeStrength >= 0) {
                    unit->defChangeTurns = 0;
                    unit->defChangeStrength = 0;
                }
                return FALSE;
            } else {
                if (unit->defChangeStrength > 0) {
                    unit->defChangeTurns = 0;
                    unit->defChangeStrength = 0;
                    return FALSE;
                }
                unit->defChangeTurns = turns;
                unit->defChangeStrength = strength;
            }
            break;
        case STATUS_CHARGE:
            unit->chargeStrength += strength;
            if (unit->chargeStrength < 0) {
                unit->chargeStrength = 0;
#if VERSION <= VERSION_DEMO_JP
            } else if (unit->chargeStrength > 99) {
#else
                return FALSE;
            }
            if (unit->chargeStrength > 99) {
#endif
                unit->chargeStrength = 99;
            }
            break;
        case STATUS_ALLERGIC:
#if VERSION >= VERSION_JP
            if (turns > 0) {
                unit->allergicTurns = turns;
            } else {
                unit->allergicTurns = 0;
                return FALSE;
            }
#else
            unit->allergicTurns = turns;
#endif
            break;
        case STATUS_FLIPPED:
            unit->flippedTurns = turns;
            break;
        case STATUS_INVISIBLE:
            unit->invisibleTurns = turns;
            break;
        case STATUS_PAYBACK:
#if VERSION >= VERSION_JP
            if (turns > 0) {
                unit->paybackTurns = turns;
            } else {
                unit->paybackTurns = 0;
                return FALSE;
            }
#else
            unit->paybackTurns = turns;
#endif
            break;
        case STATUS_HOLD_FAST:
            unit->holdFastTurns = turns;
            break;
        case STATUS_FAST:
            if (unit->slowTurns > 0) {
                if (turns > 0) {
                    unit->slowTurns = 0;
                    _CheckMoveCount(unit);
                    return FALSE;
                }
                unit->fastTurns = turns;
            } else {
                unit->fastTurns = turns;
            }
            _CheckMoveCount(unit);
            break;
        case STATUS_SLOW:
            if (unit->fastTurns > 0) {
                if (turns > 0) {
                    unit->fastTurns = 0;
                    _CheckMoveCount(unit);
                    return FALSE;
                }
                unit->slowTurns = turns;
            } else {
                unit->slowTurns = turns;
            }
            _CheckMoveCount(unit);
            break;
        case STATUS_HP_REGEN:
            unit->hpRegenTurns = turns;
            unit->hpRegenStrength = strength;
            break;
        case STATUS_FP_REGEN:
            unit->fpRegenTurns = turns;
            unit->fpRegenStrength = strength;
            break;
        case STATUS_INSTANT_KILL:
            unit->instantKillStrength = turns;
            break;
        default:
            break;
    }
    return TRUE;
}

/** clears the 0x1E bytes of status turns and strengths from sleepTurns on */
// the 0x1E bytes from sleepTurns are every status's turns and strength
void BtlUnit_ClearStatus(BattleUnit* unit) {
    memset(&unit->sleepTurns, 0, sizeof(BtlStatusCopy));
    BtlUnit_SetStatus(unit, STATUS_CHARGE, 1, -99);
#if VERSION >= VERSION_JP
    BattleStatusChangeInfoWorkInit(unit);
#endif
}

/**
 * Counts a status effect down by one turn at the end of the unit's turn (sleep, timed statuses, size,
 * attack and defense changes, poison and regen), clearing its strength when the turns run out.
 * Returns TRUE when the status just wore off; FALSE when it continues, was not active, or the unit is KO'd.
 */
#if VERSION <= VERSION_DEMO_JP
BOOL BtlUnit_CheckRecoveryStatus(BattleUnit* unit, StatusEffectType type) {
#else
BOOL BtlUnit_CheckRecoveryStatus(BattleUnit* unit, s8 type) {
#endif
    s8 turns, strength;

    if (!unit) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        return FALSE;
    }
    if (unit->currentHp <= 0) {
        return FALSE;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    if (!turns) {
        return FALSE;
    }
#if VERSION <= VERSION_DEMO_JP
    switch ((s8)type) {
#else
    switch (type) {
#endif
        case STATUS_SLEEP:
            if (turns > 1) {
                turns--;
            } else {
                turns = 0;
            }
            unit->sleepTurns = turns;
            if (!turns) {
                return TRUE;
            }
            return FALSE;
        case STATUS_ALLERGIC:
        case STATUS_STOP:
        case STATUS_DIZZY:
        case STATUS_CONFUSE:
        case STATUS_ELECTRIC:
        case STATUS_DODGY:
        case STATUS_BURN:
        case STATUS_FREEZE:
        case STATUS_INVISIBLE:
        case STATUS_FAST:
        case STATUS_SLOW:
        case STATUS_PAYBACK:
        case STATUS_HOLD_FAST:
            if (turns >= 0) {
                turns--;
            } else {
                turns++;
            }
            if (strength >= 0) {
                strength--;
            } else {
                strength++;
            }
            if (!turns) {
                strength = 0;
            }
            BtlUnit_SetStatus(unit, type, turns, strength);
            if (!turns) {
                return TRUE;
            }
            return FALSE;
        case STATUS_HUGE:
        case STATUS_TINY:
            if (turns > 0) {
                turns--;
            } else if (turns < 0) {
                turns++;
            } else {
                return FALSE;
            }
            if (!turns) {
                strength = 0;
            }
            unit->sizeChangeTurns = turns;
            unit->sizeChangeStrength = strength;
            if (!turns) {
                return TRUE;
            }
            return FALSE;
        case STATUS_ATTACK_UP:
        case STATUS_ATTACK_DOWN:
            if (turns > 0) {
                turns--;
            } else if (turns < 0) {
                turns++;
            } else {
                return FALSE;
            }
            if (!turns) {
                strength = 0;
            }
            unit->atkChangeTurns = turns;
            unit->atkChangeStrength = strength;
            if (!turns) {
                return TRUE;
            }
            return FALSE;
        case STATUS_POISON:
        case STATUS_DEFENSE_UP:
        case STATUS_DEFENSE_DOWN:
        case STATUS_HP_REGEN:
        case STATUS_FP_REGEN:
            if (turns >= 0) {
                turns--;
            } else {
                turns++;
            }
            if (!turns) {
                strength = 0;
            }
            BtlUnit_SetStatus(unit, type, turns, strength);
            if (!turns) {
                return TRUE;
            }
            return FALSE;
        default:
            return FALSE;
    }
}

/** Returns TRUE while the status effect has turns left; Electric also counts as active when the unit wears Zap Tap. */
#if VERSION <= VERSION_DEMO_JP
BOOL BtlUnit_CheckStatus(BattleUnit* unit, StatusEffectType type) {
#else
BOOL BtlUnit_CheckStatus(BattleUnit* unit, s8 type) {
#endif
    s8 turns, strength;

    if (!unit) {
        return FALSE;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    if (turns) {
        return TRUE;
    }
#if VERSION <= VERSION_DEMO_JP
    if ((s8)type == STATUS_ELECTRIC && unit->badgesEquipped.ZapTap) {
#else
    if (type == STATUS_ELECTRIC && unit->badgesEquipped.ZapTap) {
#endif
        return TRUE;
    }
    return FALSE;
}

/** Tests the unit's status flags (flipped, danger, peril, ...); a NULL unit gives 0. */
s32 BtlUnit_CheckStatusFlag(BattleUnit* unit, s32 flags) {
    if (unit) {
        return unit->statusFlags & flags;
    }
    return 0;
}

/** Sets status flags (flipped, danger, peril, ...) on the unit. */
void BtlUnit_OnStatusFlag(BattleUnit* unit, s32 flags) {
    unit->statusFlags |= flags;
}

/** Clears status flags on the unit. */
void BtlUnit_OffStatusFlag(BattleUnit* unit, s32 flags) {
    unit->statusFlags &= ~flags;
}

s32 BtlUnit_CheckUnitFlag(BattleUnit* unit, s32 flags) {
    return unit->flags & flags;
}

void BtlUnit_OnUnitFlag(BattleUnit* unit, s32 flags) {
    unit->flags |= flags;
}

void BtlUnit_OffUnitFlag(BattleUnit* unit, s32 flags) {
    unit->flags &= ~flags;
}

/**
 * Looks up the animation name for a pose type in the part's pose table,
 * taking the first entry that matches the type or its group's fallback (BTL_POSE_NORMAL,
 * BTL_POSE_DAMAGE or BTL_POSE_STAY_DEFAULT). NULL when nothing fits.
 */
const char* BtlUnit_GetPoseNameFromType(BattleUnitPart* part, s32 type) {
    PoseTableEntry* pose = part->poseTable;
    s32 alt;

    if (type <= POSE_TYPE_NORMAL) {
        alt = POSE_TYPE_NORMAL;
    } else if (type < POSE_TYPE_WALK) {
        alt = POSE_TYPE_DAMAGE;
    } else if (type < POSE_TYPE_MAX) {
        alt = POSE_TYPE_ACTION;
    } else {
        return NULL;
    }
    while (pose) {
        if (pose->poseType >= POSE_TYPE_MAX) {
            return NULL;
        }
        if (pose->poseType != type && pose->poseType != alt) {
            pose++;
            continue;
        }
        return pose->poseName;
    }
    return NULL;
}

void BtlUnit_SetAnimType(BattleUnitPart* part, s32 type) {
    const char* name = BtlUnit_GetPoseNameFromType(part, type);

    if (type >= POSE_TYPE_DAMAGE_FIRE && type < POSE_TYPE_WALK) {
        btlDispChangeAnime(part, name, TRUE);
    } else {
        btlDispChangeAnime(part, name, FALSE);
    }
    switch (type) {
        case POSE_TYPE_DAMAGE_FIRE:
        case POSE_TYPE_DAMAGE_FIRE_2_UNUSED:
            part->animFlags |= PART_ANIM_BURNT;
            break;
    }
}

/** Plays the named animation on a part. */
void BtlUnit_SetAnim(BattleUnitPart* part, const char* name) {
    btlDispChangeAnime(part, name, 0);
}

/** Plays the animation for a pose type on the unit's main (body) part. */
void BtlUnit_SetBodyAnimType(BattleUnit* unit, s32 type) {
    s32 id = BtlUnit_GetBodyPartsId(unit);
    BtlUnit_SetAnimType(BtlUnit_GetPartsPtr(unit, id), type);
}

/** Plays the named animation on the unit's main (body) part. */
void BtlUnit_SetBodyAnim(BattleUnit* unit, const char* name) {
    s32 id = BtlUnit_GetBodyPartsId(unit);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, id), name, 0);
}

/** Switches the unit's talking part to its talk pose; used while the unit speaks in battle dialogue. */
void BtlUnit_ChangeTalkAnim(s32 unitId) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, unit->talkPartId), unit->talkPose, 0);
}

/** Switches the unit's talking part back to its standing pose after it speaks. */
void BtlUnit_ChangeStayAnim(s32 unitId) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, unit->talkPartId), unit->stayPose, 0);
}

/**
 * Where a speech bubble's tail (toge, "thorn") points for this unit:
 * its position plus the kind's talk offset and the unit's own tail offset, x mirrored by facing.
 */
void BtlUnit_GetTalkTogePos(s32 unitId, f32* x, f32* y, f32* z) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    f32 posX, posY, posZ;

    BtlUnit_GetPos(unit, &posX, &posY, &posZ);
    posX += unit->faceDirection * (unit->kindParams->talkTogeBaseOffset.x + unit->togeOffset.x);
    posY += unit->kindParams->talkTogeBaseOffset.y + unit->togeOffset.y;
    posZ += unit->kindParams->talkTogeBaseOffset.z + unit->togeOffset.z;
    *x = posX;
    *y = posY;
    *z = posZ;
}

/** Returns the opposing alliance: 0 and 1 swap, any other alliance is returned unchanged. */
s32 BtlUnit_GetEnemyBelong(BattleUnit* unit) {
    s32 belong = unit->alliance;
    switch (belong) {
        case ALLIANCE_PLAYER:
            belong = ALLIANCE_ENEMY;
            break;
        case ALLIANCE_ENEMY:
            belong = ALLIANCE_PLAYER;
            break;
    }
    return belong;
}

/** Returns the HP damage the unit took from the last hit. */
s32 BtlUnit_GetHitDamage(BattleUnit* unit) {
    return unit->hpDamageTaken;
}

/** Returns the HP damage the unit has taken so far in the current attack. */
s32 BtlUnit_GetTotalHitDamage(BattleUnit* unit) {
    return unit->totalHpDamageTaken;
}

/** Sets (usually resets) the HP damage counted for the current attack. */
void BtlUnit_SetTotalHitDamage(BattleUnit* unit, s16 damage) {
    unit->totalHpDamageTaken = damage;
}

/**
 * Whether the unit can Guard or Superguard an incoming attack:
 * it must be on the player's side, not the Shell Shield, and not asleep, stopped, frozen, KO'd or flipped.
 */
BOOL BtlUnit_GetACPossibility(BattleUnit* unit) {
    if (unit->alliance) {
        return FALSE;
    }
    if (unit->currentKind == UNIT_SHELL_SHIELD) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        return FALSE;
    }
    return !BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_FLIPPED);
}

/** Looks up an entry of the unit kind's tagged data table (scripts and parameters keyed by tag); NULL when absent. */
void* BtlUnit_GetData(BattleUnit* unit, s32 tag) {
    BattleDataEntry* entry = unit->dataTable;
    if (!entry) {
        return NULL;
    }
    while (entry->tag) {
        if (entry->tag == tag) {
            return entry->data;
        }
        entry++;
    }
    return NULL;
}

/** Whether the unit kind's tagged data table has an entry with this tag. */
BOOL BtlUnit_CheckData(BattleUnit* unit, s32 tag) {
    BattleDataEntry* entry = unit->dataTable;
    if (!entry) {
        return FALSE;
    }
    while (entry->tag) {
        if (entry->tag == tag) {
            return TRUE;
        }
        entry++;
    }
    return FALSE;
}

/** Whether the unit's statuses let it guard; the same test as BtlUnit_CanActStatus. */
BOOL BtlUnit_CanGuardStatus(BattleUnit* unit) {
    return BtlUnit_CanActStatus(unit);
}

/** Whether the unit's statuses let it act: not asleep, stopped, frozen, KO'd or flipped. */
BOOL BtlUnit_CanActStatus(BattleUnit* unit) {
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        return FALSE;
    }
    return !BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_FLIPPED);
}

/**
 * Loads Mario's HP and FP, or a partner's HP, from the save data (pouch) into the battle unit;
 * partners also get back the size, status flags
 * and status turns saved in the battle's party info (and, in US and PAL, the disabled commands) and are KO'd
 * if their HP is 0.
 */
void BtlUnit_SetParamFromPouch(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    BattleWorkPartyInfo* info;

    if (unit->currentKind == UNIT_MARIO) {
        unit->currentHp = pouch->currentHP;
        unit->currentFp = pouch->currentFP;
        BtlUnit_ReviseHpFp(unit);
    } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        unit->currentHp = *(s16*)((u8*)pouch->partyData + BattleTransPartyId(unit->currentKind) * sizeof(PouchPartyData) + offsetof(PouchPartyData, currentHP)); // FAKE: byte-offset load so the field offset folds into the index (mulli/addi/lhax), as in the target
        BtlUnit_ReviseHpFp(unit);
        info = &wp->partyInfo[unit->currentKind - TYPE_PARTNER_MIN];
        unit->sizeMultiplier = info->sizeMultiplier;
        unit->statusFlags = info->statusFlags;
        unit->statusTurns = info->statusTurns;
#if VERSION >= VERSION_US
        unit->usCommandDisabled[0] = info->usCommandDisabled[0];
        unit->usCommandDisabled[1] = info->usCommandDisabled[1];
#endif
        if (unit->currentHp <= 0) {
            BtlUnit_SetStatus(unit, STATUS_INSTANT_KILL, 1, 1);
        }
    }
}

/**
 * Recomputes max HP from base HP plus 5 per HP Plus and clamps current HP to it.
 * Mario and partners take base HP from the pouch; partners also reload current HP from it.
 * For Mario and non-partner units, also recomputes max FP plus 5 per FP Plus and clamps current FP.
 */
void BtlUnit_ReviseHpFp(BattleUnit* unit) {
    PouchData* pouch = pouchGetPtr();
    PouchPartyData* party;

    if (unit->currentKind == UNIT_MARIO) {
        unit->baseMaxHp = pouch->baseMaxHP;
        unit->maxHp = pouch->baseMaxHP + unit->badgesEquipped.HpPlus * 5;
        unit->baseMaxFp = pouch->baseMaxFP;
        unit->maxFp = pouch->baseMaxFP + unit->badgesEquipped.FpPlus * 5;
        if (unit->currentHp > unit->maxHp) {
            unit->currentHp = unit->maxHp;
        }
        if (unit->currentFp > unit->maxFp) {
            unit->currentFp = unit->maxFp;
        }
    } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        party = &pouch->partyData[BattleTransPartyId(unit->currentKind)];
        unit->baseMaxHp = party->baseMaxHP;
        unit->maxHp = party->baseMaxHP + unit->badgesEquipped.HpPlus * 5;
        unit->currentHp = party->currentHP;
        if (unit->currentHp > unit->maxHp) {
            unit->currentHp = unit->maxHp;
        }
    } else {
        unit->maxHp = unit->baseMaxHp + unit->badgesEquipped.HpPlus * 5;
        if (unit->currentHp > unit->maxHp) {
            unit->currentHp = unit->maxHp;
        }
        unit->maxFp = unit->baseMaxFp + unit->badgesEquipped.FpPlus * 5;
        if (unit->currentFp > unit->maxFp) {
            unit->currentFp = unit->maxFp;
        }
    }
}

/**
 * Writes Mario's HP and FP, or a partner's HP, back to the save data (pouch) and keeps a partner's size,
 * status flags and status turns (and, in US and PAL, disabled commands) in the battle's party info for
 * when it is switched back in.
 */
void BtlUnit_SetParamToPouch(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    BattleWorkPartyInfo* info;

    if (unit->currentKind == UNIT_MARIO) {
        pouch->currentHP = unit->currentHp;
        pouch->currentFP = unit->currentFp;
    } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        s32 id = BattleTransPartyId(unit->currentKind);
        *(s16*)((u8*)pouch + id * sizeof(PouchPartyData) + offsetof(PouchPartyData, currentHP)) = unit->currentHp; // FAKE: byte-offset store so the field offset folds into the index (mulli/addi/sthx), as in the target
        info = &wp->partyInfo[unit->currentKind - TYPE_PARTNER_MIN];
        info->sizeMultiplier = unit->sizeMultiplier;
        info->statusFlags = unit->statusFlags;
        info->statusTurns = unit->statusTurns;
#if VERSION >= VERSION_US
        info->usCommandDisabled[0] = unit->usCommandDisabled[0];
        info->usCommandDisabled[1] = unit->usCommandDisabled[1];
#endif
    }
}

/**
 * Updates the peril and danger status flags from the unit's HP against its kind's perilHP
 * and dangerHP thresholds (only below max HP).
 * When Mario first drops into peril or danger the audience reacts, unless noAudience is set.
 */
void BtlUnit_CheckPinchStatus(BattleUnit* unit, BOOL noAudience) {
    if (unit->currentHp <= unit->kindParams->perilHP && unit->currentHp < unit->maxHp) {
        if (!(unit->statusFlags & UNIT_STATUS_FLAG_PERIL)) {
            unit->statusFlags |= UNIT_STATUS_FLAG_PERIL;
            if (!noAudience && unit->currentKind == UNIT_MARIO) {
                BattleAudience_Case_MarioDanger();
            }
        }
    } else if (unit->statusFlags & UNIT_STATUS_FLAG_PERIL) {
        unit->statusFlags &= ~UNIT_STATUS_FLAG_PERIL;
    }
    if (unit->currentHp <= unit->kindParams->dangerHP && unit->currentHp < unit->maxHp) {
        if (!(unit->statusFlags & UNIT_STATUS_FLAG_DANGER)) {
            unit->statusFlags |= UNIT_STATUS_FLAG_DANGER;
            if (!(unit->statusFlags & UNIT_STATUS_FLAG_PERIL) && !noAudience && unit->currentKind == UNIT_MARIO) {
                BattleAudience_Case_MarioPinch();
            }
        }
    } else if (unit->statusFlags & UNIT_STATUS_FLAG_DANGER) {
        unit->statusFlags &= ~UNIT_STATUS_FLAG_DANGER;
    }
}

/**
 * Star Points an enemy gives when defeated: nothing if Mario's level is above the enemy's,
 * otherwise half the level difference scaled by the group size (x1 up to 2 enemies, x1.1 for 3, x1.3 for 4,
 * x1.5 for 5 or more) plus the kind's bonus. Read by the btlevtcmd_GetExp script helper.
 */
s32 BtlUnit_GetExp(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    s32 level = pouch->level;
    s32 unitLevel = unit->level;
    FieldBattleInfo* fbat = wp->fbatInfo;
    f32 scale;

    if (level > unitLevel) {
        return 0;
    }
    switch (fbat->setup->groupSetup->numEnemies) {
        case 0:
        case 1:
        case 2:
            scale = 1.0f;
            break;
        case 3:
            scale = 1.1f;
            break;
        case 4:
            scale = 1.3f;
            break;
        case 5:
        default:
            scale = 1.5f;
            break;
    }
    {
        f32 t = scale * (f32)(unitLevel - level);
        return (s32)(t / 2.0f) + unit->kindParams->bonusExp;
    }
}

/** Coins the enemy drops: its kind's base coins plus one per bonus roll that succeeds at bonusCoinRate percent. */
s32 BtlUnit_GetCoin(BattleUnit* unit) {
    s32 coins, i;

    if (!unit) {
        return 0;
    }
    coins = unit->kindParams->baseCoin;
    for (i = 0; i < unit->kindParams->bonusCoin; i++) {
        if (irand(100) < unit->kindParams->bonusCoinRate) {
            coins++;
        }
    }
    return coins;
}

/** The unit's current FP; partners use Mario's, since FP is shared. */
s32 BtlUnit_GetFp(BattleUnit* unit) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s32 result = unit->currentFp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        result = BattleGetMarioPtr(wp)->currentFp;
    }
    return result;
}

/** Sets the unit's current FP; for a partner it sets Mario's shared FP. */
void BtlUnit_SetFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        BattleGetMarioPtr(wp)->currentFp = value;
    } else {
        unit->currentFp = value;
    }
}

/** Adds HP to the unit, capped at its max HP (no lower bound, so a negative value can take HP below 0). */
void BtlUnit_RecoverHp(BattleUnit* unit, s32 value) {
    unit->currentHp += value;
    if (unit->currentHp > unit->maxHp) {
        unit->currentHp = unit->maxHp;
    }
}

/** Adds FP to the unit (Mario's shared FP for a partner), capped at the max. */
void BtlUnit_RecoverFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s16 max;
    s16* fp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        max = BtlUnit_GetMaxFp(BattleGetMarioPtr(wp));
        fp = &BattleGetMarioPtr(wp)->currentFp;
    } else {
        max = BtlUnit_GetMaxFp(unit);
        fp = &unit->currentFp;
    }
    *fp += value;
    if (*fp > max) {
        *fp = max;
    }
}

/** The unit's max FP; partners use Mario's. */
s32 BtlUnit_GetMaxFp(BattleUnit* unit) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s32 result = unit->maxFp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        result = BattleGetMarioPtr(wp)->maxFp;
    }
    return result;
}

/** Sets the unit's max FP; for a partner it sets Mario's. */
void BtlUnit_SetMaxFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        BattleGetMarioPtr(wp)->maxFp = value;
    } else {
        unit->maxFp = value;
    }
}

/**
 * FP cost of a move.
 * For moves granted by badges the cost scales with copies equipped:
 * doubling per extra copy for Power Jump, Multibounce, Power Bounce, Tornado Jump, Power Smash, Quake Hammer,
 * Hammer Throw, Fire Drive, Charge, the Mega moves and Super Charge;
 * times the count for the stomps, Head Rattle and Ice Smash. Each Flower Saver takes 1 off (not below 1).
 * Retail caps it at 999.
 */
s32 BtlUnit_GetWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    s32 cost = (u8)weapon->baseFpCost;
    s32 count;

    if (weapon->itemId) {
        count = pouchEquipCheckBadge(weapon->itemId);
        switch (weapon->itemId) {
            case ITEM_POWER_JUMP:
            case ITEM_MULTIBOUNCE:
            case ITEM_POWER_BOUNCE:
            case ITEM_TORNADO_JUMP:
            case ITEM_POWER_SMASH:
            case ITEM_QUAKE_HAMMER:
            case ITEM_HAMMER_THROW:
            case ITEM_FIRE_DRIVE:
            case ITEM_CHARGE:
            case ITEM_CHARGE_P:
            case ITEM_MEGA_JUMP:
            case ITEM_MEGA_SMASH:
            case ITEM_MEGA_QUAKE:
            case ITEM_SUPER_CHARGE:
            case ITEM_SUPER_CHARGE_P:
                cost = cost * pow(2.0, count - 1);
                break;
            case ITEM_SHRINK_STOMP:
            case ITEM_SLEEPY_STOMP:
            case ITEM_SOFT_STOMP:
            case ITEM_HEAD_RATTLE:
            case ITEM_ICE_SMASH:
                cost *= count;
                break;
        }
    }
    if (cost) {
        cost -= unit->badgesEquipped.FlowerSaver;
        if (cost <= 0) {
            cost = 1;
        }
    }
#if VERSION >= VERSION_JP
    if (cost >= 1000) {
        cost = 999;
    }
#endif
    return cost;
}

/** Whether the unit can afford a move: enough FP (shared for partners) and enough Star Power (baseSpCost x 100 AP). */
BOOL BtlUnit_CheckWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    s32 fp = BtlUnit_GetFp(unit);
    if (fp < BtlUnit_GetWeaponCost(unit, weapon)) {
        return FALSE;
    }
    return pouchGetAP() >= (u8)weapon->baseSpCost * 100;
}

/** Spends a move's FP and Star Power, adding the FP to Mario's or the partner's action record for the audience. */
void BtlUnit_PayWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    BattleWork* wp = _battleWorkPointer;
    s32 fp = BtlUnit_GetFp(unit);
    s32 cost = BtlUnit_GetWeaponCost(unit, weapon);

    BtlUnit_SetFp(unit, fp - cost);
    if (unit->currentKind == UNIT_MARIO) {
        BtlActRec_AddPoint(&wp->actRecord.marioFpSpent, cost);
        BtlActRec_AddCount(&wp->actRecord.marioFpMoves);
    } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        BtlActRec_AddPoint(&wp->actRecord.partnerFpSpent, cost);
        BtlActRec_AddCount(&wp->actRecord.partnerFpMoves);
    }
    pouchAddAP(-(u8)weapon->baseSpCost * 100);
}

/** Finds the Shell Shield (Koops's move) guarding this unit, the one whose work[0] holds its id; NULL if none. */
BattleUnit* BtlUnit_GetGuardKouraPtr(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 i = 0;
    s32 id = unit->unitId;
    BattleUnit* other;

    for (; i < BATTLE_UNIT_MAX; i++) {
        other = BattleGetUnitPtr(wp, i);
        if (other && other->currentKind == UNIT_SHELL_SHIELD && other->work[SHELL_SHIELD_WORK_TARGET] == id) {
            break;
        }
    }
    if (i >= 64) {
        other = NULL;
    }
    return other;
}

/**
 * Starts the unit's turn allowance: maxMoveCount moves, doubled by Fast and halved by Slow.
 * A one-move unit under Slow acts only on every other turn (move state 10 means it skips).
 */
void BtlUnit_ResetMoveStatus(BattleUnit* unit) {
    s32 n;

    unit->moveState = MOVE_STATE_NORMAL;
    unit->movesRemaining = unit->maxMoveCount;
    if (BtlUnit_CheckStatus(unit, STATUS_FAST)) {
        unit->movesRemaining *= 2;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
        if (unit->movesRemaining >= 2) {
            unit->movesRemaining /= 2;
        } else {
            n = 2;
            if (unit->maxMoveCount < 1) {
                n *= 2;
            }
            if (unit->activeTurns >= n) {
                unit->movesRemaining = 1;
                unit->moveState = MOVE_STATE_NORMAL;
            } else {
                unit->movesRemaining = 0;
                unit->moveState = MOVE_STATE_DONE;
            }
        }
    }
    unit->maxMovesThisTurn = unit->movesRemaining;
    if (!unit->maxMovesThisTurn) {
        unit->movesRemaining = 0;
        unit->moveState = MOVE_STATE_DONE;
    }
}

/**
 * Loads the sound set for a scripted move from the unit's move sound table:
 * with kind -1, mode 0, 1 or 3 picks entries 0-2 and mode 2 the jump sounds; kind -2 restarts the timer;
 * anything else silences it. Called by the btlevtcmd move helpers.
 */
void BtlUnit_LoadSeMode(s32 mode, s32 kind, BtlPoseSoundTable* table, BecSndWork* work) {
    work->count = 0;
    work->sfxId = -1;
    switch (kind) {
        case -1:
            switch (mode) {
                case 0:
                    work->name0 = table->entry[0].name0;
                    work->name1 = table->entry[0].name1;
                    work->delay = table->entry[0].delay;
                    work->period0 = table->entry[0].period0;
                    work->period1 = table->entry[0].period1;
                    break;
                case 1:
                    work->name0 = table->entry[1].name0;
                    work->name1 = table->entry[1].name1;
                    work->delay = table->entry[1].delay;
                    work->period0 = table->entry[1].period0;
                    work->period1 = table->entry[1].period1;
                    break;
                case 2:
                    work->name0 = table->jumpName0;
                    work->name1 = table->jumpName1;
                    work->delay = 0;
                    work->period0 = 0;
                    work->period1 = 0;
                    break;
                case 3:
                    work->name0 = table->entry[2].name0;
                    work->name1 = table->entry[2].name1;
                    work->delay = table->entry[2].delay;
                    work->period0 = table->entry[2].period0;
                    work->period1 = table->entry[2].period1;
                    break;
                default:
                    work->name0 = NULL;
                    work->name1 = NULL;
                    work->delay = 0;
                    work->period0 = 0;
                    work->period1 = 0;
                    break;
            }
            work->timer = work->delay;
            return;
        case -2:
            work->timer = work->delay;
            return;
        case 0:
            break;
    }
    work->name0 = NULL;
    work->name1 = NULL;
    work->delay = 0;
    work->period0 = 0;
    work->period1 = 0;
    work->timer = work->delay;
}

/** Sets a move sound set directly: two sound names, the start delay and the repeat period, plus two extra values. */
void BtlUnit_SetSeMode(BecSndWork* work, const char* name0, const char* name1, s16 delay, s16 period, s16 unused, s16 a, s16 b) {
    work->name0 = name0;
    work->name1 = name1;
    work->delay = delay;
    work->period0 = period;
    work->period1 = period;
    work->count = 0;
    work->reserved_08 = a;
    work->reserved_0A = b;
    work->sfxId = -1;
}

/**
 * Puts the unit in its waiting-at-the-command-menu pose, unless it is in danger,
 * in peril or has a status pose to show, in which case the normal status pose is kept.
 */
void BtlUnit_SetCommandAnimPose(BattleUnit* unit) {
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    BOOL special = FALSE;

    if (!(unit->statusFlags & (UNIT_STATUS_FLAG_DANGER | UNIT_STATUS_FLAG_PERIL)) && _GetStatusPoseType(unit) == POSE_TYPE_NORMAL) {
        special = TRUE;
    }
    if (special) {
        BtlUnit_SetAnimType(part, POSE_TYPE_COMMAND);
        part->animFlags |= PART_ANIM_COMMAND_POSE;
    } else {
        btlDispPoseAnime(part);
    }
}

/** Resets a part's animation-synced sound state (loop count and timeline). */
void BtlUnit_PoseSoundInit(BattleUnitPart* part) {
    part->poseSound.loopCount = 0;
    part->poseSound.time = 0.0f;
    part->poseSound.prevTime = 0.0f;
}

#pragma push
// propagation optimisation off for BtlUnit_ControlPoseSoundMain, BtlUnit_snd_se, BtlUnit_snd_se_pos and 4 more: the original register allocation and scheduling need it
#pragma opt_propagation off

/**
 * Plays the sound effects tied to a part's current animation:
 * each frame, entries in the part's pose sound table whose time (in seconds of the animation) was just passed
 * fire their sound or callback, restarting on each loop.
 * Entries with flag 1 play at 70% volume when the unit is outside the current phase.
 */
void BtlUnit_ControlPoseSoundMain(BattleUnitPart* part) {
    BattleWork* wp = _battleWorkPointer;
    BtlPartPoseSound* state;
    AnimTableEntry* anim;
    BtlPoseSoundData* entry;
    AnimationPose* pose;
    f32 time;
    s32 id;
    u8 vol;
    f64 dv;

    if (!part) {
        return;
    }
    state = &part->poseSound;
    entry = state->table;
    if (!entry) {
        return;
    }
    if (part->poseId == -1) {
        return;
    }
    pose = animPoseGetAnimPosePtr(part->poseId);
    if (!pose) {
        return;
    }
    time = pose->localFrame / 60.0f;
    if (state->loopCount > (u32)pose->loopTime) {
        state->time = time;
        state->prevTime = -1.0f;
        state->loopCount = (s32)pose->loopTime;
        state->count = 0;
        if (BattleGetSeq(wp, SEQ_PHASE) == BATTLE_PHASE_PLAYER || BattleGetSeq(wp, SEQ_PHASE) == BATTLE_PHASE_ENEMY) {
            state->flag = 1;
        }
    } else if (state->loopCount == (u32)pose->loopTime && state->time > time) {
        state->time = time;
        state->prevTime = -1.0f;
    } else if (state->loopCount < (u32)pose->loopTime) {
        state->time = time;
        state->prevTime = -1.0f;
        state->loopCount = (s32)pose->loopTime;
    } else {
        state->prevTime = state->time;
        state->time = time;
    }
    if (state->time == state->prevTime) {
        return;
    }
    anim = animPoseGetCurrentAnim(part->poseId);
    for (; entry->animName; entry++) {
        if (strcmp(entry->animName, anim->name)) {
            continue;
        }
        if (entry->time <= state->time && entry->time > state->prevTime) {
            if (entry->flags & 1) {
                if (entry->callback) {
                    id = entry->callback(part->owner, entry, state);
                } else {
                    id = BtlUnit_snd_se(part->owner, entry->sfxName, EVT_NULLPTR, entry->pitch);
                }
                vol = 0x7F;
                psndSFX_vol(id, vol);
                state->count++;
                if (part->owner->attackPhase != BattleGetSeq(wp, SEQ_PHASE)) {
                    f64 k; // FAKE: spare temporary assigned inside the multiplication; vol = dv * 0.7 changes the object
                    dv = (f64)(u32)vol;
                    vol = dv * (k = 0.7);
                    psndSFX_vol(id, vol);
                }
            } else {
                if (entry->callback) {
                    entry->callback(part->owner, entry, state);
                } else {
                    BtlUnit_snd_se(part->owner, entry->sfxName, EVT_NULLPTR, entry->pitch);
                }
                state->count++;
            }
        }
    }
}
#pragma pop

/** Plays a sound effect at the unit's position (BtlUnit_snd_se_pos). */
s32 BtlUnit_snd_se(BattleUnit* unit, const char* sfxName, s32 volume, s32 pitch) {
    Vec pos;
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    return ((s32 (*)(BattleUnit*, const char*, s32, s32, Vec*))BtlUnit_snd_se_pos)(unit, sfxName, volume, pitch, &pos);
}

/**
 * Plays a 3D sound effect at a position with the given pitch plus a random -20..20 shift;
 * Tiny raises and Huge lowers the pitch by the size change's strength.
 * volume -250000000 keeps the sound's default volume. Returns the sound id.
 */
s32 BtlUnit_snd_se_pos(BattleUnit* unit, const char* sfxName, s32 volume, s16 pitch, Vec* pos) {
    s32 id;
    s8 turns, strength;
    s16 pit = 0;
    s16 p;

    if (BtlUnit_CheckStatus(unit, STATUS_TINY)) {
        BtlUnit_GetStatus(unit, STATUS_TINY, &turns, &strength);
        pit = (strength * 2 - 1) * -0x155;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_HUGE)) {
        BtlUnit_GetStatus(unit, STATUS_HUGE, &turns, &strength);
        pit = (strength * 2 + 1) * -0x155;
    }

    p = pitch;
    p += pit;
    p += irand(41) - 20;
    id = psndSFXOn_3D(sfxName, pos);
    psndSFX_pit(id, p);
    psndSFX_vol(id, volume == EVT_NULLPTR ? psndSFX_get_vol(id) : (u8)volume);
    return id;
}

/**
 * Resets the unit's HP gauge to its current HP with no drain animation pending (20-frame drain, shown 30 frames
 * more).
 */
void BtlUnit_HpGaugeInit(BattleUnit* unit) {
    BattleWorkHpGauge* gauge = &unit->hpGauge;

    gauge->drainTimer = 0;
    gauge->visibleTimer = 0;
    gauge->mode = 0;
    gauge->prevHp = unit->currentHp;
    gauge->targetHp = unit->currentHp;
    gauge->trailRatio = gauge->barRatio = (f32)unit->currentHp / (f32)unit->maxHp;
    switch (gauge->mode) {
        case 1:
            gauge->drainLength = 40;
            gauge->visibleLength = gauge->drainLength + 30;
            break;
        default:
            gauge->drainLength = 20;
            gauge->visibleLength = gauge->drainLength + 30;
            break;
    }
}

/**
 * Per-frame update of the HP gauge shown over an enemy:
 * animates the bar draining from the old HP to the new one
 * and hides the gauge again once its display time runs out.
 */
void BtlUnit_HpGaugeMain(BattleUnit* unit) {
    BattleWorkHpGauge* gauge = &unit->hpGauge;
    s16 maxHp;

    if (gauge->drainTimer > 0) {
        gauge->drainTimer--;
        if (gauge->drainTimer <= 0) {
            gauge->prevHp = gauge->targetHp;
            gauge->barRatio = gauge->trailRatio = (f32)unit->currentHp / (f32)unit->maxHp;
        } else {
            switch (gauge->mode) {
                case 1:
                    gauge->barRatio = (f32)unit->currentHp / (f32)unit->maxHp;
                    gauge->trailRatio = gauge->barRatio + (f32)(gauge->prevHp - gauge->targetHp) * intplGetValue(EASING_CUBIC_IN, 0.0f, 1.0f, gauge->drainTimer, gauge->drainLength) / (f32)unit->maxHp;
                    break;
                default:
                    gauge->trailRatio = 0.0f;
                    maxHp = unit->maxHp;
                    gauge->barRatio = (f32)(gauge->prevHp - gauge->targetHp) * intplGetValue(EASING_LINEAR, 0.0f, 1.0f, gauge->drainTimer, gauge->drainLength) / (f32)maxHp + (f32)unit->currentHp / (f32)maxHp;
                    break;
            }
        }
    }
    if (gauge->visibleTimer > 0) {
        gauge->visibleTimer--;
    }
    if ((unit->flags & BATTLE_UNIT_FLAG_SHOW_HP) && gauge->visibleTimer <= 0 && (gauge->flags & 1)) {
        unit->flags &= ~BATTLE_UNIT_FLAG_SHOW_HP;
        gauge->flags &= ~1;
    }
}

/**
 * Whether an enemy can use (and so may be given) this item: any badge, or an item in _enemy_can_use_item_table.
 * Returns the item id when it can, else 0.
 */
s32 BtlUnit_EnemyItemCanUseCheck(s32 item) {
    s32* table = _enemy_can_use_item_table;

    if (item >= BADGE_MIN && item < BADGE_MAX) {
        return item;
    }
    while (*table) {
        if (*table == item) {
            return item;
        }
        table++;
    }
    return FALSE;
}

BOOL BtlUnit_CheckShadowGuard(BattleUnit* unit) {
    if (unit->attributeFlags & kVeiled) {
        return TRUE;
    }
    if (unit->currentKind == UNIT_VIVIAN && unit->work[VIVIAN_WORK_HIDE_TURNS] > 0) {
        return TRUE;
    }
    return FALSE;
}
