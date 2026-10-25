#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_audience.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "libc_decls.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "eff/n64/eff_kameki_tornade_n64.h"
#include "system.h"

#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "manager/evtmgr.h"
#include "dolphin/mtx.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "battle/battle_audience.h"
#include "evt/evt_cmd.h"
#include "eff/eff_kemu_test.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "pmario_sound.h"

typedef struct ArmWork { Vec prevPos; Vec pos; f32 rot; f32 targetRot; EffectEntry* eff; } ArmWork;
double atan(double x);
typedef struct BattenEffWork { s32 field_0x0; Vec pos; u8 field_0x10[0x28 - 0x10]; Mtx mtx; } BattenEffWork;
EffectEntry* effBattenEntry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale);
typedef struct TargetWork { s32 state; s32 count; EffectEntry* eff; } TargetWork;
typedef struct AudMember { s32 flags; s32 step; s32 counter; u8 pad_0C[0x48 - 0xC]; Vec position; u8 pad_54[0x60 - 0x54]; Vec scale; u8 pad_6C[0x9C - 0x6C]; Vec rotation; u8 pad_A8[0xB4 - 0xA8]; Vec home; u8 pad_C0[0xF8 - 0xC0]; s32 frame; s32 frames; } AudMember;
s32 irand(s32 scalar);
extern EvtWord aud_hit_event[];
void aud_get_pod_pos(BattleUnit* unit, f32* x, f32* y, f32* z);
void aud_fire_eff(f32 x, f32 y, f32 z);
void effKemuTestSetRxRz(EffectEntry* eff, f32 rx, f32 rz);
void psndSFX_vol(s32 id, u8 vol);
EffectEntry* eff_tor;
s32 torCount;
EffectEntry* eff_jet[2];
s32 jetCount;
static s32* list;
EffectEntry* eff_suikomi;
s32 screamId;
s32 scream_fade_timer;
u8 scream_fade_start_vol;
u8 scream_fade_vol;
TargetWork* wp_target;
s32 targetSoundId;
static EffectEntry* eff_charge;
static ArmWork* work[2];

static USER_FUNC(arm_check);
static USER_FUNC(arm_end);
static USER_FUNC(arm_init);
static USER_FUNC(arm_main);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(get_aud_num);
static USER_FUNC(waku_main);
static USER_FUNC(arm_init2);
static USER_FUNC(eff_dead_delete);
static USER_FUNC(eff_dead);
USER_FUNC(aud_target_mark);
USER_FUNC(aud_fire_no_hit);
USER_FUNC(aud_fire_hit);
USER_FUNC(aud_scream_fadeout);
USER_FUNC(aud_suikomi);
USER_FUNC(aud_jet_end);
USER_FUNC(aud_jet);
USER_FUNC(drill_jet);
USER_FUNC(drill_tor);

BattleUnitKind unit_boss_magnum_battender_mkII;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_no_arm[16];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[4];
BattleWeapon weapon_batten_drill_attack;
BattleWeapon weapon_batten_boomerang_attack;
BattleWeapon weapon_batten_aud_attack;
static BattleUnitSetup entry_rocket_punch;
EvtWord aud_hit_event[24];
EvtWord aud_no_hit_event[16];
static EvtWord init_event[92];
static EvtWord waku_event[23];
static EvtWord damage_event[111];
static EvtWord dead_event[234];
static EvtWord entry_event[78];
static EvtWord attack_event[320];
EvtWord drill_attack_event[392];
EvtWord boomerang_attack_event[324];
EvtWord aud_attack_event[823];
EvtWord aud_attack_event_sub[139];
static EvtWord arm_bunri_event[199];
static EvtWord arm_return_event[151];
static EvtWord wait_event[6];
BattleUnitKind unit_boss_rocket_punch_mkII;
static BattleUnitDefense defence_r;
static BattleUnitDefenseAttr defence_attr_r;
static StatusVulnerability regist_r;
static PoseTableEntry pose_table_r[14];
static BattleDataEntry data_table_r[1];
static BattleUnitKindPart parts_r[1];
static BattleWeapon weapon_batten_rocket_attack;
static EvtWord init_event_r[191];
static EvtWord arm_event[59];
static EvtWord arm_event_end[35];
static EvtWord damage_event_r[30];
static EvtWord attack_event_r[368];
static EvtWord wait_event_r[18];

BattleUnitKind unit_boss_magnum_battender_mkII = {
    121,
    "btl_un_magnum_battender_mkii",
    70,
    0,
    1,
    1,
    84,
    0,
    0,
    100,
    0,
    50,
    6,
    120,
    160,
    30,
    100,
    0.0f,
    80.0f,
    0.0f,
    0,
    0,
    30.0f,
    115.0f,
    0.0f,
    60.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    52.0f,
    0.0f,
    0.0f,
    30.0f,
    110.0f,
    20.0f,
    0.0f,
    80.0f,
    0.0f,
    120.0f,
    160.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_MBTN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    4,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 40, 40, 0, 40, 0, 100, 20, 0, 0, 100, 0, 100, 80, 40, 0, 0, 100, 30, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {1, "MGN_S_1"},
    {2, "MGN_2D_1"},
    {9, "MGN_2D_1"},
    {5, "MGN_X_1"},
    {4, "MGN_X_1"},
    {3, "MGN_X_1"},
    {28, "MGN_S_1"},
    {29, "MGN_Q_1"},
    {30, "MGN_Q_1"},
    {31, "MGN_S_1"},
    {39, "MGN_D_1"},
    {40, "MGN_W_1"},
    {41, "MGN_W_2"},
    {56, "MGN_S_1"},
    {57, "MGN_S_1"},
    {65, "MGN_T_1"},
    {69, "MGN_S_1"},
};

