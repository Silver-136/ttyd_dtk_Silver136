#include "battle/battle_seq.h"

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
void psndSFXOn(const char* name);
void BtlUnit_ResetMoveStatus(void* unit);
void btlseqPhase(void* battleWork);
void BattleAudience_Case_TurnEnd(void);
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
extern f32 float_1_8042256c;
extern f32 float_900_80422594;
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
    sequence = BattleGetSeq(battleWork, 0);
    if (sequence == 2) {
        BattleCheckAllPinchStatus(battleWork, 0);
        btlseqTurn(battleWork);
    } else if (sequence < 2) {
        if (sequence == 0) {
            btlseqInit(battleWork);
            BattleCheckAllPinchStatus(battleWork, 1);
        } else if (sequence > -1) {
            BattleCheckAllPinchStatus(battleWork, 0);
            btlseqFirstAct(battleWork);
        }
    } else if (sequence != 4 && sequence < 4) {
        btlseqEnd(battleWork);
    }
}

void BattleCheckAllPinchStatus(void* battleWork, s32 firstCheck) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_CheckPinchStatus(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern s32 BtlUnit_GetBelong(void*);
    extern void* BattleGetMarioPtr(void*);
    extern void psndSFXOn(const char*);
    extern char str_SFX_BTL_DANGER1_802efc04[];
    extern char str_SFX_BTL_PINCH1_802efc14[];
    void* unit;
    void* mario;
    s32 i;
    s32 kind;
    u32 statusFlag;
    const char* sound;

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            BtlUnit_CheckPinchStatus(unit, firstCheck);
        }
    }

    statusFlag = 0x20000000;
    sound = str_SFX_BTL_DANGER1_802efc04;
check_status:
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit == 0 || (*(u32*)((s32)unit + 0x138) & statusFlag) == 0) {
            continue;
        }
        if (BtlUnit_CheckStatus(unit, 0x1B) != 0 || BtlUnit_GetBelong(unit) != 0) {
            continue;
        }
        kind = *(s32*)((s32)unit + 8);
        if ((kind == 0xDE || (kind > 0xDF && kind < 0xE7)) &&
            *(s16*)((s32)unit + 0x10C) > 0) {
            (*(s8*)((s32)battleWork + 0x18FF8))--;
            if (*(s8*)((s32)battleWork + 0x18FF8) < 1) {
                *(s8*)((s32)battleWork + 0x18FF8) = 60;
                if ((*(u32*)((s32)battleWork + 0x1720) & 1) != 0) {
                    psndSFXOn(sound);
                }
            }
            return;
        }
    }
    if (statusFlag == 0x20000000) {
        statusFlag = 0x10000000;
        sound = str_SFX_BTL_PINCH1_802efc14;
        goto check_status;
    }

    mario = BattleGetMarioPtr(battleWork);
    if (mario != 0) {
        *(s32*)((s32)mario + 0x2D0) = 0;
    }
    *(s8*)((s32)battleWork + 0x18FF8) = 0;
}

