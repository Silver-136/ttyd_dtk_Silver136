#include "battle/battle_unit.h"
#include "eff/eff_scanning.h"
#include "evt/evt_cmd.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "unit/party/unit_party_vivian.h"
#include "unit/party/unit_party_yoshi.h"
#include "unit/party/unit_party_christine.h"
#include "unit/party/unit_party_chuchurina.h"
#include "unit/party/unit_party_clauda.h"
#include "unit/party/unit_party_nokotarou.h"
#include "unit/party/unit_party_sanders.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
USER_FUNC(mario_shadow_guard_check);
USER_FUNC(yoshi_original_color_anim_set);
static USER_FUNC(_krb_get_dir);
static USER_FUNC(vivian_comeback);
static USER_FUNC(eff_scanning);
extern EvtWord btl_majolyne_zoom[];
extern EvtWord btl_marilyn_zoom[];
extern EvtWord btl_vivian_zoom[];
extern EvtWord majo_unison_phase_event[];
extern BattleUnitKind unit_boss_majolyne_las;
extern BattleUnitKind unit_boss_marilyn_las;

BattleUnitKind unit_boss_rampell_las;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_rampell_attack;
static BattleUnitSetup entry_marilyn;
static BattleUnitSetup entry_majolyne;
EvtWord mutate_event[584];
static EvtWord init_event[198];
static EvtWord entry_event[46];
static EvtWord damage_event[22];
static EvtWord dead_event[261];
static EvtWord unison_phase_event[87];
static EvtWord phase_event[68];
static EvtWord attack_event[29];
static EvtWord ogg_attack_event[491];
static EvtWord wait_event[6];
static BattleUnitDefense defence_fmario;
static BattleUnitDefenseAttr defence_attr_fmario;
static StatusVulnerability regist_fmario;
static PoseTableEntry pose_table_fmario_stay[15];
static BattleDataEntry data_table_fmario[2];
static BattleUnitKindPart parts_fmario[1];
static BattleWeapon weapon_masao_jump;
static BattleWeapon weapon_masao_hammer;
static BattleWeapon weapon_masao_kururin_jump;
static BattleWeapon weapon_masao_kaiten_hammer;
static EvtWord mario_zoom[19];
static EvtWord enemy_zoom[19];
static EvtWord fmario_zoom[19];
static EvtWord gparty_zoom[19];
static EvtWord init_event_fmario[70];
static EvtWord dead_event_fmario[80];
static EvtWord attack_event_fmario[52];
static EvtWord jump_attack_voice[36];
static EvtWord marioAttackEvent_NormalJump[344];
static EvtWord marioAttackEvent_KururinJump[469];
static EvtWord marioAttackEvent_NormalHammer[299];
static EvtWord marioAttackEvent_KaitenHammer[299];
static EvtWord wait_event_fmario[6];
static BattleWeapon enemyWeapon_ChristineNormalAttack;
static BattleUnitDefense defence_christine;
static BattleUnitDefenseAttr defence_attr_christine;
static StatusVulnerability regist_christine;
static BattleDataEntry data_table_christine[2];
static BattleUnitKindPart parts_christine[1];
static EvtWord fmario_zoom_christine[19];
static EvtWord gparty_zoom_christine[19];
static EvtWord init_event_christine[72];
static EvtWord dead_event_christine[80];
static EvtWord attack_event_christine[690];
static EvtWord wait_event_christine[6];
static BattleWeapon enemyWeapon_NokotarouKouraAttack;
static BattleWeapon enemyWeapon_NokotarouShubibinAttack;
static BattleUnitDefense defence_nokotarou;
static BattleUnitDefenseAttr defence_attr_nokotarou;
static BattleUnitDefense defence_turn_nokotarou;
static StatusVulnerability regist_nokotarou;
static BattleDataEntry data_table_nokotarou[3];
static BattleUnitKindPart parts_nokotarou[1];
static EvtWord dmg_turn_event[64];
static EvtWord wakeup_event[67];
static EvtWord init_event_nokotarou[92];
static EvtWord damage_event_nokotarou[6];
static EvtWord dead_event_nokotarou[80];
static EvtWord _change_koura_pose[34];
static EvtWord _change_koura_pose_fast[34];
static EvtWord _restore_koura_pose[47];
static EvtWord _koura_rotate_start[99];
static EvtWord _koura_rotate_stop[13];
static EvtWord attack_event_nokotarou[33];
static EvtWord attack_event_nokotarou_koura[393];
static EvtWord attack_event_nokotarou_shubibin[339];
static EvtWord wait_event_nokotarou[6];
static BattleWeapon enemyWeapon_ClaudaNormalAttack;
static BattleUnitDefense defence_clauda;
static BattleUnitDefenseAttr defence_attr_clauda;
static StatusVulnerability regist_clauda;
static BattleDataEntry data_table_clauda[2];
static BattleUnitKindPart parts_clauda[1];
static EvtWord init_event_clauda[80];
static EvtWord dead_event_clauda[80];
static EvtWord attack_event_clauda[393];
static EvtWord wait_event_clauda[6];
static BattleWeapon enemyWeapon_YoshiNormalAttack;
static BattleUnitDefense defence_yoshi;
static BattleUnitDefenseAttr defence_attr_yoshi;
static StatusVulnerability regist_yoshi;
static BattleDataEntry data_table_yoshi[2];
static BattleUnitKindPart parts_yoshi[1];
static EvtWord init_event_yoshi[54];
static EvtWord dead_event_yoshi[80];
static EvtWord attack_event_yoshi[748];
static EvtWord wait_event_yoshi[6];
BattleUnitDefense defence_vivian;
BattleUnitDefenseAttr defence_attr_vivian;
StatusVulnerability regist_vivian;
BattleDataEntry data_table_vivian[2];
BattleUnitKindPart parts_vivian[1];
BattleWeapon weapon_vivian_kagenuke_attack;
static EvtWord vivian_down[19];
static EvtWord vivian_up[27];
EvtWord init_event_vivian[64];
EvtWord dead_event_vivian[80];
EvtWord attack_event_vivian[222];
EvtWord wait_event_vivian[6];
BattleUnitDefense defence_sanders;
BattleUnitDefenseAttr defence_attr_sanders;
StatusVulnerability regist_sanders;
BattleDataEntry data_table_sanders[2];
BattleUnitKindPart parts_sanders[1];
BattleWeapon enemyWeapon_SandersNormalAttack;
EvtWord init_event_sanders[51];
EvtWord dead_event_sanders[80];
EvtWord attack_event_sanders[24];
EvtWord attack_event_sanders_normal[325];
EvtWord wait_event_sanders[6];
BattleUnitDefense defence_chuchurina;
BattleUnitDefenseAttr defence_attr_chuchurina;
StatusVulnerability regist_chuchurina;
BattleDataEntry data_table_chuchurina[2];
BattleUnitKindPart parts_chuchurina[1];
BattleWeapon enemyWeapon_ChuchurinaNormalAttackR;
BattleWeapon enemyWeapon_ChuchurinaNormalAttackL;
BattleWeapon enemyWeapon_ChuchurinaNormalAttackRlast;
BattleWeapon enemyWeapon_ChuchurinaNormalAttackLlast;
EvtWord init_event_chuchurina[51];
EvtWord dead_event_chuchurina[80];
EvtWord attack_event_chuchurina[24];
EvtWord attack_event_chuchurina_normal[831];
EvtWord wait_event_chuchurina[6];

