#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0     // 1 ends the sibling loop that spins the thrown bone (part 2, or part 3 in call_friend_event)
#define UNIT_WORK_REWARD_GIVEN 1  // 1 once coins, EXP and star points have been stored (dead_event or fake_dead_event), so it happens only once
#define UNIT_WORK_REBIRTH_TURNS 2 // turns left before it reassembles: fake_dead_event sets 2 + evt_sub_random(2), rebirth_event counts down and revives at 0

USER_FUNC(btlevtcmd_SetJumpSound);
static USER_FUNC(honenoko_copy_status);

BattleUnitKind unit_honenoko;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[4];
static BattleWeapon weapon_honenoko_attack;
static BattleUnitSetup entry_honenoko;
static PoseTableEntry pose_table[19];
static BattleDataEntry data_table[4];
static EvtWord init_event[31];
static EvtWord damage_event[10];
static EvtWord dead_event[190];
static EvtWord fake_dead_event[165];
static EvtWord rebirth_event[67];
static EvtWord attack_event[185];
static EvtWord start_avoid[26];
static EvtWord normal_attack_event[428];
static EvtWord call_friend_event[311];
static EvtWord wait_event[6];

BattleUnitKind unit_honenoko = {
    UNIT_DULL_BONES, // mUnitType
    "btl_un_honenoko", // name
    1, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    9, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    70, // runRate
    9999, // pbSoftCap
    36, // width
    40, // height
    {-4, 40}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {18.0f, 0.0f, 0.0f}, // bintaHitOffset
    {9.0f, 24.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    36.0f, // cutWidth
    40.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_HONENOKO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    4, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    95, // stop
    50, // dizzy
    0, // poison
    50, // confuse
    100, // electric
    100, // burn
    90, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    95, // defenseDown
    90, // allergic
    10, // fright
    90, // galeForce
    100, // fast
    90, // slow
    100, // dodgy
    100, // invisible
    50, // ohko
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_honenoko", "c_nagehone_g", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static BattleWeapon weapon_honenoko_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
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

static BattleUnitSetup entry_honenoko = {
    &unit_honenoko, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KRN_N_1"},
    {POSE_TYPE_STOP, "KRN_Y_1"},
    {POSE_TYPE_FREEZE, "KRN_Y_1"},
    {POSE_TYPE_CONFUSE, "KRN_K_1"},
    {POSE_TYPE_POISON, "KRN_X_1"},
    {POSE_TYPE_DIZZY, "KRN_X_1"},
    {POSE_TYPE_KO, "KRN_Z_2"},
    {POSE_TYPE_NORMAL, "KRN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRN_S_1"},
    {POSE_TYPE_DAMAGE, "KRN_D_1"},
    {POSE_TYPE_ATTACK, "KRN_A_1"},
    {POSE_TYPE_RUN, "KRN_R_1"},
    {POSE_TYPE_WALK, "KRN_W_1"},
    {POSE_TYPE_DANGER, "KRN_X_1"},
    {POSE_TYPE_PERIL, "KRN_X_1"},
    {POSE_TYPE_TALK, "KRN_T_1"},
    {POSE_TYPE_ACTION, "KRN_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FAKE_DEAD_EVENT, fake_dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 0)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("0"), STRING("SFX_ENM_HONENOKO_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 1)
        CALL(btlevtcmd_GetCoin, LW(10), LW(0))
        CALL(btlevtcmd_StoreCoin, LW(0))
        CALL(btlevtcmd_GetExp, LW(10), LW(0))
        CALL(btlevtcmd_StoreExp, LW(0))
        CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
        ADD(LW(2), 10)
        ADD(LW(1), 30)
        CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
        CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            LOOP(LW(0))
            END_LOOP
        END_IF
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
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

static EVT_BEGIN(fake_dead_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 1)
        SET_VALUE(LW(10), -2)
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
        CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
        CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            LOOP(LW(0))
            END_LOOP
        END_IF
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_2"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_CRUMBLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(1000)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 2)
    CALL(evt_sub_random, 2, LW(0))
    ADD(LW(0), 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -20, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(rebirth_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_4"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kInactive)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(rebirth_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 85)
    ADD(LW(0), 0)
    ADD(LW(0), 15)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 85)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(1)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        SET_VALUE(LW(4), LW(0))
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(4), LW(5))
        IF_INT_LESS(LW(4), 0)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(PTR(call_friend_event))
                WAIT_MS(500)
                GOTO(99)
            END_IF
            ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(1)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
                CALL_CHILD(PTR(call_friend_event))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                WAIT_MS(500)
                GOTO(99)
            END_IF
            ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    GOTO(0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_honenoko_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_honenoko_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_1"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_2"))
    WAIT_MS(250)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_BONE_THROW1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KRN_H_1"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, LW(14))
            IF_INT_EQUAL(LW(14), 1)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 2, 0, 0, 25)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_honenoko_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            ELSE
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 14)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(9))
    MULTIPLY(LW(0), LW(9))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.049804688f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE)
    CALL(evt_snd_sfxoff, LW(15))
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 14)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_honenoko_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -50)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(2.0f), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    WAIT_FRAMES(10)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(call_friend_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    SET_VALUE(LW(10), 0)
    LOOP(6)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_3"))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_4"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SWITCH(LW(10))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_4"))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_5"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_5"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_3"))
            CASE_EQUAL(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_3"))
            CASE_DEFAULT
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_2"))
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden | kNoShadow)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(4), LW(5), LW(6))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(4), 15)
        ADD(LW(5), 14)
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(4), LW(5), LW(6))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
            SET_VALUE(LW(15), 0)
            LOOP(0)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, LW(14))
                IF_INT_EQUAL(LW(14), 1)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 3, 0, 0, LW(15))
                ADD(LW(15), 25)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(6.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 20, MOVE_SOUND_NONE)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SWITCH(LW(10))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_5"))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden | kNoShadow)
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_4"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_3"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_2"))
            CASE_EQUAL(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_1"))
            CASE_DEFAULT
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_Z_2"))
        END_SWITCH
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden | kNoShadow)
        ADD(LW(10), 1)
    END_LOOP
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_D_4"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_S_1"))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_honenoko), 0)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kHidden | kNoShadow)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(honenoko_copy_status, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static USER_FUNC(honenoko_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->mSizeChangeTurns = unit1->mSizeChangeTurns;
    unit2->mSizeChangeStrength = unit1->mSizeChangeStrength;
    return EVT_RETURN_DONE;
}
