#include "version.h"
#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "eff/eff_nokotarou.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "unit/unit_koura.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_damage.h"
#include "system.h"
#include "battle/unit_work.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SHELL_SPINNING 0     // 1 keeps the shell-spin loop of _koura_rotate_start running; 0 ends it (_koura_rotate_stop, _restore_koura_pose)
#define UNIT_WORK_SHELL_SPIN_SPEED 1   // degrees per frame the shell spin ramps up to (45, 36, 24, 15, 12; the sign gives the direction)
#define UNIT_WORK_SHELL_SPIN_REVERSE 2 // nonzero makes the spin loop flip the spin direction once (the loop clears it)
#define UNIT_WORK_CHARGE_SFX 3         // sound id of SFX_BTL_NOKO_IPPATSU1 while Tsuranuki charges (-1 before it starts); its pitch follows the AC
#define UNIT_WORK_SFX_PITCH_BASE 4     // base pitch (3410) of the charge sound; evt_snd_sfx_pit gets base * slot 5 / 100
#define UNIT_WORK_SFX_PITCH_RATE 5     // percentage 0-100 of the base pitch (ramps while the shell is held, or the AC output in Tsuranuki); 0 after the shot
// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_SPIN_DUST 0    // part 1: 1 makes the shell spin raise kemuri_test dust each turn (Tsuranuki charge)
#define PARTS_WORK_CHARGE_LEVEL 1 // part 1: Tsuranuki charge level 0-3 from the AC; _tsuranuki_effect_control sizes the effNokotarou effect by it
#define PARTS_WORK_EFFECT_END 2   // part 1: 1 makes _tsuranuki_effect_control delete the charge effect and finish (it clears the flag)

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetJumpSound);
static USER_FUNC(_check_guard_koura);
USER_FUNC(_tsuranuki_effect_control);
USER_FUNC(_color_lv_set);
USER_FUNC(_check_mario_move_count);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);

PoseTableEntry pose_table_nokotarou_stay[26];
PoseTableEntry pose_table_nokotarou_turn[12];
static BattleDataEntry data_table[12];
BattleUnitKindPart unitpartsdata_Party_Nokotarou[1];
BattleUnitKind unitdata_Party_Nokotarou;
static EvtWord battle_entry_event[59];
static EvtWord init_event[78];
EvtWord change_party_spawn_init_event[28];
static EvtWord dmg_turn_event[65];
static EvtWord wakeup_event[67];
BattleWeapon partyWeapon_NokotarouFirstAttack;
BattleWeapon partyWeapon_NokotarouKouraAttack;
BattleWeapon partyWeapon_NokotarouSyubibinKoura;
BattleWeapon partyWeapon_NokotarouKouraGuard;
BattleWeapon partyWeapon_NokotarouTsuranukiKoura;
static EvtWord _change_koura_pose[34];
static EvtWord _change_koura_pose_fast[78];
static EvtWord _restore_koura_pose[47];
static EvtWord _koura_rotate_start[199];
static EvtWord _koura_rotate_stop[13];
EvtWord partyNokotarouAttack_FirstAttack[236];
EvtWord partyNokotarouAttack_NormalAttack[VERSION_PICK(793, 804)];
EvtWord partyNokotarouAttack_SyubibinKoura[VERSION_PICK(691, 702)];
EvtWord partyNokotarouAttack_KouraGuard[838];
EvtWord partyNokotarouAttack_TsuranukiKoura[VERSION_PICK(931, 942)];
EvtWord nokotarou_appeal_event[34];
static EvtWord party_win_reaction[16];
static EvtWord attack_audience_event[263];
EvtWord nokotarou_revive_event[16];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static EvtWord damage_event[10];
static EvtWord attack_event[2];
static EvtWord wait_event[6];
BattleUnitSetup entry_koura;









