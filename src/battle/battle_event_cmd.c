#include "driver/itemdrv.h"
#include "battle/ac/ac_timing_a.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_status_effect.h"
#include "mario/mario_pouch.h"
#include "battle/battle_acrobat.h"
#include "battle/battle_disp.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "battle/battle_unit_event.h"
#include "eff/eff_nice.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include <stdlib.h>
#include "battle/battle.h"
#include "battle/battle_event_subset.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "battle/battle_stage.h"
#include "battle/battle_ac.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_damage.h"
#include "battle/battle_audience.h"
#include "battle/battle_seq.h"
#include "eff/eff_itemget.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include "battle/battle_ac_help.h"
#include "pmario_sound.h"
#include <math.h>
#include <string.h>
#include "data/item_data.h"
#include "driver/animdrv.h"
#include "eff/n64/eff_coin_n64.h"

//s32* eventId
USER_FUNC(btlevtcmd_WaitEventEnd) {
    s32 id;

    id = evtGetValue(event, *event->args);
    return evtCheckID(id) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

//s32 retIndex, s32 battleFlags
USER_FUNC(btlevtcmd_check_battleflag) {
    EvtWord* args = event->args;
    s32 index = *args++;
    BattleWork* wp = _battleWorkPointer;
    u32 flags = *args++;
    if (wp->flags & flags) {
        evtSetValue(event, index, TRUE);
    }
    else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

//BOOL* onoff, s32 flags
USER_FUNC(btlevtcmd_onoff_battleflag) {
    EvtWord* args = event->args;
    BOOL val = evtGetValue(event, args[0]);
    u32 flags = args[1];
    BattleWork* wp = _battleWorkPointer;
    if (val == TRUE) {
        wp->flags |= flags;
    }
    else {
        wp->flags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

//s32 retIndex
USER_FUNC(btlevtcmd_get_turn) {
    evtSetValue(event, *event->args, _battleWorkPointer->turnCount);
    return EVT_RETURN_DONE;
}

//no params
USER_FUNC(btlevtcmd_reset_turn) {
    _battleWorkPointer->flags |= 0x2000;
    return EVT_RETURN_DONE;
}

//no params
USER_FUNC(btlevtcmd_reset_move_color_lv_all) {
    btlsubResetMoveColorLvAll(_battleWorkPointer);
    return EVT_RETURN_DONE;
}

//s32 retIndex, s32 phaseSeqId
USER_FUNC(btlevtcmd_CheckPhase) {
    EvtWord* args = event->args;
    s32 index = args[0];
    u32 seq = args[1];
    if (seq != BattleGetSeq(_battleWorkPointer, SEQ_PHASE)) {
        evtSetValue(event, index, FALSE);
    }
    else {
        evtSetValue(event, index, TRUE);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetPos) {
    EvtWord* args = event->args;
    s32 btlId;
    s32 indexX, indexY, indexZ;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetPos(unit, &x, &y, &z);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexX, (s32)x);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexY, (s32)y);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexZ, (s32)z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetPosFloat) {
    EvtWord* args = event->args;
    s32 btlId;
    s32 indexX, indexY, indexZ;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetPos(unit, &x, &y, &z);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexX, x);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexY, y);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexZ, z);
    }
    return EVT_RETURN_DONE;
}

//s32* maxValue, s32 retIndex
USER_FUNC(btlevtcmd_GetRandomValue) {
    EvtWord* args = event->args;
    s32 max, index;

    max = evtGetValue(event, args[0]);
    index = args[1];
    evtSetValue(event, index, irand(max));
    return EVT_RETURN_DONE;
}

//s32 retIndex, s32 countIndex, s32 sizeIndex[]
USER_FUNC(btlevtcmd_DrawLots) {
    EvtWord* args = event->args;
    s32 index = args[0];
    s32 *alloc;
    s32 i, count, max;
    s32 ceiling;
    s32 nv[2];

    index = args[0];
    args += 2;
    count = evtGetValue(event, args[-1]);
    alloc = BattleAlloc(sizeof(s32) * count);
    max = 0;
    for (i = 0; i < count; i++) {
        alloc[i] = evtGetValue(event, *args++);
        max += alloc[i];
    }
    ceiling = irand(max);
    nv[1] = 0; i = 0; nv[0] = i;
    for (; i < count; i++) {
        nv[1] += alloc[i];
        if (ceiling < nv[1]) {
            break;
        }
        nv[0]++;
    }
    evtSetValue(event, index, nv[0]);
    BattleFree(alloc);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SpawnUnit) {
    EvtWord* args = event->args;
    BattleUnitSetup* setup;
    BattleUnit* unit;
    s32 index;

    index = args[0];
    setup = (BattleUnitSetup*)evtGetValue(event, args[1]);
    unit = BtlUnit_Spawn(setup, args[2]);
    if (!unit) {
        evtSetValue(event, index, -1);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, index, unit->unitId);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 flags
USER_FUNC(btlevtcmd_KillUnit) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    s32 id, flags;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    flags = args[1];
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    unit->currentHp = 0;
    BtlUnit_ClearStatus(unit);
    if (flags & 2) {
        unit->mAttributeFlags |= kInactive;
        return EVT_RETURN_DONE;
    }
    else {
        BtlUnit_SetStatus(unit, STATUS_INSTANT_KILL, 1, 1);
        if (!(flags & 1)) {
            unit->flags |= BATTLE_UNIT_FLAG_BROKEN;
        }
        return EVT_RETURN_DONE;
    }
}

//s32* battleId, s32* copyToPartId, BattleUnitKindPart* kind, s32* copyFromPartId
USER_FUNC(btlevtcmd_ReplaceParts) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    BattleUnit* unit;
    BattleUnitKindPart* kind;
    s32 id, transId, lookupPart, partNum;

    id = evtGetValue(event, args[0]);
    transId = BattleTransID(event, id);
    lookupPart = evtGetValue(event, args[1]);
    kind = (BattleUnitKindPart*)args[2];
    partNum = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, transId);
    part = BattleGetUnitPartsPtr(transId, lookupPart);
    if (kind) {
        while (kind->partNum != partNum) {
            ;
        }
        part->mKindPartParams = kind;
        part->partName = part->mKindPartParams->partName;
        part->attributes = part->mKindPartParams->attributes;
        part->counterAttributes = part->mKindPartParams->counterAttributes;
        part->defense = part->mKindPartParams->defense;
        part->defenseAttr = part->mKindPartParams->defenseAttr;
        part->mPositionOffset.x = part->mKindPartParams->positionOffset.x;
        part->mPositionOffset.y = part->mKindPartParams->positionOffset.y;
        part->mPositionOffset.z = part->mKindPartParams->positionOffset.z;
        BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotateOffset(part, 0.0f, 0.0f, 0.0f);
        part->mHitBaseOffset.x = part->mKindPartParams->hitBaseOffset.x;
        part->mHitBaseOffset.y = part->mKindPartParams->hitBaseOffset.y;
        part->mHitOffset.x = 0.0f;
        part->mHitOffset.y = 0.0f;
        part->hitOffsetX = 0;
    }
    btlDispEntAnime(unit);
    return EVT_RETURN_DONE;
}

//s32 retIndex
USER_FUNC(btlevtcmd_ChangeParty) {
    BattleUnit* unit;
    s32 index, value;

    value = -1;
    index = *event->args;
    unit = BattleChangeParty(_battleWorkPointer);
    if (unit) {
        value = unit->unitId;
    }
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to set current)
USER_FUNC(btlevtcmd_SetPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;
    s32 btlId;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    indexX = args[1];
    x = (f32)evtGetValue(event, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(event, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(event, indexZ);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_POINTER_MAX) {
        x = currX;
    }
    if (indexY == EVTDAT_POINTER_MAX) {
        y = currY;
    }
    if (indexZ == EVTDAT_POINTER_MAX) {
        z = currZ;
    }
    BtlUnit_SetPos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPos) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddPos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ (can be EVTDAT_POINTER_MAX for noret)
USER_FUNC(btlevtcmd_GetPartsPos) {
    EvtWord* args = event->args;
    s32 btlId;
    s32 id, partId;
    s32 indexX, indexY, indexZ;
    f32 currX, currY, currZ;
    BattleUnitPart* part;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetPartsWorldPos(part, &currX, &currY, &currZ);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexX, (s32)currX);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexY, (s32)currY);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexZ, (s32)currZ);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsPos) {
    EvtWord* args = event->args;
    s32 btlId;
    f32 x, y, z, currX, currY, currZ;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, partId;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    if (part->attributes & 0x10000000) {
        BtlUnit_SetPartsPos(part, x, y, z);
    }
    else {
        BtlUnit_GetPos(unit, &currX, &currY, &currZ);
        x -= currX;
        y -= currY;
        z -= currZ;
        BtlUnit_SetPartsOffsetPos(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsPos) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    if (part->attributes & 0x10000000) {
        BtlUnit_AddPartsPos(part, x, y, z);
    }
    else {
        BtlUnit_AddPartsOffsetPos(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetDispOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetDispOffset(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsDispOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    BattleUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id); //unused variable
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsDispOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    BattleUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id); //unused variable
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetTogeOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    if (x != (f32)EVTDAT_POINTER_MAX) {
        unit->togeOffset.x = x;
    }
    if (y != (f32)EVTDAT_POINTER_MAX) {
        unit->togeOffset.y = y;
    }
    if (z != (f32)EVTDAT_POINTER_MAX) {
        unit->togeOffset.z = z;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetHomePos) {
    EvtWord* args = event->args;
    s32 btlId;
    s32 indexX, indexY, indexZ;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetHomePos(unit, &x, &y, &z);
    evtSetValue(event, indexX, (s32)x);
    evtSetValue(event, indexY, (s32)y);
    evtSetValue(event, indexZ, (s32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetHomePos) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetHomePos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddHomePos) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddHomePos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsHomePos) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId;
    s32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsHomePos(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetHitPos) {
    EvtWord* args = event->args;
    f32 x, y, z;
    s32 id, partId;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 indexX, indexY, indexZ;
    s32 btlId;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetHitPos(unit, part, &x, &y, &z);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexX, (s32)x);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexY, (s32)y);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexZ, (s32)z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetHitOffset) {
    EvtWord* args = event->args;
    s32 partId;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id;
    s32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitOffset(unit, part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetHitCursorOffset) {
    EvtWord* args = event->args;
    s32 partId;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id;
    s32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitCursorOffset(unit, part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetWidth) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, index, width;

    id = evtGetValue(event, args[0]);
    index = args[1];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    width = BtlUnit_GetWidth(unit);
    evtSetValue(event, index, width);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetHeight) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, index, height;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    height = BtlUnit_GetHeight(unit);
    evtSetValue(event, index, height);
    return EVT_RETURN_DONE;
}

//s32* battleId, s16* height
USER_FUNC(btlevtcmd_SetHeight) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 height;
    s32 btlId, id;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    height = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_SetHeight(unit, (s16)height);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetStatusMg) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, index;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    evtSetFloat(event, index, unit->mSizeMultiplier);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store)
