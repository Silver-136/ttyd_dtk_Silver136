// Pit of 100 Trials battle unit: the Chain-Chomp (`wanwan`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"
#include "libc_decls.h"
#include "battle/unit/unit_wanwan.h"

static USER_FUNC(chain_init);
static USER_FUNC(chain_main);

BattleUnitKind unit_wanwan;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[19];
static BattleDataEntry data_table[15];
static BattleUnitKindPart parts[9];
static BattleWeapon weapon_wanwan_attack;
static EvtWord init_event[41];
static EvtWord chain_event[23];
static EvtWord damage_event[33];

#if VERSION <= VERSION_DEMO_JP
static EvtWord counter_damage_event[115];
#else
static EvtWord counter_damage_event[68];
#endif

static EvtWord dead_event[200];
static EvtWord attack_event[452];
static EvtWord wait_event[476];
static EvtWord sound_gensui_event[22];
static EvtWord wait_sound_event1[25];

static EvtWord wait_sound_event2[11];

/**
 * Unit data of the Chain-Chomp in the Pit of 100 Trials: 7 HP, level 32;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_wanwan = {
    UNIT_CHAIN_CHOMP, // unitType
    "btl_un_wanwan", // name
    7, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    32, // level
    0, // bonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    34, // width
    36, // height
#if VERSION <= VERSION_DEMO_JP
    6,
    44,
    0.0f,
    18.0f,
    0.0f,
    0,
    0,
#else
    {6, 44}, // hitOffset
    {0.0f, 18.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {17.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
#endif
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
#if VERSION <= VERSION_DEMO_JP
    0.0f,
    17.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    0.0f,
    17.0f,
    23.4f,
    10.0f,
    0.0f,
    18.0f,
    0.0f,
#else
    {17.0f, 0.0f, 0.0f}, // bintaHitOffset
    {17.0f, 23.4f, 10.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
#endif
    34.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
#if VERSION <= VERSION_DEMO_JP
    -1,
#endif
    0, // swallowChance
#if VERSION >= VERSION_JP
    2, // swallowAttributes
#endif
    100, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_WAN_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    9, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Defence of the Chain-Chomp: 5 against normal, explosion and electric hits;
 * 99 against fire and ice, with immunity supplied by the separate element-attribute table.
 */
static BattleUnitDefense defence = {{5, 99, 99, 5, 5}};

/**
 * Damage modifiers of the Chain-Chomp per element (normal, fire, ice, explosion, electric): fire:
 * immune, ice: immune.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {0, 65, 65, 100, 65, 100, 100, 65, 100, 65, 100, 65, 100, 65, 65, 45, 30, 100, 65, 100, 100, 90};
#else
static StatusVulnerability regist = {
    0, // sleep
    70, // stop
    65, // dizzy
    100, // poison
    65, // confuse
    100, // electric
    0, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    65, // allergic
    45, // fright
    30, // galeForce
    100, // fast
    65, // slow
    100, // dodgy
    100, // invisible
    90, // ohko
};
#endif

/**
 * Pose table of the Chain-Chomp: animation names for each pose type (idle, walk, hurt, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "WAN_N_1"},
    {POSE_TYPE_STOP, "WAN_Y_1"},
    {POSE_TYPE_FREEZE, "WAN_Y_1"},
    {POSE_TYPE_CONFUSE, "WAN_K_1"},
    {POSE_TYPE_POISON, "WAN_X_1"},
    {POSE_TYPE_DIZZY, "WAN_X_1"},
    {POSE_TYPE_KO, "WAN_Z_1"},
    {POSE_TYPE_NORMAL, "WAN_S_1A"},
    {POSE_TYPE_DAMAGE_FIRE, "WAN_D_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "WAN_D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "WAN_A_1"},
    {POSE_TYPE_DAMAGE, "WAN_D_1"},
    {POSE_TYPE_ATTACK, "WAN_A_1"},
    {POSE_TYPE_RUN, "WAN_R_1"},
    {POSE_TYPE_WALK, "WAN_W_1"},
    {POSE_TYPE_DANGER, "WAN_X_1"},
    {POSE_TYPE_PERIL, "WAN_X_1"},
    {POSE_TYPE_TALK, "WAN_S_1A"},
    {POSE_TYPE_ACTION, "WAN_S_1A"},
};

/**
 * Data table of the Chain-Chomp: event scripts the battle looks up by tag (counter damage,
 * death and first-strike positioning).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Parts of the Chain-Chomp: 9 parts with their hit boxes, defence tables and pose tables. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kAbsolutePosition, 0, pose_table},
    {2, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {6, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {7, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {8, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {9, "btl_un_wanwan", "c_wanwan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Weapon of the Chain-Chomp's lunge and bite: damage 6. */
