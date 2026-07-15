#include "battle/battle_unit.h"

#include "battle/battle.h"
#include "battle/battle_disp.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_status_icon.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "system.h"

extern f32 dat_804226c4;

//.data
s32 _enemy_can_use_item_table[33] = {
    ITEM_THUNDER_BOLT,  ITEM_FIRE_FLOWER,  ITEM_POW_BLOCK,     ITEM_THUNDER_RAGE, ITEM_SHOOTING_STAR,
    ITEM_ICE_STORM,     ITEM_EARTH_QUAKE,  ITEM_HP_DRAIN_ITEM, ITEM_SLEEPY_SHEEP, ITEM_STOPWATCH,
    ITEM_DIZZY_DIAL,    ITEM_MINI_MR_MINI, ITEM_MR_SOFTENER,   ITEM_RUIN_POWDER,  ITEM_POWER_PUNCH,
    ITEM_COURAGE_SHELL, ITEM_BOOS_SHEET,   ITEM_VOLT_SHROOM,   ITEM_REPEL_CAPE,   ITEM_MUSHROOM,
    ITEM_SUPER_SHROOM,  ITEM_ULTRA_SHROOM, ITEM_LIFE_SHROOM,   ITEM_DRIED_SHROOM, ITEM_TASTY_TONIC,
    ITEM_HONEY_SYRUP,   ITEM_MAPLE_SYRUP,  ITEM_JAMMIN_JELLY,  ITEM_SLOW_SHROOM,  ITEM_GRADUAL_SYRUP,
    ITEM_HOT_DOG,       ITEM_CAKE,         ITEM_NULL};

BOOL BtlUnit_Init(void) {
    int i;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        BattleSetUnitPtr(wp, i, NULL);
    }

    return TRUE;
}

BattleWorkUnit* BtlUnit_Entry(BattleUnitSetup* setup) {
    extern void BtlUnit_SetOffsetPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetHomePos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetBaseRotate(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetRotate(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetRotateOffset(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetScale(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetBaseScale(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_HpGaugeInit(BattleWorkUnit*);
    BattleWork* wp = _battleWorkPointer;
    BattleWorkUnit* unit;
    BattleUnitType type;
    int i;

    for (i = 0; i < 64; i++) {
        if (BattleGetUnitPtr(wp, i) == NULL) {
            break;
        }
    }
    if (i >= 64) {
        return NULL;
    }
    unit = BattleAlloc(sizeof(BattleWorkUnit));
    if (unit == NULL) {
        return NULL;
    }
    memset(unit, 0, sizeof(BattleWorkUnit));
    BattleSetUnitPtr(wp, i, unit);
    unit->unitId = i;
    unit->data = setup->data;
    unit->dataTable = unit->data->dataTable;
    unit->initialType = setup->data->type;
    unit->currentType = setup->data->type;
    BattleStatusEffectInit(unit);
    BattleStatusIconInit(unit);
    BattleStatusChangeInfoWorkInit(unit);
    unit->groupId = -1;
    unit->alliance = setup->alliance;
    unit->level = unit->data->level;
    type = unit->currentType;
    if (type == UNIT_MARIO || (type >= UNIT_PARTNER_MIN && type < UNIT_PARTNER_MAX)) {
        unit->level = pouchGetPtr()->level;
    }
    unit->attackPhase = setup->attackPhase;

    *(void**)((s32)unit + 0x280) = NULL;
    *(void**)((s32)unit + 0x284) = NULL;
    *(s32*)((s32)unit + 0x288) = 0;
    *(void**)((s32)unit + 0x28C) = NULL;
    *(s32*)((s32)unit + 0x290) = 0;
    *(void**)((s32)unit + 0x294) = NULL;
    *(s32*)((s32)unit + 0x298) = 0;

    BtlUnit_SetPos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetOffsetPos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetHomePos(unit, setup->position.x, setup->position.y, setup->position.z);
    BtlUnit_SetBaseRotate(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetRotate(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetRotateOffset(unit, 0.0f, 0.0f, 0.0f);
    BtlUnit_SetScale(unit, 1.0f, 1.0f, 1.0f);
    BtlUnit_SetBaseScale(unit, 1.0f, 1.0f, 1.0f);

    unit->baseMaxHP = unit->data->maxHP;
    unit->currentMaxHP = unit->baseMaxHP;
    unit->currentHP = unit->data->maxHP;
    unit->baseMaxFP = unit->data->maxFP;
    unit->currentMaxFP = unit->baseMaxFP;
    unit->currentFP = unit->data->maxFP;
    unit->sizeMultiplier = 1.0f;
    unit->tokenFlags = 0;
    unit->statusFlags = 0;
    *(u32*)((s32)unit + 0x140) = (u32)-1;
    BtlUnit_HpGaugeInit(unit);
    BtlUnit_ClearStatus(unit);
    unit->maxMoveCount = 1;
    unit->movesRemaining = 1;
    unit->faceDirection = wp->allianceInfo[unit->alliance].attackDirection;
    unit->work[0] = *(s32*)((s32)setup + 0x18);
    unit->work[1] = *(s32*)((s32)setup + 0x1C);
    unit->work[2] = *(s32*)((s32)setup + 0x20);
    unit->work[3] = *(s32*)((s32)setup + 0x24);
    return unit;
}

BattleWorkUnit* BtlUnit_Spawn(BattleUnitSetup* setup, s32 type) {
    BattleWork* wp = _battleWorkPointer;
    BattleWorkUnit* unit;
    s32 temp_r0;
    s32 unitId;
    s32 index;
    int i;

    unit = BtlUnit_Entry(setup);
    if (unit == NULL) {
        return unit;
    }

    if (type & 2) {
        unit->moveState = 0;
        unitId = unit->unitId;
        for (i = 0; i < 64; i++) {
            if (unitId == -1) {
                break;
            }

            if (wp->movePriorityQueue[i] == -1) {
                wp->movePriorityQueue[i] = unitId;
                break;
            }
            else {
                temp_r0 = wp->movePriorityQueue[i];
                wp->movePriorityQueue[i] = unitId;
            }

            unitId = temp_r0;
        }
    }
    else if (type & 1) {
        unit->moveState = 0;
        unitId = unit->unitId;
        for (i = 0, index = 63; i < 64; i++, index--) {
            if (unitId == -1) {
                break;
            }

            if (wp->movePriorityQueue[index] == -1) {
                wp->movePriorityQueue[index] = unitId;
                break;
            }
            else {
                temp_r0 = wp->movePriorityQueue[index];
                wp->movePriorityQueue[index] = unitId;
            }

            unitId = temp_r0;
        }
    }
    return unit;
}

s32 BtlUnit_GetUnitId(BattleWorkUnit* unit) {
    return unit->unitId;
}

BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 index) {
    BattleWorkUnitPart* part = unit->parts;

    if (index < 0) {
        return part;
    }

    if (!part->nextPart) {
        return part;
    }

    while (part) {
        if (part->data->partId == index) {
            return part;
        }
        part = part->nextPart;
    }

    return part;
}

s32 BtlUnit_GetBodyPartsId(BattleWorkUnit* unit) {
    BattleWorkUnitPart* part;
    s32 partId = -1;

    if (unit) {
        for (part = unit->parts; part; part = part->nextPart) {
            if (part->attributes & 1) {
                partId = part->data->partId;
                break;
            }
        }
    }

    return partId;
}

void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->position.x;
    *y = unit->position.y;
    *z = unit->position.z;
}

void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->position.x = x;
    unit->position.y = y;
    unit->position.z = z;
}

void BtlUnit_AddPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->position.x += x;
    unit->position.y += y;
    unit->position.z += z;
}

void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->position.x;
    *y = part->position.y;
    *z = part->position.z;
}

void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->position.x = x;
    part->position.y = y;
    part->position.z = z;
}

void BtlUnit_AddPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->position.x += x;
    part->position.y += y;
    part->position.z += z;
}

