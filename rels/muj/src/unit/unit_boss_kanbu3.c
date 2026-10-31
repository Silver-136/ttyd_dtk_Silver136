// Keelhaul Key (muj) battle unit: Lord Crump (kanbu3): its parts, defence/status tables, and attack events.

#include "battle/battle_unit.h"
#include "zakogun.h"
#include "evt/evt_cmd.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "muj.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "unit_boss_gundan_zako.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_GROUP 0 // which group it calls: 0 entry_group1, 1 entry_group2 from the ceiling (after the HP <= 15 scene), 2 entry_group3 (after its revival in dead_event)
#define UNIT_WORK_GUARD 1 // 1 while in the guard stance (guard_event sets; guard_cancel_event and dead_event clear); attack_event then cancels the guard or does nothing

USER_FUNC(check_exist_group);
USER_FUNC(check_exist_kenkyu);
extern s32 down_ceil[];
extern BattleUnitKind unit_boss_gundan_zako_group2;
extern BattleUnitKind unit_boss_gundan_zako_group3;
extern BattleUnitKind unit_gundan_zako_magician;
extern EvtWord zakogun_entry[];

BattleUnitKind unit_boss_kanbu3;
static BattleUnitDefense defence;
static BattleUnitDefense defence_guard;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[3];
BattleDataEntry data_table2[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_kanbu3_attack;
BattleWeapon weapon_kanbu3_charge;
BattleUnitSetup entry_kenkyu;
BattleUnitSetup entry_group1;
BattleUnitSetup entry_group2;
BattleUnitSetup entry_group3;
static EvtWord init_event[59];
static EvtWord entry_event[45];
#ifdef VERSION_G8ME01
static EvtWord damage_event[337];
#else
static EvtWord damage_event[315];
#endif
static EvtWord dead_event[402];
static EvtWord phase_event[55];
EvtWord call_event[284];
static EvtWord attack_event[136];
static EvtWord guard_event[56];
static EvtWord guard_cancel_event[69];
EvtWord no_event[27];
static EvtWord charge_event[71];
static EvtWord wait_event[6];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_guard[18];
static EvtWord normal_attack_event[365];

/**
 * Unit data of Lord Crump: 30 HP, 56x59 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_boss_kanbu3 = {
    UNIT_LORD_CRUMP_CHAPTER5, // mUnitType
    "btl_un_kanbu3", // name
    30, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    57, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    8, // pbSoftCap
    56, // width
    59, // height
    {0, 56}, // hitOffset
    {0.0f, 26.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {28.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {21.0f, 0.0f, 0.0f}, // bintaHitOffset
    {18.0f, 38.35f, 0.0f}, // kissHitOffset
    {0.0f, 26.0f, 0.0f}, // cutBaseOffset
    42.0f, // cutWidth
    52.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_KANBU_DAMAGED1", // damageSfxName
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

/** Defence of Lord Crump: 0 against every kind of hit. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence of Lord Crump while he is guarding: 2 against every kind of hit (set by guard_event). */
static BattleUnitDefense defence_guard = {{2, 2, 2, 2, 2}};

/** Defence attributes of Lord Crump: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    80, // sleep
    70, // stop
    60, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    60, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    60, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    60, // slow
    100, // dodgy
    0, // invisible
    0, // ohko
};

/** Event table of Lord Crump: dead_event (tag 49). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Event table Lord Crump switches to after his first defeat: it no longer has the defeat script,
 * so the next defeat ends the battle.
 */
BattleDataEntry data_table2[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Parts of Lord Crump: 1 part (model c_kanbu). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_kanbu3", "c_kanbu", 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Weapon of Lord Crump (attack): targets one unit for 3 damage; used by attack_event. */
BattleWeapon weapon_kanbu3_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
    4, // targetWeightFlags
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

/** Weapon of Lord Crump (charge): targets one unit; used by charge_event. */
BattleWeapon weapon_kanbu3_charge = {
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
    3, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Spawn setup for the X-Naut PhD (unit_gundan_zako_magician): enemy side, placed off-screen at first. */
BattleUnitSetup entry_kenkyu = {
    &unit_gundan_zako_magician, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for X-Nauts (unit_boss_gundan_zako_group1): enemy side, placed off-screen at first;
 * used by call_event.
 */
BattleUnitSetup entry_group1 = {
    &unit_boss_gundan_zako_group1, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for X-Nauts (unit_boss_gundan_zako_group2): enemy side, placed off-screen at first;
 * used by damage_event, call_event.
 */
BattleUnitSetup entry_group2 = {
    &unit_boss_gundan_zako_group2, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for X-Nauts (unit_boss_gundan_zako_group3): enemy side, placed off-screen at first;
 * used by dead_event, call_event.
 */
BattleUnitSetup entry_group3 = {
    &unit_boss_gundan_zako_group3, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Init event of Lord Crump: installs his wait, entry, phase, attack,
 * damage and confusion events and his walk and run sounds,
 * clears unit work 0 (which X-Naut group is in play: 0 to 2) and 1 (guarding),
 * registers the X-Naut crowd (zakogun_entry) and sets up the first formation (zakogun_ext_init1).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    CALL_CHILD(PTR(zakogun_entry))
    CALL(zakogun_ext_init1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Entry event of Lord Crump: after 120 frames the camera closes in on him and, with the status window hidden,
 * he threatens to turn the party into seaweed (stg5_muj_125_00).
 */
static EVT_BEGIN(entry_event)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 166, 88, 422, 166, 60, 0, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_muj_125_00"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * Damage event of Lord Crump.
 * With group 3 in play (unit work 0 = 2), a knockout hit that no Life Shroom saves shows his hurt pose and
 * after 120 frames group 3 flees (zakogun_escape3).
 * Then the default damage reaction runs.
 * With group 1 in play, the first hit that leaves him at 15 HP
 * or less calls group 1 off (zakogun_escape1, unless it is already dying),
 * and he complains the party won't get off that easily (stg5_muj_125_01),
 * shouts for the X-Nauts (stg5_muj_125_02) and group 2 comes down with the ceiling (down_ceil);
 * unit work 0 becomes 1.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
            IF_INT_EQUAL(LW(0), 0)
#ifdef VERSION_G8ME01
                CALL(btlevtcmd_GetDamageCode, UNIT_ID_SELF, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(24)
                    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE_FIRE)
                    CASE_EQUAL(25)
                    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE_FIRE)
                    CASE_DEFAULT
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_1"))
                END_SWITCH
#else
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_1"))
#endif
                CALL(btlevtcmd_WaitAttackEnd)
                WAIT_FRAMES(120)
                CALL(check_exist_group, LW(0))
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_ESCAPE1"), LW(13))
                    CALL(zakogun_escape3)
                    CALL(evt_snd_sfxoff, LW(13))
                    CALL(btlevtcmd_KillUnit, LW(0), 0)
                END_IF
            END_IF
        END_IF
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(btldefaultevt_Damage))
#else
    CALL_CHILD(btldefaultevt_Damage)
#endif
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 15)
            CALL(btlevtcmd_WaitAttackEnd)
            WAIT_FRAMES(90)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
            CALL(check_exist_group, LW(0))
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetUnitWork, LW(0), ZAKO_GROUP1_WORK_DYING, LW(1))
                IF_INT_NOT_EQUAL(LW(1), 0)
                    WAIT_FRAMES(90)
                    GOTO(10)
                END_IF
                ELSE
                GOTO(10)
            END_IF
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(16)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(16)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
            CALL(check_exist_group, LW(0))
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(zakogun_escape1)
                CALL(btlevtcmd_KillUnit, LW(0), 0)
            END_IF
            WAIT_FRAMES(20)
            LABEL(10)
            WAIT_FRAMES(20)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 1, 166, 88, 422, 166, 60, 0, 20, 11)
            WAIT_FRAMES(20)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_muj_125_01"), 0, -2)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            WAIT_MS(500)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(16)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(16)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_muj_125_02"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            WAIT_MS(500)
            CALL(zakogun_ext_init2)
            CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_group2), 0)
            CALL(btlevtcmd_SetPos, LW(0), 0, 80, 0)
            CALL(btlevtcmd_SetHomePos, LW(0), 0, 80, 0)
            CALL(down_ceil)
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, 1)
        END_IF
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Runs at Lord Crump's first defeat: he drops his guard,
 * the group in play flees (group 1, or group 2 as the ceiling rises; after a wait if it is dying),
 * and he runs off to the right, where he recovers to full HP (five recovery effects)
 * and loses the KO status (27).
 * Group 3, the crowd of 500 X-Nauts, arrives as he runs back;
 * unit work 0 becomes 2, his data table loses this event (data_table2),
 * and he says he is down but not out (stg5_muj_125_03).
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence))
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_WaitAttackEnd)
        CALL(check_exist_group, LW(0))
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, LW(0), ZAKO_GROUP1_WORK_DYING, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                WAIT_FRAMES(180)
            END_IF
        END_IF
        CALL(check_exist_group, LW(0))
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(zakogun_escape1)
            CALL(btlevtcmd_KillUnit, LW(0), 0)
        END_IF
        CASE_END
        CASE_EQUAL(1)
        CALL(btlevtcmd_WaitAttackEnd)
        CALL(check_exist_group, LW(0))
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, LW(0), ZAKO_GROUP2_WORK_DYING, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                WAIT_FRAMES(180)
            END_IF
        END_IF
        CALL(check_exist_group, LW(0))
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(up_ceil)
            CALL(zakogun_escape2)
            CALL(btlevtcmd_KillUnit, LW(0), 0)
        END_IF
        CASE_END
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 300, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_moveto, 0, 220, 87, 419, 220, 60, 0, 60, 11)
    WAIT_FRAMES(60)
    CALL(evt_batstage_set_aud_dark, 0, 1, 0)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, 250, 30, 104, 4, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, 220, 30, 103, 5, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(25)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, 280, 30, 102, 6, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, 235, 60, 101, 7, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, 265, 60, 100, 8, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_OnOffStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, 0, 0, 0)
    WAIT_FRAMES(95)
    CALL(evt_batstage_return_aud_dark_base, 1, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 80)
    SUBTRACT(LW(2), 20)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_group3), 0)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, -1)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_MOVE2"), 0)
    CALL(zakogun_ext_init3)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, 2)
    // The replacement table omits the revival event, so this revival happens only once.
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table2))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 166, 88, 422, 166, 60, 0, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_muj_125_03"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    WAIT_MS(500)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Phase event of Lord Crump: before the player's turn (from turn 2), if he is not KO'd (status 27) and can act,
 * with group 3 in play (unit work 0 = 2) he calls it back when it is gone (call_event).
 */
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL_CHILD(PTR(call_event))
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Lord Crump calls his horde (call_event): if no horde unit is on the stage yet,
 * he plays the call-back sound and pose C_1 and brings in horde group 1, 2 or 3 according to unit work 0.
 */
