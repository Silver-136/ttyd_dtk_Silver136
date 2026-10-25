#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "dolphin/mtx.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"

typedef struct Chorogun { u8 flags; u8 state; u8 pad_2[2]; Vec vel; f32 speed; Vec pos; s32 timer; Vec target; } Chorogun;
Chorogun* chorogun;
s32 chorogun_targetId;
s32 BtlUnit_snd_se_pos(BattleUnit* unit, const char* name, s32 vol, s32 pan, Vec* pos);
int rand(void);
double sqrt(double x);

USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AttackDeclare);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_GetDamage);
USER_FUNC(btlevtcmd_GetEnemyBelong);
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
USER_FUNC(chorogun_after_attack);
USER_FUNC(chorogun_approach_target);
USER_FUNC(chorogun_bound_target);
USER_FUNC(chorogun_counterd);
USER_FUNC(chorogun_dive_target);
USER_FUNC(chorogun_escape);
USER_FUNC(chorogun_get_alive_num);
USER_FUNC(chorogun_set_target_id);
USER_FUNC(evt_btl_camera_set_homing_unit);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_ext_entry);
USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_set_position);
USER_FUNC(chorogun_escape_all);
USER_FUNC(chorogun_appear);
void chorogun_ext_disp(CameraId camera, void* param);
void chorogun_ext_main(void);
void chorogun_ext_init(void);
BattleUnit* chorogun_get_ptr(void);
extern EvtWord escape_chorogun_event[];

BattleUnitKind unit_chorobon_gundan;
typedef struct { BattleUnitDefense v; u8 pad[3]; } DefencePad;
typedef struct { BattleUnitDefenseAttr v; u8 pad[3]; } DefenceAttrPad;
typedef struct { StatusVulnerability v; u8 pad[2]; } RegistPad;
static DefencePad defence;
static DefenceAttrPad defence_attr;
static RegistPad regist;
static PoseTableEntry pose_table[15];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_chorobon_gundan_attack;
EvtWord chorogun_dead_reward[73];
static EvtWord init_event[26];
static EvtWord damage_event[59];
static EvtWord attack_event[217];
static EvtWord wait_event[6];
ExtTexInfo chorogun_ext_entry_data[21];
EvtWord chorogun_entry[19];

BattleUnit* chorogun_get_ptr(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == 0x15) {
            return unit;
        }
    }
    return NULL;
}

BattleUnitKind unit_chorobon_gundan = {
    21,
    "btl_un_chorobon_gundan",
    20,
    0,
    1,
    1,
    19,
    0,
    5,
    100,
    0,
    50,
    5,
    260,
    50,
    0,
    50,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    130.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    130.0f,
    0.0f,
    0.0f,
    130.0f,
    32.5f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    260.0f,
    50.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_TEAM_CHORO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x240000,
    &regist.v,
    1,
    parts,
    init_event,
    data_table,
};

static DefencePad defence = {{{0, 0, 0, 0, 0}}, {0}};

static DefenceAttrPad defence_attr = {{{0, 0, 0, 0, 0}}, {0}};

static RegistPad regist = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0}};

static PoseTableEntry pose_table[] = {
    {1, "CBN_N_1"},
    {2, "CBN_Y_1"},
    {9, "CBN_Y_1"},
    {5, "CBN_K_1"},
    {4, "CBN_X_1"},
    {3, "CBN_X_1"},
    {28, "CBN_S_1"},
    {29, "CBN_Q_1"},
    {30, "CBN_Q_1"},
    {31, "CBN_S_1"},
    {39, "CBN_D_1"},
    {50, "CBN_A_1"},
    {42, "CBN_R_1"},
    {40, "CBN_W_1"},
    {69, "CBN_S_1"},
};

static BattleDataEntry data_table[] = {
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_chorobon_gundan", "c_chorobon", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence.v, &defence_attr.v, 0x3000009, 0, pose_table},
};

BattleWeapon weapon_chorobon_gundan_attack = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x20000002, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    7, // special
    0x7FF, // counterResistFlags
    0x80000004, // targetWeightFlags
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

