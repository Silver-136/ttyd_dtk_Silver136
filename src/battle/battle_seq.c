#include "battle/battle_seq.h"
#include "battle/battle_audience.h"
#include "manager/evtmgr.h"

extern void* _battleWorkPointer;
s32 BtlUnit_CheckStatus(void* unit, s32 status);
s32 BattleGetSeq(void* battleWork, s32 seqType);
void BattleIncSeq(void* battleWork, s32 seqType);
void BattleSetSeq(void* battleWork, s32 seqType, s32 seq);
void btlseqTurnFirstProcess(void* battleWork);
s32 BattleWaitAllActiveEvtEnd(void* battleWork);
s32 BattleCheckConcluded(void* battleWork);
s32 BattlePadCheckTrigger(s32 buttons);
void* dispEntry(s32 cameraId, s32 priority, void* callback, s32 param, f32 z);
void _rule_disp(void);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 BtlUnit_CheckRecoveryStatus(void* unit, s32 status);
void* evtEntry(void* script, s32 priority, s32 flags);
s32 psndSFXOn(const char* name);
void BtlUnit_ResetMoveStatus(void* unit);
void btlseqPhase(void* battleWork);
s32 BtlUnit_CheckShadowGuard(void* unit);
void BattleDamageDirect(s32 attackerId, void* target, s32 param3, s32 param4, s32 param5, s32 param6, s32 param7, s32 param8);
void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
s32 BtlUnit_GetHeight(void* unit);
void effFreezeN64Entry(s32 type, s32 param2, f32 x, f32 y, f32 z, f32 scale);
void psndSFXOn_3D(const char* name, f32* pos);
void BtlUnit_GetStatus(void* unit, s32 status, s8* out1, s8* out2);
void BSE_TurnFirstProcessEffectEntry(void* unit, s32 effectType);
s32 BSE_TurnFirstProcessEffectMain(void* unit);
s32 irand(s32 max);
void BtlUnit_RecoverFp(void* unit, s16 amount);
void effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z);
s32 BtlUnit_GetBodyPartsId(void* unit);
void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
void BtlUnit_GetPartsWorldPos(void* part, f32* x, f32* y, f32* z);
void BtlUnit_RecoverHp(void* unit, s16 amount);
void btlseqAct(void* battleWork);
extern u8 subsetevt_happy_heart[];
extern u8 subsetevt_happy_flower[];
extern char str_SFX_CONDITION_BREAK__802efb94[];
extern const f32 float_1_8042256c;
extern const f32 float_900_80422594;
extern u8 subsetevt_status_recovery[];
extern char str_SFX_REST_CONDITION1_802efb80[];
extern u8 subsetevt_regeneration[];
extern u8 marioDefaultWeapon_Jump[];
extern u8 marioWeapon_KururinJump[];
extern u8 marioDefaultWeapon_Hammer[];
extern u8 marioDefaultWeapon_FirstAttackKaitenHammer[];
extern u8 marioDefaultWeapon_FirstAttackUltraHammer[];
extern u8 partyWeapon_NokotarouFirstAttack[];
extern u8 partyWeapon_SandersFirstAttack[];

/*
 * Lucky Start's status-only BattleWeapon.  Keep this as a byte array so
 * btlseqInit's original byte-based common-base addressing remains unchanged.
 */
__declspec(section ".rodata") u8 N_weapon_lucky_start[192] __attribute__((aligned(4))) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x08,
    0x00, 0x00, 0x07, 0xFF, 0x80, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x03, 0x64, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x02, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const char str_stg3_tou_cp_001_802ef8f8[16] = "stg3_tou_cp_001";
const char str_stg3_tou_cp_002_802ef908[16] = "stg3_tou_cp_002";
const char str_stg3_tou_cp_004_802ef918[16] = "stg3_tou_cp_004";
const char str_stg3_tou_cp_008_802ef928[16] = "stg3_tou_cp_008";
const char str_stg3_tou_cp_000_802ef938[16] = "stg3_tou_cp_000";
const char str_stg3_tou_cp_005_802ef948[16] = "stg3_tou_cp_005";
const char str_stg3_tou_cp_006_802ef958[16] = "stg3_tou_cp_006";
const char str_stg3_tou_cp_009_802ef968[16] = "stg3_tou_cp_009";
const char str_stg3_tou_cp_003_802ef978[16] = "stg3_tou_cp_003";
const char str_stg3_tou_cp_007_802ef988[16] = "stg3_tou_cp_007";
const char str_stg3_tou_cp_011_802ef998[16] = "stg3_tou_cp_011";
const char str_stg3_tou_cp_012_802ef9a8[16] = "stg3_tou_cp_012";
const char str_stg3_tou_cp_014_802ef9b8[16] = "stg3_tou_cp_014";
const char str_stg3_tou_cp_019_802ef9c8[16] = "stg3_tou_cp_019";
const char str_stg3_tou_cp_020_802ef9d8[16] = "stg3_tou_cp_020";
const char str_stg3_tou_cp_010_802ef9e8[16] = "stg3_tou_cp_010";
const char str_stg3_tou_cp_018_802ef9f8[16] = "stg3_tou_cp_018";
const char str_stg3_tou_cp_015_802efa08[16] = "stg3_tou_cp_015";
const char str_stg3_tou_cp_016_802efa18[16] = "stg3_tou_cp_016";
const char str_stg3_tou_cp_021_802efa28[16] = "stg3_tou_cp_021";
const char str_stg3_tou_cp_013_802efa38[16] = "stg3_tou_cp_013";
const char str_stg3_tou_cp_017_802efa48[16] = "stg3_tou_cp_017";
const char str_stg3_tou_cp_024_802efa58[16] = "stg3_tou_cp_024";
const char str_stg3_tou_cp_023_802efa68[16] = "stg3_tou_cp_023";
const char str_stg3_tou_cp_022_802efa78[16] = "stg3_tou_cp_022";
const char str_stg3_tou_cp_041_802efa88[16] = "stg3_tou_cp_041";
const char str_stg3_tou_cp_042_802efa98[16] = "stg3_tou_cp_042";
const char str_stg3_tou_cp_044_802efaa8[16] = "stg3_tou_cp_044";
const char str_stg3_tou_cp_049_802efab8[16] = "stg3_tou_cp_049";
const char str_stg3_tou_cp_050_802efac8[16] = "stg3_tou_cp_050";
const char str_stg3_tou_cp_040_802efad8[16] = "stg3_tou_cp_040";
const char str_stg3_tou_cp_048_802efae8[16] = "stg3_tou_cp_048";
const char str_stg3_tou_cp_045_802efaf8[16] = "stg3_tou_cp_045";
const char str_stg3_tou_cp_046_802efb08[16] = "stg3_tou_cp_046";
const char str_stg3_tou_cp_051_802efb18[16] = "stg3_tou_cp_051";
const char str_stg3_tou_cp_043_802efb28[16] = "stg3_tou_cp_043";
const char str_stg3_tou_cp_047_802efb38[16] = "stg3_tou_cp_047";
const char str_stg3_tou_cp_054_802efb48[16] = "stg3_tou_cp_054";
const char str_stg3_tou_cp_053_802efb58[16] = "stg3_tou_cp_053";
const char str_stg3_tou_cp_052_802efb68[16] = "stg3_tou_cp_052";


#define BTLSEQ_STATUS_RECOVERY_RECURSE(statusValue) \
do { \
    void* script; \
    void* unit; \
    void* evt; \
    s32 i; \
    s32 result; \
\
    script = subsetevt_status_recovery; \
    i = 0; \
\
    do { \
        unit = BattleGetUnitPtr(battleWork, i); \
        result = BtlUnit_CheckRecoveryStatus(unit, statusValue); \
        anyRecovered |= result; \
\
        if (result != 0) { \
            evt = evtEntry(script, 10, 0); \
            if (evt != 0) { \
                *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C); \
                *(s32*)((s32)evt + 0x160) = *(s32*)unit; \
            } \
            anyEvent = 1; \
        } \
\
        i++; \
    } while (i < 0x40); \
\
    if (anyRecovered != 0) { \
        psndSFXOn(str_SFX_REST_CONDITION1_802efb80); \
    } \
\
    if (anyEvent == 0) { \
        BattleIncSeq(battleWork, 3); \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } else { \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } \
} while (0)

#define BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(statusValue) \
do { \
    void* unit; \
    void* evt; \
    s32 i; \
    s32 result; \
\
    i = 0; \
    do { \
        unit = BattleGetUnitPtr(battleWork, i); \
        if (unit != 0) { \
            result = BtlUnit_CheckRecoveryStatus(unit, statusValue); \
            anyRecovered |= result; \
\
            if (result != 0) { \
                evt = evtEntry(subsetevt_status_recovery, 10, 0); \
                if (evt != 0) { \
                    *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C); \
                    *(s32*)((s32)evt + 0x160) = *(s32*)unit; \
                } \
                anyEvent = 1; \
            } \
        } \
        i++; \
    } while (i < 0x40); \
\
    if (anyRecovered != 0) { \
        psndSFXOn(str_SFX_REST_CONDITION1_802efb80); \
    } \
\
    if (anyEvent == 0) { \
        BattleIncSeq(battleWork, 3); \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } else { \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } \
} while (0)

#define BTLSEQ_STATUS_RECOVERY_NULL_RECURSE_RESET(statusValue) \
do { \
    void* unit; \
    void* evt; \
    s32 i; \
    s32 result; \
\
    i = 0; \
    do { \
        unit = BattleGetUnitPtr(battleWork, i); \
        if (unit != 0) { \
            result = BtlUnit_CheckRecoveryStatus(unit, statusValue); \
            anyRecovered |= result; \
\
            if (result != 0) { \
                BtlUnit_ResetMoveStatus(unit); \
                evt = evtEntry(subsetevt_status_recovery, 10, 0); \
                if (evt != 0) { \
                    *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C); \
                    *(s32*)((s32)evt + 0x160) = *(s32*)unit; \
                } \
                anyEvent = 1; \
            } \
        } \
        i++; \
    } while (i < 0x40); \
\
    if (anyRecovered != 0) { \
        psndSFXOn(str_SFX_REST_CONDITION1_802efb80); \
    } \
\
    if (anyEvent == 0) { \
        BattleIncSeq(battleWork, 3); \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } else { \
        BattleIncSeq(battleWork, 3); \
        btlseqTurn(battleWork); \
    } \
} while (0)

void BattleSequenceManager(void) {
    extern void* _battleWorkPointer;
    extern s32 BattleGetSeq(void*, s32);
    extern void BattleCheckAllPinchStatus(void*, s32);
    extern void btlseqInit(void*);
    extern void btlseqFirstAct(void*);
    extern void btlseqTurn(void*);
    extern void btlseqEnd(void*);
    void* battleWork;
    s32 sequence;

    battleWork = _battleWorkPointer;
    switch (BattleGetSeq(battleWork, 0)) {
        case 0:
            btlseqInit(battleWork);
            BattleCheckAllPinchStatus(battleWork, 1);
            break;
        case 1:
            BattleCheckAllPinchStatus(battleWork, 0);
            btlseqFirstAct(battleWork);
            break;
        case 2:
            BattleCheckAllPinchStatus(battleWork, 0);
            btlseqTurn(battleWork);
            break;
        case 3:
            btlseqEnd(battleWork);
            break;
    }
}

