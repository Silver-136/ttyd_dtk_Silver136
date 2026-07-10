#include "unit/party/unit_party_chuchurina.h"

extern s32 mono_alpha;
extern void* _battleWorkPointer;
extern f32 float_910_8042412c;
extern s32 BattleTransID();
extern void* BattleGetUnitPtr(void*, s32);
extern void* BtlUnit_GetPartsPtr(void*, s32);
extern void dispEntry(s32, s32, void*, void*, f32);


void mono_disp(int param_1, void* unit) {
    ;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_80381650[];
    s32 techLv;
    void* party;
    u8* data;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_80381650;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
    *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x6A0;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
    *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
        *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
    *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x8E0;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x820;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x9A0;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _chuchu_item_steal(void* evt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 irand(s32 max);
    extern s32 pouchGetItem(s32 itemId);
    extern s32 BtlUnit_CheckUnitFlag(void* unit, u32 flag);
    extern void BtlUnit_OffUnitFlag(void* unit, u32 flag);
    extern void BtlUnit_EquipItem(void* unit, s32 itemType, s32 itemId);
    s32* args;
    s32 id;
    s32 out;
    s32 flags;
    void* battleWork;
    void* unit;
    u16 itemId;
    void* itemTbl;
    s32 total;
    s16 weight;
    s32 roll;
    s32 unitId;
    void* battleUnitWork;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(evt, args[0]);
    out = args[1];
    flags = evtGetValue(evt, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(evt, id));
    itemId = *(u16*)((s32)unit + 0x308);
    if (itemId == 0) {
        if (*(void**)((s32)unit + 0x30C) != 0) {
            irand(100);
        }
        itemTbl = *(void**)((s32)unit + 0x30C);
        if (itemTbl == 0) {
            itemId = 0;
        } else {
            total = 0;
            while ((weight = *(s16*)((s32)itemTbl + 4)) > 0 || *(s32*)itemTbl != 0) {
                total += weight;
                itemTbl = (void*)((s32)itemTbl + 8);
            }
            itemTbl = *(void**)((s32)unit + 0x30C);
            if (total <= 0) {
                itemId = 0;
            } else {
                roll = irand(total);
                while ((roll -= *(s16*)((s32)itemTbl + 4)) >= 0) {
                    itemTbl = (void*)((s32)itemTbl + 8);
                }
                itemId = *(u16*)itemTbl;
            }
        }
        if (itemId == 0) {
            itemId = 0x79;
        }
    }
    if ((flags & 2) == 0) {
        evtSetValue(evt, out, 0);
        return 2;
    }
    if (pouchGetItem(itemId) == 0) {
        evtSetValue(evt, out, 0);
        return 2;
    }
    *(s32*)((s32)unit + 0x308) = 0;
    if (BtlUnit_CheckUnitFlag(unit, 0x40000000) != 0) {
        BtlUnit_OffUnitFlag(unit, 0x40000000);
        unitId = *(s8*)((s32)unit + 0xE);
        if (unitId >= 0) {
            battleUnitWork = *(void**)(*(s32*)((s32)battleWork + 0x2738) + 0xC);
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x1C) = 0;
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x5C) = 0;
        }
    } else {
        unitId = *(s8*)((s32)unit + 0xE);
        if (unitId >= 0) {
            battleUnitWork = *(void**)(*(s32*)((s32)battleWork + 0x2738) + 0xC);
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x1C) = 0;
        }
    }
    if (itemId >= 0xF0 && itemId < 0x153) {
        if (*(s32*)((s32)unit + 8) == 0xDE) {
            BtlUnit_EquipItem(unit, 3, 0);
        } else if (*(s32*)((s32)unit + 8) >= 0xE0 && *(s32*)((s32)unit + 8) < 0xE7) {
            BtlUnit_EquipItem(unit, 5, 0);
        } else {
            BtlUnit_EquipItem(unit, 1, 0);
        }
    }
    evtSetValue(evt, out, itemId);
    return 2;
}

