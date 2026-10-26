#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_monosiri.h"
#include "battle/battle_sub.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "system.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_camera.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_damage.h"

void* monosiriInit(BattleUnitType kind, f32 x, f32 y);
void monosiriMain(void* work);
void monosiriClose(void* work);
BOOL monosiriCloseWait(void* work);
void monosiriExit(void* work);
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "icon_ids.h"

USER_FUNC(_dictionary);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(_set_hustle);
USER_FUNC(_monosiri_flag_on);
USER_FUNC(btlevtcmd_get_monosiri_msg_no);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);
static USER_FUNC(krb_get_dir);

PoseTableEntry pose_table_christine_stay[25];
static BattleDataEntry data_table[8];
BattleUnitKindPart unitpartsdata_Party_Christine[1];
BattleUnitKind unitdata_Party_Christine;
static EvtWord battle_entry_event[59];
static EvtWord init_event[51];
static EvtWord damage_event[16];
BattleWeapon partyWeapon_ChristineNormalAttack;
BattleWeapon partyWeapon_ChristineMonosiri;
BattleWeapon partyWeapon_ChristineRenzokuAttack;
BattleWeapon partyWeapon_ChristineKiss;
EvtWord partyChristineAttack_NormalAttack[824];
EvtWord christine_dictionary_event[26];
EvtWord partyChristineAttack_Monosiri[397];
#ifdef VERSION_G8ME01
EvtWord partyChristineAttack_RenzokuAttack[1078];
#else
EvtWord partyChristineAttack_RenzokuAttack[1071];
#endif
EvtWord partyChristineAttack_Kiss[546];
EvtWord christine_appeal_event[71];
static EvtWord party_win_reaction[14];
static EvtWord attack_audience_event[392];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];









