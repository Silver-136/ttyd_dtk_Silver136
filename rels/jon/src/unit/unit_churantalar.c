// Pit of 100 Trials battle unit: the Arantula (`churantalar`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
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
#include "battle/battle_damage.h"

#include "unit/unit_piders.h"
#include "battle/unit/unit_churantalar.h"
static void piders_draw_yarn_sub(CameraId camera, void* param);

static USER_FUNC(piders_draw_yarn);
static USER_FUNC(piders_get_first_pos_attr);
static USER_FUNC(piders_get_num);
static USER_FUNC(piders_get_zyun);
static USER_FUNC(piders_yarn_init);
static void piders_get_next_target(s32* unitIdx, s32* partIdx);
static void piders_get_target(s32* unitIdx, s32* partIdx);
static USER_FUNC(piders_renzoku_attack_set_target);

BattleUnitKind unit_churantalar;
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

#if VERSION <= VERSION_DEMO_JP
static EvtWord dead_event[183];
static EvtWord blow_dead_event[98];
#else
static EvtWord dead_event[189];
static EvtWord blow_dead_event[104];
#endif

static EvtWord battle_end_event[7];
static EvtWord yure_event[76];
static EvtWord move_kuutei_event[83];
static EvtWord move_kuuchu_event[54];
static EvtWord unison_phase_event[120];
static EvtWord attack_event[72];
static EvtWord damage_check_event_sub[157];

#if VERSION <= VERSION_DEMO_JP
static EvtWord piders_attack_common_event1[187];
static EvtWord piders_attack_common_event1_2[128];
#else
static EvtWord piders_attack_common_event1[163];
static EvtWord piders_attack_common_event1_2[109];
#endif

static EvtWord piders_attack_common_event2[47];
static EvtWord piders_attack_common_event3[102];
static EvtWord normal_attack_event[87];
static EvtWord renzoku_attack_event[331];
static EvtWord renzoku_attack_event_sub[55];
static EvtWord renzoku_attack_event_sub_no_last[10];
static EvtWord renzoku_attack_event_sub_last[79];
static EvtWord wait_event[6];

/**
 * Script helper: allocates the yarn work block (a PidersYarn) for the calling Pider
 * and keeps it in the unit's countersetWeapon slot.
 */
static USER_FUNC(piders_yarn_init) {
    s32 id = BattleTransID(event, UNIT_ID_SELF);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    unit->countersetWeapon = (BattleWeapon*)BTL_ALLOC(sizeof(PidersYarn)); // piders keep their yarn work in the counter-set weapon slot
    return EVT_RETURN_DONE;
}

/** Unit data of the Arantula: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_churantalar = {
    UNIT_ARANTULA, // unitType
    "btl_un_churantalar", // name
    16, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    34, // level
    1, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    46, // width
    56, // height
    {0, 56}, // hitOffset
    {0.0f, 28.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {23.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {18.0f, 0.0f, 0.0f}, // bintaHitOffset
    {13.0f, 32.0f, 0.0f}, // kissHitOffset
    {0.0f, 28.0f, 0.0f}, // cutBaseOffset
    46.0f, // cutWidth
    56.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_PAIDA_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    5, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Damage reduction of the Arantula: none, every attack type does full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element response of the Arantula: none, every attack type is treated normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {70, 80, 30, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 70, 70, 80, 30, 100, 70, 100, 100, 30};
#else
static StatusVulnerability regist = {
    70, // sleep
    70, // stop
    30, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    85, // defenseDown
    70, // allergic
    80, // fright
    30, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    30, // ohko
};
#endif

/** Pose table of the Arantula: battle pose types mapped to animation names. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "PAI_N_1"},
    {POSE_TYPE_STOP, "PAI_Y_1"},
    {POSE_TYPE_FREEZE, "PAI_Y_1"},
    {POSE_TYPE_CONFUSE, "PAI_K_1"},
    {POSE_TYPE_POISON, "PAI_X_1"},
    {POSE_TYPE_DIZZY, "PAI_X_1"},
    {POSE_TYPE_NORMAL, "PAI_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "PAI_D_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "PAI_D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PAI_S_1"},
    {POSE_TYPE_DAMAGE, "PAI_D_1"},
    {POSE_TYPE_ATTACK, "PAI_A_1"},
    {POSE_TYPE_WALK, "PAI_W_1"},
    {POSE_TYPE_RUN, "PAI_W_1"},
    {POSE_TYPE_DANGER, "PAI_X_1"},
    {POSE_TYPE_PERIL, "PAI_X_1"},
    {POSE_TYPE_TALK, "PAI_S_1"},
    {POSE_TYPE_ACTION, "PAI_S_1"},
};

/**
 * Data table of the Arantula: event handlers keyed by tag (dead_event, blow_dead_event, battle_end_event,
 * ...).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, blow_dead_event},
    {UNIT_DATA_BATTLE_END_EVENT, battle_end_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/** Body parts of the Arantula: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
    {2, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 5.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/**
 * Draw callback for the Pider's thread: one thin white quad from its lower endpoint up to the ceiling (y
 * = 400), with the alpha from the yarn work block.
 */
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