void BattleCheckAllPinchStatus(void* battleWork, s32 firstCheck) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_CheckPinchStatus(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern s32 BtlUnit_GetBelong(void*);
    extern void* BattleGetMarioPtr(void*);
    extern s32 psndSFXOn(const char*);
    extern char str_SFX_BTL_DANGER1_802efc04[];
    extern char str_SFX_BTL_PINCH1_802efc14[];
    void* unit;
    s32 i;
    s32 kind;
    s32 found;

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            BtlUnit_CheckPinchStatus(unit, firstCheck);
        }
    }

    found = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && (*(u32*)((s32)unit + 0x138) & 0x20000000) != 0 &&
            BtlUnit_CheckStatus(unit, 0x1B) == 0 && BtlUnit_GetBelong(unit) == 0) {
            kind = *(s32*)((s32)unit + 8);
            if ((kind == 0xDE || (kind > 0xDF && kind < 0xE7)) &&
                *(s16*)((s32)unit + 0x10C) > 0) {
                found = 1;
                break;
            }
        }
    }

    if (found) {
        (*(s8*)((s32)battleWork + 0x18FF8))--;
        if (*(s8*)((s32)battleWork + 0x18FF8) < 1) {
            *(s8*)((s32)battleWork + 0x18FF8) = 60;
            if ((*(u32*)((s32)battleWork + 0x1720) & 1) != 0) {
                psndSFXOn(str_SFX_BTL_DANGER1_802efc04);
            }
        }
    } else {
        found = 0;
        for (i = 0; i < 0x40; i++) {
            unit = BattleGetUnitPtr(battleWork, i);
            if (unit != 0 && (*(u32*)((s32)unit + 0x138) & 0x10000000) != 0 &&
                BtlUnit_CheckStatus(unit, 0x1B) == 0 && BtlUnit_GetBelong(unit) == 0) {
                kind = *(s32*)((s32)unit + 8);
                if ((kind == 0xDE || (kind > 0xDF && kind < 0xE7)) &&
                    *(s16*)((s32)unit + 0x10C) > 0) {
                    found = 1;
                    break;
                }
            }
        }

        if (found) {
            (*(s8*)((s32)battleWork + 0x18FF8))--;
            if (*(s8*)((s32)battleWork + 0x18FF8) < 1) {
                *(s8*)((s32)battleWork + 0x18FF8) = 60;
                if ((*(u32*)((s32)battleWork + 0x1720) & 1) != 0) {
                    psndSFXOn(str_SFX_BTL_PINCH1_802efc14);
                }
            }
        } else {
            unit = BattleGetMarioPtr(battleWork);
            if (unit != 0) {
                *(s32*)((s32)unit + 0x2D0) = 0;
            }
            *(s8*)((s32)battleWork + 0x18FF8) = 0;
        }
    }
}

void btlseqInit(void* battleWork) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void Btl_UnitSetup(void*);
    extern s32 BattleCheckEndUnitInitEvent(void*);
    extern s32 animEffectAsync(void*, s32);
    extern s32 effNiceAsync(s32);
    extern void BattleStatusWindowEventOn(void);
    extern void BattleStatusWindowSystemOn(void);
    void* ptr;
    u8* commonBase = N_weapon_lucky_start;

    switch (BattleGetSeq(battleWork, 1)) {
        case 0x1000000:
            BattleSetSeq(battleWork, 1, 0x1000001);
        case 0x1000001:
            ptr = marioGetPtr();
            *(u32*)((s32)ptr + 4) |= 1;
            ptr = partyGetPtr(marioGetPartyId());
            if (ptr != 0) {
                *(u32*)((s32)ptr + 4) |= 1;
            }
            Btl_UnitSetup(battleWork);
            BattleSetSeq(battleWork, 1, 0x1000002);
            break;
        case 0x1000002:
            if (BattleCheckEndUnitInitEvent(battleWork) != 0) {
                BattleSetSeq(battleWork, 1, 0x1000003);
            }
            break;
        case 0x1000003:
            if (animEffectAsync(commonBase + 0x3AC, 1) != 0 &&
                animEffectAsync(commonBase + 0x3B4, 1) != 0 &&
                animEffectAsync(commonBase + 0x3C0, 1) != 0 &&
                effNiceAsync(1) != 0) {
                BattleSetSeq(battleWork, 1, 0x1000004);
            }
            break;
        case 0x1000004:
            BattleSetSeq(battleWork, 1, 0x1000005);
            break;
        case 0x1000005:
            BattleStatusWindowEventOn();
            BattleStatusWindowSystemOn();
            BattleSetSeq(battleWork, 0, 1);
            BattleSetSeq(battleWork, 2, 0x2000000);
            break;
    }
}

void* _GetFirstAttackWeapon(u32 firstAttackType) {
    switch (firstAttackType) {
        case 0:
            return marioDefaultWeapon_Jump;
        case 1:
            return marioWeapon_KururinJump;
        case 2:
            return marioDefaultWeapon_Jump;
        case 3:
            return marioDefaultWeapon_Hammer;
        case 4:
            return marioDefaultWeapon_FirstAttackKaitenHammer;
        case 5:
            return marioDefaultWeapon_FirstAttackUltraHammer;
        case 6:
            return partyWeapon_NokotarouFirstAttack;
        case 7:
            return partyWeapon_SandersFirstAttack;
        default:
            return 0;
    }
}

void btlseqFirstAct(void* battleWork) {
    extern s32 fadeIsFinish(void);
    extern void BattleStatusWindowEventOn(void);
    extern void BattleStatusWindowSystemOn(void);
    extern void BattleConsumeReserveItem(void);
    extern void BattleAudience_PerAct(void);
    extern u32 BattleAudience_CheckReaction(void);
    extern u32 BattleBreakSlot_CheckReaction(void);
    extern s32 BattleStatusChangeMsgMain(void*);
    extern s32 BattleStatusChangeAnnouceMain(void*);
    extern void BattleStatusChangeMsgAdjust(void*);
    extern void BattleStage_WallCloseCheck(void);
    extern s32 evtCheckID(s32);
    extern void BattleStage_NozzleWorkCheck(s32);
    extern void BattleGetFirstAttackUnit(void*, void**, void**);
    extern void* _GetFirstAttackWeapon(u32);
    extern void BattleStatusWindowEventOff(void);
    extern u32 BtlUnit_GetBelong(void*);
    extern void BattleAudience_Case_FirstAttack_Good(void);
    extern void BattleAudience_Case_FirstAttack_Bad(void);
    extern void* BattleGetPartyPtr(void*);
    extern void* BattleGetMarioPtr(void*);
    extern u32 _default_haikei_set_entry_event[];
    extern void* marioGetPtr(void);
    extern void* BtlUnit_GetData(void*, s32);
    extern void fadeEntry(s32, s32, void*);
    extern void psndSFXOff(s32);
    extern const u32 dat_80422550;
    extern char str_SFX_OFF_BTL_START_CU_802efbc4[];
    extern u32 nanika_luck_event[];
    u32 color;
    void* npcInfo;
    s32 firstAttack;
    s32 seq;
    void* unit;
    void* loopUnit;
    void* target;
    void* event;
    void* weapon;
    s32 i;
    s32 messageBusy;
    s32 announceBusy;

    firstAttack = *(s32*)(*(s32*)((s32)battleWork + 0x2738) + 8);
    seq = BattleGetSeq(battleWork, 2);
    switch (seq) {
        case 0x2000000:
            if (firstAttack == 0) {
                *(s32*)((s32)battleWork + 0xEFC) = 0;
                evtEntry(_default_haikei_set_entry_event, 10, 0);
                npcInfo = *(void**)(*(s32*)((s32)battleWork + 0x2738) + 0xC);
                for (i = 0; i < 64; i++) {
                    loopUnit = BattleGetUnitPtr(battleWork, i);
                    if (loopUnit != NULL && *(void**)((s32)loopUnit + 0x2B8) != NULL &&
                        ((*(u32*)((s32)npcInfo + 4) & 0x100000) == 0 ||
                         (*(s32*)((s32)loopUnit + 8) != 0xDE &&
                          (*(s32*)((s32)loopUnit + 8) < 0xE0 || *(s32*)((s32)loopUnit + 8) >= 0xE7)))) {
                        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1A &&
                            *(s32*)((s32)loopUnit + 8) == 0xDE) {
                            event = evtEntry(BtlUnit_GetData(loopUnit, 0x41), 10, 0);
                        } else {
                            event = evtEntry(*(void**)((s32)loopUnit + 0x2B8), 10, 0);
                        }
                        *(s32*)((s32)loopUnit + 0x2BC) = *(s32*)((s32)event + 0x15C);
                        *(s32*)((s32)event + 0x160) = *(s32*)loopUnit;
                    }
                }
                BattleSetSeq(battleWork, 2, 0x2000001);
                return;
            }
            if (firstAttack < 4) {
                if (firstAttack >= 1) {
                *(s32*)((s32)battleWork + 0xEFC) = 10;
                } else {
                    *(s32*)((s32)battleWork + 0xEFC) = 0;
                }
            } else {
                *(s32*)((s32)battleWork + 0xEFC) = 0;
            }
            *(u32*)((s32)battleWork + 0xEF4) |= 2;
            BattleGetFirstAttackUnit(battleWork, &unit, &target);
            *(s32*)((s32)battleWork + 0x420) = *(s32*)unit;
            *(u32*)((s32)unit + 0x27C) |= 0x1000;
            event = evtEntry(*(void**)((s32)target + 0x2B0), 10, 0x20);
            if (event != NULL) {
                *(s32*)((s32)event + 0x160) = *(s32*)target;
                *(s32*)((s32)target + 0x278) = 0x39;
                *(s32*)((s32)target + 0x2B4) = *(s32*)((s32)event + 0x15C);
            }
            *(u32*)((s32)unit + 0x1C) |= 0x04000000;
            if (firstAttack < 9) {
                weapon = _GetFirstAttackWeapon(firstAttack);
                *(void**)((s32)unit + 0x2A0) = *(void**)((s32)weapon + 0xB0);
                *(s32*)((s32)unit + 0x2AC) = 0;
                BattleStatusWindowEventOff();
            }
            if ((s8)BtlUnit_GetBelong(unit) == 0) {
                BattleAudience_Case_FirstAttack_Good();
            } else {
                BattleAudience_Case_FirstAttack_Bad();
            }
            event = evtEntry(*(void**)((s32)unit + 0x2A0), 10, 0);
            if (event != NULL) {
                *(s32*)((s32)event + 0x160) = *(s32*)unit;
                *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)event + 0x15C);
            }
            if (firstAttack < 7) {
                if (firstAttack > 0) {
                    target = BattleGetPartyPtr(battleWork);
                    if (target != NULL) {
                        event = evtEntry(*(void**)((s32)target + 0x2B8), 10, 0);
                        *(s32*)((s32)target + 0x2BC) = *(s32*)((s32)event + 0x15C);
                        *(s32*)((s32)event + 0x160) = *(s32*)target;
                    }
                }
            } else if (firstAttack < 9) {
                target = BattleGetMarioPtr(battleWork);
                if (target != NULL) {
                    event = evtEntry(*(void**)((s32)target + 0x2B8), 10, 0);
                    *(s32*)((s32)target + 0x2BC) = *(s32*)((s32)event + 0x15C);
                    *(s32*)((s32)event + 0x160) = *(s32*)target;
                }
            }
            BattleSetSeq(battleWork, 2, 0x2000001);
            break;
        case 0x2000001:
            (*(s32*)((s32)battleWork + 0xEFC))--;
            if (*(s32*)((s32)battleWork + 0xEFC) > 0) {
                return;
            }
            color = dat_80422550;
            fadeEntry(0x17, 200, &color);
            *(s32*)((s32)battleWork + 0x19070) = psndSFXOn(str_SFX_OFF_BTL_START_CU_802efbc4);
            BattleSetSeq(battleWork, 2, 0x2000002);
        case 0x2000002:
            if (fadeIsFinish() && *(s32*)((s32)battleWork + 0x19070) != -1) {
                psndSFXOff(*(s32*)((s32)battleWork + 0x19070));
                *(s32*)((s32)battleWork + 0x19070) = -1;
            }
            if (BattleWaitAllActiveEvtEnd(battleWork)) {
                BattleStatusWindowEventOn();
            }
            if (BattleWaitAllActiveEvtEnd(battleWork) && fadeIsFinish()) {
                BattleSetSeq(battleWork, 2, 0x2000003);
                unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
                if (unit != NULL) {
                    *(u32*)((s32)unit + 0x1C) &= 0xFBFFFFFF;
                }
            }
            break;
        case 0x2000003:
            if (BattleCheckConcluded(battleWork)) {
                BattleAudience_PerAct();
                BattleSetSeq(battleWork, 0, 3);
                BattleSetSeq(battleWork, 7, 0x7000000);
                return;
            }
            BattleSetSeq(battleWork, 2, 0x2000004);
            BattleStatusChangeMsgAdjust(battleWork);
        case 0x2000004:
            if (!BattleWaitAllActiveEvtEnd(battleWork)) return;
            messageBusy = BattleStatusChangeMsgMain(battleWork);
            announceBusy = BattleStatusChangeAnnouceMain(battleWork);
            if ((messageBusy | announceBusy) != 0) return;
            BattleIncSeq(battleWork, 2);
        case 0x2000005:
            if ((*(u32*)((s32)battleWork + 0xEF4) & 0x30) == 0) {
                BattleStatusWindowSystemOn();
            }
            BattleConsumeReserveItem();
            *(u32*)((s32)battleWork + 0xEF4) &= ~4;
            BattleAudience_PerAct();
            BattleIncSeq(battleWork, 2);
        case 0x2000006:
            if ((BattleAudience_CheckReaction() & 0xFF) == 0 &&
                (BattleCheckConcluded(battleWork) ||
                 (BattleBreakSlot_CheckReaction() & 0xFF) == 0)) {
                BattleIncSeq(battleWork, 2);
            } else {
                break;
            }
        case 0x2000007:
            messageBusy = BattleStatusChangeMsgMain(battleWork);
            announceBusy = BattleStatusChangeAnnouceMain(battleWork);
            if ((messageBusy | announceBusy) == 0) {
                BattleIncSeq(battleWork, 2);
            } else {
                break;
            }
        case 0x2000008:
            if (BattleWaitAllActiveEvtEnd(battleWork)) {
                if ((*(u32*)(*(s32*)(*(s32*)((s32)battleWork + 0x2738) + 0xC) + 4) & 0x10) != 0) {
                    BattleSetSeq(battleWork, 2, 0x2000009);
                    BattleStage_WallCloseCheck();
                } else {
                    BattleSetSeq(battleWork, 2, 0x200000A);
                }
            }
            break;
        case 0x2000009:
            if (*(s32*)((s32)battleWork + 0x182C0) != 0 &&
                !evtCheckID(*(s32*)((s32)battleWork + 0x182C0))) {
                *(s32*)((s32)battleWork + 0x182C0) = 0;
            }
            if (*(s32*)((s32)battleWork + 0x182C0) == 0) {
                BattleSetSeq(battleWork, 2, 0x200000A);
            }
            break;
        case 0x200000A:
            *(s32*)((s32)battleWork + 0x1905C) = 0;
            messageBusy = 0;
            for (i = 0; i < 64; i++) {
                loopUnit = BattleGetUnitPtr(battleWork, i);
                if (loopUnit != NULL && *(u8*)((s32)loopUnit + 0x304) != 0 && BtlUnit_CheckStatus(loopUnit, 0x1B) == 0) {
                    messageBusy = 1;
                }
            }
            if (messageBusy) {
                event = evtEntry(nanika_luck_event, 10, 0);
                *(s32*)((s32)event + 0x160) = -5;
                *(s32*)((s32)battleWork + 0x1905C) = *(s32*)((s32)event + 0x15C);
                *(u32*)((s32)battleWork + 0x18FF4) |= -1;
                *(u32*)((s32)battleWork + 0x18FF0) |= -1;
                BattleStatusChangeMsgAdjust(battleWork);
                BattleSetSeq(battleWork, 2, 0x200000B);
                *(u16*)((s32)battleWork + 0x18FFA) = 0;
            } else {
                BattleSetSeq(battleWork, 2, 0x200000C);
            }
            break;
        case 0x200000B:
            messageBusy = 1;
            if (*(u16*)((s32)battleWork + 0x18FFA) < 300) {
                (*(u16*)((s32)battleWork + 0x18FFA))++;
            }
            if (*(s32*)((s32)battleWork + 0x1905C) != 0) {
                if (evtCheckID(*(s32*)((s32)battleWork + 0x1905C))) {
                    messageBusy = 0;
                } else {
                    *(s32*)((s32)battleWork + 0x1905C) = 0;
                }
            }
            if (!BattleWaitAllActiveEvtEnd(battleWork)) {
                messageBusy = 0;
            }
            if (BattleStatusChangeAnnouceMain(battleWork) != 0) {
                messageBusy = 0;
            }
            if (BattleStatusChangeMsgMain(battleWork) != 0) {
                messageBusy = 0;
            }
            if (messageBusy) {
                BattleSetSeq(battleWork, 2, 0x200000C);
            }
            break;
        case 0x200000C:
            if ((*(u32*)((s32)battleWork + 0xEF4) & 2) != 0) {
                BattleSetSeq(battleWork, 2, 0x200000E);
                return;
            }
            BattleStage_NozzleWorkCheck(0);
            BattleSetSeq(battleWork, 2, 0x200000D);
        case 0x200000D:
            if (BattleWaitAllActiveEvtEnd(battleWork)) {
                BattleSetSeq(battleWork, 2, 0x200000E);
            } else {
                break;
            }
        case 0x200000E:
            *(u32*)((s32)battleWork + 0xEF4) &= ~2;
            BattleSetSeq(battleWork, 0, 2);
            BattleSetSeq(battleWork, 3, 0x3000000);
            break;
    }
}

