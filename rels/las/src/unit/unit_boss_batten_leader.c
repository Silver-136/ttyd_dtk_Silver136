#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/n64/eff_pokopi_thunder_n64.h"
#include "libc_decls.h"
#include "eff/n64/eff_glass_n64.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"

#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "dolphin/mtx.h"
#include "system.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "eff/eff_syuryou.h"
#include "eff/n64/eff_cloud_n64.h"
#include "driver/animdrv.h"
#include "string.h"
#include "memory.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"

Vec nowv;
f32 maer;
static s32 xlw[16];
s32 xlw2[16];
char xmemo[20];
EventEntry* xevt;
void animPoseSetDispCallBack(s32 poseId, void (*callback)(BattleUnitPart*, s32), void* userdata);
void _light_point_callback(BattleUnitPart* part, s32 group);
typedef struct BattenSpline {
    s32 n;
    Vec* table;
    f32 a[5];
    f32 b[15];
    s32 count;
} BattenSpline;
void spline_maketable(s32 n, Vec* table, f32* a, f32* b);
void spline_getvalue(f32 t, Vec* out, s32 n, Vec* table, f32* a, f32* b);
u8 battenSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table);
u8 battenSatelliteTo_Main(BattleUnit* unit, s32 n);

static USER_FUNC(_set_draw_callback);
USER_FUNC(btl_hitoffsetx);
USER_FUNC(btlevtcmd_AnimeGetPose);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(datastrcopy);
static USER_FUNC(enzan_lw_load);
USER_FUNC(enzan_lw_load2);
static USER_FUNC(enzan_lw_save);
USER_FUNC(enzan_lw_save2);
USER_FUNC(pose_check_evt_run);
USER_FUNC(pose_check_evt_stop);
USER_FUNC(_hp_gauge_flag_on);
USER_FUNC(battenSatelliteToOutside);
USER_FUNC(battenSatelliteToInside);
USER_FUNC(battleGetBattenSatelliteInitPos);
USER_FUNC(check_satellite);
static USER_FUNC(eff_staff);
USER_FUNC(evt_eff_firechange00);
USER_FUNC(evt_eff_barriersizechange);
USER_FUNC(evt_eff_colorchange00);
USER_FUNC(evt_eff_pokopi_thunder);
extern s32 bts_x_flag_add[];
extern s32 bts_x_flag_set[];
extern s32 bts_x_flag_wait[];
extern BattleUnitKind unit_boss_batten_satellite;

EvtWord batten_leader_zoom[34];
static EvtWord mario_party_zoom[34];
BattleWeapon weapon_batten_leader_thunder;
BattleWeapon weapon_batten_leader_fire;
BattleWeapon weapon_batten_leader_ice;
BattleWeapon weapon_batten_leader_shikaeshi;
BattleWeapon weapon_batten_leader_yokeyoke;
BattleWeapon weapon_batten_leader_stop;
BattleUnitKind unit_boss_batten_leader;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
BattleUnitDefenseAttr defence_attr_muteki;
static StatusVulnerability regist;
u8 defencecopy[8] __attribute__((aligned(4)));
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_furafura[18];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[7];
EvtWord hakai_snd_event[9];
BattleUnitSetup entry_batten_satellite;
/* light blink sequence read by the light event: {mapobj name, frames} steps, then a NULL name */
typedef struct LightStep { const char* name; s32 time; } LightStep;
typedef struct LightSeq1 { LightStep steps[1]; const char* end; } LightSeq1;
typedef struct LightSeq2 { LightStep steps[2]; const char* end; } LightSeq2;
typedef struct LightSeq3 { LightStep steps[3]; const char* end; } LightSeq3;
typedef struct LightSeq4 { LightStep steps[4]; const char* end; } LightSeq4;
LightSeq3 btl_shuryolight_pow_normal;
LightSeq1 btl_shuryolight_pow_tenmetu2;
LightSeq2 btl_shuryolight_pow_tenmetu;
LightSeq4 btl_shuryolight_head_1_normal;
LightSeq4 btl_shuryolight_head_2_normal;
LightSeq4 btl_shuryolight_head_3_normal;
LightSeq4 btl_shuryolight_head_1_quick;
LightSeq4 btl_shuryolight_head_2_quick;
LightSeq4 btl_shuryolight_head_3_quick;
LightSeq2 btl_shuryolight_head_1_random;
LightSeq2 btl_shuryolight_head_2_random;
LightSeq2 btl_shuryolight_head_3_random;
LightSeq1 btl_shuryolight_head_1_tenmetu;
LightSeq1 btl_shuryolight_head_2_tenmetu;
LightSeq1 btl_shuryolight_head_3_tenmetu;
LightSeq1 btl_shuryolight_head_1_off;
LightSeq1 btl_shuryolight_head_2_off;
LightSeq1 btl_shuryolight_head_3_off;
LightSeq1 btl_shuryolight_head_1_on;
LightSeq1 btl_shuryolight_head_2_on;
LightSeq1 btl_shuryolight_head_3_on;
LightSeq4 btl_shuryolight_rod_normal;
LightSeq1 btl_shuryolight_rod_off;
EvtWord light_anim_rtn[488];
EvtWord light_rod_anim_rtn[122];
EvtWord light_pose_change[314];
EvtWord maiframeanimcheck[21];
static EvtWord init_event[149];
static EvtWord entry_event[94];
static EvtWord damage_event[47];
EvtWord damage_sub_event[148];
static EvtWord phase_event[28];
EvtWord monster_before_phase_event[195];
static EvtWord dead_event[165];
static Vec splineTableIn1_1[5];
static Vec splineTableIn2_1[5];
static Vec splineTableIn2_2[5];
static Vec splineTableIn3_1[5];
static Vec splineTableIn3_2[5];
static Vec splineTableIn3_3[5];
static Vec splineTableIn4_1[5];
static Vec splineTableIn4_2[5];
static Vec splineTableIn4_3[5];
static Vec splineTableIn4_4[5];
static Vec splineTableOut1_1[5];
static Vec splineTableOut2_1[5];
static Vec splineTableOut2_2[5];
static Vec splineTableOut3_1[5];
static Vec splineTableOut3_2[5];
static Vec splineTableOut3_3[5];
static Vec splineTableOut4_1[5];
static Vec splineTableOut4_2[5];
static Vec splineTableOut4_3[5];
static Vec splineTableOut4_4[5];
static EvtWord satellite_blur_on[42];
static EvtWord satellite_blur_off[29];
EvtWord sub_call_satellite[87];
EvtWord player_before_phase_event[170];
static EvtWord attack_event[141];
EvtWord batten_leader_thunder_attack_event[377];
EvtWord batten_leader_fire_attack_event[376];
EvtWord batten_leader_ice_attack_event[321];
EvtWord batten_leader_shikaeshi_attack_event[176];
EvtWord batten_leader_yokeyoke_attack_event[176];
EvtWord batten_leader_stop_attack_event[328];
static EvtWord wait_event[16];

EVT_BEGIN(batten_leader_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
        ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, 83, 84, 349, 83, 60, 0, LW(1), 11)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(mario_party_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
        ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, -120, 58, 257, -120, 40, 0, LW(1), 11)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

