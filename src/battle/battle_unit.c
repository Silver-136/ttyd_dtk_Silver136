#pragma auto_inline off
#include "battle/battle_unit.h"
#include "battle/battle.h"
#include "system.h"
#include <string.h>
#include "battle/battle_status_effect.h"
#include "battle/battle_audience.h"
#include "battle/battle_actrecord.h"

#include "battle/battle_disp.h"
#include "mario/mario_pouch.h"
#include "driver/npcdrv.h"
#include "pmario_sound.h"
#include "driver/animdrv.h"
#include "manager/evtmgr.h"
#include "memory.h"

double pow(double, double);
void btlDispChangeAnime(BattleUnitPart* part, const char* name, s32 mode);
void btlDispPoseAnime(BattleUnitPart* part);
void psndSFX_pit(s32 id, s16 pitch);
void psndSFX_vol(s32 id, u8 vol);

typedef struct BtlPoseEntry {
    s32 type;
    char* name;
} BtlPoseEntry;


#define PART_FLAGS(part) ((part)->animFlags)
#define PART_POSE_SOUND(part) (&(part)->poseSound)



s32 _enemy_can_use_item_table[] = {
    ITEM_THUNDER_BOLT, ITEM_FIRE_FLOWER, ITEM_POW_BLOCK, ITEM_THUNDER_RAGE, ITEM_SHOOTING_STAR, ITEM_ICE_STORM,
    ITEM_EARTH_QUAKE, ITEM_HP_DRAIN_ITEM, ITEM_SLEEPY_SHEEP, ITEM_STOPWATCH, ITEM_DIZZY_DIAL, ITEM_MINI_MR_MINI,
    ITEM_MR_SOFTENER, ITEM_RUIN_POWDER, ITEM_POWER_PUNCH, ITEM_COURAGE_SHELL, ITEM_BOOS_SHEET, ITEM_VOLT_SHROOM,
    ITEM_REPEL_CAPE, ITEM_MUSHROOM, ITEM_SUPER_SHROOM, ITEM_ULTRA_SHROOM, ITEM_LIFE_SHROOM, ITEM_DRIED_SHROOM,
    ITEM_TASTY_TONIC, ITEM_HONEY_SYRUP, ITEM_MAPLE_SYRUP, ITEM_JAMMIN_JELLY, ITEM_SLOW_SHROOM, ITEM_GRADUAL_SYRUP,
    ITEM_HOT_DOG, ITEM_CAKE, 0,
};

s32 BtlUnit_snd_se_pos(BattleUnit* unit, const char* sfxName, s32 volume, s16 pitch, Vec* pos);

//local prototypes
void _CheckMoveCount(BattleUnit* unit);

BOOL BtlUnit_Init(void) {
    int i;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        BattleSetUnitPtr(wp, i, NULL);
    }
    return TRUE;
}


void BtlUnit_HpGaugeInit(BattleUnit* unit);
void BattleStatusEffectDelete(BattleUnit* unit);

BattleUnit* BtlUnit_Entry(BattleUnitSetup* setup) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleUnitKindPart* kpart;
    s32 n;
    s32 i;
    BattleUnitPart* body;
    EventEntry* evt;

    for (i = 0; i < 64; i++) {
        if (!BattleGetUnitPtr(wp, i)) break;
    }
    if (i >= 64) return NULL;

    unit = (BattleUnit*)BattleAlloc(sizeof(BattleUnit));
    if (!unit) return NULL;

    memset(unit, 0, sizeof(BattleUnit));
    BattleSetUnitPtr(wp, i, unit);
    unit->unitId = i;
    unit->mKindParams = setup->mUnitKindParams;
    unit->dataTable = unit->mKindParams->dataTable;
    unit->initialKind = setup->mUnitKindParams->mUnitType;
    unit->currentKind = setup->mUnitKindParams->mUnitType;
    BattleStatusEffectInit(unit);
    BattleStatusIconInit(unit);
    BattleStatusChangeInfoWorkInit(unit);
    unit->groupSetupId = -1;
    unit->mAlliance = setup->alliance;
    unit->mLevel = unit->mKindParams->mLevel;
    if (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
        unit->mLevel = pouchGetPtr()->level;
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
    unit->offsetCenter = unit->mKindParams->centerOffset;
    unit->heldItemBaseOffset = unit->mKindParams->heldItemBaseOffset;
    unit->bintaHitOffset = unit->mKindParams->bintaHitOffset;
    unit->kissHitOffset = unit->mKindParams->kissHitOffset;
    unit->baseMaxHp = setup->mUnitKindParams->maxHp;
    unit->maxHp = unit->baseMaxHp;
    unit->currentHp = setup->mUnitKindParams->maxHp;
    unit->baseMaxFp = setup->mUnitKindParams->maxFp;
    unit->maxFp = unit->baseMaxFp;
    unit->currentFp = setup->mUnitKindParams->maxFp;
    unit->mSizeMultiplier = 1.0f;
    unit->mTokenFlags = 0;
    unit->mStatusFlags = 0;
    unit->protectId = -1;
    BtlUnit_HpGaugeInit(unit);
    unit->mWidth = unit->mKindParams->width;
    unit->mHeight = unit->mKindParams->height;
    unit->mStatusIconOffset[0] = unit->mKindParams->hitOffset[0];
    unit->mStatusIconOffset[1] = unit->mKindParams->hitOffset[1];
    unit->mHpGaugeOffset[0] = unit->mKindParams->hpGaugeOffset[0];
    unit->mHpGaugeOffset[1] = unit->mKindParams->hpGaugeOffset[1];
    unit->cutBaseOffset = unit->mKindParams->cutBaseOffset;
    unit->cutWidth = unit->mKindParams->cutWidth;
    unit->cutHeight = unit->mKindParams->cutHeight;
    unit->swallowRate = unit->mKindParams->swallowChance;
    unit->swallowAttribute = unit->mKindParams->swallowAttributes;
    BtlUnit_ClearStatus(unit);
    unit->mAttributeFlags = unit->mKindParams->attributeFlags;
    unit->vulnStatus = (BattleVulnerableStatus*)unit->mKindParams->statusVulnerability;
    unit->maxMoveCount = 1;
    unit->movesRemaining = 1;
    unit->moveDirection = 0;
    unit->faceDirection = wp->alliances[unit->mAlliance].attackDirection;
    unit->work[0] = setup->unitWork[0];
    unit->work[1] = setup->unitWork[1];
    unit->work[2] = setup->unitWork[2];
    unit->work[3] = setup->unitWork[3];

    n = (u8)unit->mKindParams->numParts;
    part = (BattleUnitPart*)BattleAlloc(n * sizeof(BattleUnitPart));
    memset(part, 0, n * sizeof(BattleUnitPart));
    unit->mParts = part;
    kpart = unit->mKindParams->mParts;
    while (TRUE) {
        part->mKindPartParams = kpart;
        part->mOwner = unit;
        part->attributes = part->mKindPartParams->attributes;
        part->counterAttributes = part->mKindPartParams->counterAttributes;
        if (part->attributes & 1) {
            unit->talkPartId = part->mKindPartParams->partNum;
        }
        part->defense = part->mKindPartParams->defense;
        part->defenseAttr = part->mKindPartParams->defenseAttr;
        part->shadowSize = part->mKindPartParams->shadowSize;
        part->reserved_1A4 = part->mKindPartParams->reserved_32;
        part->shadowScale = 1.0f;
        part->moveDirection = 0;
        part->faceDirection = wp->alliances[unit->mAlliance].attackDirection;
        part->mPositionOffset.x = part->mKindPartParams->positionOffset.x;
        part->mPositionOffset.y = part->mKindPartParams->positionOffset.y;
        part->mPositionOffset.z = part->mKindPartParams->positionOffset.z;
        BtlUnit_SetPartsBaseScale(part, 1.0f, 1.0f, 1.0f);
        BtlUnit_SetPartsScale(part, 1.0f, 1.0f, 1.0f);
        BtlUnit_SetPartsBaseRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotateOffset(part, 0.0f, 0.0f, 0.0f);
        part->rotationOrder = 0;
        part->mHitBaseOffset.x = part->mKindPartParams->hitBaseOffset.x;
        part->mHitBaseOffset.y = part->mKindPartParams->hitBaseOffset.y;
        part->mHitBaseOffset.z = part->mKindPartParams->hitBaseOffset.z;
        part->mHitOffset.x = 0.0f;
        part->mHitOffset.y = 0.0f;
        part->mHitOffset.z = 0.0f;
        part->mHitCursorBaseOffset.x = part->mKindPartParams->hitCursorBaseOffset.x;
        part->mHitCursorBaseOffset.y = part->mKindPartParams->hitCursorBaseOffset.y;
        part->mHitCursorBaseOffset.z = part->mKindPartParams->hitCursorBaseOffset.z;
        part->mHitCursorOffset.x = 0.0f;
        part->mHitCursorOffset.y = 0.0f;
        part->mHitCursorOffset.z = 0.0f;
        part->color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
        part->hitOffsetX = setup->hitOffsetX;
        part->poseTable = kpart->poseTable;
        part->poseId = -1;
        part->poseName[0] = 0;
        part->animSpeed = 1.0f;
        part->animSpeed2 = 1.0f;
        part->poseType = 0;
        part->poseState = 0;
        part->handle = -1;
        part->partName = part->mKindPartParams->partName;
        if (--n <= 0) break;
        body = part;
        part++;
        body->mNextPart = part;
        kpart++;
    }
    part->mNextPart = NULL;

    body = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    unit->talkPose = BtlUnit_GetPoseNameFromType(body, 0x41);
    unit->stayPose = BtlUnit_GetPoseNameFromType(body, 0x2B);
    unit->mMoveColorLv = 0xFF;
    unit->moveColorLvDisp = 0xFF;
    btlDispEntAnime(unit);
    unit->countersetWeapon = NULL;
    unit->deleteFunc = NULL;
    switch (unit->currentKind) {
        case 0xD0:
            unit->countersetWeapon = BattleAlloc(0x44);
            break;
        case 0xDE:
            break;
    }
    BtlUnit_SetParamFromPouch(unit);
    unit->initEventId = 0;
    if (unit->mKindParams->initEventCode) {
        evt = evtEntry(unit->mKindParams->initEventCode, 10, 0);
        evt->unitId = unit->unitId;
        unit->initEventId = evt->eventId;
    }
    return unit;
}