EVT_BEGIN(call_event)
    CALL(check_exist_group, LW(15))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        IF_INT_EQUAL(LW(15), -1)
            GOTO(10)
        END_IF
        CASE_END
        CASE_EQUAL(1)
        IF_INT_EQUAL(LW(15), -1)
            GOTO(10)
        END_IF
        CASE_END
        CASE_EQUAL(2)
        IF_INT_EQUAL(LW(15), -1)
            GOTO(10)
        END_IF
        CASE_END
    END_SWITCH
    SET_VALUE(LW(0), 0)
    GOTO(99)
    LABEL(10)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_BACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_1"))
        WAIT_FRAMES(16)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_BACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_1"))
        CASE_END
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
        WAIT_FRAMES(16)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
        WAIT_FRAMES(16)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_ON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_2"))
        CASE_END
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_BACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_1"))
        WAIT_FRAMES(16)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_CALL_BACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("C_1"))
        CASE_END
    END_SWITCH
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(100)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(15), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 80)
            SUBTRACT(LW(2), 20)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_group1), 0)
            CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
            CALL(zakogun_ext_init1)
            WAIT_FRAMES(120)
            CASE_END
            CASE_EQUAL(1)
            CALL(zakogun_ext_init2)
            CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_group2), 0)
            CALL(btlevtcmd_SetPos, LW(0), 0, 80, 0)
            CALL(btlevtcmd_SetHomePos, LW(0), 0, 80, 0)
            CALL(down_ceil)
            WAIT_FRAMES(60)
            CASE_END
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 80)
            SUBTRACT(LW(2), 20)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_group3), 0)
            CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_MOVE2"), 0)
            CALL(zakogun_ext_init3)
            WAIT_FRAMES(120)
            CASE_END
        END_SWITCH
    END_IF
    SET_VALUE(LW(0), 1)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Attack event of Lord Crump: uses an item when the item check says so.
 * With group 1 or 2 in play (unit work 0 at most 1) and not guarding, he picks by weights 75 normal attack,
 * 0 guard (so guard_event is never chosen) and 30 calling the group (call_event);
 * if the group is already present he rolls again.
 * While guarding he lowers his guard or waits (no_event), 50/50.
 * With group 3 in play he uses his normal attack or charges (charge_event), 50/50.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUP, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            LABEL(10)
            SET_VALUE(LW(0), 75)
            ADD(LW(0), 0)
            ADD(LW(0), 30)
            SUBTRACT(LW(0), 1)
            CALL(evt_sub_random, LW(0), LW(1))
            SET_VALUE(LW(0), 75)
            IF_INT_LESS(LW(1), LW(0))
                SET_VALUE(LW(9), PTR(&weapon_kanbu3_attack))
                CALL_CHILD(PTR(normal_attack_event))
                GOTO(99)
            END_IF
            ADD(LW(0), 0)
            IF_INT_LESS(LW(1), LW(0))
                CALL_CHILD(PTR(guard_event))
                GOTO(99)
            END_IF
            CALL_CHILD(PTR(call_event))
            IF_INT_EQUAL(LW(0), 1)
                GOTO(99)
                ELSE
                GOTO(10)
            END_IF
            ELSE
            CALL(evt_sub_random, 99, LW(1))
            IF_INT_LESS(LW(1), 50)
                CALL_CHILD(PTR(guard_cancel_event))
                ELSE
                CALL_CHILD(PTR(no_event))
            END_IF
        END_IF
        ELSE
        // The random range ends at 49, so this branch always chooses the normal attack.
        SET_VALUE(LW(0), 50)
        ADD(LW(0), 0)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 50)
            SET_VALUE(LW(9), PTR(&weapon_kanbu3_attack))
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(charge_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Lord Crump raises his guard (guard poses G_1 and G_2),
 * switches to the guard defence and pose tables and sets unit work 1 (guarding); attack_event gives it weight 0.
 */
static EVT_BEGIN(guard_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_1"))
    WAIT_FRAMES(16)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_2"))
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_guard))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 1)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/**
 * Lord Crump lowers his guard: hops back into his standing pose,
 * restores his normal defence and pose tables and clears unit work 1.
 */
