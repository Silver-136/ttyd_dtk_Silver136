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


void _EquipItem(void* unit, u32 param_2, u32 item) {
}


void* BattleChangeParty(void* battleWork) {
    return 0;
}


u32 BattleMajinaiCheck(void) {
    return 0;
}


s32 BattleTransPartyId(BattleUnitType type) {
    return 0;
}


s32 BattleAfterReactionMain(void) {
    return 0;
}


void BtlUnit_EquipItem(void* unit, u32 param_2, u32 item) {
}


void BattleStatusWindowCheck(void) {
}


u8 BattleAfterReactionRelease(int param_1, int param_2) {
    return 0;
}


void BattleAfterReactionEntry(s32 unitId, s32 arg) {
}


u8 BattleConsumeReserveItem(void) {
    return 0;
}


void BattleIncSeq(void* battleWork, s32 seqType) {
}


void BattleAfterReactionQueueInit(void) {
}


void BattlePartyInfoWorkInit(BattleWork* wp) {
}


s32 BattleGetSeq(BattleWork* wp, BattleSequence seq) {
    return 0;
}


void BattleSetSeq(BattleWork* wp, BattleSequence seq, s32 value) {
}


s32 BattleTransPartyIdToUnitKind(int partyId) {
    return 0;
}
