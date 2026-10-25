#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_stage_object.h"
#include "system.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"

EffectEntry* effFallEntry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 drop, f32 dir, s32 num, s32 duration);
BOOL effFallCheckHit(EffectEntry* effect);

USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_CheckAttribute);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_DrawLots);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetDamageCode);
USER_FUNC(btlevtcmd_GetTotalDamage);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventCeilFall);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetRunSound);
USER_FUNC(btlevtcmd_SetStayPose);
USER_FUNC(btlevtcmd_SetStayPoseType);
USER_FUNC(btlevtcmd_SetTalkPose);
USER_FUNC(btlevtcmd_SetTalkPoseType);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SetWalkSound);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_StopWaitEvent);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(check_ceil);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveto);
USER_FUNC(evt_btl_camera_set_prilimit);
USER_FUNC(evt_btl_camera_wait_move_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_battle_party);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(init_ceil);
USER_FUNC(eff_fall_hit_wait);
USER_FUNC(eff_fall_entry);
static USER_FUNC(down_ceil);
USER_FUNC(ceil_exist);

BattleUnitKind unit_boss_champion;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
PoseTableEntry pose_table_roof[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_champion_drop_kick_attack;
BattleWeapon weapon_champion_screw_attack;
BattleWeapon weapon_champion_flying_chop_attack;
BattleWeapon weapon_champion_foot_stamp_attack;
BattleWeapon weapon_champion_healing;
BattleWeapon weapon_champion_roof_shake_attack;
BattleWeapon weapon_champion_roof_shake_last_attack;
EvtWord to_ceiling[103];
EvtWord to_floor[95];
static EvtWord init_event[52];
static EvtWord entry_event[354];
static EvtWord damage_event[75];
static EvtWord ceil_fall_ready_event[8];
static EvtWord dead_event[90];
static EvtWord phase_event[450];
static EvtWord attack_event[221];
EvtWord drop_kick_attack_event[299];
EvtWord screw_attack_event[295];
EvtWord flying_chop_attack_event[326];
EvtWord foot_stamp_attack_event[467];
EvtWord healing_event[209];
EvtWord roof_shake_attack_event[386];
static EvtWord wait_event[6];

USER_FUNC(ceil_exist) {
    EvtWord* args = event->args;

    if (_battleWorkPointer->stageObjectWork.flags & 0x20) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], 1);
    }
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_boss_champion = {
    64,
    "btl_un_champion",
    40,
    0,
    1,
    1,
    34,
    0,
    0,
    100,
    0,
    50,
#ifdef VERSION_G8ME01
    7,
#else
    8,
#endif
    50,
    64,
    3,
    64,
    0.0f,
    32.0f,
    0.0f,
    0,
    0,
    8.0f,
    36.0f,
    0.0f,
    25.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    10.0f,
    48.0f,
    10.0f,
    0.0f,
    32.0f,
    0.0f,
    50.0f,
    64.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_BLED_DAMAGED1",
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {60, 50, 60, 100, 60, 100, 100, 40, 0, 85, 100, 85, 100, 90, 60, 0, 0, 100, 70, 100, 100, 0};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "K_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "D_1"},
    {39, "D_1"},
    {50, "Z_6"},
    {42, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

PoseTableEntry pose_table_roof[] = {
    {1, "N_2"},
    {2, "Y_2"},
    {9, "Y_2"},
    {5, "K_2"},
    {4, "X_2"},
    {3, "K_2"},
    {28, "S_8"},
    {29, "Q_2"},
    {30, "Q_2"},
    {31, "D_2"},
    {39, "D_2"},
    {50, "Z_8"},
    {42, "W_2"},
    {40, "W_2"},
    {56, "X_2"},
    {57, "X_2"},
    {65, "T_8"},
    {69, "S_8"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_champion", "c_bred_h", 0.0f, 0.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 55.0f, 0.0f, 35, 70, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

USER_FUNC(init_ceil) {
    u8* ceil = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body = BattleSearchObjectPtr("if_body");
    BattleStageObject* wire = BattleSearchObjectPtr("if_wire");

    wire->mPosition = (Vec){0.0f, 250.0f, 0.0f};
    body->mPosition = wire->mPosition;
    *ceil |= 0x20;
    return EVT_RETURN_DONE;
}

BattleWeapon weapon_champion_drop_kick_attack = {
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
    0x1101260, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x1087, // special
    0, // counterResistFlags
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

BattleWeapon weapon_champion_screw_attack = {
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
    0x2101260, // targetClassFlags
    0x20001000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x1087, // special
    0, // counterResistFlags
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

BattleWeapon weapon_champion_flying_chop_attack = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x20001000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x1087, // special
    0, // counterResistFlags
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

BattleWeapon weapon_champion_foot_stamp_attack = {
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
    0x1101260, // targetClassFlags
    0x20001000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x1087, // special
    0, // counterResistFlags
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

BattleWeapon weapon_champion_healing = {
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
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1100160, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0, // counterResistFlags
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

BattleWeapon weapon_champion_roof_shake_attack = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
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

BattleWeapon weapon_champion_roof_shake_last_attack = {
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
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, // element
    27, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20087, // special
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
    {0, 25, 25, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(to_ceiling)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1D"))
    SET_VALUE(LW(1), 60)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_8"))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), -250000000, LW(2))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_JUMP1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_GRASP_CAILING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_roof))
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_OnAttribute, -2, 2)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 6291456)
    CALL(btlevtcmd_SetKissHitOffset, -2, 16, 48, 10)
    CALL(btlevtcmd_SetCutBaseOffset, -2, -2, -38, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, -44, 0)
    RETURN
EVT_END()

EVT_BEGIN(to_floor)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_2"))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), -250000000, LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
    CALL(btlevtcmd_OffAttribute, -2, 2)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 6291456)
    CALL(btlevtcmd_SetKissHitOffset, -2, 10, 48, 10)
    CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 32, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(init_ceil)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_BLED_MOVE1L"), STRING("SFX_BOSS_BLED_MOVE1R"), 0, 15, 15)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_BLED_MOVE1L"), STRING("SFX_BOSS_BLED_MOVE1R"), 0, 8, 8)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    IF_INT_LESS(GSW(0), 158)
        IF_INT_EQUAL(GSWF(2513), 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_6"))
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_6"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_6"))
            WAIT_FRAMES(120)
            SET_VALUE(GSWF(2513), 1)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, -102, 83, 334, -102, 59, 0, 30, 11)
            WAIT_FRAMES(37)
            CALL(evt_msg_print_battle_party, STRING("stg3_tou_432"))
            WAIT_MS(300)
            CALL(evt_btl_camera_set_moveto, 0, 81, 83, 334, 81, 59, 0, 30, 11)
            WAIT_FRAMES(37)
            CALL(evt_msg_print, 2, STRING("stg3_tou_433"), 0, -2)
            WAIT_MS(200)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(200)
            CALL(evt_msg_print_battle_party, STRING("stg3_tou_434"))
            WAIT_MS(200)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, 81, 83, 334, 81, 59, 0, 1, 11)
            WAIT_FRAMES(7)
            CALL(evt_msg_print, 2, STRING("stg3_tou_435"), 0, -2)
            WAIT_MS(200)
            CALL(evt_btl_camera_set_moveto, 0, -111, 67, 275, -111, 49, 0, 1, 11)
            WAIT_FRAMES(7)
            CALL(evt_msg_print_battle_party, STRING("stg3_tou_435_01"))
            WAIT_MS(200)
            CALL(evt_btl_camera_set_moveto, 0, 74, 67, 275, 74, 49, 0, 30, 11)
            WAIT_FRAMES(37)
            CALL(evt_msg_print, 2, STRING("stg3_tou_435_02"), 0, -2)
            WAIT_MS(200)
            ELSE
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
            WAIT_FRAMES(60)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, -102, 83, 334, -102, 59, 0, 30, 11)
            WAIT_FRAMES(37)
            CALL(evt_msg_print_battle_party, STRING("stg3_tou_436"))
            WAIT_MS(200)
            CALL(evt_btl_camera_set_moveto, 0, 81, 83, 334, 81, 59, 0, 30, 11)
            WAIT_FRAMES(37)
            CALL(evt_msg_print, 2, STRING("stg3_tou_436_01"), 0, -2)
            WAIT_MS(200)
        END_IF
        ELSE
        IF_INT_LESS(GSW(0), 172)
            ELSE
            SET_VALUE(GSWF(2514), 0)
            DEBUG_PUT_MSG(STRING("チャンピオン　再挑戦"))
            DEBUG_PUT_REG(GSWF(2515))
            IF_INT_EQUAL(GSWF(2503), 0)
                WAIT_FRAMES(60)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
                WAIT_FRAMES(60)
                CALL(evt_btl_camera_set_mode, 0, 3)
                CALL(evt_btl_camera_set_moveto, 0, -102, 83, 334, -102, 59, 0, 30, 11)
                WAIT_FRAMES(37)
                CALL(evt_msg_print_battle_party, STRING("stg3_tou_re_116"))
                WAIT_MS(200)
                CALL(evt_btl_camera_set_moveto, 0, 81, 83, 334, 81, 59, 0, 30, 11)
                WAIT_FRAMES(37)
                CALL(evt_msg_print, 2, STRING("stg3_tou_re_123"), 0, -2)
                WAIT_MS(200)
                ELSE
                WAIT_FRAMES(60)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
                WAIT_FRAMES(60)
                CALL(evt_btl_camera_set_mode, 0, 3)
                CALL(evt_btl_camera_set_moveto, 0, -102, 83, 334, -102, 59, 0, 30, 11)
                WAIT_FRAMES(37)
                CALL(evt_msg_print_battle_party, STRING("stg3_tou_re_124"))
                WAIT_MS(200)
                CALL(evt_btl_camera_set_moveto, 0, 81, 83, 334, 81, 59, 0, 30, 11)
                WAIT_FRAMES(37)
                CALL(evt_msg_print, 2, STRING("stg3_tou_re_131"), 0, -2)
                WAIT_MS(200)
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_GetBodyId, -4, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, -4, LW(0))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(2), 0)
        CALL(btlevtcmd_GetDamageCode, -2, LW(0))
        SWITCH(LW(0))
            CASE_OR_EQUAL(23)
            CASE_OR_EQUAL(24)
            CASE_OR_EQUAL(25)
            CASE_OR_EQUAL(26)
            CASE_OR_EQUAL(27)
            CALL(btlevtcmd_GetTotalDamage, -2, LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 1)
                SET_VALUE(LW(2), 1)
            END_IF
            CASE_END
        END_SWITCH
        CALL(btlevtcmd_CheckStatus, -2, 11, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(2), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL_CHILD(PTR(to_floor))
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_SetEventCeilFall, -2, 0)
    CALL_CHILD(PTR(to_floor))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(to_floor))
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_3"))
    WAIT_MS(200)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_BLED_DIE1"), 0)
    WAIT_MS(100)
    IF_INT_LESS(GSW(0), 158)
        CALL(btlevtcmd_WaitAttackEnd)
        CALL(evt_btl_camera_set_prilimit, 1)
        CALL(evt_btl_camera_wait_move_end)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 81, 83, 334, 81, 59, 0, 30, 11)
        WAIT_FRAMES(37)
        CALL(btlevtcmd_SetTalkPose, -2, STRING("T_10"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("S_10"))
        CALL(evt_msg_print, 2, STRING("stg3_tou_436_06"), 0, -2)
        WAIT_MS(300)
        CALL(evt_btl_camera_set_mode, 1, 0)
        WAIT_MS(300)
        CALL(evt_btl_camera_set_prilimit, 0)
        CALL(btlevtcmd_StatusWindowOnOff, 1)
    END_IF
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        DEBUG_PUT_REG(LW(0))
        MULTIPLY(LW(0), 67)
        DEBUG_PUT_REG(LW(0))
        DIVIDE(LW(0), 100)
        DEBUG_PUT_REG(LW(0))
        CALL(btlevtcmd_GetHp, -2, LW(1))
        DEBUG_PUT_REG(LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
            IF_INT_EQUAL(GSWF(2514), 0)
                SET_VALUE(GSWF(2514), 1)
                CALL(btlevtcmd_StopWaitEvent, -2)
                IF_INT_LESS(GSW(0), 158)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_btl_camera_set_mode, 0, 3)
                    CALL(evt_btl_camera_set_moveto, 0, 73, 78, 322, 73, 57, 0, 30, 11)
                    WAIT_FRAMES(37)
                    CALL(evt_msg_print, 2, STRING("stg3_tou_436_02"), 0, -2)
                ELSE
                    CALL(evt_btl_camera_set_mode, 0, 3)
                    CALL(evt_btl_camera_set_moveto, 0, 73, 78, 322, 73, 57, 0, 30, 11)
                    WAIT_FRAMES(37)
                END_IF
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_2"))
                WAIT_MS(200)
                CALL(evt_btl_camera_set_moveto, 0, 73, 82, 379, 73, 57, 0, 30, 11)
                CALL(evt_env_blur_on, 0, 500)
                WAIT_FRAMES(37)
                IF_INT_LESS(GSW(0), 158)
                    CALL(btlevtcmd_SetTalkPose, -2, STRING("T_2"))
                    CALL(btlevtcmd_SetStayPose, -2, STRING("S_2"))
                    CALL(evt_msg_print, 2, STRING("stg3_tou_436_03"), 0, -2)
                    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
                    CALL(btlevtcmd_SetStayPoseType, -2, 43)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                END_IF
                CALL(evt_btl_camera_set_mode, 0, 0)
            END_IF
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        DEBUG_PUT_REG(LW(0))
        MULTIPLY(LW(0), 34)
        DEBUG_PUT_REG(LW(0))
        DIVIDE(LW(0), 100)
        DEBUG_PUT_REG(LW(0))
        CALL(btlevtcmd_GetHp, -2, LW(1))
        DEBUG_PUT_REG(LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            SET_VALUE(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, -2, 11, LW(1))
            IF_INT_EQUAL(LW(1), 0)
                CALL(btlevtcmd_SetUnitWork, -2, 0, 3)
                IF_INT_LESS(GSW(0), 158)
                    IF_INT_EQUAL(GSWF(2515), 0)
                        SET_VALUE(GSWF(2515), 1)
                        CALL(btlevtcmd_StatusWindowOnOff, 0)
                        CALL(evt_btl_camera_set_mode, 0, 3)
                        CALL(evt_btl_camera_set_moveto, 0, 73, 78, 322, 73, 57, 0, 30, 11)
                        WAIT_FRAMES(37)
                        CALL(evt_msg_print, 2, STRING("stg3_tou_436_04"), 0, -2)
                        WAIT_MS(300)
                        CALL(btlevtcmd_StatusWindowOnOff, 1)
                        CALL(evt_btl_camera_set_mode, 0, 0)
                    END_IF
                END_IF
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
                WAIT_MS(200)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3C"))
                CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_JUMP1"), -250000000, 0, -250000000)
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 25, -1)
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
                CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
                BEGIN_INLINE_EVENT
                    WAIT_MS(600)
                    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
                END_INLINE_EVENT
                CALL(evt_snd_sfxon, STRING("SFX_BOSS_BLED_MOVE_CAILING1"), LW(15))
                CALL(down_ceil)
                CALL(evt_snd_sfxoff, LW(15))
                CALL(btlevtcmd_SetEventCeilFall, -2, PTR(ceil_fall_ready_event))
            END_IF
        END_IF
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        DEBUG_PUT_REG(LW(0))
        MULTIPLY(LW(0), 25)
        DEBUG_PUT_REG(LW(0))
        DIVIDE(LW(0), 100)
        DEBUG_PUT_REG(LW(0))
        CALL(btlevtcmd_GetHp, -2, LW(1))
        DEBUG_PUT_REG(LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            IF_INT_LESS(GSW(0), 158)
                IF_INT_EQUAL(GSWF(2516), 0)
                    SET_VALUE(GSWF(2516), 1)
                    CALL(btlevtcmd_StopWaitEvent, -2)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL(evt_btl_camera_set_mode, 0, 3)
                    CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(evt_btl_camera_set_moveto, 0, 73, 78, 322, 73, 57, 0, 30, 11)
                    ELSE
                        CALL(evt_btl_camera_set_moveto, 0, 73, 138, 322, 73, 117, 0, 30, 11)
                    END_IF
                    WAIT_FRAMES(37)
                    CALL(evt_msg_print, 2, STRING("stg3_tou_436_05"), 0, -2)
                    WAIT_MS(300)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, 0, 0)
                    CALL(btlevtcmd_StartWaitEvent, -2)
                END_IF
            END_IF
        END_IF
    END_SWITCH
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
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_DrawLots, LW(0), 2, 10, 15)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_champion_drop_kick_attack))
            CALL_CHILD(PTR(drop_kick_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_champion_screw_attack))
            CALL_CHILD(PTR(screw_attack_event))
        END_SWITCH
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, -2, 0, 2)
        SET_VALUE(LW(9), PTR(&weapon_champion_flying_chop_attack))
        CALL_CHILD(PTR(flying_chop_attack_event))
        CASE_EQUAL(2)
        CALL(btlevtcmd_DrawLots, LW(0), 3, 10, 15, 20)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_champion_drop_kick_attack))
            CALL_CHILD(PTR(drop_kick_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_champion_screw_attack))
            CALL_CHILD(PTR(screw_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_champion_flying_chop_attack))
            CALL_CHILD(PTR(flying_chop_attack_event))
        END_SWITCH
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetUnitWork, -2, 0, 4)
        CALL_CHILD(PTR(to_ceiling))
        SET_VALUE(LW(9), PTR(&weapon_champion_roof_shake_attack))
        CALL_CHILD(PTR(roof_shake_attack_event))
        CASE_EQUAL(4)
        CALL(btlevtcmd_CheckAttribute, -2, 2, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(2), 30)
            CALL(btlevtcmd_CheckStatus, -2, 11, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                SET_VALUE(LW(2), 0)
            END_IF
            CALL(check_ceil)
            IF_INT_NOT_EQUAL(LW(1), 0)
                SET_VALUE(LW(2), 0)
            END_IF
            CALL(btlevtcmd_DrawLots, LW(0), 4, 10, 15, 20, LW(2))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                SET_VALUE(LW(9), PTR(&weapon_champion_drop_kick_attack))
                CALL_CHILD(PTR(drop_kick_attack_event))
                CASE_EQUAL(1)
                SET_VALUE(LW(9), PTR(&weapon_champion_screw_attack))
                CALL_CHILD(PTR(screw_attack_event))
                CASE_EQUAL(2)
                SET_VALUE(LW(9), PTR(&weapon_champion_flying_chop_attack))
                CALL_CHILD(PTR(flying_chop_attack_event))
                CASE_EQUAL(3)
                CALL_CHILD(PTR(to_ceiling))
                SET_VALUE(LW(9), PTR(&weapon_champion_roof_shake_attack))
                CALL_CHILD(PTR(roof_shake_attack_event))
            END_SWITCH
            ELSE
            CALL(btlevtcmd_DrawLots, LW(0), 3, 10, 20, 20)
            SWITCH(LW(0))
                CASE_EQUAL(0)
                SET_VALUE(LW(9), PTR(&weapon_champion_foot_stamp_attack))
                CALL_CHILD(PTR(foot_stamp_attack_event))
                CASE_EQUAL(1)
                SET_VALUE(LW(9), PTR(&weapon_champion_healing))
                CALL_CHILD(PTR(healing_event))
                CASE_EQUAL(2)
                SET_VALUE(LW(9), PTR(&weapon_champion_roof_shake_attack))
                CALL_CHILD(PTR(roof_shake_attack_event))
            END_SWITCH
        END_IF
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(drop_kick_attack_event)
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
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(0), 150)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2A"))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.009765625f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_DROP_KICK1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2B"))
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    WAIT_MS(60)
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_ROUND1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(3.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1C"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    RETURN
EVT_END()

EVT_BEGIN(screw_attack_event)
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
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1F"))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(5.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_ROUND1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
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
    CALL(btlevtcmd_JumpContinue, -2)
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(1.5f), FLOAT(0.69921875f))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_ROUND1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(3.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1C"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    RETURN
EVT_END()

EVT_BEGIN(flying_chop_attack_event)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.14941406f))
    ADD(LW(0), 82)
    ADD(LW(1), 80)
    ADD(LW(2), 140)
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    MULTIPLY_FLOAT(LW(5), 30)
    SUBTRACT(LW(0), LW(5))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_ROUND1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(10.0f), FLOAT(0.009765625f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    ADD(LW(1), 5)
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_CHOP_FLY1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1C"))
    CALL(btlevtcmd_JumpContinue, -2)
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    WAIT_MS(60)
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(3.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1C"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    RETURN
EVT_END()

EVT_BEGIN(foot_stamp_attack_event)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("W_2"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, -250000000, LW(1), -250000000)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("B_1"))
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3A"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.75f))
    ADD(LW(1), 20)
    ADD(LW(2), 5)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_FUMI_FALL1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_SetTalkPoseType, -2, 65)
    CALL(btlevtcmd_SetStayPoseType, -2, 43)
    CALL(btlevtcmd_OffAttribute, -2, 2)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 6291456)
    CALL(btlevtcmd_SetKissHitOffset, -2, 10, 48, 10)
    CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 32, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_FUMI_STEPON1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
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
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3C"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_FUMI_STEPON1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3C"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 35, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_FUMI_STEPON1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 14, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    WAIT_MS(300)
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(3.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
    WAIT_MS(250)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1C"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(4.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_LANDING1"), -250000000, 0, -250000000)
    RETURN