s32 _set_effect_luck(void) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern s32 irand(s32);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern void* BattleGetSystemPtr(void*);
    extern s32 BattleSetStatusDamageFromWeapon(void*, void*, void*, void*, u32);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern s32 psndSFXOn(const char*);
    extern void* effNiceEntry(f32, f32, f32, s32);
    extern char str_SFX_SYSTEM_LUCKY1_802efbb0[];
    typedef struct { u32 a; u32 b; } CopyPair;
    struct { f32 x, y, z; } position;
    u8 weapon[0xC0];
    u32* source;
    u32* destination;
    u8* weaponData;
    const char* sound;
    void* battleWork;
    void* unit;
    void* part;
    void* effect;
    s32 random;
    s32 partId;
    s32 i;

    battleWork = _battleWorkPointer;
    source = (u32*)N_weapon_lucky_start - 1;
    destination = (u32*)weapon - 1;
    for (i = 0; i < 0x18; i++) {
        u32 first = source[1];
        u32 second = *(source += 2);
        destination[1] = first;
        *(destination += 2) = second;
    }
    weaponData = weapon;
    sound = str_SFX_SYSTEM_LUCKY1_802efbb0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit == 0 || *(u8*)((s32)unit + 0x304) == 0 ||
            BtlUnit_CheckStatus(unit, 0x1B) != 0) {
            continue;
        }
        random = irand(4);
        switch (random) {
            case 0:
                weapon[0x8D] = 0;
                weapon[0x8E] = 0;
                weapon[0xAA] = 0;
                weapon[0xAB] = 0;
                weapon[0xAC] = 0;
                weapon[0xAD] = 0;
                break;
            case 1:
                weapon[0x8B] = 0;
                weapon[0x8C] = 0;
                weapon[0xAA] = 0;
                weapon[0xAB] = 0;
                weapon[0xAC] = 0;
                weapon[0xAD] = 0;
                break;
            case 2:
                weapon[0x8B] = 0;
                weapon[0x8C] = 0;
                weapon[0x8D] = 0;
                weapon[0x8E] = 0;
                weapon[0xAC] = 0;
                weapon[0xAD] = 0;
                break;
            default:
                weapon[0x8B] = 0;
                weapon[0x8C] = 0;
                weapon[0x8D] = 0;
                weapon[0x8E] = 0;
                weapon[0xAA] = 0;
                weapon[0xAB] = 0;
                break;
        }
        partId = BtlUnit_GetBodyPartsId(unit);
        part = BtlUnit_GetPartsPtr(unit, partId);
        BattleSetStatusDamageFromWeapon(BattleGetSystemPtr(battleWork),
                                        unit, part, weaponData, 0x100);
        BtlUnit_GetPos(unit, &position.x, &position.y, &position.z);
        psndSFXOn(sound);
        effect = effNiceEntry(position.x - 45.0f, position.y + 60.0f, position.z, 6);
        *(f32*)(*(s32*)((s32)effect + 0xC) + 0x1C) = 0.75f;
    }
ordinal_pad_1:
ordinal_pad_2:
ordinal_pad_3:
ordinal_pad_4:
ordinal_pad_5:
    return 2;
}

