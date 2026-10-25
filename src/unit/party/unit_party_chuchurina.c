#include "version.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mario/mario_pouch.h"

void btlUnitPartsDisp(CameraId camId, BattleUnitPart* parts);
void BtlUnit_EquipItem(BattleUnit* unit, s32 flags, ItemType type);

PoseTableEntry pose_table_chuchurina_stay[24];
static BattleDataEntry data_table[8];
BattleUnitKindPart unitpartsdata_Party_Chuchurina[4];
BattleUnitKind unitdata_Party_Chuchurina;
static EvtWord battle_entry_event[59];
static EvtWord init_event[48];
static EvtWord damage_event[16];
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeft;
BattleWeapon partyWeapon_ChuchurinaNormalAttackRight;
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeftLast;
BattleWeapon partyWeapon_ChuchurinaNormalAttackRightLast;
BattleWeapon partyWeapon_ChuchurinaMadowaseAttack;
BattleWeapon partyWeapon_ChuchurinaItemSteal;
BattleWeapon partyWeapon_ChuchurinaKiss;
EvtWord partyChuchurinaAttack_NormalAttack[1590];
EvtWord partyChuchurinaAttack_MadowaseAttack[2020];
EvtWord partyChuchurinaAttack_ItemSteal[VERSION_PICK(788, 961)];
EvtWord partyChuchurinaAttack_Kiss[568];
EvtWord chuchurina_appeal_event[86];
static EvtWord party_win_reaction[18];
static EvtWord attack_audience_event[VERSION_PICK(204, 205)];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];
static EvtWord main_evt[4];

USER_FUNC(_chuchu_make_extra_work_area);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(mono_main);
USER_FUNC(mono_off);
USER_FUNC(mono_on);
USER_FUNC(mono_capture_event);
USER_FUNC(_chuchu_item_steal);
USER_FUNC(_get_itemsteal_param);
USER_FUNC(_make_madowase_weapon);
USER_FUNC(_get_binta_hit_position);
USER_FUNC(mono_off);
USER_FUNC(mono_on);
USER_FUNC(mono_capture_event);
USER_FUNC(mono_main);
USER_FUNC(_chuchu_item_steal);
USER_FUNC(_get_itemsteal_param);
USER_FUNC(_make_madowase_weapon);
USER_FUNC(_get_binta_hit_position);
USER_FUNC(_chuchu_make_extra_work_area);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);









