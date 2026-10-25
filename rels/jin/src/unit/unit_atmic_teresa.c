#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "driver/camdrv.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "dolphin/mtx.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

typedef struct AtmTeresa {
    u8 state;
    u8 flag;
    s32 anim;
    s32 wait;
    s32 timer;
    Vec vel;
    Vec start;
    Vec target;
    u8 mode;
} AtmTeresa;
AtmTeresa* teresa;
s32 teresa_count;
s32 irand(s32 scalar);

double cos(double x);
double sin(double x);
void teresa_ext_disp(CameraId camera, void* param);

USER_FUNC(atmic_teresa_id_set);
USER_FUNC(teresa_wait_breath2);
USER_FUNC(teresa_wait_breath);
USER_FUNC(teresa_appear_breath);
USER_FUNC(teresa_wait_bikkuri2);
USER_FUNC(teresa_wait_bikkuri);
USER_FUNC(teresa_appear_bikkuri);
USER_FUNC(target_id_set);
void teresa_ext_main(void);
void teresa_ext_init(void);

BattleUnitKind unit_atmic_teresa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static PoseTableEntry pose_table[17];
PoseTableEntry pose_table_kaokakusi[17];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
BattleWeapon weapon_atmic_teresa_press;
BattleWeapon weapon_atmic_teresa_bikkuri;
BattleWeapon weapon_atmic_teresa_surprise;
BattleWeapon weapon_atmic_teresa_breath;
static EvtWord init_event[26];
static EvtWord damage_event[10];
static EvtWord attack_event[101];
EvtWord kaokakusi_event[110];
EvtWord press_event[264];
EvtWord surprise_event[303];
EvtWord bikkuri_event[285];
static EvtWord breath_event[254];
EvtWord breath_event_sub[102];
static EvtWord wait_event[6];
s32 atmic_teresa_id[1];
s32 target_unit_id[1];
s32 target_parts_id[1];
f32 atmic_teresa_scale;
ExtTexInfo teresa_ext_entry_data[17];
EvtWord teresa_entry[19];

BattleUnitKind unit_atmic_teresa = {
    UNIT_ATOMIC_BOO,
    "btl_un_atmic_teresa",
    40,
    0,
    1,
    1,
    52,
    0,
    1,
    100,
    0,
    50,
#ifdef VERSION_G8ME01
    6,
#else
    9999,
#endif
    120,
    110,
    30,
    105,
    0.0f,
    55.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    0.0f,
    46.0f,
    71.5f,
    10.0f,
    0.0f,
    55.0f,
    0.0f,
    120.0f,
    110.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_ATMTLSA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kFloating,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

PoseTableEntry pose_table_kaokakusi[] = {
    {POSE_TYPE_SLEEP, "A_1"},
    {POSE_TYPE_STOP, "A_1"},
    {POSE_TYPE_FREEZE, "A_1"},
    {POSE_TYPE_CONFUSE, "A_1"},
    {POSE_TYPE_POISON, "A_1"},
    {POSE_TYPE_DIZZY, "A_1"},
    {POSE_TYPE_NORMAL, "A_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "A_1"},
    {POSE_TYPE_WALK, "A_1"},
    {POSE_TYPE_DANGER, "A_1"},
    {POSE_TYPE_PERIL, "A_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "A_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_atmic_teresa", "c_atmic_trs", 0.0f, 0.0f, 0.0f, 10.0f, 80.0f, 0.0f, 0.0f, 110.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

static StatusVulnerability regist = {
    40, // sleep
    40, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    80, // defenseDown
    40, // allergic
    0, // fright
    10, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    5, // ohko
};

BattleWeapon weapon_atmic_teresa_press = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_atmic_teresa_bikkuri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    50, // stopChance
    3, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_atmic_teresa_surprise = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_TARGET | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, // poisonTime..poisonStrength
    50, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_atmic_teresa_breath = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
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
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(atmic_teresa_id_set, -2)
    CALL_CHILD(PTR(teresa_entry))
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
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(1), 30)
        ADD(LW(1), 20)
        ADD(LW(1), 20)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 30)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(PTR(kaokakusi_event))
            GOTO(99)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(PTR(press_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(bikkuri_event))
        GOTO(99)
        ELSE
        SET_VALUE(LW(1), 50)
        ADD(LW(1), 50)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 50)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(PTR(surprise_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(breath_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(kaokakusi_event)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 60)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, 4, 0, -1)
    WAIT_FRAMES(20)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, 0, 5, 0, -1)
    WAIT_FRAMES(10)
    LOOP(15)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 12, 0)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_kaokakusi))
    RETURN
