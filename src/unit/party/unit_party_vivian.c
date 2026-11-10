// Battle unit data and scripts for Vivian as a partner.

#include "version.h"
#include "unit/party/unit_party_vivian.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "system.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include <string.h>
#include "libc_decls.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "driver/camdrv.h"
#include "battle/unit_work.h"

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "icon_ids.h"
#include "battle/battle_damage.h"

static USER_FUNC(_vivian_make_extra_work_area);
static USER_FUNC(_disp_heart_entry_stop_check);
USER_FUNC(_disp_heart_entry_stop);
static void _disp_heart(CameraId camera, void* param);
USER_FUNC(_disp_heart_entry);
static USER_FUNC(_make_kagenuke_weapon);
USER_FUNC(_get_vivian_attack_position);
static USER_FUNC(_get_move_frame);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);

PoseTableEntry pose_table_vivian_stay[24];
static BattleDataEntry data_table[20];
static BattleUnitKindPart unitpartsdata_Party_Vivian[1];
BattleUnitKind unitdata_Party_Vivian;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];
static EvtWord battle_entry_event[59];
static EvtWord init_event[68];
static EvtWord damage_event[16];
EvtWord vivian_hide_event[209];
BattleWeapon partyWeapon_VivianNormalAttack;
BattleWeapon partyWeapon_VivianShadowGuard;
BattleWeapon partyWeapon_VivianMagicalPowder;
BattleWeapon partyWeapon_VivianCharmKissAttack;

#if VERSION <= VERSION_DEMO_JP
static EvtWord partyVivianAttack_NormalAttack[610];
extern EvtWord partyVivianAttack_ShadowGuard[];
#else
static EvtWord partyVivianAttack_NormalAttack[581];
EvtWord partyVivianAttack_ShadowGuard[430];
#endif

EvtWord partyVivianAttack_MagicalPowder[VERSION_PICK(442, 448)];
EvtWord partyVivianAttack_CharmKissAttack[464];
EvtWord vivian_shadow_tail_event[78];
static EvtWord vivian_counter_damage_event[177];
static EvtWord vivian_appeal_event[80];
static EvtWord party_win_reaction[18];
static EvtWord attack_audience_event[189];

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
#endif

/**
 * Vivian's pose table: animations for each status (stop, dizzy, poison, confusion, sleep, freeze, KO),
 * the hurt poses, gliding and fast gliding, low HP, the failed Run Away tumble, talking, item use,
 * the command menu wait, zero gravity, the Defend stance and plain floating.
 */
PoseTableEntry pose_table_vivian_stay[] = {
    {POSE_TYPE_FREEZE, "PTR_D_4"},
    {POSE_TYPE_STOP, "PTR_D_4"},
    {POSE_TYPE_DIZZY, "PTR_X_1"},
    {POSE_TYPE_POISON, "PTR_D_2"},
    {POSE_TYPE_CONFUSE, "PTR_X_1"},
    {POSE_TYPE_SLEEP, "PTR_N_1"},
    {POSE_TYPE_KO, "PTR_D_3"},
    {POSE_TYPE_NORMAL, "PTR_S_1"},
    {POSE_TYPE_DAMAGE_DRAINED, "PTR_G_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PTR_G_1"},
    {POSE_TYPE_DAMAGE, "PTR_D_1"},
    {POSE_TYPE_WALK, "PTR_W_1"},
    {POSE_TYPE_RUN, "PTR_R_1"},
    {POSE_TYPE_PERIL, "PTR_D_2"},
    {POSE_TYPE_DANGER, "PTR_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PTR_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PTR_D_2"},
    {POSE_TYPE_TALK, "PTR_T_1"},
    {POSE_TYPE_ITEM, "PTR_I_1"},
    {POSE_TYPE_COMMAND, "PTR_C_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PTR_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PTR_D_1"},
    {POSE_TYPE_DEFEND, "PTR_G_1"},
    {POSE_TYPE_ACTION, "PTR_S_1"},
};