/**
 * Script helper: args[0] = alpha.
 * Sets the thread's two end points above the Pider's home position (the lower endpoint from work[2] up
 * to y = 400) and queues the draw callback.
 */
static USER_FUNC(piders_draw_yarn) {
    u8 alpha = evtGetValue(event, event->args[0]);
    s32 id = BattleTransID(event, UNIT_ID_SELF);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    PidersYarn* yarn = (PidersYarn*)unit->countersetWeapon;

    // from the pider down to its hanging height, and up to the ceiling
    BtlUnit_GetHomePos(unit, &yarn->x1, &yarn->y1, &yarn->z1);
    yarn->y1 = unit->work[UNIT_WORK_THREAD_END_Y];
    BtlUnit_GetHomePos(unit, &yarn->x0, &yarn->y0, &yarn->z0);
    yarn->y0 = 400.0f;
    yarn->alpha = alpha;
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, piders_draw_yarn_sub, yarn);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: returns in args[0] how many Piders and Arantulas (kinds 0x1B and 0xA4) are still on the field
 * and not knocked out.
 */
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

/**
 * Script helper: args[0] = unit; returns in args[1] its order (zyun) among the living Piders and Arantulas,
 * counting from 1.
 */
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

/**
 * Script helper: returns in args[0] the 0/1 position attribute (work[0]) of the first living Pider or Arantula,
 * flipped when that unit can act, so the group knows which side to take.
 */
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
    attr = unit->work[UNIT_WORK_LOWERED];
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

/**
 * Script helper for the chained attack (renzoku):
 * collects the current weapon's target list and picks three targets at random,
 * writing each pick's unit and part indices into args[0..5] in order.
 */
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

/** Reads the current entry of the weapon target list: unit index in *unitIdx and part index in *partIdx. */
static void piders_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

/**
 * Advances to the next entry of the weapon target list and reads it;
 * returns unit -1 and part 0 at the end of the list.
 */
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

/** Attack data for the Arantula's web throw: 7 damage. */
static BattleWeapon weapon_piders_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Attack data for the Arantula's chained web throws: 4 damage. */
static BattleWeapon weapon_piders_renzoku_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/**
 * Init script of the Arantula: registers its wait, unison-phase, attack,
 * damage and confusion handlers and its entry script, sets unit work 0 (hanging position flag) to 0,
 * unit work 1 (thread alpha) to 255, unit work 2 (thread drop) to 0 and unit work 3 (first-move flag) to 1,
 * allocates the thread work block (piders_yarn_init) and starts yarn_event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_END_Y, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_LOWER, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, 255)
    CALL(piders_yarn_init)
    SPAWN_EVENT(yarn_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Data event 48 of the Arantula, run when the battle opens with Mario's or the partner's First Strike on it:
 * it is set to y 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Background loop of the Arantula, run while it exists:
 * whenever the thread alpha in unit work 1 is below 255 it fades it by 2 per frame (down to 2)
 * and redraws the thread to the ceiling (piders_draw_yarn).
 */
static EVT_BEGIN(yarn_event)
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, LW(0))
    IF_INT_LESS(LW(0), 255)
        IF_INT_GREATER(LW(0), 2)
            SUBTRACT(LW(0), 2)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, LW(0))
        END_IF
    END_IF
    CALL(piders_draw_yarn, LW(0))
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Entrance of the Arantula: it starts at y 180 with its thread dropped by 400 (unit work 2),
 * waits a delay based on its x position and then lowers the thread end to 0 over 40 frames; its y stays 180.
 */
static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 180)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_END_Y, 400)
    DIVIDE(LW(0), 10)
    ADD(LW(0), 80)
    WAIT_FRAMES(LW(0))
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 400, 0, 40)
    LOOP(40)
        CALL(evt_sub_intpl_get_value)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_END_Y, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Damage script of the Arantula: runs the default damage handling on itself. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Defeat script of the Arantula: it spins for 48 frames (SFX_BTL_ENEMY_DIE1_1),
 * then bursts with the "expbom_n64" effect and a star-point effect, the coins and experience are stored,
 * and it tips over for 30 frames before it is hidden and removed from the battle.
 */
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
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kNotTargetable)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitNoncombatant)
    CALL(btlevtcmd_SetMaxMoveCount, LW(10), 0)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitNoHpGauge)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetPos, LW(10), 0, -1000, 0)