USER_FUNC(btlevtcmd_SetStatusIconOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, btlId, xpos, ypos;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    xpos = evtGetValue(event, args[1]);
    ypos = evtGetValue(event, args[2]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (xpos != EVTDAT_POINTER_MAX) {
        unit->mStatusIconOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_POINTER_MAX) {
        unit->mStatusIconOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store)
USER_FUNC(btlevtcmd_SetHpGaugeOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, btlId, xpos, ypos;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    xpos = evtGetValue(event, args[1]);
    ypos = evtGetValue(event, args[2]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (xpos != EVTDAT_POINTER_MAX) {
        unit->mHpGaugeOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_POINTER_MAX) {
        unit->mHpGaugeOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retX, s32 retY
USER_FUNC(btlevtcmd_GetHpGaugeOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, indexX, indexY;
    BattleUnit* unit;
    s32 x, y;

    btlId = evtGetValue(event, args[0]);
    indexX = args[1];
    indexY = args[2];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, btlId));
    x = unit->mHpGaugeOffset[0];
    y = unit->mHpGaugeOffset[1];
    if (x != EVT_NULLPTR) {
        evtSetValue(event, indexX, x);
    }
    if (y != EVT_NULLPTR) {
        evtSetValue(event, indexY, y);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, u8* alpha
USER_FUNC(btlevtcmd_SetAlpha) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, btlId, partId;
    s32 alpha;

    btlId = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);
    alpha = evtGetValue(event, args[2]);
    id = BattleTransID(event, btlId);
    part = BattleGetUnitPartsPtr(id, partId);
    part->color.a = alpha;
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, u8* redIndex, u8* greenIndex, u8* blueIndex
USER_FUNC(btlevtcmd_GetRGB) {
    EvtWord* args = event->args;
    s32 id, partId;
    BattleUnitPart* part;
    s32 indexR, indexG, indexB;

    id = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    part = BattleGetUnitPartsPtr(id, partId);
    indexR = args[2];
    indexG = args[3];
    args = (EvtWord*)args[4];
    evtSetValue(event, indexR, part->color.r);
    evtSetValue(event, indexG, part->color.g);
    evtSetValue(event, (EvtWord)args, part->color.b);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, u8* red, u8* green, u8* blue
USER_FUNC(btlevtcmd_SetRGB) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, btlId, partId;
    s32 r, g, b;

    btlId = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);
    r = evtGetValue(event, args[2]);
    g = evtGetValue(event, args[3]);
    b = evtGetValue(event, args[4]);
    id = BattleTransID(event, btlId);
    part = BattleGetUnitPartsPtr(id, partId);
    part->color.r = r;
    part->color.g = g;
    part->color.b = b;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_GetRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    f32 rotateX, rotateY, rotateZ;
    s32 indexX, indexY, indexZ;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetRotate(unit, &rotateX, &rotateY, &rotateZ);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexX, (s32)rotateX);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexY, (s32)rotateY);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetValue(event, indexZ, (s32)rotateZ);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetRotate) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;
    s32 btlId;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    indexX = args[1];
    x = (f32)evtGetValue(event, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(event, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(event, indexZ);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetRotate(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_POINTER_MAX) {
        x = currX;
    }
    if (indexY == EVTDAT_POINTER_MAX) {
        y = currY;
    }
    if (indexZ == EVTDAT_POINTER_MAX) {
        z = currZ;
    }
    BtlUnit_SetRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_GetPartsRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    s32 id, partId, indexX, indexY, indexZ;
    BattleUnitPart* part;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetPartsRotate(part, &x, &y, &z);
    evtSetValue(event, indexX, (s32)x);
    evtSetValue(event, indexY, (s32)y);
    evtSetValue(event, indexZ, (s32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId, x, y, z;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotate(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId, x, y, z;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotate(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetBaseRotate) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;
    s32 btlId;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    indexX = args[1];
    x = (f32)evtGetValue(event, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(event, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(event, indexZ);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetBaseRotate(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_POINTER_MAX) {
        x = currX;
    }
    if (indexY == EVTDAT_POINTER_MAX) {
        y = currY;
    }
    if (indexZ == EVTDAT_POINTER_MAX) {
        z = currZ;
    }
    BtlUnit_SetBaseRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetPartsBaseRotate) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    f32 currX, currY, currZ;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_GetPartsBaseRotate(part, &currX, &currY, &currZ);
    if (x != (f32)EVTDAT_POINTER_MAX) {
        currX = x;
    }
    if (y != (f32)EVTDAT_POINTER_MAX) {
        currY = y;
    }
    if (z != (f32)EVTDAT_POINTER_MAX) {
        currZ = z;
    }
    BtlUnit_SetPartsBaseRotate(part, currX, currY, currZ);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetRotateOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetRotateOffset(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsRotateOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId, x, y, z;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsRotateOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId, x, y, z;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId
USER_FUNC(btlevtcmd_SetRotateOffsetFromCenterOffset) {
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, *event->args);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetRotateOffset(unit, unit->offsetCenter.x, unit->offsetCenter.y, unit->offsetCenter.z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetCutBaseOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->cutBaseOffset.x = x;
    unit->cutBaseOffset.y = y;
    unit->cutBaseOffset.z = z;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetBintaHitOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->bintaHitOffset.x = x;
    unit->bintaHitOffset.y = y;
    unit->bintaHitOffset.z = z;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetKissHitOffset) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->kissHitOffset.x = x;
    unit->kissHitOffset.y = y;
    unit->kissHitOffset.z = z;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* value
USER_FUNC(btlevtcmd_SetCutWidth) {
    s32 btlId;
    s32 value;
    s32 id;
    BattleUnit* unit;
    EvtWord* args = event->args;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    value = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->cutWidth = value;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* value
USER_FUNC(btlevtcmd_SetCutHeight) {
    s32 btlId;
    s32 value;
    s32 id;
    BattleUnit* unit;
    EvtWord* args = event->args;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    value = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->cutHeight = value;
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetPossessionItemOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    s32 id, x, y, z, indexX, indexY, indexZ;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    indexX = args[1];
    x = evtGetValue(event, indexX);
    indexY = args[2];
    y = evtGetValue(event, indexY);
    indexZ = args[3];
    z = evtGetValue(event, indexZ);
    if (indexX != EVTDAT_POINTER_MAX) {
        unit->heldItemOffset.x = (f32)x;
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        unit->heldItemOffset.y = (f32)y;
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        unit->heldItemOffset.z = (f32)z;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetBaseScale) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    if (unit) {
        BtlUnit_SetBaseScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsBaseScale) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) return EVT_RETURN_DONE;
    {
        btlId = evtGetValue(event, args[1]);
        part = BattleGetUnitPartsPtr(id, btlId);
        x = evtGetFloat(event, args[2]);
        y = evtGetFloat(event, args[3]);
        z = evtGetFloat(event, args[4]);
        BtlUnit_SetPartsBaseScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_GetScale) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    s32 id, indexX, indexY, indexZ;
    BattleUnit* unit;
    f32 x, y, z;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetScale(unit, &x, &y, &z);
    if (indexX != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexX, x);
    }
    if (indexY != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexY, y);
    }
    if (indexZ != EVTDAT_POINTER_MAX) {
        evtSetFloat(event, indexZ, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetScale) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    if (id == -1) return EVT_RETURN_DONE;
    {
        unit = BattleGetUnitPtr(wp, id);
        x = evtGetFloat(event, args[1]);
        y = evtGetFloat(event, args[2]);
        z = evtGetFloat(event, args[3]);
        BtlUnit_SetScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddScale) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    BattleUnit* unit;
    f32 x, y, z;
    s32 id;

    btlId = evtGetValue(event, args[0]);

    id = BattleTransID(event, btlId);
    if (id == -1) return EVT_RETURN_DONE;
    {
        unit = BattleGetUnitPtr(wp, id);
        x = evtGetFloat(event, args[1]);
        y = evtGetFloat(event, args[2]);
        z = evtGetFloat(event, args[3]);
        BtlUnit_AddScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsScale) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    {
        partId = evtGetValue(event, args[1]);
        part = BattleGetUnitPartsPtr(id, partId);
        x = evtGetFloat(event, args[2]);
        y = evtGetFloat(event, args[3]);
        z = evtGetFloat(event, args[4]);
        BtlUnit_SetPartsScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsScale) {
    EvtWord* args = event->args;
    s32 btlId;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    {
        partId = evtGetValue(event, args[1]);
        part = BattleGetUnitPartsPtr(id, partId);
        x = evtGetFloat(event, args[2]);
        y = evtGetFloat(event, args[3]);
        z = evtGetFloat(event, args[4]);
        BtlUnit_AddPartsScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* retIndex
USER_FUNC(btlevtcmd_GetHp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->currentHp);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* retIndex
USER_FUNC(btlevtcmd_GetFp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, index;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    index = args[1];
    evtSetValue(event, index, BtlUnit_GetFp(unit));
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* retIndex
USER_FUNC(btlevtcmd_GetMaxHp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->maxHp);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetMaxFp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, index;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    index = args[1];
    evtSetValue(event, index, BtlUnit_GetMaxFp(unit));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetHp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    f32 ratio;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    unit->currentHp = (s16)evtGetValue(event, args[1]);
    unit->hpGauge.prevHp = unit->currentHp;
    unit->hpGauge.targetHp = unit->currentHp;
    ratio = (f32)unit->currentHp / (f32)unit->maxHp;
    unit->hpGauge.trailRatio = ratio;
    unit->hpGauge.barRatio = ratio;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetFp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    value = evtGetValue(event, args[1]);
    BtlUnit_SetFp(unit, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMaxFp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    value = evtGetValue(event, args[1]);
    BtlUnit_SetMaxFp(unit, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetSwallowParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;
    s32 value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    value = evtGetValue(event, args[1]);
    if (unit) {
        unit->swallowRate = value;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetSwallowAttribute) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;
    s32 value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    value = (u8)evtGetValue(event, args[1]);
    if (unit) {
        unit->swallowAttribute = value;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMaxMoveCount) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;
    s8 max;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    unit->maxMoveCount = (u8)evtGetValue(event, args[1]);
    if (unit->movesRemaining > (s8)unit->maxMoveCount) {
        unit->movesRemaining = unit->maxMoveCount;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDamagePartsId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->currentTarget->mKindPartParams->partNum);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetHpDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->hpDamageTaken);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetFpDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->fpDamageTaken);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetHpDamageCount) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, args[1], unit->hpDamageCount);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RecoverHp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, hp;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) { 
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtGetValue(event, args[1]); //unused, debug?
    hp = evtGetValue(event, args[2]);
    if (!(unit->mAttributeFlags & kImmuneToNegativeStatus)) {
        unit->hpGauge.prevHp = unit->currentHp;
        BtlUnit_RecoverHp(unit, hp);
        unit->hpGauge.targetHp = unit->currentHp;
        unit->hpGauge.drainTimer = unit->hpGauge.drainLength;
        unit->hpGauge.visibleTimer = unit->hpGauge.visibleLength;
        unit->hpGauge.flags |= 1;
    }
    else {
        BtlUnit_RecoverHp(unit, hp);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RecoverFp) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    if (id == -1) { 
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    evtGetValue(event, args[1]); //unused, debug?
    value = evtGetValue(event, args[2]);
    BtlUnit_RecoverFp(unit, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetOverTurnCount) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, index;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->flippedTurns);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetOverTurnCount) {
    s32 id;
    EvtWord* args = event->args;
    s32 btlId;
    s32 value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    value = evtGetValue(event, args[1]);
    { BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id); unit->flippedTurns = value; }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitWork) {
    EvtWord* args = event->args;
    s32 btlId, id, i, index;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    i = evtGetValue(event, args[1]);
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->work[i]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetUnitWork) {
    EvtWord* args = event->args;
    s32 btlId, id;
    EvtWord value;
    s32 index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->work[index] = value;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AddUnitWork) {
    EvtWord* args = event->args;
    s32 btlId, id, value, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->work[index] += value;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitWorkFloat) {
    EvtWord* args = event->args;
    s32 btlId, id, i, index;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    i = evtGetValue(event, args[1]);
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetFloat(event, index, *(f32*)&unit->work[i]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetUnitWorkFloat) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    f32 value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = evtGetValue(event, args[1]);
    value = evtGetFloat(event, args[2]);
    *(f32*)&BattleGetUnitPtr(_battleWorkPointer, id)->work[index] = value;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetPartsWork) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, i, index;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    i = evtGetValue(event, args[2]);
    index = args[3];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    evtSetValue(event, index, part->work[i]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsWork) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, index;
    EvtWord value;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    value = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(EvtWord*)((u8*)BtlUnit_GetPartsPtr(unit, partId)->work + index * sizeof(EvtWord)) = value; // FAKE: byte-stride form of ->work[index] (index scaled before the field offset is added, as in the target)
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDamage) {
    EvtWord* args = event->args;
    s32 btlId, id, index, value;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[1];
    if (unit) {
        evtSetValue(event, index, BtlUnit_GetHitDamage(unit));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetTotalDamage) {
    EvtWord* args = event->args;
    s32 btlId, id, index, value;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[1];
    if (unit) {
        evtSetValue(event, index, BtlUnit_GetTotalHitDamage(unit));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDamageCode) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, index;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, (s32)(u8)unit->damageCode);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDamageCode) {
    EvtWord* args = event->args;
    s32 btlId, id, mask, index; BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id); if (unit->damageCode & mask) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StartAvoid) {
    EvtWord* args = event->args;
    s32 btlId, id, damageCode;
    EffectEntry* entry;
    BattleUnit* unit;
    Vec pos;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    damageCode = args[1] | 0x100;
    if ((u8)damageCode == 0x26) {
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        if (pos.y < 85.0f) {
            pos.y += 50.0f;
        }
        else {
            pos.y += -50.0f;
        }
        pos.x -= 45.0f;
        entry = effNiceEntry(6, pos.x, pos.y, pos.z);
        ((NiceEffectWork*)entry->userdata)->overallScale = 0.75f;
    }
    unit->damagePattern = 0;
    BattleRunHitEvent(unit, damageCode);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RunHitEventDirect) {
    EvtWord* args = event->args;
    void* code;
    BattleUnit* unit;
    s32 btlId, id, damageCode;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    damageCode = args[1];
    code = (void*)evtGetValue(event, args[2]);
    unit->damagePattern = evtGetValue(event, args[3]);
    BattleRunHitEventDirect(unit, damageCode | 0x100, code);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDamagePattern) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    u32 pattern, dp;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    pattern = args[1];
    index = args[2];
    dp = BattleGetUnitPtr(_battleWorkPointer, id)->damagePattern;
    if (dp == pattern) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckSpace) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 boundX, boundY, boundZ;
    s32 baseX, baseY, baseZ;
    BattleUnit* unit;
    s32 index, i;
    f32 x, y, z;
    s32 ix, iy, iz;
    BOOL found;

    index = args[0];
    baseX = evtGetValue(event, args[1]);
    baseY = evtGetValue(event, args[2]);
    baseZ = evtGetValue(event, args[3]);
    boundX = evtGetValue(event, args[4]);
    boundY = evtGetValue(event, args[5]);
    boundZ = evtGetValue(event, args[6]);
    for (i = 0; i < 64; ++i) {
        found = FALSE;
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->mAlliance == ALLIANCE_ENEMY) {
            BtlUnit_GetHomePos(unit, &x, &y, &z);
            ix = x;
            iy = y;
            iz = z;
            if (boundX) {
                if (abs(ix - baseX) < boundX) {
                    found = TRUE;
                }
            }
            if (boundY) {
                if (abs(iy - baseY) < boundY) {
                    found = TRUE;
                }
            }
            if (boundZ) {
                if (abs(iz - baseZ) < boundZ) {
                    found = TRUE;
                }
            }
            if (found) {
                evtSetValue(event, index, i);
                return EVT_RETURN_DONE;
            }
        }
    }
    evtSetValue(event, index, -1);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetEnemyBelong) {
    EvtWord* args = event->args;
    s32 btlId, id, index, value;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    value = BtlUnit_GetEnemyBelong(unit);
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetFriendBelong) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->mAlliance);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetProtectId) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->protectId);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitKind) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->currentKind);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetPartnerId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit, * partner;
    s32 btlId, id, index;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(wp, id);
    partner = BattleGetPartnerPtr(wp, unit);
    if (partner) {
        evtSetValue(event, index, partner->unitId);
    }
    else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetMarioId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, alliance;
    BattleUnit* unit;
    int i;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    alliance = BattleGetUnitPtr(wp, id)->mAlliance;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->mAlliance == alliance && unit->currentKind == UNIT_MARIO) {
            break;
        }
    }
    if (i >= 64) {
        evtSetValue(event, index, -1);
    }
    else {
        evtSetValue(event, index, unit->unitId);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetPartyId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, alliance, kind;
    BattleUnit* unit;
    int i;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    alliance = BattleGetUnitPtr(wp, id)->mAlliance;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->mAlliance == alliance) {
            kind = unit->currentKind;
            if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
                break;
            }
        }
    }
    if (i >= 64) {
        evtSetValue(event, index, -1);
    }
    else {
        evtSetValue(event, index, unit->unitId);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetBodyId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, value;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(wp, id);
    value = BtlUnit_GetBodyPartsId(unit);
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(wp, id);
    if (unit && (unit->flags & BATTLE_UNIT_FLAG_BROKEN)) {
        unit = NULL;
    }
    if (unit) {
        evtSetValue(event, index, id);
    }
    else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetPartyTechLv) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, value, i;
    BattleUnitType kind;
    PouchPartyData* party;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    kind = BattleGetUnitPtr(wp, id)->currentKind;
    value = 0;
    if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        i = BattleTransPartyId(kind);
        party = &pouchGetPtr()->partyData[i];
        value = party->techLevel;
    }
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CalculateFaceDirection) {
    f32 thisX, thisY, thisZ;
    f32 targetX, targetY, targetZ;
    f32 offsetX, offsetY, offsetZ;
    f32 localX, localY, localZ;
    f32 thisOffsetX, thisOffsetY, thisOffsetZ;
    f32 calculate;
    s32 tid;
    s32 targetPartId;
    BattleUnit* thisUnit;
    EvtWord* args = event->args;
    s32 value;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId;
    s32 index;
    s32 targetId;
    s32 thisPartId;
    u32 flags;
    s32 thisId;
    BattleUnit* targetUnit;
    BattleUnitPart* thisPart = NULL;
    BattleUnitPart* targetPart = NULL;

    btlId = evtGetValue(event, args[0]);
    thisId = BattleTransID(event, btlId);
    thisPartId = evtGetValue(event, args[1]);
    tid = targetId = evtGetValue(event, args[2]);
    targetPartId = evtGetValue(event, args[3]);
    flags = args[4];
    index = args[5];

    if (!(flags & 9)) {
        tid = BattleTransID(event, tid);
    }

    thisUnit = BattleGetUnitPtr(_battleWorkPointer, thisId);
    if (thisPartId != -1) {
        thisPart = BtlUnit_GetPartsPtr(thisUnit, thisPartId);
    }

    if (flags & 0x10) {
        targetUnit = BattleGetUnitPtr(_battleWorkPointer, tid);
        if (targetPartId != -1) {
            targetPart = BtlUnit_GetPartsPtr(targetUnit, targetPartId);
        }
    }

    value = thisUnit->faceDirection;
    if (thisPart) {
        value = thisPart->faceDirection;
    }

    if (flags & 4) {
        value = wp->alliances[thisUnit->mAlliance].attackDirection;
    }

    if (flags & 8) {
        value = thisUnit->moveDirection;
        if (thisPart) {
            value = thisPart->moveDirection;
        }
    }

    if (flags & 0x10) {
        BtlUnit_GetPos(thisUnit, &thisX, &thisY, &thisZ);
        if (thisPart) {
            if (thisPart->attributes & 0x10000000) {
                BtlUnit_GetPartsPos(thisPart, &thisX, &thisY, &thisZ);
            }
            else {
                BtlUnit_GetPartsOffsetPos(thisPart, &offsetX, &offsetY, &offsetZ);
                thisX += offsetX;
            }
        }

        BtlUnit_GetPos(targetUnit, &targetX, &targetY, &targetZ);
        if (targetPart) {
            if (targetPart->attributes & 0x10000000) {
                BtlUnit_GetPartsPos(targetPart, &targetX, &targetY, &targetZ);
            }
            else {
                BtlUnit_GetPartsOffsetPos(targetPart, &offsetX, &offsetY, &offsetZ);
                targetX += offsetX;
            }
        }

        calculate = targetX - thisX;
        if (calculate > 0.0f) {
            value = 1;
        }
        else if (calculate < 0.0f) {
            value = -1;
        }
    }

    if (flags & 1) {
        BtlUnit_GetPos(thisUnit, &localX, &localY, &localZ);
        if (thisPart) {
            if (thisPart->attributes & 0x10000000) {
                BtlUnit_GetPartsPos(thisPart, &localX, &localY, &localZ);
            }
            else {
                BtlUnit_GetPartsOffsetPos(thisPart, &thisOffsetX, &thisOffsetY, &thisOffsetZ);
                localX += thisOffsetX;
            }
        }
        calculate = (f32)targetId - localX;
        if (calculate > 0.0f) {
            value = 1;
        }
        else if (calculate < 0.0f) {
            value = -1;
        }
    }
    if (flags & 0x100) {
        value *= -1;
    }
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetFaceDirection) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->faceDirection);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangeFaceDirection) {
    s32 id;
    EvtWord* args = event->args;
    s32 btlId;
    s32 value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    value = evtGetValue(event, args[1]);
    { BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id); unit->faceDirection = value; }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangePartsFaceDirection) {
    EvtWord* args = event->args;
    s32 btlId, id, partId;
    s32 value;
    BattleUnitPart* part;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    part = BattleGetUnitPartsPtr(id, partId);
    part->faceDirection = value;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ResetFaceDirection) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id;

    btlId = evtGetValue(event, *event->args);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, btlId));
    unit->faceDirection = wp->alliances[unit->mAlliance].attackDirection;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, mask, index; BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id); if (unit->mAttributeFlags & mask) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnAttribute) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, mask;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->mAttributeFlags |= mask;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffAttribute) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, mask;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->mAttributeFlags = unit->mAttributeFlags & ~mask;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckPartsAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, index, mask;
    BattleUnit* unit;
    BattleUnitPart* part;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    index = args[3];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    if (part->attributes & mask) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnPartsAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, mask;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    part->attributes |= mask;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffPartsAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, mask;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    part->attributes = ~mask & part->attributes;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, index, mask;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    index = args[3];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    if (part->counterAttributes & mask) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, mask;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    part->counterAttributes |= mask;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, partId, mask;
    BattleUnitPart* part;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    mask = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    part->counterAttributes = part->counterAttributes & ~mask;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsBlur) {
    EvtWord* args = event->args;
    s32 btlId, id, partId;
    u8 color0, color1, color2, color3, color4, color5, color6, color7;
    BattleUnit* unit;
    BattleUnitPart* part;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    color0 = (u8)evtGetValue(event, args[2]);
    color1 = (u8)evtGetValue(event, args[3]);
    color2 = (u8)evtGetValue(event, args[4]);
    color3 = (u8)evtGetValue(event, args[5]);
    color4 = (u8)evtGetValue(event, args[6]);
    color5 = (u8)evtGetValue(event, args[7]);
    color6 = (u8)evtGetValue(event, args[8]);
    color7 = (u8)evtGetValue(event, args[9]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partId);
    part->blurWork.color[0].r = color0;
    part->blurWork.color[0].g = color1;
    part->blurWork.color[0].b = color2;
    part->blurWork.color[0].a = color3;
    part->blurWork.color[1].r = color4;
    part->blurWork.color[1].g = color5;
    part->blurWork.color[1].b = color6;
    part->blurWork.color[1].a = color7;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    { BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1]; unit->mTokenFlags |= m; }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    {
        { BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1]; unit->mTokenFlags &= ~m; }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, index;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[2];
    { BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1];
    if (unit->mTokenFlags & m) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetRegistStatus) {
    EvtWord* args = event->args;
    BattleVulnerableStatus* status;
    s32 btlId, id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    status = (BattleVulnerableStatus*)evtGetValue(event, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id)->vulnStatus = status;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckCommandUnit) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnitType kind;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    kind = BattleGetUnitPtr(_battleWorkPointer, id)->currentKind;
    index = args[1];
    if (kind == UNIT_MARIO || kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
        evtSetValue(event, index, 1);
    }
    else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangeKind) {
    s32 btlId, id, kind;
    EvtWord* args = event->args;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    BattleGetUnitPtr(_battleWorkPointer, id)->currentKind = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsDefenceTable) {
    EvtWord* args = event->args;
    s32 btlId, id, partId;
    EvtWord table;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    table = evtGetValue(event, args[2]);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), partId)->defense = (BattleUnitDefense*)table;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsDefenceAttrTable) {
    EvtWord* args = event->args;
    s32 btlId, id, partId;
    EvtWord table;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    partId = evtGetValue(event, args[1]);
    table = evtGetValue(event, args[2]);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), partId)->defenseAttr = (BattleUnitDefenseAttr*)table;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangeDataTable) {
    EvtWord* args = event->args;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    args = (EvtWord*)args[1];
    BattleGetUnitPtr(_battleWorkPointer, id)->dataTable = (BattleDataEntry*)args;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetBackItem) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    NpcBattleInfo* info;
    ItemType item;
    s32 btlId, i;

    btlId = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, btlId));
    if ((unit->flags & BATTLE_UNIT_FLAG_STOLEN_ITEM) && (item = unit->heldItem) != ITEM_NULL) {
        info = wp->fbatInfo->setup;
        for (i = 0; i < 8; i++) {
            if (info->recoveredItems[i] == 0) {
                break;
            }
        }
        info->recoveredItems[i] = item;
        unit->heldItem = ITEM_NULL;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetExp) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index, val;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    val = BtlUnit_GetExp(unit);
    evtSetValue(event, index, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StoreExp) {
    s32 val = evtGetValue(event, event->args[0]);
    BattleStoreExp(_battleWorkPointer, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetCoin) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index, val;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    val = BtlUnit_GetCoin(unit);
    evtSetValue(event, index, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StoreCoin) {
    s32 val = evtGetValue(event, event->args[0]);
    BattleStoreCoin(_battleWorkPointer, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMoveSpeed) {
    EvtWord* args = event->args;
    s32 btlId, id;
    f32 speed;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    speed = evtGetFloat(event, args[1]);
    BtlUnit_SetMoveSpeed(BattleGetUnitPtr(_battleWorkPointer, id), speed);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetJumpSpeed) {
    EvtWord* args = event->args;
    s32 btlId, id;
    f32 speed;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    speed = evtGetFloat(event, args[1]);
    BtlUnit_SetJumpSpeed(BattleGetUnitPtr(_battleWorkPointer, id), speed);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetFallAccel) {
    EvtWord* args = event->args;
    s32 btlId, id;
    f32 accel;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    accel = evtGetFloat(event, args[1]);
    BtlUnit_SetFallAccel(BattleGetUnitPtr(_battleWorkPointer, id), accel);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpSetting) {
    s32 btlId, id, frames;
    EvtWord* args = event->args;
    f32 speed, accel;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    frames = evtGetValue(event, args[1]);
    speed = evtGetFloat(event, args[2]);
    accel = evtGetFloat(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->moveFrames = frames;
    unit->mMoveSpeedXZ = speed;
    unit->mFallAccel = accel;
    return EVT_RETURN_DONE;
}

/* movers begin (half_B) */
f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z);
void _UnitMoveSoundControl(BattleUnit* unit);
void _UnitMoveSoundControlEnd(BattleUnit* unit);

static inline Vec becMvVecA(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline Vec becMvVecB(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline Vec becMvVecC(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline Vec becMvVecD(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

static inline Vec becMvVecE(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

USER_FUNC(btlevtcmd_JumpPosition) {
    s32 id;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 kind, frames;
    f32 tx, ty, tz;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    kind = evtGetValue(event, args[5]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        if (frames) {
            unit->moveFrames = frames;
        }
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        unit->mMoveDirection = angleABf(x, z, tx, tz);
        unit->mMoveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        y = ty - y;
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
            unit->mMoveDistance = unit->mMoveDistance - unit->moveFrames * unit->mMoveSpeedXZ;
        }
        else {
            unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
            unit->mMoveDistance = unit->mMoveDistance - unit->moveFrames * unit->mMoveSpeedXZ;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->mMoveSpeedY = y / unit->moveFrames + (unit->mFallAccel * unit->moveFrames) / 2.0f;
        unit->mMoveSpeedXZ = unit->mMoveSpeedXZ + unit->mMoveDistance / unit->moveFrames;
        BtlUnit_LoadSeMode(2, kind, &unit->moveSndTable, &unit->moveSnd);
        if ((EvtWord)unit->moveSnd.name0) {
            Vec pos = becMvVecA(unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
            psndSFXOn_3D(unit->moveSnd.name0, &pos);
        }
    }
    unit->mMoveCurrentPos.y += unit->mMoveSpeedY;
    unit->mMoveSpeedY -= unit->mFallAccel;
    if (unit->mMoveSpeedY < 0.0f && unit->mMoveCurrentPos.y < unit->mMoveTargetPos.y) {
        unit->mMoveCurrentPos.y = unit->mMoveTargetPos.y;
    }
    btlMovePos(unit->mMoveSpeedXZ, unit->mMoveDirection, &unit->mMoveCurrentPos.x, &unit->mMoveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        if ((EvtWord)unit->moveSnd.name1) {
            Vec pos = becMvVecB(unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
            psndSFXOn_3D(unit->moveSnd.name1, &pos);
        }
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_JumpContinue) {
    BattleUnit* unit;
    s32 id;
    f32 x, y, z;
    f32 h;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
    }
    unit->mMoveCurrentPos.y += unit->mMoveSpeedY;
    unit->mMoveSpeedY -= unit->mFallAccel;
    btlMovePos(unit->mMoveSpeedXZ, unit->mMoveDirection, &unit->mMoveCurrentPos.x, &unit->mMoveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    h = BattleGetFloorHeight(_battleWorkPointer, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    if (unit->mMoveCurrentPos.y < h) {
        unit->mMoveCurrentPos.y = h;
        BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_FallPosition) {
    s32 id, frames;
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 tx, ty, tz;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        unit->moveFrames = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        unit->mMoveDirection = angleABf(x, z, tx, tz);
        unit->mMoveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        y = ty - y;
        if (unit->moveFrames == 0) {
            if (unit->mMoveSpeedXZ == 0.0f) {
                unit->mMoveSpeedXZ = 1.0f;
            }
            unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
            if (unit->moveFrames == 0) {
                unit->moveFrames = 1;
            }
        }
        else {
            unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
        }
        unit->mMoveSpeedY = 0.0f;
        unit->mFallAccel = (y / unit->moveFrames - unit->mMoveSpeedY) / (-unit->moveFrames / 2.0f);
    }
    unit->mMoveCurrentPos.y += unit->mMoveSpeedY;
    unit->mMoveSpeedY -= unit->mFallAccel;
    btlMovePos(unit->mMoveSpeedXZ, unit->mMoveDirection, &unit->mMoveCurrentPos.x, &unit->mMoveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_MovePosition) {
    s32 id;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 kind, mode, frames;
    f32 tx, ty, tz;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    kind = evtGetValue(event, args[5]);
    mode = evtGetValue(event, args[6]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        unit->moveFrames = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        unit->mMoveDirection = angleABf(x, z, tx, tz);
        unit->mMoveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
            if (unit->moveFrames == 0) {
                unit->moveFrames = 1;
            }
            unit->mMoveSpeedXZ = unit->mMoveSpeedXZ + (unit->mMoveDistance - unit->moveFrames * unit->mMoveSpeedXZ) / unit->moveFrames;
        }
        else {
            unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
        }
        BtlUnit_LoadSeMode(mode, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    btlMovePos(unit->mMoveSpeedXZ, unit->mMoveDirection, &unit->mMoveCurrentPos.x, &unit->mMoveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        _UnitMoveSoundControlEnd(unit);
        return EVT_RETURN_YIELD;
    }
    _UnitMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_GetMoveFrame) {
    s32 id;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 frame, index;
    f32 tx, tz, speed, dist;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    speed = evtGetFloat(event, args[4]);
    index = args[5];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    dist = distABf(x, z, tx, tz);
    if (!speed) {
        speed = unit->mMoveSpeedXZ;
        if (!speed) {
            speed = 1.0f;
        }
    }
    frame = dist / speed;
    if (dist - frame * speed > 0.0f) {
        frame++;
    }
    if (frame == 0) {
        frame = 1;
    }
    if (index != EVT_NULLPTR) {
        evtSetValue(event, index, frame);
    }
    return EVT_RETURN_DONE;
}

f32 intpl_sub(s32 type, f32 start, f32 end, s32 cur, s32 max);

USER_FUNC(btlevtcmd_DivePosition) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, frames, height, mode, floor, type, kind;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 h, a, sx, sz, ox, oz;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    height = evtGetValue(event, args[5]);
    mode = evtGetValue(event, args[6]);
    type = evtGetValue(event, args[7]);
    kind = evtGetValue(event, args[8]);
    floor = 0;
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (mode >= 100) {
        floor = 1;
        mode -= 100;
    }
    if (isFirstCall) {
        event->blocked = 1;
        unit->moveFrames = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveStartPos(unit, x, y, z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        {
            f32 dx = x - tx, dy = y - ty, dz = z - tz;
            unit->mMoveDistance = sqrtf(dx * dx + dy * dy + dz * dz);
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
        }
        else {
            unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->mMoveTotalFrames = 0.0f;
        unit->mMoveDirection = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        BtlUnit_LoadSeMode(3, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    unit->mMoveCurrentPos.x = intpl_sub(mode, unit->mMoveStartPos.x, unit->mMoveTargetPos.x, (s32)unit->mMoveTotalFrames, unit->moveFrames);
    unit->mMoveCurrentPos.y = intpl_sub(mode, unit->mMoveStartPos.y, unit->mMoveTargetPos.y, (s32)unit->mMoveTotalFrames, unit->moveFrames);
    unit->mMoveCurrentPos.z = intpl_sub(mode, unit->mMoveStartPos.z, unit->mMoveTargetPos.z, (s32)unit->mMoveTotalFrames, unit->moveFrames);
    if (floor) {
        f32 fy = BattleGetFloorHeight(_battleWorkPointer, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
        if (unit->mMoveCurrentPos.y < fy) {
            unit->mMoveTotalFrames = (f32)unit->moveFrames;
            BtlUnit_SetMoveTargetPos(unit, unit->mMoveCurrentPos.x, fy, unit->mMoveCurrentPos.z);
        }
    }
    unit->mMoveTotalFrames += 1.0f;
    if (unit->mMoveTotalFrames > unit->moveFrames) {
        BtlUnit_SetPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        _UnitMoveSoundControlEnd(unit);
        return EVT_RETURN_DONE;
    }
    if (type == 0) {
        f32 dx = unit->mMoveTargetPos.x - unit->mMoveCurrentPos.x, dy = unit->mMoveTargetPos.y - unit->mMoveCurrentPos.y, dz = unit->mMoveTargetPos.z - unit->mMoveCurrentPos.z;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->mMoveDistance == 0.0f) {
            unit->mMoveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->mMoveDistance));
        if (height == 0) {
            h = 0.0f;
        }
        if (height < 0) {
            h = -h * (f32)(-height);
        }
        if (height > 0) {
            h = h * (f32)height;
        }
        BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y + h, unit->mMoveCurrentPos.z);
    }
    else {
        f32 dx = unit->mMoveTargetPos.x - unit->mMoveCurrentPos.x, dy = unit->mMoveTargetPos.y - unit->mMoveCurrentPos.y, dz = unit->mMoveTargetPos.z - unit->mMoveCurrentPos.z;
        s32 neg;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (unit->mMoveTargetPos.x - unit->mMoveStartPos.x > 0.0f) {
            neg = 1;
        }
        else if (unit->mMoveTargetPos.x - unit->mMoveStartPos.x < 0.0f) {
            neg = 0;
        }
        else {
            if (unit->mMoveTargetPos.y - unit->mMoveStartPos.y < 0.0f) {
                neg = 0;
            }
            else {
                neg = 1;
            }
        }
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->mMoveDistance == 0.0f) {
            unit->mMoveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->mMoveDistance));
        if (neg) {
            a = 360.0f * atan2f_safety(unit->mMoveTargetPos.x - unit->mMoveStartPos.x, unit->mMoveTargetPos.y - unit->mMoveStartPos.y) / 6.2831855f;
        }
        else {
            a = 180.0f + 360.0f * atan2f_safety(unit->mMoveTargetPos.x - unit->mMoveStartPos.x, unit->mMoveTargetPos.y - unit->mMoveStartPos.y) / 6.2831855f;
        }
        sx = h * sinfd(a);
        sz = h * cosfd(180.0f + a);
        if (height == 0) {
            sx = 0.0f;
            sz = sx;
        }
        else {
            sx = sx * (f32)height;
            sz = sz * (f32)height;
        }
        BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x + sz, unit->mMoveCurrentPos.y + sx, unit->mMoveCurrentPos.z);
    }
    _UnitMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_EllipseMovePosition) {
    s32 width, mode;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 frames, id;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 cx, cz, r, w, ang, t;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    width = evtGetValue(event, args[5]);
    mode = evtGetValue(event, args[6]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        unit->moveFrames = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveStartPos(unit, x, y, z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        {
            f32 dx = x - tx, dy = y - ty, dz = z - tz;
            unit->mMoveDistance = sqrtf(dx * dx + dy * dy + dz * dz);
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
        }
        else {
            unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->mMoveTotalFrames = 0.0f;
        unit->mMoveDirection = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
    }
    {
        f32 dx = unit->mMoveTargetPos.x - unit->mMoveStartPos.x, dz = unit->mMoveTargetPos.z - unit->mMoveStartPos.z;
        cx = (unit->mMoveStartPos.x + unit->mMoveTargetPos.x) / 2.0f;
        cz = (unit->mMoveStartPos.z + unit->mMoveTargetPos.z) / 2.0f;
        r = 0.5f * sqrtf(dx * dx + dz * dz);
    }
    w = (f32)width;
    ang = angleABf(cx, cz, unit->mMoveStartPos.x, unit->mMoveStartPos.z);
    t = intpl_sub(mode, 180.0f, 0.0f, (s32)unit->mMoveTotalFrames, unit->moveFrames);
    unit->mMoveCurrentPos.x = cx - (r * sinfd(ang) * cosfd(t) + w * cosfd(ang) * sinfd(t));
    unit->mMoveCurrentPos.z = cz + r * cosfd(ang) * cosfd(t) - w * sinfd(ang) * sinfd(t);
    BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
    unit->mMoveTotalFrames += 1.0f;
    if (unit->mMoveTotalFrames > unit->moveFrames) {
        BtlUnit_SetPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
/* movers end (half_B) */

USER_FUNC(btlevtcmd_MoveDirectionAdd) {
    s32 btlId, id, value;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 index, dir;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    value = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(wp, id);
    if (unit->moveDirection) {
        dir = value * unit->moveDirection;
    } else {
        dir = value * unit->faceDirection;
    }
    value = evtGetValue(event, index);
    value += dir;
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_FaceDirectionAdd) {
    s32 btlId, id, value;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 index, dir;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    value = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(wp, id);
    dir = value * unit->faceDirection;
    value = evtGetValue(event, index);
    value += dir;
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_FaceDirectionSub) {
    s32 btlId, id;
    BattleWork* wp = _battleWorkPointer;
    s32 index, value, dir;
    EvtWord* args = event->args;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    value = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(wp, id);
    dir = value * unit->faceDirection;
    evtSetValue(event, index, evtGetValue(event, index) - dir);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PartsFaceDirectionSub) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId, id, partId, index, value, dir;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    index = args[2];
    value = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(wp, id);
    dir = value * BtlUnit_GetPartsPtr(unit, partId)->faceDirection;
    evtSetValue(event, index, evtGetValue(event, index) - dir);
    return EVT_RETURN_DONE;
}

/* half_A begin PRE */
/* first-half (target order 1..153) functions, generated block */
s32 BattleEnemyUseItemCheck(BattleUnit* unit);
void BtlUnit_PayWeaponCost(BattleUnit* unit, BattleWeapon* weapon);
BattleWeapon* BattleGetSelectWeapon(BattleWork* wp);
extern ItemData itemDataTable[];
void btlDispChangePeraAnime(BattleUnitPart* part, s32 anim);
void BtlUnit_PoseSoundInit(BattleUnitPart* part);
void btlDispPoseAnime(BattleUnitPart* part);
void BtlUnit_SetAnim(BattleUnitPart* part, const char* name);
void BattleAttackDeclareAll(BattleWork* wp);
void BattleGetFirstAttackUnit(BattleWork* wp, s32* a, BattleUnit** unit);
void BattleChoiceSamplingEnemy(BattleWorkWeaponTargets* targets, s32 a, s32* b, s32* c);
s32 BtlUnit_snd_se_pos(BattleUnit* unit, const char* sfxName, s32 volume, s32 pitch, Vec* pos);
void _MoveSoundControlEndCore(BattleUnit* unit, BecSndWork* work);
void _MoveSoundControlCore(BattleUnit* unit, BecSndWork* work, BattleUnitPart* part, f32 x, f32 y, f32 z);
void* BattleSetConfuseAct(BattleWork* wp, BattleUnit* unit);
void BattleSamplingEnemy(BattleWorkWeaponTargets* targets, BattleWeapon* weapon, s32 unitId, s8 belong, u32 classFlags, u32 propFlags, s8 direction);
void BattleDamageDirect(s32 unitId, BattleUnit* target, BattleUnitPart* part, s32 damage, s32 fpDamage, u32 result, u32 pattern, s32 flag);
s32 BattlePreCheckDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags);
void* itemEntry(const char* name, s32 itemId, s32 type, s32 collectFlag, s32 evtCode, f32 x, f32 y, f32 z);
static inline void bec_coin(BattleUnit* unit, s32 item) {
    s32 coins;
    Vec pos;
    if (unit->badgesEquipped.Refund) {
        coins = (s32)((f32)(unit->badgesEquipped.Refund * 5 + 70) * (f32)itemDataTable[item].sell_price / 100.0f);
        if (coins > 0) {
            pouchAddCoin(coins);
            BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
            ((void (*)(s32, s32, f32, f32, f32, f32))effCoinN64Entry)(0, 0, pos.x, pos.y, pos.z, 1.0f);
        }
    }
}

USER_FUNC(btlevtcmd_SamplingEnemy) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleWorkWeaponTargets* targets = &wp->weaponTargets;
    s32 id;
    s8 belong;
    BattleWeapon* weapon;
    s8 dir;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    belong = evtGetValue(event, args[1]);
    weapon = (BattleWeapon*)evtGetValue(event, args[2]);
    dir = -wp->alliances[belong].attackDirection;
    if (weapon->targetPropertyFlags & TARGET_PROP_SAME_SIDE_FIRST) {
        dir *= -1;
    }
    BattleSamplingEnemy(targets, weapon, id, belong, weapon->targetClassFlags, weapon->targetPropertyFlags, dir);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChoiceSamplingEnemy) {
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &_battleWorkPointer->weaponTargets;
    u8* weapon;
    s32 a, b, ra, rb;

    weapon = (u8*)evtGetValue(event, args[0]);
    a = args[1];
    b = args[2];
    BattleChoiceSamplingEnemy(targets, (s32)((BattleWeapon*)weapon)->targetWeightFlags, &ra, &rb);
    evtSetValue(event, a, ra);
    evtSetValue(event, b, rb);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetSelectEnemy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &wp->weaponTargets;
    s32 a = args[0];
    s32 b = args[1];
    BattleWorkTarget* t;

    if (wp->weaponTargets.numTargets <= 0) {
        evtSetValue(event, a, -1);
        evtSetValue(event, b, -1);
        return EVT_RETURN_DONE;
    }
    t = &targets->targets[targets->targetIndices[targets->currentTarget]];
    evtSetValue(event, a, t->unitIdx);
    evtSetValue(event, b, t->partIdx);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetSelectNextEnemy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &wp->weaponTargets;
    s32 a = args[0];
    s32 b = args[1];
    BattleWorkTarget* t;

    targets->currentTarget = wp->weaponTargets.currentTarget + 1;
    if (targets->currentTarget < targets->numTargets) {
        t = &targets->targets[targets->targetIndices[targets->currentTarget]];
        evtSetValue(event, a, t->unitIdx);
        evtSetValue(event, b, t->partIdx);
    } else {
        evtSetValue(event, a, -1);
        evtSetValue(event, b, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetFirstAttackTarget) {
    EvtWord* args = event->args;
    s32 part;
    BattleUnit* unit;
    s32 a = args[0];
    s32 b = args[1];
    s32 body;

    BattleGetFirstAttackUnit(_battleWorkPointer, &part, &unit);
    body = BtlUnit_GetBodyPartsId(unit);
    evtSetValue(event, a, *(s32*)unit);
    evtSetValue(event, b, body);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetConsumeItem) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleCommand* cmd = &wp->commandMenu;
    evtSetValue(event, *args++, cmd->weapon[cmd->cursor[11].absPos].itemId);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_DispItemIcon) {
    EvtWord* args = event->args;
    s32 x, y, z;
    s32 item;
    s32 time;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    item = evtGetValue(event, args[3]);
    time = evtGetValue(event, args[4]);
    if (isFirstCall) {
        event->userdata[0] = time;
        event->userdatap[1] = itemEntry(NULL, item, 0x11, -1, 0, x, y, z);
        return EVT_RETURN_BLOCK;
    }
    if (--event->userdata[0] > 0) {
        return EVT_RETURN_BLOCK;
    }
    itemDelete(((ItemEntry*)event->userdatap[1])->name);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandFlyItem) {
    EvtWord* args = event->args;
    s32 id, time;
    BattleCommand* cmd;
    BattleUnit* unit;
    s32 kind;
    s32 item;
    BattleWork* wp;
    Vec pos;

    id = evtGetValue(event, args[0]);
    time = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    if (isFirstCall) {
        wp = _battleWorkPointer;
        cmd = &wp->commandMenu;
        unit = BattleGetUnitPtr(wp, id);
        kind = unit->currentKind;
        if (kind == UNIT_MARIO || (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX)) {
            item = cmd->weapon[cmd->cursor[11].absPos].itemId;
        } else {
            item = unit->heldItem;
        }
        event->userdata[0] = time;
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        pos.y += 10.0f;
        if (!(unit->mAttributeFlags & kVeiled)) {
            if (unit->mAttributeFlags & kCeiling) {
                pos.y -= 5.0f;
            } else {
                pos.y += unit->mSizeMultiplier * BtlUnit_GetHeight(unit);
            }
        }
        event->userdatap[1] = itemEntry(NULL, item, 0x11, -1, 0, pos.x, pos.y, pos.z);
        effItemGetEntry(10, pos.x, 12.0f + pos.y, pos.z - 5.0f);
        if (kind == UNIT_MARIO) {
            psndSFXOn_3D("SFX_ITEM_MARIO_USE1", &pos);
        } else if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
            psndSFXOn_3D("SFX_ITEM_PARTY_USE1", &pos);
        } else {
            psndSFXOn_3D("SFX_ENM_ITEM_USE1", &pos);
        }
        return EVT_RETURN_BLOCK;
    }
    if (--event->userdata[0] > 0) {
        return EVT_RETURN_BLOCK;
    }
    itemDelete(((ItemEntry*)event->userdatap[1])->name);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ConsumeItem) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleCommandWeapon* ci;
    s32 id;
    s32 item;
    s32 idx;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
        BattleCommand* cmd = &wp->commandMenu;
        item = cmd->weapon[cmd->cursor[11].absPos].itemId;
        idx = cmd->weapon[cmd->cursor[11].absPos].index;
        if (itemDataTable[item].usable_locations & 1) {
            pouchRemoveItemIndex(item, idx);
        }
        bec_coin(unit, item);
    } else {
        unit->heldItem = ITEM_NULL;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ConsumeItemReserve) {
    s32 id;
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 item;
    s32 i;
    s32 coins;
    Vec pos;

    id = evtGetValue(event, args[0]);
    item = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
        if (itemDataTable[item].usable_locations & 1) {
            for (i = 0; i < 4; i++) {
                if ((s32)wp->reserveItems[i] == item) {
                    break;
                }
                if ((s32)wp->reserveItems[i] == 0) {
                    wp->reserveItems[i] = item;
                    break;
                }
            }
        }
        if (unit->badgesEquipped.Refund) {
            coins = (s32)((f32)(unit->badgesEquipped.Refund * 5 + 70) * (f32)itemDataTable[item].sell_price / 100.0f);
            if (coins > 0) {
                pouchAddCoin(coins);
                BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
                ((void (*)(s32, s32, f32, f32, f32, f32))effCoinN64Entry)(0, 0, pos.x, pos.y, pos.z, 1.0f);
            }
        }
    } else {
        if (unit->heldItem == ITEM_LIFE_SHROOM) {
            unit->heldItem = ITEM_NULL;
        }
        goto end;
    end:;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AttackDeclare) {
    EvtWord* args = event->args;
    s32 id;
    s32 a, b;
    s32 unitIdx, partIdx;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    a = args[1];
    b = args[2];
    unitIdx = evtGetValue(event, a);
    unitIdx = BattleTransID(event, unitIdx);
    partIdx = evtGetValue(event, b);
    BattleAttackDeclare(&unitIdx, &partIdx);
    evtSetValue(event, a, unitIdx);
    evtSetValue(event, b, partIdx);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AttackDeclareAll) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    BattleAttackDeclareAll(wp);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PreCheckDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 res;
    s32 out;
    s32 flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    weapon = (BattleWeapon*)evtGetValue(event, args[3]);
    flags = args[4];
    args = (EvtWord*)args[5];
    res = BattlePreCheckDamage(attacker, target, part, weapon, flags);
    if (res < 0) {
        return EVT_RETURN_FINISH;
    }
    evtSetValue(event, (EvtWord)args, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandPreCheckDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 out;
    s32 flags;
    s32 res;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    flags = args[3];
    out = args[4];
    weapon = BattleGetSelectWeapon(wp);
    res = BattlePreCheckDamage(attacker, target, part, weapon, flags);
    if (res < 0) {
        return EVT_RETURN_FINISH;
    }
    evtSetValue(event, out, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PreCheckCounter) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 out;
    s32 res;
    CounterWork work;
    u32 flags = 0;

    BattleInitCounterPreCheckWork(&work);
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    weapon = (BattleWeapon*)evtGetValue(event, args[3]);
    out = args[5];
    res = BattleCheckCounter(&work, attacker, target, part, weapon, &flags);
    evtSetValue(event, out, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandPreCheckCounter) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 out;
    s32 res;
    CounterWork work;
    u32 flags = 0;

    BattleInitCounterPreCheckWork(&work);
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    out = args[4];
    weapon = BattleGetSelectWeapon(wp);
    res = BattleCheckCounter(&work, attacker, target, part, weapon, &flags);
    evtSetValue(event, out, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 flags;
    s32 res;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    weapon = (BattleWeapon*)evtGetValue(event, args[3]);
    flags = args[4];
    args = (EvtWord*)args[5];
    res = BattleCheckDamage(attacker, target, part, weapon, flags);
    if (res < 0) {
        return EVT_RETURN_FINISH;
    }
    evtSetValue(event, (EvtWord)args, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandCheckDamage) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* attacker;
    BattleUnit* target;
    BattleUnitPart* part;
    BattleWeapon* weapon;
    s32 id;
    s32 id2;
    s32 pno;
    s32 out;
    s32 flags;
    s32 res;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    attacker = BattleGetUnitPtr(wp, id);
    id2 = evtGetValue(event, args[1]);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    pno = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, pno);
    flags = args[3];
    out = args[4];
    weapon = BattleGetSelectWeapon(wp);
    res = BattleCheckDamage(attacker, target, part, weapon, flags);
    if (res < 0) {
        return EVT_RETURN_FINISH;
    }
    evtSetValue(event, out, res);
    if (!evtCheckID(event->eventId)) {
        return EVT_RETURN_FINISH;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_DamageDirect) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id;
    s32 damage;
    s32 result;
    s32 pattern;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    pattern = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(unit, pattern);
    damage = evtGetValue(event, args[2]);
    result = args[3];
    pattern = evtGetValue(event, args[4]);
    BattleDamageDirect(-5, unit, part, damage, 0, result, pattern, evtGetValue(event, args[5]));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventDamage) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->damageEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventWait) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->waitEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventEntry) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->entryEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventPhase) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->phaseEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventUnisonPhase) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->unisonPhaseEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventAttack) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->attackEventCode = (void*)args[1];
    unit->attackEventFlags = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventConfusion) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->confusionEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventCeilFall) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->ceilFallEventCode = (void*)args[1];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StartWaitEvent) {
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit->waitEventCode) {
        BattleRunWaitEvent(unit);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StopWaitEvent) {
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit->waitEventId) {
        evtDeleteID(unit->waitEventId);
        unit->waitEventId = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GuardMoveEnd) {
    _battleWorkPointer->flags &= ~1;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WaitGuardMove) {
    return (_battleWorkPointer->flags & 1) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WaitAttackEnd) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, _battleWorkPointer->activeUnitId);
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    if (!unit->attackEventId) {
        return EVT_RETURN_DONE;
    }
    if (evtCheckID(unit->attackEventId)) {
        return EVT_RETURN_BLOCK;
    }
    unit->attackEventId = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WaitAttackEndCheck) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    s32 out = args[0];

    evtSetValue(event, out, 1);
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    if (!unit->attackEventId) {
        return EVT_RETURN_DONE;
    }
    if (evtCheckID(unit->attackEventId)) {
        evtSetValue(event, out, 0);
    } else {
        unit->attackEventId = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RunDataEventChild) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, type;
    void* evt;
    EventEntry* child;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    type = evtGetValue(event, args[1]);
    evt = BtlUnit_GetData(unit, type);
    if (!evt) {
        return EVT_RETURN_DONE;
    }
    child = evtChildEntry(event, evt, 0);
    event->opcode = 0;
    child->unitId = id;
    return 0xFF;
}

