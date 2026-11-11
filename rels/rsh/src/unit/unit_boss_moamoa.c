// Smorg (moamoa): boss battle unit, the main body, used on the Excess Express.

#include "version.h"
#include "rsh.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "battle/battle_camera.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "driver/npcdrv.h"
#include "unit_boss_moamoa.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"

// The battle swarm is drawn through extdrv (ext_moamoa_main), so its types differ from the field swarm in moamoa.h.
typedef struct ExtMoamoaGroup {
    char anim[16];
    char pose[8];
    f32 speed;
    s32 start;
    s32 num;
} ExtMoamoaGroup;
typedef struct ExtMoamoa {
    u16 flags;
    u8 field_0x2[2];
    Vec pos;
    s16 group;
    u8 field_0x12[6];
    OSTime time;
    s16 chain;
    s16 order;
    f32 t;
    s32 poseId;
    s32 type;
    EvtWord* owner;
    Vec vel;
    Vec pos2;
    u8 field_0x4c[4];
} ExtMoamoa;
typedef struct ExtMoamoaWork {
    s32 field_0x0;
    s32 count;
    ExtMoamoa* elems;
    s32 numGroups;
    ExtMoamoaGroup groups[32];
    s16 chainNum[128];
    s16 chainIdx[128][2];
    s32 poseId[5];
    s32 unitId;
    Vec center;
    f32 angle;
    f32 radius;
    f32 targetScale;
    f32 scale;
} ExtMoamoaWork;
static ExtMoamoaWork* wp;

#if VERSION >= VERSION_DEMO_EU
// PAL: a seeded generator for the swarm's start positions (moamoa_srand / moamoa_randf)
static u32 moamoa_rand_prev;
static u32 moamoa_rand_cur;
#endif

static void moamoa_mainUnitIdClear(void);
static f32 randf(f32 min, f32 max);
static USER_FUNC(moamoa1_on);
static void moamoa_draw(CameraId cameraId, void* param);

static USER_FUNC(moamoa_on);
static USER_FUNC(moamoa_set_bodyScale);
static void ext_moamoa_main(void);
static void ext_moamoa_init(void);
static USER_FUNC(moamoa2_on);
static USER_FUNC(moamoa_bomb);
static USER_FUNC(moamoa_guruguru);
static USER_FUNC(evt_moamoa_mainUnitIdClear);
extern BattleUnitKind unit_boss_moamoa_mouth;
extern BattleUnitKind unit_boss_moamoa_tentacle_a;
extern BattleUnitKind unit_boss_moamoa_tentacle_b;
extern BattleUnitKind unit_boss_moamoa_tentacle_c;

BattleUnitKind unit_boss_moamoa;
BattleUnitDefense moamoa_defence;
BattleUnitDefense moamoa_defence_no_parts;
BattleUnitDefense moamoa2_defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
PoseTableEntry moamoa1_pose_table[4];
PoseTableEntry moamoa1_weakness_pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleUnitSetup entry_boss_moamoa_tentacle_a;
static BattleUnitSetup entry_boss_moamoa_tentacle_b;
static BattleUnitSetup entry_boss_moamoa_tentacle_c;
static BattleUnitSetup entry_boss_moamoa_mouth;
static BattleWeapon weapon_moamoa_tsunami_attack;
static EvtWord moamoa_zoom[34];
static EvtWord mario_party_zoom[34];
static EvtWord init_event[159];
static EvtWord damage_event[10];
static const char* MOAMOA_TAIJOU[4];
static EvtWord dead_event[349];
static const char* moamoa2_change_tbl[7];
static EvtWord moamoa_tentacl_on[208];
static EvtWord moamoa_mouth_on[132];
static EvtWord phase_event[159];
static EvtWord attack_event[19];
static EvtWord wait_event[6];
static ExtTexInfo ext_moamoa_data[89];
static EvtWord evt_moamoa[151];
static EvtWord MOAMOA_SET_BODYSCALE[4];

/**
 * Script helper for the Smorg boss's tentacle and mouth units:
 * args[0] is a battle unit id, and args[1] receives 0 if the unit's x position is -120 or more and 1
 * if it is further left, i.e. whether it stands in the front (zenei) or rear (kouei) part of the stage.
 */
USER_FUNC(boss_moamoa_check_zenei_kouei) {
    BattleWork* btlwp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    f32 x, y, z;

    id = BattleTransID(event, id);
    BtlUnit_GetPos(BattleGetUnitPtr(btlwp, id), &x, &y, &z);
    if (x >= -120.0f) {
        evtSetValue(event, out, 0);
    } else {
        evtSetValue(event, out, 1);
    }
    return 2;
}

/**
 * Script helper: forgets the Smorg main body's unit id in the swarm work area (wp->unitId = -1),
 * so ext_moamoa_main stops placing the swarm on the body and runs its swirl branch once the boss is dead.
 */
static USER_FUNC(evt_moamoa_mainUnitIdClear) {
    moamoa_mainUnitIdClear();
    return 2;
}

/** Returns a random float between min and max using rand(). */
static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;
#if VERSION >= VERSION_DEMO_EU
    return r * (max - min) + min;
}

/**
 * EU only: seeds the boss swarm's own random generator (two-word state),
 * making the swarm placement reproducible for a given seed.
 */
void moamoa_srand(u32 seed) {
    moamoa_rand_prev = seed;
    moamoa_rand_cur = seed + 0x37E35F7D;
}