EVT_END()

EVT_BEGIN(press_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_press))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_press), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_press))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_press))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -125, 100, LW(2), 120, 10, 4, 0, -1)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("Z_1"))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -125, 0, LW(2), 30, 0, 1, 0, -1)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_press), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_press))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_press), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    LABEL(98)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 50, 6, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(surprise_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_surprise))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_surprise), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_surprise))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_surprise))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, 4, 0, -1)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 0, LW(1), LW(2), 10, 0, 5, 0, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_SURPRISED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 10, 0, 60, 0)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 4)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_surprise), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_surprise))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_surprise), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    LABEL(98)
    WAIT_FRAMES(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -20, 6, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(bikkuri_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_bikkuri))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_bikkuri), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_bikkuri))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_press))
    CALL(teresa_appear_bikkuri)
    WAIT_FRAMES(75)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 5)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_S_ROUND1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_bikkuri)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_SURPRISED2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_bikkuri), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_bikkuri))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_bikkuri), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    LABEL(98)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_WHISTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_bikkuri2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_UNION1"), EVT_NULLPTR, 0, LW(15))
    LOOP(3)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.25f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(30)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(250)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(breath_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_breath), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(target_id_set, LW(3), LW(4))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, 4, 0, -1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 0, 5, 0, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_BLOW2"), EVT_NULLPTR, 0, LW(15))
    CALL(teresa_appear_breath, 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(20)
    END_IF
    SPAWN_EVENT(breath_event_sub)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(20)
    WAIT_FRAMES(120)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(teresa_wait_breath)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_WHISTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_breath2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_UNION1"), EVT_NULLPTR, 0, LW(15))
    LOOP(3)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.25f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(30)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(1500)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    RETURN
EVT_END()

EVT_BEGIN(breath_event_sub)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(99)
    LABEL(91)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 256, LW(5))
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

s32 atmic_teresa_id[] = {
    -1,
};

s32 target_unit_id[] = {
    -1,
};

s32 target_parts_id[] = {
    -1,
};

f32 atmic_teresa_scale = 1.0f;

ExtTexInfo teresa_ext_entry_data[] = {
    {"c_teresa", "S_1", 0.0f},
    {"c_teresa", "S_1", 2.0f},
    {"c_teresa", "S_1", 4.0f},
    {"c_teresa", "S_1", 6.0f},
    {"c_teresa", "S_1", 8.0f},
    {"c_teresa", "S_1", 10.0f},
    {"c_teresa", "S_1", 6.0f},
    {"c_teresa", "S_1", 7.0f},
    {"c_teresa", "A_1", 0.0f},
    {"c_teresa", "A_2", 6.0f},
    {"c_teresa", "A_2", 7.0f},
    {"c_teresa", "A_2", 8.0f},
    {"c_teresa", "A_2", 9.0f},
    {"c_teresa", "A_2", 10.0f},
    {"c_teresa", "A_2", 11.0f},
    {"c_teresa", "A_2", 12.0f},
    {0},
};

EVT_BEGIN(teresa_entry)
    CALL(evt_npc_entry, STRING("ext_teresa"), STRING("c_teresa"))
    CALL(evt_npc_set_position, STRING("ext_teresa"), 0, -1000, 0)
    CALL(evt_ext_entry, 30, PTR(teresa_ext_entry_data), PTR(teresa_ext_init), PTR(teresa_ext_main), 0)
    RETURN