PoseTableEntry pose_table_nokotarou_stay[] = {
    {POSE_TYPE_FREEZE, "PNK_D_5"},
    {POSE_TYPE_STOP, "PNK_D_5"},
    {POSE_TYPE_DIZZY, "PNK_X_1"},
    {POSE_TYPE_POISON, "PNK_D_2"},
    {POSE_TYPE_CONFUSE, "PNK_X_1"},
    {POSE_TYPE_SLEEP, "PNK_N_1"},
    {POSE_TYPE_KO, "PNK_D_3"},
    {POSE_TYPE_NORMAL, "PNK_S_1"},
    {POSE_TYPE_DAMAGE_DRAINED, "PNK_G_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PNK_G_1"},
    {POSE_TYPE_DAMAGE, "PNK_D_1"},
    {POSE_TYPE_WALK, "PNK_W_1"},
    {POSE_TYPE_RUN, "PNK_R_1"},
    {POSE_TYPE_ATTACK, "PNK_A_1"},
    {POSE_TYPE_PERIL, "PNK_D_2"},
    {POSE_TYPE_DANGER, "PNK_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PNK_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PNK_X_1"},
    {POSE_TYPE_TALK, "PNK_T_1"},
    {POSE_TYPE_ITEM, "PNK_I_1"},
    {POSE_TYPE_COMMAND, "PNK_C_1"},
    {POSE_TYPE_APPEAL, "PNK_O_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PNK_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PNK_A_1"},
    {POSE_TYPE_DEFEND, "PNK_G_1"},
    {POSE_TYPE_ACTION, "PNK_S_1"},
};

PoseTableEntry pose_table_nokotarou_turn[] = {
    {POSE_TYPE_FREEZE, "PNK_D_5"},
    {POSE_TYPE_STOP, "PNK_D_5"},
    {POSE_TYPE_SLEEP, "PNK_N_2"},
    {POSE_TYPE_KO, "PNK_D_3"},
    {POSE_TYPE_NORMAL, "PNK_H_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PNK_D_1"},
    {POSE_TYPE_DAMAGE, "PNK_D_1"},
    {POSE_TYPE_PERIL, "PNK_H_1"},
    {POSE_TYPE_DANGER, "PNK_H_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PNK_H_1"},
    {POSE_TYPE_DEFEND, "PNK_H_1"},
    {POSE_TYPE_ACTION, "PNK_H_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, nokotarou_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {4, change_party_spawn_init_event},
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_WAKEUP_EVENT, wakeup_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_REVIVE_EVENT, nokotarou_revive_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

BattleUnitKindPart unitpartsdata_Party_Nokotarou[] = {
    {1, "btl_un_nokotarou", "c_pnk", 0.0f, 0.0f, 0.0f, 12.0f, 30.0f, 5.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsShelled, 0, pose_table_nokotarou_stay},
};

BattleUnitKind unitdata_Party_Nokotarou = {
    UNIT_KOOPS, // mUnitType
    "btl_un_nokotarou", // name
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
    33, // height
    {14, 36}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 40.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {10.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 21.45f, 0.0f}, // kissHitOffset
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
    unitpartsdata_Party_Nokotarou, // mParts
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
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 9, 9, 0, 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPINNING, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_BASE, 3410)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_EFFECT_END, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(change_party_spawn_init_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_PARTNER, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_PARTNER, 1, PTR(&defence_turn))
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_PARTNER, 1, PTR(pose_table_nokotarou_turn))
        CALL(btlevtcmd_OnStatusFlag, UNIT_ID_PARTNER, UNIT_STATUS_FLAG_FLIPPED)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_PARTNER, 1)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_D_1"))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_DEFEND)
    CALL(evt_snd_sfxon, STRING("SFX_CONDITION_INSIDE"), 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_stay))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_S_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