BOOL BtlUnit_Delete(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleUnitPart* part;
    s32 j;
    s32 id;

    if (!unit) return FALSE;
    for (i = 0; i < 64; i++) {
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
    for (j = 0; j < 64; j++) {
        if (wp->movePriorityQueue[j] == id) {
            wp->movePriorityQueue[j] = -1;
        }
    }
    if (unit->flags & 0x40000000) {
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
    if ((void*)unit->screenTexBuf) {
        smartFree(unit->screenTexBuf);
        unit->screenTexBuf = NULL;
    }
    for (part = unit->mParts; part; part = part->mNextPart) {
        if (part->handle != -1) {
            part->handle = -1;
        }
        if (part->poseId != -1) {
            animPoseSetEffect(part->poseId, NULL, 0);
            animPoseRelease(part->poseId);
        }
    }
    BattleFree(unit->mParts);
    BattleFree(unit);
    BattleSetUnitPtr(wp, i, NULL);
    return TRUE;
}

BattleUnit* BtlUnit_Spawn(BattleUnitSetup* setup, s32 flags) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 i;
    s32 id;
    s32 tmp;

    unit = BtlUnit_Entry(setup);
    if (!unit) return unit;
    if (flags & 2) {
        unit->moveState = 0;
        id = unit->unitId;
        for (i = 0; i < 64; i++) {
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
        unit->moveState = 0;
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

s32 BtlUnit_GetUnitId(BattleUnit* unit) {
    return unit->unitId;
}

BattleUnitPart* BtlUnit_GetPartsPtr(BattleUnit* unit, s32 partNum) {
    BattleUnitPart* part = unit->mParts;

    if (partNum < 0) {
        return part;
    }
    if (!part->mNextPart) {
        return part;
    }
    for (; part; part = part->mNextPart) {
        if (part->mKindPartParams->partNum == partNum) {
            return part;
        }
    }
    return part;
}

s32 BtlUnit_GetBodyPartsId(BattleUnit* unit) {
    BattleUnitPart* part;
    s32 ret = -1;

    if (unit) {
        for (part = unit->mParts; part; part = part->mNextPart) {
            if (part->attributes & kMainPart) {
                ret = part->mKindPartParams->partNum;
                break;
            }
        }
    }
    return ret;
}

void BtlUnit_GetPos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mPosition.x;
    *y = unit->mPosition.y;
    *z = unit->mPosition.z;
}

void BtlUnit_SetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mPosition.x = x;
    unit->mPosition.y = y;
    unit->mPosition.z = z;
}

void BtlUnit_AddPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mPosition.x += x;
    unit->mPosition.y += y;
    unit->mPosition.z += z;
}

void BtlUnit_GetPartsPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->mPosition.x;
    *y = part->mPosition.y;
    *z = part->mPosition.z;
}

void BtlUnit_SetPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mPosition.x = x;
    part->mPosition.y = y;
    part->mPosition.z = z;
}

void BtlUnit_AddPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mPosition.x += x;
    part->mPosition.y += y;
    part->mPosition.z += z;
}

void BtlUnit_GetPartsWorldPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    BattleUnit* unit = part->mOwner;
    f32 ux, ox, uy, oy, uz, oz;

    if (part->attributes & 0x10000000) {
        *x = part->mPosition.x;
        *y = part->mPosition.y;
        *z = part->mPosition.z;
    } else {
        BtlUnit_GetPos(unit, &ux, &uy, &uz);
        BtlUnit_GetPartsOffsetPos(part, &ox, &oy, &oz);
        *x = ox * unit->faceDirection + ux;
        *y = uy + oy;
        *z = uz + oz;
    }
}

