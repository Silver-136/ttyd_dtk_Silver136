#include "battle/battle_unit.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"

EffectEntry* effStardustEntry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 width, f32 height, s32 time);
EffectEntry* effThunderflareN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 lifetime);

USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(eff_magic_support);
static USER_FUNC(eff_magic_get_posX);
static USER_FUNC(eff_magic);
static USER_FUNC(eff_staff);

BattleWeapon weapon_kamec_obaba_attack_magic;
BattleWeapon weapon_kamec_obaba_dekadeka_magic;
BattleWeapon weapon_kamec_obaba_biribiri_magic;
BattleWeapon weapon_kamec_obaba_katikati_magic;
BattleWeapon weapon_kamec_obaba_trans_magic;
BattleWeapon weapon_kamec_obaba_recover_magic;
BattleUnitKind unit_boss_kamec_obaba;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_sky[17];
static PoseTableEntry pose_table_dead[3];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[2];
static EvtWord kamec_obaba_zoom[40];
static EvtWord init_event[56];
static EvtWord damage_event[74];
static EvtWord dmg_fall_event[149];
static EvtWord dead_event[155];
static EvtWord phase_event[216];
static EvtWord attack_event[352];
static EvtWord ground_magic_common_event1[87];
static EvtWord ground_magic_common_event2[76];
static EvtWord sky_magic_common_event[76];
static EvtWord magic_common_event1[27];
static EvtWord magic_common_event2[16];
static EvtWord start_avoid[26];
static EvtWord attack_magic_event[269];
EvtWord magic_select_pri_obaba[52];
EvtWord magic_select_pri_koopa[52];
static EvtWord dekadeka_magic_event[73];
static EvtWord katikati_magic_event[4];
static EvtWord biribiri_magic_event[4];
static EvtWord trans_magic_event[4];
static EvtWord recover_magic_event[90];
static EvtWord wait_event[6];

BattleWeapon weapon_kamec_obaba_attack_magic = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
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

BattleWeapon weapon_kamec_obaba_dekadeka_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_kamec_obaba_biribiri_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    3, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_kamec_obaba_katikati_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_kamec_obaba_trans_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    3, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_kamec_obaba_recover_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP | TARGET_WEIGHT_LOW_HP_RATIO, // targetWeightFlags
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

