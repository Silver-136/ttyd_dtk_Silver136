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

EffectEntry* effFireEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 duration);
void BtlUnit_SetAnim(BattleUnitPart* part, s32 anim);

#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"


USER_FUNC(_battle_majinai_effect);
USER_FUNC(_majinai_powerup_check);
USER_FUNC(btlevtcmd_ACSuccessEffect);
USER_FUNC(btlevtcmd_AddPartsRotate);
USER_FUNC(btlevtcmd_AddRotate);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnnounceMessage);
USER_FUNC(btlevtcmd_AudienceDeclareACResult);
USER_FUNC(btlevtcmd_CheckActStatus);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CommandCheckDamage);
USER_FUNC(btlevtcmd_CommandGetWeaponAddress);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetDamage);
USER_FUNC(btlevtcmd_GetHeight);
USER_FUNC(btlevtcmd_GetHitPos);
USER_FUNC(btlevtcmd_GetPartsPos);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetResultAC);
USER_FUNC(btlevtcmd_GetResultPrizeLv);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_JumpPartsContinue);
USER_FUNC(btlevtcmd_JumpPartsPosition);
USER_FUNC(btlevtcmd_JumpPartsSetting);
USER_FUNC(btlevtcmd_JumpPosition);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_OnPartsAttribute);
USER_FUNC(btlevtcmd_PayWeaponCost);
USER_FUNC(btlevtcmd_RunDataEventChild);
USER_FUNC(btlevtcmd_SetFallAccel);
USER_FUNC(btlevtcmd_SetPartsPos);
USER_FUNC(btlevtcmd_SetPartsRotate);
USER_FUNC(btlevtcmd_SetPartsRotateOffset);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(btlevtcmd_StartAvoid);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StopAC);
USER_FUNC(btlevtcmd_WeaponAftereffect);
USER_FUNC(evt_btl_camera_off_posoffset_manual);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_btl_camera_shake_h);
USER_FUNC(evt_eff64);
USER_FUNC(evt_snd_sfxon);
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
EvtWord marioAttackEvent_JyabaraJump[VERSION_PICK(1675, 1696)];
EvtWord marioAttackEvent_GatsuDokaJump[VERSION_PICK(624, 638)];
EvtWord marioAttackEvent_TatsumakiJump[VERSION_PICK(1668, 1679)];
EvtWord marioAttackEvent_RenzokuJump[840];
EvtWord marioAttackEvent_TugiTugiJump[VERSION_PICK(793, 775)];
EvtWord marioAttackEvent_NormalHammer_Core[VERSION_PICK(1326, 1333)];
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

