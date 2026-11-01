// Mario's battle attacks and badge weapons: jumps, hammers, items and special actions.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/sac/sac_bakugame.h"
#include "battle/sac/sac_deka.h"
#include "battle/sac/sac_genki.h"
#include "battle/sac/sac_muki.h"
#include "battle/sac/sac_scissor.h"
#include "battle/sac/sac_suki.h"
#include "battle/sac/sac_zubastar.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "driver/npcdrv.h"
#include "battle/battle_audience.h"
#include "eff/eff_fire.h"
#include "battle/battle_damage.h"
#include "battle/unit_work.h"
static USER_FUNC(_get_mario_hammer_lv);
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_energy.h"
#include "eff/eff_hibashira.h"
#include "eff/eff_majinai.h"
#include "eff/n64/eff_kameki_tornade_n64.h"
#include "eff/n64/eff_whirlwind_n64.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "battle/battle_database_common.h"
#include "icon_ids.h"

USER_FUNC(_battle_majinai_effect);
USER_FUNC(_majinai_powerup_check);
USER_FUNC(btlevtcmd_AnnounceMessage);
USER_FUNC(_paper_light_off);
USER_FUNC(_fire_wave);
USER_FUNC(_mario_fire_ball_controll);
USER_FUNC(_kaiten_hammer_acrobat_rotate);
USER_FUNC(_hammer_star_effect);
USER_FUNC(mario_get_renzoku_count_max);
USER_FUNC(_record_renzoku_count);
USER_FUNC(_tatsumaki_effect);
USER_FUNC(_wait_jyabara_hit_iron_frame);
USER_FUNC(_bgset_iron_frame_check);
USER_FUNC(_jump_star_effect);
USER_FUNC(_whirlwind_effect);

BattleWeapon marioDefaultWeapon_Jump;
BattleWeapon marioWeapon_KururinJump;
BattleWeapon marioWeapon_KururinJump2;
BattleWeapon marioWeapon_JyabaraJump;
BattleWeapon marioWeapon_JyabaraJumpFailed;
BattleWeapon marioDefaultWeapon_FirstAttackKaitenHammer;
BattleWeapon marioDefaultWeapon_FirstAttackUltraHammer;
BattleWeapon marioDefaultWeapon_Hammer;
BattleWeapon marioWeapon_KaitenHammer;
BattleWeapon marioWeapon_KaitenHammerInvolved;
BattleWeapon marioWeapon_UltraHammer;
BattleWeapon marioWeapon_UltraHammerInvolved;
BattleWeapon marioWeapon_UltraHammerFinish;
BattleWeapon badgeWeapon_TugitugiJump;
BattleWeapon badgeWeapon_GatsunJump;
BattleWeapon badgeWeapon_DokanJump;
BattleWeapon badgeWeapon_RenzokuJump;
BattleWeapon badgeWeapon_TatsumakiJump;
BattleWeapon badgeWeapon_TatsumakiJumpInvolved;
BattleWeapon badgeWeapon_MiniminiFumi;
BattleWeapon badgeWeapon_NemuraseFumi;
BattleWeapon badgeWeapon_FunyafunyaJump;
BattleWeapon badgeWeapon_GatsunHammer;
BattleWeapon badgeWeapon_DokanHammer;
BattleWeapon badgeWeapon_JishinHammer;
BattleWeapon badgeWeapon_UltraJishinHammer;
BattleWeapon badgeWeapon_HammerNageru;
BattleWeapon badgeWeapon_TsuranukiNaguri;
BattleWeapon badgeWeapon_ConfuseHammer;
BattleWeapon badgeWeapon_FireNaguri;
BattleWeapon badgeWeapon_FireNaguriFailed;
BattleWeapon badgeWeapon_IceNaguri;
BattleWeapon badgeWeapon_Charge;
BattleWeapon badgeWeapon_ChargeP;
BattleWeapon badgeWeapon_SuperCharge;
BattleWeapon badgeWeapon_SuperChargeP;
BattleWeapon badgeWeapon_2KaiItem;
BattleWeapon badgeWeapon_3KaiItem;
BattleWeapon badgeWeapon_DefenceCommand;
BattleWeapon badgeWeapon_DefenceCommandP;
BattleWeapon marioWeapon_BakuGame;
BattleWeapon marioWeapon_Scissor;
BattleWeapon marioWeapon_ZubaStar;
BattleWeapon marioWeapon_Genki0;
BattleWeapon marioWeapon_Genki1;
BattleWeapon marioWeapon_Deka;
BattleWeapon marioWeapon_Muki;
BattleWeapon marioWeapon_Suki;
BattleWeapon* superActionTable[18];
static EvtWord marioAttackEvent_NormalJump[VERSION_PICK(889, 963)];
static EvtWord marioAttackEvent_KururinJump[1210];
EvtWord marioAttackEvent_JyabaraJump[];
EvtWord marioAttackEvent_GatsuDokaJump[VERSION_PICK(624, 638)];
#if VERSION <= VERSION_DEMO_US
EvtWord marioAttackEvent_TatsumakiJump[1661];
#else
EvtWord marioAttackEvent_TatsumakiJump[VERSION_PICK(1668, 1679)];
#endif
EvtWord marioAttackEvent_RenzokuJump[840];
EvtWord marioAttackEvent_TugiTugiJump[VERSION_PICK(793, 775)];
#if VERSION <= VERSION_DEMO_US
EvtWord marioAttackEvent_NormalHammer_Core[1312];
#else
EvtWord marioAttackEvent_NormalHammer_Core[VERSION_PICK(1326, 1333)];
#endif
static EvtWord marioAttackEvent_KaitenHammer[1060];
EvtWord marioAttackEvent_FirstAttackKaitenHammer[19];
EvtWord marioAttackEvent_JishinHammer[VERSION_PICK(834, 845)];
EvtWord marioAttackEvent_UltraJishinHammer[665];
EvtWord marioAttackEvent_HammerRotate[25];
EvtWord marioAttackEvent_HammerNageru[VERSION_PICK(446, 457)];
EvtWord marioAttackEvent_HammerNageru_object[VERSION_PICK(240, 247)];
EvtWord marioAttackEvent_FireNaguri[702];
EvtWord marioAttackEvent_FireNaguri_object[374];
EvtWord _NormalCharge_core[103];
EvtWord _SuperCharge_core[103];
EvtWord marioAttackEvent_GatsunJump[7];
EvtWord marioAttackEvent_DokanJump[7];
EvtWord marioAttackEvent_MiniminiFumi[4];
EvtWord marioAttackEvent_NemuraseFumi[4];
EvtWord marioAttackEvent_FunyafunyaJump[4];
static EvtWord marioAttackEvent_NormalHammer[7];
EvtWord marioAttackEvent_GatsunHammer[7];
EvtWord marioAttackEvent_IceNaguri[7];
EvtWord marioAttackEvent_TsuranukiHammer[7];
EvtWord marioAttackEvent_NormalCharge[7];
EvtWord marioAttackEvent_NormalChargeP[7];
EvtWord marioAttackEvent_SuperCharge[7];
EvtWord marioAttackEvent_SuperChargeP[7];
EvtWord marioAttackEvent_MajinaiPowerUpCheck[35];
EvtWord marioAttackEvent_MajinaiJump[43];
EvtWord _MajinaiPoseReset[14];
EvtWord marioAttackEvent_MajinaiDefenceUp[29];
EvtWord marioAttackEvent_MajinaiExpUp[27];

/**
 * Mario's Jump (0 FP): a stomp with a second one on a timed press, damage from his boots level.
 * Also his first strike after jumping on an enemy in the field (_GetFirstAttackWeapon in battle_seq.c).
 */
BattleWeapon marioDefaultWeapon_Jump = {
    "btl_wn_mario_normal_jump", // name
    ICON_ITEM_BOOTS, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 2, 1, 1, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 1, 1, 0, 0, 0}, // stageEffectRates
#endif
};

/** Spin Jump (2 FP, Super Boots): Mario's first stomp; the dive after it uses marioWeapon_KururinJump2. */
BattleWeapon marioWeapon_KururinJump = {
    "btl_wn_mario_kururin_jump", // name
    ICON_ITEM_ULTRA_BOOTS, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Spin Jump's second hit: the dive Mario makes after spinning above the target on a successful press.
 * JP and earlier give it less damage at the higher boots levels.
 */
BattleWeapon marioWeapon_KururinJump2 = {
    "btl_wn_mario_kururin_jump", // name
    ICON_ITEM_ULTRA_BOOTS, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
#if VERSION >= VERSION_US
    {1, 2, 3, 4, 5, 6, 0, 0}, // hpDamageWork
#else
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
#endif
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 5, 5, 100, 0, 20, 10, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 5, 5, 100, 0, 10, 10, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Spring Jump (4 FP, Ultra Boots): Mario springs high into the air and drops on the chosen enemy.
 * JP and earlier give it less damage with the Ultra Boots.
 */
BattleWeapon marioWeapon_JyabaraJump = {
    "btl_wn_mario_jyabara_jump", // name
    ICON_ITEM_SUPER_BOOTS, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
#if VERSION >= VERSION_US
    {1, 2, 2, 4, 5, 6, 0, 0}, // hpDamageWork
#else
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
#endif
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_j_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 5, 5, 100, 0, 20, 10, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 5, 5, 100, 0, 10, 10, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Spring Jump that hits the iron frame hanging over the stage instead of an enemy:
 * no damage, only the weapon's after-effects on the stage (marioAttackEvent_JyabaraJump).
 */
BattleWeapon marioWeapon_JyabaraJumpFailed = {
    "btl_wn_mario_jyabara_jump", // name
    ICON_ITEM_SUPER_BOOTS, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    1, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // attackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 50, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Mario's first strike after hitting an enemy with the Super Hammer spin in the field
 * (_GetFirstAttackWeapon type 5); runs marioAttackEvent_FirstAttackKaitenHammer.
 */
BattleWeapon marioDefaultWeapon_FirstAttackKaitenHammer = {
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
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_KAITEN_HAMMER, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FirstAttackKaitenHammer, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon marioDefaultWeapon_FirstAttackUltraHammer = {
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
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
#if VERSION <= VERSION_DEMO_US
    {1, 2, 2, 4, 4, 8, 0, 0},
#else
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
#endif
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_ULTRA_HAMMER, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FirstAttackKaitenHammer, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Mario's Hammer (0 FP): a hold-and-release swing at the front enemy, damage from his hammer level.
 * Also his first strike after hitting an enemy with the hammer in the field.
 */
BattleWeapon marioDefaultWeapon_Hammer = {
    "btl_wn_mario_normal_hammer", // name
    ICON_ITEM_HAMMER, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalHammer, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 3, 3, 100, 3, 6, 2, 3, 3, 0, 0, 0}, // stageEffectRates
#else
    {0, 3, 3, 100, 3, 3, 2, 3, 3, 0, 0, 0}, // stageEffectRates
#endif
};

/** Super Hammer (2 FP): Mario's spinning swing that knocks the target back into the enemies behind it. */
BattleWeapon marioWeapon_KaitenHammer = {
    "btl_wn_mario_kaiten_hammer", // name
    ICON_ITEM_SUPER_HAMMER, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kaiten_hammer", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_KAITEN_HAMMER, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kaiten_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * The 1-damage hit taken by each enemy a Super Hammer target is knocked into;
 * set by subsetevt_shot_damage in battle_event_subset.c.
 */
BattleWeapon marioWeapon_KaitenHammerInvolved = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
#if VERSION <= VERSION_DEMO_US
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST,
#else
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_RECOIL | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
#endif
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_KAITEN_HAMMER_INVOLVED, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Ultra Hammer (4 FP): the stronger spinning swing with a whirlwind; it knocks the target back
 * and then hits it again with marioWeapon_UltraHammerFinish.
 */
BattleWeapon marioWeapon_UltraHammer = {
    "btl_wn_mario_ultra_hammer", // name
    ICON_ITEM_ULTRA_HAMMER, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_ULTRA_HAMMER, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_ultra_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * The 3-damage hit taken by each enemy an Ultra Hammer target is knocked into;
 * set by subsetevt_shot_damage in battle_event_subset.c.
 */
BattleWeapon marioWeapon_UltraHammerInvolved = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
#if VERSION <= VERSION_DEMO_US
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST,
#else
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_RECOIL | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
#endif
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_ULTRA_HAMMER_INVOLVED, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Ultra Hammer's extra hit on the knocked-back target (marioAttackEvent_KaitenHammer). */
BattleWeapon marioWeapon_UltraHammerFinish = {
    "btl_wn_mario_ultra_hammer", // name
    ICON_ITEM_ULTRA_HAMMER, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_ULTRA_HAMMER_FINISH, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 0, 0}, // stageEffectRates
};

/** Multibounce badge (2 FP): Mario stomps each enemy in turn, one timed press per bounce. */
BattleWeapon badgeWeapon_TugitugiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    241, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 241, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TugiTugiJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 2, 2, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 2, 2, 0, 0, 0}, // stageEffectRates
#endif
};

/** Power Jump badge (2 FP): one stronger stomp on a single enemy, run by marioAttackEvent_GatsunJump. */
BattleWeapon badgeWeapon_GatsunJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    240, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 240, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunJump, // attackEvtScript
    {0, 25, 25, 100, 25, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};

/** Mega Jump badge (6 FP): Power Jump's stomp with still more damage, run by marioAttackEvent_DokanJump. */
BattleWeapon badgeWeapon_DokanJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    332, // itemId
    NULL, // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 332, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_DokanJump, // attackEvtScript
    {0, 25, 25, 100, 25, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

/** Power Bounce badge (3 FP): Mario keeps bouncing on one enemy for as long as his timed presses succeed. */
BattleWeapon badgeWeapon_RenzokuJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    242, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 242, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_RenzokuJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 2, 2, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 2, 2, 0, 0, 0}, // stageEffectRates
#endif
};

/** Tornado Jump badge (3 FP): a stomp, then on a second command a tornado that hits the airborne enemies. */
BattleWeapon badgeWeapon_TatsumakiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    243, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    3, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 1, 1, 3, 2, 5, 243, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_tatsumaki_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TatsumakiJump, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * The tornado of Tornado Jump: the 2-damage hit on each airborne enemy after a successful second command.
 * Has no attack script of its own; marioAttackEvent_TatsumakiJump uses it for those targets.
 */
BattleWeapon badgeWeapon_TatsumakiJumpInvolved = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerTatsumaki, // hpDamageFn
    {0, 2, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_GROUNDED | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_MiniminiFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    244, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0,
95,
3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_MiniminiFumi,
{0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0},
};
#else
BattleWeapon badgeWeapon_MiniminiFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    244, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_MiniminiFumi, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#endif
};
#endif

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_NemuraseFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    245, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM,
95,
5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NemuraseFumi,
{0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0},
};
#else
BattleWeapon badgeWeapon_NemuraseFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    245, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NemuraseFumi, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#endif
};
#endif

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_FunyafunyaJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    246, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0,
95,
3, // defChangeTime
    -3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FunyafunyaJump,
{0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0},
};
#else
BattleWeapon badgeWeapon_FunyafunyaJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    246, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    -3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FunyafunyaJump, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
#endif
};
#endif

