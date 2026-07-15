#include "battle/battle.h"

#include "battle/battle_ac.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_disp.h"
#include "battle/battle_icon.h"
#include "battle/battle_menu_disp.h"
#include "battle/battle_pad.h"
#include "battle/battle_seq.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_status_effect.h"
#include "mario/mariost.h"

//.sbss
BattleWork* _battleWorkPointer;

// Local Prototypes
BOOL battleSeqEndCheck(void);
BOOL BattleMain(void);

BattleWorkUnit* BattleGetUnitPtr(BattleWork* wp, s32 index);

void BattlePartyInfoWorkInit(BattleWork* wp);

void BattleCheckUnitBroken(BattleWork* wp);

void BattleAfterReactionQueueInit(void);

void battleMain(void) {
    BattleWork* wp = _battleWorkPointer;
    if (wp) {
        if (BattleMain() == FALSE) {
            wp->flags |= 0x80000000;
        }
    }
}

BOOL battleSeqEndCheck(void) {
    return _battleWorkPointer->flags >> 31;
}

// This function only matches with -O4,s and the 0-9 loop unrolled
void BattleInit(FieldBattleInfo* info) {
    BattleWork* wp;
    int i;

    _battleWorkPointer = BattleAlloc(sizeof(BattleWork));
    memset((wp = _battleWorkPointer), 0, sizeof(BattleWork));

    wp->curtainSFXId = -1;
    wp->lectureUnitId = -1;
    wp->info = info;

    BattlePadInit();
    BattleAcHelpInit();
    BattleIconInit();
    BattleActionCommandManagerInit(wp);
    BattleAfterReactionQueueInit();
    BattleStatusChangeMsgWorkInit();
    btlDispInit();
    BattleSetSeq(wp, SEQ_UNKNOWN, 0);
    BattleSetSeq(wp, SEQ_INIT, 0x1000000);

    // Not sure why we're doing all this again when we already memset the entire struct but okay
    wp->endSequenceWork = NULL;
    wp->commandMenu.flags = 0;
    // This also isn't the full amount of cursors so someone oopsied
    wp->commandMenu.cursors[0].absolutePos = 0;
    wp->commandMenu.cursors[0].relativePos = 0;
    wp->commandMenu.cursors[1].absolutePos = 0;
    wp->commandMenu.cursors[1].relativePos = 0;
    wp->commandMenu.cursors[2].absolutePos = 0;
    wp->commandMenu.cursors[2].relativePos = 0;
    wp->commandMenu.cursors[3].absolutePos = 0;
    wp->commandMenu.cursors[3].relativePos = 0;
    wp->commandMenu.cursors[4].absolutePos = 0;
    wp->commandMenu.cursors[4].relativePos = 0;
    wp->commandMenu.cursors[5].absolutePos = 0;
    wp->commandMenu.cursors[5].relativePos = 0;
    wp->commandMenu.cursors[6].absolutePos = 0;
    wp->commandMenu.cursors[6].relativePos = 0;
    wp->commandMenu.cursors[7].absolutePos = 0;
    wp->commandMenu.cursors[7].relativePos = 0;
    wp->commandMenu.cursors[8].absolutePos = 0;
    wp->commandMenu.cursors[8].relativePos = 0;
    wp->commandMenu.cursors[9].absolutePos = 0;
    wp->commandMenu.cursors[9].relativePos = 0;
    wp->commandMenu.menu = NULL;

    wp->turnCount = 0;
    memset(wp->reserveItems, 0, sizeof(wp->reserveItems));
    wp->flags = 0;

    BtlUnit_Init();
    BattlePartyInfoWorkInit(wp);
    BattleStageInit();
    BattleAudience_Init();
    BattleBreakSlot_Init();
}

BOOL BattleMain(void) {
    BattleWork* wp = _battleWorkPointer;

    BattleCheckUnitBroken(wp);
    BattlePadManager();
    BattleSequenceManager();
    BattleActionCommandManager(wp);
    BattleStageObjectMain();
    BattleIconMain();
    BattleStageMain();
    BattleAcHelpMain();
    BattleAudience_Main();
    BattleBreakSlot_Main();
    btlDispMain();

    if (BattleGetSeq(wp, SEQ_UNKNOWN) == 4) {
        return FALSE;
    }

    if ((gp->flags & 0x1000) && (gp->flags & 0x2000) && BattleGetSeq(wp, SEQ_UNKNOWN) == 2) {
        wp->flags |= 0x60;
        BattleSetSeq(wp, SEQ_UNKNOWN, 3);
        BattleSetSeq(wp, SEQ_END, 0x7000000);
        return TRUE; //very good
    }

    return TRUE;
}

void BattleEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleWorkUnit* unit;
    BattleWorkCommandMenu* menu;
    int i;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            BtlUnit_Delete(unit);
        }
    }
    BattleIconEnd();
    battleMenuDispEnd();
    BattleStageEnd();
    BattleAudience_End();
    BattleBreakSlot_End();
    menu = wp->commandMenu.menu;
    if (menu) {
        BattleFree(menu);
    }
    BattleFree(wp);
    _battleWorkPointer = NULL;
}

BOOL Btl_UnitSetup(BattleWork* wp) {
    typedef struct PartyEntry {
        void* data;
        u8 pad[0x30 - 0x4];
    } PartyEntry;
    extern BattleUnitSetup entryunit_system;
    extern BattleUnitSetup entryunit_mario;
    extern PartyEntry entryunit_party[];
    extern BattleWorkUnit* BtlUnit_Entry(BattleUnitSetup* setup);
    extern void BtlUnit_EquipItem(void* unit, u32 flags, u32 item);
    extern void BtlUnit_SetParamFromPouch(BattleWorkUnit* unit);
    extern void BtlUnit_GetHomePos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
    extern f32 float_neg140_80422178;
    extern f32 float_5_8042217c;
    void* setupRoot;
    void* enemyList;
    BattleUnitSetup* enemySetup;
    BattleWorkUnit* unit;
    BattleWorkUnit* mario;
    PouchData* pouch;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 offset;
    s16 badge;
    s32 partyId;

    setupRoot = *(void**)(*(s32*)((s32)wp + 0x2738) + 0xC);
    enemyList = *(void**)((s32)setupRoot + 0x18);
    pouch = pouchGetPtr();

    *(s16*)((s32)wp + 0x8) = 2;
    *(s16*)((s32)wp + 0x10) = 1;
    *(s16*)((s32)wp + 0x18) = 0;
    *(s8*)((s32)wp + 0xA) = 1;
    *(s8*)((s32)wp + 0x12) = -1;
    *(s8*)((s32)wp + 0x1A) = 1;
    *(s32*)((s32)wp + 0xC) = 0;
    *(s32*)((s32)wp + 0x14) = 0;
    *(s32*)((s32)wp + 0x1C) = 0;

    unit = BtlUnit_Entry(&entryunit_system);
    BtlUnit_EquipItem(unit, 3, 0);

    mario = BtlUnit_Entry(&entryunit_mario);
    BtlUnit_EquipItem(mario, 3, 0);
    BtlUnit_SetParamFromPouch(mario);

    partyId = *(s32*)(*(s32*)((s32)wp + 0x2738) + 4);
    if (partyId != 0) {
        unit = BtlUnit_Entry((BattleUnitSetup*)&entryunit_party[partyId - 0xE0]);
        BtlUnit_GetHomePos(unit, &x, &y, &z);
        BtlUnit_SetHomePos(unit, float_neg140_80422178, y, float_5_8042217c);
        BtlUnit_SetPos(unit, float_neg140_80422178, y, float_5_8042217c);
        BtlUnit_EquipItem(unit, 5, 0);
        BtlUnit_SetParamFromPouch(unit);
    }

    enemySetup = *(BattleUnitSetup**)((s32)enemyList + 4);
    for (i = 0, offset = 0; i < *(s32*)enemyList; i++, offset += 4) {
        unit = BtlUnit_Entry(enemySetup);
        unit->groupId = i;
        *(s32*)((s32)unit + 0x308) = 0;
        if (*(s32*)((s32)setupRoot + 0x5C + unit->groupId * 4) != 0) {
            *(s32*)((s32)unit + 0x308) = *(s32*)((s32)setupRoot + 0x5C + unit->groupId * 4);
            unit->flags |= 0x40000000;
        } else {
            *(s32*)((s32)unit + 0x308) = *(s32*)((s32)setupRoot + 0x1C + offset);
            BtlUnit_EquipItem(unit, 1, *(s32*)((s32)setupRoot + 0x1C + offset));
        }
        *(s32*)((s32)unit + 0x30C) = *(s32*)((s32)setupRoot + 0x3C + offset);
        enemySetup = (BattleUnitSetup*)((s32)enemySetup + 0x30);
        unit->currentHP = unit->currentMaxHP;
        unit->currentFP = unit->currentMaxFP;
    }

    *(s32*)((s32)wp + 0x420) = -1;
    *(s32*)((s32)wp + 0x163F4) = 0;
    for (i = 0, offset = 0; i < 200; i++, offset += 2) {
        badge = *(s16*)((s32)pouch + 0x38A + offset);
        switch (badge) {
            case 0x134:
                *(s32*)((s32)wp + 0x163F4) |= 1;
                break;
            case 0x135:
                *(s32*)((s32)wp + 0x163F4) |= 2;
                break;
            case 0x136:
                *(s32*)((s32)wp + 0x163F4) |= 4;
                break;
            case 0x137:
                *(s32*)((s32)wp + 0x163F4) |= 8;
                break;
            case 0x138:
                *(s32*)((s32)wp + 0x163F4) |= 0x10;
                break;
            case 0x13A:
                *(s32*)((s32)wp + 0x163F4) |= 0x80;
                break;
            case 0x13B:
                *(s32*)((s32)wp + 0x163F4) |= 0x100;
                break;
            case 0x13C:
                *(s32*)((s32)wp + 0x163F4) |= 0x400;
                break;
            case 0x13D:
                *(s32*)((s32)wp + 0x163F4) |= 0x800;
                break;
            case 0x13E:
                *(s32*)((s32)wp + 0x163F4) |= 0x200;
                break;
            case 0x13F:
                *(s32*)((s32)wp + 0x163F4) |= 0x1000;
                break;
        }
    }
    return TRUE;
}

