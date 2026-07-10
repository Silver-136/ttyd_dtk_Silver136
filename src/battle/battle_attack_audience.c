#include "battle/battle_attack_audience.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
extern u8 btlataudevtDummy[];
extern u8 btlataudevtPresentItem_Damage[];
extern u8 btlataudevtPresentItem_Get[];
extern u8 itemDataTable[];
extern void* gp;
extern u32 dat_80422f90;
extern char str_btl_msg_get_item_802f4288[];
extern char str_SFX_BTL_DAMAGED_PLIA_802f429c[];
extern f32 float_8_80422f94;
extern f32 float_900_80422f98;
extern f32 float_0_80422f9c;
extern f32 float_10_80422fa0;
extern f32 float_34_80422fa4;
extern f32 float_60_80422fa8;
extern f32 float_32_80422fac;
extern f32 float_1_80422fb0;

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
        data = btlataudevtDummy + 4;
    }
    return data;
}

void* btlataudGetMoveEventPtr(void) {
    switch (BattleAudience_GetPresentItemType()) {
        case 1:
            return btlataudevtPresentItem_Damage;
        case 0:
        default:
            return btlataudevtPresentItem_Get;
    }
}

USER_FUNC(_get_present_target_id) {
    s32 dst;

    dst = event->args[0];

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

void _get_item_announce_disp(void) {
    extern char* msgSearch(char* msgId);
    extern int sprintf(char* str, const char* format, ...);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d1364(char* dst, const char* src);
    extern s32 FontGetMessageWidthLine(char* msg, void* lines);
    extern void windowDispGX_Waku_col(s32 texture, void* color, f32 x, f32 y, f32 width, f32 height, f32 curve);
    extern void FontDrawStart(void);
    extern void FontDrawMessage(s32 x, s32 y, char* msg);
    u32 color;
    u16 lines[2];
    char buf[0x80];
    char* itemName;
    s32 width;
    s32 halfWidth;
    f32 x;
    u16 lineCount;

    itemName = msgSearch(*(char**)(itemDataTable + BattleAudience_GetPresentItemNo() * 0x28 + 4));
    if (*(void**)((s32)gp + 0x16C) == 0) {
        sprintf(buf, msgSearch(str_btl_msg_get_item_802f4288), itemName);
    } else {
        strcpy(buf, msgSearch(str_btl_msg_get_item_802f4288));
        unk_800d1364(buf, itemName);
    }
    width = (u16)FontGetMessageWidthLine(buf, lines);
    halfWidth = width / 2;
    lineCount = lines[0] + 1;
    x = float_0_80422f9c - (f32)halfWidth;
    lines[0] = lineCount;
    color = dat_80422f90;
    windowDispGX_Waku_col(0, &color, x - float_10_80422fa0, float_34_80422fa4, width + 0x14, lineCount * 0x1D + 3, float_10_80422fa0);
    FontDrawStart();
    FontDrawMessage((s32)x, (s32)float_60_80422fa8 - 0x1D, buf);
}


s32 _get_item_announce(void* event, s32 isFirstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern s32 itemEntry(s32 id, s32 itemNo, s32 type, s32 a, s32 b, f32 x, f32 y, f32 z);
    extern void itemDelete(void* name);
    extern void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);
    void* battleWork = _battleWorkPointer;
    s32 itemNo = BattleAudience_GetPresentItemNo();
    void* unit;
    s32 targetId;
    f32 x;
    f32 y;
    f32 z;

    if (isFirstCall != 0) {
        targetId = BattleAudience_GetPresentTargetUnitId();
        unit = BattleGetUnitPtr(battleWork, targetId);
        BtlUnit_GetPos(unit, &x, &y, &z);
        y += *(f32*)((s32)unit + 0x114) * BtlUnit_GetHeight(unit) + float_8_80422f94;
        *(s32*)((s32)event + 0x7C) = itemEntry(0, itemNo, 0x11, -1, 0, x, y, z);
        *(s32*)((s32)event + 0x80) = 0x5A;
    } else {
        if (*(s32*)((s32)event + 0x80) > 0) {
            *(s32*)((s32)event + 0x80) -= 1;
        }
        if (*(s32*)((s32)event + 0x80) <= 0) {
            itemDelete((void*)(*(s32*)((s32)event + 0x7C) + 0xC));
            return 2;
        }
    }
    dispEntry(8, 1, _get_item_announce_disp, float_900_80422f98, 0);
    return 0;
}