USER_FUNC(btlevtcmd_CheckDataOfDataTable) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;
    s32 index, type;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[1];
    type = evtGetValue(event, args[2]);
    evtSetValue(event, index, BtlUnit_CheckData(unit, type));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDataFromDataTable) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;
    s32 index, type;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[1];
    type = evtGetValue(event, args[2]);
    evtSetValue(event, index, (EvtWord)BtlUnit_GetData(unit, type));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PhaseEventStartDeclare) {
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    BattlePhaseEventStartDeclare(BattleGetUnitPtr(_battleWorkPointer, id));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetConfuseActEvent) {
    EvtWord* args = event->args;
    s32 id;
    s32 out;
    BattleUnit* unit;
    s32 i = 0;
    void* evt;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    out = args[1];
    evt = NULL;
    while (!evt) {
        evt = BattleSetConfuseAct(_battleWorkPointer, unit);
        i++;
        if (i > 20) {
            evt = subsetevt_confuse_flustered;
        }
    }
    evtSetValue(event, out, (EvtWord)evt);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_MarioJumpParam) {
    EvtWord* args = event->args;
    s32 id, frames, n, out;
    f32 tx, tz, dist;
    f32 x, y, z;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    tz = evtGetValue(event, args[3]);
    out = args[4];
    frames = evtGetValue(event, out);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    dist = distABf(x, z, tx, tz);
    if (frames != 0) {
        n = frames;
    } else {
        n = dist / unit->mMoveSpeedXZ;
    }
    if (dist > 60.0f) {
        n += (s32)(0.15f * (dist - 60.0f));
    }
    if (frames == 0 || n > frames) {
        frames = n;
    }
    if (frames != EVT_NULLPTR) {
        evtSetValue(event, out, frames);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_MarioJumpPosition) {
    BattleUnit* unit;
    s32 id, frames, type, kind;
    EvtWord* args;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 rem, sn, t;

    args = event->args;
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    type = evtGetValue(event, args[5]);
    kind = evtGetValue(event, args[6]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        event->blocked = 1;
        if (type == 1) {
            event->blocked = 2;
        }
        else {
            BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
            BtlUnit_GetPos(unit, &x, &y, &z);
            BtlUnit_SetMoveCurrentPos(unit, x, y, z);
            unit->mMoveDirection = angleABf(x, z, tx, tz);
            unit->mMoveDistance = distABf(x, z, tx, tz);
            if (tx - x > 0.0f) {
                unit->moveDirection = 1;
            }
            else if (tx - x < 0.0f) {
                unit->moveDirection = -1;
            }
            unit->moveFrames = frames;
            if (unit->moveFrames == 0) {
                unit->moveFrames = unit->mMoveDistance / unit->mMoveSpeedXZ;
                rem = unit->mMoveDistance - unit->moveFrames * unit->mMoveSpeedXZ;
            }
            else {
                unit->mMoveSpeedXZ = unit->mMoveDistance / unit->moveFrames;
                rem = unit->mMoveDistance - unit->moveFrames * unit->mMoveSpeedXZ;
            }
            if (unit->moveFrames == 0) {
                return EVT_RETURN_DONE;
            }
            unit->mJumpStep.x = (unit->mMoveTargetPos.x - unit->mMoveCurrentPos.x) / unit->moveFrames;
            unit->mJumpStep.y = (unit->mMoveTargetPos.y - unit->mMoveCurrentPos.y) / unit->moveFrames;
            unit->mJumpStep.z = (unit->mMoveTargetPos.z - unit->mMoveCurrentPos.z) / unit->moveFrames;
            unit->mMoveSpeedY = 0.0f;
            unit->mFallAccel = 3.14159265f / unit->moveFrames;
            unit->mMoveSpeedXZ = unit->mMoveSpeedXZ + rem / unit->moveFrames;
            if (type < 3) {
                unit->mJumpAngle = 90.0f;
                unit->mJumpAngleStep = 360.0f / unit->moveFrames;
                t = unit->mMoveDistance - 20.0;
                t = t / 6.0;
                t += 47.0;
                unit->mMoveTotalFrames = t;
                if (type == 2) {
                    unit->mMoveTotalFrames *= 1.12; t = unit->mMoveTotalFrames;
                    unit->mMoveTotalFrames = t;
                }
                unit->mJumpBaseY = 0.0f;
                unit->mJumpBaseY = 0.0f;
                sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
                unit->mMoveSpeedY = unit->mMoveSpeedY + unit->mFallAccel * (0.53 * sn + 1.0);
            }
            else {
                unit->mJumpAngle = 90.0f;
                unit->mJumpAngleStep = (f32)(360 / unit->moveFrames);
                t = unit->mMoveDistance - 20.0;
                t = t / 6.0;
                t += 47.0;
                unit->mMoveTotalFrames = t;
                if (type == 4) {
                    unit->mMoveTotalFrames *= 1.25; t = unit->mMoveTotalFrames;
                    unit->mMoveTotalFrames = t;
                }
                unit->mJumpBaseY = 0.0f;
                unit->mJumpBaseY = 0.0f;
                sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
                unit->mMoveSpeedY = unit->mMoveSpeedY + unit->mFallAccel * (0.8 * sn + 1.0);
            }
            BtlUnit_LoadSeMode(2, kind, &unit->moveSndTable, &unit->moveSnd);
            if ((EvtWord)unit->moveSnd.name0) {
                Vec pos = becMvVecC(unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
                psndSFXOn_3D(unit->moveSnd.name0, &pos);
            }
        }
    }
    switch ((s8)event->blocked) {
    case 1:
        unit->mMoveCurrentPos.x += unit->mJumpStep.x;
        unit->mMoveCurrentPos.y += unit->mJumpStep.y;
        unit->mMoveCurrentPos.z += unit->mJumpStep.z;
        BtlUnit_GetPos(unit, &x, &y, &z);
        unit->mJumpBaseY = y;
        x = unit->mMoveCurrentPos.x;
        sn = sin(unit->mMoveSpeedY);
        y = unit->mMoveTotalFrames * sn + unit->mMoveCurrentPos.y;
        z = unit->mMoveCurrentPos.z;
        BtlUnit_SetPos(unit, x, y, z);
        if (unit->mMoveTargetPos.y > y && unit->moveFrames < 3) {
            y = unit->mMoveTargetPos.y;
            BtlUnit_SetPos(unit, x, y, z);
        }
        unit->mJumpBaseY = y;
        if (type < 3) {
            sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
                unit->mMoveSpeedY = unit->mMoveSpeedY + unit->mFallAccel * (0.53 * sn + 1.0);
        }
        else {
            sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
                unit->mMoveSpeedY = unit->mMoveSpeedY + unit->mFallAccel * (0.8 * sn + 1.0);
        }
        unit->mJumpAngle = unit->mJumpAngle + unit->mJumpAngleStep;
        unit->mJumpAngle = reviseAngle(unit->mJumpAngle);
        unit->moveFrames--;
        if (unit->moveFrames == 0) {
            y = unit->mMoveTargetPos.y;
            BtlUnit_SetPos(unit, x, y, z);
            unit->mMoveSpeedY = -(unit->mJumpBaseY - unit->mJumpBaseY);
            unit->mFallAccel = 1.8f;
            return EVT_RETURN_YIELD;
        }
        break;
    case 2:
        if (!BtlUnit_GetACPossibility(unit)) {
            return EVT_RETURN_DONE;
        }
        unit->mMoveSpeedY = -(unit->mJumpBaseY - unit->mJumpBaseY);
        unit->mFallAccel = 1.8f;
        unit->moveFrames = 1;
        unit->mJumpAngle = 90.0f;
        unit->mJumpAngleStep = (f32)(360 / unit->moveFrames);
        unit->mMoveTotalFrames = __fabs(unit->mJumpBaseY - unit->mJumpBaseY) / 16.5;
        BtlUnit_GetPos(unit, &unit->mMoveCurrentPos.x, &unit->mMoveCurrentPos.y, &unit->mMoveCurrentPos.z);
        event->blocked = 3;
        break;
    case 3:
        sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
        unit->mMoveCurrentPos.x = unit->mMoveCurrentPos.x + unit->mMoveTotalFrames * sn / 33.0;
        sn = sin(6.28318531f * unit->mJumpAngle / 360.0f);
        unit->mMoveCurrentPos.y = unit->mMoveCurrentPos.y - unit->mMoveTotalFrames * sn;
        unit->mJumpAngle = unit->mJumpAngle + unit->mJumpAngleStep;
        unit->mJumpAngle = reviseAngle(unit->mJumpAngle);
        BtlUnit_SetPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
        unit->moveFrames--;
        if (unit->moveFrames == 0) {
            return EVT_RETURN_YIELD;
        }
        break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_GetTakeoffPosition) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleUnit* target;
    s32 id;
    s32 id2;
    s32 ox;
    s32 belong;
    s32 oy;
    s32 pno;
    s32 back;
    s32 oz;
    BattleUnitPart* part;
    BattleWorkWeaponTargets targets;
    f32 hx, hy, hz, y, x, tx;
    s32 cid, cpno;

    id = evtGetValue(event, args[0]);
    id2 = evtGetValue(event, args[1]);
    pno = evtGetValue(event, args[2]);
    ox = args[3];
    oy = args[4];
    oz = args[5];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &y);
    id2 = BattleTransID(event, id2);
    target = BattleGetUnitPtr(wp, id2);
    part = BtlUnit_GetPartsPtr(target, pno);
    BtlUnit_GetHomePos(unit, &hx, &hy, &hz);
    BtlUnit_GetHitPos(target, part, &hx, &y, &hz);
    belong = (s8)BtlUnit_GetEnemyBelong(unit);
    back = ((s32 (*)(BattleWorkWeaponTargets*, BattleWeapon*, s32, s8, u32, u32, s8))BattleSamplingEnemy)(
        &targets, NULL, id, belong, 0x1001240, 0x1000000, -wp->alliances[belong].attackDirection);
    BattleChoiceSamplingEnemy(&targets, 4, &cid, &cpno);
    if (targets.numTargets > 0) {
        BattleUnit* u2;
        BattleUnitPart* p2;
        cid = BattleTransID(event, cid);
        u2 = BattleGetUnitPtr(wp, cid);
        p2 = BtlUnit_GetPartsPtr(u2, cpno);
        BtlUnit_GetHitPos(u2, p2, &tx, &y, &y);
        if (__fabs(x - hx) > __fabs(x - tx)) {
            hx = tx;
        }
        if (part == p2) {
            if (back) hx += wp->alliances[belong].attackDirection * -60;
            else hx += wp->alliances[belong].attackDirection * 60;
        }
        else {
            if (back) hx += wp->alliances[belong].attackDirection * -30;
            else hx += wp->alliances[belong].attackDirection * 30;
        }
    }
    else {
        hx += wp->alliances[belong].attackDirection * 60;
    }
    evtSetValue(event, ox, (s32)hx);
    evtSetValue(event, oy, (s32)hy);
    evtSetValue(event, oz, (s32)hz);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsMoveSpeed) {
    EvtWord* args = event->args;
    s32 id, pno;
    f32 speed;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    speed = evtGetFloat(event, args[2]);
    BtlUnit_SetPartsMoveSpeed(BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno), speed);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsFallAccel) {
    EvtWord* args = event->args;
    s32 id, pno;
    f32 accel;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    accel = evtGetFloat(event, args[2]);
    BtlUnit_SetPartsFallAccel(BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno), accel);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpPartsPosition) {
    EvtWord* args = event->args;
    s32 id, pno;
    BattleUnitPart* part;
    s32 frames, kind;
    f32 tx, ty, tz;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    tx = (f32)evtGetValue(event, args[2]);
    ty = (f32)evtGetValue(event, args[3]);
    tz = (f32)evtGetValue(event, args[4]);
    frames = evtGetValue(event, args[5]);
    kind = evtGetValue(event, args[6]);
    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    if (isFirstCall) {
        event->blocked = 1;
        if (frames) {
            part->moveTotalFrames = frames;
        }
        BtlUnit_SetPartsMoveTargetPos(part, tx, ty, tz);
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
        part->moveAngle = angleABf(x, z, tx, tz);
        part->moveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            part->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            part->moveDirection = -1;
        }
        y = ty - y;
        if (part->moveTotalFrames == 0) {
            part->moveTotalFrames = part->moveDistance / part->mMoveSpeedXZ;
            part->moveDistance = part->moveDistance - part->moveTotalFrames * part->mMoveSpeedXZ;
        }
        else {
            part->mMoveSpeedXZ = part->moveDistance / part->moveTotalFrames;
            part->moveDistance = part->moveDistance - part->moveTotalFrames * part->mMoveSpeedXZ;
        }
        if (part->moveTotalFrames == 0) {
            return EVT_RETURN_DONE;
        }
        part->moveSpeedY = y / part->moveTotalFrames + (part->mFallAccel * part->moveTotalFrames) / 2.0f;
        part->mMoveSpeedXZ = part->mMoveSpeedXZ + part->moveDistance / part->moveTotalFrames;
        BtlUnit_LoadSeMode(2, kind, &part->moveSndTable, &part->moveSnd);
        if ((EvtWord)part->moveSnd.name0) {
            Vec pos = becMvVecD(part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
            psndSFXOn_3D(part->moveSnd.name0, &pos);
        }
    }
    part->mMoveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->mFallAccel;
    if (part->moveSpeedY < 0.0f && part->mMoveCurrentPos.y < part->mMoveTargetPos.y) {
        part->mMoveCurrentPos.y = part->mMoveTargetPos.y;
    }
    btlMovePos(part->mMoveSpeedXZ, part->moveAngle, &part->mMoveCurrentPos.x, &part->mMoveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
    part->moveTotalFrames--;
    if (part->moveTotalFrames <= 0) {
        BtlUnit_SetPartsPos(part, part->mMoveTargetPos.x, part->mMoveTargetPos.y, part->mMoveTargetPos.z);
        if ((EvtWord)part->moveSnd.name1) {
            Vec pos = becMvVecE(part->mMoveTargetPos.x, part->mMoveTargetPos.y, part->mMoveTargetPos.z);
            psndSFXOn_3D(part->moveSnd.name1, &pos);
        }
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_JumpPartsSetting) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 a;
    s32 id;
    s32 pno;
    f32 f0, f1;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    a = evtGetValue(event, args[2]);
    f0 = evtGetFloat(event, args[3]);
    f1 = evtGetFloat(event, args[4]);
    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    part->moveTotalFrames = a;
    part->mMoveSpeedXZ = f0;
    part->mFallAccel = f1;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpPartsContinue) {
    EvtWord* args = event->args;
    s32 id, pno;
    BattleUnitPart* part;
    f32 h;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    if (isFirstCall) {
        f32 x, y, z;
        event->blocked = 1;
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
    }
    part->mMoveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->mFallAccel;
    btlMovePos(part->mMoveSpeedXZ, part->moveAngle, &part->mMoveCurrentPos.x, &part->mMoveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
    h = BattleGetFloorHeight(_battleWorkPointer, part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
    if (part->mMoveCurrentPos.y < h) {
        part->mMoveCurrentPos.y = h;
        BtlUnit_SetPartsPos(part, part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

void _PartsMoveSoundControl(BattleUnitPart* part);
void _PartsMoveSoundControlEnd(BattleUnitPart* part);

USER_FUNC(btlevtcmd_FallPartsPosition) {
    s32 id;
    EvtWord* args = event->args;
    s32 pno, frames;
    BattleUnitPart* part;
    f32 tx, ty, tz;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    tx = (f32)evtGetValue(event, args[2]);
    ty = (f32)evtGetValue(event, args[3]);
    tz = (f32)evtGetValue(event, args[4]);
    frames = evtGetValue(event, args[5]);
    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    if (isFirstCall) {
        event->blocked = 1;
        part->moveTotalFrames = frames;
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
        BtlUnit_SetPartsMoveTargetPos(part, tx, ty, tz);
        part->moveAngle = angleABf(x, z, tx, tz);
        part->moveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            part->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            part->moveDirection = -1;
        }
        y = ty - y;
        if (part->moveTotalFrames == 0) {
            if (part->mMoveSpeedXZ == 0.0f) {
                part->mMoveSpeedXZ = 1.0f;
            }
            part->moveTotalFrames = part->moveDistance / part->mMoveSpeedXZ;
            if (part->moveTotalFrames == 0) {
                part->moveTotalFrames = 1;
            }
        }
        else {
            part->mMoveSpeedXZ = part->moveDistance / part->moveTotalFrames;
        }
        part->moveSpeedY = 0.0f;
        part->mFallAccel = (y / part->moveTotalFrames - part->moveSpeedY) / (-part->moveTotalFrames / 2.0f);
    }
    part->mMoveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->mFallAccel;
    btlMovePos(part->mMoveSpeedXZ, part->moveAngle, &part->mMoveCurrentPos.x, &part->mMoveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->mMoveCurrentPos.x, part->mMoveCurrentPos.y, part->mMoveCurrentPos.z);
    part->moveTotalFrames--;
    if (part->moveTotalFrames <= 0) {
        BtlUnit_SetPartsPos(part, part->mMoveTargetPos.x, part->mMoveTargetPos.y, part->mMoveTargetPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_DivePartsPosition) {
    EvtWord* args = event->args;
    s32 id, pno, frames, height, mode, type, kind;
    BattleUnitPart* unit;
    s32 floor;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 h, a, sx, sz, ox, oz;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    tx = (f32)evtGetValue(event, args[2]);
    ty = (f32)evtGetValue(event, args[3]);
    tz = (f32)evtGetValue(event, args[4]);
    frames = evtGetValue(event, args[5]);
    height = evtGetValue(event, args[6]);
    mode = evtGetValue(event, args[7]);
    type = evtGetValue(event, args[8]);
    kind = evtGetValue(event, args[9]);
    floor = 0;
    unit = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    if (mode >= 100) {
        floor = 1;
        mode -= 100;
    }
    if (isFirstCall) {
        event->blocked = 1;
        unit->moveTotalFrames = frames;
        BtlUnit_GetPartsPos(unit, &x, &y, &z);
        BtlUnit_SetPartsMoveStartPos(unit, x, y, z);
        BtlUnit_SetPartsMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetPartsMoveTargetPos(unit, tx, ty, tz);
        {
            f32 dx = x - tx, dy = y - ty, dz = z - tz;
            unit->moveDistance = sqrtf(dx * dx + dy * dy + dz * dz);
        }
        if (unit->moveTotalFrames == 0) {
            unit->moveTotalFrames = unit->moveDistance / unit->mMoveSpeedXZ;
        }
        else {
            unit->mMoveSpeedXZ = unit->moveDistance / unit->moveTotalFrames;
        }
        if (unit->moveTotalFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->moveFrame = 0.0f;
        unit->moveAngle = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        }
        else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        BtlUnit_LoadSeMode(3, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    unit->mMoveCurrentPos.x = intpl_sub(mode, unit->mMoveStartPos.x, unit->mMoveTargetPos.x, (s32)unit->moveFrame, unit->moveTotalFrames);
    unit->mMoveCurrentPos.y = intpl_sub(mode, unit->mMoveStartPos.y, unit->mMoveTargetPos.y, (s32)unit->moveFrame, unit->moveTotalFrames);
    unit->mMoveCurrentPos.z = intpl_sub(mode, unit->mMoveStartPos.z, unit->mMoveTargetPos.z, (s32)unit->moveFrame, unit->moveTotalFrames);
    if (floor) {
        f32 fy = BattleGetFloorHeight(_battleWorkPointer, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y, unit->mMoveCurrentPos.z);
        if (unit->mMoveCurrentPos.y < fy) {
            unit->moveFrame = (f32)unit->moveTotalFrames;
            BtlUnit_SetPartsMoveTargetPos(unit, unit->mMoveCurrentPos.x, fy, unit->mMoveCurrentPos.z);
        }
    }
    unit->moveFrame += 1.0f;
    if (unit->moveFrame > unit->moveTotalFrames) {
        BtlUnit_SetPartsPos(unit, unit->mMoveTargetPos.x, unit->mMoveTargetPos.y, unit->mMoveTargetPos.z);
        _PartsMoveSoundControlEnd(unit);
        return EVT_RETURN_DONE;
    }
    if (type == 0) {
        f32 dx = unit->mMoveTargetPos.x - unit->mMoveCurrentPos.x, dy = unit->mMoveTargetPos.y - unit->mMoveCurrentPos.y, dz = unit->mMoveTargetPos.z - unit->mMoveCurrentPos.z;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->moveDistance == 0.0f) {
            unit->moveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->moveDistance));
        if (height == 0) {
            h = 0.0f;
        }
        if (height < 0) {
            h = -h * (f32)(-height);
        }
        if (height > 0) {
            h = h * (f32)height;
        }
        BtlUnit_SetPartsPos(unit, unit->mMoveCurrentPos.x, unit->mMoveCurrentPos.y + h, unit->mMoveCurrentPos.z);
    }
    else {
        f32 dx = unit->mMoveTargetPos.x - unit->mMoveCurrentPos.x, dy = unit->mMoveTargetPos.y - unit->mMoveCurrentPos.y, dz = unit->mMoveTargetPos.z - unit->mMoveCurrentPos.z;
        s32 neg;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (unit->mMoveTargetPos.x - unit->mMoveStartPos.x > 0.0f) {
            neg = 1;
        }
        else if (unit->mMoveTargetPos.x - unit->mMoveStartPos.x < 0.0f) {
            neg = 0;
        }
        else {
            if (unit->mMoveTargetPos.y - unit->mMoveStartPos.y < 0.0f) neg = 0; else neg = 1;
        }
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->moveDistance == 0.0f) {
            unit->moveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->moveDistance));
        if (neg) {
            a = 360.0f * atan2f_safety(unit->mMoveTargetPos.x - unit->mMoveStartPos.x, unit->mMoveTargetPos.y - unit->mMoveStartPos.y) / 6.2831855f;
        }
        else {
            a = 180.0f + 360.0f * atan2f_safety(unit->mMoveTargetPos.x - unit->mMoveStartPos.x, unit->mMoveTargetPos.y - unit->mMoveStartPos.y) / 6.2831855f;
        }
        sx = h * sinfd(a);
        sz = h * cosfd(180.0f + a);
        if (height == 0) {
            sx = 0.0f;
            sz = sx;
        }
        else {
            sx = sx * (f32)height;
            sz = sz * (f32)height;
        }
        BtlUnit_SetPartsPos(unit, unit->mMoveCurrentPos.x + sz, unit->mMoveCurrentPos.y + sx, unit->mMoveCurrentPos.z);
    }
    _PartsMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_AcSetDifficulty) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, diff;

    id = evtGetValue(event, args[0]);
    diff = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BattleActionCommandSetDifficulty(wp, unit, diff);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcGetDifficulty) {
    s32 index = event->args[0];
    evtSetValue(event, index, BattleActionCommandGetDifficulty(_battleWorkPointer));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetFlag) {
    _battleWorkPointer->actionCommands.acFlags = event->args[0];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetParamAll) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 i, value;

    for (i = 0; i < 8; i++) {
        value = evtGetValue(event, *args++);
        if (value != EVTDAT_POINTER_MAX) {
            wp->actionCommands.params[i] = value;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i = evtGetValue(event, args[0]);
    wp->actionCommands.params[i] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcGetParam) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);
    s32 index = args[1];
    s32 value = wp->actionCommands.params[i];
    if (index != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index, value);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetGaugeParam) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    wp->actionCommands.gaugeParams[0] = evtGetValue(event, args[0]);
    wp->actionCommands.gaugeParams[1] = evtGetValue(event, args[1]);
    wp->actionCommands.gaugeParams[2] = evtGetValue(event, args[2]);
    wp->actionCommands.gaugeParams[3] = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcGetOutputParam) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);
    s32 index = args[1];
    s32 value = wp->actionCommands.outputParams[i];
    if (index != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index, value);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetOutputParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i = evtGetValue(event, args[0]);
    wp->actionCommands.outputParams[i] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetupAC) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, a, b, c;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    a = evtGetValue(event, args[1]);
    b = args[2];
    c = evtGetValue(event, args[3]);
    BattleActionCommandSetup(wp, a, unit, b, c);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StartAC) {
    BattleWork* wp = _battleWorkPointer;
    s32 help = evtGetValue(event, event->args[0]);
    BattleActionCommandStart(wp);
    if (help && !(wp->actionCommands.mAcUnit->mTokenFlags & 0x10)) {
        BattleAcHelpSetDispType(2);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StopAC) {
    BattleActionCommandStop(_battleWorkPointer);
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ResultAC) {
    BattleWork* wp = _battleWorkPointer;
    BattleActionCommandResult(wp);
    if (wp->actionCommands.result & 1) {
        return EVT_RETURN_BLOCK;
    }
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ResultACDefence) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleWeapon* weapon;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    weapon = (BattleWeapon*)evtGetValue(event, args[1]);
    BattleActionCommandCheckDefence(unit, weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultAC) {
    evtSetValue(event, event->args[0], _battleWorkPointer->actionCommands.result);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultPrizeLv) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit = NULL;
    s32 id, value, index;

    id = evtGetValue(event, args[0]);
    if (id != -1 && id != -5) {
        unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    }
    value = evtGetValue(event, args[1]);
    index = args[2];
    evtSetValue(event, index, BattleActionCommandGetPrizeLv(wp, unit, value));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultACDefence) {
    s32 index = event->args[0];
    evtSetValue(event, index, BattleActionCommandGetDefenceResult());
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultCountAC) {
    evtSetValue(event, event->args[0], _battleWorkPointer->actionCommands.mResultCount);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AudienceDeclareACResult) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWeapon* weapon;
    s32 result;
    u8* ac;
    weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    result = evtGetValue(event, args[1]);
    ac = &wp->marioAcSuccess;
    if (result != -1) {
        if (ac[0] < 200) {
            ac[0]++;
        }
        wp->lastAcSuccess = 1;
    } else {
        if (ac[1] < 200) {
            ac[1]++;
        }
        wp->lastAcSuccess = 0;
    }
    BattleActionCommandDeclareACResult(wp, weapon, result);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AudienceDeclareAcrobatResult) {
    EvtWord* args = event->args;
    u8* res;
    s32 good;
    u8* aud;
    BattleWork* wp;
    BattleUnit* unit;
    f32 x, y, z;
    Vec pos;

    res = (u8*)evtGetValue(event, args[0]);
    good = evtGetValue(event, args[1]);
    wp = _battleWorkPointer;
    unit = BattleGetUnitPtr(wp, event->unitId);
    aud = (u8*)&wp->impendingBonuses.mStylishCommandMultiplier;
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    if (good) {
        EffectEntry* eff;
        aud[0] = res[0x18];
        aud[3] = res[0x1B];
        BattleAudience_Case_AcrobatGood();
        pos.y += BtlUnit_GetHeight(unit);
        pos.x += BtlUnit_GetWidth(unit) / 2;
        if (pos.x > 150.0f) {
            pos.x = 150.0f;
        }
        eff = effNiceEntry(7, pos.x + x, pos.y + y, pos.z + z);
        ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
    } else {
        BattleAudience_Case_AcrobatBad();
        psndSFXOn_3D("SFX_BTL_ACROBAT_MISS1", &pos);
    }
    return EVT_RETURN_DONE;
}

