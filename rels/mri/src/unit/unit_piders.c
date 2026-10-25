#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "dolphin/gx.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

typedef struct PidersYarn { f32 x0; f32 y0; f32 z0; f32 x1; f32 y1; f32 z1; u8 alpha; } PidersYarn;
static void piders_draw_yarn_sub(CameraId camera, void* param);

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventUnisonPhase);
static USER_FUNC(piders_draw_yarn);
static USER_FUNC(piders_get_first_pos_attr);
static USER_FUNC(piders_get_num);
static USER_FUNC(piders_get_zyun);
static USER_FUNC(piders_renzoku_attack_set_target);
static USER_FUNC(piders_yarn_init);
static void piders_get_target(s32* unitIdx, s32* partIdx);
static void piders_get_next_target(s32* unitIdx, s32* partIdx);

BattleUnitKind unit_piders;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[5];
static BattleUnitKindPart parts[5];
static BattleWeapon weapon_piders_attack;
static BattleWeapon weapon_piders_renzoku_attack;
static EvtWord init_event[53];
static EvtWord dmg_first_attack_pos[19];
static EvtWord yarn_event[44];
static EvtWord entry_event[48];
static EvtWord damage_event[10];
static EvtWord dead_event[189];
static EvtWord blow_dead_event[104];
static EvtWord battle_end_event[7];
static EvtWord yure_event[76];
static EvtWord move_kuutei_event[83];
static EvtWord move_kuuchu_event[54];
static EvtWord unison_phase_event[120];
static EvtWord attack_event[72];
static EvtWord damage_check_event_sub[157];
static EvtWord piders_attack_common_event1[163];
static EvtWord piders_attack_common_event1_2[109];
static EvtWord piders_attack_common_event2[47];
static EvtWord piders_attack_common_event3[70];
static EvtWord normal_attack_event[87];
static EvtWord renzoku_attack_event[149];
static EvtWord renzoku_attack_event_sub[55];
static EvtWord renzoku_attack_event_sub_no_last[10];
static EvtWord renzoku_attack_event_sub_last[79];
static EvtWord wait_event[7];

static USER_FUNC(piders_yarn_init) {
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    unit->countersetWeapon = (BattleWeapon*)BTL_ALLOC(sizeof(PidersYarn)); // piders keep their yarn work in the counter-set weapon slot
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_piders = {
    27,
    "btl_un_piders",
    5,
    0,
    1,
    1,
    12,
    0,
    2,
    70,
    0,
    70,
    9999,
    46,
    56,
    0,
    56,
    0.0f,
    28.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    23.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    18.0f,
    0.0f,
    0.0f,
    13.0f,
    32.0f,
    0.0f,
    0.0f,
    28.0f,
    0.0f,
    46.0f,
    56.0f,
    0,
    0,
    0,
    2,
    100,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_PAIDA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist,
    5,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {90, 90, 50, 100, 90, 100, 100, 90, 100, 90, 100, 90, 100, 95, 90, 100, 70, 100, 90, 100, 100, 95};

static PoseTableEntry pose_table[] = {
    {1, "PAI_N_1"},
    {2, "PAI_Y_1"},
    {9, "PAI_Y_1"},
    {5, "PAI_K_1"},
    {4, "PAI_X_1"},
    {3, "PAI_X_1"},
    {28, "PAI_S_1"},
    {29, "PAI_D_1"},
    {30, "PAI_D_1"},
    {31, "PAI_S_1"},
    {39, "PAI_D_1"},
    {50, "PAI_A_1"},
    {40, "PAI_W_1"},
    {42, "PAI_W_1"},
    {56, "PAI_X_1"},
    {57, "PAI_X_1"},
    {65, "PAI_S_1"},
    {69, "PAI_S_1"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {58, blow_dead_event},
    {63, battle_end_event},
    {48, dmg_first_attack_pos},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table},
    {2, "btl_un_piders", "c_paid", 0.0f, 0.0f, 5.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {3, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {4, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {5, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
};

static void piders_draw_yarn_sub(CameraId camera, void* param) {
    PidersYarn* yarn = param;
    CameraEntry* cam = camGetPtr(CAMERA_3D);

    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, yarn->alpha});
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(yarn->x0 - 1.0f, yarn->y0, yarn->z0 - 5.0f);
    GXPosition3f32(1.0f + yarn->x0, yarn->y0, yarn->z0 - 5.0f);
    GXPosition3f32(1.0f + yarn->x1, yarn->y1, yarn->z1 - 5.0f);
    GXPosition3f32(yarn->x1 - 1.0f, yarn->y1, yarn->z1 - 5.0f);
    GXEnd();
}

static USER_FUNC(piders_draw_yarn) {
    u8 alpha = evtGetValue(event, event->args[0]);
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    PidersYarn* yarn = (PidersYarn*)unit->countersetWeapon;

    // from the pider down to its hanging height, and up to the ceiling
    BtlUnit_GetHomePos(unit, &yarn->x1, &yarn->y1, &yarn->z1);
    yarn->y1 = unit->work[2];
    BtlUnit_GetHomePos(unit, &yarn->x0, &yarn->y0, &yarn->z0);
    yarn->y0 = 400.0f;
    yarn->alpha = alpha;
    dispEntry(CAMERA_3D, 0.0f, 2, piders_draw_yarn_sub, yarn);
    return EVT_RETURN_DONE;
}

static USER_FUNC(piders_get_num) {
    BattleUnit* unit;
    s32 i;
    EvtWord* args = event->args;
    s32 num = 0;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_PIDER || unit->currentKind == UNIT_ARANTULA) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            num++;
        }
    }
    evtSetValue(event, args[0], num);
    return EVT_RETURN_DONE;
}

static USER_FUNC(piders_get_zyun) {
    s32 id;
    s32 i;
    EvtWord* args = event->args;
    s32 order = 0;
    BattleWork* wp;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    wp = _battleWorkPointer;
    id = BattleTransID(event, id);
    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_PIDER || unit->currentKind == UNIT_ARANTULA)) {
            if (!BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                order++;
            }
            if (unit->unitId == id) {
                break;
            }
        }
    }
    evtSetValue(event, args[1], order);
    return EVT_RETURN_DONE;
}

