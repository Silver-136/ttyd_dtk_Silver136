#include "battle/battle_mario.h"
extern void* _battleWorkPointer;

extern s32 evtGetValue(void* evt, s32 value);
extern void evtSetValue(void* evt, s32 var, s32 value);

extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
extern void animPoseSetMaterialLightFlagOff(void* animPose, s32 flag);

extern void* pouchGetPtr(void);

extern void BattleMajinaiDone(void);
extern void BattleMajinaiEndCheck(void);
extern void BattleAudience_Case_JumpNewRecord(void);

s32 _paper_light_off(void* evt) {
    s32* args;
    s32 unitId;
    s32 partsId;
    void* parts;

    args = *(s32**)((s32)evt + 0x18);

    unitId = evtGetValue(evt, args[0]);
    unitId = BattleTransID(evt, unitId);

    partsId = evtGetValue(evt, args[1]);

    parts = BattleGetUnitPartsPtr(unitId, partsId);
    animPoseSetMaterialLightFlagOff(*(void**)((s32)parts + 0x1C0), 1);

    return 2;
}

s32 _majinai_powerup_check(void* evt) {
    s32* args;
    void* battleWork;
    s32 dst;
    void* pouch;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    dst = args[0];

    pouch = pouchGetPtr();

    if (*(u8*)((s32)pouch + 0x5BA) != 1) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, 1);
        *(u8*)((s32)battleWork + 0x18FF9) = 1;
        BattleMajinaiDone();
        BattleMajinaiEndCheck();
    }

    return 2;
}

s32 _record_renzoku_count(void* evt) {
    s32* args;
    s32 count;
    void* battleWorkBase;
    void* pouch;

    args = *(s32**)((s32)evt + 0x18);
    count = evtGetValue(evt, args[0]);

    battleWorkBase = (void*)((s32)_battleWorkPointer + 0x10000);
    pouch = pouchGetPtr();

    if (count > *(u8*)((s32)battleWorkBase + 0x6F40)) {
        if (count > *(u16*)((s32)pouch + 0x9E)) {
            *(u16*)((s32)pouch + 0x9E) = count;
            BattleAudience_Case_JumpNewRecord();
        }

        if (count > 100) {
            count = 100;
        }

        *(u8*)((s32)battleWorkBase + 0x6F40) = count;
    }

    return 2;
}

s32 _bgset_iron_frame_check(void* evt) {
    s32* args;
    void* battleWork;
    s32 dst;
    u8 flags;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);

    flags = *(u8*)((s32)battleWork + 0x180F8);
    dst = args[0];

    if (flags & 0x20) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, 1);
    }

    return 2;
}

s32 _get_mario_hammer_lv(void* evt) {
    s32* args;
    s32 dst;
    s32 value;

    args = *(s32**)((s32)evt + 0x18);
    dst = args[0];

    value = *(s8*)((s32)pouchGetPtr() + 0x99);

    evtSetValue(evt, dst, value);

    return 2;
}