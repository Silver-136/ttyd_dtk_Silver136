// Fake Mario (faker_mario): the form Doopliss takes in his Mario-disguise battle in Creepy Steeple.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "system.h"
#include "battle/unit_work.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "mario/mario_pouch.h"
#include "string.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_HP_TALK_STEP 0 // HP lines spoken: 0 none, 1 below slot 1 (stg4_jin_26_<partner>; forced to 1 when GW(20) != 0), 2 below slot 2 (stg4_jin_26_01), 3 below slot 3 (stg4_jin_26_02)
#define UNIT_WORK_HP_66 1        // 66% of max HP, threshold of the first HP line
#define UNIT_WORK_HP_50 2        // 50% of max HP, threshold of the second HP line
#define UNIT_WORK_HP_25 3        // 25% of max HP, threshold of the third HP line

typedef struct FakerStatus {
    s16 data[15];
} FakerStatus;
typedef struct FakerParam {
    f32 size;
    FakerStatus status;
    s32 statusFlags;
#if VERSION >= VERSION_US
    u16 commandDisabled[2];
#endif
    s16 currentHp;
    s16 maxHp;
    s16 currentFp;
    s16 maxFp;
} FakerParam;

static USER_FUNC(_anime_load);
static USER_FUNC(_anime_load_wait);
USER_FUNC(_gullible_party_info_init);
USER_FUNC(_load_param);
USER_FUNC(_save_param);

#if VERSION >= VERSION_US
USER_FUNC(faker_mario_sync_max_moves);
#endif

static USER_FUNC(vivian_comeback);
extern BattleUnitSetup entryunit_party[];
extern BattleUnitKind unit_gullible_christine;
extern BattleUnitKind unit_gullible_clauda;
extern BattleUnitKind unit_gullible_nokotarou;
extern BattleUnitKind unit_gullible_yoshi;

BattleUnitKind unit_faker_mario;
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
BattleUnitSetup entry_gullible_christine;
BattleUnitSetup entry_gullible_nokotarou;
BattleUnitSetup entry_gullible_clauda;
BattleUnitSetup entry_gullible_yoshi;
static EvtWord mario_zoom[19];
static EvtWord enemy_zoom[19];
static EvtWord fmario_zoom[19];
static EvtWord gparty_zoom[19];
static EvtWord init_event_fmario[189];
EvtWord fmario_entry_event[24];
EvtWord dead_event_fmario_down[53];
static EvtWord dead_event_fmario[198];
static EvtWord damage_event_fmario[22];
EvtWord phase_event_fmario[VERSION_PICK(400, 403)];
static EvtWord attack_event_fmario[60];
static EvtWord jump_attack_voice[36];
static EvtWord marioAttackEvent_NormalJump[344];
static EvtWord marioAttackEvent_KururinJump[469];
static EvtWord marioAttackEvent_NormalHammer[299];
static EvtWord marioAttackEvent_KaitenHammer[299];
EvtWord party_change_event[196];
static EvtWord wait_event_fmario[6];

/**
 * Script helper: waits until the battle animation set of the party member unit args[0] (Goombella, Koops, Yoshi
 * or Flurrie, by unit kind) has loaded; blocks until BattlePartyAnimeLoad returns true.
 */
static USER_FUNC(_anime_load_wait) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 anime;

    switch (unit->currentKind) {
        case UNIT_GOOMBELLA_CHAPTER4:
            anime = 0xE0;
            break;
        case UNIT_KOOPS_CHAPTER4:
            anime = 0xE1;
            break;
        case UNIT_FLURRIE_CHAPTER4:
            anime = 0xE3;
            break;
        case UNIT_YOSHI_CHAPTER4:
        default:
            anime = 0xE2;
            break;
    }
    if (BattlePartyAnimeLoad(anime)) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: starts loading the battle animation set of the party member unit args[0] (Goombella, Koops,
 * Yoshi or Flurrie by unit kind).
 */