#ifdef VERSION_G8ME01
BattleWeapon marioDefaultWeapon_Jump = {
    "btl_wn_mario_normal_jump", // name
    167, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x35807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 4, 2, 1, 1, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioDefaultWeapon_Jump = {
    "btl_wn_mario_normal_jump", // name
    159, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x35807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 2, 2, 1, 1, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_KururinJump = {
    "btl_wn_mario_kururin_jump", // name
    169, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 4, 5, 1, 1, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_KururinJump = {
    "btl_wn_mario_kururin_jump", // name
    161, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 2, 5, 1, 1, 0, 0, 0}, // stageEffectRates
};
#endif


#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_KururinJump2 = {
    "btl_wn_mario_kururin_jump", // name
    169, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 2, 3, 4, 5, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // mAttackEvtScript
    {0, 5, 5, 100, 0, 20, 10, 5, 5, 5, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_KururinJump2 = {
    "btl_wn_mario_kururin_jump", // name
    161, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kururin_jump", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_k_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KururinJump, // mAttackEvtScript
    {0, 5, 5, 100, 0, 10, 10, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_JyabaraJump = {
    "btl_wn_mario_jyabara_jump", // name
    168, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 2, 2, 4, 5, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_j_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // mAttackEvtScript
    {0, 5, 5, 100, 0, 20, 10, 5, 5, 5, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_JyabaraJump = {
    "btl_wn_mario_jyabara_jump", // name
    160, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    2, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_j_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // mAttackEvtScript
    {0, 5, 5, 100, 0, 10, 10, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_JyabaraJumpFailed = {
    "btl_wn_mario_jyabara_jump", // name
    168, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    1, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // mAttackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 50, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_JyabaraJumpFailed = {
    "btl_wn_mario_jyabara_jump", // name
    160, // icon
    {0}, // pad_06
    0, // itemId
    "msg_jyabara_jump", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    2, // mStylishCommandMultiplier
    5, // reserved_19
    1, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JyabaraJump, // mAttackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 50, 0, 0, 0, 0}, // stageEffectRates
};
#endif

BattleWeapon marioDefaultWeapon_FirstAttackKaitenHammer = {
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
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    14, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20887, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FirstAttackKaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon marioDefaultWeapon_FirstAttackUltraHammer = {
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
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    16, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20887, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FirstAttackKaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon marioDefaultWeapon_Hammer = {
    "btl_wn_mario_normal_hammer", // name
    164, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x30887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalHammer, // mAttackEvtScript
    {0, 3, 3, 100, 3, 6, 2, 3, 3, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioDefaultWeapon_Hammer = {
    "btl_wn_mario_normal_hammer", // name
    156, // icon
    {0}, // pad_06
    0, // itemId
    "msg_normal_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x30887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalHammer, // mAttackEvtScript
    {0, 3, 3, 100, 3, 3, 2, 3, 3, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_KaitenHammer = {
    "btl_wn_mario_kaiten_hammer", // name
    165, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kaiten_hammer", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    14, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kaiten_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 6, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_KaitenHammer = {
    "btl_wn_mario_kaiten_hammer", // name
    157, // icon
    {0}, // pad_06
    0, // itemId
    "msg_kaiten_hammer", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    14, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kaiten_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 3, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20102000, // targetPropertyFlags
    0, // element
    15, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20040, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_UltraHammer = {
    "btl_wn_mario_ultra_hammer", // name
    166, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    16, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_ultra_hammer", // acHelpMsg
    0x20807, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 6, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_UltraHammer = {
    "btl_wn_mario_ultra_hammer", // name
    158, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    2, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, // element
    16, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_ultra_hammer", // acHelpMsg
    0x20807, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 3, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20102000, // targetPropertyFlags
    0, // element
    17, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20040, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_UltraHammerFinish = {
    "btl_wn_mario_ultra_hammer", // name
    166, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, // element
    19, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x2088F, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_UltraHammerFinish = {
    "btl_wn_mario_ultra_hammer", // name
    158, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ultra_hammer", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    2, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, // element
    19, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x2088F, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_KaitenHammer, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_TugitugiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    241, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 241, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TugiTugiJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 10, 5, 2, 2, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_TugitugiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    241, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 241, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TugiTugiJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 2, 2, 0, 0, 0}, // stageEffectRates
};
#endif

BattleWeapon badgeWeapon_GatsunJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    240, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 240, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunJump, // mAttackEvtScript
    {0, 25, 25, 100, 25, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};

BattleWeapon badgeWeapon_DokanJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    332, // itemId
    NULL, // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 332, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_DokanJump, // mAttackEvtScript
    {0, 25, 25, 100, 25, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_RenzokuJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    242, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 242, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25817, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_RenzokuJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 10, 5, 2, 2, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_RenzokuJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    242, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 0, 1, 1, 2, 2, 242, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25817, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_RenzokuJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 2, 2, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_TatsumakiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    243, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 1, 1, 3, 2, 5, 243, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_tatsumaki_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TatsumakiJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_TatsumakiJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    243, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    3, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapJump1, // hpDamageFn
    {0, 1, 1, 3, 2, 5, 243, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_tatsumaki_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TatsumakiJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

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
    0x20000010, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20040, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_MiniminiFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    244, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_MiniminiFumi, // mAttackEvtScript
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
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
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_MiniminiFumi, // mAttackEvtScript
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_NemuraseFumi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    245, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    100, // sleepChance
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NemuraseFumi, // mAttackEvtScript
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
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
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
    100, // sleepChance
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NemuraseFumi, // mAttackEvtScript
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_FunyafunyaJump = {
    NULL, 0, // name..icon
    {0}, // pad_06
    246, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_FunyafunyaJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 6, 5, 1, 1, 0, 0, 0}, // stageEffectRates
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
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioJumpLv, // hpDamageFn
    {1, 1, 2, 2, 3, 3, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_jump", // acHelpMsg
    0x25807, // special
    0xC, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_FunyafunyaJump, // mAttackEvtScript
    {0, 0, 0, 100, 0, 3, 5, 1, 1, 0, 0, 0}, // stageEffectRates
};
#endif

BattleWeapon badgeWeapon_GatsunHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    247, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 247, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunHammer, // mAttackEvtScript
    {0, 25, 25, 100, 25, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};

BattleWeapon badgeWeapon_DokanHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    333, // itemId
    NULL, // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 333, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_GatsunHammer, // mAttackEvtScript
    {0, 25, 25, 100, 25, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_JishinHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    248, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 248, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000008, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x238C7, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JishinHammer, // mAttackEvtScript
    {0, 50, 50, 100, 50, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_JishinHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    248, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    2, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 248, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000008, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x238C7, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_JishinHammer, // mAttackEvtScript
    {0, 50, 50, 100, 50, 25, 25, 5, 25, 5, 0, 0}, // stageEffectRates
};
#endif

BattleWeapon badgeWeapon_UltraJishinHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    334, // itemId
    "msg_ultra_attack", // description
    100, // baseAccuracy
    7, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {3, 6, 3, 6, 3, 6, 334, 4}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000008, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_ultra_jishin_hammer", // acHelpMsg
    0x238C7, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_UltraJishinHammer, // mAttackEvtScript
    {0, 50, 50, 100, 50, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

BattleWeapon badgeWeapon_HammerNageru = {
    NULL, 0, // name..icon
    {0}, // pad_06
    249, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0, 1, 1, 3, 2, 5, 249, 1}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer_nageru", // acHelpMsg
    0x20887, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_HammerNageru, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_TsuranukiNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    250, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x208C7, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TsuranukiHammer, // mAttackEvtScript
    {0, 25, 25, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_TsuranukiNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    250, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x208C7, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_TsuranukiHammer, // mAttackEvtScript
    {0, 25, 25, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_ConfuseHammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    251, // itemId
    NULL, // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_NormalHammer, // mAttackEvtScript
    {0, 5, 5, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
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
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20887, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_NormalHammer, // mAttackEvtScript
    {0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_FireNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100, // baseAccuracy
    5, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 3, 0, 3, 1, 3, 252, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20002000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_fire_naguri", // acHelpMsg
    0x20867, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // mAttackEvtScript
    {0, 0, 0, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
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
    2, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerOverlapHammer1, // hpDamageFn
    {0xFFFFFFFF, 3, 0, 3, 1, 3, 252, 2}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20002000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_fire_naguri", // acHelpMsg
    0x20867, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // mAttackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_FireNaguriFailed = {
    NULL, 0, // name..icon
    {0}, // pad_06
    252, // itemId
    NULL, // description
    100, // baseAccuracy
    5, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    1, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 5, 2, 5, 3, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x10002000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20007, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // mAttackEvtScript
    {0, 0, 0, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
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
    2, // mStylishCommandMultiplier
    5, // reserved_19
    1, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 5, 2, 5, 3, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x10002000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20007, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_FireNaguri, // mAttackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_IceNaguri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    253, // itemId
    NULL, // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    2, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20807, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_IceNaguri, // mAttackEvtScript
    {0, 5, 5, 100, 0, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
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
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromMarioHammerLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    2, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    0x20807, // special
    0x7FB, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    marioAttackEvent_IceNaguri, // mAttackEvtScript
    {0, 5, 5, 100, 0, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_Charge = {
    "btl_wn_mario_normal_charge", // name
    167, // icon
    {0}, // pad_06
    256, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalCharge, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_Charge = {
    "btl_wn_mario_normal_charge", // name
    159, // icon
    {0}, // pad_06
    256, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalCharge, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_ChargeP = {
    "btl_wn_mario_normal_charge", // name
    167, // icon
    {0}, // pad_06
    257, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalChargeP, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_ChargeP = {
    "btl_wn_mario_normal_charge", // name
    159, // icon
    {0}, // pad_06
    257, // itemId
    "btl_hlp_cmd_operation_charge", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_NormalChargeP, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_SuperCharge = {
    "btl_wn_mario_super_charge", // name
    167, // icon
    {0}, // pad_06
    337, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperCharge, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_SuperCharge = {
    "btl_wn_mario_super_charge", // name
    159, // icon
    {0}, // pad_06
    337, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperCharge, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon badgeWeapon_SuperChargeP = {
    "btl_wn_mario_super_charge", // name
    167, // icon
    {0}, // pad_06
    338, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperChargeP, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon badgeWeapon_SuperChargeP = {
    "btl_wn_mario_super_charge", // name
    159, // icon
    {0}, // pad_06
    338, // itemId
    "btl_hlp_cmd_operation_super_charge", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_SuperChargeP, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
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

BattleWeapon badgeWeapon_3KaiItem = {
    "btl_wn_3kai_item", // name
    0, // icon
    {0}, // pad_06
    329, // itemId
    NULL, // description
    100, // baseAccuracy
    8, // baseFpCost
    0, 0, 0.0f, 0, 0, 0, 0, NULL, // baseSpCost..hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20108, // special
    0x7FF, // counterResistFlags
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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

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
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_BakuGame = {
    "btl_wn_sac_bakugame", // name
    416, // icon
    {0}, // pad_06
    0, // itemId
    "msg_bakugame", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_3", // acHelpMsg
    0x20088, // special
    0x7FF, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    100, // stopChance
    1, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_BakuGame, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_BakuGame = {
    "btl_wn_sac_bakugame", // name
    408, // icon
    {0}, // pad_06
    0, // itemId
    "msg_bakugame", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_3", // acHelpMsg
    0x20088, // special
    0x7FF, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    100, // stopChance
    1, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_BakuGame, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Scissor = {
    "btl_wn_sac_scissor", // name
    420, // icon
    {0}, // pad_06
    0, // itemId
    "msg_scissor", // description
    100, // baseAccuracy
    0, // baseFpCost
    4, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_Scissor, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_5", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Scissor, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Scissor = {
    "btl_wn_sac_scissor", // name
    412, // icon
    {0}, // pad_06
    0, // itemId
    "msg_scissor", // description
    100, // baseAccuracy
    0, // baseFpCost
    4, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_Scissor, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_5", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Scissor, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_ZubaStar = {
    "btl_wn_sac_zubastar", // name
    419, // icon
    {0}, // pad_06
    0, // itemId
    "msg_zubastar", // description
    100, // baseAccuracy
    0, // baseFpCost
    6, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_ZubaStar, // hpDamageFn
    {0, 3, 6, 9, 12, 15, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_8", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_ZubaStar, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_ZubaStar = {
    "btl_wn_sac_zubastar", // name
    411, // icon
    {0}, // pad_06
    0, // itemId
    "msg_zubastar", // description
    100, // baseAccuracy
    0, // baseFpCost
    6, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_ZubaStar, // hpDamageFn
    {0, 3, 6, 9, 12, 15, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_8", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_ZubaStar, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Genki0 = {
    "btl_wn_sac_genki0", // name
    313, // icon
    {0}, // pad_06
    0, // itemId
    "msg_genkigenki", // description
    100, // baseAccuracy
    0, // baseFpCost
    1, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_1", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki0, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Genki0 = {
    "btl_wn_sac_genki0", // name
    305, // icon
    {0}, // pad_06
    0, // itemId
    "msg_genkigenki", // description
    100, // baseAccuracy
    0, // baseFpCost
    1, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_1", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki0, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Genki1 = {
    "btl_wn_sac_genki1", // name
    421, // icon
    {0}, // pad_06
    0, // itemId
    "msg_super_genki", // description
    100, // baseAccuracy
    0, // baseFpCost
    5, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_6", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki1, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Genki1 = {
    "btl_wn_sac_genki1", // name
    413, // icon
    {0}, // pad_06
    0, // itemId
    "msg_super_genki", // description
    100, // baseAccuracy
    0, // baseFpCost
    5, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_6", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Genki1, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Deka = {
    "btl_wn_sac_dekkaku_dokkan", // name
    415, // icon
    {0}, // pad_06
    0, // itemId
    "msg_dekkaku_dokkan", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_Deka, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_2", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Deka, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Deka = {
    "btl_wn_sac_dekkaku_dokkan", // name
    407, // icon
    {0}, // pad_06
    0, // itemId
    "msg_dekkaku_dokkan", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_Deka, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_2", // acHelpMsg
    0x200C8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Deka, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Muki = {
    "btl_wn_sac_mukimuki_body", // name
    418, // icon
    {0}, // pad_06
    0, // itemId
    "msg_mukimuki_body", // description
    100, // baseAccuracy
    0, // baseFpCost
    3, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_4", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Muki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Muki = {
    "btl_wn_sac_mukimuki_body", // name
    410, // icon
    {0}, // pad_06
    0, // itemId
    "msg_mukimuki_body", // description
    100, // baseAccuracy
    0, // baseFpCost
    3, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_4", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Muki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

#ifdef VERSION_G8ME01
BattleWeapon marioWeapon_Suki = {
    "btl_wn_sac_sukkari_sukkiri", // name
    417, // icon
    {0}, // pad_06
    0, // itemId
    "msg_sukkari_sukkiri", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_7", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, // atkChangeTime..allergicTime
    100, // ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Suki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#else
BattleWeapon marioWeapon_Suki = {
    "btl_wn_sac_sukkari_sukkiri", // name
    409, // icon
    {0}, // pad_06
    0, // itemId
    "msg_sukkari_sukkiri", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_7", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, // atkChangeTime..allergicTime
    100, // ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Suki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
#endif

BattleWeapon* superActionTable[] = {
    &marioWeapon_Genki0, &marioWeapon_Deka, &marioWeapon_BakuGame, &marioWeapon_Muki, &marioWeapon_Scissor, &marioWeapon_Genki1,
    &marioWeapon_Suki, &marioWeapon_ZubaStar, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
};

#ifdef VERSION_G8ME01
static EVT_BEGIN(marioAttackEvent_NormalJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Jump))
        CALL(btlevtcmd_ACHelpSet, PTR(&marioDefaultWeapon_Jump))
        CALL(evt_btl_camera_set_mode, 0, 10)
        CALL(evt_btl_camera_set_homing_unitparts, 0, LW(3), LW(4), -2, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    ELSE
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
        CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(5))
            CALL(btlevtcmd_AcSetDifficulty, -2, LW(5))
            CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
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
                CALL(_jump_star_effect, -2, LW(13))
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CASE_END
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
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
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
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
            CALL(btlevtcmd_ACRStart, -2, LW(1), LW(2), LW(0), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 3, -1)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_snd_voice, -2, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS_OR_EQUAL(LW(6), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        WAIT_FRAMES(2)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        WAIT_FRAMES(2)
    END_IF
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
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
        CALL(btlevtcmd_ACRStart, -2, LW(1), LW(2), LW(0), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
            CALL(btlevtcmd_snd_voice, -2, 1)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), LW(6), -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
static EVT_BEGIN(marioAttackEvent_NormalJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Jump))
        CALL(btlevtcmd_ACHelpSet, PTR(&marioDefaultWeapon_Jump))
        CALL(evt_btl_camera_set_mode, 0, 10)
        CALL(evt_btl_camera_set_homing_unitparts, 0, LW(3), LW(4), -2, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    ELSE
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
        CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(5))
            CALL(btlevtcmd_AcSetDifficulty, -2, LW(5))
            CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
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
                CALL(_jump_star_effect, -2, LW(13))
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CASE_END
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
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
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            SET_VALUE(LW(0), LW(6))
            DIVIDE(LW(0), 2)
            SUBTRACT(LW(0), 3)
            SET_VALUE(LW(1), LW(0))
            SET_VALUE(LW(2), LW(0))
            SUBTRACT(LW(1), 3)
            ADD(LW(2), 3)
            SET_VALUE(LW(0), LW(6))
            SUBTRACT(LW(0), 4)
            CALL(btlevtcmd_ACRStart, -2, LW(1), LW(2), LW(0), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 3, -1)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_snd_voice, -2, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS_OR_EQUAL(LW(6), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        WAIT_FRAMES(2)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        WAIT_FRAMES(2)
    END_IF
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(marioAttackEvent_KururinJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioWeapon_KururinJump))
        CALL(btlevtcmd_ACHelpSet, PTR(&marioWeapon_KururinJump))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 10)
        CALL(evt_btl_camera_set_homing_unitparts, 0, LW(3), LW(4), -2, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(5))
            CALL(btlevtcmd_AcSetDifficulty, -2, LW(5))
            CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 250)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
        CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
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
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(0), LW(6))
            SET_VALUE(LW(1), LW(6))
            DIVIDE(LW(0), 2)
            SUBTRACT(LW(1), 1)
            CALL(btlevtcmd_ACRStart, -2, 5, LW(0), LW(1), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(btlevtcmd_snd_voice, -2, 1)
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CALL(btlevtcmd_SetRotateOffset, -2, 0, 14, 0)
                LOOP(15)
                    CALL(btlevtcmd_AddRotate, -2, 0, 0, -24)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
        IF_INT_NOT_EQUAL(LW(5), 1)
            IF_INT_EQUAL(LW(5), 3)
                CALL(btlevtcmd_StartAvoid, LW(3), 38)
            END_IF
            IF_INT_EQUAL(LW(5), 6)
                CALL(btlevtcmd_StartAvoid, LW(3), 39)
            END_IF
            IF_INT_EQUAL(LW(5), 2)
                CALL(btlevtcmd_StartAvoid, LW(3), 40)
            END_IF
            CALL(btlevtcmd_StopAC)
            CALL(btlevtcmd_JumpContinue, -2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
            WAIT_FRAMES(40)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
        CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
        CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
        DIVIDE(LW(0), 2)
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
        CALL(btlevtcmd_ResultAC)
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_CLEAR(LW(6), 2)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_StopAC)
            CALL(evt_audience_ap_recovery)
            CALL(btlevtcmd_InviteApInfoReport)
            CALL(btlevtcmd_WeaponAftereffect, LW(12))
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
            CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
            CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
            CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
            CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
            CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
            GOTO(95)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
            CALL(evt_btl_camera_set_zoom, 0, 200)
            WAIT_FRAMES(10)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_set_zoom, 0, 250)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
        ADD(LW(10), 1)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    GOTO(55)
    LABEL(52)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_N_7"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_FallPosition, -2, LW(0), LW(1), LW(2), 14)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
    WAIT_FRAMES(28)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_U_3"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    LABEL(55)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1A"))
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_KURURIN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1B"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 1)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&marioWeapon_KururinJump2), 131328, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, PTR(&marioWeapon_KururinJump2), LW(6))
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&marioWeapon_KururinJump2), 256, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_AudienceDeclareACResult, PTR(&marioWeapon_KururinJump2), LW(6))
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    END_IF
    GOTO(95)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_JyabaraJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    WAIT_FRAMES(21)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, STRING("p_jyabara"), 1)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1E"))
    WAIT_FRAMES(16)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, STRING("p_jyabara"), 0)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1B"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_2A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_2B"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_3A"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_3B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_AC3_5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_bgset_iron_frame_check, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1C"))
        BEGIN_INLINE_EVENT_GET_ID(LW(14))
            LOOP(50)
                CALL(btlevtcmd_AddRotate, -2, 0, 48, 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        BEGIN_INLINE_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 90, -2)
        END_INLINE_EVENT
        CALL(_wait_jyabara_hit_iron_frame, -2)
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_WeaponAftereffect, PTR(&marioWeapon_JyabaraJumpFailed))
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        BEGIN_SIBLING_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
            LOOP(5)
                WAIT_FRAMES(1)
                SUBTRACT_FLOAT(LW(0), FLOAT(0.19921875f))
                CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), LW(0), FLOAT(1.0f))
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), 130, LW(2), 5, 0, 0, 0, -1)
        WAIT_FRAMES(16)
        CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1H"))
        CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
        CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(5), LW(6))
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(9), EVT_NULLPTR)
            SUBTRACT(LW(9), LW(5))
            DIVIDE(LW(9), 4)
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(6), EVT_NULLPTR)
            SUBTRACT(LW(6), LW(9))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(7), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(7), LW(1))
            ADD(LW(7), LW(9))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(6), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(7), 64)
            LOOP(64)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(8), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 26)
            LOOP(26)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), 60)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 58)
            LOOP(58)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(btlevtcmd_GetHomePos, -2, LW(1), EVT_NULLPTR, EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_FRAMES(130)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1E"))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(evt_sub_intpl_init_float, 5, FLOAT(0.0f), FLOAT(1.1992188f), 10)
            LOOP(10)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init_float, 7, FLOAT(1.1992188f), FLOAT(1.0f), 30)
            LOOP(30)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        WAIT_FRAMES(20)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1C"))
    BEGIN_SIBLING_EVENT
        LOOP(50)
            CALL(btlevtcmd_AddRotate, -2, 0, 48, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1A"))
        CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 80, 255, 255, 255, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, 67108864)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
    CALL(evt_sub_random, 20, LW(5))
    SUBTRACT(LW(5), 10)
    MULTIPLY(LW(5), 6)
    ADD(LW(5), 180)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), LW(5), -2)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_B_2"))
        CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
        CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(10)
            CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 35)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            MULTIPLY(LW(0), -24)
            LOOP(20)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            MULTIPLY(LW(0), -20)
            LOOP(12)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_11"))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 10)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 20, 0, 4, 0, -1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
    END_INLINE_EVENT
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 4)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1A"))
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1B"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 1)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, -2, 8, 17, 29, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_JyabaraJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    WAIT_FRAMES(21)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, STRING("p_jyabara"), 1)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1E"))
    WAIT_FRAMES(16)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, STRING("p_jyabara"), 0)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1B"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_2A"))
    WAIT_FRAMES(8)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_2B"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_3A"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_3B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_AC3_5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_bgset_iron_frame_check, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1C"))
        BEGIN_INLINE_EVENT_GET_ID(LW(14))
            LOOP(50)
                CALL(btlevtcmd_AddRotate, -2, 0, 48, 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        BEGIN_INLINE_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 90, -2)
        END_INLINE_EVENT
        CALL(_wait_jyabara_hit_iron_frame, -2)
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_WeaponAftereffect, PTR(&marioWeapon_JyabaraJumpFailed))
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        BEGIN_SIBLING_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
            LOOP(5)
                WAIT_FRAMES(1)
                SUBTRACT_FLOAT(LW(0), FLOAT(0.19921875f))
                CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), LW(0), FLOAT(1.0f))
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), 130, LW(2), 5, 0, 0, 0, -1)
        WAIT_FRAMES(16)
        CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1H"))
        CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
        CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(5), LW(6))
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(9), EVT_NULLPTR)
            SUBTRACT(LW(9), LW(5))
            DIVIDE(LW(9), 4)
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(6), EVT_NULLPTR)
            SUBTRACT(LW(6), LW(9))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(7), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(7), LW(1))
            ADD(LW(7), LW(9))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(8), LW(1))
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(6), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(7), 64)
            LOOP(64)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 0, LW(0), LW(8), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 26)
            LOOP(26)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), 60)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 58)
            LOOP(58)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(6)
            CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(7))
            MULTIPLY(LW(7), -30)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(1), LW(7))
            CALL(btlevtcmd_GetHomePos, -2, LW(1), EVT_NULLPTR, EVT_NULLPTR)
            CALL(evt_sub_intpl_init, 11, LW(0), LW(1), 32)
            LOOP(32)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_FRAMES(130)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
        CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1E"))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(evt_sub_intpl_init_float, 5, FLOAT(0.0f), FLOAT(1.1992188f), 10)
            LOOP(10)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init_float, 7, FLOAT(1.1992188f), FLOAT(1.0f), 30)
            LOOP(30)
                CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        WAIT_FRAMES(20)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("PM_J_1C"))
    BEGIN_SIBLING_EVENT
        LOOP(50)
            CALL(btlevtcmd_AddRotate, -2, 0, 48, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1A"))
        CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 80, 255, 255, 255, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, 67108864)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
    CALL(evt_sub_random, 20, LW(5))
    SUBTRACT(LW(5), 10)
    MULTIPLY(LW(5), 6)
    ADD(LW(5), 180)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), LW(5), -2)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_B_2"))
        CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
        CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(10)
            CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 35)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            MULTIPLY(LW(0), -24)
            LOOP(20)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            MULTIPLY(LW(0), -20)
            LOOP(12)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_11"))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 10)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 20, 0, 4, 0, -1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
    CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
    END_INLINE_EVENT
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131072, LW(5))
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(6), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1A"))
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_JUMP_JABARA5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_1B"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    ADD(LW(0), 1)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_SET(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, -2, 11, 15, 29, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_GatsuDokaJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), LW(6))
        SET_VALUE(LW(1), LW(6))
        DIVIDE(LW(0), 2)
        SUBTRACT(LW(1), 1)
        CALL(btlevtcmd_ACRStart, -2, 5, LW(0), LW(1), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 14, 0)
            LOOP(15)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, -24)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SET_VALUE(LW(0), 9)
        END_IF
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    WAIT_FRAMES(2)
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_GatsuDokaJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), LW(6))
        SET_VALUE(LW(1), LW(6))
        DIVIDE(LW(0), 2)
        SUBTRACT(LW(1), 1)
        CALL(btlevtcmd_ACRStart, -2, 5, LW(0), LW(1), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 14, 0)
            LOOP(15)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, -24)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        CALL(btlevtcmd_GetDamage, LW(3), LW(0))
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    WAIT_FRAMES(2)
    LABEL(90)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_TatsumakiJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        GOTO(90)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetFriendBelong, LW(3), LW(15))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    WAIT_FRAMES(2)
    CALL(btlevtcmd_StopAC)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 80, 3, -1)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(24)
            CALL(btlevtcmd_AddRotate, -2, 0, 15, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(27)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
    CALL(btlevtcmd_SetupAC, -2, 10, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_SetUnitWork, -3, 5, 0)
    CALL(btlevtcmd_SetUnitWork, -3, 6, 0)
    CALL(btlevtcmd_SetUnitWork, -3, 7, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 30)
        CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
        SET_VALUE(LW(0), 15)
        LABEL(60)
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(61)
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 3, LW(10))
        ADD(LW(0), 1)
        IF_INT_GREATER(LW(0), LW(10))
            SET_VALUE(LW(0), LW(10))
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
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
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 1023)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 1023)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_EQUAL(2)
                    CALL(btlevtcmd_SetUnitWork, -2, 3, 45)
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 2046)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 2046)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_EQUAL(3)
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 3069)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 3069)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_DEFAULT
                    CALL(btlevtcmd_SetUnitWork, -2, 3, 60)
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_JUMP_TATUMAKI1"), LW(11))
    BEGIN_SIBLING_EVENT_GET_ID(LW(12))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(11), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetUnitWork, -3, 5, LW(1))
            CALL(btlevtcmd_GetUnitWork, -3, 6, LW(0))
            IF_INT_NOT_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_GetUnitWork, -3, 7, LW(2))
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
                CALL(btlevtcmd_SetUnitWork, -3, 6, LW(0))
                CALL(evt_snd_sfx_pit, LW(11), LW(0))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        DELETE_EVENT(LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, -3, 5, -2728)
            CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
            SUBTRACT(LW(10), -2728)
            MULTIPLY(LW(10), -1)
            DIVIDE(LW(10), 40)
            CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            LOOP(40)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
                IF_INT_GREATER(LW(0), 10)
                    SUBTRACT(LW(0), 1)
                    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
                END_IF
                CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                IF_INT_LESS(LW(1), 90)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 90)
                        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_N_7"))
                        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_FALL_CRUSH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
        BEGIN_SIBLING_EVENT_GET_ID(LW(13))
            LOOP(60)
                CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
                ADD(LW(0), LW(1))
                IF_INT_GREATER_OR_EQUAL(LW(0), 360)
                    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, -2, 40, FLOAT(0.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(12)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_FallPosition, -2, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        WAIT_FRAMES(32)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    ELSE
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetUnitWork, -2, 3, 45)
        LABEL(62)
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        IF_INT_LESS(LW(0), 44)
            WAIT_FRAMES(1)
            GOTO(62)
        END_IF
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(6), LW(3))
            CALL(_tatsumaki_effect, -2)
            SET_VALUE(LW(0), LW(15))
            CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_TatsumakiJumpInvolved))
            CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
            LABEL(50)
            IF_INT_EQUAL(LW(3), -1)
                GOTO(59)
            END_IF
            IF_INT_EQUAL(LW(3), LW(6))
                GOTO(58)
            END_IF
            CALL(btlevtcmd_PreCheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 256, LW(5))
            SWITCH(LW(5))
                CASE_EQUAL(2)
                CALL(btlevtcmd_StartAvoid, LW(3), 40)
                CASE_EQUAL(3)
                CALL(btlevtcmd_StartAvoid, LW(3), 38)
                CASE_EQUAL(6)
                CALL(btlevtcmd_StartAvoid, LW(3), 39)
                CASE_EQUAL(4)
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetResultAC, LW(0))
                IF_BITS_SET(LW(0), 2)
                    CALL(btlevtcmd_CheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 131328, LW(5))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(7))
                    CALL(btlevtcmd_ACSuccessEffect, LW(7), LW(0), LW(1), LW(2))
                ELSE
                    CALL(btlevtcmd_CheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 256, LW(5))
                END_IF
                CASE_DEFAULT
            END_SWITCH
            LABEL(58)
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(50)
            LABEL(59)
            CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_SIBLING_EVENT
        WAIT_FRAMES(60)
        BEGIN_INLINE_EVENT_GET_ID(LW(13))
            LOOP(30)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
                SUBTRACT(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
                WAIT_FRAMES(1)
            END_IF
            CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, -2, 0, 45, 0)
            END_LOOP
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, -2, 0, 15, 0)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        WAIT_FRAMES(30)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, -2, 7, 21, 21, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_B_1"))
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
        CALL(btlevtcmd_FallPosition, -2, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
        WAIT_FRAMES(6)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_GREATER_OR_EQUAL(LW(6), 2)
            GOTO(80)
        END_IF
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    LABEL(80)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
    SET_VALUE(LW(8), LW(7))
    ADD(LW(8), 12)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, -2, LW(7), LW(8), LW(8), 0)
    END_SIBLING_EVENT
    ADD(LW(8), 1)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), 30)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(10), 0)
        LOOP(LW(8))
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
            IF_INT_EQUAL(LW(10), 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
            END_IF
            ADD(LW(10), 1)
            IF_INT_GREATER_OR_EQUAL(LW(10), 12)
                SET_VALUE(LW(10), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(8), -1, 0)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
    MULTIPLY(LW(5), 36)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetRotate, -2, LW(0), LW(1), LW(2))
        MODULO(LW(2), 360)
        DIVIDE(LW(2), 10)
        ADD(LW(2), LW(5))
        LOOP(10)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
        CASE_LESS_OR_EQUAL(-1)
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_MISS1"), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
        WAIT_MS(1500)
        CASE_DEFAULT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    GOTO(99)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_TatsumakiJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_Y"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, -1)
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        GOTO(90)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetFriendBelong, LW(3), LW(15))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_IF
    WAIT_FRAMES(2)
    CALL(btlevtcmd_StopAC)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 80, 3, -1)
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(24)
            CALL(btlevtcmd_AddRotate, -2, 0, 15, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(27)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
    CALL(btlevtcmd_SetupAC, -2, 10, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_SetUnitWork, -3, 5, 0)
    CALL(btlevtcmd_SetUnitWork, -3, 6, 0)
    CALL(btlevtcmd_SetUnitWork, -3, 7, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 30)
        CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
        SET_VALUE(LW(0), 15)
        LABEL(60)
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(61)
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 3, LW(10))
        ADD(LW(0), 1)
        IF_INT_GREATER(LW(0), LW(10))
            SET_VALUE(LW(0), LW(10))
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
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
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 1023)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 1023)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_EQUAL(2)
                    CALL(btlevtcmd_SetUnitWork, -2, 3, 45)
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 2046)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 2046)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_EQUAL(3)
                    CALL(btlevtcmd_SetUnitWork, -3, 5, 3069)
                    CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
                    SUBTRACT(LW(10), 3069)
                    MULTIPLY(LW(10), -1)
                    DIVIDE(LW(10), 10)
                    CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
                    CASE_DEFAULT
                    CALL(btlevtcmd_SetUnitWork, -2, 3, 60)
                END_SWITCH
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_JUMP_TATUMAKI1"), LW(11))
    BEGIN_SIBLING_EVENT_GET_ID(LW(12))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(11), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetUnitWork, -3, 5, LW(1))
            CALL(btlevtcmd_GetUnitWork, -3, 6, LW(0))
            IF_INT_NOT_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_GetUnitWork, -3, 7, LW(2))
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
                CALL(btlevtcmd_SetUnitWork, -3, 6, LW(0))
                CALL(evt_snd_sfx_pit, LW(11), LW(0))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        DELETE_EVENT(LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, -3, 5, -2728)
            CALL(btlevtcmd_GetUnitWork, -3, 6, LW(10))
            SUBTRACT(LW(10), -2728)
            MULTIPLY(LW(10), -1)
            DIVIDE(LW(10), 40)
            CALL(btlevtcmd_SetUnitWork, -3, 7, LW(10))
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            LOOP(40)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
                IF_INT_GREATER(LW(0), 10)
                    SUBTRACT(LW(0), 1)
                    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
                END_IF
                CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                IF_INT_LESS(LW(1), 90)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 90)
                        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_N_7"))
                        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_FALL_CRUSH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
        BEGIN_SIBLING_EVENT_GET_ID(LW(13))
            LOOP(60)
                CALL(btlevtcmd_GetRotate, -2, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
                ADD(LW(0), LW(1))
                IF_INT_GREATER_OR_EQUAL(LW(0), 360)
                    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetRotate, -2, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, -2, 40, FLOAT(0.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(12)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_FallPosition, -2, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        WAIT_FRAMES(32)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_U_3"))
        WAIT_FRAMES(12)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    ELSE
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetUnitWork, -2, 3, 45)
        LABEL(62)
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        IF_INT_LESS(LW(0), 44)
            WAIT_FRAMES(1)
            GOTO(62)
        END_IF
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(6), LW(3))
            CALL(_tatsumaki_effect, -2)
            SET_VALUE(LW(0), LW(15))
            CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_TatsumakiJumpInvolved))
            CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
            LABEL(50)
            IF_INT_EQUAL(LW(3), -1)
                GOTO(59)
            END_IF
            IF_INT_EQUAL(LW(3), LW(6))
                GOTO(58)
            END_IF
            CALL(btlevtcmd_PreCheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 256, LW(5))
            SWITCH(LW(5))
                CASE_EQUAL(2)
                CALL(btlevtcmd_StartAvoid, LW(3), 40)
                CASE_EQUAL(3)
                CALL(btlevtcmd_StartAvoid, LW(3), 38)
                CASE_EQUAL(6)
                CALL(btlevtcmd_StartAvoid, LW(3), 39)
                CASE_EQUAL(4)
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetResultAC, LW(0))
                IF_BITS_SET(LW(0), 2)
                    CALL(btlevtcmd_CheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 131328, LW(5))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(7))
                    CALL(btlevtcmd_ACSuccessEffect, LW(7), LW(0), LW(1), LW(2))
                ELSE
                    CALL(btlevtcmd_CheckDamage, -5, LW(3), LW(4), PTR(&badgeWeapon_TatsumakiJumpInvolved), 256, LW(5))
                END_IF
                CASE_DEFAULT
            END_SWITCH
            LABEL(58)
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(50)
            LABEL(59)
            CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 1, LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_SIBLING_EVENT
        WAIT_FRAMES(60)
        BEGIN_INLINE_EVENT_GET_ID(LW(13))
            LOOP(30)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
                SUBTRACT(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, -2, 3, LW(0))
                WAIT_FRAMES(1)
            END_IF
            CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, -2, 0, 45, 0)
            END_LOOP
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, -2, 0, 15, 0)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(13))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(11))
        WAIT_FRAMES(30)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, -2, 7, 21, 21, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_B_1"))
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.14941406f))
        CALL(btlevtcmd_FallPosition, -2, LW(0), LW(1), LW(2), 16)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
        WAIT_FRAMES(6)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_GREATER_OR_EQUAL(LW(6), 2)
            GOTO(80)
        END_IF
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    LABEL(80)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
    SET_VALUE(LW(8), LW(7))
    ADD(LW(8), 12)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(1)
        CALL(btlevtcmd_ACRStart, -2, LW(7), LW(8), LW(8), 0)
    END_SIBLING_EVENT
    ADD(LW(8), 1)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), 30)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(10), 0)
        LOOP(LW(8))
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
            IF_INT_EQUAL(LW(10), 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
            END_IF
            ADD(LW(10), 1)
            IF_INT_GREATER_OR_EQUAL(LW(10), 12)
                SET_VALUE(LW(10), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(8), -1, 0)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
    MULTIPLY(LW(5), 36)
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetRotate, -2, LW(0), LW(1), LW(2))
        MODULO(LW(2), 360)
        DIVIDE(LW(2), 10)
        ADD(LW(2), LW(5))
        LOOP(10)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
        CASE_LESS_OR_EQUAL(-1)
        DELETE_EVENT(LW(14))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_MISS1"), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
        WAIT_MS(1500)
        CASE_DEFAULT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    GOTO(99)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_RenzokuJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 2)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
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
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, LW(6), 0, 0, -1, -1, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 10, 17, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 3, -2)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 0, -2)
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_voice, -2, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(_record_renzoku_count, LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(mario_get_renzoku_count_max, LW(3), LW(6), LW(7))
    IF_INT_LESS(LW(6), LW(7))
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131072, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    END_IF
    ADD(LW(10), 1)
    SET_VALUE(LW(1), GSW(32))
    SET_VALUE(LW(2), GSW(35))
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
        SET_VALUE(GSW(32), LW(1))
        SET_VALUE(GSW(35), LW(2))
    END_IF
    SET_VALUE(LW(0), LW(10))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS(LW(6), LW(7))
        WAIT_FRAMES(2)
        CALL(btlevtcmd_StartAC, 0)
        GOTO(10)
    END_IF
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_RenzokuJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 2)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
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
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, LW(6), 0, 0, -1, -1, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 11, 15, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CALL(btlevtcmd_snd_voice, -2, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 3, -2)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_CONTINUE1"), 0, 0, -1, -1, 0, 0)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 0, -2)
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 250)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_voice, -2, 2)
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(_record_renzoku_count, LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    CALL(mario_get_renzoku_count_max, LW(3), LW(6), LW(7))
    IF_INT_LESS(LW(6), LW(7))
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131072, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    END_IF
    ADD(LW(10), 1)
    SET_VALUE(LW(1), GSW(32))
    SET_VALUE(LW(2), GSW(35))
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
        SET_VALUE(GSW(32), LW(1))
        SET_VALUE(GSW(35), LW(2))
    END_IF
    SET_VALUE(LW(0), LW(10))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    CALL(btlevtcmd_GetResultCountAC, LW(6))
    IF_INT_LESS(LW(6), LW(7))
        WAIT_FRAMES(2)
        CALL(btlevtcmd_StartAC, 0)
        GOTO(10)
    END_IF
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_TugiTugiJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_TUGITUGI1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    SWITCH(LW(10))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 0, -2)
        CASE_EQUAL(1)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 10, 19, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 3, -2)
        CASE_DEFAULT
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 10, 19, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 4, -2)
    END_SWITCH
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_snd_voice, -2, 2)
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
            CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        END_INLINE_EVENT
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_TugiTugiJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_SetupAC, -2, 1, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_BTL_JUMP_TUGITUGI1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    SWITCH(LW(10))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 0, -2)
        CASE_EQUAL(1)
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 13, 17, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 3, -2)
        CASE_DEFAULT
        SET_VALUE(LW(8), 36)
        CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_ACRStart, -2, 13, 17, 30, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(evt_sub_random, 2, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_4"))
                    CASE_EQUAL(1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_5"))
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_6"))
                END_SWITCH
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, -60, -30, 0)
                CASE_LESS_OR_EQUAL(-1)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_N_7"))
                CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 0, 0, 0, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(8), 4, -2)
    END_SWITCH
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_JumpContinue, -2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_D_2"))
        WAIT_FRAMES(40)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1D"))
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    CALL(btlevtcmd_ac_timing_get_success_frame, LW(0))
    DIVIDE(LW(0), 2)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        ELSE
            SET_VALUE(LW(6), LW(10))
            SUBTRACT(LW(6), 1)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(6), LW(6))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_WeaponAftereffect, LW(0))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_snd_voice, -2, 2)
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
            CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        END_INLINE_EVENT
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(10), LW(6))
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_NormalHammer_Core)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Hammer))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 7)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
        CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
            CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
            CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(6))
    IF_INT_EQUAL(LW(6), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        RETURN
    END_IF
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
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
            CALL(_hammer_star_effect, -2, LW(13))
            CASE_END
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftomsec, 30, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
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
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_ACRStart, -2, 0, 12, 60, 0)
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
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_16"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    END_IF
    WAIT_FRAMES(10)
    GOTO(95)
    LABEL(92)
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
            LOOP(30)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, 12)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 35)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_EQUAL(LW(14), 0)
            IF_INT_EQUAL(LW(0), 2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
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
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
            LOOP(40)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, 18)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 40)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 40, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        BEGIN_SIBLING_EVENT
            LOOP(60)
                CALL(btlevtcmd_AddRotate, -2, 0, 12, 12)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(35)
            CALL(btlevtcmd_ACRStart, -2, 15, 28, 28, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_C_3"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 50)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
    END_IF
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_NormalHammer_Core)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioDefaultWeapon_Hammer))
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 7)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
        CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
        CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 22)
            SUBTRACT(LW(7), 22)
        ELSE
            SUBTRACT(LW(7), 22)
            MULTIPLY(LW(7), -1)
        END_IF
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(LW(7))
            CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
            CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
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
            CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 35)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        ADD(LW(2), 5)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(6))
    IF_INT_EQUAL(LW(6), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        RETURN
    END_IF
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
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
            CALL(_hammer_star_effect, -2, LW(13))
            CASE_END
        END_SWITCH
        SWITCH(LW(12))
            CASE_OR_EQUAL(PTR(&badgeWeapon_ConfuseHammer))
            CASE_OR_EQUAL(PTR(&badgeWeapon_TsuranukiNaguri))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    CALL(btlevtcmd_ftomsec, 30, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
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
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 60, 0)
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
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_16"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    END_IF
    WAIT_FRAMES(10)
    GOTO(95)
    LABEL(92)
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
            LOOP(30)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, 12)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 35)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_EQUAL(LW(14), 0)
            IF_INT_EQUAL(LW(0), 2)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
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
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_voice, -2, 1)
            CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
            LOOP(40)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, 18)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(18)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 40)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 40, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        BEGIN_SIBLING_EVENT
            LOOP(60)
                CALL(btlevtcmd_AddRotate, -2, 0, 12, 12)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(35)
            CALL(btlevtcmd_ACRStart, -2, 15, 28, 28, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_C_3"))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 50)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
        WAIT_FRAMES(4)
        CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(0), 2)
            GOTO(95)
        END_IF
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_FINISH1"), 0)
        WAIT_FRAMES(90)
    END_IF
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(marioAttackEvent_KaitenHammer)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(evt_btl_camera_set_mode, 0, 7)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
        CALL(btlevtcmd_WaitGuardMove)
        CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
        CALL(btlevtcmd_RunDataEventChild, -2, 7)
        CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
        CALL(evt_btl_camera_set_mode, 0, 11)
        CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    CALL(btlevtcmd_GetWeaponActionLv, LW(12), LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_AcSetParamAll, 10, 8, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        CASE_END
        CASE_DEFAULT
        CALL(btlevtcmd_AcSetParamAll, 7, 15, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_AcSetFlag, 1)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
    SUBTRACT(LW(7), 21)
    IF_INT_LESS(LW(7), 0)
        MULTIPLY(LW(7), -1)
        WAIT_FRAMES(LW(7))
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(7))
    IF_INT_EQUAL(LW(7), 5)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        RETURN
    END_IF
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, STRING("p_kaiten_h"), 0)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("P_H_1A"))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_paper_light_off, -2, 1)
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_KAITEN1"), EVT_NULLPTR, 0, LW(15))
        CASE_END
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_ULTRA1"), EVT_NULLPTR, 0, LW(15))
        CASE_END
        CASE_DEFAULT
    END_SWITCH
    WAIT_FRAMES(28)
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("P_H_1B"))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_AnimeChangePaperAnime, -2, 1, STRING("P_H_1C"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    IF_INT_NOT_EQUAL(LW(7), 1)
        IF_INT_EQUAL(LW(7), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(7), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(7), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        GOTO(90)
    END_IF
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        CASE_END
        CASE_DEFAULT
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        SET_VALUE(LW(0), 1)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(0), 0)
    END_SWITCH
    IF_INT_EQUAL(LW(0), 1)
        BEGIN_INLINE_EVENT
            CALL(_whirlwind_effect, -2, 60)
        END_INLINE_EVENT
        CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
        MULTIPLY(LW(0), 36)
        BEGIN_SIBLING_EVENT_GET_ID(LW(9))
            LOOP(6)
                SWITCH(LW(12))
                    CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
                    CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
                    CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_ULTRA2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CASE_END
                END_SWITCH
                LOOP(10)
                    CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        END_SIBLING_EVENT
        IF_INT_NOT_EQUAL(LW(7), 1)
            WAIT_FRAMES(50)
            DELETE_EVENT(LW(9))
            BEGIN_SIBLING_EVENT
                DIVIDE(LW(0), 3)
                LOOP(30)
                    CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
            WAIT_FRAMES(24)
            WAIT_MS(1500)
        ELSE
            CALL(btlevtcmd_SetPartsBlur, -2, 1, 255, 255, 255, 80, 255, 255, 255, 0, 0)
            CALL(btlevtcmd_OnPartsAttribute, -2, 1, 67108864)
            BEGIN_INLINE_EVENT
                WAIT_FRAMES(50)
                CALL(btlevtcmd_SetPartsBlur, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_FRAMES(10)
                CALL(btlevtcmd_OffPartsAttribute, -2, 1, 67108864)
            END_INLINE_EVENT
            CALL(btlevtcmd_ACRStart, -2, 40, 63, 63, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), EVT_NULLPTR)
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
                CALL(_kaiten_hammer_acrobat_rotate, -2, 1, 30, STRING("M_V_2"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(30)
                CASE_DEFAULT
                CALL(btlevtcmd_WaitEventEnd, LW(9))
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_IF
    ELSE
        IF_INT_NOT_EQUAL(LW(7), 1)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            MULTIPLY(LW(0), 36)
            BEGIN_SIBLING_EVENT_GET_ID(LW(9))
                LOOP(6)
                    SWITCH(LW(12))
                        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
                        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
                        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_KAITEN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                        CASE_END
                    END_SWITCH
                    LOOP(10)
                        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(50)
            DELETE_EVENT(LW(9))
            BEGIN_SIBLING_EVENT
                DIVIDE(LW(0), 3)
                LOOP(30)
                    CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_SIBLING_EVENT
            WAIT_FRAMES(8)
            CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_S"))
            WAIT_FRAMES(24)
            WAIT_MS(1500)
        ELSE
            BEGIN_INLINE_EVENT
                CALL(_whirlwind_effect, -2, 60)
            END_INLINE_EVENT
            BEGIN_SIBLING_EVENT_GET_ID(LW(9))
                CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
                MULTIPLY(LW(0), 36)
                LOOP(6)
                    SWITCH(LW(12))
                        CASE_OR_EQUAL(PTR(&marioWeapon_KaitenHammer))
                        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackKaitenHammer))
                        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_KAITEN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                        CASE_END
                    END_SWITCH
                    LOOP(10)
                        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_LOOP
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_ACRStart, -2, 40, 63, 63, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), EVT_NULLPTR)
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
                CALL(_kaiten_hammer_acrobat_rotate, -2, 1, 30, STRING("M_V_2"))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                WAIT_FRAMES(30)
                CASE_DEFAULT
                CALL(btlevtcmd_WaitEventEnd, LW(9))
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePaperGroup, -2, 1, 0, 0)
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_IF
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    SWITCH(LW(12))
        CASE_OR_EQUAL(PTR(&marioWeapon_UltraHammer))
        CASE_OR_EQUAL(PTR(&marioDefaultWeapon_FirstAttackUltraHammer))
        CASE_END
        CASE_DEFAULT
        GOTO(99)
    END_SWITCH
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        GOTO(99)
    END_IF
    IF_INT_NOT_EQUAL(LW(7), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckToken, LW(3), 32, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    LABEL(96)
    CALL(btlevtcmd_CheckToken, LW(3), 64, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(96)
    END_IF
    CALL(btlevtcmd_OffToken, LW(3), 64)
    LABEL(98)
    CALL(btlevtcmd_CheckToken, LW(3), 128, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckToken, -2, 256, LW(0))
        IF_INT_EQUAL(LW(0), 0)
        END_IF
        WAIT_FRAMES(1)
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&marioWeapon_UltraHammerFinish), 131328, LW(5))
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

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

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_JishinHammer)
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_MovePosition, -2, -60, LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(75)
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
        CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, 11, 0, LW(5), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(8)
        CALL(evt_sub_intpl_init, 12, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
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
        CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, -2, LW(6), LW(7), LW(8), 40, -1)
    END_SIBLING_EVENT
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    LABEL(40)
    CALL(evt_btl_camera_set_mode, 0, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), 2)
        CALL(btlevtcmd_GetResultPrizeLv, -5, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
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
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), 2)
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
    END_IF
    GOTO(50)
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_16"))
        WAIT_FRAMES(18)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ACRStart, -2, 0, 17, 17, 0)
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
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -30, 20, 0)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        LOOP(60)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, 12)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, -2, 42, 55, 55, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_JishinHammer)
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 25, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_MovePosition, -2, -60, LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(75)
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
        CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, 11, 0, LW(5), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(8)
        CALL(evt_sub_intpl_init, 12, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
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
        CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, -2, LW(6), LW(7), LW(8), 40, -1)
    END_SIBLING_EVENT
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    LABEL(40)
    CALL(evt_btl_camera_set_mode, 0, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), 2)
        CALL(btlevtcmd_GetResultPrizeLv, -5, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
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
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), 2)
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
    END_IF
    GOTO(50)
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_16"))
        WAIT_FRAMES(18)
    END_SIBLING_EVENT
    CALL(btlevtcmd_ACRStart, -2, 0, 17, 17, 0)
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
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        LOOP(60)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, 12)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, -2, 42, 55, 55, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