/**
 * Vivian's data events: the Veil countdown each phase (tag 3), Appeal, the audience attack reaction, her move
 * list (tag 1, __makeTechMenuFunc), the counter-hit reaction for tags 25-35, First Strike (nothing), the
 * victory pose, an empty step run after a move's cost is paid (7) and the spin when she
 * superguards an attack (47).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_HIDE_EVENT, vivian_hide_event},
    {UNIT_DATA_APPEAL_EVENT, vivian_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, vivian_counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

/** Vivian's single battle part: her targetable body (model c_vivian) with her pose table. */
static BattleUnitKindPart unitpartsdata_Party_Vivian[] = {
    {1, "btl_un_vivian", "c_vivian", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_vivian_stay},
};

/**
 * Vivian's battle unit (UNIT_VIVIAN), listed in battle_party.c's partner table:
 * a floating unit with one part, the partner hit sounds and no enemy HP gauge.
 */
BattleUnitKind unitdata_Party_Vivian = {
    UNIT_VIVIAN, // unitType
    "btl_un_vivian", // name
    10, // maxHp
    5, // maxFp
    5, // dangerHP
    1, // perilHP
    1, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    20, // width
    33, // height
    {14, 36}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 40.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {10.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 21.45f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_MARIO_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_MARIO_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_MARIO_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating | kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    unitpartsdata_Party_Vivian, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Vivian's defence: 0 against every element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Vivian's defence attributes: normal (0) against every element, no weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Vivian's status chances: 100 for every status (badges lower them in battle). */
#if VERSION >= VERSION_JP
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
#endif

/** Empty: partner moves come from the player's menu choices, not a unit attack script. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif

/**
 * Vivian-style emergence from a shadow for part args[1] of unit args[0]:
 * grows the shadow, places the unit at (args[2], args[3], args[4]) and plays the p_bibi rise animation,
 * then shrinks the shadow back. args[5] non-zero also clears the shared-shadow flag. Blocks until done.
 */
USER_FUNC(battle_evt_majo_disp_on) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, partNo, clearShadowFlag;
    f32 px, py, pz;
    s32 pose;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partNo = evtGetValue(event, args[1]);
    px = evtGetFloat(event, args[2]);
    py = evtGetFloat(event, args[3]);
    pz = evtGetFloat(event, args[4]);
    clearShadowFlag = evtGetValue(event, args[5]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partNo);
    pose = part->poseId;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            v.x = px;
            v.y = py - 200.0f;
            v.z = pz;
            unit->position = v;
        }
        animEffectAsync("p_bibi", 1);
    }
    switch (event->userdata[0]) {
        case 0:
            part->shadowScale = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, event->userdata[1], 30);
            if (event->userdata[1] == 0) {
                psndSFXOn_3D("SFX_BTL_VIVIAN_SHADOW1_1", &unit->position);
            }
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                event->userdata[0]++;
            }
            break;
        case 1:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (!animEffectAsync("p_bibi", 1)) {
                return EVT_RETURN_BLOCK;
            }
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                v.x = px;
                v.y = py;
                v.z = pz;
                unit->position = v;
            }
            animPoseSetEffect(pose, "p_bibi", 1);
            animPoseSetEffectAnim(pose, "PM_B_2", 1);
            event->userdata[0]++;
            break;
        case 2:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(pose)->paperId) >= 0.6666667f) {
                animPoseSetPaperAnimGroup(pose, NULL, 1);
                event->userdata[0]++;
                event->userdata[1] = 0;
            }
            break;
        case 3:
            part->shadowScale = intplGetValue(EASING_LINEAR, 2.0f, 1.0f, event->userdata[1], 15);
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                part->shadowScale = 1.0f;
                animPoseSetEffect(pose, NULL, 1);
                if (clearShadowFlag != 0) {
                    unit->flags &= ~BATTLE_UNIT_FLAG_SHADOW_WITH_PARTNER;
                }
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Vivian's battle entry: starts 250 units behind her home spot and, after 40 frames,
 * floats in to it at speed 4 with her hovering sound, then takes her stay pose.
 */