/** Power Smash badge (2 FP): one stronger hammer blow, run by marioAttackEvent_GatsunHammer. */
BattleWeapon badgeWeapon_GatsunHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    247, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 247, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunHammer, // attackEvtScript
    {0, 25, 25, 100, 25, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};

/** Mega Smash badge (6 FP): Power Smash with still more damage, sharing marioAttackEvent_GatsunHammer. */
BattleWeapon badgeWeapon_DokanHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    333, // itemId
    NULL, // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 333, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunHammer, // attackEvtScript
    {0, 25, 25, 100, 25, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

/**
 * Quake Hammer badge (3 FP): Mario slams the ground and the quake hits every enemy that is not floating,
 * ignoring Defense and flipping shelled enemies.
 */
BattleWeapon badgeWeapon_JishinHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    248, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 248, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_FLOATING | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_FLIP_BOMB_FLIPPABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JishinHammer, // attackEvtScript
    {0, 50, 50, 100, 50, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};

/** Mega Quake badge (7 FP): the stronger Quake Hammer slam, with a shorter command window. */
BattleWeapon badgeWeapon_UltraJishinHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    334, // itemId
    "msg_ultra_attack", // description
    100, // baseAccuracy
    7, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {3, 6, 3, 6, 3, 6, 334, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_FLOATING | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_ultra_jishin_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_FLIP_BOMB_FLIPPABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_UltraJishinHammer, // attackEvtScript
    {0, 50, 50, 100, 50, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

/** Hammer Throw badge (2 FP): Mario throws his hammer at the chosen enemy. */
BattleWeapon badgeWeapon_HammerNageru = {
    NULL, 0, // name..icon
    {0}, // pad_06
    249, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0, 1, 1, 3, 2, 5, 249, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer_nageru", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_HammerNageru, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon badgeWeapon_TsuranukiNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    250, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TsuranukiHammer, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 25, 25, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 25, 25, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_ConfuseHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    251, // itemId
    NULL, // description
    100,
3,
0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_PAYBACK | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0,
90,
3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalHammer,
{0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0},
};
#else
BattleWeapon badgeWeapon_ConfuseHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    251, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, // poisonStrength
    100, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalHammer, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 5, 5, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};
#endif

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_FireNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100,
6,
0, 0, // baseSpCost..superguardsAllowed
    1.0f,
2,
5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1,
{0xFFFFFFFF, 0, 0, 2, 1, 4, 252, 2},
NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_fire_naguri", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_TARGET | SPECIAL_PIERCE_DEFENSE | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0,
95,
3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri,
{0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0},
};
#else
BattleWeapon badgeWeapon_FireNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100, // baseAccuracy
    5, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 3, 0, 3, 1, 3, 252, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_fire_naguri", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_TARGET | SPECIAL_PIERCE_DEFENSE | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};
#endif

/**
 * Fire Drive when its Action Command fails: aimed at Mario's own side,
 * so marioAttackEvent_FireNaguri checks it only against Mario himself instead of the enemies.
 */
#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_FireNaguriFailed = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100,
6,
0, 0, // baseSpCost..superguardsAllowed
    1.0f,
2,
5, // reserved_19
    1, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv,
{1, 2, 2, 4, 3, 6, 0, 0},
NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0,
95,
3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri,
{0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0},
};
#else
BattleWeapon badgeWeapon_FireNaguriFailed = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100, // baseAccuracy
    5, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    1, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 5, 2, 5, 3, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};
#endif

#if VERSION <= VERSION_DEMO_US
BattleWeapon badgeWeapon_IceNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    253, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_PAYBACK | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0,
90,
2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_IceNaguri,
{0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0},
};
#else
BattleWeapon badgeWeapon_IceNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    253, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    100, // freezeChance
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_IceNaguri, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 5, 5, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};
#endif

BattleWeapon badgeWeapon_Charge = {
    "btl_wn_mario_normal_charge", // name
    ICON_ITEM_BOOTS, // icon
    {0}, // pad_06
    256, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_US
    0,
#else
    1, // baseFpCost
#endif
    0, 0, // baseSpCost..superguardsAllowed
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
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalCharge, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** The partner's Charge command from the Charge P badge; otherwise the same as badgeWeapon_Charge. */
BattleWeapon badgeWeapon_ChargeP = {
    "btl_wn_mario_normal_charge", // name
    ICON_ITEM_BOOTS, // icon
    {0}, // pad_06
    257, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_US
    0,
#else
    1, // baseFpCost
#endif
    0, 0, // baseSpCost..superguardsAllowed
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
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalChargeP, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Mario's Super Charge command from the Super Charge badge (2 FP), run by _SuperCharge_core. */
BattleWeapon badgeWeapon_SuperCharge = {
    "btl_wn_mario_super_charge", // name
    ICON_ITEM_BOOTS, // icon
    {0}, // pad_06
    337, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperCharge, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** The partner's Super Charge command from the Super Charge P badge (2 FP). */
BattleWeapon badgeWeapon_SuperChargeP = {
    "btl_wn_mario_super_charge", // name
    ICON_ITEM_BOOTS, // icon
    {0}, // pad_06
    338, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperChargeP, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Double Dip (4 FP): lets Mario or a partner use two items in one turn.
 * Has no attack script; battle_seq_command.c only uses it to check and pay the FP cost.
 */
BattleWeapon badgeWeapon_2KaiItem = {
    "btl_wn_2kai_item", // name
    0, // icon
    {0}, // pad_06
    254, // itemId
    NULL, // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, 0.0f, 0, 0, 0, 0, NULL, // baseSpCost..hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

BattleWeapon badgeWeapon_3KaiItem = {
    "btl_wn_3kai_item", // name
    0, // icon
    {0}, // pad_06
    329, // itemId
    NULL, // description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_US
    10,
#else
    8, // baseFpCost
#endif
    0, 0, 0.0f, 0, 0, 0, 0, NULL, // baseSpCost..hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/**
 * Mario's Defend command: raises his Defense by 1 for the turn
 * (btldefaultevt_Defend in battle_event_default.c). Its item is the unused Defend badge.
 */
BattleWeapon badgeWeapon_DefenceCommand = {
    NULL, 0, // name..icon
    {0}, // pad_06
    335, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
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
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    1, // defChangeTime
    1, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * The partner's Defend command: raises the partner's Defense by 1 for the turn (btldefaultevt_DefendParty).
 * Its item is the unused Defend badge P.
 */
BattleWeapon badgeWeapon_DefenceCommandP = {
    NULL, 0, // name..icon
    {0}, // pad_06
    336, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
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
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    1, // defChangeTime
    1, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Clock Out (2 Star Power, the Emerald Star's move): stops the enemies in time; run by sac_bakugame.c. */
BattleWeapon marioWeapon_BakuGame = {
    "btl_wn_sac_bakugame", // name
    ICON_SAC_BAKUGAME, // icon
    {0}, // pad_06
    0, // itemId
    "msg_bakugame", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_3", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
#if VERSION <= VERSION_DEMO_US
    0, // stopChance
    0,
#else
    100, // stopChance
    1, // stopTime
#endif
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_BakuGame, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Art Attack (4 Star Power, the Ruby Star's move): Mario draws circles around the enemies; sac_scissor.c. */
BattleWeapon marioWeapon_Scissor = {
    "btl_wn_sac_scissor", // name
    ICON_SAC_SCISSOR, // icon
    {0}, // pad_06
    0, // itemId
    "msg_scissor", // description
    100, // baseAccuracy
    0, // baseFpCost
    4, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPower_Scissor, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_5", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Scissor, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Supernova (6 Star Power, the Crystal Star's move): a blast that damages every enemy; sac_zubastar.c. */
BattleWeapon marioWeapon_ZubaStar = {
    "btl_wn_sac_zubastar", // name
    ICON_SAC_ZUBASTAR, // icon
    {0}, // pad_06
    0, // itemId
    "msg_zubastar", // description
    100, // baseAccuracy
    0, // baseFpCost
    6, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPower_ZubaStar, // hpDamageFn
    {0, 3, 6, 9, 12, 15, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_8", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_ZubaStar, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Sweet Treat (1 Star Power, from the Magical Map): restores Mario's HP and the partner's HP and FP
 * by how many targets the player hits; sac_genki.c.
 */
BattleWeapon marioWeapon_Genki0 = {
    "btl_wn_sac_genki0", // name
    ICON_SAC_GENKI0, // icon
    {0}, // pad_06
    0, // itemId
    "msg_genkigenki", // description
    100, // baseAccuracy
    0, // baseFpCost
    1, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_1", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki0, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Sweet Feast (5 Star Power, the Sapphire Star's move): the bigger version of Sweet Treat; sac_genki.c. */
BattleWeapon marioWeapon_Genki1 = {
    "btl_wn_sac_genki1", // name
    ICON_SAC_GENKI1, // icon
    {0}, // pad_06
    0, // itemId
    "msg_super_genki", // description
    100, // baseAccuracy
    0, // baseFpCost
    5, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_6", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki1, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Earth Tremor (2 Star Power, the Diamond Star's move): a quake that damages every enemy; sac_deka.c. */
BattleWeapon marioWeapon_Deka = {
    "btl_wn_sac_dekkaku_dokkan", // name
    ICON_SAC_DEKA, // icon
    {0}, // pad_06
    0, // itemId
    "msg_dekkaku_dokkan", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPower_Deka, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_2", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Deka, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Power Lift (3 Star Power, the Gold Star's move): raises Mario's and the partner's Attack and Defense
 * for 3 turns by the levels earned in the grid game; sac_muki.c.
 */
BattleWeapon marioWeapon_Muki = {
    "btl_wn_sac_mukimuki_body", // name
    ICON_SAC_MUKI, // icon
    {0}, // pad_06
    0, // itemId
    "msg_mukimuki_body", // description
    100, // baseAccuracy
    0, // baseFpCost
    3, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_4", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Muki, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Showstopper (2 Star Power, the Garnet Star's move): may knock out every enemy at once; sac_suki.c. */
BattleWeapon marioWeapon_Suki = {
    "btl_wn_sac_sukkari_sukkiri", // name
    ICON_SAC_SUKI, // icon
    {0}, // pad_06
    0, // itemId
    "msg_sukkari_sukkiri", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
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
    "msg_sac_7", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, // atkChangeTime..allergicTime
#if VERSION <= VERSION_DEMO_US
    0,
#else
    100, // ohkoChance
#endif
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Suki, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Mario's Special moves in Star Power bit order (the order the Crystal Stars are collected);
 * the NULL slots are unused.
 * Read by the battle Special menu (battle_seq_command.c) and the pause menu's Mario page (win_mario.c).
 */
BattleWeapon* superActionTable[] = {
    &marioWeapon_Genki0, &marioWeapon_Deka, &marioWeapon_BakuGame, &marioWeapon_Muki, &marioWeapon_Scissor, &marioWeapon_Genki1,
    &marioWeapon_Suki, &marioWeapon_ZubaStar, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
};

/**
 * Mario's plain Jump (marioDefaultWeapon_Jump),
 * also used for his first strike (battle flag 2)
 * and the stomp badges (Shrink, Sleepy and Soft Stomp add a star sparkle).
 * He runs to the takeoff spot and jumps on the target;
 * a timed press on landing gives a second stomp, a miss or dodge makes him tumble off,
 * and a Stylish press is possible.
 */
static EVT_BEGIN(marioAttackEvent_NormalJump)
    // Pick the target: on a first strike the enemy chosen for it; otherwise the enemy picked in the menu,
    // with the attack declared, its FP cost paid and Merlee's attack charm used if it is due.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Jump))
        CALL(btlevtcmd_ACHelpSet, PTR(&marioDefaultWeapon_Jump))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, LW(3), LW(4), UNIT_ID_SELF, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    ELSE
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_IF
    // On a normal turn the Action Command is set up now; on a first strike it is set up while Mario runs in.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    END_IF
    // First strike: Mario starts at his place, waits a second and runs to the takeoff spot. Otherwise he runs
    // there at once, with a star sparkle for Shrink, Sleepy and Soft Stomp.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(5))
            CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(5))
            CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    ELSE
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
        BEGIN_SIBLING_EVENT
            SWITCH(LW(12))
                CASE_EQUAL(PTR(&badgeWeapon_MiniminiFumi))
                SET_VALUE(LW(13), 4)
                CASE_EQUAL(PTR(&badgeWeapon_NemuraseFumi))
                SET_VALUE(LW(13), 3)
                CASE_EQUAL(PTR(&badgeWeapon_FunyafunyaJump))
                SET_VALUE(LW(13), 1)
            END_SWITCH
            SWITCH(LW(12))
                CASE_OR_EQUAL(PTR(&badgeWeapon_MiniminiFumi))
                CASE_OR_EQUAL(PTR(&badgeWeapon_NemuraseFumi))
                CASE_OR_EQUAL(PTR(&badgeWeapon_FunyafunyaJump))
                CALL(_jump_star_effect, UNIT_ID_SELF, LW(13))
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CASE_END
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    // Mario faces the target and the Action Command starts; each stomp starts at label 10, where the game
    // first checks whether it will land.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(4)
        CASE_OR_EQUAL(6)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    // Mario jumps onto the target; on the second stomp a Stylish press partway up is possible.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            SET_VALUE(LW(0), LW(6))
            DIVIDE(LW(0), 2)
            SUBTRACT(LW(0), 3)
            SET_VALUE(LW(1), LW(0))
            SET_VALUE(LW(2), LW(0))
#if VERSION >= VERSION_US
            SUBTRACT(LW(1), 7)
            ADD(LW(2), 5)
#else
            SUBTRACT(LW(1), 3)
            ADD(LW(2), 3)
#endif
            SET_VALUE(LW(0), LW(6))
            SUBTRACT(LW(0), 4)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(1), LW(2), LW(0), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
#if VERSION >= VERSION_US
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
#else
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_4"))
#endif
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
                CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 3, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    END_IF
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press deals the hit and Mario hops back off the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    END_IF
    // Pressed in time: the first stomp hits with the success effect and Mario bounces for a second stomp
    // (back to label 10); the second stomp deals the final hit.
    CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS_OR_EQUAL(LW(6), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        WAIT_FRAMES(2)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        WAIT_FRAMES(2)
    END_IF
    // After the final hit Mario backflips off the target towards his place.
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StopAC)
#if VERSION < VERSION_US
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
#endif
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
#if VERSION >= VERSION_US
    // Outside JP a Stylish press is possible on the backflip.
    SET_VALUE(LW(6), 32)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(3)
        SET_VALUE(LW(0), LW(6))
        DIVIDE(LW(0), 2)
        SUBTRACT(LW(0), 3)
        SET_VALUE(LW(1), LW(0))
        SET_VALUE(LW(2), LW(0))
        SUBTRACT(LW(1), 7)
        ADD(LW(2), 5)
        SET_VALUE(LW(0), LW(6))
        SUBTRACT(LW(0), 4)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(1), LW(2), LW(0), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
#else
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
#endif
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Spin Jump (Super Boots, marioWeapon_KururinJump):
 * a first stomp like the plain Jump, then on a successful press Mario spins in the air above the target
 * and dives onto it for a second, stronger hit (marioWeapon_KururinJump2).
 * Also his first strike with the Super Boots.
 */
static EVT_BEGIN(marioAttackEvent_KururinJump)
    // First strike: Mario starts at his place, waits half a second and runs to the takeoff spot, with the
    // Action Command set up on the way.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioWeapon_KururinJump))
        CALL(btlevtcmd_ACHelpSet, PTR(&marioWeapon_KururinJump))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, LW(3), LW(4), UNIT_ID_SELF, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(5))
            CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(5))
            CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    ELSE
        // A normal turn: the selected enemy is attacked, the FP cost paid, Merlee's attack charm used if due,
        // and Mario runs to the takeoff spot.
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    // Mario faces the target and the Action Command starts; the game first checks whether the stomp will
    // land.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(4)
        CASE_OR_EQUAL(6)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    // First stomp: Mario jumps onto the target, and a Stylish press partway up makes him flip.
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(0), LW(6))
            SET_VALUE(LW(1), LW(6))
            DIVIDE(LW(0), 2)
            SUBTRACT(LW(1), 1)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 5, LW(0), LW(1), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
                LOOP(15)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -24)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
        // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
            WAIT_FRAMES(40)
            GOTO(95)
        END_IF
        // The timed press on landing. A missed press deals only the first hit and Mario hops back off the
        // target.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
        CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
        CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
        DIVIDE(LW(0), 2)
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
        CALL(btlevtcmd_ResultAC)
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StopAC)
            CALL(evt_audience_ap_recovery)
            CALL(btlevtcmd_InviteApInfoReport)
            CALL(btlevtcmd_WeaponAftereffect, LW(12))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
            CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
            GOTO(95)
        END_IF
        // Pressed in time: the first stomp hits with the success effect.
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_AC_SUCCESS, LW(5))
        ADD(LW(10), 1)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    // Mario bounces up off the target.
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    GOTO(55)
    // Never reached (nothing jumps to label 52): Mario would hang in the air, drop to the floor, land hard
    // and get up.
    LABEL(52)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_N_7"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 14)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
    WAIT_FRAMES(28)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_U_3"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    // The spin: Mario somersaults in the air above the target, then dives straight down onto it.
    LABEL(55)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_1A"))
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_KURURIN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_1B"))
    WAIT_FRAMES(2)
    // The timed press as the dive lands: a good press deals the stronger second hit
    // (marioWeapon_KururinJump2) with the success effect; a missed one still deals it without the effect.
    // Either way Mario hops back off.
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 1)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&marioWeapon_KururinJump2), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, PTR(&marioWeapon_KururinJump2), LW(6))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&marioWeapon_KururinJump2), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_AudienceDeclareACResult, PTR(&marioWeapon_KururinJump2), LW(6))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    END_IF
    GOTO(95)
    // Never reached (nothing jumps to label 90): a backflip off the target.
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Spring Jump (Ultra Boots): Mario springs high into the air and drops on the chosen enemy.
 * On stages with an iron frame overhead that is still up,
 * he hits the frame instead and falls back with marioWeapon_JyabaraJumpFailed.
 */
EVT_BEGIN(marioAttackEvent_JyabaraJump)
    // Target the selected enemy: the attack is declared, the FP cost paid, Merlee's attack charm used if due,
    // and Mario turns to face it.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    // The Action Command starts, then Mario hops, his boots wind up into a spring and he springs high into
    // the air.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    WAIT_FRAMES(21)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, STRING("p_jyabara"), 1)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1E"))
    WAIT_FRAMES(16)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_JABARA1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, STRING("p_jyabara"), 0)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1B"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_JABARA2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_2A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_2B"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_JABARA3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_3A"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_3B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_JABARA4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_AC3_5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // A stage with an iron frame overhead that is still up: Mario spins up into the frame and is knocked flat
    // against it.
    CALL(_bgset_iron_frame_check, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1C"))
        BEGIN_INLINE_EVENT_GET_ID(LW(14))
            LOOP(50)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 48, 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        BEGIN_INLINE_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 90, MOVE_SOUND_KEEP)
        END_INLINE_EVENT
        CALL(_wait_jyabara_hit_iron_frame, UNIT_ID_SELF)
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_WeaponAftereffect, PTR(&marioWeapon_JyabaraJumpFailed))
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        BEGIN_SIBLING_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
            LOOP(5)
                WAIT_FRAMES(1)
                SUBTRACT_FLOAT(LW(0), FLOAT(0.19921875f))
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), LW(0), FLOAT(1.0f))
            END_LOOP
        END_SIBLING_EVENT
        // Mario drops back down on his spring, bobbing up and down while he sways back and forth and returns
        // to his place.
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), 130, LW(2), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(16)
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1H"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(5), LW(6))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(9), EVT_NULLPTR)
            SUBTRACT(LW(9), LW(5))
            DIVIDE(LW(9), 4)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(6), EVT_NULLPTR)
            SUBTRACT(LW(6), LW(9))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(7), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(7), LW(1))
            ADD(LW(7), LW(9))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, EASING_LINEAR, LW(0), LW(6), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, EASING_LINEAR, LW(0), LW(7), 64)
            LOOP(64)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, EASING_LINEAR, LW(0), LW(8), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(0), LW(1), 26)
            LOOP(26)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(7))
            MULTIPLY(LW(7), 60)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(0), LW(1), 58)
            LOOP(58)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(1), EVT_NULLPTR, EVT_NULLPTR)
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(0), LW(1), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        // He stretches back to full height and the attack ends without hitting anyone.
        WAIT_FRAMES(130)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_S"))
        CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1E"))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(evt_sub_intpl_init_float, EASING_CUBIC_OUT, FLOAT(0.0f), FLOAT(1.1992188f), 10)
            LOOP(10)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init_float, EASING_WAVE_SHRINKING, FLOAT(1.1992188f), FLOAT(1.0f), 30)
            LOOP(30)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        WAIT_FRAMES(20)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    // The real jump: Mario spins at the top, then drops onto the target with a random hang time.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("PM_J_1C"))
    BEGIN_SIBLING_EVENT
        LOOP(50)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 48, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_1A"))
        CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 80, 255, 255, 255, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
    CALL(evt_sub_random, 20, LW(5))
    SUBTRACT(LW(5), 10)
    MULTIPLY(LW(5), 6)
    ADD(LW(5), 180)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(5), MOVE_SOUND_KEEP)
    // A dodge or miss: the enemy plays its dodge, Mario tumbles off, falls flat and gets up.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_B_2"))
        CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
        CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(10)
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 35)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            MULTIPLY(LW(0), -24)
            LOOP(20)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            MULTIPLY(LW(0), -20)
            LOOP(12)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_11"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        GOTO(95)
    END_IF
    // The timed press on landing (a little later outside JP). A missed press deals one hit, the screen shakes
    // by the damage dealt (at most 9 outside JP) and Mario hops back off.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    END_INLINE_EVENT
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
#if VERSION >= VERSION_US
    ADD(LW(0), 4)