PoseTableEntry pose_table_chuchurina_stay[] = {
    {POSE_TYPE_FREEZE, "PCH_D_4"},
    {POSE_TYPE_STOP, "PCH_D_4"},
    {POSE_TYPE_DIZZY, "PCH_X_1"},
    {POSE_TYPE_POISON, "PCH_D_2"},
    {POSE_TYPE_CONFUSE, "PCH_X_1"},
    {POSE_TYPE_SLEEP, "PCH_N_1"},
    {POSE_TYPE_KO, "PCH_D_3"},
    {POSE_TYPE_NORMAL, "PCH_S_1"},
    {37, "PCH_G_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PCH_G_1"},
    {POSE_TYPE_DAMAGE, "PCH_D_1"},
    {POSE_TYPE_WALK, "PCH_W_1"},
    {POSE_TYPE_RUN, "PCH_R_1"},
    {POSE_TYPE_PERIL, "PCH_D_2"},
    {POSE_TYPE_DANGER, "PCH_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PCH_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PCH_D_2"},
    {POSE_TYPE_TALK, "PCH_T_1"},
    {POSE_TYPE_ITEM, "PCH_I_1"},
    {POSE_TYPE_COMMAND, "PCH_C_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PCH_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PCH_O_1"},
    {POSE_TYPE_DEFEND, "PCH_G_1"},
    {POSE_TYPE_ACTION, "PCH_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, chuchurina_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

BattleUnitKindPart unitpartsdata_Party_Chuchurina[] = {
    {1, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_chuchurina_stay},
    {2, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
    {3, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
    {4, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
};

BattleUnitKind unitdata_Party_Chuchurina = {
    UNIT_MS_MOWZ,
    "btl_un_chuchurina",
    10,
    5,
    5,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    20,
    33,
    10,
    22,
    0.0f,
    12.0f,
    0.0f,
    0,
    0,
    0.0f,
    30.0f,
    0.0f,
    0.0f,
    30.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    10.0f,
    21.45f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_PARTY_DAMAGE1",
    "SFX_MARIO_DAMAGE_FIRE1",
    "SFX_MARIO_DAMAGE_ICE1",
    "SFX_MARIO_DAMAGE_BIRIBIRI1",
    kUnitNoHpGauge,
    &regist,
    4,
    unitpartsdata_Party_Chuchurina,
    init_event,
    data_table,
};

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 7, 7, 0, 0)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -2, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(_chuchu_make_extra_work_area)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

BattleWeapon partyWeapon_ChuchurinaNormalAttackLeft = {
    "btl_wn_pcr_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChuchurinaNormalAttackRight = {
    "btl_wn_pcr_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChuchurinaNormalAttackLeftLast = {
    "btl_wn_pcr_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChuchurinaNormalAttackRightLast = {
    "btl_wn_pcr_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ChuchurinaMadowaseAttack = {
    "btl_wn_pcr_madowase", // name
#ifdef VERSION_G8ME01
    346, // icon
#else
    338, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_madowaseru", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#ifdef VERSION_G8ME01
    1, // mStylishCommandMultiplier
#else
    3, // mStylishCommandMultiplier
#endif
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_madowaseru", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, // sleepChance..stopTime
    100, // dizzyChance
    3, // dizzyTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonChance..dodgyChance
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyTime..sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_MadowaseAttack, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_ChuchurinaItemSteal = {
    "btl_wn_pcr_steal", // name
#ifdef VERSION_G8ME01
    345, // icon
#else
    337, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_heart_catch", // description
    100, // baseAccuracy
    2, // baseFpCost
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_ITEMSTEAL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_heart_catch", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_ItemSteal, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_ChuchurinaKiss = {
    "btl_wn_pcr_kiss", // name
#ifdef VERSION_G8ME01
    347, // icon
#else
    339, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pch_kiss", // description
    100, // baseAccuracy
    10, // baseFpCost
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
    "msg_ac_chuchurina_kiss", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80003500, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_Kiss, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(partyChuchurinaAttack_NormalAttack)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    END_IF
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    MULTIPLY_FLOAT(LW(5), 12)
    CALL(_get_binta_hit_position, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(5))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A1_1"))
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
        CALL(btlevtcmd_AcGetDifficulty, LW(0))
        SUBTRACT(LW(0), 3)
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(-3)
            SET_VALUE(LW(1), 25)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(-2)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(-1)
            SET_VALUE(LW(1), 17)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(0)
            SET_VALUE(LW(1), 15)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(1)
            SET_VALUE(LW(1), 14)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(2)
            SET_VALUE(LW(1), 13)
            SET_VALUE(LW(2), 30)
            CASE_DEFAULT
            SET_VALUE(LW(1), 12)
            SET_VALUE(LW(2), 30)
        END_SWITCH
        CALL(btlevtcmd_GetPartyTechLv, -2, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 34, 35, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
            CALL(btlevtcmd_AcSetFlag, 64)
            CASE_EQUAL(1)
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 50, 51, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
            CALL(btlevtcmd_AcSetFlag, 64)
            CASE_DEFAULT
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 100, 100, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 100, 100, 100, 100)
            CALL(btlevtcmd_AcSetFlag, 64)
        END_SWITCH
        CALL(btlevtcmd_SetupAC, -2, 6, 1, 0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_StartAC, 1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            ADD(LW(6), 1)
            SET_VALUE(LW(0), LW(6))
            DIVIDE(LW(0), 2)
            MODULO(LW(0), 2)
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetScale, -2, FLOAT(1.0498047f), FLOAT(1.0498047f), FLOAT(1.0f))
            ELSE
                CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
            SET_VALUE(LW(1), 255)
            SET_VALUE(LW(2), 190)
            MULTIPLY(LW(2), LW(0))
            DIVIDE(LW(2), 100)
            SUBTRACT(LW(1), LW(2))
            CALL(btlevtcmd_SetRGB, -2, 1, 255, LW(1), LW(1))
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_GetPartyTechLv, -2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, -2, 5, 4)
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, -2, 5, 3)
        CASE_DEFAULT
        CALL(btlevtcmd_SetUnitWork, -2, 5, 2)
    END_SWITCH
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_SetRGB, -2, 1, 255, 255, 255)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(8), -75)
        CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
        IF_INT_GREATER_OR_EQUAL(LW(0), 180)
            SUBTRACT(LW(0), 360)
        END_IF
        CALL(evt_sub_intpl_init, 5, LW(0), LW(8), 8)
        LOOP(8)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(8), -60)
        SET_VALUE(LW(9), -75)
        CALL(evt_sub_intpl_init, 11, LW(9), LW(8), 4)
        LOOP(4)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    SET_VALUE(LW(13), 18)
    SET_VALUE(LW(10), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    LABEL(10)
    ADD(LW(10), 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A1_2"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 2)
        CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    IF_INT_NOT_EQUAL(LW(5), 1)
        DELETE_EVENT(LW(14))
        SET_VALUE(LW(14), 0)
        CALL(btlevtcmd_StopAC)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        LOOP(20)
            CALL(btlevtcmd_AddRotate, -2, 0, 18, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(36)
            CALL(btlevtcmd_AddRotate, -2, 0, 10, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A2_4"))
        WAIT_FRAMES(52)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    SET_VALUE(LW(0), LW(10))
    MODULO(LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRight))
    ELSE
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(14), 0)
        DELETE_EVENT(LW(14))
        SET_VALUE(LW(14), 0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
        GOTO(20)
    ELSE
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_CLEAR(LW(6), 2)
            GOTO(20)
        END_IF
    END_IF
    GOTO(21)
    LABEL(20)
    IF_INT_EQUAL(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeftLast))
    ELSE
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRightLast))
    END_IF
    LABEL(21)
    CALL(btlevtcmd_PreCheckCounter, -2, LW(3), LW(4), LW(12), 256, LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(7)
        CASE_OR_EQUAL(8)
        CASE_OR_EQUAL(9)
        CASE_OR_EQUAL(10)
        CASE_OR_EQUAL(11)
        CASE_OR_EQUAL(12)
        CASE_OR_EQUAL(13)
        CASE_OR_EQUAL(14)
        CASE_OR_EQUAL(15)
        CASE_OR_EQUAL(16)
        CASE_OR_EQUAL(17)
        IF_INT_EQUAL(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
            SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeftLast))
        ELSE
            SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRightLast))
        END_IF
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            SET_VALUE(LW(7), LW(13))
            CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                SUBTRACT(LW(0), 360)
                CALL(evt_sub_intpl_init, 11, LW(0), 0, 4)
                LOOP(4)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
                    WAIT_FRAMES(1)
                    IF_INT_EQUAL(LW(1), 1)
                        BREAK_LOOP
                    END_IF
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(15))
            CALL(btlevtcmd_GetResultAC, LW(6))
            IF_BITS_SET(LW(6), 2)
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
            END_IF
            RETURN
            CASE_END
        END_SWITCH
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 12, 13)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
            CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK3"), 0)
            IF_BITS_SET(LW(6), 2)
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
            END_IF
        ELSE
            SET_VALUE(LW(0), LW(10))
            AND(LW(0), 1)
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK2"), 0)
            ELSE
                CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK1"), 0)
            END_IF
            IF_BITS_SET(LW(6), 2)
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
            END_IF
        END_IF
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_SET(LW(6), 2)
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SUBTRACT(LW(0), 2)
            IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(11))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_ACSuccessEffect, LW(11), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(11))
            ELSE
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            END_IF
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        IF_BITS_SET(LW(6), 2)
            SWITCH(LW(10))
                CASE_LESS_OR_EQUAL(2)
                BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                    SET_VALUE(LW(7), LW(13))
                    IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                        SET_VALUE(LW(8), 75)
                    ELSE
                        SET_VALUE(LW(8), -75)
                    END_IF
                    CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                        SUBTRACT(LW(0), 360)
                    END_IF
                    CALL(evt_sub_intpl_init, 5, LW(0), LW(8), 14)
                    LOOP(14)
                        CALL(evt_sub_intpl_get_value)
                        CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
                    IF_INT_LESS(LW(10), LW(0))
                        SET_VALUE(LW(7), LW(13))
                        IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                            SET_VALUE(LW(8), 75)
                            SET_VALUE(LW(9), 75)
                        ELSE
                            SET_VALUE(LW(8), -75)
                            SET_VALUE(LW(9), -75)
                        END_IF
                        CALL(evt_sub_intpl_init, 11, LW(9), LW(8), 6)
                        LOOP(6)
                            CALL(evt_sub_intpl_get_value)
                            CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                            WAIT_FRAMES(1)
                        END_LOOP
                    ELSE
                        SET_VALUE(LW(7), LW(13))
                        IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                            SET_VALUE(LW(8), 0)
                            SET_VALUE(LW(9), 75)
                        ELSE
                            SET_VALUE(LW(8), 0)
                            SET_VALUE(LW(9), -75)
                        END_IF
                        CALL(evt_sub_intpl_init, 11, LW(9), LW(8), 12)
                        LOOP(12)
                            CALL(evt_sub_intpl_get_value)
                            CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                            WAIT_FRAMES(1)
                        END_LOOP
                    END_IF
                END_SIBLING_EVENT
                CALL(btlevtcmd_WaitEventEnd, LW(15))
                CASE_DEFAULT
                BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                    CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(8), EVT_NULLPTR)
                    SET_VALUE(LW(7), 360)
                    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
                        SET_VALUE(LW(0), LW(8))
                        IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                            SUBTRACT(LW(0), 360)
                        END_IF
                        IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                            SUBTRACT(LW(7), LW(0))
                        ELSE
                            ADD(LW(7), LW(0))
                        END_IF
                    ELSE
                        ADD(LW(7), 60)
                        SET_VALUE(LW(0), LW(8))
                        IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                            SUBTRACT(LW(0), 360)
                        END_IF
                        IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                            SUBTRACT(LW(7), LW(0))
                        ELSE
                            ADD(LW(7), LW(0))
                        END_IF
                    END_IF
                    IF_INT_LESS(LW(13), 0)
                        MULTIPLY(LW(7), -1)
                    END_IF
                    CALL(evt_sub_intpl_init, 5, 0, LW(7), 20)
                    LOOP(20)
                        CALL(evt_sub_intpl_get_value)
                        ADD(LW(0), LW(8))
                        CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(btlevtcmd_SetUnitWork, -2, 4, LW(15))
                WAIT_FRAMES(10)
            END_SWITCH
        ELSE
            BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(8), EVT_NULLPTR)
                SET_VALUE(LW(7), 720)
                IF_INT_GREATER_OR_EQUAL(LW(10), 1)
                    SET_VALUE(LW(0), LW(8))
                    IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                        SUBTRACT(LW(0), 360)
                    END_IF
                    SUBTRACT(LW(7), LW(0))
                END_IF
                IF_INT_LESS(LW(13), 0)
                    MULTIPLY(LW(7), -1)
                END_IF
                CALL(evt_sub_intpl_init, 0, 0, LW(7), 60)
                LOOP(60)
                    CALL(evt_sub_intpl_get_value)
                    ADD(LW(0), LW(8))
                    CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(15))
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_X_1"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_StopAC)
            CALL(evt_audience_ap_recovery)
            CALL(btlevtcmd_InviteApInfoReport)
            GOTO(95)
        END_IF
        MULTIPLY(LW(13), -1)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        IF_INT_LESS(LW(10), LW(0))
            GOTO(10)
        END_IF
        LABEL(90)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A1_3"))
        WAIT_FRAMES(40)
        LABEL(95)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(14))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(14), -1, 0)
        END_SIBLING_EVENT
        SET_VALUE(LW(11), LW(14))
        SUBTRACT(LW(11), 11)
        SUBTRACT(LW(14), 1)
        IF_INT_GREATER(LW(14), LW(11))
            SET_VALUE(LW(13), LW(14))
            SUBTRACT(LW(13), 10)
            IF_BITS_SET(LW(6), 2)
                CALL(btlevtcmd_ACRStart, -2, 0, LW(13), LW(14), 0)
                CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
                IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                    DELETE_EVENT(LW(15))
                    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
                    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_O_1"))
                    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
                    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
                    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
                    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
                    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
                ELSE
                    CALL(btlevtcmd_WaitEventEnd, LW(15))
                END_IF
            ELSE
                CALL(btlevtcmd_WaitEventEnd, LW(15))
            END_IF
        ELSE
            CALL(btlevtcmd_WaitEventEnd, LW(15))
        END_IF
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, -2)
        CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