s32 _get_binta_hit_position(int param_1) {
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 evtGetValue(void* evt, s32 arg);

    void* evt = (void*)param_1;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id0;
    s32 id1;
    s32 outX;
    s32 outY;
    s32 outZ;
    void* unit;
    s8 dir;
    f32 z;
    f32 y;
    f32 x;
    f32 scale;

    id0 = evtGetValue(evt, args[0]);
    id1 = evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(battleWork, BattleTransID(evt, id0));
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id1));
    dir = *(s8*)((s32)unit + 0x189);
    BtlUnit_GetPos(unit, &x, &y, &z);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)dir * (*(f32*)((s32)unit + 0xEC) * scale);
    y += *(f32*)((s32)unit + 0xF0) * scale;
    z += *(f32*)((s32)unit + 0xF4) * scale;
    evtSetFloat(evt, outX, x);
    evtSetFloat(evt, outY, y);
    evtSetFloat(evt, outZ, z);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_itemsteal_param(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    void* battleWork;
    s32* args;
    s32 id;
    s32 outRate;
    s32 outSteal;
    void* unit;
    s32 rate;
    s32 steal;

    args = *(s32**)((s32)pEvt + 0x18);
    id = evtGetValue(pEvt, args[0]);
    outRate = args[1];
    battleWork = _battleWorkPointer;
    outSteal = args[2];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(pEvt, id));
    rate = 0x50 - ((*(u8*)((s32)battleWork + 0x1CBD) - 3) * 0x14);

    if (rate <= 5) {
        rate = 5;
    }
    steal = 0x64 - *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0x8D);
    evtSetValue(pEvt, outRate, rate);
    evtSetValue(pEvt, outSteal, steal);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mono_capture(s32 param_1, void* unit) {
    extern u32 GXGetTexBufferSize(u16 width, u16 height, s32 format, s32 mipmap, s32 maxLod);
    extern void* smartAlloc(u32 size, s32 alignment);
    extern void GXSetTexCopySrc(u16 left, u16 top, u16 width, u16 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, s32 format, s32 mipmap);
    extern void GXCopyTex(void* dest, s32 clear);
    extern void GXPixModeSync(void);

    void* parts = BtlUnit_GetPartsPtr(unit, 1);
    *(void**)((s32)unit + 0x31C) = smartAlloc(GXGetTexBufferSize(0x260, 0x1E0, 1, 0, 0), 1);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x260, 0x1E0, 1, 0);
    GXCopyTex(**(void***)((s32)unit + 0x31C), 0);
    GXPixModeSync();
    *(u32*)((s32)parts + 0x204) &= ~0x100;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _make_madowase_weapon(void* evt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern void* memcpy(void* dest, const void* src, u32 size);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    s32 flag = evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0x84) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}

s32 mono_off(void* evt) {
    extern void smartFree(void* ptr);

    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    mono_alpha -= 10;
    if (mono_alpha < 0) {
        mono_alpha = 0;
        BtlUnit_GetPartsPtr(unit, 1);
        if (*(void**)((s32)unit + 0x31C) != 0) {
            smartFree(*(void**)((s32)unit + 0x31C));
        }
        *(void**)((s32)unit + 0x31C) = 0;
        return 2;
    }
    return 0;
}

s32 mono_capture_event(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    BtlUnit_GetPartsPtr(unit, 1);
    dispEntry(8, 0, mono_capture, unit, float_910_8042412c);
    return 2;
}


s32 mono_main(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dispEntry(8, 0, mono_disp, unit, float_910_8042412c);
    return 0;
}


s32 _chuchu_make_extra_work_area(void* evt) {
    extern void* BattleAlloc(u32 size);
    void* battleWork;
    s32 id;
    void* unit;

    battleWork = _battleWorkPointer;
    id = BattleTransID(evt, -2);
    unit = BattleGetUnitPtr(battleWork, id);
    *(void**)((s32)unit + 0x314) = BattleAlloc(0xC0);
    return 2;
}

s32 mono_on(s32 param_1, int param_2) {
    if (param_2 != 0) {
        mono_alpha = 0;
    }
    mono_alpha += 10;
    if (mono_alpha > 0xFF) {
        mono_alpha = 0xFF;
        return 2;
    }
    return 0;
}