static EVT_BEGIN(guard_cancel_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence))
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/**
 * Lord Crump's idle event for turns without an action:
 * aims the battle camera at him and zooms in to 200, then returns the camera to its default mode.
 */
EVT_BEGIN(no_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_FRAMES(120)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/** Lord Crump charges up: takes the guard poses G_1 and G_2 and applies weapon_kanbu3_charge (charge strength 3) to himself. */
static EVT_BEGIN(charge_event)
    SET_VALUE(LW(3), UNIT_ID_SELF)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_kanbu3_charge))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_kanbu3_charge))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_1"))
    WAIT_FRAMES(16)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_2"))
    WAIT_FRAMES(120)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(60)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_kanbu3_charge))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kanbu3_charge), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

/** Wait (idle) event of Lord Crump: sets the idle pose from his current pose table (normal or guard). */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/** Pose table of Lord Crump: maps its battle poses to its animations. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Poses of Lord Crump while he is guarding (set by guard_event). */
static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "G_2"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "G_2"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "G_2"},
};

/**
 * Lord Crump's normal attack: he runs up to one target (the first-strike target if he struck first)
 * and jumps onto it in a butt stomp with the hip-attack sounds and a camera shake, bounces off,
 * flops down and runs home.
 */
static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
#ifdef VERSION_G8ME01
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
#else
            CALL_CHILD(subsetevt_confuse_flustered)
#endif
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(2.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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
        LABEL(90)
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)
        LABEL(98)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        WAIT_FRAMES(25)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_1"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        RETURN
EVT_END()

USER_FUNC(check_exist_group) {
    s32 i;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id = -1;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && (unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION1 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION2 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION3)) {
            id = i;
        }
    }
    evtSetValue(event, args[0], id);
    return EVT_RETURN_DONE;
}
/**
 * Script helper: writes the battle unit id of the unit of kind 0x77 to args[0],
 * or -1 if none exists (the name suggests a researcher unit).
 */
USER_FUNC(check_exist_kenkyu) {
    s32 i;
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id = -1;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_X_NAUT_PHD) {
            id = i;
        }
    }
    evtSetValue(event, args[0], id);
    return EVT_RETURN_DONE;
}
