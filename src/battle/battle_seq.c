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


void btlseqAct(void* battleWork) {
    ;
}


void btlseqFirstAct(void* battleWork) {
    ;
}


void btlseqMove(void* battleWork) {
    ;
}


u8 _set_haikei_entry_scale(void) {
    return 0;
}


void BattleCheckAllPinchStatus(void* battleWork, int param_2) {
    ;
}


s32 _set_effect_luck(void) {
    return 0;
}


void _rule_disp(void) {
    ;
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

s32 BattleCheckConcluded(void* battleWork) {
    return 0;
}


void battleMakePhaseEvtTable(void* battleWork) {
    ;
}


int BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork) {
    return 0;
}


void btlseqPhaseFirstProcess(void* battleWork) {
    ;
}


s32 BattleWaitAllActiveEvtEnd(void* battleWork) {
    return 0;
}


void btlseqTurnFirstProcess(void* battleWork) {
    ;
}


void BattleSequenceManager(void) {
    ;
}


u32 BattlePhaseEndCheck(void) {
    s32 i;
    s32 offset;
    void* battleWork;
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
                *(u8*)((s32)unit + 0x20) != 0xA &&
                *(s8*)((s32)unit + 0x22) > 0 &&
                BtlUnit_CheckStatus(unit, 0x1B) == 0) {
                break;
            }
        }
        i++;
        offset += 4;
    }

    return i >= 0x40;
}


void btlseqPhase(void* battleWork) {
    ;
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

