// Battle event commands (btlevtcmd_*): the script functions battle scripts use to query and control units.

#include "version.h"
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
#include "battle/battle_detect.h"
#include "battle/battle_seq_command.h"
#include "battle/battle_enemy_item.h"
#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif
#include "battle/battle_ac_help.h"
#include "pmario_sound.h"
#include <math.h>
#include <string.h>
#include "data/item_data.h"
#include "driver/animdrv.h"
#include "eff/n64/eff_coin_n64.h"

/** s32* eventId */
//s32* eventId
USER_FUNC(btlevtcmd_WaitEventEnd) {
    s32 id;

    id = evtGetValue(event, *event->args);
    return evtCheckID(id) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** s32 retIndex, s32 battleFlags */
//s32 retIndex, s32 battleFlags
USER_FUNC(btlevtcmd_check_battleflag) {
    EvtWord* args = event->args;
    s32 index = *args++;
    BattleWork* wp = _battleWorkPointer;
    u32 flags = *args++;
    if (wp->flags & flags) {
        evtSetValue(event, index, TRUE);
    } else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** BOOL* onoff, s32 flags */
//BOOL* onoff, s32 flags
USER_FUNC(btlevtcmd_onoff_battleflag) {
    EvtWord* args = event->args;
    BOOL val = evtGetValue(event, args[0]);
    u32 flags = args[1];
    BattleWork* wp = _battleWorkPointer;
    if (val == TRUE) {
        wp->flags |= flags;
    } else {
        wp->flags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

/** s32 retIndex */
//s32 retIndex
USER_FUNC(btlevtcmd_get_turn) {
    evtSetValue(event, *event->args, _battleWorkPointer->turnCount);
    return EVT_RETURN_DONE;
}

/** no params */
//no params
USER_FUNC(btlevtcmd_reset_turn) {
    _battleWorkPointer->flags |= BATTLE_FLAG_RESET_TURN;
    return EVT_RETURN_DONE;
}

/** no params */
//no params
USER_FUNC(btlevtcmd_reset_move_color_lv_all) {
    btlsubResetMoveColorLvAll(_battleWorkPointer);
    return EVT_RETURN_DONE;
}

/** s32 retIndex, s32 phaseSeqId */
//s32 retIndex, s32 phaseSeqId
USER_FUNC(btlevtcmd_CheckPhase) {
    EvtWord* args = event->args;
    s32 index = args[0];
    u32 seq = args[1];
    if (seq != BattleGetSeq(_battleWorkPointer, SEQ_PHASE)) {
        evtSetValue(event, index, FALSE);
    } else {
        evtSetValue(event, index, TRUE);
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32 indexX, s32 indexY, s32 indexZ */
//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetPos) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32 indexX, s32 indexY, s32 indexZ */
//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetPosFloat) {
    EvtWord* args = event->args;
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

/** s32* maxValue, s32 retIndex */
//s32* maxValue, s32 retIndex
USER_FUNC(btlevtcmd_GetRandomValue) {
    EvtWord* args = event->args;
    s32 max, index;

    max = evtGetValue(event, args[0]);
    index = args[1];
    evtSetValue(event, index, irand(max));
    return EVT_RETURN_DONE;
}

/** s32 retIndex, s32 countIndex, s32 sizeIndex[] */
//s32 retIndex, s32 countIndex, s32 sizeIndex[]
USER_FUNC(btlevtcmd_DrawLots) {
    EvtWord* args = event->args;
    s32 index = args[0];
    s32 *alloc;
    s32 i, count, max;
    s32 ceiling;
    s32 nv[2];

    args += 2;
    count = evtGetValue(event, args[-1]);
    alloc = BattleAlloc(sizeof(s32) * count);
    max = 0;
    for (i = 0; i < count; i++) {
        alloc[i] = evtGetValue(event, *args++);
        max += alloc[i];
    }
    ceiling = irand(max);
    nv[1] = 0;
    i = 0;
    nv[0] = i;
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

/**
 * Script: spawns a new unit in the battle from the BattleUnitSetup args[1] (args[2] passed through to
 * BtlUnit_Spawn) and stores its unit id in args[0], or -1 when no slot was free.
 * Used by enemies that summon others.
 */
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

/** s32* battleId, s32 flags */
//s32* battleId, s32 flags
USER_FUNC(btlevtcmd_KillUnit) {
    EvtWord* args = event->args;
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
    if (flags & KILL_UNIT_INACTIVE) {
        unit->attributeFlags |= kInactive;
        return EVT_RETURN_DONE;
    } else {
        BtlUnit_SetStatus(unit, STATUS_INSTANT_KILL, 1, 1);
        if (!(flags & KILL_UNIT_KEEP)) {
            unit->flags |= BATTLE_UNIT_FLAG_BROKEN;
        }
        return EVT_RETURN_DONE;
    }
}

/** s32* battleId, s32* copyToPartId, BattleUnitKindPart* kind, s32* copyFromPartId */
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
        // BUG: kind is never advanced, so this loops forever unless the first entry is the wanted part
        while (kind->partNum != partNum) {
            ;
        }
        part->kindPartParams = kind;
        part->partName = part->kindPartParams->partName;
        part->attributes = part->kindPartParams->attributes;
        part->counterAttributes = part->kindPartParams->counterAttributes;
        part->defense = part->kindPartParams->defense;
        part->defenseAttr = part->kindPartParams->defenseAttr;
        part->positionOffset.x = part->kindPartParams->positionOffset.x;
        part->positionOffset.y = part->kindPartParams->positionOffset.y;
        part->positionOffset.z = part->kindPartParams->positionOffset.z;
        BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotateOffset(part, 0.0f, 0.0f, 0.0f);
        part->hitBaseOffset.x = part->kindPartParams->hitBaseOffset.x;
        part->hitBaseOffset.y = part->kindPartParams->hitBaseOffset.y;
        part->hitOffset.x = 0.0f;
        part->hitOffset.y = 0.0f;
        part->hitOffsetX = 0;
    }
    btlDispEntAnime(unit);
    return EVT_RETURN_DONE;
}

/** s32 retIndex */
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

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to set current) */
//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to set current)
USER_FUNC(btlevtcmd_SetPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;

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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPos) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ (can be EVTDAT_POINTER_MAX for noret) */
//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ (can be EVTDAT_POINTER_MAX for noret)
USER_FUNC(btlevtcmd_GetPartsPos) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsPos) {
    EvtWord* args = event->args;
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
    if (part->attributes & kAbsolutePosition) {
        BtlUnit_SetPartsPos(part, x, y, z);
    } else {
        BtlUnit_GetPos(unit, &currX, &currY, &currZ);
        x -= currX;
        y -= currY;
        z -= currZ;
        BtlUnit_SetPartsOffsetPos(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsPos) {
    EvtWord* args = event->args;
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
    if (part->attributes & kAbsolutePosition) {
        BtlUnit_AddPartsPos(part, x, y, z);
    } else {
        BtlUnit_AddPartsOffsetPos(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetDispOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsDispOffset) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsDispOffset) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetTogeOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32 indexX, s32 indexY, s32 indexZ */
//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetHomePos) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetHomePos) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddHomePos) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsHomePos) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ */
//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ
USER_FUNC(btlevtcmd_GetHitPos) {
    EvtWord* args = event->args;
    f32 x, y, z;
    s32 id, partId;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 indexX, indexY, indexZ;

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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId, s32 retIndex */
//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetWidth) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index, width;

    id = evtGetValue(event, args[0]);
    index = args[1];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    width = BtlUnit_GetWidth(unit);
    evtSetValue(event, index, width);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32 retIndex */
//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetHeight) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index, height;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    height = BtlUnit_GetHeight(unit);
    evtSetValue(event, index, height);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s16* height */
//s32* battleId, s16* height
USER_FUNC(btlevtcmd_SetHeight) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 height;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    height = evtGetValue(event, args[1]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_SetHeight(unit, (s16)height);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32 retIndex */
//s32* battleId, s32 retIndex
USER_FUNC(btlevtcmd_GetStatusMg) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index;

    id = evtGetValue(event, args[0]);
    index = args[1];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    evtSetFloat(event, index, unit->sizeMultiplier);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store) */
//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store)
USER_FUNC(btlevtcmd_SetStatusIconOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, xpos, ypos;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    xpos = evtGetValue(event, args[1]);
    ypos = evtGetValue(event, args[2]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (xpos != EVTDAT_POINTER_MAX) {
        unit->statusIconOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_POINTER_MAX) {
        unit->statusIconOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store) */
//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_POINTER_MAX to not store)
USER_FUNC(btlevtcmd_SetHpGaugeOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, xpos, ypos;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    xpos = evtGetValue(event, args[1]);
    ypos = evtGetValue(event, args[2]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (xpos != EVTDAT_POINTER_MAX) {
        unit->hpGaugeOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_POINTER_MAX) {
        unit->hpGaugeOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32 retX, s32 retY */
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
    x = unit->hpGaugeOffset[0];
    y = unit->hpGaugeOffset[1];
    if (x != EVT_NULLPTR) {
        evtSetValue(event, indexX, x);
    }
    if (y != EVT_NULLPTR) {
        evtSetValue(event, indexY, y);
    }
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, u8* alpha */
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

/** s32* battleId, s32* partId, u8* redIndex, u8* greenIndex, u8* blueIndex */
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
    // FAKE: args is reused to hold the blue index; indexB = args[4] changes the object
    args = (EvtWord*)args[4];
    evtSetValue(event, indexR, part->color.r);
    evtSetValue(event, indexG, part->color.g);
    evtSetValue(event, (EvtWord)args, part->color.b);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, u8* red, u8* green, u8* blue */
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

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_GetRotate) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetRotate) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;

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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddRotate) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_GetPartsRotate) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetBaseRotate) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    f32 x, y, z, currX, currY, currZ;
    s32 id, indexX, indexY, indexZ;

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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetPartsBaseRotate) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetRotateOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
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

/** s32* battleId */
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetCutBaseOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetBintaHitOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* x, f32* y, f32* z */
//s32* battleId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetKissHitOffset) {
    EvtWord* args = event->args;
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

/** s32* battleId, f32* value */
//s32* battleId, f32* value
USER_FUNC(btlevtcmd_SetCutWidth) {
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

/** s32* battleId, f32* value */
//s32* battleId, f32* value
USER_FUNC(btlevtcmd_SetCutHeight) {
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

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip)
USER_FUNC(btlevtcmd_SetPossessionItemOffset) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
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

/** s32* battleId, f32* x, f32* y, f32* z */
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

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsBaseScale) {
    EvtWord* args = event->args;
    s32 partId;
    BattleUnitPart* part;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    BtlUnit_SetPartsBaseScale(part, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_POINTER_MAX to skip) */
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

/** s32* battleId, f32* x, f32* y, f32* z */
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
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    BtlUnit_SetScale(unit, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, f32* x, f32* y, f32* z */
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
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(wp, id);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    BtlUnit_AddScale(unit, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_SetPartsScale) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    BtlUnit_SetPartsScale(part, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* partId, f32* x, f32* y, f32* z */
//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USER_FUNC(btlevtcmd_AddPartsScale) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);

    id = BattleTransID(event, id);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    BtlUnit_AddPartsScale(part, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* retIndex */
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

/** s32* battleId, s32* retIndex */
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

/** s32* battleId, s32* retIndex */
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

/** Script: returns in args[1] the maximum FP of the unit args[0] (badge bonuses included, via BtlUnit_GetMaxFp). */
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

/**
 * Script: sets the HP of the unit args[0] to args[1] at once,
 * snapping its HP gauge to the new value without the drain.
 */
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

/** Script: sets the FP of the unit args[0] to args[1]. */
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

/** Script: sets the maximum FP of the unit args[0] to args[1]. */
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

/** Script: sets the chance (swallowRate) that Yoshi's Gulp can swallow the unit args[0]. */
USER_FUNC(btlevtcmd_SetSwallowParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
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

/** Script: sets the unit args[0]'s swallowAttribute, which decides how Yoshi's Gulp treats it. */
USER_FUNC(btlevtcmd_SetSwallowAttribute) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
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

/**
 * Script: sets how many actions per turn the unit args[0] gets (args[1]),
 * cutting the moves left this turn to match.
 */
USER_FUNC(btlevtcmd_SetMaxMoveCount) {
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
    unit->maxMoveCount = (u8)evtGetValue(event, args[1]);
    if (unit->movesRemaining > unit->maxMoveCount) {
        unit->movesRemaining = unit->maxMoveCount;
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the part number of the part the unit args[0] is currently targeting. */
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
    evtSetValue(event, args[1], unit->currentTarget->kindPartParams->partNum);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the HP damage the unit args[0] took from the current attack. */
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

/** Script: returns in args[1] the FP damage the unit args[0] took from the current attack. */
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

/** Script: returns in args[1] how many damaging hits the unit args[0] took from the current attack. */
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

/**
 * Script: heals the unit args[0] by args[2] HP (args[1], the part, is read but unused).
 * The HP gauge animates the rise unless the unit's HP is locked.
 */
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
    evtGetValue(event, args[1]); // read and ignored
    hp = evtGetValue(event, args[2]);
    if (!(unit->attributeFlags & kUnitHpFixed)) {
        unit->hpGauge.prevHp = unit->currentHp;
        BtlUnit_RecoverHp(unit, hp);
        unit->hpGauge.targetHp = unit->currentHp;
        unit->hpGauge.drainTimer = unit->hpGauge.drainLength;
        unit->hpGauge.visibleTimer = unit->hpGauge.visibleLength;
        unit->hpGauge.flags |= HP_GAUGE_FLAG_TIMED;
    } else {
        BtlUnit_RecoverHp(unit, hp);
    }
    return EVT_RETURN_DONE;
}

/** Script: restores args[2] FP to the unit args[0] (args[1] is read but unused). */
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
    evtGetValue(event, args[1]); // read and ignored
    value = evtGetValue(event, args[2]);
    BtlUnit_RecoverFp(unit, value);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] how many more turns the unit args[0] stays flipped over. */
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

/** Script: sets how many turns the unit args[0] stays flipped over (shelled enemies flipped by a jump or POW Block). */
USER_FUNC(btlevtcmd_SetOverTurnCount) {
    s32 id;
    EvtWord* args = event->args;
    s32 btlId;
    s32 value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    value = evtGetValue(event, args[1]);
    // FAKE: block-scoped unit pointer; BattleGetUnitPtr(_battleWorkPointer, id)->flippedTurns = value changes the object
    {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
        unit->flippedTurns = value;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[2] the unit args[0]'s script work slot args[1],
 * free storage that unit scripts use between events.
 */
USER_FUNC(btlevtcmd_GetUnitWork) {
    EvtWord* args = event->args;
    s32 id, i, index;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    i = evtGetValue(event, args[1]);
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->work[i]);
    return EVT_RETURN_DONE;
}

/** Script: stores args[2] in the unit args[0]'s script work slot args[1]. */
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

/** Script: adds args[2] to the unit args[0]'s script work slot args[1]. */
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

/** Script: returns in args[2] the unit args[0]'s work slot args[1] read as a float. */
USER_FUNC(btlevtcmd_GetUnitWorkFloat) {
    EvtWord* args = event->args;
    s32 id, i, index;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    i = evtGetValue(event, args[1]);
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetFloat(event, index, *(f32*)&unit->work[i]);
    return EVT_RETURN_DONE;
}

/** Script: stores the float args[2] in the unit args[0]'s work slot args[1]. */
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

/** Script: returns in args[3] work slot args[2] of part args[1] of the unit args[0]. */
USER_FUNC(btlevtcmd_GetPartsWork) {
    EvtWord* args = event->args;
    s32 id, partId, i, index;
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

/** Script: stores args[3] in work slot args[2] of part args[1] of the unit args[0]. */
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

/** Script: returns in args[1] the damage of the last hit the unit args[0] took. */
USER_FUNC(btlevtcmd_GetDamage) {
    EvtWord* args = event->args;
    s32 id, index;
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

/** Script: returns in args[1] the total damage the unit args[0] took from the current attack. */
USER_FUNC(btlevtcmd_GetTotalDamage) {
    EvtWord* args = event->args;
    s32 id, index;
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
    s32 btlId, id, mask, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit->damageCode & mask) {
        evtSetValue(event, index, 1);
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: makes the unit args[0] react to a miss with damage code args[1] (dodge, lucky, step aside),
 * running its hit event. A lucky miss (0x26) also shows the "Lucky!" sign beside it.
 */
USER_FUNC(btlevtcmd_StartAvoid) {
    EvtWord* args = event->args;
    s32 id, damageCode;
    EffectEntry* entry;
    BattleUnit* unit;
    Vec pos;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    damageCode = args[1] | DAMAGE_RESULT_FINAL_HIT;
    if ((u8)damageCode == DAMAGE_CODE_LUCKY) {
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        if (pos.y < 85.0f) {
            pos.y += 50.0f;
        } else {
            pos.y += -50.0f;
        }
        pos.x -= 45.0f;
        entry = effNiceEntry(NICE_EFFECT_LUCKY, pos.x, pos.y, pos.z);
        ((NiceEffectWork*)entry->userdata)->overallScale = 0.75f;
    }
    unit->damagePattern = 0;
    BattleRunHitEvent(unit, damageCode);
    return EVT_RETURN_DONE;
}

/**
 * Script: runs the hit event args[2] on the unit args[0] directly with damage code args[1]
 * and damage pattern args[3].
 */
USER_FUNC(btlevtcmd_RunHitEventDirect) {
    EvtWord* args = event->args;
    void* code;
    BattleUnit* unit;
    s32 id, damageCode;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    damageCode = args[1];
    code = (void*)evtGetValue(event, args[2]);
    unit->damagePattern = evtGetValue(event, args[3]);
    BattleRunHitEventDirect(unit, damageCode | DAMAGE_RESULT_FINAL_HIT, code);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns 1 in args[2] when the last hit on the unit args[0] had damage pattern args[1] (which hurt
 * reaction to play).
 */
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
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: finds a unit whose home position lies within args[4..6] of (args[1..3]) on any axis with a non-zero
 * bound and returns its id in args[0], or -1; used to find a free spot when spawning.
 * Only units of alliance 1 (the enemy side) are checked.
 */
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
    for (i = 0; i < BATTLE_UNIT_MAX; ++i) {
        found = FALSE;
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->alliance == ALLIANCE_ENEMY) {
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

/** Script: returns in args[1] the alliance hostile to the unit args[0]. */
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

/** Script: returns in args[1] the unit args[0]'s own alliance (0 = Mario's side). */
USER_FUNC(btlevtcmd_GetFriendBelong) {
    EvtWord* args = event->args;
    s32 btlId, id, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, unit->alliance);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the id of the unit that the unit args[0] is stepping in to protect. */
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

/** Script: returns in args[1] the unit paired with args[0] (Mario for the partner, the partner for Mario), or -1. */
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
    } else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the id of Mario on the side of the unit args[0], or -1. */
USER_FUNC(btlevtcmd_GetMarioId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, alliance;
    BattleUnit* unit;
    int i;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    alliance = BattleGetUnitPtr(wp, id)->alliance;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->alliance == alliance && unit->currentKind == UNIT_MARIO) {
            break;
        }
    }
    if (i >= 64) {
        evtSetValue(event, index, -1);
    } else {
        evtSetValue(event, index, unit->unitId);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the id of the partner on the side of the unit args[0], or -1. */
USER_FUNC(btlevtcmd_GetPartyId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 btlId, id, index, alliance, kind;
    BattleUnit* unit;
    int i;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    index = args[1];
    alliance = BattleGetUnitPtr(wp, id)->alliance;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->alliance == alliance) {
            kind = unit->currentKind;
            if (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX) {
                break;
            }
        }
    }
    if (i >= 64) {
        evtSetValue(event, index, -1);
    } else {
        evtSetValue(event, index, unit->unitId);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the id of the unit args[0]'s body (main) part. */
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
    } else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[1] the upgrade rank of the partner args[0] (0 = normal, 1 = Super, 2 = Ultra),
 * 0 for other units.
 */
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

/**
 * Script: works out a facing (1 = right, -1 = left) for the unit args[0] (part args[1], -1 for the whole unit)
 * and returns it in args[5].
 * The flags args[4] choose the source: its current facing, its side's attack direction, its move direction,
 * toward the unit args[2] (part args[3]) or toward the x coordinate args[2]; FACE_DIR_REVERSE flips it.
 */
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

    if (!(flags & (FACE_DIR_TOWARD_X | FACE_DIR_MOVE_DIR))) {
        tid = BattleTransID(event, tid);
    }

    thisUnit = BattleGetUnitPtr(_battleWorkPointer, thisId);
    if (thisPartId != -1) {
        thisPart = BtlUnit_GetPartsPtr(thisUnit, thisPartId);
    }

    if (flags & FACE_DIR_TOWARD_UNIT) {
        targetUnit = BattleGetUnitPtr(_battleWorkPointer, tid);
        if (targetPartId != -1) {
            targetPart = BtlUnit_GetPartsPtr(targetUnit, targetPartId);
        }
    }

    value = thisUnit->faceDirection;
    if (thisPart) {
        value = thisPart->faceDirection;
    }

    if (flags & FACE_DIR_ATTACK_DIR) {
        value = wp->alliances[thisUnit->alliance].attackDirection;
    }

    if (flags & FACE_DIR_MOVE_DIR) {
        value = thisUnit->moveDirection;
        if (thisPart) {
            value = thisPart->moveDirection;
        }
    }

    if (flags & FACE_DIR_TOWARD_UNIT) {
        BtlUnit_GetPos(thisUnit, &thisX, &thisY, &thisZ);
        if (thisPart) {
            if (thisPart->attributes & kAbsolutePosition) {
                BtlUnit_GetPartsPos(thisPart, &thisX, &thisY, &thisZ);
            } else {
                BtlUnit_GetPartsOffsetPos(thisPart, &offsetX, &offsetY, &offsetZ);
                thisX += offsetX;
            }
        }

        BtlUnit_GetPos(targetUnit, &targetX, &targetY, &targetZ);
        if (targetPart) {
            if (targetPart->attributes & kAbsolutePosition) {
                BtlUnit_GetPartsPos(targetPart, &targetX, &targetY, &targetZ);
            } else {
                BtlUnit_GetPartsOffsetPos(targetPart, &offsetX, &offsetY, &offsetZ);
                targetX += offsetX;
            }
        }

        calculate = targetX - thisX;
        if (calculate > 0.0f) {
            value = 1;
        } else if (calculate < 0.0f) {
            value = -1;
        }
    }

    if (flags & FACE_DIR_TOWARD_X) {
        BtlUnit_GetPos(thisUnit, &localX, &localY, &localZ);
        if (thisPart) {
            if (thisPart->attributes & kAbsolutePosition) {
                BtlUnit_GetPartsPos(thisPart, &localX, &localY, &localZ);
            } else {
                BtlUnit_GetPartsOffsetPos(thisPart, &thisOffsetX, &thisOffsetY, &thisOffsetZ);
                localX += thisOffsetX;
            }
        }
        calculate = (f32)targetId - localX;
        if (calculate > 0.0f) {
            value = 1;
        } else if (calculate < 0.0f) {
            value = -1;
        }
    }
    if (flags & FACE_DIR_REVERSE) {
        value *= -1;
    }
    evtSetValue(event, index, value);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the facing of the unit args[0] (1 = right, -1 = left). */
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

/** Script: sets the facing of the unit args[0] to args[1]. */
USER_FUNC(btlevtcmd_ChangeFaceDirection) {
    s32 id;
    EvtWord* args = event->args;
    s32 btlId;
    s32 value;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    value = evtGetValue(event, args[1]);
    // FAKE: block-scoped unit pointer; BattleGetUnitPtr(_battleWorkPointer, id)->faceDirection = value changes the object
    {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
        unit->faceDirection = value;
    }
    return EVT_RETURN_DONE;
}

/** Script: sets the facing of part args[1] of the unit args[0] to args[2]. */
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

/** Script: turns the unit args[0] back to its side's attack direction (toward the other side). */
USER_FUNC(btlevtcmd_ResetFaceDirection) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 btlId;

    btlId = evtGetValue(event, *event->args);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, btlId));
    unit->faceDirection = wp->alliances[unit->alliance].attackDirection;
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckAttribute) {
    EvtWord* args = event->args;
    s32 btlId, id, mask, index;
    BattleUnit* unit;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit->attributeFlags & mask) {
        evtSetValue(event, index, 1);
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: sets the attribute flags args[1] on the unit args[0]. */
USER_FUNC(btlevtcmd_OnAttribute) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, mask;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->attributeFlags |= mask;
    return EVT_RETURN_DONE;
}

/** Script: clears the attribute flags args[1] on the unit args[0]. */
USER_FUNC(btlevtcmd_OffAttribute) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 btlId, id, mask;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    mask = args[1];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->attributeFlags = unit->attributeFlags & ~mask;
    return EVT_RETURN_DONE;
}

/** Script: returns 1 in args[3] when part args[1] of the unit args[0] has any of the part attributes args[2]. */
USER_FUNC(btlevtcmd_CheckPartsAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, index, mask;
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
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: sets the part attributes args[2] on part args[1] of the unit args[0]. */
USER_FUNC(btlevtcmd_OnPartsAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, mask;
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

/** Script: clears the part attributes args[2] on part args[1] of the unit args[0]. */
USER_FUNC(btlevtcmd_OffPartsAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, mask;
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

/**
 * Script: returns 1 in args[3] when part args[1] of the unit args[0] has any of the counter attributes args[2]
 * (spiky, fiery, electric and so on).
 */
USER_FUNC(btlevtcmd_CheckPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, index, mask;
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
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: sets the counter attributes args[2] on part args[1] of the unit args[0]. */
USER_FUNC(btlevtcmd_OnPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, mask;
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

/** Script: clears the counter attributes args[2] on part args[1] of the unit args[0]. */
USER_FUNC(btlevtcmd_OffPartsCounterAttribute) {
    EvtWord* args = event->args;
    s32 id, partId, mask;
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

/**
 * Script: sets the two afterimage colours of part args[1] of the unit args[0]:
 * RGBA args[2..5] at the head of the trail and args[6..9] at its end.
 */
USER_FUNC(btlevtcmd_SetPartsBlur) {
    EvtWord* args = event->args;
    s32 id, partId;
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

/** Script: sets the token flags args[1] on the unit args[0] (per-action markers scripts use to signal each other). */
USER_FUNC(btlevtcmd_OnToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    // FAKE: block-scoped enum copy of args[1]; unit->tokenFlags |= args[1] changes the object
    {
        BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1];
        unit->tokenFlags |= m;
    }
    return EVT_RETURN_DONE;
}

/** Script: clears the token flags args[1] on the unit args[0]. */
USER_FUNC(btlevtcmd_OffToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    // FAKE: block-scoped enum mask copy; direct args[1] changes the object
    {
        BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1];
        unit->tokenFlags &= ~m;
    }
    return EVT_RETURN_DONE;
}

/** Script: returns 1 in args[2] when the unit args[0] has any of the token flags args[1]. */
USER_FUNC(btlevtcmd_CheckToken) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    index = args[2];
    // FAKE: block-scoped enum copy of args[1]; testing unit->tokenFlags & args[1] changes the object
    {
        BattleUnitToken_Flags m = (BattleUnitToken_Flags)args[1];
        if (unit->tokenFlags & m) {
            evtSetValue(event, index, 1);
        } else {
            evtSetValue(event, index, 0);
        }
    }
    return EVT_RETURN_DONE;
}

/** Script: replaces the unit args[0]'s status vulnerability table with the BattleVulnerableStatus args[1]. */
USER_FUNC(btlevtcmd_SetRegistStatus) {
    EvtWord* args = event->args;
    BattleVulnerableStatus* status;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    status = (BattleVulnerableStatus*)evtGetValue(event, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id)->vulnStatus = status;
    return EVT_RETURN_DONE;
}

/** Script: returns 1 in args[1] when the unit args[0] is one the player commands (Mario or a partner). */
USER_FUNC(btlevtcmd_CheckCommandUnit) {
    EvtWord* args = event->args;
    s32 id, index;
    BattleUnitType kind;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    kind = BattleGetUnitPtr(_battleWorkPointer, id)->currentKind;
    index = args[1];
    if (kind == UNIT_MARIO || (kind >= TYPE_PARTNER_MIN && kind < TYPE_PARTNER_MAX)) {
        evtSetValue(event, index, 1);
    } else {
        evtSetValue(event, index, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: changes the unit kind of the unit args[0] to args[1] (for enemies that transform). */
USER_FUNC(btlevtcmd_ChangeKind) {
    s32 btlId, id;
    EvtWord* args = event->args;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    BattleGetUnitPtr(_battleWorkPointer, id)->currentKind = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script: replaces the defense table of part args[1] of the unit args[0] with args[2] (BattleUnitDefense). */
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

/** Script: replaces the defense attribute table of part args[1] of the unit args[0] with args[2]. */
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

/** Script: replaces the unit args[0]'s data table (its events for damage, KO, appeal and so on) with args[1]. */
USER_FUNC(btlevtcmd_ChangeDataTable) {
    EvtWord* args = event->args;
    s32 btlId, id;

    btlId = evtGetValue(event, args[0]);
    id = BattleTransID(event, btlId);
    BattleGetUnitPtr(_battleWorkPointer, id)->dataTable = (BattleDataEntry*)args[1];
    return EVT_RETURN_DONE;
}

/**
 * Script: when the defeated unit args[0] holds an item it stole from Mario,
 * puts the item in the battle's recovered items list so Mario gets it back after the battle.
 */
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
            if (info->recoveredItems[i] == ITEM_NULL) {
                break;
            }
        }
        info->recoveredItems[i] = item;
        unit->heldItem = ITEM_NULL;
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the Star Points the unit args[0] is worth. */
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

/** Script: adds args[0] Star Points to the total earned this battle (shown by the counter in the corner). */
USER_FUNC(btlevtcmd_StoreExp) {
    s32 val = evtGetValue(event, event->args[0]);
    BattleStoreExp(_battleWorkPointer, val);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[1] the coins the unit args[0] drops. */
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

/** Script: adds args[0] coins to the coins won this battle. */
USER_FUNC(btlevtcmd_StoreCoin) {
    s32 val = evtGetValue(event, event->args[0]);
    BattleStoreCoin(_battleWorkPointer, val);
    return EVT_RETURN_DONE;
}

/** Script: sets the walking speed (units per frame) of the unit args[0] for the next move. */
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

/** Script: sets the horizontal jump speed of the unit args[0]. */
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

/** Script: sets the gravity per frame of the unit args[0] for its jumps and falls. */
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

/**
 * Script: sets the unit args[0]'s next jump: frame count args[1] (0 = derive from the speed),
 * horizontal speed args[2] and gravity args[3].
 */
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
    unit->moveSpeedXZ = speed;
    unit->fallAccel = accel;
    return EVT_RETURN_DONE;
}

void _UnitMoveSoundControl(BattleUnit* unit);
void _UnitMoveSoundControlEnd(BattleUnit* unit);

/**
 * Builds a Vec for a 3D sound position; becMvVecA to becMvVecE are identical copies, one per call site,
 * as the original code had them.
 */
static inline Vec becMvVecA(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/** Identical to becMvVecA (sound position for another mover). */
static inline Vec becMvVecB(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/** Identical to becMvVecA (sound position for another mover). */
static inline Vec becMvVecC(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/** Identical to becMvVecA (sound position for another mover). */
static inline Vec becMvVecD(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/** Identical to becMvVecA (sound position for another mover). */
static inline Vec becMvVecE(f32 x, f32 y, f32 z) {
    Vec v = {0.0f, 0.0f, 0.0f};
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/**
 * Script: jumps the unit args[0] in an arc to (args[1..3]) over args[4] frames (0 = from the jump speed),
 * using its gravity, with the jump sounds of sound set args[5]. Blocks until it lands.
 */
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
        unit->moveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        y = ty - y;
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
            unit->moveDistance = unit->moveDistance - unit->moveFrames * unit->moveSpeedXZ;
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
            unit->moveDistance = unit->moveDistance - unit->moveFrames * unit->moveSpeedXZ;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->moveSpeedY = y / unit->moveFrames + (unit->fallAccel * unit->moveFrames) / 2.0f;
        unit->moveSpeedXZ = unit->moveSpeedXZ + unit->moveDistance / unit->moveFrames;
        BtlUnit_LoadSeMode(2, kind, &unit->moveSndTable, &unit->moveSnd);
        if ((EvtWord)unit->moveSnd.name0) {
            Vec pos = becMvVecA(unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
            psndSFXOn_3D(unit->moveSnd.name0, &pos);
        }
    }
    unit->moveCurrentPos.y += unit->moveSpeedY;
    unit->moveSpeedY -= unit->fallAccel;
    if (unit->moveSpeedY < 0.0f && unit->moveCurrentPos.y < unit->moveTargetPos.y) {
        unit->moveCurrentPos.y = unit->moveTargetPos.y;
    }
    btlMovePos(unit->moveSpeedXZ, unit->mMoveDirection, &unit->moveCurrentPos.x, &unit->moveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        if ((EvtWord)unit->moveSnd.name1) {
            Vec pos = becMvVecB(unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
            psndSFXOn_3D(unit->moveSnd.name1, &pos);
        }
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/** Script: continues the unit args[0]'s current jump arc (speed and gravity as they are) until it reaches the floor. */
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
    unit->moveCurrentPos.y += unit->moveSpeedY;
    unit->moveSpeedY -= unit->fallAccel;
    btlMovePos(unit->moveSpeedXZ, unit->mMoveDirection, &unit->moveCurrentPos.x, &unit->moveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    h = BattleGetFloorHeight(_battleWorkPointer, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    if (unit->moveCurrentPos.y < h) {
        unit->moveCurrentPos.y = h;
        BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: drops the unit args[0] to (args[1..3]) over args[4] frames (0 = from its move speed),
 * starting with no upward speed and choosing the gravity to land exactly; blocks until it arrives.
 */
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
        unit->moveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        y = ty - y;
        if (unit->moveFrames == 0) {
            if (unit->moveSpeedXZ == 0.0f) {
                unit->moveSpeedXZ = 1.0f;
            }
            unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
            if (unit->moveFrames == 0) {
                unit->moveFrames = 1;
            }
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
        }
        unit->moveSpeedY = 0.0f;
        unit->fallAccel = (y / unit->moveFrames - unit->moveSpeedY) / (-unit->moveFrames / 2.0f);
    }
    unit->moveCurrentPos.y += unit->moveSpeedY;
    unit->moveSpeedY -= unit->fallAccel;
    btlMovePos(unit->moveSpeedXZ, unit->mMoveDirection, &unit->moveCurrentPos.x, &unit->moveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: walks the unit args[0] in a straight line to (args[1..3]) over args[4] frames (0 = from its move
 * speed), playing the footstep sounds of sound set args[5] in mode args[6] (run, walk and so on).
 * Blocks until it arrives.
 */
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
        unit->moveDistance = distABf(x, z, tx, tz);
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
            if (unit->moveFrames == 0) {
                unit->moveFrames = 1;
            }
            unit->moveSpeedXZ = unit->moveSpeedXZ + (unit->moveDistance - unit->moveFrames * unit->moveSpeedXZ) / unit->moveFrames;
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
        }
        BtlUnit_LoadSeMode(mode, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    btlMovePos(unit->moveSpeedXZ, unit->mMoveDirection, &unit->moveCurrentPos.x, &unit->moveCurrentPos.z);
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        _UnitMoveSoundControlEnd(unit);
        return EVT_RETURN_YIELD;
    }
    _UnitMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

/**
 * Script: returns in args[5] how many frames the unit args[0] needs to reach x = args[1],
 * z = args[3] at speed args[4] (0 = its move speed); args[2] is read but unused.
 * Passing EVT_NULLPTR as args[5] discards the result.
 */
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
        speed = unit->moveSpeedXZ;
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


/**
 * Script: flies the unit args[0] to (args[1..3]) over args[4] frames along a curve bowed by args[5] units (0 =
 * straight), the bow vertical for type args[7] 0 and sideways otherwise;
 * args[6] is the interpolation mode (+100 also pins it to the target at the end), args[8] the sound set.
 * Floating units use it instead of walking. Blocks until it arrives.
 */
USER_FUNC(btlevtcmd_DivePosition) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, frames, height, mode, floor, type, kind;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 h, a, sx, sz;

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
            unit->moveDistance = sqrtf(dx * dx + dy * dy + dz * dz);
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->moveTotalFrames = 0.0f;
        unit->mMoveDirection = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        BtlUnit_LoadSeMode(3, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    unit->moveCurrentPos.x = intpl_sub(mode, unit->moveStartPos.x, unit->moveTargetPos.x, (s32)unit->moveTotalFrames, unit->moveFrames);
    unit->moveCurrentPos.y = intpl_sub(mode, unit->moveStartPos.y, unit->moveTargetPos.y, (s32)unit->moveTotalFrames, unit->moveFrames);
    unit->moveCurrentPos.z = intpl_sub(mode, unit->moveStartPos.z, unit->moveTargetPos.z, (s32)unit->moveTotalFrames, unit->moveFrames);
    if (floor) {
        f32 fy = BattleGetFloorHeight(_battleWorkPointer, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
        if (unit->moveCurrentPos.y < fy) {
            unit->moveTotalFrames = (f32)unit->moveFrames;
            BtlUnit_SetMoveTargetPos(unit, unit->moveCurrentPos.x, fy, unit->moveCurrentPos.z);
        }
    }
    unit->moveTotalFrames += 1.0f;
    if (unit->moveTotalFrames > unit->moveFrames) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        _UnitMoveSoundControlEnd(unit);
        return EVT_RETURN_DONE;
    }
    if (type == 0) {
        f32 dx = unit->moveTargetPos.x - unit->moveCurrentPos.x, dy = unit->moveTargetPos.y - unit->moveCurrentPos.y, dz = unit->moveTargetPos.z - unit->moveCurrentPos.z;
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
        BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y + h, unit->moveCurrentPos.z);
    } else {
        f32 dx = unit->moveTargetPos.x - unit->moveCurrentPos.x, dy = unit->moveTargetPos.y - unit->moveCurrentPos.y, dz = unit->moveTargetPos.z - unit->moveCurrentPos.z;
        s32 neg;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (unit->moveTargetPos.x - unit->moveStartPos.x > 0.0f) {
            neg = 1;
        } else if (unit->moveTargetPos.x - unit->moveStartPos.x < 0.0f) {
            neg = 0;
        } else {
            if (unit->moveTargetPos.y - unit->moveStartPos.y < 0.0f) {
                neg = 0;
            } else {
                neg = 1;
            }
        }
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->moveDistance == 0.0f) {
            unit->moveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->moveDistance));
        if (neg) {
            a = 360.0f * atan2f_safety(unit->moveTargetPos.x - unit->moveStartPos.x, unit->moveTargetPos.y - unit->moveStartPos.y) / 6.2831855f;
        } else {
            a = 180.0f + 360.0f * atan2f_safety(unit->moveTargetPos.x - unit->moveStartPos.x, unit->moveTargetPos.y - unit->moveStartPos.y) / 6.2831855f;
        }
        sx = h * sinfd(a);
        sz = h * cosfd(180.0f + a);
        if (height == 0) {
            sx = 0.0f;
            sz = sx;
        } else {
            sx = sx * (f32)height;
            sz = sz * (f32)height;
        }
        BtlUnit_SetPos(unit, unit->moveCurrentPos.x + sz, unit->moveCurrentPos.y + sx, unit->moveCurrentPos.z);
    }
    _UnitMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

/**
 * Script: moves the unit args[0] to (args[1..3]) over args[4] frames along half an ellipse, args[5] units wide,
 * eased by interpolation mode args[6]; used when Mario and the partner swap places.
 */
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
            unit->moveDistance = sqrtf(dx * dx + dy * dy + dz * dz);
        }
        if (unit->moveFrames == 0) {
            unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
        }
        if (unit->moveFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->moveTotalFrames = 0.0f;
        unit->mMoveDirection = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
    }
    {
        f32 dx = unit->moveTargetPos.x - unit->moveStartPos.x, dz = unit->moveTargetPos.z - unit->moveStartPos.z;
        cx = (unit->moveStartPos.x + unit->moveTargetPos.x) / 2.0f;
        cz = (unit->moveStartPos.z + unit->moveTargetPos.z) / 2.0f;
        r = 0.5f * sqrtf(dx * dx + dz * dz);
    }
    w = (f32)width;
    ang = angleABf(cx, cz, unit->moveStartPos.x, unit->moveStartPos.z);
    t = intpl_sub(mode, 180.0f, 0.0f, (s32)unit->moveTotalFrames, unit->moveFrames);
    unit->moveCurrentPos.x = cx - (r * sinfd(ang) * cosfd(t) + w * cosfd(ang) * sinfd(t));
    unit->moveCurrentPos.z = cz + r * cosfd(ang) * cosfd(t) - w * sinfd(ang) * sinfd(t);
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    unit->moveTotalFrames += 1.0f;
    if (unit->moveTotalFrames > unit->moveFrames) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: adds args[2] in the unit args[0]'s move direction (its facing when it is not moving) to the variable
 * args[1].
 */
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

/** Script: adds args[2] in the direction the unit args[0] faces to the variable args[1] (positions in front of it). */
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

/**
 * Script: subtracts args[2] in the direction the unit args[0] faces from the variable args[1] (positions behind
 * it).
 */
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

/** Script: subtracts args[3] in the direction part args[1] of the unit args[0] faces from the variable args[2]. */
USER_FUNC(btlevtcmd_PartsFaceDirectionSub) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, partId, index, value, dir;

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

void _MoveSoundControlEndCore(BattleUnit* unit, BecSndWork* work);
void _MoveSoundControlCore(BattleUnit* unit, BecSndWork* work, BattleUnitPart* part, f32 x, f32 y, f32 z);
ItemEntry* itemEntry(const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* evtCode, f32 x, f32 y, f32 z);
/**
 * Refund badge payout when the unit uses an item:
 * (70 + 5 per Refund badge)% of the item's sell price in coins, with a coin effect at the unit.
 */
static inline void bec_coin(BattleUnit* unit, s32 item) {
    s32 coins;
    Vec pos;
    if (unit->badgesEquipped.Refund) {
        coins = (s32)((f32)(unit->badgesEquipped.Refund * 5 + 70) * (f32)itemDataTable[item].sell_price / 100.0f);
        if (coins > 0) {
            pouchAddCoin(coins);
            BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
            effCoinN64Entry(0, 0, pos.x, pos.y, pos.z, 1.0f);
        }
    }
}

/**
 * Script: fills the battle's target list for the weapon args[2] used by the unit args[0] against alliance
 * args[1], scanning from the side the weapon's target flags start on.
 */
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

/**
 * Script: picks the preferred target from the target list using the weapon args[0]'s target weights (enemy AI)
 * and returns its unit and part in args[1] and args[2].
 */
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy) {
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &_battleWorkPointer->weaponTargets;
    BattleWeapon* weapon;
    s32 unitIndex, partIndex, unitId, partId;

    weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    unitIndex = args[1];
    partIndex = args[2];
    BattleChoiceSamplingEnemy(targets, weapon->targetWeightFlags, &unitId, &partId);
    evtSetValue(event, unitIndex, unitId);
    evtSetValue(event, partIndex, partId);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns the current target of the target list in args[0] (unit) and args[1] (part),
 * or -1 when the list is empty.
 */
USER_FUNC(btlevtcmd_GetSelectEnemy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &wp->weaponTargets;
    s32 unitIndex = args[0];
    s32 partIndex = args[1];
    BattleWorkTarget* t;

    if (wp->weaponTargets.numTargets <= 0) {
        evtSetValue(event, unitIndex, -1);
        evtSetValue(event, partIndex, -1);
        return EVT_RETURN_DONE;
    }
    t = &targets->targets[targets->targetIndices[targets->currentTarget]];
    evtSetValue(event, unitIndex, t->unitIdx);
    evtSetValue(event, partIndex, t->partIdx);
    return EVT_RETURN_DONE;
}

/**
 * Script: advances to the next target in the list and returns it in args[0] (unit) and args[1] (part);
 * args[0] is -1 after the last.
 */
USER_FUNC(btlevtcmd_GetSelectNextEnemy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWorkWeaponTargets* targets = &wp->weaponTargets;
    s32 unitIndex = args[0];
    s32 partIndex = args[1];
    BattleWorkTarget* t;

    targets->currentTarget = wp->weaponTargets.currentTarget + 1;
    if (targets->currentTarget < targets->numTargets) {
        t = &targets->targets[targets->targetIndices[targets->currentTarget]];
        evtSetValue(event, unitIndex, t->unitIdx);
        evtSetValue(event, partIndex, t->partIdx);
    } else {
        evtSetValue(event, unitIndex, -1);
        evtSetValue(event, partIndex, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns the target of a first strike from the field in args[0] (unit) and args[1] (its body part). */
USER_FUNC(btlevtcmd_GetFirstAttackTarget) {
    EvtWord* args = event->args;
    BattleUnit* attacker;
    BattleUnit* unit;
    s32 unitIndex = args[0];
    s32 partIndex = args[1];
    s32 body;

    BattleGetFirstAttackUnit(_battleWorkPointer, &attacker, &unit);
    body = BtlUnit_GetBodyPartsId(unit);
    evtSetValue(event, unitIndex, unit->unitId);
    evtSetValue(event, partIndex, body);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the item chosen in the command menu's item list. */
USER_FUNC(btlevtcmd_GetConsumeItem) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleCommand* cmd = &wp->commandMenu;
    // FAKE: the post-increment of args is unused; args[0] changes the object
    evtSetValue(event, *args++, cmd->weapon[cmd->cursor[11].absPos].itemId);
    return EVT_RETURN_DONE;
}

/**
 * Script: shows the item args[3] as a fixed icon at (args[0..2]) for args[4] frames, then removes it;
 * blocks meanwhile. Used to show a Life Shroom over a revived unit.
 */
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
        event->userdatap[1] = itemEntry(NULL, item, ITEM_TYPE_STILL_NO_TOUCH, -1, NULL, x, y, z);
#if VERSION <= VERSION_DEMO_US
        itemFlagOn(event->userdatap[1], ITEM_FLAG_BADGE_RETURN);
#endif
        return EVT_RETURN_BLOCK;
    }
    if (--event->userdata[0] > 0) {
        return EVT_RETURN_BLOCK;
    }
    itemDelete(((ItemEntry*)event->userdatap[1])->name);
    return EVT_RETURN_DONE;
}

/**
 * Script: holds the item being used over the head of the unit args[0] for args[1] frames with a sparkle
 * and the item-use sound (Mario's, a partner's or an enemy's).
 * Mario and the partners show the item chosen in the menu, enemies their held item. Blocks until done.
 */
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
        if (!(unit->attributeFlags & kVeiled)) {
            if (unit->attributeFlags & kCeiling) {
                pos.y -= 5.0f;
            } else {
                pos.y += unit->sizeMultiplier * BtlUnit_GetHeight(unit);
            }
        }
        event->userdatap[1] = itemEntry(NULL, item, ITEM_TYPE_STILL_NO_TOUCH, -1, NULL, pos.x, pos.y, pos.z);
        effItemGetEntry(10, pos.x, 12.0f + pos.y, pos.z - 5.0f);
#if VERSION <= VERSION_DEMO_US
        itemFlagOn(event->userdatap[1], ITEM_FLAG_BADGE_RETURN);
#endif
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

/**
 * Script: uses up the item the unit args[0] used:
 * for Mario and the partners the item chosen in the menu leaves the pouch (if it is a pouch item)
 * and the Refund badge pays out; enemies lose their held item.
 */
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

/**
 * Script: for Mario and the partners, queues the item args[1] in the battle's reserve list,
 * to be taken out of the pouch after the battle (the automatic Life Shroom), and pays Refund;
 * an enemy holding a Life Shroom loses it.
 */
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
        bec_coin(unit, item);
    } else {
        if (unit->heldItem == ITEM_LIFE_SHROOM) {
            unit->heldItem = ITEM_NULL;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: declares the attack on the target in the variables args[1] (unit) and args[2] (part):
 * BattleAttackDeclare may redirect it, for example to a unit stepping in to protect the target,
 * and writes the final target back.
 */
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

/** Script: declares an attack on every target in the list (BattleAttackDeclareAll). */
USER_FUNC(btlevtcmd_AttackDeclareAll) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    BattleAttackDeclareAll(wp);
    return EVT_RETURN_DONE;
}

/**
 * Script: before the hit, checks whether the weapon args[3] of the unit args[0] will connect with part args[2]
 * of the unit args[1] (flags args[4]) and returns the result in args[5] (hit, miss, dodge, lucky and so on).
 * Ends the script when the check fails or the attacker's event was killed.
 */
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
    // FAKE: args is reused to hold the result index; out = args[5] changes the object
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

/** Script: like btlevtcmd_PreCheckDamage with the weapon chosen in the command menu (flags args[3], result args[4]). */
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

/**
 * Script: checks what the counter attributes of part args[2] of the unit args[1] would do to the unit args[0]
 * attacking with the weapon args[3] (spikes, fire and so on) and returns the result in args[5].
 */
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

/** Script: like btlevtcmd_PreCheckCounter with the weapon chosen in the command menu (result in args[4]). */
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

/**
 * Script: deals the hit: applies the weapon args[3] of the unit args[0] to part args[2] of the unit args[1] with
 * damage flags args[4] (damage, status, counters, the target's hit event) and returns the result in args[5].
 * Ends the script when it fails or the attacker's event was killed.
 */
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
    // FAKE: args is reused to hold the result index; a separate out local changes the object
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

/** Script: like btlevtcmd_CheckDamage with the weapon chosen in the command menu (flags args[3], result args[4]). */
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

/**
 * Script: deals args[2] damage straight to part args[1] of the unit args[0] without a weapon,
 * with damage code args[3], damage pattern args[4] and flags args[5].
 */
USER_FUNC(btlevtcmd_DamageDirect) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id;
    s32 damage;
    s32 result;
    s32 pattern;
    s32 partId;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    partId = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(unit, partId);
    damage = evtGetValue(event, args[2]);
    result = args[3];
    pattern = evtGetValue(event, args[4]);
    BattleDamageDirect(-5, unit, part, damage, 0, result, pattern, evtGetValue(event, args[5]));
    return EVT_RETURN_DONE;
}

/** Script: sets the unit args[0]'s damage event (the script run when it is hit) to args[1]. */
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

/** Script: sets the unit args[0]'s wait event (its idle loop between actions) to args[1]. */
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

/** Script: sets the unit args[0]'s entry event (run when it enters the battle) to args[1]. */
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

/** Script: sets the unit args[0]'s phase event (run at phase changes such as turn start and end) to args[1]. */
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

/**
 * Script: sets the unit args[0]'s unison phase event to args[1].
 * At each phase change battle_seq.c starts every unit's unison phase script at once and waits for all of them,
 * before running the plain phase scripts one unit at a time.
 */
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

/** Script: sets the unit args[0]'s attack event (its action this turn) to args[1] and clears the attack event flags. */
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

/** Script: sets the unit args[0]'s confusion event (its action when confused) to args[1]. */
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

/**
 * Script: sets the unit args[0]'s ceiling fall event to args[1] (run when it falls from the ceiling,
 * as when a stage hazard knocks it down).
 */
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

/** Script: (re)starts the unit args[0]'s wait event, if it has one. */
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

/** Script: stops the unit args[0]'s running wait event. */
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

/** Script: clears BATTLE_FLAG_GUARD_MOVE once a protecting unit is in place. */
USER_FUNC(btlevtcmd_GuardMoveEnd) {
    _battleWorkPointer->flags &= ~BATTLE_FLAG_GUARD_MOVE;
    return EVT_RETURN_DONE;
}

/** Script: blocks while BATTLE_FLAG_GUARD_MOVE is set (a unit is still moving to protect the target). */
USER_FUNC(btlevtcmd_WaitGuardMove) {
    return (_battleWorkPointer->flags & BATTLE_FLAG_GUARD_MOVE) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** Script: blocks until the active unit's attack event has finished. */
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

/**
 * Script: returns 1 in args[0] when the active unit's attack event has finished,
 * 0 while it still runs (the non-blocking form of btlevtcmd_WaitAttackEnd).
 */
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

/**
 * Script: runs entry args[1] of the unit args[0]'s data table (its own damage, KO, appeal ... events) as a child
 * script for that unit and waits for it; does nothing when the table has no such entry.
 */
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
    return EVT_RETURN_FINISH;
}

/** Script: returns in args[1] whether the unit args[0]'s data table has an entry of type args[2]. */
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

/** Script: returns in args[1] the script of type args[2] from the unit args[0]'s data table, or 0. */
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

/** Script: marks the unit args[0]'s phase event as started (BattlePhaseEventStartDeclare). */
USER_FUNC(btlevtcmd_PhaseEventStartDeclare) {
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    BattlePhaseEventStartDeclare(BattleGetUnitPtr(_battleWorkPointer, id));
    return EVT_RETURN_DONE;
}

/**
 * Script: picks the action a confused unit args[0] takes (BattleSetConfuseAct)
 * and returns its script in args[1]; after 20 failed tries (10 in the demos) it falls back to
 * subsetevt_confuse_flustered.
 */
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
#if VERSION <= VERSION_DEMO_US
        if (i > 10) {
#else
        if (i > 20) {
#endif
            evt = subsetevt_confuse_flustered;
        }
    }
    evtSetValue(event, out, (EvtWord)evt);
    return EVT_RETURN_DONE;
}

/**
 * Script: works out the frame count for Mario's jump from the unit args[0] to x = args[1],
 * z = args[3] (args[2] unused): the count already in args[4] (or, when that is 0,
 * the distance over his move speed) plus 0.15 frames per unit of distance beyond 60.
 * The result goes back to args[4].
 */
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
        n = dist / unit->moveSpeedXZ;
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

/**
 * Script: Mario's jump onto a target: arcs the unit args[0] to (args[1..3]) over args[4] frames (0 = from the
 * move speed) with the jump sounds of sound set args[6].
 * The arc height grows with the distance; type args[5] picks the curve (1 and 2 the normal jump,
 * 2 about 12% higher; 3 and 4 a taller curve, 4 a further 25% higher). Blocks until he lands.
 */
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
        } else {
            BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
            BtlUnit_GetPos(unit, &x, &y, &z);
            BtlUnit_SetMoveCurrentPos(unit, x, y, z);
            unit->mMoveDirection = angleABf(x, z, tx, tz);
            unit->moveDistance = distABf(x, z, tx, tz);
            if (tx - x > 0.0f) {
                unit->moveDirection = 1;
            } else if (tx - x < 0.0f) {
                unit->moveDirection = -1;
            }
            unit->moveFrames = frames;
            if (unit->moveFrames == 0) {
                unit->moveFrames = unit->moveDistance / unit->moveSpeedXZ;
                rem = unit->moveDistance - unit->moveFrames * unit->moveSpeedXZ;
            } else {
                unit->moveSpeedXZ = unit->moveDistance / unit->moveFrames;
                rem = unit->moveDistance - unit->moveFrames * unit->moveSpeedXZ;
            }
            if (unit->moveFrames == 0) {
                return EVT_RETURN_DONE;
            }
            unit->jumpStep.x = (unit->moveTargetPos.x - unit->moveCurrentPos.x) / unit->moveFrames;
            unit->jumpStep.y = (unit->moveTargetPos.y - unit->moveCurrentPos.y) / unit->moveFrames;
            unit->jumpStep.z = (unit->moveTargetPos.z - unit->moveCurrentPos.z) / unit->moveFrames;
            unit->moveSpeedY = 0.0f;
            unit->fallAccel = 3.14159265f / unit->moveFrames;
            unit->moveSpeedXZ = unit->moveSpeedXZ + rem / unit->moveFrames;
            if (type < 3) {
                unit->jumpAngle = 90.0f;
                unit->jumpAngleStep = 360.0f / unit->moveFrames;
                t = unit->moveDistance - 20.0;
                t = t / 6.0;
                t += 47.0;
                unit->moveTotalFrames = t;
                if (type == 2) {
                    unit->moveTotalFrames *= 1.12;
                }
                // FAKE: the same field is stored twice; one store changes the object
                unit->jumpBaseY = 0.0f;
                unit->jumpBaseY = 0.0f;
                sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
                unit->moveSpeedY = unit->moveSpeedY + unit->fallAccel * (0.53 * sn + 1.0);
            } else {
                unit->jumpAngle = 90.0f;
                unit->jumpAngleStep = (f32)(360 / unit->moveFrames);
                t = unit->moveDistance - 20.0;
                t = t / 6.0;
                t += 47.0;
                unit->moveTotalFrames = t;
                if (type == 4) {
                    unit->moveTotalFrames *= 1.25;
                }
                // FAKE: the same field is stored twice; one store changes the object
                unit->jumpBaseY = 0.0f;
                unit->jumpBaseY = 0.0f;
                sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
                unit->moveSpeedY = unit->moveSpeedY + unit->fallAccel * (0.8 * sn + 1.0);
            }
            BtlUnit_LoadSeMode(2, kind, &unit->moveSndTable, &unit->moveSnd);
            if ((EvtWord)unit->moveSnd.name0) {
                Vec pos = becMvVecC(unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
                psndSFXOn_3D(unit->moveSnd.name0, &pos);
            }
        }
    }
    switch ((s8)event->blocked) {
    case 1:
        unit->moveCurrentPos.x += unit->jumpStep.x;
        unit->moveCurrentPos.y += unit->jumpStep.y;
        unit->moveCurrentPos.z += unit->jumpStep.z;
        BtlUnit_GetPos(unit, &x, &y, &z);
        unit->jumpBaseY = y;
        x = unit->moveCurrentPos.x;
        sn = sin(unit->moveSpeedY);
        y = unit->moveTotalFrames * sn + unit->moveCurrentPos.y;
        z = unit->moveCurrentPos.z;
        BtlUnit_SetPos(unit, x, y, z);
        if (unit->moveTargetPos.y > y && unit->moveFrames < 3) {
            y = unit->moveTargetPos.y;
            BtlUnit_SetPos(unit, x, y, z);
        }
        unit->jumpBaseY = y;
        if (type < 3) {
            sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
            unit->moveSpeedY = unit->moveSpeedY + unit->fallAccel * (0.53 * sn + 1.0);
        } else {
            sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
            unit->moveSpeedY = unit->moveSpeedY + unit->fallAccel * (0.8 * sn + 1.0);
        }
        unit->jumpAngle = unit->jumpAngle + unit->jumpAngleStep;
        unit->jumpAngle = reviseAngle(unit->jumpAngle);
        unit->moveFrames--;
        if (unit->moveFrames == 0) {
            y = unit->moveTargetPos.y;
            BtlUnit_SetPos(unit, x, y, z);
            unit->moveSpeedY = -(unit->jumpBaseY - unit->jumpBaseY);
            unit->fallAccel = 1.8f;
            return EVT_RETURN_YIELD;
        }
        break;
    case 2:
        if (!BtlUnit_GetACPossibility(unit)) {
            return EVT_RETURN_DONE;
        }
        unit->moveSpeedY = -(unit->jumpBaseY - unit->jumpBaseY);
        unit->fallAccel = 1.8f;
        unit->moveFrames = 1;
        unit->jumpAngle = 90.0f;
        unit->jumpAngleStep = (f32)(360 / unit->moveFrames);
        unit->moveTotalFrames = __fabs(unit->jumpBaseY - unit->jumpBaseY) / 16.5;
        BtlUnit_GetPos(unit, &unit->moveCurrentPos.x, &unit->moveCurrentPos.y, &unit->moveCurrentPos.z);
        event->blocked = 3;
        break;
    case 3:
        sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
        unit->moveCurrentPos.x = unit->moveCurrentPos.x + unit->moveTotalFrames * sn / 33.0;
        sn = sin(6.28318531f * unit->jumpAngle / 360.0f);
        unit->moveCurrentPos.y = unit->moveCurrentPos.y - unit->moveTotalFrames * sn;
        unit->jumpAngle = unit->jumpAngle + unit->jumpAngleStep;
        unit->jumpAngle = reviseAngle(unit->jumpAngle);
        BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
        unit->moveFrames--;
        if (unit->moveFrames == 0) {
            return EVT_RETURN_YIELD;
        }
        break;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: returns in args[3..5] where the unit args[0] should take off to jump at part args[2] of the unit
 * args[1]: in front of the target's hit position,
 * moved up to the nearest unit in the way and stepped 30 or 60 units back toward the attacker's side.
 */
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
    back = BattleSamplingEnemy(
        &targets, NULL, id, belong,
        TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT,
        TARGET_PROP_ONLY_FRONTMOST, -wp->alliances[belong].attackDirection);
    BattleChoiceSamplingEnemy(&targets, TARGET_WEIGHT_FRONT, &cid, &cpno);
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
            if (back) {
                hx += wp->alliances[belong].attackDirection * -60;
            } else {
                hx += wp->alliances[belong].attackDirection * 60;
            }
        } else {
            if (back) {
                hx += wp->alliances[belong].attackDirection * -30;
            } else {
                hx += wp->alliances[belong].attackDirection * 30;
            }
        }
    } else {
        hx += wp->alliances[belong].attackDirection * 60;
    }
    evtSetValue(event, ox, (s32)hx);
    evtSetValue(event, oy, (s32)hy);
    evtSetValue(event, oz, (s32)hz);
    return EVT_RETURN_DONE;
}

/** Script: sets the move speed of part args[1] of the unit args[0], for parts that move on their own. */
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

/** Script: sets the gravity of part args[1] of the unit args[0] for its jumps. */
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

/**
 * Script: jumps part args[1] of the unit args[0] in an arc to (args[2..4]) over args[5] frames,
 * with the jump sounds of sound set args[6]; blocks until it lands. The part version of btlevtcmd_JumpPosition.
 */
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
        } else if (tx - x < 0.0f) {
            part->moveDirection = -1;
        }
        y = ty - y;
        if (part->moveTotalFrames == 0) {
            part->moveTotalFrames = part->moveDistance / part->moveSpeedXZ;
            part->moveDistance = part->moveDistance - part->moveTotalFrames * part->moveSpeedXZ;
        } else {
            part->moveSpeedXZ = part->moveDistance / part->moveTotalFrames;
            part->moveDistance = part->moveDistance - part->moveTotalFrames * part->moveSpeedXZ;
        }
        if (part->moveTotalFrames == 0) {
            return EVT_RETURN_DONE;
        }
        part->moveSpeedY = y / part->moveTotalFrames + (part->fallAccel * part->moveTotalFrames) / 2.0f;
        part->moveSpeedXZ = part->moveSpeedXZ + part->moveDistance / part->moveTotalFrames;
        BtlUnit_LoadSeMode(2, kind, &part->moveSndTable, &part->moveSnd);
        if ((EvtWord)part->moveSnd.name0) {
            Vec pos = becMvVecD(part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
            psndSFXOn_3D(part->moveSnd.name0, &pos);
        }
    }
    part->moveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->fallAccel;
    if (part->moveSpeedY < 0.0f && part->moveCurrentPos.y < part->moveTargetPos.y) {
        part->moveCurrentPos.y = part->moveTargetPos.y;
    }
    btlMovePos(part->moveSpeedXZ, part->moveAngle, &part->moveCurrentPos.x, &part->moveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
    part->moveTotalFrames--;
    if (part->moveTotalFrames <= 0) {
        BtlUnit_SetPartsPos(part, part->moveTargetPos.x, part->moveTargetPos.y, part->moveTargetPos.z);
        if ((EvtWord)part->moveSnd.name1) {
            Vec pos = becMvVecE(part->moveTargetPos.x, part->moveTargetPos.y, part->moveTargetPos.z);
            psndSFXOn_3D(part->moveSnd.name1, &pos);
        }
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: sets the next jump of part args[1] of the unit args[0]:
 * frame count args[2], speed args[3], gravity args[4].
 */
USER_FUNC(btlevtcmd_JumpPartsSetting) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 frames;
    s32 id;
    s32 pno;
    f32 speed, accel;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    frames = evtGetValue(event, args[2]);
    speed = evtGetFloat(event, args[3]);
    accel = evtGetFloat(event, args[4]);
    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, id), pno);
    part->moveTotalFrames = frames;
    part->moveSpeedXZ = speed;
    part->fallAccel = accel;
    return EVT_RETURN_DONE;
}

/** Script: continues the current jump arc of part args[1] of the unit args[0] until it reaches the floor. */
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
    part->moveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->fallAccel;
    btlMovePos(part->moveSpeedXZ, part->moveAngle, &part->moveCurrentPos.x, &part->moveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
    h = BattleGetFloorHeight(_battleWorkPointer, part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
    if (part->moveCurrentPos.y < h) {
        part->moveCurrentPos.y = h;
        BtlUnit_SetPartsPos(part, part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

void _PartsMoveSoundControl(BattleUnitPart* part);
void _PartsMoveSoundControlEnd(BattleUnitPart* part);

/**
 * Script: drops part args[1] of the unit args[0] to (args[2..4]) over args[5] frames,
 * choosing the gravity to land exactly; the part version of btlevtcmd_FallPosition.
 */
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
        } else if (tx - x < 0.0f) {
            part->moveDirection = -1;
        }
        y = ty - y;
        if (part->moveTotalFrames == 0) {
            if (part->moveSpeedXZ == 0.0f) {
                part->moveSpeedXZ = 1.0f;
            }
            part->moveTotalFrames = part->moveDistance / part->moveSpeedXZ;
            if (part->moveTotalFrames == 0) {
                part->moveTotalFrames = 1;
            }
        } else {
            part->moveSpeedXZ = part->moveDistance / part->moveTotalFrames;
        }
        part->moveSpeedY = 0.0f;
        part->fallAccel = (y / part->moveTotalFrames - part->moveSpeedY) / (-part->moveTotalFrames / 2.0f);
    }
    part->moveCurrentPos.y += part->moveSpeedY;
    part->moveSpeedY -= part->fallAccel;
    btlMovePos(part->moveSpeedXZ, part->moveAngle, &part->moveCurrentPos.x, &part->moveCurrentPos.z);
    BtlUnit_SetPartsPos(part, part->moveCurrentPos.x, part->moveCurrentPos.y, part->moveCurrentPos.z);
    part->moveTotalFrames--;
    if (part->moveTotalFrames <= 0) {
        BtlUnit_SetPartsPos(part, part->moveTargetPos.x, part->moveTargetPos.y, part->moveTargetPos.z);
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script: flies part args[1] of the unit args[0] to (args[2..4]) along a bowed curve;
 * the part version of btlevtcmd_DivePosition (frames args[5], bow args[6], mode args[7], type args[8],
 * sound set args[9]).
 */
USER_FUNC(btlevtcmd_DivePartsPosition) {
    EvtWord* args = event->args;
    s32 id, pno, frames, height, mode, type, kind;
    BattleUnitPart* unit;
    s32 floor;
    f32 tx, ty, tz;
    f32 x, y, z;
    f32 h, a, sx, sz;

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
            unit->moveTotalFrames = unit->moveDistance / unit->moveSpeedXZ;
        } else {
            unit->moveSpeedXZ = unit->moveDistance / unit->moveTotalFrames;
        }
        if (unit->moveTotalFrames == 0) {
            return EVT_RETURN_DONE;
        }
        unit->moveFrame = 0.0f;
        unit->moveAngle = 0.0f;
        if (tx - x > 0.0f) {
            unit->moveDirection = 1;
        } else if (tx - x < 0.0f) {
            unit->moveDirection = -1;
        }
        BtlUnit_LoadSeMode(3, kind, &unit->moveSndTable, &unit->moveSnd);
    }
    unit->moveCurrentPos.x = intpl_sub(mode, unit->moveStartPos.x, unit->moveTargetPos.x, (s32)unit->moveFrame, unit->moveTotalFrames);
    unit->moveCurrentPos.y = intpl_sub(mode, unit->moveStartPos.y, unit->moveTargetPos.y, (s32)unit->moveFrame, unit->moveTotalFrames);
    unit->moveCurrentPos.z = intpl_sub(mode, unit->moveStartPos.z, unit->moveTargetPos.z, (s32)unit->moveFrame, unit->moveTotalFrames);
    if (floor) {
        f32 fy = BattleGetFloorHeight(_battleWorkPointer, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
        if (unit->moveCurrentPos.y < fy) {
            unit->moveFrame = (f32)unit->moveTotalFrames;
            BtlUnit_SetPartsMoveTargetPos(unit, unit->moveCurrentPos.x, fy, unit->moveCurrentPos.z);
        }
    }
    unit->moveFrame += 1.0f;
    if (unit->moveFrame > unit->moveTotalFrames) {
        BtlUnit_SetPartsPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        _PartsMoveSoundControlEnd(unit);
        return EVT_RETURN_DONE;
    }
    if (type == 0) {
        f32 dx = unit->moveTargetPos.x - unit->moveCurrentPos.x, dy = unit->moveTargetPos.y - unit->moveCurrentPos.y, dz = unit->moveTargetPos.z - unit->moveCurrentPos.z;
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
        BtlUnit_SetPartsPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y + h, unit->moveCurrentPos.z);
    } else {
        f32 dx = unit->moveTargetPos.x - unit->moveCurrentPos.x, dy = unit->moveTargetPos.y - unit->moveCurrentPos.y, dz = unit->moveTargetPos.z - unit->moveCurrentPos.z;
        s32 neg;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (unit->moveTargetPos.x - unit->moveStartPos.x > 0.0f) {
            neg = 1;
        } else if (unit->moveTargetPos.x - unit->moveStartPos.x < 0.0f) {
            neg = 0;
        } else {
            if (unit->moveTargetPos.y - unit->moveStartPos.y < 0.0f) {
                neg = 0;
            } else {
                neg = 1;
            }
        }
        if (d == 0.0f) {
            d = 1.0f;
        }
        if (unit->moveDistance == 0.0f) {
            unit->moveDistance = 1.0f;
        }
        h = sinfd(180.0f * (1.0f - d / unit->moveDistance));
        if (neg) {
            a = 360.0f * atan2f_safety(unit->moveTargetPos.x - unit->moveStartPos.x, unit->moveTargetPos.y - unit->moveStartPos.y) / 6.2831855f;
        } else {
            a = 180.0f + 360.0f * atan2f_safety(unit->moveTargetPos.x - unit->moveStartPos.x, unit->moveTargetPos.y - unit->moveStartPos.y) / 6.2831855f;
        }
        sx = h * sinfd(a);
        sz = h * cosfd(180.0f + a);
        if (height == 0) {
            sx = 0.0f;
            sz = sx;
        } else {
            sx = sx * (f32)height;
            sz = sz * (f32)height;
        }
        BtlUnit_SetPartsPos(unit, unit->moveCurrentPos.x + sz, unit->moveCurrentPos.y + sx, unit->moveCurrentPos.z);
    }
    _PartsMoveSoundControl(unit);
    return EVT_RETURN_BLOCK;
}

/**
 * Script: sets the action command difficulty for the unit args[0] to args[1] (badges such as Simplifier and
 * Unsimplifier adjust it).
 */
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

/** Script: returns the current action command difficulty in args[0]. */
USER_FUNC(btlevtcmd_AcGetDifficulty) {
    s32 index = event->args[0];
    evtSetValue(event, index, BattleActionCommandGetDifficulty(_battleWorkPointer));
    return EVT_RETURN_DONE;
}

/** Script: sets the action command flags (acFlags) to the literal args[0]. */
USER_FUNC(btlevtcmd_AcSetFlag) {
    _battleWorkPointer->actionCommands.acFlags = event->args[0];
    return EVT_RETURN_DONE;
}

/** Script: sets the eight action command parameters from args[0..7]; EVT_NULLPTR leaves a parameter unchanged. */
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

/** Script: sets action command parameter args[0] to args[1]. */
USER_FUNC(btlevtcmd_AcSetParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i = evtGetValue(event, args[0]);
    wp->actionCommands.params[i] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script: returns action command parameter args[0] in args[1] (EVT_NULLPTR discards it). */
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

/** Script: sets the four action command gauge parameters from args[0..3]. */
USER_FUNC(btlevtcmd_AcSetGaugeParam) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    wp->actionCommands.gaugeParams[0] = evtGetValue(event, args[0]);
    wp->actionCommands.gaugeParams[1] = evtGetValue(event, args[1]);
    wp->actionCommands.gaugeParams[2] = evtGetValue(event, args[2]);
    wp->actionCommands.gaugeParams[3] = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns action command output value args[0] (what the command produced, such as a power level) in
 * args[1].
 */
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

/** Script: sets action command output value args[0] to args[1]. */
USER_FUNC(btlevtcmd_AcSetOutputParam) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i = evtGetValue(event, args[0]);
    wp->actionCommands.outputParams[i] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script: prepares the action command of type args[1] for the unit args[0] (args[2] and args[3] passed on to
 * BattleActionCommandSetup).
 */
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

/**
 * Script: starts the prepared action command and shows its help text (args[0] nonzero),
 * unless the unit acts confused.
 */
USER_FUNC(btlevtcmd_StartAC) {
    BattleWork* wp = _battleWorkPointer;
    s32 help = evtGetValue(event, event->args[0]);
    BattleActionCommandStart(wp);
    if (help && !(wp->actionCommands.acUnit->tokenFlags & kConfusedAction)) {
        BattleAcHelpSetDispType(2);
    }
    return EVT_RETURN_DONE;
}

/** Script: stops the action command and hides its help text. */
USER_FUNC(btlevtcmd_StopAC) {
    BattleActionCommandStop(_battleWorkPointer);
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}

/**
 * Script: blocks while the action command is still running, then hides its help text;
 * read the outcome with btlevtcmd_GetResultAC.
 */
USER_FUNC(btlevtcmd_ResultAC) {
    BattleWork* wp = _battleWorkPointer;
    BattleActionCommandResult(wp);
    if (wp->actionCommands.result & AC_RESULT_RUNNING) {
        return EVT_RETURN_BLOCK;
    }
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}

/** Script: checks the guard and Superguard timing of the unit args[0] against the attack weapon args[1]. */
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

/** Script: returns the action command result flags in args[0] (bit 2 = success). */
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
    // Both ids mean "no unit" here, not the system unit.
    if (id != -1 && id != UNIT_ID_SYSTEM) {
        unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    }
    value = evtGetValue(event, args[1]);
    index = args[2];
    evtSetValue(event, index, BattleActionCommandGetPrizeLv(wp, unit, value));
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the result of the last guard check (none, guard or Superguard). */
USER_FUNC(btlevtcmd_GetResultACDefence) {
    s32 index = event->args[0];
    evtSetValue(event, index, BattleActionCommandGetDefenceResult());
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[0] how many times the action command was completed (for repeated presses and multi-hit
 * moves).
 */
USER_FUNC(btlevtcmd_GetResultCountAC) {
    evtSetValue(event, event->args[0], _battleWorkPointer->actionCommands.resultCount);
    return EVT_RETURN_DONE;
}

/**
 * Script: reports an action command result args[1] for the weapon args[0] to the audience (-1 = failed);
 * also counts Mario's successes and failures (capped at 200) for the audience.
 */
USER_FUNC(btlevtcmd_AudienceDeclareACResult) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleWeapon* weapon;
    s32 result;
    u8* ac;
    weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    result = evtGetValue(event, args[1]);
    // FAKE: ac[0] / ac[1] are marioAcSuccess / marioAcFailed through one pointer; the field names change the object
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

/**
 * Script: reports a Stylish result: on success (args[1]) takes the audience bonus from the weapon args[0],
 * cheers the audience and shows a "Nice" sign at the unit offset by (args[2..4]);
 * on failure the audience boos and the miss sound plays.
 */
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
    aud = (u8*)&wp->impendingBonuses.stylishCommandMultiplier;
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
        eff = effNiceEntry(NICE_EFFECT_ACROBAT, pos.x + x, pos.y + y, pos.z + z);
        ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
    } else {
        BattleAudience_Case_AcrobatBad();
        psndSFXOn_3D("SFX_BTL_ACROBAT_MISS1", &pos);
    }
    return EVT_RETURN_DONE;
}

extern void psndSFX_pit(s32 id, s32 pitch);

#if VERSION >= VERSION_US
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
/**
 * Script: shows the action command rating sign args[0] (0-2 Nice at growing sizes, then Good, Great, Wonderful,
 * Excellent) above the position args[1..3], kept on screen, with its sound.
 * The US release sizes the sign by a per-language width table (ac_success_width_tbl).
 */
#if VERSION == VERSION_DEMO_JP
/**
 * Script: shows the action command rating sign args[0] (0-2 Nice at growing sizes, then Good, Great, Wonderful,
 * Excellent) above the position args[1..3], kept on screen, with its sound.
 * The US release sizes the sign by a per-language width table (ac_success_width_tbl).
 */
USER_FUNC(btlevtcmd_ACSuccessEffect) {
    EvtWord* args = event->args;
    s32 type;
    f32 scale;
    f32 w;
    Vec pos;
    f32 h;
    const char* name;
    s32 eff;
EffectEntry* entry;

    type = evtGetValue(event, args[0]);
    scale = 0.75f;
    pos.x = (f32)evtGetValue(event, args[1]);
    pos.y = (f32)evtGetValue(event, args[2]);
    pos.z = (f32)evtGetValue(event, args[3]);
    switch (type) {
        case 0:
            scale *= 0.3f;
            name = "SFX_SYSTEM_NICE_25PERCENT1";
            w = 90.0f;
            eff = 0;
            break;
        case 1:
            scale *= 0.6f;
            name = "SFX_SYSTEM_NICE_50PERCENT1";
            w = 90.0f;
            eff = 0;
            break;
        case 2:
            scale *= 1.2f;
            name = "SFX_SYSTEM_NICE1";
            w = 90.0f;
            eff = 0;
            break;
        case 3:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GOOD1";
            w = 90.0f;
            eff = 1;
            break;
        case 4:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GREAT1";
            w = 120.0f;
            eff = 2;
            break;
        case 5:
            name = "SFX_SYSTEM_WONDERFUL1";
            w = 150.0f;
            eff = 3;
            break;
        case 6:
            name = "SFX_SYSTEM_EXCELLENT1";
            w = 180.0f;
            eff = 4;
            break;
default: return EVT_RETURN_DONE;
    }
    w *= scale;
    h = w / 2.0f;
    if (pos.x + h > 160.0f) {
        pos.x = 160.0f - h;
}
    if (pos.y < 80.0f) {
        entry = effNiceEntry(eff, pos.x - h, 50.0f + pos.y, pos.z);
    } else {
        entry = effNiceEntry(eff, pos.x - h, pos.y - 50.0f, pos.z);
    }
    ((NiceEffectWork*)entry->userdata)->overallScale = scale;
    psndSFX_pit(psndSFXOn_3D(name, &pos), 0);
    return EVT_RETURN_DONE;
}
#elif VERSION == VERSION_DEMO_US
/**
 * Script: shows the action command rating sign args[0] (0-2 Nice at growing sizes, then Good, Great, Wonderful,
 * Excellent) above the position args[1..3], kept on screen, with its sound.
 * The US release sizes the sign by a per-language width table (ac_success_width_tbl).
 */
USER_FUNC(btlevtcmd_ACSuccessEffect) {
    EvtWord* args = event->args;
    s32 type;
    f32 scale;
    f32 w;
    Vec pos;
    f32 h;
    const char* name;
    s32 eff;
    EffectEntry* entry;

    type = evtGetValue(event, args[0]);
    scale = 0.65f;
    pos.x = (f32)evtGetValue(event, args[1]);
    pos.y = (f32)evtGetValue(event, args[2]);
    pos.z = (f32)evtGetValue(event, args[3]);
    switch (type) {
        case 0:
            w = 120.0f;
            scale *= 0.3f;
            name = "SFX_SYSTEM_NICE_25PERCENT1";
            eff = 0;
            break;
        case 1:
            scale *= 0.6f;
            name = "SFX_SYSTEM_NICE_50PERCENT1";
            w = 120.0f;
            eff = 0;
            break;
        case 2:
            scale *= 1.2f;
            name = "SFX_SYSTEM_NICE1";
            w = 120.0f;
            eff = 0;
            break;
        case 3:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GOOD1";
            w = 120.0f;
            eff = 1;
            break;
        case 4:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GREAT1";
            w = 150.0f;
            eff = 2;
            break;
        case 5:
            name = "SFX_SYSTEM_WONDERFUL1";
            w = 225.0f;
            eff = 3;
            break;
        case 6:
            name = "SFX_SYSTEM_EXCELLENT1";
            w = 225.0f;
            eff = 4;
            break;
        default:
            return EVT_RETURN_DONE;
    }
    w += 20.0f;
    h = 0.5f * w;
    if (pos.y < 80.0f) {
        entry = effNiceEntry(eff, pos.x - h, 50.0f + pos.y, pos.z);
    } else {
        entry = effNiceEntry(eff, pos.x - h, pos.y - 50.0f, pos.z);
    }
    ((NiceEffectWork*)entry->userdata)->overallScale = scale;
    psndSFX_pit(psndSFXOn_3D(name, &pos), 0);
    return EVT_RETURN_DONE;
}
#else
/**
 * Script: shows the action command rating sign args[0] (0-2 Nice at growing sizes, then Good, Great, Wonderful,
 * Excellent) above the position args[1..3], kept on screen, with its sound.
 * The US release sizes the sign by a per-language width table (ac_success_width_tbl).
 */
USER_FUNC(btlevtcmd_ACSuccessEffect) {
    EvtWord* args = event->args;
    s32 type;
    f32 scale;
    f32 w;
    Vec pos;
    f32 h;
    const char* name;
    s32 eff;
#if VERSION >= VERSION_US
    s32 lang;
#endif
    EffectEntry* entry;

    type = evtGetValue(event, args[0]);
    scale = 0.75f;
#if VERSION >= VERSION_US
    switch (gp->language) {
        case 1:
            lang = 1;
            break;
        case 2:
            lang = 2;
            break;
        case 3:
            lang = 3;
            break;
        case 4:
            lang = 4;
            break;
        case 5:
            lang = 5;
            break;
        case 6:
            lang = 6;
            break;
        case 0:
        default:
            lang = 0;
            break;
    }
#endif
    pos.x = (f32)evtGetValue(event, args[1]);
    pos.y = (f32)evtGetValue(event, args[2]);
    pos.z = (f32)evtGetValue(event, args[3]);
    switch (type) {
        case 0:
            scale *= 0.3f;
            name = "SFX_SYSTEM_NICE_25PERCENT1";
#if VERSION >= VERSION_US
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][1];
            eff = 0;
            break;
        case 1:
            scale *= 0.6f;
            name = "SFX_SYSTEM_NICE_50PERCENT1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][2];
            eff = 0;
            break;
        case 2:
            scale *= 1.2f;
            name = "SFX_SYSTEM_NICE1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][3];
            eff = 0;
            break;
        case 3:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GOOD1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][4];
            eff = 1;
            break;
        case 4:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GREAT1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][5];
            eff = 2;
            break;
        case 5:
            name = "SFX_SYSTEM_WONDERFUL1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][6];
            eff = 3;
            break;
        case 6:
            name = "SFX_SYSTEM_EXCELLENT1";
            w = ac_success_width_tbl[lang][0] * ac_success_width_tbl[lang][7];
#else
            w = 90.0f;
            eff = 0;
            break;
        case 1:
            scale *= 0.6f;
            name = "SFX_SYSTEM_NICE_50PERCENT1";
            w = 90.0f;
            eff = 0;
            break;
        case 2:
            scale *= 1.2f;
            name = "SFX_SYSTEM_NICE1";
            w = 90.0f;
            eff = 0;
            break;
        case 3:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GOOD1";
            w = 90.0f;
            eff = 1;
            break;
        case 4:
            scale *= 1.2f;
            name = "SFX_SYSTEM_GREAT1";
            w = 120.0f;
            eff = 2;
            break;
        case 5:
            name = "SFX_SYSTEM_WONDERFUL1";
            w = 150.0f;
            eff = 3;
            break;
        case 6:
            name = "SFX_SYSTEM_EXCELLENT1";
            w = 180.0f;
#endif
            eff = 4;
            break;
        default: return EVT_RETURN_DONE;
    }
    w *= scale;
    h = w / 2.0f;
    if (pos.x + h > 160.0f) {
        pos.x = 160.0f - h;
#if VERSION >= VERSION_US
    }
    if (pos.x - h < -180.0f) {
        pos.x = -180.0f + h;
#endif
    }
    if (pos.y < 80.0f) {
        entry = effNiceEntry(eff, pos.x - h, 50.0f + pos.y, pos.z);
    } else {
        entry = effNiceEntry(eff, pos.x - h, pos.y - 50.0f, pos.z);
    }
    ((NiceEffectWork*)entry->userdata)->overallScale = scale;
    psndSFX_pit(psndSFXOn_3D(name, &pos), 0);
    return EVT_RETURN_DONE;
}
#endif

/** Script: sets (args[0] nonzero) or clears the flags args[1] in the running timing action command's work. */
USER_FUNC(btlevtcmd_ac_timing_flag_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    BattleWork* wp;
    u32 flags = args[1];
    wp = _battleWorkPointer;
    if (on) {
        ((TimingAWork*)wp->actionCommands.work)->flags |= flags;
    } else {
        ((TimingAWork*)wp->actionCommands.work)->flags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the frame on which the running timing action command succeeds. */
USER_FUNC(btlevtcmd_ac_timing_get_success_frame) {
    s32 index = event->args[0];
    s32 frame = battleAcTimingA_GetSuccessFrame(_battleWorkPointer);
    evtSetValue(event, index, frame);
    return EVT_RETURN_DONE;
}

/** Script: shows the action command help text of the weapon args[0]. */
USER_FUNC(btlevtcmd_ACHelpSet) {
    BattleWeapon* weapon = (BattleWeapon*)evtGetValue(event, event->args[0]);
    BattleAcHelpSetHelp(weapon->acHelpMsg);
    BattleAcHelpSetDispType(2);
    return EVT_RETURN_DONE;
}

/** s32* battleId, s32* windowStartFrame, s32* windowEndFrame, s32* endFrame, s32* earlyFrames */
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

/** s32 resultRetIndex, s32 frameRetIndex (can be EVTDAT_POINTER_MAX to skip) */
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

/**
 * Script: swaps the model of part args[1] of the unit args[0] for the animation group args[2],
 * loading it asynchronously and blocking until it is ready.
 */
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

/** Script: plays the named animation args[2] on part args[1] of the unit args[0] (manual pose). */
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

/**
 * Script: plays the named animation args[2] on part args[1] of the unit args[0] straight away, restarting it,
 * and picks the plain or translucent (Invisible) draw type.
 */
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
    part->animFlags &= ~PART_ANIM_COMMAND_POSE;
    part->animFlags &= ~PART_ANIM_BURNT;
    animPoseSetAnim(part->poseId, name, TRUE);
    part->poseType = 1;
    if (BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
        part->poseType = 7;
    }
    return EVT_RETURN_DONE;
}

/** Script: blocks until the current animation of part args[1] of the unit args[0] has played through once. */
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

/**
 * Script: blocks until the looping animation of part args[1] of the unit args[0] starts its next loop;
 * returns at once when the animation is paused.
 */
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

/** Script: sets which part of the unit args[0] speaks in battle conversations (args[1]). */
USER_FUNC(btlevtcmd_SetTalkBodyId) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, body;

    id = evtGetValue(event, args[0]);
    body = evtGetValue(event, args[1]);
    BattleGetUnitPtr(wp, BattleTransID(event, id))->talkPartId = body;
    return EVT_RETURN_DONE;
}

/** Script: sets the animation the unit args[0] plays while talking to the name args[1]. */
USER_FUNC(btlevtcmd_SetTalkPose) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    EvtWord name = args[1];
    BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id))->talkPose = (void*)name;
    return EVT_RETURN_DONE;
}

/** Script: sets the animation the unit args[0] holds between lines of a conversation to the name args[1]. */
USER_FUNC(btlevtcmd_SetStayPose) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    EvtWord name = args[1];
    BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id))->stayPose = (void*)name;
    return EVT_RETURN_DONE;
}

/** Script: sets the unit args[0]'s talk animation from pose type args[1] of its body part. */
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

/** Script: sets the unit args[0]'s conversation stay animation from pose type args[1] of its body part. */
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

/**
 * Script: returns 1 in args[4] when pose types args[2]
 * and args[3] of part args[1] of the unit args[0] name different animations;
 * scripts use it to speed up a walk that shares the run animation.
 */
USER_FUNC(btlevtcmd_AnimeCmpPoseType) {
    EvtWord* args = event->args;
    s32 id, pno;
    s32 t1, t2;
    BattleUnitPart* part;
    s32 index;
    const char* n1;
    const char* n2;

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

/** Script: hands part args[1] of the unit args[0] back to automatic, status-based pose selection. */
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

/** Script: replaces the pose table of part args[1] of the unit args[0] with args[2]. */
USER_FUNC(btlevtcmd_AnimeSetPoseTable) {
    EvtWord* args = event->args;
    BattleUnitPart* part;
    s32 id, pno;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    part->poseTable = (PoseTableEntry*)args[2];
    return EVT_RETURN_DONE;
}

/**
 * Script: replaces the pose sound table (sounds tied to animation frames) of part args[1] of the unit args[0]
 * with args[2].
 */
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

/**
 * Script: returns 1 in args[3] when the pose table of part args[1] of the unit args[0] defines pose type args[2]
 * (searching only the section of the table that type belongs to).
 */
USER_FUNC(btlevtcmd_CheckDefinePosetype) {
    EvtWord* args = event->args;
    s32 type;
    BattleUnitPart* part;
    s32 id, pno;
    s32 out;
    PoseTableEntry* table;
    s32 res;
    s32 end;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    pno = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, pno);
    type = evtGetValue(event, args[2]);
    out = args[3];
    table = part->poseTable;
    res = 0;
    if (type <= POSE_TYPE_NORMAL) {
        end = POSE_TYPE_NORMAL;
    } else if (type < POSE_TYPE_WALK) {
        end = POSE_TYPE_DAMAGE;
    } else if (type < POSE_TYPE_MAX) {
        end = POSE_TYPE_ACTION;
    } else {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    for (; table; table++) {
        if (table->poseType >= POSE_TYPE_MAX) {
            res = 0;
            break;
        }
        if (table->poseType == type) {
            res = 1;
            break;
        }
        if (table->poseType == end) {
            res = 0;
            break;
        }
    }
    evtSetValue(event, out, res);
    return EVT_RETURN_DONE;
}

/** Script: sets the scripted animation speed multiplier of part args[1] of the unit args[0] to args[2]. */
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

/**
 * Script: sets the paper effect group args[2] (such as "p_b_st") on part args[1] of the unit args[0],
 * with mode args[3].
 */
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

/**
 * Script: plays the paper effect animation args[2] (frozen, dodge and similar looks) on part args[1] of the unit
 * args[0].
 */
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
    btlDispChangePeraAnime(part, (const char*)value);
    return EVT_RETURN_DONE;
}

/** Script: sets (args[3] nonzero) or clears the animation flags args[2] on part args[1] of the unit args[0]. */
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
    } else {
        part->animFlags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: returns 1 in args[1] when the unit args[0] can show guard motions:
 * not asleep, stopped, frozen, tiny, KO'd, invisible and the like, and not already defending.
 */
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
    if (BtlUnit_CheckStatusFlag(unit, UNIT_STATUS_FLAG_FLIPPED)) {
        enable = FALSE;
    }
    evtSetValue(event, index, enable);
    return EVT_RETURN_DONE;
}

/** Script: returns TRUE in args[2] when the unit args[0] has the status args[1] (StatusEffectType). */
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
    } else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: gives the unit args[0] the status args[1] for args[2] turns at strength args[3]
 * when args[4] is nonzero, or removes it.
 */
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
    } else {
        BtlUnit_SetStatus(unit, type, 0, 0);
    }
    return EVT_RETURN_DONE;
}

/** Script: removes every status from the unit args[0]. */
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

/** Script: clears the status flags args[1] on the unit args[0]. */
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

/** Script: returns TRUE in args[1] when the unit args[0] can act (no status such as sleep, stop or freeze holds it). */
USER_FUNC(btlevtcmd_CheckActStatus) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 index = args[1];
    if (BtlUnit_CanActStatus(BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id)))) {
        evtSetValue(event, index, TRUE);
    } else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns TRUE in args[1] when the unit args[0] is able to take part in action commands (guards included). */
USER_FUNC(btlevtcmd_GetACPossibility) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 index = args[1];
    if (BtlUnit_GetACPossibility(BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id)))) {
        evtSetValue(event, index, TRUE);
    } else {
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
    } else {
        evtSetValue(event, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script: sets the unit flags args[1] on the unit args[0]. */
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

/** Script: clears the unit flags args[1] on the unit args[0]. */
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

/**
 * Script: queues an after-reaction for the unit args[0] with value args[1] (BattleAfterReactionEntry),
 * run once the current attack is over.
 */
USER_FUNC(btlevtcmd_AfterReactionEntry) {
    EvtWord* args = event->args;
    s32 id, value;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    value = evtGetValue(event, args[1]);
    BattleAfterReactionEntry(id, value);
    return EVT_RETURN_DONE;
}

/** Script: returns 1 in args[1] when the weapon args[0] hits several targets, 0 when it is single-target. */
USER_FUNC(btlevtcmd_CheckWeaponPluralTarget) {
    EvtWord* args = event->args;
    BattleWeapon* weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    s32 out;
    out = args[1];
    if (weapon->targetClassFlags & TARGET_CLASS_SINGLE) {
        evtSetValue(event, out, 0);
    } else {
        evtSetValue(event, out, 1);
    }
    return EVT_RETURN_DONE;
}

/** Script: makes the unit args[0] pay the FP (and Star Power) cost of the weapon args[1]. */
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

/** Script: makes the unit args[0] pay the cost of the weapon chosen in the command menu. */
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

/** Script: returns in args[1] the action command level (acLevel) of the weapon args[0]. */
USER_FUNC(btlevtcmd_GetWeaponActionLv) {
    EvtWord* args = event->args;
    BattleWeapon* weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (u8)weapon->acLevel);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the action command level of the weapon chosen in the command menu. */
USER_FUNC(btlevtcmd_CommandGetWeaponActionLv) {
    s32 index = event->args[0];
    BattleWeapon* weapon = BattleGetSelectWeapon(_battleWorkPointer);
    evtSetValue(event, index, (u8)weapon->acLevel);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[1] the weapon the unit args[0] is using:
 * the one chosen in the menu for Mario and the partners, its held item's battle data for an enemy.
 */
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
    } else {
        weapon = itemDataTable[(s32)unit->heldItem].battle_params;
    }
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the item id of the weapon args[1] (0 for moves that are not items). */
USER_FUNC(btlevtcmd_CommandGetWeaponItemId) {
    EvtWord* args = event->args;
    s32 index = args[0];
    BattleWeapon* weapon = (BattleWeapon*)evtGetValue(event, args[1]);
    evtSetValue(event, index, weapon->itemId);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns the HP, FP and Star Power restored by the item args[0] in args[1], args[2] and args[3];
 * EVT_NULLPTR skips one.
 */
USER_FUNC(btlevtcmd_GetItemRecoverParam) {
    EvtWord* args = event->args;
    s32 id, a, b, c;
    ItemData* item;

    id = evtGetValue(event, args[0]);
    a = args[1];
    b = args[2];
    c = args[3];
    item = &itemDataTable[id];
    if (a != EVTDAT_POINTER_MAX) {
        evtSetValue(event, a, item->hp_restored);
    }
    if (b != EVTDAT_POINTER_MAX) {
        evtSetValue(event, b, item->fp_restored);
    }
    if (c != EVTDAT_POINTER_MAX) {
        evtSetValue(event, c, item->sp_restored);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: rolls the stage hazards an attack with the weapon args[0] can set off, from its stageEffectRates:
 * knocking down the background (A1, A2, then B), turning or firing the stage nozzles,
 * dropping the iron frame and the objects above it. The battle debug flags can force each one.
 */
USER_FUNC(btlevtcmd_WeaponAftereffect) {
    BattleWork* wp = _battleWorkPointer;
    BattleWeapon* w = (BattleWeapon*)evtGetValue(event, event->args[0]);
    s32 a1, a2, b;
    s32 r;

    b = 0;
    a2 = 0;
    a1 = 0;
    r = irand(w->stageEffectRates[0] + w->stageEffectRates[1] + w->stageEffectRates[2] + w->stageEffectRates[3]);
    if ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_DESTROY_A1) {
        a1 = 1;
    }
    if ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_DESTROY_A2) {
        a2 = 1;
    }
    r -= w->stageEffectRates[0];
    if (r < 0) {
        a2 = 1;
        a1 = 1;
    } else {
        r -= w->stageEffectRates[1];
        if (r < 0) {
            a1 = 1;
        } else {
            r -= w->stageEffectRates[2];
            if (r < 0) {
                a2 = 1;
            }
        }
    }
    if (wp->stageObjectWork.flags & STAGE_FLAG_NO_BG_A2_DESTROY) {
        a2 = 0;
    }
    if (wp->stageObjectWork.flags & STAGE_FLAG_NO_BG_A1_DESTROY) {
        a1 = 0;
    }
    if ((wp->stageObjectWork.flags & STAGE_FLAG_NO_BG_A1_DESTROY) && !(wp->stageObjectWork.flags & STAGE_FLAG_NO_BG_B_DESTROY)) {
        if (irand(100) < w->stageEffectRates[4] || ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_DESTROY_B)) {
            b = 1;
            if (!(wp->stageObjectWork.flags & STAGE_FLAG_NO_BG_A2_DESTROY)) {
                a2 = 1;
            }
        }
    }
    if (a1) {
        BattleStage_DestroyA1();
    }
    if (a2) {
        BattleStage_DestroyA2();
    }
    if (b) {
        BattleStage_DestroyB();
    }
    if (irand(100) < w->stageEffectRates[5] || ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_NOZZLE_DIR_CHANGE)) {
        BattleStage_NozzleDirChangeCheck();
    }
    if (irand(100) < w->stageEffectRates[6] || ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_NOZZLE_WORK)) {
        BattleStage_NozzleWorkCheck(1);
    }
    if (irand(100) < w->stageEffectRates[7] || ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_IRON_FRAME_FALL)) {
        BattleStage_IronFrameFallCheck();
    }
    if (wp->stageObjectWork.flags & STAGE_FLAG_NO_IRON_FRAME) {
        if ((u32)wp->debug.flags & BATTLE_DEBUG_FORCE_OBJECT_FALL) {
            BattleStage_ObjectFallCheck();
        } else if (irand(100) < w->stageEffectRates[8]) {
            BattleStage_ObjectFallCheck();
        }
    }
    return EVT_RETURN_DONE;
}

/** Script: makes the unit args[0] protect the unit args[1] (sets its protectId). */
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

/**
 * Script: returns in args[1] the id of the shell shield unit (Koops' Shell Shield) guarding the unit args[0],
 * or -1.
 */
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
    } else {
        evtSetValue(event, index, -1);
    }
    return EVT_RETURN_DONE;
}

