#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "pmario_sound.h"
f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
s32 irand(s32 scalar);
#include "dolphin/mtx.h"
#include "battle/battle_database_common.h"
#include "driver/extdrv.h"

typedef struct ZakoGun { s8 field_0x0; s8 state; s32 timer; s32 timerMax; Vec pos; Vec home; s32 poseBase; s32 poseNum; s32 poseDiv; s32 poseCount; Vec start; Vec target; Vec vel; u8 field_0x58[0x64 - 0x58]; Vec rot; } ZakoGun;
#ifdef VERSION_G8ME01
#define ZAKOGUNWORK_PAD 0x54
#else
#define ZAKOGUNWORK_PAD 0x48
#endif
#ifdef VERSION_G8ME01
typedef struct ZakoGunWork { s32 flag; s32 targetId; Vec targetPos; u8 field_0x14[ZAKOGUNWORK_PAD - 0x14]; Vec start; Vec end; s32 timer; s32 steps; s32 count; s32 sfx; } ZakoGunWork;
#else
typedef struct ZakoGunWork { s32 flag; s32 targetId; u8 field_0x8[ZAKOGUNWORK_PAD - 0x8]; Vec start; Vec end; s32 timer; s32 steps; s32 count; s32 sfx; } ZakoGunWork;
#endif
extern ZakoGun* zakogun;
extern ZakoGunWork* work;
void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i);
BattleUnit* zakogun_get_ptr(void);
void zakogun_set_pose(ZakoGun* zako, s32 base, s32 num, s32 div);
int rand(void);
void btl_camera_set_mode(s32 a, s32 b);

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AttackDeclare);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_CheckToken);
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy);
USER_FUNC(btlevtcmd_GetDamage);
USER_FUNC(btlevtcmd_GetEnemyBelong);
USER_FUNC(btlevtcmd_GetSelectEnemy);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_PayWeaponCost);
USER_FUNC(btlevtcmd_PreCheckDamage);
USER_FUNC(btlevtcmd_ResultACDefence);
USER_FUNC(btlevtcmd_SamplingEnemy);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartAvoid);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_WaitGuardMove);
USER_FUNC(btlevtcmd_WeaponAftereffect);
USER_FUNC(evt_btl_camera_set_homing_unit);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_btl_camera_shake_h);
USER_FUNC(evt_eff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_sub_random);
USER_FUNC(shake_ceil);
USER_FUNC(zakogun_attack2);
USER_FUNC(zakogun_attack2_yusuri);
USER_FUNC(zakogun_damage_pose);
USER_FUNC(zakogun_damage_pose_cancel);
USER_FUNC(zakogun_set_target_id);
USER_FUNC(zakogun_escape2);
USER_FUNC(zakogun_dead2);
void zakogun_ext_main2(void);
USER_FUNC(zakogun_ext_init2);
USER_FUNC(up_ceil);
USER_FUNC(down_ceil);

BattleUnitKind unit_boss_gundan_zako_group2;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gundan_zako_group2_attack;
BattleWeapon weapon_gundan_zako_group2_yusuri_attack;
static EvtWord init_event[21];
static EvtWord damage_event[129];
static EvtWord dead_event[136];
static EvtWord attack_event[32];
EvtWord fall_attack_event[93];
static EvtWord shake_attack_event[281];
static EvtWord hit_check_event[34];
EvtWord hit_check_event2[7];
EvtWord hit_check_event_sub[123];
static EvtWord wait_event[6];

BattleUnitKind unit_boss_gundan_zako_group2 = {
    101,
    "btl_un_gundan_zako_group2",
    6,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    9999,
    400,
    36,
    0,
    55,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    200.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    200.0f,
    0.0f,
    0.0f,
    200.0f,
    23.4f,
    0.0f,
    60.0f,
    -35.0f,
    0.0f,
    280.0f,
    36.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_TEAM_3RD_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200002,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "Z_1"},
    {31, "Z_1"},
    {39, "Z_1"},
    {69, "Z_1"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako_group2", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x3600009, 0, pose_table},
};

BattleWeapon weapon_gundan_zako_group2_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0x7FF, // counterResistFlags
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