static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 0, 6, 6, 0, 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(_vivian_make_extra_work_area)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, VIVIAN_WORK_HIDE_TURNS, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, 0)
    SPAWN_EVENT(vivian_shadow_tail_event)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 11, 11)
    CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 11, 11)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Hit reaction: straightens her rotation (left over from her moves), then runs the default damage script. */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

EVT_BEGIN(vivian_hide_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, VIVIAN_WORK_HIDE_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SUBTRACT(LW(0), 1)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, VIVIAN_WORK_HIDE_TURNS, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        SET_VALUE(LW(3), UNIT_ID_MARIO)
        CALL(btlevtcmd_PhaseEventStartDeclare, LW(3))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 15)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_OffAttribute, LW(3), kVeiled)
            CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kHidden | kNoShadow))
            CALL(battle_evt_majo_disp_on, LW(3), 1, LW(0), LW(1), LW(2), 1)
            CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetMoveSpeed, LW(3), FLOAT(2.0f))
            CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 1)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(2.0f), LW(15))
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), MOVE_SOUND_FROM_TABLE, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, LW(15), LW(15), 15)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            DELETE_EVENT(LW(14))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, PTR(&partyWeapon_VivianShadowGuard), 1, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_Y_1"))
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
            CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
            CALL(btlevtcmd_WaitEventEnd, LW(14))
        END_SWITCH
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    END_IF
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/** Idle: plays the stay pose from the current pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif

/**
 * Shade Fist (0 FP): a punch from the shadows on any single enemy; damage by rank 3, 4 or 5
 * (the same whether or not the action command succeeds); only a successful command burns it for 2 turns.
 * Cannot be guarded; usable while confused.
 */