BattleUnitKind unit_boss_rampell_las = {
    UNIT_DOOPLISS_CHAPTER8, // mUnitType
    "btl_un_rampell", // name
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
    50, // width
    48, // height
    {4, 48}, // hitOffset
    {0.0f, 22.0f, 0.0f}, // centerOffset
    {0, -3}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {25.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {18.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 32.0f, 0.0f}, // kissHitOffset
    {7.0f, 20.0f, 0.0f}, // cutBaseOffset
    50.0f, // cutWidth
    48.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_RNPL_DAMAGED1", // damageSfxName
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
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_2"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_rampell", "c_ranpel", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

static BattleWeapon weapon_rampell_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FLIP_SHELLED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
    &unit_boss_marilyn_las, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static BattleUnitSetup entry_majolyne = {
    &unit_boss_majolyne_las, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

EVT_BEGIN(mutate_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 12, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_sub_random, 1, LW(5))
        CASE_EQUAL(1)
        SET_VALUE(LW(5), 1)
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(2))
        SWITCH(LW(2))
            CASE_EQUAL(UNIT_GOOMBELLA)
            SET_VALUE(LW(1), 2)
            CASE_EQUAL(UNIT_KOOPS)
            SET_VALUE(LW(1), 3)
            CASE_EQUAL(UNIT_YOSHI)
            SET_VALUE(LW(1), 4)
            CASE_EQUAL(UNIT_FLURRIE)
            SET_VALUE(LW(1), 5)
            CASE_EQUAL(UNIT_VIVIAN)
            SET_VALUE(LW(1), 6)
            CASE_EQUAL(UNIT_BOBBERY)
            SET_VALUE(LW(1), 7)
            CASE_EQUAL(UNIT_MS_MOWZ)
            SET_VALUE(LW(1), 8)
        END_SWITCH
        IF_INT_EQUAL(LW(0), LW(1))
            SET_VALUE(LW(5), 0)
        ELSE
            CALL(evt_sub_random, 1, LW(5))
        END_IF
    END_SWITCH
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 14, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 14, 0)
        DELETE_EVENT(LW(0))
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_moveto, 0, LW(10), 65, 332, LW(10), 30, -340, 30, 11)
    WAIT_FRAMES(37)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 12, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_ARM_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
        WAIT_MS(600)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_ARM_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
        WAIT_MS(600)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_ARM_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
        WAIT_MS(200)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_LAUGH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3B"))
        WAIT_MS(800)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_EYE_SHINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_FLOAT_VALUE(LW(7), FLOAT(-6.0f))
        MULTIPLY_FLOAT(LW(7), LW(6))
        ADD_FLOAT(LW(0), LW(7))
        SET_FLOAT_VALUE(LW(7), FLOAT(33.0f))
        MULTIPLY_FLOAT(LW(7), LW(6))
        ADD_FLOAT(LW(1), LW(7))
        CALL(evt_eff, STRING(""), STRING("toge_flush"), 3, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
        SET_FLOAT_VALUE(LW(7), FLOAT(7.0f))
        MULTIPLY_FLOAT(LW(7), LW(6))
        ADD_FLOAT(LW(0), LW(7))
        SET_FLOAT_VALUE(LW(7), FLOAT(-1.0f))
        MULTIPLY_FLOAT(LW(7), LW(6))
        ADD_FLOAT(LW(1), LW(7))
        CALL(evt_eff, STRING(""), STRING("toge_flush"), 3, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1000)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_TRANSFORM4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, -1000, -1000, 0)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -110, 65, 332, -110, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    SWITCH(LW(5))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 1)
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        ADD(LW(0), 2)
        ADD(LW(1), -5)
        ADD(LW(2), 5)
        CALL(eff_scanning, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(200)
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(1300)
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, LW(10), 65, 332, LW(10), 30, -340, 30, 11)
        WAIT_FRAMES(37)
        CALL_CHILD(init_event_fmario)
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        ADD(LW(0), 2)
        ADD(LW(1), -5)
        ADD(LW(2), 5)
        CALL(eff_scanning, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(200)
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(1300)
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_BOSS_RNPL_TRANSFORM3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, LW(10), 65, 332, LW(10), 30, -340, 30, 11)
        WAIT_FRAMES(37)
        CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(UNIT_GOOMBELLA)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 2)
            CALL_CHILD(init_event_christine)
            CASE_EQUAL(UNIT_KOOPS)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 3)
            CALL_CHILD(init_event_nokotarou)
            CASE_EQUAL(UNIT_YOSHI)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 4)
            CALL_CHILD(init_event_yoshi)
            CASE_EQUAL(UNIT_FLURRIE)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 5)
            CALL_CHILD(init_event_clauda)
            CASE_EQUAL(UNIT_VIVIAN)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 6)
            CALL_CHILD(init_event_vivian)
            CASE_EQUAL(UNIT_BOBBERY)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 7)
            CALL_CHILD(init_event_sanders)
            CASE_EQUAL(UNIT_MS_MOWZ)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 8)
            CALL_CHILD(init_event_chuchurina)
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -50, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(5), LW(0), LW(7))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, 65)
    CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, 43)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 12, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 13, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 14, 0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, GW(10))
    CALL(btlevtcmd_SpawnUnit, GW(9), PTR(&entry_marilyn), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 55)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, GW(9), 4, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 5, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 6, 0)
    CALL(btlevtcmd_SetUnitWork, GW(9), 7, 0)
    CALL(btlevtcmd_SpawnUnit, GW(8), PTR(&entry_majolyne), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 110)
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
    SET_VALUE(GW(14), 8)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_MOVE1L"), STRING("SFX_BOSS_RNPL_MOVE1R"), 0, 15, 15)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_MOVE1L"), STRING("SFX_BOSS_RNPL_MOVE1R"), 0, 8, 8)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    WAIT_FRAMES(60)
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(btl_majolyne_zoom))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_00"), 0, GW(8))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_marilyn_zoom))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_01"), 0, GW(9))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_vivian_zoom))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_02"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
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
    CALL(btlevtcmd_WaitAttackEnd)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), UNIT_DOOPLISS_CHAPTER8)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_TRANSFORM4"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(300)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table))
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
        CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts), 1)
        CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 135)
        CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist))
        CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
        CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
        CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("D_2"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("D_2"))
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
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL_CHILD(btl_vivian_zoom)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_13"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            GOTO(90)
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL_CHILD(btl_vivian_zoom)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_10"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    SET_VALUE(GW(13), 2)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(subsetevt_dead_core_nospin_norotate)
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        DEBUG_PUT_MSG(STRING("ランペル　ユニゾンフェーズ"))
        CALL_CHILD(PTR(majo_unison_phase_event))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 4, LW(3))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(4))
        IF_INT_NOT_EQUAL(LW(3), LW(4))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, LW(4))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
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
            CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        END_IF
        DEBUG_PUT_MSG(STRING("ランペル　ユニゾンフェーズ終了"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 13, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(mario_shadow_guard_check, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 12, LW(0))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
                        CALL_CHILD(PTR(mutate_event))
                        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                        ELSE
                        CALL(evt_sub_random, 10, LW(0))
                        IF_INT_LESS(LW(0), 3)
                            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
                            CALL_CHILD(PTR(mutate_event))
                            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 13, 1)
    SET_VALUE(LW(9), PTR(&weapon_rampell_attack))
    CALL_CHILD(PTR(ogg_attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(ogg_attack_event)
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(0), 120)
    ADD(LW(1), 70)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(2.0f))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.5f))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    END_SIBLING_EVENT
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 50, 7, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(0.75f))
    WAIT_MS(200)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 135, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(80)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(2.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.009765625f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
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
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(2.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_1"))
    WAIT_MS(800)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(2.0f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 135, 0, 20)
        LOOP(0)
            CALL(evt_sub_intpl_get_value_para, LW(0), LW(1))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("J_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 60)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.75f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_RNPL_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(2.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static BattleUnitDefense defence_fmario = {0};

static BattleUnitDefenseAttr defence_attr_fmario = {0};

static StatusVulnerability regist_fmario = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

static PoseTableEntry pose_table_fmario_stay[] = {
    {POSE_TYPE_SLEEP, "EM_N_1"},
    {POSE_TYPE_STOP, "EM_Y_2"},
    {POSE_TYPE_FREEZE, "EM_Y_2"},
    {POSE_TYPE_CONFUSE, "EM_K_1"},
    {POSE_TYPE_POISON, "EM_K_1"},
    {POSE_TYPE_DIZZY, "EM_K_1"},
    {POSE_TYPE_NORMAL, "EM_S_1"},
    {POSE_TYPE_DAMAGE, "EM_D_1"},
    {POSE_TYPE_ATTACK, "EM_A_1"},
    {POSE_TYPE_RUN, "EM_R_1"},
    {POSE_TYPE_WALK, "EM_W_1"},
    {POSE_TYPE_DANGER, "EM_D_3"},
    {POSE_TYPE_PERIL, "EM_D_3"},
    {POSE_TYPE_TALK, "EM_T_1"},
    {POSE_TYPE_ACTION, "EM_S_1"},
};

static BattleDataEntry data_table_fmario[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_fmario},
    {0},
};

static BattleUnitKindPart parts_fmario[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_mario", // modelName
        {0}, // positionOffset
        {3.0f, 27.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_fmario, // defense
        &defence_attr_fmario, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_fmario_stay, // poseTable
    },
};

static BattleWeapon weapon_masao_jump = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleWeapon weapon_masao_hammer = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static BattleWeapon weapon_masao_kururin_jump = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleWeapon weapon_masao_kaiten_hammer = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static EVT_BEGIN(mario_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -110, 65, 332, -110, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(enemy_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 115, 65, 332, 115, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(fmario_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 90, 65, 332, 90, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(gparty_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 130, 65, 332, 130, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(init_event_fmario)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_fmario_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_fmario))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_fmario), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 136)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_fmario))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_fmario))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_fmario))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    SET_VALUE(GW(17), -1)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_fmario)
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_K_1"))
    WAIT_FRAMES(152)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_fmario)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_masao_jump))
        CALL_CHILD(PTR(marioAttackEvent_NormalJump))
        CASE_EQUAL(1)
        SET_VALUE(LW(9), PTR(&weapon_masao_hammer))
        CALL_CHILD(PTR(marioAttackEvent_NormalHammer))
        CASE_EQUAL(2)
        SET_VALUE(LW(9), PTR(&weapon_masao_kururin_jump))
        CALL_CHILD(PTR(marioAttackEvent_KururinJump))
        CASE_EQUAL(3)
        SET_VALUE(LW(9), PTR(&weapon_masao_kaiten_hammer))
        CALL_CHILD(PTR(marioAttackEvent_KaitenHammer))
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(jump_attack_voice)
    CALL(evt_sub_random, 2, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VOICE_NM_AC1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VOICE_NM_AC1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_VOICE_NM_AC1_5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_NormalJump)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
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
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
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
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_KururinJump)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
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
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
        CALL_CHILD(jump_attack_voice)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_Z_2"))
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP_KURURIN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
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
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_NormalHammer)
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1A"))
    WAIT_MS(66)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_HAMMER_FURU2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(233)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1B"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1C"))
    WAIT_MS(50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_HAMMER_DON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(100)
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
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_S_1"))
    WAIT_MS(200)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_KaitenHammer)
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1A"))
    WAIT_MS(66)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_HAMMER_FURU2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(233)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1B"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_A_1C"))
    WAIT_MS(50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_HAMMER_DON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(100)
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
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_S_1"))
    WAIT_MS(200)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_fmario)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static BattleWeapon enemyWeapon_ChristineNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {1, 2, 3, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleUnitDefense defence_christine = {0};

static BattleUnitDefenseAttr defence_attr_christine = {0};

static StatusVulnerability regist_christine = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

static BattleDataEntry data_table_christine[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_christine},
    {0},
};