EVT_BEGIN(marioAttackEvent_UltraJishinHammer)
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 4, 16, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_MovePosition, -2, -60, LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(32)
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
        CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
        MULTIPLY(LW(5), 45)
        CALL(evt_sub_intpl_init, 11, 0, LW(5), 10)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(14)
        CALL(evt_sub_intpl_init, 12, LW(5), 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
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
        CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.23925781f))
        CALL(btlevtcmd_JumpPosition, -2, LW(6), LW(7), LW(8), 55, -1)
    END_SIBLING_EVENT
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    WAIT_FRAMES(10)
    GOTO(40)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_ATTACK_ULTRAQUAKE1"), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    LABEL(40)
    CALL(evt_btl_camera_set_mode, 0, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_ULTRAQUAKE1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 42, 13)
    CALL(evt_audience_reflesh_natural_all, 100)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IF_BITS_SET(LW(7), 2)
        CALL(btlevtcmd_GetResultPrizeLv, -5, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
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
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 0)
        BEGIN_INLINE_EVENT
            WAIT_FRAMES(42)
            IF_BITS_SET(LW(7), 2)
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
            ELSE
                CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
            END_IF
        END_INLINE_EVENT
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
    END_IF
    GOTO(50)
    LABEL(90)
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_H_16"))
    WAIT_FRAMES(10)
    LABEL(95)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_HammerRotate)
    SET_VALUE(LW(0), -25)
    CALL(btlevtcmd_SetPartsRotateOffset, -2, 3, 0, 12, 0)
    LABEL(0)
    CALL(btlevtcmd_AddPartsRotate, -2, 3, 0, 0, LW(0))
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_HammerNageru)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 5, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_7"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_8"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_11"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_12"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_3"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_4"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_NAGERU1"), 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    WAIT_FRAMES(3)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerNageru_object), LW(15))
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_MS(1000)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
        WAIT_MS(667)
        GOTO(94)
    END_SWITCH
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -40, 30, 0)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        LOOP(30)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, -24)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    LABEL(94)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 5)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_HammerNageru)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 5, 40, 0, 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 3, 1, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 450)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("M_H_5A"))
        SET_VALUE(LW(1), STRING("M_H_5B"))
        SET_VALUE(LW(2), STRING("M_H_7"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_8"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("M_H_9A"))
        SET_VALUE(LW(1), STRING("M_H_9B"))
        SET_VALUE(LW(2), STRING("M_H_11"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_12"))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("M_H_1A"))
        SET_VALUE(LW(1), STRING("M_H_1B"))
        SET_VALUE(LW(2), STRING("M_H_3"))
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_4"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_HAMMER_NAGERU1"), 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    WAIT_FRAMES(3)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerNageru_object), LW(15))
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_MS(1000)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
        WAIT_MS(667)
        GOTO(94)
    END_SWITCH
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 20, 0)
        LOOP(30)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, -24)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(8)
        CALL(btlevtcmd_ACRStart, -2, 12, 25, 25, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_Z_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3A"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_ACRGetResult, LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_A_3B"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING("_BTLEF"), STRING("confetti"), 3, LW(0), LW(1), LW(2), 90, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(1500)
    END_IF
    LABEL(94)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 5)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_HammerNageru_object)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(btlevtcmd_SetPartsPos, -2, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
    SET_VALUE(LW(0), -25)
    CALL(btlevtcmd_SetPartsRotate, -2, 3, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerRotate), LW(13))
    CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(6.0f), FLOAT(0.34960938f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 0, -1)
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
        CALL(btlevtcmd_JumpPartsContinue, -2, 3)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -30)
        CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -15)
        CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 12, -1)
        DELETE_EVENT(LW(13))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
        GOTO(90)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_mode, 0, 1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
        SET_VALUE(LW(0), 9)
    END_IF
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
    WAIT_FRAMES(20)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_HammerNageru_object)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(btlevtcmd_SetPartsPos, -2, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
    SET_VALUE(LW(0), -25)
    CALL(btlevtcmd_SetPartsRotate, -2, 3, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_HammerRotate), LW(13))
    CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(6.0f), FLOAT(0.34960938f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 0, -1)
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
        CALL(btlevtcmd_JumpPartsContinue, -2, 3)
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -30)
        CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -15)
        CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 12, -1)
        DELETE_EVENT(LW(13))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
        GOTO(90)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_mode, 0, 1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    CALL(btlevtcmd_GetDamage, LW(3), LW(0))
    CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 10, 13)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
    WAIT_FRAMES(20)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    RETURN