void BattleStoreExp(BattleWork* wp, s32 exp) {
    *(s32*)((s32)wp + 0xF00) += exp;
}

void BattleStoreCoin(BattleWork* wp, s32 coin) {
    void* info = *(void**)((s32)wp + 0x2738);
    *(s32*)((s32)info + 0x14) += coin;
}

void BattleSetUnitPtr(BattleWork* wp, s32 index, BattleWorkUnit* unit) {
    if (index < 0x40) {
        *(BattleWorkUnit**)((s32)wp + 0x20 + index * 4) = unit;
    }
}

s32 BattleGetStockExp(BattleWork* wp) {
    s32 exp = *(s32*)((s32)wp + 0xF00);
    if (exp <= 0) {
        return 1;
    }
    if (exp >= 100) {
        return 100;
    }
    return exp;
}

void* BattleGetPartnerPtr(BattleWork* wp, void* unit) {
    extern void* BattleGetPartyPtr(BattleWork*, void*);
    extern void* BattleGetMarioPtr(BattleWork*, void*);
    if (*(s32*)((s32)unit + 8) == 0xDE) {
        return BattleGetPartyPtr(wp, unit);
    }
    return BattleGetMarioPtr(wp, unit);
}

void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId) {
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    return BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unitId), partsId);
}

void BattleStatusWindowAPRecoveryOff(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x274C) = 0;
    BattleStatusWindowCheck();
}

void BattleStatusWindowAPRecoveryOn(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x274C) = 1;
    BattleStatusWindowCheck();
}

void BattleStatusWindowEventOff(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x2748) = 0;
    BattleStatusWindowCheck();
}

void BattleStatusWindowEventOn(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x2748) = 1;
    BattleStatusWindowCheck();
}

void BattleStatusWindowSystemOff(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x2744) = 0;
    BattleStatusWindowCheck();
}

void BattleStatusWindowSystemOn(void) {
    extern void BattleStatusWindowCheck(void);
    *(s32*)((s32)_battleWorkPointer + 0x2744) = 1;
    BattleStatusWindowCheck();
}

BattleWorkUnit* BattleGetUnitPtr(BattleWork* wp, s32 index) {
    if (index == -1) {
        return NULL;
    }
    if (index < -1) {
        return NULL;
    }
    if (index < 0x40) {
        return *(BattleWorkUnit**)((s32)wp + 0x20 + index * 4);
    }
    return NULL;
}

void* BattleAlloc(u32 size) {
    extern void* R_battlemapalloc_base_ptr;
    extern void* _mapAlloc(void*, u32);
    return _mapAlloc(R_battlemapalloc_base_ptr, size);
}

void BattleFree(void* ptr) {
    extern void* R_battlemapalloc_base_ptr;
    extern void _mapFree(void*, void*);

    if (ptr != NULL) {
        _mapFree(R_battlemapalloc_base_ptr, ptr);
    }
}

void BattleMajinaiEndCheck(void) {
    extern void* pouchGetPtr(void);
    BattleWork* wp = _battleWorkPointer;
    void* pouch = pouchGetPtr();

    if (*(u8*)((s32)pouch + 0x5B8) == 0) {
        wp->flags |= 0x400000;
    }
}