static BattleUnitKindPart parts_christine[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_pkr", // modelName
        {0}, // positionOffset
        {15.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_christine, // defense
        &defence_attr_christine, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_christine_stay, // poseTable
    },
};

static EVT_BEGIN(fmario_zoom_christine)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 90, 58, 228, 90, 38, -20, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(gparty_zoom_christine)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 130, 58, 228, 130, 38, -20, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(init_event_christine)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_christine_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_christine))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_christine), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 137)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_christine))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_christine))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_christine))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_christine)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_christine)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_ChristineNormalAttack))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(14))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(14), -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_DrawLots, LW(0), 2, 50, 50)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(11), 0)
        CASE_EQUAL(1)
        CALL(evt_sub_random, 4, LW(11))
        ADD(LW(11), 1)
    END_SWITCH
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
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
        WAIT_FRAMES(20)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(15)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_KURI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(11))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(15)
            LOOP(6)
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                MULTIPLY(LW(0), -30)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 3, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
            SET_VALUE(LW(8), 0)
            LOOP(36)
                CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(12), LW(13), LW(14))
                CALL(_krb_get_dir, LW(5), LW(6), LW(12), LW(13), LW(8))
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(8))
                SET_FLOAT_VALUE(LW(5), LW(12))
                SET_FLOAT_VALUE(LW(6), LW(13))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
    END_IF
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
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_S_1"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        WAIT_MS(500)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.07910156f), FLOAT(-0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, FLOAT(162.5f))
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_INLINE_EVENT
    IF_INT_LESS(LW(10), LW(11))
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 0, LW(5))
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    LABEL(90)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        IF_INT_LESS(LW(6), 0)
            MULTIPLY(LW(6), -1)
        END_IF
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        DIVIDE(LW(6), 15)
        SET_VALUE(LW(7), 15)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
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
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_christine)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static BattleWeapon enemyWeapon_NokotarouKouraAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static BattleWeapon enemyWeapon_NokotarouShubibinAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x2004, // targetWeightFlags
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