static PoseTableEntry pose_table_no_arm[] = {
    {1, "MGN_S_2"},
    {2, "MGN_2D_2"},
    {9, "MGN_2D_2"},
    {5, "MGN_X_2"},
    {4, "MGN_X_2"},
    {3, "MGN_X_2"},
    {28, "MGN_S_2"},
    {29, "MGN_Q_2"},
    {30, "MGN_Q_2"},
    {31, "MGN_S_2"},
    {39, "MGN_D_2"},
    {40, "MGN_W_3"},
    {56, "MGN_S_2"},
    {57, "MGN_S_2"},
    {65, "MGN_T_2"},
    {69, "MGN_S_2"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
#ifdef VERSION_G8ME01
    {1, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 37.0f, 103.0f, 30.0f, 37.0f, 113.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, 0x600019, 0, pose_table},
#else
    {1, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 37.0f, 103.0f, 30.0f, 37.0f, 113.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, 0x600009, 0, pose_table},
#endif
    {2, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 30.0f, 10.0f, 20.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, 0x3100002, 0, pose_table},
    {3, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, 0x12010000, 0, pose_table},
    {4, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
};

BattleWeapon weapon_batten_drill_attack = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
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

BattleWeapon weapon_batten_boomerang_attack = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
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

BattleWeapon weapon_batten_aud_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    0x2004, // targetWeightFlags
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

static BattleUnitSetup entry_rocket_punch = {
    &unit_boss_rocket_punch_mkII, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

EVT_BEGIN(aud_hit_event)
    CALL(evt_audience_move_position_frame, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(evt_audience_jump_position_gravity, LW(0), LW(6), LW(7), LW(8), LW(9), LW(10))
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

EVT_BEGIN(aud_no_hit_event)
    CALL(evt_audience_move_position_frame, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

static USER_FUNC(waku_main) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleUnitPart* waku;
    s32 poseId;
    AnimationPose* pose;
    s32 idx;
    f32 yaw;

    animGetPtr();
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    part = BtlUnit_GetPartsPtr(unit, 1);
    waku = BtlUnit_GetPartsPtr(unit, 3);
    poseId = part->poseId;
    animPoseWorldPositionEvalOn(poseId);
    animPoseGetAnimBaseDataPtr(poseId);
    pose = animPoseGetAnimPosePtr(poseId);
    idx = animPoseGetGroupIdx(poseId, "locator17");
    BtlUnit_SetPartsPos(waku, (*pose->worldPositions)[idx].x, (*pose->worldPositions)[idx].y,
                        (*pose->worldPositions)[idx].z);
    yaw = camGetPtr(CAMERA_3D)->viewYaw;
    BtlUnit_SetPartsRotate(waku, 0.0f, -yaw, 0.0f);
    return EVT_RETURN_DONE;
}

static USER_FUNC(arm_check) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 side = evtGetValue(event, args[0]);
    s32 found = -1;
    BattleUnit* unit;
    s32 i;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_X_FIST || unit->currentKind == UNIT_X_PUNCH)) {
            if (side == 0 && (unit->work[0] == 0 || unit->work[0] == 2)) {
                found = i;
                break;
            }
            if (side == 1 && (unit->work[0] == 1 || unit->work[0] == 3)) {
                found = i;
                break;
            }
        }
    }
    evtSetValue(event, args[1], found);
    return EVT_RETURN_DONE;
}