void BtlUnit_GetHitPos(BattleUnit* unit, BattleUnitPart* part, f32* x, f32* y, f32* z) {
    f32 y_multiply = 1.0f;
    s32 direction = _battleWorkPointer->alliances[unit->mAlliance].attackDirection;
    BtlUnit_GetPartsWorldPos(part, x, y, z);
    //add hit base offset
    *x = ((f32)direction * (part->mHitBaseOffset.x * unit->mSizeMultiplier)) + *x;
    *y = (y_multiply * (part->mHitBaseOffset.y * unit->mSizeMultiplier)) + *y;
    *z = (part->mHitBaseOffset.z * unit->mSizeMultiplier) + *z;
    //add hit offset
    *x = ((f32)direction * (part->mHitOffset.x * unit->mSizeMultiplier)) + *x;
    *y = (y_multiply * (part->mHitOffset.y * unit->mSizeMultiplier)) + *y;
    *z = (part->mHitOffset.z * unit->mSizeMultiplier) + *z;
}

void BtlUnit_SetHitOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mHitOffset.x = x;
    part->mHitOffset.y = y;
    part->mHitOffset.z = z;
}

void BtlUnit_SetHitCursorOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mHitCursorOffset.x = x;
    part->mHitCursorOffset.y = y;
    part->mHitCursorOffset.z = z;
}

void BtlUnit_GetHomePos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mHomePosition.x;
    *y = unit->mHomePosition.y;
    *z = unit->mHomePosition.z;
}

void BtlUnit_SetHomePos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mHomePosition.x = x;
    unit->mHomePosition.y = y;
    unit->mHomePosition.z = z;
}

void BtlUnit_AddHomePos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mHomePosition.x += x;
    unit->mHomePosition.y += y;
    unit->mHomePosition.z += z;
}

void BtlUnit_SetPartsHomePos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mHomePosition.x = x;
    part->mHomePosition.y = y;
    part->mHomePosition.z = z;
}

void BtlUnit_SetRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 new_x, new_y, new_z;

    new_x = reviseAngle(x);
    if (new_x >= 360.0f) {
        new_x = 0.0f;
    }
    unit->mRotation.x = new_x;

    new_y = reviseAngle(y);
    if (new_y >= 360.0f) {
        new_y = 0.0f;
    }
    unit->mRotation.y = new_y;

    new_z = reviseAngle(z);
    if (new_z >= 360.0f) {
        new_z = 0.0f;
    }
    unit->mRotation.z = new_z;
}

void BtlUnit_GetRotate(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mRotation.x;
    *y = unit->mRotation.y;
    *z = unit->mRotation.z;
}

void BtlUnit_AddRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mRotation.x = reviseAngle(unit->mRotation.x + x);
    if (unit->mRotation.x >= 360.0f) {
        unit->mRotation.x = 0.0f;
    }
    unit->mRotation.y = reviseAngle(unit->mRotation.y + y);
    if (unit->mRotation.y >= 360.0f) {
        unit->mRotation.y = 0.0f;
    }
    unit->mRotation.z = reviseAngle(unit->mRotation.z + z);
    if (unit->mRotation.z >= 360.0f) {
        unit->mRotation.z = 0.0f;
    }
}

void BtlUnit_SetPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mRotation.x = x;
    part->mRotation.y = y;
    part->mRotation.z = z;
}

void BtlUnit_GetPartsRotate(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->mRotation.x;
    *y = part->mRotation.y;
    *z = part->mRotation.z;
}

void BtlUnit_AddPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mRotation.x += x;
    if (part->mRotation.x > 360.0f) {
        part->mRotation.x -= 360.0f;
    }
    else if (part->mRotation.x < 0.0f) {
        part->mRotation.x += 360.0f;
    }

    part->mRotation.y += y;
    if (part->mRotation.y > 360.0f) {
        part->mRotation.y -= 360.0f;
    }
    else if (part->mRotation.y < 0.0f) {
        part->mRotation.y += 360.0f;
    }

    part->mRotation.z += z;
    if (part->mRotation.z > 360.0f) {
        part->mRotation.z -= 360.0f;
    }
    else if (part->mRotation.z < 0.0f) {
        part->mRotation.z += 360.0f;
    }
}

void BtlUnit_SetBaseRotate(BattleUnit* unit, f32 x, f32 y, f32 z) {
    unit->mBaseRotation.x = reviseAngle(x);
    unit->mBaseRotation.y = reviseAngle(y);
    unit->mBaseRotation.z = reviseAngle(z);
}

void BtlUnit_GetBaseRotate(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mBaseRotation.x;
    *y = unit->mBaseRotation.y;
    *z = unit->mBaseRotation.z;
}

void BtlUnit_SetPartsBaseRotate(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mBaseRotation.x = x;
    part->mBaseRotation.y = y;
    part->mBaseRotation.z = z;
}

void BtlUnit_GetPartsBaseRotate(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->mBaseRotation.x;
    *y = part->mBaseRotation.y;
    *z = part->mBaseRotation.z;
}

void BtlUnit_SetRotateOffset(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mRotationOffset.x = x;
    }
    if (noValue != y) {
        unit->mRotationOffset.y = y;
    }
    if (noValue != z) {
        unit->mRotationOffset.z = z;
    }
}

void BtlUnit_SetPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mRotationOffset.x = x;
    }
    if (noValue != y) {
        part->mRotationOffset.y = y;
    }
    if (noValue != z) {
        part->mRotationOffset.z = z;
    }
}

void BtlUnit_AddPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mRotationOffset.x += x;
    part->mRotationOffset.y += y;
    part->mRotationOffset.z += z;
}

void BtlUnit_SetBaseScale(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mBaseScale.x = x;
    }
    if (noValue != y) {
        unit->mBaseScale.y = y;
    }
    if (noValue != z) {
        unit->mBaseScale.z = z;
    }
}

void BtlUnit_GetScale(BattleUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mScale.x;
    *y = unit->mScale.y;
    *z = unit->mScale.z;
}

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

void BtlUnit_SetPartsBaseScale(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mBaseScale.x = x;
    }
    if (noValue != y) {
        part->mBaseScale.y = y;
    }
    if (noValue != z) {
        part->mBaseScale.z = z;
    }
}

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

s32 BtlUnit_GetWidth(BattleUnit* unit) {
    return unit->mWidth;
}

s32 BtlUnit_GetHeight(BattleUnit* unit) {
    return unit->mHeight;
}

void BtlUnit_SetHeight(BattleUnit* unit, s16 height) {
    unit->mHeight = height;
}

void BtlUnit_SetOffsetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mPositionOffset.x = x;
    }
    if (noValue != y) {
        unit->mPositionOffset.y = y;
    }
    if (noValue != z) {
        unit->mPositionOffset.z = z;
    }
}