void btlseqInit(void* battleWork) {
    extern u8 N_weapon_lucky_start[];
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

    switch (BattleGetSeq(battleWork, 1)) {
        case 0x1000000:
            BattleSetSeq(battleWork, 1, 1);
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
            if (animEffectAsync(N_weapon_lucky_start + 0x3AC, 1) != 0 &&
                animEffectAsync(N_weapon_lucky_start + 0x3B4, 1) != 0 &&
                animEffectAsync(N_weapon_lucky_start + 0x3C0, 1) != 0 &&
                effNiceAsync(1) != 0) {
                BattleSetSeq(battleWork, 1, 0x1000004);
            }
            break;
        case 0x1000004:
            BattleSetSeq(battleWork, 1, 1);
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
    s32 seq;
    void* unit;
    s32 i;
    s32 messageBusy;
    s32 announceBusy;

    seq = BattleGetSeq(battleWork, 2);
    switch (seq) {
        case 0x2000000:
            *(s32*)((s32)battleWork + 0xEFC) = 0;
            for (i = 0; i < 64; i++) {
                unit = BattleGetUnitPtr(battleWork, i);
                if (unit != NULL) {
                    *(u32*)((s32)unit + 0x27C) &= ~0x80000000;
                }
            }
            BattleSetSeq(battleWork, 2, 0x2000001);
            break;
        case 0x2000001:
            (*(s32*)((s32)battleWork + 0xEFC))--;
            if (*(s32*)((s32)battleWork + 0xEFC) > 0) {
                return;
            }
            BattleSetSeq(battleWork, 2, 0x2000002);
        case 0x2000002:
            if (BattleWaitAllActiveEvtEnd(battleWork) && fadeIsFinish()) {
                BattleStatusWindowEventOn();
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
            if (messageBusy != 0 || announceBusy != 0) return;
            BattleIncSeq(battleWork, 2);
        case 0x2000005:
            if ((*(u32*)((s32)battleWork + 0x1720) & 0x30) == 0) {
                BattleStatusWindowSystemOn();
            }
            BattleConsumeReserveItem();
            *(u32*)((s32)battleWork + 0x1720) &= ~4;
            BattleAudience_PerAct();
            BattleIncSeq(battleWork, 2);
        case 0x2000006:
            if ((BattleAudience_CheckReaction() & 0xFF) == 0 &&
                (BattleCheckConcluded(battleWork) ||
                 (BattleBreakSlot_CheckReaction() & 0xFF) == 0)) {
                BattleIncSeq(battleWork, 2);
            }
            break;
        case 0x2000007:
            messageBusy = BattleStatusChangeMsgMain(battleWork);
            announceBusy = BattleStatusChangeAnnouceMain(battleWork);
            if (messageBusy == 0 && announceBusy == 0) {
                BattleIncSeq(battleWork, 2);
            }
            break;
        case 0x2000008:
            if (BattleWaitAllActiveEvtEnd(battleWork)) {
                BattleSetSeq(battleWork, 2, 0x200000A);
            }
            break;
        case 0x2000009:
            BattleStage_WallCloseCheck();
            break;
        case 0x200000A:
            *(s32*)((s32)battleWork + 0x1905C) = 0;
            BattleSetSeq(battleWork, 2, 0x200000B);
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
    extern void psndSFXOn(const char*);
    extern void* effNiceEntry(f32, f32, f32, s32);
    extern char str_SFX_SYSTEM_LUCKY1_802efbb0[];
    u8 weapon[0xC0];
    u32* source;
    u32* destination;
    void* unit;
    void* part;
    void* effect;
    s32 random;
    s32 partId;
    s32 i;
    f32 x;
    f32 y;
    f32 z;

    source = (u32*)0x802EF834;
    destination = (u32*)weapon;
    for (i = 0; i < 0x30; i++) {
        destination[i] = source[i];
    }
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit == 0 || *(u8*)((s32)unit + 0x304) == 0 ||
            BtlUnit_CheckStatus(unit, 0x1B) != 0) {
            continue;
        }
        random = irand(4);
        if (random != 1) {
            weapon[0x8B] = 0;
            weapon[0x8C] = 0;
        }
        if (random != 0) {
            weapon[0x8D] = 0;
            weapon[0x8E] = 0;
        }
        if (random != 2) {
            weapon[0xAA] = 0;
            weapon[0xAB] = 0;
        }
        if (random == 0 || random == 1 || random > 2) {
            weapon[0xAC] = 0;
            weapon[0xAD] = 0;
        }
        partId = BtlUnit_GetBodyPartsId(unit);
        part = BtlUnit_GetPartsPtr(unit, partId);
        BattleSetStatusDamageFromWeapon(BattleGetSystemPtr(_battleWorkPointer),
                                        unit, part, weapon, 0x100);
        BtlUnit_GetPos(unit, &x, &y, &z);
        psndSFXOn(str_SFX_SYSTEM_LUCKY1_802efbb0);
        effect = effNiceEntry(x - 45.0f, y + 60.0f, z, 6);
        *(f32*)(*(s32*)((s32)effect + 0xC) + 0x1C) = 0.75f;
    }
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

        case 0x3000002:
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

        case 0x3000005:
            BattleIncSeq(battleWork, 3);
            btlseqTurn(battleWork);
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

        default:
            break;
    }
}

