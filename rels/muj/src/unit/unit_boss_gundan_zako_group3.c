#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "libc_decls.h"
#include "battle/battle_camera.h"
#include "battle/battle_status_effect.h"
#include "rels/muj/include/muj.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_stage_object.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

typedef struct ZakoGun {
    s8 field_0x0;
    s8 state;
    s32 timer;
    s32 timerMax;
    Vec pos;
    Vec home;
    s32 poseBase;
    s32 poseNum;
    s32 poseDiv;
    s32 poseCount;
    Vec start;
    Vec target;
    Vec vel;
    Vec ofs;
    Vec rot;
} ZakoGun;
typedef struct ZakoGunWork { s32 flag; s32 targetId;
#ifdef VERSION_G8ME01
    u8 us_pad08[0xC]; // US: 12 more bytes here
#endif
    s32 state; s32 timer; Vec pos; u8 field_0x1c[0x34 - 0x1c]; f32 radius; f32 angle; f32 speed; u8 field_0x40[0x48 - 0x40]; Vec start; Vec end; s32 timer2; s32 steps; s32 count; s32 sfx; } ZakoGunWork;
extern ZakoGun* zakogun;
extern ZakoGunWork* work;
void zakogun_set_pose(ZakoGun* zako, s32 base, s32 num, s32 div);
void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i);
double sqrt(double x);

USER_FUNC(zakogun_damage_pose);
USER_FUNC(zakogun_damage_pose_cancel);
USER_FUNC(zakogun_set_target_id);
USER_FUNC(zakogun_escape3);
USER_FUNC(zakogun_dead3);
USER_FUNC(zakogun_attack3_3);
USER_FUNC(zakogun_attack3_2);
USER_FUNC(zakogun_attack3_1);
USER_FUNC(zakogun_ext_init3);
USER_FUNC(zakogun_kanbu_on_ball);
extern s32 zakogun_get_kanbu_id_evtcmd[];

BattleUnitKind unit_boss_gundan_zako_group3;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gundan_zako_group3_attack;
static EvtWord init_event[21];
static EvtWord damage_event[129];
static EvtWord dead_event[136];
static EvtWord attack_event[287];
static EvtWord hit_check_event[119];
static EvtWord wait_event[6];

USER_FUNC(zakogun_kanbu_on_ball) {
    BattleUnit* kanbu = BattleGetUnitPtr(_battleWorkPointer, zakogun_get_kanbu_id());
    BtlUnit_SetPos(kanbu, work->pos.x, 45.0f + work->pos.y, work->pos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_ext_init3) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();
    BattleUnit* kanbu;
    s32 a;
    s32 b;
    f32 dx, dy, dz, dist;

    work->flag = 2;
    for (i = 0; i < 500; i++, pose++) {
        zakogun_ext_init123_common(pose, i);
        zakogun[i].home.x = -50.0f + 20.0f * (i % 12) + (5.0f * rand() / 32767.0f - 2.5f);
        zakogun[i].home.y = 0.0f;
        zakogun[i].home.z = -80.0f + 4.0f * (i / 12) + (5.0f * rand() / 32767.0f - 2.5f);
        kanbu = BattleGetUnitPtr(_battleWorkPointer, zakogun_get_kanbu_id());
        dx = zakogun[i].home.x - kanbu->mHomePosition.x;
        dy = zakogun[i].home.y - kanbu->mHomePosition.y;
        dz = zakogun[i].home.z - kanbu->mHomePosition.z;
        dist = sqrt(dx * dx + dy * dy + dz * dz);
        if (dist <= 30.0f) {
            zakogun[i].home.x += 30.0f * (dx / dist);
            zakogun[i].home.y += 30.0f * (dy / dist);
            zakogun[i].home.z += 30.0f * (dz / dist);
        }
    }
    for (i = 0; i < 250; i++) {
        Vec tmp;
        a = irand(500);
        b = irand(500);
        tmp = zakogun[a].home;
        zakogun[a].home = zakogun[b].home;
        zakogun[b].home = tmp;
    }
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_boss_gundan_zako_group3 = {
    UNIT_X_NAUTS_CRUMP_FORMATION3, // mUnitType
    "btl_un_gundan_zako_group3", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    260, // width
    52, // height
    {0, 52}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, -15}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {130.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {150.0f, 0.0f, 70.0f}, // bintaHitOffset
    {130.0f, 33.8f, 0.0f}, // kissHitOffset
    {10.0f, 10.0f, 0.0f}, // cutBaseOffset
    260.0f, // cutWidth
    52.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_TEAM_3RD_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "Z_1"},
    {POSE_TYPE_DAMAGE, "Z_1"},
    {POSE_TYPE_ACTION, "Z_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako_group3", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHidden | kNoShadow, 0, pose_table},
};