void BtlUnit_GetPartsWorldPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    BattleWorkUnit* unit = part->owner;
    f32 unit_x, part_x;
    f32 unit_y, part_y;
    f32 unit_z, part_z;

    if (part->attributes & 0x10000000) {
        *x = part->position.x;
        *y = part->position.y;
        *z = part->position.z;
    }
    else {
        BtlUnit_GetPos(part->owner, &unit_x, &unit_y, &unit_z);
        BtlUnit_GetPartsOffsetPos(part, &part_x, &part_y, &part_z);
        *x = part_x * (f32)unit->faceDirection + unit_x;
        *y = unit_y + part_y;
        *z = unit_z + part_z;
    }
}

void BtlUnit_GetHitPos(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    s32 x_direction;
    f32 y_direction = 1.0f;

    x_direction = _battleWorkPointer->allianceInfo[unit->alliance].attackDirection;

    BtlUnit_GetPartsWorldPos(part, x, y, z);

    // Calculate base offset
    *x = (f32)x_direction * (part->hitBaseOffset.x * unit->sizeMultiplier) + *x;
    *y = (y_direction * (part->hitBaseOffset.y * unit->sizeMultiplier)) + *y;
    *z = (part->hitBaseOffset.z * unit->sizeMultiplier) + *z;

    // Calculate full offset
    *x = (f32)x_direction * (part->hitOffset.x * unit->sizeMultiplier) + *x;
    *y = (y_direction * (part->hitOffset.y * unit->sizeMultiplier)) + *y;
    *z = (part->hitOffset.z * unit->sizeMultiplier) + *z;
}

void BtlUnit_SetHitOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->hitOffset.x = x;
    part->hitOffset.y = y;
    part->hitOffset.z = z;
}

void BtlUnit_SetHitCursorOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->hitCursorOffset.x = x;
    part->hitCursorOffset.y = y;
    part->hitCursorOffset.z = z;
}

void BtlUnit_GetHomePos(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->homePosition.x;
    *y = unit->homePosition.y;
    *z = unit->homePosition.z;
}

void BtlUnit_SetHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->homePosition.x = x;
    unit->homePosition.y = y;
    unit->homePosition.z = z;
}

void BtlUnit_AddHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->homePosition.x += x;
    unit->homePosition.y += y;
    unit->homePosition.z += z;
}

void BtlUnit_SetPartsHomePos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->homePosition.x = x;
    part->homePosition.y = y;
    part->homePosition.z = z;
}

void BtlUnit_SetRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    f32 rot_x, rot_y, rot_z;

    rot_x = reviseAngle(x);
    if (rot_x >= 360.0f) {
        rot_x = 0.0f;
    }
    unit->rotation.x = rot_x;

    rot_y = reviseAngle(y);
    if (rot_y >= 360.0f) {
        rot_y = 0.0f;
    }
    unit->rotation.y = rot_y;

    rot_z = reviseAngle(z);
    if (rot_z >= 360.0f) {
        rot_z = 0.0f;
    }
    unit->rotation.z = rot_z;
}

void BtlUnit_GetRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->rotation.x;
    *y = unit->rotation.y;
    *z = unit->rotation.z;
}

void BtlUnit_AddRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
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

void BtlUnit_SetPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotation.x = x;
    part->rotation.y = y;
    part->rotation.z = z;
}

void BtlUnit_GetPartsRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->rotation.x;
    *y = part->rotation.y;
    *z = part->rotation.z;
}

void BtlUnit_AddPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotation.x += x;
    if (part->rotation.x > 360.0f) {
        part->rotation.x -= 360.0f;
    }
    else if (part->rotation.x < 0.0f) {
        part->rotation.x += 360.0f;
    }

    part->rotation.y += y;
    if (part->rotation.y > 360.0f) {
        part->rotation.y -= 360.0f;
    }
    else if (part->rotation.y < 0.0f) {
        part->rotation.y += 360.0f;
    }

    part->rotation.z += z;
    if (part->rotation.z > 360.0f) {
        part->rotation.z -= 360.0f;
    }
    else if (part->rotation.z < 0.0f) {
        part->rotation.z += 360.0f;
    }
}

void BtlUnit_SetBaseRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->baseRotation.x = reviseAngle(x);
    unit->baseRotation.y = reviseAngle(y);
    unit->baseRotation.z = reviseAngle(z);
}

void BtlUnit_GetBaseRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->baseRotation.x;
    *y = unit->baseRotation.y;
    *z = unit->baseRotation.z;
}

void BtlUnit_SetPartsBaseRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->baseRotation.x = x;
    part->baseRotation.y = y;
    part->baseRotation.z = z;
}

void BtlUnit_GetPartsBaseRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->baseRotation.x;
    *y = part->baseRotation.y;
    *z = part->baseRotation.z;
}

void BtlUnit_SetRotateOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->rotationOffset.x = x;
    }

    if (dat_804226c4 != y) {
        unit->rotationOffset.y = y;
    }

    if (dat_804226c4 != z) {
        unit->rotationOffset.z = z;
    }
}

void BtlUnit_SetPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->rotationOffset.x = x;
    }
    if (dat_804226c4 != y) {
        part->rotationOffset.y = y;
    }
    if (dat_804226c4 != z) {
        part->rotationOffset.z = z;
    }
}

void BtlUnit_AddPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->rotationOffset.x += x;
    part->rotationOffset.y += y;
    part->rotationOffset.z += z;
}

void BtlUnit_SetBaseScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->baseScale.x = x;
    }
    if (dat_804226c4 != y) {
        unit->baseScale.y = y;
    }
    if (dat_804226c4 != z) {
        unit->baseScale.z = z;
    }
}

void BtlUnit_GetScale(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->scale.x;
    *y = unit->scale.y;
    *z = unit->scale.z;
}

void BtlUnit_SetScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->scale.x = x;
    }
    if (dat_804226c4 != y) {
        unit->scale.y = y;
    }
    if (dat_804226c4 != z) {
        unit->scale.z = z;
    }
}

void BtlUnit_AddScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->scale.x += x;
    }
    if (dat_804226c4 != y) {
        unit->scale.y += y;
    }
    if (dat_804226c4 != z) {
        unit->scale.z += z;
    }
}

void BtlUnit_SetPartsBaseScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->baseScale.x = x;
    }
    if (dat_804226c4 != y) {
        part->baseScale.y = y;
    }
    if (dat_804226c4 != z) {
        part->baseScale.z = z;
    }
}

void BtlUnit_SetPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->scale.x = x;
    }
    if (dat_804226c4 != y) {
        part->scale.y = y;
    }
    if (dat_804226c4 != z) {
        part->scale.z = z;
    }
}

void BtlUnit_AddPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->scale.x += x;
    }
    if (dat_804226c4 != y) {
        part->scale.y += y;
    }
    if (dat_804226c4 != z) {
        part->scale.z += z;
    }
}

s32 BtlUnit_GetWidth(BattleWorkUnit* unit) {
    return unit->width;
}

s32 BtlUnit_GetHeight(BattleWorkUnit* unit) {
    return unit->height;
}

void BtlUnit_SetHeight(BattleWorkUnit* unit, s32 height) {
    unit->height = height;
}

void BtlUnit_SetOffsetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->positionOffset.x = x;
    }
    if (dat_804226c4 != y) {
        unit->positionOffset.y = y;
    }
    if (dat_804226c4 != z) {
        unit->positionOffset.z = z;
    }
}

void BtlUnit_GetPartsOffsetPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->positionOffset.x;
    *y = part->positionOffset.y;
    *z = part->positionOffset.z;
}

void BtlUnit_SetPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->positionOffset.x = x;
    }
    if (dat_804226c4 != y) {
        part->positionOffset.y = y;
    }
    if (dat_804226c4 != z) {
        part->positionOffset.z = z;
    }
}

void BtlUnit_AddPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->positionOffset.x += x;
    part->positionOffset.y += y;
    part->positionOffset.z += z;
}

void BtlUnit_SetDispOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->displayOffset.x = x;
    }
    if (dat_804226c4 != y) {
        unit->displayOffset.y = y;
    }
    if (dat_804226c4 != z) {
        unit->displayOffset.z = z;
    }
}

void BtlUnit_SetPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->displayOffset.x = x;
    }
    if (dat_804226c4 != y) {
        part->displayOffset.y = y;
    }
    if (dat_804226c4 != z) {
        part->displayOffset.z = z;
    }
}

