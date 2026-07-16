#include "battle/battle_event_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"

void btlsubResetMoveColorLvAll(BattleWork* battleWork);
void BattleStoreExp(BattleWork* battleWork, s32 exp);
void BattleStoreCoin(BattleWork* battleWork, s32 coin);

void BattleFogForceStop(void);
void battleSortPhaseMoveTable(BattleWork* battleWork, s32 arg);

void BattleAcHelpSetHelp(s32 help);
void BattleAcHelpSetDispType(s32 type);
void BattleActionCommandStop(BattleWork* battleWork);
s32 BattleActionCommandGetDefenceResult(void);
void psndSFXOff(s32 sfxId);

BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 partsId);
s32 BtlUnit_GetFp(BattleWorkUnit* unit);
s32 BtlUnit_GetMaxFp(BattleWorkUnit* unit);
void BtlUnit_SetFp(BattleWorkUnit* unit, s32 fp);
void BtlUnit_SetMaxFp(BattleWorkUnit* unit, s32 maxFp);
void BtlUnit_RecoverHp(BattleWorkUnit* unit, s32 hp);
void BtlUnit_RecoverFp(BattleWorkUnit* unit, s32 fp);

s32 BtlUnit_GetEnemyBelong(BattleWorkUnit* unit);
s32 BtlUnit_GetBodyPartsId(BattleWorkUnit* unit);
BattleWorkUnit* BattleGetPartnerPtr(BattleWork* battleWork, BattleWorkUnit* unit);
BattleWorkUnitPart* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetPartsOffsetPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void* BtlUnit_GetData(BattleWorkUnit* unit, s32 id);
void* evtChildEntry(void* event, void* script, s32 flags);
void evtDeleteID(s32 id);
void BattleRunWaitEvent(BattleWorkUnit* unit);

void BattleAttackDeclareAll(BattleWork* battleWork);
void BattleDamageDirect(s32 unk, BattleWorkUnit* unit, BattleWorkUnitPart* part, s32 damage, s32 zero, s32 flags, s32 arg5, s32 arg6);
s32 BattleCheckDamage(BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32 flags);
s32 BattlePreCheckDamage(BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32 flags);
void* BattleGetSelectWeapon(BattleWork* battleWork);

void BattleInitCounterPreCheckWork(void* work);
s32 BattleCheckCounter(void* work, BattleWorkUnit* attacker, BattleWorkUnit* target, BattleWorkUnitPart* part, void* weapon, s32* result);


s32 irand(s32 range);

void BtlUnit_GetPartsWorldPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetHitPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetHomePos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_GetRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_GetPartsRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetScale(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
s32 BtlUnit_GetWidth(BattleWorkUnit* unit);
s32 BtlUnit_GetHeight(BattleWorkUnit* unit);

void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetDispOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsHomePos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHitOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHitCursorOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHeight(BattleWorkUnit* unit, s32 height);

void BtlUnit_SetRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetBaseRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetBaseRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsBaseRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsBaseRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetRotateOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);

void BtlUnit_SetBaseScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsBaseScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);


BattleWorkUnit* BattleChangeParty(BattleWork* battleWork);
BattleWorkUnit* BtlUnit_Spawn(s32 unitKind, void* data);
void BtlUnit_SetMoveSpeed(BattleWorkUnit* unit, f32 speed);
void BtlUnit_SetJumpSpeed(BattleWorkUnit* unit, f32 speed);
void BtlUnit_SetFallAccel(BattleWorkUnit* unit, f32 accel);
s32 BtlUnit_GetCoin(BattleWorkUnit* unit);
s32 BtlUnit_GetExp(BattleWorkUnit* unit);
s32 BtlUnit_GetHitDamage(BattleWorkUnit* unit);
s32 BtlUnit_GetTotalHitDamage(BattleWorkUnit* unit);
void BtlUnit_SetStatus(BattleWorkUnit* unit, s32 status, s32 turns, s32 strength);
void BattleRunHitEventDirect(BattleWorkUnit* unit, s32 flags, s32 arg);
f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
s32 BattleActionCommandGetDifficulty(BattleWork* battleWork);
void BattleActionCommandSetDifficulty(BattleWork* battleWork, BattleWorkUnit* unit, s32 difficulty);
void BattleActionCommandDeclareACResult(BattleWork* battleWork, s32 result);
s32 BattleActionCommandGetPrizeLv(BattleWork* battleWork, BattleWorkUnit* unit, s32 value);
void BattleAcrobatStart(BattleWork* battleWork, s32 unitId, s32 a, s32 b, s32 c, s32 d);
s32 BattleAcrobatMain(BattleWork* battleWork);
void BattleAcrobatGetResult(BattleWork* battleWork, s32* result1, s32* result2);
s32 battleAcTimingA_GetSuccessFrame(BattleWork* battleWork);
void BattlePhaseEventStartDeclare(BattleWorkUnit* unit);
s32 BtlUnit_CheckUnitFlag(BattleWorkUnit* unit, u32 mask);
void BtlUnit_OnUnitFlag(BattleWorkUnit* unit, u32 mask);
void BtlUnit_OffUnitFlag(BattleWorkUnit* unit, u32 mask);
void BtlUnit_OnStatusFlag(BattleWorkUnit* unit, u32 mask);
void BtlUnit_OffStatusFlag(BattleWorkUnit* unit, u32 mask);
s32 BtlUnit_GetACPossibility(BattleWorkUnit* unit);
void BtlUnit_PayWeaponCost(BattleWorkUnit* unit, void* weapon);
BattleWorkUnit* BtlUnit_GetGuardKouraPtr(BattleWorkUnit* unit);
s32 BattlePadMultiCheckNow(s32 player, u32 mask);
s32 BattlePadMultiCheckTrigger(s32 player, u32 mask);
void BattleAudience_ApRecoveryBuild(void* apInfo);
void BattleStatusWindowCheck(void);
s32 BattleEnemyUseItemCheck(BattleWorkUnit* unit);
void* BattleStageGetPtr(void);
f32 BattleGetFloorHeight(BattleWork* battleWork, f32 x, f32 y, f32 z);
f32 sysMsec2FrameFloat(f32 msec);
void btlDispChangePeraAnime(BattleWorkUnitPart* part, s32 anime);
void btlDispPoseAnime(BattleWorkUnitPart* part);
void animPoseSetEffect(s32 poseId, s32 group, s32 arg);
void animPoseSetAnim(s32 poseId, s32 anim, s32 force);
void animPoseRelease(s32 poseId);
s32 animPoseEntry(s32 anim, s32 arg);
s32 animEffectAsync(s32 anim, s32 arg);
void BtlUnit_SetAnim(BattleWorkUnitPart* part, s32 anim);
void BtlUnit_SetAnimType(BattleWorkUnitPart* part, s32 type);
char* BtlUnit_GetPoseNameFromType(BattleWorkUnitPart* part, s32 type);
f32 animPoseGetLoopTimes(s32 poseId);
void* animPoseGetAnimPosePtr(s32 poseId);
void BtlUnit_PoseSoundInit(BattleWorkUnitPart* part);
s32 strcmp(const char* a, const char* b);
char* strcpy(char* dst, const char* src);
extern void* gp;
void BtlUnit_SetPartsMoveSpeed(BattleWorkUnitPart* part, f32 speed);
void BtlUnit_SetPartsFallAccel(BattleWorkUnitPart* part, f32 accel);
s32 BtlUnit_CheckData(BattleWorkUnit* unit, s32 value);
s32 BtlUnit_CheckStatusFlag(BattleWorkUnit* unit, u32 mask);
void BtlUnit_SetSeMode(void* seMode, s32 a, s32 b, s16 c, s16 d, s16 e, s16 f, s16 g);
void* BattleSetConfuseAct(BattleWork* battleWork, BattleWorkUnit* unit);
void itemDelete(void* item);
s32 itemEntry(s32 a, s32 item, s32 mode, s32 c, s32 d, f32 x, f32 y, f32 z);
extern u8 itemDataTable[];
extern s32 _mario_acrobat_voice_table[];
extern s32 _mario_attack_voice_table[];
extern s32 subsetevt_confuse_flustered[];
void BattleGetFirstAttackUnit(BattleWork* battleWork, BattleWorkUnitPart** part, BattleWorkUnit** unit);
void BattleActionCommandSetup(BattleWork* battleWork, s32 param, BattleWorkUnit* unit, s32 rawArg, s32 value);
void BattleActionCommandStart(BattleWork* battleWork);
void BattleActionCommandResult(BattleWork* battleWork);
void BattleActionCommandCheckDefence(BattleWorkUnit* unit, s32 value);
void BattleAudience_Case_Appeal(BattleWorkUnit* unit);
void BattleAfterReactionEntry(s32 unitId, s32 arg);
USER_FUNC(btlevtcmd_WaitEventEnd) {
    s32 id;

    id = evtGetValue(event, event->args[0]);

    return evtCheckID(id) ? 0 : EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_check_battleflag) {
    s32* args;
    u32 mask;
    u32 flags;
    s32 dst;

    args = event->args;
    mask = args[1];
    flags = *(u32*)((s32)_battleWorkPointer + 0xEF4);
    dst = args[0];

    if ((flags & mask) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}





USER_FUNC(btlevtcmd_onoff_battleflag) {
    s32* args;
    s32 state;
    u32 mask;
    BattleWork* battleWork;

    args = event->args;

    state = evtGetValue(event, args[0]);
    mask = args[1];
    battleWork = _battleWorkPointer;

    if (state == 1) {
        *(u32*)((s32)battleWork + 0xEF4) |= mask;
    } else {
        *(u32*)((s32)battleWork + 0xEF4) &= ~mask;
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_get_turn) {
    s32* args = event->args;

    evtSetValue(event, args[0], *(s16*)_battleWorkPointer);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_reset_turn) {
    *(u32*)((s32)_battleWorkPointer + 0xEF4) |= 0x2000;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_reset_move_color_lv_all) {
    btlsubResetMoveColorLvAll(_battleWorkPointer);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckPhase) {
    s32* args;
    u32 dst;
    u32 phase;

    args = event->args;
    dst = args[0];
    phase = args[1];

    if (phase != BattleGetSeq(_battleWorkPointer, 4)) {
        evtSetValue(event, dst, 0);
    } else {
        evtSetValue(event, dst, 1);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetPos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    BtlUnit_GetPos(unit, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)x);
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)y);
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)z);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetPosFloat) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    BtlUnit_GetPos(unit, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetFloat(event, dstX, x);
    }
    if (dstY != -250000000) {
        evtSetFloat(event, dstY, y);
    }
    if (dstZ != -250000000) {
        evtSetFloat(event, dstZ, z);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_GetRandomValue) {
    s32* args;
    s32 value;
    s32 dst;

    args = event->args;
    value = evtGetValue(event, args[0]);
    dst = args[1];

    evtSetValue(event, dst, irand(value));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_DrawLots) {
    s32* args;
    s32* weights;
    s32 dst;
    s32 count;
    s32 total;
    s32 cumulative;
    s32 result;
    s32 i;
    s32 r;

    args = event->args;
    dst = args[0];
    count = evtGetValue(event, args[1]);

    weights = BattleAlloc(count * 4);
    total = 0;

    for (i = 0; i < count; i++) {
        weights[i] = evtGetValue(event, args[i + 2]);
        total += weights[i];
    }

    r = irand(total);
    cumulative = 0;
    result = 0;

    for (i = 0; i < count; i++) {
        cumulative += weights[i];
        if (r < cumulative) {
            break;
        }
        result++;
    }

    evtSetValue(event, dst, result);
    BattleFree(weights);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SpawnUnit) {
    s32* args;
    BattleWorkUnit* unit;
    s32 dst;
    s32 kind;

    args = event->args;
    dst = args[0];

    kind = evtGetValue(event, args[1]);
    unit = BtlUnit_Spawn(kind, (void*)args[2]);

    if (unit == 0) {
        evtSetValue(event, dst, -1);
        return EVT_RETURN_DONE;
    }

    evtSetValue(event, dst, *(s32*)unit);
    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_KillUnit) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    u32 flags;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    flags = args[1];

    if (unit == 0) {
        return EVT_RETURN_DONE;
    }

    *(s16*)((s32)unit + 0x10C) = 0;
    BtlUnit_ClearStatus(unit);

    if ((flags & 2) != 0) {
        *(u32*)((s32)unit + 0x104) |= 0x20000;
        return EVT_RETURN_DONE;
    }

    BtlUnit_SetStatus(unit, 0x1B, 1, 1);

    if ((flags & 1) == 0) {
        *(u32*)((s32)unit + 0x1C) |= 0x80000000;
    }

    return EVT_RETURN_DONE;
}



s32 btlevtcmd_ReplaceParts(EventEntry* event, BOOL isFirstCall) {
    extern void BtlUnit_SetPartsRotate(f32, f32, f32, void*);
    extern void BtlUnit_SetPartsRotateOffset(f32, f32, f32, void*);
    extern void btlDispEntAnime(void*);
    extern f32 float_0_80422280;
    s32* args = event->args;
    s32 unitId;
    s32 partId;
    s32 targetPart;
    u8* kindPart;
    u8* part;
    void* unit;

    unitId = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    kindPart = (u8*)args[2];
    targetPart = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    part = (u8*)BattleGetUnitPartsPtr(unitId, partId);

    if (kindPart != NULL) {
        while (*(s32*)kindPart != targetPart) {
            kindPart += 0x4C;
        }
        *(void**)(part + 0x4) = kindPart;
        *(void**)(part + 0x8) = *(void**)(kindPart + 0x4);
        *(u32*)(part + 0x1AC) = *(u32*)(kindPart + 0x40);
        *(u32*)(part + 0x1B0) = *(u32*)(kindPart + 0x44);
        *(void**)(part + 0x1B4) = *(void**)(kindPart + 0x38);
        *(void**)(part + 0x1B8) = *(void**)(kindPart + 0x3C);
        *(Vec*)(part + 0x24) = *(Vec*)(kindPart + 0xC);
        BtlUnit_SetPartsRotate(float_0_80422280, float_0_80422280, float_0_80422280, part);
        BtlUnit_SetPartsRotateOffset(float_0_80422280, float_0_80422280, float_0_80422280, part);
        *(f32*)(part + 0x170) = *(f32*)(kindPart + 0x18);
        *(f32*)(part + 0x174) = *(f32*)(kindPart + 0x1C);
        *(f32*)(part + 0x17C) = float_0_80422280;
        *(f32*)(part + 0x180) = float_0_80422280;
        *(s16*)(part + 0x1A0) = 0;
    }
    btlDispEntAnime(unit);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangeParty) {
    s32* args;
    BattleWorkUnit* unit;
    s32 dst;
    s32 value;

    args = event->args;
    value = -1;
    dst = args[0];

    unit = BattleChangeParty(_battleWorkPointer);
    if (unit != 0) {
        value = *(s32*)unit;
    }

    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetPos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 ax;
    s32 ay;
    s32 az;
    f32 x;
    f32 y;
    f32 z;
    f32 oldX;
    f32 oldY;
    f32 oldZ;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));

    ax = args[1];
    x = (f32)evtGetValue(event, ax);
    ay = args[2];
    y = (f32)evtGetValue(event, ay);
    az = args[3];
    z = (f32)evtGetValue(event, az);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &oldX, &oldY, &oldZ);

    if (ax == -250000000) {
        x = oldX;
    }
    if (ay == -250000000) {
        y = oldY;
    }
    if (az == -250000000) {
        z = oldZ;
    }

    BtlUnit_SetPos(unit, x, y, z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AddPos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddPos(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetPartsPos) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);

    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);

    dstX = args[2];
    dstY = args[3];
    dstZ = args[4];

    BtlUnit_GetPartsWorldPos(part, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)x);
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)y);
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)z);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetPartsPos) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;
    f32 x;
    f32 y;
    f32 z;
    f32 ux;
    f32 uy;
    f32 uz;

    args = event->args;
    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);

    if ((*(u32*)((s32)part + 0x1AC) & 0x10000000) != 0) {
        BtlUnit_SetPartsPos(part, x, y, z);
    } else {
        BtlUnit_GetPos(unit, &ux, &uy, &uz);
        x -= ux;
        y -= uy;
        z -= uz;
        BtlUnit_SetPartsOffsetPos(part, x, y, z);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AddPartsPos) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    if ((*(u32*)((s32)part + 0x1AC) & 0x10000000) != 0) {
        BtlUnit_AddPartsPos(part, x, y, z);
    } else {
        BtlUnit_AddPartsOffsetPos(part, x, y, z);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetDispOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetDispOffset(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetPartsDispOffset) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsDispOffset(part, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AddPartsDispOffset) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = (f32)evtGetValue(event, args[2]);
    y = (f32)evtGetValue(event, args[3]);
    z = (f32)evtGetValue(event, args[4]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsDispOffset(part, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetTogeOffset) {
    s32* args;
    BattleWorkUnit* unit;
    f32 x, y, z;

    args = event->args;
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, evtGetValue(event, args[0])));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    if (x != 0.0f) *(f32*)((s32)unit + 0xA8) = x;
    if (y != 0.0f) *(f32*)((s32)unit + 0xAC) = y;
    if (z != 0.0f) *(f32*)((s32)unit + 0xB0) = z;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetHomePos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    BtlUnit_GetHomePos(unit, &x, &y, &z);

    
        evtSetValue(event, dstX, (s32)x);
    
    
        evtSetValue(event, dstY, (s32)y);
    
    
        evtSetValue(event, dstZ, (s32)z);
    

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetHomePos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetHomePos(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AddHomePos) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddHomePos(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetPartsHomePos) {
    s32* args;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;
    BattleWorkUnitPart* part;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsHomePos(part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetHitPos) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);

    dstX = args[2];
    dstY = args[3];
    dstZ = args[4];

    BtlUnit_GetHitPos(part, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)x);
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)y);
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)z);
    }

    return EVT_RETURN_DONE;
}





USER_FUNC(btlevtcmd_SetHitOffset) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitOffset(unit, part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetHitCursorOffset) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitCursorOffset(unit, part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetWidth) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    value = BtlUnit_GetWidth(unit);

    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetHeight) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    value = BtlUnit_GetHeight(unit);

    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetHeight) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 height;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    height = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    BtlUnit_SetHeight(unit, (s16)height);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetStatusMg) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    evtSetFloat(event, dst, *(f32*)((s32)unit + 0x114));

    return EVT_RETURN_DONE;
}