EVT_BEGIN(chorogun_dead_reward)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), -250000000, 0, -250000000)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
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
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetDamage, -2, LW(15))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), LW(15))
            BREAK_LOOP
        END_IF
        CALL(chorogun_get_alive_num, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), 50)
            CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
            ELSE
            CALL(chorogun_escape)
        END_IF
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_CheckDamageCode, -2, 256, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(chorogun_get_alive_num, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 500)
        CALL_CHILD(PTR(escape_chorogun_event))
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_chorobon_gundan_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_chorobon_gundan_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_chorobon_gundan_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_chorobon_gundan_attack))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(chorogun_set_target_id, LW(3))
    CALL(chorogun_approach_target)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), 256, LW(5))
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
    WAIT_MS(1000)
    GOTO(98)
    LABEL(91)
    CALL(chorogun_get_alive_num, LW(15))
    ADD(LW(15), 100)
    DIVIDE(LW(15), 200)
    IF_INT_LESS_OR_EQUAL(LW(15), 0)
        SET_VALUE(LW(15), 1)
    END_IF
    SET_VALUE(LW(14), 1)
    LOOP(0)
        IF_INT_GREATER(LW(14), LW(15))
            BREAK_LOOP
        END_IF
        CALL(chorogun_dive_target, 30, LW(6))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_gundan_attack))
        IF_INT_EQUAL(LW(14), LW(15))
            CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), 256, LW(5))
            ELSE
            CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), 0, LW(5))
        END_IF
        IF_INT_EQUAL(LW(5), 21)
            CALL(chorogun_counterd, LW(6))
            ELSE
            CALL(chorogun_bound_target, LW(6))
        END_IF
        ADD(LW(14), 1)
        WAIT_FRAMES(30)
    END_LOOP
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(chorogun_after_attack)
    WAIT_MS(1000)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

ExtTexInfo chorogun_ext_entry_data[] = {
    {"c_chorobon", "CBN_W_1", 0.0f},
    {"c_chorobon", "CBN_W_1", 2.0f},
    {"c_chorobon", "CBN_W_1", 4.0f},
    {"c_chorobon", "CBN_W_1", 6.0f},
    {"c_chorobon", "CBN_W_1", 8.0f},
    {"c_chorobon", "CBN_W_1", 10.0f},
    {"c_chorobon", "CBN_W_1", 12.0f},
    {"c_chorobon", "CBN_W_1", 14.0f},
    {"c_chorobon", "CBN_W_1", 16.0f},
    {"c_chorobon", "CBN_W_1", 18.0f},
    {"c_chorobon", "CBN_W_1", 20.0f},
    {"c_chorobon", "CBN_W_1", 22.0f},
    {"c_chorobon", "CBN_W_1", 24.0f},
    {"c_chorobon", "CBN_W_1", 26.0f},
    {"c_chorobon", "CBN_W_1", 28.0f},
    {"c_chorobon", "CBN_W_1", 30.0f},
    {"c_chorobon", "CBN_W_1", 32.0f},
    {"c_chorobon", "CBN_W_1", 34.0f},
    {"c_chorobon", "CBN_W_1", 36.0f},
    {"c_chorobon", "CBN_W_1", 38.0f},
    {0},
};

EVT_BEGIN(chorogun_entry)
    CALL(evt_npc_entry, STRING("ext_chorobon"), STRING("c_chorobon"))
    CALL(evt_npc_set_position, STRING("ext_chorobon"), 0, -1000, 0)
    CALL(evt_ext_entry, 1000, PTR(chorogun_ext_entry_data), PTR(chorogun_ext_init), PTR(chorogun_ext_main), 0)
    RETURN
EVT_END()