static BattleUnitDefense defence_nokotarou = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr_nokotarou = {0};

static BattleUnitDefense defence_turn_nokotarou = {0};

static StatusVulnerability regist_nokotarou = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
    100, // huge
    60, // tiny
    100, // attackUp
    60, // attackDown
    100, // defenseUp
    100, // defenseDown
    40, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    5, // ohko
};

static BattleDataEntry data_table_nokotarou[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_nokotarou},
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {0},
};

static BattleUnitKindPart parts_nokotarou[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_pnk", // modelName
        {0}, // positionOffset
        {15.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_nokotarou, // defense
        &defence_attr_nokotarou, // defenseAttr
        kMainPart | kGuardTargetPart | kPartShelled, // attributes
        0, // counterAttributes
        pose_table_nokotarou_stay, // poseTable
    },
};

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn_nokotarou))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_D_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_nokotarou))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_stay))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_S_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(init_event_nokotarou)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_nokotarou_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_nokotarou))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_nokotarou), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 138)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_nokotarou))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_nokotarou))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_nokotarou))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_nokotarou))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_nokotarou)
    CALL_CHILD(PTR(_koura_rotate_stop))
    CALL_CHILD(PTR(damage_event))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_nokotarou)
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
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

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
    RETURN
EVT_END()

static EVT_BEGIN(_restore_koura_pose)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    BEGIN_INLINE_EVENT
        LABEL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 8, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(11))
        IF_INT_NOT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(10))
            MULTIPLY(LW(10), -1)
            MULTIPLY(LW(0), -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, LW(10))
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(10))
        IF_INT_GREATER_OR_EQUAL(LW(10), 0)
            ADD(LW(0), 1)
            IF_INT_GREATER(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
            ELSE
            SUBTRACT(LW(0), 1)
            IF_INT_LESS(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
        END_IF
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
        LABEL(10)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(_koura_rotate_stop)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 2, 50, 50)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(attack_event_nokotarou_koura))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(attack_event_nokotarou_shubibin))
    END_SWITCH
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou_koura)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouKouraAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK1"), EVT_NULLPTR, 0, LW(13))
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_FRAMES(30)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(13))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(13), -1, 0)
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
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL_CHILD(_koura_rotate_stop)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(90)
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
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou_shubibin)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouShubibinAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN1"), EVT_NULLPTR, 0, LW(13))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_MS(500)
    DELETE_EVENT(LW(15))
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -400, LW(1), LW(2), 0, -1, 0)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL_CHILD(_koura_rotate_stop)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_nokotarou)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static BattleWeapon enemyWeapon_ClaudaNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 4, 6, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleUnitDefense defence_clauda = {0};