USER_FUNC(btlevtcmd_SetStatusIconOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 x;
    s32 y;

    args = event->args;

    type = evtGetValue(event, args[0]);
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (x != -250000000) {
        *(s16*)((s32)unit + 0xD0) = x;
    }
    if (y != -250000000) {
        *(s16*)((s32)unit + 0xD2) = y;
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetHpGaugeOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 x;
    s32 y;

    args = event->args;

    type = evtGetValue(event, args[0]);
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (x != -250000000) {
        *(s16*)((s32)unit + 0xD4) = x;
    }
    if (y != -250000000) {
        *(s16*)((s32)unit + 0xD6) = y;
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetHpGaugeOffset) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dstX;
    s32 dstY;
    s32 x;
    s32 y;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    dstX = args[1];
    dstY = args[2];

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, id);

    x = *(s16*)((s32)unit + 0xD4);
    y = *(s16*)((s32)unit + 0xD6);

    if (x != -250000000) {
        evtSetValue(event, dstX, x);
    }
    if (y != -250000000) {
        evtSetValue(event, dstY, y);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetAlpha) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 partId;
    s32 alpha;

    args = event->args;

    type = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);
    alpha = evtGetValue(event, args[2]);

    part = BattleGetUnitPartsPtr(BattleTransID(event, type), partId);
    *(u8*)((s32)part + 0x4F3) = alpha;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetRGB) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 partId;
    s32 dstR;
    s32 dstG;
    s32 dstB;

    args = event->args;

    type = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);

    part = BattleGetUnitPartsPtr(BattleTransID(event, type), partId);

    dstR = args[2];
    dstG = args[3];
    dstB = args[4];

    evtSetValue(event, dstR, *(u8*)((s32)part + 0x4F0));
    evtSetValue(event, dstG, *(u8*)((s32)part + 0x4F1));
    evtSetValue(event, dstB, *(u8*)((s32)part + 0x4F2));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetRGB) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 partId;
    s32 r;
    s32 g;
    s32 b;

    args = event->args;

    type = evtGetValue(event, args[0]);
    partId = evtGetValue(event, args[1]);
    r = evtGetValue(event, args[2]);
    g = evtGetValue(event, args[3]);
    b = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(BattleTransID(event, type), partId);

    *(u8*)((s32)part + 0x4F0) = r;
    *(u8*)((s32)part + 0x4F1) = g;
    *(u8*)((s32)part + 0x4F2) = b;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetRotate) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    BtlUnit_GetRotate(unit, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)x);
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)y);
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)z);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetRotate) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 ax;
    s32 ay;
    s32 az;
    f32 x;
    f32 y;
    f32 z;
    f32 oldX;
    f32 oldY;
    f32 oldZ;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));

    ax = args[1];
    x = (f32)evtGetValue(event, ax);
    ay = args[2];
    y = (f32)evtGetValue(event, ay);
    az = args[3];
    z = (f32)evtGetValue(event, az);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetRotate(unit, &oldX, &oldY, &oldZ);

    if (ax == -250000000) {
        x = oldX;
    }
    if (ay == -250000000) {
        y = oldY;
    }
    if (az == -250000000) {
        z = oldZ;
    }

    BtlUnit_SetRotate(unit, x, y, z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AddRotate) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddRotate(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetPartsRotate) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partId = evtGetValue(event, args[1]);

    part = BattleGetUnitPartsPtr(id, partId);

    dstX = args[2];
    dstY = args[3];
    dstZ = args[4];

    BtlUnit_GetPartsRotate(part, &x, &y, &z);

    evtSetValue(event, dstX, (s32)x);
    evtSetValue(event, dstY, (s32)y);
    evtSetValue(event, dstZ, (s32)z);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetPartsRotate) {
    s32* args;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;
    BattleWorkUnitPart* part;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotate(part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AddPartsRotate) {
    s32* args;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;
    BattleWorkUnitPart* part;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotate(part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetBaseRotate) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 ax;
    s32 ay;
    s32 az;
    f32 x;
    f32 y;
    f32 z;
    f32 oldX;
    f32 oldY;
    f32 oldZ;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));

    ax = args[1];
    x = (f32)evtGetValue(event, ax);
    ay = args[2];
    y = (f32)evtGetValue(event, ay);
    az = args[3];
    z = (f32)evtGetValue(event, az);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetBaseRotate(unit, &oldX, &oldY, &oldZ);

    if (ax == -250000000) {
        x = oldX;
    }
    if (ay == -250000000) {
        y = oldY;
    }
    if (az == -250000000) {
        z = oldZ;
    }

    BtlUnit_SetBaseRotate(unit, x, y, z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetPartsBaseRotate) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 id;
    s32 partId;
    f32 x;
    f32 y;
    f32 z;
    f32 oldX;
    f32 oldY;
    f32 oldZ;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);

    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_GetPartsBaseRotate(part, &oldX, &oldY, &oldZ);

    if (x != -250000000.0f) {
        oldX = x;
    }
    if (y != -250000000.0f) {
        oldY = y;
    }
    if (z != -250000000.0f) {
        oldZ = z;
    }

    BtlUnit_SetPartsBaseRotate(part, oldX, oldY, oldZ);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetRotateOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetRotateOffset(unit, x, y, z);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetPartsRotateOffset) {
    s32* args;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;
    BattleWorkUnitPart* part;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AddPartsRotateOffset) {
    s32* args;
    s32 id;
    s32 partId;
    s32 x;
    s32 y;
    s32 z;
    BattleWorkUnitPart* part;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partId = evtGetValue(event, args[1]);
    x = evtGetValue(event, args[2]);
    y = evtGetValue(event, args[3]);
    z = evtGetValue(event, args[4]);

    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetRotateOffsetFromCenterOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;
    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    BtlUnit_SetRotateOffset(unit, *(f32*)((s32)unit + 0x84), *(f32*)((s32)unit + 0x88), *(f32*)((s32)unit + 0x8C));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetCutBaseOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(f32*)((s32)unit + 0xD8) = x;
    *(f32*)((s32)unit + 0xDC) = y;
    *(f32*)((s32)unit + 0xE0) = z;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetBintaHitOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(f32*)((s32)unit + 0xEC) = x;
    *(f32*)((s32)unit + 0xF0) = y;
    *(f32*)((s32)unit + 0xF4) = z;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetKissHitOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    f32 x, y, z;

    args = event->args;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(f32*)((s32)unit + 0xF8) = x;
    *(f32*)((s32)unit + 0xFC) = y;
    *(f32*)((s32)unit + 0x100) = z;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetCutWidth) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 value;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    value = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(f32*)((s32)unit + 0xE4) = (f32)value;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetCutHeight) {
    s32* args;
    BattleWorkUnit* unit;
    s32 id;
    s32 value;

    args = event->args;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    value = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(f32*)((s32)unit + 0xE8) = (f32)value;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetPossessionItemOffset) {
    s32* args;
    BattleWorkUnit* unit;
    s32 ax, ay, az;
    s32 x, y, z;

    args = event->args;
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, evtGetValue(event, args[0])));
    ax = args[1];
    x = evtGetValue(event, ax);
    ay = args[2];
    y = evtGetValue(event, ay);
    az = args[3];
    z = evtGetValue(event, az);
    if (ax != -250000000) *(f32*)((s32)unit + 0xC0) = (f32)x;
    if (ay != -250000000) *(f32*)((s32)unit + 0xC4) = (f32)y;
    if (az != -250000000) *(f32*)((s32)unit + 0xC8) = (f32)z;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetBaseScale) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);

    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);

    if (unit != 0) {
        BtlUnit_SetBaseScale(unit, x, y, z);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetPartsBaseScale) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
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

USER_FUNC(btlevtcmd_GetScale) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);

    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    BtlUnit_GetScale(unit, &x, &y, &z);

    if (dstX != -250000000) {
        evtSetFloat(event, dstX, x);
    }
    if (dstY != -250000000) {
        evtSetFloat(event, dstY, y);
    }
    if (dstZ != -250000000) {
        evtSetFloat(event, dstZ, z);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetScale) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);

    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);

    BtlUnit_SetScale(unit, x, y, z);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_AddScale) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);

    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);

    BtlUnit_AddScale(unit, x, y, z);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetPartsScale) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
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

USER_FUNC(btlevtcmd_AddPartsScale) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
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

USER_FUNC(btlevtcmd_GetHp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtSetValue(event, args[1], *(s16*)((s32)unit + 0x10C));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetFp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    dst = args[1];
    evtSetValue(event, dst, BtlUnit_GetFp(unit));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetMaxHp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtSetValue(event, args[1], *(s16*)((s32)unit + 0x108));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetMaxFp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    dst = args[1];
    evtSetValue(event, dst, BtlUnit_GetMaxFp(unit));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetHp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;
    f32 ratio;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    value = evtGetValue(event, args[1]);

    *(s16*)((s32)unit + 0x10C) = value;
    *(s32*)((s32)unit + 0x208) = *(s16*)((s32)unit + 0x10C);
    *(s32*)((s32)unit + 0x20C) = *(s16*)((s32)unit + 0x10C);

    ratio = (f32)*(s16*)((s32)unit + 0x10C) / (f32)*(s16*)((s32)unit + 0x108);
    *(f32*)((s32)unit + 0x214) = ratio;
    *(f32*)((s32)unit + 0x210) = ratio;

    *(s16*)((s32)unit + 0x200) = *(s16*)((s32)unit + 0x202);
    *(s16*)((s32)unit + 0x204) = *(s16*)((s32)unit + 0x206);
    *(u16*)((s32)unit + 0x1FE) |= 1;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetFp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    value = evtGetValue(event, args[1]);
    BtlUnit_SetFp(unit, value);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetMaxFp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    value = evtGetValue(event, args[1]);
    BtlUnit_SetMaxFp(unit, value);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetSwallowParam) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    value = evtGetValue(event, args[1]);

    if (unit != 0) {
        *(u8*)((s32)unit + 0x25) = value;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetSwallowAttribute) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    u8 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    value = evtGetValue(event, args[1]);

    if (unit != 0) {
        *(u8*)((s32)unit + 0x26) = value;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMaxMoveCount) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    value = evtGetValue(event, args[1]);

    *(u8*)((s32)unit + 0x21) = value;

    if (*(s8*)((s32)unit + 0x22) > *(s8*)((s32)unit + 0x21)) {
        *(u8*)((s32)unit + 0x22) = *(u8*)((s32)unit + 0x21);
    }

    return EVT_RETURN_DONE;
}








USER_FUNC(btlevtcmd_GetDamagePartsId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    void* data;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    dst = args[1];
    data = *(void**)((s32)unit + 0x258);

    evtSetValue(event, dst, **(s32**)((s32)data + 4));

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_GetHpDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtSetValue(event, args[1], *(s8*)((s32)unit + 0x270));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetFpDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtSetValue(event, args[1], *(s8*)((s32)unit + 0x271));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetHpDamageCount) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtSetValue(event, args[1], *(s16*)((s32)unit + 0x260));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RecoverHp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);

    if ((*(u32*)((s32)unit + 0x104) & 0x40000000) == 0) {
        *(s32*)((s32)unit + 0x208) = *(s16*)((s32)unit + 0x10C);
        BtlUnit_RecoverHp(unit, value);
        *(s32*)((s32)unit + 0x20C) = *(s16*)((s32)unit + 0x10C);
        *(s16*)((s32)unit + 0x200) = *(s16*)((s32)unit + 0x202);
        *(s16*)((s32)unit + 0x204) = *(s16*)((s32)unit + 0x206);
        *(u16*)((s32)unit + 0x1FE) |= 1;
    } else {
        BtlUnit_RecoverHp(unit, value);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_RecoverFp) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    if (id == -1) {
        return EVT_RETURN_DONE;
    }

    unit = BattleGetUnitPtr(battleWork, id);
    evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    BtlUnit_RecoverFp(unit, value);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetOverTurnCount) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s8*)((s32)unit + 0x12B));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetOverTurnCount) {
    s32* args;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = evtGetValue(event, args[1]);

    *(u8*)((s32)BattleGetUnitPtr(_battleWorkPointer, id) + 0x12B) = value;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetUnitWork) {
    s32 id;
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 index;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    index = evtGetValue(event, args[1]);
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s32*)((s32)unit + 0x218 + index * 4));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetUnitWork) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 index;
    s32 value;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    index = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(s32*)((s32)unit + 0x218 + index * 4) = value;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AddUnitWork) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 index;
    s32 value;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    index = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(s32*)((s32)unit + 0x218 + index * 4) += value;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitWorkFloat) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 index;
    s32 offset;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    index = evtGetValue(event, args[1]);
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    offset = index * 4 + 0x218;
    evtSetFloat(event, dst, *(f32*)((s32)unit + offset));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetUnitWorkFloat) {
    BattleWorkUnit* unit;
    s32* args;
    s32 type;
    s32 id;
    s32 index;
    s32 offset;
    f32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    index = evtGetValue(event, args[1]);
    value = evtGetFloat(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    offset = index * 4 + 0x218;
    *(f32*)((s32)unit + offset) = value;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetPartsWork) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 index;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    dst = args[3];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);
    evtSetValue(event, dst, *(s32*)((s32)part + 0x130 + index * 4));

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetPartsWork) {
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 partsId;
    s32 index;
    s32 value;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    value = evtGetValue(event, args[3]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);
    *(s32*)((s32)part + 0x130 + index * 4) = value;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDamage) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    dst = args[1];

    if (unit != 0) {
        evtSetValue(event, dst, BtlUnit_GetHitDamage(unit));
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetTotalDamage) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    dst = args[1];

    if (unit != 0) {
        evtSetValue(event, dst, BtlUnit_GetTotalHitDamage(unit));
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetDamageCode) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(u32*)((s32)unit + 0x278) & 0xFF);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDamageCode) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = args[1];
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (((*(u32*)((s32)unit + 0x278) & value) != 0)) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}