#else
    ADD(LW(0), 2)
#endif
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
#endif
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_AC_SUCCESS, LW(5))
    END_IF
    // Pressed in time: the first hit lands with the success effect and Mario bounces up off the target.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    // Mario somersaults above the target and dives back down onto it.
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_1A"))
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_JABARA5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_1B"))
    WAIT_FRAMES(2)
    // The second timed press: a good press deals the second hit with the success effect; a missed one still
    // deals it, the screen shakes (capped at 9 only in the US version) and Mario hops back off.
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 1)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION == VERSION_US
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
#endif
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    END_IF
    // After the second hit the screen shakes by the damage dealt and Mario backflips home; a Stylish press is
    // possible on the way.
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
#endif
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
#if VERSION >= VERSION_US
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 8, 17, 29, 0)
#else
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 11, 15, 29, 0)
#endif
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
#if VERSION >= VERSION_US
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
#else
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_4"))
#endif
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Shared jump attack for Power Jump and Mega Jump:
 * the caller puts the badge's weapon in LW(12) and this runs the approach, the single timed stomp,
 * the damage and the return to Mario's home position.
 */
EVT_BEGIN(marioAttackEvent_GatsuDokaJump)
    // Target the selected enemy: the attack is declared, the badge's FP cost paid, Merlee's attack charm used
    // if due, and the Action Command set up.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    // Mario runs to the takeoff spot, faces the target and the Action Command starts; the game checks whether
    // the stomp will land.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // Mario jumps onto the target, and a Stylish press partway up makes him flip.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), LW(6))
        SET_VALUE(LW(1), LW(6))
        DIVIDE(LW(0), 2)
        SUBTRACT(LW(1), 1)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 5, LW(0), LW(1), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
            LOOP(15)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -24)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press still deals the hit, the screen shakes by the damage dealt
    // (at most 9 outside JP) and Mario hops back off.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
#endif
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    END_IF
    // Pressed in time: the stomp hits with the success effect.
    CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    WAIT_FRAMES(2)
    // The screen shakes by the damage dealt and Mario backflips off the target.
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
#endif
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Tornado Jump badge attack: a stomp on the chosen enemy,
 * then on a successful second command Mario spins into a tornado (_tatsumaki_effect) that hits every airborne
 * enemy with badgeWeapon_TatsumakiJumpInvolved.
 */
#if VERSION <= VERSION_DEMO_US
#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_GatsunJump)
    SET_VALUE(LW(12), PTR(&badgeWeapon_GatsunJump))
    CALL_CHILD(PTR(marioAttackEvent_GatsuDokaJump))
    RETURN
EVT_END()
EVT_BEGIN(marioAttackEvent_DokanJump)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DokanJump))
    CALL_CHILD(PTR(marioAttackEvent_GatsuDokaJump))
    RETURN
EVT_END()
#endif
/**
 * Tornado Jump badge attack: a stomp on the chosen enemy,
 * then on a successful second command Mario spins into a tornado (_tatsumaki_effect) that hits every airborne
 * enemy with badgeWeapon_TatsumakiJumpInvolved.
 */