USER_FUNC(drill_tor) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        eff_tor = effKamekiTornadeN64Entry(0, 1000, 0.0f, -1000.0f, 0.0f, 3.5f);
        torCount = 0;
    }
    if (y <= 300.0f) {
        ((BattenEffWork*)eff_tor->userdata)->pos.x = x;
        ((BattenEffWork*)eff_tor->userdata)->pos.y = y - 240.0f;
        ((BattenEffWork*)eff_tor->userdata)->pos.z = z;
    }
    if (++torCount >= 70) {
        effDelete(eff_tor);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(drill_jet) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    BattenEffWork* ew;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        for (i = 0; i < 2; i++) {
            eff_jet[i] = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 3.0f);
        }
        jetCount = 0;
    }
    ew = eff_jet[0]->userdata;
    ew->pos.x = x - 20.0f;
    ew->pos.y = y;
    ew->pos.z = z - 20.0f;
    ew = eff_jet[1]->userdata;
    ew->pos.x = 20.0f + x;
    ew->pos.y = y;
    ew->pos.z = 20.0f + z;
    if (++jetCount >= 90) {
        for (i = 0; i < 2; i++) {
            effDelete(eff_jet[i]);
        }
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(aud_jet) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    Mtx mtx;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    BattenEffWork* ew;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        for (i = 0; i < 2; i++) {
            eff_jet[i] = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 3.0f);
        }
        jetCount = 0;
    }
    PSMTXRotRad(mtx, 'z', -0.34906584f);
    ew = eff_jet[0]->userdata;
    ew->pos.x = x - 60.0f;
    ew->pos.y = 22.5f + y;
    ew->pos.z = z;
    PSMTXCopy(mtx, ((BattenEffWork*)eff_jet[0]->userdata)->mtx);
    ew = eff_jet[1]->userdata;
    ew->pos.x = x - 15.0f;
    ew->pos.y = 22.5f + y;
    ew->pos.z = 70.0f + z;
    PSMTXCopy(mtx, ((BattenEffWork*)eff_jet[1]->userdata)->mtx);
    if (jetCount == 1) {
        for (i = 0; i < 2; i++) {
            effDelete(eff_jet[i]);
        }
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(aud_jet_end) {
    jetCount = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(get_aud_num) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], BattleAudience_GetAudienceNum());
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_suikomi) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    s32 done = 0;
    AudMember* mem;
    s32 i;
    s32 j;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    s32 ids[200];

    if (isFirstCall) {
        s32 n = 0;

        if (list != NULL) {
            BattleFree(list);
        }
        list = BattleAlloc(sizeof(s32) * 20);
        for (i = 0; i < 200; i++) {
            if ((u8)BattleAudience_GetExist(i) == TRUE) {
                ids[n] = i;
                n++;
            }
        }
        for (i = 0; i < 1000; i++) {
            s32 a = irand(n);
            s32 b = irand(n);
            s32 tmp = ids[a];

            ids[a] = ids[b];
            ids[b] = tmp;
        }
        for (i = 0; i < 20; i++) {
            mem = (AudMember*)BattleAudienceGetPtr(ids[i]);
            mem->flags |= 0x10;
            BattleAudience_SetAnim(ids[i], 9, FALSE);
            mem->frames = i * 3 + 60;
            mem->counter = 0;
            mem->rotation.y = 0.0f;
            list[i] = ids[i];
        }
        BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        eff_suikomi = effKumokumoN64Entry(2, 120, x - 10.0f, 10.0f + y, z, -40.0f, 1.0f, 1.5f);
        screamId = psndSFXOn("SFX_AUDIENCE_PANIC_SCREAM1");
    }
    BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x2, &y2, &z2);
    y2 += 5.0f;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 20; j++) {
            if (i == list[j]) {
                mem = (AudMember*)BattleAudienceGetPtr(i);
                mem->rotation.y += 19.0f;
                if (mem->frame < mem->frames) {
                    mem->frame++;
                } else {
                    done++;
                    mem->flags &= ~4;
                    mem->flags |= 2;
                }
                mem->position.x = intplGetValue(4, mem->home.x, x2, mem->frame, mem->frames);
                mem->position.y = intplGetValue(2, mem->home.y, y2, mem->frame, mem->frames);
                mem->position.z = intplGetValue(4, mem->home.z, z2, mem->frame, mem->frames);
                mem->scale.x = mem->scale.y = intplGetValue(4, 1.0f, 0.65f, mem->frame, mem->frames);
                break;
            }
        }
    }
    return done < 20 ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(aud_scream_fadeout) {
    if (isFirstCall) {
        scream_fade_timer = 120;
        scream_fade_start_vol = psndSFX_get_vol(screamId);
    }
    scream_fade_timer--;
    scream_fade_vol = intplGetValue(0, scream_fade_start_vol, 0.0f, 120 - scream_fade_timer, 120);
    psndSFX_vol(screamId, scream_fade_vol);
    if (scream_fade_timer <= 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

void aud_get_pod_pos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    BtlUnit_GetPos(unit, x, y, z);
    *x += 10.0f;
    *y += 78.0f;
}

void aud_fire_eff(f32 x, f32 y, f32 z) {
    effKemuTestSetRxRz(effKemuTestEntry(11, x - 50.0f, 20.0f + y, 50.0f + z, 0.75f), 40.0f, 110.0f);
}

USER_FUNC(aud_fire_hit) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 target = BattleTransID(event, evtGetValue(event, args[1]));
    s32 start = evtGetValue(event, args[2]);
    BattleUnit* unit;
    BattleUnitPart* part;
    AudMember* mem;
    EventEntry* evt;
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;

    if (isFirstCall) {
        aud_get_pod_pos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        aud_fire_eff(x, y, z);
        unit = BattleGetUnitPtr(_battleWorkPointer, target);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetHitPos(unit, part, &hx, &hy, &hz);
        for (i = start; i < start + 2; i++) {
            mem = (AudMember*)BattleAudienceGetPtr(list[i]);
            BattleAudience_SetAnim(list[i], 9, FALSE);
            mem->position = (Vec){x, y, z};
            mem->frames = 0;
            mem->counter = 0;
            mem->rotation.y = 0.0f;
            mem->scale.x = mem->scale.y = 1.0f;
            mem->flags |= 4;
            evt = evtEntry(aud_hit_event, 0, EVENT_START_IMMEDIATE);
            evt->lwData[0] = list[i];
            evt->lwData[1] = (s32)(1024.0f * hx) - EVTDAT_FLOAT_BASE;
            evt->lwData[2] = (s32)(1024.0f * hy) - EVTDAT_FLOAT_BASE;
            evt->lwData[3] = (s32)(1024.0f * hz) - EVTDAT_FLOAT_BASE;
            evt->lwData[4] = 0;
            evt->lwData[5] = irand(3) + 10;
            evt->lwData[6] = (s32)(1024.0f * (hx + 200.0f * rand() / 32767.0f - 100.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[7] = FLOAT(-100.0f);
            evt->lwData[8] = (s32)(1024.0f * (hz + 100.0f * rand() / 32767.0f - 50.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[9] = irand(30) + 30;
            evt->lwData[10] = FLOAT(-0.3f);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_fire_no_hit) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 target = BattleTransID(event, evtGetValue(event, args[1]));
    s32 start = evtGetValue(event, args[2]);
    BattleUnit* unit;
    BattleUnitPart* part;
    AudMember* mem;
    EventEntry* evt;
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;

    if (isFirstCall) {
        aud_get_pod_pos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        aud_fire_eff(x, y, z);
        unit = BattleGetUnitPtr(_battleWorkPointer, target);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetHitPos(unit, part, &hx, &hy, &hz);
        for (i = start; i < start + 2; i++) {
            mem = (AudMember*)BattleAudienceGetPtr(list[i]);
            BattleAudience_SetAnim(list[i], 9, FALSE);
            mem->position = (Vec){x, y, z};
            mem->frames = 0;
            mem->counter = 0;
            mem->rotation.y = 0.0f;
            mem->scale.x = mem->scale.y = 1.0f;
            mem->flags |= 4;
            evt = evtEntry(aud_hit_event, 0, EVENT_START_IMMEDIATE);
            evt->lwData[0] = list[i];
            evt->lwData[1] = FLOAT(-275.0f);
            evt->lwData[2] = (s32)(1024.0f * (hy - 20.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[3] = (s32)(1024.0f * hz) - EVTDAT_FLOAT_BASE;
            evt->lwData[4] = 0;
            evt->lwData[5] = irand(3) + 10;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(aud_target_mark) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    BattenEffWork* ew;

    if (isFirstCall) {
        wp_target = BattleAlloc(sizeof(TargetWork));
        wp_target->eff = effBattenEntry(1, 1000, 0.0f, -1000.0f, 0.0f, 1.0f);
        wp_target->state = 0;
        wp_target->count = 0;
        targetSoundId = psndSFXOn("SFX_BOSS_MBTN_SEARCH1");
    }
    ew = wp_target->eff->userdata;
    switch (wp_target->state) {
        case 0:
            wp_target->count++;
            ew->pos.x = intplGetValue(4, 60.0f + x, x - 5.0f, wp_target->count, 60);
            ew->pos.y = intplGetValue(4, 20.0f + y, 60.0f + y, wp_target->count, 60);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 60) {
                wp_target->state = 5;
                wp_target->count = 0;
            }
            break;
        case 5:
            wp_target->count++;
            ew->pos.x = intplGetValue(4, x - 5.0f, x - 25.0f, wp_target->count, 40);
            ew->pos.y = intplGetValue(4, 60.0f + y, 20.0f + y, wp_target->count, 40);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 40) {
                wp_target->state = 10;
                wp_target->count = 0;
            }
            break;
        case 10:
            wp_target->count++;
            ew->pos.x = intplGetValue(4, x - 25.0f, x, wp_target->count, 20);
            ew->pos.y = intplGetValue(4, 20.0f + y, y, wp_target->count, 20);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 20) {
                wp_target->state = 15;
                wp_target->count = 0;
                effDelete(wp_target->eff);
                wp_target->eff = effBattenEntry(2, 1000, x, y, 10.0f + z, 1.0f);
                psndSFXOff(targetSoundId);
                psndSFXOn("SFX_BOSS_MBTN_LOCKON1");
            }
            break;
        case 15:
            wp_target->count++;
            if (wp_target->count >= 90) {
                effDelete(wp_target->eff);
                BattleFree(wp_target);
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(eff_dead) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    eff_charge = effAkariChargeN64Entry(0, 1000, x, y, z, 1.5f);
    return EVT_RETURN_DONE;
}

static USER_FUNC(eff_dead_delete) {
    effDelete(eff_charge);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, -2, 16777216)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("MGN_L_1"))
    SPAWN_EVENT(PTR(waku_event))
    CALL(arm_init)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 5, 0)
    CALL(btlevtcmd_SetHomePos, -2, 125, 0, 0)
    CALL(btlevtcmd_SetPos, -2, 125, 0, 0)
    CALL(btlevtcmd_SetBaseRotate, -2, 0, -55, 0)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_MBTN_MOVE1"), STRING("SFX_BOSS_MBTN_MOVE1"), 26, 26, 26)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_MBTN_MOVE1"), STRING("SFX_BOSS_MBTN_MOVE1"), 20, 20, 20)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(waku_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(waku_main, -2)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamagePattern, -2, 10, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
    CALL(btlevtcmd_GetUnitWork, -2, 6, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 14)
            CALL(btlevtcmd_WaitAttackEnd)
            CALL(btlevtcmd_SetUnitWork, -2, 6, 1)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(evt_btl_camera_set_prilimit, 1)
            CALL(evt_btl_camera_set_mode, 1, 3)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 20)
            CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
            WAIT_FRAMES(20)
            CALL(btlevtcmd_SetTalkPoseType, -2, 65)
            CALL(btlevtcmd_SetStayPoseType, -2, 69)
            CALL(evt_msg_print, 2, STRING("stg7_aji_41_06"), 0, -2)
            CALL(evt_btl_camera_set_prilimit, 0)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            WAIT_MS(500)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, -2, 6, 1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 70)
    ADD(LW(2), 10)
    CALL(eff_dead, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 20)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetTalkPoseType, -2, 39)
    CALL(btlevtcmd_SetStayPoseType, -2, 39)
    CALL(evt_msg_print, 2, STRING("stg7_aji_41_07"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    WAIT_MS(500)
    CALL(arm_check, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_CheckDamageCode, LW(0), 512, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(10), LW(0))
            BEGIN_INLINE_EVENT
                CALL_CHILD(subsetevt_dead_core)
                CALL(arm_end, 0)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 16777216)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 33554432)
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(arm_check, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_CheckDamageCode, LW(0), 512, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(10), LW(0))
            BEGIN_INLINE_EVENT
                CALL_CHILD(subsetevt_dead_core)
                CALL(arm_end, 1)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 16777216)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, 33554432)
            END_INLINE_EVENT
        END_IF
    END_IF
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 50)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(120)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    CALL(eff_dead_delete)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 20)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, 2, STRING("stg7_aji_41_00"), 0, -2)
    CALL(evt_btl_camera_set_moveto, 1, -102, 83, 334, -102, 59, 0, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print_battle_party, STRING("stg7_aji_41_01"))
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    WAIT_MS(500)
    CALL(btlevtcmd_GetBodyId, -4, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, -4, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_check, 0, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(arm_check, 1, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_AddUnitWork, -2, 1, -1)
                CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
                IF_INT_LESS_OR_EQUAL(LW(0), 0)
                    CALL_CHILD(PTR(arm_return_event))
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(drill_attack_event))
            GOTO(99)
        END_IF
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_GREATER(LW(0), 63)
            CALL_CHILD(PTR(drill_attack_event))
            GOTO(99)
            ELSE
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL_CHILD(PTR(arm_bunri_event))
                GOTO(99)
            END_IF
            CALL(btlevtcmd_GetHp, -2, LW(0))
            IF_INT_GREATER(LW(0), 35)
                SET_VALUE(LW(0), 0)
                ADD(LW(0), 40)
                ADD(LW(0), 40)
                SUBTRACT(LW(0), 1)
                CALL(evt_sub_random, LW(0), LW(1))
                SET_VALUE(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(drill_attack_event))
                    GOTO(99)
                    ELSE
                    CALL_CHILD(PTR(arm_bunri_event))
                    GOTO(99)
                END_IF
                ELSE
                CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(get_aud_num, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                        CALL_CHILD(PTR(aud_attack_event))
                        GOTO(99)
                    END_IF
                END_IF
                LABEL(10)
                SET_VALUE(LW(0), 0)
                ADD(LW(0), 40)
                ADD(LW(0), 40)
                ADD(LW(0), 25)
                SUBTRACT(LW(0), 1)
                CALL(evt_sub_random, LW(0), LW(1))
                SET_VALUE(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(drill_attack_event))
                    GOTO(99)
                END_IF
                ADD(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(arm_bunri_event))
                    GOTO(99)
                END_IF
                ADD(LW(0), 25)
                IF_INT_LESS(LW(1), LW(0))
                    CALL(get_aud_num, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                        CALL_CHILD(PTR(aud_attack_event))
                        GOTO(99)
                    END_IF
                END_IF
                GOTO(10)
            END_IF
        END_IF
        ELSE
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_GREATER(LW(0), 35)
            CALL_CHILD(PTR(boomerang_attack_event))
            GOTO(99)
            ELSE
            CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(get_aud_num, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                    CALL_CHILD(PTR(aud_attack_event))
                    GOTO(99)
                END_IF
            END_IF
            LABEL(20)
            SET_VALUE(LW(0), 0)
            ADD(LW(0), 75)
            ADD(LW(0), 20)
            SUBTRACT(LW(0), 1)
            CALL(evt_sub_random, LW(0), LW(1))
            SET_VALUE(LW(0), 75)
            IF_INT_LESS(LW(1), LW(0))
                CALL_CHILD(PTR(boomerang_attack_event))
                GOTO(99)
            END_IF
            ADD(LW(0), 20)
            IF_INT_LESS(LW(1), LW(0))
                CALL(get_aud_num, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                    CALL_CHILD(PTR(aud_attack_event))
                    GOTO(99)
                END_IF
            END_IF
            GOTO(20)
        END_IF
    END_IF
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(drill_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_batten_drill_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_drill_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 5, 1)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(evt_btl_camera_set_prilimit, 1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(evt_msg_print, 2, STRING("stg7_aji_41_02"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        WAIT_MS(500)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_drill_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_batten_drill_attack))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_DRILL_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A3_1"))
    WAIT_MS(800)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_DRILL_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A3_2"))
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, -2)
    END_SIBLING_EVENT
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 1, 0, -1)
    WAIT_MS(300)
    CALL(btlevtcmd_SetPos, -2, -125, 500, 0)
    CALL(btlevtcmd_SetRotate, -2, 180, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A3_3"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_DRILL_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, -125, -200, 0, 60, 0, 0, 0, -1)
    END_SIBLING_EVENT
    BEGIN_INLINE_EVENT
        CALL(drill_tor, -2)
    END_INLINE_EVENT
    LOOP(0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), 200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 7, 0, 80, 0)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(97)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_drill_attack), 256, LW(5))
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
    GOTO(95)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_drill_attack))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_drill_attack), 256, LW(5))
    GOTO(95)
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(97)
    LOOP(0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), -200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, 400, LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 60, -1, 0)
    RETURN
