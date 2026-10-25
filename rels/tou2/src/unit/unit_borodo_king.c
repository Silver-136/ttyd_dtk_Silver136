#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "mario/mario_pouch.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

extern ItemData itemDataTable[];
void BtlUnit_EquipItem(BattleUnit* unit, s32 flags, ItemType type);

USER_FUNC(btlevtcmd_CheckUnitFlag);
#ifdef VERSION_G8ME01
#endif
static USER_FUNC(_steal_item);

BattleUnitKind unit_borodo_king;
static BattleWeapon weapon_borodo_attack;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[20];
static PoseTableEntry pose_table_item_have[18];
#ifdef VERSION_G8ME01
static BattleDataEntry data_table[2];
#else
static BattleDataEntry data_table[1];
#endif
static BattleUnitKindPart parts[1];
static EvtWord init_event[57];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[59];
static EvtWord _borodo_normal_attack[503];
static EvtWord _borodo_steal_attack[610];
static EvtWord escape_event[115];
static EvtWord wait_event[26];

static USER_FUNC(_steal_item) {
    EvtWord* args = event->args;
    s32 coins;
    s32 num;
    PouchData* pouch = pouchGetPtr();
    BattleUnit* unit;
    BattleUnit* target;
    s32 itemVar;
    s32 iconVar;
    s32 numVar;
    s32 id;
    s32 tid;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    tid = evtGetValue(event, args[1]);
    tid = BattleTransID(event, tid);
    target = BattleGetUnitPtr(_battleWorkPointer, tid);
    if (target == NULL) {
        return EVT_RETURN_DONE;
    }
    itemVar = args[2];
    iconVar = args[3];
    numVar = args[4];
    evtSetValue(event, itemVar, 0);
    evtSetValue(event, iconVar, 0);
    evtSetValue(event, numVar, 0);
    if (irand(100) < 40) {
        coins = pouchGetCoin();
        num = 10;
        if (coins < 10) {
            num = coins;
        }
        if (num > 0) {
            evtSetValue(event, itemVar, ITEM_COIN);
#ifdef VERSION_G8ME01
            evtSetValue(event, iconVar, 403);
#else
            evtSetValue(event, iconVar, 395);
#endif
            evtSetValue(event, numVar, num);
            unit->heldItem = ITEM_COIN;
            unit->stolenCoins = num;
            pouchSetCoin(coins - num);
        }
        return EVT_RETURN_DONE;
    }
    {
        s32 item = 0;
        s32 count = 0;
        s32 r;
        s32 i;

        for (i = 0; i < 20; i++) {
            if (pouch->heldItems[i] != 0) {
                count++;
            }
        }
        if (count <= 0) {
            return EVT_RETURN_DONE;
        }
        r = irand(count);
        if (r >= count) {
            s32 j;

            r -= count;
            for (j = 0; j < 200; j++) {
                if (pouch->badges[j] != 0) {
                    if (--r < 0) {
                        item = pouch->badges[j];
                        break;
                    }
                }
            }
        } else {
            s32 k;

            for (k = 0; k < 20; k++) {
                if (pouch->heldItems[k] != 0) {
                    if (--r < 0) {
                        item = pouch->heldItems[k];
                        break;
                    }
                }
            }
        }
        if (item != 0) {
            if (!pouchRemoveItem(item)) {
                return EVT_RETURN_DONE;
            }
            unit->heldItem = item;
            if (item >= 240 && item < 339) {
                if (target->currentKind == UNIT_MARIO) {
                    BtlUnit_EquipItem(target, 3, 0);
                } else if (target->currentKind >= TYPE_PARTNER_MIN && target->currentKind < TYPE_PARTNER_MAX) {
                    BtlUnit_EquipItem(target, 5, 0);
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
            }
            evtSetValue(event, itemVar, item);
            evtSetValue(event, iconVar, itemDataTable[item].icon_id);
        }
    }
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_borodo_king = {
    45,
    "btl_un_borodo_king",
    8,
    0,
    1,
    1,
    13,
    0,
    3,
    70,
    1,
    50,
    9999,
    26,
    34,
    0,
    34,
    0.0f,
    17.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    0.0f,
    9.0f,
    26.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    26.0f,
    34.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BORODO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleWeapon weapon_borodo_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    7, // special
    2, // counterResistFlags
    0x80002004, // targetWeightFlags
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {90, 80, 60, 100, 90, 100, 100, 60, 100, 90, 100, 90, 100, 95, 80, 85, 70, 100, 80, 100, 100, 95};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "S_1"},
    {39, "D_1"},
    {50, "A_1"},
    {51, "A_2"},
    {42, "R_1"},
    {41, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

static PoseTableEntry pose_table_item_have[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {28, "S_2"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "S_2"},
    {39, "D_1"},
    {50, "A_1"},
    {51, "A_2"},
    {42, "R_2"},
    {41, "R_2"},
    {40, "W_2"},
    {65, "S_2"},
    {69, "S_2"},
};

#ifdef VERSION_G8ME01
extern EvtWord _borodo_king_emergency_revival_event[];
static BattleDataEntry data_table[] = {
    {51, _borodo_king_emergency_revival_event},
#else
static BattleDataEntry data_table[] = {
#endif
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_borodo_king", "c_borodo_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_CheckUnitFlag, -2, 1073741824, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
        ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_CheckUnitFlag, -2, 1073741824, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(escape_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(_borodo_normal_attack))
        RETURN
    END_IF
    SET_VALUE(LW(1), 215)
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    IF_INT_LESS(LW(0), 75)
        CALL_CHILD(PTR(_borodo_normal_attack))
        ELSE
        CALL_CHILD(PTR(_borodo_steal_attack))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(_borodo_normal_attack)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 50)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 50)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.25f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("O_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(5), 0)
            SET_VALUE(LW(6), 360)
        ELSE
            SET_VALUE(LW(5), 360)
            SET_VALUE(LW(6), 0)
        END_IF
        CALL(btlevtcmd_SetRotateOffsetFromCenterOffset, -2)
        CALL(evt_sub_intpl_init, 13, LW(5), LW(6), 24)
        LOOP(25)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 36, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("X_1"))
    WAIT_MS(1000)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(_borodo_steal_attack)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 50)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 50)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_borodo_attack), 256, LW(5))
    SET_VALUE(LW(8), 0)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        CALL(_steal_item, -2, LW(3), LW(8), LW(9), LW(7))
        IF_INT_NOT_EQUAL(LW(8), 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_SUCCESS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            BEGIN_INLINE_EVENT
                WAIT_FRAMES(20)
                CALL(btlevtcmd_AnnounceSetParam, 0, LW(8))
                CALL(btlevtcmd_AnnounceMessage, 1, 0, 0, STRING("btl_msg_steal_item_lost"), 90)
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconEntry, LW(9), LW(0), LW(1), LW(2), LW(10))
                CALL(btlevtcmd_BtlIconSetFallAccel, LW(10), FLOAT(0.5f))
                CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
                CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconJumpPosition, LW(10), LW(0), LW(1), LW(2), 20)
                CALL(btlevtcmd_BtlIconDelete, LW(10))
                CALL(btlevtcmd_OnUnitFlag, -2, 1073741824)
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("O_1"))
    WAIT_FRAMES(10)
    IF_INT_EQUAL(LW(8), 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_INCLINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("B_1"))
        WAIT_FRAMES(52)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("R_4"))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("O_2"))
    WAIT_FRAMES(14)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_LAUGH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_item_have))
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(escape_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_WAIT1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("R_3"))
    CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_ENM_BORODO_JUMP1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), FLOAT(20.0f), -2)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 255, 100, 100, 100, 50, 0)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 67108864)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BORODO_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, -2, 300, LW(1), LW(2), 0, 0, 0)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_CheckUnitFlag, -2, 1073741824, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
        ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()
#ifdef VERSION_G8ME01
EVT_BEGIN(_borodo_king_emergency_revival_event)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 134217728)
    SET_VALUE(LW(0), 48)
    LABEL(5)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(5)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(6)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(6)
    END_IF
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 120)
    END_SIBLING_EVENT
    WAIT_FRAMES(90)
    SET_VALUE(LW(0), 30)
    LABEL(7)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), -3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(7)
    END_IF
    CALL(btlevtcmd_OnOffStatus, LW(10), 27, 0, 0, 0)
    CALL(btlevtcmd_SetHp, LW(10), 0)
    CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_OffUnitFlag, LW(10), 134217728)
    CALL(btlevtcmd_OffUnitFlag, LW(10), 1073741824)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