static USER_FUNC(_anime_load) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 anime;

    switch (unit->currentKind) {
        case UNIT_GOOMBELLA_CHAPTER4:
            anime = 0xE0;
            break;
        case UNIT_KOOPS_CHAPTER4:
            anime = 0xE1;
            break;
        case UNIT_FLURRIE_CHAPTER4:
            anime = 0xE3;
            break;
        case UNIT_YOSHI_CHAPTER4:
        default:
            anime = 0xE2;
            break;
    }
    BattlePartyAnimeLoad(anime);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: saves the HP, FP, size, status
 * and (US and later) disabled commands of the party member unit args[1] into the fake Mario's table,
 * so they are kept while that partner is swapped out.
 */
USER_FUNC(_save_param) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id1 = evtGetValue(event, args[0]);
    s32 id2 = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id1));
    BattleUnit* party = BattleGetUnitPtr(wp, BattleTransID(event, id2));
    FakerParam* params = (FakerParam*)unit->countersetWeapon;
    FakerParam* p;

    switch (party->currentKind) {
        case UNIT_GOOMBELLA_CHAPTER4:
            p = &params[0];
            break;
        case UNIT_KOOPS_CHAPTER4:
            p = &params[1];
            break;
        case UNIT_FLURRIE_CHAPTER4:
            p = &params[2];
            break;
        case UNIT_YOSHI_CHAPTER4:
            p = &params[3];
            break;
    }
    p->size = party->sizeMultiplier;
    p->statusFlags = party->statusFlags;
#if VERSION >= VERSION_US
    p->commandDisabled[0] = party->usCommandDisabled[0];
    p->commandDisabled[1] = party->usCommandDisabled[1];
#endif
    p->status = *(FakerStatus*)&party->sleepTurns;
    p->maxHp = party->maxHp;
    p->currentHp = party->currentHp;
    p->maxFp = party->maxFp;
    p->currentFp = party->currentFp;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: restores the saved HP, FP, size,
 * status and (US and later) disabled commands of the party member unit args[1] from the fake Mario's table
 * when the partner is swapped back in.
 */
USER_FUNC(_load_param) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id1 = evtGetValue(event, args[0]);
    s32 id2 = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id1));
    BattleUnit* party = BattleGetUnitPtr(wp, BattleTransID(event, id2));
    FakerParam* params = (FakerParam*)unit->countersetWeapon;
    FakerParam* p;

    switch (party->currentKind) {
        case UNIT_GOOMBELLA_CHAPTER4:
            p = &params[0];
            break;
        case UNIT_KOOPS_CHAPTER4:
            p = &params[1];
            break;
        case UNIT_FLURRIE_CHAPTER4:
            p = &params[2];
            break;
        case UNIT_YOSHI_CHAPTER4:
            p = &params[3];
            break;
    }
    party->sizeMultiplier = p->size;
    party->statusFlags = p->statusFlags;
#if VERSION >= VERSION_US
    party->usCommandDisabled[0] = p->commandDisabled[0];
    party->usCommandDisabled[1] = p->commandDisabled[1];
#endif
    *(FakerStatus*)&party->sleepTurns = p->status;
    party->maxHp = p->maxHp;
    party->currentHp = p->currentHp;
    party->maxFp = p->maxFp;
    party->currentFp = p->currentFp;
    return EVT_RETURN_DONE;
}

/**
 * Unit kind of Doopliss as Mario:
 * 40 HP, level 54, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_faker_mario = {
    UNIT_DOOPLISS_CHAPTER4_FIGHT2, // unitType
    "btl_un_rampell", // name
    40, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    54, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    70, // runRate
    7, // pbSoftCap
    24, // width
    40, // height
    {12, 40}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {12.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {12.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 32.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    24.0f, // cutWidth
    40.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_MARIO_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist_fmario, // statusVulnerability
    1, // numParts
    parts_fmario, // parts
    init_event_fmario, // initEventCode
    data_table_fmario, // dataTable
};

/** Defence of Doopliss as Mario's body per damage type: 0 against every damage type. */
static BattleUnitDefense defence_fmario = {{0, 0, 0, 0, 0}};

