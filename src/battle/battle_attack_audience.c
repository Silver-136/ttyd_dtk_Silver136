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
    s32* args;
    s32 dst;

    args = event->args;
    dst = args[0];

    evtSetValue(event, dst, BattleAudience_GetPresentTargetUnitId());
    return 2;
}

USER_FUNC(_get_present_item) {
    void* battleWork;
    s32 itemNo;
    s32 targetId;
    void* unit;

    battleWork = _battleWorkPointer;
    itemNo = BattleAudience_GetPresentItemNo();

    targetId = BattleAudience_GetPresentTargetUnitId();
    unit = BattleGetUnitPtr(battleWork, targetId);

    if (*(s8*)((s32)unit + 0xC) == 0) {
        pouchGetItem(itemNo);
    } else if (*(s32*)((s32)unit + 0x308) == 0) {
        *(s32*)((s32)unit + 0x308) = itemNo;
    }

    return 2;
}

USER_FUNC(_delete_present_item) {
    s32* args;
    s32 dst;
    s32 itemNo;

    args = event->args;
    dst = args[0];

    itemNo = BattleAudience_GetPresentItemNo();

    if (dst != -0x0EE6B280) {
        evtSetValue(event, dst, itemNo);
    }

    BattleAudience_SetPresentItemNo(0);
    return 2;
}


u8 _get_item_announce_disp(void) {
    return 0;
}


s32 _get_item_announce(int param_1, int param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6) {
    return 0;
}


s32 _get_attack_aud_no(int param_1) {
    return 0;
}


s32 _attack_aud(int param_1) {
    return 0;
}


s32 _check_aud_item_type(int param_1) {
    return 0;
}


s32 _damage(void* pEvt) {
    return 0;
}