EVT_END()

void teresa_ext_init(void) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    teresa = BattleAlloc(sizeof(AtmTeresa) * 30);
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->flags = 1;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->dir = 90.0f;
        teresa[i].state = 0;
        teresa[i].flag = 0;
        teresa[i].anim = 0;
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            teresa[i].vel = zero;
        }
    }
}

void teresa_ext_main(void) {
    s32 i;
    s32 j;
    ExtdrvPose* pose = extGetPosePtr();
    AtmTeresa* t;
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, atmic_teresa_id[0]);
    BattleUnit* target = BattleGetUnitPtr(_battleWorkPointer, target_unit_id[0]);
    s32 n;
    f32 angle;
    s32 waits[8] = {0, 2, 5, 7, 10, 12, 15, 17};
    f32 ofs[30][2] = {
        {-30.0f, 8.0f}, {-28.0f, 10.0f}, {-26.0f, 6.0f}, {-24.0f, 30.0f}, {-22.0f, 16.0f},
        {-20.0f, 50.0f}, {-18.0f, 14.0f}, {-16.0f, 18.0f}, {-14.0f, 60.0f}, {-12.0f, 34.0f},
        {-10.0f, 66.0f}, {-8.0f, 8.0f}, {-6.0f, 28.0f}, {-4.0f, 40.0f}, {-2.0f, 46.0f},
        {0.0f, 20.0f}, {2.0f, 26.0f}, {4.0f, 52.0f}, {6.0f, 74.0f}, {8.0f, 68.0f},
        {10.0f, 0.0f}, {12.0f, 28.0f}, {14.0f, 44.0f}, {16.0f, 72.0f}, {18.0f, 42.0f},
        {20.0f, 76.0f}, {22.0f, 56.0f}, {24.0f, 32.0f}, {26.0f, 4.0f}, {28.0f, 2.0f},
    };
    u8 modes[7] = {0, 1, 2, 3, 4, 5, 6};
    f32 x, y, z;
    f32 hx, hy, hz;
    f32 sx, sy, sz;

    if (++teresa_count >= 360) {
        teresa_count -= 360;
    }
    if (unit != NULL) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        y += 40.0f;
        BtlUnit_GetScale(unit, &sx, &sy, &sz);
        if (sx > atmic_teresa_scale) {
            sx -= 0.005f;
            if (sx <= atmic_teresa_scale) {
                sx = atmic_teresa_scale;
            }
            BtlUnit_SetScale(unit, sx, sx, sz);
        }
        if (sx < atmic_teresa_scale) {
            sx += 0.005f;
            if (sx >= atmic_teresa_scale) {
                sx = atmic_teresa_scale;
            }
            BtlUnit_SetScale(unit, sx, sx, sz);
        }
    }
    if (target != NULL) {
        BattleUnitPart* part = BtlUnit_GetPartsPtr(target, target_parts_id[0]);
        BtlUnit_GetHitPos(target, part, &hx, &hy, &hz);
    }

    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        t = &teresa[i];
        switch (t->flag) {
            case 1:
                t->anim++;
                if (t->anim <= 30) {
                    pose->texId = 8;
                } else if (t->anim < 44) {
                    pose->texId = (t->anim - 30) / 2 + 9;
                } else {
                    pose->texId = 15;
                }
                break;
            default:
                t->anim++;
                if (t->anim >= 24) {
                    t->anim -= 24;
                }
                pose->texId = t->anim / 3;
                break;
        }
        if (t->wait >= 1) {
            t->wait--;
            continue;
        }
        (*pose->mtx)[0][3] += t->vel.x;
        (*pose->mtx)[1][3] += t->vel.y;
        (*pose->mtx)[2][3] += t->vel.z;
        if ((*pose->mtx)[1][3] < 0.0f) {
            (*pose->mtx)[1][3] = 0.0f;
        }

        switch (t->state) {
            case 0:
                (*pose->mtx)[0][3] = 0.0f;
                (*pose->mtx)[1][3] = -1000.0f;
                (*pose->mtx)[2][3] = 0.0f;
                pose->dir = 90.0f;
                (*pose->mtx)[1][1] = 1.0f;
                (*pose->mtx)[0][0] = 1.0f;
                break;
            case 1:
                pose->flags &= ~2;
                (*pose->mtx)[0][3] = x;
                (*pose->mtx)[1][3] = y;
                (*pose->mtx)[2][3] = z - 5.0f;
                (*pose->mtx)[0][0] = 0.0f;
                (*pose->mtx)[1][1] = 0.0f;
                angle = 0.785398f * i;
                t->vel.x = 2.0f * (f32)cos(angle);
                t->vel.y = 2.0f * (f32)sin(angle);
                t->vel.z = 0.0f;
                t->wait = waits[i];
                t->timer = 50;
                t->state = 5;
                break;
            case 5:
                t->timer--;
                if (t->timer == 25) {
                    atmic_teresa_scale -= 0.05f;
                    if (i % 2 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_DIVIDE1", 0xF1194D80, 0, &pos);
                    }
                }
                (*pose->mtx)[0][0] += 0.02f;
                (*pose->mtx)[1][1] += 0.02f;
                if (t->timer <= 0) {
                    pose->flags |= 2;
                    t->state = 10;
                    t->timer = 150;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                    if (i % 2 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                    }
                }
                break;
            case 10:
                angle = 6.283184f * (0.125f * i + 2.0f * teresa_count / 360.0f);
                t->target.x = 90.0f * (f32)cos(angle) - 100.0f;
                t->target.y = 10.0f;
                t->target.z = 90.0f * (f32)sin(angle);
                if ((*pose->mtx)[0][3] >= -100.0f) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                if (t->timer <= 0) {
                    t->state = 15;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                }
                t->timer--;
                break;
            case 15:
                angle = 6.283184f * (0.125f * i + 2.0f * teresa_count / 360.0f);
                t->target.x = 90.0f * (f32)cos(angle) - 100.0f;
                t->target.y = 10.0f;
                t->target.z = 90.0f * (f32)sin(angle);
                if ((*pose->mtx)[0][3] >= -100.0f) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                n = 0;
                for (j = 0; j < 8; j++) {
                    if (teresa[j].state != 15) {
                        break;
                    }
                    n++;
                }
                if (n == 8) {
                    for (j = 0; j < 8; j++) {
                        AtmTeresa* p = &teresa[j];
                        p->state = 20;
                        p->flag = 1;
                        p->anim = 0;
                        p->vel.x = 0.0f;
                        p->vel.y = 0.0f;
                        p->vel.z = 0.0f;
                    }
                }
                break;
            case 20:
                if (t->anim >= 180) {
                    t->state = 23;
                    t->flag = 0;
                    t->anim = 0;
                    t->timer = 60;
                    t->wait = waits[i];
                    pose->dir = 90.0f;
                }
                break;
            case 23:
                t->target.x = (x - 35.0f) + irand(90);
                t->target.y = (y - 35.0f) + irand(90);
                t->target.z = (z - 30.0f) - 0.05f * i;
                t->timer = 60;
                t->state = 25;
                if (i % 2 == 0) {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (*pose->mtx)[0][3];
                    pos.y = (*pose->mtx)[1][3];
                    pos.z = (*pose->mtx)[2][3];
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                }
                break;
            case 25:
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                if (t->timer <= 0) {
                    t->state = 30;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                    pose->flags &= ~2;
                }
                t->timer--;
                break;
            case 31:
                (*pose->mtx)[0][0] -= 0.0125f;
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                if ((*pose->mtx)[0][0] <= 0.0f) {
                    t->state = 0;
                }
                break;
            case 51:
            case 52:
                pose->flags &= ~2;
                (*pose->mtx)[0][3] = x - 15.0f;
                (*pose->mtx)[1][3] = y - 5.0f;
                (*pose->mtx)[2][3] = 5.0f + z;
                (*pose->mtx)[1][1] = 0.0f;
                (*pose->mtx)[0][0] = 0.0f;
                t->vel.x = 0.0f;
                t->vel.y = 0.0f;
                t->vel.z = 0.0f;
                t->start.x = (*pose->mtx)[0][3];
                t->start.y = (*pose->mtx)[1][3];
                t->start.z = (*pose->mtx)[2][3];
                if (t->state == 51) {
                    t->target.x = (hx + ofs[i][0]) - 200.0f;
                } else {
                    t->target.x = (hx + ofs[i][0]) - 200.0f;
                }
                if (t->state == 51) {
                    t->target.y = (hy + ofs[i][1]) - 50.0f;
                } else {
                    t->target.y = (hy + ofs[i][1]) - 50.0f;
                }
                t->target.z = (hz - 10.0f) - 0.05f * i;
                t->wait = i * 3;
                t->timer = 0;
                t->state = 53;
                t->mode = modes[irand(7)];
                break;
            case 53:
                atmic_teresa_scale -= 0.015f;
                t->state = 55;
                break;
            case 55:
                (*pose->mtx)[0][0] += 0.04f;
                if ((*pose->mtx)[0][0] >= 1.0f) {
                    (*pose->mtx)[0][0] = 1.0f;
                }
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                (*pose->mtx)[0][3] = intplGetValue(t->mode, t->start.x, t->target.x, t->timer, 30);
                (*pose->mtx)[1][3] = intplGetValue(t->mode, t->start.y, t->target.y, t->timer, 30);
                (*pose->mtx)[2][3] = intplGetValue(t->mode, t->start.z, t->target.z, t->timer, 30);
                if (t->timer >= 30) {
                    t->state = 60;
                }
                t->timer++;
                break;
            case 63:
                t->target.x = (x - 35.0f) + irand(90);
                t->target.y = (y - 35.0f) + irand(90);
                t->target.z = (z - 30.0f) - 0.05f * i;
                t->timer = 60;
                t->state = 65;
                if (i % 2 == 0) {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (*pose->mtx)[0][3];
                    pos.y = (*pose->mtx)[1][3];
                    pos.z = (*pose->mtx)[2][3];
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                }
                break;
            case 65:
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 15.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 15.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 15.0f;
                if (t->timer <= 0) {
                    t->state = 70;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                }
                t->timer--;
                break;
            case 71:
                (*pose->mtx)[0][0] -= 0.0125f;
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                if ((*pose->mtx)[0][0] <= 0.0f) {
                    t->state = 0;
                }
                break;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, 1, teresa_ext_disp, NULL);
}