BattleWeapon partyWeapon_VivianNormalAttack = {
    "btl_wn_ptr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ptr_kagenuke", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {3, 3, 4, 4, 5, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kagenuke", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    2, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_NormalAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Veil (1 FP): Vivian targets Mario and pulls him into the shadows with her so neither can be attacked
 * until they come out. No damage (the damage values here are unused).
 */
BattleWeapon partyWeapon_VivianShadowGuard = {
    "btl_wn_ptr_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ptr_kagegakure", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_FLOATING2 | TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kagegakure", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_ShadowGuard, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Fiery Jinx (6 FP): fire on every enemy that pierces defence and burns for 3 turns (100% chance);
 * the damage is the action command result (weaponGetACOutputParam on output 0). Cannot be guarded.
 */
BattleWeapon partyWeapon_VivianMagicalPowder = {
    "btl_wn_ptr_lv2", // name
    ICON_PARTNER_MOVE_LV2, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ptr_mahou_no_kona", // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_mahou_no_kona", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_MagicalPowder, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_VivianCharmKissAttack = {
    "btl_wn_ptr_lv3", // name
    ICON_PARTNER_MOVE_LV3, // icon
    {0}, // pad_06
    0, // itemId
    "msg_ptr_meromero_kiss", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_meromero_kiss", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, // poisonStrength
#if VERSION <= VERSION_DEMO_JP
    90,
#else
    100, // confuseChance
#endif
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_CharmKissAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Vivian's Shade Fist: she sinks into the shadows,
 * reappears beside the enemy (_get_vivian_attack_position) and punches it with the action command,
 * the hit able to burn it for 2 turns (_make_kagenuke_weapon), then returns.
 */
static EVT_BEGIN(partyVivianAttack_NormalAttack)
    // Pick the target, set up the burn and pay the cost; the camera follows Vivian and the enemy.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(_make_kagenuke_weapon, LW(12), 2)
        CASE_EQUAL(1)
        CALL(_make_kagenuke_weapon, LW(12), 2)
        CASE_DEFAULT
        CALL(_make_kagenuke_weapon, LW(12), 2)
    END_SWITCH
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // Action command (press the button shown on screen) starts as she sinks into her shadow.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 1, 1, 40, 1, 2, 0, 3, 0)
    CALL(btlevtcmd_AcSetFlag, AC_SIGNAL_TIMING_FLAG_SHOW_BUTTONS | AC_SIGNAL_TIMING_FLAG_HIDE_UPCOMING_BUTTONS | AC_SIGNAL_TIMING_FLAG_HIDE_HIT_COUNT | AC_SIGNAL_TIMING_FLAG_END_ON_HIT)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 18, 1, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK1"), 0)
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
    CALL(btlevtcmd_StartAC, 1)

    // She glides under the stage to a spot beside the enemy and rises out of the floor there.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_TransStageFloorPosition, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SUBTRACT(LW(1), 20)
    IF_INT_LESS(LW(1), LW(7))
        SET_VALUE(LW(1), LW(7))
    END_IF
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 50, 0, EASING_CUBIC_OUT, 0, -1)
#else
    CALL(_get_vivian_attack_position, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 50, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
#endif
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, 1)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_VIVIAN_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))

    // She turns to face the enemy and glides in close if she is not there yet.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_TransStageFloorPosition, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SUBTRACT(LW(1), 20)
    IF_INT_LESS(LW(1), LW(7))
        SET_VALUE(LW(1), LW(7))
    END_IF
#else
    CALL(_get_vivian_attack_position, LW(3), LW(0), LW(1), LW(2))
    CALL(_get_vivian_attack_position, LW(3), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_TransStageFloorPosition, LW(6), LW(7), LW(8))
#endif
    CALL(btlevtcmd_GetFaceDirection, LW(3), LW(15))
    MULTIPLY(LW(15), -1)
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(_get_move_frame, LW(0), LW(1), LW(2), LW(6), LW(7), LW(8), FLOAT(4.0f), LW(6))
    IF_INT_GREATER_OR_EQUAL(LW(6), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_VIVIAN_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    END_IF

    // A pre-emptive spiky enemy hurts her first.
    LABEL(9)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), PRECHECK_PREEMPTIVE_SPIKY)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF

    // Wait for the command result, then check whether the punch connects; a miss plays the enemy's dodge reaction.
    CALL(btlevtcmd_ResultAC)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_MISS)
        CASE_OR_EQUAL(PRECHECK_LUCKY)
        CASE_OR_EQUAL(PRECHECK_DODGY)
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        CASE_END
        CASE_EQUAL(PRECHECK_HIT)
        CASE_DEFAULT
    END_SWITCH
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_2"))
        WAIT_MS(166)
        IF_INT_EQUAL(LW(5), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        WAIT_MS(500)
        GOTO(90)
    END_IF

    // The punch lands; a successful command also burns the enemy and adds the rating effect (Nice, Good...).
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_2"))
    CALL(btlevtcmd_ftomsec, 4, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK4"), 0)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_STATUS, LW(5))
    END_IF
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    WAIT_MS(500)

    // She sinks back into the floor, glides home under the stage and rises again; pressing A as she rises is the
    // stylish.
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, 0)
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    BEGIN_SIBLING_EVENT
#if VERSION >= VERSION_US
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 14, 29, 29, 0)
#else
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 20, 29, 29, 0)
#endif
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_Y_1"))
        WAIT_FRAMES(52)
    END_IF

    // The audience refills Star Power and her turn ends.
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_ShadowGuard)
    SET_VALUE(LW(3), UNIT_ID_MARIO)
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), -1, FACE_DIR_TOWARD_UNIT, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A2_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 15)
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), -1, FACE_DIR_TOWARD_UNIT, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 420)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 330)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 270)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 240)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 200)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 160)
        CASE_DEFAULT
        SET_VALUE(LW(0), 120)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 5, -2, 5, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 10, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(90)
    END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
