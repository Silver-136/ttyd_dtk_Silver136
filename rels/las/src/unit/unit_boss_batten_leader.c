// Palace of Shadow battle unit: Sir Grodus (boss) (`boss_batten_leader`): parts, stats, poses and AI scripts.

#include "version.h"
#include "las.h"
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
#include "battle/battle_damage.h"
#include "battle/unit/satellite_spline.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SATELLITE_COUNT 0   // satellites out after the last sub_call_satellite (its LW(0) count); player_before_phase_event passes it to battenSatelliteToInside
#define UNIT_WORK_HP_STAGE 1          // 0 full; 1/2/3 once HP <= 75/50/25% (monster_before_phase_event prints stg8_las_66/67/68; 3 also switches to pose_table_furafura)
#define UNIT_WORK_MISFIRE_CHANCE 2    // percent chance the next attack fizzles into smoke: -1 at start (first damage to a non-body part makes it 100), else +10 per damage point up to 100; each attack event clears it and rolls against it
#define UNIT_WORK_SELF_SPELL 3        // 4 or 5 while casting shikaeshi or yokeyoke on itself; damage_event then holds the camera 1.5 s, resets it and clears the slot
#define UNIT_WORK_LIGHT_EVENT 4       // event id of the running light_anim_rtn (head lights); restarted when slot 6 changes, deleted in dead_event
#define UNIT_WORK_ROD_LIGHT_EVENT 5   // event id of the running light_rod_anim_rtn (rod light); restarted when slot 7 changes, deleted in dead_event
#define UNIT_WORK_LIGHT_PATTERN 6     // which btl_shuryolight_head_* set light_anim_rtn plays (0 normal, 1 quick, 2-3 tenmetu, 4 random, 5 on, 6 off, 7 dead_event); light_pose_change sets it from the body pose
#define UNIT_WORK_ROD_LIGHT_PATTERN 7 // which rod light light_rod_anim_rtn plays: 8 btl_shuryolight_rod_normal (init, every attack), 9 btl_shuryolight_rod_off (hit on a non-body part)
#define UNIT_WORK_SHIELD_EFFECT 8     // EffectEntry* of the effSyuryouEntry glow while all four satellites are out (check_satellite creates it; deletes it and sets 0 when one is lost)
#define UNIT_WORK_SATELLITE 9         // first of four satellite unit ids (slots 9-12, -1 = empty): sub_call_satellite fills them, player_before_phase_event packs them, check_satellite clears dead ones
#define UNIT_WORK_SATELLITE_SPLINE 13 // first of four SatelliteSpline* move works (slots 13-16, one per satellite slot; 0 when the move ends); see the BUG note in battenSatelliteTo_Init

static Vec nowv;
static f32 maer;
static s32 xlw[16];
static s32 xlw2[16];
static char xmemo[17];
static EventEntry* xevt;
static void _light_point_callback(void* userdata, s32 group, Mtx* modeling);

static USER_FUNC(_set_draw_callback);
static USER_FUNC(btl_hitoffsetx);
static USER_FUNC(btlevtcmd_AnimeGetPose);
static USER_FUNC(datastrcopy);
static USER_FUNC(enzan_lw_load);
static USER_FUNC(enzan_lw_load2);
static USER_FUNC(enzan_lw_save);
static USER_FUNC(enzan_lw_save2);
static USER_FUNC(pose_check_evt_run);
static USER_FUNC(pose_check_evt_stop);
USER_FUNC(_hp_gauge_flag_on);
static USER_FUNC(battenSatelliteToOutside);
static USER_FUNC(battenSatelliteToInside);
static USER_FUNC(battleGetBattenSatelliteInitPos);
static USER_FUNC(check_satellite);
static USER_FUNC(eff_staff);
static USER_FUNC(evt_eff_firechange00);
static USER_FUNC(evt_eff_barriersizechange);
static USER_FUNC(evt_eff_colorchange00);
static USER_FUNC(evt_eff_pokopi_thunder);
extern BattleUnitKind unit_boss_batten_satellite;

static EvtWord batten_leader_zoom[34];
static EvtWord mario_party_zoom[34];
static BattleWeapon weapon_batten_leader_thunder;
static BattleWeapon weapon_batten_leader_fire;
static BattleWeapon weapon_batten_leader_ice;
static BattleWeapon weapon_batten_leader_shikaeshi;
static BattleWeapon weapon_batten_leader_yokeyoke;
static BattleWeapon weapon_batten_leader_stop;
BattleUnitKind unit_boss_batten_leader;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefenseAttr defence_attr_muteki;
static StatusVulnerability regist;
static u8 defencecopy[5] __attribute__((aligned(4)));
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_furafura[18];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[7];
static EvtWord hakai_snd_event[9];
static BattleUnitSetup entry_batten_satellite;

/* light blink sequence read by the light event: {mapobj name, frames} steps, then a NULL name */
typedef struct LightStep {
    const char* name;
    s32 time;
} LightStep;
typedef struct LightSeq1 {
    LightStep steps[1];
    const char* end;
} LightSeq1;
typedef struct LightSeq2 {
    LightStep steps[2];
    const char* end;
} LightSeq2;
typedef struct LightSeq3 {
    LightStep steps[3];
    const char* end;
} LightSeq3;
typedef struct LightSeq4 {
    LightStep steps[4];
    const char* end;
} LightSeq4;
static LightSeq3 btl_shuryolight_pow_normal;
static LightSeq1 btl_shuryolight_pow_tenmetu2;
static LightSeq2 btl_shuryolight_pow_tenmetu;
static LightSeq4 btl_shuryolight_head_1_normal;
static LightSeq4 btl_shuryolight_head_2_normal;
static LightSeq4 btl_shuryolight_head_3_normal;
static LightSeq4 btl_shuryolight_head_1_quick;
static LightSeq4 btl_shuryolight_head_2_quick;
static LightSeq4 btl_shuryolight_head_3_quick;
static LightSeq2 btl_shuryolight_head_1_random;
static LightSeq2 btl_shuryolight_head_2_random;
static LightSeq2 btl_shuryolight_head_3_random;
static LightSeq1 btl_shuryolight_head_1_tenmetu;
static LightSeq1 btl_shuryolight_head_2_tenmetu;
static LightSeq1 btl_shuryolight_head_3_tenmetu;
static LightSeq1 btl_shuryolight_head_1_off;
static LightSeq1 btl_shuryolight_head_2_off;
static LightSeq1 btl_shuryolight_head_3_off;
static LightSeq1 btl_shuryolight_head_1_on;
static LightSeq1 btl_shuryolight_head_2_on;
static LightSeq1 btl_shuryolight_head_3_on;
static LightSeq4 btl_shuryolight_rod_normal;
static LightSeq1 btl_shuryolight_rod_off;
static EvtWord light_anim_rtn[488];
static EvtWord light_rod_anim_rtn[122];
static EvtWord light_pose_change[314];
static EvtWord maiframeanimcheck[21];
static EvtWord init_event[149];
static EvtWord entry_event[94];
static EvtWord damage_event[47];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord damage_sub_event[];
#else
EvtWord damage_sub_event[148];
#endif