EVT_END()
#endif

static const char battle_mario_nullstr[4] = "";
extern const f32 battle_mario_one;

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_FireNaguri)
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 0, 83, 63, 83, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 16)
    CALL(btlevtcmd_SetupAC, -2, 11, 1, 0)
    WAIT_FRAMES(2)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 40, 0, 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_SetPartsPos, -2, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_8"))
    CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(_mario_fire_ball_controll, -2, 3)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    ADD(LW(1), 140)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_DivePartsPosition, -2, 3, LW(0), LW(1), LW(2), 60, 0, 5, 0, -1)
    END_SIBLING_EVENT
    LABEL(10)
    CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        DELETE_EVENT(LW(15))
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE1"), EVT_NULLPTR, 0, LW(15))
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(4.0f), FLOAT(0.19921875f))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FallPartsPosition, -2, 3, LW(0), LW(1), LW(2), 75)
    END_INLINE_EVENT
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), 2)
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
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
            WAIT_FRAMES(7)
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 0)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(evt_eff64, PTR((char*)battle_mario_nullstr), STRING("fire_spark_n64"), 0, LW(0), LW(1), LW(2), 30, 40, FLOAT(1.0f), 12, 100, FLOAT(1.0f), FLOAT(1.0f), 0)
        ADD(LW(1), 5)
        CALL(evt_eff, PTR((char*)battle_mario_nullstr), STRING("kemuri_test"), 11, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_btl_camera_set_mode, 0, 1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
        CALL(btlevtcmd_GetUnitId, -2, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
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
        CALL(btlevtcmd_GetUnitId, -2, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        LABEL(20)
        IF_INT_EQUAL(LW(3), -1)
            GOTO(22)
        END_IF
        IF_INT_NOT_EQUAL(LW(3), LW(6))
            GOTO(21)
        END_IF
        CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    WAIT_FRAMES(7)
    DELETE_EVENT(LW(14))
    CALL(evt_btl_camera_set_mode, 0, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_FireNaguri_object), LW(15))
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        LOOP(20)
            CALL(btlevtcmd_AddRotate, -2, 0, 36, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_V_2"))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(500)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    END_SWITCH
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_FireNaguri)
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL_CHILD(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 0, 83, 63, 83, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 16)
    CALL(btlevtcmd_SetupAC, -2, 11, 1, 0)
    WAIT_FRAMES(2)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 40, 0, 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_SetPartsPos, -2, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("M_H_8"))
    CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(_mario_fire_ball_controll, -2, 3)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    ADD(LW(1), 140)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_DivePartsPosition, -2, 3, LW(0), LW(1), LW(2), 60, 0, 5, 0, -1)
    END_SIBLING_EVENT
    LABEL(10)
    CALL(btlevtcmd_PadCheckNow, 0, 262144, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        DELETE_EVENT(LW(15))
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_POWER2"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(0))
    CALL(_get_mario_hammer_lv, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_DEFAULT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_HAMMER_HOLD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE1"), EVT_NULLPTR, 0, LW(15))
    BEGIN_INLINE_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(4.0f), FLOAT(0.19921875f))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FallPartsPosition, -2, 3, LW(0), LW(1), LW(2), 75)
    END_INLINE_EVENT
    CALL(btlevtcmd_StartAC, 1)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(1))
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), 2)
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
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
            WAIT_FRAMES(7)
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(14))
        CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 0)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(evt_eff64, PTR((char*)battle_mario_nullstr), STRING("fire_spark_n64"), 0, LW(0), LW(1), LW(2), 30, 40, FLOAT(1.0f), 12, 100, FLOAT(1.0f), FLOAT(1.0f), 0)
        ADD(LW(1), 5)
        CALL(evt_eff, PTR((char*)battle_mario_nullstr), STRING("kemuri_test"), 11, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_btl_camera_set_mode, 0, 1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 0)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
        CALL(btlevtcmd_GetUnitId, -2, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
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
        CALL(btlevtcmd_GetUnitId, -2, LW(6))
        CALL(btlevtcmd_GetEnemyBelong, LW(6), LW(0))
        CALL(btlevtcmd_SamplingEnemy, -5, LW(0), PTR(&badgeWeapon_FireNaguriFailed))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        LABEL(20)
        IF_INT_EQUAL(LW(3), -1)
            GOTO(22)
        END_IF
        IF_INT_NOT_EQUAL(LW(3), LW(6))
            GOTO(21)
        END_IF
        CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
    WAIT_FRAMES(7)
    DELETE_EVENT(LW(14))
    CALL(evt_btl_camera_set_mode, 0, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_HAMMER_FIRE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SPAWN_EVENT_GET_ID(PTR(marioAttackEvent_FireNaguri_object), LW(15))
    CALL(btlevtcmd_ACRStart, -2, 0, 15, 15, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        LOOP(20)
            CALL(btlevtcmd_AddRotate, -2, 0, 36, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_snd_voice, -2, 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_V_2"))
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_MS(500)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_S_1"))
    END_SWITCH
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()
#endif

EVT_BEGIN(marioAttackEvent_FireNaguri_object)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_GetPartsPos, -2, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(5), 2)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(5))
        CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(6.0f), FLOAT(0.2998047f))
        CALL(btlevtcmd_FallPartsPosition, -2, 3, LW(0), LW(1), LW(2), 0)
        CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 0)
        RETURN
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, -2, 3, 0, FLOAT(4.0f), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 3, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_SetPartsWork, -2, 3, 0, 0)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_GetResultPrizeLv, -5, 0, LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), 0, 50, 0)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
    ELSE
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    CALL(btlevtcmd_GetPartsPos, -2, 3, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, -1, -1, 2, LW(12))
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
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, -1, -1, 2, LW(14))
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
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_GetResultAC, LW(0))
        IF_BITS_SET(LW(0), 2)
            CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(13), 2)
            SUBTRACT(LW(13), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
    END_IF
    GOTO(50)
    LABEL(90)
    LABEL(92)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, -1, -1, 2, LW(10))
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