void BattleSetMarioParamToFieldBattle(BattleWork* wp) {
    extern void* BattleGetMarioPtr(BattleWork*);
    extern void* BattleGetPartyPtr(BattleWork*);
    extern void BtlUnit_SetParamToPouch(void*);
    void* unit;

    unit = BattleGetMarioPtr(wp);
    BtlUnit_SetParamToPouch(unit);
    unit = BattleGetPartyPtr(wp);
    if (unit != NULL) {
        BtlUnit_SetParamToPouch(unit);
    }
}

f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z) {
    extern void* BattleStageGetPtr(void);
    extern f32 float_0_80422170;
    void* stage = BattleStageGetPtr();

    if (z <= *(f32*)((s32)stage + 0xB2C)) {
        return float_0_80422170;
    }
    return *(f32*)((s32)stage + 0xB34);
}

void BattleCheckUnitBroken(BattleWork* wp) {
    extern void BtlUnit_Delete(void*);
    s32 i;
    void* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && (*(u32*)((s32)unit + 0x1C) & 0x80000000)) {
            BtlUnit_Delete(unit);
        }
    }
}

void BattleMajinaiDone(void) {
    extern void* pouchGetPtr(void);
    void* pouch = pouchGetPtr();
    u8 count = *(u8*)((s32)pouch + 0x5B8);

    *(u8*)((s32)pouch + 0x5B8) = count - 1;
    if (*(u8*)((s32)pouch + 0x5B8) != 0) {
        *(s8*)((s32)pouch + 0x5B9) = -1;
        *(u8*)((s32)pouch + 0x5BA) = 0;
    } else {
        *(s8*)((s32)pouch + 0x5B9) = -1;
        *(u8*)((s32)pouch + 0x5BA) = 0;
    }
}

void* BattleGetSystemPtr(BattleWork* wp) {
    s32 i;
    void* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && *(s32*)((s32)unit + 8) == 0xDC) {
            return unit;
        }
    }
    return NULL;
}

void* BattleGetMarioPtr(BattleWork* wp) {
    s32 i;
    void* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && *(s32*)((s32)unit + 8) == 0xDE && *(s8*)((s32)unit + 0xC) == 0) {
            return unit;
        }
    }
    return NULL;
}

s32 battleDisableHResetCheck(void) {
    BattleWork* wp = _battleWorkPointer;

    if (*(s32*)((s32)gp + 0x14) == 0) {
        return 0;
    }
    if (wp == NULL) {
        return 0;
    }
    return (wp->flags >> 29) & 1;
}

s32 BattlePartyAnimeLoad(s32 partyId) {
    typedef struct PartyEntry {
        void* data;
        u8 pad[0x30 - 0x4];
    } PartyEntry;
    extern PartyEntry entryunit_party[];
    extern s32 animEffectAsync(void*, s32);
    s32 async = 1;

    partyId -= 0xE0;
    return animEffectAsync(*(void**)(*(s32*)((s32)entryunit_party[partyId].data + 0xB8) + 8), async);
}

void* BattleGetPartyPtr(BattleWork* wp) {
    s32 i;
    void* unit;
    s32 kind;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL) {
            if ((*(u32*)((s32)unit + 0x1C) & 0x8) == 0) {
                kind = *(s32*)((s32)unit + 8);
                if (kind >= 0xE0 && kind < 0xE7 && *(s8*)((s32)unit + 0xC) == 0) {
                    return unit;
                }
            }
        }
    }
    return NULL;
}