void btlseqTurn(void* battleWork) {
    s32 seq;
    s32 anyEvent;
    s32 anyRecovered;

    anyEvent = 0;
    anyRecovered = 0;

    seq = BattleGetSeq(battleWork, 3);

    switch (seq) {
        case 0x3000000:
            BattleIncSeq(battleWork, 3);
            break;

        case 0x3000001:
            btlseqTurnFirstProcess(battleWork);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

         case 0x3000003:
            if (*(s16*)((s32)battleWork + 0x0) != 1) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                void* info = *(void**)((s32)battleWork + 0x2738);

                if (*(u8*)((s32)info + 0x18) == 0) {
                    BattleIncSeq(battleWork, 3);
                    BattleIncSeq(battleWork, 3);
                    btlseqTurn(battleWork);
                } else {
                    *(s32*)((s32)battleWork + 0xEFC) = 0x78;
                    BattleIncSeq(battleWork, 3);
                    btlseqTurn(battleWork);
                }
            }
            break;

        case 0x3000004:
            if (*(s32*)((s32)battleWork + 0xEFC) != 0) {
                *(s32*)((s32)battleWork + 0xEFC) = *(s32*)((s32)battleWork + 0xEFC) - 1;
            }

            if (*(s32*)((s32)battleWork + 0xEFC) <= 0 || BattlePadCheckTrigger(0x300) != 0) {
                    BattleIncSeq(battleWork, 3);
                } else {
                  void* callback;
                  f32 z;

                 callback = _rule_disp;
                 z = float_900_80422594;
                dispEntry(8, 1, callback, 0, z);
                }
                break;

        case 0x3000002:
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x3000005:
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x3000007:
        case 0x300000B:
        case 0x300000D:
        case 0x300000F:
        case 0x3000011:
        case 0x3000013:
        case 0x3000015:
        case 0x3000017:
        case 0x3000019:
        case 0x300001B:
        case 0x300001D:
        case 0x300001F:
        case 0x3000021:
        case 0x3000023:
        case 0x3000025:
        case 0x3000027:
        case 0x3000029:
        case 0x300002B:
        case 0x300002D:
        case 0x300002F:
        case 0x3000031:
        case 0x3000033:
        case 0x3000035:
        case 0x3000037:
        case 0x3000039:
        case 0x300003B:
        case 0x300003D:
        case 0x300003F:
        case 0x3000041:
        case 0x3000043:
        case 0x3000045:
        case 0x3000047:
        case 0x3000049:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                if (BattleCheckConcluded(battleWork) != 0) {
                    BattleSetSeq(battleWork, 0, 3);
                    BattleSetSeq(battleWork, 7, 0x7000000);
                } else {
                    BattleIncSeq(battleWork, 3);
                }
            }
            break;

        case 0x3000012: {
            void* script;
            void* unit;
            void* evt;
            s32 i;
            s32 result;

            script = subsetevt_status_recovery;
            i = 0;

            do {
                unit = BattleGetUnitPtr(battleWork, i);
                result = BtlUnit_CheckRecoveryStatus(unit, 1);
                anyRecovered |= result;

                if (result != 0) {
                    evt = evtEntry(script, 10, 0);
                    if (evt != 0) {
                        *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
                        *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                    }
                    anyEvent = 1;
                }

                i++;
            } while (i < 0x40);

            if (anyRecovered != 0) {
                psndSFXOn(str_SFX_REST_CONDITION1_802efb80);
            }

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                BattleIncSeq(battleWork, 3);
            }
            break;
        }
        case 0x3000014:
            BTLSEQ_STATUS_RECOVERY_RECURSE(2);
            break;

        case 0x3000016:
            BTLSEQ_STATUS_RECOVERY_RECURSE(3);
            break;

        case 0x3000018:
            BTLSEQ_STATUS_RECOVERY_RECURSE(5);
            break;

        case 0x300001A:
            BTLSEQ_STATUS_RECOVERY_RECURSE(6);
            break;

        case 0x300001C:
            BTLSEQ_STATUS_RECOVERY_RECURSE(7);
            break;

        case 0x300001E:
            BTLSEQ_STATUS_RECOVERY_RECURSE(8);
            break;

        case 0x3000020: {
            void* unit;
            s32 i;
            f32 x;
            f32 y;
            f32 z;
            const char* sfxName;
            f32* yPtr;
            f32* zPtr;
            sfxName = str_SFX_CONDITION_BREAK__802efb94;
            zPtr = &z;
            yPtr = &y;
            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (BtlUnit_CheckRecoveryStatus(unit, 9) != 0) {
                    if (BtlUnit_CheckShadowGuard(unit) == 0) {
                        BattleDamageDirect(-5, unit, 0, 1, 0, 0x11A, 0, 1);
                    }

                    BtlUnit_GetPos(unit, &x, yPtr, zPtr);
                    y += BtlUnit_GetHeight(unit);

                    effFreezeN64Entry(0, 1, x, y, z, float_1_8042256c);
                    psndSFXOn_3D(sfxName, &x);

                    anyEvent = 1;
                }

                i++;
            } while (i < 0x40);

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            }
            break;
        }

        case 0x3000022:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xA);
            break;

        case 0x3000024:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xB);
            break;

        case 0x3000026:
            BTLSEQ_STATUS_RECOVERY_RECURSE(4);
            break;

        case 0x3000028:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xC);
            break;

        case 0x300002A:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xD);
            break;

        case 0x300002C:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xE);
            break;

        case 0x300002E:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0xF);
            break;

        case 0x3000030:
            BTLSEQ_STATUS_RECOVERY_RECURSE(0);
            break;
        case 0x3000032:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x17);
            break;

        case 0x3000034:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x18);
            break;

        case 0x3000036:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x12);
            break;

        case 0x3000038:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x13);
            break;

        case 0x300003A:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE_RESET(0x14);
            break;

        case 0x300003C:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x15);
            break;

        case 0x300003E:
            BTLSEQ_STATUS_RECOVERY_NULL_RECURSE(0x16);
            break;

        case 0x3000006: {
            void* unit;
            s32 i;
            s8 unusedStatus;
            s8 amount;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0) {
                    BtlUnit_GetStatus(unit, 4, &unusedStatus, &amount);

                    if (amount > 0 && BtlUnit_CheckShadowGuard(unit) == 0) {
                        anyEvent = 1;
                        BattleDamageDirect(-5, unit, 0, amount, 0, 0x117, 0, 1);
                    }
                }

                i++;
            } while (i < 0x40);

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            }
            break;
        }

        case 0x3000008: {
            void* unit;
            s32 i;
            s8 unusedStatus;
            s8 amount;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0) {
                    BtlUnit_GetStatus(unit, 8, &unusedStatus, &amount);

                    if (amount > 0) {
                        anyEvent = 1;
                        break;
                    }
                }

                i++;
            } while (i < 0x40);

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                *(s16*)((s32)battleWork + 0x2) = 0;
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            }
            break;
        }

                        case 0x3000009: {
            void* unit;
            s32 i;
            s32 done;
            u16 state;
            s8 unusedStatus;
            s8 amount;

            state = *(u16*)((s32)battleWork + 0x2);

            switch (state) {
                case 1:
                    break;

                case 0:
                    *(u16*)((s32)battleWork + 0x2) = state + 1;

                    i = 0;
                    do {
                        unit = BattleGetUnitPtr(battleWork, i);

                        if (unit != 0 &&
                            BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                            *(s16*)((s32)unit + 0x10C) > 0) {
                            BtlUnit_GetStatus(unit, 8, &unusedStatus, &amount);

                            if (amount > 0) {
                                BSE_TurnFirstProcessEffectEntry(unit, 1);
                            }
                        }

                        i++;
                    } while (i < 0x40);
                    break;

                default:
                    if (BattleWaitAllActiveEvtEnd(battleWork) == 0) {
                        return;
                    }

                    if (BattleCheckConcluded(battleWork) != 0) {
                        BattleSetSeq(battleWork, 0, 3);
                        BattleSetSeq(battleWork, 7, 0x7000000);
                        return;
                    }

                    BattleIncSeq(battleWork, 3);
                    return;
            }

            done = 1;
            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 && BSE_TurnFirstProcessEffectMain(unit) != 0) {
                    done = 0;
                }

                i++;
            } while (i < 0x40);

            if (done != 0) {
                *(u16*)((s32)battleWork + 0x2) = *(u16*)((s32)battleWork + 0x2) + 1;
            }
            break;
        }

        case 0x300000A: {
            void* unit;
            void* part;
            void* evt;
            s32 i;
            s32 partId;
            s32 height;
            s8 unusedStatus;
            s8 amount;
            f32 x;
            f32 y;
            f32 z;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0) {
                    BtlUnit_GetStatus(unit, 0x17, &unusedStatus, &amount);

                    if (amount > 0) {
                        if ((*(u32*)((s32)unit + 0x104) & 0x40000000) == 0) {
                            *(s32*)((s32)unit + 0x208) = *(s16*)((s32)unit + 0x10C);

                            BtlUnit_RecoverHp(unit, amount);

                            *(s32*)((s32)unit + 0x20C) = *(s16*)((s32)unit + 0x10C);
                            *(s16*)((s32)unit + 0x200) = *(s16*)((s32)unit + 0x202);
                            *(s16*)((s32)unit + 0x204) = *(s16*)((s32)unit + 0x206);
                            *(u16*)((s32)unit + 0x1FE) |= 1;
                        } else {
                            BtlUnit_RecoverHp(unit, amount);
                        }

                        partId = BtlUnit_GetBodyPartsId(unit);
                        part = BtlUnit_GetPartsPtr(unit, partId);
                        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);

                        height = BtlUnit_GetHeight(unit) + 5;
                        y += *(f32*)((s32)unit + 0x114) * height;

                        effRecoveryEntry(0, amount, x, y, z);

                        evt = evtEntry(subsetevt_regeneration, 10, 0);
                        if (evt != 0) {
                            *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
                            *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                        }

                        anyEvent = 1;
                    }
                }

                i++;
            } while (i < 0x40);

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            }
            break;
        }

        case 0x300000C: {
            void* unit;
            void* part;
            void* evt;
            s32 i;
            s32 partId;
            s32 height;
            s8 unusedStatus;
            s8 amount;
            f32 x;
            f32 y;
            f32 z;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0) {
                    BtlUnit_GetStatus(unit, 0x18, &unusedStatus, &amount);

                    if (amount > 0) {
                        BtlUnit_RecoverFp(unit, (s16)amount);

                        partId = BtlUnit_GetBodyPartsId(unit);
                        part = BtlUnit_GetPartsPtr(unit, partId);
                        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);

                        height = BtlUnit_GetHeight(unit) + 5;
                        y += *(f32*)((s32)unit + 0x114) * height;

                        effRecoveryEntry(1, amount, x, y, z);

                        evt = evtEntry(subsetevt_regeneration, 10, 0);
                        if (evt != 0) {
                            *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
                            *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                        }

                        anyEvent = 1;
                    }
                }

                i++;
            } while (i < 0x40);

            if (anyEvent == 0) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            }
            break;
        }

        case 0x300000E: {
            void* unit;
            void* part;
            void* evt;
            s32 i;
            s32 badgeCount;
            s32 healCount;
            s32 threshold;
            s32 partId;
            s32 height;
            f32 x;
            f32 y;
            f32 z;

            if (*(s16*)((s32)battleWork + 0x0) < 2) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                i = 0;
                do {
                    unit = BattleGetUnitPtr(battleWork, i);

                    if (unit != 0 &&
                        BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                        *(s16*)((s32)unit + 0x10C) > 0) {
                        healCount = 0;

                        badgeCount = *(u8*)((s32)unit + 0x2EE);
                        while (badgeCount > 0) {
                            threshold = ((*(s16*)((s32)unit + 0x10C) / *(s16*)((s32)unit + 0x108)) * -3) + 0x21;

                            if (irand(100) < threshold) {
                                healCount++;
                            }

                            badgeCount--;
                        }

                        if (healCount > 0) {
                            *(s16*)((s32)unit + 0x10C) = *(s16*)((s32)unit + 0x10C) + (s16)healCount;

                            if (*(s16*)((s32)unit + 0x108) < *(s16*)((s32)unit + 0x10C)) {
                                *(s16*)((s32)unit + 0x10C) = *(s16*)((s32)unit + 0x108);
                            }

                            partId = BtlUnit_GetBodyPartsId(unit);
                            part = BtlUnit_GetPartsPtr(unit, partId);
                            BtlUnit_GetPartsWorldPos(part, &x, &y, &z);

                            height = BtlUnit_GetHeight(unit);
                            y += *(f32*)((s32)unit + 0x114) * height;

                            effRecoveryEntry(0, healCount, x, y, z);

                            evt = evtEntry(subsetevt_happy_heart, 10, 0);
                            if (evt != 0) {
                                *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
                                *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                            }

                            anyEvent = 1;
                        }
                    }

                    i++;
                } while (i < 0x40);

                if (anyEvent == 0) {
                    BattleIncSeq(battleWork, 3);
                    BattleIncSeq(battleWork, 3);
                    btlseqTurn(battleWork);
                } else {
                    BattleIncSeq(battleWork, 3);
                }
            }
            break;
        }

        case 0x3000010: {
            void* unit;
            void* part;
            void* evt;
            s32 i;
            s32 badgeCount;
            s32 recoverCount;
            s32 threshold;
            s32 partId;
            s32 height;
            f32 x;
            f32 y;
            f32 z;

            if (*(s16*)((s32)battleWork + 0x0) < 2) {
                BattleIncSeq(battleWork, 3);
                BattleIncSeq(battleWork, 3);
                btlseqTurn(battleWork);
            } else {
                i = 0;
                do {
                    unit = BattleGetUnitPtr(battleWork, i);

                    if (unit != 0 &&
                        BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                        *(s16*)((s32)unit + 0x10C) > 0) {
                        recoverCount = 0;

                        badgeCount = *(u8*)((s32)unit + 0x2EF);
                        while (badgeCount > 0) {
                            threshold = ((*(s16*)((s32)unit + 0x112) / *(s16*)((s32)unit + 0x10E)) * -3) + 0x21;

                            if (irand(100) < threshold) {
                                recoverCount++;
                            }

                            badgeCount--;
                        }

                        if (recoverCount > 0) {
                            BtlUnit_RecoverFp(unit, (s16)recoverCount);

                            partId = BtlUnit_GetBodyPartsId(unit);
                            part = BtlUnit_GetPartsPtr(unit, partId);
                            BtlUnit_GetPartsWorldPos(part, &x, &y, &z);

                            height = BtlUnit_GetHeight(unit);
                            y += *(f32*)((s32)unit + 0x114) * height;

                            effRecoveryEntry(1, recoverCount, x, y, z);

                            evt = evtEntry(subsetevt_happy_flower, 10, 0);
                            if (evt != 0) {
                                *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)evt + 0x15C);
                                *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                            }

                            anyEvent = 1;
                        }
                    }

                    i++;
                } while (i < 0x40);

                if (anyEvent == 0) {
                    BattleIncSeq(battleWork, 3);
                    BattleIncSeq(battleWork, 3);
                    btlseqTurn(battleWork);
                } else {
                    BattleIncSeq(battleWork, 3);
                    btlseqTurn(battleWork);
                }
            }
            break;
        }

        case 0x3000040:
            if (*(s32*)((s32)battleWork + 0x19050) != 0) {
                *(s32*)((s32)battleWork + 0x19050) = *(s32*)((s32)battleWork + 0x19050) - 1;
            }
            BattleIncSeq(battleWork, 3);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x3000042:
            if (*(s32*)((s32)battleWork + 0x19054) != 0) {
                *(s32*)((s32)battleWork + 0x19054) = *(s32*)((s32)battleWork + 0x19054) - 1;
            }
            BattleIncSeq(battleWork, 3);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x3000044:
            if (*(s32*)((s32)battleWork + 0x19058) != 0) {
                *(s32*)((s32)battleWork + 0x19058) = *(s32*)((s32)battleWork + 0x19058) - 1;
            }
            BattleIncSeq(battleWork, 3);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x3000046: {
            void* unit;
            s32 i;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0 &&
                    *(u16*)((s32)unit + 0x13C) != 0) {
                    *(s16*)((s32)unit + 0x13C) = *(s16*)((s32)unit + 0x13C) - 1;
                }

                i++;
            } while (i < 0x40);

            BattleIncSeq(battleWork, 3);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;
        }

        case 0x3000048: {
            void* unit;
            s32 i;

            i = 0;
            do {
                unit = BattleGetUnitPtr(battleWork, i);

                if (unit != 0 &&
                    BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                    *(s16*)((s32)unit + 0x10C) > 0 &&
                    *(u16*)((s32)unit + 0x13E) != 0) {
                    *(u16*)((s32)unit + 0x13E) = *(u16*)((s32)unit + 0x13E) - 1;
                }

                i++;
            } while (i < 0x40);

            BattleIncSeq(battleWork, 3);
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;
        }

        case 0x300004A:
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
            break;

        case 0x300004B:
            BattleIncSeq(battleWork, 3);
            BattleSetSeq(battleWork, 4, 0x4000000);
            break;

        case 0x300004C:
            btlseqPhase(battleWork);
            break;

        case 0x300004D:
            BattleAudience_Case_TurnEnd();
            BattleSetSeq(battleWork, 3, 0x3000001);
            *(u32*)((s32)battleWork + 0xEF4) &= 0xFFFFDFFF;
            break;


        default:
            break;
    }
}