s32 _get_attack_aud_no(int param_1) {
    extern void BattleAudience_GetItemOn2(void* itemNo, f32* x, f32* y, f32* z, s32 flag);
    s32 itemNo;
    f32 x;
    f32 y;
    f32 z;
    s32* args = *(s32**)((s32)param_1 + 0x18);
    s32 dstItem = args[0];
    s32 dstX = args[1];
    s32 dstY = args[2];
    s32 dstZ = args[3];

    BattleAudience_GetItemOn2(&itemNo, &x, &y, &z, 0);
    if (dstItem != -0x0EE6B280) {
        evtSetValue((void*)param_1, dstItem, itemNo);
    }
    if (dstX != -0x0EE6B280) {
        evtSetValue((void*)param_1, dstX, (s32)x);
    }
    if (dstY != -0x0EE6B280) {
        evtSetValue((void*)param_1, dstY, (s32)y);
    }
    if (dstZ != -0x0EE6B280) {
        evtSetValue((void*)param_1, dstZ, (s32)z);
    }
    return 2;
}


s32 _attack_aud(void* event) {
    extern void BattleAudience_GetItemOn2(void* itemNo, f32* x, f32* y, f32* z, s32 flag);
    extern void BattleAudience_GetPosition(s32 id, f32* x, f32* y, f32* z);
    extern void BattleAudience_Attack(s32 id);
    extern void effHitEntry(s32 cameraId, s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern s32 psndSFXOn_3D(char* name, f32* pos);
    extern void psndSFX_pit(s32 id, s32 pitch);
    void* evt;
    s32* args;
    f32 z;
    f32 y;
    f32 x;
    s32 audienceId;
    s32 itemType;
    f32* yPtr;
    f32* zPtr;
    char* soundName;

    evt = event;
    args = *(s32**)((s32)evt + 0x18);
    audienceId = evtGetValue(evt, args[0]);
    itemType = evtGetValue(evt, args[1]);
    yPtr = &y;
    zPtr = &z;
    soundName = 0;
    BattleAudience_GetItemOn2(&audienceId, &x, yPtr, zPtr, 0);
    y += float_32_80422fac;
    if (itemType != 1) {
        effHitEntry(0, 3, x, y, z, float_1_80422fb0);
        soundName = str_SFX_BTL_DAMAGED_PLIA_802f429c;
    }
    BattleAudience_GetPosition(audienceId, &x, yPtr, zPtr);
    if (soundName != 0) {
        psndSFX_pit(psndSFXOn_3D(soundName, &x), 0x3FF);
    }
    BattleAudience_Attack(audienceId);
    return 2;
}

s32 _check_aud_item_type(int param_1) {
    extern void BattleAudience_GetItemOn2(void* itemNo, f32* x, f32* y, f32* z, s32 flag);
    s32 itemNo;
    s32 dst = **(s32**)((s32)param_1 + 0x18);
    s32 result;

    evtSetValue((void*)param_1, dst, 1);
    BattleAudience_GetItemOn2(0, 0, 0, 0, (s32)&itemNo);
    if (itemNo == 0) {
        return 2;
    }
    if (itemNo >= 0xEC && itemNo < 0xF0) {
        result = 1;
    } else {
        result = 0;
    }
    evtSetValue((void*)param_1, dst, result);
    return 2;
}


s32 _damage(void* pEvt) {
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void* BattleAudienceBaseGetPtr(void);
    extern void BattleActionCommandResetDefenceResult(void);
    extern void* BattleGetSystemPtr(void* battleWork);
    extern void BattleCheckDamage(void* system, void* unit, void* parts, void* attack, s32 flags);
    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 targetId = BattleAudience_GetPresentTargetUnitId();
    void* unit = BattleGetUnitPtr(battleWork, targetId);
    s32 partId = BtlUnit_GetBodyPartsId(unit);
    void* parts = BtlUnit_GetPartsPtr(unit, partId);
    void* audienceBase;

    evtGetValue(pEvt, args[0]);
    audienceBase = BattleAudienceBaseGetPtr();
    BattleActionCommandResetDefenceResult();
    BattleCheckDamage(BattleGetSystemPtr(battleWork), unit, parts, (void*)((s32)audienceBase + 0x137FC), 0x100);
    return 2;
}
