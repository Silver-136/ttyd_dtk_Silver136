// unit_piders: Pider, the spider enemy hanging on its thread, battle unit.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "memory.h"
#include "dolphin/gx.h"
#include "driver/camdrv.h"
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

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOW 0          // 1 while hanging low (y 10, hammer and shell toss can reach it; move_kuutei_event), 0 while high up (y 70; move_kuuchu_event)
#define UNIT_WORK_THREAD_ALPHA 1 // alpha of the thread piders_draw_yarn draws: 255 normally; battle_end_event sets 254, then yarn_event fades it by 2 per frame
#define UNIT_WORK_THREAD_END_Y 2 // y of the thread's lower end (piders_draw_yarn; the upper end is at y 400); entry_event lowers it from 400 to 0 over 40 frames
#define UNIT_WORK_FIRST_DROP 3   // 1 until the first move_kuutei_event, which then plays SFX_ENM_PAIDA_MOVE1 instead of MOVE2 and clears it
#define UNIT_WORK_WINDUP_ANGLE 4 // part 2 rotation (degrees) reached at the end of the wind-up (piders_attack_common_event1); piders_attack_common_event1_2 starts from it
#define UNIT_WORK_TARGET_UNIT 5  // first of three slots (5-7): target unit of each shot of renzoku_attack_event (slots 5-7)
#define UNIT_WORK_TARGET_PART 8  // first of three slots (8-10): target part of each shot of renzoku_attack_event (slots 8-10)
#define UNIT_WORK_TARGET_HIT 11  // first of three slots (11-13): 1 once a shot hit that target; the last shot at a target that was hit skips the precheck and applies final-hit damage (renzoku_attack_event_sub_last) (slots 11-13)

#include "unit/unit_piders.h"
static void piders_draw_yarn_sub(CameraId camera, void* param);

static USER_FUNC(piders_draw_yarn);
static USER_FUNC(piders_get_first_pos_attr);
static USER_FUNC(piders_get_num);
static USER_FUNC(piders_get_zyun);
static USER_FUNC(piders_renzoku_attack_set_target);
static USER_FUNC(piders_yarn_init);
static void piders_get_next_target(s32* unitIdx, s32* partIdx);
static void piders_get_target(s32* unitIdx, s32* partIdx);

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
static EvtWord piders_attack_common_event3[70];
static EvtWord normal_attack_event[87];
static EvtWord renzoku_attack_event[149];
static EvtWord renzoku_attack_event_sub[55];
static EvtWord renzoku_attack_event_sub_no_last[10];
static EvtWord renzoku_attack_event_sub_last[79];

#if VERSION <= VERSION_DEMO_JP
static EvtWord wait_event[6];
#else
static EvtWord wait_event[7];
#endif

/**
 * Script helper that allocates the Pider's yarn drawing state and keeps it in the unit's countersetWeapon slot,
 * which is unused for a Pider.
 */
static USER_FUNC(piders_yarn_init) {
    s32 id = BattleTransID(event, UNIT_ID_SELF);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    unit->countersetWeapon = (BattleWeapon*)BTL_ALLOC(sizeof(PidersYarn)); // piders keep their yarn work in the counter-set weapon slot
    return EVT_RETURN_DONE;
}

/** Out-of-line stand-in for the SDK's GXPosition3f32 inline: writes three floats to the GX FIFO. */
void GXPosition3f32_(f32 x, f32 y, f32 z) {
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
    GXWGFifo.f32 = z;
}

/** Out-of-line stand-in for the SDK's GXEnd inline, which does nothing. */

// GXEnd_ is empty; GXPosition3f32_ writes three coordinates to the graphics FIFO.
void GXEnd_(void) {
}

/**
 * Unit data of the Pider: hit points, level, size, sounds, parts, status vulnerability, init event and
 * event data table.
 */