/** Defence attributes of Doopliss as Mario's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr_fmario = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist_fmario = {50, 50, 50, 50, 50, 50, 100, 40, 100, 50, 100, 50, 100, 25, 50, 0, 0, 100, 50, 100, 100, 3};
#else
static StatusVulnerability regist_fmario = {
    50, // sleep
    50, // stop
    70, // dizzy
    50, // poison
    50, // confuse
    50, // electric
    100, // burn
    40, // freeze
    100, // huge
    70, // tiny
    100, // attackUp
    70, // attackDown
    100, // defenseUp
    90, // defenseDown
    50, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    50, // slow
    100, // dodgy
    100, // invisible
    3, // ohko
};
#endif

/**
 * Pose table of Doopliss as Mario:
 * the animation played for each pose type (idle, hurt, walk, run, attack and so on).
 */
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

/** Event table of Doopliss as Mario: its defeat script. */
static BattleDataEntry data_table_fmario[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_fmario},
    {0, NULL},
};

/** Body part of Doopliss as Mario: a single part using the model "c_mario". */
static BattleUnitKindPart parts_fmario[] = {
    {
        1, "btl_un_rampell", "c_mario",
        0.0f, 0.0f, 0.0f,
        3.0f, 27.0f, 0.0f,
        0.0f, 30.0f, 0.0f,
        20, 30, 255,
        &defence_fmario, &defence_attr_fmario,
        kMainPart | kGuardTargetPart, 0, pose_table_fmario_stay,
    },
};

/** Doopliss as Mario's jump: 2 damage to a single target, a jump-style hit. */
static BattleWeapon weapon_masao_jump = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Doopliss as Mario's hammer: 4 damage to a single target, a hammer-style hit. */
static BattleWeapon weapon_masao_hammer = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_CONFUSED_ACTION, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY,
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
#else
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
#endif
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

/** Doopliss as Mario's kururin jump: 2 damage to a single target, a jump-style hit. */
static BattleWeapon weapon_masao_kururin_jump = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Doopliss as Mario's kaiten hammer: 4 damage to a single target, a hammer-style hit. */
static BattleWeapon weapon_masao_kaiten_hammer = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_ATTACK_FX | SPECIAL_CONFUSED_ACTION, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY,
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
#else
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
#endif
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