static BattleUnitDefenseAttr defence_attr_clauda = {0};

static StatusVulnerability regist_clauda = {
    40, // sleep
    50, // stop
    100, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

static BattleDataEntry data_table_clauda[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_clauda},
    {0},
};

static BattleUnitKindPart parts_clauda[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_windy", // modelName
        {0}, // positionOffset
        {15.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_clauda, // defense
        &defence_attr_clauda, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_clauda_stay, // poseTable
    },
};

static EVT_BEGIN(init_event_clauda)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_clauda_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_clauda))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_clauda), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 140)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_clauda))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_clauda))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_clauda))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 3)
    CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_clauda)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_clauda)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_ClaudaNormalAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_A1_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CLAUD_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetResultAC, LW(5))
    IF_BITS_CLEAR(LW(5), 2)
        CALL(evt_sub_random, 40, LW(5))
        SUBTRACT(LW(5), 20)
        ADD(LW(0), LW(5))
        CALL(evt_sub_random, 20, LW(5))
        SUBTRACT(LW(5), 10)
        ADD(LW(1), LW(5))
    END_IF
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_A1_2"))
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
        WAIT_FRAMES(30)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CLAUD_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(700)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_clauda)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static BattleWeapon enemyWeapon_YoshiNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {1, 1, 1, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleUnitDefense defence_yoshi = {0};

static BattleUnitDefenseAttr defence_attr_yoshi = {0};

static StatusVulnerability regist_yoshi = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

static BattleDataEntry data_table_yoshi[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_yoshi},
    {0},
};