void _EquipItem(void* unit, u32 flags, u32 item) {
    u8* data = unit;
    #define ADD_BADGE(value, offset) if (item == (value)) { data[offset]++; return; }
    if (item == 0) return;
    ADD_BADGE(0x131, 0x2FB)
    ADD_BADGE(0x144, 0x305)
    ADD_BADGE(0x145, 0x306)
    if ((flags & 4) == 0) {
        ADD_BADGE(0x11C, 0x302)
        ADD_BADGE(0x11B, 0x301)
        ADD_BADGE(0x12B, 0x2E0)
        ADD_BADGE(0x12D, 0x2E1)
        ADD_BADGE(0x12F, 0x2E2)
        ADD_BADGE(0x114, 0x2ED)
        ADD_BADGE(0x104, 0x2E4)
        ADD_BADGE(0x106, 0x2E5)
        ADD_BADGE(0x108, 0x2E6)
        ADD_BADGE(0x10A, 0x2E7)
        ADD_BADGE(0x10C, 0x2E8)
        ADD_BADGE(0x10E, 0x2E9)
        ADD_BADGE(0x120, 0x2EA)
        ADD_BADGE(0x110, 0x2EB)
        ADD_BADGE(0x112, 0x2EC)
        ADD_BADGE(0x124, 0x2EE)
        ADD_BADGE(0x126, 0x2EF)
        ADD_BADGE(0x123, 0x2F0)
        ADD_BADGE(0x116, 0x2F1)
        ADD_BADGE(0x118, 0x2F2)
        ADD_BADGE(0x11D, 0x2F6)
        ADD_BADGE(0x11F, 0x2F7)
        ADD_BADGE(0x132, 0x2F8)
        ADD_BADGE(0x127, 0x2F9)
        ADD_BADGE(0x129, 0x2FA)
        ADD_BADGE(0x14F, 0x2FE)
        ADD_BADGE(0x100, 0x2FC)
        ADD_BADGE(0x151, 0x2FD)
        ADD_BADGE(0x119, 0x2F5)
        ADD_BADGE(0x102, 0x303)
        ADD_BADGE(0x146, 0x304)
    }
    if ((flags & 2) == 0) {
        ADD_BADGE(0x12C, 0x2E0)
        ADD_BADGE(0x12E, 0x2E1)
        ADD_BADGE(0x130, 0x2E2)
        ADD_BADGE(0x115, 0x2ED)
        ADD_BADGE(0x109, 0x2E6)
        ADD_BADGE(0x105, 0x2E4)
        ADD_BADGE(0x10D, 0x2E8)
        ADD_BADGE(0x113, 0x2EC)
        ADD_BADGE(0x150, 0x2FE)
        ADD_BADGE(0x10F, 0x2E9)
        ADD_BADGE(0x133, 0x2F8)
        ADD_BADGE(0x128, 0x2F9)
        ADD_BADGE(0x12A, 0x2FA)
        ADD_BADGE(0x101, 0x2FC)
        ADD_BADGE(0x152, 0x2FD)
        ADD_BADGE(0x125, 0x2EE)
        ADD_BADGE(0x117, 0x2F1)
        ADD_BADGE(0x111, 0x2EB)
        ADD_BADGE(0x10B, 0x2E7)
        ADD_BADGE(0x107, 0x2E5)
        ADD_BADGE(0x11A, 0x2F5)
        ADD_BADGE(0x11E, 0x2F6)
        ADD_BADGE(0x103, 0x303)
        ADD_BADGE(0x14A, 0x304)
    }
    if ((flags & 4) == 0) {
        ADD_BADGE(0x121, 0x2FF)
        ADD_BADGE(0x122, 0x300)
    }
    if ((flags & 2) == 0 && item == 0xFF) {
        data[0x2F3]++;
        if (data[0x2F3] >= 2 && data[0x2F4] == 0) data[0x2F4] = 1;
        return;
    }
    if ((flags & 4) == 0 && item == 0xFE) {
        data[0x2F3]++;
        if (data[0x2F3] >= 2 && data[0x2F4] == 0) data[0x2F4] = 1;
        return;
    }
    if ((flags & 6) != 0) {
        ADD_BADGE(0x14B, 0x307)
    }
    #undef ADD_BADGE
}