s32 btlevtcmd_StartAvoid(EventEntry* event, BOOL isFirstCall) {
    extern void* effNiceEntry(s32, f32, f32, f32);
    extern s32 BattleRunHitEvent(void*, u32);
    extern f32 float_85_804222e8;
    extern f32 float_50_804222ac;
    extern f32 float_neg50_804222ec;
    extern f32 float_45_804222f0;
    extern f32 float_0p75_80422290;
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    u32 flags;
    void* effect;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    flags = args[1];

    if ((flags & 0xFF) == 0x26) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        if (y < float_85_804222e8) {
            y += float_50_804222ac;
        } else {
            y += float_neg50_804222ec;
        }
        x -= float_45_804222f0;
        effect = effNiceEntry(6, x, y, z);
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = float_0p75_80422290;
    }

    *(s32*)((s32)unit + 0x274) = 0;
    BattleRunHitEvent(unit, flags | 0x100);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_RunHitEventDirect) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 flags;
    s32 arg;
    s32 pattern;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    flags = args[1];
    arg = evtGetValue(event, args[2]);
    pattern = evtGetValue(event, args[3]);

    *(s32*)((s32)unit + 0x274) = pattern;
    BattleRunHitEventDirect(unit, flags | 0x100, arg);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckDamagePattern) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = args[1];
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if ((*(u32*)((s32)unit + 0x274) == value)) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_CheckSpace) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 dst;
    s32 x;
    s32 y;
    s32 z;
    s32 rangeX;
    s32 rangeY;
    s32 rangeZ;
    s32 i;
    s32 hit;
    f32 fx;
    f32 fy;
    f32 fz;
    s32 ix;
    s32 iy;
    s32 iz;
    s32 d;
    s32 sign;

    args = event->args;
    battleWork = _battleWorkPointer;

    dst = args[0];
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);
    z = evtGetValue(event, args[3]);
    rangeX = evtGetValue(event, args[4]);
    rangeY = evtGetValue(event, args[5]);
    rangeZ = evtGetValue(event, args[6]);

    for (i = 0; i < 0x40; i++) {
        hit = 0;
        unit = BattleGetUnitPtr(battleWork, i);

        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1) {
            BtlUnit_GetHomePos(unit, &fx, &fy, &fz);

            ix = (s32)fx;
            iy = (s32)fy;
            iz = (s32)fz;

            if (rangeX != 0) {
                d = ix - x;
                sign = d >> 31;
                d = (d ^ sign) - sign;
                if (d < rangeX) {
                    hit = 1;
                }
            }
            if (rangeY != 0) {
                d = iy - y;
                sign = d >> 31;
                d = (d ^ sign) - sign;
                if (d < rangeY) {
                    hit = 1;
                }
            }
            if (rangeZ != 0) {
                d = iz - z;
                sign = d >> 31;
                d = (d ^ sign) - sign;
                if (d < rangeZ) {
                    hit = 1;
                }
            }

            if (hit != 0) {
                evtSetValue(event, dst, i);
                return EVT_RETURN_DONE;
            }
        }
    }

    evtSetValue(event, dst, -1);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetEnemyBelong) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;
    s32 value;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    value = BtlUnit_GetEnemyBelong(unit);
    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetFriendBelong) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s8*)((s32)unit + 0xC));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetProtectId) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s32*)((s32)unit + 0x140));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetUnitKind) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s32*)((s32)unit + 0x8));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetPartnerId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnit* partner;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);
    partner = BattleGetPartnerPtr(battleWork, unit);

    if (partner != 0) {
        evtSetValue(event, dst, *(s32*)partner);
    } else {
        evtSetValue(event, dst, -1);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetMarioId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 belong;
    s32 i;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);
    belong = *(s8*)((s32)unit + 0xC);

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            if (*(s8*)((s32)unit + 0xC) == belong && *(s32*)((s32)unit + 0x8) == 0xDE) {
                break;
            }
        }
    }

    if (i >= 0x40) {
        evtSetValue(event, dst, -1);
    } else {
        evtSetValue(event, dst, *(s32*)unit);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetPartyId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 belong;
    s32 kind;
    s32 i;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);
    belong = *(s8*)((s32)unit + 0xC);

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            if (*(s8*)((s32)unit + 0xC) == belong) {
                kind = *(s32*)((s32)unit + 0x8);
                if (kind >= 0xE0 && kind < 0xE7) {
                    break;
                }
            }
        }
    }

    if (i >= 0x40) {
        evtSetValue(event, dst, -1);
    } else {
        evtSetValue(event, dst, *(s32*)unit);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetBodyId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 bodyId;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);
    bodyId = BtlUnit_GetBodyPartsId(unit);
    evtSetValue(event, dst, bodyId);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetUnitId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);

    if (unit != 0 && ((*(u32*)((s32)unit + 0x1C) & 0x80000000) != 0)) {
        unit = 0;
    }

    if (unit != 0) {
        evtSetValue(event, dst, id);
    } else {
        evtSetValue(event, dst, -1);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetPartyTechLv) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    void* pouch;
    s32 type;
    s32 id;
    s32 dst;
    s32 kind;
    s32 partyId;
    s32 techLv;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, id);
    kind = *(s32*)((s32)unit + 0x8);
    techLv = 0;

    if (kind >= 0xE0 && kind < 0xE7) {
        partyId = BattleTransPartyId(kind);
        pouch = pouchGetPtr();
        pouch = (void*)((s32)pouch + partyId * 0xE);
        techLv = *(s16*)((s32)pouch + 0xC);
    }

    evtSetValue(event, dst, techLv);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CalculateFaceDirection) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnit* targetUnit;
    BattleWorkUnitPart* part;
    BattleWorkUnitPart* targetPart;
    s32 unitId;
    s32 partsId;
    s32 targetId;
    s32 targetPartsId;
    s32 targetX;
    s32 targetArg;
    s32 flags;
    s32 dst;
    s32 dir;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 ox;
    f32 oy;
    f32 oz;

    args = event->args;
    battleWork = _battleWorkPointer;
    dir = 0;
    part = 0;
    targetPart = 0;

    unitId = BattleTransID(event, evtGetValue(event, args[0]));
    partsId = evtGetValue(event, args[1]);
    targetId = evtGetValue(event, args[2]);
    targetX = targetId;
    targetPartsId = evtGetValue(event, args[3]);
    flags = args[4];
    dst = args[5];

    if ((flags & 9) == 0) {
        targetId = BattleTransID(event, targetId);
    }

    unit = BattleGetUnitPtr(battleWork, unitId);
    if (partsId != -1) {
        part = BtlUnit_GetPartsPtr(unit, partsId);
    }

    if ((flags & 0x10) != 0) {
        targetUnit = BattleGetUnitPtr(battleWork, targetId);
        if (targetPartsId != -1) {
            targetPart = BtlUnit_GetPartsPtr(targetUnit, targetPartsId);
        }
    } else {
        targetUnit = 0;
    }

    dir = *(s8*)((s32)unit + 0x189);
    if (part != 0) {
        dir = *(s8*)((s32)part + 0xBD);
    }

    if ((flags & 4) != 0) {
        dir = *(s8*)((s32)battleWork + *(s8*)((s32)unit + 0xC) * 8 + 0xA);
    }

    if ((flags & 8) != 0) {
        dir = *(s8*)((s32)unit + 0x188);
        if (part != 0) {
            dir = *(s8*)((s32)part + 0xBC);
        }
    }

    if ((flags & 0x10) != 0) {
        BtlUnit_GetPos(unit, &x1, &y1, &z1);
        if (part != 0) {
            if ((*(u32*)((s32)part + 0x1AC) & 0x10000000) != 0) {
                BtlUnit_GetPartsPos(part, &x1, &y1, &z1);
            } else {
                BtlUnit_GetPartsOffsetPos(part, &ox, &oy, &oz);
                x1 += ox;
            }
        }

        BtlUnit_GetPos(targetUnit, &x2, &y2, &z2);
        if (targetPart != 0) {
            if ((*(u32*)((s32)targetPart + 0x1AC) & 0x10000000) != 0) {
                BtlUnit_GetPartsPos(targetPart, &x2, &y2, &z2);
            } else {
                BtlUnit_GetPartsOffsetPos(targetPart, &ox, &oy, &oz);
                x2 += ox;
            }
        }

        if (x2 - x1 > 0.0f) {
            dir = 1;
        } else if (x2 - x1 < 0.0f) {
            dir = -1;
        }
    }

    if ((flags & 1) != 0) {
        BtlUnit_GetPos(unit, &x1, &y1, &z1);
        if (part != 0) {
            if ((*(u32*)((s32)part + 0x1AC) & 0x10000000) != 0) {
                BtlUnit_GetPartsPos(part, &x1, &y1, &z1);
            } else {
                BtlUnit_GetPartsOffsetPos(part, &ox, &oy, &oz);
                x1 += ox;
            }
        }

        if ((f32)targetX - x1 > 0.0f) {
            dir = 1;
        } else if ((f32)targetX - x1 < 0.0f) {
            dir = -1;
        }
    }

    if ((flags & 0x100) != 0) {
        dir *= -1;
    }

    evtSetValue(event, dst, dir);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetFaceDirection) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dst;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, dst, *(s8*)((s32)unit + 0x189));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChangeFaceDirection) {
    s32* args = event->args;
    s32 type;
    s32 id;
    s32 dir;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dir = evtGetValue(event, args[1]);

    *(s8*)((s32)BattleGetUnitPtr(_battleWorkPointer, id) + 0x189) = dir;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_ChangePartsFaceDirection) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 dir;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    dir = evtGetValue(event, args[2]);

    part = BattleGetUnitPartsPtr(id, partsId);
    *(s8*)((s32)part + 0xBD) = dir;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_ResetFaceDirection) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 belong;
    u8* row;
    u8 faceDir;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    belong = *(s8*)((s32)unit + 0xC);
    row = (u8*)battleWork;
    row += belong << 3;

    faceDir = *(u8*)(row + 0xA);
    *(volatile u8*)((s32)unit + 0x189) = faceDir;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckAttribute) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 attr;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    attr = args[1];
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if ((*(u32*)((s32)unit + 0x104) & attr) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_OnAttribute) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 attr;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    attr = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(u32*)((s32)unit + 0x104) |= attr;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_OffAttribute) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    u32 attr;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    attr = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(u32*)((s32)unit + 0x104) = *(u32*)((s32)unit + 0x104) & ~attr;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckPartsAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 dst;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];
    dst = args[3];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    if ((*(u32*)((s32)part + 0x1AC) & attr) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnPartsAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(u32*)((s32)part + 0x1AC) |= attr;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffPartsAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(u32*)((s32)part + 0x1AC) &= ~attr;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckPartsCounterAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 dst;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];
    dst = args[3];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    if ((*(u32*)((s32)part + 0x1B0) & attr) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnPartsCounterAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(u32*)((s32)part + 0x1B0) |= attr;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffPartsCounterAttribute) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    s32 attr;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    attr = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(u32*)((s32)part + 0x1B0) &= ~attr;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsBlur) {
    s32 id;
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 partsId;
    u8 v0;
    u8 v1;
    u8 v2;
    u8 v3;
    u8 v4;
    u8 v5;
    u8 v6;
    u8 v7;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);

    v0 = evtGetValue(event, args[2]);
    v1 = evtGetValue(event, args[3]);
    v2 = evtGetValue(event, args[4]);
    v3 = evtGetValue(event, args[5]);
    v4 = evtGetValue(event, args[6]);
    v5 = evtGetValue(event, args[7]);
    v6 = evtGetValue(event, args[8]);
    v7 = evtGetValue(event, args[9]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(volatile u8*)((s32)part + 0x220) = v0;
    *(volatile u8*)((s32)part + 0x221) = v1;
    *(volatile u8*)((s32)part + 0x222) = v2;
    *(volatile u8*)((s32)part + 0x223) = v3;
    *(volatile u8*)((s32)part + 0x224) = v4;
    *(volatile u8*)((s32)part + 0x225) = v5;
    *(volatile u8*)((s32)part + 0x226) = v6;
    *(volatile u8*)((s32)part + 0x227) = v7;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnToken) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    mask = args[1];
    *(u32*)((s32)unit + 0x27C) = mask | *(u32*)((s32)unit + 0x27C);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_OffToken) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    mask = args[1];
    *(u32*)((s32)unit + 0x27C) = *(u32*)((s32)unit + 0x27C) & ~mask;

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_CheckToken) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    u32 mask;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    mask = args[1];
    dst = args[2];

    if ((*(u32*)((s32)unit + 0x27C) & mask) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetRegistStatus) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(s32*)((s32)unit + 0x144) = value;

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_CheckCommandUnit) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 kind;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    kind = *(s32*)((s32)unit + 0x8);
    dst = args[1];

    if (kind == 0xDE || (kind >= 0xE0 && kind < 0xE7)) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_ChangeKind) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 kind;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    kind = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(s32*)((s32)unit + 0x8) = kind;

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetPartsDefenceTable) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);
    *(s32*)((s32)part + 0x1B4) = value;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetPartsDefenceAttrTable) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);
    *(s32*)((s32)part + 0x1B8) = value;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_ChangeDataTable) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    *(s32*)((s32)unit + 0x18) = value;

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_GetBackItem) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 item;
    void* work;
    s32* slots;
    s32 i;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, id);

    if ((*(u32*)((s32)unit + 0x1C) & 0x40000000) != 0) {
        item = *(s32*)((s32)unit + 0x308);
        if (item != 0) {
            work = *(void**)((s32)battleWork + 0x2738);
            slots = *(s32**)((s32)work + 0xC);

            for (i = 0; i < 8; i++) {
                if (*(s32*)((s32)slots + 0x7C + i * 4) == 0) {
                    break;
                }
            }

            *(s32*)((s32)slots + 0x7C + i * 4) = item;
            *(s32*)((s32)unit + 0x308) = 0;
        }
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetExp) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    value = BtlUnit_GetExp(unit);

    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_StoreExp) {
    s32* args = event->args;
    s32 value;

    value = evtGetValue(event, args[0]);
    BattleStoreExp(_battleWorkPointer, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetCoin) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    value = BtlUnit_GetCoin(unit);

    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_StoreCoin) {
    s32* args = event->args;
    s32 value;

    value = evtGetValue(event, args[0]);
    BattleStoreCoin(_battleWorkPointer, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetMoveSpeed) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = evtGetFloat(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetMoveSpeed(unit, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetJumpSpeed) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = evtGetFloat(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetJumpSpeed(unit, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetFallAccel) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    value = evtGetFloat(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetFallAccel(unit, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpSetting) {
    s32 id;
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 frames;
    f32 jumpSpeed;
    f32 fallAccel;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    frames = evtGetValue(event, args[1]);
    jumpSpeed = evtGetFloat(event, args[2]);
    fallAccel = evtGetFloat(event, args[3]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    *(volatile s32*)((s32)unit + 0x16C) = frames;
    *(volatile f32*)((s32)unit + 0x170) = jumpSpeed;
    *(volatile f32*)((s32)unit + 0x174) = fallAccel;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpPosition) {
    extern void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_LoadSeMode(s32 mode, s32 sound, void* soundData, void* soundWork);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern void psndSFXOn_3D(s32 sound, void* pos);
    extern f32 float_0_80422280;
    extern f32 float_0p5_804222a0;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, type);
    f32 targetX = (f32)evtGetValue(event, args[1]);
    f32 targetY = (f32)evtGetValue(event, args[2]);
    f32 targetZ = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 sound = evtGetValue(event, args[5]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    if (isFirstCall != 0) {
        f32 x;
        f32 y;
        f32 z;
        f32 dy;

        *(u8*)((s32)event + 0xD) = 1;
        if (frames != 0) {
            *(s32*)((s32)unit + 0x16C) = frames;
        }
        BtlUnit_SetMoveTargetPos(unit, targetX, targetY, targetZ);
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        *(f32*)((s32)unit + 0x17C) = angleABf(x, z, targetX, targetZ);
        *(f32*)((s32)unit + 0x180) = distABf(x, z, targetX, targetZ);
        if (targetX - x > float_0_80422280) {
            *(s8*)((s32)unit + 0x188) = 1;
        } else if (targetX - x < float_0_80422280) {
            *(s8*)((s32)unit + 0x188) = -1;
        }

        dy = targetY - y;
        if (*(s32*)((s32)unit + 0x16C) == 0) {
            *(s32*)((s32)unit + 0x16C) = (s32)(*(f32*)((s32)unit + 0x180) / *(f32*)((s32)unit + 0x170));
            *(f32*)((s32)unit + 0x180) -= (f32)*(s32*)((s32)unit + 0x16C) * *(f32*)((s32)unit + 0x170);
        } else {
            *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x180) / (f32)*(s32*)((s32)unit + 0x16C);
            *(f32*)((s32)unit + 0x180) -= (f32)*(s32*)((s32)unit + 0x16C) * *(f32*)((s32)unit + 0x170);
        }
        if (*(s32*)((s32)unit + 0x16C) == 0) {
            return 2;
        }
        *(f32*)((s32)unit + 0x178) = dy / (f32)*(s32*)((s32)unit + 0x16C) +
                                     (*(f32*)((s32)unit + 0x174) * (f32)*(s32*)((s32)unit + 0x16C)) * float_0p5_804222a0;
        *(f32*)((s32)unit + 0x170) += *(f32*)((s32)unit + 0x180) / (f32)*(s32*)((s32)unit + 0x16C);
        BtlUnit_LoadSeMode(2, sound, (void*)((s32)unit + 0x1C4), (void*)((s32)unit + 0x18C));
        if (*(s32*)((s32)unit + 0x18C) != 0) {
            f32 pos[3];
            pos[0] = *(f32*)((s32)unit + 0x154);
            pos[1] = *(f32*)((s32)unit + 0x158);
            pos[2] = *(f32*)((s32)unit + 0x15C);
            psndSFXOn_3D(*(s32*)((s32)unit + 0x18C), pos);
        }
    }

    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x178);
    *(f32*)((s32)unit + 0x178) -= *(f32*)((s32)unit + 0x174);
    if (*(f32*)((s32)unit + 0x178) < float_0_80422280 && *(f32*)((s32)unit + 0x158) < *(f32*)((s32)unit + 0x164)) {
        *(f32*)((s32)unit + 0x158) = *(f32*)((s32)unit + 0x164);
    }
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    *(s32*)((s32)unit + 0x16C) = *(s32*)((s32)unit + 0x16C) - 1;
    if (*(s32*)((s32)unit + 0x16C) > 0) {
        return 0;
    }
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164), *(f32*)((s32)unit + 0x168));
    if (*(s32*)((s32)unit + 0x190) != 0) {
        f32 pos[3];
        pos[0] = *(f32*)((s32)unit + 0x160);
        pos[1] = *(f32*)((s32)unit + 0x164);
        pos[2] = *(f32*)((s32)unit + 0x168);
        psndSFXOn_3D(*(s32*)((s32)unit + 0x190), pos);
    }
    return 1;
}

USER_FUNC(btlevtcmd_JumpContinue) {
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (isFirstCall != 0) {
        *(u8*)((s32)event + 0xD) = 1;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
    }

    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x178);
    *(f32*)((s32)unit + 0x178) -= *(f32*)((s32)unit + 0x174);
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));

    floor = BattleGetFloorHeight(_battleWorkPointer, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    if (*(f32*)((s32)unit + 0x158) < floor) {
        *(f32*)((s32)unit + 0x158) = floor;
        BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
        return 1;
    }

    return 0;
}

USER_FUNC(btlevtcmd_FallPosition) {
    extern void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern f32 float_0_80422280;
    extern f32 float_1_8042228c;
    extern f32 float_0p5_804222a0;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, type);
    f32 targetX = (f32)evtGetValue(event, args[1]);
    f32 targetY = (f32)evtGetValue(event, args[2]);
    f32 targetZ = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    if (isFirstCall != 0) {
        f32 x;
        f32 y;
        f32 z;
        f32 dy;

        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)((s32)unit + 0x16C) = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, targetX, targetY, targetZ);
        *(f32*)((s32)unit + 0x17C) = angleABf(x, z, targetX, targetZ);
        *(f32*)((s32)unit + 0x180) = distABf(x, z, targetX, targetZ);
        if (targetX - x > float_0_80422280) {
            *(s8*)((s32)unit + 0x184) = 1;
        } else if (targetX - x < float_0_80422280) {
            *(s8*)((s32)unit + 0x184) = -1;
        }
        dy = targetY - y;
        if (*(s32*)((s32)unit + 0x16C) == 0) {
            if (*(f32*)((s32)unit + 0x170) == float_0_80422280) {
                *(f32*)((s32)unit + 0x170) = float_1_8042228c;
            }
            *(s32*)((s32)unit + 0x16C) = (s32)(*(f32*)((s32)unit + 0x180) / *(f32*)((s32)unit + 0x170));
            if (*(s32*)((s32)unit + 0x16C) == 0) {
                *(s32*)((s32)unit + 0x16C) = 1;
            }
        } else {
            *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x180) / (f32)*(s32*)((s32)unit + 0x16C);
        }
        *(f32*)((s32)unit + 0x178) = float_0_80422280;
        *(f32*)((s32)unit + 0x174) = (dy / (f32)*(s32*)((s32)unit + 0x16C) - *(f32*)((s32)unit + 0x178)) /
                                    ((f32)-*(s32*)((s32)unit + 0x16C) * float_0p5_804222a0);
    }

    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x178);
    *(f32*)((s32)unit + 0x178) -= *(f32*)((s32)unit + 0x174);
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    *(s32*)((s32)unit + 0x16C) = *(s32*)((s32)unit + 0x16C) - 1;
    if (*(s32*)((s32)unit + 0x16C) < 1) {
        BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164), *(f32*)((s32)unit + 0x168));
        return 1;
    }
    return 0;
}

USER_FUNC(btlevtcmd_MovePosition) {
    extern void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_LoadSeMode(s32 mode, s32 sound, void* soundData, void* soundWork);
    extern void _UnitMoveSoundControl(void* unit);
    extern void _UnitMoveSoundControlEnd(void* unit);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern f32 float_0_80422280;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, type);
    f32 targetX = (f32)evtGetValue(event, args[1]);
    f32 targetY = (f32)evtGetValue(event, args[2]);
    f32 targetZ = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 sound = evtGetValue(event, args[5]);
    s32 mode = evtGetValue(event, args[6]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    if (isFirstCall != 0) {
        f32 x;
        f32 y;
        f32 z;

        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)((s32)unit + 0x16C) = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, targetX, targetY, targetZ);
        *(f32*)((s32)unit + 0x17C) = angleABf(x, z, targetX, targetZ);
        *(f32*)((s32)unit + 0x180) = distABf(x, z, targetX, targetZ);
        if (targetX - x > float_0_80422280) {
            *(s8*)((s32)unit + 0x188) = 1;
        } else if (targetX - x < float_0_80422280) {
            *(s8*)((s32)unit + 0x188) = -1;
        }
        if (*(s32*)((s32)unit + 0x16C) == 0) {
            *(s32*)((s32)unit + 0x16C) = (s32)(*(f32*)((s32)unit + 0x180) / *(f32*)((s32)unit + 0x170));
            if (*(s32*)((s32)unit + 0x16C) == 0) {
                *(s32*)((s32)unit + 0x16C) = 1;
            }
            *(f32*)((s32)unit + 0x170) +=
                -((f32)*(s32*)((s32)unit + 0x16C) * *(f32*)((s32)unit + 0x170) - *(f32*)((s32)unit + 0x180)) /
                (f32)*(s32*)((s32)unit + 0x16C);
        } else {
            *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x180) / (f32)*(s32*)((s32)unit + 0x16C);
        }
        BtlUnit_LoadSeMode(mode, sound, (void*)((s32)unit + 0x1C4), (void*)((s32)unit + 0x18C));
    }

    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    *(s32*)((s32)unit + 0x16C) = *(s32*)((s32)unit + 0x16C) - 1;
    if (*(s32*)((s32)unit + 0x16C) > 0) {
        _UnitMoveSoundControl(unit);
        return 0;
    }
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164), *(f32*)((s32)unit + 0x168));
    _UnitMoveSoundControlEnd(unit);
    return 1;
}

USER_FUNC(btlevtcmd_GetMoveFrame) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 targetX;
    s32 targetZ;
    s32 dst;
    s32 frames;
    f32 speed;
    f32 dist;
    f32 x;
    f32 y;
    f32 z;
    f32 rem;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    targetX = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    targetZ = evtGetValue(event, args[3]);
    speed = evtGetFloat(event, args[4]);
    dst = args[5];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &x, &y, &z);

    dist = distABf(x, z, (f32)targetX, (f32)targetZ);

    if (speed == 0.0f) {
        speed = *(f32*)((s32)unit + 0x170);
        if (speed == 0.0f) {
            speed = 1.0f;
        }
    }

    frames = (s32)(dist / speed);
    rem = dist - (f32)frames * speed;

    if (rem > 0.0f) {
        frames++;
    }

    if (frames == 0) {
        frames = 1;
    }

    if (dst != -250000000) {
        evtSetValue(event, dst, frames);
    }

    return EVT_RETURN_DONE;
}


