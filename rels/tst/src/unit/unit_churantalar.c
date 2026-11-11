// Battle unit Arantula (`churantalar`): tst.rel's copy of rels/jon/src/unit/unit_churantalar.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "tst_battle.h"
#include "evt/evt_cmd.h"

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
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "unit/unit_piders.h"
#include "evt/evt_sub.h"
#include "battle/battle_damage.h"
#include "battle/battle_camera.h"
#include "battle/unit/unit_churantalar.h"

static void piders_get_next_target(s32* unitIdx, s32* partIdx);
static void piders_get_target(s32* unitIdx, s32* partIdx);
static void piders_draw_yarn_sub(CameraId camera, void* param);

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
static EvtWord unison_phase_event[120];
static EvtWord attack_event[72];
static EvtWord normal_attack_event[87];
static EvtWord renzoku_attack_event[331];
static EvtWord renzoku_attack_event_sub[55];
static EvtWord renzoku_attack_event_sub_no_last[10];
static EvtWord renzoku_attack_event_sub_last[79];
static EvtWord wait_event[6];

/** Script helper: allocates the Pider's PidersYarn buffer and keeps it in the unit's countersetWeapon slot. */
static USER_FUNC(piders_yarn_init) {
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    TST_BATTLE_UNIT(unit)->countersetWeapon = (BattleWeapon*)BTL_ALLOC(sizeof(PidersYarn)); // piders keep their yarn work in the counter-set weapon slot
    return EVT_RETURN_DONE;
}

/**
 * Unit kind of Arantula: 16 max HP, level 38 in the early demos and 34 in retail. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_churantalar = {
    UNIT_ARANTULA,
    "btl_un_churantalar",
    16,
    0,
    1,
    1,
#if VERSION <= VERSION_DEMO_JP
    38,
    0,
    1,
    70,
    4,
#else
    34,
    1,
    3,
    70,
    1,
#endif
    50,
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
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_PAIDA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kFloating,
    &regist,
    5,
    parts,
    init_event,
    data_table,
};

/**
 * Defence of Arantula per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Arantula: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability of Arantula (the early demos), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 80, Dizzy 70, Confuse 70, Freeze 70, Tiny 70, Attack Down 70, Defense Down 70,
 * Allergic 70, Fright 80, Gale Force 30, Slow 70 and instant KO 30.
 */
static StatusVulnerability regist = {60, 80, 70, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 70, 70, 80, 30, 100, 70, 100, 100, 30};
#else

/**
 * Status vulnerability of Arantula (the final versions), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 70, Stop 70, Dizzy 30, Confuse 70, Freeze 70, Tiny 80, Attack Down 80, Defense Down 85,
 * Allergic 70, Fright 80, Gale Force 30, Slow 70 and instant KO 30.
 */
static StatusVulnerability regist = {70, 70, 30, 100, 70, 100, 100, 70, 100, 80, 100, 80, 100, 85, 70, 80, 30, 100, 70, 100, 100, 30};
#endif