BattleWeapon weapon_gundan_zako_group2_yusuri_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0x7FF, // counterResistFlags
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, -2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
        ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 27, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 1, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 3, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 5, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 9, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 2, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 4, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 8, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 11, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 15, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 20, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, 0, LW(14))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(zakogun_damage_pose)
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(zakogun_damage_pose_cancel)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
    BEGIN_INLINE_EVENT
        CALL(up_ceil)
    END_INLINE_EVENT
    CALL(zakogun_dead2)
    WAIT_FRAMES(50)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(48)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(20)
            CALL(evt_sub_random, 500, LW(3))
            SUBTRACT(LW(3), 250)
            SET_VALUE(LW(0), 0)
            ADD(LW(0), LW(3))
            CALL(evt_sub_random, 30, LW(3))
            SET_VALUE(LW(1), 0)
            ADD(LW(1), LW(3))
            SET_VALUE(LW(2), 75)
            CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetCoin, -2, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, -2, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(_add_star_point_disp_offset, -2, 0, 0, 0)
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, 0, 0, 0, LW(0), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, -2, 0)
    ELSE
        CALL(btlevtcmd_KillUnit, -2, 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    SET_VALUE(LW(0), 50)
    ADD(LW(0), 50)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 50)
        CALL_CHILD(PTR(fall_attack_event))
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(shake_attack_event))
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(fall_attack_event)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_group2_attack))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(zakogun_set_target_id, LW(3))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    BEGIN_SIBLING_EVENT
        CALL_CHILD(PTR(hit_check_event))
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_MOVE_CAILING2"), 0)
    CALL(zakogun_attack2)
    DELETE_EVENT(LW(15))
    RETURN
EVT_END()