s32 btlevtcmd_DivePosition(EventEntry* event, BOOL isFirstCall) {
    extern void BtlUnit_SetMoveStartPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_LoadSeMode(s32, s32, void*, void*);
    extern void _UnitMoveSoundControl(void*);
    extern void _UnitMoveSoundControlEnd(void*);
    extern f32 intpl_sub(f32, f32, s32, s32, s32);
    extern f32 sinfd(f32);
    extern f32 sqrtf(f32);
    s32* args = event->args;
    s32 unitId = BattleTransID(event, evtGetValue(event, args[0]));
    f32 tx = (f32)evtGetValue(event, args[1]);
    f32 ty = (f32)evtGetValue(event, args[2]);
    f32 tz = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 height = evtGetValue(event, args[5]);
    s32 intplType = evtGetValue(event, args[6]);
    s32 mode = evtGetValue(event, args[7]);
    s32 sound = evtGetValue(event, args[8]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    BOOL floorMode = intplType > 99;
    f32 x, y, z, dx, dy, dz, dist, extra;

    if (floorMode) intplType -= 100;
    if (isFirstCall) {
        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)((s32)unit + 0x16C) = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveStartPos(unit, x, y, z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
        dx = x - tx; dy = y - ty; dz = z - tz;
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        *(f32*)((s32)unit + 0x180) = dist;
        if (frames == 0) *(s32*)((s32)unit + 0x16C) = (s32)(dist / *(f32*)((s32)unit + 0x170));
        else *(f32*)((s32)unit + 0x170) = dist / frames;
        if (*(s32*)((s32)unit + 0x16C) == 0) return EVT_RETURN_DONE;
        *(f32*)((s32)unit + 0x178) = 0.0f;
        *(f32*)((s32)unit + 0x17C) = 0.0f;
        *(s8*)((s32)unit + 0x188) = tx > x ? 1 : (tx < x ? -1 : *(s8*)((s32)unit + 0x188));
        BtlUnit_LoadSeMode(3, sound, (u8*)unit + 0x1C8, (u8*)unit + 0x18C);
    }
    *(f32*)((s32)unit + 0x154) = intpl_sub(*(f32*)((s32)unit + 0x148), *(f32*)((s32)unit + 0x160), intplType, (s32)*(f32*)((s32)unit + 0x178), *(s32*)((s32)unit + 0x16C));
    *(f32*)((s32)unit + 0x158) = intpl_sub(*(f32*)((s32)unit + 0x14C), *(f32*)((s32)unit + 0x164), intplType, (s32)*(f32*)((s32)unit + 0x178), *(s32*)((s32)unit + 0x16C));
    *(f32*)((s32)unit + 0x15C) = intpl_sub(*(f32*)((s32)unit + 0x150), *(f32*)((s32)unit + 0x168), intplType, (s32)*(f32*)((s32)unit + 0x178), *(s32*)((s32)unit + 0x16C));
    *(f32*)((s32)unit + 0x178) += 1.0f;
    if (*(f32*)((s32)unit + 0x178) > *(s32*)((s32)unit + 0x16C)) {
        BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164), *(f32*)((s32)unit + 0x168));
        _UnitMoveSoundControlEnd(unit);
        return EVT_RETURN_DONE;
    }
    dx = *(f32*)((s32)unit + 0x160) - *(f32*)((s32)unit + 0x154);
    dy = *(f32*)((s32)unit + 0x164) - *(f32*)((s32)unit + 0x158);
    dz = *(f32*)((s32)unit + 0x168) - *(f32*)((s32)unit + 0x15C);
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist == 0.0f) dist = 1.0f;
    if (*(f32*)((s32)unit + 0x180) == 0.0f) *(f32*)((s32)unit + 0x180) = 1.0f;
    extra = sinfd(180.0f * (1.0f - dist / *(f32*)((s32)unit + 0x180))) * height;
    if (mode == 0) BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158) + extra, *(f32*)((s32)unit + 0x15C));
    else BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158) - extra, *(f32*)((s32)unit + 0x15C));
    _UnitMoveSoundControl(unit);
    return 0;
}

s32 btlevtcmd_EllipseMovePosition(EventEntry* event, BOOL isFirstCall) {
    extern void BtlUnit_SetMoveStartPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetPos(BattleWorkUnit*, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 intpl_sub(f32, f32, s32, s32, s32);
    extern f32 sinfd(f32);
    extern f32 cosfd(f32);
    extern f32 sqrtf(f32);
    s32* args = event->args;
    s32 unitId = BattleTransID(event, evtGetValue(event, args[0]));
    f32 targetX = (f32)evtGetValue(event, args[1]);
    f32 targetY = (f32)evtGetValue(event, args[2]);
    f32 targetZ = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    f32 radius = (f32)evtGetValue(event, args[5]);
    s32 intplType = evtGetValue(event, args[6]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    f32 x, y, z;
    f32 dx, dy, dz;
    f32 midX, midZ, diameter, angle, theta;

    if (isFirstCall) {
        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)((s32)unit + 0x16C) = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetMoveStartPos(unit, x, y, z);
        BtlUnit_SetMoveCurrentPos(unit, x, y, z);
        BtlUnit_SetMoveTargetPos(unit, targetX, targetY, targetZ);
        dx = x - targetX;
        dy = y - targetY;
        dz = z - targetZ;
        *(f32*)((s32)unit + 0x184) = sqrtf(dx * dx + dy * dy + dz * dz);
        if (*(s32*)((s32)unit + 0x16C) == 0) {
            *(s32*)((s32)unit + 0x16C) = (s32)(*(f32*)((s32)unit + 0x184) / *(f32*)((s32)unit + 0x170));
        } else {
            *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x184) / *(s32*)((s32)unit + 0x16C);
        }
        if (*(s32*)((s32)unit + 0x16C) == 0) return EVT_RETURN_DONE;
        *(f32*)((s32)unit + 0x178) = 0.0f;
        *(f32*)((s32)unit + 0x17C) = 0.0f;
        *(s8*)((s32)unit + 0x188) = targetX > x ? 1 : (targetX < x ? -1 : *(s8*)((s32)unit + 0x188));
    }

    midX = (*(f32*)((s32)unit + 0x148) + *(f32*)((s32)unit + 0x160)) * 0.5f;
    midZ = (*(f32*)((s32)unit + 0x150) + *(f32*)((s32)unit + 0x168)) * 0.5f;
    dx = *(f32*)((s32)unit + 0x160) - *(f32*)((s32)unit + 0x148);
    dz = *(f32*)((s32)unit + 0x168) - *(f32*)((s32)unit + 0x150);
    diameter = sqrtf(dx * dx + dz * dz);
    angle = angleABf(midX, midZ, *(f32*)((s32)unit + 0x148), *(f32*)((s32)unit + 0x150));
    theta = intpl_sub(180.0f, 0.0f, intplType, (s32)*(f32*)((s32)unit + 0x178), *(s32*)((s32)unit + 0x16C));
    *(f32*)((s32)unit + 0x154) = midX - ((diameter * 0.5f * sinfd(angle) * cosfd(theta)) + (radius * cosfd(angle) * sinfd(theta)));
    *(f32*)((s32)unit + 0x15C) = -((radius * sinfd(angle) * sinfd(theta)) - (diameter * 0.5f * cosfd(angle) * cosfd(theta) + midZ));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    *(f32*)((s32)unit + 0x178) += 1.0f;
    if (*(f32*)((s32)unit + 0x178) <= *(s32*)((s32)unit + 0x16C)) return 0;
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164), *(f32*)((s32)unit + 0x168));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_MoveDirectionAdd) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 amount;
    s32 product;
    s8 dir;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    dst = args[1];
    amount = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(battleWork, id);
    dir = *(s8*)((s32)unit + 0x188);

    if (dir != 0) {
        product = amount * dir;
    } else {
        product = amount * *(s8*)((s32)unit + 0x189);
    }

    evtSetValue(event, dst, evtGetValue(event, dst) + product);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_FaceDirectionAdd) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 amount;
    s32 product;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    dst = args[1];
    amount = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(battleWork, id);
    product = amount * *(s8*)((s32)unit + 0x189);

    evtSetValue(event, dst, evtGetValue(event, dst) + product);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_FaceDirectionSub) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 amount;
    s32 product;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    dst = args[1];
    amount = evtGetValue(event, args[2]);

    unit = BattleGetUnitPtr(battleWork, id);
    product = amount * *(s8*)((s32)unit + 0x189);

    evtSetValue(event, dst, evtGetValue(event, dst) - product);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_PartsFaceDirectionSub) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 dst;
    s32 amount;
    s32 product;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    dst = args[2];
    amount = evtGetValue(event, args[3]);

    unit = BattleGetUnitPtr(battleWork, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    product = amount * *(s8*)((s32)part + 0xBD);
    evtSetValue(event, dst, evtGetValue(event, dst) - product);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SamplingEnemy) {
    extern s32 BattleSamplingEnemy(void* targetWork, void* weapon, s32 unitId, s32 belong, u32 targetClassFlags, u32 targetPropertyFlags, s32 direction);
    BattleWork* battleWork;
    s32* args;
    void* targetWork;
    void* weapon;
    s32 type;
    s32 unitId;
    s32 belong;
    s32 direction;
    u32 flags;

    args = event->args;
    battleWork = _battleWorkPointer;
    targetWork = (void*)((s32)battleWork + 0x428);

    type = evtGetValue(event, args[0]);
    unitId = BattleTransID(event, type);
    belong = (s8)evtGetValue(event, args[1]);
    weapon = (void*)evtGetValue(event, args[2]);

    flags = *(u32*)((s32)weapon + 0x68);
    direction = -(s8)*(u8*)((s32)battleWork + belong * 8 + 0xA);
    if ((flags & 0x10000000) != 0) {
        direction = -direction;
    }

    BattleSamplingEnemy(targetWork, weapon, unitId, belong, *(u32*)((s32)weapon + 0x64), flags, direction);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ChoiceSamplingEnemy) {
    extern void BattleChoiceSamplingEnemy(void* targetWork, u32 targetWeightingFlags, s32* unitId, s32* partsId);
    s32* args;
    BattleWork* battleWork;
    void* weapon;
    s32 dstUnit;
    s32 dstPart;
    s32 unitId;
    s32 partsId;

    args = event->args;
    battleWork = _battleWorkPointer;

    weapon = (void*)evtGetValue(event, args[0]);
    dstUnit = args[1];
    dstPart = args[2];

    BattleChoiceSamplingEnemy((void*)((s32)battleWork + 0x428), *(u32*)((s32)weapon + 0x7C), &unitId, &partsId);

    evtSetValue(event, dstUnit, unitId);
    evtSetValue(event, dstPart, partsId);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetSelectEnemy) {
    s32* args;
    BattleWork* battleWork;
    s8* selectWork;
    s32 dstUnit;
    s32 dstPart;
    s32 index;
    s16* entry;

    args = event->args;
    battleWork = _battleWorkPointer;
    selectWork = (s8*)((s32)battleWork + 0x428);

    dstUnit = args[0];
    dstPart = args[1];

    if (*(s8*)((s32)battleWork + 0xE94) <= 0) {
        evtSetValue(event, dstUnit, -1);
        evtSetValue(event, dstPart, -1);
        return EVT_RETURN_DONE;
    }

    index = selectWork[0xA6D + selectWork[0xAB7]];
    entry = (s16*)(selectWork + 0x4 + index * 0x24);

    evtSetValue(event, dstUnit, entry[0]);
    evtSetValue(event, dstPart, entry[1]);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetSelectNextEnemy) {
    s32* args;
    BattleWork* battleWork;
    s8* selectWork;
    s32 dstUnit;
    s32 dstPart;
    s32 index;
    s16* entry;

    args = event->args;
    battleWork = _battleWorkPointer;
    selectWork = (s8*)((s32)battleWork + 0x428);

    dstUnit = args[0];
    dstPart = args[1];

    selectWork[0xAB7] = *(u8*)((s32)battleWork + 0xEDF) + 1;

    if (selectWork[0xAB7] < selectWork[0xA6C]) {
        index = selectWork[0xA6D + selectWork[0xAB7]];
        entry = (s16*)(selectWork + 0x4 + index * 0x24);

        evtSetValue(event, dstUnit, entry[0]);
        evtSetValue(event, dstPart, entry[1]);
    } else {
        evtSetValue(event, dstUnit, -1);
        evtSetValue(event, dstPart, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetFirstAttackTarget) {
    s32* args;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 dstUnit;
    s32 dstPart;
    s32 bodyId;

    args = event->args;
    dstUnit = args[0];
    dstPart = args[1];

    BattleGetFirstAttackUnit(_battleWorkPointer, &part, &unit);

    bodyId = BtlUnit_GetBodyPartsId(unit);

    evtSetValue(event, dstUnit, *(s32*)unit);
    evtSetValue(event, dstPart, bodyId);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetConsumeItem) {
    BattleWork* battleWork;
    s32* args;
    s32 index;
    s32 base;
    s32 value;

    battleWork = _battleWorkPointer;
    args = event->args;

    index = *(s32*)((s32)battleWork + 0x1C38);
    base = (s32)battleWork + 0x171C;
    base += index * 0x1C;
    value = *(s32*)(base + 0x94);

    evtSetValue(event, args[0], value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_DispItemIcon) {
    s32* args;
    s32 x;
    s32 y;
    s32 z;
    s32 item;
    s32 frames;

    args = event->args;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    item = evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = frames;
        *(s32*)((s32)event + 0x7C) = itemEntry(0, item, 0x11, -1, 0, (f32)x, (f32)y, (f32)z);
        return 0;
    }

    if (--*(s32*)((s32)event + 0x78) > 0) {
        return 0;
    }

    itemDelete((void*)(*(s32*)((s32)event + 0x7C) + 0xC));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CommandFlyItem) {
    extern f32 float_10_804222dc;
    extern f32 float_12_804222e4;
    extern f32 float_5_804222e0;
    extern char vec3_802ee458[];
    extern void* effItemGetEntry(s32 type, f32 x, f32 y, f32 z);
    extern void psndSFXOn_3D(char* name, void* pos);
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 unitId;
    s32 frames;
    s32 item;
    s32 kind;
    f32 x;
    f32 y;
    f32 z;
    s32 base;

    args = event->args;
    type = evtGetValue(event, args[0]);
    frames = evtGetValue(event, args[1]);
    unitId = BattleTransID(event, type);

    if (isFirstCall != 0) {
        battleWork = _battleWorkPointer;
        unit = BattleGetUnitPtr(battleWork, unitId);
        kind = *(s32*)((s32)unit + 8);
        if (kind == 0xDE || (kind >= 0xE0 && kind < 0xE7)) {
            base = (s32)battleWork + 0x171C;
            item = *(s32*)(base + (*(s32*)((s32)battleWork + 0x1C38) * 0x1C) + 0x94);
        } else {
            item = *(s32*)((s32)unit + 0x308);
        }

        *(s32*)((s32)event + 0x78) = frames;
        BtlUnit_GetPos(unit, &x, &y, &z);
        y += float_10_804222dc;
        if ((*(u32*)((s32)unit + 0x104) & 0x10) == 0) {
            if (*(u32*)((s32)unit + 0x104) & 2) {
                y -= float_5_804222e0;
            } else {
                y += *(f32*)((s32)unit + 0x114) * (f32)BtlUnit_GetHeight(unit);
            }
        }

        *(s32*)((s32)event + 0x7C) = itemEntry(0, item, 0x11, -1, 0, x, y, z);
        effItemGetEntry(0xA, x, y + float_12_804222e4, z - float_5_804222e0);

        if (kind == 0xDE) {
            psndSFXOn_3D(vec3_802ee458 + 0x2F0, &x);
        } else if (kind >= 0xE0 && kind < 0xE7) {
            psndSFXOn_3D(vec3_802ee458 + 0x304, &x);
        } else {
            psndSFXOn_3D(vec3_802ee458 + 0x318, &x);
        }
        return 0;
    }

    *(s32*)((s32)event + 0x78) -= 1;
    if (*(s32*)((s32)event + 0x78) > 0) {
        return 0;
    }

    itemDelete((void*)(*(s32*)((s32)event + 0x7C) + 0xC));
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ConsumeItem) {
    extern s32 pouchRemoveItemIndex(s32 item, s32 index);
    extern s32 pouchAddCoin(s32 coins);
    extern void effCoinN64Entry(s32 a, s32 b, f32 x, f32 y, f32 z, f32 scale);
    extern f32 float_100_804222d8;
    extern f32 float_1_8042228c;
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 unitId;
    s32 kind;
    s32 item;
    s32 itemIndex;
    u8* data;
    s32 coin;
    s32 base;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unitId = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, unitId);

    kind = *(s32*)((s32)unit + 8);
    if (kind == 0xDE || (kind >= 0xE0 && kind < 0xE7)) {
        base = (s32)battleWork + 0x171C + *(s32*)((s32)battleWork + 0x1C38) * 0x1C;
        item = *(s32*)(base + 0x94);
        itemIndex = *(s32*)(base + 0x90);
        data = itemDataTable + item * 0x28;

        if ((*(u16*)(data + 0x10) & 1) != 0) {
            pouchRemoveItemIndex(item, itemIndex);
        }

        if (*(u8*)((s32)unit + 0x2FB) != 0) {
            coin = (s32)(((f32)(*(u8*)((s32)unit + 0x2FB) * 5 + 0x46) * (f32)*(u16*)(data + 0x1A)) / float_100_804222d8);
            if (coin > 0) {
                pouchAddCoin(coin);
                BtlUnit_GetPos(unit, &x, &y, &z);
                effCoinN64Entry(0, 0, x, y, z, float_1_8042228c);
            }
        }
    } else {
        *(s32*)((s32)unit + 0x308) = 0;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ConsumeItemReserve) {
    extern s32 pouchAddCoin(s32 coins);
    extern void effCoinN64Entry(s32 a, s32 b, f32 x, f32 y, f32 z, f32 scale);
    extern f32 float_100_804222d8;
    extern f32 float_1_8042228c;
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 item;
    s32 unitId;
    s32 kind;
    u8* data;
    s32 i;
    s32 coin;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    battleWork = _battleWorkPointer;
    type = evtGetValue(event, args[0]);
    item = evtGetValue(event, args[1]);
    unitId = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, unitId);

    kind = *(s32*)((s32)unit + 8);
    if (kind == 0xDE || (kind >= 0xE0 && kind < 0xE7)) {
        data = itemDataTable + item * 0x28;
        if ((*(u16*)(data + 0x10) & 1) != 0) {
            for (i = 0; i < 4; i++) {
                if (*(s32*)((s32)battleWork + 0x19060 + i * 4) == item) {
                    break;
                }
                if (*(s32*)((s32)battleWork + 0x19060 + i * 4) == 0) {
                    *(s32*)((s32)battleWork + 0x19060 + i * 4) = item;
                    break;
                }
            }
        }

        if (*(u8*)((s32)unit + 0x2FB) != 0) {
            coin = (s32)(((f32)(*(u8*)((s32)unit + 0x2FB) * 5 + 0x46) * (f32)*(u16*)(data + 0x1A)) / float_100_804222d8);
            if (coin > 0) {
                pouchAddCoin(coin);
                BtlUnit_GetPos(unit, &x, &y, &z);
                effCoinN64Entry(0, 0, x, y, z, float_1_8042228c);
            }
        }
    } else if (*(s32*)((s32)unit + 0x308) == 0x97) {
        *(s32*)((s32)unit + 0x308) = 0;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AttackDeclare) {
    extern void BattleAttackDeclare(s32* unitId, s32* partId);
    s32* args;
    s32 type;
    s32 dstUnit;
    s32 dstPart;
    s32 unitId;
    s32 partId;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unitId = BattleTransID(event, type);

    dstUnit = args[1];
    dstPart = args[2];

    unitId = BattleTransID(event, evtGetValue(event, dstUnit));
    partId = evtGetValue(event, dstPart);

    BattleAttackDeclare(&unitId, &partId);

    evtSetValue(event, dstUnit, unitId);
    evtSetValue(event, dstPart, partId);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AttackDeclareAll) {
    BattleWork* battleWork;
    s32 type;

    battleWork = _battleWorkPointer;

    type = evtGetValue(event, event->args[0]);
    BattleTransID(event, type);
    BattleAttackDeclareAll(battleWork);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_PreCheckDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 partsId;
    s32 result;
    s32 flags;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    attacker = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    type = evtGetValue(event, args[1]);
    target = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    weapon = (void*)evtGetValue(event, args[3]);
    flags = args[4];
    dst = args[5];

    result = BattlePreCheckDamage(attacker, target, part, weapon, flags);

    if (result < 0) {
        return 0xFF;
    }

    evtSetValue(event, dst, result);

    if (evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
        return EVT_RETURN_DONE;
    }

    return 0xFF;
}







USER_FUNC(btlevtcmd_CommandPreCheckDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 partsId;
    s32 result;
    s32 dst;
    s32 flags;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    attacker = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    type = evtGetValue(event, args[1]);
    target = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    flags = args[3];
    dst = args[4];
    weapon = BattleGetSelectWeapon(battleWork);

    result = BattlePreCheckDamage(attacker, target, part, weapon, flags);

    if (result < 0) {
        return 0xFF;
    }

    evtSetValue(event, dst, result);

    if (evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
        return EVT_RETURN_DONE;
    }

    return 0xFF;
}

USER_FUNC(btlevtcmd_PreCheckCounter) {
    s32 result;
    u8 work[0x3C];
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 id;
    s32 partsId;
    s32 dst;
    s32 check;

    args = event->args;
    battleWork = _battleWorkPointer;
    result = 0;

    BattleInitCounterPreCheckWork(work);

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    attacker = BattleGetUnitPtr(battleWork, id);

    type = evtGetValue(event, args[1]);
    id = BattleTransID(event, type);
    target = BattleGetUnitPtr(battleWork, id);

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    weapon = (void*)evtGetValue(event, args[3]);
    dst = args[5];

    result = BattleCheckCounter(work, attacker, target, part, weapon, &result);

    evtSetValue(event, dst, result);

    check = evtCheckID(*(s32*)((s32)event + 0x15C));
    if (check == 0) {
        return 0xFF;
    }

    return EVT_RETURN_DONE;
}





USER_FUNC(btlevtcmd_CommandPreCheckCounter) {
    s32 result;
    u8 work[0x3C];
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 id;
    s32 partsId;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;
    result = 0;

    BattleInitCounterPreCheckWork(work);

    type = evtGetValue(event, args[0]);
    attacker = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    type = evtGetValue(event, args[1]);
    id = BattleTransID(event, type);
    target = BattleGetUnitPtr(battleWork, id);

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    dst = args[4];
    weapon = BattleGetSelectWeapon(battleWork);

    result = BattleCheckCounter(work, attacker, target, part, weapon, &result);

    evtSetValue(event, dst, result);

    if (evtCheckID(*(s32*)((s32)event + 0x15C)) == 0) {
        return 0xFF;
    }

    return EVT_RETURN_DONE;
}






USER_FUNC(btlevtcmd_CheckDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 partsId;
    s32 result;
    s32 flags;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    attacker = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    type = evtGetValue(event, args[1]);
    target = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    weapon = (void*)evtGetValue(event, args[3]);
    flags = args[4];
    dst = args[5];

    result = BattleCheckDamage(attacker, target, part, weapon, flags);

    if (result < 0) {
        return 0xFF;
    }

    evtSetValue(event, dst, result);

    if (evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
        return EVT_RETURN_DONE;
    }

    return 0xFF;
}







USER_FUNC(btlevtcmd_CommandCheckDamage) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* attacker;
    BattleWorkUnit* target;
    BattleWorkUnitPart* part;
    void* weapon;
    s32 type;
    s32 partsId;
    s32 result;
    s32 dst;
    s32 flags;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    attacker = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    type = evtGetValue(event, args[1]);
    target = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    partsId = evtGetValue(event, args[2]);
    part = BtlUnit_GetPartsPtr(target, partsId);

    flags = args[3];
    dst = args[4];
    weapon = BattleGetSelectWeapon(battleWork);

    result = BattleCheckDamage(attacker, target, part, weapon, flags);

    if (result < 0) {
        return 0xFF;
    }

    evtSetValue(event, dst, result);

    if (evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
        return EVT_RETURN_DONE;
    }

    return 0xFF;
}

USER_FUNC(btlevtcmd_DamageDirect) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 partsId;
    s32 damage;
    s32 flags;
    s32 value1;
    s32 value2;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    partsId = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    damage = evtGetValue(event, args[2]);
    flags = args[3];

    value1 = evtGetValue(event, args[4]);
    value2 = evtGetValue(event, args[5]);

    BattleDamageDirect(-5, unit, part, damage, 0, flags, value1, value2);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetEventDamage) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x2B0) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventWait) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x28C) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventEntry) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x2B8) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventPhase) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x298) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventUnisonPhase) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x294) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventAttack) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x2A0) = args[1];
    *(s32*)((s32)unit + 0x2AC) = 0;

    return EVT_RETURN_DONE;
}