BattleUnitKind unit_boss_kamec_obaba = {
    145,
    "btl_un_kamec_obaba",
    50,
    0,
    1,
    1,
    48,
    0,
    0,
    100,
    0,
    50,
    6,
    42,
    42,
    7,
    42,
    0.0f,
    21.0f,
    0.0f,
    0,
    0,
    20.0f,
    20.0f,
    0.0f,
    21.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    18.0f,
    0.0f,
    0.0f,
    21.0f,
    27.3f,
    0.0f,
    0.0f,
    21.0f,
    0.0f,
    42.0f,
    42.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_KAMEBABA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist,
    2,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {40, 40, 60, 100, 40, 100, 100, 40, 100, 60, 100, 60, 100, 80, 40, 0, 0, 100, 40, 100, 100, 5};

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
    {31, "D_1"},
    {39, "D_1"},
    {42, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

static PoseTableEntry pose_table_sky[] = {
    {1, "N_2"},
    {2, "Y_2"},
    {9, "Y_2"},
    {5, "K_2"},
    {4, "X_2"},
    {3, "X_2"},
    {28, "S_2"},
    {29, "Q_2"},
    {30, "Q_2"},
    {31, "D_2"},
    {39, "D_2"},
    {42, "R_2"},
    {40, "W_2"},
    {56, "X_2"},
    {57, "X_2"},
    {65, "T_2"},
    {69, "S_2"},
};

static PoseTableEntry pose_table_dead[] = {
    {28, "D_3"},
    {39, "D_3"},
    {69, "D_3"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_kamec_obaba", "c_kamek_bb", 0.0f, 0.0f, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table_sky},
    {2, "btl_un_kamec_obaba", "c_kamek_bb", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
};

static EVT_BEGIN(kamec_obaba_zoom)
    CALL(btlevtcmd_GetUnitWork, GW(9), 0, LW(1))
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_EQUAL(LW(1), 1)
        CALL(evt_btl_camera_set_moveto, 0, 135, 106, 296, 135, 55, -402, 30, 11)
        ELSE
        CALL(evt_btl_camera_set_moveto, 0, 140, 34, 296, 140, 50, -402, 30, 11)
    END_IF
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_KAMEBABA_MOVE1L"), STRING("SFX_BOSS_KAMEBABA_MOVE1R"), 0, 15, 15)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_KAMEBABA_MOVE1L"), STRING("SFX_BOSS_KAMEBABA_MOVE1R"), 0, 8, 8)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckDamageCode, -2, 256, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(10), 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetTotalDamage, LW(10), LW(1))
        CALL(btlevtcmd_GetUnitWork, LW(10), 1, LW(0))
        ADD(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, LW(10), 1, LW(0))
        IF_INT_GREATER(LW(0), 1)
            CALL(btlevtcmd_WaitAttackEnd)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL_CHILD(PTR(dmg_fall_event))
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_fall_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, -2, kFloating)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_2"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 39)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, -2, 2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("H_1"))
        CALL(btlevtcmd_SetAlpha, -2, 2, 255)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden)
        WAIT_FRAMES(80)
        CALL(evt_sub_intpl_init, 0, 255, 0, 30)
        LOOP(30)
            CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
            CALL(btlevtcmd_SetAlpha, -2, 2, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, -2, 2, kHidden)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 10, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
    WAIT_MS(500)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    DEBUG_PUT_MSG(STRING("カメックオババ　死亡"))
    SET_VALUE(GW(11), 1)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    DEBUG_PUT_MSG(STRING("カメックオババ　死亡イベント開始"))
    CALL(btlevtcmd_GetUnitWork, LW(10), 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(dmg_fall_event))
    END_IF
    SWITCH(GW(10))
        CASE_EQUAL(0)
        DEBUG_PUT_MSG(STRING("カメックオババ　死亡イベント　クッパ生存"))
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL_CHILD(PTR(kamec_obaba_zoom))
        CALL(btlevtcmd_SetTalkPose, -2, STRING("T_3"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("D_3"))
        CALL(evt_msg_print, 2, STRING("stg8_las_100_01"), 0, -2)
        WAIT_MS(200)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_3"))
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_dead))
        SET_VALUE(GW(11), 2)
        SET_VALUE(LW(10), -2)
        CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        CASE_EQUAL(1)
        DEBUG_PUT_MSG(STRING("カメックオババ　死亡イベント　同時に死亡"))
        LOOP(0)
            IF_INT_EQUAL(GW(10), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_3"))
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_dead))
        SET_VALUE(GW(11), 2)
        SET_VALUE(LW(10), -2)
        CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
        CASE_EQUAL(2)
        DEBUG_PUT_MSG(STRING("カメックオババ　死亡イベント　クッパ死亡"))
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL_CHILD(PTR(kamec_obaba_zoom))
        CALL(btlevtcmd_SetTalkPose, -2, STRING("D_1"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("D_1"))
        CALL(evt_msg_print, 2, STRING("stg8_las_100_03"), 0, -2)
        WAIT_MS(200)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_3"))
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_dead))
        SET_VALUE(GW(11), 2)
        SET_VALUE(LW(10), -2)
        CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
        CALL(btlevtcmd_StatusWindowOnOff, 1)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AddUnitWork, -2, 2, 1)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_LESS(LW(0), 3)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, -2, 300, LW(1), LW(2), 0, -1, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_OnAttribute, -2, kFloating)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_sky))
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_MS(166)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ChangeFaceDirection, -2, -1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), 40, LW(2))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), 40, LW(2), 60, 0, 4, 0, -1)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 40, LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    DELETE_EVENT(LW(15))
    WAIT_MS(500)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    DEBUG_PUT_REG(LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_attack_magic))
        CALL_CHILD(PTR(attack_magic_event))
        GOTO(99)
    END_IF
    SET_VALUE(LW(5), 0)
    CALL(btlevtcmd_GetHp, -2, LW(0))
    CALL(btlevtcmd_GetMaxHp, -2, LW(1))
    MULTIPLY(LW(0), 100)
    DIVIDE(LW(0), LW(1))
    SWITCH(LW(0))
        CASE_LESS(25)
        SET_VALUE(LW(5), 25)
        CASE_LESS(50)
        SET_VALUE(LW(5), 10)
        CASE_LESS(75)
        SET_VALUE(LW(5), 5)
    END_SWITCH
    SET_VALUE(LW(6), 0)
    IF_INT_EQUAL(GW(10), 0)
        CALL(btlevtcmd_GetHp, GW(8), LW(0))
        CALL(btlevtcmd_GetMaxHp, GW(8), LW(1))
        MULTIPLY(LW(0), 100)
        DIVIDE(LW(0), LW(1))
        SWITCH(LW(0))
            CASE_LESS(25)
            SET_VALUE(LW(6), 25)
            CASE_LESS(50)
            SET_VALUE(LW(6), 10)
            CASE_LESS(75)
            SET_VALUE(LW(6), 5)
        END_SWITCH
    END_IF
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_dekadeka_magic))
    CALL_CHILD(PTR(magic_select_pri_obaba))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckStatus, LW(3), 6, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(8), 10)
        END_IF
        CALL(btlevtcmd_CheckStatus, LW(3), 18, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(10), 10)
        END_IF
    END_IF
    SET_VALUE(LW(13), LW(3))
    SET_VALUE(LW(14), LW(4))
    CALL_CHILD(PTR(magic_select_pri_koopa))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckStatus, LW(3), 14, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(7), 10)
        END_IF
        SET_VALUE(LW(11), 10)
    END_IF
    LABEL(10)
    SET_VALUE(LW(1), LW(5))
    ADD(LW(1), LW(6))
    ADD(LW(1), LW(7))
    ADD(LW(1), LW(8))
    ADD(LW(1), LW(10))
    ADD(LW(1), LW(11))
    ADD(LW(1), 50)
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    SET_VALUE(LW(1), LW(5))
    ADD(LW(1), LW(6))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_recover_magic))
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL_CHILD(PTR(subsetevt_confuse_flustered))
                RETURN
            END_IF
            GOTO(99)
        END_IF
        CALL(btlevtcmd_GetHp, LW(3), LW(0))
        CALL(btlevtcmd_GetMaxHp, LW(3), LW(1))
        IF_INT_EQUAL(LW(0), LW(1))
            GOTO(10)
        END_IF
        CALL_CHILD(PTR(recover_magic_event))
        GOTO(99)
    END_IF
    ADD(LW(1), LW(7))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_katikati_magic))
        CALL_CHILD(PTR(katikati_magic_event))
        GOTO(99)
    END_IF
    ADD(LW(1), LW(8))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_biribiri_magic))
        SET_VALUE(LW(3), LW(13))
        SET_VALUE(LW(4), LW(14))
        CALL_CHILD(PTR(biribiri_magic_event))
        GOTO(99)
    END_IF
    ADD(LW(1), LW(10))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_trans_magic))
        SET_VALUE(LW(3), LW(13))
        SET_VALUE(LW(4), LW(14))
        CALL_CHILD(PTR(trans_magic_event))
        GOTO(99)
    END_IF
    ADD(LW(1), LW(11))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_dekadeka_magic))
        CALL_CHILD(PTR(dekadeka_magic_event))
        GOTO(99)
        ELSE
        SET_VALUE(LW(9), PTR(&weapon_kamec_obaba_attack_magic))
        CALL_CHILD(PTR(attack_magic_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(ground_magic_common_event1)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(eff_staff, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1C"))
    WAIT_FRAMES(30)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1D"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1E"))
    IF_INT_NOT_EQUAL(LW(9), PTR(&weapon_kamec_obaba_attack_magic))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(eff_magic_support, -2, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(ground_magic_common_event2)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(eff_staff, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2A"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2C"))
    IF_INT_NOT_EQUAL(LW(9), PTR(&weapon_kamec_obaba_attack_magic))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(eff_magic_support, -2, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(sky_magic_common_event)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(eff_staff, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3A"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
    WAIT_FRAMES(35)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3C"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3D"))
    IF_INT_NOT_EQUAL(LW(9), PTR(&weapon_kamec_obaba_attack_magic))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(eff_magic_support, -2, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(magic_common_event1)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitId, -2, LW(13))
        IF_INT_NOT_EQUAL(LW(3), LW(13))
            CALL_CHILD(PTR(ground_magic_common_event1))
            ELSE
            CALL_CHILD(PTR(ground_magic_common_event2))
        END_IF
        ELSE
        CALL_CHILD(PTR(sky_magic_common_event))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(magic_common_event2)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(ground_magic_common_event2))
        ELSE
        CALL_CHILD(PTR(sky_magic_common_event))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL_CHILD(magic_common_event1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KAMEBABA_MAGIC3_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
        LABEL(90)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(1)
            LOOP(0)
                CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(eff_magic_get_posX, -2, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                ELSE
                    CALL(eff_magic_get_posX, -2, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -57)
        ADD(LW(1), 10)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
        CALL(btlevtcmd_GetStageSize, LW(10), EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
        MULTIPLY(LW(10), LW(0))
        SUBTRACT(LW(11), 5)
        CALL(eff_magic, -2, LW(10), LW(11), LW(12), 60)
        WAIT_FRAMES(60)
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -57)
        ADD(LW(1), 10)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
        CALL(eff_magic, -2, LW(10), LW(11), LW(12), 45)
        WAIT_FRAMES(45)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        GOTO(98)
        LABEL(98)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, -2)
        RETURN
EVT_END()

EVT_BEGIN(magic_select_pri_obaba)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    IF_INT_EQUAL(GW(10), 0)
        SET_VALUE(LW(5), 25)
    END_IF
    IF_INT_EQUAL(GW(11), 0)
        SET_VALUE(LW(6), 75)
    END_IF
    SET_VALUE(LW(1), LW(5))
    ADD(LW(1), LW(6))
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    IF_INT_LESS(LW(0), LW(5))
        SET_VALUE(LW(3), GW(8))
        SET_VALUE(LW(4), 1)
        ELSE
        SET_VALUE(LW(3), -2)
        SET_VALUE(LW(4), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(magic_select_pri_koopa)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    IF_INT_EQUAL(GW(10), 0)
        SET_VALUE(LW(5), 75)
    END_IF
    IF_INT_EQUAL(GW(11), 0)
        SET_VALUE(LW(6), 25)
    END_IF
    SET_VALUE(LW(1), LW(5))
    ADD(LW(1), LW(6))
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    IF_INT_LESS(LW(0), LW(5))
        SET_VALUE(LW(3), GW(8))
        SET_VALUE(LW(4), 1)
        ELSE
        SET_VALUE(LW(3), -2)
        SET_VALUE(LW(4), 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(dekadeka_magic_event)
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL_CHILD(PTR(magic_common_event1))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(katikati_magic_event)
    CALL_CHILD(PTR(dekadeka_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(biribiri_magic_event)
    CALL_CHILD(PTR(dekadeka_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(trans_magic_event)
    CALL_CHILD(PTR(dekadeka_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(recover_magic_event)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL_CHILD(PTR(magic_common_event1))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 8)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    IF_INT_EQUAL(LW(3), GW(8))
        ADD(LW(0), -30)
        ADD(LW(1), 100)
        ADD(LW(2), 10)
        ELSE
        ADD(LW(1), 50)
        ADD(LW(2), 10)
    END_IF
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 8, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

USER_FUNC(eff_magic_support) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 type = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    switch (type) {
        case 0:
            x += 30.0f * unit->mSizeMultiplier * unit->faceDirection;
            y += 10.0f * unit->mSizeMultiplier;
            z += 5.0f;
            effStardustEntry(6, 5, x, y, z, 40.0f, 40.0f, 30);
            break;
        case 1:
            x += 30.0f * unit->mSizeMultiplier * unit->faceDirection;
            y += 50.0f * unit->mSizeMultiplier;
            z += 5.0f;
            effStardustEntry(6, 5, x, y, z, 40.0f, 40.0f, 30);
            break;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(eff_magic_get_posX) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    evtSetFloat(event, args[1], ((EffMagic1N64Work*)((EffectEntry*)unit->work[4])->userdata)->offset.x);
    return EVT_RETURN_DONE;
}

static USER_FUNC(eff_magic) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 a = evtGetFloat(event, args[1]);
    f32 b = evtGetFloat(event, args[2]);
    f32 c = evtGetFloat(event, args[3]);
    s32 type = evtGetValue(event, args[4]);
    f32 x;
    f32 y;
    f32 z;
    EffectEntry* eff;

    BtlUnit_GetPos(unit, &x, &y, &z);
    // from the tip of the wand
    x += 28.0f * unit->mSizeMultiplier * unit->faceDirection;
    y += 20.0f * unit->mSizeMultiplier;
    z += 5.0f;
    eff = effMagic1N64Entry(0, type, x, y, z, a, b, c);
    unit->work[4] = (EvtWord)eff;
    ((EffMagic1N64Work*)eff->userdata)->scale = unit->mSizeMultiplier;
    return EVT_RETURN_DONE;
}

static USER_FUNC(eff_staff) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    if (isFirstCall) {
        unit->work[3] = (EvtWord)effThunderflareN64Entry(0, 0.0f, -1000.0f, 0.0f, 1.0f, 60);
    }
    if (!(((EffectEntry*)unit->work[3])->flags & 1)) {
        return EVT_RETURN_DONE;
    }
    wk = ((EffectEntry*)unit->work[3])->userdata;
    wk[10] = unit->mSizeMultiplier;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[2] = x + 30.0f * unit->mSizeMultiplier * unit->faceDirection;
    wk[3] = y + 35.0f * unit->mSizeMultiplier;
    wk[4] = 5.0f + z;
    return EVT_RETURN_BLOCK;
}

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