void _rule_disp(void) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void*, s32);
    extern char* msgSearch(char*);
    extern s32 sprintf(char*, const char*, ...);
    extern u32 FontGetMessageWidthLine(char*, s16*);
    extern void windowDispGX_Waku_col(f32, f32, f32, f32, f32, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawMessage(u32, u32, char*);
    extern char* _rule_msg_table_1bu[];
    extern char* _rule_msg_table_2bu[];
    extern char* _rule_msg_table_after[];
    extern u32 dat_80422554;
    char buffer[256];
    void* information;
    char* message;
    s16 lines[2];
    u32 color;
    u32 dimensions;
    u32 width;
    f32 x;
    u8 condition;
    u8 parameter;

    information = *(void**)((s32)_battleWorkPointer + 0x2738);
    condition = *(u8*)((s32)information + 0x18);
    parameter = *(u8*)((s32)information + 0x1A);
    if (evtGetValue(0, -170000000) < 172) {
        if (evtGetValue(0, -170000444) < 11) {
            message = msgSearch(_rule_msg_table_1bu[condition]);
        } else {
            message = msgSearch(_rule_msg_table_2bu[condition]);
        }
    } else {
        message = msgSearch(_rule_msg_table_after[condition]);
    }
    sprintf(buffer, message, parameter);
    dimensions = FontGetMessageWidthLine(buffer, lines);
    lines[0]++;
    width = dimensions & 0xFFFF;
    x = 0.0f - (f32)((dimensions >> 1) & 0x7FFF);
    color = dat_80422554;
    windowDispGX_Waku_col(x - 10.0f, 94.0f, (f32)(width + 20),
                          (f32)(lines[0] * 29 + 3), 10.0f, 0, &color);
    FontDrawStart();
    FontDrawMessage((u32)x, (u32)(120.0f - 29.0f), buffer);
}

void btlseqPhase(void* battleWork) {
    extern s32 BattleGetSeq(void*, s32);
    extern void BattleIncSeq(void*, s32);
    extern void BattleSetSeq(void*, s32, s32);
    extern void btlseqMove(void*);
    s32 seq;

    seq = BattleGetSeq(battleWork, 4);
    if (seq == 0x4000006) {
        BattleIncSeq(battleWork, 3);
        BattleSetSeq(battleWork, 4, 0x4000000);
    } else if (seq < 0x4000006) {
        if (seq == 0x4000000) {
            BattleIncSeq(battleWork, 4);
            BattleSetSeq(battleWork, 5, 0x5000000);
        } else if (seq > 0x3FFFFFF) {
            btlseqMove(battleWork);
        }
    }
}