EVT_BEGIN(marioAttackEvent_TatsumakiJump)
    // Target the selected enemy: the attack is declared, the badge's FP cost paid, Merlee's attack charm used
    // if due, and the stomp's Action Command set up.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    // Mario runs to the takeoff spot (pausing first if it is close), faces the target and the Action Command
    // starts; the game checks whether the stomp will land.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // Mario jumps onto the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, -1)
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press deals only the stomp and Mario hops back off (label 90).
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        GOTO(90)
    END_IF
    // Pressed in time: the stomp hits with the success effect and Mario bounces high into the air, turning.
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(btlevtcmd_GetFriendBelong, LW(3), LW(15))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    WAIT_FRAMES(2)
    CALL(btlevtcmd_StopAC)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, 3, -1)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(24)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(27)
    DELETE_EVENT(LW(13))
    // The second Action Command, the tornado's button presses, is set up with a setting that depends on the
    // difficulty.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        CALL(btlevtcmd_ftof, 600, LW(0))
        CASE_EQUAL(-2)
        CALL(btlevtcmd_ftof, 480, LW(0))
        CASE_EQUAL(-1)
        CALL(btlevtcmd_ftof, 420, LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_ftof, 360, LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_ftof, 300, LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ftof, 200, LW(0))
        CASE_DEFAULT
        CALL(btlevtcmd_ftof, 100, LW(0))
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 3, -4, 3, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 10, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_StartAC, 1)
    // Mario spins in the air, faster with each correct press, and the rising whir follows his progress.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 30)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, 0)
        SET_VALUE(LW(0), 15)
        LABEL(60)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(61)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(10))
        ADD(LW(0), 1)
        IF_INT_GREATER(LW(0), LW(10))
            SET_VALUE(LW(0), LW(10))
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, LW(0))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(60)
        LABEL(61)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(0), 0)
        LOOP(1000)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(1))
            IF_INT_NOT_EQUAL(LW(1), LW(0))
                SET_VALUE(LW(0), LW(1))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 1023)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 1023)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_EQUAL(2)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 45)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 2046)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 2046)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_EQUAL(3)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 3069)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 3069)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_DEFAULT
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 60)
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    // The tornado whir plays, sliding towards the pitch set by the presses.
    CALL(evt_snd_sfxon, STRING("SFX_BTL_JUMP_TATUMAKI1"), LW(11))
    BEGIN_SIBLING_EVENT_GET_ID(LW(12))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(11), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, LW(1))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(0))
            IF_INT_NOT_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(2))
                ADD(LW(0), LW(2))
                IF_INT_GREATER_OR_EQUAL(LW(2), 0)
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(1))
                        SET_VALUE(LW(0), LW(1))
                    END_IF
                ELSE
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(1))
                        SET_VALUE(LW(0), LW(1))
                    END_IF
                END_IF
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(0))
                CALL(evt_snd_sfx_pit, LW(11), LW(0))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    // The result of the button presses.
    CALL(btlevtcmd_ResultAC)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        // Failed: the spin and the whir wind down until Mario tips over.
        DELETE_EVENT(LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, -2728)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
            SUBTRACT(LW(10), -2728)
            MULTIPLY(LW(10), -1)
            DIVIDE(LW(10), 40)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            LOOP(40)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                IF_INT_GREATER(LW(0), 10)
                    SUBTRACT(LW(0), 1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                END_IF
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                IF_INT_LESS(LW(1), 90)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 90)
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_N_7"))
                        END_IF
                END_IF
                SET_VALUE(LW(1), LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_FRAMES(40)
        DELETE_EVENT(LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        // Mario stops spinning, falls to the floor, lands hard and gets up; the attack ends.
        BEGIN_SIBLING_EVENT_GET_ID(LW(13))
            LOOP(60)
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(1))
                ADD(LW(0), LW(1))
                IF_INT_GREATER_OR_EQUAL(LW(0), 360)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 40, FLOAT(0.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(12)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        WAIT_FRAMES(32)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    ELSE
        // Succeeded: Mario spins up to full speed.
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 45)
        LABEL(62)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, LW(0))
        IF_INT_LESS(LW(0), 44)
            WAIT_FRAMES(1)
            GOTO(62)
        END_IF
        // The tornado strikes every other enemy the tornado weapon (badgeWeapon_TatsumakiJumpInvolved) can
        // reach, skipping the stomped enemy (in the demo, reaching it ends the round early); a dodge plays
        // the enemy's dodge.
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(6), LW(3))
            CALL(_tatsumaki_effect, UNIT_ID_SELF)
            SET_VALUE(LW(0), LW(15))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), PTR(&badgeWeapon_TatsumakiJumpInvolved))
            CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
            LABEL(50)
            IF_INT_EQUAL(LW(3), -1)
                GOTO(59)
            END_IF
            IF_INT_EQUAL(LW(3), LW(6))
                GOTO(59)
                END_IF
            CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT, LW(5))
            SWITCH(LW(5))
                CASE_EQUAL(2)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
                CASE_EQUAL(3)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
                CASE_EQUAL(6)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
                CASE_EQUAL(4)
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetResultAC, LW(0))
                IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
                    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
                    ELSE
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT, LW(5))
                END_IF
                CASE_DEFAULT
            END_SWITCH
            LABEL(58)
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(50)
            LABEL(59)
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_SIBLING_EVENT
        // The spin slows and stops.
        WAIT_FRAMES(60)
        BEGIN_INLINE_EVENT_GET_ID(LW(13))
            LOOP(30)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                SUBTRACT(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                WAIT_FRAMES(1)
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, 0)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
            END_LOOP
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        // Mario drops back to the floor; a Stylish press on the way down starts a spinning roll home.
        WAIT_FRAMES(30)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 7, 21, 21, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_B_1"))
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
        CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
        WAIT_FRAMES(6)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_GREATER_OR_EQUAL(LW(6), 2)
            GOTO(80)
        END_IF
    END_IF
    // No Stylish: the attack ends.
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    // Stylish: Mario rolls back to his place spinning (from the US version on the first Stylish is also
    // counted). A second press there ends in a confetti pose; a mistimed one plays the miss sound.
    LABEL(80)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
    SET_VALUE(LW(8), LW(7))
    ADD(LW(8), 12)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(7), LW(8), LW(8), 0)
    END_SIBLING_EVENT
    ADD(LW(8), 1)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), 30)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(10), 0)
        LOOP(LW(8))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
            IF_INT_EQUAL(LW(10), 0)
                CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            END_IF
            ADD(LW(10), 1)
            IF_INT_GREATER_OR_EQUAL(LW(10), 12)
                SET_VALUE(LW(10), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), -1, 0)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), 36)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        MODULO(LW(2), 360)
        DIVIDE(LW(2), 10)
        ADD(LW(2), LW(5))
        LOOP(10)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
        CASE_LESS_OR_EQUAL(-1)
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_MISS1"), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_S"))
        WAIT_MS(1500)
        CASE_DEFAULT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    GOTO(99)
    // A missed first press: the stomp's aftereffects and Mario backflips off the target.
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#else
/**
 * Tornado Jump badge attack: a stomp on the chosen enemy,
 * then on a successful second command Mario spins into a tornado (_tatsumaki_effect) that hits every airborne
 * enemy with badgeWeapon_TatsumakiJumpInvolved.
 */
EVT_BEGIN(marioAttackEvent_TatsumakiJump)
    // Target the selected enemy: the attack is declared, the badge's FP cost paid, Merlee's attack charm used
    // if due, and the stomp's Action Command set up.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    // Mario runs to the takeoff spot (pausing first if it is close), faces the target and the Action Command
    // starts; the game checks whether the stomp will land.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // Mario jumps onto the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 0, MOVE_SOUND_FROM_TABLE)
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press deals only the stomp and Mario hops back off (label 90).
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        GOTO(90)
    END_IF
    // Pressed in time: the stomp hits with the success effect and Mario bounces high into the air, turning.
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(btlevtcmd_GetFriendBelong, LW(3), LW(15))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    WAIT_FRAMES(2)
    CALL(btlevtcmd_StopAC)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, 3, MOVE_SOUND_FROM_TABLE)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(24)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(27)
    DELETE_EVENT(LW(13))
    // The second Action Command, the tornado's button presses, is set up with a setting that depends on the
    // difficulty.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        CALL(btlevtcmd_ftof, 600, LW(0))
        CASE_EQUAL(-2)
        CALL(btlevtcmd_ftof, 480, LW(0))
        CASE_EQUAL(-1)
        CALL(btlevtcmd_ftof, 420, LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_ftof, 360, LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_ftof, 300, LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ftof, 200, LW(0))
        CASE_DEFAULT
        CALL(btlevtcmd_ftof, 100, LW(0))
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 3, -4, 3, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 10, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_StartAC, 1)
    // Mario spins in the air, faster with each correct press, and the rising whir follows his progress.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 30)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, 0)
        SET_VALUE(LW(0), 15)
        LABEL(60)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(61)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(10))
        ADD(LW(0), 1)
        IF_INT_GREATER(LW(0), LW(10))
            SET_VALUE(LW(0), LW(10))
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, LW(0))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(60)
        LABEL(61)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(0), 0)
        LOOP(1000)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(1))
            IF_INT_NOT_EQUAL(LW(1), LW(0))
                SET_VALUE(LW(0), LW(1))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 1023)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 1023)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_EQUAL(2)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 45)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 2046)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 2046)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_EQUAL(3)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, 3069)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
                    SUBTRACT(LW(10), 3069)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
                    CASE_DEFAULT
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 60)
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    // The tornado whir plays, sliding towards the pitch set by the presses.
    CALL(evt_snd_sfxon, STRING("SFX_BTL_JUMP_TATUMAKI1"), LW(11))
    BEGIN_SIBLING_EVENT_GET_ID(LW(12))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(11), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, LW(1))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(0))
            IF_INT_NOT_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(2))
                ADD(LW(0), LW(2))
                IF_INT_GREATER_OR_EQUAL(LW(2), 0)
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(1))
                        SET_VALUE(LW(0), LW(1))
                    END_IF
                ELSE
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(1))
                        SET_VALUE(LW(0), LW(1))
                    END_IF
                END_IF
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(0))
                CALL(evt_snd_sfx_pit, LW(11), LW(0))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    // The result of the button presses.
    CALL(btlevtcmd_ResultAC)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        // Failed: the spin and the whir wind down until Mario tips over.
        DELETE_EVENT(LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_TARGET, -2728)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH, LW(10))
            SUBTRACT(LW(10), -2728)
            MULTIPLY(LW(10), -1)
            DIVIDE(LW(10), 40)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_MARIO, MARIO_WORK_SPIN_PITCH_STEP, LW(10))
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            LOOP(40)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                IF_INT_GREATER(LW(0), 10)
                    SUBTRACT(LW(0), 1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                END_IF
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                IF_INT_LESS(LW(1), 90)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 90)
                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_N_7"))
                        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_FALL_CRUSH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    END_IF
                END_IF
                SET_VALUE(LW(1), LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_FRAMES(40)
        DELETE_EVENT(LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        // Mario stops spinning, falls to the floor, lands hard and gets up; the attack ends.
        BEGIN_SIBLING_EVENT_GET_ID(LW(13))
            LOOP(60)
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(1))
                ADD(LW(0), LW(1))
                IF_INT_GREATER_OR_EQUAL(LW(0), 360)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 40, FLOAT(0.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(12)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        WAIT_FRAMES(32)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    ELSE
        // Succeeded: Mario spins up to full speed.
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, 45)
        LABEL(62)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED, LW(0))
        IF_INT_LESS(LW(0), 44)
            WAIT_FRAMES(1)
            GOTO(62)
        END_IF
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(6), LW(3))
            CALL(_tatsumaki_effect, UNIT_ID_SELF)
            SET_VALUE(LW(0), LW(15))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), PTR(&badgeWeapon_TatsumakiJumpInvolved))
            CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
            LABEL(50)
            IF_INT_EQUAL(LW(3), -1)
                GOTO(59)
            END_IF
            IF_INT_EQUAL(LW(3), LW(6))
                GOTO(58)
            END_IF
            CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT, LW(5))
            SWITCH(LW(5))
                CASE_EQUAL(2)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
                CASE_EQUAL(3)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
                CASE_EQUAL(6)
                CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
                CASE_EQUAL(4)
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetResultAC, LW(0))
                IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(7))
                    CALL(btlevtcmd_ACSuccessEffect, LW(7), LW(0), LW(1), LW(2))
                ELSE
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), DAMAGE_RESULT_FINAL_HIT, LW(5))
                END_IF
                CASE_DEFAULT
            END_SWITCH
            LABEL(58)
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(50)
            LABEL(59)
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_SIBLING_EVENT
        // The spin slows and stops.
        WAIT_FRAMES(60)
        BEGIN_INLINE_EVENT_GET_ID(LW(13))
            // BUG: this LOOP(30) is closed by END_IF, not END_LOOP (no IF is open here)
            LOOP(30)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                SUBTRACT(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_SPEED_MAX, LW(0))
                WAIT_FRAMES(1)
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MARIO_WORK_SPIN_ON, 0)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
            END_LOOP
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        // Mario drops back to the floor; a Stylish press on the way down starts a spinning roll home.
        WAIT_FRAMES(30)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 7, 21, 21, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_B_1"))
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
        CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
        WAIT_FRAMES(6)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_GREATER_OR_EQUAL(LW(6), 2)
            GOTO(80)
        END_IF
    END_IF
    // No Stylish: the attack ends.
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    // Stylish: Mario rolls back to his place spinning (from the US version on the first Stylish is also
    // counted). A second press there ends in a confetti pose; a mistimed one plays the miss sound.
    LABEL(80)
#if VERSION >= VERSION_US
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
#endif
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
    SET_VALUE(LW(8), LW(7))
    ADD(LW(8), 12)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(7), LW(8), LW(8), 0)
    END_SIBLING_EVENT
    ADD(LW(8), 1)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), 30)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(10), 0)
        LOOP(LW(8))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
            IF_INT_EQUAL(LW(10), 0)
                CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            END_IF
            ADD(LW(10), 1)
            IF_INT_GREATER_OR_EQUAL(LW(10), 12)
                SET_VALUE(LW(10), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), 36)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        MODULO(LW(2), 360)
        DIVIDE(LW(2), 10)
        ADD(LW(2), LW(5))
        LOOP(10)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
        CASE_LESS_OR_EQUAL(-1)
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_MISS1"), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_S"))
        WAIT_MS(1500)
        CASE_DEFAULT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    GOTO(99)
    // A missed first press: the stomp's aftereffects and Mario backflips off the target.
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

#if VERSION <= VERSION_DEMO_US
/** Shrink Stomp badge attack: a normal jump; the shrink status comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_MiniminiFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
/** Sleepy Stomp badge attack: a normal jump; the sleep status comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_NemuraseFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
/** Soft Stomp badge attack: a normal jump; the defense-down effect comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_FunyafunyaJump)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
#endif
/**
 * Power Bounce badge attack: Mario keeps bouncing on one enemy while each timed press succeeds.
 * The number of bounces allowed is capped by mario_get_renzoku_count_max,
 * and the best combo is recorded by _record_renzoku_count.
 */