void BtlUnit_AddPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->displayOffset.x += x;
    }
    if (dat_804226c4 != y) {
        part->displayOffset.y += y;
    }
    if (dat_804226c4 != z) {
        part->displayOffset.z += z;
    }
}

void BtlUnit_SetMoveStartPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->moveStartPosition.x = x;
    }
    if (dat_804226c4 != y) {
        unit->moveStartPosition.y = y;
    }
    if (dat_804226c4 != z) {
        unit->moveStartPosition.z = z;
    }
}

void BtlUnit_SetPartsMoveStartPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->moveStartPosition.x = x;
    }
    if (dat_804226c4 != y) {
        part->moveStartPosition.y = y;
    }
    if (dat_804226c4 != z) {
        part->moveStartPosition.z = z;
    }
}

void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->moveCurrentPosition.x = x;
    }
    if (dat_804226c4 != y) {
        unit->moveCurrentPosition.y = y;
    }
    if (dat_804226c4 != z) {
        unit->moveCurrentPosition.z = z;
    }
}

void BtlUnit_SetPartsMoveCurrentPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->moveCurrentPosition.x = x;
    }
    if (dat_804226c4 != y) {
        part->moveCurrentPosition.y = y;
    }
    if (dat_804226c4 != z) {
        part->moveCurrentPosition.z = z;
    }
}

void BtlUnit_SetMoveTargetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        unit->moveTargetPosition.x = x;
    }
    if (dat_804226c4 != y) {
        unit->moveTargetPosition.y = y;
    }
    if (dat_804226c4 != z) {
        unit->moveTargetPosition.z = z;
    }
}

void BtlUnit_SetPartsMoveTargetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    if (dat_804226c4 != x) {
        part->moveTargetPosition.x = x;
    }
    if (dat_804226c4 != y) {
        part->moveTargetPosition.y = y;
    }
    if (dat_804226c4 != z) {
        part->moveTargetPosition.z = z;
    }
}

void BtlUnit_SetFallAccel(BattleWorkUnit* unit, f32 acceleration) {
    unit->fallAcceleration = acceleration;
}

void BtlUnit_SetPartsFallAccel(BattleWorkUnitPart* part, f32 acceleration) {
    part->fallAcceleration = acceleration;
}

void BtlUnit_SetMoveSpeed(BattleWorkUnit* unit, f32 speed) {
    unit->moveSpeedXZ = speed;
}

void BtlUnit_SetPartsMoveSpeed(BattleWorkUnitPart* part, f32 speed) {
    part->moveSpeedXZ = speed;
}

void BtlUnit_SetJumpSpeed(BattleWorkUnit* unit, f32 speed) {
    unit->moveSpeedY = speed;
}

s8 BtlUnit_GetBelong(BattleWorkUnit* unit) {
    return unit->alliance;
}

void _CheckMoveCount(BattleWorkUnit* unit) {
    if ((unit->tokenFlags & 0x1000) == 0) {
        if (unit->maxMovesThisTurn > 0 && unit->maxMovesThisTurn == unit->movesRemaining) {
            BtlUnit_ResetMoveStatus(unit);
        }
    }
}

void BtlUnit_GetStatus(BattleWorkUnit* unit, StatusEffectType type, s8* turns, s8* strength) {
    BattleWorkStatus* status = &unit->statusEffects;
    s8 thisTurns = 0;
    s8 thisStrength = 0;

    switch ((s8)type) {
    case STATUS_SLEEP:
        thisStrength = status->sleepTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_STOPPED:
        thisStrength = status->stoppedTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_DIZZY:
        thisStrength = status->dizzyTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_POISON:
        thisTurns = status->poisonTurns;
        thisStrength = status->poisonStrength;
        break;

    case STATUS_CONFUSED:
        thisStrength = status->confusedTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_ELECTRIC:
        thisStrength = status->electricTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_DODGY:
        thisStrength = status->dodgyTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_BURN:
        thisStrength = status->burnTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_FROZEN:
        thisStrength = status->frozenTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_HUGE:
        if (status->sizeChangeStrength > 0) {
            thisTurns = status->sizeChangeTurns;
            thisStrength = status->sizeChangeStrength;
        }
        break;

    case STATUS_TINY:
        if (status->sizeChangeStrength < 0) {
            thisTurns = status->sizeChangeTurns;
            thisStrength = status->sizeChangeStrength;
        }
        break;

    case STATUS_ATTACK_UP:
        if (status->attackChangeStrength > 0) {
            thisTurns = status->attackChangeTurns;
            thisStrength = status->attackChangeStrength;
        }
        break;

    case STATUS_ATTACK_DOWN:
        if (status->attackChangeStrength < 0) {
            thisTurns = status->attackChangeTurns;
            thisStrength = status->attackChangeStrength;
        }
        break;

    case STATUS_DEFENSE_UP:
        if (status->defenseChangeStrength > 0) {
            thisTurns = status->defenseChangeTurns;
            thisStrength = status->defenseChangeStrength;
        }
        break;

    case STATUS_DEFENSE_DOWN:
        if (status->defenseChangeStrength < 0) {
            thisTurns = status->defenseChangeTurns;
            thisStrength = status->defenseChangeStrength;
        }
        break;

    case STATUS_FAST:
        thisStrength = status->fastTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_SLOW:
        thisStrength = status->slowTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_CHARGED:
        thisStrength = status->chargedStrength;
        thisTurns = thisStrength;
        break;

    case STATUS_ALLERGIC:
        thisStrength = status->allergicTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_FLIPPED:
        thisStrength = status->flippedTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_INVISIBLE:
        thisStrength = status->invisibleTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_PAYBACK:
        thisStrength = status->paybackTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_HOLD_FAST:
        thisStrength = status->holdFastTurns;
        thisTurns = thisStrength;
        break;

    case STATUS_HP_REGEN:
        thisTurns = status->hpRegenTurns;
        thisStrength = status->hpRegenStrength;
        break;

    case STATUS_FP_REGEN:
        thisTurns = status->fpRegenTurns;
        thisStrength = status->fpRegenStrength;
        break;

    case STATUS_INSTAKILL:
        thisStrength = status->instakillStrength;
        thisTurns = thisStrength;
        break;
    }

    if (turns) {
        *turns = thisTurns;
    }

    if (strength) {
        *strength = thisStrength;
    }
}