/**
 * Pose table of Arantula: maps its battle pose types (stay, walk, hurt, attack, talk and so on) to
 * animation names, 18 entries such as PAI_N_1, PAI_Y_1, PAI_K_1, PAI_X_1.
 */
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
 * Event table of Arantula: maps event tags to the scripts that react to them (dead_event,
 * blow_dead_event, battle_end_event and dmg_first_attack_pos); tag 0 ends it. Tag 48 is a First Strike.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, blow_dead_event},
    {UNIT_DATA_BATTLE_END_EVENT, battle_end_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Body parts of Arantula: 5 parts (model c_paid_b); the first is the body.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
    {2, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 5.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_churantalar", "c_paid_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/**
 * Draw callback: draws the Pider's thread as a thin white quad (2 units wide, with the unit's yarn alpha) from
 * the ceiling point down to the lower yarn endpoint, using direct GX calls and no texture.
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
 * Script helper: args[0] is the thread's alpha.
 * Sets the thread from y = 400 (ceiling) down to the lower endpoint in work[2]
 * and queues piders_draw_yarn_sub to draw it.
 */
static USER_FUNC(piders_draw_yarn) {
    u8 alpha = evtGetValue(event, event->args[0]);
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    PidersYarn* yarn = (PidersYarn*)TST_BATTLE_UNIT(unit)->countersetWeapon;

    // from the pider down to its hanging height, and up to the ceiling
    BtlUnit_GetHomePos(unit, &yarn->x1, &yarn->y1, &yarn->z1);
    yarn->y1 = unit->work[UNIT_WORK_THREAD_END_Y];
    BtlUnit_GetHomePos(unit, &yarn->x0, &yarn->y0, &yarn->z0);
    yarn->y0 = 400.0f;
    yarn->alpha = alpha;
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, piders_draw_yarn_sub, yarn);
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] receives how many Piders (kinds 0x1B and 0xA4) are still alive. */
static USER_FUNC(piders_get_num) {
    BattleUnit* unit;
    s32 i;
    EvtWord* args = event->args;
    s32 num = 0;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_PIDER || unit->currentKind == UNIT_ARANTULA) && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
            num++;
        }
    }
    evtSetValue(event, args[0], num);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is a unit id; args[1] receives that Pider's turn order ('zyun') among the living
 * Piders, counting from 1.
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
            if (!BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
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
 * Script helper: args[0] receives the position attribute (work[0]) of the first living Pider,
 * flipped between 0 and 1 when that Pider can act, so the others take the opposite side.
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
        if (unit != NULL && (unit->currentKind == UNIT_PIDER || unit->currentKind == UNIT_ARANTULA) && !BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
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
 * Script helper: picks three random targets from the weapon's current target list
 * and writes them as (unit, part) pairs to args, for the Pider's multi-hit ('renzoku') attack.
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

/** Returns the unit and part index of the weapon target currently selected in weaponTargets. */
static void piders_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

/** Advances to the next weapon target and returns its unit and part index, or (-1, 0) after the last one. */
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

/** Attack data of Arantula's piders attack: base damage 7, 100% accuracy. */
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
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

/** Attack data of Arantula's piders renzoku attack: base damage 4, 100% accuracy. */
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
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
 * Init script of the Arantula: installs the wait, unison-phase, attack, damage, confusion and entry handlers,
 * sets its work values (unit work 0 = 0, 1 = 255 yarn alpha, 2 = 0, 3 = 1),
 * initialises the shared yarn state (piders_yarn_init) and spawns yarn_event to draw its thread.
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
 * Data-table event 48, run when the battle opens with a First Strike on the unit (damage code 0x39,
 * battle_seq.c): puts it at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Background loop that draws the Arantula's silk thread every frame (piders_draw_yarn) with the alpha in unit
 * work 1; while it is below 255 the alpha drops by 2 per frame down to 2.
 * Ends when the unit no longer exists.
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
 * Entry script of the Arantula: starts it at height 180 with unit work 2 at 400,
 * waits 80 frames plus a tenth of its x position, then eases unit work 2 from 400 to 0 over 40 frames.
 * Unit work 2 is the lower end of its thread (piders_draw_yarn draws it down from y 400),
 * so the thread unrolls from the ceiling.
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

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Death script of the Arantula (LW(10) is the dying unit):
 * spins it for 48 frames with the defeat sound, bursts it with an explosion effect,
 * hands the coins and experience to the battle and shows the star points, then tilts it for 30 frames,
 * hides its parts and kills the unit (moving it off screen from the JP release on).
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
 * Death script for when the Arantula is blown away:
 * plays the breath sound, spins it while it flies up and off the stage, then stores its coins and experience,
 * hides its parts and kills the unit (moving it off screen from the JP release on).
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
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_CUBIC_IN, 0, -1)
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

/** Battle-end script: sets unit work 1 (the yarn alpha) to 254 so the thread starts to fade. */
static EVT_BEGIN(battle_end_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, 254)
    RETURN
EVT_END()

/**
 * Sways the Arantula: a damped y rotation back and forth (+60, -110, +80, -48, +28, -10 degrees over 92 frames),
 * then resets the rotation.
 * Runs in an inline event.
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
 * Moves the Arantula down to its low perch ("kuutei", height 10):
 * plays SFX_ENM_PAIDA_MOVE2 (MOVE1 the first time, while unit work 3 is set), sways, dives to the spot,
 * makes it the home position, clears kHammerlikeCannotTarget and
 * kShellTosslikeCannotTarget on its body so hammers and shell attacks reach it, and sets unit work 0
 * to 1.
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
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -10, EASING_QUADRATIC_OUT, 0, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 1)
    RETURN
EVT_END()

/**
 * Moves the Arantula up into the air ("kuuchu" = midair, height 70):
 * plays SFX_ENM_PAIDA_MOVE2, sways, dives up, makes it the home position, sets
 * kHammerlikeCannotTarget and kShellTosslikeCannotTarget on its body so hammers and shell
 * attacks cannot reach it, and sets unit work 0 to 0.
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
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 10, EASING_QUADRATIC_OUT, 0, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 0)
    RETURN
EVT_END()

/**
 * Phase script run before the player's turn (not while the Arantula is KO'd or unable to act):
 * the first living Pider or Arantula (piders_get_zyun = 1) switches between its high
 * and low perch (unit work 0);
 * each other one moves high or low (move_kuuchu_event or move_kuutei_event) against the first one's new height
 * (piders_get_first_pos_attr).
 * With an odd number of them they all match it; with an even number those at odd places match and those at even
 * places take the opposite.
 */
static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DEAD, LW(0))
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
 * Attack script of the Arantula: clears its unit work 11 to 13, handles an item use,
 * then picks at random between its normal attack (weapon_piders_attack)
 * and its multi-hit attack (weapon_piders_renzoku_attack), 50 percent each.
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