extern void psndSFX_pit(s32 id, s32 pitch);

#ifdef VERSION_G8ME01
// US: per-language width table [language][0] * [language][type + 1]
static f32 ac_success_width_tbl[7][8] = {
    {30.0f, 3.0f, 3.0f, 3.0f, 3.0f, 4.0f, 5.0f, 6.0f},
    {30.0f, 4.0f, 4.0f, 4.0f, 4.0f, 5.0f, 9.0f, 9.0f},
    {30.0f, 4.0f, 4.0f, 4.0f, 5.0f, 6.0f, 7.0f, 10.0f},
    {30.0f, 4.0f, 4.0f, 4.0f, 4.0f, 5.0f, 8.0f, 9.0f},
    {30.0f, 4.0f, 4.0f, 4.0f, 4.0f, 5.0f, 6.0f, 11.0f},
    {30.0f, 5.0f, 5.0f, 5.0f, 7.0f, 6.0f, 11.0f, 11.0f},
    {30.0f, 5.0f, 5.0f, 5.0f, 6.0f, 6.0f, 7.0f, 7.0f},
};

#endif
USER_FUNC(btlevtcmd_ACSuccessEffect) {
    EvtWord* args = event->args;
    s32 type;
    f32 scale;
    f32 w;
    Vec pos;
    f32 h;
    const char* name;
    s32 eff;
#ifdef VERSION_G8ME01
    s32 lang;
#endif
    EffectEntry* entry;

    type = evtGetValue(event, args[0]);
    scale = 0.75f;
#ifdef VERSION_G8ME01
    switch (gp->language) {
        case 1: lang = 1; break;
        case 2: lang = 2; break;
        case 3: lang = 3; break;
        case 4: lang = 4; break;
        case 5: lang = 5; break;
        case 6: lang = 6; break;
        case 0:
        default: lang = 0; break;
    }
#endif
    pos.x = (f32)evtGetValue(event, args[1]);
    pos.y = (f32)evtGetValue(event, args[2]);
    pos.z = (f32)evtGetValue(event, args[3]);
    switch (type) {
#ifdef VERSION_G8ME01
        case 0: scale *= 0.3f; name = "SFX_SYSTEM_NICE_25PERCENT1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][1]; eff = 0; break;
        case 1: scale *= 0.6f; name = "SFX_SYSTEM_NICE_50PERCENT1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][2]; eff = 0; break;
        case 2: scale *= 1.2f; name = "SFX_SYSTEM_NICE1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][3]; eff = 0; break;
        case 3: scale *= 1.2f; name = "SFX_SYSTEM_GOOD1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][4]; eff = 1; break;
        case 4: scale *= 1.2f; name = "SFX_SYSTEM_GREAT1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][5]; eff = 2; break;
        case 5: name = "SFX_SYSTEM_WONDERFUL1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][6]; eff = 3; break;
        case 6: name = "SFX_SYSTEM_EXCELLENT1"; w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][7]; eff = 4; break;
#else
        case 0: scale *= 0.3f; name = "SFX_SYSTEM_NICE_25PERCENT1"; w = 90.0f; eff = 0; break;
        case 1: scale *= 0.6f; name = "SFX_SYSTEM_NICE_50PERCENT1"; w = 90.0f; eff = 0; break;
        case 2: scale *= 1.2f; name = "SFX_SYSTEM_NICE1"; w = 90.0f; eff = 0; break;
        case 3: scale *= 1.2f; name = "SFX_SYSTEM_GOOD1"; w = 90.0f; eff = 1; break;
        case 4: scale *= 1.2f; name = "SFX_SYSTEM_GREAT1"; w = 120.0f; eff = 2; break;
        case 5: name = "SFX_SYSTEM_WONDERFUL1"; w = 150.0f; eff = 3; break;
        case 6: name = "SFX_SYSTEM_EXCELLENT1"; w = 180.0f; eff = 4; break;
#endif
        default: return EVT_RETURN_DONE;
    }
    w *= scale;
    h = w / 2.0f;
    if (pos.x + h > 160.0f) {
        pos.x = 160.0f - h;
#ifdef VERSION_G8ME01
    }
    if (pos.x - h < -180.0f) {
        pos.x = -180.0f + h;
#endif
    }
    if (pos.y < 80.0f) {
        entry = effNiceEntry(eff, pos.x - h, 50.0f + pos.y, pos.z);
    }
    else {
        entry = effNiceEntry(eff, pos.x - h, pos.y - 50.0f, pos.z);
    }
    ((NiceEffectWork*)entry->userdata)->overallScale = scale;
    psndSFX_pit(psndSFXOn_3D(name, &pos), 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ac_timing_flag_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    BattleWork* wp;
    u32 flags = args[1];
    wp = _battleWorkPointer;
    if (on) {
        ((TimingAWork*)wp->actionCommands.work)->flags |= flags;
    }
    else {
        ((TimingAWork*)wp->actionCommands.work)->flags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ac_timing_get_success_frame) {
    s32 index = event->args[0];
    s32 frame = battleAcTimingA_GetSuccessFrame(_battleWorkPointer);
    evtSetValue(event, index, frame);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ACHelpSet) {
    u8* weapon = (u8*)evtGetValue(event, event->args[0]);
    BattleAcHelpSetHelp(((BattleWeapon*)weapon)->acHelpMsg);
    BattleAcHelpSetDispType(2);
    return EVT_RETURN_DONE;
}
/* half_A end PRE */

//s32* battleId, s32* windowStartFrame, s32* windowEndFrame, s32* endFrame, s32* earlyFrames
USER_FUNC(btlevtcmd_ACRStart) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, windowStartFrame, windowEndFrame, endFrame, earlyFrames;

    if (isFirstCall) {
        id = evtGetValue(event, args[0]);
        id = BattleTransID(event, id);
        windowStartFrame = evtGetValue(event, args[1]);
        windowEndFrame = evtGetValue(event, args[2]);
        endFrame = evtGetValue(event, args[3]);
        earlyFrames = evtGetValue(event, args[4]);
        BattleAcrobatStart(wp, id, windowStartFrame, windowEndFrame, endFrame, earlyFrames);
    }
    return BattleAcrobatMain(wp);
}