EVT_END()

EVT_BEGIN(partyChuchurinaAttack_MadowaseAttack)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetFlag, 41)
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 3)
        SET_VALUE(LW(1), 50)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 4)
        SET_VALUE(LW(1), 80)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 5)
        SET_VALUE(LW(1), 100)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 7)
        SET_VALUE(LW(1), 100)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 8)
        SET_VALUE(LW(1), 110)
        CALL(btlevtcmd_AcSetFlag, 49)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 10)
        SET_VALUE(LW(1), 120)
        CALL(btlevtcmd_AcSetFlag, 49)
        CASE_DEFAULT
        SET_VALUE(LW(0), 12)
        SET_VALUE(LW(1), 130)
        CALL(btlevtcmd_AcSetFlag, 49)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 1, 240, 178, LW(0), LW(1), 100, 1, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
    CALL(btlevtcmd_SetupAC, -2, 6, 1, 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 73, 400, LW(0), 43, 0, 40, 5)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_I_1"))
    LOOP(40)
        CALL(btlevtcmd_AddRotate, -2, 0, 36, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A4_1"))
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetDiveSound, -2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, -2, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, -2, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, -2, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, -2, 2, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, -2, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, -2, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, -2, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, -2, 3, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, -2, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, -2, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, -2, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, -2, 4, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    SUBTRACT(LW(2), 20)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    CALL(btlevtcmd_SetPartsPos, -2, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 180, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("PCH_A3_1"))
    CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CHURINA_CONFUSE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, -2, 2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, -2, 2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, -2, 2, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 2, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 10)
    CALL(btlevtcmd_SetPartsPos, -2, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, -2, 3, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("PCH_A3_2"))
    CALL(btlevtcmd_OffPartsAttribute, -2, 3, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, -2, 3, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, -2, 3, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, -2, 3, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 3, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), -10)
    CALL(btlevtcmd_SetPartsPos, -2, 4, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, -2, 4, 0, 180, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 4, STRING("PCH_A3_3"))
    CALL(btlevtcmd_OffPartsAttribute, -2, 4, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, -2, 4, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, -2, 4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, -2, 4, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, -2, 4, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 4, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    WAIT_FRAMES(85)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, -2, 2, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, -2, 2, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, -2, 3, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, -2, 4, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, -2, 4, kMotionBlur)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(3.0f), LW(7))
        CALL(evt_btl_camera_set_mode, 0, 3)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 90, 500, LW(0), 60, 0, LW(7), 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        ADD(LW(0), 40)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        ADD(LW(0), 40)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 2)
        CALL(btlevtcmd_SetPartsFallAccel, -2, LW(6), FLOAT(0.69921875f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, -2, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        ADD(LW(0), 60)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        SUBTRACT(LW(0), 90)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        ADD(LW(0), 60)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        SUBTRACT(LW(0), 90)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 2, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 3)
        CALL(btlevtcmd_SetPartsFallAccel, -2, LW(6), FLOAT(0.69921875f))
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, -2, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 80)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 25, -1)
        ADD(LW(0), 160)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 40, -1)
        SUBTRACT(LW(0), 40)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 15, -1)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 20, -1)
        ADD(LW(0), 20)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 20, -1)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 3, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, -2, 3, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 4)
        CALL(btlevtcmd_SetPartsFallAccel, -2, LW(6), FLOAT(0.69921875f))
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, -2, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_GetPartsPos, -2, LW(6), LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, -2, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(6), LW(0), LW(1), LW(2), 10, -1)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 4, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, -2, 4, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    END_SIBLING_EVENT
    LABEL(9)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
    ADD(LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(9)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_CONFUSE1"), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(15), 0)
    LABEL(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
    CALL(_make_madowase_weapon, LW(12), LW(0))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(6), 0)
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            ADD(LW(15), 1)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        END_IF
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    LABEL(80)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_B_1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 80)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CHURINA_CONFUSE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 5, 0)
        CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
        MULTIPLY(LW(5), 9)
        LOOP(40)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(5))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
        MULTIPLY(LW(5), 36)
        LOOP(20)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(5))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_ACRStart, -2, 40, 59, 59, 0)