static EvtWord phase_event[28];
static EvtWord monster_before_phase_event[195];

#if VERSION <= VERSION_DEMO_JP
static EvtWord dead_event[126];
#else
static EvtWord dead_event[165];
#endif

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
static EvtWord sub_call_satellite[87];
static EvtWord player_before_phase_event[170];
static EvtWord attack_event[141];
static EvtWord batten_leader_thunder_attack_event[377];
static EvtWord batten_leader_fire_attack_event[376];
static EvtWord batten_leader_ice_attack_event[321];
static EvtWord batten_leader_shikaeshi_attack_event[176];
static EvtWord batten_leader_yokeyoke_attack_event[176];
static EvtWord batten_leader_stop_attack_event[328];

static EvtWord wait_event[15];

/**
 * Event script: moves the battle camera to the close view of Sir Grodus (position 83, 84, 349 looking at 83, 60,
 * 0).
 * LW(0) selects the timing: with 2 the move is instant, otherwise it takes 30 frames,
 * and with 1 the script waits 600 ms after it.
 */
static EVT_BEGIN(batten_leader_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
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

/**
 * Camera helper of the Sir Grodus fight: moves the battle camera to a close view of Mario's side (X -120).
 * With LW(0) = 2 it cuts immediately, otherwise it takes 30 frames; with LW(0) = 1 it then waits 600 ms.
 */
static EVT_BEGIN(mario_party_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
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

/** Weapon of Grodus's thunder spell: damage 7. */
static BattleWeapon weapon_batten_leader_thunder = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_batten_leader_fire = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
#if VERSION >= VERSION_US
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of Grodus's ice spell: damage 5; inflicts Frozen (30%). */
static BattleWeapon weapon_batten_leader_ice = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of Grodus's shikaeshi support spell. */
static BattleWeapon weapon_batten_leader_shikaeshi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, // fastChance..galeForceChance
    3, // paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of Grodus's spell that makes a unit dodgy: inflicts Dodgy. */
static BattleWeapon weapon_batten_leader_yokeyoke = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    3, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of Grodus's spell that freezes its target in time: inflicts Stop (30%). */
static BattleWeapon weapon_batten_leader_stop = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Unit data of Grodus in the Palace of Shadow: 50 HP, level 87;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_boss_batten_leader = {
    UNIT_GRODUS, // unitType
    "btl_un_batten_leader", // name
    50, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    87, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    68, // width
    66, // height
#if VERSION <= VERSION_DEMO_JP
    7,
    68,
    0.0f,
    33.0f,
    0.0f,
    0,
    0,
#else
    {7, 70}, // hitOffset
    {0.0f, 33.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {34.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
#endif
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
#if VERSION <= VERSION_DEMO_JP
    0.0f,
    34.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    34.0f,
    0.0f,
    0.0f,
    20.0f,
    42.9f,
    10.0f,
    0.0f,
    33.0f,
    0.0f,
#else
    {34.0f, 0.0f, 0.0f}, // bintaHitOffset
    {20.0f, 42.9f, 10.0f}, // kissHitOffset
    {0.0f, 33.0f, 0.0f}, // cutBaseOffset
#endif
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
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Defence of Grodus against each element (normal, fire, ice, explosion, electric):
 * base defence is 1 against every element.
 */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/**
 * Damage modifiers of Grodus per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Damage modifiers of Grodus while invincible per element (normal, fire, ice, explosion, electric): normal:
 * immune, fire: immune, ice: immune, explosion: immune, electric: immune.
 */
static BattleUnitDefenseAttr defence_attr_muteki = {{DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {30, 30, 30, 100, 30, 100, 100, 30, 100, 50, 100, 50, 100, 30, 30, 0, 0, 100, 30, 100, 100, 1};
#else
static StatusVulnerability regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    100, // poison
    30, // confuse
    100, // electric
    100, // burn
    30, // freeze
    100, // huge
    60, // tiny
    100, // attackUp
    60, // attackDown
    100, // defenseUp
    75, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    30, // slow
    100, // dodgy
    100, // invisible
    1, // ohko
};
#endif

static u8 defencecopy[5] __attribute__((aligned(4))) = { // defenses[5] copied into the part defense
    1, 1, 1, 1, 1,
};

/** Pose table of Grodus: animation names for each pose type (idle, walk, hurt, attack and so on). */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
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

/**
 * Pose table of Grodus once his HP stage reaches 3 (25% HP or less, set by monster_before_phase_event):
 * animation names for each pose type; idle and action use the wobbling X_1, talk T_3.
 */
static PoseTableEntry pose_table_furafura[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "X_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
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

/**
 * Data table of Grodus: event scripts the battle looks up by tag: damage_sub_event, and dead_event (under the literal
 * tag 50 in the early demos, UNIT_DATA_DEAD_EVENT in JP and later).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, damage_sub_event},
#if VERSION <= VERSION_DEMO_JP
    {50, dead_event},
#else
    {UNIT_DATA_DEAD_EVENT, dead_event},
#endif
    {0, NULL},
};

/** Parts of Grodus: 7 parts with their hit boxes, defence tables and pose tables. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_batten_leader", "c_shuryo", 0.0f, 0.0f, 0.0f, 0.0f, 56.0f, 0.0f, -6.0f, 55.0f, 0.0f, 53, 69, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_batten_leader", "c_shuryo", 0.0f, 0.0f, 0.0f, 18.0f, 9.0f, 0.0f, 16.0f, 15.0f, 0.0f, 64, 36, 255, &defence, &defence_attr, kNoHpLoss | kImmuneToStatus, 0, NULL},
    {3, "電源ランプ", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {4, "頭のランプ１", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {5, "頭のランプ２", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {6, "頭のランプ３", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
    {7, "杖のランプ１", "c_shuryo", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table},
};

/** Event script: plays the sound of Grodus's barrier breaking (SFX_BOSS_SRYO_BARRIER_BREAK1). */
static EVT_BEGIN(hakai_snd_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_BARRIER_BREAK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    RETURN
EVT_END()

static USER_FUNC(evt_eff_pokopi_thunder) {
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

/**
 * Script helper: recolours the cloud effect named args[0] to white fading to yellow and lifts it 65 units up
 * and 10 units forward; used for Grodus's lightning cloud.
 */
static USER_FUNC(evt_eff_colorchange00) {
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

/** Script helper: sets the scale of Grodus's barrier effect named args[0] to args[1]. */
static USER_FUNC(evt_eff_barriersizechange) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));

    ((EffSyuryouWork*)eff->userdata)->scale = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper (runs every frame until done): steers the fire effect named args[0] toward the target point
 * (args[2], args[3]) in a wobbling path at 15 units per frame, limiting turns to 15 degrees per frame except when resetting direction.
 * With mode args[1] set to 0 it finishes on arrival;
 * otherwise it then flies off to the left edge (X -270) and is deleted.
 * Used for the fireballs of Grodus's fire spell.
 */
static USER_FUNC(evt_eff_firechange00) {
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

/**
 * Script helper: creates the wand-tip barrier glow effect at the end of Grodus's staff,
 * sized by the unit's size multiplier.
 */
static USER_FUNC(eff_staff) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* barrier;

    barrier = effSyuryouEntry(0, 90, 0.0f, -1000.0f, 0.0f)->userdata;
    BtlUnit_GetPos(unit, &x, &y, &z);
    barrier[1] = x + 6.0f * (unit->sizeMultiplier * unit->faceDirection);
    barrier[2] = y + 68.0f * unit->sizeMultiplier;
    barrier[3] = 5.0f + z;
    barrier[4] = unit->sizeMultiplier;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: counts Grodus's living satellites (his unit work 9..12) and updates his invulnerability.
 * With all four present the barrier effect appears around him and he becomes immune to damage and negative statuses;
 * once a satellite is gone the barrier shatters (with glass effects and a break sound)
 * and his normal-element defence is recalculated from its stored value plus the number of satellites left.
 */
static USER_FUNC(check_satellite) {
    s32 id = BattleTransID(event, UNIT_ID_SELF);
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
            effDelete((EffectEntry*)unit->work[UNIT_WORK_SHIELD_EFFECT]);
            unit->work[UNIT_WORK_SHIELD_EFFECT] = 0;
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
            unit->work[UNIT_WORK_SHIELD_EFFECT] = (EvtWord)effSyuryouEntry(1, 1000, 0.0f, -1000.0f, 0.0f);
        }
        if (unit->work[UNIT_WORK_SHIELD_EFFECT] != 0) {
            f32* barrier = ((EffectEntry*)unit->work[UNIT_WORK_SHIELD_EFFECT])->userdata;
            f32 ex;
            f32 ey;
            f32 ez;

            BtlUnit_GetPos(unit, &ex, &ey, &ez);
            barrier[1] = ex;
            barrier[2] = ey - 12.5f * unit->sizeMultiplier;
            barrier[3] = 10.0f + ez;

            barrier[4] = 2.5f * unit->sizeMultiplier;
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

/**
 * Script helper: returns the starting offset (x, y, z, mirrored by the unit's facing) of satellite args[2]
 * when args[1] satellites surround the unit args[0], from a fixed layout for 1 to 4 satellites.
 * The offsets go to the variables args[3..5].
 */
static USER_FUNC(battleGetBattenSatelliteInitPos) {
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

/** Setup used to spawn one of Grodus's satellites: unit kind, alliance, position and turn phase. */
static BattleUnitSetup entry_batten_satellite = {
    &unit_boss_batten_satellite, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Lamp sequence of Grodus's power lamp: a steady cycle of its lit and dim animations. */
static LightSeq3 btl_shuryolight_pow_normal = {{{"L_1B", -2}, {"L_1A", 80}, {"L_1B", 80}}, NULL};

/** Lamp sequence of Grodus's power lamp: blinking. */
static LightSeq1 btl_shuryolight_pow_tenmetu2 = {{{"L_4E", 0}}, NULL};

/** Lamp sequence of Grodus's power lamp: blinking. */
static LightSeq2 btl_shuryolight_pow_tenmetu = {{{"L_1B", -3}, {"L_1A", 200}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: a steady cycle of its lit and dim animations. */
static LightSeq4 btl_shuryolight_head_1_normal = {{{"L_4A", 400}, {"L_4A", 400}, {"L_4B", 400}, {"L_4A", 400}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: a steady cycle of its lit and dim animations. */
static LightSeq4 btl_shuryolight_head_2_normal = {{{"L_3A", 400}, {"L_3B", 400}, {"L_3A", 400}, {"L_3A", 400}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: a steady cycle of its lit and dim animations. */
static LightSeq4 btl_shuryolight_head_3_normal = {{{"L_2B", 400}, {"L_2A", 400}, {"L_2A", 400}, {"L_2A", 400}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: a fast cycle. */
static LightSeq4 btl_shuryolight_head_1_quick = {{{"L_4A", 200}, {"L_4A", 200}, {"L_4B", 200}, {"L_4A", 200}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: a fast cycle. */
static LightSeq4 btl_shuryolight_head_2_quick = {{{"L_3A", 200}, {"L_3B", 200}, {"L_3A", 200}, {"L_3A", 200}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: a fast cycle. */
static LightSeq4 btl_shuryolight_head_3_quick = {{{"L_2B", 200}, {"L_2A", 200}, {"L_2A", 200}, {"L_2A", 200}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: random flicker. */
static LightSeq2 btl_shuryolight_head_1_random = {{{"L_4A", -4}, {"L_4B", -4}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: random flicker. */
static LightSeq2 btl_shuryolight_head_2_random = {{{"L_3A", -4}, {"L_3B", -4}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: random flicker. */
static LightSeq2 btl_shuryolight_head_3_random = {{{"L_2B", -4}, {"L_2A", -4}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: blinking. */
static LightSeq1 btl_shuryolight_head_1_tenmetu = {{{"L_4E", 0}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: blinking. */
static LightSeq1 btl_shuryolight_head_2_tenmetu = {{{"L_3E", 0}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: blinking. */
static LightSeq1 btl_shuryolight_head_3_tenmetu = {{{"L_2E", 0}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: dark. */
static LightSeq1 btl_shuryolight_head_1_off = {{{"L_4A", -1}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: dark. */
static LightSeq1 btl_shuryolight_head_2_off = {{{"L_3A", -1}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: dark. */
static LightSeq1 btl_shuryolight_head_3_off = {{{"L_2A", -1}}, NULL};

/** Lamp sequence of Grodus's head lamp 1: lit. */
static LightSeq1 btl_shuryolight_head_1_on = {{{"L_4B", -1}}, NULL};

/** Lamp sequence of Grodus's head lamp 2: lit. */
static LightSeq1 btl_shuryolight_head_2_on = {{{"L_3B", -1}}, NULL};

/** Lamp sequence of Grodus's head lamp 3: lit. */
static LightSeq1 btl_shuryolight_head_3_on = {{{"L_2B", -1}}, NULL};

/** Lamp sequence of Grodus's staff lamp: a steady cycle of its lit and dim animations. */
static LightSeq4 btl_shuryolight_rod_normal = {{{"L_3C", 0}, {"L_3B", 1000}, {"L_3D", 0}, {"L_3A", 1000}}, NULL};

/** Lamp sequence of Grodus's staff lamp: dark. */
static LightSeq1 btl_shuryolight_rod_off = {{{"L_3A", -1}}, NULL};

/**
 * Event script: animates Grodus's lamp parts from sequence tables (pointers in LW(5..8),
 * a list of animation names with durations; durations are in ms, 0 waits for the animation to end, -1 ends,
 * -2 blinks randomly, -3 and -4 are random waits). The three head tables run in sibling events; the power table runs in the main event.
 */
static EVT_BEGIN(light_anim_rtn)
    // First head lamp (part 4): play the animation sequence given in LW(6) over and over.
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(6))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(6))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
            ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 4)
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
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, LW(3))
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

    // Second head lamp (part 5): play the sequence given in LW(7).
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(7))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(7))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
            ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 5)
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
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, LW(3))
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

    // Third head lamp: play the sequence given in LW(8).
    BEGIN_SIBLING_EVENT
        SET_INT_READ_ADDR(LW(8))
        LOOP(0)
            READ_INT(LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_INT_READ_ADDR(LW(8))
                READ_INT(LW(1))
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, LW(1))
            READ_INT(LW(1))
            IF_INT_GREATER(LW(1), 0)
                WAIT_MS(LW(1))
            ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 6)
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
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, LW(1))
                        WAIT_MS(LW(2))
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, LW(3))
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

    // Power lamp (part 3): play the sequence given in LW(5), looping in the main event.
    SET_INT_READ_ADDR(LW(5))
    LOOP(0)
        READ_INT(LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_INT_READ_ADDR(LW(5))
            READ_INT(LW(1))
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, LW(1))
        READ_INT(LW(1))
        IF_INT_GREATER(LW(1), 0)
            WAIT_MS(LW(1))
        ELSE
            SWITCH(LW(1))
                CASE_EQUAL(0)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 3)
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
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, LW(1))
                    WAIT_MS(LW(2))
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, LW(3))
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

/** Event script: animates the staff lamp from the sequence table in LW(9), the same format as light_anim_rtn. */
static EVT_BEGIN(light_rod_anim_rtn)
    SET_INT_READ_ADDR(LW(9))
    LOOP(0)
        READ_INT(LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_INT_READ_ADDR(LW(9))
            READ_INT(LW(1))
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 7, LW(1))
        READ_INT(LW(1))
        IF_INT_GREATER(LW(1), 0)
            WAIT_MS(LW(1))
        ELSE
            SWITCH(LW(1))
                CASE_EQUAL(0)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 7)
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
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 7, LW(1))
                    WAIT_MS(LW(2))
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 7, LW(3))
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

/**
 * Event script: when Grodus's body pose changes, chooses the matching lamp sequences.
 * For the casting poses A_1B, A_1C and A_2 the head lamps use their blinking sequences and the power lamp its normal sequence;
 * other poses select their own normal, quick, random, lit, dark or blinking sequences; restarts the lamp events accordingly.
 */
static EVT_BEGIN(light_pose_change)
    CALL(enzan_lw_save2)
    CALL(btlevtcmd_AnimeGetPose, UNIT_ID_SELF, 1, LW(0))

    // Casting poses A_1B, A_1C and A_2: the head lamps use blinking sequences and the power lamp its normal sequence.
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

    // Other poses (hurt, dizzy, dead and so on) use their own mix of steady, blinking, random or dark lamps.
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))

        // Restart the lamp animation event with the chosen sequences.
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(enzan_lw_load2)
    RETURN
EVT_END()

/**
 * Event script: watches Grodus's body animation every frame and runs light_pose_change whenever it changes,
 * so his lamps follow his pose.
 */
static EVT_BEGIN(maiframeanimcheck)
    LOOP(0)
        CALL(btlevtcmd_AnimeGetPose, UNIT_ID_SELF, 1, LW(1))
        IF_STR_NOT_EQUAL(LW(0), LW(1))
            CALL(datastrcopy, LW(1))
            CALL_CHILD(light_pose_change)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Init event of Sir Grodus: installs his wait, phase, attack, damage, confusion and entry events,
 * offsets his HP gauge and sets up his unit work and the poses of his parts,
 * including the links to his satellites (the unit ids kept in unit work).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btl_hitoffsetx, UNIT_ID_SELF, 2, -3)
    CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, -2, -6)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("L_1B"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("L_4B"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("L_3B"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, STRING("L_2B"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 7, STRING("L_3B"))
    CALL(_set_draw_callback)
    CALL(enzan_lw_save)
    SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_normal))
    SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_normal))
    SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_normal))
    SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_normal))
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_PATTERN, LW(0))
    SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))
    CALL(enzan_lw_load)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
    SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    CALL(enzan_lw_load)
    CALL(pose_check_evt_run)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SELF_SPELL, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, -1)
    SET_VALUE(LW(0), 9)
    LOOP(4)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Entry event of the Sir Grodus fight: the status window is hidden,
 * the camera moves to Grodus and he speaks (stg8_las_65) before calling his satellites (sub_call_satellite)
 * and taking his battle stance.
 */
static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    SET_VALUE(LW(0), 2)
    CALL_CHILD(batten_leader_zoom)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("E_1"))
    WAIT_MS(1000)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg8_las_65"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_MS(270)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(6), 4)
    CALL_CHILD(sub_call_satellite)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    CALL(check_satellite)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Damage event of Sir Grodus: default damage handling, then clears UNIT_WORK_SELF_SPELL,
 * which his self-cast Shikaeshi (4) and Yokeyoke (5) spells set; for those it first waits and resets the camera.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SELF_SPELL, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SWITCH(LW(0))
            CASE_EQUAL(4)
            WAIT_MS(1500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            WAIT_MS(300)
            CASE_EQUAL(5)
            WAIT_MS(1500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            WAIT_MS(300)
        END_SWITCH
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SELF_SPELL, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Event script: Grodus's reaction when hit. A zero-damage hit plays the miss sound.
 * A body hit (part 1) plays the damage pose; a hit on another part (the staff) switches the staff lamp off
 * and plays pose D_2.
 * Hits that carry damage code 256 raise his HP gauge flag (JP and later).
 * Final hits on a non-body part set work[2] to 100 if it was negative;
 * otherwise they add ten times the total damage to it, capped at 100.
 */
EVT_BEGIN(damage_sub_event)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(_hp_gauge_flag_on, LW(10))
    END_IF
#endif
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
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
        IF_INT_NOT_EQUAL(LW(0), LW(1))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
            DELETE_EVENT(LW(0))
            SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        END_IF
        CALL(enzan_lw_load)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("D_2"))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(LW(11), 1)
            CALL(btlevtcmd_GetTotalDamage, LW(10), LW(0))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
            IF_INT_LESS(LW(1), 0)
                SET_VALUE(LW(1), 100)
            ELSE
                MULTIPLY(LW(0), 10)
                ADD(LW(1), LW(0))
                IF_INT_GREATER(LW(1), 100)
                    SET_VALUE(LW(1), 100)
                END_IF
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
        END_IF
    END_IF
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * Phase event of Sir Grodus: before the player phase it runs player_before_phase_event,
 * and after the player phase monster_before_phase_event, which handle his satellites and his statuses.
 */
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(player_before_phase_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(monster_before_phase_event)
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Event script: Grodus's turn before the enemy phase, driving his three story stages by HP.
 * At 75 percent HP or less he stops and speaks (stg8_las_66); at 50 percent he says stg8_las_67;
 * at 25 percent he sets his wobbly pose table (pose_table_furafura) and says stg8_las_68.
 */
static EVT_BEGIN(monster_before_phase_event)
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 75)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 1)
            CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(batten_leader_zoom)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("E_1"))
            WAIT_MS(1000)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg8_las_66"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 50)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 2)
            CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(batten_leader_zoom)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg8_las_67"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 25)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 3)
            CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_furafura))
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(batten_leader_zoom)
            CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, POSE_TYPE_TALK)
            CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, POSE_TYPE_NEUTRAL)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg8_las_68"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        END_IF
    END_SWITCH
    LABEL(0)
    RETURN
EVT_END()

/**
 * Dead event of Sir Grodus: his satellites are killed, the status window is hidden,
 * he speaks his last line (stg8_las_69) and the slow death sequence runs.
 * In JP and later, a satellite that is not already at zero HP (damage result 0x200) gets the death sequence
 * (subsetevt_dead_core) and is hidden; the early demos kill the satellites directly (KillUnit).
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    SET_VALUE(LW(0), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(1))
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_GetUnitId, LW(1), LW(1))
#endif
        IF_INT_NOT_EQUAL(LW(1), -1)
#if VERSION <= VERSION_DEMO_JP
            CALL(btlevtcmd_KillUnit, LW(1), 1)
#else
            CALL(btlevtcmd_CheckDamageCode, LW(1), DAMAGE_RESULT_ZERO_HP, LW(2))
            IF_INT_EQUAL(LW(2), 0)
                SET_VALUE(LW(10), LW(1))
                BEGIN_INLINE_EVENT
                    CALL_CHILD(subsetevt_dead_core)
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), kHidden)
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), kNoShadow)
                END_INLINE_EVENT
                WAIT_MS(200)
            END_IF
#endif
        END_IF
        ADD(LW(0), 1)
    END_LOOP
#if VERSION >= VERSION_JP
    WAIT_MS(1000)
#endif
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(pose_check_evt_stop)
    CALL(enzan_lw_save)
    SET_VALUE(LW(5), PTR(&btl_shuryolight_pow_tenmetu))
    SET_VALUE(LW(6), PTR(&btl_shuryolight_head_1_off))
    SET_VALUE(LW(7), PTR(&btl_shuryolight_head_2_off))
    SET_VALUE(LW(8), PTR(&btl_shuryolight_head_3_off))
    SET_VALUE(LW(0), 7)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
    WAIT_MS(750)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("T_5"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("D_4"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg8_las_69"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_4"))
    WAIT_MS(500)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LIGHT_EVENT, LW(0))
    DELETE_EVENT(LW(0))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    DELETE_EVENT(LW(0))
#if VERSION >= VERSION_JP
    SET_VALUE(LW(10), UNIT_ID_SELF)
    CALL_CHILD(subsetevt_dead_core_nospin_norotate)
#endif
    RETURN
EVT_END()

/**
 * Spline path of five control points that Grodus's satellite 1 of 1 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn1_1[] = {
    {38.0f, 45.0f, 5.0f},
    {30.4f, -0.6000023f, -5.0f},
    {19.0f, -15.799999f, -5.0f},
    {7.6f, -0.6000023f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 1 of 2 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn2_1[] = {
    {38.0f, 45.0f, 5.0f},
    {30.4f, -0.6000023f, -5.0f},
    {19.0f, -15.799999f, -5.0f},
    {7.6f, -0.6000023f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 2 of 2 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn2_2[] = {
    {-38.0f, 45.0f, 5.0f},
    {-30.4f, 90.600006f, -5.0f},
    {-19.0f, 90.600006f, -5.0f},
    {-7.6f, 90.600006f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 1 of 3 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn3_1[] = {
    {0.0f, 83.0f, 5.0f},
    {0.0f, 113.4f, -5.0f},
    {0.0f, 121.0f, -5.0f},
    {0.0f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 2 of 3 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn3_2[] = {
    {38.0f, 7.0f, 5.0f},
    {60.8f, -15.799999f, -5.0f},
    {45.600002f, -8.200001f, -5.0f},
    {30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 3 of 3 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn3_3[] = {
    {-38.0f, 7.0f, 5.0f},
    {-60.8f, -15.799999f, -5.0f},
    {-45.600002f, -8.200001f, -5.0f},
    {-30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 1 of 4 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn4_1[] = {
    {38.0f, 83.0f, 5.0f},
    {45.600002f, 98.2f, -5.0f},
    {60.8f, 113.4f, -5.0f},
    {30.4f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 2 of 4 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn4_2[] = {
    {-38.0f, 83.0f, 5.0f},
    {-45.600002f, 98.2f, -5.0f},
    {-60.8f, 113.4f, -5.0f},
    {-30.4f, 83.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 3 of 4 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn4_3[] = {
    {38.0f, 7.0f, 5.0f},
    {45.600002f, -8.200001f, -5.0f},
    {60.8f, -23.400002f, -5.0f},
    {30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/**
 * Spline path of five control points that Grodus's satellite 4 of 4 follows
 * when it is pulled back in to Grodus (battenSatelliteToInside).
 */
static Vec splineTableIn4_4[] = {
    {-38.0f, 7.0f, 5.0f},
    {-45.600002f, -8.200001f, -5.0f},
    {-60.8f, -23.400002f, -5.0f},
    {-30.4f, 7.0f, -5.0f},
    {0.0f, 45.0f, -5.0f},
};

/** Spline path of five control points that Grodus's satellite 1 of 1 follows when it flies out. */
static Vec splineTableOut1_1[] = {
    {0.0f, 45.0f, -5.0f},
    {-53.2f, -8.200001f, -5.0f},
    {-60.8f, 45.0f, -5.0f},
    {45.600002f, 105.8f, -5.0f},
    {38.0f, 45.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 1 of 2 follows when it flies out. */
static Vec splineTableOut2_1[] = {
    {0.0f, 45.0f, -5.0f},
    {7.6f, 90.600006f, -5.0f},
    {19.0f, 105.8f, -5.0f},
    {30.4f, 90.600006f, -5.0f},
    {38.0f, 45.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 2 of 2 follows when it flies out. */
static Vec splineTableOut2_2[] = {
    {0.0f, 45.0f, -5.0f},
    {-7.6f, -0.6000023f, -5.0f},
    {-19.0f, -15.799999f, -5.0f},
    {-30.4f, -0.6000023f, -5.0f},
    {-38.0f, 45.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 1 of 3 follows when it flies out. */
static Vec splineTableOut3_1[] = {
    {0.0f, 45.0f, -5.0f},
    {0.0f, 83.0f, -5.0f},
    {0.0f, 121.0f, -5.0f},
    {0.0f, 113.4f, -5.0f},
    {0.0f, 83.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 2 of 3 follows when it flies out. */
static Vec splineTableOut3_2[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 14.6f, -5.0f},
    {68.4f, -0.6000023f, -5.0f},
    {45.600002f, 7.0f, -5.0f},
    {38.0f, 7.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 3 of 3 follows when it flies out. */
static Vec splineTableOut3_3[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 14.6f, -5.0f},
    {-68.4f, -0.6000023f, -5.0f},
    {-45.600002f, 7.0f, -5.0f},
    {-38.0f, 7.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 1 of 4 follows when it flies out. */
static Vec splineTableOut4_1[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 75.4f, -5.0f},
    {68.4f, 90.600006f, -5.0f},
    {45.600002f, 83.0f, -5.0f},
    {38.0f, 83.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 2 of 4 follows when it flies out. */
static Vec splineTableOut4_2[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 75.4f, -5.0f},
    {-68.4f, 90.600006f, -5.0f},
    {-45.600002f, 83.0f, -5.0f},
    {-38.0f, 83.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 3 of 4 follows when it flies out. */
static Vec splineTableOut4_3[] = {
    {0.0f, 45.0f, -5.0f},
    {38.0f, 14.6f, -5.0f},
    {68.4f, -0.6000023f, -5.0f},
    {45.600002f, 7.0f, -5.0f},
    {38.0f, 7.0f, 5.0f},
};

/** Spline path of five control points that Grodus's satellite 4 of 4 follows when it flies out. */
static Vec splineTableOut4_4[] = {
    {0.0f, 45.0f, -5.0f},
    {-38.0f, 14.6f, -5.0f},
    {-68.4f, -0.6000023f, -5.0f},
    {-45.600002f, 7.0f, -5.0f},
    {-38.0f, 7.0f, 5.0f},
};

/**
 * Event script: puts the blur attribute (0x4000000) on the body parts of satellites in unit work slots 9..12,
 * so they are drawn blurred while they spin.
 */
static EVT_BEGIN(satellite_blur_on)
    SET_VALUE(LW(2), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_SetPartsBlur, LW(3), LW(4), 255, 255, 255, 255, 255, 255, 255, 100, 0)
            CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), kMotionBlur)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(satellite_blur_off)
    SET_VALUE(LW(2), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_OffPartsAttribute, LW(3), LW(4), kMotionBlur)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sub_call_satellite)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(4))
    SET_VALUE(LW(1), 9)
    SET_VALUE(LW(5), 0)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(2))
        IF_INT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_batten_satellite), 0)
            CALL(btlevtcmd_AddPos, LW(3), 0, 45, -5)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(1), LW(3))
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(battenSatelliteToOutside, UNIT_ID_SELF, LW(0))
    WAIT_MS(1000)
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SATELLITE_COUNT, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(player_before_phase_event)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 9)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(3))
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SATELLITE_COUNT, LW(1))
    CALL(battenSatelliteToInside, UNIT_ID_SELF, LW(1))
    CALL_CHILD(satellite_blur_off)
    SET_VALUE(LW(1), 9)
    LOOP(3)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(2))
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
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(3), LW(2))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(4), LW(2))
                SET_VALUE(LW(4), LW(3))
            END_LOOP
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(4), -1)
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    SET_VALUE(LW(6), 2)
    CALL_CHILD(sub_call_satellite)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
        CALL_CHILD(batten_leader_thunder_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, LW(2))
    IF_INT_LESS(LW(2), 2)
        CALL(btlevtcmd_DrawLots, LW(0), 6, 60, 40, 30, 20, 20, 20)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
            CALL_CHILD(batten_leader_thunder_attack_event)
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_fire))
            CALL_CHILD(batten_leader_fire_attack_event)
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_ice))
            CALL_CHILD(batten_leader_ice_attack_event)
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_shikaeshi))
            CALL_CHILD(batten_leader_shikaeshi_attack_event)
            CASE_EQUAL(4)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_yokeyoke))
            CALL_CHILD(batten_leader_yokeyoke_attack_event)
            CASE_EQUAL(5)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_stop))
            CALL_CHILD(batten_leader_stop_attack_event)
        END_SWITCH
    ELSE
        CALL(btlevtcmd_DrawLots, LW(0), 6, 60, 40, 30, 20, 20, 20)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_thunder))
            CALL_CHILD(batten_leader_thunder_attack_event)
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_fire))
            CALL_CHILD(batten_leader_fire_attack_event)
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_ice))
            CALL_CHILD(batten_leader_ice_attack_event)
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_shikaeshi))
            CALL_CHILD(batten_leader_shikaeshi_attack_event)
            CASE_EQUAL(4)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_yokeyoke))
            CALL_CHILD(batten_leader_yokeyoke_attack_event)
            CASE_EQUAL(5)
            SET_VALUE(LW(9), PTR(&weapon_batten_leader_stop))
            CALL_CHILD(batten_leader_stop_attack_event)
        END_SWITCH
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_thunder_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))

    // BUG: no ChoiceSamplingEnemy sets LW(3) here (GetSelectEnemy below does), so the no-target test and
    // AttackDeclare use whatever LW(3) and LW(4) held before (0 in a fresh attack event).
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -80, 0, 25)
    CALL(evt_eff_pokopi_thunder, -160, 0, -25)
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -100, 0, 35)
    CALL(evt_eff_pokopi_thunder, -140, 0, -35)
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff_pokopi_thunder, -120, 0, 25)
    CALL(evt_eff_pokopi_thunder, -120, 0, -25)
    WAIT_MS(100)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_eff_pokopi_thunder, -140, 0, 35)
        CALL(evt_eff_pokopi_thunder, -100, 0, -35)
        WAIT_MS(100)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_THUNDER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_eff_pokopi_thunder, -160, 0, 25)
        CALL(evt_eff_pokopi_thunder, -80, 0, -25)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
            GOTO(90)
            CASE_END
            CASE_EQUAL(PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            GOTO(90)
            CASE_END
            CASE_EQUAL(PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            GOTO(90)
            CASE_EQUAL(PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
            GOTO(90)
            CASE_EQUAL(PRECHECK_HIT)
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
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        LABEL(98)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_fire_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))

    // BUG: no ChoiceSamplingEnemy sets LW(3) here (GetSelectEnemy below does), so the no-target test reads
    // whatever LW(3) held before (0 in a fresh attack event).
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_FIRE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
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
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(PRECHECK_HIT)
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
                CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.005859375f), FLOAT(0.0f), 500)
                CALL(bts_x_flag_add, 1)
            END_SIBLING_EVENT
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        LABEL(97)
        WAIT_MS(150)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(bts_x_flag_wait, LW(7))
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_ice_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_ICE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), FLOAT(-52.0f))
    ADD_FLOAT(LW(1), FLOAT(60.0f))
    ADD_FLOAT(LW(2), FLOAT(10.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    ADD_FLOAT(LW(7), FLOAT(10.0f))
    CALL(evt_eff64, STRING("ICE_EFF_00"), STRING("opuku_wgun_n64"), 0, LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(1.0f), 9, 0, 0, 0)
    WAIT_MS(150)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    LABEL(98)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_shikaeshi_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
    WAIT_MS(140)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-25.0f))
            ADD_FLOAT(LW(1), FLOAT(80.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SELF_SPELL, 4)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_yokeyoke_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
    WAIT_MS(140)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-25.0f))
            ADD_FLOAT(LW(1), FLOAT(80.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SELF_SPELL, 5)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(batten_leader_stop_attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PUTOUT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(eff_staff, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(2000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STUFF_PISECE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_MS(270)
    CALL(enzan_lw_save)
    SET_VALUE(LW(9), PTR(&btl_shuryolight_rod_normal))
    SET_VALUE(LW(0), 8)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(1))
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_PATTERN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
        DELETE_EVENT(LW(0))
        SPAWN_EVENT_GET_ID(PTR(light_rod_anim_rtn), LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROD_LIGHT_EVENT, LW(0))
    END_IF
    CALL(enzan_lw_load)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, LW(1))
    IF_INT_GREATER(LW(1), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MISFIRE_CHANCE, 0)
        CALL(btlevtcmd_GetRandomValue, 100, LW(2))
        IF_INT_LESS(LW(2), LW(1))
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(-52.0f))
            ADD_FLOAT(LW(1), FLOAT(60.0f))
            ADD_FLOAT(LW(2), FLOAT(20.0f))
            CALL(evt_eff, STRING("KEMURI_EFF_00"), STRING("kemuri_test"), 19, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(600)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SRYO_STOP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_cam_get_at, CAMERA_3D, LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), FLOAT(-25.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(100.0f))
    CALL(evt_eff64, STRING("TIMESTOP_EFF_00"), STRING("timestamp_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 150, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2500)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(PRECHECK_HIT)
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
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        LABEL(97)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(check_satellite)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(check_satellite)
    END_LOOP
    RETURN
#if VERSION <= VERSION_DEMO_JP
EVT_END()
static USER_FUNC(btlevtcmd_AnimeGetPose) {
#else
EVT_END()

USER_FUNC(btlevtcmd_AnimeGetPose) {
#endif
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

/**
 * Script helper: saves the 16 local variables LW(0..15) of the running event into the static array xlw
 * so a sub-script can use the registers and restore them.
 */
static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper: restores LW(0..15) from the array saved by enzan_lw_save. */
static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper: saves LW(0..15) into a second static array xlw2 (a separate slot from enzan_lw_save). */
static USER_FUNC(enzan_lw_save2) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw2[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper: restores LW(0..15) from the second array saved by enzan_lw_save2. */
static USER_FUNC(enzan_lw_load2) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw2[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper: copies the string args[0] (at most 16 characters) into the static buffer xmemo. */
static USER_FUNC(datastrcopy) {
    s8* s = (s8*)evtGetValue(event, event->args[0]);
    s32 i;

    for (i = 0; i < 16 && s[i] != 0; i++) {
        xmemo[i] = s[i];
    }
    xmemo[i] = 0;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: starts maiframeanimcheck as a new event for this unit,
 * storing it in xevt and giving it the xmemo buffer to compare poses against.
 */
static USER_FUNC(pose_check_evt_run) {
    xevt = evtEntryType(maiframeanimcheck, event->priority, 0, event->type);
    xevt->unitId = event->unitId;
    xevt->ownerNPC = event->ownerNPC;
    xmemo[0] = 0;
    xevt->lwData[0] = (EvtWord)xmemo;
    xevt->uwBase = event->uwBase;
    xevt->ufBase = event->ufBase;
    return EVT_RETURN_DONE;
}

/** Script helper: stops the pose-watching event that pose_check_evt_run started. */
static USER_FUNC(pose_check_evt_stop) {
    evtDeleteID(xevt->eventId);
    return EVT_RETURN_DONE;
}

/** Script helper: sets hitOffsetX of part args[1] of the unit args[0] to args[2]. */
static USER_FUNC(btl_hitoffsetx) {
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

/**
 * Script helper: hooks the display callback _light_point_callback onto Grodus's body pose
 * and hides the five lamp parts (3..7), which are drawn through the callback instead.
 */
static USER_FUNC(_set_draw_callback) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, UNIT_ID_SELF);
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

/**
 * Display callback for Grodus: when the pose group being drawn is one of his five lamp locators (locator6, 9, 8,
 * 7, 5), draws the matching lamp part's plane shape at that locator.
 */
static void _light_point_callback(void* userdata, s32 group, Mtx* modeling) {
    BattleUnitPart* part = userdata;
    BattleUnit* unit;
    s32 poseId;
    s32 idx;
    s32 pose;
    s32 sh;

    poseId = part->poseId;
    unit = part->owner;

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
        case 4:
            pose = BtlUnit_GetPartsPtr(unit, 7)->poseId;
            sh = animPoseGetShapeIdx(pose, "pPlaneShape36");
            animPoseDrawShape(pose, sh);
            break;
    }
}

// Descriptive name (not in the symbol maps); placeholder unk_las_0000d538, JP las .text+0xD46C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP

/**
 * Script helper: sets the HP gauge flag 1 (HP_GAUGE_FLAG_TIMED) on the unit args[0] (JP and later);
 * damage_sub_event calls it on every final hit.
 */
USER_FUNC(_hp_gauge_flag_on) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit != NULL) {
        unit->hpGauge.flags |= HP_GAUGE_FLAG_TIMED;
    }
    return EVT_RETURN_DONE;
}
#endif

/**
 * Starts moving satellite n of the unit along a spline:
 * allocates a 5-point spline from table, remembers the table on the satellite's body part,
 * and returns TRUE (FALSE if that satellite slot is empty).
 */
static u8 battenSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table) {
    SatelliteSpline* sp;

    // BUG: allocates before the empty-slot test, so an empty slot returns without freeing sp (it stays in the
    // battle heap).
    sp = BTL_ALLOC(sizeof(SatelliteSpline));
    if (unit->work[UNIT_WORK_SATELLITE + n] == -1) {
        return FALSE;
    }

    // BUG: n goes up to 3, so the fourth satellite's spline is stored in work[16], one past the 16 work slots
    // (it lands on currentTarget)
    unit->work[UNIT_WORK_SATELLITE_SPLINE + n] = (EvtWord)sp;
    sp->numPoints = 5;
    sp->points = table;
    sp->frame = 0;
    spline_maketable(5, sp->points, sp->knots, sp->curvature);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + n]), 1)->work[1] = (EvtWord)table;
    return TRUE;
}

/**
 * Advances satellite n of the unit one frame along its spline,
 * moving over 60 frames relative to the unit's position and facing, and frees the spline when finished.
 * Returns TRUE while it is still moving.
 */
static u8 battenSatelliteTo_Main(BattleUnit* unit, s32 n) {
    SatelliteSpline* sp;
    BattleUnit* sat;
    Vec upos;
    Vec pos;

    if (unit->work[UNIT_WORK_SATELLITE + n] == -1) {
        return FALSE;
    }
    sp = (SatelliteSpline*)unit->work[UNIT_WORK_SATELLITE_SPLINE + n];
    if (sp == NULL) {
        return FALSE;
    }
    if (sp->frame >= 60) {
        if (sp != NULL) {
            BTL_FREE(sp);
            unit->work[UNIT_WORK_SATELLITE_SPLINE + n] = 0;
        }
        return FALSE;
    }
    spline_getvalue(&pos, intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 1.0f, sp->frame, 60), sp->numPoints, sp->points, sp->knots, sp->curvature);
    sat = BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + n]);
    if (sat == NULL) {
        return FALSE;
    }
    BtlUnit_GetPos(unit, &upos.x, &upos.y, &upos.z);
    BtlUnit_SetPos(sat, upos.x + pos.x * unit->faceDirection, 5.0f + (upos.y + pos.y), upos.z + pos.z);
    BtlUnit_SetHomePos(sat, upos.x + pos.x * unit->faceDirection, 5.0f + (upos.y + pos.y), upos.z + pos.z);
    sp->frame++;
    return TRUE;
}

/**
 * Script helper: pulls the satellites in to Grodus along the "in" splines paired with the "out" splines they
 * left by; blocks until all have arrived.
 */
static USER_FUNC(battenSatelliteToInside) {
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
            if (unit->work[UNIT_WORK_SATELLITE + i] != -1) {
                EvtWord table = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + i]), 1)->work[1];

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

/**
 * Script helper: sends args[1] satellites out from Grodus to their outer positions along the "out" splines for
 * that many satellites; blocks until all have arrived.
 */
static USER_FUNC(battenSatelliteToOutside) {
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