void chorogun_ext_init(void) {
    s32 i;
    s32 a, b;
    f32 tx, ty, tz;
    ExtdrvPose* pose = extGetPosePtr();

    chorogun = BattleAlloc(sizeof(Chorogun) * 1000);
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->flags = 1;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->dir = 90.0f;
        chorogun[i].flags = 0;
        chorogun[i].state = 0;
        chorogun[i].vel = (Vec){0.0f, 0.0f, 0.0f};
        if (i == 0) {
            chorogun[i].timer = 22;
        } else {
            chorogun[i].timer = irand(45);
        }
        chorogun[i].pos.x = -50.0f + 5.0f * (i % 50) + (10.0f * rand() / 32767.0f - 5.0f);
        chorogun[i].pos.y = 0.0f;
        chorogun[i].pos.z = -70.0f + 7.0f * (i / 50) + (5.0f * rand() / 32767.0f - 2.5f);
    }
    for (i = 0; i < 500; i++) {
        a = irand(extGetPoseNum());
        b = irand(extGetPoseNum());
        tx = chorogun[a].pos.x;
        ty = chorogun[a].pos.y;
        tz = chorogun[a].pos.z;
        chorogun[a].pos.x = chorogun[b].pos.x;
        chorogun[a].pos.y = chorogun[b].pos.y;
        chorogun[a].pos.z = chorogun[b].pos.z;
        chorogun[b].pos.x = tx;
        chorogun[b].pos.y = ty;
        chorogun[b].pos.z = tz;
    }
}