BattleWeapon weapon_gundan_zako_group3_attack = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
        ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_BURN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DEFENSE_DOWN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLOW, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_ALLERGIC, LW(14))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(zakogun_damage_pose)
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(zakogun_damage_pose_cancel)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 1)
    CALL(zakogun_dead3)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(48)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(20)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(evt_sub_random, 300, LW(3))
            SUBTRACT(LW(3), 150)
            ADD(LW(0), LW(3))
            CALL(evt_sub_random, 30, LW(3))
            ADD(LW(1), LW(3))
            SET_VALUE(LW(2), 75)
            CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetCoin, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(1), LW(2))
    CALL(_add_star_point_disp_offset, UNIT_ID_SELF, LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    ELSE
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_group3_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(zakogun_get_kanbu_id_evtcmd, LW(5))
    CALL(btlevtcmd_CheckActStatus, LW(5), LW(12))
    IF_INT_NOT_EQUAL(LW(12), 0)
        CALL(zakogun_get_kanbu_id_evtcmd, LW(5))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            WAIT_FRAMES(10)
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("W_1"))
            CALL(btlevtcmd_GetPos, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionSub, LW(5), LW(0), 50)
            CALL(btlevtcmd_MovePosition, LW(5), LW(0), LW(1), LW(2), 60, -1, 0)
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("S_1"))
        END_SIBLING_EVENT
    END_IF
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(32)
        CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_ADHERE1"), 0)
    END_SIBLING_EVENT
    CALL(zakogun_attack3_1)
    IF_INT_NOT_EQUAL(LW(12), 0)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("A_1A"))
        CALL(btlevtcmd_snd_se, LW(5), STRING("SFX_BOSS_KANBU_JUMP1"), EVT_NULLPTR, 0, LW(13))
        CALL(btlevtcmd_JumpSetting, LW(5), 0, FLOAT(6.0f), FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, LW(5), 0, 100, 0, 0, -1)
        CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("S_1"))
        CALL(evt_snd_sfxoff, LW(13))
    END_IF
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(zakogun_set_target_id, LW(3))
    IF_INT_NOT_EQUAL(LW(12), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            CALL(zakogun_get_kanbu_id_evtcmd, LW(5))
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("R_2"))
            LOOP(0)
                CALL(zakogun_kanbu_on_ball)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    BEGIN_SIBLING_EVENT
        CALL_CHILD(hit_check_event)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_ROUND1"), LW(13))
    CALL(zakogun_attack3_2, LW(13))
    IF_INT_NOT_EQUAL(LW(12), 0)
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), 0)
        BEGIN_INLINE_EVENT
            CALL(zakogun_get_kanbu_id_evtcmd, LW(5))
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("A_1A"))
            CALL(btlevtcmd_GetPos, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpSetting, LW(5), 30, 0, FLOAT(0.2998047f))
            CALL(btlevtcmd_JumpPosition, LW(5), LW(0), 0, LW(2), 30, -1)
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("S_1"))
            CALL(btlevtcmd_ChangeFaceDirection, LW(5), 1)
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("R_1"))
            CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetMoveSpeed, LW(5), FLOAT(6.0f))
            CALL(btlevtcmd_MovePosition, LW(5), LW(0), LW(1), LW(2), 0, -1, 0)
            CALL(btlevtcmd_AnimeChangePose, LW(5), 1, STRING("S_1"))
            CALL(btlevtcmd_ResetFaceDirection, LW(5))
        END_INLINE_EVENT
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_TEAM_3RD_ADHERE2"), 0)
    CALL(zakogun_attack3_3)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