PoseTableEntry pose_table_christine_stay[] = {
    {POSE_TYPE_FREEZE, "PKR_D_5"},
    {POSE_TYPE_STOP, "PKR_D_5"},
    {POSE_TYPE_DIZZY, "PKR_X_1"},
    {POSE_TYPE_POISON, "PKR_D_2"},
    {POSE_TYPE_CONFUSE, "PKR_X_1"},
    {POSE_TYPE_SLEEP, "PKR_N_1"},
    {POSE_TYPE_KO, "PKR_D_3"},
    {POSE_TYPE_NORMAL, "PKR_S_1"},
    {37, "PKR_G_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PKR_G_1"},
    {POSE_TYPE_DAMAGE, "PKR_D_1"},
    {POSE_TYPE_WALK, "PKR_W_1"},
    {POSE_TYPE_RUN, "PKR_R_1"},
    {POSE_TYPE_PERIL, "PKR_D_2"},
    {POSE_TYPE_DANGER, "PKR_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PKR_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PKR_D_2"},
    {POSE_TYPE_TALK, "PKR_T_1"},
    {POSE_TYPE_ITEM, "PKR_I_1"},
    {POSE_TYPE_COMMAND, "PKR_C_1"},
    {POSE_TYPE_APPEAL, "PKR_O_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PKR_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PKR_A_1"},
    {POSE_TYPE_DEFEND, "PKR_G_1"},
    {POSE_TYPE_ACTION, "PKR_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, christine_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

BattleUnitKindPart unitpartsdata_Party_Christine[] = {
    {1, "btl_un_christine", "c_pkr", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_christine_stay},
};

BattleUnitKind unitdata_Party_Christine = {
    UNIT_GOOMBELLA, // mUnitType
    "btl_un_christine", // name
    10, // maxHp
    5, // maxFp
    5, // mDangerHP
    1, // mPerilHP
    1, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    20, // width
    20, // height
    {10, 24}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 30.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 30.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {10.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 13.0f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_MARIO_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_MARIO_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_MARIO_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    unitpartsdata_Party_Christine, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 9, 3, 0, 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -2, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

BattleWeapon partyWeapon_ChristineNormalAttack = {
    "btl_wn_pkr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pkr_normal_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_zutsuki", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChristineAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChristineMonosiri = {
    "btl_wn_pkr_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pkr_monosiri", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_TATTLE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_monoshiri", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChristineAttack_Monosiri, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_ChristineRenzokuAttack = {
    "btl_wn_pkr_lv2", // name
    ICON_PARTNER_MOVE_LV2, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pkr_renzoku_zutsuki", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#ifdef VERSION_G8ME01
    1, // mStylishCommandMultiplier
#else
    2, // mStylishCommandMultiplier
#endif
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_zutsuki", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChristineAttack_RenzokuAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 5, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 5, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChristineKiss = {
    "btl_wn_pkr_lv3", // name
    ICON_PARTNER_MOVE_LV3, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pkr_nage_kiss", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kiss", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, // chargeStrength
    100, // fastChance
    2, // fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    partyChristineAttack_Kiss, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

EVT_BEGIN(partyChristineAttack_NormalAttack)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(14))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(14), -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
        ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
        BEGIN_SIBLING_EVENT
#ifdef VERSION_G8ME01
            WAIT_FRAMES(2)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 7, 7, 0)
#else
            WAIT_FRAMES(5)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 4, 4, 0)
#endif
        END_SIBLING_EVENT
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(15)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(15)
            LOOP(6)
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                MULTIPLY(LW(0), -30)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 3, 0)
        ELSE
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_EQUAL(LW(6), 2)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
                IF_INT_EQUAL(LW(15), 1)
                    SET_VALUE(LW(9), -15)
                    ELSE
                    SET_VALUE(LW(9), 15)
                END_IF
                CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
                SET_VALUE(LW(8), 0)
                LOOP(36)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(9))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            ELSE
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
                SET_VALUE(LW(8), 0)
                LOOP(36)
                    CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(12), LW(13), LW(14))
                    CALL(krb_get_dir, LW(5), LW(6), LW(12), LW(13), LW(8))
                    CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(8))
                    SET_FLOAT_VALUE(LW(5), LW(12))
                    SET_FLOAT_VALUE(LW(6), LW(13))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
        END_IF
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 0, 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_S_1"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
        WAIT_MS(500)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        END_IF
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS_OR_EQUAL(LW(6), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        ADD(LW(10), 1)
        GOTO(10)
        ELSE
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultAC, LW(0))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        IF_INT_LESS(LW(6), 0)
            MULTIPLY(LW(6), -1)
        END_IF
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        DIVIDE(LW(6), 15)
        SET_VALUE(LW(7), 15)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(christine_dictionary_event)
    LABEL(0)
    CALL(_dictionary, LW(3), LW(4))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(partyChristineAttack_Monosiri)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(evt_btl_camera_set_posoffset, 0, 40, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_2A_1"))
    WAIT_MS(500)
    LOOP(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_2A_2"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_LEARNED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(400)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_2A_3"))
        WAIT_MS(350)
    END_LOOP
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, LW(3), LW(4), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 21, 1, 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    END_SIBLING_EVENT
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff_fukidashi, 2, STRING(""), 0, 2, 0, LW(0), LW(1), LW(2), 30, 0, 56)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_FAIL_LEARNED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_2A_4"))
        WAIT_FRAMES(56)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_LEARNED2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_2A_4"))
    WAIT_FRAMES(3)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(53)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftof, 0, LW(0))
    CALL(btlevtcmd_ftof, 53, LW(1))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(0), LW(1), LW(1), 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_GREATER_OR_EQUAL(LW(6), 2)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_Y_1"))
        CALL(btlevtcmd_ftomsec, 28, LW(0))
        WAIT_MS(LW(0))
        ELSE
        CALL(evt_audience_acrobat_notry)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
    END_IF
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("PKR_2A_5"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("PKR_2A_5"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    SPAWN_EVENT(christine_dictionary_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    LABEL(10)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 3)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_get_monosiri_msg_no, LW(3), LW(4), LW(0))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, LW(0), 0, -2)
    CALL(_monosiri_flag_on, LW(3))
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("PKR_T_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("PKR_S_1"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 4)
    LABEL(20)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(20)
    END_IF
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyChristineAttack_RenzokuAttack)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 10, 10, 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
        ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(15)
            LOOP(6)
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                MULTIPLY(LW(0), -30)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(15)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 3, 0)
        ELSE
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        SET_VALUE(LW(8), 40)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        BEGIN_SIBLING_EVENT
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
#ifdef VERSION_G8ME01
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
#endif
                WAIT_FRAMES(20)
            END_IF
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
            SET_VALUE(LW(15), 0)
            LOOP(LW(8))
                CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(12), LW(13), LW(14))
                CALL(krb_get_dir, LW(5), LW(6), LW(12), LW(13), LW(15))
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(9))
                ADD(LW(15), LW(9))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(15))
                SET_FLOAT_VALUE(LW(5), LW(12))
                SET_FLOAT_VALUE(LW(6), LW(13))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_INLINE_EVENT
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                WAIT_FRAMES(20)
            END_IF
            WAIT_FRAMES(15)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_INLINE_EVENT
        IF_INT_GREATER_OR_EQUAL(LW(6), 2)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
                MULTIPLY(LW(5), -18)
                LOOP(20)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(5))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, 0)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(evt_eff64, STRING(""), STRING("kemuri2_n64"), 2, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            END_SIBLING_EVENT
            SET_VALUE(LW(9), LW(8))
            SUBTRACT(LW(9), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(9))
            BEGIN_SIBLING_EVENT
                LOOP(LW(9))
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
                    SUBTRACT(LW(0), 1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(0))
                END_LOOP
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                LOOP(1)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
                    IF_INT_GREATER(LW(0), 15)
                        SUBTRACT(LW(0), 15)
                        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, LW(0), LW(0), 0)
                        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
                        SWITCH(LW(6))
                            CASE_GREATER_OR_EQUAL(2)
                            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
                            LOOP(15)
                                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 24, 0)
                                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(0))
                                CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(1))
                                IF_INT_GREATER_OR_EQUAL(LW(1), 0)
                                    ADD(LW(0), -24)
                                    ELSE
                                    ADD(LW(0), 24)
                                END_IF
                                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, LW(0))
                                WAIT_FRAMES(1)
                            END_LOOP
                            CASE_DEFAULT
                        END_SWITCH
                    END_IF
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            ADD(LW(2), 5)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 0, 0)
            DELETE_EVENT(15)
            ELSE
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(evt_eff64, STRING(""), STRING("kemuri2_n64"), 2, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            ADD(LW(2), 5)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 0, 0)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_S_1"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
        WAIT_MS(500)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        IF_INT_LESS_OR_EQUAL(LW(10), 0)
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
            ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(mario_get_renzoku_count_max, LW(3), LW(6), LW(7))
    IF_INT_LESS(LW(6), LW(7))
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        ADD(LW(10), 1)
        CALL(btlevtcmd_StartAC, 0)
        GOTO(10)
        ELSE
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        IF_INT_LESS(LW(6), 0)
            MULTIPLY(LW(6), -1)
        END_IF
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        DIVIDE(LW(6), 15)
        SET_VALUE(LW(7), 15)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyChristineAttack_Kiss)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CalculateFaceDirection, LW(3), LW(4), -2, -1, 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, LW(3), LW(15))
    END_IF
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 27)
        SET_VALUE(LW(1), 20)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 25)
        SET_VALUE(LW(1), 28)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 23)
        SET_VALUE(LW(1), 30)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 23)
        SET_VALUE(LW(1), 30)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 23)
        SET_VALUE(LW(1), 30)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 23)
        SET_VALUE(LW(1), 30)
        CASE_DEFAULT
        SET_VALUE(LW(0), 23)
        SET_VALUE(LW(1), 32)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 16, 220, 70, 0, 0, LW(0), LW(1), 0)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 5, 3, 60)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 12)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(6), 12)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 200, -1, 0)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(6))
        ADD(LW(1), LW(6))
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(9), STRING("PKR_O_1"))
    WAIT_MS(200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 1, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        IF_BITS_SET(LW(6), 2)
            CALL_CHILD(subsetevt_check_target_movable)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_KISSED_CHRISTINE)
            END_IF
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            ADD(LW(2), 15)
            CALL(btlevtcmd_GetHeight, LW(3), LW(15))
            CALL(btlevtcmd_GetStatusMg, LW(3), LW(14))
            MULTIPLY_FLOAT(LW(15), LW(14))
            DIVIDE(LW(15), 2)
            ADD(LW(1), LW(15))
            CALL(evt_eff, 0, STRING("recovery"), 6, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_KURI_CHEER_KISS1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(40)
            CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, PTR(&partyWeapon_ChristineKiss), LW(6))
            CALL(_set_hustle, LW(3))
            ELSE
            WAIT_FRAMES(40)
            CALL(btlevtcmd_AudienceDeclareACResult, PTR(&partyWeapon_ChristineKiss), -1)
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_ChristineKiss), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_STATUS, LW(5))
        END_IF
    END_INLINE_EVENT
    CALL(btlevtcmd_ftof, 24, LW(0))
    CALL(btlevtcmd_ftof, 48, LW(1))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(0), LW(1), LW(1), 0)
    CALL(btlevtcmd_ACRGetResult, LW(7), LW(8))
    IF_INT_GREATER_OR_EQUAL(LW(7), 2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1B"))
        WAIT_MS(166)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_Y_1"))
        WAIT_MS(167)
        ELSE
        CALL(evt_audience_acrobat_notry)
        WAIT_FRAMES(20)
    END_IF
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("msg_st_chg_mario_quick"), 60)
        ELSE
        WAIT_MS(500)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_ResetFaceDirection, LW(3))
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_IF
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(christine_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_APPEAL)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_APPEAL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 1, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(667)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_Y_1"))
    WAIT_FRAMES(64)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(3), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(4))
    SET_VALUE(LW(8), LW(3))
    MULTIPLY(LW(8), LW(4))
    MULTIPLY(LW(8), -1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(8), LW(1), LW(2), 0, -1, 0)
    WAIT_FRAMES(20)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_AUDIENCE)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(36)
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(12), LW(13), LW(14))
            CALL(krb_get_dir, LW(5), LW(6), LW(12), LW(13), LW(8))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(8))
            SET_FLOAT_VALUE(LW(5), LW(12))
            SET_FLOAT_VALUE(LW(6), LW(13))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(15)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 28)
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, 0)
    WAIT_FRAMES(2)
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        IF_INT_LESS(LW(6), 0)
            MULTIPLY(LW(6), -1)
        END_IF
        DIVIDE(LW(6), 15)
        SET_VALUE(LW(7), 15)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(6), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static USER_FUNC(krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1, y1, x2, y2;
    f32 dx, dy;
    s32 value;

    x1 = evtGetFloat(event, args[0]);
    y1 = evtGetFloat(event, args[1]);
    x2 = evtGetFloat(event, args[2]);
    y2 = evtGetFloat(event, args[3]);
    value = evtGetValue(event, args[4]);
    dx = x2 - x1;
    dy = y2 - y1;
    if (dx == 0.0f && dy == 0.0f) {
        evtSetValue(event, args[4], value);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dy, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}

static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_ChristineNormalAttack;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChristineMonosiri;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChristineRenzokuAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChristineKiss;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

USER_FUNC(btlevtcmd_get_monosiri_msg_no) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, index;

    id = evtGetValue(event, args[0]);
    evtGetValue(event, args[1]);
    index = args[2];
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(event, index, (EvtWord)battleGetUnitMonosiriPtr(unit->currentKind)->tattleBtlTag);
    return EVT_RETURN_DONE;
}

