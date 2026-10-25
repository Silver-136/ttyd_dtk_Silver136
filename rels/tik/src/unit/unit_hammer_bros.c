#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static USER_FUNC(hammer_bros_nagemakuri_set_target);
static void hammer_bros_get_next_target(s32* unitIdx, s32* partIdx);
static void hammer_bros_get_target(s32* unitIdx, s32* partIdx);

BattleUnitKind unit_hammer_bros;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[6];
static BattleWeapon weapon_hammer_bros_attack;
static BattleWeapon weapon_hammer_bros_renzoku_attack;
static BattleWeapon weapon_hammer_bros_nagemakuri_attack;
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord attack_event[96];
static EvtWord start_avoid[26];
static EvtWord hammer_bros_attack_common_event1[254];
static EvtWord hammer_bros_attack_common_event2[112];
static EvtWord hammer_bros_attack_common_event3[59];
static EvtWord damage_check_event_sub[137];
static EvtWord normal_attack_event[112];
static EvtWord renzoku_attack_event[111];
static EvtWord renzoku_attack_event_sub_no_last[13];
static EvtWord renzoku_attack_event_sub_last[39];
static EvtWord nagemakuri_attack_event[186];
static EvtWord nagemakuri_attack_event_sub[49];
static EvtWord nagemakuri_attack_event_sub_no_last[13];
static EvtWord nagemakuri_attack_event_sub_last[76];
static EvtWord wait_event[6];

static void hammer_bros_get_next_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;

    wt->currentTarget++;
    if (wt->currentTarget < wt->numTargets) {
        BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];
        *unitIdx = target->unitIdx;
        *partIdx = target->partIdx;
    } else {
        *unitIdx = -1;
        *partIdx = 0;
    }
}

static void hammer_bros_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

static USER_FUNC(hammer_bros_nagemakuri_set_target) {
    EvtWord* args = event->args;
    s32 unitIdx[20];
    s32 partIdx[20];
    s32 i;
    s32 num;

    i = 0;
    _battleWorkPointer->weaponTargets.currentTarget = 0;
    hammer_bros_get_target(&unitIdx[0], &partIdx[0]);
    while (TRUE) {
        if (unitIdx[i] == -1) {
            num = i;
            break;
        }
        i++;
        hammer_bros_get_next_target(&unitIdx[i], &partIdx[i]);
    }
    for (i = 0; i < 5; i++) {
        s32 r = irand(num);
        evtSetValue(event, *args++, unitIdx[r]);
        evtSetValue(event, *args++, partIdx[r]);
    }
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_hammer_bros = {
    UNIT_HAMMER_BRO, // mUnitType
    "btl_un_hammer_bros", // name
    7, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    26, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    36, // width
    44, // height
    {3, 44}, // hitOffset
    {0.0f, 22.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 28.0f, 0.0f}, // kissHitOffset
    {0.0f, 22.0f, 0.0f}, // cutBaseOffset
    36.0f, // cutWidth
    44.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_HUMMER_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    6, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    80, // sleep
    80, // stop
    70, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    70, // fright
    60, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "BRO_N_1"},
    {POSE_TYPE_STOP, "BRO_Y_1"},
    {POSE_TYPE_FREEZE, "BRO_Y_1"},
    {POSE_TYPE_CONFUSE, "BRO_K_1"},
    {POSE_TYPE_POISON, "BRO_X_1"},
    {POSE_TYPE_DIZZY, "BRO_X_1"},
    {POSE_TYPE_KO, "BRO_D_1"},
    {POSE_TYPE_NORMAL, "BRO_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "BRO_Q_1"},
    {30, "BRO_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BRO_S_1"},
    {POSE_TYPE_DAMAGE, "BRO_D_1"},
    {POSE_TYPE_RUN, "BRO_R_1"},
    {POSE_TYPE_WALK, "BRO_W_1"},
    {POSE_TYPE_DANGER, "BRO_X_1"},
    {POSE_TYPE_PERIL, "BRO_X_1"},
    {POSE_TYPE_TALK, "BRO_T_1"},
    {POSE_TYPE_ACTION, "BRO_S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {6, "btl_un_hammer_bros", "c_burosu_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static BattleWeapon weapon_hammer_bros_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_hammer_bros_renzoku_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_hammer_bros_nagemakuri_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 13, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 14, 0)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 3)
        SET_VALUE(LW(0), 50)
        ADD(LW(0), 50)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 50)
            CALL_CHILD(PTR(renzoku_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(nagemakuri_attack_event))
        GOTO(99)
        ELSE
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(hammer_bros_attack_common_event1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
    MULTIPLY(LW(0), 10)
    ADD(LW(6), LW(0))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -135)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 5, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(10)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 32)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            ELSE
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 60, 0, FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 60, -1)
    CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(6), 100)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), 0, LW(8), 0, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(6), 150)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, FLOAT(4.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), 0, LW(8), 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    RETURN
EVT_END()