static USER_FUNC(piders_get_first_pos_attr) {
    s32 i;
    EvtWord* args = event->args;
    s32 attr;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    // find the first living pider
    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_PIDER || unit->currentKind == UNIT_ARANTULA) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            break;
        }
    }
    attr = unit->work[0];
    if (BtlUnit_CanActStatus(unit)) {
        if (attr == 0) {
            attr = 1;
        } else {
            attr = 0;
        }
    }
    evtSetValue(event, args[0], attr);
    return EVT_RETURN_DONE;
}

static USER_FUNC(piders_renzoku_attack_set_target) {
    EvtWord* args = event->args;
    s32 unitIdx[20];
    s32 partIdx[20];
    s32 i;
    s32 num;

    i = 0;
    _battleWorkPointer->weaponTargets.currentTarget = 0;
    piders_get_target(&unitIdx[0], &partIdx[0]);
    while (TRUE) {
        if (unitIdx[i] == -1) {
            num = i;
            break;
        }
        i++;
        piders_get_next_target(&unitIdx[i], &partIdx[i]);
    }
    for (i = 0; i < 3; i++) {
        s32 r = irand(num);
        evtSetValue(event, *args++, unitIdx[r]);
        evtSetValue(event, *args++, partIdx[r]);
    }
    return EVT_RETURN_DONE;
}

static void piders_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

static void piders_get_next_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;

    wt->currentTarget++;
    if (wt->currentTarget < wt->numTargets) {
        BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];
        *unitIdx = target->unitIdx;
        *partIdx = target->partIdx;
    } else {
        *unitIdx = -1;
        *partIdx = 0;
    }
}

static BattleWeapon weapon_piders_attack = {
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
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

static BattleWeapon weapon_piders_renzoku_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, -2, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 255)
    CALL(piders_yarn_init)
    SPAWN_EVENT(PTR(yarn_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(yarn_event)
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_LESS(LW(0), 255)
        IF_INT_GREATER(LW(0), 2)
            SUBTRACT(LW(0), 2)
            CALL(btlevtcmd_SetUnitWork, -2, 1, LW(0))
        END_IF
    END_IF
    CALL(piders_draw_yarn, LW(0))
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 180)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 400)
    DIVIDE(LW(0), 10)
    ADD(LW(0), 80)
    WAIT_FRAMES(LW(0))
    CALL(evt_sub_intpl_init, 4, 400, 0, 40)
    LOOP(40)
        CALL(evt_sub_intpl_get_value)
        CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
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
    CALL(evt_eff64, STRING(""), STRING("expbom_n64"), LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0, 0)
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
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 33554432)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 65536)
    CALL(btlevtcmd_OnAttribute, LW(10), 268435456)
    CALL(btlevtcmd_SetMaxMoveCount, LW(10), 0)
    CALL(btlevtcmd_OnAttribute, LW(10), 33554432)
    CALL(btlevtcmd_SetPos, LW(10), 0, -1000, 0)
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    RETURN
EVT_END()