void BtlUnit_GetPartsOffsetPos(BattleUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->mPositionOffset.x;
    *y = part->mPositionOffset.y;
    *z = part->mPositionOffset.z;
}

void BtlUnit_SetPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mPositionOffset.x = x;
    }
    if (noValue != y) {
        part->mPositionOffset.y = y;
    }
    if (noValue != z) {
        part->mPositionOffset.z = z;
    }
}

void BtlUnit_AddPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    part->mPositionOffset.x += x;
    part->mPositionOffset.y += y;
    part->mPositionOffset.z += z;
}

void BtlUnit_SetDispOffset(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mDisplayOffset.x = x;
    }
    if (noValue != y) {
        unit->mDisplayOffset.y = y;
    }
    if (noValue != z) {
        unit->mDisplayOffset.z = z;
    }
}

void BtlUnit_SetPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mDisplayOffset.x = x;
    }
    if (noValue != y) {
        part->mDisplayOffset.y = y;
    }
    if (noValue != z) {
        part->mDisplayOffset.z = z;
    }
}

void BtlUnit_AddPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mDisplayOffset.x += x;
    }
    if (noValue != y) {
        part->mDisplayOffset.y += y;
    }
    if (noValue != z) {
        part->mDisplayOffset.z += z;
    }
}

void BtlUnit_SetMoveStartPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mMoveStartPos.x = x;
    }
    if (noValue != y) {
        unit->mMoveStartPos.y = y;
    }
    if (noValue != z) {
        unit->mMoveStartPos.z = z;
    }
}

void BtlUnit_SetPartsMoveStartPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mMoveStartPos.x = x;
    }
    if (noValue != y) {
        part->mMoveStartPos.y = y;
    }
    if (noValue != z) {
        part->mMoveStartPos.z = z;
    }
}

void BtlUnit_SetMoveCurrentPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mMoveCurrentPos.x = x;
    }
    if (noValue != y) {
        unit->mMoveCurrentPos.y = y;
    }
    if (noValue != z) {
        unit->mMoveCurrentPos.z = z;
    }
}

void BtlUnit_SetPartsMoveCurrentPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mMoveCurrentPos.x = x;
    }
    if (noValue != y) {
        part->mMoveCurrentPos.y = y;
    }
    if (noValue != z) {
        part->mMoveCurrentPos.z = z;
    }
}

void BtlUnit_SetMoveTargetPos(BattleUnit* unit, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        unit->mMoveTargetPos.x = x;
    }
    if (noValue != y) {
        unit->mMoveTargetPos.y = y;
    }
    if (noValue != z) {
        unit->mMoveTargetPos.z = z;
    }
}

void BtlUnit_SetPartsMoveTargetPos(BattleUnitPart* part, f32 x, f32 y, f32 z) {
    f32 noValue = -250000000.0f;
    if (noValue != x) {
        part->mMoveTargetPos.x = x;
    }
    if (noValue != y) {
        part->mMoveTargetPos.y = y;
    }
    if (noValue != z) {
        part->mMoveTargetPos.z = z;
    }
}

void BtlUnit_SetFallAccel(BattleUnit* unit, f32 fallAccel) {
    unit->mFallAccel = fallAccel;
}

void BtlUnit_SetPartsFallAccel(BattleUnitPart* part, f32 fallAccel) {
    part->mFallAccel = fallAccel;
}

void BtlUnit_SetMoveSpeed(BattleUnit* unit, f32 moveSpeedXZ) {
    unit->mMoveSpeedXZ = moveSpeedXZ;
}

void BtlUnit_SetPartsMoveSpeed(BattleUnitPart* part, f32 moveSpeedXZ) {
    part->mMoveSpeedXZ = moveSpeedXZ;
}

void BtlUnit_SetJumpSpeed(BattleUnit* unit, f32 moveSpeedY) {
    unit->mMoveSpeedY = moveSpeedY;
}

s8 BtlUnit_GetBelong(BattleUnit* unit) {
    return unit->mAlliance;
}

void _CheckMoveCount(BattleUnit* unit) {
    s8 maxmoves;

    if (!(unit->mTokenFlags & kCurrentlyActing)) {
        maxmoves = unit->maxMovesThisTurn;
        if (maxmoves > 0 && maxmoves == unit->movesRemaining) {
            BtlUnit_ResetMoveStatus(unit);
        }
    }
}