#else
        CALL(btlevtcmd_ACRStart, -2, 45, 59, 59, 0)
#endif
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
        WAIT_MS(300)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_AddRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CASE_DEFAULT
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_R_2"))
        CALL(btlevtcmd_AddRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetDiveSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyChuchurinaAttack_ItemSteal)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    SET_VALUE(LW(14), 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_GetWidth, LW(3), LW(15))
        DIVIDE(LW(15), 2)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(15))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(8))
        BEGIN_SIBLING_EVENT_GET_ID(LW(9))
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(8), -1, 0)
        END_SIBLING_EVENT
        IF_INT_GREATER(LW(8), 5)
            SUBTRACT(LW(8), 2)
            SET_VALUE(LW(15), LW(8))
            DIVIDE(LW(15), 3)
            SET_VALUE(LW(8), LW(15))
            MULTIPLY(LW(8), 2)
            CALL(btlevtcmd_ACRStart, -2, LW(15), LW(8), LW(8), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
                CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
                CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        ELSE
            CALL(btlevtcmd_WaitEventEnd, LW(9))
        END_IF
#else
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
#endif
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), 256, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetWidth, LW(3), LW(15))
    MULTIPLY(LW(15), 2)
    DIVIDE(LW(15), 3)
    ADD(LW(15), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(15))
    ADD(LW(2), 8)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(8))
    BEGIN_SIBLING_EVENT_GET_ID(LW(9))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(8), -1, 0)
    END_SIBLING_EVENT
    IF_INT_GREATER(LW(8), 5)
        SUBTRACT(LW(8), 2)
        SET_VALUE(LW(15), LW(8))
        DIVIDE(LW(15), 3)
        SET_VALUE(LW(8), LW(15))
        MULTIPLY(LW(8), 2)
        CALL(btlevtcmd_ACRStart, -2, LW(15), LW(8), LW(8), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            DELETE_EVENT(LW(9))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
            CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
            CALL(btlevtcmd_WaitEventEnd, LW(9))
        END_SWITCH
    ELSE
        CALL(btlevtcmd_WaitEventEnd, LW(9))
    END_IF
#else
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
#endif
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_K_1"))
    WAIT_FRAMES(6)
    IF_INT_NOT_EQUAL(LW(5), 1)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_NEUTRAL)
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
            CASE_EQUAL(1)
            CASE_DEFAULT
        END_SWITCH
        WAIT_FRAMES(30)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), 256, LW(5))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, -2, LW(15))
    MULTIPLY(LW(15), 2)
    DIVIDE(LW(15), 3)
    ADD(LW(1), LW(15))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Z_1"))
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_AnimeFlagOnOff, -2, 1, 256, 1)
    WAIT_FRAMES(1)
    CALL(mono_capture_event)
    SPAWN_EVENT_GET_ID(PTR(main_evt), LW(13))
    BEGIN_INLINE_EVENT
        CALL(mono_on)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(_get_itemsteal_param, LW(3), LW(0), LW(1))
    CALL(btlevtcmd_AcSetParamAll, 1, LW(0), 2, 2, LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 15, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A4_1"))
    END_IF
    WAIT_FRAMES(35)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_EYE1"), 0)
    WAIT_FRAMES(47)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetWidth, LW(3), LW(15))
    DIVIDE(LW(15), 2)
    ADD(LW(0), LW(15))
    CALL(btlevtcmd_GetHeight, LW(3), LW(15))
    ADD(LW(1), LW(15))
    ADD(LW(1), 5)
    ADD(LW(2), 8)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(_chuchu_item_steal, LW(3), LW(14), LW(6))
    IF_INT_EQUAL(LW(14), 0)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_F_1"))
        WAIT_FRAMES(22)
        BEGIN_INLINE_EVENT
            CALL(mono_off)
            DELETE_EVENT(LW(13))
        END_INLINE_EVENT
        WAIT_FRAMES(20)
        CALL(btlevtcmd_OffUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_R_2"))
        CALL(btlevtcmd_AddRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        GOTO(99)
    END_IF
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_GetResultPrizeLv, 0, 0, LW(6))
#else
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
#endif
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_HEARTCATCH1"), 0)
    CALL(btlevtcmd_OffUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
    BEGIN_INLINE_EVENT
        CALL(mono_off)
        DELETE_EVENT(LW(13))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 50)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A3_1"))
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 10, 0)
        LOOP(18)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, 20)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    IF_INT_NOT_EQUAL(LW(14), 0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, -2, LW(5))
        ADD(LW(1), LW(5))
        ADD(LW(1), 5)
        CALL(btlevtcmd_BtlIconEntryItemId, LW(14), LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
            CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
            CALL(btlevtcmd_AddRotate, -2, 0, 180, 0)
            CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_SIBLING_EVENT
        LABEL(91)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, -2, LW(5))
        ADD(LW(1), LW(5))
        ADD(LW(1), 5)
        CALL(btlevtcmd_BtlIconSetPosition, LW(8), LW(0), LW(1), LW(2))
        IS_EVENT_RUNNING(LW(15), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(91)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A2_2"))
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnnounceSetParam, 0, LW(14))
        CALL(btlevtcmd_AnnounceMessage, 1, 0, 0, STRING("btl_msg_steal_item_get"), 90)
        CALL(btlevtcmd_BtlIconDelete, LW(8))
    ELSE
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_AddRotate, -2, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_IF
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyChuchurinaAttack_Kiss)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    END_IF
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CalculateFaceDirection, LW(3), LW(4), -2, -1, 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, LW(3), LW(15))
    END_IF
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 300)
        SET_VALUE(LW(1), 20)
        SET_VALUE(LW(2), 20)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 270)
        SET_VALUE(LW(1), 18)
        SET_VALUE(LW(2), 22)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 17)
        SET_VALUE(LW(2), 24)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 210)
        SET_VALUE(LW(1), 16)
        SET_VALUE(LW(2), 25)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 195)
        SET_VALUE(LW(1), 14)
        SET_VALUE(LW(2), 26)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 180)
        SET_VALUE(LW(1), 13)
        SET_VALUE(LW(2), 27)
        CASE_DEFAULT
        SET_VALUE(LW(0), 150)
        SET_VALUE(LW(1), 12)
        SET_VALUE(LW(2), 30)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 11, LW(0), 178, LW(1), LW(2), 12, 37, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 36, 72, 100, 100)
    CALL(btlevtcmd_AcSetFlag, 64)
    CALL(btlevtcmd_SetupAC, -2, 6, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_GetStatusMg, -2, LW(6))
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(7))
        MULTIPLY_FLOAT(LW(6), 12)
        MULTIPLY_FLOAT(LW(7), 7)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(7))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 235, -1, 0)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(5))
    SWITCH(LW(5))
        CASE_GREATER_OR_EQUAL(10)
        SET_VALUE(LW(5), 2)
        CASE_GREATER_OR_EQUAL(7)
        SET_VALUE(LW(5), 1)
        CASE_GREATER_OR_EQUAL(4)
        SET_VALUE(LW(5), 0)
        CASE_DEFAULT
        SET_VALUE(LW(5), -1)
    END_SWITCH
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, -5, LW(5), LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_K_1"))
    WAIT_FRAMES(6)
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL_CHILD(subsetevt_check_target_movable)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_KISSED_CHUCHURINA)
        END_IF
    END_IF
    CALL(btlevtcmd_GetStatusMg, -2, LW(6))
    MULTIPLY_FLOAT(LW(6), 16)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(6))
    CALL(btlevtcmd_GetStatusMg, -2, LW(6))
    MULTIPLY_FLOAT(LW(6), 40)
    ADD(LW(1), LW(6))
    CALL(btlevtcmd_GetFaceDirection, LW(3), LW(6))
    MULTIPLY(LW(6), 45)
    CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(12), LW(13), LW(14))
    CALL(btlevtcmd_GetHeight, LW(3), LW(5))
    CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(13), LW(5))
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(5))
    CALL(evt_eff, STRING("eff"), STRING("recovery"), 0, LW(12), LW(13), LW(14), LW(5), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(120)
        CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 15, 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_NEUTRAL)
        WAIT_MS(300)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_ResetFaceDirection, LW(3))
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(chuchurina_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_O_3"))
    CALL(btlevtcmd_AudienceAppeal, -2)
    CALL(btlevtcmd_ftomsec, 45, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    WAIT_MS(333)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_NEUTRAL)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(3), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(4))
    SET_VALUE(LW(8), LW(3))
    MULTIPLY(LW(8), LW(4))
    MULTIPLY(LW(8), -1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, -2, LW(8), LW(1), LW(2), 0, -1, 0)
    WAIT_FRAMES(20)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, -2, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, 0, 13)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, -2, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 24)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_A4_1"))
    CALL(btlevtcmd_ftof, 52, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PCH_Y_1"))
    CALL(btlevtcmd_ftof, 7, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, 0, 3)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftof, 35, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(6), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
#ifdef VERSION_G8ME01
EVT_END_PAD()
#else
EVT_END()
#endif

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(main_evt)
    CALL(mono_main)
    RETURN
EVT_END()


s32 mono_alpha;

USER_FUNC(_chuchu_make_extra_work_area) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(BattleWeapon));
    return EVT_RETURN_DONE;
}

