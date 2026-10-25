#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "battle/battle_camera.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
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

typedef struct MoamoaGroup { char anim[16]; char pose[8]; f32 speed; s32 start; s32 num; } MoamoaGroup;
typedef struct Moamoa { u16 flags; u8 field_0x2[2]; Vec pos; s16 group; u8 field_0x12[6]; OSTime time; s16 chain; s16 order; f32 t; s32 poseId; s32 type; EvtWord* owner; Vec vel; Vec pos2; u8 field_0x4c[4]; } Moamoa;
typedef struct MoamoaWork { s32 field_0x0; s32 count; Moamoa* elems; s32 numGroups; MoamoaGroup groups[32]; s16 chainNum[128]; s16 chainIdx[128][2]; s32 poseId[5]; s32 unitId; Vec center; f32 angle; f32 radius; f32 targetScale; f32 scale; } MoamoaWork;
typedef struct MoamoaExtData { const char* anim; const char* pose; f32 speed; } MoamoaExtData;
static MoamoaWork* wp;
void moamoa_mainUnitIdClear(void);
static f32 randf(f32 min, f32 max);
extern const char* g_moamoa_chain_tbl[][2];
USER_FUNC(moamoa1_on);
void moamoa_pose(Moamoa* elem);
void moamoa_draw(CameraId cameraId, void* param);

USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(moamoa_on);
USER_FUNC(moamoa_set_bodyScale);
static void ext_moamoa_main(void);
static void ext_moamoa_init(void);
USER_FUNC(moamoa2_on);
USER_FUNC(moamoa1_on);
USER_FUNC(moamoa_bomb);
USER_FUNC(moamoa_guruguru);
USER_FUNC(evt_moamoa_mainUnitIdClear);
USER_FUNC(boss_moamoa_check_zenei_kouei);
extern BattleUnitKind unit_boss_moamoa_mouth;
extern BattleUnitKind unit_boss_moamoa_tentacle_a;
extern BattleUnitKind unit_boss_moamoa_tentacle_b;
extern BattleUnitKind unit_boss_moamoa_tentacle_c;

BattleUnitKind unit_boss_moamoa;
BattleUnitDefense moamoa_defence;
s32 moamoa_defence_no_parts[2];
BattleUnitDefense moamoa2_defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
PoseTableEntry moamoa1_pose_table[4];
PoseTableEntry moamoa1_weakness_pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleUnitSetup entry_boss_moamoa_tentacle_a;
BattleUnitSetup entry_boss_moamoa_tentacle_b;
BattleUnitSetup entry_boss_moamoa_tentacle_c;
BattleUnitSetup entry_boss_moamoa_mouth;
BattleWeapon weapon_moamoa_tsunami_attack;
EvtWord moamoa_zoom[34];
static EvtWord mario_party_zoom[34];
static EvtWord init_event[159];
static EvtWord damage_event[10];
const char* MOAMOA_TAIJOU[4];
static EvtWord dead_event[349];
const char* moamoa2_change_tbl[7];
EvtWord moamoa_tentacl_on[208];
EvtWord moamoa_mouth_on[132];
static EvtWord phase_event[159];
static EvtWord attack_event[19];
static EvtWord wait_event[6];
static ExtTexInfo ext_moamoa_data[89];
static EvtWord evt_moamoa[151];
EvtWord MOAMOA_SET_BODYSCALE[4];

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