#else
    CALL(btlevtcmd_GetResultPrizeLv, -1, 0, LW(6))
#endif
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
#if VERSION >= VERSION_JP
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
#endif
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A2_2"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A2_3"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("M_B_3"))
        CALL(battle_evt_majo_disp_off, LW(3), 1, 1, 0)
        CALL(btlevtcmd_OnAttribute, LW(3), kVeiled)
        CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, (kHidden | kNoShadow))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 1, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, LW(3), LW(5), LW(1), LW(2))
    ADD(LW(0), LW(5))
    DIVIDE(LW(0), 2)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    ADD(LW(1), 1)
    CALL(evt_eff, 0, STRING("ripple"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(333)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable | kHidden)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, VIVIAN_WORK_HIDE_TURNS, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, -1, -1, FACE_DIR_ATTACK_DIR, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(0))
    WAIT_MS(500)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
#if VERSION >= VERSION_JP
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
#endif
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_MagicalPowder)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 360)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 270)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 210)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 180)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 160)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 140)
        CASE_DEFAULT
        SET_VALUE(LW(0), 120)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 5, -3, 1, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, AC_BUTTON_DOWN_FLAG_REVEAL_ONE_BY_ONE | AC_BUTTON_DOWN_FLAG_PARTIAL_SUCCESS | AC_BUTTON_DOWN_FLAG_ALLOW_REPEATS)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 10, 1, 0)
    WAIT_FRAMES(22)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetWidth, UNIT_ID_SELF, LW(5))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(6))
        CALL(evt_eff, 0, STRING("stardust"), 4, LW(0), LW(1), LW(2), LW(5), LW(6), 30, 60, 0, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_VIVIAN_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A3_1"))
    CALL(btlevtcmd_ftof, 20, LW(0))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_ftof, 26, LW(0))
    CALL(btlevtcmd_ftof, 46, LW(1))
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(0), LW(1), LW(1), 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_Y_1"))
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
#if VERSION >= VERSION_US
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
#endif
    WAIT_FRAMES(40)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 6, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_NOT_EQUAL(LW(6), PRECHECK_HIT)
        IF_INT_EQUAL(LW(6), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))

    // BUG: the test is the wrong way round against the prize switch below: 0 to 3 correct presses pass
    // DAMAGE_RESULT_AC_SUCCESS, 4 and more do not. The action command itself reports success from one correct
    // press (AC_BUTTON_DOWN_FLAG_PARTIAL_SUCCESS), so the flag only changes the case of no correct press:
    // with All or Nothing equipped the attack still gets the badge's bonus then.
    IF_INT_LESS_OR_EQUAL(LW(0), 3)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    IF_INT_EQUAL(LW(5), CHECK_DAMAGE_HIT)
        CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(0)
            SET_VALUE(LW(0), -1)
            CASE_OR_EQUAL(1)
            CASE_OR_EQUAL(2)
            SET_VALUE(LW(0), 0)
            CASE_END
            CASE_OR_EQUAL(3)
            CASE_OR_EQUAL(4)
            SET_VALUE(LW(0), 1)
            CASE_END
            CASE_DEFAULT
            SET_VALUE(LW(0), 2)
        END_SWITCH
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    END_IF
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_1"))
    WAIT_MS(800)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_CharmKissAttack)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    WAIT_FRAMES(20)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A4_1"))
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_MOJIMOJI1"), 0)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(5), 60)
        CASE_EQUAL(-2)
        SET_VALUE(LW(5), 45)
        CASE_EQUAL(-1)
        SET_VALUE(LW(5), 30)
        CASE_EQUAL(0)
        SET_VALUE(LW(5), 25)
        CASE_EQUAL(1)
        SET_VALUE(LW(5), 20)
        CASE_EQUAL(2)
        SET_VALUE(LW(5), 12)
        CASE_DEFAULT
        SET_VALUE(LW(5), 6)
    END_SWITCH
    CALL(evt_sub_random, LW(1), 89)
    ADD(LW(1), 30)
    SET_VALUE(LW(2), LW(1))
    SUBTRACT(LW(2), LW(5))
    CALL(btlevtcmd_AcSetParamAll, 5, LW(1), LW(2), LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_GREATER(LW(10), 0)
        CALL(btlevtcmd_AcSetFlag, AC_TIMING_WIDE_FLAG_NO_SLIDE_IN | AC_TIMING_WIDE_FLAG_HOLD_AFTER_RESULT)
    ELSE
        CALL(btlevtcmd_AcSetFlag, AC_TIMING_WIDE_FLAG_HOLD_AFTER_RESULT)
    END_IF
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 11, 1, 0)
    WAIT_FRAMES(22)
    IF_INT_GREATER(LW(10), 0)
        CALL(btlevtcmd_StartAC, 0)
    ELSE
        CALL(btlevtcmd_StartAC, 1)
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(0), 5)
        LOOP(LW(0))
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(_disp_heart_entry, 10, 20, 25, LW(3), LW(4))
                WAIT_FRAMES(1)
                BREAK_LOOP
            END_IF
            CALL(_disp_heart_entry_stop_check, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(0), 5)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(_disp_heart_entry_stop, LW(15))
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A4_2"))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_VIVIAN_MEROMERO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(6))
    MULTIPLY(LW(6), 45)
    CALL(evt_eff, 0, STRING("kiss"), 4, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_NOT_EQUAL(LW(6), PRECHECK_HIT)
        IF_INT_EQUAL(LW(6), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(50)
    END_IF
    IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_STATUS, LW(5))
    END_IF
    IF_INT_EQUAL(LW(5), CHECK_DAMAGE_HIT)
        IF_BITS_SET(LW(7), AC_RESULT_SUCCESS)
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            ADD(LW(2), 10)
            CALL(evt_eff, 0, STRING("kiss"), 5, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 30, 30, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_Y_1"))
        WAIT_FRAMES(32)
    END_IF
    WAIT_FRAMES(5)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_1"))
    WAIT_FRAMES(48)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(vivian_shadow_tail_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_X, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Y, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Z, LW(2))
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(5))
    IF_INT_EQUAL(LW(5), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, LW(5))
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_X, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Y, LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Z, LW(2))
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