void _rule_disp(void) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void*, s32);
    extern char* msgSearch(char*);
    extern s32 sprintf(char*, const char*, ...);
    extern u32 FontGetMessageWidthLine(char*, u16*);
    extern void windowDispGX_Waku_col(f32, f32, f32, f32, f32, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawMessage(u32, u32, char*);
    extern char* _rule_msg_table_1bu[];
    extern char* _rule_msg_table_2bu[];
    extern char* _rule_msg_table_after[];
    extern u8 lbl_803638E0[];
    extern const u32 dat_80422554;
    extern const f32 float_0_80422568;
    extern const f32 float_120_80422590;
    void* information;
    u8* tables;
    u32 color;
    u16 lines[2];
    char buffer[256];
    u32 dimensions;
    f32 x;

    information = *(void**)((s32)_battleWorkPointer + 0x2738);
    tables = lbl_803638E0;
    if (evtGetValue(0, -170000000) < 0xAC) {
        if (evtGetValue(0, -170000444) < 0xB) {
            sprintf(buffer,
                    msgSearch(((char**)(tables + 0x150))[*(u8*)((s32)information + 0x18)]),
                    *(u8*)((s32)information + 0x1A));
        } else {
            sprintf(buffer,
                    msgSearch(((char**)(tables + 0x40))[*(u8*)((s32)information + 0x18)]),
                    *(u8*)((s32)information + 0x1A));
        }
    } else {
        sprintf(buffer,
                msgSearch(((char**)(tables + 0x260))[*(u8*)((s32)information + 0x18)]),
                *(u8*)((s32)information + 0x1A));
    }
    dimensions = FontGetMessageWidthLine(buffer, lines);
    lines[0]++;
    x = float_0_80422568 - (f32)((dimensions >> 1) & 0x7FFF);
    color = dat_80422554;
    windowDispGX_Waku_col(x - 10.0f, 94.0f,
                          (f32)(s32)((dimensions & 0xFFFF) + 0x14),
                          (f32)(s32)(lines[0] * 0x1D + 3),
                          10.0f, 0, &color);
    FontDrawStart();
    FontDrawMessage((s32)x, (s32)float_120_80422590 - 0x1D, buffer);
}

void btlseqPhase(void* battleWork) {
    extern s32 BattleGetSeq(void*, s32);
    extern void BattleIncSeq(void*, s32);
    extern void BattleSetSeq(void*, s32, s32);
    extern void btlseqMove(void*);
    s32 seq;

    seq = BattleGetSeq(battleWork, 4);
    switch (seq) {
        case 0x4000000:
            BattleIncSeq(battleWork, 4);
            BattleSetSeq(battleWork, 5, 0x5000000);
            break;
        case 0x4000001:
        case 0x4000002:
        case 0x4000003:
        case 0x4000004:
        case 0x4000005:
            btlseqMove(battleWork);
            break;
        case 0x4000006:
            BattleIncSeq(battleWork, 3);
            BattleSetSeq(battleWork, 4, 0x4000000);
            break;
    }
}

void btlseqMove(void* battleWork) {
    extern void btlseqPhaseFirstProcess(void*);
    extern void BattleRunPhaseEvent(void*, s32);
    extern void BattleSetTotalHitDamage(void*, s32);
    extern u32 BattlePhaseEndCheck(void);
    extern u32 BattleAudience_CheckReactionPerPhase(void);
    extern s32 BattleStatusChangeMsgMain(void*);
    extern s32 BattleStatusChangeAnnouceMain(void*);
    extern void* pouchGetPtr(void);
    extern void* BattleGetMarioPtr(void*);
    extern void BattleMajinaiDone(void);
    extern void BattleMajinaiEndCheck(void);
    extern void* evtEntry(void*, s32, s32);
    extern void* memset(void*, s32, u32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern void BtlUnit_SetTotalHitDamage(void*, s32);
    extern void BattleAudience_PerPhase(s32);
    extern u8 marioAttackEvent_MajinaiDefenceUp[];
    void* unit;
    void* pouch;
    void* event;
    s32 seq;
    s32 i;
    s32 id;
    s32 advance;
    s32 state;
    s32 ended;
    s32 offset;

    do {
        advance = 0;
        seq = BattleGetSeq(battleWork, 5);
        switch (seq) {
            case 0x5000000:
                btlseqPhaseFirstProcess(battleWork);
                BattleIncSeq(battleWork, 5);
                advance = 1;
                break;
            case 0x5000001:
                BattleIncSeq(battleWork, 5);
                advance = 1;
                break;
            case 0x5000002:
                pouch = pouchGetPtr();
                unit = BattleGetMarioPtr(battleWork);
                if (*(u8*)((s32)pouch + 0x5BA) != 2 || BattleGetSeq(battleWork, 4) != 0x4000004) {
                    BattleIncSeq(battleWork, 5);
                    BattleIncSeq(battleWork, 5);
                    advance = 1;
                    break;
                } else {
                    *(u8*)((s32)battleWork + 0x18FF9) = 2;
                    BattleMajinaiDone();
                    BattleMajinaiEndCheck();
                    event = evtEntry(marioAttackEvent_MajinaiDefenceUp, 10, 0);
                    *(s32*)((s32)event + 0x160) = *(s32*)unit;
                    *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)event + 0x15C);
                    BattleIncSeq(battleWork, 5);
                }
            case 0x5000003:
                if (BattleWaitAllActiveEvtEnd(battleWork)) {
                    BattleIncSeq(battleWork, 5);
                    advance = 1;
                }
                break;
            case 0x5000004:
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(battleWork, i);
                    if (unit != NULL && *(void**)((s32)unit + 0x294) != NULL) {
                        BattleRunPhaseEvent(unit, 1);
                    }
                }
                BattleIncSeq(battleWork, 5);
                advance = 1;
                break;
            case 0x5000005:
                if (BattleWaitAllActiveEvtEnd(battleWork)) {
                    BattleIncSeq(battleWork, 5);
                    advance = 1;
                }
                break;
            case 0x5000006:
                for (i = 0, offset = 0; i < 64; i++, offset += 8) {
                    id = *(s32*)((s32)battleWork + offset + 0x220);
                    if (id != -1) {
                        unit = BattleGetUnitPtr(battleWork, id);
                        if (unit != NULL && *(void**)((s32)unit + 0x298) != NULL) {
                            break;
                        }
                    }
                }
                if (i < 64) {
                    BattleRunPhaseEvent(unit, 0);
                    ((s32*)battleWork)[0x88 + i * 2] = -1;
                    BattleIncSeq(battleWork, 5);
                } else {
                    BattleSetSeq(battleWork, 5, 0x500000A);
                    advance = 1;
                }
                break;
            case 0x5000007:
                if (BattleWaitAllActiveEvtEnd(battleWork)) {
                    if (BattleCheckConcluded(battleWork)) {
                        BattleSetSeq(battleWork, 0, 3);
                        BattleSetSeq(battleWork, 7, 0x7000000);
                    } else {
                        BattleSetSeq(battleWork, 5, 0x5000006);
                        advance = 1;
                    }
                }
                break;
            case 0x5000008:
                memset((void*)((s32)battleWork + 0x18FFC), 0, 12);
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(battleWork, i);
                    if (unit != NULL) {
                        *(u32*)((s32)unit + 0x27C) &= 0x80002000;
                        *(s32*)((s32)unit + 0x264) = 0;
                        *(s8*)((s32)unit + 0x270) = 0;
                        *(s16*)((s32)unit + 0x260) = 0;
                        *(s8*)((s32)unit + 0x273) = 0;
                        *(s32*)((s32)unit + 0x268) = 0;
                        *(s8*)((s32)unit + 0x271) = 0;
                        *(s32*)((s32)unit + 0x26C) = 0;
                        *(s8*)((s32)unit + 0x272) = 0;
                        *(s32*)((s32)unit + 0x280) = 0;
                    }
                }
                for (i = 0, offset = 0; i < 64; i++, offset += 4) {
                    id = *(s32*)((s32)battleWork + offset + 0x120);
                    if (id != -1) {
                        unit = BattleGetUnitPtr(battleWork, id);
                        if (unit != NULL && *(s8*)((s32)unit + 0x22) > 0 &&
                            (state = *(s8*)((s32)unit + 0x20)) != 10 &&
                            (!BtlUnit_CheckStatus(unit, 0x1B) || (*(u32*)((s32)unit + 0x104) & 0x10000))) {
                            if (state != 1) break;
                            *(u8*)((s32)unit + 0x20) = 0;
                        }
                    }
                }
                *(s32*)((s32)battleWork + 0x420) = id;
                unit = BattleGetUnitPtr(battleWork, id);
                *(u32*)((s32)unit + 0x27C) |= 0x1000;
                BattleIncSeq(battleWork, 5);
                BattleSetSeq(battleWork, 6, 0x6000000);
                for (i = 0, offset = 0; i < 64; i++, offset += 4) {
                    id = *(s32*)((s32)battleWork + offset + 0x120);
                    if (id != -1) {
                        unit = BattleGetUnitPtr(battleWork, id);
                        if (unit && *(u8*)((s32)unit + 0x20) == 1) *(u8*)((s32)unit + 0x20) = 0;
                    }
                }
                unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
                if (*(s32*)((s32)unit + 8) == 0xDE) {
                    pouch = pouchGetPtr();
                    *(u32*)((s32)battleWork + 0xEF4) |= 0x600;
                    for (i = 0; i < 200; i++) {
                        s32 badge = ((s16*)pouch)[0x1C5 + i];
                        if (badge >= 0xF0 && badge < 0xF7) {
                            *(u32*)((s32)battleWork + 0xEF4) &= ~0x200;
                            break;
                        }
                    }
                    if (*(s8*)((s32)pouch + 0x98) >= 2) *(u32*)((s32)battleWork + 0xEF4) &= ~0x200;
                    for (i = 0; i < 200; i++) {
                        s32 badge = ((s16*)pouch)[0x1C5 + i];
                        if (badge >= 0xF7 && badge < 0xFE) {
                            *(u32*)((s32)battleWork + 0xEF4) &= ~0x400;
                            break;
                        }
                    }
                    if (*(s8*)((s32)pouch + 0x99) >= 2) *(u32*)((s32)battleWork + 0xEF4) &= ~0x400;
                }
                break;
            case 0x5000009:
                btlseqAct(battleWork);
                break;
            case 0x500000A:
                if (BattleCheckConcluded(battleWork)) {
                    BattleSetSeq(battleWork, 0, 3);
                    BattleSetSeq(battleWork, 7, 0x7000000);
                } else {
                    for (i = 0; i < 64; i++) {
                        unit = BattleGetUnitPtr(battleWork, i);
                        if (unit != NULL) {
                            BtlUnit_SetTotalHitDamage(unit, 0);
                            *(u32*)((s32)unit + 0x27C) &= ~0x1000;
                        }
                    }
                    ended = BattlePhaseEndCheck();
                    if (*(u32*)((s32)battleWork + 0xEF4) & 0x2000) ended = 1;
                    if (ended) {
                        if (*(u8*)((s32)battleWork + 0x18FF9) == 2) *(u8*)((s32)battleWork + 0x18FF9) = 0;
                        BattleAudience_PerPhase(BattleGetSeq(battleWork, 4));
                        BattleIncSeq(battleWork, 5);
                    } else {
                        BattleSetSeq(battleWork, 5, 0x5000008);
                    }
                }
                break;
            case 0x500000B:
                    if (!(BattleAudience_CheckReactionPerPhase() & 0xFF) &&
                        BattleWaitAllActiveEvtEnd(battleWork)) {
                        BattleIncSeq(battleWork, 5);
                    }
                break;
            case 0x500000C:
                    i = BattleStatusChangeMsgMain(battleWork);
                    id = BattleStatusChangeAnnouceMain(battleWork);
                    if ((id | i) == 0) {
                        BattleIncSeq(battleWork, 4);
                        BattleSetSeq(battleWork, 5, 0x5000000);
                    }
                break;
            case 0x500000D:
                    BattleIncSeq(battleWork, 5);
                break;
            case 0x500000E:
                    BattleIncSeq(battleWork, 4);
                    BattleSetSeq(battleWork, 5, 0x5000000);
                break;
        }
    } while (advance != 0);
}