EVT_END()

EVT_BEGIN(healing_event)
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_4"))
    WAIT_FRAMES(16)
    LOOP(3)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_SHAKE_CAILING1"), -250000000, 0, -250000000)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        WAIT_FRAMES(18)
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
        ADD(LW(5), -30)
        ADD(LW(6), 80)
        ADD(LW(7), 40)
        CALL(btlevtcmd_BtlIconEntryItemId, 148, LW(5), LW(6), LW(7), LW(8))
        SET_VALUE(LW(0), LW(6))
        ADD(LW(0), -62)
        CALL(evt_sub_intpl_msec_init, 1, LW(6), LW(0), 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(btlevtcmd_BtlIconSetPosition, LW(8), LW(5), LW(0), LW(7))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_BtlIconDelete, LW(8))
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_8"))
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_EAT1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("B_2"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_2"))
    WAIT_MS(700)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 5)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 5, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static USER_FUNC(down_ceil) {
    u8* ceil = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body = BattleSearchObjectPtr("if_body");
    BattleStageObject* wire = BattleSearchObjectPtr("if_wire");

    if (isFirstCall) {
        event->userdata[0] = 120;
        *ceil &= ~0x20;
    }
    body->mPosition.y = wire->mPosition.y = intplGetValue(4, 120 - event->userdata[0], 120, 250.0f, 130.0f);
    event->userdata[0]--;
    if (event->userdata[0] < 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(check_ceil) {
    event->lwData[1] = _battleWorkPointer->stageObjectWork.flags & 0x20;
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_fall_entry) {
    EvtWord* args = event->args;
    s32 var = args[0];
    s32 type = evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    f32 a = evtGetFloat(event, args[5]);
    f32 b = evtGetFloat(event, args[6]);
    f32 c = evtGetFloat(event, args[7]);
    s32 d = evtGetValue(event, args[8]);
    s32 e = evtGetValue(event, args[9]);
    EffectEntry* eff = effFallEntry(type, x, y, z, a, b, c, d, e);

    evtSetValue(event, var, (EvtWord)eff);
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_fall_hit_wait) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    if (eff != NULL && !effFallCheckHit(eff)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(roof_shake_attack_event)
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 0, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 1, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 2, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 3, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 4, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 5, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 6, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 7, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 8, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 9, 0)
    SET_VALUE(LW(13), 0)
    LOOP(10)
        SET_VALUE(LW(0), 0)
        ADD(LW(0), LW(13))
        CALL(btlevtcmd_SetPartsWork, -2, 1, LW(0), 0)
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
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_4"))
        WAIT_FRAMES(16)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_BLED_SHAKE_CAILING1"), -250000000, 0, -250000000)
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 10)
        SET_VALUE(LW(8), LW(1))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 1)
            SET_VALUE(LW(8), 0)
        END_IF
        CALL(evt_sub_random, 2, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(eff_fall_entry, LW(14), 4, LW(0), LW(1), LW(2), 20, LW(8), 0, 1, 60)
            CASE_EQUAL(1)
            CALL(eff_fall_entry, LW(14), 5, LW(0), LW(1), LW(2), 20, LW(8), 0, 1, 60)
            CASE_EQUAL(2)
            CALL(eff_fall_entry, LW(14), 8, LW(0), LW(1), LW(2), 20, LW(8), 0, 1, 60)
        END_SWITCH
        BEGIN_INLINE_EVENT_GET_ID(LW(15))
            SWITCH(LW(5))
                CASE_EQUAL(1)
                CALL(eff_fall_hit_wait, LW(14))
                CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
                CASE_EQUAL(3)
                CALL(eff_fall_hit_wait, LW(14))
                CALL(btlevtcmd_StartAvoid, LW(3), 38)
                CASE_EQUAL(6)
                WAIT_FRAMES(10)
                CALL(btlevtcmd_StartAvoid, LW(3), 39)
                CASE_EQUAL(2)
                CALL(eff_fall_hit_wait, LW(14))
                CALL(btlevtcmd_StartAvoid, LW(3), 40)
                CASE_DEFAULT
                CALL(eff_fall_hit_wait, LW(14))
            END_SWITCH
        END_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        ADD(LW(0), LW(13))
        CALL(btlevtcmd_SetPartsWork, -2, 1, LW(0), LW(15))
        WAIT_FRAMES(4)
        LABEL(90)
        ADD(LW(13), 1)
    END_LOOP
    WAIT_FRAMES(10)
    LABEL(92)
    WAIT_FRAMES(1)
    SET_VALUE(LW(13), 0)
    LABEL(93)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), LW(13))
    CALL(btlevtcmd_GetPartsWork, -2, 1, LW(0), LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(btlevtcmd_SetPartsWork, -2, 1, LW(0), 0)
        GOTO(92)
    END_IF
    ADD(LW(13), 1)
    IF_INT_LESS(LW(13), 10)
        GOTO(93)
    END_IF
    CALL(btlevtcmd_GetGuardKouraId, -3, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_CheckDamage, -2, LW(0), 1, PTR(&weapon_champion_roof_shake_last_attack), 256, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, -3, 1, PTR(&weapon_champion_roof_shake_last_attack), 256, LW(5))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, -4, 1, PTR(&weapon_champion_roof_shake_last_attack), 256, LW(5))
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