static BattleWeapon weapon_wanwan_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Init event of the Chain-Chomp: installs its wait, attack, damage and confusion events,
 * sets up its chain (chain_init) and starts chain_event, sets the landing sound,
 * and sets UNIT_WORK_SOUND_VOLUME (the volume of its barking, 127 at the start) and unit work 0 = 1.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
    CALL(chain_init, UNIT_ID_SELF)
    SPAWN_EVENT(chain_event)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("0"), STRING("SFX_ENM_WAN_LANDING1"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SOUND_VOLUME, 127)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Event run for as long as the Chain-Chomp exists:
 * updates its chain (chain_main) every frame and ends when the unit is gone.
 */
static EVT_BEGIN(chain_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(chain_main, UNIT_ID_SELF)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Damage event of the Chain-Chomp: marks it as being damaged (UNIT_WORK_CHAIN_STATE = 2, attribute 4) around the default
 * damage handling.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHAIN_STATE, 2)
    CALL(btlevtcmd_OnAttribute, LW(10), kFloating)
    CALL_CHILD(btldefaultevt_Damage)
    CALL(btlevtcmd_OffAttribute, LW(10), kFloating)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHAIN_STATE, 0)
    RETURN
EVT_END()

/**
 * Reaction of the Chain-Chomp when a target's counter hurts it (data table entries 25 to 36: counter damage
 * codes
 * 0x2C to 0x37, every counter except the top spikes):
 * it plays pose 39 and is knocked back a short way (25 units against its facing) over 5 frames,
 * waits half a second and, in the early demos, also hops twice back toward its spot.
 * It then restores the camera and starts the wait event again,
 * except when the hit knocked it out (DAMAGE_RESULT_ZERO_HP) without a Life Shroom to revive it
 * (DAMAGE_RESULT_REVIVAL): then it just
 * returns.
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_SetPartsFallAccel, LW(10), 1, FLOAT(1.0f))
    CALL(btlevtcmd_GetPartsPos, LW(10), 1, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_PartsFaceDirectionSub, LW(10), 1, LW(2), 25)
    ADD(LW(3), 25)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_DivePartsPosition, LW(10), 1, LW(2), LW(3), LW(4), 5, 0, EASING_LINEAR, 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_SetPartsFallAccel, LW(10), 1, FLOAT(1.0f))
    CALL(btlevtcmd_GetPartsPos, LW(10), 1, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 30)
    CALL(btlevtcmd_JumpPartsPosition, LW(10), 1, LW(2), LW(3), LW(4), 20, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 15)
    CALL(btlevtcmd_JumpPartsPosition, LW(10), 1, LW(2), LW(3), LW(4), 10, -1)
#else
    CALL(btlevtcmd_DivePartsPosition, LW(10), 1, LW(2), LW(3), LW(4), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
#endif
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Dead event of the Chain-Chomp: all 9 of its parts spin with the death sound for 48 frames,
 * a smoke puff and the coins, experience and star points appear,
 * and the parts tip over before the unit is killed (with a different kill mode for damage code 0x400).
 */