/** EU only: returns a float between min and max using the boss swarm's own generator seeded by moamoa_srand. */
f32 moamoa_randf(f32 min, f32 max) {
    s32 cur = moamoa_rand_cur;
    s32 next = moamoa_rand_prev + cur + 0x195B31B7;
    f32 r;

    moamoa_rand_prev = cur;
    moamoa_rand_cur = next;
    r = (next & 0x7FFF) / 32767.0f;
#endif

    return r * (max - min) + min;
}

/**
 * Clears the Smorg main body's unit id in the swarm work area (wp->unitId = -1).
 * Called from evt_moamoa_mainUnitIdClear.
 */
static void moamoa_mainUnitIdClear(void) {
    wp->unitId = -1;
}

/**
 * Unit kind of the Smorg (HP 50, level 78): its size, offsets, damage sounds, status vulnerability,
 * parts and init event.
 */
BattleUnitKind unit_boss_moamoa = {
    UNIT_SMORG, // unitType
    "btl_un_moamoa", // name
    50, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    78, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    160, // width
    80, // height
#if VERSION <= VERSION_DEMO_JP
    0,
    24,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    280.0f,
    40.0f,
    0.0f,
    80.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    250.0f,
    0.0f,
    80.0f,
    0.0f,
    0.0f,
#else
    {0, 24}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {280.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {80.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    15.0f, // vivianOffsetX
#endif
    10.0f, // vivianOffsetZ
#if VERSION <= VERSION_DEMO_JP
    80.0f,
    20.0f,
    0.0f,
    70.0f,
    0.0f,
#else
    {80.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 80.0f, 20.0f}, // kissHitOffset
    {0.0f, 70.0f, 0.0f}, // cutBaseOffset
#endif
    160.0f, // cutWidth
    80.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MOA_G_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Smorg: 2 damage points off every hit. */
BattleUnitDefense moamoa_defence = {{2, 2, 2, 2, 2}};

/**
 * Zero base-defence table for the exposed main body's part when its attacking parts are gone.
 */
BattleUnitDefense moamoa_defence_no_parts = {{0, 0, 0, 0, 0}};

/** Defence of the Smorg: 1 damage points off every hit. */
BattleUnitDefense moamoa2_defence = {{1, 1, 1, 1, 1}};

/** Element reactions of the Smorg: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Smorg (percent multiplier on each status's chance; all 0):
 * immune to sleep, stop, dizzy, poison, confusion, electric shock, burn, freeze, growing huge, shrinking,
 * attack up, attack down, defence up, defence down, allergy, fright, gale force, speed up, slow, dodgy,
 * invisibility and instant kill.
 */
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    0, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

PoseTableEntry moamoa1_pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1_1"},
    {POSE_TYPE_DAMAGE, "D_1_1"},
    {POSE_TYPE_ACTION, "S_1_1"},
};

PoseTableEntry moamoa1_weakness_pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1_3"},
    {POSE_TYPE_DAMAGE, "D_1_3"},
    {POSE_TYPE_ACTION, "S_1_3"},
};

/** Data table of the Smorg: event handlers for being defeated. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/** Body part of the Smorg (model c_g_moamoa_b): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {
        1, "btl_un_moamoa", "c_g_moamoa_b",
        0.0f, 0.0f, 0.0f,
        -2.0f, 53.0f, 20.0f,
        -4.0f, 63.0f, 20.0f,
        20, 30, 255,
        &moamoa_defence, &defence_attr,
        kMainPart | kImmuneToDamageAndStatus, 0, moamoa1_pose_table,
    },
};

/**
 * Spawn setup for Smorg's tentacle A: starts off-screen on the enemy side; the main body moves it into place.
 */
static BattleUnitSetup entry_boss_moamoa_tentacle_a = {
    &unit_boss_moamoa_tentacle_a, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for Smorg's tentacle B: starts off-screen on the enemy side; the main body moves it into place.
 */
static BattleUnitSetup entry_boss_moamoa_tentacle_b = {
    &unit_boss_moamoa_tentacle_b, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for Smorg's tentacle C: starts off-screen on the enemy side; the main body moves it into place.
 */
static BattleUnitSetup entry_boss_moamoa_tentacle_c = {
    &unit_boss_moamoa_tentacle_c, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Spawn setup for Smorg's mouth: starts off-screen on the enemy side;
 * the main body moves it onto its own position.
 */
static BattleUnitSetup entry_boss_moamoa_mouth = {
    &unit_boss_moamoa_mouth, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Weapon of the Smorg's tsunami: 0 damage. */
static BattleWeapon weapon_moamoa_tsunami_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Battle camera script: switches to camera mode 3
 * and moves the camera to a fixed close view of the Smorg (about (83, 144, 512) looking at (83, 109, 0)).
 * LW(0) selects the speed: 1 waits 600 ms after starting the move, 2 jumps there in a single frame,
 * anything else takes 30 frames.
 */
static EVT_BEGIN(moamoa_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
    ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, 83, 144, 512, 83, 109, 0, LW(1), 11)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

/**
 * Battle camera script: zooms in on Mario and the partner (camera mode 3,
 * to (-120, 58, 257) looking at (-120, 40, 0)).
 * LW(0) works as in moamoa_zoom: 1 waits 600 ms, 2 jumps in one frame, otherwise 30 frames.
 */
static EVT_BEGIN(mario_party_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
    ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, -120, 58, 257, -120, 40, 0, LW(1), 11)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

/**
 * Init event of the Smorg main body: registers its wait, phase, attack, damage and confusion events,
 * spawns the three tentacle units (c, b and a, stored in unit work 3 to 5) with the mouth slot (6) empty,
 * starts the swarm (evt_moamoa) and the body-scale event, and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_REGROW_TURNS, 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH_FORM, 0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_c), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_b), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 4)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_a), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    SET_VALUE(LW(5), 6)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), -1)
    CALL_CHILD(evt_moamoa)
    SPAWN_EVENT(MOAMOA_SET_BODYSCALE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage event of the Smorg main body: runs the default damage handling (btldefaultevt_Damage) with the body as
 * target.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Animation names (S_1_2, then L_1_4; NULL separates steps) Smorg's body plays, in order,
 * during its defeat sequence.
 */
static const char* MOAMOA_TAIJOU[] = {
    "S_1_2",
    NULL,
    "L_1_4",
    NULL,
};

static EVT_BEGIN(dead_event)
    // Once the last attack has finished, Smorg's remaining tentacles (or, in the mouth form, its mouth) collapse
    // and vanish one by one.
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH_FORM, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        SET_VALUE(LW(0), 5)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1A"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        END_IF
        SET_VALUE(LW(0), 4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1B"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        END_IF
        SET_VALUE(LW(0), 3)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1C"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        END_IF
    ELSE
        SET_VALUE(LW(0), 6)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("L_2_6"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        END_IF
    END_IF

    // Smorg's body sags and crumples through its defeat animations.
    SET_INT_READ_ADDR(PTR(&MOAMOA_TAIJOU))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    END_LOOP

    // A pause, a lump sound, then the body keeps collapsing.
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_LUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    END_LOOP

    // The swarm swirls up, smoke puffs out and the party is paid its coins, experience and star points.
    CALL(moamoa_guruguru)
    WAIT_MS(500)
    SET_VALUE(LW(10), UNIT_ID_SELF)
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

    // The final explosion scatters the swarm, and the boss unit is removed.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_EXPLOSION1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(moamoa_bomb)
    CALL(evt_moamoa_mainUnitIdClear)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

/**
 * Animation names (L_2_1, L_2_3, L_2_4, L_2_5, then S_2) Smorg's mouth plays in order while it changes into its second form.
 */
static const char* moamoa2_change_tbl[] = {
    "L_2_1",
    "L_2_3",
    NULL,
    "L_2_4",
    "L_2_5",
    "S_2",
    NULL,
};

/**
 * Phase event of the Smorg boss (called from its phase_event): grows the three tentacles.
 * Plays the arm restore sound and the V_2 pose,
 * spawns the tentacle units (entry_boss_moamoa_tentacle_a, b and c),
 * links them to the main body through unit work 3 to 5, attaches the swarm Moamoa chains to them (moamoa1_on),
 * switches the defence table to moamoa_defence and plays each tentacle's B_1 idle pose.
 */
static EVT_BEGIN(moamoa_tentacl_on)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_RESTORE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("V_2"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_c), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    SET_VALUE(LW(6), LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_b), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 4)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    SET_VALUE(LW(7), LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_a), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    CALL(moamoa1_on)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&moamoa_defence))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kImmuneToDamageAndStatus)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(moamoa1_pose_table))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, LW(7), 1, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, LW(6), 1, kHidden)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, LW(7), 1, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, LW(6), 1, kHidden)
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("B_1A"))
    CALL(btlevtcmd_AnimeChangePose, LW(7), 1, STRING("B_1B"))
    CALL(btlevtcmd_AnimeChangePose, LW(6), 1, STRING("B_1C"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(6), 1)
    RETURN
EVT_END()

/**
 * Phase event of the Smorg boss (called from its phase_event): grows the mouth.
 * Plays the restore sound, the V_2 pose, spawns the mouth unit (entry_boss_moamoa_mouth) linked through unit
 * work 6, attaches the swarm Moamoa chains to it (moamoa2_on),
 * and sets the main body's moamoa2_defence and moamoa1_pose_table before playing the mouth's transformation sequence.
 */
static EVT_BEGIN(moamoa_mouth_on)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_RESTORE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("V_2"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_mouth), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 6)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), MOAMOA_LIMB_WORK_BODY, LW(4))
    CALL(moamoa2_on)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&moamoa2_defence))
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(moamoa1_pose_table))
    SET_INT_READ_ADDR(PTR(&moamoa2_change_tbl))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kHidden)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, kHidden)
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    END_LOOP
    WAIT_MS(1000)
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Phase event of the Smorg main body: while it can act, after the player's turn with the mouth
 * and all tentacles gone, counts down its regrowth delay. When the delay expires it regrows
 * either the three tentacles (moamoa_tentacl_on) or the mouth (moamoa_mouth_on).
 * At that point, first-form Smorg at half HP or less switches to the mouth form.
 */