USER_FUNC(evt_moamoa_mainUnitIdClear) {
    moamoa_mainUnitIdClear();
    return 2;
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

void moamoa_mainUnitIdClear(void) {
    wp->unitId = -1;
}

BattleUnitKind unit_boss_moamoa = {
    107,
    "btl_un_moamoa",
    50,
    0,
    1,
    1,
    78,
    0,
    0,
    100,
    0,
    50,
    6,
    160,
    80,
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
    15.0f,
    10.0f,
    80.0f,
    0.0f,
    0.0f,
    10.0f,
    80.0f,
    20.0f,
    0.0f,
    70.0f,
    0.0f,
    160.0f,
    80.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_MOA_G_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

BattleUnitDefense moamoa_defence = {{2, 2, 2, 2, 2}};

s32 moamoa_defence_no_parts[] = {
    0, 0,
};

BattleUnitDefense moamoa2_defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

PoseTableEntry moamoa1_pose_table[] = {
    {28, "S_1_1"},
    {31, "D_1_1"},
    {39, "D_1_1"},
    {69, "S_1_1"},
};

PoseTableEntry moamoa1_weakness_pose_table[] = {
    {28, "S_1_3"},
    {31, "D_1_3"},
    {39, "D_1_3"},
    {69, "S_1_3"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_moamoa", "c_g_moamoa_b", 0.0f, 0.0f, 0.0f, -2.0f, 53.0f, 20.0f, -4.0f, 63.0f, 20.0f, 20, 30, 255, &moamoa_defence, &defence_attr, 0x20000001, 0, moamoa1_pose_table},
};

BattleUnitSetup entry_boss_moamoa_tentacle_a = {
    &unit_boss_moamoa_tentacle_a, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_boss_moamoa_tentacle_b = {
    &unit_boss_moamoa_tentacle_b, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_boss_moamoa_tentacle_c = {
    &unit_boss_moamoa_tentacle_c, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_boss_moamoa_mouth = {
    &unit_boss_moamoa_mouth, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {0.0f, -1000.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleWeapon weapon_moamoa_tsunami_attack = {
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(moamoa_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
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

static EVT_BEGIN(mario_party_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_GetUnitId, -2, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_c), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 3)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_b), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 4)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_a), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 5)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    SET_VALUE(LW(5), 6)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), -1)
    CALL_CHILD(PTR(evt_moamoa))
    SPAWN_EVENT(PTR(MOAMOA_SET_BODYSCALE))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

const char* MOAMOA_TAIJOU[] = {
    "S_1_2",
    NULL,
    "L_1_4",
    NULL,
};

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        SET_VALUE(LW(0), 5)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1A"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        END_IF
        SET_VALUE(LW(0), 4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1B"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        END_IF
        SET_VALUE(LW(0), 3)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("R_1C"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        END_IF
    ELSE
        SET_VALUE(LW(0), 6)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("L_2_6"))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            CALL(btlevtcmd_KillUnit, LW(1), 1)
            CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        END_IF
    END_IF
    SET_INT_READ_ADDR(PTR(&MOAMOA_TAIJOU))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    END_LOOP
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_LUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, -2, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    END_LOOP
    CALL(moamoa_guruguru)
    WAIT_MS(500)
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_EXPLOSION1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(moamoa_bomb)
    CALL(evt_moamoa_mainUnitIdClear)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

const char* moamoa2_change_tbl[] = {
    "L_2_1",
    "L_2_3",
    NULL,
    "L_2_4",
    "L_2_5",
    "S_2",
    NULL,
};

EVT_BEGIN(moamoa_tentacl_on)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_RESTORE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("V_2"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_GetUnitId, -2, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_c), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 3)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    SET_VALUE(LW(6), LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_b), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 4)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    SET_VALUE(LW(7), LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_tentacle_a), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 5)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    CALL(moamoa1_on)
    CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&moamoa_defence))
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kImmuneToDamageAndStatus)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(moamoa1_pose_table))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, LW(7), 1, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, LW(6), 1, 16777216)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 16777216)
    CALL(btlevtcmd_OffPartsAttribute, LW(7), 1, 16777216)
    CALL(btlevtcmd_OffPartsAttribute, LW(6), 1, 16777216)
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("B_1A"))
    CALL(btlevtcmd_AnimeChangePose, LW(7), 1, STRING("B_1B"))
    CALL(btlevtcmd_AnimeChangePose, LW(6), 1, STRING("B_1C"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(6), 1)
    RETURN
EVT_END()

EVT_BEGIN(moamoa_mouth_on)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MOA_G_ARM_RESTORE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("V_2"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_GetUnitId, -2, LW(4))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_boss_moamoa_mouth), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 6)
    CALL(btlevtcmd_SetUnitWork, -2, LW(5), LW(3))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
    CALL(moamoa2_on)
    CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&moamoa2_defence))
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(moamoa1_pose_table))
    SET_INT_READ_ADDR(PTR(&moamoa2_change_tbl))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, 16777216)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 16777216)
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

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 6, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(1))
                    IF_INT_NOT_EQUAL(LW(1), 0)
                        SUBTRACT(LW(1), 1)
                        CALL(btlevtcmd_SetUnitWork, -2, 1, LW(1))
                        IF_INT_EQUAL(LW(1), 0)
                            CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
                            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
                            SWITCH(LW(0))
                                CASE_EQUAL(0)
                                CALL(btlevtcmd_GetMaxHp, -2, LW(0))
                                MULTIPLY(LW(0), 50)
                                DIVIDE(LW(0), 100)
                                CALL(btlevtcmd_GetHp, -2, LW(1))
                                IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
                                    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
                                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1_2"))
                                    WAIT_MS(1000)
                                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1_1"))
                                END_IF
                            END_SWITCH
                            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
                            IF_INT_NOT_EQUAL(LW(1), 1)
                                CALL_CHILD(PTR(moamoa_tentacl_on))
                                ELSE
                                CALL_CHILD(PTR(moamoa_mouth_on))
                            END_IF
                            CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
                            CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 0, 0)
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
    END_IF
    LABEL(0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

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

USER_FUNC(moamoa_guruguru) {
    s32 i;
    Moamoa* elem;
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

USER_FUNC(moamoa_bomb) {
    ExtdrvPose* p2;
    Moamoa* e2;
    s32 k;
    s32 i;
    ExtdrvPose* pose;
    Moamoa* elem;
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

USER_FUNC(moamoa1_on) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    s32 i;
    BattleUnitPart* part;
    Moamoa* elem;
    AnimationPose* ap;
    s32 j;
    Vec* p;

    for (i = 1; i < 4; i++) {
        switch (i) {
            case 1:
                part = BattleGetUnitPartsPtr(unit->work[3], 1);
                break;
            case 2:
                part = BattleGetUnitPartsPtr(unit->work[4], 1);
                break;
            case 3:
                part = BattleGetUnitPartsPtr(unit->work[5], 1);
                break;
        }
        wp->poseId[i] = part->poseId;
        animPoseWorldPositionEvalOn(wp->poseId[i]);
        ap = animPoseGetAnimPosePtr(wp->poseId[i]);
        p = *ap->worldPositions;
        for (j = 0; j < ap->numWorldPositions; j++, p++) {
            *p = (Vec){0.0f, -1000.0f, 0.0f};
        }
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        if (elem->chain < 5) {
            elem->poseId = wp->poseId[3];
            elem->owner = &unit->work[5];
        } else if (elem->chain < 10) {
            elem->poseId = wp->poseId[2];
            elem->owner = &unit->work[4];
        } else if (elem->chain < 15) {
            elem->poseId = wp->poseId[1];
            elem->owner = &unit->work[3];
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

USER_FUNC(moamoa2_on) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    Moamoa* elem;
    AnimationPose* ap;
    Vec* p;
    s32 i;

    wp->poseId[4] = BattleGetUnitPartsPtr(unit->work[6], 1)->poseId;
    animPoseWorldPositionEvalOn(wp->poseId[4]);
    ap = animPoseGetAnimPosePtr(wp->poseId[4]);
    p = *ap->worldPositions;
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
            elem->owner = &unit->work[6];
        } else {
            elem->poseId = wp->poseId[0];
            elem->owner = &wp->unitId;
        }
    }
    return 2;
}

USER_FUNC(moamoa_on) {
    s32 i;
    s32 j;
    Moamoa* elem;
    MoamoaExtData* d;
    AnimationPose* ap;
    Vec* p;
    s32 start;
    Vec v;
    char anim[32];
    char pose[32];

    wp = BTL_ALLOC(sizeof(MoamoaWork));
    memset(wp, 0, sizeof(MoamoaWork));
    wp->count = 750;
    wp->elems = BTL_ALLOC(wp->count * sizeof(Moamoa));
    memset(wp->elems, 0, wp->count * sizeof(Moamoa));
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        elem->flags = 0;
        v = (Vec){0.0f, 0.0f, 0.0f};
        v.x = randf(-10.0f, 10.0f);
        v.y = randf(-10.0f, 10.0f);
        v.z = randf(-10.0f, 10.0f);
        elem->pos = v;
    }
    wp->unitId = BattleTransID(event, -2);
    BattleGetUnitPtr(_battleWorkPointer, wp->unitId);
    wp->poseId[0] = BattleGetUnitPartsPtr(wp->unitId, 1)->poseId;
    animPoseWorldPositionEvalOn(wp->poseId[0]);
    ap = animPoseGetAnimPosePtr(wp->poseId[0]);
    p = *ap->worldPositions;
    for (i = 0; i < ap->numWorldPositions; i++, p++) {
        *p = (Vec){0.0f, -1000.0f, 0.0f};
    }
    for (i = 0; i < g_moamoa_chain_num; i++) {
        wp->chainIdx[i][0] = animPoseGetGroupIdx(wp->poseId[0], g_moamoa_chain_tbl[i][0]);
        wp->chainIdx[i][1] = animPoseGetGroupIdx(wp->poseId[0], g_moamoa_chain_tbl[i][1]);
    }
    elem = wp->elems;
    for (i = 0; i < wp->count; i++, elem++) {
        elem->chain = i % g_moamoa_chain_num;
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
    d = (MoamoaExtData*)ext_moamoa_data;
    j = 0;
    strcpy(anim, "");
    strcpy(pose, "");
    wp->numGroups = -1;
    do {
        if (strcmp(d->anim, anim) != 0) {
            strcpy(anim, d->anim);
            strcpy(pose, "");
        }
        if (strcmp(d->pose, pose) != 0) {
            strcpy(pose, d->pose);
            start = j;
            wp->numGroups++;
        }
        strcpy(wp->groups[wp->numGroups].anim, anim);
        strcpy(wp->groups[wp->numGroups].pose, pose);
        wp->groups[wp->numGroups].start = start;
        wp->groups[wp->numGroups].num = j - start + 1;
        if (elem->type >= 2 && elem->type != 10) {
            wp->groups[wp->numGroups].speed = 12.0f;
        } else {
            wp->groups[wp->numGroups].speed = d->speed;
        }
        j++;
    } while ((++d)->anim != NULL);
    wp->numGroups++;
    for (i = 0; i < wp->numGroups; i++) {
    }
    return 2;
}

static EVT_BEGIN(evt_moamoa)
    CALL(evt_npc_entry, STRING("moa_a"), STRING("c_n_moa_a"))
    CALL(evt_npc_entry, STRING("moa_b"), STRING("c_n_moa_b"))
    CALL(evt_npc_set_position, STRING("moa_a"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("moa_b"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("kinopio"), STRING("c_kinodebu"))
    CALL(evt_npc_entry, STRING("cook"), STRING("c_puku_c"))
    CALL(evt_npc_entry, STRING("powan"), STRING("c_powan"))
    CALL(evt_npc_entry, STRING("sarary"), STRING("c_smoko"))
    CALL(evt_npc_entry, STRING("rose"), STRING("c_rose"))
    CALL(evt_npc_entry, STRING("papa"), STRING("c_papab"))
    CALL(evt_npc_entry, STRING("mama"), STRING("c_mamab"))
    CALL(evt_npc_entry, STRING("konari"), STRING("c_konari"))
    CALL(evt_npc_set_position, STRING("kinopio"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("cook"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("powan"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("sarary"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("rose"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("papa"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("mama"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("konari"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("kinopio"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("cook"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("powan"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("sarary"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("rose"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("papa"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("mama"), 2048)
    CALL(evt_npc_flag_onoff, 1, STRING("konari"), 2048)
    CALL(moamoa_on)
    CALL(evt_ext_entry, 750, PTR(ext_moamoa_data), PTR(ext_moamoa_init), PTR(ext_moamoa_main), 0)
    RETURN
EVT_END()

void moamoa_pose(Moamoa* elem) {
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
    MoamoaGroup* grp;
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

static void ext_moamoa_init(void) {
    s32 i;
    ExtdrvPose* pose;
    Moamoa* elem;

    pose = extGetPosePtr();
    elem = wp->elems;
    wp->targetScale = 1.0f;
    wp->scale = 1.0f;
    for (i = 0; i < wp->count; i++, pose++, elem++) {
        pose->flags |= 5;
        PSMTXIdentity(*pose->mtx);
        moamoa_pose(elem);
    }
}

EVT_BEGIN(MOAMOA_SET_BODYSCALE)
    CALL(moamoa_set_bodyScale)
    RETURN
EVT_END()

static void ext_moamoa_main(void) {
    Moamoa* elem;
    Moamoa* e4;
    Moamoa* e2;
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
    MoamoaGroup* grp;
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
                joint[j] = (*ap->worldPositions)[animPoseGetGroupIdx(wp->poseId[j], "joint_core")];
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
            a = &(*ap->worldPositions)[wp->chainIdx[e4->chain][0]];
            b = &(*ap->worldPositions)[wp->chainIdx[e4->chain][1]];
            t = e4->t;
            s = 1.0f - t;
            if (a->y == -1000.0f || b->y == -1000.0f) {
                v = (Vec){0.0f, -1000.0f, 0.0f};
                pose->flags &= ~1;
            } else {
                PSVECScale(a, &v, t);
                PSVECScale(b, &w, s);
                PSVECAdd(&v, &w, &v);
                PSVECAdd(&v, &e4->pos, &v);
                pose->flags |= 1;
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
    dispEntry(CAMERA_3D, 0.0f, 1, moamoa_draw, NULL);
}

void moamoa_draw(CameraId cameraId, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

USER_FUNC(moamoa_set_bodyScale) {
    BattleUnit* unit;
    s32 n;

    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, -2));
    n = 0;
    if (unit == NULL) {
        extReset();
        return 2;
    }
    if (unit->work[6] == -1) {
        n += 3;
    }
    if (unit->work[5] == -1) {
        n++;
    }
    if (unit->work[4] == -1) {
        n++;
    }
    if (unit->work[3] == -1) {
        n++;
    }
    wp->targetScale = 0.333f * (n - 3) + 1.0f;
    return 0;
}