void BtlUnit_GetStatus(BattleUnit* unit, StatusEffectType type, s8* turns, s8* strength) {
    s8 status_turns = 0;
    s8 status_strength = 0;

    switch ((s8)type) {
        case STATUS_SLEEP:
            status_strength = unit->mSleepTurns;
            status_turns = status_strength;
            break;
        case STATUS_STOP:
            status_strength = unit->mStopTurns;
            status_turns = status_strength;
            break;
        case STATUS_DIZZY:
            status_strength = unit->mDizzyTurns;
            status_turns = status_strength;
            break;
        case STATUS_POISON:
            status_turns = unit->mPoisonTurns;
            status_strength = unit->mPoisonStrength;
            break;
        case STATUS_CONFUSE:
            status_strength = unit->mConfusionTurns;
            status_turns = status_strength;
            break;
        case STATUS_ELECTRIC:
            status_strength = unit->mElectricTurns;
            status_turns = status_strength;
            break;
        case STATUS_DODGY:
            status_strength = unit->mDodgyTurns;
            status_turns = status_strength;
            break;
        case STATUS_BURN:
            status_strength = unit->mBurnTurns;
            status_turns = status_strength;
            break;
        case STATUS_FREEZE:
            status_strength = unit->mFreezeTurns;
            status_turns = status_strength;
            break;
        case STATUS_HUGE:
            if (unit->mSizeChangeStrength > 0) {
                status_turns = unit->mSizeChangeTurns;
                status_strength = unit->mSizeChangeStrength;
            }
            break;
        case STATUS_TINY:
            if (unit->mSizeChangeStrength < 0) {
                status_turns = unit->mSizeChangeTurns;
                status_strength = unit->mSizeChangeStrength;
            }
            break;
        case STATUS_ATTACK_UP:
            if (unit->mAtkChangeStrength > 0) {
                status_turns = unit->mAtkChangeTurns;
                status_strength = unit->mAtkChangeStrength;
            }
            break;
        case STATUS_ATTACK_DOWN:
            if (unit->mAtkChangeStrength < 0) {
                status_turns = unit->mAtkChangeTurns;
                status_strength = unit->mAtkChangeStrength;
            }
            break;
        case STATUS_DEFENSE_UP:
            if (unit->mDefChangeStrength > 0) {
                status_turns = unit->mDefChangeTurns;
                status_strength = unit->mDefChangeStrength;
            }
            break;
        case STATUS_DEFENSE_DOWN:
            if (unit->mDefChangeStrength < 0) {
                status_turns = unit->mDefChangeTurns;
                status_strength = unit->mDefChangeStrength;
            }
            break;
        case STATUS_FAST:
            status_strength = unit->mFastTurns;
            status_turns = status_strength;
            break;
        case STATUS_SLOW:
            status_strength = unit->mSlowTurns;
            status_turns = status_strength;
            break;
        case STATUS_CHARGE:
            status_strength = unit->mChargeStrength;
            status_turns = status_strength;
            break;
        case STATUS_ALLERGIC:
            status_strength = unit->mAllergicTurns;
            status_turns = status_strength;
            break;
        case STATUS_FLIPPED:
            status_strength = unit->flippedTurns;
            status_turns = status_strength;
            break;
        case STATUS_INVISIBLE:
            status_strength = unit->mInvisibleTurns;
            status_turns = status_strength;
            break;
        case STATUS_PAYBACK:
            status_strength = unit->mPaybackTurns;
            status_turns = status_strength;
            break;
        case STATUS_HOLD_FAST:
            status_strength = unit->mHoldFastTurns;
            status_turns = status_strength;
            break;
        case STATUS_HP_REGEN:
            status_turns = unit->mHpRegenTurns;
            status_strength = unit->mHpRegenStrength;
            break;
        case STATUS_FP_REGEN:
            status_turns = unit->mFpRegenTurns;
            status_strength = unit->mFpRegenStrength;
            break;
        case STATUS_INSTANT_KILL:
            status_strength = unit->mInstantKillStrength;
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

BOOL BtlUnit_SetStatus(BattleUnit* unit, StatusEffectType type, s8 turns, s8 strength) {
    switch ((s8)type) {
        case STATUS_SLEEP:
            unit->mSleepTurns = turns;
            unit->mSleepStrength = 0;
            break;
        case STATUS_STOP:
            unit->mStopTurns = turns;
            break;
        case STATUS_DIZZY:
            unit->mDizzyTurns = turns;
            break;
        case STATUS_POISON:
            unit->mPoisonTurns = turns;
            unit->mPoisonStrength = strength;
            break;
        case STATUS_CONFUSE:
            unit->mConfusionTurns = turns;
            break;
        case STATUS_ELECTRIC:
            unit->mElectricTurns = turns;
            break;
        case STATUS_DODGY:
            unit->mDodgyTurns = turns;
            break;
        case STATUS_BURN:
            if (unit->mFreezeTurns) {
                unit->mFreezeTurns = 0;
                return FALSE;
            }
            unit->mBurnTurns = turns;
            break;
        case STATUS_FREEZE:
            if (unit->mBurnTurns) {
                unit->mBurnTurns = 0;
                return FALSE;
            }
            unit->mFreezeTurns = turns;
            break;
        case STATUS_HUGE:
        case STATUS_TINY:
            if (!strength) {
                if ((s8)type == STATUS_HUGE) {
                    if (unit->mSizeChangeStrength >= 0) {
                        unit->mSizeChangeTurns = 0;
                        unit->mSizeChangeStrength = 0;
                        return FALSE;
                    }
                }
                else if (unit->mSizeChangeStrength <= 0) {
                    unit->mSizeChangeTurns = 0;
                    unit->mSizeChangeStrength = 0;
                    return FALSE;
                }
                return FALSE;
            }
            unit->mSizeChangeTurns = turns;
            unit->mSizeChangeStrength = strength;
            break;
        case STATUS_ATTACK_UP:
            if (!strength) {
                if (unit->mAtkChangeStrength >= 0) {
                    unit->mAtkChangeTurns = 0;
                    unit->mAtkChangeStrength = 0;
                }
                return FALSE;
            }
            unit->mAtkChangeTurns = turns;
            unit->mAtkChangeStrength = strength;
            break;
        case STATUS_ATTACK_DOWN:
            if (!strength) {
                if (unit->mAtkChangeStrength <= 0) {
                    unit->mAtkChangeTurns = 0;
                    unit->mAtkChangeStrength = 0;
                }
                return FALSE;
            }
            unit->mAtkChangeTurns = turns;
            unit->mAtkChangeStrength = strength;
            break;
        case STATUS_DEFENSE_UP:
        case STATUS_DEFENSE_DOWN:
            if (strength > 0) {
                if (unit->mDefChangeStrength < 0) {
                    unit->mDefChangeTurns = 0;
                    unit->mDefChangeStrength = 0;
                    return FALSE;
                }
                unit->mDefChangeTurns = turns;
                unit->mDefChangeStrength = strength;
            }
            else if (strength == 0) {
                if ((s8)type == STATUS_DEFENSE_DOWN) {
                    if (unit->mDefChangeStrength <= 0) {
                        unit->mDefChangeTurns = 0;
                        unit->mDefChangeStrength = 0;
                    }
                }
                else if (unit->mDefChangeStrength >= 0) {
                    unit->mDefChangeTurns = 0;
                    unit->mDefChangeStrength = 0;
                }
                return FALSE;
            }
            else {
                if (unit->mDefChangeStrength > 0) {
                    unit->mDefChangeTurns = 0;
                    unit->mDefChangeStrength = 0;
                    return FALSE;
                }
                unit->mDefChangeTurns = turns;
                unit->mDefChangeStrength = strength;
            }
            break;
        case STATUS_CHARGE:
            unit->mChargeStrength += strength;
            if (unit->mChargeStrength < 0) {
                unit->mChargeStrength = 0;
                return FALSE;
            }
            if (unit->mChargeStrength > 99) {
                unit->mChargeStrength = 99;
            }
            break;
        case STATUS_ALLERGIC:
            if (turns > 0) {
                unit->mAllergicTurns = turns;
            }
            else {
                unit->mAllergicTurns = 0;
                return FALSE;
            }
            break;
        case STATUS_FLIPPED:
            unit->flippedTurns = turns;
            break;
        case STATUS_INVISIBLE:
            unit->mInvisibleTurns = turns;
            break;
        case STATUS_PAYBACK:
            if (turns > 0) {
                unit->mPaybackTurns = turns;
            }
            else {
                unit->mPaybackTurns = 0;
                return FALSE;
            }
            break;
        case STATUS_HOLD_FAST:
            unit->mHoldFastTurns = turns;
            break;
        case STATUS_FAST:
            if (unit->mSlowTurns > 0) {
                if (turns > 0) {
                    unit->mSlowTurns = 0;
                    _CheckMoveCount(unit);
                    return FALSE;
                }
                unit->mFastTurns = turns;
            }
            else {
                unit->mFastTurns = turns;
            }
            _CheckMoveCount(unit);
            break;
        case STATUS_SLOW:
            if (unit->mFastTurns > 0) {
                if (turns > 0) {
                    unit->mFastTurns = 0;
                    _CheckMoveCount(unit);
                    return FALSE;
                }
                unit->mSlowTurns = turns;
            }
            else {
                unit->mSlowTurns = turns;
            }
            _CheckMoveCount(unit);
            break;
        case STATUS_HP_REGEN:
            unit->mHpRegenTurns = turns;
            unit->mHpRegenStrength = strength;
            break;
        case STATUS_FP_REGEN:
            unit->mFpRegenTurns = turns;
            unit->mFpRegenStrength = strength;
            break;
        case STATUS_INSTANT_KILL:
            unit->mInstantKillStrength = turns;
            break;
        default:
            break;
    }
    return TRUE;
}

//TODO: define that chunk of statuses to do sizeof?
void BtlUnit_ClearStatus(BattleUnit* unit) {
    memset(&unit->mSleepTurns, 0, 0x1Eu);
    BtlUnit_SetStatus(unit, STATUS_CHARGE, 1, -99);
    BattleStatusChangeInfoWorkInit(unit);
}

BOOL BtlUnit_CheckRecoveryStatus(BattleUnit* unit, StatusEffectType type) {
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
    switch ((s8)type) {
        case STATUS_SLEEP:
            if (turns > 1) {
                turns--;
            }
            else {
                turns = 0;
            }
            unit->mSleepTurns = turns;
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
            unit->mSizeChangeTurns = turns;
            unit->mSizeChangeStrength = strength;
            if (!turns) {
                return TRUE;
            }
            return FALSE;
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
            unit->mAtkChangeTurns = turns;
            unit->mAtkChangeStrength = strength;
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
            }
            else {
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

BOOL BtlUnit_CheckStatus(BattleUnit* unit, StatusEffectType type) {
    s8 turns, strength;

    if (!unit) {
        return FALSE;
    }
    BtlUnit_GetStatus(unit, type, &turns, &strength);
    if (turns) {
        return TRUE;
    }
    if ((s8)type == STATUS_ELECTRIC && unit->badgesEquipped.ZapTap) {
        return TRUE;
    }
    return FALSE;
}

s32 BtlUnit_CheckStatusFlag(BattleUnit* unit, s32 flags) {
    if (unit) {
        return unit->mStatusFlags & flags;
    }
    return 0;
}

void BtlUnit_OnStatusFlag(BattleUnit* unit, s32 flags) {
    unit->mStatusFlags |= flags;
}

void BtlUnit_OffStatusFlag(BattleUnit* unit, s32 flags) {
    unit->mStatusFlags &= ~flags;
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

char* BtlUnit_GetPoseNameFromType(BattleUnitPart* part, s32 type) {
    BtlPoseEntry* pose = (BtlPoseEntry*)part->poseTable;
    s32 alt;

    if (type <= 0x1C) {
        alt = 0x1C;
    }
    else if (type < 0x28) {
        alt = 0x27;
    }
    else if (type < 0x46) {
        alt = 0x45;
    }
    else {
        return NULL;
    }
    while (pose) {
        if (pose->type >= 0x46) {
            return NULL;
        }
        if (pose->type != type && pose->type != alt) {
            pose++;
            continue;
        }
        return pose->name;
    }
    return NULL;
}

void BtlUnit_SetAnimType(BattleUnitPart* part, s32 type) {
    char* name = BtlUnit_GetPoseNameFromType(part, type);

    if (type >= 0x1D && type < 0x28) {
        btlDispChangeAnime(part, name, 1);
    }
    else {
        btlDispChangeAnime(part, name, 0);
    }
    switch (type) {
        case 0x1D:
        case 0x1E:
            PART_FLAGS(part) |= 0x10;
            break;
    }
}

void BtlUnit_SetAnim(BattleUnitPart* part, const char* name) {
    btlDispChangeAnime(part, name, 0);
}

void BtlUnit_SetBodyAnimType(BattleUnit* unit, s32 type) {
    s32 id = BtlUnit_GetBodyPartsId(unit);
    BtlUnit_SetAnimType(BtlUnit_GetPartsPtr(unit, id), type);
}

void BtlUnit_SetBodyAnim(BattleUnit* unit, const char* name) {
    s32 id = BtlUnit_GetBodyPartsId(unit);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, id), name, 0);
}

void BtlUnit_ChangeTalkAnim(s32 unitId) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, unit->talkPartId), unit->talkPose, 0);
}