EVT_END()

EVT_BEGIN(boomerang_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_batten_boomerang_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_boomerang_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_boomerang_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_batten_boomerang_attack))
    BEGIN_SIBLING_EVENT
        WAIT_MS(900)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_BOOME_DIVIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A6_1"))
    WAIT_MS(1160)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_BOOME_ROUND1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 70)
    ADD(LW(1), 70)
    CALL(btlevtcmd_SetPartsPos, -2, 4, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, -2, 4, 0, 55, 65)
    BEGIN_SIBLING_EVENT
        WAIT_MS(1600)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_BOOME_ACTIVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 4, STRING("MGN_A4_2"))
    CALL(btlevtcmd_OffPartsAttribute, -2, 4, 16777216)
    CALL(btlevtcmd_OffPartsAttribute, -2, 4, 33554432)
    BEGIN_SIBLING_EVENT
        WAIT_MS(2500)
        CALL(btlevtcmd_AnimeChangePose, -2, 4, STRING("MGN_A4_3"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPartsPos, -2, 4, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
    CALL(btlevtcmd_DivePartsPosition, -2, 4, LW(0), LW(1), LW(2), 80, -40, 0, 0, -1)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
    CALL(btlevtcmd_DivePartsPosition, -2, 4, LW(0), LW(1), LW(2), 80, 40, 0, 0, -1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_DivePartsPosition, -2, 4, -400, 120, LW(2), 40, -20, 1, 0, -1)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_BOOME_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(97)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_boomerang_attack), 256, LW(5))
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
    GOTO(95)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_boomerang_attack))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_boomerang_attack), 256, LW(5))
    GOTO(95)
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(97)
    WAIT_MS(700)
    CALL(btlevtcmd_OnPartsAttribute, -2, 4, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, 4, 33554432)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    RETURN