u32 BattlePhaseEndCheck(void) {
    s32 offset;
    void* battleWork;
    s32 i;
    void* unit;
    s32 unitId;

    i = 0;
    offset = 0;
    battleWork = _battleWorkPointer;
    while (i < 0x40) {
        unitId = *(s32*)((s32)battleWork + offset + 0x120);
        if (unitId != -1) {
            unit = BattleGetUnitPtr(battleWork, unitId);
            if (unit != 0 &&
                *(s8*)((s32)unit + 0x20) != 0xA &&
                *(s8*)((s32)unit + 0x22) > 0 &&
                BtlUnit_CheckStatus(unit, 0x1B) == 0) {
                break;
            }
        }
        i++;
        offset += 4;
    }

    if (i >= 0x40) {
        return 1;
    }
    return 0;
}


void btlseqAct(void* battleWork) {
    extern void effNiceAsync(s32);
    extern void BattleAudience_ActInit(void);
    extern void BattleStatusWindowSystemOn(void);
    extern void BattleStatusWindowSystemOff(void);
    extern void BattleCommandInit(void*);
    extern f32 battleCameraGetPosMoveSpeed(void);
    extern s32 BattleCommandInput(void*);
    extern void* BtlUnit_GetData(void*, s32);
    extern void* evtEntry(void*, s32, s32);
    extern s32 BattleStatusChangeMsgMain(void*);
    extern s32 BattleStatusChangeAnnouceMain(void*);
    extern void BattleStatusChangeMsgAdjust(void*);
    extern void BtlUnit_SetStatus(void*, s32, s32, s32);
    extern void BattleConsumeReserveItem(void);
    extern void BattleAudience_PerAct(void);
    extern u32 BattleAudience_CheckReaction(void);
    extern u32 BattleBreakSlot_CheckReaction(void);
    extern s32 evtCheckID(s32);
    extern void* BattleGetMarioPtr(void*);
    extern u32 BtlUnit_CheckStatusFlag(void*, u32);
    extern void BtlUnit_OffStatusFlag(void*, u32);
    extern u32 BtlUnit_GetBelong(void*);
    extern u32 BattlePhaseEndCheck(void);
    extern s32 irand(s32);
    extern u8 btldefaultevt_BiribiriMove[];
    extern u8 btldefaultevt_CantMoveZeroGravity[];
    s32 unable;
    void* unit;
    void* mario;
    void* event;
    s32 seq;
    s32 messageBusy;
    s32 announceBusy;
    s8 moveState;

    seq = BattleGetSeq(battleWork, 6);
    switch (seq) {
        case 0x6000000:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            effNiceAsync(1);
            *(s32*)((s32)unit + 0x284) = 0;
            *(void**)((s32)unit + 0x2D0) = NULL;
            BattleAudience_ActInit();
            unable = 1;
            switch (*(s32*)((s32)unit + 8)) {
                case 0xDE:
                    if (!BtlUnit_CheckStatus(unit, 1) &&
                        !BtlUnit_CheckStatus(unit, 2) &&
                        !BtlUnit_CheckStatus(unit, 9) &&
                        !BtlUnit_CheckStatus(unit, 0x1B)) {
                        unable = 0;
                    }
                    break;
                default:
                    if (*(s32*)((s32)unit + 8) >= 0xE0 &&
                        *(s32*)((s32)unit + 8) < 0xE7 &&
                        !BtlUnit_CheckStatus(unit, 1) &&
                        !BtlUnit_CheckStatus(unit, 2) &&
                        !BtlUnit_CheckStatus(unit, 9) &&
                        !BtlUnit_CheckStatus(unit, 0x1B)) {
                        unable = 0;
                    }
                    break;
            }
            if (unable) {
                BattleSetSeq(battleWork, 6, 0x6000003);
            } else if ((*(u32*)((s32)unit + 0x104) & 0x10) != 0) {
                BattleSetSeq(battleWork, 6, 0x6000004);
            } else if (*(s32*)((s32)unit + 8) == 0xE4 &&
                       (mario = BattleGetMarioPtr(battleWork),
                        (*(u32*)((s32)mario + 0x104) & 0x10) != 0)) {
                BattleSetSeq(battleWork, 6, 0x6000004);
            } else if (*(s8*)((s32)unit + 0x12B) != 0) {
                BattleSetSeq(battleWork, 6, 0x6000005);
            } else {
                BattleCommandInit(battleWork);
                BattleSetSeq(battleWork, 6, 0x6000001);
            }
            break;
        case 0x6000001:
            if (battleCameraGetPosMoveSpeed() > 2.0f) return;
            BattleSetSeq(battleWork, 6, 0x6000002);
        case 0x6000002:
            if (BattleCommandInput(battleWork) != 0) {
                *(u32*)((s32)battleWork + 0xEF4) &= ~0x80;
                BattleSetSeq(battleWork, 6, 0x6000006);
            }
            break;
        case 0x6000005:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            *(void**)((s32)unit + 0x2A0) = BtlUnit_GetData(unit, 2);
            *(s32*)((s32)unit + 0x2AC) = 1;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000003:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            *(s32*)((s32)unit + 0x2AC) = 1;
            *(u32*)((s32)unit + 0x1C) |= 0x04000000;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000004:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            *(void**)((s32)unit + 0x2A0) = BtlUnit_GetData(unit, 3);
            *(s32*)((s32)unit + 0x2AC) = 0x1B;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000006:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            if ((*(u32*)((s32)unit + 0x2AC) & 1) == 0) {
                BattleStatusWindowSystemOff();
            }
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            event = NULL;
            if ((*(u32*)((s32)battleWork + 0xEF4) & 0x40000000) != 0 &&
                (*(u32*)((s32)unit + 0x104) & 0x2000) == 0 &&
                (*(u32*)((s32)unit + 0x2AC) & 8) == 0 &&
                (*(u32*)((s32)unit + 0x2AC) & 0x10) == 0) {
                event = evtEntry(btldefaultevt_CantMoveZeroGravity, 10, 0);
            } else if ((*(u32*)((s32)unit + 0x2AC) & 0x10) != 0) {
                if (*(void**)((s32)unit + 0x2A0) != NULL) {
                    event = evtEntry(*(void**)((s32)unit + 0x2A0), 10, 0);
                }
            } else if ((*(u32*)((s32)unit + 0x2AC) & 2) != 0) {
                event = evtEntry(*(void**)((s32)unit + 0x2A0), 10, 0);
            } else if (!BtlUnit_CheckStatus(unit, 1) &&
                       !BtlUnit_CheckStatus(unit, 2) &&
                       !BtlUnit_CheckStatus(unit, 9) &&
                       (!BtlUnit_CheckStatus(unit, 0x1B) ||
                        (*(u32*)((s32)unit + 0x104) & 0x10000) != 0)) {
                if (BtlUnit_CheckStatus(unit, 6) && irand(100) < 0) {
                    event = evtEntry(btldefaultevt_BiribiriMove, 10, 0);
                } else {
                    if (BtlUnit_CheckStatus(unit, 5) && irand(100) < 50) {
                        *(u32*)((s32)unit + 0x27C) |= 0x10;
                        event = evtEntry(*(void**)((s32)unit + 0x2A4), 10, 0);
                    } else {
                        event = evtEntry(*(void**)((s32)unit + 0x2A0), 10, 0);
                    }
                }
            }
            if (event != NULL) {
                *(s32*)((s32)unit + 0x2A8) = *(s32*)((s32)event + 0x15C);
                *(s32*)((s32)event + 0x160) = *(s32*)unit;
                if (*(s32*)((s32)unit + 0x290) != 0) {
                    evtDeleteID(*(s32*)((s32)unit + 0x290));
                    *(s32*)((s32)unit + 0x290) = 0;
                }
            }
            BattleIncSeq(battleWork, 6);
            break;
        case 0x6000007:
            if (!BattleWaitAllActiveEvtEnd(battleWork)) return;
            BattleIncSeq(battleWork, 6);
            BattleStatusChangeMsgAdjust(battleWork);
        case 0x6000008:
            if (!BattleWaitAllActiveEvtEnd(battleWork)) return;
            messageBusy = BattleStatusChangeMsgMain(battleWork);
            announceBusy = BattleStatusChangeAnnouceMain(battleWork);
            if ((messageBusy | announceBusy) != 0) return;
            BattleIncSeq(battleWork, 6);
        case 0x6000009:
            if ((*(u32*)((s32)battleWork + 0xEF4) & 0x30) == 0) {
                BattleStatusWindowSystemOn();
            }
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            if (unit != NULL) {
                *(u32*)((s32)unit + 0x1C) &= 0xFBFFFFFF;
                if ((*(u32*)((s32)unit + 0x27C) & 8) != 0) {
                    BtlUnit_SetStatus(unit, 0x10, 1, -99);
                    *(u32*)((s32)unit + 0x27C) &= ~8;
                }
                if (*(u8*)((s32)battleWork + 0x18FF9) == 1) {
                    *(s8*)((s32)battleWork + 0x18FF9) = 0;
                }
                *(s32*)((s32)unit + 0x25C) = 0;
                moveState = *(s8*)((s32)unit + 0x20);
                switch (moveState) {
                    case 5: *(u8*)((s32)unit + 0x20) = 6; break;
                    case 6: *(u8*)((s32)unit + 0x20) = 7; break;
                    case 8: *(u8*)((s32)unit + 0x20) = 9; break;
                    case 7:
                    case 9: *(u8*)((s32)unit + 0x20) = 0; break;
                }
                moveState = *(s8*)((s32)unit + 0x20);
                if (moveState == 0) {
                    if (!BtlUnit_CheckStatusFlag(unit, 0x02000000)) {
                        (*(s8*)((s32)unit + 0x22))--;
                    } else {
                        BtlUnit_OffStatusFlag(unit, 0x02000000);
                    }
                    if ((*(u32*)((s32)battleWork + 0xEF4) & 0x2000) != 0) {
                        *(s8*)((s32)unit + 0x22) = 0;
                    }
                    if (*(s8*)((s32)unit + 0x22) < 1) {
                        *(u8*)((s32)unit + 0x20) = 10;
                        if ((*(u32*)((s32)unit + 0x27C) & 0x100) == 0) {
                            *(u8*)((s32)unit + 0x24) = 0;
                        } else {
                            *(u32*)((s32)unit + 0x27C) &= ~0x100;
                        }
                        if ((s8)BtlUnit_GetBelong(unit) == 0 &&
                            *(s32*)((s32)unit + 0x2C) == 0x4000002 &&
                            !BattlePhaseEndCheck() &&
                            (*(u32*)((s32)battleWork + 0xEF4) & 0x2000) == 0) {
                            *(u8*)((s32)unit + 0x311) = 0x60;
                        }
                    }
                } else if (moveState == 3 || moveState == 4) {
                    *(u8*)((s32)unit + 0x20) = 0;
                }
            }
            BattleConsumeReserveItem();
            *(u32*)((s32)battleWork + 0xEF4) &= ~4;
            BattleAudience_PerAct();
            BattleIncSeq(battleWork, 6);
        case 0x600000A:
            if ((BattleAudience_CheckReaction() & 0xFF) == 0 &&
                (BattleCheckConcluded(battleWork) ||
                 (BattleBreakSlot_CheckReaction() & 0xFF) == 0)) {
                BattleIncSeq(battleWork, 6);
            } else {
                break;
            }
        case 0x600000B:
            messageBusy = BattleStatusChangeMsgMain(battleWork);
            announceBusy = BattleStatusChangeAnnouceMain(battleWork);
            if ((messageBusy | announceBusy) != 0) {
                break;
            }
            BattleIncSeq(battleWork, 6);
        case 0x600000C:
            if (!BattleWaitAllActiveEvtEnd(battleWork)) {
                break;
            }
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            if (unit != NULL) {
                *(s32*)((s32)unit + 0x2CC) = 0;
                if (*(void**)((s32)unit + 0x2C8) != NULL) {
                    event = evtEntry(*(void**)((s32)unit + 0x2C8), 10, 0);
                    if (event != NULL) {
                        *(s32*)((s32)unit + 0x2CC) = *(s32*)((s32)event + 0x15C);
                        *(s32*)((s32)event + 0x160) = *(s32*)unit;
                        if (*(s32*)((s32)unit + 0x290) != 0) {
                            evtDeleteID(*(s32*)((s32)unit + 0x290));
                            *(s32*)((s32)unit + 0x290) = 0;
                        }
                    }
                }
            }
            BattleIncSeq(battleWork, 6);
        case 0x600000D:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
            if (unit != NULL && *(s32*)((s32)unit + 0x2CC) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x2CC))) {
                    break;
                }
                *(s32*)((s32)unit + 0x2CC) = 0;
            }
            BattleIncSeq(battleWork, 6);
        case 0x600000E:
            if (BattleWaitAllActiveEvtEnd(battleWork)) {
                BattleIncSeq(battleWork, 5);
            }
            break;
    }
}

