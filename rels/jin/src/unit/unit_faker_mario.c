#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

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

BOOL BattlePartyAnimeLoad(s32 id);
typedef struct FakerStatus { s16 data[15]; } FakerStatus;
#ifdef VERSION_G8ME01
typedef struct FakerParam { f32 size; FakerStatus status; s32 statusFlags; u16 commandDisabled[2]; s16 currentHp; s16 maxHp; s16 currentFp; s16 maxFp; } FakerParam;
#else
typedef struct FakerParam { f32 size; FakerStatus status; s32 statusFlags; s16 currentHp; s16 maxHp; s16 currentFp; s16 maxFp; } FakerParam;
#endif

static USER_FUNC(_anime_load);
static USER_FUNC(_anime_load_wait);
USER_FUNC(_gullible_party_info_init);
USER_FUNC(_load_param);
USER_FUNC(_save_param);
#ifdef VERSION_G8ME01
USER_FUNC(faker_mario_sync_max_moves);
#endif
USER_FUNC(btlevtcmd_AddScale);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_CheckDataOfDataTable);
USER_FUNC(btlevtcmd_DrawLots);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_FaceDirectionSub);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetMaxHp);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_JumpPosition);
USER_FUNC(btlevtcmd_KillUnit);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_ResetFaceDirection);
USER_FUNC(btlevtcmd_RunDataEventChild);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetFallAccel);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(btlevtcmd_SetMoveSpeed);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetRunSound);
USER_FUNC(btlevtcmd_SetScale);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SetWalkSound);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
USER_FUNC(btlevtcmd_SpawnUnit);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_TransStageFloorPosition);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveto);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_sub_random);
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

static USER_FUNC(_anime_load_wait) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 anime;

    switch (unit->currentKind) {
        case 0x50:
            anime = 0xE0;
            break;
        case 0x51:
            anime = 0xE1;
            break;
        case 0x53:
            anime = 0xE3;
            break;
        case 0x52:
        default:
            anime = 0xE2;
            break;
    }
    if (BattlePartyAnimeLoad(anime)) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(_anime_load) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 anime;

    switch (unit->currentKind) {
        case 0x50:
            anime = 0xE0;
            break;
        case 0x51:
            anime = 0xE1;
            break;
        case 0x53:
            anime = 0xE3;
            break;
        case 0x52:
        default:
            anime = 0xE2;
            break;
    }
    BattlePartyAnimeLoad(anime);
    return EVT_RETURN_DONE;
}

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
        case 0x50:
            p = &params[0];
            break;
        case 0x51:
            p = &params[1];
            break;
        case 0x53:
            p = &params[2];
            break;
        case 0x52:
            p = &params[3];
            break;
    }
    p->size = party->mSizeMultiplier;
    p->statusFlags = party->mStatusFlags;
#ifdef VERSION_G8ME01
    p->commandDisabled[0] = party->usCommandDisabled[0];
    p->commandDisabled[1] = party->usCommandDisabled[1];
#endif
    p->status = *(FakerStatus*)&party->mSleepTurns;
    p->maxHp = party->maxHp;
    p->currentHp = party->currentHp;
    p->maxFp = party->maxFp;
    p->currentFp = party->currentFp;
    return EVT_RETURN_DONE;
}

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
        case 0x50:
            p = &params[0];
            break;
        case 0x51:
            p = &params[1];
            break;
        case 0x53:
            p = &params[2];
            break;
        case 0x52:
            p = &params[3];
            break;
    }
    party->mSizeMultiplier = p->size;
    party->mStatusFlags = p->statusFlags;
#ifdef VERSION_G8ME01
    party->usCommandDisabled[0] = p->commandDisabled[0];
    party->usCommandDisabled[1] = p->commandDisabled[1];
#endif
    *(FakerStatus*)&party->mSleepTurns = p->status;
    party->maxHp = p->maxHp;
    party->currentHp = p->currentHp;
    party->maxFp = p->maxFp;
    party->currentFp = p->currentFp;
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_faker_mario = {
    79,
    "btl_un_rampell",
    40,
    0,
    1,
    1,
    54,
    0,
    0,
    100,
    0,
    70,
    7,
    24,
    40,
    12,
    40,
    0.0f,
    20.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    12.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    32.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    24.0f,
    40.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_MARIO_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist_fmario,
    1,
    parts_fmario,
    init_event_fmario,
    data_table_fmario,
};