/** Setup entry that adds the Goombella double to Doopliss's battle. */
BattleUnitSetup entry_gullible_christine = {
    &unit_gullible_christine, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Setup entry that adds the Koops double to Doopliss's battle. */
BattleUnitSetup entry_gullible_nokotarou = {
    &unit_gullible_nokotarou, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Setup entry that adds the Flurrie double to Doopliss's battle. */
BattleUnitSetup entry_gullible_clauda = {
    &unit_gullible_clauda, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Setup entry that adds the Yoshi double to Doopliss's battle. */
BattleUnitSetup entry_gullible_yoshi = {
    &unit_gullible_yoshi, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Camera shot on Mario's side of the stage (x = -110):
 * sets the battle camera to a fixed position and waits 37 frames for it to arrive.
 */
static EVT_BEGIN(mario_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -110, 65, 332, -110, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Camera shot on the enemy side (x = 115): sets the battle camera to a fixed position
 * and waits 37 frames for it to arrive.
 */
static EVT_BEGIN(enemy_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 115, 65, 332, 115, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Camera shot on the fake Mario (x = 90): sets the battle camera to a fixed position
 * and waits 37 frames for it to arrive.
 */
static EVT_BEGIN(fmario_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 90, 65, 332, 90, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Camera shot on the current partner double (x = 130): sets the battle camera to a fixed position
 * and waits 37 frames for it to arrive.
 */
static EVT_BEGIN(gparty_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 130, 65, 332, 130, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Init event of the fake Mario in the second Doopliss battle:
 * installs wait, phase, attack, damage, confusion and entry events, stores his unit id in GW(16),
 * spawns a Goombella double (entry_gullible_christine, placed next to him) as GW(17),
 * clears the GW(18) to GW(21) flags, and stores 66%,
 * 50% and 25% of his maximum HP in unit work 1 to 3 as the thresholds of his phase script.
 * Sets the Mario walk, run and jump sounds.
 */
// GW(19) tracks Faker Mario and GW(20) the staged partner: 0 alive, 1 death detected, 2 death complete.
static EVT_BEGIN(init_event_fmario)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_fmario))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event_fmario))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_fmario))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_fmario))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(fmario_entry_event))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, GW(16))
    SET_VALUE(GW(17), -1)
    CALL(_gullible_party_info_init)
    CALL(evt_sub_random, 3, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(12), PTR(&entry_gullible_christine))
        CASE_EQUAL(1)
        SET_VALUE(LW(12), PTR(&entry_gullible_nokotarou))
        CASE_EQUAL(2)
        SET_VALUE(LW(12), PTR(&entry_gullible_clauda))
        CASE_DEFAULT
        SET_VALUE(LW(12), PTR(&entry_gullible_yoshi))
    END_SWITCH

    // The random choice above is consumed, but the initial partner is always Goombella.
    SET_VALUE(GW(18), 0)
    SET_VALUE(LW(12), PTR(&entry_gullible_christine))
    CALL(btlevtcmd_SpawnUnit, LW(3), LW(12), 0)
    SET_VALUE(GW(17), LW(3))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(_load_param, UNIT_ID_SELF, LW(3))
    SET_VALUE(GW(20), 0)
    SET_VALUE(GW(19), 0)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 66)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_66, LW(1))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 50)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_50, LW(1))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 25)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_25, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, 0)
    SET_VALUE(GW(21), 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP1"), STRING("SFX_BOSS_NM_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Entrance script of the fake Mario in the second Doopliss battle:
 * the status window hides, the camera zooms in and the fake Mario speaks (stg4_jin_22).
 */
EVT_BEGIN(fmario_entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    WAIT_MS(2000)
    CALL_CHILD(fmario_zoom)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_22"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    RETURN
EVT_END()

/**
 * Defeat sequence of the fake Mario in the second battle:
 * he says stg4_jin_26_05, staggers (EM_K_1) and then falls over with a thud (EM_D_2).
 */
EVT_BEGIN(dead_event_fmario_down)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_S_1"))
    WAIT_MS(300)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("EM_T_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("EM_S_1"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_05"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_K_1"))
    WAIT_FRAMES(152)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_NM_LAND_DAMAGE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_MS(1500)
    RETURN
EVT_END()

/**
 * Death event of the fake Mario in the second battle:
 * he returns to his home position, the first time Vivian returns to the party (vivian_comeback)
 * and her unit is spawned at (-140, 0, 5).
 * Depending on GW(20) (the current partner double's death state) the camera shows him or the enemy side while
 * he says stg4_jin_26_05 and falls over (dead_event_fmario_down); while the double is still standing (GW(20) 0)
 * it then answers with its own line (stg4_jin_26_05_kur, _nok, _win or _yos). Then GW(19) is set to 2.
 */
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
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    IF_INT_EQUAL(GW(21), 0)
        SET_VALUE(GW(21), 1)
        CALL(vivian_comeback)
        CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entryunit_party[4]), 0)
        CALL(btlevtcmd_SetHomePos, LW(3), FLOAT(-140.0f), FLOAT(0.0f), FLOAT(5.0f))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_X, LW(0))
        CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_Y, LW(1))
        CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_Z, LW(2))
        WAIT_FRAMES(1)
    END_IF
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    SWITCH(GW(20))
        CASE_EQUAL(0)
        CALL_CHILD(fmario_zoom)
        CALL_CHILD(dead_event_fmario_down)
        WAIT_MS(300)
        CALL_CHILD(gparty_zoom)
        SWITCH(GW(18))
            CASE_EQUAL(0)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_05_kur"), 0, GW(17))
            CASE_EQUAL(1)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_05_nok"), 0, GW(17))
            CASE_EQUAL(2)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_05_win"), 0, GW(17))
            CASE_EQUAL(3)
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_05_yos"), 0, GW(17))
        END_SWITCH
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL_CHILD(enemy_zoom)
        CALL_CHILD(dead_event_fmario_down)
        WAIT_MS(200)
        CASE_EQUAL(2)
        CALL_CHILD(fmario_zoom)
        CALL_CHILD(dead_event_fmario_down)
        WAIT_MS(200)
    END_SWITCH
    SET_VALUE(GW(19), 2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(subsetevt_dead_core_nospin_norotate)
    RETURN
EVT_END()

/**
 * Damage event of the fake Mario in the second battle:
 * a hit with damage flag 0x400 (the hit that defeats him) sets GW(19) to 1,
 * then the default damage handling runs.
 * The partner doubles' death events read GW(19) (0 still standing, 1 defeated, 2 fallen over after
 * dead_event_fmario) to choose between their exchange with him and waiting for his fall.
 */
static EVT_BEGIN(damage_event_fmario)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(19), 1)
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Phase script of the fake Mario: before the player's phase on turn 3 Vivian rejoins Mario (stg4_jin_23 to 25;
 * she spawns at (-140, 0, 5) and the camera zooms in).
 * After the player's phase, when the partner double has been beaten (GW(20) set) party_change_event switches in the
 * next one; then one low-HP line plays per threshold crossed: below 66% of his HP the partner double warns him
 * (stg4_jin_26_kur, _nok, _win or _yos; skipped once a double has been beaten), below 50% he complains
 * (stg4_jin_26_01), below 25% he says he is nearly beaten (stg4_jin_26_02).
 */