void btlseqTurnFirstProcess(void* battleWork) {
    extern void BtlActRec_JudgeTurnRuleKeep(void);
    extern void BattleMajinaiCheck(void);
    extern void BtlActRec_AddCount(void*);
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_ResetMoveStatus(void*);
    extern s32 irand(s32);
    extern s32 BtlUnit_CheckStatusFlag(void*, u32);
    extern void BtlUnit_OffStatusFlag(void*, u32);
    extern void BattleFogEndCheck(void);
    void* unit;
    void* kind;
    s32 i;
    BtlActRec_JudgeTurnRuleKeep();
    (*(s16*)battleWork)++;
    if (*(s16*)battleWork >= 2) BattleMajinaiCheck();
    *(s32*)((s32)battleWork + 0x18FF4) = 0;
    *(s32*)((s32)battleWork + 0x18FF0) = 0;
    BtlActRec_AddCount((void*)((s32)battleWork + 0x16F57));
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            (*(u8*)((s32)unit + 0x24))++;
            BtlUnit_ResetMoveStatus(unit);
            kind = *(void**)((s32)unit + 0x10);
            *(s32*)((s32)unit + 0x28) = irand(*(s8*)((s32)kind + 0x89)) + *(s8*)((s32)kind + 0x88) - *(s8*)((s32)kind + 0x89) / 2;
            if (BtlUnit_CheckStatusFlag(unit, 0x1000000))
                BtlUnit_OffStatusFlag(unit, 0x1000000);
        }
    }
    BattleFogEndCheck();
}

void btlseqPhaseFirstProcess(void* battleWork) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern s32 BattleGetSeq(void*, s32);
    extern void battleSortPhaseMoveTable(void*, s32);
    extern void battleMakePhaseEvtTable(void*);
    void* unit;
    void* part;
    s32 count;
    s32 i;
    *(s32*)((s32)battleWork + 0x1C70) = 0;
    *(u8*)((s32)battleWork + 0x1C74) = 0;
    for (i = 0; i < 0x40; i++) ((s32*)battleWork)[i + 0x48] = -1;
    count = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            *(u8*)((s32)unit + 0x311) = 0xFF;
            if (*(u8*)((s32)unit + 0x20) != 10 || BtlUnit_CheckStatus(unit, 0x14) != 0) {
                if (*(s32*)((s32)unit + 0x2C) == BattleGetSeq(battleWork, 4) &&
                    (*(u32*)((s32)unit + 0x104) & 0x80000) == 0) {
                    ((s32*)battleWork)[count + 0x48] = *(s32*)unit;
                    *(s32*)((s32)unit + 0x140) = -1;
                    for (part = *(void**)((s32)unit + 0x14); part != 0; part = *(void**)part) {
                        if (*(s32*)((s32)part + 0x1C0) != -1) *(u8*)((s32)part + 0x4E4) = 1;
                    }
                    count++;
                }
            }
        }
    }
    battleSortPhaseMoveTable(battleWork, count);
    battleMakePhaseEvtTable(battleWork);
}


void battleSortPhaseMoveTable(void* battleWork, s32 count) {
    typedef struct { u8 pad[0x120]; s32 first, second; } MovePair;
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    s32 i, j, temp;
    void* unitA;
    void* unitB;
    MovePair* pair;
    f32 ax, ay, az, bx, by, bz;
    if (count <= 0) {
        count = 0;
        for (i = 0; i < 0x40; i++) {
            if (*(s32*)((s32)battleWork + 0x120 + i * 4) == -1) break;
            count++;
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            pair = (MovePair*)((s32)battleWork + j * 4);
            unitA = BattleGetUnitPtr(battleWork, pair->first);
            unitB = BattleGetUnitPtr(battleWork, pair->second);
            BtlUnit_GetHomePos(unitA, &ax, &ay, &az);
            BtlUnit_GetHomePos(unitB, &bx, &by, &bz);
            if (__fabsf(ax) > __fabsf(bx)) {
                temp = pair->first;
                pair->first = pair->second;
                pair->second = temp;
            }
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            pair = (MovePair*)((s32)battleWork + j * 4);
            unitA = BattleGetUnitPtr(battleWork, pair->first);
            unitB = BattleGetUnitPtr(battleWork, pair->second);
            if (*(s32*)((s32)unitA + 0x28) < *(s32*)((s32)unitB + 0x28)) {
                temp = pair->first;
                pair->first = pair->second;
                pair->second = temp;
            }
        }
    }
}

void battleMakePhaseEvtTable(void* battleWork) {
    typedef struct { s32 unit, priority; } PhaseEntry;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    void* unit;
    PhaseEntry* table;
    f32 x, y, z;
    PhaseEntry temp;
    s32 count, i, j;
    table = (PhaseEntry*)((s32)battleWork + 0x220);
    for (i = 0; i < 0x40; i++) {
        table[i].unit = -1;
        table[i].priority = 0;
    }
    count = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && (*(u32*)((s32)unit + 0x104) & 0x80000) == 0) {
            table[count].unit = i;
            BtlUnit_GetHomePos(unit, &x, &y, &z);
            if (x >= 0.0f) table[count].priority = (s32)(1000.0f + x);
            else table[count].priority = (s32)__fabsf(x);
            count++;
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (table[j].priority > table[j + 1].priority) {
                temp = table[j];
                table[j] = table[j + 1];
                table[j + 1] = temp;
            }
        }
    }
}

s32 BattleWaitAllActiveEvtEnd(void* battleWork) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void*);
    extern s32 evtCheckID(s32);
    s32* eventBase = (s32*)((s32)battleWork + 0x20000);
    s32 result = BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork);
    if (result == 0) return result;
    if (eventBase[-0x7D4C / 4] != 0) {
        if (evtCheckID(eventBase[-0x7D4C / 4])) return 0;
        eventBase[-0x7D4C / 4] = 0;
    }
    if (eventBase[-0x7D3C / 4] != 0) {
        if (evtCheckID(eventBase[-0x7D3C / 4])) return 0;
        eventBase[-0x7D3C / 4] = 0;
    }
    if (eventBase[-0x7F04 / 4] != 0) {
        if (evtCheckID(eventBase[-0x7F04 / 4])) return 0;
        eventBase[-0x7F04 / 4] = 0;
    }
    if (eventBase[-0x7D38 / 4] != 0) {
        if (evtCheckID(eventBase[-0x7D38 / 4])) return 0;
        eventBase[-0x7D38 / 4] = 0;
    }
    if (eventBase[-0x7D40 / 4] != 0) {
        if (evtCheckID(eventBase[-0x7D40 / 4])) return 0;
        eventBase[-0x7D40 / 4] = 0;
    }
    return 1;
}

int BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 evtCheckID(s32);
    extern s32 BattleAfterReactionMain(void);
    s32 i;
    s32 result;
    void* unit;
    result = 1;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            while (*(s32*)((s32)unit + 0x29C) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x29C))) { result = 0; break; }
                *(s32*)((s32)unit + 0x29C) = 0;
                break;
            }
            while (*(s32*)((s32)unit + 0x2A8) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x2A8))) { result = 0; break; }
                *(s32*)((s32)unit + 0x2A8) = 0;
                break;
            }
            while (*(s32*)((s32)unit + 0x2B4) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x2B4))) { result = 0; break; }
                *(s32*)((s32)unit + 0x2B4) = 0;
                break;
            }
            while (*(s32*)((s32)unit + 0x2BC) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x2BC))) { result = 0; break; }
                *(s32*)((s32)unit + 0x2BC) = 0;
                break;
            }
        }
    }
    if (result == 0) return result;
    if (BattleAfterReactionMain() != 0) result = 0;
    return result;
}