#endif
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    RETURN
EVT_END()

/**
 * Defeat script when the Arantula is blown away (SFX_BTL_CLAUD_BREATH3):
 * it spins and is carried to the edge of the stage, its coins and experience are stored,
 * and it is then hidden and removed from the battle.
 */
static EVT_BEGIN(blow_dead_event)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_DAMAGE)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kNotTargetable)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitNoncombatant)
    CALL(btlevtcmd_SetMaxMoveCount, LW(10), 0)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitNoHpGauge)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetPos, LW(10), 0, -1000, 0)
#endif
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    RETURN
EVT_END()

/** Battle-end script: sets the thread alpha (unit work 1) to 254, so yarn_event fades the thread out. */
static EVT_BEGIN(battle_end_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, 254)
    RETURN
EVT_END()

/**
 * Makes the Arantula sway: a damped back and forth rotation about the vertical axis over 92
 * frames, run as a separate event so it does not block the caller.
 */
static EVT_BEGIN(yure_event)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 6, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(22)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, -5, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(20)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 4, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(16)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, -3, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(14)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 2, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, -1, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Moves the Arantula down to the floor: it plays SFX_ENM_PAIDA_MOVE1 (first time only, using unit work 3)
 * or MOVE2, sways, drops to y 10, makes that its home position,
 * clears part 1's attributes 0x600000 and sets unit work 0 to 1.
 */
static EVT_BEGIN(move_kuutei_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_LOWER, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_LOWER, 0)
        BEGIN_SIBLING_EVENT
            LOOP(4)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                WAIT_FRAMES(7)
            END_LOOP
        END_SIBLING_EVENT
    ELSE
        BEGIN_SIBLING_EVENT
            LOOP(4)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                WAIT_FRAMES(7)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL_CHILD(yure_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 1)
    RETURN
EVT_END()

/**
 * Moves the Arantula up into the air: it sways, rises to y 70 with SFX_ENM_PAIDA_MOVE2,
 * makes that its home position, sets part 1's attributes 0x600000 and sets unit work 0 to 0.
 */
static EVT_BEGIN(move_kuuchu_event)
    SPAWN_EVENT(yure_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 70)
    BEGIN_SIBLING_EVENT
        LOOP(4)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 0)
    RETURN
EVT_END()

/**
 * Before the player's phase, if the Arantula can act and is not in STATUS_INSTANT_KILL (KO),
 * it moves up (move_kuuchu_event) or down (move_kuutei_event),
 * counting the living Piders and Arantulas together.
 * The first of them (piders_get_zyun gives 1) switches between the floor and the air every turn.
 * With an odd number
 * of them the others go where the first one ends up;
 * with an even number those at odd places in the order follow the
 * first and those at even places take the other height, so the group alternates.
 */
static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
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
    CALL(piders_get_zyun, UNIT_ID_SELF, LW(14))
    IF_INT_EQUAL(LW(14), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(move_kuuchu_event)
        ELSE
            CALL_CHILD(move_kuutei_event)
        END_IF
        GOTO(99)
    END_IF
    CALL(piders_get_first_pos_attr, LW(13))
    CALL(piders_get_num, LW(15))
    MODULO(LW(15), 2)
    IF_INT_EQUAL(LW(15), 1)
        IF_INT_EQUAL(LW(13), 0)
            CALL_CHILD(move_kuuchu_event)
            GOTO(99)
        ELSE
            CALL_CHILD(move_kuutei_event)
            GOTO(99)
        END_IF
    ELSE
        MODULO(LW(14), 2)
        IF_INT_EQUAL(LW(14), 1)
            IF_INT_EQUAL(LW(13), 0)
                CALL_CHILD(move_kuuchu_event)
                GOTO(99)
            ELSE
                CALL_CHILD(move_kuutei_event)
                GOTO(99)
            END_IF
        ELSE
            IF_INT_EQUAL(LW(13), 0)
                CALL_CHILD(move_kuutei_event)
                GOTO(99)
            ELSE
                CALL_CHILD(move_kuuchu_event)
                GOTO(99)
            END_IF
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Attack script of the Arantula.
 * After clearing its chained-attack work and checking for an item use it draws between the normal web throw (50
 * in 100) and the chained web throw (renzoku, 50 in 100).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET3_HIT, 0)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 50)
    ADD(LW(0), 50)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 50)
        SET_VALUE(LW(9), PTR(&weapon_piders_attack))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_piders_renzoku_attack))
    CALL_CHILD(renzoku_attack_event)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_event_sub)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(0))
    SWITCH(LW(0))
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
        GOTO(99)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, LW(7))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kNoShadow)
    GOTO(99)
    LABEL(91)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kNoShadow)
    IF_INT_EQUAL(LW(10), 1)
        SET_VALUE(LW(0), 5)
        SET_VALUE(LW(1), 8)
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(2))
            IF_INT_EQUAL(LW(3), LW(2))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(2))
                IF_INT_EQUAL(LW(4), LW(2))
                    ADD(LW(0), 6)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), 1)
                    BREAK_LOOP
                END_IF
            END_IF
            ADD(LW(0), 1)
            ADD(LW(1), 1)
        END_LOOP
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(8), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(0))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(0))
    END_IF
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

