#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_vivian.h"
#include "battle/battle_database_common.h"
#include "evt/evt_msg.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
extern EvtWord btl_marilyn_zoom[];
extern EvtWord majo3_shadow_tail_event[];
extern EvtWord majo_unison_phase_event[];

BattleUnitKind unit_boss_marilyn_las;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_marilyn_kagenuke_attack1;
static BattleWeapon weapon_marilyn_thunder_attack1;
static BattleWeapon weapon_marilyn_charge1;
static EvtWord marilyn_down[19];
static EvtWord marilyn_up[27];
static EvtWord init_event[37];
static EvtWord damage_event[22];
static EvtWord dead_event[152];
static EvtWord unison_phase_event[83];
static EvtWord phase_event[2];
static EvtWord attack_event[93];
#ifdef VERSION_G8ME01
static EvtWord kagenuke_attack_event[241];
#else
static EvtWord kagenuke_attack_event[238];
#endif
static EvtWord thunder_attack_event[235];
static EvtWord charge_attack_event[58];
#ifdef VERSION_G8ME01
static EvtWord wait_event[7];
#else
static EvtWord wait_event[6];
#endif

BattleUnitKind unit_boss_marilyn_las = {
    UNIT_MARILYN_CHAPTER8, // mUnitType
    "btl_un_marilyn4", // name
    40, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    57, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    7, // pbSoftCap
    40, // width
    54, // height
    {0, 54}, // hitOffset
    {0.0f, 27.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {6.0f, 24.0f, 0.0f}, // talkTogeBaseOffset
    {20.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {20.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 35.1f, 0.0f}, // kissHitOffset
    {0.0f, 27.0f, 0.0f}, // cutBaseOffset
    40.0f, // cutWidth
    54.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MARI_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    50, // sleep
    50, // stop
    50, // dizzy
    100, // poison
    50, // confuse
    100, // electric
    100, // burn
    40, // freeze
    100, // huge
    60, // tiny
    100, // attackUp
    60, // attackDown
    100, // defenseUp
    80, // defenseDown
    40, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    5, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "MRR_N_1"},
    {POSE_TYPE_STOP, "MRR_Z_1"},
    {POSE_TYPE_FREEZE, "MRR_Z_1"},
    {POSE_TYPE_CONFUSE, "MRR_X_1"},
    {POSE_TYPE_POISON, "MRR_X_1"},
    {POSE_TYPE_DIZZY, "MRR_X_1"},
    {POSE_TYPE_NORMAL, "MRR_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "MRR_Q_2"},
    {30, "MRR_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "MRR_D_1"},
    {POSE_TYPE_DAMAGE, "MRR_D_1"},
    {POSE_TYPE_ATTACK, "MRR_S_1"},
    {POSE_TYPE_RUN, "MRR_R_1"},
    {POSE_TYPE_WALK, "MRR_W_1"},
    {POSE_TYPE_DANGER, "MRR_S_1"},
    {POSE_TYPE_PERIL, "MRR_S_1"},
    {POSE_TYPE_TALK, "MRR_T_1"},
    {POSE_TYPE_ACTION, "MRR_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_marilyn4", "c_maririn", 0.0f, 0.0f, 0.0f, 15.0f, 45.0f, 0.0f, 0.0f, 50.0f, 0.0f, 38, 56, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

static BattleWeapon weapon_marilyn_kagenuke_attack1 = {
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
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static BattleWeapon weapon_marilyn_thunder_attack1 = {
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
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static BattleWeapon weapon_marilyn_charge1 = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    7, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(marilyn_down)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_SHADOW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(marilyn_up)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_SHADOW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    SPAWN_EVENT(PTR(majo3_shadow_tail_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(12), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    SWITCH(GW(11))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        LOOP(0)
            IF_INT_EQUAL(GW(11), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        GOTO(99)
        CASE_EQUAL(2)
        SWITCH(GW(13))
            CASE_EQUAL(0)
            CASE_DEFAULT
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL_CHILD(PTR(btl_marilyn_zoom))
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MRR_D_1"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MRR_D_1"))
            CALL(evt_msg_print, 2, STRING("btl_majo3_12"), 0, -2)
            GOTO(90)
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL_CHILD(PTR(btl_marilyn_zoom))
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MRR_D_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MRR_D_1"))
    CALL(evt_msg_print, 2, STRING("btl_majo3_09"), 0, -2)
    LABEL(90)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    LABEL(99)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_D_3"))
    SET_VALUE(GW(12), 2)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(majo_unison_phase_event))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 4, LW(3))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(4))
        IF_INT_NOT_EQUAL(LW(3), LW(4))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, LW(4))
            CALL(btlevtcmd_StartWaitEvent, -2)
            WAIT_MS(200)
            CALL_CHILD(PTR(marilyn_down))
            WAIT_MS(500)
            SWITCH(LW(4))
                CASE_EQUAL(0)
                SET_VALUE(LW(0), 55)
                SET_VALUE(LW(2), 0)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), 105)
                SET_VALUE(LW(2), 10)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), 155)
                SET_VALUE(LW(2), 20)
            END_SWITCH
            SET_VALUE(LW(1), 0)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL_CHILD(PTR(marilyn_up))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(2), 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(0))
    IF_INT_LESS(LW(0), 2)
        SET_VALUE(LW(2), 25)
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 3, 40, 35, LW(2))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 0)
        SET_VALUE(LW(9), PTR(&weapon_marilyn_kagenuke_attack1))
        CALL_CHILD(PTR(kagenuke_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 0)
        SET_VALUE(LW(9), PTR(&weapon_marilyn_thunder_attack1))
        CALL_CHILD(PTR(thunder_attack_event))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(0))
        ADD(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, LW(0))
        SET_VALUE(LW(9), PTR(&weapon_marilyn_charge1))
        CALL_CHILD(PTR(charge_attack_event))
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(kagenuke_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_ARM_FOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A1_1"))
    CALL_CHILD(marilyn_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    ADD(LW(2), 5)
#endif
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(marilyn_up)
    WAIT_MS(600)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_HAND_CLAP1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A2_1"))
    WAIT_FRAMES(4)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A2_2"))
    WAIT_MS(350)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A2_3"))
    WAIT_MS(100)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A2_4"))
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A1_1"))
    CALL_CHILD(marilyn_down)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL_CHILD(marilyn_up)
    RETURN
EVT_END()

static EVT_BEGIN(thunder_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_POWER1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A3_1"))
    WAIT_MS(800)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A3_2"))
    WAIT_MS(150)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(evt_sub_random, 2, LW(7))
    ADD(LW(7), 3)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetWidth, LW(3), LW(5))
    CALL(btlevtcmd_GetHeight, LW(3), LW(6))
    CALL(evt_eff64, STRING(""), STRING("thunder_n64"), LW(7), LW(0), LW(1), LW(2), LW(5), LW(6), 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_MARI_ATTACK_THUNDER1"), 0)
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 25, 13)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_rumble_onoff, 0, 0)
        WAIT_MS(300)
        CALL(evt_sub_rumble_onoff, 1, 0)
    END_INLINE_EVENT
    WAIT_MS(83)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(1000)
    RETURN
EVT_END()

static EVT_BEGIN(charge_attack_event)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MRR_A3_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MARI_CHARGE1"), EVT_NULLPTR, 0, LW(15))
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_btl_camera_set_mode, 0, 0)
    END_INLINE_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