EVT_BEGIN(phase_event_fmario)
    // Before the player's phase on turn 3: Vivian makes her choice.
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)

            // Vivian says she is sorry to her sister but Mario has been the only one kind to her.
            CALL(evt_msg_print, 0, STRING("stg4_jin_23"), 0, 0)
            WAIT_MS(200)
            SET_VALUE(GW(21), 1)

            // She rejoins Mario: she appears at (-140, 0, 5) and the camera zooms in.
            CALL(vivian_comeback)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entryunit_party[4]), 0)
            CALL(btlevtcmd_SetHomePos, LW(3), FLOAT(-140.0f), FLOAT(0.0f), FLOAT(5.0f))
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_X, LW(0))
            CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_Y, LW(1))
            CALL(btlevtcmd_SetUnitWork, LW(3), SHADOW_TAIL_WORK_Z, LW(2))
#if VERSION >= VERSION_US
            CALL(faker_mario_sync_max_moves, LW(3))
#endif
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetMarioId, LW(3), LW(5))
            CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
            CALL(btlevtcmd_StopWaitEvent, LW(3))
            CALL(btlevtcmd_StopWaitEvent, LW(5))
            WAIT_MS(700)
            SPAWN_EVENT(mario_zoom)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_VIVIAN_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_SetPos, LW(3), -140, -40, 5)
            CALL(btlevtcmd_DivePosition, LW(3), -140, 0, 5, 60, 0, EASING_LINEAR, 4, MOVE_SOUND_NONE)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_Z_1"))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 180, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))

            // Vivian: she will fight at his side from now on.
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_24"), 0, LW(3))
            WAIT_MS(200)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_FIND1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, LW(5), LW(7), LW(8), LW(10))
            CALL(btlevtcmd_JumpSetting, LW(5), 12, FLOAT(1.0f), FLOAT(1.1992188f))
            CALL(btlevtcmd_JumpPosition, LW(5), LW(7), LW(8), LW(10), 0, MOVE_SOUND_FROM_TABLE)
            WAIT_MS(1000)

            // Vivian: she has made her choice and is not turning back.
            CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_25"), 0, LW(3))
            WAIT_MS(1000)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, EASING_LINEAR, 180, 0, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            WAIT_MS(200)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_I_Y"))
            WAIT_MS(1000)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF

    // After the player's phase: if a partner double has just been beaten, the next one is switched in.
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(GW(20), 0)
            CALL_CHILD(party_change_event)
        END_IF
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, LW(2))
        IF_INT_NOT_EQUAL(GW(20), 0)
            IF_INT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, 1)
                SET_VALUE(LW(2), 1)
            END_IF
        END_IF
        SWITCH(LW(2))
            CASE_EQUAL(0)

            // The fake Mario's HP has dropped low:
            // the camera zooms to the partner double in front (Goombella, Koops, Flurrie or Yoshi)
            // and they call out to him.
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_66, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, 1)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL_CHILD(enemy_zoom)
                SWITCH(GW(18))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_kur"), 0, GW(17))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_nok"), 0, GW(17))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_win"), 0, GW(17))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_yos"), 0, GW(17))
                END_SWITCH
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_IF
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_50, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, 2)

                // The fake Mario is hurting and complains.
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_01"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_IF
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_25, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_TALK_STEP, 3)

                // The fake Mario is nearly beaten.
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg4_jin_26_02"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_IF
        END_SWITCH
    END_IF
    RETURN