void BtlUnit_ChangeStayAnim(s32 unitId) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    btlDispChangeAnime(BtlUnit_GetPartsPtr(unit, unit->talkPartId), unit->stayPose, 0);
}

void BtlUnit_GetTalkTogePos(s32 unitId, f32* x, f32* y, f32* z) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    f32 posX, posY, posZ;

    BtlUnit_GetPos(unit, &posX, &posY, &posZ);
    posX += unit->faceDirection * (unit->mKindParams->talkTogeBaseOffset.x + unit->togeOffset.x);
    posY += unit->mKindParams->talkTogeBaseOffset.y + unit->togeOffset.y;
    posZ += unit->mKindParams->talkTogeBaseOffset.z + unit->togeOffset.z;
    *x = posX;
    *y = posY;
    *z = posZ;
}

s32 BtlUnit_GetEnemyBelong(BattleUnit* unit) {
    s32 belong = unit->mAlliance;
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

s32 BtlUnit_GetHitDamage(BattleUnit* unit) {
    return unit->hpDamageTaken;
}

s32 BtlUnit_GetTotalHitDamage(BattleUnit* unit) {
    return unit->totalHpDamageTaken;
}

void BtlUnit_SetTotalHitDamage(BattleUnit* unit, s16 damage) {
    unit->totalHpDamageTaken = damage;
}

BOOL BtlUnit_GetACPossibility(BattleUnit* unit) {
    if (unit->mAlliance) {
        return FALSE;
    }
    if (unit->currentKind == UNIT_SHELL_SHIELD) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        return FALSE;
    }
    return !BtlUnit_CheckStatusFlag(unit, 1);
}

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

BOOL BtlUnit_CanGuardStatus(BattleUnit* unit) {
    return BtlUnit_CanActStatus(unit);
}

BOOL BtlUnit_CanActStatus(BattleUnit* unit) {
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
        return FALSE;
    }
    return !BtlUnit_CheckStatusFlag(unit, 1);
}