void teresa_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

USER_FUNC(atmic_teresa_id_set) {
    s32 id = evtGetValue(event, event->args[0]);

    atmic_teresa_id[0] = BattleTransID(event, id);
    return EVT_RETURN_DONE;
}

USER_FUNC(target_id_set) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 parts = evtGetValue(event, args[1]);

    target_unit_id[0] = BattleTransID(event, id);
    target_parts_id[0] = parts;
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_appear_bikkuri) {
    s32 i;

    for (i = 0; i < 8; i++) {
        teresa[i].state = 1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_wait_bikkuri) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (teresa[i].state != 15) {
            return EVT_RETURN_BLOCK;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_wait_bikkuri2) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (teresa[i].state != 30) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 8; i++) {
        teresa[i].state = 31;
    }
    atmic_teresa_scale = 1.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_appear_breath) {
    s32 type = evtGetValue(event, event->args[0]);
    s32 i;
    s32 state = (type == 1) ? 51 : 52;

    for (i = 0; i < extGetPoseNum(); i++) {
        teresa[i].state = state;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_wait_breath) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (teresa[i].state != 60) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 30; i++) {
        AtmTeresa* t = &teresa[i];
        t->state = 63;
        t->wait = 30;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(teresa_wait_breath2) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (teresa[i].state != 70) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 30; i++) {
        teresa[i].state = 71;
    }
    atmic_teresa_scale = 1.0f;
    return EVT_RETURN_DONE;
}