/** Script: ends the battle early: args[0] 0 = Mario ran away, 1 = interrupted by a script. */
USER_FUNC(btlevtcmd_InterruptStop) {
    s32 type = evtGetValue(event, event->args[0]);
    BattleWork* wp = _battleWorkPointer;

    if (isFirstCall) {
        switch (type) {
            case 0:
                wp->flags |= BATTLE_FLAG_ESCAPED;
                break;
            case 1:
                wp->flags |= BATTLE_FLAG_INTERRUPT_STOP;
                break;
        }
    }
    return EVT_RETURN_DONE;
}

/** Script: re-sorts the order in which units act (after a partner switch or a swap of positions). */
USER_FUNC(btlevtcmd_SortPhaseMoveTable) {
    battleSortPhaseMoveTable(_battleWorkPointer, -1);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns the stage size in args[0..2] (x, y, z); EVT_NULLPTR skips one.
 * Units fleeing or blown away move to the x edge.
 */
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

/** Script: returns the size of the audience area in args[0..2]; EVT_NULLPTR skips one. */
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

/** Script: returns in args[1] the floor height of the stage at x = args[0], z = args[2]. */
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

/**
 * Script: sets the unit args[0]'s current movement sound directly:
 * sounds args[1] and args[2] with timing values args[3..7] (BtlUnit_SetSeMode).
 */
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

/**
 * Script: sets the unit args[0]'s running footsteps:
 * sounds args[1] and args[2] alternating, after args[3] frames, every args[4] and args[5] frames.
 * One sound alone plays as a continuous loop.
 */
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

/** Script: sets the running footsteps of part args[1] of the unit args[0], like btlevtcmd_SetRunSound. */
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

/** Script: sets the unit args[0]'s walking footsteps, laid out like btlevtcmd_SetRunSound. */
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

/** Script: sets the walking footsteps of part args[1] of the unit args[0]. */
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

/** Script: sets the unit args[0]'s flying (dive move) sounds, laid out like btlevtcmd_SetRunSound. */
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

/** Script: sets the flying sounds of part args[1] of the unit args[0]. */
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

/** Script: sets the unit args[0]'s jump sounds: args[1] at take-off and args[2] on landing. */
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

/** Script: sets the take-off and landing sounds of part args[1] of the unit args[0] (args[2], args[3]). */
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

/** Per-frame movement sound for a moving unit, at its position. */
void _UnitMoveSoundControl(BattleUnit* unit) {
    f32 x, y, z;
    BtlUnit_GetPos(unit, &x, &y, &z);
    _MoveSoundControlCore(unit, &unit->moveSnd, NULL, x, y, z);
}

/** Per-frame movement sound for a moving part, at its position. */
void _PartsMoveSoundControl(BattleUnitPart* part) {
    f32 x, y, z;
    BtlUnit_GetPartsPos(part, &x, &y, &z);
    _MoveSoundControlCore(part->owner, &part->moveSnd, part, x, y, z);
}

/**
 * Plays the movement sounds while a unit or part moves: a single sound loops and follows it;
 * two sounds alternate as footsteps on their own timers.
 */
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
            } else {
                BtlUnit_GetPos(unit, &cur.x, &cur.y, &cur.z);
            }
            psndSFX_pos(work->sfxId, &cur);
        } else if (work->timer >= 0) {
            work->timer--;
            if (work->timer < 0) {
                work->count++;
                if (work->count & 1) {
                    name = work->name0;
                    work->timer = work->period0;
                } else {
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

/** Stops a unit's looping movement sound when its move ends. */
void _UnitMoveSoundControlEnd(BattleUnit* unit) {
    _MoveSoundControlEndCore(unit, &unit->moveSnd);
}

/** Stops a part's looping movement sound when its move ends. */
void _PartsMoveSoundControlEnd(BattleUnitPart* part) {
    _MoveSoundControlEndCore(part->owner, &part->moveSnd);
}

/** Mario's voice clips for acrobatic moves; btlevtcmd_snd_voice type 1 plays one at random. */
const char* _mario_acrobat_voice_table[5] = {
    "SFX_VOICE_MARIO_AC3_1", "SFX_VOICE_MARIO_AC3_2", "SFX_VOICE_MARIO_AC3_3",
    "SFX_VOICE_MARIO_AC3_4", "SFX_VOICE_MARIO_AC3_5",
};
/**
 * Mario's voice clips for ordinary attacks, two sets of five;
 * btlevtcmd_snd_voice type 2 plays one at random.
 */
const char* _mario_attack_voice_table[10] = {
    "SFX_VOICE_MARIO_AC1_1", "SFX_VOICE_MARIO_AC1_2", "SFX_VOICE_MARIO_AC1_3",
    "SFX_VOICE_MARIO_AC1_4", "SFX_VOICE_MARIO_AC1_5", "SFX_VOICE_MARIO_AC2_1",
    "SFX_VOICE_MARIO_AC2_2", "SFX_VOICE_MARIO_AC2_3", "SFX_VOICE_MARIO_AC2_4",
    "SFX_VOICE_MARIO_AC2_5",
};
/** Stops the looping movement sound in the work, if one is playing. */
void _MoveSoundControlEndCore(BattleUnit* unit, BecSndWork* work) {
    if ((u32)work->sfxId != 0xFFFFFFFF) {
        psndSFXOff(work->sfxId);
        work->sfxId = -1;
    }
}

/**
 * Script: plays the sound args[1] at the unit args[0] with volume args[2] and pitch args[3];
 * the sound id goes to args[4] unless it is EVT_NULLPTR.
 */
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
    if (index != EVTDAT_POINTER_MAX) {
        evtSetValue(event, index, ret);
    }
    return EVT_RETURN_DONE;
}

/** Script: like btlevtcmd_snd_se, at the unit's position offset by (args[4..6]); the sound id goes to args[7]. */
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

/**
 * Script: plays a random Mario voice clip on the unit args[0]:
 * args[1] 1 for a Stylish cheer (of 5), 2 for an attack grunt (of 10).
 */
USER_FUNC(btlevtcmd_snd_voice) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, type;
    BattleUnit* unit;
    s32 r;

    id = evtGetValue(event, args[0]);
    type = evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    switch (type) {
        case 1:
            r = irand(5);
            BtlUnit_snd_se(unit, _mario_acrobat_voice_table[r], EVTDAT_POINTER_MAX, 0);
            break;
        case 2:
            r = irand(10);
            BtlUnit_snd_se(unit, _mario_attack_voice_table[r], EVTDAT_POINTER_MAX, 0);
            break;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[1] the attack script of the item the enemy args[0] should use now,
 * or 0 (see BattleEnemyUseItemCheck).
 */
USER_FUNC(btlevtcmd_EnemyItemUseCheck) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index;
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    index = args[1];
    evtSetValue(event, index, (EvtWord)BattleEnemyUseItemCheck(unit));
    return EVT_RETURN_DONE;
}

/** Script: returns TRUE in args[2] when a button in the mask args[1] was just pressed on controller args[0]. */
USER_FUNC(btlevtcmd_PadCheckTrigger) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    s32 mask = args[1];
    // FAKE: args is reused to hold the result index; a separate index local changes the object
    args = (EvtWord*)args[2];
    if (BattlePadMultiCheckTrigger(chan, mask)) {
        evtSetValue(event, (EvtWord)args, TRUE);
    } else {
        evtSetValue(event, (EvtWord)args, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script: returns TRUE in args[2] when a button in the mask args[1] is held on controller args[0]. */
USER_FUNC(btlevtcmd_PadCheckNow) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    s32 mask = args[1];
    // FAKE: args is reused to hold the result index; a separate index local changes the object
    args = (EvtWord*)args[2];
    if (BattlePadMultiCheckNow(chan, mask)) {
        evtSetValue(event, (EvtWord)args, TRUE);
    } else {
        evtSetValue(event, (EvtWord)args, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script: turns the Star Power bonuses earned by the current action into audience Star Power gain and clears them. */
USER_FUNC(btlevtcmd_InviteApInfoReport) {
    StarPowerInfo* info = &_battleWorkPointer->impendingBonuses;
    BattleAudience_ApRecoveryBuild(info);
    memset(info, 0, sizeof(StarPowerInfo));
    return EVT_RETURN_DONE;
}

/**
 * Script: slides the battle stage's background objects off screen (args[0] = 0: sideways 450 units for the back
 * layers, up 260 for the top layer) or back into place (nonzero) over 45 frames; blocks until done.
 * Used by Star Power special moves that need a clear stage.
 */
#if VERSION == VERSION_EU
USER_FUNC(btlevtcmd_SetCounterAcCallBack) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    unit->hitEventCode = (void*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script: slides the battle stage's background objects off screen (args[0] = 0: sideways 450 units for the back
 * layers, up 260 for the top layer) or back into place (nonzero) over 45 frames; blocks until done.
 * Used by Star Power special moves that need a clear stage.
 */
#endif
#if VERSION <= VERSION_DEMO_US
USER_FUNC(btlevtcmd_SetCounterAcCallBack) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    unit->hitEventCode = (void*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}
#endif
/**
 * Script: slides the battle stage's background objects off screen (args[0] = 0: sideways 450 units for the back
 * layers, up 260 for the top layer) or back into place (nonzero) over 45 frames; blocks until done.
 * Used by Star Power special moves that need a clear stage.
 */
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
    mode = on ? EASING_CUBIC_OUT : EASING_CUBIC_IN;
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0) {
            switch (obj->data->layer) {
                case 0:
                case 1:
                    if (on) {
                        if (obj->position.x >= 0.0f) {
                            val = intplGetValue(mode, 450.0f, 0.0f, event->userdata[0], 45);
                        } else {
                            val = -intplGetValue(mode, 450.0f, 0.0f, event->userdata[0], 45);
                        }
                    } else {
                        if (obj->position.x >= 0.0f) {
                            val = intplGetValue(mode, 0.0f, 450.0f, event->userdata[0], 45);
                        } else {
                            val = -intplGetValue(mode, 0.0f, 450.0f, event->userdata[0], 45);
                        }
                    }
                    obj->positionOffset2.x = val;
                    break;
                case 2:
                    if (on) {
                        val = intplGetValue(mode, 260.0f, 0.0f, event->userdata[0], 45);
                    } else {
                        val = intplGetValue(mode, 0.0f, 260.0f, event->userdata[0], 45);
                    }
                    obj->positionOffset2.y = val;
                    break;
            }
        }
    }
    return !(event->userdata[0] < 45);
}

/** Script: sets (args[2] nonzero) or clears the flags args[1] on the battle stage object named args[0]. */
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
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (onoff) {
                obj->flags |= flags;
            } else {
                obj->flags &= ~flags;
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: sets the rotation of the battle stage object named args[0] to (args[1..3]) in degrees;
 * EVT_NULLPTR leaves an axis unchanged.
 */
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
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (xa != EVTDAT_POINTER_MAX) {
                obj->rotation.x = x;
            }
            if (ya != EVTDAT_POINTER_MAX) {
                obj->rotation.y = y;
            }
            if (za != EVTDAT_POINTER_MAX) {
                obj->rotation.z = z;
            }
            break;
        }
    }
    return EVT_RETURN_DONE;
}

/** Script: returns the rotation of the battle stage object named args[0] in args[1..3]; EVT_NULLPTR skips an axis. */
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
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && !strcmp(obj->data->name, name)) {
            if (x != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, x, obj->rotation.x);
            }
            if (y != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, y, obj->rotation.y);
            }
            if (z != EVTDAT_POINTER_MAX) {
                evtSetFloat(event, z, obj->rotation.z);
            }
            break;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: shows or hides the status window (Mario's and the partner's HP and FP):
 * args[0] 2 = on for the system, 1 = on for a script, 0 = off for a script.
 */