BOOL BtlUnit_SetStatus(BattleWorkUnit* unit, StatusEffectType type, s8 turns, s8 strength) {
    BattleWorkStatus* status = &unit->statusEffects;
    s8 charged;

    switch ((s8)type) {
    case STATUS_SLEEP:
        status->sleepTurns = turns;
        status->sleepStrength = 0;
        break;

    case STATUS_STOPPED:
        status->stoppedTurns = turns;
        break;

    case STATUS_DIZZY:
        status->dizzyTurns = turns;
        break;

    case STATUS_POISON:
        status->poisonTurns = turns;
        status->poisonStrength = strength;
        break;

    case STATUS_CONFUSED:
        status->confusedTurns = turns;
        break;

    case STATUS_ELECTRIC:
        status->electricTurns = turns;
        break;

    case STATUS_DODGY:
        status->dodgyTurns = turns;
        break;

    case STATUS_BURN:
        if (status->frozenTurns != 0) {
            status->frozenTurns = 0;
            return FALSE;
        }
        status->burnTurns = turns;
        break;

    case STATUS_FROZEN:
        if (status->burnTurns != 0) {
            status->burnTurns = 0;
            return FALSE;
        }
        status->frozenTurns = turns;
        break;

    case STATUS_HUGE:
    case STATUS_TINY:
        if (strength == 0) {
            if ((s8)type == STATUS_HUGE) {
                if (status->sizeChangeStrength >= 0) {
                    status->sizeChangeTurns = 0;
                    status->sizeChangeStrength = 0;
                    return FALSE;
                }
            }
            else if (status->sizeChangeStrength <= 0) {
                status->sizeChangeTurns = 0;
                status->sizeChangeStrength = 0;
                return FALSE;
            }
            return FALSE;
        }
        status->sizeChangeTurns = turns;
        status->sizeChangeStrength = strength;
        break;

    case STATUS_ATTACK_UP:
        if (strength == 0) {
            if (status->attackChangeStrength >= 0) {
                status->attackChangeTurns = 0;
                status->attackChangeStrength = 0;
            }
            return FALSE;
        }
        status->attackChangeTurns = turns;
        status->attackChangeStrength = strength;
        break;

    case STATUS_ATTACK_DOWN:
        if (strength == 0) {
            if (status->attackChangeStrength <= 0) {
                status->attackChangeTurns = 0;
                status->attackChangeStrength = 0;
            }
            return FALSE;
        }
        status->attackChangeTurns = turns;
        status->attackChangeStrength = strength;
        break;

    case STATUS_DEFENSE_UP:
    case STATUS_DEFENSE_DOWN:
        if (strength > 0) {
            if (status->defenseChangeStrength < 0) {
                status->defenseChangeTurns = 0;
                status->defenseChangeStrength = 0;
                return FALSE;
            }
            status->defenseChangeTurns = turns;
            status->defenseChangeStrength = strength;
        }
        else {
            if (strength == 0) {
                if ((s8)type == STATUS_DEFENSE_DOWN) {
                    if (status->defenseChangeStrength <= 0) {
                        status->defenseChangeTurns = 0;
                        status->defenseChangeStrength = 0;
                    }
                }
                else if (status->defenseChangeStrength >= 0) {
                    status->defenseChangeTurns = 0;
                    status->defenseChangeStrength = 0;
                }
                return FALSE;
            }
            if (status->defenseChangeStrength > 0) {
                status->defenseChangeTurns = 0;
                status->defenseChangeStrength = 0;
                return FALSE;
            }
            status->defenseChangeTurns = turns;
            status->defenseChangeStrength = strength;
        }
        break;

    case STATUS_CHARGED:
        status->chargedStrength += strength;
        charged = status->chargedStrength;
        if (charged < 0) {
            status->chargedStrength = 0;
            return FALSE;
        }
        if (charged > 99) {
            status->chargedStrength = 99;
        }
        break;

    case STATUS_ALLERGIC:
        if (turns > 0) {
            status->allergicTurns = turns;
            break;
        }
        status->allergicTurns = 0;
        return FALSE;

    case STATUS_FLIPPED:
        status->flippedTurns = turns;
        break;

    case STATUS_INVISIBLE:
        status->invisibleTurns = turns;
        break;

    case STATUS_PAYBACK:
        if (turns > 0) {
            status->paybackTurns = turns;
            break;
        }
        status->paybackTurns = 0;
        return FALSE;

    case STATUS_HOLD_FAST:
        status->holdFastTurns = turns;
        break;

    case STATUS_FAST:
        if ((s8)status->slowTurns > 0) {
            if (turns > 0) {
                status->slowTurns = 0;
                _CheckMoveCount(unit);
                return FALSE;
            }
            else {
                status->fastTurns = turns;
            }
        }
        else {
            status->fastTurns = turns;
        }
        _CheckMoveCount(unit);
        break;

    case STATUS_SLOW:
        if ((s8)status->fastTurns > 0) {
            if (turns > 0) {
                status->fastTurns = 0;
                _CheckMoveCount(unit);
                return FALSE;
            }
            else {
                status->slowTurns = turns;
            }
        }
        else {
            status->slowTurns = turns;
        }
        _CheckMoveCount(unit);
        break;

    case STATUS_HP_REGEN:
        status->hpRegenTurns = turns;
        status->hpRegenStrength = strength;
        break;

    case STATUS_FP_REGEN:
        status->fpRegenTurns = turns;
        status->fpRegenStrength = strength;
        break;

    case STATUS_INSTAKILL:
        status->instakillStrength = turns;
        break;

    default:
        break;
    }
    return TRUE;
}

void BtlUnit_ClearStatus(BattleWorkUnit* unit) {
    memset(&unit->statusEffects, 0, sizeof(unit->statusEffects));
    BtlUnit_SetStatus(unit, STATUS_CHARGED, 1u, 157u);
    BattleStatusChangeInfoWorkInit(unit);
}

BOOL BtlUnit_CheckRecoveryStatus(BattleWorkUnit* unit, StatusEffectType type) {
    BattleWorkStatus* status = &unit->statusEffects;
    s8 turns;
    s8 strength;

    if (!unit) {
        return FALSE;
    }

    if (BtlUnit_CheckStatus(unit, STATUS_INSTAKILL)) {
        return FALSE;
    }

    if (unit->currentHP <= 0) {
        return FALSE;
    }

    BtlUnit_GetStatus(unit, type, &turns, &strength);

    if (!turns) {
        return FALSE;
    }

    switch ((s8)type) {
    case STATUS_SLEEP:
        if (turns > 1) {
            turns--;
        }
        else {
            turns = 0;
        }

        status->sleepTurns = turns;

        if (turns == 0) {
            return TRUE;
        }
        else {
            return FALSE;
        }

    case STATUS_ALLERGIC:
    case STATUS_STOPPED:
    case STATUS_DIZZY:
    case STATUS_CONFUSED:
    case STATUS_ELECTRIC:
    case STATUS_DODGY:
    case STATUS_BURN:
    case STATUS_FROZEN:
    case STATUS_INVISIBLE:
    case STATUS_FAST:
    case STATUS_SLOW:
    case STATUS_PAYBACK:
    case STATUS_HOLD_FAST:
        if (turns >= 0) {
            turns--;
        }
        else {
            turns++;
        }

        if (strength >= 0) {
            strength--;
        }
        else {
            strength++;
        }

        if (turns == 0) {
            strength = 0;
        }

        BtlUnit_SetStatus(unit, type, turns, strength);

        if (turns == 0) {
            return TRUE;
        }
        else {
            return FALSE;
        }

    case STATUS_HUGE:
    case STATUS_TINY:
        if (turns > 0) {
            turns--;
        }
        else if (turns < 0) {
            turns++;
        }
        else {
            return FALSE;
        }

        if (!turns) {
            strength = 0;
        }

        status->sizeChangeTurns = turns;
        status->sizeChangeStrength = strength;

        if (turns == 0) {
            return TRUE;
        }
        else {
            return FALSE;
        }

    case STATUS_ATTACK_UP:
    case STATUS_ATTACK_DOWN:
        if (turns > 0) {
            turns--;
        }
        else if (turns < 0) {
            turns++;
        }
        else {
            return FALSE;
        }

        if (!turns) {
            strength = 0;
        }

        status->attackChangeTurns = turns;
        status->attackChangeStrength = strength;

        if (turns == 0) {
            return TRUE;
        }
        else {
            return FALSE;
        }

    case STATUS_POISON:
    case STATUS_DEFENSE_UP:
    case STATUS_DEFENSE_DOWN:
    case STATUS_HP_REGEN:
    case STATUS_FP_REGEN:
        if (turns >= 0) {
            turns--;
        }
        else {
            turns++;
        }

        if (!turns) {
            strength = 0;
        }

        BtlUnit_SetStatus(unit, type, turns, strength);

        if (turns == 0) {
            return TRUE;
        }
        else {
            return FALSE;
        }

    default:
        return FALSE;
    }
}