EVT_END()

EVT_BEGIN(aud_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_batten_aud_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_aud_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(evt_btl_camera_set_prilimit, 1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("MGN_T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("MGN_S_1"))
        ELSE
            CALL(btlevtcmd_SetTalkPose, -2, STRING("MGN_T_2"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("MGN_S_2"))
        END_IF
        CALL(evt_msg_print, 2, STRING("stg7_aji_41_04"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        WAIT_MS(500)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_aud_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_batten_aud_attack))
    WAIT_FRAMES(30)
    BEGIN_SIBLING_EVENT
        WAIT_MS(200)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOTOR_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(700)
        CALL(evt_snd_sfxoff, LW(15))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_1"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_1"))
    END_IF
    WAIT_MS(1600)
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_2"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 1, 0, -1)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, 0, 6)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_batstage_set_aud_dark, 0, 30, 0)
    BEGIN_SIBLING_EVENT
        CALL(aud_jet, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_7"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_7"))
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(2), 150)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, -2, LW(0), 25, LW(2), 60, 0, 4, 0, -1)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_3"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_3"))
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_INHALE1"), EVT_NULLPTR, 0, LW(15))
    CALL(aud_suikomi, -2)
    BEGIN_SIBLING_EVENT
        CALL(aud_scream_fadeout)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_7"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_7"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 1, 0, -1)
    CALL(aud_jet_end)
    WAIT_MS(300)
    CALL(evt_batstage_return_aud_dark_base, 30, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_2"))
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 4, 0, -1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOTOR_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(600)
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(400)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_CHEST_OPEN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_4"))
    END_IF
    WAIT_MS(1200)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 7, 1)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(evt_btl_camera_set_prilimit, 1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetTalkPose, -2, STRING("MGN_T_3"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("MGN_A2_8"))
        ELSE
            CALL(btlevtcmd_SetTalkPose, -2, STRING("MGN_T_4"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("MGN_A5_8"))
        END_IF
        CALL(evt_msg_print, 2, STRING("stg7_aji_41_05"), 0, -2)
        CALL(btlevtcmd_SetTalkPoseType, -2, 65)
        CALL(btlevtcmd_SetStayPoseType, -2, 69)
        CALL(evt_btl_camera_set_prilimit, 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        WAIT_MS(500)
    END_IF
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(aud_target_mark, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_5"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_5"))
    END_IF
    SET_VALUE(LW(10), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 8, 0)
    LOOP(10)
        WAIT_MS(250)
        CALL_CHILD(aud_attack_event_sub)
    END_LOOP
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), -10)
    ADD(LW(1), 78)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 180, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 170, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 185, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_8"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_8"))
    END_IF
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        WAIT_MS(100)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_CHEST_OPEN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A2_6"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A5_6"))
    END_IF
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    RETURN
EVT_END()