EVT_BEGIN(marioAttackEvent_RenzokuJump)
    // Target the selected enemy: the attack is declared, the badge's FP cost paid, Merlee's attack charm used
    // if due, and the Action Command set up.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 2)
    // Mario runs to the takeoff spot, faces the target and the Action Command starts.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    // Each bounce starts here (label 10); the game first checks whether it will land.
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
    // Mario jumps onto the target. The first jump is plain; each later bounce plays a higher bounce sound,
    // and a Stylish press partway up gives one of three random poses.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        SWITCH(LW(10))
            CASE_EQUAL(1)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE2"))
            CASE_EQUAL(2)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE3"))
            CASE_EQUAL(3)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE4"))
            CASE_EQUAL(4)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE5"))
            CASE_EQUAL(5)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE6"))
            CASE_EQUAL(6)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE7"))
            CASE_EQUAL(7)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE8"))
            CASE_EQUAL(8)
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE9"))
            CASE_DEFAULT
            SET_VALUE(LW(6), STRING("SFX_BTL_JUMP_CONTINUE10"))
        END_SWITCH
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, LW(6), 0, 0, -1, -1, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
#if VERSION >= VERSION_US
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 10, 17, 30, 0)
#else
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 11, 15, 30, 0)
#endif
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 3, MOVE_SOUND_KEEP)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 0, MOVE_SOUND_KEEP)
    END_IF
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press deals this hit, the audience reacts to the bounces so far
    // and Mario hops back off.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    END_IF
    // Pressed in time: the bounce hits and the combo is recorded (_record_renzoku_count); the last bounce
    // allowed by mario_get_renzoku_count_max hits as the final one.
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(_record_renzoku_count, LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(mario_get_renzoku_count_max, LW(3), LW(6), LW(7))
    IF_INT_LESS(LW(6), LW(7))
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_AC_SUCCESS, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    END_IF
    ADD(LW(10), 1)
    SET_VALUE(LW(1), GSW_MAIN_POWER_BOUNCE_RECORD_LO)
    SET_VALUE(LW(2), GSW_MAIN_POWER_BOUNCE_RECORD_HI)
    MULTIPLY(LW(2), 256)
    ADD(LW(1), LW(2))
    IF_INT_GREATER(LW(10), LW(1))
        SET_VALUE(LW(0), LW(10))
        IF_INT_GREATER(LW(0), 9999)
            SET_VALUE(LW(0), 9999)
        END_IF
        SET_VALUE(LW(1), LW(0))
        MODULO(LW(1), 256)
        SET_VALUE(LW(2), LW(0))
        DIVIDE(LW(2), 256)
        SET_VALUE(GSW_MAIN_POWER_BOUNCE_RECORD_LO, LW(1))
        SET_VALUE(GSW_MAIN_POWER_BOUNCE_RECORD_HI, LW(2))
    END_IF
    // The success effect and the audience reaction; while more bounces are allowed, the next one starts.
    SET_VALUE(LW(0), LW(10))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS(LW(6), LW(7))
        WAIT_FRAMES(2)
        CALL(btlevtcmd_StartAC, 0)
        GOTO(10)
    END_IF
    // The last allowed bounce is done: Mario backflips off the target.
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Multibounce badge attack: Mario bounces from enemy to enemy across the field, one timed press per stomp,
 * until a press fails or every target has been hit.
 */
EVT_BEGIN(marioAttackEvent_TugiTugiJump)
    // The attack is declared on every enemy, starting with the first target; the badge's FP cost is paid,
    // Merlee's attack charm used if due, and the Action Command set up.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 1, 1, 0)
    // Mario runs to the takeoff spot and the Action Command starts.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    // Each stomp starts here (label 10); the game first checks whether it will land.
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // Mario jumps to the current target; the first jump is plain.
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_BTL_JUMP_TUGITUGI1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    SWITCH(LW(10))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 0, MOVE_SOUND_KEEP)
        CASE_EQUAL(1)
        // The second jump: a Stylish press partway up gives one of three random poses.
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
#if VERSION >= VERSION_US
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 10, 19, 30, 0)
#else
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 13, 17, 30, 0)
#endif
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 3, MOVE_SOUND_KEEP)
        CASE_DEFAULT
        // The third and later jumps: the same Stylish chance; in JP a badly timed Stylish makes Mario flail
        // and counts as a failed Stylish.
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
#if VERSION >= VERSION_US
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 10, 19, 30, 0)
#else
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 13, 17, 30, 0)
#endif
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
#if VERSION < VERSION_US
                CASE_LESS_OR_EQUAL(-1)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_N_7"))
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
#endif
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), 4, MOVE_SOUND_KEEP)
    END_SWITCH
    // A dodge or miss: the enemy plays its dodge, Mario lands hard and the attack ends.
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    // The timed press on landing. A missed press deals this hit, the audience reacts to the stomps so far and
    // Mario hops back off.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
        GOTO(95)
    END_IF
    // Pressed in time: the stomp hits with the success effect, and Mario goes on to the next enemy with the
    // camera following him.
    CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 2)
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            CALL(evt_btl_camera_set_zoomSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        WAIT_FRAMES(2)
        ADD(LW(10), 1)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        END_INLINE_EVENT
        GOTO(10)
    END_IF
    // Every enemy has been stomped: the audience reacts and Mario backflips off the last one.
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    // Mario walks back to his place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // End of the attack: HP Drain and FP Drain take effect and Mario waits for the next turn.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Shared hammer attack: walk up to the front enemy, wind up with the hold-and-release command, swing and return.
 * Also runs as Mario's first strike (battle flag 2) with marioDefaultWeapon_Hammer.
 * LW(12) is the weapon and LW(14) picks the badge variant of the swing.
 */
#if VERSION <= VERSION_DEMO_US
#if VERSION <= VERSION_DEMO_US
static EVT_BEGIN(marioAttackEvent_NormalHammer)
    SET_VALUE(LW(14), 0)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
EVT_BEGIN(marioAttackEvent_GatsunHammer)
    SET_VALUE(LW(14), 1)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
/** Ice Smash badge attack: a normal hammer swing (LW(14) = 0); the freeze comes from badgeWeapon_IceNaguri. */
EVT_BEGIN(marioAttackEvent_IceNaguri)
    SET_VALUE(LW(14), 0)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
EVT_BEGIN(marioAttackEvent_TsuranukiHammer)
    SET_VALUE(LW(14), 1)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
#endif
/**
 * Shared hammer attack: walk up to the front enemy, wind up with the hold-and-release command, swing and return.
 * Also runs as Mario's first strike (battle flag 2) with marioDefaultWeapon_Hammer.
 * LW(12) is the weapon and LW(14) picks the badge variant of the swing.
 */
EVT_BEGIN(marioAttackEvent_NormalHammer_Core)
    // On a First Strike Mario starts at his home spot facing the target; on a normal turn the attack is declared,
    // FP are paid and Merlee's charm is checked.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Hammer))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    // On a normal turn the hold-and-release Action Command is set up; its timing depends on the move.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
        SWITCH(LW(12))
            CASE_EQUAL(PTR(&marioDefaultWeapon_Hammer))
            CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_DEFAULT
            CALL(btlevtcmd_AcSetParamAll, 4, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        END_SWITCH
        CALL(btlevtcmd_AcSetFlag, 0)
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    END_IF
    // Mario walks up to the target; on a First Strike he waits a second first and the Action Command is set up
    // while he is still walking.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
            CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
            SWITCH(LW(12))
                CASE_EQUAL(PTR(&marioDefaultWeapon_Hammer))
                CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
                CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
                CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_DEFAULT
                CALL(btlevtcmd_AcSetParamAll, 4, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            END_SWITCH
            CALL(btlevtcmd_AcSetFlag, 0)
            CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    // Mario faces the target; if its spikes would hurt him first, he takes the damage and the attack ends.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // Pick the wind-up, raise and swing poses for Mario's hammer level (Piercing Blow has its own swing).
    IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), STRING("M_H_5A"))
            SET_VALUE(LW(1), STRING("M_H_5B"))
            SET_VALUE(LW(2), STRING("M_H_14"))
            CASE_EQUAL(3)
            SET_VALUE(LW(0), STRING("M_H_9A"))
            SET_VALUE(LW(1), STRING("M_H_9B"))
            SET_VALUE(LW(2), STRING("M_H_15"))
            CASE_DEFAULT
            SET_VALUE(LW(0), STRING("M_H_1A"))
            SET_VALUE(LW(1), STRING("M_H_1B"))
            SET_VALUE(LW(2), STRING("M_H_13"))
        END_SWITCH
    ELSE
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), STRING("M_H_5A"))
            SET_VALUE(LW(1), STRING("M_H_5B"))
            SET_VALUE(LW(2), STRING("M_H_5C"))
            CASE_EQUAL(3)
            SET_VALUE(LW(0), STRING("M_H_9A"))
            SET_VALUE(LW(1), STRING("M_H_9B"))
            SET_VALUE(LW(2), STRING("M_H_9C"))
            CASE_DEFAULT
            SET_VALUE(LW(0), STRING("M_H_1A"))
            SET_VALUE(LW(1), STRING("M_H_1B"))
            SET_VALUE(LW(2), STRING("M_H_1C"))
        END_SWITCH
    END_IF
    // Mario raises the hammer and charges; Head Rattle and Piercing Blow make a star sparkle on the hammer.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    BEGIN_SIBLING_EVENT
        SWITCH(LW(12))
            CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            SET_VALUE(LW(13), 0)
            CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            SET_VALUE(LW(13), 2)
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_ftomsec, 9, LW(0))
            WAIT_MS(LW(0))
            CALL(_hammer_star_effect, UNIT_ID_SELF, LW(13))
            CASE_END
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    // The Action Command runs; then Mario swings down and the hammer strikes (Piercing Blow has its own sound).
    CALL(btlevtcmd_ftomsec, 30, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 0)
    CALL(btlevtcmd_ResultAC)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    WAIT_FRAMES(7)
    IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_TURANUKI1"), 0)
    ELSE
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND3"), 0)
            CASE_EQUAL(1)
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND2"), 0)
            CASE_DEFAULT
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND1"), 0)
        END_SWITCH
    END_IF
    // If the attack misses, the target plays its dodge reaction and no damage is dealt.
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(90)
    END_IF
    // The hit: normal damage after a failed Action Command; a success deals more and shows the rating.
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    // The camera shakes with the damage dealt (capped at 9 from the US version on), the weapon's side effects
    // apply and the first Stylish window opens.
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 60, 0)
    CALL(btlevtcmd_ACRGetResult, LW(15), LW(13))
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_audience_acrobat_notry)
        SET_VALUE(LW(0), 60)
        SUBTRACT(LW(0), LW(13))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            WAIT_FRAMES(LW(0))
        END_IF
        IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
        END_IF
        WAIT_FRAMES(10)
        GOTO(95)
        END_IF
        IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
            LOOP(30)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 12)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 35)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_EQUAL(LW(14), 0)
            IF_INT_EQUAL(LW(0), 2)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                ADD(LW(1), 50)
                CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(90)
                GOTO(95)
            ELSE
                CALL(evt_audience_acrobat_notry)
                GOTO(95)
            END_IF
        ELSE
            SWITCH(LW(0))
                CASE_EQUAL(2)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CASE_EQUAL(1)
                CALL(evt_audience_acrobat_notry)
                GOTO(95)
            END_SWITCH
        END_IF
        // The badge swings (Power Smash, Mega Smash, Piercing Blow) go on into a higher, faster flip with another
        // Stylish window.
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
            LOOP(40)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 18)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        // A good press there turns it into a long twisting jump, ending in a pose with confetti and the finish sound.
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
        BEGIN_SIBLING_EVENT
            LOOP(60)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 12, 12)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(35)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 15, 28, 28, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_C_3"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
    END_IF
    // Mario walks back to his home spot.
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#else
/**
 * Shared hammer attack: walk up to the front enemy, wind up with the hold-and-release command, swing and return.
 * Also runs as Mario's first strike (battle flag 2) with marioDefaultWeapon_Hammer.
 * LW(12) is the weapon and LW(14) picks the badge variant of the swing.
 */
EVT_BEGIN(marioAttackEvent_NormalHammer_Core)
    // On a First Strike Mario starts at his home spot facing the target; on a normal turn the attack is declared,
    // FP are paid and Merlee's charm is checked.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Hammer))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    // On a normal turn the hold-and-release Action Command is set up; its timing depends on the move.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
        SWITCH(LW(12))
            CASE_EQUAL(PTR(&marioDefaultWeapon_Hammer))
            CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CASE_DEFAULT
            CALL(btlevtcmd_AcSetParamAll, 4, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        END_SWITCH
        CALL(btlevtcmd_AcSetFlag, 0)
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    END_IF
    // Mario walks up to the target; on a First Strike he waits a second first and the Action Command is set up
    // while he is still walking.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
            CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
            SWITCH(LW(12))
                CASE_EQUAL(PTR(&marioDefaultWeapon_Hammer))
                CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
                CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
                CALL(btlevtcmd_AcSetParamAll, 4, 30, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
                CASE_DEFAULT
                CALL(btlevtcmd_AcSetParamAll, 4, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            END_SWITCH
            CALL(btlevtcmd_AcSetFlag, 0)
            CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    // Mario faces the target; if its spikes would hurt him first, he takes the damage and the attack ends.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // Pick the wind-up, raise and swing poses for Mario's hammer level (Piercing Blow has its own swing).
    IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), STRING("M_H_5A"))
            SET_VALUE(LW(1), STRING("M_H_5B"))
            SET_VALUE(LW(2), STRING("M_H_14"))
            CASE_EQUAL(3)
            SET_VALUE(LW(0), STRING("M_H_9A"))
            SET_VALUE(LW(1), STRING("M_H_9B"))
            SET_VALUE(LW(2), STRING("M_H_15"))
            CASE_DEFAULT
            SET_VALUE(LW(0), STRING("M_H_1A"))
            SET_VALUE(LW(1), STRING("M_H_1B"))
            SET_VALUE(LW(2), STRING("M_H_13"))
        END_SWITCH
    ELSE
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), STRING("M_H_5A"))
            SET_VALUE(LW(1), STRING("M_H_5B"))
            SET_VALUE(LW(2), STRING("M_H_5C"))
            CASE_EQUAL(3)
            SET_VALUE(LW(0), STRING("M_H_9A"))
            SET_VALUE(LW(1), STRING("M_H_9B"))
            SET_VALUE(LW(2), STRING("M_H_9C"))
            CASE_DEFAULT
            SET_VALUE(LW(0), STRING("M_H_1A"))
            SET_VALUE(LW(1), STRING("M_H_1B"))
            SET_VALUE(LW(2), STRING("M_H_1C"))
        END_SWITCH
    END_IF
    // Mario raises the hammer and charges; Head Rattle and Piercing Blow make a star sparkle on the hammer.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    BEGIN_SIBLING_EVENT
        SWITCH(LW(12))
            CASE_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            SET_VALUE(LW(13), 0)
            CASE_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            SET_VALUE(LW(13), 2)
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_ftomsec, 9, LW(0))
            WAIT_MS(LW(0))
            CALL(_hammer_star_effect, UNIT_ID_SELF, LW(13))
            CASE_END
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    // The Action Command runs; then Mario swings down and the hammer strikes (Piercing Blow has its own sound).
    CALL(btlevtcmd_ftomsec, 30, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 0)
    CALL(btlevtcmd_ResultAC)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    WAIT_FRAMES(7)
    IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_TURANUKI1"), 0)
    ELSE
        CALL(_get_mario_hammer_lv, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND3"), 0)
            CASE_EQUAL(1)
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND2"), 0)
            CASE_DEFAULT
            CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_HIT_GROUND1"), 0)
        END_SWITCH
    END_IF
    // If the attack misses, the target plays its dodge reaction and no damage is dealt.
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(90)
    END_IF
    // The hit: normal damage after a failed Action Command; a success deals more and shows the rating.
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    // The camera shakes with the damage dealt (capped at 9 from the US version on), the weapon's side effects
    // apply and the first Stylish window opens.
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 12, 60, 0)
#else
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 60, 0)
#endif
    // Without a well-timed press Mario just holds a pose and the attack ends.
    CALL(btlevtcmd_ACRGetResult, LW(15), LW(13))
    IF_INT_EQUAL(LW(15), 1)
        GOTO(91)
    END_IF
    IF_INT_LESS_OR_EQUAL(LW(15), -1)
        GOTO(91)
    END_IF
    GOTO(92)
    LABEL(91)
    CALL(evt_audience_acrobat_notry)
    SET_VALUE(LW(0), 60)
    SUBTRACT(LW(0), LW(13))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        WAIT_FRAMES(LW(0))
    END_IF
    IF_INT_EQUAL(LW(12), PTR(&badgeWeapon_TsuranukiNaguri))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
    END_IF
    WAIT_FRAMES(10)
    GOTO(95)
    LABEL(92)
    // Stylish: Mario backflips in place; for the plain Hammer and Ice Smash a second good press ends it in a pose
    // with confetti.
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
            LOOP(30)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 12)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 35)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_EQUAL(LW(14), 0)
            IF_INT_EQUAL(LW(0), 2)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                ADD(LW(1), 50)
                CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(90)
                GOTO(95)
            ELSE
                CALL(evt_audience_acrobat_notry)
                GOTO(95)
            END_IF
        ELSE
            SWITCH(LW(0))
                CASE_EQUAL(2)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CASE_EQUAL(1)
                CALL(evt_audience_acrobat_notry)
                GOTO(95)
            END_SWITCH
        END_IF
        // The badge swings (Power Smash, Mega Smash, Piercing Blow) go on into a higher, faster flip with another
        // Stylish window.
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
            LOOP(40)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 18)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        // A good press there turns it into a long twisting jump, ending in a pose with confetti and the finish sound.
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
        BEGIN_SIBLING_EVENT
            LOOP(60)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 12, 12)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(35)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 15, 28, 28, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_C_3"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
    END_IF
    // Mario walks back to his home spot.
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