static EVT_BEGIN(blow_dead_event)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 39)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, -2, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 65536)
    CALL(btlevtcmd_OnAttribute, LW(10), 268435456)
    CALL(btlevtcmd_SetMaxMoveCount, LW(10), 0)
    CALL(btlevtcmd_OnAttribute, LW(10), 33554432)
    CALL(btlevtcmd_SetPos, LW(10), 0, -1000, 0)
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    RETURN
EVT_END()

static EVT_BEGIN(battle_end_event)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 254)
    RETURN
EVT_END()

static EVT_BEGIN(yure_event)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddRotate, -2, 0, 6, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(22)
            CALL(btlevtcmd_AddRotate, -2, 0, -5, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(20)
            CALL(btlevtcmd_AddRotate, -2, 0, 4, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(16)
            CALL(btlevtcmd_AddRotate, -2, 0, -3, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(14)
            CALL(btlevtcmd_AddRotate, -2, 0, 2, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(btlevtcmd_AddRotate, -2, 0, -1, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(move_kuutei_event)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
        BEGIN_SIBLING_EVENT
            LOOP(4)
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                WAIT_FRAMES(7)
            END_LOOP
        END_SIBLING_EVENT
    ELSE
        BEGIN_SIBLING_EVENT
            LOOP(4)
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                WAIT_FRAMES(7)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL_CHILD(yure_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, -10, 4, 0, -1)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    RETURN
EVT_END()

static EVT_BEGIN(move_kuuchu_event)
    SPAWN_EVENT(yure_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 70)
    BEGIN_SIBLING_EVENT
        LOOP(4)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, 10, 4, 0, -1)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(piders_get_zyun, -2, LW(14))
    IF_INT_EQUAL(LW(14), 1)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(move_kuuchu_event))
            ELSE
            CALL_CHILD(PTR(move_kuutei_event))
        END_IF
        GOTO(99)
    END_IF
    CALL(piders_get_first_pos_attr, LW(13))
    CALL(piders_get_num, LW(15))
    MODULO(LW(15), 2)
    IF_INT_EQUAL(LW(15), 1)
        IF_INT_EQUAL(LW(13), 0)
            CALL_CHILD(PTR(move_kuuchu_event))
            GOTO(99)
            ELSE
            CALL_CHILD(PTR(move_kuutei_event))
            GOTO(99)
        END_IF
        ELSE
        MODULO(LW(14), 2)
        IF_INT_EQUAL(LW(14), 1)
            IF_INT_EQUAL(LW(13), 0)
                CALL_CHILD(PTR(move_kuuchu_event))
                GOTO(99)
                ELSE
                CALL_CHILD(PTR(move_kuutei_event))
                GOTO(99)
            END_IF
            ELSE
            IF_INT_EQUAL(LW(13), 0)
                CALL_CHILD(PTR(move_kuutei_event))
                GOTO(99)
                ELSE
                CALL_CHILD(PTR(move_kuuchu_event))
                GOTO(99)
            END_IF
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, -2, 11, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 12, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 13, 0)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 50)
    ADD(LW(0), 50)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 50)
        SET_VALUE(LW(9), PTR(&weapon_piders_attack))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_piders_renzoku_attack))
    CALL_CHILD(PTR(renzoku_attack_event))
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_event_sub)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(0))
    SWITCH(LW(0))
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
        GOTO(99)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_JumpPartsContinue, -2, LW(7))
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 33554432)
    GOTO(99)
    LABEL(91)
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 33554432)
    IF_INT_EQUAL(LW(10), 1)
        SET_VALUE(LW(0), 5)
        SET_VALUE(LW(1), 8)
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(2))
            IF_INT_EQUAL(LW(3), LW(2))
                CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(2))
                IF_INT_EQUAL(LW(4), LW(2))
                    ADD(LW(0), 6)
                    CALL(btlevtcmd_SetUnitWork, -2, LW(0), 1)
                    BREAK_LOOP
                END_IF
            END_IF
            ADD(LW(0), 1)
            ADD(LW(1), 1)
        END_LOOP
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(8), 1)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(0))
        ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(0))
    END_IF
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(piders_attack_common_event1)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(8))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(2), FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PAI_A_1"))
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("PAI_A_2a"))
    CALL(btlevtcmd_SetPartsPos, -2, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden)
    CALL(btlevtcmd_GetStatusMg, -2, LW(1))
    SET_FLOAT_VALUE(LW(0), FLOAT(30.0f))
    MULTIPLY_FLOAT(LW(0), LW(1))
    CALL(btlevtcmd_SetPartsRotateOffset, -2, 2, 0, LW(0), 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 4, 0, 110, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 4, 110, 70, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 4, 70, 100, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(piders_attack_common_event1_2)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(8))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(2), FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PAI_A_1"))
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("PAI_A_2a"))
    CALL(btlevtcmd_SetPartsPos, -2, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden)
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(0))
    CALL(btlevtcmd_GetStatusMg, -2, LW(1))
    SET_FLOAT_VALUE(LW(0), FLOAT(30.0f))
    MULTIPLY_FLOAT(LW(0), LW(1))
    CALL(btlevtcmd_SetPartsRotateOffset, -2, 2, 0, LW(0), 0)
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 4, LW(0), 100, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(piders_attack_common_event2)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(8))
    CALL(evt_sub_intpl_init, 4, 100, 0, 15)
    LOOP(15)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PAI_S_1"))
    CALL(btlevtcmd_OnPartsAttribute, -2, 2, kHidden)
    RETURN