/**
 * Shared hit resolution for one shot (part LW(7)) at the target LW(3), LW(4).
 * On a dodge the target starts its avoid animation and the shot flies on and is hidden;
 * on a hit the shot is hidden, the guard result is applied (ResultACDefence) and then the damage,
 * as the last hit
 * (DAMAGE_RESULT_FINAL_HIT) when LW(8) = 1.
 * With LW(10) = 1 (triple shot) it first marks the target as hit in unit work 11 to 13.
 */
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
        SET_VALUE(LW(0), UNIT_WORK_TARGET1_ID)
        SET_VALUE(LW(1), UNIT_WORK_TARGET1_PART)
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
 * Wind-up of the Arantula's attack: turns toward the target, takes pose PAI_A_1 and shows part 2 (pose PAI_A_2a)
 * beside it, then swings part 2 in three steps with SFX_ENM_PAIDA_ATTACK1 each (z rotation 0 to 110, to 70,
 * to 100 degrees) and stores the final angle in unit work 4.
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
 * Shorter wind-up (no script of this unit calls it): turns to the target, shows part 2 and swings it from the
 * angle in unit work 4 to 100 degrees with one SFX_ENM_PAIDA_ATTACK1.
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

/**
 * Ends the single shot's wind-up: rotates part 2 back from 100 degrees to 0 over 15 frames, returns to PAI_S_1
 * and hides part 2.
 */
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
 * Fires the shot part LW(7) (pose PAI_A_3a) with SFX_ENM_PAIDA_ATTACK2: it starts 20, 30 or 15 units above the
 * Arantula with a jump speed of 6, 4 or 8 for LW(5) = 5, 6 or 7, is shown at double size and jumps to the
 * target's hit position.
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
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(7), LW(0), LW(1), LW(2), 0, -1)
    RETURN
EVT_END()

/**
 * The Arantula's single shot (weapon_piders_attack): does nothing in a first strike; otherwise picks a target,
 * winds up, starts winding back down alongside (piders_attack_common_event2), fires part 3 at the target and
 * resolves it as the last hit (damage_check_event_sub).
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
 * The Arantula's triple shot (weapon_piders_renzoku_attack): draws three random targets, repeats allowed
 * (piders_renzoku_attack_set_target; units in unit work 5 to 7, parts in 8 to 10), winds up and fires parts 3, 4
 * and 5 at them one after another, swinging part 2 between shots.
 * Each shot is resolved by renzoku_attack_event_sub; then part 2 swings back and is hidden.
 */
static EVT_BEGIN(renzoku_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // The Arantula draws three random targets, repeats allowed.
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

    // The attack is declared on the first selected target and its cost paid.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    SET_VALUE(LW(5), UNIT_WORK_TARGET1_ID)
    SET_VALUE(LW(6), UNIT_WORK_TARGET1_PART)
    SET_VALUE(LW(7), 3)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(6), LW(4))

    // It winds up and fires its first web shot.
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

    // After a short pause it fires the second shot.
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

    // And then the third shot, each one resolved against its own target.
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

    // The Arantula swings back to its idle pose and the camera resets.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAI_S_1"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Resolves one shot of the triple shot (slot LW(5) in unit work 5 to 7): when a later shot goes to the same
 * target it runs renzoku_attack_event_sub_no_last, otherwise renzoku_attack_event_sub_last, so only a target's
 * final shot is its last hit.
 */
static EVT_BEGIN(renzoku_attack_event_sub)
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(4))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), UNIT_WORK_TARGET3_ID)
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
 * Resolves a shot that is not the last one at its target: checks the hit without DAMAGE_RESULT_FINAL_HIT and marks
 * the target as hit when it lands (LW(8) = 0, LW(10) = 1).
 */
static EVT_BEGIN(renzoku_attack_event_sub_no_last)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(damage_check_event_sub)
    RETURN
EVT_END()

/**
 * Resolves the last shot at a target of the triple shot: when an earlier shot already hit that target (its mark
 * in unit work 11 to 13) it hides the shot and applies the guard result and the damage as the last hit without a
 * new dodge check; otherwise it checks the hit normally as the last hit (LW(8) = 1, LW(10) = 0).
 */
static EVT_BEGIN(renzoku_attack_event_sub_last)
    SET_VALUE(LW(0), UNIT_WORK_TARGET1_ID)
    SET_VALUE(LW(1), UNIT_WORK_TARGET1_PART)
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

/** Idle script: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