static EVT_BEGIN(vivian_counter_damage_event)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SHOW_HP)
    IF_INT_EQUAL(LW(0), DAMAGE_CODE_COUNTER_TOP_SPIKY)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE_TOP_SPIKY)
    ELSE
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(11), LW(14))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_X, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Y, LW(1))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Z, LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, 0)
        CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ELSE
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(vivian_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_O_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_O_2"))
    WAIT_MS(333)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(333)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    WAIT_MS(500)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(3), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(4))
    SET_VALUE(LW(8), LW(3))
    MULTIPLY(LW(8), LW(4))
    MULTIPLY(LW(8), -1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(8), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_FRAMES(20)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_AUDIENCE)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PTR_A1_2"))
    CALL(btlevtcmd_ftomsec, 4, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK4"), 0)
    CALL(_attack_aud, LW(3), 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(6), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

USER_FUNC(battle_evt_majo_disp_off) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, partNo, setShadowFlag, hidePart;
    s32 pose;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partNo = evtGetValue(event, args[1]);
    setShadowFlag = evtGetValue(event, args[2]);
    hidePart = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partNo);
    pose = part->poseId;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        animEffectAsync("p_bibi", 1);
    }
    switch (event->userdata[0]) {
        case 0:
            part->shadowScale = intplGetValue(EASING_LINEAR, 1.0f, 2.0f, event->userdata[1], 30);
            if (setShadowFlag != 0) {
                unit->flags |= BATTLE_UNIT_FLAG_SHADOW_WITH_PARTNER;
            }
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                event->userdata[0]++;
            }
            break;
        case 1:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(EASING_LINEAR, 1.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (!animEffectAsync("p_bibi", 1)) {
                return EVT_RETURN_BLOCK;
            }
            animPoseSetEffect(pose, "p_bibi", 1);
            animPoseSetEffectAnim(pose, "PM_B_1", 1);
            animPoseSetLocalTime(animPoseGetAnimPosePtr(pose)->paperId, 8.0f);
            event->userdata[0]++;
            break;
        case 2:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(EASING_LINEAR, 1.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(pose)->paperId) >= 1.0f) {
                event->userdata[0]++;
                event->userdata[1] = 0;
                psndSFXOn_3D("SFX_BTL_VIVIAN_SHADOW1_2", &unit->position);
                if (hidePart != 0) {
                    part->attributes |= kHidden;
                }
            }
            break;
        case 3:
            part->shadowScale = intplGetValue(EASING_LINEAR, 2.0f, 0.0f, event->userdata[1], 20);
            event->userdata[1]++;
            if (event->userdata[1] > 20) {
                part->shadowScale = 0.0f;
                unit->position.y -= 500.0f;
                animPoseSetEffect(pose, NULL, 1);
                animPoseSetPaperAnimGroup(pose, NULL, 1);
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Allocates a spare BattleWeapon in the running unit's countersetWeapon slot,
 * which Shade Fist fills per use (_make_kagenuke_weapon). Run from init_event.
 */
static USER_FUNC(_vivian_make_extra_work_area) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(BattleWeapon));
    return EVT_RETURN_DONE;
}