// Count regrowth turns only while all limbs are absent; at regrowth, half HP selects mouth form.
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_TENTACLE_A, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_TENTACLE_B, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_TENTACLE_C, LW(0))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_REGROW_TURNS, LW(1))
                    IF_INT_NOT_EQUAL(LW(1), 0)
                        SUBTRACT(LW(1), 1)
                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_REGROW_TURNS, LW(1))
                        IF_INT_EQUAL(LW(1), 0)
                            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_REGROW_TURNS, 2)
                            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH_FORM, LW(0))
                            SWITCH(LW(0))
                                CASE_EQUAL(0)
                                    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
                                    MULTIPLY(LW(0), 50)
                                    DIVIDE(LW(0), 100)
                                    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
                                    IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
                                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH_FORM, 1)
                                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1_2"))
                                        WAIT_MS(1000)
                                        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1_1"))
                                    END_IF
                            END_SWITCH
                            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_UNIT_WORK_MOUTH_FORM, LW(1))
                            IF_INT_NOT_EQUAL(LW(1), 1)
                                CALL_CHILD(moamoa_tentacl_on)
                            ELSE
                                CALL_CHILD(moamoa_mouth_on)
                            END_IF
                            CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
                            CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
    END_IF
    LABEL(0)
    RETURN
EVT_END()