EVT_END()

/**
 * Attack event of the fake Mario: uses the enemy-item event selected by the item-use check,
 * otherwise draws one of Mario's moves with weights 50/50/20/20:
 * jump, hammer, spin jump (kururin) and spin hammer (kaiten).
 */
static EVT_BEGIN(attack_event_fmario)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 4, 50, 50, 20, 20)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_masao_jump))
        CALL_CHILD(marioAttackEvent_NormalJump)
        CASE_EQUAL(1)
        SET_VALUE(LW(9), PTR(&weapon_masao_hammer))
        CALL_CHILD(marioAttackEvent_NormalHammer)
        CASE_EQUAL(2)
        SET_VALUE(LW(9), PTR(&weapon_masao_kururin_jump))
        CALL_CHILD(marioAttackEvent_KururinJump)
        CASE_EQUAL(3)
        SET_VALUE(LW(9), PTR(&weapon_masao_kaiten_hammer))
        CALL_CHILD(marioAttackEvent_KaitenHammer)
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Plays one of three Mario-style jump voices (SFX_BOSS_VOICE_NM_AC1_1, _2 or _5) at random. */
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

/**
 * The fake Mario's jump attack (weapon_masao_jump):
 * walks to the take-off point and jumps onto the target with the Mario jump voice;
 * the pre-damage check decides between a miss (he falls back down) and a hit,
 * after which he bounces back in three small hops and returns home.
 */
static EVT_BEGIN(marioAttackEvent_NormalJump)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: it picks one for the jump.
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

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // He walks to the take-off point in front of the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)

    // He jumps with a Mario jump cry and comes down on the target;
    // the pre-damage check decides between a miss and a hit.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: he falls back to the floor (the target dodges for some kinds of miss).
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)

    // Hit: the stomp lands for its damage, then he bounces off in three small hops.
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

    // He walks back to his home position.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The fake Mario's spin jump (weapon_masao_kururin_jump):
 * like the normal jump, but after the first hop he spins in the air and dives onto the target a second time.
 */
static EVT_BEGIN(marioAttackEvent_KururinJump)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: it picks one for the jump.
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

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // He walks to the take-off point in front of the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)

    // First hop: he jumps and comes down on the target; the pre-damage check decides between a miss
    // and a hit.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: he falls back to the floor (the target dodges for some kinds of miss).
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)

    // Hit: the first stomp lands for its damage.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))

    // Second hop: he jumps again, spins in the air with the spin-jump cry
    // and dives onto the target for a second hit.
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

    // He bounces off in three small hops.
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

    // He walks back to his home position.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The fake Mario's hammer attack (weapon_masao_hammer):
 * walks next to the target, raises the hammer (EM_A_1A to 1C, SFX_BOSS_NM_HAMMER_FURU2 and DON2) and swings it;
 * the pre-damage check decides between a miss and a hit, then he returns home.
 */
static EVT_BEGIN(marioAttackEvent_NormalHammer)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: it picks one for the hammer swing.
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

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)

    // He walks up beside the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // Wind-up: he raises the hammer over his head, holds it, then swings it down.
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

    // The pre-damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: the hammer hits only the floor (the target dodges for some kinds of miss).
    LABEL(90)
    GOTO(98)

    // Hit: the hammer lands for its damage.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))

    // He pauses, straightens up and walks back to his home position.
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
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The fake Mario's spinning hammer attack: the script is identical to marioAttackEvent_NormalHammer;
 * only the weapon in LW(9) (weapon_masao_kaiten_hammer, set by attack_event_fmario) differs.
 */