/**
 * Super Hammer and Ultra Hammer (also their first strikes): Mario winds up with the hold command and swings;
 * a success knocks the target back (the shared blow-away script hits the enemies behind it with the ...Involved
 * weapon).
 * The Ultra Hammer adds a whirlwind, and after a success it waits on the target's knock-back tokens
 * and hits it once more with marioWeapon_UltraHammerFinish:
 * the blow-away script sets token 64 when the target is off the stage (Mario clears it),
 * then token 128 once it has flown back in from the other side and landed at its home.
 */
static EVT_BEGIN(marioAttackEvent_KaitenHammer)
    // On a First Strike Mario starts at his home spot facing the target; on a normal turn the attack is declared,
    // FP are paid and Merlee's charm is checked.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
        CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    // The hold-and-release Action Command is set up (the Ultra Hammer uses different timing).
    CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_AcSetParamAll, 10, 8, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        CASE_END
        CASE_DEFAULT
        CALL(btlevtcmd_AcSetParamAll, 7, 15, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_AcSetFlag, AC_STICK_KEEP_LEFT_FLAG_COMPACT_GAUGE)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    // Mario walks up beside the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // Mario faces the target; if its spikes would hurt him first, he takes the damage and the attack ends.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(7))
    IF_INT_EQUAL(LW(7), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // Mario raises the hammer and spins it around while the Action Command runs, with the Super or Ultra Hammer
    // spin sound.
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_6A"))
        SET_VALUE(LW(1), STRING("M_H_6B"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_10A"))
        SET_VALUE(LW(1), STRING("M_H_10B"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_2A"))
        SET_VALUE(LW(1), STRING("M_H_2B"))
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, STRING("p_kaiten_h"), 0)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("P_H_1A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_paper_light_off, UNIT_ID_SELF, 1)
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_KAITEN1"), EVT_NULLPTR, 0, LW(15))
        CASE_END
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_ULTRA1"), EVT_NULLPTR, 0, LW(15))
        CASE_END
        CASE_DEFAULT
    END_SWITCH
    WAIT_FRAMES(28)
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("P_H_1B"))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
        CALL(evt_snd_sfxoff, LW(15))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(evt_snd_sfxoff, LW(15))
        CASE_END
        CASE_END
    END_SWITCH
    // Mario swings the hammer through and the camera pulls back to the whole stage.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_AnimeChangePaperAnime, UNIT_ID_SELF, 1, STRING("P_H_1C"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    // If the attack misses, the target plays its dodge reaction and no damage is dealt.
    IF_INT_NOT_EQUAL(LW(7), 1)
        IF_INT_EQUAL(LW(7), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(7), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(7), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(90)
    END_IF
    // The hit: normal damage after a failed Action Command; a success deals more and shows the rating.
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CASE_END
        CASE_DEFAULT
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    // The weapon's side effects apply.
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        SET_VALUE(LW(0), 1)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(0), 0)
    END_SWITCH
    // Ultra Hammer: a whirlwind rises and Mario keeps spinning in place.
    IF_INT_EQUAL(LW(0), 1)
        BEGIN_INLINE_EVENT
            CALL(_whirlwind_effect, UNIT_ID_SELF, 60)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 36)
        BEGIN_SIBLING_EVENT_GET_ID(LW(9))
            LOOP(6)
                SWITCH(LW(12))
                    CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
                    CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
                    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_ULTRA2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CASE_END
                END_SWITCH
                LOOP(10)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_SIBLING_EVENT
        // If the swing did not connect, Mario spins to a stop and stands for a moment.
        IF_INT_NOT_EQUAL(LW(7), 1)
            WAIT_FRAMES(50)
            DELETE_EVENT(LW(9))
            BEGIN_SIBLING_EVENT
                DIVIDE(LW(0), 3)
                LOOP(30)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_S"))
            WAIT_FRAMES(24)
            WAIT_MS(1500)
        ELSE
            // On a hit, Mario spins on with a blur and a Stylish window opens; a good press ends the spin in a pose.
            CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 80, 255, 255, 255, 0, 0)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
            BEGIN_INLINE_EVENT
                WAIT_FRAMES(50)
                CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_FRAMES(10)
                CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
            END_INLINE_EVENT
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 40, 63, 63, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), EVT_NULLPTR)
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
                CALL(_kaiten_hammer_acrobat_rotate, UNIT_ID_SELF, 1, 30, STRING("M_V_2"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(30)
                CASE_DEFAULT
                CALL(btlevtcmd_WaitEventEnd, LW(9))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_IF
    ELSE
        // Super Hammer: a missed swing spins to a stop as above; on a hit a whirlwind rises and the Stylish window
        // opens.
        IF_INT_NOT_EQUAL(LW(7), 1)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            MULTIPLY(LW(0), 36)
            BEGIN_SIBLING_EVENT_GET_ID(LW(9))
                LOOP(6)
                    SWITCH(LW(12))
                        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
                        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
                        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_KAITEN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                        CASE_END
                    END_SWITCH
                    LOOP(10)
                        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(50)
            DELETE_EVENT(LW(9))
            BEGIN_SIBLING_EVENT
                DIVIDE(LW(0), 3)
                LOOP(30)
                    CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_S"))
            WAIT_FRAMES(24)
            WAIT_MS(1500)
        ELSE
            BEGIN_INLINE_EVENT
                CALL(_whirlwind_effect, UNIT_ID_SELF, 60)
            END_INLINE_EVENT
            BEGIN_SIBLING_EVENT_GET_ID(LW(9))
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                MULTIPLY(LW(0), 36)
                LOOP(6)
                    SWITCH(LW(12))
                        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
                        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
                        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_KAITEN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                        CASE_END
                    END_SWITCH
                    LOOP(10)
                        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 40, 63, 63, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), EVT_NULLPTR)
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
                CALL(_kaiten_hammer_acrobat_rotate, UNIT_ID_SELF, 1, 30, STRING("M_V_2"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(30)
                CASE_DEFAULT
                CALL(btlevtcmd_WaitEventEnd, LW(9))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePaperGroup, UNIT_ID_SELF, 1, 0, 0)
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_IF
    END_IF
    // Mario walks back to his home spot.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // Ultra Hammer only, after a successful hit: wait for the knocked-away target to leave the stage and land back
    // at its home, then hit it once more.
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CASE_END
        CASE_DEFAULT
        GOTO(99)
    END_SWITCH
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        GOTO(99)
    END_IF
    IF_INT_NOT_EQUAL(LW(7), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckToken, LW(3), kKnockedAway, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    LABEL(96)
    CALL(btlevtcmd_CheckToken, LW(3), kKnockedAwayWaiting, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(96)
    END_IF
    CALL(btlevtcmd_OffToken, LW(3), kKnockedAwayWaiting)
    LABEL(98)
    CALL(btlevtcmd_CheckToken, LW(3), kKnockedAwayReturned, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, TOKEN_FLAG_UNUSED_8, LW(0))
        IF_INT_EQUAL(LW(0), 0)
        END_IF
        WAIT_FRAMES(1)
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&marioWeapon_UltraHammerFinish), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * First strike with the Super or Ultra Hammer when the battle starts from a field hammer hit:
 * picks the first-strike weapon for Ultra Hammer (hammer level 3) or Super Hammer,
 * then runs the spinning hammer attack.
 */
EVT_BEGIN(marioAttackEvent_FirstAttackKaitenHammer)
    CALL(_get_mario_hammer_lv, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 3)
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
    ELSE
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
    END_IF
    CALL_CHILD(PTR(marioAttackEvent_KaitenHammer))
    RETURN
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_JishinHammer)
    // The attack is declared on every enemy, FP are paid, Merlee's charm is checked and the hold-and-release
    // Action Command is set up.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    // Mario walks to the middle of the stage and the camera pulls back.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    // Mario raises the hammer and charges while the player holds the stick.
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_14"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_15"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_13"))
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(75)
    // Once the charge reaches level 3, Mario leans back and jumps up for a bigger slam.
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(25)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(5)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(5), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(8)
        CALL(evt_sub_intpl_init, EASING_SINE_OUT, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(6), LW(7), LW(8), 40, -1)
    END_SIBLING_EVENT
    // When the player releases before that, Mario slams the ground at once.
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(8))
    CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        GOTO(10)
    END_IF
    DELETE_EVENT(LW(13))
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        SET_VALUE(LW(2), STRING("M_H_13"))
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        SET_VALUE(LW(2), STRING("M_H_15"))
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
        SET_VALUE(LW(2), STRING("M_H_14"))
        END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
    // Full charge: Mario slams the hammer down as he comes back down from his jump.
    LABEL(10)
    CALL(btlevtcmd_WaitEventEnd, LW(13))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    // The quake: the stage shakes, the audience reacts and the Action Command rating is shown on a success.
    LABEL(40)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    // Every enemy in turn: those the quake reaches flinch and take damage when the shaking ends (more after a
    // success); the rest play their dodge reaction.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    GOTO(51)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    LABEL(51)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
    END_IF
    GOTO(50)
    // After the shaking, the weapon's side effects apply and Mario lifts his hammer as the Stylish window opens;
    // without a good press the attack ends.
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
        WAIT_FRAMES(18)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 17, 17, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_SWITCH
    // Stylish: Mario backflips back to his home spot; a second good press ends it in a pose with confetti.
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
        LOOP(60)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 12)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 42, 55, 55, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -1)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    // Mario walks back to his home spot.
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_JishinHammer)
    // The attack is declared on every enemy, FP are paid, Merlee's charm is checked and the hold-and-release
    // Action Command is set up.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    // Mario walks to the middle of the stage and the camera pulls back.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    // Mario raises the hammer and charges while the player holds the stick.
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_14"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_15"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_13"))
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(75)
    // Once the charge reaches level 3, Mario leans back and jumps up for a bigger slam.
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(25)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(5)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(5), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(8)
        CALL(evt_sub_intpl_init, EASING_SINE_OUT, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(6), LW(7), LW(8), 40, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    // When the player releases before that, Mario slams the ground at once.
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(8))
    CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        GOTO(10)
    END_IF
    DELETE_EVENT(LW(13))
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        SET_VALUE(LW(2), STRING("M_H_14"))
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        SET_VALUE(LW(2), STRING("M_H_13"))
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
        SET_VALUE(LW(2), STRING("M_H_15"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
    // Full charge: Mario slams the hammer down as he comes back down from his jump.
    LABEL(10)
    CALL(btlevtcmd_WaitEventEnd, LW(13))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    // The quake: the stage shakes, the audience reacts and the Action Command rating is shown on a success.
    LABEL(40)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    // Every enemy in turn: those the quake reaches flinch and take damage when the shaking ends (more after a
    // success); the rest play their dodge reaction.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    GOTO(51)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    LABEL(51)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
    END_IF
    GOTO(50)
    // After the shaking, the weapon's side effects apply and Mario lifts his hammer as the Stylish window opens;
    // without a good press the attack ends.
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
        WAIT_FRAMES(18)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 17, 17, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_SWITCH
    // Stylish: Mario backflips back to his home spot; a second good press ends it in a pose with confetti.
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
#if VERSION >= VERSION_US
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -30, 20, 0)
#endif
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
        LOOP(60)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 12)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 42, 55, 55, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    // Mario walks back to his home spot.
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

/**
 * Mega Quake badge attack: the Quake Hammer slam with a shorter command window (16 instead of 25)
 * and the stronger weapon.
 * No map, chest or shop in the tree gives ITEM_MEGA_QUAKE, so it is unreachable without hacking the pouch.
 */