/**
 * Builds Vivian's battle move list (data tag 1):
 * Shade Fist and Veil always, Fiery Jinx from Super rank and Infatuate from Ultra rank,
 * appending each to command->weapon and advancing *count.
 */
static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_VivianNormalAttack;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_VivianShadowGuard;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_VivianMagicalPowder;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_VivianCharmKissAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

/**
 * Returns in args[7] how many frames moving from point args[0..2] to args[3..5] takes at speed args[6],
 * capped at 15.
 */
static USER_FUNC(_get_move_frame) {
    EvtWord* args = event->args;
    Vec a, b, d;
    f32 speed;
    s32 frames, out;

    a.x = evtGetFloat(event, args[0]);
    a.y = evtGetFloat(event, args[1]);
    a.z = evtGetFloat(event, args[2]);
    b.x = evtGetFloat(event, args[3]);
    b.y = evtGetFloat(event, args[4]);
    b.z = evtGetFloat(event, args[5]);
    speed = evtGetFloat(event, args[6]);
    out = args[7];
    PSVECSubtract(&a, &b, &d);
    frames = (s32)((f32)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) / speed);
    if (frames > 15) {
        frames = 15;
    }
    evtSetValue(event, out, frames);
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps); placeholder unk_8017d03c, JP 0x8017D03C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Takes target unit args[0] and a position in args[1]-args[3]
 * and adjusts it in place to where Vivian should surface for Shade Fist:
 * 30 units behind her facing, plus the target's vivianOffsetX/Z, 20 units lower but not below the floor.
 * Not in the early demos.
 */
USER_FUNC(_get_vivian_attack_position) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* self;
    BattleUnit* target;
    s32 targetId, x, y, z;
    s32 xArg, yArg, zArg;
    s32 offsetX, offsetZ;
    f32 floor;
    f32 sx, sy, sz;

    targetId = evtGetValue(event, args[0]);
    xArg = args[1];
    x = evtGetValue(event, xArg);
    yArg = args[2];
    y = evtGetValue(event, yArg);
    zArg = args[3];
    z = evtGetValue(event, zArg);
    self = BattleGetUnitPtr(wp, BattleTransID(event, UNIT_ID_SELF));
    target = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    BtlUnit_GetScale(target, &sx, &sy, &sz);
    offsetX = (s32)((f32)target->faceDirection * (target->kindParams->vivianOffsetX * sx));
    offsetZ = (s32)(target->kindParams->vivianOffsetZ * sz);
    x += -30.0f * self->sizeMultiplier * (f32)self->faceDirection + offsetX;
    z = z + offsetZ;
    y -= 20.0f * self->sizeMultiplier;
    floor = BattleGetFloorHeight(wp, x, y, z);
    if (floor > y) {
        y = (s32)floor;
    }
    evtSetValue(event, xArg, x);
    evtSetValue(event, yArg, y);
    evtSetValue(event, zArg, z);
    return EVT_RETURN_DONE;
}
#endif