USER_FUNC(_monosiri_flag_on) {
    s32 id = evtGetValue(event, event->args[0]);

    battleSetUnitMonosiriFlag(BattleGetUnitPtr(_battleWorkPointer, id)->currentKind);
    return EVT_RETURN_DONE;
}

USER_FUNC(_dictionary) {
    EvtWord* args = event->args;
    s32 selfId;
    s32 state;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 targetId;
    void* work;

    selfId = BattleTransID(event, -2);
    targetId = evtGetValue(event, args[0]);
    targetId = BattleTransID(event, targetId);
    evtGetValue(event, args[1]);
    unit = BattleGetUnitPtr(wp, selfId);
    state = unit->work[0];
    work = (void*)unit->work[1];
    switch (state) {
        case 0:
            break;
        case 1:
            work = monosiriInit(BattleGetUnitPtr(wp, targetId)->currentKind, -50.0f, 50.0f);
            state = 2;
            break;
        case 2:
            monosiriMain(work);
            state = 3;
            break;
        case 3:
            monosiriMain(work);
            break;
        case 4:
            monosiriClose(work);
            monosiriMain(work);
            state = 5;
            break;
        case 5:
            if (monosiriCloseWait(work)) {
                monosiriExit(work);
                state = 6;
            } else {
                monosiriMain(work);
            }
            break;
        case 6:
            break;
    }
    unit->work[0] = state;
    unit->work[1] = (EvtWord)work;
    return EVT_RETURN_DONE;
}

USER_FUNC(_set_hustle) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (unit) {
        if (unit->movesRemaining <= 0) {
            unit->movesRemaining++;
            unit->moveState = 0;
            unit->mMoveColorLv = 0xFF;
            if (unit->maxMovesThisTurn <= 0) {
                unit->maxMovesThisTurn = 1;
                if (BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
                    unit->mTokenFlags |= kHasRallyWinkStatus;
                }
            }
        } else {
            BtlUnit_OnStatusFlag(unit, UNIT_STATUS_FLAG_HUSTLE);
        }
    }
    return EVT_RETURN_DONE;
}