/**
 * First half of a web throw: the Arantula raises its second arm part (part 2, pose PAI_A_2a) and swings it up
 * and down three times with SFX_ENM_PAIDA_ATTACK1, with a pivot offset scaled to its size,
 * then keeps the final angle in unit work 4.
 */
static EVT_BEGIN(piders_attack_common_event1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAI_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("PAI_A_2a"))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 20, 0)
    ELSE
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 40, 0)
        ELSE
            CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 30, 0)
        END_IF
    END_IF
#else
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(1))
    SET_FLOAT_VALUE(LW(0), FLOAT(30.0f))
    MULTIPLY_FLOAT(LW(0), LW(1))
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, LW(0), 0)
#endif
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 0, 110, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 110, 70, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(15)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 70, 100, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_WINDUP_ANGLE, LW(0))
    RETURN
EVT_END()

/**
 * Shorter wind-up (no script in this file calls it):
 * it takes the arm angle saved in unit work 4 up to 100 in five frames with SFX_ENM_PAIDA_ATTACK1,
 * then saves the angle again.
 */
static EVT_BEGIN(piders_attack_common_event1_2)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), FLOAT(3.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAI_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("PAI_A_2a"))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(0))
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 20, 0)
    ELSE
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 40, 0)
        ELSE
            CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, 30, 0)
        END_IF
    END_IF
#else
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(1))
    SET_FLOAT_VALUE(LW(0), FLOAT(30.0f))
    MULTIPLY_FLOAT(LW(0), LW(1))
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 2, 0, LW(0), 0)
#endif
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_WINDUP_ANGLE, LW(0))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, LW(0), 100, 5)
    LOOP(5)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_WINDUP_ANGLE, LW(0))
    RETURN
EVT_END()

/** Lowers the Arantula's arm back to rest over 15 frames, returns to PAI_S_1 and hides the arm part. */
static EVT_BEGIN(piders_attack_common_event2)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 100, 0, 15)
    LOOP(15)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAI_S_1"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    RETURN
EVT_END()

/**
 * The web ball leaves: plays SFX_ENM_PAIDA_ATTACK2,
 * places part LW(7) (the web, doubled in size) above the Arantula
 * and sends it flying to the target's hit position.
 */