BattleWeapon weapon_batten_leader_thunder = {
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
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleWeapon weapon_batten_leader_fire = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
#ifdef VERSION_G8ME01
    75, // burnChance
#else
    20, // burnChance
#endif
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_batten_leader_ice = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    30, // freezeChance
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_batten_leader_shikaeshi = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, // fastChance..galeForceChance
    3, // paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_batten_leader_yokeyoke = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    3, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_batten_leader_stop = {
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
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    30, // stopChance
    2, // stopTime
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

BattleUnitKind unit_boss_batten_leader = {
    UNIT_GRODUS, // mUnitType
    "btl_un_batten_leader", // name
    50, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    87, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    68, // width
    66, // height
    {7, 70}, // hitOffset
    {0.0f, 33.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {34.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {34.0f, 0.0f, 0.0f}, // bintaHitOffset
    {20.0f, 42.9f, 10.0f}, // kissHitOffset
    {0.0f, 33.0f, 0.0f}, // cutBaseOffset
    68.0f, // cutWidth
    66.0f, // cutHeight
    1, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_SRYO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    7, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

BattleUnitDefenseAttr defence_attr_muteki = {{2, 2, 2, 2, 2}};

static StatusVulnerability regist = {30, 30, 30, 100, 30, 100, 100, 30, 100, 60, 100, 60, 100, 75, 30, 0, 0, 100, 30, 100, 100, 1};

u8 defencecopy[8] __attribute__((aligned(4))) = { // defenses[5] copied into the part defense, padded to 8
    1, 1, 1, 1, 1, 0, 0, 0,
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_2"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static PoseTableEntry pose_table_furafura[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "X_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_2"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_3"},
    {POSE_TYPE_ACTION, "X_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_batten_leader", "c_shuryo", 0.0f, 0.0f, 0.0f, 0.0f, 56.0f, 0.0f, -6.0f, 55.0f, 0.0f, 53, 69, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_batten_leader", "c_shuryo", 0.0f, 0.0f, 0.0f, 18.0f, 9.0f, 0.0f, 16.0f, 15.0f, 0.0f, 64, 36, 255, &defence, &defence_attr, kNoHpLoss | kImmuneToStatus, 0, NULL},
    {3, /*"電源ランプ"*/"\x93\x64\x8C\xB9\x83\x89\x83\x93\x83\x76", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {4, /*"頭のランプ１"*/"\x93\xAA\x82\xCC\x83\x89\x83\x93\x83\x76\x82\x50", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {5, /*"頭のランプ２"*/"\x93\xAA\x82\xCC\x83\x89\x83\x93\x83\x76\x82\x51", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {6, /*"頭のランプ３"*/"\x93\xAA\x82\xCC\x83\x89\x83\x93\x83\x76\x82\x52", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {7, /*"杖のランプ１"*/"\x8F\xF1\x82\xCC\x83\x89\x83\x93\x83\x76\x82\x50", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
};

EVT_BEGIN(hakai_snd_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_BARRIER_BREAK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    RETURN
EVT_END()

USER_FUNC(evt_eff_pokopi_thunder) {
    f32 top;
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    top = 250.0f + y;

    effPokopiThunderN64Entry(0, 30, x, top, z, x, y, z, 1.0f);
    effPokopiThunderN64Entry(1, 30, x, top, z, x, y, z, 1.0f);
    effPokopiThunderN64Entry(1, 30, x, top, z, x, y, z, 1.0f);
    return EVT_RETURN_DONE;
}
USER_FUNC(evt_eff_colorchange00) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((EffCloudN64Work*)eff->userdata)->r = 255;
    ((EffCloudN64Work*)eff->userdata)->g = 255;
    ((EffCloudN64Work*)eff->userdata)->b = 255;
    ((EffCloudN64Work*)eff->userdata)->r2 = 255;
    ((EffCloudN64Work*)eff->userdata)->g2 = 245;
    ((EffCloudN64Work*)eff->userdata)->b2 = 0;
    ((EffCloudN64Work*)eff->userdata)->position.y = 65.0f + ((EffCloudN64Work*)eff->userdata)->position.y;
    ((EffCloudN64Work*)eff->userdata)->position.z = 10.0f + ((EffCloudN64Work*)eff->userdata)->position.z;

    return EVT_RETURN_DONE;
}
USER_FUNC(evt_eff_barriersizechange) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));

    ((EffSyuryouWork*)eff->userdata)->scale = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}
USER_FUNC(evt_eff_firechange00) {
    Mtx m;
    Vec target;
    Vec pos;
    Vec dir;
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));
    s32 mode = evtGetValue(event, args[1]);
    BOOL reset;
    f32 angle1;
    f32 d;

    target.x = evtGetValue(event, args[2]);
    target.y = evtGetValue(event, args[3]);
    target.z = 0.0f;
    pos.x = ((EffCloudN64Work*)eff->userdata)->pos.x;
    pos.y = ((EffCloudN64Work*)eff->userdata)->pos.y;
    pos.z = 0.0f;
    if (isFirstCall) {
        event->userdata3 = 0;
        if (mode == 0) {
            PSVECSubtract(&target, &pos, &nowv);
            PSVECNormalize(&nowv, &nowv);
            maer = rand() % 60 - 30;
            maer += (maer < 0.0f) ? -30 : 31;
            ((EffCloudN64Work*)eff->userdata)->r = 255;
            ((EffCloudN64Work*)eff->userdata)->g = 255;
            ((EffCloudN64Work*)eff->userdata)->b = 255;
            ((EffCloudN64Work*)eff->userdata)->r2 = 255;
            ((EffCloudN64Work*)eff->userdata)->g2 = 65;
            ((EffCloudN64Work*)eff->userdata)->b2 = 20;
        }
    }
    switch (event->userdata3) {
        case 0:
            break;
        case 1:
            target.x = -2700.0f;
            break;
    }
    PSVECSubtract(&target, &pos, &dir);
    reset = FALSE;
    if (PSVECSquareMag(&dir) > 2500.0f) {
        maer += rand() % 41 - 20;
        if (maer > 80.0f || maer < -80.0f || (maer < 20.0f && maer > -20.0f)) {
            maer = rand() % 60 - 30;
            maer += (maer < 0.0f) ? -30 : 31;
            reset = TRUE;
        }
        PSMTXRotRad(m, 'Z', 0.017453292f * maer);
        PSMTXMultVec(m, &dir, &dir);
    } else {
        reset = TRUE;
    }
    PSVECNormalize(&dir, &dir);
    angle1 = angleABf(0.0f, 0.0f, nowv.x, nowv.y);
    d = angleABf(0.0f, 0.0f, dir.x, dir.y);
    d = compAngle(angle1, d);
    if (!reset) {
        if ((d >= 0.0f ? d : -d) > 15.0f) {
            d = (d > 0.0f) ? 15.0f : -15.0f;
        }
    }
    PSMTXRotRad(m, 'Z', 0.017453292f * d);
    PSMTXMultVec(m, &nowv, &nowv);
    PSVECScale(&nowv, &dir, 15.0f);
    PSVECAdd(&pos, &dir, &pos);
    if (pos.y < 10.0f) {
        if (((EffCloudN64Work*)eff->userdata)->pos.y >= 10.0f) {
            nowv.y = -nowv.y;
        } else if (nowv.y < 0.0f) {
            nowv.y = -nowv.y;
        }
    }
    ((EffCloudN64Work*)eff->userdata)->pos.x = pos.x;
    ((EffCloudN64Work*)eff->userdata)->pos.y = pos.y;
    switch (event->userdata3) {
        case 0:
            if (PSVECSquareDistance(&pos, &target) < 225.0f) {
                if (mode) {
                    event->userdata3 = 1;
                } else {
                    return 1;
                }
            }
            break;
        case 1:
            if (pos.x <= -270.0f) {
                effSoftDelete(eff);
                return 1;
            }
            break;
    }
    return 0;
}
static USER_FUNC(eff_staff) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    wk = effSyuryouEntry(0, 90, 0.0f, -1000.0f, 0.0f)->userdata;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[1] = x + 6.0f * (unit->mSizeMultiplier * unit->faceDirection);
    wk[2] = y + 68.0f * unit->mSizeMultiplier;
    wk[3] = 5.0f + z;
    wk[4] = unit->mSizeMultiplier;
    return EVT_RETURN_DONE;
}
USER_FUNC(check_satellite) {
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part;
    BattleUnitPart* part2;
    EventEntry* evt;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 count;
    BOOL all;

    count = 0;
    all = FALSE;
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    for (i = 9; i < 13; i++) {
        if (unit->work[i] != -1) {
            if (BattleGetUnitPtr(_battleWorkPointer, unit->work[i]) != NULL) {
                count++;
            } else {
                unit->work[i] = -1;
            }
        }
    }
    if (count >= 4) {
        all = TRUE;
    }
    if (!all) {
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        part2 = BtlUnit_GetPartsPtr(unit, 2);
        if (part->attributes & kImmuneToNegativeStatus) {
            evt = evtEntryType(hakai_snd_event, event->priority, 0, event->type);
            evt->unitId = event->unitId;
            evt->ownerNPC = event->ownerNPC;
            for (i = 0; i < 16; i++) {
                evt->lwData[i] = event->lwData[i];
            }
            for (i = 0; i < 3; i++) {
                evt->lfData[i] = event->lfData[i];
            }
            evt->uwBase = event->uwBase;
            evt->ufBase = event->ufBase;
            effDelete((EffectEntry*)unit->work[8]);
            unit->work[8] = 0;
            BtlUnit_GetPos(unit, &x, &y, &z);
            for (i = 0; i < 20; i++) {
                s32 w = BtlUnit_GetWidth(unit) + 15;
                s32 h = BtlUnit_GetHeight(unit) + 15;
                s32 dx = irand(w) - w / 2;
                s32 dy = irand(h) - h / 2;

                effGlassN64Entry(2, 60, x + dx, y + dy, z, 1.0f);
            }
        }
        part->attributes &= ~kImmuneToNegativeStatus;
        part2->attributes &= ~kImmuneToDamageAndStatus;
        part->defenseAttr = &defence_attr;
        for (i = 0; i < 5; i++) {
            part->defense->defenses[i] = defencecopy[i];
        }
        part->defense->defenses[0] += count;
    } else {
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        part2 = BtlUnit_GetPartsPtr(unit, 2);
        if (!(part->attributes & kImmuneToNegativeStatus)) {
            f32 px;
            f32 py;
            f32 pz;

            BtlUnit_snd_se(unit, "SFX_BOSS_SRYO_LIGHT_ROUND1", 0xF1194D80, 0);
            BtlUnit_GetPos(unit, &px, &py, &pz);
            unit->work[8] = (EvtWord)effSyuryouEntry(1, 1000, 0.0f, -1000.0f, 0.0f);
        }
        if (unit->work[8] != 0) {
            f32* ud = ((EffectEntry*)unit->work[8])->userdata;
            f32 ex;
            f32 ey;
            f32 ez;

            BtlUnit_GetPos(unit, &ex, &ey, &ez);
            ud[1] = ex;
            ud[2] = ey - 12.5f * unit->mSizeMultiplier;
            ud[3] = 10.0f + ez;

            ud[4] = 2.5f * unit->mSizeMultiplier;
        }
        part->attributes |= kImmuneToNegativeStatus;
        part2->attributes |= kImmuneToDamageAndStatus;
        part->defenseAttr = &defence_attr_muteki;
        for (i = 0; i < 5; i++) {
            part->defense->defenses[i] = defencecopy[i];
        }
        part->defense->defenses[0] += count;
    }
    return EVT_RETURN_DONE;
}
USER_FUNC(battleGetBattenSatelliteInitPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 num;
    s32 index;
    s32 id;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    static const Vec VecData[] = {
        // 1 satellite
        {38.0f, 45.0f, 5.0f},
        // 2 satellites
        {38.0f, 45.0f, 5.0f},
        {-38.0f, 45.0f, 5.0f},
        // 3 satellites
        {0.0f, 83.0f, 5.0f},
        {38.0f, 7.0f, 5.0f},
        {-38.0f, 7.0f, 5.0f},
        // 4 satellites
        {38.0f, 83.0f, 5.0f},
        {-38.0f, 83.0f, 5.0f},
        {38.0f, 7.0f, 5.0f},
        {-38.0f, 7.0f, 5.0f},
    };
    const Vec* p;
    BattleWork* wp = _battleWorkPointer;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    num = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    x = args[3];
    y = args[4];
    z = args[5];
    p = VecData;
    for (i = 0; i < num; i++) {
        for (j = 0; j < i; j++) {
            p++;
        }
    }
    for (j = 0; j < index; j++) {
        p++;
    }
    evtSetValue(event, x, (s32)(p->x * unit->faceDirection));
    evtSetValue(event, y, (s32)p->y);
    evtSetValue(event, z, (s32)p->z);
    return EVT_RETURN_DONE;
}

BattleUnitSetup entry_batten_satellite = {
    &unit_boss_batten_satellite, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

LightSeq3 btl_shuryolight_pow_normal = {{{"L_1B", -2}, {"L_1A", 80}, {"L_1B", 80}}, NULL};

LightSeq1 btl_shuryolight_pow_tenmetu2 = {{{"L_4E", 0}}, NULL};

LightSeq2 btl_shuryolight_pow_tenmetu = {{{"L_1B", -3}, {"L_1A", 200}}, NULL};

LightSeq4 btl_shuryolight_head_1_normal = {{{"L_4A", 400}, {"L_4A", 400}, {"L_4B", 400}, {"L_4A", 400}}, NULL};

LightSeq4 btl_shuryolight_head_2_normal = {{{"L_3A", 400}, {"L_3B", 400}, {"L_3A", 400}, {"L_3A", 400}}, NULL};

LightSeq4 btl_shuryolight_head_3_normal = {{{"L_2B", 400}, {"L_2A", 400}, {"L_2A", 400}, {"L_2A", 400}}, NULL};

LightSeq4 btl_shuryolight_head_1_quick = {{{"L_4A", 200}, {"L_4A", 200}, {"L_4B", 200}, {"L_4A", 200}}, NULL};

LightSeq4 btl_shuryolight_head_2_quick = {{{"L_3A", 200}, {"L_3B", 200}, {"L_3A", 200}, {"L_3A", 200}}, NULL};

LightSeq4 btl_shuryolight_head_3_quick = {{{"L_2B", 200}, {"L_2A", 200}, {"L_2A", 200}, {"L_2A", 200}}, NULL};

LightSeq2 btl_shuryolight_head_1_random = {{{"L_4A", -4}, {"L_4B", -4}}, NULL};

LightSeq2 btl_shuryolight_head_2_random = {{{"L_3A", -4}, {"L_3B", -4}}, NULL};

LightSeq2 btl_shuryolight_head_3_random = {{{"L_2B", -4}, {"L_2A", -4}}, NULL};

LightSeq1 btl_shuryolight_head_1_tenmetu = {{{"L_4E", 0}}, NULL};

LightSeq1 btl_shuryolight_head_2_tenmetu = {{{"L_3E", 0}}, NULL};

LightSeq1 btl_shuryolight_head_3_tenmetu = {{{"L_2E", 0}}, NULL};

LightSeq1 btl_shuryolight_head_1_off = {{{"L_4A", -1}}, NULL};

LightSeq1 btl_shuryolight_head_2_off = {{{"L_3A", -1}}, NULL};

LightSeq1 btl_shuryolight_head_3_off = {{{"L_2A", -1}}, NULL};

LightSeq1 btl_shuryolight_head_1_on = {{{"L_4B", -1}}, NULL};

LightSeq1 btl_shuryolight_head_2_on = {{{"L_3B", -1}}, NULL};

LightSeq1 btl_shuryolight_head_3_on = {{{"L_2B", -1}}, NULL};

LightSeq4 btl_shuryolight_rod_normal = {{{"L_3C", 0}, {"L_3B", 1000}, {"L_3D", 0}, {"L_3A", 1000}}, NULL};

LightSeq1 btl_shuryolight_rod_off = {{{"L_3A", -1}}, NULL};

EVT_BEGIN(light_anim_rtn)
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(6))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(6))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, -2, 4, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
                ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 4)
                    CASE_EQUAL(-1)
                    BREAK_LOOP
                    CASE_EQUAL(-2)
                    LOOP(0)
                        CALL(evt_sub_random, 3000, LW(0))
                        ADD(LW(0), 3000)
                        WAIT_MS(LW(0))
                        CALL(evt_sub_random, 100, LW(0))
                        DIVIDE(LW(0), 30)
                        IF_INT_GREATER(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                    END_LOOP
                    READ_INT4(LW(1), LW(2), LW(3), LW(4))
                    LOOP(LW(0))
                        CALL(btlevtcmd_AnimeChangePose, -2, 4, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, -2, 4, LW(3))
                        WAIT_MS(LW(4))
                    END_LOOP
                    CASE_EQUAL(-3)
                    CALL(evt_sub_random, 3000, LW(0))
                    CALL(evt_sub_random, 1000, LW(2))
                    ADD(LW(0), LW(2))
                    ADD(LW(0), 700)
                    WAIT_MS(LW(0))
                    CASE_EQUAL(-4)
                    CALL(evt_sub_random, 300, LW(0))
                    ADD(LW(0), 100)
                    WAIT_MS(LW(0))
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(7))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(7))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, -2, 5, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
                ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 5)
                    CASE_EQUAL(-1)
                    BREAK_LOOP
                    CASE_EQUAL(-2)
                    LOOP(0)
                        CALL(evt_sub_random, 3000, LW(0))
                        ADD(LW(0), 3000)
                        WAIT_MS(LW(0))
                        CALL(evt_sub_random, 100, LW(0))
                        DIVIDE(LW(0), 30)
                        IF_INT_GREATER(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                    END_LOOP
                    READ_INT4(LW(1), LW(2), LW(3), LW(4))
                    LOOP(LW(0))
                        CALL(btlevtcmd_AnimeChangePose, -2, 5, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, -2, 5, LW(3))
                        WAIT_MS(LW(4))
                    END_LOOP
                    CASE_EQUAL(-3)
                    CALL(evt_sub_random, 3000, LW(0))
                    CALL(evt_sub_random, 1000, LW(2))
                    ADD(LW(0), LW(2))
                    ADD(LW(0), 700)
                    WAIT_MS(LW(0))
                    CASE_EQUAL(-4)
                    CALL(evt_sub_random, 300, LW(0))
                    ADD(LW(0), 100)
                    WAIT_MS(LW(0))
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(8))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(8))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, -2, 6, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
                ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 6)
                    CASE_EQUAL(-1)
                    BREAK_LOOP
                    CASE_EQUAL(-2)
                    LOOP(0)
                        CALL(evt_sub_random, 3000, LW(0))
                        ADD(LW(0), 3000)
                        WAIT_MS(LW(0))
                        CALL(evt_sub_random, 100, LW(0))
                        DIVIDE(LW(0), 30)
                        IF_INT_GREATER(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                    END_LOOP
                    READ_INT4(LW(1), LW(2), LW(3), LW(4))
                    LOOP(LW(0))
                        CALL(btlevtcmd_AnimeChangePose, -2, 6, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, -2, 6, LW(3))
                        WAIT_MS(LW(4))
                    END_LOOP
                    CASE_EQUAL(-3)
                    CALL(evt_sub_random, 3000, LW(0))
                    CALL(evt_sub_random, 1000, LW(2))
                    ADD(LW(0), LW(2))
                    ADD(LW(0), 700)
                    WAIT_MS(LW(0))
                    CASE_EQUAL(-4)
                    CALL(evt_sub_random, 300, LW(0))
                    ADD(LW(0), 100)
                    WAIT_MS(LW(0))
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    SET_INT_READ_ADDR(LW(5))
    LOOP(0)
        READ_INT(LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_INT_READ_ADDR(LW(5))
            READ_INT(LW(1))
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 3, LW(1))
        READ_INT(LW(1))
        IF_INT_GREATER(LW(1), 0)
            WAIT_MS(LW(1))
            ELSE
            SWITCH(LW(1))
                CASE_EQUAL(0)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 3)
                CASE_EQUAL(-1)
                BREAK_LOOP
                CASE_EQUAL(-2)
                LOOP(0)
                    CALL(evt_sub_random, 3000, LW(0))
                    ADD(LW(0), 3000)
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 100, LW(0))
                    DIVIDE(LW(0), 30)
                    IF_INT_GREATER(LW(0), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
                READ_INT4(LW(1), LW(2), LW(3), LW(4))
                LOOP(LW(0))
                    CALL(btlevtcmd_AnimeChangePose, -2, 3, LW(1))
                    WAIT_MS(LW(2))
                    CALL(btlevtcmd_AnimeChangePose, -2, 3, LW(3))
                    WAIT_MS(LW(4))
                END_LOOP
                CASE_EQUAL(-3)
                CALL(evt_sub_random, 3000, LW(0))
                CALL(evt_sub_random, 1000, LW(2))
                ADD(LW(0), LW(2))
                ADD(LW(0), 700)
                WAIT_MS(LW(0))
                CASE_EQUAL(-4)
                CALL(evt_sub_random, 300, LW(0))
                ADD(LW(0), 100)
                WAIT_MS(LW(0))
            END_SWITCH
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(light_rod_anim_rtn)
    SET_INT_READ_ADDR(LW(9))
    LOOP(0)
        READ_INT(LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_INT_READ_ADDR(LW(9))
            READ_INT(LW(1))
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 7, LW(1))
        READ_INT(LW(1))
        IF_INT_GREATER(LW(1), 0)
            WAIT_MS(LW(1))
            ELSE
            SWITCH(LW(1))
                CASE_EQUAL(0)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 7)
                CASE_EQUAL(-1)
                BREAK_LOOP
                CASE_EQUAL(-2)
                LOOP(0)
                    CALL(evt_sub_random, 3000, LW(0))
                    ADD(LW(0), 3000)
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 100, LW(0))
                    DIVIDE(LW(0), 30)
                    IF_INT_GREATER(LW(0), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
                READ_INT4(LW(1), LW(2), LW(3), LW(4))
                LOOP(LW(0))
                    CALL(btlevtcmd_AnimeChangePose, -2, 7, LW(1))
                    WAIT_MS(LW(2))
                    CALL(btlevtcmd_AnimeChangePose, -2, 7, LW(3))
                    WAIT_MS(LW(4))
                END_LOOP
                CASE_EQUAL(-3)
                CALL(evt_sub_random, 3000, LW(0))
                CALL(evt_sub_random, 1000, LW(2))
                ADD(LW(0), LW(2))
                ADD(LW(0), 700)
                WAIT_MS(LW(0))
                CASE_EQUAL(-4)
                CALL(evt_sub_random, 300, LW(0))
                ADD(LW(0), 100)
                WAIT_MS(LW(0))
            END_SWITCH
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(light_pose_change)
    CALL(enzan_lw_save2)
    CALL(btlevtcmd_AnimeGetPose, -2, 1, LW(0))
    IF_STR_EQUAL(LW(0), STRING("A_1B"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 2)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("A_1C"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 2)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("A_2"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 2)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("H_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_quick))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_quick))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_quick))
        SET_VALUE(LW(0), 1)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("D_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_tenmetu2))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 3)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("N_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_off))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_off))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_off))
        SET_VALUE(LW(0), 6)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("K_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_random))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_random))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_random))
        SET_VALUE(LW(0), 4)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("Q_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_tenmetu2))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 3)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("X_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_tenmetu2))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 3)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("Y_1"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_on))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_on))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_on))
        SET_VALUE(LW(0), 5)
        GOTO(0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("D_2"))
        CALL(enzan_lw_save)
        SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
        SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_tenmetu))
        SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_tenmetu))
        SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_tenmetu))
        SET_VALUE(LW(0), 2)
        GOTO(0)
    END_IF
    CALL(enzan_lw_save)
    SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
    SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_normal))
    SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_normal))
    SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_normal))
    SET_VALUE(LW(0), 0)
    LABEL(0)
    CALL(btlevtcmd_GetUnitWork, -2, 6, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 6, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(enzan_lw_load2)
    RETURN
EVT_END()

EVT_BEGIN(maiframeanimcheck)
    LOOP(0)
        CALL(btlevtcmd_AnimeGetPose, -2, 1, LW(1))
        IF_STR_NOT_EQUAL(LW(0), LW(1))
            CALL(datastrcopy, LW(1))
            CALL_CHILD(PTR(light_pose_change))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btl_hitoffsetx, -2, 2, -3)
    CALL(btlevtcmd_SetHpGaugeOffset, -2, -2, -6)
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("L_1B"))
    CALL(btlevtcmd_AnimeChangePose, -2, 4, STRING("L_4B"))
    CALL(btlevtcmd_AnimeChangePose, -2, 5, STRING("L_3B"))
    CALL(btlevtcmd_AnimeChangePose, -2, 6, STRING("L_2B"))
    CALL(btlevtcmd_AnimeChangePose, -2, 7, STRING("L_3B"))
    CALL(_set_draw_callback)
    CALL(enzan_lw_save)
    SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
    SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_normal))
    SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_normal))
    SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_normal))
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 6, LW(0))
    SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    CALL(enzan_lw_load)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
    SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(enzan_lw_load)
    CALL(pose_check_evt_run)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, -1)
    SET_VALUE(LW(0), 9)
    LOOP(4)
        CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    SET_VALUE(LW(0), 2)
    CALL_CHILD(batten_leader_zoom)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 2, STRING("stg8_las_65"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_MS(270)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(6), 4)
    CALL_CHILD(sub_call_satellite)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_ACTION)
    CALL(check_satellite)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SWITCH(LW(0))
            CASE_EQUAL(4)
            WAIT_MS(1500)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(300)
            CASE_EQUAL(5)
            WAIT_MS(1500)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(300)
        END_SWITCH
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(damage_sub_event)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(_hp_gauge_flag_on, LW(10))
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, LW(10), LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), 1, POSE_TYPE_DAMAGE)
        ELSE
        CALL(enzan_lw_save)
        SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_off))
        SET_VALUE(LW(0), 9)
        CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
        IF_INT_NOT_EQUAL(LW(0), LW(1))
            CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
            CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
            DELETE_EVENT(LW(0))
            SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
        END_IF
        CALL(enzan_lw_load)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("D_2"))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(LW(11), 1)
            CALL(btlevtcmd_GetTotalDamage, LW(10), LW(0))
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
            IF_INT_LESS(LW(1), 0)
                SET_VALUE(LW(1), 100)
                ELSE
                MULTIPLY(LW(0), 10)
                ADD(LW(1), LW(0))
                IF_INT_GREATER(LW(1), 100)
                    SET_VALUE(LW(1), 100)
                END_IF
            END_IF
            CALL(btlevtcmd_SetUnitWork, -2, 2, LW(1))
        END_IF
    END_IF
    WAIT_MS(500)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(player_before_phase_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(monster_before_phase_event))
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(monster_before_phase_event)
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 75)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(PTR(batten_leader_zoom))
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1"))
            WAIT_MS(1000)
            CALL(evt_msg_print, 2, STRING("stg8_las_66"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 50)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(PTR(batten_leader_zoom))
            CALL(evt_msg_print, 2, STRING("stg8_las_67"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 25)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 3)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_furafura))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(PTR(batten_leader_zoom))
            CALL(btlevtcmd_SetTalkPoseType, -2, 65)
            CALL(btlevtcmd_SetStayPoseType, -2, 43)
            CALL(evt_msg_print, 2, STRING("stg8_las_68"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
        END_IF
    END_SWITCH
    LABEL(0)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    SET_VALUE(LW(0), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(1))
        CALL(btlevtcmd_GetUnitId, LW(1), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_CheckDamageCode, LW(1), 512, LW(2))
            IF_INT_EQUAL(LW(2), 0)
                SET_VALUE(LW(10), LW(1))
                BEGIN_INLINE_EVENT
                    CALL_CHILD(PTR(subsetevt_dead_core))
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 33554432)
                END_INLINE_EVENT
                WAIT_MS(200)
            END_IF
        END_IF
        ADD(LW(0), 1)
    END_LOOP
    WAIT_MS(1000)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(pose_check_evt_stop)
    CALL(enzan_lw_save)
    SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_tenmetu))
    SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_off))
    SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_off))
    SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_off))
    SET_VALUE(LW(0), 7)
    CALL(btlevtcmd_GetUnitWork, -2, 6, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 6, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_3"))
    WAIT_MS(750)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("T_5"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("D_4"))
    CALL(evt_msg_print, 2, STRING("stg8_las_69"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_4"))
    WAIT_MS(500)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
    DELETE_EVENT(LW(0))
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
    DELETE_EVENT(LW(0))
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

static Vec splineTableIn1_1[] = {
    {38.0f, 45.0f, 5.0f},
    {30.4f, -0.6000023f, -5.0f},
    {19.0f, -15.799999f, -5.0f},
    {7.6f, -0.6000023f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn2_1[] = {
    {38.0f, 45.0f, 5.0f},
    {30.4f, -0.6000023f, -5.0f},
    {19.0f, -15.799999f, -5.0f},
    {7.6f, -0.6000023f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn2_2[] = {
    {-38.0f, 45.0f, 5.0f},
    {-30.4f, 90.600006f, -5.0f},
    {-19.0f, 90.600006f, -5.0f},
    {-7.6f, 90.600006f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn3_1[] = {
    {0.0f, 83.0f, 5.0f},
    {0.0f, 113.4f, -5.0f},
    {0.0f, 121.0f, -5.0f},
    {0.0f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn3_2[] = {
    {38.0f, 7.0f, 5.0f},
    {60.8f, -15.799999f, -5.0f},
    {45.600002f, -8.200001f, -5.0f},
    {30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn3_3[] = {
    {-38.0f, 7.0f, 5.0f},
    {-60.8f, -15.799999f, -5.0f},
    {-45.600002f, -8.200001f, -5.0f},
    {-30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn4_1[] = {
    {38.0f, 83.0f, 5.0f},
    {45.600002f, 98.2f, -5.0f},
    {60.8f, 113.4f, -5.0f},
    {30.4f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn4_2[] = {
    {-38.0f, 83.0f, 5.0f},
    {-45.600002f, 98.2f, -5.0f},
    {-60.8f, 113.4f, -5.0f},
    {-30.4f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn4_3[] = {
    {38.0f, 7.0f, 5.0f},
    {45.600002f, -8.200001f, -5.0f},
    {60.8f, -23.400002f, -5.0f},
    {30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableIn4_4[] = {
    {-38.0f, 7.0f, 5.0f},
    {-45.600002f, -8.200001f, -5.0f},
    {-60.8f, -23.400002f, -5.0f},
    {-30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

static Vec splineTableOut1_1[] = {
    {0.0f, 45.0f, -5.0f},
    {-53.2f, -8.200001f, -5.0f},
    {-60.8f, 45.0f, -5.0f},
    {45.600002f, 105.8f, -5.0f},
    {38.0f, 45.0f, 5.0f},
};

static Vec splineTableOut2_1[] = {
    {0.0f, 45.0f, -5.0f},
    {7.6f, 90.600006f, -5.0f},
    {19.0f, 105.8f, -5.0f},
    {30.4f, 90.600006f, -5.0f},
    {38.0f, 45.0f, 5.0f},
};

static Vec splineTableOut2_2[] = {
    {0.0f, 45.0f, -5.0f},
    {-7.6f, -0.6000023f, -5.0f},
    {-19.0f, -15.799999f, -5.0f},
    {-30.4f, -0.6000023f, -5.0f},
    {-38.0f, 45.0f, 5.0f},
};

static Vec splineTableOut3_1[] = {
    {0.0f, 45.0f, -5.0f},
    {0.0f, 83.0f, -5.0f},
    {0.0f, 121.0f, -5.0f},
    {0.0f, 113.4f, -5.0f},
    {0.0f, 83.0f, 5.0f},
};

static Vec splineTableOut3_2[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 14.6f, -5.0f},
    {68.4f, -0.6000023f, -5.0f},
    {45.600002f, 7.0f, -5.0f},
    {38.0f, 7.0f, 5.0f},
};

static Vec splineTableOut3_3[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 14.6f, -5.0f},
    {-68.4f, -0.6000023f, -5.0f},
    {-45.600002f, 7.0f, -5.0f},
    {-38.0f, 7.0f, 5.0f},
};

static Vec splineTableOut4_1[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 75.4f, -5.0f},
    {68.4f, 90.600006f, -5.0f},
    {45.600002f, 83.0f, -5.0f},
    {38.0f, 83.0f, 5.0f},
};

static Vec splineTableOut4_2[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 75.4f, -5.0f},
    {-68.4f, 90.600006f, -5.0f},
    {-45.600002f, 83.0f, -5.0f},
    {-38.0f, 83.0f, 5.0f},
};

static Vec splineTableOut4_3[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 14.6f, -5.0f},
    {68.4f, -0.6000023f, -5.0f},
    {45.600002f, 7.0f, -5.0f},
    {38.0f, 7.0f, 5.0f},
};

static Vec splineTableOut4_4[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 14.6f, -5.0f},
    {-68.4f, -0.6000023f, -5.0f},
    {-45.600002f, 7.0f, -5.0f},
    {-38.0f, 7.0f, 5.0f},
};

static EVT_BEGIN(satellite_blur_on)
    SET_VALUE(LW(2), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_SetPartsBlur, LW(3), LW(4), 255, 255, 255, 255, 255, 255, 255, 100, 0)
            CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), 67108864)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(satellite_blur_off)
    SET_VALUE(LW(2), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_OffPartsAttribute, LW(3), LW(4), 67108864)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sub_call_satellite)
    CALL(btlevtcmd_GetUnitId, -2, LW(4))
    SET_VALUE(LW(1), 9)
    SET_VALUE(LW(5), 0)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(2))
        IF_INT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_batten_satellite), 0)
            CALL(btlevtcmd_AddPos, LW(3), 0, 45, -5)
            CALL(btlevtcmd_SetUnitWork, -2, LW(1), LW(3))
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
            ADD(LW(0), 1)
            ADD(LW(5), 1)
            IF_INT_GREATER_OR_EQUAL(LW(5), LW(6))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL_CHILD(satellite_blur_on)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(battenSatelliteToOutside, -2, LW(0))
    WAIT_MS(1000)
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(player_before_phase_event)
    CALL(btlevtcmd_StopWaitEvent, -2)
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            ADD(LW(0), 1)
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(LW(0), 4)
        GOTO(99)
    END_IF
    CALL_CHILD(satellite_blur_on)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(1))
    CALL(battenSatelliteToInside, -2, LW(1))
    CALL_CHILD(satellite_blur_off)
    SET_VALUE(LW(1), 9)
    LOOP(3)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(2))
        IF_INT_EQUAL(LW(2), -1)
            SET_VALUE(LW(4), LW(1))
            SET_VALUE(LW(3), 999999999)
            LOOP(0)
                IF_INT_EQUAL(LW(3), 999999999)
                    SET_VALUE(LW(3), LW(1))
                    ADD(LW(3), 1)
                ELSE
                    ADD(LW(3), 1)
                    ADD(LW(3), 0)
                END_IF
                SUBTRACT(LW(3), 0)
                IF_INT_GREATER(LW(3), 12)
                    ADD(LW(3), 0)
                    BREAK_LOOP
                END_IF
                ADD(LW(3), 0)
                CALL(btlevtcmd_GetUnitWork, -2, LW(3), LW(2))
                CALL(btlevtcmd_SetUnitWork, -2, LW(4), LW(2))
                SET_VALUE(LW(4), LW(3))
            END_LOOP
            CALL(btlevtcmd_SetUnitWork, -2, LW(4), -1)
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    SET_VALUE(LW(6), 2)
    CALL_CHILD(sub_call_satellite)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
        CALL_CHILD(PTR(batten_leader_thunder_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(2))
    IF_INT_LESS(LW(2), 2)
        CALL(btlevtcmd_DrawLots, LW(0), 6, 60, 40, 30, 20, 20, 20)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
            CALL_CHILD(PTR(batten_leader_thunder_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_fire))
            CALL_CHILD(PTR(batten_leader_fire_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_ice))
            CALL_CHILD(PTR(batten_leader_ice_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_shikaeshi))
            CALL_CHILD(PTR(batten_leader_shikaeshi_attack_event))
            CASE_EQUAL(4)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_yokeyoke))
            CALL_CHILD(PTR(batten_leader_yokeyoke_attack_event))
            CASE_EQUAL(5)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_stop))
            CALL_CHILD(PTR(batten_leader_stop_attack_event))
        END_SWITCH
        ELSE
        CALL(btlevtcmd_DrawLots, LW(0), 6, 60, 40, 30, 20, 20, 20)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
            CALL_CHILD(PTR(batten_leader_thunder_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_fire))
            CALL_CHILD(PTR(batten_leader_fire_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_ice))
            CALL_CHILD(PTR(batten_leader_ice_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_shikaeshi))
            CALL_CHILD(PTR(batten_leader_shikaeshi_attack_event))
            CASE_EQUAL(4)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_yokeyoke))
            CALL_CHILD(PTR(batten_leader_yokeyoke_attack_event))
            CASE_EQUAL(5)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_stop))
            CALL_CHILD(PTR(batten_leader_stop_attack_event))
        END_SWITCH
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_thunder_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -80, 0, 25)
    CALL(evt_eff_pokopi_thunder, -160, 0, -25)
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -100, 0, 35)
    CALL(evt_eff_pokopi_thunder, -140, 0, -35)
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -120, 0, 25)
    CALL(evt_eff_pokopi_thunder, -120, 0, -25)
    WAIT_MS(100)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_eff_pokopi_thunder, -140, 0, 35)
        CALL(evt_eff_pokopi_thunder, -100, 0, -35)
        WAIT_MS(100)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_eff_pokopi_thunder, -160, 0, 25)
        CALL(evt_eff_pokopi_thunder, -80, 0, -25)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
        GOTO(98)
        LABEL(91)
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 1)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        LABEL(98)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_fire_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_FIRE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(5), LW(0))
    ADD_FLOAT(LW(0), -40)
    ADD_FLOAT(LW(5), -400)
    CALL(evt_eff, STRING(""), STRING("hibashira"), 4, LW(0), LW(1), LW(2), LW(5), LW(1), LW(2), 20, FLOAT(1.0f), 60, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(bts_x_flag_set, 0)
    SET_VALUE(LW(7), 1)
    BEGIN_SIBLING_EVENT
        CALL(bts_x_flag_add, 1)
    END_SIBLING_EVENT
    WAIT_MS(600)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(1)
            SET_VALUE(LW(6), 91)
            CASE_END
            CASE_DEFAULT
            SET_VALUE(LW(6), 97)
            CASE_END
        END_SWITCH
        GOTO(LW(6))
        LABEL(90)
        GOTO(97)
        LABEL(91)
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 1)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
            ADD(LW(7), 1)
            BEGIN_SIBLING_EVENT
                CALL(evt_cam_shake, 4, FLOAT(0.005859375f), FLOAT(0.0f), 500)
                CALL(bts_x_flag_add, 1)
            END_SIBLING_EVENT
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        LABEL(97)
        WAIT_MS(150)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(bts_x_flag_wait, LW(7))
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_ice_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_ICE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), FLOAT(-52.0f))
    ADD_FLOAT(LW(1), FLOAT(60.0f))
    ADD_FLOAT(LW(2), FLOAT(10.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    ADD_FLOAT(LW(7), FLOAT(10.0f))
    CALL(evt_eff64, STRING("ICE_EFF_00"), STRING("opuku_wgun_n64"), 0, LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(1.0f), 9, 0, 0, 0)
    WAIT_MS(150)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_ACTION)
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_ACTION)
    LABEL(98)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_shikaeshi_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1C"))
    WAIT_MS(140)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-25.0f))
            ADD_FLOAT(LW(1), FLOAT(80.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 4)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_yokeyoke_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1C"))
    WAIT_MS(140)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-25.0f))
            ADD_FLOAT(LW(1), FLOAT(80.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 5)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(batten_leader_stop_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 7, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SRYO_STOP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_cam_get_at, 4, LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), FLOAT(-25.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(100.0f))
    CALL(evt_eff64, STRING("TIMESTOP_EFF_00"), STRING("timestamp_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 150, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2500)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(1)
            SET_VALUE(LW(6), 91)
            CASE_END
            CASE_DEFAULT
            SET_VALUE(LW(6), 97)
            CASE_END
        END_SWITCH
        GOTO(LW(6))
        LABEL(90)
        GOTO(97)
        LABEL(91)
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 1)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        LABEL(97)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(check_satellite)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(check_satellite)
    END_LOOP
    RETURN
EVT_END_PAD()

USER_FUNC(btlevtcmd_AnimeGetPose) {
    s32 id;
    EvtWord* args = event->args;
    s32 partNo;
    BattleUnitPart* part;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partNo = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partNo);

    evtSetValue(event, args[2], (EvtWord)animPoseGetCurrentAnim(part->poseId));
    return EVT_RETURN_DONE;
}
static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}
static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}
USER_FUNC(enzan_lw_save2) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw2[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}
USER_FUNC(enzan_lw_load2) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw2[i];
    }
    return EVT_RETURN_DONE;
}
USER_FUNC(datastrcopy) {
    s8* s = (s8*)evtGetValue(event, event->args[0]);
    s32 i;

    for (i = 0; i < 16 && s[i] != 0; i++) {
        xmemo[i] = s[i];
    }
    xmemo[i] = 0;
    return EVT_RETURN_DONE;
}
USER_FUNC(pose_check_evt_run) {
    xevt = evtEntryType(maiframeanimcheck, event->priority, 0, event->type);
    xevt->unitId = event->unitId;
    xevt->ownerNPC = event->ownerNPC;
    xmemo[0] = 0;
    xevt->lwData[0] = (EvtWord)xmemo;
    xevt->uwBase = event->uwBase;
    xevt->ufBase = event->ufBase;
    return EVT_RETURN_DONE;
}
USER_FUNC(pose_check_evt_stop) {
    evtDeleteID(xevt->eventId);
    return EVT_RETURN_DONE;
}
USER_FUNC(btl_hitoffsetx) {
    s32 id;
    EvtWord* args = event->args;
    s32 partNo;
    BattleUnitPart* part;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    partNo = evtGetValue(event, args[1]);
    part = BattleGetUnitPartsPtr(id, partNo);

    part->hitOffsetX = evtGetValue(event, args[2]);
    return EVT_RETURN_DONE;
}
static USER_FUNC(_set_draw_callback) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);
    BattleUnitPart* p3;
    BattleUnitPart* p4;
    BattleUnitPart* p5;
    BattleUnitPart* p6;
    BattleUnitPart* p7;

    animPoseSetDispCallBack(part->poseId, _light_point_callback, part);
    p3 = BtlUnit_GetPartsPtr(unit, 3);
    p4 = BtlUnit_GetPartsPtr(unit, 4);
    p5 = BtlUnit_GetPartsPtr(unit, 5);
    p6 = BtlUnit_GetPartsPtr(unit, 6);
    p7 = BtlUnit_GetPartsPtr(unit, 7);
    p3->attributes |= kHidden;
    p4->attributes |= kHidden;
    p5->attributes |= kHidden;
    p6->attributes |= kHidden;
    p7->attributes |= kHidden;
    return EVT_RETURN_DONE;
}
void _light_point_callback(BattleUnitPart* part, s32 group) {
    BattleUnit* unit;
    s32 poseId;
    s32 idx;

    poseId = part->poseId;
    unit = part->mOwner;

    if (strcmp("locator6", animPoseGetGroupName(poseId, group)) == 0) {
        idx = 0;
    } else if (strcmp("locator9", animPoseGetGroupName(poseId, group)) == 0) {
        idx = 1;
    } else if (strcmp("locator8", animPoseGetGroupName(poseId, group)) == 0) {
        idx = 2;
    } else if (strcmp("locator7", animPoseGetGroupName(poseId, group)) == 0) {
        idx = 3;
    } else if (strcmp("locator5", animPoseGetGroupName(poseId, group)) == 0) {
        idx = 4;
    } else {
        idx = -1;
    }
    switch (idx) {
        case 0: {
                s32 pose = BtlUnit_GetPartsPtr(unit, 3)->poseId;
                s32 sh = animPoseGetShapeIdx(pose, "pPlaneShape34");
                animPoseDrawShape(pose, sh);
            }
            break;
        case 1: {
                s32 pose = BtlUnit_GetPartsPtr(unit, 4)->poseId;
                s32 sh = animPoseGetShapeIdx(pose, "pPlaneShape37");
                animPoseDrawShape(pose, sh);
            }
            break;
        case 2: {
                s32 pose = BtlUnit_GetPartsPtr(unit, 5)->poseId;
                s32 sh = animPoseGetShapeIdx(pose, "pPlaneShape36");
                animPoseDrawShape(pose, sh);
            }
            break;
        case 3: {
                s32 pose = BtlUnit_GetPartsPtr(unit, 6)->poseId;
                s32 sh = animPoseGetShapeIdx(pose, "pPlaneShape35");
                animPoseDrawShape(pose, sh);
            }
            break;
        case 4: {
                s32 pose = BtlUnit_GetPartsPtr(unit, 7)->poseId;
                s32 sh = animPoseGetShapeIdx(pose, "pPlaneShape36");
                animPoseDrawShape(pose, sh);
            }
            break;
    }
}
USER_FUNC(_hp_gauge_flag_on) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit != NULL) {
        unit->hpGauge.flags |= 1;
    }
    return EVT_RETURN_DONE;
}
u8 battenSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table) {
    BattenSpline* sp;

    sp = BTL_ALLOC(sizeof(BattenSpline));
    if (unit->work[9 + n] == -1) {
        return FALSE;
    }
    unit->work[13 + n] = (EvtWord)sp;
    sp->n = 5;
    sp->table = table;
    sp->count = 0;
    spline_maketable(5, sp->table, sp->a, sp->b);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[9 + n]), 1)->work[1] = (EvtWord)table;
    return TRUE;
}
u8 battenSatelliteTo_Main(BattleUnit* unit, s32 n) {
    BattenSpline* sp;
    BattleUnit* sat;
    Vec upos;
    Vec pos;

    if (unit->work[9 + n] == -1) {
        return FALSE;
    }
    sp = (BattenSpline*)unit->work[13 + n];
    if (sp == NULL) {
        return FALSE;
    }
    if (sp->count >= 60) {
        if (sp != NULL) {
            BTL_FREE(sp);
            unit->work[13 + n] = 0;
        }
        return FALSE;
    }
    spline_getvalue(intplGetValue(4, 0.0f, 1.0f, sp->count, 60), &pos, sp->n, sp->table, sp->a, sp->b);
    sat = BattleGetUnitPtr(_battleWorkPointer, unit->work[9 + n]);
    if (sat == NULL) {
        return FALSE;
    }
    BtlUnit_GetPos(unit, &upos.x, &upos.y, &upos.z);
    BtlUnit_SetPos(sat, upos.x + pos.x * unit->faceDirection, 5.0f + (upos.y + pos.y), upos.z + pos.z);
    BtlUnit_SetHomePos(sat, upos.x + pos.x * unit->faceDirection, 5.0f + (upos.y + pos.y), upos.z + pos.z);
    sp->count++;
    return TRUE;
}
USER_FUNC(battenSatelliteToInside) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 num = evtGetValue(event, args[1]);
    BattleUnit* unit;
    u8 moving;
    s32 i;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        for (i = 0; i < 4; i++) {
            if (unit->work[9 + i] != -1) {
                EvtWord table = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[9 + i]), 1)->work[1];

                if (table == (EvtWord)splineTableOut1_1) {
                    battenSatelliteTo_Init(unit, i, splineTableIn1_1);
                }
                if (table == (EvtWord)splineTableOut2_1) {
                    battenSatelliteTo_Init(unit, i, splineTableIn2_1);
                }
                if (table == (EvtWord)splineTableOut2_2) {
                    battenSatelliteTo_Init(unit, i, splineTableIn2_2);
                }
                if (table == (EvtWord)splineTableOut3_1) {
                    battenSatelliteTo_Init(unit, i, splineTableIn3_1);
                }
                if (table == (EvtWord)splineTableOut3_2) {
                    battenSatelliteTo_Init(unit, i, splineTableIn3_2);
                }
                if (table == (EvtWord)splineTableOut3_3) {
                    battenSatelliteTo_Init(unit, i, splineTableIn3_3);
                }
                if (table == (EvtWord)splineTableOut4_1) {
                    battenSatelliteTo_Init(unit, i, splineTableIn4_1);
                }
                if (table == (EvtWord)splineTableOut4_2) {
                    battenSatelliteTo_Init(unit, i, splineTableIn4_2);
                }
                if (table == (EvtWord)splineTableOut4_3) {
                    battenSatelliteTo_Init(unit, i, splineTableIn4_3);
                }
                if (table == (EvtWord)splineTableOut4_4) {
                    battenSatelliteTo_Init(unit, i, splineTableIn4_4);
                }
            }
        }
    }
    moving = FALSE;
    for (i = 0; i < 4; i++) {
        if (battenSatelliteTo_Main(unit, i) == TRUE) {
            moving = TRUE;
        }
    }
    return moving == FALSE ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}
USER_FUNC(battenSatelliteToOutside) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 num = evtGetValue(event, args[1]);
    BattleUnit* unit;
    u8 moving;
    s32 i;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        switch (num) {
        case 1:
            i = 0;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut1_1) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 2:
            i = 0;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut2_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut2_2) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 3:
            i = 0;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut3_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut3_2) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut3_3) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 4:
            i = 0;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut4_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut4_2) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut4_3) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (battenSatelliteTo_Init(unit, i, splineTableOut4_4) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        default:
            return EVT_RETURN_DONE;
        }
    }
    moving = FALSE;
    for (i = 0; i < 4; i++) {
        if (battenSatelliteTo_Main(unit, i) == TRUE) {
            moving = TRUE;
        }
    }
    return moving == FALSE ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}