void BtlUnit_SetParamFromPouch(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    BattleWorkPartyInfo* info;

    if (unit->currentKind == UNIT_MARIO) {
        unit->currentHp = pouch->currentHP;
        unit->currentFp = pouch->currentFP;
        BtlUnit_ReviseHpFp(unit);
    }
    else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        unit->currentHp = *(s16*)((u8*)pouch->partyData + BattleTransPartyId(unit->currentKind) * sizeof(PouchPartyData) + offsetof(PouchPartyData, currentHP)); // FAKE: byte-offset load so the field offset folds into the index (mulli/addi/lhax), as in the target
        BtlUnit_ReviseHpFp(unit);
        info = &wp->partyInfo[unit->currentKind - TYPE_PARTNER_MIN];
        unit->mSizeMultiplier = info->sizeMultiplier;
        unit->mStatusFlags = info->statusFlags;
        unit->statusTurns = info->statusTurns;
#ifdef VERSION_G8ME01
        unit->usCommandDisabled[0] = info->usCommandDisabled[0];
        unit->usCommandDisabled[1] = info->usCommandDisabled[1];
#endif
        if (unit->currentHp <= 0) {
            BtlUnit_SetStatus(unit, STATUS_INSTANT_KILL, 1, 1);
        }
    }
}

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
    }
    else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        party = &pouch->partyData[BattleTransPartyId(unit->currentKind)];
        unit->baseMaxHp = party->baseMaxHP;
        unit->maxHp = party->baseMaxHP + unit->badgesEquipped.HpPlus * 5;
        unit->currentHp = party->currentHP;
        if (unit->currentHp > unit->maxHp) {
            unit->currentHp = unit->maxHp;
        }
    }
    else {
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

void BtlUnit_SetParamToPouch(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    BattleWorkPartyInfo* info;

    if (unit->currentKind == UNIT_MARIO) {
        pouch->currentHP = unit->currentHp;
        pouch->currentFP = unit->currentFp;
    }
    else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        s32 id = BattleTransPartyId(unit->currentKind);
        *(s16*)((u8*)pouch + id * sizeof(PouchPartyData) + offsetof(PouchPartyData, currentHP)) = unit->currentHp; // FAKE: byte-offset store so the field offset folds into the index (mulli/addi/sthx), as in the target
        info = &wp->partyInfo[unit->currentKind - TYPE_PARTNER_MIN];
        info->sizeMultiplier = unit->mSizeMultiplier;
        info->statusFlags = unit->mStatusFlags;
        info->statusTurns = unit->statusTurns;
#ifdef VERSION_G8ME01
        info->usCommandDisabled[0] = unit->usCommandDisabled[0];
        info->usCommandDisabled[1] = unit->usCommandDisabled[1];
#endif
    }
}

void BtlUnit_CheckPinchStatus(BattleUnit* unit, BOOL noAudience) {
    if (unit->currentHp <= unit->mKindParams->mPerilHP && unit->currentHp < unit->maxHp) {
        if (!(unit->mStatusFlags & 0x20000000)) {
            unit->mStatusFlags |= 0x20000000;
            if (!noAudience && unit->currentKind == UNIT_MARIO) {
                BattleAudience_Case_MarioDanger();
            }
        }
    }
    else if (unit->mStatusFlags & 0x20000000) {
        unit->mStatusFlags &= ~0x20000000;
    }
    if (unit->currentHp <= unit->mKindParams->mDangerHP && unit->currentHp < unit->maxHp) {
        if (!(unit->mStatusFlags & 0x10000000)) {
            unit->mStatusFlags |= 0x10000000;
            if (!(unit->mStatusFlags & 0x20000000) && !noAudience && unit->currentKind == UNIT_MARIO) {
                BattleAudience_Case_MarioPinch();
            }
        }
    }
    else if (unit->mStatusFlags & 0x10000000) {
        unit->mStatusFlags &= ~0x10000000;
    }
}

s32 BtlUnit_GetExp(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    PouchData* pouch = pouchGetPtr();
    s32 level = pouch->level;
    s32 unitLevel = unit->mLevel;
    FieldBattleInfo* fbat = wp->fbatInfo;
    f32 scale;

    if (level > unitLevel) {
        return 0;
    }
    switch (*(s32*)fbat->setup->groupSetup) {
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
        return (s32)(t / 2.0f) + unit->mKindParams->mBonusExp;
    }
}

s32 BtlUnit_GetCoin(BattleUnit* unit) {
    s32 coins, i;

    if (!unit) {
        return 0;
    }
    coins = unit->mKindParams->baseCoin;
    for (i = 0; i < unit->mKindParams->bonusCoin; i++) {
        if (irand(100) < unit->mKindParams->bonusCoinRate) {
            coins++;
        }
    }
    return coins;
}

s32 BtlUnit_GetFp(BattleUnit* unit) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s32 result = unit->currentFp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        result = BattleGetMarioPtr(wp)->currentFp;
    }
    return result;
}

void BtlUnit_SetFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        BattleGetMarioPtr(wp)->currentFp = value;
    }
    else {
        unit->currentFp = value;
    }
}

void BtlUnit_RecoverHp(BattleUnit* unit, s32 value) {
    unit->currentHp += value;
    if (unit->currentHp > unit->maxHp) {
        unit->currentHp = unit->maxHp;
    }
}

void BtlUnit_RecoverFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s16 max;
    s16* fp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        max = BtlUnit_GetMaxFp(BattleGetMarioPtr(wp));
        fp = &BattleGetMarioPtr(wp)->currentFp;
    }
    else {
        max = BtlUnit_GetMaxFp(unit);
        fp = &unit->currentFp;
    }
    *fp += value;
    if (*fp > max) {
        *fp = max;
    }
}

s32 BtlUnit_GetMaxFp(BattleUnit* unit) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    s32 result = unit->maxFp;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        result = BattleGetMarioPtr(wp)->maxFp;
    }
    return result;
}

void BtlUnit_SetMaxFp(BattleUnit* unit, s32 value) {
    BattleUnitType kind = unit->currentKind;
    BattleWork* wp = _battleWorkPointer;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        BattleGetMarioPtr(wp)->maxFp = value;
    }
    else {
        unit->maxFp = value;
    }
}

s32 BtlUnit_GetWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    s32 cost = (u8)weapon->baseFpCost;
    s32 count;

    if (weapon->itemId) {
        count = pouchEquipCheckBadge(weapon->itemId);
        switch (weapon->itemId) {
            case 0xF0:
            case 0xF1:
            case 0xF2:
            case 0xF3:
            case 0xF7:
            case 0xF8:
            case 0xF9:
            case 0xFC:
            case 0x100:
            case 0x101:
            case 0x14C:
            case 0x14D:
            case 0x14E:
            case 0x151:
            case 0x152:
                cost = cost * pow(2.0, count - 1);
                break;
            case 0xF4:
            case 0xF5:
            case 0xF6:
            case 0xFB:
            case 0xFD:
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
    if (cost >= 1000) {
        cost = 999;
    }
    return cost;
}

BOOL BtlUnit_CheckWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    s32 fp = BtlUnit_GetFp(unit);
    if (fp < BtlUnit_GetWeaponCost(unit, weapon)) {
        return FALSE;
    }
    return pouchGetAP() >= (u8)weapon->baseSpCost * 100;
}

void BtlUnit_PayWeaponCost(BattleUnit* unit, BattleWeapon* weapon) {
    BattleWork* wp = _battleWorkPointer;
    s32 fp = BtlUnit_GetFp(unit);
    s32 cost = BtlUnit_GetWeaponCost(unit, weapon);

    BtlUnit_SetFp(unit, fp - cost);
    if (unit->currentKind == UNIT_MARIO) {
        BtlActRec_AddPoint(&wp->actRecord.marioFpSpent, cost);
        BtlActRec_AddCount(&wp->actRecord.marioFpMoves);
    }
    else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        BtlActRec_AddPoint(&wp->actRecord.partnerFpSpent, cost);
        BtlActRec_AddCount(&wp->actRecord.partnerFpMoves);
    }
    ((void (*)(s32))pouchAddAP)(-(u8)weapon->baseSpCost * 100);
}