USER_FUNC(btlevtcmd_SetEventConfusion) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x2A4) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetEventCeilFall) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    *(s32*)((s32)unit + 0x2C0) = args[1];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StartWaitEvent) {
    BattleWorkUnit* unit;
    s32 type;

    type = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    if (*(u32*)((s32)unit + 0x28C) != 0) {
        BattleRunWaitEvent(unit);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_StopWaitEvent) {
    BattleWorkUnit* unit;
    s32 type;
    s32 waitId;

    type = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    waitId = *(s32*)((s32)unit + 0x290);
    if (waitId != 0) {
        evtDeleteID(waitId);
        *(s32*)((s32)unit + 0x290) = 0;
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_GuardMoveEnd) {
    *(u32*)((s32)_battleWorkPointer + 0xEF4) &= ~1;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WaitGuardMove) {
    if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 1) != 0) {
        return 0;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_WaitAttackEnd) {
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 id;

    battleWork = _battleWorkPointer;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));

    if (unit == 0) {
        return EVT_RETURN_DONE;
    }

    id = *(s32*)((s32)unit + 0x2A8);

    if (id == 0) {
        return EVT_RETURN_DONE;
    }

    if (evtCheckID(id) != 0) {
        return 0;
    }

    *(s32*)((s32)unit + 0x2A8) = 0;
    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_WaitAttackEndCheck) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 dst;
    s32 id;

    args = event->args;
    battleWork = _battleWorkPointer;

    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    dst = args[0];

    evtSetValue(event, dst, 1);

    if (unit == 0) {
        return EVT_RETURN_DONE;
    }

    id = *(s32*)((s32)unit + 0x2A8);

    if (id == 0) {
        return EVT_RETURN_DONE;
    }

    if (evtCheckID(id) != 0) {
        evtSetValue(event, dst, 0);
    } else {
        *(s32*)((s32)unit + 0x2A8) = 0;
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_RunDataEventChild) {
    s32* args;
    BattleWorkUnit* unit;
    void* data;
    void* child;
    s32 type;
    s32 id;
    s32 dataId;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dataId = evtGetValue(event, args[1]);
    data = BtlUnit_GetData(unit, dataId);

    if (data == 0) {
        return EVT_RETURN_DONE;
    }

    child = evtChildEntry(event, data, 0);
    *(u8*)((s32)event + 0xA) = 0;
    *(s32*)((s32)child + 0x160) = id;

    return 0xFF;
}



USER_FUNC(btlevtcmd_CheckDataOfDataTable) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    dst = args[1];
    value = evtGetValue(event, args[2]);

    evtSetValue(event, dst, BtlUnit_CheckData(unit, value));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetDataFromDataTable) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    dst = args[1];
    value = evtGetValue(event, args[2]);

    evtSetValue(event, dst, (s32)BtlUnit_GetData(unit, value));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_PhaseEventStartDeclare) {
    BattleWorkUnit* unit;
    s32 type;

    type = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    BattlePhaseEventStartDeclare(unit);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetConfuseActEvent) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 count;
    void* evt;

    args = event->args;
    battleWork = _battleWorkPointer;
    count = 0;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, id);

    dst = args[1];
    evt = 0;

    while (evt == 0) {
        evt = BattleSetConfuseAct(battleWork, unit);
        count++;
        if (count > 0x14) {
            evt = subsetevt_confuse_flustered;
        }
    }

    evtSetValue(event, dst, (s32)evt);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_MarioJumpParam) {
    extern f32 float_60_804222d4;
    extern f32 float_0p15_804222d0;
    s32* args;
    s32 type;
    s32 unitId;
    f32 targetX;
    f32 targetZ;
    s32 outArg;
    s32 frame;
    s32 computed;
    BattleWorkUnit* unit;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unitId = BattleTransID(event, type);
    targetX = (f32)evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    targetZ = (f32)evtGetValue(event, args[3]);
    outArg = args[4];
    frame = evtGetValue(event, outArg);

    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    BtlUnit_GetPos(unit, &x, &y, &z);

    dist = distABf(x, z, targetX, targetZ);
    if (frame != 0) {
        computed = frame;
    } else {
        computed = (s32)(dist / *(f32*)((s32)unit + 0x170));
    }
    if (dist > float_60_804222d4) {
        computed += (s32)(float_0p15_804222d0 * (dist - float_60_804222d4));
    }
    if (frame == 0 || computed > frame) {
        frame = computed;
    }
    if (outArg != -250000000) {
        evtSetValue(event, outArg, frame);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_MarioJumpPosition) {
    extern void BtlUnit_SetMoveTargetPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_SetMoveCurrentPos(BattleWorkUnit*, f32, f32, f32);
    extern void BtlUnit_GetPos(BattleWorkUnit*, f32*, f32*, f32*);
    extern void BtlUnit_SetPos(BattleWorkUnit*, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 distABf(f32, f32, f32, f32);
    extern void btlMovePos(f32*, f32*, f32, f32);
    extern void BtlUnit_LoadSeMode(s32, s32, void*, void*);
    s32* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    f32 tx = (f32)evtGetValue(event, args[1]);
    f32 ty = (f32)evtGetValue(event, args[2]);
    f32 tz = (f32)evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    s32 sound = evtGetValue(event, args[6]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32 remainder;

    if (isFirstCall != 0) {
        *(u8*)((s32)event + 0xD) = 1;
        if (mode == 1) {
            *(u8*)((s32)event + 0xD) = 2;
        } else {
            BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
            BtlUnit_GetPos(unit, &x, &y, &z);
            BtlUnit_SetMoveCurrentPos(unit, x, y, z);
            *(f32*)((s32)unit + 0x17C) = angleABf(x, z, tx, tz);
            *(f32*)((s32)unit + 0x180) = distABf(x, z, tx, tz);
            if (tx - x > 0.0f) {
                *(s8*)((s32)unit + 0x188) = 1;
            } else if (tx - x < 0.0f) {
                *(s8*)((s32)unit + 0x188) = -1;
            }
            *(s32*)((s32)unit + 0x16C) = frames;
            if (frames == 0) {
                frames = (s32)(*(f32*)((s32)unit + 0x180) / *(f32*)((s32)unit + 0x170));
                *(s32*)((s32)unit + 0x16C) = frames;
            } else {
                *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x180) / (f32)frames;
            }
            if (frames == 0) {
                return EVT_RETURN_DONE;
            }
            remainder = (f32)frames * *(f32*)((s32)unit + 0x170) - *(f32*)((s32)unit + 0x180);
            *(f32*)((s32)unit + 0x148) = (tx - x) / (f32)frames;
            *(f32*)((s32)unit + 0x14C) = (ty - y) / (f32)frames;
            *(f32*)((s32)unit + 0x150) = (tz - z) / (f32)frames;
            *(f32*)((s32)unit + 0x178) = 0.0f;
            *(f32*)((s32)unit + 0x174) = 3.1416f / (f32)frames;
            *(f32*)((s32)unit + 0x170) += -remainder / (f32)frames;
            BtlUnit_LoadSeMode(2, sound, (void*)((s32)unit + 0x1C4), (void*)((s32)unit + 0x18C));
        }
    }

    if (*(u8*)((s32)event + 0xD) == 2) {
        return EVT_RETURN_BLOCK;
    }
    *(f32*)((s32)unit + 0x154) += *(f32*)((s32)unit + 0x148);
    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x14C);
    *(f32*)((s32)unit + 0x15C) += *(f32*)((s32)unit + 0x150);
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C),
               *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158),
                   *(f32*)((s32)unit + 0x15C));
    *(s32*)((s32)unit + 0x16C) -= 1;
    if (*(s32*)((s32)unit + 0x16C) > 0) {
        return EVT_RETURN_BLOCK;
    }
    BtlUnit_SetPos(unit, tx, ty, tz);
    return EVT_RETURN_DONE;
}

s32 btlevtcmd_GetTakeoffPosition(EventEntry* event, BOOL isFirstCall) {
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    extern s32 BtlUnit_GetEnemyBelong(void*);
    extern s32 BattleSamplingEnemy(void*, void*, s32, s32, u32, u32, s32);
    extern void BattleChoiceSamplingEnemy(void*, u32, s32*, s32*);
    s32* args = event->args;
    void* battleWork = _battleWorkPointer;
    s32 attackerArg = evtGetValue(event, args[0]);
    s32 targetArg = evtGetValue(event, args[1]);
    s32 partId = evtGetValue(event, args[2]);
    s32 dstX = args[3];
    s32 dstY = args[4];
    s32 dstZ = args[5];
    s32 attackerId;
    s32 targetId;
    s32 belong;
    s32 sampledUnit;
    s32 sampledPart;
    s32 choice;
    s8 direction;
    void* attacker;
    void* target;
    void* targetPart;
    void* sampled;
    void* sampledPartPtr;
    f32 attackerX;
    f32 scratch;
    f32 homeX, homeY, homeZ;
    f32 sampledX;
    u8 targetWork[2764];

    attackerId = BattleTransID(event, attackerArg);
    attacker = BattleGetUnitPtr(battleWork, attackerId);
    BtlUnit_GetPos(attacker, &attackerX, &scratch, &scratch);
    targetId = BattleTransID(event, targetArg);
    target = BattleGetUnitPtr(battleWork, targetId);
    targetPart = BtlUnit_GetPartsPtr(target, partId);
    BtlUnit_GetHomePos(attacker, &homeX, &homeY, &homeZ);
    BtlUnit_GetHitPos(target, targetPart, &homeX, &scratch, &homeZ);
    belong = BtlUnit_GetEnemyBelong(attacker);
    direction = *(s8*)((s32)battleWork + 0xA + (s8)belong * 8);
    choice = BattleSamplingEnemy(targetWork, NULL, attackerId, (s8)belong,
                                 0x1001240, 0x1000000, -direction);
    BattleChoiceSamplingEnemy(targetWork, 4, &sampledUnit, &sampledPart);
    if (*(s8*)(targetWork + 0xA6C) < 1) {
        homeX += direction * 60;
    } else {
        sampledUnit = BattleTransID(event, sampledUnit);
        sampled = BattleGetUnitPtr(battleWork, sampledUnit);
        sampledPartPtr = BtlUnit_GetPartsPtr(sampled, sampledPart);
        BtlUnit_GetHitPos(sampled, sampledPartPtr, &sampledX, &scratch, &scratch);
        if (__fabs(attackerX - sampledX) < __fabs(attackerX - homeX)) {
            homeX = sampledX;
        }
        if (targetPart == sampledPartPtr) {
            homeX += direction * (choice == 0 ? 60 : -60);
        } else {
            homeX += direction * (choice == 0 ? 30 : -30);
        }
    }
    evtSetValue(event, dstX, (s32)homeX);
    evtSetValue(event, dstY, (s32)homeY);
    evtSetValue(event, dstZ, (s32)homeZ);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsMoveSpeed) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    f32 speed;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    speed = evtGetFloat(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    BtlUnit_SetPartsMoveSpeed(part, speed);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsFallAccel) {
    s32 type;
    s32 id;
    s32* args;
    s32 partsId;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    f32 accel;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    accel = evtGetFloat(event, args[2]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    BtlUnit_SetPartsFallAccel(part, accel);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_JumpPartsPosition) {
    extern void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetPartsMoveCurrentPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPartsMoveTargetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void BtlUnit_LoadSeMode(s32 mode, s32 sound, void* soundData, void* soundWork);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern void psndSFXOn_3D(s32 sound, void* pos);
    extern f32 float_0_80422280;
    extern f32 float_0p5_804222a0;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, type);
    s32 partsId = evtGetValue(event, args[1]);
    f32 targetX = (f32)evtGetValue(event, args[2]);
    f32 targetY = (f32)evtGetValue(event, args[3]);
    f32 targetZ = (f32)evtGetValue(event, args[4]);
    s32 frames = evtGetValue(event, args[5]);
    s32 sound = evtGetValue(event, args[6]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    BattleWorkUnitPart* part = BtlUnit_GetPartsPtr(unit, partsId);

    if (isFirstCall != 0) {
        f32 x;
        f32 y;
        f32 z;
        f32 dy;

        *(u8*)((s32)event + 0xD) = 1;
        if (frames != 0) {
            *(s32*)((s32)part + 0xA0) = frames;
        }
        BtlUnit_SetPartsMoveTargetPos(part, targetX, targetY, targetZ);
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
        *(f32*)((s32)part + 0xB0) = angleABf(x, z, targetX, targetZ);
        *(f32*)((s32)part + 0xB4) = distABf(x, z, targetX, targetZ);
        if (targetX - x > float_0_80422280) {
            *(s8*)((s32)part + 0xBC) = 1;
        } else if (targetX - x < float_0_80422280) {
            *(s8*)((s32)part + 0xBC) = -1;
        }

        dy = targetY - y;
        if (*(s32*)((s32)part + 0xA0) == 0) {
            *(s32*)((s32)part + 0xA0) = (s32)(*(f32*)((s32)part + 0xB4) / *(f32*)((s32)part + 0xA4));
            *(f32*)((s32)part + 0xB4) -= (f32)*(s32*)((s32)part + 0xA0) * *(f32*)((s32)part + 0xA4);
        } else {
            *(f32*)((s32)part + 0xA4) = *(f32*)((s32)part + 0xB4) / (f32)*(s32*)((s32)part + 0xA0);
            *(f32*)((s32)part + 0xB4) -= (f32)*(s32*)((s32)part + 0xA0) * *(f32*)((s32)part + 0xA4);
        }
        if (*(s32*)((s32)part + 0xA0) == 0) {
            return 2;
        }
        *(f32*)((s32)part + 0xAC) = dy / (f32)*(s32*)((s32)part + 0xA0) +
                                     (*(f32*)((s32)part + 0xA8) * (f32)*(s32*)((s32)part + 0xA0)) * float_0p5_804222a0;
        *(f32*)((s32)part + 0xA4) += *(f32*)((s32)part + 0xB4) / (f32)*(s32*)((s32)part + 0xA0);
        BtlUnit_LoadSeMode(2, sound, (void*)((s32)part + 0xF8), (void*)((s32)part + 0xC0));
        if (*(s32*)((s32)part + 0xC0) != 0) {
            f32 pos[3];
            pos[0] = *(f32*)((s32)part + 0x88);
            pos[1] = *(f32*)((s32)part + 0x8C);
            pos[2] = *(f32*)((s32)part + 0x90);
            psndSFXOn_3D(*(s32*)((s32)part + 0xC0), pos);
        }
    }

    *(f32*)((s32)part + 0x8C) += *(f32*)((s32)part + 0xAC);
    *(f32*)((s32)part + 0xAC) -= *(f32*)((s32)part + 0xA8);
    if (*(f32*)((s32)part + 0xAC) < float_0_80422280 && *(f32*)((s32)part + 0x8C) < *(f32*)((s32)part + 0x98)) {
        *(f32*)((s32)part + 0x8C) = *(f32*)((s32)part + 0x98);
    }
    btlMovePos((f32*)((s32)part + 0x88), (f32*)((s32)part + 0x90), *(f32*)((s32)part + 0xA4), *(f32*)((s32)part + 0xB0));
    BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x88), *(f32*)((s32)part + 0x8C), *(f32*)((s32)part + 0x90));
    *(s32*)((s32)part + 0xA0) = *(s32*)((s32)part + 0xA0) - 1;
    if (*(s32*)((s32)part + 0xA0) > 0) {
        return 0;
    }
    BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x94), *(f32*)((s32)part + 0x98), *(f32*)((s32)part + 0x9C));
    if (*(s32*)((s32)part + 0xC4) != 0) {
        f32 pos[3];
        pos[0] = *(f32*)((s32)part + 0x94);
        pos[1] = *(f32*)((s32)part + 0x98);
        pos[2] = *(f32*)((s32)part + 0x9C);
        psndSFXOn_3D(*(s32*)((s32)part + 0xC4), pos);
    }
    return 1;
}