EVT_BEGIN(_NormalCharge_core)
    CALL(btlevtcmd_GetUnitId, -2, LW(3))
    CALL(btlevtcmd_GetBodyId, -2, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    WAIT_FRAMES(45)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, -2, 0, 24, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_CHARGE1"), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, -2, LW(5))
    DIVIDE(LW(5), 2)
    ADD(LW(1), LW(5))
    ADD(LW(2), 10)
    CALL(evt_eff64, STRING("noname"), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(_SuperCharge_core)
    CALL(btlevtcmd_GetUnitId, -2, LW(3))
    CALL(btlevtcmd_GetBodyId, -2, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    WAIT_FRAMES(45)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, -2, 0, 24, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_SUPER_CHARGE1"), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, -2, LW(5))
    DIVIDE(LW(5), 2)
    ADD(LW(1), LW(5))
    ADD(LW(2), 10)
    CALL(evt_eff64, STRING("noname"), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

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

EVT_BEGIN(marioAttackEvent_MiniminiFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_NemuraseFumi)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_FunyafunyaJump)
    CALL_CHILD(PTR(marioAttackEvent_NormalJump))
    RETURN
EVT_END()

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

EVT_BEGIN(marioAttackEvent_NormalCharge)
    SET_VALUE(LW(12), PTR(&badgeWeapon_Charge))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_NormalChargeP)
    SET_VALUE(LW(12), PTR(&badgeWeapon_ChargeP))
    CALL_CHILD(PTR(_NormalCharge_core))
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_SuperCharge)
    SET_VALUE(LW(12), PTR(&badgeWeapon_SuperCharge))
    CALL_CHILD(PTR(_SuperCharge_core))
    RETURN
EVT_END()

EVT_BEGIN(marioAttackEvent_SuperChargeP)
    SET_VALUE(LW(12), PTR(&badgeWeapon_SuperChargeP))
    CALL_CHILD(PTR(_SuperCharge_core))
    RETURN
EVT_END()

static const Vec majinai_effect_pos = {0.0f, 10.0f, 0.0f};

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(_majinai_powerup_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_power_up"), 180)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_MajinaiPowerUpCheck)
    CALL(_majinai_powerup_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_power_up"), 60)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#endif

EVT_BEGIN(marioAttackEvent_MajinaiJump)
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetBodyId, -2, LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 44)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 35)
    RETURN
EVT_END()

EVT_BEGIN(_MajinaiPoseReset)
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_MajinaiDefenceUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_defence_up"), 180)
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(_MajinaiPoseReset))
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_MajinaiDefenceUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_defence_up"), 60)
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(_MajinaiPoseReset))
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(marioAttackEvent_MajinaiExpUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(evt_majinai_effect_se_btl)
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 2, 0, 0, STRING("btl_msg_mj_exp_up"), 180)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#else
EVT_BEGIN(marioAttackEvent_MajinaiExpUp)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(PTR(evt_majinai_effect_se_btl))
    CALL(_battle_majinai_effect, -3)
    CALL_CHILD(PTR(marioAttackEvent_MajinaiJump))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_mj_exp_up"), 60)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#endif

