BattleUnit* BtlUnit_GetGuardKouraPtr(BattleUnit* unit) {
    BattleWork* wp = _battleWorkPointer;
    s32 i = 0;
    s32 id = unit->unitId;
    BattleUnit* other;

    for (; i < 64; i++) {
        other = BattleGetUnitPtr(wp, i);
        if (other && other->currentKind == UNIT_SHELL_SHIELD && other->work[0] == id) {
            break;
        }
    }
    if (i >= 64) {
        other = NULL;
    }
    return other;
}

void BtlUnit_ResetMoveStatus(BattleUnit* unit) {
    s32 n;

    unit->moveState = 0;
    unit->movesRemaining = unit->maxMoveCount;
    if (BtlUnit_CheckStatus(unit, STATUS_FAST)) {
        unit->movesRemaining *= 2;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
        if (unit->movesRemaining >= 2) {
            unit->movesRemaining /= 2;
        }
        else {
            n = 2;
            if (unit->maxMoveCount < 1) {
                n *= 2;
            }
            if (unit->activeTurns >= n) {
                unit->movesRemaining = 1;
                unit->moveState = 0;
            }
            else {
                unit->movesRemaining = 0;
                unit->moveState = 10;
            }
        }
    }
    unit->maxMovesThisTurn = unit->movesRemaining;
    if (!unit->maxMovesThisTurn) {
        unit->movesRemaining = 0;
        unit->moveState = 10;
    }
}

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

void BtlUnit_SetSeMode(BecSndWork* work, const char* name0, const char* name1, s16 c, s16 e, s16 unused, s16 a, s16 b) {
    work->name0 = name0;
    work->name1 = name1;
    work->delay = c;
    work->period0 = e;
    work->period1 = e;
    work->count = 0;
    work->reserved_08 = a;
    work->reserved_0A = b;
    work->sfxId = -1;
}

void BtlUnit_SetCommandAnimPose(BattleUnit* unit) {
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
    BOOL special = FALSE;

    if (!(unit->mStatusFlags & 0x30000000) && _GetStatusPoseType(unit) == 0x1C) {
        special = TRUE;
    }
    if (special) {
        BtlUnit_SetAnimType(part, 0x43);
        PART_FLAGS(part) |= 0x80;
    }
    else {
        btlDispPoseAnime(part);
    }
}

void BtlUnit_PoseSoundInit(BattleUnitPart* part) {
    PART_POSE_SOUND(part)->loopCount = 0;
    PART_POSE_SOUND(part)->time = 0.0f;
    PART_POSE_SOUND(part)->prevTime = 0.0f;
}

#pragma push
#pragma opt_propagation off
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
    state = PART_POSE_SOUND(part);
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
        if (BattleGetSeq(wp, SEQ_PHASE) == 0x4000002 || BattleGetSeq(wp, SEQ_PHASE) == 0x4000004) {
            state->flag = 1;
        }
    }
    else if (state->loopCount == (u32)pose->loopTime && state->time > time) {
        state->time = time;
        state->prevTime = -1.0f;
    }
    else if (state->loopCount < (u32)pose->loopTime) {
        state->time = time;
        state->prevTime = -1.0f;
        state->loopCount = (s32)pose->loopTime;
    }
    else {
        state->prevTime = state->time;
        state->time = time;
    }
    if (state->time == state->prevTime) {
        return;
    }
    anim = animPoseGetCurrentAnim(part->poseId);
    for (; entry->animName; entry++) {
        if (strcmp(entry->animName, (const char*)anim)) {
            continue;
        }
        if (entry->time <= state->time && entry->time > state->prevTime) {
            if (entry->flags & 1) {
                if (entry->callback) {
                    id = entry->callback(part->mOwner, entry, state);
                }
                else {
                    id = BtlUnit_snd_se(part->mOwner, entry->sfxName, EVT_NULLPTR, entry->pitch);
                }
                psndSFX_vol(id, vol = 0x7F);
                state->count++;
                if (part->mOwner->attackPhase != BattleGetSeq(wp, SEQ_PHASE)) {
                    f64 k;
                    dv = (f64)(u32)vol;
                    vol = dv * (k = 0.7);
                    psndSFX_vol(id, vol);
                }
            }
            else {
                if (entry->callback) {
                    entry->callback(part->mOwner, entry, state);
                }
                else {
                    BtlUnit_snd_se(part->mOwner, entry->sfxName, EVT_NULLPTR, entry->pitch);
                }
                state->count++;
            }
        }
    }
}
#pragma pop

s32 BtlUnit_snd_se(BattleUnit* unit, const char* sfxName, s32 volume, s32 pan) {
    Vec pos;
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    return ((s32 (*)(BattleUnit*, const char*, s32, s32, Vec*))BtlUnit_snd_se_pos)(unit, sfxName, volume, pan, &pos);
}

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

void BtlUnit_HpGaugeMain(BattleUnit* unit) {
    BattleWorkHpGauge* gauge = &unit->hpGauge;
    f32 t;
    s16 maxHp;

    if (gauge->drainTimer > 0) {
        gauge->drainTimer--;
        if (gauge->drainTimer <= 0) {
            gauge->prevHp = gauge->targetHp;
            gauge->barRatio = gauge->trailRatio = (f32)unit->currentHp / (f32)unit->maxHp;
        }
        else {
            switch (gauge->mode) {
                case 1:
                    gauge->barRatio = (f32)unit->currentHp / (f32)unit->maxHp;
                    gauge->trailRatio = gauge->barRatio + (f32)(gauge->prevHp - gauge->targetHp) * intplGetValue(2, gauge->drainTimer, gauge->drainLength, 0.0f, 1.0f) / (f32)unit->maxHp;
                    break;
                default:
                    gauge->trailRatio = 0.0f;
                    maxHp = unit->maxHp;
                    gauge->barRatio = (f32)(gauge->prevHp - gauge->targetHp) * intplGetValue(0, gauge->drainTimer, gauge->drainLength, 0.0f, 1.0f) / (f32)maxHp + (f32)unit->currentHp / (f32)maxHp;
                    break;
            }
        }
    }
    if (gauge->visibleTimer > 0) {
        gauge->visibleTimer--;
    }
    if ((unit->flags & 1) && gauge->visibleTimer <= 0 && (gauge->flags & 1)) {
        unit->flags &= ~1;
        gauge->flags &= ~1;
    }
}

BOOL BtlUnit_EnemyItemCanUseCheck(s32 item) {
    s32* table = _enemy_can_use_item_table;

    if (item >= 0xF0 && item < 0x153) {
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
    if (unit->mAttributeFlags & 0x10) {
        return TRUE;
    }
    if (unit->currentKind == UNIT_VIVIAN && unit->work[4] > 0) {
        return TRUE;
    }
    return FALSE;
}