static BattleUnitDefense defence_fmario = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr_fmario = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist_fmario = {50, 50, 70, 50, 50, 50, 100, 40, 100, 70, 100, 70, 100, 90, 50, 0, 0, 100, 50, 100, 100, 3};

static PoseTableEntry pose_table_fmario_stay[] = {
    {1, "EM_N_1"},
    {2, "EM_Y_2"},
    {9, "EM_Y_2"},
    {5, "EM_K_1"},
    {4, "EM_K_1"},
    {3, "EM_K_1"},
    {28, "EM_S_1"},
    {39, "EM_D_1"},
    {50, "EM_A_1"},
    {42, "EM_R_1"},
    {40, "EM_W_1"},
    {56, "EM_D_3"},
    {57, "EM_D_3"},
    {65, "EM_T_1"},
    {69, "EM_S_1"},
};

static BattleDataEntry data_table_fmario[] = {
    {49, dead_event_fmario},
    {0, NULL},
};

static BattleUnitKindPart parts_fmario[] = {
    {1, "btl_un_rampell", "c_mario", 0.0f, 0.0f, 0.0f, 3.0f, 27.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_fmario, &defence_attr_fmario, 0x9, 0, pose_table_fmario_stay},
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x15807, // special
    0xC, // counterResistFlags
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x10807, // special
    0x20C, // counterResistFlags
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x15807, // special
    0xC, // counterResistFlags
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1001260, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x10807, // special
    0x20C, // counterResistFlags
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

BattleUnitSetup entry_gullible_christine = {
    &unit_gullible_christine, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_gullible_nokotarou = {
    &unit_gullible_nokotarou, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_gullible_clauda = {
    &unit_gullible_clauda, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_gullible_yoshi = {
    &unit_gullible_yoshi, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(mario_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, -110, 65, 332, -110, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(enemy_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 115, 65, 332, 115, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(fmario_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 90, 65, 332, 90, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(gparty_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, 130, 65, 332, 130, 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(init_event_fmario)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event_fmario))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event_fmario))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event_fmario))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event_fmario))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(fmario_entry_event))
    CALL(btlevtcmd_GetUnitId, -2, GW(16))
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
    SET_VALUE(GW(18), 0)
    SET_VALUE(LW(12), PTR(&entry_gullible_christine))
    CALL(btlevtcmd_SpawnUnit, LW(3), LW(12), 0)
    SET_VALUE(GW(17), LW(3))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 40)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(_load_param, -2, LW(3))
    SET_VALUE(GW(20), 0)
    SET_VALUE(GW(19), 0)
    CALL(btlevtcmd_GetMaxHp, -2, LW(0))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 66)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, -2, 1, LW(1))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 50)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(1))
    SET_VALUE(LW(1), LW(0))
    MULTIPLY(LW(1), 25)
    DIVIDE(LW(1), 100)
    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(1))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    SET_VALUE(GW(21), 0)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_NM_MOVE1L"), STRING("SFX_BOSS_NM_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_BOSS_NM_JUMP1"), STRING("SFX_BOSS_NM_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(fmario_entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    WAIT_MS(2000)
    CALL_CHILD(PTR(fmario_zoom))
    CALL(evt_msg_print, 2, STRING("stg4_jin_22"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    RETURN
EVT_END()

EVT_BEGIN(dead_event_fmario_down)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_S_1"))
    WAIT_MS(300)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("EM_T_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("EM_S_1"))
    CALL(evt_msg_print, 2, STRING("stg4_jin_26_05"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_K_1"))
    WAIT_FRAMES(152)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_NM_LAND_DAMAGE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_D_2"))
    WAIT_MS(1500)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_fmario)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_W_1"))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
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
        CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
        CALL(btlevtcmd_SetUnitWork, LW(3), 1, LW(1))
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, LW(2))
        WAIT_FRAMES(1)
    END_IF
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    SWITCH(GW(20))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(fmario_zoom))
        CALL_CHILD(PTR(dead_event_fmario_down))
        WAIT_MS(300)
        CALL_CHILD(PTR(gparty_zoom))
        SWITCH(GW(18))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 2, STRING("stg4_jin_26_05_kur"), 0, GW(17))
            CASE_EQUAL(1)
            CALL(evt_msg_print, 2, STRING("stg4_jin_26_05_nok"), 0, GW(17))
            CASE_EQUAL(2)
            CALL(evt_msg_print, 2, STRING("stg4_jin_26_05_win"), 0, GW(17))
            CASE_EQUAL(3)
            CALL(evt_msg_print, 2, STRING("stg4_jin_26_05_yos"), 0, GW(17))
        END_SWITCH
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL_CHILD(PTR(enemy_zoom))
        CALL_CHILD(PTR(dead_event_fmario_down))
        WAIT_MS(200)
        CASE_EQUAL(2)
        CALL_CHILD(PTR(fmario_zoom))
        CALL_CHILD(PTR(dead_event_fmario_down))
        WAIT_MS(200)
    END_SWITCH
    SET_VALUE(GW(19), 2)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_fmario)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(19), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(phase_event_fmario)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(evt_msg_print, 0, STRING("stg4_jin_23"), 0, 0)
            WAIT_MS(200)
            SET_VALUE(GW(21), 1)
            CALL(vivian_comeback)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entryunit_party[4]), 0)
            CALL(btlevtcmd_SetHomePos, LW(3), FLOAT(-140.0f), FLOAT(0.0f), FLOAT(5.0f))
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
            CALL(btlevtcmd_SetUnitWork, LW(3), 1, LW(1))
            CALL(btlevtcmd_SetUnitWork, LW(3), 2, LW(2))
            CALL(faker_mario_sync_max_moves, LW(3))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetMarioId, LW(3), LW(5))
            CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
            CALL(btlevtcmd_StopWaitEvent, LW(3))
            CALL(btlevtcmd_StopWaitEvent, LW(5))
            WAIT_MS(700)
            SPAWN_EVENT(mario_zoom)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_SetPos, LW(3), -140, -40, 5)
            CALL(btlevtcmd_DivePosition, LW(3), -140, 0, 5, 60, 0, 0, 4, 0)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_Z_1"))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 0, 0, 180, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -3, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(evt_msg_print, 2, STRING("stg4_jin_24"), 0, LW(3))
            WAIT_MS(200)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_FIND1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, LW(5), LW(7), LW(8), LW(10))
            CALL(btlevtcmd_JumpSetting, LW(5), 12, FLOAT(1.0f), FLOAT(1.1992188f))
            CALL(btlevtcmd_JumpPosition, LW(5), LW(7), LW(8), LW(10), 0, -1)
            WAIT_MS(1000)
            CALL(evt_msg_print, 2, STRING("stg4_jin_25"), 0, LW(3))
            WAIT_MS(1000)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 0, 180, 0, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -3, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            WAIT_MS(200)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_I_Y"))
            WAIT_MS(1000)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(GW(20), 0)
            CALL_CHILD(party_change_event)
        END_IF
        CALL(btlevtcmd_GetHp, -2, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(2))
        IF_INT_NOT_EQUAL(GW(20), 0)
            IF_INT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
                SET_VALUE(LW(2), 1)
            END_IF
        END_IF
        SWITCH(LW(2))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetUnitWork, -2, 1, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(enemy_zoom)
                SWITCH(GW(18))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_kur"), 0, GW(17))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_nok"), 0, GW(17))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_win"), 0, GW(17))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_yos"), 0, GW(17))
                END_SWITCH
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 2)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, 2, STRING("stg4_jin_26_01"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 3)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, 2, STRING("stg4_jin_26_02"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
        END_SWITCH
    END_IF
    RETURN
EVT_END()
#else
EVT_BEGIN(phase_event_fmario)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(evt_msg_print, 0, STRING("stg4_jin_23"), 0, 0)
            WAIT_MS(200)
            SET_VALUE(GW(21), 1)
            CALL(vivian_comeback)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entryunit_party[4]), 0)
            CALL(btlevtcmd_SetHomePos, LW(3), FLOAT(-140.0f), FLOAT(0.0f), FLOAT(5.0f))
            CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
            CALL(btlevtcmd_SetUnitWork, LW(3), 1, LW(1))
            CALL(btlevtcmd_SetUnitWork, LW(3), 2, LW(2))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetMarioId, LW(3), LW(5))
            CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
            CALL(btlevtcmd_StopWaitEvent, LW(3))
            CALL(btlevtcmd_StopWaitEvent, LW(5))
            WAIT_MS(700)
            SPAWN_EVENT(mario_zoom)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_SetPos, LW(3), -140, -40, 5)
            CALL(btlevtcmd_DivePosition, LW(3), -140, 0, 5, 60, 0, 0, 4, 0)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_Z_1"))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 0, 0, 180, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -3, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(evt_msg_print, 2, STRING("stg4_jin_24"), 0, LW(3))
            WAIT_MS(200)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_FIND1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, LW(5), LW(7), LW(8), LW(10))
            CALL(btlevtcmd_JumpSetting, LW(5), 12, FLOAT(1.0f), FLOAT(1.1992188f))
            CALL(btlevtcmd_JumpPosition, LW(5), LW(7), LW(8), LW(10), 0, -1)
            WAIT_MS(1000)
            CALL(evt_msg_print, 2, STRING("stg4_jin_25"), 0, LW(3))
            WAIT_MS(1000)
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init, 0, 180, 0, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, -3, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            WAIT_MS(200)
            CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("M_I_Y"))
            WAIT_MS(1000)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(GW(20), 0)
            CALL_CHILD(party_change_event)
        END_IF
        CALL(btlevtcmd_GetHp, -2, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(2))
        IF_INT_NOT_EQUAL(GW(20), 0)
            IF_INT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
                SET_VALUE(LW(2), 1)
            END_IF
        END_IF
        SWITCH(LW(2))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetUnitWork, -2, 1, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(enemy_zoom)
                SWITCH(GW(18))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_kur"), 0, GW(17))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_nok"), 0, GW(17))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_win"), 0, GW(17))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 2, STRING("stg4_jin_26_yos"), 0, GW(17))
                END_SWITCH
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 2)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, 2, STRING("stg4_jin_26_01"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
            IF_INT_LESS(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, -2, 0, 3)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(fmario_zoom)
                CALL(evt_msg_print, 2, STRING("stg4_jin_26_02"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_IF
        END_SWITCH
    END_IF
    RETURN
EVT_END()
#endif

static EVT_BEGIN(attack_event_fmario)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 4, 50, 50, 20, 20)
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
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(jump_attack_voice)
    CALL(evt_sub_random, 2, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VOICE_NM_AC1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VOICE_NM_AC1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CASE_EQUAL(2)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VOICE_NM_AC1_5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_NormalJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 36, 0, -1)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_KururinJump)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL_CHILD(jump_attack_voice)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 36, 0, -1)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_D_2"))
    WAIT_FRAMES(40)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
        CALL_CHILD(jump_attack_voice)
        CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), 36, 0, -1)
    END_SIBLING_EVENT
    WAIT_FRAMES(15)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
    MULTIPLY(LW(15), -30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_Z_2"))
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_JUMP_KURURIN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(12)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_NormalHammer)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1A"))
    WAIT_MS(66)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_HAMMER_FURU2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(233)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1B"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1C"))
    WAIT_MS(50)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_HAMMER_DON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(100)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_S_1"))
    WAIT_MS(200)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(marioAttackEvent_KaitenHammer)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 24)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1A"))
    WAIT_MS(66)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_HAMMER_FURU2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(233)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1B"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_A_1C"))
    WAIT_MS(50)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_NM_HAMMER_DON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(100)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_S_1"))
    WAIT_MS(200)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_W_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

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
    CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(3), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(3), FLOAT(-0.049804688f), FLOAT(-0.049804688f), FLOAT(-0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_IN1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(5), LW(6), LW(7), 20, 0)
    CALL(_anime_load_wait, LW(9))
    CALL(_save_param, -2, LW(3))
    CALL(_load_param, -2, LW(9))
    CALL(btlevtcmd_SetPos, LW(9), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_SetFallAccel, LW(9), FLOAT(0.69921875f))
    CALL(btlevtcmd_CheckDataOfDataTable, LW(9), LW(0), 4)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(9), 4)
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
    CALL(btlevtcmd_JumpPosition, LW(9), LW(0), LW(1), LW(2), 20, 0)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_SortPhaseMoveTable)
    RETURN
EVT_END()

USER_FUNC(_gullible_party_info_init) {
    PouchData* pouch = pouchGetPtr();
    s32 id = BattleTransID(event, -2);
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
#ifdef VERSION_G8ME01
        p->commandDisabled[0] = 0;
        p->commandDisabled[1] = 0;
#endif
    }
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
// US only: restore the moves of the unit for this turn.
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

static USER_FUNC(vivian_comeback) {
    pouchGetPtr()->partyData[6].flags &= ~2;
    return 1;
}

static EVT_BEGIN(wait_event_fmario)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