/**
 * Attack event of the Smorg main body: uses an item if the item-use check selects one; otherwise the body does
 * nothing (its tentacles and mouth attack as their own units).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle event of the Smorg main body: shows the unit's table pose on part 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/** Model, animation and starting-frame presets for the swarm sprites: 88 presets plus the terminating empty entry. */
static ExtTexInfo ext_moamoa_data[] = {
    {"c_n_moa_a", "S_1", 0.0f},
    {"c_n_moa_a", "S_1", 2.0f},
    {"c_n_moa_a", "S_1", 4.0f},
    {"c_n_moa_a", "S_1", 6.0f},
    {"c_n_moa_a", "S_1", 8.0f},
    {"c_n_moa_a", "S_1", 10.0f},
    {"c_n_moa_a", "S_1", 12.0f},
    {"c_n_moa_a", "S_1", 14.0f},
    {"c_n_moa_a", "S_1", 16.0f},
    {"c_n_moa_a", "S_1", 18.0f},
    {"c_n_moa_a", "S_1", 20.0f},
    {"c_n_moa_a", "S_1", 22.0f},
    {"c_n_moa_a", "S_1", 24.0f},
    {"c_n_moa_a", "S_1", 26.0f},
    {"c_n_moa_a", "S_1", 28.0f},
    {"c_n_moa_a", "S_1", 30.0f},
    {"c_n_moa_a", "S_1", 32.0f},
    {"c_n_moa_a", "S_1", 34.0f},
    {"c_n_moa_a", "S_1", 36.0f},
    {"c_n_moa_a", "S_1", 38.0f},
    {"c_n_moa_a", "D_1", 0.0f},
    {"c_n_moa_a", "D_1", 2.0f},
    {"c_n_moa_a", "D_1", 4.0f},
    {"c_n_moa_a", "D_1", 6.0f},
    {"c_n_moa_a", "D_1", 8.0f},
    {"c_n_moa_a", "D_1", 10.0f},
    {"c_n_moa_a", "S_2", 0.0f},
    {"c_n_moa_a", "S_2", 2.0f},
    {"c_n_moa_a", "S_2", 4.0f},
    {"c_n_moa_a", "S_2", 6.0f},
    {"c_n_moa_a", "S_2", 8.0f},
    {"c_n_moa_a", "S_2", 10.0f},
    {"c_n_moa_a", "S_2", 12.0f},
    {"c_n_moa_a", "S_2", 14.0f},
    {"c_n_moa_a", "S_2", 16.0f},
    {"c_n_moa_a", "S_2", 18.0f},
    {"c_n_moa_a", "S_2", 20.0f},
    {"c_n_moa_a", "S_2", 22.0f},
    {"c_n_moa_a", "S_2", 24.0f},
    {"c_n_moa_a", "S_2", 26.0f},
    {"c_n_moa_a", "S_2", 28.0f},
    {"c_n_moa_a", "S_2", 30.0f},
    {"c_n_moa_a", "S_2", 32.0f},
    {"c_n_moa_a", "S_2", 34.0f},
    {"c_n_moa_a", "S_2", 36.0f},
    {"c_n_moa_a", "S_2", 38.0f},
    {"c_n_moa_b", "S_1", 0.0f},
    {"c_n_moa_b", "S_1", 2.0f},
    {"c_n_moa_b", "S_1", 4.0f},
    {"c_n_moa_b", "S_1", 6.0f},
    {"c_n_moa_b", "S_1", 8.0f},
    {"c_n_moa_b", "S_1", 10.0f},
    {"c_n_moa_b", "S_1", 12.0f},
    {"c_n_moa_b", "S_1", 14.0f},
    {"c_n_moa_b", "S_1", 16.0f},
    {"c_n_moa_b", "S_1", 18.0f},
    {"c_n_moa_b", "S_1", 20.0f},
    {"c_n_moa_b", "S_1", 22.0f},
    {"c_n_moa_b", "S_1", 24.0f},
    {"c_n_moa_b", "S_1", 26.0f},
    {"c_n_moa_b", "S_1", 28.0f},
    {"c_n_moa_b", "S_1", 30.0f},
    {"c_n_moa_b", "S_1", 32.0f},
    {"c_n_moa_b", "S_1", 34.0f},
    {"c_n_moa_b", "S_1", 36.0f},
    {"c_n_moa_b", "S_1", 38.0f},
    {"c_n_moa_b", "D_1", 0.0f},
    {"c_n_moa_b", "D_1", 2.0f},
    {"c_n_moa_b", "D_1", 4.0f},
    {"c_n_moa_b", "D_1", 6.0f},
    {"c_n_moa_b", "D_1", 8.0f},
    {"c_n_moa_b", "D_1", 10.0f},
    {"c_kinodebu", "G_1", 3.0f},
    {"c_kinodebu", "G_1", 11.0f},
    {"c_puku_c", "U_1", 1.0f},
    {"c_puku_c", "U_1", 6.0f},
    {"c_powan", "R_2", 3.0f},
    {"c_powan", "R_2", 6.0f},
    {"c_smoko", "G_1", 2.0f},
    {"c_smoko", "G_1", 6.0f},
    {"c_rose", "G_1", 4.0f},
    {"c_rose", "G_1", 8.0f},
    {"c_papab", "G_1", 1.0f},
    {"c_papab", "G_1", 5.0f},
    {"c_mamab", "G_1", 1.0f},
    {"c_mamab", "G_1", 5.0f},
    {"c_konari", "G_1", 1.0f},
    {"c_konari", "G_1", 5.0f},
    {0},
};