EVT_BEGIN(marioAttackEvent_UltraJishinHammer)
    // The attack is declared on every enemy, FP are paid, Merlee's charm is checked and the hold-and-release
    // Action Command is set up.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 16, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    // Mario walks to the middle of the stage and the camera pulls back.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    // Mario raises the hammer and charges while the player holds the stick.
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_13"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_14"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_15"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(32)
    // Once the charge reaches level 2, Mario leans back and jumps up for a bigger slam.
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(44)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        WAIT_FRAMES(10)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(5), 10)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(14)
        CALL(evt_sub_intpl_init, EASING_SINE_OUT, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(6))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.23925781f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(6), LW(7), LW(8), 55, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    // When the player releases before that, Mario slams the ground at once.
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(8))
    CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        GOTO(10)
    END_IF
    DELETE_EVENT(LW(13))
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        SET_VALUE(LW(2), STRING("M_H_13"))
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        SET_VALUE(LW(2), STRING("M_H_15"))
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
        SET_VALUE(LW(2), STRING("M_H_14"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
    // Charged: Mario slams the hammer down as he comes back down from his jump.
    LABEL(10)
    CALL(btlevtcmd_WaitEventEnd, LW(13))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    // The quake: the stage shakes, the audience reacts and the Action Command rating is shown on a success.
    LABEL(40)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    // Every enemy in turn: those the quake reaches flinch and take damage when the shaking ends (more after a
    // success); the rest play their dodge reaction.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    GOTO(51)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    LABEL(51)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
    END_IF
    GOTO(50)
    // After the shaking, the weapon's side effects apply and Mario lifts his hammer; Mega Quake has no Stylish.
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_H_16"))
    WAIT_FRAMES(10)
    // Mario walks back to his home spot.
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_HammerRotate)
    SET_VALUE(LW(0), -25)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 3, 0, 12, 0)
    LABEL(0)
    CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 3, 0, 0, LW(0))
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_HammerNageru)
    // The attack is declared on the chosen enemy, FP are paid, Merlee's charm is checked and the hold-and-release
    // Action Command is set up.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 5, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    // Whether the throw can connect is decided now; Mario turns to the target and the camera pulls back.
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    // Mario raises the hammer and charges while the player holds the stick; the hammer he will throw is matched to
    // his hammer level.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_7"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("M_H_8"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_11"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("M_H_12"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_3"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("M_H_4"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    // On release Mario throws the hammer, which flies on its own, and the Stylish window opens; without a good
    // press he stands for a moment and the attack ends.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_NAGERU1"), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    WAIT_FRAMES(3)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerNageru_object), LW(15))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_MS(1000)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
        WAIT_MS(667)
        GOTO(94)
    END_SWITCH
    // Stylish: Mario backflips in place; a second good press ends it in a pose with confetti.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
#if VERSION >= VERSION_US
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -40, 30, 0)
#endif
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 20, 0)
        LOOP(30)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -24)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 12, 25, 25, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    // Wait for the hammer to finish its flight, then Mario walks back to his home spot.
    LABEL(94)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 5)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_HammerNageru_object)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    SET_VALUE(LW(0), -25)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 3, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerRotate), LW(13))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(6.0f), FLOAT(0.34960938f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, 3)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -30)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -15)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        DELETE_EVENT(LW(13))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        GOTO(90)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
#if VERSION >= VERSION_US
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
#endif
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    WAIT_FRAMES(20)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    RETURN
EVT_END()

/** Empty effect name passed to evt_eff and evt_eff64 for Fire Drive's sparks and smoke. */
static const char battle_mario_nullstr[4] = "";
extern const f32 battle_mario_one;

EVT_BEGIN(marioAttackEvent_FireNaguri)
    // The attack is declared on every enemy, FP are paid, Merlee's charm is checked and the Action Command is set
    // up.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 0, 83, 63, 83, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, AC_TIMING_WIDE_FLAG_SWAP_RESULT_ICON)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 11, 1, 0)
    WAIT_FRAMES(2)
    // The fireball appears at the top of the stage in front of Mario and drops toward him.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 40, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("M_H_8"))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, MARIO_PARTS_WORK_FIRE_BALL, 1)
    BEGIN_SIBLING_EVENT
        CALL(_mario_fire_ball_controll, UNIT_ID_SELF, 3)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    ADD(LW(1), 140)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 60, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    // Wait until the player holds the stick left; the fireball stops where it is.
    LABEL(10)
    CALL(btlevtcmd_PadCheckNow, 0, BATTLE_PAD_STICK_LEFT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        DELETE_EVENT(LW(15))
    END_IF
    // Mario raises the hammer and charges it with fire while the fireball falls toward the floor.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_18A"))
        SET_VALUE(LW(1), STRING("M_H_18B"))
        SET_VALUE(LW(2), STRING("M_H_18C"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_19A"))
        SET_VALUE(LW(1), STRING("M_H_19B"))
        SET_VALUE(LW(2), STRING("M_H_19C"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_17A"))
        SET_VALUE(LW(1), STRING("M_H_17B"))
        SET_VALUE(LW(2), STRING("M_H_17C"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_FIRE1"), EVT_NULLPTR, 0, LW(15))
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(4.0f), FLOAT(0.19921875f))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_FIRE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FallPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 75)
    END_INLINE_EVENT
    // The Action Command runs: the player must release the stick in time to hit the fireball.
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(1))
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), AC_RESULT_SUCCESS)
        // Missed: Mario swings at nothing and the fireball fizzles out in smoke on the floor; no enemy is hurt.
        CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
        BEGIN_SIBLING_EVENT
            CALL(_get_mario_hammer_lv, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(2)
                CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
                CASE_EQUAL(1)
                CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
                CASE_DEFAULT
                CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
            END_SWITCH
            CALL(evt_snd_sfxoff, LW(15))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
            WAIT_FRAMES(7)
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, MARIO_PARTS_WORK_FIRE_BALL, 0)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        // FAKE: no-op (char*) cast; PTR(battle_mario_nullstr) changes the object
        CALL(evt_eff64, PTR((char*)battle_mario_nullstr), STRING("fire_spark_n64"), 0, LW(0), LW(1), LW(2), 30, 40, FLOAT(1.0f), 12, 100, FLOAT(1.0f), FLOAT(1.0f), 0)
        ADD(LW(1), 5)
        // FAKE: no-op (char*) cast; PTR(battle_mario_nullstr) changes the object
        CALL(evt_eff, PTR((char*)battle_mario_nullstr), STRING("kemuri_test"), 11, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        LABEL(15)
        IF_INT_EQUAL(LW(3), -1)
            GOTO(17)
        END_IF
        IF_INT_EQUAL(LW(3), LW(6))
            GOTO(16)
        END_IF
        LABEL(16)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(15)
        LABEL(17)
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        LABEL(20)
        IF_INT_EQUAL(LW(3), -1)
            GOTO(22)
        END_IF
        IF_INT_NOT_EQUAL(LW(3), LW(6))
            GOTO(21)
        END_IF
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        LABEL(21)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(20)
        LABEL(22)
        WAIT_FRAMES(30)
        GOTO(99)
    END_IF
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING2"), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING1"), 0)
        CASE_DEFAULT
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_SWING3"), 0)
    END_SWITCH
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
    WAIT_FRAMES(7)
    DELETE_EVENT(LW(14))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT_ZOOM)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_HAMMER_FIRE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_FireNaguri_object), LW(15))
    // A Stylish window opens right after the smash; a good press makes Mario spin and pose.
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        LOOP(20)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 36, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_V_2"))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(500)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_S_1"))
    END_SWITCH
    // Wait for the fireball to finish, then Mario walks back to his home spot.
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The fireball (Mario's part 3) of Fire Drive: travels along the stage floor to each target in turn,
 * burning them with fire pillars (_fire_wave), or rolls off the stage edge when no target is left.
 */
EVT_BEGIN(marioAttackEvent_FireNaguri_object)
    // No enemy is left to target: the fireball rolls off past the stage edge.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(5), 2)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(6.0f), FLOAT(0.2998047f))
        CALL(btlevtcmd_FallPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, MARIO_PARTS_WORK_FIRE_BALL, 0)
        RETURN
    END_IF
    // The fireball bounces down onto the floor in front of the first enemy; the weapon's side effects apply and
    // the Action Command rating is shown on a success.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(4.0f), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, MARIO_PARTS_WORK_FIRE_BALL, 0)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    // The fireball runs along the floor toward each enemy in turn, leaving a fire pillar every 6 frames.
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, -1, -1, 2, LW(12))
    MULTIPLY(LW(12), 16)
    SET_VALUE(LW(13), 5)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    GOTO(51)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    LABEL(51)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, -1, -1, 2, LW(14))
    IF_INT_GREATER_OR_EQUAL(LW(14), 0)
        IF_INT_LESS_OR_EQUAL(LW(7), LW(11))
            WAIT_FRAMES(6)
            ADD(LW(7), LW(12))
            BEGIN_INLINE_EVENT
                CALL(_fire_wave, LW(7), LW(8), LW(9), LW(13))
            END_INLINE_EVENT
            GOTO(51)
        END_IF
    ELSE
        IF_INT_GREATER_OR_EQUAL(LW(7), LW(11))
            WAIT_FRAMES(6)
            ADD(LW(7), LW(12))
            BEGIN_INLINE_EVENT
                CALL(_fire_wave, LW(7), LW(8), LW(9), LW(13))
            END_INLINE_EVENT
            GOTO(51)
        END_IF
    END_IF
    // The enemy it reaches takes the hit (more after a successful Action Command) and the pillars get smaller;
    // enemies that avoid it play their dodge reaction.
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_GetResultAC, LW(0))
        IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(13), 2)
            SUBTRACT(LW(13), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
    END_IF
    GOTO(50)
    // After the last enemy, the trail of fire runs on to the stage edge.
    LABEL(90)
    LABEL(92)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, -1, -1, 2, LW(10))
    CALL(btlevtcmd_GetStageSize, LW(11), EVT_NULLPTR, EVT_NULLPTR)
    MULTIPLY(LW(11), LW(10))
    IF_INT_GREATER_OR_EQUAL(LW(11), 0)
        IF_INT_LESS_OR_EQUAL(LW(7), LW(11))
            WAIT_FRAMES(6)
            ADD(LW(7), LW(12))
            BEGIN_INLINE_EVENT
                CALL(_fire_wave, LW(7), LW(8), LW(9), LW(13))
            END_INLINE_EVENT
            GOTO(92)
        END_IF
    ELSE
        IF_INT_GREATER_OR_EQUAL(LW(7), LW(11))
            WAIT_FRAMES(6)
            ADD(LW(7), LW(12))
            BEGIN_INLINE_EVENT
                CALL(_fire_wave, LW(7), LW(8), LW(9), LW(13))
            END_INLINE_EVENT
            GOTO(92)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Charge and Charge P: the user poses, spins, and gains the attack boost of the weapon in LW(12),
 * with the charge sound and crystal sparkle; no action command.
 */