static EVT_BEGIN(dead_event)
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_AddPartsRotate, LW(10), LW(11), 0, -8, 0)
        ADD(LW(11), 1)
    END_LOOP
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_SetPartsRotate, LW(10), LW(11), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_SetPartsRotateOffset, LW(10), LW(11), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_AddPartsRotate, LW(10), LW(11), 3, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)

        // Pick a target for the attack; a confused unit that has none flusters.
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_wanwan_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wanwan_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SOUND_VOLUME, 127)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SOUND_VOLUME, LW(14))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wanwan_attack))

    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_wanwan_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangePartsFaceDirection, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)

    // The Chain-Chomp rears back and hops twice on the spot.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_A_1"))
    LOOP(2)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 20, 0, FLOAT(0.69921875f))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT3"), LW(14), 0, EVT_NULLPTR)
            WAIT_FRAMES(10)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT2"), LW(14), 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    END_LOOP
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(20)

        // The head returns to its idle pose during the wind-up hop.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_S_1A"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_ATTACK1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 30, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_A_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_ATTACK2"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    IF_INT_LESS(LW(1), 0)
        SET_VALUE(LW(1), 0)
    END_IF

    // Its last jump carries it onto the target to bite.
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 0, FLOAT(16.0f), FLOAT(0.0f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), LW(1), 10, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_wanwan_attack), 0, LW(5))
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

    // After the miss, its head returns to the unit's base position.
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_S_1A"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 0, FLOAT(8.0f), FLOAT(0.0f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    GOTO(98)

    // The attack connects.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)

    // Deal the damage, reduced by the action command defence.
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_wanwan_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_wanwan_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_S_1A"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 0, FLOAT(8.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    GOTO(98)

    // Wrap up the attack.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)

    // The attack is over; back to waiting.
    LABEL(99)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
    CALL(btlevtcmd_ChangePartsFaceDirection, UNIT_ID_SELF, 1, -1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Wait event of the Chain-Chomp: picks the idle behaviour by status.
 * Asleep (STATUS_SLEEP) it breathes with a slow tilting;
 * stopped or frozen (statuses 2 and 9) it holds pose WAN_Y_1;
 * dizzy, poisoned or confused (statuses 3 to 5) it hops slowly; otherwise it hops in place barking, with sounds.
 * Each runs in a sibling event and the event then loops forever.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHAIN_STATE, 0)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)

        // Asleep, it breathes with a slow tilt.
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHAIN_STATE, 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_N_1"))
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 1, 0, 10, 0)
        BEGIN_SIBLING_EVENT
            LABEL(1)
            CALL_CHILD(sound_gensui_event)
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
            WAIT_FRAMES(20)
            LOOP(10)
                WAIT_FRAMES(4)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 0, 1)
            END_LOOP
            LOOP(10)
                WAIT_FRAMES(4)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 0, -1)
            END_LOOP
            GOTO(1)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(15))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHAIN_STATE, 2)

        // Stopped or frozen, it holds pose WAN_Y_1.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_Y_1"))
        BEGIN_SIBLING_EVENT
            LABEL(5)
            CALL_CHILD(sound_gensui_event)
            CALL(evt_sub_random, 20, LW(0))
            WAIT_FRAMES(LW(0))
            GOTO(5)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(13))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(15))
    OR(LW(15), LW(13))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)

        // Dizzy, poisoned or confused, it hops slowly in place.
        BEGIN_SIBLING_EVENT
            LABEL(3)
            CALL_CHILD(sound_gensui_event)
            CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
            CALL(evt_sub_random, 20, LW(0))
            WAIT_FRAMES(LW(0))
            SET_VALUE(LW(10), 15)
            SPAWN_EVENT(wait_sound_event1)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 30, 0, FLOAT(0.25f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 30, MOVE_SOUND_NONE)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 10)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
            CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 20, 0, FLOAT(0.19921875f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 20, MOVE_SOUND_NONE)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 7)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
            CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 15, 0, FLOAT(0.39941406f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 15, MOVE_SOUND_NONE)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 7)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 15, 0, FLOAT(0.19921875f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 15, MOVE_SOUND_NONE)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            GOTO(3)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF

    // Otherwise it hops in place, barking.
    BEGIN_SIBLING_EVENT
        LABEL(4)
        CALL_CHILD(sound_gensui_event)
        CALL(evt_sub_random, 20, LW(0))
        WAIT_FRAMES(LW(0))
        SET_VALUE(LW(10), 15)
        SPAWN_EVENT(wait_sound_event1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_A_1"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 30, 0, FLOAT(0.25f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 30, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 10)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_S_1A"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 20, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 20, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 7)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_A_1"))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 15, 0, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), 0, LW(2), 15, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 7)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("WAN_S_1A"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 1, 15, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 15, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        GOTO(4)
    END_SIBLING_EVENT
    GOTO(98)
    LABEL(98)
    LABEL(99)
    WAIT_FRAMES(1)
    GOTO(99)
    RETURN
EVT_END()

/**
 * Script helper event: lowers the volume of the Chain-Chomp's barking (UNIT_WORK_SOUND_VOLUME) by about 30 percent,
 * to a minimum of 32, each time it is called.
 */
static EVT_BEGIN(sound_gensui_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SOUND_VOLUME, LW(11))
    MULTIPLY_FLOAT(LW(11), FLOAT(0.69921875f))
    IF_INT_LESS(LW(11), 32)
        SET_VALUE(LW(11), 32)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SOUND_VOLUME, LW(11))
    RETURN
EVT_END()

/**
 * Plays the Chain-Chomp's first bark sound set (WAIT1 and WAIT3, then WAIT2 after LW(10) frames) at the volume
 * in LW(11).
 */
static EVT_BEGIN(wait_sound_event1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT1"), LW(11), 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT3"), LW(11), 0, EVT_NULLPTR)
    WAIT_FRAMES(LW(10))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT2"), LW(11), 0, EVT_NULLPTR)
    RETURN
EVT_END()

/** Plays the Chain-Chomp's second bark sound (WAIT4) after LW(10) frames at the volume in LW(11). */
static EVT_BEGIN(wait_sound_event2)
    WAIT_FRAMES(LW(10))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_WAN_WAIT4"), LW(11), 0, EVT_NULLPTR)
    RETURN
EVT_END()

/**
 * Per-frame script helper that animates the Chomp's chain (parts 2-9). args[0] = unit.
 * Part 9 is the stake, behind the chomp; when work[2] is 2 the current link layout is recorded relative to the
 * stake and when it is 1 the links keep that shape (during damage, sleep, stop or freeze).
 * Otherwise the links hang as a sagging chain between the Chomp (part 1, capped at y = 110 in retail) and the stake,
 * each link moved to a fixed distance from its neighbour and held above the floor.
 */