static BattleUnitKindPart parts_yoshi[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_babyyoshi", // modelName
        {0}, // positionOffset
        {15.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_yoshi, // defense
        &defence_attr_yoshi, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_yoshi_stay, // poseTable
    },
};

static EVT_BEGIN(init_event_yoshi)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_yoshi_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_yoshi))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_yoshi), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 139)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_yoshi))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_yoshi))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_yoshi))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(yoshi_original_color_anim_set, UNIT_ID_SELF)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_yoshi)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_yoshi)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_YoshiNormalAttack))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_R_5"))
    WAIT_FRAMES(25)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1A"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    ADD(LW(1), 35)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    ADD(LW(7), 40)
    SET_VALUE(LW(10), FLOAT(1.0f))
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 190, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 381, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 668, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 859, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1145, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1241, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1527, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1814, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 2100, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 2196, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1742, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1527, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_INLINE_EVENT
    CALL(btl_yoshi_yoroyoro_jump_calc_param, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btl_yoshi_yoroyoro_jump_move, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    DELETE_EVENT(LW(13))
    CALL(evt_sub_random, 4, LW(11))
    ADD(LW(11), 1)
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
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
    END_SWITCH
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
        WAIT_FRAMES(20)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1B"))
        SET_VALUE(LW(0), 1)
        SET_VALUE(LW(1), 9)
        MULTIPLY(LW(15), -40)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(15)
        LOOP(LW(0))
            SET_VALUE(LW(2), LW(1))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_YOSHI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            LOOP(LW(2))
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
                WAIT_FRAMES(1)
            END_LOOP
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_1"))
        WAIT_FRAMES(15)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_1"))
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
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
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(15), LW(1))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        SUBTRACT(LW(15), LW(1))
        DIVIDE(LW(15), 10)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_D_1"))
        CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, -7, 0)
        CALL(evt_btl_camera_shake_h, 0, 4, 0, 10, 13)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_5"))
        CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(54)
        GOTO(95)
    END_IF
    IF_INT_LESS(LW(10), LW(11))
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 0, LW(5))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
        ADD(LW(1), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_D_1"))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(20)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_yoshi)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

BattleUnitDefense defence_vivian = {0};

BattleUnitDefenseAttr defence_attr_vivian = {0};

StatusVulnerability regist_vivian = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

BattleDataEntry data_table_vivian[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_vivian},
    {0},
};

BattleUnitKindPart parts_vivian[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_vivian", // modelName
        {0}, // positionOffset
        {15.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // reserved_32
        255, // baseAlpha
        &defence_vivian, // defense
        &defence_attr_vivian, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_vivian_stay, // poseTable
    },
};

BattleWeapon weapon_vivian_kagenuke_attack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {3, 4, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

EVT_BEGIN(init_event_vivian)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_vivian_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_vivian))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_vivian), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 141)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_vivian))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_vivian))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_vivian))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    SPAWN_EVENT_GET_ID(PTR(vivian_shadow_tail_event), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 14, LW(0))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(dead_event_vivian)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_vivian)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_vivian_kagenuke_attack))
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
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL_CHILD(vivian_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(vivian_up)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A2_2"))
    WAIT_MS(300)
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
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    CALL_CHILD(vivian_down)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL_CHILD(vivian_up)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(wait_event_vivian)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

BattleUnitDefense defence_sanders = {0};

BattleUnitDefenseAttr defence_attr_sanders = {0};

StatusVulnerability regist_sanders = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    100, // freeze
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

BattleDataEntry data_table_sanders[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_sanders},
    {0},
};