static EVT_BEGIN(marioAttackEvent_KaitenHammer)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: it picks one for the hammer swing.
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

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)

    // He walks up beside the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // Wind-up: he raises the hammer over his head, holds it, then swings it down.
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

    // The pre-damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: the hammer hits only the floor (the target dodges for some kinds of miss).
    LABEL(90)
    GOTO(98)

    // Hit: the hammer lands for its damage.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))

    // He pauses, straightens up and walks back to his home position.
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
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

// Advance to the next staged partner; indices outside 0..3 return without spawning.
EVT_BEGIN(party_change_event)
    ADD(GW(18), 1)
    IF_INT_GREATER(GW(18), 4)
        RETURN
    END_IF
    SWITCH(GW(18))
        CASE_EQUAL(0)
        SET_VALUE(LW(12), PTR(&entry_gullible_christine))
        CASE_EQUAL(1)
        SET_VALUE(LW(12), PTR(&entry_gullible_nokotarou))
        CASE_EQUAL(2)
        SET_VALUE(LW(12), PTR(&entry_gullible_clauda))
        CASE_EQUAL(3)
        SET_VALUE(LW(12), PTR(&entry_gullible_yoshi))
        CASE_DEFAULT
        RETURN
    END_SWITCH
    SET_VALUE(GW(20), 0)
    SET_VALUE(LW(3), GW(17))
    CALL(btlevtcmd_SpawnUnit, LW(9), LW(12), 0)
    SET_VALUE(GW(17), LW(9))
    CALL(_anime_load, LW(9))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(3), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(3), FLOAT(-0.049804688f), FLOAT(-0.049804688f), FLOAT(-0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_IN1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(5), LW(6), LW(7), 20, MOVE_SOUND_NONE)
    CALL(_anime_load_wait, LW(9))
    CALL(_save_param, UNIT_ID_SELF, LW(3))
    CALL(_load_param, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_SetPos, LW(9), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_SetFallAccel, LW(9), FLOAT(0.69921875f))
    CALL(btlevtcmd_CheckDataOfDataTable, LW(9), LW(0), UNIT_DATA_CHANGE_PARTY_SPAWN_INIT_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(9), UNIT_DATA_CHANGE_PARTY_SPAWN_INIT_EVENT)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_CHANGE1"), 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(9), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(9), FLOAT(0.049804688f), FLOAT(0.049804688f), FLOAT(0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, LW(9), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, LW(9), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_OUT1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(9), LW(0), LW(1), LW(2), 20, MOVE_SOUND_NONE)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_SortPhaseMoveTable)
    RETURN
EVT_END()

USER_FUNC(_gullible_party_info_init) {
    PouchData* pouch = pouchGetPtr();
    s32 id = BattleTransID(event, UNIT_ID_SELF);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    FakerParam* params = BattleAlloc(sizeof(FakerParam) * 4);
    s32 i;
    FakerParam* p;

    unit->countersetWeapon = (struct BattleWeapon*)params;
    memset(params, 0, sizeof(FakerParam) * 4);
    for (i = 0; i < 4; i++) {
        p = &params[i];
        p->size = 1.0f;
        p->statusFlags = 0;
        memset(&p->status, 0, sizeof(FakerStatus));
        switch (i) {
            case 0:
                p->maxHp = p->currentHp = pouch->partyData[1].baseMaxHP;
                break;
            case 1:
                p->maxHp = p->currentHp = pouch->partyData[2].baseMaxHP;
                break;
            case 2:
                p->maxHp = p->currentHp = pouch->partyData[5].baseMaxHP;
                break;
            default:
                p->maxHp = p->currentHp = pouch->partyData[4].baseMaxHP;
                break;
        }
        p->maxFp = p->currentFp = pouch->baseMaxFP;
#if VERSION >= VERSION_US
        p->commandDisabled[0] = 0;
        p->commandDisabled[1] = 0;
#endif
    }
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_US

// US/PAL: set this turn's maximum move count to the remaining-move count.
USER_FUNC(faker_mario_sync_max_moves) {
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

/** Battle script helper: clears the "away" flag in Vivian's party data so she is back in the party; returns 1. */
static USER_FUNC(vivian_comeback) {
    pouchGetPtr()->partyData[6].flags &= ~2;
    return EVT_RETURN_YIELD;
}

/** Wait (idle) event of the fake Mario: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event_fmario)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