void btlseqMove(void* battleWork) {
    extern void btlseqPhaseFirstProcess(void*);
    extern void BattleRunPhaseEvent(void*, s32);
    extern void BattleSetTotalHitDamage(void*, s32);
    extern u32 BattlePhaseEndCheck(void);
    void* unit;
    s32 seq;
    s32 i;
    s32 id;
    s32 advance;

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
                BattleIncSeq(battleWork, 5);
                BattleIncSeq(battleWork, 5);
                advance = 1;
                break;
            case 0x5000003:
            case 0x5000005:
                if (BattleWaitAllActiveEvtEnd(battleWork)) {
                    BattleIncSeq(battleWork, 5);
                    advance = 1;
                }
                break;
            case 0x5000004:
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(battleWork, i);
                    if (unit != NULL && *(void**)((s32)unit + 0x280) != NULL) {
                        BattleRunPhaseEvent(unit, 1);
                    }
                }
                BattleIncSeq(battleWork, 5);
                advance = 1;
                break;
            case 0x5000008:
                for (i = 0; i < 64; i++) {
                    unit = BattleGetUnitPtr(battleWork, i);
                    if (unit != NULL) {
                        *(u32*)((s32)unit + 0x27C) &= 0x80002000;
                        *(s32*)((s32)unit + 0x264) = 0;
                        *(s8*)((s32)unit + 0x270) = 0;
                        *(s32*)((s32)unit + 0x268) = 0;
                        *(s8*)((s32)unit + 0x271) = 0;
                    }
                }
                for (i = 0; i < 64; i++) {
                    id = *(s32*)((s32)battleWork + 0x430 + i * 4);
                    if (id != -1) {
                        unit = BattleGetUnitPtr(battleWork, id);
                        if (unit != NULL && *(s8*)((s32)unit + 0x22) > 0 &&
                            *(u8*)((s32)unit + 0x20) != 10) {
                            break;
                        }
                    }
                }
                *(s32*)((s32)battleWork + 0x420) = id;
                unit = BattleGetUnitPtr(battleWork, id);
                *(u32*)((s32)unit + 0x27C) |= 0x80000000;
                BattleIncSeq(battleWork, 5);
                BattleSetSeq(battleWork, 6, 0x6000000);
                break;
            case 0x5000009:
                btlseqAct(battleWork);
                break;
            case 0x500000A:
                if (!BattleCheckConcluded(battleWork)) {
                    for (i = 0; i < 64; i++) {
                        unit = BattleGetUnitPtr(battleWork, i);
                        if (unit != NULL) {
                            BattleSetTotalHitDamage(unit, 0);
                            *(u32*)((s32)unit + 0x27C) &= ~0x80000000;
                        }
                    }
                    if (!BattlePhaseEndCheck()) {
                        BattleSetSeq(battleWork, 5, 0x5000008);
                    }
                }
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
    extern void evtDeleteID(s32);
    extern s32 BattleStatusChangeMsgMain(void*);
    extern s32 BattleStatusChangeAnnouceMain(void*);
    extern void BattleStatusChangeMsgAdjust(void*);
    extern void BtlUnit_SetStatus(void*, s32, s32, s32);
    extern u8 btldefaultevt_BiribiriMove[];
    extern u8 btldefaultevt_CantMoveZeroGravity[];
    void* unit;
    void* event;
    s32 seq;
    s32 id;
    s32 kind;
    s32 messageBusy;
    s32 announceBusy;
    s32 unable;

    seq = BattleGetSeq(battleWork, 6);
    id = *(s32*)((s32)battleWork + 0x420);
    switch (seq) {
        case 0x6000000:
            unit = BattleGetUnitPtr(battleWork, id);
            effNiceAsync(1);
            *(s32*)((s32)unit + 0x258) = 0;
            *(void**)((s32)unit + 0x25C) = NULL;
            BattleAudience_ActInit();
            kind = *(s32*)((s32)unit + 8);
            unable = 1;
            if (kind == 0xDE || (kind > 0xDF && kind < 0xE7)) {
                if (!BtlUnit_CheckStatus(unit, 1) &&
                    !BtlUnit_CheckStatus(unit, 2) &&
                    !BtlUnit_CheckStatus(unit, 9) &&
                    !BtlUnit_CheckStatus(unit, 0x1B)) {
                    unable = 0;
                }
            }
            if (unable) {
                BattleSetSeq(battleWork, 6, 0x6000003);
            } else if (*(s8*)((s32)unit + 0x12B) == 0) {
                BattleCommandInit(battleWork);
                BattleSetSeq(battleWork, 6, 0x6000001);
            } else {
                BattleSetSeq(battleWork, 6, 0x6000005);
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
        case 0x6000003:
            unit = BattleGetUnitPtr(battleWork, id);
            *(s32*)((s32)unit + 0x2AC) = 1;
            *(u32*)((s32)unit + 0x1C) |= 0x04000000;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000004:
            unit = BattleGetUnitPtr(battleWork, id);
            *(void**)((s32)unit + 0x2A0) = BtlUnit_GetData(unit, 3);
            *(s32*)((s32)unit + 0x2AC) = 0x1B;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000005:
            unit = BattleGetUnitPtr(battleWork, id);
            *(void**)((s32)unit + 0x2A0) = BtlUnit_GetData(unit, 2);
            *(s32*)((s32)unit + 0x2AC) = 1;
            BattleSetSeq(battleWork, 6, 0x6000006);
            break;
        case 0x6000006:
            unit = BattleGetUnitPtr(battleWork, id);
            if ((*(u32*)((s32)unit + 0x2AC) & 1) == 0) {
                BattleStatusWindowSystemOff();
            }
            event = NULL;
            if ((*(u32*)((s32)battleWork + 0xEF4) & 0x40000000) != 0 &&
                (*(u32*)((s32)unit + 0x104) & 0x2000) == 0 &&
                (*(u32*)((s32)unit + 0x2AC) & 0x18) == 0) {
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
                       !BtlUnit_CheckStatus(unit, 0x1B)) {
                event = evtEntry(*(void**)((s32)unit + 0x2A0), 10, 0);
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
            if (messageBusy != 0 || announceBusy != 0) return;
            BattleIncSeq(battleWork, 6);
        case 0x6000009:
            if ((*(u32*)((s32)battleWork + 0x1720) & 0x30) == 0) {
                BattleStatusWindowSystemOn();
            }
            unit = BattleGetUnitPtr(battleWork, id);
            if (unit != NULL) {
                *(u32*)((s32)unit + 0x1C) &= 0xFBFFFFFF;
                if ((*(u32*)((s32)unit + 0x27C) & 8) != 0) {
                    BtlUnit_SetStatus(unit, 0x10, 1, -99);
                    *(u32*)((s32)unit + 0x27C) &= ~8;
                }
                if (*(u8*)((s32)unit + 0x20) == 0) {
                    if ((*(u32*)((s32)battleWork + 0x1720) & 0x2000) != 0) {
                        *(s8*)((s32)unit + 0x22) = 0;
                    } else {
                        (*(s8*)((s32)unit + 0x22))--;
                    }
                    if (*(s8*)((s32)unit + 0x22) < 1) {
                        *(u8*)((s32)unit + 0x20) = 10;
                    }
                }
            }
            BattleIncSeq(battleWork, 6);
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
    s32 variance;
    s32 i;

    BtlActRec_JudgeTurnRuleKeep();
    (*(s16*)battleWork)++;
    if (*(s16*)battleWork > 1) BattleMajinaiCheck();
    *(s32*)((s32)battleWork + 0x18FF4) = 0;
    *(s32*)((s32)battleWork + 0x18FF0) = 0;
    BtlActRec_AddCount((void*)((s32)battleWork + 0x16F57));
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            (*(u8*)((s32)unit + 0x24))++;
            BtlUnit_ResetMoveStatus(unit);
            kind = *(void**)((s32)unit + 0x10);
            variance = *(s8*)((s32)kind + 0x89);
            *(s32*)((s32)unit + 0x28) = *(s8*)((s32)kind + 0x88) + irand(variance) - variance / 2;
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
    for (i = 0; i < 0x40; i++) *(s32*)((s32)battleWork + 0x120 + i * 4) = -1;
    count = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            *(u8*)((s32)unit + 0x311) = 0xFF;
            if (*(u8*)((s32)unit + 0x20) != 10 || BtlUnit_CheckStatus(unit, 0x14) != 0) {
                if (*(s32*)((s32)unit + 0x2C) == BattleGetSeq(battleWork, 4) &&
                    (*(u32*)((s32)unit + 0x104) & 0x80000) == 0) {
                    *(s32*)((s32)battleWork + 0x120 + count * 4) = *(s32*)unit;
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


void battleSortPhaseMoveTable(void* battleWork, s32 arg) {
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    void* unitA;
    void* unitB;
    s32 count;
    s32 i;
    s32 j;
    s32 offset;
    s32 temp;
    f32 ax;
    f32 ay;
    f32 az;
    f32 bx;
    f32 by;
    f32 bz;

    count = arg;
    if (count <= 0) {
        count = 0;
        offset = 0;
        for (i = 0; i < 0x40; i++) {
            if (*(s32*)((s32)battleWork + 0x120 + offset) == -1) {
                break;
            }
            count++;
            offset += 4;
        }
    }

    for (i = 0; i < count - 1; i++) {
        offset = 0;
        for (j = 0; j < count - i - 1; j++) {
            unitA = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x120 + offset));
            unitB = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x124 + offset));
            BtlUnit_GetHomePos(unitA, &ax, &ay, &az);
            BtlUnit_GetHomePos(unitB, &bx, &by, &bz);
            if (__fabsf(ax) > __fabsf(bx)) {
                temp = *(s32*)((s32)battleWork + 0x120 + offset);
                *(s32*)((s32)battleWork + 0x120 + offset) = *(s32*)((s32)battleWork + 0x124 + offset);
                *(s32*)((s32)battleWork + 0x124 + offset) = temp;
            }
            offset += 4;
        }
    }

    for (i = 0; i < count - 1; i++) {
        offset = 0;
        for (j = 0; j < count - i - 1; j++) {
            unitA = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x120 + offset));
            unitB = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x124 + offset));
            if (*(s32*)((s32)unitA + 0x28) < *(s32*)((s32)unitB + 0x28)) {
                temp = *(s32*)((s32)battleWork + 0x120 + offset);
                *(s32*)((s32)battleWork + 0x120 + offset) = *(s32*)((s32)battleWork + 0x124 + offset);
                *(s32*)((s32)battleWork + 0x124 + offset) = temp;
            }
            offset += 4;
        }
    }
}