//s32 resultRetIndex, s32 frameRetIndex (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_ACRGetResult) {
    EvtWord* args = event->args;
    s32 index1, index2, result, frame;

    index1 = args[0];
    index2 = args[1];
    BattleAcrobatGetResult(_battleWorkPointer, &result, &frame);
    if (index1 != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index1, result);
    }
    if (index2 != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index2, frame);
    }
    return EVT_RETURN_DONE;
}

/* half_A begin MID */
USER_FUNC(btlevtcmd_AudienceAppeal) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit) {
        BattleAudience_Case_Appeal(unit);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangeGroup) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    const char* name;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    name = (const char*)evtGetValue(event, args[2]);
    if (isFirstCall) {
        if (part->poseId != -1) {
            animPoseRelease(part->poseId);
            part->poseId = -1;
        }
        animEffectAsync(name, 1);
    }
    if (!animEffectAsync(name, 1)) {
        return EVT_RETURN_BLOCK;
    }
    part->poseId = animPoseEntry(name, 1);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePose) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    const char* name;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    name = (const char*)evtGetValue(event, args[2]);
    BtlUnit_SetAnim(part, name);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseType) {
    EvtWord* args = event->args;
    s32 type;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    type = evtGetValue(event, args[2]);
    BtlUnit_GetPoseNameFromType(part, type);
    BtlUnit_SetAnimType(part, type);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseDirect) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, pno;
    const char* name;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    name = (const char*)evtGetValue(event, args[2]);
    strcpy(part->poseName, name);
    part->animFlags &= ~0x80;
    part->animFlags &= ~0x10;
    animPoseSetAnim(part->poseId, name, TRUE);
    part->poseType = 1;
    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
        part->poseType = 7;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeWaitPlayComplete) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    if (isFirstCall) {
        event->userdata[0] = 1;
    }
    if (event->userdata[0] > 0) {
        event->userdata[0]--;
        return EVT_RETURN_BLOCK;
    }
    if (animPoseGetLoopTimes(part->poseId) < 1.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeWaitPlayLoopTiming) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    f32 loop;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    if (isFirstCall) {
        event->userdata[0] = 1;
        event->userdata[1] = 0;
        event->userdata[2] = 0;
    }
    if (__fabs(animPoseGetAnimPosePtr(part->poseId)->localTimeRate) < (double)0.01f) {
        return EVT_RETURN_DONE;
    }
    loop = animPoseGetLoopTimes(part->poseId);
    if (event->userdata[0] > 0) {
        event->userdata[0]--;
        return EVT_RETURN_BLOCK;
    }
    switch (event->userdata[1]) {
    case 0:
        event->userdata[2] = (s32)loop;
        event->userdata[1]++;
        return EVT_RETURN_BLOCK;
    }
    if ((s32)loop != event->userdata[2]) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(btlevtcmd_SetTalkBodyId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, body;

    id = evtGetValue(event, args[0]);
    body = evtGetValue(event, args[1]);
    BattleGetUnitPtr(wp, BattleTransID(event, id))->talkPartId = body;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetTalkPose) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    EvtWord name = args[1];
    BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id))->talkPose = (void*)name;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetStayPose) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    EvtWord name = args[1];
    BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id))->stayPose = (void*)name;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetTalkPoseType) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, type, body;

    id = evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    body = BtlUnit_GetBodyPartsId(unit);
    part = BattleGetUnitPartsPtr(id, body);
    unit->talkPose = BtlUnit_GetPoseNameFromType(part, type);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetStayPoseType) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, type, body;

    id = evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    body = BtlUnit_GetBodyPartsId(unit);
    part = BattleGetUnitPartsPtr(id, body);
    unit->stayPose = BtlUnit_GetPoseNameFromType(part, type);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeCmpPoseType) {
    EvtWord* args = event->args;
    s32 id, pno;
    s32 t1, t2;
    BattleUnitPart* part;
    s32 index;
    char* n1;
    char* n2;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    t1 = evtGetValue(event, args[2]);
    t2 = evtGetValue(event, args[3]);
    index = args[4];
    n1 = BtlUnit_GetPoseNameFromType(part, t1);
    n2 = BtlUnit_GetPoseNameFromType(part, t2);
    evtSetValue(event, index, strcmp(n1, n2) != 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    btlDispPoseAnime(part);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeSetPoseTable) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    part->poseTable = (void*)args[2];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeSetPoseSoundTable) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    part->poseSound.table = (BtlPoseSoundData*)args[2];
    BtlUnit_PoseSoundInit(part);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDefinePosetype) {
    EvtWord* args = event->args;
    s32 type;
    BattleUnitPart* part;
    s32 id, pno;
    s32 out;
    s32* table;
    s32 res;
    s32 end;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    type = evtGetValue(event, args[2]);
    out = args[3];
    table = (s32*)part->poseTable;
    res = 0;
    if (type <= 28) {
        end = 28;
    } else if (type < 40) {
        end = 39;
    } else if (type < 70) {
        end = 69;
    } else {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    for (; table; table += 2) {
        if (table[0] >= 70) {
            res = 0;
            break;
        }
        if (table[0] == type) {
            res = 1;
            break;
        }
        if (table[0] == end) {
            res = 0;
            break;
        }
    }
    evtSetValue(event, out, res);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeSetMotionSpeed) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    part->animSpeed2 = evtGetFloat(event, args[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePaperGroup) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    const char* name;
    s32 value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    name = (const char*)evtGetValue(event, args[2]);
    value = evtGetValue(event, args[3]);
    animPoseSetEffect(part->poseId, name, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePaperAnime) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    s32 value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    value = evtGetValue(event, args[2]);
    btlDispChangePeraAnime(part, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeFlagOnOff) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    u32 flags, on;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    flags = args[2];
    on = evtGetValue(event, args[3]);
    if (on) {
        part->animFlags |= flags;
    }
    else {
        part->animFlags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckGuardMotionEnable) {
    EvtWord* args = event->args;
    s32 id, index;
    BOOL enable;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    enable = TRUE;
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) || BtlUnit_CheckStatus(unit, STATUS_FREEZE) ||
        BtlUnit_CheckStatus(unit, STATUS_TINY) || BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) || BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
        enable = FALSE;
    }
    if (BtlUnit_CheckStatusFlag(unit, 1)) {
        enable = FALSE;
    }
    evtSetValue(event, index, enable);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckStatus) {
    EvtWord* args = event->args;
    s32 id, index;
    s8 type;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    type = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (BtlUnit_CheckStatus(unit, type)) {
        evtSetValue(event, index, TRUE);
    }
    else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnOffStatus) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, turns, strength, onoff;
    s8 type;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    type = args[1];
    turns = evtGetValue(event, args[2]);
    strength = evtGetValue(event, args[3]);
    onoff = evtGetValue(event, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (onoff) {
        BtlUnit_SetStatus(unit, type, (s8)turns, (s8)strength);
    }
    else {
        BtlUnit_SetStatus(unit, type, 0, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ClearAllStatus) {
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    BtlUnit_ClearStatus(BattleGetUnitPtr(_battleWorkPointer, id));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnStatusFlag) {
    BattleUnit* unit;
    EvtWord* args = event->args;
    s32 id;
    EvtWord flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    flags = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OnStatusFlag(unit, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffStatusFlag) {
    BattleUnit* unit;
    EvtWord* args = event->args;
    s32 id;
    EvtWord flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    flags = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OffStatusFlag(unit, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckActStatus) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 index = args[1];
    if (BtlUnit_CanActStatus(BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id)))) {
        evtSetValue(event, index, TRUE);
    }
    else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetACPossibility) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 index = args[1];
    if (BtlUnit_GetACPossibility(BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id)))) {
        evtSetValue(event, index, TRUE);
    }
    else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckUnitFlag) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index;
    s32 flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    flags = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (BtlUnit_CheckUnitFlag(unit, flags)) {
        evtSetValue(event, index, TRUE);
    }
    else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnUnitFlag) {
    BattleUnit* unit;
    EvtWord* args = event->args;
    s32 id;
    EvtWord flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    flags = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OnUnitFlag(unit, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffUnitFlag) {
    BattleUnit* unit;
    EvtWord* args = event->args;
    s32 id;
    EvtWord flags;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    flags = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OffUnitFlag(unit, flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AfterReactionEntry) {
    EvtWord* args = event->args;
    s32 id, value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    value = evtGetValue(event, args[1]);
    BattleAfterReactionEntry(id, value);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckWeaponPluralTarget) {
    EvtWord* args = event->args;
    u8* weapon = (u8*)evtGetValue(event, args[0]);
    s32 out;
    out = args[1];
    if (((BattleWeapon*)weapon)->targetClassFlags & TARGET_CLASS_SINGLE) {
        evtSetValue(event, out, 0);
    }
    else {
        evtSetValue(event, out, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PayWeaponCost) {
    EvtWord* args = event->args;
    BattleWeapon* weapon;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    weapon = (BattleWeapon*)evtGetValue(event, args[1]);
    BtlUnit_PayWeaponCost(BattleGetUnitPtr(_battleWorkPointer, id), weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandPayWeaponCost) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    void* weapon;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    weapon = BattleGetSelectWeapon(wp);
    BtlUnit_PayWeaponCost(unit, weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetWeaponActionLv) {
    EvtWord* args = event->args;
    u8* weapon = (u8*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], weapon[0x6E]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandGetWeaponActionLv) {
    s32 index = event->args[0];
    u8* weapon = (u8*)BattleGetSelectWeapon(_battleWorkPointer);
    evtSetValue(event, index, weapon[0x6E]);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandGetWeaponAddress) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index, kind;
    void* weapon;

    id = evtGetValue(event, args[0]);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    kind = (s32)unit->currentKind;
    if (kind == UNIT_MARIO || (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX)) {
        weapon = BattleGetSelectWeapon(wp);
    }
    else {
        weapon = itemDataTable[(s32)unit->heldItem].battle_params;
    }
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandGetWeaponItemId) {
    EvtWord* args = event->args;
    s32 index = args[0];
    u8* weapon = (u8*)evtGetValue(event, args[1]);
    evtSetValue(event, index, ((BattleWeapon*)weapon)->itemId);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetItemRecoverParam) {
    EvtWord* args = event->args;
    s32 id, a, b, c;
    u8* item;

    id = evtGetValue(event, args[0]);
    a = args[1];
    b = args[2];
    c = args[3];
    item = (u8*)&itemDataTable[id];
    if (a != EVTDAT_POINTER_MAX) {
        evtSetValue(event, a, item[0x1D]);
    }
    if (b != EVTDAT_POINTER_MAX) {
        evtSetValue(event, b, item[0x1E]);
    }
    if (c != EVTDAT_POINTER_MAX) {
        evtSetValue(event, c, item[0x1F]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WeaponAftereffect) {
    BattleWork* wp = _battleWorkPointer;
    BattleWeapon* w = (BattleWeapon*)evtGetValue(event, event->args[0]);
    s32 a1, a2, b;
    s32 r;

    b = 0;
    a2 = 0;
    a1 = 0;
    r = irand(w->stageEffectRates[0] + w->stageEffectRates[1] + w->stageEffectRates[2] + w->stageEffectRates[3]);
    if ((u32)wp->debug.flags & 0x10) a1 = 1;
    if ((u32)wp->debug.flags & 0x20) a2 = 1;
    r -= w->stageEffectRates[0];
    if (r < 0) {
        a2 = 1;
        a1 = 1;
    }
    else {
        r -= w->stageEffectRates[1];
        if (r < 0) {
            a1 = 1;
        }
        else {
            r -= w->stageEffectRates[2];
            if (r < 0) {
                a2 = 1;
            }
        }
    }
    if (wp->stageObjectWork.flags & 4) a2 = 0;
    if (wp->stageObjectWork.flags & 2) a1 = 0;
    if ((wp->stageObjectWork.flags & 2) && !(wp->stageObjectWork.flags & 8)) {
        if (irand(100) < w->stageEffectRates[4] || ((u32)wp->debug.flags & 0x40)) {
            b = 1;
            if (!(wp->stageObjectWork.flags & 4)) a2 = 1;
        }
    }
    if (a1) BattleStage_DestroyA1();
    if (a2) BattleStage_DestroyA2();
    if (b) BattleStage_DestroyB();
    if (irand(100) < w->stageEffectRates[5] || ((u32)wp->debug.flags & 0x200)) BattleStage_NozzleDirChangeCheck();
    if (irand(100) < w->stageEffectRates[6] || ((u32)wp->debug.flags & 0x100)) BattleStage_NozzleWorkCheck(1);
    if (irand(100) < w->stageEffectRates[7] || ((u32)wp->debug.flags & 0x80)) BattleStage_IronFrameFallCheck();
    if (wp->stageObjectWork.flags & 0x20) {
        if ((u32)wp->debug.flags & 1) {
            BattleStage_ObjectFallCheck();
        }
        else if (irand(100) < w->stageEffectRates[8]) {
            BattleStage_ObjectFallCheck();
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetGuard) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, btlId;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    btlId = evtGetValue(event, args[1]);
    unit->protectId = BattleTransID(event, btlId);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetGuardKouraId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32* koura;
    s32 id, index;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    index = args[1];
    unit = BattleGetUnitPtr(wp, id);
    koura = (s32*)BtlUnit_GetGuardKouraPtr(unit);
    if (koura) {
        evtSetValue(event, index, *koura);
    }
    else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_InterruptStop) {
    s32 type = evtGetValue(event, event->args[0]);
    BattleWork* wp = _battleWorkPointer;

    if (isFirstCall) {
        switch (type) {
            case 0:
                wp->flags |= 0x10;
                break;
            case 1:
                wp->flags |= 0x20;
                break;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SortPhaseMoveTable) {
    battleSortPhaseMoveTable(_battleWorkPointer, -1);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetStageSize) {
    EvtWord* args = event->args;
    BattleStage* stage = BattleStageGetPtr();
    s32 x = args[0];
    s32 y = args[1];
    s32 z = args[2];

    if (x != EVTDAT_POINTER_MAX) {
        evtSetValue(event, x, stage->stageSizeX);
    }
    if (y != EVTDAT_POINTER_MAX) {
        evtSetValue(event, y, stage->stageSizeY);
    }
    if (z != EVTDAT_POINTER_MAX) {
        evtSetValue(event, z, stage->stageSizeZ);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetStageAudienceAreaSize) {
    EvtWord* args = event->args;
    BattleStage* stage = BattleStageGetPtr();
    s32 x = args[0];
    s32 y = args[1];
    s32 z = args[2];

    if (x != EVTDAT_POINTER_MAX) {
        evtSetValue(event, x, stage->audienceSizeX);
    }
    if (y != EVTDAT_POINTER_MAX) {
        evtSetValue(event, y, stage->audienceSizeY);
    }
    if (z != EVTDAT_POINTER_MAX) {
        evtSetValue(event, z, stage->audienceSizeZ);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_TransStageFloorPosition) {
    EvtWord* args = event->args;
    s32 index;
    f32 x, z;

    x = evtGetValue(event, args[0]);
    index = args[1];
    z = evtGetValue(event, args[2]);
    evtSetValue(event, index, BattleGetFloorHeight(_battleWorkPointer, x, 0.0f, z));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMoveSoundDataWork) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    const char* name0;
    const char* name1;
    s32 c, d, e;
    s16 f, g;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    name0 = (const char*)evtGetValue(event, args[1]);
    name1 = (const char*)evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);
    f = evtGetValue(event, args[6]);
    g = evtGetValue(event, args[7]);
    BtlUnit_SetSeMode(&unit->moveSnd, name0, name1, c, d, e, f, g);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetRunSound) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);
    unit->moveSndTable.entry[0].name0 = (const char*)a;
    unit->moveSndTable.entry[0].name1 = (const char*)b;
    unit->moveSndTable.entry[0].delay = c;
    unit->moveSndTable.entry[0].period0 = d;
    unit->moveSndTable.entry[0].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsRunSound) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);
    part->moveSndTable.entry[0].name0 = (const char*)a;
    part->moveSndTable.entry[0].name1 = (const char*)b;
    part->moveSndTable.entry[0].delay = c;
    part->moveSndTable.entry[0].period0 = d;
    part->moveSndTable.entry[0].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetWalkSound) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);
    unit->moveSndTable.entry[1].name0 = (const char*)a;
    unit->moveSndTable.entry[1].name1 = (const char*)b;
    unit->moveSndTable.entry[1].delay = c;
    unit->moveSndTable.entry[1].period0 = d;
    unit->moveSndTable.entry[1].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsWalkSound) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);
    part->moveSndTable.entry[1].name0 = (const char*)a;
    part->moveSndTable.entry[1].name1 = (const char*)b;
    part->moveSndTable.entry[1].delay = c;
    part->moveSndTable.entry[1].period0 = d;
    part->moveSndTable.entry[1].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetDiveSound) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);
    unit->moveSndTable.entry[2].name0 = (const char*)a;
    unit->moveSndTable.entry[2].name1 = (const char*)b;
    unit->moveSndTable.entry[2].delay = c;
    unit->moveSndTable.entry[2].period0 = d;
    unit->moveSndTable.entry[2].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsDiveSound) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    EvtWord a, b;
    s32 c, d, e;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);
    part->moveSndTable.entry[2].name0 = (const char*)a;
    part->moveSndTable.entry[2].name1 = (const char*)b;
    part->moveSndTable.entry[2].delay = c;
    part->moveSndTable.entry[2].period0 = d;
    part->moveSndTable.entry[2].period1 = e;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetJumpSound) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    EvtWord a, b;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    unit->moveSndTable.jumpName0 = (const char*)a;
    unit->moveSndTable.jumpName1 = (const char*)b;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsJumpSound) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;
    EvtWord a, b;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    part->moveSndTable.jumpName0 = (const char*)a;
    part->moveSndTable.jumpName1 = (const char*)b;
    return EVT_RETURN_DONE;
}