void zakogun_ext_main3(void) {
    s32 i;
    BattleUnit* unit = zakogun_get_ptr();
    ExtdrvPose* pose = extGetPosePtr();
    ZakoGun* zako;
    f32 left;
    f32 top;
    f32 dx;
    f32 dy;
    Mtx m;
    Mtx m2;

    for (i = 0; i < 500; i++, pose++) {
        zako = &zakogun[i];
        pose->texId = zako->poseBase + zako->poseCount / zako->poseDiv;
        zako->poseCount++;
        if (zako->poseCount >= zako->poseNum * zako->poseDiv) {
            zako->poseCount = 0;
        }
        switch (zako->state) {
            case 0:
                zako->state = 3;
                zako->timer = 0;
                zako->rot.x = zako->rot.y = zako->rot.z = 0.0f;
                zakogun_set_pose(zako, 0, 20, 2);
                break;
            case 3:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 5;
                    zako->timer = 0;
                }
                break;
            case 5:
                zako->start.x = 300.0f;
                zako->start.y = 0.0f;
                zako->start.z = -75.0f + 150.0f * rand() / 32767.0f;
                zako->timerMax = zako->timer = irand(30) + 60;
                zako->state = 10;
                zakogun_set_pose(zako, 0x16, 10, 1);
            case 10:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->home.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y = intplGetValue(0, zako->start.y, zako->home.y, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->home.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 15;
                    zakogun_set_pose(zako, 0, 20, 2);
                }
                break;
            case 15:
                zako->pos.x = zako->home.x + (unit->mPosition.x - unit->mHomePosition.x);
                zako->pos.y = zako->home.y;
                if (zako->pos.y < unit->mPosition.y) {
                    zako->pos.y = unit->mPosition.y;
                }
                zako->pos.z = zako->home.z;
                break;
            case 20:
                if (i >= 0 && i < 484) {
                    left = work->pos.x - work->radius;
                    top = work->pos.y + work->radius;
                    zako->target.x = left + (i % 22) * (2.0f * work->radius / 22.0f);
                    zako->target.y = top - (i / 22) * (2.0f * work->radius / 22.0f);
                    zako->target.z = work->pos.z + rand() / 32767.0f - 0.5f;
                    dx = work->pos.x - zako->target.x;
                    dy = work->pos.y - zako->target.y;
                    if ((f32)sqrt(dx * dx + dy * dy) > work->radius) {
                        zako->target.x = work->pos.x;
                        zako->target.y = work->pos.y;
                        zako->target.z = work->pos.z - 10.0f;
                    }
                } else {
                    zako->target.x = work->pos.x;
                    zako->target.y = work->pos.y;
                    zako->target.z = work->pos.z - 10.0f;
                }
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -200.0f) / 40.0f;
                zako->state = 25;
                zako->timer = zako->target.y;
            case 25:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 30;
                    zako->timerMax = zako->timer = 30;
                    zakogun_set_pose(zako, 0x22, 1, 1);
                }
                break;
            case 30:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 35;
                    zako->pos.y = zako->target.y;
                    zako->rot.z = 360.0f * rand() / 32767.0f;
                    zako->ofs.x = zako->pos.x - work->pos.x;
                    zako->ofs.y = zako->pos.y - work->pos.y;
                    zakogun_set_pose(zako, 0, 20, 2);
                    work->count++;
                }
                break;
            case 35:
                if (work->count == 500) {
                    zako->state = 40;
                }
                break;
            case 50:
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->target.x = 200.0f * rand() / 32767.0f - 100.0f;
                zako->target.y = 0.0f;
                zako->target.z = 150.0f * rand() / 32767.0f - 75.0f;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -200.0f) / 40.0f;
                zako->rot.z = 0.0f;
                zako->state = 55;
                zako->timer = zako->timerMax = 20;
                zakogun_set_pose(zako, 0x22, 1, 1);
            case 55:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 60;
                    zako->pos.y = zako->target.y;
                    zakogun_set_pose(zako, 0, 20, 2);
                }
                break;
            case 60:
                zako->start.x = zako->pos.x;
                zako->start.z = zako->pos.z;
                zako->timerMax = zako->timer = irand(30) + 30;
                zako->state = 65;
                zakogun_set_pose(zako, 0x16, 10, 1);
            case 65:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->home.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->home.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 15;
                    zako->pos.y = zako->home.y;
                    zakogun_set_pose(zako, 0, 20, 2);
                }
                break;
            case 80:
                zako->rot.y = 0.0f;
                zako->timer = irand(5) + 48;
                zako->state = 82;
            case 82:
                if (zako->timer <= 48) {
                    zako->rot.y -= 15.0f;
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 85;
                }
                break;
            case 85:
                zako->rot.x += 3.0f;
                if (zako->rot.x >= 90.0f) {
                    zako->state = -1;
                    {
                        Vec hide = {0.0f, -1000.0f, 0.0f};
                        zako->pos = hide;
                    }
                }
                break;
            case 90:
                zako->rot.y = 180.0f;
                zako->pos.x += 5.0f;
                if (zako->pos.x >= 300.0f) {
                    zako->state = -1;
                    zako->rot.y = 0.0f;
                }
                break;
        }
        if (zako->state >= 35 && zako->state <= 40) {
            PSMTXIdentity(*pose->mtx);
            PSMTXRotRad(m, 'z', 0.017453292f * zako->rot.z);
            PSMTXConcat(*pose->mtx, m, *pose->mtx);
            (*pose->mtx)[0][3] = zako->ofs.x;
            (*pose->mtx)[1][3] = zako->ofs.y;
            (*pose->mtx)[2][3] = work->pos.z + 0.05f * i;
            PSMTXRotRad(m, 'z', 0.017453292f * work->angle);
            PSMTXConcat(m, *pose->mtx, *pose->mtx);
            (*pose->mtx)[0][3] += work->pos.x;
            (*pose->mtx)[1][3] += work->pos.y;
            (*pose->mtx)[2][3] += work->pos.z;
            zako->pos.x = (*pose->mtx)[0][3];
            zako->pos.y = (*pose->mtx)[1][3];
            zako->pos.z = (*pose->mtx)[2][3];
        } else {
            PSMTXIdentity(*pose->mtx);
            PSMTXRotRad(m2, 'x', 0.017453292f * zako->rot.x);
            PSMTXConcat(*pose->mtx, m2, *pose->mtx);
            PSMTXRotRad(m2, 'y', 0.017453292f * zako->rot.y);
            PSMTXConcat(*pose->mtx, m2, *pose->mtx);
            PSMTXRotRad(m2, 'z', 0.017453292f * zako->rot.z);
            PSMTXConcat(*pose->mtx, m2, *pose->mtx);
            (*pose->mtx)[0][3] = zako->pos.x;
            (*pose->mtx)[1][3] = zako->pos.y;
            (*pose->mtx)[2][3] = zako->pos.z + 0.03f * i;
        }
    }
}