/**
 * Script helper used at the Smorg's death: collects the Moamoa that are above ground, finds their center,
 * and gives each a starting angle (22.4 degrees apart) so they can swirl (guruguru) around it.
 */
static USER_FUNC(moamoa_guruguru) {
    s32 i;
    ExtMoamoa* elem;
    ExtdrvPose* pose;
    f32 cnt;
    f32 ang;
    Vec v;
    Mtx m;

    BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    wp->center = (Vec){0.0f, 0.0f, 0.0f};
    pose = extGetPosePtr();
    elem = wp->elems;
    cnt = 0.0f;
    for (i = 0; i < wp->count; i++, pose++, elem++) {
        if (*elem->owner != -1) {
            v.x = (*pose->mtx)[0][3];
            v.y = (*pose->mtx)[1][3];
            v.z = (*pose->mtx)[2][3];
            if (v.y >= 0.0f) {
                elem->flags |= 1;
                cnt += 1.0f;
                PSVECAdd(&v, &wp->center, &wp->center);
            }
        }
    }
    PSVECScale(&wp->center, &wp->center, 1.0f / cnt);
    wp->angle = 0.0f;
    wp->radius = 2.0f;
    pose = extGetPosePtr();
    elem = wp->elems;
    ang = 0.0f;
    for (i = 0; i < wp->count; i++, pose++, elem++) {
        if (elem->flags & 1) {
            elem->vel.x = (*pose->mtx)[0][3];
            elem->vel.y = (*pose->mtx)[1][3];
            elem->vel.z = (*pose->mtx)[2][3];
            elem->pos2 = (Vec){0.0f, 1.0f, 0.0f};
            PSMTXRotRad(m, 'Z', 0.017453292f * ang);
            PSMTXMultVec(m, &elem->pos2, &elem->pos2);
            ang += 22.4375f;
            if (ang >= 360.0f) {
                ang -= 360.0f;
            }
        }
    }
    return 2;
}

/**
 * Script helper used at the Smorg's death: every other frame it launches up to ten more Moamoa outward
 * and upward from their body positions, makes them fall under gravity,
 * and finishes (returns done) once at most nine Moamoa remain above -200,
 * so the swarm scatters like an explosion.
 */
static USER_FUNC(moamoa_bomb) {
    ExtdrvPose* p2;
    ExtMoamoa* e2;
    s32 k;
    s32 i;
    ExtdrvPose* pose;
    ExtMoamoa* elem;
    s32 n;

    BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    wp->center = (Vec){0.0f, 0.0f, 0.0f};
    if (isFirstCall) {
        extGetPosePtr();
        elem = wp->elems;
        for (i = 0; i < wp->count; i++, elem++) {
            if (*elem->owner != -1) {
                elem->flags &= ~1;
            }
        }
    }
    if (!(gp->retraceCount & 1)) {
        p2 = extGetPosePtr();
        n = 0;
        e2 = wp->elems;
        for (k = 0; k < wp->count; k++, p2++, e2++) {
            if ((*p2->mtx)[1][3] >= 0.0f && !(e2->flags & 1)) {
                e2->flags |= 1;
                if (e2->flags & 1) {
                    e2->vel.x = randf(0.0f, 60.0f) - 30.0f;
                    e2->vel.y = randf(0.0f, 35.0f);
                    e2->vel.z = 60.0f + randf(0.0f, 30.0f);
                    n++;
                    e2->pos2.x = (*p2->mtx)[0][3];
                    e2->pos2.y = (*p2->mtx)[1][3];
                    e2->pos2.z = (*p2->mtx)[2][3];
                }
            }
            if (n >= 10) {
                break;
            }
        }
    }
    pose = extGetPosePtr();
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, pose++, elem++) {
        if (elem->flags & 1) {
            if (elem->pos2.y >= -500.0f) {
                elem->pos2.x += elem->vel.x;
                elem->pos2.y += elem->vel.y;
                elem->pos2.z += elem->vel.z;
                elem->vel.y -= 1.0f;
            }
            (*pose->mtx)[0][3] = elem->pos2.x;
            (*pose->mtx)[1][3] = elem->pos2.y;
            (*pose->mtx)[2][3] = elem->pos2.z;
        }
    }
    pose = extGetPosePtr();
    n = 0;
    for (i = 0; i < wp->count; i++, pose++) {
        if ((*pose->mtx)[1][3] >= -200.0f) {
            n++;
        }
    }
    if (n <= 9) {
        wp->center = (Vec){0.0f, -1000.0f, 0.0f};
        return 2;
    }
    return 0;
}

/**
 * Script helper: attaches the swarm Moamoa to the Smorg's three tentacle parts.
 * Looks up the tentacle poses (unit work 3 to 5), hides their joints,
 * and assigns each Moamoa's chain to the tentacle it belongs to (chains 0 to 4, 5 to 9 and 10 to 14)
 * or to the main body, recording which unit's work slot owns it.
 */