USER_FUNC(btlevtcmd_JumpPartsSetting) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 frames;
    f32 jumpSpeed;
    f32 fallAccel;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);
    frames = evtGetValue(event, args[2]);
    jumpSpeed = evtGetFloat(event, args[3]);
    fallAccel = evtGetFloat(event, args[4]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    *(s32*)((s32)part + 0xA0) = frames;
    *(f32*)((s32)part + 0xA4) = jumpSpeed;
    *(f32*)((s32)part + 0xA8) = fallAccel;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_JumpPartsContinue) {
    extern void BtlUnit_SetPartsMoveCurrentPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    partsId = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BtlUnit_GetPartsPtr(unit, partsId);

    if (isFirstCall != 0) {
        *(u8*)((s32)event + 0xD) = 1;
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
    }

    *(f32*)((s32)part + 0x8C) += *(f32*)((s32)part + 0xAC);
    *(f32*)((s32)part + 0xAC) -= *(f32*)((s32)part + 0xA8);
    btlMovePos((f32*)((s32)part + 0x88), (f32*)((s32)part + 0x90), *(f32*)((s32)part + 0xA4), *(f32*)((s32)part + 0xB0));
    BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x88), *(f32*)((s32)part + 0x8C), *(f32*)((s32)part + 0x90));

    floor = BattleGetFloorHeight(_battleWorkPointer, *(f32*)((s32)part + 0x88), *(f32*)((s32)part + 0x8C), *(f32*)((s32)part + 0x90));
    if (*(f32*)((s32)part + 0x8C) < floor) {
        *(f32*)((s32)part + 0x8C) = floor;
        BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x88), *(f32*)((s32)part + 0x8C), *(f32*)((s32)part + 0x90));
        return 1;
    }

    return 0;
}

USER_FUNC(btlevtcmd_FallPartsPosition) {
    extern void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetPartsMoveCurrentPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPartsMoveTargetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern f32 float_0_80422280;
    extern f32 float_1_8042228c;
    extern f32 float_0p5_804222a0;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, type);
    s32 partsId = evtGetValue(event, args[1]);
    f32 targetX = (f32)evtGetValue(event, args[2]);
    f32 targetY = (f32)evtGetValue(event, args[3]);
    f32 targetZ = (f32)evtGetValue(event, args[4]);
    s32 frames = evtGetValue(event, args[5]);
    BattleWorkUnit* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    BattleWorkUnitPart* part = BtlUnit_GetPartsPtr(unit, partsId);

    if (isFirstCall != 0) {
        f32 x;
        f32 y;
        f32 z;
        f32 dy;

        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)((s32)part + 0xA0) = frames;
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
        BtlUnit_SetPartsMoveTargetPos(part, targetX, targetY, targetZ);
        *(f32*)((s32)part + 0xB0) = angleABf(x, z, targetX, targetZ);
        *(f32*)((s32)part + 0xB4) = distABf(x, z, targetX, targetZ);

        if (targetX - x > float_0_80422280) {
            *(s8*)((s32)part + 0xB8) = 1;
        } else if (targetX - x < float_0_80422280) {
            *(s8*)((s32)part + 0xB8) = -1;
        }

        dy = targetY - y;
        if (*(s32*)((s32)part + 0xA0) == 0) {
            if (*(f32*)((s32)part + 0xA4) == float_0_80422280) {
                *(f32*)((s32)part + 0xA4) = float_1_8042228c;
            }
            *(s32*)((s32)part + 0xA0) = (s32)(*(f32*)((s32)part + 0xB4) / *(f32*)((s32)part + 0xA4));
            if (*(s32*)((s32)part + 0xA0) == 0) {
                *(s32*)((s32)part + 0xA0) = 1;
            }
        } else {
            *(f32*)((s32)part + 0xA4) = *(f32*)((s32)part + 0xB4) / (f32)*(s32*)((s32)part + 0xA0);
        }
        *(f32*)((s32)part + 0xAC) = float_0_80422280;
        *(f32*)((s32)part + 0xA8) = (dy / (f32)*(s32*)((s32)part + 0xA0) - *(f32*)((s32)part + 0xAC)) /
                                    ((f32)-*(s32*)((s32)part + 0xA0) * float_0p5_804222a0);
    }

    *(f32*)((s32)part + 0x8C) += *(f32*)((s32)part + 0xAC);
    *(f32*)((s32)part + 0xAC) -= *(f32*)((s32)part + 0xA8);
    btlMovePos((f32*)((s32)part + 0x88), (f32*)((s32)part + 0x90), *(f32*)((s32)part + 0xA4), *(f32*)((s32)part + 0xB0));
    BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x88), *(f32*)((s32)part + 0x8C), *(f32*)((s32)part + 0x90));
    *(s32*)((s32)part + 0xA0) = *(s32*)((s32)part + 0xA0) - 1;
    if (*(s32*)((s32)part + 0xA0) < 1) {
        BtlUnit_SetPartsPos(part, *(f32*)((s32)part + 0x94), *(f32*)((s32)part + 0x98), *(f32*)((s32)part + 0x9C));
        return 1;
    }
    return 0;
}

s32 btlevtcmd_DivePartsPosition(EventEntry* event, BOOL isFirstCall) {
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void BtlUnit_GetPartsPos(void*, f32*, f32*, f32*);
    extern void BtlUnit_SetPartsMoveStartPos(void*, f32, f32, f32);
    extern void BtlUnit_SetPartsMoveCurrentPos(void*, f32, f32, f32);
    extern void BtlUnit_SetPartsMoveTargetPos(void*, f32, f32, f32);
    extern void BtlUnit_SetPartsPos(void*, f32, f32, f32);
    extern void BtlUnit_LoadSeMode(s32, s32, void*, void*);
    extern void _PartsMoveSoundControl(void*);
    extern void _PartsMoveSoundControlEnd(void*);
    extern f32 intpl_sub(f32, f32, s32, s32, s32);
    extern f32 sinfd(f32);
    extern f32 sqrtf(f32);
    s32* args = event->args;
    s32 unitId = BattleTransID(event, evtGetValue(event, args[0]));
    s32 partId = evtGetValue(event, args[1]);
    f32 tx = (f32)evtGetValue(event, args[2]);
    f32 ty = (f32)evtGetValue(event, args[3]);
    f32 tz = (f32)evtGetValue(event, args[4]);
    s32 frames = evtGetValue(event, args[5]);
    s32 height = evtGetValue(event, args[6]);
    s32 intplType = evtGetValue(event, args[7]);
    s32 mode = evtGetValue(event, args[8]);
    s32 sound = evtGetValue(event, args[9]);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    u8* part = BtlUnit_GetPartsPtr(unit, partId);
    f32 x, y, z, dx, dy, dz, dist, extra;

    if (intplType > 99) intplType -= 100;
    if (isFirstCall) {
        *(u8*)((s32)event + 0xD) = 1;
        *(s32*)(part + 0xA0) = frames;
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        BtlUnit_SetPartsMoveStartPos(part, x, y, z);
        BtlUnit_SetPartsMoveCurrentPos(part, x, y, z);
        BtlUnit_SetPartsMoveTargetPos(part, tx, ty, tz);
        dx = x - tx; dy = y - ty; dz = z - tz;
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        *(f32*)(part + 0xB4) = dist;
        if (frames == 0) *(s32*)(part + 0xA0) = (s32)(dist / *(f32*)(part + 0xA4));
        else *(f32*)(part + 0xA4) = dist / frames;
        if (*(s32*)(part + 0xA0) == 0) return EVT_RETURN_DONE;
        *(f32*)(part + 0xB8) = 0.0f;
        *(f32*)(part + 0xB0) = 0.0f;
        *(s8*)(part + 0xBC) = tx > x ? 1 : (tx < x ? -1 : *(s8*)(part + 0xBC));
        BtlUnit_LoadSeMode(3, sound, part + 0xF8, part + 0xC0);
    }
    *(f32*)(part + 0x88) = intpl_sub(*(f32*)(part + 0x7C), *(f32*)(part + 0x94), intplType, (s32)*(f32*)(part + 0xB8), *(s32*)(part + 0xA0));
    *(f32*)(part + 0x8C) = intpl_sub(*(f32*)(part + 0x80), *(f32*)(part + 0x98), intplType, (s32)*(f32*)(part + 0xB8), *(s32*)(part + 0xA0));
    *(f32*)(part + 0x90) = intpl_sub(*(f32*)(part + 0x84), *(f32*)(part + 0x9C), intplType, (s32)*(f32*)(part + 0xB8), *(s32*)(part + 0xA0));
    *(f32*)(part + 0xB8) += 1.0f;
    if (*(f32*)(part + 0xB8) > *(s32*)(part + 0xA0)) {
        BtlUnit_SetPartsPos(part, *(f32*)(part + 0x94), *(f32*)(part + 0x98), *(f32*)(part + 0x9C));
        _PartsMoveSoundControlEnd(part);
        return EVT_RETURN_DONE;
    }
    dx = *(f32*)(part + 0x94) - *(f32*)(part + 0x88);
    dy = *(f32*)(part + 0x98) - *(f32*)(part + 0x8C);
    dz = *(f32*)(part + 0x9C) - *(f32*)(part + 0x90);
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist == 0.0f) dist = 1.0f;
    if (*(f32*)(part + 0xB4) == 0.0f) *(f32*)(part + 0xB4) = 1.0f;
    extra = sinfd(180.0f * (1.0f - dist / *(f32*)(part + 0xB4))) * height;
    if (mode == 0) BtlUnit_SetPartsPos(part, *(f32*)(part + 0x88), *(f32*)(part + 0x8C) + extra, *(f32*)(part + 0x90));
    else BtlUnit_SetPartsPos(part, *(f32*)(part + 0x88), *(f32*)(part + 0x8C) - extra, *(f32*)(part + 0x90));
    _PartsMoveSoundControl(part);
    return 0;
}

USER_FUNC(btlevtcmd_AcSetDifficulty) {
    BattleWork* battleWork;
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 difficulty;

    battleWork = _battleWorkPointer;
    args = event->args;

    type = evtGetValue(event, args[0]);
    difficulty = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    BattleActionCommandSetDifficulty(battleWork, unit, difficulty);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcGetDifficulty) {
    s32* args;
    s32 dst;

    args = event->args;
    dst = args[0];

    evtSetValue(event, dst, BattleActionCommandGetDifficulty(_battleWorkPointer));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetFlag) {
    s32* args = event->args;

    *(s32*)((s32)_battleWorkPointer + 0x1CC4) = args[0];

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetParamAll) {
    s32* args;
    BattleWork* battleWork;
    s32 i;
    s32 offset;
    s32 addrOffset;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;
    offset = 0;

    for (i = 0; i < 8; i++) {
        value = evtGetValue(event, *args++);
        if (value != -250000000) {
            addrOffset = offset + 0x1CC8;
            *(s32*)((s32)battleWork + addrOffset) = value;
        }
        offset += 4;
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AcSetParam) {
    s32* args;
    BattleWork* battleWork;
    s32 index;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    index = evtGetValue(event, args[0]);
    value = evtGetValue(event, args[1]);

    *(s32*)((s32)battleWork + 0x1CC8 + index * 4) = value;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcGetParam) {
    BattleWork* battleWork;
    s32* args;
    s32 index;
    s32 dst;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    index = evtGetValue(event, args[0]);
    dst = args[1];
    value = *(s32*)((s32)battleWork + 0x1CC8 + index * 4);

    if (dst != -250000000) {
        evtSetValue(event, dst, value);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetGaugeParam) {
    BattleWork* battleWork;
    s32* args;

    args = event->args;
    battleWork = _battleWorkPointer;

    *(s32*)((s32)battleWork + 0x1D08) = evtGetValue(event, args[0]);
    *(s32*)((s32)battleWork + 0x1D0C) = evtGetValue(event, args[1]);
    *(s32*)((s32)battleWork + 0x1D10) = evtGetValue(event, args[2]);
    *(s32*)((s32)battleWork + 0x1D14) = evtGetValue(event, args[3]);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AcGetOutputParam) {
    BattleWork* battleWork;
    s32* args;
    s32 index;
    s32 dst;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    index = evtGetValue(event, args[0]);
    dst = args[1];
    value = *(s32*)((s32)battleWork + 0x1CE8 + index * 4);

    if (dst != -250000000) {
        evtSetValue(event, dst, value);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AcSetOutputParam) {
    s32* args;
    BattleWork* battleWork;
    s32 index;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    index = evtGetValue(event, args[0]);
    value = evtGetValue(event, args[1]);

    *(s32*)((s32)battleWork + 0x1CE8 + index * 4) = value;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetupAC) {
    BattleWork* battleWork;
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 param;
    s32 rawArg;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    param = evtGetValue(event, args[1]);
    rawArg = args[2];
    value = evtGetValue(event, args[3]);

    BattleActionCommandSetup(battleWork, param, unit, rawArg, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StartAC) {
    s32* args;
    BattleWork* battleWork;
    s32 help;

    args = event->args;
    battleWork = _battleWorkPointer;

    help = evtGetValue(event, args[0]);

    BattleActionCommandStart(battleWork);

    if (help != 0) {
        if ((*(u32*)(*(s32*)((s32)battleWork + 0x1C90) + 0x27C) & 0x10) == 0) {
            BattleAcHelpSetDispType(2);
        }
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_StopAC) {
    BattleActionCommandStop(_battleWorkPointer);
    BattleAcHelpSetDispType(0);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ResultAC) {
    BattleWork* battleWork;

    battleWork = _battleWorkPointer;

    BattleActionCommandResult(battleWork);

    if ((*(u32*)((s32)battleWork + 0x1CB8) & 1) != 0) {
        return 0;
    }

    BattleAcHelpSetDispType(0);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_ResultACDefence) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    value = evtGetValue(event, args[1]);

    BattleActionCommandCheckDefence(unit, value);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetResultAC) {
    s32* args = event->args;

    evtSetValue(event, args[0], *(s32*)((s32)_battleWorkPointer + 0x1CB8));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultPrizeLv) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 value;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;
    unit = 0;

    type = evtGetValue(event, args[0]);
    if (type != -1 && type != -5) {
        unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    }

    value = evtGetValue(event, args[1]);
    dst = args[2];

    evtSetValue(event, dst, BattleActionCommandGetPrizeLv(battleWork, unit, value));

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetResultACDefence) {
    s32 dst;

    dst = event->args[0];

    evtSetValue(event, dst, BattleActionCommandGetDefenceResult());

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetResultCountAC) {
    s32* args = event->args;

    evtSetValue(event, args[0], *(s32*)((s32)_battleWorkPointer + 0x1CB4));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AudienceDeclareACResult) {
    s32* args;
    BattleWork* battleWork;
    s32 result;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;

    result = evtGetValue(event, args[0]);
    value = evtGetValue(event, args[1]);

    if (value != -1) {
        if (*(u8*)((s32)battleWork + 0x16F58) < 200) {
            *(u8*)((s32)battleWork + 0x16F58) += 1;
        }
        *(u8*)((s32)battleWork + 0x19074) = 1;
    } else {
        if (*(u8*)((s32)battleWork + 0x16F59) < 200) {
            *(u8*)((s32)battleWork + 0x16F59) += 1;
        }
        *(u8*)((s32)battleWork + 0x19074) = 0;
    }

    BattleActionCommandDeclareACResult(battleWork, result);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AudienceDeclareAcrobatResult) {
    extern void BattleAudience_Case_AcrobatGood(void);
    extern void BattleAudience_Case_AcrobatBad(void);
    extern void* effNiceEntry(s32 type, f32 x, f32 y, f32 z);
    extern void psndSFXOn_3D(char* name, void* pos);
    extern char str_SFX_BTL_ACROBAT_MISS_802ee6bc[];
    extern f32 float_150_804222b4;
    extern f32 float_0p75_80422290;
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    u8* result;
    s32 success;
    f32 x;
    f32 y;
    f32 z;
    f32 addX;
    f32 addY;
    f32 addZ;
    void* effect;

    args = event->args;
    result = (u8*)evtGetValue(event, args[0]);
    success = evtGetValue(event, args[1]);
    battleWork = _battleWorkPointer;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)event + 0x160));

    addX = evtGetFloat(event, args[2]);
    addY = evtGetFloat(event, args[3]);
    addZ = evtGetFloat(event, args[4]);

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (success != 0) {
        *(u8*)((s32)battleWork + 0x19004) = result[0x18];
        *(u8*)((s32)battleWork + 0x19007) = result[0x1B];
        BattleAudience_Case_AcrobatGood();

        y += (f32)BtlUnit_GetHeight(unit);
        x += (f32)(BtlUnit_GetWidth(unit) / 2);
        if (x > float_150_804222b4) {
            x = float_150_804222b4;
        }

        effect = effNiceEntry(7, x + addX, y + addY, z + addZ);
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = float_0p75_80422290;
    } else {
        BattleAudience_Case_AcrobatBad();
        psndSFXOn_3D(str_SFX_BTL_ACROBAT_MISS_802ee6bc, &x);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ACSuccessEffect) {
    extern void* gp;
    extern f32 unk_8034cb48[];
    extern void* effNiceEntry(f64 x, f64 y, f64 z, s32 kind);
    extern u32 psndSFXOn_3D(char* name, void* pos);
    extern void psndSFX_pit(u32 sfx, s32 pitch);
    extern char str_SFX_SYSTEM_NICE_25PE_802ee618[];
    extern char str_SFX_SYSTEM_NICE_50PE_802ee634[];
    extern char str_SFX_SYSTEM_NICE1_802ee650[];
    extern char str_SFX_SYSTEM_GOOD1_802ee664[];
    extern char str_SFX_SYSTEM_GREAT1_802ee678[];
    extern char str_SFX_SYSTEM_WONDERFUL_802ee68c[];
    extern char str_SFX_SYSTEM_EXCELLENT_802ee6a4[];
    extern f32 float_0p75_80422290;
    extern f32 float_0p3_80422294;
    extern f32 float_0p6_80422298;
    extern f32 float_1p2_8042229c;
    extern f32 float_0p5_804222a0;
    extern f32 float_160_804222a4;
    extern f32 float_neg180_804222a8;
    extern f32 float_50_804222ac;
    extern f32 float_80_804222b0;
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 lang = *(s32*)((s32)gp + 0x16C);
    f32 pos[3];
    f32 scale = float_0p75_80422290;
    f32 halfWidth;
    s32 kind;
    char* sfx;
    void* eff;
    u32 sound;

    if (lang < 0 || lang > 6) {
        lang = 0;
    }

    pos[0] = (f32)evtGetValue(event, args[1]);
    pos[1] = (f32)evtGetValue(event, args[2]);
    pos[2] = (f32)evtGetValue(event, args[3]);

    switch (type) {
        case 0:
            sfx = str_SFX_SYSTEM_NICE_25PE_802ee618;
            scale *= float_0p3_80422294;
            kind = 0;
            halfWidth = unk_8034cb48[lang * 8 + 0];
            break;
        case 1:
            sfx = str_SFX_SYSTEM_NICE_50PE_802ee634;
            scale *= float_0p6_80422298;
            kind = 0;
            halfWidth = unk_8034cb48[lang * 8 + 1];
            break;
        case 2:
            sfx = str_SFX_SYSTEM_NICE1_802ee650;
            scale *= float_1p2_8042229c;
            kind = 0;
            halfWidth = unk_8034cb48[lang * 8 + 2];
            break;
        case 3:
            sfx = str_SFX_SYSTEM_GOOD1_802ee664;
            scale *= float_1p2_8042229c;
            kind = 1;
            halfWidth = unk_8034cb48[lang * 8 + 3];
            break;
        case 4:
            sfx = str_SFX_SYSTEM_GREAT1_802ee678;
            scale *= float_1p2_8042229c;
            kind = 2;
            halfWidth = unk_8034cb48[lang * 8 + 4];
            break;
        case 5:
            sfx = str_SFX_SYSTEM_WONDERFUL_802ee68c;
            kind = 3;
            halfWidth = unk_8034cb48[lang * 8 + 5];
            break;
        case 6:
            sfx = str_SFX_SYSTEM_EXCELLENT_802ee6a4;
            kind = 4;
            halfWidth = unk_8034cb48[lang * 8 + 6];
            break;
        default:
            return EVT_RETURN_DONE;
    }

    halfWidth *= scale * float_0p5_804222a0;
    if (float_160_804222a4 < pos[0] + halfWidth) {
        pos[0] = float_160_804222a4 - halfWidth;
    }
    if (pos[0] - halfWidth < float_neg180_804222a8) {
        pos[0] = float_neg180_804222a8 + halfWidth;
    }
    if (float_80_804222b0 <= pos[1]) {
        eff = effNiceEntry(pos[0] - halfWidth, pos[1] - float_50_804222ac, pos[2], kind);
    } else {
        eff = effNiceEntry(pos[0] - halfWidth, pos[1] + float_50_804222ac, pos[2], kind);
    }
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) = scale;
    sound = psndSFXOn_3D(sfx, pos);
    psndSFX_pit(sound, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ac_timing_flag_onoff) {
    s32* args;
    BattleWork* battleWork;
    s32 state;
    u32 mask;

    args = event->args;

    state = evtGetValue(event, args[0]);
    mask = args[1];
    battleWork = _battleWorkPointer;

    if (state != 0) {
        *(u32*)((s32)battleWork + 0x1F50) |= mask;
    } else {
        *(u32*)((s32)battleWork + 0x1F50) &= ~mask;
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ac_timing_get_success_frame) {
    s32* args;
    BattleWork* battleWork;
    s32 dst;
    s32 value;

    args = event->args;
    battleWork = _battleWorkPointer;
    dst = args[0];

    value = battleAcTimingA_GetSuccessFrame(battleWork);
    evtSetValue(event, dst, value);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ACHelpSet) {
    s32* args = event->args;
    s32 value;

    value = evtGetValue(event, args[0]);

    BattleAcHelpSetHelp(*(s32*)(value + 0x70));
    BattleAcHelpSetDispType(2);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ACRStart) {
    s32* args;
    BattleWork* battleWork;
    s32 type;
    s32 id;
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    args = event->args;
    battleWork = _battleWorkPointer;

    if (isFirstCall != 0) {
        type = evtGetValue(event, args[0]);
        id = BattleTransID(event, type);
        a = evtGetValue(event, args[1]);
        b = evtGetValue(event, args[2]);
        c = evtGetValue(event, args[3]);
        d = evtGetValue(event, args[4]);

        BattleAcrobatStart(battleWork, id, a, b, c, d);
    }

    return BattleAcrobatMain(battleWork);
}



USER_FUNC(btlevtcmd_ACRGetResult) {
    s32* args;
    BattleWork* battleWork;
    s32 dst1;
    s32 dst2;
    s32 result1;
    s32 result2;

    args = event->args;
    battleWork = _battleWorkPointer;
    dst1 = args[0];
    dst2 = args[1];

    BattleAcrobatGetResult(battleWork, &result1, &result2);

    if (dst1 != -250000000) {
        evtSetValue(event, dst1, result1);
    }
    if (dst2 != -250000000) {
        evtSetValue(event, dst2, result2);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AudienceAppeal) {
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;

    battleWork = _battleWorkPointer;

    type = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    if (unit != 0) {
        BattleAudience_Case_Appeal(unit);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AnimeChangeGroup) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    s32 anim;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    anim = evtGetValue(event, args[2]);

    if (isFirstCall != 0) {
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }

        animEffectAsync(anim, 1);
    }

    if (animEffectAsync(anim, 1) == 0) {
        return 0;
    }

    *(s32*)((s32)part + 0x1C0) = animPoseEntry(anim, 1);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePose) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    s32 anim;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    anim = evtGetValue(event, args[2]);
    BtlUnit_SetAnim(part, anim);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseType) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    s32 poseType;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    poseType = evtGetValue(event, args[2]);
    BtlUnit_GetPoseNameFromType(part, poseType);
    BtlUnit_SetAnimType(part, poseType);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseDirect) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;
    char* anim;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    unit = BattleGetUnitPtr(battleWork, id);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    anim = (char*)evtGetValue(event, args[2]);

    strcpy((char*)((s32)part + 0x1C4), anim);

    *(u32*)((s32)part + 0x204) &= ~0x80;
    *(u32*)((s32)part + 0x204) &= ~0x10;

    animPoseSetAnim(*(s32*)((s32)part + 0x1C0), (s32)anim, 1);

    *(u8*)((s32)part + 0x215) = 1;

    if (BtlUnit_CheckStatus(unit, 0x12) != 0) {
        *(u8*)((s32)part + 0x215) = 7;
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AnimeWaitPlayComplete) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 1;
    }

    if (*(s32*)((s32)event + 0x78) > 0) {
        *(s32*)((s32)event + 0x78) = *(s32*)((s32)event + 0x78) - 1;
        return 0;
    }

    if (animPoseGetLoopTimes(*(s32*)((s32)part + 0x1C0)) < 1.0f) {
        return 0;
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AnimeWaitPlayLoopTiming) {
    s32* args;
    BattleWorkUnitPart* part;
    void* pose;
    s32 type;
    s32 id;
    s32 partId;
    f32 speed;
    f32 loops;
    s32 loopNow;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 1;
        *(s32*)((s32)event + 0x7C) = 0;
        *(s32*)((s32)event + 0x80) = 0;
    }

    pose = animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
    speed = *(f32*)((s32)pose + 0x44);

    if (speed < 0.0f) {
        speed = -speed;
    }

    if ((f64)speed < 0.01) {
        return EVT_RETURN_DONE;
    }

    loops = animPoseGetLoopTimes(*(s32*)((s32)part + 0x1C0));

    if (*(s32*)((s32)event + 0x78) > 0) {
        *(s32*)((s32)event + 0x78) = *(s32*)((s32)event + 0x78) - 1;
        return 0;
    }

    if (*(s32*)((s32)event + 0x7C) == 0) {
        *(s32*)((s32)event + 0x80) = (s32)loops;
        *(s32*)((s32)event + 0x7C) = *(s32*)((s32)event + 0x7C) + 1;
        return 0;
    }

    loopNow = (s32)loops;
    if (loopNow - *(s32*)((s32)event + 0x80) != 0) {
        return EVT_RETURN_DONE;
    }

    return 0;
}