#define ADD_WEAPON(w)                                                                              \
    command->weapon[*count].index = -1;                                                     \
    command->weapon[*count].itemId = 0;                                                       \
    command->weapon[*count].weapon = (BattleWeapon*)(w);                              \
    command->weapon[*count].enabled = 0;                                                   \
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;       \
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name); \
    (*count)++;

static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));
    ADD_WEAPON(&partyWeapon_ChuchurinaNormalAttackLeftLast)
    if (techLv >= 0) {
        ADD_WEAPON(&partyWeapon_ChuchurinaItemSteal)
    }
    if (techLv >= 1) {
        ADD_WEAPON(&partyWeapon_ChuchurinaMadowaseAttack)
    }
    if (techLv >= 2) {
        ADD_WEAPON(&partyWeapon_ChuchurinaKiss)
    }
}

USER_FUNC(_get_binta_hit_position) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    Vec pos;
    s32 id, targetId, dir;
    s32 outZ, outX, outY;

    id = evtGetValue(event, args[0]);
    targetId = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(wp, BattleTransID(event, id));
    unit = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    dir = unit->faceDirection;
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += dir * (unit->bintaHitOffset.x * unit->mSizeMultiplier);
    pos.y += unit->bintaHitOffset.y * unit->mSizeMultiplier;
    pos.z += unit->bintaHitOffset.z * unit->mSizeMultiplier;
    evtSetFloat(event, outX, pos.x);
    evtSetFloat(event, outY, pos.y);
    evtSetFloat(event, outZ, pos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(_make_madowase_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id, value;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    value = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->dizzyChance = value;
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_itemsteal_param) {
    BattleWork* wp;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, out1, out2, rate, resist;

    id = evtGetValue(event, args[0]);
    out1 = args[1];
    wp = _battleWorkPointer;
    out2 = args[2];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    rate = 80 - ((u8)wp->actionCommands.mAcDifficulty - 3) * 20;
    if (rate <= 5) {
        rate = 5;
    }
    resist = 100 - unit->mKindParams->itemStealParam;
    evtSetValue(event, out1, rate);
    evtSetValue(event, out2, resist);
    return EVT_RETURN_DONE;
}

USER_FUNC(_chuchu_item_steal) {
    ItemDropData* table;
    BattleWork* wp;
    s32 flag;
    s32 out;
    BattleUnit* unit;
    s32 id;
    u16 item;
    EvtWord* args;

    args = event->args;
    wp = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    out = args[1];
    flag = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    item = unit->heldItem;
    if (item == 0) {
        s32 total, r;
        u32 pick;
        if (unit->dropTable != NULL) {
            irand(100);
        }
        table = unit->dropTable;
        if (table == NULL) {
            pick = 0;
        } else {
            ItemDropData* p;
            if (table != NULL) {
                s32 i;
                total = 0;
                for (i = 0; table[i].holdWeight > 0 || table[i].type != 0; i++) {
                    total += table[i].holdWeight;
                }
                if (total > 0) {
                    r = irand(total);
                    for (;; table++) {
                        r -= table->holdWeight;
                        if (r < 0) {
                            pick = table->type;
                            goto done;
                        }
                    }
                }
            }
            pick = 0;
        }
    done:
        item = pick;
        if (item == 0) {
            item = 0x79;
        }
    }
    if (!(flag & 2)) {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    if (pouchGetItem((u16)item) == 0) {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    unit->heldItem = ITEM_NULL;
    if (BtlUnit_CheckUnitFlag(unit, 0x40000000)) {
        BtlUnit_OffUnitFlag(unit, 0x40000000);
        if (unit->groupSetupId >= 0) {
            s32* arr = (s32*)wp->fbatInfo->setup;
            arr[unit->groupSetupId + 7] = 0;
            arr[unit->groupSetupId + 0x17] = 0;
        }
    } else if (unit->groupSetupId >= 0) {
        s32* arr = (s32*)wp->fbatInfo->setup;
        arr[unit->groupSetupId + 7] = 0;
    }
    if ((u16)(s32)item >= 0xF0 && (u16)(s32)item < 0x153) {
        if (unit->currentKind == UNIT_MARIO) {
            BtlUnit_EquipItem(unit, 3, 0);
        } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
            BtlUnit_EquipItem(unit, 5, 0);
        } else {
            BtlUnit_EquipItem(unit, 1, 0);
        }
    }
    evtSetValue(event, out, (u16)item);
    return EVT_RETURN_DONE;
}

void mono_capture(CameraId camId, void* param) {
    BattleUnit* unit = param;
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);
    unit->screenTexBuf = smartAlloc(GXGetTexBufferSize(0x260, 0x1E0, 1, 0, 0), 1);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x260, 0x1E0, 1, 0);
    GXCopyTex(unit->screenTexBuf->address, 0);
    GXPixModeSync();
    part->animFlags &= ~0x100;
}