static EVT_BEGIN(shake_attack_event)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_group2_yusuri_attack))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(zakogun_set_target_id, LW(3))
    BEGIN_SIBLING_EVENT
        LOOP(8)
            CALL(shake_ceil)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.5f))
        LOOP(7)
            WAIT_FRAMES(18)
            CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 18, 0)
            CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_MOVE_CAILING3"), 0)
            ADD_FLOAT(LW(0), FLOAT(0.5f))
        END_LOOP
        WAIT_FRAMES(18)
        CALL(evt_btl_camera_shake_h, 0, LW(0), 0, 100, 0)
    END_SIBLING_EVENT
    CALL(zakogun_attack2_yusuri)
    BEGIN_SIBLING_EVENT
        CALL(evt_eff, STRING(""), STRING("fall"), 2, 0, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(20)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, 100, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(35)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, -35, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(57)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, -100, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL_CHILD(PTR(hit_check_event2))
        END_IF
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(82)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, 80, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(90)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, 40, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(105)
        CALL(evt_eff, STRING(""), STRING("fall"), 2, 130, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    END_SIBLING_EVENT
    WAIT_FRAMES(115)
    CALL(evt_eff, STRING(""), STRING("fall"), 2, -145, 0, 20, 0, 250, 0, 1, 60, 0, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL_CHILD(PTR(hit_check_event2))
    END_IF
    RETURN
EVT_END()

USER_FUNC(down_ceil) {
    u8* ceil = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body = BattleSearchObjectPtr("if_body");
    BattleStageObject* wire = BattleSearchObjectPtr("if_wire");

    if (isFirstCall) {
        work->start = (Vec){0.0f, 250.0f, 0.0f};
        work->end = (Vec){0.0f, 130.0f, 0.0f};
        work->timer = 120;
        work->steps = 120;
        *ceil &= ~0x20;
        work->sfx = work->sfx = psndSFXOn("SFX_BOSS_TEAM_3RD_MOVE_CAILING1");
    }
    body->mPosition.y = wire->mPosition.y = intplGetValue(work->start.y, work->end.y, 4, work->steps - work->timer, work->steps);
    work->timer--;
    if (work->timer < 0) {
        psndSFXOff(work->sfx);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(up_ceil) {
    u8* ceil = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body = BattleSearchObjectPtr("if_body");
    BattleStageObject* wire = BattleSearchObjectPtr("if_wire");

    if (isFirstCall) {
        work->start = (Vec){0.0f, 130.0f, 0.0f};
        work->end = (Vec){0.0f, 250.0f, 0.0f};
        work->timer = 120;
        work->steps = 120;
        *ceil |= 0x20;
        work->sfx = psndSFXOn("SFX_BOSS_TEAM_3RD_MOVE_CAILING1");
    }
    body->mPosition.y = wire->mPosition.y = intplGetValue(work->start.y, work->end.y, 4, work->steps - work->timer, work->steps);
    work->timer--;
    if (work->timer < 0) {
        psndSFXOff(work->sfx);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(shake_ceil) {
    BattleStageObject* body = BattleSearchObjectPtr("if_body");
    BattleStageObject* wire = BattleSearchObjectPtr("if_wire");

    if (isFirstCall) {
        work->start = (Vec){0.0f, 130.0f, 0.0f};
        work->end = (Vec){0.0f, 135.0f, 0.0f};
        work->timer = 0;
    }
    body->mPosition.y = wire->mPosition.y = intplGetValue(work->start.y, work->end.y, 4, work->timer, 9);
    work->timer++;
    if (work->timer > 17) {
        body->mPosition.y = wire->mPosition.y = 130.0f;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_ext_init2) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    work->flag = 1;
    for (i = 0; i < 25; i++, pose++) {
        zakogun_ext_init123_common(pose, i);
    }
    return EVT_RETURN_DONE;
}

void zakogun_ext_main2(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = zakogun_get_ptr();
    ExtdrvPose* pose = extGetPosePtr();
    BattleStageObject* body;
    s32 i;
    ZakoGun* zako;
    f32 x, y, z;
    Mtx m;
    const char* se1;
    const char* se2;
    const char* se3;

    if (unit == NULL) {
        return;
    }
    body = BattleSearchObjectPtr("if_body");
    se1 = (0, "SFX_BOSS_TEAM_3RD_ATTACK2");
    se2 = (0, "SFX_BOSS_TEAM_3RD_MOVE_CAILING2");
    se3 = "SFX_BOSS_TEAM_3RD_ATTACK2";
    for (i = 0; i < 25; i++, pose++) {
        zako = &zakogun[i];
        pose->texId = zako->poseBase + zako->poseCount / zako->poseDiv;
        zako->poseCount++;
        if (zako->poseCount >= zako->poseNum * zako->poseDiv) {
            zako->poseCount = 0;
        }
        switch (zako->state) {
            case 0:
                zako->state = 5;
                zako->rot.x = zako->rot.y = zako->rot.z = 0.0f;
                zakogun_set_pose(zako, 0x25, 4, 6);
                break;
            case 5:
                zako->pos.x = zako->home.x = body->mPosition.x - 250.0f + i * 20.0f;
                zako->pos.y = zako->home.y = body->mPosition.y - 35.0f;
                zako->pos.z = zako->home.z = 0.0f;
                zako->state = 10;
                break;
            case 10:
                zako->pos.x = zako->home.x = body->mPosition.x + unit->mPosition.x - 250.0f + i * 20.0f;
                zako->pos.y = zako->home.y = body->mPosition.y - 35.0f;
                if (zako->pos.y < unit->mPosition.y) {
                    zako->pos.y = unit->mPosition.y;
                }
                zako->pos.z = zako->home.z = -10.0f;
                break;
            case 20:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->start = zako->pos;
                if (zako->pos.x >= x) {
                    zako->target.x = zako->pos.x - 30.0f;
                    zako->rot.y = 0.0f;
                } else {
                    zako->target.x = 15.0f + zako->pos.x;
                    zako->rot.y = 180.0f;
                }
                zako->target.y = zako->start.y - 2.0f;
                zako->state = 25;
                zako->timer = zako->timerMax = 8;
            case 25:
                zako->pos.x = intplGetValue(zako->start.x, zako->target.x, 4, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y = intplGetValue(zako->start.y, zako->target.y, 4, (zako->timerMax - zako->timer) * 2, zako->timerMax);
                zako->timer--;
                if (zako->timer < 0) {
                    BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                    if (__fabs(zako->pos.x - x) <= 20.0) {
                        zako->state = 30;
                    } else {
                        zako->state = 20;
                    }
                }
                break;
            case 30:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->vel.x = rand() / 32767.0f - 0.5f;
                zako->vel.y = 0.0f;
                zako->vel.z = 1.5f;
                zako->target.y = y + work->count * 2.0f;
                zako->state = 35;
                zakogun_set_pose(zako, 0x2C, 1, 1);
                psndSFXOn(se1);
            case 35:
                zako->pos.x += zako->vel.x;
                zako->pos.y += zako->vel.y;
                zako->pos.z += zako->vel.z;
                zako->vel.y += -0.5f;
                if (zako->pos.y < zako->target.y) {
                    zako->pos.y = zako->target.y;
                    zako->state = 40;
                    work->count++;
                    if (work->count == 25) {
                        unit->work[0] = 1;
                    }
                }
                break;
            case 40:
                if (work->count == 25) {
                    zako->state = 45;
                    zako->timer = irand(60);
                }
                break;
            case 45:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->vel.x = -5.0f;
                    zako->vel.y = 3.0f;
                    zako->state = 48;
                    zakogun_set_pose(zako, 0x22, 1, 1);
                    zako->rot.y = 0.0f;
                }
                break;
            case 48:
                zako->pos.x += zako->vel.x;
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                if (zako->pos.y < 0.0f) {
                    zako->pos.y = 0.0f;
                    zakogun_set_pose(zako, 0x16, 10, 1);
                    zako->state = 49;
                }
                break;
            case 49:
                zako->pos.x += zako->vel.x;
                if (zako->pos.x <= -300.0f) {
                    zako->state = 50;
                    zako->pos.x = 300.0f;
                    zako->pos.y = zako->home.y;
                    zako->pos.z = zako->home.z;
                    zakogun_set_pose(zako, 0x25, 4, 6);
                    if (i == 0) {
                        psndSFXOn(se2);
                    }
                }
                break;
            case 50:
#ifdef VERSION_G8ME01
            {
                BattleUnit* tgt = BattleGetUnitPtr(wp, work->targetId);
                if (tgt != NULL) {
                    BtlUnit_GetPos(tgt, &x, &y, &z);
                    work->targetPos = (Vec){x, y, z};
                } else {
                    x = work->targetPos.x;
                    y = work->targetPos.y;
                    z = work->targetPos.z;
                }
            }
#else
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
#endif
                zako->rot.y = 0.0f;
                zako->start = zako->pos;
                zako->target.x = zako->pos.x - 30.0f;
                if (zako->target.x < zako->home.x) {
                    zako->target.x = zako->home.x;
                }
                zako->target.y = zako->start.y - 2.0f;
                zako->state = 55;
                zako->timer = zako->timerMax = 8;
                btl_camera_set_mode(0, 0);
            case 55:
                zako->pos.x = intplGetValue(zako->start.x, zako->target.x, 4, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y = intplGetValue(zako->start.y, zako->target.y, 4, (zako->timerMax - zako->timer) * 2, zako->timerMax);
                zako->timer--;
                if (zako->timer < 0) {
                    if (zako->pos.x <= zako->home.x) {
                        zako->pos.x = zako->home.x;
                        zako->state = 10;
                    } else {
                        zako->state = 50;
                    }
                }
                break;
            case 60:
                zakogun_set_pose(zako, 0x29, 3, 6);
                zako->state = 65;
                zako->timer = 120;
            case 65:
                zako->pos.x = body->mPosition.x - 250.0f + i * 20.0f;
                zako->pos.y = body->mPosition.y - 35.0f;
                zako->pos.z = 0.0f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 10;
                    zakogun_set_pose(zako, 0x25, 4, 6);
                }
                break;
            case 80:
                zako->timer = irand(10);
                zako->state = 82;
            case 82:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 83;
                    zako->vel.y = 0.0f;
                    psndSFXOn(se3);
                }
                break;
            case 83:
                zako->vel.y += -0.5f;
                zako->pos.y += zako->vel.y;
                if (zako->pos.y <= 0.0f) {
                    zako->pos.y = 0.0f;
                    zako->state = 84;
                    zakogun_set_pose(zako, 0x21, 3, 3);
                }
                break;
            case 84:
                zako->start = zako->pos;
                zako->target.x = zako->start.x + irand(100) - 50.0f;
                zako->target.y = 0.0f;
                zako->target.z = zako->start.z + irand(20) - 10.0f;
                zako->state = 85;
                zako->timer = zako->timerMax = 20;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -200.0f) / 40.0f;
            case 85:
                zako->pos.x = intplGetValue(zako->start.x, zako->target.x, 0, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(zako->start.z, zako->target.z, 0, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 86;
                }
                break;
            case 86:
                zako->start = zako->pos;
                zako->target.x = zako->start.x + irand(50) - 25.0f;
                zako->target.y = 0.0f;
                zako->target.z = zako->start.z + irand(10) - 5.0f;
                zako->state = 87;
                zako->timer = zako->timerMax = 10;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -50.0f) / 20.0f;
            case 87:
                zako->pos.x = intplGetValue(zako->start.x, zako->target.x, 0, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(zako->start.z, zako->target.z, 0, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 88;
                    zako->rot.y = 0.0f;
                    zako->timer = 48;
                }
                break;
            case 88:
                zako->rot.y -= 15.0f;
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 89;
                }
                break;
            case 89:
                zako->rot.x += 3.0f;
                if (zako->rot.x >= 90.0f) {
                    zako->state = -1;
                    zako->pos = (Vec){0.0f, -1000.0f, 0.0f};
                }
                break;
            case 90:
                zako->pos.x = 0.0f;
                zako->pos.y = -1000.0f;
                zako->pos.z = 0.0f;
                zako->state = -1;
                break;
        }
        PSMTXIdentity(*pose->mtx);
        PSMTXRotRad(m, 'x', zako->rot.x * 0.017453292f);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        PSMTXRotRad(m, 'y', zako->rot.y * 0.017453292f);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        (*pose->mtx)[0][3] = zako->pos.x;
        (*pose->mtx)[1][3] = zako->pos.y;
        (*pose->mtx)[2][3] = zako->pos.z + i * 0.25f;
    }
}

USER_FUNC(zakogun_attack2) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        work->count = 0;
        for (i = 0; i < 25; i++) {
            zakogun[i].state = 20;
        }
    }
    i = 0;
    n = 0;
    for (; i < 25; i++) {
        if (zakogun[i].state == 10) {
            n++;
        }
    }
    return n == 25 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_attack2_yusuri) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 25; i++) {
            zakogun[i].state = 60;
        }
    }
    i = 0;
    n = 0;
    for (; i < 25; i++) {
        if (zakogun[i].state == 10) {
            n++;
        }
    }
    return n == 25 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_dead2) {
    s32 i;

    for (i = 0; i < 25; i++) {
        zakogun[i].state = 80;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_escape2) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 25; i++) {
            zakogun[i].state = 90;
        }
    }
    i = 0;
    n = 0;
    for (; i < 25; i++) {
        if (zakogun[i].state == -1) {
            n++;
        }
    }
    return n == 25 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static EVT_BEGIN(hit_check_event)
    WAIT_FRAMES(10)
    LOOP(4)
        WAIT_FRAMES(20)
        SET_VALUE(LW(15), 0)
        CALL_CHILD(PTR(hit_check_event_sub))
    END_LOOP
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(LW(15), 1)
    CALL_CHILD(PTR(hit_check_event_sub))
    RETURN
EVT_END()

EVT_BEGIN(hit_check_event2)
    SET_VALUE(LW(15), 1)
    CALL_CHILD(PTR(hit_check_event_sub))
    RETURN
EVT_END()

EVT_BEGIN(hit_check_event_sub)
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_group2_attack))
        IF_INT_EQUAL(LW(15), 1)
            CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                GOTO(92)
            END_IF
        END_IF
    END_IF
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
    GOTO(98)
    LABEL(91)
    LABEL(92)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(15), 1)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        CALL(evt_btl_camera_set_mode, 0, 0)
        ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_group2_attack))
        CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    END_IF
    GOTO(98)
    LABEL(98)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