EVT_BEGIN(_NormalCharge_core)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    WAIT_FRAMES(45)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 24, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_CHARGE1"), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    DIVIDE(LW(5), 2)
    ADD(LW(1), LW(5))
    ADD(LW(2), 10)
    CALL(evt_eff64, STRING("noname"), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
/** Charge badge move for Mario. */
EVT_BEGIN(marioAttackEvent_NormalCharge)
    SET_VALUE(LW(12), PTR(&badgeWeapon_Charge))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()
/** Charge P badge move for the partner. */
EVT_BEGIN(marioAttackEvent_NormalChargeP)
    SET_VALUE(LW(12), PTR(&badgeWeapon_ChargeP))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()
#endif
/**
 * Super Charge and Super Charge P: the same pose and spin as Charge with the super charge sound
 * and the weapon in LW(12). No map, chest or shop in the tree gives ITEM_SUPER_CHARGE or ITEM_SUPER_CHARGE_P.
 */
EVT_BEGIN(_SuperCharge_core)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    WAIT_FRAMES(45)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 24, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_SUPER_CHARGE1"), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    DIVIDE(LW(5), 2)
    ADD(LW(1), LW(5))
    ADD(LW(2), 10)
    CALL(evt_eff64, STRING("noname"), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

#if VERSION > VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_GatsunJump)
    SET_VALUE(LW(12), PTR(&badgeWeapon_GatsunJump))
    CALL_CHILD(PTR(marioAttackEvent_GatsuDokaJump))
    RETURN
EVT_END()
#endif

#if VERSION > VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_DokanJump)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DokanJump))
    CALL_CHILD(PTR(marioAttackEvent_GatsuDokaJump))
    RETURN
EVT_END()
#endif

/** Shrink Stomp badge attack: a normal jump; the shrink status comes from the badge's weapon data. */
#if VERSION > VERSION_DEMO_US
/** Shrink Stomp badge attack: a normal jump; the shrink status comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_MiniminiFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
#endif

/** Sleepy Stomp badge attack: a normal jump; the sleep status comes from the badge's weapon data. */
#if VERSION > VERSION_DEMO_US
/** Sleepy Stomp badge attack: a normal jump; the sleep status comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_NemuraseFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
#endif

/** Soft Stomp badge attack: a normal jump; the defense-down effect comes from the badge's weapon data. */
#if VERSION > VERSION_DEMO_US
/** Soft Stomp badge attack: a normal jump; the defense-down effect comes from the badge's weapon data. */
EVT_BEGIN(marioAttackEvent_FunyafunyaJump)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()
#endif

#if VERSION > VERSION_DEMO_US
static EVT_BEGIN(marioAttackEvent_NormalHammer)
    SET_VALUE(LW(14), 0)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
#endif

#if VERSION > VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_GatsunHammer)
    SET_VALUE(LW(14), 1)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
#endif

/** Ice Smash badge attack: a normal hammer swing (LW(14) = 0); the freeze comes from badgeWeapon_IceNaguri. */
#if VERSION > VERSION_DEMO_US
/** Ice Smash badge attack: a normal hammer swing (LW(14) = 0); the freeze comes from badgeWeapon_IceNaguri. */
EVT_BEGIN(marioAttackEvent_IceNaguri)
    SET_VALUE(LW(14), 0)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
#endif

#if VERSION > VERSION_DEMO_US
EVT_BEGIN(marioAttackEvent_TsuranukiHammer)
    SET_VALUE(LW(14), 1)
    CALL_CHILD(PTR(marioAttackEvent_NormalHammer_Core))
    RETURN
EVT_END()
#endif

/** Charge badge move for Mario. */
#if VERSION > VERSION_DEMO_US
/** Charge badge move for Mario. */
EVT_BEGIN(marioAttackEvent_NormalCharge)
    SET_VALUE(LW(12), PTR(&badgeWeapon_Charge))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()
#endif

/** Charge P badge move for the partner. */
#if VERSION > VERSION_DEMO_US
/** Charge P badge move for the partner. */
EVT_BEGIN(marioAttackEvent_NormalChargeP)
    SET_VALUE(LW(12), PTR(&badgeWeapon_ChargeP))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()
#endif

/** Super Charge badge move for Mario. */
EVT_BEGIN(marioAttackEvent_SuperCharge)
    SET_VALUE(LW(12), PTR(&badgeWeapon_SuperCharge))
    CALL_CHILD(PTR(_SuperCharge_core))
    RETURN
EVT_END()

/** Super Charge P badge move for the partner. */
EVT_BEGIN(marioAttackEvent_SuperChargeP)
    SET_VALUE(LW(12), PTR(&badgeWeapon_SuperChargeP))
    CALL_CHILD(PTR(_SuperCharge_core))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
static const Vec majinai_effect_pos = {0.0f, 0.0f, 0.0f};
#else
static const Vec majinai_effect_pos = {0.0f, 10.0f, 0.0f};
#endif

/**
 * Run at the start of Mario's attacks: if Merlee's next charm is the attack boost (merleeNextCurseType 1),
 * uses it up with the Merlee sparkle, a hop and the "power up" banner.
 * JP and earlier show the banner for 60 frames instead of the US style 180-frame banner that A or B closes.
 */
EVT_BEGIN(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(_majinai_powerup_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
#if VERSION >= VERSION_US
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_power_up"), 180)
#else
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_power_up"), 60)
#endif
    WAIT_FRAMES(60)
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_MajinaiJump)
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_JUMP)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_DAMAGE_STATUS)
    RETURN
EVT_END()

/** Puts Mario back in his idle stance after a Merlee charm, if he can act. */
EVT_BEGIN(_MajinaiPoseReset)
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Merlee's defense charm: sparkle, hop and the "defense up" banner; started from battle_seq.c. */
EVT_BEGIN(marioAttackEvent_MajinaiDefenceUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
#if VERSION >= VERSION_US
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_defence_up"), 180)
#else
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_defence_up"), 60)
#endif
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(_MajinaiPoseReset))
    RETURN
EVT_END()

/**
 * Merlee's Star Point charm: sparkle, hop and the "experience up" banner;
 * started from battle_seq_end.c after a win.
 */
EVT_BEGIN(marioAttackEvent_MajinaiExpUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
#if VERSION >= VERSION_US
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_exp_up"), 180)
#else
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_exp_up"), 60)
#endif
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/** Puts Mario's hammer level (pouch hammerLevel; 3 is the Ultra Hammer) in args[0]. */
static USER_FUNC(_get_mario_hammer_lv) {
    s32 ret = *event->args;
    evtSetValue(event, ret, pouchGetPtr()->hammerLevel);
    return EVT_RETURN_DONE;
}

/** Spawns the N64-style whirlwind effect around unit args[0] for args[1] frames; used by the spinning hammer attack. */
USER_FUNC(_whirlwind_effect) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 life = evtGetValue(event, args[1]);
    BattleUnit* unit;
    EffectEntry* eff;
    f32 pos[3];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    eff = effWhirlwindN64Entry(unit->faceDirection < 0, NULL, unit->sizeMultiplier, life);
    ((EffWhirlwindN64Work*)eff->userdata)->position.x = pos[0];
    ((EffWhirlwindN64Work*)eff->userdata)->position.y = pos[1];
    ((EffWhirlwindN64Work*)eff->userdata)->position.z = pos[2];
    return EVT_RETURN_DONE;
}

/**
 * Over 25 frames, emits growing energy stars (effect type args[1]) rising behind unit args[0];
 * the charge-up sparkle of the jump attacks.
 */
USER_FUNC(_jump_star_effect) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    s32 step;
    f32 rot;
    f32 dx;
    f32 size;
    f32 pos[3];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    step = event->userdata[0] / 4;
    if (event->userdata[0] % 4 == 0) {
        if (unit->faceDirection >= 0) {
            rot = 10.0f;
            dx = -10.0f;
        } else {
            rot = -10.0f;
            dx = 10.0f;
        }
        BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
        pos[0] += dx * unit->sizeMultiplier;
        pos[1] += (f32)(step * 8 / 10);
        size = 0.1f * step + 0.8f;
        if (size > 2.0f) {
            size = 2.0f;
        }
        effEnergyEntry(type, 50, pos[0], pos[1], pos[0], size, rot, 0.05f);
    }
    if (event->userdata[0] <= 24) {
        event->userdata[0]++;
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Sets args[0] to 1 while the stage's overhead iron frame is still there, 0 once it is gone; Spring Jump checks it. */
USER_FUNC(_bgset_iron_frame_check) {
    s32 ret = *event->args;
    if (_battleWorkPointer->stageObjectWork.flags & STAGE_FLAG_NO_IRON_FRAME) {
        evtSetValue(event, ret, 0);
    } else {
        evtSetValue(event, ret, 1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Blocks until the top of unit args[0] reaches height 130 (the iron frame), then pins it there;
 * the Spring Jump collision with the frame.
 */
USER_FUNC(_wait_jyabara_hit_iron_frame) {
    BattleUnit* unit;
    f32 pos[3];
    s32 id = evtGetValue(event, *event->args);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    if (pos[1] + BtlUnit_GetHeight(unit) < 130.0f) {
        return EVT_RETURN_BLOCK;
    }
    BtlUnit_SetPos(unit, pos[0], 130 - BtlUnit_GetHeight(unit), pos[2]);
    return EVT_RETURN_DONE;
}

/**
 * Tornado Jump's twister: two Kammy-style tornado effects spiralling out to 80 units from unit args[0] over 90
 * frames, with the tornado sound following them.
 */
USER_FUNC(_tatsumaki_effect) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* ud = event->userWork;
    BattleUnit* unit;
    s32 id = evtGetValue(event, *event->args);
    f32 r;
    f32 dx;
    f32 ang;
    f32 dz;
    Vec v;
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (isFirstCall) {
        ud[0] = 0;
    }
    switch (ud[0]) {
    case 0:
        ud[0]++;
        ud[1] = 0;
        ud[2] = (s32)unit->position.x;
        ud[3] = (s32)unit->position.y - 40;
        ud[4] = (s32)unit->position.z;
        ud[5] = (EvtWord)effKamekiTornadeN64Entry(0, 90, ud[2], ud[3], ud[4], 3.0f);
        ud[6] = (EvtWord)effKamekiTornadeN64Entry(0, 90, ud[2], ud[3], ud[4], 3.0f);
        ud[7] = psndSFXOn("SFX_BTL_JUMP_TATUMAKI2");
        v.x = ud[2];
        v.y = ud[3];
        v.z = ud[4];
        psndSFX_pos(ud[7], &v);
        // fall through
    case 1:
        ud[1]++;
        r = intplGetValue(EASING_CUBIC_OUT, 0.0f, 80.0f, ud[1], 90);
        ang = ud[1] * 4;
        dx = r * cosfd(ang);
        dz = r * sinfd(ang);
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[5])->userdata)->position.x = ud[2] + dx;
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[5])->userdata)->position.y = ud[3];
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[5])->userdata)->position.z = ud[4] + dz;
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[6])->userdata)->position.x = ud[2] - dx;
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[6])->userdata)->position.y = ud[3];
        ((EffKamekiTornadeN64Work*)((EffectEntry*)ud[6])->userdata)->position.z = ud[4] - dz;
        if (ud[1] >= 90) {
            psndSFXOff(ud[7]);
            return EVT_RETURN_DONE;
        }
        break;
    default:
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Records a Power Bounce combo of args[0]: keeps this battle's best (capped at 100) and, on a new pouch record,
 * sets powerBounceRecord and makes the audience react.
 */
USER_FUNC(_record_renzoku_count) {
    s32 n = evtGetValue(event, *event->args);
    BattleActRecord* rec = &_battleWorkPointer->actRecord;
    PouchData* pouch = pouchGetPtr();
    if (n > rec->powerBounceCombo) {
        if (n > pouch->powerBounceRecord) {
            pouch->powerBounceRecord = n;
            BattleAudience_Case_JumpNewRecord();
        }
        if (n > 100) {
            n = 100;
        }
        rec->powerBounceCombo = n;
    }
    return EVT_RETURN_DONE;
}

/**
 * Power Bounce limit for target args[0] after args[1] bounces, into args[2]:
 * below the target's pbSoftCap, the cap; from the cap up to twice it,
 * one more bounce with a 33% chance each time; at twice the cap, no more. Also used by Goombella's Multibonk.
 */
USER_FUNC(mario_get_renzoku_count_max) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 count = evtGetValue(event, args[1]);
    s32 ret = args[2];
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 max;
    if (unit == NULL) {
        evtSetValue(event, ret, 0);
        return EVT_RETURN_DONE;
    }
    max = unit->kindParams->pbSoftCap;
    if (count < max) {
        evtSetValue(event, ret, max);
        return EVT_RETURN_DONE;
    }
    if (count >= max * 2) {
        evtSetValue(event, ret, count);
        return EVT_RETURN_DONE;
    }
    if (irand(100) < 33) {
        evtSetValue(event, ret, count + 1);
    } else {
        evtSetValue(event, ret, count);
    }
    return EVT_RETURN_DONE;
}

/**
 * Over 12 frames, emits energy stars (effect type args[1]) in an arc around the hammer head of unit args[0];
 * the charge sparkle of the hammer attacks.
 */
USER_FUNC(_hammer_star_effect) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    f32 angle;
    f32 rot;
    f32 pos[3];
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    if (unit->faceDirection >= 0) {
        angle = intplGetValue(EASING_QUADRATIC_IN, 210.0f, 420.0f, event->userdata[0], 12);
        rot = -10.0f;
    } else {
        angle = intplGetValue(EASING_QUADRATIC_IN, 150.0f, -60.0f, event->userdata[0], 12);
        rot = 10.0f;
    }
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    pos[1] += unit->sizeMultiplier * (0.85f * BtlUnit_GetHeight(unit));
    pos[0] += unit->sizeMultiplier * (26.0f * -sinfd(angle));
    pos[1] += unit->sizeMultiplier * (26.0f * cosfd(angle));
    effEnergyEntry(type, 30, pos[0], pos[1], pos[0], 0.08f * event->userdata[0] + 0.1f, rot, 0.05f);
    if (event->userdata[0] < 12) {
        event->userdata[0]++;
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Spins unit args[0] one full turn and a bit over args[2] frames,
 * switching part args[1] to animation args[3] once it is three-quarters round;
 * the acrobatic spin of the spinning hammer.
 */
USER_FUNC(_kaiten_hammer_acrobat_rotate) {
    BattleUnit* unit;
    BattleUnitPart* part;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 n;
    const char* anim;
    f32 rx, ry, rz;
    s32 id = evtGetValue(event, args[0]);
    s32 pn;
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    pn = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(unit, pn);
    n = evtGetValue(event, args[2]);
    anim = (const char*)evtGetValue(event, args[3]);
    if (isFirstCall) {
        BtlUnit_GetRotate(unit, &rx, &ry, &rz);
        event->userdata[0] = n;
        if (unit->faceDirection >= 0) {
            event->userdata[1] = 360.0f + (360.0f - reviseAngle(ry));
        } else {
            event->userdata[1] = 360.0f + reviseAngle(ry);
        }
        event->userdata[2] = unit->faceDirection * event->userdata[1] / n;
        event->userdata3 = 0;
    }
    BtlUnit_AddRotate(unit, 0, event->userdata[2], 0);
    event->userdata[0]--;
    // FAKE: (s32) cast on the u32 userdata3; the plain compare with 0 changes the object
    if ((s32)event->userdata3 == 0) {
        event->userdata[1] -= event->userdata[2];
        if (event->userdata[1] <= 270) {
            event->userdata3 = 1;
            BtlUnit_SetAnim(part, anim);
        }
    }
    if (event->userdata[0] > 0) {
        return EVT_RETURN_BLOCK;
    }
    BtlUnit_SetRotate(unit, 0, 0, 0);
    return EVT_RETURN_DONE;
}

/**
 * Keeps a fire effect on Mario's part args[1] (the Fire Drive fireball) until the part's work[0] is cleared,
 * then deletes it.
 */
USER_FUNC(_mario_fire_ball_controll) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 partId = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, partId);
    EffectEntry* eff;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = (EvtWord)effFireEntry(5, 1000, 0.0f, 0.0f, 0.0f, battle_mario_one);
    }
    eff = (EffectEntry*)event->userdata[1];
    if (part->work[MARIO_PARTS_WORK_FIRE_BALL] == 0) {
        effDelete(eff);
        return EVT_RETURN_DONE;
    }
    {
        EffFireWork* work = (EffFireWork*)eff->userdata;
        BtlUnit_GetPartsPos(part, &work->position.x, &work->position.y, &work->position.z);
    }
    return EVT_RETURN_BLOCK;
}

/** Spawns a fire pillar at args[0..2] scaled by 0.6 * args[3]; Fire Drive's burn on each target. */
USER_FUNC(_fire_wave) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    f32 scale = 0.6f * evtGetValue(event, args[3]);
    if (isFirstCall) {
        event->userdata[0] = (EvtWord)effHibashiraEntry(0, 1, 90, x, y, z, x, y, z, scale);
    }
    return EVT_RETURN_DONE;
}

/**
 * Sets args[0] to 1 and uses up the charm if Merlee's next charm is the attack boost (merleeNextCurseType 1),
 * else 0.
 */
USER_FUNC(_majinai_powerup_check) {
    BattleWork* wp = _battleWorkPointer;
    s32 ret = *event->args;
    if (pouchGetPtr()->merleeNextCurseType != 1) {
        evtSetValue(event, ret, 0);
    } else {
        evtSetValue(event, ret, 1);
        wp->majinaiState = 1;
        BattleMajinaiDone();
        BattleMajinaiEndCheck();
    }
    return EVT_RETURN_DONE;
}

/** Plays Merlee's charm sparkle (mjef_coinup) and blocks until it finishes. */
USER_FUNC(_battle_majinai_effect) {
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    if (isFirstCall) {
        Vec v = majinai_effect_pos;
        event->userdata[0] = (EvtWord)effMajinaiEntry(0, v.x, v.y, v.z);
        effSetName((EffectEntry*)event->userdata[0], "mjef_coinup");
    }
    if (event->userdata[0] && effNameToPtr("mjef_coinup")) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Turns off paper lighting on part args[1] of unit args[0]; used by the spinning hammer attack. */
USER_FUNC(_paper_light_off) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 part;
    id = BattleTransID(event, id);
    part = evtGetValue(event, args[1]);
    animPoseSetMaterialLightFlagOff(BattleGetUnitPartsPtr(id, part)->poseId, MATERIAL_LIGHT_PAPER);
    return EVT_RETURN_DONE;
}

/** The 1.0 scale passed to effFireEntry for the flames on Fire Drive's fireball (_mario_fire_ball_controll). */
const f32 battle_mario_one = 1.0f;