BattleWeapon partyWeapon_NokotarouFirstAttack = {
    NULL, // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {2, 2, 3, 3, 5, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyNokotarouAttack_FirstAttack, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_NokotarouKouraAttack = {
    "btl_wn_pnk_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pnk_normal_attack", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 2, 2, 3, 3, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hammer", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyNokotarouAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_NokotarouSyubibinKoura = {
    "btl_wn_pnk_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pnk_syubibin_koura", // description
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
    {1, 2, 2, 3, 3, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_koura_shubibin", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyNokotarouAttack_SyubibinKoura, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_NokotarouKouraGuard = {
    "btl_wn_pnk_lv2", // name
    ICON_PARTNER_MOVE_LV2, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pnk_koura_no_mamori", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_ONLY_MARIO | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_koura_no_mamori", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyNokotarouAttack_KouraGuard, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_NokotarouTsuranukiKoura = {
    "btl_wn_pnk_lv3_tsuranuki", // name
    ICON_PARTNER_MOVE_LV3, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pnk_koura_tsuranuki", // description
    100, // baseAccuracy
    6, // baseFpCost
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
    weaponGetACOutputParam, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_tsuranuki_koura", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyNokotarouAttack_TsuranukiKoura, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

static EVT_BEGIN(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(18)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(_change_koura_pose_fast)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 5, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 27, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(3)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 30, 0)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 5, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(_restore_koura_pose)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPINNING, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(_koura_rotate_start)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPINNING, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_REVERSE, 0)
    BEGIN_INLINE_EVENT
        LABEL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPINNING, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_REVERSE, LW(11))
        IF_INT_NOT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_REVERSE, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, LW(10))
            MULTIPLY(LW(10), -1)
            MULTIPLY(LW(0), -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, LW(10))
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, LW(10))
        IF_INT_GREATER_OR_EQUAL(LW(10), 0)
            ADD(LW(0), 1)
            IF_INT_GREATER(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            IF_INT_LESS(LW(1), LW(0))
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, LW(1))
                IF_INT_NOT_EQUAL(LW(1), 0)
                    BEGIN_SIBLING_EVENT
                        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                        CALL(evt_eff, 0, STRING("kemuri_test"), 5, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
                    END_SIBLING_EVENT
                END_IF
            END_IF
        ELSE
            SUBTRACT(LW(0), 1)
            IF_INT_LESS(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                BEGIN_SIBLING_EVENT
                    MULTIPLY(LW(0), -1)
                    CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                    ADD(LW(1), 360)
                    IF_INT_LESS(LW(1), LW(0))
                        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                        CALL(evt_eff, 0, STRING("kemuri_test"), 5, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
                    END_IF
                END_SIBLING_EVENT
            END_IF
        END_IF
        WAIT_FRAMES(1)
        GOTO(0)
        LABEL(10)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(_koura_rotate_stop)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPINNING, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(partyNokotarouAttack_FirstAttack)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&partyWeapon_NokotarouFirstAttack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    SET_VALUE(LW(0), 45)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
    CALL_CHILD(PTR(_koura_rotate_start))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(50)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_ATTACK2"), 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL_CHILD(PTR(_koura_rotate_stop))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&partyWeapon_NokotarouFirstAttack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_NokotarouFirstAttack), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    SET_VALUE(LW(0), 45)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
    CALL_CHILD(PTR(_koura_rotate_start))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    CALL_CHILD(PTR(_koura_rotate_stop))
    CALL_CHILD(PTR(_restore_koura_pose))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(PTR(btldefaultevt_SuitoruBadgeEffect))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyNokotarouAttack_NormalAttack)
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_ATTACK1"), LW(13))
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
    CALL_CHILD(_koura_rotate_start)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 360)
        SET_VALUE(LW(1), 100)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 280)
        SET_VALUE(LW(1), 150)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 200)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 200)
        SET_VALUE(LW(1), 250)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 200)
        SET_VALUE(LW(1), 360)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 200)
        SET_VALUE(LW(1), 600)
        CASE_DEFAULT
        SET_VALUE(LW(0), 200)
        SET_VALUE(LW(1), 960)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 2, LW(0), 0, 1, LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, AC_GAUGE_TIMING_FLAG_WRAP | AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 19, 1, 60)
    WAIT_FRAMES(22)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(10000)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(6), 0)
        LOOP(10000)
            ADD(LW(6), 1)
            IF_INT_GREATER(LW(6), 100)
                SET_VALUE(LW(6), 100)
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(6))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_BASE, LW(0))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(1))
            MULTIPLY(LW(0), LW(1))
            DIVIDE(LW(0), 100)
            CALL(evt_snd_sfx_pit, LW(13), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_ATTACK2"), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(13))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(13), MOVE_SOUND_NONE, 0)
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
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL_CHILD(_koura_rotate_stop)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_CommandPreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), 256, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL_CHILD(_koura_rotate_stop)
    END_IF
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
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 17, 17, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
#endif
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
        LOOP(9)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 20)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_2"))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
            LABEL(91)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                GOTO(92)
            END_IF
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, LW(0))
            WAIT_FRAMES(2)
            GOTO(91)
            LABEL(92)
        END_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 60, 90, 90, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_EQUAL(LW(6), 1)
            GOTO(94)
        END_IF
        IF_INT_NOT_EQUAL(LW(6), 2)
            GOTO(94)
        END_IF
        CALL_CHILD(_koura_rotate_stop)
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_3"))
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
        END_LOOP
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        WAIT_FRAMES(60)
        GOTO(95)
        LABEL(94)
        CALL_CHILD(_koura_rotate_stop)
        CALL(evt_audience_acrobat_notry)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(95)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyNokotarouAttack_SyubibinKoura)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    WAIT_FRAMES(20)
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_SHUBIBIN1"), LW(13))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
    CALL_CHILD(_koura_rotate_start)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 480)
        SET_VALUE(LW(1), 240)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 300)
        SET_VALUE(LW(1), 180)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 120)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 180)
        SET_VALUE(LW(1), 105)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 150)
        SET_VALUE(LW(1), 90)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 120)
        SET_VALUE(LW(1), 60)
        CASE_DEFAULT
        SET_VALUE(LW(0), 100)
        SET_VALUE(LW(1), 30)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 2, LW(0), 1, 95, LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, AC_GAUGE_TIMING_FLAG_WRAP | AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 19, 1, 60)
    WAIT_FRAMES(22)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(10000)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(6), 0)
        LOOP(10000)
            ADD(LW(6), 1)
            IF_INT_GREATER(LW(6), 200)
                SET_VALUE(LW(6), 200)
            END_IF
            SET_VALUE(LW(7), LW(6))
            DIVIDE(LW(7), 2)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(7))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_BASE, LW(0))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(1))
            MULTIPLY(LW(0), LW(1))
            DIVIDE(LW(0), 100)
            CALL(evt_snd_sfx_pit, LW(13), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_SHUBIBIN2"), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(5))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        GOTO(50)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(btlevtcmd_CommandPreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), 256, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL_CHILD(_koura_rotate_stop)
    END_IF
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
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 400, LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 9, 9, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
#endif
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 12)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_1"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 49, 79, 79, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_NOT_EQUAL(LW(6), 2)
            CALL_CHILD(_koura_rotate_stop)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_2"))
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
            END_LOOP
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_S_1"))
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            END_LOOP
            CALL(evt_audience_acrobat_notry)
            GOTO(99)
        END_IF
        CALL_CHILD(_koura_rotate_stop)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_2"))
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_3"))
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
        END_LOOP
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        WAIT_FRAMES(60)
        GOTO(99)
    END_IF
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyNokotarouAttack_KouraGuard)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(_check_guard_koura, UNIT_ID_MARIO, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DESTROY_EVENT)
    END_IF
    CALL(_color_lv_set, UNIT_ID_MARIO, 0, LW(14))
    CALL(_color_lv_set, UNIT_ID_MARIO, 1, 255)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_A_2"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_SpawnUnit, LW(10), PTR(&entry_koura), 0)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitHidden)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    ADD(LW(1), 240)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffAttribute, LW(10), kUnitHidden)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 360)
        SET_VALUE(LW(1), 480)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 180)
        SET_VALUE(LW(1), 360)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 120)
        SET_VALUE(LW(1), 300)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 90)
        SET_VALUE(LW(1), 240)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 70)
        SET_VALUE(LW(1), 200)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 40)
        SET_VALUE(LW(1), 120)
        CASE_DEFAULT
        SET_VALUE(LW(0), 20)
        SET_VALUE(LW(1), 80)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 1, LW(10), LW(11), 30, LW(0), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 12, 1, 0)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 125)
    CALL(btlevtcmd_DivePosition, LW(10), LW(0), LW(1), LW(2), 60, 0, EASING_SINE_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_INT_EQUAL(0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(10)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
            WAIT_FRAMES(35)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_B_1"))
            WAIT_FRAMES(15)
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_NEUTRAL)
        END_SIBLING_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 400)
        CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
        CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.39941406f))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_NOKO_GUARD2"), EVT_NULLPTR, 0, LW(5))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(2)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 48, 67, 67, 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            IF_INT_EQUAL(LW(6), 2)
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_O_1"))
                WAIT_MS(1000)
            ELSE
                CALL(evt_audience_acrobat_notry)
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_FallPosition, LW(10), LW(0), LW(1), LW(2), 60)
        CALL(evt_snd_sfxoff, LW(5))
        CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("PKO_F_1"))
        CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_NOKO_GUARD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(9), LW(10))
            CALL(btlevtcmd_GetRotate, LW(9), EVT_NULLPTR, EVT_NULLPTR, LW(8))
            IF_INT_LESS(LW(8), 180)
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(8), 0, 50)
            ELSE
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(8), 360, 50)
            END_IF
            LOOP(50)
                CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
                CALL(btlevtcmd_SetRotate, LW(9), EVT_NULLPTR, EVT_NULLPTR, LW(1))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        IF_BITS_SET(LW(6), 2)
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
            SWITCH(LW(0))
                CASE_GREATER_OR_EQUAL(8)
                SET_VALUE(LW(0), 2)
                CASE_GREATER_OR_EQUAL(6)
                SET_VALUE(LW(0), 1)
                CASE_GREATER_OR_EQUAL(4)
                SET_VALUE(LW(0), 0)
                CASE_DEFAULT
                SET_VALUE(LW(0), -1)
            END_SWITCH
            IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(7))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_ACSuccessEffect, LW(7), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(7))
            ELSE
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            END_IF
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        WAIT_FRAMES(50)
        CALL(btlevtcmd_OnAttribute, LW(3), kShellShielded)
        CALL(btlevtcmd_GetUnitId, LW(3), LW(0))
        CALL(btlevtcmd_SetUnitWork, LW(10), SHELL_SHIELD_WORK_TARGET, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        CALL(btlevtcmd_SetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, 3)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            SET_VALUE(LW(5), 5)
            LOOP(LW(5))
                CALL(btlevtcmd_GetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, LW(10))
                IF_INT_NOT_EQUAL(LW(0), 3)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
                SET_VALUE(LW(5), 5)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(btlevtcmd_SetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, 0)
        IF_BITS_SET(LW(6), 2)
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
        ELSE
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
        END_IF
        CALL(btlevtcmd_SetHp, LW(10), LW(0))
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_POSE_TABLE_RESET_EVENT)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), 1, POSE_TYPE_NEUTRAL)
        CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kHidden)
        CALL(btlevtcmd_OnAttribute, LW(3), (kShellShielded | kUnitNeverTargetable))
        CALL(btlevtcmd_StartWaitEvent, LW(3))
        CALL(_check_mario_move_count, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
            CALL(evt_btl_camera_off_posoffset_manual, 0)
            WAIT_MS(500)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_KOURA_OPEN_EVENT)
            WAIT_FRAMES(60)
        END_IF
        CALL(btlevtcmd_ResetFaceDirection, -2)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_off_posoffset_manual, 0)
        RETURN
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
        WAIT_FRAMES(35)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_B_1"))
        WAIT_FRAMES(150)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_NOKO_GUARD2"), EVT_NULLPTR, 0, LW(5))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_FallPosition, LW(10), LW(0), LW(1), LW(2), 60)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxoff, LW(5))
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("PKO_F_1"))
    WAIT_FRAMES(50)
    CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("PKO_H_1"))
    CALL(btlevtcmd_AnimeChangePose, LW(10), 2, STRING("PKO_H_2"))
    CALL(btlevtcmd_OffPartsAttribute, LW(10), 2, kHidden)
    CALL(btlevtcmd_SetPartsRotateOffset, LW(10), 1, 20, 0, 0)
    CALL(btlevtcmd_SetPartsRotateOffset, LW(10), 2, -20, 0, 0)
    LOOP(20)
        CALL(btlevtcmd_AddPartsRotate, LW(10), 1, 0, 0, 2)
        CALL(btlevtcmd_AddPartsRotate, LW(10), 2, 0, 0, -2)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(10)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 2, kHidden)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 2, kHidden)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(btlevtcmd_KillUnit, LW(10), 0)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_off_posoffset_manual, 0)
    CALL(_color_lv_set, UNIT_ID_MARIO, 1, LW(14))
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyNokotarouAttack_TsuranukiKoura)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 25)
        SET_VALUE(LW(1), 15)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 24)
        SET_VALUE(LW(1), 21)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 22)
        SET_VALUE(LW(1), 24)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 20)
        SET_VALUE(LW(1), 25)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 17)
        SET_VALUE(LW(1), 27)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 15)
        SET_VALUE(LW(1), 30)
        CASE_DEFAULT
        SET_VALUE(LW(0), 12)
        SET_VALUE(LW(1), 30)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 7, 200, 178, LW(0), LW(1), 20, 61, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 60, 80, 100, 100)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGE_SFX, -1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LABEL(5)
        CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(5)
        END_IF
        CALL_CHILD(_change_koura_pose_fast)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_IPPATSU1"), LW(13))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGE_SFX, LW(13))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
        SET_VALUE(LW(0), 15)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 15)
        CALL_CHILD(_koura_rotate_start)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(_tsuranuki_effect_control, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_StartAC, 1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SWITCH(LW(0))
                CASE_LESS_OR_EQUAL(1)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_CHARGE_LEVEL, 0)
                CASE_EQUAL(2)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_CHARGE_LEVEL, 1)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 24)
                CASE_EQUAL(3)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_CHARGE_LEVEL, 2)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 36)
                CASE_EQUAL(4)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_CHARGE_LEVEL, 3)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
            END_SWITCH
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGE_SFX, LW(13))
            IF_INT_NOT_EQUAL(LW(13), -1)
                CALL(btlevtcmd_AcGetOutputParam, 1, LW(1))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(1))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_BASE, LW(0))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, LW(1))
                MULTIPLY(LW(0), LW(1))
                DIVIDE(LW(0), 100)
                CALL(evt_snd_sfx_pit, LW(13), LW(0))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
        LOOP(10)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 12, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
        LOOP(16)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_H_1"))
        LOOP(14)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ACROBAT_SHAKE1"), 0)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_EFFECT_END, 1)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_SPIN_DUST, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGE_SFX, LW(13))
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_IPPATSU2"), 0)
    CALL(btlevtcmd_StopAC)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SFX_PITCH_RATE, 0)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_SHUBIBIN2"), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        GOTO(50)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(btlevtcmd_CommandPreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), 256, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_EFFECT_END, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL_CHILD(_koura_rotate_stop)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_AcGetOutputParam, 2, LW(9))
        IF_INT_GREATER_OR_EQUAL(LW(9), 4)
            SUBTRACT(LW(9), 4)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(9), LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), LW(6))
        ELSE
            CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
        END_IF
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(0), -1)
    END_IF
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 400, LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, PARTS_WORK_EFFECT_END, 1)
    WAIT_MS(100)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 9, 9, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