static USER_FUNC(chain_main) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    BattleUnitPart* part;
    f32 x;
    f32 y;
    f32 z;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 len;
    f32 sag;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_BLOCK;
    }
    part = BtlUnit_GetPartsPtr(unit, 1);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    BtlUnit_SetPartsPos(BtlUnit_GetPartsPtr(unit, 9), x - 20.0f * unit->faceDirection, y, z);

    // 2: remember the chain's shape relative to the stake
    if (unit->work[UNIT_WORK_CHAIN_STATE] == 2) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x, &y, &z);
        for (i = 8; i >= 1; i--) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
            part->work[PARTS_WORK_OFFSET_X] = x2 - x;
            part->work[PARTS_WORK_OFFSET_Y] = y2 - y;
        }
        unit->work[UNIT_WORK_CHAIN_STATE] = 1;
    }

    // 1: keep that shape
    if (unit->work[UNIT_WORK_CHAIN_STATE] == 1) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x, &y, &z);
        for (i = 8; i >= 1; i--) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_SetPartsPos(part, x + part->work[PARTS_WORK_OFFSET_X], y + part->work[PARTS_WORK_OFFSET_Y], z);
        }
        return EVT_RETURN_DONE;
    }

    // otherwise hang the links between the chomp and the stake
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 1), &x1, &y1, &z1);
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x2, &y2, &z2);
    dx = x1 - x2;
    dy = y1 - y2;
    dz = z1 - z2;
#if VERSION <= VERSION_DEMO_JP
    len = (f32)sqrt(dx * dx + dy * dy + dz * dz) / 7;
#else
    len = (f32)sqrt(dx * dx + dy * dy + dz * dz) / 8;
#endif
    if (len < 10.0f) {
        len = 10.0f;
    }
    sag = 300.0f / (len * len);
#if VERSION >= VERSION_JP
    part = BtlUnit_GetPartsPtr(unit, 1);
    BtlUnit_GetPartsPos(part, &x, &y, &z);
    if (y > 110.0f) {
        y = 110.0f;
    }
    BtlUnit_SetPartsPos(part, x, y, z);
#endif
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 1), &x, &y, &z);
    x += -(unit->faceDirection * 16);
    y += 16.0f;
    BtlUnit_SetPartsPos(BtlUnit_GetPartsPtr(unit, 2), x, y, z);
#if VERSION <= VERSION_DEMO_JP
    for (i = 2; i <= 6; i++) {
#else
    for (i = 2; i <= 7; i++) {
#endif
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, i), &x1, &y1, &z1);
        part = BtlUnit_GetPartsPtr(unit, i + 1);
        BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
        y2 -= sag;
        dx = x1 - x2;
        dy = y1 - y2;
        dz = z1 - z2;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        x2 += dx * t;
        y2 += dy * t;
        z2 += dz * t;
        if (y2 < 0.0f) {
            y2 = 0.0f;
        }
        BtlUnit_SetPartsPos(part, x2, y2, z2);
    }
    for (i = 9; i >= 3; i--) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, i), &x1, &y1, &z1);
        part = BtlUnit_GetPartsPtr(unit, i - 1);
        BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
        y2 -= sag;
        dx = x1 - x2;
        dy = y1 - y2;
        dz = z1 - z2;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        x2 += dx * t;
        y2 += dy * t;
        z2 += dz * t;
        if (y2 < 0.0f) {
            y2 = 0.0f;
        }
        BtlUnit_SetPartsPos(part, x2, y2, z2);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = unit.
 * Puts the chain's link parts (2-7) on top of the Chomp,
 * links 8 and 9 behind it and plays their chain animations (WAN_S_1B, WAN_Z_2 for the stake).
 */
static USER_FUNC(chain_init) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    BattleUnitPart* part;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);

    // Intermediate links 2 through 7 start at the chomp.
    for (i = 2; i <= 7; i++) {
        part = BtlUnit_GetPartsPtr(unit, i);
        BtlUnit_SetPartsPos(part, x, y, z);
        BtlUnit_SetAnim(part, "WAN_S_1B");
    }
    part = BtlUnit_GetPartsPtr(unit, 8);
    BtlUnit_SetPartsPos(part, x - 20.0f * unit->faceDirection, y, z);
    BtlUnit_SetAnim(part, "WAN_S_1B");
    part = BtlUnit_GetPartsPtr(unit, 9);
    BtlUnit_SetPartsPos(part, x - 20.0f * unit->faceDirection, y, z);
    BtlUnit_SetAnim(part, "WAN_Z_2");
    return EVT_RETURN_DONE;
}