EVT_BEGIN(aud_attack_event_sub)
    IF_INT_EQUAL(LW(10), 9)
        CALL(btlevtcmd_GetUnitWork, -2, 8, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            GOTO(91)
        END_IF
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_MACHINEGUN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_aud_attack), 256, LW(5))
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
    SET_VALUE(LW(11), LW(10))
    MULTIPLY(LW(11), 2)
    CALL(aud_fire_no_hit, -2, LW(3), LW(11))
    GOTO(95)
    LABEL(91)
    SET_VALUE(LW(11), LW(10))
    MULTIPLY(LW(11), 2)
    CALL(aud_fire_hit, -2, LW(3), LW(11))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_aud_attack))
    IF_INT_EQUAL(LW(10), 9)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_aud_attack), 256, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_aud_attack), 0, LW(5))
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 8, 1)
    GOTO(95)
    LABEL(95)
    ADD(LW(10), 1)
    RETURN
EVT_END()

static EVT_BEGIN(arm_bunri_event)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(evt_btl_camera_set_prilimit, 1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(evt_msg_print, 2, STRING("stg7_aji_41_03"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, 0)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        WAIT_MS(500)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_1"))
    WAIT_MS(600)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_STOP1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_2"))
    WAIT_MS(500)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_3"))
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_IGNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 3, 0, 20, 0)
    WAIT_MS(100)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), 1, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, 69)
    CALL(btlevtcmd_SpawnUnit, LW(4), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(4), 0, 1)
    CALL(btlevtcmd_SetUnitWork, LW(4), 1, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, LW(4), 1, 69)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_no_arm))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(1500)
    RETURN