EVT_END()

static EVT_BEGIN(piders_attack_common_event3)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAIDA_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, LW(7), STRING("PAI_A_3a"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(btlevtcmd_JumpPartsSetting, -2, LW(7), 0, FLOAT(6.0f), FLOAT(0.25f))
    CALL(btlevtcmd_SetPartsPos, -2, LW(7), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, -2, LW(7), 16777216)
    CALL(btlevtcmd_OffPartsAttribute, -2, LW(7), 33554432)
    CALL(btlevtcmd_SetPartsScale, -2, LW(7), FLOAT(2.0f), FLOAT(2.0f), FLOAT(2.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, -2, LW(7), LW(0), LW(1), LW(2), 0, -1)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(99)
        ELSE
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
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
    SET_VALUE(LW(7), 3)
    CALL_CHILD(PTR(piders_attack_common_event1))
    SPAWN_EVENT(PTR(piders_attack_common_event2))
    SET_VALUE(LW(8), 1)
    SET_VALUE(LW(5), 5)
    CALL_CHILD(PTR(piders_attack_common_event3))
    SET_VALUE(LW(10), 0)
    CALL_CHILD(PTR(damage_check_event_sub))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(500)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event)
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
    CALL(piders_renzoku_attack_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 6, LW(2))
    CALL(btlevtcmd_SetUnitWork, -2, 7, LW(4))
    CALL(btlevtcmd_SetUnitWork, -2, 8, LW(1))
    CALL(btlevtcmd_SetUnitWork, -2, 9, LW(3))
    CALL(btlevtcmd_SetUnitWork, -2, 10, LW(5))
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(3))
    CALL(btlevtcmd_GetUnitWork, -2, 8, LW(4))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    SET_VALUE(LW(5), 5)
    SET_VALUE(LW(6), 8)
    SET_VALUE(LW(7), 3)
    CALL_CHILD(PTR(piders_attack_common_event1))
    LOOP(3)
        CALL(btlevtcmd_GetUnitWork, -2, LW(5), LW(3))
        CALL(btlevtcmd_GetUnitWork, -2, LW(6), LW(4))
        CALL_CHILD(PTR(piders_attack_common_event3))
        SPAWN_EVENT(PTR(renzoku_attack_event_sub))
        ADD(LW(5), 1)
        ADD(LW(6), 1)
        ADD(LW(7), 1)
        WAIT_FRAMES(30)
    END_LOOP
    SPAWN_EVENT(PTR(piders_attack_common_event2))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event_sub)
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(3))
    CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(4))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 7)
            CALL_CHILD(PTR(renzoku_attack_event_sub_last))
            BREAK_LOOP
        END_IF
        ADD(LW(0), 1)
        ADD(LW(1), 1)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(8))
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(10))
        IF_INT_EQUAL(LW(3), LW(8))
            IF_INT_EQUAL(LW(4), LW(10))
                CALL_CHILD(PTR(renzoku_attack_event_sub_no_last))
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event_sub_no_last)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(PTR(damage_check_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event_sub_last)
    SET_VALUE(LW(0), 5)
    SET_VALUE(LW(1), 8)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(2))
        IF_INT_EQUAL(LW(3), LW(2))
            CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(2))
            IF_INT_EQUAL(LW(4), LW(2))
                ADD(LW(0), 6)
                CALL(btlevtcmd_GetUnitWork, -2, LW(0), LW(2))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(0), 1)
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_EQUAL(LW(2), 1)
        CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 16777216)
        CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 33554432)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(0))
        ELSE
        SET_VALUE(LW(8), 1)
        SET_VALUE(LW(10), 0)
        CALL_CHILD(PTR(damage_check_event_sub))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END_PAD()