#endif
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 12)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_1"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 49, 79, 79, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        IF_INT_NOT_EQUAL(LW(6), 2)
            CALL(evt_audience_acrobat_notry)
            CALL_CHILD(_koura_rotate_stop)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_2"))
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
            END_LOOP
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_S_1"))
            LOOP(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
            END_LOOP
            GOTO(99)
        END_IF
        CALL_CHILD(_koura_rotate_stop)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_2"))
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_E_3"))
        LOOP(6)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
        END_LOOP
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        WAIT_FRAMES(60)
        GOTO(99)
    ELSE
        CALL(evt_audience_acrobat_notry)
    END_IF
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(nokotarou_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_APPEAL)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(58)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FLIPPED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_Y_1"))
    END_IF
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
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(8), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_FRAMES(20)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_AUDIENCE_MEMBER)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(0), 15)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHELL_SPIN_SPEED, 45)
        CALL_CHILD(_koura_rotate_start)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_NOKO_ATTACK2"), 0)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 15)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(6), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(nokotarou_revive_event)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_stay))
    RETURN
EVT_END()

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static BattleUnitDefense defence_turn = {0};

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

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

BattleUnitSetup entry_koura = {
    &unit_koura, // mUnitKindParams
    ALLIANCE_PLAYER, // alliance
    {0}, // pad_05
    0x4000001, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_NokotarouKouraAttack;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = ICON_PARTNER_MOVE_NORMAL;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_NokotarouSyubibinKoura;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = ICON_PARTNER_MOVE_LV1;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_NokotarouKouraGuard;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_NokotarouTsuranukiKoura;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

USER_FUNC(_check_mario_move_count) {
    s32 index = event->args[0];
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);

    evtSetValue(event, index, mario->movesRemaining);
    return EVT_RETURN_DONE;
}