void chorogun_ext_main(void) {
    BattleWork* wp;
    BattleUnit* unit;
    s32 i;
    s32 n;
    ExtdrvPose* pose;
    Chorogun* c;
    f32 d;
    f32 dz;
    f32 dx;
    f32 tz;
    f32 tx;
    f32 dx2;
    f32 dz2;
    f32 x, y, z;

    wp = _battleWorkPointer;
    unit = chorogun_get_ptr();
    pose = extGetPosePtr();
    n = 0;
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].flags & 1) {
            n++;
        }
    }
    n = 1000 - n;
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->texId = (gp->retraceCount + i * i) % 20;
        c = &chorogun[i];
        (*pose->mtx)[0][3] += c->vel.x;
        (*pose->mtx)[1][3] += c->vel.y;
        (*pose->mtx)[2][3] += c->vel.z;
        switch (c->state) {
        case 0:
            break;
        case 1:
            c->timer--;
            if (c->timer > 0) {
                break;
            }
            (*pose->mtx)[0][3] = 500.0f * rand() / 32767.0f - 250.0f;
            (*pose->mtx)[1][3] = 250.0f + 100.0f * rand() / 32767.0f;
            (*pose->mtx)[2][3] = 800.0f * rand() / 32767.0f - 100.0f;
            c->vel.x = 0.0f;
            c->vel.y = 0.0f;
            c->vel.z = 0.0f;
            c->speed = 3.0f;
            c->state = 2;
            c->timer = 0;
            if (i == 0) {
                BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_FALL2", 0x7F, 0, &(Vec){0.0f, 200.0f, 200.0f});
            }
            break;
        case 2:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                if (i % 100 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_FALL1", 0x7F, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                }
                c->state = 10;
            } else {
                c->vel.y += -0.25f;
            }
            break;
        case 10:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                dx = c->pos.x - (*pose->mtx)[0][3];
                dz = c->pos.z - (*pose->mtx)[2][3];
                d = sqrt(dx * dx + dz * dz);
                if (i % 100 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_TOGETHER1", 0x7F, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                }
                if (d <= 5.0f) {
                    c->state = 20;
                } else {
                    if (d <= 20.0f) {
                        c->speed *= 0.9f;
                    }
                    c->vel.x = c->speed * (dx / d);
                    c->vel.y = c->speed;
                    c->vel.z = c->speed * (dz / d);
                    (*pose->mtx)[1][3] = 0.0f;
                }
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 20:
            (*pose->mtx)[1][3] = 0.0f;
            c->vel.x = 0.0f;
            c->vel.y = c->speed;
            c->vel.z = 0.0f;
            c->state = 21;
            break;
        case 21:
            d = unit->mPosition.x - unit->mHomePosition.x + intplGetValue(0, n, 1000, c->pos.x, unit->mHomePosition.x);
            tz = unit->mPosition.z - unit->mHomePosition.z + intplGetValue(0, n, 1000, c->pos.z, unit->mHomePosition.z);
            if ((*pose->mtx)[1][3] <= unit->mPosition.y) {
                dx2 = d - (*pose->mtx)[0][3];
                dz2 = tz - (*pose->mtx)[2][3];
                d = sqrt(dx2 * dx2 + dz2 * dz2);
                if (i % 100 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_WAIT1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                }
                c->speed = 3.0f + 0.5f * rand() / 32767.0f;
                c->vel.x = c->speed * (dx2 / d);
                c->vel.y = c->speed;
                c->vel.z = c->speed * (dz2 / d);
                (*pose->mtx)[1][3] = unit->mPosition.y;
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 30:
            c->state = 31;
            c->timer = 0;
            BtlUnit_GetPos(BattleGetUnitPtr(wp, chorogun_targetId), &x, &y, &z);
            x += 50.0f * rand() / 32767.0f - 25.0f;
            c->target.x = x;
            z += 50.0f * rand() / 32767.0f - 25.0f;
            c->target.z = z;
        case 31:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                dz2 = c->target.x - (*pose->mtx)[0][3];
                dx2 = c->target.z - (*pose->mtx)[2][3];
                d = sqrt(dz2 * dz2 + dx2 * dx2);
                if (i % 100 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_MOVE1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                }
                if (d <= 30.0f || c->timer >= 240) {
                    c->state = 40;
                } else {
                    c->vel.x = c->speed * (dz2 / d);
                    c->vel.y = c->speed;
                    c->vel.z = c->speed * (dx2 / d);
                    (*pose->mtx)[1][3] = 0.0f;
                    c->timer++;
                }
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 40:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                (*pose->mtx)[1][3] = 0.0f;
                c->vel.x = 0.0f;
                c->vel.y = c->speed;
                c->vel.z = 0.0f;
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 45:
            c->timer--;
            if (c->timer <= 0) {
                c->timer = 0;
                c->vel.y = 0.0f;
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 46:
            c->state = 47;
            c->timer = 0;
            c->vel.y = 6.0f;
            if ((*pose->mtx)[1][3] <= 0.0f) {
                (*pose->mtx)[1][3] = 0.1f;
            }
        case 47:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                c->state = 40;
                (*pose->mtx)[1][3] = 0.0f;
                c->vel.x = 0.0f;
                c->vel.y = c->speed;
                c->vel.z = 0.0f;
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 48:
            c->state = 49;
            c->timer = 0;
            c->vel.x = -4.0f;
            c->vel.y = 10.0f;
            if ((*pose->mtx)[1][3] <= 0.0f) {
                (*pose->mtx)[1][3] = 0.1f;
            }
        case 49:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                c->state = 40;
                (*pose->mtx)[1][3] = 0.0f;
                c->vel.x = 0.0f;
                c->vel.y = c->speed;
                c->vel.z = 0.0f;
            } else {
                c->vel.y += -0.5f;
            }
            break;
        case 50:
            c->state = 20;
            break;
        case 60:
            c->state = 61;
            break;
        case 61:
            if ((*pose->mtx)[1][3] <= 0.0f) {
                if (i % 100 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_MOVE1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                }
                (*pose->mtx)[1][3] = 0.0f;
                c->vel.x = 5.0f;
                c->vel.y = c->speed + irand(5);
                c->vel.z = 0.0f;
            } else {
                c->vel.y += -0.5f;
            }
            if ((*pose->mtx)[0][3] >= 250.0f) {
                c->state = 99;
            }
            break;
        case 99:
            (*pose->mtx)[0][3] = 0.0f;
            (*pose->mtx)[1][3] = -1000.0f;
            (*pose->mtx)[2][3] = 0.0f;
            break;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, 1, chorogun_ext_disp, NULL);
}

void chorogun_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

USER_FUNC(chorogun_get_alive_num) {
    EvtWord* args = event->args;
    s32 i;
    s32 n = 0;

    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].flags & 1) {
            n++;
        }
    }
    evtSetValue(event, args[0], n);
    return 2;
}

USER_FUNC(chorogun_set_target_id) {
    chorogun_targetId = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(chorogun_appear) {
    s32 i;
    s32 n;
    Chorogun* c;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            c = &chorogun[i];
            c->flags |= 1;
            c->state = 1;
        }
    }
    n = 0;
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].state == 21) {
            n++;
        }
    }
    if (n == extGetPoseNum()) {
        return 2;
    }
    return 0;
}