s32 BattleCheckConcluded(void* battleWork) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    s32 concluded;
    s32 alliance;
    s32 offset;
    s32 unitId;
    void* unit;
    u16 flags;

    concluded = 0;
    alliance = 0;
    offset = 0;
    do {
        *(s32*)((s32)battleWork + offset + 0xC) = 0;
        flags = *(u16*)((s32)battleWork + offset + 8);
        if (flags == 0) {
            concluded++;
            *(s32*)((s32)battleWork + offset + 0xC) = 1;
        } else {
            if (flags & 1) {
                unitId = 0;
                do {
                    unit = BattleGetUnitPtr(battleWork, unitId);
                    if (unit != 0 && *(s8*)((s32)unit + 0xC) == alliance &&
                        BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                        (*(u32*)((s32)unit + 0x104) & 0x20000) == 0 &&
                        (*(u32*)((s32)unit + 0x104) & 0x10000000) == 0) {
                        break;
                    }
                    unitId++;
                } while (unitId < 0x40);
                if (unitId >= 0x40) {
                    concluded++;
                    *(s32*)((s32)battleWork + offset + 0xC) = 1;
                    goto next_alliance;
                }
            }
            if (*(u16*)((s32)battleWork + offset + 8) & 2) {
                unitId = 0;
                do {
                    unit = BattleGetUnitPtr(battleWork, unitId);
                    if (unit != 0 && *(s8*)((s32)unit + 0xC) == alliance &&
                        (*(u32*)((s32)unit + 0x104) & 0x40000) != 0 &&
                        BtlUnit_CheckStatus(unit, 0x1B) == 0 &&
                        (*(u32*)((s32)unit + 0x104) & 0x10000000) == 0) {
                        break;
                    }
                    unitId++;
                } while (unitId < 0x40);
                if (unitId >= 0x40) {
                    concluded++;
                    *(s32*)((s32)battleWork + offset + 0xC) = 1;
                }
            }
        }
next_alliance:
        alliance++;
        offset += 8;
    } while (alliance < 3);

    if (concluded >= 2) {
        return 1;
    }
    return (*(u32*)((s32)battleWork + 0xEF4) & 0x30) != 0;
}

void _mapobj_data_touch_scale(s32 id) {
    s32 offset = 0;
    void* battleWork = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 0x20; i++, offset += 0x7C) {
        void* entry = (void*)((s32)battleWork + offset + 0x1715C);

        if (*(s32*)entry > 0) {
            void* data = *(void**)((s32)entry + 0x64);
            if (*(u16*)((s32)data + 6) == id) {
                *(u32*)((s32)entry + 0x68) |= 2;
            }
        }
    }
}

s32 _set_haikei_entry_scale(void* work, s32 reset) {
    extern void* _battleWorkPointer;
    extern const f32 float_0_80422568;
    extern const f32 float_1_8042256c;
    extern const f32 float_90_80422578;
    extern const f32 float_100_8042257c;
    extern const f32 float_110_80422574;
    extern char str_A1_80422564[3];
    extern char str_A2_80422570[3];
    extern void* mapGetMapObj(char* name);
    extern void mapObjScale(char* name, f32 x, f32 y, f32 z);
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern void _mapobj_data_touch_scale(s32 id);

    void* battleWork = _battleWorkPointer;
    s32 touched = 0;
    s32 i;
    s32 offset;
    f32 sy;
    f32 sx;
    f32 hundred;

    if (reset != 0) {
        *(s32*)((s32)work + 0x78) = 0x3C;
        *(s32*)((s32)work + 0x7C) = 0;
        if (mapGetMapObj(str_A1_80422564) != 0) {
            mapObjScale(str_A1_80422564, float_0_80422568, float_0_80422568, float_1_8042256c);
        }
        if (mapGetMapObj(str_A2_80422570) != 0) {
            mapObjScale(str_A2_80422570, float_0_80422568, float_0_80422568, float_1_8042256c);
        }
    }

    if (*(s32*)((s32)work + 0x78) > 0) {
        *(s32*)((s32)work + 0x78) = *(s32*)((s32)work + 0x78) - 1;
        if (*(s32*)((s32)work + 0x78) > 0) {
            if (mapGetMapObj(str_A1_80422564) != 0) {
                mapObjScale(str_A1_80422564, float_0_80422568, float_0_80422568, float_1_8042256c);
                _mapobj_data_touch_scale(0);
            }
            if (mapGetMapObj(str_A2_80422570) != 0) {
                mapObjScale(str_A2_80422570, float_0_80422568, float_0_80422568, float_1_8042256c);
                _mapobj_data_touch_scale(1);
            }
            return 0;
        }
    }

    *(s32*)((s32)work + 0x7C) = *(s32*)((s32)work + 0x7C) + 1;

    if (mapGetMapObj(str_A1_80422564) != 0) {
        if (*(s32*)((s32)work + 0x7C) >= 0x28) {
            mapObjScale(str_A1_80422564, float_1_8042256c, float_1_8042256c, float_1_8042256c);
            _mapobj_data_touch_scale(0);
        } else if (*(s32*)((s32)work + 0x7C) < 0xF) {
            touched = 1;
            mapObjScale(str_A1_80422564, float_0_80422568, float_0_80422568, float_1_8042256c);
            _mapobj_data_touch_scale(0);
        } else {
            touched = 1;
            mapObjScale(str_A1_80422564, float_1_8042256c, float_1_8042256c, float_1_8042256c);
            _mapobj_data_touch_scale(0);
            if (*(s32*)((s32)work + 0x7C) < 0x19) {
                sx = intpl_sub(5, float_0_80422568, float_110_80422574, *(s32*)((s32)work + 0x7C) - 0xF, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x23) {
                sx = intpl_sub(0xB, float_110_80422574, float_90_80422578, *(s32*)((s32)work + 0x7C) - 0x19, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x28) {
                sx = intpl_sub(0xB, float_90_80422578, float_100_8042257c, *(s32*)((s32)work + 0x7C) - 0x23, 5);
            }

            if (*(s32*)((s32)work + 0x7C) < 0x19) {
                sy = intpl_sub(5, float_100_8042257c, float_90_80422578, *(s32*)((s32)work + 0x7C) - 0xF, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x23) {
                sy = intpl_sub(0xB, float_90_80422578, float_110_80422574, *(s32*)((s32)work + 0x7C) - 0x19, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x28) {
                sy = intpl_sub(0xB, float_110_80422574, float_100_8042257c, *(s32*)((s32)work + 0x7C) - 0x23, 5);
            }

            hundred = float_100_8042257c;
            offset = 0;
            for (i = 0; i < 0x20; i++, offset += 0x7C) {
                void* entry = (void*)((s32)battleWork + offset + 0x1715C);
                if (*(s32*)entry > 0) {
                    void* data = *(void**)((s32)entry + 0x64);
                    if (*(u16*)((s32)data + 6) == 0) {
                        mapObjScale(*(char**)data, sy / hundred, sx / hundred, float_1_8042256c);
                        *(u32*)((s32)entry + 0x68) |= 2;
                    }
                }
            }
        }
    }

    if (mapGetMapObj(str_A2_80422570) != 0) {
        if (*(s32*)((s32)work + 0x7C) >= 0x2D) {
            mapObjScale(str_A2_80422570, float_1_8042256c, float_1_8042256c, float_1_8042256c);
            _mapobj_data_touch_scale(1);
        } else if (*(s32*)((s32)work + 0x7C) < 0x14) {
            touched = 1;
            mapObjScale(str_A2_80422570, float_0_80422568, float_0_80422568, float_1_8042256c);
            _mapobj_data_touch_scale(1);
        } else {
            touched = 1;
            mapObjScale(str_A2_80422570, float_1_8042256c, float_1_8042256c, float_1_8042256c);
            _mapobj_data_touch_scale(1);
            if (*(s32*)((s32)work + 0x7C) < 0x1E) {
                sx = intpl_sub(5, float_0_80422568, float_110_80422574, *(s32*)((s32)work + 0x7C) - 0x14, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x28) {
                sx = intpl_sub(0xB, float_110_80422574, float_90_80422578, *(s32*)((s32)work + 0x7C) - 0x1E, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x2D) {
                sx = intpl_sub(0xB, float_90_80422578, float_100_8042257c, *(s32*)((s32)work + 0x7C) - 0x28, 5);
            }

            if (*(s32*)((s32)work + 0x7C) < 0x1E) {
                sy = intpl_sub(5, float_100_8042257c, float_90_80422578, *(s32*)((s32)work + 0x7C) - 0x14, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x28) {
                sy = intpl_sub(0xB, float_90_80422578, float_110_80422574, *(s32*)((s32)work + 0x7C) - 0x1E, 0xA);
            } else if (*(s32*)((s32)work + 0x7C) < 0x2D) {
                sy = intpl_sub(0xB, float_110_80422574, float_100_8042257c, *(s32*)((s32)work + 0x7C) - 0x28, 5);
            }

            hundred = float_100_8042257c;
            offset = 0;
            for (i = 0; i < 0x20; i++, offset += 0x7C) {
                void* entry = (void*)((s32)battleWork + offset + 0x1715C);
                if (*(s32*)entry > 0) {
                    void* data = *(void**)((s32)entry + 0x64);
                    if (*(u16*)((s32)data + 6) == 1) {
                        mapObjScale(*(char**)data, sy / hundred, sx / hundred, float_1_8042256c);
                        *(u32*)((s32)entry + 0x68) |= 2;
                    }
                }
            }
        }
    }

    return touched != 0 ? 0 : 2;
}

const u32 dat_80422550 = 0x000000FF;
const u32 dat_80422554 = 0xFFFFFFFF;
const char str_a_80422558[2] = "a";
const u8 gap_after_str_a_8042255a[2] = {0};
const char str_test_8042255c[5] = "test";
const u8 gap_after_str_test_80422561[3] = {0};
const char str_A1_80422564[3] = "A1";
const u8 gap_09_80422567_sdata2 = 0;
const f32 float_0_80422568 = 0.0f;
const f32 float_1_8042256c = 1.0f;
const char str_A2_80422570[3] = "A2";
const u8 gap_09_80422573_sdata2 = 0;
const f32 float_110_80422574 = 110.0f;
const f32 float_90_80422578 = 90.0f;
const f32 float_100_8042257c = 100.0f;
const f32 float_1000_80422580 = 1000.0f;
const f32 float_2_80422584 = 2.0f;
const f32 float_10_80422588 = 10.0f;
const f32 float_94_8042258c = 94.0f;
const f32 float_120_80422590 = 120.0f;
const f32 float_900_80422594 = 900.0f;
const f32 float_45_80422598 = 45.0f;
const f32 float_60_8042259c = 60.0f;
const f32 float_0p75_804225a0 = 0.75f;
const u32 gap_09_804225A4_sdata2 = 0;


/* Recovered battle-sequence event bytecode from the US target object. */
extern s32 btlevtcmd_WaitEventEnd();

u32 nanika_luck_event[12] = {
    0x0001006F,
    0xFE363C8F,
    0x00010009,
    0x0000003C,
    0x00000070,
    0x0001005B,
    (u32)_set_effect_luck,
    0x0002005B,
    (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F,
    0x00000002,
    0x00000001,
};

u32 _default_haikei_set_entry_event[4] = {
    0x0001005B,
    (u32)_set_haikei_entry_scale,
    0x00000002,
    0x00000001,
};