EVT_END()

static EVT_BEGIN(arm_return_event)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_6"))
    WAIT_MS(600)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, 2)
    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, 69)
    CALL(btlevtcmd_SpawnUnit, LW(4), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(4), 0, 3)
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, LW(4), 1, 69)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetPos, LW(3), 300, 40, -100)
        CALL(btlevtcmd_DivePosition, LW(3), 30, 100, -40, 30, -10, 0, 0, -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetPos, LW(4), 350, 40, 100)
    CALL(btlevtcmd_DivePosition, LW(4), 80, 100, 40, 30, -10, 0, 0, -1)
    CALL(arm_end, 0)
    CALL(arm_end, 1)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_KillUnit, LW(4), 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_IGNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 3, 0, 20, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_7"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_8"))
    WAIT_MS(600)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(500)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

BattleUnitKind unit_boss_rocket_punch_mkII = {
    122,
    "btl_un_rocket_punch_mkii",
    5,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    10,
    50,
    34,
    0,
    15,
    0.0f,
    0.0f,
    0.0f,
    0,
    -20,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    24.0f,
    16.0f,
    0.0f,
    -5.0f,
    0.0f,
    0.0f,
    50.0f,
    34.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_MBTN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist_r,
    1,
    parts_r,
    init_event_r,
    data_table_r,
};

static BattleUnitDefense defence_r = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr_r = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist_r = {0, 70, 70, 100, 70, 100, 100, 70, 0, 0, 100, 0, 100, 85, 70, 0, 100, 100, 70, 0, 0, 0};

static PoseTableEntry pose_table_r[] = {
    {1, "MGN_A1_9"},
    {2, "MGN_A1_4"},
    {9, "MGN_A1_4"},
    {5, "MGN_A1_9"},
    {4, "MGN_A1_9"},
    {3, "MGN_A1_9"},
    {28, "MGN_A1_9"},
    {29, "MGN_A1_9"},
    {30, "MGN_A1_9"},
    {31, "MGN_A1_9"},
    {39, "MGN_A1_9"},
    {56, "MGN_A1_9"},
    {57, "MGN_A1_9"},
    {69, "MGN_A1_9"},
};

static BattleDataEntry data_table_r[] = {
    {0, NULL},
};