BOOL BtlUnit_CheckStatus(BattleWorkUnit* unit, StatusEffectType type) {
    s8 turns;
    s8 strength;

    if (!unit) {
        return FALSE;
    }

    BtlUnit_GetStatus(unit, type, &turns, &strength);

    if (turns) {
        return TRUE;
    }

    if ((s8)type == STATUS_ELECTRIC && unit->badgesEquipped.zapTap) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

s32 BtlUnit_CheckStatusFlag(BattleWorkUnit* unit, s32 flags) {
    if (unit) {
        return unit->statusFlags & flags;
    }

    return 0;
}

void BtlUnit_OnStatusFlag(BattleWorkUnit* unit, s32 flags) {
    unit->statusFlags |= flags;
}

void BtlUnit_OffStatusFlag(BattleWorkUnit* unit, s32 flags) {
    unit->statusFlags &= ~flags;
}

s32 BtlUnit_CheckUnitFlag(BattleWorkUnit* unit, s32 flags) {
    return unit->flags & flags;
}

void BtlUnit_OnUnitFlag(BattleWorkUnit* unit, s32 flags) {
    unit->flags |= flags;
}

void BtlUnit_OffUnitFlag(BattleWorkUnit* unit, s32 flags) {
    unit->flags &= ~flags;
}

const char* BtlUnit_GetPoseNameFromType(BattleWorkUnitPart* part, BattlePoseType type) {
    BattlePoseEntry* pose = part->poseTable;
    s32 bound;

    if (type <= 28) {
        bound = 28;
    }
    else if (type < 40) {
        bound = 39;
    }
    else if (type < 70) {
        bound = 69;
    }
    else {
        return NULL;
    }

    while (pose) {
        if (pose->type >= 70) {
            return NULL;
        }

        if (pose->type != type && pose->type != bound) {
            pose++;
        }
        else {
            return pose->name;
        }
    };

    return NULL;
}

void BtlUnit_SetAnimType(BattleWorkUnitPart* part, BattlePoseType type) {
    const char* name;

    name = BtlUnit_GetPoseNameFromType(part, type);
    if (type >= 29 && type < 40) {
        btlDispChangeAnime(part, name, TRUE);
    }
    else {
        btlDispChangeAnime(part, name, FALSE);
    }

    switch (type) {
    case 29:
    case 30:
        part->animFlags |= 0x10;
        break;
    }
}

void BtlUnit_SetAnim(BattleWorkUnitPart* part, const char* name) {
    btlDispChangeAnime(part, name, FALSE);
}

void BtlUnit_SetBodyAnim(BattleWorkUnit* unit, const char* name) {
    BattleWorkUnitPart* part;
    s32 partId;

    partId = BtlUnit_GetBodyPartsId(unit);
    part = BtlUnit_GetPartsPtr(unit, partId);
    btlDispChangeAnime(part, name, FALSE);
}

void BtlUnit_SetBodyAnimType(BattleWorkUnit* unit, BattlePoseType type) {
    BattleWorkUnitPart* part;
    s32 partId;

    partId = BtlUnit_GetBodyPartsId(unit);
    part = BtlUnit_GetPartsPtr(unit, partId);
    BtlUnit_SetAnimType(part, type);
}

void BtlUnit_ChangeTalkAnim(s32 index) {
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    unit = BattleGetUnitPtr(_battleWorkPointer, index);
    part = BtlUnit_GetPartsPtr(unit, unit->talkPartId);
    btlDispChangeAnime(part, unit->talkPoseName, 0);
}

void BtlUnit_ChangeStayAnim(s32 index) {
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    unit = BattleGetUnitPtr(_battleWorkPointer, index);
    part = BtlUnit_GetPartsPtr(unit, unit->talkPartId);
    btlDispChangeAnime(part, unit->stayPoseName, 0);
}

void BtlUnit_GetTalkTogePos(s32 index, f32* x, f32* y, f32* z) {
    BattleWorkUnit* unit;
    f32 pos_x, pos_y, pos_z;

    unit = BattleGetUnitPtr(_battleWorkPointer, index);
    BtlUnit_GetPos(unit, &pos_x, &pos_y, &pos_z);
    pos_x += (f32)unit->faceDirection * (unit->data->togeBaseOffset.x + unit->togeOffset.x);
    pos_y += unit->data->togeBaseOffset.y + unit->togeOffset.y;
    pos_z += unit->data->togeBaseOffset.z + unit->togeOffset.z;
    *x = pos_x;
    *y = pos_y;
    *z = pos_z;
}

s32 BtlUnit_GetEnemyBelong(BattleWorkUnit* unit) {
    s32 alliance = unit->alliance;
    switch (alliance) {
    case 0:
        return 1;
    case 1:
        return 0;
    default:
        return alliance;
    }
}

s32 BtlUnit_GetHitDamage(BattleWorkUnit* unit) {
    return unit->hpDamageTaken;
}

s32 BtlUnit_GetTotalHitDamage(BattleWorkUnit* unit) {
    return unit->totalHpDamageTaken;
}

void BtlUnit_SetTotalHitDamage(BattleWorkUnit* unit, s16 damage) {
    unit->totalHpDamageTaken = damage;
}

BOOL BtlUnit_GetACPossibility(BattleWorkUnit* unit) {
    if (unit->alliance) {
        return FALSE;
    }
    if (unit->currentType == 223) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOPPED)
        || BtlUnit_CheckStatus(unit, STATUS_FROZEN) || BtlUnit_CheckStatus(unit, STATUS_INSTAKILL)) {
        return FALSE;
    }
    return BtlUnit_CheckStatusFlag(unit, 1) == 0;
}

void* BtlUnit_GetData(BattleWorkUnit* unit, BattleDataType type) {
    BattleDataEntry* entry;

    entry = unit->dataTable;
    if (!entry) {
        return NULL;
    }

    while (entry->type) {
        if (entry->type == type) {
            return entry->event;
        }
        entry++;
    }

    return NULL;
}

BOOL BtlUnit_CheckData(BattleWorkUnit* unit, BattleDataType type) {
    BattleDataEntry* entry;

    entry = unit->dataTable;
    if (!entry) {
        return FALSE;
    }
    while (entry->type) {
        if (entry->type == type) {
            return TRUE;
        }
        entry++;
    }
    return FALSE;
}

BOOL BtlUnit_CanGuardStatus(BattleWorkUnit* unit) {
    return BtlUnit_CanActStatus(unit);
}

BOOL BtlUnit_CanActStatus(BattleWorkUnit* unit) {
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOPPED)
        || BtlUnit_CheckStatus(unit, STATUS_FROZEN) || BtlUnit_CheckStatus(unit, STATUS_INSTAKILL)) {
        return FALSE;
    }
    return BtlUnit_CheckStatusFlag(unit, 1) == 0;
}

void BtlUnit_SetParamFromPouch(BattleWorkUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    BattleWorkPartyInfo* info;
    PouchData* pouch;

    pouch = pouchGetPtr();
    if (unit->currentType == UNIT_MARIO) {
        unit->currentHP = pouch->currentHP;
        unit->currentFP = pouch->currentFP;
        BtlUnit_ReviseHpFp(unit);
    }
    else if ((unit->currentType >= UNIT_PARTNER_MIN) && (unit->currentType < UNIT_PARTNER_MAX)) {
        s32 index = BattleTransPartyId(unit->currentType);
        PouchPartyData* data = &pouch->partyData[index];
        unit->currentHP = data->currentHP;
        BtlUnit_ReviseHpFp(unit);
        info = &wp->partyInfo[unit->currentType - UNIT_PARTNER_MIN];
        unit->sizeMultiplier = info->sizeMultiplier;
        unit->statusFlags = info->statusFlags;
        unit->statusEffects = info->statusEffects;
        unit->unk13C = info->unk28;
        unit->unk13E = info->unk2A;
        if (unit->currentHP <= 0) {
            BtlUnit_SetStatus(unit, STATUS_INSTAKILL, 1, 1);
        }
    }
}

void BtlUnit_ReviseHpFp(BattleWorkUnit* unit) {
    PouchData* pouch;
    PouchPartyData* data;

    pouch = pouchGetPtr();
    if (unit->currentType == UNIT_MARIO) {
        unit->baseMaxHP = pouch->baseMaxHP;
        unit->currentMaxHP = pouch->baseMaxHP + (unit->badgesEquipped.hpPlus * 5);
        unit->baseMaxFP = pouch->baseMaxFP;
        unit->currentMaxFP = pouch->baseMaxFP + (unit->badgesEquipped.fpPlus * 5);
        if (unit->currentHP > unit->currentMaxHP) {
            unit->currentHP = unit->currentMaxHP;
        }
        if (unit->currentFP > unit->currentMaxFP) {
            unit->currentFP = unit->currentMaxFP;
        }
    }
    else if ((unit->currentType >= UNIT_PARTNER_MIN) && (unit->currentType < UNIT_PARTNER_MAX)) {
        data = &pouch->partyData[BattleTransPartyId(unit->currentType)];
        unit->baseMaxHP = data->baseMaxHP;
        unit->currentMaxHP = data->baseMaxHP + (unit->badgesEquipped.hpPlus * 5);
        unit->currentHP = data->currentHP;
        if (unit->currentHP > unit->currentMaxHP) {
            unit->currentHP = unit->currentMaxHP;
        }
    }
    else {
        unit->currentMaxHP = unit->baseMaxHP + (unit->badgesEquipped.hpPlus * 5);
        if (unit->currentHP > unit->currentMaxHP) {
            unit->currentHP = unit->currentMaxHP;
        }
        unit->currentMaxFP = unit->baseMaxFP + (unit->badgesEquipped.fpPlus * 5);
        if (unit->currentFP > unit->currentMaxFP) {
            unit->currentFP = unit->currentMaxFP;
        }
    }
}