#pragma dont_inline on
void _UnitMoveSoundControl(BattleUnit* unit) {
    f32 x, y, z;
    BtlUnit_GetPos(unit, &x, &y, &z);
    _MoveSoundControlCore(unit, &unit->moveSnd, NULL, x, y, z);
}

void _PartsMoveSoundControl(BattleUnitPart* part) {
    f32 x, y, z;
    BtlUnit_GetPartsPos(part, &x, &y, &z);
    _MoveSoundControlCore(part->mOwner, &part->moveSnd, part, x, y, z);
}

void _MoveSoundControlCore(BattleUnit* unit, BecSndWork* work, BattleUnitPart* part, f32 x, f32 y, f32 z) {
    Vec pos;
    Vec cur;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    const char* name;

    tmp.x = x;
    tmp.y = y;
    tmp.z = z;
    pos = tmp;
    if ((EvtWord)work->name0 || (EvtWord)work->name1) {
        if ((EvtWord)work->name0 && !(EvtWord)work->name1) {
            if (work->sfxId == 0xFFFFFFFF) {
                work->sfxId = BtlUnit_snd_se_pos(unit, work->name0, EVTDAT_POINTER_MAX, 0, &pos);
            }
            if (part) {
                BtlUnit_GetPartsWorldPos(part, &cur.x, &cur.y, &cur.z);
            }
            else {
                BtlUnit_GetPos(unit, &cur.x, &cur.y, &cur.z);
            }
            psndSFX_pos(work->sfxId, &cur);
        }
        else if (work->timer >= 0) {
            work->timer--;
            if (work->timer < 0) {
                work->count++;
                if (work->count & 1) {
                    name = work->name0;
                    work->timer = work->period0;
                }
                else {
                    name = work->name1;
                    work->timer = work->period1;
                }
                if ((EvtWord)name) {
                    BtlUnit_snd_se_pos(unit, name, EVTDAT_POINTER_MAX, 0, &pos);
                }
            }
        }
    }
}