BattleUnitKind unit_piders = {
    UNIT_PIDER, // unitType
    "btl_un_piders", // name
    5, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    12, // level
    0, // bonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    70, // runRate
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
#if VERSION >= VERSION_JP
    100, // hammerKnockbackChance
#endif
    0, // itemStealParam
#if VERSION <= VERSION_DEMO_JP
    0,
    0.0f,
    0.0f,
    0.0f,
#else
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
#endif
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

/**
 * Defense of the Pider against normal, fire, ice, explosion and electric attacks: none, so it takes full
 * damage from every element.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Damage-type reactions of the Pider: none, so every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {90, 100, 50, 100, 90, 100, 100, 90, 100, 90, 100, 90, 100, 90, 90, 100, 70, 100, 90, 100, 100, 95};
#else
static StatusVulnerability regist = {
    90, // sleep
    90, // stop
    50, // dizzy
    100, // poison
    90, // confuse
    100, // electric
    100, // burn
    90, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    90, // allergic
    100, // fright
    70, // galeForce
    100, // fast
    90, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

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
 * Event data table of the Pider: scripts that replace the standard handling for its defeat, being
 * blown away, the end of the battle and the battle opening with a First Strike on it.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, blow_dead_event},
    {UNIT_DATA_BATTLE_END_EVENT, battle_end_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Parts of the Pider: 5 parts: parts 1 to 5 (model c_paid), each with its defense tables, attributes and
 * pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
    {2, "btl_un_piders", "c_paid", 0.0f, 0.0f, 5.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_piders", "c_paid", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Draw callback that renders the thin yarn quad between the ceiling and the Pider with the current alpha. */
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
    GXPosition3f32_(yarn->x0 - 1.0f, yarn->y0, yarn->z0 - 5.0f);
    GXPosition3f32_(1.0f + yarn->x0, yarn->y0, yarn->z0 - 5.0f);
    GXPosition3f32_(1.0f + yarn->x1, yarn->y1, yarn->z1 - 5.0f);
    GXPosition3f32_(yarn->x1 - 1.0f, yarn->y1, yarn->z1 - 5.0f);
    GXEnd_();
}

/**
 * Script helper: arg0 = alpha (0..255).
 * Sets the yarn's endpoints (the Pider's current hang height and y=400 at the ceiling) and queues the draw.
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

/** Script helper: arg0 receives the number of living Piders and Arantulas (unit kinds 0x1B and 0xA4) in the battle. */
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
 * Script helper (zyun = order): arg0 = unit id;
 * arg1 receives its position in the line of living Piders, counted in unit order.
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
 * Script helper: arg0 receives the height the first living Pider (or Arantula) will have after this turn's move:
 * its unit work 0 (1 = low, 0 = high), flipped when it is able to act, since the first one always switches.
 * unison_phase_event lines the other Piders up against it.
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
    attr = unit->work[UNIT_WORK_LOW];
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
 * Script helper for the Pider's multi-hit attack:
 * collects the possible targets and writes three random (unit, part) pairs into the argument variables.
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

/** Returns the unit and part index of the weapon target currently selected in the target list. */
static void piders_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

/**
 * Advances the weapon target list to the next target and returns its unit and part index,
 * or -1 and 0 at the end of the list.
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

/** Weapon record of the Pider's attack: 2 damage. */
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