void BtlUnit_SetParamToPouch(BattleWorkUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    BattleWorkPartyInfo* info;
    PouchData* pouch;

    pouch = pouchGetPtr();
    if (unit->currentType == UNIT_MARIO) {
        pouch->currentHP = unit->currentHP;
        pouch->currentFP = unit->currentFP;
    }
    else if ((unit->currentType >= UNIT_PARTNER_MIN) && (unit->currentType < UNIT_PARTNER_MAX)) {
        s32 index = BattleTransPartyId(unit->currentType);
        PouchPartyData* data = &pouch->partyData[index];
        data->currentHP = unit->currentHP;
        info = &wp->partyInfo[unit->currentType - UNIT_PARTNER_MIN];
        info->sizeMultiplier = unit->sizeMultiplier;
        info->statusFlags = unit->statusFlags;
        info->statusEffects = unit->statusEffects;
        info->unk28 = unit->unk13C;
        info->unk2A = unit->unk13E;
    }
}

void BtlUnit_CheckPinchStatus(BattleWorkUnit* unit, BOOL a2) {
    if (unit->currentHP <= unit->data->perilHP && unit->currentHP < unit->currentMaxHP) {
        if ((unit->statusFlags & 0x20000000) == 0) {
            unit->statusFlags |= 0x20000000;
            if (!a2 && unit->currentType == UNIT_MARIO) {
                BattleAudience_Case_MarioDanger();
            }
        }
    }
    else {
        if ((unit->statusFlags & 0x20000000) != 0) {
            unit->statusFlags &= ~0x20000000;
        }
    }

    if (unit->currentHP <= unit->data->dangerHP && unit->currentHP < unit->currentMaxHP) {
        if ((unit->statusFlags & 0x10000000) == 0) {
            unit->statusFlags |= 0x10000000;
            if ((unit->statusFlags & 0x20000000) == 0 && !a2 && unit->currentType == UNIT_MARIO) {
                BattleAudience_Case_MarioPinch();
            }
        }
    }
    else {
        if ((unit->statusFlags & 0x10000000) != 0) {
            unit->statusFlags &= ~0x10000000;
        }
    }
}

s32 BtlUnit_GetExp(BattleWorkUnit* unit) {
    void* pouch;
    void* battleWork;
    void* stage;
    void* table;
    s32 marioLevel;
    s32 unitLevel;
    s32 levelDiff;
    f32 factor;

    battleWork = _battleWorkPointer;
    pouch = pouchGetPtr();
    marioLevel = *(s16*)((s32)pouch + 0x8A);
    unitLevel = *(u8*)((s32)unit + 0xD);
    if (marioLevel > unitLevel) {
        return 0;
    }

    stage = *(void**)((s32)battleWork + 0x2738);
    table = *(void**)((s32)*(void**)((s32)stage + 0xC) + 0x18);
    switch (*(s32*)table) {
        case 0:
        case 1:
        case 2:
            factor = 1.0f;
            break;
        case 3:
            factor = 1.1f;
            break;
        case 4:
            factor = 1.3f;
            break;
        default:
            factor = 1.5f;
            break;
    }

    levelDiff = unitLevel - marioLevel;
    return (s32)(factor * (f32)levelDiff * 0.5f) + *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0xF);
}

u32 BtlUnit_snd_se(BattleWorkUnit* unit, s32 lookup, s32 volume, s16 add_pitch) {
    Vec position;

    BtlUnit_GetPos(unit, &position.x, &position.y, &position.z);
    return BtlUnit_snd_se_pos(unit, lookup, volume, add_pitch, &position);
}

s32 BtlUnit_snd_se_pos(BattleWorkUnit* unit, s32 lookup, s32 volume, s16 add_pitch, Vec* position) {
    s8 turns;
    s8 strength;
    s16 pitch;
    s16 pitch_shift;
    u32 index;
    u8 vol;

    pitch_shift = 0;
    if (BtlUnit_CheckStatus(unit, STATUS_TINY) != 0) {
        BtlUnit_GetStatus(unit, STATUS_TINY, &turns, &strength);
        pitch_shift = (((s8)strength * 2) - 1) * -0x155;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_HUGE) != 0) {
        BtlUnit_GetStatus(unit, STATUS_HUGE, &turns, &strength);
        pitch_shift = (((s8)strength * 2) + 1) * -0x155;
    }
    pitch = 0;
    pitch += (add_pitch);
    pitch += pitch_shift;
    pitch += irand(0x29) - 20; //pain
    index = psndSFXOn_3D(lookup, position);
    psndSFX_pit(index, pitch);
    if (volume == -250000000) {
        vol = psndSFX_get_vol(index);
    }
    else {
        vol = volume;
    }
    psndSFX_vol(index, vol);
    return index;
}

void BtlUnit_HpGaugeInit(BattleWorkUnit* unit) {
    f32 ratio;
    unit->healthGauge.unk4 = 0;
    unit->healthGauge.unk8 = 0;
    unit->healthGauge.unk0 = 0;
    unit->healthGauge.unkC = unit->currentHP;
    unit->healthGauge.unk10 = unit->currentHP;
    ratio = (f32)unit->currentHP / (f32)unit->currentMaxHP;
    unit->healthGauge.unk14 = ratio;
    unit->healthGauge.unk18 = ratio;
    switch (unit->healthGauge.unk0) {
    case 1:
        unit->healthGauge.unk6 = 40;
        unit->healthGauge.unkA = unit->healthGauge.unk6 + 30;
        break;
    default:
        unit->healthGauge.unk6 = 20;
        unit->healthGauge.unkA = unit->healthGauge.unk6 + 30;
        break;
    }
}

void BtlUnit_HpGaugeMain(BattleWorkUnit* unit) {
    f32 ratio;
    if (unit->healthGauge.unk4 > 0) {
        unit->healthGauge.unk4--;
        if (unit->healthGauge.unk4 <= 0) {
            unit->healthGauge.unkC = unit->healthGauge.unk10;
            ratio = (f32)unit->currentHP / (f32)unit->currentMaxHP;
            unit->healthGauge.unk18 = ratio;
            unit->healthGauge.unk14 = ratio;
        }
        else {
            switch (unit->healthGauge.unk0) {
            case 1:
                unit->healthGauge.unk14 = (f32)unit->currentHP / (f32)unit->currentMaxHP;
                unit->healthGauge.unk18 =
                    unit->healthGauge.unk14
                    + ((f32)(unit->healthGauge.unkC - unit->healthGauge.unk10)
                       * intplGetValue(2, unit->healthGauge.unk4, unit->healthGauge.unk6, 0.0f, 1.0f))
                          / (f32)unit->currentMaxHP;
                break;
            default:
                unit->healthGauge.unk18 = 0.0f;
                unit->healthGauge.unk14 =
                    (f32)unit->currentHP / (f32)unit->currentMaxHP
                    + ((f32)(unit->healthGauge.unkC - unit->healthGauge.unk10)
                       * intplGetValue(0, unit->healthGauge.unk4, unit->healthGauge.unk6, 0.0f, 1.0f))
                          / (f32)unit->currentMaxHP;
                break;
            }
        }
    }

    if (unit->healthGauge.unk8 > 0) {
        unit->healthGauge.unk8--;
    }

    if (unit->flags & 1 && unit->healthGauge.unk8 <= 0 && unit->healthGauge.unk2 & 1) {
        unit->flags &= ~1;
        unit->healthGauge.unk2 &= ~1;
    }
}

s32 BtlUnit_EnemyItemCanUseCheck(ItemType type) {
    s32 item;
    s32* table;

    table = _enemy_can_use_item_table;
    // If we're just using a badge, return it
    if (type >= BADGE_MIN && type < BADGE_MAX) {
        return type;
    }

    // Otherwise, check the item table to see if it's allowed
    while (item = *table) {
        if (item == type) {
            return type;
        }
        table++;
    }
    return 0;
}