static EVT_BEGIN(piders_attack_common_event3)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(7), STRING("PAI_A_3a"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SWITCH(LW(5))
        CASE_EQUAL(5)
        ADD(LW(1), 20)
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(7), 0, FLOAT(6.0f), FLOAT(0.25f))
        CASE_END
        CASE_EQUAL(6)
        ADD(LW(1), 30)
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(7), 0, FLOAT(4.0f), FLOAT(0.25f))
        CASE_END
        CASE_EQUAL(7)
        ADD(LW(1), 15)
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(7), 0, FLOAT(8.0f), FLOAT(0.25f))
        CASE_END
    END_SWITCH
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(7), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), kNoShadow)
    CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(7), FLOAT(2.0f), FLOAT(2.0f), FLOAT(2.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(7), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * The Arantula's normal attack: the web throw.
 * It skips the throw on a first strike; otherwise it picks a target and winds up (piders_attack_common_event1),
 * has the arm drop in the background and throws the web at the target,
 * then checks the damage and finishes with a short pause.
 */
static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(99)
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
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
    SET_VALUE(LW(7), 3)
    CALL_CHILD(piders_attack_common_event1)
    SPAWN_EVENT(piders_attack_common_event2)
    SET_VALUE(LW(8), 1)
    SET_VALUE(LW(5), 5)
    CALL_CHILD(piders_attack_common_event3)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(damage_check_event_sub)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(500)
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Arantula's chained attack: three web throws in a row at three targets picked at random by
 * piders_renzoku_attack_set_target (target unit ids go in unit work 5 to 7 and part ids in 8 to 10).
 * The first two throws spawn renzoku_attack_event_sub; the third waits for it as a child,
 * and a throw can pick the same target again.
 */
static EVT_BEGIN(renzoku_attack_event)

    // Pick targets; a confused Arantula with nobody to hit gets flustered.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // Choose three targets at random; the same one can come up twice.
    CALL(piders_renzoku_attack_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_ID, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_ID, LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET3_ID, LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_PART, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_PART, LW(3))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET3_PART, LW(5))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_ID, LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_PART, LW(4))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    SET_VALUE(LW(5), 5)
    SET_VALUE(LW(6), 8)
    SET_VALUE(LW(7), 3)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(6), LW(4))

    // First throw: the Arantula winds up its arm and the web ball flies to the first target.
    CALL_CHILD(piders_attack_common_event1)
    CALL_CHILD(piders_attack_common_event3)
    SPAWN_EVENT(renzoku_attack_event_sub)
    WAIT_FRAMES(1)
    ADD(LW(5), 1)
    ADD(LW(6), 1)
    ADD(LW(7), 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(6), LW(4))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))

    // It swings its arm back for the second throw while the first web is resolved.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 100, -200, 20)
    BEGIN_SIBLING_EVENT
        LOOP(20)
            CALL(evt_sub_intpl_get_value)
            MULTIPLY(LW(0), LW(8))
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(15)

    // Second throw: the web ball flies to the second target.
    CALL_CHILD(piders_attack_common_event3)
    SPAWN_EVENT(renzoku_attack_event_sub)
    WAIT_FRAMES(1)
    ADD(LW(5), 1)
    ADD(LW(6), 1)
    ADD(LW(7), 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(6), LW(4))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))

    // The arm swings back again for the third throw.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, -200, 170, 20)
    BEGIN_SIBLING_EVENT
        LOOP(20)
            CALL(evt_sub_intpl_get_value)
            MULTIPLY(LW(0), LW(8))
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(15)

    // Third throw: the last web ball flies out, and this time the script waits for its damage to be resolved.
    CALL_CHILD(piders_attack_common_event3)
    CALL_CHILD(renzoku_attack_event_sub)
    WAIT_FRAMES(1)
    ADD(LW(5), 1)
    ADD(LW(6), 1)
    ADD(LW(7), 1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(8))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(8))
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 170, 0, 15)
    LOOP(15)
        CALL(evt_sub_intpl_get_value)
        MULTIPLY(LW(0), LW(8))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP

    // The arm returns to rest and the Arantula settles down.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAI_S_1"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Damage step of one throw in the chained attack:
 * takes the throw's target from unit work 5 to 10 and looks at the later throws.
 * If a later throw picks the same unit and part it runs renzoku_attack_event_sub_no_last,
 * otherwise renzoku_attack_event_sub_last.
 */
static EVT_BEGIN(renzoku_attack_event_sub)
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(4))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 7)
            CALL_CHILD(renzoku_attack_event_sub_last)
            BREAK_LOOP
        END_IF
        ADD(LW(0), 1)
        ADD(LW(1), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(10))
        IF_INT_EQUAL(LW(3), LW(8))
            IF_INT_EQUAL(LW(4), LW(10))
                CALL_CHILD(renzoku_attack_event_sub_no_last)
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Damage check of a chained throw that is not the last one at its target:
 * it runs damage_check_event_sub with LW(8) = 0 and LW(10) = 1,
 * so the target is flagged as hit and the final-hit argument is 0.
 */
static EVT_BEGIN(renzoku_attack_event_sub_no_last)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(damage_check_event_sub)
    RETURN
EVT_END()

/**
 * Damage check of the last chained throw at a target:
 * if an earlier throw already flagged this target (unit work 11 to
 * 13) it sets the web attributes and deals the damage with the defence result and the 256 argument; otherwise it runs
 * damage_check_event_sub with LW(8) = 1 and LW(10) = 0.
 */
static EVT_BEGIN(renzoku_attack_event_sub_last)
    SET_VALUE(LW(0), 5)
    SET_VALUE(LW(1), 8)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(2))
        IF_INT_EQUAL(LW(3), LW(2))
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(2))
            IF_INT_EQUAL(LW(4), LW(2))
                ADD(LW(0), 6)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(2))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(0), 1)
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_EQUAL(LW(2), 1)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), kNoShadow)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(0))
    ELSE
        SET_VALUE(LW(8), 1)
        SET_VALUE(LW(10), 0)
        CALL_CHILD(damage_check_event_sub)
    END_IF
    RETURN
EVT_END()

/** Wait script: plays the Arantula's idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