BattleUnitKindPart parts_sanders[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_bomt_n", // modelName
        {0}, // positionOffset
        {16.0f, 24.0f, 5.0f}, // hitBaseOffset
        {0.0f, 24.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        24, // reserved_32
        255, // baseAlpha
        &defence_sanders, // defense
        &defence_attr_sanders, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_sanders_stay, // poseTable
    },
};

BattleWeapon enemyWeapon_SandersNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {4, 5, 6, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
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

EVT_BEGIN(init_event_sanders)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_sanders_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_sanders))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_sanders), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 142)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_sanders))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_sanders))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_sanders))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(dead_event_sanders)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_sanders)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_SandersNormalAttack))
    CALL_CHILD(PTR(attack_event_sanders_normal))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_sanders_normal)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetWidth, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(6), LW(15))
    DIVIDE(LW(6), 2)
    MULTIPLY_FLOAT(LW(15), 20)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(15))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_BTL_THUNDERS_ATTACK1"), 0, 0, -1, -1, 0, 0)
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 90, -2, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_THUNDERS_ATTACK2"), LW(14))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    WAIT_FRAMES(60)
    CALL(evt_snd_sfxoff, LW(14))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_THUNDERS_ATTACK3"), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(6))
    DIVIDE(LW(6), 2)
    ADD(LW(1), LW(6))
    CALL(evt_eff64, STRING(""), STRING("bomb_n64"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        GOTO(92)
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(92)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("J_1A"))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("J_1B"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 90, MOVE_SOUND_FROM_TABLE)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

EVT_BEGIN(wait_event_sanders)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

BattleUnitDefense defence_chuchurina = {0};

BattleUnitDefenseAttr defence_attr_chuchurina = {0};

StatusVulnerability regist_chuchurina = {
    40, // sleep
    50, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    30, // freeze
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

BattleDataEntry data_table_chuchurina[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_chuchurina},
    {0},
};

BattleUnitKindPart parts_chuchurina[] = {
    {
        1, // partNum
        "btl_un_rampell", // partName
        "c_tyutyu", // modelName
        {0}, // positionOffset
        {16.0f, 24.0f, 5.0f}, // hitBaseOffset
        {0.0f, 24.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        24, // reserved_32
        255, // baseAlpha
        &defence_chuchurina, // defense
        &defence_attr_chuchurina, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_chuchurina_stay, // poseTable
    },
};

BattleWeapon enemyWeapon_ChuchurinaNormalAttackR = {
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
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
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

BattleWeapon enemyWeapon_ChuchurinaNormalAttackL = {
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
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
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

BattleWeapon enemyWeapon_ChuchurinaNormalAttackRlast = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 4, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

BattleWeapon enemyWeapon_ChuchurinaNormalAttackLlast = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 4, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

EVT_BEGIN(init_event_chuchurina)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(11), PTR(pose_table_chuchurina_stay))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_chuchurina))
    CALL(btlevtcmd_ReplaceParts, UNIT_ID_SELF, LW(11), PTR(parts_chuchurina), 1)
    CALL(btlevtcmd_ChangeKind, UNIT_ID_SELF, 143)
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_chuchurina))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_chuchurina))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_chuchurina))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_KIND_CHANGED)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(dead_event_chuchurina)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL_CHILD(dead_event)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_chuchurina)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_ChuchurinaNormalAttackR))
    CALL_CHILD(PTR(attack_event_chuchurina_normal))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_chuchurina_normal)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_GetWidth, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(6), LW(5))
    DIVIDE(LW(6), 2)
    MULTIPLY_FLOAT(LW(5), 12)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(5))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A1_1"))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            ADD(LW(6), 1)
            SET_VALUE(LW(0), LW(6))
            DIVIDE(LW(0), 2)
            MODULO(LW(0), 2)
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0498047f), FLOAT(1.0498047f), FLOAT(1.0f))
            ELSE
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        SET_VALUE(LW(0), 0)
        LOOP(LW(5))
            ADD(LW(0), 10)
            SET_VALUE(LW(1), 255)
            SET_VALUE(LW(2), 190)
            MULTIPLY(LW(2), LW(0))
            DIVIDE(LW(2), 100)
            SUBTRACT(LW(1), LW(2))
            CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, LW(1), LW(1))
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(1000)
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    SET_VALUE(LW(15), 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, 255, 255)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(8), -75)
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
        IF_INT_GREATER_OR_EQUAL(LW(0), 180)
            SUBTRACT(LW(0), 360)
        END_IF
        CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, LW(0), LW(8), 8)
        LOOP(8)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(8), -60)
        SET_VALUE(LW(9), -75)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 4)
        LOOP(4)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(evt_sub_random, 2, LW(14))
    ADD(LW(14), 2)
    SET_VALUE(LW(13), 18)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    ADD(LW(10), 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A1_2"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 2)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(6))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    END_IF
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
        LOOP(20)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 18, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(36)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 10, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A2_4"))
        WAIT_FRAMES(52)
        GOTO(95)
    END_IF
    SET_VALUE(LW(0), LW(10))
    MODULO(LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&enemyWeapon_ChuchurinaNormalAttackR))
        IF_INT_EQUAL(LW(10), LW(14))
            SET_VALUE(LW(12), PTR(&enemyWeapon_ChuchurinaNormalAttackRlast))
        END_IF
    ELSE
        SET_VALUE(LW(12), PTR(&enemyWeapon_ChuchurinaNormalAttackL))
        IF_INT_EQUAL(LW(10), LW(14))
            SET_VALUE(LW(12), PTR(&enemyWeapon_ChuchurinaNormalAttackLlast))
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        DELETE_EVENT(LW(15))
        SET_VALUE(LW(15), 0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(14))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHUCHURINA_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        SET_VALUE(LW(0), LW(10))
        AND(LW(0), 1)
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHUCHURINA_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        ELSE
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHUCHURINA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(10))
        CASE_LESS_OR_EQUAL(2)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            SET_VALUE(LW(7), LW(13))
            IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                SET_VALUE(LW(8), 75)
            ELSE
                SET_VALUE(LW(8), -75)
            END_IF
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                SUBTRACT(LW(0), 360)
            END_IF
            CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, LW(0), LW(8), 14)
            LOOP(14)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
            IF_INT_LESS(LW(10), LW(14))
                SET_VALUE(LW(7), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                    SET_VALUE(LW(8), 60)
                    SET_VALUE(LW(9), 75)
                ELSE
                    SET_VALUE(LW(8), -60)
                    SET_VALUE(LW(9), -75)
                END_IF
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 6)
                LOOP(6)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            ELSE
                SET_VALUE(LW(7), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                    SET_VALUE(LW(8), 0)
                    SET_VALUE(LW(9), 75)
                ELSE
                    SET_VALUE(LW(8), 0)
                    SET_VALUE(LW(9), -75)
                END_IF
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 12)
                LOOP(12)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CASE_DEFAULT
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(8), EVT_NULLPTR)
            SET_VALUE(LW(7), 360)
            IF_INT_GREATER_OR_EQUAL(LW(10), LW(14))
                SET_VALUE(LW(0), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                    SUBTRACT(LW(0), 360)
                END_IF
                IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                    SUBTRACT(LW(7), LW(0))
                ELSE
                    ADD(LW(7), LW(0))
                END_IF
            ELSE
                ADD(LW(7), 60)
                SET_VALUE(LW(0), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                    SUBTRACT(LW(0), 360)
                END_IF
                IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                    SUBTRACT(LW(7), LW(0))
                ELSE
                    ADD(LW(7), LW(0))
                END_IF
            END_IF
            IF_INT_LESS(LW(13), 0)
                MULTIPLY(LW(7), -1)
            END_IF
            CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, 0, LW(7), 20)
            LOOP(20)
                CALL(evt_sub_intpl_get_value)
                ADD(LW(0), LW(8))
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_FRAMES(10)
    END_SWITCH
    MULTIPLY(LW(13), -1)
    IF_INT_LESS(LW(10), LW(14))
        GOTO(10)
    END_IF
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

EVT_BEGIN(wait_event_chuchurina)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


static USER_FUNC(eff_scanning) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    EffectEntry* eff = effScanningEntry(0, x, y, z);

    ((effScanningData*)eff->userdata)->scale = unit->mSizeMultiplier;
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_shadow_guard_check) {
    EvtWord* args = event->args;

    if (BtlUnit_CheckShadowGuard(BattleGetMarioPtr(_battleWorkPointer)) == 1) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
static USER_FUNC(vivian_comeback) {
    pouchGetPtr()->partyData[6].flags &= ~2;
    return 1;
}

#ifdef VERSION_G8ME01
// US only: restore the moves of the unit for this turn.
USER_FUNC(boss_rampell_las_sync_max_moves) {
    BattleUnit* unit;
    s32 id = evtGetValue(event, event->args[0]);

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (unit != NULL) {
        unit->maxMovesThisTurn = unit->movesRemaining;
    }
    return EVT_RETURN_DONE;
}
#endif
static USER_FUNC(_krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 z1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 z2 = evtGetFloat(event, args[3]);
    s32 dir = evtGetValue(event, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;

    if (dx == 0.0f && dz == 0.0f) {
        evtSetValue(event, args[4], dir);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dz, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}