void battleMakePhaseEvtTable(void* battleWork) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    void* unit;
    s32* table;
    f32 x;
    f32 y;
    f32 z;
    s32 count;
    s32 i;
    s32 j;
    s32 temp;
    s32 tempPriority;

    table = (s32*)((s32)battleWork + 0x220);
    for (i = 0; i < 0x40; i++) {
        table[i * 2] = -1;
        table[i * 2 + 1] = 0;
    }
    count = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && (*(u32*)((s32)unit + 0x104) & 0x80000) == 0) {
            table[count * 2] = i;
            BtlUnit_GetHomePos(unit, &x, &y, &z);
            if (x < 0.0f) table[count * 2 + 1] = (s32)-x;
            else table[count * 2 + 1] = (s32)(1000.0f + x);
            count++;
        }
    }
    for (i = 0; (u32)i < (u32)(count - 1); i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (table[j * 2 + 1] > table[j * 2 + 3]) {
                temp = table[j * 2];
                tempPriority = table[j * 2 + 1];
                table[j * 2] = table[j * 2 + 2];
                table[j * 2 + 1] = table[j * 2 + 3];
                table[j * 2 + 2] = temp;
                table[j * 2 + 3] = tempPriority;
            }
        }
    }
}

s32 BattleWaitAllActiveEvtEnd(void* battleWork) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void*);
    extern s32 evtCheckID(s32);
    s32* eventId;

    if (!BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork)) return 0;
    eventId = (s32*)((s32)battleWork + 0x182B4);
    if (*eventId != 0) { if (evtCheckID(*eventId)) return 0; *eventId = 0; }
    eventId = (s32*)((s32)battleWork + 0x182C4);
    if (*eventId != 0) { if (evtCheckID(*eventId)) return 0; *eventId = 0; }
    eventId = (s32*)((s32)battleWork + 0x180FC);
    if (*eventId != 0) { if (evtCheckID(*eventId)) return 0; *eventId = 0; }
    eventId = (s32*)((s32)battleWork + 0x182C8);
    if (*eventId != 0) { if (evtCheckID(*eventId)) return 0; *eventId = 0; }
    eventId = (s32*)((s32)battleWork + 0x182C0);
    if (*eventId != 0) { if (evtCheckID(*eventId)) return 0; *eventId = 0; }
    return 1;
}

int BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 evtCheckID(s32);
    extern s32 BattleAfterReactionMain(void);
    void* unit;
    s32 result;
    s32 i;
    s32 eventId;

    result = 1;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0) {
            eventId = *(s32*)((s32)unit + 0x29C);
            if (eventId != 0) {
                if (evtCheckID(eventId)) result = 0;
                else *(s32*)((s32)unit + 0x29C) = 0;
            }
            eventId = *(s32*)((s32)unit + 0x2A8);
            if (eventId != 0) {
                if (evtCheckID(eventId)) result = 0;
                else *(s32*)((s32)unit + 0x2A8) = 0;
            }
            eventId = *(s32*)((s32)unit + 0x2B4);
            if (eventId != 0) {
                if (evtCheckID(eventId)) result = 0;
                else *(s32*)((s32)unit + 0x2B4) = 0;
            }
            eventId = *(s32*)((s32)unit + 0x2BC);
            if (eventId != 0) {
                if (evtCheckID(eventId)) result = 0;
                else *(s32*)((s32)unit + 0x2BC) = 0;
            }
        }
    }
    if (result != 0 && BattleAfterReactionMain() != 0) result = 0;
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
    extern f32 float_0_80422568;
    extern f32 float_1_8042256c;
    extern f32 float_90_80422578;
    extern f32 float_100_8042257c;
    extern f32 float_110_80422574;
    extern char str_A1_80422564[];
    extern char str_A2_80422570[];
    extern void* mapGetMapObj(char* name);
    extern void mapObjScale(char* name, f32 x, f32 y, f32 z);
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern void _mapobj_data_touch_scale(s32 id);

    void* battleWork = _battleWorkPointer;
    s32 touched = 0;
    s32 i;
    s32 offset;
    f32 sx;
    f32 sy;
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