USER_FUNC(_color_lv_set) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, index, set, value;

    id = evtGetValue(event, args[0]);
    set = evtGetValue(event, args[1]);
    index = args[2];
    value = evtGetValue(event, index);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (set) {
        unit->mMoveColorLv = value;
    } else {
        evtSetValue(event, index, unit->mMoveColorLv);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_tsuranuki_effect_control) {
    BattleWork* wp;
    BattleUnit* unit;
    BattleUnitPart* part;
    EffectEntry* effect;
    Vec pos;
    s32 id, target;

    id = evtGetValue(event, event->args[0]);
    wp = _battleWorkPointer;
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    part = BtlUnit_GetPartsPtr(unit, 1);
    if (isFirstCall) {
        event->userdatap[0] = effNokotarouEntry(0, 1000, pos.x, pos.y, pos.z, unit->mSizeMultiplier);
        part->work[PARTS_WORK_EFFECT_END] = 0;
        part->work[PARTS_WORK_CHARGE_LEVEL] = 0;
        event->userdata[1] = 0;
    }
    target = part->work[PARTS_WORK_CHARGE_LEVEL] * 100;
    if (event->userdata[1] != target) {
        if (event->userdata[1] < target) {
            event->userdata[1] += 10;
            if (event->userdata[1] > target) {
                event->userdata[1] = target;
            }
        } else {
            event->userdata[1] -= 10;
            if (event->userdata[1] < target) {
                event->userdata[1] = target;
            }
        }
    }
    {
        f32 size = (2.0f + 0.006f * event->userdata[1]);

        size *= unit->mSizeMultiplier;
        effect = event->userdatap[0];
        ((EffNokotarouWork*)effect->userdata)->position.x = pos.x;
        ((EffNokotarouWork*)effect->userdata)->position.y = pos.y;
        ((EffNokotarouWork*)effect->userdata)->position.z = pos.z;
        ((EffNokotarouWork*)effect->userdata)->size = size;
    }
    if (part->work[PARTS_WORK_EFFECT_END]) {
        part->work[PARTS_WORK_EFFECT_END] = 0;
        effSoftDelete(effect);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(_check_guard_koura) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnit* koura;
    s32 id, index;

    id = evtGetValue(event, args[0]);
    index = args[1];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (!(unit->mAttributeFlags & kShellShielded)) {
        evtSetValue(event, index, -1);
        return EVT_RETURN_DONE;
    }
    koura = BtlUnit_GetGuardKouraPtr(unit);
    if (!koura) {
        evtSetValue(event, index, -1);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, index, koura->unitId);
    return EVT_RETURN_DONE;
}