USER_FUNC(btlevtcmd_SetTalkBodyId) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 pose;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    pose = evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    *(s32*)((s32)unit + 0x2D4) = pose;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetTalkPose) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 pose;

    args = event->args;

    type = evtGetValue(event, args[0]);
    pose = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    *(s32*)((s32)unit + 0x2D8) = pose;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetStayPose) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 pose;

    args = event->args;

    type = evtGetValue(event, args[0]);
    pose = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    *(s32*)((s32)unit + 0x2DC) = pose;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetTalkPoseType) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 poseType;
    s32 bodyId;
    char* poseName;

    args = event->args;

    type = evtGetValue(event, args[0]);
    poseType = evtGetValue(event, args[1]);

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    bodyId = BtlUnit_GetBodyPartsId(unit);
    part = BattleGetUnitPartsPtr(id, bodyId);

    poseName = BtlUnit_GetPoseNameFromType(part, poseType);
    *(s32*)((s32)unit + 0x2D8) = (s32)poseName;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_SetStayPoseType) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 poseType;
    s32 bodyId;
    char* poseName;

    args = event->args;

    type = evtGetValue(event, args[0]);
    poseType = evtGetValue(event, args[1]);

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    bodyId = BtlUnit_GetBodyPartsId(unit);
    part = BattleGetUnitPartsPtr(id, bodyId);

    poseName = BtlUnit_GetPoseNameFromType(part, poseType);
    *(s32*)((s32)unit + 0x2DC) = (s32)poseName;

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AnimeCmpPoseType) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    s32 type1;
    BattleWorkUnitPart* part;
    s32 type2;
    s32 dst;
    char* pose1;
    char* pose2;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    type1 = evtGetValue(event, args[2]);
    type2 = evtGetValue(event, args[3]);
    dst = args[4];

    pose1 = BtlUnit_GetPoseNameFromType(part, type1);
    pose2 = BtlUnit_GetPoseNameFromType(part, type2);

    evtSetValue(event, dst, strcmp(pose1, pose2) != 0);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    btlDispPoseAnime(part);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeSetPoseTable) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    *(s32*)((s32)part + 0x1BC) = args[2];
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeSetPoseSoundTable) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    *(s32*)((s32)part + 0x4D0) = args[2];
    BtlUnit_PoseSoundInit(part);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckDefinePosetype) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 poseType;
    s32 dst;
    s32 result;
    s32* table;
    s32 value;
    s32 upper;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partsId);

    poseType = evtGetValue(event, args[2]);
    dst = args[3];
    table = *(s32**)((s32)part + 0x1BC);
    result = 0;

    if (poseType <= 0x1C) {
        upper = 0x1C;
    } else if (poseType < 0x28) {
        upper = 0x27;
    } else if (poseType < 0x46) {
        upper = 0x45;
    } else {
        evtSetValue(event, dst, 0);
        return EVT_RETURN_DONE;
    }

    while (table != 0) {
        value = table[0];

        if (value >= 0x46) {
            result = 0;
            break;
        }
        if (value == poseType) {
            result = 1;
            break;
        }
        if (value == upper) {
            result = 0;
            break;
        }

        table += 2;
    }

    evtSetValue(event, dst, result);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AnimeSetMotionSpeed) {
    s32 type;
    s32 id;
    s32* args;
    s32 partId;
    BattleWorkUnitPart* part;
    f32 speed;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    speed = evtGetFloat(event, args[2]);
    *(f32*)((s32)part + 0x20C) = speed;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AnimeChangePaperGroup) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;
    s32 group;
    s32 value;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    group = evtGetValue(event, args[2]);
    value = evtGetValue(event, args[3]);

    animPoseSetEffect(*(s32*)((s32)part + 0x1C0), group, value);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_AnimeChangePaperAnime) {
    BattleWorkUnitPart* part;
    s32* args;
    s32 type;
    s32 id;
    s32 partId;
    s32 anime;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    anime = evtGetValue(event, args[2]);
    btlDispChangePeraAnime(part, anime);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_AnimeFlagOnOff) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partId;
    u32 mask;
    u32 state;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);

    mask = args[2];
    state = evtGetValue(event, args[3]);

    if (state != 0) {
        *(u32*)((s32)part + 0x204) |= mask;
    } else {
        *(u32*)((s32)part + 0x204) &= ~mask;
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckGuardMotionEnable) {
    s32* args;
    s32 type;
    s32 id;
    s32 dst;
    s32 result;
    BattleWorkUnit* unit;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    result = 1;

    if (BtlUnit_CheckStatus(unit, 1) != 0 ||
        BtlUnit_CheckStatus(unit, 2) != 0 ||
        BtlUnit_CheckStatus(unit, 9) != 0 ||
        BtlUnit_CheckStatus(unit, 0xB) != 0 ||
        BtlUnit_CheckStatus(unit, 0x1B) != 0 ||
        BtlUnit_CheckStatus(unit, 0x12) != 0) {
        result = 0;
    }

    if (BtlUnit_CheckStatusFlag(unit, 1) != 0) {
        result = 0;
    }

    evtSetValue(event, dst, result);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckStatus) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s8 status;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    status = args[1];
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (BtlUnit_CheckStatus(unit, status) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnOffStatus) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s8 status;
    s32 turns;
    s32 strength;
    s32 on;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    status = args[1];
    turns = evtGetValue(event, args[2]);
    strength = evtGetValue(event, args[3]);
    on = evtGetValue(event, args[4]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (on != 0) {
        BtlUnit_SetStatus(unit, status, (s8)turns, (s8)strength);
    } else {
        BtlUnit_SetStatus(unit, status, 0, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_ClearAllStatus) {
    BattleWorkUnit* unit;
    s32* args = event->args;
    s32 type;

    type = evtGetValue(event, *args++);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));
    BtlUnit_ClearStatus(unit);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OnStatusFlag) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    mask = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OnStatusFlag(unit, mask);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffStatusFlag) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    mask = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OffStatusFlag(unit, mask);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckActStatus) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    if (BtlUnit_CanActStatus(unit) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetACPossibility) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, type));

    if (BtlUnit_GetACPossibility(unit) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_CheckUnitFlag) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 checkArg;
    s32 dst;
    s32 result;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    checkArg = args[1];
    dst = args[2];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    result = BtlUnit_CheckUnitFlag(unit, checkArg);

    if (result != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_OnUnitFlag) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    mask = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OnUnitFlag(unit, mask);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_OffUnitFlag) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    u32 mask;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    mask = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_OffUnitFlag(unit, mask);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_AfterReactionEntry) {
    s32* args;
    s32 type;
    s32 id;
    s32 arg;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    arg = evtGetValue(event, args[1]);

    BattleAfterReactionEntry(id, arg);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CheckWeaponPluralTarget) {
    s32* args;
    void* weapon;
    s32 dst;

    args = event->args;

    weapon = (void*)evtGetValue(event, args[0]);
    dst = args[1];

    if ((*(u32*)((s32)weapon + 0x64) & 0x01000000) != 0) {
        evtSetValue(event, dst, 0);
    } else {
        evtSetValue(event, dst, 1);
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PayWeaponCost) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    void* weapon;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    weapon = (void*)evtGetValue(event, args[1]);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_PayWeaponCost(unit, weapon);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandPayWeaponCost) {
    s32 type;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    void* weapon;

    battleWork = _battleWorkPointer;

    type = evtGetValue(event, event->args[0]);

    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    weapon = BattleGetSelectWeapon(battleWork);

    BtlUnit_PayWeaponCost(unit, weapon);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetWeaponActionLv) {
    s32* args;
    void* weapon;

    args = event->args;

    weapon = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(u8*)((s32)weapon + 0x6E));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandGetWeaponActionLv) {
    s32* args;
    BattleWork* battleWork;
    s32 dst;
    void* weapon;

    args = event->args;
    battleWork = _battleWorkPointer;
    dst = args[0];

    weapon = BattleGetSelectWeapon(battleWork);
    evtSetValue(event, dst, *(u8*)((s32)weapon + 0x6E));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_CommandGetWeaponAddress) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 dst;
    s32 kind;
    s32 item;
    void* weapon;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    dst = args[1];

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(battleWork, id);
    kind = *(s32*)((s32)unit + 0x8);

    if (kind == 0xDE || (kind >= 0xE0 && kind < 0xE7)) {
        weapon = BattleGetSelectWeapon(battleWork);
    } else {
        item = *(s32*)((s32)unit + 0x308);
        weapon = *(void**)((s32)itemDataTable + item * 0x28 + 0x24);
    }

    evtSetValue(event, dst, (s32)weapon);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_CommandGetWeaponItemId) {
    s32* args;
    s32 dst;
    void* weapon;

    args = event->args;

    dst = args[0];
    weapon = (void*)evtGetValue(event, args[1]);

    evtSetValue(event, dst, *(s32*)((s32)weapon + 8));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_GetItemRecoverParam) {
    s32* args;
    s32 item;
    s32 dstHp;
    s32 dstFp;
    s32 dstSp;
    u8* data;

    args = event->args;

    item = evtGetValue(event, args[0]);
    dstHp = args[1];
    dstFp = args[2];
    dstSp = args[3];

    data = itemDataTable + item * 0x28;

    if (dstHp != -250000000) {
        evtSetValue(event, dstHp, data[0x1D]);
    }
    if (dstFp != -250000000) {
        evtSetValue(event, dstFp, data[0x1E]);
    }
    if (dstSp != -250000000) {
        evtSetValue(event, dstSp, data[0x1F]);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_WeaponAftereffect) {
    extern void BattleStage_DestroyA1(void);
    extern void BattleStage_DestroyA2(void);
    extern void BattleStage_DestroyB(void);
    extern void BattleStage_NozzleDirChangeCheck(void);
    extern void BattleStage_NozzleWorkCheck(s32 force);
    extern void BattleStage_IronFrameFallCheck(void);
    extern void BattleStage_ObjectFallCheck(void);
    s32* args;
    BattleWork* battleWork;
    u8* weapon;
    s32 destroyB;
    s32 destroyA2;
    s32 destroyA1;
    s32 value;
    u8 stageFlags;

    args = event->args;
    battleWork = _battleWorkPointer;
    weapon = (u8*)evtGetValue(event, args[0]);

    destroyB = 0;
    destroyA2 = 0;
    destroyA1 = 0;
    value = irand(weapon[0xB4] + weapon[0xB5] + weapon[0xB6] + weapon[0xB7]);

    if (*(u32*)((s32)battleWork + 0x19078) & 0x10) {
        destroyA1 = 1;
    }
    if (*(u32*)((s32)battleWork + 0x19078) & 0x20) {
        destroyA2 = 1;
    }

    value -= weapon[0xB4];
    if (value < 0) {
        destroyA2 = 1;
        destroyA1 = 1;
    } else {
        value -= weapon[0xB5];
        if (value < 0) {
            destroyA1 = 1;
        } else {
            value -= weapon[0xB6];
            if (value < 0) {
                destroyA2 = 1;
            }
        }
    }

    stageFlags = *(u8*)((s32)battleWork + 0x180F8);
    if (stageFlags & 4) {
        destroyA2 = 0;
    }
    if (stageFlags & 2) {
        destroyA1 = 0;
    }

    if ((stageFlags & 2) != 0) {
        if ((stageFlags & 8) == 0) {
            if (irand(100) < weapon[0xB8] || (*(u32*)((s32)battleWork + 0x19078) & 0x40)) {
                destroyB = 1;
                if ((*(u8*)((s32)battleWork + 0x180F8) & 4) == 0) {
                    destroyA2 = 1;
                }
            }
        }
    }

    if (destroyA1 != 0) {
        BattleStage_DestroyA1();
    }
    if (destroyA2 != 0) {
        BattleStage_DestroyA2();
    }
    if (destroyB != 0) {
        BattleStage_DestroyB();
    }

    if (irand(100) < weapon[0xB9] || (*(u32*)((s32)battleWork + 0x19078) & 0x200)) {
        BattleStage_NozzleDirChangeCheck();
    }
    if (irand(100) < weapon[0xBA] || (*(u32*)((s32)battleWork + 0x19078) & 0x100)) {
        BattleStage_NozzleWorkCheck(1);
    }
    if (irand(100) < weapon[0xBB] || (*(u32*)((s32)battleWork + 0x19078) & 0x80)) {
        BattleStage_IronFrameFallCheck();
    }

    if (*(u8*)((s32)battleWork + 0x180F8) & 0x20) {
        if (*(u32*)((s32)battleWork + 0x19078) & 1) {
            BattleStage_ObjectFallCheck();
        } else if (irand(100) < weapon[0xBC]) {
            BattleStage_ObjectFallCheck();
        }
    }

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetGuard) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 guardType;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    guardType = evtGetValue(event, args[1]);
    *(s32*)((s32)unit + 0x140) = BattleTransID(event, guardType);

    return EVT_RETURN_DONE;
}