/** Weapon record of the Pider's repeated attack: 1 damage. */
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
 * Init script of the Pider: registers its wait, unison-phase, attack, damage, confusion and entry handlers,
 * sets its unit work (0 = hanging in the air, 1 = yarn opacity 255, 2 = entry offset, 3 = first-move flag),
 * initialises the thread drawing (piders_yarn_init) and spawns yarn_event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_END_Y, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_DROP, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, 255)
    CALL(piders_yarn_init)
    SPAWN_EVENT(PTR(yarn_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Position event for a first strike on this unit: sets its height to 10 and waits for the attack to finish. */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Background loop for the Pider's thread: while the unit exists,
 * once unit work 1 has been lowered below 255 it keeps subtracting 2 from it each frame down to 2
 * and redraws the thread with that value (piders_draw_yarn), which fades the thread out;
 * battle_end_event starts this by setting the value to 254.
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
 * Entrance of the Pider: moves it to height 180, waits 80 frames plus a tenth of its x position,
 * then eases unit work 2, the lower end of its thread (piders_draw_yarn draws it down from y 400),
 * from 400 to 0 over 40 frames, so the thread unrolls from the ceiling.
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
 * Damage reaction: runs the default damage handling (btldefaultevt_Damage) with LW(10) = -2 (the unit itself)
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Death of the Pider: it spins in place with SFX_BTL_ENEMY_DIE1_1 and 2,
 * bursts with the expbom_n64 effect and a star_point effect, then hides its parts and kills the unit.
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
 * Death when the Pider is blown away (Flurrie's gust, SFX_BTL_CLAUD_BREATH3):
 * it spins and dives off the edge of the stage, stores its coin and experience rewards,
 * and the unit is removed (from the JP release on it is also moved far below the stage first).
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

/** Runs when the battle ends: sets unit work 1 to 254 so that yarn_event starts fading the thread out. */
static EVT_BEGIN(battle_end_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_THREAD_ALPHA, 254)
    RETURN
EVT_END()

/**
 * Swings the Pider about its vertical axis in a damped sway (rotation steps of +6, -5, +4, -3, +2,
 * -1 degrees per frame over 10 to 22 frames each) and then resets the rotation.
 * Used when it settles on a new perch.
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
 * Moves the Pider down to its low perch ("kuutei", height 10) with a sway
 * and SFX_ENM_PAIDA_MOVE2 (SFX_ENM_PAIDA_MOVE1 the first time, while unit work 3 is set),
 * sets its home position there, clears PART_ATTR_HAMMERLIKE_CANNOT_TARGET and
 * PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET (6291456) on its body part so hammers and shell attacks reach it,
 * and sets unit work 0 to 1.
 */
static EVT_BEGIN(move_kuutei_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_DROP, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FIRST_DROP, 0)
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 1)
    RETURN
EVT_END()

/**
 * Moves the Pider up into the air ("kuuchu" = midair, height 70) with SFX_ENM_PAIDA_MOVE2 and a sway,
 * sets its home position there, sets PART_ATTR_HAMMERLIKE_CANNOT_TARGET
 * and PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET
 * (6291456) on its body part so hammers and shell attacks cannot reach it, and sets unit work 0 to 0.
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 0)
    RETURN
EVT_END()

/**
 * Start-of-turn event of the Pider (not while it is KO'd or unable to act): the first living Pider
 * (piders_get_zyun = 1) switches between its high and low perch (unit work 0);
 * each other one moves high or low (move_kuuchu_event or move_kuutei_event) against the first one's new height
 * (piders_get_first_pos_attr).
 * With an odd number of Piders they all match it; with an even number those at odd places match and those at
 * even places take the opposite.
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
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, LW(0))
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

/**
 * Chooses the Pider's attack: item use if any; otherwise a 50% chance of a single web attack
 * (normal_attack_event with weapon_piders_attack) and
 * otherwise the chained attack (renzoku_attack_event with weapon_piders_renzoku_attack).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_HIT + 1, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_HIT + 2, 0)
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
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_piders_renzoku_attack))
    CALL_CHILD(PTR(renzoku_attack_event))
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Resolves one Pider shot (part LW(7)) against the target in LW(3) and LW(4) with weapon LW(9):
 * on a dodge (pre-check results 2, 3, 4, 6) the target starts its avoid (none for 4) and the shot flies on
 * and is hidden; on a hit
 * the shot is hidden, with LW(10) = 1 the target's slot is marked in unit work 11 to 13 (matched against units
 * in
 * work 5 to 7, parts in 8 to 10), and the guard result and the damage are applied,
 * as the last hit when LW(8) = 1.
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
 * Start of the Pider's web attack: strikes the PAI_A_1 pose,
 * brings the second part (Guess: the web shot) into the PAI_A_2a pose
 * and swings it around with three SFX_ENM_PAIDA_ATTACK1 whooshes.
 * The swing radius depends on the unit's size (statuses 10 and 11); the final rotation is saved in unit work 4.
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
 * A shortened version of the web wind-up: starts from the rotation saved in unit work 4
 * and makes only one more swing with SFX_ENM_PAIDA_ATTACK1.
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
 * Winds the web back after the attack: turns the Pider to face the target,
 * eases the web's rotation back to zero over 15 frames, returns to PAI_S_1 and hides the web part.
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
 * Throws the web: SFX_ENM_PAIDA_ATTACK2 and the PAI_A_3a pose on part LW(7),
 * which is scaled up to 2.0 and flung at the target's hit position.
 */
static EVT_BEGIN(piders_attack_common_event3)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAIDA_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(7), STRING("PAI_A_3a"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(7), 0, FLOAT(6.0f), FLOAT(0.25f))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(7), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), kNoShadow)
    CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(7), FLOAT(2.0f), FLOAT(2.0f), FLOAT(2.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(7), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * The Pider's single web shot with the weapon in LW(9):
 * picks a target, runs the wind-up, throws the web (piders_attack_common_event1 and 3)
 * and resolves the hit with damage_check_event_sub.
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
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    SET_VALUE(LW(7), 3)
    CALL_CHILD(PTR(piders_attack_common_event1))
    SPAWN_EVENT(PTR(piders_attack_common_event2))
    SET_VALUE(LW(8), 1)
    SET_VALUE(LW(5), 5)
    CALL_CHILD(PTR(piders_attack_common_event3))
    SET_VALUE(LW(10), 0)
    CALL_CHILD(PTR(damage_check_event_sub))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(500)
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Pider's chained web attack: picks the targets (or the confused-flustered path),
 * records each target and part in unit work 5 to 10, and then throws a web at each of them in turn,
 * handing the results to renzoku_attack_event_sub.
 */
static EVT_BEGIN(renzoku_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(piders_renzoku_attack_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_UNIT, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_UNIT + 1, LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_UNIT + 2, LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_PART, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_PART + 1, LW(3))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_PART + 2, LW(5))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_UNIT, LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_PART, LW(4))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    SET_VALUE(LW(5), 5)
    SET_VALUE(LW(6), 8)
    SET_VALUE(LW(7), 3)
    CALL_CHILD(PTR(piders_attack_common_event1))
    LOOP(3)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(5), LW(3))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(6), LW(4))
        CALL_CHILD(PTR(piders_attack_common_event3))
        SPAWN_EVENT(PTR(renzoku_attack_event_sub))
        ADD(LW(5), 1)
        ADD(LW(6), 1)
        ADD(LW(7), 1)
        WAIT_FRAMES(30)
    END_LOOP
    SPAWN_EVENT(PTR(piders_attack_common_event2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Resolves one shot using the target records saved in unit work (6 slots, 5 to 10):
 * calls the no-last variant if a later shot targets the same unit and part, otherwise
 * renzoku_attack_event_sub_last.
 */
static EVT_BEGIN(renzoku_attack_event_sub)
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(3))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(4))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 7)
            CALL_CHILD(PTR(renzoku_attack_event_sub_last))
            BREAK_LOOP
        END_IF
        ADD(LW(0), 1)
        ADD(LW(1), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(10))
        IF_INT_EQUAL(LW(3), LW(8))
            IF_INT_EQUAL(LW(4), LW(10))
                CALL_CHILD(PTR(renzoku_attack_event_sub_no_last))
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Chain step for any target that is not the last:
 * resolves its shot with damage_check_event_sub (LW(8) = 0, LW(10) = 1).
 */
static EVT_BEGIN(renzoku_attack_event_sub_no_last)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(PTR(damage_check_event_sub))
    RETURN
EVT_END()

/**
 * Chain step for the last target: if its record shows a still-pending hit it applies the damage with
 * ResultACDefence and CheckDamage directly, otherwise it resolves the shot with damage_check_event_sub (LW(8) =
 * 1, LW(10) = 0).
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
        CALL_CHILD(PTR(damage_check_event_sub))
    END_IF
    RETURN
EVT_END()

/** Idle event of the Pider: sets its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
#if VERSION <= VERSION_DEMO_JP
EVT_END()
#else
EVT_END_PAD()
#endif

