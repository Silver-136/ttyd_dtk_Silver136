#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "unit/party/unit_party_vivian.h"
#include "battle/battle_database_common.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
extern EvtWord btl_majolyne_zoom[];
extern EvtWord btl_marilyn_zoom[];
extern EvtWord btl_vivian_zoom[];
extern EvtWord majo3_shadow_tail_event[];
extern EvtWord majo_unison_phase_event[];
extern BattleUnitKind unit_boss_majolyne;
extern BattleUnitKind unit_boss_marilyn;

BattleUnitKind unit_boss_vivian;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_vivian_kagenuke_attack1;
BattleWeapon weapon_vivian_flame_attack1;
static BattleUnitSetup entry_marilyn;
static BattleUnitSetup entry_majolyne;
static EvtWord vivian_down[19];
static EvtWord vivian_up[27];
static EvtWord init_event[179];
static EvtWord entry_event[46];
static EvtWord damage_event[22];
static EvtWord dead_event[189];
static EvtWord unison_phase_event[81];
static EvtWord phase_event[2];
static EvtWord attack_event[98];
#ifdef VERSION_G8ME01
static EvtWord kagenuke_attack_event[207];
#else
static EvtWord kagenuke_attack_event[204];
#endif
EvtWord flame_attack_event[185];
#ifdef VERSION_G8ME01
static EvtWord wait_event[7];
#else
static EvtWord wait_event[6];
#endif

BattleUnitKind unit_boss_vivian = {
    UNIT_VIVIAN_CHAPTER2, // mUnitType
    "btl_un_vivian1", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    26, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    30, // width
    40, // height
    {0, 40}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {6.0f, 16.0f, 0.0f}, // talkTogeBaseOffset
    {15.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {11.0f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    30.0f, // cutWidth
    40.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_VIVI_DAMAGED1", // damageSfxName
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
    70, // sleep
    80, // stop
    70, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    50, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
    70, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "PTR_N_1"},
    {POSE_TYPE_STOP, "PTR_Z_1"},
    {POSE_TYPE_FREEZE, "PTR_Z_1"},
    {POSE_TYPE_CONFUSE, "PTR_X_1"},
    {POSE_TYPE_POISON, "PTR_X_1"},
    {POSE_TYPE_DIZZY, "PTR_X_1"},
    {POSE_TYPE_NORMAL, "PTR_S_4"},
    {POSE_TYPE_DAMAGE_FIRE, "PTR_Q_1"},
    {30, "PTR_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PTR_D_1"},
    {POSE_TYPE_DAMAGE, "PTR_D_1"},
    {POSE_TYPE_ATTACK, "PTR_S_4"},
    {POSE_TYPE_RUN, "PTR_R_1"},
    {POSE_TYPE_WALK, "PTR_W_1"},
    {POSE_TYPE_DANGER, "PTR_D_2"},
    {POSE_TYPE_PERIL, "PTR_D_2"},
    {POSE_TYPE_TALK, "PTR_T_5"},
    {POSE_TYPE_ACTION, "PTR_S_4"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_vivian", "c_vivian", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

BattleWeapon weapon_vivian_kagenuke_attack1 = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

BattleWeapon weapon_vivian_flame_attack1 = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
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

static BattleUnitSetup entry_marilyn = {
    &unit_boss_marilyn, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static BattleUnitSetup entry_majolyne = {
    &unit_boss_majolyne, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(vivian_down)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VIVI_SHADOW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(vivian_up)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VIVI_SHADOW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 6, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, GW(10))
    CALL(btlevtcmd_SpawnUnit, GW(9), PTR(&entry_marilyn), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, GW(9), 4, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 5, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 6, 0)
    CALL(btlevtcmd_SetUnitWork, GW(9), 7, 0)
    CALL(btlevtcmd_SpawnUnit, GW(8), PTR(&entry_majolyne), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    ADD(LW(2), 20)
    CALL(btlevtcmd_SetPos, GW(8), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(8), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, GW(8), 4, 2)
    CALL(btlevtcmd_SetUnitWork, GW(8), 5, 2)
    CALL(btlevtcmd_SetUnitWork, GW(8), 6, 0)
    CALL(btlevtcmd_SetUnitWork, GW(8), 7, 0)
    SET_VALUE(GW(13), 0)
    SET_VALUE(GW(12), 0)
    SET_VALUE(GW(11), 0)
    SET_VALUE(GW(14), 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    SPAWN_EVENT(PTR(majo3_shadow_tail_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    WAIT_FRAMES(60)
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(btl_majolyne_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_00"), 0, GW(8))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_marilyn_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_01"), 0, GW(9))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_vivian_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_02"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(13), 1)
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
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_EQUAL(1)
            LOOP(0)
                IF_INT_EQUAL(GW(12), 2)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            GOTO(99)
            CASE_EQUAL(2)
        END_SWITCH
        CASE_EQUAL(1)
        LOOP(0)
            IF_INT_EQUAL(GW(11), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        GOTO(99)
        CASE_EQUAL(2)
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_EQUAL(1)
            LOOP(0)
                IF_INT_EQUAL(GW(12), 2)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            GOTO(99)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL_CHILD(PTR(btl_vivian_zoom))
            CALL(evt_msg_print, 2, STRING("btl_majo3_13"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            GOTO(90)
        END_SWITCH
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_DEFAULT
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("PTR_D_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("PTR_D_1"))
    CALL_CHILD(PTR(btl_vivian_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_10"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_D_3"))
    SET_VALUE(GW(13), 2)
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
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            CALL_CHILD(PTR(vivian_down))
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
            CALL_CHILD(PTR(vivian_up))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
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
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 7, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 2)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 8, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_vivian_kagenuke_attack1))
        CALL_CHILD(PTR(kagenuke_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 2)
        SET_VALUE(LW(9), PTR(&weapon_vivian_flame_attack1))
        CALL_CHILD(PTR(flame_attack_event))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 70)
        ADD(LW(1), 30)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        IF_INT_LESS(LW(0), 70)
            SET_VALUE(LW(9), PTR(&weapon_vivian_kagenuke_attack1))
            CALL_CHILD(PTR(kagenuke_attack_event))
            ELSE
            SET_VALUE(LW(9), PTR(&weapon_vivian_flame_attack1))
            CALL_CHILD(PTR(flame_attack_event))
        END_IF
    END_SWITCH
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(kagenuke_attack_event)
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
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL_CHILD(vivian_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    ADD(LW(2), 5)
#endif
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(vivian_up)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_2"))
    WAIT_MS(70)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VIVI_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    CALL_CHILD(vivian_down)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL_CHILD(vivian_up)
    RETURN
EVT_END()

EVT_BEGIN(flame_attack_event)
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A3_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VIVI_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 6, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