USER_FUNC(zakogun_attack3_1) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        Vec pos = {0.0f, 55.0f, 0.0f};
        work->pos = pos;
        work->radius = 25.0f;
        work->angle = 0.0f;
        work->count = 0;
        for (i = 0; i < 500; i++) {
            zakogun[i].state = 20;
        }
    }
    for (i = 0, n = 0; i < 500; i++) {
        if (zakogun[i].state == 40) {
            n++;
        }
    }
    return n == 500 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_attack3_2) {
    BattleUnit* unit = zakogun_get_ptr();
    s32 sfx = evtGetValue(event, event->args[0]);

    if (isFirstCall) {
        work->count = 0;
        work->state = 0;
    }
    switch (work->state) {
        case 0:
            work->state = 5;
            work->speed = 0.0f;
        case 5:
            work->pos.x += work->speed;
            work->angle -= work->speed;
            work->speed -= 0.06f;
            if (work->targetId != -1) {
                BattleUnit* target = BattleGetUnitPtr(_battleWorkPointer, work->targetId);
                if (work->pos.x <= target->mPosition.x) {
                    unit->work[0] = 1;
                }
            }
            if (work->pos.x <= -200.0f) {
                work->state = 8;
            }
            break;
        case 8:
            work->pos.x += work->speed;
            work->angle -= work->speed;
            work->speed += 5.0f;
            if (work->speed >= 0.0f) {
                work->state = 10;
            }
            break;
        case 10:
            work->pos.x += work->speed;
            work->angle -= work->speed;
            work->speed += 0.06f;
            if (work->pos.x >= -30.0f) {
                work->state = 13;
            }
            break;
        case 13:
            work->pos.x += work->speed;
            work->angle -= work->speed;
            work->speed -= 1.0f;
            if (work->speed <= 0.0f) {
                work->state = 15;
                psndSFXOff(sfx);
                work->timer = 20;
            }
            break;
        case 15:
            work->timer--;
            if (work->timer < 0) {
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_attack3_3) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            zakogun[i].state = 50;
        }
    }
    for (i = 0, n = 0; i < 500; i++) {
        if (zakogun[i].state == 15) {
            n++;
        }
    }
    return n == 500 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_dead3) {
    s32 i;

    for (i = 0; i < 500; i++) {
        zakogun[i].state = 80;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_escape3) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            ZakoGun* zako = &zakogun[i];
            zako->state = 90;
            zakogun_set_pose(zako, 0x16, 10, 1);
        }
    }
    for (i = 0, n = 0; i < 500; i++) {
        if (zakogun[i].state == -1) {
            n++;
        }
    }
    return n == 500 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static EVT_BEGIN(hit_check_event)
    LABEL(10)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
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
    END_SWITCH
    LABEL(90)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(zakogun_set_target_id, LW(3))
        GOTO(10)
        ELSE
        CALL(zakogun_set_target_id, -1)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