void* BattleChangeParty(void* battleWork) {
    typedef struct PartyEntry {
        void* data;
        u8 pad[0x30 - 0x4];
    } PartyEntry;
    extern PartyEntry entryunit_party[];
    extern void* BattleGetPartyPtr(void);
    extern void* BtlUnit_Spawn(void* setup, s32 flag);
    extern void BtlUnit_GetHomePos(void* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_EquipItem(void* unit, u32 flags, u32 item);
    extern f32 BattleGetFloorHeight(void* wp, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetHomePos(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetParamToPouch(void* unit);
    s32 kind = *(s32*)((s32)battleWork + 0x1C68);
    void* oldUnit = BattleGetPartyPtr();
    s32 oldId = *(s32*)oldUnit;
    void* newUnit;
    s32 i;
    f32 x;
    f32 y;
    f32 z;

    if (kind < 0xE0 || kind >= 0xE7) {
        return NULL;
    }
    newUnit = BtlUnit_Spawn(&entryunit_party[kind - 0xE0], 1);
    for (i = 0; i < 64; i++) {
        s32* slot = (s32*)((s32)battleWork + 0x120 + i * 4);
        if (*slot != -1) {
            if (*slot == oldId) {
                *slot = *(s32*)newUnit;
            } else if (*slot == *(s32*)newUnit) {
                *slot = oldId;
            }
        }
    }
    BtlUnit_GetHomePos(newUnit, &x, &y, &z);
    BtlUnit_GetHomePos(oldUnit, &x, &y, &z);
    BtlUnit_EquipItem(newUnit, 5, 0);
    y += BattleGetFloorHeight(_battleWorkPointer, x, y, z);
    BtlUnit_SetHomePos(newUnit, x, y, z);
    if (*(s32*)((s32)battleWork + 0x420) == oldId) {
        *(s32*)((s32)battleWork + 0x420) = *(s32*)newUnit;
    }
    *(u8*)((s32)newUnit + 0x20) = *(u8*)((s32)oldUnit + 0x20);
    *(u8*)((s32)newUnit + 0x22) = *(u8*)((s32)oldUnit + 0x22);
    *(u8*)((s32)newUnit + 0x23) = *(u8*)((s32)oldUnit + 0x23);
    *(u8*)((s32)newUnit + 0x24) = *(u8*)((s32)oldUnit + 0x24);
    BtlUnit_SetParamToPouch(oldUnit);
    *(u32*)((s32)oldUnit + 0x1C) |= 8;
    *(u8*)((s32)newUnit + 0x311) = *(u8*)((s32)oldUnit + 0x311);
    *(u8*)((s32)newUnit + 0x312) = *(u8*)((s32)oldUnit + 0x312);
    return newUnit;
}

u32 BattleMajinaiCheck(void) {
    extern void* pouchGetPtr(void);
    extern s32 irand(s32);
    void* pouch = pouchGetPtr();
    BattleWork* wp = _battleWorkPointer;
    u8 turn;
    s32 value;

    if (*(u8*)((s32)pouch + 0x5B8) == 0) {
        return 0;
    }

    turn = *(u8*)((s32)pouch + 0x5B9);
    if ((s8)turn < 0) {
        *(u8*)((s32)pouch + 0x5BA) = 0;
        *(u8*)((s32)pouch + 0x5B9) = irand(5) + 6;
        *(u8*)((s32)pouch + 0x5B9) = *(u8*)((s32)pouch + 0x5B9) - 1;
        return 0;
    }

    value = *(u8*)((s32)pouch + 0x5BA);
    if (value != 0) {
        return value;
    }

    *(u8*)((s32)pouch + 0x5B9) = turn - 1;
    if (*(s8*)((s32)pouch + 0x5B9) > 0) {
        return 0;
    }

    *(u8*)((s32)pouch + 0x5B9) = 0;
    if ((*(u32*)(*(s32*)(*(s32*)((s32)wp + 0x2738) + 0xC) + 4) & 0x20) != 0) {
        value = irand(100);
        if (value < 45) {
            *(u8*)((s32)pouch + 0x5BA) = 1;
        } else if (value < 90) {
            *(u8*)((s32)pouch + 0x5BA) = 2;
        } else {
            *(u8*)((s32)pouch + 0x5BA) = 3;
        }
    } else {
        value = irand(100);
        if (value < 30) {
            *(u8*)((s32)pouch + 0x5BA) = 1;
        } else if (value < 60) {
            *(u8*)((s32)pouch + 0x5BA) = 2;
        } else if (value < 80) {
            *(u8*)((s32)pouch + 0x5BA) = 3;
        } else {
            *(u8*)((s32)pouch + 0x5BA) = 4;
        }
    }
    return *(u8*)((s32)pouch + 0x5BA);
}

s32 BattleTransPartyId(BattleUnitType type) {
    switch (type) {
        case 0x50:
        case 0x89:
        case UNIT_GOOMBELLA:
            return 1;
        case 0x51:
        case 0x8A:
        case UNIT_KOOPS:
            return 2;
        case 0x52:
        case 0x8B:
        case UNIT_YOSHI:
            return 4;
        case 0x53:
        case 0x8C:
        case UNIT_FLURRIE:
            return 5;
        case 0x8D:
        case UNIT_VIVIAN:
            return 6;
        case 0x8E:
        case UNIT_BOBBERY:
            return 3;
        case 0x8F:
        case UNIT_MS_MOWZ:
            return 7;
        case 0:
            return 0;
        default:
            return 0;
    }
}

s32 BattleAfterReactionMain(void) {
    extern BattleWorkUnit* BattleGetUnitPtr(BattleWork* wp, s32 index);
    extern void BattleConsumeReserveItem(void);
    extern s32 BattleCheckConcluded(BattleWork* wp);
    extern void BattleAfterReactionRelease(int unitId, int arg);
    extern void* BtlUnit_GetData(void* unit, s32 arg);
    extern void* evtEntry(void* script, s32 type, s32 flags);
    BattleWork* wp = _battleWorkPointer;
    s32* queue = (s32*)((s32)wp + 0x16F5C);
    s32 i;

    for (i = 0; i < 64; i++) {
        s32 unitId = queue[i * 2];
        s32 arg;
        void* unit;
        void* evt;

        if (unitId == -1) {
            continue;
        }
        arg = queue[i * 2 + 1];
        unit = BattleGetUnitPtr(wp, unitId);
        if (unit == NULL) {
            BattleAfterReactionRelease(unitId, arg);
            return BattleAfterReactionMain();
        }
        BattleConsumeReserveItem();
        if (BattleCheckConcluded(wp) != 0) {
            return 0;
        }
        BattleAfterReactionRelease(unitId, arg);
        evt = evtEntry(BtlUnit_GetData(unit, arg), 10, 0);
        if (evt != NULL) {
            *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
            *(s32*)((s32)evt + 0x160) = *(s32*)unit;
        }
        return 1;
    }
    return 0;
}

void BtlUnit_EquipItem(void* unit, u32 param_2, u32 item) {
    extern void* pouchGetPtr(void);
    extern void BtlUnit_ReviseHpFp(void*);
    void* pouch;
    void* unitLocal;
    u32 flags;
    u32 itemLocal;
    s32 i;
    s32 offset;

    unitLocal = unit;
    flags = param_2;
    itemLocal = item;
    pouch = pouchGetPtr();
    if (unitLocal != NULL) {
        if (flags & 1) {
            for (i = 0; i < 40; i++) {
                *(u8*)((s32)unitLocal + 0x2E0 + i) = 0;
            }
        }
        if (*(s32*)((s32)unitLocal + 8) != 0xDC) {
            if (flags & 6) {
                itemLocal = 0;
                flags &= ~1;
                offset = itemLocal;
                for (i = 0; i < 200; i++, offset += 2) {
                    _EquipItem(unitLocal, flags, *(s16*)((s32)pouch + 0x38A + offset));
                }
            } else {
                _EquipItem(unitLocal, flags, itemLocal);
            }
            BtlUnit_ReviseHpFp(unitLocal);
        }
    }
}

s32 BattleStatusWindowCheck(void) {
    extern s32 statusWinCheck(void);
    extern void statusWinOpen(void);
    extern void statusWinClose(void);
    BattleWork* wp = _battleWorkPointer;
    s32 flag;

    if (*(s32*)((s32)wp + 0x273C) != 0) {
        flag = 0;
    } else if (*(s32*)((s32)wp + 0x2740) != 0) {
        flag = 0;
    } else if (*(s32*)((s32)wp + 0x274C) != 0) {
        flag = 1;
    } else if (*(s32*)((s32)wp + 0x2748) != 0 && *(s32*)((s32)wp + 0x2744) != 0) {
        flag = 1;
    } else {
        flag = 0;
    }

    if (flag != 0) {
        if (statusWinCheck() == 0) {
            statusWinOpen();
        }
    } else {
        if (statusWinCheck() != 0) {
            statusWinClose();
        }
    }
    return flag;
}

void BattleAfterReactionRelease(int param_1, int param_2) {
    s32 i;
    s32 j;
    s32 offset;
    s32* queue;

    queue = (s32*)((s32)_battleWorkPointer + 0x16F5C);
    for (i = 0; i < 64; i++) {
        offset = i * 8;
        if (*(s32*)((s32)queue + offset) != -1 &&
            param_1 == *(s32*)((s32)queue + offset) &&
            param_2 == *(s32*)((s32)queue + offset + 4)) {
            for (j = i; j < 63; j++) {
                offset = j * 8;
                *(s32*)((s32)queue + offset) = *(s32*)((s32)queue + offset + 8);
                *(s32*)((s32)queue + offset + 4) = *(s32*)((s32)queue + offset + 12);
            }
            queue[126] = -1;
            i--;
            queue[127] = 0;
        }
    }
}

void BattleAfterReactionEntry(s32 unitId, s32 arg) {
    s32 i;
    s32 count;
    s32 offset;
    s32* queue;

    queue = (s32*)((s32)_battleWorkPointer + 0x16F5C);
    offset = 0;
    for (i = 0; i < 64; i++, offset += 8) {
        s32 value = *(s32*)((s32)queue + offset);
        if (value != -1 && value == unitId) {
            return;
        }
    }

    offset = 0;
    count = 0;
    for (i = 0; i < 64; i++, offset += 8) {
        if (*(s32*)((s32)queue + offset) == -1) {
            break;
        }
        count++;
    }

    if (count < 64) {
        queue[count * 2] = unitId;
        queue[count * 2 + 1] = arg;
    }
}


void BattleConsumeReserveItem(void) {
    extern void pouchRemoveItem(s32 item);
    s32 i;
    BattleWork* wp;
    s32 entryOffset;
    s32 zero;
    s32 offset;
    s32 item;

    offset = 0;
    zero = offset;
    i = 0;
    wp = _battleWorkPointer;
    do {
        entryOffset = offset + 0x19060;
        item = *(s32*)((s32)wp + entryOffset);
        if (item != 0) {
            pouchRemoveItem(item);
        }
        i++;
        *(s32*)((s32)wp + entryOffset) = zero;
        offset += 4;
    } while (i < 4);
}


void BattleIncSeq(void* battleWork, s32 seqType) {
    BattleSetSeq(battleWork, seqType, BattleGetSeq(battleWork, seqType) + 1);
}

void BattleAfterReactionQueueInit(void) {
    s32 i;
    s32 offset;
    s32* base;
    s32* entry;

    offset = 0;
    base = (s32*)((s32)_battleWorkPointer + 0x16F5C);
    for (i = 0; i < 64; i++) {
        entry = (s32*)((s32)base + offset);
        offset += 8;
        entry[0] = -1;
        entry[1] = 0;
    }
}


void BattlePartyInfoWorkInit(BattleWork* wp) {
    extern f32 float_1_80422174;
    f32 size;
    s32 type;
    s32 zero;
    BattleWorkPartyInfo* info;

    size = float_1_80422174;
    zero = 0;
    for (type = UNIT_GOOMBELLA; type < UNIT_MAX; type++) {
        info = (BattleWorkPartyInfo*)((s32)wp + 0x162A0 + ((type - UNIT_GOOMBELLA) * sizeof(BattleWorkPartyInfo)));
        info->sizeMultiplier = size;
        info->statusFlags = zero;
        memset(&info->statusEffects, zero, sizeof(BattleWorkStatus));
        info->unk28 = zero;
        info->unk2A = zero;
    }
}

s32 BattleGetSeq(BattleWork* wp, BattleSequence seq) {
    switch (seq) {
        case SEQ_UNKNOWN:
            return *(s32*)((s32)wp + 0x4);
        case SEQ_INIT:
            return *(s32*)((s32)wp + 0xF0C);
        case SEQ_FIRST_ACT:
            return *(s32*)((s32)wp + 0xF10);
        case SEQ_TURN:
            return *(s32*)((s32)wp + 0xF14);
        case SEQ_PHASE:
            return *(s32*)((s32)wp + 0xF18);
        case SEQ_MOVE:
            return *(s32*)((s32)wp + 0xF1C);
        case SEQ_ACT:
            return *(s32*)((s32)wp + 0xF20);
        case SEQ_END:
            return *(s32*)((s32)wp + 0xF24);
    }
    return 0;
}


void BattleSetSeq(BattleWork* wp, BattleSequence seq, s32 value) {
    switch (seq) {
        case SEQ_UNKNOWN:
            *(s32*)((s32)wp + 0x4) = value;
            break;
        case SEQ_INIT:
            *(s32*)((s32)wp + 0xF0C) = value;
            break;
        case SEQ_FIRST_ACT:
            *(s32*)((s32)wp + 0xF10) = value;
            break;
        case SEQ_TURN:
            *(s32*)((s32)wp + 0xF14) = value;
            break;
        case SEQ_PHASE:
            *(s32*)((s32)wp + 0xF18) = value;
            break;
        case SEQ_MOVE:
            *(s32*)((s32)wp + 0xF1C) = value;
            break;
        case SEQ_ACT:
            *(s32*)((s32)wp + 0xF20) = value;
            break;
        case SEQ_END:
            *(s32*)((s32)wp + 0xF24) = value;
            break;
    }
}


s32 BattleTransPartyIdToUnitKind(int partyId) {
    switch (partyId) {
        case 1:
            return 0xE0;
        case 2:
            return 0xE1;
        case 3:
            return 0xE2;
        case 4:
            return 0xE3;
        case 5:
            return 0xE4;
        case 6:
            return 0xE5;
        case 7:
            return 0xE6;
        default:
            return 0;
    }
}

