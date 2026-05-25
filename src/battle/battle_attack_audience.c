#include "battle/battle_attack_audience.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
extern void* btlataudevtDummy[];
extern u8 btlataudevtPresentItem_Damage[];
extern u8 btlataudevtPresentItem_Get[];

void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void* BtlUnit_GetData(void* unit, s32 kind);
s32 BattleAudience_GetPresentItemType(void);
s32 BattleAudience_GetPresentTargetUnitId(void);
s32 BattleAudience_GetPresentItemNo(void);
void BattleAudience_SetPresentItemNo(s32 itemNo);
void pouchGetItem(s32 itemNo);

void* btlataudGetAttackEventPtr(s32 unitId) {
    void* data = BtlUnit_GetData(BattleGetUnitPtr(_battleWorkPointer, unitId), 6);

    if (data == 0) {
        data = &btlataudevtDummy[1];
    }
    return data;
}

void* btlataudGetMoveEventPtr(void) {
    switch (BattleAudience_GetPresentItemType()) {
        case 1:
            return btlataudevtPresentItem_Damage;
        default:
            return btlataudevtPresentItem_Get;
    }
}

USER_FUNC(_get_present_target_id) {
    s32* args = event->args;

    evtSetValue(event, args[0], BattleAudience_GetPresentTargetUnitId());
    return 2;
}

USER_FUNC(_get_present_item) {
    void* battleWork = _battleWorkPointer;
    s32 itemNo = BattleAudience_GetPresentItemNo();
    void* unit = BattleGetUnitPtr(battleWork, BattleAudience_GetPresentTargetUnitId());

    if (*(s8*)((s32)unit + 0xC) == 0) {
        pouchGetItem(itemNo);
    } else if (*(s32*)((s32)unit + 0x308) == 0) {
        *(s32*)((s32)unit + 0x308) = itemNo;
    }
    return 2;
}

USER_FUNC(_delete_present_item) {
    s32* args = event->args;
    s32 itemNo = BattleAudience_GetPresentItemNo();

    if (args[0] != 0x1194D80) {
        evtSetValue(event, args[0], itemNo);
    }
    BattleAudience_SetPresentItemNo(0);
    return 2;
}