static BattleUnitKindPart parts_r[] = {
    {1, "btl_un_rocket_punch_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 10.0f, 0.0f, 106, 174, 255, &defence_r, &defence_attr_r, 0x600009, 0x2, pose_table_r},
};

static BattleWeapon weapon_batten_rocket_attack = {
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
    {9, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
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

static EVT_BEGIN(init_event_r)
    CALL(btlevtcmd_OnUnitFlag, -2, 16777216)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_4"))
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event_r))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event_r))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event_r))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetBaseRotate, -2, 0, -90, 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_init2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_init2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_init2, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_init2, 1)
    END_IF
    SPAWN_EVENT(arm_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetPos, -2, 30, 100, -40)
        CALL(btlevtcmd_DivePosition, -2, -100, 150, -60, 20, 0, 4, 0, -1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, 350, 110, -80, 30, 50, 0, 0, -1)
        CALL(btlevtcmd_DivePosition, -2, 30, 60, -40, 25, -30, 4, 0, -1)
        CALL(btlevtcmd_SetHomePos, -2, 30, 60, -40)
        CALL(btlevtcmd_StartWaitEvent, -2)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPos, -2, 80, 100, 40)
        CALL(btlevtcmd_DivePosition, -2, -50, 150, 60, 20, 0, 4, 0, -1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, 300, 120, 80, 30, 50, 0, 0, -1)
        CALL(btlevtcmd_DivePosition, -2, 80, 60, 40, 30, -30, 4, 0, -1)
        CALL(btlevtcmd_SetHomePos, -2, 80, 60, 40)
        CALL(btlevtcmd_StartWaitEvent, -2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(arm_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL_CHILD(PTR(arm_event_end))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_main, -2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_main, -2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_main, -2, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_main, -2, 1)
    END_IF
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(arm_event_end)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_end, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_end, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_end, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_end, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_r)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckDamageCode, -2, 512, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -2, 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(arm_event_end))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_r)
    CALL(btlevtcmd_AddUnitWork, -2, 1, -1)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_batten_rocket_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_rocket_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(evt_sub_random, 1, LW(6))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_rocket_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_batten_rocket_attack))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MGN_A1_4"))
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_rocket_attack), 256, LW(5))
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
    GOTO(95)
    LABEL(91)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetPos, -2, LW(10), LW(11), LW(12))
            IF_INT_LESS_OR_EQUAL(LW(10), LW(0))
                CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_rocket_attack))
                CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_batten_rocket_attack), 256, LW(5))
                CALL(evt_btl_camera_shake_w, 0, 3, 0, 10, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    GOTO(95)
    LABEL(95)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 110)
        SET_VALUE(LW(1), 120)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, 10, 4, 0, -1)
        WAIT_FRAMES(5)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -20)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, -250, 0, LW(2), 20, -10, 0, 0, -1)
    ELSE
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -200)
        ADD(LW(1), 140)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, -80, 1, 0, -1)
    END_IF
    WAIT_MS(250)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_SetPos, -2, 300, LW(1), -100)
    ELSE
        CALL(btlevtcmd_SetPos, -2, 300, LW(1), 100)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, 0, 4, 0, -1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_r)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

static USER_FUNC(arm_init) {
    s32 i;

    for (i = 0; i < 2; i++) {
        work[i] = BattleAlloc(sizeof(ArmWork));
        work[i]->prevPos = work[i]->pos = (Vec){0.0f, 0.0f, 0.0f};
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(arm_init2) {
    s32 idx = evtGetValue(event, event->args[0]);
    ArmWork* arm = work[idx];

    arm->eff = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 1.0f);
    return EVT_RETURN_DONE;
}

static USER_FUNC(arm_main) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id = evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);
    ArmWork* arm = work[idx];
    BattenEffWork* eff;
    f32 x, y, z;
    f32 rx, ry, rz;
    f32 bx, by, bz;
    Mtx m;
    Mtx rot;
    Vec ofs;
    f32 dx, dy;

    if (arm->eff == NULL) {
        return EVT_RETURN_DONE;
    }
    eff = arm->eff->userdata;
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    arm->prevPos = arm->pos;
    {
        Vec p = {0.0f, 0.0f, 0.0f};

        p.x = x;
        p.y = y;
        p.z = z;
        arm->pos = p;
    }
    if (evtCheckID(unit->waitEventId) != TRUE) {
        dx = arm->pos.x - arm->prevPos.x;
        dy = arm->pos.y - arm->prevPos.y;
        if (dx != 0.0f) {
            arm->targetRot = 360.0f * (f32)atan(dy / dx) / 6.283184f;
            arm->rot += (arm->targetRot - arm->rot) / 3.0f;
            BtlUnit_GetRotate(unit, &rx, &ry, &rz);
            BtlUnit_SetRotate(unit, rx, ry, arm->rot);
        } else {
            arm->targetRot = 0.0f;
        }
    }
    BtlUnit_GetBaseRotate(unit, &bx, &by, &bz);
    BtlUnit_GetRotate(unit, &rx, &ry, &rz);
    rx += bx;
    ry += by;
    rz += bz;
    PSMTXRotRad(m, 'x', 1.5707964f);
    PSMTXRotRad(rot, 'y', 0.017453292f * ry);
    PSMTXConcat(rot, m, m);
    PSMTXRotRad(rot, 'x', 0.017453292f * rx);
    PSMTXConcat(rot, m, m);
    PSMTXRotRad(rot, 'z', 0.017453292f * rz);
    PSMTXConcat(rot, m, m);
    PSMTXCopy(m, eff->mtx);
    ofs = (Vec){0.0f, -18.0f, 0.0f};
    PSMTXMultVec(m, &ofs, &ofs);
    eff->pos.x = arm->pos.x + ofs.x;
    eff->pos.y = arm->pos.y + ofs.y;
    eff->pos.z = arm->pos.z + ofs.z;
    return EVT_RETURN_DONE;
}

static USER_FUNC(arm_end) {
    s32 idx = evtGetValue(event, event->args[0]);
    ArmWork* arm = work[idx];

    if (arm->eff != NULL) {
        effDelete(arm->eff);
        arm->eff = NULL;
    }
    return EVT_RETURN_DONE;
}