static EVT_BEGIN(hammer_bros_attack_common_event2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
    MULTIPLY(LW(0), 10)
    ADD(LW(6), LW(0))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -135)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 5, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HUMMER_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(10)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 32)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 60, 0, FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 60, -1)
    RETURN
EVT_END()

static EVT_BEGIN(hammer_bros_attack_common_event3)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 32)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 50)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, FLOAT(2.0f), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), 0, LW(8), 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_event_sub)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL_CHILD(PTR(hammer_bros_attack_common_event1))
    GOTO(99)
    LABEL(91)
    IF_INT_EQUAL(LW(11), 1)
        IF_INT_EQUAL(LW(9), PTR(&weapon_hammer_bros_renzoku_attack))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 1)
            ELSE
            SET_VALUE(LW(13), 0)
            SET_VALUE(LW(14), 5)
            LOOP(0)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(5))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(14), LW(6))
                IF_INT_EQUAL(LW(3), LW(5))
                    IF_INT_EQUAL(LW(4), LW(6))
                        ADD(LW(13), 10)
                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(13), 1)
                        BREAK_LOOP
                    END_IF
                END_IF
                ADD(LW(13), 1)
                ADD(LW(14), 1)
            END_LOOP
        END_IF
    END_IF
    CALL_CHILD(PTR(hammer_bros_attack_common_event2))
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 256, LW(5))
        ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    END_IF
    CALL_CHILD(PTR(hammer_bros_attack_common_event3))
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_hammer_bros_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_hammer_bros_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_hammer_bros_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_hammer_bros_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_1A"))
    WAIT_FRAMES(27)
    SET_VALUE(LW(15), 2)
    SET_VALUE(LW(9), PTR(&weapon_hammer_bros_attack))
    SET_VALUE(LW(10), 1)
    SET_VALUE(LW(11), 0)
    CALL_CHILD(PTR(damage_check_event_sub))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_hammer_bros_renzoku_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_hammer_bros_renzoku_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_hammer_bros_renzoku_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_hammer_bros_renzoku_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(14), 0)
    SET_VALUE(LW(15), 2)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(14), 4)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_1A"))
        WAIT_FRAMES(27)
        IF_INT_EQUAL(LW(14), 3)
            SPAWN_EVENT(PTR(renzoku_attack_event_sub_last))
            ELSE
            SPAWN_EVENT(PTR(renzoku_attack_event_sub_no_last))
        END_IF
        WAIT_MS(250)
        ADD(LW(14), 1)
        ADD(LW(15), 1)
    END_LOOP
    LABEL(98)
    WAIT_MS(1500)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event_sub_no_last)
    SET_VALUE(LW(9), PTR(&weapon_hammer_bros_renzoku_attack))
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(damage_check_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event_sub_last)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(hammer_bros_attack_common_event2))
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_hammer_bros_renzoku_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_hammer_bros_renzoku_attack), 256, LW(5))
        CALL_CHILD(PTR(hammer_bros_attack_common_event3))
        ELSE
        SET_VALUE(LW(9), PTR(&weapon_hammer_bros_renzoku_attack))
        SET_VALUE(LW(10), 1)
        SET_VALUE(LW(11), 0)
        CALL_CHILD(PTR(damage_check_event_sub))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(nagemakuri_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_hammer_bros_nagemakuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_hammer_bros_nagemakuri_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_hammer_bros_nagemakuri_attack))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_hammer_bros_nagemakuri_attack))
    CALL(hammer_bros_nagemakuri_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, LW(6))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, LW(8))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 6, LW(3))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, LW(5))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, LW(7))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, LW(9))
    SET_VALUE(LW(12), 0)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    SET_VALUE(LW(15), 2)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(12), 4)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(3))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(14), LW(4))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(10))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(10))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_1A"))
        WAIT_FRAMES(27)
        SPAWN_EVENT(PTR(nagemakuri_attack_event_sub))
        WAIT_MS(250)
        ADD(LW(12), 1)
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        ADD(LW(15), 1)
    END_LOOP
    LABEL(98)
    WAIT_MS(1500)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(nagemakuri_attack_event_sub)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(14), LW(4))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(13), 3)
            CALL_CHILD(PTR(nagemakuri_attack_event_sub_last))
            BREAK_LOOP
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(5))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                CALL_CHILD(PTR(nagemakuri_attack_event_sub_no_last))
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(nagemakuri_attack_event_sub_no_last)
    SET_VALUE(LW(9), PTR(&weapon_hammer_bros_nagemakuri_attack))
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(damage_check_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(nagemakuri_attack_event_sub_last)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(5))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                ADD(LW(13), 10)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(13), LW(0))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
    END_LOOP
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(hammer_bros_attack_common_event2))
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_hammer_bros_nagemakuri_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_hammer_bros_nagemakuri_attack), 256, LW(5))
        CALL_CHILD(PTR(hammer_bros_attack_common_event3))
        ELSE
        SET_VALUE(LW(9), PTR(&weapon_hammer_bros_nagemakuri_attack))
        SET_VALUE(LW(10), 1)
        SET_VALUE(LW(11), 0)
        CALL_CHILD(PTR(damage_check_event_sub))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