BOOL BtlUnit_CheckShadowGuard(BattleWorkUnit* unit) {
    if (unit->attributes & ATTRIBUTE_VEILED) {
        return TRUE;
    }

    if (unit->currentType == UNIT_VIVIAN && unit->work[4] > 0) {
        return TRUE;
    }

    return FALSE;
}

BattleWorkUnit* BattleGetMarioPtr(struct BattleWork* battleWork);
BattleWorkUnit* BattleGetUnitPtr(struct BattleWork* battleWork, s32 unitId);
BattleWorkUnit* BattleGetMarioPtr(struct BattleWork* battleWork);
s32 BtlUnit_GetWeaponCost(BattleWorkUnit* unit, void* weapon);
s32 pouchGetAP(void);
s32 irand(s32 max);

void BtlUnit_PoseSoundInit(void* unit) {
    f32 zero;

    zero = 0.0f;

    *(s32*)((s32)unit + 0x4D4) = 0;
    *(f32*)((s32)unit + 0x4D8) = zero;
    *(f32*)((s32)unit + 0x4DC) = zero;
}

void BtlUnit_SetSeMode(void* se, s32 a, s32 b, s16 c, s16 d, s16 unused, s16 e, s16 f) {
    *(s32*)((s32)se + 0x0) = a;
    *(s32*)((s32)se + 0x4) = b;
    *(s16*)((s32)se + 0xC) = c;
    *(s16*)((s32)se + 0xE) = d;
    *(s16*)((s32)se + 0x10) = d;
    *(s16*)((s32)se + 0x14) = 0;
    *(s16*)((s32)se + 0x8) = e;
    *(s16*)((s32)se + 0xA) = f;
    *(s32*)((s32)se + 0x18) = -1;
}

void BtlUnit_RecoverHp(void* unit, s32 amount) {
    *(s16*)((s32)unit + 0x10C) += amount;

    if (*(s16*)((s32)unit + 0x10C) > *(s16*)((s32)unit + 0x108)) {
        *(s16*)((s32)unit + 0x10C) = *(s16*)((s32)unit + 0x108);
    }
}

s32 BtlUnit_GetMaxFp(void* unit) {
    s32 kind;
    struct BattleWork* battleWork;
    s32 fp;

    kind = *(s32*)((s32)unit + 0x8);
    battleWork = _battleWorkPointer;
    fp = *(s16*)((s32)unit + 0x10E);

    if (kind >= 0xE0 && kind < 0xE7) {
        unit = BattleGetMarioPtr(battleWork);
        fp = *(s16*)((s32)unit + 0x10E);
    }

    return fp;
}

void BtlUnit_SetMaxFp(void* unit, s32 fp) {
    s32 kind;
    struct BattleWork* battleWork;

    kind = *(s32*)((s32)unit + 0x8);
    battleWork = _battleWorkPointer;

    if (kind >= 0xE0 && kind < 0xE7) {
        fp = (s16)fp;
        unit = BattleGetMarioPtr(battleWork);
        *(s16*)((s32)unit + 0x10E) = fp;
    } else {
        *(s16*)((s32)unit + 0x10E) = fp;
    }
}

void BtlUnit_SetFp(void* unit, s32 fp) {
    s32 kind;
    struct BattleWork* battleWork;

    kind = *(s32*)((s32)unit + 0x8);
    battleWork = _battleWorkPointer;

    if (kind >= 0xE0 && kind < 0xE7) {
        fp = (s16)fp;
        unit = BattleGetMarioPtr(battleWork);
        *(s16*)((s32)unit + 0x112) = fp;
    } else {
        *(s16*)((s32)unit + 0x112) = fp;
    }
}

s32 BtlUnit_GetFp(void* unit) {
    s32 kind;
    struct BattleWork* battleWork;
    s32 fp;

    kind = *(s32*)((s32)unit + 0x8);
    battleWork = _battleWorkPointer;
    fp = *(s16*)((s32)unit + 0x112);

    if (kind >= 0xE0 && kind < 0xE7) {
        unit = BattleGetMarioPtr(battleWork);
        fp = *(s16*)((s32)unit + 0x112);
    }

    return fp;
}

s32 BtlUnit_CheckWeaponCost(void* unit, void* weapon) {
    s32 fp;
    s32 cost;
    s32 ap;
    s32 required;

    fp = BtlUnit_GetFp(unit);
    cost = BtlUnit_GetWeaponCost(unit, weapon);

    if (fp < cost) {
        return 0;
    }

    ap = pouchGetAP();
    required = *(u8*)((s32)weapon + 0x12) * 100;

    return ap >= required;
}

void* BtlUnit_GetGuardKouraPtr(void* data) {
    void* battleWork;
    s32 i;
    s32 id;
    void* unit;

    i = 0;
    battleWork = _battleWorkPointer;
    id = *(s32*)((s32)data + 0x0);

    while (i < 0x40) {
        unit = BattleGetUnitPtr(battleWork, i);

        if (unit != 0) {
            if (*(s32*)((s32)unit + 0x8) == 0xDF) {
                if (*(s32*)((s32)unit + 0x218) == id) {
                    break;
                }
            }
        }

        i++;
    }

    if (i >= 0x40) {
        unit = 0;
    }

    return unit;
}

s32 BtlUnit_GetCoin(void* unit) {
    s32 coin;
    s32 i;

    if (unit == 0) {
        return 0;
    }

    i = 0;
    coin = *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0x12);

    while (i < *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0x10)) {
        if (irand(100) < *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0x11)) {
            coin++;
        }

        i++;
    }

    return coin;
}


void BtlUnit_ControlPoseSoundMain(void* part) {
    ;
}


void BtlUnit_PayWeaponCost(void* unit, void* weapon) {
    extern void BtlActRec_AddPoint(void*, u32);
    extern void BtlActRec_AddCount(void*);
    extern void pouchAddAP(s32);
    void* battleWork;
    s32 fp;
    s32 cost;
    s32 type;

    battleWork = _battleWorkPointer;
    fp = BtlUnit_GetFp(unit);
    cost = BtlUnit_GetWeaponCost(unit, weapon);
    BtlUnit_SetFp(unit, fp - cost);

    type = *(s32*)((s32)unit + 8);
    if (type == 0xDE) {
        BtlActRec_AddPoint((void*)((s32)battleWork + 0x16F4B), (u8)cost);
        BtlActRec_AddCount((void*)((s32)battleWork + 0x16F4C));
    } else if (type >= 0xE0 && type < 0xE7) {
        BtlActRec_AddPoint((void*)((s32)battleWork + 0x16F4D), (u8)cost);
        BtlActRec_AddCount((void*)((s32)battleWork + 0x16F4E));
    }

    pouchAddAP((s32)(-(s32)*(u8*)((s32)weapon + 0x12)) * 100);
}

void BtlUnit_RecoverFp(void* unit, s32 fp) {
    void* battleWork;
    void* mario;
    s16 maxFp;
    s16* currentFp;

    battleWork = _battleWorkPointer;
    if (*(s32*)((s32)unit + 8) >= 0xE0 && *(s32*)((s32)unit + 8) < 0xE7) {
        mario = BattleGetMarioPtr(battleWork);
        maxFp = BtlUnit_GetMaxFp(mario);
        mario = BattleGetMarioPtr(battleWork);
        currentFp = (s16*)((s32)mario + 0x112);
    } else {
        maxFp = BtlUnit_GetMaxFp(unit);
        currentFp = (s16*)((s32)unit + 0x112);
    }

    *currentFp += fp;
    if (*currentFp > maxFp) {
        *currentFp = maxFp;
    }
}

u8 BtlUnit_SetCommandAnimPose(void* unit) {
    return 0;
}