/**
 * Copies the weapon pointed to by args[0] into the unit's spare weapon,
 * sets its burn duration (burnTime) to args[1] turns, and writes the copy's address back into args[0].
 * Shade Fist always passes 2.
 */
static USER_FUNC(_make_kagenuke_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 weaponArg;
    BattleWeapon* weapon;
    void* src;
    s32 id, burnTime;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    weaponArg = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, weaponArg);
    burnTime = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->burnTime = burnTime;
    evtSetValue(event, weaponArg, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

/**
 * Shows Infatuate's heart over part args[4] of unit args[3] for args[2] frames,
 * popping in by frame args[0] and settling by frame args[1] (state kept in event->heart).
 * Ends early once _disp_heart_entry_stop is called.
 */
USER_FUNC(_disp_heart_entry) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    if (isFirstCall) {
        event->heart.unit = unit;
        event->heart.cur = 0;
        event->heart.start = evtGetValue(event, args[0]);
        event->heart.end = evtGetValue(event, args[1]);
        event->heart.total = evtGetValue(event, args[2]);
        event->heart.stop = 0;
        event->heart.unitId = evtGetValue(event, args[3]);
        event->heart.partId = evtGetValue(event, args[4]);
    }
    if (event->heart.stop != 0) {
        return EVT_RETURN_DONE;
    }
    event->heart.cur++;
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, _disp_heart, event);
    return event->heart.cur < event->heart.total ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/**
 * Display callback for _disp_heart_entry: draws the speech-bubble and heart icons at the target's hit position,
 * scaled through the pop-in animation.
 */
static void _disp_heart(CameraId camera, void* param) {
    EventEntry* w;
    f32 scale;
    Vec p, b, a;
    BattleUnit* unit;

    if (param == NULL) {
        return;
    }
    w = param;
    if (w->heart.unit != NULL) {
        if (w->heart.cur <= w->heart.start) {
            scale = intplGetValue(EASING_CUBIC_OUT, 0.0f, 1.2f, w->heart.cur, w->heart.start);
        } else if (w->heart.cur <= w->heart.end) {
            scale = intplGetValue(EASING_SINE_IN_OUT, 1.2f, 1.0f, w->heart.cur - w->heart.start, w->heart.end - w->heart.start);
        } else {
            scale = 1.0f;
        }
        unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(w, w->heart.unitId));
        BtlUnit_GetHitPos(unit, BtlUnit_GetPartsPtr(unit, w->heart.partId), &p.x, &p.y, &p.z);
        p.z += 10.0f;
        a = p;
        b = p;
        b.y = 16.0f * scale + a.y;
        b.z = 5.0f + a.z;
        iconDispGx(&a, 0, ICON_FUKIDASHI, scale);
        iconDispGx(&b, 0, ICON_ITEM_HEART, scale);
    }
}

/** Stops the heart display of the event whose id is args[0] (sets its heart.stop). */
USER_FUNC(_disp_heart_entry_stop) {
    EventEntry* e = evtGetPtrID(evtGetValue(event, *event->args));
    e->heart.stop = 1;
    return EVT_RETURN_DONE;
}

/** Returns in args[0] whether the running event's heart display has been stopped. */
static USER_FUNC(_disp_heart_entry_stop_check) {
    evtSetValue(event, *event->args, event->heart.stop);
    return EVT_RETURN_DONE;
}