USER_FUNC(btlevtcmd_StatusWindowOnOff) {
    BattleWork* wp = _battleWorkPointer;
    switch (evtGetValue(event, event->args[0])) {
        case STATUS_WINDOW_SYSTEM_ON:
            wp->statusWindowSystemOn = 1;
            break;
        case STATUS_WINDOW_EVENT_ON:
            wp->statusWindowEventOn = 1;
            break;
        case STATUS_WINDOW_EVENT_OFF:
            wp->statusWindowEventOn = 0;
            break;
    }
    BattleStatusWindowCheck();
    return EVT_RETURN_DONE;
}

/** Script: clears the battle fog effect at once. */
USER_FUNC(btlevtcmd_StageDispellFog) {
    BattleFogForceStop();
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ftomsec) {
    EvtWord* args = event->args;
    s32 value = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], value * 1000 / 60);
    return EVT_RETURN_DONE;
}

/**
 * Script: converts args[0] frames at 60 fps into frames at the running frame rate and returns them in args[1];
 * the PAL 50 Hz mode needs fewer frames. In the US release a 60 Hz setting returns the value unchanged.
 */
USER_FUNC(btlevtcmd_ftof) {
    EvtWord* args = event->args;
    s32 index, value, frames;
    value = evtGetValue(event, args[0]);
    index = args[1];
#if VERSION >= VERSION_US
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