BOOL BtlUnit_Delete(BattleWorkUnit* unit) {
    extern void evtDeleteID(s32);
    extern void BattleStatusEffectDelete(void*);
    extern void BattleStatusIconDelete(void*);
    extern void BattleFree(void*);
    extern void smartFree(void*);
    extern void animPoseSetEffect(s32, s32, s32);
    extern void animPoseRelease(s32);
    extern void BattleSetUnitPtr(void*, s32, void*);
    void* battleWork;
    void* part;
    void* data;
    void (*freeFunc)(void*);
    s32 i;
    s32 unitId;

    battleWork = _battleWorkPointer;
    if (unit == 0) {
        return 0;
    }

    for (i = 0; i < 0x40; i++) {
        if (BattleGetUnitPtr(battleWork, i) == unit) {
            break;
        }
    }
    if (i >= 0x40) {
        return 0;
    }

    if (*(s32*)((s32)unit + 0x290) != 0) {
        evtDeleteID(*(s32*)((s32)unit + 0x290));
        *(s32*)((s32)unit + 0x290) = 0;
    }
    if (*(s32*)((s32)unit + 0x29C) != 0) {
        evtDeleteID(*(s32*)((s32)unit + 0x29C));
        *(s32*)((s32)unit + 0x29C) = 0;
    }
    if (*(s32*)((s32)unit + 0x2B4) != 0) {
        evtDeleteID(*(s32*)((s32)unit + 0x2B4));
        *(s32*)((s32)unit + 0x2B4) = 0;
    }

    unitId = *(s32*)unit;
    for (i = 0; i < 0x40; i++) {
        if (*(s32*)((s32)battleWork + 0x120 + i * 4) == unitId) {
            *(s32*)((s32)battleWork + 0x120 + i * 4) = -1;
        }
    }

    if (*(u32*)((s32)unit + 0x1C) & 0x40000000) {
        if (*(s8*)((s32)unit + 0xE) >= 0) {
            data = *(void**)((s32)*(void**)((s32)battleWork + 0x2738) + 0xC);
            *(s32*)((s32)data + *(s8*)((s32)unit + 0xE) * 4 + 0x5C) = *(s32*)((s32)unit + 0x308);
        }
    }

    BattleStatusEffectDelete(unit);
    BattleStatusIconDelete(unit);

    data = *(void**)((s32)unit + 0x314);
    if (data != 0) {
        freeFunc = *(void (**)(void*))((s32)unit + 0x318);
        if (freeFunc != 0) {
            freeFunc(unit);
        } else {
            BattleFree(data);
        }
    }

    if (*(void**)((s32)unit + 0x31C) != 0) {
        smartFree(*(void**)((s32)unit + 0x31C));
        *(void**)((s32)unit + 0x31C) = 0;
    }

    part = *(void**)((s32)unit + 0x14);
    while (part != 0) {
        if (*(s32*)((s32)part + 0x4E8) != -1) {
            *(s32*)((s32)part + 0x4E8) = -1;
        }
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseSetEffect(*(s32*)((s32)part + 0x1C0), 0, 0);
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
        }
        part = *(void**)part;
    }

    BattleFree(*(void**)((s32)unit + 0x14));
    BattleFree(unit);
    BattleSetUnitPtr(battleWork, i, 0);
    return 1;
}

s32 BtlUnit_GetWeaponCost(BattleWorkUnit* unit, void* weapon) {
    return 0;
}


void BtlUnit_LoadSeMode(s32 mode, s32 selector, void* soundDataTable, void* moveSoundWork) {
    s32 zero;

    zero = 0;
    *(s16*)((s32)moveSoundWork + 0x14) = zero;
    *(s32*)((s32)moveSoundWork + 0x18) = -1;

    if (selector != -1) {
        if (selector >= -1) {
            goto clear;
        }
        if (selector >= -2) {
            goto done;
        }
        goto clear;
    }

    if (selector == -1) {
        switch (mode) {
            case 0:
                *(s32*)((s32)moveSoundWork + 0) = *(s32*)((s32)soundDataTable + 8);
                *(s32*)((s32)moveSoundWork + 4) = *(s32*)((s32)soundDataTable + 0xC);
                *(s16*)((s32)moveSoundWork + 0xC) = *(s16*)((s32)soundDataTable + 0x10);
                *(s16*)((s32)moveSoundWork + 0xE) = *(s16*)((s32)soundDataTable + 0x12);
                *(s16*)((s32)moveSoundWork + 0x10) = *(s16*)((s32)soundDataTable + 0x14);
                break;
            case 1:
                *(s32*)((s32)moveSoundWork + 0) = *(s32*)((s32)soundDataTable + 0x18);
                *(s32*)((s32)moveSoundWork + 4) = *(s32*)((s32)soundDataTable + 0x1C);
                *(s16*)((s32)moveSoundWork + 0xC) = *(s16*)((s32)soundDataTable + 0x20);
                *(s16*)((s32)moveSoundWork + 0xE) = *(s16*)((s32)soundDataTable + 0x22);
                *(s16*)((s32)moveSoundWork + 0x10) = *(s16*)((s32)soundDataTable + 0x24);
                break;
            case 2:
                *(s32*)((s32)moveSoundWork + 0) = *(s32*)((s32)soundDataTable + 0);
                *(s32*)((s32)moveSoundWork + 4) = *(s32*)((s32)soundDataTable + 4);
                *(s16*)((s32)moveSoundWork + 0xC) = zero;
                *(s16*)((s32)moveSoundWork + 0xE) = zero;
                *(s16*)((s32)moveSoundWork + 0x10) = zero;
                break;
            case 3:
                *(s32*)((s32)moveSoundWork + 0) = *(s32*)((s32)soundDataTable + 0x28);
                *(s32*)((s32)moveSoundWork + 4) = *(s32*)((s32)soundDataTable + 0x2C);
                *(s16*)((s32)moveSoundWork + 0xC) = *(s16*)((s32)soundDataTable + 0x30);
                *(s16*)((s32)moveSoundWork + 0xE) = *(s16*)((s32)soundDataTable + 0x32);
                *(s16*)((s32)moveSoundWork + 0x10) = *(s16*)((s32)soundDataTable + 0x34);
                break;
            default:
                *(s32*)((s32)moveSoundWork + 0) = zero;
                *(s32*)((s32)moveSoundWork + 4) = zero;
                *(s16*)((s32)moveSoundWork + 0xC) = zero;
                *(s16*)((s32)moveSoundWork + 0xE) = zero;
                *(s16*)((s32)moveSoundWork + 0x10) = zero;
                break;
        }
    }
    goto done;

clear:
        *(s32*)((s32)moveSoundWork + 0) = zero;
        *(s32*)((s32)moveSoundWork + 4) = zero;
        *(s16*)((s32)moveSoundWork + 0xC) = zero;
        *(s16*)((s32)moveSoundWork + 0xE) = zero;
        *(s16*)((s32)moveSoundWork + 0x10) = zero;

done:
    *(s16*)((s32)moveSoundWork + 0x12) = *(s16*)((s32)moveSoundWork + 0xC);
}

void BtlUnit_ResetMoveStatus(BattleWorkUnit* unit) {
    s32 value;
    s32 temp;

    *(u8*)((s32)unit + 0x20) = 0;
    *(u8*)((s32)unit + 0x22) = *(u8*)((s32)unit + 0x21);
    if (BtlUnit_CheckStatus(unit, 0x13) != 0) {
        temp = *(u8*)((s32)unit + 0x22);
        temp <<= 1;
        *(u8*)((s32)unit + 0x22) = temp;
    }
    if (BtlUnit_CheckStatus(unit, 0x14) != 0) {
        if (*(s8*)((s32)unit + 0x22) >= 2) {
            *(u8*)((s32)unit + 0x22) = *(s8*)((s32)unit + 0x22) / 2;
        } else {
            value = 2;
            if (*(s8*)((s32)unit + 0x21) < 1) {
                value <<= 1;
            }
            if (*(s8*)((s32)unit + 0x24) >= value) {
                *(u8*)((s32)unit + 0x22) = 1;
                *(u8*)((s32)unit + 0x20) = 0;
            } else {
                *(u8*)((s32)unit + 0x22) = 0;
                *(u8*)((s32)unit + 0x20) = 10;
            }
        }
    }
    *(u8*)((s32)unit + 0x23) = *(u8*)((s32)unit + 0x22);
    if (*(s8*)((s32)unit + 0x23) == 0) {
        *(u8*)((s32)unit + 0x22) = 0;
        *(u8*)((s32)unit + 0x20) = 10;
    }
}