USER_FUNC(btlevtcmd_GetGuardKouraId) {
    s32* args;
    BattleWorkUnit* unit;
    BattleWorkUnit* guard;
    s32 type;
    s32 id;
    s32 dst;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    dst = args[1];

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    guard = BtlUnit_GetGuardKouraPtr(unit);

    if (guard != 0) {
        evtSetValue(event, dst, *(s32*)guard);
    } else {
        evtSetValue(event, dst, -1);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_InterruptStop) {
    s32 state;
    BattleWork* battleWork;

    state = evtGetValue(event, event->args[0]);
    battleWork = _battleWorkPointer;

    if (isFirstCall != 0) {
        switch (state) {
            case 0:
                *(u32*)((s32)battleWork + 0xEF4) |= 0x10;
                break;
            case 1:
                *(u32*)((s32)battleWork + 0xEF4) |= 0x20;
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
    s32* args;
    void* stage;
    s32 dstX;
    s32 dstY;
    s32 dstZ;

    args = event->args;
    stage = BattleStageGetPtr();

    dstX = args[0];
    dstY = args[1];
    dstZ = args[2];

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)*(f32*)((s32)stage + 0xB24));
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)*(f32*)((s32)stage + 0xB28));
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)*(f32*)((s32)stage + 0xB2C));
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_GetStageAudienceAreaSize) {
    s32* args;
    void* stage;
    s32 dstX;
    s32 dstY;
    s32 dstZ;

    args = event->args;
    stage = BattleStageGetPtr();

    dstX = args[0];
    dstY = args[1];
    dstZ = args[2];

    if (dstX != -250000000) {
        evtSetValue(event, dstX, (s32)*(f32*)((s32)stage + 0xB30));
    }
    if (dstY != -250000000) {
        evtSetValue(event, dstY, (s32)*(f32*)((s32)stage + 0xB34));
    }
    if (dstZ != -250000000) {
        evtSetValue(event, dstZ, (s32)*(f32*)((s32)stage + 0xB38));
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_TransStageFloorPosition) {
    s32* args;
    f32 x;
    s32 dstY;
    f32 z;
    f32 height;

    args = event->args;

    x = (f32)evtGetValue(event, args[0]);
    dstY = args[1];
    z = (f32)evtGetValue(event, args[2]);

    height = BattleGetFloorHeight(_battleWorkPointer, x, 0.0f, z);
    evtSetValue(event, dstY, (s32)height);

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_SetMoveSoundDataWork) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s16 f;
    s16 g;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);
    f = (s16)evtGetValue(event, args[6]);
    g = (s16)evtGetValue(event, args[7]);

    BtlUnit_SetSeMode((void*)((s32)unit + 0x18C), a, b, (s16)c, (s16)d, (s16)e, f, g);

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetRunSound) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);

    *(s32*)((s32)unit + 0x1CC) = a;
    *(s32*)((s32)unit + 0x1D0) = b;
    *(s16*)((s32)unit + 0x1D4) = c;
    *(s16*)((s32)unit + 0x1D6) = d;
    *(s16*)((s32)unit + 0x1D8) = e;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsRunSound) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partsId);

    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);

    *(s32*)((s32)part + 0x100) = a;
    *(s32*)((s32)part + 0x104) = b;
    *(s16*)((s32)part + 0x108) = c;
    *(s16*)((s32)part + 0x10A) = d;
    *(s16*)((s32)part + 0x10C) = e;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetWalkSound) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);

    *(s32*)((s32)unit + 0x1DC) = a;
    *(s32*)((s32)unit + 0x1E0) = b;
    *(s16*)((s32)unit + 0x1E4) = c;
    *(s16*)((s32)unit + 0x1E6) = d;
    *(s16*)((s32)unit + 0x1E8) = e;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsWalkSound) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partsId);

    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);

    *(s32*)((s32)part + 0x110) = a;
    *(s32*)((s32)part + 0x114) = b;
    *(s16*)((s32)part + 0x118) = c;
    *(s16*)((s32)part + 0x11A) = d;
    *(s16*)((s32)part + 0x11C) = e;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetDiveSound) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    c = evtGetValue(event, args[3]);
    d = evtGetValue(event, args[4]);
    e = evtGetValue(event, args[5]);

    *(s32*)((s32)unit + 0x1EC) = a;
    *(s32*)((s32)unit + 0x1F0) = b;
    *(s16*)((s32)unit + 0x1F4) = c;
    *(s16*)((s32)unit + 0x1F6) = d;
    *(s16*)((s32)unit + 0x1F8) = e;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsDiveSound) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partsId);

    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);
    c = evtGetValue(event, args[4]);
    d = evtGetValue(event, args[5]);
    e = evtGetValue(event, args[6]);

    *(s32*)((s32)part + 0x120) = a;
    *(s32*)((s32)part + 0x124) = b;
    *(s16*)((s32)part + 0x128) = c;
    *(s16*)((s32)part + 0x12A) = d;
    *(s16*)((s32)part + 0x12C) = e;

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_SetJumpSound) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 a;
    s32 b;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));

    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);

    *(s32*)((s32)unit + 0x1C4) = a;
    *(s32*)((s32)unit + 0x1C8) = b;

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_SetPartsJumpSound) {
    s32* args;
    BattleWorkUnitPart* part;
    s32 type;
    s32 id;
    s32 partsId;
    s32 a;
    s32 b;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);

    partsId = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partsId);

    a = evtGetValue(event, args[2]);
    b = evtGetValue(event, args[3]);

    *(s32*)((s32)part + 0xF8) = a;
    *(s32*)((s32)part + 0xFC) = b;

    return EVT_RETURN_DONE;
}


// _UnitMoveSoundControl

// _PartsMoveSoundControl

// _MoveSoundControlCore

void _MoveSoundControlEndCore(void* unit, void* work) {
    u32 sfxId;

    sfxId = *(u32*)((s32)work + 0x18);

    if (sfxId != 0xFFFFFFFF) {
        psndSFXOff(sfxId);
        *(s32*)((s32)work + 0x18) = -1;
    }
}

void _PartsMoveSoundControlEnd(void* parts) {
    _MoveSoundControlEndCore(*(void**)((s32)parts + 0x4EC), (void*)((s32)parts + 0xC0));
}

void _UnitMoveSoundControlEnd(void* unit) {
    _MoveSoundControlEndCore(unit, (void*)((s32)unit + 0x18C));
}

USER_FUNC(btlevtcmd_snd_se) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 se;
    s32 flags;
    s32 vol;
    s32 dst;
    s32 result;

    args = event->args;

    type = evtGetValue(event, args[0]);
    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    se = evtGetValue(event, args[1]);
    flags = evtGetValue(event, args[2]);
    vol = evtGetValue(event, args[3]);
    dst = args[4];

    result = BtlUnit_snd_se(unit, se, flags, (s16)vol);

    if (dst != -250000000) {
        evtSetValue(event, dst, result);
    }

    return EVT_RETURN_DONE;
}


s32 btlevtcmd_snd_se_offset(EventEntry* event, BOOL isFirstCall) {
    s32* args;
    void* battleWork;
    BattleWorkUnit* unit;
    s32 id;
    s32 se;
    s32 flags;
    s16 volume;
    s32 destination;
    s32 result;
    f32 pos[3];

    args = event->args;
    battleWork = _battleWorkPointer;
    id = BattleTransID(event, evtGetValue(event, args[0]));
    unit = BattleGetUnitPtr(battleWork, id);
    se = evtGetValue(event, args[1]);
    flags = evtGetValue(event, args[2]);
    volume = (s16)evtGetValue(event, args[3]);
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    pos[0] += evtGetFloat(event, args[4]);
    pos[1] += evtGetFloat(event, args[5]);
    pos[2] += evtGetFloat(event, args[6]);
    destination = args[7];
    result = BtlUnit_snd_se_pos(unit, se, flags, volume, (Vec*)pos);
    if (destination != -250000000) {
        evtSetValue(event, destination, result);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_snd_voice) {
    s32* args;
    BattleWorkUnit* unit;
    s32 type;
    s32 id;
    s32 voiceType;
    s32 se;

    args = event->args;

    type = evtGetValue(event, args[0]);
    voiceType = evtGetValue(event, args[1]);

    id = BattleTransID(event, type);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (voiceType == 1) {
        se = _mario_acrobat_voice_table[irand(5)];
        BtlUnit_snd_se(unit, se, -250000000, 0);
    } else if (voiceType == 2) {
        se = _mario_attack_voice_table[irand(10)];
        BtlUnit_snd_se(unit, se, -250000000, 0);
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_EnemyItemUseCheck) {
    s32* args;
    BattleWork* battleWork;
    BattleWorkUnit* unit;
    s32 type;
    s32 dst;

    args = event->args;
    battleWork = _battleWorkPointer;

    type = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, type));
    dst = args[1];

    evtSetValue(event, dst, BattleEnemyUseItemCheck(unit));

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_PadCheckTrigger) {
    s32* args;
    s32 player;
    u32 mask;
    s32 dst;

    args = event->args;

    player = evtGetValue(event, args[0]);
    mask = args[1];
    dst = args[2];

    if (BattlePadMultiCheckTrigger(player, mask) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_PadCheckNow) {
    s32* args;
    s32 player;
    u32 mask;
    s32 dst;

    args = event->args;

    player = evtGetValue(event, args[0]);
    mask = args[1];
    dst = args[2];

    if (BattlePadMultiCheckNow(player, mask) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }

    return EVT_RETURN_DONE;
}




USER_FUNC(btlevtcmd_InviteApInfoReport) {
    void* apInfo;

    apInfo = (void*)((s32)_battleWorkPointer + 0x18FFC);

    BattleAudience_ApRecoveryBuild(apInfo);
    memset(apInfo, 0, 0xC);

    return EVT_RETURN_DONE;
}


s32 btlevtcmd_SacBgsetMove(EventEntry* event, BOOL isFirstCall) {
    extern f32 intplGetValue(f32, f32, s32, s32, s32);
    extern f32 float_260_80422288;
    extern f32 float_450_80422284;
    extern f32 float_0_80422280;
    s32* args = event->args;
    void* battleWork = _battleWorkPointer;
    s32 state;
    s32 mode;
    s32 direction;
    s32 i;
    u8* object;
    f32 value;

    direction = evtGetValue(event, args[0]);
    if (isFirstCall) {
        *(s32*)((s32)event + 0x78) = 0;
    }
    state = *(s32*)((s32)event + 0x78) + 1;
    *(s32*)((s32)event + 0x78) = state;
    mode = direction == 0 ? 2 : 5;

    for (i = 0, object = (u8*)battleWork + 0x1715C; i < 32; i++, object += 0x7C) {
        if (*(s32*)object <= 0) {
            continue;
        }
        if (*(u16*)(*(s32*)(object + 0x64) + 6) == 2) {
            if (direction == 0) {
                value = intplGetValue(float_0_80422280, float_260_80422288, mode, state, 45);
            } else {
                value = intplGetValue(float_260_80422288, float_0_80422280, mode, state, 45);
            }
            *(f32*)(object + 0x2C) = value;
        } else if (*(u16*)(*(s32*)(object + 0x64) + 6) < 2) {
            if (direction == 0) {
                value = intplGetValue(float_0_80422280, float_450_80422284, mode, state, 45);
            } else {
                value = intplGetValue(float_450_80422284, float_0_80422280, mode, state, 45);
            }
            if (*(f32*)(object + 4) < float_0_80422280) {
                value = -value;
            }
            *(f32*)(object + 0x28) = value;
        }
    }
    return state > 44;
}

USER_FUNC(btlevtcmd_BtlStageObjFlagOnOff) {
    s32* args;
    BattleWork* battleWork;
    char* name;
    u32 mask;
    u32 invMask;
    s32 state;
    s32 i;
    s32 offset;
    void* obj;

    args = event->args;
    battleWork = _battleWorkPointer;

    name = (char*)evtGetValue(event, args[0]);
    mask = args[1];
    state = evtGetValue(event, args[2]);
    invMask = ~mask;

    offset = 0;
    for (i = 0; i < 0x20; i++, offset += 0x7C) {
        obj = (void*)((s32)battleWork + 0x1715C + offset);
        if (*(s32*)obj > 0 && strcmp(**(char***)((s32)obj + 0x64), name) == 0) {
            if (state != 0) {
                *(u32*)((s32)obj + 0x68) |= mask;
            } else {
                *(u32*)((s32)obj + 0x68) &= invMask;
            }
        }
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_BtlStageObjSetRotate) {
    s32* args;
    BattleWork* battleWork;
    char* name;
    s32 xArg;
    s32 yArg;
    s32 zArg;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 offset;
    void* obj;

    args = event->args;
    battleWork = _battleWorkPointer;

    name = (char*)evtGetValue(event, args[0]);

    xArg = args[1];
    x = evtGetValue(event, xArg);
    yArg = args[2];
    y = evtGetValue(event, yArg);
    zArg = args[3];
    z = evtGetValue(event, zArg);

    offset = 0;
    for (i = 0; i < 0x20; i++, offset += 0x7C) {
        obj = (void*)((s32)battleWork + 0x1715C + offset);
        if (*(s32*)obj > 0 && strcmp(**(char***)((s32)obj + 0x64), name) == 0) {
            if (xArg != -250000000) {
                *(f32*)((s32)obj + 0x1C) = (f32)x;
            }
            if (yArg != -250000000) {
                *(f32*)((s32)obj + 0x20) = (f32)y;
            }
            if (zArg != -250000000) {
                *(f32*)((s32)obj + 0x24) = (f32)z;
            }
            break;
        }
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_BtlStageObjGetRotate) {
    s32* args;
    BattleWork* battleWork;
    char* name;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    s32 i;
    s32 offset;
    void* obj;

    args = event->args;
    battleWork = _battleWorkPointer;

    name = (char*)evtGetValue(event, args[0]);
    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];

    offset = 0;
    for (i = 0; i < 0x20; i++, offset += 0x7C) {
        obj = (void*)((s32)battleWork + 0x1715C + offset);
        if (*(s32*)obj > 0 && strcmp(**(char***)((s32)obj + 0x64), name) == 0) {
            if (dstX != -250000000) {
                evtSetFloat(event, dstX, *(f32*)((s32)obj + 0x1C));
            }
            if (dstY != -250000000) {
                evtSetFloat(event, dstY, *(f32*)((s32)obj + 0x20));
            }
            if (dstZ != -250000000) {
                evtSetFloat(event, dstZ, *(f32*)((s32)obj + 0x24));
            }
            break;
        }
    }

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_StatusWindowOnOff) {
    s32 state;
    BattleWork* battleWork;

    battleWork = _battleWorkPointer;
    state = evtGetValue(event, event->args[0]);

    switch (state) {
        case 2:
            *(s32*)((s32)battleWork + 0x2744) = 1;
            break;
        case 1:
            *(s32*)((s32)battleWork + 0x2748) = 1;
            break;
        case 0:
            *(s32*)((s32)battleWork + 0x2748) = 0;
            break;
    }

    BattleStatusWindowCheck();

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_StageDispellFog) {
    BattleFogForceStop();

    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_ftomsec) {
    s32* args;
    s32 frames;
    s32 dst;

    args = event->args;

    frames = evtGetValue(event, args[0]);
    dst = args[1];

    evtSetValue(event, dst, frames * 1000 / 60);

    return EVT_RETURN_DONE;
}


USER_FUNC(btlevtcmd_ftof) {
    s32* args;
    s32 frames;
    s32 dst;
    f32 value;

    args = event->args;

    frames = evtGetValue(event, args[0]);
    dst = args[1];

    if (*(s32*)((s32)gp + 4) == 60) {
        evtSetValue(event, dst, frames);
        return EVT_RETURN_DONE;
    }

    value = sysMsec2FrameFloat((f32)(frames * 1000 / 60));
    evtSetValue(event, dst, (s32)value);

    return EVT_RETURN_DONE;
}


void _PartsMoveSoundControl(void* part) {
    extern void _MoveSoundControlCore(void* unit, void* moveSoundWork, void* part, f32 z, f32 y, f32 x);
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPartsPos(part, &x, &y, &z);
    _MoveSoundControlCore(*(void**)((s32)part + 0x4EC), (void*)((s32)part + 0xC0), part, z, y, x);
}

void _UnitMoveSoundControl(void* unit) {
    extern void _MoveSoundControlCore();
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    _MoveSoundControlCore(x, y, z, unit, (void*)((s32)unit + 0x18C), NULL);
}

void _MoveSoundControlCore(double x, double y, double z, void* unit, void* moveSoundWork, void* part) {
    extern void BtlUnit_GetPartsWorldPos(void*, f32*, f32*, f32*);
    extern void psndSFX_pos(s32, Vec*);
    extern Vec vec3_802ee494;
    Vec soundPos;
    Vec worldPos;
    char* sound;

    soundPos = vec3_802ee494;
    soundPos.x = (f32)x;
    soundPos.y = (f32)y;
    soundPos.z = (f32)z;
    sound = *(char**)((s32)moveSoundWork + 0x0);

    if (sound != NULL || *(char**)((s32)moveSoundWork + 0x4) != NULL) {
        if (sound != NULL && *(char**)((s32)moveSoundWork + 0x4) == NULL) {
            if (*(s32*)((s32)moveSoundWork + 0x18) == -1) {
                *(s32*)((s32)moveSoundWork + 0x18) =
                    BtlUnit_snd_se_pos(unit, (s32)sound, -250000000, 0, &soundPos);
            }
            if (part != NULL) {
                BtlUnit_GetPartsWorldPos(part, &worldPos.x, &worldPos.y, &worldPos.z);
            } else {
                BtlUnit_GetPos(unit, &worldPos.x, &worldPos.y, &worldPos.z);
            }
            psndSFX_pos(*(s32*)((s32)moveSoundWork + 0x18), &worldPos);
        } else {
            if (*(s16*)((s32)moveSoundWork + 0x12) > -1) {
                *(s16*)((s32)moveSoundWork + 0x12) -= 1;
                if (*(s16*)((s32)moveSoundWork + 0x12) < 0) {
                    *(s16*)((s32)moveSoundWork + 0x14) += 1;
                    if ((*(s16*)((s32)moveSoundWork + 0x14) & 1) == 0) {
                        sound = *(char**)((s32)moveSoundWork + 0x4);
                        *(s16*)((s32)moveSoundWork + 0x12) = *(s16*)((s32)moveSoundWork + 0x10);
                    } else {
                        sound = *(char**)((s32)moveSoundWork + 0x0);
                        *(s16*)((s32)moveSoundWork + 0x12) = *(s16*)((s32)moveSoundWork + 0xE);
                    }
                    if (sound != NULL) {
                        BtlUnit_snd_se_pos(unit, (s32)sound, -250000000, 0, &soundPos);
                    }
                }
            }
        }
    }
}