static USER_FUNC(moamoa1_on) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    s32 i;
    BattleUnitPart* part;
    ExtMoamoa* elem;
    AnimationPose* ap;
    s32 j;
    Vec* p;

    for (i = 1; i < 4; i++) {
        switch (i) {
            case 1:
                part = BattleGetUnitPartsPtr(unit->work[MOAMOA_UNIT_WORK_TENTACLE_C], 1);
                break;
            case 2:
                part = BattleGetUnitPartsPtr(unit->work[MOAMOA_UNIT_WORK_TENTACLE_B], 1);
                break;
            case 3:
                part = BattleGetUnitPartsPtr(unit->work[MOAMOA_UNIT_WORK_TENTACLE_A], 1);
                break;
        }
        wp->poseId[i] = part->poseId;
        animPoseWorldPositionEvalOn(wp->poseId[i]);
        ap = animPoseGetAnimPosePtr(wp->poseId[i]);
        p = (Vec*)ap->worldPositions->address;
        for (j = 0; j < ap->numWorldPositions; j++, p++) {
            *p = (Vec){0.0f, -1000.0f, 0.0f};
        }
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        if (elem->chain < 5) {
            elem->poseId = wp->poseId[3];
            elem->owner = &unit->work[MOAMOA_UNIT_WORK_TENTACLE_A];
        } else if (elem->chain < 10) {
            elem->poseId = wp->poseId[2];
            elem->owner = &unit->work[MOAMOA_UNIT_WORK_TENTACLE_B];
        } else if (elem->chain < 15) {
            elem->poseId = wp->poseId[1];
            elem->owner = &unit->work[MOAMOA_UNIT_WORK_TENTACLE_C];
        } else if (elem->chain < 30) {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        } else {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        }
    }
    return 2;
}

/**
 * Script helper: attaches the swarm Moamoa to the Smorg's mouth.
 * Looks up the mouth pose (unit work 6), hides its joints,
 * and assigns the Moamoa of chains 15 to 29 to the mouth and all others to the main body.
 */
static USER_FUNC(moamoa2_on) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    ExtMoamoa* elem;
    AnimationPose* ap;
    Vec* p;
    s32 i;

    wp->poseId[4] = BattleGetUnitPartsPtr(unit->work[MOAMOA_UNIT_WORK_MOUTH], 1)->poseId;
    animPoseWorldPositionEvalOn(wp->poseId[4]);
    ap = animPoseGetAnimPosePtr(wp->poseId[4]);
    p = (Vec*)ap->worldPositions->address;
    for (i = 0; i < ap->numWorldPositions; i++, p++) {
        *p = (Vec){0.0f, -1000.0f, 0.0f};
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        if (elem->chain < 5) {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        } else if (elem->chain < 10) {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        } else if (elem->chain < 15) {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        } else if (elem->chain < 30) {
            elem->poseId = wp->poseId[4];
            elem->owner = &unit->work[MOAMOA_UNIT_WORK_MOUTH];
        } else {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        }
    }
    return 2;
}

/**
 * Script helper: creates the boss's swarm.
 * Allocates 750 Moamoa with small random offsets (EU: from its own seeded generator), reads the main body pose,
 * sets up the bone chains and the animation groups from ext_moamoa_data,
 * and attaches the chains to the tentacles (moamoa1_on); types 1 to 9 select alternate Moamoa and passenger models.
 */
static USER_FUNC(moamoa_on) {
    s32 i;
    ExtMoamoa* elem;
    ExtTexInfo* d;
    s32 j;
    AnimationPose* ap;
    Vec* p;
    s32 start;
#if VERSION >= VERSION_DEMO_EU
    u32 seeds[4] = {0x3056, 0xF36, 0xF2A93, 0x17F57};
#endif
    Vec v;
    char anim[32];
    char pose[32];

    wp = BTL_ALLOC(sizeof(ExtMoamoaWork));
    memset(wp, 0, sizeof(ExtMoamoaWork));
    wp->count = 750;
    wp->elems = BTL_ALLOC(wp->count * sizeof(ExtMoamoa));
    memset(wp->elems, 0, wp->count * sizeof(ExtMoamoa));
    elem = wp->elems;
#if VERSION >= VERSION_DEMO_EU
    moamoa_srand(seeds[rand() & 3]);
#endif
    for (i = 0; i < wp->count; i++, elem++) {
        elem->flags = 0;
        v = (Vec){0.0f, 0.0f, 0.0f};
#if VERSION >= VERSION_DEMO_EU
        v.x = moamoa_randf(-10.0f, 10.0f);
        v.y = moamoa_randf(-10.0f, 10.0f);
        v.z = moamoa_randf(-10.0f, 10.0f);
#else
        v.x = randf(-10.0f, 10.0f);
        v.y = randf(-10.0f, 10.0f);
        v.z = randf(-10.0f, 10.0f);
#endif
        elem->pos = v;
    }
    wp->unitId = BattleTransID(event, UNIT_ID_SELF);
    BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    wp->poseId[0] = BattleGetUnitPartsPtr(wp->unitId, 1)->poseId;
    animPoseWorldPositionEvalOn(wp->poseId[0]);
    ap = animPoseGetAnimPosePtr(wp->poseId[0]);
    p = (Vec*)ap->worldPositions->address;
    for (i = 0; i < ap->numWorldPositions; i++, p++) {
        *p = (Vec){0.0f, -1000.0f, 0.0f};
    }
    for (i = 0; i < g_moamoa_chain_max; i++) {
        wp->chainIdx[i][0] = animPoseGetGroupIdx(wp->poseId[0], g_moamoa_chain_tbl[i].parent);
        wp->chainIdx[i][1] = animPoseGetGroupIdx(wp->poseId[0], g_moamoa_chain_tbl[i].child);
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        elem->chain = i % g_moamoa_chain_max;
        elem->order = wp->chainNum[elem->chain]++;
    }
    moamoa1_on(event, isFirstCall);
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        elem->t = (f32)elem->order / (f32)wp->chainNum[elem->chain];
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        if (elem->chain == 0 || elem->chain == 5 || elem->chain == 10 || elem->chain == 15 || elem->chain == 46) {
            elem->type = 1;
        }
        if (elem->order == 0) {
            if (elem->chain == 3) {
                elem->type = 2;
            }
            if (elem->chain == 4) {
                elem->type = 3;
            }
            if (elem->chain == 7) {
                elem->type = 4;
            }
            if (elem->chain == 8) {
                elem->type = 5;
            }
            if (elem->chain == 9) {
                elem->type = 6;
            }
            if (elem->chain == 12) {
                elem->type = 7;
            }
            if (elem->chain == 13) {
                elem->type = 8;
            }
            if (elem->chain == 14) {
                elem->type = 9;
            }
        }
    }
    d = ext_moamoa_data;
    j = 0;
    strcpy(anim, "");
    strcpy(pose, "");
    wp->numGroups = -1;
    do {
        if (strcmp(d->group, anim) != 0) {
            strcpy(anim, d->group);
            strcpy(pose, "");
        }
        if (strcmp(d->anim, pose) != 0) {
            strcpy(pose, d->anim);
            start = j;
            wp->numGroups++;
        }
        strcpy(wp->groups[wp->numGroups].anim, anim);
        strcpy(wp->groups[wp->numGroups].pose, pose);
        wp->groups[wp->numGroups].start = start;
        wp->groups[wp->numGroups].num = j - start + 1;

        // BUG: the preceding particle loop left elem one past the allocated array.
        if (elem->type >= 2 && elem->type != 10) {
            wp->groups[wp->numGroups].speed = 12.0f;
        } else {
            wp->groups[wp->numGroups].speed = d->frame;
        }
        j++;
    } while ((++d)->group != NULL);
    wp->numGroups++;
    for (i = 0; i < wp->numGroups; i++) {
    }
    return 2;
}