void _UnitMoveSoundControlEnd(BattleUnit* unit) {
    _MoveSoundControlEndCore(unit, &unit->moveSnd);
}

void _PartsMoveSoundControlEnd(BattleUnitPart* part) {
    _MoveSoundControlEndCore(part->mOwner, &part->moveSnd);
}

const char* _mario_acrobat_voice_table[5] = {
    "SFX_VOICE_MARIO_AC3_1", "SFX_VOICE_MARIO_AC3_2", "SFX_VOICE_MARIO_AC3_3",
    "SFX_VOICE_MARIO_AC3_4", "SFX_VOICE_MARIO_AC3_5",
};
const char* _mario_attack_voice_table[10] = {
    "SFX_VOICE_MARIO_AC1_1", "SFX_VOICE_MARIO_AC1_2", "SFX_VOICE_MARIO_AC1_3",
    "SFX_VOICE_MARIO_AC1_4", "SFX_VOICE_MARIO_AC1_5", "SFX_VOICE_MARIO_AC2_1",
    "SFX_VOICE_MARIO_AC2_2", "SFX_VOICE_MARIO_AC2_3", "SFX_VOICE_MARIO_AC2_4",
    "SFX_VOICE_MARIO_AC2_5",
};
void _MoveSoundControlEndCore(BattleUnit* unit, BecSndWork* work) {
    if ((u32)work->sfxId != 0xFFFFFFFF) {
        psndSFXOff(work->sfxId);
        work->sfxId = -1;
    }
}
#pragma dont_inline reset