void mono_disp(CameraId camId, BattleUnit* unit) {
    CameraEntry* cam = camGetPtr(camId);
    smartEntry* e = unit->screenTexBuf;
    void* buf = NULL;
    GXTexObj tex;
    if (e != NULL) {
        buf = e->address;
    }
    if (buf != NULL) {
        GXInitTexObj(&tex, buf, 0x260, 0x1E0, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXLoadPosMtxImm(cam->view, 0);
        GXSetCurrentMtx(0);
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, mono_alpha});
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-304.0f, 240.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(304.0f, 240.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(304.0f, -240.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-304.0f, -240.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        {
            BattleUnit* party = BattleGetPartyPtr(_battleWorkPointer);
            if (party != NULL) {
                BattleUnitPart* p = BtlUnit_GetPartsPtr(party, 1);
                CameraId cur = camGetCurNo();
                camSetCurNo(CAMERA_3D);
                btlUnitPartsDisp(CAMERA_3D, p);
                camSetCurNo(cur);
            }
        }
    }
}

USER_FUNC(mono_main) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    dispEntry(CAMERA_2D, 910.0f, 0, (DispCallback)mono_disp, unit);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(mono_capture_event) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPartsPtr(unit, 1);
    dispEntry(CAMERA_2D, 910.0f, 0, (DispCallback)mono_capture, unit);
    return EVT_RETURN_DONE;
}

USER_FUNC(mono_on) {
    if (isFirstCall) {
        mono_alpha = 0;
    }
    mono_alpha += 10;
    if (mono_alpha > 255) {
        mono_alpha = 255;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(mono_off) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    mono_alpha -= 10;
    if (mono_alpha < 0) {
        mono_alpha = 0;
        BtlUnit_GetPartsPtr(unit, 1);
        if (unit->screenTexBuf != NULL) {
            smartFree(unit->screenTexBuf);
        }
        unit->screenTexBuf = NULL;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