USER_FUNC(chorogun_approach_target) {
    s32 i;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            if (chorogun[i].flags & 1) {
                chorogun[i].state = 30;
            }
        }
    }
    for (i = 0; i < extGetPoseNum(); i++) {
        if ((chorogun[i].flags & 1) && chorogun[i].state != 40) {
            return 0;
        }
    }
    return 2;
}

USER_FUNC(chorogun_dive_target) {
    ExtdrvPose* pose;
    s32 frames;
    s32 n;
    s32 sel;
    EvtWord* args;
    Chorogun* c;
    s32 list[1000];
    s32 i;
    f32 t;
    f32* row;
    f32 x, y, z;
    f32 dx, dy, dz;
    ExtdrvPose* p;
    BattleUnit* unit;

    args = event->args;
    n = 0;
    frames = evtGetValue(event, args[0]);
    t = frames;
    pose = extGetPosePtr();
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i & 0xFFFFFFFFFFFFFFFFu].flags & 1) {
            list[n++] = i;
        }
    }
    sel = list[irand(n)];
    c = &chorogun[sel];
    c->state = 45;
    c->timer = frames;
    BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, chorogun_targetId), &x, &y, &z);
    p = &pose[sel];
    row = (*p->mtx)[2];
    dx = x - (*p->mtx)[0][3];
    dy = y - row[3];
    dz = z - row[3];
    c->vel.x = dx / t;
    c->vel.y = (2.0f * dy - -0.5f * t * t) / (2.0f * t);
    c->vel.z = dz / t;
    evtSetValue(event, args[1], sel);
    unit = chorogun_get_ptr();
    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_ATTACK1", 0x80, 0, &(Vec){(*p->mtx)[0][3], (*p->mtx)[1][3], (*p->mtx)[2][3]});
    return 2;
}

USER_FUNC(chorogun_bound_target) {
    s32 id = evtGetValue(event, event->args[0]);

    chorogun[id].state = 46;
    return 2;
}

USER_FUNC(chorogun_counterd) {
    s32 id = evtGetValue(event, event->args[0]);

    chorogun[id].state = 48;
    return 2;
}

USER_FUNC(chorogun_after_attack) {
    s32 i;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            if (chorogun[i].flags & 1) {
                chorogun[i].state = 50;
            }
        }
    }
    return 2;
}

USER_FUNC(chorogun_escape) {
    f32 dist[1000];
    s32 idx[1000];
    BattleUnit* unit = chorogun_get_ptr();
    s32 i, j, m;
    s32 n = 0;
    Chorogun* c;
    ExtdrvPose* pose = extGetPosePtr();
    f32 dx, dz, d, tf;
    s32 ti;

    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        if (chorogun[i].flags & 1) {
            dx = unit->mHomePosition.x - (*pose->mtx)[0][3];
            dz = unit->mHomePosition.z - (*pose->mtx)[2][3];
            d = dx * dx + dz * dz;
            if (n >= 50) {
                if (d <= 2500.0f) {
                    dist[n] = d;
                    idx[n] = i;
                    n++;
                }
            } else {
                dist[n] = d;
                idx[n] = i;
                n++;
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 1; j < n; j++) {
            if (dist[i] > dist[j]) {
                tf = dist[i];
                ti = idx[i];
                dist[i] = dist[j];
                idx[i] = idx[j];
                dist[j] = tf;
                idx[j] = ti;
            }
        }
    }
    m = n >= 50 ? 50 : n;
    for (i = 0; i < m; i++) {
        c = &chorogun[idx[i]];
        c->flags &= ~1;
        c->state = 60;
    }
    return 2;
}

USER_FUNC(chorogun_escape_all) {
    s32 i;
    Chorogun* c;

    for (i = 0; i < extGetPoseNum(); i++) {
        c = &chorogun[i];
        if (c->flags & 1) {
            c->flags &= ~1;
            c->state = 60;
        }
    }
    return 2;
}