USER_FUNC(btlevtcmd_snd_se) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    const char* name;
    s32 volume;
    BattleUnit* unit;
    s32 index, ret;
    s16 pitch;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    name = (const char*)evtGetValue(event, args[1]);
    volume = evtGetValue(event, args[2]);
    pitch = evtGetValue(event, args[3]);
    index = args[4];
    ret = BtlUnit_snd_se(unit, name, volume, pitch);
    if ((index) != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index, ret);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_snd_se_offset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    s32 index;
    const char* name;
    s32 volume, ret;
    BattleUnit* unit;
    s16 pitch;
    Vec pos;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    name = (const char*)evtGetValue(event, args[1]);
    volume = evtGetValue(event, args[2]);
    pitch = evtGetValue(event, args[3]);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += evtGetFloat(event, args[4]);
    pos.y += evtGetFloat(event, args[5]);
    pos.z += evtGetFloat(event, args[6]);
    index = args[7];
    ret = BtlUnit_snd_se_pos(unit, name, volume, pitch, &pos);
    if (index != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index, ret);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_snd_voice) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, type;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    switch (type) {
        case 1:
            BtlUnit_snd_se(unit, _mario_acrobat_voice_table[irand(5)], EVTDAT_POINTER_MAX, 0);
            break;
        case 2:
            BtlUnit_snd_se(unit, _mario_attack_voice_table[irand(10)], EVTDAT_POINTER_MAX, 0);
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_EnemyItemUseCheck) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index;
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    index = args[1];
    evtSetValue(event, index, BattleEnemyUseItemCheck(unit));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PadCheckTrigger) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    s32 mask = args[1];
    args = (EvtWord*)args[2];
    if (BattlePadMultiCheckTrigger(chan, mask)) {
        evtSetValue(event, (EvtWord)args, TRUE);
    }
    else {
        evtSetValue(event, (EvtWord)args, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PadCheckNow) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    s32 mask = args[1];
    args = (EvtWord*)args[2];
    if (BattlePadMultiCheckNow(chan, mask)) {
        evtSetValue(event, (EvtWord)args, TRUE);
    }
    else {
        evtSetValue(event, (EvtWord)args, FALSE);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_InviteApInfoReport) {
    StarPowerInfo* info = &_battleWorkPointer->impendingBonuses;
    BattleAudience_ApRecoveryBuild(info);
    memset(info, 0, sizeof(StarPowerInfo));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SacBgsetMove) {
    BattleWork* wp = _battleWorkPointer;
    s32 on, i;
    BattleStageObject* obj;
    s32 mode;
    f32 val;

    on = evtGetValue(event, event->args[0]);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    event->userdata[0]++;
    mode = on ? 5 : 2;
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0) {
            switch (obj->data->mLayer) {
                case 0:
                case 1:
                    if (on) {
                        if (obj->mPosition.x >= 0.0f) {
                            val = intplGetValue(mode, 450.0f, 0.0f, event->userdata[0], 45);
                        }
                        else {
                            val = -intplGetValue(mode, 450.0f, 0.0f, event->userdata[0], 45);
                        }
                    }
                    else {
                        if (obj->mPosition.x >= 0.0f) {
                            val = intplGetValue(mode, 0.0f, 450.0f, event->userdata[0], 45);
                        }
                        else {
                            val = -intplGetValue(mode, 0.0f, 450.0f, event->userdata[0], 45);
                        }
                    }
                    obj->positionOffset2.x = val;
                    break;
                case 2:
                    if (on) {
                        val = intplGetValue(mode, 260.0f, 0.0f, event->userdata[0], 45);
                    }
                    else {
                        val = intplGetValue(mode, 0.0f, 260.0f, event->userdata[0], 45);
                    }
                    obj->positionOffset2.y = val;
                    break;
            }
        }
    }
    return !(event->userdata[0] < 45);
}

USER_FUNC(btlevtcmd_BtlStageObjFlagOnOff) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleStageObject* obj;
    s32 onoff;
    const char* name;
    s32 flags;

    name = (const char*)evtGetValue(event, args[0]);
    flags = args[1];
    onoff = evtGetValue(event, args[2]);
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (onoff) {
                obj->mFlags |= flags;
            }
            else {
                obj->mFlags &= ~flags;
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_BtlStageObjSetRotate) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleStageObject* obj;
    const char* name;
    s32 xa, y, z, x, ya, za;

    name = (const char*)evtGetValue(event, args[0]);
    xa = args[1];
    x = evtGetValue(event, xa);
    ya = args[2];
    y = evtGetValue(event, ya);
    za = args[3];
    z = evtGetValue(event, za);
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (xa != EVTDAT_POINTER_MAX) {
                obj->mRotation.x = x;
            }
            if (ya != EVTDAT_POINTER_MAX) {
                obj->mRotation.y = y;
            }
            if (za != EVTDAT_POINTER_MAX) {
                obj->mRotation.z = z;
                if (!obj) {
                }
            }
            break;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_BtlStageObjGetRotate) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    BattleStageObject* obj;
    const char* name;
    s32 x, y, z;

    name = (const char*)evtGetValue(event, args[0]);
    x = args[1];
    y = args[2];
    z = args[3];
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (x != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, x, obj->mRotation.x);
            }
            if (y != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, y, obj->mRotation.y);
            }
            if (z != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, z, obj->mRotation.z);
            }
            break;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StatusWindowOnOff) {
    BattleWork* wp = _battleWorkPointer;
    switch (evtGetValue(event, event->args[0])) {
        case 2:
            wp->statusWindowSystemOn = 1;
            break;
        case 1:
            wp->statusWindowEventOn = 1;
            break;
        case 0:
            wp->statusWindowEventOn = 0;
            break;
    }
    BattleStatusWindowCheck();
    return EVT_RETURN_DONE;
}
/* half_A end MID */

USER_FUNC(btlevtcmd_StageDispellFog) {
    BattleFogForceStop();
    return EVT_RETURN_DONE;
}

/* half_A begin POST */
USER_FUNC(btlevtcmd_ftomsec) {
    EvtWord* args = event->args;
    s32 value = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], value * 1000 / 60);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ftof) {
    EvtWord* args = event->args;
    s32 index, value, frames;
    value = evtGetValue(event, args[0]);
    index = args[1];
#ifdef VERSION_G8ME01
    if (gp->framerate == 60) {
        evtSetValue(event, index, value);
        return EVT_RETURN_DONE;
    }
    frames = sysMsec2FrameFloat(value * 1000 / 60);
#else
    frames = sysMsec2Frame(value * 1000 / 60);
#endif
    evtSetValue(event, index, frames);
    return EVT_RETURN_DONE;
}
/* half_A end POST */