static USER_FUNC(_get_mario_hammer_lv) {
    s32 ret = *event->args;
    evtSetValue(event, ret, pouchGetPtr()->hammerLevel);
    return EVT_RETURN_DONE;
}

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
    eff = effWhirlwindN64Entry(unit->faceDirection < 0, NULL, unit->mSizeMultiplier, life);
    ((EffWhirlwindN64Work*)eff->userdata)->position.x = pos[0];
    ((EffWhirlwindN64Work*)eff->userdata)->position.y = pos[1];
    ((EffWhirlwindN64Work*)eff->userdata)->position.z = pos[2];
    return EVT_RETURN_DONE;
}

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
        pos[0] += dx * unit->mSizeMultiplier;
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

USER_FUNC(_bgset_iron_frame_check) {
    s32 ret = *event->args;
    if (_battleWorkPointer->stageObjectWork.flags & 0x20) {
        evtSetValue(event, ret, 0);
    } else {
        evtSetValue(event, ret, 1);
    }
    return EVT_RETURN_DONE;
}

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
        ud[2] = (s32) * &unit->mPosition.x;
        ud[3] = (s32) * &unit->mPosition.y - 40;
        ud[4] = (s32) * &unit->mPosition.z;
        ud[5] = (EvtWord)effKamekiTornadeN64Entry(0, 90, ud[2], ud[3], ud[4], 3.0f);
        ud[6] = (EvtWord)effKamekiTornadeN64Entry(0, 90, ud[2], ud[3], ud[4], 3.0f);
        ud[7] = psndSFXOn("SFX_BTL_JUMP_TATUMAKI2");
        v.x = ud[2];
        v.y = ud[3];
        v.z = ud[4];
        psndSFX_pos(ud[7], &v);
    case 1:
        ud[1]++;
        r = ((f32(*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, 80.0f, 5, ud[1], 90);
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

USER_FUNC(_record_renzoku_count) {
    s32 n = evtGetValue(event, *event->args);
    BattleActRecord* rec = &_battleWorkPointer->actRecord;
    PouchData* pouch = pouchGetPtr();
    if (n > rec->powerBounceCombo) {
        if (n > pouch->mPowerBounceRecord) {
            pouch->mPowerBounceRecord = n;
            BattleAudience_Case_JumpNewRecord();
        }
        if (n > 100) {
            n = 100;
        }
        rec->powerBounceCombo = n;
    }
    return EVT_RETURN_DONE;
}

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
    max = unit->mKindParams->pbSoftCap;
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
        angle = ((f32(*)(f32, f32, s32, s32, s32))intplGetValue)(210.0f, 420.0f, 1, event->userdata[0], 12);
        rot = -10.0f;
    } else {
        angle = ((f32(*)(f32, f32, s32, s32, s32))intplGetValue)(150.0f, -60.0f, 1, event->userdata[0], 12);
        rot = 10.0f;
    }
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    pos[1] += unit->mSizeMultiplier * (0.85f * BtlUnit_GetHeight(unit));
    pos[0] += unit->mSizeMultiplier * (26.0f * -sinfd(angle));
    pos[1] += unit->mSizeMultiplier * (26.0f * cosfd(angle));
    effEnergyEntry(type, 30, pos[0], pos[1], pos[0], 0.08f * event->userdata[0] + 0.1f, rot, 0.05f);
    if (event->userdata[0] < 12) {
        event->userdata[0]++;
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_kaiten_hammer_acrobat_rotate) {
    BattleUnit* unit;
    BattleUnitPart* part;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 n;
    s32 anim;
    f32 rx, ry, rz;
    s32 id = evtGetValue(event, args[0]);
    s32 pn;
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    pn = evtGetValue(event, args[1]);
    part = BtlUnit_GetPartsPtr(unit, pn);
    n = evtGetValue(event, args[2]);
    anim = evtGetValue(event, args[3]);
    if (isFirstCall) {
        BtlUnit_GetRotate(unit, &rx, &ry, &rz);
        event->userdata[0] = n;
        if (unit->faceDirection >= 0) {
            f32 a = reviseAngle(ry);
            event->userdata[1] = 360.0f + (360.0f - a);
        } else {
            event->userdata[1] = 360.0f + reviseAngle(ry);
        }
        event->userdata[2] = unit->faceDirection * event->userdata[1] / n;
        event->userdata3 = 0;
    }
    BtlUnit_AddRotate(unit, 0, event->userdata[2], 0);
    event->userdata[0]--;
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
        event->userdata[1] = (EvtWord)effFireEntry(5, 0.0f, 0.0f, 0.0f, battle_mario_one, 1000);

    }
    eff = (EffectEntry*)event->userdata[1];
    if (part->work[0] == 0) {
        effDelete(eff);
        return EVT_RETURN_DONE;
    }
    {
        f32* p = (f32*)eff->userdata;
        BtlUnit_GetPartsPos(part, &p[1], &p[2], &p[3]);
    }
    return EVT_RETURN_BLOCK;
}
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


USER_FUNC(_majinai_powerup_check) {
    BattleWork* wp = _battleWorkPointer;
    s32 ret = *event->args;
    if (pouchGetPtr()->mMerleeNextCurseType != 1) {
        evtSetValue(event, ret, 0);
    } else {
        evtSetValue(event, ret, 1);
        wp->majinaiState = 1;
        BattleMajinaiDone();
        BattleMajinaiEndCheck();
    }
    return EVT_RETURN_DONE;
}

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

USER_FUNC(_paper_light_off) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 part;
    id = BattleTransID(event, id);
    part = evtGetValue(event, args[1]);
    animPoseSetMaterialLightFlagOff(BattleGetUnitPartsPtr(id, part)->poseId, 1);
    return EVT_RETURN_DONE;
}

const f32 battle_mario_one = 1.0f;