/**
 * Sets up the boss's swarm: hides the passenger model NPCs, runs moamoa_on,
 * and adds the 750-sprite swarm effect with ext_moamoa_data, ext_moamoa_init and ext_moamoa_main.
 */
static EVT_BEGIN(evt_moamoa)
    CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a")) // hidden Smorg model A for the battle swarm sprites
    CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b")) // hidden Smorg model B for the battle swarm sprites
    CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("kinopio"), STRING("c_kinodebu")) // Heff T., passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("cook"), STRING("c_puku_c")) // Chef Shimi, passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("powan"), STRING("c_powan")) // Pennington, passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("sarary"), STRING("c_smoko")) // the Ratooey businessman (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("rose"), STRING("c_rose")) // Toodles, passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("papa"), STRING("c_papab")) // Goldbob, passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("mama"), STRING("c_mamab")) // Sylvia, passenger (hidden during the Smorg battle)
    CALL(evt_npc_entry, STRING("konari"), STRING("c_konari")) // Bub, passenger (hidden during the Smorg battle)
    CALL(evt_npc_set_position, STRING("kinopio"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("cook"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("powan"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("sarary"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("rose"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("papa"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("mama"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("konari"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("kinopio"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("cook"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("powan"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("sarary"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("rose"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("papa"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("mama"), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("konari"), NPC_FLAG_HIDE)
    CALL(moamoa_on)
    CALL(evt_ext_entry, 750, PTR(ext_moamoa_data), PTR(ext_moamoa_init), PTR(ext_moamoa_main), 0)
    RETURN
EVT_END()

static void moamoa_pose(ExtMoamoa* elem) {
    static const char* group_tbl[][2] = {
        {"c_n_moa_a", "S_1"},
        {"c_n_moa_b", "S_1"},
        {"c_kinodebu", "G_1"},
        {"c_puku_c", "U_1"},
        {"c_powan", "R_2"},
        {"c_smoko", "G_1"},
        {"c_rose", "G_1"},
        {"c_papab", "G_1"},
        {"c_mamab", "G_1"},
        {"c_konari", "G_1"},
        {"c_n_moa_a", "S_2"},
    };
    ExtMoamoaGroup* grp;
    s32 i;
    const char* pose;
    const char* anim;

    grp = wp->groups;
    anim = group_tbl[elem->type][0];
    pose = group_tbl[elem->type][1];
    for (i = 0; i < wp->numGroups; i++, grp++) {
        if (strcmp(grp->anim, anim) == 0 && strcmp(grp->pose, pose) == 0) {
            break;
        }
    }
    if (i < wp->numGroups && elem->group != i) {
        elem->group = i;
        elem->time = gp->renderTime;
    }
}

/**
 * Init callback of the boss swarm effect: sets scale to 1 and gives every sprite an identity matrix
 * and its starting pose.
 */
static void ext_moamoa_init(void) {
    s32 i;
    ExtdrvPose* pose;
    ExtMoamoa* elem;

    pose = extGetPosePtr();
    elem = wp->elems;
    wp->targetScale = 1.0f;
    wp->scale = 1.0f;
    for (i = 0; i < wp->count; i++, pose++, elem++) {
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_NO_SHADOW);
        PSMTXIdentity(*pose->mtx);
        moamoa_pose(elem);
    }
}

/**
 * Event run beside the main body's script: calls moamoa_set_bodyScale to resize the body Moamoa as tentacles
 * and mouth are lost.
 */
static EVT_BEGIN(MOAMOA_SET_BODYSCALE)
    CALL(moamoa_set_bodyScale)
    RETURN
EVT_END()

static void ext_moamoa_main(void) {
    ExtMoamoa* elem;
    ExtMoamoa* e4;
    ExtMoamoa* e2;
    s32 k4;
    s32 k3;
    s32 k;
    s32 i;
    ExtdrvPose* pose;
    s32 j;
    BattleUnit* unit;
    AnimationPose* ap;
    Vec* a;
    Vec* b;
    ExtMoamoaGroup* grp;
    s32 n;
    f32 t;
    f32 s;
    Vec joint[5];
    Vec ofs[5];
    Mtx m;
    Vec w;
    Vec v;
    Vec r;

    pose = extGetPosePtr();
    for (i = 0; i < wp->count; i++, pose++) {
    }
    wp->scale += 0.1f * (wp->targetScale - wp->scale);

    pose = extGetPosePtr();
    e2 = wp->elems;
    for (k = 0; k < wp->count; k++, e2++, pose++) {
        grp = &wp->groups[e2->group];
        t = gp->framerate * (f32)OSTicksToMilliseconds((u64)(gp->renderTime - e2->time)) / 1000.0f / grp->speed;
        if (t >= 1.0f) {
            e2->time = gp->renderTime;
            t = 0.0f;
        }
        n = grp->num * t;
        if (n >= grp->num) {
            n = grp->num - 1;
        }
        pose->texId = grp->start + n;
    }

    if (wp->unitId != -1) {
        unit = BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
        for (j = 0; j < 5; j++) {
            if (j == 0 || unit->work[j + 2] != -1) {
                ap = animPoseGetAnimPosePtr(wp->poseId[j]);
                joint[j] = ((Vec*)ap->worldPositions->address)[animPoseGetGroupIdx(wp->poseId[j], "joint_core")];
                PSVECSubtract(&joint[0], &joint[j], &ofs[j]);
            }
        }
        pose = extGetPosePtr();
        e4 = wp->elems;
        for (k4 = 0; k4 < wp->count; k4++, pose++, e4++) {
            if (*e4->owner == -1) {
                (*pose->mtx)[0][3] = 0.0f;
                (*pose->mtx)[1][3] = -1000.0f;
                (*pose->mtx)[2][3] = 0.0f;
                continue;
            }
            ap = animPoseGetAnimPosePtr(e4->poseId);
            a = &((Vec*)ap->worldPositions->address)[wp->chainIdx[e4->chain][0]];
            b = &((Vec*)ap->worldPositions->address)[wp->chainIdx[e4->chain][1]];
            t = e4->t;
            s = 1.0f - t;
            if (a->y == -1000.0f || b->y == -1000.0f) {
                v = (Vec){0.0f, -1000.0f, 0.0f};
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            } else {
                PSVECScale(a, &v, t);
                PSVECScale(b, &w, s);
                PSVECAdd(&v, &w, &v);
                PSVECAdd(&v, &e4->pos, &v);
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
            }
            if (e4->owner != &wp->unitId) {
                for (j = 1; j < 5; j++) {
                    if (&unit->work[j + 2] == e4->owner) {
                        PSVECAdd(&v, &ofs[j], &v);
                        break;
                    }
                }
            }
            if (e4->chain >= 46) {
                (*pose->mtx)[0][0] = (*pose->mtx)[1][1] = wp->scale;
            }
            (*pose->mtx)[0][3] += (v.x - (*pose->mtx)[0][3]) / 2;
            (*pose->mtx)[1][3] += (v.y - (*pose->mtx)[1][3]) / 2;
            (*pose->mtx)[2][3] += (v.z - (*pose->mtx)[2][3]) / 2;
        }
    } else {
        wp->angle += 8.571428f;
        wp->radius -= 0.0044444446f;
        if (wp->radius < 0.8f) {
            wp->radius = 0.8f;
        }
        if (wp->angle >= 360.0f) {
            wp->angle -= 360.0f;
        }
        pose = extGetPosePtr();
        elem = wp->elems;
        for (k3 = 0; k3 < wp->count; k3++, pose++, elem++) {
            if (elem->flags & 1) {
                PSVECSubtract(&elem->vel, &wp->center, &r);
                PSMTXRotAxisRad(m, &elem->pos2, 0.017453292f * wp->angle);
                PSMTXMultVec(m, &r, &r);
                PSVECScale(&r, &r, wp->radius);
                PSVECAdd(&r, &wp->center, &r);
                (*pose->mtx)[0][3] += (r.x - (*pose->mtx)[0][3]) / 2;
                (*pose->mtx)[1][3] += (r.y - (*pose->mtx)[1][3]) / 2;
                (*pose->mtx)[2][3] += (r.z - (*pose->mtx)[2][3]) / 2;
            }
        }
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, moamoa_draw, NULL);
}

/** Draw callback of the boss swarm: loads the ext driver state and draws all sprites. */
static void moamoa_draw(CameraId cameraId, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/**
 * Script helper: sets the target scale of the body's own Moamoa sprites (chains 46 and up),
 * which ext_moamoa_main eases toward.
 * The scale is 0.333 * (n - 3) + 1, where n counts missing parts (each missing tentacle counts 1,
 * a missing mouth counts 3). Removes the effect if the main body no longer exists.
 */
// Missing mouth contributes three, each missing tentacle one; the target scale follows their sum.
static USER_FUNC(moamoa_set_bodyScale) {
    BattleUnit* unit;
    s32 n;

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, UNIT_ID_SELF));
    n = 0;
    if (unit == NULL) {
        extReset();
        return 2;
    }
    if (unit->work[MOAMOA_UNIT_WORK_MOUTH] == -1) {
        n += 3;
    }
    if (unit->work[MOAMOA_UNIT_WORK_TENTACLE_A] == -1) {
        n++;
    }
    if (unit->work[MOAMOA_UNIT_WORK_TENTACLE_B] == -1) {
        n++;
    }
    if (unit->work[MOAMOA_UNIT_WORK_TENTACLE_C] == -1) {
        n++;
    }
    wp->targetScale = 0.333f * (n - 3) + 1.0f;
    return 0;
}
